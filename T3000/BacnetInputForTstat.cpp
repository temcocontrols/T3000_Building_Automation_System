#include "stdafx.h"
#include "T3000.h"
#include "BacnetInput.h"
#include "globle_function.h"
#include <bitset>
#include "BuildTable1.h"
#include "TstatRangeDlg.h"
#include "bado/BADO.h"
LRESULT CBacnetInput::Initial_Input_List(WPARAM wParam,LPARAM lParam){
	if(g_protocol == PROTOCOL_BACNET_IP){
		Initial_List();
		PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
		SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	}
	else{

		Initial_ListFor_Tstat();
	}
	return 0;
}
void CBacnetInput::Initial_ListFor_Tstat(){
    m_input_list.ShowWindow(SW_HIDE);
	m_input_list.DeleteAllItems();
	while ( m_input_list.DeleteColumn (0)) ;

   if (product_type==CS3000)
   {
	   m_input_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	   m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	   m_input_list.InsertColumn(INPUT_NUM, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	   m_input_list.InsertColumn(INPUT_FULL_LABLE, _T("Full Label"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	   m_input_list.InsertColumn(INPUT_AUTO_MANUAL, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	   m_input_list.InsertColumn(INPUT_VALUE, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	   m_input_list.InsertColumn(INPUT_UNITE, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	   m_input_list.InsertColumn(INPUT_RANGE, _T("Range"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	   m_input_list.InsertColumn(INPUT_CAL, _T("Calibration"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	   m_input_list.InsertColumn(TSTAT_INPUT_FITLER, _T("Filter"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	   m_input_list.InsertColumn(TSTAT_INPUT_FUNCTION, _T("Function"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	   m_input_list.InsertColumn(TSTAT_INPUT_CUST_FIELD, _T("Custom Table"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

	   CString strTemp;
	   //m_input_list.DeleteAllItems();
	   for (int i=0;i<=1;i++)
	   {
		   strTemp.Format(_T("%d"),i+1);
		   m_input_list.InsertItem(i,strTemp);
		   for (int x=0;x<INPUT_COL_NUMBER;x++)
		   {
			   if((i%2)==0)
				   m_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			   else
				   m_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		   }
	   }

	   for (int i=0;i<=1;i++)
	   {
		   if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
		   {
			   ListCtrlEx::CStrList strlist;
			   for (int i=0;i<3;i++)
			   {
				   strlist.push_back(CS3000_INPUT_RANGE[i]);
			   }
			   m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);		
		   }
	   } 
   }
   else
   {
	m_input_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_input_list.InsertColumn(INPUT_NUM, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_input_list.InsertColumn(INPUT_FULL_LABLE, _T("Full Label"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_AUTO_MANUAL, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_VALUE, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_UNITE, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_RANGE, _T("Range"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_CAL, _T("Calibration"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(TSTAT_INPUT_FITLER, _T("Filter"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(TSTAT_INPUT_FUNCTION, _T("Function"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(TSTAT_INPUT_CUST_FIELD, _T("Custom Table"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

    CString strTemp;
	//m_input_list.DeleteAllItems();
	for (int i=0;i<=11;i++)
	{
		strTemp.Format(_T("%d"),i+1);
		m_input_list.InsertItem(i,strTemp);
		for (int x=0;x<INPUT_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}
	}
	
	for (int i=1;i<=8;i++)
	{
		if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(TSTAT_INPUT_FUNCTION))
		{
			ListCtrlEx::CStrList strlist;
			for (int i=0;i<(int)sizeof(INPUT_FUNS)/sizeof(INPUT_FUNS[0]);i++)
			{
				strlist.push_back(INPUT_FUNS[i]);
			}
			m_input_list.SetCellStringList(i, TSTAT_INPUT_FUNCTION, strlist);		
		}
	}

	}
	m_input_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_input_dlg_hwnd;
	CRect list_rect,win_rect;
	m_input_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_input_dlg_hwnd,win_rect);
	m_input_list.Set_My_WindowRect(win_rect);
	m_input_list.Set_My_ListRect(list_rect);

	m_input_list.ShowWindow(SW_SHOW);
}

LRESULT CBacnetInput::Fresh_Input_List_Tstat(WPARAM wParam,LPARAM lParam){
    //Initial_ListFor_Tstat();

//	int Fresh_Item;
//	int isFreshOne = (int)lParam;
//	if(isFreshOne == REFRESH_ON_ITEM)
//	{
//		Fresh_Item = (int)wParam;
//	}
//	else
//	{
//	    
//// 		if(m_input_list.IsDataNewer((char *)&m_Input_data.at(0),sizeof(Str_in_point) * BAC_INPUT_ITEM_COUNT))
//// 		{
//// 			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
//// 			//m_input_list.SetListData((char *)&m_Input_data.at(0),sizeof(Str_in_point) * BAC_INPUT_ITEM_COUNT);
//// 		}
//// 		else
//// 		{
//// 			return 0;
//// 		}
//	}
//
      Initial_ListFor_Tstat();
	 CString strTemp;
	 //
	 for(int i=0;i<(int)m_tstat_input_data.size();i++){
	 m_input_list.SetItemText(i,INPUT_FULL_LABLE,m_tstat_input_data.at(i).InputName.StrValue);
	 m_input_list.SetItemText(i,INPUT_AUTO_MANUAL,m_tstat_input_data.at(i).AM.StrValue);
	 m_input_list.SetItemText(i,INPUT_VALUE,m_tstat_input_data.at(i).Value.StrValue);
	 m_input_list.SetItemText(i,INPUT_UNITE,m_tstat_input_data.at(i).Unit.StrValue);
	 m_input_list.SetItemText(i,INPUT_RANGE,m_tstat_input_data.at(i).Range.StrValue);
	 m_input_list.SetItemText(i,INPUT_CAL,_T("Adjust..."));
	 m_input_list.SetItemText(i,TSTAT_INPUT_FITLER,m_tstat_input_data.at(i).Filter.StrValue);
	 m_input_list.SetItemText(i,TSTAT_INPUT_FUNCTION,m_tstat_input_data.at(i).Function.StrValue);
	 m_input_list.SetItemText(i,TSTAT_INPUT_CUST_FIELD,m_tstat_input_data.at(i).CustomTable.StrValue);
	 }
	 m_input_list.GetFocus();
	 return 0;
}
LRESULT CBacnetInput::Fresh_Input_Item_Tstat(WPARAM wParam,LPARAM lParam)
{
//	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	CString New_CString =  m_input_list.GetItemText(Changed_Item,Changed_SubItem);
	 
	BOOL IS_SEND=FALSE;
	_MessageWriteOneInfo_List  *pwrite_info = new _MessageWriteOneInfo_List;
	pwrite_info->list_type=LIST_TYPE_INPUT_TSTAT;
	if(Changed_SubItem == INPUT_FULL_LABLE)
	{
		if (product_type==CS3000)
		{
			IS_SEND=FALSE;
			m_input_list.Set_Edit(false);
			return 0;
		}
		CString cs_temp = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()> STR_IN_LABEL)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not greater than 8"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
			return 0;
		}

	   if (m_tstat_input_data.at(Changed_Item).InputName.StrValue.CompareNoCase(cs_temp)==0)
	   {
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
			return 0; 
		}

	   char cTemp1[8];
	   memset(cTemp1,0,8);
	   WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 8, NULL, NULL );

	   _MessageWriteMultiInfo_List *pwrite_info = new _MessageWriteMultiInfo_List;
	   pwrite_info->device_id=g_tstat_id;
	   pwrite_info->Changed_Name.Format(_T("Change Inputname%d from %s to %s"),Changed_Item,m_tstat_input_data.at(Changed_Item).InputName.StrValue,cs_temp);
	   pwrite_info->hwnd=m_input_dlg_hwnd;
	   pwrite_info->mRow=Changed_Item;
	   pwrite_info->mCol=Changed_SubItem;
	   pwrite_info->Start_Address=m_tstat_input_data.at(Changed_Item).InputName.regAddress;
	   memcpy_s(pwrite_info->RegValue,STR_IN_LABEL-1,cTemp1,STR_IN_LABEL-1);
	   m_input_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
	   if(!PostThreadMessage(nThreadID,MY_WRITE_MULTI_LIST,(WPARAM)pwrite_info,NULL))//post thread msg
	   {
		   return FALSE;
	   }
	   else
	   {
		   return TRUE;
	   }

	}
	if(Changed_SubItem== INPUT_VALUE)//Manual + 模拟量
	{
	   if (m_tstat_input_data.at(Changed_Item).Value.StrValue.CompareNoCase(New_CString)==0)
	   {
		   m_input_list.Set_Edit(false);
		   return 0;
	   }
	   
	   int SendValue=0;
	   float new_fvalue=_wtof(New_CString);
	   if (product_type == CS3000)
	   {
		   if (product_register_value[7]==PM_CS_RSM_AC||product_register_value[7]==PM_CS_SM_AC)
		   {
			   SendValue = (int)new_fvalue*100;
		   } 
		   else
		   {
			   SendValue = (int)new_fvalue*10;
		   }

	   } 
	   else
	   {
		   if (m_tstat_input_data.at(Changed_Item).Range.StrValue.CompareNoCase(_T("0-100%"))==0)
		   {
			   SendValue=(int)new_fvalue;
		   }
		   else{
			   SendValue=(int)new_fvalue*10;
		   }
	   }
	   pwrite_info->Changed_Name.Format(_T("'s Value,From %s to %s"),m_tstat_input_data.at(Changed_Item).InputName.StrValue,m_tstat_input_data.at(Changed_Item).Value.StrValue,New_CString);
	   pwrite_info->address=m_tstat_input_data.at(Changed_Item).Value.regAddress;
	   pwrite_info->new_value=SendValue;
	   IS_SEND=TRUE;
	}
	if (Changed_SubItem==INPUT_CAL)
	{   
	    if(New_CString.IsEmpty()||m_tstat_input_data.at(Changed_Item).Value.StrValue.CompareNoCase(New_CString)==0)
	    {
		m_input_list.SetItemText(Changed_Item,Changed_SubItem,L"Adjust...");
		return 0;
	    }
	    
		int SendValue=0;
		float new_fvalue=_wtof(New_CString);
		if (product_type == CS3000)
		{
			  if(product_register_value[7]==PM_CS_RSM_AC||product_register_value[7]==PM_CS_SM_AC)
			 {
			   SendValue = (int)new_fvalue*100;
			 } 
			 else
			 {
			   SendValue = (int)new_fvalue*10;
			 }
			
		} 
		else
		{
			if (m_tstat_input_data.at(Changed_Item).Range.StrValue.CompareNoCase(_T("0-100%"))==0)
			{
				SendValue=(int)new_fvalue;
			}
			else{
				SendValue=(int)new_fvalue*10;
			}
		}

		pwrite_info->Changed_Name.Format(_T("Calibration %s 's Value,From %s to %s"),m_tstat_input_data.at(Changed_Item).InputName.StrValue,m_tstat_input_data.at(Changed_Item).Value.StrValue,New_CString);
		pwrite_info->address=m_tstat_input_data.at(Changed_Item).Value.regAddress;
		pwrite_info->new_value=SendValue;
		IS_SEND=TRUE;
	}
	if (Changed_SubItem == TSTAT_INPUT_FITLER)
	{
		if (New_CString.IsEmpty()||m_tstat_input_data.at(Changed_Item).Filter.StrValue.CompareNoCase(New_CString)==0)
		{
		   m_input_list.Set_Edit(false);
			//m_input_list.SetItemText(Changed_Item,Changed_SubItem,L"Adjust...");
			return 0;
		}
		int SendValue=0;
		    SendValue=_wtoi(New_CString);
		pwrite_info->Changed_Name.Format(_T(" %s 's Filter,From %s to %s"),m_tstat_input_data.at(Changed_Item).InputName.StrValue,m_tstat_input_data.at(Changed_Item).Filter.StrValue,New_CString);
		pwrite_info->address=m_tstat_input_data.at(Changed_Item).Filter.regAddress;
		pwrite_info->new_value=SendValue;
		IS_SEND=TRUE;
	}
	if (Changed_SubItem == TSTAT_INPUT_FUNCTION)
	{
	     if (m_tstat_input_data.at(Changed_Item).Function.StrValue.CompareNoCase(New_CString)==0)
	     {
		  m_input_list.Set_Edit(false);
		  return 0;
	     }
		 int func=0;
		 for (int i=0;i<8;i++)
		 {
		   if (New_CString.CompareNoCase(INPUT_FUNS[i])==0)
		   {
		     func=i;
			 break;
		   }
		   
		 }
		 
		 pwrite_info->Changed_Name.Format(_T("%s 's Function,From %s to %s"),m_tstat_input_data.at(Changed_Item).InputName.StrValue,m_tstat_input_data.at(Changed_Item).Function.StrValue,New_CString);
		 pwrite_info->address=m_tstat_input_data.at(Changed_Item).Function.regAddress;
		 pwrite_info->new_value=func;
	     IS_SEND = TRUE;
	    
	}
	if (Changed_SubItem == INPUT_RANGE) 
	{
	  if (product_type==CS3000)
	  {
	    if (m_tstat_input_data.at(Changed_Item).Range.Reg_Property==REG_READ_ONLY)
	    {
			IS_SEND=FALSE;
			m_input_list.Set_Edit(false);
			return 0;
	    }
	    int sendvalue=-1;
	    for (int i=0;i<3;i++)
	    {
		  if (New_CString.CompareNoCase(CS3000_INPUT_RANGE[i])==0)
		  {
		     sendvalue=i;
			 break;
		  }
		  
	    }
	    if (sendvalue!=-1)
	    {
			pwrite_info->Changed_Name.Format(_T("%s 's Range,From %s to %s"),m_tstat_input_data.at(Changed_Item).InputName.StrValue,m_tstat_input_data.at(Changed_Item).Range.StrValue,New_CString);
			pwrite_info->address=m_tstat_input_data.at(Changed_Item).Range.regAddress;
			pwrite_info->new_value=sendvalue;
			 IS_SEND = TRUE;
	    }
	    
	  }
	  
	}
	
	if (IS_SEND)
	{	
		pwrite_info->device_id=g_tstat_id;
		pwrite_info->hwnd=m_input_dlg_hwnd;
		pwrite_info->mCol=Changed_SubItem;
		pwrite_info->mRow=Changed_Item;
		m_input_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		if(!PostThreadMessage(nThreadID,MY_WRITE_ONE_LIST,(WPARAM)pwrite_info,NULL))//post thread msg
		{
			return FALSE ;
		}
		else
		{
			return TRUE ;
		}
	}
 return 0;
}
void CBacnetInput::OnNMClickList_Tstat(NMHDR *pNMHDR, LRESULT *pResult){
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

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_input_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;
	BOOL IS_SEND=FALSE;
	CString temp1;
	CStringArray temparray;
	_MessageWriteOneInfo_List  *pwrite_info = new _MessageWriteOneInfo_List;
	pwrite_info->list_type=LIST_TYPE_INPUT_TSTAT;
	if(lCol == INPUT_FULL_LABLE)
	{
	   if (product_type==CS3000||lRow==0||lRow==9||lRow==10||lRow==11)
	   {
	     m_input_list.Set_Edit(false);
		 IS_SEND=FALSE;
	     return;
	   //goto ENDCLICK;
	   }   
	}
	if (lCol == INPUT_VALUE)
	{
		if (product_type == T3000_6_ADDRESS)
		{
			if (m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(NO_APPLICATION)==0)
			{
				IS_SEND=FALSE;
				m_input_list.Set_Edit(false);
				return;
			}
			if (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(NO_APPLICATION)==0)
			{
				IS_SEND=FALSE;
				m_input_list.Set_Edit(false);
				return;
			}
			if (m_tstat_input_data.at(lRow).AM.StrValue.CompareNoCase(_T("Auto"))==0)
			{
				IS_SEND=FALSE;
				m_input_list.Set_Edit(false);
				return;
			}
			//Auto && 开关量 --不可以写值
			if (m_tstat_input_data.at(lRow).AM.StrValue.CompareNoCase(_T("Auto"))!=0&&
				(
				m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("On"))==0||
				m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Off"))==0||
				m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Occupied"))==0||
				m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Unoccupied"))==0||
				m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Open"))==0||
				m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Close"))==0
				)
				)
			{
				//Manual 的开关量
				if (
					m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("On"))==0||
					// m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Off"))==0||
					m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Occupied"))==0||
					//m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Unoccupied"))==0||
					m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Open"))==0
					// m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Close"))==0
					){
						pwrite_info->Changed_Name.Format(_T("%s's Value,From 1 to 0"),lRow,m_tstat_input_data.at(lRow).InputName.StrValue);
						pwrite_info->address=m_tstat_input_data.at(lRow).Value.regAddress;
						pwrite_info->new_value=0;
				}
				else{
					pwrite_info->Changed_Name.Format(_T("%s's Value,From 0 to 1"),lRow,m_tstat_input_data.at(lRow).InputName.StrValue);
					pwrite_info->address=m_tstat_input_data.at(lRow).Value.regAddress;
					pwrite_info->new_value=1;
				}  
				IS_SEND=TRUE;
				m_input_list.Set_Edit(false);
			} 
		}
	}
	if (lCol == INPUT_UNITE)
	{
	IS_SEND=FALSE;
	return;
	}
	if (lCol == INPUT_RANGE) 
	{
	    if(m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(NO_APPLICATION)==0){
			m_input_list.Set_Edit(false);
			IS_SEND=FALSE;
			return;
		}
	    if (product_type != CS3000)
	    {
			if (lRow==0)
			{
				if (m_tstat_input_data.at(lRow).Range.RegValue==0)
				{
					pwrite_info->Changed_Name.Format(_T("%s's Range,From 0 to 1"),m_tstat_input_data.at(lRow).InputName.StrValue);
					pwrite_info->address=m_tstat_input_data.at(lRow).Range.regAddress;
					pwrite_info->new_value=1;
				}
				else
				{
					pwrite_info->Changed_Name.Format(_T("%s's Range,From 1 to 0"),m_tstat_input_data.at(lRow).InputName.StrValue);
					pwrite_info->address=m_tstat_input_data.at(lRow).Range.regAddress;
					pwrite_info->new_value=0;

				}
				IS_SEND=TRUE;
			}

			if(lRow>=1&&lRow<=8)
			{
				CTstatRangeDlg   dlg;
				dlg.m_current_range=m_tstat_input_data.at(lRow).Range.RegValue;
				for (int i=0;i<11;i++)
				{
					if (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(analog_range[i])==0)
					{
						dlg.m_current_range=i;
						break;
					}

				}

				if (IDOK==dlg.DoModal())
				{   int realRange,dbRange;
				int range=dlg.m_current_range;
				//m_input_list.SetItemText(lRow,lCol,analog_range[range]);
				realRange=range;

				if (range==9||range==7)
				{
					realRange=3;
				}
				if (range==10||range==8)
				{
					realRange=5;
				}
				if (range==4||range==6)
				{
					realRange=4;
				}
				dbRange=range;
				pwrite_info->Changed_Name.Format(_T("%s's Range,From %s to %s"),m_tstat_input_data.at(lRow).InputName.StrValue,m_tstat_input_data.at(lRow).Range.StrValue,analog_range[range]);
				pwrite_info->address=m_tstat_input_data.at(lRow).Range.regAddress;
				pwrite_info->new_value=realRange;
				pwrite_info->db_value=dbRange;
				IS_SEND=TRUE;
				}
				else{
					IS_SEND=FALSE;
				}

			}
	    }
	    //IS_SEND=FALSE;
	//return;
	}
	if (lCol == INPUT_CAL)
	{

	   if (product_type != CS3000){
	   //开关量和不可用，无应用
	   if((m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(NO_APPLICATION)==0)||
	      (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("UNUSED"))==0)||
		  (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("On/Off"))==0)||
		  (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("Off/On"))==0)||
		  (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("Occupied/Unoccupied"))==0)||
		  (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("Unoccupied/Occupied"))==0)||
		  (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("Close/Open"))==0)||
		  (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("Open/Close"))==0)
		  )
	   {
	   m_input_list.Set_Edit(false);
	   IS_SEND=FALSE;
	   return;
	   }
	   }
	   else{
		   //模拟量
		   m_input_list.SetItemText(lRow,lCol,L"");
		   m_input_list.Set_Edit(TRUE);
	   }
	   
	}
	if (lCol == INPUT_AUTO_MANUAL)
	{
	   if (m_tstat_input_data.at(lRow).AM.StrValue.CompareNoCase(NO_APPLICATION)==0)
	   {
	   IS_SEND=FALSE;
	   return;
	   }

	   if (lRow==0||lRow==9||lRow==10||lRow==11)
	   {	 
		   if(m_tstat_input_data.at(lRow).AM.StrValue.CompareNoCase(_T("Manual"))!=0)
		   {

			   pwrite_info->Changed_Name.Format(_T("%s,From %s to Manual"),m_tstat_input_data.at(lRow).InputName.StrValue,m_tstat_input_data.at(lRow).AM.StrValue);
			   pwrite_info->address=m_tstat_input_data.at(lRow).AM.regAddress;
			   pwrite_info->new_value=1;

		   } 
		   else
		   {
			   pwrite_info->Changed_Name.Format(_T("%s,From %s to Auto"),m_tstat_input_data.at(lRow).InputName.StrValue,m_tstat_input_data.at(lRow).AM.StrValue);
			   pwrite_info->address=m_tstat_input_data.at(lRow).AM.regAddress;
			   pwrite_info->new_value=0;
		   }   
	   }
	   if (lRow>=1&&lRow<=8)
	   {
	       
		    int AM=1;
		    bitset<16> RegValue(m_tstat_input_data.at(lRow).AM.RegValue);
		   if(m_tstat_input_data.at(lRow).AM.StrValue.CompareNoCase(_T("Manual"))!=0)
		   {

			   pwrite_info->Changed_Name.Format(_T("%s,From %s to Manual"),m_tstat_input_data.at(lRow).InputName.StrValue,m_tstat_input_data.at(lRow).AM.StrValue);
			   pwrite_info->address=m_tstat_input_data.at(lRow).AM.regAddress;   
			   RegValue[lRow-1]=true;
			   pwrite_info->new_value=(short)RegValue.to_ulong();

		   } 
		   else
		   {
			   pwrite_info->Changed_Name.Format(_T("%s,From %s to Auto"),m_tstat_input_data.at(lRow).InputName.StrValue,m_tstat_input_data.at(lRow).AM.StrValue);
			   pwrite_info->address=m_tstat_input_data.at(lRow).AM.regAddress;
			   RegValue[lRow-1]=FALSE;
			   pwrite_info->new_value=(short)RegValue.to_ulong();
		   }   
	   }
	    
	   
	   IS_SEND=TRUE;
	   

	}
	if (lCol == TSTAT_INPUT_CUST_FIELD)
	{
		if (m_tstat_input_data.at(lRow).CustomTable.StrValue.CompareNoCase(NO_APPLICATION)==0)
		{
			IS_SEND=FALSE;
			return;
		}
		else if (m_tstat_input_data.at(lRow).CustomTable.StrValue.CompareNoCase(L"Custom1...")==0)
		{
			CBuildTable1 Dlg(2);
			Dlg.DoModal();
		}
		else if (m_tstat_input_data.at(lRow).CustomTable.StrValue.CompareNoCase(L"Custom2...")==0)
		{
			CBuildTable1 Dlg(3);
			Dlg.DoModal();
		}
		
		
	   
	}
	if (lCol == INPUT_FITLER)
	{
		if(m_tstat_input_data.at(lRow).Filter.StrValue.CompareNoCase(NO_APPLICATION)==0){
			m_input_list.Set_Edit(false);
			IS_SEND=FALSE;
			return;
		}

	}
	 
	if (lCol == TSTAT_INPUT_FUNCTION)
	{ 
		if(m_tstat_input_data.at(lRow).Function.StrValue.CompareNoCase(NO_APPLICATION)==0){
			m_input_list.Set_Edit(false);
			IS_SEND=FALSE;
			return;
		}

	    if (lRow>=1&&lRow<=8)
	    {
	    }
		else{
		m_input_list.Set_Edit(false);
		return;
		}
	    
	}
	
	
	if (IS_SEND)
	{	
	if (pwrite_info->address==NO_ADDRESS)
	{
		m_input_list.Set_Edit(false);
		return;
	}
	
	pwrite_info->device_id=g_tstat_id;
	pwrite_info->hwnd=m_input_dlg_hwnd;
	pwrite_info->mCol=lCol;
	pwrite_info->mRow=lRow;
	m_input_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
	if(!PostThreadMessage(nThreadID,MY_WRITE_ONE_LIST,(WPARAM)pwrite_info,NULL))//post thread msg
	{
		return  ;
	}
	else
	{
		return  ;
	}
	}
	else{
	if (pwrite_info)
	{
	   delete pwrite_info;
	}
	
	}

// ENDCLICK:
// 	m_input_list.Set_Edit(false);

}
LRESULT CBacnetInput::InputMessageCallBack_Tstat(WPARAM wParam, LPARAM lParam){
  int msg_result=(int) wParam;

  CString temp_task_info;
  CString Show_Results;
  if (msg_result==1||msg_result==2)//单写
  {
    _MessageWriteOneInfo_List *pInvoke =(_MessageWriteOneInfo_List *)lParam;
	CString temp_cs = pInvoke->Changed_Name;
	if (msg_result==1)
	{
		Show_Results = temp_cs + _T(" Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results); 
	}
	else
	{
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}
	if (product_type == CS3000)
	{
	LoadInputData_CS3000();
	}
	
	
	Fresh_One_Item(pInvoke->mRow);

	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);

	m_input_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


	if(pInvoke)
		delete pInvoke;
  }
  if (msg_result==3||msg_result==4)//多写
  { 
    _MessageWriteMultiInfo_List *pInvoke =(_MessageWriteMultiInfo_List *)lParam;
	
	CString temp_cs = pInvoke->Changed_Name;
	if (msg_result==3){
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);   
	}
	else{
	    Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}




	Fresh_One_Item(pInvoke->mRow);




	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);

	m_input_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


	if(pInvoke)
		delete pInvoke;

  }
  return 0;
}

void CBacnetInput::Fresh_One_Item(int row){
	m_input_list.SetItemText(row,INPUT_FULL_LABLE,m_tstat_input_data.at(row).InputName.StrValue);
	m_input_list.SetItemText(row,INPUT_AUTO_MANUAL,m_tstat_input_data.at(row).AM.StrValue);
	m_input_list.SetItemText(row,INPUT_VALUE,m_tstat_input_data.at(row).Value.StrValue);
	m_input_list.SetItemText(row,INPUT_UNITE,m_tstat_input_data.at(row).Unit.StrValue);
	m_input_list.SetItemText(row,INPUT_RANGE,m_tstat_input_data.at(row).Range.StrValue);
	m_input_list.SetItemText(row,INPUT_CAL,_T("Adjust..."));
	m_input_list.SetItemText(row,TSTAT_INPUT_FITLER,m_tstat_input_data.at(row).Filter.StrValue);
	m_input_list.SetItemText(row,TSTAT_INPUT_FUNCTION,m_tstat_input_data.at(row).Function.StrValue);
	m_input_list.SetItemText(row,TSTAT_INPUT_CUST_FIELD,m_tstat_input_data.at(row).CustomTable.StrValue);
}
 