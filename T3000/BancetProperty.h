#pragma once
#include "afxcmn.h"
#include "CM5\CStatic\staticex.h"
#include "CM5/ListCtrlEx.h"
// CBancetProperty 对话框

class CBancetProperty : public CDialogEx
{
	DECLARE_DYNAMIC(CBancetProperty)

public:
	CBancetProperty(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBancetProperty();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_PROPERTY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    ListCtrlEx::CListCtrlEx m_bacnet_property_list;
    void Initial_List();
    void SetParameter(int nBACNET_PROPERTY_ID);
    void SetBacnetReadString(CString temp_cs);
    void SetTitle(CString temp_title);
    int n_property_id;
    CString cs_bacnet_string;
    CString cs_window_title;
};
