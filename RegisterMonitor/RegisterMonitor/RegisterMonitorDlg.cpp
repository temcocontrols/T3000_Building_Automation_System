
// RegisterMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegisterMonitor.h"
#include "RegisterMonitorDlg.h"
#include "afxdialogex.h"


#pragma comment(lib,"setupapi.lib")	//Enum comport need this lib and Dll.

#include "EnumSerial.h"

#include "ModbusDllforVc.h"


#include "global_variable_extern.h"
#include "globle_function.h"
#include "global_variable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  WM_WRITE_REG WM_USER + 101

DWORD nThreadID;
BOOL GetSerialComPortNumber_my(vector<CString>& szComm);
// CAboutDlg dialog used for App About




extern HINSTANCE RDll;

void show_RegisterMonitorDlg()
{
	//	AFX_MAMAGE_STATE(AfxGetStaticModuleState()); 


	HINSTANCE RResOld = AfxGetResourceHandle();
	AfxSetResourceHandle(RDll);

	CRegisterMonitorDlg dlg;
	dlg.DoModal();

	AfxSetResourceHandle(RResOld);

}




CRegisterMonitorDlg::CRegisterMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegisterMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_port_open = false;
	hFirstThread=NULL;
	suspend_thread = false;
	hWriteThread=NULL;


	reg_1_value=-1;
	reg_2_value=-1;
	reg_3_value=-1;
	reg_4_value=-1;
	reg_5_value=-1;

	have_First_Read=false;

}


CRegisterMonitorDlg::~CRegisterMonitorDlg()
{

	if(hFirstThread!=NULL)
		TerminateThread(hFirstThread, 0);

	if(hWriteThread!=NULL)
		TerminateThread(hWriteThread, 0);
}

void CRegisterMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_PORT, m_port);
	DDX_Control(pDX, IDC_EDIT1, m_device_edit);

	DDX_Control(pDX, IDC_COMBO2, m_combo_reg);


	DDX_Control(pDX, IDC_EDIT_REG1, m_reg_edit1);
	DDX_Control(pDX, IDC_EDIT_REG2, m_reg_edit2);
	DDX_Control(pDX, IDC_EDIT_REG3, m_reg_edit3);
	DDX_Control(pDX, IDC_EDIT_REG4, m_reg_edit4);
	DDX_Control(pDX, IDC_EDIT_REG5, m_reg_edit5);
}

BEGIN_MESSAGE_MAP(CRegisterMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &CRegisterMonitorDlg::OnBnClickedBtnOpen)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO2, &CRegisterMonitorDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON1, &CRegisterMonitorDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRegisterMonitorDlg message handlers

BOOL CRegisterMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		//BOOL bNameValid;
		//CString strAboutMenu;
		//bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		//ASSERT(bNameValid);
		//if (!strAboutMenu.IsEmpty())
		//{
		//	/*pSysMenu->AppendMenu(MF_SEPARATOR);
		//	pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);*/
		//}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	GetSerialComPortNumber_my(m_szComm);
	for (int i=0;i<(int)m_szComm.size();i++)
	{
		m_port.AddString(m_szComm.at(i));
	}
	m_port.SetCurSel(0);

	m_combo_reg.AddString(_T("Register 1"));
	m_combo_reg.AddString(_T("Register 2"));
	m_combo_reg.AddString(_T("Register 3"));
	m_combo_reg.AddString(_T("Register 4"));
	m_combo_reg.AddString(_T("Register 5"));


	GetModuleFileName(NULL,myapp_path.GetBuffer(MAX_PATH),MAX_PATH);  //获取当前运行的绝对地址;
	PathRemoveFileSpec(myapp_path.GetBuffer(MAX_PATH));            //返回绝对地址的上层目录?;
	myapp_path.ReleaseBuffer();
	myData_path = myapp_path;

	myapp_path = myapp_path + _T("\\");
	//current_time=CTime::GetCurrentTime();
	//CString str=current_time.Format("%Y_%m_%d_%H_%M_%S");
	//myData_path = myData_path + _T("\\") + str + _T(".txt"); 

	//	SetTimer(1,1000,NULL);
	m_combo_reg.EnableWindow(0);
	((CEdit *)GetDlgItem(IDC_EDIT_WRITE))->EnableWindow(0);
	((CButton *)GetDlgItem(IDC_BUTTON1))->EnableWindow(0);

	m_device_edit.SetFocus();
	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CRegisterMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	/*if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{*/
		CDialogEx::OnSysCommand(nID, lParam);
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRegisterMonitorDlg::OnPaint()
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
HCURSOR CRegisterMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL GetSerialComPortNumber_my(vector<CString>& szComm)
{
	CArray<SSerInfo,SSerInfo&> asi;

	// Populate the list of serial ports.
	EnumSerialPorts(asi,FALSE/*include all*/);
	szComm.clear();
	for (int ii=0; ii<asi.GetSize(); ii++) 
	{
		//m_listPorts.AddString(asi[ii].strFriendlyName);
		CString strCom = asi[ii].strFriendlyName;
		int nPos;
		if ((nPos = strCom.Find(_T("COM")))!=-1)
		{
			int startdex = strCom.ReverseFind(_T('('));
			int enddex = strCom.ReverseFind(_T(')'));
			//if (startdex > 0 && enddex == (strCom.GetLength()-1))
			strCom = strCom.Mid(startdex+1, enddex-startdex-1);
		}
		szComm.push_back(strCom);

	}

	return (szComm.size() !=0);
}


void CRegisterMonitorDlg::OnBnClickedBtnOpen()
{
	// TODO: Add your control notification handler code here

	wchar_t	 PortNumber[100];

	m_port.GetWindowTextW(PortNumber,100);
	CString tempcs = PortNumber;
	tempcs = tempcs.Right(tempcs.GetLength()-3);
	int comport = _wtoi(tempcs);
	CString reg_cs_temp;
	if(!m_port_open)
	{
		if(OnConnect(comport))
		{
			m_port.EnableWindow(FALSE);
			m_reg_edit1.EnableWindow(FALSE);
			m_reg_edit2.EnableWindow(FALSE);
			m_reg_edit3.EnableWindow(FALSE);
			m_reg_edit4.EnableWindow(FALSE);
			m_reg_edit5.EnableWindow(FALSE);
			m_device_edit.EnableWindow(FALSE);

			m_combo_reg.EnableWindow(TRUE);
			((CEdit *)GetDlgItem(IDC_EDIT_WRITE))->EnableWindow(TRUE);
			((CButton *)GetDlgItem(IDC_BUTTON1))->EnableWindow(1);
			SetDlgItemTextW(IDC_BTN_OPEN,_T("Close"));
			m_port_open = true;
			UpdateData(FALSE);

			m_reg_edit1.GetWindowText(reg_cs_temp);
			m_reg_address1 = _wtoi(reg_cs_temp);
			reg_cs_temp.Empty();

			m_reg_edit2.GetWindowText(reg_cs_temp);
			m_reg_address2 = _wtoi(reg_cs_temp);
			reg_cs_temp.Empty();

			m_reg_edit3.GetWindowText(reg_cs_temp);
			m_reg_address3 = _wtoi(reg_cs_temp);
			reg_cs_temp.Empty();

			m_reg_edit4.GetWindowText(reg_cs_temp);
			m_reg_address4 = _wtoi(reg_cs_temp);
			reg_cs_temp.Empty();

			m_reg_edit5.GetWindowText(reg_cs_temp);
			m_reg_address5 = _wtoi(reg_cs_temp);
			reg_cs_temp.Empty();

			m_device_edit.GetWindowText(reg_cs_temp);
			m_device_ID = _wtoi(reg_cs_temp);
			reg_cs_temp.Empty();

			SetTimer(1,1000,NULL);

			suspend_thread =false;

			if(hFirstThread==NULL)
				hFirstThread = CreateThread(NULL,NULL,ReadRegAddress,this,NULL,0);


			if(hWriteThread==NULL)
				hWriteThread = CreateThread(NULL,NULL,WriteRegAddress,this,NULL,&nThreadID);

		}

	}
	else
	{
		OnDisconnect();
		m_port.EnableWindow(1);
		m_reg_edit1.EnableWindow(1);
		m_reg_edit2.EnableWindow(1);
		m_reg_edit3.EnableWindow(1);
		m_reg_edit4.EnableWindow(1);
		m_reg_edit5.EnableWindow(1);
		m_device_edit.EnableWindow(1);

		m_combo_reg.EnableWindow(0);
		((CEdit *)GetDlgItem(IDC_EDIT_WRITE))->EnableWindow(0);
		((CButton *)GetDlgItem(IDC_BUTTON1))->EnableWindow(0);

		SetDlgItemTextW(IDC_BTN_OPEN,_T("Open"));
		m_port_open =false;
		UpdateData(FALSE);

		KillTimer(1);
		SetDlgItemTextW(IDC_STATIC_VALUE1,_T(""));
		SetDlgItemTextW(IDC_STATIC_VALUE2,_T(""));
		SetDlgItemTextW(IDC_STATIC_VALUE3,_T(""));
		SetDlgItemTextW(IDC_STATIC_VALUE4,_T(""));
		SetDlgItemTextW(IDC_STATIC_VALUE5,_T(""));

		suspend_thread =true;

		//if(hFirstThread!=NULL)
		//	TerminateThread(hFirstThread, 0);
		//hFirstThread=NULL;
	}
}
DWORD WINAPI CRegisterMonitorDlg::WriteRegAddress(LPVOID lpVoid)
{
	CRegisterMonitorDlg *pParent = (CRegisterMonitorDlg *)lpVoid;
	while(1)
	{
		MSG msg;
		PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
		if(GetMessage(&msg,0,0,0)) //get msg from message queue
		{
			char * pInfo = (char *)msg.wParam;
			switch(msg.message)
			{
			case WM_WRITE_REG:	
				if(write_one(pParent->m_device_ID,pParent->write_register,pParent->write_value,10)<0)
					AfxMessageBox(_T("Write Register Fail!Please try it again!"));
				else
					AfxMessageBox(_T("Change success!"));

				pParent->have_First_Read=false;
				//switch(pParent->sel_item)
				//{
				//case 0:
				//	pParent->reg_1_value= read_one(pParent->m_device_ID,pParent->m_reg_address1,6);
				//	break;
				//case 1:
				//	pParent->reg_2_value= read_one(pParent->m_device_ID,pParent->m_reg_address2,6);
				//	break;
				//case 2:
				//	pParent->reg_3_value= read_one(pParent->m_device_ID,pParent->m_reg_address3,6);
				//break;
				//case 3:
				//	pParent->reg_4_value= read_one(pParent->m_device_ID,pParent->m_reg_address4,6);
				//	break;
				//case 4:
				//	pParent->reg_5_value= read_one(pParent->m_device_ID,pParent->m_reg_address5,6);
				//	break;
				//default:
				//	break;
				//}

				break;
			default :
				break;
			}

		}

		Sleep(500);
	}

}

DWORD WINAPI CRegisterMonitorDlg::ReadRegAddress(LPVOID lpVoid)
{
	CRegisterMonitorDlg *pParent = (CRegisterMonitorDlg *)lpVoid;
	CTime Time_Click;

	CString cs_file_time;

	CString cs_time_now;
	CString cs_reg_value;


	CString Write_Position;
	while(1)
	{
		if(pParent->suspend_thread ==true)
		{
			Sleep(1000);
			continue;
		}
		if(pParent->have_First_Read==false)
		{
			pParent->reg_1_value= read_one(pParent->m_device_ID,pParent->m_reg_address1,6);
			pParent->reg_2_value= read_one(pParent->m_device_ID,pParent->m_reg_address2,6);
			pParent->reg_3_value= read_one(pParent->m_device_ID,pParent->m_reg_address3,6);
			pParent->reg_4_value= read_one(pParent->m_device_ID,pParent->m_reg_address4,6);
			pParent->reg_5_value= read_one(pParent->m_device_ID,pParent->m_reg_address5,6);
			pParent->have_First_Read=true;

		}
		else
		{
			if(!pParent->read_once1)
				pParent->reg_1_value= read_one(pParent->m_device_ID,pParent->m_reg_address1,6);
			if(!pParent->read_once2)
				pParent->reg_2_value= read_one(pParent->m_device_ID,pParent->m_reg_address2,6);
			if(!pParent->read_once3)
				pParent->reg_3_value= read_one(pParent->m_device_ID,pParent->m_reg_address3,6);
			if(!pParent->read_once4)
				pParent->reg_4_value= read_one(pParent->m_device_ID,pParent->m_reg_address4,6);
			if(!pParent->read_once5)
				pParent->reg_5_value= read_one(pParent->m_device_ID,pParent->m_reg_address5,6);
		}

		pParent->WriteFileString.Empty();
		cs_reg_value.Empty();
		cs_reg_value.Format(_T("Reg%d = %d ,Reg%d = %d , Reg%d = %d , Reg%d = %d , Reg%d = %d"),
			pParent->m_reg_address1,pParent->reg_1_value,
			pParent->m_reg_address2,pParent->reg_2_value,
			pParent->m_reg_address3,pParent->reg_3_value,	
			pParent->m_reg_address4,pParent->reg_4_value,
			pParent->m_reg_address5,pParent->reg_5_value);

		Time_Click = CTime::GetCurrentTime();
		cs_time_now = Time_Click.Format(_T("%m-%d %H:%M:%S    "));
		cs_file_time = Time_Click.Format(_T("%m_%d.txt"));
		pParent->WriteFileString = cs_time_now  + cs_reg_value + _T("\r\n");

		Write_Position = pParent->myapp_path + cs_file_time;


		CFile file(Write_Position,CFile::modeCreate |CFile::modeReadWrite |CFile::modeNoTruncate);
		file.SeekToEnd();
		int write_length = sizeof(TCHAR)*wcslen(pParent->WriteFileString.GetBuffer());
		file.Write(pParent->WriteFileString,write_length);
		file.Flush();
		pParent->WriteFileString.ReleaseBuffer();
		file.Close();

		Sleep(1000);
	}
	return 1;
}

//----------------------------------------------------------------------------------------
//Function:use this function to close the com port and enable the combobox (port and baud rate)
//Add Time:2013-03-20
//author  :Fan Du
//Modify Time:N/A
//----------------------------------------------------------------------------------------
void CRegisterMonitorDlg::OnDisconnect()
{
	//AfxMessageBox(_T("DisConnect."));
	close_com();
	CString strInfo = _T("No Connnection");
	m_port.EnableWindow(1);
	//m_baudrate.EnableWindow(1);
	m_port_open = false;
}


bool CRegisterMonitorDlg::OnConnect(int comport)
{

	open_com(comport);//open*************************************
	CString strInfo;
	if(!is_connect())
	{	
		strInfo.Format(_T("COM %d : Not available "), comport);
		MessageBox(strInfo);
		return false;
	}
	else
	{
		strInfo.Format(_T("COM %d Connected: Yes"), comport);	
		MessageBox(strInfo);
		return true;
	}	
}





void CRegisterMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CString TEMPCS;
	TEMPCS.Format(_T("%d"),reg_1_value);
	SetDlgItemTextW(IDC_STATIC_VALUE1,TEMPCS);
	TEMPCS.Empty();

	TEMPCS.Format(_T("%d"),reg_2_value);
	SetDlgItemTextW(IDC_STATIC_VALUE2,TEMPCS);

	TEMPCS.Format(_T("%d"),reg_3_value);
	SetDlgItemTextW(IDC_STATIC_VALUE3,TEMPCS);

	TEMPCS.Format(_T("%d"),reg_4_value);
	SetDlgItemTextW(IDC_STATIC_VALUE4,TEMPCS);

	TEMPCS.Format(_T("%d"),reg_5_value);
	SetDlgItemTextW(IDC_STATIC_VALUE5,TEMPCS);
	UpdateData(TRUE);
	read_once1= ((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck();
	read_once2= ((CButton *)GetDlgItem(IDC_CHECK2))->GetCheck();
	read_once3= ((CButton *)GetDlgItem(IDC_CHECK3))->GetCheck();
	read_once4= ((CButton *)GetDlgItem(IDC_CHECK4))->GetCheck();
	read_once5= ((CButton *)GetDlgItem(IDC_CHECK5))->GetCheck();

	CDialogEx::OnTimer(nIDEvent);
}


void CRegisterMonitorDlg::OnCbnSelchangeCombo2()
{
	// TODO: Add your control notification handler code here
	if(!m_port_open)
		MessageBox(_T("Please Open the Com port"));
	UpdateData(TRUE);

	sel_item=m_combo_reg.GetCurSel();
	CString temp_cs;
	switch(sel_item)
	{
	case 0:
		temp_cs.Format(_T("%d"),m_reg_address1);
		write_register = m_reg_address1;
		break;
	case 1:
		temp_cs.Format(_T("%d"),m_reg_address2);
		write_register = m_reg_address2;
		break;
	case 2:
		temp_cs.Format(_T("%d"),m_reg_address3);
		write_register = m_reg_address3;
		break;
	case 3:
		temp_cs.Format(_T("%d"),m_reg_address4);
		write_register = m_reg_address4;
		break;
	case 4:
		temp_cs.Format(_T("%d"),m_reg_address5);
		write_register = m_reg_address5;
		break;
	default:
		break;
	}

	SetDlgItemTextW(IDC_EDIT_WRITE,_T(""));

	SetDlgItemTextW(IDC_STATIC_REGISTER_WRITE,temp_cs);

	CString cs_write;
	GetDlgItemTextW(IDC_EDIT_WRITE,cs_write.GetBuffer(MAX_PATH),MAX_PATH);
	cs_write.ReleaseBuffer();
	write_value = _wtoi(cs_write);

	//if(hWriteThread==NULL)


}


void CRegisterMonitorDlg::OnBnClickedButton1()
{

	// TODO: Add your control notification handler code here

	CString cs_write;
	GetDlgItemTextW(IDC_EDIT_WRITE,cs_write.GetBuffer(MAX_PATH),MAX_PATH);
	cs_write.ReleaseBuffer();
	write_value = _wtoi(cs_write);

	PostThreadMessage(nThreadID,WM_WRITE_REG,0,0);
}




