// CBacnetUnitsSelection.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "CBacnetUnitsSelection.h"
#include "afxdialogex.h"


// CBacnetUnitsSelection dialog

IMPLEMENT_DYNAMIC(CBacnetUnitsSelection, CDialogEx)

CBacnetUnitsSelection::CBacnetUnitsSelection(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BACNET_UNITS, pParent)
{

}

CBacnetUnitsSelection::~CBacnetUnitsSelection()
{
}

void CBacnetUnitsSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BACNETIP_UNIT, m_ComboBacnetIpUnit);
}


BEGIN_MESSAGE_MAP(CBacnetUnitsSelection, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBacnetUnitsSelection::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBacnetUnitsSelection::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_BACNETIP_UNIT, &CBacnetUnitsSelection::OnCbnSelchangeComboBacnetipUnit)
END_MESSAGE_MAP()
BOOL CBacnetUnitsSelection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	const char* temp = NULL;
	char temp_char[100];
	CString Add_CString;
	m_ComboBacnetIpUnit.ResetContent();
	for (int i = 0; i < 255; i++)
	{
		CString temp_1;
		temp_1.Empty();
		temp_1.Format(_T("%-3d. "), i);
		Add_CString.Empty();
		memset(temp_char, 0, sizeof(temp_char));
		temp = bactext_engineering_unit_name(i);

		strcpy_s(temp_char, 100, temp);
		::MultiByteToWideChar(CP_ACP, 0, temp_char, strlen(temp_char) + 1, Add_CString.GetBuffer(MAX_PATH), MAX_PATH);
		Add_CString.ReleaseBuffer();
		Add_CString = temp_1 + Add_CString;
		m_ComboBacnetIpUnit.AddString(Add_CString);
	}
	m_ComboBacnetIpUnit.SetCurSel(bac_range_number_choose);
	
	range_cancel = true;
	return FALSE;
}

// CBacnetUnitsSelection message handlers


void CBacnetUnitsSelection::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	range_cancel = false;
	CDialogEx::OnOK();
}


void CBacnetUnitsSelection::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	range_cancel = true;
	CDialogEx::OnCancel();
}


void CBacnetUnitsSelection::OnCbnSelchangeComboBacnetipUnit()
{
	// TODO: Add your control notification handler code here

	bac_range_number_choose = m_ComboBacnetIpUnit.GetCurSel();
}
