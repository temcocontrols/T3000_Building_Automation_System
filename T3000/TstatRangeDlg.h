#pragma once
#include "afxwin.h"

#include "CM5\CStatic\staticex.h"
// CTstatRangeDlg dialog
#define  WM_TSTAT_RANGE WM_USER + 3300
class CTstatRangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTstatRangeDlg)

public:
	CTstatRangeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTstatRangeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TSTAT_RANGES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStaticEx m_show_unit;
	CStaticEx m_static_range_units_select;
	CStatic m_rang_pic;
	virtual BOOL OnInitDialog();
	void Initial_window();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Click_Radio();
    void InitRangeUI();
public:
  
	int m_input_Analog_select;
	 
	//afx_msg void OnEnKillfocusEditRangeSelect();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnSetfocusEditRangeSelect();
	afx_msg void OnBnClickedOk();
	int m_current_range;
    int m_10v;
	afx_msg void OnBnClickedRadioT0();
	afx_msg void OnBnClickedRadioT1();
	afx_msg void OnBnClickedRadioT2();
	afx_msg void OnBnClickedRadioT3();
	afx_msg void OnBnClickedRadioT4();
	afx_msg void OnBnClickedRadio();
	afx_msg void OnBnClickedRadioT6();
	afx_msg void OnBnClickedRadioT7();
	afx_msg void OnBnClickedRadioT8();
	afx_msg void OnBnClickedRadioT9();
	afx_msg void OnBnClickedRadioT10();
	afx_msg void OnBnClickedRadioT11();
    afx_msg void OnBnClickedRadioT12();
   
     //////////////////////////////////////////////////////////////////////////
    afx_msg void OnEnChangeEdit1();
    afx_msg void OnEnChangeEdit8();
    afx_msg void OnEnChangeEdit9();
    afx_msg void OnEnChangeEdit13();
    afx_msg void OnEnChangeEdit14();
    afx_msg void OnEnChangeEdit15();
    afx_msg void OnEnChangeEdit16();
    afx_msg void OnEnChangeEdit17();
    afx_msg void OnEnChangeEdit18();
    afx_msg void OnEnChangeEdit19();
    afx_msg void OnEnChangeEdit20();
    afx_msg void OnEnChangeEdit12();
    afx_msg void OnEnKillfocusEdit1();
    afx_msg void OnEnKillfocusEdit8();
    afx_msg void OnEnKillfocusEdit9();
    afx_msg void OnEnKillfocusEdit13();
    afx_msg void OnEnKillfocusEdit14();
    afx_msg void OnEnKillfocusEdit15();
    afx_msg void OnEnKillfocusEdit16();
    afx_msg void OnEnKillfocusEdit17();
    afx_msg void OnEnKillfocusEdit18();
    afx_msg void OnEnKillfocusEdit19();
    afx_msg void OnEnKillfocusEdit20();
    afx_msg void OnEnKillfocusEdit12();
    afx_msg void OnEnChangeEdit10();
    afx_msg void OnEnSetfocusEdit10();
    //
     void to_fresh();
     void refresh();
     short Get_Min(short *short_Array,int length);
         short Get_Max(short *short_Array,int length) ;
         void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
          BOOL OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
          void SaveInfoDataToRegister();
          BOOL CheckDataisRight();
private:
    int m_nMax;
    int m_nMin;
    BOOL m_bIncreasingMode;
    bool IsModfied ;	
    float m_version;
    int m_tipvalue;


    int m_slider1_i;
    int m_slider2_i;
    int m_slider3_i;
    int m_slider4_i;
    int m_slider5_i;
    int m_slider6_i;
    int m_slider7_i;
    int m_slider8_i;
    int m_slider9_i;
    int m_slider10_i;
    int m_slider11_i;
    int m_InputNo;          //false is table1,true is table2;
    CString m_units_s;
    CString m_max;
    CString m_min;


    CSliderCtrl m_slider1_ctrl;
    CSliderCtrl m_slider2_ctrl;
    CSliderCtrl m_slider3_ctrl;
    CSliderCtrl m_slider4_ctrl;
    CSliderCtrl m_slider5_ctrl;
    CSliderCtrl m_slider6_ctrl;
    CSliderCtrl m_slider7_ctrl;
    CSliderCtrl m_slider8_ctrl;
    CSliderCtrl m_slider9_ctrl;
    CSliderCtrl m_slider10_ctrl;
    CSliderCtrl m_slider11_ctrl;
    CToolTipCtrl m_tooltips;
public:
    afx_msg void OnBnClickedRadio5v();
    afx_msg void OnBnClickedRadio10v();
	afx_msg void OnBnClickedRadioT13();
	afx_msg void OnBnClickedRadioT14();
    void Show4_20maUI(bool nshow);
    void Show5_10VUI(bool nshow);
};
