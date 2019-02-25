#pragma once
#include "afxcmn.h"

#include "CM5/ListCtrlEx.h"
// CBacnetRemotePoint dialog

class CBacnetRemotePoint : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetRemotePoint)

public:
	CBacnetRemotePoint(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetRemotePoint();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_REMOTE_POINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlEx::CListCtrlEx m_remote_point_list;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	afx_msg void OnClose();
	void Initial_List();
	afx_msg LRESULT Fresh_Remote_List(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void Reset_RemotePoint_Rect();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	bool window_max; // 标记是否已经最大化;
};

const int REMOTE_NUMBER = 0;
const int REMOTE_MAIN_ID = 1;
const int REMOTE_DEVICE_ID = 2;
const int REMOTE_REG = 3;
const int REMOTE_INSTANCE = 4;
const int REMOTE_TPYE = 5;
const int REMOTE_VALUE = 6;
const int REMOTE_DEVICE_STATUS = 7;
const int REMOTE_DESCRIPTION = 8;
const int REMOTE_TIME_REMAINING = 9;

const int REMOTE_COL_NUMBER = 10;
