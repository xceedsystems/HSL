/****************************************************************************

                            Driver.h

     Driver specfic UIOT structures and adjunct to the network node

*****************************************************************************/



#ifndef __DRIVER_H__
#define __DRIVER_H__


//  id num. Make sure this id is unique across all VLC C-modules and drivers.
#define DriverREMOTE       0x5e7f10B5L ///

#define COMPILED_VER  0x00010060L

/*
    Version 01.0002
    Bump this version # every time DRIVER_INST, DEVICE_INST or
    DEVICE_IO structs have changed. 
    This will force old projects to be recompiled before execution. 
*/
#define REMOTEVERS         COMPILED_VER



//  Network config struct id
#define NETCONFIG_ID        DriverREMOTE


/* 
    Network config struct version
    Bump this version # every time NETCONFIG got new fields. 
    NetPass1() will force drivers in old projects to be reconfigured. 
    If old fields and NETCONFIG size are preserved, 
    configuration in old fields will be kept. 
*/
#define NETCONFIG_VERS      COMPILED_VER



// Device  config struct id
#define DEVCONFIG_ID        COMPILED_VER    



/*
    Device  config struct version
    Bump this version # every time DEVCONFIG got new fields. 
    NetPass1() will force devices in old projects to be reconfigured. 
    If old fields and DEVCONFIG size are preserved, 
    configuration in old fields will be kept. 
*/
#define DEVCONFIG_VERS      COMPILED_VER     


// load this value in DRIVER_INST for rt checking
#define RT3_SENTINEL        0x55667788L


// max 4 networks can be controlled by 1 PC
#define  MAX_DRV_INSTANCES          4     /// MAX_PCI_CARDS ref



#include  "errors.h"


#ifndef APSTUDIO_READONLY_SYMBOLS



#ifndef VLCPORT_H__
#include "vlcport.h"
#pragma warning( disable: 4244 )
#endif

#ifndef DATASEG_H__
#include "dataseg.h"
#endif


/*****************************************************************************************

    This file keeps all necessary definitions needed for our  driver.
    This is a driver build for didactical purposes. It can be used as a starting point 
    when we need to create a new real driver.
    The  driver assumes we have an IO network. 
    The network is controlled by a card and can have up to MAX_DEVICES devices. 
    The card is accesible by a dual port ram and a port address.
    The DRP is DPR_TOTAL_SIZE bytes large and contains a control space, an input space and
    an output space.
    To keep things simple, our devices have all the same size: 8 bytes. 
    They are mapped in the DPR IO spaces based on their network address: device 0's input
    area can be found at the beginning of the DPR input space, device 1's input area is 
    8 bytes farther...  The same mechanism works for devices' output points. 
    In order to see input tags changing, we have to use an external application 
    that writes in the DPR input space.  We can also use the POKE special function to write there. 
    When generating the driver we can change DPR_OUTPUT_OFF to match DPR_INPUT_OFF. 
    Input space will then overlap the output space, so in the VLC project all input tags 
    will be animated by their correspondent output tags.    

    Configuring the driver:
    1.  Choose a DPR address.
    2.  Choose a port address (didactic purpose only: will not be used)
    3.  Choose an interrupt level (didactic purpose only: the interrupt routine does nothing)
    4.  Skip HW tests. We may want to control the thoroughness of initial hw tests.
    5.  Simulate:  if on, there will be no attempt to touch the hardware.
    6.  Watchdog:  if on, the card's watchdog must be cyclicly kicked at run time.
    7.  Cyclic Input Read:  if on,  we update UIOT input image every Input() call.
                            if off, we have to rely on some hardware features telling us if 
                                    any input changed.
    
    Configuring devices:
    1.  Choose a link address (0 ... 127). This determines allocation in the DPR IO space
    2.  Critical:  if on, this device must be active at Online()    
    

    There are 5 different driver models we are studying:
    Model1:     No special functions at all.    (Simulate)
    Model2:     Only sync s.f. No background task.  (PID, Utility, ...)
    Model3:     Sequential async s.f. processing:  Pend & Done lists. (ex. ABKTX, MTL, ...)
                    DRIVER_INST needs  MarkTime.
    Model4:     Simoultaneous async s.f. processing: Pend, Run, Done lists (ex. DHPLUS, ...)
                    The hw supports commands with reply. 
                    New commands can be launched while others are waiting for their replies.
    Model5:     Paralel sequential s.f. processing. 
                    The hw supports a fixed # of channels that can accept commands.
                    Commands executed on different channels can run in paralel.
                    Commands executed on a channel are performed on a FIFO basis.
                    Pend[], Done lists    (DATALOG)
                    DRIVER_INST needs  MarkTime[].
    

    Here is an example for model 3:    
    
*****************************************************************************************/



//************   1 byte alignment typedef structures !!!   ************

#pragma BYTE_ALIGN(_SPECIAL_INST)
typedef  struct _SPECIAL_INST*  LPSPECIAL_INST_;
typedef                 UINT8*  LPUINT8;

typedef  selector   SEMAPHORE;

typedef  UINT32     UIOTREF2CHAR;
typedef  UINT32     UIOTREF2SINT8;
typedef  UINT32     UIOTREF2UINT8;
typedef  UINT32     UIOTREF2SINT16;
typedef  UINT32     UIOTREF2UINT16;
typedef  UINT32     UIOTREF2SINT32;
typedef  UINT32     UIOTREF2UINT32;
typedef  UINT32     UIOTREF2DOUBLE;
typedef  UINT32     UIOTREF2VOID;


#pragma BYTE_ALIGN(_LINKED_LIST)
typedef struct _LINKED_LIST 
{
    LPSPECIAL_INST_     pHead;      // Pointer to the first element in the linked list
    LPSPECIAL_INST_     pTail;      // Pointer to the last element in the linked list
    SEMAPHORE           Semaphore;  // Semaphore that locks the SPECIAL_INST list
    UINT16              uCounter;   // How many items are enqueued here
} LINKED_LIST, *LPLINKED_LIST; 
#pragma BYTE_NORMAL()


#pragma BYTE_ALIGN(_PTBUFFER)
typedef struct _PTBUFFER 
{
    UIOTREF2VOID    Offset;  
    UINT32          Size;           // Use PTBUFFER type for PT_BUFFERs
} PTBUFFER, * LPPTBUFFER;           // Its size is 8 bytes
#pragma BYTE_NORMAL()


#pragma BYTE_ALIGN(_TASK)
typedef struct _TASK 
{
    UINT16      hTask;          // background/interrupt task handle
    SEMAPHORE   Semaphore;      // Where the background task waits
    void*       pStack;         // Pointer to the stack allocated to the task
    UINT16      bBusy;          // True if Special I/O Task is working on packet, used during shutdown
    UINT16      Error;          // error code for the task's init sequence
    void*       IrqThunk;       // pointer to the interrupt routine
    UINT16      level;          // irmx encoded IRQ
    UINT16      align;
} TASK, * LPTASK; 
#pragma BYTE_NORMAL()





#pragma BYTE_ALIGN(_DEVICE_IO)  // 1 byte alignment
typedef struct _DEVICE_IO       // Specifies the UIOT offset and the size for each device
{
    void*        pSrc;          // DPR  offset/pointer for input devices || UIOT offset/pointer for output devices
    void*        pDst;          // UIOT offset/pointer for input devices || DPR  offset/pointer for output devices
    UIOTREF2VOID ofsUiot;       // ofsDst for input devices or ofsSrc for output devices
    UINT16       Size;          // device input or output size.  Never 0 !!!
    UINT16       bUsed;         // If no I/O tags defined in the UIOT, skip it
} DEVICE_IO, *LPDEVICE_IO;            
#pragma BYTE_NORMAL()


#pragma BYTE_ALIGN(_DEVICE_INST)         // 1 byte alignment
typedef struct _DEVICE_INST
{
    UIOTREF2VOID ofsName;       // UIOT offset to the device name generated at compile time
    UINT16       Address;       // device's network address
    UINT16       Type;          // DEVICE_1W_INPUT, ... Never 0 !!!
    UINT16       ChannelID;	
	UINT16       Critical;
    UINT16       bPresent;      // if 1 --> device was online when load and go
    char*        pName;         // Usable UIOT pointer to the device name generated at runtime based on ofsName.
	DEVICE_IO	 Input;			// how to handle the input image
	DEVICE_IO	 Output;		// how to handle the output image
///
    UINT16  AdlinkId;          
	UINT16  IoMode;				
/// motion
	UINT16	OuputMode;		
	UINT16	InputMode;
	UINT16	EncSrc;
	UINT16	logic;
	UINT16	Almlogic;
	UINT16	AlmResp;

	UINT16	OrgLogic;
	UINT16	LMResp;
	UINT16	HomeMode;
	UINT16	CntSrc;
	UINT16	Inp;
	UINT16	InpMode;

	UINT16	SDMode;
	UINT16	SDResp;
	UINT16	SDLogic;
	UINT16	SDLatch;

	UINT16	ERCLogic;
	UINT16	EZLogic;
	UINT16	LTCLogic;
	UINT16	ServoInitialState; //not used
	UINT16	motionAddress; //
	UINT16	motionHead; //



	UINT32       Sentinel;
} DEVICE_INST, *LPDEVICE_INST;
#pragma BYTE_NORMAL()


#pragma BYTE_ALIGN(_DRIVER_INST) 
typedef struct _DRIVER_INST 
{
    NETWORK Net;

        // Compile-time Static variables.  This structure maps to UIOT

    UIOTREF2VOID    ofsDeviceList;      // Where the DEVICE_INST list starts.
    UINT32          nDevices;           // how many Devices are in the list
	UINT16			PciIndex;			// 1...8  which board
	UINT16			PciCount;			// 0...8  how many boards expected in the system. 0--> skip test.
	UINT32          PhyAddr;            //  read from the PCI card
    UINT16          pciDeviceId;        // 
    UINT16          bSimulate;          // =0 --> interface card must be present
    UINT16           TransferRate;        // 24  HWTEST_RW, HWTEST_OFF

   UINT16           MaxNodeAddr;         // 26  INPUT_READ_CYCLIC, INPUT_READ_COS,

	// Run-time Dynamic Variables
    LPDEVICE_INST   pDeviceList;        // Where the DEVICE_INST list starts.

	void* 			pDpr;               // virtual pointer built based on BAR0
    UINT16          bFirstCycle;        // Set by OnLine(), reset by Output(). Read by Input() and Output()
    UINT16          bGoOffLine;         // Tell all the bkg functions to shutdown

    LINKED_LIST     Pend;               // Pointer to the linked list of pending functions
    LINKED_LIST     Done;               // Pointer to the linked list of done  functions

    TASK            BackgroundTask;     // controls for the background task
    TASK            InterruptTask;      // controls for the interrupt task

    UIOTREF2UINT32  ofsSentinel;        // 0x55667788 - display this value using Soft Scope to check corrct map
    UINT32          Sentinel;           // 0x55667788 - display this value using Soft Scope to check corrct map

} DRIVER_INST, *LPDRIVER_INST;    
#pragma BYTE_NORMAL() 


#pragma BYTE_ALIGN( _SPECIAL_INST_HEADER )      // Must be first block in all paremeter blocks
typedef struct _SPECIAL_INST_HEADER
{       // Compile-time Static variables.  This structure maps to .rcd descrition
                                    // off, sz, ob.sz
    UINT16          FunctionId;     //  0    2   2L   PT_CONST  --> UINT16, _SIZE 2L
    UINT16          align;          //  2    2
    UIOTREF2UINT16  ofsStatus;      //  4    4   2L   PT_REF    --> tag's offset in the UIOT
    UIOTREF2UINT16  ofsResult;      //  8    4   2L   PT_REF    --> tag's offset in the UIOT
} SPECIAL_INST_HEADER;              //      12 == sizeof( SPECIAL_INST_HEADER )
#pragma BYTE_NORMAL()
/*
    Note: beacuse all functions have an Id field and a return status, we can standardize them 
    at offsets 0 and 4. This is especially helpful when using customized parameter structures 
    to better match function particularities and to save memory. 
*/


#pragma BYTE_ALIGN( _SPECIAL_INST_COMMAND ) 
typedef struct _SPECIAL_INST_COMMAND
{       // Compile-time Static variables.  This structure maps to .rcd descrition
    SPECIAL_INST_HEADER Header;         //  0   12        the header must always be first
    UINT16      Address;        // 12  2   2L   PT_VALUE, PT_DEVICE --> UINT32
    UINT16	    Function;		// 14    2   2L   PT_REF    --> tag's offset in the UIOT
	double		fPos;		
	double      fstVel;
	double      fmaxVel;
	double      fTAcc;
	double      fTDec;
	double      fSVAcc;
	double      fSVDec;

	double      fPosX;
	double      fPosY;
	double      fPosZ;
	double      fPosU;

	double      fCx;
	double      fCy;
	double      fEx;
	double      fEy;
	double      fOrg;
	PTBUFFER	AxisArray;
	UINT16		OptDir;
	UINT32		Slave_ChannelID;

} SPECIAL_INST_COMMAND;                 //     152 == sizeof( SPECIAL_INST_COMMAND )
#pragma BYTE_NORMAL()

/// DRVF_MOTION_INT/F64
#pragma BYTE_ALIGN( _SPECIAL_INST_OTHERS ) 
typedef struct _SPECIAL_INST_OTHERS
{       // Compile-time Static variables.  This structure maps to .rcd descrition
                                        // off, sz, ob.sz
    SPECIAL_INST_HEADER Header;         //  0   12        the header must always be first
    UINT16      Address;        // 12   2   2L   PT_VALUE, PT_DEVICE --> UINT32
    UINT16	    Function;		// 14   2   2L   PT_REF    --> tag's offset in the UIOT
UIOTREF2SINT16	iGetValue;			//16 4L
	SINT16		iSetValue1;			//20 2L
	SINT16		iSetValue2;			//22
	SINT16		iSetValue3;			//24
	SINT16		iSetValue4;			//26
	SINT16		iSetValue5;			//28
	SINT16		spare;			//		30

	double		fSetValue1;			//32
	double		fSetValue2;			//40   8L
	double		fSetValue3;			//48
	double		fSetValue4;			//56
	double		fSetValue5;			//64
UIOTREF2DOUBLE  fGetValue;			//72	4L
	UINT32		Slave_ChannelID;	//76


} SPECIAL_INST_OTHERS;                 //   76
#pragma BYTE_NORMAL()

#pragma BYTE_ALIGN( _SPECIAL_INST_SETGET ) 
typedef struct _SPECIAL_INST_SETGET
{       // Compile-time Static variables.  This structure maps to .rcd descrition
                                        // off, sz, ob.sz
    SPECIAL_INST_HEADER Header;         //  0   12        the header must always be first
    UINT16      Address;			// 72   2   2L   PT_VALUE, PT_DEVICE --> UINT32
    UINT16	    Function;				// 74    2   2L   PT_REF    --> tag's offset in the UIOT
	SINT32		ArraySize;		// 16	 4
    double		SetSingleValue;		// 20    8  
	PTBUFFER      SetValue;	// 24
	PTBUFFER      GetValue;	// 32
	UINT32		Slave_ChannelID;


} SPECIAL_INST_SETGET;                  //     152 == sizeof( SPECIAL_INST_COMMAND )
#pragma BYTE_NORMAL()



#pragma BYTE_ALIGN(_SPECIAL_INST_PORT)  // we may have substitutes for SPECIAL_INST_PARAM
typedef struct _SPECIAL_INST_PORT
{       // Compile-time Static variables.  This structure maps to .rcd descrition
                                        // off, sz, ob.sz
    SPECIAL_INST_HEADER Header;         //  0   12        the header must always be first
    UINT16              Address;        // 12    2   2L   PT_VALUE  --> UINT16, _SIZE 2L
    UINT16              Length;         // 14    2   2L   PT_VALUE  --> UINT16, _SIZE 2L
    UIOTREF2UINT16      ofsInValue;     // 16    4   2L   PT_REF    --> tag's offset in the UIOT 
    UINT16              OutValue;       // 20    2   2L   PT_VALUE  --> UINT16, _SIZE 2L
    UINT16              align;          // 22    2   2L   PT_VALUE  --> UINT16, _SIZE 2L
} SPECIAL_INST_PORT;                    //      24 == sizeof( SPECIAL_INST_PORT )
#pragma BYTE_NORMAL()

typedef union _SPECIAL_INST_PARAM
{       // Compile-time Static variables.  This structure maps to .rcd descrition
                                        // off, sz
    SPECIAL_INST_HEADER  paramHeader;   //  0   12
    SPECIAL_INST_COMMAND paramCommand;  //  0   48
	SPECIAL_INST_SETGET  paramSetGet;
	SPECIAL_INST_OTHERS  paramOthers;

} SPECIAL_INST_PARAM;                   //      48 == sizeof(SPECIAL_INST_PARAM)


typedef struct _SPECIAL_INST
{       // Compile-time Static variables.  This structure maps to .rcd descrition
                                        // off,  sz
    SPECIAL_INST_PARAM  User;           //   0   48
    SPECIAL_INST_PARAM  Work;           //  48   48

        // generic, same for all drivers having asyncronous special functions
    UINT32                MarkTime;     //  96    4  when this s.f. must be complete
    SINT16                Status;       // 100    2
    UINT16                Busy;         // 102    2    
    struct _SPECIAL_INST* pNext;        // 104    4

} SPECIAL_INST, *LPSPECIAL_INST;        //      108 == sizeof( SPECIAL_INST )

/*
Note1: This struct is declared 1 byte aligned on top of file. The struct description is 
       evaluated by the "Runtime" sub-project only.  
       The 'Gui' subproject evaluates the SPECIAL_INST parameter block as presented 
       by the FNC_... definitions. 

Note2: For a very simple function module,  SPECIAL_INST is sufficient.  
       Parameter fields can be described directly in SPECIAL_INST.  
       SPECIAL_INST_PARAM, SPECIAL_INST_PORT, SPECIAL_INST_COMMAND and SPECIAL_INST_HEADER 
       are optional.  They have been defined here only to show a more complex example. 

Note3: In order to save memory SPECIAL_INST can be used only for asynchronous special functions. 
       SPECIAL_INST_COMMAND, SPECIAL_INST_PORT, or even SPECIAL_INST_HEADER 
       will do the job for synchronous special functions. 
       Make sure the correct param block size is declared NET(DEV)_FUNC_TYPE paragraph (p#2).

Note4: Because asynchronous functions are executed concurenlty with the flowchart code, 
       it is safer to provide a copy of the parameter block, to be used by the background thread. 
       This is why we have introduced the 'User' and 'Work' areas. 
       'User' is the area marked by the compiler to be filled in every time a function 
       is called. When the function is posted for execution, 'User' is copied into 'Work' 
       and 'Work' is what the background sees.
       Make sure the fields in 'User' and 'Header' match the FNC_... definitions. 
       It is a good idea to have them both mapped at offset 0.

Note5: The Runtime Special() entry point offers a pointer to the associated SPECIAL_INST. 
       Depending on the FunctionId, the right parameter layout will be selected. 
       This can be implemented in 3 ways: 
       a. Define 1 layout only large enough to encompass all parameters needed by any function. 
       b. Define 1 layout for every function, and cast to the right one based on the FunctionId. 
       c. Define 1 layout for every function, store them into a union and select the right 
          union branch based on the FunctionId. 
       Our current implementation is a mixture of a. and c. and should be optimal 
       for consumed memory and code complexity. 
*/


#ifdef WINVER          // This is for MSVC compiler


#ifndef DRVRUTIL_H__
#include "drvrutil.h"   // SS_ZERO
#endif

// What we put into the database for network config

#pragma BYTE_ALIGN(_NETCONFIG)     // 1 byte alignment
typedef struct _NETCONFIG
{
    UINT32           NetcfgId;          //  0  NETCONFIG_ID
    UINT16           NetcfgVersMinor;   //  4  LOW(  NETCONFIG_VERS )
    UINT16           NetcfgVersMajor;   //  6  HIGH( NETCONFIG_VERS )
    UINT32           PciIndex;          //  8  1...8 max 8 boards in the system
    UINT16           PciCount;          // 12  0...8 if 0--> skip the count test
    UINT16           IrqLevel;          // 14  0 ... 15
    UINT16           BaudRate;          // 16  BAUDRATE_125, BAUDRATE_250, ...
    UINT16			 pciDeviceId;       // 18  8 or 16 channels

    UINT16           bSimulate;         // 20  =0 --> interface card must be present
    UINT16           bWatchdog;         // 22  =1 --> kick the watchdog
    UINT16           TransferRate;        // 24  HWTEST_RW, HWTEST_OFF

    UINT16           MaxNodeAddr;         // 26  INPUT_READ_CYCLIC, INPUT_READ_COS,
    
    UINT8            Reserved[128-28];  // 28  add new fields without changing NETCONFIG size
} NETCONFIG;                            // 128  == NET_CONFIG_SIZE == sizeof(NETCONFIG)
#pragma BYTE_NORMAL()


#pragma BYTE_ALIGN(_DEVCONFIG)  // 1 byte alignment
typedef struct _DEVCONFIG
{                               // Byte Offset
    UINT32  NetcfgId;           //  0 NETCONFIG_ID
    UINT32  DevcfgId;           //  4 DEVCONFIG_ID
    UINT16  DevcfgVersMinor;    //  8 LOW(  DEVCONFIG_VERS )
    UINT16  DevcfgVersMajor;    // 10 HIGH( DEVCONFIG_VERS )
    UINT16  Address;            // 12 device's address on the link
    UINT16  AdlinkId;          // 14 =1 --> this device must be present on the link
    UINT16  ChannelID;			// 16 id
	UINT16	Critical;			// 18 
	UINT16  IoMode;				// 20 for digital type
/// motion
	UINT16	OuputMode;			///22
	UINT16	InputMode;
	UINT16	EncSrc;
	UINT16	logic;
	UINT16	Almlogic;
	UINT16	AlmResp;

	UINT16	OrgLogic;
	UINT16	LMResp;
	UINT16	HomeMode;
	UINT16	CntSrc;
	UINT16	Inp;
	UINT16	InpMode;

	UINT16	SDMode;
	UINT16	SDResp;
	UINT16	SDLogic;
	UINT16	SDLatch;

	UINT16	ERCLogic;
	UINT16	EZLogic;
	UINT16	LTCLogic;
	UINT16	ServoInitialState; //not used
	UINT16	motionAddress; //
	UINT16	motionHead; //


    UINT8   Reserved[128-66];	// 20 add new fields without changing NETCONFIG size
} DEVCONFIG;                    // 128 == DEVCONFIG_SIZE == sizeof(DEVCONFIG)
#pragma BYTE_NORMAL()

/*
    Note: The reserved fields will be used for future developpment. 
    They ensure compatibility with projects generated by older versions of this driver.
*/


#endif      // WINVER


#endif      // ! APSTUDIO_READONLY_SYMBOLS

/* 
    Defines for .rcd file 
    Arithmetic expressions are allowed to define RC and RCD constants, 
    when  ONLY using + and -.  
    It is a good idea to have them encapsulated in ( ).
    Never use * and /.  The RC compiler silently ignores them.
*/



#define FNC_HD_SPECIAL_INST_SIZE        12
#define FNC_CM_SPECIAL_INST_SIZE        164
#define FNC_OT_SPECIAL_INST_SIZE        88
#define FNC_SPECIAL_INST_SIZE           164


// SPECIAL_INST offsets & sizes
#define FNC_HD_FUNCTIONID           0L 
#define FNC_HD_FUNCTIONID_SIZE          2L      
#define FNC_HD_STATUS               4L 
#define FNC_HD_STATUS_SIZE              2L  
#define FNC_HD_RESULT               8L 
#define FNC_HD_RESULT_SIZE              2L   
  
#define FNC_CM_AXISNUM               12L 
#define FNC_CM_AXISNUM_SIZE              2L    
#define FNC_CM_FUNCTIONID           14L 
#define FNC_CM_FUNCTIONID_SIZE           2L 

/////////// above always fix

#define FNC_CM_FINAL_POS             16L    
#define FNC_CM_FINAL_POS_SIZE            8L     
#define FNC_CM_ST_VEL              24L 
#define FNC_CM_ST_VEL_SIZE              8L     
#define FNC_CM_MAX_VEL               32L 
#define FNC_CM_MAX_VEL_SIZE              8L     
#define FNC_CM_ACC               40L 
#define FNC_CM_ACC_SIZE              8L     
#define FNC_CM_DEC               48L 
#define FNC_CM_DEC_SIZE              8L     
#define FNC_CM_SVACC              56L 
#define FNC_CM_SVACC_SIZE              8L    
#define FNC_CM_SVDEC              64L 
#define FNC_CM_SVDEC_SIZE              8L    

#define FNC_CM_POSX              72L 
#define FNC_CM_POSX_SIZE              8L    
#define FNC_CM_POSY              80L 
#define FNC_CM_POSY_SIZE              8L    
#define FNC_CM_POSZ              88L 
#define FNC_CM_POSZ_SIZE              8L    
#define FNC_CM_POSU              96L 
#define FNC_CM_POSU_SIZE              8L     

#define FNC_CM_CX              104L 
#define FNC_CM_CX_SIZE              8L    
#define FNC_CM_CY              112L 
#define FNC_CM_CY_SIZE              8L    
#define FNC_CM_EX              120L 
#define FNC_CM_EX_SIZE              8L    
#define FNC_CM_EY              128L 
#define FNC_CM_EY_SIZE              8L     

#define FNC_CM_ORG              136L 
#define FNC_CM_ORG_SIZE              8L     

#define FNC_CM_AXISARRY           144L 
#define FNC_CM_AXISARRY_SIZE        8L     

#define FNC_CM_OPT              152L 
#define FNC_CM_OPT_SIZE              2L ////

#define FNC_CM_SLAVENUM_2		154L 




#define FNC_CM_SET             16L    
#define FNC_CM_SET_SIZE            8L      
#define FNC_CM_GET             24L 
#define FNC_CM_GET_SIZE              8L     


#define FNC_OT_GET             16L 
#define FNC_OT_GET_SIZE             4L      

#define FNC_OT_SET1             20L    
#define FNC_OT_SET2             22L    
#define FNC_OT_SET3             24L    
#define FNC_OT_SET4             26L   
#define FNC_OT_SET5             28L    
#define FNC_OT_SET_SIZE          2L     

#define FNC_OT_SET6             32L    
#define FNC_OT_SET7             40L    
#define FNC_OT_SET8             48L    
#define FNC_OT_SET9             56L    
#define FNC_OT_SET10            64L  
#define FNC_OT_SET_SIZE2		 8L     ///4

#define FNC_OT_GET2             72L 
#define FNC_OT_GET_SIZE2	     4L    /// 4 

#define FNC_CM_SLAVENUM_1		76L
             
#define FNC_CM__SLAVENUM_SIZE 4L /// fixed



// NETCONFIG offsets & sizes
#define NET_ID                      0 
#define NET_ID_SIZE                     32 
#define NET_VERS                    4 
#define NET_VERS_SIZE                   32 
#define NET_PCI_INDEX               8 
#define NET_PCI_INDEX_SIZE              32 
#define NET_PCI_COUNT               12
#define NET_PCI_COUNT_SIZE              16 
#define NET_NUMCHANNELS             18 
#define NET_NUMCHANNELS_SIZE            16 
#define NET_SIMULATE                20 
#define NET_SIMULATE_SIZE               16 
#define NET_WATCHDOG                22 
#define NET_WATCHDOG_SIZE               16 
#define NET_TRATE			24
#define NET_MAX_NODE_ADDR2			26

#define NET_PMAX_NODE_ADDR_SIZE			16

#define NETCONFIG_SIZE              128 

// DEVICECONFIG offsets & sizes
#define DEV_DRVID                   0
#define DEV_DRVID_SIZE                  32
#define DEV_ID                      4
#define DEV_ID_SIZE                     32
#define DEV_VERS                    8
#define DEV_VERS_SIZE                   32
#define DEV_ADDRESS                 12 
#define DEV_ADDRESS_SIZE                16 
#define DEV_IO_ID					16			
#define DEV_IO_ID_SIZE					16 
#define DEV_ADLINK_ID               14 

#define DEV_CRITICAL                18 
#define DEV_CRITICAL_SIZE               16

#define DEV_MOTION_SIZE				    16 //ALL MOTION VALUE 16BITS	
#define DEV_IO_MODE					20
#define DEV_OUTPUTMODE				22
#define DEV_INPUTMODE				24
#define DEV_ENCSCR					26
#define DEV_LOGIC					28
#define DEV_ALMLOGIC				30
#define DEV_ALMRESP					32

#define DEV_ORGLOGIC				34
#define DEV_LMRESP					36
#define DEV_HOMEMODE				38
#define DEV_CNTSCR					40
#define DEV_INP						42
#define DEV_INPMODE					44

#define DEV_SDMODE					46
#define DEV_SDRESP					48
#define DEV_SDLOGIC					50
#define DEV_SDLATCH					52

#define DEV_ERCLOGIC				54
#define DEV_EZLOGIC					56
#define DEV_LTCLOGIC				58
#define DEV_SERVOSTATE				60

#define DEVCONFIG_SIZE              128 



// This is for the demo only.  Replace it with the real settings. 
// Dual port ram layout
#define  DPR_CONTROL_OFF        0
#define  DPR_CONTROL_SIZE       2048
#define  DPR_INPUT_OFF          2048    // where the input image can be found in the dpr
#define  DPR_INPUT_SIZE         1024    // 1kbyte =  MAX_DEVICES * 8bytes input devices
#define  DPR_OUTPUT_OFF         3072    // where the output image can be found in the dpr
//#define  DPR_OUTPUT_OFF       2048    // for didactic purposes use 2048 --> outputs will be looped back in inputs
#define  DPR_OUTPUT_SIZE        1024    // 1kbyte =  MAX_DEVICES * 8bytes input devices
#define  DPR_TOTAL_SIZE         4096    // 4 kbytes
#define  MAX_DEVICES            63      // max 16 devices allowed by our didactical network


#define  DEVICE_1B_INPUT		10    
#define  DEVICE_1B_OUTPUT		11
    
#define  DEVICE_1W_INPUT		20
#define  DEVICE_1W_OUTPUT		21
	
#define  DEVICE_2W_INPUT		30
#define  DEVICE_2W_OUTPUT		31
	
#define  DEVICE_4W_INPUT		40
#define  DEVICE_4W_OUTPUT		41
	
#define  DEVICE_1B_IANDO		50 //8 in 8 out
#define  DEVICE_1B_IAN16DO		51 //8 in 16 out


#define  DEVICE_1W_IANDO		60 //16in 16 out
#define  DEVICE_1W_IAN32DO		61 //16in 32 out


#define  DEVICE_2W_IANDO		70 //32 in 32 out

#define  DEVICE_4X_MOTION		80 //Motion Axis



#define DEVICE_8DO			    0xa4
#define DEVICE_8R8DI		 	0xa0
#define DEVICE_8R16DI		 	0xa1

#define DEVICE_8DI				0xa6
#define DEVICE_8DI8DO_NP        0xa2    

#define DEVICE_16DI				0xa7
#define DEVICE_16DI16DO_NP		0xa3


#define DEVICE_16DO				0xa5
#define DEVICE_16R				0xa8
#define DEVICE_16R8DI			0xa9

#define DEVICE_4CI8DO			0xaa
#define DEVICE_4CI8DO_H			0xab

#define DEVICE_8PO8DI			0xac



#define DEVICE_16AI8DO_V		0xb1
#define DEVICE_16AI8DO_A	 	0xb2
#define DEVICE_4AO8DI_V		    0xb3
#define DEVICE_4AO8DI_A		    0xb4
#define DEVICE_8AO_V			0xb5
#define DEVICE_8AO_A			0xb6
#define DEVICE_8AI_HR			0xb7
#define DEVICE_AI16_M_T			0xb8
#define DEVICE_8AI_RT			0xb9
#define DEVICE_AI16AO2_M_VV		0xba
#define DEVICE_AI16AO2_M_AV		0xbb
#define DEVICE_UB47        		0xbc //similar with DEVICE_AI16AO2_M_VV

#define DEVICE_4XMO_CG_N_FD		0xc1
#define DEVICE_4XMO_CG_P_FD		0xc2
#define DEVICE_4XMO_CG_N_HD		0xc3
#define DEVICE_4XMO_CG_P_HD		0xc4

#define DEVICE_32DI_DB			0xe1
#define DEVICE_32DO_DB			0xe2


#define  DRVF_COLLECT               2105    

#define  DEVICE_FUNC                2010    // special device functions ids

#define DEVICE_MOT_FUN				2100






////motion function special block
#define  DEVF_GET_DEVSTAT          2200    // functions at device level
#define  DRVF_MOTION               2201    
#define  DRVF_SETGET               2202    
#define  DRVF_SETAXIS              2203
#define  DRVF_OTHERS_INT           2204    
#define  DRVF_OTHERS_F64           2205    
#define  DRVF_ONFLY                2206
#define  DRVF_MOTION_INTERP        2207    

#define  LIST_FUNCTION1				2231
#define  LIST_FUNCTION2				2232
#define  LIST_FUNCTION3				2233
#define  LIST_FUNCTION4				2234
#define	 LIST_FUNCTION_INTERP		2235

#define  LIST_DEV_OUTMODE			2236
#define  DEV_CO_LEVELLOGIC			2237
#define  DEV_CO_ALMMODE          2238





#define	 FUNC_NUM		1000

#define	 SET_PLS_OUTMODE	 1001 //"set pls outmode\0",
#define  SET_PLS_INMODE		 1002 //"set pls ipmode\0",
#define  SET_FEEDBK_SRC		 1003 //"set feedback src\0",

#define	TV_MOVE				 1011 //"tv move\0"
#define	SV_MOVE				 1012 //"sv move\0"
#define	V_CHANGE			 1013 //"v change\0"
#define	EMG_STOP			 1014 //"emg stop\0"
#define	FIX_SPEED_RANGE		 1015 //"fix speed range\0"
#define	UNFIX_SPEED_RANGE	 1016 //"unfix speed range\0"
#define	GET_CURRENT_SPEED	 1017 //"get current speed\0"
#define	VERIFY_SPEED		 1018 //"verify speed\0"

#define	START_TR_MOVE		 1021 //"start tr move\0"
#define	START_TA_MOVE		 1022 //"start ta move\0"
#define	START_SR_MOVE		 1023 //"start sr move\0"
#define	START_SA_MOVE		 1024 //"start sa move\0"
#define	SET_MOVE_RATIO		 1025 //"set move ratio\0"
#define	P_CHANGE			 1026 //"p change\0"
#define	SET_PCS_LOGIC		 1027 //"set pcs logic\0"
#define	SET_SD_PIN			 1028 //"set sd pin\0"
#define	BACKLASH_COMP		 1029 //"backlash comp\0"
#define	SUPPRESS_VIBRATION	 1031 //"suppress vibration\0"

#define	SET_IDLE_PULSE		 1041 //"set idle pulse\0"
#define SET_LATCH_SRC		 1042

//// INTERPOLE FUNCTION 1200 ~ 1300
#define	START_SA_LINE2		 1201 //"start sa line2\0"
#define	START_SA_LINE3		 1202 //"start sa line3\0"
#define	START_SA_LINE4		 1203 //"start sa line4\0"
#define	START_TA_LINE2		 1204 //"start ta line2\0"
#define	START_TA_LINE3		 1205 //"start ta line3\0"
#define	START_TA_LINE4		 1206 //"start ta line4\0"
#define	START_SR_LINE2		 1207 //"start sr line2\0"
#define	START_SR_LINE3		 1208 //"start sr line3\0"
#define	START_SR_LINE4		 1209 //"start sr line4\0"
#define	START_TR_LINE2		 1211 //"start tr line2\0"
#define	START_TR_LINE3		 1212 //"start tr line3\0"
#define	START_TR_LINE4		 1213 //"start tr line4\0"

#define	START_TR_MOVE_XY	 1214 //"start tr move xy\0"
#define	START_TA_MOVE_XY	 1215 //"start ta move xy\0"
#define	START_SR_MOVE_XY	 1216 //"start sr move xy\0"
#define	START_SA_MOVE_XY	 1217 //"start sa move xy\0"

#define	START_TR_MOVE_ZU	 1218 //"start tr move zu\0"
#define	START_TA_MOVE_ZU	 1219 //"start ta move zu\0"
#define	START_SR_MOVE_ZU	 1221 //"start sr move zu\0"
#define	START_SA_MOVE_ZU	 1222 //"start sa move zu\0"

///circular type

#define	START_TR_ARC_XYU	 1223 //"start tr arc xyu\0"
#define	START_TA_ARC_XYU	 1224 //"start ta arc xyu\0"
#define	START_SR_ARC_XYU	 1225 //"start sr arc xyu\0"
#define	START_SA_ARC_XYU	 1226 //"start sa arc xyu\0"

#define	START_TR_ARC_XY	 1227 //"start tr arc xy\0"
#define	START_TA_ARC_XY	 1228 //"start ta arc xy\0"
#define	START_SR_ARC_XY	 1229 //"start sr arc xy\0"
#define	START_SA_ARC_XY	 1231 //"start sa arc xy\0"

#define	START_TR_ARC_ZU	 1232 //"start tr arc zu\0"
#define	START_TA_ARC_ZU	 1233 //"start ta arc zu\0"
#define	START_SR_ARC_ZU	 1234 //"start sr arc zu\0"
#define	START_SA_ARC_ZU	 1235 //"start sa arc zu\0"

#define	START_TR_ARC2	 1236 //"start tr arc2\0"
#define	START_TA_ARC2	 1237 //"start ta arc2\0"
#define	START_SR_ARC2	 1238 //"start sr arc2\0"
#define	START_SA_ARC2	 1239 //"start sa arc2\0"

#define	PULSER_R_LINE2		 1240 //"pulser r line2\0"
#define	PULSER_R_ARC2		 1241 //"pulser r arc2\0"

#define	PULSER_A_LINE2		 1242 //"pulser a line2\0"
#define	PULSER_A_ARC2		 1243 //"pulser a arc2\0"

#define	SET_TR_MOVE_ALL			1244 //"set tr move all\0"
#define	SET_TA_MOVE_ALL			1245  //"set ta move all\0"
#define	SET_SR_MOVE_ALL			1246  //"set sr move all\0"
#define	SET_SA_MOVE_ALL			1247  //"set sa move all\0"
#define	START_MOVE_ALL			1248  //"start move all\0"
#define	STOP_MOVE_ALL			1249  //"stop move all\0"


//////////////////

#define	SET_HOME_CONFIG		 1301 //"set home config\0"
#define	HOMVE_MOVE			 1302 //"home move\0"
#define	ESCAPE_HOME			 1303 //"escape home\0"
#define	HOME_SEARCH			 1304 //"home search\0"


#define	SET_PULSER_IPTMODE	 1305 //"set pulser iptmode\0"
#define	PULSER_VMODE		 1306 //"pulser vmode\0"
#define	PULSER_PMOVE		 1307 //"pulser pmode\0"
#define	PULSER_HOME_MOVE	 1308 //"pulser home_move\0"
#define	SET_PULSE_RATIO		 1309 //"set pulser ratio\0"

#define	MOTION_DONE			 1312 //"motion done\0"

#define	GET_IO_STATUS		 1313 //"get io status\0"

#define	SET_ALM		 1314 //"set alm\0"
#define	SET_INP		 1315 //"set inp\0"
#define	SET_ERC		 1316 //"set erc\0"
#define	SET_SERVO	 1317 //"set servo\0"
#define	SET_SD		 1318 //"set sd\0"
#define	SET_EL		 1319 //"set el\0"

#define	INT_CONTROL			 1360 //"int control\0"
#define	SET_INT_FACTOR		 1361 //"set INT factor\0"
#define	SET_AXIS_STOP_INT	 1362 //"set_axis stop int\0"
#define	MASK_AXIS_STOP_INT	 1363 //"mask_axis stop int\0"


#define	GET_POSITION		 1323 //"get position\0"
#define	SET_POSITION		 1324 //"set position\0"
#define	GET_COMMAND			 1325 //"get command\0"
#define	SET_COMMAND			 1326 //"set command\0"
#define	GET_ERROR_COUNTER	 1327 //"get error counter\0"
#define	RESET_ERROR_COUNTER	 1328 //"reset error counter\0"
#define	GET_GENERAL_COUNTER	 1329 //"get general counter\0"
#define	SET_GENERAL_COUNTER	 1330 //"set general counter\0"
#define	GET_TARGET_POS		 1331 //"get target pos\0"
#define	RESET_TARGET_POS	 1332 //"reset target pos\0"
#define	GET_RESET_COMMAND	 1333 //"get reset command\0"
#define	CHECK_RDP			 1334 //"check rdp\0"

#define	SET_LTC_LOGIC		1335  //"set ltc logic\0"
#define	GET_LATCH_LOGIC		1336  //"get latch logic\0"
#define	SET_SOFT_LIMIT		1337  //"set soft limit\0"
#define	ENABLE_SOFT_LIMIT	1338  //"enable soft limit\0"
#define	DISABLE_SOFT_LIMIT	1339  //"disale soft limit\0"
#define	SET_ERROR_COUNTER_CHECK	1340  //"set error counter check\0"
#define	SET_GENERAL_COMPARATOR	1341  //"set general comparator\0"
#define	SET_TRIGGER_COMPARATOR	1342  //"set trigger comparator\0"
#define	CHECK_COMPARE_DATA		1343  //"check compare data\0"
#define	CHECK_COMPARE_STATUS	1344  //"check compare status\0"
#define	SET_AUTO_COMPARE		1345  //"set auto compare\0"
#define	BUILD_COMPARE_FUNCTION	1346  //"build compare function\0"
#define	BUILD_COMPARE_TABLE		1347  //"build compare table\0"
#define	COMP_V_CHANGE			1348  //"comp v change\0"

#define	SET_CONTINUOUS_MOVE		1349  //"set continuous move\0"
#define	CHECK_CONTINUOUS_BUFFER	1350  //"check continuous buffer\0"


#define	SET_SYNC_OPTION			1357  //"set sync option\0"
#define	SET_SYNC_STOP_MODE		1358  //"set sync stop mode\0"

#define SET_SD_STOP				1360
#define SET_AXIS_OPTION			1361
#define SET_AXIS_RDP			1362
#define SET_CMP_VALUE			1363


#define START_A_ARC_XY			1370
#define START_R_ARC_XY			1371
#define START_A_ARC_ZU			1372
#define START_R_ARC_ZU			1373
#define START_A_ARC2			1374
#define	START_R_ARC2			1375



#endif       // __DRIVER_H__ 




