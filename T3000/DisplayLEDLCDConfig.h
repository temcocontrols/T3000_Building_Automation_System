#pragma once


// CDisplayLEDLCDConfig dialog

class CDisplayLEDLCDConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDisplayLEDLCDConfig)

public:
	CDisplayLEDLCDConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDisplayLEDLCDConfig();

// Dialog Data
	enum { IDD = IDD_DISPLAY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedLed();
    afx_msg void OnBnClickedLcd();
};
