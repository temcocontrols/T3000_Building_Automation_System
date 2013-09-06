// TestMultiReadTraffic.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TestMultiReadTraffic.h"
#include "afxdialogex.h"


// CTestMultiReadTraffic dialog

IMPLEMENT_DYNAMIC(CTestMultiReadTraffic, CDialogEx)

CTestMultiReadTraffic::CTestMultiReadTraffic(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestMultiReadTraffic::IDD, pParent)
	, m_ID(0)
	, m_length(0)
	, m_startid(0)
{
  m_index=0;
}

CTestMultiReadTraffic::~CTestMultiReadTraffic()
{
}

void CTestMultiReadTraffic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listbox);
	DDX_Text(pDX, IDC_EDIT1, m_ID);
	DDX_Text(pDX, IDC_EDIT2, m_length);
	DDX_Text(pDX, IDC_EDIT3, m_startid);
}


BEGIN_MESSAGE_MAP(CTestMultiReadTraffic, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTestMultiReadTraffic::OnBnClickedOk)
END_MESSAGE_MAP()


// CTestMultiReadTraffic message handlers


void CTestMultiReadTraffic::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	CString m_Tx=_T("Tx: ");
	CString temp;
	CString m_Rx=_T("Rx: ");
	UpdateData(TRUE);
	unsigned short rev_back_rawData[300],send_data[8];
	unsigned short rev_back_Data[300];
	register_critical_section.Lock();
	read_multi_log(m_ID,&rev_back_Data[0],m_startid,m_length,&send_data[0],&rev_back_rawData[0]);
	register_critical_section.Unlock();
	if (g_CommunicationType==0)
	{
		for (int i=0;i<8;i++)
		{
			temp.Format(_T("%02X "),send_data[i]);
			m_Tx+=temp;
		}
		temp.Format(_T("%04d--"),m_index);
		temp+=m_Tx;
		m_listbox.InsertString(m_index++,temp);
	  
		for(int i=0;i<5+2*m_length;i++){
			temp.Format(_T("%02X "),rev_back_rawData[i]);
			m_Rx+=temp;
		}
		temp.Format(_T("%04d--"),m_index);
		temp+=m_Rx;
		m_listbox.InsertString(m_index++,temp);
		 
	}
	else
	{
		for (int i=0;i<6;i++)
		{
			temp.Format(_T("%0X "),send_data[i]);
			m_Tx+=temp;
		}
		m_listbox.InsertString(m_index++,m_Tx);
		for(int i=0;i<3+2*m_length;i++){
			temp.Format(_T("%0X "),rev_back_rawData[i]);
			m_Rx+=temp;
		}
		m_listbox.InsertString(m_index++,m_Rx);
	}
    
}
