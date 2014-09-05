
#ifndef PTP_H
#define PTP_H

#include "ud_str.h"
//#include "types.h"
#define S16_T  signed short  
#define U16_T unsigned short
#define U8_T  unsigned char  
#define U32_T unsigned int
#define S32_T int;
#define TIMERESOLUTION_PTP            50

//#define	T_HEARTBEAT_SEC		15
//#define	T_INACTIVITY_SEC  	60

#define	N_RETRIES		      1
#define	T_CONN_REQ		15000 / TIMERESOLUTION_PTP
#define	T_CONN_RSP		15000 / TIMERESOLUTION_PTP
#define	T_FRAME_ABORT	 2000 / TIMERESOLUTION_PTP
#define	T_HEARTBEAT		15000 / TIMERESOLUTION_PTP
#define	T_INACTIVITY    60000 / TIMERESOLUTION_PTP
#define	T_RESPONSE		 5000 / TIMERESOLUTION_PTP


U16_T CalcBlockDataCRC( U8_T *, U16_T length,U16_T crcValue );//KR new code
//S16_T Calc_CRC_and_toggle_activity_light( U8_T *addr, S32_T length, S16_T CRC );
//S16_T ReceiveFrameAvailable( STR_PTP *ps );
//void RemoveReceivedEntry( STR_PTP *ps );
//FRAME_ENTRY *SendFreeEntry(  S16_T reply );
//FRAME_ENTRY *SendFrameAvailable(  S16_T reply );
//void RemoveSentEntry( FRAME_ENTRY *frame_ptr );
//void PTP_Send_frame( FRAME_ENTRY *frame, S16_T frame_type, S16_T dest,STR_PTP *ps );
S16_T send_octet( S16_T octet, U8_T **buffer);

void PTP_connection_sm(void );
void PTP_reception_sm(void );
void PTP_transmission_sm(void );

void vStartPTPTask(void);
//int router( int service, int command, char *dat, Uint length,STR_PTP *ps, int send );
void PTP_Receive_task(void) ;
void PTP_Receive_Frame_FSM( void);


#endif


