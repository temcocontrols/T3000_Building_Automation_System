// Write_Single_Reg.cpp : implementation file
//

#include "stdafx.h"
#include "TestTool.h"
#include "Write_Single_Reg.h"
#include "afxdialogex.h"
#include "global_variable_extern.h"
#include "globle_function.h"
// CWrite_Single_Reg dialog

IMPLEMENT_DYNAMIC(CWrite_Single_Reg, CDialogEx)

CWrite_Single_Reg::CWrite_Single_Reg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWrite_Single_Reg::IDD, pParent)
{

}

CWrite_Single_Reg::~CWrite_Single_Reg()
{
}

void CWrite_Single_Reg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWrite_Single_Reg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_SEND, &CWrite_Single_Reg::OnBnClickedButtonWriteSend)

	ON_MESSAGE(MY_RESULT, &CWrite_Single_Reg::FunctionCallBack)
	ON_BN_CLICKED(IDC_BUTTON2, &CWrite_Single_Reg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CWrite_Single_Reg message handlers


BOOL CWrite_Single_Reg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	((CButton *)GetDlgItem(IDC_RADIO_06))->SetCheck(TRUE);
	// TODO:  Add extra initialization here
	CString cs_write_id;
	CString cs_write_address;
	cs_write_id.Format(_T("%d"),g_ready_write_ID);
	cs_write_address.Format(_T("%d"),g_ready_write_Address);
	GetDlgItem(IDC_EDIT_WRITE_ID)->SetWindowTextW(cs_write_id);
	GetDlgItem(IDC_EDIT_WRITE_ADDRESS)->SetWindowTextW(cs_write_address);
	GetDlgItem(IDC_EDIT_WRITE_VALUE)->SetWindowTextW(_T("0"));
	((CEdit *)GetDlgItem(IDC_EDIT_WRITE_VALUE))->SetSel(0,-1);
	GetDlgItem(IDC_EDIT_WRITE_VALUE)->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CWrite_Single_Reg::OnBnClickedButtonWriteSend()
{
	// TODO: Add your control notification handler code here
	CString temp_value,temp_address,temp_id;
	GetDlgItemText(IDC_EDIT_WRITE_ID,temp_id);
	GetDlgItemText(IDC_EDIT_WRITE_ADDRESS,temp_address);
	GetDlgItemText(IDC_EDIT_WRITE_VALUE,temp_value);
	_MessageWriteOneInfo *my_write_stuct = new _MessageWriteOneInfo;
	my_write_stuct->device_id = _wtoi(temp_id);
	my_write_stuct->address   = _wtoi(temp_address);
	my_write_stuct->new_value = _wtoi(temp_value);
	my_write_stuct->receive_hwnd= this->m_hWnd;
	Post_Thread_Message(MY_WRITE_ONE,my_write_stuct);
}

LRESULT CWrite_Single_Reg::FunctionCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageWriteOneInfo *m_write_struct = (_MessageWriteOneInfo *)lParam;
	bool _result=false;
	_result = wParam;
	if(!_result)
		MessageBox(_T("Request Failed"));
	else
		MessageBox(_T("Request OK!"));
	GetDlgItem(IDC_EDIT_WRITE_VALUE)->SetFocus();
	((CEdit *)GetDlgItem(IDC_EDIT_WRITE_VALUE))->SetSel(0,-1);
	if(m_write_struct)
		delete m_write_struct;
	return 0;
}




void CWrite_Single_Reg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	OnOK();
}


BOOL CWrite_Single_Reg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN) 
	{ 
		if(pMsg->wParam==VK_RETURN)
		{
			
			OnBnClickedButtonWriteSend();
			GetDlgItem(IDC_EDIT_WRITE_VALUE)->SetFocus();
			return TRUE; 
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
