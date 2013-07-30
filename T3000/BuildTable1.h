#pragma once
#include "afxcmn.h"
#include "resource.h"
// CBuildTable1 对话框

class CBuildTable1 : public CDialog
{
	DECLARE_DYNAMIC(CBuildTable1)

public:
	CBuildTable1(bool table1_table2,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBuildTable1();

	// 对话框数据
	enum { IDD = IDD_BUILD_TABLE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider10(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider11(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider7(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider8(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider9(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider10(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSlider11(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnEnSetfocusEdit8();
	afx_msg void OnEnSetfocusEdit9();
	afx_msg void OnEnSetfocusEdit13();
	afx_msg void OnEnSetfocusEdit14();
	afx_msg void OnEnSetfocusEdit15();
	afx_msg void OnEnSetfocusEdit16();
	afx_msg void OnEnSetfocusEdit17();
	afx_msg void OnEnSetfocusEdit18();
	afx_msg void OnEnSetfocusEdit19();
	afx_msg void OnEnSetfocusEdit20();
	afx_msg void OnEnSetfocusEdit12();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit8();
	afx_msg void OnEnChangeEdit9();
	afx_msg void OnEnChangeEdit13();
	afx_msg void OnEnChangeEdit14();
	afx_msg void OnEnChangeEdit15();
	afx_msg void OnEnChangeEdit16();
	afx_msg void OnEnChangeEdit17();
	afx_msg void OnEnChangeEdit18();
	afx_msg void OnEnChangeEdit19();
	afx_msg void OnEnChangeEdit20();
	afx_msg void OnEnChangeEdit12();
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnKillfocusEdit8();
	afx_msg void OnEnKillfocusEdit9();
	afx_msg void OnEnKillfocusEdit13();
	afx_msg void OnEnKillfocusEdit14();
	afx_msg void OnEnKillfocusEdit15();
	afx_msg void OnEnKillfocusEdit16();
	afx_msg void OnEnKillfocusEdit17();
	afx_msg void OnEnKillfocusEdit18();
	afx_msg void OnEnKillfocusEdit19();
	afx_msg void OnEnKillfocusEdit20();
	afx_msg void OnEnKillfocusEdit12();

	afx_msg void OnEnKillfocusEdit10();
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnEnSetfocusEdit10();
	afx_msg void OnBnClickedCancel();
	afx_msg short GetValueFromEdtCtrol(UINT ID_EDIT);
	afx_msg CString GetValueFromRegister(short nValue);
	afx_msg void OnBnClickedSaveok();

	void to_fresh();
	void refresh();
	void SaveInfoDataToRegister();

	void refresh_rule(float fMin,float fMax);

	BOOL CheckDataisRight();
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int init_rulePos(void);

	CString GetUnit();

private:
	int m_nMax;
	int m_nMin;
	BOOL m_bIncreasingMode;
	bool IsModfied ;	
	float m_version;


	int m_slider1_i;
	int m_slider2_i;
	int m_slider3_i;
	int m_slider4_i;
	int m_slider5_i;
	int m_slider6_i;
	int m_slider7_i;
	int m_slider8_i;
	int m_slider9_i;
	int m_slider10_i;
	int m_slider11_i;
	CString m_max;
	CString m_min;
	bool m_table1_table2;//false is table1,true is table2;
	CString m_units_s;

	CSliderCtrl m_slider1_ctrl;
	CSliderCtrl m_slider2_ctrl;
	CSliderCtrl m_slider3_ctrl;
	CSliderCtrl m_slider4_ctrl;
	CSliderCtrl m_slider5_ctrl;
	CSliderCtrl m_slider6_ctrl;
	CSliderCtrl m_slider7_ctrl;
	CSliderCtrl m_slider8_ctrl;
	CSliderCtrl m_slider9_ctrl;
	CSliderCtrl m_slider10_ctrl;
	CSliderCtrl m_slider11_ctrl;
};
