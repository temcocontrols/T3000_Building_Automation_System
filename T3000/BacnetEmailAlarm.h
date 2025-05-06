#pragma once


// CBacnetEmailAlarm 

class CBacnetEmailAlarm : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetEmailAlarm)

public:
	CBacnetEmailAlarm(CWnd* pParent = NULL);   // 
	virtual ~CBacnetEmailAlarm();

// 
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_EMAIL_ALARM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void Fresh_UI();
};
