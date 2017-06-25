// RegisterWriteValueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "RegisterWriteValueDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "global_function.h"
 
 #include "../SQLiteDriver/CppSQLite3.h"
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
m_color=0;
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
	ON_WM_CTLCOLOR()
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
		   m_color=1;
		   return;
	   }
	   product_register_value[Add]=value;
	   m_color=0;
	   AfxMessageBox(_T("Write OK"));
   } 
   else
   {
   int Address=_wtoi(m_no);
   CString description;
   m_descriptionEdit.GetWindowText(description);
   CppSQLite3DB SqliteDBT3000;
   CppSQLite3Query q;
   SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
   CString sql;
   if (product_type==T3000_5ABCDFG_LED_ADDRESS)
   {
       try
       {
		   sql.Format(_T("update T3000_Register_Address_By_ID set TSTAT5_LED_DESCRIPTION='%s' where Register_Address=%d"),description.GetBuffer(),Address);
		   SqliteDBT3000.execDML((UTF8MBSTR)sql);
		   AfxMessageBox(_T("Save OK"));
       }
       catch (CException* e)
       {
	      AfxMessageBox(_T("Save Error"));
       }
	   
   }
   else if (product_type==T3000_5EH_LCD_ADDRESS)
   {
	   try
	   {
		   sql.Format(_T("update T3000_Register_Address_By_ID set TSTAT5_LCD_DESCRIPTION='%s' where Register_Address=%d"),description.GetBuffer(),Address);
		   SqliteDBT3000.execDML((UTF8MBSTR)sql);
		   AfxMessageBox(_T("Save OK"));
	   }
	   catch (CException* e)
	   {
		   AfxMessageBox(_T("Save Error"));
	   }
   }
   else if (product_type==T3000_6_ADDRESS)
   {
	   try
	   {
		   sql.Format(_T("update T3000_Register_Address_By_ID set TSTAT6_DESCRIPTION='%s' where Register_Address=%d"),description.GetBuffer(),Address);
		   SqliteDBT3000.execDML((UTF8MBSTR)sql);
		   AfxMessageBox(_T("Save OK"));
	   }
	   catch (CException* e)
	   {
		   AfxMessageBox(_T("Save Error"));
	   }
   }
   else
   {

   }

   }
	
  
}




HBRUSH CRegisterWriteValueDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	
	if (pWnd->GetDlgCtrlID()==IDC_EDIT_VALUE)
	{
	  if (m_color==0)
	  {
	      HBRUSH B = CreateSolidBrush(RGB(255,255,255));
		  pDC->SetTextColor(RGB(0,0,0));
		  pDC->SetBkColor(RGB(255,255,255));
		  pDC->SetBkMode(TRANSPARENT);
		  return (HBRUSH)B;
	  } 
	  else
	  {
		  pDC->SetTextColor(RGB(255,0,0));
		  pDC->SetBkColor(RGB(0,0,0));
		  pDC->SetBkMode(TRANSPARENT);
		  HBRUSH B = CreateSolidBrush(RGB(255,0,0));
		  return (HBRUSH)B;
	  }
	}
	
	return hbr;
}
