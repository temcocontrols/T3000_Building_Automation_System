
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
//	try
///	{
		m_bHiColorIcons = TRUE;
//	}
// 	catch (...)
// 	{
// 		
// 		AfxMessageBox(_T("1111"));
// 	}


	
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

/*
FunctionName:RegisterOcx

Comment:Alex
Date:2012-11-29
Purpose:这个函数的作用，获取控件的位置，注册到注册表中
也可以通过Bat文件注册
*/
BOOL CT3000App::RegisterOcx(LPCTSTR   OcxFileName)
{
	LPCTSTR   pszDllName   =   OcxFileName   ;     //ActiveX控件的路径及文件名       
	HINSTANCE   hLib   =   LoadLibrary(pszDllName);   //装载ActiveX控件   
	if   (hLib   <   (HINSTANCE)HINSTANCE_ERROR)   
	{   
		return   FALSE;   
	}   
	FARPROC   lpDllEntryPoint;     
	lpDllEntryPoint   =   GetProcAddress(hLib,(LPCSTR)(_T("DllRegisterServer")));   //获取注册函数DllRegisterServer地址   
	if(lpDllEntryPoint!=NULL)      
	 //调用注册函数DllRegisterServer   
	{   
		if(FAILED((*lpDllEntryPoint)()))   
		{
		//   AfxMessageBox(_T("false"));
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
/*
FunctionName:InitInstance

Comment:Alex
Date:2012-11-29
Purpose:这个是T3000的入口函数，从这里开始初始化，
然后进入T3000的MainFrm界面
*/
BOOL CT3000App::InitInstance()//Alex-
{
	try
	{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
 	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

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


		  days =0 /*GetSoftInstallDays()*/;
	
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
	/*else
	{
	AfxMessageBox(_T("Error code: 102 \nSystem closed!"));
	return FALSE;
	}*/
#endif

	CWinAppEx::InitInstance();
	HRESULT hr;//



	 
	if (!AfxSocketInit())//
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);//
		return FALSE;//
	}
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

#if 1	
	try
	{

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
	//GOOD==GOOD
		CopyFile(g_strOrigDatabaseFilePath,g_strDatabasefilepath,FALSE);//
	}//
	else//
	{
	
	}
	FindClose(hFind);//

	g_strDatabasefilepath=(CString)FOR_DATABASE_CONNECT+g_strDatabasefilepath;//
	g_strImgeFolder=g_strExePth+_T("Database\\image\\");//
	CreateDirectory(g_strImgeFolder,NULL);//
	
	//CString strocx=g_strExePth+_T("MSFLXGRD.OCX");


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


	}
	catch (...)
	{
		AfxMessageBox(_T("Database operation to stop!"));

	}
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
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

	//cmdInfo.m_nShellCommand   =   CCommandLineInfo::FileNothing; //lsc


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	
	GdiplusStartupInput gdiplusStartupInput;//
	GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);//
#if 1
//这个活动片段的目的：初始g_bPrivilegeMannage的值
//当是否有用户登录到系统
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
		else
		{
			

		}
		
	}

#endif

  	
	((CMainFrame*)m_pMainWnd)->InitViews();//

	m_pMainWnd->SetWindowText(_T("T3000 Building Automation System"));//
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	//::ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\Temcocontrols\\T3000\\REG_msado15.dll.bat"), _T(""), _T(""), SW_SHOW);
	//vcredist_x86.zip

	//	::ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\Temcocontrols\\T3000\\vcredist_x86.zip"), _T(""), _T(""), SW_SHOW);
	//这个要先试试，当电脑没有安装这个文件时，如何捕获这个信息，然后再执行这个。
	//AfxMessageBox(_T("Open'T3000'Again,Please!"));

	}
	catch (...)
	{


	//	AfxMessageBox(_T("Double click 'REG_msado15.dll',Please!\nAt C:\\Program Files\\Temcocontrols\\T3000"));
// 		CString strFilter = _T("hex File;bin File|*.hex;*.bin|all File|*.*||");
// 		CFileDialog dlg(true,_T("hex"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
// 		dlg.DoModal();

//		CFileDialog dlg(true,NULL,_T("C:\Program Files\Common Files\System\ado"));
		//dlg.lpstrInitialDir = "..\\hisdata";
		//dlg.op
////
	//		OPENFILENAME

		
//		if (dlg.DoModal()==IDOK)
//		{
// 			path = dlg.GetPathName();
// 			pLogFile = fopen("Log.txt", "wt+");   
// 			fprintf(pLogFile, "%s", (LPCSTR)path); 
// 			fclose(pLogFile);
// 			pLogFile = NULL; 
//		}


// 		CFileDialog fileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
// 			NULL,NULL);//_T("工作表(*.xls)|*.xls|文本文件(*.txt)|*.txt||")
// 		fileDlg.m_ofn.lpstrInitialDir = _T("C:\\Program Files\\Temcocontrols\\T3000\\REG_msado15.dll.bat");
// 		fileDlg.DoModal();



		::ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\Temcocontrols\\T3000\\REG_msado15.dll.bat"), _T(""), _T(""), SW_SHOW);
		//vcredist_x86.zip
		
	//	::ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\Temcocontrols\\T3000\\vcredist_x86.zip"), _T(""), _T(""), SW_SHOW);
		//这个要先试试，当电脑没有安装这个文件时，如何捕获这个信息，然后再执行这个。
		AfxMessageBox(_T("Open'T3000'Again,Please!"));


		return TRUE;

	}


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

	//Black 
}

void CT3000App::SaveCustomState()
{
	//Black 
	 
}
/*
FunctionName:InitModeName
Comment:Alex
Date:2012/11/29
Purpose:
初始化设备的名字
*/
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
//如果使用天数为0的话.就弹出让用户注册
if (m_maxClients == 0)
		return FALSE;
	else 
		return TRUE;
}
/*
FunctionName:GetModulePath
Comment:Alex
Date:2012/11/29
Purpose:
这个函数的作用是获取可执行文件的路径
*/
CString CT3000App::GetModulePath()
{
	wchar_t path[MAX_PATH];

	GetModuleFileName(NULL,path,MAX_PATH);
	return CString(path);
}
/*
FunctionName:GetSoftInstallDays
Comment:Alex
Date:2012/11/29
Purpose:
获取客户端该版本T3000，距当天的天数
*/
int CT3000App::GetSoftInstallDays()
{
	CFileStatus fileStatus;
	CTime curTime;
	CString strPath;
	CTimeSpan timeSpan;

	curTime=CTime::GetCurrentTime();
	strPath=GetModulePath();

	strPath=strPath.Left(strPath.ReverseFind(_T('\\'))+1);
	//strPath+=_T("T3000_Rev2.5.0.99.exe");
	strPath+=_T("T3000.exe");
//	strPath+=_T("T3000.exe");
	CFile::GetStatus(strPath,fileStatus);

	//timeSpan=curTime-fileStatus.m_ctime;
	timeSpan=curTime-fileStatus.m_mtime;//这个是修改时间，不论你CUT，COPY都不会修改的

	return timeSpan.GetDays();
}
/*
FunctionName:WriteNumber
Comment:Alex
Date:2012/11/29
Purpose:
向一个指定注册表项里面写一个值
*/
void CT3000App::WriteNumber( CRegKey& key,CStringW valueName,DWORD value )
{
	key.SetDWORDValue(valueName,value);
}
/*
FunctionName:ReadNameber
Comment:Alex
Date:2012/11/29
Purpose:
读一个值，从指定注册表项目
*/
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
/*
FunctionName:Judgestore
Comment:Alex
Date:2012/11/29
Purpose:判断 HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows NT\\CurrentVersion\\KEY_READ
查看是否有改项目
没有创建，并写入一个定值到该表项中
*/
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
/*
FunctionName:ReadREG
Comment:Alex
Date:2012/11/29
Purpose:
从一个注册表中读取相应的值给类中的成员变量

*/
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

