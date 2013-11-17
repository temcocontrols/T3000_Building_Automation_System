#pragma once
#include "afxwin.h"


// CAirflowSettingDlg dialog

class CAirflowSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAirflowSettingDlg)

public:
	CAirflowSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAirflowSettingDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOGAIRFLOW_SETTING };
	BOOL Get_Data_Bit(UINT Data,int n,int N);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocusEditPid2offsetpoint2();
	afx_msg void OnEnKillfocusEditPid2offsetpoint4();
	afx_msg void OnEnKillfocusEditPid2offsetpoint3();
	afx_msg void OnEnKillfocusEditPid2offsetpoint5();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnKillfocusMaxSupply();
	afx_msg void OnEnKillfocusMinSupply();
	void Fresh();
	CString m_strUnit;
	CEdit m_Editor_maxsupply;
	BOOL is_have_AVASensor;
};
