#pragma once

#include "ChartCtrl/ChartCtrl.h"
#include "ChartCtrl/ChartLineSerie.h"
#include "afxcmn.h"

#include "CM5/ListCtrlEx.h"
// CGraphicMode dialog

class CGraphicMode : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphicMode)

public:
	CGraphicMode(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGraphicMode();

// Dialog Data
	enum { IDD = IDD_DIALOG_GRAPHIC_MODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ShowChecks();
	  void OnBnClickedUser();
	  void OnBnClickedDefault();

	virtual BOOL OnInitDialog();
public: 
	 
	void InitChartCtrl();
	void DrawerPoint();
    void Fresh_Graphic();
	void CalculateChartData();
	void Initial_UserList();
	void Initial_FactoryList(); 
private:
	CChartCtrl m_ChartCtrl;
	int m_xMax;
	int m_xMin;
	int m_yMax;
	int m_yMin;
	unsigned short m_Factory_Table_Value[20];
	BOOL m_Factory_Fre_First ;
	unsigned short m_User_Table_Value[20];
	BOOL m_User_Fre_First ;
	CButton m_check_user;
	CButton m_check_default;
    CChartXYSerie* pSeries[127];
public:
	ListCtrlEx::CListCtrlEx m_user_list;
	ListCtrlEx::CListCtrlEx m_factory_list;
    afx_msg void OnBnClickedCheckUser();
    afx_msg void OnBnClickedCheckDefault();
    afx_msg void OnBnClickedUserTableAdd();
    afx_msg void OnBnClickedUserTableDel();
    afx_msg LRESULT Fresh_Input_List(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT Change_Input_Item(WPARAM wParam,LPARAM lParam);
};
