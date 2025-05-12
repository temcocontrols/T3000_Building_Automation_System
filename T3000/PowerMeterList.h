#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"
#include "afxwin.h"
#include "CM5\CStatic\staticex.h"
// CPowerMeterList 对话框

class CPowerMeterList : public CDialogEx
{
	DECLARE_DYNAMIC(CPowerMeterList)

public:
	CPowerMeterList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPowerMeterList();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POWER_METER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    ListCtrlEx::CListCtrlEx m_powermeter_list;
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void Initial_List();
    afx_msg LRESULT Fresh_PowerMeter_List(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT Fresh_PowerMeter_Item(WPARAM wParam, LPARAM lParam);

   
    CStaticEx m_powermeter_title;
    afx_msg void OnBnClickedButtonClearPowerLaenergt();
    afx_msg void OnBnClickedButtonClearPowerLaenergtU();
    afx_msg void OnBnClickedButtonClearPowerLaenergtV();
    afx_msg void OnBnClickedButtonClearPowerLaenergtW();
    afx_msg void OnBnClickedRadio3();
    afx_msg void OnBnClickedRadioExternalCt();
    afx_msg void OnBnClickedRadioPowerInternalCt();
    afx_msg void OnEnChangeEdit2();
    afx_msg void OnEnChangeEdit4();
    afx_msg void OnEnKillfocusEditRatioValue();
    afx_msg void OnStnClickedPowermeterTitle();
};
