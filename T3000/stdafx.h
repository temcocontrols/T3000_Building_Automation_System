
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define _BIND_TO_CURRENT_VCLIBS_VERSION 1


#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#include <afxsock.h>            // MFC socket extensions
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
#include <GdiPlus.h>
using namespace Gdiplus;
#pragma  comment(lib,"GdiPlus.lib")

#pragma  comment(lib,"HtmlHelp.lib")
#pragma  comment(lib,"Iphlpapi.lib")

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","EndOfFile") rename("BOF","FirstOfFile")
//**********************************link to dll*********************
#define INPUT extern "C" __declspec(dllimport)

#pragma comment(lib, "WS2_32")
#pragma comment(lib,"ModbusDllforVc")
#pragma comment(lib,"FlexSlideBar")
#pragma comment(lib,"ISP")
 
INPUT void  show_ISPDlg();
INPUT int Write_One(unsigned char device_var,unsigned short address,unsigned short value);
INPUT int Read_One(unsigned char device_var,unsigned short address);
INPUT int write_multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length);
INPUT int read_multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length);
//INPUT bool open_com(unsigned char m_com);
INPUT bool open_com(int m_com);
INPUT void close_com();
INPUT bool is_connect();
INPUT int CheckTstatOnline(unsigned char devLo=1,unsigned char devHi=254);
INPUT bool Change_BaudRate(unsigned short new_baurate);
INPUT bool SetComm_Timeouts(LPCOMMTIMEOUTS lpCommTimeouts);
INPUT void SetComnicationHandle(int nType,HANDLE hCommunication);
INPUT bool Open_Socket(CString strIPAdress);
INPUT void SetCommunicationType(int nType);
INPUT int NetController_CheckTstatOnline(unsigned char devLo=1,unsigned char devHi=254);
INPUT bool Open_Socket2(CString strIPAdress,short nPort);
INPUT HANDLE GetCommunicationHandle();
//////////////////////////////////////////////////////////////////////////
INPUT int NetController_CheckTstatOnline_a(unsigned char  devLo,unsigned char  devHi, bool bComm_Type);
INPUT int NetController_CheckTstatOnline2_a(unsigned char  devLo,unsigned char  devHi, bool bComm_Type);
INPUT int CheckTstatOnline_a(unsigned char  devLo,unsigned char devHi, bool bComm_Type);
INPUT int CheckTstatOnline2_a(unsigned char  devLo,unsigned char  devHi, bool bComm_Type);

INPUT int Read_One2(unsigned char  device_var,unsigned short  address, bool bComm_Type);
INPUT int Write_One2(unsigned char  device_var,unsigned short  address,unsigned short  value, bool bComm_Type);
//OUTPUT int write_multi(TS_UC device_var,TS_UC *to_write,TS_US start_address,int length);
INPUT int read_multi2(unsigned char device_var,unsigned short  *put_data_into_here,unsigned short  start_address,int length, bool bComm_Type);
INPUT void closefile();//scan
INPUT void writefile( CString strip,CString strport);//scan
INPUT void Createfile();//scan


//INPUT SOCKET GetSocketHandle();

#include "modbus.h"
#include <vector>  // STL vector header. There is no ".h"
#include <afxdhtml.h>
using namespace std;  // Ensure that the namespace is set to std

#define CUSTOM_TABLE_FLOAT_VERSION 50.1
#define SETPOINT_SPECIAL_VERSION	50

typedef struct _STATUSBARINFO
{
	int nIndex;
	CString strInfoText;
}status_info;
#include "fileRW.h"



//#define _DEBUG
//*********************************link to dll***************************


//#ifdef ISPDLG_H
//
//class _declspec(dllexport) CISPDlg: public CDialog ,public CFlashBase //导出类circle
//
//#else
//
//class _declspec(dllimport)CISPDlg: public CDialog ,public CFlashBase //导入类circle
//#endif
//{
//// Construction
//public:
//	CISPDlg(CWnd* pParent = NULL);	// standard constructor
//	  virtual ~CISPDlg();
//// Dialog Data
//	enum { IDD = IDD_ISP_DIALOG };
//
//	protected:
//	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
//
//
//// Implementation
//protected:
//	HICON m_hIcon;
//
//	// Generated message map functions
//	virtual BOOL OnInitDialog();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
//	afx_msg void OnPaint();
//	afx_msg HCURSOR OnQueryDragIcon();
//	DWORD GetIPAddress();
//public:
//	//选择flash文件
//	afx_msg void OnBnClickedButtonSelfile();
//	//flash button click
//	afx_msg void OnBnClickedButtonFlash();
//	afx_msg void OnBnClickedButtonCancel();
//	afx_msg void OnBnClickedCheckSelcom();
//	afx_msg void OnBnClickedCheckSelnet();
//	afx_msg LRESULT OnAddStatusInfo(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnReplaceStatusInfo(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnFlashFinish(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnBnClickedButtonFindnc();
//	afx_msg void OnClose();
//	afx_msg void OnBnClickedRadioFlhfirmware();
//	afx_msg void OnBnClickedRadioFlhbtldr();
//	
//	//afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
//	DECLARE_MESSAGE_MAP()
//
//public:
//	//////////////////////////////////////////////////////////////////////////
//	// public method
//	////////////////////////////////////////////////////////////////////////////
//	// 参数 BOOL, =TRUE replace the current line, =FALSE add a new line
//	void UpdateStatusInfo(const CString& strInfo, BOOL bReplace=FALSE);
//	CString GetIPFromHostName(CString& strHostName);
//	void SetFlashFileName(const CString& strFileName);
//	CString GetFlashFileName();
//
//	void EnableFlash(BOOL bEnable);
//
//	int GetCurSelTabPage();
//	CString GetCurSelPageStr();
//
//	void Show_Flash_DeviceInfor(int ID);
//	void Show_Flash_DeviceInfor_NET();
//	CString Get_NET_Infor(CString strIPAdress,short nPort);
//	//void SetCurTabSel(int nSel);
//
//	//////////////////////////////////////////////////////////////////////////
//	// for flash sn
//	BOOL GetFlashSNParam(int& nHWVerison, CString& strProductModel);
//	
//	void OnTestPing(const CString& strIP);
//	//According to Model Name,return the hex file prefix of the Device Model Name
//	CString GetFilePrefix_FromDB(const CString& ModeName);
//	int   Judge_Model_Version();
//public: 
//	// public member
//	//CIPAddressCtrl m_IPAddr;
//	/*提示信息控件*/
//	CToolTipCtrl *m_ToolTip;
// 
//protected:	// private method
//	/*void InitTabCtrl();*/
//	// initialize some control by read config file.
// 
//	// show splash window for several second 
//	void ShowSplashWnd(int nMillisecond);
//	// write parameter to config file
//	void SaveParamToConfigFile();
//
//	void GetFlashParameter();
//
//	/*void InitISPUI();*/
//
//	
//
//	void SaveFlashSNParamToFile();
//	unsigned int  Judge_Flash_Type();
//	 CString GetFileName_FromFilePath();
//	/*
//	Author Alex
//	Date:2012-10-25
//	For TSTAT
//	*/
//	BOOL			FileValidation(const CString& strFileName);
//	BOOL			ValidMdbIDString();
//	void			FlashByCom();
//	int				GetModbusID(vector<int>& szMdbIDs);
//	int				GetComPortNo();
//
//	void FlashByEthernet();
//	//
//	void FlashSN();
//	int m_serialNo;
//  void 	OnlyFlashsn();
//
//protected:	// private member
//
//	//int					m_nFlashTypeFlag;			// =0 flash by com;  =1 flash by ethernet;
//	//CString			m_strInstallPath;				// 老毛要的，其实没用
//	CString			m_strFlashMethod;			// flash的方式
//	CString			m_strLogoFileName;			// logo文件名，包含路径
//	CString 		m_strLogFileName;           //log file name
//	vector<int>		m_szMdbIDs;					// 记录所有输入的Modbus ID
//	CListBox			m_lbStatusInfo;
//	NC_FLASH_TYPE		m_ftFlashType;
//	CComWriter*	m_pComWriter;				// 用串口flash的类的指针，在使用时才实例化，用完后立即释放
//	
//	TFTPServer*	m_pTFTPServer;				// 使用网络，TFTP协议flash，使用时实例化
//	CComWriter*		m_pTCPFlasher;//使用网络接口来flash subid
//	//CTstatFlashDlg			m_DlgTstatFlash;
//	//CNCFlashDlg			m_DlgNCFlash;
//	//CLightCtrlFlashDlg		m_DlgLightCtrlFlash;
//public:
//	//CTabCtrl					m_tab;
//	CString					m_strHexFileName;			// hex文件名，包含路径，要烧录的文件，实际上也可能是bin文件
//	CConfigFileHandler		m_cfgFileHandler;	
//	CString					m_strCfgFilePath;				// cfg配置文件名，包含路径
//
//	int							m_nTabSel;
//	
//	BOOL						m_bShowSN;					// 是否显示隐藏的界面。
//
//	BOOL                        m_bFlashSN;
//	BOOL                        m_bOnlyFlashSN;
//	map<int, CString>	m_mapModel;
//
//	CString					m_strPingIP;	
//	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
//	 
//	afx_msg void OnBnClickedCom();
//	afx_msg void OnBnClickedNet();
//	void COM_NET_Set_ReadOnly();
//	//是否点击了刷新子ID的按钮，点击了，标志位TRUE
//	void set_SUBID();
//	//void Set_Device_FHInfor();	//FirmVersion,HardVersion
//    void InitISPUI();
//	void initFlashSN();
//private:
//	CEdit id;
//	CComboBox m_ComPort;
//	CIPAddressCtrl m_ipAddress;
//	CEdit m_ipPort;
//	CButton m_Check_SubID;
//	CEdit m_SubID;
//	// 文件缓冲区，用来存放读取的文件的内容，flash完后应当delete，使用时再new
//	char*					m_pFileBuf;	//FOR SUBID					
//	    //这个变量来控制控件是否可用的判断条件 
//	    //COM_INPUT=TRUE 那就是选择COM的状态
//		BOOL 						COM_INPUT;
//		//这个变量是控制 通过IP刷新连到NC或者LC下面的设备的
//		//TRUE的话，用户需要输入
//		BOOL						FLASH_SUBID;
//	   	/*	 Flash_Type
//		1:TStat-by com 
//		2.NC 
//		3.TStat By NC 
//		4.LC-Main Board 
//		5.LC-input Board
//		6.LC-output Board
//		*/
//		unsigned int  Flash_Type;	 
//
//	 	//为写入日志文件  系统文件类
//		CStdioFile*		 m_plogFile;
//		//这个变量是数据库的路径
//		CString	m_strDatabasefilepath;
//public:
//	//点击click-box 的空间的触发事件
//	afx_msg void OnBnClickedCheckFlashSubid();
//	//初始化Combox控件
//	void InitCombox(void);
//	//这个函数为COM_TStat的总函数
//	//当用户点击COM_FLASH的时候，
//	BOOL FlashTstat(void);
//	BOOL FlashNC_LC(void);
//	BOOL FlashSubID(void);
//	void OnFlashSubID(void);
//	BOOL ValidMdbIDStringSUBID();
//	int	  GetModbusIDSUBID (vector<int>& szMdbIDs);
//	CString m_ID; //initial the ID text
//	afx_msg void OnBnClickedButtonPing2();
//private:
//	CButton m_Btn_ping;
//
//public:
//	CString m_ModelName;
//	CString m_FirmVer;
//	CString m_HardVer;
//	 
// 
//	short m_IPPort;
//	 
//	afx_msg void OnMainClear();
//	afx_msg void OnSaveLogInfo();
//	afx_msg void OnBnClickedClearLog();
//	afx_msg void OnBnClickedSaveLog();
//	afx_msg void OnLbnSelchangeListInfo();
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
//
//	afx_msg void OnMenuApp();
//	afx_msg void OnMenuAbout();
//	CString m_website;
//	afx_msg void OnBnClickedRadio1();
//	afx_msg void OnBnClickedRadio2();
//	afx_msg void OnMenuVersion();
//};
