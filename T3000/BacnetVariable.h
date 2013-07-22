#pragma once


// CBacnetVariable dialog

class CBacnetVariable : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetVariable)

public:
	CBacnetVariable(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetVariable();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_VARIABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
