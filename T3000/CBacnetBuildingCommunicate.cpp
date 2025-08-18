// CBacnetBuildingCommunicate.cpp: Implementation file
// CBacnetBuildingCommunicate.cpp: 实现文件
//
#include "stdafx.h"
#include "T3000.h"
#include "CBacnetBuildingCommunicate.h"
#include "afxdialogex.h"


// CBacnetBuildingCommunicate dialog box
// CBacnetBuildingCommunicate 对话框

IMPLEMENT_DYNAMIC(CBacnetBuildingCommunicate, CDialogEx)

CBacnetBuildingCommunicate::CBacnetBuildingCommunicate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BACNET_BUILDING_COMMUNICATE, pParent)
{

}

CBacnetBuildingCommunicate::~CBacnetBuildingCommunicate()
{
}

void CBacnetBuildingCommunicate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetBuildingCommunicate, CDialogEx)
END_MESSAGE_MAP()


// CBacnetBuildingCommunicate message handlers
// CBacnetBuildingCommunicate 消息处理程序


BOOL CBacnetBuildingCommunicate::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_szComm.clear();
	// TODO: Add additional initialization here
	// TODO:  在此添加额外的初始化
	Initialize();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // Exception: OCX property pages should return FALSE
				  // 异常: OCX 属性页应返回 FALSE
}

void CBacnetBuildingCommunicate::Initialize()
{
	GetSerialComPortNumber1(m_szComm);

	((CComboBox*)GetDlgItem(IDC_COMBO_PROTOCOL))->ResetContent();
	for (int i = 0; i < sizeof(BM_Protocol) / sizeof(BM_Protocol[0]); i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_PROTOCOL))->AddString(BM_Protocol[i]);
	}
	// Set default value
	//设置默认值
	((CComboBox*)GetDlgItem(IDC_COMBO_PROTOCOL))->SetWindowText(BM_Protocol[0]);


	((CComboBox*)GetDlgItem(IDC_COMBO_BM_BAUDRATE))->ResetContent();

	for (int x = 0; x < (sizeof(Baudrate_Array) / sizeof(Baudrate_Array[0])); x++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_BM_BAUDRATE))->AddString(Baudrate_Array[x]);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_BM_BAUDRATE))->SetWindowText(Baudrate_Array[9]);

	for (int i = 0; i < m_szComm.size(); i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_BM_COMPORT))->AddString(m_szComm.at(i));
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_BM_COMPORT))->SetWindowText(m_szComm.at(0));

	for (int i = 0; i < g_Vector_Subnet.size(); i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_ENDPOINT))->AddString(g_Vector_Subnet.at(i).StrIP);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO_ENDPOINT))->SetWindowText(g_Vector_Subnet.at(0).StrIP);
	
	GetDlgItem(IDC_EDIT_BM_PORT)->SetWindowText(_T("47808"));
}
