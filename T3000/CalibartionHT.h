#pragma once


// CCalibartionHT dialog

class CCalibartionHT : public CDialogEx
{
	DECLARE_DYNAMIC(CCalibartionHT)

public:
	CCalibartionHT(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCalibartionHT();

// Dialog Data
	enum { IDD = IDD_DIALOG_TH_CALIBAETION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
};
