// BacnetRange.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetRange.h"
#include "afxdialogex.h"
#include "gloab_define.h"

// BacnetRange dialog

IMPLEMENT_DYNAMIC(BacnetRange, CDialogEx)

BacnetRange::BacnetRange(CWnd* pParent /*=NULL*/)
	: CDialogEx(BacnetRange::IDD, pParent)
	, m_analog_select(0)
	, m_digital_select(0)
	, m_output_Analog_select(0)
{

}

BacnetRange::~BacnetRange()
{
}

void BacnetRange::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_STATIC_RANGE_TITLE, m_static_range_title);
	//	DDX_Control(pDX, IDC_STATIC_RANGES, m_static_range_detail);
	DDX_Control(pDX, IDC_STATIC_RANGE_ENTER_UNITS, m_static_range_units_select);
	DDX_Radio(pDX, IDC_RADIO1, m_analog_select);
	DDX_Radio(pDX, IDC_RADIO35, m_digital_select);
	DDX_Radio(pDX, IDC_RADIO47, m_output_Analog_select);
}


BEGIN_MESSAGE_MAP(BacnetRange, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// BacnetRange message handlers


BOOL BacnetRange::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Initial_static();
	// TODO:  Add extra initialization here
	SetTimer(1,400,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void BacnetRange::Initial_static()
{
	CRect Temp_Rect;;
	GetWindowRect(Temp_Rect);
	if(bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE)
	{
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->ShowWindow(0);
		for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
		{
				GetDlgItem(i)->ShowWindow(true);
		}
		for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
		{
			GetDlgItem(i)->ShowWindow(false);
		}
		for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
		{
			GetDlgItem(i)->ShowWindow(0);
		}


		MoveWindow(Temp_Rect.left,Temp_Rect.top,580,550);
	}
	else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) ||(bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE))
	{
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->ShowWindow(1);
		for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
		{
			GetDlgItem(i)->ShowWindow(true);
		}
		for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
		{
			GetDlgItem(i)->ShowWindow(false);
		}
		for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
		{
			GetDlgItem(i)->ShowWindow(0);
		}
		MoveWindow(Temp_Rect.left,Temp_Rect.top,600,300);
	}
	else if(bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE)
	{
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->ShowWindow(0);
		for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
		{
			GetDlgItem(i)->ShowWindow(true);
		}
		for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
		{
			GetDlgItem(i)->ShowWindow(0);
		}
		for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
		{
			GetDlgItem(i)->ShowWindow(false);
		}
		MoveWindow(Temp_Rect.left,Temp_Rect.top,400,400);
	}
	GetDlgItem(IDC_EDIT_RANGE_SELECT)->ShowWindow(1);
	GetDlgItem(IDC_STATIC_RANGE_ENTER_UNITS)->ShowWindow(1);
	//CString temp_cs = _T("\r\n");
	//
	//m_static_range_title.textColor(RGB(255,255,255));
	//m_static_range_title.bkColor(RGB(0,0,255));
	//m_static_range_title.setFont(28,20,NULL,_T("Arial"));


#if 0
	if(bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE)
	{
		m_static_range_title.SetWindowTextW(Range_Type[0]);
		
		CString temp_2;
		for (int i=0;i<(int)sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]);i++)
		{
			temp_2.Format(_T("%-2d. %-16s"),i,Variable_Analog_Units_Array[i]);
			temp_cs = temp_cs + temp_2;
			if(i%4==3)
				temp_cs = temp_cs + _T("\n");
		}
	}
	else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) ||(bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE))
	{
		m_static_range_title.SetWindowTextW(Range_Type[1]);
		CString temp_3;
		temp_3.Format(_T("%2d    %-18s"),0,Digital_Units_Array[0]);
		temp_cs = temp_cs + temp_3;
		for (int i=1;i<(int)sizeof(Digital_Units_Array)/sizeof(Digital_Units_Array[0]);i++)
		{
			temp_3.Format(_T("%2d/%2d %-18s"),i,i+11,Digital_Units_Array[i]);
			temp_cs = temp_cs + temp_3;
			if(i%3==2)
				temp_cs = temp_cs + _T("\n\n");
		}
	}
	else if(bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE)
	{
		m_static_range_title.SetWindowTextW(Range_Type[0]);
		CString temp_4;
		for (int i=0;i<(int)sizeof(Output_Analog_Units_Array)/sizeof(Output_Analog_Units_Array[0]);i++)
		{
			temp_4.Format(_T("%d. %-20s"),i,Output_Analog_Units_Array[i]);
			temp_cs = temp_cs + temp_4;
			if(i%2==1)
				temp_cs = temp_cs + _T("\n\n");
		}

	}
	
	m_static_range_detail.SetWindowTextW(temp_cs);
	m_static_range_detail.textColor(RGB(0,0,0));
	m_static_range_detail.bkColor(RGB(0,255,255));
	m_static_range_detail.setFont(18,11,NULL,_T("SimSun-ExtB"));

	m_static_range_units_select.textColor(RGB(0,0,255));	
#endif
}


BOOL BacnetRange::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}


void BacnetRange::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	CString temp;
	GetDlgItemText(IDC_EDIT_RANGE_SELECT,temp);
	if(!temp.IsEmpty())
		bac_range_number_choose = _wtoi(temp);
	else
		bac_range_number_choose = 0;
	CDialogEx::OnOK();
}


void BacnetRange::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	bac_range_number_choose = 0;
	CDialogEx::OnCancel();
}


void BacnetRange::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	UpdateData();
	CString temp_cs;
	if(bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE)
	{
		bac_range_number_choose = m_analog_select;
	}
	else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) ||(bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE))
	{
		bac_range_number_choose = m_digital_select;
	}
	else if(bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE)
	{
		bac_range_number_choose = m_output_Analog_select;
	}
	temp_cs.Format(_T("%d"),bac_range_number_choose);
	GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_cs);
	//GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetFocus();
	CDialogEx::OnTimer(nIDEvent);
}
