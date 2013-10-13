#pragma once

#include "msflexgrid1.h"
#include "MbPollOptions.h"
#include "MbpExterns.h"
//#include "E:\Home\Personal\Extra Curricular\Proj5\MBP\MBP\20131002\T3000_Building_Automation_System-master\T3000\CM5\myownlistctrl.h"
#include "CM5\myownlistctrl.h"

#define WM_MBPOLL_CLOSED (WM_USER + 555)
#define WM_TRAFFIC_CLOSED	(WM_USER + 556)
// CMbPoll dialog

class CMbPoll : public CDialog
{
	DECLARE_DYNAMIC(CMbPoll)

public:
	CMbPoll(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMbPoll();

// Dialog Data
	enum { IDD = IDD_MBPOLL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	afx_msg void OnNcDestroy();
	virtual void PostNcDestroy();
	CWnd* mParent;
//	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClose();
//	afx_msg void OnSetupOptions();
	CMbPollOptions mbPollOptions;
	CStatic ctrlLabel1;
	UINT_PTR timer1;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit ctrlEditGrid1;
	CString strGrid1;
	CMsflexgrid ctrlGrid1;
	CMsflexgrid ctrlGrid2;
	CMsflexgrid ctrlGrid3;
	CMsflexgrid ctrlGrid4;
	CMsflexgrid ctrlGrid5;
	void ClickGrid(CMsflexgrid &grid);
	void ClickGrid1();
	void EnterCellGrid1();
	void SelChangeGrid1();
	void DblClickGrid1();
	void ClickGrid2();
	void EnterCellGrid2();
	void SelChangeGrid2();
	void DblClickGrid2();
	void ClickGrid3();
	void EnterCellGrid3();
	void SelChangeGrid3();
	void DblClickGrid3();
	void ClickGrid4();
	void EnterCellGrid4();
	void SelChangeGrid4();
	void DblClickGrid4();
	void ClickGrid5();
	void EnterCellGrid5();
	void SelChangeGrid5();
	void DblClickGrid5();

	DECLARE_EVENTSINK_MAP()

	afx_msg void OnKillfocusEditForGrid();

	int currentRow;
	int currentCol;
	int curGridForEdit;
	int noOfGridsVisible;

	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboNoofgrids();
	CComboBox comboNoOfGrids;
	int valNoOfGrids;
	void displayGrids(int number);
	afx_msg void OnFileExit();
	void OnMbPollDestroyWindow();
	afx_msg void OnClickedButtonConfig1();
	afx_msg void OnClickedButtonConfig2();
	afx_msg void OnClickedButtonConfig3();
	afx_msg void OnClickedButtonConfig4();
	afx_msg void OnClickedButtonConfig5();
	CButton btnConfig1;
	CButton btnConfig2;
	CButton btnConfig3;
	CButton btnConfig4;
	CButton btnConfig5;
	void updateStaticConfig(int configNo, int noRowsUpdate = 0);
	void updateBtnLabel(int configNo);
	CStatic staticConfig1;
	CStatic staticConfig2;
	CStatic staticConfig3;
	CStatic staticConfig4;
	CStatic staticConfig5;
	void updateDataSharingVariables(int configNo);
	CButton btnStartStop1;
	CButton btnStartStop2;
	CButton btnStartStop3;
	CButton btnStartStop4;
	CButton btnStartStop5;
	afx_msg void OnClickedButtonStartStop1();
	afx_msg void OnClickedButtonStartStop2();
	afx_msg void OnClickedButtonStartStop3();
	afx_msg void OnClickedButtonStartStop4();
	afx_msg void OnClickedButtonStartStop5();
	void OnClickedButtonStartStop(CMsflexgrid &grid, int gridNum, CButton &btn, CButton &configbtn);
	afx_msg void OnClickedCheckTapData();
	CButton checkTapData;
	void startStopBtnState();
	void restoreConfigs();
	CStatic staticRunning1;
	CStatic staticRunning2;
	CStatic staticRunning3;
	CStatic staticRunning4;
	CStatic staticRunning5;
	void callMbFunc(int slotNo);
	CString convertFunction(int convType, unsigned short val);
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	void saveRegNames();
	void restoreRegNames();
	CStatic staticConnectionStatus;
	afx_msg void OnClickedButtonTraffic1();
	afx_msg void OnClickedButtonTraffic2();
	afx_msg void OnClickedButtonTraffic3();
	afx_msg void OnClickedButtonTraffic4();
	afx_msg void OnClickedButtonTraffic5();
	CButton btnShowTraffic1;
	CButton btnShowTraffic2;
	CButton btnShowTraffic3;
	CButton btnShowTraffic4;
	CButton btnShowTraffic5;
	void showTrafficWindow(int slotNo);
protected:
	afx_msg LRESULT OnTrafficClosed(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
protected:
	int m_nCurHeight;
	int m_nScrollPos;
	CRect m_rect;
	int m_nCurWidth;
	int m_nHScrollPos;
public:
	CMyOwnListCtrl mbPollList;
	CRect mbPollListRect;
	void InitListCtrl();
	static DWORD WINAPI displayThreadStarter(LPVOID lpVoid);
	DWORD WINAPI displayThread();
	static DWORD WINAPI mbCommThreadStarter(LPVOID lpVoid);
	DWORD WINAPI mbCommThread();
	int radioButtonShow;
	void showRadioButton();
	void restoreRadioButtonState();
	void saveRadioButtonState();
	afx_msg void OnClickedMbpollCheck1();
	afx_msg void OnClickedMbpollCheck2();
	afx_msg void OnClickedMbpollCheck3();
	afx_msg void OnClickedMbpollCheck4();
	CButton mbpollCheck1;
	CButton mbpollCheck2;
	CButton mbpollCheck3;
	CButton mbpollCheck4;
	void OnClickedMbpollCheck(int gridNo);
	void saveRegisterNames(int gridNo);
	void restoreRegisterNames(int gridNo);
	CString registerNames1;
	CString registerNames2;
	CString registerNames3;
	CString registerNames4;
};

