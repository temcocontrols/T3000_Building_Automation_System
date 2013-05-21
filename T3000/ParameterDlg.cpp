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

ON_MESSAGE(MY_RESUME_DATA, ResumeMessageCallBack)

//ON_WM_KILLFOCUS()
ON_WM_CTLCOLOR()
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

	 m_brush.CreateSolidBrush(RGB(255,0,0));
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
	{
		m_idAdressEdit.EnableWindow(FALSE);
		((CButton *)GetDlgItem(IDC_ENABLEIDBUTTON))->SetWindowTextW(_T("Enable Change"));
	}
	else
	{
		m_idAdressEdit.EnableWindow(TRUE);
		((CButton *)GetDlgItem(IDC_ENABLEIDBUTTON))->SetWindowTextW(_T("Disable Change"));
	}
}



void CParameterDlg::OnCbnSelchangeAutoonlycombo()
{
	int nindext=m_autoOnlyCombox.GetCurSel();


	if(product_register_value[MODBUS_AUTO_ONLY]==nindext)	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_AUTO_ONLY,nindext,
		product_register_value[MODBUS_AUTO_ONLY],this->m_hWnd,IDC_AUTOONLYCOMBO,_T("AUTO ONLY"));

	
	// TODO: Add your control notification handler code here
}

//Annull by Fance recode
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

void CParameterDlg::OnCbnSelendcancelBraudratecombo()
{

	if(g_ParamLevel==1)
		return;	

	if(product_register_value[MODBUS_BAUDRATE]==m_braudRateCombox.GetCurSel())	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_BAUDRATE,m_braudRateCombox.GetCurSel(),
		product_register_value[MODBUS_BAUDRATE],this->m_hWnd,IDC_BRAUDRATECOMBO,_T("BAUDRATE"));


	//g_bPauseMultiRead = TRUE;

	//if(write_one(g_tstat_id, MODBUS_BAUDRATE, m_braudRateCombox.GetCurSel())<0)	//Modify by Fance
	//	MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	//else
	//	product_register_value[MODBUS_BAUDRATE] = m_braudRateCombox.GetCurSel();
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


	//g_bPauseMultiRead = TRUE;	
	//if(write_one(g_tstat_id, MODBUS_SPECIAL_MENU_LOCK, m_keyLockCombox.GetCurSel())<0)	//133  396
	//	{MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);g_bPauseMultiRead = FALSE;return;}
	//else
	//	product_register_value[MODBUS_SPECIAL_MENU_LOCK] =  m_keyLockCombox.GetCurSel();
	//g_bPauseMultiRead = FALSE;
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
	if(product_register_value[MODBUS_DISPLAY]==m_displayCombox.GetCurSel())	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_DISPLAY,m_displayCombox.GetCurSel(),
		product_register_value[MODBUS_DISPLAY],this->m_hWnd,IDC_DISPLAYCOMBO,_T("DISPLAY"));

}

void CParameterDlg::OnCbnSelchangeSequencecombox()
{

	if(g_ParamLevel==1)
		return;

	if(product_register_value[MODBUS_SEQUENCE]==m_SequenceCombox.GetCurSel())	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_SEQUENCE,m_SequenceCombox.GetCurSel(),
		product_register_value[MODBUS_SEQUENCE],this->m_hWnd,IDC_SEQUENCECOMBOX,_T("SEQUENCE"));
}



//This Edit is invisible for every product.May be we can delete it.
void CParameterDlg::OnEnKillfocusValveedit()
{

	if(g_ParamLevel==1)
		return;
	CString strTemp;
	m_valveEdit.GetWindowText(strTemp);
	if(strTemp.IsEmpty())
		return;	

	if(product_register_value[MODBUS_VALVE_TRAVEL_TIME]==(int)(_wtof(strTemp)*10))	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_VALVE_TRAVEL_TIME,(int)(_wtof(strTemp)*10),
		product_register_value[MODBUS_VALVE_TRAVEL_TIME],this->m_hWnd,IDC_VALVEEDIT,_T("VALVE TRAVEL TIME"));

}

void CParameterDlg::OnCbnSelchangeHcchangecombo()
{
	if(g_ParamLevel==1)
		return;	
	if(product_register_value[MODBUS_HEAT_COOL_CONFIG]==m_hcChangeCombox.GetCurSel())	//Add this to judge weather this value need to change.
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
	if(product_register_value[MODBUS_POWERUP_SETPOINT]==_wtoi(strTemp))		//if not changed ,return  .Add by Fance
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
	if(product_register_value[MODBUS_SETPOINT_INCREASE] == nvalue)	//if not changed ,return  .Add by Fance
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

	if(product_register_value[MODBUS_DEFAULT_SETPOINT]==_wtoi(strTemp))	//Add this to judge weather this value need to change.
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
	if(product_register_value[MODBUS_FILTER] == ndata) //if not changed ,return  .Add by Fance
		return;//213  142


	if(product_register_value[MODBUS_FILTER]==ndata)	//Add this to judge weather this value need to change.
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
	if(product_register_value[MODBUS_CYCLING_DELAY] ==nValue)	//if not changed ,return  .Add by Fance
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_CYCLING_DELAY,nValue,
		product_register_value[MODBUS_CYCLING_DELAY],this->m_hWnd,IDC_CYCLEEDIT,_T("CYCLING_DELAY"));
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


	

	if(product_register_value[MODBUS_ROTATION_TIME_LEFT]==_wtoi(strTemp))	//Add this to judge weather this value need to change.
			return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_ROTATION_TIME_LEFT,_wtoi(strTemp),
		product_register_value[MODBUS_ROTATION_TIME_LEFT],this->m_hWnd,IDC_TIMERLEFT,_T("ROTATION TIME LEFT"));

}


//modify by Fance 2013 04 11     Fance_0411
void CParameterDlg::OnCbnSelchangeTimerdelectcombo()
{
	if(g_ParamLevel==1)
		return;	

	if(product_register_value[MODBUS_TIMER_SELECT]==m_timerSelectCombox.GetCurSel())	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_TIMER_SELECT,m_timerSelectCombox.GetCurSel(),
		product_register_value[MODBUS_TIMER_SELECT],this->m_hWnd,IDC_TIMERDELECTCOMBO,_T("TIMER_SELECT"));

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

	if(product_register_value[MODBUS_OVERRIDE_TIMER_LEFT] ==nValue)	//if not changed ,return  .
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

	if(product_register_value[MODBUS_OVERRIDE_TIMER] ==nValue)	//if not changed ,return  .
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
	// TODO: Add your control notification handler code here
}


//Recode by Fance
void CParameterDlg::OnCbnSelchangeInputselect1()
{

	if(g_ParamLevel==1)
		return;

	if(product_register_value[MODBUS_TEMP_SELECT]==m_InputSelect1.GetCurSel()+1)	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_TEMP_SELECT,m_InputSelect1.GetCurSel()+1,
		product_register_value[MODBUS_TEMP_SELECT],this->m_hWnd,IDC_INPUTSELECT1,_T("TEMP SELECT"));
}


//Recode by Fance 2013 04 11
void CParameterDlg::OnCbnSelchangeInputselect2()
{
	if(g_ParamLevel==1)
		return;
	int nSel = m_inputSelect2.GetCurSel();	
	if(product_register_value[MODBUS_INPUT1_SELECT]==nSel)	//Add this to judge weather this value need to change.
		return;
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_INPUT1_SELECT,nSel,
		product_register_value[MODBUS_INPUT1_SELECT],this->m_hWnd,IDC_INPUTSELECT2,_T("INPUT1_SELECT"));
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

}

void CParameterDlg::OnEnKillfocusSetvalue2()
{
			
	if(g_ParamLevel==1)
		return;

	CString strText;
	m_pid_setptEdt2.GetWindowText(strText);
	int nValue=_wtoi(strText);
	

	if(product_register_value[MODBUS_UNIVERSAL_SET]==nValue)	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_UNIVERSAL_SET,nValue,
		product_register_value[MODBUS_UNIVERSAL_SET],this->m_hWnd,IDC_SETVALUE2,_T("UNIVERSAL SET"));

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


	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_TIMER_OFF,nValue,
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

	if(product_register_value[MODBUS_MAX_SETPOINT]==nValue)	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_MAX_SETPOINT,nValue,
		product_register_value[MODBUS_MAX_SETPOINT],this->m_hWnd,IDC_ESETPOINTHI,_T("MAX SETPOINT"));

}

void CParameterDlg::OnEnKillfocusEsetpointlo()
{
	if(g_ParamLevel==1)
		return;

	CString strText;
	m_setptLoEdit.GetWindowText(strText);
	int nValue= _wtoi(strText);


	if(product_register_value[MODBUS_MIN_SETPOINT]==nValue)	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_MIN_SETPOINT,nValue,
		product_register_value[MODBUS_MIN_SETPOINT],this->m_hWnd,IDC_ESETPOINTLO,_T("MIN SETPOINT"));
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

		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_DAY_COOLING_DEADBAND,int(nValue *10),
			product_register_value[MODBUS_DAY_COOLING_DEADBAND],this->m_hWnd,IDC_ECOOLDEADBAND1,_T("DAY COOLING DEADBAND"));
	}
	else
	{
		if(nValue == product_register_value[MODBUS_COOLING_DEADBAND]/10)	//119
			return;

		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_COOLING_DEADBAND,int(nValue *10),
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
	float nValue= (float)_wtof(strText);

	g_bPauseMultiRead = TRUE;
	if((product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7))
	{
		if(nValue == product_register_value[MODBUS_DAY_HEATING_DEADBAND]/10)	//347
			return;

		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_DAY_HEATING_DEADBAND,(short)nValue,
			product_register_value[MODBUS_DAY_HEATING_DEADBAND],this->m_hWnd,IDC_ECOOLINGITERM1,_T("DAY HEATING DEADBAND"));
	}
	else
	{
		if(nValue == product_register_value[MODBUS_HEATING_DEADBAND]/10)	//120
			return;

		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_HEATING_DEADBAND,(short)nValue,
			product_register_value[MODBUS_HEATING_DEADBAND],this->m_hWnd,IDC_ECOOLINGITERM1,_T("HEATING DEADBAND"));

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
	int nValue= _wtoi(strText);



	if(m_application_ctrl.GetCurSel()==0)
	{
		if(product_register_value[MODBUS_NIGHT_HEATING_SETPOINT]==nValue)	//Add this to judge weather this value need to change.
			return;

		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_HEATING_SETPOINT,nValue,
			product_register_value[MODBUS_NIGHT_HEATING_SETPOINT],this->m_hWnd,IDC_ENIGNTHEATING,_T("NIGHT HEATING SETPOINT"));
	}
	else
	{
		if(product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]==nValue)	//Add this to judge weather this value need to change.
			return;

		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_HEATING_DEADBAND,nValue,
			product_register_value[MODBUS_NIGHT_HEATING_DEADBAND],this->m_hWnd,IDC_ENIGNTHEATING,_T("NIGHT HEATING DEADBAND"));
	}

}

void CParameterDlg::OnEnKillfocusEnigntcooling1()
{
	
	if(g_ParamLevel==1)
		return;
	CString strText;
	m_nightcooling.GetWindowText(strText);
	int nValue= _wtoi(strText);

	if(m_application_ctrl.GetCurSel()==0)
	{
		if(product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]==nValue)	//Add this to judge weather this value need to change.
			return;

		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_COOLING_SETPOINT,nValue,
			product_register_value[MODBUS_NIGHT_COOLING_SETPOINT],this->m_hWnd,IDC_ENIGNTCOOLING1,_T("NIGHT COOLING SETPOINT"));
	}
	else
	{
		if(product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]==nValue)	//Add this to judge weather this value need to change.
			return;

		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_COOLING_DEADBAND,nValue,
			product_register_value[MODBUS_NIGHT_COOLING_DEADBAND],this->m_hWnd,IDC_ENIGNTCOOLING1,_T("NIGHT COOLING DEADBAND"));
	}
}

BOOL CParameterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN)
		{
			CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
			GetDlgItem(IDC_REFRESHBUTTON)->SetFocus();
			temp_focus->SetFocus();
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
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,380,short(fValue*10),
			product_register_value[380],this->m_hWnd,IDC_SETVALUE1,_T("SETPOINT"));
		//write_one(g_tstat_id, 380,short(fValue*10));
	}
	else if(m_version<34.9 || multi_register_value[7] == PM_TSTAT5E)  // 只有5E使用135
	{
		short nVal = short(fValue);
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,135,nVal,
			product_register_value[380],this->m_hWnd,IDC_SETVALUE1,_T("SETPOINT"));
		//write_one(g_tstat_id, 135, nVal);
	}
	else
	{
		Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,374,short(fValue*10),
			product_register_value[380],this->m_hWnd,IDC_SETVALUE1,_T("SETPOINT"));
		//write_one(g_tstat_id, 374,short(fValue*10));
	}
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
	UpdateData();
	if(product_register_value[MODBUS_DAY_COOLING_SETPOINT]==m_coolsp*10)	//Add this to judge weather this value need to change.
		return;
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_DAY_COOLING_SETPOINT,m_coolsp*10,
		product_register_value[MODBUS_DAY_COOLING_SETPOINT],this->m_hWnd,IDC_EDIT_CSPD,_T("DAY COOLING SETPOINT"));
}

void CParameterDlg::OnEnKillfocusEditCdbdn()
{
	UpdateData();
	if(product_register_value[MODBUS_DAY_COOLING_DEADBAND]==m_cooldb*10)	//Add this to judge weather this value need to change.
		return;
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_DAY_COOLING_DEADBAND,m_cooldb*10,
		product_register_value[MODBUS_DAY_COOLING_DEADBAND],this->m_hWnd,IDC_EDIT_CDBDN,_T("DAY COOLING DEADBAND"));
}

void CParameterDlg::OnEnKillfocusEdit31()
{
	UpdateData();
	if(product_register_value[MODBUS_DAY_SETPOINT]==m_setpoint*10)	//Add this to judge weather this value need to change.
		return;
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_DAY_SETPOINT,m_setpoint*10,
		product_register_value[MODBUS_DAY_SETPOINT],this->m_hWnd,IDC_EDIT31,_T("DAY SETPOINT"));
}

void CParameterDlg::OnEnKillfocusEdit34()
{
	UpdateData();
	if(write_one(g_tstat_id,MODBUS_DAY_HEATING_DEADBAND,m_heatdb*10)<0)//Modify by Fance  2013 04 11
	  MessageBox(_T("Write Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
	else
	product_register_value[MODBUS_DAY_HEATING_DEADBAND] = m_heatdb*10;
}

void CParameterDlg::OnEnKillfocusEdit37()
{
	UpdateData();
	if(product_register_value[MODBUS_DAY_HEATING_SETPOINT]==m_heatsp*10)	//Add this to judge weather this value need to change.
		return;
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_DAY_HEATING_SETPOINT,m_heatsp*10,
		product_register_value[MODBUS_DAY_HEATING_SETPOINT],this->m_hWnd,IDC_EDIT37,_T("DAY HEATING SETPOINT"));
}

void CParameterDlg::OnEnKillfocusEditCspnn()
{
	UpdateData();
	if(product_register_value[MODBUS_NIGHT_COOLING_SETPOINT]==m_coolspN*10)	//Add this to judge weather this value need to change.
		return;
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_COOLING_SETPOINT,m_coolspN*10,
		product_register_value[MODBUS_NIGHT_COOLING_SETPOINT],this->m_hWnd,IDC_EDIT_CSPNN,_T("NIGHT COOLING SETPOINT"));
}

//Recode by Fance
void CParameterDlg::OnEnKillfocusEditCdbnn()
{

	UpdateData();
	if(product_register_value[MODBUS_NIGHT_COOLING_DEADBAND]==m_cooldbN*10)	//Add this to judge weather this value need to change.
		return;
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_COOLING_DEADBAND,m_cooldbN*10,
		product_register_value[MODBUS_NIGHT_COOLING_DEADBAND],this->m_hWnd,IDC_EDIT_CDBNN,_T("NIGHT COOLING DEADBAND"));
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
	UpdateData();

	if(product_register_value[MODBUS_NIGHT_SETPOINT]==m_setpointN*10)	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_SETPOINT,m_setpointN*10,
		product_register_value[MODBUS_NIGHT_SETPOINT],this->m_hWnd,IDC_EDIT32,_T("NIGHT SETPOINT"));
}

void CParameterDlg::OnEnKillfocusEdit35()
{
	UpdateData();
	if(product_register_value[MODBUS_NIGHT_HEATING_DEADBAND]==m_heatdbN*10)	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_HEATING_DEADBAND,m_heatdbN*10,
		product_register_value[MODBUS_NIGHT_HEATING_DEADBAND],this->m_hWnd,IDC_EDIT35,_T("NIGHT HEATING DEADBAND"));
}

void CParameterDlg::OnEnKillfocusEdit38()
{
	UpdateData();
	if(product_register_value[MODBUS_NIGHT_HEATING_SETPOINT] == m_heatspN*10)
		return;
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_NIGHT_HEATING_SETPOINT,m_heatspN*10,
		product_register_value[MODBUS_NIGHT_HEATING_SETPOINT],this->m_hWnd,IDC_EDIT38,_T("NIGHT HEATING SETPOINT"));
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


	if(product_register_value[MODBUS_VALVE_TRAVEL_TIME]==ret)	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_VALVE_TRAVEL_TIME,ret,
		product_register_value[MODBUS_VALVE_TRAVEL_TIME],this->m_hWnd,IDC_EDIT_ValueTravelTime,_T("VALVE TRAVEL TIME"));
}

void CParameterDlg::OnEnKillfocusEditPid2offsetpoint()
{

	CString str;
	//str.Format(_T("%d"),multi_register_value[275]);
	GetDlgItem(IDC_EDIT_PID2OFFSETPOINT)->GetWindowText(str);
	int nValue= _wtoi(str);

	if(product_register_value[275]==nValue*10)	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,275,nValue*10,
		product_register_value[275],this->m_hWnd,IDC_EDIT_PID2OFFSETPOINT,_T("Pid2 off setpoint"));
}

//Add 20130516  by Fance
LRESULT  CParameterDlg::ResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageWriteOneInfo *Write_Struct_feedback =(_MessageWriteOneInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	vector <int>::iterator Iter;
	if(msg_result)
	{
		int indexid = -1;
		for (int i=0;i<(int)Change_Color_ID.size();i++)
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

		SetPaneString(1,temp);
		product_register_value[Write_Struct_feedback->address]= Write_Struct_feedback->new_value;
		if(Write_Struct_feedback!=NULL)
			delete Write_Struct_feedback;
		//MessageBox(temp);
	}
	else
	{
		CString temp;
		temp.Format(_T("Change \"%s\" value from %d to %d Fail!"),
			Write_Struct_feedback->Changed_Name,
			Write_Struct_feedback->old_value,
			Write_Struct_feedback->new_value);
		SetPaneString(1,temp);
		Beep(10,100);
		/*if(MessageBox(temp,_T("Information"),MB_RETRYCANCEL)== IDRETRY )
		{
			Post_Thread_Message(MY_WRITE_ONE,
				Write_Struct_feedback->device_id,
				Write_Struct_feedback->address,
				Write_Struct_feedback->new_value,
				Write_Struct_feedback->old_value,
				Write_Struct_feedback->hwnd,
				Write_Struct_feedback->CTRL_ID,
				Write_Struct_feedback->Changed_Name);
		}
		else
		{*/
			product_register_value[Write_Struct_feedback->address]= Write_Struct_feedback->old_value;
			//GetDlgItem(Write_Struct_feedback->CTRL_ID)->SetWindowTextW(_T(""));

			int indexid = -1;
			for (int i=0;i<(int)Change_Color_ID.size();i++)
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
				//MessageBox(_T("delete Write_Struct_feedback"));
			}
	//	}
	}
	Reflesh_ParameterDlg();
	return 0;
}

HBRUSH CParameterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	for (int i=0;i<(int)Change_Color_ID.size();i++)
	{
		if(/*nCtlColor==CTLCOLOR_EDIT &&*/pWnd->GetDlgCtrlID()==Change_Color_ID.at(i))//注意此处的（pWnd->），否则没效果
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->SetBkColor(RGB(255,0,0));//设置文本背景色
			pDC->SetBkMode(TRANSPARENT);//设置背景透明
			hbr = (HBRUSH)m_brush;

			//((CComboBox *)GetDlgItem(pWnd->GetDlgCtrlID()))->
		}
		//else if()
	}
	
	return hbr;
}
