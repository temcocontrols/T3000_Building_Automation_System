// HumChamber.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "HumChamber.h"
#include  "ADO\ADO.h"
DWORD WINAPI _UpdateThread(LPVOID pParam);
//UINT _UpdateSensorTableThread(LPVOID pParam);
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
#define  SENSOR_NUM 24
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

DDX_Text(pDX, IDC_TEMP1, m_temp1);
DDX_Text(pDX, IDC_HUM1, m_hum1);
DDX_Text(pDX, IDC_TIME1, m_time1);

DDX_Text(pDX, IDC_TEMP2, m_temp2);
DDX_Text(pDX, IDC_HUM2, m_hum2);
DDX_Text(pDX, IDC_TIME2, m_time2);

DDX_Text(pDX, IDC_TEMP3, m_temp3);
DDX_Text(pDX, IDC_HUM3, m_hum3);
DDX_Text(pDX, IDC_TIME3, m_time3);

DDX_Text(pDX, IDC_TEMP4, m_temp4);
DDX_Text(pDX, IDC_HUM4, m_hum4);
DDX_Text(pDX, IDC_TIME4, m_time4);

DDX_Text(pDX, IDC_TEMP5, m_temp5);
DDX_Text(pDX, IDC_HUM5, m_hum5);
DDX_Text(pDX, IDC_TIME5, m_time5);

DDX_Text(pDX, IDC_TEMP6, m_temp6);
DDX_Text(pDX, IDC_HUM6, m_hum6);
DDX_Text(pDX, IDC_TIME6, m_time6);

DDX_Text(pDX, IDC_TEMP7, m_temp7);
DDX_Text(pDX, IDC_HUM7, m_hum7);
DDX_Text(pDX, IDC_TIME7, m_time7);

DDX_Text(pDX, IDC_TEMP8, m_temp8);
DDX_Text(pDX, IDC_HUM8, m_hum8);
DDX_Text(pDX, IDC_TIME8, m_time8);

DDX_Text(pDX, IDC_TEMP9, m_temp9);
DDX_Text(pDX, IDC_HUM9, m_hum9);
DDX_Text(pDX, IDC_TIME9, m_time9);

DDX_Text(pDX, IDC_TEMP10, m_temp10);
DDX_Text(pDX, IDC_HUM10, m_hum10);
DDX_Text(pDX, IDC_TIME10, m_time10);


DDX_Control(pDX, IDC_START, m_StartBtn);
DDX_Control(pDX, IDC_STOP, m_StopBtn);
DDX_Control(pDX, IDC_CONTINUE, m_ContinueBtn);
DDX_Control(pDX, IDC_RUNNING, m_Running);
DDX_Control(pDX, IDC_RAMPING, m_Ramping);
DDX_Control(pDX, IDC_TOLERANCE, m_tolerance);
	}

BEGIN_MESSAGE_MAP(CHumChamber, CFormView)
	ON_BN_CLICKED(IDC_REFRESH, &CHumChamber::OnBnClickedRefresh)
	ON_WM_TIMER()
//	ON_EN_CHANGE(IDC_HUM_TOL, &CHumChamber::OnEnChangeHumTol)
//	ON_EN_UPDATE(IDC_MASTER_ID, &CHumChamber::OnEnUpdateMasterId)
ON_EN_KILLFOCUS(IDC_MASTER_ID, &CHumChamber::OnEnKillfocusMasterId)
ON_EN_KILLFOCUS(IDC_SLAVE_ID, &CHumChamber::OnEnKillfocusSlaveId)
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
ON_EN_KILLFOCUS(IDC_SENSOR_ID, &CHumChamber::OnEnKillfocusSensorId)
ON_EN_KILLFOCUS(IDC_HUM_TOL, &CHumChamber::OnEnKillfocusHumTol)
ON_EN_KILLFOCUS(IDC_TEMP_TOL, &CHumChamber::OnEnKillfocusTempTol)
ON_EN_KILLFOCUS(IDC_TEST_STATE, &CHumChamber::OnEnKillfocusTestState)
ON_EN_KILLFOCUS(IDC_START_POINT, &CHumChamber::OnEnKillfocusStartPoint)
ON_EN_KILLFOCUS(IDC_NUM_ERROR_SENSOR, &CHumChamber::OnEnKillfocusNumErrorSensor)
ON_EN_KILLFOCUS(IDC_NUM_SENSOR, &CHumChamber::OnEnKillfocusNumSensor)
//ON_BN_CLICKED(IDC_RUNNING, &CHumChamber::OnBnClickedRunning)
ON_BN_CLICKED(IDC_START, &CHumChamber::OnBnClickedStart)
//ON_BN_CLICKED(IDC_RAMPING, &CHumChamber::OnBnClickedRamping)
ON_BN_CLICKED(IDC_STOP, &CHumChamber::OnBnClickedStop)
ON_BN_CLICKED(IDC_CONTINUE, &CHumChamber::OnBnClickedContinue)
END_MESSAGE_MAP()

void CHumChamber::OnInitialUpdate()
{
 CFormView::OnInitialUpdate();
 m_Start=FALSE;
  hFirstThread = CreateThread(NULL,NULL,_UpdateThread,this,NULL,0);
}
// CHumChamber diagnostics
void CHumChamber::Fresh()
{
   m_msflexgrid.put_Cols(4);
   m_msflexgrid.put_Rows(SENSOR_NUM+1);
   m_msflexgrid.put_TextMatrix(0,0,_T("Sensor"));
   m_msflexgrid.put_TextMatrix(0,1,_T("State"));
   m_msflexgrid.put_TextMatrix(0,2,_T("TEMP"));
   m_msflexgrid.put_TextMatrix(0,3,_T("Hum"));
   GetRegInfoFromDB(First_Sensor_State,_T("First_Sensor_State_RegID"));
   CString temp,temp1;
   for (int i=1;i<=SENSOR_NUM;i++)
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
		 temp1=_T("Tested");
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
	 default:
	     {
		 temp1=_T("Not Tested");
		 break;
		 }
	 }
	 m_msflexgrid.put_TextMatrix(i,1,temp1);
   } 
  // multi_register_value[626]=1;//test use
   GetRegInfoFromDB(CurrentTestSensor,_T("CurrentTestSensor_RegID"));
   GetRegInfoFromDB(CurrentTestSensorTemp,_T("CurrentTestSensorTemp_RegID"));
   temp.Format(_T("%d"),multi_register_value[CurrentTestSensorTemp.Start_ID]);
   m_msflexgrid.put_TextMatrix(multi_register_value[CurrentTestSensor.Start_ID],2,temp);
   GetRegInfoFromDB(CurrentTestSensorHum,_T("CurrentTestSensorHum_RegID"));
   temp.Format(_T("%d"),multi_register_value[CurrentTestSensorHum.Start_ID]);
   m_msflexgrid.put_TextMatrix(multi_register_value[CurrentTestSensor.Start_ID],3,temp);
   GetRegInfoFromDB(CurrentCalibrationTimeLeft,_T("CurrentCalibrationTimeLeft_RegID"));
   GetRegInfoFromDB(CurrentCalibrationID,_T("CurrentCalibrationPointID_RegID"));
 
    ShowDialogData();
	Update_SensorTable();
    
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

void CHumChamber::UPdate_Time()
	{
	CString str;
	while(TRUE)
	{
	 
	Sleep(1000);
	if (m_Start)
	{
	int Time_left=read_one(g_tstat_id,CurrentCalibrationTimeLeft.Start_ID);
	if (Time_left>0)
		{
		int Hour=Time_left/3600;
		int Minute=(Time_left%3600)/60;
		int Second=(Time_left%3600)%60;

		str.Format(_T("%d:%d:%d"),Hour,Minute,Second);
		switch(read_one(g_tstat_id,CurrentCalibrationID.Start_ID))
			{
		case 1:
			{
			GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(str);
			break;
			}
		case 2:
			{
			GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(str);
			break;
			}
		case 3:
			{
			GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(str);
			break;
			}
		case 4:
			{
			GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(str);
			break;
			}
		case 5:
			{
			GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(str);
			break;
			}
		case 6:
			{
			GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(str);
			break;
			}
		case 7:
			{
			GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(str);
			break;
			}
		case 8:
			{
			GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(str);

			break;
			}
		case 9:
			{
			GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(str);
			break;
			}
		case 10:
			{
			GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(str);
			break;
			}

			}
		}

	
	
	CString temp,temp1;
	for (int i=1;i<=SENSOR_NUM;i++)
		{
		temp.Format(_T("%d"),i);
		m_msflexgrid.put_TextMatrix(i,0,temp);
		switch (read_one(g_tstat_id,First_Sensor_State.Start_ID+i-1))
			{
		case 0:
			{
			temp1=_T("Not Tested");
			break;
			}
		case 1:
			{
			temp1=_T("Tested");
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
		default:
			{
			temp1=_T("Not Tested");
			break;
			}
			}
		m_msflexgrid.put_TextMatrix(i,1,temp1);
		} 
	//更新表格Col=3-4的
	temp.Format(_T("%d"),read_one(g_tstat_id,CurrentTestSensorTemp.Start_ID));
	m_msflexgrid.put_TextMatrix(read_one(g_tstat_id,CurrentTestSensor.Start_ID),2,temp);
	temp.Format(_T("%d"),read_one(g_tstat_id,CurrentTestSensorHum.Start_ID));
	m_msflexgrid.put_TextMatrix(read_one(g_tstat_id,CurrentTestSensor.Start_ID),3,temp);

	/*

	switch(read_one(g_tstat_id,TestState.Start_ID))
		{
	case 4:
		{

		break;
		}
	case 5:
		{

		break;
		}
	case 6:
		{

		break;
		}
	default:
		{

		}
		}
		*/
	}
	
    //更新温度和湿度的值

	int RegTemp=read_one(g_tstat_id,CurrentTemp.Start_ID);
	if (RegTemp>0)
	{
	str.Format(_T("%d"),RegTemp);
	GetDlgItem(IDC_CUR_TEMP)->SetWindowText(str);
	}

	RegTemp=read_one(g_tstat_id,CurrentHum.Start_ID);
	if (RegTemp>0)
	{
	str.Format(_T("%d"),RegTemp);
	GetDlgItem(IDC_CUR_HUM)->SetWindowText(str);
	}

	RegTemp=read_one(g_tstat_id,NumBadSensors.Start_ID);
	if (RegTemp>0)
	{
	str.Format(_T("%d"),RegTemp);
	GetDlgItem(IDC_NUM_ERROR_SENSOR)->SetWindowText(str);
	}




	
	
	}

}
void CHumChamber::Update_SensorTable()
{
 
	CString str_master_id;
	str_master_id.Format(_T("%d"),read_one(g_tstat_id,First_Sensor_Fr.Start_ID));
	GetDlgItem(IDC_FREQ1)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),read_one(g_tstat_id,First_Sensor_Fr.Start_ID+2));
	GetDlgItem(IDC_FREQ2)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),read_one(g_tstat_id,First_Sensor_Fr.Start_ID+4));
	GetDlgItem(IDC_FREQ3)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),read_one(g_tstat_id,First_Sensor_Fr.Start_ID+6));
	GetDlgItem(IDC_FREQ4)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),read_one(g_tstat_id,First_Sensor_Fr.Start_ID+8));
	GetDlgItem(IDC_FREQ5)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),read_one(g_tstat_id,First_Sensor_Fr.Start_ID+10));
	GetDlgItem(IDC_FREQ6)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),read_one(g_tstat_id,First_Sensor_Fr.Start_ID+12));
	GetDlgItem(IDC_FREQ7)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),read_one(g_tstat_id,First_Sensor_Fr.Start_ID+14));
	GetDlgItem(IDC_FREQ8)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),read_one(g_tstat_id,g_tstat_id,First_Sensor_Fr.Start_ID+16));
	GetDlgItem(IDC_FREQ9)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),read_one(g_tstat_id,g_tstat_id,First_Sensor_Fr.Start_ID+18));
	GetDlgItem(IDC_FREQ10)->SetWindowText(str_master_id);
	 
 
	 
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID]);
	GetDlgItem(IDC_RH1)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+2]);
	GetDlgItem(IDC_RH2)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+4]);
	GetDlgItem(IDC_RH3)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+6]);
	GetDlgItem(IDC_RH4)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+8]);
	GetDlgItem(IDC_RH5)->SetWindowText(str_master_id); 
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+10]);
	GetDlgItem(IDC_RH6)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+12]);
	GetDlgItem(IDC_RH7)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+14]);
	GetDlgItem(IDC_RH8)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+16]);
	GetDlgItem(IDC_RH9)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+18]);
	GetDlgItem(IDC_RH10)->SetWindowText(str_master_id);
 
}
void CHumChamber::Update_SensorReadingTable()
{
 

}
// CHumChamber message handlers
void CHumChamber::OnBnClickedRefresh()
	{
	  Fresh();
	}

DWORD WINAPI _UpdateThread(LPVOID pParam)
	{
	CHumChamber* pDlg = (CHumChamber*)(pParam);
	 
	 pDlg->UPdate_Time();
	return 1;
	}
void CHumChamber::OnTimer(UINT_PTR nIDEvent)
	{
	CFormView::OnTimer(nIDEvent);
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
if (GetRegInfoFromDB(MasterID,_T("MasterID_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"), multi_register_value[MasterID.Start_ID]);
	 
	GetDlgItem(IDC_MASTER_ID)->SetWindowText(str_master_id);
	} 
if (GetRegInfoFromDB(SlaveID,_T("SlaveID_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[SlaveID.Start_ID]);
	 
	GetDlgItem(IDC_SLAVE_ID)->SetWindowText(str_master_id);
	} 
if (GetRegInfoFromDB(CurrentTemp,_T("CurrentTemp_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[CurrentTemp.Start_ID]);

	GetDlgItem(IDC_CUR_TEMP)->SetWindowText(str_master_id);
	}
if (GetRegInfoFromDB(TempTolerence,_T("TempTolerence_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[TempTolerence.Start_ID]);
	 
	GetDlgItem(IDC_TEMP_TOL)->SetWindowText(str_master_id);
	} 
if (GetRegInfoFromDB(NumSensors,_T("NumSensors_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[NumSensors.Start_ID]);
 
	GetDlgItem(IDC_NUM_SENSOR)->SetWindowText(str_master_id);
	} 
if (GetRegInfoFromDB(StartPoint,_T("StartPoint_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[StartPoint.Start_ID]);
	 
	GetDlgItem(IDC_START_POINT)->SetWindowText(str_master_id);
	} 
if (GetRegInfoFromDB(CurrentHum,_T("CurrentHum_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[CurrentHum.Start_ID]);
	 
	GetDlgItem(IDC_CUR_HUM)->SetWindowText(str_master_id);
	} 
if (GetRegInfoFromDB(HumTolenrence,_T("HumTolrence_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[HumTolenrence.Start_ID]);
	 
	GetDlgItem(IDC_HUM_TOL)->SetWindowText(str_master_id);
	} 
if (GetRegInfoFromDB(NumBadSensors,_T("NumBadSensors_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[NumBadSensors.Start_ID]);

	GetDlgItem(IDC_NUM_ERROR_SENSOR)->SetWindowText(str_master_id);
	} 
if (GetRegInfoFromDB(TestState,_T("TestState_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[TestState.Start_ID]);
	GetDlgItem(IDC_TEST_STATE)->SetWindowText(str_master_id);
	} 
if (GetRegInfoFromDB(SensorTemp,_T("SensorTemp_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[SensorTemp.Start_ID]);
	GetDlgItem(IDC_SENSOR_TEMP)->SetWindowText(str_master_id);
	} 
//这个是有规律的 寄存器 Set Calibration Points
if (GetRegInfoFromDB(First_Calibration_Points_Temp,_T("First_Calibration_Points_Temp_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Temp.Start_ID]);
	GetDlgItem(IDC_TEMP1)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Temp.Start_ID+3]);
	GetDlgItem(IDC_TEMP2)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Temp.Start_ID+6]);
	GetDlgItem(IDC_TEMP3)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Temp.Start_ID+9]);
	GetDlgItem(IDC_TEMP4)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Temp.Start_ID+12]);
	GetDlgItem(IDC_TEMP5)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Temp.Start_ID+15]);
	GetDlgItem(IDC_TEMP6)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Temp.Start_ID+18]);
	GetDlgItem(IDC_TEMP7)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Temp.Start_ID+21]);
	GetDlgItem(IDC_TEMP8)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Temp.Start_ID+24]);
	GetDlgItem(IDC_TEMP9)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Temp.Start_ID+27]);
	GetDlgItem(IDC_TEMP10)->SetWindowText(str_master_id);
	} 
if (GetRegInfoFromDB(First_Calibration_Points_Hum,_T("First_Calibration_Points_Hum_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Hum.Start_ID]);
	GetDlgItem(IDC_HUM1)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Hum.Start_ID+3]);
	GetDlgItem(IDC_HUM2)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Hum.Start_ID+6]);
	GetDlgItem(IDC_HUM3)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Hum.Start_ID+9]);
	GetDlgItem(IDC_HUM4)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Hum.Start_ID+12]);
	GetDlgItem(IDC_HUM5)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Hum.Start_ID+15]);
	GetDlgItem(IDC_HUM6)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Hum.Start_ID+18]);
	GetDlgItem(IDC_HUM7)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Hum.Start_ID+21]);
	GetDlgItem(IDC_HUM8)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Hum.Start_ID+24]);
	GetDlgItem(IDC_HUM9)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Hum.Start_ID+27]);
	GetDlgItem(IDC_HUM10)->SetWindowText(str_master_id);
	}
if (GetRegInfoFromDB(First_Calibration_Points_Time,_T("First_Calibration_Points_Time_RegID")))
	{
	CString str_master_id; 

	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID]);
	GetDlgItem(IDC_TIME1)->SetWindowText(str_master_id);
	Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID];
	Hour=Time_left/60;
	Minute=Time_left%60;
	Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(str_master_id);

	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+3]);
	GetDlgItem(IDC_TIME2)->SetWindowText(str_master_id);
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+3];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT2)->SetWindowText(str_master_id);

	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+6]);
	GetDlgItem(IDC_TIME3)->SetWindowText(str_master_id);
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+6];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT3)->SetWindowText(str_master_id);

	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+9]);
	GetDlgItem(IDC_TIME4)->SetWindowText(str_master_id);
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+9];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT4)->SetWindowText(str_master_id);

	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+12]);
	GetDlgItem(IDC_TIME5)->SetWindowText(str_master_id);
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+12];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT5)->SetWindowText(str_master_id);

	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+15]);
	GetDlgItem(IDC_TIME6)->SetWindowText(str_master_id);
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+15];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT6)->SetWindowText(str_master_id);

	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+18]);
	GetDlgItem(IDC_TIME7)->SetWindowText(str_master_id);
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+18];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT7)->SetWindowText(str_master_id);

	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+21]);
	GetDlgItem(IDC_TIME8)->SetWindowText(str_master_id);
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+21];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT8)->SetWindowText(str_master_id);

	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+24]);
	GetDlgItem(IDC_TIME9)->SetWindowText(str_master_id);
	  Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+24];
	  Hour=Time_left/60;
	  Minute=Time_left%60;
	  Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT9)->SetWindowText(str_master_id);

	str_master_id.Format(_T("%d"),multi_register_value[First_Calibration_Points_Time.Start_ID+27]);
	GetDlgItem(IDC_TIME10)->SetWindowText(str_master_id);

	 Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID+27];
	 Hour=Time_left/60;
	 Minute=Time_left%60;
	 Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT10)->SetWindowText(str_master_id);
	}
// Sensor Table
if (GetRegInfoFromDB(First_Sensor_Fr,_T("First_Sensor_Fr_RegID")))
	{
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_Fr.Start_ID]);
	GetDlgItem(IDC_FREQ1)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_Fr.Start_ID+2]);
	GetDlgItem(IDC_FREQ2)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_Fr.Start_ID+4]);
	GetDlgItem(IDC_FREQ3)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_Fr.Start_ID+6]);
	GetDlgItem(IDC_FREQ4)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_Fr.Start_ID+8]);
	GetDlgItem(IDC_FREQ5)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_Fr.Start_ID+10]);
	GetDlgItem(IDC_FREQ6)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_Fr.Start_ID+12]);
	GetDlgItem(IDC_FREQ7)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_Fr.Start_ID+14]);
	GetDlgItem(IDC_FREQ8)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[g_tstat_id,First_Sensor_Fr.Start_ID+16]);
	GetDlgItem(IDC_FREQ9)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[g_tstat_id,First_Sensor_Fr.Start_ID+18]);
	GetDlgItem(IDC_FREQ10)->SetWindowText(str_master_id);
	}
 if (GetRegInfoFromDB(First_Sensor_RH,_T("First_Sensor_RH_RegID")))
 {
	CString str_master_id;
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID]);
	GetDlgItem(IDC_RH1)->SetWindowText(str_master_id);
	 str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+2]);
	GetDlgItem(IDC_RH2)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+4]);
	GetDlgItem(IDC_RH3)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+6]);
	GetDlgItem(IDC_RH4)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+8]);
	GetDlgItem(IDC_RH5)->SetWindowText(str_master_id); 
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+10]);
	GetDlgItem(IDC_RH6)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+12]);
	GetDlgItem(IDC_RH7)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+14]);
	GetDlgItem(IDC_RH8)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+16]);
	GetDlgItem(IDC_RH9)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[First_Sensor_RH.Start_ID+18]);
	GetDlgItem(IDC_RH10)->SetWindowText(str_master_id);
 }
  
}
 


 
BOOL CHumChamber::WriteValueToRegID(UINT RegID,UINT &Value)
	{
	int IntTemp=multi_register_value[RegID];
	if (IntTemp!=Value)
		{
		write_one(g_tstat_id,RegID,Value);
		if (read_one(g_tstat_id,RegID)!=Value)
			{
			//AfxMessageBox(_T("Try again"));
			Value=IntTemp;
			return FALSE;
			} 
		else
			{
			multi_register_value[RegID]=Value;
			return TRUE;
			}
		}
		return TRUE;
	}

void CHumChamber::OnEnKillfocusMasterId()
{
 
   UpdateData(TRUE);
  if (!WriteValueToRegID(MasterID.Start_ID,m_masterid))
  {
     AfxMessageBox(_T("Try again"));
  } 
  UpdateData(FALSE);
}


//
void CHumChamber::OnEnKillfocusSlaveId()
{
UpdateData(TRUE);
if (!WriteValueToRegID(SlaveID.Start_ID,m_slaveid))
	{
	AfxMessageBox(_T("Try again"));
	} 
UpdateData(FALSE);
}


void CHumChamber::OnEnKillfocusTemp1()
{
UpdateData(TRUE);
if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID,m_temp1))
	{
	AfxMessageBox(_T("Try again"));
	} 
UpdateData(FALSE);
}


void CHumChamber::OnEnKillfocusTemp2()
	{
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+3,m_temp2))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusTemp3()
	{
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+6,m_temp3))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusTemp4()
	{
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+9,m_temp4))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusTemp5()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+12,m_temp5))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	 
	}

void CHumChamber::OnEnKillfocusTemp6()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+15,m_temp6))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	 
	}


void CHumChamber::OnEnKillfocusTemp7()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+18,m_temp7))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	 
	}
void CHumChamber::OnEnKillfocusTemp8()
	{
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+21,m_temp8))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusTemp9()
	{
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+24,m_temp9))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	 
	}
void CHumChamber::OnEnKillfocusTemp10()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+27,m_temp10))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	 
	 
	}

void CHumChamber::OnEnKillfocusHum1()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID,m_hum1))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusHum2()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+3,m_hum2))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusHum3()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+6,m_hum3))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusHum4()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+9,m_hum4))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusHum5()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+12,m_hum5))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusHum6()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+15,m_hum6))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusHum7()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+18,m_hum7))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusHum8()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+21,m_hum8))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusHum9()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+24,m_hum9))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
void CHumChamber::OnEnKillfocusHum10()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+27,m_hum10))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}


 
void CHumChamber::OnEnKillfocusTime1()
	{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID,m_time1))
		{
		AfxMessageBox(_T("Try again"));
		} 

	CString str_master_id;
	Time_left=multi_register_value[First_Calibration_Points_Time.Start_ID];
	Hour=Time_left/60;
	Minute=Time_left%60;
	Second=0;
	str_master_id.Format(_T("%d:%d:%d"),Hour,Minute,Second);
	GetDlgItem(IDC_TEMP_LEFT1)->SetWindowText(str_master_id);
	UpdateData(FALSE);

	}
void CHumChamber::OnEnKillfocusTime2()
	{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+3,m_time2))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
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
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+6,m_time3))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
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
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+9,m_time4))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
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
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+12,m_time5))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);

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
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+15,m_time6))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);

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
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+18,m_time7))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);

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
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+21,m_time8))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);

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
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+24,m_time9))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);

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
	UpdateData(TRUE);
	if (!WriteValueToRegID(First_Calibration_Points_Time.Start_ID+27,m_time10))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);

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
	UpdateData(TRUE);
	if (!WriteValueToRegID(SensorTemp.Start_ID,m_sensorTemp))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}


void CHumChamber::OnEnKillfocusSensorId()
	{
	UpdateData(TRUE);
	if (!WriteValueToRegID(CurrentTestSensor.Start_ID,m_sensorid))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	Update_SensorTable();
	}


void CHumChamber::OnEnKillfocusHumTol()
	{
	UpdateData(TRUE);
	if (!WriteValueToRegID(HumTolenrence.Start_ID,m_humtolerance))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}


void CHumChamber::OnEnKillfocusTempTol()
{
UpdateData(TRUE);
if (!WriteValueToRegID(TempTolerence.Start_ID,m_temptolerance))
	{
	AfxMessageBox(_T("Try again"));
	} 
UpdateData(FALSE);
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
	UpdateData(TRUE);
	if (!WriteValueToRegID(StartPoint.Start_ID,m_startpoint))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
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


void CHumChamber::OnEnKillfocusNumSensor()
	{
	UpdateData(TRUE);
	if (!WriteValueToRegID(NumSensors.Start_ID,m_numsensors))
		{
		AfxMessageBox(_T("Try again"));
		} 
	UpdateData(FALSE);
	}
BEGIN_EVENTSINK_MAP(CHumChamber, CFormView)
	ON_EVENT(CHumChamber, IDC_MSFLEXGRID_INPUT3, DISPID_DBLCLICK, CHumChamber::DblClickMsflexgridInput3, VTS_NONE)
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

 
write_one(g_tstat_id,CurrentTestSensor.Start_ID,m_nCurRow);
 
Update_SensorTable();
}


//void CHumChamber::OnBnClickedRunning()
//	{
//	// TODO: Add your control notification handler code here
//	}


void CHumChamber::OnBnClickedStart()
	{
		int ret=write_one(g_tstat_id,TestState.Start_ID,2);
	     m_Start=TRUE;
	}


 


void CHumChamber::OnBnClickedStop()
	{
	// TODO: Add your control notification handler code here
	write_one(g_tstat_id,TestState.Start_ID,0);

	m_Start=FALSE;
	}


void CHumChamber::OnBnClickedContinue()
	{
	 write_one(g_tstat_id,TestState.Start_ID,1);

	 m_Start=TRUE;
	}
