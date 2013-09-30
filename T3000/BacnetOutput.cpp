// BacnetOutput.cpp : implementation file
// Programming by Fance  2013 08 

#include "stdafx.h"
#include "T3000.h"
#include "BacnetOutput.h"
#include "afxdialogex.h"

#include "afxdialogex.h"
#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
#include "gloab_define.h"
#include "BacnetRange.h"
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.

IMPLEMENT_DYNAMIC(CBacnetOutput, CDialogEx)

CBacnetOutput::CBacnetOutput(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetOutput::IDD, pParent)
{

}

CBacnetOutput::~CBacnetOutput()
{
}

void CBacnetOutput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_output_list);
}


BEGIN_MESSAGE_MAP(CBacnetOutput, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_READ, &CBacnetOutput::OnBnClickedButtonOutputRead)
	ON_MESSAGE(WM_REFRESH_BAC_OUTPUT_LIST,Fresh_Output_List)	
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Output_Item)	
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_APPLY, &CBacnetOutput::OnBnClickedButtonOutputApply)
	ON_NOTIFY(NM_CLICK, IDC_LIST_OUTPUT, &CBacnetOutput::OnNMClickListOutput)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CBacnetOutput message handlers


BOOL CBacnetOutput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_List();
	PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);

	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_BUTTON_OUTPUT_READ))->SetIcon(hIcon);	
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_OK);
	((CButton *)GetDlgItem(IDC_BUTTON_OUTPUT_APPLY))->SetIcon(hIcon);
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetOutput::Initial_List()
{
	m_output_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_output_list.SetExtendedStyle(m_output_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_output_list.InsertColumn(OUTPUT_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_output_list.InsertColumn(OUTPUT_FULL_LABLE, _T("Full Label"), 140, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_AUTO_MANUAL, _T("Auto/Manual"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_VALUE, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_UNITE, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_RANGE, _T("Range"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_0_PERSENT, _T("0%"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_100_PERSENT, _T("100%"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_DECOM, _T("Status"), 70, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_LABLE, _T("Label"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_output_dlg_hwnd;

	m_output_list.DeleteAllItems();
	for (int i=0;i<(int)m_Output_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_output_list.InsertItem(i,temp_item);
		if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_AUTO_MANUAL))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(_T("Auto"));
			strlist.push_back(_T("Manual"));
			m_output_list.SetCellStringList(i, OUTPUT_AUTO_MANUAL, strlist);
		}

		if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_RANGE))
		{
			ListCtrlEx::CStrList strlist;
			for (int i=0;i<(int)sizeof(Units_Type)/sizeof(Units_Type[0]);i++)
			{
				strlist.push_back(Units_Type[i]);
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
	}
}

void CBacnetOutput::OnBnClickedButtonOutputRead()
{
		PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
}



LRESULT CBacnetOutput::Fresh_Output_List(WPARAM wParam,LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);

//	m_output_list.DeleteAllItems();
	for (int i=0;i<(int)m_Output_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

		MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(i).description, (int)strlen((char *)m_Output_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_output_list.SetItemText(i,OUTPUT_FULL_LABLE,temp_des);

		if(m_Output_data.at(i).auto_manual==0)	//In output table if it is auto ,the value can't be edit by user
		{
			m_output_list.SetItemText(i,OUTPUT_AUTO_MANUAL,_T("Auto"));
			m_output_list.SetCellEnabled(i,OUTPUT_VALUE,0);
		}
		else
		{
			m_output_list.SetItemText(i,OUTPUT_AUTO_MANUAL,_T("Manual"));
			m_output_list.SetCellEnabled(i,OUTPUT_VALUE,1);
		}

			m_output_list.SetCellEnabled(i,OUTPUT_UNITE,0);
		if(m_Output_data.at(i).digital_analog == BAC_UNITS_ANALOG)
		{
			m_output_list.SetCellEnabled(i,OUTPUT_0_PERSENT,1);
			m_output_list.SetCellEnabled(i,OUTPUT_100_PERSENT,1);

			m_output_list.SetItemText(i,OUTPUT_RANGE,OutPut_List_Analog_Range[m_Output_data.at(i).range]);
			m_output_list.SetItemText(i,OUTPUT_UNITE,OutPut_List_Analog_Units[m_Output_data.at(i).range]);

			CString temp_low,temp_high;
			temp_low.Format(_T("%d"),m_Output_data.at(i).m_del_low);
			temp_high.Format(_T("%d"),m_Output_data.at(i).s_del_high);

			CString temp_value;
			temp_value.Format(_T("%d"),m_Output_data.at(i).value);
			m_output_list.SetItemText(i,OUTPUT_VALUE,temp_value);
			m_output_list.SetItemText(i,OUTPUT_0_PERSENT,temp_low);
			m_output_list.SetItemText(i,OUTPUT_100_PERSENT,temp_high);
		}
		else if(m_Output_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{
			m_output_list.SetCellEnabled(i,OUTPUT_0_PERSENT,0);
			m_output_list.SetCellEnabled(i,OUTPUT_100_PERSENT,0);
			m_output_list.SetItemText(i,OUTPUT_0_PERSENT,_T(""));
			m_output_list.SetItemText(i,OUTPUT_100_PERSENT,_T(""));
			m_output_list.SetItemText(i,OUTPUT_RANGE,Digital_Units_Array[m_Output_data.at(i).range]);

			m_output_list.SetItemText(i,OUTPUT_UNITE,_T(""));

			if((m_Output_data.at(i).range>=12)&&(m_Output_data.at(i).range<=22))
			{
				CString temp1;
				CStringArray temparray;
				temp1 = Digital_Units_Array[m_Output_data.at(i).range - 11];//11 is the sizeof the array
				SplitCStringA(temparray,temp1,_T("/"));
				if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
				{
					m_output_list.SetItemText(i,OUTPUT_VALUE,temparray.GetAt(1));
				}
				m_output_list.SetItemText(i,OUTPUT_RANGE,temp1);
			}
			else if((m_Output_data.at(i).range>=1)&&(m_Output_data.at(i).range<=11))
			{
				CString temp1;
				CStringArray temparray;
				temp1 = Digital_Units_Array[m_Output_data.at(i).range];
				SplitCStringA(temparray,temp1,_T("/"));
				if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
				{
					m_output_list.SetItemText(i,OUTPUT_VALUE,temparray.GetAt(0));
				}
				m_output_list.SetItemText(i,OUTPUT_RANGE,temp1);
			}
			else
			{
				m_output_list.SetItemText(i,OUTPUT_UNITE,Output_Analog_Units_Show[0]);
			}

		}

		if(m_Output_data.at(i).decom==0)
			temp_status.Format(Output_Decom_Array[0]);
		else if(m_Output_data.at(i).decom==1)
			temp_status.Format(Output_Decom_Array[1]);
		else
			temp_status.Empty();
		m_output_list.SetItemText(i,OUTPUT_DECOM,temp_status);


		CString temp_des2;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(i).label, (int)strlen((char *)m_Output_data.at(i).label)+1, 
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des2.ReleaseBuffer();
		m_output_list.SetItemText(i,OUTPUT_LABLE,temp_des2);

	}
	copy_data_to_ptrpanel(TYPE_OUTPUT);
	return 0;
}


void CBacnetOutput::OnBnClickedButtonOutputApply()
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<(int)m_Output_data.size();i++)
	{
		CString cs_temp=m_output_list.GetItemText(i,OUTPUT_FULL_LABLE);
		char cTemp[255];
		memset(cTemp,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 255, NULL, NULL );
		memcpy_s(m_Output_data.at(i).description,STR_OUT_DESCRIPTION_LENGTH,cTemp,STR_OUT_DESCRIPTION_LENGTH);


		cs_temp=m_output_list.GetItemText(i,OUTPUT_AUTO_MANUAL);
		if(cs_temp.CompareNoCase(_T("Auto"))==0)
		{
			m_Output_data.at(i).auto_manual=0;
		}
		else
		{
			m_Output_data.at(i).auto_manual=1;
		}

		cs_temp = m_output_list.GetItemText(i,OUTPUT_VALUE);
		m_Output_data.at(i).value = _wtoi(cs_temp);

		cs_temp = m_output_list.GetItemText(i,OUTPUT_0_PERSENT);
		m_Output_data.at(i).m_del_low = _wtoi(cs_temp);
		cs_temp = m_output_list.GetItemText(i,OUTPUT_100_PERSENT);
		m_Output_data.at(i).s_del_high = _wtoi(cs_temp);


		cs_temp=m_output_list.GetItemText(i,OUTPUT_LABLE);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Output_data.at(i).label,STR_OUT_LABEL,cTemp1,STR_OUT_LABEL);


	}
	Post_Write_Message(bac_gloab_device_id,WRITEOUTPUT_T3000,0,19,sizeof(Str_out_point),BacNet_hwd);

}

LRESULT CBacnetOutput::Fresh_Output_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	CString temp_task_info;
	CString New_CString =  m_output_list.GetItemText(Changed_Item,Changed_SubItem);
	CString cstemp_value;

	m_output_list.SetBkColor(RGB(255,0,0));

	if(Changed_SubItem == OUTPUT_LABLE)
	{
		CString cs_temp = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Output_data.at(Changed_Item).label,STR_OUT_LABEL,cTemp1,STR_OUT_LABEL);
	}

	if(Changed_SubItem == OUTPUT_FULL_LABLE)
	{
		CString cs_temp = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
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
			m_output_list.SetCellEnabled(Changed_Item,OUTPUT_VALUE,0);
			m_Output_data.at(Changed_Item).auto_manual = BAC_AUTO ;
		}
		else
		{
			m_output_list.SetCellEnabled(Changed_Item,OUTPUT_VALUE,1);
			m_Output_data.at(Changed_Item).auto_manual = BAC_MANUAL ;
		}
	}

	if(Changed_SubItem == OUTPUT_VALUE)
	{
		CString temp_cs = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
		int temp_int = _wtoi(temp_cs);
		m_Output_data.at(Changed_Item).value = temp_int;
	}

	if(Changed_SubItem == OUTPUT_RANGE)
	{
		CString temp_cs = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
		BacnetRange dlg;
		if(temp_cs.CompareNoCase(Units_Type[UNITS_TYPE_ANALOG])==0)
		{
			bac_ranges_type = OUTPUT_RANGE_ANALOG_TYPE;
			dlg.DoModal();
			m_Output_data.at(Changed_Item).digital_analog =  BAC_UNITS_ANALOG;
			m_Output_data.at(Changed_Item).range =  bac_range_number_choose;
			m_output_list.SetItemText(Changed_Item,OUTPUT_UNITE,Output_Analog_Units_Show[bac_range_number_choose]);		
			m_output_list.SetItemText(Changed_Item,OUTPUT_RANGE,OutPut_List_Analog_Range[bac_range_number_choose]);	

			m_output_list.SetItemText(Changed_Item,OUTPUT_0_PERSENT,_T("0"));
			m_output_list.SetCellEnabled(Changed_Item,OUTPUT_0_PERSENT,1);
			m_output_list.SetItemText(Changed_Item,OUTPUT_100_PERSENT,_T("10"));
			m_output_list.SetCellEnabled(Changed_Item,OUTPUT_100_PERSENT,1);

			cstemp_value.Format(_T("%d"),m_Output_data.at(Changed_Item).value);
			m_output_list.SetItemText(Changed_Item,OUTPUT_VALUE,cstemp_value);	

		}
		else if(temp_cs.CompareNoCase(Units_Type[UNITS_TYPE_DIGITAL])==0)
		{
			bac_ranges_type = OUTPUT_RANGE_DIGITAL_TYPE;
			dlg.DoModal();

			m_Output_data.at(Changed_Item).digital_analog =  BAC_UNITS_DIGITAL;
			m_Output_data.at(Changed_Item).range =  bac_range_number_choose;

			m_output_list.SetItemText(Changed_Item,OUTPUT_0_PERSENT,_T(""));
			m_output_list.SetCellEnabled(Changed_Item,OUTPUT_0_PERSENT,0);
			m_output_list.SetItemText(Changed_Item,OUTPUT_100_PERSENT,_T(""));
			m_output_list.SetCellEnabled(Changed_Item,OUTPUT_100_PERSENT,0);

			CString temp1;
			CStringArray temparray;
			temp1 = Digital_Units_Array[bac_range_number_choose];//11 is the sizeof the array
			SplitCStringA(temparray,temp1,_T("/"));

			if(m_Output_data.at(Changed_Item).control == 1)
			{
				if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
				{
					m_output_list.SetItemText(Changed_Item,OUTPUT_VALUE,temparray.GetAt(1));
				}
			}
			else
			{
				if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
				{
					m_output_list.SetItemText(Changed_Item,OUTPUT_VALUE,temparray.GetAt(0));
				}	
			}

		}
		else if(temp_cs.CompareNoCase(Units_Type[UNITS_TYPE_CUSTOM])==0)
		{
			bac_ranges_type = OUTPUT_RANGE_CUSTOM_DIG_TYPE;
			//dlg.DoModal();
		}	
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

	temp_task_info.Format(_T("Write Output List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
	Post_Write_Message(bac_gloab_device_id,WRITEOUTPUT_T3000,Changed_Item,Changed_Item,sizeof(Str_out_point),BacNet_hwd ,temp_task_info);

	return 0;
}


void CBacnetOutput::OnNMClickListOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	long lRow,lCol;
	m_output_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_output_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_output_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_output_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;

	if(lCol != OUTPUT_VALUE)	
		return;

	if(m_Output_data.at(lRow).digital_analog != BAC_UNITS_DIGITAL)
		return;

	if(m_Output_data.at(lRow).auto_manual == BAC_AUTO)	//If it is auto mode, disable to change the value.
		return;



	CString New_CString;
	CString temp_task_info;
	CString temp1;
	CStringArray temparray;
	if(m_Output_data.at(lRow).range > 11)
		temp1 = Digital_Units_Array[m_Output_data.at(lRow).range - 11];//11 is the sizeof the array
	else
		temp1 = Digital_Units_Array[m_Output_data.at(lRow).range];
	SplitCStringA(temparray,temp1,_T("/"));


	if(m_Output_data.at(lRow).range>=12)
	{

		if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
		{
			m_output_list.SetItemText(lRow,OUTPUT_VALUE,temparray.GetAt(0));
			m_Output_data.at(lRow).range = m_Output_data.at(lRow).range - 11;
			New_CString = temparray.GetAt(0);
			m_Output_data.at(lRow).control = 0;	
		}

	}
	else if(m_Output_data.at(lRow).range>=1)
	{

		if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
		{
			m_output_list.SetItemText(lRow,OUTPUT_VALUE,temparray.GetAt(1));
			m_Output_data.at(lRow).range = m_Output_data.at(lRow).range + 11;
			New_CString = temparray.GetAt(1);
			m_Output_data.at(lRow).control = 1;	
		}

	}
	m_output_list.Set_Edit(false);


	temp_task_info.Format(_T("Write Output List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
	Post_Write_Message(bac_gloab_device_id,WRITEOUTPUT_T3000,lRow,lRow,sizeof(Str_out_point),BacNet_hwd,temp_task_info);

	*pResult = 0;
}


void CBacnetOutput::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
	Post_Refresh_Message(bac_gloab_device_id,READOUTPUT_T3000,0,19,sizeof(Str_out_point),5);

	CDialogEx::OnTimer(nIDEvent);
}
