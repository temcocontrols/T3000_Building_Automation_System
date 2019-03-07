// BacnetRange.cpp : implementation file
//  Code by Fance

#include "stdafx.h"
#include "T3000.h"
#include "BacnetRange.h"
#include "afxdialogex.h"
#include "global_define.h"
#include "BacnetAnalogCusRang.h"
#include "global_function.h"
#include "BacnetCustomerDigitalRange.h"
#include "BacnetVarCusRang.h"
CBacnetAnalogCusRang * bac_analog_window = NULL;
static int temp_static_value = 0;
int old_bac_range_number_choose = 0;
int initial_dialog = 0;
bool move_window_to_right = true; 
// BacnetRange dialog

IMPLEMENT_DYNAMIC(BacnetRange, CDialogEx)

BacnetRange::BacnetRange(CWnd* pParent /*=NULL*/)
	: CDialogEx(BacnetRange::IDD, pParent)
	, m_analog_select(0)
	, m_digital_select(0)
	, m_output_Analog_select(0)
	, m_input_Analog_select(0)
    , m_device_type(0)
{

}

BacnetRange::~BacnetRange()
{
}

void BacnetRange::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RANGE_ENTER_UNITS, m_static_range_units_select);
	DDX_Radio(pDX, IDC_RADIO1, m_analog_select);
	DDX_Radio(pDX, IDC_RADIO35, m_digital_select);
	DDX_Radio(pDX, IDC_RADIO47, m_output_Analog_select);
	DDX_Radio(pDX, IDC_RADIO54, m_input_Analog_select);
	DDX_Control(pDX, IDC_STATIC_RANGE_UNITE_SHOW, m_show_unit);
	DDX_Control(pDX, IDC_STATIC_SELECT_RANGE, m_rang_pic);
}


BEGIN_MESSAGE_MAP(BacnetRange, CDialogEx)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_EDIT_RANGE_SELECT, &BacnetRange::OnEnKillfocusEditRangeSelect)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &BacnetRange::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_EDIT_CUSTOMER_RANGE, &BacnetRange::OnBnClickedBtnEditCustomerRange)
	ON_BN_CLICKED(IDC_BTN_EDIT_CUSTOMER_VAR_RANGE, &BacnetRange::OnBnClickedBtnEditCustomerVarRange)
	ON_BN_CLICKED(IDC_RADIO73, &BacnetRange::OnBnClickedRadio73)
	ON_BN_CLICKED(IDC_RADIO74, &BacnetRange::OnBnClickedRadio74)
	ON_BN_CLICKED(IDC_RADIO75, &BacnetRange::OnBnClickedRadio75)
	ON_BN_CLICKED(IDC_RADIO76, &BacnetRange::OnBnClickedRadio76)
	ON_BN_CLICKED(IDC_RADIO77, &BacnetRange::OnBnClickedRadio77)
	ON_BN_CLICKED(IDC_RADIO78, &BacnetRange::OnBnClickedRadio78)
	ON_BN_CLICKED(IDC_RADIO79, &BacnetRange::OnBnClickedRadio79)
	ON_BN_CLICKED(IDC_RADIO80, &BacnetRange::OnBnClickedRadio80)
    ON_BN_CLICKED(IDC_RADIO82, &BacnetRange::OnBnClickedRadio82)
    ON_BN_CLICKED(IDC_RADIO83, &BacnetRange::OnBnClickedRadio83)
    ON_BN_CLICKED(IDC_RADIO84, &BacnetRange::OnBnClickedRadio84)
    ON_BN_CLICKED(IDC_RADIO85, &BacnetRange::OnBnClickedRadio85)
    ON_BN_CLICKED(IDC_RADIO86, &BacnetRange::OnBnClickedRadio86)
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
	

	((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetFocus();
	((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetSel(0,-1);
	Timer2_handle();
	SetTimer(1,1000,NULL);

	if((bacnet_device_type == PM_T3PT12) || (m_device_type == PM_T3PT12))
	{

		GetDlgItem(IDC_RADIO55)->SetWindowTextW(_T("31.  PT100 -40 to 1000 Deg.C"));
		GetDlgItem(IDC_RADIO56)->SetWindowTextW(_T("32.  PT100 -40 to 1800 Deg.F"));
		GetDlgItem(IDC_RADIO59)->SetWindowTextW(_T("35.  PT1000 -40 to 450 Deg.C"));
		GetDlgItem(IDC_RADIO60)->SetWindowTextW(_T("36.  PT1000 -40 to 800 Deg.F"));

		//ÏÈ½ûÓÃËùÓÐµÄÑ¡Ïî;
		for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
		{
			((CButton *)GetDlgItem(i))->EnableWindow(FALSE);
		}

		for (int i = IDC_RADIO101;i <= IDC_RADIO116;i++)
		{
			((CButton *)GetDlgItem(i))->EnableWindow(FALSE);
		}

		for (int i=IDC_RADIO81;i<=IDC_RADIO88;i++)
		{
			((CButton *)GetDlgItem(i))->EnableWindow(FALSE);
		}

		for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
		{
			((CButton *)GetDlgItem(i))->EnableWindow(TRUE);
		}
		for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
		{
			((CButton *)GetDlgItem(i))->EnableWindow(TRUE);
		}
		for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
		{
			((CButton *)GetDlgItem(i))->EnableWindow(FALSE);
		}
		((CButton *)GetDlgItem(IDC_BTN_EDIT_CUSTOMER_RANGE))->EnableWindow(FALSE);
		
		//Ê¹ÄÜPT12ÓÐµÄÑ¡Ïî
		((CButton *)GetDlgItem(IDC_RADIO35))->EnableWindow(1);
		((CButton *)GetDlgItem(IDC_RADIO55))->EnableWindow(1);
		((CButton *)GetDlgItem(IDC_RADIO56))->EnableWindow(1);
		((CButton *)GetDlgItem(IDC_RADIO57))->EnableWindow(1);
		((CButton *)GetDlgItem(IDC_RADIO58))->EnableWindow(1);
		((CButton *)GetDlgItem(IDC_RADIO59))->EnableWindow(1);
		((CButton *)GetDlgItem(IDC_RADIO60))->EnableWindow(1);

		((CButton *)GetDlgItem(IDC_RADIO61))->EnableWindow(1);
		((CButton *)GetDlgItem(IDC_RADIO62))->EnableWindow(1);
	}
	return FALSE;
	//return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void BacnetRange::Initial_static()
{
	CString temp_cs;
	CRect Temp_Rect;
	GetWindowRect(Temp_Rect);

	if((receive_customer_unit) || (offline_mode))
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
			Custom_Digital_Range[i] = temp_off[i] + _T("/") + temp_on[i];

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

	if((bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE) || (initial_dialog == 1))
	{
		m_show_unit.ShowWindow(TRUE);
		
		if(bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE)
		{
			if(bac_range_number_choose> 33)
				bac_range_number_choose = 1;
			if(bac_range_number_choose == 0)
			{
				temp_cs.Format(_T("%d"),bac_range_number_choose);
				GetDlgItem(IDC_RADIO35)->SetFocus();
			}
			else
			{
				temp_cs.Format(_T("%d"),bac_range_number_choose + 30);
				GetDlgItem(IDC_RADIO1 + bac_range_number_choose)->SetFocus();
			}
			GetDlgItem(IDC_BTN_EDIT_CUSTOMER_VAR_RANGE)->ShowWindow(true);
			
		}
		else
		{
			if(bac_range_number_choose> 30)
				bac_range_number_choose = 0;
			GetDlgItem(IDC_RADIO35 + bac_range_number_choose)->SetFocus();
			temp_cs.Format(_T("%d"),bac_range_number_choose);

		}
		GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_cs);
		m_analog_select = bac_range_number_choose;

#pragma region VARIABLE_RANGE

		for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left - 420,c1.top + 310,0,0,SWP_NOZORDER|SWP_NOSIZE);

			GetDlgItem(i)->ShowWindow(true);
		}
		for (int i=IDC_RADIO_VAR_CUS_1;i<=IDC_RADIO_VAR_CUS_5;i++)
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left - 420,c1.top + 310,0,0,SWP_NOZORDER|SWP_NOSIZE);

			GetDlgItem(i)->ShowWindow(true);
			CString temp_info;
			temp_info.Format(_T("%d . %s"),(i-IDC_RADIO_VAR_CUS_1)+64,Analog_Variable_Units[i-IDC_RADIO_VAR_CUS_1]);
			GetDlgItem(i)->SetWindowTextW(temp_info);
		}
		CRect cREC1; 
		GetDlgItem(IDC_BTN_EDIT_CUSTOMER_VAR_RANGE)->GetWindowRect(cREC1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
		ScreenToClient(cREC1);  
		GetDlgItem(IDC_BTN_EDIT_CUSTOMER_VAR_RANGE)->SetWindowPos(NULL,cREC1.left - 420,cREC1.top + 310,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_BTN_EDIT_CUSTOMER_VAR_RANGE)->ShowWindow(true);
		
		CRect cREC2; 
		GetDlgItem(IDC_STATIC_CUSTOMER_VAR_ANALOG_UNITS)->GetWindowRect(cREC2);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
		ScreenToClient(cREC2);  
		GetDlgItem(IDC_STATIC_CUSTOMER_VAR_ANALOG_UNITS)->SetWindowPos(NULL,cREC2.left - 420,cREC2.top + 310,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_CUSTOMER_VAR_ANALOG_UNITS)->ShowWindow(true);

#pragma endregion VARIABLE_RANGE

		GetDlgItem(IDC_RADIO1)->ShowWindow(false);
		for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 25,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
			GetDlgItem(i)->ShowWindow(true);
		}

		for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 25,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
			GetDlgItem(i)->ShowWindow(true);
		}

		for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 25,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
			GetDlgItem(i)->ShowWindow(true);
		}

		CRect c1; 
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
		ScreenToClient(c1);  
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->SetWindowPos(NULL,c1.left + 27,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->ShowWindow(true);


		for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
		{
			GetDlgItem(i)->ShowWindow(false);
		}
		GetDlgItem(IDC_RADIO_NEW200)->ShowWindow(false);
        GetDlgItem(IDC_RADIO_NEW201)->ShowWindow(false);
		
		

		for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
		{
			GetDlgItem(i)->ShowWindow(false);
		}
        GetDlgItem(IDC_STATIC_CUSTOM_RANGE_GROUPBOX)->ShowWindow(false);

		for (int i = IDC_RADIO101;i <= IDC_RADIO116;i++)
		{
			GetDlgItem(i)->ShowWindow(false);
		}

		for (int i=IDC_RADIO81;i<=IDC_RADIO88;i++)
		{
			GetDlgItem(i)->ShowWindow(false);

		}

		

		CRect c3;
		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->GetWindowRect(c3);   
		ScreenToClient(c3);  
		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->SetWindowPos(NULL,c3.left - 420,c3.top + 310,0,0,SWP_NOZORDER|SWP_NOSIZE);

		CRect c2; 
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->GetWindowRect(c2);   
		ScreenToClient(c2);  
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->SetWindowPos(NULL,c2.left + 22,c2.top,0,0,SWP_NOZORDER|SWP_NOSIZE);



		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(true);//variable
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(false);//output
		GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->ShowWindow(false);//input
		MoveWindow(Temp_Rect.left,Temp_Rect.top,750,780);
	}
	else if((bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE) || (initial_dialog == 3))
	{
		m_show_unit.ShowWindow(TRUE);


		if(bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE)
		{
			if((bac_ranges_type>0) && (bac_ranges_type <=5))
			{
				if(bac_range_number_choose == 0)
				{
					temp_cs.Format(_T("%d"),bac_range_number_choose);
					GetDlgItem(IDC_RADIO35)->SetFocus();
				}
				else
				{
					temp_cs.Format(_T("%d"),bac_range_number_choose + 30);
					GetDlgItem(IDC_RADIO1 + bac_range_number_choose)->SetFocus();
				}
			}
			else if(bac_ranges_type == 7)
			{
				temp_cs.Format(_T("%d"),bac_range_number_choose + 30);
				GetDlgItem(IDC_RADIO_NEW200)->SetFocus();
			}
            else if (bac_ranges_type == 8)
            {
                temp_cs.Format(_T("%d"), bac_range_number_choose + 30);
                GetDlgItem(IDC_RADIO_NEW201)->SetFocus();
            }

		}
		else
		{
			if((bac_range_number_choose >= 30) && (bac_range_number_choose <= 37))
				GetDlgItem(IDC_RADIO47 + bac_range_number_choose)->SetFocus();
			temp_cs.Format(_T("%d"),bac_range_number_choose);

		}
		GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_cs);
		m_output_Analog_select = bac_range_number_choose;


		for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)	//Output analog
		{
			CRect c2; 
			GetDlgItem(i)->GetWindowRect(c2);   
			ScreenToClient(c2);  
			GetDlgItem(i)->SetWindowPos(NULL,c2.left - 780,c2.top - 70,0,0,SWP_NOZORDER|SWP_NOSIZE);

			GetDlgItem(i)->ShowWindow(true);
		}
		CRect temp_200; 
		GetDlgItem(IDC_RADIO_NEW200)->GetWindowRect(temp_200);   
		ScreenToClient(temp_200);  
		GetDlgItem(IDC_RADIO_NEW200)->SetWindowPos(NULL,temp_200.left - 780,temp_200.top - 70,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_RADIO_NEW200)->ShowWindow(true);

        CRect temp_201;
        GetDlgItem(IDC_RADIO_NEW201)->GetWindowRect(temp_201);
        ScreenToClient(temp_201);
        GetDlgItem(IDC_RADIO_NEW201)->SetWindowPos(NULL, temp_201.left - 780, temp_201.top - 70, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        GetDlgItem(IDC_RADIO_NEW201)->ShowWindow(true);

        CRect temp_groupbox;
        GetDlgItem(IDC_STATIC_CUSTOM_RANGE_GROUPBOX)->GetWindowRect(temp_groupbox);
        ScreenToClient(temp_groupbox);
        GetDlgItem(IDC_STATIC_CUSTOM_RANGE_GROUPBOX)->SetWindowPos(NULL, temp_groupbox.left - 780, temp_groupbox.top - 70, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        GetDlgItem(IDC_STATIC_CUSTOM_RANGE_GROUPBOX)->ShowWindow(true);

		GetDlgItem(IDC_RADIO47)->ShowWindow(false);
		for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)	//Digital
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top ,0,0,SWP_NOZORDER|SWP_NOSIZE);
			GetDlgItem(i)->ShowWindow(true);
		}
		for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)	//Digital
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top ,0,0,SWP_NOZORDER|SWP_NOSIZE);
			GetDlgItem(i)->ShowWindow(true);
		}
		for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)	//Digital Custom
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top ,0,0,SWP_NOZORDER|SWP_NOSIZE);
			GetDlgItem(i)->ShowWindow(true);
		}
		CRect c1; 
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
		ScreenToClient(c1);  
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->SetWindowPos(NULL,c1.left + 50,c1.top ,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->ShowWindow(true);


		for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
		{
			GetDlgItem(i)->ShowWindow(false);
		}
		for (int i=IDC_RADIO_VAR_CUS_1;i<=IDC_RADIO_VAR_CUS_5;i++)
		{
			GetDlgItem(i)->ShowWindow(false);
		}
		GetDlgItem(IDC_STATIC_CUSTOMER_VAR_ANALOG_UNITS)->ShowWindow(false);
		GetDlgItem(IDC_BTN_EDIT_CUSTOMER_VAR_RANGE)->ShowWindow(false);


		for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
		{
			GetDlgItem(i)->ShowWindow(false);
		}
        GetDlgItem(IDC_STATIC_CUSTOM_RANGE_GROUPBOX)->ShowWindow(false);
		for (int i = IDC_RADIO101;i <= IDC_RADIO116;i++)
		{
			GetDlgItem(i)->ShowWindow(false);
		}

		for (int i=IDC_RADIO81;i<=IDC_RADIO88;i++)
		{
			GetDlgItem(i)->ShowWindow(false);

		}



		CRect c3;
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->GetWindowRect(c3);   
		ScreenToClient(c3);  
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->SetWindowPos(NULL,c3.left - 780,c3.top - 50,0,0,SWP_NOZORDER|SWP_NOSIZE);

		CRect c2; 
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->GetWindowRect(c2);   
		ScreenToClient(c2);  
		GetDlgItem(IDC_STATIC_DIGITAL_UNITS)->SetWindowPos(NULL,c2.left + 45,c2.top ,0,0,SWP_NOZORDER|SWP_NOSIZE);

		GetDlgItem(IDC_STATIC_ANALOG_UNITS2)->ShowWindow(false);//variable
		GetDlgItem(IDC_STATIC_ANALOG_UNITS)->ShowWindow(true);//output
		GetDlgItem(IDC_STATIC_INPUT_ANALOG_UNITS)->ShowWindow(false);//input
		MoveWindow(Temp_Rect.left,Temp_Rect.top,750,630);
	}
	else if((bac_ranges_type == INPUT_RANGE_ANALOG_TYPE) || (initial_dialog == 2))
	{
		m_show_unit.ShowWindow(TRUE);

		if(bac_ranges_type == INPUT_RANGE_ANALOG_TYPE)
		{
			if(bac_range_number_choose == 0)
			{
				temp_cs.Format(_T("%d"),bac_range_number_choose);
				GetDlgItem(IDC_RADIO35)->SetFocus();
			}
			else
			{
				temp_cs.Format(_T("%d"),bac_range_number_choose + 30);
				GetDlgItem(IDC_RADIO54 + bac_range_number_choose)->SetFocus();
			}
		}
		else
		{
			if((bac_range_number_choose >=0) && (bac_range_number_choose <= 11))
				GetDlgItem(IDC_RADIO35 + bac_range_number_choose)->SetFocus();
			else if((bac_range_number_choose >=12) && (bac_range_number_choose <= 22))
				GetDlgItem(IDC_RADIO89 + bac_range_number_choose - 12)->SetFocus();


			temp_cs.Format(_T("%d"),bac_range_number_choose);
		}
		GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_cs);
		m_input_Analog_select = bac_range_number_choose;

		for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
		{
			//GetDlgItem(i)->ShowWindow(false);
			((CButton *)GetDlgItem(i))->ShowWindow(false);
		}
		GetDlgItem(IDC_RADIO_NEW200)->ShowWindow(false);

		for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
		{
			GetDlgItem(i)->ShowWindow(0);
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
			((CButton *)GetDlgItem(i))->ShowWindow(1);
		}

		for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
		{
			GetDlgItem(i)->ShowWindow(0);
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
			((CButton *)GetDlgItem(i))->ShowWindow(1);

		}

		for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
			GetDlgItem(i)->ShowWindow(true);
		}

		CRect c1; 
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
		ScreenToClient(c1);  
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->SetWindowPos(NULL,c1.left + 50,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_CUSTOMER_GROUP)->ShowWindow(true);

		for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
		{
			((CButton *)GetDlgItem(i))->ShowWindow(false);
		}
		for (int i=IDC_RADIO_VAR_CUS_1;i<=IDC_RADIO_VAR_CUS_5;i++)
		{
			((CButton *)GetDlgItem(i))->ShowWindow(false);
		}
		GetDlgItem(IDC_STATIC_CUSTOMER_VAR_ANALOG_UNITS)->ShowWindow(false);
		GetDlgItem(IDC_BTN_EDIT_CUSTOMER_VAR_RANGE)->ShowWindow(false);

		for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

			((CButton *)GetDlgItem(i))->ShowWindow(1);
		}
        CRect c1_range_groupbox;
        GetDlgItem(IDC_STATIC_CUSTOM_RANGE_GROUPBOX)->GetWindowRect(c1_range_groupbox);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
        ScreenToClient(c1_range_groupbox);
        GetDlgItem(IDC_STATIC_CUSTOM_RANGE_GROUPBOX)->SetWindowPos(NULL, c1_range_groupbox.left + 50, c1_range_groupbox.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        ((CButton *)GetDlgItem(IDC_STATIC_CUSTOM_RANGE_GROUPBOX))->ShowWindow(1);


		for (int i = IDC_RADIO101;i <= IDC_RADIO116;i++)
		{
			CRect c1;
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);
			GetDlgItem(i)->SetWindowPos(NULL, c1.left + 50, c1.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

			((CButton *)GetDlgItem(i))->ShowWindow(1);
		}

		for (int i=IDC_RADIO81;i<=IDC_RADIO88;i++)
		{
			CRect c1; 
			GetDlgItem(i)->GetWindowRect(c1);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c1);  
			GetDlgItem(i)->SetWindowPos(NULL,c1.left + 50,c1.top,0,0,SWP_NOZORDER|SWP_NOSIZE);

			((CButton *)GetDlgItem(i))->ShowWindow(1);
		}
		CString temp_cs_analog;
		if(!Analog_Customer_Units[0].IsEmpty())
		{
			temp_cs_analog.Format(_T("50.  Table 1 ( %s )"),Analog_Customer_Units[0]);
			GetDlgItem(IDC_RADIO82)->SetWindowTextW(temp_cs_analog);
		}
		if(!Analog_Customer_Units[1].IsEmpty())
		{
			temp_cs_analog.Format(_T("51.  Table 2 ( %s )"),Analog_Customer_Units[1]);
			GetDlgItem(IDC_RADIO83)->SetWindowTextW(temp_cs_analog);
		}
		if(!Analog_Customer_Units[2].IsEmpty())
		{
			temp_cs_analog.Format(_T("52.  Table 3 ( %s )"),Analog_Customer_Units[2]);
			GetDlgItem(IDC_RADIO84)->SetWindowTextW(temp_cs_analog);
		}
		if(!Analog_Customer_Units[3].IsEmpty())
		{
			temp_cs_analog.Format(_T("53.  Table 4 ( %s )"),Analog_Customer_Units[3]);
			GetDlgItem(IDC_RADIO85)->SetWindowTextW(temp_cs_analog);
		}
		if(!Analog_Customer_Units[4].IsEmpty())
		{
			temp_cs_analog.Format(_T("54.  Table 5 ( %s )"),Analog_Customer_Units[4]);
			GetDlgItem(IDC_RADIO86)->SetWindowTextW(temp_cs_analog);
		}

		// Big  input27 - 32  small 11 - 16  tiny 6 - 11 range  T3-22 1-11 ÀïÃæÓÐ¸ßËÙÂö³å.  ÆäËûµÄ ÉèÖÃ´Ërange ¶¼ÊÇµÍËÙÂö³å;
		if((bacnet_device_type == BIG_MINIPANEL || bacnet_device_type == MINIPANELARM) && (input_list_line >=26) && (input_list_line <=31))
		{
			GetDlgItem(IDC_RADIO87)->SetWindowText(_T("55. Pulse Count (Fast 100Hz)"));
			GetDlgItem(IDC_RADIO69)->EnableWindow(FALSE);	
			GetDlgItem(IDC_RADIO87)->EnableWindow(TRUE);
		}
		else if((bacnet_device_type == SMALL_MINIPANEL || bacnet_device_type == MINIPANELARM_LB) && (input_list_line >=10) && (input_list_line <=16))
		{
			GetDlgItem(IDC_RADIO87)->SetWindowText(_T("55. Pulse Count (Fast 100Hz)"));
			GetDlgItem(IDC_RADIO69)->EnableWindow(FALSE);	
			GetDlgItem(IDC_RADIO87)->EnableWindow(TRUE);
		}
		else if((bacnet_device_type == TINY_MINIPANEL) && (input_list_line >=5) && (input_list_line <=11))
		{
			GetDlgItem(IDC_RADIO87)->SetWindowText(_T("55. Pulse Count (Fast 100Hz)"));
			GetDlgItem(IDC_RADIO69)->EnableWindow(FALSE);	
			GetDlgItem(IDC_RADIO87)->EnableWindow(TRUE);
		}
		else if ((bacnet_device_type == TINY_EX_MINIPANEL || bacnet_device_type == MINIPANELARM_TB) && (input_list_line >= 0) && (input_list_line <= 7))
		{
			GetDlgItem(IDC_RADIO87)->SetWindowText(_T("55. Pulse Count (Fast 100Hz)"));
			GetDlgItem(IDC_RADIO69)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO87)->EnableWindow(TRUE);
		}
		else if((bacnet_device_type == PID_T322AI) && (input_list_line >= 0) && (input_list_line <=10))
		{
			GetDlgItem(IDC_RADIO87)->SetWindowText(_T("55. Pulse Count (Fast 100Hz)"));
			GetDlgItem(IDC_RADIO69)->EnableWindow(FALSE);	
			GetDlgItem(IDC_RADIO87)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_RADIO69)->EnableWindow(TRUE);	
			GetDlgItem(IDC_RADIO87)->EnableWindow(FALSE);
		}

		GetDlgItem(IDC_RADIO54)->ShowWindow(false);

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
		MoveWindow(Temp_Rect.left,Temp_Rect.top,900,800);
	}



	GetDlgItem(IDC_EDIT_RANGE_SELECT)->EnableWindow(1);
	GetDlgItem(IDC_EDIT_RANGE_SELECT)->ShowWindow(1);
	GetDlgItem(IDC_STATIC_RANGE_ENTER_UNITS)->ShowWindow(1);
	UpdateData(FALSE);
}


BOOL BacnetRange::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message == WM_KEYDOWN)
	{
		if(GetFocus()==GetDlgItem(IDC_EDIT_RANGE_SELECT))
		{
		if(((pMsg->wParam >= 48) && (pMsg->wParam <=57)) 
			||(pMsg->wParam >= 96) && (pMsg->wParam <=105))
		{
			//KillTimer(1);
			SetTimer(2,1000,NULL);
		}
		}
		//if(pMsg->lParam)
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void BacnetRange::OnOK()
{
	
	CString temp;
	GetDlgItemText(IDC_EDIT_RANGE_SELECT,temp);
	if(bacnet_device_type == PM_T3PT12)
	{
		if(!temp.IsEmpty())
		{
			int temp_value_pt12 = _wtoi(temp);
			if((temp_value_pt12 != 0) &&
				(temp_value_pt12 != 31) &&
				(temp_value_pt12 != 32) &&
				(temp_value_pt12 != 33) &&
				(temp_value_pt12 != 34) &&
				(temp_value_pt12 != 35) &&
				(temp_value_pt12 != 36) &&
				(temp_value_pt12 != 37) &&
				(temp_value_pt12 != 38) &&
				((temp_value_pt12 < 1) || (temp_value_pt12 >22)))
			{
				MessageBox(_T("Please select a available range!"));
				return;
			}
		}
	}

	range_cancel = false;
	
	
	if(!temp.IsEmpty())
	{
		int temp_value = _wtoi(temp);
		if(temp_value > 30)
		{
			temp_value = _wtoi(temp) - 30;
			if(initial_dialog == 1)
			{
				bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
			}
			else if(initial_dialog == 2)
			{
				bac_ranges_type = INPUT_RANGE_ANALOG_TYPE;
			}
			else if(initial_dialog == 3)
			{
				bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
			}
		}
		else
		{
			if(initial_dialog == 1)
			{
				bac_ranges_type = VARIABLE_RANGE_DIGITAL_TYPE;
			}
			else if(initial_dialog == 2)
			{
				bac_ranges_type = INPUT_RANGE_DIGITAL_TYPE;
			}
			else if(initial_dialog == 3)
			{
				bac_ranges_type = OUTPUT_RANGE_DIGITAL_TYPE;
			}
		}
		bac_range_number_choose = temp_value;
	}
	else
		bac_range_number_choose = 0;

	KillTimer(1);
	KillTimer(2);
	CDialogEx::OnOK();
}


void BacnetRange::OnCancel()
{
	
	range_cancel = true;
	bac_range_number_choose = 0;
	CDialogEx::OnCancel();
}


void BacnetRange::OnTimer(UINT_PTR nIDEvent)
{
	 
	int sel_value;
	CString temp_value;
	bool click_radio = false;
	switch(nIDEvent)
	{
	case 1:
		{
			CRect TempRect;
			GetWindowRect(TempRect);
			if(bac_analog_window !=NULL)
			{
				if(bac_analog_window->m_hWnd !=NULL)
				{
					if(move_window_to_right)
					{
						bac_analog_window->SetWindowPos(NULL,TempRect.right,TempRect.top,0,0,SWP_NOZORDER|SWP_NOSIZE |SWP_NOACTIVATE);
						move_window_to_right = false;
					}
				}
			}




			int nfocusid = 0;
			
			if(GetFocus())
			{
				nfocusid = GetFocus()->GetDlgCtrlID();
			}
			else
				break;


			if(((nfocusid >= IDC_RADIO54) && (nfocusid <= IDC_RADIO72)) || ((nfocusid >= IDC_RADIO81) && (nfocusid <= IDC_RADIO88)) || ( (nfocusid>= IDC_RADIO101) && (nfocusid <= IDC_RADIO116)))
			{
				bac_ranges_type = INPUT_RANGE_ANALOG_TYPE;
				click_radio = true;
			}
			else if((nfocusid >= IDC_RADIO1) && (nfocusid <= IDC_RADIO34))
			{
				bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
				click_radio = true;
			}
			else if((nfocusid >= IDC_RADIO_VAR_CUS_1) && (nfocusid <= IDC_RADIO_VAR_CUS_5))
			{
				bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
				click_radio = true;
			}
			else if((nfocusid >= IDC_RADIO47) && (nfocusid <= IDC_RADIO53))
			{
				bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
				click_radio = true;
			}
			else if(nfocusid == IDC_RADIO_NEW200)
			{
				bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
				click_radio = true;
			}
            else if (nfocusid == IDC_RADIO_NEW201)
            {
                bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
                click_radio = true;
            }
			else if((nfocusid >= IDC_RADIO35) && (nfocusid <= IDC_RADIO46))
			{
					bac_ranges_type = INPUT_RANGE_DIGITAL_TYPE;
					click_radio = true;
			}
			else if((nfocusid >= IDC_RADIO89) && (nfocusid <= IDC_RADIO99)) // 111111111111111111111111
			{
				bac_ranges_type = INPUT_RANGE_DIGITAL_TYPE;
				click_radio = true;

			}
			else if((nfocusid >= IDC_RADIO73) && (nfocusid <= IDC_RADIO80))
			{
				if(initial_dialog == 1)
					bac_ranges_type = VARIABLE_RANGE_DIGITAL_TYPE;
				else if(initial_dialog == 2)
					bac_ranges_type = INPUT_RANGE_DIGITAL_TYPE;
				else if(initial_dialog == 3)
					bac_ranges_type = OUTPUT_RANGE_DIGITAL_TYPE;
				click_radio = true;
			}
			else if(nfocusid == IDC_EDIT_RANGE_SELECT)
			{
				Sleep(1);
				click_radio = false;
				break;
			}
			else
			{
				break;
			}

			for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
			{
				if(((CButton *)GetDlgItem(i))->GetCheck())
				{
					m_digital_select = i - IDC_RADIO35;
					break;
				}
			}

			for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)	//111111111111111111111111
			{
				if(((CButton *)GetDlgItem(i))->GetCheck())
				{
					m_digital_select = i - IDC_RADIO89 + 12;
					break;
				}
			}

			for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
			{
				if(((CButton *)GetDlgItem(i))->GetCheck())
				{
					m_digital_select = i - IDC_RADIO73 + 23;
					break;
				}
			}

			if (m_digital_select != 0)
			{
				m_input_Analog_select = 0;
				if (bac_analog_window != NULL)
				{
					delete bac_analog_window;
					bac_analog_window = NULL;
				}
			}


			if(initial_dialog == 1)
			{
				for (int i=IDC_RADIO2;i<=IDC_RADIO34;i++)
				{
					if(((CButton *)GetDlgItem(i))->GetCheck())
					{
						m_analog_select = i - IDC_RADIO1;
						break;
					}
				}

				for (int i=IDC_RADIO_VAR_CUS_1;i<=IDC_RADIO_VAR_CUS_5;i++)
				{
					if(((CButton *)GetDlgItem(i))->GetCheck())
					{
						m_analog_select = i - (IDC_RADIO_VAR_CUS_1 - 1) + 33;
						break;
					}
				}
				
			}

			if(initial_dialog == 2)
			{
				for (int i=IDC_RADIO55;i<=IDC_RADIO72;i++)
				{
					if(((CButton *)GetDlgItem(i))->GetCheck())
					{
						m_input_Analog_select = i - IDC_RADIO54;
						break;
					}
				}

				for (int i = IDC_RADIO101;i <= IDC_RADIO116;i++)
				{
					if (((CButton *)GetDlgItem(i))->GetCheck())
					{
						m_input_Analog_select = i - IDC_RADIO101 + 27;
						break;
					}
				}

				if(((CButton *)GetDlgItem(IDC_RADIO81))->GetCheck())
				{
					m_input_Analog_select = 19;
				}
				if(((CButton *)GetDlgItem(IDC_RADIO82))->GetCheck())
				{
					m_input_Analog_select = 20;
				}
				if(((CButton *)GetDlgItem(IDC_RADIO83))->GetCheck())
				{
					m_input_Analog_select = 21;
				}
				if(((CButton *)GetDlgItem(IDC_RADIO84))->GetCheck())
				{
					m_input_Analog_select = 22;
				}
				if(((CButton *)GetDlgItem(IDC_RADIO85))->GetCheck())
				{
					m_input_Analog_select = 23;
				}
				if(((CButton *)GetDlgItem(IDC_RADIO86))->GetCheck())
				{
					m_input_Analog_select = 24;
				}
				if(((CButton *)GetDlgItem(IDC_RADIO87))->GetCheck())
				{
					m_input_Analog_select = 25;
				}
				if(((CButton *)GetDlgItem(IDC_RADIO88))->GetCheck())
				{
					m_input_Analog_select = 26;
				}

				//if((m_input_Analog_select == 20)||
				//	(m_input_Analog_select == 21)||
				//	(m_input_Analog_select == 22)||
				//	(m_input_Analog_select == 23)||
				//	(m_input_Analog_select == 24))
				//{
    //                if (bac_analog_window != NULL)
    //                {
    //                    delete bac_analog_window;
    //                    bac_analog_window = NULL;
    //                }
				//	if((bac_analog_window == NULL) || (bac_analog_window->m_hWnd == NULL))
				//	{
				//		bac_analog_window = new CBacnetAnalogCusRang;
				//		bac_analog_window->Create(IDD_DIALOG_BACNET_RANGES_ANALOG_DEF, this);

				//		CRect TempRect1;
				//		GetWindowRect(TempRect1);
				//		bac_analog_window->SetWindowPos(NULL,TempRect1.right,TempRect1.top,0,0,SWP_NOZORDER|SWP_NOSIZE |SWP_NOACTIVATE);

				//		CString temp_window_text;
				//		temp_window_text.Format(_T("Custom Units %d"),m_input_Analog_select - 19);
				//		bac_analog_window->SetWindowTextW(temp_window_text);
				//		bac_analog_window->ShowWindow(SW_SHOW);
				//		TRACE(_T("Create Analog window\r\n"));

				//		int temp_value = 0;
				//		temp_value = m_input_Analog_select - 20;
				//		analog_range_tbl_line = temp_value;
				//		Post_Refresh_Message(g_bac_instance,READANALOG_CUS_TABLE_T3000,temp_value,temp_value,sizeof(Str_table_point),1);
				//		temp_static_value = m_input_Analog_select;
				//		move_window_to_right = true;
				//	}
				//	else
				//	{
				//		if(temp_static_value!= m_input_Analog_select)
				//		{
				//			CRect TempRect1;
				//			GetWindowRect(TempRect1);
				//			bac_analog_window->SetWindowPos(NULL,TempRect1.right,TempRect1.top,0,0,SWP_NOZORDER|SWP_NOSIZE |SWP_NOACTIVATE);
				//			CString temp_window_text;
				//			temp_window_text.Format(_T("Custom Units %d"),m_input_Analog_select - 19);
				//			bac_analog_window->SetWindowTextW(temp_window_text);
				//			bac_analog_window->ShowWindow(SW_SHOW);

				//			int temp_value = 0;
				//			temp_value = m_input_Analog_select - 20;
				//			analog_range_tbl_line = temp_value;
				//			Post_Refresh_Message(g_bac_instance,READANALOG_CUS_TABLE_T3000,temp_value,temp_value,sizeof(Str_table_point),1);
				//			temp_static_value = m_input_Analog_select;
				//		}
				//	}

				//}
				//else
				//{
				//	if(bac_analog_window!=NULL)
				//	{
				//		delete bac_analog_window;
				//		bac_analog_window = NULL;
				//	}

				//}
			}


			if(initial_dialog == 3)
			{
				for (int i=IDC_RADIO48;i<=IDC_RADIO53;i++)
				{
					if(((CButton *)GetDlgItem(i))->GetCheck())
					{
						m_output_Analog_select = i - IDC_RADIO47;
						break;
					}
				}

				if(((CButton *)GetDlgItem(IDC_RADIO_NEW200))->GetCheck())
				{
					m_output_Analog_select = 7;
				}

                if (((CButton *)GetDlgItem(IDC_RADIO_NEW201))->GetCheck())
                {
                    m_output_Analog_select = 8;
                }
			}


			CString temp_cs;
				//initial_dialog;
				if((bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE) || (initial_dialog == 1))
				{
					if(bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE)
					{
						if(m_analog_select < 0) 
							break;

						bac_range_number_choose = m_analog_select;
						
						if((m_analog_select>=34) && (m_analog_select <= 38))
						{
							m_show_unit.SetWindowTextW(	Analog_Variable_Units[m_analog_select-34]);
							CRect c4;
							GetDlgItem(IDC_RADIO_VAR_CUS_1 + m_analog_select - 34)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
							ScreenToClient(c4);  
							m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
							m_rang_pic.Invalidate(TRUE);
						}
						else
						{
							m_show_unit.SetWindowTextW(Variable_Analog_Units_Array[bac_range_number_choose]);
							CRect c4;
							GetDlgItem(IDC_RADIO1 + m_analog_select)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
							ScreenToClient(c4);  
							m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
							m_rang_pic.Invalidate(TRUE);
						}

						if(nfocusid != IDC_EDIT_RANGE_SELECT)
						{
							CString temp_cs_number;
							temp_cs_number.Format(_T("%d"),m_analog_select + 30);
							GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_cs_number);
						}


						
						for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
						for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
						for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
					}
				}
				else if((bac_ranges_type == INPUT_RANGE_ANALOG_TYPE) || (initial_dialog == 2))
				{
					if(bac_ranges_type == INPUT_RANGE_ANALOG_TYPE)
					{
						if(m_input_Analog_select < 0) 
							break;

						bac_range_number_choose = m_input_Analog_select;
						if(bacnet_device_type == PM_T3PT12)
						{
							if(bac_range_number_choose == 1)
							{
								m_show_unit.SetWindowTextW(_T("PT100 -40 to 1000"));
							}
							if(bac_range_number_choose == 2)
							{
								m_show_unit.SetWindowTextW(_T("PT100 -40 to 1800"));
							}
							if(bac_range_number_choose == 5)
							{
								m_show_unit.SetWindowTextW(_T("PT1000 -40 to 450"));
							}
							if(bac_range_number_choose == 6)
							{
								m_show_unit.SetWindowTextW(_T("PT1000 -40 to 800"));
							}
						}
						else
							m_show_unit.SetWindowTextW(Input_Analog_Units_Array[bac_range_number_choose]);

						if(nfocusid != IDC_EDIT_RANGE_SELECT)
						{
							CString temp_cs_number;
							temp_cs_number.Format(_T("%d"),m_input_Analog_select + 30);
							GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_cs_number);
						}
						//if(m_input_Analog_select==19)
						if((m_input_Analog_select>=19) && (m_input_Analog_select<=26))
						{
							int temp_value;
							temp_value = m_input_Analog_select - 19;
							CRect c4;
							GetDlgItem(IDC_RADIO81 + temp_value)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
							ScreenToClient(c4);  
							m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
							m_rang_pic.Invalidate(TRUE);
						}
						else if ((m_input_Analog_select >= 27) && (m_input_Analog_select <= 42))
						{
							int temp_value;
							temp_value = m_input_Analog_select - 27;
							CRect c4;
							GetDlgItem(IDC_RADIO101 + temp_value)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
							ScreenToClient(c4);
							m_rang_pic.SetWindowPos(NULL, c4.left - 40, c4.top - 4, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
							m_rang_pic.Invalidate(TRUE);

						}
						else
						{
							CRect c4;
							GetDlgItem(IDC_RADIO54 + m_input_Analog_select)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
							ScreenToClient(c4);  
							m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
							m_rang_pic.Invalidate(TRUE);
						}




						for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
						for (int i = IDC_RADIO73;i <= IDC_RADIO80;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
						for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
						
						
					}
				}
				else if((bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE) || (initial_dialog == 3))
				{
					if(bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE)
					{
						if(m_output_Analog_select < 0) 
							break;

						bac_range_number_choose = m_output_Analog_select;

						m_show_unit.SetWindowTextW(Output_Analog_Units_Array[bac_range_number_choose]);

						if(nfocusid != IDC_EDIT_RANGE_SELECT)
						{
							CString temp_cs_number;
							temp_cs_number.Format(_T("%d"),m_output_Analog_select + 30);
							GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_cs_number);
						}

                        CRect c4;
                        if (m_output_Analog_select <= 6)
                        {
                            GetDlgItem(IDC_RADIO47 + m_output_Analog_select)->GetWindowRect(c4);   
                        }
                        else if (m_output_Analog_select == 7)
                        {
                            GetDlgItem(IDC_RADIO_NEW200)->GetWindowRect(c4);   
                        }
                        else if (m_output_Analog_select == 8)
                        {
                            GetDlgItem(IDC_RADIO_NEW201)->GetWindowRect(c4);
                        }
						
						
						ScreenToClient(c4);  
						m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
						m_rang_pic.Invalidate(TRUE);

						for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
						for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
						for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
					}
				}

				if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) ||(bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE))
				{
					if(m_digital_select<0)
						break;

						
						if(m_digital_select >22)
						{
							
							bac_range_number_choose = m_digital_select ;

							CRect c6;
							GetDlgItem(IDC_RADIO73 + bac_range_number_choose - 23)->GetWindowRect(c6);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
							ScreenToClient(c6);  
							m_rang_pic.SetWindowPos(NULL,c6.left - 40,c6.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
							m_rang_pic.Invalidate(TRUE);

						}
						else if((m_digital_select >11) && (m_digital_select <=22))
						{
							bac_range_number_choose = m_digital_select ;
							CRect c6;
							GetDlgItem(IDC_RADIO89 + bac_range_number_choose - 12)->GetWindowRect(c6);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
							ScreenToClient(c6);  
							m_rang_pic.SetWindowPos(NULL,c6.left - 40,c6.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
							m_rang_pic.Invalidate(TRUE);
						}
						else
						{
							bac_range_number_choose = m_digital_select;

							CRect c6;
							GetDlgItem(IDC_RADIO35 + bac_range_number_choose)->GetWindowRect(c6);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
							ScreenToClient(c6);  
							m_rang_pic.SetWindowPos(NULL,c6.left - 40,c6.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
							m_rang_pic.Invalidate(TRUE);

						}

						if((bac_range_number_choose >= 23) && (bac_range_number_choose <=30))
						{
							m_show_unit.SetWindowTextW(Custom_Digital_Range[bac_range_number_choose - 23]);
						}
						else
							m_show_unit.SetWindowTextW(Digital_Units_Array[bac_range_number_choose]);

						if(nfocusid != IDC_EDIT_RANGE_SELECT)
						{
							CString temp_cs_d_number;
							temp_cs_d_number.Format(_T("%d"),bac_range_number_choose);
							GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetWindowTextW(temp_cs_d_number);
						}


					if(initial_dialog == 1)
					{
						for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
						for (int i=IDC_RADIO_VAR_CUS_1;i<=IDC_RADIO_VAR_CUS_5;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
					}

					if(initial_dialog == 2)
					{
						for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}

						for (int i=IDC_RADIO81;i<=IDC_RADIO88;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}

						for (int i = IDC_RADIO101;i <= IDC_RADIO116;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
						
					}

					if(initial_dialog == 3)
					{
						for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
						{
							((CButton *)GetDlgItem(i))->SetCheck(false);
						}
						((CButton *)GetDlgItem(IDC_RADIO_NEW200))->SetCheck(false);
                        ((CButton *)GetDlgItem(IDC_RADIO_NEW201))->SetCheck(false);
					}
				}
			
			}


			break;

	case 2:
		{
			KillTimer( 2);
			Timer2_handle();
		}
		break;

	default:
		break;
	}
	
	//GetDlgItem(IDC_EDIT_RANGE_SELECT)->SetFocus();
	CDialogEx::OnTimer(nIDEvent);
}


void BacnetRange::Timer2_handle()
{
	int sel_value;
	CString temp_value;
	SetDlgItemTextW(IDC_STATIC_RANGE_UNITE_SHOW,_T(""));
	GetDlgItemTextW(IDC_EDIT_RANGE_SELECT,temp_value);
	sel_value = _wtoi(temp_value);

	if((bac_ranges_type == VARIABLE_RANGE_ANALOG_TYPE) || 
		(initial_dialog == 1))
	{
		if(sel_value <= 30)
		{
			bac_ranges_type = VARIABLE_RANGE_DIGITAL_TYPE;
			for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO_VAR_CUS_1;i<=IDC_RADIO_VAR_CUS_5;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			if(sel_value<=11)
			{
				m_digital_select = sel_value;
				CRect c4;
				GetDlgItem(IDC_RADIO35 + m_digital_select)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);
				for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				((CButton *)GetDlgItem(IDC_RADIO35 + m_digital_select))->SetCheck(true);
			}
			else if((sel_value>11) && (sel_value <=22))
			{
				m_digital_select = sel_value - 11;
				CRect c4;
				GetDlgItem(IDC_RADIO35 + m_digital_select)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);

				for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				((CButton *)GetDlgItem(IDC_RADIO35 + m_digital_select))->SetCheck(true);
			}
			else
			{
				m_digital_select = sel_value - 11;
				CRect c4;
				GetDlgItem(IDC_RADIO73 + sel_value - 23 )->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);
				for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				((CButton *)GetDlgItem(IDC_RADIO73 + sel_value - 23))->SetCheck(true);
			}
		}
		else if(sel_value <= 63)
		{
			bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
			for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			m_analog_select = sel_value - 30 ;
			CRect c4;
			GetDlgItem(IDC_RADIO1 + m_analog_select )->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c4);  
			m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
			m_rang_pic.Invalidate(TRUE);

			for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO_VAR_CUS_1;i<=IDC_RADIO_VAR_CUS_5;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			((CButton *)GetDlgItem(IDC_RADIO1 + m_analog_select))->SetCheck(true);
		}
		else if((sel_value >= 64) && (sel_value <= 68))
		{
			bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
			for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			m_analog_select = sel_value - 30 ;

			CRect c4;
			GetDlgItem(IDC_RADIO_VAR_CUS_1 + m_analog_select - 34 )->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c4);  
			m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
			m_rang_pic.Invalidate(TRUE);
			for (int i=IDC_RADIO1;i<=IDC_RADIO34;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO_VAR_CUS_1;i<=IDC_RADIO_VAR_CUS_5;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			((CButton *)GetDlgItem(IDC_RADIO_VAR_CUS_1 + m_analog_select - 34))->SetCheck(true);
		}
		else
		{
			MessageBox(_T("Out of range"));
			SetDlgItemTextW(IDC_EDIT_RANGE_SELECT,_T(""));
		}
	}
	else if((bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE) || 	(initial_dialog == 3))
	{
		if(sel_value <= 30)
		{
			bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
			for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			((CButton *)GetDlgItem(IDC_RADIO_NEW200))->SetCheck(false);
            ((CButton *)GetDlgItem(IDC_RADIO_NEW201))->SetCheck(false);

			m_output_Analog_select = 0;
			if(sel_value<=11)
			{
				m_digital_select = sel_value;
				CRect c4;
				GetDlgItem(IDC_RADIO35 + m_digital_select)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);

				for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				((CButton *)GetDlgItem(IDC_RADIO35 + m_digital_select))->SetCheck(true);


			}
			else if((sel_value>11) && (sel_value <=22))
			{
				m_digital_select = sel_value;
				CRect c4;
				GetDlgItem(IDC_RADIO35 + m_digital_select)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);

				for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)  // 111111111111111111111111111111111111111111111111
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				((CButton *)GetDlgItem(IDC_RADIO89 + sel_value - 12))->SetCheck(true);
			}
			else
			{
				m_digital_select = sel_value - 11;
				CRect c4;
				GetDlgItem(IDC_RADIO73 + sel_value - 23 )->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);

				for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				((CButton *)GetDlgItem(IDC_RADIO73 + sel_value - 23))->SetCheck(true);
			}
		}
		else if(sel_value <= 38)
		{
			bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
			for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			m_output_Analog_select = sel_value - 30 ;
			CRect c4;
			GetDlgItem(IDC_RADIO47 + m_output_Analog_select )->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
			ScreenToClient(c4);  
			m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
			m_rang_pic.Invalidate(TRUE);

			for (int i=IDC_RADIO47;i<=IDC_RADIO53;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
            if (sel_value == 37)
            {
                ((CButton *)GetDlgItem(IDC_RADIO_NEW200))->SetCheck(true);
            }
			else
				((CButton *)GetDlgItem(IDC_RADIO_NEW200))->SetCheck(false);

            if (sel_value == 38)
            {
                ((CButton *)GetDlgItem(IDC_RADIO_NEW201))->SetCheck(true);
            }
            else
                ((CButton *)GetDlgItem(IDC_RADIO_NEW201))->SetCheck(false);

			((CButton *)GetDlgItem(IDC_RADIO47 + m_output_Analog_select))->SetCheck(true);
		}
		else
		{
			MessageBox(_T("Out of range"));
			SetDlgItemTextW(IDC_EDIT_RANGE_SELECT,_T(""));
		}
	}
	else if((bac_ranges_type == INPUT_RANGE_ANALOG_TYPE) || 	(initial_dialog == 2))
	{
		if(sel_value <= 30)
		{
			bac_ranges_type = INPUT_RANGE_DIGITAL_TYPE;
			for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO81;i<=IDC_RADIO88;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}

			for (int i = IDC_RADIO101;i <= IDC_RADIO116;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			
			m_input_Analog_select = 0;
			if(sel_value<=11)
			{
				m_digital_select = sel_value;
				CRect c4;
				GetDlgItem(IDC_RADIO35 + m_digital_select)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);
				for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				((CButton *)GetDlgItem(IDC_RADIO35 + m_digital_select))->SetCheck(true);
			}
			else if((sel_value>11) && (sel_value <=22))
			{
				//m_digital_select = sel_value - 11;
				m_digital_select = sel_value ;
				CRect c4;
				GetDlgItem(IDC_RADIO89 + m_digital_select - 12)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);
				for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				((CButton *)GetDlgItem(IDC_RADIO89 + m_digital_select - 12))->SetCheck(true);
			}
			else
			{
				m_digital_select = sel_value;
				CRect c4;
				GetDlgItem(IDC_RADIO73 + sel_value - 23 )->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);

				for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
				{
					((CButton *)GetDlgItem(i))->SetCheck(false);
				}
				((CButton *)GetDlgItem(IDC_RADIO73 + sel_value - 23))->SetCheck(true);

			}
		}
		else if(sel_value <= 72)
		{
			bac_ranges_type = INPUT_RANGE_ANALOG_TYPE;
			for (int i=IDC_RADIO35;i<=IDC_RADIO46;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO89;i<=IDC_RADIO99;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO73;i<=IDC_RADIO80;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			m_input_Analog_select = sel_value - 30 ;

			if(sel_value == 49)
			{
				CRect c4;
				GetDlgItem(IDC_RADIO81)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);
			}
			else if(sel_value == 55)
			{
				CRect c4;
				GetDlgItem(IDC_RADIO87)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);
			}
			else if(sel_value == 56)
			{
				CRect c4;
				GetDlgItem(IDC_RADIO88)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);
			}
			else if ((sel_value >= 57) && (sel_value <= 72))
			{
				int temp_delt = 0;
				temp_delt = sel_value - 57;
				CRect c4;
				GetDlgItem(IDC_RADIO101 + temp_delt)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);
				m_rang_pic.SetWindowPos(NULL, c4.left - 40, c4.top - 4, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);
			}
			else if((sel_value >= 50) && (sel_value <= 54))
			{
				int temp_delt = 0;
				temp_delt = sel_value - 50;
				CRect c4;
				GetDlgItem(IDC_RADIO82 + temp_delt)->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);
			}
			else
			{
				CRect c4;
				GetDlgItem(IDC_RADIO54 + m_input_Analog_select )->GetWindowRect(c4);   //»ñÈ¡¿Ø¼þµÄÎ»ÖÃ £¬²¢µ÷ÕûÎ»ÖÃ;
				ScreenToClient(c4);  
				m_rang_pic.SetWindowPos(NULL,c4.left - 40,c4.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
				m_rang_pic.Invalidate(TRUE);
			}


			for (int i=IDC_RADIO54;i<=IDC_RADIO72;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i = IDC_RADIO101;i <= IDC_RADIO116;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			for (int i=IDC_RADIO81;i<=IDC_RADIO88;i++)
			{
				((CButton *)GetDlgItem(i))->SetCheck(false);
			}
			
			if(sel_value == 49)
			{
				((CButton *)GetDlgItem(IDC_RADIO81))->SetCheck(true);
			}
			else if(sel_value == 56)
			{
				((CButton *)GetDlgItem(IDC_RADIO88))->SetCheck(true);
			}
			else if((sel_value >=50) && (sel_value <=55))
			{
				int delta_temp = 0;
				delta_temp = sel_value - 50;
				((CButton *)GetDlgItem(IDC_RADIO82 + delta_temp))->SetCheck(true);
			}
			else if ((sel_value >= 57) && (sel_value <= 72))
			{
				int delta_temp = 0;
				delta_temp = sel_value - 57;
				((CButton *)GetDlgItem(IDC_RADIO101 + delta_temp))->SetCheck(true);
			}
			else
			{
				((CButton *)GetDlgItem(IDC_RADIO54 + m_input_Analog_select))->SetCheck(true);
			}

		}
		else
		{
			MessageBox(_T("Out of range"));
			SetDlgItemTextW(IDC_EDIT_RANGE_SELECT,_T(""));
		}
	}
}

void BacnetRange::OnEnKillfocusEditRangeSelect()
{
	
	KillTimer(2);
	SetTimer(1,400,NULL);
}


void BacnetRange::OnClose()
{
	 
	range_cancel = true;
	CDialogEx::OnClose();
}


void BacnetRange::OnBnClickedCancel()
{
	
	range_cancel = true;
	CDialogEx::OnCancel();
}


void BacnetRange::UpdateCustomerRangeText()
{

    for (int i = 0;i < BAC_CUSTOMER_UNITS_COUNT; i++)
    {
        MultiByteToWideChar(CP_ACP, 0, (char *)m_customer_unit_data.at(i).digital_units_off, (int)strlen((char *)m_customer_unit_data.at(i).digital_units_off) + 1,
            temp_off[i].GetBuffer(MAX_PATH), MAX_PATH);
        temp_off[i].ReleaseBuffer();
        if (temp_off[i].GetLength() >= 12)
            temp_off[i].Empty();

        MultiByteToWideChar(CP_ACP, 0, (char *)m_customer_unit_data.at(i).digital_units_on, (int)strlen((char *)m_customer_unit_data.at(i).digital_units_on) + 1,
            temp_on[i].GetBuffer(MAX_PATH), MAX_PATH);
        temp_on[i].ReleaseBuffer();
        if (temp_on[i].GetLength() >= 12)
            temp_on[i].Empty();

        CString unit_index;
        unit_index.Format(_T("%d.     "), i + 23);

        temp_unit[i] = unit_index + temp_off[i] + _T("/") + temp_on[i];
        Custom_Digital_Range[i] = temp_off[i] + _T("/") + temp_on[i];

    }

    for (int j = 0;j <= 7; j++)
    {
        GetDlgItem(IDC_RADIO73 + j)->SetWindowTextW(temp_unit[j]);
    }
}

void BacnetRange::OnBnClickedBtnEditCustomerRange()
{

	CBacnetCustomerDigitalRange dlg;
	dlg.DoModal();

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
		Custom_Digital_Range[i] = temp_off[i] + _T("/") + temp_on[i];

	}

	for(int j=0;j<=7 ; j++)
	{
		GetDlgItem(IDC_RADIO73 + j)->SetWindowTextW(temp_unit[j]);
	}
}


void BacnetRange::OnBnClickedBtnEditCustomerVarRange()
{
	
	CBacnetVarCusRang Rangdlg;
	Rangdlg.DoModal();
	for (int i=IDC_RADIO_VAR_CUS_1;i<=IDC_RADIO_VAR_CUS_5;i++)
	{
		CString temp_info;
		temp_info.Format(_T("%d . %s"),(i-IDC_RADIO_VAR_CUS_1)+64,Analog_Variable_Units[i-IDC_RADIO_VAR_CUS_1]);
		GetDlgItem(i)->SetWindowTextW(temp_info);
	}
}


void BacnetRange::OnBnClickedRadio73()
{
	
    UpdateCustomerRangeText();
}


void BacnetRange::OnBnClickedRadio74()
{
	
    UpdateCustomerRangeText();
}


void BacnetRange::OnBnClickedRadio75()
{
	
    UpdateCustomerRangeText();
}


void BacnetRange::OnBnClickedRadio76()
{
	
    UpdateCustomerRangeText();
}


void BacnetRange::OnBnClickedRadio77()
{
	
    UpdateCustomerRangeText();
}


void BacnetRange::OnBnClickedRadio78()
{
	
    UpdateCustomerRangeText();
}


void BacnetRange::OnBnClickedRadio79()
{
	
    UpdateCustomerRangeText();
}


void BacnetRange::OnBnClickedRadio80()
{
	
    UpdateCustomerRangeText();
}





void BacnetRange::OnBnClickedRadio82()
{
    // TODO: 在此添加控件通知处理程序代码
    m_input_Analog_select = 20;
    ShowAnalogCusRange();
}


void BacnetRange::OnBnClickedRadio83()
{
    // TODO: 在此添加控件通知处理程序代码
    m_input_Analog_select = 21;
    ShowAnalogCusRange();
}


void BacnetRange::OnBnClickedRadio84()
{
    // TODO: 在此添加控件通知处理程序代码
    m_input_Analog_select = 22;
    ShowAnalogCusRange();
}


void BacnetRange::OnBnClickedRadio85()
{
    // TODO: 在此添加控件通知处理程序代码
    m_input_Analog_select = 23;
    ShowAnalogCusRange();
}


void BacnetRange::OnBnClickedRadio86()
{
    // TODO: 在此添加控件通知处理程序代码
    m_input_Analog_select = 24;
    ShowAnalogCusRange();
}

extern CString cs_windowtext;
void BacnetRange::ShowAnalogCusRange()
{
    if (bac_analog_window != NULL)
    {
        delete bac_analog_window;
        bac_analog_window = NULL;
    }

    if ((m_input_Analog_select == 20) ||
        (m_input_Analog_select == 21) ||
        (m_input_Analog_select == 22) ||
        (m_input_Analog_select == 23) ||
        (m_input_Analog_select == 24))
    {
        if ((g_protocol == MODBUS_BACNET_MSTP) ||
            (g_protocol == PROTOCOL_BACNET_IP))// MSTP_转MUDBUS 协议，因为10000以后没有自定义的CUSTOM 表;
        {
            int temp_value = 0;
            temp_value = m_input_Analog_select - 20;
            analog_range_tbl_line = temp_value;
            GetPrivateData_Blocking(g_bac_instance, READANALOG_CUS_TABLE_T3000, temp_value, temp_value, sizeof(Str_table_point));
        }
        CBacnetAnalogCusRang AnalogCusRangdlg;
        cs_windowtext.Format(_T("Custom Units %d"), m_input_Analog_select - 19);
        AnalogCusRangdlg.DoModal();
    }
}
