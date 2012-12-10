#pragma once
#include "afxwin.h"


// CLabelEditDlg dialog

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
	_ConnectionPtr m_pCon;
	_RecordsetPtr m_pRs;
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
};
