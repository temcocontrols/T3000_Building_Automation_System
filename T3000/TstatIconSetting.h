#pragma once

#include "CM5/ListCtrlEx.h"
#include "afxcmn.h"
// CTstatIconSetting 对话框

class CTstatIconSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CTstatIconSetting)

public:
	CTstatIconSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTstatIconSetting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TSTAT_ICON_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void Initial_List();
    ListCtrlEx::CListCtrlEx m_tstat_icon_list;
    afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonIconOk();
};

const int TSTAT_ICON_NAME = 0;
const int TSTAT_ICON_NA = 1;
const int TSTAT_ICON_K1 = 2;
const int TSTAT_ICON_K2 = 3;
const int TSTAT_ICON_K3 = 4;
const int TSTAT_ICON_K4 = 5;
const int TSTAT_ICON_K5 = 6;
const int TSTAT_ICON_AO1 = 7;
const int TSTAT_ICON_AO2 = 8;

const int MAX_COL_COUNT = 9;

