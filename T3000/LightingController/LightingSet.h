#pragma once
#include "afxwin.h"
#include "../LightingController/LightingController.h"
#include "../MainFrm.h"
#include <map>


// CLightingSet dialog

class CLightingSet : public CDialog
{
	DECLARE_DYNAMIC(CLightingSet)

public:
	CLightingSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightingSet();

// Dialog Data
	enum { IDD = IDD_DIALOG_LightCSeting };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CListBox m_List_Box;
	int m_outputboardsum;

	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonmodify();
	afx_msg void OnLbnSelchangeListLcset();

	typedef std::map<int,CString> MAP_OUTPUTADDRESS;
	MAP_OUTPUTADDRESS m_mapoutputaddress;

	int m_intaddress;
};
