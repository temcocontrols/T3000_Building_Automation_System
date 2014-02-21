// HumChamber.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "HumChamber.h"
#include  "ADO\ADO.h"
#include "MainFrm.h"
#include <locale.h>
#include "WriteSingleRegDlg.h"
#include "Dialog_Progess.h"
DWORD WINAPI _UpdateThread(LPVOID pParam);

void Register::Set_Reg(Register_info reginfo)
{
   reg_data=reginfo;
}
int Register::Read_Single_Value()
{ 
 return read_one(g_tstat_id,reg_data.Start_ID);
}
BOOL Register::Write_Single_Value(int Value)
{
 int ret;
 write_one(g_tstat_id,reg_data.Start_ID,Value);
 ret=read_one(g_tstat_id,reg_data.Start_ID);
 if (Value==ret)
 {
 return TRUE;
 } 
 else
 {
 return FALSE;
 }
}
// CHumChamber
#define  SENSOR_NUM 104
IMPLEMENT_DYNCREATE(CHumChamber, CFormView)

CHumChamber::CHumChamber()
	: CFormView(CHumChamber::IDD)
	, m_masterid(0)
	, m_slaveid(0)
	, m_temptolerance(0)
	, m_humtolerance(0)
	, m_startpoint(0)
	, m_numsensors(0)
	, m_numbadsensors(0)
	, m_teststate(0)
	, m_sensorTemp(0)
	, m_sensorid(0)
	, m_temp1(0)
	, m_hum1(0)
	, m_time1(0)
	{
		hFirstThread=NULL;
 
		m_ReadOnly=TRUE;
		is_Show_Write_singleDLG=FALSE;
	
    }

CHumChamber::~CHumChamber()
{
if(hFirstThread != NULL)
	TerminateThread(hFirstThread, 0);
	
}

void CHumChamber::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_INPUT3, m_msflexgrid);
	DDX_Text(pDX, IDC_MASTER_ID, m_masterid);
	DDV_MinMaxUInt(pDX, m_masterid, 0, 255);
	DDX_Text(pDX, IDC_SLAVE_ID, m_slaveid);
	DDX_Text(pDX, IDC_TEMP_TOL, m_temptolerance);
	DDX_Text(pDX, IDC_HUM_TOL, m_humtolerance);
	DDX_Text(pDX, IDC_START_POINT, m_startpoint);
	DDX_Text(pDX, IDC_NUM_SENSOR, m_numsensors);
	DDX_Text(pDX, IDC_NUM_ERROR_SENSOR, m_numbadsensors);
	DDX_Text(pDX, IDC_TEST_STATE, m_teststate);
	DDX_Text(pDX, IDC_SENSOR_TEMP, m_sensorTemp);
	DDX_Text(pDX, IDC_SENSOR_ID, m_sensorid);

	DDX_Control(pDX, IDC_START, m_StartBtn);
	DDX_Control(pDX, IDC_STOP, m_StopBtn);
	DDX_Control(pDX, IDC_CONTINUE, m_ContinueBtn);
	//DDX_Control(pDX, IDC_RUNNING, m_Running);
	//DDX_Control(pDX, IDC_RAMPING, m_Ramping);
	//DDX_Control(pDX, IDC_TOLERANCE, m_tolerance);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Control(pDX, IDC_STATIC_TESTSTATUS, m_Static_TestStatus);
	DDX_Control(pDX, IDC_CHECK_1, m_checkBtn1);
	DDX_Control(pDX, IDC_CHECK_2, m_checkBtn2);
	DDX_Control(pDX, IDC_CHECK_3, m_checkBtn3);
	DDX_Control(pDX, IDC_CHECK_4, m_checkBtn4);
	DDX_Control(pDX, IDC_CHECK_5, m_checkBtn5);
	DDX_Control(pDX, IDC_CHECK_6, m_checkBtn6);
	DDX_Control(pDX, IDC_CHECK_7, m_checkBtn7);
	DDX_Control(pDX, IDC_CHECK_8, m_checkBtn8);
	DDX_Control(pDX, IDC_CHECK_9, m_checkBtn9);
	DDX_Control(pDX, IDC_CHECK_10, m_checkBtn10);
	DDX_Control(pDX, IDC_TEST_TEMP1, m_test_temp1);
	DDX_Control(pDX, IDC_TEST_HUM1, m_test_hum1);
	DDX_Control(pDX, IDC_TEST_TEMP2, m_test_temp2);
	DDX_Control(pDX, IDC_TEST_HUM2, m_test_hum2);
	DDX_Control(pDX, IDC_TEST_TEMP3, m_test_temp3);
	DDX_Control(pDX, IDC_TEST_HUM3, m_test_hum3);
}

BEGIN_MESSAGE_MAP(CHumChamber, CFormView)
	ON_BN_CLICKED(IDC_REFRESH, &CHumChamber::OnBnClickedRefresh)
 
 
//ON_EN_KILLFOCUS(IDC_MASTER_ID, &CHumChamber::OnEnKillfocusMasterId)
//ON_EN_KILLFOCUS(IDC_SLAVE_ID, &CHumChamber::OnEnKillfocusSlaveId)
ON_EN_KILLFOCUS(IDC_TEMP1, &CHumChamber::OnEnKillfocusTemp1)
ON_EN_KILLFOCUS(IDC_TEMP2, &CHumChamber::OnEnKillfocusTemp2)
ON_EN_KILLFOCUS(IDC_TEMP3, &CHumChamber::OnEnKillfocusTemp3)
ON_EN_KILLFOCUS(IDC_TEMP4, &CHumChamber::OnEnKillfocusTemp4)
ON_EN_KILLFOCUS(IDC_TEMP5, &CHumChamber::OnEnKillfocusTemp5)
ON_EN_KILLFOCUS(IDC_TEMP6, &CHumChamber::OnEnKillfocusTemp6)
ON_EN_KILLFOCUS(IDC_TEMP7, &CHumChamber::OnEnKillfocusTemp7)
ON_EN_KILLFOCUS(IDC_TEMP8, &CHumChamber::OnEnKillfocusTemp8)
ON_EN_KILLFOCUS(IDC_TEMP9, &CHumChamber::OnEnKillfocusTemp9)
ON_EN_KILLFOCUS(IDC_TEMP10, &CHumChamber::OnEnKillfocusTemp10)

ON_EN_KILLFOCUS(IDC_HUM1, &CHumChamber::OnEnKillfocusHum1)
ON_EN_KILLFOCUS(IDC_HUM2, &CHumChamber::OnEnKillfocusHum2)
ON_EN_KILLFOCUS(IDC_HUM3, &CHumChamber::OnEnKillfocusHum3)
ON_EN_KILLFOCUS(IDC_HUM4, &CHumChamber::OnEnKillfocusHum4)
ON_EN_KILLFOCUS(IDC_HUM5, &CHumChamber::OnEnKillfocusHum5)
ON_EN_KILLFOCUS(IDC_HUM6, &CHumChamber::OnEnKillfocusHum6)
ON_EN_KILLFOCUS(IDC_HUM7, &CHumChamber::OnEnKillfocusHum7)
ON_EN_KILLFOCUS(IDC_HUM8, &CHumChamber::OnEnKillfocusHum8)
ON_EN_KILLFOCUS(IDC_HUM9, &CHumChamber::OnEnKillfocusHum9)
ON_EN_KILLFOCUS(IDC_HUM10, &CHumChamber::OnEnKillfocusHum10)

ON_EN_KILLFOCUS(IDC_TIME1, &CHumChamber::OnEnKillfocusTime1)
ON_EN_KILLFOCUS(IDC_TIME2, &CHumChamber::OnEnKillfocusTime2)
ON_EN_KILLFOCUS(IDC_TIME3, &CHumChamber::OnEnKillfocusTime3)
ON_EN_KILLFOCUS(IDC_TIME4, &CHumChamber::OnEnKillfocusTime4)
ON_EN_KILLFOCUS(IDC_TIME5, &CHumChamber::OnEnKillfocusTime5)
ON_EN_KILLFOCUS(IDC_TIME6, &CHumChamber::OnEnKillfocusTime6)
ON_EN_KILLFOCUS(IDC_TIME7, &CHumChamber::OnEnKillfocusTime7)
ON_EN_KILLFOCUS(IDC_TIME8, &CHumChamber::OnEnKillfocusTime8)
ON_EN_KILLFOCUS(IDC_TIME9, &CHumChamber::OnEnKillfocusTime9)
ON_EN_KILLFOCUS(IDC_TIME10, &CHumChamber::OnEnKillfocusTime10)
ON_EN_KILLFOCUS(IDC_SENSOR_TEMP, &CHumChamber::OnEnKillfocusSensorTemp)
//ON_EN_KILLFOCUS(IDC_SENSOR_ID, &CHumChamber::OnEnKillfocusSensorId)
ON_EN_KILLFOCUS(IDC_HUM_TOL, &CHumChamber::OnEnKillfocusHumTol)
ON_EN_KILLFOCUS(IDC_TEMP_TOL, &CHumChamber::OnEnKillfocusTempTol)
ON_EN_KILLFOCUS(IDC_TEST_STATE, &CHumChamber::OnEnKillfocusTestState)
ON_EN_KILLFOCUS(IDC_START_POINT, &CHumChamber::OnEnKillfocusStartPoint)
ON_EN_KILLFOCUS(IDC_NUM_ERROR_SENSOR, &CHumChamber::OnEnKillfocusNumErrorSensor)
ON_EN_KILLFOCUS(IDC_NUM_SENSOR, &CHumChamber::OnEnKillfocusNumSensor)
ON_BN_CLICKED(IDC_START, &CHumChamber::OnBnClickedStart)
ON_BN_CLICKED(IDC_STOP, &CHumChamber::OnBnClickedStop)
ON_BN_CLICKED(IDC_CONTINUE, &CHumChamber::OnBnClickedContinue)
ON_MESSAGE(WM_MY_MESSAGE,&CHumChamber::my_message)
ON_EN_SETFOCUS(IDC_NUM_SENSOR, &CHumChamber::OnEnSetfocusNumSensor)
 

ON_EN_SETFOCUS(IDC_SLAVE_ID, &CHumChamber::OnEnSetfocusSlaveId)
ON_EN_SETFOCUS(IDC_START_POINT, &CHumChamber::OnEnSetfocusStartPoint)
ON_EN_SETFOCUS(IDC_TEMP_TOL, &CHumChamber::OnEnSetfocusTempTol)
ON_EN_SETFOCUS(IDC_HUM_TOL, &CHumChamber::OnEnSetfocusHumTol)


ON_EN_SETFOCUS(IDC_TEMP1, &CHumChamber::OnEnSetfocusTemp1)
ON_EN_SETFOCUS(IDC_TEMP3, &CHumChamber::OnEnSetfocusTemp3)
ON_EN_SETFOCUS(IDC_TEMP4, &CHumChamber::OnEnSetfocusTemp4)
ON_EN_SETFOCUS(IDC_TEMP5, &CHumChamber::OnEnSetfocusTemp5)
ON_EN_SETFOCUS(IDC_TEMP6, &CHumChamber::OnEnSetfocusTemp6)
ON_EN_SETFOCUS(IDC_TEMP7, &CHumChamber::OnEnSetfocusTemp7)
ON_EN_SETFOCUS(IDC_TEMP8, &CHumChamber::OnEnSetfocusTemp8)
ON_EN_SETFOCUS(IDC_TEMP9, &CHumChamber::OnEnSetfocusTemp9)
ON_EN_SETFOCUS(IDC_TEMP10,&CHumChamber::OnEnSetfocusTemp10)

ON_EN_SETFOCUS(IDC_HUM1, &CHumChamber::OnEnSetfocusHum1)
ON_EN_SETFOCUS(IDC_HUM2, &CHumChamber::OnEnSetfocusHum2)
ON_EN_SETFOCUS(IDC_HUM3, &CHumChamber::OnEnSetfocusHum3)
ON_EN_SETFOCUS(IDC_HUM4, &CHumChamber::OnEnSetfocusHum4)
ON_EN_SETFOCUS(IDC_HUM5, &CHumChamber::OnEnSetfocusHum5)
ON_EN_SETFOCUS(IDC_HUM6, &CHumChamber::OnEnSetfocusHum6)
ON_EN_SETFOCUS(IDC_HUM7, &CHumChamber::OnEnSetfocusHum7)
ON_EN_SETFOCUS(IDC_HUM8, &CHumChamber::OnEnSetfocusHum8)
ON_EN_SETFOCUS(IDC_HUM9, &CHumChamber::OnEnSetfocusHum9)
ON_EN_SETFOCUS(IDC_HUM10,&CHumChamber::OnEnSetfocusHum10)

ON_EN_SETFOCUS(IDC_TEMP2, &CHumChamber::OnEnSetfocusTemp2)


ON_EN_SETFOCUS(IDC_TIME1, &CHumChamber::OnEnSetfocusTime1)
ON_EN_SETFOCUS(IDC_TIME2, &CHumChamber::OnEnSetfocusTime2)
ON_EN_SETFOCUS(IDC_TIME3, &CHumChamber::OnEnSetfocusTime3)
ON_EN_SETFOCUS(IDC_TIME4, &CHumChamber::OnEnSetfocusTime4)
ON_EN_SETFOCUS(IDC_TIME5, &CHumChamber::OnEnSetfocusTime5)
ON_EN_SETFOCUS(IDC_TIME6, &CHumChamber::OnEnSetfocusTime6)
ON_EN_SETFOCUS(IDC_TIME7, &CHumChamber::OnEnSetfocusTime7)
ON_EN_SETFOCUS(IDC_TIME8, &CHumChamber::OnEnSetfocusTime8)
ON_EN_SETFOCUS(IDC_TIME9, &CHumChamber::OnEnSetfocusTime9)
ON_EN_SETFOCUS(IDC_TIME10, &CHumChamber::OnEnSetfocusTime10)

ON_WM_CTLCOLOR()
ON_BN_CLICKED(IDC_CHECK_1, &CHumChamber::OnBnClickedCheck1)
ON_BN_CLICKED(IDC_CHECK_2, &CHumChamber::OnBnClickedCheck2)
ON_BN_CLICKED(IDC_CHECK_3, &CHumChamber::OnBnClickedCheck3)
ON_BN_CLICKED(IDC_CHECK_4, &CHumChamber::OnBnClickedCheck4)
ON_BN_CLICKED(IDC_CHECK_5, &CHumChamber::OnBnClickedCheck5)
ON_BN_CLICKED(IDC_CHECK_6, &CHumChamber::OnBnClickedCheck6)
ON_BN_CLICKED(IDC_CHECK_7, &CHumChamber::OnBnClickedCheck7)
ON_BN_CLICKED(IDC_CHECK_8, &CHumChamber::OnBnClickedCheck8)
ON_BN_CLICKED(IDC_CHECK_9, &CHumChamber::OnBnClickedCheck9)
ON_BN_CLICKED(IDC_CHECK_10, &CHumChamber::OnBnClickedCheck10)
//ON_EN_KILLFOCUS(IDC_TEST_TEMP1, &CHumChamber::OnEnKillfocusTestTemp1)
//ON_EN_KILLFOCUS(IDC_TEST_TEMP2, &CHumChamber::OnEnKillfocusTestTemp2)
//ON_EN_KILLFOCUS(IDC_TEST_TEMP3, &CHumChamber::OnEnKillfocusTestTemp3)
//ON_EN_KILLFOCUS(IDC_TEST_HUM1, &CHumChamber::OnEnKillfocusTestHum1)
//ON_EN_KILLFOCUS(IDC_TEST_HUM2, &CHumChamber::OnEnKillfocusTestHum2)
//ON_EN_KILLFOCUS(IDC_TEST_HUM3, &CHumChamber::OnEnKillfocusTestHum3)
ON_EN_SETFOCUS(IDC_TEST_TEMP1, &CHumChamber::OnEnSetfocusTestTemp1)
ON_EN_SETFOCUS(IDC_TEST_TEMP2, &CHumChamber::OnEnSetfocusTestTemp2)
ON_EN_SETFOCUS(IDC_TEST_TEMP3, &CHumChamber::OnEnSetfocusTestTemp3)
ON_EN_SETFOCUS(IDC_TEST_HUM1, &CHumChamber::OnEnSetfocusTestHum1)
ON_EN_SETFOCUS(IDC_TEST_HUM2, &CHumChamber::OnEnSetfocusTestHum2)
ON_EN_SETFOCUS(IDC_TEST_HUM3, &CHumChamber::OnEnSetfocusTestHum3)
ON_EN_SETFOCUS(IDC_SENSOR_ID, &CHumChamber::OnEnSetfocusSensorId)
END_MESSAGE_MAP()


LRESULT CHumChamber::my_message(WPARAM wParam,LPARAM lParam)
{
	if(wParam ==1)
	{
		SetPaneString(1,g_strScanInfo);
	}
	return 0;
}

void CHumChamber::OnInitialUpdate()
{
 CFormView::OnInitialUpdate();
 m_Start=FALSE;
//hSecondThread=CreateThread(NULL,NULL,_UpdateThread,this,NULL,0);
}

// CHumChamber diagnostics
//初始化23个寄存器号，从数据库中读出来
void CHumChamber::InitialRegisterNo()
{
GetRegInfoFromDB(MasterID,_T("MasterID_RegID"));
GetRegInfoFromDB(SlaveID,_T("SlaveID_RegID"));
GetRegInfoFromDB(CurrentTemp,_T("CurrentTemp_RegID"));
GetRegInfoFromDB(TempTolerence,_T("TempTolerence_RegID"));
GetRegInfoFromDB(NumSensors,_T("NumSensors_RegID"));
GetRegInfoFromDB(StartPoint,_T("StartPoint_RegID"));
GetRegInfoFromDB(CurrentHum,_T("CurrentHum_RegID"));
GetRegInfoFromDB(HumTolenrence,_T("HumTolrence_RegID"));
GetRegInfoFromDB(NumBadSensors,_T("NumBadSensors_RegID"));
GetRegInfoFromDB(TestState,_T("TestState_RegID"));
GetRegInfoFromDB(SensorTemp,_T("SensorTemp_RegID"));
GetRegInfoFromDB(First_Calibration_Points_Temp,_T("First_Calibration_Points_Temp_RegID"));
GetRegInfoFromDB(First_Calibration_Points_Hum,_T("First_Calibration_Points_Hum_RegID"));
GetRegInfoFromDB(First_Calibration_Points_Time,_T("First_Calibration_Points_Time_RegID"));
GetRegInfoFromDB(First_Sensor_Fr,_T("First_Sensor_Fr_RegID"));
GetRegInfoFromDB(First_Sensor_RH,_T("First_Sensor_RH_RegID"));
GetRegInfoFromDB(First_Sensor_State,_T("First_Sensor_State_RegID"));
GetRegInfoFromDB(CurrentTestSensor,_T("CurrentTestSensor_RegID"));
GetRegInfoFromDB(CurrentTestSensorTemp,_T("CurrentTestSensorTemp_RegID"));
GetRegInfoFromDB(CurrentTestSensorHum,_T("CurrentTestSensorHum_RegID"));                                        
GetRegInfoFromDB(CurrentCalibrationTimeLeft,_T("CurrentCalibrationTimeLeft_RegID"));
GetRegInfoFromDB(CurrentCalibrationID,_T("CurrentCalibrationPointID_RegID"));
GetRegInfoFromDB(First_Sensor_Temp_Hum_RegID,_T("First_Sensor_Temp_Hum_RegID"));

GetRegInfoFromDB(TEST_STEP_STATUS_RegID,_T("TEST_STEP_STATUS_RegID"));
GetRegInfoFromDB(TEST_CHOICE_RegID,_T("TEST_CHOICE_RegID"));
GetRegInfoFromDB(First_Test_Temp_Hum_RegID,_T("First_Test_Temp_Hum_RegID"));
 
}
void CHumChamber::Fresh()
{	float progress;
//初始化表格
//初始化寄存器的值
//底下直接应用这些寄存器号码
	CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pMain->m_pFreshMultiRegisters->SuspendThread();
//	pMain->m_pFreshTree->SuspendThread();
	pMain->m_pRefreshThread->SuspendThread();
	 if (!is_connect())
	 {
		pMain->OnConnect();
	 }
	 CDialog_Progess* pDlg = new CDialog_Progess(this,1,100);
	 pDlg->Create(IDD_DIALOG10_Progress, this);
	 pDlg->ShowProgress(0,0);
	 pDlg->ShowWindow(SW_SHOW);
	 RECT RECT_SET1;
	 GetClientRect(&RECT_SET1);
	 pDlg->MoveWindow(RECT_SET1.left+400,RECT_SET1.bottom-19,RECT_SET1.right/2+20,20);

	 InitialRegisterNo();

	register_critical_section.Lock();
	Read_Multi(g_tstat_id,&multi_register_value[0],0,20);  //0-20
	g_register_occuppied=TRUE;
    for(int  i=0;i<6;i++)
	{
		if (pDlg!=NULL)
		{
			progress=float((i+1)*(100/6));
			pDlg->ShowProgress(int(progress),(int)progress);
		}
	Sleep(3000);
	Read_Multi(g_tstat_id,&multi_register_value[581+i*100],581+i*100,100);
	}
   register_critical_section.Unlock();
   g_register_occuppied=FALSE;
   pDlg->ShowWindow(SW_HIDE);
   if(pDlg!=NULL)
   {
   delete pDlg;
   pDlg=NULL;
   }
   FreshGrid();
 
    ShowDialogData();
 Fresh_Checks();
	Update_SensorTable();
	int C_sensor=multi_register_value[CurrentCalibrationID.Start_ID];
	CString tempstr;
	if (multi_register_value[TEST_STEP_STATUS_RegID.Start_ID]>=1&&multi_register_value[TEST_STEP_STATUS_RegID.Start_ID]<=10)
	{
	m_test_status=1;//   
	tempstr.Format(_T("Calibrating Senor:%d T/H Setpoint %0.1f℃/%0.1f"),C_sensor,
	               (float)multi_register_value[First_Calibration_Points_Temp.Start_ID+3*(C_sensor-1)]/10.0,
				   (float)multi_register_value[First_Calibration_Points_Hum.Start_ID+3*(C_sensor-1)]/10.0);
				   tempstr+=_T("%");
	} 
	else if (multi_register_value[TEST_STEP_STATUS_RegID.Start_ID]>=11&&multi_register_value[TEST_STEP_STATUS_RegID.Start_ID]<=13)
	{
	m_test_status=2;
    tempstr.Format(_T("Calibrating Senor:%d T/H Setpoint %0.1f℃/%0.1f"),C_sensor,
	(float)multi_register_value[First_Test_Temp_Hum_RegID.Start_ID+3*(C_sensor-1)]/10.0,
	(float)multi_register_value[First_Test_Temp_Hum_RegID.Start_ID+1+3*(C_sensor-1)]/10.0);
	 tempstr+=_T("%");
	}
	else
	{
	m_test_status=0;
	tempstr=_T("                      Ready");
	}

     if (multi_register_value[TEST_STEP_STATUS_RegID.Start_ID]==255)
   {
	   m_test_status=3;
	   tempstr=_T("                      Finished");
   }
	m_Static_TestStatus.SetWindowText(tempstr);
	

    if (multi_register_value[TestState.Start_ID]==4||multi_register_value[TestState.Start_ID]==5)//Running....started
    {
		m_Start=TRUE;
		m_StartBtn.ShowWindow(FALSE);
		m_StopBtn.ShowWindow(TRUE);
		m_ContinueBtn.ShowWindow(FALSE);
    }
	  if (multi_register_value[TestState.Start_ID]==0)//Stop 状态
	{
		m_Start=FALSE;
		m_StartBtn.ShowWindow(TRUE);
		m_StopBtn.ShowWindow(FALSE);
		m_ContinueBtn.ShowWindow(TRUE);
	}
      if(hFirstThread != NULL)
          TerminateThread(hFirstThread, 0);
      hFirstThread=NULL;
      if (!hFirstThread)
      {
          hFirstThread = CreateThread(NULL,NULL,_UpdateThread,this,NULL,0);
      }
	  
}

#ifdef _DEBUG
void CHumChamber::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CHumChamber::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG
//用多读寄存器的方法把所有寄存器的值读出来
void CHumChamber::Update_AllData()
{
	CString str;
	while(TRUE)
	{
		Sleep(1500);
		if (m_Start)
		{
			 

			Show_AllData();
		}	

	}

}
void CHumChamber::Fresh_Hum_Temp()
{
if (is_connect())
{	 

CString str_master_id;
int times=0;
int Time=0;
BOOL ADD=TRUE;
CTime time;
CString strwrite;
CString str_hum;
short read_temp;

	while(TRUE)
	{
		 if ((!is_connect())||(!g_bChamber))
		 {	 
		 if (!is_connect())
		 {
			 g_strScanInfo =_T("Disconnect");
			 PostMessage(WM_MY_MESSAGE,1,0);
		 } 
		// break;
		 } 
		 
		
	 
	 
		Sleep(1000);
	
		if (ADD)
		{
			times++;
			if (times>=10)
			{
				ADD=FALSE;
			}
			else
			{
				ADD=TRUE;
			}
		}
		else
		{
			times--;
			if (times<=0)
			{
				ADD=TRUE;
			}
			else
			{
				ADD=FALSE;
			}
		}


		read_temp=read_one(g_tstat_id,TestState.Start_ID);
		if (read_temp>0)
		{
		multi_register_value[TestState.Start_ID]=read_temp;
		}
		
		if (multi_register_value[TestState.Start_ID]==4||multi_register_value[TestState.Start_ID]==5)//Running....started
		{
			m_Start=TRUE;
			m_StartBtn.ShowWindow(FALSE);
			m_StopBtn.ShowWindow(TRUE);
			m_ContinueBtn.ShowWindow(FALSE);
		}
		if (multi_register_value[TestState.Start_ID]==0)//Stop 状态
		{
			m_Start=FALSE;
			m_StartBtn.ShowWindow(TRUE);
			m_StopBtn.ShowWindow(FALSE);
			m_ContinueBtn.ShowWindow(TRUE);
		}

	//sleep for communication.
	Sleep(1000);
	read_temp=read_one(g_tstat_id,CurrentTemp.Start_ID);
	if (read_temp>0)
	{multi_register_value[CurrentTemp.Start_ID]=read_temp;
	}
		
		str_master_id.Format(_T("%0.1f°C"),multi_register_value[CurrentTemp.Start_ID]/10.0);
		GetDlgItem(IDC_CUR_TEMP)->SetWindowText(str_master_id);
		read_temp=read_one(g_tstat_id,CurrentHum.Start_ID);
		if (read_temp>0)
		{
		multi_register_value[CurrentHum.Start_ID]=read_temp;
		}
		
		str_master_id.Format(_T("%0.1f%%"), multi_register_value[CurrentHum.Start_ID]/10.0);
		GetDlgItem(IDC_CUR_HUM)->SetWindowText(str_master_id);

		//sleep for communication.
		Sleep(1000);
		read_temp=read_one(g_tstat_id,NumBadSensors.Start_ID);
		if (read_temp>0)
		{
          multi_register_value[NumBadSensors.Start_ID]=read_temp;
		}

		
		str_master_id.Format(_T("%d"),multi_register_value[NumBadSensors.Start_ID]);
		GetDlgItem(IDC_NUM_ERROR_SENSOR)->SetWindowText(str_master_id);
 		if (m_Start)
 		{   
		    Time++;
		    if (Time==10)
		    {
			  Time=0;
			   for(int  i=0;i<4;i++)
			   {
				   Read_Multi(g_tstat_id,&multi_register_value[660+i*100],660+i*100,100);
				   //sleep for communication.
				   Sleep(2000);
			   }
			   

			   FreshGrid();
			   Fresh_Checks();
		    }
		    ShowChangingData();
		}
	 
	}
	//close_com();
	
	return;
}
else
{
  g_strScanInfo=_T("Please Click Connection");
  
  PostMessage(WM_MY_MESSAGE,1,0);
}

return;	 

}
void CHumChamber::FreshGrid(){
	m_msflexgrid.put_Cols(4);
	m_msflexgrid.put_Rows(multi_register_value[NumSensors.Start_ID]+1);
	m_msflexgrid.put_TextMatrix(0,0,_T("Sensor"));
	m_msflexgrid.put_TextMatrix(0,1,_T("State"));
	m_msflexgrid.put_TextMatrix(0,2,_T("TEMP(°C)"));
	m_msflexgrid.put_TextMatrix(0,3,_T("Hum(%)"));
	CString temp,temp1;
	for (int i=1;i<=multi_register_value[NumSensors.Start_ID];i++)
	{
		temp.Format(_T("%d"),i);
		m_msflexgrid.put_TextMatrix(i,0,temp);
		switch (multi_register_value[First_Sensor_State.Start_ID+i-1])
		{
		case 0:
			{
				temp1=_T("Not Tested");
				break;
			}
		case 1:
			{
				temp1=_T("OK");
				break;
			}
		case 2:
			{
				temp1=_T("Bad Hum");
				break;
			}
		case 3:
			{
				temp1=_T("Bad Temp");
				break;
			}
		case 4:
			{
				temp1=_T("Bad Communications");
				break;
			}
		case 5:
			{
				temp1=_T("Testing");
				break;
			}
		default:
			{
				temp1=_T("Not Tested");
				break;
			}
		}
		m_msflexgrid.put_TextMatrix(i,1,temp1);

		temp.Format(_T("%0.1f"),(multi_register_value[First_Sensor_Temp_Hum_RegID.Start_ID+2*(i-1)]/10.0));
		m_msflexgrid.put_TextMatrix(i,2,temp);
		temp.Format(_T("%0.1f"),(multi_register_value[First_Sensor_Temp_Hum_RegID.Start_ID+1+2*(i-1)]/10.0));
		m_msflexgrid.put_TextMatrix(i,3,temp);
	} 

}
void CHumChamber::Show_AllData(){

	 FreshGrid();

	ShowDialogData();
	//Update_SensorTable();
}
void CHumChamber::Update_SensorTable()
{
    
	CString str_master_id;
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID]);
	GetDlgItem(IDC_FREQ1)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+2]);
	GetDlgItem(IDC_FREQ2)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+4]);
	GetDlgItem(IDC_FREQ3)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+6]);
	GetDlgItem(IDC_FREQ4)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+8]);
	GetDlgItem(IDC_FREQ5)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+10]);
	GetDlgItem(IDC_FREQ6)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+12]);
	GetDlgItem(IDC_FREQ7)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+14]);
	GetDlgItem(IDC_FREQ8)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+16]);
	GetDlgItem(IDC_FREQ9)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+18]);
	GetDlgItem(IDC_FREQ10)->SetWindowText(str_master_id);
	 
	 
	//m_progress.SetPos(80);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID]);
	GetDlgItem(IDC_RH1)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+2]);
	GetDlgItem(IDC_RH2)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+4]);
	GetDlgItem(IDC_RH3)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+6]);
	GetDlgItem(IDC_RH4)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+8]);
	GetDlgItem(IDC_RH5)->SetWindowText(str_master_id); 
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+10]);
	GetDlgItem(IDC_RH6)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+12]);
	GetDlgItem(IDC_RH7)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+14]);
	GetDlgItem(IDC_RH8)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+16]);
	GetDlgItem(IDC_RH9)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+18]);
	GetDlgItem(IDC_RH10)->SetWindowText(str_master_id);
//	m_progress.SetPos(90);
 
}
 
// CHumChamber message handlers
void CHumChamber::OnBnClickedRefresh()
{
   if (is_connect())
   { 
   for(int  i=0;i<7;i++)
   {
	   Read_Multi(g_tstat_id,&multi_register_value[660+i*50],660+i*50,50);
   }
   FreshGrid();
   }
   else
   {
    AfxMessageBox(_T("Disconnect"));
   }
  
}

DWORD WINAPI _UpdateThread(LPVOID pParam)
	{
	 	 if (g_bChamber)
	 	 { 
		 CHumChamber* pDlg = (CHumChamber*)(pParam);
		 pDlg->Fresh_Hum_Temp();
	 	 }
	return 1;
	}
 
BOOL CHumChamber::GetRegInfoFromDB(Register_info &reg_data,CString Area_Name)
{
CADO m_cado;  
m_cado.OnInitADOConn();
bool ret = m_cado.IsHaveTable(m_cado,_T("TstatHumChamber"));
if (ret)
	{
	CString strsql;
	strsql=_T("select * from TstatHumChamber where T3000_ID ='");
	strsql+=Area_Name;
	strsql+=_T("'");
 
	m_cado.m_pRecordset=m_cado.OpenRecordset(strsql);
	if (!m_cado.m_pRecordset->EndOfFile)
		{
		reg_data.Destription=m_cado.m_pRecordset->GetCollect("Description");
		reg_data.Length=m_cado.m_pRecordset->GetCollect("Length");
		reg_data.Start_ID=m_cado.m_pRecordset->GetCollect("RegID");
		return TRUE;
		}
	else
		{
		return FALSE;
		}
	} 
else
	{
	return FALSE;
	}
}
void CHumChamber::ShowDialogData()
{
 ////这里是所有单个的 TextArea 


	CString str_master_id;
	str_master_id.Format(_T("%d"), multi_register_value[MasterID.Start_ID]);
	GetDlgItem(IDC_MASTER_ID)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[SlaveID.Start_ID]); 
	GetDlgItem(IDC_SLAVE_ID)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%0.1f°C"),( short)multi_register_value[CurrentTemp.Start_ID]/10.0);
	GetDlgItem(IDC_CUR_TEMP)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%0.1f°C"),( short)multi_register_value[TempTolerence.Start_ID]/10.0);
	GetDlgItem(IDC_TEMP_TOL)->SetWindowText(str_master_id);
    str_master_id.Format(_T("%d"),multi_register_value[NumSensors.Start_ID]);
	GetDlgItem(IDC_NUM_SENSOR)->SetWindowText(str_master_id);
    str_master_id.Format(_T("%d"),multi_register_value[StartPoint.Start_ID]);
	GetDlgItem(IDC_START_POINT)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%0.1f%%"), ( short)multi_register_value[CurrentHum.Start_ID]/10.0);
	GetDlgItem(IDC_CUR_HUM)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%0.1f%%"),( short)multi_register_value[HumTolenrence.Start_ID]/10.0);
	GetDlgItem(IDC_HUM_TOL)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[NumBadSensors.Start_ID]);
	GetDlgItem(IDC_NUM_ERROR_SENSOR)->SetWindowText(str_master_id);
	 
	str_master_id.Format(_T("%d"),multi_register_value[TestState.Start_ID]);
	GetDlgItem(IDC_TEST_STATE)->SetWindowText(str_master_id);
	 
	str_master_id.Format(_T("%d"),multi_register_value[SensorTemp.Start_ID]);
	GetDlgItem(IDC_SENSOR_TEMP)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(multi_register_value[CurrentTestSensor.Start_ID])); 
	      GetDlgItem(IDC_SENSOR_ID)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID]/10.0);
		  GetDlgItem(IDC_TEMP1)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+3]/10.0);
		  GetDlgItem(IDC_TEMP2)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+6]/10.0);
		  GetDlgItem(IDC_TEMP3)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+9]/10.0);
		  GetDlgItem(IDC_TEMP4)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+12]/10.0);
		  GetDlgItem(IDC_TEMP5)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+15]/10.0);
		  GetDlgItem(IDC_TEMP6)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+18]/10.0);
		  GetDlgItem(IDC_TEMP7)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+21]/10.0);
		  GetDlgItem(IDC_TEMP8)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+24]/10.0);
		  GetDlgItem(IDC_TEMP9)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+27]/10.0);
		  GetDlgItem(IDC_TEMP10)->SetWindowText(str_master_id);

		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID]/10.0);
		  GetDlgItem(IDC_HUM1)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+3]/10.0);
		  GetDlgItem(IDC_HUM2)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+6]/10.0);
		  GetDlgItem(IDC_HUM3)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+9]/10.0);
		  GetDlgItem(IDC_HUM4)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+12]/10.0);
		  GetDlgItem(IDC_HUM5)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+15]/10.0);
		  GetDlgItem(IDC_HUM6)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+18]/10.0);
		  GetDlgItem(IDC_HUM7)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+21]/10.0);
		  GetDlgItem(IDC_HUM8)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+24]/10.0);
		  GetDlgItem(IDC_HUM9)->SetWindowText(str_master_id);
		  str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+27]/10.0);
		  GetDlgItem(IDC_HUM10)->SetWindowText(str_master_id);

		  str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID]);
		  GetDlgItem(IDC_TIME1)->SetWindowText(str_master_id);
		  if (multi_register_value[CurrentCalibrationID.Start_ID]>1)
		  {
			  str_master_id.Format(_T("0:0:0"));
			  GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(str_master_id);
		  } 
		  else
		  {
			  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID];
			  Hour=Time_left/60;
			  Minute=Time_left%60;
			  Second=0;
			  str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			  GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(str_master_id);
		  }
		 

		  str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+3]);
		  GetDlgItem(IDC_TIME2)->SetWindowText(str_master_id);
		  if (multi_register_value[CurrentCalibrationID.Start_ID]>2)
		  {
			  str_master_id.Format(_T("0:0:0"));
			  GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(str_master_id);
		  } 
		  else
		  {
			  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+3];
			  Hour=Time_left/60;
			  Minute=Time_left%60;
			  Second=0;
			  str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			  GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(str_master_id);
		  }


		  str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+6]);
		  GetDlgItem(IDC_TIME3)->SetWindowText(str_master_id);
		  if (multi_register_value[CurrentCalibrationID.Start_ID]>3)
		  {
			  str_master_id.Format(_T("0:0:0"));
			  GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(str_master_id);
		  } 
		  else
		  {
		  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+6];
		  Hour=Time_left/60;
		  Minute=Time_left%60;
		  Second=0;
		  str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
		  GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(str_master_id);
		  }
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+9]);
	GetDlgItem(IDC_TIME4)->SetWindowText(str_master_id);
	if (multi_register_value[CurrentCalibrationID.Start_ID]>4)
	{
		str_master_id.Format(_T("0:0:0"));
		GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(str_master_id);
	} 
	else
	{
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+9];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(str_master_id);
	}
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+12]);
	GetDlgItem(IDC_TIME5)->SetWindowText(str_master_id);
	if (multi_register_value[CurrentCalibrationID.Start_ID]>5)
	{
		str_master_id.Format(_T("0:0:0"));
		GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(str_master_id);
	} 
	else
	{
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+12];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(str_master_id);
	}
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+15]);
	GetDlgItem(IDC_TIME6)->SetWindowText(str_master_id);
	if (multi_register_value[CurrentCalibrationID.Start_ID]>6)
	{
		str_master_id.Format(_T("0:0:0"));
		GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(str_master_id);
	} 
	else
	{
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+15];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(str_master_id);
	}
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+18]);
	GetDlgItem(IDC_TIME7)->SetWindowText(str_master_id);
	if (multi_register_value[CurrentCalibrationID.Start_ID]>7)
	{
		str_master_id.Format(_T("0:0:0"));
		GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(str_master_id);
	} 
	else
	{
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+18];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(str_master_id);
	}
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+21]);
	GetDlgItem(IDC_TIME8)->SetWindowText(str_master_id);
	if (multi_register_value[CurrentCalibrationID.Start_ID]>8)
	{
		str_master_id.Format(_T("0:0:0"));
		GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(str_master_id);
	} 
	else
	{
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+21];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(str_master_id);
	}
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+24]);
	GetDlgItem(IDC_TIME9)->SetWindowText(str_master_id);
	if (multi_register_value[CurrentCalibrationID.Start_ID]>9)
	{
		str_master_id.Format(_T("0:0:0"));
		GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(str_master_id);
	} 
	else
	{
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+24];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(str_master_id);}

	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+27]);
	GetDlgItem(IDC_TIME10)->SetWindowText(str_master_id);
	if (multi_register_value[CurrentCalibrationID.Start_ID]>10)
	{
		str_master_id.Format(_T("0:0:0"));
		GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(str_master_id);
	} 
	else
	{
	 Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+27];
	 Hour=Time_left/60;
	 Minute=Time_left%60;
	 Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(str_master_id);}
 //m_progress.SetPos(65);


// Sensor Table
 
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID]);
	GetDlgItem(IDC_FREQ1)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+2]);
	GetDlgItem(IDC_FREQ2)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+4]);
	GetDlgItem(IDC_FREQ3)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+6]);
	GetDlgItem(IDC_FREQ4)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+8]);
	GetDlgItem(IDC_FREQ5)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+10]);
	GetDlgItem(IDC_FREQ6)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+12]);
	GetDlgItem(IDC_FREQ7)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_Fr.Start_ID+14]);
	GetDlgItem(IDC_FREQ8)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[g_tstat_id,First_Sensor_Fr.Start_ID+16]);
	GetDlgItem(IDC_FREQ9)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[g_tstat_id,First_Sensor_Fr.Start_ID+18]);
	GetDlgItem(IDC_FREQ10)->SetWindowText(str_master_id);
 
 
 //m_progress.SetPos(70);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID]);
	GetDlgItem(IDC_RH1)->SetWindowText(str_master_id);
	 str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+2]);
	GetDlgItem(IDC_RH2)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+4]);
	GetDlgItem(IDC_RH3)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+6]);
	GetDlgItem(IDC_RH4)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+8]);
	GetDlgItem(IDC_RH5)->SetWindowText(str_master_id); 
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+10]);
	GetDlgItem(IDC_RH6)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+12]);
	GetDlgItem(IDC_RH7)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+14]);
	GetDlgItem(IDC_RH8)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+16]);
	GetDlgItem(IDC_RH9)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),(signed short)multi_register_value[First_Sensor_RH.Start_ID+18]);
	GetDlgItem(IDC_RH10)->SetWindowText(str_master_id);

    str_master_id.Format(_T("%0.1f"),(float)multi_register_value[First_Test_Temp_Hum_RegID.Start_ID]/10.0);
	m_test_temp1.SetWindowText(str_master_id);
	str_master_id.Format(_T("%0.1f"),(float)multi_register_value[First_Test_Temp_Hum_RegID.Start_ID+1]/10.0);
	m_test_hum1.SetWindowText(str_master_id);
	str_master_id.Format(_T("%0.1f"),(float)multi_register_value[First_Test_Temp_Hum_RegID.Start_ID+2]/10.0);
	m_test_temp2.SetWindowText(str_master_id);
	str_master_id.Format(_T("%0.1f"),(float)multi_register_value[First_Test_Temp_Hum_RegID.Start_ID+3]/10.0);
	m_test_hum2.SetWindowText(str_master_id);
	str_master_id.Format(_T("%0.1f"),(float)multi_register_value[First_Test_Temp_Hum_RegID.Start_ID+4]/10.0);
	m_test_temp3.SetWindowText(str_master_id);
	str_master_id.Format(_T("%0.1f"),(float)multi_register_value[First_Test_Temp_Hum_RegID.Start_ID+5]/10.0);
	m_test_hum3.SetWindowText(str_master_id);

    
}
void CHumChamber::Fresh_Checks(){
BOOL Checked;
Checked=Get_Bit_FromRegister(multi_register_value[TEST_CHOICE_RegID.Start_ID],1);
if (!Checked)
{
 m_checkBtn1.SetCheck(1);
}
else
{
 m_checkBtn1.SetCheck(0);
}
Checked=Get_Bit_FromRegister(multi_register_value[TEST_CHOICE_RegID.Start_ID],2);
if (!Checked)
{
	m_checkBtn2.SetCheck(1);
}
else
{
	m_checkBtn2.SetCheck(0);
}
 
Checked=Get_Bit_FromRegister(multi_register_value[TEST_CHOICE_RegID.Start_ID],3);
if (!Checked)
{
	m_checkBtn3.SetCheck(1);
}
else
{
	m_checkBtn3.SetCheck(0);
}

Checked=Get_Bit_FromRegister(multi_register_value[TEST_CHOICE_RegID.Start_ID],4);
if (!Checked)
{
	m_checkBtn4.SetCheck(1);
}
else
{
	m_checkBtn4.SetCheck(0);
}
Checked=Get_Bit_FromRegister(multi_register_value[TEST_CHOICE_RegID.Start_ID],5);
if (!Checked)
{
	m_checkBtn5.SetCheck(1);
}
else
{
	m_checkBtn5.SetCheck(0);
}
Checked=Get_Bit_FromRegister(multi_register_value[TEST_CHOICE_RegID.Start_ID],6);
if (!Checked)
{
	m_checkBtn6.SetCheck(1);
}
else
{
	m_checkBtn6.SetCheck(0);
}
Checked=Get_Bit_FromRegister(multi_register_value[TEST_CHOICE_RegID.Start_ID],7);
if (!Checked)
{
	m_checkBtn7.SetCheck(1);
}
else
{
	m_checkBtn7.SetCheck(0);
}
Checked=Get_Bit_FromRegister(multi_register_value[TEST_CHOICE_RegID.Start_ID],8);
if (!Checked)
{
	m_checkBtn8.SetCheck(1);
}
else
{
	m_checkBtn8.SetCheck(0);
}
Checked=Get_Bit_FromRegister(multi_register_value[TEST_CHOICE_RegID.Start_ID],9);
if (!Checked)
{
	m_checkBtn9.SetCheck(1);
}
else
{
	m_checkBtn9.SetCheck(0);
}
Checked=Get_Bit_FromRegister(multi_register_value[TEST_CHOICE_RegID.Start_ID],10);
if (!Checked)
{
	m_checkBtn10.SetCheck(1);
}
else
{
	m_checkBtn10.SetCheck(0);
}


if (m_checkBtn1.GetCheck()!=0)
{
	GetDlgItem(IDC_TEMP1)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP1)->EnableWindow(FALSE);
	GetDlgItem(IDC_HUM1)->SetWindowText(_T("0"));
	GetDlgItem(IDC_HUM1)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME1)->SetWindowText(_T("0"));
	GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(_T("0:0:0"));
	GetDlgItem(IDC_TIME1)->EnableWindow(FALSE);
}
else
{   
	GetDlgItem(IDC_TEMP1)->EnableWindow(TRUE);
	GetDlgItem(IDC_HUM1)->EnableWindow(TRUE);
	GetDlgItem(IDC_TIME1)->EnableWindow(TRUE);
	CString str_master_id;
	str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID]/10.0);
	GetDlgItem(IDC_TEMP1)->SetWindowText(str_master_id);


	str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID]/10.0);
	GetDlgItem(IDC_HUM1)->SetWindowText(str_master_id);

	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID]);
	GetDlgItem(IDC_TIME1)->SetWindowText(str_master_id);
	if (multi_register_value[CurrentCalibrationID.Start_ID]>1)
	{
		str_master_id.Format(_T("0:0:0"));
		GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(str_master_id);
	} 
	else
	{
		Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID];
		Hour=Time_left/60;
		Minute=Time_left%60;
		Second=0;
		str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
		GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(str_master_id);
	}
}


 

	if (m_checkBtn2.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP2)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP2)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM2)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM2)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME2)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP2)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM2)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME2)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+3]/10.0);
		GetDlgItem(IDC_TEMP2)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+3]/10.0);
		GetDlgItem(IDC_HUM2)->SetWindowText(str_master_id);

		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+3]);
		GetDlgItem(IDC_TIME2)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>2)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+3];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(str_master_id);
		}

	}

 
	if (m_checkBtn3.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP3)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP3)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM3)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM3)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME3)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME3)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP3)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM3)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME3)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+6]/10.0);
		GetDlgItem(IDC_TEMP3)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+6]/10.0);
		GetDlgItem(IDC_HUM3)->SetWindowText(str_master_id);
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+6]);
		GetDlgItem(IDC_TIME3)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>3)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+6];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(str_master_id);
		}
	}
 
	if (m_checkBtn4.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP4)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP4)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM4)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM4)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME4)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME4)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP4)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM4)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME4)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+9]/10.0);
		GetDlgItem(IDC_TEMP4)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+9]/10.0);
		GetDlgItem(IDC_HUM4)->SetWindowText(str_master_id);
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+9]);
		GetDlgItem(IDC_TIME4)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>4)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+9];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(str_master_id);
		}
	}
 
	if (m_checkBtn5.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP5)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP5)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM5)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM5)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME5)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME5)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP5)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM5)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME5)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+12]/10.0);
		GetDlgItem(IDC_TEMP5)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+12]/10.0);
		GetDlgItem(IDC_HUM5)->SetWindowText(str_master_id); 
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+12]);
		GetDlgItem(IDC_TIME5)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>5)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+12];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(str_master_id);
		}
	}
 
	if (m_checkBtn6.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP6)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP6)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM6)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM6)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME6)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME6)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP6)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM6)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME6)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+15]/10.0);
		GetDlgItem(IDC_TEMP6)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+15]/10.0);
		GetDlgItem(IDC_HUM6)->SetWindowText(str_master_id); 
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+15]);
		GetDlgItem(IDC_TIME6)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>6)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+15];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(str_master_id);
		}
	}
 
	if (m_checkBtn7.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP7)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP7)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM7)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM7)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME7)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME7)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP7)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM7)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME7)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+18]/10.0);
		GetDlgItem(IDC_TEMP7)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+18]/10.0);
		GetDlgItem(IDC_HUM7)->SetWindowText(str_master_id); 
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+18]);
		GetDlgItem(IDC_TIME7)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>7)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+18];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(str_master_id);
		}
	}
 
 
	if (m_checkBtn8.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP8)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP8)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM8)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM8)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME8)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME8)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP8)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM8)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME8)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+21]/10.0);
		GetDlgItem(IDC_TEMP8)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+21]/10.0);
		GetDlgItem(IDC_HUM8)->SetWindowText(str_master_id);
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+21]);
		GetDlgItem(IDC_TIME8)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>8)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+21];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(str_master_id);
		}
	}
 
 
	if (m_checkBtn9.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP9)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP9)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM9)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM9)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME9)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME9)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP9)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM9)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME9)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+24]/10.0);
		GetDlgItem(IDC_TEMP9)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+24]/10.0);
		GetDlgItem(IDC_HUM9)->SetWindowText(str_master_id); 
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+24]);
		GetDlgItem(IDC_TIME9)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>9)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+24];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(str_master_id);
		}
	}
 
 
	 
	if (m_checkBtn10.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP10)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP10)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM10)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM10)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME10)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME10)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP10)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM10)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME10)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+27]/10.0);
		GetDlgItem(IDC_TEMP10)->SetWindowText(str_master_id);
		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+27]/10.0);
		GetDlgItem(IDC_HUM10)->SetWindowText(str_master_id); 
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+27]);
		GetDlgItem(IDC_TIME10)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>10)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+27];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(str_master_id);
		}
	}
 

}
void CHumChamber::ShowChangingData(){

int timeleft,Hour,Minute,Second,read_temp;
	CString str_master_id;
	

read_temp=read_one(g_tstat_id,CurrentCalibrationID.Start_ID);
if (read_temp>0)
{
 multi_register_value[CurrentCalibrationID.Start_ID] =read_temp;
}
int point=multi_register_value[CurrentCalibrationID.Start_ID];

Sleep(1000);
read_temp=read_one(g_tstat_id,CurrentCalibrationTimeLeft.Start_ID);
if (read_temp>0)
{
	multi_register_value[CurrentCalibrationTimeLeft.Start_ID]=read_temp;
}
 
timeleft=multi_register_value[CurrentCalibrationTimeLeft.Start_ID];

Hour=timeleft/3600;
Minute=(timeleft-Hour*3600)/60;
Second=(timeleft-Hour*3600)%60;
str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);

switch(point)
{
case 1:
	{
	  GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(str_master_id);
		break;
	}
case 2:
	{
	GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(str_master_id);
		break;
	}
case 3:
	{
	GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(str_master_id);
		break;
	}
case 4:
	{
	GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(str_master_id);
		break;
	}
case 5:
	{
	GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(str_master_id);
		break;
	}
case 6:
	{
	GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(str_master_id);
		break;
	}
case 7:
	{
	GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(str_master_id);
		break;
	}
case 8:
	{
	GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(str_master_id);
		break;
	}
case 9:
	{
	GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(str_master_id);
		break;
	}
case 10:
	{
	GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(str_master_id);
		break;
	}
}

}

 
BOOL CHumChamber::WriteValueToRegID(UINT RegID,UINT &Value)
	{
	int IntTemp=multi_register_value[RegID];
	if (IntTemp!=Value)
		{
		int ret=write_one(g_tstat_id,RegID,Value);
		 
			if (ret>0)
			{
				multi_register_value[RegID]=Value;
				return TRUE;
			}
			else
			{
			return FALSE;
			}
		}
		return TRUE;
	}


 
void CHumChamber::OnEnKillfocusTemp1()
{
	
	/*Set(First_Calibration_Points_Temp.Start_ID,m_temp1);
	if (!ret)
	{
		AfxMessageBox(_T("Write Fail"));
	}*/
	BOOL flag=TRUE;
	if (flag)
	{
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TEMP1)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		m_temp1=(UINT)temp_float*10;
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Temp.Start_ID,m_temp1);
		////m_writesingleregdlg.DoModal();
		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID]/10.0)); 
		GetDlgItem(IDC_TEMP1)->SetWindowText(str_text);
		flag=FALSE;
	}



	


	//Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,First_Calibration_Points_Temp.Start_ID,m_temp1,
	//	product_register_value[MODBUS_UNIVERSAL_SET],this->m_hWnd,IDC_SETVALUE2,_T("UNIVERSAL SET"));

}


void CHumChamber::OnEnKillfocusTemp2()
	{
	
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TEMP2)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		m_temp2=(UINT)temp_float*10;
		/*Set(First_Calibration_Points_Temp.Start_ID+3,m_temp2);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Temp.Start_ID+3,m_temp2);
		////m_writesingleregdlg.DoModal();;

		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID+3]/10.0)); 
		GetDlgItem(IDC_TEMP2)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusTemp3()
	{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP3)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
	temp_float=(float)_wtof(str_text);
	m_temp3=(UINT)temp_float*10;
   /* Set(First_Calibration_Points_Temp.Start_ID+6,m_temp3);
	if (!ret)
	{
		AfxMessageBox(_T("Write Fail"));
	}*/
	//CWriteSingleRegDlg //m_writesingleregdlg;
	//m_writesingleregdlg.Set(First_Calibration_Points_Temp.Start_ID+6,m_temp3);
	////m_writesingleregdlg.DoModal();;

	str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID+6]/10.0)); 
	GetDlgItem(IDC_TEMP3)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusTemp4()
	{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP4)->GetWindowText(str_text);
	 
	//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
	 
	temp_float=(float)_wtof(str_text);
	m_temp4=(UINT)temp_float*10;
 /*  Set(First_Calibration_Points_Temp.Start_ID+9,m_temp4);
   if (!ret)
   {
	   AfxMessageBox(_T("Write Fail"));
   }*/
   //CWriteSingleRegDlg //m_writesingleregdlg;
	//m_writesingleregdlg.Set(First_Calibration_Points_Temp.Start_ID+9,m_temp4);
	////m_writesingleregdlg.DoModal();;

	str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID+9]/10.0)); 
	 
	GetDlgItem(IDC_TEMP4)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusTemp5()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+12,m_temp5))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TEMP5)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		m_temp5=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Temp.Start_ID+12,m_temp5);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Temp.Start_ID+12,m_temp5);
		////m_writesingleregdlg.DoModal();;

		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID+12]/10.0)); 
		GetDlgItem(IDC_TEMP5)->SetWindowText(str_text);
	 
	}

void CHumChamber::OnEnKillfocusTemp6()
	{
	// TODO: Add your control notification handler code here
	/*UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+15,m_temp6))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);*/
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TEMP6)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		m_temp6=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Temp.Start_ID+15,m_temp6);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Temp.Start_ID+15,m_temp6);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID+15]/10.0)); 
		GetDlgItem(IDC_TEMP6)->SetWindowText(str_text);
	}


void CHumChamber::OnEnKillfocusTemp7()
	{
	// TODO: Add your control notification handler code here
	/*UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+18,m_temp7))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);*/
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TEMP7)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		m_temp7=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Temp.Start_ID+18,m_temp7);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Temp.Start_ID+18,m_temp7);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID+18]/10.0)); 
		GetDlgItem(IDC_TEMP7)->SetWindowText(str_text);
	 
	}
void CHumChamber::OnEnKillfocusTemp8()
	{
	/*UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+21,m_temp8))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);*/

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TEMP8)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		m_temp8=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Temp.Start_ID+21,m_temp8);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Temp.Start_ID+21,m_temp8);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID+21]/10.0)); 
		GetDlgItem(IDC_TEMP8)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusTemp9()
	{
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+24,m_temp9))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TEMP9)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		m_temp9=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Temp.Start_ID+24,m_temp9);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Temp.Start_ID+24,m_temp9);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID+24]/10.0)); 
		GetDlgItem(IDC_TEMP9)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusTemp10()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+27,m_temp10))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
	 
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TEMP10)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		m_temp10=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Temp.Start_ID+27,m_temp10);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Temp.Start_ID+27,m_temp10);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID+27]/10.0)); 
		GetDlgItem(IDC_TEMP10)->SetWindowText(str_text);
 
}

void CHumChamber::OnEnKillfocusHum1()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID,m_hum1))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM1)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum1=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Hum.Start_ID,m_hum1);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Hum.Start_ID,m_hum1);
		////m_writesingleregdlg.DoModal();;

		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Hum.Start_ID]/10.0)); 
		GetDlgItem(IDC_HUM1)->SetWindowText(str_text);

	}
void CHumChamber::OnEnKillfocusHum2()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+3,m_hum2))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM2)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum2=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Hum.Start_ID+3,m_hum2);
		if (!ret)
		{
		AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Hum.Start_ID+3,m_hum2);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Hum.Start_ID+3]/10.0)); 
		GetDlgItem(IDC_HUM2)->SetWindowText(str_text);

	}
void CHumChamber::OnEnKillfocusHum3()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+6,m_hum3))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM3)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum3=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Hum.Start_ID+6,m_hum3);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Hum.Start_ID+6,m_hum3);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Hum.Start_ID+6]/10.0)); 
		GetDlgItem(IDC_HUM3)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusHum4()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+9,m_hum4))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM4)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum4=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Hum.Start_ID+9,m_hum4);
		if (!ret)
		{
		AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Hum.Start_ID+9,m_hum4);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Hum.Start_ID+9]/10.0)); 
		GetDlgItem(IDC_HUM4)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusHum5()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+12,m_hum5))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM5)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum5=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Hum.Start_ID+12,m_hum5);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Hum.Start_ID+12,m_hum5);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Hum.Start_ID+12]/10.0)); 
		GetDlgItem(IDC_HUM5)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusHum6()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+15,m_hum6))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM6)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum6=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Hum.Start_ID+15,m_hum6);
		if (!ret)
		{
		AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Hum.Start_ID+15,m_hum6);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Hum.Start_ID+15]/10.0)); 
		GetDlgItem(IDC_HUM6)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusHum7()
	{
	// TODO: Add your control notification handler code here
	/*UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+18,m_hum7))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);*/

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM7)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum7=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Hum.Start_ID+18,m_hum7);

		if (!ret)
		{
		AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Hum.Start_ID+18,m_hum7);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Hum.Start_ID+18]/10.0)); 
		GetDlgItem(IDC_HUM7)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusHum8()
	{
	// TODO: Add your control notification handler code here
	/*UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+21,m_hum8))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);*/

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM8)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum8=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Hum.Start_ID+21,m_hum8);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Hum.Start_ID+21,m_hum8);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Hum.Start_ID+21]/10.0)); 
		GetDlgItem(IDC_HUM8)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusHum9()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+24,m_hum9))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM9)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum9=(UINT)(temp_float*10);
		/*Set(First_Calibration_Points_Hum.Start_ID+24,m_hum9);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Hum.Start_ID+24,m_hum9);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Hum.Start_ID+24]/10.0)); 
		GetDlgItem(IDC_HUM9)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusHum10()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+27,m_hum10))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM10)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum10=(UINT)temp_float*10;
		/*Set(First_Calibration_Points_Hum.Start_ID+27,m_hum10);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Hum.Start_ID+27,m_hum10);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Hum.Start_ID+27]/10.0)); 
		GetDlgItem(IDC_HUM10)->SetWindowText(str_text);
	}


 
void CHumChamber::OnEnKillfocusTime1()
	{
	// TODO: Add your control notification handler code here
	
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID,m_time1))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 

	//
	//UpdateData(FALSE);
    //Set the time of calibration.
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME1)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time1=(UINT)(temp_float);
		/*Set(First_Calibration_Points_Time.Start_ID,m_time1);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Time.Start_ID,m_time1);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%d"),(multi_register_value[First_Calibration_Points_Time.Start_ID])); 
		GetDlgItem(IDC_TIME1)->SetWindowText(str_text);
//show the left time
	CString str_master_id;
	Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID];
	Hour=Time_left/60;
	Minute=Time_left%60;
	Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(str_master_id);
	}
void CHumChamber::OnEnKillfocusTime2()
	{
	//// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+3,m_time2))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME2)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time2=(UINT)temp_float;
		/*Set(First_Calibration_Points_Time.Start_ID+3,m_time2);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Time.Start_ID+3,m_time2);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%d"),(multi_register_value[First_Calibration_Points_Time.Start_ID+3])); 
		GetDlgItem(IDC_TIME2)->SetWindowText(str_text);

	CString str_master_id;
	Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+3];
	Hour=Time_left/60;
	Minute=Time_left%60;
	Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(str_master_id);
	}
void CHumChamber::OnEnKillfocusTime3()
	{
 

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME3)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time3=(UINT)temp_float;
		/*Set(First_Calibration_Points_Time.Start_ID+6,m_time3);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Time.Start_ID+6,m_time3);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%d"),(multi_register_value[First_Calibration_Points_Time.Start_ID+6])); 
		GetDlgItem(IDC_TIME3)->SetWindowText(str_text);

	CString str_master_id;
	Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+6];
	Hour=Time_left/60;
	Minute=Time_left%60;
	Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(str_master_id);
	}
void CHumChamber::OnEnKillfocusTime4()
	{
 
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME4)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time4=(UINT)temp_float;
		/*Set(First_Calibration_Points_Time.Start_ID+9,m_time4);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Time.Start_ID+9,m_time4);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%d"),(multi_register_value[First_Calibration_Points_Time.Start_ID+9])); 
		GetDlgItem(IDC_TIME4)->SetWindowText(str_text);

	CString str_master_id;
	Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+9];
	Hour=Time_left/60;
	Minute=Time_left%60;
	Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(str_master_id);
	}
void CHumChamber::OnEnKillfocusTime5()
	{
 

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME5)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time5=(UINT)temp_float;
		/*Set(First_Calibration_Points_Time.Start_ID+12,m_time5);

		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Time.Start_ID+12,m_time5);
		////m_writesingleregdlg.DoModal();;
		
		str_text.Format(_T("%d"),(multi_register_value[First_Calibration_Points_Time.Start_ID+12])); 
		GetDlgItem(IDC_TIME5)->SetWindowText(str_text);

	CString str_master_id;
	Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+12];
	Hour=Time_left/60;
	Minute=Time_left%60;
	Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(str_master_id);
	}
void CHumChamber::OnEnKillfocusTime6()
	{
 

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME6)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time6=(UINT)temp_float;
		/*Set(First_Calibration_Points_Time.Start_ID+15,m_time6);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Time.Start_ID+15,m_time6);
		////m_writesingleregdlg.DoModal();;

		str_text.Format(_T("%d"),(multi_register_value[First_Calibration_Points_Time.Start_ID+15])); 
		GetDlgItem(IDC_TIME6)->SetWindowText(str_text);

	CString str_master_id;
	Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+15];
	Hour=Time_left/60;
	Minute=Time_left%60;
	Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(str_master_id);
	}
void CHumChamber::OnEnKillfocusTime7()
	{
 

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME7)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time7=(UINT)temp_float;
		/*Set(First_Calibration_Points_Time.Start_ID+18,m_time7);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Time.Start_ID+18,m_time7);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%d"),(multi_register_value[First_Calibration_Points_Time.Start_ID+18])); 
		GetDlgItem(IDC_TIME7)->SetWindowText(str_text);

	CString str_master_id;
	Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+18];
	Hour=Time_left/60;
	Minute=Time_left%60;
	Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(str_master_id);
	}
void CHumChamber::OnEnKillfocusTime8()
	{
 

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME8)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time8=(UINT)temp_float;
		/*Set(First_Calibration_Points_Time.Start_ID+21,m_time8);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Time.Start_ID+21,m_time8);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%d"),(multi_register_value[First_Calibration_Points_Time.Start_ID+21])); 
		GetDlgItem(IDC_TIME8)->SetWindowText(str_text);

	CString str_master_id;
	Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+21];
	Hour=Time_left/60;
	Minute=Time_left%60;
	Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(str_master_id);
	}
void CHumChamber::OnEnKillfocusTime9()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+24,m_time9))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME9)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time9=(UINT)temp_float;
		/*Set(First_Calibration_Points_Time.Start_ID+24,m_time9);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Time.Start_ID+24,m_time9);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%d"),(multi_register_value[First_Calibration_Points_Time.Start_ID+24])); 
		GetDlgItem(IDC_TIME9)->SetWindowText(str_text);


	CString str_master_id;
	Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+24];
	Hour=Time_left/60;
	Minute=Time_left%60;
	Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(str_master_id);
	}
void CHumChamber::OnEnKillfocusTime10()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+27,m_time10))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
   

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME10)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time10=(UINT)temp_float;
		/*Set(First_Calibration_Points_Time.Start_ID+27,m_time10);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(First_Calibration_Points_Time.Start_ID+27,m_time10);
		////m_writesingleregdlg.DoModal();;


		str_text.Format(_T("%d"),(multi_register_value[First_Calibration_Points_Time.Start_ID+27])); 
		GetDlgItem(IDC_TIME10)->SetWindowText(str_text);


	CString str_master_id;
	Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+27];
	Hour=Time_left/60;
	Minute=Time_left%60;
	Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(str_master_id);
	}


void CHumChamber::OnEnKillfocusSensorTemp()
	{

	}

void CHumChamber::OnEnKillfocusHumTol()
	{

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM_TOL)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_humtolerance=(UINT)(temp_float*10);
		/*Set(HumTolenrence.Start_ID,m_humtolerance);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		//m_writesingleregdlg.Set(HumTolenrence.Start_ID,m_humtolerance);
		////m_writesingleregdlg.DoModal();;

		str_text.Format(_T("%0.1f%%"),(multi_register_value[HumTolenrence.Start_ID]/10.0)); 
		GetDlgItem(IDC_HUM_TOL)->SetWindowText(str_text);

	}


void CHumChamber::OnEnKillfocusTempTol()
{   float temp_float;
    CString str_text;
	GetDlgItem(IDC_TEMP_TOL)->GetWindowText(str_text);
   // AfxMessageBox(str_text);
	//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
	//AfxMessageBox(str_text);
	temp_float=(float)_wtof(str_text);
	m_temptolerance=(UINT)(temp_float*10);
 //Set(TempTolerence.Start_ID,m_temptolerance);
 //if (!ret)
 //{
	// AfxMessageBox(_T("Write Fail"));
 //}
 //CWriteSingleRegDlg //m_writesingleregdlg;
	//m_writesingleregdlg.Set(TempTolerence.Start_ID,m_temptolerance);
	 
	////m_writesingleregdlg.DoModal();;

	str_text.Format(_T("%0.1f°C"),(float)multi_register_value[TempTolerence.Start_ID]/10.0);
	 
	GetDlgItem(IDC_TEMP_TOL)->SetWindowText(str_text);
}


void CHumChamber::OnEnKillfocusTestState()
{
		UpdateData(TRUE);
		if (!WriteValueToRegID(TestState.Start_ID,m_teststate))
		{
			AfxMessageBox(_T("Try again"));
		} 
		UpdateData(FALSE);
}


void CHumChamber::OnEnKillfocusStartPoint()
	{
	/*UpdateData(TRUE);IDC_START_POINT
	if (!WriteValueToRegID(StartPoint.Start_ID,m_startpoint))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);*/

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_START_POINT)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_startpoint=(UINT)temp_float;
		/*Set(StartPoint.Start_ID,m_startpoint);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}*/
		//CWriteSingleRegDlg //m_writesingleregdlg;
		 
		//m_writesingleregdlg.Set(StartPoint.Start_ID,m_startpoint);
		////m_writesingleregdlg.DoModal();

		str_text.Format(_T("%d"),(multi_register_value[StartPoint.Start_ID])); 
		GetDlgItem(IDC_START_POINT)->SetWindowText(str_text);
	}

	//Read_Only
void CHumChamber::OnEnKillfocusNumErrorSensor()
	{
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(CurrentTestSensor.Start_ID,m_sensorid))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
	}


BEGIN_EVENTSINK_MAP(CHumChamber, CFormView)
	ON_EVENT(CHumChamber, IDC_MSFLEXGRID_INPUT3, DISPID_DBLCLICK, CHumChamber::DblClickMsflexgridInput3, VTS_NONE)
//    ON_EVENT(CHumChamber, IDC_MSFLEXGRID_INPUT3, DISPID_KEYUP, CHumChamber::KeyUpMsflexgridInput3, VTS_PI2 VTS_I2)
//ON_EVENT(CHumChamber, IDC_MSFLEXGRID_INPUT3, DISPID_KEYDOWN, CHumChamber::KeyDownMsflexgridInput3, VTS_PI2 VTS_I2)
//ON_EVENT(CHumChamber, IDC_MSFLEXGRID_INPUT3, 70, CHumChamber::RowColChangeMsflexgridInput3, VTS_NONE)
ON_EVENT(CHumChamber, IDC_MSFLEXGRID_INPUT3, DISPID_CLICK, CHumChamber::ClickMsflexgridInput3, VTS_NONE)
END_EVENTSINK_MAP()


void CHumChamber::DblClickMsflexgridInput3()
{

long lRow,lCol;
lRow = m_msflexgrid.get_RowSel();//获取点击的行号	
lCol = m_msflexgrid.get_ColSel(); //获取点击的列号
if(lRow==0)
	return;
CRect rect;
m_msflexgrid.GetWindowRect(rect); //获取表格控件的窗口矩形
ScreenToClient(rect); //转换为客户区矩形	
// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
//需要将其转化为像素，1440缇= 1英寸
CDC* pDC =GetDC();
//计算象素点和缇的转换比例
int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
//计算选中格的左上角的坐标(象素为单位)
long y = m_msflexgrid.get_RowPos(lRow)/nTwipsPerDotY;
long x = m_msflexgrid.get_ColPos(lCol)/nTwipsPerDotX;
//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
long width = m_msflexgrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
long height = m_msflexgrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
//形成选中个所在的矩形区域
CRect rc(x,y,x+width,y+height);
//转换成相对对话框的坐标
rc.OffsetRect(rect.left+1,rect.top+1);
//获取选中格的文本信息	
CString strValue = m_msflexgrid.get_TextMatrix(lRow,lCol);
m_nCurRow=lRow;
m_nCurCol=lCol;
///////////////先刷新当前主表信息
if (is_connect())
{ 
	for(int  i=0;i<7;i++)
	{
		Read_Multi(g_tstat_id,&multi_register_value[660+i*50],660+i*50,50);
	}
	FreshGrid();
}
else
{
	AfxMessageBox(_T("Disconnect"));
}

/////////////////////////
//write_one(g_tstat_id,CurrentTestSensor.Start_ID,m_nCurRow);

}





void CHumChamber::OnBnClickedStart()
	{
 
		int ret=write_one(g_tstat_id,TestState.Start_ID,2);
		if (ret>0)
		{
			m_Start=TRUE;
			m_StartBtn.ShowWindow(FALSE);
			m_StopBtn.ShowWindow(TRUE);
			m_ContinueBtn.ShowWindow(FALSE);
			 
		}
		
		 
	 

	}


 


void CHumChamber::OnBnClickedStop()
	{
	// TODO: Add your control notification handler code here
 
	    int ret=write_one(g_tstat_id,TestState.Start_ID,0);
		if (ret>0)
		{
		m_Start=FALSE;
		m_StartBtn.ShowWindow(TRUE);
		m_StopBtn.ShowWindow(FALSE);
		m_ContinueBtn.ShowWindow(TRUE);
		}

	 
 
	}


void CHumChamber::OnBnClickedContinue()
	{
	
	  int ret= write_one(g_tstat_id,TestState.Start_ID,1);
	  if (ret>0)
		  {
		  m_Start=FALSE;
		  m_StartBtn.ShowWindow(FALSE);
		  m_StopBtn.ShowWindow(TRUE);
		  m_ContinueBtn.ShowWindow(FALSE);
		}
	}
 
void CHumChamber::ClickMsflexgridInput3()
{
	long lRow,lCol;
	lRow = m_msflexgrid.get_RowSel();//获取点击的行号	
	lCol = m_msflexgrid.get_ColSel(); //获取点击的列号
	if(lRow==0)
		return;
	CRect rect;
	m_msflexgrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_msflexgrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_msflexgrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_msflexgrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_msflexgrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息	
	CString strValue = m_msflexgrid.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;
	if (m_nCurCol==3)
	{
	  Set(628,_wtof(strValue)*10);
	}
	if (m_nCurCol==2)
	{
	   Set(627,_wtof(strValue)*10);
	}
	if (m_nCurCol==0)
	{
		UINT temp=m_nCurRow;
		CString str_text;
		int ret=WriteValueToRegID(CurrentTestSensor.Start_ID,temp);

		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
		str_text.Format(_T("%d"),(multi_register_value[CurrentTestSensor.Start_ID])); 
		GetDlgItem(IDC_SENSOR_ID)->SetWindowText(str_text);

		Sleep(2000);
		Read_Multi(g_tstat_id,&multi_register_value[633],633,20);
		Update_SensorTable();
	}
}
void CHumChamber::OnEnSetfocusNumSensor()
{
	 //  AfxMessageBox(_T("OK"));
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_NUM_SENSOR)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	m_numsensors=(UINT)temp_float;
	////CWriteSingleRegDlg //m_writesingleregdlg;
	////m_writesingleregdlg.Set(NumSensors.Start_ID,m_numsensors); 
	//////m_writesingleregdlg.DoModal();
	m_ID=NumSensors.Start_ID;
	m_Value=m_numsensors;
	is_Show_Write_singleDLG=TRUE;
}
	//////////////////////////////////////////////////////////
void CHumChamber::OnEnKillfocusNumSensor()
	{
	 


		//float temp_float;
		//CString str_text;
		//GetDlgItem(IDC_NUM_SENSOR)->GetWindowText(str_text);
		////str_text.Delete(str_text.GetLength()-1,1);//delete % 
		//temp_float=(float)_wtof(str_text);
		//m_numsensors=(UINT)temp_float;
		//////CWriteSingleRegDlg //m_writesingleregdlg;
		//////m_writesingleregdlg.Set(NumSensors.Start_ID,m_numsensors); 
		////////m_writesingleregdlg.DoModal();
		//m_ID=NumSensors.Start_ID;
		//m_Value=m_numsensors;
	/*	str_text.Format(_T("%d"),(multi_register_value[NumSensors.Start_ID])); 
		GetDlgItem(IDC_NUM_SENSOR)->SetWindowText(str_text);*/

	 	//FreshGrid(); 
	}
 BOOL CHumChamber::PreTranslateMessage(MSG* pMsg)
 {
    											  
	 if(pMsg->message == WM_KEYDOWN  )
	 {
	     
		 if(pMsg->wParam == VK_F5)
		 {
		    if (is_Show_Write_singleDLG)
		    {
				CWriteSingleRegDlg m_writesingleregdlg;
				m_writesingleregdlg.Set(m_ID,m_Value); 
				m_writesingleregdlg.DoModal();

				ShowDialogData();
				FreshGrid();
				is_Show_Write_singleDLG=FALSE;
		    }
			
			 return 1;
		 }
	     

	 }	 
   return CFormView::PreTranslateMessage(pMsg);
 }

void CHumChamber::Set(int id,int value){
m_ID=id;
m_Value=value;
}
void CHumChamber::OnEnSetfocusSlaveId()
{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_SLAVE_ID)->GetWindowText(str_text);

	temp_float=(float)_wtof(str_text);
	m_slaveid=(UINT)temp_float;
	//BOOL ret=WriteValueToRegID(SlaveID.Start_ID,m_slaveid);
	Set(SlaveID.Start_ID,m_slaveid);
	 
	is_Show_Write_singleDLG=TRUE;
}
void CHumChamber::OnEnSetfocusStartPoint()
{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_START_POINT)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	m_startpoint=(UINT)temp_float;
	Set(StartPoint.Start_ID,m_startpoint);
	is_Show_Write_singleDLG=TRUE;
}
void CHumChamber::OnEnSetfocusTempTol()
{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP_TOL)->GetWindowText(str_text);
	// AfxMessageBox(str_text);
	//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
	//AfxMessageBox(str_text);
	temp_float=(float)_wtof(str_text);
	m_temptolerance=(UINT)(temp_float*10);
	Set(TempTolerence.Start_ID,m_temptolerance);
	is_Show_Write_singleDLG=TRUE;
}
void CHumChamber::OnEnSetfocusHumTol()
{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_HUM_TOL)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-1,1);//delete % 
	temp_float=(float)_wtof(str_text);
	m_humtolerance=(UINT)(temp_float*10);
	Set(HumTolenrence.Start_ID,m_humtolerance);
	is_Show_Write_singleDLG=TRUE;
}

void CHumChamber::OnEnSetfocusTemp1()
{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP1)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
	temp_float=(float)_wtof(str_text);
	m_temp1=(UINT)temp_float*10;
	Set(First_Calibration_Points_Temp.Start_ID,m_temp1);
	is_Show_Write_singleDLG=TRUE;
}
void CHumChamber::OnEnSetfocusTemp2()
{

	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP2)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
	temp_float=(float)_wtof(str_text);
	m_temp2=(UINT)temp_float*10;
	Set(First_Calibration_Points_Temp.Start_ID+3,m_temp2);
	is_Show_Write_singleDLG=TRUE;
}
void CHumChamber::OnEnSetfocusTemp3()
{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP3)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
	temp_float=(float)_wtof(str_text);
	m_temp3=(UINT)temp_float*10;
	Set(First_Calibration_Points_Temp.Start_ID+6,m_temp3);
	is_Show_Write_singleDLG=TRUE;
}
void CHumChamber::OnEnSetfocusTemp4()
{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP4)->GetWindowText(str_text);

	//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符

	temp_float=(float)_wtof(str_text);
	m_temp4=(UINT)temp_float*10;
	Set(First_Calibration_Points_Temp.Start_ID+9,m_temp4);
	is_Show_Write_singleDLG=TRUE;
}
void CHumChamber::OnEnSetfocusTemp5()
{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+12,m_temp5))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);

	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP5)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
	temp_float=(float)_wtof(str_text);
	m_temp5=(UINT)(temp_float*10);
	Set(First_Calibration_Points_Temp.Start_ID+12,m_temp5);
	is_Show_Write_singleDLG=TRUE;

}
void CHumChamber::OnEnSetfocusTemp6()
{
	// TODO: Add your control notification handler code here
	/*UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+15,m_temp6))
	{
	AfxMessageBox(_T("Try again"));
	} 
	UpdateData(FALSE);*/
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP6)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
	temp_float=(float)_wtof(str_text);
	m_temp6=(UINT)(temp_float*10);
	Set(First_Calibration_Points_Temp.Start_ID+15,m_temp6);
	is_Show_Write_singleDLG=TRUE;
}
void CHumChamber::OnEnSetfocusTemp7()
{
	// TODO: Add your control notification handler code here
	/*UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+18,m_temp7))
	{
	AfxMessageBox(_T("Try again"));
	} 
	UpdateData(FALSE);*/
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP7)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
	temp_float=(float)_wtof(str_text);
	m_temp7=(UINT)(temp_float*10);
	Set(First_Calibration_Points_Temp.Start_ID+18,m_temp7);

	is_Show_Write_singleDLG=TRUE;
}
void CHumChamber::OnEnSetfocusTemp8()
{
	/*UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+21,m_temp8))
	{
	AfxMessageBox(_T("Try again"));
	} 
	UpdateData(FALSE);*/

	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP8)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
	temp_float=(float)_wtof(str_text);
	m_temp8=(UINT)(temp_float*10);
	Set(First_Calibration_Points_Temp.Start_ID+21,m_temp8);
	is_Show_Write_singleDLG=TRUE;
}
void CHumChamber::OnEnSetfocusTemp9()
	{
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+24,m_temp9))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TEMP9)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		m_temp9=(UINT)(temp_float*10);
		Set(First_Calibration_Points_Temp.Start_ID+24,m_temp9);
		is_Show_Write_singleDLG=TRUE;
	}
void CHumChamber::OnEnSetfocusTemp10()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+27,m_temp10))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
	 
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TEMP10)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		m_temp10=(UINT)(temp_float*10);
		Set(First_Calibration_Points_Temp.Start_ID+27,m_temp10);
	
 is_Show_Write_singleDLG=TRUE;
}

void CHumChamber::OnEnSetfocusHum1()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID,m_hum1))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM1)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum1=(UINT)(temp_float*10);
		Set(First_Calibration_Points_Hum.Start_ID,m_hum1);
		is_Show_Write_singleDLG=TRUE;

	}
void CHumChamber::OnEnSetfocusHum2()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+3,m_hum2))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM2)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum2=(UINT)(temp_float*10);
		Set(First_Calibration_Points_Hum.Start_ID+3,m_hum2);
	
	is_Show_Write_singleDLG=TRUE;
	}
void CHumChamber::OnEnSetfocusHum3()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+6,m_hum3))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM3)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum3=(UINT)(temp_float*10);
		Set(First_Calibration_Points_Hum.Start_ID+6,m_hum3);
		is_Show_Write_singleDLG=TRUE;
	}
void CHumChamber::OnEnSetfocusHum4()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+9,m_hum4))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM4)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum4=(UINT)(temp_float*10);
		Set(First_Calibration_Points_Hum.Start_ID+9,m_hum4);
	is_Show_Write_singleDLG=TRUE;
	}
void CHumChamber::OnEnSetfocusHum5()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+12,m_hum5))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM5)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum5=(UINT)(temp_float*10);
		Set(First_Calibration_Points_Hum.Start_ID+12,m_hum5);
		is_Show_Write_singleDLG=TRUE;
	}
void CHumChamber::OnEnSetfocusHum6()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+15,m_hum6))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM6)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum6=(UINT)(temp_float*10);
		Set(First_Calibration_Points_Hum.Start_ID+15,m_hum6);
is_Show_Write_singleDLG=TRUE;
	}
void CHumChamber::OnEnSetfocusHum7()
	{
	// TODO: Add your control notification handler code here
	/*UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+18,m_hum7))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);*/

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM7)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum7=(UINT)(temp_float*10);
		Set(First_Calibration_Points_Hum.Start_ID+18,m_hum7);
	is_Show_Write_singleDLG=TRUE;
	}
void CHumChamber::OnEnSetfocusHum8()
	{
	// TODO: Add your control notification handler code here
	/*UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+21,m_hum8))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);*/

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM8)->GetWindowText(str_text);
	//	str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum8=(UINT)(temp_float*10);
		Set(First_Calibration_Points_Hum.Start_ID+21,m_hum8);
		is_Show_Write_singleDLG=TRUE;
	}
void CHumChamber::OnEnSetfocusHum9()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+24,m_hum9))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM9)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum9=(UINT)(temp_float*10);
		Set(First_Calibration_Points_Hum.Start_ID+24,m_hum9);
		is_Show_Write_singleDLG=TRUE;
	}
void CHumChamber::OnEnSetfocusHum10()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+27,m_hum10))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM10)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_hum10=(UINT)temp_float*10;
		Set(First_Calibration_Points_Hum.Start_ID+27,m_hum10);
		is_Show_Write_singleDLG=TRUE;
	}


 
void CHumChamber::OnEnSetfocusTime1()
	{
	// TODO: Add your control notification handler code here
	
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID,m_time1))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 

	//
	//UpdateData(FALSE);
    //Set the time of calibration.
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME1)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time1=(UINT)(temp_float);
		Set(First_Calibration_Points_Time.Start_ID,m_time1);
		is_Show_Write_singleDLG=TRUE;
	}
void CHumChamber::OnEnSetfocusTime2()
	{
	//// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+3,m_time2))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME2)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time2=(UINT)temp_float;
		Set(First_Calibration_Points_Time.Start_ID+3,m_time2);
		is_Show_Write_singleDLG=TRUE;
	}
void CHumChamber::OnEnSetfocusTime3()
	{
 

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME3)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time3=(UINT)temp_float;
		Set(First_Calibration_Points_Time.Start_ID+6,m_time3);
		is_Show_Write_singleDLG=TRUE;
	}
void CHumChamber::OnEnSetfocusTime4()
	{
 
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME4)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time4=(UINT)temp_float;
		Set(First_Calibration_Points_Time.Start_ID+9,m_time4);
		is_Show_Write_singleDLG=TRUE;
	
	}
void CHumChamber::OnEnSetfocusTime5()
	{
 

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME5)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time5=(UINT)temp_float;
		Set(First_Calibration_Points_Time.Start_ID+12,m_time5);
		is_Show_Write_singleDLG=TRUE;
	
	}
void CHumChamber::OnEnSetfocusTime6()
	{
 

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME6)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time6=(UINT)temp_float;
		Set(First_Calibration_Points_Time.Start_ID+15,m_time6);
		is_Show_Write_singleDLG=TRUE;
		
	}
void CHumChamber::OnEnSetfocusTime7()
	{
 

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME7)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time7=(UINT)temp_float;
		Set(First_Calibration_Points_Time.Start_ID+18,m_time7);
		is_Show_Write_singleDLG=TRUE;

	}
void CHumChamber::OnEnSetfocusTime8()
	{
 

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME8)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time8=(UINT)temp_float;
		Set(First_Calibration_Points_Time.Start_ID+21,m_time8);
		is_Show_Write_singleDLG=TRUE;
		
	}
void CHumChamber::OnEnSetfocusTime9()
	{
	// TODO: Add your control notification handler code here
	//UpdateData(TRUE);
	//if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+24,m_time9))
	//	{
	//	AfxMessageBox(_T("Try again"));
	//	} 
	//UpdateData(FALSE);
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME9)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time9=(UINT)temp_float;
		Set(First_Calibration_Points_Time.Start_ID+24,m_time9);
		is_Show_Write_singleDLG=TRUE;

	}
void CHumChamber::OnEnSetfocusTime10()
	{
	 
   

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TIME10)->GetWindowText(str_text);
		temp_float=(float)_wtof(str_text);
		m_time10=(UINT)temp_float;
		Set(First_Calibration_Points_Time.Start_ID+27,m_time10);
		is_Show_Write_singleDLG=TRUE;
}

HBRUSH CHumChamber::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	switch(pWnd->GetDlgCtrlID())
	{
	 case IDC_STATIC_TESTSTATUS:
		 {
			 if (m_test_status==0)
			 {
				 pDC->SetTextColor(RGB(255,255,255));
				 pDC->SetBkColor(RGB(0,0,255));
				 pDC->SetBkMode(TRANSPARENT);
				 HBRUSH B = CreateSolidBrush(RGB(0,0,255));
				 return (HBRUSH)B;
			 } 
			 else if (m_test_status==1)
			 {
				 pDC->SetTextColor(RGB(255,255,255));
				 pDC->SetBkColor(RGB(0,0,255));
				 pDC->SetBkMode(TRANSPARENT);
				 HBRUSH B = CreateSolidBrush(RGB(255,0,255));
				 return (HBRUSH)B;
			 }
			 else if (m_test_status==2)
			 {
				 pDC->SetTextColor(RGB(255,255,255));
				 pDC->SetBkColor(RGB(0,0,255));
				 pDC->SetBkMode(TRANSPARENT);
				 HBRUSH B = CreateSolidBrush(RGB(0,139,0));
				 return (HBRUSH)B;
			 }
			 else
			 {
				 pDC->SetTextColor(RGB(255,255,255));
				 pDC->SetBkColor(RGB(0,0,255));
				 pDC->SetBkMode(TRANSPARENT);
				 HBRUSH B = CreateSolidBrush(RGB(137,137,137));
				 return (HBRUSH)B;
			 }

		 } 
	 case  IDC_STATIC_WARNING:
	 {
		 pDC->SetTextColor(RGB(255,255,255));
		 pDC->SetBkColor(RGB(0,0,255));
		 pDC->SetBkMode(TRANSPARENT);
		 HBRUSH B = CreateSolidBrush(RGB(255,0,0));
		 return (HBRUSH)B;
	 }
	}
	// TODO:  Return a different brush if the default is not desired
 return hbr;
}


void CHumChamber::OnBnClickedCheck1()
{  
     
   unsigned short VLV=Get_Checks();
   int ret= write_one(g_tstat_id,TEST_CHOICE_RegID.Start_ID,VLV);
  
   if (ret<0)
   {
	   if (m_checkBtn1.GetCheck()!=0)
	   {
		   m_checkBtn1.SetCheck(0);
	   }
	   else
	   {
		   m_checkBtn1.SetCheck(1);
	   }
	    
   AfxMessageBox(_T("Try again!"));
   return;
   } 
    
  multi_register_value[TEST_CHOICE_RegID.Start_ID]=VLV;
   if (m_checkBtn1.GetCheck()!=0)
   {
	   GetDlgItem(IDC_TEMP1)->SetWindowText(_T("0"));
	   GetDlgItem(IDC_TEMP1)->EnableWindow(FALSE);
	   GetDlgItem(IDC_HUM1)->SetWindowText(_T("0"));
	   GetDlgItem(IDC_HUM1)->EnableWindow(FALSE);
	   GetDlgItem(IDC_TIME1)->SetWindowText(_T("0"));
	   GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(_T("0:0:0"));
	   GetDlgItem(IDC_TIME1)->EnableWindow(FALSE);
   }
   else
   {   
	   GetDlgItem(IDC_TEMP1)->EnableWindow(TRUE);
	   GetDlgItem(IDC_HUM1)->EnableWindow(TRUE);
	   GetDlgItem(IDC_TIME1)->EnableWindow(TRUE);
	   CString str_master_id;
	   str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID]/10.0);
	   GetDlgItem(IDC_TEMP1)->SetWindowText(str_master_id);


	   str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID]/10.0);
	   GetDlgItem(IDC_HUM1)->SetWindowText(str_master_id);

	   str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID]);
	   GetDlgItem(IDC_TIME1)->SetWindowText(str_master_id);
	   if (multi_register_value[CurrentCalibrationID.Start_ID]>1)
	   {
		   str_master_id.Format(_T("0:0:0"));
		   GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(str_master_id);
	   } 
	   else
	   {
		   Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID];
		   Hour=Time_left/60;
		   Minute=Time_left%60;
		   Second=0;
		   str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
		   GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(str_master_id);
	   }
   }
	 
}
void CHumChamber::OnBnClickedCheck2()
{
	 
	unsigned short VLV= Get_Checks();
	int ret= write_one(g_tstat_id,TEST_CHOICE_RegID.Start_ID,VLV);
	if (ret<0)
	{  
 	 if (m_checkBtn2.GetCheck()!=0)
 	{
 		m_checkBtn2.SetCheck(0);
 	}
 	else
 	{
 		m_checkBtn2.SetCheck(1);
 	}
		 
			 

		  
		AfxMessageBox(_T("Try again!"));
		return;
	} 

	multi_register_value[TEST_CHOICE_RegID.Start_ID]=VLV;

	if (m_checkBtn2.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP2)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP2)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM2)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM2)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME2)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP2)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM2)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME2)->EnableWindow(TRUE);
		CString str_master_id;
	 
		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+3]/10.0);
		GetDlgItem(IDC_TEMP2)->SetWindowText(str_master_id);
		 

	 
		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+3]/10.0);
		GetDlgItem(IDC_HUM2)->SetWindowText(str_master_id);

		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+3]);
		GetDlgItem(IDC_TIME2)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>2)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+3];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(str_master_id);
		}
		 
	}

}
void CHumChamber::OnBnClickedCheck3()
{ 
unsigned short VLV=Get_Checks();
int ret= write_one(g_tstat_id,TEST_CHOICE_RegID.Start_ID,VLV);
if (ret<0)
{ 
 	if (m_checkBtn3.GetCheck()!=0)
 	{
 		m_checkBtn3.SetCheck(0);
 	}
 	else
 	{
 		m_checkBtn3.SetCheck(1);
 	}
 
	AfxMessageBox(_T("Try again!"));
	return;
} 

multi_register_value[TEST_CHOICE_RegID.Start_ID]=VLV;
	if (m_checkBtn3.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP3)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP3)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM3)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM3)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME3)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME3)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP3)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM3)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME3)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+6]/10.0);
		GetDlgItem(IDC_TEMP3)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+6]/10.0);
		GetDlgItem(IDC_HUM3)->SetWindowText(str_master_id);
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+6]);
		GetDlgItem(IDC_TIME3)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>3)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+6];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(str_master_id);
		}
	}
}
void CHumChamber::OnBnClickedCheck4()
{ 
unsigned short VLV=Get_Checks();
int ret= write_one(g_tstat_id,TEST_CHOICE_RegID.Start_ID,VLV);
if (ret<0)
{
 if (m_checkBtn4.GetCheck()!=0)
 {
 	m_checkBtn4.SetCheck(0);
 }
 else
 {
 	m_checkBtn4.SetCheck(1);
 }
	 
	AfxMessageBox(_T("Try again!"));
	return;
} 

multi_register_value[TEST_CHOICE_RegID.Start_ID]=VLV;
	if (m_checkBtn4.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP4)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP4)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM4)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM4)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME4)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME4)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP4)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM4)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME4)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+9]/10.0);
		GetDlgItem(IDC_TEMP4)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+9]/10.0);
		GetDlgItem(IDC_HUM4)->SetWindowText(str_master_id);
		 str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+9]);
		GetDlgItem(IDC_TIME4)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>4)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+9];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(str_master_id);
		}
	}
}
void CHumChamber::OnBnClickedCheck5()
{ 
unsigned short VLV=Get_Checks();
int ret= write_one(g_tstat_id,TEST_CHOICE_RegID.Start_ID,VLV);
if (ret<0){
	if (m_checkBtn5.GetCheck()!=0)
	{
		m_checkBtn5.SetCheck(0);
	}
	else
	{
		m_checkBtn5.SetCheck(1);
	}
	 
	AfxMessageBox(_T("Try again!"));
	return;
} 

multi_register_value[TEST_CHOICE_RegID.Start_ID]=VLV;
	if (m_checkBtn5.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP5)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP5)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM5)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM5)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME5)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME5)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP5)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM5)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME5)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+12]/10.0);
		GetDlgItem(IDC_TEMP5)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+12]/10.0);
		GetDlgItem(IDC_HUM5)->SetWindowText(str_master_id); 
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+12]);
		GetDlgItem(IDC_TIME5)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>5)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+12];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(str_master_id);
		}
	}
}
void CHumChamber::OnBnClickedCheck6()
{ 
unsigned short VLV=Get_Checks();
int ret= write_one(g_tstat_id,TEST_CHOICE_RegID.Start_ID,VLV);
if (ret<0)
{ 
	if (m_checkBtn6.GetCheck()!=0)
	{
		m_checkBtn6.SetCheck(0);
	}
	else
	{
		m_checkBtn6.SetCheck(1);
	}
	AfxMessageBox(_T("Try again!"));
	return;
} 

multi_register_value[TEST_CHOICE_RegID.Start_ID]=VLV;
	if (m_checkBtn6.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP6)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP6)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM6)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM6)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME6)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME6)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP6)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM6)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME6)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+15]/10.0);
		GetDlgItem(IDC_TEMP6)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+15]/10.0);
		GetDlgItem(IDC_HUM6)->SetWindowText(str_master_id); 
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+15]);
		GetDlgItem(IDC_TIME6)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>6)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+15];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(str_master_id);
		}
	}
}
void CHumChamber::OnBnClickedCheck7()
{
unsigned short VLV=Get_Checks();
int ret= write_one(g_tstat_id,TEST_CHOICE_RegID.Start_ID,VLV);
if (ret<0){
	if (m_checkBtn7.GetCheck()!=0)
	{
		m_checkBtn7.SetCheck(0);
	}
	else
	{
		m_checkBtn7.SetCheck(1);
	}
Fresh_Checks();	AfxMessageBox(_T("Try again!"));
	return;
} 

multi_register_value[TEST_CHOICE_RegID.Start_ID]=VLV;
	if (m_checkBtn7.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP7)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP7)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM7)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM7)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME7)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME7)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP7)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM7)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME7)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+18]/10.0);
		GetDlgItem(IDC_TEMP7)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+18]/10.0);
		GetDlgItem(IDC_HUM7)->SetWindowText(str_master_id); 
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+18]);
		GetDlgItem(IDC_TIME7)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>7)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+18];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(str_master_id);
		}
	}
}
void CHumChamber::OnBnClickedCheck8()
{ 
unsigned short VLV= Get_Checks();
int ret= write_one(g_tstat_id,TEST_CHOICE_RegID.Start_ID,VLV);
if (ret<0)
{ 
 if (m_checkBtn8.GetCheck()!=0)
 {
 	m_checkBtn8.SetCheck(0);
 }
 else
 {
 	m_checkBtn8.SetCheck(1);
 }
/*Fresh_Checks();*/AfxMessageBox(_T("Try again!"));
return;
} 

multi_register_value[TEST_CHOICE_RegID.Start_ID]=VLV;
if (m_checkBtn8.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP8)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP8)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM8)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM8)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME8)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME8)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP8)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM8)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME8)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+21]/10.0);
		GetDlgItem(IDC_TEMP8)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+21]/10.0);
		GetDlgItem(IDC_HUM8)->SetWindowText(str_master_id);
		 str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+21]);
		GetDlgItem(IDC_TIME8)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>8)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+21];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(str_master_id);
		}
	}
}
void CHumChamber::OnBnClickedCheck9()
{
unsigned short VLV=Get_Checks();
int ret= write_one(g_tstat_id,TEST_CHOICE_RegID.Start_ID,VLV);
if (ret<0){
 if (m_checkBtn9.GetCheck()!=0)
 {
 	m_checkBtn9.SetCheck(0);
 }
 else
 {
 	m_checkBtn9.SetCheck(1);
 }
	 AfxMessageBox(_T("Try again!"));
	return;
} 

multi_register_value[TEST_CHOICE_RegID.Start_ID]=VLV;
	if (m_checkBtn9.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP9)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP9)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM9)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM9)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME9)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME9)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP9)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM9)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME9)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+24]/10.0);
		GetDlgItem(IDC_TEMP9)->SetWindowText(str_master_id);



		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+24]/10.0);
		GetDlgItem(IDC_HUM9)->SetWindowText(str_master_id); 
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+24]);
		GetDlgItem(IDC_TIME9)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>9)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+24];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(str_master_id);
		}
	}
}
void CHumChamber::OnBnClickedCheck10()
{
unsigned short VLV=Get_Checks();
int ret= write_one(g_tstat_id,TEST_CHOICE_RegID.Start_ID,VLV);
if (ret<0)
{
 	if (m_checkBtn10.GetCheck()!=0)
 	{
 		m_checkBtn10.SetCheck(0);
 	}
 	else
 	{
 		m_checkBtn10.SetCheck(1);
 	}
 	AfxMessageBox(_T("Try again!"));
	return;
} 

multi_register_value[TEST_CHOICE_RegID.Start_ID]=VLV;
	if (m_checkBtn10.GetCheck()!=0)
	{
		GetDlgItem(IDC_TEMP10)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP10)->EnableWindow(FALSE);
		GetDlgItem(IDC_HUM10)->SetWindowText(_T("0"));
		GetDlgItem(IDC_HUM10)->EnableWindow(FALSE);
		GetDlgItem(IDC_TIME10)->SetWindowText(_T("0"));
		GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(_T("0:0:0"));
		GetDlgItem(IDC_TIME10)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_TEMP10)->EnableWindow(TRUE);
		GetDlgItem(IDC_HUM10)->EnableWindow(TRUE);
		GetDlgItem(IDC_TIME10)->EnableWindow(TRUE);
		CString str_master_id;

		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Temp.Start_ID+27]/10.0);
		GetDlgItem(IDC_TEMP10)->SetWindowText(str_master_id);
		str_master_id.Format(_T("%0.1f"),multi_register_value[First_Calibration_Points_Hum.Start_ID+27]/10.0);
		GetDlgItem(IDC_HUM10)->SetWindowText(str_master_id); 
		str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+27]);
		GetDlgItem(IDC_TIME10)->SetWindowText(str_master_id);
		if (multi_register_value[CurrentCalibrationID.Start_ID]>10)
		{
			str_master_id.Format(_T("0:0:0"));
			GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(str_master_id);
		} 
		else
		{
			Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+27];
			Hour=Time_left/60;
			Minute=Time_left%60;
			Second=0;
			str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
			GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(str_master_id);
		}
	}
}
unsigned short CHumChamber::Get_Checks(){
unsigned short Reg_Value=0;
 
if (m_checkBtn10.GetCheck()==0)
{
	Reg_Value+=1<<9;
}
if (m_checkBtn9.GetCheck()==0)
{
	Reg_Value+=1<<8;
}
if (m_checkBtn8.GetCheck()==0)
{
	Reg_Value+=1<<7;
}
if (m_checkBtn7.GetCheck()==0)
{
	Reg_Value+=1<<6;
}
if (m_checkBtn6.GetCheck()==0)
{
	Reg_Value+=1<<5;
}
if (m_checkBtn5.GetCheck()==0)
{
	Reg_Value+=1<<4;
}
if (m_checkBtn4.GetCheck()==0)
{
	Reg_Value+=1<<3;
}
if (m_checkBtn3.GetCheck()==0)
{
	Reg_Value+=1<<2;
}
if (m_checkBtn2.GetCheck()==0)
{
	Reg_Value+=1<<1;
}
if (m_checkBtn1.GetCheck()==0)
{
	Reg_Value+=1;
}
return Reg_Value;
}
void CHumChamber::OnEnSetfocusTestTemp1()
{
		float temp_float;
		int temp;
		CString str_text;
		m_test_temp1.GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		temp=(UINT)temp_float*10;
		Set(First_Test_Temp_Hum_RegID.Start_ID,temp);
		is_Show_Write_singleDLG=TRUE;
}


void CHumChamber::OnEnSetfocusTestTemp2()
{
	float temp_float;int temp;
		CString str_text;
		m_test_temp2.GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		temp=(UINT)temp_float*10;
		Set(First_Test_Temp_Hum_RegID.Start_ID+2,temp);
		is_Show_Write_singleDLG=TRUE;
}


void CHumChamber::OnEnSetfocusTestTemp3()
{
	int temp;
		float temp_float;
		CString str_text;
		m_test_temp3.GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		temp=(UINT)temp_float*10;
		Set(First_Test_Temp_Hum_RegID.Start_ID+4,temp);
		is_Show_Write_singleDLG=TRUE;
}


void CHumChamber::OnEnSetfocusTestHum1()
{
	int temp;
		float temp_float;
		CString str_text;
		m_test_hum1.GetWindowText(str_text);
		 
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		temp=(UINT)temp_float*10;
		Set(First_Test_Temp_Hum_RegID.Start_ID+1,temp);
		is_Show_Write_singleDLG=TRUE;
}


void CHumChamber::OnEnSetfocusTestHum2()
{
	int temp;
		float temp_float;
		CString str_text;
		//GetDlgItem(IDC_TEST_HUM2)->GetWindowText(str_text);
		m_test_hum2.GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		temp=(UINT)temp_float*10;
		Set(First_Test_Temp_Hum_RegID.Start_ID+3,temp);
		is_Show_Write_singleDLG=TRUE;
}


void CHumChamber::OnEnSetfocusTestHum3()
{
	    int temp;
		float temp_float;
		CString str_text;
		m_test_hum3.GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//去掉°C两个字符
		temp_float=(float)_wtof(str_text);
		temp=(UINT)temp_float*10;
		Set(First_Test_Temp_Hum_RegID.Start_ID+5,temp);
		is_Show_Write_singleDLG=TRUE;
}


void CHumChamber::OnEnSetfocusSensorId()
{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_SENSOR_ID)->GetWindowText(str_text);

	temp_float=(float)_wtof(str_text);
	m_sensorid=(UINT)temp_float;
	Set(CurrentTestSensor.Start_ID,m_sensorid);
	is_Show_Write_singleDLG=TRUE;
}
