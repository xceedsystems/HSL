#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "type_def.h"

//#define REMOTE_WINDOWS
//#define REMOTE_LINUX
//#define REMOTE_RTX
///#define REMOTE_INTIME

	
#define	VENDOR_ID		0x10B5
#define	DEVICE_ID9030	0x9030
#define	DEVICE_ID9060	0x9060

#ifdef REMOTE_LINUX
	#include <linux/ioctl.h>
	#include <linux/fs.h>
#endif

#ifdef REMOTE_INTIME
///include intime header here
//#include "stdafx.h"
#include "rt.h"
#define FNTYPE _stdcall

#endif


/**************************************************/
/*		Faile Check Timer Use					  */
/**************************************************/
#define Timer_ID1 0  //For CardID=0, Connect Index = 0
#define Timer_ID2 1  //For CardID=0, Connect Index = 1

/**************************************************/
/*		Data Definitions are place here.		  */
/**************************************************/
#define		TARGET_RESOLUTION	1         // 1-millisecond target resolution
#define		MEMORY_SIZE			0x20000

//#define		MAX_PCI_CARDS		12
#define		MAX_CONNECT_INDEX	2
#define		MAX_SLAVE_NO		31 //// 64
#define		MAX_MOTION_CARDS	15

#define     MAX_FULL_4XMO_ID     57
#define     MAX_HALF_4XMO_ID     60
#define		MAX_DIDO32_ID		 61

#define     TRUE	            1
#define     FALSE		        0

#define     HIGH			    1
#define     LOW					0

#define     IC8259_1			0x20
#define     IC8259_2			0xA0
#define     EOI					0x20

#define 	TMR_INT				0
#define 	CHK2_INT			1
#define		SCANR_INT			2
#define		SCANW_INT			3
#define		DREQ_INT			4

#define 	LINT1				1
#define		LINT2				2

//#define 	ENABLE				1
//#define		DISABLE				0

#define		SIGEVENT1			61
#define		SIGEVENT2			62

#define CRC_KEY 0x07 //crc-8

//Parameter Setting //define in 4xmo kernel
#define		PVA0		1	// Value0
#define		PVA1		2	// Value1
#define		PVA2		3	// Value2
#define		PVA3		4	// Value3
#define		PVA4		5	// Value4
#define		PVA5		6	// Value5
#define		PVA6		7	// Value6
#define		PVA7		8	// Value7
#define		PVA8		9	// Value8
#define		PVA9		10	// Value9
#define		PSTV		11	// StrVel
#define		PMXV		12	// MaxVel
#define		PARC		13	// ArcVel
#define		PTAC		14	// Tacc
#define		PTDC		15	// Tdec
#define		PSVA		16	// SVacc
#define		PSVD		17	// SVdec
#define		PDTA		18	// DistA
#define		PSTA		19	// StrVelA
#define		PMXA		20	// MaxVelA
#define		PTAA		21	// TaccA
#define		PTDA		22	// TdecA
#define		PSAA		23	// SVaccA
#define		PSDA		24	// SVdecA
#define		PPTP		25	// P_Value1
#define		PPTV		26	// P_Vel
#define		PPTA		27	// P_Value2

// Setting & Getting Function
#define		SOTM		40	// set_pls_outmode
#define		SITM		41	// set_pls_iptmode
#define		SSRC		42	// set_feedback_src
#define		SFSR		43	// fix_speed_range
#define		SUSR		44	// unfix_speed_range
#define		GCSP		45	// get_current_speed
#define		GVS1		46	// verify_speed
#define		GVS2		47	// verify_speed
#define		SMVR		48	// set_move_ratio
#define		SBCP		49	// backlash_comp
#define		SVIB		50	// suppress_vibration
#define		SIDP		51	// set_idle_pulse
#define		SFAS		52	// set_fa_speed
#define		SHCG		53	// set_home_config
#define		SALM		54	// set_alm
#define		SELM		55	// set_el
#define		SELL		56	// set_el_logic
#define		SINP		57	// set_inp
#define		SERC		58	// set_erc
#define		SSVO		59	// set_servo
#define		SSLD		60	// set_sd
#define		GIOS		61	// get_io_status
#define		GPOS		62	// get_position
#define		SPOS		63	// set_position
#define		GCMD		64	// get_command
#define		SCMD		65	// set_command
#define		GECN		66	// get_error_counter
#define		RECN		67	// reset_error_counter
#define		GGCN		68	// get_general_counter
#define		SGCN		69	// set_general_counter
#define		GTPO		70	// get_target_pos
#define		RTPO		71	// reset_target_pos
#define		GRCM		72	// get_rest_command
#define		CRDP		73	// check_rdp
#define		SLTC		74	// set_ltc_logic
#define		GLTD		75	// get_latch_data
#define		SSLM		76	// set_soft_limit
#define		ESLM		77	// enable_soft_limit
#define		DSLM		78	// disable_soft_limit
#define		SECC		79	// set_error_counter_check
#define		SGCP		80	// set_general_comparator
#define		STCP		81	// set_trigger_comparator
#define		STRT		82	// set_compare_output_type
#define		CCPD		83	// check_compare_data
#define		CCPS		84	// check_compare_status
#define		SRMA		85	// set_tr_move_all
#define		SAMA		86	// set_ta_move_all
#define		SSRM		87	// set_sr_move_all
#define		SSAM		88	// set_sa_move_all
#define		SSOP		89	// set_sync_option
#define		SSSM		90	// set_sync_stop_mode
#define		SWDO		91	// write_do
#define		GRDI		92	// read_di
#define		SPTT		93	// set_point_table
#define		SRPT		94	// set_random_points
#define		STBD		95	// send compare table data
#define		BCTB		96	// build_compare_table
#define		BCFN		97	// build_compare_function
#define		SACP		98	// set_auto_compare
#define		SCPD		99	// set_compare_dir
#define		ORGO		100 // set_org_offset
#define     REST        101 // get_abnormal_stop_status

//Motion Function command defination
#define		MSLDS		120	// sd_stop
#define		MEMGS		121	// emg_stop
#define		MTVMV		122	// tv_move
#define		MSVMV		123	// sv_move
#define		MVCHG		124	// v_change
#define		MRMOV		125 // start_tr_move
#define		MAMOV		126 // start_ta_move
#define		MSRMV		127	// start_sr_move
#define		MSAMV		128	// start_sa_move
#define		MPCHG		129	// p_change
#define		MTRXY		130	// start_tr_move_xy
#define		MTAXY		131	// start_ta_move_xy
#define		MSRXY		132	// start_sr_move_xy
#define		MSAXY		133	// start_sa_move_xy
#define		MTRZU		134	// start_tr_move_zu
#define		MTAZU		135	// start_ta_move_zu
#define		MSRZU		136	// start_sr_move_zu
#define		MSAZU		137	// start_sa_move_zu
#define		MTRL2		138	// start_tr_line2
#define		MTAL2		139	// start_ta_line2
#define		MSRL2		140	// start_sr_line2
#define		MSAL2		141	// start_sa_line2
#define		MTRL3		142	// start_tr_line3
#define		MTAL3		143	// start_ta_line3
#define		MSRL3		144	// start_sr_line3
#define		MSAL3		145	// start_sa_line3
#define		MTRL4		146	// start_tr_line4
#define		MTAL4		147	// start_ta_line4
#define		MSRL4		148	// start_sr_line4
#define		MSAL4		149	// start_sa_line4
#define		MRAXY		150	// start_r_arc_xy
#define		MAAXY		151	// start_a_arc_xy
#define		MRAZU		152	// start_r_arc_zu
#define		MAAZU		153	// start_a_arc_zu
#define		MRAR2		154	// start_r_arc2
#define		MAAR2		155	// start_a_arc2
#define		MTRAX		156	// start_tr_arc_xy
#define		MTAAX		157	// start_ta_arc_xy
#define		MSRAX		158	// start_sr_arc_xy
#define		MSAAX		159	// start_sa_arc_xy
#define		MTRAZ		160	// start_tr_arc_zu
#define		MTAAZ		161	// start_ta_arc_zu
#define		MSRAZ		162	// start_sr_arc_zu
#define		MSAAZ		163	// start_sa_arc_zu
#define		MTRA2		164	// start_tr_arc2
#define		MTAA2		165	// start_ta_arc2
#define		MSRA2		166	// start_sr_arc2
#define		MSAA2		167	// start_sa_arc2
#define		MHMOV		168	// home_move
#define		MEHOM		169	// escape_home
#define		MHSCH		170	// home_search
#define		MDONE 		171	// motion_done
#define		MSTMA		172	// start_move_all
#define		MSPMA		173	// stop_move_all
#define		MMPTR		174	// move_point_range
#define		MMPTB		175	// move_point_table

#define		GCDL		240	// G_Code Download
#define		GCDO		241 // G_Code Download OK
#define		GCSL		242	// G_Code Start line
#define		GCST		243 // G_Code Stop or Resume
#define		GCPS		244 // G_Code Pause
#define		GCCL		245 // G_Code get_current_line
#define		GCSF		246	// G_Code Function Setting

#define     GCKS		251 // Get GD->checkSum and reset it to 0;(For Download kernel)
#define		GVER		252	// Get CPLD & Kernel Version
#define 	SFAD		253	// set flash address & data
#define		SFDL		254	// start flash download
#define		SRSG		255 // reset address length
//dont exceed 255
/**************************************************/
/*		Structures are placed here. 			  */
/**************************************************/
#if defined ( REMOTE_WINDOWS )
	
	#include <windows.h>
	CRITICAL_SECTION RW_Lock;
	CRITICAL_SECTION RW_Lock2[MAX_MOTION_CARDS]; //目前只有kernel update時才有用到，更新的速度增加
	//CRITICAL_SECTION REMOTE_Lock[MAX_PCI_CARDS][2][64];

	#define _OUTPORTB _outp
	#define _OUTPORTW _outpw
	#define _OUTPORTD _outpd
	#define _INPORTB _inp
	#define _INPORTW _inpw
	#define _INPORTD _inpd

	void DoEvents(void);
	U32 uSecSleep(int uSecond);
	#define _Sleep(t) Sleep(t)
	#define _uSleep(t) uSecSleep(t)


#elif defined ( REMOTE_INTIME )

	#define SUCCESS					0
	#define TASK_TIMEOUT			-1
	#define	ERR_CREATE_TASK			-2
	#define ERR_DELETE_TASK			-3
	#define ERR_DELETE_SEMAPHORE	-4
	#define ERR_CREATE_SEMAPHORE	-5
	#define ERR_DELETE_DESCRIPTOR	-10
	#define ERR_PCI_OUTOF_IOIMAGE	-20
	#define ERR_PCI_INVALID_REGSIZE -21

	#define _OUTPORTB outbyte
	#define _OUTPORTW outhword
	#define _OUTPORTD outword
	#define _INPORTB inbyte
	#define _INPORTW inhword
	#define _INPORTD inword
	#define HANDLE	RTHANDLE
	#define	SEMAPHORE RTHANDLE
	RTHANDLE  RW_Lock;
	RTHANDLE RW_Lock2[MAX_MOTION_CARDS]; ///map as intime handler for semaphore


#define EnterCriticalSection(p) Wait (p)
#define LeaveCriticalSection(p) Kick (p)
	#define InitializeCriticalSection(p) CreateSemaphore (p)
	#define DeleteCriticalSection(p) DeleteSemaphore (p)  
//	#define EnterCriticalSection(p) pthread_mutex_lock(p)  //Wait (RTHANDLE semaphore)
//	#define LeaveCriticalSection(p) pthread_mutex_unlock(p)// Kick (RTHANDLE semaphore)
	#define CloseHandle(p)	DeleteSemaphore(p)
	#define _Sleep(t) RtSleep(t) 
//	#define _uSleep(t) RtSleep(t)

	#define _uSleep(t) knRtSleep( t)
	#define Sleep(t) RtSleep(t) 
	#define wsprintf(p,q){}
////CreateEvent neeed to change it to interrrupt
#define LOBYTE(x)           (((int)(x)) & 0xFF)
#define HIBYTE(x)           ((((int)(x)) >> 8) & 0xFF)


#elif defined ( REMOTE_LINUX )

	#define _OUTPORTB outb
	#define _OUTPORTW outw
	#define _OUTPORTD outl
	#define _INPORTB inb
	#define _INPORTW inw
	#define _INPORTD inl

	pthread_mutex_t work_mutex;
	#define EnterCriticalSection(p) pthread_mutex_lock(p)
	#define LeaveCriticalSection(p) pthread_mutex_unlock(p)
	#define RW_Lock work_mutex
	#define _Sleep(t) sleep(1000*t)
	#define _uSleep(t) usleep(t)

#elif defined ( REMOTE_RTX )

	#include <windows.h>
	#include <rtapi.h>
	CRITICAL_SECTION RW_Lock;
	//CRITICAL_SECTION REMOTE_Lock[MAX_PCI_CARDS][2][64];

	#define _OUTPORTB(t, s) RtWritePortUchar((PUCHAR)t, (UCHAR)s)
	#define _OUTPORTW(t, s) RtWritePortUshort((PUSHORT)t, s)
	#define _OUTPORTD(t, s) RtWritePortUlong((PULONG)t, s)
	#define _INPORTB(t) RtReadPortUchar((PUCHAR)t)
	#define _INPORTW(t) RtReadPortUshort((PUSHORT)t)
	#define _INPORTD(t) RtReadPortUlong((PULONG)t)

	U32 uSecSleep(int uSecond);
	#define _Sleep(t) Sleep(t)
	#define _uSleep(t) uSecSleep(t)

#endif

typedef struct
{
	U16 control[64];  
	U16 data_out[64]; //DO
	U16 data_in[64];  //DI
	U16 counter[6][64];
	U16 sidr[64]; // counter 7
	U16 drc[64];  // DRC(data renewal check)
	U16 data_reg[10]; 
} satellite_data;



typedef struct
{
	U8 data[0x100]; ///control starts at 1600

} PLX9030_control;


#ifdef REMOTE_WINDOWS

	typedef struct 
	{
		U32 RetValue[15];
	}RET_INFO, *PRET_INFO;

	typedef struct
	{
        U16	   busNo;  // for Win95/98 Use
		U16    initFlag;
        U16    devFunc;
        U32    lcrBase;
        U32    iobaseAddr;
		U32    membaseAddr;
        U32    irqNo;
        U32    ViewAddr;
		U32    MemLen;
        volatile PLX9030_data *share1;
        volatile PLX9030_control *share2;
		U8	   Interrupt_Source; //for 7851/52
		HANDLE hEvent;  //timer
        HANDLE hDevice;
		U16    int_src; //interrupt source for 7853/54
		HANDLE hDiEvent; //Di events for 7853/54
		U8	   DiEventFlag; // 0:mean disable, 1:mean DI interrupt enable
		U8	   timerEventFlag; // 0:mean disable, 1:mean DI interrupt enable

	}PCI_Info;

#elif defined ( REMOTE_INTIME )
	typedef unsigned char UINT8;
	typedef unsigned short UINT16;

	typedef char SINT8;
	typedef short SINT16;

	typedef void far *LPVOID;
	
	typedef struct 
	{
		U32 RetValue[15];
	}RET_INFO, *PRET_INFO;

	typedef struct
	{
        U16	   busNo;  // for Win95/98 Use
		U16    initFlag;
        U16    devFunc;
        U32    lcrBase;
        UINT8*    iobaseAddr;
		UINT8*    membaseAddr;
        U32    irqNo;
        void*  ViewAddr;

		U32    MemLen;
		U8	   Interrupt_Source; //for 7851/52
		RTHANDLE hEvent;  //timer
        RTHANDLE hDevice;
		U16    int_src; //interrupt source for 7853/54
		RTHANDLE hDiEvent; //Di events for 7853/54
		U8	   DiEventFlag; // 0:mean disable, 1:mean DI interrupt enable
		U8	   timerEventFlag; // 0:mean disable, 1:mean DI interrupt enable

	}PCI_Info;



#elif defined ( REMOTE_LINUX )

	typedef struct
	{
        U16    initFlag;
        U32    ViewAddr;
	    U32    MemLen;
		volatile satellite_data  *share1;
        volatile satellite_data  *share2;
		U8	   Interrupt_Source;
        U8     Owner_Set;
        int    DeviceID;
        struct sigaction sig_act;
	}PCI_Info;
	
	typedef struct
	{
		U16 IntCh;
		U16 IntSet;
	}INT_SETTING;

	typedef struct
	{
		U16 Int1;
		U16 Int2;
	}INT_STATUS;

	typedef struct
	{
		U16 Clock1;
		U16 Clock2;
	}TIMER_SETTING;

#elif defined ( REMOTE_RTX )

	typedef struct
	{
		U16	initFlag;
		U16	busNo;
		U16	devFunc;
		U32	lcrBase;
		U32	baseAddr;
		U32	iobaseAddr;  // timer=8254
		U16	irqNo;
		U16	Reference;
		BOOLEAN IsrEnable;
		U32	MemLen;
		
		PVOID ViewAddr;
		satellite_data *share1;
		satellite_data *share2;
		U8 Interrupt_Source;
		HANDLE hInterrupt;

	} PCI_Info;

#endif

//This structure is designed to store additional information about slave modules. 
typedef struct
{
	U16 Last_Channel;
	U16 DAC_for_DA0;
	U16 DAC_for_DA1;
	U16 DAC_for_pre;
	U16 DAC_for_post;
	U16 DAC_for_AD_gain;
	U16 DP_for_DA0_gain;
	U16 DP_for_DA1_gain;
	U16 AD_gain;
} AIO_Data;


/****************************************************************/
/*			Slave Module ID Definition						    */
/****************************************************************/
#define BEGIN_ID			 	0xa0
#define END_ID					0xc4  

#define HSL_8R8DI			 	0xa0
#define HSL_8R16DI			 	0xa1
#define HSL_8DI8DO_NP           0xa2    
#define HSL_16DI16DO_NP			0xa3
#define HSL_8DO				    0xa4
#define HSL_16DO				0xa5
#define HSL_8DI					0xa6
#define HSL_16DI				0xa7
#define HSL_16R					0xa8
#define HSL_16R8DI				0xa9
#define HSL_4CI8DO				0xaa
#define HSL_4CI8DO_H			0xab
#define HSL_8PO8DI				0xac

#define HSL_16AI8DO_V		 	0xb1
#define HSL_16AI8DO_A		 	0xb2
#define HSL_4AO8DI_V		    0xb3
#define HSL_4AO8DI_A		    0xb4
#define HSL_8AO_V			    0xb5
#define HSL_8AO_A			    0xb6
#define HSL_8AI_HR			    0xb7
#define HSL_AI16_M_T			0xb8
#define HSL_8AI_RT			    0xb9
#define HSL_AI16AO2_M_VV		0xba
#define HSL_AI16AO2_M_AV		0xbb
#define HSL_UB47        		0xbc //similar with HSL_AI16AO2_M_VV

#define HSL_4XMO_CG_N_FD		0Xc1
#define HSL_4XMO_CG_P_FD		0Xc2
#define HSL_4XMO_CG_N_HD		0Xc3
#define HSL_4XMO_CG_P_HD		0Xc4

#define HSL_32DI_DB				0xe1
#define HSL_32DO_DB				0xe2

/**************************************************************/
#define OS_Unknown				(I16)0
#define OS_MSDOS				(I16)1
#define OS_WIN_31				(I16)2
#define OS_WIN_95				(I16)3
#define OS_WIN_98				(I16)4
#define OS_WIN_351				(I16)5
#define OS_WIN_40				(I16)6
#define OS_WIN_2K				(I16)7
#define OS_WIN_XP				(I16)8
#define OS_LINUX				(I16)9
/**************************************************************/

/**************************************************************/
/*   Following definition is for Win95/98 initialization use  */
/**************************************************************/
// pci configuration register
#define PCI_CS_VENDOR_ID         0x00
#define PCI_CS_DEVICE_ID         0x02
#define PCI_CS_COMMAND           0x04
#define PCI_CS_STATUS            0x06
#define PCI_CS_REVISION_ID       0x08
#define PCI_CS_CLASS_CODE        0x09
#define PCI_CS_CACHE_LINE_SIZE   0x0C
#define PCI_CS_MASTER_LATENCY    0x0D
#define PCI_CS_HEADER_TYPE       0x0E
#define PCI_CS_BIST              0x0F
#define PCI_CS_BASE_ADDRESS_0    0x10
#define PCI_CS_BASE_ADDRESS_1    0x14
#define PCI_CS_BASE_ADDRESS_2    0x18
#define PCI_CS_BASE_ADDRESS_3    0x1C
#define PCI_CS_BASE_ADDRESS_4    0x20
#define PCI_CS_BASE_ADDRESS_5    0x24
#define PCI_CS_Subsystem_VID     0x2C
#define PCI_CS_Subsystem_ID      0x2E
#define PCI_CS_EXPANSION_ROM     0x30
#define PCI_CS_INTERRUPT_LINE    0x3C
#define PCI_CS_INTERRUPT_PIN     0x3D
#define PCI_CS_MIN_GNT           0x3E
#define PCI_CS_MAX_LAT           0x3F

#define SET_IO_BASEADDR		     1
#define SET_INT_INIT			 2
#define SET_INT_STOP			 3

#ifdef REMOTE_WINDOWS
	typedef struct _DAS_IOT_SETEVT
	{// for Timer interrupt event
		HANDLE	    hEvent;
		USHORT		Index;
	}
	DAS_IOT_SETEVT;
#endif

#ifdef REMOTE_INTIME
	typedef struct _DAS_IOT_SETEVT
	{// for Timer interrupt event
		RTHANDLE    hEvent;
		USHORT		Index;
	}
	DAS_IOT_SETEVT;
#endif


#if defined (REMOTE_VLCTYPE)
	typedef unsigned int UINT32;
	typedef int SINT32; 

 typedef struct _TASK 
	{
    UINT16      hTask;          
    SEMAPHORE   Semaphore;      
    void*       pStack;         
    UINT16      bBusy;          
    UINT16      Error;          
    void*       IrqThunk;       
    UINT16      level;          
    UINT16      align;
	} TASK, * LPTASK; 

#endif



#ifdef __cplusplus
}
#endif

#endif
