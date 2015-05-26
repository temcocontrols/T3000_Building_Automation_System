
// RegisterListManager.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RegisterListManager.h"
#include "RegisterListManagerDlg.h"

#include "global_variable.h"

#include "LoginMode.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRegisterListManagerApp

BEGIN_MESSAGE_MAP(CRegisterListManagerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CRegisterListManagerApp construction

CRegisterListManagerApp::CRegisterListManagerApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CRegisterListManagerApp object

CRegisterListManagerApp theApp;


// CRegisterListManagerApp initialization

BOOL CRegisterListManagerApp::InitInstance()
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

	CWinApp::InitInstance();
    //g_database_path =GetExePath(true)+ _T("\T3000.mdb");
    g_database_operator.SetDBPath(g_database_path); //Set the db path
    g_database_operator.OnInitADOConn();           //Initial DB

	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    CLoginMode logindlg;
    INT_PTR Resp = logindlg.DoModal(); //Login Model
    if (Resp == IDOK)
    {
        CRegisterListManagerDlg dlg;
        m_pMainWnd = &dlg;
        INT_PTR nResponse = dlg.DoModal();
        if (nResponse == IDOK)
        {
            // TODO: Place code here to handle when the dialog is
            //  dismissed with OK
        }
        else if (nResponse == IDCANCEL)
        {
            // TODO: Place code here to handle when the dialog is
            //  dismissed with Cancel
        }
    }
	// Delete the shell manager created above.
     g_database_operator.CloseConn();
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

