
// ISPDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "TstatFlashDlg.h"
#include "ConfigFileHandler.h"
#include "StaticEx\staticex.h"

enum NC_FLASH_TYPE
{
	NC_FLASH_FIRMWARE = 0,
	NC_FLASH_BOOTLOADER = 1,
};
     	/*
		1:TStat-by com 
		2.NC 
		3.TStat By NC 
		4.LC-Main Board 
		5.LC-input Board
		6.LC-output Board
		*/
#define FLASH_TSTAT_COM 1
#define FLASH_NC_LC_NET 2
#define FLASH_SUBID_NET 3

 
class CComWriter;
class TFTPServer;
// CISPDlg dialog
class CISPDlg : public CDialog ,public CFlashBase
{
// Construction
public:
	CISPDlg(CWnd* pParent = NULL);	// standard constructor
	  virtual ~CISPDlg();
// Dialog Data
	enum { IDD = IDD_ISP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DWORD GetIPAddress();
	int Judge_BinHexFile(CString filepath);
public:
	//选择flash文件
	afx_msg void OnBnClickedButtonSelfile();
	//flash button click
	afx_msg void OnBnClickedButtonFlash();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedCheckSelcom();
	afx_msg void OnBnClickedCheckSelnet();
	afx_msg LRESULT OnAddStatusInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReplaceStatusInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFlashFinish(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonFindnc();
	afx_msg void OnClose();
	afx_msg void OnBnClickedRadioFlhfirmware();
	afx_msg void OnBnClickedRadioFlhbtldr();
	afx_msg LRESULT Show_Flash_DeviceInfor(WPARAM wParam, LPARAM lParam);
	 
	DECLARE_MESSAGE_MAP()
    
public:
	//////////////////////////////////////////////////////////////////////////
	// public method
	////////////////////////////////////////////////////////////////////////////
	// 参数 BOOL, =TRUE replace the current line, =FALSE add a new line

	void UpdateStatusInfo(const CString& strInfo, BOOL bReplace);
	 
	void SetFlashFileName(const CString& strFileName);
	CString GetFlashFileName();

	void EnableFlash(BOOL bEnable);

	int GetCurSelTabPage();
	CString GetCurSelPageStr();

	 
	BOOL Show_Flash_DeviceInfor_NET();
	CString Get_NET_Infor(CString strIPAdress,short nPort);
	//void SetCurTabSel(int nSel);

	//////////////////////////////////////////////////////////////////////////
	// for flash sn
	//BOOL GetFlashSNParam(int& nHWVerison, CString& strProductModel);
BOOL	GetFlashSNParam(int& nHWVerison, CString& strProductModel,int &nProductID);
	void OnTestPing(const CString& strIP);
	//According to Model Name,return the hex file prefix of the Device Model Name
	CString GetFilePrefix_FromDB(const CString& ModeName);
	int   Judge_Model_Version();
   void FlashSN();
public: 
	// public member
	//CIPAddressCtrl m_IPAddr;
	/*提示信息控件*/
	CToolTipCtrl *m_ToolTip;
 
public:	// private method
	/*void InitTabCtrl();*/
	// initialize some control by read config file.
 
	// show splash window for several second 
	void ShowSplashWnd(int nMillisecond);
	// write parameter to config file
	void SaveParamToConfigFile();

	void GetFlashParameter();

	/*void InitISPUI();*/
  BOOL  DetectBraudrate ()    ;
	

	void SaveFlashSNParamToFile();
	unsigned int  Judge_Flash_Type();
	 CString GetFileName_FromFilePath();
	/*
	Author Alex
	Date:2012-10-25
	For TSTAT
	*/
	BOOL			FileValidation(const CString& strFileName);
	BOOL			ValidMdbIDString();
	void			FlashByCom();
	int				GetModbusID(vector<int>& szMdbIDs);
	int				GetComPortNo();

	void FlashByEthernet();
	//

protected:	// private member

	//int					m_nFlashTypeFlag;			// =0 flash by com;  =1 flash by ethernet;
	//CString			m_strInstallPath;				// 老毛要的，其实没用
	CString			m_strFlashMethod;			// flash的方式
	CString			m_strLogoFileName;			// logo文件名，包含路径
	CString 		m_strLogFileName;           //log file name
	vector<int>		m_szMdbIDs;					// 记录所有输入的Modbus ID
	CListBox			m_lbStatusInfo;
	NC_FLASH_TYPE		m_ftFlashType;
	CComWriter*	m_pComWriter;				// 用串口flash的类的指针，在使用时才实例化，用完后立即释放
	
	TFTPServer*	m_pTFTPServer;				// 使用网络，TFTP协议flash，使用时实例化
	CComWriter*		m_pTCPFlasher;//使用网络接口来flash subid
	
public:
	//CTabCtrl					m_tab;
	CString					m_strHexFileName;			// hex文件名，包含路径，要烧录的文件，实际上也可能是bin文件
	CConfigFileHandler		m_cfgFileHandler;	
	CString					m_strCfgFilePath;				// cfg配置文件名，包含路径

	int							m_nTabSel;
	
	BOOL						m_bShowSN;					// 是否显示隐藏的界面。
    BOOL                         m_enable_sn_mac;
    
	map<int, CString>	m_mapModel;

	CString					m_strPingIP;	
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	 
	afx_msg void OnBnClickedCom();
	afx_msg void OnBnClickedNet();
	void COM_NET_Set_ReadOnly();
	//是否点击了刷新子ID的按钮，点击了，标志位TRUE
	void set_SUBID();
	//void Set_Device_FHInfor();	//FirmVersion,HardVersion
    CString m_strASIX;
    CString m_strProductName;
private:
	CEdit id;
	CComboBox m_ComPort;
	CIPAddressCtrl m_ipAddress;
	CEdit m_ipPort;
	CButton m_Check_SubID;
	CEdit m_SubID;
	// 文件缓冲区，用来存放读取的文件的内容，flash完后应当delete，使用时再new
	char*					m_pFileBuf;	//FOR SUBID					
	    //这个变量来控制控件是否可用的判断条件 
	    //COM_INPUT=TRUE 那就是选择COM的状态
		BOOL 						COM_INPUT;
		//这个变量是控制 通过IP刷新连到NC或者LC下面的设备的
		//TRUE的话，用户需要输入
		BOOL						FLASH_SUBID;
	   	/*	 Flash_Type
		1:TStat-by com 
		2.NC 
		3.TStat By NC 
		4.LC-Main Board 
		5.LC-input Board
		6.LC-output Board
		*/
		unsigned int  Flash_Type;	 

	 	//为写入日志文件  系统文件类
		CStdioFile*		 m_plogFile;
		//这个变量是数据库的路径
		CString	m_strDatabasefilepath;


		UINT m_ProductModel;
		float m_softwareRev;
		CString m_ProductName;
		CString m_ChipName;
		UINT m_ChipSize;

public:
	//点击click-box 的空间的触发事件
	afx_msg void OnBnClickedCheckFlashSubid();
	//初始化Combox控件
	void InitCombox(void);
	//这个函数为COM_TStat的总函数
	//当用户点击COM_FLASH的时候，
	BOOL FlashTstat(void);
	BOOL FlashNC_LC(void);
	BOOL FlashSubID(void);
	void OnFlashSubID(void);
	BOOL ValidMdbIDStringSUBID();
	int	  GetModbusIDSUBID (vector<int>& szMdbIDs);
	CString m_ID; //initial the ID text
	afx_msg void OnBnClickedButtonPing2();
private:
	CButton m_Btn_ping;

public:
	CString m_ModelName;
	CString m_FirmVer;
	CString m_HardVer;
	 
    void ShowProductNameFromIni();
	short m_IPPort;
    void InitISPUI();
    void initFlashSN();
	afx_msg void OnMainClear();
	afx_msg void OnSaveLogInfo();
	afx_msg void OnBnClickedClearLog();
	afx_msg void OnBnClickedSaveLog();
	afx_msg void OnLbnSelchangeListInfo();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	afx_msg void OnMenuApp();
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuSetting();

	afx_msg void OnMenuCheckhex();
	void CheckAutoFlash();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    CComboBox m_combox_baudrate;
    afx_msg void OnMenuFlashsn();


	BOOL m_FlashEnable;
	int m_isRAM;
	int m_FlashTimes;
	int m_Brandrate;
    afx_msg void OnBnClickedButtonFlashSn();
    CStaticEx m_static_info;
    static DWORD WINAPI  SN_MAC_Threadfun(LPVOID lpVoid);
    afx_msg LRESULT Fresh_CloseSN_fcuntion(WPARAM wParam, LPARAM lParam);
};
