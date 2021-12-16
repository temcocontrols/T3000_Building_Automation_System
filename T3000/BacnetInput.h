#pragma once
#include "afxcmn.h"

//#include "CM5/MyOwnListCtrl.h"
#include "CM5/ListCtrlEx.h"
#include "afxwin.h"
#include "CM5/CStatic/staticex.h"
// CBacnetInput dialog
#define INPUT_REFRESH_DATA_TIMER  1
#define UPDATE_INPUT_ONE_ITEM_TIMER 3
class CBacnetInput : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetInput)

public:
	CBacnetInput(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetInput();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT Fresh_Input_List(WPARAM wParam,LPARAM lParam);
	//afx_msg LRESULT Initial_Input_List(WPARAM wParam,LPARAM lParam);
	LRESULT Fresh_Input_List_Tstat(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Input_Item(WPARAM wParam,LPARAM lParam);
	LRESULT Fresh_Input_Item_Tstat(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT InputMessageCallBack(WPARAM wParam, LPARAM lParam);

	LRESULT InputMessageCallBack_Tstat(WPARAM wParam, LPARAM lParam);

	void Initial_List();
	void Initial_ListFor_Tstat();
	void Reload_Unit_Type();
	ListCtrlEx::CListCtrlEx m_input_list;
	afx_msg void OnBnClickedOk();
//	afx_msg void OnBnClickedButtonCheckInvokeId();
//	afx_msg LRESULT  InputResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnBnClickedButtonApply();
//	afx_msg void OnBnClickedButtonRead();

	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	        void OnNMClickList_Tstat(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	void Fresh_One_Item(int row);
	void Reset_Input_Rect();
	int m_latest_protocol;
	bool window_max; // 标记是否已经最大化;

/*
Alex
For Help system
*/

//afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

public:
#if 1// TSTAT 
	int m_cvalue;
	int m_crange;
	long m_sn;
	int					m_nModel;
	float				m_fFirmwareVersion;
	vector<CString> m_strUnitList;
	int m_nCurRow;
	int m_nCurCol;

	CMFCButton m_upButton;
	CMFCButton m_downButton;
	CComboBox m_AmCombox;
	CString		m_strCustomsensorUnit;
	BOOL   m_disable_hum ;
	BOOL   m_disable_CO2;
#endif
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	CStaticEx m_input_item_info;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
//int GetPrivateData();
//int WritePrivateData();


