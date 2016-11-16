// ISPModeSlove.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ISPModeSlove.h"
#include "afxdialogex.h"
#include "globle_function.h"

// CISPModeSlove dialog

IMPLEMENT_DYNAMIC(CISPModeSlove, CDialogEx)

CISPModeSlove::CISPModeSlove(CWnd* pParent /*=NULL*/)
	: CDialogEx(CISPModeSlove::IDD, pParent)
{

}

CISPModeSlove::~CISPModeSlove()
{
}

void CISPModeSlove::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DEVICE_IP, m_device_ip_title);
	DDX_Control(pDX, IDC_STATIC_DEVICE_MODULE, m_product_name_title);
	DDX_Control(pDX, IDC_STATIC_EDIT_DEVICE_IP, m_device_ip_edit);
	DDX_Control(pDX, IDC_STATIC_EDIT_PRODUCT_NAME, m_device_name_edit);
	DDX_Control(pDX, IDC_STATIC_ISP_TITLE, m_isp_mode_title);
}


BEGIN_MESSAGE_MAP(CISPModeSlove, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ISP_CHOOSE_FIRMWARE, &CISPModeSlove::OnBnClickedButtonIspChooseFirmware)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_FIRMWARE, &CISPModeSlove::OnBnClickedButtonUpdateFirmware)
	ON_BN_CLICKED(IDC_BUTTON_ISP_CANCEL, &CISPModeSlove::OnBnClickedButtonIspCancel)

	ON_BN_CLICKED(IDC_RADIO_FROM_SERVER, &CISPModeSlove::OnBnClickedRadioFromServer)
	ON_BN_CLICKED(IDC_RADIO_FROM_HARDISK, &CISPModeSlove::OnBnClickedRadioFromHardisk)
END_MESSAGE_MAP()


// CISPModeSlove message handlers


BOOL CISPModeSlove::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_static();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CStaticEx m_device_ip_title;
CStaticEx m_product_name_title;
CStaticEx m_device_ip_edit;
CStaticEx m_device_name_edit;
CStaticEx m_isp_mode_title;

void CISPModeSlove::Initial_static()
{
	m_isp_mode_title.SetWindowTextW(_T("Your device is in ISP mode !\r\n\
We suggest you to update your firmware."));
	m_isp_mode_title.textColor(RGB(0,0,0));
	//m_isp_mode_title.bkColor(RGB(0,255,255));
	m_isp_mode_title.setFont(28,16,NULL,_T("Arial"));

	//m_device_ip_title.SetWindowTextW(_T(""));
	m_device_ip_title.textColor(RGB(0,0,0));
	//m_device_ip_title.bkColor(RGB(0,255,255));
	m_device_ip_title.setFont(22,16,NULL,_T("Arial"));

	//m_product_name_title.SetWindowTextW(_T(""));
	m_product_name_title.textColor(RGB(0,0,0));
	//m_product_name_title.bkColor(RGB(0,255,255));
	m_product_name_title.setFont(22,16,NULL,_T("Arial"));

	CString temp_ip;
	temp_ip.Format(_T("%u.%u.%u.%u"),need_isp_device.ipaddress[0],need_isp_device.ipaddress[1],need_isp_device.ipaddress[2],need_isp_device.ipaddress[3]);

	m_device_ip_edit.SetWindowTextW(temp_ip);
	m_device_ip_edit.textColor(RGB(0,0,255));
	//m_device_ip_edit.bkColor(RGB(0,255,255));
	m_device_ip_edit.setFont(22,16,NULL,_T("Arial"));


	CString temp_device_name;
	temp_device_name = GetProductName(need_isp_device.product_id);

	m_device_name_edit.SetWindowTextW(temp_device_name);
	m_device_name_edit.textColor(RGB(0,0,255));
	//m_device_name_edit.bkColor(RGB(0,255,255));
	m_device_name_edit.setFont(22,16,NULL,_T("Arial"));

	GetDlgItem(IDC_STATIC_ISP_FIRMWARE_PATH)->SetWindowTextW(_T(""));
	((CButton *)GetDlgItem(IDC_RADIO_FROM_SERVER))->SetCheck(true);

	
	GetDlgItem(IDC_STATIC_ISP_FIRMWARE)->EnableWindow(0);
	GetDlgItem(IDC_STATIC_ISP_FIRMWARE_PATH)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_ISP_CHOOSE_FIRMWARE)->EnableWindow(0);
}


BOOL CISPModeSlove::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}



void CISPModeSlove::OnBnClickedButtonIspChooseFirmware()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(true,_T(""),_T(" "),OFN_HIDEREADONLY ,_T("hex File;bin File|*.hex;*.bin|all File|*.*||"),NULL,0);
	if(IDOK!=dlg.DoModal())
		return ;
	CString temp_path;
	temp_path = dlg.GetPathName();
	GetDlgItem(IDC_STATIC_ISP_FIRMWARE_PATH)->SetWindowTextW(temp_path);

}


void CISPModeSlove::OnBnClickedButtonUpdateFirmware()
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_STATIC_ISP_FIRMWARE_PATH,isp_mode_detect_firmware_path);	
	isp_mode_firmware_auto = ((CButton *)GetDlgItem(IDC_RADIO_FROM_SERVER))->GetCheck(); //返回1表示选上，0表示没选上;
	isp_mode_is_cancel = false;
	PostMessage(WM_CLOSE,NULL,NULL);
}


void CISPModeSlove::OnBnClickedButtonIspCancel()
{
	// TODO: Add your control notification handler code here
	isp_mode_is_cancel = true;
	PostMessage(WM_CLOSE,NULL,NULL);
}




void CISPModeSlove::OnBnClickedRadioFromServer()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_ISP_FIRMWARE)->EnableWindow(0);
	GetDlgItem(IDC_STATIC_ISP_FIRMWARE_PATH)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_ISP_CHOOSE_FIRMWARE)->EnableWindow(0);
}


void CISPModeSlove::OnBnClickedRadioFromHardisk()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_ISP_FIRMWARE)->EnableWindow(1);
	GetDlgItem(IDC_STATIC_ISP_FIRMWARE_PATH)->EnableWindow(1);
	GetDlgItem(IDC_BUTTON_ISP_CHOOSE_FIRMWARE)->EnableWindow(1);
}
