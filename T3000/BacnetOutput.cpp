// BacnetOutput.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetOutput.h"
#include "afxdialogex.h"

#include "afxdialogex.h"
#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
// CBacnetOutput dialog

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
END_MESSAGE_MAP()


// CBacnetOutput message handlers


BOOL CBacnetOutput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_List();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetOutput::Initial_List()
{
	m_output_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_output_list.SetExtendedStyle(m_output_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);

	m_output_list.InsertColumn(OUTPUT_NUM, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_output_list.InsertColumn(OUTPUT_FULL_LABLE, _T("Full Lable"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_AUTO_MANUAL, _T("Auto/Manual"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_VALUE, _T("Value"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_UNITE, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_RANGE, _T("Range"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_0_PERSENT, _T("0%"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_100_PERSENT, _T("100%"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_DECOM, _T("Status"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_LABLE, _T("Lable"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_input_dlg_hwnd;
}

void CBacnetOutput::OnBnClickedButtonOutputRead()
{
	// TODO: Add your control notification handler code here
	g_invoke_id = GetPrivateData(1234,READOUTPUT_T3000,0,4,sizeof(Str_out_point));
	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);
}


LRESULT CBacnetOutput::Fresh_Output_List(WPARAM wParam,LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);

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

		//if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_RANGE))
		//{
		//	ListCtrlEx::CStrList strlist;
		//	int test = sizeof(Input_Range_Array)/sizeof(Input_Range_Array[0]);
		//	for (int loop=0;loop<(int)sizeof(Input_Range_Array)/sizeof(Input_Range_Array[0]);loop++)
		//	{
		//		strlist.push_back(Input_Range_Array[loop]) ;
		//	}
		//	m_output_list.SetCellStringList(i, OUTPUT_RANGE, strlist);
		//}



		if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(OUTPUT_DECOM))
		{
			ListCtrlEx::CStrList strlist;
			for (int m=0;m<(int)(sizeof(Output_Decom_Array)/sizeof(Output_Decom_Array[0]));m++)
			{
				strlist.push_back(Output_Decom_Array[m]);
			}
			m_output_list.SetCellStringList(i, OUTPUT_DECOM, strlist);
		}


		MultiByteToWideChar( CP_ACP, 0, (char *)m_Output_data.at(i).description, (int)strlen((char *)m_Output_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_output_list.SetItemText(i,OUTPUT_FULL_LABLE,temp_des);
		if(m_Output_data.at(i).auto_manual==0)
			m_output_list.SetItemText(i,OUTPUT_AUTO_MANUAL,_T("Auto"));
		else
			m_output_list.SetItemText(i,OUTPUT_AUTO_MANUAL,_T("Manual"));

		//switch(m_Output_data.at(i).range)
		//{
		//case 0:
		//	temp_units = _T("Not Used");
		//	break;
		//case 1:
		//	temp_units = _T("¡æ");
		//	if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(INPUT_UNITE))
		//	{
		//		ListCtrlEx::CStrList strlist;
		//		strlist.push_back(_T("¡æ"));
		//		strlist.push_back(_T("¨H"));
		//		m_output_list.SetCellStringList(i, INPUT_UNITE, strlist);
		//	}
		//	break;
		//case 2:
		//	if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(INPUT_UNITE))
		//	{
		//		ListCtrlEx::CStrList strlist;
		//		strlist.push_back(Input_Unit[ma]);
		//		m_output_list.SetCellStringList(i, INPUT_UNITE, strlist);
		//	}
		//	temp_units = _T("mA");
		//	break;
		//case 3:
		//case 4:
		//	if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(INPUT_UNITE))
		//	{
		//		ListCtrlEx::CStrList strlist;
		//		strlist.push_back(Input_Unit[Volts]);
		//		m_output_list.SetCellStringList(i, INPUT_UNITE, strlist);
		//	}
		//	temp_units = _T("Volt");
		//	break;
		//case 5:
		//case 6:
		//	if(ListCtrlEx::ComboBox == m_output_list.GetColumnType(INPUT_UNITE))
		//	{
		//		ListCtrlEx::CStrList strlist;
		//		strlist.push_back(_T("ON/OFF"));
		//		m_output_list.SetCellStringList(i, INPUT_UNITE, strlist);
		//	}
		//	temp_units = _T("ON/OFF");
		//	break;
		//default:
		//	break;
		//}
		//m_output_list.SetItemText(i,INPUT_UNITE,temp_units);

		temp_value.Format(_T("%d"),m_Output_data.at(i).value);
		m_output_list.SetItemText(i,OUTPUT_VALUE,temp_value);

		//CString temp_show_rang;
		//if((m_Output_data.at(i).range>=7)||(m_Output_data.at(i).range<0))
		//	temp_show_rang = _T("");
		//else
		//	temp_show_rang = Input_Range_Array[m_Output_data.at(i).range];

		//m_output_list.SetItemText(i,OUTPUT_RANGE,temp_show_rang);

		//temp_cal.Format(_T("%d"),(m_Output_data.at(i).calibration));
		//m_output_list.SetItemText(i,INPUT_CAL,temp_cal);

		//temp_filter.Format(_T("%d"),(int)pow((double)2,(int)m_Output_data.at(i).filter));
		//m_output_list.SetItemText(i,INPUT_FITLER,temp_filter);

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

	//MessageBox("1");
	return 0;
}
