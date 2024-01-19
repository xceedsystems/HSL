/****************************************************************************/
/*  Copyright (c) 1995-2000, ADLink Technology Inc.  All rights reserved.   */
/*                                                                          */
/*  File Name   :   REMOTE.H                                                           */
/*  Purpose     :   Header file for REMOTE-init.C module                       */
/*  Date        :   04/29/2000                                              */
/*  Revision    :   1.00                                                    */
/*  Programmer  :   Rick Lee                                                */
/****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************/
/*      Typedef  Definitions                                                */
/****************************************************************************/
typedef unsigned char   U8;
typedef short           I16;
typedef unsigned short  U16;
typedef long            I32;
typedef unsigned long   U32;
typedef float           F32;
typedef double          F64;
//typedef char            Boolean;

#define     MAX_PCI_CARDS       16

#define     TRUE                1
#define     FALSE               0

#define     HIGH                1
#define     LOW                 0

#define FNTYPE _stdcall
/****************************************************************************/
/*      Function  Declerations                                              */
/****************************************************************************/

U16 FNTYPE W_REMOTE_Initial(U16 card_ID) ;
U16 FNTYPE W_REMOTE_Close(U16 card_ID) ;
U16 FNTYPE W_REMOTE_Auto_Start(U16 card_ID, U16 connect_index) ;
U16 FNTYPE W_REMOTE_Start(U16 card_ID, U16 connect_index, U16 max_slave_No) ;
U16 FNTYPE W_REMOTE_Stop(U16 card_ID, U16 connect_index) ;
U16 FNTYPE W_REMOTE_Connect_Status(U16 card_ID, U16 connect_index, U16 slave_No, U8 *sts_data) ;
U16 FNTYPE W_REMOTE_Slave_Live(U16 card_ID, U16 connect_index, U16 slave_No, U8 *live_data) ;

void FNTYPE W_REMOTE_Software_Reset(U16 card_ID) ;
void FNTYPE W_REMOTE_Get_IRQ_Channel(U16 card_ID, U16 *irq_no) ;

U16 FNTYPE W_REMOTE_DIO_In(U16 card_ID, U16 connect_index, U16 slave_No, U32 *in_data) ;
U16 FNTYPE W_REMOTE_DIO_Channel_In(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, U16 *in_data) ;
U16 FNTYPE W_REMOTE_DIO_Out(U16 card_ID, U16 connect_index, U16 slave_No, U32 out_data) ;
U16 FNTYPE W_REMOTE_DIO_Channel_Out(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, U16 out_data) ;
U16 FNTYPE W_REMOTE_DI8DO8_DIO_In(U16 card_ID, U16 connect_index, U16 slave_No, U16 *in_data) ;
U16 FNTYPE W_REMOTE_DI8DO8_DIO_Channel_In(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, U16 *in_data) ;
U16 FNTYPE W_REMOTE_DI8DO8_DIO_Out(U16 card_ID, U16 connect_index, U16 slave_No, U16 out_data) ;
U16 FNTYPE W_REMOTE_DI8DO8_DIO_Channel_Out(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, U16 out_data) ;
U16 FNTYPE W_REMOTE_Read_DIO_Out(U16 card_ID, U16 connect_index, U16 slave_No, U32 *out_data_in_ram) ;
U16 FNTYPE W_REMOTE_Read_DIO_Channel_Out(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, U16 *out_channel_data_in_ram) ;
U16 FNTYPE W_REMOTE_Read_DI8DO8_Out(U16 card_ID, U16 connect_index, U16 slave_No, U16 *out_data_in_ram) ;
U16 FNTYPE W_REMOTE_Read_DI8DO8_Channel_Out(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, U16 *out_channel_data_in_ram) ;
U16 FNTYPE W_REMOTE_Set_In_Out_Logic(U16 card_ID, U16 connect_index, U16 slave_No, U16 Input_Logic, U16 Output_Logic) ;

U16 FNTYPE W_REMOTE_AI_Start_Read(U16 card_ID, U16 connect_index) ;
U16 FNTYPE W_REMOTE_AI_Stop_Read(U16 card_ID, U16 connect_index) ;
U16 FNTYPE W_REMOTE_AI_Channel_In(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, F64 *ai_data) ;
U16 FNTYPE W_REMOTE_AI_SetConfig(U16 card_ID, U16 connect_index, U16 slave_No, U16 signal_range, U16 signal_type, U16 cjc_status) ;
U16 FNTYPE W_REMOTE_AI_GetConfig(U16 card_ID, U16 connect_index, U16 slave_No, U16 *signal_range, U16 *signal_type, U16 *cjc_status) ;
U16 FNTYPE W_REMOTE_AI_OffsetCalib(U16 card_ID, U16 connect_index, U16 slave_No) ;
U16 FNTYPE W_REMOTE_AI_SpanCalib(U16 card_ID, U16 connect_index, U16 slave_No) ;
U16 FNTYPE W_REMOTE_AI_WriteDefault(U16 card_ID, U16 connect_index, U16 slave_No) ;
U16 FNTYPE W_REMOTE_AI_SetCJCOffset(U16 card_ID, U16 connect_index, U16 slave_No, F64 CJC_data2) ;
U16 FNTYPE W_REMOTE_AI_GetCJCOffset(U16 card_ID, U16 connect_index, U16 slave_No, F64 *CJC_data) ;
U16 FNTYPE W_REMOTE_AI_SetChannelStatus(U16 card_ID, U16 connect_index, U16 slave_No, U32 enable_chans) ;
U16 FNTYPE W_REMOTE_AI_GetChannelStatus(U16 card_ID, U16 connect_index, U16 slave_No, U32 *enable_chans) ;

U16 FNTYPE W_REMOTE_Timer_Set(U16 card_ID, U16 c1, U16 c2) ;
U16 FNTYPE W_REMOTE_TMRINT_Enable(U16 card_ID, HANDLE *phEvent) ;
U16 FNTYPE W_REMOTE_TMRINT_Disable(U16 card_ID) ;

U16 FNTYPE W_REMOTE_DIO_Memory_In(U16 card_ID, U16 connect_index, U16 *data_in) ;
U16 FNTYPE W_REMOTE_DIO_Memory_Out(U16 card_ID, U16 connect_index, U16 *data_out) ;

I16 FNTYPE vlcREMOTE_Initial(I16 CardType) ;  ///vlc will call this to init 
I16 FNTYPE vlc785x_Initial(I16 CardType) ;  ///vlc will call this to init 

I16 FNTYPE REMOTE_get_slave_ID_type(I16 cardNo, I16 connect_index, I16 sat_id, I16 *type, I16 *count) ;

I16 FNTYPE GetMotionSw_id(I16 card_ID, I16 connect_index, I16 slaveAddr, I16 *axisaddr) ;/// , I16 *slaveId, I16 *MotionId) ;


/*
U16  W_REMOTE_Initial(U16 card_ID) ;
U16  W_REMOTE_Close(U16 card_ID) ;
U16  W_REMOTE_Auto_Start(U16 card_ID, U16 set_ID) ;
U16  W_REMOTE_Start(U16 card_ID, U16 set_ID, U16 sat_num) ;
U16  W_REMOTE_Stop(U16 card_ID, U16 set_ID) ;
U16  W_REMOTE_Connect_Status(U16 card_ID, U16 set_ID, U16 slave_No, U8* sts_data) ;
U16  W_REMOTE_Slave_Live(U16 card_ID, U16 set_ID, U16 slave_No, U8 *live_data) ;

void  W_REMOTE_Software_Reset(U16 card_ID) ;
void  W_REMOTE_Get_IRQ_Channel(U16 card_ID, U16 *irq_no ) ;

U16  W_REMOTE_DIO_In(U16 card_ID, U16 set_ID, U16 slave_No, U32* in_data) ;
U16  W_REMOTE_DIO_Channel_In(U16 card_ID, U16 set_ID, U16 slave_No, U16 channel, U16* in_data) ;
U16  W_REMOTE_DIO_Out(U16 card_ID, U16 set_ID, U16 slave_No, U32 out_data) ;
U16  W_REMOTE_DIO_Channel_Out(U16 card_ID, U16 set_ID, U16 slave_No, U16 channel, U16 out_data) ;

U16  W_REMOTE_Counter_In(U16 card_ID, U16 set_ID, U16 slave_No, U16 counter_No, U16* in_data) ;
*/
#ifdef __cplusplus
}
#endif
