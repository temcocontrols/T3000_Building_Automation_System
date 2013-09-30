#pragma once
#include "afxcmn.h"
#include "CM5/ListCtrlEx.h"

// BacnetController dialog

class BacnetController : public CDialogEx
{
	DECLARE_DYNAMIC(BacnetController)

public:
	BacnetController(CWnd* pParent = NULL);   // standard constructor
	virtual ~BacnetController();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_CONTROLLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	ListCtrlEx::CListCtrlEx m_controller_list;
	void Initial_List();
	afx_msg LRESULT Fresh_Controller_List(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Controller_Item(WPARAM wParam,LPARAM lParam);
};

const int CONTROLLER_NUM = 0;
const int CONTROLLER_INPUT = 1;
const int CONTROLLER_INPUTVALUE = 2;
const int CONTROLLER_INPUTUNITS = 3;
const int CONTROLLER_AUTO_MANUAL = 4;
const int CONTROLLER_OUTPUT = 5;
const int CONTROLLER_SETPOINT = 6;
const int CONTROLLER_SETVALUE = 7;
const int CONTROLLER_SETPOINTUNITS = 8;
const int CONTROLLER_ACTION = 9;
const int CONTROLLER_PROPORTIONAL = 10;
const int CONTROLLER_RESET = 11;
const int CONTROLLER_RATE = 12;
const int CONTROLLER_BIAS = 13;

