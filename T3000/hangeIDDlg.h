#pragma once


// ChangeIDDlg dialog

class CChangeIDDlg : public CDialog
{
	DECLARE_DYNAMIC(CChangeIDDlg)

public:
	CChangeIDDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CChangeIDDlg();

// Dialog Data
	enum { IDD = IDD_CAHNGEIDDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int m_nChange;
private:
	DWORD m_dwTimeStart;
	//BOOL m_bShowOkBtn;
	int m_nTime;//;
	CString m_strPromtionText;

	UINT m_serialdb;
	int m_iddb;
	UINT m_serialScaned;
	int m_idScanned;
	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void SetPromtionTxt(UINT SerialDb,int idDb,UINT SerialScanned,int idScanned);
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnKillfocusRadio1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedChangedatabseradio();
};
