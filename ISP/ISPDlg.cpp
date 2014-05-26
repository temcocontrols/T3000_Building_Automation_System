
// ISPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ISP.h"
#include "ISPDlg.h"
#include "globle_function.h"
#include "JPGLoader.h"
#include "afxwin.h"
#include "Splash.h"
#include "HexFileParser.h"
#include "BinFileParser.h"
#include "ComWriter.h"
#include "TFTPServer.h"
#include "NCFinder.h"
#include "FlashSN.h"
#include "ISPSetting.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//int g_CommunicationType;

CString g_strExePath;
CString SettingPath;
HANDLE get_file_thread_handle = NULL;
CString g_strFlashInfo;
 const TCHAR c_strLogFileName[]=_T("Log_info.txt");				//log information file name
 const TCHAR c_strDBFileName[]=_T("Database\\t3000.mdb") ;
//extern CString g_strExePath;
const int BOOTLOADER_FILE_SIZE = 16384;
/*这个变量虽然和DLL中的变量相同，但是他们两个没有关系，DLL中的该变量是被封装起来了
如果想改变dll中的g_Commu_type 通过函数 SetCommunicationType*/
int g_Commu_type=0;
UINT _PingThread(LPVOID pParam);

unsigned int Remote_timeout = 1000;
DWORD WINAPI GetFWFileProc(LPVOID lPvoid);

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	virtual ~CAboutDlg(void);

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	int ShowLogo();
	JPGLoader* m_pPicLoader;
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
		afx_msg void OnPaint();
public:
	CStatic m_logoPic;
	//afx_msg void OnMainClear();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_pPicLoader = NULL;
}
CAboutDlg::~CAboutDlg()
{
	/*if (m_pPicLoader)
	{
		delete m_pPicLoader;
	}*/
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGOPIC, m_logoPic);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
		ON_WM_PAINT()
		//ON_COMMAND(ID_MAIN_CLEAR, &CAboutDlg::OnMainClear)
END_MESSAGE_MAP()

int CAboutDlg::ShowLogo()
{
	CString strPath = GetExePath(true);
	strPath = strPath + c_strLogoFileName;
	if (!m_pPicLoader)
	{
		m_pPicLoader = new JPGLoader;
	}

	CWnd* pWnd = (CWnd*)(&m_logoPic);

	ASSERT(pWnd);
	m_pPicLoader->SetDisplayWnd(pWnd);

	m_pPicLoader->SetJPGFileName(strPath);

	m_pPicLoader->LoadPic();
	int nRet = m_pPicLoader->ShowJPG();

	//pWnd->Invalidate(TRUE);

	//UpdateData(TRUE);

	return nRet;

}

void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	ShowLogo();
}
// CISPDlg dialog




CISPDlg::CISPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CISPDlg::IDD, pParent)
	, m_ID(_T(""))
	, m_ModelName(_T("")) 
	,m_FirmVer(_T(""))
	, m_HardVer(_T(""))
	 
	, m_IPPort(0)
    
    
{
	 m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_nTabSel = 0;
	 m_bShowSN = FALSE;
 
	  FLASH_SUBID=FALSE;
	 m_pComWriter = NULL;
	 m_pTFTPServer = NULL;
	 m_plogFile=new CStdioFile;
	 m_pTCPFlasher = NULL;
	 m_pFileBuf = NULL;
    
 
}
CISPDlg::~CISPDlg()
{
	if (m_pComWriter)
	{
		delete m_pComWriter;
		m_pComWriter = NULL;
	}
	if (m_pTFTPServer)
	{
		delete m_pTFTPServer;
		m_pTFTPServer = NULL;
	}
	if (m_plogFile)
	{
		delete m_plogFile;
		m_plogFile=NULL;
	}
	if(m_pTCPFlasher)
	{
		delete m_pTCPFlasher;
	}
	if (m_pFileBuf)
	{
		delete m_pFileBuf;
	}
	 
}

void CISPDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_INFO, m_lbStatusInfo);
    //DDX_Control(pDX, IDC_TAB1, m_tab);

    DDX_Control(pDX, IDC_EDIT_MDBID1, id);
    DDX_Control(pDX, IDC_COMBO_COM, m_ComPort);
    DDX_Control(pDX, IDC_IPADDRESS_NC, m_ipAddress);
    DDX_Control(pDX, IDC_EDIT_NCPORT, m_ipPort);
    DDX_Control(pDX, IDC_CHECK_FLASH_SUBID, m_Check_SubID);
    DDX_Control(pDX, IDC_EDIT_MDBID2, m_SubID);
    DDX_Text(pDX, IDC_EDIT_MDBID1, m_ID);
    DDX_Control(pDX, IDC_BUTTON_PING2, m_Btn_ping);
    DDX_Text(pDX, IDC_Model_Name, m_ModelName);
    DDX_Text(pDX, IDC_Model_FIRMVER, m_FirmVer);
    DDX_Text(pDX, IDC_Model_HARDWAREVER, m_HardVer);
    DDX_Text(pDX, IDC_EDIT_NCPORT, m_IPPort);
    //  DDX_Text(pDX, IDC_BIN_INFORMATION, m_bin_inforamtion);
    //  DDX_Text(pDX, IDC_EDIT_FILEPATH2, m_strFirmwarepath);
}

BEGIN_MESSAGE_MAP(CISPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SELFILE, &CISPDlg::OnBnClickedButtonSelfile)
	ON_BN_CLICKED(IDC_BUTTON_FLASH, &CISPDlg::OnBnClickedButtonFlash)
	//wether repleace the line information
	ON_MESSAGE(WM_REPLACE_STATUSINFO, OnReplaceStatusInfo)
	ON_MESSAGE(WM_ADD_STATUSINFO, OnAddStatusInfo)
	//Finish 通知 Flash按钮是否可用
	ON_MESSAGE(WM_FLASH_FINISH, OnFlashFinish)
	 ON_MESSAGE(WM_UPDATA_DEVICE_INFORMATION,Show_Flash_DeviceInfor)
	ON_WM_CLOSE()
	//ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CISPDlg::OnTcnSelchangeTab1)
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_COM, &CISPDlg::OnBnClickedCom)
	ON_BN_CLICKED(IDC_NET, &CISPDlg::OnBnClickedNet)
	ON_BN_CLICKED(IDC_CHECK_FLASH_SUBID, &CISPDlg::OnBnClickedCheckFlashSubid)
	 
	ON_BN_CLICKED(IDC_BUTTON_PING2, &CISPDlg::OnBnClickedButtonPing2)
	 
	ON_COMMAND(ID_MAIN_CLEAR, &CISPDlg::OnMainClear)
	ON_COMMAND(ID_SAVE_LOG_INFO, &CISPDlg::OnSaveLogInfo)
	ON_BN_CLICKED(IDC_CLEAR_LOG, &CISPDlg::OnBnClickedClearLog)
	ON_BN_CLICKED(IDC_SAVE_LOG, &CISPDlg::OnBnClickedSaveLog)
	 
	ON_WM_CONTEXTMENU()

	ON_COMMAND(ID_MENU_APP, &CISPDlg::OnMenuApp)
	ON_COMMAND(ID_MENU_ABOUT, &CISPDlg::OnMenuAbout)
	ON_BN_CLICKED(IDC_SHOW_HEX, &CISPDlg::OnBnClickedShowHex)
    ON_BN_CLICKED(IDC_FLASH_SN, &CISPDlg::OnBnClickedFlashSn)
	ON_COMMAND(ID_MENU_SETTING, &CISPDlg::OnMenuSetting)
	ON_COMMAND(ID_MENU_CHECKHEX, &CISPDlg::OnMenuCheckhex)
END_MESSAGE_MAP()


// CISPDlg message handlers

BOOL CISPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);		// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	 GetDlgItem(IDC_EDIT_BAUDRATE)->SetWindowText(_T("19200"));
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//展示启动图片
	 g_strExePath=GetExePath(true);
	 m_strLogoFileName=g_strExePath + c_strLogoFileName;
	 SettingPath = g_strExePath + _T("\\Setting.ini");
	 ShowSplashWnd(c_nSplashTime);
	////////////////////////////////////////////////////////////////////////////
 	//从文件中读取配置参数
	 m_cfgFileHandler.SetParentWnd(this);
	 m_cfgFileHandler.CreateConfigFile(g_strExePath + c_strCfgFileName);
	 m_strCfgFilePath = g_strExePath + c_strCfgFileName;
	 CString filename;
	 CString flashmethod;
	 CString id;
	 CString comport;
	 CString BD;
	 CString ip;
	 CString ipport;

	 CString subnote;
	 CString subID;

	 Remote_timeout = GetPrivateProfileInt(_T("Setting"),_T("REMOTE_TIMEOUT"),1000,SettingPath);
	 m_cfgFileHandler.ReadFromCfgFileForAll(
		 filename,
		 flashmethod,
		 id,
		 comport,
		 BD,
		 ip,
		 ipport,
		 subnote,
		 subID
		 );
		 if(Remote_timeout < 50)
			 Remote_timeout = 1000;


		 CString AutoFlashConfigPath;
		 AutoFlashConfigPath = g_strExePath + _T("//AutoFlashFile.ini");
		 CFileFind fFind;
		 if(fFind.FindFile(AutoFlashConfigPath))
		 {
			 int command = GetPrivateProfileInt(_T("Data"),_T("Command"),0,AutoFlashConfigPath);
			 if(command != START_AUTO_FLASH_COMMAND)
			 {
				 DeleteFile(AutoFlashConfigPath);
				 MessageBox(_T("ISP Tool will exit soon."));
				 PostMessage(WM_CLOSE,NULL,NULL);
				 return TRUE;
				 //删除文件并退出;
			 }
			 GetPrivateProfileStringW(_T("Data"),_T("ID"),_T("255"),id.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
			 id.ReleaseBuffer();
			 GetPrivateProfileStringW(_T("Data"),_T("COM_OR_NET"),_T("COM"),flashmethod.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
			 flashmethod.ReleaseBuffer();
			 GetPrivateProfileStringW(_T("Data"),_T("COMPORT"),_T("COM1"),comport.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
			 comport.ReleaseBuffer();
			 GetPrivateProfileStringW(_T("Data"),_T("Baudrate"),_T("19200"),BD.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
			 BD.ReleaseBuffer();
			 GetPrivateProfileStringW(_T("Data"),_T("IPAddress"),_T("192.168.0.3"),ip.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
			 ip.ReleaseBuffer();
			 GetPrivateProfileStringW(_T("Data"),_T("IPPort"),_T("6001"),ipport.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
			 ipport.ReleaseBuffer();
			 GetPrivateProfileStringW(_T("Data"),_T("Subnote"),_T("0"),subnote.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
			 subnote.ReleaseBuffer();
			 GetPrivateProfileStringW(_T("Data"),_T("SubID"),_T("255"),subID.GetBuffer(MAX_PATH),MAX_PATH,AutoFlashConfigPath);
			 subID.ReleaseBuffer();
			 get_file_thread_handle=CreateThread(NULL,NULL,GetFWFileProc,this,NULL,NULL);
			 CloseHandle(get_file_thread_handle);
		 }

	 GetDlgItem(IDC_EDIT_FILEPATH)->SetWindowText(filename); 
	 GetDlgItem(IDC_EDIT_MDBID1)->SetWindowText(id);
	 GetDlgItem(IDC_COMBO_COM)->SetWindowText(comport);
	 GetDlgItem(IDC_EDIT_BAUDRATE)->SetWindowText(BD);
	 m_ipAddress.SetWindowText(ip);
	 GetDlgItem(IDC_EDIT_NCPORT)->SetWindowText(ipport);



	/////////////////
	 if(flashmethod.Find(_T("COM"))!=0)
	 {
		 
		 g_Commu_type=1;
		 COM_INPUT=FALSE;
		 ((CButton *)GetDlgItem(IDC_COM))->SetCheck(FALSE);
		 ((CButton *)GetDlgItem(IDC_NET))->SetCheck(TRUE);

	 }
	 else
	 {	 
		 COM_INPUT=TRUE;
		 g_Commu_type=0;
		  
		 ((CButton *)GetDlgItem(IDC_COM))->SetCheck(TRUE);
		 ((CButton *)GetDlgItem(IDC_NET))->SetCheck(FALSE);

	
	 }
	 if(subnote.CompareNoCase(_T("1"))==0)
	 {
		 ((CButton *)GetDlgItem(IDC_CHECK_FLASH_SUBID))->SetCheck(1);
		 GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(1);
		 FLASH_SUBID = TRUE;
	 }
	 else
	 {
		 ((CButton *)GetDlgItem(IDC_CHECK_FLASH_SUBID))->SetCheck(0);
		 GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(0);
		 FLASH_SUBID = FALSE;
	 }
	 GetDlgItem(IDC_EDIT_MDBID2)->SetWindowText(subID);
	////g_Commu_type=0;	//初始的为COM口烧写
	//((CButton *)GetDlgItem(IDC_COM))->SetCheck(TRUE);
	// COM_INPUT=TRUE;

	 COM_NET_Set_ReadOnly();
	 FLASH_SUBID=FALSE;
	 //初始化COM口
	 InitCombox();
	 //日志文件的初始化
	 //日志文件的路径
	 m_strLogFileName=g_strExePath+c_strLogFileName;
	 if (!m_plogFile->Open(m_strLogFileName.GetString(),CStdioFile::modeRead))
	 {
		 if(m_plogFile->Open(m_strLogFileName.GetString(),CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate ))
		 {
		 }
		 else
		 {
			 CString strTips = _T("|Cannot create a new Log file. Please try again.");
			 UpdateStatusInfo(strTips, FALSE);
			 m_plogFile->Close();
		 }
	 }
	 m_plogFile->Close();
	 //get db path;
	 CString DB_T=_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=");
	 m_strDatabasefilepath=DB_T+g_strExePath+c_strDBFileName;


	 m_ToolTip= new CToolTipCtrl();
	 m_ToolTip->Create(this);
	 //ID 
	 CWnd* pW_ID=GetDlgItem(IDC_EDIT_MDBID1);
	 m_ToolTip->AddTool(pW_ID,_T("You want to input more than one IDs,using ';'as a seperator.250;251;252;253"));
	 //IDC_COMBO_COM
	 CWnd* pW_COMBO_COM=GetDlgItem(IDC_COMBO_COM);
	 m_ToolTip->AddTool(pW_COMBO_COM,_T("Please Select a COM Port"));
	 //IDC_EDIT_BAUDRATE
	 CWnd* pW_BAUDRATE=GetDlgItem(IDC_EDIT_BAUDRATE);
	 m_ToolTip->AddTool(pW_BAUDRATE,_T("This baudrate is default:19200"));
	 //IDC_IPADDRESS_NC
	 //CWnd* pW_IPADDRESS=GetDlgItem(IDC_IPADDRESS_NC);
	 //m_ToolTip->AddTool(pW_IPADDRESS,_T("This is IP for NC or Light Controller"));
	 ////IDC_EDIT_NCPORT
	 //CWnd* pW_NCPORT=GetDlgItem(IDC_IPADDRESS_NC);
	 //m_ToolTip->AddTool(pW_NCPORT,_T("Input the port responding to IP"));
	 ////IDC_CHECK_FLASH_SUBID
	 //CWnd* pW_FLASH_SUBID=GetDlgItem(IDC_CHECK_FLASH_SUBID);
	 //m_ToolTip->AddTool(pW_NCPORT,_T("Flash Sub node thought NC or LC."));
	 //IDC_EDIT_MDBID2
	 CWnd* pW_EDIT_MDBID2=GetDlgItem(IDC_EDIT_MDBID2);
	 m_ToolTip->AddTool(pW_EDIT_MDBID2,_T("Input more then one IDs of the Sub nodes,using ';'as a seperator.250;251;252;253"));

	 m_ToolTip->Activate(TRUE);


	 CString TitleShow;
	 CString m_strVersion;
	 m_strVersion.LoadString(IDS_STR_VIRSIONNUM);
	 TitleShow = _T("ISP Tool ") + m_strVersion;
	 SetWindowTextW(TitleShow);


	 if(IsDlgButtonChecked(IDC_CHECK_FLASH_SUBID)==1)
	 {
		 GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(1);
		 FLASH_SUBID=TRUE;
	 }
	 else
	 {
		 GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(0);
		 FLASH_SUBID=FALSE;
	 }

     initFlashSN();
// 	 GetDlgItem(IDC_EDIT_FILEPATH)->GetWindowText(m_strHexFileName);
// 	 ShowHexBinInfor();
	return TRUE;  // return TRUE  unless you set the focus to a control
}
 

void CISPDlg::CheckAutoFlash()
{

}

void CISPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
void CISPDlg::InitISPUI()
{
    CenterWindow(this);

    if(m_bShowSN)
    {
        WINDOWPLACEMENT wp;

        GetWindowPlacement(&wp);

        wp.rcNormalPosition.bottom += 0;

        SetWindowPlacement(&wp);

        GetDlgItem(IDC_STATIC_SEPERATOR)->ShowWindow(SW_NORMAL);
    }
    else
    {
        WINDOWPLACEMENT wp;
        GetWindowPlacement(&wp);

        CRect rc;
        CWnd* pWnd = GetDlgItem(IDC_STATIC_SEPERATOR);
        pWnd->GetWindowRect(&rc);
        //ScreenToClient(&rc);

        wp.rcNormalPosition.bottom = rc.bottom - 10;
        SetWindowPlacement(&wp);

        GetDlgItem(IDC_STATIC_SEPERATOR)->ShowWindow(SW_HIDE); 
    }

}

void CISPDlg::initFlashSN()
{
    
    if (!m_bShowSN)
    {
        GetDlgItem(IDC_BUTTON_FLASH)->SetWindowText(_T("FLASH"));
    }
    else
    {
        GetDlgItem(IDC_BUTTON_FLASH)->SetWindowText(_T("FLASH"));
    }
    InitISPUI();



    //////////////////////////////////////////////////////////////////////////
    //
    int nHWVersion = 0;
    int nModelValue = 0;

    m_cfgFileHandler.GetFlashSNParam(g_strExePath + CString(c_strFlashSNCfgFileName), nHWVersion, nModelValue);

    CWnd* pWnd = GetDlgItem(IDC_EDIT_HWVERSION);
    CString strTemp;strTemp.Format(_T("%d"), nHWVersion);

    pWnd->SetWindowText(strTemp);

    //////////////////////////////////////////////////////////////////////////
    m_cfgFileHandler.GetProductModel(g_strExePath + CString(c_strProductModelFileName), m_mapModel);

    CComboBox* pModel = (CComboBox*)GetDlgItem(IDC_COMBO_PM);

    pModel->ResetContent();
    map<int, CString>::iterator m1 = m_mapModel.begin();


    int nCurrentSel = 0;
    for (int i = 0 ; m1 != m_mapModel.end( ); m1++, i++ )
    {
        pModel->AddString(m1->second);
        if (m1->first == nModelValue)
        {
            nCurrentSel = i;
        }
    }

    pModel->SetCurSel(nCurrentSel);

}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CISPDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CISPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CISPDlg::ShowSplashWnd(int nMillisecond)
{
	CSplashWnd::SetSplashPicFilePath(m_strLogoFileName);
	CSplashWnd::EnableSplashScreen();
	CSplashWnd::ShowSplashScreen(this);

	Sleep(nMillisecond);
	CSplashWnd::endSplashScreen();
}
void CISPDlg::OnBnClickedButtonSelfile()
{
	UpdateData();
	//CString strFilter = _T("hex File|*.hex|bin File|*.bin|all File|*.*||");
	CString strFilter = _T("hex File;bin File|*.hex;*.bin|all File|*.*||");
    CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
    dlg.DoModal();
    m_strHexFileName=dlg.GetPathName();
    m_strFlashFileName=dlg.GetPathName();
	 
	CWnd* pEditFilePath = (CWnd*)GetDlgItem(IDC_EDIT_FILEPATH);
	pEditFilePath->SetWindowText(m_strHexFileName);

 ShowHexBinInfor();
   
  

}
void CISPDlg::SaveParamToConfigFile()
{	   
	 
	GetDlgItem(IDC_EDIT_FILEPATH)->GetWindowText(m_strHexFileName); //m_strFlashFileName;//((CISPDlg*)GetParent())->GetFlashFileName();
	GetDlgItem(IDC_EDIT_FILEPATH)->GetWindowText(m_strFlashFileName);
	CString flashmethod;
	if (g_Commu_type==0)
	{
		flashmethod=_T("COM");
	}
	else
	{
		flashmethod=_T("Ethernet");
	}

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MDBID1);		
	CString strMID;
	pEdit->GetWindowText(strMID);
	strMID.Trim();
	strMID.TrimLeft(_T("0"));

	// com port
	CString strComPort;
	m_ComPort.GetWindowText(strComPort);

	// Baudrate
	CString strBaudrate = _T("19200");

	// 
	/*CString strFlashPage;
	strFlashPage = GetCurSelPageStr();*/
	//	
	CString subnote;
	if(IsDlgButtonChecked(IDC_CHECK_FLASH_SUBID)==TRUE)
		subnote=_T("1");
	else
		subnote=_T("0");

	CString subID;
	GetDlgItemText(IDC_EDIT_MDBID2,subID);

	 CString strip;
	  m_ipAddress.GetWindowText(strip);
	  CString stripport;
	 GetDlgItem(IDC_EDIT_NCPORT)-> GetWindowText(stripport);
	m_cfgFileHandler.WriteToCfgFile(m_strHexFileName,
		flashmethod,
		strMID,
		strComPort,
		strBaudrate,
		strip,
		stripport,
		subnote,
		subID);
}

void CISPDlg::OnBnClickedButtonFlash()
{
 
	  UpdateData();
	  SaveParamToConfigFile();	//保存用户输入的数据

      
	  switch(Judge_Flash_Type())
	  {
	  case FLASH_TSTAT_COM:
		  {	 
			  SetCommunicationType(0);

			  FlashTstat();
			  break;
		  }
	  case FLASH_NC_LC_NET:
		  {	  
          
          
              SetCommunicationType(1);
              FlashNC_LC();
		    
		  
			 
			  
			  break;
		  }
	  case FLASH_SUBID_NET:
		  {	
			  SetCommunicationType(1);
			   FlashSubID();
			  break;
		  }
	  
	  default:
		  {
			 //这个是不存在的类型状态
		  }
	  }

 
}
//这里要想办法判断插入电脑上的是何种设备
unsigned int CISPDlg::Judge_Flash_Type()
{
	if (g_Commu_type==0)
	{
		return FLASH_TSTAT_COM;
	}
	else
	{	 
		 	
			if(FLASH_SUBID)
			{
				return FLASH_SUBID_NET;
			}
			else
			{
				return FLASH_NC_LC_NET;
			}

	 
		
	}

	 
}

// NC 是使用tftp协议来进行flash的
afx_msg LRESULT CISPDlg::OnFlashFinish(WPARAM wParam, LPARAM lParam)
{

	int nRet = lParam; // 线程中有消息弹出，这里就不弹了
	// flash完了，释放资源
	if (m_pComWriter)
	{
		delete m_pComWriter;
		m_pComWriter = NULL;
	}
	if (m_pFileBuffer)
	{
		delete[] m_pFileBuffer;
		m_pFileBuffer = NULL;
	}

	if (m_pTCPFlasher)
	{
		delete m_pTCPFlasher;
		m_pTCPFlasher = NULL;
	}
	EnableFlash(TRUE);	

    if (m_bShowSN	)
    {
        FlashSN();
    }
	return 1;
}

afx_msg LRESULT CISPDlg::OnAddStatusInfo(WPARAM wParam, LPARAM lParam)
{
	CString strInfo = CString(LPTSTR(lParam));
	UpdateStatusInfo(strInfo, false);
	WCHAR* szTemp = (LPTSTR(lParam));
	delete [] szTemp;

	//UpdateStatusInfo(g_strFlashInfo, false);
	return 1;
}

afx_msg LRESULT CISPDlg::OnReplaceStatusInfo(WPARAM wParam, LPARAM lParam)
{
	CString strInfo = CString(LPTSTR(lParam));
	UpdateStatusInfo(strInfo, true);
	WCHAR* szTemp = (LPTSTR(lParam));
	delete[] szTemp;

	//UpdateStatusInfo(g_strFlashInfo, true);
	return 1;
}




////////////////////////////////////////////////////////////////////////////
// 参数 BOOL, =TRUE replace the current line, =FALSE add a new line
void CISPDlg::UpdateStatusInfo(const CString& strInfo, BOOL bReplace)
{
	int nLineCount = m_lbStatusInfo.GetCount();
	int nCurrentLineNum = nLineCount;
	if (bReplace)
	{
		nCurrentLineNum--;
		m_lbStatusInfo.DeleteString(nCurrentLineNum);
	}

	int nRet =m_lbStatusInfo.InsertString(nCurrentLineNum,strInfo);
	if (nRet == LB_ERR )
	{
		ASSERT(0);
	}
	m_lbStatusInfo.SetCurSel(nRet);

	UpdateData(FALSE);
}


void CISPDlg::OnClose()
{

	CDialog::OnClose();
}

BOOL CISPDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
	
		if (pMsg->wParam  == VK_RETURN)
		{
			OnBnClickedButtonFlash();
		}
	
		if (pMsg->wParam == VK_F2 )
		{
			if(GetKeyState(VK_MENU) == 1)
			{
            initFlashSN();
            }
				//initFlashSN();
		}
	}
	if(m_ToolTip != NULL)
		m_ToolTip->RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

void CISPDlg::SetFlashFileName(const CString& strFileName)
{
	 CWnd* pWnd = GetDlgItem(IDC_EDIT_FILEPATH);
	 pWnd->SetWindowText(strFileName);	
}

CString CISPDlg::GetFlashFileName()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_FILEPATH);
	pWnd->GetWindowText(m_strHexFileName);

	return m_strHexFileName;
}
void CISPDlg::EnableFlash(BOOL bEnable)
{
	CWnd* pBtn = GetDlgItem(IDC_BUTTON_FLASH);
	pBtn->EnableWindow(bEnable);
	CWnd* pBtn1 = GetDlgItem(IDC_CLEAR_LOG);
	pBtn1->EnableWindow(bEnable);
	CWnd* pBtn2 = GetDlgItem(IDC_SAVE_LOG);
	pBtn2->EnableWindow(bEnable);

}
void CISPDlg::SaveFlashSNParamToFile()
{
	CString strHWV;
	CWnd* pWnd = GetDlgItem(IDC_EDIT_HWVERSION);
	pWnd->GetWindowText(strHWV);
	int nHWV = _wtoi(strHWV);

	CString strModel;		
	CComboBox* pCbx =(CComboBox*) GetDlgItem(IDC_COMBO_PM);
	int nSel = pCbx->GetCurSel();
	
	pCbx->GetWindowText(strModel);

	CString strtemp = m_mapModel[nSel];
	
	map<int, CString>::iterator m1;
	int nModelVal= nSel;
	for( m1=m_mapModel.begin(); m1 != m_mapModel.end(); m1++ )
	{
		CString strVal = m1->second;
		
		if(strVal.CompareNoCase(strModel) == 0)
		{
			nModelVal = m1->first;
			break;
		}
	}

	m_cfgFileHandler.SaveFlashSNParam(g_strExePath + CString(c_strFlashSNCfgFileName) , nHWV, nModelVal);

}
//For Old UI 
//选择page的时候触发的
CString CISPDlg::GetCurSelPageStr()
{
	CString strPageInfo = CString(c_strCfgTstatSection);
	int nSel = m_nTabSel;
	switch(nSel)
	{
	case 0:
		strPageInfo = CString(c_strCfgTstatSection);
		break;
	case 1:
		strPageInfo = CString(c_strCfgNCSection);
		break;
	case 2:
		strPageInfo = CString(c_strCfgLightCtrlSection);
		break;
	default:
		ASSERT(0);
		break;
	}

	return strPageInfo;
}
//For Old UI 
BOOL CISPDlg::GetFlashSNParam(int& nHWVerison, CString& strProductModel,int &nProductID)
{
    if (m_bShowSN)
    {
        //nProductModel = 1;

        CString strHWV;
        CWnd* pWnd = GetDlgItem(IDC_EDIT_HWVERSION);
        pWnd->GetWindowText(strHWV);
        nHWVerison = _wtoi(strHWV);

        CString strPM;
        pWnd = GetDlgItem(IDC_COMBO_PM);
        pWnd->GetWindowText(strProductModel);

        //nProductID=m_nproductid;
        CString strModel;		
        CComboBox* pCbx =(CComboBox*) GetDlgItem(IDC_COMBO_PM);
        int nSel = pCbx->GetCurSel();

        pCbx->GetWindowText(strModel);

        CString strtemp = m_mapModel[nSel];

        map<int, CString>::iterator m1;
        int nModelVal= nSel;
        for( m1=m_mapModel.begin(); m1 != m_mapModel.end(); m1++ )
        {
            CString strVal = m1->second;

            if(strVal.CompareNoCase(strModel) == 0)
            {
                nProductID = m1->first;

                break;
            }
        }


    }
    else
    {
        nHWVerison = 0;
        return FALSE;
    }

    return TRUE;
}

//Ping 
void CISPDlg::OnTestPing(const CString& strIP)
{
	m_strPingIP = strIP;
	AfxBeginThread(_PingThread, this);
}
UINT _PingThread(LPVOID pParam)
{
	CISPDlg* pDlg = (CISPDlg*)(pParam);
	CString strIP = pDlg->m_strPingIP;
	Ping(strIP, pDlg);
	return 1;
}

BOOL CISPDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: Add your message handler code here and/or call default
	int nSize = pCopyDataStruct->cbData;// = (strMsg.GetLength()+1)*sizeof(TCHAR); //(lstrlen(szBuffer)+1)*sizeof(TCHAR);
	//pCopyDataStruct->lpData =  (LPVOID)(LPCTSTR)strMsg;
	CString strMsg = CString(LPCTSTR(pCopyDataStruct->lpData));

	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST_INFO);
	//pList->AddString(strMsg);
	UpdateStatusInfo(strMsg, FALSE);
	//UpdateData(TRUE);
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
	/*return CDialog::OnCopyData(pWnd, pCopyDataStruct);*/
}
//点击COM口按钮的设置
void CISPDlg::OnBnClickedCom()
{	
	GetDlgItem(IDC_COM)->SetWindowText(_T("Input more than one ID"));
	GetDlgItem(IDC_NET)->SetWindowText(_T("NET FLASH"));
	 COM_INPUT=TRUE;
	 g_Commu_type=0;
	 COM_NET_Set_ReadOnly();
}
//可用性设置
void CISPDlg::COM_NET_Set_ReadOnly(){
	if (COM_INPUT)
	{
		  id.EnableWindow();
		  m_ComPort.EnableWindow();
		 // m_ComPort.SetWindowText(_T("Input more than one ID and Select COM"));
		  m_ipAddress.EnableWindow(FALSE);
	      m_ipPort.EnableWindow(FALSE);
	      m_Check_SubID.EnableWindow(FALSE);
		  m_SubID.EnableWindow(FALSE);
		  m_Btn_ping.EnableWindow(FALSE);
		   GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(FALSE);

	} 
	else
	{
		id.EnableWindow(FALSE);
		m_ComPort.EnableWindow(FALSE);

		m_ipAddress.EnableWindow();
		m_ipPort.EnableWindow();
		m_Check_SubID.EnableWindow();
		m_Btn_ping.EnableWindow();
		set_SUBID();
		if(IsDlgButtonChecked(IDC_CHECK_FLASH_SUBID))
		{
			 GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(1);
		}
		else
		{
			GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(0);
		}
	}
}
//if Click check box ,it means that click button .
void CISPDlg::set_SUBID(){		 
	if (FLASH_SUBID)
	{
		m_SubID.EnableWindow();
	}
	else
	{
		m_SubID.EnableWindow(FALSE);
	}
}
void CISPDlg::OnBnClickedNet()
{
	COM_INPUT=FALSE; 
	 g_Commu_type=1;
	 GetDlgItem(IDC_COM)->SetWindowText(_T("COM FLASH"));
	 GetDlgItem(IDC_NET)->SetWindowText(_T("Input IP and Port"));
	COM_NET_Set_ReadOnly();
}

void CISPDlg::OnBnClickedCheckFlashSubid()
{	  
	m_Check_SubID.SetWindowTextW(_T("Flash SubNode By ID"));
	if(IsDlgButtonChecked(IDC_CHECK_FLASH_SUBID)==1)
	{
		  GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(1);
		  FLASH_SUBID=TRUE;
	}
	else
	{
		 GetDlgItem(IDC_EDIT_MDBID2)->EnableWindow(0);
		 FLASH_SUBID=FALSE;
	}
	 //if (FLASH_SUBID)
	 //{		 
		// FLASH_SUBID=FALSE;
		// m_Check_SubID.SetWindowTextW(_T("Flash SubNode By ID"));
	 //}
	 //else
	 //{
		// FLASH_SUBID=TRUE;
		// m_Check_SubID.SetWindowTextW(_T("Input SubID Below"));
	 //}

	 // set_SUBID();
}

/*
Author:	Alex
Date: 2012-10-25
Function:
Initial COM Port
*/
void CISPDlg::InitCombox(void)
{
	vector<CString> szComms;
	BOOL bRet = GetSerialComm_ISP(szComms);
	if (bRet && szComms.size()>0)
	{
		m_ComPort.Clear();
		for (UINT i = 0; i < szComms.size(); i++)
		{
			m_ComPort.AddString(szComms[i]);
		}
	}
	else  // can not get com port name from register, then add com port manually
	{
		CString strTips = _T("|Can not find any Com port on your computer.");
		UpdateStatusInfo(strTips, FALSE);
		m_ComPort.Clear();
		for (UINT i = 1; i < 9; i++)
		{
			CString strCom;
			strCom.Format(_T("COM%d"), i);
			m_ComPort.AddString(strCom);
		}
	}
}
/*
Author:	Alex
Date: 2012-10-25
Function:
Choose com port,Flash Tstat.
*/
BOOL CISPDlg::FlashTstat(void)
{
      int TempID=0;
	  int times=0;
	  CString temp;
      if (m_strHexFileName.IsEmpty())
      {
      // PostMessage(WM_FLASH_FINISH, 0, LPARAM(1));
      // int	nRet =PostMessage(m_pParentWnd->m_hWnd, WM_FLASH_FINISH, 0, LPARAM(nFlashFlag));

      FlashSN();
      return FALSE;
      }
	if(!FileValidation(m_strHexFileName))
	{
		return FALSE;
	}

	if (g_Commu_type == 0)
	{ 
        int nPort = GetComPortNo();
		if(!ValidMdbIDString())
		{
			return FALSE;
		}

		if(!GetModbusID(m_szMdbIDs))
		{
			return FALSE;		
		}
		BOOL flag=open_com(nPort);
		if (flag)
		{
			if (m_szMdbIDs.size()<=1)
			{
				TempID=m_szMdbIDs[0];

				int realID=-1;
					realID=Read_One(TempID,6);
				while(realID<0){//the return value == -1 ,no connecting
					if(times<5)
					{
						times++;
						realID=Read_One(TempID,6);
						continue;
					}
					else
					{
						break;
					}

				}
				if (times>=5)
				{
					temp.Format(_T("ISPTool Can't connect to %d\n"),TempID);
					UpdateStatusInfo(temp,false);
					/*int ret=AfxMessageBox(temp,MB_OKCANCEL);
					if(ret==IDOK)
					{
						TempID=255;
						m_szMdbIDs[0]=255;
						temp.Format(_T("%d"),TempID);
						id.SetWindowText(temp);
					}*/
				 return FALSE;

				}
				 else
				 {
					 if (realID<0)
					 {
						 AfxMessageBox(_T("Can't read the device ID,please check the connection."));
						 return FALSE;
					 }
					 if (realID!=TempID)
					 {
						 temp.Format(_T("The ID is %d,retry to flash it !\n"),realID);
						  AfxMessageBox(temp);
						  UpdateStatusInfo(temp,false);
						  temp.Format(_T("%d"),realID);
						  id.SetWindowText(temp);
						  return FALSE;
						
					 }



				 }
			} 
			else
			{   
				for (UINT i=0;i<m_szMdbIDs.size();i++)
				{
					TempID=m_szMdbIDs[i];
					while(Read_One(TempID,0xee10)<0){//the return value == -1 ,no connecting
						if(times<5)
						{
							times++;
							continue;
						}
						else
						{
							break;
						}

					}
					if (times>=5)
					{
						temp.Format(_T("ISPTool Can't connect to %d\n"),TempID);
						continue;
						UpdateStatusInfo(temp,false);
					}
				}
			}
			close_com();
		}
		else{
			UpdateStatusInfo(_T("Com Occupied"),false);
			AfxMessageBox(_T("Com Occupied"));
			return FALSE;
		}

		
		 
		FlashByCom();
	}
	else
	{

	}

	return TRUE;
}
 /*
 Author:	Alex
 Date: 2012-10-30
 Function:
 Choose NET,Can Flash NC
*/
BOOL CISPDlg::FlashNC_LC(void)
{
   
        if(!FileValidation(m_strHexFileName))
        {
            return FALSE;
        }
        if(BinFileValidation(m_strHexFileName))
        {
            CBinFileParser* pBinFile=new CBinFileParser;
            pBinFile->SetBinFileName(m_strHexFileName);
            m_pFileBuffer=new char[c_nBinFileBufLen];
            memset(m_pFileBuffer, 0xFF, c_nBinFileBufLen);

            int nDataSize=pBinFile->GetBinFileBuffer(m_pFileBuffer,c_nBinFileBufLen);
            CString hexinfor=_T("The Bin For ");
            hexinfor+=pBinFile->GetBinInfor();
            CString strFilesize;
            strFilesize.Format(_T("Bin size=%d Bs"),nDataSize);
            GetDlgItem(IDC_HEX_SIZE)->SetWindowText(strFilesize);
            m_strASIX=pBinFile->m_strASIX;
            m_strProductName=pBinFile->m_strProductName;
            GetDlgItem(IDC_BIN_INFORMATION)->SetWindowText(hexinfor);
        }
        if (g_Commu_type == 1)
        {
            FlashByEthernet();
        }
       return TRUE;
  

	

	return TRUE;
}
 /*
 Author:	Alex
 Date: 2012-10-30
 Function:
 通过tcp/ip，烧写hex文件
主要是通过NC/LC
flash tstat/input/output board.
BOOL FlashSubID(void)
*/
BOOL CISPDlg::FlashSubID(void)
{
	if(!ValidMdbIDStringSUBID())
	{
		return FALSE;
	}
	GetModbusIDSUBID(m_szMdbIDs);
	OnFlashSubID();
	return TRUE;
}
BOOL CISPDlg::ValidMdbIDStringSUBID()
{
	CString strModbusID;
	GetDlgItem(IDC_EDIT_MDBID2)->GetWindowText(strModbusID);
	if (strModbusID==_T(""))
	{
		AfxMessageBox(_T("Please Input sub node IDs!"));
		return FALSE;
	} 
	else
	{
		for(int i = 0; i < strModbusID.GetLength(); i++)
		{
			if(!(strModbusID.GetAt(i) >='0' && strModbusID.GetAt(i)<='9'))
			{
				if (strModbusID.GetAt(i) != ';')
				{
					CString strTips =_T("Error: Modbus ID invalidation."); 
					UpdateStatusInfo(strTips, FALSE);
					AfxMessageBox(strTips);
					return FALSE;	
				}			
			}
		}
		return TRUE;
	}
	
}

int CISPDlg::GetModbusIDSUBID(vector<int>& szMdbIDs)
{
	szMdbIDs.clear();
	TCHAR c = ';';
	CString strSrc;
	GetDlgItem(IDC_EDIT_MDBID2)->GetWindowText(strSrc);
	int nPos =0;
	while(strSrc.GetLength() > 0)
	{
		nPos = strSrc.Find(c);

		if (nPos != 0)
		{
			if (nPos < 0)
			{
				nPos = strSrc.GetLength();
			}
			CString strID = strSrc.Left(nPos);
			int nID = _wtoi(strID);//.GetString());

			if (!(nID > 0 && nID <= 255))
			{
				CString strTips;
				strTips.Format(_T("Error : Wrong Modbus ID : %d. Please input right ID."), nID);
				UpdateStatusInfo(strTips, FALSE);
				AfxMessageBox(strTips);
				return FALSE;
			}
			szMdbIDs.push_back(nID);
		}
		// 		else if(nPos == 0) // 在第一位
		// 		{
		// 		}

		if (nPos < 0 || (nPos == strSrc.GetLength())) // 找不到或者位于最后一位
		{
			return TRUE;
		}
		strSrc = strSrc.Mid(nPos+1);
	}

	return TRUE;
}

void CISPDlg::OnFlashSubID()
{
	 
	
	if (m_pTCPFlasher)
	{
		delete m_pTCPFlasher;
		m_pTCPFlasher = NULL;
	}

	if (m_pFileBuffer)
	{
		delete[] m_pFileBuffer;
		m_pFileBuf= NULL;
	}

	CHexFileParser* pHexFile = new CHexFileParser;
	pHexFile->SetFileName(m_strFlashFileName);

	m_pFileBuf = new char[c_nHexFileBufLen];
	memset(m_pFileBuf, 0xFF, c_nHexFileBufLen);
	int nDataSize = pHexFile->GetHexFileBuffer(m_pFileBuf, c_nHexFileBufLen);


	//////////////////////////////////////////////////////////////////////////
	if(nDataSize > 0)
	{
		CString strTips = _T("Hex file verified okay.");
		UpdateStatusInfo(strTips, FALSE);

		//////////////////////////////////////////////////////////////////////////


		m_pTCPFlasher = new CComWriter;
		m_pTCPFlasher->SetParentWnd(this);
		  CString ipaddress;
		  m_ipAddress.GetWindowText(ipaddress);
		m_pTCPFlasher->SetIPAddr(ipaddress);
		CString strIPPort;
		 
		 
		m_pTCPFlasher->SetIPPort(m_IPPort);		

		m_pTCPFlasher->SetModbusID(m_szMdbIDs);

		m_pTCPFlasher->SetFileBuffer((TS_UC*)m_pFileBuf, nDataSize);



//Fance
		if(pHexFile->GetHexFileFormatType() == HEXFILE_DATA)
		{
			m_pTCPFlasher->SetFileBuffer((TS_UC*)m_pFileBuf, nDataSize);

			m_pTCPFlasher->m_nHexFileType = HEXFILE_DATA;
		}
		else if(pHexFile->GetHexFileFormatType() == HEXFILE_LINERADDR)
		{
			vector<int> szFlags;
			pHexFile->GetExtendHexFileSectionFlag(szFlags);
			m_pTCPFlasher->SetExtendHexFileParam(szFlags, (TS_UC*)m_pFileBuf);

			m_pTCPFlasher->m_nHexFileType = HEXFILE_LINERADDR;
		}
//Fance
		//m_szHexFileFlags


		m_pTCPFlasher->BeginWirteByTCP();

	}
	else
	{
		CString strTips1 =_T("File error: The file is not a properly formatted HEX file.");
		UpdateStatusInfo(strTips1, FALSE);
		CString strTips2 =_T("Please reselect a right file.");
		UpdateStatusInfo(strTips2, FALSE);
		AfxMessageBox(strTips1+strTips2, MB_OK);

	}

	delete pHexFile;
}
 /*
 Author:	Alex
 Date: 2012-10-30
 Function:
 Choose NET,Flash NC,LCBy net
*/
void CISPDlg::FlashByEthernet()
{
	if (m_pTFTPServer)
	{
		delete m_pTFTPServer;
		m_pTFTPServer = NULL;
	}
	if (m_pFileBuffer)
	{
		delete[] m_pFileBuffer;
		m_pFileBuffer = NULL;
	}
	Bin_Info bin_infor;
	Get_Binfile_Information(m_strFlashFileName.GetBuffer(),bin_infor);
	 
	CBinFileParser* pBinFile = new CBinFileParser;
	pBinFile->SetBinFileName(m_strFlashFileName);
	m_pFileBuffer = new char[c_nBinFileBufLen];
	ZeroMemory(m_pFileBuffer, c_nBinFileBufLen);
	int nDataSize = pBinFile->GetBinFileBuffer(m_pFileBuffer, c_nBinFileBufLen);

	if(nDataSize > 0)
	{
		CString strTips = _T("|Firmware bin file verified okay.");
		UpdateStatusInfo(strTips, FALSE);

        
         

		m_pTFTPServer=new TFTPServer;
		DWORD dwIP = GetIPAddress();
		//m_IPAddr.GetAddress(dwIP);


		m_pTFTPServer->SetParentDlg(this);
		m_pTFTPServer->SetClientPort(m_IPPort);
		m_pTFTPServer->SetClientIP(dwIP);
		m_pTFTPServer->SetFileName(m_strFlashFileName);
        m_pTFTPServer->Set_FileProductName(m_strProductName);
		m_pTFTPServer->SetDataSource((BYTE*)m_pFileBuffer, nDataSize);
	    m_pTFTPServer->Set_bininfor(bin_infor);
		m_pTFTPServer->FlashByEthernet();
		
		 EnableFlash(FALSE);	
	}
	else
	{
		CString strTips1 =_T("|Error: The file is not a properly formatted BIN file.");
		UpdateStatusInfo(strTips1, FALSE);
		CString strTips2 =_T("|Please reselect a right file.");
		UpdateStatusInfo(strTips2, FALSE);
		//AfxMessageBox(strTips1+strTips2, MB_OK);
	}

	delete pBinFile;
}
DWORD CISPDlg::GetIPAddress()
{
	DWORD dwIP=0;
	m_ipAddress.GetAddress(dwIP);

	return dwIP;
}
/*
Author:	Alex
Date: 2012-10-25
Function:
verify the hex or bin file
*/
BOOL CISPDlg::FileValidation(const CString& strFileName)
{
	if (g_Commu_type==0 && !HexFileValidation(strFileName))
	{
		CString strTips;
		strTips = _T("|To Update over Com port, please select a *.HEX file");
		AfxMessageBox(strTips);
		UpdateStatusInfo(strTips, FALSE);
		return FALSE;
	}
	else if(g_Commu_type && !BinFileValidation(strFileName))
	{
		CString strTips;
		//strTips.Format(_T("%s isn't a BIN file."), strFileName);
		strTips = _T("|To Updating over Ethernet, please select a *.BIN file");
		AfxMessageBox(strTips);
		UpdateStatusInfo(strTips, FALSE);
		return FALSE;
	}

	return TRUE;
}
 /*
Author:	Alex
Date: 2012-10-25
Function:
verify ID that customer input.
*/
BOOL CISPDlg::ValidMdbIDString(void)
{
	CString strModbusID;
	GetDlgItem(IDC_EDIT_MDBID1)->GetWindowText(strModbusID);
	if (strModbusID==_T(""))
	{
		AfxMessageBox(_T("Please input IDs"));
		return FALSE;
	} 
	else
	{
	}
	for(int i = 0; i < strModbusID.GetLength(); i++)
	{
		if(!(strModbusID.GetAt(i) >='0' && strModbusID.GetAt(i)<='9'))
		{
			if (strModbusID.GetAt(i) != ';')
			{
				CString strTips =_T("|Error: Modbus ID invalidation."); 
				UpdateStatusInfo(strTips, FALSE);
				//AfxMessageBox(strTips);
				return FALSE;	
			}			
		}
	}
	return TRUE;


}
//或者ModbusID 
int CISPDlg::GetModbusID(vector<int>& szMdbIDs)
{
	szMdbIDs.clear();
	TCHAR c = ';';
	CString strSrc;
	GetDlgItem(IDC_EDIT_MDBID1)->GetWindowText(strSrc);
	int nPos =0;
	while(strSrc.GetLength() > 0)
	{
		nPos = strSrc.Find(c);

		if (nPos != 0)
		{
			if (nPos < 0)
			{
				nPos = strSrc.GetLength();
			}
			CString strID = strSrc.Left(nPos);
			int nID = _wtoi(strID);//.GetString());

			if (!(nID > 0 && nID <= 255))
			{
				CString strTips;
				strTips.Format(_T("|Error : Wrong Modbus ID : %d. Please input right ID."), nID);
				UpdateStatusInfo(strTips, FALSE);
				//AfxMessageBox(strTips);
				return FALSE;
			}
			szMdbIDs.push_back(nID);
		}
		if (nPos < 0 || (nPos == strSrc.GetLength())) // 找不到或者位于最后一位
		{
			return TRUE;
		}
		strSrc = strSrc.Mid(nPos+1);
	}
	return TRUE;
}
//When Flash by COM,use the function.
void CISPDlg::ShowHexBinInfor(){

	Get_HexFile_Information(m_strHexFileName.GetBuffer(),temp1);
	CString temp;
	UpdateStatusInfo(_T(">>>>>The File Information<<<<<"), FALSE);
	temp.Format(_T("Company Name: "));
	for (int i=0;i<5;i++)
	{
		temp.AppendFormat(_T("%c"),temp1.company[i]);
	}
	GetDlgItem(IDC_Model_Name2)->SetWindowText(temp);
	UpdateStatusInfo(temp,FALSE);
	temp.Format(_T("Product Name: "));
	for (int i=0;i<10;i++)
	{
		temp.AppendFormat(_T("%c"),temp1.product_name[i]);
	}
	GetDlgItem(IDC_Model_FIRMVER2)->SetWindowText(temp);
	UpdateStatusInfo(temp,FALSE);
	float rev;
	rev=((float)(temp1.software_high*256+temp1.software_low))/10;
	temp.Format(_T("The Version: %0.1f"),rev);
	GetDlgItem(IDC_Model_HARDWAREVER2)->SetWindowText(temp);
	UpdateStatusInfo(temp,FALSE);
}
void CISPDlg::FlashByCom()
{
	if (m_pComWriter)
	{
		delete m_pComWriter;
		m_pComWriter = NULL;
	}

	if (m_pFileBuffer)
	{
		delete[] m_pFileBuffer;
		m_pFileBuffer= NULL;
	}

    CHexFileParser* pHexFile = new CHexFileParser;
    pHexFile->SetFileName(m_strHexFileName);

     
   /*  Get_HexFile_Information(m_strHexFileName.GetBuffer(),temp1);*/
	 ShowHexBinInfor();
// 	 CString temp;
// 	 UpdateStatusInfo(_T(">>>>>The Hex Information<<<<<"), FALSE);
// 
// 	 temp.Format(_T("Company Name: "));
// 	 for (int i=0;i<5;i++)
// 	 {
// 		 temp.AppendFormat(_T("%c"),temp1.company[i]);
// 	 }
// 	 UpdateStatusInfo(temp,FALSE);
// 	 temp.Format(_T("Product Name: "));
// 	 for (int i=0;i<10;i++)
// 	 {
// 		 temp.AppendFormat(_T("%c"),temp1.product_name[i]);
// 	 }
// 
// 	 UpdateStatusInfo(temp,FALSE);
// 	 float rev;
// 	 rev=((float)(temp1.software_high*256+temp1.software_low))/10;
// 	 temp.Format(_T("The Hex Version: %0.1f"),rev);
// 	 UpdateStatusInfo(temp,FALSE);
	//ShowHexBinInfor();
    //unsigned short chip_size=temp1.reserved[2];
    //temp.Format(_T("Chip Size=%d"),chip_size);
    //UpdateStatusInfo(temp,FALSE);


	m_pFileBuffer = new char[c_nHexFileBufLen];
	memset(m_pFileBuffer, 0xFF, c_nHexFileBufLen);
	int nDataSize = pHexFile->GetHexFileBuffer(m_pFileBuffer, c_nHexFileBufLen);//获取文件的buffer

	

		
	
	if(nDataSize > 0)
	{
		CString strTips = _T("|Hex file verified okay.");
		UpdateStatusInfo(strTips, FALSE);

		m_pComWriter = new CComWriter;
		m_pComWriter->SetModbusID(m_szMdbIDs);
        m_pComWriter->SetHexInfor(temp1);
		m_pComWriter->SetHexFileType(pHexFile->GetHexFileFormatType());

		CString strBaudrate; GetDlgItem(IDC_EDIT_BAUDRATE)->GetWindowText(strBaudrate);
		int nBautrate = _wtoi (strBaudrate.GetBuffer());
		m_pComWriter->SetBautrate(nBautrate);
		int nPort = GetComPortNo();
		m_pComWriter->SetParentWnd(this);

		m_pComWriter->SetComPortNO(nPort);
		if(pHexFile->GetHexFileFormatType() == HEXFILE_DATA)
		{
			m_pComWriter->SetFileBuffer((TS_UC*)m_pFileBuffer, nDataSize);
		}
		else if(pHexFile->GetHexFileFormatType() == HEXFILE_LINERADDR)
		{
			vector<int> szFlags;
			pHexFile->GetExtendHexFileSectionFlag(szFlags);
			m_pComWriter->SetExtendHexFileParam(szFlags, (TS_UC*)m_pFileBuffer);
		}

		int nRet = m_pComWriter->BeginWirteByCom();

		// disable flash button
		if (nRet != 0) // 表示开始写了
		{
			EnableFlash(FALSE);
		}
	}
	else
	{
		CString strTips1 =_T("|File error: The file is not a properly formatted HEX file.");
		UpdateStatusInfo(strTips1, FALSE);
		CString strTips2 =_T("|Please reselect a right file.");
		UpdateStatusInfo(strTips2, FALSE);
		 AfxMessageBox(strTips1+strTips2, MB_OK);

	}
	delete pHexFile;
}

int	CISPDlg::GetComPortNo()
{
	CString strComPort; 
	m_ComPort.GetWindowText(strComPort);
	CString c = strComPort.Mid(3);
	int nPort = _wtoi (c.GetBuffer());
	return nPort;
}
void CISPDlg::OnBnClickedButtonPing2()
{
	UpdateData(TRUE);
	CString strIP,strPort;
	GetDlgItem(IDC_IPADDRESS_NC)->GetWindowText(strIP);
	if (strIP.GetLength() <= 6)
	{
		AfxMessageBox(_T("Please Input a right IP address."));
		return;
	}
	OnTestPing(strIP);  
  Show_Flash_DeviceInfor_NET();
 
}
void Split_Cstring(CString Str,vector<CString>& ret_str,CString split_str)
{  CString temp,str_temp;
int index=0,start=0,count=-1;
while (start<Str.GetLength())
{
	int index=Str.Find(split_str,start);
	if (index!=-1)
	{
		str_temp.Empty();
		++count;
		for (int i=start;i<index;i++ )
		{
			temp.Format(_T("%C"),Str.GetAt(i));
			str_temp+=temp;
		}
		ret_str.push_back(str_temp);
		start=index+1; 
	}
	else
		break;

}

}
//更新正在刷新的产品信息
afx_msg LRESULT CISPDlg::Show_Flash_DeviceInfor(WPARAM wParam, LPARAM lParam)
{	 
	CString Device_info = CString(LPTSTR(lParam));
	vector<CString> vet_Device;
	Split_Cstring(Device_info,vet_Device,_T(" "));

	unsigned short US_Device_infor[10];
	for (int i=0;i<10;i++)
	{
		US_Device_infor[i]=_wtoi(vet_Device[i]);
	}

	int DeviceModel=-1; 
	//DeviceModel=read_multi(ID,&Device_infor[0],0,10);//获得了正在刷新的Model号-更加Model号码-返回产品的名字

	m_ModelName=GetProductName(US_Device_infor[7]);
	if (m_ModelName.Find(_T("ERROR"))!=0)
	{

		float tstat_version2 = (float)(US_Device_infor[MODBUS_VERSION_NUMBER_HI]*256+US_Device_infor[MODBUS_VERSION_NUMBER_LO]);	
		tstat_version2 /=10;

		m_FirmVer.Format(_T("%.1f"),tstat_version2);
		m_HardVer.Format(_T("%d"),US_Device_infor[MODBUS_HARDWARE_REV]); 
		/*展示到消息log框中*/
		UpdateStatusInfo(_T("|Device Information-Begin"),FALSE);
		UpdateStatusInfo(_T("|Model  Name:")+m_ModelName,FALSE);
		UpdateStatusInfo(_T("|Firmware Ver:")+m_FirmVer,FALSE);
		UpdateStatusInfo(_T("|Hardware Ver:")+m_HardVer,FALSE);
		UpdateStatusInfo(_T("|Device Information-End"),FALSE);
		UpdateData(FALSE); //显示在界面上
	}
	else
	{ m_ModelName=_T("");
	m_FirmVer=_T("");
	m_HardVer=_T("");
	}




	return TRUE;
	//  close_com();
}

BOOL CISPDlg::Show_Flash_DeviceInfor_NET()
{
	UpdateData(TRUE);
	CString strIP;
	GetDlgItem(IDC_IPADDRESS_NC)->GetWindowText(strIP);
	

	if (Open_Socket2(strIP,m_IPPort))
	{
		int DeviceModel=-1; 
		DeviceModel=read_one(255,MODBUS_PRODUCT_MODEL);//获得了正在刷新的Model号-更加Model号码-返回产品的名字
        if (DeviceModel<0)//Modbus 不通信，说明有可能在bootloader
        {
        return TRUE;
        }
		m_ModelName=GetProductName(DeviceModel);
		CString modelname=m_ModelName;
		CString productname=m_strProductName;
		modelname.MakeUpper();
		productname.MakeUpper();
		if (productname.Find(modelname)==-1)
		{
          CString infor;
          infor.Format(_T("The Bin file is for %s,but your device is %s"),m_strProductName.GetBuffer(),m_ModelName.GetBuffer());
          AfxMessageBox(infor);
          UpdateStatusInfo(infor,FALSE);
          return FALSE;
		}

		if (m_ModelName.Find(_T("ERROR"))!=0)
		{
			float tstat_version2=(float)read_one(255,MODBUS_VERSION_NUMBER_LO); //tstat version			
			if(tstat_version2<=0)
				m_FirmVer.Format(_T("%0.1f"),tstat_version2);
			if(tstat_version2 >=240 && tstat_version2 <250)
				tstat_version2 /=10;
			else 
			{
				tstat_version2 = (float)(read_one(255,MODBUS_VERSION_NUMBER_HI)*100+read_one(255,MODBUS_VERSION_NUMBER_LO));	
				tstat_version2 /=100;
			}
			m_FirmVer.Format(_T("%.1f"),tstat_version2);
			m_HardVer.Format(_T("%d"),read_one(255,MODBUS_HARDWARE_REV));  
			/*展示到消息log框中*/
 			UpdateStatusInfo(_T("|Device Information-Begin"),FALSE);
 			UpdateStatusInfo(_T("|Model  Name:")+m_ModelName,FALSE);
 			UpdateStatusInfo(_T("|Firmware Ver:")+m_FirmVer,FALSE);
 			UpdateStatusInfo(_T("|Hardware Ver:")+m_HardVer,FALSE);
 			UpdateStatusInfo(_T("|Device Information-End"),FALSE);
			UpdateData(FALSE);
		}
		else
		{ 
			m_ModelName=_T("");
			m_FirmVer=_T("");
			m_HardVer=_T("");
             
		}
        return TRUE;
	} 
	else
	{
		CString tips;
		tips.Format(_T("IP:%s,Port:%d can't open,You can press Ping for test connection"),strIP,m_IPPort);
		UpdateStatusInfo(tips,FALSE);
        return FALSE;
	}
    UpdateData(FALSE);
	close_com();
}
CString CISPDlg::Get_NET_Infor(CString strIPAdress,short nPort){
	CString modelname(_T(""));
	if (Open_Socket2(strIPAdress,nPort))
	{	
		int DeviceModel=-1; 
		DeviceModel=read_one(255,MODBUS_PRODUCT_MODEL);//获得了正在刷新的Model号-更加Model号码-返回产品的名字
		modelname=GetProductName(DeviceModel);
		//m_ModelName.Format(_T("%d"),DeviceModel);
		if(modelname.Find(_T("ERROR"))!=-1)
		{
			return modelname;
		}

	}
	else{
		CString tips;
		tips.Format(_T("IP:%s,Port:%d can't open,You can press Ping for test connection"),strIPAdress,nPort);
	    UpdateStatusInfo(tips,FALSE);	
	}
	
	return 	modelname;
	
}
 
void CISPDlg::OnMainClear()
{
	 OnBnClickedClearLog(); 
	 
}
void CISPDlg::OnSaveLogInfo()
{
	  OnBnClickedSaveLog();
}


void CISPDlg::OnBnClickedClearLog()
{
	 m_lbStatusInfo.ResetContent(); 
}


/*
把日志信息写入文件
*/
void CISPDlg::OnBnClickedSaveLog()
{
	if (m_lbStatusInfo.GetCount()!=0)
	{

		if(m_plogFile->Open(m_strLogFileName.GetString(),CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate ))
		{
			for (int i=0;i<m_lbStatusInfo.GetCount();i++)
			{
				CString logInfo;
				m_lbStatusInfo.GetText(i,logInfo);
				m_plogFile->WriteString(logInfo+_T("\n"));
			}

			m_plogFile->Close();
		}
		else
		{
			ASSERT(0);
			m_plogFile->Close();

		}

	   //打开文件
		 //LPCSTR filename=m_strLogFileName.GetBuffer();
	   //ShellExecute(NULL, "open","Log_info.txt",NULL, NULL, SW_SHOWNORMAL); 
	   ShellExecute(this->m_hWnd, _T("open"), m_strLogFileName, NULL, NULL, SW_SHOWNORMAL);
	}
	else
	{
	   //没有日志内容
	}
	 
	
}
//只有响应上下文菜单的时候，才可以触发Command事件
void CISPDlg::OnContextMenu(CWnd*  pWnd , CPoint  point )
{
	  //CPoint pt;	  //当不再OnContextMenu 时，获取鼠标的位置
	  //::GetCursorPos(&pt);
	  //
	  CRect rect;
	  (GetDlgItem(IDC_LIST_INFO))->GetWindowRect(&rect);
	 /* ScreenToClient(&rect);*/

	  if (rect.PtInRect(point))
	  {		  CWnd* pBtn = GetDlgItem(IDC_BUTTON_FLASH);
		  if ((m_lbStatusInfo.GetCount()>0)&&pBtn->IsWindowEnabled())
		  {
			  CMenu menu; 
			  menu.LoadMenu(IDR_MENU1);//菜单资源ID 
			  menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,point.x,point.y,this);     //m_newListCtrl是CListCtrl对象
			  menu.DestroyMenu();
		  }
		 
	  }
	  
}
 /*
 Author:	Alex
 Date: 2012-11-2 
 Function:
According to Model Name,return the hex file prefix of the Device Model Name
 */
CString CISPDlg::GetFilePrefix_FromDB(const CString& ModeName)
{
	_ConnectionPtr m_pConnection;
	AfxOleInit();
	m_pConnection.CreateInstance(__uuidof(Connection));
	try 
	{ 

		m_pConnection->Open(m_strDatabasefilepath.GetString(),"","",adModeUnknown);

	}
	catch(_com_error e)
	{
		AfxMessageBox(_T("数据库连接失败，确认数据库Demo.mdb是否在当前路径下!"));

	}
	_RecordsetPtr m_pRecordset;
	m_pRecordset.CreateInstance(__uuidof(Recordset));

	// 在ADO操作中建议语句中要常用try...catch()来捕获错误信息，
	// 因为它有时会经常出现一些意想不到的错误。jingzhou xu
	try
	{
		CString sql;
		sql.Format(_T("select HexPrefix from Products where Model='%s'"),ModeName);
		m_pRecordset->Open((_variant_t)sql,_variant_t((IDispatch *)m_pConnection,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	 _variant_t var;
	 CString str_FilePrefix=_T("");
	 try
	 {
		 if(!m_pRecordset->EndOfFile)  //说明Product表中有符合条件的数据
		 {
			 m_pRecordset->MoveFirst(); 
		 var = m_pRecordset->GetCollect("HexPrefix");
		 if(var.vt != VT_NULL)
			 str_FilePrefix = (LPCSTR)_bstr_t(var);
		 }
		 else
		 {
			 //AfxMessageBox(_T("表内数据为空"));	 //无符合条件的数据，返回空

			//return str_FilePrefix;
		 }
	

	 }
	 catch(_com_error *e)
	 {
		 AfxMessageBox(e->ErrorMessage());
	 }
	  m_pRecordset->Close();
	 m_pConnection->Close();
	
	return str_FilePrefix;

}
 /*
 Author:	Alex
 Date: 2012-11-6 
 Function:
判断设备的名字和选择的文件是不是相符合
有三种情况
1.符合
2.不符合
3.数据库中不存在
 */
int CISPDlg::Judge_Model_Version()
{
	CString File_Prefix=GetFilePrefix_FromDB(m_ModelName);
	CStringArray strArry;
	//char div='\\';
	//decodeCString(m_strHexFileName,strArry,div);
    CString flashfileName=GetFileName_FromFilePath();//strArry[strArry.GetCount()-1];
	flashfileName.MakeUpper();
	File_Prefix.MakeUpper();
	if (File_Prefix.IsEmpty())
	{
		return NO_HEXFILEPREFIX;
	}
	else if (flashfileName.Find(File_Prefix)!=-1) //字符串查找
    {
		return OK_HEXFILEPREFIX;
    } 
    else
    {
		return ERR_HEXFILEPREFIX;
    }

}



void CISPDlg::OnMenuApp()
{
     CDialog::OnOK();
}


void CISPDlg::OnMenuAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}
 /*
 Author:	Alex
 Date: 2012-11-2 
 Function:
 从用户选择的文件路径中提取文件的名字，
 这样就不会作为无判断
 */
 CString CISPDlg::GetFileName_FromFilePath() 
{
	CStringArray strArry;
	ExtractString(strArry,m_strHexFileName,_T("\\"));
	CString str;
	str.Format(_T("%s"),strArry.GetAt(strArry.GetCount()-1));
	return str;
 }

 void CISPDlg::OnBnClickedShowHex()
 {
	 UpdateData();
	 SaveParamToConfigFile();	//保存用户输入的数据


	 if (m_pFileBuffer)
	 {
		 delete[] m_pFileBuffer;
		 m_pFileBuffer= NULL;
	 }

	 CHexFileParser* pHexFile = new CHexFileParser;
	 pHexFile->SetFileName(m_strHexFileName);

	 m_pFileBuffer = new char[c_nHexFileBufLen];
	 memset(m_pFileBuffer, 0xFF, c_nHexFileBufLen);
	 int nDataSize = pHexFile->GetHexFileBuffer(m_pFileBuffer, c_nHexFileBufLen);//获取文件的buffer
	 TS_UC *Databuff=(TS_UC*)m_pFileBuffer;
	 int ii=0;
	 CString aline_hex(_T(""));
	 CString temp;
     if(nDataSize>0)
     { 
         while(ii<nDataSize){
		  aline_hex=_T("");
		 for (int i=ii;i<ii+128;i++)
		 {
		 temp.Format(_T("%02X"),Databuff[i]);
		 aline_hex+=temp;
		// aline_hex.AppendChar(m_pFileBuffer[i]);
		 }
		 aline_hex+=_T("\n");
		 UpdateStatusInfo(aline_hex,FALSE);
		 ii+=128;
		 }
	 }

     
     delete pHexFile;
     pHexFile=NULL;
 }


 void CISPDlg::OnBnClickedFlashSn()
 {
     initFlashSN();
 }

 void CISPDlg::FlashSN(){
     CFlashSN* pFlashSN = new CFlashSN;
     int nComport = GetComPortNo();

     pFlashSN->SetComport(nComport);
     pFlashSN->SetFlashTpye(0);

     if(!GetModbusID(m_szMdbIDs))
     {
         AfxMessageBox(_T("Please input a slave ID "));
         return ;		
     }
     int nHWVerison;
     //	int nProductModel;
     CString  strProductModel;

     int ProductID;

     GetFlashSNParam(nHWVerison, strProductModel,ProductID);
     pFlashSN->SetMBID(m_szMdbIDs[0]);
     pFlashSN->SetFlashParam((static_cast<const CString>(strProductModel)), nHWVerison,ProductID);

     pFlashSN->StartWriteSN();
    CString SN;
    SN.Format(_T("%d"),pFlashSN->m_nSerialNumber); 
    GetDlgItem(IDC_EDIT_SN)->SetWindowText(SN);
     delete pFlashSN;
 }

// void CISPDlg::OnBnClickedButtonSelfile2()
// {
//     UpdateData();
//     //CString strFilter = _T("hex File|*.hex|bin File|*.bin|all File|*.*||");
//     CString strFilter = _T("bin File|*.bin|all File|*.*||");
//     CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
//     dlg.DoModal();
//     m_strFirmwarepath=dlg.GetPathName();
//     // m_strFlashFileName=dlg.GetPathName();
//
//     CWnd* pEditFilePath = (CWnd*)GetDlgItem(IDC_EDIT_FILEPATH2);
//     pEditFilePath->SetWindowText(m_strFirmwarepath);
// }
void CISPDlg::OnMenuSetting()
 {
	 // TODO: Add your command handler code here
	 CISPSetting Dlg;
	 Dlg.DoModal();
}

void CISPDlg::OnMenuCheckhex()
{
	if (m_pFileBuffer)
	{
		delete[] m_pFileBuffer;
		m_pFileBuffer= NULL;
	}
	if(HexFileValidation(m_strHexFileName))
	{
		CHexFileParser* pHexFile = new CHexFileParser;
		pHexFile->SetFileName(m_strHexFileName);

		m_pFileBuffer = new char[c_nHexFileBufLen];
		memset(m_pFileBuffer, 0xFF, c_nHexFileBufLen);
		int nDataSize = pHexFile->GetHexFileBuffer(m_pFileBuffer, c_nHexFileBufLen);//获取文件的buffer
		CString hexinfor=_T("The Hex For ");
		hexinfor+=pHexFile->Get_HexInfor();
		CString strFilesize;
		strFilesize.Format(_T("Hex size=%d Bs"),nDataSize);
		GetDlgItem(IDC_HEX_SIZE)->SetWindowText(strFilesize);

		GetDlgItem(IDC_BIN_INFORMATION)->SetWindowText(hexinfor);
		delete pHexFile;
		pHexFile=NULL;
	} 
	if(BinFileValidation(m_strHexFileName))
	{
		CBinFileParser* pBinFile=new CBinFileParser;
		pBinFile->SetBinFileName(m_strHexFileName);
		m_pFileBuffer=new char[c_nBinFileBufLen];
		memset(m_pFileBuffer, 0xFF, c_nBinFileBufLen);

		int nDataSize=pBinFile->GetBinFileBuffer(m_pFileBuffer,c_nBinFileBufLen);

		CString strFilesize;
		strFilesize.Format(_T("Bin size=%d Bs"),nDataSize);
		GetDlgItem(IDC_HEX_SIZE)->SetWindowText(strFilesize);
		m_strASIX=pBinFile->m_strASIX;
		m_strProductName=pBinFile->m_strProductName;

		if (m_strASIX.Find(_T("ASIX"))!=-1)
		{


			CString hexinfor=_T("The Bin For ");
			hexinfor.Format(_T("The Bin file is for the firmware of %s"),m_strProductName.GetBuffer());
			GetDlgItem(IDC_BIN_INFORMATION)->SetWindowText(hexinfor);


		}
		else
		{
			CString hexinfor=_T("The Bin For ");
			hexinfor.Format(_T("The Bin file is for the bootloader of %s"),m_strProductName.GetBuffer());
			GetDlgItem(IDC_BIN_INFORMATION)->SetWindowText(hexinfor);



		}
	}
}

DWORD WINAPI GetFWFileProc(LPVOID lPvoid)
{
	return 0;
}
