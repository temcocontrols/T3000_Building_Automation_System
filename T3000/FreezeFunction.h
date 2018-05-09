#pragma once


// CFreezeFunction 对话框

class CFreezeFunction : public CDialog
{
	DECLARE_DYNAMIC(CFreezeFunction)

public:
	CFreezeFunction(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFreezeFunction();

// 对话框数据
	enum { IDD = IDD_DIALOG10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeTempture();
//	afx_msg void OnEnChangeOpen();
//	afx_msg void OnEnChangeClose();
	void Fresh_Data();
private:
	int m_temp;
public:
	int m_open;
	int m_close;
//	afx_msg void OnEnUpdateTempture();
//	afx_msg void OnEnSetfocusTempture();
//	afx_msg void OnEnKillfocusOpen();
//	afx_msg void OnEnSetfocusClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedTemp();
	afx_msg void OnBnClickedTemp2();
	afx_msg void OnBnClickedTemp3();
	};
