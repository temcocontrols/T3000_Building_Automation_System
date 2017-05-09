#pragma once
#include "msflexgrid1.h"
#include "afxwin.h"
#include "DialogInfo.h"
// CAllNodesDiaolg dialog

class CAllNodesDiaolg : public CDialog
{
	DECLARE_DYNAMIC(CAllNodesDiaolg)

public:
	CAllNodesDiaolg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAllNodesDiaolg();

	// Dialog Data
	enum { IDD = IDD_ALLNODESDIALOG };
	BOOL m_bChanged;
private:

	int m_nCurRow;
	int m_nCurCol;
	CMsflexgrid m_FlexGrid;

private:
	CString m_strMainBuildingName;
	CString m_strSubNetName;
	CString m_strID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	void ReloadAddBuildingDB();
	bool IsOnline(int SN);
	void PutRowColor(int Row, unsigned long rgb);
	void SetBuildingMainName(CString strBuildName);
	afx_msg void OnBnClickedAddbutton();
	afx_msg void OnBnClickedDelbutton();
	afx_msg void OnBnClickedDelallbutton();
	afx_msg void OnBnClickedExitbutton();
	CButton m_AddBtn;
	CButton m_delBtn;
	CButton m_DelBtn;
	CButton m_ExitBtn;
	afx_msg void OnBnClickedCancel();
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgrid1();

	CComboBox m_productCombox;
	CEdit m_InputTextEdt;

	CComboBox m_SubLstCombox;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnEnKillfocusTextedit();
	afx_msg void OnEnSetfocusTextedit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedArd();
	afx_msg void OnBnClickedDelbuttonOffline();

	CDialogInfo *m_pDialogInfo;

};
