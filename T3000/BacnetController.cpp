// BacnetController.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetController.h"
#include "afxdialogex.h"



#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "global_function.h"
#include "global_define.h"

//extern int Station_NUM;
// BacnetController dialog
int PID_CONTROLLER_LIMITE_ITEM_COUNT = 0;


extern int pointtotext_for_controller(char *buf,Point_T3000 *point);
extern char *ispoint(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network, byte panel, int *netpresent);

IMPLEMENT_DYNAMIC(BacnetController, CDialogEx)

BacnetController::BacnetController(CWnd* pParent /*=NULL*/)
	: CDialogEx(BacnetController::IDD, pParent)
{
	window_max = true;
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
		ON_WM_HELPINFO()
		ON_WM_SIZE()
		ON_WM_SYSCOMMAND()
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
		if((pInvoke->mRow < BAC_PID_COUNT) && (pInvoke->mRow >= 0))
		{
			Post_Refresh_One_Message(g_bac_instance,READCONTROLLER_T3000,
				pInvoke->mRow,pInvoke->mRow,sizeof(Str_controller_point));
			TRACE(_T("Controller receive \r\n"));
			SetTimer(2,2000,NULL);
		}
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
	SetWindowTextW(_T("PID"));
	Initial_List();
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_PID);
	SetIcon(m_hIcon,TRUE);
	PostMessage(WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
	SetTimer(1,CONTROLLOR_REFRESH_TIME,NULL);
	
	ShowWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL BacnetController::PreTranslateMessage(MSG* pMsg)
{
	
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
	else if(pMsg->message==WM_NCLBUTTONDBLCLK)
	{
		if(!window_max)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 90 ,temp_mynew_rect.top + 70,500,700,SWP_SHOWWINDOW);
		}


		return 1; 
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void BacnetController::Initial_List()
{
	m_controller_list.ShowWindow(SW_HIDE);
	m_controller_list.DeleteAllItems();
	while ( m_controller_list.DeleteColumn (0)) ;

	m_controller_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_controller_list.SetExtendedStyle(m_controller_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_controller_list.InsertColumn(CONTROLLER_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_controller_list.InsertColumn(CONTROLLER_INPUT, _T("Input"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_INPUTVALUE, _T("Value"), 60, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_INPUTUNITS, _T("Units"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_AUTO_MANUAL, _T("A/M"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_OUTPUT, _T("Output"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_SETPOINT, _T("Setpoint"), 60, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_SETVALUE, _T("Set Value"), 60, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_SETPOINTUNITS, _T("Units"), 70, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_ACTION, _T("Action"), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_PROPORTIONAL, _T("Prop"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_RESET, _T("Int"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_I_TIME, _T("Time"), 50, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_RATE, _T("Der"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_controller_list.InsertColumn(CONTROLLER_BIAS, _T("Bias"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	
	
	m_controller_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_controller_dlg_hwnd;
	m_controller_list.SetListHwnd(this->m_hWnd);

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

		if(i>=controller_item_limit_count)
			break;

		temp_item.Format(_T("%d"),i+1);
		m_controller_list.InsertItem(i,temp_item);
		m_controller_list.SetCellEnabled(i,CONTROLLER_INPUTUNITS,0);
		m_controller_list.SetCellEnabled(i,CONTROLLER_SETPOINTUNITS,0);

		if(ListCtrlEx::ComboBox == m_controller_list.GetColumnType(CONTROLLER_I_TIME))
		{
			ListCtrlEx::CStrList strlist;

			for (int i=0;i<(int)sizeof(PID_Time_Type)/sizeof(PID_Time_Type[0]);i++)
			{
				strlist.push_back(PID_Time_Type[i]);
			}
			m_controller_list.SetCellStringList(i, CONTROLLER_I_TIME, strlist);		
		}


		for (int x=0;x<CONTROLLER_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_controller_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_controller_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}
	}
	m_controller_list.InitListData();
		m_controller_list.ShowWindow(SW_SHOW);
}

LRESULT BacnetController::Fresh_Controller_List(WPARAM wParam,LPARAM lParam)
{
	CString temp_des2;
	CString temp_des3;
	CString temp_in_unit;
	CString temp_set_unit;
	int Fresh_Item;
	int isFreshOne = (int)lParam;

	if(bacnet_device_type == STM32_HUM_NET)
	{
		PID_CONTROLLER_LIMITE_ITEM_COUNT = 3;
	}
	else
		PID_CONTROLLER_LIMITE_ITEM_COUNT = BAC_PID_COUNT;


	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
#if 1
		if(m_controller_list.IsDataNewer((char *)&m_controller_data.at(0),sizeof(Str_controller_point) * BAC_PID_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_controller_list.SetListData((char *)&m_controller_data.at(0),sizeof(Str_controller_point) * BAC_PID_COUNT);
		}
		else
		{
			//return 0;
		}
#endif
	}
	


	for (int i=0;i<(int)m_controller_data.size();i++)
	{
		if(i>=controller_item_limit_count)
			break;

		if(isFreshOne)
		{
			i = Fresh_Item;
		}


		if(i>= PID_CONTROLLER_LIMITE_ITEM_COUNT)
		{
			for (int a=0;a<CONTROLLER_COL_NUMBER; a++)
			{
				m_controller_list.SetItemText(i,a,_T(""));
			}
			continue;
		}


		temp_des2.Empty();

		if( (BAC_IN + 1) == m_controller_data.at(i).input.point_type)
		{
			if(m_controller_data.at(i).input.number< BAC_INPUT_ITEM_COUNT)
			{	
					MultiByteToWideChar( CP_ACP, 0, (char *)m_Input_data.at(m_controller_data.at(i).input.number ).label, 
						(int)strlen((char *)m_Input_data.at(m_controller_data.at(i).input.number ).label)+1, 
						temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
					temp_des2.ReleaseBuffer();	

					//如果是小叶的设备,因为没有input 就直接显示in2之类.
					if(bacnet_device_type == STM32_HUM_NET)
					{
						temp_des2.Empty();
					}


					if(temp_des2.GetLength()>9)
						temp_des2.Format(_T("%d-IN%d"),m_controller_data.at(i).input.panel,m_controller_data.at(i).input.number + 1);
					if(temp_des2.IsEmpty())
						temp_des2.Format(_T("%d-IN%d"),m_controller_data.at(i).input.panel,m_controller_data.at(i).input.number + 1);

					//temp_des3.Format(_T("%d"),m_Input_data.at(m_controller_data.at(i).input.number - 1).value);	
					CString cstemp_value;
					float temp_float_value;
					temp_float_value = ((float)m_controller_data.at(i).input_value) / 1000;

					cstemp_value.Format(_T("%.1f"),temp_float_value);
					m_controller_list.SetItemText(i,CONTROLLER_INPUTVALUE,cstemp_value);


					//********************************************************************************************************
					// bind the the input units.
					int x = m_controller_data.at(i).input.number;
					if(m_Input_data.at(x).digital_analog == BAC_UNITS_ANALOG)
					{
						m_controller_list.SetItemText(i,CONTROLLER_INPUTUNITS,Input_List_Analog_Units[m_Input_data.at(x).range]);
					}

					//如果是小叶的设备,因为没有input 就直接显示rang 对应的值.
					if(bacnet_device_type == STM32_HUM_NET)
					{
						if(m_controller_data.at(i).units < sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0]))
						{
							m_controller_list.SetItemText(i,CONTROLLER_INPUTUNITS,Input_List_Analog_Units[m_controller_data.at(i).units]);
						}
					}
			
			}
		}
		else if((BAC_VAR + 1) == m_controller_data.at(i).input.point_type)
		{
			if(m_controller_data.at(i).input.number< BAC_VARIABLE_ITEM_COUNT)
			{	
					MultiByteToWideChar( CP_ACP, 0, (char *)m_Variable_data.at(m_controller_data.at(i).input.number ).label, 
						(int)strlen((char *)m_Variable_data.at(m_controller_data.at(i).input.number ).label)+1, 
						temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
					temp_des2.ReleaseBuffer();	



					if(temp_des2.GetLength()>9)
						temp_des2.Format(_T("%d-VAR%d"),m_controller_data.at(i).input.panel,m_controller_data.at(i).input.number + 1);
					if(temp_des2.IsEmpty())
						temp_des2.Format(_T("%d-VAR%d"),m_controller_data.at(i).input.panel,m_controller_data.at(i).input.number + 1);

					//temp_des3.Format(_T("%d"),m_Input_data.at(m_controller_data.at(i).input.number - 1).value);	
					CString cstemp_value;
					float temp_float_value;
					temp_float_value = ((float)m_controller_data.at(i).input_value) / 1000;

					cstemp_value.Format(_T("%.1f"),temp_float_value);
					m_controller_list.SetItemText(i,CONTROLLER_INPUTVALUE,cstemp_value);


					//********************************************************************************************************
					// bind the the input units.
					int x = m_controller_data.at(i).input.number;
					if(m_Variable_data.at(x).digital_analog == BAC_UNITS_ANALOG)
					{
						if(m_Variable_data.at(x).range <= sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]))
							m_controller_list.SetItemText(i,CONTROLLER_INPUTUNITS,Variable_Analog_Units_Array[m_Variable_data.at(x).range]);
					}
			
			}
		}
		else
		{
			m_controller_list.SetItemText(i,CONTROLLER_INPUTVALUE,_T(""));
			m_controller_list.SetItemText(i,CONTROLLER_INPUTUNITS,_T(""));
		}
		
		m_controller_list.SetItemText(i,CONTROLLER_INPUT,temp_des2);

		float persend_data;
		persend_data = ((float)m_controller_data.at(i).value)/ 1000;
		CString temp_output_value;
		temp_output_value.Format(_T("%.1f%%"),persend_data);
		m_controller_list.SetItemText(i,CONTROLLER_OUTPUT,temp_output_value);
			
		temp_des2.Empty();
		temp_des3.Empty();
		temp_set_unit.Empty();
		if(m_controller_data.at(i).setpoint.point_type == (BAC_OUT + 1)) 
		{
			if(m_controller_data.at(i).setpoint.number< BAC_OUTPUT_ITEM_COUNT)
			{

			}
		}
		else if(m_controller_data.at(i).setpoint.point_type == (BAC_IN + 1)) 
		{
			if(m_controller_data.at(i).setpoint.number< BAC_INPUT_ITEM_COUNT)
			{
					int num_point,num_panel,num_net,k;
					Point_T3000 point;
					point.number = m_controller_data.at(i).setpoint.number;
					point.number = point.number + 1;	//input setpoint 是从 0 开始计数的 ，但是要去找point label 要从1开始;
					point.panel = m_controller_data.at(i).setpoint.panel;
					point.point_type = m_controller_data.at(i).setpoint.point_type - 1;	//调用 ispoint的时候要减一;
					byte point_type,var_type;

					int temp_network = 0;
					char buf[255];
					char q[17];
					pointtotext_for_controller(q, &point);

					char * temp1 = ispoint(q,&num_point,&var_type, &point_type, &num_panel, &num_net, temp_network, point.panel, &k);
					if(temp1!=NULL)
					{
						if(strlen(temp1) < 255)
						{
							strcpy(buf,temp1);

							MultiByteToWideChar( CP_ACP, 0, (char *)buf,(int)strlen((char *)buf)+1, 
								temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
							temp_des2.ReleaseBuffer();	

							float temp_float_value;
							temp_float_value = ((float)m_controller_data.at(i).setpoint_value) / 1000;
							temp_des3.Format(_T("%.1f"),temp_float_value);



							int x = m_controller_data.at(i).setpoint.number;
							if(m_Input_data.at(x).digital_analog == BAC_UNITS_ANALOG)
							{

								if(m_Input_data.at(x).range <  (sizeof(Input_List_Analog_Units)/sizeof(Input_List_Analog_Units[0])))
									temp_set_unit = Input_List_Analog_Units[m_Input_data.at(x).range];
									
							}
						}
					}
				
			}
		}
		else if(m_controller_data.at(i).setpoint.point_type == (BAC_VAR + 1))	//Variable
		{
			if(m_controller_data.at(i).setpoint.number< BAC_VARIABLE_ITEM_COUNT)
			{
				int num_point,num_panel,num_net,k;
				Point_T3000 point;
				point.number = m_controller_data.at(i).setpoint.number;
				point.number = point.number + 1;	//input setpoint 是从 0 开始计数的 ，但是要去找point label 要从1开始;
				point.panel = m_controller_data.at(i).setpoint.panel;
				point.point_type = m_controller_data.at(i).setpoint.point_type - 1;	//调用 ispoint的时候要减一;
				byte point_type,var_type;

				int temp_network = 0;
				char buf[255];
				char q[17];
				pointtotext_for_controller(q, &point);

				char * temp1 = ispoint(q,&num_point,&var_type, &point_type, &num_panel, &num_net, temp_network, point.panel, &k);
				if(temp1!=NULL)
				{
					if(strlen(temp1) < 255)
					{
						strcpy(buf,temp1);

						MultiByteToWideChar( CP_ACP, 0, (char *)buf,(int)strlen((char *)buf)+1, 
							temp_des2.GetBuffer(MAX_PATH), MAX_PATH );
						temp_des2.ReleaseBuffer();	

						float temp_float_value;
						temp_float_value = ((float)m_controller_data.at(i).setpoint_value) / 1000;
						temp_des3.Format(_T("%.1f"),temp_float_value);



						int x = m_controller_data.at(i).setpoint.number;
						if(m_Variable_data.at(x).digital_analog == BAC_UNITS_DIGITAL)
						{


							if(m_Variable_data.at(x).range>30)
							{
								temp_set_unit = _T("");
							}
							else
							{
								CString temp1;
								CStringArray temparray;

								if((m_Variable_data.at(x).range < 23) &&(m_Variable_data.at(x).range !=0))
									temp1 = Digital_Units_Array[m_Variable_data.at(x).range];
								else if((m_Variable_data.at(x).range >=23) && (m_Variable_data.at(x).range <= 30))
								{
									if(receive_customer_unit)
										temp1 = Custom_Digital_Range[m_Variable_data.at(x).range - 23];
								}

								SplitCStringA(temparray,temp1,_T("/"));
								if((temparray.GetSize()==2))
								{
									if(m_Variable_data.at(x).control == 0)
										temp_set_unit = temparray.GetAt(0);
									else
										temp_set_unit = temparray.GetAt(1);
								}

							}
						}
						else
						{
							if(m_Variable_data.at(x).range == 20)	//如果是时间;
							{
								temp_set_unit = Variable_Analog_Units_Array[m_Variable_data.at(x).range];
								char temp_char[50];
								int time_seconds = m_Variable_data.at(x).value / 1000;
								intervaltotext(temp_char,time_seconds,0,0);
								CString temp_11;
								MultiByteToWideChar( CP_ACP, 0, temp_char, strlen(temp_char) + 1, 
									temp_11.GetBuffer(MAX_PATH), MAX_PATH );
								temp_11.ReleaseBuffer();		
								
								temp_set_unit = temp_11;
								//temp_value.Format(_T("%d"),m_Variable_data.at(i).value);
								//m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_value);
							}
							else if(m_Variable_data.at(x).range<=sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]))
							{
								//m_variable_list.SetItemText(i,VARIABLE_UNITE,Variable_Analog_Units_Array[m_Variable_data.at(i).range]);
								temp_set_unit = Variable_Analog_Units_Array[m_Variable_data.at(x).range];
								//CString cstemp_value;
								//float temp_float_value;
								//temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
								//cstemp_value.Format(_T("%.3f"),temp_float_value);
								//m_variable_list.SetItemText(i,VARIABLE_VALUE,cstemp_value);

								//temp_value.Format(_T("%d"),m_Variable_data.at(i).value);
								//m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_value);
							}
						}





					}
				}
			}
		}


		if(bacnet_device_type == STM32_HUM_NET)
		{
			temp_des3.Format(_T("%.2f"),((float)m_controller_data.at(i).setpoint_value)/1000);
		}

		m_controller_list.SetItemText(i,CONTROLLER_SETPOINT,temp_des2);
		m_controller_list.SetItemText(i,CONTROLLER_SETVALUE,temp_des3);
		m_controller_list.SetItemText(i,CONTROLLER_SETPOINTUNITS,temp_set_unit);
	
		if(m_controller_data.at(i).repeats_per_min == 0)
		{
			m_controller_list.SetItemText(i,CONTROLLER_I_TIME,PID_Time_Type[0]);
		}
		else
		{
			m_controller_list.SetItemText(i,CONTROLLER_I_TIME,PID_Time_Type[1]);
		}

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
			m_controller_data.at(i).bias = 0;
			m_controller_list.SetItemText(i,CONTROLLER_BIAS,_T("0"));
		}

		if(m_controller_data.at(i).rate<=200)
		{
			temp_des3.Format(_T("%.2f"),((float)m_controller_data.at(i).rate)/100);
			m_controller_list.SetItemText(i,CONTROLLER_RATE,temp_des3);
		}
		else
		{
			m_controller_data.at(i).rate = 0;
			m_controller_list.SetItemText(i,CONTROLLER_RATE,_T("0"));
		}

		if(isFreshOne)
		{
			break;
		}
	
	}
	
	return	 0;
}






LRESULT BacnetController::Fresh_Controller_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	if(Changed_Item>= PID_CONTROLLER_LIMITE_ITEM_COUNT)
	{
		m_controller_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
		return 0;
	}


	CString temp_task_info;
	CString New_CString =  m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
	CString cstemp_value;

	//先保存 原来的值，等结束的时候来比对，看是否有改变，有改变就进行写动作;
	memcpy_s(&m_temp_controller_data[Changed_Item],sizeof(Str_controller_point),&m_controller_data.at(Changed_Item),sizeof(Str_controller_point));


	if(Changed_SubItem == CONTROLLER_INPUT)
	{
		if(bacnet_device_type == STM32_HUM_NET) //小叶的设备不支持修改
		{
			return 0;
		}
		CString cs_temp = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.IsEmpty() == false)
		{
			cs_temp.MakeUpper();

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
				if(temp_number > 0);	//因为Input2  的number 是1;
					temp_number = temp_number - 1;
				temp_point_type = temp_point_type + 1;
				if(2 == temp_point_type)
				{
					m_controller_data.at(Changed_Item).input.number = temp_number;
					m_controller_data.at(Changed_Item).input.panel = temp_panel;//bac_gloab_panel;
					m_controller_data.at(Changed_Item).input.point_type = temp_point_type;//1 means input point

					CString temp_des3;
					if(temp_number < BAC_INPUT_ITEM_COUNT)
					{
						//temp_des3.Format(_T("%d"),m_Input_data.at(temp_number - 1).value);

						CString cstemp_value;
						float temp_float_value;
						temp_float_value = ((float)m_Input_data.at(temp_number).value) / 1000;
						temp_des3.Format(_T("%.1f"),temp_float_value);


						m_controller_data.at(Changed_Item).input_value = m_Input_data.at(temp_number).value;	

						m_controller_list.SetItemText(Changed_Item,CONTROLLER_INPUTVALUE,temp_des3);
					}
				}
				else if(3 == temp_point_type)
				{
					m_controller_data.at(Changed_Item).input.number = temp_number;
					m_controller_data.at(Changed_Item).input.panel = temp_panel;//bac_gloab_panel;
					m_controller_data.at(Changed_Item).input.point_type = temp_point_type;//1 means input point

					CString temp_des3;
					if(temp_number < BAC_VARIABLE_ITEM_COUNT)
					{
						//temp_des3.Format(_T("%d"),m_Input_data.at(temp_number - 1).value);

						CString cstemp_value;
						float temp_float_value;
						temp_float_value = ((float)m_Variable_data.at(temp_number).value) / 1000;
						temp_des3.Format(_T("%.1f"),temp_float_value);


						m_controller_data.at(Changed_Item).input_value = m_Variable_data.at(temp_number).value;	

						m_controller_list.SetItemText(Changed_Item,CONTROLLER_INPUTVALUE,temp_des3);
					}
				}
				else
				{
					m_controller_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
					return 0;
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
        else
        {
            m_controller_data.at(Changed_Item).input.number = 0;
            m_controller_data.at(Changed_Item).input.panel = 0;//bac_gloab_panel;
            m_controller_data.at(Changed_Item).input.point_type = 0;//1 means input point
        }


	}

	if(Changed_SubItem == CONTROLLER_INPUTVALUE)
	{
		if(bacnet_device_type == STM32_HUM_NET) //STM32_HUM_NET的设备不支持修改
		{
			return 0;
		}
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
		if(bacnet_device_type == STM32_HUM_NET) //STM32_HUM_NET的设备不支持修改
		{
			int temp_proportional=0;
			CString cs_temp = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
			m_controller_data.at(Changed_Item).setpoint_value = _wtoi(cs_temp)* 1000;

		}
		else
		{
			int temp_value = (int)(_wtof(New_CString) * 1000);
			if(m_controller_data.at(Changed_Item).setpoint.number == 0 )
				return 0;
			if((m_controller_data.at(Changed_Item).setpoint.number -1 )< BAC_VARIABLE_ITEM_COUNT)
				m_Variable_data.at(m_controller_data.at(Changed_Item).setpoint.number - 1).value = temp_value;

			temp_task_info.Format(_T("Write Controllers List Item%d .Changed the variable value to \"%s\" "),Changed_Item + 1,New_CString);
			Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,Changed_Item,Changed_Item,sizeof(Str_in_point),BacNet_hwd ,temp_task_info);
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

	if(Changed_SubItem == CONTROLLER_OUTPUT)
	{
		CString temp_cs = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);

		int temp_int = (int)(_wtof(temp_cs) * 1000);
		m_controller_data.at(Changed_Item).value = temp_int;
	}

	if(Changed_SubItem == CONTROLLER_SETPOINT)
	{
		CString cs_temp = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
        if (cs_temp.IsEmpty() == true)
        {
            m_controller_data.at(Changed_Item).setpoint.number = 0;
            m_controller_data.at(Changed_Item).setpoint.panel = 0;//bac_gloab_panel;
            m_controller_data.at(Changed_Item).setpoint.point_type = 0;//1 means input point
        }
        else
        {
            char cTemp1[255];
            char temp_setpoint[250];
            char * tempcs = NULL;
            cs_temp = cs_temp.MakeUpper();
            memset(cTemp1, 0, 255);
            memset(temp_setpoint, 0, 250);
            WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL);

            int temp_number = -1;
            byte temp_value_type = -1;
            byte temp_point_type = -1;
            int temp_panel = -1;
            int temp_net = -1;
            int k = 0;
            //int temp1;
            //Change the lable.ex: Change the
            tempcs = ispoint(cTemp1, &temp_number, &temp_value_type, &temp_point_type, &temp_panel, &temp_net, 0, bac_gloab_panel, &k);
            if (tempcs != NULL)
            {
                memcpy_s(temp_setpoint, 250, tempcs, 9);
                //strcpy_s(temp_setpoint,tempcs);
                //strcpy(temp_setpoint,tempcs);
                CString temp_des2;
                temp_des2.Empty();

                MultiByteToWideChar(CP_ACP, 0, temp_setpoint, STR_VARIABLE_LABEL + 1,
                    temp_des2.GetBuffer(MAX_PATH), MAX_PATH);
                temp_des2.ReleaseBuffer();

                m_controller_list.SetItemText(Changed_Item, CONTROLLER_SETPOINT, temp_des2);


                if (temp_panel != bac_gloab_panel)
                {
                    MessageBox(_T("Don't support other panel currently!"), _T("Warning"), MB_OK | MB_ICONINFORMATION);
                    m_controller_list.SetItemText(Changed_Item, Changed_SubItem, _T(""));
                    return 0;
                }
                if (temp_number > 0)	//Setpoint 也是这样;从0 开始的;
                    temp_number = temp_number - 1;
                temp_point_type = temp_point_type + 1; //OUTPUT=1, INPUT, VARIABLE 要错位;
                m_controller_data.at(Changed_Item).setpoint.number = temp_number;
                m_controller_data.at(Changed_Item).setpoint.panel = temp_panel;//bac_gloab_panel;
                m_controller_data.at(Changed_Item).setpoint.point_type = temp_point_type;//1 means input point

                CString temp_des3;
                if (temp_number < BAC_VARIABLE_ITEM_COUNT)
                {
                    if (m_Variable_data.at(temp_number).range < VARIABLE_ANALOG_UNITE_COUNT)
                        temp_des3 = Variable_Analog_Units_Array[m_Variable_data.at(temp_number).range];
                    m_controller_list.SetItemText(Changed_Item, CONTROLLER_SETPOINTUNITS, temp_des3);
                }
                char tempAAAA[250];
                memset(tempAAAA, 0, 250);
                temp_des3.Empty();
                if (m_Variable_data.at(temp_number).range == 20)
                {
                    if ((m_controller_data.at(Changed_Item).setpoint_value >= 0) && (m_controller_data.at(Changed_Item).setpoint_value<86400))
                        intervaltotext(tempAAAA, m_controller_data.at(Changed_Item).setpoint_value, 0, 0);

                    MultiByteToWideChar(CP_ACP, 0, tempAAAA, strlen(tempAAAA) + 1,
                        temp_des3.GetBuffer(MAX_PATH), MAX_PATH);
                    temp_des3.ReleaseBuffer();
                }
                else
                {
                    CString cstemp_value;
                    float temp_float_value;
                    temp_float_value = ((float)m_Variable_data.at(m_controller_data.at(Changed_Item).setpoint.number).value) / 1000;
                    temp_des3.Format(_T("%.1f"), temp_float_value);

                }

                m_controller_list.SetItemText(Changed_Item, CONTROLLER_SETVALUE, temp_des3);

            }
            else
            {
                CString temp_show_ret;
                temp_show_ret = _T("\"") + cs_temp + _T("\"") + _T(" is a invalid label or keyword .");
                SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_show_ret);
                m_controller_list.SetItemText(Changed_Item, CONTROLLER_SETPOINT, _T(""));
            }
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

		int i_value =  (int)(_wtof(cs_temp) * 100) ;

		if((i_value > 200) || (i_value < 0))
		//if((cs_temp.GetLength()>1) || (_wtoi(cs_temp)>2) )
		{
			MessageBox(_T("Please input a effective value  0.00 - 2.00"),_T("Information"),MB_OK |MB_ICONINFORMATION);
			m_controller_list.SetItemText(Changed_Item,Changed_SubItem,_T(""));
		}
		else
		{
			m_controller_data.at(Changed_Item).rate = i_value;
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

	if(Changed_SubItem == CONTROLLER_I_TIME)
	{
		CString cs_temp = m_controller_list.GetItemText(Changed_Item,Changed_SubItem);
		New_CString = cs_temp;
		if(cs_temp.CompareNoCase(PID_Time_Type[0]) == 0)
		{
			m_controller_data.at(Changed_Item).repeats_per_min = 0;
		}
		else if(cs_temp.CompareNoCase(PID_Time_Type[1]) == 0)
		{
			m_controller_data.at(Changed_Item).repeats_per_min = 1;
		}
		else
		{
			m_controller_data.at(Changed_Item).repeats_per_min = 0;
		}
	}

	cmp_ret = memcmp(&m_temp_controller_data[Changed_Item],&m_controller_data.at(Changed_Item),sizeof(Str_controller_point));
	if(cmp_ret!=0)
	{
		m_controller_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Controllers List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEPID_T3000,Changed_Item,Changed_Item,sizeof(Str_controller_point),m_controller_dlg_hwnd ,temp_task_info,Changed_Item,Changed_SubItem);
	}


	
	return 0;
}



void BacnetController::OnNMClickListController(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
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

	if(lRow>= PID_CONTROLLER_LIMITE_ITEM_COUNT)
		return;

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

			m_controller_list.SetCellEnabled(lRow,CONTROLLER_OUTPUT,1);


		}
		else
		{
			m_controller_data.at(lRow).auto_manual = 0;
			m_controller_list.SetItemText(lRow,CONTROLLER_AUTO_MANUAL,_T("Auto"));
			New_CString = _T("Auto");
			m_controller_list.SetCellEnabled(lRow,CONTROLLER_OUTPUT,0);

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
		Post_Write_Message(g_bac_instance,WRITEPID_T3000,(int8_t)lRow,(int8_t)lRow,sizeof(Str_controller_point),m_controller_dlg_hwnd,temp_task_info,lRow,lCol);
	}


	*pResult = 0;
}


void BacnetController::OnTimer(UINT_PTR nIDEvent)
{
	 
	switch(nIDEvent)
	{
	case 1:
		{
			if(g_protocol == PROTOCOL_BIP_TO_MSTP)
			{
				PostMessage(WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
			}
			else if((this->IsWindowVisible()) && (Gsm_communication == false) )	//GSM连接时不要刷新;
			{
				if(bac_select_device_online)
				{
					PostMessage(WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
					Post_Refresh_Message(g_bac_instance,READCONTROLLER_T3000,0,BAC_PID_COUNT - 1,sizeof(Str_controller_point),BAC_PID_GROUP);
				}
			}
		}
		break;
	case 2:
		{
			KillTimer(2);
			if(this->IsWindowVisible())
				PostMessage(WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
		}
		break;
	default :
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void BacnetController::OnClose()
{
	 
	ShowWindow(FALSE);
	return;
	KillTimer(1);
	m_controller_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}

void BacnetController::OnCancel()
{
	
	//KillTimer(1);
	//m_output_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	//CDialogEx::OnCancel();
}


int GetPidValue(int index,CString &Auto_M,CString &persend_data)
{
	CStringArray temparray;
	CString temp1;
	if(index >= BAC_PID_COUNT)
	{
		Auto_M.Empty();
		return -1;
	}
	int i = index;

	if(m_controller_data.at(i).auto_manual == 1)
	{
		Auto_M = _T("M");
	}
	else
	{
		Auto_M.Empty();
	}

	float temp_persend_data;
	temp_persend_data = ((float)m_controller_data.at(i).value)/ 1000;
	//CString temp_output_value;
	persend_data.Format(_T("%.1f%%"),temp_persend_data);

	return 1;
}

void BacnetController::OnSysCommand(UINT nID, LPARAM lParam)
{
	 

	if(nID == SC_MAXIMIZE)
	{
		if(window_max == false)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left  + 90 ,temp_mynew_rect.top + 70,500,700,SWP_SHOWWINDOW);
		}
		return;
	}

	CDialogEx::OnSysCommand(nID, lParam);
}


void BacnetController::Reset_Controller_Rect()
{

	CRect temp_mynew_rect;
	::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;

	CRect temp_window;
	GetWindowRect(&temp_window);

	if(window_max)
	{
		CRect temp_mynew_rect;
		::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height(), NULL);
	}
	else if((temp_window.Width() <= temp_mynew_rect.Width() ) && (temp_window.Height() <= temp_mynew_rect.Height()))
	{
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,0,0,SWP_NOSIZE );
	}
	else
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left + 90,temp_mynew_rect.top + 70,500,700, NULL);


	return;

}

void BacnetController::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	CRect rc;
	GetClientRect(rc);
	if(m_controller_list.m_hWnd != NULL)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0,0, 0,0,  SWP_NOSIZE | SWP_NOMOVE);
		//m_program_list.MoveWindow(&rc);
		m_controller_list.MoveWindow(rc.left,rc.top,rc.Width(),rc.Height() - 80);
	}
}


BOOL BacnetController::OnHelpInfo(HELPINFO* pHelpInfo)
{ 

	if (g_protocol==PROTOCOL_BACNET_IP){
		HWND hWnd;

		if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, 			
			theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, 			
			HH_HELP_CONTEXT, IDH_TOPIC_PID_LOOPS_INTRODUCTION_1);
		return (hWnd != NULL);
	}
	else{
		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
	}

	return CDialogEx::OnHelpInfo(pHelpInfo);
}