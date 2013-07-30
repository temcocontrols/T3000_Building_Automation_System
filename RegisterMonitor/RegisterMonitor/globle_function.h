#ifndef _GLOBLE_FUNCTION_H
#define _GLOBLE_FUNCTION_H
#include <vector>
using namespace std;




int read_one(unsigned char device_var,unsigned short address,int retry_times=3);
int write_one(unsigned char device_var,unsigned short address,short value,int retry_times=3);
int Write_Multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times=3);
int Read_Multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length,int retry_times=3);
int read_one_org(unsigned char device_var,unsigned short address,int retry_times=3);
int write_one_org(unsigned char device_var,unsigned short address,short value,int retry_times=3);
int Write_Multi_org(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times=3);
//int Read_Multi_org(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length,int retry_times=3);

int Write_Multi_Block(unsigned char device_var,unsigned short *to_write,unsigned short start_address,int length,int retry_times=3);
 //int write_multi_block(unsigned char device_var,unsigned short *to_write,unsigned short  start_address,int length);

int turn_hex_str_to_ten_num(char *str);
unsigned short turn_4_hex_char_to_unsigned_short(char *source);
bool turn_hex_file_line_to_unsigned_char(char *str);
int turn_hex_char_to_int(char c);
void turn_int_to_unsigned_char(char *source,int length_source,unsigned char *aim);
void turn_unsigned_short_to_hex_char(char *order,unsigned short source);
char turn_unsigned_to_char(unsigned short source);
void delete_folder(CString DirName);
CString get_units_from_now_range(int the_tstat_id,int analog1_or_analog2);//for picturetstatmanager 

bool judgement_file_or_path_exist(CString path);
void clear_a_char_array(char *p,char t='\0');
float get_tstat_version(unsigned short tstat_id);

float get_curtstat_version();

int make_sure_isp_mode(int the_tstat_id);
bool get_serialnumber(long & serial,int the_id_of_product=-1);
void message_box_function(CString content,CString title,UINT Utype=MB_OK);
void button_load_bitmap(CButton &p_wnd,CString name);
CString get_unit_a_str(CString have_unit_str);
bool is_a_float_str(CString str);
bool multi_read_tstat(int id);

//
void SetPaneCommunicationPrompt(CString strComInfo);
void SetPaneString(int nIndext,CString str);

bool Get_SerialNumberA(long & serial,int the_id_of_product);
UINT get_serialnumber();
bool can_be_writed_hex_file(int product_model,int hex_file_product_model);
CString get_product_name_by_product_model(int product_model);

//CString GetTempUnit(int analog1_or_analog2=-1);
CString GetTempUnit(int nRange=-1,int nPIDNO = 0);
CString get_product_class_name_by_model_ID(int nModelID);



// 获得本地串口号
BOOL GetSerialComPortNumber(vector<CString>& szComm);
BOOL GetSerialComPortNumber1(vector<CString>& szComm);




#define FOR_DATABASE_CONNECT					_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")
#define USER_DATABASE_PASSWORD					_T(";Jet OLEDB:Database Password=alancat")
//Persist Security Info=False;
#define NO_APPLICATION _T("N/A")
#define Modbus_Serial	0
#define Modbus_TCP		1
#define	default_com1_port_baudrate	19200

#define COLOR_CELL RGB(226,238,251)
#define DISABLE_COLOR_CELL RGB(140,140,140)
#define SETPOINT_SPECIAL_VERSION	50
#define MsgT3000ViewFresh 5000

//the product model of product declare
//#define NET_WORK_CONT_PRODUCT_MODEL					100
#define	LED_PRODUCT_MODEL							25
#define TSTAT_PRODUCT_MODEL							19
//#define SOLAR_PRODUCT_MODEL							30		//SOLOR
#define PWM_TRANSDUCER_PRODUCT_MODEL				31
//#define T3_8IOD_PRODUCT_MODEL						20		//T3-8IOD
//#define T3_8IOA_PRODUCT_MODEL						21		//T3-8IOA
#define	T3_32I_PRODUCT_MODEL						22		//T3-32I
#define T3_8I_16O_PRODUCT_MODEL						23		//T3-8I-16O
//#define ZIGBEE_PRODUCT_MODEL						24		
#define NET_WORK_OR485_PRODUCT_MODEL				102
// #define CMFIVE_PRODUCT_MODEL                        50      //CM5
// #define MINIPANEL_MODEL								35		//Mini Panel
#define T3_4AO_PRODUCT_MODEL						28		//T3-4AO


#define WM_USER_FRESHSTATUSBAR						10000
#define WM_MYMSG_REFRESHBUILDING					80001


//scan


//Database operation

#define DB_SELECT_FOUND							VARIANT_FALSE==m_pRs->EndOfFile
#define DB_SELECT_NO_FOUND						VARIANT_FALSE!=m_pRs->EndOfFile
#define CLOSE_DATABASE								if(m_pRs->State) \
	m_pRs->Close(); \
	if(m_pCon->State) \
	m_pCon->Close(); 



#define LOAD_FAILURE_ONE							_T("Section1:FAN VALVE ")
#define LOAD_FAILURE_TWO							_T("Section2:VALVE SETTING ")
#define LOAD_FAILURE_THREE							_T("Section3:DELAY TIMES ")
#define LOAD_FAILURE_FOUR							_T("Section4:LOOKUP TABLE ")
#define LOAD_FAILURE_FIVE							_T("Section5:UNIVERSAL RELAY OUTPUTS ")
#define LOAD_FAILURE_SIX							_T("Section6:UNIVERSAL VALVE ")
#define LOAD_FAILURE_SEVEN							_T("Section7:VARIABLE ")
#define STEP_FAILURE								_T(" Error\r\n")
#define STEP_FINISH									_T(" Success\r\n")
// CBuildTable1 对话框
// Address 219
#define MODBUS_TABLE1_ZERO  219
#define	MODBUS_TABLE1_ZERO_HI  220              // 1     254    the range from reg 20 to reg 41.A changable look up table for "custom" sensor 
#define	MODBUS_TABLE1_HALFONE  221   
#define	MODBUS_TABLE1_HALFONE_HI 222               //              the range of input voltage is 0----5v and the range is divided 10 equal partions,at 0.5v interval
#define	MODBUS_TABLE1_ONE       223
#define	MODBUS_TABLE1_ONE_HI    224             //              the value's unit in the table is different according to different sensor
#define	MODBUS_TABLE1_HALFTWO   225
#define	MODBUS_TABLE1_HALFTWO_HI 226             
#define	MODBUS_TABLE1_TWO       227
#define	MODBUS_TABLE1_TWO_HI    228
#define	MODBUS_TABLE1_HALFTHREE 229
#define	MODBUS_TABLE1_HALFTHREE_HI 230
#define	MODBUS_TABLE1_THREE     231
#define	MODBUS_TABLE1_THREE_HI 232
#define	MODBUS_TABLE1_HALFFOUR 233
#define	MODBUS_TABLE1_HALFFOUR_HI 234
#define	MODBUS_TABLE1_FOUR     235
#define	MODBUS_TABLE1_FOUR_HI  236
#define	MODBUS_TABLE1_HALFFIVE 237
#define	MODBUS_TABLE1_HALFFIVE_HI 238
#define	MODBUS_TABLE1_FIVE 239
#define	MODBUS_TABLE1_FIVE_HI 240

#define MODBUS_TABLE2_ZERO        314  
#define	MODBUS_TABLE2_ZERO_HI     315                   // 1     254    the range from reg 20 to reg 41.A changable look up table for "custom" sensor 
#define	MODBUS_TABLE2_HALFONE     316
#define	MODBUS_TABLE2_HALFONE_HI  317             //              the range of input voltage is 0----5v and the range is divided 10 equal partions,at 0.5v interval
#define	MODBUS_TABLE2_ONE         318
#define	MODBUS_TABLE2_ONE_HI      319                   //              the value's unit in the table is different according to different sensor
#define	MODBUS_TABLE2_HALFTWO     320
#define	MODBUS_TABLE2_HALFTWO_HI  321          
#define	MODBUS_TABLE2_TWO         322 
#define	MODBUS_TABLE2_TWO_HI      323
#define	MODBUS_TABLE2_HALFTHREE   324
#define	MODBUS_TABLE2_HALFTHREE_HI 325
#define	MODBUS_TABLE2_THREE        326
#define	MODBUS_TABLE2_THREE_HI     327
#define	MODBUS_TABLE2_HALFFOUR     328
#define	MODBUS_TABLE2_HALFFOUR_HI  329
#define	MODBUS_TABLE2_FOUR         330
#define	MODBUS_TABLE2_FOUR_HI      331
#define	MODBUS_TABLE2_HALFFIVE     332
#define	MODBUS_TABLE2_HALFFIVE_HI  333
#define	MODBUS_TABLE2_FIVE         334
#define	MODBUS_TABLE2_FIVE_HI      335










#endif