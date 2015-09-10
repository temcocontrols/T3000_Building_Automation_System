// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
#pragma once
#include "ImageTreeCtrl.h"
//#include "RenameDlg.h"
class CWorkspaceBar : public CDockablePane
{
public:
	CWorkspaceBar();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// Attributes
protected:
	//CMFCShellTreeCtrl m_wndTree;

	CString m_name_old;
	CString m_name_new;


	HTREEITEM m_hSelItem;
	int	  m_level;
	CImageList m_image_list;
public:
	CImageTreeCtrl m_TreeCtrl;
	//CTreeCtrl m_wndTree;
	//CRenameDlg m_renamedlg;
	// Operations
public:
	BOOL  UpdateDataToDB();

	// Overrides

	// Implementation
public:
	virtual ~CWorkspaceBar();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFolderSeletedChanged(NMHDR* pNMHDR, LRESULT* pResult);
//	afx_msg void OnKeyDownChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnEndlabeleditTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBeginlabeleditTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSelectChanged_Tree(NMHDR *pNMHDR, LRESULT *pResult);


	
	//afx_msg void OnKYDOWNClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFolderBrowerBtn();
	DECLARE_MESSAGE_MAP()


public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};
