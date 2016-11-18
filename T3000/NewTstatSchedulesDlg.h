#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"
#include <list>
#include "afxdtctl.h"
using namespace std;
// CNewTstatSchedulesDlg dialog
struct Schedule_Node 
{
	int Hour;
	int Minite;
	int Monday;
	int Tuesday;
	int Wednesday;
	int Thursday;
	int Friday;
	int Saturday;
	int Sunday;

	COLORREF Col_Monday;
	COLORREF Col_Tuesday;
	COLORREF Col_Wednesday;
	COLORREF Col_Thursday;
	COLORREF Col_Friday;
	COLORREF Col_Saturday;
	COLORREF Col_Sunday;
};
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
	ListCtrlEx::CListCtrlEx WeeeklyList;
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
	BOOL Delete_Schdule(int POS);
	list<Schedule_Node>::iterator GetNode(int POS);
	int GetValueItem(int Row,int Col); 
	COLORREF GetItemColor(int Row,int Col); 
	void SetValueItem(int Row,int Col,int Val); 
	int GetDayScheduleCount(int Col);
	CString m_strScheduleMode[4];
	COLORREF m_COLScheduleMode[4];
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
};
