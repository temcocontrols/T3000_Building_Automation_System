// WriteFunctionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "WriteFunctionDlg.h"
#include "afxdialogex.h"
#include "global_variable_extern.h"
#include "global_function.h"
#include "ModbusDllForVC.h"


UINT _WriteMulti_SingleValue(LPVOID pParam)
{
	CWriteFunctionDlg* pFrame = (CWriteFunctionDlg*)(pParam);
	//pFrame->OnBnClickedOk();

	 if (pFrame->m_endaddress < pFrame->m_startaddress)
	{
	AfxMessageBox(L"start address > end address");
	return 0 ;
	}

	for (UINT i = pFrame->m_startaddress; i < pFrame->m_endaddress; i++)
	{
		int index = i - pFrame->m_startaddress; //pFrame->m_log_list.GetCount() ;
	CString m_strLog;

	int ret=	Write_One(pFrame->m_device_id, i, pFrame->m_value);
	if (ret > 0)
	{
	m_strLog.Format(_T("Write ID = %d,Address:%d,Value:%d,Result:OK"), pFrame->m_device_id, i, pFrame->m_value);
	}
	else
	{
	m_strLog.Format(_T("Write ID = %d,Address:%d,Value:%d,Result:Fail"), pFrame->m_device_id, i, pFrame->m_value);
	}

	pFrame->m_log_list.InsertString(index, m_strLog);
	} 
	return 1;
}
// CWriteFunctionDlg dialog

IMPLEMENT_DYNAMIC(CWriteFunctionDlg, CDialogEx)

CWriteFunctionDlg::CWriteFunctionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_startaddress(0)
	, m_endaddress(0)
	, m_value(0)
{
	m_device_id = 255;
}

CWriteFunctionDlg::~CWriteFunctionDlg()
{
}

void CWriteFunctionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_START, m_startaddress);
	DDX_Text(pDX, IDC_EDIT_END, m_endaddress);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_value);
	DDX_Control(pDX, IDC_LIST1, m_log_list);
}


BEGIN_MESSAGE_MAP(CWriteFunctionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWriteFunctionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CWriteFunctionDlg message handlers


void CWriteFunctionDlg::OnBnClickedOk()
{
	 
	UpdateData(TRUE);

	AfxBeginThread(_WriteMulti_SingleValue, this);
	/*if (m_endaddress < m_startaddress)
	{
		AfxMessageBox(L"start address > end address");
		return;
	}

	for (UINT i = m_startaddress; i < m_endaddress; i++)
	{
		int index = m_log_list.GetCount() - 1;
		CString m_strLog;

	  int ret=	Write_One(m_device_id, i, m_value);
	  if (ret > 0)
	  {
		  m_strLog.Format(_T("Write ID = %d,Address:%d,Value:%d,Result:OK"), m_device_id, i, m_value);
	  }
	  else
	  {
		  m_strLog.Format(_T("Write ID = %d,Address:%d,Value:%d,Result:Fail"), m_device_id, i, m_value);
	  }

	  m_log_list.InsertString(index, m_strLog);
	}*/

}
