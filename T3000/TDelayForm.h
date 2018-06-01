#pragma once
// TDelayForm 对话框

class TDelayForm : public CDialog
{
	DECLARE_DYNAMIC(TDelayForm)
	int		m_address;
	CString	m_output1_delay_off;
	CString	m_output1_delay_on;
	CString	m_output2_delay_off;
	CString	m_output3_delay_off;
	CString	m_output4_delay_off;
	CString	m_output5_delay_off;
	CString	m_output2_delay_on;
	CString	m_output3_delay_on;
	CString	m_output4_delay_on;
	CString	m_output5_delay_on;
	CString	m_cycling;
	CString	m_change_over;
public:
	TDelayForm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TDelayForm();

// 对话框数据
	enum { IDD = IDD_DELAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFresh();
	afx_msg void OnWriteChanges();
	afx_msg void OnKillfocusOutput1DelayOffEdit();
	afx_msg void OnKillfocusOutput2DelayOffEdit();
	afx_msg void OnKillfocusOutput3DelayOffEdit();
	afx_msg void OnKillfocusOutput4DelayOffEdit();
	afx_msg void OnKillfocusOutput5DelayOffEdit();
	afx_msg void OnKillfocusOutput1DelayOnEdit();
	afx_msg void OnKillfocusOutput2DelayOnEdit();
	afx_msg void OnKillfocusOutput3DelayOnEdit();
	afx_msg void OnKillfocusOutput4DelayOnEdit();
	afx_msg void OnKillfocusOutput5DelayOnEdit();
	afx_msg void OnKillfocusCyclingEdit();
	afx_msg void OnKillfocusChangeOverEdit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnSetfocusOutput1DelayOffEdit();
	afx_msg void OnSetfocusOutput2DelayOffEdit();
	afx_msg void OnSetfocusOutput3DelayOffEdit();
	afx_msg void OnSetfocusOutput4DelayOffEdit();
	afx_msg void OnSetfocusOutput5DelayOffEdit();
	afx_msg void OnSetfocusOutput1DelayOnEdit();
	afx_msg void OnSetfocusOutput2DelayOnEdit();
	afx_msg void OnSetfocusOutput3DelayOnEdit();
	afx_msg void OnSetfocusOutput4DelayOnEdit();
	afx_msg void OnSetfocusOutput5DelayOnEdit();
	afx_msg void OnSetfocusCyclingEdit();
	afx_msg void OnSetfocusChangeOverEdit();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//	int float_CString_to_atoi(CString a,char *p);//this function only use in this class	
};
