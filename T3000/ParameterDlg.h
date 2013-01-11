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
	afx_msg void OnBnClickedEnableidbutton();
	afx_msg void OnCbnSelendcancelBraudratecombo();
	afx_msg void OnCbnSelchangePowermodelcombo();
	afx_msg void OnCbnSelchangeKeypadlockcombo();
	afx_msg void OnCbnSelchangeDisplaycombo();
	afx_msg void OnCbnSelchangeAutoonlycombo();
	afx_msg void OnEnKillfocusIdaddressedit();
	afx_msg void OnDestroy();
	afx_msg void OnCbnKillfocusKeypadselect();
	afx_msg void OnEnKillfocusValuposedit();
	afx_msg void OnCbnSelchangeSequencecombox();
	afx_msg void OnCbnSelchangeHcchangecombo();
	afx_msg void OnEnKillfocusPowsetpomitedit();
	afx_msg void OnEnKillfocusStincreaedit();
	afx_msg void OnEnKillfocusValveedit();
	afx_msg void OnEnKillfocusDefSetpointEdt();
	afx_msg void OnEnKillfocusSetpointctledit();
	afx_msg void OnEnKillfocusCycleedit();
	afx_msg void OnCbnSelchangeTimerdelectcombo();
	afx_msg void OnEnKillfocusTimeedit();
	afx_msg void OnEnKillfocusOverridetimeedit();
	afx_msg void OnEnKillfocusInfileredit();
	afx_msg void OnEnKillfocusTimerleft();
	afx_msg void OnCbnSelchangeUnitcombo();
	afx_msg void OnEnKillfocusTempalarmedit();
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

	void Refresh();
	 void Refresh6();
	void InitPID2ComboBox();
	void  UpdateCoolingandHeatingData();
public:
//	unsigned short multi_register_value[513];
	CWinThread* pParamBackFresh;
public:
	CEdit m_idAdressEdit;
	CComboBox m_braudRateCombox;
	CComboBox m_keySelectCombox;
	CComboBox m_powerupModelCombox;
	CComboBox m_displayCombox;
	CComboBox m_keyLockCombox;
	CComboBox m_SequenceCombox;
	CComboBox m_autoOnlyCombox;
	CEdit m_valveEdit;
	CEdit m_value_percentEdit;
	CComboBox m_hcChangeCombox;
	CEdit m_powerSetPointEdit;
	CEdit m_setpointIncreasement;
	CEdit m_defSetPointEdit;
	CEdit m_setPointCtrlEdit;
	CEdit m_inputFilterEdit;
	CEdit m_cycledlayEdit;
	CEdit m_tempAlarmEdit;
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

	float m_version;
	afx_msg void OnCbnSelchangeComboLcdscrn1();
	afx_msg void OnCbnSelchangeComboLcdscrn2();
	afx_msg void OnCbnSelchangeComboRounddis();

	CDialog_Progess* pProgess;




	afx_msg void OnCbnKillfocusCombo1();
	afx_msg void OnCbnKillfocusCombo4();
	int m_coolsp;
	int m_cooldb;
	int m_setpoint;
	int m_heatdb;
	int m_heatsp;
	int m_coolspN;
	int m_cooldbN;
	int m_setpointN;
	int m_heatdbN;
	int m_heatspN;
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
	afx_msg void OnEnChangeEdit26();
};
