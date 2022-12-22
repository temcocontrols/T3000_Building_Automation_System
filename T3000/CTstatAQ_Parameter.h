#pragma once


// CTstatAQ_Parameter 对话框

class CTstatAQ_Parameter : public CDialogEx
{
	DECLARE_DYNAMIC(CTstatAQ_Parameter)

public:
	CTstatAQ_Parameter(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTstatAQ_Parameter();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AIRLAB_PARAMETER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void InitialUI();
	afx_msg void OnCbnSelchangeComboAirlabLine1();
	afx_msg void OnCbnSelchangeComboAirlabLine2();
	afx_msg void OnCbnSelchangeComboAirlabLine3();
	afx_msg void OnCbnSelchangeComboAirlabLine4();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedButtonDone();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnCbnSelchangeComboAirlabLine5();
};

const int TSTAT_AQ_BLANK_SCREEN = 1955;
const int TATAT_AQ_CONFIG_LINE1 = 1956;
const int TATAT_AQ_CONFIG_LINE2 = 1957;
const int TATAT_AQ_CONFIG_LINE3 = 1958;
const int TATAT_AQ_CONFIG_LINE4 = 1959;
const int TATAT_AQ_CONFIG_LINE5 = 1954;

const int HIDE_IN_ARRAY = 5;
const CString AirLab_LCD[] =
{
	_T("Temperature"),
	_T("Humidity"),
	_T("CO2"),
	_T("VOC"),
	_T("Light"),
	_T("Hide Item")
};