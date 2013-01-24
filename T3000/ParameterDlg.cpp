// ParameterDlg.cpp : implementation file
//

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


// CParameterDlg dialog

IMPLEMENT_DYNAMIC(CParameterDlg, CDialog)

CParameterDlg::CParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParameterDlg::IDD, pParent)
{
	//memset(&multi_register_value[0],0,sizeof(multi_register_value));
	m_version=1.0;
	pProgess = new CDialog_Progess(this,1,10);

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
	ON_EN_KILLFOCUS(IDC_IDADDRESSEDIT, &CParameterDlg::OnEnKillfocusIdaddressedit)
	ON_WM_DESTROY()
	ON_CBN_KILLFOCUS(IDC_KEYPADSELECT, &CParameterDlg::OnCbnKillfocusKeypadselect)
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

ON_BN_CLICKED(IDCANCEL, &CParameterDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


UINT BackparaFreshProc(LPVOID pParam)
{
	CParameterDlg* pdlg = (CParameterDlg*)pParam;
	
	pdlg->Refresh();
	while(1)
	{
		Sleep(15*1000);
		pdlg->Refresh();

	}

	return 0;
}

BOOL CParameterDlg::OnInitDialog()
{

	pProgess->Create(IDD_DIALOG10_Progress, this);
	pProgess->CenterWindow();
	pProgess->ShowProgress(0,0);
	pProgess->ShowWindow(SW_SHOW);

	pProgess->ShowProgress(4,40);
	//


	//



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



//	Refresh();

	

	if(multi_register_value[283]==2)
	{
		write_one(g_tstat_id,327,3);
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
	Refresh();
	pProgess->ShowProgress(8,80);

	pParamBackFresh = AfxBeginThread(BackparaFreshProc,this);
	pParamBackFresh->m_bAutoDelete =FALSE;

	pProgess->ShowProgress(10,100);

	pProgess->ShowWindow(SW_HIDE);//tstat6
	delete pProgess;
	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CParameterDlg::InitPID2ComboBox()
{
	//m_inputSelect2.Clear();
	
	//ASSERT(m_inputSelect2.GetCount() == 0);
	m_inputSelect2.ResetContent();

	
	if(multi_register_value[7] == PM_PRESSURE)
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

	Refresh();
}

void CParameterDlg::OnBnClickedEnableidbutton()
{
	m_idAdressEdit.EnableWindow(TRUE);
	// TODO: Add your control notification handler code here
}

void CParameterDlg::OnCbnSelchangeAutoonlycombo()
{


	// TODO: Add your control notification handler code here
}
void CParameterDlg::Refresh()
{
	CString strTemp;
	strTemp.Format(_T("%d"),g_tstat_id);
	m_idAdressEdit.SetWindowText(strTemp);
	CString strUnit=GetTempUnit();//Unit string.

	if(multi_register_value[185]>=0&&multi_register_value[185]<=1)
		m_braudRateCombox.SetCurSel(multi_register_value[185]);
	else
		m_braudRateCombox.SetCurSel(-1);

	int nItem;
	nItem = multi_register_value[128];
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

	m_powerupModelCombox.SetCurSel(multi_register_value[127]);

	strTemp.Format(_T("%d"),multi_register_value[285]);
	m_value_percentEdit.SetWindowText(strTemp);



	m_displayCombox.SetCurSel(multi_register_value[203]);
	m_keyLockCombox.SetCurSel(multi_register_value[133]);
	
	m_autoOnlyCombox.SetCurSel(multi_register_value[129]);

	m_SequenceCombox.SetCurSel(multi_register_value[118]);


	strTemp.Format(_T("%.1f"),multi_register_value[279]/10.0);
	m_valveEdit.SetWindowText(strTemp);


	m_hcChangeCombox.SetCurSel(multi_register_value[214]);

	strTemp.Format(_T("%d"),multi_register_value[126]);
	m_powerSetPointEdit.SetWindowText(strTemp);
   

	strTemp.Format(_T("%.1f"),multi_register_value[293]/10.0);
	m_setpointIncreasement.SetWindowText(strTemp);

	strTemp.Format(_T("%d"),multi_register_value[338]);
	m_defSetPointEdit.SetWindowText(strTemp);

//	strTemp.Format(_T("%d"),multi_register_value[339]);
//	m_setPointCtrlEdit.SetWindowText(strTemp);
	if(multi_register_value[339]>=0&&multi_register_value[339]<3)
	{
		m_occupiedSetPointModeCmbox.SetCurSel(multi_register_value[339]);
	}
		
	if(multi_register_value[339]==0||multi_register_value[339]==2)
	{
		m_defSetPointEdit.EnableWindow(FALSE);
	}
	if(multi_register_value[339]==1)
	{
		m_defSetPointEdit.EnableWindow(TRUE);
	}

	strTemp.Format(_T("%d"),multi_register_value[213]);
	m_inputFilterEdit.SetWindowText(strTemp);

	strTemp.Format(_T("%d"),multi_register_value[201]);
	m_cycledlayEdit.SetWindowText(strTemp);


	strTemp.Format(_T("%d"),multi_register_value[301]);
	m_timerOnEdit.SetWindowText(strTemp);

	
	strTemp.Format(_T("%d"),multi_register_value[302]);
	m_timerOffEdit.SetWindowText(strTemp);

	m_uniteCombox.SetCurSel(multi_register_value[303]);

	strTemp.Format(_T("%d"),multi_register_value[333]);
	m_timerLeft.SetWindowText(strTemp);



	if(multi_register_value[283]==2||multi_register_value[284]==2)
	{
		CComboBox* pCombox=(CComboBox*)GetDlgItem(IDC_TIMERDELECTCOMBO);
		int	indext=3;
		write_one(g_tstat_id,327,indext);
		pCombox->SetCurSel(3);

	}
	else
	{

	}

	if(multi_register_value[283]==2)
	{
			
		CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMERON_STATIC);
		ptimerout->SetWindowText(_T("Out4:"));
	}
	else
	{
		CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMERON_STATIC);
		ptimerout->SetWindowText(_T("Timer On:"));
	}

	if(multi_register_value[284]==2)
	{
		
		CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMEOFF_STATIC);
		ptimerout->SetWindowText(_T("Out5:"));

	}
	else
	{
		CStatic* ptimerout=(CStatic*)GetDlgItem(IDC_TIMEOFF_STATIC);
		ptimerout->SetWindowText(_T("Timer Off:"));
	}


	m_timerSelectCombox.SetCurSel(multi_register_value[327]);

	strTemp.Format(_T("%d"),multi_register_value[212]);
	m_TimeEdit.SetWindowText(strTemp);
	strTemp.Format(_T("%d"),multi_register_value[211]);
	m_OverRideEdit.SetWindowText(strTemp);

	m_InputSelect1.SetCurSel(multi_register_value[111]-1);
	m_inputSelect2.SetCurSel(multi_register_value[241]);
	
	strTemp.Format(_T("%.1f"),multi_register_value[101]/10.0);
	m_inputvalue1.SetWindowText(strTemp+strUnit);
	
	//////////////////////////////////////////////////////////////////////////
	// PID2 m_inputValue2 m_inputSelect2 关联设置
	if(multi_register_value[241]==1) // input1
	{	
	    if(multi_register_value[7]== PM_PRESSURE)  // pressure
		{
			strTemp.Format(_T("%.1f W.C"),multi_register_value[180]/100.0);
		}
		else
		{
			if(multi_register_value[188]==4||multi_register_value[188]==1)
			{
				strTemp.Format(_T("%.1f"),(float)multi_register_value[180]/10);
				strTemp=strTemp+strUnit;
			}
			if (strUnit==""||strUnit=="%")
			{
				strTemp.Format(_T("%d"),multi_register_value[180]);
			}
			if(multi_register_value[180]==3)
			{
				if(multi_register_value[180]==0)
					strTemp=_T("OFF");
				if(multi_register_value[180]==1)
					strTemp=_T("ON");
			}
		}
		m_inputValue2.SetWindowText(strTemp);
	}

	if(multi_register_value[241]==2) // input2 //m_inputvalue1
	{
		if(multi_register_value[189]==4||multi_register_value[189]==1)
		{
			strTemp.Format(_T("%.1f"),(float)multi_register_value[181]/10.0);	
			strTemp=strTemp+strUnit;
		}
		if (strUnit==""||strUnit=="%")
		{
			strTemp.Format(_T("%d"),multi_register_value[181]);
		}
		if(multi_register_value[189]==3)
		{
			if(multi_register_value[181]==0)
				strTemp=_T("OFF");
			if(multi_register_value[181]==1)
				strTemp=_T("ON");
		}
	}
	else
	{
//		m_hotelCoolEdt.SetWindowText(_T("N/A"));
	}


	if (multi_register_value[241]==3) // humidity
	{
		strTemp.Format(_T("%.1f%%"),multi_register_value[422]/10.0);
		m_inputValue2.SetWindowText(strTemp);
	}

	//
	//////////////////////////////////////////////////////////////////////////


	if (multi_register_value[7] == PM_TSTAT7 || multi_register_value[7] == PM_TSTAT6)
	{
		short nOccupied = multi_register_value[184];  // Day setpoint option  
		BOOL bOccupied = nOccupied & 0x0001;
		if (bOccupied)  // day  - Occupied
		{
			m_application_ctrl.SetCurSel(multi_register_value[454]);
			 // 5E 以及以后的型号
		}
		else
		{
			m_application_ctrl.SetCurSel(multi_register_value[125]); 
		}
	}
	else
	{
			m_application_ctrl.SetCurSel(multi_register_value[125]); 
	}
	UpdateCoolingandHeatingData();


	//OnCbnSelchangeEapplication();

// 	strTemp.Empty();
// 	int nSelect=multi_register_value[241];
// 	if(nSelect==1)
// 	{
// 		if(multi_register_value[188]==4||multi_register_value[188]==1)
// 		{
// 			strTemp.Format(_T("%.1f"),(float)multi_register_value[180]/10);
// 			strTemp=strTemp+strUnit;
// 		}
// 		if (strUnit==""||strUnit=="%")
// 		{
// 			strTemp.Format(_T("%d"),multi_register_value[180]);
// 		}
// 		if(multi_register_value[180]==3)
// 		{
// 			if(multi_register_value[180]==0)
// 				strTemp=_T("OFF");
// 			if(multi_register_value[180]==1)
// 				strTemp=_T("ON");
// 		}
// 		
// 		m_inputValue2.SetWindowText(strTemp);
// 	}
// 	if(nSelect==2)
// 	{	
// 		if(multi_register_value[189]==4||multi_register_value[189]==1)
// 		{
// 			strTemp.Format(_T("%.1f"),(float)multi_register_value[181]/10.0);	
// 			strTemp=strTemp+strUnit;
// 		}
// 		if (strUnit==""||strUnit=="%")
// 		{
// 			strTemp.Format(_T("%d"),multi_register_value[181]);
// 		}
// 		if(multi_register_value[189]==3)
// 		{
// 			if(multi_register_value[181]==0)
// 				strTemp=_T("OFF");
// 			if(multi_register_value[181]==1)
// 				strTemp=_T("ON");
// 		}
// 		m_inputValue2.SetWindowText(strTemp);
// 	}

	strTemp.Empty();
	m_version=get_curtstat_version();

// 	if (multi_register_value[7] == PM_TSTAT7)
// 	{
// 		strTemp.Format(_T("%.1f"),multi_register_value[380]/10.0);	//	set point 
// 	}
// 	else if(m_version<34.9|| multi_register_value[7] == 16)  // 老版本不除10，
// 	{
// 		strTemp.Format(_T("%d"),(int)multi_register_value[135]);	//	set point 
// 	}
// 	else
// 	{
// 		strTemp.Format(_T("%.1f"),multi_register_value[374]/10.0);		// set point for TStat A,B,C,D
// 	}
// 	m_pid_setptEdt1.SetWindowText(strTemp);

	strTemp.Empty();
	strTemp.Format(_T("%.1f"),multi_register_value[246]/10.0);	
	m_pid_setptEdt2.SetWindowText(strTemp);

	strTemp.Format(_T("%d"),multi_register_value[104]);
	m_pid_outputEdt1.SetWindowText(strTemp);
	strTemp.Format(_T("%d"),multi_register_value[270]);
	m_pid_outputEdt2.SetWindowText(strTemp);
			

	strTemp.Format(_T("%.1f"),(float)multi_register_value[114]/10.0);
	m_pternEdt1.SetWindowText(strTemp);
	
	strTemp.Format(_T("%.1f"),(float)multi_register_value[244]/10.0);	
	m_ptermEdt2.SetWindowText(strTemp);


	strTemp.Format(_T("%.1f"),(float)multi_register_value[115]/10.0);	
	m_coolingPitemEdt1.SetWindowText(strTemp);

	strTemp.Format(_T("%.1f"),(float)multi_register_value[245]/10.0);	
	m_pidPitemEdt2.SetWindowText(strTemp);


	strTemp.Empty();
	strTemp.Format(_T("%.1f"),(float)multi_register_value[246]/10.0);	
	strTemp+=strUnit;
	m_dayOccEdt2.SetWindowText(strTemp);
	if (multi_register_value[241]==3) // humidity
	{
		strTemp.Format(_T("%.1f%%"),multi_register_value[422]/10.0);
		m_dayOccEdt2.SetWindowText(strTemp);
	}

	strTemp.Empty();


// 	if (m_version>=SETPOINT_SPECIAL_VERSION)//
// 	{
// 		strTemp.Format(_T("%.1f"),multi_register_value[374]/10.0);
// 	}
	if(multi_register_value[7] == PM_TSTAT7)
	{
		strTemp.Format(_T("%.1f"),multi_register_value[380]/10.0);//	set point 
	}
	else
	{
		if(m_version<34.9 || multi_register_value[7] == PM_TSTAT5E)   // 老版本135，新版本374// 老版本不除10，
		{
			strTemp.Format(_T("%d"),(int)multi_register_value[135]);
		}
		else
		{
			strTemp.Format(_T("%.1f"),multi_register_value[374]/10.0);	// set point for TStat A,B,C,D
		}	
	}

	m_dayOccEdt1.SetWindowText(strTemp);

	// unoccupied and office mode, we should recalc the setpoint here	
	short nOccupied = multi_register_value[184];  // Day setpoint option  
	BOOL bOccupied = nOccupied & 0x0001;
	if (!bOccupied && multi_register_value[125] == 0)  // unoccupied and office mode
	{ 
		float temp =float( (multi_register_value[183]-multi_register_value[182]) / 2.0 + multi_register_value[182] );
		//strTemp.Format(_T("%.1f"),temp);
		strTemp.Format(_T("%d"),(int)temp);
	}
	m_pid_setptEdt1.SetWindowText(strTemp+strUnit);



		strTemp.Format(_T("%d"),multi_register_value[131]);
		strTemp+=strUnit;
		m_setptHiEdit.SetWindowText(strTemp);

		strTemp.Empty();
		strTemp.Format(_T("%d"),multi_register_value[132]);
		strTemp+=strUnit;
		m_setptLoEdit.SetWindowText(strTemp);

		strTemp.Empty();
		strTemp.Format(_T("%.1f"),multi_register_value[119]/10.0);
		strTemp+=strUnit;
		m_setptCDDEdt1.SetWindowText(strTemp);

		strTemp.Empty();
		strTemp.Format(_T("%.1f"),multi_register_value[243]/10.0);
		strTemp+=strUnit;
		m_setptCCDEdt2.SetWindowText(strTemp);

		strTemp.Empty();
		strTemp.Format(_T("%.1f"),multi_register_value[120]/10.0);
		strTemp+=strUnit;
		m_HeadDEdt1.SetWindowText(strTemp);

		
		strTemp.Empty();
		strTemp.Format(_T("%.1f"),multi_register_value[242]/10.0);
		strTemp+=strUnit;
		m_HeadDEdt2.SetWindowText(strTemp);

		CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN1);
		pCbx->SetCurSel(multi_register_value[400]);
		pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN2);
		pCbx->SetCurSel(multi_register_value[401]);

	
		
		pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_ROUNDDIS);
		if(multi_register_value[318] == 0)
		{
			pCbx->SetCurSel(0);
		}
		else if(multi_register_value[318] == 1)
		{
			pCbx->SetCurSel(1);
		}
		else if(multi_register_value[318] == 5)
		{
			pCbx->SetCurSel(2);
		}


}

void CParameterDlg::OnEnKillfocusIdaddressedit()
{
	CMainFrame* pPraent=(CMainFrame*)GetParent();
	if(g_ParamLevel==1)
		return;
	
	
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

void CParameterDlg::OnCbnKillfocusKeypadselect()
{
	// TODO: Add your control notification handler code here
		
		int nItem=m_keySelectCombox.GetCurSel();

}

void CParameterDlg::OnEnKillfocusValuposedit()
{

	if(g_ParamLevel==1)
		return;

}

void CParameterDlg::OnCbnSelendcancelBraudratecombo()
{

	if(g_ParamLevel==1)
		return;	

}


void CParameterDlg::OnCbnSelchangePowermodelcombo()
{	

	if(g_ParamLevel==1)
		return;	

}


void CParameterDlg::OnCbnSelchangeKeypadlockcombo()
{
		g_bPauseMultiRead = TRUE;	

}

void CParameterDlg::OnCbnSelchangeDisplaycombo()
{

	if(g_ParamLevel==1)
		return;
	g_bPauseMultiRead = TRUE;	


}

void CParameterDlg::OnCbnSelchangeSequencecombox()
{

	if(g_ParamLevel==1)
		return;

}

void CParameterDlg::OnEnKillfocusValveedit()
{

	if(g_ParamLevel==1)
		return;
	CString strTemp;
	m_valveEdit.GetWindowText(strTemp);

}

void CParameterDlg::OnCbnSelchangeHcchangecombo()
{

	if(g_ParamLevel==1)
		return;	

}

void CParameterDlg::OnEnKillfocusPowsetpomitedit()
{

	if(g_ParamLevel==1)
		return;
	CString strTemp;

}

void CParameterDlg::OnEnKillfocusStincreaedit()
{

	if(g_ParamLevel==1)
		return;


}

void CParameterDlg::OnEnKillfocusDefSetpointEdt()
{

	if(g_ParamLevel==1)
		return;
	CString strTemp;

	
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

	// TODO: Add your control notification handler code here
}

void CParameterDlg::OnEnKillfocusCycleedit()
{

	if(g_ParamLevel==1)
		return;

	CString strTemp;

}

void CParameterDlg::OnEnKillfocusTempalarmedit()
{
	//m_tempAlarmEdit
}

void CParameterDlg::OnEnKillfocusTimeronedit()
{
		
	if(g_ParamLevel==1)
		return;
	CString strTemp;

}

void CParameterDlg::OnEnKillfocusEdit12()
{	

	if(g_ParamLevel==1)
		return;

	CString strTemp;

}

void CParameterDlg::OnCbnSelchangeUnitcombo()
{

	if(g_ParamLevel==1)
		return;		

}

void CParameterDlg::OnEnKillfocusTimerleft()
{

if(g_ParamLevel==1)
		return;
	CString strTemp;

}

void CParameterDlg::OnCbnSelchangeTimerdelectcombo()
{

	if(g_ParamLevel==1)
		return;	

}

void CParameterDlg::OnEnKillfocusTimeedit()
{

	if(g_ParamLevel==1)
		return;
	CString strTemp;

}

void CParameterDlg::OnEnKillfocusOverridetimeedit()
{

	if(g_ParamLevel==1)
		return;
	CString strTemp;

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

void CParameterDlg::OnCbnSelchangeInputselect1()
{

	if(g_ParamLevel==1)
		return;
	g_bPauseMultiRead = TRUE;	

}

void CParameterDlg::OnCbnSelchangeInputselect2()
{
	
	if(g_ParamLevel==1)
		return;


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
;
}

void CParameterDlg::OnCbnSelchangeEapplication()
{
	if(g_ParamLevel==1)
		return;

	g_bPauseMultiRead = TRUE;	

}

void  CParameterDlg::UpdateCoolingandHeatingData()
{
	CString strText;
	UINT uint_temp=GetOEMCP();		//get system is for chinese or english'


}

void CParameterDlg::OnEnKillfocusSetvalue2()
{
			
	if(g_ParamLevel==1)
		return;

	CString strText;
	m_pid_setptEdt2.GetWindowText(strText);
	int nValue=_wtoi(strText);

}

void CParameterDlg::OnEnKillfocusOutput1()
{

	if(g_ParamLevel==1)
		return;

	CString strText;
	m_pid_outputEdt1.GetWindowText(strText);

}

void CParameterDlg::OnEnKillfocusOutput2()
{

	if(g_ParamLevel==1)
		return;

	CString strText;
	
}

void CParameterDlg::OnEnKillfocusEcoolingpterm1()
{
	
	if(g_ParamLevel==1)
		return;

	CString strText;
;
}

void CParameterDlg::OnEnKillfocusEcoolingpterm2()
{
	if(g_ParamLevel==1)
		return;

	CString strText;

}

void CParameterDlg::OnEnKillfocusEdit26()
{
	if(g_ParamLevel==1)
		return;

	CString strText;

}

void CParameterDlg::OnEnKillfocusEdit27()
{
	if(g_ParamLevel==1)
		return;

	CString strText;

}
void CParameterDlg::OnEnKillfocusSpset1()
{
	if(g_ParamLevel==1)
		return;

	CString strText;
	m_dayOccEdt1.GetWindowText(strText);
			g_bPauseMultiRead = TRUE;

}

void CParameterDlg::OnEnKillfocusSpset2()
{
	if(g_ParamLevel==1)
		return;

	CString strText;

}

void CParameterDlg::OnEnKillfocusEsetpointhi()
{
	if(g_ParamLevel==1)
		return;

	CString strText;

}

void CParameterDlg::OnEnKillfocusEsetpointlo()
{
	if(g_ParamLevel==1)
		return;

	CString strText;

}

void CParameterDlg::OnEnKillfocusEcooldeadband1()
{
	if(g_ParamLevel==1)
		return;


	g_bPauseMultiRead = FALSE;


}

void CParameterDlg::OnEnKillfocusEcooldeadband2()
{
	if(g_ParamLevel==1)
		return;

	CString strText;


}

void CParameterDlg::OnEnKillfocusEcoolingiterm1()
{
	if(g_ParamLevel==1)
		return;

	CString strText;

}

void CParameterDlg::OnEnKillfocusEcoolingiterm2()
{
	if(g_ParamLevel==1)
		return;

	CString strText;

}

void CParameterDlg::OnBnClickedInputsbutton()
{


	CInputSetDlg dlg;
	dlg.DoModal();	
	InitPID2ComboBox();
	Refresh();


}

void CParameterDlg::OnBnClickedOutputsbutton()
{
	COutputSetDlg dlg;
	dlg.DoModal();
	Refresh();
}

void CParameterDlg::OnBnClickedOutputstablebutton()
{
	COutPutDlg Dlg;
	Dlg.DoModal();
	Refresh();
}
void CParameterDlg::OnEnKillfocusEnigntheating()
{
	if(g_ParamLevel==1)
		return;
	CString strText;
	m_nightheating.GetWindowText(strText);
	int nValue= _wtoi(strText);

	
	g_bPauseMultiRead = FALSE;

}

void CParameterDlg::OnEnKillfocusEnigntcooling1()
{
	
	if(g_ParamLevel==1)
		return;
	CString strText;
	m_nightcooling.GetWindowText(strText);

	
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

g_bPauseMultiRead = FALSE;
}

void CParameterDlg::OnCbnSelchangeOccupiedmodecombo()
{
	if(g_ParamLevel==1)
		return;
	int nItem=m_occupiedSetPointModeCmbox.GetCurSel();

			g_bPauseMultiRead = TRUE;

}

void CParameterDlg::OnCbnSelchangeComboLcdscrn1()
{
	CString index;
	CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN1);
	int nSel = pCbx->GetCurSel();

}

void CParameterDlg::OnCbnSelchangeComboLcdscrn2()
{
	CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_LCDSCRN2);
	int nSel = pCbx->GetCurSel();

}

void CParameterDlg::OnCbnSelchangeComboRounddis()
{
	CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_COMBO_ROUNDDIS);
	int nCursel = pCbx->GetCurSel();

			g_bPauseMultiRead = TRUE;	

}


void CParameterDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
