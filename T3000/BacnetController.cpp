// BacnetController.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetController.h"
#include "afxdialogex.h"


// BacnetController dialog

IMPLEMENT_DYNAMIC(BacnetController, CDialogEx)

BacnetController::BacnetController(CWnd* pParent /*=NULL*/)
	: CDialogEx(BacnetController::IDD, pParent)
{

}

BacnetController::~BacnetController()
{
}

void BacnetController::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BACNET_CONTROLLER, m_controller_list);
}


BEGIN_MESSAGE_MAP(BacnetController, CDialogEx)
		ON_MESSAGE(WM_REFRESH_BAC_CONTROLLER_LIST,Fresh_Controller_List)	
		ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Controller_Item)
END_MESSAGE_MAP()


// BacnetController message handlers


BOOL BacnetController::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Initial_List();
	PostMessage(WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL BacnetController::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialogEx::PreTranslateMessage(pMsg);
}

void BacnetController::Initial_List()
{
	m_controller_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_controller_list.SetExtendedStyle(m_controller_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_controller_list.InsertColumn(CONTROLLER_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_controller_list.InsertColumn(CONTROLLER_INPUT, _T("Input"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_INPUTVALUE, _T("Value"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_INPUTUNITS, _T("Units"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_AUTO_MANUAL, _T("A/M"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_OUTPUT, _T("Output"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_SETPOINT, _T("Setpoint"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_SETVALUE, _T("Set Value"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_SETPOINTUNITS, _T("Units"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_ACTION, _T("Action"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_PROPORTIONAL, _T("Proportional"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_RESET, _T("Reset"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_RATE, _T("Rate"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_BIAS, _T("Bias"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_dlg_hwnd = this->m_hWnd;
	g_hwnd_now = m_controller_dlg_hwnd;

	m_controller_list.DeleteAllItems();
	for (int i=0;i<(int)m_controller_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_controller_list.InsertItem(i,temp_item);
		if(ListCtrlEx::ComboBox == m_controller_list.GetColumnType(CONTROLLER_AUTO_MANUAL))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(_T("Auto"));
			strlist.push_back(_T("Manual"));
			m_controller_list.SetCellStringList(i, CONTROLLER_AUTO_MANUAL, strlist);
		}
		m_controller_list.SetCellEnabled(i,CONTROLLER_INPUTVALUE,0);
		m_controller_list.SetCellEnabled(i,CONTROLLER_INPUTUNITS,0);
		m_controller_list.SetCellEnabled(i,CONTROLLER_SETPOINTUNITS,0);
	}
}

LRESULT BacnetController::Fresh_Controller_List(WPARAM wParam,LPARAM lParam)
{
	CString temp_des2;
	CString temp_des3;
	for (int i=0;i<(int)m_controller_data.size();i++)
	{

		temp_des2.Empty();
		if(m_controller_data.at(i).input.number< BAC_INPUT_ITEM_COUNT)
		{	
			MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(m_controller_data.at(i).input.number).label, 
				(int)strlen((char *)m_Input_data.at(m_controller_data.at(i).setpoint.number).label)+1, 
				temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
			temp_des2.ReleaseBuffer();		
		}
		m_controller_list.SetItemText(i,CONTROLLER_INPUT,temp_des2);

		temp_des2.Format(_T("%d"),m_controller_data.at(i).input_value);
		m_controller_list.SetItemText(i,CONTROLLER_INPUTVALUE,temp_des2);

		if(m_controller_data.at(i).input.number < BAC_INPUT_ITEM_COUNT)
		{
			if(m_Input_data.at(m_controller_data.at(i).input.number).range < INPUT_ANOLAG_UNITE_COUNT)
				m_controller_list.SetItemText(i,CONTROLLER_INPUTUNITS,Input_List_Analog_Units[m_Input_data.at(i).range]);
			else
				m_controller_list.SetItemText(i,CONTROLLER_INPUTUNITS,_T(""));
		}
		
			
		temp_des2.Empty();
		temp_des3.Empty();
		if(m_controller_data.at(i).setpoint.number< BAC_VARIABLE_ITEM_COUNT)
		{	
			MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(m_controller_data.at(i).setpoint.number).label, 
				(int)strlen((char *)m_Variable_data.at(m_controller_data.at(i).setpoint.number).label)+1, 
				temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
			temp_des2.ReleaseBuffer();	
			temp_des2 = temp_des2.Left(9);
	
			temp_des3.Format(_T("%d"),m_Variable_data.at(m_controller_data.at(i).setpoint.number).value);
		}
		m_controller_list.SetItemText(i,CONTROLLER_SETPOINT,temp_des2);
		m_controller_list.SetItemText(i,CONTROLLER_INPUTVALUE,temp_des3);

	
		if(m_controller_data.at(i).units < VARIABLE_ANALOG_UNITE_COUNT)
			m_controller_list.SetItemText(i,CONTROLLER_SETPOINTUNITS,Variable_Analog_Units_Array[m_controller_data.at(i).units]);
		else
			m_controller_list.SetItemText(i,CONTROLLER_SETPOINTUNITS,_T(""));


		if(m_controller_data.at(i).action == 0)
		{
			m_controller_list.SetItemText(i,CONTROLLER_SETPOINTUNITS,_T("-"));
		}
		else if(m_controller_data.at(i).action == 1)
		{
			m_controller_list.SetItemText(i,CONTROLLER_SETPOINTUNITS,_T("+"));
		}
		else
		{
			m_controller_list.SetItemText(i,CONTROLLER_SETPOINTUNITS,_T(""));
		}

		temp_des3.Format(_T("%d"),m_controller_data.at(i).proportional);
		m_controller_list.SetItemText(i,CONTROLLER_PROPORTIONAL,temp_des3);

		temp_des3.Format(_T("%d"),m_controller_data.at(i).reset);
		m_controller_list.SetItemText(i,CONTROLLER_RESET,temp_des3);


		if(m_controller_data.at(i).auto_manual==0)
		{
			m_controller_list.SetItemText(i,CONTROLLER_AUTO_MANUAL,_T("Auto"));
			m_controller_list.SetCellEnabled(i,CONTROLLER_OUTPUT,0);
		}
		else
		{
			m_controller_list.SetItemText(i,CONTROLLER_AUTO_MANUAL,_T("Manual"));
			m_controller_list.SetCellEnabled(i,CONTROLLER_OUTPUT,1);
		}


		if(m_controller_data.at(i).bias<=100)
		{
			temp_des3.Format(_T("%d"),m_controller_data.at(i).bias);
			m_controller_list.SetItemText(i,CONTROLLER_BIAS,temp_des3);
		}
		else
		{
			m_controller_list.SetItemText(i,CONTROLLER_BIAS,_T(""));
		}

		if(m_controller_data.at(i).bias<=2)
		{
			temp_des3.Format(_T("%d"),m_controller_data.at(i).rate);
			m_controller_list.SetItemText(i,CONTROLLER_RATE,temp_des3);
		}
		else
		{
			m_controller_list.SetItemText(i,CONTROLLER_RATE,_T(""));
		}
	
	}
	
	return	 0;
}

extern char *ispoint(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network, byte panel, int *netpresent);

LRESULT BacnetController::Fresh_Controller_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;


	CString temp_task_info;
	CString New_CString =  m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
	CString cstemp_value;
	if(Changed_SubItem == CONTROLLER_INPUT)
	{
		CString cs_temp = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
		char cTemp1[255];
		char temp_input[250];
		char * tempcs=NULL;
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		//Search the input label from the list.

		int temp_number=-1;
		byte temp_value_type = -1;
		byte temp_point_type=-1;
		int temp_panel = -1;
		int temp_net = -1;
		int k=0;
		int temp1;
		tempcs = ispoint(cTemp1,&temp_number,&temp_value_type,&temp_point_type,&temp_panel,&temp_net,0,bac_gloab_panel,&k);
		if(tempcs!=NULL)
		{
			strcpy(temp_input,tempcs);
			CString temp_des2;
			temp_des2.Empty();

				MultiByteToWideChar( CP_ACP, 0, temp_input, (int)strlen(temp_input)+1, 
					temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des2.ReleaseBuffer();		

			m_controller_list.SetItemText(Changed_Item,CONTROLLER_INPUT,temp_des2);

			if(temp_panel != bac_gloab_panel)
			{
				MessageBox(_T("Don't support other panel currently!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
				m_controller_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
				return 0;
			}

			m_controller_data.at(Changed_Item).input.number = temp_number;
			m_controller_data.at(Changed_Item).input.panel = temp_panel;//bac_gloab_panel;
			m_controller_data.at(Changed_Item).input.point_type = temp_point_type;//1 means input point

			CString temp_des3;
			if(temp_number < BAC_INPUT_ITEM_COUNT)
			{
			temp_des3.Format(_T("%d"),m_Input_data.at(temp_number - 1).value);
			m_controller_list.SetItemText(Changed_Item,CONTROLLER_INPUTVALUE,temp_des3);
			}

		}
		else
		{
			MessageBox(_T("Please input a effective label"),_T("Information"),MB_OK |MB_ICONINFORMATION);
			m_controller_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
			return 0;
		}

	}


	if(Changed_SubItem == CONTROLLER_AUTO_MANUAL)
	{
		CString temp_cs = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
		if(temp_cs.CompareNoCase(_T("Auto"))==0)
		{
			m_controller_list.SetCellEnabled(Changed_Item,CONTROLLER_OUTPUT,0);
			m_controller_data.at(Changed_Item).auto_manual = BAC_AUTO ;
		}
		else
		{
			m_controller_list.SetCellEnabled(Changed_Item,CONTROLLER_OUTPUT,1);
			m_controller_data.at(Changed_Item).auto_manual = BAC_MANUAL ;
		}
	}


	if(Changed_SubItem == CONTROLLER_SETPOINT)
	{
		CString cs_temp = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
		char cTemp1[255];
		char temp_setpoint[250];
		char * tempcs=NULL;
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );

		int temp_number=-1;
		byte temp_value_type = -1;
		byte temp_point_type=-1;
		int temp_panel = -1;
		int temp_net = -1;
		int k=0;
		int temp1;
		//Change the lable.ex: Change the
		tempcs = ispoint(cTemp1,&temp_number,&temp_value_type,&temp_point_type,&temp_panel,&temp_net,0,bac_gloab_panel,&k);
		if(tempcs!=NULL)
		{
			strcpy(temp_setpoint,tempcs);
			CString temp_des2;
			temp_des2.Empty();

			MultiByteToWideChar( CP_ACP, 0, temp_setpoint, (int)strlen(temp_setpoint)+1, 
				temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
			temp_des2.ReleaseBuffer();		

			m_controller_list.SetItemText(Changed_Item,CONTROLLER_SETPOINT,temp_des2);


			if(temp_panel != bac_gloab_panel)
			{
				MessageBox(_T("Don't support other panel currently!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
				m_controller_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
				return 0;
			}

			m_controller_data.at(Changed_Item).setpoint.number = temp_number;
			m_controller_data.at(Changed_Item).setpoint.panel = temp_panel;//bac_gloab_panel;
			m_controller_data.at(Changed_Item).setpoint.point_type = temp_point_type;//1 means input point

			CString temp_des3;
			if(temp_number < BAC_VARIABLE_ITEM_COUNT)
			{
				if(m_Variable_data.at(temp_number - 1).range < VARIABLE_ANALOG_UNITE_COUNT)
					temp_des3 = Variable_Analog_Units_Array[m_Variable_data.at(temp_number - 1).range];
				m_controller_list.SetItemText(Changed_Item,CONTROLLER_SETPOINTUNITS,temp_des3);
			}

		}
	}

	return 0;
}