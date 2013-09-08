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
extern void copy_data_to_ptrpanel(int Data_type);//Used for copy the structure to the ptrpanel.

//extern  Str_in_point m_Input_data[100];
//extern int m_Input_data_length;
//extern HWND m_input_dlg_hwnd;
// CBacnetInput dialog


IMPLEMENT_DYNAMIC(CBacnetInput, CDialogEx)

CBacnetInput::CBacnetInput(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetInput::IDD, pParent)
{

}


CBacnetInput::~CBacnetInput()
{
}

void CBacnetInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_input_list);
}


BEGIN_MESSAGE_MAP(CBacnetInput, CDialogEx)
	ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Input_List)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Input_Item)	
	ON_BN_CLICKED(IDOK, &CBacnetInput::OnBnClickedOk)
//	ON_BN_CLICKED(IDC_BUTTON_CHECK_INVOKE_ID, &CBacnetInput::OnBnClickedButtonCheckInvokeId)
	ON_MESSAGE(MY_RESUME_DATA, InputResumeMessageCallBack)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_APPLY, &CBacnetInput::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_INPUT_READ, &CBacnetInput::OnBnClickedButtonRead)
END_MESSAGE_MAP()

LRESULT  CBacnetInput::InputResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	CString temp_cs;
	temp_cs.Format(_T("%d"),pInvoke->Invoke_ID);
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	if(msg_result)
	{
		CString temp_ok;
		temp_ok = _T("Bacnet operation success!   Request ID:") +  temp_cs;
		SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_ok);
		#ifdef SHOW_MESSAGEBOX
		MessageBox(temp_ok);
		#endif
	}
	else
	{
		CString temp_fail;
		temp_fail = _T("Bacnet operation fail!   Request ID:") +  temp_cs;
		SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_fail);
		#ifdef SHOW_ERROR_MESSAGE
		MessageBox(temp_fail);
		#endif
	}
	if(pInvoke)
		delete pInvoke;
	return 0;
}


//int WritePrivateData()
//{
//	// TODO: Add your control notification handler code here
//
//	uint8_t apdu[480] = { 0 };
//	uint8_t test_value[480] = { 0 };
//	int private_data_len = 0;	
//	BACNET_APPLICATION_DATA_VALUE data_value = { 0 };
//	BACNET_APPLICATION_DATA_VALUE test_data_value = { 0 };
//	BACNET_PRIVATE_TRANSFER_DATA private_data = { 0 };
//	BACNET_PRIVATE_TRANSFER_DATA test_data = { 0 };
//	bool status = false;
//
//	private_data.vendorID = BACNET_VENDOR_ID;
//	private_data.serviceNumber = 1;
//
//	unsigned max_apdu = 0;
//
//	Str_user_data_header private_data_chunk;
//	private_data_chunk.total_length=6+((int)m_Input_data.size())*sizeof(Str_in_point);
//	private_data_chunk.command = WRITEINPUT_T3000;
//	private_data_chunk.point_start_instance=0;
//	private_data_chunk.point_end_instance=((int)m_Input_data.size())-1;
//	private_data_chunk.entitysize=sizeof(Str_in_point);
//
//	char SendBuffer[500];
//	memcpy_s(SendBuffer,6,&private_data_chunk,6);
//
//	for (int i=0;i<(int)m_Input_data.size();i++)
//	{
//		memcpy_s(SendBuffer + i*sizeof(Str_in_point) +6,sizeof(Str_in_point),&m_Input_data[i],sizeof(Str_in_point));
//	}
//
//	Set_transfer_length(private_data_chunk.total_length);
//	//transfer_len=6;
//
//	status =bacapp_parse_application_data(BACNET_APPLICATION_TAG_OCTET_STRING,(char *)&SendBuffer, &data_value);
//	//ct_test(pTest, status == true);
//	private_data_len =	bacapp_encode_application_data(&test_value[0], &data_value);
//	private_data.serviceParameters = &test_value[0];
//	private_data.serviceParametersLen = private_data_len;
//
//	BACNET_ADDRESS dest = { 0 };
//	status = address_get_by_device(1234, &max_apdu, &dest);
//	if (status) 
//	{
//		return Send_ConfirmedPrivateTransfer(&dest,&private_data);
//	}
//	return -2;
//}


// CBacnetInput message handlers


BOOL CBacnetInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_List();
	PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
	//g_invoke_id = GetPrivateData(1234,READINPUT_T3000,0,4,sizeof(Str_in_point));
	//if(g_invoke_id>=0)
	//	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);

	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_BUTTON_INPUT_READ))->SetIcon(hIcon);	
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_OK);
	((CButton *)GetDlgItem(IDC_BUTTON_INPUT_APPLY))->SetIcon(hIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetInput::Initial_List()
{
	m_input_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_input_list.InsertColumn(INPUT_NUM, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_input_list.InsertColumn(INPUT_FULL_LABLE, _T("Full Label"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_AUTO_MANUAL, _T("Auto/Manual"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_VALUE, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_UNITE, _T("Units"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_RANGE, _T("Range"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_CAL, _T("Calibration"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_FITLER, _T("Filter"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_DECOM, _T("Status"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_list.InsertColumn(INPUT_LABLE, _T("Label"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_input_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_input_dlg_hwnd;
}

void CBacnetInput::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
}
LRESULT CBacnetInput::Fresh_Input_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	if(Changed_SubItem == INPUT_LABLE)
	{
		CString cs_temp = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Input_data.at(Changed_Item).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);
	}

	if(Changed_SubItem == INPUT_FULL_LABLE)
	{
		CString cs_temp = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
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
			m_input_list.SetCellEnabled(Changed_Item,INPUT_VALUE,0);
			m_Input_data.at(Changed_Item).auto_manual = BAC_AUTO ;
		}
		else
		{
			m_input_list.SetCellEnabled(Changed_Item,INPUT_VALUE,1);
			m_Input_data.at(Changed_Item).auto_manual = BAC_MANUAL ;
		}
	}
	else if(Changed_SubItem==INPUT_RANGE)
	{
		CString temp_cs = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(_T("Not Used"))==0)
		{
			m_input_list.SetItemText(Changed_Item,INPUT_UNITE,Input_Unit[_unused]);
		}
		else if(temp_cs.CompareNoCase(_T("10K(-40->120)"))==0)
		{
			ListCtrlEx::CStrList strlist;
			strlist.clear();
			strlist.push_back(Input_Unit[degC]);
			//strlist.push_back(Input_Unit[degF]);
			m_input_list.SetCellStringList(Changed_Item, INPUT_UNITE, strlist);
			m_input_list.SetItemText(Changed_Item,INPUT_UNITE,Input_Unit[degC]);
		}
		else if(temp_cs.CompareNoCase(_T("I 4->20ma"))==0)
		{
			ListCtrlEx::CStrList strlist;
			strlist.clear();
			TRACE(Input_Unit[ma]);
			strlist.push_back(Input_Unit[ma]);
			m_input_list.SetCellStringList(Changed_Item, INPUT_UNITE, strlist);
			m_input_list.SetItemText(Changed_Item,INPUT_UNITE,Input_Unit[ma]);
		}
		else if(temp_cs.CompareNoCase(_T("V 0->10V"))==0)
		{
			ListCtrlEx::CStrList strlist;
			strlist.clear();
			TRACE(Input_Unit[Volts]);
			strlist.push_back(Input_Unit[Volts]);
			m_input_list.SetCellStringList(Changed_Item, INPUT_UNITE, strlist);
			m_input_list.SetItemText(Changed_Item,INPUT_UNITE,Input_Unit[Volts]);
		}
		else if(temp_cs.CompareNoCase(_T("V 0->5V"))==0)
		{
			ListCtrlEx::CStrList strlist;
			strlist.clear();
			TRACE(Input_Unit[Volts]);
			strlist.push_back(Input_Unit[Volts]);
			m_input_list.SetCellStringList(Changed_Item, INPUT_UNITE, strlist);
			m_input_list.SetItemText(Changed_Item,INPUT_UNITE,Input_Unit[Volts]);
		}
		else if(temp_cs.CompareNoCase(_T("V 0->24AC"))==0)
		{
			ListCtrlEx::CStrList strlist;
			strlist.clear();
			strlist.push_back(_T("ON/OFF"));
			m_input_list.SetCellStringList(Changed_Item, INPUT_UNITE, strlist);
			m_input_list.SetItemText(Changed_Item,INPUT_UNITE,_T("ON/OFF"));
		}
		else if(temp_cs.CompareNoCase(_T("TST Normal"))==0)
		{
			ListCtrlEx::CStrList strlist;
			strlist.clear();
			strlist.push_back(_T("ON/OFF"));
			m_input_list.SetCellStringList(Changed_Item, INPUT_UNITE, strlist);
			m_input_list.SetItemText(Changed_Item,INPUT_UNITE,_T("ON/OFF"));
		}
	}

	if(Changed_SubItem==INPUT_CAL)
	{
		CString cs_temp=m_input_list.GetItemText(Changed_Item,INPUT_CAL);
		int cal_value = _wtoi(cs_temp);
		m_Input_data.at(Changed_Item).calibration = cal_value;
	}
	else if(Changed_SubItem==INPUT_FITLER)
	{
		CString cs_temp=m_input_list.GetItemText(Changed_Item,INPUT_FITLER);
		int  temp2 = _wtoi(cs_temp);
		m_Input_data.at(Changed_Item).filter = log((double)temp2)/log((double)2);
	}
	else if(Changed_SubItem==INPUT_DECOM)
	{
		CString cs_temp = m_input_list.GetItemText(Changed_Item,INPUT_DECOM);
		int dec_index=-1;
		for (int m=0;m<(int)sizeof(Decom_Array)/sizeof(Decom_Array[0]);m++)
		{
			if(cs_temp.CompareNoCase(Decom_Array[m])==0)
			{
				dec_index = m;
				break;
			}
		}
		m_Input_data.at(Changed_Item).decom = dec_index;
	}


	Post_Write_Message(1234,WRITEINPUT_T3000,Changed_Item,Changed_Item,sizeof(Str_in_point),this->m_hWnd);
	//m_input_list.Invalidate();
	return 0;
}

LRESULT CBacnetInput::Fresh_Input_List(WPARAM wParam,LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);

	m_input_list.DeleteAllItems();
	for (int i=0;i<(int)m_Input_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_input_list.InsertItem(i,temp_item);
		if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_AUTO_MANUAL))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(_T("Auto"));
			//CString str; 
			//str.Format(_T("some other %d, %d"), nRow, j); 
			strlist.push_back(_T("Manual"));
			m_input_list.SetCellStringList(i, INPUT_AUTO_MANUAL, strlist);
		}

		if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_RANGE))
		{
			ListCtrlEx::CStrList strlist;
			int test = sizeof(Input_Range_Array)/sizeof(Input_Range_Array[0]);
			for (int loop=0;loop<(int)sizeof(Input_Range_Array)/sizeof(Input_Range_Array[0]);loop++)
			{
				strlist.push_back(Input_Range_Array[loop]) ;
			}
			m_input_list.SetCellStringList(i, INPUT_RANGE, strlist);
		}

		if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_FITLER))
		{
			ListCtrlEx::CStrList strlist;
			for (int m=0;m<(int)(sizeof(Input_Filter_Array)/sizeof(Input_Filter_Array[0]));m++)
			{
				strlist.push_back(Input_Filter_Array[m]);
			}
			m_input_list.SetCellStringList(i, INPUT_FITLER, strlist);
		}

		if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_DECOM))
		{
			ListCtrlEx::CStrList strlist;
			for (int m=0;m<(int)(sizeof(Decom_Array)/sizeof(Decom_Array[0]));m++)
			{
				strlist.push_back(Decom_Array[m]);
			}
			m_input_list.SetCellStringList(i, INPUT_DECOM, strlist);
		}


		MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(i).description, (int)strlen((char *)m_Input_data.at(i).description)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_input_list.SetItemText(i,INPUT_FULL_LABLE,temp_des);
		if(m_Input_data.at(i).auto_manual==0)
		{
			m_input_list.SetItemText(i,INPUT_AUTO_MANUAL,_T("Auto"));
			m_input_list.SetCellEnabled(i,INPUT_VALUE,0);
		}
		else
		{
			m_input_list.SetItemText(i,INPUT_AUTO_MANUAL,_T("Manual"));
			m_input_list.SetCellEnabled(i,INPUT_VALUE,1);
		}

		switch(m_Input_data.at(i).range)
		{
		case 0:
			temp_units = _T("Not Used");
			break;
		case 1:
			temp_units = _T("℃");
			if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_UNITE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(_T("℃"));
				strlist.push_back(_T("℉"));
				m_input_list.SetCellStringList(i, INPUT_UNITE, strlist);
			}
			break;
		case 2:
			if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_UNITE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Input_Unit[ma]);
				m_input_list.SetCellStringList(i, INPUT_UNITE, strlist);
			}
			temp_units = _T("mA");
			break;
		case 3:
		case 4:
			if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_UNITE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(Input_Unit[Volts]);
				m_input_list.SetCellStringList(i, INPUT_UNITE, strlist);
			}
			temp_units = _T("Volt");
			break;
		case 5:
		case 6:
			if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(INPUT_UNITE))
			{
				ListCtrlEx::CStrList strlist;
				strlist.push_back(_T("ON/OFF"));
				m_input_list.SetCellStringList(i, INPUT_UNITE, strlist);
			}
			temp_units = _T("ON/OFF");
			break;
		default:
			break;
		}
		m_input_list.SetItemText(i,INPUT_UNITE,temp_units);

		temp_value.Format(_T("%d"),m_Input_data.at(i).value);
		m_input_list.SetItemText(i,INPUT_VALUE,temp_value);

		CString temp_show_rang;
		if((m_Input_data.at(i).range>=7)||(m_Input_data.at(i).range<0))
			temp_show_rang = _T("");
		else
			temp_show_rang = Input_Range_Array[m_Input_data.at(i).range];

		m_input_list.SetItemText(i,INPUT_RANGE,temp_show_rang);

		temp_cal.Format(_T("%d"),(m_Input_data.at(i).calibration));
		m_input_list.SetItemText(i,INPUT_CAL,temp_cal);

		temp_filter.Format(_T("%d"),(int)pow((double)2,(int)m_Input_data.at(i).filter));
		m_input_list.SetItemText(i,INPUT_FITLER,temp_filter);

		if(m_Input_data.at(i).decom==0)
			temp_status.Format(Decom_Array[0]);
		else if(m_Input_data.at(i).decom==1)
			temp_status.Format(Decom_Array[1]);
		else
			temp_status.Empty();
		m_input_list.SetItemText(i,INPUT_DECOM,temp_status);


		CString temp_des2;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(i).label, (int)strlen((char *)m_Input_data.at(i).label)+1, 
			temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des2.ReleaseBuffer();
		m_input_list.SetItemText(i,INPUT_LABLE,temp_des2);
		//ptr_panel
	}
	copy_data_to_ptrpanel(TYPE_INPUT);
	//MessageBox("1");
	return 0;
}

//void CBacnetInput::OnBnClickedButtonCheckInvokeId()
//{
//	// TODO: Add your control notification handler code here
//	if(tsm_invoke_id_free(g_invoke_id))
//		MessageBox(_T("Success!"));
//	else
//		MessageBox(_T("Fail!"));
//}


void CBacnetInput::OnBnClickedButtonApply()
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<(int)m_Input_data.size();i++)
	{
		CString cs_temp=m_input_list.GetItemText(i,INPUT_FULL_LABLE);
		char cTemp[255];
		memset(cTemp,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp, 255, NULL, NULL );
		memcpy_s(m_Input_data.at(i).description,21,cTemp,21);

		cs_temp=m_input_list.GetItemText(i,INPUT_AUTO_MANUAL);
		if(cs_temp.CompareNoCase(_T("Auto"))==0)
		{
			m_Input_data.at(i).auto_manual=0;
		}
		else
		{
			m_Input_data.at(i).auto_manual=1;
		}

		cs_temp=m_input_list.GetItemText(i,INPUT_RANGE);
		int index_number=0;
		for(int j=0;j<(int)sizeof(Input_Range_Array)/sizeof(Input_Range_Array[0]);j++)
		{
			if(cs_temp.CompareNoCase(Input_Range_Array[j])==0)
			{
				index_number=j;
				break;
			}
		}
		m_Input_data.at(i).range = index_number;

		cs_temp=m_input_list.GetItemText(i,INPUT_UNITE);
		index_number=0;
		for(int j=0;j<(int)sizeof(Input_Unit)/sizeof(Input_Unit[0]);j++)//unit单位很多，在列表中查找
		{
			if(cs_temp.CompareNoCase(Input_Unit[j])==0)
			{
				index_number=j;
				break;
			}
		}
		//m_Input_data[i].


		cs_temp = m_input_list.GetItemText(i,INPUT_VALUE);
		m_Input_data.at(i).value = _wtoi(cs_temp);


		cs_temp=m_input_list.GetItemText(i,INPUT_CAL);
		int cal_value = _wtoi(cs_temp);
		m_Input_data.at(i).calibration = cal_value;


		cs_temp=m_input_list.GetItemText(i,INPUT_FITLER);
		int  temp2 = _wtoi(cs_temp);
		m_Input_data.at(i).filter = log((double)temp2)/log((double)2);


		cs_temp = m_input_list.GetItemText(i,INPUT_DECOM);
		int dec_index=-1;
		for (int m=0;m<(int)sizeof(Decom_Array)/sizeof(Decom_Array[0]);m++)
		{
			if(cs_temp.CompareNoCase(Decom_Array[m])==0)
			{
				dec_index = m;
				break;
			}
		}
		m_Input_data.at(i).decom = dec_index;

		cs_temp=m_input_list.GetItemText(i,INPUT_LABLE);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Input_data.at(i).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);
	}
	Post_Write_Message(1234,WRITEINPUT_T3000,0,19,sizeof(Str_in_point),this->m_hWnd);
	//g_invoke_id =WritePrivateData(1234,WRITEINPUT_T3000,0,(int)m_Input_data.size()-1,sizeof(Str_in_point));
	//if(g_invoke_id>=0)
	//	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);
}


void CBacnetInput::OnBnClickedButtonRead()
{
	// TODO: Add your control notification handler code here
	//g_invoke_id = GetPrivateData(1234,READINPUT_T3000,0,4,sizeof(Str_in_point));
	//if(g_invoke_id>=0)
	//	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);
	PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
}

//
//int GetPrivateData()
//{
//	// TODO: Add your control notification handler code here
//
//	uint8_t apdu[480] = { 0 };
//	uint8_t test_value[480] = { 0 };
//	int apdu_len = 0;
//	int private_data_len = 0;	
//	unsigned max_apdu = 0;
//	BACNET_APPLICATION_DATA_VALUE data_value = { 0 };
//	//	BACNET_APPLICATION_DATA_VALUE test_data_value = { 0 };
//	BACNET_PRIVATE_TRANSFER_DATA private_data = { 0 };
//	//	BACNET_PRIVATE_TRANSFER_DATA test_data = { 0 };
//	bool status = false;
//
//	private_data.vendorID = BACNET_VENDOR_ID;
//	private_data.serviceNumber = 1;
//
//
//
//	Str_user_data_header private_data_chunk;
//	private_data_chunk.total_length=6;
//	private_data_chunk.command = READINPUT_T3000;
//	private_data_chunk.point_start_instance=0;
//	private_data_chunk.point_end_instance=4;
//	private_data_chunk.entitysize=sizeof(Str_in_point);
//	// char private_data_chunk[33] = { "3031323334353637383940" };
//	Set_transfer_length(6);
//	//transfer_len=6;
//
//	status =bacapp_parse_application_data(BACNET_APPLICATION_TAG_OCTET_STRING,(char *)&private_data_chunk, &data_value);
//	//ct_test(pTest, status == true);
//	private_data_len =	bacapp_encode_application_data(&test_value[0], &data_value);
//	private_data.serviceParameters = &test_value[0];
//	private_data.serviceParametersLen = private_data_len;
//
//	BACNET_ADDRESS dest = { 0 };
//	status = address_get_by_device(1234, &max_apdu, &dest);
//	if (status) 
//	{
//		return Send_ConfirmedPrivateTransfer(&dest,&private_data);
//		//return g_invoke_id;
//	}
//	else
//		return -1;
//
//}






