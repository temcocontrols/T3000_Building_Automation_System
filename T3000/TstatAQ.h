#pragma once
#include "afxwin.h"

#include "CM5\CStatic\staticex.h"
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"
// CTstatAQ ������ͼ - CTstatAQ form view

class CTstatAQ : public CFormView
{
	DECLARE_DYNCREATE(CTstatAQ)

protected:
	
	

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TSTAT_AQ };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧�� - DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    CTstatAQ();           // ��̬������ʹ�õ��ܱ����Ĺ��캯�� - Protected constructor used for dynamic creation
    virtual ~CTstatAQ();
    void Fresh();
    void UpdateUI();
    void UpdatePM25UI();
    void EnableCus(bool flag);
    void PM2_5_Window(int nflag);
    virtual void OnInitialUpdate();
    afx_msg LRESULT UpdateUI(WPARAM wParam, LPARAM lParam);
    afx_msg void OnEnKillfocusEditCo2OnTime();
    afx_msg void OnEnKillfocusEditCo2OffTime();
    afx_msg void OnEnKillfocusEditPmOnTime();
    afx_msg void OnEnKillfocusEditPmOffTime();
    afx_msg void OnBnClickedRadioDegC();
    afx_msg void OnBnClickedRadioDegF();
    afx_msg void OnCbnSelchangeComboAqiRegion();
    afx_msg void OnEnKillfocusEditLevel1();
    afx_msg void OnEnKillfocusEditLevel2();
    afx_msg void OnEnKillfocusEditLevel3();
    afx_msg void OnEnKillfocusEditLevel4();
    afx_msg void OnEnKillfocusEditLevel5();
    afx_msg void OnBnClickedButtonCusAqi();
    CStaticEx m_aqi_title;
    CStaticEx m_static_info;
    afx_msg void OnBnClickedButtonAutoCal();

    ListCtrlEx::CListCtrlEx m_airlab_list;
    void Initial_List();
    afx_msg LRESULT Fresh_Airlab_Item(WPARAM wParam, LPARAM lParam);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg LRESULT  AirlabMessageCallBack(WPARAM wParam, LPARAM lParam);
    CDateTimeCtrl m_time_trigger;
    afx_msg void OnNMClickListAirlab(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMKillfocusDatetimepickerTimeRemain(NMHDR* pNMHDR, LRESULT* pResult);
    int m_row;
    int m_col;
    afx_msg void OnBnClickedButtonAirlabParameter();
    afx_msg void OnPaint();
    afx_msg void OnBnClickedButtonHelpWbgt();
};
#define AIRLAB_TYPE    0
#define AIRLAB_VALUE   1
#define AIRLAB_UNIT    2
#define AIRLAB_TRIGGER 3
#define AIRLAB_TIME    4
#define AIRLAB_TIME_LEFT 5
#define AIRLAB_ALARM   6
#define AIRLAB_MAX_COUNT 7


const int AIRLAB_CO2 = 0;
const int AIRLAB_OCC = 1;
const int AIRLAB_SOUND = 2;
const int AIRLAB_LIGHT = 3;

const COLORREF AQI_Info_Status_Color[] =
{
    RGB(255,0,255),
    RGB(150,0,150),
    RGB(147,106,0),
    RGB(205,156,109),
    RGB(180,157,225),
    RGB(83,39,144)
};

const COLORREF AQI_Info_Status_Back_Color[] =
{
    RGB(255- 255,255- 0,255 - 255),
    RGB(255- 150,255- 0,255 - 150),
    RGB(255- 147,255- 106,255 - 0),
    RGB(255- 205,255- 156,255 - 109),
    RGB(255- 180,255- 157,255 - 225),
    RGB(255- 83 ,255- 39 ,255 - 144)
};


static DWORD WINAPI Update_TstatAQ_Thread(LPVOID lPvoid);
const int TSTAT_AQ_TEMP_AMBIENT = 101;
const int TSTAT_AQ_TEMP_REMOTE = 102;  //���⴫�����������¶�ֵ; - Two temperature values from infrared sensor;
const int TSTAT_AQ_TEMP_UNIT = 104;   //�¶ȵ�λ   0 ���϶�     1���϶� - Temperature unit: 0 Celsius, 1 Fahrenheit
const int TSTAT_AQ_TEMPERATURE = 121;
const int TSTAT_AQ_HUM = 140;
const int TSTAT_AQ_CO2 = 139;
const int TSTAT_AQ_VOC = 998;

const int TSTAT_AQ_VOC_AIRLAB = 988;  //��Ʒ��62 ��Airlab �õ���988 - Product number 62 Airlab uses 988

const int TSTAT_AQ_WEIGHT_1 = 760;
const int TSTAT_AQ_WEIGHT_2_5 = 761;
const int TSTAT_AQ_WEIGHT_4 = 762;
const int TSTAT_AQ_WEIGHT_10 = 763;
const int TSTAT_AQ_INDEX_1 = 764;
const int TSTAT_AQ_INDEX_2_5 = 765;
const int TSTAT_AQ_INDEX_4 = 766;
const int TSTAT_AQ_INDEX_10 = 767;
const int TSTAT_AQ_LIGHT = 628;



const int TSTAT_AQ_SOUND_TRIGGER = 613;
const int TSTAT_AQ_SOUND_TIMER = 614;
const int TSTAT_AQ_SOUND_ALARM_ON = 615;
const int TSTAT_AQ_SOUND_COUNT_DOWN = 616;

const int TSTAT_AQ_LIGHT_TRIGGER = 617;
const int TSTAT_AQ_LIGHT_TIMER = 618;
const int TSTAT_AQ_LIGHT_ALARM_ON = 619;
const int TSTAT_AQ_LIGHT_COUNT_DOWN = 620;

const int TSTAT_AQ_OCC_VALUE = 630;
const int TSTAT_AQ_OCC_TRIGGER = 633;
const int TSTAT_AQ_OCC_TIMER = 634;
const int TSTAT_AQ_OCC_ALARM_ON = 635;
const int TSTAT_AQ_OCC_COUNT_DOWN = 636;

const int TSTAT_AQ_CO2_TRIGGER = 621;
const int TSTAT_AQ_CO2_TIMER = 622;
const int TSTAT_AQ_CO2_ALARM_ON = 623;
const int TSTAT_AQ_CO2_COUNT_DOWN = 624;

const int TSTAT_AQ_SOUND = 640;
const int TSTAT_AQ_WBGT = 994;
const int TATAT_AQ_CO2_ON = 1009;
const int TATAT_AQ_CO2_OFF = 1010;
const int TATAT_AQ_PM_ON = 1011;
const int TATAT_AQ_PM_OFF = 1012;

const int TATAT_AQ_MODBUS_AQI = 1010;
const int TATAT_AQ_MODBUS_AQI_LEVEL = 1011;
const int TATAT_AQ_MODBUS_AQI_AREA = 1012;  // 0 for USA, 1 for China, and 2 for customer customization
const int TATAT_AQ_MODBUS_AQI_CUSTOMER_FIRST_LINE = 1013;
const int TATAT_AQ_MODBUS_AQI_CUSTOMER_SECOND_LINE = 1014;
const int TATAT_AQ_MODBUS_AQI_CUSTOMER_THIRD_LINE = 1015;
const int TATAT_AQ_MODBUS_AQI_CUSTOMER_FOURTH_LINE = 1016;
const int TATAT_AQ_MODBUS_AQI_CUSTOMER_FIFTH_LINE = 1017;


const int PM25_MODBUS_AQI = 102;
const int PM25_MODBUS_AQI_LEVEL = 103;
const int PM25_MODBUS_WORK_PERIOD = 104;
const int PM25_MODBUS_PM25_SENSOR_ID = 105;
const int PM25_MODBUS_PM25_SENSOR_STATUS = 107;
const int PM25_MODBUS_MAIN_DISPLAY_SET = 109;
const int PM25_MODBUS_SCROLL_DISPLAY_SET = 110;
const int PM25_MODBUS_MAIN_DISPLAY_SWITCH_TIME = 111;
const int PM25_MODBUS_PM25_OFFSET = 112;
const int PM25_MODBUS_PM10_OFFSET = 113;
const int PM25_MODBUS_PM25_FILTER = 114;
const int PM25_MODBUS_PM10_FILTER = 115;
const int PM25_MODBUS_PM_OUTPUT_AUTO_MANUAL = 116;
const int PM25_MODBUS_PM_OUTPUT_MODE = 117;
const int PM25_MODBUS_PM10_OUTPUT_MIN_RANGE = 118;
const int PM25_MODBUS_PM10_OUTPUT_MAX_RANGE = 119;
const int PM25_MODBUS_PM25_OUTPUT_MIN_RANGE = 120;
const int PM25_MODBUS_PM25_OUTPUT_MAX_RANGE = 121;
const int PM25_MODBUS_PM25_WEIGHT_1_0 = 126;
const int PM25_MODBUS_PM25_WEIGHT_2_5 = 127;
const int PM25_MODBUS_PM25_WEIGHT_4_0 = 128;
const int PM25_MODBUS_PM25_WEIGHT_10 = 129;
const int PM25_MODBUS_PM25_NUMBER_0_5 = 130;
const int PM25_MODBUS_PM25_NUMBER_1_0 = 131;
const int PM25_MODBUS_PM25_NUMBER_2_5 = 132;
const int PM25_MODBUS_PM25_NUMBER_4_0 = 133;
const int PM25_MODBUS_PM25_NUMBER_10 = 134;
const int PM25_MODBUS_PM25_OUTPUT_RANGE = 135;
const int PM25_MODBUS_PM25_AREA = 136;
const int PM25_MODBUS_AQI_FIRST_LINE = 137;
const int PM25_MODBUS_AQI_SECOND_LINE = 138;
const int PM25_MODBUS_AQI_THIRD_LINE = 139;
const int PM25_MODBUS_AQI_FOURTH_LINE = 140;
const int PM25_MODBUS_AQI_FIFTH_LINE = 141;
//const int PM25_MODBUS_PM10_OFFSET = 142;
const int PM25_MODBUS_PM10_OFFSET_ENABLE = 143;
const int PM25_MODBUS_TEMPERATURE_FAHRENHEIT = 144;
const int PM25_MODBUS_TEMPERATURE_CELSIUS = 145;
const int PM25_MODBUS_HUMIDITY = 146;
const int PM25_MODBUS_UART1_PARITY = 3079;





