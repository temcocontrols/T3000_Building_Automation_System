#pragma once
#include "afxwin.h"

#include "Dialog_Progess.h"

// CParameterDlg dialog

class CParameterDlg : public CDialog
{
	DECLARE_DYNAMIC(CParameterDlg)

public:
	CParameterDlg(CWnd* pParent = NULL,CString str=_T(""));   // standard constructor
	virtual ~CParameterDlg();

	// Dialog Data
	enum { IDD = IDD_PARAMETERDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	CString strparamode;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRefreshbutton();
//	afx_msg void OnBnClickedEnableidbutton();
	afx_msg void OnCbnSelendcancelBaudratecombo();
	afx_msg void OnCbnSelchangePowermodelcombo();
	afx_msg void OnCbnSelchangeKeypadlockcombo();
	afx_msg void OnCbnSelchangeDisplaycombo();
	afx_msg void OnCbnSelchangeAutoonlycombo();
// 	afx_msg void OnEnKillfocusIdaddressedit();
	afx_msg void OnDestroy();

	afx_msg void OnCbnSelchangekeypadcombo();

	//afx_msg void OnCbnKillfocusKeypadselect();
	afx_msg void OnEnKillfocusValuposedit();
	afx_msg void OnCbnSelchangeSequencecombox();
	afx_msg void OnCbnSelchangeHcchangecombo();
	afx_msg void OnEnKillfocusPowsetpomitedit();
	afx_msg void OnEnKillfocusStincreaedit();
//	afx_msg void OnEnKillfocusValveedit();
	afx_msg void OnEnKillfocusDefSetpointEdt();
	afx_msg void OnEnKillfocusSetpointctledit();
	afx_msg void OnEnKillfocusCycleedit();
	afx_msg void OnCbnSelchangeTimerdelectcombo();
	afx_msg void OnEnKillfocusTimeedit();
	afx_msg void OnEnKillfocusOverridetimeedit();
	afx_msg void OnEnKillfocusInfileredit();
	afx_msg void OnEnKillfocusTimerleft();
	afx_msg void OnCbnSelchangeUnitcombo();
//	afx_msg void OnEnKillfocusTempalarmedit();
	afx_msg void OnEnKillfocusTimeronedit();
	afx_msg void OnEnKillfocusEdit12();
	afx_msg void OnBnClickedFreecoolbgtn();
	afx_msg void OnBnClickedOutdoorresetbtn();
	afx_msg void OnCbnSelchangeInputselect1();
	afx_msg void OnCbnSelchangeInputselect2();
	afx_msg void OnEnKillfocusInputvalue2();
	afx_msg void OnCbnSelchangeEapplication();
	afx_msg void OnEnKillfocusSetvalue2();
	afx_msg void OnEnKillfocusOutput1();
	afx_msg void OnEnKillfocusOutput2();
	afx_msg void OnEnKillfocusEcoolingpterm1();
	afx_msg void OnEnKillfocusEcoolingpterm2();
	afx_msg void OnEnKillfocusEdit26();
	afx_msg void OnEnKillfocusEdit27();
	afx_msg void OnEnKillfocusSpset1();
	afx_msg void OnEnKillfocusSpset2();
	afx_msg void OnEnKillfocusEsetpointhi();
	afx_msg void OnEnKillfocusEsetpointlo();
	afx_msg void OnEnKillfocusEcooldeadband1();
	afx_msg void OnEnKillfocusEcooldeadband2();
	afx_msg void OnEnKillfocusEcoolingiterm1();
	afx_msg void OnEnKillfocusEcoolingiterm2();
	afx_msg void OnBnClickedInputsbutton();
	afx_msg void OnBnClickedOutputsbutton();
	afx_msg void OnBnClickedOutputstablebutton();
	afx_msg void OnEnKillfocusEnigntheating();
	afx_msg void OnEnKillfocusEnigntcooling1();
	afx_msg void OnEnKillfocusEnigntcooling2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnKillfocusSetvalue1();
	afx_msg void OnCbnSelchangeOccupiedmodecombo();
	virtual BOOL OnInitDialog();


	afx_msg LRESULT  ResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  ReadDataCallBack(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT  ReadDataRefreshUI(WPARAM wParam, LPARAM lParam);
	void Refresh();
	void Refresh6();
	void Reflesh_ParameterDlg();
	void Read_SliderData();
	void InitPID2ComboBox();
	void UpdateCoolingandHeatingData();
public:
	//	unsigned short multi_register_value[513];
	HANDLE hFirstThread;
public:
 
	//CEdit m_idAdressEdit;
	CComboBox m_baudRateCombox;
	CComboBox m_keySelectCombox;
	CComboBox m_powerupModelCombox;
	CComboBox m_displayCombox;
	CComboBox m_keyLockCombox;
	CComboBox m_SequenceCombox;
	//CComboBox m_autoOnlyCombox;
	 
	CEdit m_value_percentEdit;
	CComboBox m_hcChangeCombox;
	CEdit m_powerSetPointEdit;
	CEdit m_setpointIncreasement;
	CEdit m_defSetPointEdit;
 
	CEdit m_inputFilterEdit;
	CEdit m_cycledlayEdit;

	CEdit m_timerOnEdit;
	CEdit m_timerOffEdit;
	CComboBox m_uniteCombox;
	CEdit m_timerLeft;
	CComboBox m_timerSelectCombox;
	CComboBox m_InputSelect1;
	CComboBox m_inputSelect2;	
	CEdit m_inputvalue1;
	CEdit m_TimeEdit;
	CEdit m_OverRideEdit;
	CEdit m_inputValue2;
	CComboBox m_application_ctrl;
	CEdit m_pid_outputEdt1;
	CEdit m_pid_outputEdt2;
	CEdit m_nightcooling;
	CEdit m_nightheating;
	CMFCMenuButton m_IOSelectMenuBtn;
	CMenu m_IOmenu;
	CEdit m_hotelCoolEdt;
	CEdit m_pid_setptEdt1;
	CEdit m_pid_setptEdt2;
	CEdit m_pternEdt1;
	CEdit m_ptermEdt2;
	CEdit m_coolingPitemEdt1;
	CEdit m_pidPitemEdt2;
	CEdit m_dayOccEdt1;
	CEdit m_dayOccEdt2;
	CEdit m_setptHiEdit;
	CEdit m_setptLoEdit;
	CEdit m_setptCDDEdt1;
	CEdit m_setptCCDEdt2;
	CEdit m_HeadDEdt1;
	CEdit m_HeadDEdt2;
	CComboBox m_occupiedSetPointModeCmbox;
	BOOL m_disable_hum,m_disable_CO2;
	float m_version;
	afx_msg void OnCbnSelchangeComboLcdscrn1();
	afx_msg void OnCbnSelchangeComboLcdscrn2();
	afx_msg void OnCbnSelchangeComboRounddis();

	CDialog_Progess* pProgess;




	afx_msg void OnCbnKillfocusCombo1();
	afx_msg void OnCbnKillfocusCombo4();
	float m_coolsp;
	float m_cooldb;
	float m_setpoint;
	float m_heatdb;
	float m_heatsp;
	float m_coolspN;
	float m_cooldbN;
	float m_setpointN;
	float m_heatdbN;
	float m_heatspN;
	afx_msg void OnEnKillfocusEditCspd();
	afx_msg void OnEnKillfocusEditCdbdn();
	afx_msg void OnEnKillfocusEdit31();
	afx_msg void OnEnKillfocusEdit34();
	afx_msg void OnEnKillfocusEdit37();
	afx_msg void OnEnKillfocusEditCspnn();
	afx_msg void OnEnKillfocusEditCdbnn();
	afx_msg void OnEnKillfocusEdit32();
	afx_msg void OnEnKillfocusEdit35();
	afx_msg void OnEnKillfocusEdit38();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnKillfocusEditValuetraveltime();
	afx_msg void OnEnKillfocusEditPid2offsetpoint();
	//	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    
public:
	CBrush m_brush;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void Fresh_Single_UI();
	//	afx_msg void OnEnChangeEdit34();
	afx_msg void OnCbnSelchangeStaticunint2();
	CComboBox m_gUnit;
	afx_msg void OnBnClickedCs1();
	afx_msg void OnBnClickedCs2();

	CString GetInputValue(int InputNo);
	BOOL Get_Data_Bit(UINT Data,int n,int N);
	void ShowPID3();
	afx_msg void OnBnClickedAirflowSetting();
	afx_msg void OnCbnSelchangeInputselect3Pid3();
	CComboBox m_inputSelect3;
	CEdit m_inputValue3;
	afx_msg void OnEnKillfocusSetvalue3Pid3();
	afx_msg void OnEnKillfocusOutput3Pid3();
	afx_msg void OnEnKillfocusEcoolingpterm3Pid3();
	afx_msg void OnEnKillfocusEdit52Pid3();
	afx_msg void OnEnKillfocusSpset4Pid3();
	afx_msg void OnEnKillfocusEsetpointhi2Pid3();
	afx_msg void OnEnKillfocusEsetpointlo2Pid3();
	afx_msg void OnEnKillfocusEcooldeadband3Pid3();
	afx_msg void OnEnKillfocusEcoolingiterm3Pid3();
	CEdit m_edit_backlighttime;
	afx_msg void OnEnKillfocusEditBacklightTime();
	afx_msg void OnBnClickedCheck2();
	CButton m_check_occupiedenable;
	CEdit m_occupied_timer;
	afx_msg void OnEnKillfocusEditOccupiedTimer();
	afx_msg void OnEnKillfocusEditDeadMaster();
	CComboBox m_combox_pir_endisable;
	CEdit m_sensitivity_editor;
	afx_msg void OnCbnSelchangeComboEnableDis();
	afx_msg void OnEnKillfocusEditSensitivty();
	//CComboBox m_combox_setpointdisplay;
//	afx_msg void OnCbnSelchangeComboSetpointdisplay();
// 	CComboBox m_combox_zigbee;
// 	CComboBox m_combox_channel;
	//afx_msg void OnCbnSelchangeComboZigbeetype();
/*	afx_msg void OnCbnSelchangeComboChannel();*/
//	afx_msg void OnBnClickedButtonZigbeeReboot();
//	afx_msg void OnBnClickedBtnTopological();
	afx_msg void OnEnKillfocusEditPid2offsetpoint6();
//	afx_msg void OnBnClickedUnlock();
//	afx_msg void OnBnClickedLock();
	afx_msg void OnBnClickedTrendlogview();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo4();
	CComboBox m_day_combox;
	CComboBox m_night_combox;
	afx_msg void OnEnKillfocusEditTranducerMax();
	afx_msg void OnEnKillfocusEditTranducerMin();
    afx_msg void OnEnKillfocusEditChangeOverTime();
    afx_msg void OnEnKillfocusEditTstatName();
	afx_msg void OnCbnSelchangeComboMrd();
	CComboBox m_combox_mrd;
	afx_msg void OnBnClickedButtonApplyAll();
	afx_msg void OnBnClickedButtonZigbee();
	afx_msg void OnEnKillfocusEditShowid();
	afx_msg void OnBnClickedButtonShowModbusId();

	int m_time_left  ;
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    CEdit m_edit_dterm;
    CEdit m_edit_sampling_interval;
    afx_msg void OnEnKillfocusEditDterm();
    afx_msg void OnEnKillfocusEditSamplingInterval();
    afx_msg void OnBnClickedButtonParameterExt();
    afx_msg void OnBnClickedButtonIconSetting();
    afx_msg void OnCbnSelchangeDeltaSelect1();
    afx_msg void OnCbnSelchangeDeltaSelect2();
    BOOL OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
};
