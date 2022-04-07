#ifndef _ROUTER_H
#define _ROUTER_H

#define ESTABLISH_CONNECTION_NET  90
#define ESTABLISH_CONNECTION_PORT 91

#define   ASYNCRON_INSTALLED  0x80
#define   PORT_ACTIVE      0x03
#define   PTP_INSTALLED    0x05
#define   PTP_ACTIVE       0x07
#define   RS485_INSTALLED  0x11
#define   RS485_ACTIVE     0x13
#define   IPX_INSTALLED  	 0x21
#define   IPX_ACTIVE     	 0x23
#define   TCPIP_INSTALLED  0x41
#define   TCPIP_ACTIVE     0x43

//#define   NETBIOS_INSTALLED  	0x21
//#define   NETBIOS_ACTIVE     	0x23


#define UNREACHABLE            1     // 0x00000001
#define TEMPORARY_UNREACHABLE  2     // 0x00000010
#define USER_UNREACHABLE       7     // 0x00000111
#define REACHABLE              3     // 0x00000011

#define MAX_reachable_networks 5
#define MAX_Routing_table      9
#define MAX_NetSession         5

typedef enum {
	  Who_Is_Router_To_Network, I_Am_Router_To_Network, I_Could_Be_Router_To_Network,
	  Reject_Message_To_Network, Router_Busy_To_Network, Router_Available_To_Network,
	  Initialize_Routing_Table, Initialize_Routing_Table_Ack,
	  Establish_Connection_To_Network, Disconnect_Connection_To_Network,
	  I_Am_Router_To_Network_Prop=0x80
	 } Network_Layer_Message_Type;


typedef struct {
		char 		status;
		char     service;
		char     source;
		char     dest;
		int      network;
		signed char *semaphore;
		char        task;
		char        *data;
		int         id;
		int         timeout;
	} NetworkSession ;

struct reachable_networks {
		 int   network;
		 byte  router_address;
		 byte  half_router_address;
		 byte  status;
	  };

struct Port {
	  byte address;
	  int  network;
	  struct reachable_networks networks_list[MAX_reachable_networks];
	};

// status :   0  - free
//            0x01 Port_installed  ( 0000 0001 )
//            0x03 Port active     ( 0000 0011 )
//            0x05 PTP installed   ( 0000 0101 )
//            0x07 PTP active      ( 0000 0111 )
//            0x11 RS485 installed { 0001 0001 )
//            0x13 RS485 active    { 0001 0011 )
//            ETHERNET             { 0010 0011 )



typedef struct {
		char 		status;
		struct 	Port Port;
		void 		*ptr;
		int      task;
		PORT_STATUS_variables port_status_vars;
	} ROUTING_TABLE;





typedef struct
{
	int 		SNET;
	char		SLEN;
	char     SADR[6];
	int 		DNET;
	char		DLEN;
	char     DADR[6];
	char     termination_time_value;
	int      len;
	char     *data;
} ROUTER_PARAMETERS;

#ifndef ROUTER
//Fance//extern ROUTING_TABLE Routing_table[MAX_Routing_table];
ROUTING_TABLE Routing_table[MAX_Routing_table];
extern char trigger_sequence[];
extern int networkaddress;
extern int NetworkAddress;

extern int router(int service, int command, ROUTER_PARAMETERS *r=NULL, int	port_number=-1, int send=0);
#endif

#endif
