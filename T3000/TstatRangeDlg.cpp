// TstatRangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TstatRangeDlg.h"
#include "afxdialogex.h"


// CTstatRangeDlg dialog

IMPLEMENT_DYNAMIC(CTstatRangeDlg, CDialogEx)

CTstatRangeDlg::CTstatRangeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTstatRangeDlg::IDD, pParent)
	, m_input_Analog_select(0)
	, m_current_range(0)
{

}

CTstatRangeDlg::~CTstatRangeDlg()
{
  
}

void CTstatRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RANGE_UNITE_SHOW, m_show_unit);
	DDX_Control(pDX, IDC_STATIC_SELECT_RANGE, m_rang_pic);
	DDX_Control(pDX, IDC_STATIC_RANGE_ENTER_UNITS, m_static_range_units_select);
	//DDX_Radio(pDX, IDC_RADIO_T_0, m_input_Analog_select);
	DDX_Text(pDX, IDC_EDIT_RANGE_SELECT, m_current_range);
	DDV_MinMaxInt(pDX, m_current_range, 0, 12);
}


BEGIN_MESSAGE_MAP(CTstatRangeDlg, CDialogEx)

	 
	ON_WM_TIMER()
	ON_EN_SETFOCUS(IDC_EDIT_RANGE_SELECT, &CTstatRangeDlg::OnEnSetfocusEditRangeSelect)
	ON_BN_CLICKED(IDOK, &CTstatRangeDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_T_0, &CTstatRangeDlg::OnBnClickedRadioT0)
	ON_BN_CLICKED(IDC_RADIO_T_1, &CTstatRangeDlg::OnBnClickedRadioT1)
	ON_BN_CLICKED(IDC_RADIO_T_2, &CTstatRangeDlg::OnBnClickedRadioT2)
	ON_BN_CLICKED(IDC_RADIO_T_3, &CTstatRangeDlg::OnBnClickedRadioT3)
	ON_BN_CLICKED(IDC_RADIO_T_4, &CTstatRangeDlg::OnBnClickedRadioT4)
	ON_BN_CLICKED(IDC_RADIO_T_5, &CTstatRangeDlg::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO_T_6, &CTstatRangeDlg::OnBnClickedRadioT6)
	ON_BN_CLICKED(IDC_RADIO_T_7, &CTstatRangeDlg::OnBnClickedRadioT7)
	ON_BN_CLICKED(IDC_RADIO_T_8, &CTstatRangeDlg::OnBnClickedRadioT8)
	ON_BN_CLICKED(IDC_RADIO_T_9, &CTstatRangeDlg::OnBnClickedRadioT9)
	ON_BN_CLICKED(IDC_RADIO_T_10, &CTstatRangeDlg::OnBnClickedRadioT10)
	ON_BN_CLICKED(IDC_RADIO_T_11, &CTstatRangeDlg::OnBnClickedRadioT11)
END_MESSAGE_MAP()


// CTstatRangeDlg message handlers


BOOL CTstatRangeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_show_unit.ShowWindow(FALSE);
	m_show_unit.SetWindowTextW(_T(""));
	m_show_unit.textColor(RGB(255,0,0));
	m_show_unit.setFont(24,12,NULL,_T("Arial"));
	m_input_Analog_select=m_current_range;
	Initial_window();
	//SetTimer(1,400,NULL);
	((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTstatRangeDlg::Initial_window(){
     
	CString temp_cs;
	m_show_unit.ShowWindow(TRUE);
	temp_cs.Format(_T("%d"),m_input_Analog_select);
	((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetWindowTextW(temp_cs);
	 for (int i=IDC_RADIO_T_0;i<=IDC_RADIO_T_11;i++)
	 {
	    ((CButton *)GetDlgItem(i))->SetCheck(0);
	 }
	  ((CButton *)GetDlgItem(IDC_RADIO_T_0+m_input_Analog_select))->SetCheck(1);
	 
	  m_show_unit.SetWindowTextW(analog_range_TSTAT6[m_input_Analog_select]);
	  CRect c13;
	  GetDlgItem(IDC_RADIO_T_0 + m_input_Analog_select)->GetWindowRect(c13);   //获取控件的位置 ，饼调整位置;
	  ScreenToClient(c13);  
	  m_rang_pic.SetWindowPos(NULL,c13.left - 40,c13.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
	  m_rang_pic.Invalidate(TRUE);

}
BOOL CTstatRangeDlg::PreTranslateMessage(MSG* pMsg)
{
	//CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RANGE_SELECT);

	//ASSERT(pEdit);

	//if(pMsg->hwnd == pEdit->GetSafeHwnd() && VK_RETURN == pMsg->wParam)
	//{
	//  SetTimer(1,400,NULL);
 //   }
// 	if(pMsg->message == WM_KEYDOWN)
// 	{
// 		if(GetFocus()==GetDlgItem(IDC_EDIT_RANGE_SELECT))
// 		{
// 			if(((pMsg->wParam >= 48) && (pMsg->wParam <=57)) 
// 				||(pMsg->wParam >= 96) && (pMsg->wParam <=105))
// 			{
// 				
// 			}
// 		}
// 	 
// 		
// 	}



	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if(GetFocus()==GetDlgItem(IDC_EDIT_RANGE_SELECT))
			{
				CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
				GetDlgItem(IDC_EDIT1_TEST)->SetFocus();
				temp_focus->SetFocus();
			}



			return 1;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CTstatRangeDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if (message==WM_TSTAT_RANGE)
	{
	 
	   Initial_window();
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CTstatRangeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	int sel_value;
	CString temp_value;
	switch(nIDEvent)
	{
	case 1:{
	UpdateData();
	int nfocusid = 0;

	if(GetFocus())
	{
		nfocusid = GetFocus()->GetDlgCtrlID();
	}
	else
		break;
	if((nfocusid >= IDC_RADIO_T_0) && (nfocusid <= IDC_RADIO_T_11))
	{
		//bac_ranges_type = INPUT_RANGE_DIGITAL_TYPE;
	}
	else if(nfocusid == IDC_EDIT_RANGE_SELECT)
	{
		Sleep(1);
	}
	else
	{
		break;
	}

	m_current_range =	m_input_Analog_select;
	m_show_unit.SetWindowTextW(analog_range_TSTAT6[m_current_range]);
	if(nfocusid == IDC_EDIT_RANGE_SELECT)
	{
		CRect c2;
		GetDlgItem(IDC_RADIO_T_0 + m_current_range)->GetWindowRect(c2);   //获取控件的位置 ，饼调整位置;
		ScreenToClient(c2);  
		m_rang_pic.SetWindowPos(NULL,c2.left - 40,c2.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
		m_rang_pic.Invalidate(TRUE);
	}

	temp_value.Format(_T("%d"),m_current_range);
	GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_value);
	((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetSel(0,-1);


	if(nfocusid != IDC_EDIT_RANGE_SELECT)
	{
		CRect c1;
		GetFocus()->GetWindowRect(c1);   //获取控件的位置 ，饼调整位置;
		ScreenToClient(c1);  
		m_rang_pic.SetWindowPos(NULL,c1.left - 40,c1.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
		m_rang_pic.Invalidate(TRUE);
	}
//	PostMessage(WM_TSTAT_RANGE,0,0);
	}break;
	case 2:{
		GetDlgItemTextW(IDC_EDIT_RANGE_SELECT,temp_value);
		sel_value = _wtoi(temp_value);
		if(sel_value>12)
		{
			MessageBox(_T("Please input a value between 0 - 10"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(_T(""));
			((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetSel(0,-1);
			((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetFocus();
		}
		else
		{
			 m_current_range = sel_value;
			m_input_Analog_select = sel_value;
			//m_show_unit.SetWindowTextW(Input_Analog_Units_Array[bac_range_number_choose]);
			////CRect c13;
			////GetDlgItem(IDC_RADIO_T_0 + m_input_Analog_select)->GetWindowRect(c13);   //获取控件的位置 ，饼调整位置;
			////ScreenToClient(c13);  
			////m_rang_pic.SetWindowPos(NULL,c13.left - 40,c13.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
			////m_rang_pic.Invalidate(TRUE);
			//PostMessage(WM_TSTAT_RANGE,0,0);
			//UpdateData(false);

			m_show_unit.SetWindowTextW(analog_range_TSTAT6[m_current_range]);
			CRect c13;
			GetDlgItem(IDC_RADIO_T_0 + m_input_Analog_select)->GetWindowRect(c13);   //获取控件的位置 ，饼调整位置;
			ScreenToClient(c13);  
			m_rang_pic.SetWindowPos(NULL,c13.left - 40,c13.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
			m_rang_pic.Invalidate(TRUE);

			UpdateData(false);

		}
// 		((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetSel(0,-1);
// 		((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetFocus();
  //		KillTimer(2);
// 		SetTimer(1,400,NULL);
	}break;
	default:break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CTstatRangeDlg::OnEnSetfocusEditRangeSelect()
{
   UpdateData();
   m_input_Analog_select = m_current_range;
   PostMessage(WM_TSTAT_RANGE,0,0);
}
void CTstatRangeDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
 
	CString temp;
	GetDlgItemText(IDC_EDIT_RANGE_SELECT,temp);
	if(!temp.IsEmpty())
		bac_range_number_choose = _wtoi(temp);
 

 
	CDialogEx::OnOK();
}
void CTstatRangeDlg::OnCancel()
{
 
	CDialogEx::OnCancel();
}
void CTstatRangeDlg::Click_Radio(){
	  m_input_Analog_select = 0;
	for (int i=IDC_RADIO_T_0;i<=IDC_RADIO_T_11;i++,m_input_Analog_select++)
	{
		if (((CButton *)GetDlgItem(i))->GetCheck()==1)
		{
			break;
		}

	}
	 
	m_current_range=m_input_Analog_select;
	PostMessage(WM_TSTAT_RANGE,0,0);
}
void CTstatRangeDlg::OnBnClickedOk()
{
 OnOK();
}


void CTstatRangeDlg::OnBnClickedRadioT0()
{
	 Click_Radio();
}


void CTstatRangeDlg::OnBnClickedRadioT1()
{
	 Click_Radio();
}


void CTstatRangeDlg::OnBnClickedRadioT2()
{
	 Click_Radio();
}


void CTstatRangeDlg::OnBnClickedRadioT3()
{
	 Click_Radio();
}


void CTstatRangeDlg::OnBnClickedRadioT4()
{
Click_Radio();
	// TODO: Add your control notification handler code here
}


void CTstatRangeDlg::OnBnClickedRadio()
{
	Click_Radio();// TODO: Add your control notification handler code here
}


void CTstatRangeDlg::OnBnClickedRadioT6()
{
	Click_Radio();// TODO: Add your control notification handler code here
}


void CTstatRangeDlg::OnBnClickedRadioT7()
{
	Click_Radio();	// TODO: Add your control notification handler code here
}


void CTstatRangeDlg::OnBnClickedRadioT8()
{
	Click_Radio();	// TODO: Add your control notification handler code here
}


void CTstatRangeDlg::OnBnClickedRadioT9()
{
	Click_Radio();	// TODO: Add your control notification handler code here
}


void CTstatRangeDlg::OnBnClickedRadioT10()
{
	Click_Radio();	// TODO: Add your control notification handler code here
}

void CTstatRangeDlg::OnBnClickedRadioT11()
{
	Click_Radio();	// TODO: Add your control notification handler code here
}