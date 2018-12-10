#pragma once
#include "CM5/ListCtrlEx.h"
#include "CM5/CStatic/staticex.h"

// CTStatInputView form view

class CTStatInputView : public CFormView
{
	DECLARE_DYNCREATE(CTStatInputView)

public:
	CTStatInputView();           // protected constructor used by dynamic creation
	virtual ~CTStatInputView();

public:
	enum { IDD = IDD_DIALOG_TSTAT_INPUT };
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
afx_msg LRESULT InputMessageCallBack(WPARAM wParam, LPARAM lParam);
afx_msg LRESULT Fresh_Input_List(WPARAM wParam,LPARAM lParam);
afx_msg LRESULT Fresh_Input_Item(WPARAM wParam,LPARAM lParam);
afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
public:
void Fresh();
void Initial_ListFor_Tstat();
void Fresh_One_Item(int row);
public:
ListCtrlEx::CListCtrlEx m_input_list;
virtual void OnInitialUpdate();
afx_msg void OnSize(UINT nType, int cx, int cy);
 CWinThread* m_Fresh_BackgroundThreadHandle;
 virtual BOOL PreTranslateMessage(MSG* pMsg);
 afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

 bool b_hum_sensor;   //用于判断TSTAT 输入 有无 Hum
 bool b_co2_sensor;   //输入 有无 CO2 用此标志 来判断是否显示在输入想里;
 bool b_lux_sensor;   // 光强
};


