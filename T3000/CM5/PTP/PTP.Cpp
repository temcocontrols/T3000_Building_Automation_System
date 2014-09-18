#include "stdafx.h"
#include "bacnet.h"
#include "dlmstp.h"
#include "..\ud_str.h"

//#include "product.h"
//#include "..\..\T3000\CM5\ROUTER.H"
//#ifndef PTP

#include <string.h>
//#include "main.h"
//#include "product.h"


#include "ptp.h"
#include "timer.h"
//#include "netlayer.h"
//#include "mstp.h"
#include "stdio.h"
//#include "delay.h"
#include "rs485.h"

#define TRUE      1

#if 0
xTaskHandle far xHandlePTPreceive;
xTaskHandle far xHandlePTPconnection;
xTaskHandle far xHandlePTPreception;
xTaskHandle far xHandlePTPtransmission;
#endif

#define MAXAPCI                 40

char      npci_buffer[MAXAPCI];

S16_T init_idle_state( S16_T r );

const char  trigger_sequence[8] = "BACnet\x0d";

const U16_T  crc16_table[256] = {
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

const U8_T  crc8_table[256] = {
0x00, 0xfe, 0xff, 0x01, 0xfd, 0x03, 0x02, 0xfc,
0xf9, 0x07, 0x06, 0xf8, 0x04, 0xfa, 0xfb, 0x05,
0xf1, 0x0f, 0x0e, 0xf0, 0x0c, 0xf2, 0xf3, 0x0d,
0x08, 0xf6, 0xf7, 0x09, 0xf5, 0x0b, 0x0a, 0xf4,
0xe1, 0x1f, 0x1e, 0xe0, 0x1c, 0xe2, 0xe3, 0x1d,
0x18, 0xe6, 0xe7, 0x19, 0xe5, 0x1b, 0x1a, 0xe4,
0x10, 0xee, 0xef, 0x11, 0xed, 0x13, 0x12, 0xec,
0xe9, 0x17, 0x16, 0xe8, 0x14, 0xea, 0xeb, 0x15,
0xc1, 0x3f, 0x3e, 0xc0, 0x3c, 0xc2, 0xc3, 0x3d,
0x38, 0xc6, 0xc7, 0x39, 0xc5, 0x3b, 0x3a, 0xc4,
0x30, 0xce, 0xcf, 0x31, 0xcd, 0x33, 0x32, 0xcc,
0xc9, 0x37, 0x36, 0xc8, 0x34, 0xca, 0xcb, 0x35,
0x20, 0xde, 0xdf, 0x21, 0xdd, 0x23, 0x22, 0xdc,
0xd9, 0x27, 0x26, 0xd8, 0x24, 0xda, 0xdb, 0x25,
0xd1, 0x2f, 0x2e, 0xd0, 0x2c, 0xd2, 0xd3, 0x2d,
0x28, 0xd6, 0xd7, 0x29, 0xd5, 0x2b, 0x2a, 0xd4,
0x81, 0x7f, 0x7e, 0x80, 0x7c, 0x82, 0x83, 0x7d,
0x78, 0x86, 0x87, 0x79, 0x85, 0x7b, 0x7a, 0x84,
0x70, 0x8e, 0x8f, 0x71, 0x8d, 0x73, 0x72, 0x8c,
0x89, 0x77, 0x76, 0x88, 0x74, 0x8a, 0x8b, 0x75,
0x60, 0x9e, 0x9f, 0x61, 0x9d, 0x63, 0x62, 0x9c,
0x99, 0x67, 0x66, 0x98, 0x64, 0x9a, 0x9b, 0x65,
0x91, 0x6f, 0x6e, 0x90, 0x6c, 0x92, 0x93, 0x6d,
0x68, 0x96, 0x97, 0x69, 0x95, 0x6b, 0x6a, 0x94,
0x40, 0xbe, 0xbf, 0x41, 0xbd, 0x43, 0x42, 0xbc,
0xb9, 0x47, 0x46, 0xb8, 0x44, 0xba, 0xbb, 0x45,
0xb1, 0x4f, 0x4e, 0xb0, 0x4c, 0xb2, 0xb3, 0x4d,
0x48, 0xb6, 0xb7, 0x49, 0xb5, 0x4b, 0x4a, 0xb4,
0xa1, 0x5f, 0x5e, 0xa0, 0x5c, 0xa2, 0xa3, 0x5d,
0x58, 0xa6, 0xa7, 0x59, 0xa5, 0x5b, 0x5a, 0xa4,
0x50, 0xae, 0xaf, 0x51, 0xad, 0x53, 0x52, 0xac,
0xa9, 0x57, 0x56, 0xa8, 0x54, 0xaa, 0xab, 0x55
};

const U8_T  frame_array[16][8] =
{
 { 0x06, 0x055, 0x0ff, 0x00, 0x00, 0x00, 0x0ee, 0x00 },
 { 0x06, 0x055, 0x0ff, 0x01, 0x00, 0x00, 0x076, 0x00 },
 { 0x06, 0x055, 0x0ff, 0x02, 0x00, 0x00, 0x0dd, 0x00 },
 { 0x06, 0x055, 0x0ff, 0x03, 0x00, 0x00, 0x045, 0x00 },
 { 0x06, 0x055, 0x0ff, 0x04, 0x00, 0x00, 0x088, 0x00 },
 { 0x07, 0x055, 0x0ff, 0x05, 0x00, 0x00, 0x010, 0x90 },
 { 0x06, 0x055, 0x0ff, 0x06, 0x00, 0x00, 0x0bb, 0x00 },
 { 0x06, 0x055, 0x0ff, 0x07, 0x00, 0x00, 0x023, 0x00 },
 { 0x06, 0x055, 0x0ff, 0x08, 0x00, 0x00, 0x022, 0x00 },
 { 0x06, 0x055, 0x0ff, 0x09, 0x00, 0x00, 0x0ba, 0x00 },
 { 0x07, 0x055, 0x0ff, 0x0a, 0x00, 0x00, 0x010, 0x91 },
 { 0x06, 0x055, 0x0ff, 0x0b, 0x00, 0x00, 0x089, 0x00 },
 { 0x06, 0x055, 0x0ff, 0x0c, 0x00, 0x00, 0x044, 0x00 },
 { 0x06, 0x055, 0x0ff, 0x0d, 0x00, 0x00, 0x0dc, 0x00 },
 { 0x06, 0x055, 0x0ff, 0x0e, 0x00, 0x00, 0x077, 0x00 },
 { 0x06, 0x055, 0x0ff, 0x0f, 0x00, 0x00, 0x0ef, 0x00 }
};

#if 0
ROUTING_TABLE Routing_table[MAX_Routing_table];
#endif
FRAME_ENTRY 								 SendFrame[MAX_SEND_FRAMES];
UNITDATA_PARAMETERS  		NL_PARAMETERS;
STR_PTP  					Ptp_para;
extern int Station_NUM;
void init_PTP_structures( void )
{
	int i;
//	STR_PTP *ptr;
  	FRAME_ENTRY *pframe;

	memset( SendFrame, 0, sizeof( FRAME_ENTRY )* MAX_SEND_FRAMES );
	pframe= &SendFrame[0];
	for( i=0; i<MAX_SEND_FRAMES; i++, pframe++ )
  		pframe->number = i;

#if 0 //
	memset( &Routing_table, '\0', /*MAX_Routing_table**/sizeof( Routing_Table ) );
#endif
	memset( &NL_PARAMETERS, '\0', sizeof( UNITDATA_PARAMETERS ) );
	NL_PARAMETERS.primitive = DL_INVALID;
	
	memset( &Ptp_para, '\0', sizeof( STR_PTP ) );
	//ReceiveFrame_0 = 0;
//	Ptp_para.tx_end = 1;
//	Ptp_para.tx_wake_up = 0x0ff;
	
	Ptp_para.PTP_DLE_mask = 0x0FF;
	Ptp_para.reception_blocked = Q_NOT_BLOCKED;
	Ptp_para.receive_frame_free = 1;
//	Ptp_para.send_frame_free = 1;
	Ptp_para.transmission_blocked = 1;
	
	Ptp_para.PTP_transmission_state = TR_IDLE;
	Ptp_para.PTP_reception_state = REC_IDLE;
	Ptp_para.PTP_connection_state = DISCONNECTED;
	Ptp_para.Rec_frame_state = RX_IDLE;
	Ptp_para.rec_trigger_sequence = 0;
	
	Ptp_para.InactivityTimer = 0;   // 60s
	Ptp_para.ResponseTimer = 0;		// 5s 
	Ptp_para.HeartbeatTimer = 0;  	// 15s
	
	Ptp_para.DataBuffer = NULL;
#if 0
	Recievebuf_Initialize();	
#endif

//	Ptp_para.MSTP_MASTER_state = MSTP_MASTER_IDLE;


//	password_needed = 0;
//	RemoveReceivedEntry(ptr);
#if 0
	Device_Basic_Setting.reg.network_number = 9999;
#endif
   // Setting_Info.reg.network_number = 9999;
//	Station_NUM = 1;
//	panel_net_info.panel_type = MINI_T3000;
//	panel_net_info.network_number = 9999;
//	panel_net_info.panel_number = Station_NUM;
//	strcpy( panel_net_info.network_name, BACNET_VENDOR_NAME );
//	strcpy( panel_net_info.panel_name, "T3000 Mini" );

}






void RemoveReceivedEntry( STR_PTP *ps )
{
		ps->received_valid_frame = 0;
		ps->received_invalid_frame = 0;
//		ps->rec_frame_ready = 0;
		ps->reception_blocked = Q_NOT_BLOCKED;
		ps->receive_frame_free = 1;
}




FRAME_ENTRY *SendFreeEntry( S16_T reply )
{
	FRAME_ENTRY * frame_ptr;
	S16_T i = 0;

	//if( no_of_send_frame_used >= MAX_SEND_FRAMES )   return NULL;
	//vSemaphoreCreateBinary((xSemaphoreHandle)Using_send_frame);
  	//no_of_send_frame_used++;
	frame_ptr = SendFrame;

  	do
	{
		if( frame_ptr->being_used == 0)
		{
			frame_ptr->being_used = 1;
			frame_ptr->locked = 1;
			frame_ptr->reply_flag = reply;
			return frame_ptr;
    	}
    	i++;
		frame_ptr++;
	}
 	while( i < MAX_SEND_FRAMES );

 	return NULL;
}



FRAME_ENTRY *SendFrameAvailable(S16_T reply )
{
	 FRAME_ENTRY *frame_ptr;
	 S16_T i = 0;
//	STR_PTP *ps;
	
	//ps = Ptp_para;
	//frame_ptr = Ptp_para
	frame_ptr = SendFrame;
	
	do
	{
		if( frame_ptr->being_used == 1 && frame_ptr->locked == 0 )
		//if(GetByteBit(frame_ptr->flag,0,1) && !GetByteBit(frame_ptr->flag,1,1) && GetByteBit(frame_ptr->flag,2,1) == port)
		{
			if( reply < 0 )	return frame_ptr;
			else
				if( /*GetByteBit(frame_ptr->flag,3,1) == reply*/frame_ptr->reply_flag == reply )
					return frame_ptr;
		}
		i++;
		frame_ptr++;
	}
	while( i < MAX_SEND_FRAMES );	
	return NULL;
}

/*
 * ----------------------------------------------------------------------------
 * Function Name: RemoveSentEntry
 * Purpose: 
 * Params:   
 * Returns:
 * Note: 
 * ----------------------------------------------------------------------------
 */
void RemoveSentEntry( FRAME_ENTRY *frame_ptr )
{
	if(frame_ptr)
	{
		frame_ptr->being_used = 0;	
		frame_ptr->locked = 0;
		frame_ptr->reply_flag = 0;
		frame_ptr->delay = 0;
		
		//no_of_send_frame_used--;
	}
}



 
void PTP_Send_frame( FRAME_ENTRY *frame, int frame_type, int dest,STR_PTP *ps )
{
	U32_T Time_frame;
	U8_T HeaderCRC;
	U16_T Send_index;
	U8_T work_byte;
	U16_T i;
	U8_T *send_buffer;

	if( frame == NULL )
	{
		ps->SendFrameTimer = 5;
	    Time_frame = 0;
		send_buffer = (unsigned char*)&frame_array[frame_type][1];
		Send_index = (int)frame_array[frame_type][0];
	}
	else
	{  	
#if 0
		send_buffer = &uart1_data_buffer[0];
#endif
		if(frame_type != -1)
   		{
			Send_index = 2;
			*send_buffer = 0x55;
			send_buffer++;
			*send_buffer = 0x0ff;
			send_buffer++;
	
			HeaderCRC = 0x0ff;
				/* Frame Type */
			if( frame_type == DATA_0 )
			{
				if( ps->TxSequence_number )
				{
					frame_type = DATA_1;
					frame->FrameType = frame_type;
				}
			}

			work_byte = frame_type;
			HeaderCRC = crc8_table[ work_byte ^ HeaderCRC ];
			Send_index += send_octet( (int)work_byte, &send_buffer );
	
			/* Destination Address  */
			*send_buffer = dest;
			send_buffer++;
			Send_index++;
			HeaderCRC = crc8_table[ dest ^ HeaderCRC ];
			/* Source Address  */
			if(!ps->nack0received)         /* panelOff for RS485 */
			{
				*send_buffer = Station_NUM;
				HeaderCRC = crc8_table[ Station_NUM ^ HeaderCRC ];
			}
			else
			{
			 	*send_buffer = ps->PTP_reception_state; /* OS - original station for RS485 */
				HeaderCRC = crc8_table[ ps->PTP_reception_state ^ HeaderCRC ];
			}
			/*			*send_buffer = Station_NUM;		*/
			send_buffer++;
			Send_index++;
			/*			HeaderCRC = crc8_table[ Station_NUM ^ HeaderCRC ];*/

			if( frame )
				i = frame->Length;
			else
				i = 0;
			/* MSB of frame->Length */
			work_byte = i >> 8;
			HeaderCRC = crc8_table[ work_byte ^ HeaderCRC ];
			Send_index += send_octet( (int)work_byte, &send_buffer);
	
			/* LSB of frame->Length */
			work_byte = i & 0x0ff;
			HeaderCRC = crc8_table[ work_byte ^ HeaderCRC ];
			Send_index += send_octet( (int)work_byte, &send_buffer);
	
			work_byte = ~HeaderCRC;
			Send_index += send_octet( (int)work_byte, &send_buffer);
	
			if( i )
			{
				frame->DataCRC = 0x0ffff;
				frame->DataCRC = CalcBlockDataCRC( (unsigned char *)frame->Buffer,frame->Length, frame->DataCRC );

				for( i=0; i<frame->Length; i++ )
				{
					Send_index += send_octet( (int)frame->Buffer[i], &send_buffer);
				}
				frame->DataCRC = ~frame->DataCRC;
				i = frame->DataCRC;
				/* MSB of frame->DataCRC */
				work_byte = i & 0x0ff;
				Send_index += send_octet( (int)work_byte, &send_buffer);

				/* LSB of frame->DataCRC */
				work_byte = i >> 8;
				Send_index += send_octet( (int)work_byte, &send_buffer);
			}
#if 0	//
			send_buffer = uart1_data_buffer;
#endif
		}
	}

	ps->ResponseTimer = 0;
	ps->SilenceTimer = 0; 

	RS485_Send_Frame(NULL,(uint8_t *)send_buffer,Send_index);
#if 0 //
  	uart_send_string(send_buffer,Send_index,1);
    Timer_Silence_Reset();
#endif

}




S16_T send_octet( S16_T octet, U8_T **buffer)
{
	if( octet == 0x10 || octet == 0x11 || octet == 0x13 ) 
	{
		**buffer = 0x10;
		(*buffer)++;
		**buffer = octet | 0x80;
		(*buffer)++;
		return 2;
	}
	**buffer = (U8_T)octet;
	(*buffer)++;
	return 1;
}


S16_T init_idle_state( S16_T r )
{
	STR_PTP *ps;
	/* PORT_STATUS_variables *ps; */

	ps = &Ptp_para;

//	if(r)
//	{
//		router( N_UNITDATArequest, Initialize_Routing_Table, NULL, 0, ps, 1);
//	}	
#if 0	//
	Routing_table[0].status = PTP_INSTALLED;
#endif
	ps->reception_blocked = Q_NOT_BLOCKED;
	ps->receive_frame_free = 1;
//	ps->send_frame_free = 1;
	ps->transmission_blocked = 1;
	
	NL_PARAMETERS.primitive = DL_INVALID;
	ps->PTP_transmission_state = TR_IDLE;
	ps->PTP_reception_state = REC_IDLE;
	ps->PTP_connection_state = DISCONNECTED;
	ps->Rec_frame_state = RX_IDLE;
	ps->rec_trigger_sequence = 0;


	ps->InactivityTimer = 0;   // 60s
	ps->ResponseTimer = 0;		// 5s 
	ps->HeartbeatTimer = 0;  	// 15s


//	memset( Routing_table[port].Port.networks_list, 0, sizeof(Routing_table[port].Port.networks_list));
/*	if( ps->link_type==SERIAL_LINK || ps->link_type == MODEM_LINK )
	 	outputd( "ATZ\r", 4, ps->port);*/

		/*SET COMM PORT*/


	return 0;
}
HANDLE hThread_Receive = NULL;
HANDLE hThread_connection = NULL;
HANDLE hThread_reception = NULL;
HANDLE hThread_transmission = NULL;
HANDLE hThread_timer = NULL;

DWORD nThreadID_Receive;
DWORD nThreadID_connection;
DWORD nThreadID_reception;
DWORD nThreadID_transmission;
DWORD nThreadID_timer;



void vStartPTPTask(void)
{  	
#if 0
	MSTP_Flag.TransmitPacketPending = false;
	init_PTP_structures();
	init_idle_state(1);
	sTaskCreate(PTP_Receive_task, (const signed portCHAR * const)"ptp_receive",512, NULL, tskIDLE_PRIORITY + 2, (xTaskHandle *)&xHandlePTPreceive); 
 	sTaskCreate(PTP_connection_sm, (const signed portCHAR * const)"ptp_connection",512, NULL, tskIDLE_PRIORITY + 2, (xTaskHandle *)&xHandlePTPconnection); 
	sTaskCreate(PTP_reception_sm, (const signed portCHAR * const)"ptp_reception",512, NULL, tskIDLE_PRIORITY + 2, (xTaskHandle *)&xHandlePTPreception); 
	sTaskCreate(PTP_transmission_sm, (const signed portCHAR * const)"ptp_transmission",512, NULL, tskIDLE_PRIORITY + 2, (xTaskHandle *)&xHandlePTPtransmission); 
#endif
}

/*
 * Note:  PTP connect state machine have 5 state : DISCONNECTED, INBOUND, OUTBOUND, CONNECTED, DISCONNECTING
 */
DWORD WINAPI    PTP_connection_sm(LPVOID lpVoid)
//void PTP_connection_sm(void )
{
	FRAME_ENTRY *rec_frame, *frame;
	STR_PTP *ps;
//	U8_T physical_connection_state;
	U16_T PTP_connection_sleep;
#if 0 //
	portTickType xDelayPeriod = ( portTickType ) 50 / portTICK_RATE_MS;
#endif
//	physical_connection_state = 1;
	
	for( ;; )
	{
		ps = &Ptp_para;
		Sleep(1);
#if 0
		vTaskDelay(xDelayPeriod);		
#endif
		PTP_connection_sleep = 0;
		switch( ps->PTP_connection_state )
		{
			case DISCONNECTED:
				/* connect Outbound
					if a DL-CONNECT.request is received, establish a physical connection
				*/
#if 0 //fance				
				if( NL_PARAMETERS.primitive == DL_CONNECT_REQUEST )  // if a DL-CONNECT.request is received, establish a physical connection
				{
#endif
					/* start sending */
					ps->SendFrameTimer = 12;
#if 0 //
					uart_send_string(trigger_sequence, 7,1);   /* send trigger squence "BACnet<CR>"*/
#endif
					ps->RetryCount = 0;
					ps->ResponseTimer = 0;  
					ps->PTP_connection_state = OUTBOUND;
/*					PTP_connection_sleep =  T_CONN_REQ;*/
//	        		physical_connection_state = 1;


#if (DEBUG_UART1)
	sprintf(debug_str," PTP send trigger \r\n ");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif

					break;
#if 0 //fance
				}
#endif
				/*	connect inbound
					if a physical layer connection has been made and the "BACnet<CR>" trigger sequence is receive
					transmit ConnectRequestFrame
				*/

				RS485_Send_Frame(NULL,(uint8_t *)&trigger_sequence,8);
				Sleep(1000);
				/* for test */
				if( ps->rec_trigger_sequence >= 7 &&  ps->rec_trigger_sequence != 255)
				{
				//	flag_test = 0;
#if (DEBUG_UART1)
	sprintf(debug_str," PTP receive trigger \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
					ps->PTP_connection_state = INBOUND;
					PTP_Send_frame( NULL, CONNECT_REQUEST, 0, ps );  // 	transmit ConnectRequestFrame
					ps->rec_trigger_sequence = 0;
/*					PTP_connection_sleep = T_CONN_RSP;*/
//	        		physical_connection_state = 1;
          			ps->RetryCount = 0;
					ps->ResponseTimer = 0;  
					break;
				}

				if( ps->received_valid_frame )
				{
					/*	Unwanted frame received */
#if (DEBUG_UART1)
	sprintf(debug_str,"\r\n  DISCONNECTED receive valid frame ");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
					RemoveReceivedEntry( ps );
					PTP_connection_sleep = 0;
					break;
				}
#if 0 //
	        	Routing_table[0].status = PTP_INSTALLED;	 
#endif
				break;

			case OUTBOUND:
			/* Connect Request Received */	
				if( ps->received_valid_frame ) //if ReceiveValidFrame is TRUE and FrameType is equal to Connect Reques		
				{
#if (DEBUG_UART1)
	sprintf(debug_str," outboud receive valid frame \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif

					if( Ptp_para.FrameType/*Ptp_para.FrameType*/ == CONNECT_REQUEST )
					{
						PTP_Send_frame( NULL, CONNECT_RESPONSE, 0, ps ); // transmit a Connect Response frame
#if 0 //
						Routing_table[0].status = PTP_ACTIVE;	
#endif
						ps->TxSequence_number = 0;
						ps->RxSequence_number = 0;
#if (DEBUG_UART1)
	sprintf(debug_str," outboud receive request frame \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
						// issue a DL_CONNECT.confirm to notify the network layer that a connection has been established
//						router( N_UNITDATArequest, I_Am_Router_To_Network, NULL, 0, ps, 2);	  
						RemoveReceivedEntry( ps );
						ps->PTP_connection_state = CONNECTED;
						break;
					}
				}

				if( ps->ResponseTimer >= T_CONN_REQ )
				{
#if (DEBUG_UART1)
	sprintf(debug_str," outboud response timer timeout \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
					if( ps->RetryCount < N_RETRIES )
					{
					/* Connect Request Timeout */
						ps->RetryCount++;  
						ps->ResponseTimer = 0;
						ps->SendFrameTimer = 12;
#if 0 //
						uart_send_string(trigger_sequence, 7,1);   /* send trigger squence "BACnet<CR>"*/
#endif
						RS485_Send_Frame(NULL,(uint8_t *)&trigger_sequence,8);

						PTP_connection_sleep = T_CONN_REQ;
						ps->PTP_connection_state = OUTBOUND;  /*enter OUTBOUND status */
						break;
					}
					else
					{
#if (DEBUG_UART1)
	sprintf(debug_str," outboud Connect Request Failure \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif

					/*	Connect Request Failure	*/
						/* signal network layer	*/
						/* issue a DL-CONNECT.confirm to notify the network layer of the failure */
						ps->PTP_connection_state = DISCONNECTED;
						PTP_connection_sleep = 0;
						break;
					}
				}
				break;

			case INBOUND:
			/*	The net layer has recognized that the calling device wishes to
			establish a BACnet connection, and the local device is waiting for
			a Connect Response frame from the calling device	*/

			/*	Valid Connect Response Received	*/
				if( ps->received_valid_frame )
				{
#if (DEBUG_UART1)
	sprintf(debug_str," INBOUND received_valid_frame %u\r\n",(U16_T)Ptp_para.FrameType);
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
					if( Ptp_para.FrameType == CONNECT_RESPONSE )
					{
#if (DEBUG_UART1)
	sprintf(debug_str," INBOUND received connect response \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif

						RemoveReceivedEntry( ps );

//						if( !password_needed || password_OK )  //tbd:
/* if password is not need or a valid password is present in the data field of the frame */
						{
						//	ps->received_valid_frame = 0;
							ps->TxSequence_number = 0;
							ps->RxSequence_number = 0;
#if 0 //
							Routing_table[0].status = PTP_ACTIVE; 
							Routing_table[0].Port.network = 0;						
#endif
							ps->PTP_connection_state = CONNECTED;	
//							while( /*!Routing_table[port].Port.network && */ps->InactivityTimer < 100 ) ;
//							/* issue a DL-CONNECT.indication to notify the network layer of the connection */
//							router( N_UNITDATArequest, I_Am_Router_To_Network_Prop, NULL, 0, ps, 1);
							ps->InactivityTimer = 0;	
							break;
						}

					}
					if( Ptp_para.FrameType == DISCONNECT_REQUEST )
					{
#if (DEBUG_UART1)
	sprintf(debug_str," INBOUND received disconnect request \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
						RemoveReceivedEntry( ps );
						PTP_Send_frame( NULL, DISCONNECT_RESPONSE, 0, ps );
						ps->PTP_connection_state = DISCONNECTED;
						PTP_connection_sleep = 0;
						break;
					}
				}
				if( ps->ResponseTimer >= T_CONN_RSP )
				{ /* connect response timeout */
#if (DEBUG_UART1)
	sprintf(debug_str," INBOUND connect response timeout\r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
					if( ps->RetryCount >= N_RETRIES )
					{	
#if (DEBUG_UART1)
	sprintf(debug_str," INBOUND retry count too big\r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif

						ps->ResponseTimer = 0;
						ps->PTP_connection_state = DISCONNECTED;
						init_idle_state( 0);
						break;
					}
					else
					{ /*connect Reponse Failure */ 
#if (DEBUG_UART1)
	sprintf(debug_str," INBOUND connect Reponse Failure\r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif

						ps->RetryCount++;
						PTP_Send_frame( NULL, CONNECT_REQUEST, 0, ps );  /* transmit a Connect Request Frame */
						ps->PTP_connection_state = INBOUND;
						break;
					}
				} 
				break;

			case CONNECTED:
			/*	The connection procedure has been completed and the two devices
			are exchanging BACnet PDUs. The data link remains in this
			PTP_connection_state until termination	*/
				/*	Network disconnect	*/

				if( NL_PARAMETERS.primitive == DL_DISCONNECT_REQUEST )  
				{

#if (DEBUG_UART1)
	sprintf(debug_str," CONNECTED send DISCONNECT_REQUEST\r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif

					PTP_Send_frame( NULL, DISCONNECT_REQUEST, 0, ps );
					ps->PTP_connection_state = DISCONNECTING;
					PTP_connection_sleep = T_RESPONSE;
          			ps->ResponseTimer=0;
					break;
				}
			
				if( ps->received_valid_frame )
				{
#if (DEBUG_UART1)
	sprintf(debug_str," CONNECTED received_valid_frame  %u\r\n",(U16_T)Ptp_para.FrameType);
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif

					if( Ptp_para.FrameType == DISCONNECT_REQUEST )
					{	/*	Disconnect request received	*/

						RemoveReceivedEntry( ps );
						PTP_Send_frame( NULL, DISCONNECT_RESPONSE, 0, ps );
						ps->PTP_connection_state = DISCONNECTED;
						init_idle_state(1);
						/* issue a DL-CONNECT.indication to notify the network layer of the connection */
//						router(N_UNITDATArequest, I_Am_Router_To_Network_Prop, NULL, 0, ps ,1);
						break;
					}
					if( Ptp_para.FrameType == CONNECT_REQUEST )
					{	/*	Connect request received	*/
						RemoveReceivedEntry( ps );
						PTP_Send_frame( frame, CONNECT_RESPONSE, 0, ps );
						ps->PTP_connection_state = CONNECTED;
						/* issue a DL-DISCONNECT.indication to notify the network layer of the connection */
//						router(N_UNITDATArequest, I_Am_Router_To_Network_Prop, NULL, 0, ps ,1 );
						break;
					}
					/*	Unwanted frame received */
					RemoveReceivedEntry( ps );
				}
				if( ps->received_invalid_frame )
				{
#if (DEBUG_UART1)
	sprintf(debug_str," CONNECTED received_invalid_frame \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif

					/*	Unwanted frame received */
					RemoveReceivedEntry( ps );
				}

				/* Inactivity Timeout	*/
				if( ps->InactivityTimer > T_INACTIVITY )
				{	
#if (DEBUG_UART1)
	sprintf(debug_str," CONNECTED InactivityTimer timeout \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif

					ps->PTP_connection_state = DISCONNECTED;
					init_idle_state( 1);
					/*issue a DL-DISCONNECT.indicaion to notify the network layer of the disconnection */
//					router(N_UNITDATArequest, I_Am_Router_To_Network_Prop, NULL, 0, ps ,1);
					break;
				}

				/*	Connection Lost	*/
//				if( !physical_connection_state )
//				{
//#if (DEBUG_UART1)
//	sprintf(debug_str," CONNECTED Connection Lost \r\n");
//	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
//#endif
//
//					ps->PTP_connection_state = DISCONNECTED;
//					init_idle_state( 1);
//					/*issue a DL-DISCONNECT.indicaion to notify the network layer of the disconnection */
////					router(N_UNITDATArequest, I_Am_Router_To_Network_Prop, NULL, 0, ps ,1 );
//					break;
//				}				
				break;
			case DISCONNECTING:
			/*	The network layer has requested termination of the data link. The
			device is waiting for a Disconnect Response frame from the peer device*/
			//	msleep(T_RESPONSE);
				if( ps->received_valid_frame )
				{
#if (DEBUG_UART1)
	sprintf(debug_str," CONNECTED received_valid_frame %u \r\n",(U16_T)Ptp_para.FrameType);
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif					
					if( Ptp_para.FrameType == DISCONNECT_RESPONSE )
					{  /*	Disconnect response received */
						RemoveReceivedEntry( ps );
						ps->PTP_connection_state = DISCONNECTED;
						break;
					}
					else if( Ptp_para.FrameType == DISCONNECT_REQUEST )
					{  /*	Disconnect request received */
						RemoveReceivedEntry( ps );
						PTP_Send_frame( NULL, DISCONNECT_RESPONSE, 0, ps );
						ps->PTP_connection_state = DISCONNECTED;
						break;
					}
					else
					{	/* Unwanted Frame Received */
					//	ps->received_valid_frame = 0;
						ps->PTP_connection_state = DISCONNECTED;
						break;
					}
				}
				else
				{
					if( ps->ResponseTimer > T_RESPONSE )
					{  
#if (DEBUG_UART1)
	sprintf(debug_str," CONNECTED ResponseTimer timeout %u \r\n",(U16_T)ps->RetryCount);
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
						if( ps->RetryCount < N_RETRIES )
						{	/*Disconnect Response Timeout*/
							/*	Disconnect response timeout	*/
							ps->ResponseTimer = 0;
							ps->RetryCount++;
							PTP_Send_frame( NULL, DISCONNECT_REQUEST, 0, ps );
							PTP_connection_sleep = T_RESPONSE;
							ps->PTP_connection_state = DISCONNECTED;
							break;
						}
						else
						{	/*Disconnect Response failure */
							ps->PTP_connection_state = DISCONNECTED;
							break;
						}
					}						
				}
				init_idle_state( 1);
		}	
	}
	return 0;
}


/*
 * ----------------------------------------------------------------------------
 * Function Name: PTP_reception_sm
 * Purpose: PTP reception state machine
 * Params:   
 * Returns:
 * Note:  PTP reception state machine have 5 state : REC_IDLE, REC_READY, DATA, DATA_ACK, DATA_NAK
 * ----------------------------------------------------------------------------
 */
DWORD WINAPI    PTP_reception_sm(LPVOID lpVoid)
//void PTP_reception_sm( void)
{
//	FRAME_ENTRY *rec_frame/*, *frame*/;
//	U8_T ttframe[520];
	FRAME_ENTRY *frame;
	STR_PTP *ps;
	U8_T FrameType;
	U16_T loop=0 ;
#if 0	//
	portTickType xDelayPeriod = ( portTickType )50 / portTICK_RATE_MS;
#endif
	
	for( ; ; ) 
	{
#if 0
		vTaskDelay(xDelayPeriod);	
#endif	
		Sleep(1);
		#if 1
		{	
			ps = &Ptp_para;			
//			rec_frame = &Ptp_para.rec_frame[0];
//			frame = &Ptp_para.rec_frame[0];
			switch( ps->PTP_reception_state )
			{
				case REC_IDLE:
				/*	The receiver is waiting for the data link to be established
				 between the local device and the peer device. The receiver waits
					to be notified that a peer device is ready to communicate	*/
					/*	Connection Established	*/
					if( ps->PTP_connection_state == CONNECTED )
					{
						ps->RxSequence_number = 0;
						ps->reception_blocked = Q_NOT_BLOCKED;
						ps->PTP_reception_state = REC_READY;
						break;
					}
					break;
				case REC_READY:
				/*	The device is ready to receive frames from the peer device	*/
					if( ps->received_valid_frame )
					{	 
#if (DEBUG_UART1)
	sprintf(debug_str," REC_READY received_valid_frame %u \r\n",(U16_T)Ptp_para.FrameType);
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif

						//memcpy(frame, rec_frame, sizeof(FRAME_ENTRY));
						//for(loop = 0;loop < sizeof(FRAME_ENTRY);loop++ )
						//frame[loop] =  rec_frame[loop];//memcpy(frame, rec_frame, sizeof(FRAME_ENTRY));
						RemoveReceivedEntry( ps );
						//ps->received_valid_frame = 0;
						ps->InactivityTimer = 0;
						//memset(frame, '\0', sizeof(FRAME_ENTRY));
						//memcpy(frame, rec_frame, sizeof(FRAME_ENTRY));
						//memcpy(frame, rec_frame, sizeof(FRAME_ENTRY));
						/* Data Received */
						if( Ptp_para.FrameType == DATA_0 || Ptp_para.FrameType == DATA_1 )
						{
							ps->PTP_reception_state = DATA;	  
							continue;
						}
						/*	Data ACK */
						if( DATA_ACK_0_XOFF <= Ptp_para.FrameType &&
								Ptp_para.FrameType <= DATA_ACK_1_XON )
						{
							ps->PTP_reception_state = DATA_ACK;
							continue;
						}
					   	/* Data NCK */
						if( DATA_NAK_0_XOFF <= Ptp_para.FrameType &&
								Ptp_para.FrameType <= DATA_NAK_1_XON )
						{
							ps->PTP_reception_state = DATA_NAK;
							continue;
						}
						if( HEARTBEAT_XON <= Ptp_para.FrameType &&
								Ptp_para.FrameType <= HEARTBEAT_XOFF )
						{	
							ps->PTP_reception_state = DATA_NAK;
							continue;
						}
						/* test request */
//						if(  Ptp_para.FrameType == TEST_REQUEST )
//						{
//							ps->InactivityTimer = 0;
//							frame = SendFreeEntry(  0 );
//							if( frame )
//							{
//								frame->FrameType = TEST_RESPONSE;
//								frame->Length = rec_frame->Length;
//								//memcpy( frame->Buffer, rec_frame->Buffer, frame->Length );
//								RemoveReceivedEntry( ps );
//								PTP_Send_frame( frame, frame->FrameType, 0, ps );
//								RemoveSentEntry( frame );
//								ps->PTP_reception_state = REC_READY;
//								break;
//							}
//						}
//						/* test reponse */
//						if( frame->FrameType == TEST_RESPONSE )
//						{
//							/* issue a DL_UNITDATA.indication conveying the test_reponse data */
//						    MSTP_Flag.ReceivePacketPending = true;
//// TBD							NL_unitdata_ind( ps, rec_frame->Buffer, rec_frame->Length, rec_frame->Source  );
//							RemoveReceivedEntry( ps );
//							PTP_Send_frame( NULL, HEARTBEAT_XON, 0, ps );
//							ps->InactivityTimer = 0;
//							ps->PTP_reception_state = REC_READY;
//							break;
//
//						}
						break;
					}
					if( ps->received_invalid_frame )
					{
#if (DEBUG_UART1)
	sprintf(debug_str," REC_READY received_invalid_frame %u \r\n",(U16_T)Ptp_para.FrameType);
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
						/*	Bad Data0 / Full buffers	*/	
						//memcpy(frame, rec_frame, sizeof(FRAME_ENTRY));
						RemoveReceivedEntry( ps );	 /* Discard Frame */	
						ps->InactivityTimer = 0;
						if( Ptp_para.FrameType == DATA_0 )
						{
							if( ps->reception_blocked == Q_BLOCKED)
									FrameType = DATA_NAK_0_XOFF;
							else   /* NOT_BLOCKED or ALMOST_BLOCKED */
									FrameType = DATA_NAK_0_XON;
							PTP_Send_frame( NULL, FrameType, 0, ps );
							break;
						}
						/*	Bad Data1  /  Full buffers	*/
						if( Ptp_para.FrameType == DATA_1 )
						{								
							if( ps->reception_blocked == Q_BLOCKED )
									FrameType = DATA_NAK_1_XOFF;
							else	/* NOT_BLOCKED or ALMOST_BLOCKED */
									FrameType = DATA_NAK_1_XON;
							PTP_Send_frame( NULL, FrameType, 0, ps );
							break;
						}
						/* other bad frames */
						ps->PTP_reception_state = REC_READY;
						break;
					}
					/* disconnected */
					if( ps->PTP_connection_state == DISCONNECTED )
					{
						ps->PTP_reception_state = REC_IDLE;
						break;
					}
	        		break;
	
				case DATA:
				/*	In this state the device has received a Data frame for processing */
					ps->PTP_reception_state = REC_READY;

					if( Ptp_para.FrameType == DATA_0 )
					{
						RemoveReceivedEntry( ps );
						if( ps->RxSequence_number == 1 )  
						{ /*  duplication0 fullbuffers	/ duplication 0*/
								/*Discard frame */	
						//	if( ps->reception_blocked == Q_BLOCKED )
						//		FrameType = DATA_ACK_0_XOFF;
						//	else	/* NOT_BLOCKED or ALMOST_BLOCKED */
								FrameType = DATA_ACK_0_XON;
							PTP_Send_frame( NULL, FrameType, 0, ps );
						}
						else
						{	/* data0 fullbuffer / newdata0*/
						//	if( ps->reception_blocked == Q_BLOCKED )
						//	{   //	Last Data0_Full_Buffers									
						//			FrameType = DATA_NAK_0_XOFF;
						//	}
						//	else
						//	{   /*	New Data0	*/
								/*	DL_UNITDATA.indication */
							 	ps->RxSequence_number = 1;
						//	 	if( ps->reception_blocked == Q_NOT_BLOCKED )					
								FrameType = DATA_ACK_0_XON;
								PTP_Send_frame( NULL, FrameType, 0, ps );
						//	 	if( ps->reception_blocked == Q_ALMOST_BLOCKED )					FrameType = DATA_ACK_0_XOFF;
//tbd:							 	NL_unitdata_ind( ps, frame->Buffer, frame->Length, frame->Source ); // convey NPDU( or frame->Buffer)
#if 0 //
	             				MSTP_Flag.ReceivePacketPending = true;
								while(!MSTP_Flag.TransmitPacketPending) ;
#endif						
#if (DEBUG_UART1)
	sprintf(debug_str," send data0 \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
#if 0 //
								MSTP_Flag.TransmitPacketPending = false;
#endif
								PTP_Send_frame( &SendFrame[0], DATA_0, 0, ps );
								 
						}
					}
	        		else if( Ptp_para.FrameType == DATA_1 )
					{
						RemoveReceivedEntry( ps );
						if( ps->RxSequence_number == 0 )
						{
							/*Discard frame */
						//	if( ps->reception_blocked == Q_BLOCKED )
						//			FrameType = DATA_ACK_1_XOFF;
						//	else
									FrameType = DATA_ACK_1_XON;
							PTP_Send_frame( NULL, FrameType, 0, ps );
						}
						else
						{
						//	if( ps->reception_blocked == Q_BLOCKED )
						//	{
						//		RemoveReceivedEntry( ps );
						//		FrameType = DATA_NAK_1_XOFF;
						//	}
						//	else
						//	{  /*	New Data1	*/
								ps->RxSequence_number = 0;
						//		if( ps->reception_blocked == Q_NOT_BLOCKED )		
								FrameType = DATA_ACK_1_XON;
								PTP_Send_frame( NULL, FrameType, 0, ps );
						//		uart1_PutChar('7');
						//		if( ps->reception_blocked == Q_ALMOST_BLOCKED )			FrameType = DATA_ACK_1_XOFF;
//tbd:							  	NL_unitdata_ind( ps, frame->Buffer, frame->Length, frame->Source  );
#if 0 //
								MSTP_Flag.ReceivePacketPending = true;
								while(!MSTP_Flag.TransmitPacketPending) ;
#endif
#if (DEBUG_UART1)
	sprintf(debug_str," send data1 \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);  
#endif
#if 0 //
								MSTP_Flag.TransmitPacketPending = false;
#endif
								PTP_Send_frame( &SendFrame[0], DATA_1, 0, ps );
								
		
						 // }
						}
				  	}
					

				//	PTP_Send_frame( NULL, FrameType, 0, ps );
					ps->PTP_reception_state = REC_READY;
	        		break;
				case DATA_ACK:
					ps->PTP_reception_state = REC_READY;
					break;
				case DATA_NAK:
					/*	In this state the device has received a Data Nak frame for processing */
				  	ps->PTP_reception_state = REC_READY;
	        break;
			}
		}
		#endif
	}
	return 0;
}



/*
 * ----------------------------------------------------------------------------
 * Function Name: PTP_transmission_sm
 * Purpose: PTP transmission state machine
 * Params:   
 * Returns:
 * Note:  PTP transmission state machine have 5 state : TR_IDLE, TR_PENDING, TR_READY, TR_BLOCKED
 * ----------------------------------------------------------------------------
 */
 //void handle_PTP_rx_interrupt( STR_PTP *ps );
DWORD WINAPI    PTP_transmission_sm(LPVOID lpVoid)
//void PTP_transmission_sm(void )
{
	FRAME_ENTRY *frame;
	STR_PTP *ps;
    S16_T ResponseTimer;
#if 0 //
	portTickType xDelayPeriod = ( portTickType ) 50 / portTICK_RATE_MS;

ps->HeartbeatTimer = 0;
	#endif
	for( ; ; ) 
	{
		ps = &Ptp_para;
	Sleep(1);
	
	//	ps->transmission_blocked = 1;
#if 0
		vTaskDelay(xDelayPeriod);
#endif
	//	if( cQueueReceive( xPTPtran, &Ptp_para, ( portTickType ) 0 ) == pdPASS )
		//if( ps->rec_frame->HeaderCRC != 0)
		//if( cSemaphoreTake( xPTPtran, 1000 ) == pdTRUE )
		{
			switch( ps->PTP_transmission_state )
			{
				case TR_IDLE:
				
					/*	In this state the transmitter is waiting for the data link to be
					established between the local device and the peer device. The
					transmitter waits to be notified that a peer device is ready to
					communicate	*/
					if( ps->PTP_connection_state == CONNECTED )
					{	 
#if (DEBUG_UART1)
	sprintf(debug_str," TR_IDLE send heartbeat %u \r\n",(U16_T)ps->reception_blocked);
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
						if( ps->reception_blocked == Q_NOT_BLOCKED )
							PTP_Send_frame( NULL, HEARTBEAT_XON, 0, ps );
						else
							PTP_Send_frame( NULL, HEARTBEAT_XOFF, 0, ps );
						ps->TxSequence_number = 0;
						ps->PTP_transmission_state = TR_BLOCKED;
						break;
					}				
					break;
				case TR_BLOCKED:
				/*	In this state the peer device has indicated that it is not ready
				to receive data frames. The local device may have data ready to transmit.
				The local device periodically transmits a Heartbeat frame to maintain
				the data link, and waits for the peer device to become ready to receive
				data or for the termination of the data link.	*/
					/*	Send Request	*/
					/*
					if a DL-UNITDATA.request promitive is received


					
					*/
					/*	Peer Receiver Ready	*/
					if( !ps->transmission_blocked )
					{
						ps->PTP_transmission_state = TR_READY;
						break;
					}
					/* Disconnected */
					if( ps->PTP_connection_state == DISCONNECTED )
					{
						ps->PTP_transmission_state = TR_IDLE;
						break;
					}
					/* HeartbearTimer Expired */
					if( ps->HeartbeatTimer >= T_HEARTBEAT )
					{
						ps->HeartbeatTimer = 0;
						if( ps->reception_blocked == Q_BLOCKED )
							PTP_Send_frame( NULL, HEARTBEAT_XOFF, 0, ps );
						else
							PTP_Send_frame( NULL, HEARTBEAT_XON, 0, ps );
					   /* transmission state dont change */
						break;
					}
					break;
				case TR_READY:
				/*	The peer device has indicated its readiness to receive data frames
				but the local device has no data ready to transmit. The local device
				periodically transmits a Heartbeat frame to maintain the data link, and
				waits for a local request to transmit data or for the termination of
				the data link	*/

					/* Disconnected */
					if( ps->PTP_connection_state == DISCONNECTED )
					{
						ps->PTP_transmission_state = TR_IDLE;
						break;
					}
					/*	Send Request	*/
					/*
					if a DL-UNITDATA.request promitive is received


					
					*/

					/*	Transmit Message */
					if( !ps->transmission_blocked )
					{
						 if( ps->reception_blocked == Q_NOT_BLOCKED )
						 {	/*	if transimission queue is not empty and transmissionblocked is equal to false */
							frame = SendFrameAvailable( -1 );
							if( frame )
							{		  
#if (DEBUG_UART1)
	sprintf(debug_str," TR_READY Transmit Message \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif							
								ps->RetryCount = 0;
								ps->HeartbeatTimer = 0;
								ps->ack0received = 0;
								ps->ack1received = 0;
								ps->nack0received = 0;
								ps->nack1received = 0;
//									ps->sending_frame_now = 1;
								PTP_Send_frame(frame, 2, 0, ps );
								ps->PTP_transmission_state = TR_PENDING;
							 	ResponseTimer = T_RESPONSE;
								break;
							}
						 }
					}
					/* Remote Busy */
			        else 
			        { /* if transmissionBlocked is equal to Ture  */
						 ps->PTP_transmission_state = TR_BLOCKED;
						 break;
			        }
					/* 	Heart Beart Expired */
					if( ps->HeartbeatTimer >= T_HEARTBEAT )
					{
#if (DEBUG_UART1)
	sprintf(debug_str," TR_READY send Heart Beart \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif

						 ps->HeartbeatTimer = 0;
						 /* dont change transmission state */
						if( ps->reception_blocked == Q_BLOCKED )
							PTP_Send_frame( NULL, HEARTBEAT_XOFF, 0, ps );
						else
							PTP_Send_frame( NULL, HEARTBEAT_XON, 0, ps );
						break;
					}

//					if( ps->reception_blocked == Q_NOT_BLOCKED )
//						;//suspend( ps->base_task + PTP_TRANSMISSION ); TBD:
//					//	vTaskSuspend(xHandlePTPtransmission);	
//	        		else
//	          			;//msleep(1);	TBD:
//					//	DELAY_Us(6000);
					break;
	
				case TR_PENDING:
				/* in this state, the local devie has transmitted a data frame to the peer device
				and is waiting for an acknowlodement from the peer device */
					/*	Disconnected	*/
					if( ps->PTP_connection_state == DISCONNECTED )
					{
#if (DEBUG_UART1)
	sprintf(debug_str," TR_PENDING disconnected \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
						RemoveSentEntry( frame );
						ps->PTP_transmission_state = TR_IDLE;
						break;
					}
					/*	Send Request	*/
					/*
					if a DL-UNITDATA.request promitive is received


					
					*/

					/*	Receive Acknowledgement	*/
					if( ( ps->ack0received && ps->TxSequence_number == 0 ) ||
						( ps->ack1received && ps->TxSequence_number == 1 ) )
					{
#if (DEBUG_UART1)
	sprintf(debug_str," TR_PENDING Receive Acknowledgement \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
	
						RemoveSentEntry( frame );
	          			frame=NULL;
//						ps->sending_frame_now = 0;
						ps->TxSequence_number = 1 - ps->TxSequence_number;
						ps->ack0received = 0;
						ps->ack1received = 0;
						ps->PTP_transmission_state = TR_READY;
						break;
					}	
					if( !ResponseTimer )
	        		{ 
						if( ps->RetryCount < N_RETRIES )
						{
							/*	Retry	*/
							ps->RetryCount++;
							ps->PTP_transmission_state = TR_PENDING;
//							ps->sending_frame_now = 1;
							PTP_Send_frame( frame, 0, 0, ps );
							ResponseTimer = T_RESPONSE;
							break;
						}
						else
						{
							/*	Retries failed	*/
							RemoveSentEntry( frame );
	            			frame=NULL;
							ps->RetryCount = 0;
//							ps->sending_frame_now = 0;
							ps->ResponseTimer = 0;
							ps->PTP_transmission_state = TR_READY;
							break;
						}
					}
					break;
			}
		}
	}
	return 0;
}







U16_T CalcBlockDataCRC( U8_T *dat,U16_T length , U16_T crcValue )
{
	U16_T crcLow, crcHigh;
	S16_T i;
	for( i=0; i<length; i++ )
	{
		crcHigh = crcValue >> 8;
		crcLow = ( crcValue & 0x0ff ) ^ dat[i];
		crcValue = crcHigh ^ crc16_table[crcLow];
	}
	return crcValue;
}

//extern 
uint8_t  PDUBuffer_ptp[MAX_APDU];
DWORD WINAPI    PTP_Receive_task(LPVOID lpVoid)
{
#if 0
	portTickType xDelayPeriod  = ( portTickType ) 5 / portTICK_RATE_MS; // 1000
#endif
	uint16_t pdu_len = 0;  
	BACNET_ADDRESS far src; /* source address */


	//task_test.enable[11] = 1;
	for (;;)
    { 	
		//task_test.count[11]++; 
		Sleep(1);
		//if(Modbus.com_config[1] == PTP_RS232_GSM)
		{
#if 0
			vTaskDelay(5 / portTICK_RATE_MS);
#endif

#if 0
			if(Modbus.protocal == 0)
			{
				Modbus.protocal = BAC_PTP;
#endif
				//pdu_len = datalink_receive(&src, &PDUBuffer_ptp[0], sizeof(PDUBuffer_ptp), 0,Modbus.protocal);
				//pdu_len = datalink_receive(&src, &PDUBuffer_ptp[0], sizeof(PDUBuffer_ptp), 0);
				pdu_len = dl_ptp_receive(&src, &PDUBuffer_ptp[0], sizeof(PDUBuffer_ptp), 0);
			    { 
					
				    if(pdu_len > 0) 
					{
//#if (DEBUG_UART1)
//	sprintf(debug_str,"rx %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u\r\n",
//	(U16_T)PDUBuffer_ptp[0],(U16_T)PDUBuffer_ptp[1],PDUBuffer_ptp[2],PDUBuffer_ptp[3],PDUBuffer_ptp[4],
//	(U16_T)PDUBuffer_ptp[5],(U16_T)PDUBuffer_ptp[6],PDUBuffer_ptp[7],PDUBuffer_ptp[8],PDUBuffer_ptp[9],
//	(U16_T)PDUBuffer_ptp[10],(U16_T)PDUBuffer_ptp[11],PDUBuffer_ptp[12],PDUBuffer_ptp[13],PDUBuffer_ptp[14]
//	
//	
//	);
//	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
//#endif

			            npdu_handler(&src, &PDUBuffer_ptp[0], pdu_len);	
			        }
					else
					{
						Sleep(1);
					}
					
				}
#if 0
				Modbus.protocal = 0;
			}
#endif
		}
//		else
//		{
////			if(flag_logic_control != 0)	  // for cus
////				vTaskDelay(5 / portTICK_RATE_MS);
////			else
//				vTaskDelay(5000 / portTICK_RATE_MS);
//		}		  

	
		
    }		
	return 0;
}




/* Return the length of the packet */
uint16_t dl_ptp_receive(
    BACNET_ADDRESS * src,       /* source address */
    uint8_t * pdu,      /* PDU data */
    uint16_t max_pdu,   /* amount of space available in the PDU  */
    unsigned port)
{       /* milliseconds to wait for a packet */
    uint16_t pdu_len = 0;       /* return value */
    /* dummy - unused parameter */
//    timeout = timeout;
	pdu_len = 0;
    /* set the input buffer to the same data storage for zero copy */

	if (!Ptp_para.DataBuffer) {
        Ptp_para.DataBuffer = (char *)pdu;
  //      InputBufferSize = max_pdu;
   		}
    /* only do receive state machine while we don't have a frame */
	    if ((Ptp_para.received_valid_frame == false) &&
	        (Ptp_para.received_invalid_frame == false)) 
		{
	        for (;;) {
					
				PTP_Receive_Frame_FSM();

	            if (Ptp_para.received_valid_frame || Ptp_para.received_invalid_frame)
				{	
	                break;
				}
	            /* if we are not idle, then we are
	               receiving a frame or timing out */
	            if (Ptp_para.Rec_frame_state == RX_IDLE)
				{	
	                break;
				}
	        }
	    }

    /* only do master state machine while rx is idle */

#if 0
    /* if there is a packet that needs processed, do it now. */
    if (MSTP_Flag.ReceivePacketPending) {
        MSTP_Flag.ReceivePacketPending = false;


        	pdu_len = Ptp_para.Length;


        src->mac_len = 1;
        src->mac[0] = SourceAddress;


        /* data is already in the pdu pointer */
    }
#endif
    return pdu_len;
}




void PTP_Receive_Frame_FSM( void)
{
	STR_PTP *ps;
	ps = &Ptp_para;	

	ps->Rx_work_byte = 0;
	RS485_PTP_Check_UART_Data(ps);
	Sleep(1);
	//if (RS485_DataAvailable(&ps->Rx_work_byte,1)) 
	{	
		if( ps->Rx_work_byte != 0x11 && ps->Rx_work_byte != 0x13 )
	 	{	  
			if( ps->Rx_work_byte != 0x10 )  /* DLE received */
			{	
				ps->Rx_work_byte &= ps->PTP_DLE_mask;
				ps->PTP_DLE_mask = 0x0FF;
				if( ps->PTP_connection_state != DISCONNECTED )
				{	
					switch( ps->Rec_frame_state )
					{
						case RX_DATA:
						if( ps->SilenceTimer > T_FRAME_ABORT )
						{
#if (DEBUG_UART1)
	sprintf(debug_str,"RX_DATA crc SilenceTimer timeout \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
	 
							ps->received_invalid_frame = 1;
							ps->rec_frame_ready = 1;
							ps->reception_blocked = Q_BLOCKED;
							ps->receive_frame_free = 0;
							ps->Rec_frame_state = RX_IDLE;
							break;
						}
						ps->DataCRC = ( ps->DataCRC >> 8 ) ^
										crc16_table[( ps->DataCRC & 0x0ff ) ^ ps->Rx_work_byte ]; 
						
						if( ps->Index < ps->Length )
						{	 
							ps->DataBuffer[ps->Index] = ps->Rx_work_byte;
							ps->Index++;
							break;
						} 
						else if( ps->Index == ps->Length )
						{	 
							ps->Index++;
							break;
						}
						else
						{	
							if( ps->DataCRC != 0x0f0b8 )
							{	 
#if (DEBUG_UART1)
	sprintf(debug_str,"crc fail \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif
								ps->received_invalid_frame = 1;
							}
							else
							{
#if (DEBUG_UART1)
	sprintf(debug_str,"crc ok %u\r\n",(U16_T)ps->Length);
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif						
								

								ps->received_valid_frame = 1;
							}
						
							ps->reception_blocked = Q_BLOCKED;
							ps->receive_frame_free = 0;							
							ps->rec_frame_ready = 1; 
							ps->Rec_frame_state = RX_IDLE;
							break;
						}
						break;
					case RX_IDLE:
						
						if( ps->Rx_work_byte == 0x55 )
						{
							ps->Rec_frame_state = RX_PREAMBLE;
						}
						else
							ps->Rec_frame_state = RX_IDLE;
						break;
					case RX_PREAMBLE:
						if( ps->SilenceTimer > T_FRAME_ABORT )
						{
							ps->Rec_frame_state = RX_IDLE;
							break;
						}
						if( ps->Rx_work_byte == 0x55 )
						{  
							break;
						}
						if( ps->Rx_work_byte == 0x0ff )
						{	
							ps->HeaderCRC = 0x0ff;
							ps->Length = 0;

							ps->Index = 0;
							ps->PTP_DLE_mask = 0x0FF;
							ps->Rec_frame_state = RX_HEADER;
						}
						else
						{
							ps->Rec_frame_state = RX_IDLE;
						}
						break;
					case RX_HEADER:
						if( ps->SilenceTimer > T_FRAME_ABORT )
						{
							ps->Rec_frame_state = RX_IDLE;
							break;
						}
						ps->HeaderCRC = crc8_table[ ps->Rx_work_byte ^ ps->HeaderCRC ];
						switch( ps->Index )
						{
							case 0:	
								ps->FrameType = ps->Rx_work_byte;
								ps->Index++;
								break;
							case 1:	 
								ps->Length = ps->Rx_work_byte;
								ps->Length <<= 8;
								ps->Index++;
								break;
							case 2:	 
								ps->Length += ps->Rx_work_byte;
								ps->Index++;
								break;
							case 3:	 
								if( ps->HeaderCRC != 0x55 )
								{	
#if (DEBUG_UART1)
	sprintf(debug_str,"RX_HEADER crc fail \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif									
									ps->Rec_frame_state = RX_IDLE;
									break;
								}
								
								if( ps->Length < MAXFRAMEBUFFER )
								{	  

									if( ps->FrameType == HEARTBEAT_XOFF )
									{	
										ps->transmission_blocked = 1;
										ps->InactivityTimer = 0;
								//		tasks[ ps->base_task + PTP_TRANSMISSION ].status = READY;
										ps->Rec_frame_state = RX_IDLE;
										break;
									}
									if( ps->FrameType == HEARTBEAT_XON )
									{	
											ps->transmission_blocked = 0;
											ps->InactivityTimer = 0;
									//		tasks[ ps->base_task + PTP_TRANSMISSION ].status = READY;
											ps->Rec_frame_state = RX_IDLE;
											break;
									}
									if( ps->FrameType >= DATA_ACK_0_XOFF &&	ps->FrameType <= DATA_NAK_1_XON )
									{	 
										switch( ps->FrameType )
										{
										case DATA_ACK_0_XOFF:
											if( ps->TxSequence_number == 0 )
											{			/*	Ack0 XOFF	*/
												ps->ack0received = 1;
												ps->transmission_blocked = 1;
											}
											else
											{
												/*	Duplicate XOFF	*/
												ps->transmission_blocked = 1;
											}
											break;
										case DATA_ACK_1_XOFF:
											if( ps->TxSequence_number == 1 )
											{			/*	Ack1 XOFF	*/
												ps->ack1received = 1;
												ps->transmission_blocked = 1;
											}
											else
											{
												/*	Duplicate XOFF	*/
												ps->transmission_blocked = 1;
											}
											break;
										case DATA_ACK_0_XON:
											if( ps->TxSequence_number == 0 )
											{			/*	Ack0 XON	*/
												ps->ack0received = 1;
												ps->transmission_blocked = 0;
											}
											else
											{
												/*	Duplicate XON	*/
												ps->transmission_blocked = 0;
											}
											break;
										case DATA_ACK_1_XON:
											if( ps->TxSequence_number == 1 )
											{			/*	Ack1 XON	*/
												ps->ack1received = 1;
												ps->transmission_blocked = 0;
											}
											else
											{
												/*	Duplicate XON	*/
												ps->transmission_blocked = 0;
											}
											break;
										case DATA_NAK_0_XOFF:
											if( ps->TxSequence_number == 0 )
											{    	/*	Nack0 XOFF	*/
												ps->nack0received = 1;
												ps->transmission_blocked = 1;
											}
											else
											{
												/*	Duplicate XOFF	*/
												ps->transmission_blocked = 1;
											}
											break;
										case DATA_NAK_1_XOFF:
											if( ps->TxSequence_number == 1 )
											{    	/*	Nack0 XOFF	*/
												ps->nack1received = 1;
												ps->transmission_blocked = 1;
											}
											else
											{
												/*	Duplicate XOFF	*/
												ps->transmission_blocked = 1;
											}
											break;
										case DATA_NAK_0_XON:
											if( ps->TxSequence_number == 0 )
											{      /*	Nack0 XON	*/
												ps->nack0received = 1;
												ps->transmission_blocked = 0;
											}
											else
											{
												/*	Duplicate XON	*/
												ps->transmission_blocked = 0;
											}
											break;
										case DATA_NAK_1_XON:
											if( ps->TxSequence_number == 1 )
											{			/*	Nack1 XON	*/
												ps->nack1received = 1;
												ps->transmission_blocked = 0;
											}
											else
											{
												/*	Duplicate XON	*/
												ps->transmission_blocked = 0;
											}
											break;
										}
										ps->InactivityTimer = 0;
										ps->Rec_frame_state = RX_IDLE;
										break;
									}
									else
									{	
										if( ps->receive_frame_free )
										{	
//											ps->rec_frame->FrameType = ps->FrameType;
//											ps->rec_frame->Length = ps->Length;
											if( ps->Length  && (ps->Length <= sizeof(PDUBuffer_ptp)))
											{
#if (DEBUG_UART1)
	sprintf(debug_str,"crc initial \r\n");
	uart_send_string(debug_str,strlen(debug_str),UART_SUB1);
#endif									
												ps->DataCRC = 0x0ffff;
												ps->Index = 0;
//												ps->DataBuffer = ps->rec_frame->Buffer;
												ps->Rec_frame_state = RX_DATA;
												break;
											}
											else
											{
												ps->received_valid_frame = 1;
		  										ps->rec_frame_ready = 1;
												ps->reception_blocked = Q_BLOCKED;
												ps->receive_frame_free = 0;
												ps->Rec_frame_state = RX_IDLE;
												break;
											}
										}

									}
									ps->Rec_frame_state = RX_IDLE;
									break;
								}
								else
								{	
									if( ps->receive_frame_free )
									{	
										ps->received_invalid_frame = 1;
										ps->rec_frame_ready = 1;
										ps->reception_blocked = Q_BLOCKED;
										ps->receive_frame_free = 0;
									//	tasks[ ps->base_task + PTP_RECEPTION ].status = READY;
		              				}
									ps->Rec_frame_state = RX_IDLE;
								}
								break;
							}
							break;
						}
					}
	
				else
				{
					if( ps->Rx_work_byte == trigger_sequence[ps->rec_trigger_sequence] )
						ps->rec_trigger_sequence++;
					else
					{
						ps->rec_trigger_sequence = 0;
					}
				}
			}
			else
				ps->PTP_DLE_mask = 0x7F;
		}
	
	ps->SilenceTimer = 0;
	
	}

    return;
}

DWORD WINAPI    PTP_timer(LPVOID lpVoid)
{
	while(1)
	{
		Sleep(1);
		Ptp_para.SilenceTimer ++; 
		Ptp_para.HeartbeatTimer++; 
		Ptp_para.InactivityTimer ++;  
		Ptp_para.ResponseTimer++;
	}
	return 0;
}

void vstartptptask()
{
	hThread_Receive =CreateThread(NULL,NULL,PTP_Receive_task,NULL,NULL, &nThreadID_Receive);
	hThread_connection =CreateThread(NULL,NULL,PTP_connection_sm,NULL,NULL, &nThreadID_connection);
	hThread_reception =CreateThread(NULL,NULL,PTP_reception_sm,NULL,NULL, &nThreadID_reception);
	hThread_transmission =CreateThread(NULL,NULL,PTP_transmission_sm,NULL,NULL, &nThreadID_transmission);
	hThread_timer =CreateThread(NULL,NULL,PTP_timer,NULL,NULL, &nThreadID_timer);
}




bool dl_ptp_init(
    char *ifname)
{
  //  unsigned long hThread = 0;
    uint32_t arg_value = 0;
	Sleep(1);
    /* initialize packet queue */
#if 0
    Receive_Packet.ready = false;
    Receive_Packet.pdu_len = 0;
    Receive_Packet_Flag = CreateSemaphore(NULL, 0, 1, "dlmstpReceivePacket");
   // if (Receive_Packet_Flag == NULL)
   //     exit(1);
    Received_Frame_Flag = CreateSemaphore(NULL, 0, 1, "dlsmtpReceiveFrame");
    if (Received_Frame_Flag == NULL) {
        CloseHandle(Receive_Packet_Flag);
   //     exit(1);
    }
#endif
    /* initialize hardware */
  //  timer_init();
    if (ifname) {
        RS485_Set_Interface(ifname);
    }
    RS485_Initialize();
#if 0
    MSTP_Port.InputBuffer = &RxBuffer[0];
    MSTP_Port.InputBufferSize = sizeof(RxBuffer);
    MSTP_Port.OutputBuffer = &TxBuffer[0];
    MSTP_Port.OutputBufferSize = sizeof(TxBuffer);
    MSTP_Port.SilenceTimer = Timer_Silence;
    MSTP_Port.SilenceTimerReset = Timer_Silence_Reset;
    MSTP_Init(&MSTP_Port);
#endif
	init_PTP_structures();
#if 0
    uint8_t data;

    /* FIXME: implement your data storage */
    data = 64;  /* I2C_Read_Byte(
                   EEPROM_DEVICE_ADDRESS,
                   EEPROM_MSTP_MAC_ADDR); */
    if (data <= 127)
        MSTP_Port.This_Station = data;
    else
        dlmstp_set_my_address(DEFAULT_MAC_ADDRESS);
    /* FIXME: implement your data storage */
    data = 127; /* I2C_Read_Byte(
                   EEPROM_DEVICE_ADDRESS,
                   EEPROM_MSTP_MAX_MASTER_ADDR); */
    if ((data <= 127) && (data >= MSTP_Port.This_Station))
        MSTP_Port.Nmax_master = data;
    else
        dlmstp_set_max_master(DEFAULT_MAX_MASTER);
    /* FIXME: implement your data storage */
    data = 1;
    /* I2C_Read_Byte(
       EEPROM_DEVICE_ADDRESS,
       EEPROM_MSTP_MAX_INFO_FRAMES_ADDR); */
    if (data >= 1)
        MSTP_Port.Nmax_info_frames = data;
    else
        dlmstp_set_max_info_frames(DEFAULT_MAX_INFO_FRAMES);
#endif
#if PRINT_ENABLED
    fprintf(stderr, "MS/TP MAC: %02X\n", MSTP_Port.This_Station);
    fprintf(stderr, "MS/TP Max_Master: %02X\n", MSTP_Port.Nmax_master);
    fprintf(stderr, "MS/TP Max_Info_Frames: %u\n",
        (unsigned) MSTP_Port.Nmax_info_frames);
#endif


#if 0 //Fance
    hThread1 = _beginthread(dlmstp_receive_fsm_task, 4096, &arg_value);
    if (hThread1 == 0) {
        fprintf(stderr, "Failed to start recive FSM task\n");
    }
    hThread2 = _beginthread(dlmstp_master_fsm_task, 4096, &arg_value);
    if (hThread2 == 0) {
        fprintf(stderr, "Failed to start Master Node FSM task\n");
    }
#endif

	vstartptptask();
    return true;
}




//#endif
