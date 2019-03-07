#pragma once
#include "CProgramEditor0.h"
// CNewT3000ProgramEditorDlg dialog

class CNewT3000ProgramEditorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewT3000ProgramEditorDlg)

public:
	CNewT3000ProgramEditorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewT3000ProgramEditorDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_DIALOG_NEW_T3000_PRG_EDIT
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	CProgramEditor0 m_programeditor;
	virtual BOOL OnInitDialog();
};
