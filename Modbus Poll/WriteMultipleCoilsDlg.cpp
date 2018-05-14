// WriteMultipleCoilsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WriteMultipleCoilsDlg.h"
#include "afxdialogex.h"

 
#include "global_function.h"
 
#include "ModbusDllForVC.h"

// CWriteMultipleCoilsDlg dialog

IMPLEMENT_DYNAMIC(CWriteMultipleCoilsDlg, CDialogEx)

CWriteMultipleCoilsDlg::CWriteMultipleCoilsDlg(UINT Slave_ID, UINT address, UINT quantity, unsigned short *databuffer, UINT dataformat, BOOL Base_0, CWnd* pParent)
	: CDialogEx(IDD_DIALOG_WRITE_MULTIPLE_COILS, pParent)
{
	m_relative_address = m_address = address;
	m_slave_id = Slave_ID;
	m_quantity = quantity;
 
	m_DataBuffer = databuffer;
	 
}

CWriteMultipleCoilsDlg::~CWriteMultipleCoilsDlg()
{
}

void CWriteMultipleCoilsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_relative_address);
	DDX_Text(pDX, IDC_EDIT_ID, m_slave_id);
	DDX_Text(pDX, IDC_EDIT_QUANTITY, m_quantity);
	DDX_Control(pDX, IDC_LIST3, m_ListBox_registers);
}


BEGIN_MESSAGE_MAP(CWriteMultipleCoilsDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWriteMultipleCoilsDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CWriteMultipleCoilsDlg message handlers


BOOL CWriteMultipleCoilsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	/*
	m_ListBox_registers.AddString(L"0");
	m_ListBox_registers.AddString(L"1");
	m_ListBox_registers.AddString(L"2");
	m_ListBox_registers.AddString(L"3");
	m_ListBox_registers.SetCheck(0, 1);
	m_ListBox_registers.SetCheck(1, 1);
	m_ListBox_registers.SetCheck(2, 1);
	m_ListBox_registers.SetCheck(3, 1);
	*/
	CString str;
	for (int i=0;i<m_quantity;i++)
	{
		str.Format(_T("Coil %02d"), m_address + i);
		m_ListBox_registers.AddString(str);
		m_ListBox_registers.SetCheck(i,m_DataBuffer[i]);

	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CWriteMultipleCoilsDlg::OnBnClickedOk()
{
	int Send_length;
	int Rev_length;
	CString temp;
	unsigned char rev_back_rawData[300], send_data[100];
	CString m_Tx, m_Rx;
	UpdateData(TRUE);

	bool DataBuffer[127];
	for (int i=0;i<m_quantity;i++)
	{
		DataBuffer[i] = m_ListBox_registers.GetCheck(i);
	}
	 
		register_critical_section.Lock();
	int ret = write_multi_Coil_log(m_slave_id, DataBuffer, m_address, m_quantity, &send_data[0], &rev_back_rawData[0], &Send_length, &Rev_length);
	register_critical_section.Unlock();
	++g_Tx_Rx;
	temp.Format(_T("%06d--"), g_Tx_Rx);
	m_Tx += temp;

	for (int i = 0; i < Send_length; i++)
	{
		temp.Format(_T("%02X "), send_data[i]);
		m_Tx += temp;
	}
	Traffic_Data(m_Tx);
	++g_Tx_Rx;
	temp.Format(_T("%06d--"), g_Tx_Rx);
	m_Rx += temp;
	for (int i = 0; i < Rev_length; i++) {
		temp.Format(_T("%02X "), rev_back_rawData[i]);
		m_Rx += temp;
	}
	Traffic_Data(m_Rx);
	if (ret > 0)
	{
		AfxMessageBox(_T("Write OK!"));
		return;
	}


	if (ret < 0)
	{
		AfxMessageBox(_T("Write Fail,Try,again!"));
		return;
	}
}
