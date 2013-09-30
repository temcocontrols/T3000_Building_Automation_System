// BacnetWait.cpp : implementation file
// Code by Fance

#include "stdafx.h"
#include "T3000.h"
#include "BacnetWait.h"
#include "afxdialogex.h"
#include "gloab_define.h"
#include "globle_function.h"
extern _Refresh_Info Bacnet_Refresh_Info;
// BacnetWait dialog

IMPLEMENT_DYNAMIC(BacnetWait, CDialogEx)

BacnetWait::BacnetWait(CWnd* pParent /*=NULL*/)
	: CDialogEx(BacnetWait::IDD, pParent)
{

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


BOOL BacnetWait::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_wait_detail.SetWindowTextW(_T("Send command to device..."));
	m_wait_detail.textColor(RGB(0,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_wait_detail.setFont(25,18,NULL,_T("Arial"));
	

	m_wait_persent.SetWindowTextW(_T("0%"));
	m_wait_persent.textColor(RGB(255,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_wait_persent.setFont(18,12,NULL,_T("Arial"));
	


	// TODO:  Add extra initialization here
	SetTimer(1,200,NULL);
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
	CString temp_pos;
	static int static_count=0;
	static_count = (++static_count)%(5+BAC_READ_GROUP_NUMBER*2-1);
	CString tempcs,tempcs2;
	tempcs.Format(_T("Reading descriptors "));
	for (int i=0;i<=static_count;i++)
	{
		tempcs2 = tempcs2 + _T(".");
	}
	int total_count = BAC_INPUT_GROUP + 
					  BAC_OUTPUT_GROUP +	
					  BAC_PROGRAM_GROUP +		
					  BAC_VARIABLE_GROUP +
					  BAC_WEEKLY_GROUP +
					  BAC_ANNUAL_GROUP +
					  BAC_TIME_COMMAND_GROUP +
					  BAC_CONTROLLER_GROUP;
	int all_step = 1000 / total_count;
	int input_step = 1000 / BAC_INPUT_GROUP;
	int output_step = 1000 / BAC_OUTPUT_GROUP;
	int program_step = 1000 / BAC_PROGRAM_GROUP;
	int variable_step = 1000 / BAC_VARIABLE_GROUP;
	int weekly_step  = 1000/ BAC_WEEKLY_GROUP;
	int annual_step = 1000 / BAC_ANNUAL_GROUP;
	int time_command_step = 1000 / BAC_TIME_COMMAND_GROUP;
	int controller_step = 1000 / BAC_CONTROLLER_GROUP;
	tempcs = tempcs + tempcs2;
	int success_count =0;
	int pos=0;

	for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
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

	for (int i=0;i<BAC_WEEKLY_GROUP;i++)
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

	for (int i=0;i<BAC_ANNUAL_GROUP;i++)
	{
		if(bac_read_which_list ==BAC_READ_ALL_LIST)
		{
			if(Bacnet_Refresh_Info.Read_Annual_Info[i].task_result == BAC_RESULTS_OK)
			{
				success_count ++;
				pos = pos +all_step;
			}
		}
		else if(bac_read_which_list == BAC_READ_ANNUAL_LIST)
		{
			if(Bacnet_Refresh_Info.Read_Annual_Info[i].task_result == BAC_RESULTS_OK)
			{
				success_count ++;
				pos = pos + annual_step;
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
	pos = pos /10;
	if(pos>100)
		pos=100;
	m_wait_progress.SetPos(pos);

	temp_pos.Format(_T("%d%%"),pos);
	m_wait_persent.SetWindowTextW(temp_pos);

	UpdateData(FALSE);

	switch(nIDEvent)
	{
	case 1:

		for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_CONTROLLER_LIST) || (bac_read_which_list == BAC_READ_ALL_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Controller_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					m_wait_detail.SetWindowTextW(tempcs);
					goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Controller_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Controller_Info[i].resend_count ++ ;

					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Controller_Info[i].resend_count>10) 
						|| (Bacnet_Refresh_Info.Read_Controller_Info[i].has_resend_yes_or_no))
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
						continue;
					Bacnet_Refresh_Info.Read_Controller_Info[i].has_resend_yes_or_no = true;
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
			if((bac_read_which_list == BAC_READ_TIME_COMMAND) || (bac_read_which_list == BAC_READ_ALL_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Time_Command[i].task_result == BAC_RESULTS_UNKONW)
				{
					m_wait_detail.SetWindowTextW(tempcs);
					goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Time_Command[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Time_Command[i].resend_count ++ ;

					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Time_Command[i].resend_count>10) 
						|| (Bacnet_Refresh_Info.Read_Time_Command[i].has_resend_yes_or_no))
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
						continue;
					Bacnet_Refresh_Info.Read_Time_Command[i].has_resend_yes_or_no = true;
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







		for (int i=0;i<BAC_ANNUAL_GROUP;i++)
		{
			if((bac_read_which_list == BAC_READ_ANNUAL_LIST) || (bac_read_which_list == BAC_READ_ALL_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Annual_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					m_wait_detail.SetWindowTextW(tempcs);
					goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Annual_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Annual_Info[i].resend_count ++ ;

					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Annual_Info[i].resend_count>10) 
						|| (Bacnet_Refresh_Info.Read_Annual_Info[i].has_resend_yes_or_no))
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
						continue;
					Bacnet_Refresh_Info.Read_Annual_Info[i].has_resend_yes_or_no = true;
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
			if((bac_read_which_list == BAC_READ_WEEKLY_LIST) || (bac_read_which_list == BAC_READ_ALL_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					m_wait_detail.SetWindowTextW(tempcs);
					goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Weekly_Info[i].resend_count ++ ;

					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Weekly_Info[i].resend_count>10) 
						|| (Bacnet_Refresh_Info.Read_Weekly_Info[i].has_resend_yes_or_no))
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
						continue;
					Bacnet_Refresh_Info.Read_Weekly_Info[i].has_resend_yes_or_no = true;
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
			if((bac_read_which_list == BAC_READ_PROGRAM_LIST) || (bac_read_which_list == BAC_READ_ALL_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Program_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					m_wait_detail.SetWindowTextW(tempcs);
					goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Program_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Program_Info[i].resend_count ++ ;

					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Program_Info[i].resend_count>10) 
						|| (Bacnet_Refresh_Info.Read_Program_Info[i].has_resend_yes_or_no))
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
						continue;
					Bacnet_Refresh_Info.Read_Program_Info[i].has_resend_yes_or_no = true;
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
		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			 if((bac_read_which_list == BAC_READ_INPUT_LIST)|| (bac_read_which_list == BAC_READ_ALL_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Input_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					m_wait_detail.SetWindowTextW(tempcs);
					goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Input_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Input_Info[i].resend_count ++ ;

					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Input_Info[i].resend_count>10) 
						|| (Bacnet_Refresh_Info.Read_Input_Info[i].has_resend_yes_or_no))
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
						continue;
					Bacnet_Refresh_Info.Read_Input_Info[i].has_resend_yes_or_no = true;
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
			if((bac_read_which_list == BAC_READ_OUTPUT_LIST)|| (bac_read_which_list == BAC_READ_ALL_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Output_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					m_wait_detail.SetWindowTextW(tempcs);
					goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Output_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Output_Info[i].resend_count ++ ;

					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
					if((Bacnet_Refresh_Info.Read_Output_Info[i].resend_count>10) 
						|| (Bacnet_Refresh_Info.Read_Output_Info[i].has_resend_yes_or_no))
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
						continue;
					Bacnet_Refresh_Info.Read_Output_Info[i].has_resend_yes_or_no = true;
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
			if((bac_read_which_list == BAC_READ_VARIABLE_LIST) || (bac_read_which_list == BAC_READ_ALL_LIST))
			{
				if(Bacnet_Refresh_Info.Read_Variable_Info[i].task_result == BAC_RESULTS_UNKONW)
				{
					m_wait_detail.SetWindowTextW(tempcs);
					goto endthis;
				}
				else if(Bacnet_Refresh_Info.Read_Variable_Info[i].task_result == BAC_RESULTS_FAIL)
				{
					Bacnet_Refresh_Info.Read_Variable_Info[i].resend_count ++ ;

					//只要发送10次超时，或者判断已经发送了，并且还是返回失败 就显示超时;
						if((Bacnet_Refresh_Info.Read_Variable_Info[i].resend_count>10) 
							|| (Bacnet_Refresh_Info.Read_Variable_Info[i].has_resend_yes_or_no))
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
							continue;
						Bacnet_Refresh_Info.Read_Variable_Info[i].has_resend_yes_or_no = true;
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
m_wait_progress.SetPos(100);
m_wait_persent.SetWindowTextW(_T("100%"));

		bac_input_read_results = true;
		bac_output_read_results = true;
		bac_variable_read_results = true;
		bac_program_read_results = true;
		bac_weekly_read_results = true;
		bac_annual_read_results = true;
		bac_controller_read_results = true;
			m_wait_detail.SetWindowTextW(_T("Reading descriptors success!"));
			KillTimer(1);
			if(bac_read_which_list ==BAC_READ_ALL_LIST)
			SetTimer(2,2000,NULL);
			else
			SetTimer(2,100,NULL);

endthis:
		break;
	case 2:
		{
			KillTimer(2);
			SetTimer(3,500,NULL);
			ShowWindow(SW_HIDE);
		}
		break;
	case 3:
		{
			KillTimer(3);
			::PostMessage(BacNet_hwd,WM_DELETE_WAIT_DLG,0,0);
			PostMessage(WM_CLOSE,NULL,NULL);
			//OnCancel();
		}
	}
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);
}


