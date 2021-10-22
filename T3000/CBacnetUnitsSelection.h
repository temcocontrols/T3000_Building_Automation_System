#pragma once


// CBacnetUnitsSelection dialog

class CBacnetUnitsSelection : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetUnitsSelection)

public:
	CBacnetUnitsSelection(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CBacnetUnitsSelection();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_UNITS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComboBox m_ComboBacnetIpUnit;
	afx_msg void OnCbnSelchangeComboBacnetipUnit();
};
