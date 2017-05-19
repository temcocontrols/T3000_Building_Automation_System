// BacnetMessageInput.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetMessageInput.h"
#include "afxdialogex.h"


// CBacnetMessageInput dialog

IMPLEMENT_DYNAMIC(CBacnetMessageInput, CDialogEx)

CBacnetMessageInput::CBacnetMessageInput(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetMessageInput::IDD, pParent)
{
}

CBacnetMessageInput::~CBacnetMessageInput()
{
}

void CBacnetMessageInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BACNET_INPUT_TITLE, m_static_title);
	DDX_Control(pDX, IDC_EDIT_BACNET_MESSAGE_INPUT, m_edit_return);
}


BEGIN_MESSAGE_MAP(CBacnetMessageInput, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBacnetMessageInput::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBacnetMessageInput::OnBnClickedCancel)
END_MESSAGE_MAP()


// CBacnetMessageInput message handlers


BOOL CBacnetMessageInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_static_title.SetWindowTextW(bacnet_message_input_title);
	m_static_title.textColor(RGB(0,0,0));
	//m_static_title.bkColor(RGB(0,255,255));
	m_static_title.setFont(24, 14,NULL,_T("Arial"));


	m_edit_return.SetWindowTextW(_T(""));
	m_edit_return.textColor(RGB(0,0,0));
	m_edit_return.bkColor(RGB(255,255,255));
	m_edit_return.setFont(24, 14,NULL,_T("Arial"));

	m_edit_return.SetFocus();
	// TODO:  Add extra initialization here

	return FALSE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetMessageInput::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetMessageInput::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_BACNET_MESSAGE_INPUT)->GetWindowTextW(bacnet_message_return_string);
	CDialogEx::OnOK();
}


void CBacnetMessageInput::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	bacnet_message_return_string.Empty();
	CDialogEx::OnCancel();
}
