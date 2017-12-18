#pragma once


// CProgramEditorTestDlg dialog

class CProgramEditorTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProgramEditorTestDlg)

public:
	CProgramEditorTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgramEditorTestDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PROGRAMEDITOR_TEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
