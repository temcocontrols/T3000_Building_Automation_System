#pragma once

#include "CM5/ListCtrlEx.h"
#include "afxcmn.h"
// CTstatZigbeeLogic dialog

class CTstatZigbeeLogic : public CDialogEx
{
	DECLARE_DYNAMIC(CTstatZigbeeLogic)

public:
	CTstatZigbeeLogic(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTstatZigbeeLogic();

// Dialog Data
	enum { IDD = IDD_DIALOG_TSTAT_ZIGBEE_LOGIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	ListCtrlEx::CListCtrlEx m_zigbee_logic_list;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void AddTSTAT_ID(unsigned short * npoint , int nlength,unsigned char nId);
	void AddMinipanel_ID(unsigned short * npoint , int nlength,unsigned char nId);
   static	UINT   GetZigbeeInfo(LPVOID lpVoid);
	void HandleOneTSTAT(unsigned char nId);
	void HandleOneMinipanel(unsigned char nId);
	afx_msg LRESULT  HandleMessage(WPARAM wParam, LPARAM lParam);
	void Initial_ID_List();
	void MainListRefresh();
public:
	vector <unsigned char> Zigbee_Tstat_Id;
	vector <unsigned char> Zigbee_Show_Tstat_Id;
	ListCtrlEx::CListCtrlEx m_tstat_id_list;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	COLORREF GetCOLORREF(int Value);
	BOOL m_abort;

};




const int  MINIPANEL_ZIGBEE_RSSI_COUNT = 5030;
const int  MINIPANEL_ZIGBEE_FIRST_ID = 5031;

const int TSTAT_ZIGBEE_RSSI_COUNT = 51;
const int TSTAT_ZIGBEE_FIRST_ID = 52;

const int TSTAT_LIST_COLUM_WIDTH = 40;
