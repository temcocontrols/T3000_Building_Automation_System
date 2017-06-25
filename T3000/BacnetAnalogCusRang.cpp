// BacnetAnalogCusRang.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetAnalogCusRang.h"
#include "afxdialogex.h"
#include "global_function.h"

CString C_or_V;

int max_unit = 0xffffffff;
int min_unit = 0x7fffffff;

int max_voltage = 0xffffffff;
int min_voltage = 0x7fffffff;

int voltage_up = -1;
int voltage_down = -1;

int value_up = -1;
int value_down = -1;

// CBacnetAnalogCusRang dialog
HWND temp_gloab_hwnd =NULL;
IMPLEMENT_DYNAMIC(CBacnetAnalogCusRang, CDialogEx)

CBacnetAnalogCusRang::CBacnetAnalogCusRang(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetAnalogCusRang::IDD, pParent)
{

}

CBacnetAnalogCusRang::~CBacnetAnalogCusRang()
{
}

void CBacnetAnalogCusRang::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_DIALOG_BACNET_RANGE_LIST, m_analog_cus_range_list);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_1, m_cus_analog_range_ctrl_0);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_2, m_cus_analog_range_ctrl_1);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_3, m_cus_analog_range_ctrl_2);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_4, m_cus_analog_range_ctrl_3);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_5, m_cus_analog_range_ctrl_4);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_6, m_cus_analog_range_ctrl_5);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_7, m_cus_analog_range_ctrl_6);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_8, m_cus_analog_range_ctrl_7);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_9, m_cus_analog_range_ctrl_8);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_10, m_cus_analog_range_ctrl_9);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_11, m_cus_analog_range_ctrl_10);
}


BEGIN_MESSAGE_MAP(CBacnetAnalogCusRang, CDialogEx)
	 ON_WM_VSCROLL()
	ON_MESSAGE(MY_RESUME_DATA, AnalogRangeTblMessageCallBack)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_AnalogCusRange_Item)
	ON_MESSAGE(WM_REFRESH_BAC_ANALOGCUSRANGE_LIST,Fresh_AnalogCusRange_List)
	ON_WM_CLOSE()
	  ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
	  ON_EN_KILLFOCUS(IDC_EDIT_BAC_CUS_ANALOG_UNIT, &CBacnetAnalogCusRang::OnEnKillfocusEditBacCusAnalogUnit)
	  ON_WM_PAINT()
	  ON_WM_TIMER()
END_MESSAGE_MAP()


// CBacnetAnalogCusRang message handlers
LRESULT  CBacnetAnalogCusRang::AnalogRangeTblMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if(msg_result)
	{
		
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}
	else
	{
		memcpy_s(&m_Program_data.at(pInvoke->mRow),sizeof(Str_program_point),&m_temp_program_data[pInvoke->mRow],sizeof(Str_program_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}
	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_analog_cus_range_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_analog_cus_range_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_analog_cus_range_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);

	Fresh_AnalogCusRange_List(analog_range_tbl_line,analog_range_tbl_line);

	if(pInvoke)
		delete pInvoke;
	return 0;
}

BOOL CBacnetAnalogCusRang::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_tooltips.Create(GetDlgItem(IDC_SLIDER_BAC_DEF_1));

	m_tooltips.Activate(TRUE);
	CheckAllDataValid();
	
	Initial_List();
	Fresh_AnalogCusRange_List(analog_range_tbl_line, analog_range_tbl_line);
	SetTimer(1,1000,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetAnalogCusRang::OnCancel()
{
	
	Fresh_AnalogCusRange_Item(0,0);
	//g_hwnd_now =	temp_gloab_hwnd;
	//analog_cus_range_dlg = NULL;
	CDialogEx::OnCancel();
}


void CBacnetAnalogCusRang::Initial_List()
{
	m_analog_cus_range_list.ShowWindow(SW_HIDE);
	m_analog_cus_range_list.DeleteAllItems();
	while ( m_analog_cus_range_list.DeleteColumn (0)) ;

	int temp_jumper = 0;
	CString Unit_temp;
	temp_jumper = (m_Input_data.at(input_list_line).decom & 0xf0 ) >> 4;
	if(temp_jumper == 1)
	{
		Unit_temp.Format(_T("Current (4-20ma)"));
		C_or_V.Format(_T("ma"));
	}
	else if(temp_jumper == 2)
	{
		Unit_temp.Format(_T("Voltage (0-5V)"));
		C_or_V.Format(_T("V"));
	}
	else if(temp_jumper == 3)
	{
		Unit_temp.Format(_T("Voltage (0-10V)"));
		C_or_V.Format(_T("V"));
	}
	else 
	{
		Unit_temp.Format(_T("Voltage"));
		C_or_V.Format(_T("V"));
	}



	m_analog_cus_range_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_analog_cus_range_list.SetExtendedStyle(m_analog_cus_range_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_analog_cus_range_list.SetExtendedStyle(m_analog_cus_range_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_analog_cus_range_list.InsertColumn(0, Unit_temp, 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_analog_cus_range_list.InsertColumn(1, _T("Value"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);

	temp_gloab_hwnd = g_hwnd_now;
	//g_hwnd_now = this->m_hWnd;
	analog_cus_range_dlg = this->m_hWnd;

	CRect list_rect,win_rect;
	m_analog_cus_range_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_input_dlg_hwnd,win_rect);
	m_analog_cus_range_list.Set_My_WindowRect(win_rect);
	m_analog_cus_range_list.Set_My_ListRect(list_rect);
	m_analog_cus_range_list.Support_Col_0_Edit(true);
	m_analog_cus_range_list.SetListHwnd(this->m_hWnd);
	//m_analog_cus_range_list.DeleteAllItems();
	for (int i=0;i<11;i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

		if(i>=input_item_limit_count)	//vector的大小始终不变 ,用次变量来 约束 要显示的 item 数量;
			break;

		temp_item.Format(_T("%.1f"),i*0.5);
		m_analog_cus_range_list.InsertItem(i,temp_item);



		for (int x=0;x<2;x++)
		{
			if((i%2)==0)
				m_analog_cus_range_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_analog_cus_range_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}
	}
	m_analog_cus_range_list.InitListData();
	m_analog_cus_range_list.ShowWindow(SW_SHOW);
}



LRESULT CBacnetAnalogCusRang::Fresh_AnalogCusRange_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	CString cs_info;



	for (int i=0;i<11;i++)//只要有修改1个，就重新将界面上所有的值 读到 内存 并写入 设备;
	{
			CString cs_temp = m_analog_cus_range_list.GetItemText(i,ANALOG_CUS_RANGE_RBL_VALUE);
			float temp_value =_wtof(cs_temp);
			if((temp_value >6553.5) || (temp_value <0))
			{
				m_analog_cus_range_list.SetItemText(Changed_Item,0,_T(""));
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Value Out of range"));
				continue;
			}
			m_analog_custmer_range.at(analog_range_tbl_line).dat[i].value = (unsigned short)(temp_value * 10);

			CString cs_temp1 = m_analog_cus_range_list.GetItemText(i,ANALOG_CUS_RANGE_RBL_UNIT);
			int temp_value1 =_wtoi(cs_temp1);
			m_analog_custmer_range.at(analog_range_tbl_line).dat[i].unit = temp_value1;
			if(i==0)
			{
				GetDlgItem(IDC_STATIC_VC_1)->SetWindowText(cs_temp);
				GetDlgItem(IDC_STATIC_VALUE_1)->SetWindowText(cs_temp1);
			}
	}

	bool ret_check = false;
	ret_check = CheckAllDataValid();
	if(!ret_check)
	{
		MessageBox(_T("Please input a regular table"));
		return 0;
	}

	CString temp_task_info;
	temp_task_info.Format(_T("Update Analog Range  "));
	Post_Write_Message(g_bac_instance,WRITEANALOG_CUS_TABLE_T3000,analog_range_tbl_line,analog_range_tbl_line,sizeof(Str_table_point),analog_cus_range_dlg ,temp_task_info,Changed_Item,Changed_SubItem);


	return 0;
}


BOOL CBacnetAnalogCusRang::CheckAllDataValid()
{
	int slop_value[11];

	int voltage[11];
	int mvalue[11];
	voltage_up = -1;
	voltage_down = -1;

	value_up = -1;
	value_down = -1;

	memset(voltage,0,sizeof(voltage));
	memset(mvalue,0,sizeof(mvalue));

	max_unit = 0xffffffff;
	min_unit = 0x7fffffff;

	max_voltage = 0xffffffff;
	min_voltage = 0x7fffffff;

	int valid_data_count = 0;
	for (int i=0;i<11;i++)
	{
		if(m_analog_custmer_range.at(analog_range_tbl_line).dat[i+1].value == 0)
		{
			valid_data_count = i;
			break;
		}
		voltage[i] = m_analog_custmer_range.at(analog_range_tbl_line).dat[i+1].value - m_analog_custmer_range.at(analog_range_tbl_line).dat[i].value;
		if(m_analog_custmer_range.at(analog_range_tbl_line).dat[i+1].unit == 0)
		{
			valid_data_count = i;
			break;
		}	
		mvalue[i] = m_analog_custmer_range.at(analog_range_tbl_line).dat[i+1].unit - m_analog_custmer_range.at(analog_range_tbl_line).dat[i].unit;
	}
	for (int i=0;i<11;i++)
	{
		if(i>=valid_data_count)
			break;
		if(mvalue[i] > 0)
		{
			value_up = 1;
		}
		else if(mvalue[i] < 0)
		{
			value_down = 1;
		}
	}

	for (int i=0;i<11;i++)
	{
		if(i>=valid_data_count)
			break;
		if(voltage[i] > 0)
		{
			voltage_up = 1;
		}
		else if(voltage[i] < 0)
		{
			voltage_down = 1;
		}
	}

	//if((voltage_up == 1) && (value_down == 1))
	//if((voltage_up == 1) && (voltage_down == voltage_up))
	if(voltage_down == 1)
	{
		return 0;
	}
	else if((value_up == 1) && (value_up == value_down))
	{
		return 0;
	}
	min_voltage = m_analog_custmer_range.at(analog_range_tbl_line).dat[0].value;
	max_voltage = m_analog_custmer_range.at(analog_range_tbl_line).dat[valid_data_count].value;
	if(value_up == 1)
	{
		min_unit = m_analog_custmer_range.at(analog_range_tbl_line).dat[0].unit;
		max_unit = m_analog_custmer_range.at(analog_range_tbl_line).dat[valid_data_count].unit;
	}
	else if(value_down)
	{
		min_unit = m_analog_custmer_range.at(analog_range_tbl_line).dat[valid_data_count].unit;
		max_unit = m_analog_custmer_range.at(analog_range_tbl_line).dat[0].unit;
	}


	return 1;
}

LRESULT CBacnetAnalogCusRang::Fresh_AnalogCusRange_List(WPARAM wParam,LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);


	CString temp_show_unit;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_analog_custmer_range.at(analog_range_tbl_line).table_name, (int)strlen(m_analog_custmer_range.at(analog_range_tbl_line).table_name)+1, 
		temp_show_unit.GetBuffer(MAX_PATH), MAX_PATH );
	temp_show_unit.ReleaseBuffer();
	if(temp_show_unit.GetLength() > 9)
		temp_show_unit = temp_show_unit.Left(9);
	Analog_Customer_Units[analog_range_tbl_line] = temp_show_unit;
	((CEdit *)GetDlgItem(IDC_EDIT_BAC_CUS_ANALOG_UNIT))->SetWindowText(temp_show_unit);
	
	for (int i=0;i<11;i++)
	{
		CString n_value_2byte;
		CString n_unite_4byte;
		if((i!=0) && (m_analog_custmer_range.at(analog_range_tbl_line).dat[i].value == 0))
		{
			//n_value_2byte.Format(_T("-"));
			//n_unite_4byte.Format(_T("-"));
			n_value_2byte.Empty();
			n_unite_4byte.Empty();
		}
		else
		{
			n_value_2byte.Format(_T("%.1f"),((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[i].value)/10);
			n_unite_4byte.Format(_T("%d"),m_analog_custmer_range.at(analog_range_tbl_line).dat[i].unit);
		}
		m_analog_cus_range_list.SetItemText(i,0,n_value_2byte);
		m_analog_cus_range_list.SetItemText(i,1,n_unite_4byte);

		if(i==0)
		{
			if (!n_value_2byte.IsEmpty())
				GetDlgItem(IDC_STATIC_VC_1)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_1)->SetWindowText(n_unite_4byte);
		}
		else if(i==1)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_2)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_2)->SetWindowText(n_unite_4byte);
		}
		else if(i==2)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_3)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_3)->SetWindowText(n_unite_4byte);
		}
		else if(i==3)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_4)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_4)->SetWindowText(n_unite_4byte);
		}
		else if(i==4)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_5)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_5)->SetWindowText(n_unite_4byte);
		}
		else if(i==5)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_6)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_6)->SetWindowText(n_unite_4byte);
		}
		else if(i==6)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_7)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_7)->SetWindowText(n_unite_4byte);
		}
		else if(i==7)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_8)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_8)->SetWindowText(n_unite_4byte);
		}
		else if(i==8)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_9)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_9)->SetWindowText(n_unite_4byte);
		}
		else if(i==9)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_10)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_10)->SetWindowText(n_unite_4byte);
		}
		else if(i==10)
		{
			if (!n_value_2byte.IsEmpty())
			GetDlgItem(IDC_STATIC_VC_11)->SetWindowText(n_value_2byte+C_or_V);
			GetDlgItem(IDC_STATIC_VALUE_11)->SetWindowText(n_unite_4byte);
		}
	}




	SetSlideRange();

	return 0;
}

void CBacnetAnalogCusRang::SetSlideRange()
{
	if(max_unit == min_unit)
	{
		max_unit = min_unit + 1;
	}
	  m_cus_analog_range_ctrl_0.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_1.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_2.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_3.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_4.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_5.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_6.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_7.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_8.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_9.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_10.SetRange(min_unit,max_unit);	

	  m_cus_analog_range_ctrl_0.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[0].unit-min_unit));
	  m_cus_analog_range_ctrl_1.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[1].unit-min_unit));
	  m_cus_analog_range_ctrl_2.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[2].unit-min_unit));
	  m_cus_analog_range_ctrl_3.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[3].unit-min_unit));
	  m_cus_analog_range_ctrl_4.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[4].unit-min_unit));
	  m_cus_analog_range_ctrl_5.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[5].unit-min_unit));
	  m_cus_analog_range_ctrl_6.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[6].unit-min_unit));
	  m_cus_analog_range_ctrl_7.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[7].unit-min_unit));
	  m_cus_analog_range_ctrl_8.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[8].unit-min_unit));
	  m_cus_analog_range_ctrl_9.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[9].unit-min_unit));
	  m_cus_analog_range_ctrl_10.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[10].unit-min_unit));
	  UpdateData();
}


void CBacnetAnalogCusRang::OnClose()
{
	 
	//g_hwnd_now =	temp_gloab_hwnd;
	//analog_cus_range_dlg = NULL;
	CDialogEx::OnClose();
}


BOOL CBacnetAnalogCusRang::PreTranslateMessage(MSG* pMsg)
{
	
	CWnd * window_focus =	GetFocus();
	if(window_focus == NULL)
		return 0;
	int temp_focus_id = GetFocus()->GetDlgCtrlID();
	if(temp_focus_id == IDC_EDIT_BAC_CUS_ANALOG_UNIT )
	{
		if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		{
			UpdateCusAnalogUnit();
			return TRUE;
		}
	}
	else if((pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)  || (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE))
	{
		CWnd * window_focus =	GetFocus();
		if(window_focus == NULL)
			return CDialogEx::PreTranslateMessage(pMsg);
		if((GetFocus()->GetDlgCtrlID() == IDD_DIALOG_BACNET_RANGE_LIST) && (pMsg->wParam==VK_RETURN))
		{
			CRect list_rect,win_rect;
			m_analog_cus_range_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(analog_cus_range_dlg,win_rect);
			m_analog_cus_range_list.Set_My_WindowRect(win_rect);
			m_analog_cus_range_list.Set_My_ListRect(list_rect);

			m_analog_cus_range_list.Get_clicked_mouse_position();
			return TRUE;
		}
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}



void CBacnetAnalogCusRang::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{


	int CurPos;
	int temp_value;
	//float fvalue; 0oi-i--9999999999
	CRect temp_1;
	CRect temp_2;

	int ret_top = 0;

	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_0.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_0.GetPos();

		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(0,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_1));
		m_tooltips.Update();
		m_cus_analog_range_ctrl_0.GetWindowRect(temp_1);
		GetDlgItem(IDC_STATIC_VALUE_1)->GetWindowRect(temp_2);
		ScreenToClient(temp_1);
		ScreenToClient(temp_2);
		ret_top = temp_1.top  + (360 -36) * CurPos / 1000;
		GetDlgItem(IDC_STATIC_VALUE_1)->SetWindowPos(NULL,temp_2.left,ret_top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_VALUE_1)->SetWindowText(strtext);

	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_1.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_1.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(1,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_2));
		m_tooltips.Update();
		m_cus_analog_range_ctrl_1.GetWindowRect(temp_1);
		GetDlgItem(IDC_STATIC_VALUE_2)->GetWindowRect(temp_2);
		ScreenToClient(temp_1);
		ScreenToClient(temp_2);
		ret_top = temp_1.top  + (360 -36) * CurPos / 1000;
		GetDlgItem(IDC_STATIC_VALUE_2)->SetWindowPos(NULL,temp_2.left,ret_top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_VALUE_2)->SetWindowText(strtext);
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_2.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_2.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(2,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_3));
		m_tooltips.Update();
		m_cus_analog_range_ctrl_2.GetWindowRect(temp_1);
		GetDlgItem(IDC_STATIC_VALUE_3)->GetWindowRect(temp_2);
		ScreenToClient(temp_1);
		ScreenToClient(temp_2);
		ret_top = temp_1.top  + (360 -36) * CurPos / 1000;
		GetDlgItem(IDC_STATIC_VALUE_3)->SetWindowPos(NULL,temp_2.left,ret_top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_VALUE_3)->SetWindowText(strtext);
	}	
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_3.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_3.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(3,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_4));
		m_tooltips.Update();
		m_cus_analog_range_ctrl_3.GetWindowRect(temp_1);
		GetDlgItem(IDC_STATIC_VALUE_4)->GetWindowRect(temp_2);
		ScreenToClient(temp_1);
		ScreenToClient(temp_2);
		ret_top = temp_1.top  + (360 -36) * CurPos / 1000;
		GetDlgItem(IDC_STATIC_VALUE_4)->SetWindowPos(NULL,temp_2.left,ret_top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_VALUE_4)->SetWindowText(strtext);
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_4.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_4.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(4,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_5));
		m_tooltips.Update();
		m_cus_analog_range_ctrl_4.GetWindowRect(temp_1);
		GetDlgItem(IDC_STATIC_VALUE_5)->GetWindowRect(temp_2);
		ScreenToClient(temp_1);
		ScreenToClient(temp_2);
		ret_top = temp_1.top  + (360 -36) * CurPos / 1000;
		GetDlgItem(IDC_STATIC_VALUE_5)->SetWindowPos(NULL,temp_2.left,ret_top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_VALUE_5)->SetWindowText(strtext);
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_5.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_5.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(5,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_6));
		m_tooltips.Update();
		m_cus_analog_range_ctrl_5.GetWindowRect(temp_1);
		GetDlgItem(IDC_STATIC_VALUE_6)->GetWindowRect(temp_2);
		ScreenToClient(temp_1);
		ScreenToClient(temp_2);
		ret_top = temp_1.top  + (360 -36) * CurPos / 1000;
		GetDlgItem(IDC_STATIC_VALUE_6)->SetWindowPos(NULL,temp_2.left,ret_top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_VALUE_6)->SetWindowText(strtext);
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_6.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_6.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(6,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_7));
		m_tooltips.Update();
		m_cus_analog_range_ctrl_6.GetWindowRect(temp_1);
		GetDlgItem(IDC_STATIC_VALUE_7)->GetWindowRect(temp_2);
		ScreenToClient(temp_1);
		ScreenToClient(temp_2);
		ret_top = temp_1.top  + (360 -36) * CurPos / 1000;
		GetDlgItem(IDC_STATIC_VALUE_7)->SetWindowPos(NULL,temp_2.left,ret_top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_VALUE_7)->SetWindowText(strtext);
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_7.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_7.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(7,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_8));
		m_tooltips.Update();
		m_cus_analog_range_ctrl_7.GetWindowRect(temp_1);
		GetDlgItem(IDC_STATIC_VALUE_8)->GetWindowRect(temp_2);
		ScreenToClient(temp_1);
		ScreenToClient(temp_2);
		ret_top = temp_1.top  + (360 -36) * CurPos / 1000;
		GetDlgItem(IDC_STATIC_VALUE_8)->SetWindowPos(NULL,temp_2.left,ret_top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_VALUE_8)->SetWindowText(strtext);
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_8.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_8.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(8,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_9));
		m_tooltips.Update();
		m_cus_analog_range_ctrl_8.GetWindowRect(temp_1);
		GetDlgItem(IDC_STATIC_VALUE_9)->GetWindowRect(temp_2);
		ScreenToClient(temp_1);
		ScreenToClient(temp_2);
		ret_top = temp_1.top  + (360 -36) * CurPos / 1000;
		GetDlgItem(IDC_STATIC_VALUE_9)->SetWindowPos(NULL,temp_2.left,ret_top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_VALUE_9)->SetWindowText(strtext);
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_9.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_9.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(9,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_10));
		m_tooltips.Update();
		m_cus_analog_range_ctrl_9.GetWindowRect(temp_1);
		GetDlgItem(IDC_STATIC_VALUE_10)->GetWindowRect(temp_2);
		ScreenToClient(temp_1);
		ScreenToClient(temp_2);
		ret_top = temp_1.top  + (360 -36) * CurPos / 1000;
		GetDlgItem(IDC_STATIC_VALUE_10)->SetWindowPos(NULL,temp_2.left,ret_top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_VALUE_10)->SetWindowText(strtext);
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_10.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_10.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(10,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_11));
		m_tooltips.Update();
		m_cus_analog_range_ctrl_10.GetWindowRect(temp_1);
		GetDlgItem(IDC_STATIC_VALUE_11)->GetWindowRect(temp_2);
		ScreenToClient(temp_1);
		ScreenToClient(temp_2);
		ret_top = temp_1.top  + (360 -36) * CurPos / 1000;
		GetDlgItem(IDC_STATIC_VALUE_11)->SetWindowPos(NULL,temp_2.left,ret_top,0,0,SWP_NOZORDER|SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_VALUE_11)->SetWindowText(strtext);
	}




	//}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CBacnetAnalogCusRang::OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
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


void CBacnetAnalogCusRang::OnEnKillfocusEditBacCusAnalogUnit()
{
	

	UpdateCusAnalogUnit();
}


void CBacnetAnalogCusRang::UpdateCusAnalogUnit()
{
	CString temp_cs;
	((CEdit *)GetDlgItem(IDC_EDIT_BAC_CUS_ANALOG_UNIT))->GetWindowTextW(temp_cs);
	if(temp_cs.GetLength() > 9)
	{
		MessageBox(_T("Nodes label length must between 1-9"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		return;
	}

	char cTemp1[255];
	memset(cTemp1,0,255);
	WideCharToMultiByte( CP_ACP, 0, temp_cs.GetBuffer(), -1, cTemp1, 255, NULL, NULL );

	memcpy_s(m_analog_custmer_range.at(analog_range_tbl_line).table_name,9,cTemp1,9);
	//m_analog_custmer_range.at(analog_range_tbl_line).table_name
	bool ret_check = false;
	ret_check = CheckAllDataValid();
	if(!ret_check)
	{
		MessageBox(_T("Please input a regular table"));
		return ;
	}
	CString temp_task_info;
	temp_task_info.Format(_T("Update Analog Range Unit : %s  "),temp_cs);
	Post_Write_Message(g_bac_instance,WRITEANALOG_CUS_TABLE_T3000,analog_range_tbl_line,analog_range_tbl_line,sizeof(Str_table_point),analog_cus_range_dlg ,temp_task_info);

}


void CBacnetAnalogCusRang::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	

	
}




void CBacnetAnalogCusRang::OnTimer(UINT_PTR nIDEvent)
{
	 
	switch(nIDEvent)
	{
	case 1:
		{
			//int voltage_up = -1;
			//int voltage_down = -1;

			//int value_up = -1;
			//int value_down = -1;
			//min_unit,max_unit);
			float real_voltage = ((float)m_Input_data.at(input_item_select_for_range).value)/1000;

			int temp_index_of = 0;
			for (int i=0;i<10;i++)
			{
				if((real_voltage >= m_analog_custmer_range.at(analog_range_tbl_line).dat[i].value/10  ) && 
					(real_voltage <= m_analog_custmer_range.at(analog_range_tbl_line).dat[i+1].value/10))
				{
					temp_index_of = i;
					break;
				}
			}

			float show_value = 0;
			if(voltage_up)
			{
				if(max_voltage != min_voltage)
				{
					//show_value =  ((float)(max_unit - min_unit))/((float)(max_voltage - min_voltage)/10) *  (real_voltage -((float)min_voltage)/10) + min_unit;
					float y2 = m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of+1].unit;
					float y1 = m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of].unit;
					float x2 = m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of+1].value;
					float x1 = m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of].value;
					float x3 = real_voltage*10;
					show_value = (y2 - y1)/(x2 - x1)*(x3 - x1) + y1;
					//show_value =  ((float)(m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of+1].unit - m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of].unit))/(((float)(m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of+1].value - m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of].value))/10) *  (real_voltage -((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of].value)/10) + m_analog_custmer_range.at(analog_range_tbl_line).dat[temp_index_of].value;
				}
			}
			CString temp_voltage;
			temp_voltage.Format(_T("%.2f"), real_voltage);
			((CStatic *)GetDlgItem(IDC_STATIC_CUS_ANALOG_VALUE))->SetWindowText(temp_voltage);
			//
			CString temp_unit;
			((CEdit *)GetDlgItem(IDC_EDIT_BAC_CUS_ANALOG_UNIT))->GetWindowTextW(temp_unit);
			CString ret_string;
			ret_string.Format(_T("IN%u  %.3f  %s"),input_item_select_for_range+1,show_value,temp_unit);
			((CEdit *)GetDlgItem(IDC_STATIC_CUS_RANGE_VALUE))->SetWindowText(ret_string);
			//TRACE(_T("%f\r\n"),show_value);
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}
