
// TestToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestTool.h"
#include "TestToolDlg.h"
#include "MyAboutDlg.h"
#include "afxdialogex.h"

#include "RegisterMonitorDlg.h"
#include "Reg_ControlDlg.h"
#include "Connection.h"

#include "MyOwnListCtrl.h"
#include "Polldefinition.h"
#include "Write_Single_Reg.h"
//#include "Reg_ControlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#pragma comment(lib,"setupapi.lib")	//Enum comport need this lib and Dll.

#include "EnumSerial.h"

#include "ModbusDllforVc.h"


#include "global_variable_extern.h"
#include "globle_function.h"
#include "global_variable.h"



//BOOL GetSerialComPortNumber_my(vector<CString>& szComm);


// CTestToolDlg dialog

CTestToolDlg::~CTestToolDlg()
{
	if(my_readmult!=NULL)
		delete my_readmult;
}


CTestToolDlg::CTestToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

	m_Show_Data_Type = TYPE_UNSIGNED;
	my_readmult = new _MessageReadMultyInfo;
	Main_Read_Thread = NULL;

	
}

void CTestToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODBUS, m_list_control);
}

BEGIN_MESSAGE_MAP(CTestToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(ID_FUNCTION_REGISTERMONITOR, &CTestToolDlg::OnBnClickedRegMonitor)
//	ON_BN_CLICKED(ID_FUNCTION_LCTESTJAT, &CTestToolDlg::OnBnClickedLCTestJat)
	ON_COMMAND(ID_CONNECTION_CONNECT, &CTestToolDlg::OnConnectionConnect)
//	ON_UPDATE_COMMAND_UI(ID_CONNECTION_CONNECT, &CTestToolDlg::OnUpdateConnectionConnect)
ON_COMMAND(ID_CONNECTION_QUICKCONNECT, &CTestToolDlg::OnConnectionQuickconnect)
ON_COMMAND(ID_CONNECTION_DISCONNECT, &CTestToolDlg::OnConnectionDisconnect)

ON_COMMAND(ID_SETUP_POLLDEFINITION, &CTestToolDlg::OnSetupPolldefinition)
ON_COMMAND(ID_DISPLAY_SIGNED, &CTestToolDlg::OnDisplaySigned)
ON_COMMAND(ID_DISPLAY_UNSIGNED, &CTestToolDlg::OnDisplayUnsigned)
ON_COMMAND(ID_DISPLAY_HEX, &CTestToolDlg::OnDisplayHex)
ON_COMMAND(ID_DISPLAY_BINARY, &CTestToolDlg::OnDisplayBinary)
ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipNotify)
ON_WM_TIMER()
ON_COMMAND(ID_SETUP_RESETCOUNTERS, &CTestToolDlg::OnSetupResetcounters)
ON_COMMAND(ID_FUNCTION_READONCE, &CTestToolDlg::OnFunctionReadonce)

ON_MESSAGE(MY_FRESH_LISTCTRL, &CTestToolDlg::FreshListControl)
ON_MESSAGE(MY_OPEN_WRITE_DLG, &CTestToolDlg::OpenWriteDlg)
ON_MESSAGE(MY_SHOW_MSG, &CTestToolDlg::ShowMSG)


ON_COMMAND(ID_FUNCTION_06, &CTestToolDlg::OnFunction06)
ON_COMMAND(ID_FILE_EXIT, &CTestToolDlg::OnFileExit)

ON_NOTIFY(HDN_BEGINTRACK, 0, &CTestToolDlg::OnHdnBegintrackList1) //调整列宽度的消息响应;

ON_COMMAND(ID_FUNCTION_REGISTERMONITOR, &CTestToolDlg::OnFunctionRegistermonitor)
ON_COMMAND(ID_FUNCTION_LCTESTJAT, &CTestToolDlg::OnFunctionLctestjat)
ON_COMMAND(ID_FUNCTION_3, &CTestToolDlg::OnFunction3)
ON_COMMAND(ID_ABOUT_ABOUT, &CTestToolDlg::OnAboutAbout)
ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTestToolDlg message handlers

BOOL CTestToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	// TODO: Add extra initialization here
	if(!m_wndToolBar.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		/*| CBRS_GRIPPER*/ | CBRS_TOOLTIPS /*|CBRS_SIZE_DYNAMIC*/|CBRS_BORDER_TOP) || !m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	InitParameters();
//	((CButton *)GetDlgItem(ID_CONNECTION_DISCONNECT))->ShowWindow(0);

	CRect rcClientOld;
	CRect rcClientNew;
	GetClientRect(rcClientOld);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0,reposQuery,rcClientNew);
	CPoint ptOffset(rcClientNew.left -rcClientOld.left,rcClientNew.top-rcClientOld.top);
	CRect rcClild;
	CWnd * pwndChild = GetWindow(GW_CHILD);
	while(pwndChild)
	{
		pwndChild->GetWindowRect(rcClild);
		ScreenToClient(rcClild);
		rcClild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcClild,FALSE);
		pwndChild = pwndChild->GetNextWindow();
	}

	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right+=rcClientOld.Width()-rcClientNew.Width();
	rcWindow.bottom +=rcClientOld.Height()-rcClientNew.Height();
	MoveWindow(rcWindow,FALSE);

	m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(17,17));
	//m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(54,54));
	//m_wndToolBar.SetButtonText(0,_T("Connect"));
	//m_wndToolBar.SetButtonText(1,_T("Disconnect"));

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);


	
	InitListCtrl();
	InitStateBar();
	Refresh_Menu_And_ToolBar();
	GetSerialComPortNumber_new(m_szComm);
	main_hwnd = this->m_hWnd;
	g_ready_write_ID = g_read_ID;

	CToolBarCtrl     *btn=(CToolBarCtrl*)&m_wndToolBar.GetToolBarCtrl();       
	btn->EnableButton(ID_CONNECTION_DISCONNECT,false);       
	EnableMenuItem(GetMenu()->m_hMenu,ID_CONNECTION_DISCONNECT,MF_GRAYED);

	m_list_control.GetWindowRect(ListRect);
	//ScreenToClient(&ListRect);       // 得到的是在客户区坐标系下的RECT


	hThread =CreateThread(NULL,NULL,Get_All_Dlg_Message,this,NULL, &n_Main_ThreadID);
	hDeal_thread =CreateThread(NULL,NULL,Translate_My_Message,this,NULL, &n_Main_ThreadID_Do);
	PostMessage(MY_FRESH_LISTCTRL,(WPARAM)DO_SUCCESS,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CTestToolDlg::InitParameters()
{
	GetModuleFileName(NULL,Program_Path.GetBuffer(MAX_PATH),MAX_PATH);  //获取当前运行的绝对地址;
	PathRemoveFileSpec(Program_Path.GetBuffer(MAX_PATH) );            //返回绝对地址的上层目录?;
	Program_Path.ReleaseBuffer();
	Program_ConfigFile_Path = Program_Path + _T("\\Config.ini");

	CFileFind fFind;
	if(!fFind.FindFile(Program_ConfigFile_Path))
	{
		WritePrivateProfileStringW(_T("Setting"),_T("IP Address"),_T("192.168.0.100"),Program_ConfigFile_Path);
		WritePrivateProfileStringW(_T("Setting"),_T("IP Port"),_T("6001"),Program_ConfigFile_Path);
		WritePrivateProfileStringW(_T("Setting"),_T("Port"),_T("COM1"),Program_ConfigFile_Path);
		WritePrivateProfileStringW(_T("Setting"),_T("Baudrate"),_T("19200"),Program_ConfigFile_Path);
		WritePrivateProfileStringW(_T("Setting"),_T("Communication Type"),_T("0"),Program_ConfigFile_Path);
		WritePrivateProfileStringW(_T("Setting"),_T("Show Data Type"),_T("0"),Program_ConfigFile_Path);
		WritePrivateProfileStringW(_T("Setting"),_T("Function Type"),_T("2"),Program_ConfigFile_Path);


		WritePrivateProfileStringW(_T("PollDefinition"),_T("SlaveID"),_T("255"),Program_ConfigFile_Path);
		WritePrivateProfileStringW(_T("PollDefinition"),_T("Function"),_T("3"),Program_ConfigFile_Path);
		WritePrivateProfileStringW(_T("PollDefinition"),_T("Start Address"),_T("0"),Program_ConfigFile_Path);
		WritePrivateProfileStringW(_T("PollDefinition"),_T("Read Length"),_T("127"),Program_ConfigFile_Path);
		WritePrivateProfileStringW(_T("PollDefinition"),_T("Scan Time"),_T("1000"),Program_ConfigFile_Path);
		WritePrivateProfileStringW(_T("PollDefinition"),_T("Auto Read"),_T("1"),Program_ConfigFile_Path);
	}
	g_read_ID = GetPrivateProfileInt(_T("PollDefinition"),_T("SlaveID"),255,Program_ConfigFile_Path);
	g_read_function = GetPrivateProfileInt(_T("PollDefinition"),_T("Function"),3,Program_ConfigFile_Path);
	g_read_start_address = GetPrivateProfileInt(_T("PollDefinition"),_T("Start Address"),0,Program_ConfigFile_Path);
	g_read_length = GetPrivateProfileInt(_T("PollDefinition"),_T("Read Length"),127,Program_ConfigFile_Path);
	g_read_interval = GetPrivateProfileInt(_T("PollDefinition"),_T("Scan Time"),1000,Program_ConfigFile_Path);
	g_auto_read = GetPrivateProfileInt(_T("PollDefinition"),_T("Auto Read"),1,Program_ConfigFile_Path);
	
	
	m_function_type=GetPrivateProfileInt(_T("Setting"),_T("Function Type"),2,Program_ConfigFile_Path);
	m_Show_Data_Type = GetPrivateProfileInt(_T("Setting"),_T("Show Data Type"),0,Program_ConfigFile_Path);
	GetPrivateProfileString(_T("Setting"),_T("IP Address"),_T("192.168.0.132"),g_IPAddress.GetBuffer(MAX_PATH),MAX_PATH,Program_ConfigFile_Path);
	g_IPAddress.ReleaseBuffer();
	g_IPPort=GetPrivateProfileInt(_T("Setting"),_T("IP Port"),6001,Program_ConfigFile_Path);
	m_nbaudrat=GetPrivateProfileInt(_T("Setting"),_T("Baudrate"),19200,Program_ConfigFile_Path);
	g_CommunicationType=GetPrivateProfileInt(_T("Setting"),_T("Communication Type"),0,Program_ConfigFile_Path);
	GetPrivateProfileString(_T("Setting"),_T("Port"),_T("COM1"),g_PortDescribe.GetBuffer(MAX_PATH),MAX_PATH,Program_ConfigFile_Path);
	g_PortDescribe.ReleaseBuffer();

	if(g_PortDescribe.CompareNoCase(_T("TCP/IP"))!=0)
	{
		CString temp_cs=g_PortDescribe;
		temp_cs = temp_cs.Right(temp_cs.GetLength()-3);
		g_PortNum = _wtoi(temp_cs);
		if(g_PortNum<0)
			g_PortNum=1;
	}

}

void CTestToolDlg::InitListCtrl()
{
	long style;
	style=GetWindowLong(m_list_control.m_hWnd,GWL_STYLE);
	style&=~LVS_TYPEMASK;
	style|=LVS_REPORT;	
//	style|=LVS_EX_CHECKBOXES;

	m_list_control.SetExtendedStyle(style);
	SetWindowLong(m_list_control.m_hWnd,GWL_STYLE,style);
	DWORD dwstyle=m_list_control.GetExtendedStyle();
	dwstyle|=LVS_EX_FULLROWSELECT; 
	dwstyle|=LVS_EX_GRIDLINES;     
	m_list_control.SetExtendedStyle(dwstyle);
	m_list_control.InsertColumn(0,_T(""),LVCFMT_LEFT,120);  
	m_list_control.InsertColumn(1,_T(""),LVCFMT_LEFT,120);
	m_list_control.InsertColumn(2,_T(""),LVCFMT_LEFT,120);
	m_list_control.InsertColumn(3,_T(""),LVCFMT_LEFT,120);
	m_list_control.InsertColumn(4,_T(""),LVCFMT_LEFT,120);
	m_list_control.InsertColumn(5,_T(" "),LVCFMT_LEFT,120);
	m_list_control.InsertColumn(6,_T(" "),LVCFMT_LEFT,120);
	//SurpportAutoInstall
	//m_list_control.InsertColumn(5,_T("444"),LVCFMT_CENTER,100);
	m_list_control.SetTextColor(RGB(0,0,255));

	//m_list_control.SortItems(0, FALSE); // sort the 1st column, ascending
	m_list_control.SetSortable(FALSE);

	m_list_control.SetGridLines(0);


	g_Tx_Struct.Tx_value=0;

	g_Err_Struct.Err_value = 0;
	g_ID_Struct.ID_value = 0;
	g_SR_Struct.SR_value = 1000;

	g_Tx_Struct.CS_TX=_T("");
	Fresh_TX_ERR_ID(0,0,0,1000,1);

	CString tempcs;
	for (int i=0;i<22;i++)
	{
		tempcs.Format(_T(""),i);
		m_list_control.InsertItem(i,tempcs);
	}
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestToolDlg::Fresh_TX_ERR_ID(int nTX,int nErr,int nID,int nSR,int is_it_auto_read)
{
	g_Tx_Struct.CS_TX.Format(_T("Tx =%5d"),nTX);
	g_Err_Struct.Err_TX.Format(_T("Err =%5d"),nErr);
	g_ID_Struct.ID_TX.Format(_T("ID =%5d"),nID);
	g_SR_Struct.SR_TX.Format(_T("SR =%5d"),nSR);

	m_list_control.SetHeaderText(0,g_Tx_Struct.CS_TX);
	m_list_control.SetHeaderText(1,g_Err_Struct.Err_TX);
	m_list_control.SetHeaderText(2,g_ID_Struct.ID_TX);
	m_list_control.SetHeaderText(3,_T("F = 03"));
	m_list_control.SetHeaderText(4,g_SR_Struct.SR_TX);

	if(is_it_auto_read)
		m_list_control.SetHeaderText(5,_T("( ENABLED )"));
	else
		m_list_control.SetHeaderText(5,_T("( DISABLED )"));
}
void CTestToolDlg::InitStateBar()
{
	cs_tool_version.LoadString(IDS_TOOL_VERSION);
	m_StatusBar = new CStatusBarCtrl;
	m_StatusBar->Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW,CRect(0,0,0,0),this,0);
	int strPartDim[3]={320,580,950};
	m_StatusBar->SetParts(3,strPartDim);
	m_StatusBar->SetText(_T("For Help,call Fance.For edit, double click on a value"),0,0);
	m_StatusBar->SetText(_T("Temcocontrols Test Tool Rev ") + cs_tool_version +_T("  (Fance)"),2,0);
	m_StatusBar->SetText(_T("No connection"),1,0);	



	if(m_Show_Data_Type ==TYPE_SIGNED)
	{
		GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_SIGNED, MF_BYPOSITION|MF_CHECKED);
		GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_UNSIGNED, MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_HEX, MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_BINARY, MF_BYPOSITION|MF_UNCHECKED);
	}
	else if(m_Show_Data_Type == TYPE_UNSIGNED)
	{
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_SIGNED, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_UNSIGNED, MF_BYPOSITION|MF_CHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_HEX, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_BINARY, MF_BYPOSITION|MF_UNCHECKED);
	}
	else if(m_Show_Data_Type == TYPE_HEX)
	{
		GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_SIGNED, MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_UNSIGNED, MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_HEX, MF_BYPOSITION|MF_CHECKED);
		GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_BINARY, MF_BYPOSITION|MF_UNCHECKED);
	}
	else if(m_Show_Data_Type == TYPE_BINARY)
	{
		GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_SIGNED, MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_UNSIGNED, MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_HEX, MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_BINARY, MF_BYPOSITION|MF_CHECKED);

		m_list_control.SetColumnWidth(0,160);
		m_list_control.SetColumnWidth(1,160);
		m_list_control.SetColumnWidth(2,160);
		m_list_control.SetColumnWidth(3,160);
		m_list_control.SetColumnWidth(4,160);
		m_list_control.SetColumnWidth(5,160);
	}

	if(m_function_type == FUNCTION_REGISTER_MONITOR)
	{
		GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_REGISTER_MONITOR, MF_BYPOSITION|MF_CHECKED);
		GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_LC_TEST_JAT, MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_MODBUS_POLL, MF_BYPOSITION|MF_UNCHECKED);
	}
	else if(m_function_type == FUNCTION_LC_TEST_JAT)
	{
		GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_REGISTER_MONITOR, MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_LC_TEST_JAT, MF_BYPOSITION|MF_CHECKED);
		GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_MODBUS_POLL, MF_BYPOSITION|MF_UNCHECKED);
	}
	else if(m_function_type == FUNCTION_MODBUS_POLL)
	{
		GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_REGISTER_MONITOR, MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_LC_TEST_JAT, MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_MODBUS_POLL, MF_BYPOSITION|MF_CHECKED);
	}
}

void CTestToolDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CTestToolDlg::OnBnClickedRegMonitor()
//{
//	// TODO: Add your control notification handler code here
//	this->ShowWindow(0);
//	CRegisterMonitorDlg dlg;
//	dlg.DoModal();
//	this->ShowWindow(1);
//}


//void CTestToolDlg::OnBnClickedLCTestJat()
//{
//	// TODO: Add your control notification handler code here
//	this->ShowWindow(0);
//	CReg_ControlDlg dlg;
//	dlg.DoModal();
//	this->ShowWindow(1);
//}

void CTestToolDlg::Refresh_Menu_And_ToolBar()
{
	if(g_bIs_connected)
	{
		CToolBarCtrl     *btn=(CToolBarCtrl*)&m_wndToolBar.GetToolBarCtrl();       
		btn->EnableButton(ID_CONNECTION_QUICKCONNECT,false);       
		btn->EnableButton(ID_CONNECTION_DISCONNECT,true);     
		EnableMenuItem(GetMenu()->m_hMenu,ID_CONNECTION_QUICKCONNECT,MF_GRAYED);
		EnableMenuItem(GetMenu()->m_hMenu,ID_CONNECTION_CONNECT,MF_GRAYED);
		EnableMenuItem(GetMenu()->m_hMenu,ID_CONNECTION_DISCONNECT,MF_ENABLED);
		my_readmult->device_id =g_read_ID;// g_read_ID;
		my_readmult->length =g_read_length;//g_read_length;
		my_readmult->start_address= g_read_start_address;
		my_readmult->myreaddata=&product_register_value[g_read_start_address];


		if(m_function_type == FUNCTION_REGISTER_MONITOR)
		{
				this->ShowWindow(0);
				CRegisterMonitorDlg dlg;
				dlg.DoModal();
				this->ShowWindow(1);

				OnConnectionDisconnect();
				return;
		}
		else if(m_function_type == FUNCTION_LC_TEST_JAT)
		{
				this->ShowWindow(0);
				CReg_ControlDlg dlg;
				dlg.DoModal();
				this->ShowWindow(1);
				OnConnectionDisconnect();
				return;
		}
		else if(m_function_type == FUNCTION_MODBUS_POLL)
		{
			SetTimer(1,g_read_interval,NULL);
		}

		
		if(!g_auto_read)	//在未连接的情况下 不能点击auto read 和 写单个寄存器;
		{
			EnableMenuItem(GetMenu()->m_hMenu,ID_FUNCTION_READONCE,MF_ENABLED);
			btn->EnableButton(ID_FUNCTION_READONCE,true);
		}
		else
		{
			EnableMenuItem(GetMenu()->m_hMenu,ID_FUNCTION_READONCE,MF_GRAYED);
			btn->EnableButton(ID_FUNCTION_READONCE,false);
		}
		EnableMenuItem(GetMenu()->m_hMenu,ID_FUNCTION_06,MF_ENABLED);	
		btn->EnableButton(ID_FUNCTION_06,true);
	}
	else
	{
		CToolBarCtrl     *btn=(CToolBarCtrl*)&m_wndToolBar.GetToolBarCtrl();       
		btn->EnableButton(ID_CONNECTION_QUICKCONNECT,true);   
		btn->EnableButton(ID_CONNECTION_DISCONNECT,false);  
		EnableMenuItem(GetMenu()->m_hMenu,ID_CONNECTION_QUICKCONNECT,MF_ENABLED);
		EnableMenuItem(GetMenu()->m_hMenu,ID_CONNECTION_CONNECT,MF_ENABLED);
		EnableMenuItem(GetMenu()->m_hMenu,ID_CONNECTION_DISCONNECT,MF_GRAYED);
		KillTimer(1);

		EnableMenuItem(GetMenu()->m_hMenu,ID_FUNCTION_READONCE,MF_GRAYED);	//禁用菜单和快捷栏;
		EnableMenuItem(GetMenu()->m_hMenu,ID_FUNCTION_06,MF_GRAYED);
		btn->EnableButton(ID_FUNCTION_06,false);
		btn->EnableButton(ID_FUNCTION_READONCE,false);
	}
}

void CTestToolDlg::OnConnectionConnect()
{
	// TODO: Add your command handler code here
	CConnection dlg;
	dlg.DoModal();
	Refresh_Menu_And_ToolBar();
}





void CTestToolDlg::OnConnectionQuickconnect()
{
	CString strInfo;
	// TODO: Add your command handler code here
	if(g_CommunicationType==1)
	{
		SetCommunicationType(1);
		BOOL ret=Open_Socket2(g_IPAddress,g_IPPort);	
		if(ret)
		{	
			strInfo.Format((_T("Connect to IP : %s successful")), g_IPAddress);//prompt info;
			g_bIs_connected = true;
		}
		else
		{
			strInfo.Format((_T("Connect to IP : %s failure")), g_IPAddress);//prompt info;
			g_bIs_connected = false;
		}
	}
	else
	{
		SetCommunicationType(0);
		open_com(g_PortNum);//open*************************************
		if(!is_connect())
		{
			g_bIs_connected = false;
			strInfo.Format(_T("COM %d : Not available "), g_PortNum);
			AfxMessageBox(strInfo);
		}
		else
		{
			g_bIs_connected = true;
			strInfo.Format(_T("COM %d Connected: Yes"), g_PortNum);
			Change_BaudRate(m_nbaudrat);
		}	
	}
	m_StatusBar->SetText(strInfo,1,0);
	Refresh_Menu_And_ToolBar();




}


void CTestToolDlg::OnConnectionDisconnect()
{
	// TODO: Add your command handler code here
	close_com();
	g_bIs_connected = false;
	m_StatusBar->SetText(_T("No connection"),1,0);
	Refresh_Menu_And_ToolBar();
}

LRESULT CTestToolDlg::ShowMSG(WPARAM wParam, LPARAM lParam)
{
	CString tempcs;
	wchar_t temp_111[250];
	wchar_t *my111111 = (wchar_t *)lParam;
	
	int mywparam = wParam;
	if(mywparam == 10)
	{
		PostMessage(MY_FRESH_LISTCTRL,(WPARAM)DO_SUCCESS,NULL);
	}

	tempcs.Format(_T("%s"),my111111);
	m_StatusBar->SetText(tempcs,1,0);

	if(my111111)
		delete my111111;
	return 0;
}

LRESULT CTestToolDlg::OpenWriteDlg(WPARAM wParam, LPARAM lParam)
{
	CWrite_Single_Reg Dlg;
	Dlg.DoModal();
	return 0;
}

//此函数用来刷新List
//当用户显示不同的显示方式时，会再次刷新List;
LRESULT CTestToolDlg::FreshListControl(WPARAM wParam, LPARAM lParam)
{
	_MessageReadMultyInfo *m_read_multy = (_MessageReadMultyInfo *)lParam;
	bool _result=false;
	_result = wParam;


	Fresh_TX_ERR_ID(g_Tx_count,g_Error_count,g_read_ID,g_read_interval,g_auto_read);

	if(_result ==false)
	{
		//处理读错的事情;
		return 0;
	}

	//CString tempcs;
	//for (int i=0;i<22;i++)
	//{
	//	tempcs.Format(_T("%05d"),i);
	//	m_list_control.InsertItem(i,tempcs);
	//}
	int i_count=0;

	switch(m_Show_Data_Type)
	{
	case TYPE_SIGNED:
	case TYPE_UNSIGNED:
		for (int col=0;col<6;col++)
		{
			for (int item=0;item<22;item++)
			{
				if(i_count>g_read_length)	//如果大于要读到得item就不显示了;
				{
					m_list_control.SetItemText(item,col,_T(""));
					continue;
				}
				//	return 0;
				i_count++;

				//这里是为了使显示的值右对齐，好看一点。;
				CString ShowText;
				CString temp1;
				ShowText.Format(_T("%05d = "),col*22+item +g_read_start_address/*,product_register_value[col*22+item]*/);
				if(m_Show_Data_Type == TYPE_SIGNED)
				{
					if(product_register_value[col*22+item + g_read_start_address]>0x7fff)
					{
					temp1.Format(_T("%d"),product_register_value[col*22+item + g_read_start_address]-0xffff -1);
					}
					else
					temp1.Format(_T("%d"),product_register_value[col*22+item + g_read_start_address]);
				}
				else
					temp1.Format(_T("%d"),product_register_value[col*22+item + g_read_start_address]);
				int space_count = temp1.GetLength();
				for (int i=0;i<(8-space_count)*2;i++)
				{
					ShowText = ShowText +_T(" ");
				}
				ShowText = ShowText + temp1;
				m_list_control.SetItemText(item,col,ShowText);
			}
		}
		break;
	case TYPE_HEX:
		for (int col=0;col<6;col++)
		{
			for (int item=0;item<22;item++)
			{
				if(i_count>g_read_length)	//如果大于要读到得item就不显示了;
					return 0;
				i_count++;

				//这里是为了使显示的值右对齐，好看一点。;
				CString ShowText;
				CString temp1;
				ShowText.Format(_T("%05d = "),col*22+item +g_read_start_address/*,product_register_value[col*22+item]*/);

				temp1.Format(_T("%04x"),product_register_value[col*22+item + g_read_start_address]);
				temp1 = temp1.MakeUpper();
				temp1 = _T("0x") + temp1;
				int space_count = temp1.GetLength();
				for (int i=0;i<(8-space_count)*2;i++)
				{
					ShowText = ShowText +_T(" ");
				}
				ShowText = ShowText + temp1;
				m_list_control.SetItemText(item,col,ShowText);
			}
		}
		break;
	case TYPE_BINARY:
		for (int col=0;col<6;col++)
		{
			for (int item=0;item<22;item++)
			{
				if(i_count>g_read_length)	//如果大于要读到得item就不显示了;
					return 0;
				i_count++;

				//这里是为了使显示的值右对齐，好看一点。;
				CString ShowText;
				CString temp_high;
				CString temp_low;
				CString temp2;
				CString temp3;
				CString temp_ret;
				ShowText.Format(_T("%05d = "),col*22+item +g_read_start_address/*,product_register_value[col*22+item]*/);

				temp_high.Format(_T("%d"),product_register_value[col*22+item + g_read_start_address]/256);
				temp_low.Format(_T("%d"),product_register_value[col*22+item + g_read_start_address]%256);
				temp2=DecToBin(temp_high);
				temp3 =DecToBin(temp_low);
				temp_ret = temp2 + temp3;
				int space_count = temp_ret.GetLength();
				for (int i=0;i<(8-space_count)*2;i++)
				{
					ShowText = ShowText +_T(" ");
				}
				ShowText = ShowText + temp_ret;
				m_list_control.SetItemText(item,col,ShowText);
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

CString CTestToolDlg::DecToBin(CString strDec)
{
	int nDec = _wtoi(strDec) ;//atoi(strDec.GetString());
	int nYushu, nShang;
	CString strBin = _T(""), strTemp;
	TCHAR buf[2];
	BOOL bContinue = TRUE;
	while ( bContinue )
	{
		nYushu = nDec % 2;
		nShang = nDec / 2;
		wsprintf(buf, _T("%d"), nYushu);
		strTemp = strBin;
		strBin.Format(_T("%s%s"), buf, strTemp);
		nDec = nShang;
		if ( nShang == 0 )
			bContinue = FALSE;
	}
	int nTemp = strBin.GetLength()%8;
	switch(nTemp)
	{
	case 1:
		strTemp.Format(_T("0000000%s"), strBin);
		strBin = strTemp;
		break;
	case 2:
		strTemp.Format(_T("000000%s"), strBin);
		strBin = strTemp;
		break;
	case 3:
		strTemp.Format(_T("00000%s"), strBin);
		strBin = strTemp;
		break;
	case 4:
		strTemp.Format(_T("0000%s"), strBin);
		strBin = strTemp;
		break;
	case 5:
		strTemp.Format(_T("000%s"), strBin);
		strBin = strTemp;
		break;
	case 6:
		strTemp.Format(_T("00%s"), strBin);
		strBin = strTemp;
		break;
	case 7:
		strTemp.Format(_T("0%s"), strBin);
		strBin = strTemp;
		break;
	default:
		break;
	}
	return strBin;
}



DWORD WINAPI   CTestToolDlg::Get_All_Dlg_Message(LPVOID lpVoid)
{
	//_MessageWriteOneInfo *My_Write_Struct=NULL;
	MSG msg;
	PeekMessage(&msg, NULL, MY_WRITE_ONE, MY_CLOSE, PM_REMOVE);
	while(true)
	{
		if(GetMessage(&msg,0,0,0)) //get msg from message queue
		{
			switch(msg.message)
			{
			case MY_WRITE_ONE:	
				MyCriticalSection.Lock();
				My_Receive_msg.push_back(msg);
				MyCriticalSection.Unlock();
				break;
			case  MY_READ_ONE:
				MyCriticalSection.Lock();
				My_Receive_msg.push_back(msg);
				MyCriticalSection.Unlock();
				////My_Write_Struct= (_MessageWriteOneInfo *)msg.wParam;
				break;
			case MY_READ_MULTY:
				MyCriticalSection.Lock();
				My_Receive_msg.push_back(msg);
				MyCriticalSection.Unlock();
				break;
			case MY_CLOSE:
				hThread = NULL;
				goto myend;
				break;
			}

		}
	}
myend:
	return 0;
}



DWORD WINAPI  CTestToolDlg::Translate_My_Message(LPVOID lpVoid)
{
	CTestToolDlg * mParent = (CTestToolDlg *)lpVoid;
	MSG msg;
	while(1)
	{
		if(My_Receive_msg.size()>0)
		{
			MSG my_temp_msg = My_Receive_msg.at(0);
			switch(my_temp_msg.message)
			{
			case MY_WRITE_ONE:
				{
					MyCriticalSection.Lock();
					msg=My_Receive_msg.at(0);
					_MessageWriteOneInfo *My_Write_Struct = (_MessageWriteOneInfo *)msg.wParam;
					My_Receive_msg.erase(My_Receive_msg.begin());
					MyCriticalSection.Unlock();

					if(write_one(My_Write_Struct->device_id, My_Write_Struct->address,My_Write_Struct->new_value,10)<0)
					{
						::PostMessage(My_Write_Struct->receive_hwnd,MY_RESULT,(WPARAM)WRITE_FAIL,(LPARAM)My_Write_Struct);
					}
					else
					{
						::PostMessage(My_Write_Struct->receive_hwnd,MY_RESULT,(WPARAM)WRITE_SUCCESS,(LPARAM)My_Write_Struct);
					}
				}
				break;
			case MY_READ_ONE:
				{
					//MyCriticalSection.Lock();
					//msg=My_Receive_msg.at(0);
					//_MessageReadOneInfo *My_Read_Struct = (_MessageReadOneInfo *)msg.wParam;
					//My_Receive_msg.erase(My_Receive_msg.begin());
					//MyCriticalSection.Unlock();
					////int ret = read_one(My_Read_Struct->device_id, My_Read_Struct->address,10);
					////if(ret<0)
					////{
					////	//::PostMessage(My_Read_Struct->hwnd,MY_READ_DATA_CALLBACK,(WPARAM)WRITE_FAIL,(LPARAM)My_Read_Struct);
					////	if(My_Read_Struct!=NULL)
					////		delete	My_Read_Struct;
					////}
					////else
					////{
					////	My_Read_Struct->new_value = ret;	//refresh the old value; post message to dlg deal the read data;
					////	::PostMessage(My_Read_Struct->hwnd,MY_READ_DATA_CALLBACK,(WPARAM)WRITE_SUCCESS,(LPARAM)My_Read_Struct);
					////}


				}
				break;
			case MY_READ_MULTY:
				MyCriticalSection.Lock();
				g_is_reading_now = true; //设置标志位，如果正在读取，则不要让线程在发送读的消息，否则，消息队列会爆掉;
				msg=My_Receive_msg.at(0);
				_MessageReadMultyInfo *p_my_read_muti = (_MessageReadMultyInfo *)msg.wParam;
				My_Receive_msg.erase(My_Receive_msg.begin());
				MyCriticalSection.Unlock();
				int return_value=0;
				int try_time=0;
				//do 
				//{
					return_value=Read_Multi(p_my_read_muti->device_id,&(product_register_value[p_my_read_muti->start_address]),p_my_read_muti->start_address,p_my_read_muti->length,1);
					g_Tx_count ++;
				//} while ((try_time++<3)&&(return_value!=p_my_read_muti->length));
				if (return_value<0)
				{
					g_Error_count++;
				}
				//if(try_time>=3)
				//{
					
					::PostMessage(mParent->m_hWnd,MY_FRESH_LISTCTRL,(WPARAM)DO_FAIL,(LPARAM)p_my_read_muti);
				//}
				::PostMessage(mParent->m_hWnd,MY_FRESH_LISTCTRL,(WPARAM)DO_SUCCESS,(LPARAM)p_my_read_muti);
				g_is_reading_now =false;
				/*if(Read_Multi(p_my_read_muti->device_id,&(product_register_value[p_my_read_muti->start_address]),p_my_read_muti->start_address,p_my_read_muti->length)>0)
				{

				}
				else
				{

				}*/

				break;
			}
		}
		Sleep(10);
	}
	return 0;
}



void CTestToolDlg::OnSetupPolldefinition()
{
	// TODO: Add your command handler code here
	CPolldefinition dlg;
	dlg.DoModal();

	my_readmult->device_id =g_read_ID;// g_read_ID;
	my_readmult->length =g_read_length;//g_read_length;
	my_readmult->start_address= g_read_start_address;
	my_readmult->myreaddata=&product_register_value[g_read_start_address];
	if(g_bIs_connected)
		SetTimer(1,g_read_interval,NULL);
	Fresh_TX_ERR_ID(g_Tx_count,g_Error_count,g_read_ID,g_read_interval,g_auto_read);
}


void CTestToolDlg::OnDisplaySigned()
{
	// TODO: Add your command handler code here
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_SIGNED, MF_BYPOSITION|MF_CHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_UNSIGNED, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_HEX, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_BINARY, MF_BYPOSITION|MF_UNCHECKED);
	m_Show_Data_Type = TYPE_SIGNED;
	WritePrivateProfileStringW(_T("Setting"),_T("Show Data Type"),_T("0"),Program_ConfigFile_Path);

	PostMessage(MY_FRESH_LISTCTRL,(WPARAM)DO_SUCCESS,NULL);
	m_list_control.SetColumnWidth(0,120);
	m_list_control.SetColumnWidth(1,120);
	m_list_control.SetColumnWidth(2,120);
	m_list_control.SetColumnWidth(3,120);
	m_list_control.SetColumnWidth(4,120);
	m_list_control.SetColumnWidth(5,120);
}


void CTestToolDlg::OnDisplayUnsigned()
{
	// TODO: Add your command handler code here
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_SIGNED, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_UNSIGNED, MF_BYPOSITION|MF_CHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_HEX, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_BINARY, MF_BYPOSITION|MF_UNCHECKED);
	m_Show_Data_Type = TYPE_UNSIGNED;
	WritePrivateProfileStringW(_T("Setting"),_T("Show Data Type"),_T("1"),Program_ConfigFile_Path);


	//m_list_control.SetColumnWidth(0,120);
	//m_list_control.SetColumnWidth(1,120);
	//m_list_control.SetColumnWidth(2,120);
	//m_list_control.SetColumnWidth(3,120);
	//m_list_control.SetColumnWidth(4,120);
	//m_list_control.SetColumnWidth(5,120);
	m_list_control.SetColumnWidth(0,120);
	m_list_control.SetColumnWidth(1,120);
	m_list_control.SetColumnWidth(2,120);
	m_list_control.SetColumnWidth(3,120);
	m_list_control.SetColumnWidth(4,120);
	m_list_control.SetColumnWidth(5,120);
	PostMessage(MY_FRESH_LISTCTRL,(WPARAM)DO_SUCCESS,NULL);
	//m_list_control.DeleteAllColumns();
	//m_list_control.InsertColumn(0,_T(""),LVCFMT_LEFT,180);  
	//m_list_control.InsertColumn(1,_T(""),LVCFMT_LEFT,180);
	//m_list_control.InsertColumn(2,_T(""),LVCFMT_LEFT,180);
	//m_list_control.InsertColumn(3,_T(""),LVCFMT_LEFT,180);
	//m_list_control.InsertColumn(4,_T(""),LVCFMT_LEFT,180);
	//m_list_control.InsertColumn(5,_T(""),LVCFMT_LEFT,180);
}


void CTestToolDlg::OnDisplayHex()
{
	// TODO: Add your command handler code here
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_SIGNED, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_UNSIGNED, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_HEX, MF_BYPOSITION|MF_CHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_BINARY, MF_BYPOSITION|MF_UNCHECKED);
	m_Show_Data_Type = TYPE_HEX;
	WritePrivateProfileStringW(_T("Setting"),_T("Show Data Type"),_T("2"),Program_ConfigFile_Path);
	PostMessage(MY_FRESH_LISTCTRL,(WPARAM)DO_SUCCESS,NULL);
	m_list_control.SetColumnWidth(0,120);
	m_list_control.SetColumnWidth(1,120);
	m_list_control.SetColumnWidth(2,120);
	m_list_control.SetColumnWidth(3,120);
	m_list_control.SetColumnWidth(4,120);
	m_list_control.SetColumnWidth(5,120);
}


void CTestToolDlg::OnDisplayBinary()
{
	// TODO: Add your command handler code here
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_SIGNED, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_UNSIGNED, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_HEX, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_DISPLAY)->CheckMenuItem(TYPE_BINARY, MF_BYPOSITION|MF_CHECKED);
	m_Show_Data_Type = TYPE_BINARY;
	WritePrivateProfileStringW(_T("Setting"),_T("Show Data Type"),_T("3"),Program_ConfigFile_Path);
	PostMessage(MY_FRESH_LISTCTRL,(WPARAM)DO_SUCCESS,NULL);
	m_list_control.SetColumnWidth(0,160);
	m_list_control.SetColumnWidth(1,160);
	m_list_control.SetColumnWidth(2,160);
	m_list_control.SetColumnWidth(3,160);
	m_list_control.SetColumnWidth(4,160);
	m_list_control.SetColumnWidth(5,160);

}


void CTestToolDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if((!g_is_reading_now)&&(g_auto_read))	//线程没有正在读，并且自动读取已经打开;
	{
		Post_Thread_Message(MY_READ_MULTY,my_readmult);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CTestToolDlg::OnSetupResetcounters()
{
	// TODO: Add your command handler code here
	g_Tx_count = 0;
	g_Error_count = 0;
		PostMessage(MY_FRESH_LISTCTRL,(WPARAM)DO_SUCCESS,NULL);
}

BOOL CTestToolDlg::OnToolTipNotify(UINT id,NMHDR *Pnmhdr,LRESULT *pResult)
{

	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *) Pnmhdr;
	UINT nID = Pnmhdr->idFrom;
	int index = m_wndToolBar.GetToolBarCtrl().CommandToIndex(nID);
	if(index==ENUM_DISCONNECTION)
	{
		pTTT->lpszText = _T("Disconnect (F4)");
	}
	else if(index==ENUM_CONNECTION)
	{
		pTTT->lpszText = _T("Quick connect (F5)");
	}
	else if(index==ENUM_RESET_COUNTER)
	{
		pTTT->lpszText = _T("Reset counters");;
	}
	else if(index==ENUM_POLL_DEFINITION)
	{
		pTTT->lpszText = _T("Poll definition (F2)");
	}
	else if(index ==ENUM_READ_ONCE)
	{
		pTTT->lpszText = _T("Read once if auto read disabled (F6)");
	}
	else if(index == ENUM_WRITE_SINGLE_REGISTER)
	{
		pTTT->lpszText = _T("06: Write Single Register (F8)");
	}
	m_StatusBar->SetText(pTTT->lpszText,0,0);
	pTTT->hinst = AfxGetResourceHandle();
	return true;

}


void CTestToolDlg::OnFunctionReadonce()
{
	// TODO: Add your command handler code here
	Post_Thread_Message(MY_READ_MULTY,my_readmult);
}


void CTestToolDlg::OnFunction06()
{
	// TODO: Add your command handler code here
	CWrite_Single_Reg Dlg;
	Dlg.DoModal();
}


BOOL CTestToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	m_list_control.GetWindowRect(ListRect);
	if((pMsg->pt.x>ListRect.left)&&(pMsg->pt.x<ListRect.right)&&(pMsg->pt.y>ListRect.top)&&(pMsg->pt.y<ListRect.bottom))
	{
		CString temp1;
		m_StatusBar->GetText(temp1.GetBuffer(MAX_PATH),0,0);
		if(temp1.CompareNoCase(_T("For Help,call Fance.For edit, double click on a value")))
			m_StatusBar->SetText(_T("For Help,call Fance.For edit, double click on a value"),0,0);
	}

	if (pMsg->message==WM_KEYDOWN) 
	{ 

		if(pMsg->wParam==VK_RETURN||pMsg->wParam==VK_ESCAPE)
			return TRUE; 
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CTestToolDlg::OnFileExit()
{
	// TODO: Add your command handler code here
	if(hThread!=NULL)
		TerminateThread(hThread,0);
	if(hDeal_thread!=NULL)
		TerminateThread(hDeal_thread,0);
	OnOK();
}



void CTestToolDlg::OnHdnBegintrackList1(NMHDR *pNMHDR, LRESULT *pResult) 
{ 
	LPNMHEADER phdr = reinterpret_cast <LPNMHEADER>(pNMHDR); 
	// TODO: 在此添加控件通知处理程序代码 ;
	*pResult = 1; 
} 



void CTestToolDlg::OnFunctionRegistermonitor()
{
	// TODO: Add your command handler code here
		m_function_type = FUNCTION_REGISTER_MONITOR;
		GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_REGISTER_MONITOR, MF_BYPOSITION|MF_CHECKED);
		GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_LC_TEST_JAT, MF_BYPOSITION|MF_UNCHECKED);
		GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_MODBUS_POLL, MF_BYPOSITION|MF_UNCHECKED);
		WritePrivateProfileStringW(_T("Setting"),_T("Function Type"),_T("0"),Program_ConfigFile_Path);
		OnConnectionDisconnect();
}


void CTestToolDlg::OnFunctionLctestjat()
{
	// TODO: Add your command handler code here
	m_function_type = FUNCTION_LC_TEST_JAT;
	GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_REGISTER_MONITOR, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_LC_TEST_JAT, MF_BYPOSITION|MF_CHECKED);
	GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_MODBUS_POLL, MF_BYPOSITION|MF_UNCHECKED);
	WritePrivateProfileStringW(_T("Setting"),_T("Function Type"),_T("1"),Program_ConfigFile_Path);
	OnConnectionDisconnect();
}


void CTestToolDlg::OnFunction3()
{
	// TODO: Add your command handler code here
	m_function_type = FUNCTION_MODBUS_POLL;
	GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_REGISTER_MONITOR, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_LC_TEST_JAT, MF_BYPOSITION|MF_UNCHECKED);
	GetMenu()->GetSubMenu(MENU_FUNCTION)->CheckMenuItem(FUNCTION_MODBUS_POLL, MF_BYPOSITION|MF_CHECKED);
	WritePrivateProfileStringW(_T("Setting"),_T("Function Type"),_T("2"),Program_ConfigFile_Path);
	OnConnectionDisconnect();
}


void CTestToolDlg::OnAboutAbout()
{
	// TODO: Add your command handler code here
	CMyAboutDlg mydlg;
	mydlg.DoModal();
}


void CTestToolDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}
