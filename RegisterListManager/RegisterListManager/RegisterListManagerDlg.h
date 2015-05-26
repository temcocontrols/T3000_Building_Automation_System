
// RegisterListManagerDlg.h : header file
//


#pragma once
#include "ListCtrlEx.h"
#include "CStatic/staticex.h"
#include "afxwin.h"


// CRegisterListManagerDlg dialog
class CRegisterListManagerDlg : public CDialogEx
{
// Construction
public:
	CRegisterListManagerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_REGISTERLISTMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    ListCtrlEx::CListCtrlEx m_Product_ListCtrl;
    ListCtrlEx::CListCtrlEx m_RegisterListCtrl;
    CStaticEx m_label_product;
    CStaticEx m_label_registerlist;
    CStaticEx m_data_loading;
public:
    void Load_ProductList();
    void Initial_ProductList();
    void Load_RegisterList();
    void Initial_RegistersList();
    afx_msg void OnNMDblclkListProduct(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg LRESULT Fresh_Lists(WPARAM wParam,LPARAM lParam);
    
    afx_msg void OnNMDblclkListRegisterlist(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnUpdateSystemChange(CCmdUI *pCmdUI);
//    afx_msg void OnSystemProjectmanagerment();
    afx_msg void OnUpdateSystemProjectmanagerment(CCmdUI *pCmdUI);
    afx_msg void OnUpdateSystemUsersmanagerment(CCmdUI *pCmdUI);
    afx_msg void OnSystemExit();
    afx_msg void OnSystemExportregisterlists();
    afx_msg void OnSystemChange();
    afx_msg void OnSystemUsersmanagerment();
    afx_msg void OnSystemProjectmanagerment();
    afx_msg void OnBnClickedMfcbuttonExportHeadFile();
    afx_msg void OnBnClickedMfcbuttonPassword();
    afx_msg void OnBnClickedMfcbuttonUsersmanagerment();
    afx_msg void OnBnClickedMfcbuttonProjectmanagerment();
};
