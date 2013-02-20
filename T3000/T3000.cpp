
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
*/

/* Comment by Jay:
RegisterOcx:
This function is used to register a .dll or library file passed to it. 
Function returns TRUE if it is able to successfully load the file. 
It returns FALSE if it fails to load the file. 
*/

BOOL CT3000App::RegisterOcx(LPCTSTR   OcxFileName)
{
	LPCTSTR   pszDllName   =   OcxFileName   ;

	/* Comment by Jay:
	LoadLibrary - Loads the specified module into the address space of the calling process. 
	The specified module may cause other modules to be loaded. 
	If the function succeeds, the return value is a handle to the module.
	If the function fails, the return value is NULL. To get extended error information, 
	call GetLastError.
	http://social.msdn.microsoft.com/Forums/en-US/vclanguage/thread/1b994ff3-da28-46a8-90ef-6c7fab8dcd21/
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms684175(v=vs.85).aspx
	*/
	HINSTANCE   hLib   =   LoadLibrary(pszDllName);
	
	/* Suggestion by Jay: 
	As per documentation of LoadLibrary API, it returns a handle or NULL. LoadLibrary used to
	return HINSTANCE_ERROR during 16-bit windows era. If this still holds true or not shall be 
	checked by actually calling this function and checking value of hLib variable. 
	*/
	
	/* Comment by Jay:
	Check if library got loaded properly or not? */
	if   (hLib   <   (HINSTANCE)HINSTANCE_ERROR)   
	{   
		return   FALSE;   
	}   

	FARPROC   lpDllEntryPoint;     
	
	/* Comment by Jay: 
	GetProcAddress - Retrieves the address of an exported function or variable from 
	the specified dynamic-link library (DLL).
	If the function succeeds, the return value is the address of the 
	exported function or variable.
	If the function fails, the return value is NULL. To get extended error 
	information, call GetLastError.
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms683212(v=vs.85).aspx
	*/
	/* Comment by Jay: 
	_T macro - will add L as prefix to Unicode builds, hence making code as portable
	as possible. 
	*/
	lpDllEntryPoint   =   GetProcAddress(hLib,(LPCSTR)(_T("DllRegisterServer")));
	
	/* Comment by Jay: 
	Check if entry point function is identified or not? If it is not identified, then
	the library loaded by LoadLibrary function shall be unloaded using FreeLibrary function. 
	*/
	if(lpDllEntryPoint != NULL)      
	{   
		/* Comment by Jay: 
		FAILED - this function is defiend in WinError.h as 
		#define FAILED(hr) (((HRESULT)(hr)) < 0)
		It checks if the return value by the function is negative. If it is,
		FAILED() returns a true value meaning that the function has failed. 
		http://stackoverflow.com/questions/377322/can-someone-explain-the-c-failed-function
		*/
		/* Comment by Jay: 
		If lpDllEntryPoint fails, unload the loaded DLL / library. 
		*/
		if(FAILED((*lpDllEntryPoint)()))
		{
			//   AfxMessageBox(_T("false"));
			/* Comment by Jay: 
			FreeLibrary - frees the loaded DLL module.
			If the function succeeds, the return value is nonzero.
			If the function fails, the return value is zero. 
			To get extended error information, call the GetLastError function.
			http://msdn.microsoft.com/en-us/library/windows/desktop/ms683152(v=vs.85).aspx
			*/
			/* Suggestion by Jay:
			If possible check return value of FreeLibrary also. It returns 
			zero or non-zero values as commented above.
			*/
			FreeLibrary(hLib);   

			/* Comment by Jay:
			Registration of OCX failed. 
			*/
			return   FALSE;   
			
		}
		/* Comment by Jay:
		Registration of OCX succeeded. 
		*/
		return   TRUE;   
	}
	else   
	{
		//	AfxMessageBox(_T("false"));
	
		/* Comment by Jay:
		GetProcAddress returned NULL and hence entry point was not found. 
		Registration of OCX failed. 
		*/
		/* Suggestion by Jay:
		In this case also, check if there is necessity to call FreeLibrary 
		function. If called, in that case, 
		if possible check return value of FreeLibrary also. It returns 
		zero or non-zero values as commented above.
		*/
		return   FALSE;


	}
	
}
/*
FunctionName:InitInstance

Comment:Alex
Date:2012-11-29
*/
BOOL CT3000App::InitInstance()//Alex-
{
	try
	{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.

	/* Comment by Jay:
	INITCOMMONCONTROLSEX structure Carries information used to load 
	common control classes from the dynamic-link library (DLL). 
	This structure is used with the InitCommonControlsEx function.
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb775507(v=vs.85).aspx
	*/
 	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.

	/* Comment by Jay:
	ICC_WIN95_CLASSES loads following:
	Loads animate control, header, hot key, list-view, progress bar, 
	status bar, tab, tooltip, toolbar, trackbar, tree-view, 
	and up-down control classes.
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb775507(v=vs.85).aspx
	*/
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	
	/* Comment by Jay:
	This function nsures that the common control DLL (Comctl32.dll) is loaded, 
	and registers specific common control classes from the DLL. 
	An application must call this function before creating 
	a common control.
	http://msdn.microsoft.com/en-us/library/windows/desktop/bb775697(v=vs.85).aspx
	*/
	/* Suggestion by Jay: 
	Return type (TRUE or FALSE) must be checked to ensure that Common Control
	DLL is loaded successfully. If the same is not checked then there could be 
	issues later on. Also, if it is found that common controls are not 
	loaded properly, then the same can be managed in a more meaningful way at 
	this stage. 
	*/
	InitCommonControlsEx(&InitCtrls);

	/* Comment by Jay:
	This function checks a registries. If the same is not existing, then it creates them.
	*/
	Judgestore();

	/* Comment by Jay:
	This function is used to update m_maxClients and password variables 
	with the values contained in maxClients and Regvalue registries.
	*/
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

	/* Comment by Jay:
	Call InitInstance of parent class. 
	*/
	CWinAppEx::InitInstance();
	HRESULT hr;


	/* Comment by Jay:
	AfxSocketInit - This function is called in CWinApp::InitInstance override to initialize Windows Sockets.
	It returns nonzero if the function is successful; otherwise 0.
	http://msdn.microsoft.com/en-us/library/x12941w5(v=vs.80).aspx
	*/
	if (!AfxSocketInit())
	{
		/* Comment by Jay:
		AfxMessageBox - Displays a message box on the screen.
		Return: Zero if there is not enough memory to display the message box; otherwise nonzero.
		http://msdn.microsoft.com/en-us/library/as6se7cb(v=vs.80).aspx
		*/
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	// Initialize OLE libraries

	/* Comment by Jay:
	AfxOleInit - Initializes OLE support for the application.
	Return: Nonzero if successful; 0 if initialization fails, possibly because incorrect versions of the 
	OLE system DLLs are installed. 
	http://msdn.microsoft.com/en-us/library/e91aseaz(v=vs.80).aspx
	*/
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	/* Comment by Jay:
	AfxEnableControlContainer - Call this function in your application object's InitInstance function 
	to enable support for containment of OLE controls.
	http://msdn.microsoft.com/en-us/library/x1se4y1y(v=vs.80).aspx
	*/
	AfxEnableControlContainer();

#if 1	
	try
	{

	TCHAR exeFullPath[MAX_PATH+1];

	/* Comment by Jay:
	GetModuleFileName - Retrieves the fully qualified path for the file that contains 
	the specified module. The module must have been loaded by the current process.
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms683197(v=vs.85).aspx
	*/
	GetModuleFileName(NULL, exeFullPath, MAX_PATH); 

	/* Comment by Jay:
	_tcsrchr function scans a string for the last occurrence of a character.
	Its parameters are: a null-terminated string to search, and character to be located.
	It returns a pointer to the last occurrence of ChartoLocate in string, or NULL 
	if ChartoLocate is not found. 
	http://rosasm.freeforums.org/tcsrchr-t78.html

	So, following statement is used to add NULL to the end of exeFullPath string. 
	This is because array a[1] internally expands like *(a + 1). 
	Here, a is the pointer returned by _tcsrchr. +1 is done so that NULL character
	is added next to the last character. 
	*/
	(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
	g_strDatabasefilepath=exeFullPath;
	g_strExePth=g_strDatabasefilepath;


	/* Comment by Jay:
	CreateDirectory - Creates a new directory. If the underlying file system supports security 
	on files and directories, the function applies a specified security descriptor to 
	the new directory.
	If the function succeeds, the return value is nonzero.
	Possible errors include the following:
	ERROR_ALREADY_EXISTS
	ERROR_PATH_NOT_FOUND
	http://msdn.microsoft.com/en-us/library/windows/desktop/aa363855(v=vs.85).aspx
	*/
	/* Suggestion by Jay:
	Here, in the following statement, its return type shall be checked for above mentioned
	errors. This will also result in handling things properly in a nice way.
	*/
	CreateDirectory(g_strExePth+_T("Database"),NULL);

	g_strOrigDatabaseFilePath=g_strExePth+_T("t3000.mdb");
	g_strDatabasefilepath+=_T("Database\\t3000.mdb");


	HANDLE hFind;
	WIN32_FIND_DATA wfd;

	/* Comment by Jay:
	FindFirstFile - Searches a directory for a file or subdirectory with a name that matches 
	a specific name (or partial name if wildcards are used).
	If the function succeeds, the return value is a search handle used in a subsequent 
	call to FindNextFile or FindClose. Second parameter contains information about 
	the first file or directory found.
	http://msdn.microsoft.com/en-us/library/windows/desktop/aa364418(v=vs.85).aspx
	*/
	hFind = FindFirstFile(g_strDatabasefilepath, &wfd);
	if (hFind==INVALID_HANDLE_VALUE)
	{
	//GOOD==GOOD
		/* Comment by Jay:
		CopyFile - This function copies an existing file to a new file. 
		http://msdn.microsoft.com/en-us/library/windows/desktop/aa363851(v=vs.85).aspx

		Since the third argument is FALSE, this function will overwrite already existing 
		file, if any. 
		*/
		CopyFile(g_strOrigDatabaseFilePath,g_strDatabasefilepath,FALSE);
	}
	else
	{
	
	}
	/* Comment by Jay:
	FindClose - Closes a file search handle. 
	http://msdn.microsoft.com/en-us/library/windows/desktop/aa364413(v=vs.85).aspx
	*/
	FindClose(hFind);

	g_strDatabasefilepath=(CString)FOR_DATABASE_CONNECT+g_strDatabasefilepath;
	g_strImgeFolder=g_strExePth+_T("Database\\image\\");
	CreateDirectory(g_strImgeFolder,NULL);
	
	//CString strocx=g_strExePth+_T("MSFLXGRD.OCX");


	InitModeName();



#endif

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Temco T3000 Application"));
//	LoadStdProfileSettings(12);  // Load standard INI file options (including MRU)//
	InitContextMenuManager();
	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams); 
#if 1
	hr=CoInitialize(NULL);
	if(FAILED(hr))
	{
		AfxMessageBox(_T("Error Initialize the COM Interface"));
		return FALSE;
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

	
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);
#if 1
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
		temp_variant=m_pRs->GetCollect("USE_PASSWORD");
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

	::ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\Temcocontrols\\T3000\\REG_msado15.dll.bat"), _T(""), _T(""), SW_SHOW);
	//vcredist_x86.zip

	//	::ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\Temcocontrols\\T3000\\vcredist_x86.zip"), _T(""), _T(""), SW_SHOW);
	//这个要先试试，当电脑没有安装这个文件时，如何捕获这个信息，然后再执行这个。
	//AfxMessageBox(_T("Open'T3000'Again,Please!"));

	}
	catch (...)
	{
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

/* Comment by Jay:
This function deals with .ini file present in the directory. 
The name of the ini file is CustomInfo.ini. 
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
/* Comment by Jay:
This function is used to set a DWORD value of the key passed to it. 
*/
void CT3000App::WriteNumber( CRegKey& key,CStringW valueName,DWORD value )
{
	/* Comment by Jay:
	This method sets the DWORD value of the registry key.
	http://msdn.microsoft.com/en-us/library/1y3z5z87(v=vs.80).aspx
	*/
	key.SetDWORDValue(valueName,value);
}
/*
FunctionName:ReadNameber
Comment:Alex
Date:2012/11/29
Purpose:
读一个值，从指定注册表项目
*/

/* Comment by Jay:
This is a user defined function to read value of registry key
*/
BOOL CT3000App::ReadNameber( CRegKey& key,CStringW valueName,DWORD& value )
{
	DWORD s;

	/* Comment by Jay:
	QueryDWORDValue - Call this method to retrieve the DWORD data for a specified value name.
	http://msdn.microsoft.com/en-us/library/5sts2k7t(v=vs.80).aspx
	*/
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

/* Comment by Jay:
Check if maxClients and RegValue registry keys are existing or not? If they are 
existing, then simply return. Else create them with specified value and return.
*/
void CT3000App::Judgestore()
{
	HKEY hKEY;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion\\");
	/* Comment by Jay:
	RegOpenKeyEx - Opens the specified registry key. 
	If the function succeeds, the return value is ERROR_SUCCESS.
	If the function fails, the return value is a nonzero error code defined in Winerror.h.
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms724897(v=vs.85).aspx
	*/
	long ret0 = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_READ,&hKEY);

	/* Comment by Jay:
	Return if RegOpenKeyEx fails to open specified registry. 
	*/
	if (ret0!=ERROR_SUCCESS)
	{
		return;
	}

	LPBYTE  owner_Get = new BYTE[80];
	DWORD type_1 = REG_SZ;
	DWORD cbData_1 = 80;

	/* Comment by Jay:
	RegQueryValueEx - Retrieves the type and data for the specified value name associated 
	with an open registry key.
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms724911(v=vs.85).aspx
	*/
	long ret1 = ::RegQueryValueEx(hKEY,_T("maxClients"),NULL,&type_1,owner_Get,&cbData_1);

	/* Comment by Jay:
	If there is no registry with name "maxClients", create a new key.
	*/
	if (ret1!=ERROR_SUCCESS)
	{
		/* Comment by Jay:
		CRegKey - This class provides methods for manipulating entries in the 
		system registry.
		http://msdn.microsoft.com/en-in/library/xka57xy4(v=vs.80).aspx
		*/
		CRegKey key;

		/* Comment by Jay:
		Call this method to create the specified key, if it does not exist as a subkey of hKeyParent.
		*/
		key.Create(HKEY_LOCAL_MACHINE,data_Set);
		/* Comment by Jay:
		This function is used to assign value to the given registry name. It is a user-defined function. 
		*/
		WriteNumber(key,_T("maxClients"),4315);

	}

	/* Comment by Jay:
	All comments as above apply here also. 
	*/
	long ret2 = ::RegQueryValueEx(hKEY,_T("Regvalue"),NULL,&type_1,owner_Get,&cbData_1);
	if (ret2!=ERROR_SUCCESS)
	{
		CRegKey key;
		key.Create(HKEY_LOCAL_MACHINE,data_Set);
		WriteNumber(key,_T("Regvalue"),100);
	}

	/* Comment by Jay:
	Release memory assigned to owner_get variable. 
	*/
	delete[] owner_Get;

	/* Comment by Jay:
	RegCloseKey - Closes a handle to the specified registry key.
	http://msdn.microsoft.com/en-us/library/windows/desktop/ms724837(v=vs.85).aspx
	*/
	::RegCloseKey(hKEY);
}


/*
FunctionName:ReadREG
Comment:Alex
Date:2012/11/29
Purpose:
从一个注册表中读取相应的值给类中的成员变量
*/

/* Comment by Jay:
This function is used to check values of maxClients and Regvalue registries.
And sets values of m_maxClients and password variables. 
There is some conditional checking also. If m_maxClients is greater than password, 
m_maxClients is made 0.
*/
void CT3000App::ReadREG()
{
	CRegKey key;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion");//_T("Software\\Microsoft\\");//
	/* Comment by Jay:
	opens registry key
	*/
	key.Open(HKEY_LOCAL_MACHINE,data_Set);
	/* Comment by Jay:
	User defined function to read value of registry key.
	*/
	/* Suggestion by Jay:
	Spelling of number in ReadNameber can be corrected and new name could be ReadNumber().
	*/
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

