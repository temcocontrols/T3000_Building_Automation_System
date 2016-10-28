#pragma once
#include "afxwin.h"
 #include "../SQLiteDriver/CppSQLite3.h"

// CLabelEditDlg dialog
#define START_ID 20000
// CLabelEditDlg dialog
#define INPUT_NUMBER 11

#define OUTPUT_NUMBER 7
//CString m_input[INPUT_NUMBER]={_T("Internal"),_T("Analog 1"),_T("Analog 2"),_T("Digital 1")};
//g_strSensorName
//	g_strInName1);
//g_strInName2);
//g_strInName3);
//g_strInName4);
//g_strInName5);
//g_strInName6);
//g_strInName7);
//g_strInName8); 
//g_strInHumName)
//	g_strInCO2);

class CLabelEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CLabelEditDlg)

public:
	CLabelEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLabelEditDlg();

// Dialog Data
	enum { IDD = IDD_LABELEDITDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnNewClickedOk();
	afx_msg void OnBnNewClickedCancel();

	CMFCColorButton m_textClorBtn;
	CMFCColorButton m_bkClrBtn;
	virtual BOOL OnInitDialog();
	void SaveEditValue();
	void AddLabel();
	void SetCommandType(int nType);
	void SaveToDb();
private:
	int m_nCommandType;//1:Add;2:Edit
	CppSQLite3DB m_SqliteDBBuilding;
	CppSQLite3Table m_table;
	CppSQLite3Query m_q;
 
public:
	void SetInitValueForEdit(int id,int serialNum,CString strScreen,int idControl,int in_out_put=0,int nWidth=75,int nHeight=25,int nStatus=0,int clrText=RGB(0, 0, 0),int bkClr=RGB(224, 232, 246));
	int m_nControlID;
	int m_id;
	int m_input_or_output;//1 == output ,0 == input;2:reg
	int m_nSerialNum;
	int m_nwidth;//the cstatic width;
	int m_nheight;//the cstatic height
	int m_nstatus;//for input or output selection,such as Relay 1 ,Relay 2,Relay 3...
	CString m_strScreenName;
	COLORREF m_clrTxt;
	COLORREF m_bkColor;
	CComboBox m_IOCombox;
	afx_msg void OnCbnSelchangeIocombox();
	CComboBox m_statusComBox;
	CEdit m_regEdit;
	afx_msg void OnCbnSelchangeStatuscombo();
	afx_msg void OnEnKillfocusRegedit();
	afx_msg void OnEnKillfocusWidthedit();
	afx_msg void OnEnKillfocusHeightedit();
	CEdit m_widthEdit;
	CEdit m_heightEdt;
	afx_msg void OnBnClickedExitbutton();
	afx_msg void OnBnClickedBkclorbtn();
	afx_msg void OnBnClickedTextbutton();
	CString m_input[INPUT_NUMBER];
	CString m_output[OUTPUT_NUMBER];
};
