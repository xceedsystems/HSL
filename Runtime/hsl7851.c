
#include "global.h"

// System
PCI_Info pci_Info[12];
//I16 Maximum_Satellite[MAX_PCI_CARDS][MAX_CONNECT_INDEX];  //the last slave ID in used
//I16 Module_Type[MAX_PCI_CARDS][MAX_CONNECT_INDEX][MAX_SLAVE_NO];
// DIO
//I16 DIO_Output_Logic[MAX_PCI_CARDS][MAX_CONNECT_INDEX][MAX_SLAVE_NO];
//I16 DIO_Input_Logic[MAX_PCI_CARDS][MAX_CONNECT_INDEX][MAX_SLAVE_NO];
// AIO
//U8 Start_Read_AI[MAX_PCI_CARDS][MAX_CONNECT_INDEX];
//U8 AI_Channel_Status[MAX_PCI_CARDS][MAX_CONNECT_INDEX][MAX_SLAVE_NO][16];
//F64 AI_Buffer[MAX_PCI_CARDS][MAX_CONNECT_INDEX][MAX_SLAVE_NO][16];
//AIO_Data AIO_Info[MAX_PCI_CARDS][MAX_CONNECT_INDEX][MAX_SLAVE_NO];
//U16 AI16AO2_Command_Counter[MAX_PCI_CARDS][MAX_CONNECT_INDEX][MAX_SLAVE_NO];
// Motion
//U16 M_FH_Type[MAX_PCI_CARDS];
//U16 Maximum_Motion_Number[MAX_PCI_CARDS][MAX_CONNECT_INDEX]; //on line 4xmo numbers( 1 means one module not two )
//U16 M_Switch_ID[MAX_PCI_CARDS][MAX_CONNECT_INDEX][MAX_MOTION_CARDS]; //紀錄每個4xmo的第一個ID
//U16 M_FirmwareFlag[MAX_PCI_CARDS][MAX_CONNECT_INDEX][MAX_MOTION_CARDS]; //紀錄4xmo firmware version 的標示

int slaveId[MAX_CONNECT_INDEX][MAX_SLAVE_NO];
int MotionIdArray[MAX_CONNECT_INDEX][MAX_SLAVE_NO];





U16 FNTYPE W_REMOTE_Initial(U16 card_ID) {return 0;} ;
U16 FNTYPE W_REMOTE_Close(U16 card_ID) {return 0;} ;
U16 FNTYPE W_REMOTE_Auto_Start(U16 card_ID, U16 connect_index) {return 0;} ;
U16 FNTYPE W_REMOTE_Start(U16 card_ID, U16 connect_index, U16 max_slave_No) {return 0;} ;
U16 FNTYPE W_REMOTE_Stop(U16 card_ID, U16 connect_index) {return 0;} ;
U16 FNTYPE W_REMOTE_Connect_Status(U16 card_ID, U16 connect_index, U16 slave_No, U8 *sts_data) {return 0;} ;
U16 FNTYPE W_REMOTE_Slave_Live(U16 card_ID, U16 connect_index, U16 slave_No, U8 *live_data) {return 0;} ;

void FNTYPE W_REMOTE_Software_Reset(U16 card_ID) {return ;} ;
void FNTYPE W_REMOTE_Get_IRQ_Channel(U16 card_ID, U16 *irq_no) {return ;} ;

U16 FNTYPE W_REMOTE_DIO_In(U16 card_ID, U16 connect_index, U16 slave_No, U32 *in_data) {return 0;} ;
U16 FNTYPE W_REMOTE_DIO_Channel_In(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, U16 *in_data) {return 0;} ;
U16 FNTYPE W_REMOTE_DIO_Out(U16 card_ID, U16 connect_index, U16 slave_No, U32 out_data) {return 0;} ;
U16 FNTYPE W_REMOTE_DIO_Channel_Out(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, U16 out_data) {return 0;} ;
U16 FNTYPE W_REMOTE_DI8DO8_DIO_In(U16 card_ID, U16 connect_index, U16 slave_No, U16 *in_data) {return 0;} ;
U16 FNTYPE W_REMOTE_DI8DO8_DIO_Channel_In(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, U16 *in_data) {return 0;} ;
U16 FNTYPE W_REMOTE_DI8DO8_DIO_Out(U16 card_ID, U16 connect_index, U16 slave_No, U16 out_data) {return 0;} ;
U16 FNTYPE W_REMOTE_DI8DO8_DIO_Channel_Out(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, U16 out_data) {return 0;} ;
U16 FNTYPE W_REMOTE_Read_DIO_Out(U16 card_ID, U16 connect_index, U16 slave_No, U32 *out_data_in_ram) {return 0;} ;
U16 FNTYPE W_REMOTE_Read_DIO_Channel_Out(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, U16 *out_channel_data_in_ram) {return 0;} ;
U16 FNTYPE W_REMOTE_Read_DI8DO8_Out(U16 card_ID, U16 connect_index, U16 slave_No, U16 *out_data_in_ram) {return 0;} ;
U16 FNTYPE W_REMOTE_Read_DI8DO8_Channel_Out(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, U16 *out_channel_data_in_ram) {return 0;} ;
U16 FNTYPE W_REMOTE_Set_In_Out_Logic(U16 card_ID, U16 connect_index, U16 slave_No, U16 Input_Logic, U16 Output_Logic) {return 0;} ;

U16 FNTYPE W_REMOTE_AI_Start_Read(U16 card_ID, U16 connect_index) {return 0;} ;
U16 FNTYPE W_REMOTE_AI_Stop_Read(U16 card_ID, U16 connect_index) {return 0;} ;
U16 FNTYPE W_REMOTE_AI_Channel_In(U16 card_ID, U16 connect_index, U16 slave_No, U16 channel, F64 *ai_data) {return 0;} ;
U16 FNTYPE W_REMOTE_AI_SetConfig(U16 card_ID, U16 connect_index, U16 slave_No, U16 signal_range, U16 signal_type, U16 cjc_status) {return 0;} ;
U16 FNTYPE W_REMOTE_AI_GetConfig(U16 card_ID, U16 connect_index, U16 slave_No, U16 *signal_range, U16 *signal_type, U16 *cjc_status) {return 0;} ;
U16 FNTYPE W_REMOTE_AI_OffsetCalib(U16 card_ID, U16 connect_index, U16 slave_No) {return 0;} ;
U16 FNTYPE W_REMOTE_AI_SpanCalib(U16 card_ID, U16 connect_index, U16 slave_No) {return 0;} ;
U16 FNTYPE W_REMOTE_AI_WriteDefault(U16 card_ID, U16 connect_index, U16 slave_No) {return 0;} ;
U16 FNTYPE W_REMOTE_AI_SetCJCOffset(U16 card_ID, U16 connect_index, U16 slave_No, F64 CJC_data2) {return 0;} ;
U16 FNTYPE W_REMOTE_AI_GetCJCOffset(U16 card_ID, U16 connect_index, U16 slave_No, F64 *CJC_data) {return 0;} ;
U16 FNTYPE W_REMOTE_AI_SetChannelStatus(U16 card_ID, U16 connect_index, U16 slave_No, U32 enable_chans) {return 0;} ;
U16 FNTYPE W_REMOTE_AI_GetChannelStatus(U16 card_ID, U16 connect_index, U16 slave_No, U32 *enable_chans) {return 0;} ;

U16 FNTYPE W_REMOTE_Timer_Set(U16 card_ID, U16 c1, U16 c2) {return 0;} ;
U16 FNTYPE W_REMOTE_TMRINT_Enable(U16 card_ID, HANDLE *phEvent) {return 0;} ;
U16 FNTYPE W_REMOTE_TMRINT_Disable(U16 card_ID) {return 0;} ;

U16 FNTYPE W_REMOTE_DIO_Memory_In(U16 card_ID, U16 connect_index, U16 *data_in) {return 0;} ;
U16 FNTYPE W_REMOTE_DIO_Memory_Out(U16 card_ID, U16 connect_index, U16 *data_out) {return 0;} ;

I16 FNTYPE vlcREMOTE_Initial(I16 CardType) {return 0;} ;  ///vlc will call this to init 
I16 FNTYPE vlc785x_Initial(I16 CardType) {return 0;} ;  ///vlc will call this to init 

I16 FNTYPE REMOTE_get_slave_ID_type(I16 cardNo, I16 connect_index, I16 sat_id, I16 *type, I16 *count) {return 0;} ;

I16 FNTYPE GetMotionSw_id(I16 card_ID, I16 connect_index, I16 slaveAddr, I16 *axisaddr) {return 0;} ;/// , I16 *slaveId, I16 *MotionId) {return 0;} ;


/*
U16  W_REMOTE_Initial(U16 card_ID) {return 0;} ;
U16  W_REMOTE_Close(U16 card_ID) {return 0;} ;
U16  W_REMOTE_Auto_Start(U16 card_ID, U16 set_ID) {return 0;} ;
U16  W_REMOTE_Start(U16 card_ID, U16 set_ID, U16 sat_num) {return 0;} ;
U16  W_REMOTE_Stop(U16 card_ID, U16 set_ID) {return 0;} ;
U16  W_REMOTE_Connect_Status(U16 card_ID, U16 set_ID, U16 slave_No, U8* sts_data) {return 0;} ;
U16  W_REMOTE_Slave_Live(U16 card_ID, U16 set_ID, U16 slave_No, U8 *live_data) {return 0;} ;

void  W_REMOTE_Software_Reset(U16 card_ID) {return 0;} ;
void  W_REMOTE_Get_IRQ_Channel(U16 card_ID, U16 *irq_no ) {return 0;} ;

U16  W_REMOTE_DIO_In(U16 card_ID, U16 set_ID, U16 slave_No, U32* in_data) {return 0;} ;
U16  W_REMOTE_DIO_Channel_In(U16 card_ID, U16 set_ID, U16 slave_No, U16 channel, U16* in_data) {return 0;} ;
U16  W_REMOTE_DIO_Out(U16 card_ID, U16 set_ID, U16 slave_No, U32 out_data) {return 0;} ;
U16  W_REMOTE_DIO_Channel_Out(U16 card_ID, U16 set_ID, U16 slave_No, U16 channel, U16 out_data) {return 0;} ;

U16  W_REMOTE_Counter_In(U16 card_ID, U16 set_ID, U16 slave_No, U16 counter_No, U16* in_data) {return 0;} ;
*/