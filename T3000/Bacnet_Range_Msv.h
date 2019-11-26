#pragma once
#include "CM5/ListCtrlEx.h"
#include "afxcmn.h"

// CBacnet_Range_Msv 对话框

class CBacnet_Range_Msv : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnet_Range_Msv)

public:
	CBacnet_Range_Msv(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBacnet_Range_Msv();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_RANGE_MSV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    ListCtrlEx::CListCtrlEx m_range_msv_list;
    void Initial_List();
    afx_msg LRESULT Fresh_Msv_Item(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT Fresh_Msv_List(WPARAM wParam, LPARAM lParam);
};

const int CUSTOMER_MSV_RANGE_NUM = 0;
const int CUSTOMER_MSV_RANGE_DESCRIPTION = 1;
const int CUSTOMER_MSV_RANGE_VALUE = 2;
const int CUSTOMER_MSV_RANGE_COL_NUMBER = 3;
