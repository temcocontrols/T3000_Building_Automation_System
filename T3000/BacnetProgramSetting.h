#pragma once


// CBacnetProgramSetting dialog

class CBacnetProgramSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetProgramSetting)

public:
	CBacnetProgramSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetProgramSetting();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_PROGRAM_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
