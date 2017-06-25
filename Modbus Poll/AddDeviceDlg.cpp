// AddDeviceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "AddDeviceDlg.h"
#include "afxdialogex.h"


// CAddDeviceDlg dialog

IMPLEMENT_DYNAMIC(CAddDeviceDlg, CDialogEx)

CAddDeviceDlg::CAddDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddDeviceDlg::IDD, pParent)
	, m_count(0)
{

	m_from = 0;
	m_modbus_id = 0;
	m_repeat = 0;
	m_length = 0;
}

CAddDeviceDlg::~CAddDeviceDlg()
{
}

void CAddDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FROM, m_from);
	DDX_Text(pDX, IDC_EDIT_ID, m_modbus_id);
	DDX_Text(pDX, IDC_EDIT_REPEAT, m_repeat);
	DDX_Text(pDX, IDC_EDIT_TO, m_length);
	DDX_Text(pDX, IDC_EDIT_COUNT, m_count);
}


BEGIN_MESSAGE_MAP(CAddDeviceDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddDeviceDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddDeviceDlg message handlers


void CAddDeviceDlg::OnBnClickedOk()
{
	
	
	UpdateData(TRUE);
CDialogEx::OnOK();
}
