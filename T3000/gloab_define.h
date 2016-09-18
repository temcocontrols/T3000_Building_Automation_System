#pragma once
#include "RelayLabel.h"
const int SORT_UNKNOW = 0;
const int SORT_BY_CONNECTION = 1;
const int SORT_BY_BUILDING_FLOOR = 2;

const int LIST_UP = 1;
const int LIST_DOWN = 2;

const int NO_COMMAND = 0;
const int START_AUTO_FLASH_COMMAND = 1;
const int FLASH_SUCCESS = 2;
const int FAILED_NORESPONSED = 3;
const int FAILED_UNKNOW_ERROR = 4;


const int SEND_DOWNLOAD_COMMAND = 1;
const int START_SEND_WANT_PACKAGE = 2;
const int RECEIVE_WANTED_PACKAGE = 3;
const int RECEIVE_COMPLET = 4;
const int SEND_GET_MD5_VALUE = 5;
const int CHECK_MD5_VALUE = 6;
const int SHOW_FTP_PATH = 7;
const int THREAD_IDLE = 255;

const int TFTP_SEND_LENGTH = 3 * 1024 + 512;
//const int TFTP_SEND_LENGTH = 512;
//const int TFTP_SEND_LENGTH = 1024;
#pragma pack(push) //保存对齐状态 
#pragma pack(1)
typedef struct
{
	char HEAD_1;
	char HEAD_2;
	unsigned short length;
	unsigned char commad;
}Black_head_struct;
struct Batch_SN{
    int ModbusID;
    int SN;
    BOOL Result;
};
typedef struct
{
	char HEAD_1;
	char HEAD_2;
	unsigned short length;
	unsigned char commad;
	unsigned char product_id;
	unsigned char get_newest;
	unsigned char file_type;
	unsigned short softversion;
	unsigned char CRC;
}Download_Info;


typedef struct
{
	char HEAD_1;
	char HEAD_2;
	unsigned short length;
	unsigned char commad;
	unsigned short send_package;
	char download_filename[40];
	unsigned short fw_version;
	unsigned char CRC;
}Download_File_Return;

const int KEY_INPUT = 1;
const int KEY_OUTPUT = 2;
const int KEY_VARIABLE = 3;
const int KEY_PROGRAM = 4;
const int KEY_PID = 5;
const int KEY_SCH = 6;
const int KEY_HOL = 7;
const int KEY_SCREEN = 8;
const int KEY_MONITOR = 9;
const int KEY_FUNCTION = 10;
const int KEY_COMMAND = 11;

const int KEY_ERROR_LABEL = 255;

typedef struct  
{
	long startpos;
	long endpos;
	DWORD ncolor;
	unsigned int key_type;
}Str_char_pos_color;

typedef struct 
{
	char HEAD_1;
	char HEAD_2;
	unsigned short length;
	unsigned char commad;
	unsigned short package_index;
	unsigned short total_package;
	unsigned char CRC;
}File_Package_Struct;



#pragma pack(pop)//恢复对齐状态 


typedef enum
{
	GET_SERIAL_NUMBER = 1,
	DOWNLOAD_FILE	= 2,
	UPLOAD_FILE = 3,
	DOWNLOAD_NEW_FILE = 4,	//用于区别  DOWNLOAD_FILE ， 代表新的下载利用3K下载;
	GET_MD5_VALUE = 99,

	RETURN_SERIAL_NUMBER = 101,
	RETURN_FILE_SIZE = 102,
	ACK_GET_UPLOAD_FILE_PAGE = 103,

	ACK_MD5_VALUE = 199,
	ACK_SERIAL_NUMBER = 201,
	ACK_GET_FILE_PAGE = 202,
	TRANSFER_UPLOAD_FILE_PAGE = 203,
	TRANSFER_FILE_PAGE = 212,
	UPLOAD_COMPLET = 213,
	DOWNLOAD_COMPLETE = 222,
	NO_MD5_FILE_EXSIT = 252,
	UPLOAD_FILE_ERROR = 253,
	DOWNLOAD_FILE_ERROR = 254,
	HOST_BUSY = 255
};


const int DOWNLOAD_CONNECT_SUCCESS = 1;
const int DOWNLOAD_DISCONNEC = 2;
const int DOWNLOAD_CONNECT_FAILED = 3;
const int RETURN_ERROR = 4;
const int DOWNLOAD_CLOSE_SOCKET = 5;
const int DOWNLOAD_PERSENT = 6;
const int DOWNLOAD_FINISHED = 7;
const int DOWNLOAD_FILE_INFO = 8;
const int DOWNLOAD_NOT_FIND_LOCAL = 9;
const int DOWNLOAD_LOCAL_EXSIT = 10;
const int DOWNLOAD_RESULTS = 11;
const int DOWNLOAD_T3000_NO_UPDATE = 12;
const int DOWNLOAD_MD5_FAILED = 20;
const int DOWNLOAD_MD5_CHECK_PASS = 21;




//#define SHOW_MESSAGEBOX
#define	 DELETE_LABEL_MESSAGE	WM_USER + 1228
#define  EDIT_LABEL_MESSAGE		WM_USER + 1229
#define  ADD_LABEL_MESSAGE		WM_USER + 1230
#define	 MY_REDRAW_WINDOW		WM_USER + 1231

#define WM_CHANGE_NEXT_PANEL_MESSAGE WM_USER + 972
#define WM_SHOW_STATUS_TEXT		 WM_USER + 973
#define WM_SHOW_STATUS_PROGRESS WM_USER + 974
#define WM_FRESH_CM_LIST		WM_USER + 975
#define WM_FRESH_SETTING_UI		WM_USER + 976
#define  WM_SHOW_PROGRESS		WM_USER + 977
#define  WM_SHOW_PANELSTRING		WM_USER + 978
#define  WM_RS485_MESSAGE       WM_USER + 979
#define WM_DELETE_NEW_MESSAGE_DLG WM_USER + 2001
#define MY_BAC_CONFIG_READ_RESULTS  WM_USER + 2002
#define SHOW_ERROR_MESSAGE

#define  MY_FRESH_DRAW_GRAPHIC      WM_USER + 2003
#define  MY_FRESH_TESTO_GRAPHIC      WM_USER + 2004
#define  WM_LIST_MONITOR_CHANGED WM_USER+ 976
#define  WM_LIST_MONITOR_INPUT_CHANGED WM_USER+ 977
#define WM_SCREENEDIT_CLOSE WM_USER + 1232

//const int BACNETIP_PORT =  47808;
const int BACNETIP_PORT =  47808;
const int TEMCO_SERVER_PORT = 31234;
const int UDP_BROADCAST_PORT =1234;
const int RECV_RESPONSE_PORT = 4321;
#define UPD_BROADCAST_QRY_MSG 100
#define RESPONSE_MSG          UPD_BROADCAST_QRY_MSG+1
#define RESPONSE_LABEL		  0x6A

const int MODBUS_RS485 = 0;
const int MODBUS_TCPIP = 1;
const int MODBUS_BACNET_MSTP = 2;
const int PROTOCOL_BACNET_IP   = 3;
const int PROTOCOL_GSM = 4;
const int PROTOCOL_REMOTE_IP = 6;
const int PROTOCOL_BIP_TO_MSTP = 10;
const int PROTOCOL_UNKNOW = 255;

const int BAC_WAIT_NORMAL_READ = 0;
const int BAC_WAIT_READ_CONFIG_WRITE_DEVICE = 1;
const int BAC_WAIT_READ_DATA_WRITE_CONFIG = 2;
const int BAC_WAIT_READ_MONITOR_DATA = 3;

const int READ_ANALOG = 1;
const int READ_DIGITAL = 2;

#define WM_COMMAND_WHO_IS  1
#define MENU_CLICK			2

const COLORREF LIST_ITEM_CHANGED_BKCOLOR = RGB(255,0,0);
const COLORREF LIST_ITEM_DEFAULT_BKCOLOR = GetSysColor(COLOR_WINDOW);
const COLORREF LIST_ITEM_DEFAULT_BKCOLOR_GRAY = RGB(235,235,235);
const COLORREF LIST_ITEM_SELECTED = RGB(150,150,200);

const bool REFRESH_ON_ITEM = TRUE;

//const int SEND_COMMAND_DELAY_TIME = 100;

const int RESEND_COUNT = 10;
const int FAIL_RESEND_COUNT = 4;

const int TYPE_SVAE_CONFIG = 254;
const int TYPE_ALL = 255;
const int TYPE_CONNECT_WITH_DEVICE = 253;
const int TYPE_IN_OUT_VAR = 252;
const int TYPE_MAIN = 0;
const int TYPE_INPUT = 1;
const int TYPE_OUTPUT = 2;
const int TYPE_PROGRAM = 3;
const int TYPE_VARIABLE = 4;
const int TYPE_WEEKLY =5;
const int TYPE_ANNUAL = 6;
const int TYPE_CONTROLLER = 8;
const int TYPE_SCREENS = 9;
const int TYPE_MONITOR = 10;
const int TYPE_PROGRAMCODE = 11;
const int TYPE_WEEKLYCODE = 12;
const int TYPE_ANNUALCODE = 13;
const int TYPE_ALARMLOG = 14;
const int TYPE_TSTAT = 15;
const int TYPE_SETTING = 16;
const int TYPE_READ_MONITOR_DATA = 17;
const int TYPE_READ_CUSTOMER_UNIT = 18;
const int TYPE_READ_USER_LOGIN_INFO = 19;
const int TYPE_READ_GRAPHIC_LABEL_INFO = 20;
const int TYPE_READ_REMOTE_POINT_INFO = 21;

const int TYPE_TSTAT_MAIN_INFOR = 22;

const int DELETE_WINDOW_MSG = 200;
const int START_BACNET_TIMER = 201;
const int CONNECT_TO_MODBUS_FAILED = 202;
const int PASSWORD_OK_INITIAL_UI = 203;
const int SHOW_PROGRAM_IDE = 204;
const int SHOW_DEVICE_STATUS = 205;

const int BAC_READ_ALL_LIST = 255;
const int BAC_READ_SVAE_CONFIG = 254;
const int BAC_READ_CONNECT_WITH_DEVICE_LIST = 253;
const int BAC_READ_IN_OUT_VAR_LIST = 252;
const int BAC_READ_INPUT_LIST = 1;
const int BAC_READ_OUTPUT_LIST = 2;
const int BAC_READ_PROGRAM_LIST = 3;
const int BAC_READ_VARIABLE_LIST = 4;
const int BAC_READ_WEEKLY_LIST = 5;
const int BAC_READ_ANNUAL_LIST = 6;
const int BAC_READ_TIME_COMMAND = 7;
const int BAC_READ_CONTROLLER_LIST = 8;
const int BAC_READ_SCREEN_LIST = 9;
const int BAC_READ_MONITOR_LIST = 10;
const int BAC_READ_PROGRAMCODE_LIST = 11;
const int BAC_READ_WEEKLTCODE_LIST = 12;
const int BAC_READ_ANNUALCODE_LIST = 13;
const int BAC_READ_ALARMLOG_LIST = 14;
const int BAC_READ_TSTAT_LIST = 15;
const int BAC_READ_BASIC_SETTING_COMMAND = 16;
const int BAC_READ_MONITOR_DATA = 17;
const int BAC_READ_CUSTOMER_UNITS = 18;
const int BAC_READ_USER_LOGIN_INFO = 19;
const int BAC_READ_GRAPHIC_LABEL_INFO = 20;
const int BAC_READ_REMOTE_POINT_INFO = 21;




const int VARIABLE_RANGE_ANALOG_TYPE = 1;
const int VARIABLE_RANGE_DIGITAL_TYPE = 2;
const int VARIABLE_RANGE_CUSTOM_DIG_TYPE = 3;

const int INPUT_RANGE_ANALOG_TYPE = 4;
const int INPUT_RANGE_DIGITAL_TYPE = 5;
const int INPUT_RANGE_CUSTOM_DIG_TYPE = 6;

const int OUTPUT_RANGE_ANALOG_TYPE = 7;
const int OUTPUT_RANGE_DIGITAL_TYPE = 8;
const int OUTPUT_RANGE_CUSTOM_DIG_TYPE = 9;

const int BAC_RESULTS_UNKONW = 2;
const int BAC_RESULTS_OK = 1;
const int BAC_RESULTS_FAIL = 0;

const int BAC_UNITS_DIGITAL = 0;
const int BAC_UNITS_ANALOG  = 1;

const int BAC_AUTO = 0;
const int BAC_MANUAL = 1;

const int BAC_DECOM_YES = 0;
const int BAC_DECOM_NO  = 1;

const int BAC_READ_GROUP_NUMBER = 4;

const int BAC_READ_INPUT_GROUP_NUMBER = 10;
const int BAC_READ_OUTPUT_GROUP_NUMBER = 10;
const int BAC_READ_VARIABLE_GROUP_NUMBER = 10;
const int BAC_READ_PROGRAM_GROUP_NUMBER = 10;
const int BAC_READ_PID_GROUP_NUMBER = 10;
const int BAC_READ_SCHEDULE_GROUP_NUMBER = 10;
const int BAC_READ_HOLIDAY_GROUP_NUMBER = 8;

const int BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER = 8;

const int BAC_READ_SCREEN_GROUP_NUMBER = 8;
const int BAC_READ_MONITOR_GROUP_NUMBER = 4;
const int BAC_READ_ALARMLOG_GROUP_NUMBER = 4;
const int BAC_READ_TSTAT_GROUP_NUMBER = 8;
const int BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER = 8;

const int BAC_READ_GRPHIC_LABEL_GROUP_NUMBER = 6;
const int BAC_REMOTE_POINT_GROUP_NUMBER = 25;
const int BAC_ALALOG_CUSTMER_RANGE_TABLE_GROUP_NUMBER = 1;


const int BAC_READ_INPUT_REMAINDER = BAC_READ_INPUT_GROUP_NUMBER - 1;
const int BAC_READ_OUTPUT_REMAINDER = BAC_READ_OUTPUT_GROUP_NUMBER - 1;
const int BAC_READ_VARIABLE_REMAINDER = BAC_READ_VARIABLE_GROUP_NUMBER - 1;
const int BAC_READ_PROGRAM_REMAINDER = BAC_READ_PROGRAM_GROUP_NUMBER - 1;
const int BAC_READ_PID_REMAINDER = BAC_READ_PID_GROUP_NUMBER - 1;
const int BAC_READ_SCHEDULE_REMAINDER = BAC_READ_SCHEDULE_GROUP_NUMBER - 1;
const int BAC_READ_HOLIDAY_REMAINDER = BAC_READ_HOLIDAY_GROUP_NUMBER - 1;
const int BAC_READ_USER_LOGIN_INFO_REMAINDER = BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER - 1;

const int BAC_READ_CUSTOMER_UNITS_REMAINDER = BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER - 1;;

const int BAC_READ_SCREEN_REMAINDER = BAC_READ_SCREEN_GROUP_NUMBER - 1;
const int BAC_READ_MONOTOR_REMAINDER = BAC_READ_MONITOR_GROUP_NUMBER - 1;
const int BAC_READ_ALARMLOG_REMAINDER = BAC_READ_ALARMLOG_GROUP_NUMBER - 1;
const int BAC_READ_TSTAT_REMAINDER = BAC_READ_TSTAT_GROUP_NUMBER - 1;
const int BAC_READ_GRPHIC_LABEL_REMAINDER	= BAC_READ_GRPHIC_LABEL_GROUP_NUMBER - 1;
const int BAC_REMOTE_POINT_GROUP_REMAINDER = BAC_REMOTE_POINT_GROUP_NUMBER - 1;


const int BAC_ALALOG_CUSTMER_RANGE_TABLE_REMAINDER = BAC_ALALOG_CUSTMER_RANGE_TABLE_GROUP_NUMBER - 1;

const int BAC_INPUT_ITEM_COUNT = 64;	//sizeof(Str_in_point);					//46
const int BAC_OUTPUT_ITEM_COUNT = 64;	//sizeof(Str_out_point);					//45
const int BAC_VARIABLE_ITEM_COUNT = 128; // sizeof(Str_variable_point);			//39
const int BAC_PROGRAM_ITEM_COUNT = 16;	//sizeof(Str_program_point);				//37
const int BAC_PROGRAMCODE_ITEM_COUNT = 16;

const int BAC_PID_COUNT = 16;
const int BAC_SCREEN_COUNT = 16;

const int BAC_SCHEDULE_COUNT = 8;
const int BAC_WEEKLYCODE_ROUTINES_COUNT = 8;

const int BAC_HOLIDAY_COUNT = 4;
const int BAC_ANNUAL_CODE_COUNT = 4;

const int BAC_MONITOR_COUNT = 12;
const int BAC_ALARMLOG_COUNT = 16;
const int BAC_TSTAT_COUNT = 32;
const int BAC_CUSTOMER_UNITS_COUNT = 8;

const int BAC_SCHEDULE_TIME_COUNT = 8;
const int BAC_TIME_COMMAND_COUNT = 1;
const int BAC_BASIC_SETTING_COUNT = 1;
const int BAC_USER_LOGIN_COUNT = 8;
const int BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT = 5;

const int BAC_CONNECT_WITH_DEVICE_COUNT = 1;

const int BAC_GRPHIC_LABEL_COUNT = 240;
const int BAC_REMOTE_POINT_COUNT = 128;





const int BAC_INPUT_GROUP = (BAC_INPUT_ITEM_COUNT + BAC_READ_INPUT_GROUP_NUMBER - 1)  / BAC_READ_INPUT_GROUP_NUMBER;
const int BAC_OUTPUT_GROUP = (BAC_OUTPUT_ITEM_COUNT + BAC_READ_OUTPUT_GROUP_NUMBER -1)  / BAC_READ_OUTPUT_GROUP_NUMBER;
const int BAC_VARIABLE_GROUP = (BAC_VARIABLE_ITEM_COUNT + BAC_READ_VARIABLE_GROUP_NUMBER - 1) / BAC_READ_VARIABLE_GROUP_NUMBER;

const int BAC_PROGRAM_GROUP = (BAC_PROGRAM_ITEM_COUNT + BAC_READ_PROGRAM_GROUP_NUMBER - 1) / BAC_READ_PROGRAM_GROUP_NUMBER;
const int BAC_PROGRAMCODE_GROUP = BAC_PROGRAMCODE_ITEM_COUNT;

const int BAC_PID_GROUP = (BAC_PID_COUNT + BAC_READ_PID_GROUP_NUMBER - 1) / BAC_READ_PID_GROUP_NUMBER;

const int BAC_SCHEDULE_GROUP = (BAC_SCHEDULE_COUNT + BAC_READ_SCHEDULE_GROUP_NUMBER - 1) / BAC_READ_SCHEDULE_GROUP_NUMBER;
const int BAC_SCHEDULECODE_GOUP = BAC_WEEKLYCODE_ROUTINES_COUNT;
const int BAC_HOLIDAY_GROUP = (BAC_HOLIDAY_COUNT + BAC_READ_HOLIDAY_GROUP_NUMBER - 1) / BAC_READ_HOLIDAY_GROUP_NUMBER;
const int BAC_TIME_COMMAND_GROUP = 1;
const int BAC_BASIC_SETTING_GROUP = 1;
const int BAC_HOLIDAYCODE_GROUP = BAC_ANNUAL_CODE_COUNT;
const int BAC_SCREEN_GROUP = (BAC_SCREEN_COUNT + BAC_READ_SCREEN_GROUP_NUMBER -1) / BAC_READ_SCREEN_GROUP_NUMBER;
const int BAC_MONITOR_GROUP = (BAC_MONITOR_COUNT + BAC_READ_MONITOR_GROUP_NUMBER - 1) / BAC_READ_MONITOR_GROUP_NUMBER;
const int BAC_CONNECT_WITH_DEVICE_GROUP = 1;
const int BAC_ALARMLOG_GROUP = (BAC_ALARMLOG_COUNT + BAC_READ_ALARMLOG_GROUP_NUMBER - 1) / BAC_READ_ALARMLOG_GROUP_NUMBER;
const int BAC_TSTAT_GROUP = (BAC_TSTAT_COUNT + BAC_READ_TSTAT_GROUP_NUMBER - 1) / BAC_READ_TSTAT_GROUP_NUMBER;
const int BAC_CUSTOMER_UNIT_GROUP = (BAC_CUSTOMER_UNITS_COUNT + BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER - 1) / BAC_READ_CUSTOMER_UNITS_GROUP_NUMBER;
const int BAC_USER_LOGIN_GROUP = (BAC_USER_LOGIN_COUNT + BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER - 1) / BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER;
const int BAC_GRPHIC_LABEL_GROUP = (BAC_GRPHIC_LABEL_COUNT + BAC_READ_GRPHIC_LABEL_GROUP_NUMBER - 1)/ BAC_READ_GRPHIC_LABEL_GROUP_NUMBER;
const int BAC_REMOTE_POINT_GROUP = (BAC_REMOTE_POINT_COUNT + BAC_REMOTE_POINT_GROUP_NUMBER - 1) / BAC_REMOTE_POINT_GROUP_NUMBER;

const int BAC_SHOW_CONNECT_RESULTS = 1;
const int BAC_SHOW_MISSION_RESULTS = 3;

const int BAC_LIST_REFRESH_TIME = 15000;//ms

  
const int SCHEDULE_TIME_NUM = 0;
const int SCHEDULE_TIME_MONDAY = 1;
const int SCHEDULE_TIME_TUESDAY = 2;
const int SCHEDULE_TIME_WEDNESDAY = 3;
const int SCHEDULE_TIME_THURSDAY = 4;
const int SCHEDULE_TIME_FRIDAY = 5;
const int SCHEDULE_TIME_SATURDAY = 6;
const int SCHEDULE_TIME_SUNDAY = 7;
const int SCHEDULE_TIME_HOLIDAY1 = 8;
const int SCHEDULE_TIME_HOLIDAY2 = 9;


const int WEEKLY_SCHEDULE_SIZE = 144;
const int ANNUAL_CODE_SIZE = 46;
struct _Bac_Scan_Com_Info
{
	int device_id;
	int macaddress;
};
struct _Bac_Scan_results_Info
{
	int device_id;
	char ipaddress[6];
	unsigned char modbus_addr;
	unsigned char product_type;
	unsigned int serialnumber;
	unsigned char panel_number;
	unsigned short modbus_port;
	unsigned short software_version;
	unsigned char hardware_version;
	unsigned char m_protocol;
};

struct Client_Info
{
	SOCKET client_socket;
	CString IPAddress;
	int ipport;
	CString Product_name;
	unsigned int serialnumber;
	int object_instance;
	int pannel_number;
	unsigned char protocol;
	
};

struct _Resend_Read_Info
{
	int device_id;
	int command;
	int start_instance;
	int end_instance;
	int resend_count;
	int task_result;
	int invoke_id;
	int has_resend_yes_or_no;
	int timeout_count;//多少次还没收到回复，就算 失败;
};

struct _Com_Scan_Read_Info
{
	int device_id;
	int command;
	int start_instance;
	int end_instance;
	int resend_count;
	int invoke_id;
	int retry_time;
};

struct refresh_net_device
{
	DWORD nSerial;
	int modbusID;
	unsigned short product_id;
	CString ip_address;
	int nport;
	float sw_version;
	float hw_version;
	unsigned int object_instance;
	unsigned char panal_number;
	DWORD parent_serial_number;
	CString NetCard_Address;
	CString show_label_name;
};

struct refresh_net_label_info
{
	char label_name[20];
	unsigned int serial_number;
};

struct _Refresh_Info 
{
	_Resend_Read_Info Read_Variable_Info[BAC_VARIABLE_GROUP];
	_Resend_Read_Info Read_Input_Info[BAC_INPUT_GROUP];
	_Resend_Read_Info Read_Output_Info[BAC_OUTPUT_GROUP];
	_Resend_Read_Info Read_Program_Info[BAC_PROGRAM_GROUP];
	_Resend_Read_Info Read_Programcode_Info[BAC_PROGRAMCODE_GROUP];
	_Resend_Read_Info Read_Weekly_Info[BAC_SCHEDULE_GROUP];
	_Resend_Read_Info Read_Weeklycode_Info[BAC_SCHEDULECODE_GOUP];
	_Resend_Read_Info Read_Annual_Info[BAC_HOLIDAY_GROUP];
	_Resend_Read_Info Read_Annualcode_Info[BAC_HOLIDAYCODE_GROUP];
	_Resend_Read_Info Read_Time_Command[BAC_TIME_COMMAND_GROUP];
	_Resend_Read_Info Read_Controller_Info[BAC_PID_GROUP];
	_Resend_Read_Info Read_Screen_Info[BAC_SCREEN_GROUP];
	_Resend_Read_Info Read_Monitor_Info[BAC_MONITOR_GROUP];
	_Resend_Read_Info Read_Connect_With_Device[BAC_CONNECT_WITH_DEVICE_GROUP];
	_Resend_Read_Info Read_AlarmLog_Info[BAC_ALARMLOG_GROUP];
	_Resend_Read_Info Read_Tstat_Info[BAC_TSTAT_GROUP];
	_Resend_Read_Info Read_BasicSetting_Info[BAC_BASIC_SETTING_GROUP];
	_Resend_Read_Info Read_Customer_unit_Info[BAC_CUSTOMER_UNIT_GROUP];
	_Resend_Read_Info Read_UserLogin[BAC_USER_LOGIN_GROUP];
	_Resend_Read_Info Read_Label_Graphic_Info[BAC_GRPHIC_LABEL_GROUP];
	_Resend_Read_Info Read_Remote_Point_Info[BAC_REMOTE_POINT_GROUP];
};

struct _Refresh_Write_Info 
{
	_Resend_Read_Info Write_Variable_Info[BAC_VARIABLE_GROUP];
	_Resend_Read_Info Write_Input_Info[BAC_INPUT_GROUP];
	_Resend_Read_Info Write_Output_Info[BAC_OUTPUT_GROUP];
	_Resend_Read_Info Write_Program_Info[BAC_PROGRAM_GROUP];
	_Resend_Read_Info Write_Programcode_Info[BAC_PROGRAMCODE_GROUP];
	_Resend_Read_Info Write_Weekly_Info[BAC_SCHEDULE_GROUP];
	_Resend_Read_Info Write_Weeklycode_Info[BAC_SCHEDULECODE_GOUP];
	_Resend_Read_Info Write_Annual_Info[BAC_HOLIDAY_GROUP];
	_Resend_Read_Info Write_Annualcode_Info[BAC_HOLIDAYCODE_GROUP];
	_Resend_Read_Info Write_Time_Command[BAC_TIME_COMMAND_GROUP];
	_Resend_Read_Info Write_Controller_Info[BAC_PID_GROUP];
	_Resend_Read_Info Write_Screen_Info[BAC_SCREEN_GROUP];
	_Resend_Read_Info Write_Monitor_Info[BAC_MONITOR_GROUP];
};


struct Monitor_Input_Info
{
	int Max_Value;
	int Min_Value;
	bool be_record;//是否记录了这条;
	bool use_own_scale;//是否单独用自己的刻度;
	bool show_graphic; //是否显示图像;
};

struct Changed_Item_Info
{
	int nRow;
	int nCol;
};

/*typedef*/ struct _MessageWriteListInfo
{
	uint32_t deviceid;
	int8_t command;
	int8_t start_instance;
	int8_t end_instance;
	unsigned short entitysize;
	CString Write_Info;
	HWND hWnd;
	Changed_Item_Info ItemInfo;
};

/*typedef*/ struct _MessageRefreshListInfo
{
	uint32_t deviceid;
	int8_t command;
	int8_t start_instance;
	int8_t end_instance;
	unsigned short entitysize;
	int8_t block_size;
	HWND hWnd;
};

struct Data_Time_Match
{
	int analogdata;
	unsigned long loggingtime;
};

struct Point_Data_str
{
	PointF data_point;
	int analogdata;
};


const int UNITS_TYPE_ANALOG = 0;
const int UNITS_TYPE_DIGITAL = 1;
const int UNITS_TYPE_CUSTOM = 2;

const int INPUT_ANOLAG_UNITE_COUNT = 19;
const int VARIABLE_ANALOG_UNITE_COUNT = 34;

const CString Units_Type[]=
{
	_T("Analog"),
	_T("Digital")
	//_T("Custom dig")
};

const CString Units_Analog_Only = _T("Analog");
const CString Units_Digital_Only = _T("Digital");

const CString Yes_No[] = 
{
	_T("NO"),
	_T("YES")
};

const CString ACK_UNACK[] = 
{
	_T(""),
	_T("ACK")
};

const CString Digital_Units_Array[] = 
{
	_T("Unused"),
	_T("Off/On"),
	_T("Close/Open"),
	_T("Stop/Start"),
	_T("Disable/Enable"),
	_T("Normal/Alarm"),
	_T("Normal/High"),
	_T("Normal/Low"),
	_T("No/Yes"),
	_T("Cool/Heat"),
	_T("Unoccupy/Occupy"),
	_T("Low/High"),
	_T("On/Off"),
	_T("Open/Close"),
	_T("Start/Stop"),
	_T("Enable/Disable"),
	_T("Alarm/Normal"),
	_T("High/Normal"),
	_T("Low/Normal"),
	_T("Yes/No"),
	_T("Heat/Cool"),
	_T("Occupy/Unoccupy"),
	_T("High/Low")
};

//const CString Input_Analog_Units_Show[] =
//{
//	_T("")
//};

const CString Output_Analog_Units_Show[] = 
{
	_T(""),
	_T("Volts"),
	_T("%Open"),
	_T("psi"),
	_T("%"),
	_T("%Cls"),
	_T("ma"),	
};

const CString OutPut_List_Analog_Range[] =
{
	_T(""),
	_T("0.0 -> 10"),
	_T("0.0 -> 100"),
	_T("0.0 -> 20"),
	_T("0.0 -> 100"),
	_T("0.0 -> 100"),
	_T("0.0 -> 20"),
	_T("0.0 -> 100")
};

const CString OutPut_List_Analog_Units[] =
{
	_T(""),
	_T("Volts"),
	_T("%Open"),
	_T("psi"),
	_T("%"),
	_T("%Cls"),
	_T("ma"),
	_T("%PWM")
};

const CString Input_List_Analog_Units[] =
{
	_T(""),
	 _T("Deg.C"),
	 _T("Deg.F"),
	 _T("Deg.C"),
	 _T("Deg.F"),
	 _T("Deg.C"),
	 _T("Deg.F"),
	 _T("Deg.C"),
	 _T("Deg.F"),
	 _T("Deg.C"),
	 _T("Deg.F"),
	 _T("Volts"),
	 _T("Amps"),
	 _T("ma"),
	 _T("psi"),
	 _T("counts"),
	 _T("FPM"),
	 _T("%(0-5V)"),
	 _T("%(4-20ma)"),
	 _T("Volts"),
	 _T(""),
	 _T(""),
	 _T(""),
	 _T(""),
	 _T(""),
	 _T("counts"),
	 _T("HZ")
};

const CString Input_Analog_Units_Array[] =
{
	 _T("Unused"),
	 _T("Y3K -40 to 150"),
	 _T("Y3K -40 to 300"),
	 _T("10K -40 to 120"),
	 _T("10K -40 to 250"),
	 _T("G3K -40 to 120"),
	 _T("G3K -40 to 250"),
	 _T("KM10K -40 to 120"),
	 _T("KM10K -40 to 250"),
	 _T("A10K -50 to 110"),
	 _T("A10K -60 to 200"),
	 _T("0.0 to 5.0"),
	 _T("0.0 to 100"),
	 _T("0.0 to 20"),
	 _T("0.0 to 20"),
	 _T("Speed Count"),
	 _T("0.0 to 3000"),
	 _T("0 to 100"),
	 _T("0 to 100"),
	  _T("0.0 to 10.0"),
	  _T("Table 1"),
	  _T("Table 2"),
	  _T("Table 3"),
	  _T("Table 4"),
	  _T("Table 5"),
	  _T("High Speed Count"),
	  _T("Frequency")
};

const CString Output_Analog_Units_Array[] = 
{
	_T("Unused"),
	_T("0.0 -> 10   Volts"),
	_T("0.0 -> 100  %Open"),
	_T("0.0 -> 20   psi"),
	_T("0.0 -> 100  %"),
	_T("0.0 -> 100  %Cls"),
	_T("0.0 -> 20   ma"),
	_T("0.0 -> 100  PWM")
};

const CString Time_Server_Name[] =
{
	_T("ntp.sjtu.edu.cn"),
	_T("time.nist.gov"),
	_T("time.windows.com")
	
};

const signed short Time_Zone_Value[] =
{
	-1200,
	-1100,
	-1000,
	-900,
	-800,
	-700,
	-600,
	-500,
	-400,
	-300,
	-200,
	-100,
	0,
	100,
	200,
	300,
	400,
	500,
	600,
	700,
	800,
	900,
	1000,
	1100,
	1200,
	1300
};

const CString Time_Zone_Name[] =
{
	_T("(UTC - 12:00) , Yankee Time Zone"),
	_T("(UTC - 11:00) , X-ray Time Zone"),
	_T("(UTC - 10:00) , Cook Island , Hawaii-Aleutian Standard Time"),
	_T("(UTC - 09:00) , Alaska Standard Time , Gambier Time"),
	_T("(UTC - 08:00) , Pacific Standard Time , Tiempo del Pacífico"),
	_T("(UTC - 07:00) , Mountain Standard Time , Pacific Daylight Time"),
	_T("(UTC - 06:00) , Central Standard Time , Galapagos Time"),
	_T("(UTC - 05:00) , Eastern Standard Time"),
	_T("(UTC - 04:00) , Atlantic Standard Time , Bolivia Time"),
	_T("(UTC - 03:00) , Atlantic Daylight Time , Amazon Summer Time"),
	_T("(UTC - 02:00) , Fernando de Noronha Time , Oscar Time Zone"),
	_T("(UTC - 01:00) , Azores Time , Cape Verde Time"),
	_T("(UTC) , Azores Summer Time"),
	_T("(UTC + 01:00) , Alpha Time Zone"),
	_T("(UTC + 02:00) , Central Africa Time , Central European Summer Time"),
	_T("(UTC + 03:00) , Charlie Time Zone , Eastern Africa Time"),
	_T("(UTC + 04:00) , Armenia Time , Azerbaijan Time"),
	_T("(UTC + 05:00) , Armenia Summer Time , Aqtobe Time"),
	_T("(UTC + 06:00) , Bangladesh Standard Time , Bhutan Time"),
	_T("(UTC + 07:00) , Christmas Island Time , Davis Time"),
	_T("(UTC + 08:00) , China Standard Time , Hotel Time Zone"),
	_T("(UTC + 09:00) , Australian Western Daylight Time"),
	_T("(UTC + 10:00) , Chamorro Standard Time , Kilo Time Zone"),
	_T("(UTC + 11:00) , Lima ,Vanuatu , Lima "),
	_T("(UTC + 12:00) , Anadyr Time , Fiji Time , Gilbert Island Time"),
	_T("(UTC + 13:00) , New Zealand Daylight Time")
};

const CString DDNS_Server_Name[] =
{
	_T("www.3322.org"),
	_T("www.dyndns.com"),
	_T("www.no-ip.com"),
	_T("newfirmware.com")
};

const CString Device_Serial_Port_Status[] =
{
	_T("Unused"),
	_T("Bacnet MSTP"),
	_T("Modbus Slave"),
	_T("Bacnet PTP"),
	_T("GSM"),
	_T("Main Zigbee"),
	_T("Sub Zigbee"),
	_T("Modbus Master"),
};

const CString Variable_Analog_Units_Array[] = 
{
	_T("Unused"),
	_T("Deg.C"),
	_T("Deg.F"),
	_T("Feet per Min"),
	_T("Pascals"),
	_T("KPascals"),
	_T("lbs/sqr.inch"),
	_T("inches of WC"),
	_T("Watts"),
	_T("KWatts"),
	_T("KWH"),
	_T("Volts"),
	_T("KV"),
	_T("Amps"),//13
	_T("ma"),
	_T("CFM"),
	_T("Seconds"),
	_T("Minutes"),
	_T("Hours"),
	_T("Days"),
	_T("Time"),
	_T("Ohms"),
	_T("%"),
	_T("%RH"),
	_T("p/min"),
	_T("Counts"),
	_T("%Open"),
	_T("Kg"),
	_T("L/Hour"),
	_T("GPH"),
	_T("GAL"),
	_T("CF"),
	_T("BTU"),
	_T("CMH"),
};



struct _Graphic_Value_Info
{
	uint32_t deviceid;
	int8_t command;
	int8_t value_type;
	int8_t value_item;
	unsigned short entitysize;
	CRelayLabel* control_pt;
	HWND hWnd;
};


const int WINDOW_TAB_COUNT = 14; //多少个Window 嵌入在TAB里面;
const int WINDOW_INPUT = 0;
const int WINDOW_OUTPUT = 1;
const int WINDOW_VARIABLE = 2;
const int WINDOW_PROGRAM = 3;
const int WINDOW_CONTROLLER = 4;
const int WINDOW_SCREEN = 5;
const int WINDOW_WEEKLY = 6;
const int WINDOW_ANNUAL = 7;
const int WINDOW_MONITOR = 8;
const int WINDOW_ALARMLOG = 9;
const int WINDOW_TSTAT	= 10;
const int WINDOW_SETTING = 11;
const int WINDOW_USER_LOGIN = 12;
const int WINDOW_REMOTE_POINT = 13;



const int KEY_INSERT = 1020;


typedef enum
{		
	PRODUCT_CM5 = 0,//10A + 8 D			//10D
	//IN				//OUT	
	BIG_MINIPANEL = 1,			//32 A				//12D   12A
	SMALL_MINIPANEL = 2,		//16 A				//6 D	4 A
	TINY_MINIPANEL = 3,	
	PID_T322AI = 43,
	T38AI8AO6DO = 44,
	PID_T3PT12 = 46
};

const int BIG_MINIPANEL_IN_A = 32;
const int BIG_MINIPANEL_IN_D = 0;	//If  this place is not 0 ,need code.
const int SMALL_MINIPANEL_IN_A = 16;
const int SMALL_MINIPANEL_IN_D = 0;
const int CM5_MINIPANEL_IN_A = 10;
const int CM5_MINIPANEL_IN_D = 8;

const int TINY_MINIPANEL_IN_A = 11;
const int TINY_MINIPANEL_IN_D = 0;

const int BIG_MINIPANEL_OUT_A = 12;
const int BIG_MINIPANEL_OUT_D = 12;
const int SMALL_MINIPANEL_OUT_A = 4;
const int SMALL_MINIPANEL_OUT_D = 6;
const int CM5_MINIPANEL_OUT_A = 0;
const int CM5_MINIPANEL_OUT_D = 10;

const int TINY_MINIPANEL_OUT_D = 6;
const int TINY_MINIPANEL_OUT_A = 2;

const int T38AI8AO6DO_OUT_D = 6;
const int T38AI8AO6DO_OUT_A = 8;

const int T322AI_OUT_D = 0;
const int T322AI_OUT_A = 0;

enum
{
	UART_1200 = 0,
	UART_2400,
	UART_3600,
	UART_4800,
	UART_7200,
	UART_9600,
	UART_19200,
	UART_38400,
	UART_57600,
	UART_115200,
	UART_921600

};


const CString Baudrate_Array[] = 
{
	_T("1200"),
	_T("2400"),
	_T("3600"),
	_T("4800"),
	_T("7200"),
	_T("9600"),
	_T("19200"),
	_T("38400"),
	_T("76800"),
	_T("115200"),
	_T("921600"),
};

typedef struct
{
	SOCKET m_gsm_socket;
	UINT   product_serialnumber;
}GSM_connection_info;


enum
{
	SCAN_BY_SERIAL_PORT = 1,
	SCAN_BY_UDP,
	SCAN_BY_BACNET_IP,
	SCAN_BY_MSTP,
	SCAN_BY_REMOTE_IP
};

enum
{
	SCAN_STATUS_WAIT = 1,
	SCAN_STATUS_RUNNING ,
	SCAN_STATUS_FINISHED ,
	SCAN_STATUS_FAILED,
	SCAN_STATUS_SKIP
};

typedef struct
{
	int scan_list_item;
	int scan_mode;
	int scan_com_port;
	int scan_baudrate;
	bool scan_skip;
	int scan_status;
	char scan_notes[250];
	int scan_found;
}Scan_Info;	// 扫描的时候 用于显示给list 的结构;


typedef struct
{
	int nSerialNum;
	int nScreen_index;
	int nLabel_index;
	uint8_t nMain_Panel;
	uint8_t nSub_Panel;
	uint8_t nPoint_type;
	uint8_t nPoint_number;
	int  nPoint_x;
	int  nPoint_y;
	COLORREF nclrTxt;
	uint8_t nDisplay_Type;
	uint8_t nMouse_Status;
	char ico_name[10];
	char ico_name_2[10];
	uint8_t ntext_place;
	uint8_t n_iconsize;
}Bacnet_Label_Info;

const int LABEL_MOUSE_NORMAL = 0;
const int LABEL_MOUSE_ON = 1;
const int LABEL_MOUSE_ON_LB_DOWN = 2;
const int LABEL_MOUSE_ON_LB_UP = 3;




#pragma region Label_Display
const int LABEL_SHOW_VALUE = 0;
const int LABEL_SHOW_FULL_DESCRIPTION = 1;
const int LABEL_SHOW_LABEL = 2;
const int LABEL_ICON_VALUE = 3;
const int LABEL_ICON_FULL_DESCRIPTION = 4;
const int LABEL_ICON_LABEL = 5;
const CString Label_Display_Array[] = 
{
	_T("Value"),
	_T("Full Description"),
	_T("Label"),
	_T("Icon"),
	_T("Icon Description"),
	_T("Icon Label"),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
};
#pragma endregion Label_Display


#pragma region Label_position
const int  LABEL_TEXT_BOTTOM = 0;
const int LABEL_TEXT_LEFT = 1;
const int  LABEL_TEXT_TOP = 2;
const int  LABEL_TEXT_RIGHT = 3;

const CString Label_Text_Place[] = 
{
	_T("Bottom"),
	_T("Left"),
	_T("Top"),
	_T("Right"),
	_T(""),
	_T(""),
	_T(""),
	_T(""),
};
#pragma endregion Label_position


#pragma region Label_Icon_Size
const int  LABEL_ICON_SMALL = 0;
const int LABEL_ICON_NORMAL = 1;
const int  LABEL_ICON_LARGE = 2;

const CString Label_ICON_SIZE[] = 
{
	_T("Small"),
	_T("Normal"),
	_T("Large"),
	_T(""),
	_T(""),
};

#pragma endregion Label_Icon_Size



const int LOGIN_SUCCESS_READ_ONLY = 1;
const int LOGIN_SUCCESS_FULL_ACCESS = 2;
const int LOGIN_SUCCESS_GRAPHIC_MODE = 3;
const int LOGIN_SUCCESS_ROUTINE_MODE = 4;
const int LOGIN_USER_ERROR = 100;
const int LOGIN_PASSWORD_ERROR = 110;
const int LOGIN_USER_OR_PASSWORD_ERROR = 120;

#define LABEL_START_ID 1

const int AT_CLEAR = 0;
const int AT_SEND = 1;
const int AT_OPEN = 2;
const int AT_CLOSE = 3;

const int NO_UNSED_LABEL = 0;
const int ENABLE_LABEL = 1;
const int EMPTY_LABEL = 2;

const int SCREEN_HOTKEY_COUNT = 9;
const int m_screenHotKeyID[SCREEN_HOTKEY_COUNT] = 
{
	4001,
	4002,
	4003,
	4004,
	4005,
	4006,
	4007,
	4008,
	4009
};

struct AddressMap
{
	char AddressName[255];
	int AddressValue;
};

const int P_MODBUS_485 = 0;
const int P_MODBUS_TCP = 1;
const int P_BACNET_MSTP = 2;
const int P_BACNET_IP = 3;
const int P_GSM = 4;
const int P_AUTO = 5;
const int P_REMOTE_DEVICE = 6;

//const CString Building_Protocol[] =
//{
//	_T("Modbus 485"),
//	_T("Modbus TCP"),
//	_T("Bacnet MSTP"),
//	_T("Bacnet IP"),
//	_T("GSM"),
//	_T("Auto"),
//	_T("Remote Device")
//
//};

const CString Building_Protocol[] =
{
	_T("Modbus 485"),
	_T("Modbus TCP"),
	_T("Bacnet MSTP"),
	_T("Remote Device"),
	_T("Auto")
};

const CString Building_Baudrate[] =
{
	_T("9600"),
	_T("19200"),
    _T("38400"),
    _T("57600"),
    _T("115200")
};

#define  DEFAULT_PRG_TEXT_COLOR			RGB(0,0,255)
#define  DEFAULT_PRG_LABEL_COLOR		RGB(84,193,255)
#define  DEFAULT_PRG_COMMAND_COLOR		RGB(0,0,130)
#define  DEFAULT_PRG_FUNCTION_COLOR		RGB(4,135,255)
#define  DEFAULT_PRG_ERROR_LABEL_COLOR	RGB(255,0,0)


const CString Program_Fonts[] = 
{
	_T("Default"),
	_T("Arial"),
	_T("NSimSun"),
	_T("Arial Black"),
	_T("Colonna MT"),
	_T("GulimChe"),
	_T("SimSun-ExtB"),
	_T("Times New Roman"),
	_T("Dotum"),
	_T("DotumChe"),
	_T("Gulim"),
	_T("GulimChe"),
	_T("Malgun Gothic"),
	_T("Meiryo"),
	_T("Microsoft JhengHei"),
	_T("MingLiU"),
	_T("Arial Narrow")
};

const CString JumperStatus[]=
{
	_T("Thermistor Dry Contact"),
	_T("4-20 ma"),//_T("4-20 ma / 0-24 V"),
	_T("0-5 V"),
	_T("0-10 V"),
	_T("Thermistor Dry Contact")
};


const int T3_REG_TOTAL_COUNT = 292;

const unsigned char MONITOR_READ_TIMEOUT = 0;
const unsigned char MONITOR_READ_SUCCESS = 1;
const unsigned char MONITOR_READ_NO_DATA = 2;

#pragma region VARIABLE_DEFINE

const int VARIABLE_NUM = 0;
const int VARIABLE_FULL_LABLE = 1;
const int VARIABLE_AUTO_MANUAL = 2;
const int VARIABLE_VALUE = 3;
const int VARIABLE_UNITE = 4;
const int VARIABLE_LABLE = 5;
const int VARIABLE_COL_NUMBER = 6;

#pragma endregion VARIABLE_DEFINE


#pragma region INPUT_DEFINE

const int INPUT_NUM = 0;
const int INPUT_PANEL = 1;
const int INPUT_FULL_LABLE = 2;
const int INPUT_AUTO_MANUAL = 3;
const int INPUT_VALUE = 4;
const int INPUT_UNITE = 5;
const int INPUT_RANGE = 6;
const int INPUT_CAL = 7;
const int INPUT_CAL_OPERATION = 8;
const int INPUT_FITLER = 9;
const int INPUT_DECOM = 10;
const int INPUT_JUMPER = 11;
const int INPUT_LABLE = 12;
const int INPUT_EXTERNAL = 13;
const int INPUT_PRODUCT = 14;
const int INPUT_EXT_NUMBER = 15;
const int INPUT_COL_NUMBER = 16;




const int TSTAT_INPUT_FITLER = 7;
const int TSTAT_INPUT_FUNCTION = 8;
const int TSTAT_INPUT_CUST_FIELD = 9;

const int TSTAT_INPUT_COL_NUMBER = 10;

const CString Input_Range_Array[7] = 
{
	_T("Not Used"),
	_T("10K(-40->120)"),
	_T("I 4->20ma"),
	_T("V 0->10V"),
	_T("V 0->5V"),
	_T("V 0->24AC"),
	_T("TST Normal"),
};

const CString Input_Filter_Array[8] =
{
	_T("1"),
	_T("2"),
	_T("4"),
	_T("8"),
	_T("16"),
	_T("32"),
	_T("64"),
	_T("128")
};




const CString Input_Unit[42]=
{
	_T("Not Used"), 
	_T("℃"),
	_T("℉"), 
	_T("FPM"), 
	_T("Pa"), 
	_T("KPa"), 
	_T("psi"), 
	_T("in_w"), 
	_T("Watts"), 
	_T("KW"), 
	_T("KWH"),
	_T("Volt"), 
	_T("KV"), 
	_T("Amps"), 
	_T("mA"), 
	_T("CFM"), 
	_T("Sec"), 
	_T("Min"), 
	_T("Hours"), 
	_T("Days"), 
	_T("time_unit"), 
	_T("ohms"),
	_T("procent"), 
	_T("RH"), 
	_T("ppm"), 
	_T("counts"), 
	_T("Open"), 
	_T("CFH"), 
	_T("GPM"), 
	_T("GPH"), 
	_T("GAL"), 
	_T("CF"), 
	_T("BTU"), 
	_T("CMH"),
	_T("custom1"), 
	_T("custom2"), 
	_T("custom3"), 
	_T("custom4"), 
	_T("custom5"), 
	_T("custom6"), 
	_T("custom7"), 
	_T("custom8")
};

const CString Input_Unit_Digit[2]=
{ 
	_T("Not Used"),
	_T("ON/OFF")
} ;



const CString Decom_Array[3] =
{
	_T("Normal"),
	_T("Open"),
	_T("Shorted")
};



#pragma endregion INPUT_DEFINE


#pragma region OUTPUT_DEFINE

const int OUTPUT_NUM = 0;
const int OUTPUT_PANEL = 1;
const int OUTPUT_FULL_LABLE = 2;
const int OUTPUT_AUTO_MANUAL = 3;
const int OUTPUT_HW_SWITCH = 4;
const int OUTPUT_VALUE = 5;
const int OUTPUT_UNITE = 6;
const int OUTPUT_RANGE = 7;
const int OUTPUT_PWM_PERIOD = 8;
const int OUTPUT_DECOM = 9;
const int OUTPUT_LABLE = 10;
const int OUTPUT_EXTERNAL = 11;
const int OUTPUT_PRODUCT = 12;
const int OUTPUT_EXT_NUMBER = 13;
const int OUTPUT_COL_NUMBER = 14;

const CString Output_Decom_Array[2] =
{
	_T("OK"),
	_T("-"),
};

typedef enum
{
	HW_SW_OFF = 0,
	HW_SW_AUTO = 1,
	HW_SW_HAND = 2
}HW_SWITCH_ENUM;
#pragma endregion OUTPUT_DEFINE


const CString Debug_Combo_Choose[] =
{
	_T("Show Nothing"),
	_T("Show All"),
	_T("Only Scan"),
	_T("Only Monotor Data"),
	_T("Only Program Code"),
	_T("Screen Pictrue Data"),
	_T("Bacnet Data")
};

const CString System_Log[] =
{
	_T("SUB_TX"),
	_T("SUB_RX"),
	_T("ZIG/GSM_TX"),
	_T("ZIG/GSM_RX"),
	_T("MAIN_TX"),
	_T("MAIN_RX"),
	_T("ETHER_RX"),
	_T("ETHER_TX"),
	_T("BOOT")
};


const CString System_Unit[] =
{
	_T("Byte/s"),
	_T("Byte/s"),
	_T("Byte/s"),
	_T("Byte/s"),
	_T("Byte/s"),
	_T("Byte/s"),
	_T("Package/s"),
	_T("Package/s"),
	_T("Off/On")
};





const int DEBUG_SHOW_NOTHING = 0;
const int DEBUG_SHOW_ALL = 1;
const int DEBUG_SHOW_SCAN_ONLY = 2;
const int DEBUG_SHOW_MONITOR_DATA_ONLY = 3;
const int DEBUG_SHOW_PROGRAM_DATA_ONLY = 4;
const int DEBUG_SHOW_WRITE_PIC_DATA_ONLY = 5;
const int DEBUG_SHOW_BACNET_ALL_DATA = 6;
#define NUMBER_BAUDRATE 5
typedef struct
{
	char crc_cal[4];
	unsigned int total_packet;
	unsigned int pic_file_size;
	char md5_32byte[33];
	char file_name[11];

}pic_sd_struct;

typedef struct
{
	unsigned char download_product_type;
	char firmware_file_path[255];
}Str_download_firmware_info;



const int LENGTH_MODBUS_SETTING = 200;
const int LENGTH_MODBUS_OUTPUT = (45 + 1)/2 *  BAC_OUTPUT_ITEM_COUNT ;//  23*64;
const int LENGTH_MODBUS_INPUT = (46 + 1) / 2  * BAC_INPUT_ITEM_COUNT;	 // 23 * 64;
const int LENGTH_MODBUS_VARIABLE = (39 + 1) / 2 * BAC_VARIABLE_ITEM_COUNT; // 20 * 128;
const int LENGTH_MODBUS_PRG = (37 + 1) /2 * BAC_PROGRAM_ITEM_COUNT; // 19 * 16
const int LENGTH_MODBUS_SCHEDULE = (42 + 1) / 2  * BAC_SCHEDULE_COUNT; // 21 * 8;
const int LENGTH_MODBUS_HOLIDAY =  (33 + 1) / 2 * BAC_HOLIDAY_COUNT;   //17 * 4
const int LENGTH_MODBUS_PRG_CODE = 400 * 5 / 2 * BAC_PROGRAMCODE_ITEM_COUNT ; // 1000 * 16;
const int LENGTH_MODBUS_SCHEDULE_CODE = WEEKLY_SCHEDULE_SIZE / 2 * BAC_WEEKLYCODE_ROUTINES_COUNT ; // 72 * 8;
const int LENGTH_MODBUS_HOLIDAY_CODE = ANNUAL_CODE_SIZE /2 ;


//以下是 bacnet 寄存器 映射到modbus的位置;
const int REG_SETTING_START_ADDRESS  = 9800;
const int REG_OUTPUT_START_ADDRESS = REG_SETTING_START_ADDRESS + LENGTH_MODBUS_SETTING; //10000
const int REG_INPUT_START_ADDRESS = REG_OUTPUT_START_ADDRESS + LENGTH_MODBUS_OUTPUT;	//11472
const int REG_VARIABLE_START_ADDRESS  = REG_INPUT_START_ADDRESS + LENGTH_MODBUS_INPUT;	//12944
const int REG_PRG_START_ADDRESS = REG_VARIABLE_START_ADDRESS + LENGTH_MODBUS_VARIABLE;  //15504
const int REG_SCHEDULE_START_ADDRESS = REG_PRG_START_ADDRESS + LENGTH_MODBUS_PRG;		//15808



#define  RANGE_ERROR   -2



#define BAC_OUT 		 0
#define BAC_IN  		 1
#define BAC_VAR 		 2
#define BAC_PID 		 3
#define BAC_SCH 		 4
#define BAC_HOL 		 5
#define BAC_PRG 		 6
#define BAC_TBL 	 	 7
#define BAC_DMON 		 8
#define BAC_AMON	    9
#define BAC_GRP 	   10
#define BAC_AY        11                  /* table to hold file names */
#define BAC_ALARMM    12
#define BAC_UNIT      13
#define BAC_USER_NAME 14
#define BAC_ALARMS    15
#define BAC_WR_TIME   16
#define BAC_AR_Y      17


#define MAX_OBJ_INSTANCE  4194303


#define  MY_COLOR_RED			Color(255,255,0,0)
#define  MY_COLOR_BLACK_CHARACTER			Color(255,204,204,204)
#define  BLACK_GROUND_START_X    10
#define  BLACK_GROUND_START_Y    193
#define  BLACK_GROUND_WIDTH      400
#define  BLACK_GROUND_HEIGHT      40

#define  BLACK_2_GROUND_START_X    (BLACK_GROUND_START_X  + BLACK_GROUND_WIDTH + 60)
#define  BLACK_2_GROUND_START_Y    BLACK_GROUND_START_Y
#define  BLACK_2_GROUND_WIDTH      180
#define  BLACK_2_GROUND_HEIGHT      40

#define BLACK_PC_GROUND_START_X		BLACK_GROUND_START_X
#define BLACK_PC_GROUND_START_Y   ( BLACK_GROUND_START_Y + BLACK_GROUND_HEIGHT + 10 )
#define BLACK_PC_GROUND_WIDTH      BLACK_GROUND_WIDTH
#define BLACK_PC_GROUND_HEIGHT     BLACK_GROUND_HEIGHT


//从机 发起的 FF 55 后面的 请求位;
typedef enum
{
	SEND_MINIPANEL_INFO = 1,
	GET_MINIPANEL_INFO = 2
};


//主机应答的FF 55 后面的命令位;
typedef enum
{
	RECEIVE_DATA_LEBGTH_ERROR = 1,
	RECEIVE_MINI_DATA = 2,
	RECEIVE_UNKNOWN_COMMAND = 3,
	RECEIVE_MINIPANEL_NOT_CONNECT_YET = 4,
	RETURN_MINI_DATA = 5
};

#pragma pack(push) //保存对齐状态 
#pragma pack(1)
typedef struct
{
	unsigned int update_time;
	unsigned int sn;

	UCHAR	 product_id;
	UINT object_instance;
	UCHAR panel;
	unsigned short modbus_port;
	unsigned short bacnet_port;

	char username[20];
	char password[10];

	unsigned int external_ip;
	unsigned short external_tcp_port;

}STR_For_T3000;

#pragma pack(pop)//恢复对齐状态 



const int day_of_month[] = 
{
	31,
	29,
	31,
	30,
	31,
	30,
	31,
	31,
	30,
	31,
	30,
	31
};

const int day_in_this_year[] =
{
	0,       
	day_of_month[0],
	day_of_month[0] + day_of_month[1],
	day_of_month[0] + day_of_month[1] + day_of_month[2],
	day_of_month[0] + day_of_month[1] + day_of_month[2] +day_of_month[3],
	day_of_month[0] + day_of_month[1] + day_of_month[2] +day_of_month[3] + +day_of_month[4],
	day_of_month[0] + day_of_month[1] + day_of_month[2] +day_of_month[3] + +day_of_month[4] + day_of_month[5],
	day_of_month[0] + day_of_month[1] + day_of_month[2] +day_of_month[3] + +day_of_month[4] + day_of_month[5] + day_of_month[6],
	day_of_month[0] + day_of_month[1] + day_of_month[2] +day_of_month[3] + +day_of_month[4] + day_of_month[5] + day_of_month[6] + day_of_month[7],
	day_of_month[0] + day_of_month[1] + day_of_month[2] +day_of_month[3] + +day_of_month[4] + day_of_month[5] + day_of_month[6] + day_of_month[7] + day_of_month[8] ,
	day_of_month[0] + day_of_month[1] + day_of_month[2] +day_of_month[3] + +day_of_month[4] + day_of_month[5] + day_of_month[6] + day_of_month[7] + day_of_month[8] + day_of_month[9],
	day_of_month[0] + day_of_month[1] + day_of_month[2] +day_of_month[3] + +day_of_month[4] + day_of_month[5] + day_of_month[6] + day_of_month[7] + day_of_month[8] + day_of_month[9] + day_of_month[10],
};

