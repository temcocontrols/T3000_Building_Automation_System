#pragma once


// CCustomSTable dialog

class CCustomSTable : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomSTable)

public:
	CCustomSTable(int Input_NO,CWnd* pParent = NULL);   // standard constructor
	virtual ~CCustomSTable();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_CSTABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocusEdit12();
	afx_msg void OnEnKillfocusEdit20();
	afx_msg void OnEnKillfocusEdit19();
	afx_msg void OnEnKillfocusEdit18();
	afx_msg void OnEnKillfocusEdit17();
	afx_msg void OnEnKillfocusEdit16();
	afx_msg void OnEnKillfocusEdit15();
	afx_msg void OnEnKillfocusEdit14();
	afx_msg void OnEnKillfocusEdit13();
	afx_msg void OnEnKillfocusEdit9();
	afx_msg void OnEnKillfocusEdit8();
	afx_msg void OnEnKillfocusEdit1();
	void to_fresh();
public:
int m_InputNo;
CString m_units_s;
int m_slider1_i;
int m_slider2_i;
int m_slider3_i;
int m_slider4_i;
int m_slider5_i;
int m_slider6_i;
int m_slider7_i;
int m_slider8_i;
int m_slider9_i;
int m_slider10_i;
int m_slider11_i;
int m_row1,m_row2;
afx_msg void OnBnClickedOk();
};
