#pragma once
#include "CM5/ListCtrlEx.h"

// CTstatSetpointDetail dialog
// CTstatSetpointDetail �Ի���

class CTstatSetpointDetail : public CDialogEx
{
	DECLARE_DYNAMIC(CTstatSetpointDetail)

public:
    // Standard constructor
	CTstatSetpointDetail(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTstatSetpointDetail();

// Dialog data
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TSTAT_SETPOINT };
#endif

protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
    void Fresh_window();
    ListCtrlEx::CListCtrlEx m_tstat_setpoint_list;
    void Initial_List();
    virtual BOOL OnInitDialog();
    afx_msg LRESULT Fresh_Setpoint_Item(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT Fresh_Setpoint_List(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT  ReadDataAgain(WPARAM wParam, LPARAM lParam);
};

void Check_Tstat_Home_Setpoint();
void Check_Tstat_Work_Setpoint();
void Check_Tstat_Away_Setpoint();
void Check_Tstat_Sleep_Setpoint();

const int TSTAT_SETPOINT_ITEM = 0;
const int TSTAT_SETPOINT_TYPE = 1;
const int TSTAT_SETPOINT_SETPOINT = 2;
const int TSTAT_SETPOINT_HEAT_SP = 3;
const int TSTAT_SETPOINT_COOL_SP = 4;
const int TSTAT_SETPOINT_COOL_DB = 5;
const int TSTAT_SETPOINT_HEAT_DB = 6;
const int TSTAT_SETPOINT_MAX = 7;
const CString SetpointType[2] =
{
    _T("Single"),
    _T("Dual")
};
