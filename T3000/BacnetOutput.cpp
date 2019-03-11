// BacnetOutput.cpp : implementation file
// Programming by Fance  2013 08 

#include "stdafx.h"
#include "T3000.h"
#include "BacnetOutput.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "global_function.h"
#include "global_define.h"
#include "BacnetRange.h"
#include "MainFrm.h"
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.
extern int initial_dialog;
CRect Output_rect;
static int show_output_external =  -1;
int OUTPUT_LIMITE_ITEM_COUNT = 0;
#define UPDATE_OUTPUT_ONE_ITEM_TIMER 3
int changed_output_item = -1; //// 用于改变某一列后 ，立即刷新 当前列的其他变化;

IMPLEMENT_DYNAMIC(CBacnetOutput, CDialogEx)

CBacnetOutput::CBacnetOutput(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetOutput::IDD, pParent)
{
  //m_latest_protocol=3;
	window_max = true;
}

CBacnetOutput::~CBacnetOutput()
{

}

void CBacnetOutput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_output_list);
	DDX_Control(pDX, IDC_STATIC_OUTPUT_ITEM_INFO, m_output_item_info);
}
BEGIN_MESSAGE_MAP(CBacnetOutput, CDialogEx)
	ON_MESSAGE(MY_RESUME_DATA, OutputMessageCallBack)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_READ, &CBacnetOutput::OnBnClickedButtonOutputRead)
	ON_MESSAGE(WM_REFRESH_BAC_OUTPUT_LIST,Fresh_Output_List)	
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Output_Item)	
	ON_NOTIFY(NM_CLICK, IDC_LIST_OUTPUT, &CBacnetOutput::OnNMClickListOutput)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_HELPINFO()
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()


// CBacnetOutput message handlers

LRESULT  CBacnetOutput::OutputMessageCallBack(WPARAM wParam, LPARAM lParam)
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
		if((pInvoke->mRow < BAC_OUTPUT_ITEM_COUNT) && (pInvoke->mRow >= 0))
		{
			Post_Refresh_One_Message(g_bac_instance,READOUTPUT_T3000,
				pInvoke->mRow,pInvoke->mRow,sizeof(Str_out_point));
			SetTimer(2,2000,NULL);
		}
		//Save_OutputData_to_db(pInvoke->mRow);
	}
	else
	{
		memcpy_s(&m_Output_data.at(pInvoke->mRow),sizeof(Str_out_point),&m_temp_output_data[pInvoke->mRow],sizeof(Str_out_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}

	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_output_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_output_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_output_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);

	if(pInvoke)
		delete pInvoke;
	return 0;
}


BOOL CBacnetOutput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowTextW(_T("OUTPUT"));
	
	m_output_item_info.SetWindowTextW(_T(""));
	m_output_item_info.textColor(RGB(0,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_output_item_info.setFont(20,14,NULL,_T("Arial"));

	Initial_List();
	PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_BUTTON_OUTPUT_READ))->SetIcon(hIcon);	
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_OK);
	((CButton *)GetDlgItem(IDC_BUTTON_OUTPUT_APPLY))->SetIcon(hIcon);
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);

	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_OUTPUT);
	SetIcon(m_hIcon,TRUE);

	ShowWindow(FALSE);
	//SetTimer(6,250,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetOutput::Reload_Unit_Type()
{
	 

	for (int i=0;i<(int)m_Output_data.size();i++)	//Initial All first.
	{
		if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_RANGE))
		{
			ListCtrlEx::CStrList strlist;
			for (int i=0;i<(int)sizeof(Units_Type)/sizeof(Units_Type[0]);i++)
			{
				strlist.push_back(Units_Type[i]);
			}
			m_output_list.SetCellStringList(i, OUTPUT_RANGE, strlist);		
		}
	}

	int d_initial_count = 0;
	int a_initial_count = 0;
	if(bacnet_device_type == BIG_MINIPANEL || bacnet_device_type == MINIPANELARM)	//Special Initial
	{	
		#ifdef NEED_ANALOG_DIGITAL_ONLY
		if(BIG_MINIPANEL_OUT_D > (int)m_Output_data.size()) 
			d_initial_count = (int)m_Output_data.size();
		else
			d_initial_count = BIG_MINIPANEL_OUT_D;

		a_initial_count = BIG_MINIPANEL_OUT_A;
		for (int i=0;i<d_initial_count;i++)
		{
			if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Digital_Only);
				m_output_list.SetCellStringList(i, OUTPUT_RANGE, strlist);		
			}
		}

		for (int i=d_initial_count;i< (d_initial_count + a_initial_count);i++ )
		{
			if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_output_list.SetCellStringList(i, OUTPUT_RANGE, strlist);		
			}
		}
		#endif
		m_output_list.SetColumnWidth(OUTPUT_HW_SWITCH,80);
	}
	else if(bacnet_device_type == SMALL_MINIPANEL || bacnet_device_type == MINIPANELARM_LB)
	{
		#ifdef NEED_ANALOG_DIGITAL_ONLY
		if(SMALL_MINIPANEL_OUT_D > (int)m_Output_data.size()) 
			d_initial_count = (int)m_Output_data.size();
		else
			d_initial_count = SMALL_MINIPANEL_OUT_D;

		a_initial_count = SMALL_MINIPANEL_OUT_A;
		for (int i=0;i<d_initial_count;i++)
		{
			if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Digital_Only);
				m_output_list.SetCellStringList(i, OUTPUT_RANGE, strlist);		
			}
		}

		for (int i=d_initial_count;i< (d_initial_count + a_initial_count);i++ )
		{
			if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_output_list.SetCellStringList(i, OUTPUT_RANGE, strlist);		
			}
		}
		#endif
		m_output_list.SetColumnWidth(OUTPUT_HW_SWITCH,80);
	}
	else if((bacnet_device_type == TINY_MINIPANEL) 
		   || ((bacnet_device_type == TINY_EX_MINIPANEL) || bacnet_device_type == MINIPANELARM_TB))
	{
#ifdef NEED_ANALOG_DIGITAL_ONLY
		if(TINY_MINIPANEL_OUT_D > (int)m_Output_data.size()) 
			d_initial_count = (int)m_Output_data.size();
		else
			d_initial_count = TINY_MINIPANEL_OUT_D;

		a_initial_count = TINY_MINIPANEL_OUT_A;
		for (int i=0;i<d_initial_count;i++)
		{
			if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Digital_Only);
				m_output_list.SetCellStringList(i, OUTPUT_RANGE, strlist);		
			}
		}

		for (int i=d_initial_count;i< (d_initial_count + a_initial_count);i++ )
		{
			if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_output_list.SetCellStringList(i, OUTPUT_RANGE, strlist);		
			}
		}
#endif
		m_output_list.SetColumnWidth(OUTPUT_HW_SWITCH,80);
	}
	else if(bacnet_device_type == PRODUCT_CM5)
	{
		#ifdef NEED_ANALOG_DIGITAL_ONLY
		d_initial_count = CM5_MINIPANEL_OUT_D;
		for (int i=0;i<d_initial_count;i++)
		{
			if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Digital_Only);
				m_output_list.SetCellStringList(i, OUTPUT_RANGE, strlist);		
			}
		}
		#endif
		m_output_list.SetColumnWidth(OUTPUT_HW_SWITCH,0);
	}

}


void CBacnetOutput::Initial_List()
{

	m_output_list.ShowWindow(SW_HIDE);
	m_output_list.DeleteAllItems();
	while ( m_output_list.DeleteColumn (0)) ;

	m_output_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_output_list.SetExtendedStyle(m_output_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_output_list.InsertColumn(OUTPUT_NUM, _T("Output"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_output_list.InsertColumn(OUTPUT_PANEL, _T("Panel"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_output_list.InsertColumn(OUTPUT_FULL_LABLE, _T("Full Label"), 140, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_AUTO_MANUAL, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_HW_SWITCH, _T("HOA Switch"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_VALUE, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_UNITE, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_RANGE, _T("Range"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	
	m_output_list.InsertColumn(OUTPUT_LOW_VOLTAGE, _T("Low V"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_HIGH_VOLTAGE, _T("High V"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	
	m_output_list.InsertColumn(OUTPUT_PWM_PERIOD, _T("PWM\r\n Period"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	


	m_output_list.InsertColumn(OUTPUT_DECOM, _T("Status"), 70, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_LABLE, _T("Label"), 70, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);

	m_output_list.InsertColumn(OUTPUT_EXTERNAL, _T("External"), 0, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_PRODUCT, _T("Product Name"), 0, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_EXT_NUMBER, _T("Product Output"), 0, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_output_list.Setlistcolcharlimit(OUTPUT_FULL_LABLE,STR_OUT_DESCRIPTION_LENGTH -1);
	m_output_list.Setlistcolcharlimit(OUTPUT_LABLE,STR_OUT_LABEL-1);

	m_output_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_output_dlg_hwnd;
	m_output_list.GetClientRect(Output_rect);
	m_output_list.SetListHwnd(this->m_hWnd);
	for (int i=0;i<(int)m_Output_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;


		if(i>=output_item_limit_count)
			break;

		temp_item.Format(_T("OUT%d"),i+1);
		m_output_list.InsertItem(i,temp_item);


		if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_RANGE))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa=0;aaa<(int)sizeof(Units_Type)/sizeof(Units_Type[0]);aaa++)
			{
				strlist.push_back(Units_Type[aaa]);
			}
			m_output_list.SetCellStringList(i, OUTPUT_RANGE, strlist);		
		}

		if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_DECOM))
		{
			ListCtrlEx::CStrList strlist;
			for (int m=0;m<(int)(sizeof(Output_Decom_Array)/sizeof(Output_Decom_Array[0]));m++)
			{
				strlist.push_back(Output_Decom_Array[m]);
			}
			m_output_list.SetCellStringList(i, OUTPUT_DECOM, strlist);
		}

		for (int x=0;x<OUTPUT_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_output_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_output_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

	}
	m_output_list.InitListData();
	m_output_list.ShowWindow(SW_SHOW);
}



void CBacnetOutput::OnBnClickedButtonOutputRead()
{
	Post_Refresh_Message(g_bac_instance,READOUTPUT_T3000,0,BAC_OUTPUT_ITEM_COUNT - 1,sizeof(Str_out_point),BAC_OUTPUT_GROUP);
		PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
}

       // PostMessage(g_hwnd_now,WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);

LRESULT CBacnetOutput::Fresh_Output_List(WPARAM wParam,LPARAM lParam)
{
    bool need_refresh_all = false; 
	int Fresh_Item;
	int isFreshOne = (bool)lParam;
	int  Minipanel_device = 1;
	int Fresh_List_Now = (int)wParam;
	if( Fresh_List_Now == REFRESH_LIST_NOW)
	{
		need_refresh_all = true;
	}

	int digital_special_output_count = 0;
	int analog_special_output_count = 0;
	if (bacnet_device_type == BIG_MINIPANEL || bacnet_device_type == MINIPANELARM)
	{
		digital_special_output_count = BIG_MINIPANEL_OUT_D;
		analog_special_output_count = BIG_MINIPANEL_OUT_A;
		Minipanel_device = 1;
	}
	else if(bacnet_device_type == SMALL_MINIPANEL || bacnet_device_type == MINIPANELARM_LB)
	{
		digital_special_output_count = SMALL_MINIPANEL_OUT_D;
		analog_special_output_count = SMALL_MINIPANEL_OUT_A;
		Minipanel_device = 1;
	}
	else if(bacnet_device_type == TINY_MINIPANEL)
	{
		digital_special_output_count = TINY_MINIPANEL_OUT_D;
		analog_special_output_count = TINY_MINIPANEL_OUT_A;
		Minipanel_device = 1;
	}
	else if (bacnet_device_type == TINY_EX_MINIPANEL || bacnet_device_type == MINIPANELARM_TB)
	{
		digital_special_output_count = TINYEX_MINIPANEL_OUT_D;
		analog_special_output_count = TINYEX_MINIPANEL_OUT_A;
		Minipanel_device = 1;
	}
	else if(bacnet_device_type == T38AI8AO6DO)
	{
		digital_special_output_count = T38AI8AO6DO_OUT_D;
		analog_special_output_count = T38AI8AO6DO_OUT_A;
        OUTPUT_LIMITE_ITEM_COUNT = digital_special_output_count + analog_special_output_count;
		Minipanel_device = 0;
	}
	else if (bacnet_device_type == PID_T322AI)
	{
		digital_special_output_count = T322AI_OUT_D;
		analog_special_output_count = T322AI_OUT_A;
        OUTPUT_LIMITE_ITEM_COUNT = digital_special_output_count + analog_special_output_count;
		Minipanel_device = 0;
	}
	else if (bacnet_device_type == PWM_TRANSDUCER)
	{
		digital_special_output_count = PWM_TRANSDUCER_OUT_D;
		analog_special_output_count = PWM_TRANSDUCER_OUT_A;
		Minipanel_device = 0;
	}

	if((bacnet_device_type == T38AI8AO6DO) ||
		(bacnet_device_type == PID_T322AI))
	{
		OUTPUT_LIMITE_ITEM_COUNT = digital_special_output_count + analog_special_output_count;
	}
	else if((bacnet_device_type == PID_T322AI) ||
		    (bacnet_device_type == PID_T3PT12))
	{
		OUTPUT_LIMITE_ITEM_COUNT = 0;
	}
	else if (bacnet_device_type == PM_T3_LC)
	{
		OUTPUT_LIMITE_ITEM_COUNT = 8;
	}
	else if ((bacnet_device_type == PID_T36CTA))
	{
		OUTPUT_LIMITE_ITEM_COUNT = 2;
	}
	else if (bacnet_device_type == TINY_EX_MINIPANEL)
	{
		//OUTPUT_LIMITE_ITEM_COUNT = 14;
	}
    else if (bacnet_device_type == PWM_TRANSDUCER)
    {
        OUTPUT_LIMITE_ITEM_COUNT = digital_special_output_count + analog_special_output_count;
    }
	else
	{
		OUTPUT_LIMITE_ITEM_COUNT = BAC_OUTPUT_ITEM_COUNT;
	}

	int temp_need_show_external = 0;
	for (int z= 0 ;z < (int)m_Output_data.size();z++)
	{
		if(z>= OUTPUT_LIMITE_ITEM_COUNT)
		{
			m_output_list.SetItemText(z,0,_T(""));
			continue;
		}
		if((m_Output_data.at(z).sub_id !=0) &&
			(m_Output_data.at(z).sub_product !=0))
		{
			temp_need_show_external = 1;
			break;
		}

	}
	if(show_output_external != temp_need_show_external)
	{
		show_output_external = temp_need_show_external;
		if(temp_need_show_external)
		{
			//CRect temp_rect;
			//temp_rect = Output_rect;
			//temp_rect.right = 1150;
			//temp_rect.top = temp_rect.top + 24;
			//m_output_list.MoveWindow(temp_rect);
			m_output_list.SetColumnWidth(OUTPUT_EXTERNAL,60);
			m_output_list.SetColumnWidth(OUTPUT_PRODUCT,80);
			m_output_list.SetColumnWidth(OUTPUT_EXT_NUMBER,80);
		}
		else
		{
			//CRect temp_rect;
			//temp_rect = Output_rect;
			//temp_rect.right = 950;
			//temp_rect.top = temp_rect.top + 24;
			//m_output_list.MoveWindow(temp_rect);

			m_output_list.SetColumnWidth(OUTPUT_EXTERNAL,0);
			m_output_list.SetColumnWidth(OUTPUT_PRODUCT,0);
			m_output_list.SetColumnWidth(OUTPUT_EXT_NUMBER,0);
		}
	}
	if(Minipanel_device == 0)	//如果不是minipanel的界面就隐藏扩展行;
	{
		//CRect temp_rect;
		//temp_rect = Output_rect;
		//temp_rect.right = 950;
		//temp_rect.top = temp_rect.top + 24;
		//m_output_list.MoveWindow(temp_rect);

		m_output_list.SetColumnWidth(OUTPUT_EXTERNAL,0);
		m_output_list.SetColumnWidth(OUTPUT_PRODUCT,0);
		m_output_list.SetColumnWidth(OUTPUT_EXT_NUMBER,0);
	}


	if(need_refresh_all == false)
	{
		if(isFreshOne == REFRESH_ON_ITEM)
		{
			Fresh_Item = (int)wParam;
		}
		else
		{
			if(m_output_list.IsDataNewer((char *)&m_Output_data.at(0),sizeof(Str_out_point) * BAC_OUTPUT_ITEM_COUNT))
			{
				//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
				m_output_list.SetListData((char *)&m_Output_data.at(0),sizeof(Str_out_point) * BAC_OUTPUT_ITEM_COUNT);
			}
			else
			{
				return 0;
			}
		}
	}



	CString temp1;
	for (int i=0;i<(int)m_Output_data.size();i++)
	{
		if(isFreshOne)
		{
			i = Fresh_Item;
            if (i >= m_Output_data.size())
                return 1;
		}

		if(i>=output_item_limit_count)
			break;

		if(i>= OUTPUT_LIMITE_ITEM_COUNT)
		{
			for (int a=0;a<OUTPUT_COL_NUMBER; a++)
			{
				m_output_list.SetItemText(i,a,_T(""));
			}
			continue;
		}
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		CString low_voltage,high_voltage;

		MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(i).description, (int)strlen((char *)m_Output_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		temp_item.Format(_T("OUT%d"),i+1);
		m_output_list.SetItemText(i,OUTPUT_NUM,temp_item);
		m_output_list.SetItemText(i,OUTPUT_FULL_LABLE,temp_des);
		m_output_list.SetCellEnabled(i,OUTPUT_HW_SWITCH,0);
		m_output_list.SetItemText(i,OUTPUT_HW_SWITCH,_T(""));

		if(m_Output_data.at(i).low_voltage == 0)
			low_voltage.Empty();
		else
			low_voltage.Format(_T("%.1f"),m_Output_data.at(i).low_voltage/10);

		if(m_Output_data.at(i).high_voltage == 0)
			high_voltage.Empty();
		else
			high_voltage.Format(_T("%.1f"),m_Output_data.at(i).high_voltage/10);
		
		m_output_list.SetItemText(i,OUTPUT_LOW_VOLTAGE,low_voltage);	
		m_output_list.SetItemText(i,OUTPUT_HIGH_VOLTAGE,high_voltage);	
		if(
			(bacnet_device_type == BIG_MINIPANEL || bacnet_device_type == MINIPANELARM || bacnet_device_type == MINIPANELARM_LB || bacnet_device_type == MINIPANELARM_TB)
			|| ((bacnet_device_type == SMALL_MINIPANEL)) || (bacnet_device_type == TINY_MINIPANEL) || (bacnet_device_type == TINY_EX_MINIPANEL) ||
			(bacnet_device_type == T38AI8AO6DO) || (bacnet_device_type == PID_T322AI) || (bacnet_device_type == PWM_TRANSDUCER)
            )
		{
			if(i < (digital_special_output_count +analog_special_output_count) )
			{
				if(m_Output_data.at(i).hw_switch_status == HW_SW_OFF)
				{
					m_output_list.SetItemText(i,OUTPUT_HW_SWITCH,_T("MAN-OFF"));		
					m_output_list.SetCellEnabled(i,OUTPUT_AUTO_MANUAL,0);

					m_output_list.SetItemTextColor(i,-1,RGB(255,0,0),0);
				}
				else if(m_Output_data.at(i).hw_switch_status == HW_SW_HAND)
				{
					m_output_list.SetItemText(i,OUTPUT_HW_SWITCH,_T("MAN-ON"));
					m_output_list.SetCellEnabled(i,OUTPUT_AUTO_MANUAL,0);

					m_output_list.SetItemTextColor(i,-1,RGB(255,0,0),0);
				}
				else
				{
					m_output_list.SetItemTextColor(i,-1,RGB(0,0,0),0);

					m_output_list.SetItemText(i,OUTPUT_HW_SWITCH,_T("AUTO"));
					m_output_list.SetCellEnabled(i,OUTPUT_AUTO_MANUAL,1);
					if(m_Output_data.at(i).auto_manual==0)	//In output table if it is auto ,the value can't be edit by user
					{
						m_output_list.SetItemText(i,OUTPUT_AUTO_MANUAL,_T("Auto"));
						//m_output_list.SetCellEnabled(i,OUTPUT_VALUE,0);
					}
					else
					{
						m_output_list.SetItemText(i,OUTPUT_AUTO_MANUAL,_T("Manual"));
						//m_output_list.SetCellEnabled(i,OUTPUT_VALUE,1);
					}
				}
			}
			else
			{
				m_output_list.SetItemTextColor(i,-1,RGB(0,0,0),0);
				m_output_list.SetItemText(i,OUTPUT_HW_SWITCH,_T("AUTO"));
				m_output_list.SetCellEnabled(i,OUTPUT_AUTO_MANUAL,1);
				if(m_Output_data.at(i).auto_manual==0)	//In output table if it is auto ,the value can't be edit by user
				{
					m_output_list.SetItemText(i,OUTPUT_AUTO_MANUAL,_T("Auto"));
					//m_output_list.SetCellEnabled(i,OUTPUT_VALUE,0);
				}
				else
				{
					m_Output_data.at(i).auto_manual = 1;
					m_output_list.SetItemText(i,OUTPUT_AUTO_MANUAL,_T("Manual"));
					//m_output_list.SetCellEnabled(i,OUTPUT_VALUE,1);
				}
			}
		}
		else
		{
			m_output_list.SetCellEnabled(i,OUTPUT_AUTO_MANUAL,1);
			if(m_Output_data.at(i).auto_manual==0)	//In output table if it is auto ,the value can't be edit by user
			{
				m_output_list.SetItemText(i,OUTPUT_AUTO_MANUAL,_T("Auto"));
				//m_output_list.SetCellEnabled(i,OUTPUT_VALUE,0);
			}
			else
			{
				m_Output_data.at(i).auto_manual = 1;
				m_output_list.SetItemText(i,OUTPUT_AUTO_MANUAL,_T("Manual"));
				//m_output_list.SetCellEnabled(i,OUTPUT_VALUE,1);
			}
		}




			m_output_list.SetCellEnabled(i,OUTPUT_UNITE,0);
		if(m_Output_data.at(i).digital_analog == BAC_UNITS_ANALOG)
		{
			m_output_list.SetCellEnabled(i,OUTPUT_LOW_VOLTAGE,1);
			m_output_list.SetCellEnabled(i,OUTPUT_HIGH_VOLTAGE,1);
			m_output_list.SetCellEnabled(i,OUTPUT_PWM_PERIOD,1);
			if(m_Output_data.at(i).range == 0)
				m_output_list.SetItemText(i,OUTPUT_RANGE,_T("Unused"));
			else if(m_Output_data.at(i).range < (sizeof(OutPut_List_Analog_Range)/sizeof(OutPut_List_Analog_Range[0])))
				m_output_list.SetItemText(i,OUTPUT_RANGE,OutPut_List_Analog_Range[m_Output_data.at(i).range]);
			else
				m_output_list.SetItemText(i,OUTPUT_RANGE,_T("Out of range"));

			if(m_Output_data.at(i).range < (sizeof(OutPut_List_Analog_Units)/sizeof(OutPut_List_Analog_Units[0])))
				m_output_list.SetItemText(i,OUTPUT_UNITE,OutPut_List_Analog_Units[m_Output_data.at(i).range]);
			else
				m_output_list.SetItemText(i,OUTPUT_UNITE,_T("Unused"));

			CString temp_low,temp_high;
			temp_low.Format(_T("%d"),0);
			temp_high.Format(_T("%d"),0);


			CString temp_value;
			temp_value.Format(_T("%.2f"),((float)m_Output_data.at(i).value) / 1000);
			m_output_list.SetItemText(i,OUTPUT_VALUE,temp_value);

		}
		else if(m_Output_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{
			m_output_list.SetCellEnabled(i,OUTPUT_LOW_VOLTAGE,0);
			m_output_list.SetCellEnabled(i,OUTPUT_HIGH_VOLTAGE,0);
			m_output_list.SetCellEnabled(i,OUTPUT_PWM_PERIOD,0);
			m_output_list.SetItemText(i,OUTPUT_UNITE,_T(""));

			if(m_Output_data.at(i).range == 0)
			{
				CString temp_value2;
				temp_value2.Format(_T("%.2f"),((float)m_Output_data.at(i).value) / 1000);
				m_output_list.SetItemText(i,OUTPUT_VALUE,temp_value2);

				m_output_list.SetItemText(i,OUTPUT_RANGE,Digital_Units_Array[0]);
			}
			else if(m_Output_data.at(i).range<=22)
				m_output_list.SetItemText(i,OUTPUT_RANGE,Digital_Units_Array[m_Output_data.at(i).range]);
			else if((m_Output_data.at(i).range >= 23) && (m_Output_data.at(i).range <= 30))
			{
				if(receive_customer_unit)
					m_output_list.SetItemText(i,OUTPUT_RANGE,Custom_Digital_Range[m_Output_data.at(i).range - 23]);
				else
					m_output_list.SetItemText(i,OUTPUT_RANGE,Digital_Units_Array[0]);
			}
			else
				m_output_list.SetItemText(i,OUTPUT_RANGE,Digital_Units_Array[0]);
#if 0
			if(m_Output_data.at(i).range>=12)
				m_output_list.SetItemText(i,OUTPUT_RANGE,Digital_Units_Array[m_Output_data.at(i).range -11]);
			else
				m_output_list.SetItemText(i,OUTPUT_RANGE,Digital_Units_Array[m_Output_data.at(i).range]);
#endif
			if((m_Output_data.at(i).range>30) || (m_Output_data.at(i).range == 0))
			{
				//m_output_list.SetItemText(i,OUTPUT_UNITE,Digital_Units_Array[0]);
			}
			else
			{
				
				CStringArray temparray;

				if((m_Output_data.at(i).range < 23) &&(m_Output_data.at(i).range !=0))
					temp1 = Digital_Units_Array[m_Output_data.at(i).range];
				else if((m_Output_data.at(i).range >=23) && (m_Output_data.at(i).range <= 30))
				{
					if(receive_customer_unit)
						temp1 = Custom_Digital_Range[m_Output_data.at(i).range - 23];
				}
				else
				{
					temp1.Empty();
					m_output_list.SetItemText(i,OUTPUT_VALUE,_T(""));
				}

				SplitCStringA(temparray,temp1,_T("/"));
				if((temparray.GetSize()==2))
				{
					if(m_Output_data.at(i).control == 0)
						m_output_list.SetItemText(i,OUTPUT_VALUE,temparray.GetAt(0));
					else
						m_output_list.SetItemText(i,OUTPUT_VALUE,temparray.GetAt(1));
				}
			}
		}


			CString main_sub_panel;

#pragma region External info
		if((m_Output_data.at(i).sub_id !=0) &&
			//(m_Input_data.at(input_list_line).sub_number !=0) &&
			(m_Output_data.at(i).sub_product !=0) &&
			Minipanel_device == 1)
		{
			unsigned char temp_pid = m_Output_data.at(i).sub_product;
			if((temp_pid == PM_T3PT10) ||
				(temp_pid == PM_T3IOA) ||
				(temp_pid == PM_T332AI) ||
				(temp_pid == PM_T38AI16O) ||
				(temp_pid == PM_T38I13O) ||
				(temp_pid == PM_T34AO) ||
				(temp_pid == PM_T322AI) ||
				(temp_pid == PM_T38AI8AO6DO) ||
				(temp_pid == PM_T36CT) ||
				(temp_pid == PM_T36CTA)||
				(temp_pid == PM_T3_LC)
				)
			{
				CString temp_name;
				temp_name = GetProductName(m_Output_data.at(i).sub_product);
				CString show_info;
				CString temp_id;
				CString temp_number;

				bitset<8> mytest((unsigned char)m_Output_data.at(i).sub_number);
				bool highest_bit;
				highest_bit = mytest.test(7);

				unsigned char temp_sub_number = 0;
				mytest = mytest.set(7,0);
				temp_sub_number = (unsigned char)mytest.to_ulong();


				if(highest_bit)
					temp_number.Format(_T("AO%d"),temp_sub_number + 1);
				else
					temp_number.Format(_T("DO%d"),temp_sub_number + 1);


				if(m_Output_data.at(i).hw_switch_status == HW_SW_OFF)
				{
					m_output_list.SetItemText(i,OUTPUT_HW_SWITCH,_T("MAN-OFF"));		
					//m_output_list.SetCellEnabled(i,OUTPUT_AUTO_MANUAL,0);

					m_output_list.SetItemTextColor(i,-1,RGB(255,0,0),0);
				}
				else if(m_Output_data.at(i).hw_switch_status == HW_SW_HAND)
				{
					m_output_list.SetItemText(i,OUTPUT_HW_SWITCH,_T("MAN-ON"));
					//m_output_list.SetCellEnabled(i,OUTPUT_AUTO_MANUAL,0);

					m_output_list.SetItemTextColor(i,-1,RGB(255,0,0),0);
				}
				else
				{
					m_output_list.SetItemTextColor(i,-1,RGB(0,0,0),0);

					m_output_list.SetItemText(i,OUTPUT_HW_SWITCH,_T("AUTO"));
					//m_output_list.SetCellEnabled(i,OUTPUT_AUTO_MANUAL,1);
				}

				
				m_output_list.SetItemText(i,OUTPUT_EXTERNAL,_T("External"));
				m_output_list.SetItemTextColor(i,OUTPUT_EXTERNAL,RGB(0,0,255),FALSE);
				m_output_list.SetItemText(i,OUTPUT_PRODUCT,temp_name);
				m_output_list.SetItemText(i,OUTPUT_EXT_NUMBER,temp_number);

				main_sub_panel.Format(_T("%d-%d"),(unsigned char)Station_NUM,(unsigned char)m_Output_data.at(i).sub_id);
				m_output_list.SetItemText(i,OUTPUT_PANEL,main_sub_panel);


			}	
			else
			{
				main_sub_panel.Format(_T("%d"),(unsigned char)Station_NUM);
				m_output_list.SetItemText(i,OUTPUT_PANEL,main_sub_panel);
			}

		}
		else
		{
			
			m_output_list.SetItemText(i,OUTPUT_PANEL,Statuspanel);
			//main_sub_panel.Format(_T("%d"),(unsigned char)Station_NUM);
			//m_output_list.SetItemText(i,OUTPUT_PANEL,main_sub_panel);

			m_output_list.SetItemText(i,OUTPUT_EXTERNAL,_T(""));
			m_output_list.SetItemText(i,OUTPUT_PRODUCT,_T(""));
			m_output_list.SetItemText(i,OUTPUT_EXT_NUMBER,_T(""));
		}

#pragma endregion External info




		if(m_Output_data.at(i).decom==0)
			temp_status.Format(Output_Decom_Array[0]);
		else if(m_Output_data.at(i).decom==1)
			temp_status.Format(Output_Decom_Array[1]);
		else
			temp_status.Empty();
		m_output_list.SetItemText(i,OUTPUT_DECOM,temp_status);

		CString temp_pwm_period;
		temp_pwm_period.Format(_T("%u"),(unsigned char)m_Output_data.at(i).pwm_period);
		m_output_list.SetItemText(i,OUTPUT_PWM_PERIOD,temp_pwm_period);

		CString temp_des2;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(i).label, (int)strlen((char *)m_Output_data.at(i).label)+1, 
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des2.ReleaseBuffer();
		m_output_list.SetItemText(i,OUTPUT_LABLE,temp_des2);
		if(isFreshOne)
		{
			break;
		}
	}
	copy_data_to_ptrpanel(TYPE_OUTPUT);
	//Invalidate();
	return 0;
}



LRESULT CBacnetOutput::Fresh_Output_Item(WPARAM wParam,LPARAM lParam)
{
	 
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	CString temp_task_info;
	CString New_CString =  m_output_list.GetItemText(Changed_Item,Changed_SubItem);
	CString cstemp_value;


	if(Changed_Item>= OUTPUT_LIMITE_ITEM_COUNT)
	{
		m_output_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
		return 0;
	}

	//先保存 原来的值，等结束的时候来比对，看是否有改变，有改变就进行写动作;
	memcpy_s(&m_temp_output_data[Changed_Item],sizeof(Str_out_point),&m_Output_data.at(Changed_Item),sizeof(Str_out_point));

	if(Changed_SubItem == OUTPUT_PWM_PERIOD)
	{
		CString temp_cs = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
		int temp_int = (int)(_wtof(temp_cs));
		if((temp_int<0) || (temp_int >=256))
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Data out of range!"));
			PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		m_Output_data.at(Changed_Item).pwm_period = (unsigned char)temp_int;
	}

	if(Changed_SubItem == OUTPUT_LABLE)
	{
		CString cs_temp = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_OUT_LABEL)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 8"),_T("Warning"));
#pragma region note_what_do	
			//如下所做的事 达到 ,在弹窗后 继续选中客户输入的部分 ，毛总的要求;
			CRect list_rect,win_rect;
			m_output_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(m_output_dlg_hwnd,win_rect);
			m_output_list.Set_My_WindowRect(win_rect);
			m_output_list.Set_My_ListRect(list_rect);

			m_output_list.Get_clicked_mouse_position();
#pragma endregion note_what_do



			PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
			return 0;
		}
		cs_temp.Replace(_T("-"),_T("_"));
		cs_temp.MakeUpper();
		if(Check_Label_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Output_data.at(Changed_Item).label,STR_OUT_LABEL,cTemp1,STR_OUT_LABEL);
	}

	if(Changed_SubItem == OUTPUT_FULL_LABLE)
	{
		CString cs_temp = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_OUT_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 18"),_T("Warning"));
#pragma region note_what_do	
			//如下所做的事 达到 ,在弹窗后 继续选中客户输入的部分 ，毛总的要求;
			CRect list_rect,win_rect;
			m_output_list.GetWindowRect(list_rect);
			ScreenToClient(&list_rect);
			::GetWindowRect(m_output_dlg_hwnd,win_rect);
			m_output_list.Set_My_WindowRect(win_rect);
			m_output_list.Set_My_ListRect(list_rect);

			m_output_list.Get_clicked_mouse_position();
#pragma endregion note_what_do
			PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
			return 0;
		}
		if(Check_FullLabel_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Output_data.at(Changed_Item).description,STR_OUT_DESCRIPTION_LENGTH,cTemp1,STR_OUT_DESCRIPTION_LENGTH);
	}

	if(Changed_SubItem==OUTPUT_AUTO_MANUAL)	//If auto man changed to mannul , the value can be edit by user
	{
		CString temp_cs = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(_T("Auto"))==0)
		{
			//m_output_list.SetCellEnabled(Changed_Item,OUTPUT_VALUE,0);
			m_Output_data.at(Changed_Item).auto_manual = BAC_AUTO ;
		}
		else
		{
			//m_output_list.SetCellEnabled(Changed_Item,OUTPUT_VALUE,1);
			m_Output_data.at(Changed_Item).auto_manual = BAC_MANUAL ;
		}
	}

	if(Changed_SubItem == OUTPUT_VALUE)
	{
		CString temp_cs = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
		int temp_int = (int)(_wtof(temp_cs) * 1000);
		m_Output_data.at(Changed_Item).value = temp_int;
	}


	if(Changed_SubItem == OUTPUT_DECOM)
	{
		CString temp_cs = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(_T("OK"))==0)
		{
			m_Output_data.at(Changed_Item).decom = BAC_DECOM_YES ;
		}
		else if(temp_cs.CompareNoCase(_T("-"))==0)
		{
			m_Output_data.at(Changed_Item).decom = BAC_DECOM_NO ;
		}
	}


	if(Changed_SubItem == OUTPUT_LOW_VOLTAGE)
	{
		int temp_int = (int)(_wtof(New_CString) * 10);
		if(temp_int> 120)
		{
			MessageBox(_T("Voltage can not greater than 12"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0 ;
		}
		m_Output_data.at(Changed_Item).low_voltage = temp_int;
	}

	if(Changed_SubItem == OUTPUT_HIGH_VOLTAGE)
	{
		int temp_int = (int)(_wtof(New_CString) * 10);
		if(temp_int> 120)
		{
			MessageBox(_T("Voltage can not greater than 12"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		m_Output_data.at(Changed_Item).high_voltage = temp_int;
	}

	cmp_ret = memcmp(&m_temp_output_data[Changed_Item],&m_Output_data.at(Changed_Item),sizeof(Str_out_point));
	if(cmp_ret!=0)
	{
		m_output_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Output List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEOUTPUT_T3000,Changed_Item,Changed_Item,sizeof(Str_out_point),m_output_dlg_hwnd,temp_task_info,Changed_Item,Changed_SubItem);
	}


	return 0;
}


void CBacnetOutput::OnNMClickListOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	 
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	long lRow,lCol;
	m_output_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_output_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_output_list.SubItemHitTest(&lvinfo);
	

	 (CMainFrame*)(AfxGetApp()->m_pMainWnd)->SetActiveWindow();
	if((nItem!=-1) && (nItem < BAC_OUTPUT_ITEM_COUNT))
	{
		output_list_line = nItem;
		if((m_Output_data.at(output_list_line).sub_id !=0) &&
			//(m_Output_data.at(output_list_line).sub_number !=0) &&
			(m_Output_data.at(output_list_line).sub_product !=0))
		{
			unsigned char temp_pid = m_Output_data.at(output_list_line).sub_product;
			if((temp_pid == PM_T3PT10) ||
				(temp_pid == PM_T3IOA) ||
				(temp_pid == PM_T332AI) ||
				(temp_pid == PM_T38AI16O) ||
				(temp_pid == PM_T38I13O) ||
				(temp_pid == PM_T34AO) ||
				(temp_pid == PM_T36CT) ||
				(temp_pid == PM_T322AI) ||
				(temp_pid == PM_T38AI8AO6DO)||
				(temp_pid == PM_T36CTA)||
				(temp_pid == PM_T3_LC)
				)
			{
				//m_output_item_info.ShowWindow(true); //由于改成全屏的 支持resize 的显示，所以先屏蔽;
				CString temp_name;
				temp_name = GetProductName(m_Output_data.at(output_list_line).sub_product);
				CString show_info;
				CString temp_id;
				CString temp_number;
				temp_id.Format(_T(" Sub ID: %u        "),(unsigned char)m_Output_data.at(output_list_line).sub_id);
				temp_number.Format(_T("Output%d"),(unsigned char)m_Output_data.at(output_list_line).sub_number+1);
				show_info = _T("Module:") + temp_name +_T("        ") + temp_id + temp_number;
				m_output_item_info.SetWindowTextW(show_info);

			}	
			else
			{
				m_output_item_info.ShowWindow(false);
			}
		}
		else
		{
			m_output_item_info.ShowWindow(false);
		}
	}

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;

	if(lRow>= OUTPUT_LIMITE_ITEM_COUNT)
		return;

	if(lRow>m_output_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;

	CString New_CString;
	CString temp_task_info;
	CString temp1;
	CStringArray temparray;

	if(lCol == OUTPUT_VALUE)
	{
		if(m_Output_data.at(lRow).auto_manual == BAC_AUTO)	//If it is auto mode, disable to change the value.
		{
			m_output_list.Set_Edit(false);
			return;
		}

		if(m_Output_data.at(lRow).digital_analog != BAC_UNITS_DIGITAL)
			return;



		memcpy_s(&m_temp_output_data[lRow],sizeof(Str_out_point),&m_Output_data.at(lRow),sizeof(Str_out_point));





		if((m_Output_data.at(lRow).range < 23) &&(m_Output_data.at(lRow).range !=0))
			temp1 = Digital_Units_Array[m_Output_data.at(lRow).range];
		else if((m_Output_data.at(lRow).range >=23) && (m_Output_data.at(lRow).range <= 30))
		{
			if(receive_customer_unit)
				temp1 = Custom_Digital_Range[m_Output_data.at(lRow).range - 23];
			else
			{
				m_output_list.Set_Edit(false);
				return;
			}
		}
		else
			return;


		SplitCStringA(temparray,temp1,_T("/"));
		if(m_Output_data.at(lRow).control == 0)
		{
			m_Output_data.at(lRow).control = 1;
			m_output_list.SetItemText(lRow,OUTPUT_VALUE,temparray.GetAt(1));
			New_CString = temparray.GetAt(1);
		}
		else
		{
			m_Output_data.at(lRow).control = 0;
			m_output_list.SetItemText(lRow,OUTPUT_VALUE,temparray.GetAt(0));
			New_CString = temparray.GetAt(0);
		}
	}
	else if(lCol == OUTPUT_AUTO_MANUAL)
	{
		memcpy_s(&m_temp_output_data[lRow],sizeof(Str_out_point),&m_Output_data.at(lRow),sizeof(Str_out_point));
		if(m_Output_data.at(lRow).auto_manual == 0)
		{
			m_Output_data.at(lRow).auto_manual = 1;
			m_output_list.SetItemText(lRow,OUTPUT_AUTO_MANUAL,_T("Manual"));
			New_CString = _T("Manual");
		}
		else
		{
			m_Output_data.at(lRow).auto_manual = 0;
			m_output_list.SetItemText(lRow,OUTPUT_AUTO_MANUAL,_T("Auto"));
			New_CString = _T("Auto");
		}
	}
	else if(lCol == OUTPUT_RANGE)
	{



			//CString temp_cs = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
			BacnetRange dlg;
			//点击产品的时候 需要读custom units，老的产品firmware 说不定没有 这些，所以不强迫要读到;
			if(!read_customer_unit)
			{

				int temp_invoke_id = -1;
				int send_status = true;
				int	resend_count = 0;
				for (int z=0;z<3;z++)
				{
					do 
					{
						resend_count ++;
						if(resend_count>5)
						{
							send_status = false;
							break;
						}
						temp_invoke_id =  GetPrivateData(
							g_bac_instance,
							READUNIT_T3000,
							0,
							BAC_CUSTOMER_UNITS_COUNT - 1,
							sizeof(Str_Units_element));		

						Sleep(SEND_COMMAND_DELAY_TIME);
					} while (g_invoke_id<0);
					if(send_status)
					{
						for (int z=0;z<1000;z++)
						{
							Sleep(1);
							if(tsm_invoke_id_free(temp_invoke_id))
							{
								read_customer_unit = true;
								break;
							}
							else
								continue;
						}

					}
					if(read_customer_unit)
						break;
				}


			}

			if(m_Output_data.at(lRow).digital_analog == BAC_UNITS_ANALOG)
			{
				bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
				if(m_Output_data.at(lRow).range > (sizeof(Output_Analog_Units_Array) / sizeof(Output_Analog_Units_Array[0])))
				{
					m_Output_data.at(lRow).range = 0;
					bac_range_number_choose = 0;
				}
			}
			else
			{
				bac_ranges_type = OUTPUT_RANGE_DIGITAL_TYPE;
				if(m_Output_data.at(lRow).range > 30)
				{
					m_Output_data.at(lRow).range = 0;
					bac_range_number_choose = 0;
				}
			}


			//if(temp_cs.CompareNoCase(Units_Type[UNITS_TYPE_ANALOG])==0)
			//{
				initial_dialog = 3;
				bac_range_number_choose = m_Output_data.at(lRow).range;
				//bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
				dlg.DoModal();
				if(range_cancel)
				{
					PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,lRow,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
					return ;
				}
				if(bac_range_number_choose == 0)	//如果选择的是 unused 就认为是analog 的unused;这样 能显示对应的value;
				{
					m_Output_data.at(lRow).digital_analog =  BAC_UNITS_ANALOG;
					bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
					m_output_list.SetCellEnabled(lRow,OUTPUT_PWM_PERIOD,1);
					m_output_list.SetCellEnabled(lRow,OUTPUT_LOW_VOLTAGE,1);
					m_output_list.SetCellEnabled(lRow,OUTPUT_HIGH_VOLTAGE,1);
				}

				if(bac_ranges_type == OUTPUT_RANGE_ANALOG_TYPE)
				{
					m_Output_data.at(lRow).digital_analog =  BAC_UNITS_ANALOG;
					m_Output_data.at(lRow).range =  bac_range_number_choose;
					m_output_list.SetItemText(lRow,OUTPUT_UNITE,Output_Analog_Units_Show[bac_range_number_choose]);		
					m_output_list.SetItemText(lRow,OUTPUT_RANGE,OutPut_List_Analog_Range[bac_range_number_choose]);	
					m_output_list.SetCellEnabled(lRow,OUTPUT_PWM_PERIOD,1);
					m_output_list.SetCellEnabled(lRow,OUTPUT_LOW_VOLTAGE,1);
					m_output_list.SetCellEnabled(lRow,OUTPUT_HIGH_VOLTAGE,1);

					CString cstemp_value;
					cstemp_value.Format(_T("%.2f"),((float)m_Output_data.at(lRow).value) / 1000);
					m_output_list.SetItemText(lRow,OUTPUT_VALUE,cstemp_value);	
				}
				else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE))
				{
					m_Output_data.at(lRow).digital_analog =  BAC_UNITS_DIGITAL;
					m_Output_data.at(lRow).range =  bac_range_number_choose;

					CStringArray temparray;

					if((bac_range_number_choose >= 23) && (bac_range_number_choose <= 30))
					{
						//temp1.Format(_T("%s"), Custom_Digital_Range[bac_range_number_choose - 23]);
						temp1 = Custom_Digital_Range[bac_range_number_choose - 23];
					}
					else
						temp1 = Digital_Units_Array[bac_range_number_choose];//22 is the sizeof the array



					SplitCStringA(temparray,temp1,_T("/"));

					if(m_Output_data.at(lRow).control == 1)
					{
						if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
						{
							m_output_list.SetItemText(lRow,OUTPUT_VALUE,temparray.GetAt(1));
						}
					}
					else
					{
						if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
						{
							m_output_list.SetItemText(lRow,OUTPUT_VALUE,temparray.GetAt(0));
						}	
					}
					m_output_list.SetItemText(lRow,OUTPUT_RANGE,temp1);
					m_output_list.SetItemText(lRow,OUTPUT_UNITE,_T(""));//如果是数字单位 Unit 要清空;
					m_output_list.SetCellEnabled(lRow,OUTPUT_PWM_PERIOD,0);
					m_output_list.SetCellEnabled(lRow,OUTPUT_LOW_VOLTAGE,0);
					m_output_list.SetCellEnabled(lRow,OUTPUT_HIGH_VOLTAGE,0);
				}
			
	}
	else
	{
		return;
	}




	m_output_list.Set_Edit(false);

	int cmp_ret = memcmp(&m_temp_output_data[lRow],&m_Output_data.at(lRow),sizeof(Str_out_point));
	if(cmp_ret!=0)
	{
		changed_output_item = lRow;
		SetTimer(UPDATE_OUTPUT_ONE_ITEM_TIMER,2000,NULL);
		m_output_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Output List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEOUTPUT_T3000,lRow,lRow,sizeof(Str_out_point),m_output_dlg_hwnd,temp_task_info,lRow,lCol);



	}


	*pResult = 0;
}


void CBacnetOutput::OnTimer(UINT_PTR nIDEvent)
{
	 
	switch(nIDEvent)
	{
	case 1:
		{
			
			if(g_protocol == PROTOCOL_BIP_TO_MSTP)
			{
				PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
			}
			else if((this->IsWindowVisible()) && (Gsm_communication == false) &&  ((this->m_hWnd  == ::GetActiveWindow()) || (bacnet_view_number == TYPE_OUTPUT))  )	//GSM连接时不要刷新;
			{
				PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
				if((bac_select_device_online) && (g_protocol == PROTOCOL_BACNET_IP))
					Post_Refresh_Message(g_bac_instance,READOUTPUT_T3000,0,BAC_OUTPUT_ITEM_COUNT - 1,sizeof(Str_out_point),BAC_OUTPUT_GROUP);
				else if((bac_select_device_online) && ((g_protocol == MODBUS_RS485) || (g_protocol == MODBUS_TCPIP) || (g_protocol == PROTOCOL_MSTP_TO_MODBUS) || (g_protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS)))
				{
					if(read_each_485_fun_thread == NULL)
					{
						hide_485_progress = true;
						::PostMessage(BacNet_hwd,WM_RS485_MESSAGE,bacnet_device_type,BAC_OUT);//第二个参数 OUT
					}
				}
			}

		}
		break;
	case 2:	//在更改某一列之后要在读取此列的值，并刷新此列;
		{
			if(this->IsWindowVisible())
				PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
			KillTimer(2);
		}
		break;
	case UPDATE_OUTPUT_ONE_ITEM_TIMER:
		{
			KillTimer(UPDATE_OUTPUT_ONE_ITEM_TIMER);
			if((bac_select_device_online) && (g_protocol == PROTOCOL_BACNET_IP))
			{
				GetPrivateData_Blocking(g_bac_instance,READINPUT_T3000,changed_output_item,changed_output_item,sizeof(Str_out_point));
			}
			PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,changed_output_item,REFRESH_ON_ITEM);
		}
		break;

	default:
		break;
	}



	CDialogEx::OnTimer(nIDEvent);
}


BOOL CBacnetOutput::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		CRect list_rect,win_rect;
		m_output_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_output_dlg_hwnd,win_rect);
		m_output_list.Set_My_WindowRect(win_rect);
		m_output_list.Set_My_ListRect(list_rect);

		m_output_list.Get_clicked_mouse_position();
		return TRUE;
	}
	else if(pMsg->message==WM_NCLBUTTONDBLCLK)
	{
		if(!window_max)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 30 ,temp_mynew_rect.top + 30,500,700,SWP_SHOWWINDOW);
		}

		return 1; 
	}
    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    if (pFrame->m_pDialogInfo != NULL&&pFrame->m_pDialogInfo->IsWindowVisible())
    {
        if (pMsg->message == WM_LBUTTONDOWN||pMsg->message == WM_RBUTTONDOWN)
        {
            //  pMain->m_pDialogInfo->ShowWindow(SW_HIDE);
            ::PostMessage(MainFram_hwd,WM_LBUTTONDOWN,0,0);
        }
    }
     
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetOutput::OnClose()
{
	 

	ShowWindow(FALSE);
	return;

	KillTimer(1);
	//m_output_dlg_hwnd = NULL;
	//::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}


void CBacnetOutput::OnCancel()
{
	
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
}

int GetOutputLabel(int index, CString &ret_label, Point_Net * npoint)
{
	if(index >= BAC_OUTPUT_ITEM_COUNT)
	{
		ret_label.Empty();
		return -1;
	}
	int i = index;
	CString temp_des2;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(i).label, (int)strlen((char *)m_Output_data.at(i).label)+1, 
		ret_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_label.ReleaseBuffer();

	CString temp_cs;
	temp_cs = ret_label;
	temp_cs = temp_cs.Trim();
	if(temp_cs.IsEmpty())
	{
        if(npoint == NULL)
		    temp_cs.Format(_T("OUT%u"),index + 1);
        else
            temp_cs.Format(_T("%dOUT%u"),npoint->panel, index + 1);
		ret_label = temp_cs;
	}

	return 1;
}

int GetOutputFullLabel(int index,CString &ret_full_label , Point_Net * npoint)
{
	if(index >= BAC_OUTPUT_ITEM_COUNT)
	{
		ret_full_label.Empty();
		return -1;
	}
	int i = index;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(i).description, (int)strlen((char *)m_Output_data.at(i).description)+1, 
		ret_full_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_full_label.ReleaseBuffer();

	CString temp_cs;
	temp_cs = ret_full_label;
	temp_cs = temp_cs.Trim();
	if(temp_cs.IsEmpty())
	{
        if (npoint == NULL)
		    temp_cs.Format(_T("OUT%u"),index + 1);
        else
            temp_cs.Format(_T("%dOUT%u"), npoint->panel, index + 1);
		ret_full_label = temp_cs;
	}

	return 1;
}


int GetOutputValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M,int &digital_value)
{
	CStringArray temparray;
	CString temp1;
	if(index >= BAC_OUTPUT_ITEM_COUNT)
	{
		ret_cstring.Empty();
		ret_unit.Empty();
		Auto_M.Empty();
		return -1;
	}
	int i = index;

	if(m_Output_data.at(i).auto_manual == 1)
	{
		Auto_M = _T("M");
	}
	else
	{
		Auto_M.Empty();
	}

	if(m_Output_data.at(i).range == 0)
	{
		m_Output_data.at(i).digital_analog = BAC_UNITS_ANALOG;
	}

	if(m_Output_data.at(i).digital_analog == BAC_UNITS_ANALOG)
	{

		if(m_Output_data.at(i).range < (sizeof(OutPut_List_Analog_Units)/sizeof(OutPut_List_Analog_Units[0])))
		{
			ret_unit = OutPut_List_Analog_Units[m_Output_data.at(i).range];
			if(m_Output_data.at(i).range == 0)
				ret_unit.Empty();
		}
		else
		{
			ret_unit.Empty();
			return -1;
		}

		CString temp_value;
		ret_cstring.Format(_T("%.1f"),((float)m_Output_data.at(i).value) / 1000);
		digital_value = 2;//analog;
	}
	else if(m_Output_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
	{
		ret_unit.Empty();

		if(m_Output_data.at(i).range>30)
		{
			ret_cstring.Empty();
			return -1;
		}
		else
		{
			if((m_Output_data.at(i).range < 23) &&(m_Output_data.at(i).range !=0))
				temp1 = Digital_Units_Array[m_Output_data.at(i).range];
			else if((m_Output_data.at(i).range >=23) && (m_Output_data.at(i).range <= 30))
			{
				if(receive_customer_unit)
					temp1 = Custom_Digital_Range[m_Output_data.at(i).range - 23];
			}
			else
			{
				temp1.Empty();
				ret_cstring.Empty();
				return -1;
			}

			SplitCStringA(temparray,temp1,_T("/"));
			if((temparray.GetSize()==2))
			{
				if(m_Output_data.at(i).control == 0)
				{
					digital_value = 0;
					ret_cstring = temparray.GetAt(0);
					return 1;
				}
				else
				{
					digital_value = 1;
					ret_cstring = temparray.GetAt(1);
					return 1;
				}
			}
		}
	}

	return 1;
}


BOOL CBacnetOutput::OnHelpInfo(HELPINFO* pHelpInfo)
{
	 
		HWND hWnd;

		if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_6_4_OUTPUTS);
		return (hWnd != NULL);

	return CDialogEx::OnHelpInfo(pHelpInfo);
}


void CBacnetOutput::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(rc);
	if(m_output_list.m_hWnd != NULL)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0,0, 0,0,  SWP_NOSIZE | SWP_NOMOVE);
				m_output_list.MoveWindow(&rc);
	}

	
}




void CBacnetOutput::OnSysCommand(UINT nID, LPARAM lParam)
{
	 
	if(nID == SC_MAXIMIZE)
	{
		if(window_max == false)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 30 ,temp_mynew_rect.top + 30,500,700,SWP_SHOWWINDOW);
		}
		return;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}

void CBacnetOutput::Reset_Output_Rect()
{

		CRect temp_mynew_rect;
		::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;

		CRect temp_window;
		GetWindowRect(&temp_window);
		if(window_max)
		{
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), NULL);
		}
		else if((temp_window.Width() <= temp_mynew_rect.Width() ) && (temp_window.Height() <= temp_mynew_rect.Height()))
		{
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,0,0,SWP_NOSIZE );
		}
		else
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left + 30,temp_mynew_rect.top + 30,700,700, NULL);


		return;

}
