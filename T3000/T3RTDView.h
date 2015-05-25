#pragma once

#include "msflexgrid1.h"
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "global_struct.h"

// CT3RTDView form view
union  DataFormat{
	float floatData;
	unsigned char  charData[4];
};
class CT3RTDView : public CFormView
{
	DECLARE_DYNCREATE(CT3RTDView)

public:
	CT3RTDView();           // protected constructor used by dynamic creation
	virtual ~CT3RTDView();
    virtual void OnInitialUpdate();
public:
	enum { IDD = IDD_T3RTD };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
    void Fresh();
    void InitialDialog();
    void Initial_RegisterList();
    int  Get_RegID(CString Name);
    void InitialTableName();
    vector<T3Register> m_vecT3Register;
    int m_sn;
    CString m_oldname;
    int m_curcol;
    int m_currow;
    HANDLE hFirstThread;
	UINT m_EDIT_ID;
	BOOL m_BACK_fresh;
    private:
        int	SN_LOW	;
        int	SN_HI	;
        int	EPROM_VER_NUMBER	;
        int	FIRMWARE_VER_NUMBER	;
        int	MODBUS_ID	;
        int	PRODUCT_MODEL	;
        int	HARDWARE_VER_NUMBER	;
        int	BAUDRATE	;
        int	FLAG_CALIBRATION	;
        int	MODBUS_FORMAT	;
        int	DEADMASTER_TIME	;
        int	RESOLUTION	;
        int	CHANNEL1_LOW	;
        int	CHANNEL1_HIGHT	;
        int	CHANNEL2_LOW	;
        int	CHANNEL2_HIGHT	;
        int	CHANNEL3_LOW	;
        int	CHANNEL3_HIGHT	;
        int	CHANNEL4_LOW	;
        int	CHANNEL4_HIGHT	;
        int	CHANNEL5_LOW	;
        int	CHANNEL5_HIGHT	;
        int	CHANNEL6_LOW	;
        int	CHANNEL6_HIGHT	;
        int	CHANNEL7_LOW	;
        int	CHANNEL7_HIGHT	;
        int	CHANNEL8_LOW	;
        int	CHANNEL8_HIGHT	;
        int	CHANNEL9_LOW	;
        int	CHANNEL9_HIGHT	;
        int	CHANNEL10_LOW	;
        int	CHANNEL10_HIGHT	;
        int	SECOND	;
        int	MINUTE	;
        int	HOUR	;
        int	DAY	;
        int	WEEK	;
        int	MONTH	;
        int	YEAR	;
        int	TEMP_OFFSET_CHANNEL1_LOW	;
        int	TEMP_OFFSET_CHANNEL1_HIGHT	;
        int	TEMP_OFFSET_CHANNEL2_LOW	;
        int	TEMP_OFFSET_CHANNEL2_HIGHT	;
        int	TEMP_OFFSET_CHANNEL3_LOW	;
        int	TEMP_OFFSET_CHANNEL3_HIGHT	;
        int	TEMP_OFFSET_CHANNEL4_LOW	;
        int	TEMP_OFFSET_CHANNEL4_HIGHT	;
        int	TEMP_OFFSET_CHANNEL5_LOW	;
        int	TEMP_OFFSET_CHANNEL5_HIGHT	;
        int	TEMP_OFFSET_CHANNEL6_LOW	;
        int	TEMP_OFFSET_CHANNEL6_HIGHT	;
        int	TEMP_OFFSET_CHANNEL7_LOW	;
        int	TEMP_OFFSET_CHANNEL7_HIGHT	;
        int	TEMP_OFFSET_CHANNEL8_LOW	;
        int	TEMP_OFFSET_CHANNEL8_HIGHT	;
        int	TEMP_OFFSET_CHANNEL9_LOW	;
        int	TEMP_OFFSET_CHANNEL9_HIGHT	;
        int	TEMP_OFFSET_CHANNEL10_LOW	;
        int	TEMP_OFFSET_CHANNEL10_HIGHT	;
        int	FOUR_POINT_CALIBRATION_1	;
        int	FOUR_POINT_CALIBRATION_2	;
        int	FOUR_POINT_CALIBRATION_3	;
        int	FOUR_POINT_CALIBRATION_4	;
        int	FOUR_POINT_CALIBRATION_5	;
        int	FOUR_POINT_CALIBRATION_6	;
        int	FOUR_POINT_CALIBRATION_7	;
        int	FOUR_POINT_CALIBRATION_8	;
        int	PARAMETER_CHANNEL1_1	;
        int	PARAMETER_CHANNEL1_2	;
        int	PARAMETER_CHANNEL1_3	;
        int	PARAMETER_CHANNEL1_4	;
        int	PARAMETER_CHANNEL1_5	;
        int	PARAMETER_CHANNEL1_6	;
        int	PARAMETER_CHANNEL2_1	;
        int	PARAMETER_CHANNEL2_2	;
        int	PARAMETER_CHANNEL2_3	;
        int	PARAMETER_CHANNEL2_4	;
        int	PARAMETER_CHANNEL2_5	;
        int	PARAMETER_CHANNEL2_6	;
        int	PARAMETER_CHANNEL3_1	;
        int	PARAMETER_CHANNEL3_2	;
        int	PARAMETER_CHANNEL3_3	;
        int	PARAMETER_CHANNEL3_4	;
        int	PARAMETER_CHANNEL3_5	;
        int	PARAMETER_CHANNEL3_6	;
        int	PARAMETER_CHANNEL4_1	;
        int	PARAMETER_CHANNEL4_2	;
        int	PARAMETER_CHANNEL4_3	;
        int	PARAMETER_CHANNEL4_4	;
        int	PARAMETER_CHANNEL4_5	;
        int	PARAMETER_CHANNEL4_6	;
        int	PARAMETER_CHANNEL5_1	;
        int	PARAMETER_CHANNEL5_2	;
        int	PARAMETER_CHANNEL5_3	;
        int	PARAMETER_CHANNEL5_4	;
        int	PARAMETER_CHANNEL5_5	;
        int	PARAMETER_CHANNEL5_6	;
        int	PARAMETER_CHANNEL6_1	;
        int	PARAMETER_CHANNEL6_2	;
        int	PARAMETER_CHANNEL6_3	;
        int	PARAMETER_CHANNEL6_4	;
        int	PARAMETER_CHANNEL6_5	;
        int	PARAMETER_CHANNEL6_6	;
        int	PARAMETER_CHANNEL7_1	;
        int	PARAMETER_CHANNEL7_2	;
        int	PARAMETER_CHANNEL7_3	;
        int	PARAMETER_CHANNEL7_4	;
        int	PARAMETER_CHANNEL7_5	;
        int	PARAMETER_CHANNEL7_6	;
        int	PARAMETER_CHANNEL8_1	;
        int	PARAMETER_CHANNEL8_2	;
        int	PARAMETER_CHANNEL8_3	;
        int	PARAMETER_CHANNEL8_4	;
        int	PARAMETER_CHANNEL8_5	;
        int	PARAMETER_CHANNEL8_6	;
        int	PARAMETER_CHANNEL9_1	;
        int	PARAMETER_CHANNEL9_2	;
        int	PARAMETER_CHANNEL9_3	;
        int	PARAMETER_CHANNEL9_4	;
        int	PARAMETER_CHANNEL9_5	;
        int	PARAMETER_CHANNEL9_6	;
        int	PARAMETER_CHANNEL10_1	;
        int	PARAMETER_CHANNEL10_2	;
        int	PARAMETER_CHANNEL10_3	;
        int	PARAMETER_CHANNEL10_4	;
        int	PARAMETER_CHANNEL10_5	;
        int	PARAMETER_CHANNEL10_6	;
        int	RANGE_CHANNEL1	;
        int	RANGE_CHANNEL2	;
        int	RANGE_CHANNEL3	;
        int	RANGE_CHANNEL4	;
        int	RANGE_CHANNEL5	;
        int	RANGE_CHANNEL6	;
        int	RANGE_CHANNEL7	;
        int	RANGE_CHANNEL8	;
        int	RANGE_CHANNEL9	;
        int	RANGE_CHANNEL10	;
        int	FILTER_CHANNEL1	;
        int	FILTER_CHANNEL2	;
        int	FILTER_CHANNEL3	;
        int	FILTER_CHANNEL4	;
        int	FILTER_CHANNEL5	;
        int	FILTER_CHANNEL6	;
        int	FILTER_CHANNEL7	;
        int	FILTER_CHANNEL8	;
        int	FILTER_CHANNEL9	;
        int	FILTER_CHANNEL10	;
        int	SENSOR_STATUS_CHANNEL1	;
        int	SENSOR_STATUS_CHANNEL2	;
        int	SENSOR_STATUS_CHANNEL3	;
        int	SENSOR_STATUS_CHANNEL4	;
        int	SENSOR_STATUS_CHANNEL5	;
        int	SENSOR_STATUS_CHANNEL6	;
        int	SENSOR_STATUS_CHANNEL7	;
        int	SENSOR_STATUS_CHANNEL8	;
        int	SENSOR_STATUS_CHANNEL9	;
        int	SENSOR_STATUS_CHANNEL10	;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedChangeid();
    CMsflexgrid m_msflexgrid_input;
    CComboBox m_dataformat;
    CEdit m_accuracy;
    afx_msg void OnCbnSelchangeCombo1();
     void OnEnKillfocusEdit1();
    CComboBox m_comboxRange;
    DECLARE_EVENTSINK_MAP()
    void ClickMsflexgridInput();
    CEdit m_inNameEdt;
    afx_msg void OnCbnSelchangeRange();
     void OnEnKillfocusEdit2();
protected:
	afx_msg LRESULT OnFreshPt10Back(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnEnSetfocusEdit1();
	afx_msg void OnEnSetfocusEditName();

	afx_msg void OnBnClickedButtonReset();
};


