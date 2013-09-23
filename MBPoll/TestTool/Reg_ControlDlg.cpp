
// Reg_ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Reg_ControlDlg.h"
#include "afxdialogex.h"

#pragma warning(disable:4800)
#pragma warning(disable:4805)

#pragma comment(lib,"setupapi.lib")	//Enum comport need this lib and Dll.

#include "EnumSerial.h"

#include "ModbusDllforVc.h"


#include "global_variable_extern.h"
#include "globle_function.h"
//#include "global_variable.h"



//BOOL GetSerialComPortNumber_my1(vector<CString>& szComm);


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

//class CAboutDlg : public CDialogEx
//{
//public:
//	CAboutDlg();
//
//// Dialog Data
//	enum { IDD = IDD_ABOUTBOX };
//
//	protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//
//// Implementation
//protected:
//	DECLARE_MESSAGE_MAP()
//};
//
//CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
//{
//}
//
//void CAboutDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CDialogEx::DoDataExchange(pDX);
//}
//
//BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//END_MESSAGE_MAP()


// CReg_ControlDlg dialog




CReg_ControlDlg::CReg_ControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReg_ControlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	hFirstThread=NULL;
	m_port_open =false;
}

void CReg_ControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_PORT, m_port);
	DDX_Control(pDX, IDC_EDIT_ID, m_reg_id);
	DDX_Control(pDX, IDC_EDIT_ON_TIME, m_reg_on_time);
	DDX_Control(pDX, IDC_EDIT_CYCLE, m_reg_cycle);


}

BEGIN_MESSAGE_MAP(CReg_ControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LC_OPEN, &CReg_ControlDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_DEVICE_ID_WRITE, &CReg_ControlDlg::OnBnClickedBtnDeviceIdWrite)
	ON_BN_CLICKED(IDC_OUTPUT_BTN_WRITE, &CReg_ControlDlg::OnBnClickedOutputBtnWrite)
	ON_BN_CLICKED(IDC_BUTTON4, &CReg_ControlDlg::OnBnClickedButton4)
//	ON_COMMAND(ID_SETUP_POLLDEFINITION, &CReg_ControlDlg::OnSetupPolldefinition)
END_MESSAGE_MAP()


CReg_ControlDlg::~CReg_ControlDlg()
{

	if(hFirstThread!=NULL)
		TerminateThread(hFirstThread, 0);
}


// CReg_ControlDlg message handlers

BOOL CReg_ControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	/*ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);*/

	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != NULL)
	//{
	//	BOOL bNameValid;
	//	CString strAboutMenu;
	//	bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	ASSERT(bNameValid);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	//}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//GetSerialComPortNumber_my1(m_szComm);
	for (int i=0;i<(int)m_szComm.size();i++)
	{
		m_port.AddString(m_szComm.at(i));
	}
	m_port.SetCurSel(0);
	m_port_open = false;
	m_reg_on_time.SetWindowTextW(_T("200"));
	m_reg_cycle.SetWindowTextW(_T("500"));

	suspend_thread=false;
	memset(out_put_bit,0,64);

	 reg_100 = 0;
	 reg_101 = 0;
	  reg_102=0;
	  reg_103=0;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CReg_ControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	//if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	//{
	//	CAboutDlg dlgAbout;
	//	dlgAbout.DoModal();
	//}
	//else
	//{
		CDialogEx::OnSysCommand(nID, lParam);
	//}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CReg_ControlDlg::OnPaint()
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
HCURSOR CReg_ControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//BOOL GetSerialComPortNumber_my1(vector<CString>& szComm)
//{
//	CArray<SSerInfo,SSerInfo&> asi;
//
//	// Populate the list of serial ports.
//	EnumSerialPorts(asi,FALSE/*include all*/);
//	szComm.clear();
//	for (int ii=0; ii<asi.GetSize(); ii++) 
//	{
//		//m_listPorts.AddString(asi[ii].strFriendlyName);
//		CString strCom = asi[ii].strFriendlyName;
//		int nPos;
//		if ((nPos = strCom.Find(_T("COM")))!=-1)
//		{
//			int startdex = strCom.ReverseFind(_T('('));
//			int enddex = strCom.ReverseFind(_T(')'));
//			//if (startdex > 0 && enddex == (strCom.GetLength()-1))
//			strCom = strCom.Mid(startdex+1, enddex-startdex-1);
//		}
//		szComm.push_back(strCom);
//
//	}
//
//	return (szComm.size() !=0);
//}


void CReg_ControlDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	if(!m_port_open)
	{
		if(g_bIs_connected)
		{
			suspend_thread=false;
			m_port.EnableWindow(FALSE);
			m_reg_on_time.EnableWindow(0);
			m_reg_cycle.EnableWindow(0);
			SetDlgItemTextW(IDC_BTN_LC_OPEN,_T("Stop"));
			m_port_open = true;
			UpdateData(FALSE);

			if(hFirstThread==NULL)
				hFirstThread = CreateThread(NULL,NULL,ReadRegAddress,this,NULL,0);
		}

	}
	else
	{
		m_port.EnableWindow(1);
		SetDlgItemTextW(IDC_BTN_LC_OPEN,_T("Start"));
		m_reg_on_time.EnableWindow(1);
		m_reg_cycle.EnableWindow(1);
		suspend_thread=true;
	}
}
//void CReg_ControlDlg::OnBnClickedButton1()
//{
//	// TODO: Add your control notification handler code here
//	wchar_t	 PortNumber[100];
//
//	m_port.GetWindowTextW(PortNumber,100);
//	CString tempcs = PortNumber;
//	tempcs = tempcs.Right(tempcs.GetLength()-3);
//	int comport = _wtoi(tempcs);
//	CString reg_cs_temp;
//	if(!m_port_open)
//	{
//		if(OnConnect(comport))
//		{
//			suspend_thread=false;
//			m_port.EnableWindow(FALSE);
//			m_reg_on_time.EnableWindow(0);
//			m_reg_cycle.EnableWindow(0);
//			SetDlgItemTextW(IDC_BTN_LC_OPEN,_T("Close"));
//			m_port_open = true;
//			UpdateData(FALSE);
//
//			if(hFirstThread==NULL)
//				hFirstThread = CreateThread(NULL,NULL,ReadRegAddress,this,NULL,0);
//		}
//
//	}
//	else
//	{
//		OnDisconnect();
//		m_port.EnableWindow(1);
//		SetDlgItemTextW(IDC_BTN_LC_OPEN,_T("Open"));
//		m_reg_on_time.EnableWindow(1);
//		m_reg_cycle.EnableWindow(1);
//		suspend_thread=true;
//	}
//}


void CReg_ControlDlg::OnDisconnect()
{
	close_com();
	CString strInfo = _T("No Connnection");
	m_port.EnableWindow(1);
	m_port_open = false;
}

bool CReg_ControlDlg::OnConnect(int comport)
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

void CReg_ControlDlg::OnBnClickedBtnDeviceIdWrite()
{
	// TODO: Add your control notification handler code here
	CString temp_id;
	CString temp_on_time;
	CString temp_cycle;
	int i_id;
	int i_time;
	int i_cycle;
	m_reg_id.GetWindowTextW(temp_id);
	m_reg_on_time.GetWindowTextW(temp_on_time);
	m_reg_cycle.GetWindowTextW(temp_cycle);
	i_id = _wtoi(temp_id);
	i_time = _wtoi(temp_on_time);
	i_cycle = _wtoi(temp_cycle);

	if(write_one(255,6,i_id)<0)
	{
		MessageBox(_T("Ð´ÈëÊ§°Ü£¬ÇëÖØÊÔ"));
		return;
	}

	if(write_one(255,104,i_time)<0)
	{
		MessageBox(_T("Ð´ÈëÊ§°Ü£¬ÇëÖØÊÔ"));
		return;
	}

	if(write_one(255,105,i_cycle)<0)
	{
		MessageBox(_T("Ð´ÈëÊ§°Ü£¬ÇëÖØÊÔ"));
		return;
	}
	MessageBox(_T("Ð´Èë³É¹¦£¡"));

	int id_read=0;
	int time_read=0;
	int cycle_read=0;
	CString temp_read_id;
	CString temp_read_time;
	CString temp_read_cycle;
	id_read=read_one(i_id,6,3);
	time_read=read_one(i_id,104,3);
	cycle_read=read_one(i_id,105,3);
	temp_read_id.Format(_T("%d"),id_read);
	temp_read_time.Format(_T("%d"),time_read);
	temp_read_cycle.Format(_T("%d"),cycle_read);
	GetDlgItem(IDC_STATIC_ID)->SetWindowText(temp_read_id);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(temp_read_time);
	GetDlgItem(IDC_STATIC_CYCLE)->SetWindowText(temp_read_cycle);

}


void CReg_ControlDlg::OnBnClickedOutputBtnWrite()
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<32;i++)
	{
		out_put_bit[i] = ((CButton *)GetDlgItem(2000 + 32 -i))->GetCheck();
	}


	reg_100 = out_put_bit[15]<<15 | out_put_bit[14]<<14 | out_put_bit[13]<<13 |
		 out_put_bit[12]<<12 | out_put_bit[11]<<11 | out_put_bit[10]<<10 | 
		 out_put_bit[9]<<9 | out_put_bit[8]<<8 | out_put_bit[7]<<7 | 
		 out_put_bit[6]<<6 | out_put_bit[5]<<5 | out_put_bit[4]<<4 | 
		 out_put_bit[3]<<3 | out_put_bit[2]<<2 | out_put_bit[1]<<1 | out_put_bit[0];

	reg_101 = out_put_bit[31]<<15 | out_put_bit[30]<<14 | out_put_bit[29]<<13 |
		out_put_bit[28]<<12 | out_put_bit[27]<<11 | out_put_bit[26]<<10 | 
		out_put_bit[25]<<9 | out_put_bit[24]<<8 | out_put_bit[23]<<7 | 
		out_put_bit[22]<<6 | out_put_bit[21]<<5 | out_put_bit[20]<<4 | 
		out_put_bit[19]<<3 | out_put_bit[18]<<2 | out_put_bit[17]<<1 | out_put_bit[16];

	if(write_one(255,100,reg_100)<0)
	{
		MessageBox(_T("Ð´ÈëÊ§°Ü£¬ÇëÖØÊÔ"));
	    return;
	}

	if(write_one(255,101,reg_101)<0)
	{
		MessageBox(_T("Ð´ÈëÊ§°Ü£¬ÇëÖØÊÔ"));
		return;
	}
	MessageBox(_T("Ð´Èë³É¹¦£¡"));


	fresh_output();
}


void CReg_ControlDlg::fresh_output()
{
	reg_102=read_one(255,102);
	if(reg_102<0)
	{
		//MessageBox(_T("¶ÁÊ§°Ü£¬ÇëÖØÊÔ"));
		return;
	}

	reg_103=read_one(255,103);
	if(reg_103<0)
	{
		//MessageBox(_T("¶ÁÊ§°Ü£¬ÇëÖØÊÔ"));
		return;
	}

	for (int i=0;i<16;i++)
	{
		out_put_bit[32+i] = reg_102 &(0x0001<<i);
	}

	for (int i=0;i<16;i++)
	{
		out_put_bit[48+i] = reg_103 &(0x0001<<i);
	}

	for (int i =0;i<32;i++)
	{
		if(out_put_bit[32+i]==1)
			((CButton *)GetDlgItem(2032 +32 - i))->SetCheck(1);
		else
			((CButton *)GetDlgItem(2032 +32 - i))->SetCheck(0);
	}
}

void CReg_ControlDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	fresh_output();
}

BOOL CReg_ControlDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN)
		{
			CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
			GetDlgItem(IDC_BTN_DEVICE_ID_WRITE)->SetFocus();
			temp_focus->SetFocus();
			return 1;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

DWORD WINAPI CReg_ControlDlg::ReadRegAddress(LPVOID lpVoid)
{
	CReg_ControlDlg *pParent = (CReg_ControlDlg *)lpVoid;
	CTime Time_Click;

	CString cs_file_time;

	CString cs_time_now;
	CString cs_reg_value;


	CString Write_Position;
	while(1)
	{
		if(pParent->suspend_thread ==true)
		{
			Sleep(2000);
			continue;
		}

		pParent->fresh_output();
		Sleep(1000);
	}
	return 0;
}

//void CReg_ControlDlg::OnSetupPolldefinition()
//{
//	// TODO: Add your command handler code here
//}
