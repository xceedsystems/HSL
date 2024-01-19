/********************************************************************

                                PciStuff.c

    Interface specific code. 
    This file only should touch the hardware.

*********************************************************************/


#include "stdafx.h"

#include <rt.h>
#include <pcibus.h>     // 
#include <string.h>     // strlen()
#include <stdio.h>      // sprintf()
#include <stdlib.h>     // malloc()

#include "vlcport.h"
#include "dcflat.h"             // EROP()
#include "driver.h"             // SEMAPHORE
#include "errors.h"             // IDS_SYNERGETIC_HW_TEST
#include "auxrut.h"             // StartTimeout(), IsTimeout(), EROP
#include "pcistuff.h"           // Init()

#include "Global.h"
///#include "..\REMOTEDLL\REMOTE785x.h"

extern PCI_Info pci_Info[12];

///extern W_REMOTE_Initial( (U16)CardNo);

void pciinfo(PCIDEV PciDev, int CardNo);



/******************* Local definitions *******************************/



/******************* Initialization  *******************************/


/*
// ****************************************************************
//
//  PCI helper functions:
//
///////////////////////////////////////////////////////////////
//
//  Configuration registers in general and BARs in particular
//
*/

/* Make sure the configuration access parameters are valid */
static int CheckConfigRegister( UINT32 Offset, UINT32 Size, UINT8* pSizeCode )
{
    int rc = SUCCESS;

    if( Offset + Size > 256 )
        rc = IDS_ERR_PCI_OUTOF_IOIMAGE;
    else
    {
        switch( Size )
        {
            case 1:
                    *pSizeCode = T_BYTE;
                    break;
            case 2:
                    *pSizeCode = T_WORD;
                    break;
            case 4:
                    *pSizeCode = T_DWORD;
                    break;
            default:
                    *pSizeCode = 0;
                    rc = IDS_ERR_PCI_INVALID_REGSIZE;
        }
    }

    return rc;
}

static int WriteConfigRegister( PCIDEV* pPciDev, UINT32 Offset, UINT32 Size, UINT32 Value )
{
    UINT8 SizeCode;
    int rc = CheckConfigRegister( Offset, Size, &SizeCode );
    if( rc == SUCCESS )
        PciSetConfigRegister( pPciDev, (UINT8)Offset, SizeCode, (DWORD)Value );
    return rc;
}

static int ReadConfigRegister( PCIDEV* pPciDev, UINT32 Offset, UINT32 Size, UINT32* pValue )
{
    UINT8 SizeCode;
    int rc = CheckConfigRegister( Offset, Size, &SizeCode );
    if( rc == SUCCESS )
        *pValue = PciGetConfigRegister( pPciDev, (UINT8)Offset, SizeCode );
    return rc;
}



int vlcWriteConfigRegister( UINT32 PciBusNum, UINT32 PciSlotNum, UINT32 Offset, UINT32 Size, UINT32 Value )
{
    PCIDEV PciDev;
    memset( &PciDev, 0, sizeof( PCIDEV ) );
    PciDev.wBusNum    = (UINT16)PciBusNum;
    PciDev.wDeviceNum = (UINT16)PciSlotNum;
    return WriteConfigRegister( &PciDev, Offset, Size, Value );
}

int vlcReadConfigRegister( UINT32 PciBusNum, UINT32 PciSlotNum, UINT32 Offset, UINT32 Size, UINT32* pValue )
{
    PCIDEV PciDev;
    memset( &PciDev, 0, sizeof( PCIDEV ) );
    PciDev.wBusNum    = (UINT16)PciBusNum;
    PciDev.wDeviceNum = (UINT16)PciSlotNum;
    return ReadConfigRegister( &PciDev, Offset, Size, pValue );
}


static int ReadHeaderType( PCIDEV* pPciDev, UINT8* pHeaderType )
{
    UINT32 HeaderType;
    UINT32 Offset = 0x0e;
    int rc = ReadConfigRegister( pPciDev, Offset, 1, &HeaderType );
    if( rc == SUCCESS )
        *pHeaderType = (UINT8)HeaderType;
    return rc;
}

static int ReadBar( PCIDEV* pPciDev, int BarIndex, UINT32* pValue )
{
    UINT32 Offset = BarIndex * sizeof(UINT32) + 0x10;
    return ReadConfigRegister( pPciDev, Offset, 4, pValue );
}

static int WriteBar( PCIDEV* pPciDev, int BarIndex, UINT32 Value )
{
    UINT32 Offset = BarIndex * sizeof(UINT32) + 0x10;
    return WriteConfigRegister( pPciDev, Offset, 4, Value );
}



/*
///////////////////////////////////////////////////////////////
//
//  Find the configuration for the specified 
//  Vendor/Device/Function set.
//
*/

/* returns TRUE if the next index is found. */
static int vlcPciFindNextDeviceOrDuplicate( PCIDEV* pPciDev )
{
    pPciDev->wDeviceIndex++;
    return PciFindDevice( pPciDev );
}


/* returns TRUE if the next different index is found. */
static int vlcPciFindNextDevice( PCIDEV* pPciDev )
{
    int bFound = 0;
    if( pPciDev->wFunction )
    {
        /* Multi-function device */
        bFound = vlcPciFindNextDeviceOrDuplicate( pPciDev );
    }
    else
    {
        /* Function 0. */
        UINT8 HeaderType;
        if( ReadHeaderType( pPciDev, &HeaderType ) == SUCCESS )
        {
            int bMultiFunctionDevice = HeaderType & 0x80;   /* see PCI specs for Header Type */

            if( bMultiFunctionDevice )
            {
                /* Multi-function device  */
                bFound = vlcPciFindNextDeviceOrDuplicate( pPciDev );
            }
            else
            {
	            /*
                    Single function device.  May show up as more dups, which we need to skip.
                    All duplicates share the same slot. 
                */
                UINT16 BusNum    = pPciDev->wBusNum;
	            UINT16 DeviceNum = pPciDev->wDeviceNum;
                int    bSameSlot;
                
                do
                {
                    bFound    = vlcPciFindNextDeviceOrDuplicate( pPciDev );
                    bSameSlot = ( ( BusNum == pPciDev->wBusNum ) && ( DeviceNum == pPciDev->wDeviceNum ) );
                } while( bFound && bSameSlot );     /* skip all duplicates */
            }
        }
    }
    		 // printf( "vlcPciFindNextDevice %d \n", bFound);///-

    return bFound;
}


/* returns TRUE if there is at least 1 device. */
static int vlcPciFindFirstDevice( PCIDEV* pPciDev, int VendorId, int DeviceId )
{
    memset( pPciDev, 0, sizeof(PCIDEV) );
    pPciDev->wVendorId = VendorId;
    pPciDev->wDeviceId = DeviceId;
	pPciDev->wDeviceIndex= 0;
			
    return PciFindDevice( pPciDev );
}




/* BoardIndex is 1 based */
/* called by InitPCI() */
static int FindPCIBoard( LPDRIVER_INST pNet, P_ERR_PARAM pErrors, PCIDEV* pPciDev )
{
    int rc = SUCCESS; 

    UINT16 const  BoardIndex         = pNet->PciIndex ;	/* 1-based index,  key to our board. */
    UINT16 const  ExpectedBoardCount = pNet->PciCount;	/* 0 --> any # of boards.  For check purposes only. */
    UINT16        FoundBoardCount    = 0;               /* how many boards are pluged in this system        */


///not used
//	if(BoardIndex == MAX_DRV_INSTANCES+1)  BoardIndex=ExpectedBoardCount;
//
//	if((BoardIndex == MAX_DRV_INSTANCES+1) &&(ExpectedBoardCount==0))
//	        {
//  /          rc = IDS_ERR_PCI_BOARD_NO_MISMATCH;
//            sprintf( pErrors->Param3, "%d", ExpectedBoardCount );
//            sprintf( pErrors->Param4, "%d", FoundBoardCount    );
//        }


    if( rc == SUCCESS )
    {
        PCIDEV PciDev;

        /*
            TO DO: 
            Define here he VENDOR ID and the DEVICE ID  for our PCI card. 
        */

        const   int VendorId = VENDOR_ID;  // TO DO: define here the VENDOR ID
        const   int DeviceId = pNet->pciDeviceId ;///  TO DO: define here the DEVICE ID

        int     bDone  = FALSE;                          /* when to stop looking for new boards of this type. */
        int     bFound = vlcPciFindFirstDevice( &PciDev, VendorId, DeviceId );
	
		  //printf( "vlcPciFindFirstDevice bFound %d \n",  bFound);///-

        for( ;  bFound && !bDone ; bFound = vlcPciFindNextDevice( &PciDev ) )
        {
        //    printf( "FoundBoardCount %d, board index %d \n", FoundBoardCount, BoardIndex);///-

            
			if( ++FoundBoardCount == BoardIndex )
            {
            //printf( "Found FoundBoardCount %d \n",FoundBoardCount);///-

                /* Found our board.  PciDev contains a handle to it. */
                if( !ExpectedBoardCount )   /* if no BoardCount check requested,                   */
                    bDone = TRUE;           /* no need to find the exact # of boards of this type. */
					pciinfo( PciDev,FoundBoardCount-1);
			///set pci info here

                memcpy( pPciDev, &PciDev, sizeof( PCIDEV ) );
            }
        }
    }

    if( rc == SUCCESS ) ///==0
    {

        if( FoundBoardCount < BoardIndex )
        {
            /* Not enough boards available on this system */
            rc = IDS_ERR_PCI_CANNOT_FIND_PCIDEVICE;
            sprintf( pErrors->Param3, "%d", BoardIndex );
            sprintf( pErrors->Param4, "%d", FoundBoardCount );
        }
    }

    if( rc == SUCCESS )
    {
        if( ExpectedBoardCount && ( ExpectedBoardCount != FoundBoardCount ) )
        {
            /* BoardCount check requested and failed. */
            rc = IDS_ERR_PCI_BOARD_NO_MISMATCH;
            sprintf( pErrors->Param3, "%d", ExpectedBoardCount );
            sprintf( pErrors->Param4, "%d", FoundBoardCount    );
        }
    }

	          //  printf( "last rc return %d \n", rc );//-

    return rc;
}




void pciinfo(PCIDEV PciDev, int CardNo)
{

	UINT32 tAddr = 0;

		///fill data
		pci_Info[CardNo].lcrBase		=PciDev.dwBaseAddr[2]  & 0xfffffffc;

		tAddr = PciDev.dwBaseAddr[2]  & 0xfffffffc;
		pci_Info[CardNo].membaseAddr = AllocateDpr(tAddr, MEMORY_SIZE);	
		tAddr = PciDev.dwBaseAddr[0]  & 0xFFFFfffc;
		pci_Info[CardNo].iobaseAddr		= AllocateDpr(tAddr, 0x1000L);	



        pci_Info[CardNo].busNo = PciDev.wBusNum;
        pci_Info[CardNo].devFunc = PciDev.wFunction;

		pci_Info[CardNo].irqNo=PciDev.byIntLine;
		pci_Info[CardNo].initFlag=PciDev.wDeviceId;   

		pci_Info[CardNo].Interrupt_Source = 0xE0;

		pci_Info[CardNo].hEvent = 0;
		pci_Info[CardNo].int_src = 0;
		pci_Info[CardNo].hDiEvent = 0;
		pci_Info[CardNo].DiEventFlag = 0; //disable
		pci_Info[CardNo].timerEventFlag = 0; //disable

/*
 printf("PciDev.wDeviceId %x,  \n", PciDev.wDeviceId);
 printf("PciDev.wFunction %x,  \n", PciDev.wFunction);
 printf("PciDev.wBusNum %x,  \n", PciDev.wBusNum);
 printf("PciDev.byIntLine %x,  \n", PciDev.byIntLine);
 printf("PciDev.byIntPin %x,  \n", PciDev.byIntPin);
 

 printf("PciDev.dwBaseAddr[0] %x,  \n", PciDev.dwBaseAddr[0]);
 printf("PciDev.dwBaseAddr[1] %x,  \n", PciDev.dwBaseAddr[1]);
 printf("PciDev.dwBaseAddr[2] %x,  \n", PciDev.dwBaseAddr[2]);
 printf("PciDev.dwBaseAddr[3] %x,  \n", PciDev.dwBaseAddr[3]);
 printf("PciDev.dwBaseAddr[4] %x,  \n", PciDev.dwBaseAddr[4]);
 printf("PciDev.dwBaseAddr[5] %x,  \n", PciDev.dwBaseAddr[5]);
 printf("pci_Info[%x] %x,  \n",CardNo, pci_Info[CardNo].initFlag);
*/


	pci_Info[CardNo].MemLen = MEMORY_SIZE;


}


void FreePlxDpr( int CardNo)
{



		if (pci_Info[CardNo].membaseAddr) FreeDpr( pci_Info[CardNo].membaseAddr );
		if (pci_Info[CardNo].iobaseAddr	) FreeDpr( pci_Info[CardNo].iobaseAddr );


}



/* called by rtOpen() */
int InitPCI( LPDRIVER_INST pNet, P_ERR_PARAM pErrors )
{
    PCIDEV PciDev;                             /* work variable */
    int rc = FindPCIBoard( pNet, pErrors, &PciDev );

    if( rc == SUCCESS )
        pNet->PhyAddr = PciDev.dwBaseAddr[ 2 ] & 0xfffffff0;

    return rc;
}






/*
1) Enable Chipsel
The offset is 0x3C
The data is 0x80001


2) Set Baud Rate to 6 Mbps
The offset is 0x1634
The data is 0x0012

3) Start the link
The offset is 0x1600
The data is 0x003F

The PCI bar index is 2.

For write to IO card  - offset = (Addr * 0x0008) + 0x0200; 
For read from IO card - offset = (Addr * 0x0008) + 0x0400;


1) Configure chip select

Write Addr: Bar[0] + 0x3C

Data: 0x00080001 (previously u mention 0x8001. It is 0x80001) Read back to confirm

 

2) Start the link scan

Write Addr: Bar[2] + 0x1600

Data: 0x0000003F

Read back to confirm

 

3) Stop the link scan

Write Addr: Bar[2] + 0x1600

Data: 0x00000000

Read back to confirm

 

Setting Baud Rate

Write Addr: Bar[2] + 0x1638

Data: 0x00000013 (for 12.0 Mbps)

Data: 0x00000012 (for 6.0 Mbps)

Data: 0x00000011 (for 3.0 Mbps)

Data: 0x00000010 (for 1.5 Mbps)


This is the read data on my side:-

Addr: Bar[0] + 0x3C. Data is 0x00080001

Addr: Bar[0] + 0x00. Data is 0x0FF00000

Addr: Bar[0] + 0x14. Data is 0x00000001

Addr: Bar[0] + 0x28. Data is 0x00800000


*/





















