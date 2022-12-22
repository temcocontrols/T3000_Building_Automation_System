#ifndef _GLOBAL_FUNCTION_H
#define _GLOBAL_FUNCTION_H

#pragma region For_Bacnet
#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "T3000RegAddress.h"
#include "msflexgrid1.h"
#include "global_define.h"
#pragma endregion For_Bacnet


//typedef union
//{
//	Str_in_point  m_group_input_data;
//	Str_out_point m_group_output_data;
//	Str_variable_point m_group_variable_data;
//	Str_program_point m_group_program_data;
//	Str_weekly_routine_point m_group_schedual_data;
//	Str_annual_routine_point m_group_annual_data;
//	Str_controller_point  m_group_pid_data;
//	Str_monitor_point     m_monitor_data;
//}groupdata;

//typedef struct grouppoint
//{
//	groupdata data;
//}str_group_point;



typedef union
{
	Str_in_point  m_group_input_data;
	Str_out_point m_group_output_data;
	Str_variable_point m_group_variable_data;
	Str_program_point m_group_program_data;
	Str_weekly_routine_point m_group_schedual_data;
	Str_annual_routine_point m_group_annual_data;
	Str_controller_point  m_group_pid_data;
	Str_monitor_point     m_monitor_data;
}groupdata;

typedef struct grouppoint
{
	groupdata data;
}str_group_point;

typedef struct
{
	unsigned char ndataindex;
	int command_type;
}str_command_info;

typedef struct
{
	unsigned char npanel_id;
	int ninstance;
	int npanel_commad;
	int nstandard_command;
	int npoint_number;
}str_point_info;

enum Bacnet_Vector_Status
{
	STATUS_NOUSE = 0,
	STATUS_USE   = 1
};

typedef struct
{
	int flag; // 0 - nouse 
	int nindex;
	int operation_type; // 0 by panel id			1 by instance
	str_point_info str_info;
	int nstatus;
	int nreq_time; //last request time
	int nrec_time; //last updated time
	groupdata ret_data;
}bacnet_background_struct;



#include "global_variable_extern.h"
#define FLEXGRID_CELL_COLOR RGB(128,128,128) 
#define GREEN_COLOR RGB(0,255,0)
#define YELLOW_COLOR RGB(255,255,0)
#define ORANGE_COLOR RGB(255,165,0)

#include "global_variable_extern.h"

//#define USE_MOD_SHIFT_DF

#ifdef ENABLE_HTTP_FUCTION
#include "HttpApiDefine.h"
#include "..\BravocontrolAPI\BravocontrolAPI\HttpAPI\HttpAPI.h"
#endif



#include <afxinet.h>
BOOL CheckForUpdate(
	LPCTSTR szFtpServer,
	LPCTSTR szFtpUsername,
	LPCTSTR szFtpPassword,
	LPCTSTR szFtpFilename,
	LPCTSTR szCurrentVersion,
	LPTSTR szLastVersion );


void FLEX_GRID_PUT_STR(CMsflexgrid m_FlexGri,int row,int col,CString str);
int Set_Communication_Count(bool b_transmission,int bac_instanceid);
int modbus_read_one_value( 
				int& value, 
				unsigned char device_var,
				unsigned short address, 
				int retry_times );
int read_multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length);
int modbus_read_multi_value( 
		unsigned short *put_data_into_here,
		unsigned char device_var,
		unsigned short start_address,
		int length,
		int retry_times );

int read_one(unsigned char device_var,unsigned short address,int retry_times=3);
int g_CheckTstatOnline_a(unsigned char  devLo,unsigned char devHi, bool bComm_Type);
int g_CheckTstatOnline_nocritical(unsigned char  devLo, unsigned char devHi, bool bComm_Type,int ncomport);
int g_NetController_CheckTstatOnline_a(unsigned char  devLo,unsigned char  devHi, bool bComm_Type);
int g_MINI_CheckTstatOnline_a(unsigned char devLo,unsigned char devHi, bool bComm_Type,int NET_COM);
int write_one(unsigned char device_var,unsigned short address,short value,int retry_times=3);
int write_one_multy_thread(unsigned char device_var, unsigned short address, short value, int retry_times = 3, int nindex = 0);
int Write_Multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times=3);
int Write_Multi_short(unsigned char device_var,unsigned short *to_write,unsigned short start_address,int length,int retry_times=3);
int Read_Multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length,int retry_times=3);
int write_one_org(unsigned char device_var,unsigned short address,short value,int retry_times=3);
int Write_Multi_org(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times=3);
int Write_Multi_org_short(unsigned char device_var,unsigned short *to_write,unsigned short start_address,int length,int retry_times=3);
int turn_hex_str_to_ten_num(char *str);
bool turn_hex_file_line_to_unsigned_char(char *str);
int turn_hex_char_to_int(char c);
void turn_int_to_unsigned_char(char *source,int length_source,unsigned char *aim);

float get_tstat_version(unsigned short tstat_id);

float get_curtstat_version();



int handle_bacnet_to_modbus_data(char *npoint, int nlength);

//
void SetPaneString(int nIndext,CString str);

UINT get_serialnumber();


BOOL IS_Temco_Product(int product_model);
//CString GetTempUnit(int analog1_or_analog2=-1);
CString GetTempUnit(int nRange=-1,int nPIDNO = 0);


extern int Read_One_t(unsigned char device_var,unsigned short address, int slot);
extern int Write_One_t(unsigned char device_var, unsigned short address, unsigned short val, int slot);
extern int read_multi_t(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length, int slot);
extern int write_multi_t(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length, int slot);


BOOL GetSerialComPortNumber1(vector<CString>& szComm);

BOOL Post_Thread_Message(UINT MsgType,
	unsigned char device_id,
	unsigned short address,
	short new_value,
	short old_value,
	HWND Dlg_hwnd,
	UINT CTRL_ID,
	CString Changed_Name);


BOOL Post_Read_one_Thread_Message(
	unsigned char device_id,
	unsigned short address,
	HWND Dlg_hwnd);

BOOL Post_Invoke_ID_Monitor_Thread(UINT MsgType,
	int Invoke_ID,
	HWND hwnd,
	CString Show_Detail = _T(""),
	int nRow = 0,
	int nCol = 0
	);
BOOL Post_Refresh_One_Message(uint32_t deviceid,int8_t command,int8_t start_instance,int8_t end_instance,unsigned short entitysize);
BOOL Post_Refresh_Message(uint32_t deviceid,int8_t command,int8_t start_instance,int8_t end_instance,unsigned short entitysize,int block_size);
BOOL Post_Write_Message(uint32_t deviceid,int8_t command,int8_t start_instance,int8_t end_instance,unsigned short entitysize,HWND hWnd,CString Task_Info = _T(""),int nRow = 0,int nCol = 0);
int Post_Background_Read_Message_ByPanel(unsigned char panel_id, int command_type, int npoint);
int Post_Background_Write_Message_ByIndex(str_command_info ret_index, groupdata write_data);
int GetProgramData(uint32_t deviceid,uint8_t start_instance,uint8_t end_instance,uint8_t npackgae);
int GetProgramData_Blocking(uint32_t deviceid,uint8_t start_instance,uint8_t end_instance,uint8_t npackgae);
int GetPrivateData(uint32_t deviceid,uint8_t command,uint8_t start_instance,uint8_t end_instance,int16_t entitysize);
int GetPriavteDataByPanelBlocking(Str_points* npoint, str_group_point* temp_data, uint8_t retrytime = 5);
int GetPrivateDataSaveSPBlocking(uint32_t deviceid, uint8_t command, uint8_t start_instance, uint8_t end_instance, int16_t entitysize, uint8_t retrytime = 10);
int GetPrivateData_Blocking(uint32_t deviceid,uint8_t command,uint8_t start_instance,uint8_t end_instance,int16_t entitysize, uint8_t retrytime = 10);
int GetPrivateBacnetToModbusData(uint32_t deviceid, uint16_t start_reg, int16_t readlength, unsigned short *data_out);//Bacnet Э��ת��Ϊ modbus Э��;
int WritePrivateBacnetToModbusData(uint32_t deviceid, int16_t start_reg, uint16_t writelength, unsigned short *data_in);

int GetMonitorBlockData(uint32_t deviceid,int8_t command,int8_t nIndex,int8_t ntype_ad, uint32_t ntotal_seg, uint32_t nseg_index,MonitorUpdateData* up_data);
int WritePrivateData(uint32_t deviceid,unsigned char n_command,unsigned char start_instance,unsigned char end_instance, char* ext_data=NULL);
int WritePrivateData_Blocking(uint32_t deviceid, unsigned char n_command, unsigned char start_instance, unsigned char end_instance, uint8_t retrytime = 5,  char* ext_data=NULL);
int Write_Private_Data_Blocking(uint8_t ncommand, uint8_t nstart_index, uint8_t nstop_index, unsigned int write_object_list = 0);
int WriteProgramData(uint32_t deviceid,uint8_t n_command,uint8_t start_instance,uint8_t end_instance ,uint8_t npackage);
int WriteProgramData_Blocking(uint32_t deviceid,uint8_t n_command,uint8_t start_instance,uint8_t end_instance ,uint8_t npackage);
int Bacnet_PrivateData_Deal(char * bacnet_apud_point, uint32_t len_value_type, bool &end_flag, short invoke_id = -1);
int Bacnet_PrivateData_Handle(	BACNET_PRIVATE_TRANSFER_DATA * data,bool &end_flag , short invoke_id = -1);
bool Check_Label_Exsit(LPCTSTR m_new_label);
bool Check_FullLabel_Exsit(LPCTSTR m_new_fulllabel);
void local_handler_conf_private_trans_ack(
	uint8_t * service_request,
	uint16_t service_len,
	BACNET_ADDRESS * src,
	BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data);
void local_handler_update_bacnet_ui(int receive_data_type, bool each_end_flag);

void local_handler_read_property_multiple_ack(
    uint8_t * service_request,
    uint16_t service_len,
    BACNET_ADDRESS * src,
    BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data);

//int Bacnet_Read_Property_Multiple();
int Bacnet_Read_Property_Multiple(uint32_t deviceid, BACNET_OBJECT_TYPE object_type, uint32_t object_instance, int property_id); 
int Bacnet_Read_Properties_Multiple_Blocking(uint32_t deviceid, BACNET_OBJECT_TYPE object_type, uint32_t object_instance, int property_id, BACNET_READ_ACCESS_DATA &value, uint8_t retrytime, uint32_t index = BACNET_ARRAY_ALL);

int Bacnet_Read_Properties(uint32_t deviceid, BACNET_OBJECT_TYPE object_type, uint32_t object_instance, int property_id, uint32_t index = BACNET_ARRAY_ALL);
int Bacnet_Read_Properties_Blocking(uint32_t deviceid, BACNET_OBJECT_TYPE object_type, uint32_t object_instance, int property_id, BACNET_APPLICATION_DATA_VALUE &value, uint8_t retrytime = 3, uint32_t index = BACNET_ARRAY_ALL);
int Bacnet_Write_Properties(uint32_t deviceid, BACNET_OBJECT_TYPE object_type, uint32_t object_instance, int property_id, BACNET_APPLICATION_DATA_VALUE * object_value, uint8_t priority = 16,BACNET_READ_PROPERTY_DATA * objectData=NULL );
int Bacnet_Write_Properties_Blocking(uint32_t deviceid, BACNET_OBJECT_TYPE object_type, uint32_t object_instance, int property_id, BACNET_APPLICATION_DATA_VALUE * object_value, uint8_t priority = 16, uint8_t retrytime = 3);
void localhandler_read_property_ack(
    uint8_t * service_request,
    uint16_t service_len,
    BACNET_ADDRESS * src,
    BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data); //��׼�Ķ�����;

void LocalIAmHandler(	uint8_t * service_request,	uint16_t service_len,	BACNET_ADDRESS * src);

void LocalBacnetAbortHandler(BACNET_ADDRESS* src,uint8_t invoke_id,	uint8_t abort_reason,	bool server);
void LocalBacnetReadErrorHandler(BACNET_ADDRESS* src, uint8_t invoke_id, BACNET_ERROR_CLASS error_class, BACNET_ERROR_CODE error_code);
void LocalBacnetErrorHandler(BACNET_ADDRESS* src, uint8_t invoke_id, BACNET_ERROR_CLASS error_class,BACNET_ERROR_CODE error_code);
void Localhandler_write_property_ack(
	uint8_t* service_request,
	uint16_t service_len,
	BACNET_ADDRESS* src,
	BACNET_CONFIRMED_SERVICE_ACK_DATA* service_data);

void SplitCStringA(CStringArray &saArray, CString sSource, CString sToken);
char * intervaltotext(char *textbuf, long seconds , unsigned minutes , unsigned hours, char *c =":");
char * intervaltotextfull(char *textbuf, long seconds , unsigned minutes , unsigned hours,char *c =":");
char* intervaltotext_2022_full(char* textbuf, unsigned long var_value, char* c);
 DWORD WINAPI   MSTP_Receive(LPVOID lpVoid);
  void local_rp_ack_print_data(	BACNET_READ_PROPERTY_DATA * data);
  void local_value_rp_ack_print_data(BACNET_READ_PROPERTY_DATA * data, BACNET_APPLICATION_DATA_VALUE &value);
  void close_bac_com();
 bool Initial_bac(int comport = 0,CString bind_local_ip = _T(""),int n_baudrate = 19200);
  bool Open_bacnetSocket2(CString strIPAdress,unsigned short nPort,SOCKET &mysocket);
  unsigned char Str_to_Byte(CString need_conver);
  void Init_Service_Handlers(void);
CString GetProductName(int ModelID);
void Inial_Product_map();
void Inial_Product_Reglist_map();
void Inial_Product_Menu_map();
void Inial_Product_Input_map();
int Get_Product_Menu_Map(unsigned char product_tpye, int menu_item);
int Get_Product_Input_Map(unsigned char product_tpye, int input_item);
CString Get_Table_Name(int SerialNo,CString Type ,int Row);
void    Insert_Update_Table_Name(int SerialNo,CString Type,int Row,CString TableName); 
int Get_Unit_Process(CString Unit);
BOOL Get_Bit_FromRegister(unsigned short RegisterValue,unsigned short Position);
int AddNetDeviceForRefreshList(BYTE* buffer, int nBufLen,  sockaddr_in& siBind);
int AddSubNetInfoIntoRefreshList(BYTE* buffer);
UINT RefreshNetWorkDeviceListByUDPFunc();
//void DFTrace(CString &nCString);
void DFTrace(LPCTSTR lpCString);
bool GetFileNameFromPath(CString ncstring,CString &cs_return);
BOOL Ping(const CString& strIP, CWnd* pWndEcho);
 
void Send_WhoIs_remote_ip(CString ipaddress);
void ClearBacnetData(); //������ʼ��bacnet �ڴ�;
void SaveBacnetBinaryFile(CString &SaveConfigFilePath);

int SaveModbusConfigFile(CString &SaveConfigFilePath);
void SaveModbusConfigFile_Cache(CString &SaveConfigFilePath,char *npoint,unsigned int bufferlength);
int LoadBacnetBinaryFile(int write_to_device,LPCTSTR tem_read_path);
int LoadModbusConfigFile_Cache(LPCTSTR tem_read_path);
int GetDeviceCountTable(int device_serialnumber, int ntype, device_io_status &temp_device_io_status);
void Create_DeviceDatabase();
int UpdateDeviceCountTable(int device_serialnumber, int ntype, device_io_status temp_device_io_status);
int CheckDeviceCountTable(int device_serialnumber, int objectinstance = 0);
int LoadMiniModbusConfigFile(LPCTSTR tem_read_path);
//For MINIPanel ARM
int WriteDeviceDataIntoAccessDB(int nTableType, int ncount, int device_serialnumber);
#ifdef LOCAL_DB_FUNCTION
void init_product_list();
int CheckDeviceDataDBAndUpdateDB();
#endif
void Copy_Data_From_485_to_Bacnet(unsigned short *start_point);
int handle_read_monitordata_ex(char *npoint,int nlength);
int handle_read_pic_data_ex(char *npoint,int nlength);
bool IP_is_Local(LPCTSTR ip_address);
bool Support_relinquish_device(unsigned short n_product_class_id);
bool Bacnet_Private_Device(unsigned short n_product_class_id);
BOOL DirectoryExist(CString Path);
BOOL CreateDirectory(CString path);
BOOL DeleteDirectory(CString path);
int GetHostAdaptersInfo(CString &IP_address_local);
DWORD WinExecAndWait( LPCTSTR lpszAppPath,LPCTSTR lpParameters,LPCTSTR lpszDirectory, 	DWORD dwMilliseconds);
CString GetContentFromURL(CString URL);

BOOL KillProcessFromName(CString strProcessName) ;
BOOL CheckTheSameSubnet(CString strIP ,CString strIP2);

void Save_Product_Value_Cache(CString &SaveFilePath);
int Load_Product_Value_Cache(LPCTSTR tem_read_path);


CString GetExePath(bool bHasSlash);
void GetIPMaskGetWay();
void GetIPMaskGetWayForScan();
BOOL Is_Dig_Num(CString str);
BOOL ValidAddress(CString sAddress);

BOOL GetIPbyHostName(CString strHostName,CString &strIP);
void TraverseFolder( const CString& strDir,std::vector<CString>& vecFile);

void LoadTstat_InputData();
void LoadInputData_CS3000();
CString GetTextFromReg(unsigned short reg);
CString GetTextFromRegLength(unsigned short reg, int reg_nlength);
CString GetTextFromReg_Buffer(unsigned short reg,unsigned short *Buffer);
void LoadTstat_OutputData();
void LoadOutputData_CS3000();


void  LoadRegistersGraphicMode();
void  LoadRegistersGraphicMode_AQ();
void  LoadRegistersGraphicMode_HUMTEMPSENSOR();
void  LoadRegistersGraphicMode_CO2485();
BOOL  ReadLineFromHexFile(CFile& file, char* pBuffer);
int Get_Binfile_Information(LPCTSTR filepath,Bin_Info &ret_bin_Info);
int Get_HexFile_Information(LPCTSTR filepath,Bin_Info &ret_bin_Info);
int WriteBacnetPictureData(uint32_t deviceid,uint8_t index ,unsigned short transfer_packet, unsigned short total_packet,unsigned char * senddata);
int WriteBacnetPictureData_Blocking(uint32_t deviceid,uint8_t index , unsigned short transfer_packet, unsigned short total_packet,unsigned char * senddata);

int GetPictureBlockData(uint32_t deviceid,int8_t nIndex, uint16_t ntotal_seg,uint16_t nseg_index);
int GetPictureBlockData_Blocking(uint32_t deviceid,int8_t nIndex, uint16_t ntotal_seg,uint16_t nseg_index);
BOOL ShowBacnetView(unsigned char product_type);
BOOL HexFileValidation(const CString& strFileName);
BOOL BinFileValidation(const CString& strFileName);
BOOL AllCharactorIsDigital(LPCTSTR lpszSrc);



bool Save_InputData_to_db(unsigned char  temp_input_index);

bool Save_InputData_to_db(unsigned char  temp_input_index, unsigned int nserialnumber);
bool Save_OutputData_to_db(unsigned char  temp_output_index, unsigned int nserialnumber);
bool Save_VariableData_to_db(unsigned char  temp_output_index, unsigned int nserialnumber);
bool Save_OutputData_to_db(unsigned char  temp_output_index);
bool Save_AVData_to_db();
CString GetGUID();
int  SetCommandDelayTime(unsigned char product_id); //��������ÿ����Ʒ�� Ӧ����ʱʱ��.Ĭ��Ϊ100ms.
bool Open_Socket_Retry(CString strIPAdress, short nPort, int retry_time = 3);
void Inial_ProductName_map();  //��ʼ�� Panel Name ��ŵ�λ�� Ĭ����714λ�� 
bool bac_Invalid_range(unsigned char nrange);  //�ж�bacnet range �Ƿ��ں���ֵ��Χ��;
void Initial_Instance_Reg_Map();  //��ʼ�� instance ���λ��;
int PanelName_Map(int product_type);  //���� Panel  Name ��ŵ�λ�� û���ҵ�����Ϊ�Ǵ�714λ�ÿ�ʼ��
int Get_Instance_Reg_Map(int product_type, unsigned short &temp_high, unsigned short &temp_low);
unsigned int GetDeviceInstance(unsigned char pid_type);  //ͨ�� map �д�ŵ� instance �Ĵ��� ��ȡ ��Ӧ��Ʒ�� instance ֵ;
int ChangeDeviceProtocol(bool modbus_0_bacnet_1,   // 0  modbus           1  bacnet 
    unsigned char modbus_id,
    unsigned short nreg_address,
    unsigned short nreg_value,
    unsigned char sub_device,         // ��������豸  �����ݿ��е�Э�� �Ƚ�����;
    LPCTSTR Dbpath);
int ChangeModbusDB(unsigned int nserialnumber, int nmodbusid, LPCTSTR Dbpath);
void switch_product_last_view();
int Initial_Function();
int Check_Function(int product_id, unsigned char nprotocol, FunctionNumber function_number);//���ݲ�ƷЭ�����ʹ�ܹ���
int Get_Msv_Table_Name(int x);// ��ȡMSV�ı���������
int Get_Msv_Item_Name(int ntable, int nitemvalue, CString &csItemString); // ƥ��MSV ��Ӧ��ֵ��ʾ��һ�� ����
int Get_Msv_next_Name_and_Value_BySearchName(int ntable, CString nitemname, CString  &csNextItemString, int &nNextValue);
int Get_Msv_next_Name_and_Value_BySearchValue(int ntable, int nitemvalue, CString  &csNextItemString, int &nNextValue);
int Check_DaXiaoDuan(unsigned char npid, unsigned char Mainsw, unsigned char subsw);
void Time32toCString(unsigned long ntime, CString &outputtime, int nproduct_id = 74);
int GetOutputType(UCHAR nproductid, UCHAR nproductsubid, UCHAR portindex); //��ȡ���״̬
int GetInputType(UCHAR nproductid, UCHAR nproductsubid, UCHAR portindex, UCHAR n_digital_analog); //��ȡ���״̬
char* decode_point(char* token, Str_points& temp);
void Initial_All_Point();
void Initial_Virtual_Device_Setting();
int bacnet_set_read_result(int nret);
int CheckDeviceDatabase();
int GetInputLabelEx(Str_in_point temp_in, CString& ret_label, Point_Net* npoint);
int GetInputValueEx(Str_in_point temp_in, CString& ret_cstring, CString& ret_unit, CString& Auto_M, int& digital_value);
int GetInputFullLabelEx(Str_in_point temp_in, CString& ret_full_label, Point_Net* npoint);
int GetOutputLabelEx(Str_out_point temp_out, CString& ret_label, Point_Net* npoint);
int GetOutputFullLabelEx(Str_out_point temp_out, CString& ret_full_label, Point_Net* npoint);
int GetOutputValueEx(Str_out_point temp_out, CString& ret_cstring, CString& ret_unit, CString& Auto_M, int& digital_value);
int GetVariableLabelEx(Str_variable_point temp_var, CString& ret_label, Point_Net* npoint);
int GetVariableFullLabelEx(Str_variable_point temp_var, CString& ret_full_label, Point_Net* npoint);
int GetVariableValueEx(Str_variable_point temp_var, CString& ret_cstring, CString& ret_unit, CString& Auto_M, int& digital_value);

void InputDataToString(Str_in_point source_input, Input_CString* ret_string);
void OutputDataToString(Str_out_point source_output, Output_CString* ret_string);
void VariableDataToString(Str_variable_point source_variable, Variable_CString* ret_string);
#endif