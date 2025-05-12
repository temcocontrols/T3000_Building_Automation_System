#pragma once


// CBacnetEmailAlarm 对话框

class CBacnetEmailAlarm : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetEmailAlarm)

public:
	CBacnetEmailAlarm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBacnetEmailAlarm();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_EMAIL_ALARM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void Fresh_UI();
};
