#pragma once


// CTstatAQI_Detail 对话框

class CTstatAQI_Detail : public CDialogEx
{
	DECLARE_DYNAMIC(CTstatAQI_Detail)

public:
	CTstatAQI_Detail(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTstatAQI_Detail();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AQI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
