
// Modbus Poll.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Modbus Poll.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "Modbus PollDoc.h"
#include "Modbus PollView.h"
#include "CVcWeighting.h"
#include "CVcWall.h"
#include "CVcView3d.h"
#include "CVcValueScale.h"
#include "CVcTitle.h"
#include "CVcTick.h"
#include "CVcTextLayout.h"
#include "CVcStatLine.h"
#include "CVcShadow.h"
#include "CVcSeriesPosition.h"
#include "CVcSeriesMarker.h"
#include "CVcSeriesCollection.h"
#include "CVcSeries.h"
#include "CVcRect.h"
#include "CVcPlotBase.h"
#include "CVcPlot.h"
#include "CVcPen.h"
#include "CVcMarker.h"
#include "CVcLocation.h"
#include "CVcLightSources.h"
#include "CVcLightSource.h"
#include "CVcLight.h"
#include "CVcLegend.h"
#include "CVcLCoor.h"
#include "CVcLabels.h"
#include "CVcLabel.h"
#include "CVcIntersection.h"
#include "CVcFrame.h"
#include "CVcFootnote.h"
#include "CVcFont.h"
#include "CVcFill.h"
#include "CVcDataPoints.h"
#include "CVcDataPointLabel.h"
#include "CVcDataPoint.h"
#include "CVcDataGrid.h"
#include "CVcCoor.h"
#include "CVcColor.h"
#include "CVcCategoryScale.h"
#include "CVcBrush.h"
#include "CVcBackdrop.h"
#include "CVcAxisTitle.h"
#include "CVcAxisScale.h"
#include "CVcAxisGrid.h"
#include "CVcAxis.h"
#include "CVBDataObjectFiles.h"
#include "CVBDataObject.h"
#include "CDMSChart.h"



#include "../SQLiteDriver/CppSQLite3.h"
//#include "global_variable_extern.h"
//#include "global_Function.h"
//#include "ModbusDllforVC.h"
#include <objbase.h>
#include <initguid.h>
#include <Setupapi.h>
 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CModbusPollApp

BEGIN_MESSAGE_MAP(CModbusPollApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CModbusPollApp::OnAppAbout)
	 
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CModbusPollApp construction

CModbusPollApp::CModbusPollApp()
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	//  replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Modbus Poll.AppID.NoVersion"));

	//  add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CModbusPollApp object

CModbusPollApp theApp;


// CModbusPollApp initialization

BOOL CModbusPollApp::InitInstance()
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
	if(!AfxInitRichEdit())
	{
		AfxMessageBox(_T("Initial RichEdit Control Error."));//
		return FALSE;//
	}
	//AfxInitRichEdit2();
	if(!AfxInitRichEdit2())
	{
		AfxMessageBox(_T("Initial RichEdit Control Error."));//
		return FALSE;//
	}
	get_ExePath();
	CString g_strExePth;
	CString g_strImgeFolder;

	TCHAR exeFullPath[MAX_PATH+1]; //
	GetModuleFileName(NULL, exeFullPath, MAX_PATH); 
	(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
	g_strDatabasefilepath=exeFullPath;
	g_strExePth=g_strDatabasefilepath;
	CreateDirectory(g_strExePth+_T("Database"),NULL);//creat database folder;
	g_strDatabasefilepath=g_strExePth+_T("Database\\T3000.db");

	CppSQLite3DB SqliteDBT3000;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
	CString SqlText;
	SqlText.Format(_T("Select * from Building ;"));
	if (!SqliteDBT3000.tableExists("Building"))
	{
		AfxMessageBox(L"Building is not there!");
		return FALSE;
	}
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	//  You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;

	pDocTemplate = new CMultiDocTemplate(IDR_ModbusPollTYPE,
		RUNTIME_CLASS(CModbusPollDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CModbusPollView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();


	
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it

	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
 
	

	return TRUE;
}

int CModbusPollApp::ExitInstance()
{
	// handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CModbusPollApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
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
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CModbusPollApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CModbusPollApp customization load/save methods

void CModbusPollApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CModbusPollApp::LoadCustomState()
{
}

void CModbusPollApp::SaveCustomState()
{
}

// CModbusPollApp message handlers



