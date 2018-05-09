#pragma once


// CWhichOneToChooseDlg dialog

class CWhichOneToChooseDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWhichOneToChooseDlg)

public:
	CWhichOneToChooseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWhichOneToChooseDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_THE_SAME_DEVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_Bool_Check_Com;
	BOOL m_Bool_Check_Net;
	CString m_StrCom;
	CString m_StrNet;
};
