#pragma once


// CMyStatusbarCtrl dialog

class CMyStatusbarCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CMyStatusbarCtrl)

public:
	CMyStatusbarCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyStatusbarCtrl();

// Dialog Data
	enum { IDD = IDD_DIALOG_STATUSBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnPaint();
	BOOL	InitStatusBarDC();
	void DrawStatusBar(HDC my_hdc);
	afx_msg LRESULT ShowProgressText(WPARAM wParam,LPARAM lParam);
};
