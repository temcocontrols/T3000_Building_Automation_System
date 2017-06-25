// ReadWriteDefinitionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "ReadWriteDefinitionDlg.h"
#include "afxdialogex.h"
#include "global_variable_extern.h"
#include "Modbus PollView.h"

// CReadWriteDefinitionDlg dialog

IMPLEMENT_DYNAMIC(CReadWriteDefinitionDlg, CDialogEx)

CReadWriteDefinitionDlg::CReadWriteDefinitionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReadWriteDefinitionDlg::IDD, pParent)
	, m_slave_id(0)
	, m_plc_addresses(FALSE)
	 
{

	m_slave_id = 255;
	m_scan_rate = 1000;
	m_quantity = 10;
	m_address = 0;
	m_function = 0;
	m_data_formate = 1;
	m_apply=TRUE;
	m_wronce=FALSE;
	 

 

}

CReadWriteDefinitionDlg::~CReadWriteDefinitionDlg()
{
}

void CReadWriteDefinitionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT_SLAVE_ID, m_slave_id);
	DDX_Text(pDX, IDC_EDIT_SLAVE_ID, m_slave_id);
	DDV_MinMaxUInt(pDX, m_slave_id, 1, 255);
	DDX_Text(pDX, IDC_EDIT_SCAN_RATE, m_scan_rate);
	DDX_Text(pDX, IDC_EDIT_QUANTITY, m_quantity);
	DDV_MinMaxUInt(pDX, m_quantity, 1, 127);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_address);
	DDV_MinMaxUInt(pDX, m_address, 0, UINT_MAX);
	DDX_CBIndex(pDX, IDC_COMBO_FUNCTIONS, m_function);
	DDX_CBIndex(pDX, IDC_COMBO_DATA_FORMATE, m_data_formate);

	DDX_Check(pDX, IDC_CHECK_HIDE_ALIAS_COLUMNS, m_hide_alias_col);
	DDX_Check(pDX, IDC_CHECK_aDDRESS_CELL, m_address_in_cell);
	DDX_Control(pDX, IDC_COMBO_FUNCTIONS, m_combox_function);
	DDX_Control(pDX, IDC_COMBO_DATA_FORMATE, m_combox_dataformate);
	DDX_Check(pDX, IDC_PLC_ADD, m_plc_addresses);
	//  DDX_Control(pDX, IDC_CHECK_HIDE_ALIAS_COLUMNS, m_Check_hide_alias_col);
	DDX_Control(pDX, IDC_RADIO_10, m_Radio_10);
	DDX_Control(pDX, IDC_RADIO20, m_Radio_20);
	DDX_Control(pDX, IDC_RADIO50, m_Radio_50);
	DDX_Control(pDX, IDC_RADIO100, m_Radio_100);
	DDX_Control(pDX, IDC_RADIOFIT, m_Radio_Fit);


	DDX_Control(pDX, IDC_EDIT_SLAVE_ID, m_editor_slave_id);
	//  DDX_Check(pDX, IDC_CHECK_READ_WRITE_ONCE, m_is_read_write_once);
}


BOOL CReadWriteDefinitionDlg::OnInitDialog(){
CDialogEx::OnInitDialog();
m_combox_function.ResetContent();
m_combox_function.AddString(_T("01 Read Coils(0x)"));
m_combox_function.AddString(_T("02 Read Discrete Inputs(1x)"));
m_combox_function.AddString(_T("03 Read Holding Registers(4x)"));
m_combox_function.AddString(_T("04 Read Input Registers(3x)"));
//m_combox_function.AddString(_T("05 Write Single Coil"));
//m_combox_function.AddString(_T("06 Write Single Register"));
//m_combox_function.AddString(_T("15 Write Multiple Coils"));
//m_combox_function.AddString(_T("16 Write Multiple Registers"));
m_combox_function.SetCurSel(m_function);

m_combox_dataformate.ResetContent();
m_combox_dataformate.AddString(_T("Signed"));
m_combox_dataformate.AddString(_T("Unsigned"));
m_combox_dataformate.AddString(_T("Hex"));
m_combox_dataformate.AddString(_T("Binary"));

if (m_rows==0)
{
	 m_Radio_10.SetCheck(1);
}
else if (m_rows==1)
{
	 m_Radio_20.SetCheck(1);
}
else if (m_rows==2)
{
	 m_Radio_50.SetCheck(1);
}
else if (m_rows==3)
{
	 m_Radio_100.SetCheck(1);
}
else {
	  m_Radio_Fit.SetCheck(1);
}
/*m_address=0;*/
UpdateData(FALSE);

return TRUE;
}

BEGIN_MESSAGE_MAP(CReadWriteDefinitionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CReadWriteDefinitionDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_10, &CReadWriteDefinitionDlg::OnBnClickedRadio10)
	ON_BN_CLICKED(IDC_RADIO20, &CReadWriteDefinitionDlg::OnBnClickedRadio20)
	ON_BN_CLICKED(IDC_RADIO50, &CReadWriteDefinitionDlg::OnBnClickedRadio50)
	ON_BN_CLICKED(IDC_RADIO100, &CReadWriteDefinitionDlg::OnBnClickedRadio100)
	ON_BN_CLICKED(IDC_RADIOFIT, &CReadWriteDefinitionDlg::OnBnClickedRadiofit)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CReadWriteDefinitionDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_RW_ONCE, &CReadWriteDefinitionDlg::OnBnClickedButtonRwOnce)
END_MESSAGE_MAP()


// CReadWriteDefinitionDlg message handlers



void CReadWriteDefinitionDlg::OnBnClickedOk()
{
	  
	  UpdateData(TRUE);
	  if (m_plc_addresses!=0)
	  {
		   if (m_address==0)
		   {
			   AfxMessageBox(_T("Please input [Address] an interger between 1 and 65535"));
			   return;
		   }
		   
	  }
	  CDialogEx::OnOK();
}


void CReadWriteDefinitionDlg::OnBnClickedRadio10()
{
	 
	m_rows=0;
}


void CReadWriteDefinitionDlg::OnBnClickedRadio20()
{
	m_rows=1;
}


void CReadWriteDefinitionDlg::OnBnClickedRadio50()
{
	
	m_rows=2;
}


void CReadWriteDefinitionDlg::OnBnClickedRadio100()
{
	
	m_rows=3;
}


void CReadWriteDefinitionDlg::OnBnClickedRadiofit()
{
	
	m_rows=4;
}


void CReadWriteDefinitionDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialogEx::OnPaint() for painting messages
	m_editor_slave_id.SetSel(0,-1);
	m_editor_slave_id.SetFocus();
}


void CReadWriteDefinitionDlg::OnBnClickedButtonApply()
{
	m_apply=TRUE;
	m_wronce=FALSE;
    ::SendMessage(m_pWnd,MY_READ_ONCE,(WPARAM)this,0);
 
}


void CReadWriteDefinitionDlg::OnBnClickedButtonRwOnce()
{
   m_apply=FALSE;
   m_wronce=TRUE;
   ::SendMessage(m_pWnd,MY_READ_ONCE,(WPARAM)this,0);
 
}
