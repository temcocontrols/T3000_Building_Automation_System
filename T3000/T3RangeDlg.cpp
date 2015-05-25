// T3RangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3RangeDlg.h"
#include "afxdialogex.h"


// CT3RangeDlg dialog

IMPLEMENT_DYNAMIC(CT3RangeDlg, CDialog)

	CT3RangeDlg::CT3RangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CT3RangeDlg::IDD, pParent)
	, m_current_range(0)
{
	
}

CT3RangeDlg::~CT3RangeDlg()
{
}

void CT3RangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RANGE_UNITE_SHOW, m_show_unit);
	DDX_Control(pDX, IDC_STATIC_SELECT_RANGE, m_rang_pic);
	DDX_Control(pDX, IDC_STATIC_RANGE_ENTER_UNITS, m_static_range_units_select);
	DDX_Text(pDX, IDC_EDIT_RANGE_SELECT, m_current_range);
}


BEGIN_MESSAGE_MAP(CT3RangeDlg, CDialog)
END_MESSAGE_MAP()


// CT3RangeDlg message handlers


BOOL CT3RangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_show_unit.ShowWindow(FALSE);
	m_show_unit.SetWindowTextW(_T(""));
	m_show_unit.textColor(RGB(255,0,0));
	m_show_unit.setFont(24,12,NULL,_T("Arial"));
	m_input_Analog_select=m_current_range;
	Initial_window();
	//SetTimer(1,400,NULL);
	((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CT3RangeDlg::Initial_window(){
	CString temp_cs;
	m_show_unit.ShowWindow(TRUE);
	temp_cs.Format(_T("%d"),m_input_Analog_select);
	((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetWindowTextW(temp_cs);
	for (int i=IDC_RADIO_T_0;i<=IDC_RADIO_T_13;i++)
	{
		((CButton *)GetDlgItem(i))->SetCheck(0);
	}
	((CButton *)GetDlgItem(IDC_RADIO_T_0+m_input_Analog_select))->SetCheck(1);

	m_show_unit.SetWindowTextW(Range_Units[m_input_Analog_select]);
	CRect c13;
	GetDlgItem(IDC_RADIO_T_0 + m_input_Analog_select)->GetWindowRect(c13);   //获取控件的位置 ，饼调整位置;
	ScreenToClient(c13);  
	m_rang_pic.SetWindowPos(NULL,c13.left - 40,c13.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_rang_pic.Invalidate(TRUE);
}
