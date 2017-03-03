#pragma once


// CBacnetVarCusRang dialog

class CBacnetVarCusRang : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetVarCusRang)

public:
	CBacnetVarCusRang(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetVarCusRang();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_CUSTOMER_VARIABLE_RANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBacVarOk();
	afx_msg void OnBnClickedButtonBacVarCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Initial_Static_Unit();
};
