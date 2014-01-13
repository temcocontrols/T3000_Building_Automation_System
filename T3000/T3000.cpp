
// T3000.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "T3000.h"
#include "MainFrm.h"

#include "T3000Doc.h"
#include "T3000View.h"
#include "T3000TableView.h"
#include "DialogCM5_BacNet.h"

#include "LoginDlg.h"

#include "iniFile.h"
#include "afxinet.h"
 const int g_versionNO=201401;
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
	if(lpDllEntryPoint!=NULL)       //调用注册函数DllRegisterServer   
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
 void CT3000App::ImportData(){
  CADO ado;
  ado.OnInitADOConn();
  CString sql=_T("select * from ALL_NODE");
  ado.m_pRecordset=ado.OpenRecordset(sql);
  ALL_NODE temp;
  while(!ado.m_pRecordset->EndOfFile){
	   temp.MainBuilding_Name=ado.m_pRecordset->GetCollect(_T("MainBuilding_Name"));
	   temp.Building_Name=ado.m_pRecordset->GetCollect(_T("Building_Name"));
	   temp.Serial_ID=ado.m_pRecordset->GetCollect(_T("Serial_ID"));
	   temp.Floor_name=ado.m_pRecordset->GetCollect(_T("Floor_name"));
	   temp.Room_name=ado.m_pRecordset->GetCollect(_T("Room_name"));
	   temp.Product_name=ado.m_pRecordset->GetCollect(_T("Product_name"));
	   temp.Product_class_ID=ado.m_pRecordset->GetCollect(_T("Product_class_ID"));
	   temp.Product_ID=ado.m_pRecordset->GetCollect(_T("Product_ID"));
	   temp.Screen_Name=ado.m_pRecordset->GetCollect(_T("Screen_Name"));
	   temp.Bautrate=ado.m_pRecordset->GetCollect(_T("Bautrate"));
	   temp.Background_imgID=ado.m_pRecordset->GetCollect(_T("Background_imgID"));
	   temp.Hardware_Ver=ado.m_pRecordset->GetCollect(_T("Hardware_Ver"));
	   temp.Software_Ver=ado.m_pRecordset->GetCollect(_T("Software_Ver"));
	   temp.Com_Port=ado.m_pRecordset->GetCollect(_T("Com_Port"));
	   temp.EPsize=ado.m_pRecordset->GetCollect(_T("EPsize"));
	   temp.Protocol=ado.m_pRecordset->GetCollect(_T("Protocol"));
      
	   ado.m_pRecordset->MoveNext();

	   m_AllNodes.push_back(temp);
  }
  ado.CloseRecordset();
  ado.CloseConn();
 }



void CT3000App::JudgeDB(){
 CADO ado;
 int versionno=0;
 ado.OnInitADOConn();
 if (ado.IsHaveTable(ado,_T("Version")))//有Version表
 {
   CString sql=_T("Select * from Version");
   ado.m_pRecordset=ado.OpenRecordset(sql);
   ado.m_pRecordset->MoveFirst();
   while (!ado.m_pRecordset->EndOfFile)
   {
     versionno=ado.m_pRecordset->GetCollect(_T("VersionNO"));
	 ado.m_pRecordset->MoveNext();
   }
   ado.CloseRecordset();
  
   
 } 
  ado.CloseConn();

 if (g_versionNO>versionno)//版本过低
 {
     SetPaneString(0,_T("The version of DB is lower,Updating....."));
	 _ConnectionPtr srcConTmp;
	 _RecordsetPtr srcRsTemp;
	 srcConTmp.CreateInstance("ADODB.Connection");
	 srcRsTemp.CreateInstance("ADODB.Recordset");
	 srcConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	 srcRsTemp->Open(_T("select * from ALL_NODE"),_variant_t((IDispatch *)srcConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);		

	 ALL_NODE temp;
	 while(!srcRsTemp->EndOfFile){
		 temp.MainBuilding_Name=srcRsTemp->GetCollect(_T("MainBuilding_Name"));
		 temp.Building_Name=srcRsTemp->GetCollect(_T("Building_Name"));
		 temp.Serial_ID=srcRsTemp->GetCollect(_T("Serial_ID"));
		 temp.Floor_name=srcRsTemp->GetCollect(_T("Floor_name"));
		 temp.Room_name=srcRsTemp->GetCollect(_T("Room_name"));
		 temp.Product_name=srcRsTemp->GetCollect(_T("Product_name"));
		 temp.Product_class_ID=srcRsTemp->GetCollect(_T("Product_class_ID"));
		 temp.Product_ID=srcRsTemp->GetCollect(_T("Product_ID"));
		 temp.Screen_Name=srcRsTemp->GetCollect(_T("Screen_Name"));
		 temp.Bautrate=srcRsTemp->GetCollect(_T("Bautrate"));
		 temp.Background_imgID=srcRsTemp->GetCollect(_T("Background_imgID"));
		 temp.Hardware_Ver=srcRsTemp->GetCollect(_T("Hardware_Ver"));
		 temp.Software_Ver=srcRsTemp->GetCollect(_T("Software_Ver"));
//		 temp.Com_Port=srcRsTemp->GetCollect(_T("Com_Port"));
		 temp.EPsize=srcRsTemp->GetCollect(_T("EPsize"));
		 // temp.Protocol=srcRsTemp->GetCollect(_T("Protocol"));

		 srcRsTemp->MoveNext();

		 m_AllNodes.push_back(temp);
	 }

	 srcRsTemp->Close();
	 if (srcConTmp->State)
	 {srcConTmp->Close();
	 }
	 
	 

	 
	 CString filePath=g_strExePth+_T("Database\\T3000.mdb");
	 DeleteFile(filePath);



	 HANDLE hFind;//
	 WIN32_FIND_DATA wfd;//
	 hFind = FindFirstFile(filePath, &wfd);//
	 if (hFind==INVALID_HANDLE_VALUE)//说明当前目录下无t3000.mdb
	 {
		 //CopyFile(g_strOrigDatabaseFilePath,g_strDatabasefilepath,FALSE);//
		 //没有找到就创建一个默认的数据库
		 filePath=g_strExePth+_T("Database\\T3000.mdb");
		 HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_T3000_DATABASE), _T("DB"));   
		 HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   


		 LPVOID lpExe = LockResource(hGlobal);   
		 CFile file;
		 if(file.Open(filePath, CFile::modeCreate | CFile::modeWrite))    
			 file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
		 file.Close();    
		 ::UnlockResource(hGlobal);   
		 ::FreeResource(hGlobal);
	 }//
	 CString strsql;
	 srcConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

	 try
	 {

		 for (int i=0;i<m_AllNodes.size();i++)
		 {
			 strsql.Format(_T("insert into ALL_NODE (MainBuilding_Name,Building_Name,Serial_ID,Floor_name,Room_name,Product_name,Product_class_ID,Product_ID,Screen_Name,Bautrate,Background_imgID,Hardware_Ver,Software_Ver,EPsize) values('"
				 +m_AllNodes[i].MainBuilding_Name+"','"
				 +m_AllNodes[i].Building_Name+"','"
				 +m_AllNodes[i].Serial_ID+"','"
				 +m_AllNodes[i].Floor_name+"','"
				 +m_AllNodes[i].Room_name+"','"
				 +m_AllNodes[i].Product_name+"','"
				 +m_AllNodes[i].Product_class_ID+"','"
				 +m_AllNodes[i].Product_ID+"','"
				 +m_AllNodes[i].Screen_Name+"','"
				 +m_AllNodes[i].Bautrate+"','"
				 +m_AllNodes[i].Background_imgID+"','"
				 +m_AllNodes[i].Hardware_Ver+"','"
				 +m_AllNodes[i].Software_Ver+"','"
				 +m_AllNodes[i].EPsize+"')"));


			 srcConTmp->Execute(strsql.GetString(),NULL,adCmdText);	
		 }
	 }
	 catch(_com_error *e)
	 {
		 AfxMessageBox(e->ErrorMessage());
	 }

	 if (srcConTmp->State)
	 {srcConTmp->Close();
	 }
	 srcConTmp=NULL;

 }



}
 
BOOL CT3000App::InitInstance()
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

	


	CWinAppEx::InitInstance();
	HRESULT hr;//


	if(!AfxInitRichEdit())
	{
		AfxMessageBox(IDS_INIT_RICHEDIT_ERROR);//
		return FALSE;//
	}

 
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
	g_strOrigDatabaseFilePath=g_strExePth+_T("T3000.mdb");//
	g_strDatabasefilepath+=_T("Database\\T3000.mdb");//

	CString FilePath;
	HANDLE hFind;//
	WIN32_FIND_DATA wfd;//
	hFind = FindFirstFile(g_strDatabasefilepath, &wfd);//
	if (hFind==INVALID_HANDLE_VALUE)//说明当前目录下无t3000.mdb
	{
		//CopyFile(g_strOrigDatabaseFilePath,g_strDatabasefilepath,FALSE);//
		  //没有找到就创建一个默认的数据库
		FilePath=g_strExePth+_T("Database\\T3000.mdb");
		HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_T3000_DATABASE), _T("DB"));   
		HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   


		LPVOID lpExe = LockResource(hGlobal);   
		CFile file;
		if(file.Open(FilePath, CFile::modeCreate | CFile::modeWrite))    
			file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
		file.Close();    
		::UnlockResource(hGlobal);   
		::FreeResource(hGlobal);
	}//
	
	 
	FindClose(hFind);//
	g_strDatabasefilepath=(CString)FOR_DATABASE_CONNECT+g_strDatabasefilepath;//
	g_strImgeFolder=g_strExePth+_T("Database\\image\\");//
	CreateDirectory(g_strImgeFolder,NULL);//

 	JudgeDB();
	//CString strocx=g_strExePth+_T("MSFLXGRD.OCX");
	
	/*CStdioFile file;
	CString versionno;
	file.
	file.SetFilePath(_T("http://www.temcocontrols.com/ftp/software/T3000_Version.txt"));
	file.ReadString(versionno);
	file.Close();*/
	//CFile file;
//	file.Open(_T("http://www.temcocontrols.com/ftp/software/T3000_Version.txt"),modeRead);

 	InitModeName();//

	#if 0
	CInternetSession session;
	CInternetFile *file=NULL;
	try
	{
		INTERNET_PROXY_INFO  proxyinfo;
		proxyinfo.dwAccessType=INTERNET_OPEN_TYPE_PROXY;
		proxyinfo.lpszProxy=_T("192.168.0.4:8080 ");
		proxyinfo.lpszProxyBypass=NULL; 
		if (!session.SetOption(INTERNET_OPTION_PROXY,(LPVOID)&proxyinfo,sizeof(INTERNET_PROXY_INFO)))
		{
			AfxMessageBox(_T("UserName"));
		} 
		CString username=_T("alex");
		if(!session.SetOption(INTERNET_OPTION_PROXY_USERNAME,username.GetBuffer(),username.GetLength()+ 1)){

			AfxMessageBox(_T("UserName"));
		}
		CString password=_T("travel");
		if(!session.SetOption(INTERNET_OPTION_PROXY_PASSWORD,password.GetBuffer(),password.GetLength()+ 1)){

			AfxMessageBox(_T("Password"));
		}
		file=(CInternetFile*)session.OpenURL(_T("www.temcocontrols.com/ftp/software/T3000_Version.txt"));

	}
	catch (CInternetException* e)
	{
		file=NULL;
		e->Delete();

	}
	CString version;

	if (file)
	{
		while(file->ReadString(version)!=NULL){

		}
		AfxMessageBox(version);
	}
   #endif
	 

#endif

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Temco T3000 Application"));//
    LoadStdProfileSettings();  // Load standard INI file options (including MRU)//
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
	 
	
	CString registerfilename;
	 registerfilename=g_strExePth+_T("REG_msado15.bat");
	// ::ShellExecute(NULL, _T("open"), registerfilename.GetBuffer(), _T(""), _T(""), SW_HIDE);
	registerfilename=g_strExePth+_T("REG_MSFLXGRD.bat");
	//::ShellExecute(NULL, _T("open"), registerfilename.GetBuffer(), _T(""), _T(""), SW_HIDE);
	CString languagepath=g_strExePth+_T("\\Language");
	m_locale.AddCatalogLookupPath(languagepath);
	m_locale.SetLanguage(CLanguageLocale::LANGUAGE_ENGLISH);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views

#ifndef Fance_Enable_Test
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CT3000Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CT3000View));
#endif

#ifdef Fance_Enable_Test
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CT3000Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDialogCM5_BacNet));
#endif




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

   	m_pMainWnd->SetWindowText(_T("T3000 Building Automation System"));//
  	m_pMainWnd->ShowWindow(SW_SHOW);
  	m_pMainWnd->UpdateWindow();
     

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



		//::ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\Temcocontrols\\T3000\\REG_msado15.dll.bat"), _T(""), _T(""), SW_SHOW);
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
	afx_msg void OnBnClickedOk();
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
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
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
		g_strHumChamber=_T("HumChamber");
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

	return (int)timeSpan.GetDays();
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

int CT3000App::GetLanguage(){
	CRegKey key;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion");//_T("Software\\Microsoft\\");//
	key.Open(HKEY_LOCAL_MACHINE,data_Set);
	DWORD language;
	ReadNameber(key,_T("T3000"),language);
	return language;

}

void CT3000App::SetLanguage(DWORD Last){
	CRegKey key;
	LPCTSTR data_Set = _T("Software\\Microsoft\\Windows NT\\CurrentVersion");//_T("Software\\Microsoft\\");//
	key.Open(HKEY_LOCAL_MACHINE,data_Set);
	WriteNumber(key,_T("T3000"),Last);
}

void CAboutDlg::OnBnClickedOk()
	{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
	}
