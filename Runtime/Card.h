/***************************************************************

                Card.h             

   This file contains the interface to the manufacturer code

****************************************************************/


#ifndef  __CARD_H__
#define  __CARD_H__

#include "..\IE\include\PlxApi.h"


#define ERR_No_Error	0

int Init( void* const dp, P_ERR_PARAM const lpErrors);
int TestConfig( LPDRIVER_INST const pNet, P_ERR_PARAM const lpErrors);
int REMOTE_ReadIO( LPDEVICE_INST const pNet,  unsigned short  CardId , VOID *Dest );
int REMOTE_WriteIO( LPDEVICE_INST const pNet,  unsigned short  CardId , unsigned long data );

void AdLinkMotion( const LPDRIVER_INST pNet, SPECIAL_INST* const pData );
void AdLinkInterpol( const LPDRIVER_INST pNet, SPECIAL_INST* const pData );
void AdLinkF64( const LPDRIVER_INST pNet, SPECIAL_INST* const pData );
void AdLinkI16( const LPDRIVER_INST pNet, SPECIAL_INST* const pData );
int ADlinkReadIO( LPDEVICE_INST const pDevice, int bn, VOID *Dest );



///  int HSL_initial (PLX_DEVICE_OBJECT *pDevice);
int HSL_Init (U16 card_ID, U16 connect_index, U8 Limit, U16 t_rate, U8 last_addr);
int HSL_stop (U16 card_ID, U16 connect_index);
int HSL_auto_start  (U16 card_ID, U16 connect_index, U8 Limit,U16 t_rate, U8 last_addr);
int read_channel_output(U16 card_ID, U16 connect_index, U16 slave_No, U32 *in_data);
int write_channel_output(U16 card_ID, U16 connect_index, U16 slave_No, U32 out_data);
int write_channel_output16(U16 card_ID, U16 connect_index, U16 slave_No, U16 out_data);
int write_channel_output8(U16 card_ID, U16 connect_index, U16 slave_No, U8 out_data);

int read_channel_input(U16 card_ID, U16 connect_index, U16 slave_No, U32 *in_data);
int read_channel_input16(U16 card_ID, U16 connect_index, U16 slave_No, U16 *in_data);
int read_channel_input8(U16 card_ID, U16 connect_index, U16 slave_No, U8 *in_data);

void write_LCR_word(WORD plx_lcr_ba, WORD n, WORD value);


#endif      /* __CARD_H__ */

