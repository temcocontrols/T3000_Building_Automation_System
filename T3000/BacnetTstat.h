#pragma once
#include "afxcmn.h"

#include "CM5/ListCtrlEx.h"
// CBacnetTstat dialog

class CBacnetTstat : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetTstat)

public:
	CBacnetTstat(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetTstat();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_TSTAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlEx::CListCtrlEx m_tstat_list;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Initial_List();
	afx_msg LRESULT Fresh_Tstat_List(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Tstat_Item(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT  TstatsMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnCancel();
};
typedef enum {
	 TSTAT_NUM ,
	 TSTAT_PRODUCT_MODEL_TYPE ,
	 TSTAT_ADDRESS ,
	 TSTAT_SERIAL_NUM ,
	 TSTAT_OCCUPIED ,
	 TSTAT_COOL_SETPOINT ,
	 TSTAT_HEAT_SETPOINT ,
	 TSTAT_ROOM_SETPOINT ,
	 TSTAT_ROOM_TEM ,
	 TSTAT_MODE ,
	 TSTAT_COOL_HEAT_MODE ,
	 TSTAT_OUTPUT_STATE ,
	 TSTAT_NIGHT_HEAT_DB ,
	 TSTAT_NIGHT_COOL_DB ,
	 TSTAT_NIGHT_HEAT_SP ,
	 TSTAT_NIGHT_COOL_SP 
};

const int TSTAT_COL_NUMBER = 16;

const CString CString_Occupied[3] =
{
	_T("Unoccupied"),
	_T("Occupied"),
	_T("Unknown")
};

const CString CString_Cool_Heat[4] =
{
	_T("Coasting"),
	_T("Cool"),
	_T("Heat"),
	_T("Unknown")
};

//const int TSTAT_NUM = 0;
//const int TSTAT_PRODUCT_MODEL_TYPE = 1;
//const int TSTAT_ADDRESS = 2;
//const int TSTAT_SERIAL_NUM = 3;
//const int TSTAT_OCCUPIED = 4;
//const int TSTAT_COOL_SETPOINT = 5;
//const int TSTAT_HEAT_SETPOINT = 6;
//const int TSTAT_ROOM_SETPOINT = 7;
//const int TSTAT_ROOM_TEM = 6;
//const int TSTAT_MODE = 7;
//const int TSTAT_COOL_HEAT_MODE = 8;
//const int TSTAT_OUTPUT_STATE = 9;
//const int TSTAT_NIGHT_HEAT_DB = 10;
//const int TSTAT_NIGHT_COOL_DB = 11;
//const int TSTAT_NIGHT_HEAT_SP = 12;
//const int TSTAT_NIGHT_COOL_SP = 13;




