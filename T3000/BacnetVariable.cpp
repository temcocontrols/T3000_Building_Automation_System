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
	ON_MESSAGE(MY_RESUME_DATA, VariableResumeMessageCallBack)
	ON_MESSAGE(WM_REFRESH_BAC_VARIABLE_LIST,Fresh_Variable_List)
	ON_BN_CLICKED(IDC_BUTTON_VARIABLE_APPLY, &CBacnetVariable::OnBnClickedButtonVariableApply)
END_MESSAGE_MAP()


// CBacnetVariable message handlers


BOOL CBacnetVariable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Initial_List();	//Initial the list of Variable,read from device;

	g_invoke_id = GetPrivateData(1234,READVARIABLE_T3000,0,4,sizeof(Str_variable_point));
	if(g_invoke_id>=0)
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);

	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_BUTTON_VARIABLE_READ))->SetIcon(hIcon);	
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_OK);
	((CButton *)GetDlgItem(IDC_BUTTON_VARIABLE_APPLY))->SetIcon(hIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetVariable::OnBnClickedButtonVariableRead()
{
	// TODO: Add your control notification handler code here
}

LRESULT  CBacnetVariable::VariableResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	if(msg_result)
	{
		SetPaneString(0,_T("Bacnet operation success!"));
		//MessageBox(_T("Bacnet operation success!"));
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


void CBacnetVariable::Initial_List()
{
	m_variable_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_variable_list.SetExtendedStyle(m_variable_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_variable_list.InsertColumn(VARIABLE_NUM, _T("NUM"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_variable_list.InsertColumn(VARIABLE_FULL_LABLE, _T("Full Lable"), 200, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_AUTO_MANUAL, _T("Auto/Manual"), 150, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_VALUE, _T("Value"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_UNITE, _T("Units"), 120, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_list.InsertColumn(VARIABLE_LABLE, _T("Lable"), 150, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_variable_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_variable_dlg_hwnd;
}


LRESULT CBacnetVariable::Fresh_Variable_List(WPARAM wParam,LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);

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
			//CString str; 
			//str.Format(_T("some other %d, %d"), nRow, j); 
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



		temp_value.Format(_T("%d"),m_Variable_data.at(i).value);
		m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_value);


	
	//	m_variable_list.SetItemText(i,VARIABLE_UNITE,temp_units);	//单位 这个要商量 看要怎么搞





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


	if(Changed_SubItem == VARIABLE_AUTO_MANUAL)
	{
		CString temp_cs = m_variable_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(_T("Auto"))==0)
		{
			m_variable_list.SetCellEnabled(Changed_Item,VARIABLE_VALUE,0);
		}
		else
		{
			m_variable_list.SetCellEnabled(Changed_Item,VARIABLE_VALUE,1);
		}
	}
	if(Changed_SubItem == VARIABLE_UNITE)
	{
		CString temp_cs = m_variable_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(Units_Type[0])==0)
		{
			bac_ranges_type = VARIABLE_RANGE_ANALOG_TYPE;
		}
		else if(temp_cs.CompareNoCase(Units_Type[1])==0)
		{
			bac_ranges_type = VARIABLE_RANGE_DIGITAL_TYPE;
		}
		else if(temp_cs.CompareNoCase(Units_Type[2])==0)
		{
			bac_ranges_type = VARIABLE_RANGE_CUSTOM_DIG_TYPE;
		}

		BacnetRange dlg;
		dlg.DoModal();
	}
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
			m_Variable_data.at(i).auto_manual=0;
			m_variable_list.SetCellEnabled(i,VARIABLE_VALUE,0);
		}
		else
		{
			m_Variable_data.at(i).auto_manual=1;
			m_variable_list.SetCellEnabled(i,VARIABLE_VALUE,1);
		}


		/*
		int index_number=0;

		cs_temp=m_variable_list.GetItemText(i,VARIABLE_UNITE);
		index_number=0;
		for(int j=0;j<(int)sizeof(Input_Unit)/sizeof(Input_Unit[0]);j++)//unit单位很多，在列表中查找
		{
			if(cs_temp.CompareNoCase(Input_Unit[j])==0)
			{
				index_number=j;
				break;
			}
		}
		*/
		//m_Variable_data[i].


		cs_temp = m_variable_list.GetItemText(i,VARIABLE_VALUE);
		m_Variable_data.at(i).value = _wtoi(cs_temp);



		cs_temp=m_variable_list.GetItemText(i,VARIABLE_LABLE);
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_Variable_data.at(i).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);
	}

	g_invoke_id =WritePrivateData(1234,WRITEVARIABLE_T3000,0,(int)m_Variable_data.size()-1,sizeof(Str_variable_point));
	if(g_invoke_id>=0)
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);
}
