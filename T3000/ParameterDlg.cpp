// ParameterDlg.cpp : implementation file
//
// ParameterDlg.cpp add support tstat 6 7 function by Fance Du.

#include "stdafx.h"
#include "T3000.h"
#include "ParameterDlg.h"
#include "global_function.h"
#include "FreeCoolDlg.h"
#include "OutsidDailog.h"
#include "ParameterExtDlg.h"
#include "InputSetDlg.h"
#include "OutputSetDlg.h"
#include "AfxMessageDialog.h"
#include "OutPutDlg.h"

#include "MainFrm.h"

#include "Dialog_Progess.h"//tstat6
#include "TstatZigbeeLogic.h"

#include "T3000RegAddress.h"
#include "CustomSTable.h"
#include "AirflowSettingDlg.h"
#include "../SQLiteDriver/CppSQLite3.h"
extern int Mdb_Adress_Map;
// CParameterDlg dialog
//#define PID_INTERNAL_TEMPERATURE									   2
//#define PID_HUM_TEMPERATURE										   4
//#define PID_AI1                                                      5
//#define PID_AI2                                                      6
//#define PID_AI3                                                      7
//#define PID_AI4                                                      8
//#define PID_AI5                                                      9
//#define PID_AI6                                                      10
//#define PID_AI7                                                      11
//#define PID_AI8                                                      12
//#define PID_CO2													   13
//#define PID_HUM													   14
//#define PID_AIRFLOW_SENSOR										   15

static DWORD WINAPI hReadKillFocsData(LPVOID pParam);
#define NUM_INPUT_SETTING 17
CString INPUT_SETTING[NUM_INPUT_SETTING]=
{
    _T("Internal Sensor"),
    _T("Avg Temperature"),
    _T("Internal Hum"),
    _T("AI1"),_T("AI2"),_T("AI3"),_T("AI4"),_T("AI5"),_T("AI6"),_T("AI7"),_T("AI8"),
    _T("HUM Sensor"),_T("CO2 Sensor"),_T("Airflow Sensor"),
    _T("Avg AI1ToAI2"),_T("Avg AI1ToAI3"),_T("Avg AI1ToAI4")
};
#define NUM_INPUT_SETTING_T7 13
CString INPUT_SETTING_T7[NUM_INPUT_SETTING_T7]=
{
    _T("Internal Sensor"),
    _T("Avg Temperature"),
    _T("Internal Hum") ,
    _T("AI1"),_T("AI2"),_T("AI3"),_T("AI4"),_T("AI5"),_T("AI6"),_T("AI7"),_T("AI8"),
    _T("CO2 Sensor"),_T("HUM Sensor")
};
CString INPUT_SETTING_LCD[6]= {
                               
                               _T("Internal Sensor"),
                               //_T(""),
                               _T("AI1"),
							   _T("Avg Temperature")
                               /*_T("AI2"),_T("AI3"),_T("AI4"),_T("AI5"),_T("AI6"),_T("AI7"),_T("AI8"),
                               _T("CO2 Sensor"),_T("HUM Sensor"),_T("Airflow Sensor"),*/
                               _T("Avg AI1ToAI2"),_T("Avg AI1ToAI3"),_T("Avg AI1ToAI4")
                              };

typedef enum
{
    PID_INTERNAL_TEMPERATURE=2,
    PID_AVG_TEMP=3,
    PID_HUM_TEMPERATURE=4,
    PID_AI1,
    PID_AI2,
    PID_AI3,
    PID_AI4,
    PID_AI5,
    PID_AI6,
    PID_AI7,
    PID_AI8,
    PID_CO2,
    PID_HUM,
    PID_AIRFLOW_SENSOR,
    PID_AVG_A12,
    PID_AVG_A13,
    PID_AVG_A14
} INPUT_SETTING_VALUE;

IMPLEMENT_DYNAMIC(CParameterDlg, CDialog)

CParameterDlg::CParameterDlg(CWnd* pParent /*=NULL*/,CString str)
    : CDialog(CParameterDlg::IDD, pParent)
    , m_coolsp(0)
    , m_cooldb(0)
    , m_setpoint(0)
    , m_heatdb(0)
    , m_heatsp(0)
    , m_coolspN(0)
    , m_cooldbN(0)
    , m_setpointN(0)
    , m_heatdbN(0)
    , m_heatspN(0)
{
    //memset(&product_register_value[0],0,sizeof(product_register_value));
    m_version=1.0;

    strparamode = str;
	m_time_left = 0;
}

CParameterDlg::~CParameterDlg()
{
}

void CParameterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //DDX_Control(pDX, IDC_IDADDRESSEDIT, m_idAdressEdit);
    DDX_Control(pDX, IDC_BAUDRATECOMBO, m_baudRateCombox);
    DDX_Control(pDX, IDC_COMBO2, m_keySelectCombox);
    DDX_Control(pDX, IDC_POWERMODELCOMBO, m_powerupModelCombox);
    DDX_Control(pDX, IDC_DISPLAYCOMBO, m_displayCombox);
    DDX_Control(pDX, IDC_KEYPADLOCKCOMBO, m_keyLockCombox);
    DDX_Control(pDX, IDC_SEQUENCECOMBOX, m_SequenceCombox);
    //DDX_Control(pDX, IDC_AUTOONLYCOMBO, m_autoOnlyCombox);
    DDX_Control(pDX, IDC_VALUPOSEDIT, m_value_percentEdit);

    DDX_Control(pDX, IDC_HCCHANGECOMBO, m_hcChangeCombox);
    DDX_Control(pDX, IDC_POWSETPOMITEDIT, m_powerSetPointEdit);
    DDX_Control(pDX, IDC_STINCREAEDIT, m_setpointIncreasement);
    DDX_Control(pDX, IDC_DEF_SETPOINT_EDT, m_defSetPointEdit);

    DDX_Control(pDX, IDC_INFILEREDIT, m_inputFilterEdit);
    DDX_Control(pDX, IDC_CYCLEEDIT, m_cycledlayEdit);

    DDX_Control(pDX, IDC_TIMERONEDIT, m_timerOnEdit);
    DDX_Control(pDX, IDC_EDIT12, m_timerOffEdit);
    DDX_Control(pDX, IDC_UNITCOMBO, m_uniteCombox);
    DDX_Control(pDX, IDC_TIMERLEFT, m_timerLeft);
    DDX_Control(pDX, IDC_TIMERDELECTCOMBO, m_timerSelectCombox);
    DDX_Control(pDX, IDC_TIMEEDIT, m_TimeEdit);
    DDX_Control(pDX, IDC_OVERRIDETIMEEDIT, m_OverRideEdit);
    DDX_Control(pDX, IDC_INPUTSELECT1, m_InputSelect1);
    DDX_Control(pDX, IDC_INPUTSELECT2, m_inputSelect2);
    DDX_Control(pDX, IDC_INPUTVALUE1, m_inputvalue1);
    DDX_Control(pDX, IDC_INPUTVALUE2, m_inputValue2);
    DDX_Control(pDX, IDC_EAPPLICATION, m_application_ctrl);
    DDX_Control(pDX, IDC_ENIGNTCOOLING1, m_nightcooling);
    DDX_Control(pDX, IDC_ENIGNTHEATING, m_nightheating);
    //	DDX_Control(pDX, IDC_ENIGNTCOOLING2, m_hotelCoolEdt);
    DDX_Control(pDX, IDC_SETVALUE1, m_pid_setptEdt1);
    DDX_Control(pDX, IDC_SETVALUE2, m_pid_setptEdt2);
    DDX_Control(pDX, IDC_OUTPUT1, m_pid_outputEdt1);
    DDX_Control(pDX, IDC_OUTPUT2, m_pid_outputEdt2);
    DDX_Control(pDX, IDC_ECOOLINGPTERM1, m_pternEdt1);
    DDX_Control(pDX, IDC_ECOOLINGPTERM2, m_ptermEdt2);
    DDX_Control(pDX, IDC_EDIT26, m_coolingPitemEdt1);
    DDX_Control(pDX, IDC_EDIT27, m_pidPitemEdt2);
    DDX_Control(pDX, IDC_SPSET1, m_dayOccEdt1);
    DDX_Control(pDX, IDC_SPSET2, m_dayOccEdt2);
    DDX_Control(pDX, IDC_ESETPOINTHI, m_setptHiEdit);
    DDX_Control(pDX, IDC_ESETPOINTLO, m_setptLoEdit);
    DDX_Control(pDX, IDC_ECOOLDEADBAND1, m_setptCDDEdt1);
    DDX_Control(pDX, IDC_ECOOLDEADBAND2, m_setptCCDEdt2);
    DDX_Control(pDX, IDC_ECOOLINGITERM1, m_HeadDEdt1);
    DDX_Control(pDX, IDC_ECOOLINGITERM2, m_HeadDEdt2);
    DDX_Control(pDX, IDC_OCCUPIEDMODECOMBO, m_occupiedSetPointModeCmbox);

    DDX_Text(pDX, IDC_EDIT_CSPD, m_coolsp);
    DDX_Text(pDX, IDC_EDIT_CDBDN, m_cooldb);
    DDX_Text(pDX, IDC_EDIT31, m_setpoint);
    DDX_Text(pDX, IDC_EDIT34, m_heatdb);
    DDX_Text(pDX, IDC_EDIT37, m_heatsp);
    DDX_Text(pDX, IDC_EDIT_CSPNN, m_coolspN);
    DDX_Text(pDX, IDC_EDIT_CDBNN, m_cooldbN);
    DDX_Text(pDX, IDC_EDIT32, m_setpointN);
    DDX_Text(pDX, IDC_EDIT35, m_heatdbN);
    DDX_Text(pDX, IDC_EDIT38, m_heatspN);
    DDX_Control(pDX, IDC_STATICUNINT2, m_gUnit);
    DDX_Control(pDX, IDC_INPUTSELECT3_PID3, m_inputSelect3);
    DDX_Control(pDX, IDC_INPUTVALUE3_PID3, m_inputValue3);
    DDX_Control(pDX, IDC_EDIT_BACKLIGHT_TIME, m_edit_backlighttime);
    DDX_Control(pDX, IDC_CHECK2, m_check_occupiedenable);
    DDX_Control(pDX, IDC_EDIT_OCCUPIED_TIMER, m_occupied_timer);
    DDX_Control(pDX, IDC_COMBO_ENABLE_DIS, m_combox_pir_endisable);
    DDX_Control(pDX, IDC_EDIT_SENSITIVTY, m_sensitivity_editor);

    DDX_Control(pDX, IDC_COMBO1, m_day_combox);
    DDX_Control(pDX, IDC_COMBO4, m_night_combox);
    DDX_Control(pDX, IDC_COMBO_MRD, m_combox_mrd);
    DDX_Control(pDX, IDC_EDIT_DTERM, m_edit_dterm);
    DDX_Control(pDX, IDC_EDIT_SAMPLING_INTERVAL, m_edit_sampling_interval);
}

BEGIN_MESSAGE_MAP(CParameterDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CParameterDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_REFRESHBUTTON, &CParameterDlg::OnBnClickedRefreshbutton)
//    ON_BN_CLICKED(IDC_ENABLEIDBUTTON, &CParameterDlg::OnBnClickedEnableidbutton)
    ON_CBN_SELENDCANCEL(IDC_BAUDRATECOMBO, &CParameterDlg::OnCbnSelendcancelBaudratecombo)
    ON_CBN_SELCHANGE(IDC_POWERMODELCOMBO, &CParameterDlg::OnCbnSelchangePowermodelcombo)
    ON_CBN_SELCHANGE(IDC_KEYPADLOCKCOMBO, &CParameterDlg::OnCbnSelchangeKeypadlockcombo)
    ON_CBN_SELCHANGE(IDC_DISPLAYCOMBO, &CParameterDlg::OnCbnSelchangeDisplaycombo)
//	ON_CBN_SELCHANGE(IDC_AUTOONLYCOMBO, &CParameterDlg::OnCbnSelchangeAutoonlycombo)

    ON_CBN_SELCHANGE(IDC_KEYPADSELECT, &CParameterDlg::OnCbnSelchangekeypadcombo)

//    ON_EN_KILLFOCUS(IDC_IDADDRESSEDIT, &CParameterDlg::OnEnKillfocusIdaddressedit)
    ON_WM_DESTROY()
    //ON_CBN_KILLFOCUS(IDC_KEYPADSELECT, &CParameterDlg::OnCbnKillfocusKeypadselect)
    ON_EN_KILLFOCUS(IDC_VALUPOSEDIT, &CParameterDlg::OnEnKillfocusValuposedit)
    ON_CBN_SELCHANGE(IDC_SEQUENCECOMBOX, &CParameterDlg::OnCbnSelchangeSequencecombox)
//	ON_EN_KILLFOCUS(IDC_VALVEEDIT, &CParameterDlg::OnEnKillfocusValveedit)
    ON_CBN_SELCHANGE(IDC_HCCHANGECOMBO, &CParameterDlg::OnCbnSelchangeHcchangecombo)
    ON_EN_KILLFOCUS(IDC_POWSETPOMITEDIT, &CParameterDlg::OnEnKillfocusPowsetpomitedit)
    ON_EN_KILLFOCUS(IDC_STINCREAEDIT, &CParameterDlg::OnEnKillfocusStincreaedit)
    ON_EN_KILLFOCUS(IDC_DEF_SETPOINT_EDT, &CParameterDlg::OnEnKillfocusDefSetpointEdt)
    ON_EN_KILLFOCUS(IDC_SETPOINTCTLEDIT, &CParameterDlg::OnEnKillfocusSetpointctledit)
    ON_EN_KILLFOCUS(IDC_INFILEREDIT, &CParameterDlg::OnEnKillfocusInfileredit)
    ON_EN_KILLFOCUS(IDC_CYCLEEDIT, &CParameterDlg::OnEnKillfocusCycleedit)
 
    ON_EN_KILLFOCUS(IDC_TIMERONEDIT, &CParameterDlg::OnEnKillfocusTimeronedit)
    ON_EN_KILLFOCUS(IDC_EDIT12, &CParameterDlg::OnEnKillfocusEdit12)
    ON_CBN_SELCHANGE(IDC_UNITCOMBO, &CParameterDlg::OnCbnSelchangeUnitcombo)
    ON_EN_KILLFOCUS(IDC_TIMERLEFT, &CParameterDlg::OnEnKillfocusTimerleft)
    ON_CBN_SELCHANGE(IDC_TIMERDELECTCOMBO, &CParameterDlg::OnCbnSelchangeTimerdelectcombo)
    ON_EN_KILLFOCUS(IDC_TIMEEDIT, &CParameterDlg::OnEnKillfocusTimeedit)
    ON_EN_KILLFOCUS(IDC_OVERRIDETIMEEDIT, &CParameterDlg::OnEnKillfocusOverridetimeedit)
    ON_BN_CLICKED(IDC_FREECOOLBGTN, &CParameterDlg::OnBnClickedFreecoolbgtn)
    ON_BN_CLICKED(IDC_OUTDOORRESETBTN, &CParameterDlg::OnBnClickedOutdoorresetbtn)
    ON_CBN_SELCHANGE(IDC_INPUTSELECT1, &CParameterDlg::OnCbnSelchangeInputselect1)
    ON_CBN_SELCHANGE(IDC_INPUTSELECT2, &CParameterDlg::OnCbnSelchangeInputselect2)
    ON_EN_KILLFOCUS(IDC_INPUTVALUE2, &CParameterDlg::OnEnKillfocusInputvalue2)
    ON_CBN_SELCHANGE(IDC_EAPPLICATION, &CParameterDlg::OnCbnSelchangeEapplication)
    ON_EN_KILLFOCUS(IDC_SETVALUE2, &CParameterDlg::OnEnKillfocusSetvalue2)
    ON_EN_KILLFOCUS(IDC_OUTPUT1, &CParameterDlg::OnEnKillfocusOutput1)
    ON_EN_KILLFOCUS(IDC_OUTPUT2, &CParameterDlg::OnEnKillfocusOutput2)
    ON_EN_KILLFOCUS(IDC_ECOOLINGPTERM1, &CParameterDlg::OnEnKillfocusEcoolingpterm1)
    ON_EN_KILLFOCUS(IDC_ECOOLINGPTERM2, &CParameterDlg::OnEnKillfocusEcoolingpterm2)
    ON_EN_KILLFOCUS(IDC_EDIT26, &CParameterDlg::OnEnKillfocusEdit26)
    ON_EN_KILLFOCUS(IDC_EDIT27, &CParameterDlg::OnEnKillfocusEdit27)
    ON_EN_KILLFOCUS(IDC_SPSET1, &CParameterDlg::OnEnKillfocusSpset1)
    ON_EN_KILLFOCUS(IDC_SPSET2, &CParameterDlg::OnEnKillfocusSpset2)
    ON_EN_KILLFOCUS(IDC_ESETPOINTHI, &CParameterDlg::OnEnKillfocusEsetpointhi)
    ON_EN_KILLFOCUS(IDC_ESETPOINTLO, &CParameterDlg::OnEnKillfocusEsetpointlo)
    ON_EN_KILLFOCUS(IDC_ECOOLDEADBAND1, &CParameterDlg::OnEnKillfocusEcooldeadband1)
    ON_EN_KILLFOCUS(IDC_ECOOLDEADBAND2, &CParameterDlg::OnEnKillfocusEcooldeadband2)
    ON_EN_KILLFOCUS(IDC_ECOOLINGITERM1, &CParameterDlg::OnEnKillfocusEcoolingiterm1)
    ON_EN_KILLFOCUS(IDC_ECOOLINGITERM2, &CParameterDlg::OnEnKillfocusEcoolingiterm2)
    ON_BN_CLICKED(IDC_INPUTSBUTTON, &CParameterDlg::OnBnClickedInputsbutton)
    ON_BN_CLICKED(IDC_OutPutsBUTTON, &CParameterDlg::OnBnClickedOutputsbutton)
    ON_BN_CLICKED(IDC_OUTPUTSTABLEBUTTON, &CParameterDlg::OnBnClickedOutputstablebutton)
    ON_EN_KILLFOCUS(IDC_ENIGNTHEATING, &CParameterDlg::OnEnKillfocusEnigntheating)
    ON_EN_KILLFOCUS(IDC_ENIGNTCOOLING1, &CParameterDlg::OnEnKillfocusEnigntcooling1)
    //ON_EN_KILLFOCUS(IDC_ENIGNTCOOLING2, &CParameterDlg::OnEnKillfocusEnigntcooling2)
    ON_EN_KILLFOCUS(IDC_SETVALUE1, &CParameterDlg::OnEnKillfocusSetvalue1)
    ON_CBN_SELCHANGE(IDC_OCCUPIEDMODECOMBO, &CParameterDlg::OnCbnSelchangeOccupiedmodecombo)
    ON_CBN_SELCHANGE(IDC_COMBO_LCDSCRN1, &CParameterDlg::OnCbnSelchangeComboLcdscrn1)
    ON_CBN_SELCHANGE(IDC_COMBO_LCDSCRN2, &CParameterDlg::OnCbnSelchangeComboLcdscrn2)
    ON_CBN_SELCHANGE(IDC_COMBO_ROUNDDIS, &CParameterDlg::OnCbnSelchangeComboRounddis)

    ON_CBN_KILLFOCUS(IDC_COMBO1, &CParameterDlg::OnCbnKillfocusCombo1)
    ON_CBN_KILLFOCUS(IDC_COMBO4, &CParameterDlg::OnCbnKillfocusCombo4)
    ON_EN_KILLFOCUS(IDC_EDIT_CSPD, &CParameterDlg::OnEnKillfocusEditCspd)
    ON_EN_KILLFOCUS(IDC_EDIT_CDBDN, &CParameterDlg::OnEnKillfocusEditCdbdn)
    ON_EN_KILLFOCUS(IDC_EDIT31, &CParameterDlg::OnEnKillfocusEdit31)
    ON_EN_KILLFOCUS(IDC_EDIT34, &CParameterDlg::OnEnKillfocusEdit34)
    ON_EN_KILLFOCUS(IDC_EDIT37, &CParameterDlg::OnEnKillfocusEdit37)
    ON_EN_KILLFOCUS(IDC_EDIT_CSPNN, &CParameterDlg::OnEnKillfocusEditCspnn)
    ON_EN_KILLFOCUS(IDC_EDIT_CDBNN, &CParameterDlg::OnEnKillfocusEditCdbnn)
    ON_EN_KILLFOCUS(IDC_EDIT32, &CParameterDlg::OnEnKillfocusEdit32)
    ON_EN_KILLFOCUS(IDC_EDIT35, &CParameterDlg::OnEnKillfocusEdit35)
    ON_EN_KILLFOCUS(IDC_EDIT38, &CParameterDlg::OnEnKillfocusEdit38)
    ON_BN_CLICKED(IDCANCEL, &CParameterDlg::OnBnClickedCancel)
    ON_EN_KILLFOCUS(IDC_EDIT_ValueTravelTime, &CParameterDlg::OnEnKillfocusEditValuetraveltime)
    ON_EN_KILLFOCUS(IDC_EDIT_PID2OFFSETPOINT, &CParameterDlg::OnEnKillfocusEditPid2offsetpoint)

    ON_MESSAGE(MY_RESUME_DATA, ResumeMessageCallBack)
    ON_MESSAGE(MY_READ_DATA_CALLBACK, ReadDataCallBack)
    ON_MESSAGE(WM_KILLFOCS_READ_DATA, ReadDataRefreshUI)
    

    //ON_WM_KILLFOCUS()
    ON_WM_CTLCOLOR()
    ON_WM_TIMER()
    //ON_EN_CHANGE(IDC_EDIT34, &CParameterDlg::OnEnChangeEdit34)
    ON_CBN_SELCHANGE(IDC_STATICUNINT2, &CParameterDlg::OnCbnSelchangeStaticunint2)
    ON_BN_CLICKED(IDC_CS_1, &CParameterDlg::OnBnClickedCs1)
    ON_BN_CLICKED(IDC_CS_2, &CParameterDlg::OnBnClickedCs2)
    ON_BN_CLICKED(IDC_AIRFLOW_SETTING, &CParameterDlg::OnBnClickedAirflowSetting)
    ON_CBN_SELCHANGE(IDC_INPUTSELECT3_PID3, &CParameterDlg::OnCbnSelchangeInputselect3Pid3)
    ON_EN_KILLFOCUS(IDC_SETVALUE3_PID3, &CParameterDlg::OnEnKillfocusSetvalue3Pid3)
    ON_EN_KILLFOCUS(IDC_OUTPUT3_PID3, &CParameterDlg::OnEnKillfocusOutput3Pid3)
    ON_EN_KILLFOCUS(IDC_ECOOLINGPTERM3_PID3, &CParameterDlg::OnEnKillfocusEcoolingpterm3Pid3)
    ON_EN_KILLFOCUS(IDC_EDIT52_PID3, &CParameterDlg::OnEnKillfocusEdit52Pid3)
    ON_EN_KILLFOCUS(IDC_SPSET4_PID3, &CParameterDlg::OnEnKillfocusSpset4Pid3)
    ON_EN_KILLFOCUS(IDC_ESETPOINTHI2_PID3, &CParameterDlg::OnEnKillfocusEsetpointhi2Pid3)
    ON_EN_KILLFOCUS(IDC_ESETPOINTLO2_PID3, &CParameterDlg::OnEnKillfocusEsetpointlo2Pid3)
    ON_EN_KILLFOCUS(IDC_ECOOLDEADBAND3_PID3, &CParameterDlg::OnEnKillfocusEcooldeadband3Pid3)
    ON_EN_KILLFOCUS(IDC_ECOOLINGITERM3_PID3, &CParameterDlg::OnEnKillfocusEcoolingiterm3Pid3)
    ON_EN_KILLFOCUS(IDC_EDIT_BACKLIGHT_TIME, &CParameterDlg::OnEnKillfocusEditBacklightTime)
    ON_BN_CLICKED(IDC_CHECK2, &CParameterDlg::OnBnClickedCheck2)
    ON_EN_KILLFOCUS(IDC_EDIT_OCCUPIED_TIMER, &CParameterDlg::OnEnKillfocusEditOccupiedTimer)
    ON_EN_KILLFOCUS(IDC_EDIT_DEAD_MASTER, &CParameterDlg::OnEnKillfocusEditDeadMaster)
    ON_CBN_SELCHANGE(IDC_COMBO_ENABLE_DIS, &CParameterDlg::OnCbnSelchangeComboEnableDis)
    ON_EN_KILLFOCUS(IDC_EDIT_SENSITIVTY, &CParameterDlg::OnEnKillfocusEditSensitivty)
   // ON_CBN_SELCHANGE(IDC_COMBO_ZIGBEETYPE, &CParameterDlg::OnCbnSelchangeComboZigbeetype)
 /*   ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, &CParameterDlg::OnCbnSelchangeComboChannel)*/
//    ON_BN_CLICKED(IDC_BUTTON_ZIGBEE_REBOOT, &CParameterDlg::OnBnClickedButtonZigbeeReboot)
    ON_EN_KILLFOCUS(IDC_EDIT_PID2OFFSETPOINT6, &CParameterDlg::OnEnKillfocusEditPid2offsetpoint6)
//    ON_BN_CLICKED(IDC_UNLOCK, &CParameterDlg::OnBnClickedUnlock)
//    ON_BN_CLICKED(IDC_LOCK, &CParameterDlg::OnBnClickedLock)
    ON_BN_CLICKED(IDC_TRENDLOGVIEW, &CParameterDlg::OnBnClickedTrendlogview)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CParameterDlg::OnCbnSelchangeCombo1)
    ON_CBN_SELCHANGE(IDC_COMBO4, &CParameterDlg::OnCbnSelchangeCombo4)
    ON_EN_KILLFOCUS(IDC_EDIT_TRANDUCER_MAX, &CParameterDlg::OnEnKillfocusEditTranducerMax)
    ON_EN_KILLFOCUS(IDC_EDIT_TRANDUCER_MIN, &CParameterDlg::OnEnKillfocusEditTranducerMin)
    ON_EN_KILLFOCUS(IDC_EDIT_CHANGE_OVER_TIME, &CParameterDlg::OnEnKillfocusEditChangeOverTime)
    ON_EN_KILLFOCUS(IDC_EDIT_TSTAT_NAME, &CParameterDlg::OnEnKillfocusEditTstatName)
	ON_CBN_SELCHANGE(IDC_COMBO_MRD, &CParameterDlg::OnCbnSelchangeComboMrd)
	ON_BN_CLICKED(IDC_BUTTON_APPLY_ALL, &CParameterDlg::OnBnClickedButtonApplyAll)
	ON_BN_CLICKED(IDC_BUTTON_ZIGBEE, &CParameterDlg::OnBnClickedButtonZigbee)
	ON_EN_KILLFOCUS(IDC_EDIT_SHOWID, &CParameterDlg::OnEnKillfocusEditShowid)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_MODBUS_ID, &CParameterDlg::OnBnClickedButtonShowModbusId)
		ON_WM_HELPINFO()
        ON_EN_KILLFOCUS(IDC_EDIT_DTERM, &CParameterDlg::OnEnKillfocusEditDterm)
        ON_EN_KILLFOCUS(IDC_EDIT_SAMPLING_INTERVAL, &CParameterDlg::OnEnKillfocusEditSamplingInterval)
        ON_BN_CLICKED(IDC_BUTTON_PARAMETER_EXT, &CParameterDlg::OnBnClickedButtonParameterExt)
        ON_BN_CLICKED(IDC_BUTTON_ICON_SETTING, &CParameterDlg::OnBnClickedButtonIconSetting)
        END_MESSAGE_MAP()

	/// <summary>
	/// back fresh
	/// </summary>
	/// <param name="pParam"></param>
	/// <returns></returns>
DWORD WINAPI BackparaFreshProc(LPVOID pParam)
{
    CParameterDlg* pdlg = (CParameterDlg*)pParam;

    //if ((pdlg->strparamode.CompareNoCase(_T("Tstat6")) == 0)||(pdlg->strparamode.CompareNoCase(_T("Tstat7")) == 0))
    //	pdlg->Refresh6();
    //else
    //	pdlg->Refresh();

    pdlg->Reflesh_ParameterDlg();
    while(1)
    {
        Sleep(15*1000);

        pdlg->Reflesh_ParameterDlg();
    }

    return 0;
}

BOOL CParameterDlg::OnInitDialog()
{
	//20120420
	//pProgess = new CDialog_Progess(this,1,10);
	//创建对话框窗口


	//居中显示
	//	pProgess->CenterWindow();//2.5.0.98

	//显示对话框窗口

	//20120420

	//2.5.0.98
//	gGetDialogStrings(this->m_hWnd,IDD_PARAMETERDIALOG);

	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	RECT RECT_SET1;
	pMain->GetClientRect(&RECT_SET1);

	CDialog::OnInitDialog();

    if (product_register_value[7] == PM_TSTAT7 ||
        product_register_value[7] == PM_TSTAT8)
    {
        GetDlgItem(IDC_BUTTON_ICON_SETTING)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_BUTTON_ICON_SETTING)->EnableWindow(FALSE);
    }

	if (product_register_value[7] == PM_TSTAT8
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
	{
		m_baudRateCombox.ResetContent();
		m_baudRateCombox.AddString(L"9600");
		m_baudRateCombox.AddString(L"19200");
		m_baudRateCombox.AddString(L"38400");
		m_baudRateCombox.AddString(L"57600");
		m_baudRateCombox.AddString(L"115200");

	}
	else
	{
		m_baudRateCombox.ResetContent();
		m_baudRateCombox.AddString(L"9600");
		m_baudRateCombox.AddString(L"19200");
	}

	m_gUnit.AddString(_T("°C"));
	m_gUnit.AddString(_T("°F"));


    //Fandu 20180127  TSTAT8 才显示 额外新增的 TSTAT 参数对话框的 按钮;
    if (product_register_value[7] == PM_TSTAT8)
    {
        GetDlgItem(IDC_BUTTON_PARAMETER_EXT)->ShowWindow(SW_SHOW);
    }
    else
    {
        GetDlgItem(IDC_BUTTON_PARAMETER_EXT)->ShowWindow(SW_HIDE);
    }

	if (g_ParamLevel == 1)
	{
		CAfxMessageDialog dlg;
		CString strPromption;
		strPromption.LoadString(IDS_STRNOPRIVILEGE);
		dlg.SetPromtionTxt(strPromption);
		dlg.DoModal();
	}
	// m_idAdressEdit.EnableWindow(FALSE);


	if (product_register_value[MODBUS_MODE_OUTPUT4] == 2)//283
	{
		write_one(g_tstat_id, MODBUS_TIMER_SELECT, 3);//327
		CStatic* ptimerout = (CStatic*)GetDlgItem(IDC_TIMERON_STATIC);
		ptimerout->SetWindowText(_T("Out4"));
	}
	else
	{
		CStatic* ptimerout = (CStatic*)GetDlgItem(IDC_TIMERON_STATIC);
		ptimerout->SetWindowText(_T("Timer On"));
	}
	//     m_combox_setpointdisplay.ResetContent();
	//     m_combox_setpointdisplay.AddString(_T("Temperature "));
	//     m_combox_setpointdisplay.AddString(_T("Co2"));
	//     m_combox_setpointdisplay.AddString(_T("Hum"));
		// initialize PID2 combo box
	//     if (product_register_value[704]>=0&&product_register_value[704]<=2)
	//     {
	//         m_combox_setpointdisplay.SetCurSel(product_register_value[704]);
	//     }
	//     else
	//     {
	//         m_combox_setpointdisplay.SetCurSel(0);
	//     }

	InitPID2ComboBox();

	//Refresh();//Changed by Fance ,When initial the dialog ,it should be mapping by the product.
	////if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))
	////	Refresh6();
	////else
	////	Refresh();



	if (hFirstThread != NULL)
		TerminateThread(hFirstThread, 0);
	hFirstThread = NULL;
	if (!hFirstThread)
	{
		hFirstThread = CreateThread(NULL, NULL, BackparaFreshProc, this, NULL, 0);
	}
	// pParamBackFresh = AfxBeginThread(BackparaFreshProc,this);
	//pParamBackFresh->m_bAutoDelete =FALSE;






	// #define  DaySP		0  //address 345
	// #define  DcoolDB	1  //address 346
	// #define  DheatDB	2  //address 347
	// #define  DcoolSP	3  //address 348
	// #define  DheatSP	4  //address 349
	// #define  NightSP	5  //address 350
	// #define  NheatDB	6  //address 352
	// #define  NcoolDB	7  //address 353
	// #define  NheatSP	8  //address 354
	// #define  NcoolSP	9  //address 355
	// #define  Max		10  //address 365
	// #define  Min		11  //address 366



	//Setting the refresh button,add by Fan
	HICON hIcon = NULL;
	HINSTANCE hInstResource = NULL;
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_REFRESH), RT_GROUP_ICON);
	hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_REFRESH), IMAGE_ICON, 16, 16, 0);
	((CButton *)GetDlgItem(IDC_REFRESHBUTTON))->SetIcon(hIcon);

	m_brush.CreateSolidBrush(RGB(255, 0, 0));


	if (product_type == T3000_6_ADDRESS)
	{
		INPUT_SETTING[3] = g_strInName1;
		INPUT_SETTING[4] = g_strInName2;
		INPUT_SETTING[5] = g_strInName3;
		INPUT_SETTING[6] = g_strInName4;
		INPUT_SETTING[7] = g_strInName5;
		INPUT_SETTING[8] = g_strInName6;
		INPUT_SETTING[9] = g_strInName7;
		INPUT_SETTING[10] = g_strInName8;


		if ((product_register_value[20] & 2) != 2)
		{
			INPUT_SETTING[11] = _T("Spare");
		}

		if ((product_register_value[MODBUS_TSTAT6_CO2_AVALUE] >= 0) && (product_register_value[MODBUS_TSTAT6_CO2_AVALUE] <= 3000))
		{
			//  m_disable_CO2=TRUE;
		}
		else
		{
			//m_disable_CO2=FALSE;
			INPUT_SETTING[12] = _T("Spare");
		}


		INPUT_SETTING_T7[3] = g_strInName1;
		INPUT_SETTING_T7[4] = g_strInName2;
		INPUT_SETTING_T7[5] = g_strInName3;
		INPUT_SETTING_T7[6] = g_strInName4;
		INPUT_SETTING_T7[7] = g_strInName5;
		INPUT_SETTING_T7[8] = g_strInName6;
		INPUT_SETTING_T7[9] = g_strInName7;
		INPUT_SETTING_T7[10] = g_strInName8;

	}

	if (product_register_value[7] == PM_TSTAT5i || product_register_value[7] == PM_TSTAT6 || product_register_value[7] == PM_TSTAT7 || product_register_value[7] == PM_TSTAT8
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
	{
		m_InputSelect1.ResetContent();
		m_inputSelect2.ResetContent();
		m_inputSelect3.ResetContent();
		for (int i = 0; i < NUM_INPUT_SETTING; i++)
		{
			m_InputSelect1.AddString(INPUT_SETTING[i]);
			m_inputSelect2.AddString(INPUT_SETTING[i]);
			m_inputSelect3.AddString(INPUT_SETTING[i]);
		}
	}
    else if ((product_register_value[7] == PM_PM5E) || (product_register_value[7] == PM_TSTAT5E))
    {
        //Fandu add 2018 01 08;
        m_InputSelect1.ResetContent();
        m_InputSelect1.AddString(INPUT_SETTING[0]);
        m_InputSelect1.AddString(INPUT_SETTING[1]);
        m_InputSelect1.AddString(INPUT_SETTING[3]);
    }
	if (product_register_value[7] == PM_TSTAT7)
	{
		m_InputSelect1.ResetContent();
		m_inputSelect2.ResetContent();

		for (int i = 0; i < NUM_INPUT_SETTING_T7; i++)
		{
			m_InputSelect1.AddString(INPUT_SETTING_T7[i]);
			m_inputSelect2.AddString(INPUT_SETTING_T7[i]);

		}
	}
	//ShowPID3();
	m_keyLockCombox.AddString(_T("Lock Off"));
	m_keyLockCombox.AddString(_T("FAN.SP UNLOCK.Menu Lock."));
	m_keyLockCombox.AddString(_T("FAN.SP UNLOCK.Menu Part Lock CAL,ORT,Unlock."));
	m_keyLockCombox.AddString(_T("FAN.SP LOCK.Menu Lock"));
	m_keyLockCombox.AddString(_T("FAN.SP UNLOCK.Most Menu Unlock "));

	CRect rect;
	GetClientRect(&rect);
	CString strTemp;
	if ((product_register_value[7] == PM_TSTAT6 || product_register_value[7] == PM_TSTAT8)
		/*&& product_register_value[MODBUS_RS485_MODE] == 1*/)
	{
		GetDlgItem(IDC_BUTTON_ZIGBEE)->ShowWindow(1);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ZIGBEE)->ShowWindow(0);
	}

	if (product_register_value[7] == PM_TSTAT5i || product_register_value[7] == PM_TSTAT6 || product_register_value[7] == PM_TSTAT7 || product_register_value[7] == PM_TSTAT8
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
	{
		GetDlgItem(IDC_INPUTSBUTTON)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_OutPutsBUTTON)->ShowWindow(SW_HIDE);
	}
	else
	{
		GetDlgItem(IDC_INPUTSBUTTON)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_OutPutsBUTTON)->ShowWindow(SW_SHOW);
	}
	 
	if (product_register_value[7] == PM_TSTAT6 || product_register_value[7] == PM_TSTAT8 || product_register_value[7] == PM_TSTAT8_WIFI || product_register_value[7] == PM_TSTAT8_OCC || product_register_value[7] == PM_TSTAT7_ARM || product_register_value[7] == PM_TSTAT8_220V)	 
	{
		GetDlgItem(IDC_BUTTON_ZIGBEE)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ZIGBEE)->ShowWindow(FALSE);
	}

    Reflesh_ParameterDlg();



    UpdateData(FALSE);
    
    //////////////////////////////////////////////////////////////////////////
    
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
/// <summary>
/// InitPID2ComboBox
/// </summary>
void CParameterDlg::InitPID2ComboBox()
{
    //m_inputSelect2.Clear();

    //ASSERT(m_inputSelect2.GetCount() == 0);
    m_inputSelect2.ResetContent();


    if(product_register_value[MODBUS_PRODUCT_MODEL] == PM_PRESSURE)
    {
        m_inputSelect2.AddString(_T("None"));
        m_inputSelect2.AddString(_T("Pressure"));
    }
    else
    {
        if((product_register_value[20]&2)==2)
        {
            m_disable_hum=TRUE;
        }
        else
        {
            m_disable_hum=FALSE;
        }
        if((product_register_value[20]&4)==4)
        {
            m_disable_CO2=TRUE;
        }
        else
        {
            m_disable_CO2=FALSE;
        }

        //Loop1 : LCD
        if (product_type == T3000_5EH_LCD_ADDRESS)
        {
            m_InputSelect1.ResetContent();
            for (int i=0; i<6; i++)
            {
                m_InputSelect1.AddString(INPUT_SETTING_LCD[i]);
            }
        }

        m_inputSelect2.AddString(_T("None"));
        m_inputSelect2.AddString(g_strInName1);
        m_inputSelect2.AddString(g_strInName2);
        m_inputSelect2.AddString(g_strInName3);
        m_inputSelect2.AddString(g_strInName4);
        m_inputSelect2.AddString(g_strInName5);
        m_inputSelect2.AddString(g_strInName6);
        m_inputSelect2.AddString(g_strInName7);
        m_inputSelect2.AddString(g_strInName8);


        // VAV  LCD

        /*	m_inputSelect3.AddString(_T("None"));
        	m_inputSelect3.AddString(g_strInName1);
        	m_inputSelect3.AddString(g_strInName2);
        	m_inputSelect3.AddString(g_strInName3);
        	m_inputSelect3.AddString(g_strInName4);
        	m_inputSelect3.AddString(g_strInName5);
        	m_inputSelect3.AddString(g_strInName6);
        	m_inputSelect3.AddString(g_strInName7);
        	m_inputSelect3.AddString(g_strInName8);*/

        //    ||product_register_value[7]==PM_TSTAT7

        if(product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||product_register_value[7]==PM_TSTAT8
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
        {
            m_InputSelect1.ResetContent();
            m_inputSelect2.ResetContent();
            m_inputSelect3.ResetContent();
            for (int i=0; i<NUM_INPUT_SETTING; i++)
            {
                m_InputSelect1.AddString(INPUT_SETTING[i]);
                m_inputSelect2.AddString(INPUT_SETTING[i]);
                m_inputSelect3.AddString(INPUT_SETTING[i]);
            }
        }

        if (product_register_value[7]==PM_TSTAT7)
        {
            m_InputSelect1.ResetContent();
            m_inputSelect2.ResetContent();

            for (int i=0; i<NUM_INPUT_SETTING_T7; i++)
            {
                m_InputSelect1.AddString(INPUT_SETTING_T7[i]);
                m_inputSelect2.AddString(INPUT_SETTING_T7[i]);

            }
        }

    }


}


/// <summary>
/// 
/// </summary>
void CParameterDlg::OnBnClickedOk()
{
    
    //OnOK();
}
/// <summary>
/// 
/// </summary>
void CParameterDlg::OnBnClickedRefreshbutton()
{
    Reflesh_ParameterDlg();
    UpdateData(FALSE);

}

//void CParameterDlg::OnBnClickedEnableidbutton()
//{
//    // 	m_idAdressEdit.EnableWindow(TRUE);
//    // 	
//
//    if(m_idAdressEdit.IsWindowEnabled())
//    {
//        m_idAdressEdit.EnableWindow(FALSE);
//        ((CButton *)GetDlgItem(IDC_ENABLEIDBUTTON))->SetWindowTextW(_T("Enable Change"));
//    }
//    else
//    {
//        m_idAdressEdit.EnableWindow(TRUE);
//        ((CButton *)GetDlgItem(IDC_ENABLEIDBUTTON))->SetWindowTextW(_T("Disable Change"));
//    }
//}

void CParameterDlg::OnCbnSelchangeAutoonlycombo()
{
    //int nindext=m_autoOnlyCombox.GetCurSel();


    //if(product_register_value[MODBUS_AUTO_ONLY]==nindext)	//Add this to judge weather this value need to change.
    //	return;

    //Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_AUTO_ONLY,nindext,
    //	product_register_value[MODBUS_AUTO_ONLY],this->m_hWnd,IDC_AUTOONLYCOMBO,_T("AUTO ONLY"));


    
}

//void CParameterDlg::OnEnKillfocusIdaddressedit()
//{
//    CMainFrame* pPraent=(CMainFrame*)GetParent();
//    if(g_ParamLevel==1)
//        return;
//
//    CString strTemp;
//    m_idAdressEdit.GetWindowText(strTemp);
//    strTemp.Trim();
//    if(strTemp.IsEmpty())
//        return;
//    int nID=_wtoi(strTemp);
//    if(nID<=0||nID>=255)
//    {
//        AfxMessageBox(_T("Please input a correct ID!"));
//        return;
//    }
//    if(g_tstat_id==nID)
//        return;
//
// 
//	CppSQLite3DB SqliteDBBuilding;
//	CppSQLite3Table table;
//	CppSQLite3Query q;
//	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
//
//
//    CString strSql;
//    strSql.Format(_T("select * from ALL_NODE where Product_ID ='%s'"),strTemp);
//    //m_pRsTemp->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
//   q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
//    if(!q.eof())
//    {
//        CString strPop;
//        strPop.Format(_T("There is another TSTAT has the ID ='%s', Please input a new one!"),strTemp);
//        AfxMessageBox(strPop);
// 		if(m_pRsTemp->State)
// 			m_pRsTemp->Close();
//        SqliteDBBuilding.closedb();
//        return;
//    }
//   
//
//    int nRet=0;
//    g_bPauseMultiRead = TRUE;
//    nRet=write_one(g_tstat_id, 6,nID);
//
//    Sleep(1000);
//
//    nRet=write_one(g_tstat_id, 6,nID);
//    if(nRet>0)
//    {
//        CString strSerial;
//        strSerial.Format(_T("%d"),g_serialNum);
//        strSerial.Trim();
//        g_tstat_id=nID;
//
//        // 修改product name
//        int nProductClassID = 0;
//        for(UINT i = 0; i < pPraent->m_product.size(); i++)
//        {
//            if(pPraent->m_product[i].serial_number== g_serialNum)
//            {
//                nProductClassID = pPraent->m_product[i].product_class_id;
//                break;
//            }
//        }
//        CString strProductName;
//        CString strNewID;
//        if (nProductClassID != 0)
//        {
//
//            strNewID.Format(_T("%d"), g_tstat_id);
//            strProductName = pPraent->GetDeviceClassName(nProductClassID);
//            strProductName = strProductName+_T(":")+strSerial+_T("--")+strNewID;
//        }
//        CString strScreenName = pPraent->GetScreenName(g_serialNum, g_tstat_id);
//
//        //
//
//        try
//        {
//
//            strSql.Format(_T("update ALL_NODE set Product_ID ='%s', Product_name = '%s', Screen_Name = '%s' where Serial_ID = '%s'"),strNewID,strProductName,strScreenName,strSerial);
//            SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
//        }
//        catch(_com_error *e)
//        {
//            AfxMessageBox(e->ErrorMessage());
//        }
//
//        TRACE(strSql);
//
//
//    }
//    g_bPauseMultiRead = FALSE;
//
//    SqliteDBBuilding.closedb();
//
//    //Sleep(1000);
//    pPraent->ScanTstatInDB();
//}

void CParameterDlg::OnDestroy()
{

    CDialog::OnDestroy();

    //DWORD dwCode;
    //if (pParamBackFresh!=NULL)
    //{
    //	GetExitCodeThread(pParamBackFresh->m_hThread,&dwCode);
    //	if (dwCode==STILL_ACTIVE)
    //	{
    //		Sleep(200);

    //		TerminateThread(pParamBackFresh->m_hThread,dwCode);
    //		delete pParamBackFresh;
    //	}
    //}
    if(hFirstThread != NULL)
        TerminateThread(hFirstThread, 0);
    
}

void CParameterDlg::OnCbnSelchangekeypadcombo()
{
    int nItem=m_keySelectCombox.GetCurSel();
    switch(nItem)
    {
    case 0:
        nItem=0;
        break;
    case 1:
        nItem=5;
        break;
    case 2:
        nItem=1;
        break;
    case 3:
        nItem=4;
        break;
    case 4:
        nItem=2;
        break;
    case 5:
        nItem=3;
        break;
    case 6:
        nItem=6;
        break;
    case 7:
        nItem=7;
        break;
    case 8:
        nItem=8;
        break;
    default :
        nItem=0;
        break;
    }

    if(product_register_value[MODBUS_KEYPAD_SELECT]==nItem)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_KEYPAD_SELECT,nItem,
                        product_register_value[MODBUS_KEYPAD_SELECT],this->m_hWnd,IDC_KEYPADSELECT,_T("KEYPAD SELECT"));



    //g_bPauseMultiRead = TRUE;
    //if(write_one(g_tstat_id,MODBUS_KEYPAD_SELECT,nItem)<0)// 128 395
    //{
    //	MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
    //	nItem = product_register_value[MODBUS_KEYPAD_SELECT];// resume the old status.
    //	switch(nItem)
    //	{
    //	case 0:nItem=0;break;
    //	case 1:nItem=2;break;
    //	case 2:nItem=4;break;
    //	case 3:nItem=5;break;
    //	case 4:nItem=3;break;
    //	case 5:nItem=1;break;
    //	case 6:nItem=6;break;
    //	case 7:nItem=7;break;
    //	case 8:nItem=8;break;
    //	default :nItem=0;break;
    //	}
    //	m_keySelectCombox.SetCurSel(nItem);
    //}
    //else
    //	product_register_value[MODBUS_KEYPAD_SELECT] = nItem;
    ////	write_one(g_tstat_id,128,nItem,5);	//Marked by Fance

    //g_bPauseMultiRead = FALSE;
}

//void CParameterDlg::OnCbnKillfocusKeypadselect()
//{
//	
//
//		int nItem=m_keySelectCombox.GetCurSel();
//		switch(nItem)
//		{
//		case 0:nItem=0;break;
//		case 1:nItem=5;break;
//		case 2:nItem=1;break;
//		case 3:nItem=4;break;
//		case 4:nItem=2;break;
//		case 5:nItem=3;break;
//		case 6:nItem=6;break;
//		case 7:nItem=7;break;
//		case 8:nItem=8;break;
//		default :nItem=0;break;
//		}
//		g_bPauseMultiRead = TRUE;
//		int MODBUS_KEYPAD_SELECT = MODBUS_KEYPAD_SELECT;// 128 395
//		if(write_one(g_tstat_id,MODBUS_KEYPAD_SELECT,nItem)<0)
//		{
//			MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
//			nItem = product_register_value[MODBUS_KEYPAD_SELECT];// resume the old status.
//			switch(nItem)
//			{
//			case 0:nItem=0;break;
//			case 1:nItem=2;break;
//			case 2:nItem=4;break;
//			case 3:nItem=5;break;
//			case 4:nItem=3;break;
//			case 5:nItem=1;break;
//			case 6:nItem=6;break;
//			case 7:nItem=7;break;
//			case 8:nItem=8;break;
//			default :nItem=0;break;
//			}
//			m_keySelectCombox.SetCurSel(nItem);
//		}
//		else
//			product_register_value[MODBUS_KEYPAD_SELECT] = nItem;
//	//	write_one(g_tstat_id,128,nItem,5);	//Marked by Fance
//
//		g_bPauseMultiRead = FALSE;
//}


//This Edit is invisible for all product, May be it can be delete.
void CParameterDlg::OnEnKillfocusValuposedit()
{
    if(g_ParamLevel==1)
        return;
    CString strTemp;
    m_value_percentEdit.GetWindowText(strTemp);
    if(strTemp.IsEmpty())
        return;
    //MODBUS_VALVE_PERCENT
    if(product_register_value[MODBUS_VALVE_PERCENT]==_wtoi(strTemp))	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_VALVE_PERCENT,_wtoi(strTemp),
                        product_register_value[MODBUS_VALVE_PERCENT],this->m_hWnd,IDC_VALUPOSEDIT,_T("VALVE_PERCENT"));

    //g_bPauseMultiRead = TRUE;
    //write_one(g_tstat_id, 285,_wtoi(strTemp), 5);
    //g_bPauseMultiRead = FALSE;
}

void CParameterDlg::OnCbnSelendcancelBaudratecombo()
{

    if(g_ParamLevel==1)
        return;

    if(product_register_value[MODBUS_BAUDRATE]==m_baudRateCombox.GetCurSel())	//Add this to judge weather this value need to change.
        return;

   /* Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_BAUDRATE,m_baudRateCombox.GetCurSel(),
                        product_register_value[MODBUS_BAUDRATE],this->m_hWnd,IDC_BAUDRATECOMBO,_T("BAUDRATE"));*/

	int ret = write_one(g_tstat_id,MODBUS_BAUDRATE,m_baudRateCombox.GetCurSel());
    int index_brandrate = m_baudRateCombox.GetCurSel(); 
	int brandrate = 19200;

	if (index_brandrate ==0)
	{
		brandrate = 9600;
	}
	else if (index_brandrate == 1)
	{
		brandrate = 19200;
	}
	else if (index_brandrate == 2)
	{
		brandrate = 38400;
	}
	else if (index_brandrate == 3)
	{
		brandrate = 57600;
	}
	else if (index_brandrate == 4)
	{
		brandrate = 115200;
	}
    CString SqlText;
     
        SqlText.Format(_T("update ALL_NODE set Bautrate = '%d' where Serial_ID='%d'"),brandrate,get_serialnumber());
        Change_BaudRate(brandrate);
    

		CppSQLite3DB SqliteDBBuilding;
		CppSQLite3Table table;
		CppSQLite3Query q;
		SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

		 SqliteDBBuilding.execDML((UTF8MBSTR)SqlText);
		 SqliteDBBuilding.closedb();
    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    pFrame->ScanTstatInDB();


    //g_bPauseMultiRead = TRUE;

    //if(write_one(g_tstat_id, MODBUS_BAUDRATE, m_baudRateCombox.GetCurSel())<0)	//Modify by Fance
    //	MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
    //else
    //	product_register_value[MODBUS_BAUDRATE] = m_baudRateCombox.GetCurSel();
    //g_bPauseMultiRead = FALSE;
}



void CParameterDlg::OnCbnSelchangePowermodelcombo()
{

    if(g_ParamLevel==1)
        return;

    if(product_register_value[MODBUS_POWERUP_MODE]==m_powerupModelCombox.GetCurSel())	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_POWERUP_MODE,m_powerupModelCombox.GetCurSel(),
                        product_register_value[MODBUS_POWERUP_MODE],this->m_hWnd,IDC_POWERMODELCOMBO,_T("POWERUP_MODE"));



    //g_bPauseMultiRead = TRUE;
    //if(write_one(g_tstat_id, MODBUS_POWERUP_MODE, m_powerupModelCombox.GetCurSel())<0)	//127  106
    //	{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
    //else
    //	product_register_value[MODBUS_POWERUP_MODE] = m_powerupModelCombox.GetCurSel();
    //g_bPauseMultiRead = FALSE;
}


void CParameterDlg::OnCbnSelchangeKeypadlockcombo()
{
    if(product_register_value[MODBUS_SPECIAL_MENU_LOCK]==m_keyLockCombox.GetCurSel())	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_SPECIAL_MENU_LOCK,m_keyLockCombox.GetCurSel(),
                        product_register_value[MODBUS_SPECIAL_MENU_LOCK],this->m_hWnd,IDC_KEYPADLOCKCOMBO,_T("SPECIAL MENU LOCK"));


    
}



void CParameterDlg::OnCbnSelchangeDisplaycombo()
{
    if(g_ParamLevel==1)
        return;
    if(product_register_value[MODBUS_DISPLAY]==m_displayCombox.GetCurSel())	//Check if value has changed.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_DISPLAY,m_displayCombox.GetCurSel(),
                        product_register_value[MODBUS_DISPLAY],this->m_hWnd,IDC_DISPLAYCOMBO,_T("DISPLAY"));

}

void CParameterDlg::OnCbnSelchangeSequencecombox()
{

    if(g_ParamLevel==1)
        return;

    if(product_register_value[MODBUS_SEQUENCE]==m_SequenceCombox.GetCurSel())	//Check if value has changed.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_SEQUENCE,m_SequenceCombox.GetCurSel(),
                        product_register_value[MODBUS_SEQUENCE],this->m_hWnd,IDC_SEQUENCECOMBOX,_T("SEQUENCE"));
}



 

void CParameterDlg::OnCbnSelchangeHcchangecombo()
{
    if(g_ParamLevel==1)
        return;
    if(product_register_value[MODBUS_HEAT_COOL_CONFIG]==m_hcChangeCombox.GetCurSel())	//Check if value has changed.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_HEAT_COOL_CONFIG,m_hcChangeCombox.GetCurSel(),
                        product_register_value[MODBUS_HEAT_COOL_CONFIG],this->m_hWnd,IDC_HCCHANGECOMBO,_T("HEAT_COOL_CONFIG"));
}



//Modify to support T6 T7 by Fance
void CParameterDlg::OnEnKillfocusPowsetpomitedit()
{

    if(g_ParamLevel==1)
        return;
    CString strTemp;
    m_powerSetPointEdit.GetWindowText(strTemp);
    if(strTemp.IsEmpty())
        return;
    if(product_register_value[MODBUS_POWERUP_SETPOINT]==_wtoi(strTemp))		//Check if value has changed.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_POWERUP_SETPOINT,_wtoi(strTemp),
                        product_register_value[MODBUS_POWERUP_SETPOINT],this->m_hWnd,IDC_POWSETPOMITEDIT,_T("POWERUP SETPOINT"));

}


void CParameterDlg::OnEnKillfocusStincreaedit()	//recode by Fance,for support T6 T7
{
    if(g_ParamLevel==1)
        return;
    CString strTemp;
    m_setpointIncreasement.GetWindowText(strTemp);
    if(strTemp.IsEmpty())
        return;
    int nvalue = (int)(_wtof(strTemp) *10);
    if(nvalue>10)
    {
        MessageBox(_T("Please Input the value between 0 - 1 "),_T("Warning"),MB_OK | MB_ICONINFORMATION);
        nvalue = 10;
    }
    if(product_register_value[MODBUS_SETPOINT_INCREASE] == nvalue)	//Check if value has changed.
        return;


    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_SETPOINT_INCREASE,nvalue,
                        product_register_value[MODBUS_SETPOINT_INCREASE],this->m_hWnd,IDC_STINCREAEDIT,_T("SETPOINT INCREASE"));
}


//Modify by Fance , support T6T7
void CParameterDlg::OnEnKillfocusDefSetpointEdt()
{

    if(g_ParamLevel==1)
        return;
    CString strTemp;
    m_defSetPointEdit.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
        return;

    if(product_register_value[MODBUS_DEFAULT_SETPOINT]==_wtoi(strTemp))	//Check if value has changed.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_DEFAULT_SETPOINT,_wtoi(strTemp),
                        product_register_value[MODBUS_DEFAULT_SETPOINT],this->m_hWnd,IDC_DEF_SETPOINT_EDT,_T("DEFAULT_SETPOINT"));

}

void CParameterDlg::OnEnKillfocusSetpointctledit()
{
    if(g_ParamLevel==1)
        return;

}

void CParameterDlg::OnEnKillfocusInfileredit()
{

    if(g_ParamLevel==1)
        return;
    CString strTemp;
    m_inputFilterEdit.GetWindowText(strTemp);
    int ndata=_wtoi(strTemp);
    if (ndata<0)
        ndata=0;
    if (ndata>100)
        ndata=100;
    if(product_register_value[MODBUS_FILTER] == ndata) //Check if value has changed.
        return;//213  142


    if(product_register_value[MODBUS_FILTER]==ndata)	//Check if value has changed.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_FILTER,ndata,
                        product_register_value[MODBUS_FILTER],this->m_hWnd,IDC_INFILEREDIT,_T("FILTER"));
}

void CParameterDlg::OnEnKillfocusCycleedit()
{
    if(g_ParamLevel==1)
        return;

    CString strTemp;
    m_cycledlayEdit.GetWindowText(strTemp);
    if (strTemp.IsEmpty())
        return;
    int nValue=_wtoi(strTemp);
    if(product_register_value[MODBUS_CYCLING_DELAY] ==nValue)	//Check if value has changed.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_CYCLING_DELAY,nValue,
                        product_register_value[MODBUS_CYCLING_DELAY],this->m_hWnd,IDC_CYCLEEDIT,_T("CYCLING_DELAY"));
}

//void CParameterDlg::OnEnKillfocusTempalarmedit()
//{
//    //m_tempAlarmEdit
//}




//Modify by Fance  2013  04 11
void CParameterDlg::OnEnKillfocusTimeronedit()
{
    if(g_ParamLevel==1)
        return;
    CString strTemp;
    m_timerOnEdit.GetWindowText(strTemp);
    if(strTemp.IsEmpty())
        return;
    if(product_register_value[MODBUS_TIMER_ON]==_wtoi(strTemp))
        return;
    //Change_Color_ID.push_back(IDC_TIMERONEDIT);
    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_TIMER_ON,
                        _wtoi(strTemp),product_register_value[MODBUS_TIMER_ON],
                        this->m_hWnd,IDC_TIMERONEDIT,_T("Time On"));
}


//Modify by Fance  2013  04 11
void CParameterDlg::OnEnKillfocusEdit12()
{

    if(g_ParamLevel==1)
        return;

    CString strTemp;
    m_timerOffEdit.GetWindowText(strTemp);
    if(strTemp.IsEmpty())
        return;

    if(product_register_value[MODBUS_TIMER_OFF]==_wtoi(strTemp))
        return;


    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_TIMER_OFF,_wtoi(strTemp),
                        product_register_value[MODBUS_TIMER_OFF],this->m_hWnd,IDC_EDIT12,_T("Time Off"));


}


//Recode by Fance
void CParameterDlg::OnCbnSelchangeUnitcombo()
{
    if(g_ParamLevel==1)
        return;
    if(product_register_value[MODBUS_TIMER_UNITS]==m_uniteCombox.GetCurSel())
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_TIMER_UNITS,m_uniteCombox.GetCurSel(),
                        product_register_value[MODBUS_TIMER_UNITS],this->m_hWnd,IDC_UNITCOMBO,_T("Units"));

}

void CParameterDlg::OnEnKillfocusTimerleft()
{

    if(g_ParamLevel==1)
        return;
    CString strTemp;
    m_timerLeft.GetWindowText(strTemp);
    if(strTemp.IsEmpty())
        return;




    if(product_register_value[MODBUS_ROTATION_TIME_LEFT]==_wtoi(strTemp))	//Check if value has changed.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_ROTATION_TIME_LEFT,_wtoi(strTemp),
                        product_register_value[MODBUS_ROTATION_TIME_LEFT],this->m_hWnd,IDC_TIMERLEFT,_T("ROTATION TIME LEFT"));

}


//modify by Fance 2013 04 11     Fance_0411
void CParameterDlg::OnCbnSelchangeTimerdelectcombo()
{
    if(g_ParamLevel==1)
        return;
    int sel=m_timerSelectCombox.GetCurSel();
    if (sel!=0)
    {
        sel++;
    }
    if(product_register_value[MODBUS_TIMER_SELECT]==sel)	//Check if value has changed.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_TIMER_SELECT,sel,
                        product_register_value[MODBUS_TIMER_SELECT],this->m_hWnd,IDC_TIMERDELECTCOMBO,_T("TIMER_SELECT"));
    /*if (product_register_value[MODBUS_TIMER_SELECT]==3)
    {
    GetDlgItem(IDC_EDIT12)->EnableWindow(FALSE);
    }
    else
    {
      GetDlgItem(IDC_EDIT12)->EnableWindow(TRUE);
    }*/
}

//modify for support t6 t7.
void CParameterDlg::OnEnKillfocusTimeedit()
{
    if(g_ParamLevel==1)
        return;

    CString strTemp;
    m_TimeEdit.GetWindowText(strTemp);
    int nValue=_wtoi(strTemp);

    if (strTemp.IsEmpty())
        return;

    if(product_register_value[MODBUS_OVERRIDE_TIMER_LEFT] ==nValue)	//Check if value has changed.
        return;

    //m_hWnd
    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,
                        MODBUS_OVERRIDE_TIMER_LEFT,nValue,
                        product_register_value[MODBUS_OVERRIDE_TIMER_LEFT],
                        this->m_hWnd,IDC_TIMEEDIT,_T("Time Left"));

}



//modify for support t6 t7.
void CParameterDlg::OnEnKillfocusOverridetimeedit()
{
    if(g_ParamLevel==1)
        return;

    CString strTemp;
    m_OverRideEdit.GetWindowText(strTemp);
    int nValue=_wtoi(strTemp);

    if (strTemp.IsEmpty())
        return;

    if(product_register_value[MODBUS_OVERRIDE_TIMER] ==nValue)	//Check if value has changed.
        return;


    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_OVERRIDE_TIMER,nValue,
                        product_register_value[MODBUS_OVERRIDE_TIMER],this->m_hWnd,IDC_OVERRIDETIMEEDIT,_T("OVERRIDE TIMER"));

}


void CParameterDlg::OnBnClickedFreecoolbgtn()
{
    if(g_ParamLevel==1)
        return;

    CFreeCoolDlg Dlg;
    Dlg.DoModal();
}

void CParameterDlg::OnBnClickedOutdoorresetbtn()
{
    if(g_ParamLevel==1)
        return;

    COutsidDailog Dlg;
    Dlg.DoModal();
    
}


//Recode by Fance
void CParameterDlg::OnCbnSelchangeInputselect1()
{
    if(g_ParamLevel==1)
        return;
    int sel = 0;
    if ((product_register_value[7] == PM_PM5E) || (product_register_value[7] == PM_TSTAT5E))
    {
        int temp_sel;
        CString selstring;
        temp_sel = m_InputSelect1.GetCurSel();
        m_InputSelect1.GetLBText(temp_sel, selstring);

        //GetDlgItem(IDC_INPUTSELECT1)->GetWindowTextW(selstring);
        if (selstring.CompareNoCase(INPUT_SETTING[3]) == 0)
            sel = 1;
        else if (selstring.CompareNoCase(INPUT_SETTING[0]) == 0)
            sel = 2;
        else if (selstring.CompareNoCase(INPUT_SETTING[1]) == 0)
            sel = 3;
        else
            return;
    }
    else
    {
        sel = m_InputSelect1.GetCurSel();

        sel += 2;
        if (product_type == T3000_5EH_LCD_ADDRESS)
        {
            sel += 1;
        }
    }
   
    if(product_register_value[MODBUS_TEMP_SELECT]==sel)//Check if value has changed.
        return;
    product_register_value[MODBUS_TEMP_SELECT] = sel;
    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_TEMP_SELECT,sel,
                        product_register_value[MODBUS_TEMP_SELECT],this->m_hWnd,IDC_INPUTSELECT1,_T("TEMP SELECT"));
    Reflesh_ParameterDlg();
}


//Recode by Fance 2013 04 11
void CParameterDlg::OnCbnSelchangeInputselect2()
{
    if(g_ParamLevel==1)
        return;
    /*	int nSel = m_inputSelect2.GetCurSel();	*/
    unsigned short TempValue;
    int sel=m_inputSelect2.GetCurSel();
// 	if (sel==0)
// 	{
    TempValue=sel+2;
// 	}
// 	else
// 	{
// 		TempValue=sel+4;
// 	}

    CString strText;
    m_inputSelect2.GetWindowTextW(strText);
    if (strText.IsEmpty())
    {
        Reflesh_ParameterDlg();
        return;
    }
    if(product_register_value[MODBUS_INPUT1_SELECT]==TempValue)	//Check if value has changed.
        return;
    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_INPUT1_SELECT,TempValue,
                        product_register_value[MODBUS_INPUT1_SELECT],this->m_hWnd,IDC_INPUTSELECT2,_T("INPUT1_SELECT"));


    Reflesh_ParameterDlg();
}



void CParameterDlg::OnEnKillfocusInputvalue2()
{

    if(g_ParamLevel==1)
        return;
    CString strTemp;
    m_inputValue2.GetWindowText(strTemp);
    if(strTemp.IsEmpty())
        return;
    int nValue=_wtoi(strTemp);
    g_bPauseMultiRead = TRUE;
    if(product_register_value[241]==2)
    {
        if(product_register_value[189]==4||product_register_value[189]==1)
            write_one(g_tstat_id, 181,nValue*10);
        else
            write_one(g_tstat_id, 181,nValue);

    }
    else
    {
        if(product_register_value[MODBUS_ANALOG_IN1]==4||product_register_value[MODBUS_ANALOG_IN1]==1)
            write_one(g_tstat_id, 180,nValue*10);
        else
            write_one(g_tstat_id, 180,nValue);
    }
    g_bPauseMultiRead = FALSE;
}

void CParameterDlg::OnCbnSelchangeEapplication()
{
    if(g_ParamLevel==1)
        return;

    g_bPauseMultiRead = TRUE;
    int nRet = 0;

    float m_fFirmwareVersion=get_curtstat_version();

    if (product_register_value[7] == PM_TSTAT7 || product_register_value[7] == PM_TSTAT6|| product_register_value[7] == PM_TSTAT5i|| product_register_value[7] == PM_TSTAT8
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {
        short nOccupied = product_register_value[184];  // Day setpoint option
        BOOL bOccupied = nOccupied & 0x0001;
        if (bOccupied)  // day  - Occupied
        {
            //nRet = write_one(g_tstat_id, 454 ,m_application_ctrl.GetCurSel()); // 5E 以及以前的型号  //tstat6 在TSTAT6会死在这里
        }
        else
        {
            //write_one(g_tstat_id,125 ,m_application_ctrl.GetCurSel()); //tstat6
            //nRet = write_one(g_tstat_id,reg_tststold[125],m_application_ctrl.GetCurSel()); //tstat6 350

        }
        nRet = write_one(g_tstat_id,reg_tststold[125],m_application_ctrl.GetCurSel()); //tstat6 350

    }
    else if(((product_register_value[7]==PM_TSTAT5E||product_register_value[7] == PM_PM5E)||(product_register_value[7]==PM_TSTATRUNAR)) && (m_fFirmwareVersion >= 35.4))//0912
    {
        write_one(g_tstat_id,423 ,m_application_ctrl.GetCurSel()); //
        nRet=write_one(g_tstat_id,125 ,m_application_ctrl.GetCurSel()); //
    }
    else
    {
        nRet=write_one(g_tstat_id,125 ,m_application_ctrl.GetCurSel()); //
    }


    if(nRet>0)
        product_register_value[125] =m_application_ctrl.GetCurSel();
    product_register_value[423] =m_application_ctrl.GetCurSel();
    Reflesh_ParameterDlg();
    g_bPauseMultiRead = FALSE;
}

void  CParameterDlg::UpdateCoolingandHeatingData()
{

    CString strText;
    UINT uint_temp=GetOEMCP();		//get system is for chinese or english'product_type == T3000_5ABCDFG_LED_ADDRESS ||
    if (product_type == T3000_6_ADDRESS)
    {
        if(uint_temp!=936 && uint_temp!=950)
        {
            //not chinese
            if(m_application_ctrl.GetCurSel()==0)
            {


                if (g_unint)
                {
                    strText.Format(_T("%d C"),product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]/10);//182
                    m_nightheating.SetWindowText(strText);
                    strText.Format(_T("%d C"),product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]/10);//183 355
                    m_nightcooling.SetWindowText(strText);
                }
                else
                {
                    strText.Format(_T("%d F"),product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]/10);//182
                    m_nightheating.SetWindowText(strText);
                    strText.Format(_T("%d F"),product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]/10);//183
                    m_nightcooling.SetWindowText(strText);
                }



            }
            else
            {
                if (g_unint)
                {
                    strText.Format(_T("%0.1f°C"),(float)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10);//123
                    m_nightheating.SetWindowText(strText);

                    //m_nightheating.Format("%d%c",product_register_value[123],176);//night heating deadband
                    strText.Format(_T("%0.1f°C"),(float)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10);		//124
                    m_nightcooling.SetWindowText(strText);

                }
                else
                {
                    strText.Format(_T("%0.1f°F"),(float)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10.0);//182   354
                    m_nightheating.SetWindowText(strText);
                    strText.Format(_T("%0.1f°F"),(float)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10.0);//183
                    m_nightcooling.SetWindowText(strText);

                }

            }
        }
        else
        {
            if(m_application_ctrl.GetCurSel()==0)
            {
                if (g_unint)
                {
                    strText.Format(_T("%d°C"),product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]/10);
                    m_nightheating.SetWindowText(strText);

                    strText.Format(_T("%d°C"),product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]/10);//183
                    m_nightcooling.SetWindowText(strText);
                }
                else
                {
                    strText.Format(_T("%d°F"),product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]/10);
                    m_nightheating.SetWindowText(strText);

                    strText.Format(_T("%d°F"),product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]/10);//183
                    m_nightcooling.SetWindowText(strText);
                }

            }
            else
            {
                if (g_unint)
                {
                    strText.Format(_T("%0.1f°C"),(float)(product_register_value[123]/10.0));
                    m_nightheating.SetWindowText(strText);

                    strText.Format(_T("%0.1f°C"),(float)(product_register_value[124]/10.0));
                    m_nightcooling.SetWindowText(strText);
                }
                else
                {
                    strText.Format(_T("%0.1f°F"),(float)product_register_value[123]/10.0);
                    m_nightheating.SetWindowText(strText);

                    strText.Format(_T("%0.1f°F"),(float)product_register_value[124]/10.0);
                    m_nightcooling.SetWindowText(strText);

                }

            }
        }
    }
    else
    {
        if(uint_temp!=936 && uint_temp!=950)
        {
            //not chinese
            if(m_application_ctrl.GetCurSel()==0)
            {


                if (g_unint)
                {
                    strText.Format(_T("%d C"),product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]);//182
                    m_nightheating.SetWindowText(strText);
                    strText.Format(_T("%d C"),product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]);//183 355
                    m_nightcooling.SetWindowText(strText);
                }
                else
                {
                    strText.Format(_T("%d F"),product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]);//182
                    m_nightheating.SetWindowText(strText);
                    strText.Format(_T("%d F"),product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]);//183
                    m_nightcooling.SetWindowText(strText);
                }



            }
            else
            {
                if (g_unint)
                {
                    strText.Format(_T("%0.1f°C"),(float)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10);//123
                    m_nightheating.SetWindowText(strText);

                    //m_nightheating.Format("%d%c",product_register_value[123],176);//night heating deadband
                    strText.Format(_T("%0.1f°C"),(float)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10);		//124
                    m_nightcooling.SetWindowText(strText);

                }
                else
                {
                    strText.Format(_T("%0.1f°F"),(float)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10.0);//182   354
                    m_nightheating.SetWindowText(strText);
                    strText.Format(_T("%0.1f°F"),(float)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10.0);//183
                    m_nightcooling.SetWindowText(strText);

                }

            }
        }
        else
        {
            if(m_application_ctrl.GetCurSel()==0)
            {
                if (g_unint)
                {
                    strText.Format(_T("%d°C"),product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]);
                    m_nightheating.SetWindowText(strText);

                    strText.Format(_T("%d°C"),product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]);//183
                    m_nightcooling.SetWindowText(strText);
                }
                else
                {
                    strText.Format(_T("%d°F"),product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]);
                    m_nightheating.SetWindowText(strText);

                    strText.Format(_T("%d°F"),product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]);//183
                    m_nightcooling.SetWindowText(strText);
                }

            }
            else
            {
                if (g_unint)
                {
                    strText.Format(_T("%0.1f°C"),(float)(product_register_value[123]/10.0));
                    m_nightheating.SetWindowText(strText);

                    strText.Format(_T("%0.1f°C"),(float)(product_register_value[124]/10.0));
                    m_nightcooling.SetWindowText(strText);
                }
                else
                {
                    strText.Format(_T("%0.1f°F"),(float)product_register_value[123]/10.0);
                    m_nightheating.SetWindowText(strText);

                    strText.Format(_T("%0.1f°F"),(float)product_register_value[124]/10.0);
                    m_nightcooling.SetWindowText(strText);

                }

            }
        }
    }



}

void CParameterDlg::OnEnKillfocusSetvalue2()
{

    if(g_ParamLevel==1)
        return;

    CString strText;
    m_pid_setptEdt2.GetWindowText(strText);
    float nValue=_wtof(strText);

    int IValue =    nValue*10;
    if(product_register_value[MODBUS_UNIVERSAL_SET]==IValue)	//Add this to judge weather this value need to change.
        return;

    //Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_UNIVERSAL_SET,nValue*10,
    //	product_register_value[MODBUS_UNIVERSAL_SET],this->m_hWnd,IDC_SETVALUE2,_T("UNIVERSAL SET"));

    int ret=write_one(g_tstat_id,MODBUS_UNIVERSAL_SET,IValue);
    if (ret>0)
    {
        product_register_value[MODBUS_UNIVERSAL_SET]=IValue;
        strText.Format(_T("%0.1f"),product_register_value[MODBUS_UNIVERSAL_SET]/10);
        CString strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN1], 1);
        //strText+=strUnit;
        m_pid_setptEdt2.SetWindowText(strText);
    }
    else
    {
        AfxMessageBox(_T("Fail!"));
    }


}

void CParameterDlg::OnEnKillfocusOutput1()
{

    if(g_ParamLevel==1)
        return;

    CString strText;
    m_pid_outputEdt1.GetWindowText(strText);
    int nValue=_wtoi(strText);

    if(product_register_value[MODBUS_COOLING_PID]==nValue)	//Add this to judge weather this value need to change.
        return;



    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_COOLING_PID,nValue,
                        product_register_value[MODBUS_COOLING_PID],this->m_hWnd,IDC_OUTPUT1,_T("COOLING PID"));

}




void CParameterDlg::OnEnKillfocusOutput2()
{

    if(g_ParamLevel==1)
        return;

    CString strText;
    m_pid_outputEdt2.GetWindowText(strText);
    int nValue=_wtoi(strText);

    if(product_register_value[MODBUS_PID_UNIVERSAL]==nValue)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_PID_UNIVERSAL,nValue,
                        product_register_value[MODBUS_PID_UNIVERSAL],this->m_hWnd,IDC_OUTPUT2,_T("PID UNIVERSAL"));

}




void CParameterDlg::OnEnKillfocusEcoolingpterm1()
{

    if(g_ParamLevel==1)
        return;

    CString strText;
    m_pternEdt1.GetWindowText(strText);
    int nValue=(int)( _wtof(strText) * 10);


    if(product_register_value[MODBUS_COOLING_PTERM]==nValue)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_COOLING_PTERM,nValue,
                        product_register_value[MODBUS_COOLING_PTERM],this->m_hWnd,IDC_ECOOLINGPTERM1,_T("COOLING PTERM"));
}


void CParameterDlg::OnEnKillfocusEcoolingpterm2()
{

    if(g_ParamLevel==1)
        return;

    CString strText;
    m_ptermEdt2.GetWindowText(strText);
    int nValue=(int)( _wtof(strText) * 10);


    if(product_register_value[MODBUS_UNIVERSAL_PTERM]==nValue)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_UNIVERSAL_PTERM,nValue,
                        product_register_value[MODBUS_UNIVERSAL_PTERM],this->m_hWnd,IDC_ECOOLINGPTERM2,_T("UNIVERSAL_PTERM"));
}





void CParameterDlg::OnEnKillfocusEdit26()
{


    if(g_ParamLevel==1)
        return;

    CString strText;
    m_coolingPitemEdt1.GetWindowText(strText);
    int nValue=(int)( _wtof(strText) * 10);


    if(product_register_value[MODBUS_COOLING_ITERM]==nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_COOLING_ITERM,nValue,
                        product_register_value[MODBUS_COOLING_ITERM],this->m_hWnd,IDC_EDIT26,_T("COOLING ITERM"));

}




void CParameterDlg::OnEnKillfocusEdit27()
{
    if(g_ParamLevel==1)
        return;

    CString strText;
    m_pidPitemEdt2.GetWindowText(strText);
    int nValue=(int)( _wtof(strText) * 10);


    if(product_register_value[MODBUS_UNIVERSAL_ITERM]==nValue)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_UNIVERSAL_ITERM,nValue,
                        product_register_value[MODBUS_UNIVERSAL_ITERM],this->m_hWnd,IDC_EDIT27,_T("UNIVERSAL ITERM"));

}


//Recode by Fance 2013 04 17
void CParameterDlg::OnEnKillfocusSpset1()
{
    if(g_ParamLevel==1)
        return;

    float fValue;
    short nOrig;
    short nMax;
    short nMin;
    CString strText;
    m_dayOccEdt1.GetWindowText(strText);
    g_bPauseMultiRead = TRUE;
    if (m_version>=SETPOINT_SPECIAL_VERSION )//Fance Comment ,don't know why?? Maybe it's error
    {
        int num;
        num=(int)(_wtof(strText)*10);
        write_one(g_tstat_id, 374,num);

    }
    else
    {
        fValue=float(_wtof(strText));
        nOrig=(short)fValue;
        nMax=(short)product_register_value[MODBUS_MAX_SETPOINT];	//131	365
        nMin=(short)product_register_value[MODBUS_MIN_SETPOINT];	//132	366
        if (nOrig>nMax)
        {
            strText.Format(_T("%d"),(short)product_register_value[MODBUS_MAX_SETPOINT]);
        }
        if (nOrig<nMin)
        {
            strText.Format(_T("%d"),(short)product_register_value[MODBUS_MIN_SETPOINT]);
        }

        if ((product_register_value[7] == PM_TSTAT7)||(product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT5i)||(product_register_value[7]==PM_TSTAT8)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
        {
            if(product_register_value[MODBUS_DAY_SETPOINT]==short(nOrig*10))
            {
                g_bPauseMultiRead = FALSE;
                return;
            }
            if(write_one(g_tstat_id, MODBUS_DAY_SETPOINT, short(nOrig*10))<0)
            {
                MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
                CString strTemp;
                strTemp.Format(_T("%.1f"),product_register_value[MODBUS_DAY_SETPOINT]/10.0);//345	set point
                m_dayOccEdt1.SetWindowText(strTemp);
                g_bPauseMultiRead = FALSE;
                return;
            }
            product_register_value[MODBUS_DAY_SETPOINT] = short(nOrig*10);
        }
        else if(m_version<34.9 || product_register_value[7] == PM_TSTAT5E||product_register_value[7] == PM_PM5E||(product_register_value[7]==PM_TSTATRUNAR))
        {
            int nRet = write_one(g_tstat_id, 135, short(nOrig));	//Fance comments: because the version which below 34.9 is too low ,135 register I Don't know it's real meaning.
        }
        else
        {
            //write_one(g_tstat_id, 374,short(fValue*10+0.5));//374
            write_one(g_tstat_id, MODBUS_TWO_BYTE_SETPOINT,short(fValue*10+0.5));	//Changed by Fance
        }
    }
    g_bPauseMultiRead = FALSE;
    Reflesh_ParameterDlg();
}



void CParameterDlg::OnEnKillfocusSpset2()
{
    if(g_ParamLevel==1)
        return;

    CString strText;
    m_dayOccEdt2.GetWindowText(strText);
    int nValue=(int)(_wtof(strText)*10);

    if(product_register_value[MODBUS_UNIVERSAL_SET]==nValue)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_UNIVERSAL_SET,nValue,
                        product_register_value[MODBUS_UNIVERSAL_SET],this->m_hWnd,IDC_SPSET2,_T("UNIVERSAL SET"));

}

void CParameterDlg::OnEnKillfocusEsetpointhi()
{
    if(g_ParamLevel==1)
        return;

    CString strText;
    m_setptHiEdit.GetWindowText(strText);
    int nValue= _wtoi(strText);
    if(nValue<=0||nValue>=255)
    {
        AfxMessageBox(_T("Setpoint Max must between 10 and 99"));
        return;
    }

    if((short)product_register_value[MODBUS_MAX_SETPOINT]==nValue)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_MAX_SETPOINT,nValue,
                        (short)product_register_value[MODBUS_MAX_SETPOINT],this->m_hWnd,IDC_ESETPOINTHI,_T("MAX SETPOINT"));



// 	CString strText;
// 	m_setptHiEdit.GetWindowText(strText);
// 	int nValue= _wtoi(strText);
// 	if(nValue<=0||nValue>=255)
// 	{
// 		AfxMessageBox(_T("Setpoint Max must between 10 and 99"));
// 		return;
// 	}
// 	g_bPauseMultiRead = TRUE;
// 	write_one(g_tstat_id,131 , nValue);g_bPauseMultiRead = FALSE;
//	Refresh();
}

void CParameterDlg::OnEnKillfocusEsetpointlo()
{
    if(g_ParamLevel==1)
        return;

    CString strText;
    m_setptLoEdit.GetWindowText(strText);
    int nValue= _wtoi(strText);


    if((short)product_register_value[MODBUS_MIN_SETPOINT]==nValue)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_MIN_SETPOINT,nValue,
                        (short)product_register_value[MODBUS_MIN_SETPOINT],this->m_hWnd,IDC_ESETPOINTLO,_T("MIN SETPOINT"));




}

void CParameterDlg::OnEnKillfocusEcooldeadband1()
{
    if(g_ParamLevel==1)
        return;
    CString strTemp;
    CString strText;
    m_setptCDDEdt1.GetWindowText(strText);
    int nValue= (int)(_wtof(strText)*10);

    g_bPauseMultiRead = TRUE;
    if((product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7)||(product_register_value[7]==PM_TSTAT5i)||(product_register_value[7]==PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {
        if(nValue == product_register_value[MODBUS_DAY_COOLING_DEADBAND])	//346
            return;

        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_DAY_COOLING_DEADBAND,nValue,
                            product_register_value[MODBUS_DAY_COOLING_DEADBAND],this->m_hWnd,IDC_ECOOLDEADBAND1,_T("DAY COOLING DEADBAND"));
    }
    else
    {
        if(nValue == product_register_value[MODBUS_COOLING_DEADBAND])	//119
            return;

        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_COOLING_DEADBAND,nValue,
                            product_register_value[MODBUS_COOLING_DEADBAND],this->m_hWnd,IDC_ECOOLDEADBAND1,_T("COOLING DEADBAND"));
    }


}




void CParameterDlg::OnEnKillfocusEcooldeadband2()
{
    if(g_ParamLevel==1)
        return;

    CString strText;
    m_setptCCDEdt2.GetWindowText(strText);
    int nValue=(int)( _wtof(strText) * 10);


    if(product_register_value[MODBUS_UNIVERSAL_DB_LO]==nValue)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_UNIVERSAL_DB_LO,nValue,
                        product_register_value[MODBUS_UNIVERSAL_DB_LO],this->m_hWnd,IDC_ECOOLDEADBAND2,_T("cool dead band"));
}


void CParameterDlg::OnEnKillfocusEcoolingiterm1()
{

    if(g_ParamLevel==1)
        return;
    CString strTemp;
    CString strText;
    m_HeadDEdt1.GetWindowText(strText);

    int nValue= (int)(_wtof(strText.GetBuffer())*10.0);

    g_bPauseMultiRead = TRUE;
    if((product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7)||(product_register_value[7]==PM_TSTAT5i)||(product_register_value[7]==PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {
        if(nValue == product_register_value[MODBUS_DAY_HEATING_DEADBAND])	//347
            return;

        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_DAY_HEATING_DEADBAND,nValue,
                            product_register_value[MODBUS_DAY_HEATING_DEADBAND],this->m_hWnd,IDC_ECOOLINGITERM1,_T("DAY HEATING DEADBAND"));
    }
    else
    {
        if(nValue == product_register_value[MODBUS_HEATING_DEADBAND])	//120
            return;

        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_HEATING_DEADBAND,nValue,
                            product_register_value[MODBUS_HEATING_DEADBAND],this->m_hWnd,IDC_ECOOLINGITERM1,_T("HEATING DEADBAND"));

    }
    Reflesh_ParameterDlg();
    g_bPauseMultiRead = FALSE;
}





void CParameterDlg::OnEnKillfocusEcoolingiterm2()
{
    if(g_ParamLevel==1)
        return;

    CString strText;
    m_HeadDEdt2.GetWindowText(strText);
    int nValue=(int)( _wtof(strText) * 10);


    if(product_register_value[MODBUS_UNIVERSAL_DB_HI]==nValue)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_UNIVERSAL_DB_HI,nValue,
                        product_register_value[MODBUS_UNIVERSAL_DB_HI],this->m_hWnd,IDC_ECOOLINGITERM2,_T("cooling iterm"));
}


void CParameterDlg::OnBnClickedInputsbutton()
{
    CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;//tstat6
    //	pMain->m_pFreshMultiRegisters->SuspendThread(); //Annul by Fance
    pMain->m_pRefreshThread->SuspendThread();
    g_bPauseMultiRead = TRUE;	//暂停主线程读取 寄存器;	//Add by Fance
    CInputSetDlg dlg;
    dlg.DoModal();
    InitPID2ComboBox();

    Reflesh_ParameterDlg();

    //	pMain->m_pFreshMultiRegisters->ResumeThread();//tstat6
    pMain->m_pRefreshThread->ResumeThread();
    g_bPauseMultiRead =FALSE;	//Add by Fance
}

void CParameterDlg::OnBnClickedOutputsbutton()
{
    g_bPauseMultiRead = TRUE;	//暂停主线程读取 寄存器;	//Add by Fance
    COutputSetDlg dlg;
    dlg.DoModal();
    g_bPauseMultiRead =FALSE;		//Add by Fance
    Reflesh_ParameterDlg();//Recode by Fance
}

void CParameterDlg::OnBnClickedOutputstablebutton()
{
    CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;


    g_bPauseMultiRead = TRUE;	//暂停主线程读取 寄存器;	//Add by Fance
    COutPutDlg Dlg;
    Dlg.DoModal();
    g_bPauseMultiRead =FALSE;	//Add by Fance

    Reflesh_ParameterDlg();
}
void CParameterDlg::OnEnKillfocusEnigntheating()
{
    if(g_ParamLevel==1)
        return;
    CString strText;
    m_nightheating.GetWindowText(strText);


    int nValue= (int)(_wtof(strText)*10);

    if(m_application_ctrl.GetCurSel()==0)
    {
        if(product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]==(int)(nValue))	//Add this to judge weather this value need to change.
            return;

        /*Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_HEATING_SETPOINT,nValue,
        	product_register_value[MODBUS_NIGHT_HEATING_SETPOINT],this->m_hWnd,IDC_ENIGNTHEATING,
        	_T("NIGHT HEATING SETPOINT"));*/
        int ret=write_one(g_tstat_id,MODBUS_NIGHT_HEATING_SETPOINT,nValue);
        if (ret>0)
        {
            product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]=nValue;
        }
        else
        {
            AfxMessageBox(_T("Fail"));
        }

        CString strText;
        strText.Format(_T("%0.1f"),(float)product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]/10.0);
        m_nightheating.SetWindowText(strText);

    }
    else
    {
        if(product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]==nValue)
            //Add this to judge weather this value need to change.
            return;

        /*Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_HEATING_DEADBAND,nValue,
        	product_register_value[MODBUS_NIGHT_HEATING_DEADBAND],this->m_hWnd,IDC_ENIGNTHEATING,
        	_T("NIGHT HEATING DEADBAND"));*/

        int ret=write_one(g_tstat_id,MODBUS_NIGHT_HEATING_DEADBAND,nValue);
        if (ret>0)
        {
            product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]=nValue;
        }
        else
        {
            AfxMessageBox(_T("Fail"));
        }


        CString strText;
        strText.Format(_T("%0.1f"),(float)(product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10.0));
        m_nightheating.SetWindowText(strText);


    }

}

void CParameterDlg::OnEnKillfocusEnigntcooling1()
{

    if(g_ParamLevel==1)
        return;
    CString strText;
    m_nightcooling.GetWindowText(strText);
    int nValue= (int)(_wtof(strText)*10);

    if(m_application_ctrl.GetCurSel()==0)
    {
        if(product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]==(int)(nValue))	//Add this to judge weather this value need to change.
            return;

        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_COOLING_SETPOINT,(int)(nValue),
                            product_register_value[MODBUS_NIGHT_COOLING_SETPOINT],this->m_hWnd,IDC_ENIGNTCOOLING1,_T("NIGHT COOLING SETPOINT"));
    }
    else
    {
        if((int)(product_register_value[MODBUS_NIGHT_COOLING_DEADBAND])==nValue)	//Add this to judge weather this value need to change.
            return;

        /*Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_COOLING_DEADBAND,nValue,
        product_register_value[MODBUS_NIGHT_COOLING_DEADBAND],
        this->m_hWnd,IDC_ENIGNTCOOLING1,_T("NIGHT COOLING DEADBAND"));*/
        int ret=write_one(g_tstat_id,MODBUS_NIGHT_COOLING_DEADBAND,nValue);
        if (ret>0)
        {
            product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]=nValue;
        }
        strText.Format(_T("%0.1f"),(float)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10.0);
        m_nightcooling.SetWindowText(strText);
    }
}

BOOL CParameterDlg::PreTranslateMessage(MSG* pMsg)
{
    
    if(pMsg->message == WM_KEYDOWN  )
    {
        if(pMsg->wParam == VK_RETURN)
        {
            CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
            GetDlgItem(IDC_EDIT_TEST)->SetFocus();
            temp_focus->SetFocus();
            return 1;
        }
    }

    return CDialog::PreTranslateMessage(pMsg);
}

void CParameterDlg::OnEnKillfocusSetvalue1()
{
//修改这个因为 用消息 一直在消息队列中，故而改成这种方式
    if(g_ParamLevel==1)
        return;
    CString strText;
    m_pid_setptEdt1.GetWindowText(strText);
    strText.Trim();
    if(strText.IsEmpty())
        return;
    float fValue=(float)_wtoi(strText);


    g_bPauseMultiRead = TRUE;
    if ((product_register_value[7]==PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT5i)||(product_register_value[7] == PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {
        /*Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,345,short(fValue*10),
        	product_register_value[345],this->m_hWnd,IDC_SETVALUE1,_T("SETPOINT"));*/
        if (product_register_value[345]!=short(fValue*10))
        {
            int ret=write_one(g_tstat_id,345,short(fValue*10));
            if (ret>0)
            {
                product_register_value[345]=short(fValue*10);
            }
            else
            {
                AfxMessageBox(_T("Fail!"));
            }
            strText.Format(_T("%0.1f"),(((float)product_register_value[345])/10));
        }




    }
    else if(product_register_value[7] == PM_TSTAT5G || product_register_value[7] == PM_TSTAT5E||product_register_value[7] == PM_PM5E||(product_register_value[7]==PM_TSTATRUNAR))  // 只有5E使用135
    {
        //short nVal = short(fValue);
        /*Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,135,short(fValue),
        	product_register_value[380],this->m_hWnd,IDC_SETVALUE1,_T("SETPOINT"));*/
        if (product_register_value[135]!=short(fValue))
        {
            int ret=write_one(g_tstat_id, 135, short(fValue));
            if (ret>0)
            {
                product_register_value[135]=short(fValue);
            }
            else
            {
                AfxMessageBox(_T("Fail!"));
            }
            strText.Format(_T("%0.1f"),(float)(product_register_value[135]));
        }

    }
    else
    {
        /*	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,374,short(fValue*10),
        		product_register_value[380],this->m_hWnd,IDC_SETVALUE1,_T("SETPOINT"));*/
        if (product_register_value[374]!=short(fValue*10))
        {
            int ret=write_one(g_tstat_id, 374,short(fValue*10));
            if (ret>0)
            {
                product_register_value[374]=short(fValue*10);
            }
            else
            {
                AfxMessageBox(_T("Fail!"));
            }
            strText.Format(_T("%0.1f"),((float)product_register_value[374]/10));
        }

    }
    m_pid_setptEdt1.SetWindowText(strText);
    g_bPauseMultiRead = FALSE;
}

//Recode by Fance , support T6 T7  2013 04 15
void CParameterDlg::OnCbnSelchangeOccupiedmodecombo()
{
    if(g_ParamLevel==1)
        return;
    int nItem=m_occupiedSetPointModeCmbox.GetCurSel();


    if(product_register_value[MODBUS_SETPOINT_CONTROL]==nItem)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_SETPOINT_CONTROL,nItem,
                        product_register_value[MODBUS_SETPOINT_CONTROL],this->m_hWnd,IDC_OCCUPIEDMODECOMBO,_T("SETPOINT CONTROL"));
}

//Annul by Fance ,this code not support T5T6
//void CParameterDlg::OnCbnSelchangeOccupiedmodecombo()
//{
//	if(g_ParamLevel==1)
//		return;
//	int nItem=m_occupiedSetPointModeCmbox.GetCurSel();
//
//			g_bPauseMultiRead = TRUE;
//	write_one(g_tstat_id, 339,nItem);
//
//	if(product_register_value[339]==0||product_register_value[339]==2)
//	{
//		m_defSetPointEdit.EnableWindow(FALSE);
//	}
//	if(product_register_value[339]==1)
//	{
//		m_defSetPointEdit.EnableWindow(TRUE);
//	}
//	g_bPauseMultiRead = FALSE;
//	//if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))
//	//	Refresh6();
//	//else
//	//	Refresh();
//	Reflesh_ParameterDlg();
//}
//Modify by Fance 这里原来的代码400只支持 5E 不支持T6 所以 改为如下
void CParameterDlg::OnCbnSelchangeComboLcdscrn1()
{
    CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN1);
    int nSel = pCbx->GetCurSel();

    if(product_register_value[MODBUS_LCD_SCREEN1]==nSel)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_LCD_SCREEN1,nSel,
                        product_register_value[MODBUS_LCD_SCREEN1],this->m_hWnd,IDC_COMBO_LCDSCRN1,_T("LCD_SCREEN1"));

}

void CParameterDlg::OnCbnSelchangeComboLcdscrn2()
{
    CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN2);
    int nSel = pCbx->GetCurSel();

    if(product_register_value[MODBUS_LCD_SCREEN2]==nSel)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_LCD_SCREEN2,nSel,
                        product_register_value[MODBUS_LCD_SCREEN2],this->m_hWnd,IDC_COMBO_LCDSCRN2,_T("LCD SCREEN2"));

}

//Recode by Fance,Also Support T6 T7
void CParameterDlg::OnCbnSelchangeComboRounddis()
{
    CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_ROUNDDIS);
    int nCursel = pCbx->GetCurSel();

    bool write_error_flag = false;
    g_bPauseMultiRead = TRUE;
    int round_display = MODBUS_ROUND_DISPLAY;//318  405

    if(product_register_value[MODBUS_ROUND_DISPLAY]==nCursel)	//Add this to judge weather this value need to change.
        return;



    if(nCursel == 0)
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_ROUND_DISPLAY,0,
                            product_register_value[MODBUS_ROUND_DISPLAY],this->m_hWnd,IDC_COMBO_ROUNDDIS,_T("ROUND_DISPLAY"));
    }
    else if(nCursel == 1)
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_ROUND_DISPLAY,1,
                            product_register_value[MODBUS_ROUND_DISPLAY],this->m_hWnd,IDC_COMBO_ROUNDDIS,_T("ROUND_DISPLAY"));
    }
    else if(nCursel == 2)
    {
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_ROUND_DISPLAY,5,
                            product_register_value[MODBUS_ROUND_DISPLAY],this->m_hWnd,IDC_COMBO_ROUNDDIS,_T("ROUND_DISPLAY"));
    }
}


//Annul by Fance, this function don't support T6 T7. Need recode.
//void CParameterDlg::OnCbnSelchangeComboRounddis()
//{
//	CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_ROUNDDIS);
//	int nCursel = pCbx->GetCurSel();
//
//			g_bPauseMultiRead = TRUE;
//	if(nCursel == 0)
//	{
//		write_one(g_tstat_id, 318, 0);
//	}
//	else if(nCursel == 1)
//	{
//		write_one(g_tstat_id, 318, 1);
//	}
//	else if(nCursel == 2)
//	{
//		write_one(g_tstat_id, 318, 5);
//	}
//
//	g_bPauseMultiRead = FALSE;
//}


void CParameterDlg::OnCbnKillfocusCombo1()
{
    
    //CString str;

    //GetDlgItem(IDC_COMBO1)->GetWindowText(str);

    //if (str.CompareNoCase(_T("Single")) == 0)
    //	MDAY = 0;
    //else
    //	MDAY = 1;

//	FlexSP = 1;
}

void CParameterDlg::OnCbnKillfocusCombo4()
{
    
//CString str;

    //GetDlgItem(IDC_COMBO4)->GetWindowText(str);

//	if (str.CompareNoCase(_T("Single")) == 0)
    //	MNIGHT = 0;
    //else
//		MNIGHT = 1;
//FlexSPN = 1;

}


//-------------------------------------------------------
//Annul by Fance ,it not support t6
//void CParameterDlg::OnEnKillfocusEditCdbnn()
//{
//	BeginWaitCursor();
//	UpdateData();
//	write_one(g_tstat_id,353,m_cooldbN*10);
//	newtstat6[353] = m_cooldbN*10;
//	EndWaitCursor();
//}

CString CParameterDlg::GetInputValue(int InputNo) //这个是行号，如果是Input的话，要在这个基础上加1
{
//所以InputNO=InputNo+1;
    InputNo+=1;
    float fValue;
    short nValue;
    CString strTemp;
    CString strValueUnit=GetTempUnit(product_register_value[MODBUS_ANALOG1_RANGE+InputNo-2], InputNo-1); //5e=359   122
    {
        if(product_register_value[MODBUS_ANALOG1_RANGE+InputNo-2]==1)	//359  122
        {
            fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+InputNo-2]/10.0);	//367   131
            strTemp.Format(_T("%0.1f"),fValue);

            strTemp +=strValueUnit;
        }
        else if(product_register_value[MODBUS_ANALOG1_RANGE+InputNo-2]==3 || product_register_value[MODBUS_ANALOG1_RANGE+InputNo-2]==5) // On/Off or Off/On ==1 On ==0 Off   359  122
        {
            int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+InputNo-2]); //367  131
            if (nValue == 0)
            {
                strTemp = _T("Off");
            }
            else
            {
                strTemp = _T("On");
            }
        }
        else if (product_register_value[MODBUS_ANALOG1_RANGE+InputNo-2]==4 )  // custom sensor	359 122
        {
            fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+InputNo-2]/10.0);	//367  131
            strTemp.Format(_T("%0.1f"), (float)fValue/10.0);
            strTemp +=strValueUnit;
        }
        else if(product_register_value[MODBUS_ANALOG1_RANGE+InputNo-2]==2)	//359 122
        {
            nValue=product_register_value[MODBUS_ANALOG_INPUT1+InputNo-2];		//367  131
            strTemp.Format(_T("%0.1f%%"),  (float)nValue);
        }
        else
        {
            strTemp.Format(_T("%d"),product_register_value[MODBUS_ANALOG_INPUT1+InputNo-2]);//lsc
            //	strTemp.Format(_T("UNUSED"));

        }
        return strTemp;
    }



}

//Add by Fance ,use this function to replace the  Reflash() and Reflash6();
void CParameterDlg::Reflesh_ParameterDlg()
{
    if (product_type ==T3000_6_ADDRESS)
    {
        GetDlgItem(IDC_STATIC_CUSTOMSENSORTABLE)->ShowWindow(FALSE);
        GetDlgItem(IDC_CS_1)->ShowWindow(FALSE);
        GetDlgItem(IDC_CS_2)->ShowWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDC_STATIC_CUSTOMSENSORTABLE)->ShowWindow(TRUE);
        GetDlgItem(IDC_CS_1)->ShowWindow(TRUE);
        GetDlgItem(IDC_CS_2)->ShowWindow(TRUE);
    }


    if(product_register_value[MODBUS_DEGC_OR_F] == 1)	//t5= 121;t6=104
    {
        g_unint = FALSE;
        GetDlgItem(IDC_STATICUNINT2)->SetWindowText(_T("°F"));
    }
    else
    {
        g_unint = TRUE;
        GetDlgItem(IDC_STATICUNINT2)->SetWindowText(_T("°C"));
    }

#if 1
    if (product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT8||product_register_value[7]==PM_TSTAT5E||product_register_value[7] == PM_PM5E||(product_register_value[7]==PM_TSTATRUNAR)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {
        GetDlgItem(IDC_TRENDLOGVIEW)->SetWindowText(_T("LCD"));
    }
    else
    {
        GetDlgItem(IDC_TRENDLOGVIEW)->SetWindowText(_T("LED"));
    }
#endif


    CString strTemp;
    strTemp.Format(_T("%d"),g_tstat_id);
   // m_idAdressEdit.SetWindowText(strTemp);
    CString strUnit=GetTempUnit();//Unit string.

    //185	110	1	Low byte	W/R(Reboot after write)	Bau - Baudrate, 0=9600, 1=19.2kbaud
    if(product_register_value[MODBUS_BAUDRATE]>=0&&product_register_value[MODBUS_BAUDRATE]<=4)	//110
        m_baudRateCombox.SetCurSel(product_register_value[MODBUS_BAUDRATE]);//110 //Modify by Fance
    else
        m_baudRateCombox.SetCurSel(0);



    strTemp.Format(_T("%d"),product_register_value[242]);
    GetDlgItem(IDC_EDIT_CHANGE_OVER_TIME)->SetWindowText(strTemp);




    int nItem;
    nItem = product_register_value[MODBUS_KEYPAD_SELECT];//395
    switch(nItem)
    {
    case 0:
        nItem=0;
        break;
    case 1:
        nItem=2;
        break;
    case 2:
        nItem=4;
        break;
    case 3:
        nItem=5;
        break;
    case 4:
        nItem=3;
        break;
    case 5:
        nItem=1;
        break;
    case 6:
        nItem=6;
        break;
    case 7:
        nItem=7;
        break;
    case 8:
        nItem=8;
        break;
    default :
        nItem=0;
        break;
    }
    m_keySelectCombox.SetCurSel(nItem);

    //127	106	1	Low byte	W/R	"POWER UP_MODE, mode of operation on power up. 0 = power off,
    //1 = power up in on mode, 2 = last value (default), 3 =  auto mode."
    m_powerupModelCombox.SetCurSel(product_register_value[MODBUS_POWERUP_MODE]);//127  T6=106

    //285	244	1	Low byte	R	Valve percent. Show the valve opened how much percent.  READ ONLY
    strTemp.Format(_T("%d"),product_register_value[MODBUS_VALVE_PERCENT]);	//285  T6=244
    m_value_percentEdit.SetWindowText(strTemp);



    // 	203	397	1	Low byte	W/R	"dIS ¨C Display.  This sets the display to either room temperature or setpoint.  0 = room temp,
    // 		1 = setpoint, 2 = Blank Display,3 = PID2 value,4 = PID2 setpoint,
    // 		5 = set segment code by manually, 6 = Display sleep"

    //m_displayCombox.SetCurSel(product_register_value[203]);
    m_displayCombox.SetCurSel(product_register_value[MODBUS_DISPLAY]);	//203 397 MODBUS_DISPLAY

    //	133	396	1	Low byte	W/R	"SPECIAL_MENU_LOCK, Special menu lockout via keypad, serial port only,
    m_keyLockCombox.SetCurSel(product_register_value[MODBUS_SPECIAL_MENU_LOCK]);		//133 396


    //129	107	1	Low byte	W/R	"AUTO_ONLY , enables or disables manual mode. 0 = Manual Fan Modes 1-x Allowed
    //m_autoOnlyCombox.SetCurSel(product_register_value[MODBUS_AUTO_ONLY]);		//129 107
    //118	103	1	Low byte	W/R(Reboot after write)	SEQUENCE , 0 = internal test sequence, outputs slowly cycle on/off or ramp up & down. 1 = normal, operation according to the output tables.
    m_SequenceCombox.SetCurSel(product_register_value[MODBUS_SEQUENCE]);	//118 103



    //279	243	1	Low byte	W/R	Valve travel time. The time of the valve travel  from one end to another end. The units is second.
    strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_VALVE_TRAVEL_TIME]/10.0);	//279 T6=243
   

    //214	113	1	Low byte	W/R(not to eeprom)
    m_hcChangeCombox.SetCurSel(product_register_value[MODBUS_HEAT_COOL_CONFIG]);	//214  T6=113

    //126	364	1	Low byte	W/R	POWERUP_SETPOINT , setpoint on power up
    strTemp.Format(_T("%d"),product_register_value[MODBUS_POWERUP_SETPOINT]);		//126  T6=364
    m_powerSetPointEdit.SetWindowText(strTemp);

    //293	373	1	Low byte	W/R	Setpoint increment on the display each time the user hits the up/down buttons. Units are 0.1Deg, 10 = 1Deg and so on.
    strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_SETPOINT_INCREASE]/10.0);		//293  373
    m_setpointIncreasement.SetWindowText(strTemp);


    //	338	341	1	Low byte	W/R	Default occupied setpoint. Works in concert with the "occupied setpoint control register", register 339
    strTemp.Format(_T("%d"),product_register_value[MODBUS_DEFAULT_SETPOINT]);	//338 341
    m_defSetPointEdit.SetWindowText(strTemp);

    //339	342	1	Low byte	W/R	Occupied Setpoint Control Register: 0 = normal, setpoint is managed by the serial port and keypad, the stat will remember the last occupied setpoint and use that during the next occupied period. 1 = Default mode, the last occupied setpoint if forgotten and the occupied setpoint gets reset to the default. 2 = trigger an event, when a master controller writes 2 to this register, the default setpoint will be copied to the occupied setpoint after which the Tstat will set the value back to 1 to show the event has been serviced.

    if(product_register_value[MODBUS_SETPOINT_CONTROL]>=0&&product_register_value[MODBUS_SETPOINT_CONTROL]<3)	//339 342
    {
        m_occupiedSetPointModeCmbox.SetCurSel(product_register_value[MODBUS_SETPOINT_CONTROL]);//339 T6=342
    }

    if(product_register_value[MODBUS_SETPOINT_CONTROL]==0||product_register_value[MODBUS_SETPOINT_CONTROL]==2)//342
    {
        m_defSetPointEdit.EnableWindow(FALSE);
    }
    //if(product_register_value[339]==1)
    if(product_register_value[MODBUS_SETPOINT_CONTROL]==1)//339  T6=342
    {
        m_defSetPointEdit.EnableWindow(TRUE);
    }



    //213	142	1	Low byte	W/R	Temperature sensor filter, FIL, weighted average of stored value to new raw value
    //	strTemp.Format(_T("%d"),product_register_value[213]);
    strTemp.Format(_T("%d"),product_register_value[MODBUS_FILTER]);	//213  T6=142
    m_inputFilterEdit.SetWindowText(strTemp);


    //201	241	1	Low byte	W/R	"MODBUS_CYCLING_DELAY ¨C delay time (in minutes) for switching out of heating or cooling
    //strTemp.Format(_T("%d"),product_register_value[201]);
    strTemp.Format(_T("%d"),product_register_value[MODBUS_CYCLING_DELAY]);	//201  T6=241
    m_cycledlayEdit.SetWindowText(strTemp);

    //301	114	2	Full	W/R	Period timer ON time.
    //strTemp.Format(_T("%d"),product_register_value[301]);
    strTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_ON]);	//301   T6=114
    m_timerOnEdit.SetWindowText(strTemp);

    //302	115	2	Full	W/R	Period timer OFF time.
    //strTemp.Format(_T("%d"),product_register_value[302]);

    strTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_OFF]);	//302   115
    m_timerOffEdit.SetWindowText(strTemp);

    //303	116	1	Low byte	W/R(not to eeprom)	Period timer units. 0, second; 1, minute; 2, hour.
    m_uniteCombox.SetCurSel(product_register_value[MODBUS_TIMER_UNITS]);	//303    116

    if(product_type!=3)	//TSTAT6 7 does not have this parameter
    {
        strTemp.Format(_T("%d"),product_register_value[MODBUS_ROTATION_TIME_LEFT]);//tstat6,7Ã»ÕÒµ½¡£	333
        m_timerLeft.SetWindowText(strTemp);

    }
    else
    {
        if(product_register_value[MODBUS_MODE_OUTPUT4]==2)	//283  205
        {
            CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMERON_STATIC);
            ptimerout->SetWindowText(_T("Out4:"));
        }
        else
        {
            CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMERON_STATIC);
            ptimerout->SetWindowText(_T("Timer On:"));
        }

        //if(product_register_value[284]==2)
        if(product_register_value[MODBUS_MODE_OUTPUT5]==2)	//284,206
        {

            CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMEOFF_STATIC);
            ptimerout->SetWindowText(_T("Out5:"));

        }
        else
        {
            CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMEOFF_STATIC);
            ptimerout->SetWindowText(_T("Timer Off:"));
        }
    }




    //283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM
    //284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM

    //if(product_register_value[283]==2||product_register_value[284]==2)
    if(product_register_value[MODBUS_MODE_OUTPUT4]==2||product_register_value[MODBUS_MODE_OUTPUT5]==2)//283 284	t6->//205  206
    {
        CComboBox* pCombox=(CComboBox*)GetDlgItem(IDC_TIMERDELECTCOMBO);
        int	indext=3;
        //327	408	1	Low byte	W/R	"Assign the timer to be used for which feature. 0 = period timer, 1 = rotation timer,
        //409 Fan
        write_one(g_tstat_id,MODBUS_TIMER_SELECT,indext); //327 408  *_*
        pCombox->SetCurSel(3);
    }




    //327	408	1	Low byte	W/R	"Assign the timer to be used for which feature. 0 = period timer, 1 = rotation timer,
    //409
    //m_timerSelectCombox.SetCurSel(product_register_value[327]);
    int sel=product_register_value[MODBUS_TIMER_SELECT];
    if (sel!=0)
    {
        --sel;
    }
    m_timerSelectCombox.SetCurSel(sel);	//327  408->409才对


    //211	111	1	Low byte	W/R	Unoccupied Override Timer, Ort. 0=disabled, >0=number of minutes manual override is allowed
    //212	112	1	Low byte	W/R	"OVERRIDE_TIMER_DOWN_COUNT - Number of minutes remaining on the timer when

    //strTemp.Format(_T("%d"),product_register_value[212]);
    strTemp.Format(_T("%d"),product_register_value[MODBUS_OVERRIDE_TIMER_LEFT]);	//112
    m_TimeEdit.SetWindowText(strTemp);
    strTemp.Format(_T("%d"),product_register_value[MODBUS_OVERRIDE_TIMER]);	//111
    m_OverRideEdit.SetWindowText(strTemp);
 
    sel =product_register_value[MODBUS_TEMP_SELECT]-2;
    if (sel<0)
    {
        sel =0;
    }

   

	if (product_type == T3000_5EH_LCD_ADDRESS)
	{
		sel +=1;
	}

    sel =product_register_value[MODBUS_INPUT1_SELECT]-2;
    if (sel<0)
    {
        sel =0;
    }
    m_inputSelect2.SetCurSel(sel);



    strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);	//121
    m_inputvalue1.SetWindowText(strTemp+strUnit);


    if ((product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7)||(product_register_value[7]==PM_TSTAT5i)||(product_register_value[7]==PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {

        m_combox_pir_endisable.ResetContent();
        m_combox_pir_endisable.AddString(_T("Enable"));
        m_combox_pir_endisable.AddString(_T("Disable"));


#if 1


        if(product_register_value[382]>=5&&product_register_value[382]<=14) // input1
        {

            strTemp=m_tstat_input_data.at(product_register_value[382]-5 + 1).Value.StrValue + m_tstat_input_data.at(product_register_value[382]-5 + 1).Unit.StrValue;
            m_inputvalue1.SetWindowText(strTemp);
        }
        //else if(product_register_value[382]==6) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(2);
        //	m_inputvalue1.SetWindowText(strTemp);
        //}
        //else if(product_register_value[382]==7) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(3);
        //	m_inputvalue1.SetWindowText(strTemp);
        //}
        //else if(product_register_value[382]==8) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(4);
        //	m_inputvalue1.SetWindowText(strTemp);
        //}
        //else if(product_register_value[382]==9) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(5);
        //	m_inputvalue1.SetWindowText(strTemp);
        //}
        //else if(product_register_value[382]==10) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(6);
        //	m_inputvalue1.SetWindowText(strTemp);
        //}
        //else if(product_register_value[382]==11) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(7);
        //	m_inputvalue1.SetWindowText(strTemp);
        //}
        //else if(product_register_value[382]==12) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(8);
        //	m_inputvalue1.SetWindowText(strTemp);
        //}
        //else if (product_register_value[382]==13)//Humidity
        //{		CString temp;
        //strUnit=_T("%");
        //if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
        //{

        //	temp.Format(_T("%0.1f"),(float)(product_register_value[MODBUS_TSTAT6_HUM_AVALUE]/10));

        //}
        //else
        //{
        //	temp.Format(_T("%0.1f"),(float)(product_register_value[MODBUS_TSTAT6_HUM_MVALUE]/10));
        //}


        //m_inputvalue1.SetWindowText(temp+strUnit);


        //}
        //else if (product_register_value[382]==14)//Co2
        //{ CString temp;
        //strUnit=_T("ppm");
        //if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
        //{

        //	temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_AVALUE]);
        //	temp=temp+strUnit;

        //}
        //else
        //{

        //	temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_MVALUE]);
        //	temp=temp+strUnit;
        //}

        //m_inputvalue1.SetWindowText(temp);


        //}
        else
        {
            //m_inputvalue1.SetWindowText(_T("UNUSED"));
            int ret=read_one(g_tstat_id,MODBUS_TEMPRATURE_CHIP,5);
            if (ret>0)
            {
                product_register_value[MODBUS_TEMPRATURE_CHIP]=ret;
            }
            strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);	//121
            m_inputvalue1.SetWindowText(strTemp+strUnit);
        }


        // float fValue;
        int nValue=0;
        CString strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG1_RANGE+product_register_value[383]-1], product_register_value[383]);

        //if(product_register_value[383]==5) // input1
        //{
        //
        //	strTemp=GetInputValue(1);
        //	m_inputValue2.SetWindowText(strTemp);
        //}
        //else if(product_register_value[383]==6) // input2 //m_inputvalue1
        //{
        //
        //	strTemp=GetInputValue(2);
        //	m_inputValue2.SetWindowText(strTemp);
        //}
        //else if(product_register_value[383]==7) // input2 //m_inputvalue1
        //{
        //
        //	strTemp=GetInputValue(3);
        //	m_inputValue2.SetWindowText(strTemp);
        //}
        //else if(product_register_value[383]==8) // input2 //m_inputvalue1
        //{
        //
        //	strTemp=GetInputValue(4);
        //	m_inputValue2.SetWindowText(strTemp);
        //}
        //else if(product_register_value[383]==9) // input2 //m_inputvalue1
        //{
        //
        //	strTemp=GetInputValue(5);
        //	m_inputValue2.SetWindowText(strTemp);
        //}
        //else if(product_register_value[383]==10) // input2 //m_inputvalue1
        //{
        //
        //	strTemp=GetInputValue(6);
        //	m_inputValue2.SetWindowText(strTemp);
        //}
        //else if(product_register_value[383]==11) // input2 //m_inputvalue1
        //{
        //
        //	strTemp=GetInputValue(7);
        //	m_inputValue2.SetWindowText(strTemp);
        //}
        //else if(product_register_value[383]==12) // input2 //m_inputvalue1
        //{
        //
        //	strTemp=GetInputValue(8);
        //	m_inputValue2.SetWindowText(strTemp);
        //}
        //else if (product_register_value[383]==13)//Humidity
        //{		CString temp;
        //strUnit=_T("%");
        //if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
        //{

        //	temp.Format(_T("%0.1f"),(float)(product_register_value[MODBUS_TSTAT6_HUM_AVALUE]/10));

        //}
        //else
        //{
        //	temp.Format(_T("%0.1f"),(float)(product_register_value[MODBUS_TSTAT6_HUM_MVALUE]/10));
        //}


        //m_inputValue2.SetWindowText(temp+strUnit);


        //}
        //else if (product_register_value[383]==14)//Co2
        //{ CString temp;
        //strUnit=_T("ppm");
        //if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
        //{

        //	temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_AVALUE]);
        //	temp=temp+strUnit;

        //}
        //else
        //{

        //	temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_MVALUE]);
        //	temp=temp+strUnit;
        //}

        //m_inputValue2.SetWindowText(temp);


        //}

		if (product_register_value[7] == PM_TSTAT5i
			|| product_register_value[7] == PM_TSTAT6
			|| product_register_value[7] == PM_TSTAT7
			|| product_register_value[7] == PM_TSTAT8
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
		{
			bitset<16>  value109(product_register_value[109]);
			int sel = value109[4];//第五位
			m_combox_mrd.SetCurSel(sel);
		}
		else
		{
			bitset<16>  value184(product_register_value[184]);
			int sel = value184[4];//第五位
			m_combox_mrd.SetCurSel(sel);
		}
        if(product_register_value[383]>=5&&product_register_value[383]<=14) // input1
        {

            strTemp=m_tstat_input_data.at(product_register_value[383]-5 + 1).Value.StrValue + m_tstat_input_data.at(product_register_value[383]-5 + 1).Unit.StrValue;
            m_inputValue2.SetWindowText(strTemp);
        }
        else
        {
            //m_inputValue2.SetWindowText(_T("UNUSED"));

            strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);	//121
            m_inputValue2.SetWindowText(strTemp+strUnit);

        }
        strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_UNIVERSAL_NIGHTSET]/10.0);			//275
        GetDlgItem(IDC_EDIT_PID2OFFSETPOINT)->SetWindowText(strTemp+strUnit);
#endif

    }
    else
    {
#if 1
        strTemp.Format(_T("%0.1f"),product_register_value[101]/10.0);
        m_inputvalue1.SetWindowText(strTemp+strUnit);
        float fValue;
        int nValue=0;
        CString strValueUnit=GetTempUnit(product_register_value[359], 1);
        if(product_register_value[241]==1) // input1
        {
            if(product_register_value[7]== PM_PRESSURE)  // pressure
            {
                strTemp.Format(_T("%0.1f W.C"),product_register_value[180]/100.0);
            }
            else if (product_register_value[7]==16)
            {
                if(product_register_value[359]==1)
                {
                    fValue=float(product_register_value[367]/10.0);
                    strTemp.Format(_T("%0.1f"),fValue);

                    strTemp +=strValueUnit;
                }
                else if(product_register_value[359]==3 || product_register_value[359]==5) // On/Off or Off/On ==1 On ==0 Off
                {
                    int nValue=(product_register_value[367]);
                    if (nValue == 1)
                    {
                        strTemp = _T("On");
                    }
                    else
                    {
                        strTemp = _T("Off");
                    }
                }
                else if (product_register_value[359]==4 )  // custom sensor
                {
                    fValue=float(product_register_value[367]/10.0);
                    strTemp.Format(_T("%0.1f"), (float)fValue/10.0);
                    strTemp +=strValueUnit;
                }
                else if(product_register_value[359]==2)
                {
                    nValue=product_register_value[367];
                    strTemp.Format(_T("%d%%"), nValue);
                }
                else
                {

                    strTemp=_T("UNUSED");
                }


            }
            else
            {
                if(product_register_value[MODBUS_ANALOG_IN1]==4||product_register_value[MODBUS_ANALOG_IN1]==1)
                {
                    strTemp.Format(_T("%0.1f"),(float)product_register_value[180]/10);
                    strTemp=strTemp+strUnit;
                }
                if (strUnit==""||strUnit=="%")
                {
                    strTemp.Format(_T("%d"),product_register_value[180]);
                    strTemp=strTemp+strUnit;
                }
                if(product_register_value[180]==3)
                {
                    if(product_register_value[180]==0)
                        strTemp=_T("OFF");
                    if(product_register_value[180]==1)
                        strTemp=_T("ON");
                }
            }
            m_inputValue2.SetWindowText(strTemp);
        }
        else if(product_register_value[241]==2) // input2 //m_inputvalue1
        {
            if (product_register_value[7]==16)//5E
            {
                if(product_register_value[360]==1)
                {
                    fValue=float(product_register_value[368]/10.0);
                    strTemp.Format(_T("%0.1f"),fValue);

                    strTemp +=strValueUnit;
                }
                else if(product_register_value[360]==3 || product_register_value[360]==5) // On/Off or Off/On ==1 On ==0 Off
                {
                    int nValue=(product_register_value[368]);
                    if (nValue == 1)
                    {
                        strTemp = _T("On");
                    }
                    else
                    {
                        strTemp = _T("Off");
                    }
                }
                else if (product_register_value[360]==4 )  // custom sensor
                {
                    fValue=float(product_register_value[368]/10.0);
                    strTemp.Format(_T("%0.1f"), (float)fValue/10.0);
                    strTemp +=strValueUnit;
                }
                else if(product_register_value[360]==2)
                {
                    nValue=product_register_value[368];
                    strTemp.Format(_T("%d%%"), nValue);
                    strTemp=strTemp+strUnit;
                }
                else
                {

                    strTemp=_T("UNUSED");
                }
            }

            m_inputValue2.SetWindowText(strTemp);
        }
        else
        {
            m_inputValue2.SetWindowText(_T("UNUSED"));
        }

        if (product_register_value[7]==PM_TSTAT5G)
        {
            if(product_register_value[241]==1)
            {
                strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN1], 1);//188
                if(product_register_value[MODBUS_ANALOG_IN1]==4||product_register_value[MODBUS_ANALOG_IN1]==1)//188
                {
                    strTemp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_0]/10);//180
                    strTemp=strTemp+strUnit;
                }
                if (product_register_value[MODBUS_ANALOG_IN1]==0||product_register_value[MODBUS_ANALOG_IN1]==2)//188
                {
                    //strUnit=GetTempUnit(180);
                    strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_0]);//180
                }
                if(product_register_value[MODBUS_ANALOG_IN1]==3)//188
                {
                    if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//180
                        strTemp=_T("ON");
                    if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//180
                        strTemp=_T("OFF");
                }


                if (product_register_value[MODBUS_ANALOG_IN1] == 0)//188
                {
                    strTemp = _T("UNUSED");
                }
            }
            else if (product_register_value[241]==2)
            {
                strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN2],2);
                strTemp.Empty();
                if(product_register_value[MODBUS_ANALOG_IN2]==4||product_register_value[MODBUS_ANALOG_IN2]==1)//189
                {

                    strTemp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_1]/10);	//181
                    strTemp=strTemp+strUnit;
                }
                if (product_register_value[MODBUS_ANALOG_IN2]==0||product_register_value[MODBUS_ANALOG_IN2]==2)//189
                {
                    strUnit=GetTempUnit(181);
                    strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_1]);//181
                    strTemp=strTemp+strUnit;
                }
                if(product_register_value[MODBUS_ANALOG_IN2]==3)//189
                {
                    if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
                        strTemp=_T("ON");
                    if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
                        strTemp=_T("OFF");
                }


                if (product_register_value[MODBUS_ANALOG_IN2] == 0)//189
                {
                    strTemp = _T("UNUSED");
                }

            }
            else
            {
                strTemp = _T("UNUSED");
            }


            m_inputValue2.SetWindowText(strTemp);

        }
        strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_UNIVERSAL_NIGHTSET]/10.0);			//275
        GetDlgItem(IDC_EDIT_PID2OFFSETPOINT)->SetWindowText(strTemp+strValueUnit);
#endif
    }






    float m_fFirmwareVersion=get_curtstat_version();//0912
    if (product_register_value[7] == PM_TSTAT5E||product_register_value[7] == PM_PM5E||(product_register_value[7]==PM_TSTATRUNAR))//0912
    {
        short nOccupied = product_register_value[184];  // Day setpoint option
        BOOL bOccupied = nOccupied & 0x0001;
        if (bOccupied)  // day  - Occupied
        {
            if(m_fFirmwareVersion >= 35.4)
            {
                m_application_ctrl.SetCurSel(product_register_value[423]);
            }
            else
            {
                m_application_ctrl.SetCurSel(product_register_value[454]);
                // 5E 以及以后的型号
            }

        }
        else
        {
            m_application_ctrl.SetCurSel(product_register_value[125]);

        }
    }
    else
    {
        m_application_ctrl.SetCurSel(product_register_value[125]);
    }


    UpdateCoolingandHeatingData();

    //UpdateCoolingandHeatingData();
    int index = 0;
    index = m_application_ctrl.GetCurSel();
    switch(index)
    {
    case 0:
        GetDlgItem(IDC_STATIC_CSP)->SetWindowText(_T("Cooling SetPoint"));
        GetDlgItem(IDC_STATIC_HSP)->SetWindowText(_T("Heating SetPoint"));
        break;
    case 1:
        GetDlgItem(IDC_STATIC_CSP)->SetWindowText(_T("Cooling DB"));
        GetDlgItem(IDC_STATIC_HSP)->SetWindowText(_T("Heating DB"));
        break;
    }


    strTemp.Empty();
    m_version=get_curtstat_version();



    strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN1], 1);//188
    strTemp.Empty();
    strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_UNIVERSAL_SET]/10.0);	 //246  359
    strTemp=strTemp+strUnit;
    m_pid_setptEdt2.SetWindowText(strTemp);

    strTemp.Format(_T("%d"),product_register_value[MODBUS_COOLING_PID]);		//104 384
    strTemp+=_T("%");
    m_pid_outputEdt1.SetWindowText(strTemp);
    strTemp.Format(_T("%d"),product_register_value[MODBUS_PID_UNIVERSAL]);		//270  389
    strTemp+=_T("%");
    m_pid_outputEdt2.SetWindowText(strTemp);


    //114	385	1	Low byte	R	COOLING PTERM , proportional term for PI calculation
    strTemp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_COOLING_PTERM]/10.0);	//114  385
    m_pternEdt1.SetWindowText(strTemp);

    strTemp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_UNIVERSAL_PTERM]/10.0);		//244  387
    m_ptermEdt2.SetWindowText(strTemp);

    strTemp.Format(_T("%u"), product_register_value[799]); //799  DTerm
    m_edit_dterm.SetWindowText(strTemp);

    strTemp.Format(_T("%u"), product_register_value[800]); //799  DTerm
    m_edit_sampling_interval.SetWindowText(strTemp);

    strTemp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_COOLING_ITERM]/10.0);	//115  386
    m_coolingPitemEdt1.SetWindowText(strTemp);

    strTemp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_UNIVERSAL_ITERM]/10.0);	//245  388
    m_pidPitemEdt2.SetWindowText(strTemp);

    strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN1], 1);//188

    strTemp.Empty();
    strTemp.Format(_T("%0.1f"),(float)(product_register_value[MODBUS_UNIVERSAL_SET]/10.0));	//246  359
    strTemp+=strUnit;

    m_dayOccEdt2.SetWindowText(strTemp);


    //下面有问题;
    //if (product_register_value[MODBUS_INPUT1_SELECT]==3) // humidity		//241
    //{
    //	strTemp.Format(_T("%0.1f%%"),product_register_value[422]/10.0);	//422 Don't know why???
    //	m_dayOccEdt2.SetWindowText(strTemp);
    //}

    strTemp.Empty();
    //上面可能有问题;

    if(product_type!=T3000_6_ADDRESS)
    {
        if(m_version<34.9 || product_register_value[7] == PM_TSTAT5E||product_register_value[7] == PM_PM5E||(product_register_value[7]==PM_TSTATRUNAR)||
                (product_register_value[7] == PM_TSTAT5G))   //
        {
            strTemp.Format(_T("%d"),(int)product_register_value[MODBUS_COOLING_SETPOINT]);//135
        }
        else
        {
            strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_TWO_BYTE_SETPOINT]/10.0);	// set point for TStat A,B,C,D	//374
        }

        m_dayOccEdt1.SetWindowText(strTemp+GetTempUnit());
        if (product_register_value[7] == PM_TSTAT5E||product_register_value[7] == PM_PM5E||(product_register_value[7]==PM_TSTATRUNAR)||
                (product_register_value[7] == PM_TSTAT5G))//0911
        {
            strUnit=GetTempUnit();
            strTemp.Format(_T("%d"),(int)product_register_value[MODBUS_COOLING_SETPOINT]);
            m_pid_setptEdt1.SetWindowText(strTemp+strUnit);
        }
        else
        {
            // unoccupied and office mode, we should recalc the setpoint here
            short nOccupied = product_register_value[184];  // Day setpoint option  //184


            BOOL bOccupied = nOccupied & 0x0001;
            if (!bOccupied && product_register_value[125] == 0)  // unoccupied and office mode	//125
            {
                float temp =float( (product_register_value[183]-product_register_value[182]) / 2.0 + product_register_value[182] );//183  182  182
                //strTemp.Format(_T("%0.1f"),temp);
                strTemp.Format(_T("%d"),(int)temp);
            }
            m_pid_setptEdt1.SetWindowText(strTemp+strUnit);
        }


    }
    else
    {
        strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_DAY_SETPOINT]/10.0);//345	set point
        strTemp+=strUnit;
        m_dayOccEdt1.SetWindowText(strTemp);
        float temp =float(product_register_value[MODBUS_DAY_SETPOINT]/10.0);
        strTemp+=strUnit;
        strTemp.Format(_T("%d"),(int)temp);
        m_pid_setptEdt1.SetWindowText(strTemp+strUnit);
    }


    //131	365	1	Low byte	W/R	MAX_SETPOINT, Setpoint high, the highest setpoint a user will be able to set from the keypad.
    //132	366	1	Low byte	W/R	MIN_SETPOINT, Setpoint Low, the lowest setpoint a user will be able to set from the keypad.
    strTemp.Format(_T("%d"),product_register_value[MODBUS_MAX_SETPOINT]);//131 365
    strTemp+=strUnit;
    m_setptHiEdit.SetWindowText(strTemp);

    strTemp.Empty();
    //strTemp.Format(_T("%d"),product_register_value[132]);
    strTemp.Format(_T("%d"),(short)product_register_value[MODBUS_MIN_SETPOINT]);//132  366
    strTemp+=strUnit;
    m_setptLoEdit.SetWindowText(strTemp);

    //119	346	1	Low byte	W/R	(Day)Occupied cooling setpoint dead band  , offset from setpoint for cooling to begin.  Units are 0.1 deg.
    //		347	1	Low byte	W/R	(Day)Occupied heating setpoint dead band  , offset from setpoint for heating to begin.  Units are 0.1 deg.
    strUnit=GetTempUnit();
    if(product_type!=T3000_6_ADDRESS)
    {
        strTemp.Empty();
        strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_COOLING_DEADBAND]/10.0);	//119
        strTemp+=strUnit;
        m_setptCDDEdt1.SetWindowText(strTemp);

        strTemp.Empty();
        strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_HEATING_DEADBAND]/10.0);	//120
        strTemp+=strUnit;
        m_HeadDEdt1.SetWindowText(strTemp);
    }
    else
    {
        strTemp.Format(_T("%0.1f"),((float)product_register_value[MODBUS_DAY_COOLING_DEADBAND])/10.0);
        strTemp+=strUnit;
        m_setptCDDEdt1.SetWindowText(strTemp);


        strTemp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_DAY_HEATING_DEADBAND]/10.0);
        strTemp+=strUnit;
        m_HeadDEdt1.SetWindowText(strTemp);

    }
    strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN1], 1);//188
    strTemp.Empty();
    strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_UNIVERSAL_DB_LO]/10.0);//243->361
    strTemp+=strUnit;
    m_setptCCDEdt2.SetWindowText(strTemp);

    strTemp.Empty();
    strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_UNIVERSAL_DB_HI]/10.0);// 242->360
    strTemp+=strUnit;
    m_HeadDEdt2.SetWindowText(strTemp);





    CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN1);
    pCbx->SetCurSel(product_register_value[MODBUS_LCD_SCREEN1]);
    pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN2);
    pCbx->SetCurSel(product_register_value[MODBUS_LCD_SCREEN2]);
    pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_ROUNDDIS);
    if(product_register_value[MODBUS_ROUND_DISPLAY] == 0)	//318->405
    {
        pCbx->SetCurSel(0);
    }
    else if(product_register_value[MODBUS_ROUND_DISPLAY] == 1)//318->405
    {
        pCbx->SetCurSel(1);
    }
    else if(product_register_value[MODBUS_ROUND_DISPLAY] == 5)//318->405
    {
        pCbx->SetCurSel(2);
    }





    if (MDAY == 0)
        GetDlgItem(IDC_COMBO1)->SetWindowText(_T("Single"));
    else
        GetDlgItem(IDC_COMBO1)->SetWindowText(_T("Dual"));

    if (MNIGHT == 0)
        GetDlgItem(IDC_COMBO4)->SetWindowText(_T("Single"));
    else
        GetDlgItem(IDC_COMBO4)->SetWindowText(_T("Dual"));

    CString str;//add by Fance. T5 also support this function
    str.Format(_T("%d"),product_register_value[MODBUS_VALVE_TRAVEL_TIME]);	//279  243
    GetDlgItem(IDC_EDIT_ValueTravelTime)->SetWindowText(str);
    str.Format(_T("%d"),product_register_value[MODBUS_DEAD_MASTER]);
    GetDlgItem(IDC_EDIT_DEAD_MASTER)->SetWindowText(str);
    //////////////////////////////////////////////////////////////////////////
    /*if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))*/
    if (product_register_value[714]==0x56)
    {
        GetDlgItem(IDC_STATIC_NAME_TSTAT)->ShowWindow(TRUE);
        GetDlgItem(IDC_EDIT_TSTAT_NAME)->ShowWindow(TRUE);
        CString TstatName;
        TstatName.Format(_T("%s%s"),GetTextFromReg(715),GetTextFromReg(719));
        GetDlgItem(IDC_EDIT_TSTAT_NAME)->SetWindowText(TstatName);
    }
    else
    {
        GetDlgItem(IDC_STATIC_NAME_TSTAT)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_TSTAT_NAME)->ShowWindow(SW_HIDE);
    }


    if (product_register_value[7] == PM_TSTAT8)
    {
        short temp_buffer[10] = { 0 };
        temp_buffer[0] = htons( product_register_value[753]) ;
        temp_buffer[1] = htons(product_register_value[754]) ;
        temp_buffer[2] = htons(product_register_value[755]);
        temp_buffer[3] = htons(product_register_value[756]);
        
        CString temp_Name;
        MultiByteToWideChar(CP_ACP, 0, (char *)temp_buffer,
            (int)strlen((char *)temp_buffer) + 1,
            temp_Name.GetBuffer(MAX_PATH), MAX_PATH);
        temp_Name.ReleaseBuffer();

        GetDlgItem(IDC_STATIC_DAYSETPOINT)->SetWindowTextW(temp_Name);
        GetDlgItem(IDC_STATIC_DAYN)->SetWindowTextW(temp_Name);


        short temp_buffer2[10] = { 0 };
        temp_buffer2[0] = htons(product_register_value[737]);
        temp_buffer2[1] = htons(product_register_value[738]);
        temp_buffer2[2] = htons(product_register_value[739]);
        temp_buffer2[3] = htons(product_register_value[740]);

        CString temp_Name2;
        MultiByteToWideChar(CP_ACP, 0, (char *)temp_buffer2,
            (int)strlen((char *)temp_buffer2) + 1,
            temp_Name2.GetBuffer(MAX_PATH), MAX_PATH);
        temp_Name2.ReleaseBuffer();

        GetDlgItem(IDC_STATIC_NIGHTN)->SetWindowTextW(temp_Name2);

    }

    if((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT8)||(product_register_value[7] == PM_TSTAT5i)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {


        /*GetDlgItem(IDC_SPSET1)->EnableWindow(FALSE);*/GetDlgItem(IDC_SPSET1)->EnableWindow(TRUE);
        /*GetDlgItem(IDC_SPSET2)->EnableWindow(FALSE);*/GetDlgItem(IDC_SPSET2)->EnableWindow(TRUE);
        /*GetDlgItem(IDC_ESETPOINTHI)->EnableWindow(FALSE);*/GetDlgItem(IDC_ESETPOINTHI)->EnableWindow(TRUE);
        /*GetDlgItem(IDC_ESETPOINTLO)->EnableWindow(FALSE);*/GetDlgItem(IDC_ESETPOINTLO)->EnableWindow(TRUE);
        /*GetDlgItem(IDC_ECOOLDEADBAND1)->EnableWindow(FALSE);*/GetDlgItem(IDC_ECOOLDEADBAND1)->EnableWindow(TRUE);
        /*GetDlgItem(IDC_ECOOLINGITERM1)->EnableWindow(FALSE);*/GetDlgItem(IDC_ECOOLINGITERM1)->EnableWindow(TRUE);

        /*GetDlgItem(IDC_ECOOLDEADBAND2)->EnableWindow(FALSE);*/GetDlgItem(IDC_ECOOLDEADBAND2)->EnableWindow(TRUE);
        /*GetDlgItem(IDC_ECOOLINGITERM2)->EnableWindow(FALSE);*/GetDlgItem(IDC_ECOOLINGITERM2)->EnableWindow(TRUE);
        GetDlgItem(IDC_EAPPLICATION)->EnableWindow(FALSE);
        GetDlgItem(IDC_ENIGNTCOOLING1)->EnableWindow(FALSE);//GetDlgItem(IDC_ENIGNTCOOLING1)->EnableWindow(TRUE);
        GetDlgItem(IDC_ENIGNTHEATING)->EnableWindow(FALSE);//GetDlgItem(IDC_ENIGNTHEATING)->EnableWindow(TRUE);





        GetDlgItem(IDC_EDIT_CSPD)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_CDBDN)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT31)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT34)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT37)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_CSPNN)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_CDBNN)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT32)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT35)->EnableWindow(TRUE);

        GetDlgItem(IDC_EDIT38)->EnableWindow(TRUE);
        GetDlgItem(IDC_COMBO4)->EnableWindow(TRUE);
      
 
        GetDlgItem(IDC_EDIT_ValueTravelTime)->EnableWindow(TRUE);

        //GetDlgItem(IDC_STATIC_1SP2SPN)->ShowWindow(FALSE);
        //GetDlgItem(IDC_COMBO1)->ShowWindow(FALSE);
        //GetDlgItem(IDC_COMBO4)->ShowWindow(FALSE);


        m_edit_backlighttime.ShowWindow(TRUE);
        CString backlight;
        backlight.Format(_T("%d"),product_register_value[434]);
        m_edit_backlighttime.SetWindowText(backlight);

// 		m_check_occupiedenable.ShowWindow(TRUE);
// 		if ((product_register_value[629]==1)&&(product_register_value[565]==0))
// 		{
// 			m_check_occupiedenable.SetCheck(1);
// 		}
// 		else
// 		{
// 			m_check_occupiedenable.SetCheck(0);
// 		}
// 		m_occupied_timer.ShowWindow(TRUE);
        //CString occupiedtimer;
        //occupiedtimer.Format(_T("%d"),product_register_value[641]);
        //m_occupied_timer.SetWindowText(occupiedtimer);


        GetDlgItem(IDC_STATIC_LCDSCREEN1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_LCDSCRN1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_LCDSCREEN2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_LCDSCRN2)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_DISPLAYCOMBO)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_DISPLAY)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_STATIC_PIR_SENSOR_SETTING)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_ENDIS)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_SENSITIVTY)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_SENSITIVTY)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_COMBO_ENABLE_DIS)->ShowWindow(SW_SHOW);
        if (product_register_value[629]==1&&product_register_value[565]==0)
        {
            m_combox_pir_endisable.SetCurSel(0);
        }
        else
        {
            m_combox_pir_endisable.SetCurSel(1);
        }
        /*	if (product_register_value[641]>=20&&product_register_value[641]<=100)
        	{*/
        strTemp.Format(_T("%d"),product_register_value[641]);
        /*}
        else
        {
        strTemp=_T("N/A");
        }*/

        m_sensitivity_editor.SetWindowTextW(strTemp);
    }
    else
    {


        GetDlgItem(IDC_STATIC_PIR_SENSOR_SETTING)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_ENDIS)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_SENSITIVTY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_SENSITIVTY)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO_ENABLE_DIS)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_STATIC_DISPLAY)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_DISPLAYCOMBO)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_LCDSCREEN1)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_COMBO_LCDSCRN1)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_LCDSCREEN2)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_COMBO_LCDSCRN2)->ShowWindow(SW_SHOW);

        m_edit_backlighttime.ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_CSPD)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_CDBDN)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT31)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT34)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT37)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_CSPNN)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_CDBNN)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT32)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT35)->EnableWindow(FALSE);

        GetDlgItem(IDC_EDIT38)->EnableWindow(FALSE);
        GetDlgItem(IDC_COMBO4)->EnableWindow(FALSE);
     
 



        GetDlgItem(IDC_SPSET1)->EnableWindow(TRUE);
        GetDlgItem(IDC_ESETPOINTHI)->EnableWindow(TRUE);
        GetDlgItem(IDC_ESETPOINTLO)->EnableWindow(TRUE);
        GetDlgItem(IDC_ECOOLDEADBAND1)->EnableWindow(TRUE);
        GetDlgItem(IDC_ECOOLINGITERM1)->EnableWindow(TRUE);
        GetDlgItem(IDC_SPSET2)->EnableWindow(TRUE);
        GetDlgItem(IDC_ECOOLDEADBAND2)->EnableWindow(TRUE);
        GetDlgItem(IDC_ECOOLINGITERM2)->EnableWindow(TRUE);
        GetDlgItem(IDC_EAPPLICATION)->EnableWindow(TRUE);
        GetDlgItem(IDC_ENIGNTCOOLING1)->EnableWindow(TRUE);
        GetDlgItem(IDC_ENIGNTHEATING)->EnableWindow(TRUE);
        /*GetDlgItem(IDC_EDIT_ValueTravelTime)->EnableWindow(FALSE);*/


    }

    // 		345	1	Low byte	W/R	(Day)Occupied   setpoint
    // 		346	1	Low byte	W/R	(Day)Occupied cooling setpoint dead band  , offset from setpoint for cooling to begin.  Units are 0.1 deg.
    // 		347	1	Low byte	W/R	(Day)Occupied heating setpoint dead band  , offset from setpoint for heating to begin.  Units are 0.1 deg.
    // 		348	2	Full	W/R	(Day)Occupied  cooling setpoint (day cooling setpoint)
    // 		349	2	Full	W/R	(Day)Occupied  heating setpoint (day heating setpoint)



    if (product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||product_register_value[7]==PM_TSTAT5i||(product_register_value[7] == PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {
        int TempSel=product_register_value[MODBUS_TEMP_SELECT];
        if (TempSel<2)
        {
            TempSel=2;
        }

        m_InputSelect1.SetCurSel(TempSel-2);


        TempSel=product_register_value[MODBUS_INPUT1_SELECT];


        if (TempSel<2)
        {
            TempSel=2;
        }
        m_inputSelect2.SetCurSel(TempSel-2);

        int RegAddress=-1;
        if (product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||(product_register_value[7] == PM_TSTAT8)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
        {
            RegAddress=650;
        }
        else
        {
            RegAddress=MODBUS_SUPPLY_TEMP_SELECT;
        }

        TempSel=product_register_value[RegAddress];
        if (TempSel<2)
        {
            TempSel=2;
        }

        m_inputSelect3.SetCurSel(TempSel-2);

        m_coolsp= ((float)(short)product_register_value[MODBUS_DAY_COOLING_SETPOINT])/10;//348

        m_cooldb=((float)(short)product_register_value[MODBUS_DAY_COOLING_DEADBAND])/10;//346
        m_setpoint =((float)(short)product_register_value[MODBUS_DAY_SETPOINT])/10;//345
        m_heatdb =((float)(short)product_register_value[MODBUS_DAY_HEATING_DEADBAND])/10;//347
        m_heatsp =((float)(short)product_register_value[MODBUS_DAY_HEATING_SETPOINT])/10;//349

        // 		350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
        // 		351	1	Low byte		spare
        // 		352	1	Low byte	W/R	(Night)Unoccupied heating setpoint dead band , heating deadband for the night (OFF) mode. Units of 1 deg.
        // 		353	1	Low byte	W/R	(Night)Unoccupied cooling setpoint dead band , cooling deadband for the night (OFF) mode. Units of 1 deg.
        // 		354	2	Full	W/R	(Night)Unoccupied heating setpoint
        // 		355	2	Full	W/R	(Night)Unoccupied cooling setpoint
        m_coolspN=((float)(short)product_register_value[MODBUS_NIGHT_COOLING_SETPOINT])/10;//355
        m_cooldbN=((float)(short)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND])/10;//353
        m_setpointN=((float)(short)product_register_value[MODBUS_NIGHT_SETPOINT])/10;//350
        m_heatdbN=((float)(short)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND])/10;//352
        m_heatspN=((float)(short)product_register_value[MODBUS_NIGHT_HEATING_SETPOINT])/10;
    }
    else if ((product_register_value[7] == PM_PM5E) || (product_register_value[7] == PM_TSTAT5E))
    {
        //Fandu add 2018 01 08;
        int TempSel = product_register_value[MODBUS_TEMP_SELECT];

        if (TempSel == 1)
        {
            m_InputSelect1.SetWindowText(INPUT_SETTING[3]);
        }
        else if (TempSel == 2)
        {
            m_InputSelect1.SetWindowText(INPUT_SETTING[0]);
        }
        else if (TempSel == 3)
        {
            m_InputSelect1.SetWindowText(INPUT_SETTING[1]);
        }
    }
    //CenterWindow(this);
    if (product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||product_register_value[7]==PM_TSTAT5i||(product_register_value[7] == PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {
        //WINDOWPLACEMENT wp;
        //GetWindowPlacement(&wp);
        //wp.rcNormalPosition.bottom += 120;
        //SetWindowPlacement(&wp);
        //GetDlgItem(IDC_STATIC_SEPERATOR)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_STATIC_1SP2SPN)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_STATIC_COOLDBN)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_STATIC_SPN)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_STATIC_HEATDBN)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_STATIC_HEATSPN)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_STATIC_COOLSPN)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_COMBO1)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_STATIC_DAYN)->ShowWindow(SW_NORMAL);

        GetDlgItem(IDC_EDIT_CDBDN)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_EDIT31)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_EDIT34)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_EDIT37)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_EDIT_CSPD)->ShowWindow(SW_NORMAL);

        GetDlgItem(IDC_STATIC_NIGHTN)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_COMBO4)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_EDIT_CDBNN)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_EDIT32)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_EDIT35)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_EDIT38)->ShowWindow(SW_NORMAL);
        GetDlgItem(IDC_EDIT_CSPNN)->ShowWindow(SW_NORMAL);

        GetDlgItem(IDC_STATIC_SETPOINTN)->ShowWindow(SW_NORMAL);

    }
    else
    {

        //WINDOWPLACEMENT wp;
        //GetWindowPlacement(&wp);
        //CRect rc;
        //CWnd* pWnd = GetDlgItem(IDC_STATIC_SEPERATOR);
        //pWnd->GetWindowRect(&rc);
        ////ScreenToClient(&rc);
        //wp.rcNormalPosition.bottom = rc.bottom - 10;
        //SetWindowPlacement(&wp);
        //GetDlgItem(IDC_STATIC_SEPERATOR)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_STATIC_1SP2SPN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_COOLDBN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_SPN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_HEATDBN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_HEATSPN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_COOLSPN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_DAYN)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_EDIT_CDBDN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT31)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT34)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT37)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CSPD)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_STATIC_NIGHTN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_COMBO4)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CDBNN)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT32)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT35)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT38)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_CSPNN)->ShowWindow(SW_HIDE);

        GetDlgItem(IDC_STATIC_SETPOINTN)->ShowWindow(SW_HIDE);

    }
    ShowPID3();

    strTemp.Format(_T("%d"),product_register_value[726]);
    GetDlgItem(IDC_EDIT_TRANDUCER_MIN)->SetWindowText(strTemp);

    strTemp.Format(_T("%d"),product_register_value[727]);
    GetDlgItem(IDC_EDIT_TRANDUCER_MAX)->SetWindowText(strTemp);

    if(product_register_value[MODBUS_SCHEDULE_ON_OFF] == 1)
    {
        m_powerupModelCombox.SetWindowTextW(L"Schedule");
        m_powerupModelCombox.EnableWindow(FALSE);

    }
    else
    {

        m_powerupModelCombox.EnableWindow(TRUE);
    }


}

void CParameterDlg::OnBnClickedCancel()
{
    
// 	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
// 	CView* pT3000View = pFrame->m_pViews[DLG_T3000_VIEW];
// 	if(pFrame->m_pViews[DLG_T3000_VIEW]->m_hWnd!=NULL)
// 	{
// 		::PostMessage(pFrame->m_pViews[DLG_T3000_VIEW]->m_hWnd,MsgT3000ViewFresh,0,0);
// 	}

    OnCancel();
}

void CParameterDlg::OnEnKillfocusEditValuetraveltime()//0914
{


    CString strTemp;
    //m_value_percentEdit.GetWindowText(strTemp);
    GetDlgItem(IDC_EDIT_ValueTravelTime)->GetWindowText(strTemp);
    if(strTemp.IsEmpty())
        return;
    int ret = _wtoi(strTemp);
    if (ret >255)
    {
        ret = 255;
    }


    if(product_register_value[MODBUS_VALVE_TRAVEL_TIME]==ret)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_VALVE_TRAVEL_TIME,ret,
                        product_register_value[MODBUS_VALVE_TRAVEL_TIME],this->m_hWnd,IDC_EDIT_ValueTravelTime,_T("VALVE TRAVEL TIME"));
}

void CParameterDlg::OnEnKillfocusEditPid2offsetpoint()
{

    CString str;
    //str.Format(_T("%d"),product_register_value[275]);
    GetDlgItem(IDC_EDIT_PID2OFFSETPOINT)->GetWindowText(str);
    float nValue= _wtof(str);

    if(product_register_value[MODBUS_UNIVERSAL_NIGHTSET]==(int)nValue*10)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_UNIVERSAL_NIGHTSET,(int)nValue*10,
                        product_register_value[275],this->m_hWnd,IDC_EDIT_PID2OFFSETPOINT,_T("Pid2 off setpoint"));
}

LRESULT  CParameterDlg::ReadDataRefreshUI(WPARAM wParam, LPARAM lParam)
{
    Reflesh_ParameterDlg();
    UpdateData(FALSE);
    return 1;
}

LRESULT  CParameterDlg::ReadDataCallBack(WPARAM wParam, LPARAM lParam)
{
    _MessageReadOneInfo *Read_Struct_feedback =(_MessageReadOneInfo *)lParam;
    bool msg_result=WRITE_FAIL;
    msg_result = MKBOOL(wParam);
    if(msg_result)
    {
        product_register_value[Read_Struct_feedback->address]=Read_Struct_feedback->new_value;
    }
    if(Read_Struct_feedback!=NULL)
        delete Read_Struct_feedback;
    return 1;
}


//Add 20130516  by Fance
LRESULT  CParameterDlg::ResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
    UINT temp_id;
    _MessageWriteOneInfo *Write_Struct_feedback =(_MessageWriteOneInfo *)lParam;
    bool msg_result=WRITE_FAIL;
    msg_result = MKBOOL(wParam);
    vector <int>::iterator Iter;
    if(msg_result)
    {
        int indexid = -1;
        for (int i=0; i<(int)Change_Color_ID.size(); i++)
        {
            if(Change_Color_ID.at(i)!=Write_Struct_feedback->CTRL_ID)
                continue;
            else
                indexid = i;
        }

        if(indexid!=-1)
        {
            Iter = Change_Color_ID.begin()+indexid;
            Change_Color_ID.erase(Iter);
        }

        CString temp;
        temp.Format(_T("Change \"%s\" value from %d to %d success!"),
                    Write_Struct_feedback->Changed_Name,
                    Write_Struct_feedback->old_value,
                    Write_Struct_feedback->new_value);
        temp_id = Write_Struct_feedback->CTRL_ID;
        SetPaneString(1,temp);
        product_register_value[Write_Struct_feedback->address]= Write_Struct_feedback->new_value;
        if(Write_Struct_feedback!=NULL)
            delete Write_Struct_feedback;

    }
    else
    {
        CString temp;
        temp.Format(_T("Change \"%s\" value from %d to %d Fail!"),
                    Write_Struct_feedback->Changed_Name,
                    Write_Struct_feedback->old_value,
                    Write_Struct_feedback->new_value);

        temp_id = Write_Struct_feedback->CTRL_ID;
        SetPaneString(1,temp);
        Beep(10,100);
        product_register_value[Write_Struct_feedback->address]= Write_Struct_feedback->old_value;
        //GetDlgItem(Write_Struct_feedback->CTRL_ID)->SetWindowTextW(_T(""));

        int indexid = -1;
        for (int i=0; i<(int)Change_Color_ID.size(); i++)
        {
            if(Change_Color_ID.at(i)!=Write_Struct_feedback->CTRL_ID)
                continue;
            else
                indexid = i;
        }
        Iter = Change_Color_ID.begin()+indexid;
        if(indexid!=-1)
        {
            Iter = Change_Color_ID.begin()+indexid;
            Change_Color_ID.erase(Iter);
        }

        if(Write_Struct_feedback!=NULL)
        {
            delete Write_Struct_feedback;
        }
    }
    Reflesh_ParameterDlg();
    GetDlgItem(temp_id)->Invalidate();

    return 0;
}

HBRUSH CParameterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

    

    
    for (int i=0; i<(int)Change_Color_ID.size(); i++)
    {
        if(pWnd->GetDlgCtrlID()==Change_Color_ID.at(i))//注意此处的（pWnd->），否则没效果
        {
            pDC->SetTextColor(RGB(0,0,0));
            pDC->SetBkColor(RGB(255,0,0));//设置文本背景色
            pDC->SetBkMode(TRANSPARENT);//设置背景透明
            hbr = (HBRUSH)m_brush;
        }

    }

    return hbr;
}




void CParameterDlg::Fresh_Single_UI()
{
    //CString strTemp;
    //CString strUnit=GetTempUnit();//Unit string.
    //strTemp.Format(_T("%.1f"),product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);	//121
    //m_inputvalue1.SetWindowText(strTemp+strUnit);

    //strTemp.Format(_T("%d"),product_register_value[MODBUS_COOLING_PID]);		//104 384
    //m_pid_outputEdt1.SetWindowText(strTemp);

    //strTemp.Format(_T("%d"),product_register_value[MODBUS_PID_UNIVERSAL]);		//270  389
    //m_pid_outputEdt2.SetWindowText(strTemp);

}


//void CParameterDlg::OnEnChangeEdit34()
//{
//	
//	// send this notification unless you override the CDialog::OnInitDialog()
//	// function and call CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//
//	// 
//}


void CParameterDlg::OnCbnSelchangeStaticunint2()
{
    int sel=m_gUnit.GetCurSel();
    if (sel!=product_register_value[MODBUS_DEGC_OR_F])
    {
        write_one(g_tstat_id,MODBUS_DEGC_OR_F,sel);
        product_register_value[MODBUS_DEGC_OR_F]=sel;

        if(product_register_value[MODBUS_DEGC_OR_F] == 1)	//t5= 121;t6=104
        {
            g_unint = FALSE;
            GetDlgItem(IDC_STATICUNINT)->SetWindowText(_T("°F"));
        }
        else
        {
            g_unint = TRUE;
            GetDlgItem(IDC_STATICUNINT)->SetWindowText(_T("°C"));
        }

    }
}


void CParameterDlg::OnBnClickedCs1()
{
    CCustomSTable Dlg(2);
    Dlg.DoModal();
}


void CParameterDlg::OnBnClickedCs2()
{
    CCustomSTable Dlg(3);
    Dlg.DoModal();
}

void CParameterDlg::Read_SliderData()
{
    m_coolsp= (float)((short)product_register_value[MODBUS_DAY_COOLING_SETPOINT])/10;//348

    m_cooldb= (float)((short)product_register_value[MODBUS_DAY_COOLING_DEADBAND]) /10;//346
    m_setpoint = (float)((short)product_register_value[MODBUS_DAY_SETPOINT]) /10;//345
    m_heatdb = (float)((short)product_register_value[MODBUS_DAY_HEATING_DEADBAND]) /10;//347
    m_heatsp = (float)((short)product_register_value[MODBUS_DAY_HEATING_SETPOINT]) /10;//349

    m_coolspN= (float)((short)product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]) /10;//355
    m_cooldbN= (float)((short)product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]) /10;//353
    m_setpointN= (float)((short)product_register_value[MODBUS_NIGHT_SETPOINT]) /10;//350
    m_heatdbN= (float)((short)product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]) /10;//352
    m_heatspN= (float)((short)product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]) /10;


 	if (product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7)
 	{
 	 Read_Multi(g_tstat_id,&product_register_value[MODBUS_DAY_SETPOINT],MODBUS_DAY_SETPOINT,20);
 	}
 	else
 	{
 	Read_Multi(g_tstat_id,&product_register_value[MODBUS_COOLING_DEADBAND],MODBUS_COOLING_DEADBAND,2);
 	Read_Multi(g_tstat_id,&product_register_value[MODBUS_NIGHT_HEATING_DEADBAND],MODBUS_NIGHT_HEATING_DEADBAND,2);

 	Read_Multi(g_tstat_id,&product_register_value[MODBUS_COOLING_SETPOINT],MODBUS_COOLING_SETPOINT,2);

 	Read_Multi(g_tstat_id,&product_register_value[MODBUS_NIGHT_HEATING_SETPOINT],MODBUS_NIGHT_HEATING_SETPOINT,2);
 	}
}
void CParameterDlg::OnBnClickedAirflowSetting()
{
    CAirflowSettingDlg dlg;
    dlg.DoModal();
}
void CParameterDlg::OnCbnSelchangeInputselect3Pid3()
{
    if(g_ParamLevel==1)
        return;

    unsigned short TempValue;
    int sel=m_inputSelect3.GetCurSel();
// 	if (sel==0)
// 	{
    TempValue=sel+2;
// 	}
// 	else
// 	{
// 		TempValue=sel+4;
// 	}
    int RegAddress=-1;
    if (product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||(product_register_value[7] == PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {
        RegAddress=650;
    }
    else
    {
        RegAddress=MODBUS_SUPPLY_TEMP_SELECT;
    }


    CString strText;
    m_inputSelect3.GetWindowTextW(strText);
    if (strText.IsEmpty())
    {
        Reflesh_ParameterDlg();
        return;
    }

    if(product_register_value[RegAddress]==TempValue)	//Add this to judge weather this value need to change.
        return;
    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAddress,TempValue,
                        product_register_value[RegAddress],this->m_hWnd,IDC_INPUTSELECT3_PID3,_T("INPUT3_SELECT"));


    Reflesh_ParameterDlg();
}
//获取一个第八位数据，s:system:进制，n;num:这个数是几位的，第N位是1还是0
//n>=N
BOOL CParameterDlg::Get_Data_Bit(UINT Data,int n,int N)
{
    //Data=Data&0FFx;//去掉高八位数据
    //下面是算幂 2的N
    //最大的num位数的数是？ 2~num-1
    // AfxMessageBox(_T("OK"));
    UINT num=1;
    for (int i=1; i<=n; i++)
    {
        num=num*2;
    }
    num=num-1;//最大的num位数
    Data=Data&num;//取得这个n位数是什么 ？
    num=1;
    for (int i=1; i<N; i++)
    {
        num=num*2;
    }
    if (num==(Data&num))//说明是1
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
void CParameterDlg::ShowPID3()
{
    float m_fFirmwareVersion=get_curtstat_version();
    if (product_register_value[7]==PM_TSTAT6||
            product_register_value[7]==PM_TSTAT7||
            product_register_value[7]==PM_TSTAT5i||
            product_register_value[7] == PM_TSTAT8||
		    (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		||
            product_register_value[7]==PM_TSTAT5E||
            product_register_value[7] == PM_PM5E||
            (product_register_value[7]==PM_TSTATRUNAR)||
            ((product_register_value[7]==PM_TSTAT5G)
             &&(m_fFirmwareVersion>=37.5))
       )
    {
        GetDlgItem(IDC_STATIC_PID3_LOOP3)->ShowWindow(TRUE);
        GetDlgItem(IDC_SPSET4_PID3)->ShowWindow(TRUE);
        //GetDlgItem(IDC_ESETPOINTHI2_PID3)->ShowWindow(TRUE);
        //GetDlgItem(IDC_ESETPOINTLO2_PID3)->ShowWindow(TRUE);
        GetDlgItem(IDC_ECOOLDEADBAND3_PID3)->ShowWindow(TRUE);
        GetDlgItem(IDC_ECOOLINGITERM3_PID3)->ShowWindow(TRUE);

        GetDlgItem(IDC_STATIC_LOOP3)->ShowWindow(TRUE);
        GetDlgItem(IDC_INPUTSELECT3_PID3)->ShowWindow(TRUE);
        GetDlgItem(IDC_INPUTVALUE3_PID3)->ShowWindow(TRUE);
        GetDlgItem(IDC_SETVALUE3_PID3)->ShowWindow(TRUE);
        GetDlgItem(IDC_OUTPUT3_PID3)->ShowWindow(TRUE);
        GetDlgItem(IDC_ECOOLINGPTERM3_PID3)->ShowWindow(TRUE);
        GetDlgItem(IDC_EDIT52_PID3)->ShowWindow(TRUE);

        GetDlgItem(IDC_AIRFLOW_SETTING)->ShowWindow(TRUE);

        GetDlgItem(IDC_STATIC_PID2OFFSETPOINT2)->ShowWindow(TRUE);
        GetDlgItem(IDC_EDIT_PID2OFFSETPOINT6)->ShowWindow(TRUE);

        CString strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN1], 1);

        CString temp,strTemp;
        temp.Format(_T("%.1f"),((float)((short)product_register_value[MODBUS_PID3_DAY_SETPOINT]))/10);
        temp+=strUnit;
        GetDlgItem(IDC_SPSET4_PID3)->SetWindowText(temp);
        temp.Format(_T("%d"),product_register_value[MODBUS_MAX_SUPPLY_SETPOINT]);
        temp+=strUnit;
        GetDlgItem(IDC_ESETPOINTHI2_PID3)->SetWindowText(temp);
        temp.Format(_T("%d"),product_register_value[MODBUS_MIN_SUPPLY_SETPOINT]);
        temp+=strUnit;
        GetDlgItem(IDC_ESETPOINTLO2_PID3)->SetWindowText(temp);
        temp.Format(_T("%d"),product_register_value[MODBUS_PID3_COOLING_DB]);
        temp+=strUnit;
        GetDlgItem(IDC_ECOOLDEADBAND3_PID3)->SetWindowText(temp);
        temp.Format(_T("%d"),product_register_value[MODBUS_PID3_HEATING_DB]);
        temp+=strUnit;
        GetDlgItem(IDC_ECOOLINGITERM3_PID3)->SetWindowText(temp);
        int RegAddress=-1;
        if (product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||(product_register_value[7] == PM_TSTAT8)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
        {
            RegAddress=650;
        }
        else
        {
            RegAddress=MODBUS_SUPPLY_TEMP_SELECT;
        }
// 		if (product_register_value[RegAddress]<3)
// 		{
// 			m_inputSelect3.SetCurSel(0);
// 		}
// 		else if (product_register_value[RegAddress]>4)
// 		{
        m_inputSelect3.SetCurSel(product_register_value[RegAddress]-2);
// 		}
// 		else
// 		{
// 			m_inputSelect3.SetCurSel(0);
// 		}




        RegAddress=-1;
        if (product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||(product_register_value[7] == PM_TSTAT8)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
        {
            RegAddress=650;
        }
        else
        {
            RegAddress=MODBUS_SUPPLY_TEMP_SELECT;
        }

        //if(product_register_value[RegAddress]==5) // input1
        //{

        //	strTemp=GetInputValue(1);
        //	m_inputValue3.SetWindowText(strTemp);
        //}
        //else if(product_register_value[RegAddress]==6) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(2);
        //	m_inputValue3.SetWindowText(strTemp);
        //}
        //else if(product_register_value[RegAddress]==7) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(3);
        //	m_inputValue3.SetWindowText(strTemp);
        //}
        //else if(product_register_value[RegAddress]==8) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(4);
        //	m_inputValue3.SetWindowText(strTemp);
        //}
        //else if(product_register_value[RegAddress]==9) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(5);
        //	m_inputValue3.SetWindowText(strTemp);
        //}
        //else if(product_register_value[RegAddress]==10) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(6);
        //	m_inputValue3.SetWindowText(strTemp);
        //}
        //else if(product_register_value[RegAddress]==11) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(7);
        //	m_inputValue3.SetWindowText(strTemp);
        //}
        //else if(product_register_value[RegAddress]==12) // input2 //m_inputvalue1
        //{

        //	strTemp=GetInputValue(8);
        //	m_inputValue3.SetWindowText(strTemp);
        //}
        //else if (product_register_value[RegAddress]==13)//Humidity
        //{		CString temp;
        //strUnit=_T("%");
        //if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
        //{

        //	temp.Format(_T("%0.1f"),(float)(product_register_value[MODBUS_TSTAT6_HUM_AVALUE]/10));

        //}
        //else
        //{
        //	temp.Format(_T("%0.1f"),(float)(product_register_value[MODBUS_TSTAT6_HUM_MVALUE]/10));
        //}


        //m_inputValue3.SetWindowText(temp+strUnit);

        //}
        //else if (product_register_value[RegAddress]==14)//Co2
        //{ CString temp;
        //strUnit=_T("ppm");
        //if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
        //{

        //	temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_AVALUE]);
        //	temp=temp+strUnit;

        //}
        //else
        //{

        //	temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_MVALUE]);
        //	temp=temp+strUnit;
        //}

        //m_inputValue3.SetWindowText(temp);


        //}
        if(product_register_value[RegAddress]>=5&&product_register_value[RegAddress]<=14) // input1
        {

            strTemp=m_tstat_input_data.at(product_register_value[RegAddress]-5 + 1).Value.StrValue + m_tstat_input_data.at(product_register_value[RegAddress]-5 + 1).Unit.StrValue;
            m_inputValue3.SetWindowText(strTemp);
        }
        else
        {
            //m_inputvalue1.SetWindowText(_T("UNUSED"));
            strTemp.Format(_T("%0.1f"),product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);	//121
            m_inputValue3.SetWindowText(strTemp+strUnit);
        }


        if (product_register_value[7]==PM_TSTAT5i||(product_register_value[7] == PM_TSTAT8)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V) 
			||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7)
        {

            temp.Format(_T("%.1f"),((float)(short)product_register_value[MODBUS_PID3_DAY_SETPOINT])/10);
            //temp+=strUnit;
            GetDlgItem(IDC_SETVALUE3_PID3)->SetWindowText(temp);
            temp.Format(_T("%d%%"),product_register_value[664]);
            //temp+=strUnit;
            GetDlgItem(IDC_OUTPUT3_PID3)->SetWindowText(temp);
            temp.Format(_T("%0.1f"),((float)(short)product_register_value[660])/10);
            GetDlgItem(IDC_ECOOLINGPTERM3_PID3)->SetWindowText(temp);
            temp.Format(_T("%0.1f"),((float)(short)product_register_value[661])/10);
            GetDlgItem(IDC_EDIT52_PID3)->SetWindowText(temp);

            temp.Format(_T("%0.1f"),((float)(short)product_register_value[MODBUS_PID3_NIGHT_SETPOINT])/10);

            GetDlgItem(IDC_EDIT_PID2OFFSETPOINT6)->SetWindowText(temp);
        }
        else
        {
            if (product_register_value[7]==PM_TSTAT5G)
            {
                if(product_register_value[RegAddress]==1)
                {
                    strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN1], 1);//188
                    if(product_register_value[MODBUS_ANALOG_IN1]==4||product_register_value[MODBUS_ANALOG_IN1]==1)//188
                    {
                        strTemp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_0]/10);//180
                        strTemp=strTemp+strUnit;
                    }
                    if (product_register_value[MODBUS_ANALOG_IN1]==0||product_register_value[MODBUS_ANALOG_IN1]==2)//188
                    {
                        //strUnit=GetTempUnit(180);
                        strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_0]);//180
                    }
                    if(product_register_value[MODBUS_ANALOG_IN1]==3)//188
                    {
                        if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//180
                            strTemp=_T("ON");
                        if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//180
                            strTemp=_T("OFF");
                    }


                    if (product_register_value[MODBUS_ANALOG_IN1] == 0)//188
                    {
                        strTemp = _T("UNUSED");
                    }
                }
                else if (product_register_value[RegAddress]==2)
                {
                    strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN2],2);
                    strTemp.Empty();
                    if(product_register_value[MODBUS_ANALOG_IN2]==4||product_register_value[MODBUS_ANALOG_IN2]==1)//189
                    {

                        strTemp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_1]/10);	//181
                        strTemp=strTemp+strUnit;
                    }
                    if (product_register_value[MODBUS_ANALOG_IN2]==0||product_register_value[MODBUS_ANALOG_IN2]==2)//189
                    {
                        strUnit=GetTempUnit(181);
                        strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_1]);//181
                        strTemp=strTemp+strUnit;
                    }
                    if(product_register_value[MODBUS_ANALOG_IN2]==3)//189
                    {
                        if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
                            strTemp=_T("ON");
                        if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
                            strTemp=_T("OFF");
                    }


                    if (product_register_value[MODBUS_ANALOG_IN2] == 0)//189
                    {
                        strTemp = _T("UNUSED");
                    }

                }
                else
                {
                    strTemp = _T("UNUSED");
                }


                m_inputValue3.SetWindowText(strTemp);

            }
            temp.Format(_T("%d"),product_register_value[MODBUS_AIRFLOW_SETPOINT]);
            temp+=strUnit;
            GetDlgItem(IDC_SETVALUE3_PID3)->SetWindowText(temp);
            temp.Format(_T("%d%%"),product_register_value[MODBUS_PID3_OUTPUT]);
            //temp+=strUnit;
            GetDlgItem(IDC_OUTPUT3_PID3)->SetWindowText(temp);
            temp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_PID3_PTERM]/10);
            GetDlgItem(IDC_ECOOLINGPTERM3_PID3)->SetWindowText(temp);
            temp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_PID3_ITERM]/10);
            GetDlgItem(IDC_EDIT52_PID3)->SetWindowText(temp);

            BOOL is_have_AVASensor=product_register_value[MODBUS_VAV_CONTROL]&2;
            if (!is_have_AVASensor)
            {
                GetDlgItem(IDC_STATIC_PID3_LOOP3)->EnableWindow(FALSE);
                GetDlgItem(IDC_SPSET4_PID3)->EnableWindow(FALSE);
                //GetDlgItem(IDC_ESETPOINTHI2_PID3)->ShowWindow(FALSE);
                //GetDlgItem(IDC_ESETPOINTLO2_PID3)->ShowWindow(FALSE);
                GetDlgItem(IDC_ECOOLDEADBAND3_PID3)->EnableWindow(FALSE);
                GetDlgItem(IDC_ECOOLINGITERM3_PID3)->EnableWindow(FALSE);
                GetDlgItem(IDC_STATIC_LOOP3)->EnableWindow(FALSE);
                GetDlgItem(IDC_INPUTSELECT3_PID3)->EnableWindow(FALSE);
                GetDlgItem(IDC_INPUTVALUE3_PID3)->EnableWindow(FALSE);
                GetDlgItem(IDC_SETVALUE3_PID3)->EnableWindow(FALSE);
                GetDlgItem(IDC_OUTPUT3_PID3)->EnableWindow(FALSE);
                GetDlgItem(IDC_ECOOLINGPTERM3_PID3)->EnableWindow(FALSE);
                GetDlgItem(IDC_EDIT52_PID3)->EnableWindow(FALSE);
            }
        }





    }
    else
    {
        GetDlgItem(IDC_STATIC_PID3_LOOP3)->ShowWindow(FALSE);
        GetDlgItem(IDC_SPSET4_PID3)->ShowWindow(FALSE);
        //GetDlgItem(IDC_ESETPOINTHI2_PID3)->ShowWindow(FALSE);
        //GetDlgItem(IDC_ESETPOINTLO2_PID3)->ShowWindow(FALSE);
        GetDlgItem(IDC_ECOOLDEADBAND3_PID3)->ShowWindow(FALSE);
        GetDlgItem(IDC_ECOOLINGITERM3_PID3)->ShowWindow(FALSE);
        GetDlgItem(IDC_STATIC_LOOP3)->ShowWindow(FALSE);
        GetDlgItem(IDC_INPUTSELECT3_PID3)->ShowWindow(FALSE);
        GetDlgItem(IDC_INPUTVALUE3_PID3)->ShowWindow(FALSE);
        GetDlgItem(IDC_SETVALUE3_PID3)->ShowWindow(FALSE);
        GetDlgItem(IDC_OUTPUT3_PID3)->ShowWindow(FALSE);
        GetDlgItem(IDC_ECOOLINGPTERM3_PID3)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT52_PID3)->ShowWindow(FALSE);

        GetDlgItem(IDC_AIRFLOW_SETTING)->ShowWindow(FALSE);


        GetDlgItem(IDC_STATIC_PID2OFFSETPOINT2)->ShowWindow(FALSE);
        GetDlgItem(IDC_EDIT_PID2OFFSETPOINT6)->ShowWindow(FALSE);

    }
}

void CParameterDlg::OnEnKillfocusSetvalue3Pid3()
{

    CString temp;
    GetDlgItem(IDC_SETVALUE3_PID3)->GetWindowText(temp);
    float Val=_wtof(temp)*10;

    int RegValue=(int)Val;
    int RegAddress;
    if (product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||(product_register_value[7] == PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {


        RegAddress=705;


    }
    else
    {
        RegAddress=MODBUS_SUPPLY_SETPOINT;

    }

    //temp.Format(_T("%d"),product_register_value[MODBUS_SUPPLY_SETPOINT]);


    if (RegValue==product_register_value[RegAddress])
    {
        return;
    }





    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAddress,RegValue,
                        product_register_value[RegAddress],this->m_hWnd,IDC_SETVALUE3_PID3,_T("SetPoint PID3"));


}

//PID3 Output
void CParameterDlg::OnEnKillfocusOutput3Pid3()
{
    CString temp;
    //temp.Format(_T("%d"),product_register_value[MODBUS_SUPPLY_SETPOINT]);
    GetDlgItem(IDC_OUTPUT3_PID3)->GetWindowText(temp);
    int Val=_wtoi(temp);
    int RegValue=(int)Val;
    int RegAddress;
    if (product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||(product_register_value[7] == PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {
        RegAddress=664;
    }
    else
    {
        RegAddress=MODBUS_PID3_OUTPUT;
    }
    if (RegValue==product_register_value[RegAddress])
    {
        return;
    }
    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAddress,RegValue,
                        product_register_value[RegAddress],this->m_hWnd,IDC_OUTPUT3_PID3,_T("Output PID3"));

}

//PID3 Ptem
void CParameterDlg::OnEnKillfocusEcoolingpterm3Pid3()
{
    CString temp;
    //temp.Format(_T("%d"),product_register_value[MODBUS_SUPPLY_SETPOINT]);
    GetDlgItem(IDC_ECOOLINGPTERM3_PID3)->GetWindowText(temp);
    //int Val=_wtoi(temp);
    float Val=_wtof(temp)*10;
    //if (Val==product_register_value[MODBUS_PID3_PTERM])
    //{
    //	return;
    //}
    //int ret=write_one(g_tstat_id,MODBUS_PID3_PTERM,Val);
    //if (ret>0)
    //{
    //	product_register_value[MODBUS_PID3_PTERM]=Val;
    //	ShowPID3();
    //}
    //else
    //{
    //	AfxMessageBox(_T("Fail"));
    //}

    int RegValue=(int)Val;
    int RegAddress;
    if (product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||(product_register_value[7] == PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V) || (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {
        RegAddress=660;
    }
    else
    {
        RegAddress=MODBUS_PID3_PTERM;
    }
    if (RegValue==product_register_value[RegAddress])
    {
        return;
    }
    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAddress,RegValue,
                        product_register_value[RegAddress],this->m_hWnd,IDC_ECOOLINGPTERM3_PID3,_T("Ptem PID3"));

}

//PID3 Item
void CParameterDlg::OnEnKillfocusEdit52Pid3()
{
    CString temp;
    //temp.Format(_T("%d"),product_register_value[MODBUS_SUPPLY_SETPOINT]);
    GetDlgItem(IDC_EDIT52_PID3)->GetWindowText(temp);
    //int Val=_wtoi(temp);
    float Val=_wtof(temp)*10;
    int RegValue=(int)Val;
    int RegAddress;
    if (product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||(product_register_value[7] == PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {
        RegAddress=661;
    }
    else
    {
        RegAddress=MODBUS_PID3_ITERM;
    }
    if (RegValue==product_register_value[RegAddress])
    {
        return;
    }
    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAddress,RegValue,
                        product_register_value[RegAddress],this->m_hWnd,IDC_EDIT52_PID3,_T("Item PID3"));

}


///PID3   off setpoint
void CParameterDlg::OnEnKillfocusEditPid2offsetpoint6()
{
    CString temp;
    //temp.Format(_T("%d"),product_register_value[MODBUS_SUPPLY_SETPOINT]);
    GetDlgItem(IDC_EDIT_PID2OFFSETPOINT6)->GetWindowText(temp);
//	int Val=_wtoi(temp);
    float Val=_wtof(temp)*10;
    int RegValue=(int)Val;
    int RegAddress;
    if (product_register_value[7]==PM_TSTAT5i||product_register_value[7]==PM_TSTAT6||product_register_value[7]==PM_TSTAT7||(product_register_value[7] == PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
    {
        RegAddress=706;
        if (RegValue==product_register_value[RegAddress])
        {
            return;
        }
        Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAddress,RegValue,
                            product_register_value[RegAddress],this->m_hWnd,IDC_EDIT_PID2OFFSETPOINT6,_T("Off Setpoint PID3"));
    }



}
//SetPoint
void CParameterDlg::OnEnKillfocusSpset4Pid3()
{
    CString temp;
    //temp.Format(_T("%d"),product_register_value[MODBUS_SUPPLY_SETPOINT]);
    GetDlgItem(IDC_SPSET4_PID3)->GetWindowText(temp);

    unsigned short temp_value = (unsigned short)(_wtof(temp)*10);
    int ret = write_one(g_tstat_id, MODBUS_PID3_DAY_SETPOINT, temp_value);
    if (ret > 0)
    {
        product_register_value[MODBUS_PID3_DAY_SETPOINT] = temp_value;
    }

}


void CParameterDlg::OnEnKillfocusEsetpointhi2Pid3()
{
    CString temp;
    //temp.Format(_T("%d"),product_register_value[MODBUS_SUPPLY_SETPOINT]);
    GetDlgItem(IDC_ESETPOINTHI2_PID3)->GetWindowText(temp);
    int Val=_wtoi(temp);
    if (Val==product_register_value[MODBUS_MAX_SUPPLY_SETPOINT])
    {
        return;
    }
    int ret=write_one(g_tstat_id,MODBUS_MAX_SUPPLY_SETPOINT,Val);
    if (ret>0)
    {
        product_register_value[MODBUS_MAX_SUPPLY_SETPOINT]=Val;
        ShowPID3();
    }
    else
    {
        AfxMessageBox(_T("Fail"));
    }
}


void CParameterDlg::OnEnKillfocusEsetpointlo2Pid3()
{
    CString temp;
    //temp.Format(_T("%d"),product_register_value[MODBUS_SUPPLY_SETPOINT]);
    GetDlgItem(IDC_ESETPOINTLO2_PID3)->GetWindowText(temp);
    int Val=_wtoi(temp);
    if (Val==product_register_value[MODBUS_MIN_SUPPLY_SETPOINT])
    {
        return;
    }
    int ret=write_one(g_tstat_id,MODBUS_MIN_SUPPLY_SETPOINT,Val);
    if (ret>0)
    {
        product_register_value[MODBUS_MIN_SUPPLY_SETPOINT]=Val;
        ShowPID3();
    }
    else
    {
        AfxMessageBox(_T("Fail"));
    }
}


void CParameterDlg::OnEnKillfocusEcooldeadband3Pid3()
{
    CString temp;
    //temp.Format(_T("%d"),product_register_value[MODBUS_SUPPLY_SETPOINT]);
    GetDlgItem(IDC_ECOOLDEADBAND3_PID3)->GetWindowText(temp);
    int Val=_wtoi(temp);
    if (Val==product_register_value[MODBUS_PID3_COOLING_DB])
    {
        return;
    }
    int ret=write_one(g_tstat_id,MODBUS_PID3_COOLING_DB,Val);
    if (ret>0)
    {
        product_register_value[MODBUS_PID3_COOLING_DB]=Val;
        ShowPID3();
    }
    else
    {
        AfxMessageBox(_T("Fail"));
    }
}


void CParameterDlg::OnEnKillfocusEcoolingiterm3Pid3()
{
    CString temp;
    //temp.Format(_T("%d"),product_register_value[MODBUS_SUPPLY_SETPOINT]);
    GetDlgItem(IDC_ECOOLINGITERM3_PID3)->GetWindowText(temp);
    int Val=_wtoi(temp);
    if (Val==product_register_value[MODBUS_PID3_HEATING_DB])
    {
        return;
    }
    int ret=write_one(g_tstat_id,MODBUS_PID3_HEATING_DB,Val);
    if (ret>0)
    {
        product_register_value[MODBUS_PID3_HEATING_DB]=Val;
        ShowPID3();
    }
    else
    {
        AfxMessageBox(_T("Fail"));
    }
}


void CParameterDlg::OnEnKillfocusEditBacklightTime()
{
    CString temp;
    m_edit_backlighttime.GetWindowText(temp);
    int val=_wtoi(temp);
    int ret=write_one(g_tstat_id,434,val);
    if (ret>0)
    {
        product_register_value[434]=val;
        CString backlight;
        backlight.Format(_T("%d"),product_register_value[434]);
        m_edit_backlighttime.SetWindowText(backlight);
    }
    else
    {
        AfxMessageBox(_T("Write Fail!"));
    }
}


void CParameterDlg::OnBnClickedCheck2()
{
    if (m_check_occupiedenable.GetCheck()==1)
    {
        int ret1=write_one(g_tstat_id,629,1);
        int ret2=write_one(g_tstat_id,565,0);
        if (ret1&&ret2)
        {
            product_register_value[629]=1;
            product_register_value[565]=0;
        }
    }

    if (m_check_occupiedenable.GetCheck()==0)
    {
        int ret1=write_one(g_tstat_id,629,0);

        if (ret1)
        {
            product_register_value[629]=0;

        }
    }


}


void CParameterDlg::OnEnKillfocusEditOccupiedTimer()
{
    CString timer;
    int times;
    m_occupied_timer.GetWindowText(timer);
    times=_wtoi(timer);

    if (product_register_value[641]!=times)
    {
        int ret=write_one(g_tstat_id,641,times);
        if (ret>0)
        {
            product_register_value[641]=times;
        }
    }

    CString occupiedtimer;
    occupiedtimer.Format(_T("%d"),product_register_value[641]);
    m_occupied_timer.SetWindowText(occupiedtimer);

}


void CParameterDlg::OnEnKillfocusEditDeadMaster()
{
    CString deadmaster;
    GetDlgItem(IDC_EDIT_DEAD_MASTER)->GetWindowText(deadmaster);
    int deadmastervalue=_wtoi(deadmaster.GetBuffer());
    if (product_register_value[MODBUS_DEAD_MASTER]==deadmastervalue)
    {
        return;
    }
    else
    {

        int ret=write_one(g_tstat_id,MODBUS_DEAD_MASTER,deadmastervalue);
        if (ret>0)
        {
            product_register_value[MODBUS_DEAD_MASTER]=deadmastervalue;
        }

    }

}


void CParameterDlg::OnCbnSelchangeComboEnableDis()
{
    int sel=m_combox_pir_endisable.GetCurSel();
    if (sel==0)
    {
        int ret1=write_one(g_tstat_id,629,1);
        
        if (ret1>0)
        {
            product_register_value[629]=1;
          /*  product_register_value[565]=0;*/
        }
    }
    else
    {
        int ret1= write_one(g_tstat_id,629,00);
        if (ret1>0)
        {
            product_register_value[629]=0;
        }
    }
    Reflesh_ParameterDlg();
}


void CParameterDlg::OnEnKillfocusEditSensitivty()
{
    CString strTemp;
    m_sensitivity_editor.GetWindowTextW(strTemp);
    int intTemp=_wtoi(strTemp);
    if (intTemp>=20&&intTemp<=100)
    {
        int ret=write_one(g_tstat_id,735,intTemp);
        if (ret>0)
        {
            product_register_value[735]=intTemp;
        }
    }
    else
    {
        AfxMessageBox(_T("please input a value between 20 and 100!"));
    }
    Reflesh_ParameterDlg();
}


//void CParameterDlg::OnCbnSelchangeComboSetpointdisplay()
//{
//    int sel=m_combox_setpointdisplay.GetCurSel();
//    int ret=write_one(g_tstat_id,704,sel);
//    if (ret>0)
//    {
//        product_register_value[704]=sel;
//    }
//    m_combox_setpointdisplay.SetCurSel(product_register_value[704]);
//}


// void CParameterDlg::OnCbnSelchangeComboZigbeetype()
// {
//     int sel=m_combox_zigbee.GetCurSel();
//     if (sel==product_register_value[22])
//     {
//         return;
//     }
//     int ret=write_one(g_tstat_id,22,sel);
//     if (ret>0)
//     {
//         product_register_value[22]=sel;
//     }
// }


// void CParameterDlg::OnCbnSelchangeComboChannel()
// {
//     int sel=m_combox_channel.GetCurSel();
//     int channelno=sel+11;
//     int channelvalue=0;
//     if (channelno==11)
//     {
//         channelvalue=0x07FFF8000;
//     }
//     else if (channelno==12)
//     {
//         channelvalue=0x00001000;
//     }
//     else if (channelno==13)
//     {
//         channelvalue=0x00002000;
//     }
//     else if (channelno==14)
//     {
//         channelvalue=0x00004000;
//     }
//     else if (channelno==15)
//     {
//         channelvalue=0x00008000;
//     }
//     else if (channelno==16)
//     {
//         channelvalue=0x00010000;
//     }
//     else if (channelno==17)
//     {
//         channelvalue=0x00020000;
//     }
//     else if (channelno==18)
//     {
//         channelvalue=0x00040000;
//     }
//     else if (channelno==19)
//     {
//         channelvalue=0x00080000;
//     }
//     else if (channelno==20)
//     {
//         channelvalue=0x00100000;
//     }
//     else if (channelno==21)
//     {
//         channelvalue=0x00200000;
//     }
//     else if (channelno==22)
//     {
//         channelvalue=0x00400000;
//     }
//     else if (channelno==23)
//     {
//         channelvalue=0x00800000;
//     }
//     else if (channelno==24)
//     {
//         channelvalue=0x01000000;
//     }
//     else if (channelno==25)
//     {
//         channelvalue=0x02000000;
//     }
//     else if (channelno==26)
//     {
//         channelvalue=0x04000000;
//     }
//     else
//     {
// 
//         channelvalue=0;
//     }
//     int high=0;
//     int low=0;
//     if (channelvalue>0)
//     {
//         high=channelvalue/65535;
//         low=channelvalue%65535;
//     }
//     if ((product_register_value[23]!=high)||(product_register_value[24]!=low))
//     {
//         write_one(g_tstat_id,23,high);
//         write_one(g_tstat_id,24,low);
//     }
// 
// }


//void CParameterDlg::OnBnClickedButtonZigbeeReboot()
//{
//    int ret= write_one(g_tstat_id,34,1);
//    if (ret<0)
//    {
//        AfxMessageBox(_T("Reboot it ,again"));
//    }
//}

extern bool b_pause_refresh_tree ;
//void CParameterDlg::OnBnClickedBtnTopological()
//{
//    int temp_pause = g_bPauseRefreshTree;	//resume all the status.
//    int temp_g_id =  g_tstat_id;
//    int temp_value = b_pause_refresh_tree;
//    bool temp_pasueread = g_bPauseMultiRead;
//    b_pause_refresh_tree = true;
//    g_bPauseRefreshTree = true;
//    g_bPauseMultiRead = true;
//    CTstatZigbeeLogic dlg;
//    dlg.DoModal();
//    g_bPauseRefreshTree = temp_pause;
//    g_tstat_id = temp_g_id ;
//    b_pause_refresh_tree = temp_value;
//    g_bPauseMultiRead = temp_pasueread;
//}





//void CParameterDlg::OnBnClickedUnlock()
//{
//    if (product_register_value[7]==6)
//    {
//        write_one(g_tstat_id,639,0);
//    }
//
//}


//void CParameterDlg::OnBnClickedLock()
//{
//    if (product_register_value[7]==6)
//    {
//        write_one(g_tstat_id,639,1);
//    }
//}
#include "DisplayConfig.h"
#include "LedsDialog.h"

void CParameterDlg::OnBnClickedTrendlogview()
{
    if ((product_register_value[7]==PM_TSTAT5E||product_register_value[7] == PM_PM5E||(product_register_value[7]==PM_TSTATRUNAR)||(product_register_value[7]==PM_TSTAT5G))||(product_register_value[7]==PM_TSTAT6)||(product_register_value[7] == PM_TSTAT8)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V) 
		||(product_register_value[7]==PM_TSTAT5i)||(product_register_value[7]==PM_TSTAT7))
    {
        CDisplayConfig display_cfg;
        display_cfg.DoModal();
    }
    else
    {
        CLedsDialog dialog;
        dialog.DoModal();
    }
}


void CParameterDlg::OnCbnSelchangeCombo1()
{
    CString str;
    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    GetDlgItem(IDC_COMBO1)->GetWindowText(str);

    /*if (str.CompareNoCase(_T("Single")) == 0)
    	MDAY = 0;
    else
    	MDAY = 1;*/
    MDAY=m_day_combox.GetCurSel();
    FlexSP = 1;
    ::PostMessage(pFrame->m_pViews[DLG_T3000_VIEW]->m_hWnd,MsgT3000ViewFresh,0,0);
}


void CParameterDlg::OnCbnSelchangeCombo4()
{
    CString str;
    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    //GetDlgItem(IDC_COMBO4)->GetWindowText(str);

    /*if (str.CompareNoCase(_T("Single")) == 0)
    	MNIGHT = 0;
    else
    	MNIGHT = 1;*/
    MNIGHT=m_night_combox.GetCurSel();
    FlexSPN = 1;
    ::PostMessage(pFrame->m_pViews[DLG_T3000_VIEW]->m_hWnd,MsgT3000ViewFresh,0,0);
}


void CParameterDlg::OnEnKillfocusEditTranducerMax()
{
    CString deadmaster;
    GetDlgItem(IDC_EDIT_TRANDUCER_MAX)->GetWindowText(deadmaster);
    int deadmastervalue=_wtoi(deadmaster.GetBuffer());
    if (product_register_value[727]==deadmastervalue)
    {
        return;
    }

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,727,deadmastervalue,
                        product_register_value[727],this->m_hWnd,IDC_EDIT_TRANDUCER_MAX,_T("Tranducer Max"));
}


void CParameterDlg::OnEnKillfocusEditTranducerMin()
{
    CString deadmaster;
    GetDlgItem(IDC_EDIT_TRANDUCER_MIN)->GetWindowText(deadmaster);
    int deadmastervalue=_wtoi(deadmaster.GetBuffer());
    if (product_register_value[726]==deadmastervalue)
    {
        return;
    }

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,726,deadmastervalue,
                        product_register_value[726],this->m_hWnd,IDC_EDIT_TRANDUCER_MIN,_T("Tranducer Min"));
}


void CParameterDlg::OnEnKillfocusEditChangeOverTime()
{
    CString deadmaster;
    GetDlgItem(IDC_EDIT_CHANGE_OVER_TIME)->GetWindowText(deadmaster);
    int deadmastervalue=_wtoi(deadmaster.GetBuffer());
    if (product_register_value[242]==deadmastervalue)
    {
        return;
    }

    Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,242,deadmastervalue,
                        product_register_value[242],this->m_hWnd,IDC_EDIT_CHANGE_OVER_TIME,_T("Change Over Time(Min)"));
}


void CParameterDlg::OnEnKillfocusEditTstatName()
{
    CString strSerial;
    strSerial.Format(_T("%d"),get_serialnumber());

    CString oldname;
    CString newname;
    CString strSql;
    GetDlgItem(IDC_EDIT_TSTAT_NAME)->GetWindowText(newname);
    if (newname.IsEmpty())
    {
        return;
    }
    oldname.Format(_T("%s%s"),GetTextFromReg(715),GetTextFromReg(719));
    if (oldname.CompareNoCase(newname)==0)
    {
        return;
    }
    if(newname.GetLength()> 17)	//长度不能大于结构体定义的长度;
    {
        newname.Delete(16,newname.GetLength()-16);
    }
    char cTemp1[16];
    memset(cTemp1,0,16);
    WideCharToMultiByte( CP_ACP, 0, newname.GetBuffer(), -1, cTemp1, 16, NULL, NULL );
    unsigned char Databuffer[16];
    memcpy_s(Databuffer,16,cTemp1,16);
    if (Write_Multi(g_tstat_id,Databuffer,715,16,10)>0)
    {
        for (int i=0; i<16; i++)
        {
            product_register_value[715+i]=Databuffer[i];
        }
        GetDlgItem(IDC_EDIT_TSTAT_NAME)->GetWindowText(newname);
		CppSQLite3DB SqliteDBBuilding;
		CppSQLite3Table table;
		CppSQLite3Query q;
		SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

        strSql.Format(_T("update ALL_NODE set Product_name='%s' where Serial_ID='%s'"),newname,strSerial);
        SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
        SqliteDBBuilding.closedb();
        CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
        // 这里就是用来刷新产品树结构,刷新树的产品的名字，以及名字产品的装填
        ::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);
    }
    /* else
     {
        GetDlgItem(IDC_EDIT_TSTAT_NAME)->GetWindowText(oldname);
     }*/
}


void CParameterDlg::OnCbnSelchangeComboMrd()
{
	int sel =  m_combox_mrd.GetCurSel();
	int write_Value = 0;
	if (product_register_value[7] == PM_TSTAT5i 
		|| product_register_value[7] == PM_TSTAT6 
		|| product_register_value[7] == PM_TSTAT7
		|| product_register_value[7] == PM_TSTAT8
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		)
	{

		 if (sel == 0)
		 {
			 write_Value = product_register_value[109] & 0xffef;
		 }
		 else
		 {
			 write_Value = product_register_value[109] | 0x10;
		 }
		int ret = write_one(g_tstat_id, 109, write_Value);
		if (ret>0)
		{
			product_register_value[109] = write_Value;
		}
	}
	else
	{
		if (sel == 0)
		{
			write_Value = product_register_value[184] & 0xffef;
		}
		else
		{
			write_Value = product_register_value[184] | 0x10;
		}

		int ret = write_one(g_tstat_id, 184, write_Value);
		if (ret > 0)
		{
			product_register_value[184] = write_Value;
		}
	}
	
	Reflesh_ParameterDlg();
		
}


void CParameterDlg::OnBnClickedButtonApplyAll()
{
	 
}

#include "ZigbeeInformationDlg.h"
void CParameterDlg::OnBnClickedButtonZigbee()
{
	CZigbeeInformationDlg dlg;
	dlg.DoModal();
	 
}


void CParameterDlg::OnEnKillfocusEditShowid()
{
	CString deadmaster;
	GetDlgItem(IDC_EDIT_SHOWID)->GetWindowText(deadmaster);
	int deadmastervalue = _wtoi(deadmaster.GetBuffer());
	 

	Post_Thread_Message(MY_WRITE_ONE, 255, 725, deadmastervalue,
		product_register_value[725], this->m_hWnd, IDC_EDIT_SHOWID, _T("Show Modbus ID(Min)"));
}


void CParameterDlg::OnBnClickedButtonShowModbusId()
{
	int deadmastervalue = 5;

	Post_Thread_Message(MY_WRITE_ONE, 255, 725, deadmastervalue,
		product_register_value[725], this->m_hWnd, IDC_EDIT_SHOWID, _T("Show Modbus ID(Min)"));
	m_time_left = 5 * 60;
	SetTimer(1, 1000, NULL);
}
void CParameterDlg::OnTimer(UINT_PTR nIDEvent)
{
	 
	if (nIDEvent == 1)
	{
		if (m_time_left == 0)
		{
			GetDlgItem(IDC_BUTTON_SHOW_MODBUS_ID)->SetWindowTextW(L"Show Modbus ID");
			GetDlgItem(IDC_BUTTON_SHOW_MODBUS_ID)->EnableWindow(TRUE);
			KillTimer(1);
			 
		}
		else
		{
			CString time;
			--m_time_left;
			time.Format(L"Time Left:%d(s)", m_time_left);
			GetDlgItem(IDC_BUTTON_SHOW_MODBUS_ID)->SetWindowTextW(time);
			GetDlgItem(IDC_BUTTON_SHOW_MODBUS_ID)->EnableWindow(FALSE);

		}

	}
	CDialog::OnTimer(nIDEvent);
}
#include "Tstat_HelpDoc.h"
BOOL CParameterDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{

	HWND hWnd;

	if (pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szTstatHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
	else
		hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szTstatHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_ONE_HEAT_ONE_COOL_SETTING);

	return (hWnd != NULL);

	return CDialog::OnHelpInfo(pHelpInfo);
}


void CParameterDlg::OnEnKillfocusEditDterm()
{
    // TODO: 在此添加控件通知处理程序代码
    if (g_ParamLevel == 1)
        return;

    CString strText;
    m_edit_dterm.GetWindowText(strText);
    int nValue = (int)(_wtoi(strText));


    if (product_register_value[799] == nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, 799, nValue,
        product_register_value[799], this->m_hWnd, IDC_EDIT_DTERM, _T("DTERM"));
}


void CParameterDlg::OnEnKillfocusEditSamplingInterval()
{
    // TODO: 在此添加控件通知处理程序代码
    if (g_ParamLevel == 1)
        return;

    CString strText;
    m_edit_sampling_interval.GetWindowText(strText);
    int nValue = (int)(_wtoi(strText));


    if (product_register_value[800] == nValue)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, 800, nValue,
        product_register_value[800], this->m_hWnd, IDC_EDIT_DTERM, _T("Sampling Interval"));
}


void CParameterDlg::OnBnClickedButtonParameterExt()
{
    // TODO: 在此添加控件通知处理程序代码
    CParameterExtDlg dlg;
    dlg.DoModal();
}

HANDLE hkillfocu = NULL;
DWORD WINAPI hReadKillFocsData(LPVOID pParam)
{
    CParameterDlg* pdlg = (CParameterDlg*)pParam;
    Sleep(1500);
    int read_ret;
    read_ret = Read_Multi(g_tstat_id, &product_register_value[300], 300, 100, 3);
    if (read_ret >= 0)
    {
        ::PostMessage(pdlg->m_hWnd, WM_KILLFOCS_READ_DATA, 0, 0);
    }
    hkillfocu = NULL;
    return true;
}
//Day setpoint 

void CParameterDlg::OnEnKillfocusEdit31()
{
    UpdateData();
    if ((short)product_register_value[MODBUS_DAY_SETPOINT] == m_setpoint * 10)	//Add this to judge weather this value need to change.
        return;
    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_DAY_SETPOINT, m_setpoint * 10,
        product_register_value[MODBUS_DAY_SETPOINT], this->m_hWnd, IDC_EDIT31, _T("DAY SETPOINT"));
    product_register_value[MODBUS_DAY_SETPOINT] = m_setpoint * 10;
    if(hkillfocu == NULL)
        hkillfocu = CreateThread(NULL, NULL, hReadKillFocsData, this, NULL, 0);
}

//Day Heat SP
void CParameterDlg::OnEnKillfocusEdit37()
{
    UpdateData();
    if (product_register_value[MODBUS_DAY_HEATING_SETPOINT] == m_heatsp * 10)	//Add this to judge weather this value need to change.
        return;


    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_DAY_HEATING_SETPOINT, m_heatsp * 10,
        product_register_value[MODBUS_DAY_HEATING_SETPOINT], this->m_hWnd, IDC_EDIT37, _T("DAY HEATING SETPOINT"));
    product_register_value[MODBUS_DAY_HEATING_SETPOINT] = m_heatsp * 10;
    if (hkillfocu == NULL)
        hkillfocu = CreateThread(NULL, NULL, hReadKillFocsData, this, NULL, 0);
}


void CParameterDlg::OnEnKillfocusEditCspd()
{
    UpdateData();
    if (product_register_value[MODBUS_DAY_COOLING_SETPOINT] == m_coolsp * 10)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_DAY_COOLING_SETPOINT, m_coolsp * 10,
        product_register_value[MODBUS_DAY_COOLING_SETPOINT], this->m_hWnd, IDC_EDIT_CSPD, _T("DAY COOLING SETPOINT"));

    product_register_value[MODBUS_DAY_COOLING_SETPOINT] = m_coolsp * 10;
    if (hkillfocu == NULL)
        hkillfocu = CreateThread(NULL, NULL, hReadKillFocsData, this, NULL, 0);
}


void CParameterDlg::OnEnKillfocusEditCdbdn()
{
    UpdateData();
    if (product_register_value[MODBUS_DAY_COOLING_DEADBAND] == m_cooldb * 10)	//Add this to judge weather this value need to change.
        return;
    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_DAY_COOLING_DEADBAND, m_cooldb * 10,
        product_register_value[MODBUS_DAY_COOLING_DEADBAND], this->m_hWnd, IDC_EDIT_CDBDN, _T("DAY COOLING DEADBAND"));
    product_register_value[MODBUS_DAY_COOLING_DEADBAND] = m_cooldb * 10;
    if (hkillfocu == NULL)
        hkillfocu = CreateThread(NULL, NULL, hReadKillFocsData, this, NULL, 0);
}


void CParameterDlg::OnEnKillfocusEdit34()
{
    UpdateData();
    if (write_one(g_tstat_id, MODBUS_DAY_HEATING_DEADBAND, m_heatdb * 10)<0)//Modify by Fance  2013 04 11
        MessageBox(_T("Write Register Fail!Please try it again!"), _T("Warning"), MB_OK | MB_ICONINFORMATION);
    else
        product_register_value[MODBUS_DAY_HEATING_DEADBAND] = m_heatdb * 10;

    product_register_value[MODBUS_DAY_HEATING_DEADBAND] = m_heatdb * 10;
    if (hkillfocu == NULL)
        hkillfocu = CreateThread(NULL, NULL, hReadKillFocsData, this, NULL, 0);

}


void CParameterDlg::OnEnKillfocusEdit32()
{
    UpdateData();

    if ((short)product_register_value[MODBUS_NIGHT_SETPOINT] == m_setpointN * 10)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_NIGHT_SETPOINT, m_setpointN * 10,
        product_register_value[MODBUS_NIGHT_SETPOINT], this->m_hWnd, IDC_EDIT32, _T("NIGHT SETPOINT"));

    product_register_value[MODBUS_NIGHT_SETPOINT] = m_setpointN * 10;
    if (hkillfocu == NULL)
        hkillfocu = CreateThread(NULL, NULL, hReadKillFocsData, this, NULL, 0);
}


void CParameterDlg::OnEnKillfocusEdit38()
{
    UpdateData();
    if (product_register_value[MODBUS_NIGHT_HEATING_SETPOINT] == m_heatspN * 10)
        return;

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_NIGHT_HEATING_SETPOINT, m_heatspN * 10,
        product_register_value[MODBUS_NIGHT_HEATING_SETPOINT], this->m_hWnd, IDC_EDIT38, _T("NIGHT HEATING SETPOINT"));
    product_register_value[MODBUS_NIGHT_HEATING_SETPOINT] = m_heatspN * 10;
    if (hkillfocu == NULL)
        hkillfocu = CreateThread(NULL, NULL, hReadKillFocsData, this, NULL, 0);

}



void CParameterDlg::OnEnKillfocusEditCspnn()
{
    UpdateData();
    if (product_register_value[MODBUS_NIGHT_COOLING_SETPOINT] == m_coolspN * 10)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_NIGHT_COOLING_SETPOINT, m_coolspN * 10,
        product_register_value[MODBUS_NIGHT_COOLING_SETPOINT], this->m_hWnd, IDC_EDIT_CSPNN, _T("NIGHT COOLING SETPOINT"));
    product_register_value[MODBUS_NIGHT_COOLING_SETPOINT] = m_coolspN * 10;
    if (hkillfocu == NULL)
        hkillfocu = CreateThread(NULL, NULL, hReadKillFocsData, this, NULL, 0);
}


//Recode by Fance
void CParameterDlg::OnEnKillfocusEditCdbnn()
{

    UpdateData();
    if (product_register_value[MODBUS_NIGHT_COOLING_DEADBAND] == m_cooldbN * 10)	//Add this to judge weather this value need to change.
        return;
    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_NIGHT_COOLING_DEADBAND, m_cooldbN * 10,
        product_register_value[MODBUS_NIGHT_COOLING_DEADBAND], this->m_hWnd, IDC_EDIT_CDBNN, _T("NIGHT COOLING DEADBAND"));
    product_register_value[MODBUS_NIGHT_COOLING_DEADBAND] = m_cooldbN * 10;
    if (hkillfocu == NULL)
        hkillfocu = CreateThread(NULL, NULL, hReadKillFocsData, this, NULL, 0);
}



void CParameterDlg::OnEnKillfocusEdit35()
{
    UpdateData();
    if (product_register_value[MODBUS_NIGHT_HEATING_DEADBAND] == m_heatdbN * 10)	//Add this to judge weather this value need to change.
        return;

    Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_NIGHT_HEATING_DEADBAND, m_heatdbN * 10,
        product_register_value[MODBUS_NIGHT_HEATING_DEADBAND], this->m_hWnd, IDC_EDIT35, _T("NIGHT HEATING DEADBAND"));

    product_register_value[MODBUS_NIGHT_HEATING_DEADBAND] = m_heatdbN * 10;
    if (hkillfocu == NULL)
        hkillfocu = CreateThread(NULL, NULL, hReadKillFocsData, this, NULL, 0);
}






#include "TstatIconSetting.h"
void CParameterDlg::OnBnClickedButtonIconSetting()
{
    // TODO: 在此添加控件通知处理程序代码
    CTstatIconSetting dlg;
    dlg.DoModal();
}
