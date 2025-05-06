#if !defined(AFX_EDITLISTCTRL_H__A02CE0B0_FF7B_4A54_8570_12B03905EC3E__INCLUDED_)
#define AFX_EDITLISTCTRL_H__A02CE0B0_FF7B_4A54_8570_12B03905EC3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditListCtrl.h : header file
// Download by http://www.codefans.net

/////////////////////////////////////////////////////////////////////////////
//CItemEdit window

class CItemEdit : public CEdit
{
	// Construction
public:
	CItemEdit();
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemEdit)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	int m_iXPos;
	virtual ~CItemEdit();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CItemEdit)
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CEditListCtrl window
// Download by http://www.codefans.net
class CEditListCtrl : public CListCtrl
{
// Construction
public:
	CEditListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditListCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
private:
	int m_iSubItem;            //
	int m_iItem;               //
	BOOL m_bHighLight;         //
	BOOL m_bFocus;             //
	CItemEdit m_edtItemEdit;   // EditLabelCEdit*
	CString preEditString;     //

public:
	BOOL m_bEditDataChanged;		   //
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITLISTCTRL_H__A02CE0B0_FF7B_4A54_8570_12B03905EC3E__INCLUDED_)
