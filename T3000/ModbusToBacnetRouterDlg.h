#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "CM5/ListCtrlEx.h"
#include <vector>
#include <algorithm>
using namespace std;

// CModbusToBacnetRouterDlg dialog
struct ModbusToBacnetRouter
{
	CString  Poll_YES_NO;
	CString  Bacnet_Object_Name;
	CString  Modbus_Variable_Type;
	CString  From1_To_65536;
	CString  Data_Format;
	CString  Bit_1	;
	CString  Low_Actual;
	CString  High_Actual;
	CString  Low_Scale;
	CString  High_Scale;
	CString  Read_Only_Or_RW;
	CString  Bacnet_Type;
	CString  Bacnet_Object_Description;
	CString  COV_Increment;
	CString  Unit_Group;
	CString  Unit_Value;
	CString  Grouping_YES_NO;
    CString   Update_On_Reconnect;
};
class CModbusToBacnetRouterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModbusToBacnetRouterDlg)

public:
	CModbusToBacnetRouterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModbusToBacnetRouterDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MODBUS_BACNET_ROUTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlEx::CListCtrlEx m_datalist;
	CComboBox m_combox_productname;
	CComboBox m_combox_rev;
	vector<ModbusToBacnetRouter> m_vecMd2BT;
	vector<CString> m_vecYesNo;

	vector<CString> m_vecVariableType;
	vector<CString> m_vecDataFormat;
	vector<CString> m_vecDataRW;
	vector<CString> m_vecBacnetType;

	vector<CString> m_GroupUnits;
	vector<CString> m_UnitValue;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonBlxfile();
	afx_msg LRESULT Fresh_Input_Item(WPARAM wParam, LPARAM lParam);
public:
	vector<CString> GetUnitValueByGroupUnit(CString strGroupUnit);
	vector<CString> GetDataFormatByVariable(CString strVariable);
	vector<CString> GetRWByVariableDataFormat(CString strVariable, CString strDataFormat);
	vector<CString> GetBacnetType(CString strVariable, CString strDataFormat,CString RW);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
