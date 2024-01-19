/************************************************************

              


This file implements all the module entry points: 

int rtIdentify( P_IDENTITY_BLOCK* ppIdentityBlock );
int rtLoad(   UINT32 ScanRate, UINT32* rDirectCalls );
int rtOpen(   LPDRIVER_INST lpNet, P_ERR_PARAM lpErrors);
int rtReload( LPDRIVER_INST lpNet, P_ERR_PARAM lpErrors);
int rtOnLine( LPDRIVER_INST lpNet, P_ERR_PARAM lpErrors);
int rtInput(  LPDRIVER_INST lpNet);
int rtOutput( LPDRIVER_INST lpNet);
int rtSpecial(LPDRIVER_INST lpNet, LPSPECIAL_INST lpData);
int rtOffLine(LPDRIVER_INST lpNet, P_ERR_PARAM  lpErrors);
int rtClose(  LPDRIVER_INST lpNet, P_ERR_PARAM  lpErrors);
int rtUnload( );

**************************************************************/


#include "stdafx.h"


/*********************************************************/
/*                                                       */
/*                ******** Sample Program                */
/*                                                       */
/*********************************************************/ 
                 
#include <rt.h>

#include "vlcport.h"
#include "CSFlat.h"     // FCoreSup
#include "DCFlat.h"     // FDriverCore
#include "driver.h"
#include <string.h>     // strlen()
#include "version.h"
#include "auxrut.h"
#include "REMOTE.h"   // DUAL_PORT
#include "task.h"
#include "card.h"
#include "pcistuff.h"
#include "..\IE\Include\PlxApi.h"
#include "..\IE\Include\Plxioctl.h"

#include "type_def.h"
#include "hsl7851.h"
/************************************************************/

/*
    This code is provided here as an example on how to use 
    call back functions implemented in the csFlat.lib

*/
static int maxStation;
static int maxStation1;

extern slaveId[2][64];
extern MotionIdArray[2][64];


int rtIdentify( P_IDENTITY_BLOCK* ppIdentityBlock )
{
    static IDENTITY_BLOCK IdentityBlock; 
    IdentityBlock.DriverId   = DriverREMOTE;
    IdentityBlock.DriverVers = REMOTEVERS;
    IdentityBlock.pName      = PRODUCT_NAME ", " PRODUCT_VERSION;
    *ppIdentityBlock = &IdentityBlock;
    return 0;
}

int rtLoad( UINT32 ScanRate, UINT32* rDirectCalls )
{
    // Executing the LOAD PROJECT command

    #if defined( _DEBUG )
        SetDebuggingFlag( 1 );  // Disable the VLC watchdog, so we can step through our code. 
    #endif  // _DEBUG
 

    // Use direct calls for very fast applications.  
    // With the appropriate bit set, Input(), Output() and/or Special()
    //  can be directly called from the engine thread, 
    //  saving the delay introduced by a task switch. 
    // Note:  Functions exectuted in the engine thread cannot call 
    //  some C stdlib APIs, like sprintf(), malloc(), ...
    
    // *rDirectCalls = ( DIRECT_INPUT | DIRECT_OUTPUT | DIRECT_SPECIAL );

  //  S16               DeviceSelected;
  //  PLX_STATUS        rc;
  //  PLX_DEVICE_KEY    DeviceKey;

    EROP( "rtLoad() ScanRate=%d, rDirectCalls=%x", ScanRate, *rDirectCalls, 0, 0 );

    return 0;
}

int rtOpen( LPDRIVER_INST pNet, P_ERR_PARAM pErrors)
{
    // Executing the LOAD PROJECT command

    int rc = SUCCESS;
	U16 bn; // cRes ;
	U8 live_data;
//	I16 type, count;

	I16 IsHead ;
	I16 HeadArddr;

	int i;

    LPDEVICE_INST pDevice;

	for(i=0; i<64; i++)
	{
			slaveId[0][i] = 0;
			MotionIdArray[0][i] = 0;
			slaveId[1][i] = 0;
			MotionIdArray[1][i] = 0;
	}

//	  printf( "rtOpen \n");

	
	bn=pNet->PciIndex -1;
	maxStation = 0;
	maxStation1 = 0;
	IsHead= 0;
	HeadArddr=0;

    if( pNet->Sentinel != RT3_SENTINEL )
        rc = IDS_VLCRTERR_ALIGNMENT;

    if( rc == SUCCESS )
    {
        UINT32* pSentinel = BuildUiotPointer( pNet->ofsSentinel );
        if( *pSentinel != RT3_SENTINEL )
            rc = IDS_VLCRTERR_ALIGNMENT;
    }

    EROP( "rtOpen() pNet=%p, pErrors=%p", pNet, pErrors, 0, 0 );

    if( rc == SUCCESS )
	{

	//	                U32* pZero;

        pNet->pDeviceList = BuildUiotPointer( pNet->ofsDeviceList );
		for( pDevice = pNet->pDeviceList; pDevice->Type && ( rc == SUCCESS ) ; pDevice++ )
		{
			if( pDevice->Sentinel != RT3_SENTINEL )
				rc = IDS_VLCRTERR_ALIGNMENT;
			else
			{
				if ((pDevice->Address > maxStation) && (pDevice->ChannelID == 0))
					maxStation = pDevice->Address ;
				if ((pDevice->Address > maxStation) && (pDevice->ChannelID == 1))
					maxStation1 = pDevice->Address ;


                // Create UIOT pointers
				pDevice->pName = BuildUiotPointer( pDevice->ofsName );
                if ( pDevice->Input.bUsed )  
                    pDevice->Input.pDst  = BuildUiotPointer( pDevice->Input.ofsUiot );
                if ( pDevice->Output.bUsed ) 
					pDevice->Output.pDst    = BuildUiotPointer( pDevice->Output.ofsUiot ); //was pDst
   
			}
		}
	}

 if( !pNet->bSimulate )
{
   if( rc == SUCCESS )
        rc = Init( pNet, pErrors );

	bn = pNet->PciIndex -1;

		 
	if (rc == SUCCESS) 
		rc = HSL_Init(bn,0, (U8)maxStation,	pNet->TransferRate , (U8)pNet->MaxNodeAddr );/// W_REMOTE_Start(bn, 0,(U16)maxStation);

	live_data= rc;

        if( rc == SUCCESS )
        {

		for( pDevice = pNet->pDeviceList; pDevice->Type && ( rc == SUCCESS ) ; pDevice++ )
            {
		IsHead = 0;
		HeadArddr =0;

		if( pDevice->Type == DEVICE_4X_MOTION ) 
		{
			IsHead= GetMotionSw_id(bn, (U16)pDevice->ChannelID, (U16)pDevice->Address, &HeadArddr); //, &slaveId[0], &MotionId[0]);
			// printf("motion card found");

			if (IsHead == 1 )	pDevice->motionHead = 1;

///				REMOTE_get_slave_ID_type(bn, (U16)pDevice->ChannelID, 1, &type, &count);
	//			pDevice->AdlinkId =		Module_Type[card_ID][connect_index][pDevice->Address];/// = temp_id;
			}

		}



		for( pDevice = pNet->pDeviceList; pDevice->Type && ( rc == SUCCESS ) ; pDevice++ )
            {


	//		if( pDevice->Type == DEVICE_4X_MOTION ) 
	//		{
	//			pDevice->motionAddress = MotionIdArray[pDevice->ChannelID][pDevice->Address];
	//			pDevice->Critical = 1; ///all motion modules default critical
	//		}

			if( pDevice->Input.bUsed )
			{ 

		
			W_REMOTE_Slave_Live( bn, (U16)pDevice->ChannelID, (U16)pDevice->Address, &live_data);
	
			if (pDevice->Critical == 0) live_data =1;


			if (live_data == 1 ) rc = SUCCESS;
			else
			rc = IDS_REMOTE_DEVICE_OFFLINE;

			
			/// module there and also if it is motion type then
			if( pDevice->Type == DEVICE_4X_MOTION )  /// && ( rc == SUCCESS )
			{
				/// init motion if ok

			}



			}  //if

		} /// for

        
		}  //if


 //   if( rc == SUCCESS )
  //  rc = CreateBackgroundTask(pNet);
}
	      return rc;
}

int rtReload( LPDRIVER_INST pNet, P_ERR_PARAM pErrors)
{
	int rc = SUCCESS;
	U16 bn = pNet->PciIndex -1;

//	    LPDEVICE_INST pDevice;


    // Executing the LOAD PROJECT command
    EROP( "rtReload() pNet=%p, pErrors=%p", pNet, pErrors, 0, 0);
    if( !pNet->bSimulate )
    {
		rc = HSL_auto_start(bn,0, (U8)maxStation,	pNet->TransferRate, (U8)pNet->MaxNodeAddr);/// W_REMOTE_Start(bn, 0,(U16)maxStation);

    }

    // make sure pNet is in the same state as after rtOpen(). 
    return 0;
}

int rtOnLine( LPDRIVER_INST pNet, P_ERR_PARAM pErrors)
{
    int	rc = SUCCESS;
	U16 bn = pNet->PciIndex -1;

	EROP( "rtOnLine() pNet=%p, pErrors=%p", pNet, pErrors, 0, 0 );
    pNet->bFirstCycle = 1;
    pNet->bGoOffLine  = 0;

    if( !pNet->bSimulate )
    {
		rc = HSL_auto_start(bn,0, (U8)maxStation,	pNet->TransferRate, (U8)pNet->MaxNodeAddr);

    }

	EROP( "rtOnLine(). exit.", 0, 0, 0, 0 );

	
    return rc;

}


int rtInput( LPDRIVER_INST pNet ) 
{
	EROP( "rtInput() pNet=%p", pNet, 0, 0, 0 );
    // This is the beginning of the VLC scan cycle
 
	
	if( !pNet->bSimulate )
    {
		// Copy new input data from the hw to the driver input image in the UIOT. 
		LPDEVICE_INST pDevice = pNet->pDeviceList;

		for( ; pDevice->Type ; pDevice++ )
		{
			if( pDevice->Input.bUsed ) 
				REMOTE_ReadIO(pDevice, pNet->PciIndex -1, pDevice->Input.pDst );

		}

      //  VerifyDoneList(&pNet->Done);    // Flush the completed background functions
    }

	EROP( "rtInput(). exit", 0, 0, 0, 0 );

    return SUCCESS;
}



int rtOutput( LPDRIVER_INST pNet)
{

	// This is the end of the VLC scan cycle
    if( !pNet->bSimulate )
    {
        // Copy new output data from the UIOT driver output image to the hw.
		LPDEVICE_INST pDevice = pNet->pDeviceList;
    
		for( ; pDevice->Type ; pDevice++ )
            if( pDevice->Output.bUsed ) 
			{
				
			U32 *pDst =   pDevice->Output.pDst;
			U32 Data = (U32) *(pDst);

        if( pNet->bFirstCycle )   
			REMOTE_WriteIO(pDevice, pNet->PciIndex -1, 0L);
		else
			REMOTE_WriteIO(pDevice, pNet->PciIndex -1, Data);


			}

    }

        if( pNet->bFirstCycle )     // first Output() ?
        {
            //  Only now we have a valid output image in the DPR. 
            //    EnableOutputs(dp);  enable outputs (if our hardware lets us) 
            
            pNet->bFirstCycle = 0;
        }       

    EROP( "rtOutput() pNet=%p", pNet, 0, 0, 0 );

    return SUCCESS;
}

int rtSpecial( LPDRIVER_INST pNet, LPSPECIAL_INST pData)
{
    // A trapeziodal block has been hit.

    UINT16  Result = 0;
    UINT16  Status = VLCFNCSTAT_OK;

	SPECIAL_INST_COMMAND* const pUser = &pData->User.paramCommand;
	SPECIAL_INST_SETGET*  const paramSetGet = &pData->User.paramSetGet ;
	SPECIAL_INST_OTHERS*  const paramOthers =&pData->User.paramOthers ;


    EROP( "rtSpecial() pNet=%p, pData=%p", pNet, pData, 0, 0 );

//printf("User addr :%x, paramCommand : %x \n", pUser->Address ,pUser->Slave_ChannelID  );
//printf("addr :%x, paramOthers : %x \n",paramOthers->Address , paramOthers->Slave_ChannelID  );
//printf("addr :%x, paramSetGet : %x \n",paramSetGet->Address  , paramSetGet->Slave_ChannelID );

//printf("fun:%x \n", pData->User.paramHeader.FunctionId);


    if( !pNet->bSimulate )
    {
        int  FunctionId = pData->User.paramHeader.FunctionId;
        switch( FunctionId ) 
        {
            case DRVF_MOTION:
	//		Status=	AdLinkInterpol( pNet, pData );
					break;
            case DRVF_MOTION_INTERP:
	///		Status=	AdLinkInterpol( pNet, pData );
					break;
			case DRVF_SETGET:
					break;
            case DRVF_OTHERS_INT:
	//		Status=	AdLinkI16( pNet, pData );
				break;
            case DRVF_OTHERS_F64:
	//		Status=	AdLinkF64( pNet, pData );

				break;
            case DRVF_ONFLY:
					break;
         	default:
                    Status = VLCFNCSTAT_WRONGPARAM;
                    break;
        }
    
        EROP("Special();  FunId= %d, Status= %d, pData= %p", FunctionId, Status, pData, 0);
    }
    else
    {
		UINT16* pResult = BuildUiotPointer( pData->User.paramHeader.ofsResult );
        if( pResult )   // some functions may not have the Result param implemented
		    *pResult = (UINT32) SUCCESS;

        Status = VLCFNCSTAT_SIMULATE;
    }

    if( pData->User.paramHeader.ofsStatus )   // some functions may not have the status param implemented
	{
		UINT16* pStatus = BuildUiotPointer( pData->User.paramHeader.ofsStatus );
		*pStatus = Status;
	}
    
    return SUCCESS;
}

int rtOffLine( LPDRIVER_INST pNet, P_ERR_PARAM pErrors)
{
    // Executing the STOP PROJECT command
    int rc = SUCCESS;
	LPDEVICE_INST pDevice = pNet->pDeviceList;

    EROP( "rtOffLine() pNet=%p, pErrors=%p", pNet, pErrors, 0, 0 );

    if( !pNet->bSimulate )
    {

	rc =HSL_stop((U16)(pNet->PciIndex -1),0);

    pNet->bGoOffLine = 1;

        rc = WaitForAllFunctionCompletion(pNet);  /* wait for the backgroung task to calm down */
        
        if( rc == SUCCESS )
        {
 		HSL_stop((U16)(pNet->PciIndex -1),0);
		
		/*
            DUAL_PORT far *  dp  = (DUAL_PORT far *)pNet->pDpr;
            if( pNet->StopState == 1 )
                rc = stop scanning;
   
            DisableOutputs(dp, &pNet->trans_count);
            DisableWD(dp); 
            */
        }
        
    }    

    EROP("rtOffLine(). exit  rc= %d", rc, 0, 0, 0);

    return rc;
}

/*   if Open() fails, Close() is not automatically called for this instance.
     if lpErrors == NULL, do not report any error, keep the Open()'s error code and params.  
 */ 
int rtClose( LPDRIVER_INST pNet, P_ERR_PARAM pErrors)
{
    int rc = SUCCESS;

	U16 bn;
		LPDEVICE_INST pDevice = pNet->pDeviceList;

    // Executing the UNLOAD PROJECT command
    if( !pNet->bSimulate )
    {
        EROP("rtClose(). start. pNet= %p", pNet, 0, 0, 0);
        /*
        {
            DUAL_PORT far* const dp = (DUAL_PORT far *)pNet->pDpr;     / * pointer to the dualport * /
            Reset the board;
        }
        */
        
        //DeleteInterruptTask( pNet );
        DeleteBackgroundTask( pNet );
    
		bn= (pNet->PciIndex)-1 ;

		HSL_stop((U16)(pNet->PciIndex -1),0);


		FreePlxDpr(bn);


		if( pNet->pDpr )
        {
            FreeDpr( pNet->pDpr );
            pNet->pDpr = NULL;
        }

    }

    EROP( "rtClose() pNet=%p, pErrors=%p", pNet, pErrors, 0, 0 );
    return SUCCESS;
}

int rtUnload()
{
    // Executing the UNLOAD PROJECT command
    EROP( "rtUnload()", 0,0,0,0 );
    return 0;
}

