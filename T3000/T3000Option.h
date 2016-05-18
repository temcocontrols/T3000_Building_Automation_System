#pragma once


// CT3000Option dialog

class CT3000Option : public CDialogEx
{
	DECLARE_DYNAMIC(CT3000Option)

public:
	CT3000Option(CWnd* pParent = NULL);   // standard constructor
	virtual ~CT3000Option();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_combox_language;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboLanguage();
};
