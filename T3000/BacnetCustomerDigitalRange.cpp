// BacnetCustomerDigitalRange.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetCustomerDigitalRange.h"
#include "afxdialogex.h"
#include "global_function.h"
Str_Units_element m_temp_customer_unite_data[BAC_CUSTOMER_UNITS_COUNT];
// CBacnetCustomerDigitalRange dialog

IMPLEMENT_DYNAMIC(CBacnetCustomerDigitalRange, CDialogEx)

CBacnetCustomerDigitalRange::CBacnetCustomerDigitalRange(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetCustomerDigitalRange::IDD, pParent)
{

}

CBacnetCustomerDigitalRange::~CBacnetCustomerDigitalRange()
{
}

void CBacnetCustomerDigitalRange::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CUSTOMER_DIGITAL_RANGE, m_customer_dig_range_list);
}


BEGIN_MESSAGE_MAP(CBacnetCustomerDigitalRange, CDialogEx)
	ON_MESSAGE(WM_REFRESH_BAC_CUSTOMER_DIGITAL_RANGE_LIST,Fresh_Customer_Digital_Range_List)	
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Customer_Digital_Range_Item)	
	ON_NOTIFY(NM_CLICK, IDC_LIST_CUSTOMER_DIGITAL_RANGE, &CBacnetCustomerDigitalRange::OnNMClickListCustomerDigitalRange)

	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CBacnetCustomerDigitalRange message handlers


BOOL CBacnetCustomerDigitalRange::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	Initial_List();
	PostMessage(WM_REFRESH_BAC_CUSTOMER_DIGITAL_RANGE_LIST,NULL,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CBacnetCustomerDigitalRange::Initial_List()
{
	m_customer_dig_range_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_customer_dig_range_list.SetExtendedStyle(m_customer_dig_range_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_customer_dig_range_list.SetExtendedStyle(m_customer_dig_range_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_customer_dig_range_list.InsertColumn(CUSTOMER_DIGITAL_RANGE_NUM, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_customer_dig_range_list.InsertColumn(CUSTOMER_DIGITAL_RANGE_OFF, _T("Digital Off"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_customer_dig_range_list.InsertColumn(CUSTOMER_DIGITAL_RANGE_ON, _T("Digital On"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_customer_dig_range_list.InsertColumn(CUSTOMER_DIGITAL_RANGE_DIRECT_INV, _T("Direct_Invers"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	
	m_customer_digital_range_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_customer_digital_range_dlg_hwnd;

	CRect list_rect,win_rect;
	m_customer_dig_range_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_input_dlg_hwnd,win_rect);
	m_customer_dig_range_list.Set_My_WindowRect(win_rect);
	m_customer_dig_range_list.Set_My_ListRect(list_rect);


	m_customer_dig_range_list.DeleteAllItems();
	for (int i=0;i<(int)m_customer_unit_data.size();i++)
	{
		CString temp_item,temp_dig_off,temp_dig_on,temp_dir_inv;
		temp_item.Format(_T("%d"),i+1);
		m_customer_dig_range_list.InsertItem(i,temp_item);

		for (int x=0;x<CUSTOMER_DIGITAL_RANGE_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_customer_dig_range_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_customer_dig_range_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}



	}
}



BOOL CBacnetCustomerDigitalRange::PreTranslateMessage(MSG* pMsg)
{
	
	if((pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN))
	{
		CRect list_rect,win_rect;
		m_customer_dig_range_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_customer_digital_range_dlg_hwnd,win_rect);
		m_customer_dig_range_list.Set_My_WindowRect(win_rect);
		m_customer_dig_range_list.Set_My_ListRect(list_rect);

		m_customer_dig_range_list.Get_clicked_mouse_position();
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}



LRESULT CBacnetCustomerDigitalRange::Fresh_Customer_Digital_Range_List(WPARAM wParam,LPARAM lParam)
{
	for (int i=0;i<(int)m_customer_unit_data.size();i++)
	{
		CString temp_dig_off;
		CString temp_dig_on;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_customer_unit_data.at(i).digital_units_off, (int)strlen((char *)m_customer_unit_data.at(i).digital_units_off)+1, 
			temp_dig_off.GetBuffer(MAX_PATH), MAX_PATH );
		temp_dig_off.ReleaseBuffer();

		MultiByteToWideChar( CP_ACP, 0, (char *)m_customer_unit_data.at(i).digital_units_on, (int)strlen((char *)m_customer_unit_data.at(i).digital_units_on)+1, 
			temp_dig_on.GetBuffer(MAX_PATH), MAX_PATH );
		temp_dig_on.ReleaseBuffer();

		if(temp_dig_off.GetLength() >=12)
		{
			temp_dig_off.Empty();
			memset(m_customer_unit_data.at(i).digital_units_off,0,12);
		}

		if(temp_dig_on.GetLength() >=12)
		{
			temp_dig_on.Empty();
			memset(m_customer_unit_data.at(i).digital_units_on,0,12);
		}
		m_customer_dig_range_list.SetItemText(i,CUSTOMER_DIGITAL_RANGE_OFF,temp_dig_off);
		m_customer_dig_range_list.SetItemText(i,CUSTOMER_DIGITAL_RANGE_ON,temp_dig_on);

		CString temp_direct_inv;
		if(m_customer_unit_data.at(i).direct == 0)
		{
			temp_direct_inv = _T("Direct");
		}
		else if(m_customer_unit_data.at(i).direct == 1)
		{
			temp_direct_inv = _T("Invers");
		}
		else
		{
			temp_direct_inv.Empty();
		}

		m_customer_dig_range_list.SetItemText(i,CUSTOMER_DIGITAL_RANGE_DIRECT_INV,temp_direct_inv);
	}

	return 0;
}


LRESULT CBacnetCustomerDigitalRange::Fresh_Customer_Digital_Range_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	memcpy_s(&m_temp_customer_unite_data[Changed_Item],sizeof(Str_Units_element),&m_customer_unit_data.at(Changed_Item),sizeof(Str_Units_element));

	CString temp_task_info;
	CString New_CString =  m_customer_dig_range_list.GetItemText(Changed_Item,Changed_SubItem);
	CString cstemp_value;
	if(Changed_SubItem == CUSTOMER_DIGITAL_RANGE_OFF)
	{
		CString cs_temp = m_customer_dig_range_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= 12)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("String length can not greater than 11"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_CUSTOMER_DIGITAL_RANGE_LIST,NULL,NULL);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_customer_unit_data.at(Changed_Item).digital_units_off,12,cTemp1,12);
	}

	if(Changed_SubItem == CUSTOMER_DIGITAL_RANGE_ON)
	{
		CString cs_temp = m_customer_dig_range_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= 12)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("String length can not greater than 11"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_CUSTOMER_DIGITAL_RANGE_LIST,NULL,NULL);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_customer_unit_data.at(Changed_Item).digital_units_on,12,cTemp1,12);
	}


	cmp_ret = memcmp(&m_temp_customer_unite_data[Changed_Item],&m_customer_unit_data.at(Changed_Item),sizeof(Str_Units_element));
	if(cmp_ret!=0)
	{
		m_customer_dig_range_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Input List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEUNIT_T3000,Changed_Item,Changed_Item,sizeof(Str_Units_element),m_customer_digital_range_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
	}
	//m_input_list.Invalidate();
	return 0;
}




void CBacnetCustomerDigitalRange::OnNMClickListCustomerDigitalRange(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	

	long lRow,lCol;
	m_customer_dig_range_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_customer_dig_range_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_customer_dig_range_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_customer_dig_range_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;
	CString New_CString;
	CString temp_task_info;

	if(lCol == CUSTOMER_DIGITAL_RANGE_DIRECT_INV)
	{
		memcpy_s(&m_temp_customer_unite_data[lRow],sizeof(Str_Units_element),&m_customer_unit_data.at(lRow),sizeof(Str_Units_element));

		if(m_customer_unit_data.at(lRow).direct == 0)
		{

			m_customer_unit_data.at(lRow).direct = 1;
			m_customer_dig_range_list.SetItemText(lRow,CUSTOMER_DIGITAL_RANGE_DIRECT_INV,_T("Invers"));
			New_CString = _T("Invers");
		}
		else
		{
			m_customer_unit_data.at(lRow).direct = 0;
			m_customer_dig_range_list.SetItemText(lRow,CUSTOMER_DIGITAL_RANGE_DIRECT_INV,_T("Direct"));
			New_CString = _T("Direct");
		}

	}
	else
		return;

	m_customer_dig_range_list.Set_Edit(false);

	int cmp_ret = memcmp(&m_temp_customer_unite_data[lRow],&m_customer_unit_data.at(lRow),sizeof(Str_Units_element));
	if(cmp_ret!=0)
	{
		m_customer_dig_range_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Custom Digital Units Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEUNIT_T3000,(int8_t)lRow,(int8_t)lRow,sizeof(Str_Units_element),m_customer_digital_range_dlg_hwnd,temp_task_info,lRow,lCol);
	}

	*pResult = 0;
}


BOOL CBacnetCustomerDigitalRange::OnHelpInfo(HELPINFO* pHelpInfo)
{ 

	//if (g_protocol==PROTOCOL_BACNET_IP){
		HWND hWnd;

		if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, 			
			theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, 			
			HH_HELP_CONTEXT, IDH_TOPIC_6_9_TREND_LOGS);
		return (hWnd != NULL);
// 	}
// 	else{
// 		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
// 	}

	return CDialogEx::OnHelpInfo(pHelpInfo);
}