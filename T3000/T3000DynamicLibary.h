

#if !defined(AFX_ELEMENTBAR_H__3BAE8904_36AA_11D2_809C_ABC7258C6120__INCLUDED_)
#define AFX_ELEMENTBAR_H__3BAE8904_36AA_11D2_809C_ABC7258C6120__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PropertyBar.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDynamicHelp window
struct TEXTBLOCK;


class CDynamicHelp : public CControlBar
{
	typedef CControlBar inherited;
	void SizeToolBar(int length, bool vert= false);
	CToolTipCtrl tool_tip_;
	CRichEditCtrl help_wnd_;
	int header_height_;
	CToolBarCtrl close_wnd_;

// Construction
public:
	CDynamicHelp();

// Attributes
public:
	void SetContextHelp(const TCHAR* text, const TCHAR* header= 0);

	void DisplayHelp(const CString& line, int word_start, int word_end);

// Operations
public:
	bool Create(CWnd* parent_wnd, UINT id);


public:
	virtual ~CDynamicHelp();
	virtual void DoPaint(CDC* dc);
	virtual CSize CalcFixedLayout(BOOL stretch, BOOL horz);
	virtual CSize CalcDynamicLayout(int length, DWORD mode);
	virtual void OnUpdateCmdUI(CFrameWnd* target, BOOL disable_if_no_hndler);
	CSize CalcLayout(DWORD mode, int length);
	virtual void OnBarStyleChange(DWORD old_style, DWORD new_style);


protected:
	//{{AFX_MSG(CDynamicHelp)
	afx_msg BOOL OnEraseBkgnd(CDC* dc);
	afx_msg void OnSize(UINT type, int cx, int cy);
	afx_msg void OnNcPaint();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT create_struct);
	afx_msg void OnCloseWnd();
	//}}AFX_MSG
	afx_msg BOOL OnToolTipGetText(UINT uId, NMHDR* nm_hdr, LRESULT* result);

	DECLARE_MESSAGE_MAP()

	static CString wnd_class_;
	void RegisterWndClass();
	CSize default_size_;
	void Resize();

	LRESULT OnDelayedResize(WPARAM, LPARAM);
};



#endif
