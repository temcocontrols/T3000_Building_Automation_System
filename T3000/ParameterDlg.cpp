// ParameterDlg.cpp : implementation file
//
// ParameterDlg.cpp add support tstat 6 7 function by Fance Du.

#include "stdafx.h"
#include "T3000.h"
#include "ParameterDlg.h"
#include "globle_function.h"
#include "FreeCoolDlg.h"
#include "OutsidDailog.h"

#include "InputSetDlg.h"
#include "OutputSetDlg.h"
#include "AfxMessageDialog.h"
#include "OutPutDlg.h"

#include "MainFrm.h"

#include "Dialog_Progess.h"//tstat6


#include "T3000RegAddress.h"
extern int Mdb_Adress_Map;
// CParameterDlg dialog

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
	//memset(&multi_register_value[0],0,sizeof(multi_register_value));
	m_version=1.0;
	pProgess = new CDialog_Progess(this,1,10);
	strparamode = str;

}

CParameterDlg::~CParameterDlg()
{
}

void CParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IDADDRESSEDIT, m_idAdressEdit);
	DDX_Control(pDX, IDC_BRAUDRATECOMBO, m_braudRateCombox);
	DDX_Control(pDX, IDC_COMBO2, m_keySelectCombox);
	DDX_Control(pDX, IDC_POWERMODELCOMBO, m_powerupModelCombox);
	DDX_Control(pDX, IDC_DISPLAYCOMBO, m_displayCombox);
	DDX_Control(pDX, IDC_KEYPADLOCKCOMBO, m_keyLockCombox);
	DDX_Control(pDX, IDC_SEQUENCECOMBOX, m_SequenceCombox);
	DDX_Control(pDX, IDC_AUTOONLYCOMBO, m_autoOnlyCombox);
	DDX_Control(pDX, IDC_VALUPOSEDIT, m_value_percentEdit);
	DDX_Control(pDX, IDC_VALVEEDIT, m_valveEdit);
	DDX_Control(pDX, IDC_HCCHANGECOMBO, m_hcChangeCombox);
	DDX_Control(pDX, IDC_POWSETPOMITEDIT, m_powerSetPointEdit);
	DDX_Control(pDX, IDC_STINCREAEDIT, m_setpointIncreasement);
	DDX_Control(pDX, IDC_DEF_SETPOINT_EDT, m_defSetPointEdit);
	DDX_Control(pDX, IDC_SETPOINTCTLEDIT, m_setPointCtrlEdit);
	DDX_Control(pDX, IDC_INFILEREDIT, m_inputFilterEdit);
	DDX_Control(pDX, IDC_CYCLEEDIT, m_cycledlayEdit);
	DDX_Control(pDX, IDC_TEMPALARMEDIT, m_tempAlarmEdit);
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
}

BEGIN_MESSAGE_MAP(CParameterDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CParameterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_REFRESHBUTTON, &CParameterDlg::OnBnClickedRefreshbutton)
	ON_BN_CLICKED(IDC_ENABLEIDBUTTON, &CParameterDlg::OnBnClickedEnableidbutton)
	ON_CBN_SELENDCANCEL(IDC_BRAUDRATECOMBO, &CParameterDlg::OnCbnSelendcancelBraudratecombo)
	ON_CBN_SELCHANGE(IDC_POWERMODELCOMBO, &CParameterDlg::OnCbnSelchangePowermodelcombo)
	ON_CBN_SELCHANGE(IDC_KEYPADLOCKCOMBO, &CParameterDlg::OnCbnSelchangeKeypadlockcombo)
	ON_CBN_SELCHANGE(IDC_DISPLAYCOMBO, &CParameterDlg::OnCbnSelchangeDisplaycombo)
	ON_CBN_SELCHANGE(IDC_AUTOONLYCOMBO, &CParameterDlg::OnCbnSelchangeAutoonlycombo)

	ON_CBN_SELCHANGE(IDC_KEYPADSELECT, &CParameterDlg::OnCbnSelchangekeypadcombo)

	ON_EN_KILLFOCUS(IDC_IDADDRESSEDIT, &CParameterDlg::OnEnKillfocusIdaddressedit)
	ON_WM_DESTROY()
	//ON_CBN_KILLFOCUS(IDC_KEYPADSELECT, &CParameterDlg::OnCbnKillfocusKeypadselect)
	ON_EN_KILLFOCUS(IDC_VALUPOSEDIT, &CParameterDlg::OnEnKillfocusValuposedit)
	ON_CBN_SELCHANGE(IDC_SEQUENCECOMBOX, &CParameterDlg::OnCbnSelchangeSequencecombox)
	ON_EN_KILLFOCUS(IDC_VALVEEDIT, &CParameterDlg::OnEnKillfocusValveedit)
	ON_CBN_SELCHANGE(IDC_HCCHANGECOMBO, &CParameterDlg::OnCbnSelchangeHcchangecombo)
	ON_EN_KILLFOCUS(IDC_POWSETPOMITEDIT, &CParameterDlg::OnEnKillfocusPowsetpomitedit)
	ON_EN_KILLFOCUS(IDC_STINCREAEDIT, &CParameterDlg::OnEnKillfocusStincreaedit)
	ON_EN_KILLFOCUS(IDC_DEF_SETPOINT_EDT, &CParameterDlg::OnEnKillfocusDefSetpointEdt)
	ON_EN_KILLFOCUS(IDC_SETPOINTCTLEDIT, &CParameterDlg::OnEnKillfocusSetpointctledit)
	ON_EN_KILLFOCUS(IDC_INFILEREDIT, &CParameterDlg::OnEnKillfocusInfileredit)
	ON_EN_KILLFOCUS(IDC_CYCLEEDIT, &CParameterDlg::OnEnKillfocusCycleedit)
	ON_EN_KILLFOCUS(IDC_TEMPALARMEDIT, &CParameterDlg::OnEnKillfocusTempalarmedit)
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
END_MESSAGE_MAP()


UINT BackparaFreshProc(LPVOID pParam)
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
		//pdlg->Refresh();
		//if ((pdlg->strparamode.CompareNoCase(_T("Tstat6")) == 0)||(pdlg->strparamode.CompareNoCase(_T("Tstat7")) == 0))
		//	pdlg->Refresh6();
		//else
		//	pdlg->Refresh();
		pdlg->Reflesh_ParameterDlg();
	}

	return 0;
}

BOOL CParameterDlg::OnInitDialog()
{
	//20120420
	//pProgess = new CDialog_Progess(this,1,10);
	//创建对话框窗口
	pProgess->Create(IDD_DIALOG10_Progress, this);

	//居中显示
//	pProgess->CenterWindow();//2.5.0.98
	pProgess->ShowProgress(0,0);
	//显示对话框窗口
	pProgess->ShowWindow(SW_SHOW);
	//20120420

//2.5.0.98
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	RECT RECT_SET1;
	pMain->GetClientRect(&RECT_SET1);
	pProgess->MoveWindow(RECT_SET1.left+400,RECT_SET1.bottom-19,RECT_SET1.right/2+20,20);
	pProgess->ShowProgress(4,40);

	CDialog::OnInitDialog();
	GetDlgItem(IDC_TEMPALARMEDIT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATICALARM)->ShowWindow(SW_HIDE);
	m_setPointCtrlEdit.ShowWindow(SW_HIDE);

	if(g_ParamLevel==1)
	{
		CAfxMessageDialog dlg;
		CString strPromption;
		strPromption.LoadString(IDS_STRNOPRIVILEGE);
		dlg.SetPromtionTxt(strPromption);
		dlg.DoModal();
	}
	m_idAdressEdit.EnableWindow(FALSE);


	if(product_register_value[MODBUS_MODE_OUTPUT4]==2)//283
	{
		write_one(g_tstat_id,MODBUS_TIMER_SELECT,3);//327 
		CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMERON_STATIC);
		ptimerout->SetWindowText(_T("Out4"));
	}
	else
	{
		CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMERON_STATIC);
		ptimerout->SetWindowText(_T("Timer On"));
	}

	// initialize PID2 combo box

	pProgess->ShowProgress(5,50);
	InitPID2ComboBox();
	pProgess->ShowProgress(7,70);
	//Refresh();//Changed by Fance ,When initial the dialog ,it should be mapping by the product.
	////if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))
	////	Refresh6();
	////else
	////	Refresh();
	Reflesh_ParameterDlg();

	pProgess->ShowProgress(8,80);

	pParamBackFresh = AfxBeginThread(BackparaFreshProc,this);
	pParamBackFresh->m_bAutoDelete =FALSE;

	pProgess->ShowProgress(10,100);

	pProgess->ShowWindow(SW_HIDE);//tstat6
	delete pProgess;
	pProgess=NULL;
	if (MDAY == 0)
		GetDlgItem(IDC_COMBO1)->SetWindowText(_T("1 SP"));
	else
		GetDlgItem(IDC_COMBO1)->SetWindowText(_T("2 SP"));
	
	if (MNIGHT == 0)
		GetDlgItem(IDC_COMBO4)->SetWindowText(_T("1 SP"));
	else
		GetDlgItem(IDC_COMBO4)->SetWindowText(_T("2 SP"));

	CString str;//add by Fance. T5 also support this function
	str.Format(_T("%d"),product_register_value[MODBUS_VALVE_TRAVEL_TIME]);	//279  243  
	GetDlgItem(IDC_EDIT_ValueTravelTime)->SetWindowText(str);

	//////////////////////////////////////////////////////////////////////////
	if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))
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
		GetDlgItem(IDC_EDIT_CSPWN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT39)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ValueTravelTime)->EnableWindow(TRUE);
		/*CString str;
		str.Format(_T("%d"),newtstat6[243]);
		GetDlgItem(IDC_EDIT_ValueTravelTime)->SetWindowText(str);*/	//Fance comments  TSTAT 5 also support this function ,so recode it


	}
	else
	{
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
		GetDlgItem(IDC_EDIT_CSPWN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT39)->EnableWindow(FALSE);



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


// 

	//if (parameterSet == 0)
	//{
	  m_coolsp= product_register_value[MODBUS_DAY_COOLING_SETPOINT]/10;//348

	 m_cooldb=product_register_value[MODBUS_DAY_COOLING_DEADBAND]/10;//346
	 m_setpoint =product_register_value[MODBUS_DAY_SETPOINT]/10;//345
	 m_heatdb =product_register_value[MODBUS_DAY_HEATING_DEADBAND]/10;//347
	 m_heatsp =product_register_value[MODBUS_DAY_HEATING_SETPOINT]/10;//349

	 // 		350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
	 // 		351	1	Low byte		spare
	 // 		352	1	Low byte	W/R	(Night)Unoccupied heating setpoint dead band , heating deadband for the night (OFF) mode. Units of 1 deg.
	 // 		353	1	Low byte	W/R	(Night)Unoccupied cooling setpoint dead band , cooling deadband for the night (OFF) mode. Units of 1 deg.
	 // 		354	2	Full	W/R	(Night)Unoccupied heating setpoint
	 // 		355	2	Full	W/R	(Night)Unoccupied cooling setpoint
	 m_coolspN=product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]/10;//355
	 m_cooldbN=product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10;//353
	 m_setpointN=product_register_value[MODBUS_NIGHT_SETPOINT]/10;//350
	 m_heatdbN=product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10;//352
	 m_heatspN=product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]/10;//354

	 if (tstat6flex[0] == 0)
	 {
		 memcpy(tstat6flex,m_slidertsta6,sizeof(m_slidertsta6));
	 }

	// parameterSet = 1;

	//}else		//Annul by Fance  
	//{
	//	m_coolsp= tstat6flex[3];// DcoolSP	3
	//	m_cooldb=tstat6flex[1];//DcoolDB	1
	//	m_setpoint = tstat6flex[0];// DaySP		0
	//	m_heatdb = tstat6flex[2];//DheatDB	2
	//	m_heatsp = tstat6flex[4];//DheatSP	4

	//	// 		350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
	//	// 		351	1	Low byte		spare
	//	// 		352	1	Low byte	W/R	(Night)Unoccupied heating setpoint dead band , heating deadband for the night (OFF) mode. Units of 1 deg.
	//	// 		353	1	Low byte	W/R	(Night)Unoccupied cooling setpoint dead band , cooling deadband for the night (OFF) mode. Units of 1 deg.
	//	// 		354	2	Full	W/R	(Night)Unoccupied heating setpoint
	//	// 		355	2	Full	W/R	(Night)Unoccupied cooling setpoint
	//	m_coolspN=tstat6flex[9];// NcoolSP	9
	//	m_cooldbN=tstat6flex[7];//NcoolDB	7
	//	m_setpointN=tstat6flex[5];//NightSP	5 
	//	m_heatdbN=tstat6flex[6];// NheatDB	6 
	//	m_heatspN=tstat6flex[8];//NheatSP	8

	//}

	 UpdateData(FALSE);

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

	 //tstat6flex

	 //Setting the refresh button,add by Fan 
	 HICON hIcon = NULL; 
	 HINSTANCE hInstResource    = NULL; 
	 hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(IDI_ICON_REFRESH), RT_GROUP_ICON); 
	 hIcon = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(IDI_ICON_REFRESH), IMAGE_ICON, 16, 16, 0); 
	 ((CButton *)GetDlgItem(IDC_REFRESHBUTTON))->SetIcon(hIcon);

	//////////////////////////////////////////////////////////////////////////
	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

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
		m_inputSelect2.AddString(_T("None"));
		m_inputSelect2.AddString(g_strInName1);
		m_inputSelect2.AddString(g_strInName2);
		m_inputSelect2.AddString(_T("Humidity"));
	}


}

// CParameterDlg message handlers

void CParameterDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CParameterDlg::OnBnClickedRefreshbutton()
{
// 	int i;
// 	for(i=0;i<7;i++)
// 	{
// 		register_critical_section.Lock();
// 		Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64);
// 		register_critical_section.Unlock();
// 	}
	////if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))
	////	Refresh6();
	////else
	////	Refresh();


	Reflesh_ParameterDlg();

}

void CParameterDlg::OnBnClickedEnableidbutton()
{
// 	m_idAdressEdit.EnableWindow(TRUE);
// 	// TODO: Add your control notification handler code here

	if(m_idAdressEdit.IsWindowEnabled())
		m_idAdressEdit.EnableWindow(FALSE);
	else
		m_idAdressEdit.EnableWindow(TRUE);
}



void CParameterDlg::OnCbnSelchangeAutoonlycombo()
{
	int nindext=m_autoOnlyCombox.GetCurSel();

	if(write_one(g_tstat_id,MODBUS_AUTO_ONLY,nindext)<0)	// 129  107
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		m_autoOnlyCombox.SetCurSel(product_register_value[MODBUS_AUTO_ONLY]);
	}
	else
		product_register_value[MODBUS_AUTO_ONLY] = nindext;

	
	// TODO: Add your control notification handler code here
}

//Annule by Fance recode
//void CParameterDlg::Refresh()
//{
//	CString strTemp;
//	strTemp.Format(_T("%d"),g_tstat_id);
//	m_idAdressEdit.SetWindowText(strTemp);
//	CString strUnit=GetTempUnit();//Unit string.
//
//	if(product_register_value[MODBUS_BAUDRATE]>=0&&product_register_value[MODBUS_BAUDRATE]<=1)//185
//		m_braudRateCombox.SetCurSel(product_register_value[MODBUS_BAUDRATE]);//185
//	else
//		m_braudRateCombox.SetCurSel(1);
//
//
//
//	if (product_register_value[MODBUS_PRODUCT_MODEL] == 18)//tstat5g	//7
//	{
//		CString str;
//		//	str.Format(_T("%d"),multi_register_value[275]);
//		//GetDlgItem(IDC_EDIT_PID2OFFSETPOINT)->SetWindowText(str);
//
//		//CString strUnit=GetTempUnit();//Unit string.
//		str.Format(_T("%.1f"),product_register_value[_P("MODBUS_UNIVERSAL_NIGHTSET")]/10.0);			//275 
//		GetDlgItem(IDC_EDIT_PID2OFFSETPOINT)->SetWindowText(str+strUnit);
//	}
//
//
//
//
//	int nItem;
//	nItem = product_register_value[MODBUS_KEYPAD_SELECT];//128
//	switch(nItem)
//	{
//		case 0:nItem=0;break;
//		case 1:nItem=2;break;
//		case 2:nItem=4;break;
//		case 3:nItem=5;break;
//		case 4:nItem=3;break;
//		case 5:nItem=1;break;
//		case 6:nItem=6;break;
//		case 7:nItem=7;break;
//		case 8:nItem=8;break;
//		default :nItem=0;break;
//	}
//	m_keySelectCombox.SetCurSel(nItem);
//
//	m_powerupModelCombox.SetCurSel(product_register_value[MODBUS_POWERUP_MODE]);//127
//
//	strTemp.Format(_T("%d"),product_register_value[MODBUS_VALVE_PERCENT]);//285
//	m_value_percentEdit.SetWindowText(strTemp);
//
//
//
//	m_displayCombox.SetCurSel(product_register_value[_P("MODBUS_DISPLAY")]);//203
//	m_keyLockCombox.SetCurSel(product_register_value[MODBUS_SPECIAL_MENU_LOCK]);//133
//	
//	m_autoOnlyCombox.SetCurSel(product_register_value[MODBUS_AUTO_ONLY]);//129
//
//	m_SequenceCombox.SetCurSel(product_register_value[MODBUS_SEQUENCE]);//118
//
//
//	strTemp.Format(_T("%.1f"),product_register_value[_P("MODBUS_VALVE_TRAVEL_TIME")]/10.0);//279
//	m_valveEdit.SetWindowText(strTemp);
//
//
//	m_hcChangeCombox.SetCurSel(product_register_value[MODBUS_HEAT_COOL_CONFIG]);//214
//
//	strTemp.Format(_T("%d"),product_register_value[_P("MODBUS_POWERUP_SETPOINT")]);//126
//	m_powerSetPointEdit.SetWindowText(strTemp);
//   
//
//	strTemp.Format(_T("%.1f"),product_register_value[_P("MODBUS_SETPOINT_INCREASE")]/10.0);//293
//	m_setpointIncreasement.SetWindowText(strTemp);
//
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_DEFAULT_SETPOINT")]);//338
//	m_defSetPointEdit.SetWindowText(strTemp);
//
////	strTemp.Format(_T("%d"),multi_register_value[339]);
//	if(multi_register_value[_G("MODBUS_SETPOINT_CONTROL")]>=0&&multi_register_value[_G("MODBUS_SETPOINT_CONTROL")]<3)//339
//	{
//		m_occupiedSetPointModeCmbox.SetCurSel(multi_register_value[_G("MODBUS_SETPOINT_CONTROL")]);//339
//	}
//		
//	if(multi_register_value[_G("MODBUS_SETPOINT_CONTROL")]==0||multi_register_value[_G("MODBUS_SETPOINT_CONTROL")]==2)//339
//	{
//		m_defSetPointEdit.EnableWindow(FALSE);
//	}
//	if(multi_register_value[_G("MODBUS_SETPOINT_CONTROL")]==1)//339
//	{
//		m_defSetPointEdit.EnableWindow(TRUE);
//	}
//
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_FILTER")]);	//213
//	m_inputFilterEdit.SetWindowText(strTemp);
//
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_CYCLING_DELAY")]);	//201
//	m_cycledlayEdit.SetWindowText(strTemp);
//
//
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_TIMER_ON")]);//301
//	m_timerOnEdit.SetWindowText(strTemp);
//
//	
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_TIMER_OFF")]);//302
//	m_timerOffEdit.SetWindowText(strTemp);
//
//	m_uniteCombox.SetCurSel(multi_register_value[_G("MODBUS_TIMER_UNITS")]);//303
//
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_ROTATION_TIME_LEFT")]);//333
//	m_timerLeft.SetWindowText(strTemp);
//
//
//
//	if(multi_register_value[_G("MODBUS_MODE_OUTPUT4")]==2||multi_register_value[_G("MODBUS_MODE_OUTPUT5")]==2)//283  //284
//	{
//		CComboBox* pCombox=(CComboBox*)GetDlgItem(IDC_TIMERDELECTCOMBO);
//		int	indext=3;
//		write_one(g_tstat_id,327,indext);
//		pCombox->SetCurSel(3);
//
//	}
//	else
//	{
//
//	}
//
//	//if(multi_register_value[283]==2)
//	//{
//	//		
//	//	CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMERON_STATIC);
//	//	ptimerout->SetWindowText(_T("Out4:"));
//	//}
//	//else
//	//{
//	//	CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMERON_STATIC);
//	//	ptimerout->SetWindowText(_T("Timer On:"));
//	//}
//
//	//if(multi_register_value[284]==2)
//	//{
//	//	
//	//	CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMEOFF_STATIC);
//	//	ptimerout->SetWindowText(_T("Out5:"));
//
//	//}
//	//else
//	//{
//	//	CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMEOFF_STATIC);
//	//	ptimerout->SetWindowText(_T("Timer Off:"));
//	//}
//
//
//	m_timerSelectCombox.SetCurSel(multi_register_value[_G("MODBUS_TIMER_SELECT")]);	//327
//
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_OVERRIDE_TIMER_LEFT")]);			//212
//	m_TimeEdit.SetWindowText(strTemp);
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_OVERRIDE_TIMER")]);			//211
//	m_OverRideEdit.SetWindowText(strTemp);
//
//	if (multi_register_value[_G("MODBUS_TEMP_SELECT")]<1)				//111
//	{
//		m_InputSelect1.SetCurSel(0);
//	}else if(multi_register_value[_G("MODBUS_TEMP_SELECT")]>3)			//111
//	{
//		m_InputSelect1.SetCurSel(2);
//
//	}else
//	{
//		m_InputSelect1.SetCurSel(multi_register_value[_G("MODBUS_TEMP_SELECT")]-1);	//111
//	}
//
//
//	if (multi_register_value[MODBUS_INPUT1_SELECT]<0)	//241
//	{
//		m_inputSelect2.SetCurSel(0);
//	}else if (multi_register_value[MODBUS_INPUT1_SELECT]>5)	//241
//	{
//		m_inputSelect2.SetCurSel(5);
//	}else
//	{
//		m_inputSelect2.SetCurSel(multi_register_value[MODBUS_INPUT1_SELECT]);//241
//	}
//	
//	
//	strTemp.Format(_T("%.1f"),multi_register_value[_G("MODBUS_TEMPRATURE_CHIP")]/10.0);
//	m_inputvalue1.SetWindowText(strTemp+strUnit);
//	
//	//////////////////////////////////////////////////////////////////////////
//	// PID2 m_inputValue2 m_inputSelect2 关联设置
//// 
//// 	if(multi_register_value[7]== 18)//18是tstat5G
//// 	{
//// 		if(multi_register_value[241]==2) // input1
//// 		{	
//// 			if(multi_register_value[7]== PM_PRESSURE)  // pressure
//// 			{
//// 				strTemp.Format(_T("%.1f W.C"),multi_register_value[180]/100.0);
//// 			}
//// 			else
//// 			{
//// 				if(multi_register_value[188]==4||multi_register_value[188]==1)
//// 				{
//// 					strTemp.Format(_T("%.1f"),(float)multi_register_value[180]/10);
//// 					strTemp=strTemp+strUnit;
//// 				}
//// 				if (strUnit==""||strUnit=="%")
//// 				{
//// 					strTemp.Format(_T("%d"),multi_register_value[180]);
//// 				}
//// 				if(multi_register_value[180]==3)
//// 				{
//// 					if(multi_register_value[180]==0)
//// 						strTemp=_T("OFF");
//// 					if(multi_register_value[180]==1)
//// 						strTemp=_T("ON");
//// 				}
//// 			}
//// 			m_inputValue2.SetWindowText(strTemp);
//// 		}else if(multi_register_value[241]==1) // input2 //m_inputvalue1
//// 		{
//// 			if(multi_register_value[189]==4||multi_register_value[189]==1)
//// 			{
//// 				strTemp.Format(_T("%.1f"),(float)multi_register_value[181]/10.0);	
//// 				strTemp=strTemp+strUnit;
//// 			}
//// 			if (strUnit==""||strUnit=="%")
//// 			{
//// 				strTemp.Format(_T("%d"),multi_register_value[181]);
//// 			}
//// 			if(multi_register_value[189]==3)
//// 			{
//// 				if(multi_register_value[181]==0)
//// 					strTemp=_T("OFF");
//// 				if(multi_register_value[181]==1)
//// 					strTemp=_T("ON");
//// 			}
//// 			m_inputValue2.SetWindowText(strTemp);
//// 		}
//// 		else //if(multi_register_value[241]!=1)
//// 		{
//// 			m_inputValue2.SetWindowText(_T("UNUSED"));
//// 			//		m_hotelCoolEdt.SetWindowText(_T("N/A"));
//// 		}
//// 
//// 		//0914
//// 		// 	if (multi_register_value[241]==3) // humidity
//// 		// 	{
//// 		// 		strTemp.Format(_T("%.1f%%"),multi_register_value[422]/10.0);
//// 		// 		m_inputValue2.SetWindowText(strTemp);
//// 		// 	}else
//// 		// 	{
//// 		// 		m_inputValue2.SetWindowText(_T("0"));
//// 		// 
//// 		// 	}
//// 
//// 		//
//// 
//// 	}else
//// 	{
//		if(multi_register_value[241]==1) // input1
//		{	
//			if(multi_register_value[7]== PM_PRESSURE)  // pressure
//			{
//				strTemp.Format(_T("%.1f W.C"),multi_register_value[180]/100.0);
//			}
//			else
//			{
//		 if(multi_register_value[MODBUS_ANALOG_IN1]==4||multi_register_value[MODBUS_ANALOG_IN1]==1)	//188   188
//		 {
//			 strTemp.Format(_T("%.1f"),(float)multi_register_value[MODBUS_EXTERNAL_SENSOR_0]/10);	//180
//			 strTemp=strTemp+strUnit;
//		 }
//		 if (strUnit==""||strUnit=="%")
//		 {
//			 strTemp.Format(_T("%d"),multi_register_value[MODBUS_EXTERNAL_SENSOR_0]);//180
//		 }
//		 if(multi_register_value[MODBUS_EXTERNAL_SENSOR_0]==3)//180
//		 {
//			 if(multi_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//180
//				 strTemp=_T("OFF");
//			 if(multi_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//180
//				 strTemp=_T("ON");
//		 }
//			}
//			m_inputValue2.SetWindowText(strTemp);
//		}//else //if(multi_register_value[241]!=2)
//		//{
//		//	m_inputValue2.SetWindowText(_T("UNUSED"));
//		//	}
//
//
//		else if(multi_register_value[241]==2) // input2 //m_inputvalue1
//		{
//			if(multi_register_value[189]==4||multi_register_value[189]==1)
//			{
//				strTemp.Format(_T("%.1f"),(float)multi_register_value[181]/10.0);	
//				strTemp=strTemp+strUnit;
//			}
//	//if (strUnit==""||strUnit=="%")//0914
//			{
//				//strTemp.Format(_T("%d"),multi_register_value[181]);//0914
//				strTemp.Format(_T("%.1f"),(float)multi_register_value[181]/10);
//				strTemp=strTemp+strUnit;
//			}
//			if(multi_register_value[189]==3)
//			{
//				if(multi_register_value[181]==0)
//					strTemp=_T("OFF");
//				if(multi_register_value[181]==1)
//					strTemp=_T("ON");
//			}
//			m_inputValue2.SetWindowText(strTemp);
//		}
//		else //if(multi_register_value[241]!=1)
//		{
//			m_inputValue2.SetWindowText(_T("UNUSED"));
//			//		m_hotelCoolEdt.SetWindowText(_T("N/A"));
//		}
//
//		//0914
//		// 	if (multi_register_value[241]==3) // humidity
//		// 	{
//		// 		strTemp.Format(_T("%.1f%%"),multi_register_value[422]/10.0);
//		// 		m_inputValue2.SetWindowText(strTemp);
//		// 	}else
//		// 	{
//		// 		m_inputValue2.SetWindowText(_T("0"));
//		// 
//		// 	}
//
//		//
////	}
//
//	//////////////////////////////////////////////////////////////////////////
//
// float m_fFirmwareVersion=get_curtstat_version();//0912
// 	if (multi_register_value[7] == PM_TSTAT5E)//0912
// 	{
//		
// 		short nOccupied = multi_register_value[_G("MODBUS_INFO_BYTE",2)];  // Day setpoint option  //184
// 		BOOL bOccupied = nOccupied & 0x0001;
// 		if (bOccupied)  // day  - Occupied
// 		{
//			if(m_fFirmwareVersion >= 35.4)
//			{
//				m_application_ctrl.SetCurSel(multi_register_value[_G("MODBUS_DAYSETPOINT_OPTION",2)]);	//423
//			}
//			else
//			{
//				m_application_ctrl.SetCurSel(multi_register_value[454]);	//454  ????????  Don't know why
//				// 5E 以及以后的型号;
//			}
// 
// 		}
// 		else
// 		{
// 			m_application_ctrl.SetCurSel(multi_register_value[_G("MODBUS_APPLICATION",2)]);  //125
// 		}
//		
// 	}
// 	else
//	{
//			m_application_ctrl.SetCurSel(multi_register_value[_G("MODBUS_APPLICATION")]); //125
//	}
//
//
//	UpdateCoolingandHeatingData();
//
//
//
//	int index = 0;
//	index = m_application_ctrl.GetCurSel();
//	switch(index)
//	{
//	case 0:
//// 		CWnd *pSPWnd1 = GetDlgItem(IDC_STATIC_CSP);
//// 		pSPWnd1->SetWindowText(_T("Cooling Set Point"));
//// 		CWnd *pSPWnd2 = GetDlgItem(IDC_STATIC_HSP);
//// 		pSPWnd2->SetWindowText(_T("Heating Set Point"));
//		GetDlgItem(IDC_STATIC_CSP)->SetWindowText(_T("Cooling Set Point"));
//		GetDlgItem(IDC_STATIC_HSP)->SetWindowText(_T("Heating Set Point"));
//
//		break;
//	case 1:
//		GetDlgItem(IDC_STATIC_CSP)->SetWindowText(_T("Cooling DB"));
//		GetDlgItem(IDC_STATIC_HSP)->SetWindowText(_T("Heating DB"));
//		break;
//
//
//	}
//	//int GetCursor( ) const;
//
//
//	//OnCbnSelchangeEapplication();
//
//// 	strTemp.Empty();
//// 	int nSelect=multi_register_value[241];
//// 	if(nSelect==1)
//// 	{
//// 		if(multi_register_value[188]==4||multi_register_value[188]==1)
//// 		{
//// 			strTemp.Format(_T("%.1f"),(float)multi_register_value[180]/10);
//// 			strTemp=strTemp+strUnit;
//// 		}
//// 		if (strUnit==""||strUnit=="%")
//// 		{
//// 			strTemp.Format(_T("%d"),multi_register_value[180]);
//// 		}
//// 		if(multi_register_value[180]==3)
//// 		{
//// 			if(multi_register_value[180]==0)
//// 				strTemp=_T("OFF");
//// 			if(multi_register_value[180]==1)
//// 				strTemp=_T("ON");
//// 		}
//// 		
//// 		m_inputValue2.SetWindowText(strTemp);
//// 	}
//// 	if(nSelect==2)
//// 	{	
//// 		if(multi_register_value[189]==4||multi_register_value[189]==1)
//// 		{
//// 			strTemp.Format(_T("%.1f"),(float)multi_register_value[181]/10.0);	
//// 			strTemp=strTemp+strUnit;
//// 		}
//// 		if (strUnit==""||strUnit=="%")
//// 		{
//// 			strTemp.Format(_T("%d"),multi_register_value[181]);
//// 		}
//// 		if(multi_register_value[189]==3)
//// 		{
//// 			if(multi_register_value[181]==0)
//// 				strTemp=_T("OFF");
//// 			if(multi_register_value[181]==1)
//// 				strTemp=_T("ON");
//// 		}
//// 		m_inputValue2.SetWindowText(strTemp);
//// 	}
//
//	strTemp.Empty();
//	m_version=get_curtstat_version();
//
//// 	if (multi_register_value[7] == PM_TSTAT7)
//// 	{
//// 		strTemp.Format(_T("%.1f"),multi_register_value[380]/10.0);	//	set point 
//// 	}
//// 	else if(m_version<34.9|| multi_register_value[7] == 16)  // 老版本不除10，
//// 	{
//// 		strTemp.Format(_T("%d"),(int)multi_register_value[135]);	//	set point 
//// 	}
//// 	else
//// 	{
//// 		strTemp.Format(_T("%.1f"),multi_register_value[374]/10.0);		// set point for TStat A,B,C,D
//// 	}
//// 	m_pid_setptEdt1.SetWindowText(strTemp);
//
//	strTemp.Empty();
//	strTemp.Format(_T("%.1f"),multi_register_value[_G("MODBUS_UNIVERSAL_SET")]/10.0);	//246
//	m_pid_setptEdt2.SetWindowText(strTemp);
//
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_COOLING_PID")]);	//104
//	m_pid_outputEdt1.SetWindowText(strTemp);
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_PID_UNIVERSAL")]);	//270
//	m_pid_outputEdt2.SetWindowText(strTemp);
//			
//
//	strTemp.Format(_T("%.1f"),(float)multi_register_value[_G("MODBUS_COOLING_PTERM")]/10.0);	//114
//	m_pternEdt1.SetWindowText(strTemp);
//	
//	strTemp.Format(_T("%.1f"),(float)multi_register_value[_G("MODBUS_UNIVERSAL_PTERM")]/10.0);		//244
//	m_ptermEdt2.SetWindowText(strTemp);
//
///*
//	itemp = multi_register_value[115];
//	ftemp = itemp;
//	ftemp /= 10;
//	m_coolingiterm.Format("%.1f",ftemp);
//	*/
//	strTemp.Format(_T("%.1f"),(float)multi_register_value[_G("MODBUS_COOLING_ITERM")]/10.0);	//115
//	m_coolingPitemEdt1.SetWindowText(strTemp);
//
//	strTemp.Format(_T("%.1f"),(float)multi_register_value[_G("MODBUS_UNIVERSAL_ITERM")]/10.0);	//245
//	m_pidPitemEdt2.SetWindowText(strTemp);
//
//
//	strTemp.Empty();
//	strTemp.Format(_T("%.1f"),(float)multi_register_value[_G("MODBUS_UNIVERSAL_SET")]/10.0);	//246
//	strTemp+=strUnit;
//	m_dayOccEdt2.SetWindowText(strTemp);
//	if (multi_register_value[MODBUS_INPUT1_SELECT]==3) // humidity		//241
//	{
//		strTemp.Format(_T("%.1f%%"),multi_register_value[422]/10.0);	//422 Don't know why???
//		m_dayOccEdt2.SetWindowText(strTemp);
//	}
//
//	strTemp.Empty();
//
//
//// 	if (m_version>=SETPOINT_SPECIAL_VERSION)//
//// 	{
//// 		strTemp.Format(_T("%.1f"),multi_register_value[374]/10.0);
//// 	}
//	if((multi_register_value[7] == PM_TSTAT7)||(multi_register_value[7] == PM_TSTAT6))
//	{
//
//		strTemp.Format(_T("%.1f"),multi_register_value[MODBUS_RH_SETPOINT]/10.0);//	set point 380
//
//	}
//	else
//	{
//		if(m_version<34.9 || multi_register_value[7] == PM_TSTAT5E)   // 老版本135，新版本374// 老版本不除10，
//		{
//			strTemp.Format(_T("%d"),(int)multi_register_value[MODBUS_COOLING_SETPOINT]);//135
//		}
//		else
//		{
//			strTemp.Format(_T("%.1f"),multi_register_value[MODBUS_TWO_BYTE_SETPOINT]/10.0);	// set point for TStat A,B,C,D	//374
//		}	
//	}
//
//	m_dayOccEdt1.SetWindowText(strTemp);
//	if (multi_register_value[7] == PM_TSTAT5E)//0911
//	{
//		strTemp.Format(_T("%.1f"),multi_register_value[135]/10.0);//0911	//135
//		m_pid_setptEdt1.SetWindowText(strTemp+strUnit);
//	}
//	else
//	{
//		// unoccupied and office mode, we should recalc the setpoint here	
//		short nOccupied = multi_register_value[_G("MODBUS_INFO_BYTE")];  // Day setpoint option  //184
//		BOOL bOccupied = nOccupied & 0x0001;
//		if (!bOccupied && multi_register_value[_G("MODBUS_APPLICATION")] == 0)  // unoccupied and office mode	//125
//		{ 
//			float temp =float( (multi_register_value[_G("MODBUS_NIGHT_COOLING_SETPOINT")]-multi_register_value[MODBUS_NIGHT_HEATING_SETPOINT]) / 2.0 + multi_register_value[MODBUS_NIGHT_HEATING_SETPOINT] );//183  182  182
//			//strTemp.Format(_T("%.1f"),temp);
//			strTemp.Format(_T("%d"),(int)temp);
//		}
//		m_pid_setptEdt1.SetWindowText(strTemp+strUnit);
//	}
//
//
//
//
//
//
//		strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_MAX_SETPOINT")]);//131
//		strTemp+=strUnit;
//		m_setptHiEdit.SetWindowText(strTemp);
//
//		strTemp.Empty();
//		strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_MIN_SETPOINT")]);//132
//		strTemp+=strUnit;
//		m_setptLoEdit.SetWindowText(strTemp);
//
//		strTemp.Empty();
//		strTemp.Format(_T("%.1f"),multi_register_value[MODBUS_COOLING_DEADBAND]/10.0);	//119
//		strTemp+=strUnit;
//		m_setptCDDEdt1.SetWindowText(strTemp);
//
//		strTemp.Empty();
//		strTemp.Format(_T("%.1f"),multi_register_value[_G("MODBUS_UNIVERSAL_DB_LO")]/10.0);	//243
//		strTemp+=strUnit;
//		m_setptCCDEdt2.SetWindowText(strTemp);
//
//		strTemp.Empty();
//		strTemp.Format(_T("%.1f"),multi_register_value[MODBUS_HEATING_DEADBAND]/10.0);	//120
//		strTemp+=strUnit;
//		m_HeadDEdt1.SetWindowText(strTemp);
//
//		
//		strTemp.Empty();
//		strTemp.Format(_T("%.1f"),multi_register_value[_G("MODBUS_UNIVERSAL_DB_HI")]/10.0);	//242
//		strTemp+=strUnit;
//		m_HeadDEdt2.SetWindowText(strTemp);
//
//
//
//		CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN1);
//		pCbx->SetCurSel(multi_register_value[_G("MODBUS_LCD_SCREEN1",2)]);//400
//		pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN2);
//		pCbx->SetCurSel(multi_register_value[_G("MODBUS_LCD_SCREEN2",2)]);//401
//
//	
//		
//		pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_ROUNDDIS);
//		if(multi_register_value[_G("MODBUS_ROUND_DISPLAY",2)] == 0)//318
//		{
//			pCbx->SetCurSel(0);
//		}
//		else if(multi_register_value[_G("MODBUS_ROUND_DISPLAY",2)] == 1)//318
//		{
//			pCbx->SetCurSel(1);
//		}
//		else if(multi_register_value[_G("MODBUS_ROUND_DISPLAY",2)] == 5)	//318
//		{
//			pCbx->SetCurSel(2);
//		}
//
//
//}


void CParameterDlg::OnEnKillfocusIdaddressedit()
{
	CMainFrame* pPraent=(CMainFrame*)GetParent();
	if(g_ParamLevel==1)
		return;
	
	CString strTemp;
	m_idAdressEdit.GetWindowText(strTemp);
	strTemp.Trim();
	if(strTemp.IsEmpty())
		return;
	int nID=_wtoi(strTemp);
	if(nID<=0||nID>=255)
	{
		AfxMessageBox(_T("Please input a correct ID!"));
		return;
	}
	if(g_tstat_id==nID)
		return;

	_ConnectionPtr m_pConTmp;
	_RecordsetPtr m_pRsTemp;
	m_pConTmp.CreateInstance("ADODB.Connection");
	m_pRsTemp.CreateInstance("ADODB.Recordset");
	m_pConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	
	CString strSql;
	strSql.Format(_T("select * from ALL_NODE where Product_ID ='%s'"),strTemp);
	m_pRsTemp->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
	if(m_pRsTemp->GetRecordCount()>0)
	{
		CString strPop;
		strPop.Format(_T("There is another TSTAT has the ID ='%s', Please input a new one!"),strTemp);
		AfxMessageBox(strPop);
		if(m_pRsTemp->State)
		m_pRsTemp->Close();
		return;
	}
	if(m_pRsTemp->State)
		m_pRsTemp->Close();
	int nRet=0;
			g_bPauseMultiRead = TRUE;	
	nRet=write_one(g_tstat_id, 6,nID);
	if(nRet>0)
	{		
		CString strSerial;
		strSerial.Format(_T("%d"),g_serialNum);
		strSerial.Trim();
		g_tstat_id=nID;

		// 修改product name
		int nProductClassID = 0;
		for(UINT i = 0; i < pPraent->m_product.size(); i++)
		{
			if(pPraent->m_product[i].serial_number== g_serialNum)
			{
				nProductClassID = pPraent->m_product[i].product_class_id;
				break;
			}
		}
		CString strProductName;
		CString strNewID;
		if (nProductClassID != 0)
		{
	
			strNewID.Format(_T("%d"), g_tstat_id);
			strProductName = pPraent->GetDeviceClassName(nProductClassID);
			strProductName = strProductName+_T(":")+strSerial+_T("--")+strNewID;
		}
		CString strScreenName = pPraent->GetScreenName(g_serialNum, g_tstat_id);

		//

		try
		{

		strSql.Format(_T("update ALL_NODE set Product_ID ='%s', Product_name = '%s', Screen_Name = '%s' where Serial_ID = '%s'"),strNewID,strProductName,strScreenName,strSerial);
		m_pConTmp->Execute(strSql.GetString(),NULL,adCmdText);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
		
		TRACE(strSql);
	

	}
g_bPauseMultiRead = FALSE;

	if(m_pConTmp->State)
		m_pConTmp->Close();	
	
	//Sleep(1000);
	pPraent->ScanTstatInDB();
}

void CParameterDlg::OnDestroy()
{

	CDialog::OnDestroy();
	
	DWORD dwCode;
	if (pParamBackFresh!=NULL)
	{
		GetExitCodeThread(pParamBackFresh->m_hThread,&dwCode);
		if (dwCode==STILL_ACTIVE)
		{
			Sleep(200);

			TerminateThread(pParamBackFresh->m_hThread,dwCode);
			delete pParamBackFresh;
		}
	}
	// TODO: Add your message handler code here
}


void CParameterDlg::OnCbnSelchangekeypadcombo()
{
	int nItem=m_keySelectCombox.GetCurSel();
	switch(nItem)
	{
	case 0:nItem=0;break;
	case 1:nItem=5;break;
	case 2:nItem=1;break;
	case 3:nItem=4;break;
	case 4:nItem=2;break;
	case 5:nItem=3;break;
	case 6:nItem=6;break;
	case 7:nItem=7;break;
	case 8:nItem=8;break;
	default :nItem=0;break;
	}	
	g_bPauseMultiRead = TRUE;	
	if(write_one(g_tstat_id,MODBUS_KEYPAD_SELECT,nItem)<0)// 128 395
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		nItem = product_register_value[MODBUS_KEYPAD_SELECT];// resume the old status.
		switch(nItem)
		{
		case 0:nItem=0;break;
		case 1:nItem=2;break;
		case 2:nItem=4;break;
		case 3:nItem=5;break;
		case 4:nItem=3;break;
		case 5:nItem=1;break;
		case 6:nItem=6;break;
		case 7:nItem=7;break;
		case 8:nItem=8;break;
		default :nItem=0;break;
		}
		m_keySelectCombox.SetCurSel(nItem);
	}
	else
		product_register_value[MODBUS_KEYPAD_SELECT] = nItem;
	//	write_one(g_tstat_id,128,nItem,5);	//Marked by Fance

	g_bPauseMultiRead = FALSE;
}

//void CParameterDlg::OnCbnKillfocusKeypadselect()
//{
//	// TODO: Add your control notification handler code here
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

void CParameterDlg::OnEnKillfocusValuposedit()
{

	if(g_ParamLevel==1)
		return;
	CString strTemp;
	m_value_percentEdit.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
		return;
	g_bPauseMultiRead = TRUE;
	write_one(g_tstat_id, 285,_wtoi(strTemp), 5);
	g_bPauseMultiRead = FALSE;
}

void CParameterDlg::OnCbnSelendcancelBraudratecombo()
{

	if(g_ParamLevel==1)
		return;	
	g_bPauseMultiRead = TRUE;

	if(write_one(g_tstat_id, MODBUS_BAUDRATE, m_braudRateCombox.GetCurSel())<0)	//Modify by Fance
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
		product_register_value[MODBUS_BAUDRATE] = m_braudRateCombox.GetCurSel();
	g_bPauseMultiRead = FALSE;
}



void CParameterDlg::OnCbnSelchangePowermodelcombo()
{	

	if(g_ParamLevel==1)
		return;	
	g_bPauseMultiRead = TRUE;
	if(write_one(g_tstat_id, MODBUS_POWERUP_MODE, m_powerupModelCombox.GetCurSel())<0)	//127  106
		{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	else
		product_register_value[MODBUS_POWERUP_MODE] = m_powerupModelCombox.GetCurSel();
	g_bPauseMultiRead = FALSE;
}


void CParameterDlg::OnCbnSelchangeKeypadlockcombo()
{
	g_bPauseMultiRead = TRUE;	
	if(write_one(g_tstat_id, MODBUS_SPECIAL_MENU_LOCK, m_keyLockCombox.GetCurSel())<0)	//133  396
		{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	else
		product_register_value[MODBUS_SPECIAL_MENU_LOCK] =  m_keyLockCombox.GetCurSel();
	g_bPauseMultiRead = FALSE;
}

//Annul by Fance 2013 04 15

//void CParameterDlg::OnCbnSelendcancelBraudratecombo()
//{
//
//	if(g_ParamLevel==1)
//		return;	
//	g_bPauseMultiRead = TRUE;
//	int nRet = write_one(g_tstat_id, 185, m_braudRateCombox.GetCurSel());
//	g_bPauseMultiRead = FALSE;
//}

//void CParameterDlg::OnCbnSelchangePowermodelcombo()
//{	
//
//	if(g_ParamLevel==1)
//		return;	
//	g_bPauseMultiRead = TRUE;
//	write_one(g_tstat_id, 133, m_keyLockCombox.GetCurSel());	//this place has a  obvious error.  not m_keyLockCombox. recode by Fance
//	g_bPauseMultiRead = FALSE;
//}
//
//
//void CParameterDlg::OnCbnSelchangeKeypadlockcombo()
//{
//		g_bPauseMultiRead = TRUE;	
//	write_one(g_tstat_id, 133, m_keyLockCombox.GetCurSel());	
//	g_bPauseMultiRead = FALSE;
//}

void CParameterDlg::OnCbnSelchangeDisplaycombo()
{

	if(g_ParamLevel==1)
		return;

	g_bPauseMultiRead = TRUE;	

	if(write_one(g_tstat_id, MODBUS_DISPLAY, m_displayCombox.GetCurSel())<0)//203   397
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		m_autoOnlyCombox.SetCurSel(product_register_value[MODBUS_DISPLAY]);
	}
	else
		product_register_value[MODBUS_DISPLAY] = m_displayCombox.GetCurSel();
	g_bPauseMultiRead = FALSE;

}

void CParameterDlg::OnCbnSelchangeSequencecombox()
{

	if(g_ParamLevel==1)
		return;
g_bPauseMultiRead = TRUE;	
	//write_one(g_tstat_id, 118, m_SequenceCombox.GetCurSel());
	if(write_one(g_tstat_id, MODBUS_SEQUENCE, m_SequenceCombox.GetCurSel())<0)	//118 103	Modify by Fance support T6 T7
		{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	else
		product_register_value[MODBUS_SEQUENCE] =  m_SequenceCombox.GetCurSel();
g_bPauseMultiRead = FALSE;
}

void CParameterDlg::OnEnKillfocusValveedit()
{

	if(g_ParamLevel==1)
		return;
	CString strTemp;
	m_valveEdit.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
		return;	
	g_bPauseMultiRead = TRUE;
	write_one(g_tstat_id, 279,(int)(_wtof(strTemp)*10));
	g_bPauseMultiRead = FALSE;
}

void CParameterDlg::OnCbnSelchangeHcchangecombo()
{

	if(g_ParamLevel==1)
		return;	
	g_bPauseMultiRead = TRUE;
	
	//write_one(g_tstat_id, 214, m_hcChangeCombox.GetCurSel());	
	if(write_one(g_tstat_id, MODBUS_HEAT_COOL_CONFIG, m_hcChangeCombox.GetCurSel())<0)	//214  113  Modify by Fance  support t6 t7
		{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	else
		product_register_value[MODBUS_HEAT_COOL_CONFIG] = m_hcChangeCombox.GetCurSel();
	g_bPauseMultiRead = FALSE;
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
	if(product_register_value[MODBUS_POWERUP_SETPOINT]==_wtoi(strTemp))		//if not changed ,return  .Add by Fance
		return;
	g_bPauseMultiRead = TRUE;
	if(write_one(g_tstat_id, MODBUS_POWERUP_SETPOINT,(int)(_wtoi(strTemp)))<0)//126  364
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
		product_register_value[MODBUS_POWERUP_SETPOINT] = _wtoi(strTemp);
	g_bPauseMultiRead = FALSE;
}


void CParameterDlg::OnEnKillfocusStincreaedit()	//recode by Fance,for support T6 T7
{
	if(g_ParamLevel==1)
		return;
	CString strTemp;
	m_setpointIncreasement.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
		return;	
	int nvalue = _wtof(strTemp) *10;
	if(nvalue>10)
	{
	  MessageBox(_T("Please Input the value between 0 - 1 "),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	  nvalue = 10;
	}
	if(product_register_value[MODBUS_SETPOINT_INCREASE] == nvalue)	//if not changed ,return  .Add by Fance
		return;

	g_bPauseMultiRead = TRUE;	
	if(write_one(g_tstat_id, MODBUS_SETPOINT_INCREASE,(int)(nvalue))<0)//293  373
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
		product_register_value[MODBUS_SETPOINT_INCREASE] = nvalue;
	g_bPauseMultiRead = FALSE;

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
	g_bPauseMultiRead = TRUE;	
	if(write_one(g_tstat_id, MODBUS_DEFAULT_SETPOINT,_wtoi(strTemp))<0)	//338  341
		{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION); }
	else
		product_register_value[MODBUS_DEFAULT_SETPOINT] = _wtoi(strTemp) ;
	g_bPauseMultiRead = FALSE;
	//if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))
	//	Refresh6();
	//else
	//	Refresh();//Annul by Fance 
	Reflesh_ParameterDlg();
	
}

void CParameterDlg::OnEnKillfocusSetpointctledit()
{
	if(g_ParamLevel==1)
		return;
	/*

	CString strTemp;
	m_setPointCtrlEdit.GetWindowText(strTemp);
	if (strTemp.IsEmpty()) 
		return;
	write_one(g_tstat_id, 339,_wtoi(strTemp));
	strTemp.Format(_T("%d"),multi_register_value[339]);
	m_setPointCtrlEdit.SetWindowText(strTemp);
	if(multi_register_value[339]==0||multi_register_value[339]==1)
	{
		m_defSetPointEdit.EnableWindow(FALSE);
	}
	if(multi_register_value[339]==2)
	{
		m_defSetPointEdit.EnableWindow(TRUE);
	}
	*/

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
	if(product_register_value[MODBUS_FILTER] == ndata) //if not changed ,return  .Add by Fance
		return;//213  142


	g_bPauseMultiRead = TRUE;	
	if(write_one(g_tstat_id,MODBUS_FILTER,ndata)<0)	//213	142
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
		product_register_value[MODBUS_FILTER] = ndata;

	strTemp.Format(_T("%d"),product_register_value[MODBUS_FILTER]);
	m_inputFilterEdit.SetWindowText(strTemp);
	
	g_bPauseMultiRead = FALSE;
	// TODO: Add your control notification handler code here
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
	if(product_register_value[MODBUS_CYCLING_DELAY] ==nValue)	//if not changed ,return  .Add by Fance
		return;
	g_bPauseMultiRead = TRUE;	
	if(write_one(g_tstat_id, MODBUS_CYCLING_DELAY,nValue)<0)//201  241
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
		product_register_value[MODBUS_CYCLING_DELAY] = nValue;
	//write_one(g_tstat_id, 201,nValue); //Marked by Fance
	g_bPauseMultiRead = FALSE;
}

void CParameterDlg::OnEnKillfocusTempalarmedit()
{
	//m_tempAlarmEdit
}




//Modify by Fance  2013  04 11 
void CParameterDlg::OnEnKillfocusTimeronedit()
{
	if(g_ParamLevel==1)
		return;
	CString strTemp;
	m_timerOnEdit.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
		return;	
	g_bPauseMultiRead = TRUE;	
	//write_one(g_tstat_id,301,_wtoi(strTemp));		marked by fance
	if(write_one(g_tstat_id,MODBUS_TIMER_ON,_wtoi(strTemp))<0)	//T5 = 301   T6 =114
		{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	g_bPauseMultiRead = FALSE;
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
	g_bPauseMultiRead = TRUE;	
	//write_one(g_tstat_id,302,_wtoi(strTemp));marked by fance
	if(write_one(g_tstat_id,MODBUS_TIMER_OFF,_wtoi(strTemp))<0)	//T5=302  T6=115
		{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	g_bPauseMultiRead = FALSE;
}


//Recode by Fance
void CParameterDlg::OnCbnSelchangeUnitcombo()
{
	if(g_ParamLevel==1)
		return;		
	g_bPauseMultiRead = TRUE;
	if(write_one(g_tstat_id,MODBUS_TIMER_UNITS,m_uniteCombox.GetCurSel())<0)		//T5= 303    T6=116
		{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	g_bPauseMultiRead = FALSE;
	product_register_value[MODBUS_TIMER_UNITS] = m_uniteCombox.GetCurSel();
}

//Annul by Fance 2013 04 11
//void CParameterDlg::OnCbnSelchangeUnitcombo()
//{
//
//	if(g_ParamLevel==1)
//		return;		
//	g_bPauseMultiRead = TRUE;
//	int nRet = 0;
//	if ((multi_register_value[7] == 6)||(multi_register_value[7] == 7))
//		nRet = write_one(g_tstat_id,reg_tststold[303],m_uniteCombox.GetCurSel());//tstat6 116
//	else
//		nRet = write_one(g_tstat_id,303,m_uniteCombox.GetCurSel());
//	g_bPauseMultiRead = FALSE;
//
//	if(nRet>0)
//		multi_register_value[303] = m_uniteCombox.GetCurSel();
//}

void CParameterDlg::OnEnKillfocusTimerleft()
{

if(g_ParamLevel==1)
		return;
	CString strTemp;
	m_timerLeft.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
		return;	
	g_bPauseMultiRead = TRUE;	
	write_one(g_tstat_id,333,_wtoi(strTemp));
g_bPauseMultiRead = FALSE;
}

//modify by Fance 2013 04 11     Fance_0411
void CParameterDlg::OnCbnSelchangeTimerdelectcombo()
{
	if(g_ParamLevel==1)
		return;	
	g_bPauseMultiRead = TRUE;
	if(write_one(g_tstat_id,MODBUS_TIMER_SELECT,m_timerSelectCombox.GetCurSel())<0)  // T5=327   //T6=409
	{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	product_register_value[MODBUS_TIMER_SELECT]=m_timerSelectCombox.GetCurSel();
	g_bPauseMultiRead = FALSE;
}


//Annul by Fance 
//void CParameterDlg::OnCbnSelchangeTimerdelectcombo()
//{
//
//	if(g_ParamLevel==1)
//		return;	
//	g_bPauseMultiRead = TRUE;
//	int nRet = 0;
//	if ((multi_register_value[7] == 6) ||(multi_register_value[7] == 7))
//		nRet = write_one(g_tstat_id,reg_tststold[327],m_timerSelectCombox.GetCurSel());	//tstat6   406
//	else
//		nRet = write_one(g_tstat_id,327,m_timerSelectCombox.GetCurSel());	
//	if(nRet>0)
//		multi_register_value[327] = m_timerSelectCombox.GetCurSel();
//		
//	g_bPauseMultiRead = FALSE;
//}

void CParameterDlg::OnEnKillfocusTimeedit()
{

	if(g_ParamLevel==1)
		return;
	CString strTemp;
	m_TimeEdit.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
		return;	
	g_bPauseMultiRead = TRUE;	
	write_one(g_tstat_id,212,_wtoi(strTemp));
g_bPauseMultiRead = FALSE;
}

void CParameterDlg::OnEnKillfocusOverridetimeedit()
{

	if(g_ParamLevel==1)
		return;
	CString strTemp;
	m_OverRideEdit.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
		return;	
	g_bPauseMultiRead = TRUE;
	write_one(g_tstat_id,211,_wtoi(strTemp));
	g_bPauseMultiRead = FALSE;
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
	// TODO: Add your control notification handler code here
}


//Recode by Fance
void CParameterDlg::OnCbnSelchangeInputselect1()
{

	if(g_ParamLevel==1)
		return;
	g_bPauseMultiRead = TRUE;	

	if(write_one(g_tstat_id, MODBUS_TEMP_SELECT,m_InputSelect1.GetCurSel()+1)<0)//T5 =111   T6 =382
		{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	product_register_value[MODBUS_TEMP_SELECT] = m_InputSelect1.GetCurSel()+1;
	g_bPauseMultiRead = FALSE;
	Reflesh_ParameterDlg();
}
//Annul by Fance 20130411
//void CParameterDlg::OnCbnSelchangeInputselect1()
//{
//
//	if(g_ParamLevel==1)
//		return;
//	g_bPauseMultiRead = TRUE;	
//	int nRet = 0;
//	if ((multi_register_value[7] == 6)||(multi_register_value[7] == 7))//tstat6
//		nRet = write_one(g_tstat_id,reg_tststold[111] ,m_InputSelect1.GetCurSel()+1);//379
//	else
//		nRet = write_one(g_tstat_id, 111,m_InputSelect1.GetCurSel()+1);
//	g_bPauseMultiRead = FALSE;
//
//	if(nRet>0)
//		multi_register_value[111] = m_InputSelect1.GetCurSel()+1;
//	//if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))
//	//	Refresh6();
//	//else
//	//	Refresh();
//	Reflesh_ParameterDlg();
//}

//void CParameterDlg::OnCbnSelchangeInputselect2()
//{
//	
//	if(g_ParamLevel==1)
//		return;
//
//	int nSel = m_inputSelect2.GetCurSel();	
//	g_bPauseMultiRead = TRUE;	
//	int nRet = 0;
//	if ((multi_register_value[7] == 6)||(multi_register_value[7] == 7))//tstat6
//		 nRet = write_one(g_tstat_id, reg_tststold[241], nSel);//380
//	else
//		nRet = write_one(g_tstat_id, 241, nSel);
//	g_bPauseMultiRead = FALSE;
//	if(nRet>0)
//		multi_register_value [241] = nSel;
//	//if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))
//	//	Refresh6();
//	//else
//	//	Refresh();
//	Reflesh_ParameterDlg();
//}



//Recode by Fance 2013 04 11
void CParameterDlg::OnCbnSelchangeInputselect2()
{

	if(g_ParamLevel==1)
		return;
	int nSel = m_inputSelect2.GetCurSel();	
	g_bPauseMultiRead = TRUE;	
	int nRet = 0;

	if(write_one(g_tstat_id, MODBUS_INPUT1_SELECT, nSel)<0)		//T5=241  T6=383
		{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	product_register_value[MODBUS_INPUT1_SELECT] =nSel;	//T5=241  T6=383
	g_bPauseMultiRead = FALSE;
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
	if(multi_register_value[241]==2)
	{
		if(multi_register_value[189]==4||multi_register_value[189]==1)
			write_one(g_tstat_id, 181,nValue*10);
		else
			write_one(g_tstat_id, 181,nValue);

	}
	else
	{
		if(multi_register_value[188]==4||multi_register_value[188]==1)
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

	if (multi_register_value[7] == PM_TSTAT7 || multi_register_value[7] == PM_TSTAT6)
	{
		short nOccupied = multi_register_value[184];  // Day setpoint option  
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
	else if((multi_register_value[7]==PM_TSTAT5E) && (m_fFirmwareVersion >= 35.4))//0912
	{
		write_one(g_tstat_id,423 ,m_application_ctrl.GetCurSel()); // 
		write_one(g_tstat_id,125 ,m_application_ctrl.GetCurSel()); // 
	}
// 	else if (multi_register_value[7]==PM_TSTAT5E)
// 	{
// 		write_one(g_tstat_id,454 ,m_application_ctrl.GetCurSel()); // 
// 	}
	else
	{
		write_one(g_tstat_id,125 ,m_application_ctrl.GetCurSel()); // 
	}


	if(nRet>0)
		multi_register_value[125] = m_application_ctrl.GetCurSel();

	g_bPauseMultiRead = FALSE;
}

void  CParameterDlg::UpdateCoolingandHeatingData()
{

	CString strText;
	UINT uint_temp=GetOEMCP();		//get system is for chinese or english'

	if(uint_temp!=936 && uint_temp!=950)
	{//not chinese
		if(m_application_ctrl.GetCurSel()==0)
		{	
			if (g_unint)
			{
				strText.Format(_T("%d°C"),product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]);//182 
				m_nightheating.SetWindowText(strText);
				strText.Format(_T("%d°C"),product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]);//183 355
				m_nightcooling.SetWindowText(strText);
			}
			else
			{
				strText.Format(_T("%d°F"),product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]);//182
				m_nightheating.SetWindowText(strText);
				strText.Format(_T("%d°F"),product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]);//183
				m_nightcooling.SetWindowText(strText);
			}

		}
		else
		{
			if (g_unint)
			{		
				strText.Format(_T("%d°C"),product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]);//123
				m_nightheating.SetWindowText(strText);

				//m_nightheating.Format("%d%c",multi_register_value[123],176);//night heating deadband
				strText.Format(_T("%d°C"),product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]);		//124
				m_nightcooling.SetWindowText(strText);
				//m_nightcooling.Format("%d%c",multi_register_value[124],176);//night cooling deadband
			}else
			{
				strText.Format(_T("%d°F"),product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]);//182   354
				m_nightheating.SetWindowText(strText);
				strText.Format(_T("%d°F"),product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]);//183
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
			}else
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
				strText.Format(_T("%d°C"),product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]/10);//123
				m_nightheating.SetWindowText(strText);

				strText.Format(_T("%d°C"),product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]/10);//124
				m_nightcooling.SetWindowText(strText);
			}else
			{
				strText.Format(_T("%d°F"),product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]/10);//182
				m_nightheating.SetWindowText(strText);

				strText.Format(_T("%d°F"),product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]/10);//183
				m_nightcooling.SetWindowText(strText);

			}

		}
	}


	//CString strText;
	//UINT uint_temp=GetOEMCP();		//get system is for chinese or english'

	//if(uint_temp!=936 && uint_temp!=950)
	//{//not chinese
	//	if(m_application_ctrl.GetCurSel()==0)
	//	{	
	//		if (g_unint)
	//		{
	//			strText.Format(_T("%d°C"),multi_register_value[182]);
	//			m_nightheating.SetWindowText(strText);
	//			strText.Format(_T("%d°C"),multi_register_value[183]);
	//			m_nightcooling.SetWindowText(strText);
	//		}
	//		else
	//		{
	//			strText.Format(_T("%d°F"),multi_register_value[182]);
	//			m_nightheating.SetWindowText(strText);
	//			strText.Format(_T("%d°F"),multi_register_value[183]);
	//			m_nightcooling.SetWindowText(strText);
	//		}

	//	}
	//	else
	//	{
	//		if (g_unint)
	//		{		
	//			strText.Format(_T("%d°C"),multi_register_value[123]);
	//		m_nightheating.SetWindowText(strText);

	//		//m_nightheating.Format("%d%c",multi_register_value[123],176);//night heating deadband
	//		strText.Format(_T("%d°C"),multi_register_value[124]);
	//		m_nightcooling.SetWindowText(strText);
	//		//m_nightcooling.Format("%d%c",multi_register_value[124],176);//night cooling deadband
	//		}else
	//		{
	//			strText.Format(_T("%d°F"),multi_register_value[182]);
	//			m_nightheating.SetWindowText(strText);
	//			strText.Format(_T("%d°F"),multi_register_value[183]);
	//			m_nightcooling.SetWindowText(strText);

	//		}

	//	}
	//}
	//else
	//	{
	//		if(m_application_ctrl.GetCurSel()==0)
	//		{
	//			if (g_unint)
	//			{
	//				strText.Format(_T("%d°C"),multi_register_value[182]);
	//				m_nightheating.SetWindowText(strText);

	//				strText.Format(_T("%d°C"),multi_register_value[183]);
	//				m_nightcooling.SetWindowText(strText);
	//			}else
	//			{
	//				strText.Format(_T("%d°F"),multi_register_value[182]);
	//				m_nightheating.SetWindowText(strText);

	//				strText.Format(_T("%d°F"),multi_register_value[183]);
	//				m_nightcooling.SetWindowText(strText);
	//			}

	//		}
	//		else
	//		{
	//			if (g_unint)
	//			{		
	//				strText.Format(_T("%d°C"),multi_register_value[123]);
	//			m_nightheating.SetWindowText(strText);

	//			strText.Format(_T("%d°C"),multi_register_value[124]);
	//			m_nightcooling.SetWindowText(strText);
	//			}else
	//			{
	//				strText.Format(_T("%d°F"),multi_register_value[182]);
	//				m_nightheating.SetWindowText(strText);

	//				strText.Format(_T("%d°F"),multi_register_value[183]);
	//				m_nightcooling.SetWindowText(strText);

	//			}

	//		}
	//	}
}

void CParameterDlg::OnEnKillfocusSetvalue2()
{
			
	if(g_ParamLevel==1)
		return;

	CString strText;
	m_pid_setptEdt2.GetWindowText(strText);
	int nValue=_wtoi(strText);
	
	g_bPauseMultiRead = TRUE;	
	write_one(g_tstat_id, 246,nValue*10);
	//	refresh();
	
	g_bPauseMultiRead = FALSE;
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
	g_bPauseMultiRead = TRUE;
	//write_one(g_tstat_id, 104,nValue);	recode by Fance ,this code not support T6 T7;
	if(write_one(g_tstat_id, MODBUS_COOLING_PID,nValue)<0)		//when t5 MODBUS_COOLING_PID=104  ,when t6 t7 MODBUS_COOLING_PID=384	
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		strText.Format(_T("%d"),product_register_value[MODBUS_COOLING_PID]);		//104 384
		m_pid_outputEdt1.SetWindowText(strText);//if wrote fail, resume the last data.
	}
	else
		product_register_value[MODBUS_COOLING_PID] = nValue;
	g_bPauseMultiRead = FALSE;
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
	g_bPauseMultiRead = TRUE;
	//write_one(g_tstat_id, 270,nValue);	recode by Fance ,this code not support T6 T7;
	if(write_one(g_tstat_id, MODBUS_PID_UNIVERSAL,nValue)<0)		//when t5 MODBUS_PID_UNIVERSAL=270  ,when t6 t7 MODBUS_PID_UNIVERSAL=389	
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		strText.Format(_T("%d"),product_register_value[MODBUS_PID_UNIVERSAL]);		//270 389
		m_pid_outputEdt2.SetWindowText(strText);//if wrote fail, resume the last data.
	}
	else
		product_register_value[MODBUS_PID_UNIVERSAL] = nValue;
	g_bPauseMultiRead = FALSE;
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
	g_bPauseMultiRead = TRUE;
	//write_one(g_tstat_id, 114,nValue);	recode by Fance ,this code not support T6 T7;
	if(write_one(g_tstat_id, MODBUS_COOLING_PTERM,nValue)<0)		//when t5 MODBUS_COOLING_PTERM=114  ,when t6 t7 MODBUS_COOLING_PTERM=385	
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		strText.Format(_T("%d"),product_register_value[MODBUS_COOLING_PTERM]);		//114 385
		m_pternEdt1.SetWindowText(strText);//if wrote fail, resume the last data.
	}
	else
		product_register_value[MODBUS_COOLING_PTERM] = nValue;
	g_bPauseMultiRead = FALSE;
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
	g_bPauseMultiRead = TRUE;
	//write_one(g_tstat_id, 244,nValue);	recode by Fance ,this code not support T6 T7;
	if(write_one(g_tstat_id, MODBUS_UNIVERSAL_PTERM,nValue)<0)		//when t5 MODBUS_UNIVERSAL_PTERM=244  ,when t6 t7 MODBUS_UNIVERSAL_PTERM=387	
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		strText.Format(_T("%d"),product_register_value[MODBUS_UNIVERSAL_PTERM]);		//244 387
		m_ptermEdt2.SetWindowText(strText);//if wrote fail, resume the last data.
	}
	else
		product_register_value[MODBUS_UNIVERSAL_PTERM] = nValue;
	g_bPauseMultiRead = FALSE;
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
	g_bPauseMultiRead = TRUE;
	//write_one(g_tstat_id, 115,nValue);	recode by Fance ,this code not support T6 T7;
	if(write_one(g_tstat_id, MODBUS_COOLING_ITERM,nValue)<0)		//when t5 MODBUS_COOLING_ITERM=115  ,when t6 t7 MODBUS_COOLING_ITERM=386	
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		strText.Format(_T("%d"),product_register_value[MODBUS_COOLING_ITERM]);		//115  386
		m_coolingPitemEdt1.SetWindowText(strText);//if wrote fail, resume the last data.
	}
	else
		product_register_value[MODBUS_COOLING_ITERM] = nValue;
	g_bPauseMultiRead = FALSE;
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
	g_bPauseMultiRead = TRUE;
	//write_one(g_tstat_id, 245,nValue);	recode by Fance ,this code not support T6 T7;
	if(write_one(g_tstat_id, MODBUS_UNIVERSAL_ITERM,nValue)<0)		//when t5 MODBUS_UNIVERSAL_ITERM=245  ,when t6 t7 MODBUS_UNIVERSAL_ITERM=388	
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		strText.Format(_T("%d"),product_register_value[MODBUS_UNIVERSAL_ITERM]);		//245  388
		m_pidPitemEdt2.SetWindowText(strText);//if wrote fail, resume the last data.
	}
	else
		product_register_value[MODBUS_UNIVERSAL_ITERM] = nValue;
	g_bPauseMultiRead = FALSE;
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
		nMax=product_register_value[MODBUS_MAX_SETPOINT];	//131	365
		nMin=product_register_value[MODBUS_MIN_SETPOINT];	//132	366
		if (nOrig>nMax)
		{
			strText.Format(_T("%d"),product_register_value[MODBUS_MAX_SETPOINT]);
		}
		if (nOrig<nMin)
		{
			strText.Format(_T("%d"),product_register_value[MODBUS_MIN_SETPOINT]);
		}

		if ((product_register_value[7] == PM_TSTAT7)||(product_register_value[7]==PM_TSTAT6))
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
		else if(m_version<34.9 || multi_register_value[7] == PM_TSTAT5E)
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
			g_bPauseMultiRead = TRUE;	
	write_one(g_tstat_id, MODBUS_UNIVERSAL_SET,(int)(_wtof(strText)*10));//246  359
	g_bPauseMultiRead = FALSE;
	//	nret=read_one(tstat_id,246);
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
			g_bPauseMultiRead = TRUE;	
	//write_one(g_tstat_id,131 , nValue);g_bPauseMultiRead = FALSE;
	if(write_one(g_tstat_id,MODBUS_MAX_SETPOINT , nValue)<0)
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		CString strTemp;
		strTemp.Format(_T("%d"),product_register_value[MODBUS_MAX_SETPOINT]);	//if change fail,resume the last data;
		m_setptHiEdit.SetWindowText(strTemp);
	}
	else
	{
		product_register_value[MODBUS_MAX_SETPOINT] = nValue;
	}
	g_bPauseMultiRead = FALSE; //Modify by Fance support T6 T7
	Reflesh_ParameterDlg();
}

void CParameterDlg::OnEnKillfocusEsetpointlo()
{
	if(g_ParamLevel==1)
		return;

	CString strText;
	m_setptLoEdit.GetWindowText(strText);
	int nValue= _wtoi(strText);
	g_bPauseMultiRead = TRUE;

	if(write_one(g_tstat_id,MODBUS_MIN_SETPOINT , nValue)<0)//132  366
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		CString strTemp;
		strTemp.Format(_T("%d"),product_register_value[MODBUS_MIN_SETPOINT]);//132  366
		m_setptLoEdit.SetWindowText(strTemp);//Add by Fance ,if change fail.resume the data.
	}
	else
	{
		product_register_value[MODBUS_MIN_SETPOINT] = nValue;
	}
	
	g_bPauseMultiRead = FALSE;
	//if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))
	//	Refresh6();
	//else
	//	Refresh();
	Reflesh_ParameterDlg();
}

void CParameterDlg::OnEnKillfocusEcooldeadband1()
{
	if(g_ParamLevel==1)
		return;
	CString strTemp;
	CString strText;
	m_setptCDDEdt1.GetWindowText(strText);
	float nValue= (float)_wtof(strText);

	g_bPauseMultiRead = TRUE;
	if((product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7))
	{
		if(nValue == product_register_value[MODBUS_DAY_COOLING_DEADBAND]/10)	//346
			return;
		if(write_one(g_tstat_id,MODBUS_DAY_COOLING_DEADBAND,int(nValue *10))<0)
		{
			MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			strTemp.Format(_T("%.1f"),product_register_value[MODBUS_DAY_COOLING_DEADBAND]/10.0);
			m_setptCDDEdt1.SetWindowText(strTemp);
		}
		else
		{
			product_register_value[MODBUS_DAY_COOLING_DEADBAND] = int(nValue *10);
		}
	}
	else
	{
		if(nValue == product_register_value[MODBUS_COOLING_DEADBAND]/10)	//119
			return;
		if(write_one(g_tstat_id,MODBUS_COOLING_DEADBAND,int(nValue *10))<0)
		{
			MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			strTemp.Format(_T("%.1f"),product_register_value[MODBUS_COOLING_DEADBAND]/10.0);
			m_setptCDDEdt1.SetWindowText(strTemp);
		}
		else
		{
			product_register_value[MODBUS_COOLING_DEADBAND] = int(nValue *10);
		}
	}
	
	
	g_bPauseMultiRead = FALSE;


	/*
	float version=get_tstat_version(tstat_id);
	ftemp = atof(m_cooldeadband);
	itemp = int(ftemp *10);
	write_one(tstat_id,119,itemp);
	*/

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
	g_bPauseMultiRead = TRUE;
	//write_one(g_tstat_id, 243,nValue);	recode by Fance ,this code not support T6 T7;
	if(write_one(g_tstat_id, MODBUS_UNIVERSAL_DB_LO,nValue)<0)		//when t5 MODBUS_UNIVERSAL_DB_LO=243  ,when t6 t7 MODBUS_UNIVERSAL_DB_LO=361	
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		strText.Format(_T("%d"),product_register_value[MODBUS_UNIVERSAL_DB_LO]);		//243   361
		m_setptCCDEdt2.SetWindowText(strText);//if wrote fail, resume the last data.
	}
	else
		product_register_value[MODBUS_UNIVERSAL_DB_LO] = nValue;
	g_bPauseMultiRead = FALSE;
}



//
//void CParameterDlg::OnEnKillfocusEcooldeadband2()
//{
//	if(g_ParamLevel==1)
//		return;
//
//	CString strText;
//	m_setptCCDEdt2.GetWindowText(strText);
//	float nValue= (float)_wtof(strText);
//			g_bPauseMultiRead = TRUE;	
//	write_one(g_tstat_id, 243,(int)(nValue*10));g_bPauseMultiRead = FALSE;
//
//
//	MODBUS_UNIVERSAL_DB_LO
//}

void CParameterDlg::OnEnKillfocusEcoolingiterm1()
{

	if(g_ParamLevel==1)
		return;
	CString strTemp;
	CString strText;
	m_HeadDEdt1.GetWindowText(strText);
	float nValue= (float)_wtof(strText);

	g_bPauseMultiRead = TRUE;
	if((product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7))
	{
		if(nValue == product_register_value[MODBUS_DAY_HEATING_DEADBAND]/10)	//347
			return;
		if(write_one(g_tstat_id,MODBUS_DAY_HEATING_DEADBAND,int(nValue *10))<0)
		{
			MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			strTemp.Format(_T("%.1f"),product_register_value[MODBUS_DAY_HEATING_DEADBAND]/10.0);
			m_HeadDEdt1.SetWindowText(strTemp);
		}
		else
		{
			product_register_value[MODBUS_DAY_HEATING_DEADBAND] =int(nValue *10);
		}
	}
	else
	{
		if(nValue == product_register_value[MODBUS_HEATING_DEADBAND]/10)	//120
			return;
		if(write_one(g_tstat_id,MODBUS_HEATING_DEADBAND,int(nValue *10))<0)
		{
			MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			strTemp.Format(_T("%.1f"),product_register_value[MODBUS_HEATING_DEADBAND]/10.0);
			m_HeadDEdt1.SetWindowText(strTemp);
		}
		else
		{
			product_register_value[MODBUS_HEATING_DEADBAND] = int(nValue *10);
		}
	}


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
	g_bPauseMultiRead = TRUE;
	//write_one(g_tstat_id, 242,nValue);	recode by Fance ,this code not support T6 T7;
	if(write_one(g_tstat_id, MODBUS_UNIVERSAL_DB_HI,nValue)<0)		//when t5 MODBUS_UNIVERSAL_DB_HI=242  ,when t6 t7 MODBUS_UNIVERSAL_DB_HI=360	
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		strText.Format(_T("%d"),product_register_value[MODBUS_UNIVERSAL_DB_HI]);		//242   360
		m_HeadDEdt2.SetWindowText(strText);//if wrote fail, resume the last data.
	}
	else
		product_register_value[MODBUS_UNIVERSAL_DB_HI] = nValue;
	g_bPauseMultiRead = FALSE;
}


//void CParameterDlg::OnEnKillfocusEcoolingiterm2()
//{
//	if(g_ParamLevel==1)
//		return;
//
//	CString strText;
//	m_HeadDEdt2.GetWindowText(strText);
//	float nValue= (float)_wtof(strText);
//			g_bPauseMultiRead = TRUE;	
//	write_one(g_tstat_id, 242,(int)(nValue*10));g_bPauseMultiRead = FALSE;
//}




void CParameterDlg::OnBnClickedInputsbutton()
{
	CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;//tstat6
//	pMain->m_pFreshMultiRegisters->SuspendThread(); //Annul by Fance
	pMain->m_pRefreshThread->SuspendThread();
	g_bPauseMultiRead = TRUE;	//暂停主线程读取 寄存器;	//Add by Fance 
	CInputSetDlg dlg;
	dlg.DoModal();	
	InitPID2ComboBox();
	//if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))
	//	Refresh6();
	//else
	//	Refresh();
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
	//if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))
	//	Refresh6();
	//else
	//	Refresh();
	Reflesh_ParameterDlg();//Recode by Fance
}

void CParameterDlg::OnBnClickedOutputstablebutton()
{
	CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;


	g_bPauseMultiRead = TRUE;	//暂停主线程读取 寄存器;	//Add by Fance 
	COutPutDlg Dlg;
	Dlg.DoModal();
	g_bPauseMultiRead =FALSE;	//Add by Fance 

	//if ((strparamode.CompareNoCase(_T("Tstat6")) == 0)||(strparamode.CompareNoCase(_T("Tstat7")) == 0))
	//	Refresh6();
	//else
	//	Refresh();
	Reflesh_ParameterDlg();
}
void CParameterDlg::OnEnKillfocusEnigntheating()
{
	if(g_ParamLevel==1)
		return;
	CString strText;
	m_nightheating.GetWindowText(strText);
	int nValue= _wtoi(strText);

			g_bPauseMultiRead = TRUE;
	if(m_application_ctrl.GetCurSel()==0)
		write_one(g_tstat_id,182 ,nValue);
	else
		write_one(g_tstat_id,123 ,nValue);
	
	g_bPauseMultiRead = FALSE;

}

void CParameterDlg::OnEnKillfocusEnigntcooling1()
{
	
	if(g_ParamLevel==1)
		return;
	CString strText;
	m_nightcooling.GetWindowText(strText);
	int nValue= _wtoi(strText);
			g_bPauseMultiRead = TRUE;	
	if(m_application_ctrl.GetCurSel()==0)
		write_one(g_tstat_id,183 ,nValue);
	else
		write_one(g_tstat_id,124 ,nValue);
	
	g_bPauseMultiRead = FALSE;
}

BOOL CParameterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN)
		{
			GetDlgItem(IDC_REFRESHBUTTON)->SetFocus();
			return 1;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CParameterDlg::OnEnKillfocusSetvalue1()
{
	if(g_ParamLevel==1)
		return;
	CString strText;
	m_pid_setptEdt1.GetWindowText(strText);
	strText.Trim();
	if(strText.IsEmpty())
		return;
	float fValue=(float)_wtoi(strText);
	
			g_bPauseMultiRead = TRUE;	
	if (multi_register_value[7] == PM_TSTAT7)
	{
		write_one(g_tstat_id, 380,short(fValue*10));
	}
	else if(m_version<34.9 || multi_register_value[7] == PM_TSTAT5E)  // 只有5E使用135
	{
		short nVal = short(fValue);
		write_one(g_tstat_id, 135, nVal);
	}
	else
	{
		write_one(g_tstat_id, 374,short(fValue*10));
	}
g_bPauseMultiRead = FALSE;
}

//Recode by Fance , support T6 T7  2013 04 15
void CParameterDlg::OnCbnSelchangeOccupiedmodecombo()
{
	if(g_ParamLevel==1)
		return;
	int nItem=m_occupiedSetPointModeCmbox.GetCurSel();	
	g_bPauseMultiRead = TRUE;
	if(write_one(g_tstat_id, MODBUS_SETPOINT_CONTROL,nItem)<0)  //339->342
	{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);}
	else
		product_register_value[MODBUS_SETPOINT_CONTROL] = nItem;
	if(product_register_value[MODBUS_SETPOINT_CONTROL]==0||product_register_value[MODBUS_SETPOINT_CONTROL]==2)
	{
		m_defSetPointEdit.EnableWindow(FALSE);
	}
	else if(product_register_value[MODBUS_SETPOINT_CONTROL]==1)
	{
		m_defSetPointEdit.EnableWindow(TRUE);
	}	
	g_bPauseMultiRead = FALSE;
	Reflesh_ParameterDlg();
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
//	if(multi_register_value[339]==0||multi_register_value[339]==2)
//	{
//		m_defSetPointEdit.EnableWindow(FALSE);
//	}
//	if(multi_register_value[339]==1)
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
	g_bPauseMultiRead = TRUE;
	if(write_one(g_tstat_id, MODBUS_LCD_SCREEN1, nSel)<0)	//5E = 400   T6 = 539
		{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	//write_one(g_tstat_id, 400, nSel);	//Marked by Fance 
	g_bPauseMultiRead = FALSE;
}

void CParameterDlg::OnCbnSelchangeComboLcdscrn2()
{
	CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN2);
	int nSel = pCbx->GetCurSel();

	g_bPauseMultiRead = TRUE;
	if(write_one(g_tstat_id, MODBUS_LCD_SCREEN2, nSel)<0)	//5E = 401   T6 = 540
	{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	g_bPauseMultiRead = FALSE;
}

//Recode by Fance,Also Support T6 T7
void CParameterDlg::OnCbnSelchangeComboRounddis()
{
	CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_ROUNDDIS);
	int nCursel = pCbx->GetCurSel();

	bool write_error_flag = false;
	g_bPauseMultiRead = TRUE;	
	int round_display = MODBUS_ROUND_DISPLAY;//318  405
	if(nCursel == 0)
	{
		if(write_one(g_tstat_id,round_display, 0)<0)	//318  405
		{
			MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			write_error_flag = true;
		}
		else
			product_register_value[round_display] = 0;
	}
	else if(nCursel == 1)
	{
		if(write_one(g_tstat_id,round_display, 1)<0)	//318  405
		{
			MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			write_error_flag = true;
		}
		else
			product_register_value[round_display] = 1;
	}
	else if(nCursel == 2)
	{
		if(write_one(g_tstat_id, round_display, 5)<0)	//318  405
		{
			MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			write_error_flag = true;
		}
		else
			product_register_value[round_display] = 5;
	}

	g_bPauseMultiRead = FALSE;

	if(write_error_flag)	//resume the combobox 
	{
		if(product_register_value[round_display] == 0)	//318->405
		{
			pCbx->SetCurSel(0);
		}
		else if(product_register_value[round_display] == 1)//318->405
		{
			pCbx->SetCurSel(1);
		}
		else if(product_register_value[round_display] == 5)//318->405
		{
			pCbx->SetCurSel(2);
		}
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
	// TODO: Add your control notification handler code here
	CString str;

	GetDlgItem(IDC_COMBO1)->GetWindowText(str);

	if (str.CompareNoCase(_T("1 SP")) == 0)
		MDAY = 0;
	else
		MDAY = 1;

	FlexSP = 1;
}

void CParameterDlg::OnCbnKillfocusCombo4()
{
	// TODO: Add your control notification handler code here
	CString str;

	GetDlgItem(IDC_COMBO4)->GetWindowText(str);

	if (str.CompareNoCase(_T("1 SP")) == 0)
		MNIGHT = 0;
	else
		MNIGHT = 1;
	FlexSPN = 1;
	
}








// 		350	1	Low byte	W/R	(Night)Unoccupied  setpoin.
// 		351	1	Low byte		spare
// 		352	1	Low byte	W/R	(Night)Unoccupied heating setpoint dead band , heating deadband for the night (OFF) mode. Units of 1 deg.
// 		353	1	Low byte	W/R	(Night)Unoccupied cooling setpoint dead band , cooling deadband for the night (OFF) mode. Units of 1 deg.
// 		354	2	Full	W/R	(Night)Unoccupied heating setpoint
// 		355	2	Full	W/R	(Night)Unoccupied cooling setpoint








void CParameterDlg::OnEnKillfocusEditCspd()
{
	//m_coolsp= newtstat6[348]/10;
	//m_coolsp;

	BeginWaitCursor();
	UpdateData();
	if(write_one(g_tstat_id,MODBUS_DAY_COOLING_SETPOINT,m_coolsp*10)<0)
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
	product_register_value[MODBUS_DAY_COOLING_SETPOINT]=m_coolsp*10;
	EndWaitCursor();
}

void CParameterDlg::OnEnKillfocusEditCdbdn()
{
	//m_cooldb=newtstat6[346]/10;
//m_cooldb;
	BeginWaitCursor();
	UpdateData();
	if(write_one(g_tstat_id,MODBUS_DAY_COOLING_DEADBAND,m_cooldb*10)<0)	//Modify by Fance
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
	product_register_value[MODBUS_DAY_COOLING_DEADBAND] = m_cooldb*10;
	EndWaitCursor();

}

void CParameterDlg::OnEnKillfocusEdit31()
{
	//m_setpoint = newtstat6[345]/10;
	//m_setpoint;
	BeginWaitCursor();
	UpdateData();
	if(write_one(g_tstat_id,MODBUS_DAY_SETPOINT,m_setpoint*10)<0)	//Modify by  Fance 2013 04 11
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
	product_register_value[MODBUS_DAY_SETPOINT] = m_setpoint*10;
	EndWaitCursor();

}

void CParameterDlg::OnEnKillfocusEdit34()
{
	//m_heatdb = newtstat6[347]/10;
	//m_heatdb;

	BeginWaitCursor();
	UpdateData();
	if(write_one(g_tstat_id,MODBUS_DAY_HEATING_DEADBAND,m_heatdb*10)<0)//Modify by Fance  2013 04 11
	  MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
	product_register_value[MODBUS_DAY_HEATING_DEADBAND] = m_heatdb*10;
	EndWaitCursor();

}

void CParameterDlg::OnEnKillfocusEdit37()
{
	//m_heatsp = newtstat6[349]/10;
	//m_heatsp;

	BeginWaitCursor();
	UpdateData();
	if(write_one(g_tstat_id,MODBUS_DAY_HEATING_SETPOINT,m_heatsp*10)<0)	//349 only t5 no this item
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
	product_register_value[MODBUS_DAY_HEATING_SETPOINT] = m_heatsp*10;
	EndWaitCursor();

}

void CParameterDlg::OnEnKillfocusEditCspnn()
{
	BeginWaitCursor();
	UpdateData();

	//write_one(g_tstat_id,355,m_coolspN*10);
	//newtstat6[355] = m_coolspN*10;

	if(write_one(g_tstat_id,MODBUS_NIGHT_COOLING_SETPOINT,m_coolspN*10)<0)	//Add by Fance ,also support t6
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
		product_register_value[MODBUS_NIGHT_COOLING_SETPOINT] = m_coolspN*10;
	EndWaitCursor();
}

//Recode by Fance
void CParameterDlg::OnEnKillfocusEditCdbnn()
{
	BeginWaitCursor();
	UpdateData();

	if(write_one(g_tstat_id,MODBUS_NIGHT_COOLING_DEADBAND,m_cooldbN*10)<0)		//T5=124  T6=353
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
		product_register_value[MODBUS_NIGHT_COOLING_DEADBAND] = m_cooldbN*10;
	EndWaitCursor();
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

void CParameterDlg::OnEnKillfocusEdit32()
{
	BeginWaitCursor();
	UpdateData();
	if(write_one(g_tstat_id,MODBUS_NIGHT_SETPOINT,m_setpointN*10)<0)	//T6 T7 =350		//Modify by Fance
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
		product_register_value[MODBUS_NIGHT_SETPOINT] = m_setpointN*10;
	EndWaitCursor();
}

void CParameterDlg::OnEnKillfocusEdit35()
{
	BeginWaitCursor();
	UpdateData();

	//write_one(g_tstat_id,352,m_heatdbN*10);//Annul by Fance
	//newtstat6[352] = m_heatdbN*10;
	if(write_one(g_tstat_id,MODBUS_NIGHT_HEATING_DEADBAND,m_heatdbN*10)<0)	//this change can support T5 T6
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
		product_register_value[MODBUS_NIGHT_HEATING_DEADBAND] = m_heatdbN*10;

	EndWaitCursor();
}

void CParameterDlg::OnEnKillfocusEdit38()
{
	//m_heatspN=newtstat6[354]/10;
	//m_heatspN;
	UpdateData();
	if(product_register_value[MODBUS_NIGHT_HEATING_SETPOINT] == m_heatspN*10)
		return;
	BeginWaitCursor();


	//write_one(g_tstat_id,354,m_heatspN*10);	//Annul by Fance
	//newtstat6[354] = m_heatspN*10;
	if(write_one(g_tstat_id,MODBUS_NIGHT_HEATING_SETPOINT,m_heatspN*10)<0)	//Add by Fance,this change can support t5 t6
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
		product_register_value[MODBUS_NIGHT_HEATING_SETPOINT] = m_heatspN*10;

	EndWaitCursor();
}


//Add by Fance ,use this function to replace the  Reflash() and Reflash6();
void CParameterDlg::Reflesh_ParameterDlg()
{
	CString strTemp;
	strTemp.Format(_T("%d"),g_tstat_id);
	m_idAdressEdit.SetWindowText(strTemp);
	CString strUnit=GetTempUnit();//Unit string.

	//185	110	1	Low byte	W/R(Reboot after write)	Bau - Baudrate, 0=9600, 1=19.2kbaud
	if(product_register_value[MODBUS_BAUDRATE]>=0&&product_register_value[MODBUS_BAUDRATE]<=1)	//110
		m_braudRateCombox.SetCurSel(product_register_value[MODBUS_BAUDRATE]);//110 //Modify by Fance
	else
		m_braudRateCombox.SetCurSel(0);



	if (product_register_value[MODBUS_PRODUCT_MODEL] == 18)//tstat5g	//7
	{
		CString str;
		str.Format(_T("%.1f"),product_register_value[MODBUS_UNIVERSAL_NIGHTSET]/10.0);			//275 
		GetDlgItem(IDC_EDIT_PID2OFFSETPOINT)->SetWindowText(str+strUnit);
	}


	int nItem;
	nItem = product_register_value[MODBUS_KEYPAD_SELECT];//395
	switch(nItem)
	{
	case 0:nItem=0;break;
	case 1:nItem=2;break;
	case 2:nItem=4;break;
	case 3:nItem=5;break;
	case 4:nItem=3;break;
	case 5:nItem=1;break;
	case 6:nItem=6;break;
	case 7:nItem=7;break;
	case 8:nItem=8;break;
	default :nItem=0;break;
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

	//m_displayCombox.SetCurSel(multi_register_value[203]);
	m_displayCombox.SetCurSel(product_register_value[MODBUS_DISPLAY]);	//203 397 MODBUS_DISPLAY

	//	133	396	1	Low byte	W/R	"SPECIAL_MENU_LOCK, Special menu lockout via keypad, serial port only, 
	m_keyLockCombox.SetCurSel(product_register_value[MODBUS_SPECIAL_MENU_LOCK]);		//133 396 


	//129	107	1	Low byte	W/R	"AUTO_ONLY , enables or disables manual mode. 0 = Manual Fan Modes 1-x Allowed 
	m_autoOnlyCombox.SetCurSel(product_register_value[MODBUS_AUTO_ONLY]);		//129 107
	//118	103	1	Low byte	W/R(Reboot after write)	SEQUENCE , 0 = internal test sequence, outputs slowly cycle on/off or ramp up & down. 1 = normal, operation according to the output tables.
	m_SequenceCombox.SetCurSel(product_register_value[MODBUS_SEQUENCE]);	//118 103



	//279	243	1	Low byte	W/R	Valve travel time. The time of the valve travel  from one end to another end. The units is second.
	strTemp.Format(_T("%.1f"),product_register_value[MODBUS_VALVE_TRAVEL_TIME]/10.0);	//279 T6=243
	m_valveEdit.SetWindowText(strTemp);

	//214	113	1	Low byte	W/R(not to eeprom)
	m_hcChangeCombox.SetCurSel(product_register_value[MODBUS_HEAT_COOL_CONFIG]);	//214  T6=113 

	//126	364	1	Low byte	W/R	POWERUP_SETPOINT , setpoint on power up
	strTemp.Format(_T("%d"),product_register_value[MODBUS_POWERUP_SETPOINT]);		//126  T6=364
	m_powerSetPointEdit.SetWindowText(strTemp);

	//293	373	1	Low byte	W/R	Setpoint increment on the display each time the user hits the up/down buttons. Units are 0.1Deg, 10 = 1Deg and so on. 
	strTemp.Format(_T("%.1f"),product_register_value[MODBUS_SETPOINT_INCREASE]/10.0);		//293  373
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
	//if(multi_register_value[339]==1)
	if(product_register_value[MODBUS_SETPOINT_CONTROL]==1)//339  T6=342
	{
		m_defSetPointEdit.EnableWindow(TRUE);
	}














	//213	142	1	Low byte	W/R	Temperature sensor filter, FIL, weighted average of stored value to new raw value
	//	strTemp.Format(_T("%d"),multi_register_value[213]);
	strTemp.Format(_T("%d"),product_register_value[MODBUS_FILTER]);	//213  T6=142
	m_inputFilterEdit.SetWindowText(strTemp);


	//201	241	1	Low byte	W/R	"MODBUS_CYCLING_DELAY ¨C delay time (in minutes) for switching out of heating or cooling
	//strTemp.Format(_T("%d"),multi_register_value[201]);
	strTemp.Format(_T("%d"),product_register_value[MODBUS_CYCLING_DELAY]);	//201  T6=241
	m_cycledlayEdit.SetWindowText(strTemp);

	//301	114	2	Full	W/R	Period timer ON time.
	//strTemp.Format(_T("%d"),multi_register_value[301]);
	strTemp.Format(_T("%d"),product_register_value[MODBUS_TIMER_ON]);	//301   T6=114
	m_timerOnEdit.SetWindowText(strTemp);

	//302	115	2	Full	W/R	Period timer OFF time.
	//strTemp.Format(_T("%d"),multi_register_value[302]);

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

		//if(multi_register_value[284]==2)
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

	//if(multi_register_value[283]==2||multi_register_value[284]==2)
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
		//m_timerSelectCombox.SetCurSel(multi_register_value[327]);
		m_timerSelectCombox.SetCurSel(product_register_value[MODBUS_TIMER_SELECT]);	//327  408->409才对









		//211	111	1	Low byte	W/R	Unoccupied Override Timer, Ort. 0=disabled, >0=number of minutes manual override is allowed
		//212	112	1	Low byte	W/R	"OVERRIDE_TIMER_DOWN_COUNT - Number of minutes remaining on the timer when 

		//strTemp.Format(_T("%d"),multi_register_value[212]);
		strTemp.Format(_T("%d"),product_register_value[MODBUS_OVERRIDE_TIMER_LEFT]);	//112
		m_TimeEdit.SetWindowText(strTemp);
		strTemp.Format(_T("%d"),product_register_value[MODBUS_OVERRIDE_TIMER]);	//111
		m_OverRideEdit.SetWindowText(strTemp);
		//tstat6
		//111	382	1	Low byte	W/R	Sensor to be used for the PID calculations,  1= external sensor analog input 1 , 2 = internal thermistor, 3 = average the internal thermistor and analog input1
		if (product_register_value[MODBUS_TEMP_SELECT]<1)	//382
		{
			m_InputSelect1.SetCurSel(0);
		}else if (product_register_value[MODBUS_TEMP_SELECT]>3)//382
		{
			m_InputSelect1.SetCurSel(2);
		}else
		{
			m_InputSelect1.SetCurSel(product_register_value[MODBUS_TEMP_SELECT]-1);//382
		}

		if (product_register_value[MODBUS_INPUT1_SELECT]<0)	//383
		{
			m_inputSelect2.SetCurSel(0);
		}else if (product_register_value[MODBUS_INPUT1_SELECT]>5)	//383
		{
			m_inputSelect2.SetCurSel(5);
		}else
		{
			m_inputSelect2.SetCurSel(product_register_value[MODBUS_INPUT1_SELECT]);	//383
		}


		strTemp.Format(_T("%.1f"),product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);	//121
		m_inputvalue1.SetWindowText(strTemp+strUnit);






			if(product_register_value[MODBUS_INPUT1_SELECT]==1) // input1		//383  241
			{	
				//X下面这一段肯定有问题
				if(product_register_value[MODBUS_PRODUCT_MODEL]== PM_PRESSURE)  // pressure
				{
					strTemp.Format(_T("%.1f W.C"),product_register_value[MODBUS_EXTERNAL_SENSOR_0]/100.0);//180
				}
				else
				{

					if(product_register_value[MODBUS_ANALOG_IN1]==4||product_register_value[MODBUS_ANALOG_IN1]==1)//188
					{
						strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_0]/10);//180
						strTemp=strTemp+strUnit;
					}
					if (strUnit==""||strUnit=="%")
					{
						strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_0]);//180
					}
					if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==3)//180
					{
						if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//180
							strTemp=_T("OFF");
						if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//180
							strTemp=_T("ON");
					}

				}
				//X上面面这一段肯定有问题
				m_inputValue2.SetWindowText(strTemp);
			}

			//X下面这一段肯定有问题
			if(multi_register_value[241]==2) // input2 //m_inputvalue1
			{
				if(multi_register_value[189]==4||multi_register_value[189]==1)
				{
					strTemp.Format(_T("%.1f"),(float)multi_register_value[181]/10.0);	
					strTemp=strTemp+strUnit;
				}
				//if (strUnit==""||strUnit=="%")//0914
				{
					//strTemp.Format(_T("%d"),multi_register_value[181]);//0914
					strTemp.Format(_T("%.1f"),(float)multi_register_value[181]/10);
					strTemp=strTemp+strUnit;
				}
				if(multi_register_value[189]==3)
				{
					if(multi_register_value[181]==0)
						strTemp=_T("OFF");
					if(multi_register_value[181]==1)
						strTemp=_T("ON");
				}
				m_inputValue2.SetWindowText(strTemp);
			}
			else //if(multi_register_value[241]!=1)
			{
				m_inputValue2.SetWindowText(_T("UNUSED"));
				//		m_hotelCoolEdt.SetWindowText(_T("N/A"));
			}

			if (product_register_value[MODBUS_INPUT1_SELECT]==3) // humidity	//383
			{
				strTemp.Format(_T("%.1f%%"),multi_register_value[422]/10.0);	//422 Don't know why???
				m_inputValue2.SetWindowText(strTemp);
			}


			//X上面面这一段肯定有问题;

			short nOccupied = product_register_value[MODBUS_INFO_BYTE];  // Day setpoint option  //109 
			BOOL bOccupied = nOccupied & 0x0001;
			//下面有问题
			if (bOccupied)  // day  - Occupied
			{
				m_application_ctrl.SetCurSel(product_register_value[351]);//tstat6,7ÕÒ²»µ½	454
				// 5E ÒÔ¼°ÒÔºóµÄÐÍºÅ
			}
			else
			{
				m_application_ctrl.SetCurSel(product_register_value[125]); 
			}
			UpdateCoolingandHeatingData();
			if(product_type!=3)
			{
				//UpdateCoolingandHeatingData();
				int index = 0;
				index = m_application_ctrl.GetCurSel();
				switch(index)
				{
				case 0:
					GetDlgItem(IDC_STATIC_CSP)->SetWindowText(_T("Cooling Set Point"));
					GetDlgItem(IDC_STATIC_HSP)->SetWindowText(_T("Heating Set Point"));
					break;
				case 1:
					GetDlgItem(IDC_STATIC_CSP)->SetWindowText(_T("Cooling DB"));
					GetDlgItem(IDC_STATIC_HSP)->SetWindowText(_T("Heating DB"));
					break;
				}
			}
			else
			{
				//UpdateCoolingandHeatingData();
			}

			strTemp.Empty();
			m_version=get_curtstat_version();



			strTemp.Empty();
			strTemp.Format(_T("%.1f"),product_register_value[MODBUS_UNIVERSAL_SET]/10.0);	 //246  359
			m_pid_setptEdt2.SetWindowText(strTemp);

			strTemp.Format(_T("%d"),product_register_value[MODBUS_COOLING_PID]);		//104 384
			m_pid_outputEdt1.SetWindowText(strTemp);
			strTemp.Format(_T("%d"),product_register_value[MODBUS_PID_UNIVERSAL]);		//270  389
			m_pid_outputEdt2.SetWindowText(strTemp);


			//114	385	1	Low byte	R	COOLING PTERM , proportional term for PI calculation
			strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_COOLING_PTERM]/10.0);	//114  385
			m_pternEdt1.SetWindowText(strTemp);

			strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_UNIVERSAL_PTERM]/10.0);		//244  387
			m_ptermEdt2.SetWindowText(strTemp);
			



			strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_COOLING_ITERM]/10.0);	//115  386
			m_coolingPitemEdt1.SetWindowText(strTemp);

			strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_UNIVERSAL_ITERM]/10.0);	//245  388
			m_pidPitemEdt2.SetWindowText(strTemp);


			strTemp.Empty();
			strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_UNIVERSAL_SET]/10.0);	//246  359
			strTemp+=strUnit;
			m_dayOccEdt2.SetWindowText(strTemp);


			//下面有问题;
			if (product_register_value[MODBUS_INPUT1_SELECT]==3) // humidity		//241
			{
				strTemp.Format(_T("%.1f%%"),multi_register_value[422]/10.0);	//422 Don't know why???
				m_dayOccEdt2.SetWindowText(strTemp);
			}

			strTemp.Empty();
			//上面可能有问题;

			if(product_type!=3)
			{
				if(m_version<34.9 || multi_register_value[7] == PM_TSTAT5E)   // ÀÏ°æ±¾135£¬ÐÂ°æ±¾374// ÀÏ°æ±¾²»³ý10£¬
				{
					strTemp.Format(_T("%d"),(int)product_register_value[MODBUS_COOLING_SETPOINT]);//135
				}
				else
				{
					strTemp.Format(_T("%.1f"),product_register_value[MODBUS_TWO_BYTE_SETPOINT]/10.0);	// set point for TStat A,B,C,D	//374
				}	

				m_dayOccEdt1.SetWindowText(strTemp);
				if (multi_register_value[7] == PM_TSTAT5E)//0911
				{
					strTemp.Format(_T("%.1f"),multi_register_value[135]/10.0);//0911	//135
					m_pid_setptEdt1.SetWindowText(strTemp+strUnit);
				}
				else
				{
					// unoccupied and office mode, we should recalc the setpoint here	
					short nOccupied = multi_register_value[184];  // Day setpoint option  //184


					BOOL bOccupied = nOccupied & 0x0001;
					if (!bOccupied && multi_register_value[125] == 0)  // unoccupied and office mode	//125
					{ 
						float temp =float( (multi_register_value[183]-multi_register_value[182]) / 2.0 + multi_register_value[182] );//183  182  182
						//strTemp.Format(_T("%.1f"),temp);
						strTemp.Format(_T("%d"),(int)temp);
					}
					m_pid_setptEdt1.SetWindowText(strTemp+strUnit);
				}


			}
			else
			{
				strTemp.Format(_T("%.1f"),product_register_value[MODBUS_DAY_SETPOINT]/10.0);//345	set point 
						m_dayOccEdt1.SetWindowText(strTemp);
					float temp =float(product_register_value[MODBUS_DAY_SETPOINT]/10.0);
				strTemp.Format(_T("%d"),(int)temp);
				m_pid_setptEdt1.SetWindowText(strTemp+strUnit);
			}


			//131	365	1	Low byte	W/R	MAX_SETPOINT, Setpoint high, the highest setpoint a user will be able to set from the keypad.
			//132	366	1	Low byte	W/R	MIN_SETPOINT, Setpoint Low, the lowest setpoint a user will be able to set from the keypad. 
			strTemp.Format(_T("%d"),product_register_value[MODBUS_MAX_SETPOINT]);//131 365
			strTemp+=strUnit;
			m_setptHiEdit.SetWindowText(strTemp);

			strTemp.Empty();
			//strTemp.Format(_T("%d"),multi_register_value[132]);
			strTemp.Format(_T("%d"),product_register_value[MODBUS_MIN_SETPOINT]);//132  366
			strTemp+=strUnit;
			m_setptLoEdit.SetWindowText(strTemp);

			//119	346	1	Low byte	W/R	(Day)Occupied cooling setpoint dead band  , offset from setpoint for cooling to begin.  Units are 0.1 deg.
			//		347	1	Low byte	W/R	(Day)Occupied heating setpoint dead band  , offset from setpoint for heating to begin.  Units are 0.1 deg.

			if(product_type!=3)
			{
				strTemp.Empty();
				strTemp.Format(_T("%.1f"),product_register_value[MODBUS_COOLING_DEADBAND]/10.0);	//119
				strTemp+=strUnit;
				m_setptCDDEdt1.SetWindowText(strTemp);

				strTemp.Empty();
				strTemp.Format(_T("%.1f"),product_register_value[MODBUS_HEATING_DEADBAND]/10.0);	//120
				strTemp+=strUnit;
				m_HeadDEdt1.SetWindowText(strTemp);
			}
			else
			{
				strTemp.Format(_T("%.1f"),product_register_value[MODBUS_DAY_COOLING_DEADBAND]/10.0);
				m_setptCDDEdt1.SetWindowText(strTemp);


				strTemp.Format(_T("%.1f"),product_register_value[MODBUS_DAY_HEATING_DEADBAND]/10.0);
				m_HeadDEdt1.SetWindowText(strTemp);

			}
			
			strTemp.Empty();
			strTemp.Format(_T("%.1f"),product_register_value[MODBUS_UNIVERSAL_DB_LO]/10.0);//243->361
			strTemp+=strUnit;
			m_setptCCDEdt2.SetWindowText(strTemp);

			strTemp.Empty();
			strTemp.Format(_T("%.1f"),product_register_value[MODBUS_UNIVERSAL_DB_HI]/10.0);// 242->360
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
}
//Annule by Fance recode
//void CParameterDlg::Refresh6()
//{
//
//	CString strTemp;
//	strTemp.Format(_T("%d"),g_tstat_id);
//	m_idAdressEdit.SetWindowText(strTemp);
//	CString strUnit=GetTempUnit();//Unit string.
//	//185	110	1	Low byte	W/R(Reboot after write)	Bau - Baudrate, 0=9600, 1=19.2kbaud
//	if(newtstat6[_G("MODBUS_BAUDRATE",3)]>=0&&newtstat6[_G("MODBUS_BAUDRATE",3)]<=1)	//110
//		//m_braudRateCombox.SetCurSel(multi_register_value[110]);
//		m_braudRateCombox.SetCurSel(newtstat6[_G("MODBUS_BAUDRATE",3)]);//110 //Modify by Fance
//	else
//		//m_braudRateCombox.SetCurSel(-1);
//		m_braudRateCombox.SetCurSel(0);
//
//
//	//128	395	1	Low byte	W/R(Reboot after write)
//
//	int nItem;
//	nItem = newtstat6[_G("MODBUS_KEYPAD_SELECT",3)];//395
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
//
//	//127	106	1	Low byte	W/R	"POWER UP_MODE, mode of operation on power up. 0 = power off, 
//	//1 = power up in on mode, 2 = last value (default), 3 =  auto mode."
//
//	//m_powerupModelCombox.SetCurSel(multi_register_value[127]);
//	m_powerupModelCombox.SetCurSel(newtstat6[_G("MODBUS_POWERUP_MODE",3)]);//106
//
//
//	//285	244	1	Low byte	R	Valve percent. Show the valve opened how much percent.  READ ONLY
//	//strTemp.Format(_T("%d"),multi_register_value[285]);
//	strTemp.Format(_T("%d"),newtstat6[_G("MODBUS_VALVE_PERCENT",3)]);	//244 
//	m_value_percentEdit.SetWindowText(strTemp);
//
//// 	203	397	1	Low byte	W/R	"dIS – Display.  This sets the display to either room temperature or setpoint.  0 = room temp, 
//// 		1 = setpoint, 2 = Blank Display,3 = PID2 value,4 = PID2 setpoint,
//// 		5 = set segment code by manually, 6 = Display sleep"//
//
//
//	//m_displayCombox.SetCurSel(multi_register_value[203]);
//	m_displayCombox.SetCurSel(newtstat6[_G("MODBUS_DISPLAY",3)]);	//397 MODBUS_DISPLAY
//
////	133	396	1	Low byte	W/R	"SPECIAL_MENU_LOCK, Special menu lockout via keypad, serial port only, 
//
//	//m_keyLockCombox.SetCurSel(multi_register_value[133]);
//	m_keyLockCombox.SetCurSel(newtstat6[_G("MODBUS_SPECIAL_MENU_LOCK",3)]);		//396 
//
//
//	//129	107	1	Low byte	W/R	"AUTO_ONLY , enables or disables manual mode. 0 = Manual Fan Modes 1-x Allowed 
//	
//	//m_autoOnlyCombox.SetCurSel(multi_register_value[129]);
//	m_autoOnlyCombox.SetCurSel(newtstat6[_G("MODBUS_AUTO_ONLY",3)]);		//107
//
//	//118	103	1	Low byte	W/R(Reboot after write)	SEQUENCE , 0 = internal test sequence, outputs slowly cycle on/off or ramp up & down. 1 = normal, operation according to the output tables.
//	//m_SequenceCombox.SetCurSel(multi_register_value[118]);
//	m_SequenceCombox.SetCurSel(newtstat6[_G("MODBUS_SEQUENCE",3)]);	//103
//
//	//279	243	1	Low byte	W/R	Valve travel time. The time of the valve travel  from one end to another end. The units is second.
//	//strTemp.Format(_T("%.1f"),multi_register_value[279]/10.0);
//	strTemp.Format(_T("%.1f"),newtstat6[_G("MODBUS_VALVE_TRAVEL_TIME",3)]/10.0);	//243
//	m_valveEdit.SetWindowText(strTemp);
//
//	//214	113	1	Low byte	W/R(not to eeprom)
//	//m_hcChangeCombox.SetCurSel(multi_register_value[214]);
//	m_hcChangeCombox.SetCurSel(newtstat6[_G("MODBUS_HEAT_COOL_CONFIG",3)]);	//113 
//
//	//126	364	1	Low byte	W/R	POWERUP_SETPOINT , setpoint on power up
//	//strTemp.Format(_T("%d"),multi_register_value[126]);
//	strTemp.Format(_T("%d"),newtstat6[_G("MODBUS_POWERUP_SETPOINT",3)]);		//364
//
//	m_powerSetPointEdit.SetWindowText(strTemp);
//
//	//293	373	1	Low byte	W/R	Setpoint increment on the display each time the user hits the up/down buttons. Units are 0.1Deg, 10 = 1Deg and so on. 
//	//strTemp.Format(_T("%.1f"),multi_register_value[293]/10.0);
//	strTemp.Format(_T("%.1f"),newtstat6[_G("MODBUS_SETPOINT_INCREASE",3)]/10.0);		//373
//
//	m_setpointIncreasement.SetWindowText(strTemp);
//
//
////	338	341	1	Low byte	W/R	Default occupied setpoint. Works in concert with the "occupied setpoint control register", register 339
//	//strTemp.Format(_T("%d"),multi_register_value[338]);
//	strTemp.Format(_T("%d"),newtstat6[_G("MODBUS_DEFAULT_SETPOINT",3)]);	//341
//	
//	m_defSetPointEdit.SetWindowText(strTemp);
//
//
//	//339	342	1	Low byte	W/R	Occupied Setpoint Control Register: 0 = normal, setpoint is managed by the serial port and keypad, the stat will remember the last occupied setpoint and use that during the next occupied period. 1 = Default mode, the last occupied setpoint if forgotten and the occupied setpoint gets reset to the default. 2 = trigger an event, when a master controller writes 2 to this register, the default setpoint will be copied to the occupied setpoint after which the Tstat will set the value back to 1 to show the event has been serviced. 
//
//	if(newtstat6[_G("MODBUS_SETPOINT_CONTROL",3)]>=0&&newtstat6[_G("MODBUS_SETPOINT_CONTROL",3)]<3)	//342
//	{
//		m_occupiedSetPointModeCmbox.SetCurSel(newtstat6[_G("MODBUS_SETPOINT_CONTROL",3)]);//342
//	}
//
//	if(newtstat6[_G("MODBUS_SETPOINT_CONTROL",3)]==0||newtstat6[_G("MODBUS_SETPOINT_CONTROL",3)]==2)//342
//	{
//		m_defSetPointEdit.EnableWindow(FALSE);
//	}
//	//if(multi_register_value[339]==1)
//	if(newtstat6[_G("MODBUS_SETPOINT_CONTROL",3)]==1)//342
//	{
//		m_defSetPointEdit.EnableWindow(TRUE);
//	}
//
//	//213	142	1	Low byte	W/R	Temperature sensor filter, FIL, weighted average of stored value to new raw value
////	strTemp.Format(_T("%d"),multi_register_value[213]);
//	strTemp.Format(_T("%d"),newtstat6[_G("MODBUS_FILTER",3)]);	//142
//	m_inputFilterEdit.SetWindowText(strTemp);
//
//
//	//201	241	1	Low byte	W/R	"MODBUS_CYCLING_DELAY – delay time (in minutes) for switching out of heating or cooling
//	//strTemp.Format(_T("%d"),multi_register_value[201]);
//	strTemp.Format(_T("%d"),newtstat6[_G("MODBUS_CYCLING_DELAY",3)]);	//241
//	m_cycledlayEdit.SetWindowText(strTemp);
//
//	//301	114	2	Full	W/R	Period timer ON time.
//	//strTemp.Format(_T("%d"),multi_register_value[301]);
//	strTemp.Format(_T("%d"),newtstat6[_G("MODBUS_TIMER_ON",3)]);	//114
//	m_timerOnEdit.SetWindowText(strTemp);
//
//	//302	115	2	Full	W/R	Period timer OFF time.
//	//strTemp.Format(_T("%d"),multi_register_value[302]);
//
//	strTemp.Format(_T("%d"),newtstat6[_G("MODBUS_TIMER_OFF",3)]);	//115
//	m_timerOffEdit.SetWindowText(strTemp);
//
//	//303	116	1	Low byte	W/R(not to eeprom)	Period timer units. 0, second; 1, minute; 2, hour.
//	m_uniteCombox.SetCurSel(newtstat6[_G("MODBUS_TIMER_UNITS",3)]);	//116
//
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_COOL_ORIGINAL_TABLE",3)]);//tstat6,7没找到。	333
//	m_timerLeft.SetWindowText(strTemp);
//
//
//	//283	205	1	Low byte	W/R	Determine the output4 mode. 0, ON/OFF mode; 1, floating valve for cooling; 2, lighting control; 3, PWM 
//	//284	206	1	Low byte	W/R	Determine the output5 mode. 0, ON/OFF mode; 1, floating valve for heating; 2, lighting control; 3, PWM
//
//	//if(multi_register_value[283]==2||multi_register_value[284]==2)
//	if(newtstat6[_G("MODBUS_MODE_OUTPUT4",3)]==2||newtstat6[_G("MODBUS_MODE_OUTPUT5",3)]==2)	//205  206
//	{
//		CComboBox* pCombox=(CComboBox*)GetDlgItem(IDC_TIMERDELECTCOMBO);
//		int	indext=3;
//		//327	408	1	Low byte	W/R	"Assign the timer to be used for which feature. 0 = period timer, 1 = rotation timer,
//		write_one(g_tstat_id,_G("MODBUS_EEPROM_SIZE",3),indext); //408 
//		pCombox->SetCurSel(3);
//
//	}
//	else
//	{
//
//	}
//
//	//if(multi_register_value[283]==2)
//	if(newtstat6[_G("MODBUS_MODE_OUTPUT4",3)]==2)	//205 
//	{
//		CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMERON_STATIC);
//		ptimerout->SetWindowText(_T("Out4:"));
//	}
//	else
//	{
//		CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMERON_STATIC);
//		ptimerout->SetWindowText(_T("Timer On:"));
//	}
//
//	//if(multi_register_value[284]==2)
//	if(newtstat6[_G("MODBUS_MODE_OUTPUT5",3)]==2)	//206 
//	{
//
//		CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMEOFF_STATIC);
//		ptimerout->SetWindowText(_T("Out5:"));
//
//	}
//	else
//	{
//		CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMEOFF_STATIC);
//		ptimerout->SetWindowText(_T("Timer Off:"));
//	}
//
////327	408	1	Low byte	W/R	"Assign the timer to be used for which feature. 0 = period timer, 1 = rotation timer,
//	//m_timerSelectCombox.SetCurSel(multi_register_value[327]);
//	m_timerSelectCombox.SetCurSel(newtstat6[_G("MODBUS_EEPROM_SIZE",3)]);	//408
//
//
//		//211	111	1	Low byte	W/R	Unoccupied Override Timer, Ort. 0=disabled, >0=number of minutes manual override is allowed
//		//212	112	1	Low byte	W/R	"OVERRIDE_TIMER_DOWN_COUNT - Number of minutes remaining on the timer when 
//	
//
//
//
//	//strTemp.Format(_T("%d"),multi_register_value[212]);
//	strTemp.Format(_T("%d"),newtstat6[_G("MODBUS_OVERRIDE_TIMER_LEFT",3)]);	//112
//	m_TimeEdit.SetWindowText(strTemp);
//	strTemp.Format(_T("%d"),newtstat6[_G("MODBUS_OVERRIDE_TIMER",3)]);	//111
//	m_OverRideEdit.SetWindowText(strTemp);
////tstat6
//	//111	382	1	Low byte	W/R	Sensor to be used for the PID calculations,  1= external sensor analog input 1 , 2 = internal thermistor, 3 = average the internal thermistor and analog input1
//	if (newtstat6[_G("MODBUS_TEMP_SELECT",3)]<1)	//382
//	{
//		m_InputSelect1.SetCurSel(0);
//	}else if (newtstat6[_G("MODBUS_TEMP_SELECT",3)]>3)//382
//	{
//		m_InputSelect1.SetCurSel(2);
//	}else
//	{
//		m_InputSelect1.SetCurSel(newtstat6[_G("MODBUS_TEMP_SELECT",3)]-1);//382
//	}
//	
//	if (newtstat6[_G("MODBUS_INPUT1_SELECT",3)]<0)	//383
//	{
//		m_inputSelect2.SetCurSel(0);
//	}else if (newtstat6[_G("MODBUS_INPUT1_SELECT",3)]>5)	//383
//	{
//		m_inputSelect2.SetCurSel(5);
//	}else
//	{
//		m_inputSelect2.SetCurSel(newtstat6[_G("MODBUS_INPUT1_SELECT",3)]);	//383
//	}
//	
//
//	strTemp.Format(_T("%.1f"),newtstat6[_G("MODBUS_TEMPRATURE_CHIP",3)]/10.0);	//121
//	m_inputvalue1.SetWindowText(strTemp+strUnit);
//
//	//////////////////////////////////////////////////////////////////////////
//	// PID2 m_inputValue2 m_inputSelect2 关联设置
//	if(newtstat6[_G("MODBUS_INPUT1_SELECT",3)]==1) // input1		//383
//	{	
//
//		if(newtstat6[7]== PM_PRESSURE)  // pressure
//		{
//			strTemp.Format(_T("%.1f W.C"),multi_register_value[MODBUS_EXTERNAL_SENSOR_0]/100.0);//180
//		}
//		else
//		{
//			
//			if(multi_register_value[MODBUS_ANALOG_IN1]==4||multi_register_value[MODBUS_ANALOG_IN1]==1)//188
//			{
//				strTemp.Format(_T("%.1f"),(float)multi_register_value[MODBUS_EXTERNAL_SENSOR_0]/10);//180
//				strTemp=strTemp+strUnit;
//			}
//			if (strUnit==""||strUnit=="%")
//			{
//				strTemp.Format(_T("%d"),multi_register_value[MODBUS_EXTERNAL_SENSOR_0]);//180
//			}
//			if(multi_register_value[MODBUS_EXTERNAL_SENSOR_0]==3)//180
//			{
//				if(multi_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//180
//					strTemp=_T("OFF");
//				if(multi_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)//180
//					strTemp=_T("ON");
//			}
//			
//		}
//
//		m_inputValue2.SetWindowText(strTemp);
//	}
//
//	//241	383	2	Full	W/R	Universal PID input select, 0=none, 1=analog_in1, 2=analog_in2, 3= humidity  4= co2
//
//
//	//if(multi_register_value[241]==2) // input2 //m_inputvalue1
//	if(newtstat6[_G("MODBUS_INPUT1_SELECT",3)]==2) // input2 //m_inputvalue1	383
//	{
//
//		if(multi_register_value[_G("MODBUS_ANALOG_IN2")]==4||multi_register_value[_G("MODBUS_ANALOG_IN2")]==1)//tstat6找不到	//189 MODBUS_ANALOG_IN2
//		{
//			strTemp.Format(_T("%.1f"),(float)multi_register_value[_G("MODBUS_EXTERNAL_SENSOR_1")]/10.0);	//181
//			strTemp=strTemp+strUnit;
//		}
//		if (strUnit==""||strUnit=="%")
//		{
//			strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_EXTERNAL_SENSOR_1")]);//181
//		}
//		if(multi_register_value[_G("MODBUS_ANALOG_IN2")]==3)	//189
//		{
//			if(multi_register_value[_G("MODBUS_EXTERNAL_SENSOR_1")]==0)//181
//				strTemp=_T("OFF");
//			if(multi_register_value[_G("MODBUS_EXTERNAL_SENSOR_1")]==1)//181
//				strTemp=_T("ON");
//		}
//
//	}
//	else
//	{
//			//m_hotelCoolEdt.SetWindowText(_T("N/A"));//0911
//	}
//
//
//	if (newtstat6[_G("MODBUS_INPUT1_SELECT",3)]==3) // humidity	//383
//	{
//		strTemp.Format(_T("%.1f%%"),multi_register_value[422]/10.0);	//422 Don't know why???
//		m_inputValue2.SetWindowText(strTemp);
//	}
//
//	//
//	//////////////////////////////////////////////////////////////////////////
//	//184	109	1	Low byte	W/R	Info Byte, this register contains info about the state of the tstat.  
//
//
//// 	if (newtstat6[7] == PM_TSTAT7) || newtstat6[7] == PM_TSTAT6)
//// 	{
//		//short nOccupied = multi_register_value[184];  // Day setpoint option  
//	short nOccupied = newtstat6[_G("MODBUS_INFO_BYTE",3)];  // Day setpoint option  //109 
//		BOOL bOccupied = nOccupied & 0x0001;
//		if (bOccupied)  // day  - Occupied
//		{
//			m_application_ctrl.SetCurSel(multi_register_value[454]);//tstat6,7找不到	454
//			// 5E 以及以后的型号
//		}
//		else
//		{
//			m_application_ctrl.SetCurSel(multi_register_value[125]); 
//		}
//// 	}
//// 	else
//// 	{
//// 		m_application_ctrl.SetCurSel(multi_register_value[125]); 
//// 	}
////	UpdateCoolingandHeatingData();
//
//
//
//
//	strTemp.Empty();
//	m_version=get_curtstat_version();
//
//
//	strTemp.Empty();
//	strTemp.Format(_T("%.1f"),multi_register_value[_G("MODBUS_UNIVERSAL_SET")]/10.0);	 //246
//	m_pid_setptEdt2.SetWindowText(strTemp);
//
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_COOLING_PID")]);		//104
//	m_pid_outputEdt1.SetWindowText(strTemp);
//	strTemp.Format(_T("%d"),multi_register_value[_G("MODBUS_PID_UNIVERSAL")]);		//270
//	m_pid_outputEdt2.SetWindowText(strTemp);
//
////tstat6
//	//114	385	1	Low byte	R	COOLING PTERM , proportional term for PI calculation
//
//
//	strTemp.Format(_T("%.1f"),(float)newtstat6[_G("MODBUS_COOLING_PTERM",3)]/10.0);	//385
//	m_pternEdt1.SetWindowText(strTemp);
//
////tstat6
//	//115	386	1	Low byte	W/R	COOLING ITEM
//	strTemp.Format(_T("%.1f"),(float)newtstat6[_G("MODBUS_COOLING_ITERM",3)]/10.0);		//386
//	m_coolingPitemEdt1.SetWindowText(strTemp);
//
//	strTemp.Format(_T("%.1f"),(float)multi_register_value[_G("MODBUS_UNIVERSAL_ITERM")]/10.0);	//245
//	m_pidPitemEdt2.SetWindowText(strTemp);
//
//
//	strTemp.Empty();
//	strTemp.Format(_T("%.1f"),(float)multi_register_value[_G("MODBUS_UNIVERSAL_SET")]/10.0);	//246
//	strTemp+=strUnit;
//	m_dayOccEdt2.SetWindowText(strTemp);
//	if (multi_register_value[MODBUS_INPUT1_SELECT]==3) // humidity	//241
//	{
//		strTemp.Format(_T("%.1f%%"),multi_register_value[422]/10.0);	//422	???????
//		m_dayOccEdt2.SetWindowText(strTemp);
//	}
//
//	strTemp.Empty();
//
////	if(newtstat6[7] == PM_TSTAT7)
////	{
//	//380	160	2	Full	W/R	Calibration for analog input6
//
//		//strTemp.Format(_T("%.1f"),multi_register_value[380]/10.0);//	set point 
//	strTemp.Format(_T("%.1f"),newtstat6[160]/10.0);//	set point 
////	}
//// 	else
//// 	{
//// 		if(m_version<34.9 || multi_register_value[7] == PM_TSTAT5E)   // 老版本135，新版本374// 老版本不除10，
//// 		{
//// 			strTemp.Format(_T("%d"),(int)multi_register_value[135]);
//// 		}
//// 		else
//// 		{
//// 			strTemp.Format(_T("%.1f"),multi_register_value[374]/10.0);	// set point for TStat A,B,C,D
//// 		}	
//// 	}
//
//	m_dayOccEdt1.SetWindowText(strTemp);
//
//	//184	109	1	Low byte	W/R	Info Byte, this register contains info about the state of the tstat.  
//
//	//tstat6
//	// unoccupied and office mode, we should recalc the setpoint here	
//	//short nOccupied = multi_register_value[184];  // Day setpoint option  
//	
////	BOOL bOccupied = nOccupied & 0x0001;
////	if (!bOccupied && multi_register_value[125] == 0)  // unoccupied and office mode
////	{ //DON'T UNDERSTAND  WHY？？？？？？？？？？？？？;
//		float temp =float( (multi_register_value[355]-multi_register_value[354]) / 2.0 + multi_register_value[354] );
//		
//	
//
//		//strTemp.Format(_T("%.1f"),temp);
//		strTemp.Format(_T("%d"),(int)temp);
//	//}
//	m_pid_setptEdt1.SetWindowText(strTemp+strUnit);
//
//
//	//131	365	1	Low byte	W/R	MAX_SETPOINT, Setpoint high, the highest setpoint a user will be able to set from the keypad.
//	//132	366	1	Low byte	W/R	MIN_SETPOINT, Setpoint Low, the lowest setpoint a user will be able to set from the keypad. 
//
////	strTemp.Format(_T("%d"),multi_register_value[131]);
//		strTemp.Format(_T("%d"),newtstat6[_G("MODBUS_MAX_SETPOINT",3)]);//365
//	strTemp+=strUnit;
//	m_setptHiEdit.SetWindowText(strTemp);
//
//	strTemp.Empty();
//	//strTemp.Format(_T("%d"),multi_register_value[132]);
//	strTemp.Format(_T("%d"),newtstat6[_G("MODBUS_MIN_SETPOINT",3)]);//366
//	strTemp+=strUnit;
//	m_setptLoEdit.SetWindowText(strTemp);
//
//
////119	346	1	Low byte	W/R	(Day)Occupied cooling setpoint dead band  , offset from setpoint for cooling to begin.  Units are 0.1 deg.
////		347	1	Low byte	W/R	(Day)Occupied heating setpoint dead band  , offset from setpoint for heating to begin.  Units are 0.1 deg.
//
//
//
////the section below modified by Fance 
//	//strTemp.Empty();
//	//strTemp.Format(_T("%.1f"),multi_register_value[119]/10.0);//119
//	//strTemp+=strUnit;
//	//m_setptCDDEdt1.SetWindowText(strTemp);
//
//	//strTemp.Empty();
//	//strTemp.Format(_T("%.1f"),multi_register_value[243]/10.0);//243
//	//strTemp+=strUnit;
//	//m_setptCCDEdt2.SetWindowText(strTemp);
//	//strTemp.Empty();
//	//strTemp.Format(_T("%.1f"),multi_register_value[120]/10.0);
//	//strTemp+=strUnit;
//	//m_HeadDEdt1.SetWindowText(strTemp);
//	//strTemp.Empty();
//	//strTemp.Format(_T("%.1f"),multi_register_value[242]/10.0);
//	//strTemp+=strUnit;
//	//m_HeadDEdt2.SetWindowText(strTemp);
//
//	//Modify by Fance 2013-03-22 , the last code shows the error information.
//
//	strTemp.Empty();//6  7 没有 MODBUS_COOLING_DEADBAND =119  这一项;
//	m_setptCDDEdt1.SetWindowText(strTemp);
//
//
//	strTemp.Empty();
//	strTemp.Format(_T("%.1f"),newtstat6[_G("MODBUS_UNIVERSAL_DB_LO",3)]/10.0);//243->361
//	strTemp+=strUnit;
//	m_setptCCDEdt2.SetWindowText(strTemp);
//
//	strTemp.Empty();
//	m_HeadDEdt1.SetWindowText(strTemp);//6  7 没有 MODBUS_HEATING_DEADBAND =120  这一项
//	
//
//	strTemp.Empty();
//	strTemp.Format(_T("%.1f"),newtstat6[_G("MODBUS_UNIVERSAL_DB_HI",3)]/10.0);// 242->360
//	strTemp+=strUnit;
//	m_HeadDEdt2.SetWindowText(strTemp);
//
//
//	//The code below is error.Modify by Fance.
//	//CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN1);
//	//pCbx->SetCurSel(multi_register_value[400]);
//	//pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN2);
//	//pCbx->SetCurSel(multi_register_value[401]);
//	//pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_ROUNDDIS);
//	//if(multi_register_value[318] == 0)
//	//{
//	//	pCbx->SetCurSel(0);
//	//}
//	//else if(multi_register_value[318] == 1)
//	//{
//	//	pCbx->SetCurSel(1);
//	//}
//	//else if(multi_register_value[318] == 5)
//	//{
//	//	pCbx->SetCurSel(2);
//	//}
//	//Modify by Fance  2013-03-22
//	CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN1);
//	pCbx->SetCurSel(newtstat6[_G("MODBUS_LCD_SCREEN1",3)]);
//	pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN2);
//	pCbx->SetCurSel(newtstat6[_G("MODBUS_LCD_SCREEN2",3)]);	
//	pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_ROUNDDIS);
//	if(newtstat6[_G("MODBUS_ROUND_DISPLAY",3)] == 0)	//318->405
//	{
//		pCbx->SetCurSel(0);
//	}
//	else if(newtstat6[_G("MODBUS_ROUND_DISPLAY",3)] == 1)//318->405
//	{
//		pCbx->SetCurSel(1);
//	}
//	else if(newtstat6[_G("MODBUS_ROUND_DISPLAY",3)] == 5)//318->405
//	{
//		pCbx->SetCurSel(2);
//	}
//	
//
//
//}
void CParameterDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
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
	int R =0;
	R = write_one(g_tstat_id, MODBUS_VALVE_TRAVEL_TIME,ret, 5);//243	Only Tstats 6 has this function; 
	if (R>0)
	{
		newtstat6[MODBUS_VALVE_TRAVEL_TIME] = ret;	//243
		product_register_value[MODBUS_VALVE_TRAVEL_TIME] = ret;
	}
	else
	{
		MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	}

}

void CParameterDlg::OnEnKillfocusEditPid2offsetpoint()
{
	CString str;
	//str.Format(_T("%d"),multi_register_value[275]);
	GetDlgItem(IDC_EDIT_PID2OFFSETPOINT)->GetWindowText(str);
	int nValue= _wtoi(str);
	int ret =0;
	ret = write_one(g_tstat_id,275,nValue*10,5);
	if (ret>0)
	{
		multi_register_value[275] = nValue;
	}else
	{
		AfxMessageBox(_T("Pleas try again"));
	}



}
