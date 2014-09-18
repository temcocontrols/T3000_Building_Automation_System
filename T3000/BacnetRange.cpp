// BacnetRange.cpp : implementation file
//  Code by Fance

#include "stdafx.h"
#include "T3000.h"
#include "BacnetRange.h"
#include "afxdialogex.h"
#include "gloab_define.h"

int old_bac_range_number_choose = 0;

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
	DDX_Control(pDX, IDC_STATIC_RANGE_UNITE_SHOW, m_show_unit);
}


BEGIN_MESSAGE_MAP(BacnetRange, CDialogEx)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_EDIT_RANGE_SELECT, &BacnetRange::OnEnKillfocusEditRangeSelect)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &BacnetRange::OnBnClickedCancel)
END_MESSAGE_MAP()


// BacnetRange message handlers


BOOL BacnetRange::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_show_unit.ShowWindow(FALSE);
	m_show_unit.SetWindowTextW(_T(""));
	m_show_unit.textColor(RGB(255,0,0));
	m_show_unit.setFont(24,12,NULL,_T("Arial"));
	Initial_static();
	// TODO:  Add extra initialization here
	SetTimer(1,400,NULL);
	((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetFocus();
	return FALSE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void BacnetRange::Initial_static()
{
	CString temp_cs;
	CRect Temp_Rect;
	GetWindowRect(Temp_Rect);

	if(receive_customer_unit)	//如果接受到了 customer unit 的回复;
	{

		for(int i=0 ;i < BAC_CUSTOMER_UNITS_COUNT ; i++)
		{
			MultiByteToWideChar( CP_ACP, 0, (char *)m_customer_unit_data.at(i).digital_units_off, (int)strlen((char *)m_customer_unit_data.at(i).digital_units_off)+1, 
				temp_off[i].GetBuffer(MAX_PATH), MAX_PATH );
			temp_off[i].ReleaseBuffer();
			if(temp_off[i].GetLength() >= 12)
				temp_off[i].Empty();

			MultiByteToWideChar( CP_ACP, 0, (char *)m_customer_unit_data.at(i).digital_units_on, (int)strlen((char *)m_customer_unit_data.at(i).digital_units_on)+1, 
				temp_on[i].GetBuffer(MAX_PATH), MAX_PATH );
			temp_on[i].ReleaseBuffer();
			if(temp_on[i].GetLength() >= 12)
				temp_on[i].Empty();

			CString unit_index;
			unit_index.Format(_T("%d.     "),i+23);

			temp_unit[i] =unit_index + temp_off[i] + _T("/") + temp_on[i];
			temp_unit_no_index[i] = temp_off[i] + _T("/") + temp_on[i];
		}

		for(int j=0;j<=7 ; j++)
		{
			GetDlgItem(IDC_RADIO73 + j)->SetWindowTextW(temp_unit[j]);
		}

		
		GetDlgItem(IDC_RADIO73)->ShowWindow(true);
		GetDlgItem(IDC_RADIO74)->ShowWindow(true);
		GetDlgItem(IDC_RADIO75)->ShowWindow(true);
		GetDlgItem(IDC_RADIO76)->ShowWindow(true);
		GetDlgItem(IDC_RADIO77)->ShowWindow(true);
		GetDlgItem(IDC_RADIO78)->ShowWindow(true);
		GetDlgItem(IDC_RADIO79)->ShowWindow(true);
		GetDlgItem(IDC_RADIO80)->ShowWindow(true);
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->ShowWindow(true);
		
	}
	else
	{
		GetDlgItem(IDC_RADIO73)->ShowWindow(false);
		GetDlgItem(IDC_RADIO74)->ShowWindow(false);
		GetDlgItem(IDC_RADIO75)->ShowWindow(false);
		GetDlgItem(IDC_RADIO76)->ShowWindow(false);
		GetDlgItem(IDC_RADIO77)->ShowWindow(false);
		GetDlgItem(IDC_RADIO78)->ShowWindow(false);
		GetDlgItem(IDC_RADIO79)->ShowWindow(false);
		GetDlgItem(IDC_RADIO80)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->ShowWindow(false);
	}

	if((bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE) || (bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE))
	{
		m_show_unit.ShowWindow(TRUE);
		temp_cs.Format(_T("%d"),bac_range_number_choose);
		GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_cs);
		m_analog_select = bac_range_number_choose;
		for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
		{
				//GetDlgItem(i)->ShowWindow(true);
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //获取控件的位置 ，饼调整位置;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left - 420,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

			GetDlgItem(i)->ShowWindow(true);
		}
		for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
		{
			//GetDlgItem(i)->ShowWindow(false);
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //获取控件的位置 ，饼调整位置;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 30,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
			GetDlgItem(i)->ShowWindow(true);
		}

		for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //获取控件的位置 ，饼调整位置;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 30,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
			GetDlgItem(i)->ShowWindow(true);
		}

		CRect c1; 
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->GetWindowRect(c1);   //获取控件的位置 ，饼调整位置;
		ScreenToClient(c1);  
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->SetWindowPos(NULL,c1.left + 30,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->ShowWindow(true);


		for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
		{
			//GetDlgItem(i)->ShowWindow(0);
			GetDlgItem(i)->ShowWindow(false);
		}
		for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
		{
			//GetDlgItem(i)->ShowWindow(0);
			GetDlgItem(i)->ShowWindow(false);
		}

		CRect c3;
		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->GetWindowRect(c3);   
		ScreenToClient(c3);  
		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->SetWindowPos(NULL,c3.left - 420,c3.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

		CRect c2; 
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->GetWindowRect(c2);   
		ScreenToClient(c2);  
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->SetWindowPos(NULL,c2.left + 25,c2.top,0,0,SWP_NOZORDER|SWP_NOSIZE);



		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(true);//variable
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(false);//output
		GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->ShowWindow(false);//input
		MoveWindow(Temp_Rect.left,Temp_Rect.top,550,780);
		//MoveWindow(Temp_Rect.left,Temp_Rect.top,580,550);
	}
	else if((bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE) || (bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE))
	{
		m_show_unit.ShowWindow(TRUE);
		temp_cs.Format(_T("%d"),bac_range_number_choose);
		GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_cs);
		m_output_Analog_select = bac_range_number_choose;

		for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
		{
			//GetDlgItem(i)->ShowWindow(true);
			CRect c2; 
			GetDlgItem(i)->GetWindowRect(c2);   //获取控件的位置 ，饼调整位置;
			ScreenToClient(c2);  
			GetDlgItem(i)->SetWindowPos(NULL,c2.left - 400,c2.top - 370,0,0,SWP_NOZORDER|SWP_NOSIZE);

			GetDlgItem(i)->ShowWindow(true);
		}
		for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
		{
			//GetDlgItem(i)->ShowWindow(0);
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //获取控件的位置 ，饼调整位置;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top - 170,0,0,SWP_NOZORDER|SWP_NOSIZE);
			GetDlgItem(i)->ShowWindow(true);
		}
		for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //获取控件的位置 ，饼调整位置;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top - 170,0,0,SWP_NOZORDER|SWP_NOSIZE);
			GetDlgItem(i)->ShowWindow(true);
		}
		CRect c1; 
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->GetWindowRect(c1);   //获取控件的位置 ，饼调整位置;
		ScreenToClient(c1);  
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->SetWindowPos(NULL,c1.left + 50,c1.top - 170,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->ShowWindow(true);


		for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
		{
			//GetDlgItem(i)->ShowWindow(false);
			GetDlgItem(i)->ShowWindow(false);
		}
		for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
		{
			//GetDlgItem(i)->ShowWindow(0);
			GetDlgItem(i)->ShowWindow(false);
		}
		CRect c3;
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->GetWindowRect(c3);   
		ScreenToClient(c3);  
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->SetWindowPos(NULL,c3.left - 400,c3.top - 370,0,0,SWP_NOZORDER|SWP_NOSIZE);

		CRect c2; 
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->GetWindowRect(c2);   
		ScreenToClient(c2);  
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->SetWindowPos(NULL,c2.left + 45,c2.top - 170,0,0,SWP_NOZORDER|SWP_NOSIZE);

		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(false);//variable
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(true);//output
		GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->ShowWindow(false);//input
		MoveWindow(Temp_Rect.left,Temp_Rect.top,550,610);
		//MoveWindow(Temp_Rect.left,Temp_Rect.top,400,400);
	}
	else if((bac_ranges_type == INPUT_RANGE_ANALOG_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE))
	{
		m_show_unit.ShowWindow(TRUE);
		temp_cs.Format(_T("%d"),bac_range_number_choose);
		((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetWindowTextW(temp_cs);
		m_input_Analog_select = bac_range_number_choose;

		for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
		{
			//GetDlgItem(i)->ShowWindow(false);
			((CButton *)GetDlgItem(i))->ShowWindow(false);
		}
		for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
		{
			GetDlgItem(i)->ShowWindow(0);
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //获取控件的位置 ，饼调整位置;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
			((CButton *)GetDlgItem(i))->ShowWindow(1);

		}

		for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //获取控件的位置 ，饼调整位置;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
			GetDlgItem(i)->ShowWindow(true);
		}

		CRect c1; 
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->GetWindowRect(c1);   //获取控件的位置 ，饼调整位置;
		ScreenToClient(c1);  
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->SetWindowPos(NULL,c1.left + 50,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->ShowWindow(true);

		for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
		{
			//GetDlgItem(i)->ShowWindow(false);
			((CButton *)GetDlgItem(i))->ShowWindow(false);
		}
		for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //获取控件的位置 ，饼调整位置;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

			((CButton *)GetDlgItem(i))->ShowWindow(1);
		}

		CRect c2; 
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->GetWindowRect(c2);   
		ScreenToClient(c2);  
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->SetWindowPos(NULL,c2.left + 50,c2.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

		GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->GetWindowRect(c2);   
		ScreenToClient(c2);  
		GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->SetWindowPos(NULL,c2.left + 50,c2.top,0,0,SWP_NOZORDER|SWP_NOSIZE);


		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(false);//variable
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(false);//output
		GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->ShowWindow(true);//input
		//new player,
		//MoveWindow(Temp_Rect.left,Temp_Rect.top,490,480);
		MoveWindow(Temp_Rect.left,Temp_Rect.top,580,780);
	}





	if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) ||(bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE))
	{
		if(bac_range_number_choose > 23)
			bac_range_number_choose = 0;

		m_show_unit.ShowWindow(TRUE);
		temp_cs.Format(_T("%d"),bac_range_number_choose);
		GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_cs);

		m_show_unit.SetWindowTextW(Digital_Units_Array[bac_range_number_choose]);

		if(bac_range_number_choose<=11)
			m_digital_select = bac_range_number_choose ;
		else
			m_digital_select = bac_range_number_choose - 11;
		old_bac_range_number_choose = m_digital_select;

		for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
		{
			//GetDlgItem(i)->ShowWindow(true);
			GetDlgItem(i)->ShowWindow(1);
		}
		if(bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE)
		{
			for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
			{
				//GetDlgItem(i)->ShowWindow(false);
				GetDlgItem(i)->ShowWindow(1);
			}	
			GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(true);//variable
			GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(false);//output
			GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->ShowWindow(false);//input
		}
		else
		{
			for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
			{
				//GetDlgItem(i)->ShowWindow(false);
				GetDlgItem(i)->ShowWindow(0);
			}	
		}

		if(bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE)
		{
			for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
			{
				GetDlgItem(i)->ShowWindow(1);
			}
			GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(false);//variable
			GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(true);//output
			GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->ShowWindow(false);//input
		}
		else
		{
			for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
			{
				GetDlgItem(i)->ShowWindow(0);
			}
		}

		if(bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE)
		{
			for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
			{
				GetDlgItem(i)->ShowWindow(1);
			}
			GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(false);//variable
			GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(false);//output
			GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->ShowWindow(true);//input
		}
		else
		{
			for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
			{
				GetDlgItem(i)->ShowWindow(0);
			}
		}

		//MoveWindow(Temp_Rect.left,Temp_Rect.top,600,300);
	}

	GetDlgItem(IDC_EDIT_RANGE_SELECT)->EnableWindow(1);
	GetDlgItem(IDC_EDIT_RANGE_SELECT)->ShowWindow(1);
	GetDlgItem(IDC_STATIC_RANGE_ENTER_UNITS)->ShowWindow(1);
	UpdateData(FALSE);
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
	range_cancel = false;
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
	range_cancel = true;
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
			int nfocusid = 0;
			
			if(GetFocus())
			{
				nfocusid = GetFocus()->GetDlgCtrlID();
			}
			else
				break;
			
			if((nfocusid >= IDC_RADIO54) && (nfocusid <= IDC_RADIO72))
			{
				bac_ranges_type = INPUT_RANGE_ANALOG_TYPE;
			}
			else if((nfocusid >= IDC_RADIO1) && (nfocusid <= IDC_RADIO34))
			{
				bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
			}
			else if((nfocusid >= IDC_RADIO47) && (nfocusid <= IDC_RADIO53))
			{
				bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
			}
			else if((nfocusid >= IDC_RADIO35) && (nfocusid <= IDC_RADIO46))
			{
					bac_ranges_type = INPUT_RANGE_DIGITAL_TYPE;
			}
			else if((nfocusid >= IDC_RADIO73) && (nfocusid <= IDC_RADIO80))
			{
				bac_ranges_type = INPUT_RANGE_DIGITAL_TYPE;
			}
			else if(nfocusid == IDC_EDIT_RANGE_SELECT)
			{

			}
			else
			{
				break;
			}
			CString temp_cs;
			if(bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE)
			{
				bac_range_number_choose = m_analog_select;
				m_show_unit.SetWindowTextW(Variable_Analog_Units_Array[bac_range_number_choose]);
			}
			else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) ||(bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE))
			{
				if(m_digital_select != old_bac_range_number_choose)
				{
					//TRACE(_T("%d\r\n"),m_digital_select);
					if(m_digital_select >11)
					{
						bac_range_number_choose = m_digital_select - 11 + 22;
					}
					else
					{
						bac_range_number_choose = m_digital_select;
					}
					
					if((bac_range_number_choose >= 23) && (bac_range_number_choose <=30))
					{
						m_show_unit.SetWindowTextW(temp_unit_no_index[bac_range_number_choose - 23]);
					}
					else
						m_show_unit.SetWindowTextW(Digital_Units_Array[bac_range_number_choose]);
				}
			}
			else if(bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE)
			{
				bac_range_number_choose = m_output_Analog_select;
				m_show_unit.SetWindowTextW(Output_Analog_Units_Array[bac_range_number_choose]);
			}
			else if(bac_ranges_type == INPUT_RANGE_ANALOG_TYPE)
			{
				bac_range_number_choose =	m_input_Analog_select;
				m_show_unit.SetWindowTextW(Input_Analog_Units_Array[bac_range_number_choose]);
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
				((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetFocus();
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
			if(sel_value>=31)
			{
				MessageBox(_T("Please input a value between 0 - 30"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
				GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(_T(""));
				((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetSel(0,-1);
				((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetFocus();
			}
			else
			{
				if(sel_value <= 22)
				{
					bac_range_number_choose = sel_value;
					m_show_unit.SetWindowTextW(Digital_Units_Array[bac_range_number_choose]);
					if(sel_value < 11)
						m_digital_select = sel_value;
					else
						m_digital_select = sel_value - 11;
				}
				else
				{
					bac_range_number_choose = sel_value;
					m_digital_select = sel_value - 11;
					CString temp_cs;
					temp_cs = temp_off[bac_range_number_choose - 23] + _T("/") + temp_on[bac_range_number_choose - 23];
					m_show_unit.SetWindowTextW(temp_cs);
				}

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
				((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetFocus();
			}
			else
			{
				bac_range_number_choose = sel_value;
				m_output_Analog_select = sel_value;
				m_show_unit.SetWindowTextW(Output_Analog_Units_Show[bac_range_number_choose]);
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
				((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetFocus();
			}
			else
			{
				bac_range_number_choose = sel_value;
				m_input_Analog_select = sel_value;
				m_show_unit.SetWindowTextW(Input_Analog_Units_Array[bac_range_number_choose]);
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


void BacnetRange::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	range_cancel = true;
	CDialogEx::OnClose();
}


void BacnetRange::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	range_cancel = true;
	CDialogEx::OnCancel();
}
