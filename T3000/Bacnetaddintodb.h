#pragma once


// CBacnetaddintodb dialog

class CBacnetaddintodb : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetaddintodb)

public:
	CBacnetaddintodb(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetaddintodb();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_ADD_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBacnetSettingAddBtn();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonSettingAddOk();
	afx_msg void OnBnClickedButtonSettingAddCancel();
	afx_msg void OnBnClickedBacnetSettingClearBtn();
};
