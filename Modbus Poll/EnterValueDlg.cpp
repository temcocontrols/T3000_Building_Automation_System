// EnterValueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "EnterValueDlg.h"
#include "afxdialogex.h"


// CEnterValueDlg dialog

IMPLEMENT_DYNAMIC(CEnterValueDlg, CDialogEx)

CEnterValueDlg::CEnterValueDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnterValueDlg::IDD, pParent)
	 
	, m_static_StrValue(_T(""))
{
	/*m_StrValue=str;*/
}

CEnterValueDlg::~CEnterValueDlg()
{
}
//BOOL CEnterValueDlg::OnInitDialog(){
//
//	
//	return CDialogEx::OnInitDialog();
//}
void CEnterValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,m_StrValue);
	DDX_Control(pDX, IDC_EDIT1, m_Value_Edit);
	DDX_Text(pDX, IDC_STATIC_DATA_FORMATE, m_static_StrValue);
}


BEGIN_MESSAGE_MAP(CEnterValueDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEnterValueDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEnterValueDlg message handlers
BOOL CEnterValueDlg::OnInitDialog(){
	CDialogEx::OnInitDialog();
	CString title;
	if (m_dataformate==0)
	{
		title=_T("Enter Value(Signed)");
		m_static_StrValue=_T("Value(Signed)");
	} 
	else if (m_dataformate==1)
	{
		title=_T("Enter Value(Unsigned)");
		m_static_StrValue=_T("Value(Unsigned)");
	} 
	else if (m_dataformate==2)
	{
		title=_T("Enter Value(Hex)");
		m_static_StrValue=_T("Value(Hex)");
	} 
	else if (m_dataformate==3)
	{
		title=_T("Enter Value(Binary)");
		m_static_StrValue=_T("Value(Binary)");
	} 
	else 
	{
		title=_T("Enter Value");
	} 
	
	SetWindowTextW(title);

	UpdateData(FALSE);
	
	
	m_Value_Edit.SetCapture();//LSC
	int nLenth=m_StrValue.GetLength();
	m_Value_Edit.SetSel(0,nLenth); //全选//
	 m_Value_Edit.SetFocus();
	return FALSE;
}

void CEnterValueDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_dataformate==2)//Hex
	{
		//格式化Hex数据格式
		int ret;
		CString temp=_T("0x");
		if (m_StrValue.Find(_T("0x"))==-1)
		{
			//AfxMessageBox(_T("Please add '0x' "));
			temp+=m_StrValue;
			m_StrValue=temp;
			
		}
		StrToIntEx(m_StrValue.GetBuffer(),STIF_SUPPORT_HEX,&ret);
		m_StrValue.Format(_T("0x%04X"),ret);
		UpdateData(FALSE);

		UpdateData(TRUE);
	}
	CDialogEx::OnOK();
}
