// BacnetInput.cpp : implementation file
// BacnetInput File Add by Fance 2013 0620

#include "stdafx.h"
#include "T3000.h"
#include "BacnetInput.h"
#include "afxdialogex.h"
#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
#include <math.h>

#include "gloab_define.h"
#include "BacnetRange.h"
#include "MainFrm.h"
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.
extern int initial_dialog;



// CBacnetInput dialog
 

IMPLEMENT_DYNAMIC(CBacnetInput, CDialogEx)

CBacnetInput::CBacnetInput(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetInput::IDD, pParent)
{
m_latest_protocol=g_protocol;

}


CBacnetInput::~CBacnetInput()
{
   
}

void CBacnetInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_input_list);
	DDX_Control(pDX, IDC_STATIC_INPUT_ITEM_INFO, m_input_item_info);
}

BEGIN_MESSAGE_MAP(CBacnetInput, CDialogEx)
    ON_MESSAGE(MY_RESUME_DATA, InputMessageCallBack)
    ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Input_List)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Input_Item)	
    ON_BN_CLICKED(IDOK, &CBacnetInput::OnBnClickedOk)
    ON_NOTIFY(NM_CLICK, IDC_LIST1, &CBacnetInput::OnNMClickList1)
    ON_WM_TIMER()
    ON_WM_CLOSE()
    
    ON_WM_HELPINFO()
END_MESSAGE_MAP()

// CBacnetInput message handlers
LRESULT  CBacnetInput::InputMessageCallBack(WPARAM wParam, LPARAM lParam)
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
		if((pInvoke->mRow < BAC_INPUT_ITEM_COUNT) && (pInvoke->mRow >= 0))
		{
			Post_Refresh_One_Message(g_bac_instance,READINPUT_T3000,
				pInvoke->mRow,pInvoke->mRow,sizeof(Str_in_point));
			SetTimer(2,2000,NULL);
		}
		
		//MessageBox(_T("Bacnet operation success!"));
	}
	else
	{
		memcpy_s(&m_Input_data.at(pInvoke->mRow),sizeof(Str_in_point),&m_temp_Input_data[pInvoke->mRow],sizeof(Str_in_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_INPUT_LIST,pInvoke->mRow,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);

	}
	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_input_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


	if(pInvoke)
		delete pInvoke;
	return 0;
}


BOOL CBacnetInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_input_item_info.SetWindowTextW(_T(""));
	m_input_item_info.textColor(RGB(0,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_input_item_info.setFont(20,14,NULL,_T("Arial"));

	// TODO:  Add extra initialization here
	Initial_List();
	PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
   

	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetInput::Reload_Unit_Type()
{

	for (int i=0;i<(int)m_Input_data.size();i++)	//Initial All first.
	{
		if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
		{
			ListCtrlEx::CStrList strlist;
			for (int i=0;i<(int)sizeof(Units_Type)/sizeof(Units_Type[0]);i++)
			{
				strlist.push_back(Units_Type[i]);
			}
			m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);		
		}
	}

	int initial_count = 0;
	if(bacnet_device_type == BIG_MINIPANEL)	//Special Initial
	{	
		if(BIG_MINIPANEL_IN_A > (int)m_Input_data.size()) 
			initial_count = (int)m_Input_data.size();
		else
			initial_count = BIG_MINIPANEL_IN_A;
		for (int i=0;i<initial_count;i++)
		{
			if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);		
			}
		}
	}
	else if(bacnet_device_type == SMALL_MINIPANEL)
	{
		if(SMALL_MINIPANEL_IN_A > (int)m_Input_data.size()) 
			initial_count = (int)m_Input_data.size();
		else
			initial_count = SMALL_MINIPANEL_IN_A;
		for (int i=0;i<initial_count;i++)
		{
			if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);		
			}
		}
	}
	else if(bacnet_device_type == TINY_MINIPANEL)
	{
		if(TINY_MINIPANEL_IN_A > (int)m_Input_data.size()) 
			initial_count = (int)m_Input_data.size();
		else
			initial_count = TINY_MINIPANEL_IN_A;
		for (int i=0;i<initial_count;i++)
		{
			if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);		
			}
		}
	}
	else if(bacnet_device_type == PRODUCT_CM5)
	{
		int analog_count;
		int digital_count;
		if(CM5_MINIPANEL_IN_A > (int)m_Input_data.size()) 
			analog_count = (int)m_Input_data.size();
		else
			analog_count = CM5_MINIPANEL_IN_A;

		digital_count = CM5_MINIPANEL_IN_D;
		
		for (int i=0;i<analog_count;i++)
		{
			if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Analog_Only);
				m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);		
			}
		}

		for (int i=analog_count;i< (analog_count + digital_count); i++ )
		{
			if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Units_Digital_Only);
				m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);		
			}
		}
		
		
	}



}

void CBacnetInput::Initial_List()
{
    m_input_list.ShowWindow(SW_HIDE);
	m_input_list.DeleteAllItems();
	while ( m_input_list.DeleteColumn (0)) ;

	m_input_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_input_list.InsertColumn(INPUT_NUM, _T("Input"), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_input_list.InsertColumn(INPUT_FULL_LABLE, _T("Full Label"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_AUTO_MANUAL, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_VALUE, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_UNITE, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_RANGE, _T("Range"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_CAL, _T("Calibration"), 70, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_CAL_OPERATION, _T("Sign"), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_FITLER, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_DECOM, _T("Status"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_JUMPER, _T("Jumper"), 70, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_LABLE, _T("Label"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_input_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_input_dlg_hwnd;

	CRect list_rect,win_rect;
	m_input_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_input_dlg_hwnd,win_rect);
	m_input_list.Set_My_WindowRect(win_rect);
	m_input_list.Set_My_ListRect(list_rect);


	//m_input_list.DeleteAllItems();
	for (int i=0;i<(int)m_Input_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

		if(i>=input_item_limit_count)	//vector的大小始终不变 ,用次变量来 约束 要显示的 item 数量;
			break;

		temp_item.Format(_T("IN%d"),i+1);
		m_input_list.InsertItem(i,temp_item);



		if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
		{
			ListCtrlEx::CStrList strlist;
			for (int i=0;i<(int)sizeof(Units_Type)/sizeof(Units_Type[0]);i++)
			{
				strlist.push_back(Units_Type[i]);
			}
			m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);		
		}

		if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_JUMPER))
		{
			ListCtrlEx::CStrList strlist;
			for (int i=0;i<(int)sizeof(JumperStatus)/sizeof(JumperStatus[0]);i++)
			{
				strlist.push_back(JumperStatus[i]);
			}
			m_input_list.SetCellStringList(i, INPUT_JUMPER, strlist);		
		}

		//if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_DECOM))
		//{
		//	ListCtrlEx::CStrList strlist;
		//	for (int m=0;m<(int)(sizeof(Decom_Array)/sizeof(Decom_Array[0]));m++)
		//	{
		//		strlist.push_back(Decom_Array[m]);
		//	}
		//	m_input_list.SetCellStringList(i, INPUT_DECOM, strlist);
		//}

		for (int x=0;x<INPUT_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

		

	}
	m_input_list.InitListData();
	 m_input_list.ShowWindow(SW_SHOW);
}

void CBacnetInput::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
//	CDialogEx::OnOK();
}
LRESULT CBacnetInput::Fresh_Input_Item(WPARAM wParam,LPARAM lParam)
{   
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	memcpy_s(&m_temp_Input_data[Changed_Item],sizeof(Str_in_point),&m_Input_data.at(Changed_Item),sizeof(Str_in_point));

	CString temp_task_info;
	CString New_CString =  m_input_list.GetItemText(Changed_Item,Changed_SubItem);
	CString cstemp_value;
	if(Changed_SubItem == INPUT_LABLE)
	{
		CString cs_temp = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_IN_LABEL)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not greater than 8"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
			return 0;
		}
		cs_temp.Replace(_T("-"),_T("_"));
		cs_temp.MakeUpper();
		if(Check_Label_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Input_data.at(Changed_Item).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);
	}

	if(Changed_SubItem == INPUT_FULL_LABLE)
	{
		CString cs_temp = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_IN_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 20"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
			return 0;
		}

		if(Check_FullLabel_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}

		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Input_data.at(Changed_Item).description,STR_IN_DESCRIPTION_LENGTH,cTemp1,STR_IN_DESCRIPTION_LENGTH);
	}


	if(Changed_SubItem == INPUT_AUTO_MANUAL)
	{
		CString temp_cs = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(_T("Auto"))==0)
		{
			//m_input_list.SetCellEnabled(Changed_Item,INPUT_VALUE,0);
			m_Input_data.at(Changed_Item).auto_manual = BAC_AUTO ;
		}
		else
		{
			//m_input_list.SetCellEnabled(Changed_Item,INPUT_VALUE,1);
			m_Input_data.at(Changed_Item).auto_manual = BAC_MANUAL ;
		}
	}



	if(Changed_SubItem == INPUT_VALUE)
	{
		CString temp_cs = m_input_list.GetItemText(Changed_Item,Changed_SubItem);

		int temp_int = (int)(_wtof(temp_cs) * 1000);
		//int temp_int = _wtoi(temp_cs);
		m_Input_data.at(Changed_Item).value = temp_int;
	}

	


	if(Changed_SubItem==INPUT_CAL)
	{
		CString cs_temp=m_input_list.GetItemText(Changed_Item,INPUT_CAL);
		float temp_value = _wtof(cs_temp);
		int cal_value = (int)(temp_value * 10);
		if((cal_value<0) || (cal_value >65535))
		{
			MessageBox(_T("Please Input an value between 0.0 - 6553.6"),_T("Warning"),MB_OK);
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,Changed_Item,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
			return 0;
		}

		//int cal_value = _wtoi(cs_temp);
		
		m_Input_data.at(Changed_Item).calibration_l = cal_value & 0x00ff;
		m_Input_data.at(Changed_Item).calibration_h = (cal_value & 0xff00)>>8;
	}
	else if(Changed_SubItem==INPUT_FITLER)
	{
		CString cs_temp=m_input_list.GetItemText(Changed_Item,INPUT_FITLER);
		int  temp2 = _wtoi(cs_temp);
		if((temp2<0) || (temp2 >255))
		{
			MessageBox(_T("Please Input an value between 0 - 255"),_T("Warning"),MB_OK);
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,Changed_Item,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
			return 0;
		}
		m_Input_data.at(Changed_Item).filter = (unsigned char)temp2;//(int8_t)(log((double)temp2)/log((double)2));
	}
	//else if(Changed_SubItem==INPUT_DECOM)
	//{
	//	CString cs_temp = m_input_list.GetItemText(Changed_Item,INPUT_DECOM);
	//	int dec_index=-1;
	//	for (int m=0;m<(int)sizeof(Decom_Array)/sizeof(Decom_Array[0]);m++)
	//	{
	//		if(cs_temp.CompareNoCase(Decom_Array[m])==0)
	//		{
	//			dec_index = m;
	//			break;
	//		}
	//	}
	//	m_Input_data.at(Changed_Item).decom = dec_index;
	//}

	if(Changed_SubItem == INPUT_JUMPER)
	{
		CString temp_jump = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		for (int z=0;z<sizeof(JumperStatus)/sizeof(JumperStatus[0]);z++)
		{
			if(temp_jump.CompareNoCase(JumperStatus[z]) == 0)
			{
				unsigned char temp_value = 0;
				if((z == 0) || (z == 1) || (z == 2) || (z == 3) || (z == 4))
					temp_value = z;
				unsigned char temp1;
				temp1 = m_Input_data.at(Changed_Item).decom ;
				temp1 = temp1 & 0x0f;
				temp1 = temp1 | (temp_value << 4);
				m_Input_data.at(Changed_Item).decom = temp1;
			}
		}

	}

	cmp_ret = memcmp(&m_temp_Input_data[Changed_Item],&m_Input_data.at(Changed_Item),sizeof(Str_in_point));
	if(cmp_ret!=0)
	{
		m_input_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Input List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEINPUT_T3000,Changed_Item,Changed_Item,sizeof(Str_in_point),m_input_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
	}
	//m_input_list.Invalidate();
	return 0;
}

LRESULT CBacnetInput::Fresh_Input_List(WPARAM wParam,LPARAM lParam)
{

	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == (int)REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if(m_input_list.IsDataNewer((char *)&m_Input_data.at(0),sizeof(Str_in_point) * BAC_INPUT_ITEM_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_input_list.SetListData((char *)&m_Input_data.at(0),sizeof(Str_in_point) * BAC_INPUT_ITEM_COUNT);
		}
		else
		{
			return 0;
		}
	}
	if(0)
	{
		int temp_select_raw = 0;
		int temp_select_col = 0;
		m_input_list.Get_Selected_Item(temp_select_raw,temp_select_col);
		m_input_list.SetItemBkColor(temp_select_raw,temp_select_col,LIST_ITEM_SELECTED,0);
	}
	CString temp1;
	//m_input_list.DeleteAllItems();
	for (int i=0;i<(int)m_Input_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

		if(isFreshOne)
		{
			i = Fresh_Item;
		}

		if(i>=input_item_limit_count)
			break;

		MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(i).description, (int)strlen((char *)m_Input_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();

		m_input_list.SetItemText(i,INPUT_FULL_LABLE,temp_des);
		if(m_Input_data.at(i).auto_manual==0)
		{
			m_input_list.SetItemText(i,INPUT_AUTO_MANUAL,_T("Auto"));
			//m_input_list.SetCellEnabled(i,INPUT_VALUE,0);
		}
		else
		{
			m_input_list.SetItemText(i,INPUT_AUTO_MANUAL,_T("Manual"));
			//m_input_list.SetCellEnabled(i,INPUT_VALUE,1);
		}

		if(m_Input_data.at(i).digital_analog == BAC_UNITS_ANALOG)
		{

			m_input_list.SetCellEnabled(i,INPUT_CAL,1);

			if(m_Input_data.at(i).range == 0)
				m_input_list.SetItemText(i,INPUT_RANGE,_T("Unused"));
			else if(m_Input_data.at(i).range <  (sizeof(Input_Analog_Units_Array)/sizeof(Input_Analog_Units_Array[0])))
				m_input_list.SetItemText(i,INPUT_RANGE,Input_Analog_Units_Array[m_Input_data.at(i).range]);
			else
				m_input_list.SetItemText(i,INPUT_RANGE,_T("Out of range"));

			if(m_Input_data.at(i).range <  (sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0])))
				m_input_list.SetItemText(i,INPUT_UNITE,Input_List_Analog_Units[m_Input_data.at(i).range]);
			else
				m_input_list.SetItemText(i,INPUT_RANGE,_T("Unused"));
			


			CString cstemp_value;
			float temp_float_value;
			temp_float_value = ((float)m_Input_data.at(i).value) / 1000;
			cstemp_value.Format(_T("%.2f"),temp_float_value);
			m_input_list.SetItemText(i,INPUT_VALUE,cstemp_value);

			//CString temp_value;
			//temp_value.Format(_T("%d"),m_Input_data.at(i).value);
			//m_input_list.SetItemText(i,INPUT_VALUE,temp_value);
			unsigned short temp_cal_value = ((unsigned char)(m_Input_data.at(i).calibration_h)) *256 + (unsigned char)m_Input_data.at(i).calibration_l;

			temp_cal.Format(_T("%.1f"),((float)temp_cal_value)/10);
			m_input_list.SetItemText(i,INPUT_CAL,temp_cal);
			if(m_Input_data.at(i).calibration_sign == 0)
			{
				m_input_list.SetItemText(i,INPUT_CAL_OPERATION,_T("+"));
			}
			else
			{
				m_input_list.SetItemText(i,INPUT_CAL_OPERATION,_T("-"));
			}
		}
		else if(m_Input_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{

			m_input_list.SetItemText(i,INPUT_CAL,_T(""));

			if(m_Input_data.at(i).range == 0)
			{
				CString cstemp_value1;
				float temp_float_value1;
				temp_float_value1 = ((float)m_Input_data.at(i).value) / 1000;
				cstemp_value1.Format(_T("%.2f"),temp_float_value1);
				m_input_list.SetItemText(i,INPUT_VALUE,cstemp_value1);

				m_input_list.SetItemText(i,INPUT_RANGE,Digital_Units_Array[0]);
			}
			else if(m_Input_data.at(i).range<=22)
				m_input_list.SetItemText(i,INPUT_RANGE,Digital_Units_Array[m_Input_data.at(i).range]);
			else if((m_Input_data.at(i).range >= 23) && (m_Input_data.at(i).range <= 30))
			{
				if(receive_customer_unit)
				   m_input_list.SetItemText(i,INPUT_RANGE,temp_unit_no_index[m_Input_data.at(i).range - 23]);
				else
					m_input_list.SetItemText(i,INPUT_RANGE,Digital_Units_Array[0]);
			}
			else
				m_input_list.SetItemText(i,INPUT_RANGE,Digital_Units_Array[0]);
			m_input_list.SetItemText(i,INPUT_UNITE,_T(""));

			if((m_Input_data.at(i).range>30)  || (m_Input_data.at(i).range == 0))
			{
				//m_input_list.SetItemText(i,INPUT_UNITE,Digital_Units_Array[0]);
			}
			else
			{
				
				CStringArray temparray;

				if((m_Input_data.at(i).range < 23) &&(m_Input_data.at(i).range !=0))
					temp1 = Digital_Units_Array[m_Input_data.at(i).range];
				else if((m_Input_data.at(i).range >=23) && (m_Input_data.at(i).range <= 30))
				{
					if(receive_customer_unit)
						temp1 = temp_unit_no_index[m_Input_data.at(i).range - 23];
				}


				//temp1 = Digital_Units_Array[m_Input_data.at(i).range];
				SplitCStringA(temparray,temp1,_T("/"));
				if((temparray.GetSize()==2))
				{
					if(m_Input_data.at(i).control == 0)
						m_input_list.SetItemText(i,INPUT_VALUE,temparray.GetAt(0));
					else
						m_input_list.SetItemText(i,INPUT_VALUE,temparray.GetAt(1));
				}
			}

		}


		//temp_filter.Format(_T("%d"),(int)pow((double)2,(int)m_Input_data.at(i).filter));
		temp_filter.Format(_T("%d"),(unsigned char)m_Input_data.at(i).filter);
		m_input_list.SetItemText(i,INPUT_FITLER,temp_filter);

		int temp_decom = 0;
		int temp_jumper = 0;
		temp_decom = m_Input_data.at(i).decom & 0x0f;
		temp_jumper = (m_Input_data.at(i).decom & 0xf0 ) >> 4;

		//如果range 是0 或者 不在正常范围内，就不要显示 open short 的报警 状态;
		if((temp_decom==0) || (m_Input_data.at(i).range == 0) || (m_Input_data.at(i).range > 30))
		{
			temp_status.Format(Decom_Array[0]);
			m_input_list.SetItemTextColor(i,INPUT_DECOM,RGB(0,0,0),false);
		}
		else if(temp_decom==1)
		{
			temp_status.Format(Decom_Array[1]);
			m_input_list.SetItemTextColor(i,INPUT_DECOM,RGB(255,0,0),false);
		}
		else if(temp_decom==2)
		{
			temp_status.Format(Decom_Array[2]);
			m_input_list.SetItemTextColor(i,INPUT_DECOM,RGB(255,0,0),false);
		}
		else
		{
			temp_status.Empty();
			m_input_list.SetItemTextColor(i,INPUT_DECOM,RGB(0,0,0),false);
		}
		m_input_list.SetItemText(i,INPUT_DECOM,temp_status);


		if(temp_jumper == 1)
		{
			temp_status.Format(JumperStatus[1]);
		}
		else if(temp_jumper == 2)
		{
			temp_status.Format(JumperStatus[2]);
		}
		else if(temp_jumper == 3)
		{
			temp_status.Format(JumperStatus[3]);
		}
		else if(temp_jumper == 4)
		{
			temp_status.Format(JumperStatus[4]);
		}
		else if(temp_jumper == 0)
		{
			temp_status.Format(JumperStatus[0]);
		}
		else
		{
			m_Input_data.at(i).decom = m_Input_data.at(i).decom & 0x0f;	 //如果最高位不是 有效值，清零;
		}
		m_input_list.SetItemText(i,INPUT_JUMPER,temp_status);

		CString temp_des2;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(i).label, (int)strlen((char *)m_Input_data.at(i).label)+1, 
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des2.ReleaseBuffer();

		m_input_list.SetItemText(i,INPUT_LABLE,temp_des2);
		if(isFreshOne)
		{
			break;
		}
		//ptr_panel
	}
	copy_data_to_ptrpanel(TYPE_INPUT);
	return 0;
}


void CBacnetInput::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{


	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
 
	CString temp_cstring;
	long lRow,lCol;
	m_input_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_input_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_input_list.SubItemHitTest(&lvinfo);
	if((nItem!=-1) && (nItem < BAC_INPUT_ITEM_COUNT))
	{
		input_list_line = nItem;
		if((m_Input_data.at(input_list_line).sub_id !=0) &&
			(m_Input_data.at(input_list_line).sub_number !=0) &&
			(m_Input_data.at(input_list_line).sub_product !=0))
		{
			unsigned char temp_pid = m_Input_data.at(input_list_line).sub_product;
			if((temp_pid == PM_T3PT10) ||
				(temp_pid == PM_T3IOA) ||
				(temp_pid == PM_T332AI) ||
				(temp_pid == PM_T38AI16O) ||
				(temp_pid == PM_T38I13O) ||
				(temp_pid == PM_T34AO) ||
				(temp_pid == PM_T36CT))
			{
				m_input_item_info.ShowWindow(true);
				CString temp_name;
				temp_name = GetProductName(m_Input_data.at(input_list_line).sub_product);
				CString show_info;
				CString temp_id;
				CString temp_number;
				temp_id.Format(_T(" Sub ID: %d        "),m_Input_data.at(input_list_line).sub_id);
				temp_number.Format(_T("Input%d"),m_Input_data.at(input_list_line).sub_number);
				show_info = _T("Module:") + temp_name +_T("        ") + temp_id + temp_number;
				m_input_item_info.SetWindowTextW(show_info);

			}	
			else
			{
				m_input_item_info.ShowWindow(false);
			}
		}
		else
		{
			m_input_item_info.ShowWindow(false);
		}
	}
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_input_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的;
		return;
	if(lRow<0)
		return;
	CString New_CString;
	CString temp_task_info;

	CString temp1;
	CStringArray temparray;
	if(lCol == INPUT_VALUE)
	{
		if(m_Input_data.at(lRow).auto_manual == BAC_AUTO)	//If it is auto mode, disable to change the value.
		{
			m_input_list.Set_Edit(false);
			return;
		}

		if(m_Input_data.at(lRow).digital_analog != BAC_UNITS_DIGITAL)
			return;



		memcpy_s(&m_temp_Input_data[lRow],sizeof(Str_in_point),&m_Input_data.at(lRow),sizeof(Str_in_point));


		if((m_Input_data.at(lRow).range < 23) &&(m_Input_data.at(lRow).range !=0))
			temp1 = Digital_Units_Array[m_Input_data.at(lRow).range];
		else if((m_Input_data.at(lRow).range >=23) && (m_Input_data.at(lRow).range <= 30))
		{
			if(receive_customer_unit)
				temp1 = temp_unit_no_index[m_Input_data.at(lRow).range - 23];
			else
			{
				m_input_list.Set_Edit(false);
				return;
			}
		}
		else
			return;
		SplitCStringA(temparray,temp1,_T("/"));

		if(m_Input_data.at(lRow).control == 0)
		{
			m_Input_data.at(lRow).control = 1;
			m_input_list.SetItemText(lRow,INPUT_VALUE,temparray.GetAt(1));
			New_CString = temparray.GetAt(1);
		}
		else
		{
			m_Input_data.at(lRow).control = 0;
			m_input_list.SetItemText(lRow,INPUT_VALUE,temparray.GetAt(0));
			New_CString = temparray.GetAt(0);
		}

	}
	else if(lCol == INPUT_CAL_OPERATION)
	{
		CString n_option1;
		CString n_option2;
		CString n_value;


		if((m_Input_data.at(lRow).calibration_l != 0) && (m_Input_data.at(lRow).calibration_h != 0))
		{
			float temp_float_value;
			unsigned short temp_cal_value;
			temp_cal_value = (m_Input_data.at(lRow).calibration_h << 8 ) + m_Input_data.at(lRow).calibration_l;
			temp_float_value = ((float)temp_cal_value) / 10;
			n_value.Format(_T("%.1f"),temp_float_value);
		}

		if(m_Input_data.at(lRow).calibration_sign == 0)
		{
			n_option1.Format(_T("+"));
			n_option2.Format(_T("-"));
			n_option1 = n_option1 + n_value;
			n_option2 = n_option2 + n_value;
		}
		else
		{
			n_option1.Format(_T("-"));
			n_option2.Format(_T("+"));
			n_option1 = n_option1 + n_value;
			n_option2 = n_option2 + n_value;
		}

		CString notic_message;
		notic_message.Format(_T("This will change the calibration of this input from %s to %s"),n_option1.GetString(),n_option2.GetString());
		if(IDYES != MessageBox(notic_message,_T("Warning"),MB_YESNOCANCEL))
			return;
		memcpy_s(&m_temp_Input_data[lRow],sizeof(Str_in_point),&m_Input_data.at(lRow),sizeof(Str_in_point));
		if(m_Input_data.at(lRow).calibration_sign == 0)
		{
			m_Input_data.at(lRow).calibration_sign = 1;
			m_input_list.SetItemText(lRow,INPUT_CAL_OPERATION,_T("-"));
			New_CString = _T("-");
		}
		else
		{
			m_Input_data.at(lRow).calibration_sign = 0;
			m_input_list.SetItemText(lRow,INPUT_CAL_OPERATION,_T("+"));
			New_CString = _T("+");
		}
	}
	else if(lCol == INPUT_AUTO_MANUAL)
	{
		memcpy_s(&m_temp_Input_data[lRow],sizeof(Str_in_point),&m_Input_data.at(lRow),sizeof(Str_in_point));
		if(m_Input_data.at(lRow).auto_manual == 0)
		{
			m_Input_data.at(lRow).auto_manual = 1;
			m_input_list.SetItemText(lRow,INPUT_AUTO_MANUAL,_T("Manual"));
			//m_input_list.SetCellEnabled(lRow,INPUT_VALUE,TRUE);
			New_CString = _T("Manual");
		}
		else
		{
			m_Input_data.at(lRow).auto_manual = 0;
			m_input_list.SetItemText(lRow,INPUT_AUTO_MANUAL,_T("Auto"));
			//m_input_list.SetCellEnabled(lRow,INPUT_VALUE,FALSE);
			New_CString = _T("Auto");
		}
	}
	else if(lCol == INPUT_RANGE)
	{

		//CString temp_cs = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		BacnetRange dlg;

		//点击产品的时候 需要读customer units，老的产品firmware 说不定没有 这些，所以不强迫要读到;
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
				} while (temp_invoke_id<0);
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
		bac_range_number_choose = m_Input_data.at(lRow).range;

		if(m_Input_data.at(lRow).digital_analog == BAC_UNITS_ANALOG)
		{
			bac_ranges_type = INPUT_RANGE_ANALOG_TYPE;
			if(m_Input_data.at(lRow).range > (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
			{
				m_Input_data.at(lRow).range = 0;
				bac_range_number_choose = 0;
			}
		}
		else
		{
			bac_ranges_type = INPUT_RANGE_DIGITAL_TYPE;
			if(m_Input_data.at(lRow).range > 30)
			{
				m_Input_data.at(lRow).range = 0;
				bac_range_number_choose = 0;
			}
		}
		

		//if(temp_cs.CompareNoCase(Units_Type[UNITS_TYPE_ANALOG])==0)
		//{
			initial_dialog = 2;
			dlg.DoModal();
			if(range_cancel)
			{
				PostMessage(WM_REFRESH_BAC_INPUT_LIST,lRow,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
				return ;
			}
			if(bac_range_number_choose == 0)	//如果选择的是 unused 就认为是analog 的unused;这样 能显示对应的value;
			{
				m_Input_data.at(lRow).digital_analog =  BAC_UNITS_ANALOG;
				bac_ranges_type = INPUT_RANGE_ANALOG_TYPE;
			}
			if(bac_ranges_type == INPUT_RANGE_ANALOG_TYPE)
			{
				m_Input_data.at(lRow).digital_analog =  BAC_UNITS_ANALOG;
				m_Input_data.at(lRow).range =  bac_range_number_choose;
				m_input_list.SetItemText(lRow,INPUT_RANGE,Input_Analog_Units_Array[bac_range_number_choose]);		
				m_input_list.SetItemText(lRow,INPUT_UNITE,Input_List_Analog_Units[bac_range_number_choose]);	

				unsigned short temp_cal_value;
				temp_cal_value = (m_Input_data.at(lRow).calibration_h << 8 ) + m_Input_data.at(lRow).calibration_l;


				CString cstemp_value;
				cstemp_value.Format(_T("%.1f"),((float)temp_cal_value)/10);
				m_input_list.SetItemText(lRow,INPUT_CAL,cstemp_value);


				m_input_list.SetCellEnabled(lRow,INPUT_CAL,1);
				m_input_list.SetCellEnabled(lRow,INPUT_UNITE,1);
				//m_input_list.SetItemText(Changed_Item,OUTPUT_100_PERSENT,_T("10"));
				//m_input_list.SetCellEnabled(Changed_Item,OUTPUT_100_PERSENT,1);

				float temp_float_value;
				temp_float_value = ((float)m_Input_data.at(lRow).value) / 1000;
				cstemp_value.Format(_T("%.2f"),temp_float_value);
				m_input_list.SetItemText(lRow,INPUT_VALUE,cstemp_value);	
			}
			else if((bac_ranges_type == VARIABLE_RANGE_DIGITAL_TYPE) || (bac_ranges_type == INPUT_RANGE_DIGITAL_TYPE) || (bac_ranges_type == OUTPUT_RANGE_DIGITAL_TYPE))
			{
				m_Input_data.at(lRow).digital_analog =  BAC_UNITS_DIGITAL;
				m_Input_data.at(lRow).range =  bac_range_number_choose;
				//m_input_list.SetItemText(Changed_Item,INPUT_RANGE,INPUT_Digital_Units_Show[bac_range_number_choose]);		

				m_input_list.SetItemText(lRow,INPUT_CAL,_T(""));
				m_input_list.SetCellEnabled(lRow,INPUT_CAL,0);
				m_input_list.SetItemText(lRow,INPUT_UNITE,_T(""));
				m_input_list.SetCellEnabled(lRow,INPUT_UNITE,0);

				

				CStringArray temparray;
				if((bac_range_number_choose >= 23) && (bac_range_number_choose <= 30))
				{
					//temp1.Format(_T("%s"), temp_unit_no_index[bac_range_number_choose - 23]);
					temp1 = temp_unit_no_index[bac_range_number_choose - 23];
				}
				else
					temp1 = Digital_Units_Array[bac_range_number_choose];//22 is the sizeof the array
				SplitCStringA(temparray,temp1,_T("/"));
				New_CString = temp1;
				if(m_Input_data.at(lRow).control == 1)
				{
					if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
					{
						m_input_list.SetItemText(lRow,INPUT_VALUE,temparray.GetAt(1));
					}
				}
				else
				{
					if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
					{
						m_input_list.SetItemText(lRow,INPUT_VALUE,temparray.GetAt(0));
					}			
				}
				m_input_list.SetItemText(lRow,INPUT_RANGE,temp1);
			}
	}
	else
		return;





	m_input_list.Set_Edit(false);

	int cmp_ret = memcmp(&m_temp_Input_data[lRow],&m_Input_data.at(lRow),sizeof(Str_in_point));
	if(cmp_ret!=0)
	{
		m_input_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Input List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEINPUT_T3000,(int8_t)lRow,(int8_t)lRow,sizeof(Str_in_point),m_input_dlg_hwnd,temp_task_info,lRow,lCol);
	}



	*pResult = 0;
}


void CBacnetInput::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		{
			if(g_protocol == PROTOCOL_BIP_TO_MSTP)
			{
				PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
			}
			else if((this->IsWindowVisible()) && (Gsm_communication == false) )	//GSM连接时不要刷新;
			{
				PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
				if(bac_select_device_online)
					Post_Refresh_Message(g_bac_instance,READINPUT_T3000,0,BAC_INPUT_ITEM_COUNT - 1,sizeof(Str_in_point), BAC_INPUT_GROUP);
			}
		}
		break;
	case 2:	//在更改某一列之后要在读取此列的值，并刷新此列;
		{
			if(this->IsWindowVisible())
				PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
			KillTimer(2);
		}
		break;
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetInput::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_input_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}
void CBacnetInput::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
//	m_input_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
//	CDialogEx::OnCancel();
}


BOOL CBacnetInput::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if((pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN))
	{
		CRect list_rect,win_rect;
		m_input_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_input_dlg_hwnd,win_rect);
		m_input_list.Set_My_WindowRect(win_rect);
		m_input_list.Set_My_ListRect(list_rect);

		m_input_list.Get_clicked_mouse_position();
		return TRUE;
	}
    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    if (pFrame->m_pDialogInfo->IsWindowVisible())
    {
        if (pMsg->message == WM_LBUTTONDOWN||pMsg->message == WM_RBUTTONDOWN)
        {
            //  pMain->m_pDialogInfo->ShowWindow(SW_HIDE);
            ::PostMessage(MainFram_hwd,WM_LBUTTONDOWN,0,0);
        }
    }
   
	return CDialogEx::PreTranslateMessage(pMsg);
}


int GetInputLabel(int index,CString &ret_label)
{
	if(index >= BAC_INPUT_ITEM_COUNT)
	{
		ret_label.Empty();
		return -1;
	}
	int i = index;
	CString temp_des2;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(i).label, (int)strlen((char *)m_Input_data.at(i).label)+1, 
		ret_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_label.ReleaseBuffer();
	ret_label = ret_label.Trim();
	if(ret_label.IsEmpty())
	{
		ret_label.Format(_T("IN%d"),index+1);
	}
	return 1;
}

int GetInputFullLabel(int index,CString &ret_full_label)
{
	if(index >= BAC_INPUT_ITEM_COUNT)
	{
		ret_full_label.Empty();
		return -1;
	}
	int i = index;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(i).description, (int)strlen((char *)m_Input_data.at(i).description)+1, 
		ret_full_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_full_label.ReleaseBuffer();
	ret_full_label = ret_full_label.Trim();
	if(ret_full_label.IsEmpty())
	{
		ret_full_label.Format(_T("IN%d"),index+1);
	}
	return 1;
}

int GetInputValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M,int &digital_value)
{
	CStringArray temparray;
	CString temp1;
	if(index >= BAC_INPUT_ITEM_COUNT)
	{
		ret_cstring.Empty();
		ret_unit.Empty();
		Auto_M.Empty();
		return -1;
	}
	int i = index;

	if(m_Input_data.at(i).auto_manual == 1)
	{
		Auto_M = _T("M");
	}
	else
	{
		Auto_M.Empty();
	}

	if(m_Input_data.at(i).digital_analog == BAC_UNITS_ANALOG)
	{
		float temp_float_value;
		temp_float_value = ((float)m_Input_data.at(i).value) / 1000;
		ret_cstring.Format(_T("%.2f"),temp_float_value);

		if(m_Input_data.at(i).range <  (sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0])))
			ret_unit =Input_List_Analog_Units[m_Input_data.at(i).range];
		else
		{
			ret_unit.Empty();
			return -2;
		}
		digital_value = 2;//analog;
		return 1;
	}
	else if(m_Input_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
	{
		if((m_Input_data.at(i).range == 0))
		{
			float temp_float_value;
			temp_float_value = ((float)m_Input_data.at(i).value) / 1000;
			ret_cstring.Format(_T("%.2f"),temp_float_value);
		}
		else if((m_Input_data.at(i).range>30))
		{
			ret_cstring.Empty();
			return -1;
		}
		else
		{

			if((m_Input_data.at(i).range < 23) &&(m_Input_data.at(i).range !=0))
				temp1 = Digital_Units_Array[m_Input_data.at(i).range];
			else if((m_Input_data.at(i).range >=23) && (m_Input_data.at(i).range <= 30))
			{
				if(receive_customer_unit)
					temp1 = temp_unit_no_index[m_Input_data.at(i).range - 23];
			}


			SplitCStringA(temparray,temp1,_T("/"));
			if((temparray.GetSize()==2))
			{
				if(m_Input_data.at(i).control == 0)
				{
					digital_value = 0;//analog;
					ret_cstring = temparray.GetAt(0);
					return 1;
				}
				else
				{
					digital_value = 1;//analog;
					ret_cstring = temparray.GetAt(1);
					return 1;
				}
			}
		}

	}
	return 1;
}


/*
F1>>Help
this is responsible for F1
Go to the Help ID 
*/


BOOL CBacnetInput::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: Add your message handler code here and/or call default
	 
	 if (m_latest_protocol==PROTOCOL_BACNET_IP){
		 HWND hWnd;

		 if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		 else
			 hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_INPUTS);
			 return (hWnd != NULL);
		}
	else{
	::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
	}
	return CDialogEx::OnHelpInfo(pHelpInfo);
}
