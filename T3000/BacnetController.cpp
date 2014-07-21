// BacnetController.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetController.h"
#include "afxdialogex.h"



#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
#include "gloab_define.h"

extern int Station_NUM;
// BacnetController dialog
Str_controller_point m_temp_controller_data[BAC_CONTROLLER_COUNT];
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
		ON_MESSAGE(MY_RESUME_DATA, ControllerMessageCallBack)
		ON_MESSAGE(WM_REFRESH_BAC_CONTROLLER_LIST,Fresh_Controller_List)	
		ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Controller_Item)
		ON_NOTIFY(NM_CLICK, IDC_LIST_BACNET_CONTROLLER, &BacnetController::OnNMClickListController)
		ON_WM_TIMER()
		ON_WM_CLOSE()
END_MESSAGE_MAP()


// BacnetController message handlers

LRESULT  BacnetController::ControllerMessageCallBack(WPARAM wParam, LPARAM lParam)
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
		//MessageBox(_T("Bacnet operation success!"));
	}
	else
	{
		memcpy_s(&m_controller_data.at(pInvoke->mRow),sizeof(Str_controller_point),&m_temp_controller_data[pInvoke->mRow],sizeof(Str_controller_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_CONTROLLER_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//AfxMessageBox(Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}
	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_controller_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_controller_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_controller_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);

	if(pInvoke)
		delete pInvoke;
	return 0;
}

BOOL BacnetController::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	Initial_List();
	PostMessage(WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL BacnetController::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{
		CRect list_rect,win_rect;
		m_controller_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_controller_dlg_hwnd,win_rect);
		m_controller_list.Set_My_WindowRect(win_rect);
		m_controller_list.Set_My_ListRect(list_rect);

		m_controller_list.Get_clicked_mouse_position();
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void BacnetController::Initial_List()
{
	m_controller_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_controller_list.SetExtendedStyle(m_controller_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_controller_list.InsertColumn(CONTROLLER_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_controller_list.InsertColumn(CONTROLLER_INPUT, _T("Input"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_INPUTVALUE, _T("Value"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_INPUTUNITS, _T("Units"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_AUTO_MANUAL, _T("A/M"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
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

	CRect list_rect,win_rect;
	m_controller_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_controller_dlg_hwnd,win_rect);
	m_controller_list.Set_My_WindowRect(win_rect);
	m_controller_list.Set_My_ListRect(list_rect);


	m_controller_list.DeleteAllItems();
	for (int i=0;i<(int)m_controller_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_controller_list.InsertItem(i,temp_item);
		//if(ListCtrlEx::ComboBox == m_controller_list.GetColumnType(CONTROLLER_AUTO_MANUAL))
		//{
		//	ListCtrlEx::CStrList strlist;
		//	strlist.push_back(_T("Auto"));
		//	strlist.push_back(_T("Manual"));
		//	m_controller_list.SetCellStringList(i, CONTROLLER_AUTO_MANUAL, strlist);
		//}
		//m_controller_list.SetCellEnabled(i,CONTROLLER_INPUTVALUE,0);
		m_controller_list.SetCellEnabled(i,CONTROLLER_INPUTUNITS,0);
		m_controller_list.SetCellEnabled(i,CONTROLLER_SETPOINTUNITS,0);


		for (int x=0;x<CONTROLLER_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_controller_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_controller_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}
	}
}
extern int pointtotext_for_controller(char *buf,Point_T3000 *point);
extern char *ispoint(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network, byte panel, int *netpresent);
LRESULT BacnetController::Fresh_Controller_List(WPARAM wParam,LPARAM lParam)
{
	CString temp_des2;
	CString temp_des3;
	CString temp_in_unit;
	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
#if 1
		if(m_controller_list.IsDataNewer((char *)&m_controller_data.at(0),sizeof(Str_controller_point) * BAC_CONTROLLER_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_controller_list.SetListData((char *)&m_controller_data.at(0),sizeof(Str_controller_point) * BAC_CONTROLLER_COUNT);
		}
		else
		{
			return 0;
		}
#endif
	}
	


	for (int i=0;i<(int)m_controller_data.size();i++)
	{

		if(isFreshOne)
		{
			i = Fresh_Item;
		}

		temp_des2.Empty();
		if(m_controller_data.at(i).input.number< BAC_INPUT_ITEM_COUNT)
		{	
			if(m_controller_data.at(i).input.number == 0 )
			{
				temp_des2.Empty();
				temp_in_unit.Empty();
			}
			else//如果 number不是0  还需要 -1 才是 data的 vector;
			{
			MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(m_controller_data.at(i).input.number -1).label, 
				(int)strlen((char *)m_Input_data.at(m_controller_data.at(i).input.number -1).label)+1, 
				temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
			temp_des2.ReleaseBuffer();	

			
			if(temp_des2.GetLength()>9)
				temp_des2.Format(_T("%d-IN%d"),m_controller_data.at(i).input.panel,m_controller_data.at(i).input.number);
			if(temp_des2.IsEmpty())
				temp_des2.Format(_T("%d-IN%d"),m_controller_data.at(i).input.panel,m_controller_data.at(i).input.number);

			//temp_des3.Format(_T("%d"),m_Input_data.at(m_controller_data.at(i).input.number - 1).value);	
			CString cstemp_value;
			float temp_float_value;
			temp_float_value = ((float)m_Input_data.at(m_controller_data.at(i).input.number - 1).value) / 1000;
			cstemp_value.Format(_T("%.1f"),temp_float_value);
			m_controller_list.SetItemText(i,CONTROLLER_INPUTVALUE,cstemp_value);


			//********************************************************************************************************
			// bind the the input units.

			if(m_Input_data.at(i).digital_analog == BAC_UNITS_ANALOG)
			{
				//m_input_list.SetItemText(i,INPUT_RANGE,Input_Analog_Units_Array[m_Input_data.at(i).range]);
				m_controller_list.SetItemText(i,CONTROLLER_INPUTUNITS,Input_List_Analog_Units[m_Input_data.at(i).range]);


				//CString temp_value;
				//temp_value.Format(_T("%d"),m_Input_data.at(i).value);
				//m_input_list.SetItemText(i,INPUT_VALUE,temp_value);

				//temp_cal.Format(_T("%d"),(m_Input_data.at(i).calibration));
				//m_input_list.SetItemText(i,INPUT_CAL,temp_cal);
			}
			//Digital 的暂时不考虑;
			//else if(m_Input_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
			//{

			//	m_input_list.SetItemText(i,INPUT_CAL,_T(""));
			//	m_input_list.SetItemText(i,INPUT_RANGE,Digital_Units_Array[m_Input_data.at(i).range]);
			//	m_input_list.SetItemText(i,INPUT_UNITE,_T(""));

			//	if((m_Input_data.at(i).range>=12)&&(m_Input_data.at(i).range<=22))
			//	{
			//		CString temp1;
			//		CStringArray temparray;
			//		temp1 = Digital_Units_Array[m_Input_data.at(i).range - 11];//11 is the sizeof the array
			//		SplitCStringA(temparray,temp1,_T("/"));
			//		if((temparray.GetSize()==2)&&(!temparray.GetAt(1).IsEmpty()))
			//		{
			//			m_input_list.SetItemText(i,INPUT_VALUE,temparray.GetAt(1));
			//		}
			//		m_input_list.SetItemText(i,INPUT_RANGE,temp1);
			//	}
			//	else if((m_Input_data.at(i).range>=1)&&(m_Input_data.at(i).range<=11))
			//	{
			//		CString temp1;
			//		CStringArray temparray;
			//		temp1 = Digital_Units_Array[m_Input_data.at(i).range];
			//		SplitCStringA(temparray,temp1,_T("/"));
			//		if((temparray.GetSize()==2)&&(!temparray.GetAt(0).IsEmpty()))
			//		{
			//			m_input_list.SetItemText(i,INPUT_VALUE,temparray.GetAt(0));
			//		}
			//		m_input_list.SetItemText(i,INPUT_RANGE,temp1);
			//	}
			//	else
			//	{
			//		m_input_list.SetItemText(i,INPUT_UNITE,Input_List_Analog_Units[0]);
			//	}
			//}
			//********************************************************************************************************

			}
		}
		m_controller_list.SetItemText(i,CONTROLLER_INPUT,temp_des2);

		float persend_data;
		persend_data = ((float)m_controller_data.at(i).value)/ 1000;
		CString temp_output_value;
		temp_output_value.Format(_T("%.1f%%"),persend_data);
		m_controller_list.SetItemText(i,CONTROLLER_OUTPUT,temp_output_value);

		//temp_des2.Format(_T("%d"),m_controller_data.at(i).input_value);
		//m_controller_list.SetItemText(i,CONTROLLER_INPUTVALUE,temp_des2);

		//if(m_controller_data.at(i).input.number < BAC_INPUT_ITEM_COUNT)
		//{
		//	if(m_Input_data.at(m_controller_data.at(i).input.number).range < INPUT_ANOLAG_UNITE_COUNT)
		//		m_controller_list.SetItemText(i,CONTROLLER_INPUTUNITS,Input_List_Analog_Units[m_Input_data.at(m_controller_data.at(i).input.number).range]);
		//	else
		//		m_controller_list.SetItemText(i,CONTROLLER_INPUTUNITS,_T(""));
		//}
		
			
		temp_des2.Empty();
		temp_des3.Empty();
		if(m_controller_data.at(i).setpoint.number< BAC_VARIABLE_ITEM_COUNT)
		{	
			if(m_controller_data.at(i).setpoint.number == 0 )//如果number是0 说明是默认初始值，无意义，置空;
				temp_des2.Empty();
			else
			{
				int num_point,num_panel,num_net,k;
				Point_T3000 point;
				point.number = m_controller_data.at(i).setpoint.number;
				point.panel = m_controller_data.at(i).setpoint.panel;
				point.point_type = m_controller_data.at(i).setpoint.point_type;
				byte point_type,var_type;
				
				int temp_network = 0;
				char buf[255];
				char q[17];
				pointtotext_for_controller(q, &point);
				strcpy(buf,ispoint(q,&num_point,&var_type, &point_type, &num_panel, &num_net, temp_network, point.panel, &k));
				
				MultiByteToWideChar( CP_ACP, 0, (char *)buf,(int)strlen((char *)buf)+1, 
					temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des2.ReleaseBuffer();	
#if 0
				MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(m_controller_data.at(i).setpoint.number - 1).label, 
					(int)strlen((char *)m_Variable_data.at(m_controller_data.at(i).setpoint.number - 1).label)+1, 
					temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des2.ReleaseBuffer();	
				if(temp_des2.GetLength()>9)//如果获取的Var长度大于9，说明获取的Label不对，也要重置;
				{
					temp_des2.Empty();
					temp_des3.Empty();
				}
				else
				{
					CString cstemp_value;
					float temp_float_value;
					temp_float_value = ((float)m_Variable_data.at(m_controller_data.at(i).setpoint.number - 1).value) / 1000;
					temp_des3.Format(_T("%.1f"),temp_float_value);
					

					//temp_des3.Format(_T("%d"),m_Variable_data.at(m_controller_data.at(i).setpoint.number - 1).value);
				}
				if(m_Variable_data.at(i).digital_analog == BAC_UNITS_ANALOG)
				{
					if(m_Variable_data.at(i).range<=sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]))
					{
						m_controller_list.SetItemText(i,CONTROLLER_SETPOINTUNITS,Variable_Analog_Units_Array[m_Variable_data.at(m_controller_data.at(i).setpoint.number - 1).range]);
						//temp_value.Format(_T("%d"),m_Variable_data.at(i).value);
						//m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_value);
					}
				}
#endif
			}


			

			
		}
		m_controller_list.SetItemText(i,CONTROLLER_SETPOINT,temp_des2);
		m_controller_list.SetItemText(i,CONTROLLER_SETVALUE,temp_des3);

	
		//if(m_controller_data.at(i).units < VARIABLE_ANALOG_UNITE_COUNT)
		//	m_controller_list.SetItemText(i,CONTROLLER_SETPOINTUNITS,Variable_Analog_Units_Array[m_controller_data.at(i).units]);
		//else
		//	m_controller_list.SetItemText(i,CONTROLLER_SETPOINTUNITS,_T(""));


		if(m_controller_data.at(i).action == 0)
		{
			m_controller_list.SetItemText(i,CONTROLLER_ACTION,_T("-"));
		}
		else if(m_controller_data.at(i).action >= 1)
		{
			m_controller_list.SetItemText(i,CONTROLLER_ACTION,_T("+"));
		}
		//else
		//{
		//	m_controller_list.SetItemText(i,CONTROLLER_SETPOINTUNITS,_T(""));
		//}

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

		if(isFreshOne)
		{
			break;
		}
	
	}
	
	return	 0;
}

extern char *ispoint(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network, byte panel, int *netpresent);




LRESULT BacnetController::Fresh_Controller_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;


	CString temp_task_info;
	CString New_CString =  m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
	CString cstemp_value;

	//先保存 原来的值，等结束的时候来比对，看是否有改变，有改变就进行写动作;
	memcpy_s(&m_temp_controller_data[Changed_Item],sizeof(Str_controller_point),&m_controller_data.at(Changed_Item),sizeof(Str_controller_point));


	if(Changed_SubItem == CONTROLLER_INPUT)
	{
		CString cs_temp = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.IsEmpty() == false)
		{


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
			//int temp1;
			tempcs = ispoint(cTemp1,&temp_number,&temp_value_type,&temp_point_type,&temp_panel,&temp_net,0,Station_NUM,&k);
			if(tempcs!=NULL)
			{
				strcpy(temp_input,tempcs);
				CString temp_des2;
				temp_des2.Empty();

				MultiByteToWideChar( CP_ACP, 0, temp_input, (int)strlen(temp_input)+1, 
					temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des2.ReleaseBuffer();		

				m_controller_list.SetItemText(Changed_Item,CONTROLLER_INPUT,temp_des2);

				if(temp_panel != Station_NUM)
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
					//temp_des3.Format(_T("%d"),m_Input_data.at(temp_number - 1).value);

					CString cstemp_value;
					float temp_float_value;
					temp_float_value = ((float)m_Input_data.at(temp_number - 1).value) / 1000;
					temp_des3.Format(_T("%.1f"),temp_float_value);


					m_controller_data.at(Changed_Item).input_value = m_Input_data.at(temp_number - 1).value;	

					m_controller_list.SetItemText(Changed_Item,CONTROLLER_INPUTVALUE,temp_des3);
				}

			}
			else
			{
				CString temp_message;
				temp_message.Format(_T("%s character not allowed in labels!"),cs_temp.GetString());
				
				MessageBox(temp_message,_T("Information"),MB_OK |MB_ICONINFORMATION);
				m_controller_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
				return 0;
			}
		}


	}

	if(Changed_SubItem == CONTROLLER_INPUTVALUE)
	{
		//int temp_value = _wtoi(New_CString);
		int temp_value = (int)(_wtof(New_CString) * 1000);

		if(m_controller_data.at(Changed_Item).input.number == 0 )
			return 0;
		if((m_controller_data.at(Changed_Item).input.number -1 )< BAC_INPUT_ITEM_COUNT)
			m_Input_data.at(m_controller_data.at(Changed_Item).input.number - 1).value = temp_value;
		
		temp_task_info.Format(_T("Write Controllers List Item%d .Changed the input value to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEINPUT_T3000,Changed_Item,Changed_Item,sizeof(Str_in_point),BacNet_hwd ,temp_task_info);
		return 0;
	}




	if(Changed_SubItem == CONTROLLER_SETVALUE)
	{
	//	int temp_value = _wtoi(New_CString);
		int temp_value = (int)(_wtof(New_CString) * 1000);
		if(m_controller_data.at(Changed_Item).setpoint.number == 0 )
			return 0;
		if((m_controller_data.at(Changed_Item).setpoint.number -1 )< BAC_VARIABLE_ITEM_COUNT)
			m_Variable_data.at(m_controller_data.at(Changed_Item).setpoint.number - 1).value = temp_value;

		temp_task_info.Format(_T("Write Controllers List Item%d .Changed the variable value to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,Changed_Item,Changed_Item,sizeof(Str_in_point),BacNet_hwd ,temp_task_info);
		return 0;
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
		memset(temp_setpoint,0,250);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );

		int temp_number=-1;
		byte temp_value_type = -1;
		byte temp_point_type=-1;
		int temp_panel = -1;
		int temp_net = -1;
		int k=0;
		//int temp1;
		//Change the lable.ex: Change the
		tempcs = ispoint(cTemp1,&temp_number,&temp_value_type,&temp_point_type,&temp_panel,&temp_net,0,bac_gloab_panel,&k);
		if(tempcs!=NULL)
		{
			memcpy_s(temp_setpoint,250,tempcs,9);
			//strcpy_s(temp_setpoint,tempcs);
			//strcpy(temp_setpoint,tempcs);
			CString temp_des2;
			temp_des2.Empty();

			MultiByteToWideChar( CP_ACP, 0, temp_setpoint, STR_VARIABLE_LABEL+1, 
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
			char tempAAAA[250];
			memset(tempAAAA,0,250);
			temp_des3.Empty();
			if(m_Variable_data.at(temp_number - 1).range  == 20 )
			{
				if((m_controller_data.at(Changed_Item).setpoint_value>=0)&&(m_controller_data.at(Changed_Item).setpoint_value<86400))
					intervaltotext( tempAAAA, m_controller_data.at(Changed_Item).setpoint_value ,0 , 0);

				MultiByteToWideChar( CP_ACP, 0, tempAAAA, strlen(tempAAAA) + 1, 
					temp_des3.GetBuffer(MAX_PATH), MAX_PATH );
				temp_des3.ReleaseBuffer();	
			}
			else
			{
				CString cstemp_value;
				float temp_float_value;
				temp_float_value = ((float)m_Variable_data.at(m_controller_data.at(Changed_Item).setpoint.number - 1).value) / 1000;
				temp_des3.Format(_T("%.1f"),temp_float_value);

			}
	
			m_controller_list.SetItemText(Changed_Item,CONTROLLER_SETVALUE,temp_des3);

		}
	}

	if(Changed_SubItem == CONTROLLER_PROPORTIONAL)
	{
		int temp_proportional=0;
		CString cs_temp = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
		if((cs_temp.GetLength()>3) || (_wtoi(cs_temp)>255) )
		{
			MessageBox(_T("Please input a effective value"),_T("Information"),MB_OK |MB_ICONINFORMATION);
			m_controller_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
		}
		else
		{
			m_controller_data.at(Changed_Item).proportional = _wtoi(cs_temp);
		}
	}
	if(Changed_SubItem == CONTROLLER_RESET)
	{
		int temp_reset=0;
		CString cs_temp = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
		if((cs_temp.GetLength()>3) || (_wtoi(cs_temp)>255) )
		{
			MessageBox(_T("Please input a effective value"),_T("Information"),MB_OK |MB_ICONINFORMATION);
			m_controller_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
		}
		else
		{
			m_controller_data.at(Changed_Item).reset = _wtoi(cs_temp);
		}
	}

	if(Changed_SubItem == CONTROLLER_RATE)
	{
		int temp_rate=0;
		CString cs_temp = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
		if((cs_temp.GetLength()>1) || (_wtoi(cs_temp)>2) )
		{
			MessageBox(_T("Please input a effective value"),_T("Information"),MB_OK |MB_ICONINFORMATION);
			m_controller_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
		}
		else
		{
			m_controller_data.at(Changed_Item).rate = _wtoi(cs_temp);
		}
	}

	if(Changed_SubItem == CONTROLLER_BIAS)
	{
		int temp_bias=0;
		CString cs_temp = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
		if((cs_temp.GetLength()>3) || (_wtoi(cs_temp)>100) )
		{
			MessageBox(_T("Please input a effective value"),_T("Information"),MB_OK |MB_ICONINFORMATION);
			m_controller_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
		}
		else
		{
			m_controller_data.at(Changed_Item).bias = _wtoi(cs_temp);
		}
	}

	cmp_ret = memcmp(&m_temp_controller_data[Changed_Item],&m_controller_data.at(Changed_Item),sizeof(Str_controller_point));
	if(cmp_ret!=0)
	{
		m_controller_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Controllers List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITECONTROLLER_T3000,Changed_Item,Changed_Item,sizeof(Str_controller_point),m_controller_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);

	}


	
	return 0;
}



void BacnetController::OnNMClickListController(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	long lRow,lCol;
	m_controller_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_controller_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_controller_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_controller_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;
	CString New_CString;
	CString temp_task_info;
	if(lCol == CONTROLLER_ACTION)	
	{
		memcpy_s(&m_temp_controller_data[lRow],sizeof(Str_controller_point),&m_controller_data.at(lRow),sizeof(Str_controller_point));
		if(m_controller_data.at(lRow).action == 0)
		{
			m_controller_data.at(lRow).action = 1;
			m_controller_list.SetItemText(lRow,lCol,_T("+"));
			New_CString.Format(_T("+"));
		}
		else
		{
			m_controller_data.at(lRow).action = 0;
			m_controller_list.SetItemText(lRow,lCol,_T("-"));
			New_CString.Format(_T("-"));
		}
	}
	else if(lCol == CONTROLLER_AUTO_MANUAL)
	{
		memcpy_s(&m_temp_controller_data[lRow],sizeof(Str_controller_point),&m_controller_data.at(lRow),sizeof(Str_controller_point));
		if(m_controller_data.at(lRow).auto_manual == 0)
		{
			m_controller_data.at(lRow).auto_manual = 1;
			m_controller_list.SetItemText(lRow,CONTROLLER_AUTO_MANUAL,_T("Manual"));
			New_CString = _T("Manual");
		}
		else
		{
			m_controller_data.at(lRow).auto_manual = 0;
			m_controller_list.SetItemText(lRow,CONTROLLER_AUTO_MANUAL,_T("Auto"));
			New_CString = _T("Auto");
		}
	}
	else
		return;

	//if(m_Output_data.at(lRow).digital_analog != BAC_UNITS_DIGITAL)
	//	return;

	//if(m_Output_data.at(lRow).auto_manual == BAC_AUTO)	//If it is auto mode, disable to change the value.
	//	return;
	



	m_controller_list.Set_Edit(false);

	int cmp_ret = memcmp(&m_temp_controller_data[lRow],&m_controller_data.at(lRow),sizeof(Str_controller_point));
	if(cmp_ret!=0)
	{
		m_controller_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Controller List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITECONTROLLER_T3000,(int8_t)lRow,(int8_t)lRow,sizeof(Str_controller_point),m_controller_dlg_hwnd,temp_task_info,lRow,lCol);

	}


	*pResult = 0;
}


void BacnetController::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(this->IsWindowVisible())
	{
	PostMessage(WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
	if(bac_select_device_online)
		Post_Refresh_Message(g_bac_instance,READCONTROLLER_T3000,0,BAC_CONTROLLER_COUNT - 1,sizeof(Str_controller_point),BAC_CONTROLLER_GROUP);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void BacnetController::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	m_controller_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}

void BacnetController::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	//KillTimer(1);
	//m_output_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	//CDialogEx::OnCancel();
}