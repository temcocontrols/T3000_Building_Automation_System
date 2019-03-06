// TstatRangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TstatRangeDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CTstatRangeDlg dialog

IMPLEMENT_DYNAMIC(CTstatRangeDlg, CDialogEx)

CTstatRangeDlg::CTstatRangeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTstatRangeDlg::IDD, pParent)
	, m_input_Analog_select(0)
	, m_current_range(0)
{
    
    IsModfied = false;

    m_bIncreasingMode=TRUE;
    m_10v = 0;
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
	DDV_MinMaxInt(pDX, m_current_range, 0, 14);



    DDX_Control(pDX, IDC_SLIDER1, m_slider1_ctrl);
    DDX_Control(pDX, IDC_SLIDER2, m_slider2_ctrl);
    DDX_Control(pDX, IDC_SLIDER3, m_slider3_ctrl);
    DDX_Control(pDX, IDC_SLIDER4, m_slider4_ctrl);
    DDX_Control(pDX, IDC_SLIDER5, m_slider5_ctrl);
    DDX_Control(pDX, IDC_SLIDER6, m_slider6_ctrl);
    DDX_Control(pDX, IDC_SLIDER7, m_slider7_ctrl);
    DDX_Control(pDX, IDC_SLIDER8, m_slider8_ctrl);
    DDX_Control(pDX, IDC_SLIDER9, m_slider9_ctrl);
    DDX_Control(pDX, IDC_SLIDER10, m_slider10_ctrl);
    DDX_Control(pDX, IDC_SLIDER11, m_slider11_ctrl);

    DDX_Text(pDX, IDC_EDIT12, m_units_s);

   
}


BEGIN_MESSAGE_MAP(CTstatRangeDlg, CDialogEx)
     ON_WM_VSCROLL()
	 
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
    ON_BN_CLICKED(IDC_RADIO_T_12, &CTstatRangeDlg::OnBnClickedRadioT12)
    ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
    ON_EN_CHANGE(IDC_EDIT8, OnEnChangeEdit8)
    ON_EN_CHANGE(IDC_EDIT9, OnEnChangeEdit9)
    ON_EN_CHANGE(IDC_EDIT13, OnEnChangeEdit13)
    ON_EN_CHANGE(IDC_EDIT14, OnEnChangeEdit14)
    ON_EN_CHANGE(IDC_EDIT15, OnEnChangeEdit15)
    ON_EN_CHANGE(IDC_EDIT16, OnEnChangeEdit16)
    ON_EN_CHANGE(IDC_EDIT17, OnEnChangeEdit17)
    ON_EN_CHANGE(IDC_EDIT18, OnEnChangeEdit18)
    ON_EN_CHANGE(IDC_EDIT19, OnEnChangeEdit19)
    ON_EN_CHANGE(IDC_EDIT20, OnEnChangeEdit20)
    ON_EN_CHANGE(IDC_EDIT12, OnEnChangeEdit12)
    ON_EN_KILLFOCUS(IDC_EDIT1, OnEnKillfocusEdit1)
    ON_EN_KILLFOCUS(IDC_EDIT8, OnEnKillfocusEdit8)
    ON_EN_KILLFOCUS(IDC_EDIT9, OnEnKillfocusEdit9)
    ON_EN_KILLFOCUS(IDC_EDIT13, OnEnKillfocusEdit13)
    ON_EN_KILLFOCUS(IDC_EDIT14, OnEnKillfocusEdit14)
    ON_EN_KILLFOCUS(IDC_EDIT15, OnEnKillfocusEdit15)
    ON_EN_KILLFOCUS(IDC_EDIT16, OnEnKillfocusEdit16)
    ON_EN_KILLFOCUS(IDC_EDIT17, OnEnKillfocusEdit17)
    ON_EN_KILLFOCUS(IDC_EDIT18, OnEnKillfocusEdit18)
    ON_EN_KILLFOCUS(IDC_EDIT19, OnEnKillfocusEdit19)
    ON_EN_KILLFOCUS(IDC_EDIT20, OnEnKillfocusEdit20)
    ON_EN_KILLFOCUS(IDC_EDIT12, OnEnKillfocusEdit12)

    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
    ON_BN_CLICKED(IDC_RADIO_5V, &CTstatRangeDlg::OnBnClickedRadio5v)
    ON_BN_CLICKED(IDC_RADIO_10V, &CTstatRangeDlg::OnBnClickedRadio10v)
	ON_BN_CLICKED(IDC_RADIO_T_13, &CTstatRangeDlg::OnBnClickedRadioT13)
	ON_BN_CLICKED(IDC_RADIO_T_14, &CTstatRangeDlg::OnBnClickedRadioT14)
END_MESSAGE_MAP()


// CTstatRangeDlg message handlers


BOOL CTstatRangeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	m_show_unit.ShowWindow(FALSE);
	m_show_unit.SetWindowTextW(_T(""));
	m_show_unit.textColor(RGB(255,0,0));
	m_show_unit.setFont(24,12,NULL,_T("Arial"));
	m_input_Analog_select=m_current_range;
	Initial_window();
    m_tipvalue =m_slider1_i;
    m_tooltips.Create(GetDlgItem(IDC_SLIDER11));

    m_tooltips.Activate(TRUE);
	((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetFocus();

    if (m_10v==1)
    {
        ((CButton*)GetDlgItem(IDC_RADIO_5V))->SetCheck(0);
        ((CButton*)GetDlgItem(IDC_RADIO_10V))->SetCheck(1);  
    } 
    else
    {
       ((CButton*)GetDlgItem(IDC_RADIO_5V))->SetCheck(1);
       ((CButton*)GetDlgItem(IDC_RADIO_10V))->SetCheck(0);
        
    }
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTstatRangeDlg::Initial_window(){
     

	 for (int i=IDC_RADIO_T_0;i<=IDC_RADIO_T_14;i++)
	 {
	    ((CButton *)GetDlgItem(i))->SetCheck(0);
	 }
     if (m_input_Analog_select > 128)
         m_input_Analog_select = m_input_Analog_select - 128;

     if (m_input_Analog_select > 14)
         m_input_Analog_select = 0;

     CString temp_cs;
     m_show_unit.ShowWindow(TRUE);
     temp_cs.Format(_T("%d"), m_input_Analog_select);
     ((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetWindowTextW(temp_cs);

	  ((CButton *)GetDlgItem(IDC_RADIO_T_0+m_input_Analog_select))->SetCheck(1);
	 
      if (m_input_Analog_select == 12)
      {
          Show4_20maUI(true);
      }
      else
      {
          Show4_20maUI(false);
      }

	  m_show_unit.SetWindowTextW(analog_range_TSTAT6[m_input_Analog_select]);
	  CRect c13;
	  GetDlgItem(IDC_RADIO_T_0 + m_input_Analog_select)->GetWindowRect(c13);   //获取控件的位置 ，饼调整位置;
	  ScreenToClient(c13);  
	  m_rang_pic.SetWindowPos(NULL,c13.left - 40,c13.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
	  m_rang_pic.Invalidate(TRUE);
      InitRangeUI();
   
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
				CWnd *temp_focus=GetFocus();	
                //Maurice require ,click enter and the cursor still in this edit or combobox.
				GetDlgItem(IDC_EDIT1_TEST)->SetFocus();
				temp_focus->SetFocus();
			}
//              else{
//                  this->GetFocus()->GetNextWindow()->SetFocus();
//                  return true;
//              }


			return 1;
		}
        
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CTstatRangeDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	if (message==WM_TSTAT_RANGE)
	{
	 
	   Initial_window();
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CTstatRangeDlg::OnTimer(UINT_PTR nIDEvent)
{
	 
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
	if((nfocusid >= IDC_RADIO_T_0) && (nfocusid <= IDC_RADIO_T_14))
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
	
 
	CString temp;
	GetDlgItemText(IDC_EDIT_RANGE_SELECT,temp);
	if(!temp.IsEmpty())
		bac_range_number_choose = _wtoi(temp);
 

    /*if (m_10v==1)
    {
        ((CButton*)GetDlgItem(IDC_RADIO_5V))->SetCheck(0);
        ((CButton*)GetDlgItem(IDC_RADIO_10V))->SetCheck(1);  
    } 
    else
    {
        ((CButton*)GetDlgItem(IDC_RADIO_5V))->SetCheck(1);
        ((CButton*)GetDlgItem(IDC_RADIO_10V))->SetCheck(0);

    }*/
	CDialogEx::OnOK();
}
void CTstatRangeDlg::OnCancel()
{
 
	CDialogEx::OnCancel();
}
void CTstatRangeDlg::Click_Radio(){
	  m_input_Analog_select = 0;
	for (int i=IDC_RADIO_T_0;i<=IDC_RADIO_T_14;i++)
	{
		if (((CButton *)GetDlgItem(i))->GetCheck()==1)
		{
            m_input_Analog_select = i - IDC_RADIO_T_0;
			break;
		}

	}
	 
	m_current_range=m_input_Analog_select;
	PostMessage(WM_TSTAT_RANGE,0,0);
}
void CTstatRangeDlg::OnBnClickedOk()
{
  if (m_input_Analog_select == 4|| m_input_Analog_select==6 || m_input_Analog_select == 12)
  {
      if(!CheckDataisRight())
      {
          CString strPropt=_T("The volt values should be linear increase or decrease,otherwise will not be saved.\ndo you want to go back to change them now?");
          if((AfxMessageBox(strPropt, MB_YESNO|MB_ICONWARNING)==IDYES))
              return;
          else
          {
              OnOK();
              return;
          }
      }
      RestoreWaitCursor();
      SaveInfoDataToRegister();
      EndWaitCursor();
     	
  }
  OnOK();
}
 BOOL CTstatRangeDlg::CheckDataisRight()
{
 
		CString strtxt;
		GetDlgItem(IDC_EDIT1)->GetWindowText(strtxt);
		int ftemp=_wtoi(strtxt);
		m_slider1_i=ftemp;

		GetDlgItem(IDC_EDIT8)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider2_i=ftemp;

		GetDlgItem(IDC_EDIT9)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider3_i=ftemp;


		GetDlgItem(IDC_EDIT13)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider4_i=ftemp;


		GetDlgItem(IDC_EDIT14)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider5_i=ftemp;


		GetDlgItem(IDC_EDIT15)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider6_i=ftemp;

		GetDlgItem(IDC_EDIT16)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider7_i=ftemp;

		GetDlgItem(IDC_EDIT17)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider8_i=ftemp;

		GetDlgItem(IDC_EDIT18)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider9_i=ftemp;

		GetDlgItem(IDC_EDIT19)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider10_i=ftemp;

		GetDlgItem(IDC_EDIT20)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider11_i=ftemp;

	 
		return TRUE;
	//return FALSE;
}
 void CTstatRangeDlg::SaveInfoDataToRegister()
 {
     if(m_InputNo==2)
     {

          
         write_one(g_tstat_id, MODBUS_TABLE1_ZERO,m_slider1_i*10);
         write_one(g_tstat_id, MODBUS_TABLE1_HALFONE,m_slider2_i*10);
         write_one(g_tstat_id, MODBUS_TABLE1_ONE,m_slider3_i*10);
         write_one(g_tstat_id, MODBUS_TABLE1_HALFTWO,m_slider4_i*10);
         write_one(g_tstat_id, MODBUS_TABLE1_TWO,m_slider5_i*10);
         write_one(g_tstat_id, MODBUS_TABLE1_HALFTHREE,m_slider6_i*10);
         write_one(g_tstat_id, MODBUS_TABLE1_THREE,m_slider7_i*10);
         write_one(g_tstat_id, MODBUS_TABLE1_HALFFOUR,m_slider8_i*10);
         write_one(g_tstat_id, MODBUS_TABLE1_FOUR,m_slider9_i*10);
         write_one(g_tstat_id, MODBUS_TABLE1_HALFFIVE,m_slider10_i*10);
         write_one(g_tstat_id, MODBUS_TABLE1_FIVE,m_slider11_i*10);

         product_register_value[MODBUS_TABLE1_ZERO]      =      m_slider1_i*10 ;
         product_register_value[MODBUS_TABLE1_HALFONE]   =      m_slider2_i*10 ;
         product_register_value[MODBUS_TABLE1_ONE]       =      m_slider3_i*10 ;
         product_register_value[MODBUS_TABLE1_HALFTWO]   =      m_slider4_i*10 ;
         product_register_value[MODBUS_TABLE1_TWO]       =      m_slider5_i*10 ;
         product_register_value[MODBUS_TABLE1_HALFTHREE] =      m_slider6_i*10 ;
         product_register_value[MODBUS_TABLE1_THREE]     =      m_slider7_i*10 ;
         product_register_value[MODBUS_TABLE1_HALFFOUR]  =      m_slider8_i*10 ;
         product_register_value[MODBUS_TABLE1_FOUR]      =      m_slider9_i*10 ;
         product_register_value[MODBUS_TABLE1_HALFFIVE]  =      m_slider10_i*10 ;
         product_register_value[MODBUS_TABLE1_FIVE]      =      m_slider11_i*10 ;

         //}


     }
     else if(m_InputNo==3)
     {

          
         write_one(g_tstat_id, MODBUS_TABLE2_ZERO,m_slider1_i*10);
         write_one(g_tstat_id, MODBUS_TABLE2_HALFONE,m_slider2_i*10);
         write_one(g_tstat_id, MODBUS_TABLE2_ONE,m_slider3_i*10);
         write_one(g_tstat_id, MODBUS_TABLE2_HALFTWO,m_slider4_i*10);
         write_one(g_tstat_id, MODBUS_TABLE2_TWO,m_slider5_i*10);
         write_one(g_tstat_id, MODBUS_TABLE2_HALFTHREE,m_slider6_i*10);
         write_one(g_tstat_id, MODBUS_TABLE2_THREE,m_slider7_i*10);
         write_one(g_tstat_id, MODBUS_TABLE2_HALFFOUR,m_slider8_i*10);
         write_one(g_tstat_id, MODBUS_TABLE2_FOUR,m_slider9_i*10);
         write_one(g_tstat_id, MODBUS_TABLE2_HALFFIVE,m_slider10_i*10);
         write_one(g_tstat_id, MODBUS_TABLE2_FIVE,m_slider11_i*10);


         product_register_value[MODBUS_TABLE2_ZERO]      =      m_slider1_i*10 ;
         product_register_value[MODBUS_TABLE2_HALFONE]   =      m_slider2_i*10 ;
         product_register_value[MODBUS_TABLE2_ONE]       =      m_slider3_i*10 ;
         product_register_value[MODBUS_TABLE2_HALFTWO]   =      m_slider4_i*10 ;
         product_register_value[MODBUS_TABLE2_TWO]       =      m_slider5_i*10 ;
         product_register_value[MODBUS_TABLE2_HALFTHREE] =      m_slider6_i*10 ;
         product_register_value[MODBUS_TABLE2_THREE]     =      m_slider7_i*10 ;
         product_register_value[MODBUS_TABLE2_HALFFOUR]  =      m_slider8_i*10 ;
         product_register_value[MODBUS_TABLE2_FOUR]      =      m_slider9_i*10 ;
         product_register_value[MODBUS_TABLE2_HALFFIVE]  =      m_slider10_i*10 ;
         product_register_value[MODBUS_TABLE2_FIVE]      =      m_slider11_i*10 ;

         //}

     }
     else if (m_input_Analog_select == 12)
     {
         //fandu 2018 01 05  TSTAT 4-20ma 对应2个点的table;
         //979 4ma 对应的最小值
         //980 20ma 对应的最大值
         CString temp_units;
         CString temp_4_value;
         CString temp_20_value;
         GetDlgItemText(IDC_EDIT_TSTAT_4_20_UNITS, temp_units);
         GetDlgItemText(IDC_EDIT_TSTAT_4MA_VALUE, temp_4_value);
         GetDlgItemText(IDC_EDIT__20MA_VALUE, temp_20_value);
         int int_4_value = _wtof(temp_4_value) * 10;
         if (int_4_value > 6553)
         {
             MessageBox(_T("Value can not larger thean 6553"));
         }
         int int_20_value = _wtof(temp_20_value) * 10;
         if (int_20_value > 6553)
         {
             MessageBox(_T("Value can not larger thean 6553"));
         }
         if(temp_units.GetLength()>4)
            temp_units = temp_units.Left(4);
         char cTemp1[255];
         memset(cTemp1, 0, 255);
         WideCharToMultiByte(CP_ACP, 0, temp_units.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
         short high_short = 0;
         short low_short = 0;
         memcpy(&high_short, cTemp1, 2);
         memcpy(&low_short, cTemp1+2, 2);

         write_one(g_tstat_id, 979, (short)int_4_value);
         write_one(g_tstat_id, 980, (short)int_20_value);
         write_one(g_tstat_id, 981, high_short);
         write_one(g_tstat_id, 982, low_short);
         product_register_value[979] = (short)int_4_value;
         product_register_value[980] = (short)int_20_value;
         product_register_value[981] = high_short;
         product_register_value[982] = low_short;
         
     }
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
	
}


void CTstatRangeDlg::OnBnClickedRadio()
{
	Click_Radio();
}


void CTstatRangeDlg::OnBnClickedRadioT6()
{
	Click_Radio();
}


void CTstatRangeDlg::OnBnClickedRadioT7()
{
	Click_Radio();	
}


void CTstatRangeDlg::OnBnClickedRadioT8()
{
	Click_Radio();	
}


void CTstatRangeDlg::OnBnClickedRadioT9()
{
	Click_Radio();	
}


void CTstatRangeDlg::OnBnClickedRadioT10()
{
	Click_Radio();	
}

void CTstatRangeDlg::OnBnClickedRadioT11()
{
	Click_Radio();	
}

void CTstatRangeDlg::OnBnClickedRadioT12()
{
    
    Click_Radio();
}
void CTstatRangeDlg::InitRangeUI()
{
    CenterWindow(this);
    BOOL SHOW = FALSE;
     if (m_input_Analog_select == 4|| m_input_Analog_select==6 )
     {
     SHOW =TRUE;
     if (m_input_Analog_select == 4)
     {
         m_InputNo = 2;
     } 
     else
     {
         m_InputNo = 3;
     }
     }
      
    if(SHOW)
    {
        WINDOWPLACEMENT wp;

        GetWindowPlacement(&wp);

        CRect rc;
        CWnd* pWnd = GetDlgItem(IDC_STATIC_SEPERATOR);
        pWnd->GetWindowRect(&rc); 

        wp.rcNormalPosition.bottom = rc.bottom +380;

        SetWindowPlacement(&wp);

        GetDlgItem(IDC_STATIC_SEPERATOR)->ShowWindow(SW_NORMAL);
        to_fresh();
      
    }
    else
    {
        WINDOWPLACEMENT wp;
        GetWindowPlacement(&wp);

        CRect rc;
        CWnd* pWnd = GetDlgItem(IDC_STATIC_SEPERATOR);
        pWnd->GetWindowRect(&rc);
        //ScreenToClient(&rc);

        wp.rcNormalPosition.bottom = rc.bottom ;//- 20;
        SetWindowPlacement(&wp);

        GetDlgItem(IDC_STATIC_SEPERATOR)->ShowWindow(SW_HIDE);
       
    }
     
}

void CTstatRangeDlg::to_fresh()
{
    if(m_InputNo==2)
    {


        short temp[11];
        for (int i=0;i<11;i++)
        {
            temp[i]=(short)product_register_value[MODBUS_TABLE1_ZERO+i];
        }

        m_nMax=Get_Max(&temp[0],11)/10;
        m_nMin=Get_Min(&temp[0],11)/10;


    }
    else if (m_InputNo==3)
    {

        m_nMax=product_register_value[MODBUS_TABLE2_FIVE]/10;
        m_nMin=product_register_value[MODBUS_TABLE2_ZERO]/10;


        short temp[11];
        for (int i=0;i<11;i++)
        {
            temp[i]=(short)product_register_value[MODBUS_TABLE2_ZERO+i];
        }

        m_nMax=Get_Max(&temp[0],11)/10;
        m_nMin=Get_Min(&temp[0],11)/10;


    }
    int nTemp;
    if(m_nMin>m_nMax)
    {
        nTemp=m_nMin;
        m_nMin=m_nMax;
        m_nMax=nTemp;
    }

    //GetDlgItem(IDC_EDIT10)->SetWindowText(m_max);
    //GetDlgItem(IDC_EDIT11)->SetWindowText(m_min);

    //GetDlgItem(IDC_EDIT10)->EnableWindow(FALSE);
    //GetDlgItem(IDC_EDIT11)->EnableWindow(FALSE);


    if(m_nMax>m_nMin)
    {
        m_bIncreasingMode=TRUE;
    }
    else
    {
        m_bIncreasingMode=FALSE;
    }

    CStatic * pstatic2 = (CStatic *)GetDlgItem(IDC_STATIC2);
    CStatic * pstatic3 = (CStatic *)GetDlgItem(IDC_STATIC3);
    CStatic * pstatic4 = (CStatic *)GetDlgItem(IDC_STATIC4);
    CStatic * pstatic5 = (CStatic *)GetDlgItem(IDC_STATIC5);
    CStatic * pstatic6 = (CStatic *)GetDlgItem(IDC_STATIC6);
    CStatic * pstatic7 = (CStatic *)GetDlgItem(IDC_STATIC7);
    CStatic * pstatic8 = (CStatic *)GetDlgItem(IDC_STATIC8);
    CStatic * pstatic9 = (CStatic *)GetDlgItem(IDC_STATIC9);
    CStatic * pstatic10 = (CStatic *)GetDlgItem(IDC_STATIC10);
    CStatic * pstatic11 = (CStatic *)GetDlgItem(IDC_STATIC11);

    m_slider1_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider2_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider3_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider4_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider5_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider6_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider7_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider8_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider9_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider10_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider11_ctrl.SetRange(m_nMin,m_nMax);
    
   
    if(m_InputNo==2)
    { 


        m_slider1_i=(short)product_register_value[MODBUS_TABLE1_ZERO]/10;
        m_slider2_i=(short)product_register_value[MODBUS_TABLE1_HALFONE]/10;
        m_slider3_i=(short)product_register_value[MODBUS_TABLE1_ONE]/10;
        m_slider4_i=(short)product_register_value[MODBUS_TABLE1_HALFTWO]/10;
        m_slider5_i=(short)product_register_value[MODBUS_TABLE1_TWO]/10;
        m_slider6_i=(short)product_register_value[MODBUS_TABLE1_HALFTHREE]/10;
        m_slider7_i=(short)product_register_value[MODBUS_TABLE1_THREE]/10;
        m_slider8_i=(short)product_register_value[MODBUS_TABLE1_HALFFOUR]/10;
        m_slider9_i=(short)product_register_value[MODBUS_TABLE1_FOUR]/10;
        m_slider10_i=(short)product_register_value[MODBUS_TABLE1_HALFFIVE]/10;
        m_slider11_i=(short)product_register_value[MODBUS_TABLE1_FIVE]/10;

        int m_271=product_register_value[MODBUS_UNITS1_HIGH];
        int m_272=product_register_value[MODBUS_UNITS1_LOW];
        if(m_271>>8=='0')
        {
            if((m_271 & 0xFF) =='0')
            {
                if(m_272>>8=='0')
                    m_units_s.Format(_T("%c"),m_272 & 0xFF);
                else
                    m_units_s.Format(_T("%c%c"),m_272>>8,m_272 & 0xFF);
            }
            else
                m_units_s.Format(_T("%c%c%c"),m_271 & 0xFF,m_272>>8,m_272 & 0xFF);
        }
        else
            m_units_s.Format(_T("%c%c%c%c"),m_271>>8,m_271 & 0xFF,m_272>>8,m_272 & 0xFF);

        GetDlgItem(IDC_EDIT12)->SetWindowText(m_units_s);
    }
    else if (m_InputNo==3)
    {

        m_slider1_i=(short)product_register_value[MODBUS_TABLE2_ZERO]/10;
        m_slider2_i=(short)product_register_value[MODBUS_TABLE2_HALFONE]/10;
        m_slider3_i=(short)product_register_value[MODBUS_TABLE2_ONE]/10;
        m_slider4_i=(short)product_register_value[MODBUS_TABLE2_HALFTWO]/10;
        m_slider5_i=(short)product_register_value[MODBUS_TABLE2_TWO]/10;
        m_slider6_i=(short)product_register_value[MODBUS_TABLE2_HALFTHREE]/10;
        m_slider7_i=(short)product_register_value[MODBUS_TABLE2_THREE]/10;
        m_slider8_i=(short)product_register_value[MODBUS_TABLE2_HALFFOUR]/10;
        m_slider9_i=(short)product_register_value[MODBUS_TABLE2_FOUR]/10;
        m_slider10_i=(short)product_register_value[MODBUS_TABLE2_HALFFIVE]/10;
        m_slider11_i=(short)product_register_value[MODBUS_TABLE2_FIVE]/10;




        int m_273=product_register_value[MODBUS_UNITS2_HIGH];
        int m_274=product_register_value[MODBUS_UNITS2_LOW];
        if(m_273>>8=='0')
        {
            if((m_273 & 0xFF)=='0')
            {
                if(m_274>>8=='0')
                    m_units_s.Format(_T("%c"),m_274 & 0xFF);
                else
                    m_units_s.Format(_T("%c%c"),m_274>>8,m_274 & 0xFF);
            }
            else
                m_units_s.Format(_T("%c%c%c"),m_273 & 0xFF,m_274>>8,m_274 & 0xFF);
        }		
        else
            m_units_s.Format(_T("%c%c%c%c"),m_273>>8,m_273 & 0xFF,m_274>>8,m_274 & 0xFF);

        GetDlgItem(IDC_EDIT12)->SetWindowText(m_units_s);
    }

    m_slider1_ctrl.SetPos(m_nMax - (m_slider1_i-m_nMin));
    m_slider2_ctrl.SetPos(m_nMax - (m_slider2_i-m_nMin));
    m_slider3_ctrl.SetPos(m_nMax - (m_slider3_i-m_nMin));
    m_slider4_ctrl.SetPos(m_nMax - (m_slider4_i-m_nMin));
    m_slider5_ctrl.SetPos(m_nMax - (m_slider5_i-m_nMin));
    m_slider6_ctrl.SetPos(m_nMax - (m_slider6_i-m_nMin));
    m_slider7_ctrl.SetPos(m_nMax - (m_slider7_i-m_nMin));
    m_slider8_ctrl.SetPos(m_nMax - (m_slider8_i-m_nMin));
    m_slider9_ctrl.SetPos(m_nMax - (m_slider9_i-m_nMin));
    m_slider10_ctrl.SetPos(m_nMax - (m_slider10_i-m_nMin));
    m_slider11_ctrl.SetPos(m_nMax - (m_slider11_i-m_nMin));


    int npos=m_slider1_ctrl.GetPos();
    int fvalue=(m_nMax+m_nMin-npos);//IDC_EDIT1
    CString strtext;
    strtext.Format(_T("%d"),fvalue);
    GetDlgItem(IDC_EDIT1)->SetWindowText(strtext);

    fvalue=m_slider2_i;//IDC_EDIT1
    strtext.Format(_T("%d"),fvalue);
    GetDlgItem(IDC_EDIT8)->SetWindowText(strtext);

    fvalue=m_slider3_i;//IDC_EDIT1
    strtext.Format(_T("%d"),fvalue);
    GetDlgItem(IDC_EDIT9)->SetWindowText(strtext);


    fvalue=m_slider4_i;//IDC_EDIT1
    strtext.Format(_T("%d"),fvalue);
    GetDlgItem(IDC_EDIT13)->SetWindowText(strtext);

    fvalue=m_slider5_i;//IDC_EDIT1
    strtext.Format(_T("%d"),fvalue);
    GetDlgItem(IDC_EDIT14)->SetWindowText(strtext);

    fvalue=m_slider6_i;//IDC_EDIT1
    strtext.Format(_T("%d"),fvalue);
    GetDlgItem(IDC_EDIT15)->SetWindowText(strtext);

    fvalue=m_slider7_i;//IDC_EDIT1
    strtext.Format(_T("%d"),fvalue);
    GetDlgItem(IDC_EDIT16)->SetWindowText(strtext);


    fvalue=m_slider8_i;//IDC_EDIT1
    strtext.Format(_T("%d"),fvalue);
    GetDlgItem(IDC_EDIT17)->SetWindowText(strtext);

    fvalue=m_slider9_i;//IDC_EDIT1
    strtext.Format(_T("%d"),fvalue);
    GetDlgItem(IDC_EDIT18)->SetWindowText(strtext);

    fvalue=m_slider10_i;//IDC_EDIT1
    strtext.Format(_T("%d"),fvalue);
    GetDlgItem(IDC_EDIT19)->SetWindowText(strtext);


    fvalue=m_slider11_i;//IDC_EDIT1
    strtext.Format(_T("%d"),fvalue);
    GetDlgItem(IDC_EDIT20)->SetWindowText(strtext);

}
 
short CTstatRangeDlg::Get_Min(short *short_Array,int length){
    short min=short_Array[0];
    for (int i=1;i<length;i++)
    {
        if (min>short_Array[i])
        {
            min=short_Array[i];
        }
    }
    return min;
}
short CTstatRangeDlg::Get_Max(short *short_Array,int length){
    short max=short_Array[0];
    for (int i=1;i<length;i++)
    {
        if (max<short_Array[i])
        {
            max=short_Array[i];
        }
    }
    return max;
}

void CTstatRangeDlg::OnEnChangeEdit1()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	
	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CTstatRangeDlg::OnEnChangeEdit8()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CTstatRangeDlg::OnEnChangeEdit9()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CTstatRangeDlg::OnEnChangeEdit13()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CTstatRangeDlg::OnEnChangeEdit14()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CTstatRangeDlg::OnEnChangeEdit15()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CTstatRangeDlg::OnEnChangeEdit16()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CTstatRangeDlg::OnEnChangeEdit17()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CTstatRangeDlg::OnEnChangeEdit18()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CTstatRangeDlg::OnEnChangeEdit19()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CTstatRangeDlg::OnEnChangeEdit20()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CTstatRangeDlg::OnEnChangeEdit12()
{
	
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	//   在此添加控件通知处理程序代码
	IsModfied=true;
}

void CTstatRangeDlg::OnEnKillfocusEdit1()
{

 
		 
		CString strtxt;
		GetDlgItem(IDC_EDIT1)->GetWindowText(strtxt);
		m_slider1_i=_wtoi(strtxt);
 
}

void CTstatRangeDlg::OnEnKillfocusEdit8()
{
	
// if(m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// {
// 	float ftemp;
// 	CString strtxt;
// 	GetDlgItem(IDC_EDIT8)->GetWindowText(strtxt);
// 	ftemp=(float)_wtof(strtxt);
// 
// 	strtxt.Format(_T("%.1f"),ftemp);
// 	GetDlgItem(IDC_EDIT8)->SetWindowText(strtxt);
// 	ftemp=(float)_wtof(strtxt);
// 	m_slider2_i=(int)(ftemp*10);
// }
// else
// {
	int ftemp;
	CString strtxt;
	GetDlgItem(IDC_EDIT8)->GetWindowText(strtxt);
	ftemp=_wtoi(strtxt);
	 
	m_slider2_i=ftemp;
//}
	

	//UpdateData();	
	refresh();
	
}

void CTstatRangeDlg::OnEnKillfocusEdit9()
{
	
// 	if(m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT9)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT9)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider3_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT9)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

	/*	if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT9)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider3_i=ftemp;

	//}
	

//	UpdateData();		
	refresh();


}

void CTstatRangeDlg::OnEnKillfocusEdit13()
{
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT13)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT13)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider4_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT13)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		/*if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/

		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT13)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider4_i=ftemp;
	//}

	refresh();

}

void CTstatRangeDlg::OnEnKillfocusEdit14()
{
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION) 
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT14)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT14)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider5_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT14)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
	/*	if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT14)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider5_i=ftemp;
	//}

	//UpdateData();		
	refresh();
}

void CTstatRangeDlg::OnEnKillfocusEdit15()
{
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT15)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT15)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider6_i=(int)(ftemp*10);
// 
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT15)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

		/*if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT15)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider6_i=ftemp;
	//}
	
	//	UpdateData();		
	refresh();
	
}

void CTstatRangeDlg::OnEnKillfocusEdit16()
{
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT16)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT16)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider7_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT16)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

		/*if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT16)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider7_i=ftemp;
	//}
	
	//UpdateData();		
	refresh();

}

void CTstatRangeDlg::OnEnKillfocusEdit17()
{
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT17)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT17)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider8_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT17)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

		//if (ftemp<0)
		//{
		//	AfxMessageBox(_T("your input can not be negative"));
		//	to_fresh();
		//	return;
		//}
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT17)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider8_i=ftemp;
	//}

	//UpdateData();		
	refresh();
}

void CTstatRangeDlg::OnEnKillfocusEdit18()
{
	
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT18)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT18)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider9_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int  ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT18)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

		/*if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT18)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider9_i=ftemp;
	//}
	
	//UpdateData();		
	refresh();
	
}

void CTstatRangeDlg::OnEnKillfocusEdit19()
{
	
// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION) 
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT19)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT19)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider10_i=(int)(ftemp*10);
// 
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT19)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

	/*	if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT19)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider10_i=ftemp;
	//}

	//UpdateData();		
	refresh();


}

void CTstatRangeDlg::OnEnKillfocusEdit20()
{

// 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
// 	{
// 		float ftemp;
// 		CString strtxt;
// 		GetDlgItem(IDC_EDIT20)->GetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 
// 		strtxt.Format(_T("%.1f"),ftemp);
// 		GetDlgItem(IDC_EDIT20)->SetWindowText(strtxt);
// 		ftemp=(float)_wtof(strtxt);
// 		m_slider11_i=(int)(ftemp*10);
// 	}
// 	else
// 	{
		int ftemp;
		CString strtxt;
		GetDlgItem(IDC_EDIT20)->GetWindowText(strtxt);
		ftemp=_wtoi(strtxt);

		/*if (ftemp<0)
		{
			AfxMessageBox(_T("your input can not be negative"));
			to_fresh();
			return;
		}*/
		strtxt.Format(_T("%d"),ftemp);
		GetDlgItem(IDC_EDIT20)->SetWindowText(strtxt);
		ftemp=_wtoi(strtxt);
		m_slider11_i=ftemp;
	//}
	//	UpdateData();		
	refresh();

	 
}

void CTstatRangeDlg::OnEnKillfocusEdit12()
{
	
	if(IsModfied)
	{
		UpdateData();		
		unsigned short first,second;
		switch(m_units_s.GetLength())
		{
		case 1:	m_units_s=_T("000")+m_units_s;break;
		case 2:	m_units_s=_T("00")+m_units_s;break;
		case 3:	m_units_s=_T("0")+m_units_s;break;
		case 4:	break;
		default : AfxMessageBox(_T("Wrong input!"));m_units_s=_T("");return;			
		}
		first=m_units_s.GetAt(0)*256+m_units_s.GetAt(1);
		second=m_units_s.GetAt(2)*256+m_units_s.GetAt(3);
		if(m_InputNo==2)
		{
			write_one(g_tstat_id, MODBUS_UNITS1_HIGH,first);
			write_one(g_tstat_id, MODBUS_UNITS1_LOW,second);
		}
		else if(m_InputNo==3)
		{
			write_one(g_tstat_id, MODBUS_UNITS2_HIGH,first);
			write_one(g_tstat_id, MODBUS_UNITS2_LOW,second);
		}
		IsModfied = false;
		//to_fresh();
		refresh();
	}
 
}
void CTstatRangeDlg::OnEnChangeEdit10()
{
    
    // 发送该通知，除非重写 CDialog::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。
    IsModfied=true;
    //   在此添加控件通知处理程序代码
}

void CTstatRangeDlg::OnEnSetfocusEdit10()
{
    
    KillTimer(1);//killtimer
}

void CTstatRangeDlg::refresh()
{


    if(m_slider11_i>=m_slider1_i)
    {

        m_nMin=m_slider1_i;
        m_nMax=m_slider11_i;
    }
    else
    {
        m_nMax=m_slider1_i;
        m_nMin=m_slider11_i;
    }

    // 	if (m_version>=CUSTOM_TABLE_FLOAT_VERSION)
    // 	{
    // 		m_max.Format(_T("%.1f"),m_nMax/10.0);
    // 		m_min.Format(_T("%.1f"),m_nMin/10.0);
    // 		refresh_rule((float)(m_nMin/10.0),(float)(m_nMax/10.0));
    // 	}
    // 	else
    // 	{

    m_max.Format(_T("%d"),m_nMax);
    m_min.Format(_T("%d"),m_nMin);
    //refresh_rule((float)m_nMin,(float)m_nMax);
    //}





   /* GetDlgItem(IDC_EDIT10)->SetWindowText(m_max);
    GetDlgItem(IDC_EDIT11)->SetWindowText(m_min);*/

    CStatic * pstatic2 = (CStatic *)GetDlgItem(IDC_STATIC2);
    CStatic * pstatic3 = (CStatic *)GetDlgItem(IDC_STATIC3);
    CStatic * pstatic4 = (CStatic *)GetDlgItem(IDC_STATIC4);
    CStatic * pstatic5 = (CStatic *)GetDlgItem(IDC_STATIC5);
    CStatic * pstatic6 = (CStatic *)GetDlgItem(IDC_STATIC6);
    CStatic * pstatic7 = (CStatic *)GetDlgItem(IDC_STATIC7);
    CStatic * pstatic8 = (CStatic *)GetDlgItem(IDC_STATIC8);
    CStatic * pstatic9 = (CStatic *)GetDlgItem(IDC_STATIC9);
    CStatic * pstatic10 = (CStatic *)GetDlgItem(IDC_STATIC10);
    CStatic * pstatic11 = (CStatic *)GetDlgItem(IDC_STATIC11);


    m_slider1_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider2_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider3_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider4_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider5_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider6_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider7_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider8_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider9_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider10_ctrl.SetRange(m_nMin,m_nMax);	
    m_slider11_ctrl.SetRange(m_nMin,m_nMax);	



    if(m_InputNo==2)
    {//table1 is click

        int m_271=product_register_value[MODBUS_UNITS1_HIGH];
        int m_272=product_register_value[MODBUS_UNITS1_LOW];
        if(m_271>>8=='0')
        {
            if((m_271 & 0xFF) =='0')
            {
                if(m_272>>8=='0')
                    m_units_s.Format(_T("%c"),m_272 & 0xFF);
                else
                    m_units_s.Format(_T("%c%c"),m_272>>8,m_272 & 0xFF);
            }
            else
                m_units_s.Format(_T("%c%c%c"),m_271 & 0xFF,m_272>>8,m_272 & 0xFF);
        }
        else
            m_units_s.Format(_T("%c%c%c%c"),m_271>>8,m_271 & 0xFF,m_272>>8,m_272 & 0xFF);
    }
    else if(m_InputNo==3)
    {
        //table2 is click
        int m_273=product_register_value[MODBUS_UNITS2_HIGH];
        int m_274=product_register_value[MODBUS_UNITS2_LOW];
        if(m_273>>8=='0')
        {
            if((m_273 & 0xFF)=='0')
            {
                if(m_274>>8=='0')
                    m_units_s.Format(_T("%c"),m_274 & 0xFF);
                else
                    m_units_s.Format(_T("%c%c"),m_274>>8,m_274 & 0xFF);
            }
            else
                m_units_s.Format(_T("%c%c%c"),m_273 & 0xFF,m_274>>8,m_274 & 0xFF);
        }		
        else
            m_units_s.Format(_T("%c%c%c%c"),m_273>>8,m_273 & 0xFF,m_274>>8,m_274 & 0xFF);
    }

    m_slider1_ctrl.SetPos(m_nMax - (m_slider1_i-m_nMin));
    m_slider2_ctrl.SetPos(m_nMax - (m_slider2_i-m_nMin));
    m_slider3_ctrl.SetPos(m_nMax - (m_slider3_i-m_nMin));
    m_slider4_ctrl.SetPos(m_nMax - (m_slider4_i-m_nMin));
    m_slider5_ctrl.SetPos(m_nMax - (m_slider5_i-m_nMin));
    m_slider6_ctrl.SetPos(m_nMax - (m_slider6_i-m_nMin));
    m_slider7_ctrl.SetPos(m_nMax - (m_slider7_i-m_nMin));
    m_slider8_ctrl.SetPos(m_nMax - (m_slider8_i-m_nMin));
    m_slider9_ctrl.SetPos(m_nMax - (m_slider9_i-m_nMin));
    m_slider10_ctrl.SetPos(m_nMax - (m_slider10_i-m_nMin));
    m_slider11_ctrl.SetPos(m_nMax - (m_slider11_i-m_nMin));


    m_slider1_ctrl.Invalidate();
    m_slider2_ctrl.Invalidate();
    m_slider3_ctrl.Invalidate();
    m_slider4_ctrl.Invalidate();
    m_slider5_ctrl.Invalidate();
    m_slider6_ctrl.Invalidate();
    m_slider7_ctrl.Invalidate();
    m_slider8_ctrl.Invalidate();
    m_slider9_ctrl.Invalidate();
    m_slider10_ctrl.Invalidate();
    m_slider11_ctrl.Invalidate();


}
void CTstatRangeDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

     
    int CurPos;
    //float fvalue; 

    if(pScrollBar->m_hWnd==m_slider1_ctrl.m_hWnd)
    {

        CurPos= m_slider1_ctrl.GetPos();
        m_slider1_i=m_nMax+m_nMin-CurPos;
        m_tipvalue =m_slider1_i;
        CString strtext;
        strtext.Format(_T("%d"),m_slider1_i);
        GetDlgItem(IDC_EDIT1)->SetWindowText(strtext);
        m_tooltips.SetParent(GetDlgItem(IDC_SLIDER1));
        m_tooltips.Update();
    }
    if(pScrollBar->m_hWnd==m_slider2_ctrl.m_hWnd)
    {
        CurPos= m_slider2_ctrl.GetPos();

        m_slider2_i=m_nMax+m_nMin-CurPos;
         m_tipvalue =m_slider2_i;
        CString strtext;
        strtext.Format(_T("%d"),m_slider2_i);
        GetDlgItem(IDC_EDIT8)->SetWindowText(strtext);
        m_tooltips.SetParent(GetDlgItem(IDC_SLIDER2));
        m_tooltips.Update();
    }
    if(pScrollBar->m_hWnd==m_slider3_ctrl.m_hWnd)
    {
        CurPos= m_slider3_ctrl.GetPos();

        m_slider3_i=m_nMax+m_nMin-CurPos;
         m_tipvalue =m_slider3_i;
        CString strtext;
        strtext.Format(_T("%d"),m_slider3_i);
        //	AfxMessageBox(strtext);
        GetDlgItem(IDC_EDIT9)->SetWindowText(strtext);
        m_tooltips.SetParent(GetDlgItem(IDC_SLIDER3));
        m_tooltips.Update();
    }
    if(pScrollBar->m_hWnd==m_slider4_ctrl.m_hWnd)
    {
        CurPos= m_slider4_ctrl.GetPos();
        m_slider4_i=m_nMax+m_nMin-CurPos;
         m_tipvalue =m_slider4_i;
        CString strtext;
        strtext.Format(_T("%d"),m_slider4_i);
        GetDlgItem(IDC_EDIT13)->SetWindowText(strtext);
        m_tooltips.SetParent(GetDlgItem(IDC_SLIDER4));
        m_tooltips.Update();
    }
    if(pScrollBar->m_hWnd==m_slider5_ctrl.m_hWnd)
    {
        CurPos= m_slider5_ctrl.GetPos();
        m_slider5_i=m_nMax+m_nMin-CurPos;
        m_tipvalue =m_slider5_i;
        CString strtext;
        strtext.Format(_T("%d"),m_slider5_i);
        GetDlgItem(IDC_EDIT14)->SetWindowText(strtext);
        m_tooltips.SetParent(GetDlgItem(IDC_SLIDER5));
        m_tooltips.Update();
    }
    if(pScrollBar->m_hWnd==m_slider6_ctrl.m_hWnd)
    {
        CurPos= m_slider6_ctrl.GetPos();

        m_slider6_i=m_nMax+m_nMin-CurPos;
         m_tipvalue =m_slider6_i;
        CString strtext;
        strtext.Format(_T("%d"),m_slider6_i);
        GetDlgItem(IDC_EDIT15)->SetWindowText(strtext);
        m_tooltips.SetParent(GetDlgItem(IDC_SLIDER6));
        m_tooltips.Update();

    }
    if(pScrollBar->m_hWnd==m_slider7_ctrl.m_hWnd)
    {
        CurPos= m_slider7_ctrl.GetPos();
        m_slider7_i=m_nMax+m_nMin-CurPos;
         m_tipvalue =m_slider7_i;
        CString strtext;
        strtext.Format(_T("%d"),m_slider7_i);
        GetDlgItem(IDC_EDIT16)->SetWindowText(strtext);
        m_tooltips.SetParent(GetDlgItem(IDC_SLIDER7));
        m_tooltips.Update();
    }
    if(pScrollBar->m_hWnd==m_slider8_ctrl.m_hWnd)
    {
        CurPos= m_slider8_ctrl.GetPos();	
        m_slider8_i=m_nMax+m_nMin-CurPos;
         m_tipvalue =m_slider8_i;
        CString strtext;
        strtext.Format(_T("%d"),m_slider8_i);
        GetDlgItem(IDC_EDIT17)->SetWindowText(strtext);
        m_tooltips.SetParent(GetDlgItem(IDC_SLIDER8));
        m_tooltips.Update();
    }
    if(pScrollBar->m_hWnd==m_slider9_ctrl.m_hWnd)
    {
        CurPos= m_slider9_ctrl.GetPos();
        m_slider9_i=m_nMax+m_nMin-CurPos;
      
        m_tipvalue =m_slider9_i;
        CString strtext;
        strtext.Format(_T("%d"),m_slider9_i);
        GetDlgItem(IDC_EDIT18)->SetWindowText(strtext);
        m_tooltips.SetParent(GetDlgItem(IDC_SLIDER9));
        m_tooltips.Update();
    }
    if(pScrollBar->m_hWnd==m_slider10_ctrl.m_hWnd)
    {
        CurPos= m_slider10_ctrl.GetPos();
        m_slider10_i=m_nMax+m_nMin-CurPos;
         m_tipvalue =m_slider10_i;
        CString strtext;
        strtext.Format(_T("%d"),m_slider10_i);
        GetDlgItem(IDC_EDIT19)->SetWindowText(strtext);
        m_tooltips.SetParent(GetDlgItem(IDC_SLIDER10));
        m_tooltips.Update();
    }
    if(pScrollBar->m_hWnd==m_slider11_ctrl.m_hWnd)
    {
        CurPos= m_slider11_ctrl.GetPos();
        m_slider11_i=m_nMax+m_nMin-CurPos;
       m_tipvalue =   m_slider11_i;
        CString strtext;
        strtext.Format(_T("%d"),m_slider11_i);
        GetDlgItem(IDC_EDIT20)->SetWindowText(strtext);
        //	m_slider11_i=CurPos;



         m_tooltips.SetParent(GetDlgItem(IDC_SLIDER11));
         m_tooltips.Update();
        
    }


     
       

    //}

    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CTstatRangeDlg::OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
            //  这里就是你要显示的Tooltips，可以根据需要来定制
            CString strToolTips;
            strToolTips.Format(_T("%d"), m_tipvalue);
            
             

             pTTT->lpszText=strToolTips.AllocSysString();
            
            pTTT->hinst = NULL;
            return(TRUE);
        }
    }

    return(FALSE);
}

void CTstatRangeDlg::OnBnClickedRadio5v()
{
    m_10v = 0;

    if (m_10v==1)
    {
        ((CButton*)GetDlgItem(IDC_RADIO_5V))->SetCheck(0);
        ((CButton*)GetDlgItem(IDC_RADIO_10V))->SetCheck(1);  

        GetDlgItem(IDC_STATIC_BOTTOM_0)->SetWindowTextW(_T("0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_1)->SetWindowTextW(_T("1.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_2)->SetWindowTextW(_T("2.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_3)->SetWindowTextW(_T("3.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_4)->SetWindowTextW(_T("4.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_5)->SetWindowTextW(_T("5.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_6)->SetWindowTextW(_T("6.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_7)->SetWindowTextW(_T("7.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_8)->SetWindowTextW(_T("8.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_9)->SetWindowTextW(_T("9.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_10)->SetWindowTextW(_T("10V"));

        GetDlgItem(IDC_STATIC_RIGHT_0)->SetWindowTextW(_T("0V"));
        GetDlgItem(IDC_STATIC_RIGHT_1)->SetWindowTextW(_T("1.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_2)->SetWindowTextW(_T("2.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_3)->SetWindowTextW(_T("3.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_4)->SetWindowTextW(_T("4.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_5)->SetWindowTextW(_T("5.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_6)->SetWindowTextW(_T("6.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_7)->SetWindowTextW(_T("7.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_8)->SetWindowTextW(_T("8.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_9)->SetWindowTextW(_T("9.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_10)->SetWindowTextW(_T("10V"));
    } 
    else
    {
        ((CButton*)GetDlgItem(IDC_RADIO_5V))->SetCheck(1);
        ((CButton*)GetDlgItem(IDC_RADIO_10V))->SetCheck(0);

        GetDlgItem(IDC_STATIC_BOTTOM_0)->SetWindowTextW(_T("0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_1)->SetWindowTextW(_T("0.5V"));
        GetDlgItem(IDC_STATIC_BOTTOM_2)->SetWindowTextW(_T("1.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_3)->SetWindowTextW(_T("1.5V"));
        GetDlgItem(IDC_STATIC_BOTTOM_4)->SetWindowTextW(_T("2.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_5)->SetWindowTextW(_T("2.5V"));
        GetDlgItem(IDC_STATIC_BOTTOM_6)->SetWindowTextW(_T("3.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_7)->SetWindowTextW(_T("3.5V"));
        GetDlgItem(IDC_STATIC_BOTTOM_8)->SetWindowTextW(_T("4.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_9)->SetWindowTextW(_T("4.5V"));
        GetDlgItem(IDC_STATIC_BOTTOM_10)->SetWindowTextW(_T("5.0V"));

        GetDlgItem(IDC_STATIC_RIGHT_0)->SetWindowTextW(_T("0V"));
        GetDlgItem(IDC_STATIC_RIGHT_1)->SetWindowTextW(_T("0.5V"));
        GetDlgItem(IDC_STATIC_RIGHT_2)->SetWindowTextW(_T("1.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_3)->SetWindowTextW(_T("1.5V"));
        GetDlgItem(IDC_STATIC_RIGHT_4)->SetWindowTextW(_T("2.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_5)->SetWindowTextW(_T("2.5V"));
        GetDlgItem(IDC_STATIC_RIGHT_6)->SetWindowTextW(_T("3.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_7)->SetWindowTextW(_T("3.5V"));
        GetDlgItem(IDC_STATIC_RIGHT_8)->SetWindowTextW(_T("4.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_9)->SetWindowTextW(_T("4.5V"));
        GetDlgItem(IDC_STATIC_RIGHT_10)->SetWindowTextW(_T("5.0V"));
    }
}


void CTstatRangeDlg::OnBnClickedRadio10v()
{
    m_10v = 1;
    if (m_10v==1)
    {
        ((CButton*)GetDlgItem(IDC_RADIO_5V))->SetCheck(0);
        ((CButton*)GetDlgItem(IDC_RADIO_10V))->SetCheck(1);  

        GetDlgItem(IDC_STATIC_BOTTOM_0)->SetWindowTextW(_T("0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_1)->SetWindowTextW(_T("1.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_2)->SetWindowTextW(_T("2.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_3)->SetWindowTextW(_T("3.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_4)->SetWindowTextW(_T("4.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_5)->SetWindowTextW(_T("5.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_6)->SetWindowTextW(_T("6.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_7)->SetWindowTextW(_T("7.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_8)->SetWindowTextW(_T("8.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_9)->SetWindowTextW(_T("9.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_10)->SetWindowTextW(_T("10V"));

        GetDlgItem(IDC_STATIC_RIGHT_0)->SetWindowTextW(_T("0V"));
        GetDlgItem(IDC_STATIC_RIGHT_1)->SetWindowTextW(_T("1.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_2)->SetWindowTextW(_T("2.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_3)->SetWindowTextW(_T("3.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_4)->SetWindowTextW(_T("4.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_5)->SetWindowTextW(_T("5.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_6)->SetWindowTextW(_T("6.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_7)->SetWindowTextW(_T("7.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_8)->SetWindowTextW(_T("8.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_9)->SetWindowTextW(_T("9.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_10)->SetWindowTextW(_T("10V"));
    } 
    else
    {
        ((CButton*)GetDlgItem(IDC_RADIO_5V))->SetCheck(1);
        ((CButton*)GetDlgItem(IDC_RADIO_10V))->SetCheck(0);

        GetDlgItem(IDC_STATIC_BOTTOM_0)->SetWindowTextW(_T("0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_1)->SetWindowTextW(_T("0.5V"));
        GetDlgItem(IDC_STATIC_BOTTOM_2)->SetWindowTextW(_T("1.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_3)->SetWindowTextW(_T("1.5V"));
        GetDlgItem(IDC_STATIC_BOTTOM_4)->SetWindowTextW(_T("2.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_5)->SetWindowTextW(_T("2.5V"));
        GetDlgItem(IDC_STATIC_BOTTOM_6)->SetWindowTextW(_T("3.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_7)->SetWindowTextW(_T("3.5V"));
        GetDlgItem(IDC_STATIC_BOTTOM_8)->SetWindowTextW(_T("4.0V"));
        GetDlgItem(IDC_STATIC_BOTTOM_9)->SetWindowTextW(_T("4.5V"));
        GetDlgItem(IDC_STATIC_BOTTOM_10)->SetWindowTextW(_T("5.0V"));

        GetDlgItem(IDC_STATIC_RIGHT_0)->SetWindowTextW(_T("0V"));
        GetDlgItem(IDC_STATIC_RIGHT_1)->SetWindowTextW(_T("0.5V"));
        GetDlgItem(IDC_STATIC_RIGHT_2)->SetWindowTextW(_T("1.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_3)->SetWindowTextW(_T("1.5V"));
        GetDlgItem(IDC_STATIC_RIGHT_4)->SetWindowTextW(_T("2.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_5)->SetWindowTextW(_T("2.5V"));
        GetDlgItem(IDC_STATIC_RIGHT_6)->SetWindowTextW(_T("3.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_7)->SetWindowTextW(_T("3.5V"));
        GetDlgItem(IDC_STATIC_RIGHT_8)->SetWindowTextW(_T("4.0V"));
        GetDlgItem(IDC_STATIC_RIGHT_9)->SetWindowTextW(_T("4.5V"));
        GetDlgItem(IDC_STATIC_RIGHT_10)->SetWindowTextW(_T("5.0V"));
    }
}


void CTstatRangeDlg::OnBnClickedRadioT13()
{
	  Click_Radio();
}


void CTstatRangeDlg::Show4_20maUI(bool nshow)
{
    if (nshow)
    {
        GetDlgItem(IDC_STATIC_TSTAT_GRP1)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_TSTAT_UNITS)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_TSTAT_4_MA)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC_TATSTA_20_MA)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_TSTAT_4_20_UNITS)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT_TSTAT_4MA_VALUE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_EDIT__20MA_VALUE)->ShowWindow(SW_SHOW);

        CString temp_units;
        CString temp_4_value;
        CString temp_20_value;


        char cTemp1[255];
        memset(cTemp1, 0, 255);
        short high_short = product_register_value[981];
        short low_short = product_register_value[982];

        temp_4_value.Format(_T("%0.1f"), ((float)product_register_value[979])/10);
        temp_20_value.Format(_T("%0.1f"), ((float)product_register_value[980])/10);

        memcpy(cTemp1,&high_short, 2);
        memcpy(cTemp1 + 2 ,&low_short, 2);

        MultiByteToWideChar(CP_ACP, 0, (char *)cTemp1, (int)strlen(cTemp1) + 1,
            temp_units.GetBuffer(MAX_PATH), MAX_PATH);
        temp_units.ReleaseBuffer();

        GetDlgItem(IDC_EDIT_TSTAT_4_20_UNITS)->SetWindowTextW(temp_units);
        GetDlgItem(IDC_EDIT_TSTAT_4MA_VALUE)->SetWindowTextW(temp_4_value);
        GetDlgItem(IDC_EDIT__20MA_VALUE)->SetWindowTextW(temp_20_value);
    }
    else
    {
        GetDlgItem(IDC_STATIC_TSTAT_GRP1)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_TSTAT_UNITS)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_TSTAT_4_MA)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC_TATSTA_20_MA)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_TSTAT_4_20_UNITS)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT_TSTAT_4MA_VALUE)->ShowWindow(SW_HIDE);
        GetDlgItem(IDC_EDIT__20MA_VALUE)->ShowWindow(SW_HIDE);
    }
    
}

void CTstatRangeDlg::OnBnClickedRadioT14()
{
    m_input_Analog_select = 0;

    if ((((CButton *)GetDlgItem(IDC_RADIO_5V))->GetCheck() == FALSE) &&
        (((CButton *)GetDlgItem(IDC_RADIO_10V))->GetCheck() == FALSE))
    {
        ((CButton *)GetDlgItem(IDC_RADIO_5V))->SetCheck(True);
        m_current_range = 14;
    }
    else if(   ((CButton *) GetDlgItem(IDC_RADIO_5V))->GetCheck() == TRUE)
    {
        m_current_range = 14;   //// 5V - 14 
    }
    else if (((CButton *)GetDlgItem(IDC_RADIO_10V))->GetCheck() == TRUE)
    {
        m_current_range = 142;  //10 V --  142
    }
    m_input_Analog_select = m_current_range;
    PostMessage(WM_TSTAT_RANGE, 0, 0);
}

