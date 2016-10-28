#pragma once
#include "afxwin.h"
#include "msflexgrid1.h"

// ApplyGraphicLabelsDlg dialog

class ApplyGraphicLabelsDlg : public CDialog
{
	DECLARE_DYNAMIC(ApplyGraphicLabelsDlg)

public:
	ApplyGraphicLabelsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ApplyGraphicLabelsDlg();

// Dialog Data
	enum { IDD = IDD_APPLYGRAPHIC_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void OnUnSelect();
	void OnSelect();
	void OnSelectAll();
	vector<Label_information> m_labelList;
	void SetLabelLst(vector<Label_information>* pLabelLst);
public:
 
	int m_nCurRow;
	int m_nCurCol;
	CMsflexgrid m_FlexGrid;
	CComboBox m_subNetList;
	CString m_strMainBuilding;
	CString m_strSubNetName;

	CString m_strID;
	int m_nTotalRecoders;


	afx_msg void OnBnClickedOk();
	DECLARE_EVENTSINK_MAP()
	void ClickAddbuildingMsflexgrid();
	void SetBuilding(CString strMainBuilding,CString strSubNetName);
	afx_msg void OnCbnSelchangeCombo1();
	void ReloadAddBuildingDB();
	void ApplyTo();

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
