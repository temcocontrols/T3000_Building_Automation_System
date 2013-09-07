// HumChamber.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "HumChamber.h"
#include  "ADO\ADO.h"
#include "MainFrm.h"
#include <locale.h>
DWORD WINAPI _UpdateThread(LPVOID pParam);
//DWORD WINAPI _Update_TMEPHUM_Thread(LPVOID pParam);
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
DDX_Control(pDX, IDC_RUNNING, m_Running);
DDX_Control(pDX, IDC_RAMPING, m_Ramping);
DDX_Control(pDX, IDC_TOLERANCE, m_tolerance);
DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	}

BEGIN_MESSAGE_MAP(CHumChamber, CFormView)
	ON_BN_CLICKED(IDC_REFRESH, &CHumChamber::OnBnClickedRefresh)
 
 
//ON_EN_KILLFOCUS(IDC_MASTER_ID, &CHumChamber::OnEnKillfocusMasterId)
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
ON_BN_CLICKED(IDC_START, &CHumChamber::OnBnClickedStart)
ON_BN_CLICKED(IDC_STOP, &CHumChamber::OnBnClickedStop)
ON_BN_CLICKED(IDC_CONTINUE, &CHumChamber::OnBnClickedContinue)
ON_MESSAGE(WM_MY_MESSAGE,&CHumChamber::my_message)
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
//��ʼ��23���Ĵ����ţ������ݿ��ж�����
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
}
void CHumChamber::Fresh()
{
//��ʼ�����
//��ʼ���Ĵ�����ֵ
//����ֱ��Ӧ����Щ�Ĵ�������
	CMainFrame*pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	pMain->m_pFreshMultiRegisters->SuspendThread();
//	pMain->m_pFreshTree->SuspendThread();
	pMain->m_pRefreshThread->SuspendThread();
	 if (!is_connect())
	 {
		pMain->OnConnect();
	 }
	 InitialRegisterNo();

	register_critical_section.Lock();
	Read_Multi(g_tstat_id,&multi_register_value[0],0,20);  //0-20
	g_register_occuppied=TRUE;
for(int  i=0;i<9;i++)
	{
	Read_Multi(g_tstat_id,&multi_register_value[581+i*50],581+i*50,50);
	}

   register_critical_section.Unlock();
   g_register_occuppied=FALSE;
   
   FreshGrid();
   
  // m_msflexgrid.put_Cols(4);
  // m_msflexgrid.put_Rows(SENSOR_NUM+1);
  // m_msflexgrid.put_TextMatrix(0,0,_T("Sensor"));
  // m_msflexgrid.put_TextMatrix(0,1,_T("State"));
  // m_msflexgrid.put_TextMatrix(0,2,_T("TEMP(��C)"));
  // m_msflexgrid.put_TextMatrix(0,3,_T("Hum(%)"));
  // 
  // CString temp,temp1;
  // for (int i=1;i<=SENSOR_NUM;i++)
  // {
  //   temp.Format(_T("%d"),i);
  //   m_msflexgrid.put_TextMatrix(i,0,temp);
	 //switch (multi_register_value[First_Sensor_State.Start_ID+i-1])
	 //{
	 //case 0:
	 //    {
		// temp1=_T("Not Tested");
		// break;
		// }
	 //case 1:
		// {
		// temp1=_T("Tested");
		// break;
		// }
	 //case 2:
		// {
		// temp1=_T("Bad Hum");
		// break;
		// }
	 //case 3:
		// {
		// temp1=_T("Bad Temp");
		// break;
		// }
	 //case 4:
		// {
		// temp1=_T("Bad Communications");
		// break;
		// }
	 //default:
	 //    {
		// temp1=_T("Not Tested");
		// break;
		// }
	 //}
	 //m_msflexgrid.put_TextMatrix(i,1,temp1);
  // 
	 //temp.Format(_T("%0.1f"),(short)(multi_register_value[First_Sensor_Temp_Hum_RegID.Start_ID+2*(i-1)]/10));
	 //m_msflexgrid.put_TextMatrix(i,2,temp);
	 //temp.Format(_T("%0.1f"),(short)(multi_register_value[First_Sensor_Temp_Hum_RegID.Start_ID+1+2*(i-1)]/10));
	 //m_msflexgrid.put_TextMatrix(i,3,temp);
  // } 
 
  
 
    
    ShowDialogData();
 
	Update_SensorTable();
    if (multi_register_value[TestState.Start_ID]==4||multi_register_value[TestState.Start_ID]==5)//Running....started
    {
		m_Start=TRUE;
		m_StartBtn.ShowWindow(FALSE);
		m_StopBtn.ShowWindow(TRUE);
		m_ContinueBtn.ShowWindow(FALSE);
    }
	  if (multi_register_value[TestState.Start_ID]==0)//Stop ״̬
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
//�ö���Ĵ����ķ��������мĴ�����ֵ������
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

		switch(times)
		{
		case 0:g_strScanInfo = _T("Communicating ");break;
		case 1:g_strScanInfo = _T("Communicating . ");break;
		case 2:g_strScanInfo = _T("Communicating . .");break;
		case 3:g_strScanInfo = _T("Communicating . . .");break;
		case 4:g_strScanInfo = _T("Communicating . . . .");break;
		case 5:g_strScanInfo = _T("Communicating . . . . .");break;  
		case 6:g_strScanInfo = _T("Communicating . . . . . .");break;
		case 7:g_strScanInfo = _T("Communicating . . . . . . . .");break; 
		case 8:g_strScanInfo = _T("Communicating . . . . . . . . .");break; 
		case 9:g_strScanInfo = _T("Communicating . . . . . . . . . .");break; 
		case 10:g_strScanInfo = _T("Communicating . . . . . . . . . . .");break; 
		}
		PostMessage(WM_MY_MESSAGE,1,0);
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
		if (multi_register_value[TestState.Start_ID]==0)//Stop ״̬
		{
			m_Start=FALSE;
			m_StartBtn.ShowWindow(TRUE);
			m_StopBtn.ShowWindow(FALSE);
			m_ContinueBtn.ShowWindow(TRUE);
		}

	
	read_temp=read_one(g_tstat_id,CurrentTemp.Start_ID);
	if (read_temp>0)
	{multi_register_value[CurrentTemp.Start_ID]=read_temp;
	}
		
		str_master_id.Format(_T("%0.1f��C"),multi_register_value[CurrentTemp.Start_ID]/10.0);
		GetDlgItem(IDC_CUR_TEMP)->SetWindowText(str_master_id);
		read_temp=read_one(g_tstat_id,CurrentHum.Start_ID);
		if (read_temp>0)
		{
		multi_register_value[CurrentHum.Start_ID]=read_temp;
		}
		
		str_master_id.Format(_T("%0.1f%%"), multi_register_value[CurrentHum.Start_ID]/10.0);
		GetDlgItem(IDC_CUR_HUM)->SetWindowText(str_master_id);

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
			   for(int  i=0;i<6;i++)
			   {
				   Read_Multi(g_tstat_id,&multi_register_value[660+i*50],660+i*50,50);
			   }
			   FreshGrid();

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
	m_msflexgrid.put_TextMatrix(0,2,_T("TEMP(��C)"));
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
		case 5:
			{
				temp1=_T("Testing");
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
 ////���������е����� TextArea 


	CString str_master_id;
	str_master_id.Format(_T("%d"), multi_register_value[MasterID.Start_ID]);
	GetDlgItem(IDC_MASTER_ID)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%d"),multi_register_value[SlaveID.Start_ID]); 
	GetDlgItem(IDC_SLAVE_ID)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%0.1f��C"),( short)multi_register_value[CurrentTemp.Start_ID]/10.0);
	GetDlgItem(IDC_CUR_TEMP)->SetWindowText(str_master_id);
	str_master_id.Format(_T("%0.1f��C"),( short)multi_register_value[TempTolerence.Start_ID]/10.0);
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
 
  /*m_progress.SetPos(75);*/
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


void CHumChamber::OnEnKillfocusSlaveId()
{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_SLAVE_ID)->GetWindowText(str_text);

	temp_float=(float)_wtof(str_text);
	m_slaveid=(UINT)temp_float;
	BOOL ret=WriteValueToRegID(SlaveID.Start_ID,m_slaveid);
	if (!ret)
	{
	 AfxMessageBox(_T("Write Fail"));
	}
	str_text.Format(_T("%d"),(multi_register_value[SlaveID.Start_ID])); 
	GetDlgItem(IDC_SLAVE_ID)->SetWindowText(str_text);
}


void CHumChamber::OnEnKillfocusTemp1()
{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP1)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-2,2);//ȥ����C�����ַ�
	temp_float=(float)_wtof(str_text);
	m_temp1=(UINT)temp_float*10;
	BOOL ret=WriteValueToRegID(First_Calibration_Points_Temp.Start_ID,m_temp1);
	if (!ret)
	{
		AfxMessageBox(_T("Write Fail"));
	}
	str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID]/10.0)); 
	GetDlgItem(IDC_TEMP1)->SetWindowText(str_text);
}


void CHumChamber::OnEnKillfocusTemp2()
	{
	
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_TEMP2)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-2,2);//ȥ����C�����ַ�
		temp_float=(float)_wtof(str_text);
		m_temp2=(UINT)temp_float*10;
		BOOL ret=WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+3,m_temp2);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
		str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID+3]/10.0)); 
		GetDlgItem(IDC_TEMP2)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusTemp3()
	{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP3)->GetWindowText(str_text);
	//str_text.Delete(str_text.GetLength()-2,2);//ȥ����C�����ַ�
	temp_float=(float)_wtof(str_text);
	m_temp3=(UINT)temp_float*10;
    BOOL ret=WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+6,m_temp3);
	if (!ret)
	{
		AfxMessageBox(_T("Write Fail"));
	}
	str_text.Format(_T("%0.1f"),(multi_register_value[First_Calibration_Points_Temp.Start_ID+6]/10.0)); 
	GetDlgItem(IDC_TEMP3)->SetWindowText(str_text);
	}
void CHumChamber::OnEnKillfocusTemp4()
	{
	float temp_float;
	CString str_text;
	GetDlgItem(IDC_TEMP4)->GetWindowText(str_text);
	 
	//str_text.Delete(str_text.GetLength()-2,2);//ȥ����C�����ַ�
	 
	temp_float=(float)_wtof(str_text);
	m_temp4=(UINT)temp_float*10;
   BOOL ret=WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+9,m_temp4);
   if (!ret)
   {
	   AfxMessageBox(_T("Write Fail"));
   }
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
	//	str_text.Delete(str_text.GetLength()-2,2);//ȥ����C�����ַ�
		temp_float=(float)_wtof(str_text);
		m_temp5=(UINT)(temp_float*10);
		BOOL ret=WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+12,m_temp5);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		//str_text.Delete(str_text.GetLength()-2,2);//ȥ����C�����ַ�
		temp_float=(float)_wtof(str_text);
		m_temp6=(UINT)(temp_float*10);
		BOOL ret=WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+15,m_temp6);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}

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
		//str_text.Delete(str_text.GetLength()-2,2);//ȥ����C�����ַ�
		temp_float=(float)_wtof(str_text);
		m_temp7=(UINT)(temp_float*10);
		BOOL ret=WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+18,m_temp7);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		//str_text.Delete(str_text.GetLength()-2,2);//ȥ����C�����ַ�
		temp_float=(float)_wtof(str_text);
		m_temp8=(UINT)(temp_float*10);
		BOOL ret=WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+21,m_temp8);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		//str_text.Delete(str_text.GetLength()-2,2);//ȥ����C�����ַ�
		temp_float=(float)_wtof(str_text);
		m_temp9=(UINT)(temp_float*10);
		BOOL ret=WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+24,m_temp9);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		//str_text.Delete(str_text.GetLength()-2,2);//ȥ����C�����ַ�
		temp_float=(float)_wtof(str_text);
		m_temp10=(UINT)(temp_float*10);
		BOOL  ret=WriteValueToRegID(First_Calibration_Points_Temp.Start_ID+27,m_temp10);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Hum.Start_ID,m_hum1);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+3,m_hum2);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+6,m_hum3);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+9,m_hum4);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+12,m_hum5);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+15,m_hum6);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+18,m_hum7);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+21,m_hum8);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+24,m_hum9);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Hum.Start_ID+27,m_hum10);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Time.Start_ID,m_time1);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Time.Start_ID+3,m_time2);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Time.Start_ID+6,m_time3);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Time.Start_ID+9,m_time4);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Time.Start_ID+12,m_time5);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}str_text.Format(_T("%d"),(multi_register_value[First_Calibration_Points_Time.Start_ID+12])); 
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Time.Start_ID+15,m_time6);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}str_text.Format(_T("%d"),(multi_register_value[First_Calibration_Points_Time.Start_ID+15])); 
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Time.Start_ID+18,m_time7);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Time.Start_ID+21,m_time8);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Time.Start_ID+24,m_time9);
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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
		BOOL ret =WriteValueToRegID(First_Calibration_Points_Time.Start_ID+27,m_time10);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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


void CHumChamber::OnEnKillfocusSensorId()
	{
	
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_SENSOR_ID)->GetWindowText(str_text);
		 
		temp_float=(float)_wtof(str_text);
		m_sensorid=(UINT)temp_float;
		BOOL ret =WriteValueToRegID(CurrentTestSensor.Start_ID,m_sensorid);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
		str_text.Format(_T("%d"),(multi_register_value[CurrentTestSensor.Start_ID])); 
		GetDlgItem(IDC_SENSOR_ID)->SetWindowText(str_text);
	
		Update_SensorTable();
	}


void CHumChamber::OnEnKillfocusHumTol()
	{

		float temp_float;
		CString str_text;
		GetDlgItem(IDC_HUM_TOL)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_humtolerance=(UINT)(temp_float*10);
		BOOL ret =WriteValueToRegID(HumTolenrence.Start_ID,m_humtolerance);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
		str_text.Format(_T("%0.1f%%"),(multi_register_value[HumTolenrence.Start_ID]/10.0)); 
		GetDlgItem(IDC_HUM_TOL)->SetWindowText(str_text);

	}


void CHumChamber::OnEnKillfocusTempTol()
{   float temp_float;
    CString str_text;
	GetDlgItem(IDC_TEMP_TOL)->GetWindowText(str_text);
   // AfxMessageBox(str_text);
	//str_text.Delete(str_text.GetLength()-2,2);//ȥ����C�����ַ�
	//AfxMessageBox(str_text);
	temp_float=(float)_wtof(str_text);
	m_temptolerance=(UINT)(temp_float*10);
 BOOL ret =WriteValueToRegID(TempTolerence.Start_ID,m_temptolerance);
 if (!ret)
 {
	 AfxMessageBox(_T("Write Fail"));
 }
	  str_text.Format(_T("%0.1f��C"),( short)multi_register_value[TempTolerence.Start_ID]/10.0);
	 
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
		BOOL ret =WriteValueToRegID(StartPoint.Start_ID,m_startpoint);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
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


void CHumChamber::OnEnKillfocusNumSensor()
	{
		float temp_float;
		CString str_text;
		GetDlgItem(IDC_NUM_SENSOR)->GetWindowText(str_text);
		//str_text.Delete(str_text.GetLength()-1,1);//delete % 
		temp_float=(float)_wtof(str_text);
		m_numsensors=(UINT)temp_float;
		BOOL ret=WriteValueToRegID(NumSensors.Start_ID,m_numsensors);
		
		if (!ret)
		{
			AfxMessageBox(_T("Write Fail"));
		}
		str_text.Format(_T("%d"),(multi_register_value[NumSensors.Start_ID])); 
		GetDlgItem(IDC_NUM_SENSOR)->SetWindowText(str_text);

		FreshGrid();
	}
BEGIN_EVENTSINK_MAP(CHumChamber, CFormView)
	ON_EVENT(CHumChamber, IDC_MSFLEXGRID_INPUT3, DISPID_DBLCLICK, CHumChamber::DblClickMsflexgridInput3, VTS_NONE)
//    ON_EVENT(CHumChamber, IDC_MSFLEXGRID_INPUT3, DISPID_KEYUP, CHumChamber::KeyUpMsflexgridInput3, VTS_PI2 VTS_I2)
//ON_EVENT(CHumChamber, IDC_MSFLEXGRID_INPUT3, DISPID_KEYDOWN, CHumChamber::KeyDownMsflexgridInput3, VTS_PI2 VTS_I2)
//ON_EVENT(CHumChamber, IDC_MSFLEXGRID_INPUT3, 70, CHumChamber::RowColChangeMsflexgridInput3, VTS_NONE)
END_EVENTSINK_MAP()


void CHumChamber::DblClickMsflexgridInput3()
{

long lRow,lCol;
lRow = m_msflexgrid.get_RowSel();//��ȡ������к�	
lCol = m_msflexgrid.get_ColSel(); //��ȡ������к�
if(lRow==0)
	return;
CRect rect;
m_msflexgrid.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
ScreenToClient(rect); //ת��Ϊ�ͻ�������	
// MSFlexGrid�ؼ��ĺ����ĳ��ȵ�λ��"�(twips)"��
//��Ҫ����ת��Ϊ���أ�1440�= 1Ӣ��
CDC* pDC =GetDC();
//�������ص��羵�ת������
int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
long y = m_msflexgrid.get_RowPos(lRow)/nTwipsPerDotY;
long x = m_msflexgrid.get_ColPos(lCol)/nTwipsPerDotX;
//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
long width = m_msflexgrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
long height = m_msflexgrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
//�γ�ѡ�и����ڵľ�������
CRect rc(x,y,x+width,y+height);
//ת������ԶԻ��������
rc.OffsetRect(rect.left+1,rect.top+1);
//��ȡѡ�и���ı���Ϣ	
CString strValue = m_msflexgrid.get_TextMatrix(lRow,lCol);
m_nCurRow=lRow;
m_nCurCol=lCol;

 
//write_one(g_tstat_id,CurrentTestSensor.Start_ID,m_nCurRow);
UINT temp=m_nCurRow;
CString str_text;
BOOL ret=WriteValueToRegID(CurrentTestSensor.Start_ID,temp);

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
//void CHumChamber::KeyUpMsflexgridInput3(short* KeyCode, short Shift)
//	{
//	// TODO: Add your message handler code here
//	 
//	}


//void CHumChamber::KeyDownMsflexgridInput3(short* KeyCode, short Shift)
//{
//	 if (KeyCode==)
//	 {
//	 } 
//	 else
//	 {
//	 }
//}


//void CHumChamber::RowColChangeMsflexgridInput3()
//{
//	long lRow,lCol;
//	lRow = m_msflexgrid.get_RowSel();//��ȡ������к�	
//	lCol = m_msflexgrid.get_ColSel(); //��ȡ������к�
//	if(lRow==0)
//		return;
//	CRect rect;
//	m_msflexgrid.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
//	ScreenToClient(rect); //ת��Ϊ�ͻ�������	
//	// MSFlexGrid�ؼ��ĺ����ĳ��ȵ�λ��"�(twips)"��
//	//��Ҫ����ת��Ϊ���أ�1440�= 1Ӣ��
//	CDC* pDC =GetDC();
//	//�������ص��羵�ת������
//	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
//	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
//	//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
//	long y = m_msflexgrid.get_RowPos(lRow)/nTwipsPerDotY;
//	long x = m_msflexgrid.get_ColPos(lCol)/nTwipsPerDotX;
//	//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
//	long width = m_msflexgrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
//	long height = m_msflexgrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
//	//�γ�ѡ�и����ڵľ�������
//	CRect rc(x,y,x+width,y+height);
//	//ת������ԶԻ��������
//	rc.OffsetRect(rect.left+1,rect.top+1);
//	//��ȡѡ�и���ı���Ϣ	
//	CString strValue = m_msflexgrid.get_TextMatrix(lRow,lCol);
//	m_nCurRow=lRow;
//	m_nCurCol=lCol;
//
//
//	//write_one(g_tstat_id,CurrentTestSensor.Start_ID,m_nCurRow);
//	UINT temp=m_nCurRow;
//	CString str_text;
//	WriteValueToRegID(CurrentTestSensor.Start_ID,temp);
//	str_text.Format(_T("%d"),(multi_register_value[CurrentTestSensor.Start_ID])); 
//	GetDlgItem(IDC_SENSOR_ID)->SetWindowText(str_text);
//	Update_SensorTable(); 
//}
