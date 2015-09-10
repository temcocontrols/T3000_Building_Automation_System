// Enter_Value_BinaryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "Enter_Value_BinaryDlg.h"
#include "afxdialogex.h"


// CEnter_Value_BinaryDlg dialog

IMPLEMENT_DYNAMIC(CEnter_Value_BinaryDlg, CDialogEx)

CEnter_Value_BinaryDlg::CEnter_Value_BinaryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnter_Value_BinaryDlg::IDD, pParent)
	, m_Check16(FALSE)
{

}

CEnter_Value_BinaryDlg::~CEnter_Value_BinaryDlg()
{
}

void CEnter_Value_BinaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECK16, m_Check16);
    DDX_Check(pDX, IDC_CHECK15, m_Check15);
    DDX_Check(pDX, IDC_CHECK14, m_Check14);
    DDX_Check(pDX, IDC_CHECK13, m_Check13);
    DDX_Check(pDX, IDC_CHECK12, m_Check12);
    DDX_Check(pDX, IDC_CHECK11, m_Check11);
    DDX_Check(pDX, IDC_CHECK10, m_Check10);
    DDX_Check(pDX, IDC_CHECK9, m_Check9);
    DDX_Check(pDX, IDC_CHECK8, m_Check8);
    DDX_Check(pDX, IDC_CHECK7, m_Check7);
    DDX_Check(pDX, IDC_CHECK6, m_Check6);
    DDX_Check(pDX, IDC_CHECK5, m_Check5);
    DDX_Check(pDX, IDC_CHECK4, m_Check4);
    DDX_Check(pDX, IDC_CHECK3, m_Check3);
    DDX_Check(pDX, IDC_CHECK2, m_Check2);
    DDX_Check(pDX, IDC_CHECK1, m_Check1);
}


BEGIN_MESSAGE_MAP(CEnter_Value_BinaryDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CEnter_Value_BinaryDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CEnter_Value_BinaryDlg::OnInitDialog(){
	 
    m_Check16=m_Binary_Value[0];
    m_Check15=m_Binary_Value[1];
    m_Check14=m_Binary_Value[2];
    m_Check13=m_Binary_Value[3];
    m_Check12=m_Binary_Value[4];
    m_Check11=m_Binary_Value[5];
    m_Check10=m_Binary_Value[6];
    m_Check9=m_Binary_Value[7];
    m_Check8=m_Binary_Value[8];
    m_Check7=m_Binary_Value[9];
    m_Check6=m_Binary_Value[10];
    m_Check5=m_Binary_Value[11];
    m_Check4=m_Binary_Value[12];
    m_Check3=m_Binary_Value[13];
    m_Check2=m_Binary_Value[14];
    m_Check1=m_Binary_Value[15];

	UpdateData(FALSE);
	return CDialogEx::OnInitDialog();
}
// CEnter_Value_BinaryDlg message handlers


void CEnter_Value_BinaryDlg::OnBnClickedOk()
{
	 

	UpdateData(TRUE);
	
	CString StrTemp;
	m_Binary_Value[0]=m_Check16;//=m_Binary_Value[0];
	m_Binary_Value[1]=m_Check15;//=m_Binary_Value[1];
	m_Binary_Value[2]=m_Check14;//=m_Binary_Value[2];
	m_Binary_Value[3]=m_Check13;//=m_Binary_Value[3];
	m_Binary_Value[4]=m_Check12;//=m_Binary_Value[4];
	m_Binary_Value[5]=m_Check11;//=m_Binary_Value[5];
	m_Binary_Value[6]=m_Check10;//=m_Binary_Value[6];
	m_Binary_Value[7]=m_Check9;//=;m_Binary_Value[7];
	m_Binary_Value[8]=m_Check8;//=m_Binary_Value[8];
	m_Binary_Value[9]=m_Check7;//=m_Binary_Value[9];
	m_Binary_Value[10]=m_Check6;//=m_Binary_Value[10];
	m_Binary_Value[11]=m_Check5;//=m_Binary_Value[11];
	m_Binary_Value[12]=m_Check4;//=m_Binary_Value[12];
	m_Binary_Value[13]=m_Check3;//=m_Binary_Value[13];
	m_Binary_Value[14]=m_Check2;//=m_Binary_Value[14];
	m_Binary_Value[15]=m_Check1;//=m_Binary_Value[15];

 
	CDialogEx::OnOK();
}
