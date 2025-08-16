#pragma once

#include "CM5/ListCtrlEx.h"
#include "afxcmn.h"
// CTstatIconSetting dialog
// CTstatIconSetting �Ի���

class CTstatIconSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CTstatIconSetting)

public:
	// Standard constructor
	CTstatIconSetting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTstatIconSetting();

// Dialog data
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TSTAT_ICON_SETTING };
#endif

protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

