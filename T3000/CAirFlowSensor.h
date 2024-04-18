#pragma once



// CAirFlowSensor 窗体视图

class CAirFlowSensor : public CFormView
{
	DECLARE_DYNCREATE(CAirFlowSensor)

public:
	CAirFlowSensor();           // 动态创建所使用的受保护的构造函数
	virtual ~CAirFlowSensor();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AIRFLOW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void ShowTempHumUI(bool show_window);
	virtual void OnInitialUpdate();
	void Fresh();
	void InitialUI();
	void UpdateUserInterface();
    afx_msg	LRESULT UpdateUI(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeComboAirflowMode();
	afx_msg void OnCbnSelchangeComboAirflowRange();
	afx_msg void OnEnKillfocusEditAirflowDifLength();
	afx_msg void OnEnKillfocusEditAirflowDifWidth();
	afx_msg void OnCbnSelchangeComboAirflowUnit();
	afx_msg void OnBnClickedButtonAirflowDone();
	afx_msg void OnPaint();

	int m_shape_type; //The shape of the channel, 0: square; 1: round
	CString show_duct_picture;
	afx_msg void OnBnClickedRadioCircular();
	afx_msg void OnBnClickedRadioRectangular();
	afx_msg void OnEnKillfocusEditAirflowRadius();
	afx_msg void OnBnClickedRadioUnitM();
	afx_msg void OnBnClickedRadioUnitIn();
	afx_msg void OnBnClickedRadioDefault();
	afx_msg void OnBnClickedRadioUserDefined();
	afx_msg void OnEnKillfocusEditVolatgeMin();
	afx_msg void OnEnKillfocusEditVolatgeMax();
	afx_msg void OnEnKillfocusEditCurrentMin();
	afx_msg void OnEnKillfocusEditCurrentMax();
	afx_msg void OnEnKillfocusEditPascalMin();
	afx_msg void OnEnKillfocusEditPascalMax();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeComboAirflowSensorType();
};
static DWORD WINAPI Update_Airflow_Thread(LPVOID lPvoid);

#define SENSOR_SPD31   60
#define SENSOR_SPD33   20

#define MODBUS_FIRMWARE_VERSION         4
#define MODBUS_SWITCH_OUTPUT_MODE       17
#define MODBUS_SWITCH_DP_RANGE          18
#define MODBUS_SENSOR_TYPE				29
#define MODBUS_DIFF_PRESSURE_VALUE      34
#define MODBUS_TEMPERATURE_C			37
#define MODBUS_TEMPERATURE_F			38
#define MODBUS_HUMIDITY					39
#define MODBUS_DEWPOINT					41
#define MODBUS_ENTHALPY					42
#define MODBUS_ABSOLUTE_HUMI			44
#define MODBUS_FLOW_LENGTH				57
#define MODBUS_FLOW_WIDTH				58
#define MODBUS_FLOW_UNIT				59
#define MODBUS_FLOW_SPEED				60
#define MODBUS_FLOW_HI					61
#define MODBUS_FLOW_LO					62
#define MODBUS_FLOW_RADIUS				63
#define MODBUS_DEF_CUSTOMER				64
#define MODBUS_CURRENT_MIN				65
#define MODBUS_CURRENT_MAX				66
#define MODBUS_VOLTAGE_MIN				67
#define MODBUS_VOLTAGE_MAX				68
#define MODBUS_PASCAL_MIN				69
#define MODBUS_PASCAL_MAX				70
#define MODBUS_OUTPUT_CUR				71
#define MODBUS_OUTPUT_VOL				72
#define MODBUS_FLOW_SHAPE				92
#define MODBUS_FLOW_SHAPE_UNIT          93

