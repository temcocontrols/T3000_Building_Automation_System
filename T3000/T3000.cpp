
// T3000.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "T3000.h"
#include "MainFrm.h"

#include "T3000Doc.h"
#include "T3000View.h"
#include "T3000TableView.h"

#include "LoginDlg.h"

#include "iniFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "global_variable.h"
ULONG_PTR g_gdiplusToken;
BEGIN_MESSAGE_MAP(CT3000App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CT3000App::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_SAVE_CONFIG, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_VERSIONHISTORY,OnVersionInfo)
END_MESSAGE_MAP()	
// CT3000App construction
CT3000App::CT3000App()
{
	m_bHiColorIcons = TRUE;
	
}
// The one and only CT3000App object
CT3000App theApp;


BOOL CT3000App::user_login()
{
	BOOL bRet=FALSE;
	CLoginDlg dlg(bRet);
	dlg.DoModal();
	if(g_buser_log_in==false)
		return false;
	return true;
}
// CT3000App initialization
BOOL CT3000App::RegisterOcx(LPCTSTR   OcxFileName)
{
	LPCTSTR   pszDllName   =   OcxFileName   ;     
	HINSTANCE   hLib   =   LoadLibrary(pszDllName);   
	if   (hLib   <   (HINSTANCE)HINSTANCE_ERROR)   
	{   
		return   FALSE;   
	}   
	FARPROC   lpDllEntryPoint;     
	lpDllEntryPoint   =   GetProcAddress(hLib,(LPCSTR)(_T("DllRegisterServer")));  
	if(lpDllEntryPoint!=NULL)      
	{   
		if(FAILED((*lpDllEntryPoint)()))   
		{//   AfxMessageBox(_T("false"));
			FreeLibrary(hLib);   
			return   FALSE;   
			
		}   
		return   TRUE;   
	}   
	else   
	{
	//	AfxMessageBox(_T("false"));
			return   FALSE;


	}
	
}
BOOL CT3000App::InitInstance()
{

 	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	 

	CWinAppEx::InitInstance();
	HRESULT hr;//
/********************************************************************
	created:	2012/11/27
	created:	27:11:2012   13:29
	filename: 	T3000\T3000\T3000.cpp
	file path:	T3000\T3000
	file base:	T3000
	file ext:	cpp
	author:		Alex
	
	purpose:	使用是1 不使用设置成0
*********************************************************************/


	Judgestore();
	ReadREG();
 

#if 0

	// 检测注册信息

	//Here is for Register information.
	if(haveRegister())
	{
		int days = GetSoftInstallDays();
		// 		 		if(days<=30&&days>=20)
		// 		 		{
		// 		 			CString msg;
		// 		 			msg.Format(_T("please update new version on the website"));
		// 		 			AfxMessageBox(msg);
		// 		 		}
		// 		 		else if(days>30)
		// 		 		{
		// 		 			AfxMessageBox(_T("please update new version on the website"));
		// 		 			return FALSE;
		// }

		CString strDays;
		strDays.Format(_T("%d"),days);
		AfxMessageBox(strDays);

		if(days>160)
		{
			AfxMessageBox(_T("please update new version on the website!"));
			return FALSE;

		}


		//int days = GetSoftInstallDays();

		if (((m_maxClients-days)<=5)&&(days<=m_maxClients))
		{
			CString msg;
			if (days != m_maxClients)
			{
				msg.Format(_T("Contact Software Developer in %d days,Please."),m_maxClients-days);
				AfxMessageBox(msg);
			}else
			{
				msg.Format(_T("Contact Software Developer in today,Please."));
				AfxMessageBox(msg);
			}
		}
		else if (days>m_maxClients)
		{
			CRegKey key;
			LPCTSTR data_Set =_T("Software\\Microsoft\\Windows NT\\CurrentVersion\\");//_T("Software\\Microsoft\\"); //
			key.Create(HKEY_LOCAL_MACHINE,data_Set);
			WriteNumber(key,_T("maxClients"),0);
			AfxMessageBox(_T("Error code: 101 \nSystem closed!"));
			return FALSE;
		}


	}
	else
	{
		AfxMessageBox(_T("Error code: 102 \nSystem closed!"));
		return FALSE;
	}
#endif



	if (!AfxSocketInit())//初始化socket
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);//
		return FALSE;//
	}
	// Initialize OLE libraries
	if (!AfxOleInit())//
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

#if 1	
	TCHAR exeFullPath[MAX_PATH+1]; //
	GetModuleFileName(NULL, exeFullPath, MAX_PATH); //
	(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;//
	g_strDatabasefilepath=exeFullPath;//
	g_strExePth=g_strDatabasefilepath;//
	CreateDirectory(g_strExePth+_T("Database"),NULL);//creat database folder;//
	g_strOrigDatabaseFilePath=g_strExePth+_T("t3000.mdb");//
	g_strDatabasefilepath+=_T("Database\\t3000.mdb");//


	HANDLE hFind;//
	WIN32_FIND_DATA wfd;//
	hFind = FindFirstFile(g_strDatabasefilepath, &wfd);//
	if (hFind==INVALID_HANDLE_VALUE)//
	{
		CopyFile(g_strOrigDatabaseFilePath,g_strDatabasefilepath,FALSE);//
	}//
	else//
	{
	
	}
	FindClose(hFind);//

	g_strDatabasefilepath=(CString)FOR_DATABASE_CONNECT+g_strDatabasefilepath;//
	g_strImgeFolder=g_strExePth+_T("Database\\image\\");//
	CreateDirectory(g_strImgeFolder,NULL);//
	
	 CString strocx=g_strExePth+_T("MSFLXGRD.OCX");
	 RegisterOcx(strocx);
	InitModeName();//
#endif

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Temco T3000 Application"));//
//	LoadStdProfileSettings(12);  // Load standard INI file options (including MRU)//
	InitContextMenuManager();
	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams); 
#if 1
	hr=CoInitialize(NULL);//
	if(FAILED(hr)) 	//
	{
		AfxMessageBox(_T("Error Initialize the COM Interface"));//
		return FALSE;//
	}

#endif




	 //Register the application's document templates.  Document templates
	 // serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CT3000Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CT3000View));




	if (!pDocTemplate)
		return FALSE;


	


	AddDocTemplate(pDocTemplate);
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;	
	ParseCommandLine(cmdInfo);

//	cmdInfo.m_nShellCommand   =   CCommandLineInfo::FileNothing; //lsc


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	
	GdiplusStartupInput gdiplusStartupInput;//
	GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);//
#if 1
////////////////////////////////////////////////////////
	_ConnectionPtr m_pCon;
	_RecordsetPtr m_pRs;
	m_pCon.CreateInstance(_T("ADODB.Connection"));
	hr=m_pRs.CreateInstance(_T("ADODB.Recordset"));
	if(FAILED(hr)) 	
	{
	    AfxMessageBox(_T("Load msado12.dll erro"));
	      return FALSE;
	}
	m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	m_pRs->Open(_T("select * from Userlogin"),_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);		
	int nRecord=m_pRs->GetRecordCount();
	if (nRecord<=0)
	{
		g_bPrivilegeMannage=FALSE;
	}
	else
	{
		int nUse;
		_variant_t temp_variant;
		temp_variant=m_pRs->GetCollect("USE_PASSWORD");//
		if(temp_variant.vt!=VT_NULL)
			nUse=temp_variant;
		else
			nUse=0;
		if(nUse==-1)
		{
			g_bPrivilegeMannage=TRUE;
		}
		else
		{
			g_bPrivilegeMannage=FALSE;
		}
	}
	m_pRs->Close();
	m_pCon->Close();

	if (g_bPrivilegeMannage)
	{//for just quick debug,only on this computer
		if(!user_login())
		{
			AfxMessageBox(_T("Error password!"));	
			return false;
		}
		
	}

#endif

  	((CMainFrame*)m_pMainWnd)->InitViews();//
 // 	// The one and only window has been initialized, so show and update it
   	m_pMainWnd->SetWindowText(_T("T3000 Building Automation System"));//
  	m_pMainWnd->ShowWindow(SW_SHOW);
  	m_pMainWnd->UpdateWindow();




	return TRUE;
}
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CAboutDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

// App command to run the dialog
void CT3000App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CT3000App customization load/save methods

void CT3000App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CT3000App::LoadCustomState()
{
}

void CT3000App::SaveCustomState()
{
}
void CT3000App::InitModeName()
{
	CString strInfoFile=g_strExePth+_T("CustomInfo.ini");
	HANDLE hFile;
	WIN32_FIND_DATA fData;
	hFile = FindFirstFile(strInfoFile,&fData);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		//m_strCompany=_T("Temco Controls LTD.");
		//m_strWebLinker=_T("http://www.temcocontrols.com");
		g_strTstat5a=_T("Tstat5A");
		g_strTstat5b=_T("Tstat5B");
		g_strTstat5b2=_T("Tstat5B2");
		g_strTstat5c=_T("Tstat5C");
		g_strTstat5d=_T("Tstat5D");
		g_strnetWork=_T("NC");
		g_strTstat5f=_T("Tstat5F");
		g_strTstat5g=_T("Tstat5G");
		g_strTstat5e=_T("Tstat5E");
		g_strTstat5h=_T("Tstat5H");
		g_strTstat6=_T("Tstat6");
		g_strTstat7=_T("Tstat7");
		g_strPressure=_T("Pressure");
		g_strOR485=_T("OR485");

	}
	else
	{
		CIniFile file(strInfoFile);
		g_strTstat5a = file.GetProfileString(_T("Section 3"),_T("Product1A"),_T("Tstat5A"));
		g_strTstat5b = file.GetProfileString(_T("Section 4"),_T("Product1B"),_T("Tstat5B"));
		g_strTstat5b2 = file.GetProfileString(_T("Section 5"),_T("Product1B2"),_T("Tstat5B2"));
		g_strTstat5c=file.GetProfileString(_T("Section 6"),_T("Product1C"),_T("Tstat5C"));
		g_strTstat5d=file.GetProfileString(_T("Section 7"),_T("Product1D"),_T("Tstat5D"));
		g_strnetWork=file.GetProfileString(_T("Section 8"),_T("NetworkController"),_T("NC"));		
		g_strTstat5f=file.GetProfileString(_T("Section 9"),_T("Product1F"),_T("Tstat5F"));
		g_strTstat5g=file.GetProfileString(_T("Section 10"),_T("Product1G"),_T("Tstat5G"));
		g_strTstat5e=file.GetProfileString(_T("Section 11"),_T("Product1E"),_T("Tstat5E"));
		g_strTstat5h=file.GetProfileString(_T("Section 12"),_T("Product1H"),_T("Tstat5H"));

// 		g_strTstat6=file.GetProfileString(_T("Section 13"),_T("Product6"),_T("Tstat6"));
// 		g_strTstat7=file.GetProfileString(_T("Section 14"),_T("Product7"),_T("Tstat7"));
	}
	FindClose(hFile);
}
// CT3000App message handlers
void CAboutDlg::OnBnClickedButton1()
{
	CString m_strWebLinker=_T("http://www.temcocontrols.com");
	ShellExecute(GetSafeHwnd(), NULL,m_strWebLinker,   NULL, NULL,   SW_SHOWNORMAL);
}
int CT3000App::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	CoUninitialize( );
	return CWinAppEx::ExitInstance();
}
void CT3000App::CopyDirectory(CString strSrcPath,CString strDstPath)
{
	SHFILEOPSTRUCT FileOp;
	memset(&FileOp,0,sizeof(FileOp));
	FileOp.hwnd=NULL;   
	FileOp.wFunc=FO_COPY;   
	FileOp.pFrom=strSrcPath;   
	FileOp.pTo=strDstPath;
	FileOp.fFlags=FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_SILENT;
	FileOp.hNameMappings=NULL;
	FileOp.lpszProgressTitle=NULL;
	if(0==SHFileOperation(&FileOp))
		AfxMessageBox(_T("Success"));
	else
		AfxMessageBox(_T("Failed"));
  }
void CT3000App::OnVersionInfo()
{
	CString strHistotyFile=g_strExePth+_T("history.txt");
	ShellExecute(NULL, _T("open"), strHistotyFile, NULL, NULL, SW_SHOWNORMAL);
}

BOOL CT3000App::haveRegister()
{
	// 	CString strPath;
	// 	CFile tempFile;
	// 
	// 	strPath=GetModulePath();
	// 	strPath=strPath.Left(strPath.ReverseFind(_T('\\'))+1);
	// 	strPath+=_T("77984329");
	// 
	// 	if(!tempFile.Open(strPath,CFile::modeRead))
	// 		return FALSE;
	// 
	// 	tempFile.Close();
	// 	return TRUE;
	//Alex

	if (m_maxClients == 0)
		return FALSE;
	else 
		return TRUE;
}

CString CT3000App::GetModulePath()
{
	wchar_t path[MAX_PATH];

	GetModuleFileName(NULL,path,MAX_PATH);
	return CString(path);
}

int CT3000App::GetSoftInstallDays()
{
	CFileStatus fileStatus;
	CTime curTime;
	CString strPath;
	CTimeSpan timeSpan;

	curTime=CTime::GetCurrentTime();
	strPath=GetModulePath();

	strPath=strPath.Left(strPath.ReverseFind(_T('\\'))+1);
	strPath+=_T("T3000_Rev2.5.0.99.exe");

	//	strPath+=_T("T3000.exe");
	CFile::GetStatus(strPath,fileStatus);

	//timeSpan=curTime-fileStatus.m_ctime;
	timeSpan=curTime-fileStatus.m_mtime;//这个是修改时间，不论你CUT，COPY都不会修改的

	return timeSpan.GetDays();
}

void CT3000App::WriteNumber( CRegKey& key,CStringW valueName,DWORD value )
{
	key.SetDWORDValue(valueName,value);
}

BOOL CT3000App::ReadNameber( CRegKey& key,CStringW valueName,DWORD& value )
{
	DWORD s;
	if (key.QueryDWORDValue(valueName,s) == ERROR_SUCCESS)
	{
		value = s;
		return TRUE;
	}else
	{
		return FALSE;
	}
}

void CT3000App::Judgestore()
{
	HKEY hKEY;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion\\");//_T("Software\\Microsoft\\");//
	long ret0 = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);
	if (ret0!=ERROR_SUCCESS)
	{
		return;
	}

	LPBYTE  owner_Get = new BYTE[80];
	DWORD type_1 = REG_SZ;
	DWORD cbData_1 = 80;

	long ret1 = ::RegQueryValueEx(hKEY,_T("maxClients"),NULL,&type_1,owner_Get,&cbData_1);
	if (ret1!=ERROR_SUCCESS)
	{
		CRegKey key;
		key.Create(HKEY_LOCAL_MACHINE,data_Set);
		WriteNumber(key,_T("maxClients"),4315);

	}
	long ret2 = ::RegQueryValueEx(hKEY,_T("Regvalue"),NULL,&type_1,owner_Get,&cbData_1);
	if (ret2!=ERROR_SUCCESS)
	{
		CRegKey key;
		key.Create(HKEY_LOCAL_MACHINE,data_Set);
		WriteNumber(key,_T("Regvalue"),100);
	}
	delete[] owner_Get;
	::RegCloseKey(hKEY);
}

void CT3000App::ReadREG()
{
	CRegKey key;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion");//_T("Software\\Microsoft\\");//
	key.Open(HKEY_LOCAL_MACHINE,data_Set);
	ReadNameber(key,_T("maxClients"),m_maxClients);
	ReadNameber(key,_T("Regvalue"),password);
	if (password != 100)
	{
		WriteNumber(key,_T("Regvalue"),100);
		password =100;
	}
	if (m_maxClients !=4315&&m_maxClients!=0)
	{
		WriteNumber(key,_T("maxClients"),4315);
		m_maxClients = 15;

	}else if (m_maxClients == 0)
	{
		m_maxClients = 0;
	}else 
	{
		m_maxClients = m_maxClients - 4300;
		if (m_maxClients>password)
		{
			m_maxClients = 0;
		}
	}
}