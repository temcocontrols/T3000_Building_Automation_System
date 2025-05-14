#include "stdafx.h"
#include "T3000.h"
#include "afxdialogex.h"
#include "CT3000_Options.h"
#include "global_function.h"

// CT3000_Options 对话框

IMPLEMENT_DYNAMIC(CT3000_Options, CDialogEx)

CT3000_Options::CT3000_Options(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OPTIONS, pParent)
{

}

CT3000_Options::~CT3000_Options()
{
}

void CT3000_Options::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CT3000_Options, CDialogEx)
	ON_BN_CLICKED(IDOK, &CT3000_Options::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_IP_BIND, &CT3000_Options::OnBnClickedRadioIpBind)
	ON_BN_CLICKED(IDC_RADIO_IPAUTO, &CT3000_Options::OnBnClickedRadioIpauto)
END_MESSAGE_MAP()


// CT3000_Options 消息处理程序


BOOL CT3000_Options::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//读取配置文件g_configfile_path
	//int auto_local_ip = 1; //自动使用本地所有ip端口扫描;
	CString local_ip_scan_address; //bind本地ip地址;
	 auto_local_ip = GetPrivateProfileInt(_T("Parameter"), _T("AutoLocalIP"), 1, g_configfile_path);
	 if (auto_local_ip)
	 {
		 ((CButton*)GetDlgItem(IDC_RADIO_IPAUTO))->SetCheck(true);
		 ((CButton*)GetDlgItem(IDC_RADIO_IP_BIND))->SetCheck(false);
		 ((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_LOCAL_IP))->EnableWindow(false);
	 }
	 else
	 {
		 ((CButton*)GetDlgItem(IDC_RADIO_IPAUTO))->SetCheck(false);
		 ((CButton*)GetDlgItem(IDC_RADIO_IP_BIND))->SetCheck(true);
		 ((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_LOCAL_IP))->EnableWindow(true);
		 //从配置文件获取local_ip_scan_address
		 GetPrivateProfileStringW(_T("Parameter"), _T("LocalIP"), _T(""), local_ip_scan_address.GetBuffer(MAX_PATH), MAX_PATH, g_configfile_path);
		 local_ip_scan_address.ReleaseBuffer();
		 //将 local_ip_scan_address 填充 至IDC_IPADDRESS_LOCAL_IP
		 BYTE address1, address2, address3, address4;
		 CStringArray temp_array;
		 SplitCStringA(temp_array, local_ip_scan_address, _T("."));
		 if (temp_array.GetSize() == 4)
		 {
			 address1 = (BYTE)_ttoi(temp_array.GetAt(0));
			 address2 = (BYTE)_ttoi(temp_array.GetAt(1));
			 address3 = (BYTE)_ttoi(temp_array.GetAt(2));
			 address4 = (BYTE)_ttoi(temp_array.GetAt(3));
			 ((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_LOCAL_IP))->SetAddress(address1, address2, address3, address4);
		 }

	 }
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CT3000_Options::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//检查checkbox  IDC_RADIO_IPAUTO是否选中
	if (((CButton*)GetDlgItem(IDC_RADIO_IPAUTO))->GetCheck())
	{
		WritePrivateProfileString(_T("Parameter"), _T("AutoLocalIP"), _T("1"), g_configfile_path);
		auto_local_ip = 1;
	}
	else
	{
		WritePrivateProfileString(_T("Parameter"), _T("AutoLocalIP"), _T("0"), g_configfile_path);
		auto_local_ip = 0;
		BYTE address1, address2, address3, address4;
		((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_LOCAL_IP))->GetAddress(address1, address2, address3, address4);
		local_ip_scan_address.Format(_T("%d.%d.%d.%d"), address1, address2, address3, address4);
		WritePrivateProfileString(_T("Parameter"), _T("LocalIP"), local_ip_scan_address, g_configfile_path);


	}





	CDialogEx::OnOK();
}


void CT3000_Options::OnBnClickedRadioIpBind()
{
	// TODO: 在此添加控件通知处理程序代码
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_LOCAL_IP))->EnableWindow(true);
}


void CT3000_Options::OnBnClickedRadioIpauto()
{
	// TODO: 在此添加控件通知处理程序代码
	((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_LOCAL_IP))->EnableWindow(false);
}
