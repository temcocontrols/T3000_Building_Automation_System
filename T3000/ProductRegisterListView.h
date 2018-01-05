#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"
#include "MainFrm.h"
#include "afxwin.h"
#include <vector>
using namespace  std;
// CProductRegisterListView form view
struct CustomProductTable_T{
         int ModelNo;
	CString  Poll_YES_NO;
	CString  Bacnet_Object_Name;//CString  Bacnet_Object_Name;
	int      function_code;
    int      Reg_ID;
	int      Counts_Number;
	CString  Value;
	CString  DataFormat;
	CString  Bit_1;
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
	CString  Update_On_Reconnect;
	CString  Default;
	CString  Reg_Description;
	CString  SN;
};
class CProductRegisterListView : public CFormView
{
	DECLARE_DYNCREATE(CProductRegisterListView)

public:
	CProductRegisterListView();           // protected constructor used by dynamic creation
	virtual ~CProductRegisterListView();

public:
	enum { IDD = IDD_DIALOG_CUSTOMER_PRODUCT_VIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
 
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    ListCtrlEx::CListCtrlEx m_register_list;
    void Fresh(void);
	void SetStart(BOOL Start);
    void Initial_List(void);
	void ShowDataInList();
    virtual void OnInitialUpdate();
    void LoadDataSheet();
	void Import_CustomProductTable();
    tree_product m_current_tree_node ;
    vector<CustomProductTable_T> m_register_data_sheet;
    afx_msg LRESULT Fresh_Input_List(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT Change_Input_Item(WPARAM wParam,LPARAM lParam);
    afx_msg void OnNMClickList_output(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnDestroy();
    BOOL m_isopen;
    CWinThread* m_BackgroundTreadHandle;
    CComboBox m_combox_dataformat;
    CComboBox m_combox_property;
    CEdit m_edit_counts;
    CString m_string_dataformat;
    CString m_string_property;
    short m_short_counts;
    CComboBox m_combox_paratypes;
    CString m_string_paratypes;
    BOOL m_isReading;
    int m_sort_type;
	int m_value_format;
    afx_msg void OnBnClickedReadDevice();
    CMFCButton m_upButton;
    CMFCButton m_downButton;

    afx_msg void OnBnClickedButton4();
    afx_msg void OnBnClickedDownbutton();
    afx_msg void OnBnClickedUpbutton();
    UINT m_UINT_delay_loop;
    UINT m_UINT_delay_items;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnEnChangeEditDelayLoop();
    afx_msg void OnEnChangeEditDelayItems();
	 
	 
	int GetFunctionCode(CString FunctionName);
	CString GetFunctionName(int  FunctionCode);
	int GetCaculateCode(CString CalName);
	CString GetCalName(int calcode);

	vector<CString> m_vecYesNo;

	vector<CString> m_vecVariableType;
	vector<CString> m_vecDataFormat;
	vector<CString> m_vecDataRW;
	vector<CString> m_vecBacnetType;

	vector<CString> m_GroupUnits;
	vector<CString> m_UnitValue;


public:
	vector<CString> GetUnitValueByGroupUnit(CString strGroupUnit);
	vector<CString> GetDataFormatByVariable(CString strVariable);
	vector<CString> GetRWByVariableDataFormat(CString strVariable, CString strDataFormat);
	vector<CString> GetBacnetType(CString strVariable, CString strDataFormat, CString RW);
	void FreshOneRowInGrid(int Row, CustomProductTable_T tp, int operator_Grid);

	afx_msg void OnNMDblclkListCustomList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboProductList();
	CComboBox m_combox_productname;
	afx_msg void OnBnClickedButtonExportexcelfile();
};


