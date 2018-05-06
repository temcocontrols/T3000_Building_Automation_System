// FreezeFunction.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "FreezeFunction.h"
#include "afxdialogex.h"

#include "global_function.h"
// CFreezeFunction 对话框

IMPLEMENT_DYNAMIC(CFreezeFunction, CDialog)

CFreezeFunction::CFreezeFunction(CWnd* pParent /*=NULL*/)
	: CDialog(CFreezeFunction::IDD, pParent)
	, m_temp(0)
	, m_open(0)
	, m_close(0)
	{

}

CFreezeFunction::~CFreezeFunction()
{
}

void CFreezeFunction::DoDataExchange(CDataExchange* pDX)
{
CDialog::DoDataExchange(pDX);
DDX_Text(pDX, IDC_TEMPTURE, m_temp);
DDX_Text(pDX, IDC_OPEN, m_open);
DDX_Text(pDX, IDC_CLOSE, m_close);
}


BEGIN_MESSAGE_MAP(CFreezeFunction, CDialog)
//	ON_EN_CHANGE(IDC_TEMPTURE, &CFreezeFunction::OnEnChangeTempture)
//	ON_EN_CHANGE(IDC_OPEN, &CFreezeFunction::OnEnChangeOpen)
//	ON_EN_CHANGE(IDC_CLOSE, &CFreezeFunction::OnEnChangeClose)
//	ON_EN_UPDATE(IDC_TEMPTURE, &CFreezeFunction::OnEnUpdateTempture)
//ON_EN_SETFOCUS(IDC_TEMPTURE, &CFreezeFunction::OnEnSetfocusTempture)
//ON_EN_KILLFOCUS(IDC_OPEN, &CFreezeFunction::OnEnKillfocusOpen)
//ON_EN_SETFOCUS(IDC_CLOSE, &CFreezeFunction::OnEnSetfocusClose)
ON_BN_CLICKED(IDOK, &CFreezeFunction::OnBnClickedOk)
ON_BN_CLICKED(ID_EXIT, &CFreezeFunction::OnBnClickedExit)
ON_BN_CLICKED(IDC_TEMP, &CFreezeFunction::OnBnClickedTemp)
ON_BN_CLICKED(IDC_TEMP2, &CFreezeFunction::OnBnClickedTemp2)
ON_BN_CLICKED(IDC_TEMP3, &CFreezeFunction::OnBnClickedTemp3)
END_MESSAGE_MAP()


// CFreezeFunction 消息处理程序
  BOOL CFreezeFunction::OnInitDialog()
  {
     CDialog::OnInitDialog();
	Fresh_Data();
	return TRUE;
  }

//void CFreezeFunction::OnEnChangeTempture()
//	{
//	 
//	UpdateData(TRUE);
//	CString str;
//	GetDlgItem(IDC_TEMPTURE)->GetWindowText(str);
//	if (!str.IsEmpty())
//		{
//		int var=write_one(g_tstat_id,295,m_temp);
//		if (var>0)
//			{
//			multi_register_value[295]=m_temp;
//			} 
//		else
//			{
//			AfxMessageBox(_T("Try again"));
//			}
//
//		}
//	Fresh_Data();
//	 
//	}


//void CFreezeFunction::OnEnChangeOpen()
//	{
//	  
//	}


//void CFreezeFunction::OnEnChangeClose()
//	{
//	
//
//	}

	void CFreezeFunction::Fresh_Data()
	{ 
	    m_temp=multi_register_value[295];
		m_open=multi_register_value[296];
		m_close=multi_register_value[297];
		UpdateData(FALSE);

	}



//	void CFreezeFunction::OnEnUpdateTempture()
//		{
//		//// TODO:  如果该控件是 RICHEDIT 控件，它将不
//		//// 发送此通知，除非重写 CDialog::OnInitDialog()
//		//// 函数，以将 EM_SETEVENTMASK 消息发送到该控件，
//		//// 同时将 ENM_UPDATE 标志“或”运算到 lParam 掩码中。
//	
//		//// TODO:  在此添加控件通知处理程序代码
//		}


//	void CFreezeFunction::OnEnSetfocusTempture()
//		{
//		
//		}


//	void CFreezeFunction::OnEnKillfocusOpen()
//		{
//		
//		}


//	void CFreezeFunction::OnEnSetfocusClose()
//		{
//		
//		}


	void CFreezeFunction::OnBnClickedOk()
		{
		// TODO: 在此添加控件通知处理程序代码
		//CDialog::OnOK();
		}


	void CFreezeFunction::OnBnClickedExit()
		{
		// TODO: 在此添加控件通知处理程序代码
		 CDialog::OnCancel();
		}


	void CFreezeFunction::OnBnClickedTemp()
		{
		UpdateData(TRUE);
		CString str;
		GetDlgItem(IDC_TEMPTURE)->GetWindowText(str);
		if (!str.IsEmpty())
			{
			int var=write_one(g_tstat_id,295,m_temp);
			if (var>0)
				{
				multi_register_value[295]=m_temp;
				AfxMessageBox(_T("OK"));
				} 
			else
			{
			AfxMessageBox(_T("Fail to Write"));
			}

			}
		Fresh_Data();
		}


	void CFreezeFunction::OnBnClickedTemp2()
		{
		UpdateData(TRUE);
		CString str;
		GetDlgItem(IDC_OPEN)->GetWindowText(str);
		if (!str.IsEmpty())
			{
			int var=write_one(g_tstat_id,296,m_open);
			if (var>0)
				{
				multi_register_value[296]=m_open;
				AfxMessageBox(_T("OK"));
				} 
			else
				{
				AfxMessageBox(_T("Fail to Write"));
				}
			}
		Fresh_Data();
		}


	void CFreezeFunction::OnBnClickedTemp3()
		{
		  UpdateData(TRUE);
		CString str;
		GetDlgItem(IDC_CLOSE)->GetWindowText(str);
		if (!str.IsEmpty())
			{
			int var=write_one(g_tstat_id,297,m_close);
			if (var>0)
				{
				multi_register_value[297]=m_close;
				AfxMessageBox(_T("OK"));
				} 
			else
				{
				AfxMessageBox(_T("Fail to Write"));
				}
			}
		Fresh_Data();
		}
