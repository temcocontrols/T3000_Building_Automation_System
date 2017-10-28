#pragma once
#include "afxwin.h"
#include "msflexgrid1.h"
#include "PressureSensorForm.h"
#include "OXBitmapButton.h"

class COutPutDlg : public CDialog
{
	DECLARE_DYNAMIC(COutPutDlg)

public:
	COutPutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COutPutDlg();
	/*CPressureSensorForm pressuresensorview;*/
	// Dialog Data
	enum { IDD = IDD_OUTPUTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	void FreshGrids();
	 
	void FreshGrid_PID1();
	void FreshGrid_PID2();
	void FreshGrid_PID3();
	void put_fan_variable();
	int get_real_fan_select();
    int set_real_fan_select();
	void OnWrite(int bflexgrid1_or_2,int col,int row);
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgrid1();
	void ClickMsflexgrid2();
	afx_msg void OnCbnSelchangeFanmode();
	afx_msg void OnCbnSelchangeCbfan();
	afx_msg void OnEnKillfocusPid1Heatstageedit();
	afx_msg void OnEnKillfocusPid1coolstageedit();
	afx_msg void OnBnClickedFanautocheck();

	afx_msg void OnCbnKillfocusValueitemcombo();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeValueitemcombo();

public:
	//unsigned short multi_register_value[513];
	CWinThread* pParamBackFresh;


	CComboBox m_fan;
	CComboBox m_fan_mode_ctrl;
	CMsflexgrid m_FlexGrid1;
	CMsflexgrid m_FlexGrid2;
	CMsflexgrid m_FlexGrid3;
	CButton m_fanAutoCheck;
	BOOL m_bFanAutoOnly;

	int m_bflexgrid1_or_2;//true:grid 2;false:grid 1;
	BOOL m_bOut4PWM;
	BOOL m_bOut5PWM;
	BOOL m_bFloat;

	int m_PID1_heat_stages;
	int m_PID1_cool_stages;
	int m_nCurRow;
	int m_nCurCol;
	int m_nmoduleType;
	CEdit m_PID1_heatEdit;
	CEdit m_PID1_coolEdit;

	int m_pwm_row1;
	int m_pwm_row2;
	CEdit m_DescriptEdt;
	CComboBox m_ItemValueCombx;
	CEdit m_PID2_heatEdit;
	CEdit m_PID2_coolEdit;
	int m_PID2_heat_stages;
	int m_PID2_cool_stages;

	int m_PID3_heat_stages;
	int m_PID3_cool_stages;

	CString strOldText;
	int i_104_pid1,i_268_pid2;


	afx_msg void OnEnKillfocusPid2Heatstageedit2();
	afx_msg void OnEnKillfocusPid2coolstageedit2();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnKillfocusDescriptedit();
	afx_msg void OnEnSetfocusDescriptedit();


	void FreshGrid_PID1tstat6();
	void FreshGrid_PID2tstat6();
	void FreshGrid_PID2tstat5GLessthan368();
	CString strdemo;
	CComboBox m_fan2;
	int m_pids;
	long pid2lrow;//2.5.0.94
	long pid1lrow;//2.5.0.96

	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedRefresh();
	void FLEX_GRID1_PUT_COLOR_STR(int ,int ,CString);
	void FLEX_GRID2_PUT_COLOR_STR(int ,int ,CString);
	void FLEX_GRID3_PUT_COLOR_STR(int ,int ,CString);
	void FLEX_GRID1_PUT_STR(int ,int ,CString);
	void FLEX_GRID2_PUT_STR(int ,int ,CString);
	void FLEX_GRID3_PUT_STR(int ,int ,CString);
	CEdit m_PID3_heatEdit;
	CEdit m_PID3_coolEdit;
	void ClickMsflexgrid3();
	afx_msg void OnEnKillfocusPid3Heatstageedit();
	afx_msg void OnEnKillfocusPid3coolstageedit();
	
    afx_msg void OnEnKillfocusEditFanOffName();
    afx_msg void OnEnKillfocusEditFanOnName();
    afx_msg void OnEnKillfocusEditFanLowName();
    afx_msg void OnEnKillfocusEditFanMidName();
    afx_msg void OnEnKillfocusEditFanAutoName();
    afx_msg void OnEnKillfocusEditFanHeatName();
    afx_msg void OnEnKillfocusEditFanCoolName();
    BOOL Write_Model_Name(CString Name,int Address);
    CEdit m_edit_auto;
    CEdit m_edit_cool;
    CEdit m_edit_heat;
    CEdit m_edit_off;
    CEdit m_edit_model1;
    CEdit m_edit_model2;
    CEdit m_edit_model3;
	afx_msg void OnBnClickedButtonModel1();
	afx_msg void OnBnClickedButtonModel2();
	afx_msg void OnBnClickedButtonModel3();
	afx_msg void OnBnClickedButtonModel4();
	afx_msg void OnBnClickedButtonModel5();
	void SetModelButton();
	 
	//COXBitmapButton m_model1;
	//COXBitmapButton m_model2;
	//COXBitmapButton m_model3;
	//COXBitmapButton m_model4;
	//COXBitmapButton m_model5;
	CButton m_model1;
	CButton m_model2;
	CButton m_model3;
	CButton m_model4;
	CButton m_model5;
	afx_msg void OnBnClickedButtonModel11();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
