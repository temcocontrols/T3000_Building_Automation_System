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

class CWorkspaceBar : public CDockablePane
{
public:
	CWorkspaceBar();

// Attributes
protected:
	//CMFCShellTreeCtrl m_wndTree;
	


	CImageList m_image_list;
public:
		CImageTreeCtrl m_TreeCtrl;
		//CTreeCtrl m_wndTree;
// Operations
public:

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
	afx_msg void OnTvnEndlabeleditTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBeginlabeleditTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFolderBrowerBtn();
	DECLARE_MESSAGE_MAP()


public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
