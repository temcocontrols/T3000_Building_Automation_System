
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
	//flash
	afx_msg void OnBnClickedButtonSelfile();
	//flash button click
	afx_msg void OnBnClickedButtonFlash();
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedCheckSelcom();
	afx_msg void OnBnClickedCheckSelnet();
	afx_msg LRESULT OnAddStatusInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReplaceStatusInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFlashFinish(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnFlashBoot_Update_boot(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnFlashNewBootFinish(WPARAM wParam, LPARAM lParam);
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
	//  BOOL, =TRUE replace the current line, =FALSE add a new line

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
	/**/
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
	//CString			m_strInstallPath;				// 
	CString			m_strFlashMethod;			// flash
	CString			m_strLogoFileName;			// logo
	CString 		m_strLogFileName;           //log file name
	vector<int>		m_szMdbIDs;					// Modbus ID
	CListBox			m_lbStatusInfo;
	NC_FLASH_TYPE		m_ftFlashType;
	CComWriter*	m_pComWriter;				// flash
	
	TFTPServer*	m_pTFTPServer;				// TFTPflash
	CComWriter*		m_pTCPFlasher;//flash subid
	
public:
	//CTabCtrl					m_tab;
	CString					m_strHexFileName;			// hexbin
	CConfigFileHandler		m_cfgFileHandler;	
	CString					m_strCfgFilePath;				// cfg

	int							m_nTabSel;
	
	BOOL						m_bShowSN;					// 
    BOOL                         m_enable_sn_mac;
    
	map<int, CString>	m_mapModel;

	CString					m_strPingIP;	
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	 
	afx_msg void OnBnClickedCom();
	afx_msg void OnBnClickedNet();
	void COM_NET_Set_ReadOnly();
	//IDTRUE
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
	// flashdeletenew
	char*					m_pFileBuf;	//FOR SUBID					
	    // 
	    //COM_INPUT=TRUE COM
		BOOL 						COM_INPUT;
		// IPNCLC
		//TRUE
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

	 	//  
		CStdioFile*		 m_plogFile;
		//
		CString	m_strDatabasefilepath;


		UINT m_ProductModel;
		float m_softwareRev;
		CString m_ProductName;
		CString m_ChipName;
		UINT m_ChipSize;

public:
	//click-box 
	afx_msg void OnBnClickedCheckFlashSubid();
	//Combox
	void InitCombox(void);
	//COM_TStat
	//COM_FLASH
	BOOL FlashByComport(void);
	BOOL FlashByNetwork(void);
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

	//afx_msg void OnMenuCheckhex();
	void CheckAutoFlash();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    CComboBox m_combox_baudrate;



	BOOL m_FlashEnable;
	int m_FlashTimes;
	int m_Brandrate;
    afx_msg void OnBnClickedButtonFlashSn();
    CStaticEx m_static_info;
    static DWORD WINAPI  SN_MAC_Threadfun(LPVOID lpVoid);
    afx_msg LRESULT Fresh_CloseSN_fcuntion(WPARAM wParam, LPARAM lParam);
    afx_msg void OnCbnSelchangeComboPm();
    void Enable_Sensor_Window(bool nenable);
    afx_msg void OnBnClickedCheckNoItem();
#ifdef ISP_BURNING_MODE
	afx_msg void OnBnClickedButtonLoopFlash();
	static DWORD WINAPI  Thread_BurningTest_fun(LPVOID lpVoid);
#endif
};
