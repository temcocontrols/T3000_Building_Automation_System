#pragma once

#include "CM5/ListCtrlEx.h"
#include "afxwin.h"
#include "CM5/CStatic/staticex.h"
// CBacnetOutput dialog

class CBacnetOutput : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetOutput)

public:
	CBacnetOutput(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetOutput();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_OUTPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	HICON	hIcon;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonOutputRead();
	afx_msg LRESULT OutputMessageCallBack(WPARAM wParam, LPARAM lParam);
			LRESULT OutputMessageCallBack_Tstat(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Output_List(WPARAM wParam,LPARAM lParam);
	        LRESULT Fresh_Output_List_Tstat(WPARAM wParam,LPARAM lParam);
//	afx_msg LRESULT OutputResumeMessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Output_Item(WPARAM wParam,LPARAM lParam);
			LRESULT Fresh_Output_Item_Tstat(WPARAM wParam,LPARAM lParam);
	ListCtrlEx::CListCtrlEx m_output_list;
	void Initial_List();
	void Initial_ListFor_Tstat();
	void Initial_Combox_Show_Tstat();
	void Reload_Unit_Type();
	afx_msg void OnNMClickListOutput(NMHDR *pNMHDR, LRESULT *pResult);
			void OnNMClickListOutput_Tstat(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	virtual void OnCancel();
public:
	int m_latest_protocol;
	bool window_max; // 标记是否已经最大化;
	void Fresh_One_Item(int row);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	CStaticEx m_output_item_info;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	void Reset_Output_Rect();
    afx_msg void OnNMRClickListOutput(NMHDR *pNMHDR, LRESULT *pResult);
};




const int TStat_OutputName = 1 ;
const int TStat_AM =2 ;
const int TStat_Value = 3 ;
const int TStat_Unit = 4 ;
const int TStat_Range = 5 ;
const int TStat_Funcation = 6 ;
const int TStat_Interlock = 7 ;
const int TStat_OFFON_Delay = 8 ;
const int TStat_ONOFF_Delay = 9 ;
const int TStat_Signal_Type = 10 ;



