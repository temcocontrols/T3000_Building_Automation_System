#pragma once


// CDialogInfo dialog

class CDialogInfo : public CDialog
{
	DECLARE_DYNAMIC(CDialogInfo)

public:
	CDialogInfo(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogInfo();

// Dialog Data
	enum { IDD = IDD_DIALOG_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
