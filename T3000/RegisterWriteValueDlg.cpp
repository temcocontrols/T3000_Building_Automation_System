// RegisterWriteValueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "RegisterWriteValueDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "globle_function.h"
#include "ado/ADO.h"

IMPLEMENT_DYNAMIC(CRegisterWriteValueDlg, CDialog)

CRegisterWriteValueDlg::CRegisterWriteValueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterWriteValueDlg::IDD, pParent)
{

}

CRegisterWriteValueDlg::~CRegisterWriteValueDlg()
{
}
void CRegisterWriteValueDlg::Set(int Col,CString Type,CString Name,CString No,CString Value,CString description,CString operation){
	m_col=Col;
	m_type=Type;
	m_name=Name;
	m_no=No;
	m_value=Value;
	m_description=description;
	m_operation=operation;
}
BOOL CRegisterWriteValueDlg::OnInitDialog(){
CDialog::OnInitDialog();
 m_caption=_T("Write Register Value-");
m_caption+=m_type;
SetWindowText(m_caption);
m_caption=_T("Reg Name:");
m_caption+=m_name;
m_reg_name.SetWindowText(m_caption);
m_caption=_T("");
m_caption+=m_no;
m_reg_no.SetWindowText(m_caption);
m_reg_no.EnableWindow(FALSE);
m_valueEdit.SetWindowText(m_value);
if (m_col==0)
{
	if((m_operation.CompareNoCase(_T("R"))==0)&&(m_operation.CompareNoCase(_T("/W"))!=0))
	{
		m_valueEdit.EnableWindow(FALSE);
		m_CanWrite=FALSE;
		m_sendBtn.SetWindowText(_T("Save"));
	}
	else
	{
		m_valueEdit.EnableWindow(TRUE);
		m_CanWrite=TRUE;
		m_sendBtn.SetWindowText(_T("Send"));
	}
	
	m_descriptionEdit.EnableWindow(FALSE);
} 
else
{
	m_valueEdit.EnableWindow(FALSE);
	m_CanWrite=FALSE;
	m_sendBtn.SetWindowText(_T("Save"));
	m_descriptionEdit.EnableWindow(TRUE);
}
m_descriptionEdit.SetWindowText(m_description);

return TRUE;

}
void CRegisterWriteValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_REG_NAME, m_reg_name);
	//  DDX_Control(pDX, IDC_REG_NO, m_reg_no);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_valueEdit);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTON, m_descriptionEdit);
	DDX_Control(pDX, IDC_REG_NAME, m_reg_name);
	DDX_Control(pDX, IDC_REG_NO, m_reg_no);
	DDX_Control(pDX, IDOK, m_sendBtn);
}


BEGIN_MESSAGE_MAP(CRegisterWriteValueDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRegisterWriteValueDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRegisterWriteValueDlg message handlers


void CRegisterWriteValueDlg::OnBnClickedOk()
{
   if (m_col==0)
   {
	   if (!m_CanWrite)
	   {
		   AfxMessageBox(_T("The register can't be written.\nBecause it is read-only."));
		   return;
	   }
	   m_valueEdit.GetWindowText(m_value);
	   int value=_wtoi(m_value);
	   int Add=_wtoi(m_no);
	   int ret=write_one(g_tstat_id,Add,value);
	   if (ret<0)
	   {
		   AfxMessageBox(_T("Write Fail"));
		   return;
	   }

	   product_register_value[Add]=value;
	   AfxMessageBox(_T("Write OK"));
   } 
   else
   {
   CADO ado;

   AfxMessageBox(_T("Save OK"));
   }
	
  
}


