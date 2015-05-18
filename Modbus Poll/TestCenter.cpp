// TestCenter.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "TestCenter.h"
#include "afxdialogex.h"


// TestCenter dialog

IMPLEMENT_DYNAMIC(TestCenter, CDialogEx)

TestCenter::TestCenter(CWnd* pParent /*=NULL*/)
	: CDialogEx(TestCenter::IDD, pParent)
{

}

TestCenter::~TestCenter()
{
}

void TestCenter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ADDLIST, m_addlist_btn);
	DDX_Control(pDX, IDC_BUTTON_COPY, m_copy_btn);
	DDX_Control(pDX, IDC_BUTTON_OPENLIST, m_openlist_btn);
	DDX_Control(pDX, IDC_BUTTON_SAVELIST, m_savelist_btn);
	DDX_Control(pDX, IDC_CHECK_ADD, m_add_check);
	DDX_Control(pDX, IDC_LIST2, m_list);
	DDX_Control(pDX, IDC_COMBO1, m_combox_send);
}


BEGIN_MESSAGE_MAP(TestCenter, CDialogEx)
	ON_BN_CLICKED(IDOK, &TestCenter::OnBnClickedOk)
 
END_MESSAGE_MAP()


// TestCenter message handlers


void TestCenter::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


 
