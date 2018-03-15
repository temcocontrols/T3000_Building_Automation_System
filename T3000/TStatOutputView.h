#pragma once

#include "CM5/ListCtrlEx.h"
#include "afxwin.h"
#include "CM5/CStatic/staticex.h"

// CTStatOutputView form view

class CTStatOutputView : public CFormView
{
	DECLARE_DYNCREATE(CTStatOutputView)

public:
	CTStatOutputView();           // protected constructor used by dynamic creation
	virtual ~CTStatOutputView();

public:
	enum { IDD = IDD_DIALOG_TSTAT_OUTPUT };

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
ListCtrlEx::CListCtrlEx m_output_list;
public:
    afx_msg LRESULT OutputMessageCallBack(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT Fresh_Output_List(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT Fresh_Output_Item(WPARAM wParam,LPARAM lParam);
    afx_msg void OnNMClickListOutput(NMHDR *pNMHDR, LRESULT *pResult); 
    void Initial_ListFor_Tstat();
    void Fresh_One_Item(int row);
    void Fresh(); 
    virtual void OnInitialUpdate();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    void Initial_Combox_Show_Tstat();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    
    
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};


