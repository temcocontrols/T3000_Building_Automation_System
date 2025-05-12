#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"
#include <list>
#include <vector>
#include <cctype>
#include "afxdtctl.h"
using namespace std;
// CNewTstatSchedulesDlg dialog
class Schedule_Node 
{
public:
	int Hour;
	int Minite;
	int Monday;
	int Tuesday;
	int Wednesday;
	int Thursday;
	int Friday;
	int Saturday;
	int Sunday;
	int Holiday;

	COLORREF Col_Monday;
	COLORREF Col_Tuesday;
	COLORREF Col_Wednesday;
	COLORREF Col_Thursday;
	COLORREF Col_Friday;
	COLORREF Col_Saturday;
	COLORREF Col_Sunday;
	COLORREF Col_Holiday;

public:
	Schedule_Node()
	{
		Hour = 0;
		Minite = 0;
		Monday = 0;
		Tuesday = 0;
		Wednesday = 0;
		Thursday = 0;
		Friday = 0;
		Saturday = 0;
		Sunday = 0;
		Holiday = 0;
		Col_Monday = 0;
		Col_Tuesday = 0;
		Col_Wednesday = 0;
		Col_Thursday = 0;
		Col_Friday = 0;
		Col_Saturday = 0;
		Col_Sunday = 0;
		Col_Holiday = 0;
	}

	inline bool operator<(const Schedule_Node& t1) {
		return Hour*60+ Minite < t1.Hour*60+t1.Minite;    //会产生升序排序,若改为>,则变为降序
	}

};
struct Event
{
	int Day;
	int Hour;
	int Minite;
	int Event_Number;
};
typedef vector<Event> DayEvent;
class CNewTstatSchedulesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewTstatSchedulesDlg)

public:
	CNewTstatSchedulesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewTstatSchedulesDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SCHEDULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlEx::CListCtrlEx WeeklyList;
	CDateTimeCtrl m_start_time;
	CDateTimeCtrl m_end_time;
	afx_msg void OnBnClickedButtonInsert();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg LRESULT Fresh_Input_Item(WPARAM wParam,LPARAM lParam);
	virtual BOOL OnInitDialog();
	void Fresh_List();
	int getvalue(CString modelname);
	 
	list<Schedule_Node> m_ScheduleList;
	 
	BOOL Insert_Schdule(Schedule_Node SR,int POS);
	BOOL InsertAndUpdate_Schdule(Schedule_Node SR);
	BOOL Delete_Schdule(int POS);
	void CopyFromMonToFri();
	list<Schedule_Node>::iterator GetNode(int POS);
	int GetValueItem(int Row,int Col); 
	COLORREF GetItemColor(int Row,int Col); 
	void SetValueItem(int Row,int Col,int Val); 
	int GetDayScheduleCount(int Col);
	CString m_strScheduleMode[10];
	COLORREF m_COLScheduleMode[6];
	CString m_oldtext;
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	int m_curRow;
	int m_curCol;
	int m_hour;
	int m_minute;
	CDateTimeCtrl m_schedual_time_picker;
	afx_msg void OnNMKillfocusDatetimepicker1Schedual(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonScheduleCopyBtn();
	afx_msg void OnBnClickedOk();
	void LoadSheduleDataAndColor();
	int GetEventNumber(int DayIndex);
	int PowerFour(int number,int index);
	unsigned short m_SchduleBuffer[121];
 
	Event WeeklyEvent[48];
	DayEvent m_Monday;
	DayEvent m_Tuesday;
	DayEvent m_Wednesday;
	DayEvent m_Thursday;
	DayEvent m_Friday;
	DayEvent m_Saturday;
	DayEvent m_Sunday;
	DayEvent m_Holiday;
	 
	 
	afx_msg void OnBnClickedCheckEnableSchedule();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};
