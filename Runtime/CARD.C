/********************************************************************

                                Card.c

    Interface specific code. 
    This file only should touch the hardware.

*********************************************************************/


#include "stdafx.h"

#include <rt.h>
#include <string.h>     // strlen()
#include <stdio.h>      // sprintf()
#include <time.h>
#include <string.h>     // strlen()

#include "vlcport.h"
#include "dcflat.h"     // EROP()
#include "driver.h"     /* SEMAPHORE */
#include "errors.h"     /* IDS_RT_DP_RW_TEST                     */
#include "auxrut.h"     /* StartTimeout(), IsTimeout(), EROP     */
#include "REMOTE.h"   /* DUAL_PORT                             */
#include "card.h"       /* Init()    */
#include "pcistuff.h"
#include "type_def.h"

#include "..\IE\Include\PlxApi.h"
#include "..\IE\Include\Plxioctl.h"

#include "hsl7851.h"
#include "Global.h"

#define SIZE_BUFFER					0x008
#define MAX_DEVICES_TO_LIST			100

#define SIZE_BAR_DATA				0x008

///#include "REMOTE_motion.h"       /* Init()   */  
#include  <rtbase.h>                          

/******************* Card specific  Functions  *******************************/


/******************* Initialization  *******************************/
extern slaveId[2][64];
extern MotionIdArray[2][64];
extern PCI_Info pci_Info[MAX_PCI_CARDS];

static int TestAndFill(UINT8* pc, const int Size, const int test, const int fill)   /* test == 1  --> no test */
{
    int i  = 0;
    for(; i < Size;  *pc++ = fill, i++)
    {
        int c = *pc & 255;
        if(test != 1  &&  test != c)
        {
            EROP("Ram Error.  Address %p, is 0x%02x, and should be 0x%02x", pc, c, test, 0);
            return IDS_REMOTE_HW_TEST;
        }
    }
    return SUCCESS;
}


int  Init( LPDRIVER_INST pNet, P_ERR_PARAM const lpErrors)
{
    int rc = SUCCESS;

	rc = InitPCI(pNet,lpErrors);

//	W_REMOTE_Initial(cardidx);
    return rc;
}



/****************************************************************************************
    IN:     pName   --> pointer to the device user name
            Address --> device's network address
            pBuf    --> pointer to the destination buffer
            szBuf   --> size of the destination buffer

    OUT:    *pBuf   <-- "Address xx (usr_name)".  
    Note:   The device user name may be truncated!
*/
static void LoadDeviceName( char* pName, UINT16 Address, char* pBuf, size_t szBuf )
{
    if( szBuf && (pBuf != NULL) )
    {
        char* format = "Address %d";

        *pBuf = '\0';
				RtSleep(50);

        if( szBuf > (strlen(format)+3) )    /* Address may need more digits */
        {
            size_t  len;

            sprintf(pBuf, format, Address & 0xffff);

            len = strlen( pBuf ); 

            if( pName && ((szBuf - len) > 10) )     /* if we still have 10 free bytes  */
            {
                strcat(pBuf, " (");
                len += 2;
                strncat( pBuf, pName, szBuf-len-2 );
                *(pBuf + szBuf - 2) = '\0';
                strcat( pBuf, ")" );
            }
        }
    }
}



int  TestConfig( LPDRIVER_INST const pNet, P_ERR_PARAM const lpErrors )
{
    int rc = SUCCESS;

    LPDEVICE_INST pDevice = (LPDEVICE_INST)pNet->pDeviceList;
        
    for( ; pDevice->Type && (rc == SUCCESS); pDevice++ )
    {
        
		// TO DO:
//		DUAL_PORT* const dp = NULL; // (DUAL_PORT*)pNet->pDpr;     /* pointer to the dualport */

//        pDevice->bPresent = 1;

        /*
        Check pDevice. 
        if( the device is not on the network )
            pDevice->bPresent = 0;
        */

		/*
		printf( "TO DO File=%s, line=%d \n" __FILE__, __LINE__ );
        
        if( !pDevice->bPresent)
        {
            LoadDeviceName( pDevice->pName, pDevice->Address, lpErrors->Param3, sizeof(lpErrors->Param3) );
            rc = IDS_ _DEVICE_OFFLINE; 
        }
		*/
    }
				RtSleep(50);

    return rc;
}





int REMOTE_ReadIO( LPDEVICE_INST const pDevice, unsigned short  CardId , VOID *Dest )
{

				U16 ChannelId	= pDevice->ChannelID ;
				int cRes =0 ;
				int cRes2 = 0;
//				U8 live_data;
//				U8 stat;

				U32 ValueU32=0;
				U16 ValueU16=0;
				U8  ValueU8 =0;

				I16 ValueI16 = 0;

				UINT16 ValueGet16 = 0;
				I32	ValuePos =0 ;
				float	ValuePos32=0;
				double ValuePosF64 = 0;

				I16 i=0;

				U16 addr = pDevice->Address;



//W_REMOTE_Slave_Live( CardId, ChannelId, addr, &live_data); //active

//W_REMOTE_Connect_Status( CardId, ChannelId, addr, &stat); //status


	switch( pDevice->Type )
	{
    case DEVICE_4W_INPUT:

		ValueU32 = 0;
		ValueU16 = 0;
		cRes=  read_channel_input16( CardId, ChannelId  , addr, &ValueU16);
		ValueU32 = ValueU16 & 0xFFFF;		//// low 
		ValueU16 = 0;		
		cRes=  read_channel_input16( CardId, ChannelId  , addr+1, &ValueU16);
		ValueU32 = ValueU32 + (ValueU16 <<16);	//// hi
		
		if (cRes == ERR_No_Error )
		*((UINT32  volatile*)Dest) = ValueU32;

		ValueU32 = 0;
		ValueU16 = 0;
		cRes=  read_channel_input16( CardId, ChannelId  , addr+2, &ValueU16);
		ValueU32 = ValueU16 & 0xFFFF;		//// low 
		ValueU16 = 0;	
		cRes=  read_channel_input16( CardId, ChannelId  , addr+3, &ValueU16);
		ValueU32 = ValueU32 + (ValueU16 <<16);	//// hi
		
		if (cRes == ERR_No_Error )
		*((UINT32  volatile*)Dest+1) = ValueU32;

		break;

	case DEVICE_2W_INPUT:
		ValueU32 = 0;
		ValueU16 = 0;
		cRes=  read_channel_input16( CardId, ChannelId  , addr, &ValueU16);
		ValueU32 = ValueU16 & 0xFFFF;		//// low 
	//	printf("data %x --- %x \n",ValueU32,ValueU16);
		ValueU16 = 0;		


		cRes=  read_channel_input16( CardId, ChannelId  , addr+1, &ValueU16);
		ValueU32 = ValueU32 + (ValueU16 <<16);	//// hi
		
		if (cRes == ERR_No_Error )
		*((UINT32  volatile*)Dest) = ValueU32;

	
		break;
		case DEVICE_1W_INPUT:
		case DEVICE_1W_IANDO:
		ValueU16 = 0;
		cRes=  read_channel_input16( CardId, ChannelId  , addr, &ValueU16);
		if (cRes == ERR_No_Error )
		*((UINT16  volatile*)Dest) = ValueU16;

			break;
		case DEVICE_1B_INPUT:
		case DEVICE_1B_IANDO:
		ValueU8=0;
		cRes=  read_channel_input8( CardId, ChannelId  , addr, &ValueU8);
		if (cRes == ERR_No_Error )
		*((UINT8  volatile*)Dest) = ValueU8;

//		*((UINT8  volatile*)Dest+1) = cRes; //stat;
//		*((UINT8  volatile*)Dest+2) = live_data;

			break;
		case DEVICE_4X_MOTION:
//		*((UINT8  volatile*)Dest+20) = stat;
//		*((UINT8  volatile*)Dest+21) = live_data;


		addr = MotionIdArray[ChannelId][pDevice->Address ] ;
//printf("REMOTE read slv addr : %x, mot addr:%x ,motionAddress=%x \n",pDevice->Address ,addr, pDevice->motionAddress); 

//-		ValueI16 = REMOTE_M_motion_done(CardId, ChannelId  , addr);  
		*((UINT16  volatile*)Dest+9) = (UINT16)ValueI16;

//-		cRes= REMOTE_M_get_io_status(CardId, ChannelId  , addr, &ValueGet16); 
		if (cRes == ERR_No_Error)
		*((UINT16  volatile*)Dest+8) = (UINT16) ValueGet16 ;

//-		cRes= REMOTE_M_get_command(CardId, ChannelId  , addr, &ValuePos);  /// current pos
		if (cRes == ERR_No_Error)
		*((UINT32  volatile*)Dest+0) = (UINT32)ValuePos;
	
//-		cRes= REMOTE_M_get_position(CardId, ChannelId  , addr, &ValuePosF64); 
		ValuePos32=ValuePosF64;	// convert to 32 bits
		if (cRes == ERR_No_Error)
		*((UINT32  volatile*)Dest+1) = (UINT32)ValuePos32;

//-		cRes= REMOTE_M_get_error_counter(CardId, ChannelId  , addr,&ValueGet16); ///error cnt
		if (cRes == ERR_No_Error)
		*((UINT32  volatile*)Dest+2) = (UINT32)ValueGet16;

//-		cRes= REMOTE_M_get_general_counter(CardId, ChannelId  , addr, &ValuePosF64); ///general cnt

		ValuePos32=ValuePosF64;	// convert to 32 bits
		if (cRes == ERR_No_Error)

		*((UINT32  volatile*)Dest+3) = (UINT32)ValuePos32;

	}

return 0;


}


int REMOTE_WriteIO( LPDEVICE_INST const pDevice, unsigned short  CardId , unsigned long data )
{

				U16 ChannelId	= pDevice->ChannelID ;
				int cRes =0 ;
				int cRes2 = 0;

				U32 ValueU32=0;
				U16 ValueU16=0;
				U8  ValueU8 =0;

				U16 addr = pDevice->Address;

				U32 *pSrc =   pDevice->Output.pDst;
			
				U32 Data1 = (U32) *(pSrc+1);

				
	switch( pDevice->Type )
	{

        case DEVICE_4W_OUTPUT:

		ValueU32 = (data  & 0x0000FFFF) ;
		cRes=  write_channel_output(CardId, ChannelId , addr,ValueU32 );

		ValueU32 = (data  & 0xFFFF0000) >> 16;
		cRes=  write_channel_output(CardId, ChannelId , addr+1,ValueU32 );

		ValueU32 = (Data1  & 0x0000FFFF) ;
		cRes=  write_channel_output(CardId, ChannelId , addr+2,ValueU32 );

		ValueU32 = (Data1  & 0xFFFF0000) >> 16;
		cRes=  write_channel_output(CardId, ChannelId , addr+3,ValueU32 );

		
			break;

		case DEVICE_2W_OUTPUT:

		ValueU32 = (data  & 0x0000FFFF) ;
		cRes=  write_channel_output(CardId, ChannelId , addr,ValueU32 );

		ValueU32 = (data  & 0xFFFF0000) >> 16;
		cRes=  write_channel_output(CardId, ChannelId , addr+1,ValueU32 );

			break;


		case DEVICE_1W_OUTPUT:
		ValueU32 = (data  & 0x0000FFFF) ;
		cRes=  write_channel_output(CardId, ChannelId , addr,ValueU32 );

			break;
		case DEVICE_1B_OUTPUT:

		ValueU32 = (data  & 0x00FF) ;
		cRes=  write_channel_output(CardId, ChannelId , addr,ValueU32);

			break;
		case DEVICE_1B_IANDO:
		ValueU32 = (data  & 0x00FF) ;
		cRes=  write_channel_output(CardId, ChannelId , addr,ValueU32 );


			break;
		case DEVICE_1W_IANDO:
		ValueU32 = (data  & 0x0000FFFF) ;
		cRes=  write_channel_output(CardId, ChannelId , addr,ValueU32 );

			break;

	}

return 0;


}

void AdLinkMotion( const LPDRIVER_INST pNet, SPECIAL_INST* const pData )
{
    int rc = SUCCESS ;

//	UINT16 axis;
		
	I16 iret=0;
    SPECIAL_INST_COMMAND* const pUser = &pData->User.paramCommand;

    UINT16* pResult = BuildUiotPointer( pData->User.paramHeader.ofsResult );
 	*pResult=rc;	

//	UINT16* pStatus = BuildUiotPointer( pData->User.paramHeader.ofsStatus );
//	*pStatus = 1;


//	axis= MotionId[pUser->Address]; // axis number

	iret= -1;

  	*pResult=iret;

				RtSleep(1);

}


void AdLinkF64( const LPDRIVER_INST pNet, SPECIAL_INST* const pData )
{
	    int rc = SUCCESS ;

//	UINT16 AxisNo;
	I16 iret=0;

	I16 iSet1,iSet2,iSet3,iSet4,iSet5;
	F64 fSet1,fSet2,fSet3,fSet4,fSet5;

			

	SPECIAL_INST_OTHERS* const pUser = &pData->User.paramOthers  ;

    UINT16* pResult = BuildUiotPointer( pUser->Header.ofsResult );
    UINT16* pStatus = BuildUiotPointer( pUser->Header.ofsStatus );
 
    double* pGetValue = BuildUiotPointer( pUser->fGetValue  );

	F64 pGet64 =0;
	I32 pGetI32=0 ;
	U16 pGetU16=0 ;

	I16 CardNo = (pNet->PciIndex -1);
//	AxisNo= MotionId[pUser->Address]; // axis number
//	ChannelId = 

 	*pResult=rc;
	*pStatus=rc;
	
	iSet1=pUser->iSetValue1 ;
	iSet2=pUser->iSetValue2;
	iSet3=pUser->iSetValue3 ;
	iSet4=pUser->iSetValue4 ;
	iSet5=pUser->iSetValue5 ;

	fSet1=pUser->fSetValue1  ;
	fSet2=pUser->fSetValue2  ;
	fSet3=pUser->fSetValue3  ;
	fSet4=pUser->fSetValue4  ;
	fSet5=pUser->fSetValue5  ;

#if defined( _DEBUG )
	SayOut( "function Code: %d, Axis : %d", pUser->Function, AxisNo,0 ,0,0 );
	SayOut( "AdLinkF64 int : %d,%d,%d,%d,%d", iSet1, iSet2, iSet3,iSet4,iSet5 );
	SayOut( "AdLinkF64 float : %f,%f,%f,%f,%f", fSet1, fSet2, fSet3,fSet4,fSet5 );
#endif

		iret= -1;
/*
	switch(pUser->Function  ) 
    {

	case	SET_GENERAL_COMPARATOR	 :
	iret=_8164_set_general_comparator( AxisNo,iSet1,iSet2,iSet3, fSet1);
	break;
	case	SET_TRIGGER_COMPARATOR :
	iret= _8164_set_trigger_comparator(AxisNo,iSet1,iSet2, fSet1);
	break;
	case	SET_GENERAL_COUNTER	 :
	iret=_8164_set_general_counter( AxisNo,iSet1, fSet1);
	break;
	case	PULSER_HOME_MOVE	 :
	iret=_8164_pulser_home_move( AxisNo,iSet1, fSet1);

	break;
	case	SET_MOVE_RATIO	 :
	iret= _8164_set_move_ratio( AxisNo, fSet1);
	break;
	case	SET_POSITION	 :
	iret=_8164_set_position( AxisNo, fSet1);
	iret=_8164_set_general_counter( AxisNo,0, fSet1);
	iret=_8164_set_general_counter( AxisNo,1, fSet1);
	iret=_8164_set_general_counter( AxisNo,2, fSet1);
	iret=_8164_set_general_counter( AxisNo,3, fSet1);
	
	break;
	case	RESET_TARGET_POS :
	iret=_8164_reset_target_pos( AxisNo, fSet1);
	break;
	case	PULSER_VMODE :
	iret=_8164_pulser_vmove( AxisNo, fSet1);
	break;
	case	P_CHANGE :
	iret=_8164_p_change( AxisNo, fSet1);
	break;

	case	PULSER_PMOVE :
	iret= _8164_pulser_pmove( AxisNo,fSet1,fSet2);
	break;
	case	FIX_SPEED_RANGE :
	iret=_8164_fix_speed_range( AxisNo, fSet1);
	break;
	case	SET_COMMAND :
	iret=_8164_set_command( AxisNo, (I32)fSet1);
	break;
	case	SET_SOFT_LIMIT	 :
	iret=_8164_set_soft_limit( AxisNo, (I32)fSet1, (I32)fSet2);
	break;
	case	ESCAPE_HOME :
	iret= _8164_escape_home	( AxisNo,fSet1, fSet2, fSet3);
	break;
	case	COMP_V_CHANGE	 :
	iret=_8164_cmp_v_change	( AxisNo,fSet1, fSet2, fSet3,fSet4);
	break;

	case	SET_SD_STOP :
	iret= _8164_sd_stop( AxisNo, fSet1);
	break;

  ///gets float value here	
	case	CHECK_COMPARE_DATA :
	iret=_8164_check_compare_data( AxisNo,iSet1,&pGet64);
	*pGetValue = pGet64;

	break;

	case	CHECK_COMPARE_STATUS :
	iret=_8164_check_compare_status	( AxisNo,&pGetU16);
	*pGetValue = (double) pGetU16;
	break;
	case	GET_LATCH_LOGIC	 :
	iret=_8164_get_latch_data( AxisNo,iSet1,&pGet64);
	*pGetValue = pGet64;
	break;
	case	GET_TARGET_POS :
	iret=_8164_get_target_pos( AxisNo,&pGet64);
	*pGetValue = pGet64;
	break;
	case	GET_CURRENT_SPEED :
	iret=_8164_get_current_speed( AxisNo,&pGet64);
	*pGetValue = pGet64;
	break;
//	case	 :
//	iret=_8164_get_general_counter(I16 AxisNo, F64 *CntValue);
//	break;
	case	GET_RESET_COMMAND	 :
	iret=_8164_get_rest_command	( AxisNo, &pGetI32);
	*pGetValue = (double) pGetI32;

	break;
	case	CHECK_RDP	 :
	iret=_8164_check_rdp( AxisNo, &pGetI32); 
	*pGetValue = (double) pGetI32;
	break;

	case	CHECK_CONTINUOUS_BUFFER :
	iret=_8164_check_continuous_buffer(AxisNo);
	*pGetValue = (double) iret;
	break;

///set comp value
	case SET_CMP_VALUE :
	iret = _8164_set_compare_data(AxisNo, iSet1, fSet1);
	break;
	}

	*pResult=iret;

#if defined( _DEBUG )
	SayOut( "RetCode : %d, Axis : %d", iret, AxisNo,0 ,0,0 );
#endif

*/

}

///////////// start of custom card from IE_IO.h
///The PCI bar index is 2.
// For write to IO card  - offset = (Addr * 0x0008) + 0x0200; 
// For read from IO card - offset = (Addr * 0x0008) + 0x0400;



int write_channel_output8 (U16 card_ID, U16 connect_index, U16 slave_No, U8 out_data)
{
	int rc = 0;


				U16	offset;
				U32 *DestData;
				offset = (slave_No * SIZE_BAR_DATA)+0x200 ;

				DestData	= (U32*)(pci_Info[card_ID].membaseAddr + offset); 


					*((UINT32 *)DestData) = out_data;

return rc;
}


int write_channel_output16 (U16 card_ID, U16 connect_index, U16 slave_No, U16 out_data)
{
	int rc = 0;


				U16	offset;
				U32 *DestData;
				offset = (slave_No * SIZE_BAR_DATA)+0x200 ;

				DestData	= (U32*)(pci_Info[card_ID].membaseAddr + offset); 

				*((UINT32 *)DestData) =out_data;
				


return rc;
}


int write_channel_output(U16 card_ID, U16 connect_index, U16 slave_No, U32 out_data)
{
	int rc = 0;


				U16	offset;
				U32 *DestData;
				U32 cData;
				offset = (slave_No * SIZE_BAR_DATA)+0x200 ;

				read_channel_output(card_ID,connect_index,slave_No, & cData);

				DestData	= (U32 *)(pci_Info[card_ID].membaseAddr + offset); 

				if (cData != out_data) 	*((U32 volatile *)DestData) = out_data;

		return rc;
}

int read_channel_output (U16 card_ID, U16 connect_index, U16 slave_No, U32 *in_data)
{
	int rc = 0;

				U16	offset;
				U32 *SrcData;
				offset = (slave_No * SIZE_BAR_DATA)+0x200 ;

				SrcData	= (U32*)(pci_Info[card_ID].membaseAddr + offset); 

				*in_data = *SrcData ;
			
return rc;
}


int read_channel_input8(U16 card_ID, U16 connect_index, U16 slave_No, U8 *in_data)
{
	int rc = 0;

				U16	offset;
				U8 *SrcData;
				offset = (slave_No * SIZE_BAR_DATA)+0x400 ;

				SrcData	= (U8*)(pci_Info[card_ID].membaseAddr + offset); 

				*in_data = *SrcData ;
			
return rc;
}

int read_channel_input16(U16 card_ID, U16 connect_index, U16 slave_No, U16 *in_data)
{
	int rc = 0;

				U16	offset;
				U16 *SrcData;
				offset = (slave_No * SIZE_BAR_DATA)+0x400 ;

				SrcData	= (U16*)(pci_Info[card_ID].membaseAddr + offset); 

				*in_data = *SrcData ;
			
return rc;
}

int read_channel_input(U16 card_ID, U16 connect_index, U16 slave_No, U32 *in_data)
{
	int rc = 0;

				U16	offset;
				U32 *SrcData;
				offset = (slave_No * SIZE_BAR_DATA)+0x400 ;

				SrcData	= (U32*)(pci_Info[card_ID].membaseAddr + offset); 

				*in_data = *SrcData ;
			
return rc;
}


int HSL_Init (U16 card_ID, U16 connect_index, U8 Limit, U16 t_rate, U8 last_addr)
{
			int	rc=0;

			UINT32 Size = 0x100;
			UINT32 *Dest = (UINT32*)(pci_Info[card_ID].membaseAddr + 0x200); 

            UINT32* pOutput  = (UINT32*)(pci_Info[card_ID].iobaseAddr + 0x3c); 
    
                *pOutput = 0x00080001;
				RtSleep(100);

				for(; Size >= 4; Size -= 4) *((UINT32 volatile*)Dest)++ = 0x00;
				
return rc;
}


/*
Setting Baud Rate

Write Addr: Bar[2] + 0x1638

Data: 0x00000013 (for 12.0 Mbps)

Data: 0x00000012 (for 6.0 Mbps)

Data: 0x00000011 (for 3.0 Mbps)

Data: 0x00000010 (for 1.5 Mbps)

*/

int HSL_auto_start (U16 card_ID, U16 connect_index, U8 Limit, U16 t_rate, U8 last_addr)
{
			int	rc=0;

			UINT32 Size = 0x100;
			UINT32 *Dest = (UINT32*)(pci_Info[card_ID].membaseAddr + 0x200); 
			UINT8* pOutput2  = (UINT8*)(pci_Info[card_ID].membaseAddr + 0x1600); 

			UINT32 *rate = (UINT32*)(pci_Info[card_ID].membaseAddr + 0x1638); 

				*rate = (U32)t_rate;	
				RtSleep(100);
                *pOutput2 = last_addr+3 ;/// 0x3F;
				RtSleep(100);
			
				for(; Size >= 4; Size -= 4) *((UINT32 volatile*)Dest)++ = 0x00;


if (*pOutput2 > 0x3f) rc=1;

///printf("transfer rate :%x",t_rate);


return rc;
}



int HSL_stop (U16 card_ID, U16 connect_index)
{
int	rc=0;
			UINT32 Size = 0x100;
			UINT32 *Dest = (UINT32*)(pci_Info[card_ID].membaseAddr + 0x200); 
    
			UINT8* pOutput2  = (UINT8*)(pci_Info[card_ID].membaseAddr + 0x1600); 

				RtSleep(100);
				for(; Size >= 4; Size -= 4) *((UINT32 volatile*)Dest)++ = 0x00;
				RtSleep(100);
             *pOutput2 = 0x0;



if (*pOutput2 == 0x00) rc=1;



return rc;
}


/****************************************************************************/
/*	read_LCR_dword/word/byte												*/
/*	write_LCR_dword/word/byte												*/
/*	Functions:																*/
/*	Purpose		: read or write the LCRs									*/
/*	Inputs		:															*/
/*	word n		: LC register numbers										*/
/*	word value	: write value												*/
/*	Outputs		: None														*/
/*	Return		: (for read functions) 										*/
/*	register value															*/
/****************************************************************************/
DWORD read_LCR_dword(WORD plx_lcr_ba, WORD n)
{
	return(_INPORTD((WORD)(plx_lcr_ba + n)));
}

WORD read_LCR_word(WORD plx_lcr_ba, WORD n)
{
	return(_INPORTW((WORD)(plx_lcr_ba + n)));
}

BYTE read_LCR_byte(WORD plx_lcr_ba, WORD n)
{
	return(_INPORTB((WORD)(plx_lcr_ba + n)));
}

void write_LCR_dword(WORD plx_lcr_ba, WORD n, WORD value)
{
	_OUTPORTD((WORD)(plx_lcr_ba + n), value);
}

void write_LCR_word(WORD plx_lcr_ba, WORD n, WORD value)
{
	_OUTPORTW((WORD)(plx_lcr_ba + n), value);
}

void write_LCR_byte(WORD plx_lcr_ba, WORD n, WORD value)
{
	_OUTPORTB((WORD)(plx_lcr_ba + n), value);
}
