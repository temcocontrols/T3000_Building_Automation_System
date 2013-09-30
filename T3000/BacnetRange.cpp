// BacnetRange.cpp : implementation file
//  Code by Fance

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
	, m_input_Analog_select(0)
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
	DDX_Radio(pDX, IDC_RADIO54, m_input_Analog_select);
}


BEGIN_MESSAGE_MAP(BacnetRange, CDialogEx)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_EDIT_RANGE_SELECT, &BacnetRange::OnEnKillfocusEditRangeSelect)
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
		GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->ShowWindow(0);
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
		for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
		{
			GetDlgItem(i)->ShowWindow(0);
		}
		MoveWindow(Temp_Rect.left,Temp_Rect.top,580,550);
	}
	else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) ||(bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE))
	{
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->ShowWindow(1);
		GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->ShowWindow(0);
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
		for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
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
		GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->ShowWindow(0);
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
		for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
		{
			GetDlgItem(i)->ShowWindow(0);
		}
		MoveWindow(Temp_Rect.left,Temp_Rect.top,400,400);
	}
	else if(bac_ranges_type == INPUT_RANGE_ANALOG_TYPE)
	{
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->ShowWindow(1);
		for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
		{
			GetDlgItem(i)->ShowWindow(false);
		}
		for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
		{
			GetDlgItem(i)->ShowWindow(0);
		}
		for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
		{
			GetDlgItem(i)->ShowWindow(false);
		}
		for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
		{
			GetDlgItem(i)->ShowWindow(true);
		}
		MoveWindow(Temp_Rect.left,Temp_Rect.top,490,480);
	}
	GetDlgItem(IDC_EDIT_RANGE_SELECT)->ShowWindow(1);
	GetDlgItem(IDC_STATIC_RANGE_ENTER_UNITS)->ShowWindow(1);

}


BOOL BacnetRange::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(GetFocus()==GetDlgItem(IDC_EDIT_RANGE_SELECT))
		{
		if(((pMsg->wParam >= 48) && (pMsg->wParam <=57)) 
			||(pMsg->wParam >= 96) && (pMsg->wParam <=105))
		{
			KillTimer(1);
			SetTimer(2,200,NULL);
		}
		}
		//if(pMsg->lParam)
	}
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

	KillTimer(1);
	KillTimer(2);
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
	int sel_value;
	CString temp_value;
	switch(nIDEvent)
	{
	case 1:
		{
			UpdateData();
			CString temp_cs;
			if(bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE)
			{
				bac_range_number_choose = m_analog_select;
			}
			else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) ||(bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE))
			{
				bac_range_number_choose = m_digital_select;
			}
			else if(bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE)
			{
				bac_range_number_choose = m_output_Analog_select;
			}
			else if(bac_ranges_type == INPUT_RANGE_ANALOG_TYPE)
			{
				bac_range_number_choose =	m_input_Analog_select;
			}
			temp_cs.Format(_T("%d"),bac_range_number_choose);
			GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_cs);
			((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetSel(0,-1);
		}
		break;
	case 2:
		
		
		GetDlgItemTextW(IDC_EDIT_RANGE_SELECT,temp_value);
		sel_value = _wtoi(temp_value);
		if(bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE)	//34
		{
			if(sel_value>=34)
			{
				MessageBox(_T("Please input a value between 0 - 33"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
				GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(_T(""));
				((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetSel(0,-1);
			}
			else
			{
				bac_range_number_choose = sel_value;
				m_analog_select = bac_range_number_choose ;
				UpdateData(false);
			}
		}
		else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) 
			||(bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE) 
			|| (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE))	//12
		{
			if(sel_value>=12)
			{
				MessageBox(_T("Please input a value between 0 - 11"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
				GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(_T(""));
				((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetSel(0,-1);
			}
			else
			{
				bac_range_number_choose = sel_value;
				m_digital_select = sel_value;
				UpdateData(false);
			}
		}
		else if(bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE)	//7
		{
			if(sel_value>=7)
			{
				MessageBox(_T("Please input a value between 0 - 6"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
				GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(_T(""));
				((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetSel(0,-1);
			}
			else
			{
				bac_range_number_choose = sel_value;
				m_output_Analog_select = sel_value;
				UpdateData(false);
			}
		}
		else if(bac_ranges_type == INPUT_RANGE_ANALOG_TYPE)	//19
		{
			if(sel_value>=19)
			{
				MessageBox(_T("Please input a value between 0 - 18"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
				GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(_T(""));
				((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetSel(0,-1);
			}
			else
			{
				bac_range_number_choose = sel_value;
				m_input_Analog_select = sel_value;
				UpdateData(false);
			}
		}
		break;
	default:
		break;
	}
	
	//GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetFocus();
	CDialogEx::OnTimer(nIDEvent);
}


void BacnetRange::OnEnKillfocusEditRangeSelect()
{
	// TODO: Add your control notification handler code here
	KillTimer(2);
	SetTimer(1,400,NULL);
}
