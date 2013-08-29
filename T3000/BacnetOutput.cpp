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
	ON_MESSAGE(WM_REFRESH_BAC_OUTPUT_LIST,Fresh_Output_List)	
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Output_Item)	
	ON_MESSAGE(MY_RESUME_DATA, OutputResumeMessageCallBack)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_APPLY, &CBacnetOutput::OnBnClickedButtonOutputApply)
END_MESSAGE_MAP()


// CBacnetOutput message handlers


BOOL CBacnetOutput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_List();

	g_invoke_id = GetPrivateData(1234,READOUTPUT_T3000,0,4,sizeof(Str_out_point));
	if(g_invoke_id>=0)
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);

	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_BUTTON_OUTPUT_READ))->SetIcon(hIcon);	
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_OK);
	((CButton *)GetDlgItem(IDC_BUTTON_OUTPUT_APPLY))->SetIcon(hIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetOutput::Initial_List()
{
	m_output_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_output_list.SetExtendedStyle(m_output_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_output_list.SetExtendedStyle(m_output_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_output_list.InsertColumn(OUTPUT_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_output_list.InsertColumn(OUTPUT_FULL_LABLE, _T("Full Lable"), 140, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_AUTO_MANUAL, _T("Auto/Manual"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_VALUE, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_UNITE, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_RANGE, _T("Range"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_0_PERSENT, _T("0%"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_100_PERSENT, _T("100%"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_DECOM, _T("Status"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_list.InsertColumn(OUTPUT_LABLE, _T("Lable"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_output_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_output_dlg_hwnd;
}

void CBacnetOutput::OnBnClickedButtonOutputRead()
{
	// TODO: Add your control notification handler code here
	g_invoke_id = GetPrivateData(1234,READOUTPUT_T3000,0,4,sizeof(Str_out_point));
	if(g_invoke_id>=0)
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);
}



LRESULT  CBacnetOutput::OutputResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	if(msg_result)
	{
		SetPaneString(0,_T("Bacnet operation success!"));
		MessageBox(_T("Bacnet operation success!"));
	}
	else
	{
		SetPaneString(0,_T("Bacnet operation fail!"));
		MessageBox(_T("Bacnet operation fail!"));
	}
	if(pInvoke)
		delete pInvoke;
	return 0;
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

		temp_value.Format(_T("%d"),m_Output_data.at(i).m_del_low);
		m_output_list.SetItemText(i,OUTPUT_0_PERSENT,temp_value);

		temp_value.Format(_T("%d"),m_Output_data.at(i).s_del_high);
		m_output_list.SetItemText(i,OUTPUT_100_PERSENT,temp_value);
		

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

		//cs_temp=m_output_list.GetItemText(i,_STATUS);
		//if(cs_temp.CompareNoCase(_T("OFF"))==0)
		//{
		//	m_Output_data.at(i).auto_manual=0;
		//}
		//else
		//{
		//	m_Output_data.at(i).auto_manual=1;
		//}

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
		memcpy_s(m_Output_data.at(i).label,STR_PROGRAM_LABEL_LENGTH,cTemp1,STR_PROGRAM_LABEL_LENGTH);


	}

	g_invoke_id =WritePrivateData(1234,WRITEPROGRAM_T3000,0,(int)m_Output_data.size()-1,sizeof(Str_program_point));
	if(g_invoke_id>=0)
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);
}

LRESULT CBacnetOutput::Fresh_Output_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;


	if(Changed_SubItem==OUTPUT_AUTO_MANUAL)	//If auto man changed to mannul , the value can be edit by user
	{
		CString temp_cs = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(_T("Auto"))==0)
		{
			m_output_list.SetCellEnabled(Changed_Item,OUTPUT_VALUE,0);
		}
		else
		{
			m_output_list.SetCellEnabled(Changed_Item,OUTPUT_VALUE,1);
		}
	}

	return 0;
}
