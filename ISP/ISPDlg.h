
// ISPDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
//#include "TstatFlashDlg.h"
#include "ConfigFileHandler.h"
//#include "LightCtrlFlashDlg.h"
//#include "NCFlashDlg.h"
#include "FlashBase.h"
#include "Resource.h"
//#define  _declspec(dllexport)  EXPPORT_CLASS
//#define  _declspec(dllimport)   IMPORT_CLASS
extern "C" __declspec(dllexport) void  show_ISPDlg();



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
#define FLASH_ONLY_SN 4


class CComWriter;
class TFTPServer;


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
public:
	//ѡ��flash�ļ�
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
	
	//afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	//////////////////////////////////////////////////////////////////////////
	// public method
	////////////////////////////////////////////////////////////////////////////
	// ���� BOOL, =TRUE replace the current line, =FALSE add a new line
	void UpdateStatusInfo(const CString& strInfo, BOOL bReplace);

	void SetFlashFileName(const CString& strFileName);
	CString GetFlashFileName();

	void EnableFlash(BOOL bEnable);

	int GetCurSelTabPage();
	CString GetCurSelPageStr();

	void Show_Flash_DeviceInfor(int ID);
	void Show_Flash_DeviceInfor_NET();
	CString Get_NET_Infor(CString strIPAdress,short nPort);
	//void SetCurTabSel(int nSel);

	//////////////////////////////////////////////////////////////////////////
	// for flash sn
	BOOL GetFlashSNParam(int& nHWVerison, CString& strProductModel);
	
	void OnTestPing(const CString& strIP);
	//According to Model Name,return the hex file prefix of the Device Model Name
	CString GetFilePrefix_FromDB(const CString& ModeName);
	int   Judge_Model_Version();
public: 
	// public member
	//CIPAddressCtrl m_IPAddr;
	/*��ʾ��Ϣ�ؼ�*/
	CToolTipCtrl *m_ToolTip;
 
protected:	// private method
	/*void InitTabCtrl();*/
	// initialize some control by read config file.
 
	// show splash window for several second 
	void ShowSplashWnd(int nMillisecond);
	// write parameter to config file
	void SaveParamToConfigFile();

	void GetFlashParameter();

	/*void InitISPUI();*/

	

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
	//CString			m_strInstallPath;				// ��ëҪ�ģ���ʵû��
	CString			m_strFlashMethod;			// flash�ķ�ʽ
	CString			m_strLogoFileName;			// logo�ļ���������·��
	CString 		m_strLogFileName;           //log file name
	vector<int>		m_szMdbIDs;					// ��¼���������Modbus ID
	CListBox			m_lbStatusInfo;
	NC_FLASH_TYPE		m_ftFlashType;
	CComWriter*	m_pComWriter;				// �ô���flash�����ָ�룬��ʹ��ʱ��ʵ����������������ͷ�
	
	TFTPServer*	m_pTFTPServer;				// ʹ�����磬TFTPЭ��flash��ʹ��ʱʵ����
	CComWriter*		m_pTCPFlasher;//ʹ������ӿ���flash subid
	//CTstatFlashDlg			m_DlgTstatFlash;
	//CNCFlashDlg			m_DlgNCFlash;
	//CLightCtrlFlashDlg		m_DlgLightCtrlFlash;
public:
	//CTabCtrl					m_tab;
	CString					m_strHexFileName;			// hex�ļ���������·����Ҫ��¼���ļ���ʵ����Ҳ������bin�ļ�
	CConfigFileHandler		m_cfgFileHandler;	
	CString					m_strCfgFilePath;				// cfg�����ļ���������·��

	int							m_nTabSel;
	
	BOOL						m_bShowSN;					// �Ƿ���ʾ���صĽ��档


	map<int, CString>	m_mapModel;

	CString					m_strPingIP;	
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	 
	afx_msg void OnBnClickedCom();
	afx_msg void OnBnClickedNet();
	void COM_NET_Set_ReadOnly();
	//�Ƿ�����ˢ����ID�İ�ť������ˣ���־λTRUE
	void set_SUBID();
	//void Set_Device_FHInfor();	//FirmVersion,HardVersion

private:
	CEdit id;
	CComboBox m_ComPort;
	CIPAddressCtrl m_ipAddress;
	CEdit m_ipPort;
	CButton m_Check_SubID;
	CEdit m_SubID;
	// �ļ���������������Ŷ�ȡ���ļ������ݣ�flash���Ӧ��delete��ʹ��ʱ��new
	char*					m_pFileBuf;	//FOR SUBID					
	    //������������ƿؼ��Ƿ���õ��ж����� 
	    //COM_INPUT=TRUE �Ǿ���ѡ��COM��״̬
		BOOL 						COM_INPUT;
		//��������ǿ��� ͨ��IPˢ������NC����LC������豸��
		//TRUE�Ļ����û���Ҫ����
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

	 	//Ϊд����־�ļ�  ϵͳ�ļ���
		CStdioFile*		 m_plogFile;
		//������������ݿ��·��
		CString	m_strDatabasefilepath;
public:
	//���click-box �Ŀռ�Ĵ����¼�
	afx_msg void OnBnClickedCheckFlashSubid();
	//��ʼ��Combox�ؼ�
	void InitCombox(void);
	//�������ΪCOM_TStat���ܺ���
	//���û����COM_FLASH��ʱ��
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
	 
 
	short m_IPPort;
	 
	afx_msg void OnMainClear();
	afx_msg void OnSaveLogInfo();
	afx_msg void OnBnClickedClearLog();
	afx_msg void OnBnClickedSaveLog();
	afx_msg void OnLbnSelchangeListInfo();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	afx_msg void OnMenuApp();
	afx_msg void OnMenuAbout();
};
