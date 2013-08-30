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
{

}

BacnetRange::~BacnetRange()
{
}

void BacnetRange::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RANGE_TITLE, m_static_range_title);
	DDX_Control(pDX, IDC_STATIC_RANGES, m_static_range_detail);
	DDX_Control(pDX, IDC_STATIC_RANGE_ENTER_UNITS, m_static_range_units_select);
}


BEGIN_MESSAGE_MAP(BacnetRange, CDialogEx)
END_MESSAGE_MAP()


// BacnetRange message handlers


BOOL BacnetRange::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Initial_static();
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void BacnetRange::Initial_static()
{
	CString temp_cs = _T("\r\n");
	
	m_static_range_title.textColor(RGB(255,255,255));
	m_static_range_title.bkColor(RGB(0,0,255));
	m_static_range_title.setFont(28,20,NULL,_T("Arial"));
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
