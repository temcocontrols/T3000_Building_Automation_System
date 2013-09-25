#pragma once
#include "..//ado//ADO.h"
#include "../MainFrm.h"
#include "afxwin.h"
#include "../global_struct.h"
#include "../msflexgrid1.h"
#include "../globle_function.h"
#include "../Weekly_Routines.h"
#include "../Annual_Routines.h"
#include "../Config_Routines.h"
#include "../global_variable_extern.h"

 
#include "afxcmn.h"
#include "../msflexgrid1.h"
#define FLEXGRID_CELL_COLOR						13421772
#define FLEXGRID_CELL_GRAY_COLOR				13421772
#define MiniPanelTimer							11

// CDialgMiniPanel form view


typedef struct MINIPANEL 
{
	int iaddress;
	CString CStName;
	CString CStvalue;
	CString CStautomannel;
	CString CStrange;
}m_minipanel;

class CDialgMiniPanel : public CFormView
{
	DECLARE_DYNCREATE(CDialgMiniPanel)

protected:
public:
	CDialgMiniPanel();           // protected constructor used by dynamic creation
	virtual ~CDialgMiniPanel();

public:
	enum { IDD = IDD_DIALOG_MINIPANEL };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void Fresh(); //进入miniPanel的入口函数
	CMsflexgrid m_msflexgridinput;

	CMsflexgrid m_msflexgridDO;
	CMsflexgrid m_msflexgridAO;
	void ShowMinipDialog();//界面显示数据
	BOOL GetDatabase();		  //从数据库中读取数据
	int m_inaddress;
	int m_inSerialNum;
	float m_flFirmware;
	int m_inHardware;
	CString m_CStrModel;
	int m_inBaudrate;
	unsigned short shmPRegister[512];
	vector<m_minipanel>vecminipanel;
	m_minipanel MINImini;
	BOOL  MiniUpdateData();//更新数据并写入数据库

	
	int comnum;

	afx_msg void OnBnClickedButtonweeklyschedule();
	 
	afx_msg void OnBnClickedButtonannualschedule();
	afx_msg void OnBnClickedButtonidschedule();
	afx_msg void OnBnClickedButtonClear();
	CString m_datetime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Updatedatabase();
	afx_msg void OnEnKillfocusEditInput();
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgridInput();
	CEdit m_editinput;
	CEdit m_editoutputdo;
	CEdit m_editoutputao;
	afx_msg void OnEnKillfocusEditoutputdo();
	afx_msg void OnEnKillfocusEditOutputao();
	void ClickMsflexgridOutputDo();
	void ClickMsflexgridOutputAo();
	void writedbinputname(int& num,CString& str );
public:
	void Initial_RegisterList();
	int  Get_RegID(CString Name);
	vector<T3Register> m_vecT3Register;
private:
	int	SN_LOW	;

	int	SN_HI	;

	int	FIRMWARE_VER_NUMBER_0	;
	int	FIRMWARE_VER_NUMBER_1	;
	int	MODBUS_ID	;
	int	PRODUCT_MODEL	;
	int	HARDWARE_VER_NUMBER	;
	int	BAUDRATE	;
	int	CALIBRATION	;
	int	PLUG_N_PLAY	;
	int	VER_ISP	;

	int	UPDATE_STATUS	;
	int	PROTOCAL_TYPE	;
	int	DIGITAL_OUTPUT1	;
	int	DIGITAL_OUTPUT2	;
	int	DIGITAL_OUTPUT3	;
	int	DIGITAL_OUTPUT4	;
	int	DIGITAL_OUTPUT5	;
	int	DIGITAL_OUTPUT6	;
	int	DIGITAL_OUTPUT7	;
	int	DIGITAL_OUTPUT8	;
	int	DIGITAL_OUTPUT9	;
	int	DIGITAL_OUTPUT10	;
	int	DIGITAL_OUTPUT11	;
	int	DIGITAL_OUTPUT12	;
	int	ANALOG_OUTPUT1	;
	int	ANALOG_OUTPUT2	;
	int	ANALOG_OUTPUT3	;
	int	ANALOG_OUTPUT4	;
	int	ANALOG_OUTPUT5	;
	int	ANALOG_OUTPUT6	;
	int	ANALOG_OUTPUT7	;
	int	ANALOG_OUTPUT8	;
	int	ANALOG_OUTPUT9	;
	int	ANALOG_OUTPUT10	;
	int	ANALOG_OUTPUT11	;
	int	ANALOG_OUTPUT12	;
	int	SWITCH_STATUS1	;
	int	SWITCH_STATUS2	;
	int	SWITCH_STATUS3	;
	int	SWITCH_STATUS4	;
	int	SWITCH_STATUS5	;
	int	SWITCH_STATUS6	;
	int	SWITCH_STATUS7	;
	int	SWITCH_STATUS8	;
	int	SWITCH_STATUS9	;
	int	SWITCH_STATUS10	;
	int	SWITCH_STATUS11	;
	int	SWITCH_STATUS12	;
	int	SWITCH_STATUS13	;
	int	SWITCH_STATUS14	;
	int	SWITCH_STATUS15	;
	int	SWITCH_STATUS16	;
	int	SWITCH_STATUS17	;
	int	SWITCH_STATUS18	;
	int	SWITCH_STATUS19	;
	int	SWITCH_STATUS20	;
	int	SWITCH_STATUS21	;
	int	SWITCH_STATUS22	;
	int	SWITCH_STATUS23	;
	int	SWITCH_STATUS24	;
	int	INPUT1	;
	int	INPUT2	;
	int	INPUT3	;
	int	INPUT4	;
	int	INPUT5	;
	int	INPUT6	;
	int	INPUT7	;
	int	INPUT8	;
	int	INPUT9	;
	int	INPUT10	;
	int	INPUT11	;
	int	INPUT12	;
	int	INPUT13	;
	int	INPUT14	;
	int	INPUT15	;
	int	INPUT16	;
	int	INPUT17	;
	int	INPUT18	;
	int	INPUT19	;
	int	INPUT20	;
	int	INPUT21	;
	int	INPUT22	;
	int	INPUT23	;
	int	INPUT24	;
	int	INPUT25	;
	int	INPUT26	;
	int	INPUT27	;
	int	INPUT28	;
	int	INPUT29	;
	int	INPUT30	;
	int	INPUT31	;
	int	INPUT32	;
	int	LOW_VOLTAGE	;
	int	INPUT_UNIT1	;
	int	INPUT_UNIT2	;
	int	INPUT_UNIT3	;
	int	INPUT_UNIT4	;
	int	INPUT_UNIT5	;
	int	INPUT_UNIT6	;
	int	INPUT_UNIT7	;
	int	INPUT_UNIT8	;
	int	INPUT_UNIT9	;
	int	INPUT_UNIT10	;
	int	INPUT_UNIT11	;
	int	INPUT_UNIT12	;
	int	INPUT_UNIT13	;
	int	INPUT_UNIT14	;
	int	INPUT_UNIT15	;
	int	INPUT_UNIT16	;
	int	INPUT_UNIT17	;
	int	INPUT_UNIT18	;
	int	INPUT_UNIT19	;
	int	INPUT_UNIT20	;
	int	INPUT_UNIT21	;
	int	INPUT_UNIT22	;
	int	INPUT_UNIT23	;
	int	INPUT_UNIT24	;
	int	INPUT_UNIT25	;
	int	INPUT_UNIT26	;
	int	INPUT_UNIT27	;
	int	INPUT_UNIT28	;
	int	INPUT_UNIT29	;
	int	INPUT_UNIT30	;
	int	INPUT_UNIT31	;
	int	INPUT_UNIT32	;
	int	LOW_VOLTAGE_UNIT	;
	int	HIGHER_BYTE_YEAR	;
	int	LOWER_BYTE_YEAR	;
	int	MONTH	;
	int	WEEK	;
	int	DAY	;
	int	HOUR	;
	int	MINUTE	;
	int	SECOND	;
	int	WEEKLY_ROUTINE_620	;
	int	ANNUAL_ROUTINE_464	;
	int	ID_SCHEDULE_762	;
	int	ANNUAL_ROUTINE_TIME_736	;
	int	WEEKLY_ROUTINE_ONTIME_1440	;
	int	WEEKLY_ROUTINE_OFFTIME_1440	;
	int	TSTAT_OCCUPIEDSTATUS_SUBTSTATS	;
	int	TSTAT_COOLINGSETPOINT_SUBTSTATS	;
	int	TSTAT_HEATINGSETPOINT_SUBTSTATS	;
	int	TSTAT_SETPOINT_SUBTSTATS	;
	int	TSTAT_TEMPERATURE_SUBTSTATS	;
	int	TSTAT_MODE_SUBTSTATS	;
	int	TSTAT_OUTPUTSTATE_SUBTSTATS	;
	int	TSTAT_NIGHT_HEATTINGDB_SUBTSTATS	;
	int	TSTAT_NIGHT_COOLINGDB_SUBTSTATS	;
	int	TSTAT_NIGHT_HEATTINGSETPOINT_SUBTSTATS	;
	int	TSTAT_NIGHT_COOLINGSETPOINT_SUBTSTATS	;
	int	TSTAT_PRODUCTMODEL_SUBTSTATS	;
	int	TSTAT_OVERRIDE_SUBTSTATS	;
	int	TSTAT_SERIALNO_SUBTSTATS	;
	int	TSTAT_OFFTIME_SUBTSTATS	;
	int	TSTAT_ONTIME_SUBTSTATS	;
	int	TEST50	;
	int	VALUEINPUT1TO34	;
	int	RANGE_INPUT1TO32	;
	int	FILTER_INPUT1TO32	;
	int	CALIBRATION_INPUT1TO32	;
	int	TOTAL_TSTAT0	;
	int	TOTAL_TSTAT1	;
	int	TOTAL_TSTAT2	;
	int	TSTAT_ADD_BOTH_CHANNEL_UART0	;
	int	TSTAT_ADD_BOTH_CHANNEL_UART1	;
	int	TSTAT_ADD_BOTH_CHANNEL_UART2	;


};


