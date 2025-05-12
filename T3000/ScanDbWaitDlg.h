#pragma once
#include "afxcmn.h"
//#include "TStatScanner.h"
#include "CM5/ListCtrlEx.h"
#include "afxwin.h"
// CScanDbWaitDlg dialog

#include "CM5/CStatic/staticex.h"

class CTStatScanner;
class CScanDbWaitDlg : public CDialog
{
	DECLARE_DYNAMIC(CScanDbWaitDlg)

public:
	CScanDbWaitDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScanDbWaitDlg();

// Dialog Data
	enum { IDD = IDD_SCANDBWAITDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedExitbutton();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


	void SetPromtText(CString strInfo);
	void SetTStatScaner(CTStatScanner* pScanner);
	void Initial_List();
	CString m_strInfopromp;
	CString m_strDot;

	CTStatScanner*		m_pScaner;
	ListCtrlEx::CListCtrlEx m_scan_com_list;
    CStaticEx m_waiting_title;
    int n_time_count; //用于计数。
    afx_msg void OnSize(UINT nType, int cx, int cy);
};

const int SCAN_MODE = 0;
//const int SCAN_BAUDRATE = 1;
//const int SCAN_SKIP = 1;
const int SCAN_STATUS = 1;
const int SCAN_FOUND = 2;
const int SCAN_NOTES = 3;

const int SCAN_ALL_COLUME = 4;
