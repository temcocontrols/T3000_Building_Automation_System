// BacnetVariable.cpp : implementation file
// BacnetVariable.cpp Add by Fance 2013 08 21

#include "stdafx.h"
#include "T3000.h"
#include "BacnetVariable.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
#include "gloab_define.h"
#include "BacnetRange.h"
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.
IMPLEMENT_DYNAMIC(CBacnetVariable, CDialogEx)

CBacnetVariable::CBacnetVariable(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetVariable::IDD, pParent)
{

}

CBacnetVariable::~CBacnetVariable()
{
}

void CBacnetVariable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VARIABLE, m_variable_list);
}


BEGIN_MESSAGE_MAP(CBacnetVariable, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_VARIABLE_READ, &CBacnetVariable::OnBnClickedButtonVariableRead)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Variable_Item)
//	ON_MESSAGE(MY_RESUME_DATA, VariableResumeMessageCallBack)
	ON_MESSAGE(WM_REFRESH_BAC_VARIABLE_LIST,Fresh_Variable_List)
	ON_BN_CLICKED(IDC_BUTTON_VARIABLE_APPLY, &CBacnetVariable::OnBnClickedButtonVariableApply)
	ON_NOTIFY(NM_CLICK, IDC_LIST_VARIABLE, &CBacnetVariable::OnNMClickListVariable)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBacnetVariable message handlers


BOOL CBacnetVariable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_List();	//Initial the list of Variable,read from device;
	PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);

	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_BUTTON_VARIABLE_READ))->SetIcon(hIcon);	
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_OK);
	((CButton *)GetDlgItem(IDC_BUTTON_VARIABLE_APPLY))->SetIcon(hIcon);
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetVariable::OnBnClickedButtonVariableRead()
{
	// TODO: Add your control notification handler code here

	PostMessage(WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
}


void CBacnetVariable::Initial_List()
{
	m_variable_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_variable_list.SetExtendedStyle(m_variable_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_variable_list.InsertColumn(VARIABLE_NUM, _T("NUM"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_variable_list.InsertColumn(VARIABLE_FULL_LABLE, _T("Full Label"), 200, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_AUTO_MANUAL, _T("Auto/Manual"), 150, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_VALUE, _T("Value"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_UNITE, _T("Units"), 120, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_LABLE, _T("Label"), 130, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_variable_dlg_hwnd;

	m_variable_list.DeleteAllItems();
	for (int i=0;i<(int)m_Variable_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_variable_list.InsertItem(i,temp_item);
		if(ListCtrlEx::ComboBox == m_variable_list.GetColumnType(VARIABLE_AUTO_MANUAL))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(_T("Auto"));
			strlist.push_back(_T("Manual"));
			m_variable_list.SetCellStringList(i, VARIABLE_AUTO_MANUAL, strlist);
		}

		if(ListCtrlEx::ComboBox == m_variable_list.GetColumnType(VARIABLE_UNITE))
		{
			ListCtrlEx::CStrList strlist;

			for (int i=0;i<(int)sizeof(Units_Type)/sizeof(Units_Type[0]);i++)
			{
				strlist.push_back(Units_Type[i]);
			}
			m_variable_list.SetCellStringList(i, VARIABLE_UNITE, strlist);		
		}
	}
}


LRESULT CBacnetVariable::Fresh_Variable_List(WPARAM wParam,LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);

	//m_variable_list.DeleteAllItems();
	for (int i=0;i<(int)m_Variable_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;


		MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(i).description, (int)strlen((char *)m_Variable_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_variable_list.SetItemText(i,VARIABLE_FULL_LABLE,temp_des);
		if(m_Variable_data.at(i).auto_manual==0)
		{
			m_variable_list.SetItemText(i,VARIABLE_AUTO_MANUAL,_T("Auto"));
			m_variable_list.SetCellEnabled(i,VARIABLE_VALUE,0);
		}
		else
		{
			m_variable_list.SetItemText(i,VARIABLE_AUTO_MANUAL,_T("Manual"));
			m_variable_list.SetCellEnabled(i,VARIABLE_VALUE,1);
		}

		if(m_Variable_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{
			m_variable_list.SetItemText(i,VARIABLE_UNITE,Digital_Units_Array[m_Variable_data.at(i).range]);	//单位 这个要商量 看要怎么搞;
			if((m_Variable_data.at(i).range >= 12)&&(m_Variable_data.at(i).range <= 22))
			{
				CString temp1;
				CStringArray temparray;
				temp1 = Digital_Units_Array[m_Variable_data.at(i).range - 11];//11 is the sizeof the array
				SplitCStringA(temparray,temp1,_T("/"));
				if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
				{
					m_variable_list.SetItemText(i,VARIABLE_VALUE,temparray.GetAt(1));
				}
				m_variable_list.SetItemText(i,VARIABLE_UNITE,temp1);
			}
			else if((m_Variable_data.at(i).range >= 1)&&(m_Variable_data.at(i).range <= 11))
			{
				CString temp1;
				CStringArray temparray;
				temp1 = Digital_Units_Array[m_Variable_data.at(i).range];
				SplitCStringA(temparray,temp1,_T("/"));
				if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
				{
					m_variable_list.SetItemText(i,VARIABLE_VALUE,temparray.GetAt(0));
				}
				m_variable_list.SetItemText(i,VARIABLE_UNITE,temp1);
			}
			else
			{
				m_variable_list.SetItemText(i,VARIABLE_VALUE,_T("0"));
			}
		}
		else
		{
			if(m_Variable_data.at(i).range<=sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]))
			{
			m_variable_list.SetItemText(i,VARIABLE_UNITE,Variable_Analog_Units_Array[m_Variable_data.at(i).range]);
			temp_value.Format(_T("%d"),m_Variable_data.at(i).value);
			m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_value);
			}
		}
		
		





		CString temp_des2;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(i).label, (int)strlen((char *)m_Variable_data.at(i).label)+1, 
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des2.ReleaseBuffer();
		m_variable_list.SetItemText(i,VARIABLE_LABLE,temp_des2);

	}
	copy_data_to_ptrpanel(TYPE_VARIABLE);
	//MessageBox("1");
	return 0;
}
LRESULT CBacnetVariable::Fresh_Variable_Item(WPARAM wParam,LPARAM lParam)
{
	

	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	CString temp_task_info;
	CString New_CString =  m_variable_list.GetItemText(Changed_Item,Changed_SubItem);
	CString cstemp_value;
	if(Changed_SubItem == VARIABLE_LABLE)
	{
		CString cs_temp = m_variable_list.GetItemText(Changed_Item,Changed_SubItem);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Variable_data.at(Changed_Item).label,STR_VARIABLE_LABEL,cTemp1,STR_VARIABLE_LABEL);
	}

	if(Changed_SubItem == VARIABLE_FULL_LABLE)
	{
		CString cs_temp = m_variable_list.GetItemText(Changed_Item,Changed_SubItem);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Variable_data.at(Changed_Item).description,STR_VARIABLE_DESCRIPTION_LENGTH,cTemp1,STR_VARIABLE_DESCRIPTION_LENGTH);
	}


	if(Changed_SubItem == VARIABLE_AUTO_MANUAL)
	{
		CString temp_cs = m_variable_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(_T("Auto"))==0)
		{
			m_variable_list.SetCellEnabled(Changed_Item,VARIABLE_VALUE,0);
			m_Variable_data.at(Changed_Item).auto_manual = BAC_AUTO;
		}
		else
		{
			m_variable_list.SetCellEnabled(Changed_Item,VARIABLE_VALUE,1);
			m_Variable_data.at(Changed_Item).auto_manual = BAC_MANUAL;
		}
	}
	if(Changed_SubItem == VARIABLE_VALUE)//这里只用处理 ANALOG 的值就看要了， DIGITAL 的值在Click 事件中处理过了;
	{
		CString temp_cs = m_variable_list.GetItemText(Changed_Item,Changed_SubItem);
		int temp_int = _wtoi(temp_cs);
		m_Variable_data.at(Changed_Item).value = temp_int;
	}
	if(Changed_SubItem == VARIABLE_UNITE)
	{
		BacnetRange dlg;
		CString temp_cs = m_variable_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(Units_Type[UNITS_TYPE_ANALOG])==0)
		{
			m_Variable_data.at(Changed_Item).digital_analog = BAC_UNITS_ANALOG;
			bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
			dlg.DoModal();
			m_Variable_data.at(Changed_Item).range = bac_range_number_choose;
			m_variable_list.SetItemText(Changed_Item,Changed_SubItem,Variable_Analog_Units_Array[bac_range_number_choose]);

			cstemp_value.Format(_T("%d"),m_Variable_data.at(Changed_Item).value);
			m_variable_list.SetItemText(Changed_Item,VARIABLE_VALUE,cstemp_value);	
		}
		else if(temp_cs.CompareNoCase(Units_Type[UNITS_TYPE_DIGITAL])==0)
		{
			bac_ranges_type = VARIABLE_RANGE_DIGITAL_TYPE;
			dlg.DoModal();
			m_Variable_data.at(Changed_Item).digital_analog = BAC_UNITS_DIGITAL;
			m_Variable_data.at(Changed_Item).range = bac_range_number_choose;

			CString temp1;
			CStringArray temparray;
			temp1 = Digital_Units_Array[bac_range_number_choose];//11 is the sizeof the array
			SplitCStringA(temparray,temp1,_T("/"));

			

			if(m_Variable_data.at(Changed_Item).control == 1)
			{
				if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
				{
					m_variable_list.SetItemText(Changed_Item,VARIABLE_VALUE,temparray.GetAt(1));
				}
			}
			else
			{
				if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
				{
					m_variable_list.SetItemText(Changed_Item,VARIABLE_VALUE,temparray.GetAt(0));
				}			
			}
			m_variable_list.SetItemText(Changed_Item,VARIABLE_UNITE,temp1);

		}
		else if(temp_cs.CompareNoCase(Units_Type[UNITS_TYPE_CUSTOM])==0)
		{
			bac_ranges_type = VARIABLE_RANGE_CUSTOM_DIG_TYPE;
		}
	}

	temp_task_info.Format(_T("Write Variable List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
	Post_Write_Message(bac_gloab_device_id,WRITEVARIABLE_T3000,Changed_Item,Changed_Item,sizeof(Str_variable_point),BacNet_hwd ,temp_task_info);
	return 0;
}


void CBacnetVariable::OnBnClickedButtonVariableApply()
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<(int)m_Variable_data.size();i++)
	{
		CString cs_temp=m_variable_list.GetItemText(i,VARIABLE_FULL_LABLE);
		char cTemp[255];
		memset(cTemp,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 255, NULL, NULL );
		memcpy_s(m_Variable_data.at(i).description,21,cTemp,21);

		cs_temp=m_variable_list.GetItemText(i,VARIABLE_AUTO_MANUAL);
		if(cs_temp.CompareNoCase(_T("Auto"))==0)
		{
			m_Variable_data.at(i).auto_manual= BAC_AUTO;
			m_variable_list.SetCellEnabled(i,VARIABLE_VALUE,0);
		}
		else
		{
			m_Variable_data.at(i).auto_manual = BAC_MANUAL;
			m_variable_list.SetCellEnabled(i,VARIABLE_VALUE,1);
		}


		cs_temp=m_variable_list.GetItemText(i,VARIABLE_UNITE);
		int index_number=0;

		cs_temp=m_variable_list.GetItemText(i,VARIABLE_LABLE);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Variable_data.at(i).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);
	}
	Post_Write_Message(bac_gloab_device_id,WRITEVARIABLE_T3000,0,19,sizeof(Str_variable_point),BacNet_hwd);
		
}


void CBacnetVariable::OnNMClickListVariable(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	long lRow,lCol;
	m_variable_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_variable_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_variable_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_variable_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;

	if(lCol != VARIABLE_VALUE)	
		return;

	if(m_Variable_data.at(lRow).digital_analog != BAC_UNITS_DIGITAL)
		return;

	if(m_Variable_data.at(lRow).auto_manual == BAC_AUTO)	//If it is auto mode, disable to change the value.
		return;

	CString New_CString;
	CString temp_task_info;
	CString temp1;
	CStringArray temparray;
	if(m_Variable_data.at(lRow).range > 11)
		temp1 = Digital_Units_Array[m_Variable_data.at(lRow).range - 11];//11 is the sizeof the array
	else
		temp1 = Digital_Units_Array[m_Variable_data.at(lRow).range];
	SplitCStringA(temparray,temp1,_T("/"));
	if(m_Variable_data.at(lRow).range>=12)
	{

		if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
		{
			m_variable_list.SetItemText(lRow,VARIABLE_VALUE,temparray.GetAt(0));
			m_Variable_data.at(lRow).range = m_Variable_data.at(lRow).range - 11;
			m_Variable_data.at(lRow).control = 0;	
			New_CString = temparray.GetAt(0);
		}

	}
	else if(m_Variable_data.at(lRow).range>=1)
	{

		if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
		{
			m_variable_list.SetItemText(lRow,VARIABLE_VALUE,temparray.GetAt(1));
			m_Variable_data.at(lRow).range = m_Variable_data.at(lRow).range + 11;
			New_CString = temparray.GetAt(1);
			m_Variable_data.at(lRow).control = 1;
		}

	}
	m_variable_list.Set_Edit(false);

	temp_task_info.Format(_T("Write Variable List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
	Post_Write_Message(bac_gloab_device_id,WRITEVARIABLE_T3000,lRow,lRow,sizeof(Str_variable_point),BacNet_hwd,temp_task_info);
//	GetDlgItem(IDC_BUTTON_VARIABLE_APPLY)->SetFocus();
	*pResult = 0;
}





void CBacnetVariable::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	::PostMessage(m_variable_dlg_hwnd,WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
	Post_Refresh_Message(bac_gloab_device_id,READVARIABLE_T3000,0,63,sizeof(Str_variable_point),16);
	CDialogEx::OnTimer(nIDEvent);
}
