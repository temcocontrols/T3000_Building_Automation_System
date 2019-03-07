#pragma once
#include "afxcmn.h"

#include "CM5/ListCtrlEx.h"
// CBacnetAnalogCusRang dialog

class CBacnetAnalogCusRang : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetAnalogCusRang)

public:
	CBacnetAnalogCusRang(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetAnalogCusRang();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_RANGES_ANALOG_DEF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	ListCtrlEx::CListCtrlEx m_analog_cus_range_list;
	void Initial_List();
	afx_msg LRESULT Fresh_AnalogCusRange_Item(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_AnalogCusRange_List(WPARAM wParam,LPARAM lParam);
	afx_msg void OnClose();
	afx_msg LRESULT  AnalogRangeTblMessageCallBack(WPARAM wParam, LPARAM lParam);
	void SetSlideRange();
	CSliderCtrl m_cus_analog_range_ctrl_0;
	CSliderCtrl m_cus_analog_range_ctrl_1;
	CSliderCtrl m_cus_analog_range_ctrl_2;
	CSliderCtrl m_cus_analog_range_ctrl_3;
	CSliderCtrl m_cus_analog_range_ctrl_4;
	CSliderCtrl m_cus_analog_range_ctrl_5;
	CSliderCtrl m_cus_analog_range_ctrl_6;
	CSliderCtrl m_cus_analog_range_ctrl_7;
	CSliderCtrl m_cus_analog_range_ctrl_8;
	CSliderCtrl m_cus_analog_range_ctrl_9;
	CSliderCtrl m_cus_analog_range_ctrl_10;
	    CToolTipCtrl m_tooltips;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	 void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	 BOOL OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	  int m_tipvalue;
	  afx_msg void OnEnKillfocusEditBacCusAnalogUnit();
	  void UpdateCusAnalogUnit();
	  BOOL CheckAllDataValid();
	  afx_msg void OnPaint();
	  afx_msg void OnTimer(UINT_PTR nIDEvent);
      afx_msg void OnEnKillfocusEditcusPointValue();
      afx_msg void OnEnKillfocusEditMinVoltValue();
      afx_msg void OnEnKillfocusEditMinValueValue();
      afx_msg void OnEnKillfocusEditMaxVoltValue3();
      afx_msg void OnEnKillfocusEditMaxValueValue();
      void ReSetSlideAndList();
      void InitialPointCount(); //弹出对话框 初始化 个数以及最大最小值;
      void ShowWindowByCount(int n_count);
      int n_point_count ;
      void ReAllocationValue();
      void ReAllocationVolts();
      void Handle_Static_ctrl_0();
      void Handle_Static_ctrl_1();
      void Handle_Static_ctrl_2();
      void Handle_Static_ctrl_3();
      void Handle_Static_ctrl_4();
      void Handle_Static_ctrl_5();
      void Handle_Static_ctrl_6();
      void Handle_Static_ctrl_7();
      void Handle_Static_ctrl_8();
      void Handle_Static_ctrl_9();
      void Handle_Static_ctrl_10();
};

const int ANALOG_CUS_RANGE_RBL_VALUE = 0;
const int ANALOG_CUS_RANGE_RBL_UNIT = 1;
