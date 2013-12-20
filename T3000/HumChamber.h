#pragma once


#include "msflexgrid1.h"
#include "globle_function.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "WriteSingleRegDlg.h"

#define  WM_MY_MESSAGE  WM_USER + 101

// CHumChamber form view
struct  Register_info 
{
  int  Start_ID;
  int  Length;
  CString Destription;
};
class Register
	{
	public:
		int Read_Single_Value();
		BOOL Write_Single_Value(int Value);
		void Set_Reg(Register_info reginfo);
	private:
		Register_info reg_data;
	};
class CHumChamber : public CFormView
{
	DECLARE_DYNCREATE(CHumChamber)

public:
	CHumChamber();           // protected constructor used by dynamic creation
	virtual ~CHumChamber();

public:
	enum { IDD = IDD_HUM_CHAMBER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
    BOOL m_ReadOnly;
	void Fresh();
	void FreshGrid();
    void Fresh_Hum_Temp();
	void InitialRegisterNo();
	void ShowDialogData();
	BOOL GetRegInfoFromDB(Register_info &reg_data,CString Area_Name);
	void Update_AllData();
	void Update_SensorTable();
	void Show_AllData();
	BOOL WriteValueToRegID(UINT RegID,UINT &Value); 
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedRefresh();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CMsflexgrid m_msflexgrid;
	CString g_strScanInfo;
	void Set(int id,int value);
 
    int m_ID;
	int m_Value;
private:
	Register_info MasterID;
	Register_info SlaveID;
	Register_info CurrentTemp;
	Register_info TempTolerence;
	Register_info NumSensors;
	Register_info StartPoint;
	Register_info CurrentHum;
	Register_info HumTolenrence;
	Register_info NumBadSensors;
	Register_info TestState;
	Register_info SensorTemp;
	Register_info First_Calibration_Points_Temp;
	Register_info First_Calibration_Points_Hum;
	Register_info First_Calibration_Points_Time;
	Register_info First_Sensor_Fr;
	Register_info First_Sensor_RH;
	Register_info First_Sensor_State;
	Register_info CurrentTestSensor;
	Register_info CurrentTestSensorTemp;
	Register_info CurrentTestSensorHum;
	Register_info CurrentCalibrationTimeLeft;
	Register_info CurrentCalibrationID;
	Register_info First_Sensor_Temp_Hum_RegID;
public:
//	afx_msg void OnEnUpdateMasterId();
//	afx_msg void OnEnKillfocusMasterId();
private:
	UINT m_masterid;
	UINT m_slaveid;
	UINT m_temptolerance;
	UINT m_humtolerance;
	UINT m_startpoint;
	UINT m_numsensors;
	UINT m_numbadsensors;
	UINT m_teststate;
	UINT m_sensorTemp;
private:
	UINT m_sensorid;

	UINT m_temp1;
	UINT m_hum1;
	UINT m_time1;

	UINT m_temp2;
	UINT m_hum2;
	UINT m_time2;

	UINT m_temp3;
	UINT m_hum3;
	UINT m_time3;

	UINT m_temp4;
	UINT m_hum4;
	UINT m_time4;

	UINT m_temp5;
	UINT m_hum5;
	UINT m_time5;

	UINT m_temp6;
	UINT m_hum6;
	UINT m_time6;

	UINT m_temp7;
	UINT m_hum7;
	UINT m_time7;

	UINT m_temp8;
	UINT m_hum8;
	UINT m_time8;

	UINT m_temp9;
	UINT m_hum9;
	UINT m_time9;

	UINT m_temp10;
	UINT m_hum10;
	UINT m_time10;

	int m_nCurRow;
	int m_nCurCol;

	int Time_left;
	int Hour;
	int Second;
	int Minute;
	
	HANDLE hFirstThread;
	CString m_recordhumchamber;
	
	//CWinThread* hFirstThread;
	//HANDLE hSecondThread;
	public:
		afx_msg void OnEnKillfocusSlaveId();
		afx_msg void OnEnKillfocusTemp1();
		afx_msg void OnEnKillfocusTemp2();
		afx_msg void OnEnKillfocusTemp3();
		afx_msg void OnEnKillfocusTemp4();
		afx_msg void OnEnKillfocusTemp5();
		afx_msg void OnEnKillfocusTemp6();
		afx_msg void OnEnKillfocusTemp7();
		afx_msg void OnEnKillfocusTemp8();
		afx_msg void OnEnKillfocusTemp9();
		afx_msg void OnEnKillfocusTemp10();
		afx_msg void OnEnKillfocusHum1();
		afx_msg void OnEnKillfocusHum2();
		afx_msg void OnEnKillfocusHum3();
		afx_msg void OnEnKillfocusHum4();
		afx_msg void OnEnKillfocusHum5();
		afx_msg void OnEnKillfocusHum6();
		afx_msg void OnEnKillfocusHum7();
		afx_msg void OnEnKillfocusHum8();
		afx_msg void OnEnKillfocusHum9();
		afx_msg void OnEnKillfocusHum10();

		afx_msg void OnEnKillfocusTime1();
		afx_msg void OnEnKillfocusTime2();
		afx_msg void OnEnKillfocusTime3();
		afx_msg void OnEnKillfocusTime4();
		afx_msg void OnEnKillfocusTime5();
		afx_msg void OnEnKillfocusTime6();
		afx_msg void OnEnKillfocusTime7();
		afx_msg void OnEnKillfocusTime8();
		afx_msg void OnEnKillfocusTime9();
		afx_msg void OnEnKillfocusTime10();
		afx_msg void OnEnKillfocusSensorTemp();
		afx_msg void OnEnKillfocusSensorId();
		afx_msg void OnEnKillfocusHumTol();
		afx_msg void OnEnKillfocusTempTol();
		afx_msg void OnEnKillfocusTestState();
		afx_msg void OnEnKillfocusStartPoint();
		afx_msg void OnEnKillfocusNumErrorSensor();
		afx_msg void OnEnKillfocusNumSensor();

		afx_msg LRESULT my_message(WPARAM wParam,LPARAM lParam);
		DECLARE_EVENTSINK_MAP()
		void DblClickMsflexgridInput3();
		CButton m_StartBtn;
//		afx_msg void OnBnClickedRunning();
		afx_msg void OnBnClickedStart();
		
//		afx_msg void OnBnClickedRamping();
		afx_msg void OnBnClickedStop();
		afx_msg void OnBnClickedContinue();
		CButton m_StopBtn;
		CButton m_ContinueBtn;
		CStatic m_Running;
		CStatic m_Ramping;
		CStatic m_tolerance;

		BOOL m_Start;
//		void MouseMoveMsflexgridInput3(short Button, short Shift, long x, long y);
//		void KeyDownMsflexgridInput3(short* KeyCode, short Shift);
//		void KeyUpMsflexgridInput3(short* KeyCode, short Shift);
//		void KeyPressMsflexgridInput3(short* KeyAscii);
//		void KeyDownMsflexgridInput3(short* KeyCode, short Shift);
//		void KeyUpMsflexgridInput3(short* KeyCode, short Shift);
		// //process
		CProgressCtrl m_progress;
//		void KeyDownMsflexgridInput3(short* KeyCode, short Shift);
//		void RowColChangeMsflexgridInput3();
		void ShowChangingData();
		void ClickMsflexgridInput3();
		afx_msg void OnEnSetfocusNumSensor();
	 
		
		afx_msg void OnEnSetfocusSlaveId();
		afx_msg void OnEnSetfocusStartPoint();
		afx_msg void OnEnSetfocusTempTol();
		afx_msg void OnEnSetfocusHumTol();

		afx_msg void OnEnSetfocusTemp1();
		afx_msg void OnEnSetfocusTemp2();
		afx_msg void OnEnSetfocusTemp3();
		afx_msg void OnEnSetfocusTemp4();
		afx_msg void OnEnSetfocusTemp5();
		afx_msg void OnEnSetfocusTemp6();
		afx_msg void OnEnSetfocusTemp7();
		afx_msg void OnEnSetfocusTemp8();
		afx_msg void OnEnSetfocusTemp9();
		afx_msg void OnEnSetfocusTemp10();

		afx_msg void OnEnSetfocusHum1();
		afx_msg void OnEnSetfocusHum2();
		afx_msg void OnEnSetfocusHum3();
		afx_msg void OnEnSetfocusHum4();
		afx_msg void OnEnSetfocusHum5();
		afx_msg void OnEnSetfocusHum6();
		afx_msg void OnEnSetfocusHum7();
		afx_msg void OnEnSetfocusHum8();
		afx_msg void OnEnSetfocusHum9();
		afx_msg void OnEnSetfocusHum10();

		afx_msg void OnEnSetfocusTime1();
		afx_msg void OnEnSetfocusTime2();
		afx_msg void OnEnSetfocusTime3();
		afx_msg void OnEnSetfocusTime4();
		afx_msg void OnEnSetfocusTime5();
		afx_msg void OnEnSetfocusTime6();
		afx_msg void OnEnSetfocusTime7();
		afx_msg void OnEnSetfocusTime8();
		afx_msg void OnEnSetfocusTime9();
		afx_msg void OnEnSetfocusTime10();
};


