// BacnetToolReadProperty.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetToolReadProperty.h"
#include "afxdialogex.h"


// CBacnetToolReadProperty dialog

IMPLEMENT_DYNAMIC(CBacnetToolReadProperty, CDialogEx)

CBacnetToolReadProperty::CBacnetToolReadProperty(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetToolReadProperty::IDD, pParent)
{

}

CBacnetToolReadProperty::~CBacnetToolReadProperty()
{
}

void CBacnetToolReadProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BAC_SERVICE_CHOICE, m_combo_service_choice);
	DDX_Control(pDX, IDC_COMBO_BAC_OBJECT_IDENTIFIER, m_combo_object_identifier);
	DDX_Control(pDX, IDC_COMBO_BAC_PROPERTY, m_combo_property_identifier);
}


BEGIN_MESSAGE_MAP(CBacnetToolReadProperty, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_BAC_SEND, &CBacnetToolReadProperty::OnBnClickedBtnBacSend)
END_MESSAGE_MAP()


// CBacnetToolReadProperty message handlers


void CBacnetToolReadProperty::OnBnClickedBtnBacSend()
{
	// TODO: Add your control notification handler code here
}


BOOL CBacnetToolReadProperty::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CBacnetToolReadProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_combo();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetToolReadProperty::Initial_combo()
{
	m_combo_service_choice.AddString(_T("Read Property (12)"));
	m_combo_service_choice.SetCurSel(0);
	GetDlgItem(IDC_TOOL_NETWORK_EDIT)->SetWindowTextW(_T("0"));

	const char * temp = NULL;
	
	CString Add_CString;
	char temp_char[100];
	CString temp_1;
	for (int i=0;i<=OBJECT_LIGHTING_OUTPUT;i++)
	{
		temp_1.Empty();
		temp_1.Format(_T("( %d )"),i);
		Add_CString.Empty();
		memset(temp_char,0,sizeof(temp_char));
		temp = bactext_object_type_name(i);
		strcpy_s(temp_char,100,temp);
		::MultiByteToWideChar(CP_ACP,0,temp_char,strlen(temp_char) + 1,Add_CString.GetBuffer(MAX_PATH),MAX_PATH);
		Add_CString.ReleaseBuffer();
		Add_CString = Add_CString + temp_1;
		m_combo_object_identifier.AddString(Add_CString);
		m_combo_object_identifier.SetCurSel(8);
	}

	for (int i=0;i<=PROP_EGRESS_ACTIVE;i++)
	{
		temp_1.Empty();
		temp_1.Format(_T("( %d )"),i);
		Add_CString.Empty();
		memset(temp_char,0,sizeof(temp_char));
		temp = bactext_property_name(i);
		strcpy_s(temp_char,100,temp);
		::MultiByteToWideChar(CP_ACP,0,temp_char,strlen(temp_char) + 1,Add_CString.GetBuffer(MAX_PATH),MAX_PATH);
		Add_CString.ReleaseBuffer();
		Add_CString = Add_CString + temp_1;
		m_combo_property_identifier.AddString(Add_CString);
		m_combo_property_identifier.SetCurSel(PROP_OBJECT_NAME);
	}

	
	//bactext_property_name
//	m_combo_object_identifier
}

