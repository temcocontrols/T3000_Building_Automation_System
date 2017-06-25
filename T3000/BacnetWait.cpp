// BacnetWait.cpp : implementation file
// Code by Fance

#include "stdafx.h"
#include "T3000.h"
#include "BacnetWait.h"
#include "afxdialogex.h"
#include "global_define.h"
#include "global_function.h"
extern _Refresh_Info Bacnet_Refresh_Info;
extern _Refresh_Write_Info Write_Config_Info;
// BacnetWait dialog

IMPLEMENT_DYNAMIC(BacnetWait, CDialogEx)

BacnetWait::BacnetWait(CWnd* pParent /*=NULL*/)
	: CDialogEx(BacnetWait::IDD, pParent)
	,m_wait_type(0)
{

}

BacnetWait::BacnetWait(int wait_type)
{
	m_wait_type = wait_type;
}

BacnetWait::~BacnetWait()
{
}

void BacnetWait::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WAIT_DETAIL, m_wait_detail);
	DDX_Control(pDX, IDC_PROGRESS1, m_wait_progress);
	DDX_Control(pDX, IDC_STATIC_PERSENT, m_wait_persent);
}


BEGIN_MESSAGE_MAP(BacnetWait, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// BacnetWait message handlers

void BacnetWait::Set_Read_Write(bool read_0_write_1)
{
	m_wait_type = read_0_write_1;
}

void BacnetWait::Set_Show_String(CString temp)
{
	PosString = temp;
}

void BacnetWait::Set_Pos(int temp_pos)
{
	m_pos = temp_pos;
}

BOOL BacnetWait::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	PosString.Empty();
	m_pos = 0;
	m_wait_detail.SetWindowTextW(_T("Send command to device..."));
	m_wait_detail.textColor(RGB(0,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_wait_detail.setFont(25,18,NULL,_T("Arial"));
	

	m_wait_persent.SetWindowTextW(_T("0%"));
	m_wait_persent.textColor(RGB(255,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_wait_persent.setFont(18,12,NULL,_T("Arial"));
	
	this->ShowWindow(SW_HIDE);

	
    if((m_wait_type == BAC_WAIT_NORMAL_READ ) || (m_wait_type == BAC_WAIT_READ_DATA_WRITE_CONFIG))	//Read one of the list or read all list;
	{
		SetTimer(1,200,NULL);
		SetTimer(5,120000,NULL);
	}
	else if(m_wait_type == BAC_WAIT_READ_MONITOR_DATA)
	{
		SetTimer(READ_MONITORDATA_TIME,40,NULL);
	}
	else
	{
		PostMessage(WM_CLOSE,NULL,NULL);
	}
	m_wait_progress.SetPos(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void BacnetWait::OnOK()
{
	

	CDialogEx::OnOK();
}


void BacnetWait::OnCancel()
{
	
	DestroyWindow();
	//CDialogEx::OnCancel();
}


BOOL BacnetWait::PreTranslateMessage(MSG* pMsg)
{
	
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void BacnetWait::OnTimer(UINT_PTR nIDEvent)
{
	this->ShowWindow(SW_HIDE);
	bool cotinue_waite = false;
	bool write_cotinue_waite = false;
	CString temp_pos;
	static int static_count=0;
	static_count = (++static_count)%(5+BAC_READ_GROUP_NUMBER*2-1);
	CString tempcs,tempcs2;
	if(m_wait_type ==1)
	{
		tempcs.Format(_T("Write Config File"));
	}
	else
	{
		tempcs.Format(_T("   Reading data "));

	}
	for (int i=0;i<=static_count;i++)
	{
		tempcs2 = tempcs2 + _T(".");
	}




	if(m_wait_type == BAC_WAIT_READ_DATA_WRITE_CONFIG)	//这里是 读取config 所要的 资料;
	{
		int read_config_total_count = BAC_INPUT_GROUP +
									  BAC_OUTPUT_GROUP +
									  BAC_PROGRAM_GROUP +
									  BAC_VARIABLE_GROUP +
									  BAC_PID_GROUP +
									  BAC_PROGRAMCODE_GROUP +
									  BAC_SCREEN_GROUP +
									  BAC_SCHEDULE_GROUP +
									  BAC_HOLIDAY_GROUP +
									  BAC_MONITOR_GROUP +
									  BAC_SCHEDULECODE_GOUP +
									  BAC_HOLIDAYCODE_GROUP;//这里还有很多要加;
									  BAC_BASIC_SETTING_GROUP;
		int read_config_all_step = 1000 / read_config_total_count;
		int read_config_success_count =0;
		int read_config_pos = 0;
		for (int i=0;i<BAC_PROGRAM_GROUP;i++ )
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_Program_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}
		for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++ )
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}

		for (int i=0;i<BAC_HOLIDAYCODE_GROUP;i++ )
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_Annualcode_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}

		
		for (int i=0;i<BAC_BASIC_SETTING_GROUP;i++)
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_BasicSetting_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}

		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_Input_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}

		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_Output_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}

		for (int i=0;i<BAC_VARIABLE_GROUP ;i++ )
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_Variable_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}

		for (int i=0;i<BAC_PID_GROUP ;i++ )
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_Controller_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}

		for (int i=0;i<BAC_SCREEN_GROUP ;i++ )
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_Screen_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}

		for (int i=0;i<BAC_SCHEDULE_GROUP ;i++ )
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}

		for (int i=0;i<BAC_SCHEDULECODE_GOUP;i++ )
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_Weeklycode_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}


		for (int i=0;i<BAC_HOLIDAY_GROUP ;i++ )
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_Annual_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}

		for (int i=0;i<BAC_MONITOR_GROUP ;i++ )
		{
			if(bac_read_which_list ==BAC_READ_SVAE_CONFIG)
			{
				if(Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result == BAC_RESULTS_OK)
				{
					read_config_success_count ++;
					read_config_pos = read_config_pos +read_config_all_step;
				}
			}
		}

		read_config_pos = read_config_pos /10;
		if(read_config_pos>=99)
			read_config_pos=100;
		m_wait_progress.SetPos(read_config_pos);
		temp_pos.Format(_T("%d%%"),read_config_pos);
		g_progress_persent = read_config_pos;

	}
	else if(m_wait_type == BAC_WAIT_NORMAL_READ)//这里是点击的时候读，只要INput output var program 和time;
	{
		int total_count = BAC_INPUT_GROUP + 
			BAC_OUTPUT_GROUP +	
			BAC_PROGRAM_GROUP +		
			BAC_VARIABLE_GROUP +
			BAC_TIME_COMMAND_GROUP +
			BAC_SCHEDULE_GROUP +
			BAC_HOLIDAY_GROUP +
			BAC_PID_GROUP +
			BAC_SCREEN_GROUP +
			BAC_MONITOR_GROUP;// +
			//BAC_TSTAT_GROUP; 
		int graphic_label_step = 1000 / BAC_GRPHIC_LABEL_GROUP;
		int in_out_var_step = 1000 / (BAC_INPUT_GROUP + BAC_OUTPUT_GROUP + BAC_VARIABLE_GROUP);

		int all_step = 1000 / total_count;
		int input_step = 1000 / BAC_INPUT_GROUP;
		int output_step = 1000 / BAC_OUTPUT_GROUP;
		int program_step = 1000 / BAC_PROGRAM_GROUP;
		int variable_step = 1000 / BAC_VARIABLE_GROUP;
		int weekly_step  = 1000/ BAC_SCHEDULE_GROUP;
		int annual_step = 1000 / BAC_HOLIDAY_GROUP;
		int time_command_step = 1000 / BAC_TIME_COMMAND_GROUP;
		int controller_step = 1000 / BAC_PID_GROUP;
		int screen_step = 1000 / BAC_SCREEN_GROUP;
		int monitor_step = 1000 /BAC_MONITOR_GROUP;
		int alarmlog_step = 1000 /BAC_ALARMLOG_GROUP;
		int tstat_step = 1000 / BAC_TSTAT_GROUP;
		int customer_unit_step = 1000 / BAC_CUSTOMER_UNIT_GROUP;
		int user_login_step = 1000 / BAC_CUSTOMER_UNIT_GROUP;
		tempcs = tempcs + tempcs2;
		int success_count =0;
		int pos=0;

		for (int i=0;i<BAC_GRPHIC_LABEL_GROUP;i++)
		{
			if(bac_read_which_list == BAC_READ_GRAPHIC_LABEL_INFO)
			{
				if(Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + graphic_label_step;
				}
			}
		}


		for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
		{
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Time_Command[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos +all_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_TIME_COMMAND)
			{
				if(Bacnet_Refresh_Info.Read_Time_Command[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + time_command_step;
				}
			}
		}


		for (int i=0;i<BAC_USER_LOGIN_GROUP;i++ )
		{
			if(bac_read_which_list == BAC_READ_USER_LOGIN_INFO)
			{
				if(Bacnet_Refresh_Info.Read_UserLogin[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + user_login_step;
				}
			}
		}

		for (int i=0;i<BAC_CUSTOMER_UNIT_GROUP;i++ )
		{
			if(bac_read_which_list == BAC_READ_TSTAT_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Customer_unit_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + customer_unit_step;
				}
			}
		}


		for (int i=0;i<BAC_PROGRAM_GROUP;i++ )
		{
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Program_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos +all_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_PROGRAM_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Program_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + program_step;
				}
			}
		}

		for (int i=0;i<BAC_SCHEDULE_GROUP;i++ )
		{
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos +all_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_WEEKLY_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + weekly_step;
				}
			}
		}

		for (int i=0;i<BAC_HOLIDAY_GROUP;i++ )
		{
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Annual_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos +all_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_WEEKLY_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Annual_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + annual_step;
				}
			}
		}

		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Input_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos +all_step;
				}
			}
			if(bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Input_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + in_out_var_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_INPUT_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Input_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + input_step;
				}
			}
		}

		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Output_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos +all_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Output_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + in_out_var_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_OUTPUT_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Output_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + output_step;
				}
			}
		}

		for (int i=0;i<BAC_VARIABLE_GROUP ;i++ )
		{
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Variable_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos +all_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Variable_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + in_out_var_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_VARIABLE_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Variable_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + variable_step;
				}
			}
		}

		for (int i=0;i<BAC_ALARMLOG_GROUP ;i++ )
		{
			/*			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			{
			if(Bacnet_Refresh_Info.Read_AlarmLog_Info[i].task_result == BAC_RESULTS_OK)
			{
			success_count ++;
			pos = pos +all_step;
			}
			}
			else */if(bac_read_which_list == BAC_READ_ALARMLOG_LIST)
			{
				if(Bacnet_Refresh_Info.Read_AlarmLog_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + alarmlog_step;
				}
			}
		}

		for (int i=0;i<BAC_PID_GROUP ;i++ )
		{
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Controller_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos +all_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_CONTROLLER_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Controller_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + controller_step;
				}
			}
		}

		for (int i=0;i<BAC_SCREEN_GROUP ;i++ )
		{
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Screen_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos +all_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_SCREEN_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Screen_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + screen_step;
				}
			}
		}

		for (int i=0;i<BAC_MONITOR_GROUP ;i++ )
		{
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos +all_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_MONITOR_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + monitor_step;
				}
			}
		}

		pos = pos /10;
		//TRACE(_T("%d\r\n"),pos);
		if(pos>=98)
			pos=100;
		m_wait_progress.SetPos(pos);
		temp_pos.Format(_T("%d%%"),pos);
		g_progress_persent = pos;
		if(bac_read_which_list != BAC_READ_ALL_LIST)
		{
			g_progress_persent = 0;
			this->ShowWindow(SW_HIDE);
		}
		if(bac_read_which_list == BAC_READ_GRAPHIC_LABEL_INFO)	// 如果是第一次 读 图片里面的label,就慢慢读,并且显示 进度条.
		{
			g_progress_persent = pos;
			//this->ShowWindow(SW_SHOW);
		}

	}

	m_wait_persent.SetWindowTextW(temp_pos);

	UpdateData(FALSE);

	switch(nIDEvent)
	{
	case 1:
		{
			if(bac_read_which_list == BAC_READ_GRAPHIC_LABEL_INFO)
			{
				for (int i=0;i<BAC_GRPHIC_LABEL_GROUP;i++)
				{
					if(Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].task_result == BAC_RESULTS_UNKONW)
					{
						if(Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].invoke_id > 0)
						{
							Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].timeout_count ++;
						}
						if(Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].timeout_count > 20)
						{
							Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].task_result = BAC_RESULTS_FAIL;
							Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].timeout_count = 0;
							continue;
						}
						m_wait_detail.SetWindowTextW(tempcs);
						cotinue_waite = true;
						break;
						//goto endthis;
					}
					else if(Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].task_result == BAC_RESULTS_FAIL)
					{
						cotinue_waite = true;
						Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].resend_count ++ ;
						//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
						if((Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].resend_count>RESEND_COUNT) 
							|| (Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
						{
							//m_wait_detail.SetWindowTextW(_T("Read Tstats table Time Out!"));
							m_wait_persent.SetWindowTextW(_T("Timeout"));
							KillTimer(1);
							SetTimer(2,2000,NULL);
							goto endthis;
						}

						g_invoke_id = GetPrivateData(
							Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].device_id,
							Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].command,
							Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].start_instance,
							Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].end_instance,
							sizeof(Str_Units_element));
						if(g_invoke_id<0)	//如果没有获取到 就继续循环;
						{
							Sleep(50);
							continue;
						}
						Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].has_resend_yes_or_no ++;
						Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
						Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

						CString temp_cs_show;
						temp_cs_show.Format(_T("Read graphic label "));
						Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);
					}
				}
			}

			if(bac_read_which_list == BAC_READ_CUSTOMER_UNITS)
			{
				for (int i=0;i<BAC_CUSTOMER_UNIT_GROUP;i++)
				{

					if(Bacnet_Refresh_Info.Read_Customer_unit_Info[i].task_result == BAC_RESULTS_UNKONW)
					{
						if(Bacnet_Refresh_Info.Read_Customer_unit_Info[i].invoke_id > 0)
						{
							Bacnet_Refresh_Info.Read_Customer_unit_Info[i].timeout_count ++;
						}
						if(Bacnet_Refresh_Info.Read_Customer_unit_Info[i].timeout_count > 20)
						{
							Bacnet_Refresh_Info.Read_Customer_unit_Info[i].task_result = BAC_RESULTS_FAIL;
							Bacnet_Refresh_Info.Read_Customer_unit_Info[i].timeout_count = 0;
							continue;
						}
						m_wait_detail.SetWindowTextW(tempcs);
						cotinue_waite = true;
						break;
						//goto endthis;
					}
					else if(Bacnet_Refresh_Info.Read_Customer_unit_Info[i].task_result == BAC_RESULTS_FAIL)
					{
						cotinue_waite = true;
						Bacnet_Refresh_Info.Read_Customer_unit_Info[i].resend_count ++ ;
						//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
						if((Bacnet_Refresh_Info.Read_Customer_unit_Info[i].resend_count>RESEND_COUNT) 
							|| (Bacnet_Refresh_Info.Read_Customer_unit_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
						{
							//m_wait_detail.SetWindowTextW(_T("Read Tstats table Time Out!"));
							m_wait_persent.SetWindowTextW(_T("Timeout"));
							KillTimer(1);
							SetTimer(2,2000,NULL);
							goto endthis;
						}

						g_invoke_id = GetPrivateData(
							Bacnet_Refresh_Info.Read_Customer_unit_Info[i].device_id,
							Bacnet_Refresh_Info.Read_Customer_unit_Info[i].command,
							Bacnet_Refresh_Info.Read_Customer_unit_Info[i].start_instance,
							Bacnet_Refresh_Info.Read_Customer_unit_Info[i].end_instance,
							sizeof(Str_Units_element));
						if(g_invoke_id<0)	//如果没有获取到 就继续循环;
						{
							Sleep(50);
							continue;
						}
						Bacnet_Refresh_Info.Read_Customer_unit_Info[i].has_resend_yes_or_no ++;
						Bacnet_Refresh_Info.Read_Customer_unit_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
						Bacnet_Refresh_Info.Read_Customer_unit_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

						CString temp_cs_show;
						temp_cs_show.Format(_T("Read Custom units "));
						Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);
					}
				}
			}

			if(bac_read_which_list == BAC_READ_USER_LOGIN_INFO)
			{
				for (int i=0;i<BAC_USER_LOGIN_GROUP;i++)
				{
					if(Bacnet_Refresh_Info.Read_UserLogin[i].task_result == BAC_RESULTS_UNKONW)
					{
						if(Bacnet_Refresh_Info.Read_UserLogin[i].invoke_id > 0)
						{
							Bacnet_Refresh_Info.Read_UserLogin[i].timeout_count ++;
						}
						if(Bacnet_Refresh_Info.Read_UserLogin[i].timeout_count > 20)
						{
							Bacnet_Refresh_Info.Read_UserLogin[i].task_result = BAC_RESULTS_FAIL;
							Bacnet_Refresh_Info.Read_UserLogin[i].timeout_count = 0;
							continue;
						}
						m_wait_detail.SetWindowTextW(tempcs);
						cotinue_waite = true;
						break;
						//goto endthis;
					}
					else if(Bacnet_Refresh_Info.Read_UserLogin[i].task_result == BAC_RESULTS_FAIL)
					{
						cotinue_waite = true;
						Bacnet_Refresh_Info.Read_UserLogin[i].resend_count ++ ;
						//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
						if((Bacnet_Refresh_Info.Read_UserLogin[i].resend_count>RESEND_COUNT) 
							|| (Bacnet_Refresh_Info.Read_UserLogin[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
						{
							//m_wait_detail.SetWindowTextW(_T("Read Tstats table Time Out!"));
							m_wait_persent.SetWindowTextW(_T("Timeout"));
							KillTimer(1);
							SetTimer(2,2000,NULL);
							goto endthis;
						}

						g_invoke_id = GetPrivateData(
							Bacnet_Refresh_Info.Read_UserLogin[i].device_id,
							Bacnet_Refresh_Info.Read_UserLogin[i].command,
							Bacnet_Refresh_Info.Read_UserLogin[i].start_instance,
							Bacnet_Refresh_Info.Read_UserLogin[i].end_instance,
							sizeof(Str_userlogin_point));
						if(g_invoke_id<0)	//如果没有获取到 就继续循环;
						{
							Sleep(50);
							continue;
						}
						Bacnet_Refresh_Info.Read_UserLogin[i].has_resend_yes_or_no ++;
						Bacnet_Refresh_Info.Read_UserLogin[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
						Bacnet_Refresh_Info.Read_UserLogin[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

						CString temp_cs_show;
						temp_cs_show.Format(_T("Read Tstats list "));
						Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);
					}
				}
			}

		for (int i=0;i<BAC_MONITOR_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_MONITOR_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_ALL_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_Monitor_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_Monitor_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_Monitor_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_Monitor_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					break;
					//goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					cotinue_waite = true;
					Bacnet_Refresh_Info.Read_Monitor_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Monitor_Info[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_Monitor_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read Monitor table Time Out!"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}

					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_Monitor_Info[i].device_id,
						Bacnet_Refresh_Info.Read_Monitor_Info[i].command,
						Bacnet_Refresh_Info.Read_Monitor_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance,
						sizeof(Str_monitor_point));
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_Monitor_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_Monitor_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Read Monitor list "));
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);


					//TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
					//	Bacnet_Refresh_Info.Read_Monitor_Info[i].start_instance,
					//	Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance);
					

				}
			}
		}



		for (int i=0;i<BAC_SCREEN_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_SCREEN_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_ALL_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Screen_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_Screen_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_Screen_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_Screen_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_Screen_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_Screen_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					//goto endthis;
					break;
				}
				else if(Bacnet_Refresh_Info.Read_Screen_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					cotinue_waite = true;
					Bacnet_Refresh_Info.Read_Screen_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Screen_Info[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_Screen_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read Screen table Time Out!"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}

					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_Screen_Info[i].device_id,
						Bacnet_Refresh_Info.Read_Screen_Info[i].command,
						Bacnet_Refresh_Info.Read_Screen_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Screen_Info[i].end_instance,
						sizeof(Control_group_point));
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_Screen_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_Screen_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_Screen_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Read Screen list "));
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);


					//TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
					//	Bacnet_Refresh_Info.Read_Screen_Info[i].start_instance,
					//	Bacnet_Refresh_Info.Read_Screen_Info[i].end_instance);
					

				}
			}
		}

		for (int i=0;i<BAC_PID_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_CONTROLLER_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_ALL_LIST))
			{


				if(Bacnet_Refresh_Info.Read_Controller_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_Controller_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_Controller_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_Controller_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_Controller_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					//goto endthis;
					break;
				}
				else if(Bacnet_Refresh_Info.Read_Controller_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					cotinue_waite = true;
					Bacnet_Refresh_Info.Read_Controller_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
						if((Bacnet_Refresh_Info.Read_Controller_Info[i].resend_count>RESEND_COUNT) 
							|| (Bacnet_Refresh_Info.Read_Controller_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read Controller table Time Out!"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}

					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_Controller_Info[i].device_id,
						Bacnet_Refresh_Info.Read_Controller_Info[i].command,
						Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance,
						sizeof(Str_controller_point));
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_Controller_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_Controller_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Read Controller list "),g_invoke_id);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);


					//TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
					//	Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance,
					//	Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance);


				}
			}
		}
		for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_TIME_COMMAND) || (bac_read_which_list == BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
			{


				if(Bacnet_Refresh_Info.Read_Time_Command[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_Time_Command[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_Time_Command[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_Time_Command[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_Time_Command[i].timeout_count = 0;
						continue;
					}

					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					//goto endthis;
					break;
				}
				else if(Bacnet_Refresh_Info.Read_Time_Command[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Time_Command[i].resend_count ++ ;
					cotinue_waite = true;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Time_Command[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_Time_Command[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read Device Time table Time Out!"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}
					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_Time_Command[i].device_id,
						Bacnet_Refresh_Info.Read_Time_Command[i].command,
						Bacnet_Refresh_Info.Read_Time_Command[i].start_instance,
						Bacnet_Refresh_Info.Read_Time_Command[i].end_instance,
						sizeof(Time_block_mini));
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_Time_Command[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_Time_Command[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Read device time "),g_invoke_id);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);


					//TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
					//	Bacnet_Refresh_Info.Read_Time_Command[i].start_instance,
					//	Bacnet_Refresh_Info.Read_Time_Command[i].end_instance);


				}
			}
		}


		for (int i=0;i<BAC_BASIC_SETTING_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_BASIC_SETTING_COMMAND) || (bac_read_which_list == BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
			{


				if(Bacnet_Refresh_Info.Read_BasicSetting_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_BasicSetting_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_BasicSetting_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_BasicSetting_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_BasicSetting_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_BasicSetting_Info[i].timeout_count = 0;
						continue;
					}

					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					//goto endthis;
					break;
				}
				else if(Bacnet_Refresh_Info.Read_BasicSetting_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_BasicSetting_Info[i].resend_count ++ ;
					cotinue_waite = true;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_BasicSetting_Info[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_BasicSetting_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read Basic Setting Time Out!"));
						TRACE(_T("Read Basic Setting Time Out!\r\n"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}
					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_BasicSetting_Info[i].device_id,
						Bacnet_Refresh_Info.Read_BasicSetting_Info[i].command,
						Bacnet_Refresh_Info.Read_BasicSetting_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_BasicSetting_Info[i].end_instance,
						sizeof(Str_Setting_Info));
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_BasicSetting_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_BasicSetting_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_BasicSetting_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;
					CString temp_cs_show;
					temp_cs_show.Format(_T("Read basic setting "),g_invoke_id);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);


					TRACE(_T("Read basic setting Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_BasicSetting_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_BasicSetting_Info[i].end_instance);


				}
			}
		}



		for (int i=0;i<BAC_HOLIDAY_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_ANNUAL_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST)|| (bac_read_which_list == TYPE_SVAE_CONFIG))
			{


				if(Bacnet_Refresh_Info.Read_Annual_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_Annual_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_Annual_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_Annual_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_Annual_Info[i].timeout_count = 0;
						continue;
					}

					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					break;
					//goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Annual_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					cotinue_waite = true;
					Bacnet_Refresh_Info.Read_Annual_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Annual_Info[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_Annual_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read Annual Table Time Out!"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}

					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_Annual_Info[i].device_id,
						Bacnet_Refresh_Info.Read_Annual_Info[i].command,
						Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance,
						sizeof(Str_annual_routine_point));
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_Annual_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_Annual_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Read annual routine from item %d to %d "),g_invoke_id,
						Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					//TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
					//	Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance,
					//	Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance);


				}
			}
		}

		for (int i=0;i<BAC_SCHEDULE_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_WEEKLY_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST)|| (bac_read_which_list == TYPE_SVAE_CONFIG))
			{


				if(Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_Weekly_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_Weekly_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_Weekly_Info[i].timeout_count = 0;
						continue;
					}

					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					break;
					//goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Weekly_Info[i].resend_count ++ ;
					cotinue_waite = true;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Weekly_Info[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_Weekly_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read Weekly Routine Table Time Out!"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}
					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_Weekly_Info[i].device_id,
						Bacnet_Refresh_Info.Read_Weekly_Info[i].command,
						Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance,
						sizeof(Str_weekly_routine_point));
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_Weekly_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;


					CString temp_cs_show;
					temp_cs_show.Format(_T("Read weekly routine from item %d to %d "),
						Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					//TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
					//	Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance,
					//	Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance);


				}
			}
		}

		for (int i=0;i<BAC_PROGRAM_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_PROGRAM_LIST) || (bac_read_which_list == BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
			{
				if(Bacnet_Refresh_Info.Read_Program_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_Program_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_Program_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_Program_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_Program_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					break;
					//goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Program_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Program_Info[i].resend_count ++ ;
					cotinue_waite = true;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Program_Info[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_Program_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read input Table Time Out!"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}
					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_Program_Info[i].device_id,
						Bacnet_Refresh_Info.Read_Program_Info[i].command,
						Bacnet_Refresh_Info.Read_Program_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Program_Info[i].end_instance,
						sizeof(Str_program_point));
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_Program_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_Program_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;
					
					CString temp_cs_show;
					temp_cs_show.Format(_T("Read program list from item %d to %d "),
						Bacnet_Refresh_Info.Read_Program_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Program_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);
					
					
					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					//TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
					//	Bacnet_Refresh_Info.Read_Program_Info[i].start_instance,
					//	Bacnet_Refresh_Info.Read_Program_Info[i].end_instance);


				}
			}
		}

		for (int i=0;i<BAC_ALARMLOG_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_ALARMLOG_LIST) /*|| (bac_read_which_list == BAC_READ_ALL_LIST)*/)
			{
				if(Bacnet_Refresh_Info.Read_AlarmLog_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_AlarmLog_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_AlarmLog_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_AlarmLog_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_AlarmLog_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_AlarmLog_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					break;
					//goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_AlarmLog_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_AlarmLog_Info[i].resend_count ++ ;
					cotinue_waite = true;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_AlarmLog_Info[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_AlarmLog_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read Alarm Log Time Out!"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}
					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_AlarmLog_Info[i].device_id,
						Bacnet_Refresh_Info.Read_AlarmLog_Info[i].command,
						Bacnet_Refresh_Info.Read_AlarmLog_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_AlarmLog_Info[i].end_instance,
						sizeof(Alarm_point));
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_AlarmLog_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_AlarmLog_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_AlarmLog_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Read Alarm Log %d "),
						Bacnet_Refresh_Info.Read_AlarmLog_Info[i].start_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

				}
			}
		}

#if 1

		if((bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
			{
					if(Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result == BAC_RESULTS_UNKONW)
					{
						m_wait_detail.SetWindowTextW(tempcs);
						cotinue_waite = true;
						break;
						//goto endthis;
					}
					else if(Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result == BAC_RESULTS_FAIL)
					{

							KillTimer(1);
							SetTimer(2,2000,NULL);
							goto endthis;
					}
			}
		}
#endif

		if(bac_read_which_list == BAC_READ_WEEKLTCODE_LIST)
		{
			if(Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].task_result == BAC_RESULTS_UNKONW)
			{
				if(Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].invoke_id > 0)
				{
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].timeout_count ++;
				}
				if(Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].timeout_count > 20)
				{
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].task_result = BAC_RESULTS_FAIL;
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].timeout_count = 0;
				}
				m_wait_detail.SetWindowTextW(tempcs);
				goto endthis;
			}
			else if(Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].task_result == BAC_RESULTS_FAIL)
			{
				Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].resend_count ++ ;
				cotinue_waite = true;
				//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
				if((Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].resend_count>RESEND_COUNT) 
					|| (Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].has_resend_yes_or_no > FAIL_RESEND_COUNT))
				{
					//m_wait_detail.SetWindowTextW(_T("Read Weekly Schedule Time Out!"));
					m_wait_persent.SetWindowTextW(_T("Timeout"));
					KillTimer(1);
					SetTimer(2,2000,NULL);
					goto endthis;
				}
				g_invoke_id = GetPrivateData(
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].device_id,
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].command,
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].start_instance,
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].end_instance,
					WEEKLY_SCHEDULE_SIZE);
				if(g_invoke_id<0)	//如果没有获取到 就继续循环;
				{
					goto endthis;
				}
				Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].has_resend_yes_or_no ++;
				Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
				Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

				CString temp_cs_show;
				temp_cs_show.Format(_T("Read schedule time list from item %d to %d "),
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].start_instance,
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].end_instance);
				Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);


				//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

				//TRACE(_T("Program code Resend start = %d , Resend end = %d\r\n"),
				//	Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].start_instance,
				//	Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].end_instance);


			}
		}
		else if((bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			for (int i=0;i<BAC_SCHEDULECODE_GOUP;i++)
			{
				if(Bacnet_Refresh_Info.Read_Weeklycode_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_Weeklycode_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_Weeklycode_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					break;
					//goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Weeklycode_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Weeklycode_Info[i].resend_count ++ ;
					cotinue_waite = true;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Weeklycode_Info[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_Weeklycode_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read weekly schedule Time Out!"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}
					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].device_id,
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].command,
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].end_instance,
						WEEKLY_SCHEDULE_SIZE);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_Weeklycode_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_Weeklycode_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_Weeklycode_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Read Schedule time list from item %d to %d "),
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);


					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					//TRACE(_T("Weekly schedule time Resend start = %d , Resend end = %d\r\n"),
					//	Bacnet_Refresh_Info.Read_Weeklycode_Info[i].start_instance,
					//	Bacnet_Refresh_Info.Read_Weeklycode_Info[i].end_instance);


				}
			}
		}

		if(bac_read_which_list == BAC_READ_ANNUALCODE_LIST)
		{
			if(Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].task_result == BAC_RESULTS_UNKONW)
			{
				if(Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].invoke_id > 0)
				{
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].timeout_count ++;
				}
				if(Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].timeout_count > 20)
				{
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].task_result = BAC_RESULTS_FAIL;
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].timeout_count = 0;
				}
				m_wait_detail.SetWindowTextW(tempcs);
				goto endthis;
			}
			else if(Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].task_result == BAC_RESULTS_FAIL)
			{
				Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].resend_count ++ ;
				cotinue_waite = true;
				//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
				if((Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].resend_count>RESEND_COUNT) 
					|| (Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].has_resend_yes_or_no > FAIL_RESEND_COUNT))
				{
					//m_wait_detail.SetWindowTextW(_T("Read annual day Time Out!"));
					m_wait_persent.SetWindowTextW(_T("Timeout"));
					KillTimer(1);
					SetTimer(2,2000,NULL);
					goto endthis;
				}
				g_invoke_id = GetPrivateData(
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].device_id,
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].command,
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].start_instance,
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].end_instance,
					WEEKLY_SCHEDULE_SIZE);
				if(g_invoke_id<0)	//如果没有获取到 就继续循环;
				{
					goto endthis;
				}
				Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].has_resend_yes_or_no ++;
				Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
				Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

				CString temp_cs_show;
				temp_cs_show.Format(_T("Read Holiday list from item %d to %d "),
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].start_instance,
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].end_instance);
				Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);


				//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

				//TRACE(_T("Program code Resend start = %d , Resend end = %d\r\n"),
				//	Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].start_instance,
				//	Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].end_instance);


			}
		}
		else if((bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			for (int i=0;i<BAC_HOLIDAYCODE_GROUP;i++)
			{
				if(Bacnet_Refresh_Info.Read_Annualcode_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_Annualcode_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_Annualcode_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					break;
					//goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Annualcode_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Annualcode_Info[i].resend_count ++ ;
					cotinue_waite = true;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Annualcode_Info[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_Annualcode_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read annual day Time Out!"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}
					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].device_id,
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].command,
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].end_instance,
						WEEKLY_SCHEDULE_SIZE);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_Annualcode_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_Annualcode_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_Annualcode_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Read Holiday list from item %d to %d "),
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);


					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					//TRACE(_T("Weekly schedule time Resend start = %d , Resend end = %d\r\n"),
					//	Bacnet_Refresh_Info.Read_Annualcode_Info[i].start_instance,
					//	Bacnet_Refresh_Info.Read_Annualcode_Info[i].end_instance);


				}
			}
		}

		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			 if((bac_read_which_list == BAC_READ_INPUT_LIST)|| (bac_read_which_list == BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Input_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_Input_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_Input_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_Input_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_Input_Info[i].timeout_count = 0;
						continue;
					}

					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					break;
					//goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Input_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Input_Info[i].resend_count ++ ;
					cotinue_waite = true;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Input_Info[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_Input_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read input Table Time Out!"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}
					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_Input_Info[i].device_id,
						Bacnet_Refresh_Info.Read_Input_Info[i].command,
						Bacnet_Refresh_Info.Read_Input_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Input_Info[i].end_instance,
						sizeof(Str_in_point));
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_Input_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_Input_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;
					
					CString temp_cs_show;
					temp_cs_show.Format(_T("Read input list from item %d to %d "),
						Bacnet_Refresh_Info.Read_Input_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Input_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);
					
					
					
					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					//TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
					//	Bacnet_Refresh_Info.Read_Input_Info[i].start_instance,
					//	Bacnet_Refresh_Info.Read_Input_Info[i].end_instance);

				}
			}

		}

		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_OUTPUT_LIST)|| (bac_read_which_list == BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Output_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
						if(Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id > 0)
						{
							Bacnet_Refresh_Info.Read_Output_Info[i].timeout_count ++;
						}
						if(Bacnet_Refresh_Info.Read_Output_Info[i].timeout_count > 20)
						{
							Bacnet_Refresh_Info.Read_Output_Info[i].task_result = BAC_RESULTS_FAIL;
							Bacnet_Refresh_Info.Read_Output_Info[i].timeout_count = 0;
							continue;
						}
					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					break;
					//goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Output_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Output_Info[i].resend_count ++ ;
					cotinue_waite = true;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Output_Info[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_Output_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						//m_wait_detail.SetWindowTextW(_T("Read input Table Time Out!"));
						m_wait_persent.SetWindowTextW(_T("Timeout"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}
					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_Output_Info[i].device_id,
						Bacnet_Refresh_Info.Read_Output_Info[i].command,
						Bacnet_Refresh_Info.Read_Output_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Output_Info[i].end_instance,
						sizeof(Str_out_point));
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_Output_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_Output_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;
					
					
					CString temp_cs_show;
					temp_cs_show.Format(_T("Read output list from item %d to %d "),
						Bacnet_Refresh_Info.Read_Output_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Output_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);
					
					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					//TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
					//	Bacnet_Refresh_Info.Read_Output_Info[i].start_instance,
					//	Bacnet_Refresh_Info.Read_Output_Info[i].end_instance);

				}
			}

		}

		for (int i=0;i<BAC_VARIABLE_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_VARIABLE_LIST) || (bac_read_which_list == BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Variable_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_Variable_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_Variable_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_Variable_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					cotinue_waite = true;
					break;
					//goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Variable_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Variable_Info[i].resend_count ++ ;
					cotinue_waite = true;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
						if((Bacnet_Refresh_Info.Read_Variable_Info[i].resend_count>10) 
							|| (Bacnet_Refresh_Info.Read_Variable_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
						{
							//m_wait_detail.SetWindowTextW(_T("Read Variable Table Time Out!"));
							m_wait_persent.SetWindowTextW(_T("Timeout"));
							KillTimer(1);
							SetTimer(2,2000,NULL);
							goto endthis;
						}
						g_invoke_id = GetPrivateData(
							Bacnet_Refresh_Info.Read_Variable_Info[i].device_id,
							Bacnet_Refresh_Info.Read_Variable_Info[i].command,
							Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance,
							Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance,
							sizeof(Str_variable_point));
						if(g_invoke_id<0)	//如果没有获取到 就继续循环;
						{
							Sleep(50);
							continue;
						}
						Bacnet_Refresh_Info.Read_Variable_Info[i].has_resend_yes_or_no ++;
						Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
						Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;
						CString temp_cs_show;
						temp_cs_show.Format(_T("Read variable list from item %d to %d "),
							Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance,
							Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance);
						Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

						
						//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

						//TRACE(_T("Resend = %d ,Resend start = %d , Resend end = %d\r\n"),Bacnet_Refresh_Info.Read_Variable_Info[i].has_resend_yes_or_no,
						//	Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance,
						//	Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance);
				}
			}
		}
		if(cotinue_waite)
			goto endthis;
m_wait_progress.SetPos(100);
m_wait_persent.SetWindowTextW(_T("100%"));
		bac_read_all_results = true;
		bac_tstat_read_results = true;
		bac_input_read_results = true;
		bac_output_read_results = true;
		bac_variable_read_results = true;
		bac_program_read_results = true;
		bac_weekly_read_results = true;
		bac_annual_read_results = true;
		bac_controller_read_results = true;
		bac_screen_read_results = true;
		bac_monitor_read_results = true;
		bac_weeklycode_read_results = true;
		bac_annualcode_read_results = true;
		bac_alarmlog_read_results = true;
		bac_basic_setting_read_results = true;
		bac_customer_unit_read_results = true;
		bac_user_login_read_results = true;
		bac_graphic_label_read_results = true;
			m_wait_detail.SetWindowTextW(_T("Reading descriptors success!"));
			KillTimer(1);
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			SetTimer(2,2000,NULL);
			else
			SetTimer(2,100,NULL);
			}
endthis:
		break;
	case 2:
		{
			KillTimer(2);
			SetTimer(3,100,NULL);
			ShowWindow(SW_HIDE);
		}
		break;
	case 3:
		{
			KillTimer(3);
			if(m_wait_type == 1)
			{
			::PostMessage(MainFram_hwd,WM_DELETE_NEW_MESSAGE_DLG,0,0);
			}
			else
			{
			::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,0,0);
			}
			PostMessage(WM_CLOSE,NULL,NULL);
			//OnCancel();
		}
		break;
	case 5:
		{
			KillTimer(1);
			KillTimer(3);
			KillTimer(2);
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read Timeout!!"));
			//AfxMessageBox(_T("Read Timeout!!"));
			if(m_wait_type==1)
			{
				::PostMessage(MainFram_hwd,WM_DELETE_NEW_MESSAGE_DLG,0,0);
			}
			else
			{
				::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,0,0);
			}
			
			PostMessage(WM_CLOSE,NULL,NULL);
		}
		break;
	case  READ_MONITORDATA_TIME:
		{
			if(m_pos == 100)
			{
				KillTimer(READ_MONITORDATA_TIME);
				PostMessage(WM_CLOSE,NULL,NULL);
			}
			m_wait_persent.SetWindowTextW(PosString);
			m_wait_progress.SetPos(m_pos);
			UpdateData();
		}
		break;
	}
	 

	CDialogEx::OnTimer(nIDEvent);
}




void BacnetWait::PostNcDestroy()
{
	

	CDialogEx::PostNcDestroy();
	delete this;
}
