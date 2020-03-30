// BacnetTstat.cpp : implementation file
//  Programming by Fance  2014 02 

#include "stdafx.h"
#include "T3000.h"
#include "BacnetTstat.h"
#include "afxdialogex.h"
#include "global_function.h"

// CBacnetTstat dialog
Str_TstatInfo_point m_temp_tstat_data[BAC_TSTAT_COUNT];
IMPLEMENT_DYNAMIC(CBacnetTstat, CDialogEx)

CBacnetTstat::CBacnetTstat(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetTstat::IDD, pParent)
{

}

CBacnetTstat::~CBacnetTstat()
{
}

void CBacnetTstat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TSTAT, m_tstat_list);
}


BEGIN_MESSAGE_MAP(CBacnetTstat, CDialogEx)
	ON_MESSAGE(WM_REFRESH_BAC_TSTAT_LIST,Fresh_Tstat_List)	
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Tstat_Item)	
	ON_MESSAGE(MY_RESUME_DATA, TstatsMessageCallBack)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBacnetTstat message handlers


BOOL CBacnetTstat::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	Initial_List();
	PostMessage(WM_REFRESH_BAC_TSTAT_LIST,NULL,NULL);
	ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetTstat::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		return 1;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT  CBacnetTstat::TstatsMessageCallBack(WPARAM wParam, LPARAM lParam)
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
		memcpy_s(&m_Tstat_data.at(pInvoke->mRow),sizeof(Str_TstatInfo_point),&m_temp_tstat_data[pInvoke->mRow],sizeof(Str_TstatInfo_point));//还原没有改对的值;
		PostMessage(WM_REFRESH_BAC_TSTAT_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}

	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_tstat_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_tstat_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_tstat_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);

	if(pInvoke)
		delete pInvoke;
	return 0;
}


void CBacnetTstat::Initial_List()
{
	m_tstat_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_tstat_list.SetExtendedStyle(m_tstat_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_tstat_list.InsertColumn(TSTAT_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_tstat_list.InsertColumn(TSTAT_PRODUCT_MODEL_TYPE, _T("Model Type"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_ADDRESS , _T("Address"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_SERIAL_NUM , _T("Serial Number"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_OCCUPIED , _T("Un/Occupied"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_COOL_SETPOINT , _T("Cool SP"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_HEAT_SETPOINT , _T("Heat SP"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_ROOM_SETPOINT , _T("Room SP"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_ROOM_TEM , _T("Room Temp"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_MODE , _T("Stage"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_COOL_HEAT_MODE , _T("COOL/HEAT"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_OUTPUT_STATE , _T("Output State"), 90, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_NIGHT_HEAT_DB , _T("NHeat DB"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_NIGHT_COOL_DB , _T("NCool DB"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_NIGHT_HEAT_SP , _T("NHeat SP"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_list.InsertColumn(TSTAT_NIGHT_COOL_SP , _T("NCool SP"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	
	//m_tstat_list.InsertColumn(TSTAT_SERIAL_NUM, _T("Serial Number"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_tstat_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_tstat_dlg_hwnd;


	m_tstat_list.DeleteAllItems();
	for (int i=0;i<(int)m_Tstat_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_tstat_list.InsertItem(i,temp_item);

		for (int x=0;x<TSTAT_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_tstat_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_tstat_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}
	}

}

LRESULT CBacnetTstat::Fresh_Tstat_List(WPARAM wParam,LPARAM lParam)
{
	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if(m_tstat_list.IsDataNewer((char *)&m_Tstat_data.at(0),sizeof(Str_TstatInfo_point) * BAC_TSTAT_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_tstat_list.SetListData((char *)&m_Tstat_data.at(0),sizeof(Str_TstatInfo_point) * BAC_TSTAT_COUNT);
		}
		else
		{
			return 0;
		}
	}

	for (int i=0;i<(int)m_Tstat_data.size();i++)
	{
		if(isFreshOne)
		{
			i = Fresh_Item;
		}

		if((m_Tstat_data.at(i).product_model == 255) || ((m_Tstat_data.at(i).tst_db.id == 0)))
		{
			for (int x = 1; x < TSTAT_COL_NUMBER ; x++)
			{
				m_tstat_list.SetItemText(i,x,_T(""));
			}
			continue;
		}

		CString ModuleName;	CString Address;CString RoomTemperature;CString SerialNumber;
		CString OCCUPIED;	CString Cool_Setpoint;	CString Heat_Setpoint;	CString Room_Setpoint;
		CString CS_TSTAT_MODE;	CString CS_COOL_HEAT; CString CS_OUTPUT_STAGE;
		ModuleName = GetProductName(m_Tstat_data.at(i).product_model);
		m_tstat_list.SetItemText(i,TSTAT_PRODUCT_MODEL_TYPE,ModuleName);
		
		Address.Format(_T("%d"),m_Tstat_data.at(i).tst_db.id);
		m_tstat_list.SetItemText(i,TSTAT_ADDRESS,Address);
		SerialNumber.Format(_T("%d"),m_Tstat_data.at(i).tst_db.sn);
		m_tstat_list.SetItemText(i,TSTAT_SERIAL_NUM,SerialNumber);	

		if((m_Tstat_data.at(i).occupied == 0) || (m_Tstat_data.at(i).occupied == 1))
		{
			OCCUPIED =  CString_Occupied[m_Tstat_data.at(i).occupied];
		}
		else
		{
			OCCUPIED = CString_Occupied[2];
		}
		m_tstat_list.SetItemText(i,TSTAT_OCCUPIED,OCCUPIED);

		Cool_Setpoint.Format(_T("%.1f"),((float)m_Tstat_data.at(i).cool_setpoint)/10.0);
		m_tstat_list.SetItemText(i,TSTAT_COOL_SETPOINT,Cool_Setpoint);	
		Heat_Setpoint.Format(_T("%.1f"),((float)m_Tstat_data.at(i).heat_setpoint)/10.0);
		m_tstat_list.SetItemText(i,TSTAT_HEAT_SETPOINT,Heat_Setpoint);	
		Room_Setpoint.Format(_T("%.1f"),((float)m_Tstat_data.at(i).setpoint)/10.0);
		m_tstat_list.SetItemText(i,TSTAT_ROOM_SETPOINT,Room_Setpoint);
		RoomTemperature.Format(_T("%.1f"),((float)m_Tstat_data.at(i).temperature)/10.0);
		m_tstat_list.SetItemText(i,TSTAT_ROOM_TEM,RoomTemperature);
		CS_TSTAT_MODE.Format(_T("%d"),m_Tstat_data.at(i).mode);
		m_tstat_list.SetItemText(i,TSTAT_MODE,CS_TSTAT_MODE);
		
		if((m_Tstat_data.at(i).cool_heat_mode == 1) || (m_Tstat_data.at(i).cool_heat_mode == 2))
		{
			CS_COOL_HEAT = CString_Cool_Heat[m_Tstat_data.at(i).cool_heat_mode];
		}
		else if(m_Tstat_data.at(i).cool_heat_mode == 0)
		{
			CS_COOL_HEAT = CString_Cool_Heat[0];
		}
		else
		{
			CS_COOL_HEAT = CString_Cool_Heat[3];
		}
		m_tstat_list.SetItemText(i,TSTAT_COOL_HEAT_MODE,CS_COOL_HEAT);
		CS_OUTPUT_STAGE.Format(_T("%d"),m_Tstat_data.at(i).output_state);
		m_tstat_list.SetItemText(i,TSTAT_OUTPUT_STATE,CS_OUTPUT_STAGE);


		CString Night_Heat_Db;
		CString Night_Cool_Db;
		CString Night_Heat_Sp;
		CString Night_Cool_Sp;
		Night_Heat_Db.Format(_T("%d"),m_Tstat_data.at(i).night_heat_db);
		Night_Cool_Db.Format(_T("%d"),m_Tstat_data.at(i).night_cool_db);
		Night_Heat_Sp.Format(_T("%d"),m_Tstat_data.at(i).night_heat_sp);
		Night_Cool_Sp.Format(_T("%d"),m_Tstat_data.at(i).night_cool_sp);
		m_tstat_list.SetItemText(i,TSTAT_NIGHT_HEAT_DB,Night_Heat_Db);
		m_tstat_list.SetItemText(i,TSTAT_NIGHT_COOL_DB,Night_Cool_Db);
		m_tstat_list.SetItemText(i,TSTAT_NIGHT_HEAT_SP,Night_Heat_Sp);
		m_tstat_list.SetItemText(i,TSTAT_NIGHT_COOL_SP,Night_Cool_Sp);

	}
	return 1;
}

LRESULT CBacnetTstat::Fresh_Tstat_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	CString temp_task_info;
	CString New_CString =  m_tstat_list.GetItemText(Changed_Item,Changed_SubItem);
	CString cstemp_value;
	//先保存 原来的值，等结束的时候来比对，看是否有改变，有改变就进行写动作;
	memcpy_s(&m_temp_tstat_data[Changed_Item],sizeof(Str_TstatInfo_point),&m_Tstat_data.at(Changed_Item),sizeof(Str_TstatInfo_point));
	if((m_Tstat_data.at(Changed_Item).product_model == 255) || (m_Tstat_data.at(Changed_Item).tst_db.id == 0))
	{
		for (int x = 1; x < TSTAT_COL_NUMBER ; x++)
		{
			m_tstat_list.SetItemText(Changed_Item,x,_T(""));
		}
		return 0;
	}

	if(Changed_SubItem == TSTAT_COOL_SETPOINT )
	{
		CString cs_temp = m_tstat_list.GetItemText(Changed_Item,Changed_SubItem);
		int temp_value = (int)(_wtof(cs_temp) * 10);
		if(temp_value >= 65535)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Please input An effective value "),_T("Warning"));
			return 0;
		}

		m_Tstat_data.at(Changed_Item).cool_setpoint = temp_value;
	}

	if(Changed_SubItem == TSTAT_HEAT_SETPOINT )
	{
		CString cs_temp = m_tstat_list.GetItemText(Changed_Item,Changed_SubItem);
		int temp_value = (int)(_wtof(cs_temp) * 10);
		if(temp_value >= 65535)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Please input An effective value "),_T("Warning"));
			return 0;
		}

		m_Tstat_data.at(Changed_Item).heat_setpoint = temp_value;
	}
	
	if(Changed_SubItem == TSTAT_ROOM_SETPOINT )
	{
		CString cs_temp = m_tstat_list.GetItemText(Changed_Item,Changed_SubItem);
		int temp_value = (int)(_wtof(cs_temp) * 10);
		if(temp_value >= 65535)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Please input An effective value "),_T("Warning"));
			return 0;
		}

		m_Tstat_data.at(Changed_Item).setpoint = temp_value;
	}


	if(Changed_SubItem == TSTAT_ROOM_TEM )
	{
		CString cs_temp = m_tstat_list.GetItemText(Changed_Item,Changed_SubItem);
		int temp_value = (int)(_wtof(cs_temp) * 10);
		if(temp_value >= 65535)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Please input An effective value "),_T("Warning"));
			return 0;
		}

		m_Tstat_data.at(Changed_Item).temperature = temp_value;
	}

	if(Changed_SubItem == TSTAT_NIGHT_HEAT_DB )
	{
		CString cs_temp = m_tstat_list.GetItemText(Changed_Item,Changed_SubItem);
		int temp_value = _wtoi(cs_temp);
		if(temp_value >= 255)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Please input An effective value "),_T("Warning"));
			return 0;
		}

		m_Tstat_data.at(Changed_Item).night_heat_db = temp_value;
	}

	if(Changed_SubItem == TSTAT_NIGHT_COOL_DB )
	{
		CString cs_temp = m_tstat_list.GetItemText(Changed_Item,Changed_SubItem);
		int temp_value = _wtoi(cs_temp);
		if(temp_value >= 255)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Please input An effective value "),_T("Warning"));
			return 0;
		}

		m_Tstat_data.at(Changed_Item).night_cool_db = temp_value;
	}

	if(Changed_SubItem == TSTAT_NIGHT_HEAT_SP )
	{
		CString cs_temp = m_tstat_list.GetItemText(Changed_Item,Changed_SubItem);
		int temp_value = _wtoi(cs_temp);
		if(temp_value >= 255)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Please input An effective value "),_T("Warning"));
			return 0;
		}

		m_Tstat_data.at(Changed_Item).night_heat_sp = temp_value;
	}

	if(Changed_SubItem == TSTAT_NIGHT_COOL_SP )
	{
		CString cs_temp = m_tstat_list.GetItemText(Changed_Item,Changed_SubItem);
		int temp_value = _wtoi(cs_temp);
		if(temp_value >= 255)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Please input An effective value "),_T("Warning"));
			return 0;
		}
		m_Tstat_data.at(Changed_Item).night_cool_sp = temp_value;
	}

	

	
	
	


	cmp_ret = memcmp(&m_temp_tstat_data[Changed_Item],&m_Tstat_data.at(Changed_Item),sizeof(Str_TstatInfo_point));
	if(cmp_ret!=0)
	{
		m_tstat_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Tstat List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITETSTAT_T3000,Changed_Item,Changed_Item,sizeof(Str_TstatInfo_point),m_tstat_dlg_hwnd,temp_task_info,Changed_Item,Changed_SubItem);
	}

	return 1;
}


void CBacnetTstat::OnTimer(UINT_PTR nIDEvent)
{
	 

	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetTstat::OnCancel()
{
	
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	//CDialogEx::OnCancel();
}
