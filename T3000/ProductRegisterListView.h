#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"
#include "MainFrm.h"
#include "afxwin.h"
// CProductRegisterListView form view
struct CustomProductTable_T{
    int ModelNo;
    CString  Reg_Description;
    int Reg_ID;
    CString Para_Type;
	int function_code;
    int  Counts_Number;
    CString  Property;
    CString Value;
    CString DataFormat;
	CString SN;
	int caculate;
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
	CString m_array_modbus_function[8];
	CString m_array_caculate[3];
	
	 
};


