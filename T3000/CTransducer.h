#pragma once



// CTransducer 窗体视图

class CTransducer : public CFormView
{
	DECLARE_DYNCREATE(CTransducer)

public:
	CTransducer();           // 动态创建所使用的受保护的构造函数
	virtual ~CTransducer();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TRANSDUCER };
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
	virtual void OnInitialUpdate();
	void Fresh();
	afx_msg LRESULT UpdateUI(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedRadioTransducerDegC();
	afx_msg void OnBnClickedRadioTransducerDegF();
	afx_msg void OnEnKillfocusEditTempOffset();
	afx_msg void OnEnKillfocusEditHumOffset();
	afx_msg void OnBnClickedRadioTransducer420Ma();
	afx_msg void OnBnClickedRadioTransducer010v();
	afx_msg void OnBnClickedRadioDew();
	afx_msg void OnBnClickedRadioEnthalpy();
	afx_msg void OnBnClickedRadioAbs();
	afx_msg void OnBnClickedRadioReal();
};

#define MODBUS_SWITCH_OUTPUT_MODE    17
#define MODBUS_SWITCH_TEMP_RANGE     18
#define MODBUS_SWITCH_HUMI_RANGE	 19
#define MODBUS_TEMPERATURE_TYPE			23
#define MODBUS_TEMPERATURE			 34
#define MODBUS_HUMIDITY				35
#define MODBUS_TEMPERATURE_OFFSET   36
#define MODBUS_HUMIDITY_OFFSET      37
#define MODBUS_DEWPOINT				41
#define MODBUS_ENTHALPY			    42
#define MODBUS_ABSOLUTE_HUMI		44


#define HUM_DEWPOINT   0  
#define HUM_ENTHALPY   1
#define HUM_ABSOLUTE    2
#define HUM_REAL        3

const CString TransducerTempRange[] =
{
	_T("0.   0  - 100"),
	_T("1.   20 - 80"),       //1
	_T("2.   0  - 50"),
	_T("3.  -50 - 50")
};

