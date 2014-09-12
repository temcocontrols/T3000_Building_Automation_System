
#ifndef PTP_H
#define PTP_H

#include "ud_str.h"
//#include "types.h"
#define S16_T  signed short  
#define U16_T unsigned short
#define U8_T  unsigned char  
#define U32_T unsigned int
#define S32_T int;
//#define TIMERESOLUTION_PTP            50
#define TIMERESOLUTION_PTP            1

//#define	T_HEARTBEAT_SEC		15
//#define	T_INACTIVITY_SEC  	60

#define	N_RETRIES		      1
#define	T_CONN_REQ		15000 / TIMERESOLUTION_PTP
#define	T_CONN_RSP		15000 / TIMERESOLUTION_PTP
#define	T_FRAME_ABORT	 2000 / TIMERESOLUTION_PTP
#define	T_HEARTBEAT		15000 / TIMERESOLUTION_PTP
#define	T_INACTIVITY    60000 / TIMERESOLUTION_PTP
#define	T_RESPONSE		 5000 / TIMERESOLUTION_PTP




#pragma region NewAdd
#define MAXFRAMEBUFFER			      490 

#define MAX_SEND_FRAMES             5


#pragma pack(push) //±£´æ¶ÔÆë×´Ì¬ 
#pragma pack(1)

typedef struct {
	uint8_t FrameType;
	uint8_t Destination;
	uint8_t Source;
	unsigned short Length;
	uint8_t HeaderCRC;
	char Buffer[MAXFRAMEBUFFER+2+1];
	unsigned short DataCRC;
	uint8_t being_used ;
	uint8_t locked ;
	uint8_t port ;
	uint8_t reply_flag ;
	uint8_t number ;
	uint8_t delay ;

} FRAME_ENTRY;

struct STR_PTP
{

	/* Timers for both protocols */
	unsigned short HeartbeatTimer;
	unsigned short InactivityTimer;
	unsigned short SilenceTimer;
	unsigned short ResponseTimer;     /* only for PTP */
	unsigned short SendFrameTimer;

	/* variables for both protocols */
	uint8_t base_task;
	uint8_t tx_wake_up;
	/*	uint8_t Using_send_frame;*/
	uint8_t Using_send_buffer;

	/* States for PTP */
	uint8_t PTP_reception_state;       /* OS - original station for RS485 */
	uint8_t PTP_transmission_state;    /* NS - next station for RS485     */
	uint8_t PTP_connection_state;      /* sendinfo_flag for RS485         */

	uint8_t Rec_frame_state;

	/* offset 20 */
	uint8_t FrameType;
	uint8_t Destination;
	uint8_t Source;
	unsigned short Length;
	uint8_t HeaderCRC;
	uint8_t PTP_DLE_mask;
	unsigned short DataCRC;
	unsigned short Index;
	uint8_t Rx_work_byte;
	char *DataBuffer;
	//	FRAME_ENTRY *rec_frame;

	/* offset 38 */
	/* PTP related flags */
	uint8_t ack0received;//				    : 1; /* receivedpollformaster for RS485 */
	uint8_t ack1received;//				    : 1; /* newpanelbehind for RS485 */
	uint8_t nack0received;//				  : 1; /* panelOff for RS485 */
	uint8_t nack1received;//				  : 1; /* receivedtoken for RS485 */
	uint8_t transmission_blocked;//		: 1;
	uint8_t TxSequence_number;//			: 1;
	uint8_t RxSequence_number;//			: 1;
	uint8_t reception_blocked;//			: 2; /* wantpointsentry for RS485 */
	/* flags related to both protocols */
	uint8_t rec_frame_ready;//			  : 1;
	uint8_t received_invalid_frame;//	: 1;
	uint8_t received_valid_frame;//		: 1;
	uint8_t receive_frame_free;//		  : 1;
	//	uint8_t send_frame_free;//        : 1;
	//	uint8_t send_frame_locked;//			: 1;
	//	uint8_t sending_frame_now;//			: 1;
	/* MSTP related flags */
	unsigned short     EventCount;

	/* offset 42 */
	uint8_t RetryCount;//             : 3;
	uint8_t FrameCount ;//            : 3;
	uint8_t FirstToken ;//            : 3;
	uint8_t replyflag ;//             : 2;
	uint8_t validint  ;//             : 1;
	uint8_t SoleMaster;//             : 1;
	uint8_t UsedToken ;//             : 1;
	uint8_t action ;//                : 1;
	uint8_t netpointsflag ;//         : 1;

	//	uint8_t ready_for_descriptors;//  : 4;
	uint8_t rec_trigger_sequence ;//  : 4; /* only for PTP */
	/* laststation_connected for RS485 */
	//	uint8_t timeonnet;//              : 3;
	//	uint8_t link_type;//              : 2;
	//	uint8_t port   ;//                : 1;
	//	uint8_t tx_end  ;//               : 1;
	//	uint8_t first_send ;//            : 1;

	//	char     nextpanelisoff;
	//	uint32_t need_info;

};  


typedef struct
{
	int   network;
	unsigned char  router_address;
	unsigned char  half_router_address;
	unsigned char  status;
}  Reachable_Networks;

typedef struct
{
	unsigned char address;
	int  network;
	Reachable_Networks networks_list;
} Port_Def;

#define   PORT_ACTIVE      0x03
#define   PTP_INSTALLED    0x05
#define   PTP_ACTIVE       0x07


#define MAX_reachable_networks 2
#define MAX_Routing_table      2

/*
// status :   0  - free
//            0x01 Port_installed  ( 0000 0001 )
//            0x03 Port active     ( 0000 0011 )
//            0x05 PTP installed   ( 0000 0101 )
//            0x07 PTP active      ( 0000 0111 )
//            0x11 RS485 installed ( 0001 0001 )
//            0x13 RS485 active    ( 0001 0011 )
//            0x23 ETHERNET        ( 0010 0011 )
*/


typedef enum
{
	DL_INVALID = -1, DL_UNITDATA_REQUEST, DL_UNITDATA_INDICATION,
	DL_UNITDATA_RESPONSE, DL_CONNECT_REQUEST, DL_CONNECT_INDICATION,
	DL_CONNECT_RESPONSE, DL_DISCONNECT_REQUEST, DL_TEST_REQUEST,
	DL_TEST_RESPONSE, DL_CONNECT_ATTEMPT_FAILED
}
DL_Primitives;
typedef enum { N_UNITDATArequest, N_UNITDATAindication } NETWORKPrimitive;


typedef struct {
	unsigned ack0received				:1;
	unsigned ack1received				:1;
	unsigned nak0received				:1;
	unsigned nak1received				:1;
	unsigned reception_blocked			:2;
	unsigned transmission_blocked		:1;
	unsigned sending_frame_now			:1;
	unsigned send_done					:1;
	unsigned send_ok						:1;
	unsigned TxSequence_number			:1;
	unsigned RxSequence_number			:1;
	unsigned receive_error				:1;
}	PTP_COMMUNICATION_flags;

#ifndef byte
#define byte  unsigned char
#endif

typedef struct {
	unsigned 					  	SilenceTimer;
	unsigned 					  	InactivityTimer;   //Timebetweentoken
	unsigned 					  	HeartbeatTimer;    //timerunMSTP
	int      					  	EventCount;
	byte     					  	ReceiveError;
	PTP_COMMUNICATION_flags 	PTP_comm_status;
	char                     MSTP_MASTERState;
	byte     						Preamble1;
	byte     						Preamble2;
	byte 	  						HeaderCRC;
	unsigned 						DataCRC;
	int      						Length;
	byte 	  						Destination;
	byte 	  						Source;
	byte     						FrameType;
	byte         	         HEADERState;         //HEADERStateEnum
	byte  	                  ReceiveFrameStatus;  //ReceiveFrameStatusEnum
	int      						task;
	byte                     ring;
	byte     						connection;
	byte    			         validint;
	byte                     mode;
	byte                     physical_connection_state;
	byte                     rings_number;         // answer after number_rings
	long                     time_modem;
} PORT_STATUS_variables;


//typedef struct {
//	char 		status;
//	struct 	Port Port;
//	void 		*ptr;
//	int      task;
//	PORT_STATUS_variables port_status_vars;
//} ROUTING_TABLE;

typedef struct {
	char 		  status;
	Port_Def  Port;
} Routing_Table;


typedef enum
{
	Who_Is_Router_To_Network, I_Am_Router_To_Network,
	I_Could_Be_Router_To_Network,	Reject_Message_To_Network,
	Router_Busy_To_Network, Router_Available_To_Network,
	Initialize_Routing_Table, Initialize_Routing_Table_Ack,
	Establish_Connection_To_Network, Disconnect_Connection_To_Network,
	I_Am_Router_To_Network_Prop=0x80

} Network_Layer_Message_Type;
typedef enum { Q_BLOCKED, Q_ALMOST_BLOCKED, Q_NOT_BLOCKED } Blocked_States;
typedef enum
{
	HEARTBEAT_XOFF = 0, HEARTBEAT_XON, DATA_0, DATA_1, DATA_ACK_0_XOFF,
	DATA_ACK_1_XOFF, DATA_ACK_0_XON, DATA_ACK_1_XON, DATA_NAK_0_XOFF,
	DATA_NAK_1_XOFF, DATA_NAK_0_XON, DATA_NAK_1_XON, CONNECT_REQUEST,
	CONNECT_RESPONSE, DISCONNECT_REQUEST, DISCONNECT_RESPONSE,
	TEST_REQUEST, TEST_RESPONSE
}	Standard_Frame_Types;
typedef enum
{
	RX_IDLE, RX_PREAMBLE, RX_HEADER, RX_DATA, RX_DATA_CRC
} ReceiveFrameStates;

typedef enum
{
	DISCONNECTED, INBOUND, OUTBOUND, CONNECTED, DISCONNECTING
}
PTP_CON_States;

typedef enum { REC_IDLE, REC_READY, DATA, DATA_ACK, DATA_NAK } PTP_REC_States;

typedef enum { TR_IDLE, TR_PENDING, TR_READY, TR_BLOCKED } PTP_TR_States;

#pragma endregion NewAdd
#pragma pack(pop)



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

void vstartptptask();
bool dl_ptp_init(
	char *ifname);
uint16_t dl_ptp_receive(
	BACNET_ADDRESS * src,       /* source address */
	uint8_t * pdu,      /* PDU data */
	uint16_t max_pdu,   /* amount of space available in the PDU  */
	unsigned port);
#endif


