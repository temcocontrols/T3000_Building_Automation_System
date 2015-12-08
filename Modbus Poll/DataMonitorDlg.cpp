// DataMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "DataMonitorDlg.h"
#include "afxdialogex.h"


// CDataMonitorDlg dialog

IMPLEMENT_DYNAMIC(CDataMonitorDlg, CDialogEx)

CDataMonitorDlg::CDataMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataMonitorDlg::IDD, pParent)
{

}

CDataMonitorDlg::~CDataMonitorDlg()
{
}

void CDataMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDataMonitorDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataMonitorDlg message handlers
