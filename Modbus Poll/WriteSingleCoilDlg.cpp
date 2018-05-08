// WriteSingleCoilDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WriteSingleCoilDlg.h"
#include "afxdialogex.h"
#include "Resource.h"


#include "global_variable_extern.h"
#include "global_function.h"
#include "ModbusDllForVC.h"
 


// CWriteSingleCoilDlg dialog

IMPLEMENT_DYNAMIC(CWriteSingleCoilDlg, CDialogEx)

CWriteSingleCoilDlg::CWriteSingleCoilDlg(UINT Slave_ID , UINT Address , BOOL Close_Dlg , BOOL Function, BOOL PLC_ADDRESS, BOOL Value,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_WRITE_SINGLE_COIL, pParent)
	, m_slave_id(Slave_ID)
	, m_address(Address)
	, m_close_dlg(Close_Dlg)
{
	m_function15 = Function;
	m_Value = Value;
	m_PLC_Address = PLC_ADDRESS;
}

CWriteSingleCoilDlg::~CWriteSingleCoilDlg()
{
}

void CWriteSingleCoilDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_slave_id);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_address);
	DDX_Check(pDX, IDC_CHECK_CLOSE_DLG, m_close_dlg);
}


BEGIN_MESSAGE_MAP(CWriteSingleCoilDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWriteSingleCoilDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_ON, &CWriteSingleCoilDlg::OnBnClickedRadioOn)
	ON_BN_CLICKED(IDC_RADIO_OFF, &CWriteSingleCoilDlg::OnBnClickedRadioOff)
	ON_BN_CLICKED(IDC_CHECK_CLOSE_DLG, &CWriteSingleCoilDlg::OnBnClickedCheckCloseDlg)
	ON_BN_CLICKED(IDC_RADIO_SINGLECOIL_REG, &CWriteSingleCoilDlg::OnBnClickedRadioSinglecoilReg)
	ON_BN_CLICKED(IDC_RADIO_MULTIPLECOIL_REG, &CWriteSingleCoilDlg::OnBnClickedRadioMultiplecoilReg)
END_MESSAGE_MAP()


// CWriteSingleCoilDlg message handlers


BOOL CWriteSingleCoilDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (m_Value)
	{
		((CButton *)GetDlgItem(IDC_RADIO_ON))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_OFF))->SetCheck(0);
	} 
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_ON))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO_OFF))->SetCheck(1);
	}
	if (m_function15)
	{
		((CButton *)GetDlgItem(IDC_RADIO_SINGLECOIL_REG))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO_MULTIPLECOIL_REG))->SetCheck(1);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_SINGLECOIL_REG))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_MULTIPLECOIL_REG))->SetCheck(0);
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
void CWriteSingleCoilDlg::FreshUI()
{
	if (m_Value)
	{
		((CButton *)GetDlgItem(IDC_RADIO_ON))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_OFF))->SetCheck(0);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_ON))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO_OFF))->SetCheck(1);
	}
	if (m_function15)
	{
		((CButton *)GetDlgItem(IDC_RADIO_SINGLECOIL_REG))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO_MULTIPLECOIL_REG))->SetCheck(1);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_SINGLECOIL_REG))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_MULTIPLECOIL_REG))->SetCheck(0);
	}
	if (m_close_dlg)
	{
		((CButton *)GetDlgItem(IDC_CHECK_CLOSE_DLG))->SetCheck(1);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_CLOSE_DLG))->SetCheck(0);
	}
}

void CWriteSingleCoilDlg::OnBnClickedOk()
{

	CString m_Tx, m_Rx;
	UpdateData(TRUE);
	int Send_length;
	int Rev_length;

	CString temp;

	unsigned char rev_back_rawData[300], send_data[100];
	
	if (((CButton *)GetDlgItem(IDC_RADIO_ON))->GetCheck() == 1)
	{
		m_Value = TRUE;
	} 
	else
	{
		m_Value = FALSE;
	}
	 
	if (m_function15)//∂‡–¥
	{
		int Reg_Value;
		 bool bValue = (bool)m_Value;
	
		register_critical_section.Lock();
		int ret = write_multi_Coil_log(m_slave_id,&bValue, m_address, 1, &send_data[0], &rev_back_rawData[0], &Send_length, &Rev_length);
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
	else//µ•–¥
	{
		int Reg_Value;
		unsigned short Single_Value;

		 
		register_critical_section.Lock();
		int ret = Write_Coil_log(m_slave_id, m_address, m_Value, &send_data[0], &rev_back_rawData[0], &Send_length, &Rev_length);
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
	if (m_close_dlg)
	{
		CDialogEx::OnOK();
	}
}


void CWriteSingleCoilDlg::OnBnClickedRadioOn()
{
	m_Value = TRUE;
	FreshUI();
}


void CWriteSingleCoilDlg::OnBnClickedRadioOff()
{
	m_Value = FALSE;
	FreshUI();
}


void CWriteSingleCoilDlg::OnBnClickedCheckCloseDlg()
{
	 if (m_close_dlg)
	 {
		 m_close_dlg = FALSE;
	 } 
	 else
	 {
		 m_close_dlg = TRUE;
	 }
	 FreshUI();
}


void CWriteSingleCoilDlg::OnBnClickedRadioSinglecoilReg()
{
	 
		m_function15 = FALSE;
	 
	    FreshUI();
}


void CWriteSingleCoilDlg::OnBnClickedRadioMultiplecoilReg()
{
	 
		m_function15 = FALSE;
 
		m_function15 = TRUE;
	 
	FreshUI();
}
