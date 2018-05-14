// WriteSingle_BinaryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "WriteSingle_BinaryDlg.h"
#include "afxdialogex.h"

#include "global_variable_extern.h"
#include "global_function.h"
#include "ModbusDllForVC.h"
// CWriteSingle_BinaryDlg dialog

IMPLEMENT_DYNAMIC(CWriteSingle_BinaryDlg, CDialogEx)

CWriteSingle_BinaryDlg::CWriteSingle_BinaryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWriteSingle_BinaryDlg::IDD, pParent)
	//, m_Check1(FALSE)
{

	/*m_address = 0;
	m_slave_id = 0;*/
}

CWriteSingle_BinaryDlg::~CWriteSingle_BinaryDlg()
{
}

void CWriteSingle_BinaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_Check1);
	DDX_Check(pDX, IDC_CHECK2, m_Check2);
	DDX_Check(pDX, IDC_CHECK3, m_Check3);
	DDX_Check(pDX, IDC_CHECK4, m_Check4);
	DDX_Check(pDX, IDC_CHECK5, m_Check5);
	DDX_Check(pDX, IDC_CHECK6, m_Check6);
	DDX_Check(pDX, IDC_CHECK7, m_Check7);
	DDX_Check(pDX, IDC_CHECK8, m_Check8);
	DDX_Check(pDX, IDC_CHECK9, m_Check9);
	DDX_Check(pDX, IDC_CHECK10, m_Check10);
	DDX_Check(pDX, IDC_CHECK11, m_Check11);
	DDX_Check(pDX, IDC_CHECK12, m_Check12);
	DDX_Check(pDX, IDC_CHECK13, m_Check13);
	DDX_Check(pDX, IDC_CHECK14, m_Check14);
	DDX_Check(pDX, IDC_CHECK15, m_Check15);
	DDX_Check(pDX, IDC_CHECK16, m_Check16);


	DDX_Text(pDX, IDC_EDIT_ID, m_slave_id);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_address);
	DDX_Check(pDX, IDC_CHECK_CLOSE_DLG, m_close_dlg);


	DDX_Control(pDX, IDC_RADIO_MULTIPLE_REG_B, m_radio_mulitple);
	DDX_Control(pDX, IDC_RADIO_SINGLE_REG_B,m_radio_single);



	DDX_Control(pDX, IDC_CHECK_CLOSE_DLG, m_closeCheck);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_editor_address);
}


BEGIN_MESSAGE_MAP(CWriteSingle_BinaryDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWriteSingle_BinaryDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_SINGLE_REG_B, &CWriteSingle_BinaryDlg::OnBnClickedRadioSingleRegB)
	ON_BN_CLICKED(IDC_RADIO_MULTIPLE_REG_B, &CWriteSingle_BinaryDlg::OnBnClickedRadioMultipleRegB)
	ON_BN_CLICKED(IDC_CHECK_CLOSE_DLG, &CWriteSingle_BinaryDlg::OnBnClickedCheckCloseDlg)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CWriteSingle_BinaryDlg::OnInitDialog(){
	 
	

	 
	CButton *m_radio_0=(CButton*)GetDlgItem(IDC_RADIO_SINGLE_REG_B);
	m_radio_0->SetCheck(!m_function);
	CButton *m_radio_1=(CButton*)GetDlgItem(IDC_RADIO_MULTIPLE_REG_B);
	m_radio_1->SetCheck(m_function);

	//}
   // m_closeCheck.SetCheck(m_close_dlg);
	CButton *m_close_check=(CButton*)GetDlgItem(IDC_CHECK_CLOSE_DLG);
	m_close_check->SetCheck(!m_close_dlg);
 
	if (m_base_0)
	{
		m_address-=40000;
	}
	int len=m_StrValue.GetLength();
	int j=15;
	for (int i=0;i<len;i++)
	{
		char temp=m_StrValue.GetAt(i);
		if (temp==' ')
		{
			continue;
		} 
		else{
			if (temp!='0')
			{
				m_Binary_Value[j]=TRUE;
			} 
			else
			{
				m_Binary_Value[j]=FALSE;
			}
			--j;
		}

	}

	m_Check1=m_Binary_Value[0];
	m_Check2=m_Binary_Value[1];
	m_Check3=m_Binary_Value[2];
	m_Check4=m_Binary_Value[3];
	m_Check5=m_Binary_Value[4];
	m_Check6=m_Binary_Value[5];
	m_Check7=m_Binary_Value[6];
	m_Check8=m_Binary_Value[7];
	m_Check9=m_Binary_Value[8];
	m_Check10=m_Binary_Value[9];
	m_Check11=m_Binary_Value[10];
	m_Check12=m_Binary_Value[11];
	m_Check13=m_Binary_Value[12];
	m_Check14=m_Binary_Value[13];
	m_Check15=m_Binary_Value[14];
	m_Check16=m_Binary_Value[15];

    UpdateData(FALSE);

	return CDialogEx::OnInitDialog();
}

// CWriteSingle_BinaryDlg message handlers


void CWriteSingle_BinaryDlg::OnBnClickedOk()
{
	CString m_Tx,m_Rx;
	
	
	UpdateData(TRUE);
	m_Binary_Value[0]=m_Check1;
	m_Binary_Value[1]=m_Check2;
	m_Binary_Value[2]=m_Check3;
	m_Binary_Value[3]=m_Check4;
	m_Binary_Value[4]=m_Check5;
	m_Binary_Value[5]=m_Check6;
	m_Binary_Value[6]=m_Check7;
	m_Binary_Value[7]=m_Check8;
	m_Binary_Value[8]=m_Check9;
	m_Binary_Value[9]=m_Check10;
	m_Binary_Value[10]=m_Check11;
	m_Binary_Value[11]=m_Check12;
	m_Binary_Value[12]=m_Check13;
	m_Binary_Value[13]=m_Check14;
	m_Binary_Value[14]=m_Check15;
	m_Binary_Value[15]=m_Check16;
	
	//unsigned short value=m_Check16<<15+m_Check15<<14+m_Check14<<13+m_Check13<<12+m_Check12<<11+m_Check11<<10+
	//	                 m_Check10<<9+m_Check9<<8+m_Check8<<7+m_Check7<<6+m_Check6<<5+m_Check5<<4+m_Check4<<3+m_Check3<<2+m_Check2<<1+m_Check1;
	int Send_length;
	int Rev_length;
	unsigned short Reg_value=0;
	unsigned short UN_temp;
	for (int i=15;i>=0;i--)
	{

		if (m_Binary_Value[i])
		{
			UN_temp=1<<i;
			Reg_value+=UN_temp;
		}

	}
	
	CString temp;
	unsigned char rev_back_rawData[300],send_data[100];

	if (m_base_0!=0)//ÓÃ
	{
		--m_address;
	} 
	
	if (m_function)//¶àÐ´
	{
	 
		 
		unsigned char Single_Value[2];
 
		Single_Value[0]=(unsigned char)(Reg_value >> 8 & 0xff);
		Single_Value[1]=(unsigned char)(Reg_value & 0xff);
		 
		register_critical_section.Lock();
		int ret=write_multi_log(m_slave_id,Single_Value,m_address,sizeof(Single_Value),&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
		//int ret=write_multi(m_slave_id,Single_Value,m_address,1);
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

		
		//int Reg_Value;
		unsigned short Single_Value=Reg_value;
		 
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
	//CDialogEx::OnOK();
}


void CWriteSingle_BinaryDlg::OnBnClickedRadioSingleRegB()
{
	 m_function=!m_function;
}


void CWriteSingle_BinaryDlg::OnBnClickedRadioMultipleRegB()
{
	m_function=!m_function; 
}


void CWriteSingle_BinaryDlg::OnBnClickedCheckCloseDlg()
{
	// if (m_close_dlg)
	// {
		 m_close_dlg=!m_close_dlg;
	 //}
}


void CWriteSingle_BinaryDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialogEx::OnPaint() for painting messages
	m_editor_address.SetSel(0,-1);
	m_editor_address.SetFocus();
}
