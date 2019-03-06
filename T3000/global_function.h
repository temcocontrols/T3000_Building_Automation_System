#ifndef _GLOBAL_FUNCTION_H
#define _GLOBAL_FUNCTION_H

#pragma region For_Bacnet
#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "T3000RegAddress.h"
#include "msflexgrid1.h"
 
#pragma endregion For_Bacnet


#include "global_variable_extern.h"
#define FLEXGRID_CELL_COLOR RGB(128,128,128) 
#define GREEN_COLOR RGB(0,255,0)
#define YELLOW_COLOR RGB(255,255,0)
#define ORANGE_COLOR RGB(255,165,0)

#include "global_variable_extern.h"
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
int GetProgramData(uint32_t deviceid,uint8_t start_instance,uint8_t end_instance,uint8_t npackgae);
int GetProgramData_Blocking(uint32_t deviceid,uint8_t start_instance,uint8_t end_instance,uint8_t npackgae);
int GetPrivateData(uint32_t deviceid,uint8_t command,uint8_t start_instance,uint8_t end_instance,int16_t entitysize);
int GetPrivateData_Blocking(uint32_t deviceid,uint8_t command,uint8_t start_instance,uint8_t end_instance,int16_t entitysize, uint8_t retrytime = 10);
int GetPrivateBacnetToModbusData(uint32_t deviceid, uint16_t start_reg, int16_t readlength, unsigned short *data_out);//Bacnet 协议转换为 modbus 协议;
int WritePrivateBacnetToModbusData(uint32_t deviceid, int16_t start_reg, uint16_t writelength, unsigned short *data_in);

int GetMonitorBlockData(uint32_t deviceid,int8_t command,int8_t nIndex,int8_t ntype_ad, uint32_t ntotal_seg, uint32_t nseg_index,MonitorUpdateData* up_data);
int WritePrivateData(uint32_t deviceid,unsigned char n_command,unsigned char start_instance,unsigned char end_instance);
int WriteProgramData(uint32_t deviceid,uint8_t n_command,uint8_t start_instance,uint8_t end_instance ,uint8_t npackage);
int WriteProgramData_Blocking(uint32_t deviceid,uint8_t n_command,uint8_t start_instance,uint8_t end_instance ,uint8_t npackage);
int Write_Private_Data_Blocking(uint8_t ncommand,uint8_t nstart_index,uint8_t nstop_index,unsigned int write_object_list = 0);
int Bacnet_PrivateData_Handle(	BACNET_PRIVATE_TRANSFER_DATA * data,bool &end_flag);
bool Check_Label_Exsit(LPCTSTR m_new_label);
bool Check_FullLabel_Exsit(LPCTSTR m_new_fulllabel);
void local_handler_conf_private_trans_ack(
	uint8_t * service_request,
	uint16_t service_len,
	BACNET_ADDRESS * src,
	BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data);

void LocalIAmHandler(	uint8_t * service_request,	uint16_t service_len,	BACNET_ADDRESS * src);

void SplitCStringA(CStringArray &saArray, CString sSource, CString sToken);
char * intervaltotext(char *textbuf, long seconds , unsigned minutes , unsigned hours, char *c =":");
char * intervaltotextfull(char *textbuf, long seconds , unsigned minutes , unsigned hours,char *c =":");
 DWORD WINAPI   MSTP_Receive(LPVOID lpVoid);
 void Localhandler_read_property_ack(
	 uint8_t * service_request,
	 uint16_t service_len,
	 BACNET_ADDRESS * src,
	 BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data);
  void local_rp_ack_print_data(	BACNET_READ_PROPERTY_DATA * data);
  void close_bac_com();
 bool Initial_bac(int comport = 0,CString bind_local_ip = _T(""),int n_baudrate = 19200);
  bool Open_bacnetSocket2(CString strIPAdress,unsigned short nPort,SOCKET &mysocket);
  unsigned char Str_to_Byte(CString need_conver);
  void Init_Service_Handlers(void);
CString GetProductName(int ModelID);
void Inial_Product_map();
void Inial_Product_Reglist_map();
CString Get_Table_Name(int SerialNo,CString Type ,int Row);
void    Insert_Update_Table_Name(int SerialNo,CString Type,int Row,CString TableName); 
int Get_Unit_Process(CString Unit);
BOOL Get_Bit_FromRegister(unsigned short RegisterValue,unsigned short Position);
int AddNetDeviceForRefreshList(BYTE* buffer, int nBufLen,  sockaddr_in& siBind);
UINT RefreshNetWorkDeviceListByUDPFunc();
//void DFTrace(CString &nCString);
void DFTrace(LPCTSTR lpCString);
bool GetFileNameFromPath(CString ncstring,CString &cs_return);
BOOL Ping(const CString& strIP, CWnd* pWndEcho);
 
void Send_WhoIs_remote_ip(CString ipaddress);
void ClearBacnetData(); //用来初始化bacnet 内存;
void SaveBacnetBinaryFile(CString &SaveConfigFilePath);
void SaveBacnetConfigFile(CString &SaveConfigFilePath);
void SaveBacnetConfigFile_Cache(CString &SaveConfigFilePath);
int SaveModbusConfigFile(CString &SaveConfigFilePath);
void SaveModbusConfigFile_Cache(CString &SaveConfigFilePath,char *npoint,unsigned int bufferlength);
int LoadBacnetBinaryFile(bool write_to_device,LPCTSTR tem_read_path);
int LoadBacnetBinaryFile_Cache(LPCTSTR tem_read_path);
int LoadModbusConfigFile_Cache(LPCTSTR tem_read_path);

int LoadMiniModbusConfigFile(LPCTSTR tem_read_path);
//For MINIPanel ARM
int LoadMiniModbusConfigFile_MINIARM(LPCTSTR tem_read_path);
void Copy_Data_From_485_to_Bacnet(unsigned short *start_point);
int handle_read_monitordata_ex(char *npoint,int nlength);
int handle_read_pic_data_ex(char *npoint,int nlength);
bool IP_is_Local(LPCTSTR ip_address);
bool Is_Bacnet_Device(unsigned short n_product_class_id);
BOOL DirectoryExist(CString Path);
BOOL CreateDirectory(CString path);
BOOL DeleteDirectory(CString path);
int GetHostAdaptersInfo(CString &IP_address_local);
DWORD WinExecAndWait( LPCTSTR lpszAppPath,LPCTSTR lpParameters,LPCTSTR lpszDirectory, 	DWORD dwMilliseconds);
CString GetContentFromURL(CString URL);
CString GetProductFirmwareTimeFromTemcoWebsite(CString URL,CString HexOrBinName,CString &FileSize);
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


bool Input_data_to_string(unsigned char  temp_input_index,
    CString &temp_in_main_panel,
    CString &temp_in_des,
    CString &temp_in_auto_manual,
    CString &temp_in_value,
    CString &temp_in_units,
    CString &temp_in_range,
    CString &temp_in_cal,
    CString &temp_cacl_sign,
    CString &temp_in_filter,
    CString &temp_in_decon,
    CString &temp_in_jumper,
    CString &temp_in_label);
bool Save_InputData_to_db(unsigned char  temp_input_index);
bool Output_data_to_string(unsigned char  temp_output_index,
    CString &temp_out_panel,
    CString &temp_out_des,
    CString &temp_out_auto_manual,
    CString &temp_out_value,
    CString &temp_out_units,
    CString &temp_out_range,
    CString &temp_out_pwm_period,
    CString &temp_out_decom,
    CString &temp_out_label);
bool Save_InputData_to_db(unsigned char  temp_input_index, unsigned int nserialnumber);
bool Save_OutputData_to_db(unsigned char  temp_output_index, unsigned int nserialnumber);
bool Save_VariableData_to_db(unsigned char  temp_output_index, unsigned int nserialnumber);
bool Save_OutputData_to_db(unsigned char  temp_output_index);
bool Save_AVData_to_db();
CString GetGUID();

bool Open_Socket_Retry(CString strIPAdress, short nPort, int retry_time = 3);
void Inial_ProductName_map();  //初始化 Panel Name 存放的位置 默认是714位置 
int PanelName_Map(int product_type);  //查找 Panel  Name 存放的位置 没有找到就认为是从714位置开始的
void switch_product_last_view();
#endif