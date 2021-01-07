#pragma once


// AnnualRout_InsertDia dialog
#include <vector>
#include "afxwin.h"
#include "atlcomtime.h"
#include "afxdtctl.h"
#include "monthview1.h"
#include "../MultipleMonthCal32/MultipleMonthCalCtrl.h"
using namespace std;

//#define NTDDI_LONGHORN  0x051

// AnnualRout_InsertDia 对话框
#define ONE_YEAR_BETYS 46
class AnnualRout_InsertDia : public CDialog
{
	DECLARE_DYNAMIC(AnnualRout_InsertDia)

public:
	AnnualRout_InsertDia(CWnd* pParent = NULL);   // standard constructor
	AnnualRout_InsertDia(unsigned char row,CString strtype = _T(""),CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AnnualRout_InsertDia();

// 对话框数据
	enum { IDD = IDD_ANNUAL_ROUTINES_INSERT_DIA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
public:
	//mannual insert
	void load();
	unsigned char m_addr;
	unsigned short the_days[ONE_YEAR_BETYS];
	bool is_leap;//true leap year,false no leap year
	vector< CString >holiday_list;
	vector< CString >::iterator list_pointer;
	void leap_year();//if the year is leap year,get the holiday by this function
	void no_leap_year();//if the year is not a leap year,get the holiday by this function
	int month_nCount;//for set_day_state function
	LPMONTHDAYSTATE pDayState;//for set_day_state function
	SYSTEMTIME timeFrom;//for set_day_state function
	SYSTEMTIME timeUntil;//for set_day_state function
	void set_day_state(CString month_day);///////////////attention ,the control must be 12 month
	void add_holiday(CString var1);
	void delete_holiday(CString var1);
	int the_day_number(CString var1);
	bool the_holiday_is_exist(CString var1);//return value ,true is exist ,false is no exist
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	CListBox m_list_ctrl;
	COleDateTime m_date_time;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnDestroy();
	//afx_msg void OnBnClickedButton3();
	CMultipleMonthCalCtrl m_month_ctrl;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnAnnualroutAdd();
	afx_msg void OnAnnualroutClear();
	afx_msg void OnBnClickedOk();

	BOOL m_bXpOS;
	
//	CMonthview1 m_monthViewCtrl;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CComboBox m_yearComBox;
	afx_msg void OnCbnSelchangeCombo1();
	CString m_strtype;


	afx_msg void OnMcnSelectBacMonthcalendar(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT  DayResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	LRESULT Fresh_Schedule_Day_Cal(WPARAM wParam,LPARAM lParam);
	//LPMONTHDAYSTATE pDayState;
	MONTHDAYSTATE	pBacDayState[12];// = new MONTHDAYSTATE[nCount];

	BOOL m_offline;
	CString m_configfile_path;

    int m_leap_year;

};
