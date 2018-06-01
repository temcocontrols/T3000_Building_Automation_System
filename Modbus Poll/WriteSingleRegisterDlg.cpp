// WriteSingleRegisterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "WriteSingleRegisterDlg.h"
#include "afxdialogex.h"

#include "global_variable_extern.h"
#include "global_function.h"
#include "ModbusDllForVC.h"

// CWriteSingleRegisterDlg dialog

IMPLEMENT_DYNAMIC(CWriteSingleRegisterDlg, CDialogEx)

CWriteSingleRegisterDlg::CWriteSingleRegisterDlg(UINT Slave_ID,UINT Address,BOOL Close_Dlg,BOOL Single_Function,UINT DataFormat,BOOL Base_0,CString value,CWnd* pParent /*=NULL*/)
	: CDialogEx(CWriteSingleRegisterDlg::IDD, pParent)

{
	m_slave_id=Slave_ID;

	m_address=Address;
	m_data_format=DataFormat;
	m_StrValue =value;
	if (m_data_format==0)
	{
		m_static_StrValue = _T("Value(Signed)");
	} 
	else if (m_data_format==1)
	{
		m_static_StrValue = _T("Value(Unsigned)");
	} 
	else
	{
		m_static_StrValue = _T("Value(Hex)");
	}
	

	m_close_dlg=Close_Dlg;
	m_function=Single_Function;//single write
	m_base_0=Base_0;
	if (m_base_0)
	{
		m_address-=40000;
	}
}

CWriteSingleRegisterDlg::~CWriteSingleRegisterDlg()
{
}

void CWriteSingleRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_slave_id);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_address);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_StrValue);
	DDX_Text(pDX, IDC_STATIC_VALUE, m_static_StrValue);
	DDX_Check(pDX, IDC_CHECK_CLOSE_DLG, m_close_dlg);


	DDX_Control(pDX, IDC_RADIO_MULTIPLE_REG, m_radio_mulitple);
	DDX_Control(pDX, IDC_RADIO_SINGLE_REG,m_radio_single);

	DDX_Control(pDX, IDC_CHECK_CLOSE_DLG, m_closeCheck);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_editor_value);
}


BEGIN_MESSAGE_MAP(CWriteSingleRegisterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWriteSingleRegisterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_SINGLE_REG, &CWriteSingleRegisterDlg::OnBnClickedRadioSingleReg)
	ON_BN_CLICKED(IDC_RADIO_MULTIPLE_REG, &CWriteSingleRegisterDlg::OnBnClickedRadioMultipleReg)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CWriteSingleRegisterDlg message handlers

BOOL CWriteSingleRegisterDlg::OnInitDialog(){

    if (m_data_format==2)
    {
		int Edit_Value;
		StrToIntEx(m_StrValue.GetBuffer(),STIF_SUPPORT_HEX,&Edit_Value);
		m_StrValue.Format(_T("0x%X"),Edit_Value);
    }
     

	UpdateData(FALSE);
	if (m_function)
	{
		m_radio_mulitple.SetCheck(m_function);
		m_radio_single.SetCheck(!m_function);
	} 
	else
	{
		m_radio_mulitple.SetCheck(m_function);
		m_radio_single.SetCheck(!m_function);
	}

	m_closeCheck.SetCheck(m_close_dlg);
	
	return CDialogEx::OnInitDialog();
}
void CWriteSingleRegisterDlg::OnBnClickedOk()
{
	CString m_Tx,m_Rx;

 
	UpdateData(TRUE);
	int Send_length;
	int Rev_length;

	CString temp;

	unsigned char rev_back_rawData[300],send_data[100];

	if (m_base_0!=0)//ÓÃ
	{
		--m_address;
	} 
	if (m_function)//¶àÐ´
	{
		int Reg_Value;
		 
	//	int ret=read_multi_log(ID,&DataBuffer[0],startAdd,quantity,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
		//data_to_write[2]=start_address >> 8 & 0xff;//Address Hi
		//data_to_write[3]=start_address & 0xff;//Address low
		if (m_data_format==0)
		{
			StrToIntEx(m_StrValue,STIF_DEFAULT,&Reg_Value);
			
		} 
		else if (m_data_format==1)
		{
			StrToIntEx(m_StrValue,STIF_DEFAULT,&Reg_Value);
			 
		}
		else//2
		{
			CString temp=_T("0x");
			if ((m_StrValue.FindOneOf(_T("0X"))!=-1)||(m_StrValue.FindOneOf(_T("0x"))!=-1))
			{
				temp=m_StrValue;
			} 
			else
			{
				temp+=m_StrValue;
			}
			StrToIntEx(temp,STIF_SUPPORT_HEX,&Reg_Value);
			 
		}
		unsigned short un_Value=(unsigned short)Reg_Value;
		register_critical_section.Lock();
		 int ret=write_multi_Short_log(m_slave_id,&un_Value,m_address,1,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
		//int ret=write_multi_Short(m_slave_id,&un_Value,m_address,1);
		register_critical_section.Unlock();
		++g_Tx_Rx;
		temp.Format(_T("%06d--"),g_Tx_Rx);
		m_Tx+=temp;

		for (int i=0;i<Send_length;i++)
		{
			temp.Format(_T("%02X "),send_data[i]);
			m_Tx+=temp;
		}
		Traffic_Data(m_Tx);


		++g_Tx_Rx;
		temp.Format(_T("%06d--"),g_Tx_Rx);
		m_Rx+=temp;

		for(int i=0;i<Rev_length;i++){
			temp.Format(_T("%02X "),rev_back_rawData[i]);
			m_Rx+=temp;
		}
		Traffic_Data(m_Rx);
		if (ret>0)
		{
			AfxMessageBox(_T("Write OK!"));
			return;
		}
		if (ret<0)
		{
			AfxMessageBox(_T("Write Fail,Try,again!"));
			return;
		} 

	} 
	else//µ¥Ð´
	{
		int Reg_Value;
		unsigned short Single_Value;

		if (m_data_format==0)
		{
			StrToIntEx(m_StrValue,STIF_DEFAULT,&Reg_Value);
			Single_Value=(unsigned short)Reg_Value;

		} 
		else if (m_data_format==1)
		{
			StrToIntEx(m_StrValue,STIF_DEFAULT,&Reg_Value);
			Single_Value=(unsigned short)Reg_Value;
		}
		else//2
		{
			CString temp=_T("0x");
			if ((m_StrValue.FindOneOf(_T("0X"))!=-1)||(m_StrValue.FindOneOf(_T("0x"))!=-1))
			{
				temp=m_StrValue;
			} 
			else
			{
				temp+=m_StrValue;
			}

			StrToIntEx(temp,STIF_SUPPORT_HEX,&Reg_Value);
			Single_Value=(unsigned short)Reg_Value;
		}
		register_critical_section.Lock();
		int ret=Write_One_log(m_slave_id,m_address,Single_Value,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
		register_critical_section.Unlock();
		++g_Tx_Rx;
		temp.Format(_T("%06d--"),g_Tx_Rx);
		m_Tx+=temp;

		for (int i=0;i<Send_length;i++)
		{
			temp.Format(_T("%02X "),send_data[i]);
			m_Tx+=temp;
		}
		Traffic_Data(m_Tx);


		++g_Tx_Rx;
		temp.Format(_T("%06d--"),g_Tx_Rx);
		m_Rx+=temp;

		for(int i=0;i<Rev_length;i++){
			temp.Format(_T("%02X "),rev_back_rawData[i]);
			m_Rx+=temp;
		}
		Traffic_Data(m_Rx);
		if (ret>0)
		{
			AfxMessageBox(_T("Write OK!"));
			return;
		}
		if (ret<0)
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

void CWriteSingleRegisterDlg::OnBnClickedRadioSingleReg()
{
	
	m_function=0;
}


void CWriteSingleRegisterDlg::OnBnClickedRadioMultipleReg()
{
	
	m_function=1;
}


void CWriteSingleRegisterDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialogEx::OnPaint() for painting messages
	m_editor_value.SetSel(0,-1);
	m_editor_value.SetFocus();
	//GetDlgItem(IDC_EDIT_VALUE)->SetCaretPos()
}

