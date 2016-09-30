// DeviceTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "DeviceTestDlg.h"
#include "afxdialogex.h"


// CDeviceTestDlg dialog

IMPLEMENT_DYNAMIC(CDeviceTestDlg, CDialogEx)

CDeviceTestDlg::CDeviceTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceTestDlg::IDD, pParent)
{

}

CDeviceTestDlg::~CDeviceTestDlg()
{
}

void CDeviceTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDeviceTestDlg, CDialogEx)
END_MESSAGE_MAP()


// CDeviceTestDlg message handlers
