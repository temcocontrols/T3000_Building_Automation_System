// BacnetWait.cpp : implementation file
// Code by Fance

#include "stdafx.h"
#include "T3000.h"
#include "BacnetWait.h"
#include "afxdialogex.h"
#include "gloab_define.h"
#include "globle_function.h"
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
	


	// TODO:  Add extra initialization here
	if(m_wait_type == BAC_WAIT_READ_CONFIG_WRITE_DEVICE)	//1 is now write config
	{
		SetTimer(4,200,NULL);
	}
	else if((m_wait_type == BAC_WAIT_NORMAL_READ ) || (m_wait_type == BAC_WAIT_READ_DATA_WRITE_CONFIG))	//Read one of the list or read all list;
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
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();
}


void BacnetWait::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	//DestroyWindow();
	CDialogEx::OnCancel();
}


BOOL BacnetWait::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
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




	if(m_wait_type == BAC_WAIT_READ_CONFIG_WRITE_DEVICE)//如果是写config;将config文件的data写入device;
	{
		int write_total_count = BAC_INPUT_GROUP +
			BAC_OUTPUT_GROUP +	
			BAC_PROGRAM_GROUP +		
			BAC_VARIABLE_GROUP +
			BAC_CONTROLLER_GROUP +
			BAC_PROGRAMCODE_GROUP +
			BAC_SCREEN_GROUP +
			BAC_MONITOR_GROUP +
			BAC_WEEKLY_GROUP +
			BAC_ANNUAL_GROUP +
			BAC_WEEKLYCODE_GOUP +
			BAC_ANNUALCODE_GROUP;
		int write_all_step = 1000 / write_total_count;
		int write_success_count = 0;
		int write_pos = 0;

		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
				if(Write_Config_Info.Write_Input_Info[i].task_result == BAC_RESULTS_OK)
				{
					write_success_count ++;
					write_pos = write_pos +write_all_step;
				}
		}
		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if(Write_Config_Info.Write_Output_Info[i].task_result == BAC_RESULTS_OK)
			{
				write_success_count ++;
				write_pos = write_pos +write_all_step;
			}
		}
		for (int i=0;i<BAC_VARIABLE_GROUP;i++)
		{
			if(Write_Config_Info.Write_Variable_Info[i].task_result == BAC_RESULTS_OK)
			{
				write_success_count ++;
				write_pos = write_pos +write_all_step;
			}
		}
		for (int i=0;i<BAC_PROGRAM_GROUP;i++)
		{
			if(Write_Config_Info.Write_Program_Info[i].task_result == BAC_RESULTS_OK)
			{
				write_success_count ++;
				write_pos = write_pos +write_all_step;
			}
		}
		for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
		{
			if(Write_Config_Info.Write_Programcode_Info[i].task_result == BAC_RESULTS_OK)
			{
				write_success_count ++;
				write_pos = write_pos +write_all_step;
			}
		}
		for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
		{
			if(Write_Config_Info.Write_Controller_Info[i].task_result == BAC_RESULTS_OK)
			{
				write_success_count ++;
				write_pos = write_pos +write_all_step;
			}
		}
		for (int i=0;i<BAC_SCREEN_GROUP;i++)
		{
			if(Write_Config_Info.Write_Screen_Info[i].task_result == BAC_RESULTS_OK)
			{
				write_success_count ++;
				write_pos = write_pos +write_all_step;
			}
		}

		for (int i=0;i<BAC_MONITOR_GROUP;i++)
		{
			if(Write_Config_Info.Write_Monitor_Info[i].task_result == BAC_RESULTS_OK)
			{
				write_success_count ++;
				write_pos = write_pos +write_all_step;
			}
		}
		
		for (int i=0;i<BAC_WEEKLY_GROUP;i++)
		{
			if(Write_Config_Info.Write_Weekly_Info[i].task_result == BAC_RESULTS_OK)
			{
				write_success_count ++;
				write_pos = write_pos +write_all_step;
			}
		}

		for (int i=0;i<BAC_ANNUAL_GROUP;i++)
		{
			if(Write_Config_Info.Write_Annual_Info[i].task_result == BAC_RESULTS_OK)
			{
				write_success_count ++;
				write_pos = write_pos +write_all_step;
			}
		}

		for (int i=0;i<BAC_WEEKLYCODE_GOUP;i++)
		{
			if(Write_Config_Info.Write_Weeklycode_Info[i].task_result == BAC_RESULTS_OK)
			{
				write_success_count ++;
				write_pos = write_pos +write_all_step;
			}
		}

		for (int i=0;i<BAC_ANNUALCODE_GROUP;i++)
		{
			if(Write_Config_Info.Write_Annualcode_Info[i].task_result == BAC_RESULTS_OK)
			{
				write_success_count ++;
				write_pos = write_pos +write_all_step;
			}
		}

		write_pos = write_pos /10;
		if(write_pos>100)
			write_pos=100;
		m_wait_progress.SetPos(write_pos);
		temp_pos.Format(_T("%d%%"),write_pos);

	}
	else if(m_wait_type == BAC_WAIT_READ_DATA_WRITE_CONFIG)	//这里是 读取config 所要的 资料;
	{
		int read_config_total_count = BAC_INPUT_GROUP +
									  BAC_OUTPUT_GROUP +
									  BAC_PROGRAM_GROUP +
									  BAC_VARIABLE_GROUP +
									  BAC_CONTROLLER_GROUP +
									  BAC_PROGRAMCODE_GROUP +
									  BAC_SCREEN_GROUP +
									  BAC_WEEKLY_GROUP +
									  BAC_ANNUAL_GROUP +
									  BAC_MONITOR_GROUP +
									  BAC_WEEKLYCODE_GOUP +
									  BAC_ANNUALCODE_GROUP;//这里还有很多要加;
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

		for (int i=0;i<BAC_ANNUALCODE_GROUP;i++ )
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

		for (int i=0;i<BAC_CONTROLLER_GROUP ;i++ )
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

		for (int i=0;i<BAC_WEEKLY_GROUP ;i++ )
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

		for (int i=0;i<BAC_WEEKLYCODE_GOUP;i++ )
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


		for (int i=0;i<BAC_ANNUAL_GROUP ;i++ )
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
		if(read_config_pos>100)
			read_config_pos=100;
		m_wait_progress.SetPos(read_config_pos);
		temp_pos.Format(_T("%d%%"),read_config_pos);

	}
	else if(m_wait_type == BAC_WAIT_NORMAL_READ)//这里是点击的时候读，只要INput output var program 和time;
	{
		int total_count = BAC_INPUT_GROUP + 
			BAC_OUTPUT_GROUP +	
			BAC_PROGRAM_GROUP +		
			BAC_VARIABLE_GROUP +
			BAC_TIME_COMMAND_GROUP +
			BAC_WEEKLY_GROUP +
			BAC_ANNUAL_GROUP +
			BAC_CONTROLLER_GROUP +
			BAC_SCREEN_GROUP +
			BAC_MONITOR_GROUP +
			BAC_TSTAT_GROUP; 

		int all_step = 1000 / total_count;
		int input_step = 1000 / BAC_INPUT_GROUP;
		int output_step = 1000 / BAC_OUTPUT_GROUP;
		int program_step = 1000 / BAC_PROGRAM_GROUP;
		int variable_step = 1000 / BAC_VARIABLE_GROUP;
		int weekly_step  = 1000/ BAC_WEEKLY_GROUP;
		int annual_step = 1000 / BAC_ANNUAL_GROUP;
		int time_command_step = 1000 / BAC_TIME_COMMAND_GROUP;
		int controller_step = 1000 / BAC_CONTROLLER_GROUP;
		int screen_step = 1000 / BAC_SCREEN_GROUP;
		int monitor_step = 1000 /BAC_MONITOR_GROUP;
		int alarmlog_step = 1000 /BAC_ALARMLOG_GROUP;
		int tstat_step = 1000 / BAC_TSTAT_GROUP;
		tempcs = tempcs + tempcs2;
		int success_count =0;
		int pos=0;

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
		for (int i=0;i<BAC_TSTAT_GROUP;i++ )
		{
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Tstat_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos +all_step;
				}
			}
			else if(bac_read_which_list == BAC_READ_TSTAT_LIST)
			{
				if(Bacnet_Refresh_Info.Read_Tstat_Info[i].task_result == BAC_RESULTS_OK)
				{
					success_count ++;
					pos = pos + tstat_step;
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

		for (int i=0;i<BAC_WEEKLY_GROUP;i++ )
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

		for (int i=0;i<BAC_ANNUAL_GROUP;i++ )
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

		for (int i=0;i<BAC_CONTROLLER_GROUP ;i++ )
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
	//	TRACE(_T("%d\r\n"),pos);
		if(pos>100)
			pos=100;
		m_wait_progress.SetPos(pos);
		temp_pos.Format(_T("%d%%"),pos);
	}

	m_wait_persent.SetWindowTextW(temp_pos);

	UpdateData(FALSE);

	switch(nIDEvent)
	{
	case 1:
		{
			for (int i=0;i<BAC_TSTAT_GROUP;i++)
			{
				if((bac_read_which_list == BAC_READ_TSTAT_LIST) || (bac_read_which_list == BAC_READ_ALL_LIST))
				{
					if(Bacnet_Refresh_Info.Read_Tstat_Info[i].task_result == BAC_RESULTS_UNKONW)
					{
						if(Bacnet_Refresh_Info.Read_Tstat_Info[i].invoke_id > 0)
						{
							Bacnet_Refresh_Info.Read_Tstat_Info[i].timeout_count ++;
						}
						if(Bacnet_Refresh_Info.Read_Tstat_Info[i].timeout_count > 20)
						{
							Bacnet_Refresh_Info.Read_Tstat_Info[i].task_result = BAC_RESULTS_FAIL;
							Bacnet_Refresh_Info.Read_Tstat_Info[i].timeout_count = 0;
							continue;
						}
						m_wait_detail.SetWindowTextW(tempcs);
						cotinue_waite = true;
						break;
						//goto endthis;
					}
					else if(Bacnet_Refresh_Info.Read_Tstat_Info[i].task_result == BAC_RESULTS_FAIL)
					{
						cotinue_waite = true;
						Bacnet_Refresh_Info.Read_Tstat_Info[i].resend_count ++ ;
						//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
						if((Bacnet_Refresh_Info.Read_Tstat_Info[i].resend_count>RESEND_COUNT) 
							|| (Bacnet_Refresh_Info.Read_Tstat_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
						{
							m_wait_detail.SetWindowTextW(_T("Read Tstats table Time Out!"));
							KillTimer(1);
							SetTimer(2,2000,NULL);
							goto endthis;
						}

						g_invoke_id = GetPrivateData(
							Bacnet_Refresh_Info.Read_Tstat_Info[i].device_id,
							Bacnet_Refresh_Info.Read_Tstat_Info[i].command,
							Bacnet_Refresh_Info.Read_Tstat_Info[i].start_instance,
							Bacnet_Refresh_Info.Read_Tstat_Info[i].end_instance,
							sizeof(Str_TstatInfo_point));
						if(g_invoke_id<0)	//如果没有获取到 就继续循环;
						{
							Sleep(50);
							continue;
						}
						Bacnet_Refresh_Info.Read_Tstat_Info[i].has_resend_yes_or_no ++;
						Bacnet_Refresh_Info.Read_Tstat_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
						Bacnet_Refresh_Info.Read_Tstat_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

						CString temp_cs_show;
						temp_cs_show.Format(_T("Task ID = %d. Read Tstats list "),g_invoke_id);
						Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);


						TRACE(_T("TSTATS Resend start = %d , Resend end = %d\r\n"),
							Bacnet_Refresh_Info.Read_Tstat_Info[i].start_instance,
							Bacnet_Refresh_Info.Read_Tstat_Info[i].end_instance);


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
						m_wait_detail.SetWindowTextW(_T("Read Monitor table Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read Monitor list "),g_invoke_id);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);


					TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_Monitor_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance);
					

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
						m_wait_detail.SetWindowTextW(_T("Read Screen table Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read Screen list "),g_invoke_id);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);


					TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_Screen_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Screen_Info[i].end_instance);
					

				}
			}
		}

		for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
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
						m_wait_detail.SetWindowTextW(_T("Read Controller table Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read Controller list "),g_invoke_id);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);


					TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance);


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
						m_wait_detail.SetWindowTextW(_T("Read Device Time table Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read device time "),g_invoke_id);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);


					TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_Time_Command[i].start_instance,
						Bacnet_Refresh_Info.Read_Time_Command[i].end_instance);


				}
			}
		}


		for (int i=0;i<BAC_BASIC_SETTING_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_BASIC_SETTING_COMMAND) || (bac_read_which_list == BAC_READ_ALL_LIST))
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
						m_wait_detail.SetWindowTextW(_T("Read Basic Setting Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read basic setting "),g_invoke_id);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);


					TRACE(_T("Read basic setting Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_BasicSetting_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_BasicSetting_Info[i].end_instance);


				}
			}
		}



		for (int i=0;i<BAC_ANNUAL_GROUP;i++)
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
						m_wait_detail.SetWindowTextW(_T("Read Annual Table Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read annual routine from item %d to %d "),g_invoke_id,
						Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance);


				}
			}
		}

		for (int i=0;i<BAC_WEEKLY_GROUP;i++)
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
						m_wait_detail.SetWindowTextW(_T("Read Weekly Routine Table Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read weekly routine from item %d to %d "),g_invoke_id,
						Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance);


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
						m_wait_detail.SetWindowTextW(_T("Read input Table Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read program list from item %d to %d "),g_invoke_id,
						Bacnet_Refresh_Info.Read_Program_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Program_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);
					
					
					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_Program_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Program_Info[i].end_instance);


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
						m_wait_detail.SetWindowTextW(_T("Read Alarm Log Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read Alarm Log %d "),g_invoke_id,
						Bacnet_Refresh_Info.Read_AlarmLog_Info[i].start_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);


					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_AlarmLog_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_AlarmLog_Info[i].end_instance);


				}
			}
		}


		if(bac_read_which_list == BAC_READ_PROGRAMCODE_LIST)
		{
			if(Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].task_result == BAC_RESULTS_UNKONW)
			{
				if(Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].invoke_id > 0)
				{
					Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].timeout_count ++;
				}
				if(Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].timeout_count > 20)
				{
					Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].task_result = BAC_RESULTS_FAIL;
					Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].timeout_count = 0;
				}
				m_wait_detail.SetWindowTextW(tempcs);
				goto endthis;
			}
			else if(Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].task_result == BAC_RESULTS_FAIL)
			{
				Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].resend_count ++ ;
				cotinue_waite = true;
				//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
				if((Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].resend_count>RESEND_COUNT) 
					|| (Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].has_resend_yes_or_no > FAIL_RESEND_COUNT))
				{
					m_wait_detail.SetWindowTextW(_T("Read Program Code Time Out!"));
					KillTimer(1);
					SetTimer(2,2000,NULL);
					goto endthis;
				}
				g_invoke_id = GetPrivateData(
					Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].device_id,
					Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].command,
					Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].start_instance,
					Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].end_instance,
					m_Program_data.at(Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].start_instance).bytes + 10);
				if(g_invoke_id<0)	//如果没有获取到 就继续循环;
				{
					goto endthis;
				}
				Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].has_resend_yes_or_no ++;
				Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
				Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

				CString temp_cs_show;
				temp_cs_show.Format(_T("Task ID = %d. Read program code list from item %d to %d "),g_invoke_id,
					Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].start_instance,
					Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].end_instance);
				Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);


				//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

				TRACE(_T("Program code Resend start = %d , Resend end = %d\r\n"),
					Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].start_instance,
					Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].end_instance);


			}
		}
		else if((bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
			{
					if(Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result == BAC_RESULTS_UNKONW)
					{
						if(Bacnet_Refresh_Info.Read_Programcode_Info[i].invoke_id > 0)
						{
							Bacnet_Refresh_Info.Read_Programcode_Info[i].timeout_count ++;
						}
						if(Bacnet_Refresh_Info.Read_Programcode_Info[i].timeout_count > 20)
						{
							Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result = BAC_RESULTS_FAIL;
							Bacnet_Refresh_Info.Read_Programcode_Info[i].timeout_count = 0;
							continue;
						}
						m_wait_detail.SetWindowTextW(tempcs);
						cotinue_waite = true;
						break;
						//goto endthis;
					}
					else if(Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result == BAC_RESULTS_FAIL)
					{
						Bacnet_Refresh_Info.Read_Programcode_Info[i].resend_count ++ ;
						cotinue_waite = true;
						//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
						if((Bacnet_Refresh_Info.Read_Programcode_Info[i].resend_count>RESEND_COUNT) 
							|| (Bacnet_Refresh_Info.Read_Programcode_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
						{
							m_wait_detail.SetWindowTextW(_T("Read Program Code Time Out!"));
							KillTimer(1);
							SetTimer(2,2000,NULL);
							goto endthis;
						}
						g_invoke_id = GetPrivateData(
							Bacnet_Refresh_Info.Read_Programcode_Info[i].device_id,
							Bacnet_Refresh_Info.Read_Programcode_Info[i].command,
							Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance,
							Bacnet_Refresh_Info.Read_Programcode_Info[i].end_instance,
							m_Program_data.at(Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance).bytes + 10);
						if(g_invoke_id<0)	//如果没有获取到 就继续循环;
						{
							Sleep(50);
							continue;
						}
						Bacnet_Refresh_Info.Read_Programcode_Info[i].has_resend_yes_or_no ++;
						Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
						Bacnet_Refresh_Info.Read_Programcode_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

						CString temp_cs_show;
						temp_cs_show.Format(_T("Task ID = %d. Read program code list from item %d to %d "),g_invoke_id,
							Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance,
							Bacnet_Refresh_Info.Read_Programcode_Info[i].end_instance);
						Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);


						//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

						TRACE(_T("Program code Resend start = %d , Resend end = %d\r\n"),
							Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance,
							Bacnet_Refresh_Info.Read_Programcode_Info[i].end_instance);


					}
			}
		}

#if 0
		for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_PROGRAMCODE_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
			{
				if(Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Bacnet_Refresh_Info.Read_Programcode_Info[i].invoke_id > 0)
					{
						Bacnet_Refresh_Info.Read_Programcode_Info[i].timeout_count ++;
					}
					if(Bacnet_Refresh_Info.Read_Programcode_Info[i].timeout_count > 20)
					{
						Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result = BAC_RESULTS_FAIL;
						Bacnet_Refresh_Info.Read_Programcode_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Programcode_Info[i].resend_count ++ ;

					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Programcode_Info[i].resend_count>RESEND_COUNT) 
						|| (Bacnet_Refresh_Info.Read_Programcode_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						m_wait_detail.SetWindowTextW(_T("Read Program Code Time Out!"));
						KillTimer(1);
						SetTimer(2,2000,NULL);
						goto endthis;
					}
					g_invoke_id = GetPrivateData(
						Bacnet_Refresh_Info.Read_Programcode_Info[i].device_id,
						Bacnet_Refresh_Info.Read_Programcode_Info[i].command,
						Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Programcode_Info[i].end_instance,
						m_Program_data.at(Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance).bytes + 10);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Bacnet_Refresh_Info.Read_Programcode_Info[i].has_resend_yes_or_no ++;
					Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Bacnet_Refresh_Info.Read_Programcode_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Task ID = %d. Read program code list from item %d to %d "),g_invoke_id,
						Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Programcode_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);


					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					TRACE(_T("Program code Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Programcode_Info[i].end_instance);


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
					m_wait_detail.SetWindowTextW(_T("Read Weekly Schedule Time Out!"));
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
				temp_cs_show.Format(_T("Task ID = %d. Read weekly schedule time list from item %d to %d "),g_invoke_id,
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].start_instance,
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].end_instance);
				Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);


				//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

				TRACE(_T("Program code Resend start = %d , Resend end = %d\r\n"),
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].start_instance,
					Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].end_instance);


			}
		}
		else if((bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			for (int i=0;i<BAC_WEEKLYCODE_GOUP;i++)
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
						m_wait_detail.SetWindowTextW(_T("Read weekly schedule Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read weekly shcedule time list from item %d to %d "),g_invoke_id,
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);


					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					TRACE(_T("Weekly schedule time Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Weeklycode_Info[i].end_instance);


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
					m_wait_detail.SetWindowTextW(_T("Read annual day Time Out!"));
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
				temp_cs_show.Format(_T("Task ID = %d. Read annual day time list from item %d to %d "),g_invoke_id,
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].start_instance,
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].end_instance);
				Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);


				//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

				TRACE(_T("Program code Resend start = %d , Resend end = %d\r\n"),
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].start_instance,
					Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].end_instance);


			}
		}
		else if((bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			for (int i=0;i<BAC_ANNUALCODE_GROUP;i++)
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
						m_wait_detail.SetWindowTextW(_T("Read annual day Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read annual day time list from item %d to %d "),g_invoke_id,
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);


					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					TRACE(_T("Weekly schedule time Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Annualcode_Info[i].end_instance);


				}
			}
		}

		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			 if((bac_read_which_list == BAC_READ_INPUT_LIST)|| (bac_read_which_list == BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
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
						m_wait_detail.SetWindowTextW(_T("Read input Table Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read input list from item %d to %d "),g_invoke_id,
						Bacnet_Refresh_Info.Read_Input_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Input_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);
					
					
					
					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_Input_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Input_Info[i].end_instance);

				}
			}

		}

		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_OUTPUT_LIST)|| (bac_read_which_list == BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
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
						m_wait_detail.SetWindowTextW(_T("Read input Table Time Out!"));
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
					temp_cs_show.Format(_T("Task ID = %d. Read output list from item %d to %d "),g_invoke_id,
						Bacnet_Refresh_Info.Read_Output_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Output_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);
					
					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

					TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
						Bacnet_Refresh_Info.Read_Output_Info[i].start_instance,
						Bacnet_Refresh_Info.Read_Output_Info[i].end_instance);

				}
			}

		}

		for (int i=0;i<BAC_VARIABLE_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_VARIABLE_LIST) || (bac_read_which_list == BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
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
							m_wait_detail.SetWindowTextW(_T("Read Variable Table Time Out!"));
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
						temp_cs_show.Format(_T("Task ID = %d. Read variable list from item %d to %d "),g_invoke_id,
							Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance,
							Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance);
						Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

						
						//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);

						TRACE(_T("Resend start = %d , Resend end = %d\r\n"),
							Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance,
							Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance);
				}
			}
		}
		if(cotinue_waite)
			goto endthis;
m_wait_progress.SetPos(100);
m_wait_persent.SetWindowTextW(_T("100%"));
		bac_read_all_results = true;
		bac_monitor_read_results = true;
		bac_input_read_results = true;
		bac_output_read_results = true;
		bac_variable_read_results = true;
		bac_program_read_results = true;
		bac_weekly_read_results = true;
		bac_annual_read_results = true;
		bac_controller_read_results = true;
		bac_screen_read_results = true;
		bac_monitor_read_results = true;
		bac_programcode_read_results = true;
		bac_weeklycode_read_results = true;
		bac_annualcode_read_results = true;
		bac_alarmlog_read_results = true;
		bac_basic_setting_read_results = true;
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
	case 4:
		{
			for (int i=0;i<BAC_INPUT_GROUP;i++)
			{
					if(Write_Config_Info.Write_Input_Info[i].task_result == BAC_RESULTS_UNKONW)
					{
						if(Write_Config_Info.Write_Input_Info[i].invoke_id > 0)
						{
							Write_Config_Info.Write_Input_Info[i].timeout_count ++;
						}
						if(Write_Config_Info.Write_Input_Info[i].timeout_count > 20)
						{
							Write_Config_Info.Write_Input_Info[i].task_result = BAC_RESULTS_FAIL;
							Write_Config_Info.Write_Input_Info[i].timeout_count = 0;
							continue;
						}
						m_wait_detail.SetWindowTextW(tempcs);
						write_cotinue_waite = true;
						break;

						//if(Write_Config_Info.Write_Input_Info[i].invoke_id > 0)
						//{
						//	Write_Config_Info.Write_Input_Info[i].timeout_count ++;
						//}

						//if((Write_Config_Info.Write_Input_Info[i].timeout_count > 20) ||
						//	((Write_Config_Info.Write_Input_Info[i].invoke_id <=0) && (Write_Config_Info.Write_Input_Info[i].has_resend_yes_or_no>0) ))
						//{
						//	Write_Config_Info.Write_Input_Info[i].task_result = BAC_RESULTS_FAIL;
						//	Write_Config_Info.Write_Input_Info[i].timeout_count = 0;
						//	//continue;
						//}
						//m_wait_detail.SetWindowTextW(tempcs);
						//goto write_endthis;
					}
					else if(Write_Config_Info.Write_Input_Info[i].task_result == BAC_RESULTS_FAIL)
					{
						Write_Config_Info.Write_Input_Info[i].resend_count ++ ;
						write_cotinue_waite = true;
						//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
						if((Write_Config_Info.Write_Input_Info[i].resend_count>RESEND_COUNT) || (Write_Config_Info.Write_Input_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
						{
							m_wait_detail.SetWindowTextW(_T("Write input Table Time Out!"));
							KillTimer(4);
							SetTimer(2,2000,NULL);
							goto write_endthis;
						}
						//if(Write_Config_Info.Write_Input_Info[i].device_id ==0)
						//	Sleep(1);
						//TRACE(_T("INPUT ID = %d ,command =%d\r\n"),Write_Config_Info.Write_Input_Info[i].device_id,Write_Config_Info.Write_Input_Info[i].command);
						g_invoke_id = WritePrivateData(	Write_Config_Info.Write_Input_Info[i].device_id,Write_Config_Info.Write_Input_Info[i].command,
														Write_Config_Info.Write_Input_Info[i].start_instance,Write_Config_Info.Write_Input_Info[i].end_instance);
						if(g_invoke_id<0)	//如果没有获取到 就继续循环;
						{
							Sleep(50);
							continue;
						}
						Write_Config_Info.Write_Input_Info[i].has_resend_yes_or_no ++;
						Write_Config_Info.Write_Input_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
						Write_Config_Info.Write_Input_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

						CString temp_cs_show;
						temp_cs_show.Format(_T("Task ID = %d. Write input list from item %d to %d "),g_invoke_id,
							Write_Config_Info.Write_Input_Info[i].start_instance,
							Write_Config_Info.Write_Input_Info[i].end_instance);
						Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs_show);

						//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
						TRACE(_T("Write input list Resend start = %d , Resend end = %d\r\n"),
							Write_Config_Info.Write_Input_Info[i].start_instance,
							Write_Config_Info.Write_Input_Info[i].end_instance);
					}
			}

			for (int i=0;i<BAC_OUTPUT_GROUP;i++)
			{
				if(Write_Config_Info.Write_Output_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Write_Config_Info.Write_Output_Info[i].invoke_id > 0)
					{
						Write_Config_Info.Write_Output_Info[i].timeout_count ++;
					}
					if(Write_Config_Info.Write_Output_Info[i].timeout_count > 20)
					{
						Write_Config_Info.Write_Output_Info[i].task_result = BAC_RESULTS_FAIL;
						Write_Config_Info.Write_Output_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					write_cotinue_waite = true;
					break;
					//if(Write_Config_Info.Write_Output_Info[i].invoke_id > 0)
					//{
					//	Write_Config_Info.Write_Output_Info[i].timeout_count ++;
					//}
					//if((Write_Config_Info.Write_Output_Info[i].timeout_count > 20) ||
					//	((Write_Config_Info.Write_Output_Info[i].invoke_id <=0) &&((Write_Config_Info.Write_Output_Info[i].has_resend_yes_or_no >0)) ))
					//{
					//	Write_Config_Info.Write_Output_Info[i].task_result = BAC_RESULTS_FAIL;
					//	Write_Config_Info.Write_Output_Info[i].timeout_count = 0;
					//	//continue;
					//}
					//m_wait_detail.SetWindowTextW(tempcs);
					//goto write_endthis;
				}
				else if(Write_Config_Info.Write_Output_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					write_cotinue_waite = true;
					Write_Config_Info.Write_Output_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Write_Config_Info.Write_Output_Info[i].resend_count>RESEND_COUNT) 
						|| (Write_Config_Info.Write_Output_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						m_wait_detail.SetWindowTextW(_T("Write output Table Time Out!"));
						KillTimer(4);
						SetTimer(2,2000,NULL);
						goto write_endthis;
					}
					//TRACE(_T("OUTPUT ID = %d ,command =%d\r\n"),Write_Config_Info.Write_Output_Info[i].device_id,Write_Config_Info.Write_Output_Info[i].command);
					g_invoke_id = WritePrivateData(
						Write_Config_Info.Write_Output_Info[i].device_id,
						Write_Config_Info.Write_Output_Info[i].command,
						Write_Config_Info.Write_Output_Info[i].start_instance,
						Write_Config_Info.Write_Output_Info[i].end_instance);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Write_Config_Info.Write_Output_Info[i].has_resend_yes_or_no ++;
					Write_Config_Info.Write_Output_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Write_Config_Info.Write_Output_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Task ID = %d. Write output list from item %d to %d "),g_invoke_id,
						Write_Config_Info.Write_Output_Info[i].start_instance,
						Write_Config_Info.Write_Output_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
					TRACE(_T(" Write output list Resend start = %d , Resend end = %d\r\n"),
						Write_Config_Info.Write_Output_Info[i].start_instance,
						Write_Config_Info.Write_Output_Info[i].end_instance);
				}
			}

			for (int i=0;i<BAC_VARIABLE_GROUP;i++)
			{
				if(Write_Config_Info.Write_Variable_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Write_Config_Info.Write_Variable_Info[i].invoke_id > 0)
					{
						Write_Config_Info.Write_Variable_Info[i].timeout_count ++;
					}
					if(Write_Config_Info.Write_Variable_Info[i].timeout_count > 20)
					{
						Write_Config_Info.Write_Variable_Info[i].task_result = BAC_RESULTS_FAIL;
						Write_Config_Info.Write_Variable_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					write_cotinue_waite = true;
					break;
					//if(Write_Config_Info.Write_Variable_Info[i].invoke_id > 0)
					//{
					//	Write_Config_Info.Write_Variable_Info[i].timeout_count ++;
					//}
					//if((Write_Config_Info.Write_Variable_Info[i].timeout_count > 20) ||
					//	((Write_Config_Info.Write_Variable_Info[i].invoke_id <=0) &&((Write_Config_Info.Write_Variable_Info[i].has_resend_yes_or_no >0))))
					//{
					//	Write_Config_Info.Write_Variable_Info[i].task_result = BAC_RESULTS_FAIL;
					//	Write_Config_Info.Write_Variable_Info[i].timeout_count = 0;
					//	//continue;
					//}
					//m_wait_detail.SetWindowTextW(tempcs);
					//goto write_endthis;
				}
				else if(Write_Config_Info.Write_Variable_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					write_cotinue_waite = true;
					Write_Config_Info.Write_Input_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Write_Config_Info.Write_Variable_Info[i].resend_count>RESEND_COUNT) 
						|| (Write_Config_Info.Write_Variable_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						m_wait_detail.SetWindowTextW(_T("Write variable Table Time Out!"));
						KillTimer(4);
						SetTimer(2,2000,NULL);
						goto write_endthis;
					}
					g_invoke_id = WritePrivateData(
						Write_Config_Info.Write_Variable_Info[i].device_id,
						Write_Config_Info.Write_Variable_Info[i].command,
						Write_Config_Info.Write_Variable_Info[i].start_instance,
						Write_Config_Info.Write_Variable_Info[i].end_instance);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Write_Config_Info.Write_Variable_Info[i].has_resend_yes_or_no ++;
					Write_Config_Info.Write_Variable_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Write_Config_Info.Write_Variable_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Task ID = %d. Write variable list from item %d to %d "),g_invoke_id,
						Write_Config_Info.Write_Variable_Info[i].start_instance,
						Write_Config_Info.Write_Variable_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
					TRACE(_T("Write variable list Resend start = %d , Resend end = %d\r\n"),
						Write_Config_Info.Write_Variable_Info[i].start_instance,
						Write_Config_Info.Write_Variable_Info[i].end_instance);
				}
			}

			for (int i=0;i<BAC_PROGRAM_GROUP;i++)
			{
				if(Write_Config_Info.Write_Program_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Write_Config_Info.Write_Program_Info[i].invoke_id > 0)
					{
						Write_Config_Info.Write_Program_Info[i].timeout_count ++;
					}
					if(Write_Config_Info.Write_Program_Info[i].timeout_count > 20)
					{
						Write_Config_Info.Write_Program_Info[i].task_result = BAC_RESULTS_FAIL;
						Write_Config_Info.Write_Program_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					write_cotinue_waite = true;
					break;
					//if(Write_Config_Info.Write_Program_Info[i].invoke_id > 0)
					//{
					//	Write_Config_Info.Write_Program_Info[i].timeout_count ++;
					//}
					//if((Write_Config_Info.Write_Program_Info[i].timeout_count > 20) ||
					//	((Write_Config_Info.Write_Program_Info[i].invoke_id <=0) && ((Write_Config_Info.Write_Program_Info[i].has_resend_yes_or_no>0))))
					//{
					//	Write_Config_Info.Write_Program_Info[i].task_result = BAC_RESULTS_FAIL;
					//	Write_Config_Info.Write_Program_Info[i].timeout_count = 0;
					//	//continue;
					//}
					//m_wait_detail.SetWindowTextW(tempcs);
					//goto write_endthis;
				}
				else if(Write_Config_Info.Write_Program_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					write_cotinue_waite = true;
					Write_Config_Info.Write_Program_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Write_Config_Info.Write_Program_Info[i].resend_count>RESEND_COUNT) 
						|| (Write_Config_Info.Write_Program_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						m_wait_detail.SetWindowTextW(_T("Write program Table Time Out!"));
						KillTimer(4);
						SetTimer(2,2000,NULL);
						goto write_endthis;
					}
					g_invoke_id = WritePrivateData(
						Write_Config_Info.Write_Program_Info[i].device_id,
						Write_Config_Info.Write_Program_Info[i].command,
						Write_Config_Info.Write_Program_Info[i].start_instance,
						Write_Config_Info.Write_Program_Info[i].end_instance);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Write_Config_Info.Write_Program_Info[i].has_resend_yes_or_no ++;
					Write_Config_Info.Write_Program_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Write_Config_Info.Write_Program_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Task ID = %d. Write program list from item %d to %d "),g_invoke_id,
						Write_Config_Info.Write_Program_Info[i].start_instance,
						Write_Config_Info.Write_Program_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
					TRACE(_T("Write program Table Resend start = %d , Resend end = %d\r\n"),
						Write_Config_Info.Write_Program_Info[i].start_instance,
						Write_Config_Info.Write_Program_Info[i].end_instance);
				}
			}



			for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
			{
				if(Write_Config_Info.Write_Programcode_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Write_Config_Info.Write_Programcode_Info[i].invoke_id > 0)
					{
						Write_Config_Info.Write_Programcode_Info[i].timeout_count ++;
					}
					if(Write_Config_Info.Write_Programcode_Info[i].timeout_count > 20)
					{
						Write_Config_Info.Write_Programcode_Info[i].task_result = BAC_RESULTS_FAIL;
						Write_Config_Info.Write_Programcode_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					write_cotinue_waite = true;
					break;
					//if(Write_Config_Info.Write_Programcode_Info[i].invoke_id > 0)
					//{
					//	Write_Config_Info.Write_Programcode_Info[i].timeout_count ++;
					//}
					//if((Write_Config_Info.Write_Programcode_Info[i].timeout_count > 20) ||
					//	((Write_Config_Info.Write_Programcode_Info[i].invoke_id <=0) &&((Write_Config_Info.Write_Programcode_Info[i].has_resend_yes_or_no >0)) ))
					//{
					//	Write_Config_Info.Write_Programcode_Info[i].task_result = BAC_RESULTS_FAIL;
					//	Write_Config_Info.Write_Programcode_Info[i].timeout_count = 0;
					//	//continue;
					//}
					//m_wait_detail.SetWindowTextW(tempcs);
					//goto write_endthis;
				}
				else if(Write_Config_Info.Write_Programcode_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					write_cotinue_waite = true;
					Write_Config_Info.Write_Programcode_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Write_Config_Info.Write_Programcode_Info[i].resend_count>RESEND_COUNT) 
						|| (Write_Config_Info.Write_Programcode_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						m_wait_detail.SetWindowTextW(_T("Write program code Time Out!"));
						KillTimer(4);
						SetTimer(2,2000,NULL);
						goto write_endthis;
					}
					g_invoke_id = WritePrivateData(
						Write_Config_Info.Write_Programcode_Info[i].device_id,
						Write_Config_Info.Write_Programcode_Info[i].command,
						Write_Config_Info.Write_Programcode_Info[i].start_instance,
						Write_Config_Info.Write_Programcode_Info[i].end_instance);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Write_Config_Info.Write_Programcode_Info[i].has_resend_yes_or_no ++;
					Write_Config_Info.Write_Programcode_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Write_Config_Info.Write_Programcode_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Task ID = %d. Write program code from item %d to %d "),g_invoke_id,
						Write_Config_Info.Write_Programcode_Info[i].start_instance,
						Write_Config_Info.Write_Programcode_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
					TRACE(_T("Program code Resend start = %d , Resend end = %d\r\n"),
						Write_Config_Info.Write_Programcode_Info[i].start_instance,
						Write_Config_Info.Write_Programcode_Info[i].end_instance);
				}
			}



			for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
			{
				if(Write_Config_Info.Write_Controller_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Write_Config_Info.Write_Controller_Info[i].invoke_id > 0)
					{
						Write_Config_Info.Write_Controller_Info[i].timeout_count ++;
					}
					if(Write_Config_Info.Write_Controller_Info[i].timeout_count > 20)
					{
						Write_Config_Info.Write_Controller_Info[i].task_result = BAC_RESULTS_FAIL;
						Write_Config_Info.Write_Controller_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					write_cotinue_waite = true;
					break;
					//if(Write_Config_Info.Write_Controller_Info[i].invoke_id > 0)
					//{
					//	Write_Config_Info.Write_Controller_Info[i].timeout_count ++;
					//}
					//if((Write_Config_Info.Write_Controller_Info[i].timeout_count > 20) ||
					//	((Write_Config_Info.Write_Controller_Info[i].invoke_id <=0)&& ((Write_Config_Info.Write_Controller_Info[i].has_resend_yes_or_no>0)) ))
					//{
					//	Write_Config_Info.Write_Controller_Info[i].task_result = BAC_RESULTS_FAIL;
					//	Write_Config_Info.Write_Controller_Info[i].timeout_count = 0;
					//	//continue;
					//}
					//m_wait_detail.SetWindowTextW(tempcs);
					//goto write_endthis;
				}
				else if(Write_Config_Info.Write_Controller_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					write_cotinue_waite = true;
					Write_Config_Info.Write_Controller_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Write_Config_Info.Write_Controller_Info[i].resend_count>RESEND_COUNT) 
						|| (Write_Config_Info.Write_Controller_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						m_wait_detail.SetWindowTextW(_T("Write controller Table Time Out!"));
						KillTimer(4);
						SetTimer(2,2000,NULL);
						goto write_endthis;
					}
					g_invoke_id = WritePrivateData(
						Write_Config_Info.Write_Controller_Info[i].device_id,
						Write_Config_Info.Write_Controller_Info[i].command,
						Write_Config_Info.Write_Controller_Info[i].start_instance,
						Write_Config_Info.Write_Controller_Info[i].end_instance);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Write_Config_Info.Write_Controller_Info[i].has_resend_yes_or_no ++;
					Write_Config_Info.Write_Controller_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Write_Config_Info.Write_Controller_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Task ID = %d. Write controller list from item %d to %d "),g_invoke_id,
						Write_Config_Info.Write_Controller_Info[i].start_instance,
						Write_Config_Info.Write_Controller_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
					TRACE(_T("Write controller list Resend start = %d , Resend end = %d\r\n"),
						Write_Config_Info.Write_Controller_Info[i].start_instance,
						Write_Config_Info.Write_Controller_Info[i].end_instance);
				}
			}


			for (int i=0;i<BAC_SCREEN_GROUP;i++)
			{
				if(Write_Config_Info.Write_Screen_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Write_Config_Info.Write_Screen_Info[i].invoke_id > 0)
					{
						Write_Config_Info.Write_Screen_Info[i].timeout_count ++;
					}
					if(Write_Config_Info.Write_Screen_Info[i].timeout_count > 20)
					{
						Write_Config_Info.Write_Screen_Info[i].task_result = BAC_RESULTS_FAIL;
						Write_Config_Info.Write_Screen_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					write_cotinue_waite = true;
					break;
					//if(Write_Config_Info.Write_Screen_Info[i].invoke_id > 0)
					//{
					//	Write_Config_Info.Write_Screen_Info[i].timeout_count ++;
					//}
					//if((Write_Config_Info.Write_Screen_Info[i].timeout_count > 20) ||
					//	((Write_Config_Info.Write_Screen_Info[i].invoke_id <=0) &&((Write_Config_Info.Write_Screen_Info[i].has_resend_yes_or_no >0)) ))
					//{
					//	Write_Config_Info.Write_Screen_Info[i].task_result = BAC_RESULTS_FAIL;
					//	Write_Config_Info.Write_Screen_Info[i].timeout_count = 0;
					//	//continue;
					//}
					//m_wait_detail.SetWindowTextW(tempcs);
					//goto write_endthis;
				}
				else if(Write_Config_Info.Write_Screen_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					write_cotinue_waite = true;
					Write_Config_Info.Write_Screen_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Write_Config_Info.Write_Screen_Info[i].resend_count>RESEND_COUNT) 
						|| (Write_Config_Info.Write_Screen_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						m_wait_detail.SetWindowTextW(_T("Write Screen Table Time Out!"));
						KillTimer(4);
						SetTimer(2,2000,NULL);
						goto write_endthis;
					}
					//TRACE(_T("OUTPUT ID = %d ,command =%d\r\n"),Write_Config_Info.Write_Output_Info[i].device_id,Write_Config_Info.Write_Output_Info[i].command);
					g_invoke_id = WritePrivateData(
						Write_Config_Info.Write_Screen_Info[i].device_id,
						Write_Config_Info.Write_Screen_Info[i].command,
						Write_Config_Info.Write_Screen_Info[i].start_instance,
						Write_Config_Info.Write_Screen_Info[i].end_instance);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Write_Config_Info.Write_Screen_Info[i].has_resend_yes_or_no ++;
					Write_Config_Info.Write_Screen_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Write_Config_Info.Write_Screen_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Task ID = %d. Write Screen list from item %d to %d "),g_invoke_id,
						Write_Config_Info.Write_Screen_Info[i].start_instance,
						Write_Config_Info.Write_Screen_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
					TRACE(_T(" Write screen list Resend start = %d , Resend end = %d\r\n"),
						Write_Config_Info.Write_Screen_Info[i].start_instance,
						Write_Config_Info.Write_Screen_Info[i].end_instance);
				}
			}

			for (int i=0;i<BAC_MONITOR_GROUP;i++)
			{
				if(Write_Config_Info.Write_Monitor_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Write_Config_Info.Write_Monitor_Info[i].invoke_id > 0)
					{
						Write_Config_Info.Write_Monitor_Info[i].timeout_count ++;
					}
					if(Write_Config_Info.Write_Monitor_Info[i].timeout_count > 20)
					{
						Write_Config_Info.Write_Monitor_Info[i].task_result = BAC_RESULTS_FAIL;
						Write_Config_Info.Write_Monitor_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					write_cotinue_waite = true;
					break;
					//if(Write_Config_Info.Write_Monitor_Info[i].invoke_id > 0)
					//{
					//	Write_Config_Info.Write_Monitor_Info[i].timeout_count ++;
					//}
					//if((Write_Config_Info.Write_Monitor_Info[i].timeout_count > 20) ||
					//	((Write_Config_Info.Write_Monitor_Info[i].invoke_id <=0) &&((Write_Config_Info.Write_Monitor_Info[i].has_resend_yes_or_no >0)) ))
					//{
					//	Write_Config_Info.Write_Monitor_Info[i].task_result = BAC_RESULTS_FAIL;
					//	Write_Config_Info.Write_Monitor_Info[i].timeout_count = 0;
					//	//continue;
					//}
					//m_wait_detail.SetWindowTextW(tempcs);
					//goto write_endthis;
				}
				else if(Write_Config_Info.Write_Monitor_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					write_cotinue_waite = true;
					Write_Config_Info.Write_Monitor_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Write_Config_Info.Write_Monitor_Info[i].resend_count>RESEND_COUNT) 
						|| (Write_Config_Info.Write_Monitor_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						m_wait_detail.SetWindowTextW(_T("Write Monitor Table Time Out!"));
						KillTimer(4);
						SetTimer(2,2000,NULL);
						goto write_endthis;
					}
					//TRACE(_T("OUTPUT ID = %d ,command =%d\r\n"),Write_Config_Info.Write_Output_Info[i].device_id,Write_Config_Info.Write_Output_Info[i].command);
					g_invoke_id = WritePrivateData(
						Write_Config_Info.Write_Monitor_Info[i].device_id,
						Write_Config_Info.Write_Monitor_Info[i].command,
						Write_Config_Info.Write_Monitor_Info[i].start_instance,
						Write_Config_Info.Write_Monitor_Info[i].end_instance);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Write_Config_Info.Write_Monitor_Info[i].has_resend_yes_or_no ++;
					Write_Config_Info.Write_Monitor_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Write_Config_Info.Write_Monitor_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Task ID = %d. Write Monitor list from item %d to %d "),g_invoke_id,
						Write_Config_Info.Write_Screen_Info[i].start_instance,
						Write_Config_Info.Write_Screen_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
					TRACE(_T(" Write monitor list Resend start = %d , Resend end = %d\r\n"),
						Write_Config_Info.Write_Monitor_Info[i].start_instance,
						Write_Config_Info.Write_Monitor_Info[i].end_instance);
				}
			}

			for (int i=0;i<BAC_WEEKLY_GROUP;i++)
			{
				if(Write_Config_Info.Write_Weekly_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Write_Config_Info.Write_Weekly_Info[i].invoke_id > 0)
					{
						Write_Config_Info.Write_Weekly_Info[i].timeout_count ++;
					}
					if(Write_Config_Info.Write_Weekly_Info[i].timeout_count > 20)
					{
						Write_Config_Info.Write_Weekly_Info[i].task_result = BAC_RESULTS_FAIL;
						Write_Config_Info.Write_Weekly_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					write_cotinue_waite = true;
					break;
				}
				else if(Write_Config_Info.Write_Weekly_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					write_cotinue_waite = true;
					Write_Config_Info.Write_Weekly_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Write_Config_Info.Write_Weekly_Info[i].resend_count>RESEND_COUNT) 
						|| (Write_Config_Info.Write_Weekly_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						m_wait_detail.SetWindowTextW(_T("Write Weekly routines Table Time Out!"));
						KillTimer(4);
						SetTimer(2,2000,NULL);
						goto write_endthis;
					}
					//TRACE(_T("OUTPUT ID = %d ,command =%d\r\n"),Write_Config_Info.Write_Output_Info[i].device_id,Write_Config_Info.Write_Output_Info[i].command);
					g_invoke_id = WritePrivateData(
						Write_Config_Info.Write_Weekly_Info[i].device_id,
						Write_Config_Info.Write_Weekly_Info[i].command,
						Write_Config_Info.Write_Weekly_Info[i].start_instance,
						Write_Config_Info.Write_Weekly_Info[i].end_instance);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Write_Config_Info.Write_Weekly_Info[i].has_resend_yes_or_no ++;
					Write_Config_Info.Write_Weekly_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Write_Config_Info.Write_Weekly_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Task ID = %d. Write weekly routines list from item %d to %d "),g_invoke_id,
						Write_Config_Info.Write_Weekly_Info[i].start_instance,
						Write_Config_Info.Write_Weekly_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
					TRACE(_T(" Write weekly routines Resend start = %d , Resend end = %d\r\n"),
						Write_Config_Info.Write_Weekly_Info[i].start_instance,
						Write_Config_Info.Write_Weekly_Info[i].end_instance);
				}
			}

			for (int i=0;i<BAC_WEEKLYCODE_GOUP;i++)
			{
				if(Write_Config_Info.Write_Weeklycode_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Write_Config_Info.Write_Weeklycode_Info[i].invoke_id > 0)
					{
						Write_Config_Info.Write_Weeklycode_Info[i].timeout_count ++;
					}
					if(Write_Config_Info.Write_Weeklycode_Info[i].timeout_count > 20)
					{
						Write_Config_Info.Write_Weeklycode_Info[i].task_result = BAC_RESULTS_FAIL;
						Write_Config_Info.Write_Weeklycode_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					write_cotinue_waite = true;
					break;
				}
				else if(Write_Config_Info.Write_Weeklycode_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					write_cotinue_waite = true;
					Write_Config_Info.Write_Weeklycode_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Write_Config_Info.Write_Weeklycode_Info[i].resend_count>RESEND_COUNT) 
						|| (Write_Config_Info.Write_Weeklycode_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						m_wait_detail.SetWindowTextW(_T("Write weekly shcedule time Time Out!"));
						KillTimer(4);
						SetTimer(2,2000,NULL);
						goto write_endthis;
					}
					g_invoke_id = WritePrivateData(
						Write_Config_Info.Write_Weeklycode_Info[i].device_id,
						Write_Config_Info.Write_Weeklycode_Info[i].command,
						Write_Config_Info.Write_Weeklycode_Info[i].start_instance,
						Write_Config_Info.Write_Weeklycode_Info[i].end_instance);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Write_Config_Info.Write_Weeklycode_Info[i].has_resend_yes_or_no ++;
					Write_Config_Info.Write_Weeklycode_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Write_Config_Info.Write_Weeklycode_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Task ID = %d. Write weekly schedule time from item %d to %d "),g_invoke_id,
						Write_Config_Info.Write_Weeklycode_Info[i].start_instance,
						Write_Config_Info.Write_Weeklycode_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
					TRACE(_T("weekly schedule time Resend start = %d , Resend end = %d\r\n"),
						Write_Config_Info.Write_Weeklycode_Info[i].start_instance,
						Write_Config_Info.Write_Weeklycode_Info[i].end_instance);
				}
			}

			for (int i=0;i<BAC_ANNUALCODE_GROUP;i++)
			{
				if(Write_Config_Info.Write_Annualcode_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Write_Config_Info.Write_Annualcode_Info[i].invoke_id > 0)
					{
						Write_Config_Info.Write_Annualcode_Info[i].timeout_count ++;
					}
					if(Write_Config_Info.Write_Annualcode_Info[i].timeout_count > 20)
					{
						Write_Config_Info.Write_Annualcode_Info[i].task_result = BAC_RESULTS_FAIL;
						Write_Config_Info.Write_Annualcode_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					write_cotinue_waite = true;
					break;
				}
				else if(Write_Config_Info.Write_Annualcode_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					write_cotinue_waite = true;
					Write_Config_Info.Write_Annualcode_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Write_Config_Info.Write_Annualcode_Info[i].resend_count>RESEND_COUNT) 
						|| (Write_Config_Info.Write_Annualcode_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						m_wait_detail.SetWindowTextW(_T("Write weekly shcedule time Time Out!"));
						KillTimer(4);
						SetTimer(2,2000,NULL);
						goto write_endthis;
					}
					g_invoke_id = WritePrivateData(
						Write_Config_Info.Write_Annualcode_Info[i].device_id,
						Write_Config_Info.Write_Annualcode_Info[i].command,
						Write_Config_Info.Write_Annualcode_Info[i].start_instance,
						Write_Config_Info.Write_Annualcode_Info[i].end_instance);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Write_Config_Info.Write_Annualcode_Info[i].has_resend_yes_or_no ++;
					Write_Config_Info.Write_Annualcode_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Write_Config_Info.Write_Annualcode_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Task ID = %d. Write annual day time from item %d to %d "),g_invoke_id,
						Write_Config_Info.Write_Annualcode_Info[i].start_instance,
						Write_Config_Info.Write_Annualcode_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
					TRACE(_T("annual day time Resend start = %d , Resend end = %d\r\n"),
						Write_Config_Info.Write_Annualcode_Info[i].start_instance,
						Write_Config_Info.Write_Annualcode_Info[i].end_instance);
				}
			}

			for (int i=0;i<BAC_ANNUAL_GROUP;i++)
			{
				if(Write_Config_Info.Write_Annual_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					if(Write_Config_Info.Write_Annual_Info[i].invoke_id > 0)
					{
						Write_Config_Info.Write_Annual_Info[i].timeout_count ++;
					}
					if(Write_Config_Info.Write_Annual_Info[i].timeout_count > 20)
					{
						Write_Config_Info.Write_Annual_Info[i].task_result = BAC_RESULTS_FAIL;
						Write_Config_Info.Write_Annual_Info[i].timeout_count = 0;
						continue;
					}
					m_wait_detail.SetWindowTextW(tempcs);
					write_cotinue_waite = true;
					break;
				}
				else if(Write_Config_Info.Write_Annual_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					write_cotinue_waite = true;
					Write_Config_Info.Write_Annual_Info[i].resend_count ++ ;
					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Write_Config_Info.Write_Annual_Info[i].resend_count>RESEND_COUNT) 
						|| (Write_Config_Info.Write_Annual_Info[i].has_resend_yes_or_no > FAIL_RESEND_COUNT))
					{
						m_wait_detail.SetWindowTextW(_T("Write Annual routines Table Time Out!"));
						KillTimer(4);
						SetTimer(2,2000,NULL);
						goto write_endthis;
					}
					//TRACE(_T("OUTPUT ID = %d ,command =%d\r\n"),Write_Config_Info.Write_Output_Info[i].device_id,Write_Config_Info.Write_Output_Info[i].command);
					g_invoke_id = WritePrivateData(
						Write_Config_Info.Write_Annual_Info[i].device_id,
						Write_Config_Info.Write_Annual_Info[i].command,
						Write_Config_Info.Write_Annual_Info[i].start_instance,
						Write_Config_Info.Write_Annual_Info[i].end_instance);
					if(g_invoke_id<0)	//如果没有获取到 就继续循环;
					{
						Sleep(50);
						continue;
					}
					Write_Config_Info.Write_Annual_Info[i].has_resend_yes_or_no ++;
					Write_Config_Info.Write_Annual_Info[i].task_result = BAC_RESULTS_UNKONW;//并且将 反馈的状态 设置为未知;
					Write_Config_Info.Write_Annual_Info[i].invoke_id = g_invoke_id;	//重新记录下重发的 ID 号;

					CString temp_cs_show;
					temp_cs_show.Format(_T("Task ID = %d. Write annual routines list from item %d to %d "),g_invoke_id,
						Write_Config_Info.Write_Annual_Info[i].start_instance,
						Write_Config_Info.Write_Annual_Info[i].end_instance);
					Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,MainFram_hwd,temp_cs_show);

					//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
					TRACE(_T(" Write annual routines list Resend start = %d , Resend end = %d\r\n"),
						Write_Config_Info.Write_Annual_Info[i].start_instance,
						Write_Config_Info.Write_Annual_Info[i].end_instance);
				}
			}
			if(write_cotinue_waite == true)
				goto write_endthis;
			m_wait_progress.SetPos(100);
			m_wait_persent.SetWindowTextW(_T("100%"));
			m_wait_detail.SetWindowTextW(_T("Write descriptors success!"));
			KillTimer(4);
			SetTimer(2,2000,NULL);


write_endthis:
			Sleep(1);

		}
		break;
	case 5:
		{
			KillTimer(1);
			KillTimer(3);
			KillTimer(2);
			AfxMessageBox(_T("Read Timeout!!"));
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
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);
}


