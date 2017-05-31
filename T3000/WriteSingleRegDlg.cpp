// WriteSingleRegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "WriteSingleRegDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "global_function.h"
// CWriteSingleRegDlg dialog

IMPLEMENT_DYNAMIC(CWriteSingleRegDlg, CDialogEx)

 
CWriteSingleRegDlg::CWriteSingleRegDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWriteSingleRegDlg::IDD, pParent)

{

	m_address=0;
	m_value=0;

	m_slave=g_tstat_id;
}
void CWriteSingleRegDlg::Set(int Add,int Value){
	m_address=Add;
	m_value=Value;

	m_slave=g_tstat_id;
	/*UpdateData(FALSE);*/
}
CWriteSingleRegDlg::~CWriteSingleRegDlg()
{
}

void CWriteSingleRegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SLAVE, m_slave);
	DDX_Text(pDX, IDC_ADDRESS, m_address);
	DDX_Text(pDX, IDC_LALUE, m_value);
	DDX_Control(pDX, IDC_LALUE, m_valueedit);
}


BEGIN_MESSAGE_MAP(CWriteSingleRegDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWriteSingleRegDlg::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CWriteSingleRegDlg message handlers
BOOL CWriteSingleRegDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_version=get_tstat_version(g_tstat_id);
	
	
	//GotoDlgCtrl(GetDlgItem(IDC_LALUE));
	m_valueedit.SetFocus();
	UpdateData(FALSE);
	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CWriteSingleRegDlg::OnBnClickedOk()
{
	
	//CDialogEx::OnOK();
	UpdateData(TRUE);
	int ret=write_one(g_tstat_id,m_address,m_value);
	if (ret>0)
	{
	  product_register_value[m_address]=m_value;
	  AfxMessageBox(_T("Write OK"));

	} 
	else
	{
// 	 int value_ret=read_one(g_tstat_id,m_address,7);
// 	 if (value_ret==m_value)
// 	 {
// 	 multi_register_value[m_address]=m_value;
// 	 AfxMessageBox(_T("Write OK"));
// 	 	UpdateData(FALSE);
// 		return;
// 	 }
// 	 m_value=multi_register_value[m_address];
// 	 AfxMessageBox(_T("Write Fail"));
	}

}


void CWriteSingleRegDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialogEx::OnPaint() for painting messages
}
