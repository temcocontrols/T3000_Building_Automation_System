// DialogCM5_BacNet.cpp : implementation file
// DialogCM5 Bacnet programming by Fance 2013 05 01
/*
2014-01-13
	Update by Fance
	1.Add BacnetScreenEdit.cpp User can edit screen window,add label delete label.
	2.Modify the ListTree if one device is choose by user , It will change his color to red.
	3.Modify the range ,user can change the range such as  "ON/OFF" "OFF/ON" .In "ON/OFF" range the ON is 1 but in "OFF/ON" the ON it is 0;
	4.Change the Bacnet Main window. Insert all the property dialog into the view window.User can operated by the mouse click or keyboard input;
	5.Add Load config all and write config file .The file will be saved with "*.prg" and also user can loaded.
	6.Fix a bug that the program function "Interval" is not normally used;
2013-12-19
	Update by Fance
	1.Monify the list class,add function which we can select the list item.Also add list backcolor function;
	2.Add bacnet "Monitor" "Graphic" and "Screens".
	3.Use GDI to draw the bacnet monitor data.
	4.Add bacnet IP Scan,now T3000 can scan our own bacnet ip products.
	5.Add bacnet config file ,user can save the bacnet data to the config file,or load the config file;
	6.Fix the tree view can't show the bacnet products normally.
	7.Fix the transmission results,"RX" "TX" "ERROR".Now it can show bacnet ip transfer results normally;
	8.Fix all the bacnet listctrl , add function user can use keyboard operate it.
	9.Improve the function when user choose one of the bacnet list item ,if it is a combobox ,it will auto pull-down,and show all the items for user to choose.
	10.Improve  when user use bacnet function to change one of the value ,if the data change fail ,the data will resume to origin instead of show the error data on the screen.
	11.Fix program code dialog crashed,when the text length longer than the MAX_PATH;
	12.Fix when the label length can longer than the structure defined.
	13.Change the communication protocol from bacnetMSTP to BacnetIp;
	14.Save the bacnet instance id to mdb sw version and hw version,save the ip address to boudrate;
*/

#include "stdafx.h"
#include "T3000.h"
#include "DialogCM5_BacNet.h"
#include "CM5\MyOwnListCtrl.h"
#include "BacnetInput.h"
#include "BacnetOutput.h"
#include "BacnetProgram.h"
#include "BacnetVariable.h"
#include "global_function.h"

#include "gloab_define.h"
#include "datalink.h"
#include "BacnetWait.h"
#include "Bacnet_Include.h"
#include "CM5\ud_str.h"
#include "BacnetWeeklyRoutine.h"
#include "BacnetAnnualRoutine.h"
#include "AnnualRout_InsertDia.h"
#include "BacnetController.h"
#include "BacnetScreen.h"
#include "BacnetMonitor.h"
#include "BacnetProgramEdit.h"
#include "rs485.h"
#include "BacnetScheduleTime.h"
#include "BacnetAlarmLog.h"
#include "BacnetAlarmWindow.h"
//bool CM5ProcessPTA(	BACNET_PRIVATE_TRANSFER_DATA * data);
//bool need_to_read_description = true;
int g_gloab_bac_comport = 1;
CString temp_device_id,temp_mac,temp_vendor_id;
HANDLE hwait_thread;
BacnetWait *WaitDlg=NULL;
extern int Station_NUM;
BacnetScreen *Screen_Window = NULL;
CBacnetProgram *Program_Window =NULL;
CBacnetInput *Input_Window = NULL;
CBacnetOutput *Output_Window = NULL;
CBacnetVariable *Variable_Window = NULL;
BacnetWeeklyRoutine *WeeklyRoutine_Window = NULL;
BacnetAnnualRoutine *AnnualRoutine_Window = NULL;
BacnetController *Controller_Window = NULL;
CBacnetMonitor *Monitor_Window = NULL;
CBacnetAlarmLog *AlarmLog_Window = NULL;
extern CBacnetAlarmWindow * AlarmWindow_Window;
extern bool is_in_scan_mode;
extern char mycode[1024];
// CDialogCM5_BacNet

_Refresh_Info Bacnet_Refresh_Info;

//#define WM_SEND_OVER     WM_USER + 1287
// int m_Input_data_length;
extern void  init_info_table( void );
extern void Init_table_bank();

IMPLEMENT_DYNCREATE(CDialogCM5_BacNet, CFormView)

CDialogCM5_BacNet::CDialogCM5_BacNet()
	: CFormView(CDialogCM5_BacNet::IDD)
{
	m_MSTP_THREAD = true;
	m_cur_tab_sel = 0;
	//CM5_hThread = NULL;
}

CDialogCM5_BacNet::~CDialogCM5_BacNet()
{
}

void CDialogCM5_BacNet::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST1, m_device_list_info);
	DDX_Control(pDX, IDC_TIME_PICKER, m_cm5_time_picker);
	DDX_Control(pDX, IDC_DATE_PICKER, m_cm5_date_picker);

	DDX_Control(pDX, IDC_BAC_MAINTAB, m_bac_main_tab);
}

BEGIN_MESSAGE_MAP(CDialogCM5_BacNet, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDialogCM5_BacNet::OnBnClickedButtonTest)
	ON_MESSAGE(WM_FRESH_CM_LIST,Fresh_UI)
	ON_MESSAGE(MY_RESUME_DATA, AllMessageCallBack)
	ON_MESSAGE(WM_DELETE_NEW_MESSAGE_DLG,Delete_New_Dlg)	
	ON_BN_CLICKED(IDC_BUTTON_BAC_TEST, &CDialogCM5_BacNet::OnBnClickedButtonBacTest)
	ON_BN_CLICKED(IDC_BAC_ENABLE_EDIT_TIME, &CDialogCM5_BacNet::OnBnClickedBacEnableEditTime)
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATE_PICKER, &CDialogCM5_BacNet::OnNMKillfocusDatePicker)
	ON_NOTIFY(NM_KILLFOCUS, IDC_TIME_PICKER, &CDialogCM5_BacNet::OnNMKillfocusTimePicker)
	ON_NOTIFY(NM_SETFOCUS, IDC_DATE_PICKER, &CDialogCM5_BacNet::OnNMSetfocusDatePicker)
	ON_NOTIFY(NM_SETFOCUS, IDC_TIME_PICKER, &CDialogCM5_BacNet::OnNMSetfocusTimePicker)
	ON_BN_CLICKED(IDC_BTN_BAC_WRITE_TIME, &CDialogCM5_BacNet::OnBnClickedBtnBacWriteTime)
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_BAC_MAINTAB, &CDialogCM5_BacNet::OnTcnSelchangeBacMaintab)
END_MESSAGE_MAP()


// CDialogCM5_BacNet diagnostics

#ifdef _DEBUG
void CDialogCM5_BacNet::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDialogCM5_BacNet::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

//The window which created by the button ,will delete when the wait dialog send this message,to this window.
//It means ,it has done .we don't needed.
LRESULT CDialogCM5_BacNet::Delete_New_Dlg(WPARAM wParam,LPARAM lParam)
{
	int message_type = wParam;
	switch(message_type)
	{
	case 0:
		if(WaitDlg!=NULL)
		{
			Sleep(50);
			if(WaitDlg)
				delete WaitDlg;
			WaitDlg = NULL;

			if(bac_read_which_list == BAC_READ_SVAE_CONFIG)
			{
				if(bac_read_all_results)
					::PostMessageW(MainFram_hwd,MY_BAC_CONFIG_READ_RESULTS,1,NULL);
				return 0;
			}

			if(bac_read_which_list == BAC_READ_INPUT_LIST)
			{
				if(bac_input_read_results)
				{
					if(Input_Window == NULL)
					{
						Input_Window = new CBacnetInput;
						Input_Window->Create(IDD_DIALOG_BACNET_INPUT,this);	
					}
					else
					{
						::PostMessage(m_input_dlg_hwnd,WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
					}
					Input_Window->ShowWindow(SW_SHOW);
					m_bac_main_tab.SetCurSel(WINDOW_INPUT);
				}
				else
					MessageBox(_T("Inputs list read time out!"));	
				return 0;
			}

			if(bac_read_which_list == BAC_READ_OUTPUT_LIST)
			{
				if(bac_output_read_results)
				{
					if(Output_Window == NULL)
					{
						Output_Window = new CBacnetOutput;
						Output_Window->Create(IDD_DIALOG_BACNET_OUTPUT,this);	
					}
					else
					{
						::PostMessage(m_output_dlg_hwnd,WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
					}
					Output_Window->ShowWindow(SW_SHOW);
					m_bac_main_tab.SetCurSel(WINDOW_OUTPUT);

				}
				else
					MessageBox(_T("Outputs list read time out!"));
				return 0;
			}

			if(bac_read_which_list == BAC_READ_VARIABLE_LIST)
			{
				if(bac_variable_read_results)
				{
					if(Variable_Window == NULL)
					{
						Variable_Window = new CBacnetVariable;
						Variable_Window->Create(IDD_DIALOG_BACNET_VARIABLE,this);	
					}
					else
					{
						::PostMessage(m_variable_dlg_hwnd,WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
					}
					Variable_Window->ShowWindow(SW_SHOW);
					m_bac_main_tab.SetCurSel(WINDOW_VARIABLE);

				}
				else
					MessageBox(_T("Variable list read time out!"));
				return 0;
			}

			if(bac_read_which_list == BAC_READ_PROGRAM_LIST)
			{
				if(bac_program_read_results)
				{
					//CBacnetProgram DLG;
					//DLG.DoModal();

					if(Program_Window == NULL)
					{
						Program_Window = new CBacnetProgram;
						Program_Window->Create(IDD_DIALOG_BACNET_PROGRAM,this);	
					}
					else
					{
						::PostMessage(m_pragram_dlg_hwnd,WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
					}
					Program_Window->ShowWindow(SW_SHOW);
					m_bac_main_tab.SetCurSel(WINDOW_PROGRAM);

				}
				else
					MessageBox(_T("Program list read time out!"));
				return 0;
			}

			if(bac_read_which_list == BAC_READ_WEEKLY_LIST)
			{
				if(bac_weekly_read_results)
				{
					//CBacnetProgram DLG;
					//DLG.DoModal();

					if(WeeklyRoutine_Window == NULL)
					{
						WeeklyRoutine_Window = new BacnetWeeklyRoutine;
						WeeklyRoutine_Window->Create(IDD_DIALOG_BACNET_WEEKLY_ROUTINES,this);	
					}
					else
					{
						::PostMessage(m_weekly_dlg_hwnd,WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
					}
					WeeklyRoutine_Window->ShowWindow(SW_SHOW);
					m_bac_main_tab.SetCurSel(WINDOW_WEEKLY);
				}
				else
					MessageBox(_T("Program list read time out!"));
				return 0;
			}
#if 0
			if(bac_read_which_list == BAC_READ_WEEKLY_LIST)
			{
				if(bac_weekly_read_results)
				{
					BacnetWeeklyRoutine Dlg;
					Dlg.DoModal();
				}
				else
					MessageBox(_T("Weekly Routine list read time out!"));
			}
#endif
			if(bac_read_which_list == BAC_READ_ANNUAL_LIST)
			{
				if(bac_annual_read_results)
				{
					if(AnnualRoutine_Window == NULL)
					{
						AnnualRoutine_Window = new BacnetAnnualRoutine;
						AnnualRoutine_Window->Create(IDD_DIALOG_BACNET_ANNUAL_ROUTINES,this);
					}
					else
					{
						::PostMessage(m_annual_dlg_hwnd,WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
					}
					AnnualRoutine_Window->ShowWindow(SW_SHOW);
					m_bac_main_tab.SetCurSel(WINDOW_ANNUAL);
				}
				else
					MessageBox(_T("Annual Routine list read time out!"));
			}

			if(bac_read_which_list == BAC_READ_CONTROLLER_LIST)
			{
				if(bac_controller_read_results)
				{
					if(Controller_Window == NULL)
					{
						Controller_Window = new BacnetController;
						Controller_Window->Create(IDD_DIALOG_BACNET_CONTROLLER,this);
					}
					else
					{
						::PostMessage(m_controller_dlg_hwnd,WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
					}
					Controller_Window->ShowWindow(SW_SHOW);
					m_bac_main_tab.SetCurSel(WINDOW_CONTROLLER);
				}
				else
					MessageBox(_T("Controller list read time out!"));
			}
			if(bac_read_which_list == BAC_READ_SCREEN_LIST)
			{
				if(bac_screen_read_results)
				{
					if(Screen_Window == NULL)
					{
						Screen_Window = new BacnetScreen;
						Screen_Window->Create(IDD_DIALOG_BACNET_SCREENS,this);	
					}
					else
					{
						::PostMessage(m_screen_dlg_hwnd,WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
					}
					Screen_Window->ShowWindow(SW_SHOW);
					m_bac_main_tab.SetCurSel(WINDOW_SCREEN);
				}
				else
					MessageBox(_T("Screens list read time out!"));
			}
			if(bac_read_which_list == BAC_READ_MONITOR_LIST)
			{
				if(bac_monitor_read_results)
				{
					if(Monitor_Window == NULL)
					{
						Monitor_Window = new CBacnetMonitor;
						Monitor_Window->Create(IDD_DIALOG_BACNET_MONITOR,this);
					}
					else
					{
						::PostMessage(m_screen_dlg_hwnd,WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
					}
					Monitor_Window->ShowWindow(SW_SHOW);
					m_bac_main_tab.SetCurSel(WINDOW_MONITOR);

				}
				else
					MessageBox(_T("Monitor list read time out!"));
			}

			if(bac_read_which_list == BAC_READ_ALARMLOG_LIST)
			{
				if(bac_alarmlog_read_results)
				{
					if(AlarmLog_Window == NULL)
					{
						AlarmLog_Window = new CBacnetAlarmLog;
						AlarmLog_Window->Create(IDD_DIALOG_BACNET_ALARMLOG,this);
					}
					else
					{
						//Refresh the alarmlog list;
					}
					AlarmLog_Window->ShowWindow(SW_SHOW);
					m_bac_main_tab.SetCurSel((WINDOW_ALARMLOG));
				}
			}

			if(bac_read_which_list == BAC_READ_PROGRAMCODE_LIST)
			{
				if(bac_programcode_read_results)
				{
					CBacnetProgramEdit Dlg;
					Dlg.DoModal();
				}
				else
					MessageBox(_T("Monitor list read time out!"));
			}

			if(bac_read_which_list == BAC_READ_WEEKLTCODE_LIST)
			{
				if(bac_weeklycode_read_results)
				{
					CBacnetScheduleTime Dlg;
					Dlg.DoModal();
				}
				else
					MessageBox(_T("Weekly schedual time read time out!"));
			}
								
			if(bac_read_which_list == BAC_READ_ANNUALCODE_LIST)
			{
				if(bac_annualcode_read_results)
				{
					AnnualRout_InsertDia Dlg;
					Dlg.DoModal();
				}
				else
					MessageBox(_T("Annual day time read time out!"));
			}
		}
		break;
	case DELETE_WINDOW_MSG:
		m_bac_main_tab.SetFocus();
	//	m_bac_main_tab.SetCurSel(m_cur_tab_sel);
		break;
		if(Screen_Window != NULL)
		{
			//Sleep(1000);
			delete Screen_Window;
			Screen_Window = NULL;
		}

		if(Program_Window !=NULL)
		{
			delete Program_Window;
			Program_Window = NULL;
		}

		if(WeeklyRoutine_Window !=NULL)
		{
			delete WeeklyRoutine_Window;
			WeeklyRoutine_Window = NULL;
		}

		if(Input_Window !=NULL)
		{
			delete Input_Window;
			Input_Window = NULL;
		}

		if(Output_Window !=NULL)
		{
			delete Output_Window;
			Output_Window = NULL;
		}
		if(Variable_Window !=NULL)
		{
			delete Variable_Window;
			Variable_Window = NULL;
		}
		if(Monitor_Window !=NULL)
		{
			delete Monitor_Window;
			Monitor_Window = NULL;
		}
		if(Controller_Window !=NULL)
		{
			delete Controller_Window;
			Controller_Window = NULL;
		}
		if(AnnualRoutine_Window !=NULL)
		{
			delete AnnualRoutine_Window;
			AnnualRoutine_Window = NULL;
		}
		break;
	}
	
	return 0;
}
// CDialogCM5_BacNet message handlers

LRESULT  CDialogCM5_BacNet::AllMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if(msg_result)
	{
		for (int i=0;i<BAC_PROGRAM_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Program_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Programcode_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Output_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Input_Info[i].task_result = true;
		}
		for (int i=0;i<BAC_VARIABLE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_WEEKLY_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_WEEKLYCODE_GOUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Weeklycode_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Weeklycode_Info[i].task_result = true;
		}


		for (int i=0;i<BAC_ANNUAL_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Annual_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id)
				Bacnet_Refresh_Info.Read_Time_Command[i].task_result = true;
		}
		for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Controller_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_SCREEN_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Screen_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Screen_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_MONITOR_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Monitor_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result = true;
		}
		for (int i=0;i<BAC_ANNUALCODE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Annualcode_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Annualcode_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_ALARMLOG_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_AlarmLog_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_AlarmLog_Info[i].task_result = true;
		}

		if(connect_replay)
		{
			if(connect_invoke_id == pInvoke->Invoke_ID)
			{
				PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALL);
				CString temp_cstring;
				temp_cstring.Format(_T("Connected"),g_bac_instance);
				SetPaneString(1,temp_cstring);
				m_bac_main_tab.SetFocus();
				connect_invoke_id = -1;
				connect_replay = false;
			}
		}

		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//MessageBox(_T("Bacnet operation success!"));
	}
	else
	{
		for (int i=0;i<BAC_PROGRAM_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Program_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Programcode_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_OUTPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Output_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_INPUT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Input_Info[i].task_result = false;
		}
		for (int i=0;i<BAC_VARIABLE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_WEEKLY_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_WEEKLYCODE_GOUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Weeklycode_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Weeklycode_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_ANNUAL_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Annual_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id)
				Bacnet_Refresh_Info.Read_Time_Command[i].task_result = false;
		}
		for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Controller_Info[i].task_result = false;
		}
		for (int i=0;i<BAC_SCREEN_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Screen_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Screen_Info[i].task_result = false;
		}
		for (int i=0;i<BAC_MONITOR_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Monitor_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result = false;
		}
		for (int i=0;i<BAC_ANNUALCODE_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Annualcode_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Annualcode_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_ALARMLOG_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_AlarmLog_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_AlarmLog_Info[i].task_result = false;
		}

		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//AfxMessageBox(Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}
	
	if(pInvoke)
		delete pInvoke;
	return 0;
}

void CDialogCM5_BacNet::OnBnClickedButtonTest()
{
	// TODO: Add your control notification handler code here
	Send_WhoIs_Global(g_bac_instance, g_bac_instance);
}


void CDialogCM5_BacNet::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	//#ifdef Fance_Enable_Test
	//Fresh();//Fance
	Initial_All_Point();

	

	//#endif
	// TODO: Add your specialized code here and/or call the base class
}

void CDialogCM5_BacNet::Tab_Initial()
{
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//为Tab Control增加两个页面;
	m_bac_main_tab.InsertItem(WINDOW_INPUT, _T("Input    "));
	m_bac_main_tab.InsertItem(WINDOW_OUTPUT, _T("Output    "));
	m_bac_main_tab.InsertItem(WINDOW_VARIABLE, _T("Variable    "));
	m_bac_main_tab.InsertItem(WINDOW_PROGRAM, _T("Program    "));
	m_bac_main_tab.InsertItem(WINDOW_CONTROLLER, _T("Controller    "));
	m_bac_main_tab.InsertItem(WINDOW_SCREEN, _T("Graphic    "));
	m_bac_main_tab.InsertItem(WINDOW_WEEKLY, _T("Weekly Routine    "));
	m_bac_main_tab.InsertItem(WINDOW_ANNUAL, _T("Annual Routine    "));
	m_bac_main_tab.InsertItem(WINDOW_MONITOR, _T("Monitor    "));
	m_bac_main_tab.InsertItem(WINDOW_ALARMLOG, _T("Alarm Log    "));

	pDialog[WINDOW_INPUT] = Input_Window = new CBacnetInput;
	pDialog[WINDOW_OUTPUT] =Output_Window = new CBacnetOutput;
	pDialog[WINDOW_VARIABLE] = Variable_Window = new CBacnetVariable;
	pDialog[WINDOW_PROGRAM] = Program_Window = new CBacnetProgram;
	pDialog[WINDOW_CONTROLLER] = Controller_Window = new BacnetController;
	pDialog[WINDOW_SCREEN] = Screen_Window = new BacnetScreen;
	pDialog[WINDOW_WEEKLY] = WeeklyRoutine_Window = new BacnetWeeklyRoutine;
	pDialog[WINDOW_ANNUAL] = AnnualRoutine_Window = new BacnetAnnualRoutine;
	pDialog[WINDOW_MONITOR] = Monitor_Window = new CBacnetMonitor;
	pDialog[WINDOW_ALARMLOG] = AlarmLog_Window = new CBacnetAlarmLog;
	
	//创建两个对话框;
	Input_Window->Create(IDD_DIALOG_BACNET_INPUT, &m_bac_main_tab);
	Output_Window->Create(IDD_DIALOG_BACNET_OUTPUT, &m_bac_main_tab);
	Variable_Window->Create(IDD_DIALOG_BACNET_VARIABLE, &m_bac_main_tab);
	Program_Window->Create(IDD_DIALOG_BACNET_PROGRAM, &m_bac_main_tab);
	Controller_Window->Create(IDD_DIALOG_BACNET_CONTROLLER, &m_bac_main_tab);
	Screen_Window->Create(IDD_DIALOG_BACNET_SCREENS, &m_bac_main_tab);
	WeeklyRoutine_Window->Create(IDD_DIALOG_BACNET_WEEKLY_ROUTINES, &m_bac_main_tab);
	AnnualRoutine_Window->Create(IDD_DIALOG_BACNET_ANNUAL_ROUTINES, &m_bac_main_tab);
	Monitor_Window->Create(IDD_DIALOG_BACNET_MONITOR, &m_bac_main_tab);
	AlarmLog_Window->Create(IDD_DIALOG_BACNET_ALARMLOG,&m_bac_main_tab);
	//设定在Tab内显示的范围;
	CRect rc;
	m_bac_main_tab.GetClientRect(rc);
	rc.top += 20;
	//rc.bottom -= 8;
	//rc.left += 8;
	//rc.right -= 8;

	for (int i=0;i<10;i++)
	{
		pDialog[i]->MoveWindow(&rc);
	}


	//显示初始页面
	pDialog[WINDOW_INPUT]->ShowWindow(SW_SHOW);
	pDialog[WINDOW_OUTPUT]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_VARIABLE]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_PROGRAM]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_CONTROLLER]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_SCREEN]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_WEEKLY]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_ANNUAL]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_MONITOR]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_ALARMLOG]->ShowWindow(SW_HIDE);

	g_hwnd_now = m_input_dlg_hwnd;
	//保存当前选择
//	m_CurSelTab = WINDOW_INPUT;

}

void CDialogCM5_BacNet::Initial_All_Point()
{
	m_Input_data.clear();
	m_Variable_data.clear();
	m_Output_data.clear();
	m_Program_data.clear();
	m_Weekly_data.clear();
	m_Annual_data.clear();
	m_Schedual_Time_data.clear();
	m_controller_data.clear();
	m_screen_data.clear();
	m_monitor_data.clear();
	m_alarmlog_data.clear();
	for(int i=0;i<BAC_INPUT_ITEM_COUNT;i++)
	{
		Str_in_point temp_in;
		memset(temp_in.description,0,21);
		memset(temp_in.label,0,9);
		m_Input_data.push_back(temp_in);

	}
	for(int i=0;i<BAC_OUTPUT_ITEM_COUNT;i++)
	{
		Str_out_point temp_out;
		memset(temp_out.description,0,21);
		memset(temp_out.label,0,9);
		m_Output_data.push_back(temp_out);
	}
	for (int i=0;i<BAC_VARIABLE_ITEM_COUNT;i++)
	{
		Str_variable_point temp_variable;
		memset(temp_variable.description,0,21);
		memset(temp_variable.label,0,9);
		m_Variable_data.push_back(temp_variable);
	}
	for(int i=0;i<BAC_PROGRAM_ITEM_COUNT;i++)
	{
		Str_program_point temp_program;
		memset(temp_program.description,0,21);
		memset(temp_program.label,0,9);
		temp_program.bytes = 400;//初始化时默认为400的长度，避免读不到数据;
		m_Program_data.push_back(temp_program);
	}
	for(int i=0;i<BAC_WEEKLY_ROUTINES_COUNT;i++)
	{
		Str_weekly_routine_point temp_weekly;
		m_Weekly_data.push_back(temp_weekly);
	}
	for (int i=0;i<BAC_ANNUAL_ROUTINES_COUNT;i++)
	{
		Str_annual_routine_point temp_annual;
		m_Annual_data.push_back(temp_annual);
	}

	for (int i=0;i<BAC_WEEKLY_ROUTINES_COUNT;i++)
	{
		Str_schedual_time_point temp_schedual;
		m_Schedual_Time_data.push_back(temp_schedual);
	}

	for (int i=0;i<BAC_CONTROLLER_COUNT;i++)
	{
		Str_controller_point temp_controller;
		m_controller_data.push_back(temp_controller);
	}
	for (int i=0;i<BAC_SCREEN_COUNT;i++)
	{
		Control_group_point temp_screen;
		m_screen_data.push_back(temp_screen);
	}
	for (int i=0;i<BAC_MONITOR_COUNT;i++)
	{
		Str_monitor_point temp_monitor;
		m_monitor_data.push_back(temp_monitor);
	}

	for (int i=0;i<BAC_ALARMLOG_COUNT;i++)
	{
		Alarm_point temp_alarmpoint;
		memset(temp_alarmpoint.alarm_message,0,ALARM_MESSAGE_SIZE);
		m_alarmlog_data.push_back(temp_alarmpoint);
	}
}
//__declspec(dllexport) HANDLE	Get_RS485_Handle();
HWND temp_hwnd;
//INPUT int test_function_return_value();
void CDialogCM5_BacNet::Fresh()
{
	PHOSTENT  hostinfo;  
	char  name[255];  
	CString  ip; 

	if(  gethostname  (  name,  sizeof(name))  ==  0)  
	{  
		if((hostinfo  =  gethostbyname(name))  !=  NULL)  
		{  
			ip  =  inet_ntoa  (*(struct  in_addr  *)*hostinfo->h_addr_list);  
		}  
	}  

	CStringArray TEMPCS;
	SplitCStringA(TEMPCS, ip, _T("."));
	int ttttt = TEMPCS.GetSize();
	if((int)TEMPCS.GetSize() ==4)
	{
		if(0)
		{
		my_ip[0] = _wtoi(TEMPCS.GetAt(0));
		my_ip[1] = _wtoi(TEMPCS.GetAt(1));
		my_ip[2] = _wtoi(TEMPCS.GetAt(2));
		my_ip[3] = _wtoi(TEMPCS.GetAt(3));
		}
		my_ip[3] = _wtoi(TEMPCS.GetAt(0));
		my_ip[2] = _wtoi(TEMPCS.GetAt(1));
		my_ip[1] = _wtoi(TEMPCS.GetAt(2));
		my_ip[0] = _wtoi(TEMPCS.GetAt(3));
	}
#if 0
	BACNET_ADDRESS src = {
		0
	};  /* address where message came from */
	uint16_t pdu_len = 0;
	unsigned timeout = 100;     /* milliseconds */
	BACNET_ADDRESS my_address, broadcast_address;
	char my_port[50];

	//if(bac_net_initial_once)
	//{
	//	BacNet_hwd = this->m_hWnd;
	//}

	if(!bac_net_initial_once)
	{
		bac_net_initial_once =true;
		bac_program_pool_size = 26624;
		bac_program_size = 0;
		bac_free_memory = 26624;
		hwait_thread = NULL;
		HANDLE temphandle;
		temphandle = Get_RS485_Handle();
		if(temphandle !=NULL)
		{
			TerminateThread((HANDLE)Get_Thread1(),0);
			TerminateThread((HANDLE)Get_Thread2(),0);

			CloseHandle(temphandle);
			Set_RS485_Handle(NULL);
		}

		SetCommunicationType(0);	//这里要同时关闭网络的和串口的;
		close_com();
		//SetCommunicationType(1);
		//close_com();
		Device_Set_Object_Instance_Number(4194300);
		address_init();
		Init_Service_Handlers();
		dlmstp_set_baud_rate(19200);
		dlmstp_set_mac_address(0);
		dlmstp_set_max_info_frames(DEFAULT_MAX_INFO_FRAMES);
		dlmstp_set_max_master(DEFAULT_MAX_MASTER);
		memset(my_port,0,50);

		CString Program_Path,Program_ConfigFile_Path;
		int g_com=0;
		GetModuleFileName(NULL,Program_Path.GetBuffer(MAX_PATH),MAX_PATH);  //获取当前运行的绝对地址;
		PathRemoveFileSpec(Program_Path.GetBuffer(MAX_PATH) );            //返回绝对地址的上层目录?;
		Program_Path.ReleaseBuffer();
		Program_ConfigFile_Path = Program_Path + _T("\\MyConfig.ini");

		CFileFind fFind;
		if(!fFind.FindFile(Program_ConfigFile_Path))
		{
			WritePrivateProfileStringW(_T("Setting"),_T("ComPort"),_T("1"),Program_ConfigFile_Path);
		}
		g_com = GetPrivateProfileInt(_T("Setting"),_T("ComPort"),1,Program_ConfigFile_Path);
		CString temp_cs;
		temp_cs.Format(_T("COM%d"),g_com);

		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, temp_cs.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		temp_cs.ReleaseBuffer();


		sprintf(my_port,cTemp1);
		dlmstp_init(my_port);


		//Initial_List();
		//		BacNet_hwd = this->m_hWnd;
		//    dlenv_init();
		datalink_get_broadcast_address(&broadcast_address);
		//    print_address("Broadcast", &broadcast_address);
		datalink_get_my_address(&my_address);
		//		print_address("Address", &my_address);

		CM5_hThread =CreateThread(NULL,NULL,MSTP_Receive,this,NULL, &nThreadID);

		CM5_UI_Thread = CreateThread(NULL,NULL,CM5_UI,this,NULL, &cm5_nThreadID);

		Send_WhoIs_Global(-1,-1);

		m_cm5_date_picker.EnableWindow(0);
		m_cm5_time_picker.EnableWindow(0);
		GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(0);
		GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(FALSE);
		m_cm5_time_picker.SetFormat(_T("HH:mm"));


		SetTimer(1,500,NULL);
		BacNet_hwd = this->m_hWnd;
		//m_bac_static_status.SetWindowTextW(_T("Disconnected"));	//放在这里，并且重发一次WHOIS
		//m_bac_static_status.textColor(RGB(255,255,255));
		//m_bac_static_status.bkColor(RGB(255,0,0));
		//m_bac_static_status.setFont(26,18,NULL,_T("Cambria"));
		timesec1970 = time(NULL);
		timestart = 0;
		init_info_table();
		Init_table_bank();
	}
#endif
	//Building_info build_info;
	//build_info=m_subNetLst.at(m_nCurSubBuildingIndex);
	//int nComPort = _wtoi(build_info.strComPort.Mid(3));

	Initial_bac(g_gloab_bac_comport);
	m_cm5_date_picker.EnableWindow(0);
	m_cm5_time_picker.EnableWindow(0);
	GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(0);
	GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(FALSE);
	m_cm5_time_picker.SetFormat(_T("HH:mm"));

	SetTimer(1,500,NULL);
	SetTimer(2,15000,NULL);//定时器2用于间隔发送 whois;不知道设备什么时候会被移除;
	SetTimer(3,1000,NULL); //Check whether need  show Alarm dialog.
	BacNet_hwd = this->m_hWnd;

	CString temp_cs;
	temp_cs.Format(_T("%d"),g_bac_instance);
	GetDlgItem(IDC_STATIC_CM_DEVICE_ID)->SetWindowTextW(temp_cs);
	temp_cs.Format(_T("%d"),g_mac);
	GetDlgItem(IDC_STATIC_CM5_MAC)->SetWindowTextW(temp_cs);
	Station_NUM = g_mac;

	//GetDlgItem(IDC_STATIC_CM_DEVICE_ID)->SetWindowTextW(bac_cs_device_id);
	//GetDlgItem(IDC_STATIC_CM5_MAC)->SetWindowTextW(bac_cs_mac);
	GetDlgItem(IDC_STATIC_CM5_VENDOR_ID)->SetWindowTextW(bac_cs_vendor_id);
	//Send_WhoIs_Global(-1,-1);
	Send_WhoIs_Global(g_bac_instance, g_bac_instance);
	Sleep(300);
	bool find_exsit = false;
	for (int i=0;i<(int)m_bac_scan_com_data.size();i++)
	{
		for (int i=0;i<(int)m_bac_scan_com_data.size();i++)
		{
			if(m_bac_scan_com_data.at(i).device_id == g_bac_instance)
				find_exsit = true;
		}
	}
	if(find_exsit)
	{
		//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SVAE_CONFIG);
 		Post_Write_Message(g_bac_instance,CONNECTED_WITH_DEVICE,0,0,sizeof(Str_connected_point),BacNet_hwd ,_T("Connect with device"));
#if 0
		PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALL);
		CString temp_cstring;
		temp_cstring.Format(_T("Connected"),g_bac_instance);
		SetPaneString(1,temp_cstring);
		m_bac_main_tab.SetFocus();
#endif
	}
	else
	{
		SetPaneString(1,_T("Device is offline!"));
		MessageBox(_T("Device is offline!"),_T("Notice"),MB_OK | MB_ICONINFORMATION);;
	}
	//prevent when user doesn't click the bacnet device,the view also initial ,It's a waste of resource;
#if 1
	static bool initial_once = true;

	if(initial_once)
	{
		initial_once = false;
		Tab_Initial();
	}
#endif
}









typedef struct BACnet_Object_Property_Value_Own {
	BACNET_OBJECT_TYPE object_type;
	uint32_t object_instance;
	BACNET_PROPERTY_ID object_property;
	uint32_t array_index;
	BACNET_APPLICATION_DATA_VALUE value;
	HTREEITEM t_PropertyChild;
} BACNET_OBJECT_PROPERTY_VALUE_Own;

typedef struct _DEVICE_INFO
{
	uint32_t i_device_id;
	uint32_t i_vendor_id;
	uint32_t i_mac;
	HTREEITEM t_DeviceChild;
	vector	<BACnet_Object_Property_Value_Own> my_Property_value;

}DEVICE_INFO;



vector <_DEVICE_INFO> g_device_info;

//Str_in_point Private_data[100];
//int Private_data_length;
//HWND      m_input_dlg_hwnd;
//HWND      m_pragram_dlg_hwnd;


//extern  MSTP_Port;
volatile struct mstp_port_struct_t MSTP_Port;
static void Read_Properties(
    void)
{
    uint32_t device_id = 0;
    bool status = false;
    unsigned max_apdu = 0;
    BACNET_ADDRESS src;
    bool next_device = false;
    static unsigned index = 0;
    static unsigned property = 0;
    /* list of required (and some optional) properties in the
       Device Object
       note: you could just loop through
       all the properties in all the objects. */
	 const int object_props[] = {
	PROP_OBJECT_LIST//PROP_MODEL_NAME//PROP_OBJECT_LIST
	 };
    //const int object_props[] = {
    //    PROP_OBJECT_IDENTIFIER,
    //    PROP_OBJECT_NAME,
    //    PROP_OBJECT_TYPE,
    //    PROP_SYSTEM_STATUS,
    //    PROP_VENDOR_NAME,
    //    PROP_VENDOR_IDENTIFIER,
    //    PROP_MODEL_NAME,
    //    PROP_FIRMWARE_REVISION,
    //    PROP_APPLICATION_SOFTWARE_VERSION,
    //    PROP_PROTOCOL_VERSION,
    //    PROP_PROTOCOL_SERVICES_SUPPORTED,
    //    PROP_PROTOCOL_OBJECT_TYPES_SUPPORTED,
    //    PROP_MAX_APDU_LENGTH_ACCEPTED,
    //    PROP_SEGMENTATION_SUPPORTED,
    //    PROP_LOCAL_TIME,
    //    PROP_LOCAL_DATE,
    //    PROP_UTC_OFFSET,
    //    PROP_DAYLIGHT_SAVINGS_STATUS,
    //    PROP_APDU_SEGMENT_TIMEOUT,
    //    PROP_APDU_TIMEOUT,
    //    PROP_NUMBER_OF_APDU_RETRIES,
    //    PROP_TIME_SYNCHRONIZATION_RECIPIENTS,
    //    PROP_MAX_MASTER,
    //    PROP_MAX_INFO_FRAMES,
    //    PROP_DEVICE_ADDRESS_BINDING,
    //    /* note: PROP_OBJECT_LIST is missing cause
    //       we need to get it with an index method since
    //       the list could be very large */
    //    /* some proprietary properties */
    //    514, 515,
    //    /* end of list */
    //    -1
    //};

    if (address_count()) {
        if (address_get_by_index(index, &device_id, &max_apdu, &src)) {
            if (object_props[property] < 0)
                next_device = true;
            else {
                /*status*/ g_invoke_id= Send_Read_Property_Request(device_id,  /* destination device */
                    OBJECT_DEVICE, device_id, (BACNET_PROPERTY_ID)object_props[property],
                    BACNET_ARRAY_ALL);
              //  if (status)	Fance
              //      property++;
            }
        } else
            next_device = true;
        if (next_device) {
            next_device = false;
           // index++;Fance
            if (index >= MAX_ADDRESS_CACHE)
                index = 0;
            property = 0;
        }
    }

    return;
}
//uint32_t g_bac_instance;
uint32_t g_vendor_id;
//uint32_t g_mac;
HTREEITEM FirstChild ;
HTREEITEM SecondChild ;
HTREEITEM DeviceChild;





/*
 * This is called when we receive a private transfer packet.
 * We parse the data, send the private part for processing and then send the
 * response which the application generates.
 * If there are any BACnet level errors we send an error response from here.
 * If there are any application level errors they will be packeged up in the
 * response block which we send back to the originator of the request.
 *
 */



 






#if 0
void Localhandler_read_property_ack(
	uint8_t * service_request,
	uint16_t service_len,
	BACNET_ADDRESS * src,
	BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data)
{
	int len = 0;
	BACNET_READ_PROPERTY_DATA data;

	(void) src;
	(void) service_data;        /* we could use these... */
	len = rp_ack_decode_service_request(service_request, service_len, &data);
#if 0
	fprintf(stderr, "Received Read-Property Ack!\n");
#endif
	if (len > 0)
	{
		local_rp_ack_print_data(&data);
		::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,WM_COMMAND_WHO_IS,NULL);
	}
}
#endif

#if 0
static void LocalIAmHandler(
    uint8_t * service_request,
    uint16_t service_len,
    BACNET_ADDRESS * src)
{
	
    int len = 0;
    uint32_t device_id = 0;
    unsigned max_apdu = 0;
    int segmentation = 0;
    uint16_t vendor_id = 0;

    (void) src;
    (void) service_len;
    len =  iam_decode_service_request(service_request, &device_id, &max_apdu,
        &segmentation, &vendor_id);




    fprintf(stderr, "Received I-Am Request");
    if (len != -1) 
	{
        fprintf(stderr, " from %u!\n", device_id);
        address_add(device_id, max_apdu, src);
    } else
        fprintf(stderr, "!\n");

	if(src->mac_len==1)
		temp_mac.Format(_T("%d"),src->mac[0]);

	temp_device_id.Format(_T("%d"),device_id);
	temp_vendor_id.Format(_T("%d"),vendor_id);
	g_bac_instance =device_id;
	g_mac = _wtoi(temp_mac);

	TRACE(_T("Find ") + temp_device_id +_T("  ") + temp_mac + _T("\r\n"));

	::PostMessage(BacNet_hwd,WM_FRESH_CM_LIST,WM_COMMAND_WHO_IS,NULL);
	return;

}
#endif


LRESULT CDialogCM5_BacNet::Fresh_UI(WPARAM wParam,LPARAM lParam)
{
	int command_type = wParam;
	int button_click = 0;
	CString temp_cs;
	int temp_year;
	CTime	TimeTemp;
	switch(command_type)
	{
	case WM_COMMAND_WHO_IS:
		temp_cs.Format(_T("%d"),g_bac_instance);
		GetDlgItem(IDC_STATIC_CM_DEVICE_ID)->SetWindowTextW(temp_cs);
		temp_cs.Format(_T("%d"),g_mac);
		GetDlgItem(IDC_STATIC_CM5_MAC)->SetWindowTextW(temp_cs);
		Station_NUM = g_mac;
#if 0
		/*((CStatic *)*/GetDlgItem(IDC_STATIC_CM_DEVICE_ID)->SetWindowTextW(bac_cs_device_id);
		g_bac_instance = _wtoi(bac_cs_device_id);
		/*((CStatic *)*/GetDlgItem(IDC_STATIC_CM5_MAC)->SetWindowTextW(bac_cs_mac);
		bac_gloab_panel = _wtoi(bac_cs_mac);
		Station_NUM = bac_gloab_panel;;
		/*((CStatic *)*/GetDlgItem(IDC_STATIC_CM5_VENDOR_ID)->SetWindowTextW(bac_cs_vendor_id);
#endif
		break;
	case TIME_COMMAND:
		if(Device_time.year<2000)
			temp_year = Device_time.year + 2000;
		TimeTemp = CTime(temp_year,Device_time.month,Device_time.dayofmonth,Device_time.ti_hour,Device_time.ti_min,Device_time.ti_sec);
		

		m_cm5_time_picker.SetFormat(_T("HH:mm"));
		m_cm5_time_picker.SetTime(&TimeTemp);

		//m_cm5_date_picker.SetFormat(_T("YY/MM/DD"));
		m_cm5_date_picker.SetTime(&TimeTemp);
		break;;
	case MENU_CLICK:
		button_click = lParam;
		if(button_click == WRITEPRGFLASH_COMMAND)
		{
			WriteFlash();
		}
		else
		{
			tsm_free_all_invoke_id();//每次点击的时候都将所有INVOKE ID 清零;
			Show_Wait_Dialog_And_SendMessage(button_click);
		}
		break;
	case TYPE_SVAE_CONFIG:
		tsm_free_all_invoke_id();//每次点击的时候都将所有INVOKE ID 清零;
		Show_Wait_Dialog_And_SendMessage(TYPE_SVAE_CONFIG);
		break;
	default: 
		break;
	}

	
	return 0;
}
#if 0
DWORD WINAPI   CDialogCM5_BacNet::CM5_UI(LPVOID lpVoid)
{
	CDialogCM5_BacNet * mparent = (CDialogCM5_BacNet *)lpVoid;
	while(1)
	{
		Sleep(500);
		bool connect_results = Get_MSTP_Connect_Status();
		static bool Old_connect_results = false;


			if(Old_connect_results != connect_results)//Prevent repeatedly to redraw the Static.
			{
				if(connect_results)
				{

					SetPaneString(BAC_SHOW_CONNECT_RESULTS,_T("Connected"));

					//mparent->m_bac_static_status.SetWindowTextW(_T("Connected"));
					//mparent->m_bac_static_status.textColor(RGB(0,0,255));
					//mparent->m_bac_static_status.bkColor(RGB(0,255,0));


				}
				else
				{
					SetPaneString(BAC_SHOW_CONNECT_RESULTS,_T("Disconnected"));
					//mparent->m_bac_static_status.SetWindowTextW(_T("Disconnected"));
					//mparent->m_bac_static_status.textColor(RGB(255,255,255));
					//mparent->m_bac_static_status.bkColor(RGB(255,0,0));

				}
			}

			/*((CStatic *)*/mparent->GetDlgItem(IDC_STATIC_CM_DEVICE_ID)->SetWindowTextW(bac_cs_device_id);
			/*((CStatic *)*/mparent->GetDlgItem(IDC_STATIC_CM5_MAC)->SetWindowTextW(bac_cs_mac);
			/*((CStatic *)*/mparent->GetDlgItem(IDC_STATIC_CM5_VENDOR_ID)->SetWindowTextW(bac_cs_vendor_id);
			g_bac_instance = _wtoi(bac_cs_device_id);
			bac_gloab_panel = _wtoi(bac_cs_mac);

		Old_connect_results = connect_results;

	}
	return 0;
}
#endif
#if 0
DWORD WINAPI   CDialogCM5_BacNet::MSTP_Receive(LPVOID lpVoid)
{
	BACNET_ADDRESS src = {0};
	uint16_t pdu_len;
	CDialogCM5_BacNet *mparent = (CDialogCM5_BacNet *)lpVoid;
	uint8_t Rx_Buf[MAX_MPDU] = { 0 };
	while(mparent->m_MSTP_THREAD)
	{
		pdu_len = dlmstp_receive(&src, &Rx_Buf[0], MAX_MPDU, INFINITE);
		if(pdu_len==0)
			continue;
		npdu_handler(&src, &Rx_Buf[0], pdu_len);
		//CString TEMP1;
		//TEMP1.Format("%s",Rx_Buf);
		//	AfxMessageBox(TEMP1);
	}
	return 0;
}
#endif
#if 0
static void Init_Service_Handlers(
	void)
{
	Device_Init(NULL);

	/* we need to handle who-is to support dynamic device binding */
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_WHO_IS, handler_who_is);
	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_I_AM, LocalIAmHandler);



	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_PRIVATE_TRANSFER,local_handler_conf_private_trans_ack);
	//apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,Read_Property_feed_back);

	apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,	Localhandler_read_property_ack);
	/* set the handler for all the services we don't implement */
	/* It is required to send the proper reject message... */
	apdu_set_unrecognized_service_handler_handler
		(handler_unrecognized_service);
	/* we must implement read property - it's required! */
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROPERTY,
		handler_read_property);
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_PROP_MULTIPLE,
		handler_read_property_multiple);
	/* handle the data coming back from confirmed requests */
	//   apdu_set_confirmed_ack_handler(SERVICE_CONFIRMED_READ_PROPERTY,handler_read_property_ack);
#if defined(BACFILE)
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_READ_FILE,
		handler_atomic_read_file);
#endif
	apdu_set_confirmed_handler(SERVICE_CONFIRMED_SUBSCRIBE_COV,
		handler_cov_subscribe);

////#if 0
////	/* Adding these handlers require the project(s) to change. */
////#if defined(BACFILE)
////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_ATOMIC_WRITE_FILE,
////		handler_atomic_write_file);
////#endif
////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_READ_RANGE,
////		handler_read_range);
////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_REINITIALIZE_DEVICE,
////		handler_reinitialize_device);
////	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_UTC_TIME_SYNCHRONIZATION,
////		handler_timesync_utc);
////	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_TIME_SYNCHRONIZATION,
////		handler_timesync);
////	apdu_set_unconfirmed_handler(SERVICE_UNCONFIRMED_COV_NOTIFICATION,
////		handler_ucov_notification);
////	/* handle communication so we can shutup when asked */
////	apdu_set_confirmed_handler(SERVICE_CONFIRMED_DEVICE_COMMUNICATION_CONTROL,
////		handler_device_communication_control);
////#endif
}
#endif
//void CDialogCM5_BacNet::Initial_List()
//{
//	long style;
//	style=GetWindowLong(m_device_list_info.m_hWnd,GWL_STYLE);
//	style&=~LVS_TYPEMASK;
//	style|=LVS_REPORT;	
//	style|=LVS_EX_CHECKBOXES;
//	//	style|=RC_CHKBOX_SINGLE;
//
//	m_device_list_info.SetExtendedStyle(style);
//	SetWindowLong(m_device_list_info.m_hWnd,GWL_STYLE,style);//list_infomation.m_hWnd´
//	DWORD dwstyle=m_device_list_info.GetExtendedStyle();
//	dwstyle|=LVS_EX_FULLROWSELECT; //
//	dwstyle|=LVS_EX_GRIDLINES;     //
//	m_device_list_info.SetExtendedStyle(dwstyle);
//	m_device_list_info.InsertColumn(0,_T("Device ID"),LVCFMT_CENTER,100);  
//	m_device_list_info.InsertColumn(1,_T("MAC"),LVCFMT_CENTER,200);
//	m_device_list_info.InsertColumn(2,_T("Vendor ID"),LVCFMT_CENTER,200);
//	m_device_list_info.SetTextColor(RGB(0,0,255));
//	m_device_list_info.SetCheckboxeStyle(RC_CHKBOX_SINGLE);
//	//m_list_control.SortItems(0, FALSE); // sort the 1st column, ascending
//	m_device_list_info.SetSortable(FALSE);
//}










//void CDialogCM5_BacNet::OnBnClickedButton1()
//{
//	// TODO: Add your control notification handler code here
//	//WritePrivateData();
//}



void CDialogCM5_BacNet::Show_Wait_Dialog_And_SendMessage(int read_list_type)
{
	bac_read_which_list = read_list_type;
	if(WaitDlg==NULL)
	{
		if(bac_read_which_list == BAC_READ_SVAE_CONFIG)
		{
			WaitDlg = new BacnetWait((int)2);//如果是保存为ini文件 就要读取全部的data;
		}
		else
		{
			WaitDlg = new BacnetWait((int)0);
		}
		WaitDlg->Create(IDD_DIALOG_BACNET_WAIT,this);
		WaitDlg->ShowWindow(SW_SHOW);

		RECT RECT_SET1;
		GetWindowRect(&RECT_SET1);
		//	GetClientRect(&RECT_SET1);
		//WaitDlg->MoveWindow(RECT_SET1.left+100,RECT_SET1.bottom-200,560/*RECT_SET1.left+270*//*RECT_SET1.right/2+20*/,100);
		WaitDlg->MoveWindow(RECT_SET1.left+100,RECT_SET1.top+400,560/*RECT_SET1.left+270*//*RECT_SET1.right/2+20*/,100);
	}

	//::PostMessage(BacNet_hwd,WM_SEND_OVER,0,0);
	if(hwait_thread==NULL)
	{
		hwait_thread =CreateThread(NULL,NULL,Send_read_Command_Thread,this,NULL, NULL);

	}
}



static int resend_count=0;
DWORD WINAPI  CDialogCM5_BacNet::Send_read_Command_Thread(LPVOID lpVoid)
{
	CDialogCM5_BacNet *pParent = (CDialogCM5_BacNet *)lpVoid;
	bac_read_all_results = false;
	for (int i=0; i<BAC_MONITOR_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_MONITOR_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG)|| (bac_read_which_list ==BAC_READ_ALL_LIST))
		//if(bac_read_which_list == BAC_READ_MONITOR_LIST)
		{
			Bacnet_Refresh_Info.Read_Monitor_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].invoke_id = -1;
			bac_monitor_read_results = 0;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_SCREEN_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_SCREEN_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG)|| (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			Bacnet_Refresh_Info.Read_Screen_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Screen_Info[i].invoke_id = -1;
			bac_screen_read_results = 0;
			Bacnet_Refresh_Info.Read_Screen_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_TIME_COMMAND) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Time_Command[i].command = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Time_Command[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Time_Command[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id = -1;
			bac_time_command_read_results = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].timeout_count = 0;
		}
	}

	for(int i=0;i<BAC_WEEKLY_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_WEEKLY_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Weekly_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].resend_count = 0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id = -1;
			bac_weekly_read_results = 0;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].timeout_count = 0;
		}
	}

	for(int i=0;i<BAC_ANNUAL_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_ANNUAL_LIST) ||(bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Annual_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].resend_count = 0;

			Bacnet_Refresh_Info.Read_Annual_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id = -1;

			bac_annual_read_results = 0;
			Bacnet_Refresh_Info.Read_Annual_Info[i].timeout_count = 0;
		}
	}
	
	for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
	{

		if((bac_read_which_list == BAC_READ_PROGRAMCODE_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Programcode_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].resend_count = 0;

			Bacnet_Refresh_Info.Read_Programcode_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].invoke_id = -1;
			bac_programcode_read_results = 0;
			//bac_program_read_results = 0;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_WEEKLYCODE_GOUP;i++)
	{

		if((bac_read_which_list == BAC_READ_WEEKLTCODE_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].resend_count = 0;

			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].invoke_id = -1;
			bac_weeklycode_read_results = 0;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_PROGRAM_GROUP;i++)
	{

		if((bac_read_which_list == BAC_READ_PROGRAM_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Program_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Program_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Program_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Program_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Program_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Program_Info[i].resend_count = 0;

			Bacnet_Refresh_Info.Read_Program_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id = -1;

			bac_program_read_results = 0;
			Bacnet_Refresh_Info.Read_Program_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_INPUT_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_INPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Input_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Input_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Input_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].resend_count = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id = -1;
			bac_input_read_results = 0;
			Bacnet_Refresh_Info.Read_Input_Info[i].timeout_count = 0;

		}
	}
	for (int i=0;i<BAC_ANNUALCODE_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_ANNUALCODE_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].resend_count = 0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].invoke_id = -1;
			bac_annualcode_read_results = 0;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].timeout_count = 0;

		}
	}

	for (int i=0;i<BAC_OUTPUT_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_OUTPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Output_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Output_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Output_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].resend_count = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id = -1;
			bac_output_read_results = 0;
			Bacnet_Refresh_Info.Read_Output_Info[i].timeout_count = 0;

		}
	}

	for (int i=0;i<BAC_VARIABLE_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_VARIABLE_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_Variable_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id = -1;
			Bacnet_Refresh_Info.Read_Variable_Info[i].resend_count = 0;
			bac_variable_read_results = 0;
			Bacnet_Refresh_Info.Read_Variable_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_CONTROLLER_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG)|| (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			Bacnet_Refresh_Info.Read_Controller_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Controller_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id = -1;
			Bacnet_Refresh_Info.Read_Controller_Info[i].resend_count = 0;
			bac_controller_read_results = 0;
				Bacnet_Refresh_Info.Read_Controller_Info[i].timeout_count = 0;
		}
	}


	for (int i=0;i<BAC_ALARMLOG_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_ALARMLOG_LIST)/* || (bac_read_which_list ==BAC_READ_ALL_LIST)*/)
		{
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].invoke_id = -1;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].resend_count = 0;
			bac_alarmlog_read_results = 0;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].timeout_count = 0;
		}
	}


	for (int i=0;i<BAC_MONITOR_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_MONITOR_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG) || (bac_read_which_list ==BAC_READ_ALL_LIST))
		//if(bac_read_which_list == BAC_READ_MONITOR_LIST)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READMONITOR_T3000,(BAC_READ_GROUP_NUMBER)*i,
					3+(BAC_READ_GROUP_NUMBER)*i,
				//	2+(BAC_READ_GROUP_NUMBER)*i,
					sizeof(Str_monitor_point));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Monitor_Info[i].command = READMONITOR_T3000;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
			//Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance =2+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Monitor Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Monitor_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_SCREEN_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_SCREEN_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG)|| (bac_read_which_list ==BAC_READ_ALL_LIST))
		//if(bac_read_which_list == BAC_READ_SCREEN_LIST)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READSCREEN_T3000,(BAC_READ_GROUP_NUMBER)*i,
					3+(BAC_READ_GROUP_NUMBER)*i,
					sizeof(Control_group_point));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Screen_Info[i].command = READSCREEN_T3000;
			Bacnet_Refresh_Info.Read_Screen_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Screen_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Screen_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Screen_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Screens Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Screen_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Screen_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_TIME_COMMAND_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_TIME_COMMAND) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,
					TIME_COMMAND,0,
					0,
					sizeof(Time_block_mini));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Time_Command[i].command = TIME_COMMAND;
			Bacnet_Refresh_Info.Read_Time_Command[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Time_Command[i].start_instance = 0;
			Bacnet_Refresh_Info.Read_Time_Command[i].end_instance =0;
			Bacnet_Refresh_Info.Read_Time_Command[i].invoke_id = g_invoke_id;

			CString temp_cs_show;
			temp_cs_show.Format(_T("Task ID = %d. Read device time "),g_invoke_id);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

			//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,_T("1111111111111"));
			//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
		}
	}
	// TODO: Add your control notification handler code here
	for (int i=0;i<BAC_WEEKLY_GROUP;i++)
	{

		if((bac_read_which_list == BAC_READ_WEEKLY_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		//if(bac_read_which_list == BAC_READ_WEEKLY_LIST)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,
					READWEEKLYROUTINE_T3000,(BAC_READ_GROUP_NUMBER)*i,
					3+(BAC_READ_GROUP_NUMBER)*i,
					sizeof(Str_weekly_routine_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Weekly_Info[i].command = READWEEKLYROUTINE_T3000;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].invoke_id = g_invoke_id;

			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Weekly Routine Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_ANNUAL_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_ANNUAL_LIST) ||(bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		//if(bac_read_which_list == BAC_READ_ANNUAL_LIST)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,
					READANNUALROUTINE_T3000,(BAC_READ_GROUP_NUMBER)*i,
					3+(BAC_READ_GROUP_NUMBER)*i,
					sizeof(Str_annual_routine_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Annual_Info[i].command = READANNUALROUTINE_T3000;
			Bacnet_Refresh_Info.Read_Annual_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id = g_invoke_id;


			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Annual Routine Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


	for (int i=0;i<BAC_OUTPUT_GROUP;i++)
	{
		
		if((bac_read_which_list == BAC_READ_OUTPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,
											 READOUTPUT_T3000,(BAC_READ_GROUP_NUMBER)*i,
											 3+(BAC_READ_GROUP_NUMBER)*i,
											 sizeof(Str_out_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Output_Info[i].command = READOUTPUT_T3000;
			Bacnet_Refresh_Info.Read_Output_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Output_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Output_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id = g_invoke_id;

			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Output List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Output_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Output_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_ALARMLOG_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_ALARMLOG_LIST) /*|| (bac_read_which_list ==BAC_READ_ALL_LIST)*/)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READALARM_T3000,i,i,sizeof(Alarm_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
				//if(g_invoke_id<0)
				//	TRACE(_T("Input g_invoke_id = %d ,resend count = %d\r\n"),g_invoke_id,resend_count);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].command = READALARM_T3000;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].start_instance = i;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].end_instance = i;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].invoke_id = g_invoke_id;


			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Alarm Log List %d"),g_invoke_id,
				Bacnet_Refresh_Info.Read_AlarmLog_Info[i].start_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}



	for (int i=0;i<BAC_INPUT_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_INPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READINPUT_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_in_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
				//if(g_invoke_id<0)
				//	TRACE(_T("Input g_invoke_id = %d ,resend count = %d\r\n"),g_invoke_id,resend_count);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Input_Info[i].command = READINPUT_T3000;
			Bacnet_Refresh_Info.Read_Input_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Input_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Input_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id = g_invoke_id;


			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Input List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Input_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Input_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_VARIABLE_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_VARIABLE_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READVARIABLE_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_variable_point));
				//TRACE(_T("g_invoke_id = %d\r\n",g_invoke_id));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Variable_Info[i].command = READVARIABLE_T3000;
			Bacnet_Refresh_Info.Read_Variable_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance = 3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Variable_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Variable List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_PROGRAM_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_PROGRAM_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READPROGRAM_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_program_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Program_Info[i].command = READPROGRAM_T3000;
			Bacnet_Refresh_Info.Read_Program_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Program_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Program_Info[i].end_instance = 3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Program List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Program_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Program_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}
	if(bac_read_which_list == BAC_READ_PROGRAMCODE_LIST)
	{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				memset(mycode,0,1024);
				g_invoke_id = GetPrivateData(g_bac_instance,READPROGRAMCODE_T3000,program_list_line,program_list_line,m_Program_data.at(program_list_line).bytes + 10);
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].command = READPROGRAMCODE_T3000;
			Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].start_instance = program_list_line;
			Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].end_instance = program_list_line;
			Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Programcode List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].start_instance,
				Bacnet_Refresh_Info.Read_Programcode_Info[program_list_line].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
	}
	else if(bac_read_which_list ==TYPE_SVAE_CONFIG)
	{
		for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
		{
				int resend_count = 0;
				do 
				{
					resend_count ++;
					if(resend_count>RESEND_COUNT)
						goto myend;
					g_invoke_id = GetPrivateData(g_bac_instance,READPROGRAMCODE_T3000,i,i,m_Program_data.at(i).bytes + 10);
					Sleep(SEND_COMMAND_DELAY_TIME);
				} while (g_invoke_id<0);

				Bacnet_Refresh_Info.Read_Programcode_Info[i].command = READPROGRAMCODE_T3000;
				Bacnet_Refresh_Info.Read_Programcode_Info[i].device_id = g_bac_instance;
				Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance = i;
				Bacnet_Refresh_Info.Read_Programcode_Info[i].end_instance = i;
				Bacnet_Refresh_Info.Read_Programcode_Info[i].invoke_id = g_invoke_id;
				CString temp_cs;
				temp_cs.Format(_T("Task ID = %d. Read Programcode List Item From %d to %d "),g_invoke_id,
					Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance,
					Bacnet_Refresh_Info.Read_Programcode_Info[i].end_instance);
				Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}
#if 0
	for (int i=0;i<BAC_PROGRAMCODE_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_PROGRAMCODE_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG))
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READPROGRAMCODE_T3000,i,i,m_Program_data.at(i).bytes + 10);
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Programcode_Info[i].command = READPROGRAMCODE_T3000;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance = i;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].end_instance = i;
			Bacnet_Refresh_Info.Read_Programcode_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Programcode List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Programcode_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Programcode_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}
#endif

	for (int i=0;i<BAC_CONTROLLER_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_CONTROLLER_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG)|| (bac_read_which_list ==BAC_READ_ALL_LIST))
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READCONTROLLER_T3000,(BAC_READ_GROUP_NUMBER)*i,3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_controller_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Controller_Info[i].command = READCONTROLLER_T3000;
			Bacnet_Refresh_Info.Read_Controller_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance = 3+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Controller_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Controller List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


	if(bac_read_which_list == BAC_READ_WEEKLTCODE_LIST)
	{
		int resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto myend;
			g_invoke_id = GetPrivateData(g_bac_instance,READTIMESCHEDULE_T3000,weekly_list_line,weekly_list_line,WEEKLY_SCHEDULE_SIZE);
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].command = READTIMESCHEDULE_T3000;
		Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].device_id = g_bac_instance;
		Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].start_instance = weekly_list_line;
		Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].end_instance = weekly_list_line;
		Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].invoke_id = g_invoke_id;
		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Read annuale Schedule time List Item From %d to %d "),g_invoke_id,
			Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].start_instance,
			Bacnet_Refresh_Info.Read_Weeklycode_Info[weekly_list_line].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
	}
	else if(bac_read_which_list ==TYPE_SVAE_CONFIG)
	{
		for (int i=0;i<BAC_WEEKLYCODE_GOUP;i++)
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READTIMESCHEDULE_T3000,i,i,WEEKLY_SCHEDULE_SIZE);
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].command = READTIMESCHEDULE_T3000;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].start_instance = i;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].end_instance = i;
			Bacnet_Refresh_Info.Read_Weeklycode_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Weekly Schedule time List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Weeklycode_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Weeklycode_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


	if(bac_read_which_list == BAC_READ_ANNUALCODE_LIST)
	{
		int resend_count = 0;
		do 
		{
			resend_count ++;
			if(resend_count>RESEND_COUNT)
				goto myend;
			g_invoke_id = GetPrivateData(g_bac_instance,READANNUALSCHEDULE_T3000,annual_list_line,annual_list_line,ANNUAL_CODE_SIZE);
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);

		Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].command = READANNUALSCHEDULE_T3000;
		Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].device_id = g_bac_instance;
		Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].start_instance = annual_list_line;
		Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].end_instance = annual_list_line;
		Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].invoke_id = g_invoke_id;
		CString temp_cs;
		temp_cs.Format(_T("Task ID = %d. Read Annual day List Item From %d to %d "),g_invoke_id,
			Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].start_instance,
			Bacnet_Refresh_Info.Read_Annualcode_Info[annual_list_line].end_instance);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
	}
	else if(bac_read_which_list ==TYPE_SVAE_CONFIG)
	{
		for (int i=0;i<BAC_ANNUALCODE_GROUP;i++)
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READANNUALSCHEDULE_T3000,i,i,ANNUAL_CODE_SIZE);
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Annualcode_Info[i].command = READTIMESCHEDULE_T3000;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].start_instance = i;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].end_instance = i;
			Bacnet_Refresh_Info.Read_Annualcode_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Annual day time List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Annualcode_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Annualcode_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


		hwait_thread = NULL;

	return 0;
		//::PostMessage(BacNet_hwd,WM_SEND_OVER,0,0);
myend:	hwait_thread = NULL;
		AfxMessageBox(_T("Send Command Timeout!!!!!!!!!!"));
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,0,0);
	return 0;
}


void CDialogCM5_BacNet::WriteFlash()
{
	// TODO: Add your control notification handler code here
	//WRITEPRGFLASH_COMMAND
	int resend_count = 0;
	do 
	{
		resend_count ++;
		if(resend_count>10)
			break;
		g_invoke_id = GetPrivateData(g_bac_instance,WRITEPRGFLASH_COMMAND,0,0,0);
		Sleep(100);
	} while (g_invoke_id<0);

	if(g_invoke_id>0)
	{
		CString temp_cs_show;
		temp_cs_show.Format(_T("Task ID = %d. Write into flash "),g_invoke_id);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);
	}


	//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);
}








void CDialogCM5_BacNet::OnBnClickedButtonBacTest()
{
	// TODO: Add your control notification handler code here

	int	resend_count = 0;
	do 
	{
		resend_count ++;
		if(resend_count>20)
			return;
		g_invoke_id = GetPrivateData(g_bac_instance,
			TIME_COMMAND,0,
			0,
			sizeof(Time_block_mini));
		Sleep(200);
	} while (g_invoke_id<0);

	CString temp_cs_show;
	temp_cs_show.Format(_T("Task ID = %d. Read time "),g_invoke_id);
	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

	//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd);


}


void CDialogCM5_BacNet::OnBnClickedBacEnableEditTime()
{
	// TODO: Add your control notification handler code here
	//g_invoke_id = GetPrivateData(
	//	177,
	//	GETSERIALNUMBERINFO, 
	//	0,
	//	0,
	//	sizeof(Str_Serial_info));
	//if(g_invoke_id<0)
	//	MessageBox(_T("111"));
#if 1

	static bool edit_status = false;
	if(edit_status == false)
	{
		edit_status = true;
		m_cm5_date_picker.EnableWindow(1);
		m_cm5_time_picker.EnableWindow(1);
		GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(1);
		GetDlgItem(IDC_BAC_ENABLE_EDIT_TIME)->SetWindowTextW(_T("Disable Edit"));
	}
	else
	{
		edit_status = false;
		m_cm5_date_picker.EnableWindow(0);
		m_cm5_time_picker.EnableWindow(0);
		GetDlgItem(IDC_BAC_SYNC_LOCAL_PC)->EnableWindow(0);
		GetDlgItem(IDC_BAC_ENABLE_EDIT_TIME)->SetWindowTextW(_T("Enable Edit"));
	}
#endif
}

void CDialogCM5_BacNet::Get_Time_Edit_By_Control()
{
	CTime temp_day;

	m_cm5_date_picker.GetTime(temp_day);
	Device_time.year = temp_day.GetYear();
	Device_time.month = temp_day.GetMonth();
	Device_time.dayofmonth = temp_day.GetDay();
	Device_time.dayofweek = temp_day.GetDayOfWeek() -1;
	Device_time.dayofyear = 1;

	
	CTime temp_time;
	m_cm5_time_picker.GetTime(temp_time);
	Device_time.ti_hour = temp_time.GetHour();
	Device_time.ti_min = temp_time.GetMinute();
	Device_time.ti_sec = temp_time.GetMinute();
	CString temp_task_info;
	temp_task_info.Format(_T("Write Device Time.Changed to %02d-%02d %02d:%02d  "),
		Device_time.month,Device_time.dayofmonth,
		Device_time.ti_hour,Device_time.ti_min);
	Post_Write_Message(g_bac_instance,RESTARTMINI_COMMAND,0,0,sizeof(Time_block_mini),this->m_hWnd,temp_task_info);
}

void CDialogCM5_BacNet::OnNMKillfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	Get_Time_Edit_By_Control();
	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(0);
	*pResult = 0;
}


void CDialogCM5_BacNet::OnNMKillfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	Get_Time_Edit_By_Control();
	//GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(0);
	*pResult = 0;
}




void CDialogCM5_BacNet::OnNMSetfocusDatePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(TRUE);
	*pResult = 0;
}


void CDialogCM5_BacNet::OnNMSetfocusTimePicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_BAC_WRITE_TIME)->EnableWindow(TRUE);
	*pResult = 0;
}


void CDialogCM5_BacNet::OnBnClickedBtnBacWriteTime()
{
	// TODO: Add your control notification handler code here

	int aaa = Get_MSTP_Connect_Status();
	TRACE(_T("Status is %d\r\n"),aaa);


}


void CDialogCM5_BacNet::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	bool connect_results = Get_MSTP_Connect_Status();
	static bool Old_connect_results = false;
	
	switch(nIDEvent)
	{
	case 1:
		if(Old_connect_results != connect_results)//Prevent repeatedly to redraw the Static.
		{
			if(connect_results)
			{
				SetPaneString(BAC_SHOW_CONNECT_RESULTS,_T("Connected"));
			}
			else
			{
				SetPaneString(BAC_SHOW_CONNECT_RESULTS,_T("Disconnected"));
			}
		}

		break;
	case 2:
		{
		if(!is_in_scan_mode)
			m_bac_scan_com_data.clear();
		if(g_bac_instance>0)
			Send_WhoIs_Global(-1, -1);
		}
		break;
	case 3:
		{
			if(AlarmWindow_Window!=NULL)
			{
				if(bac_show_alarm_window)//显示;
				{
					if(!(CBacnetAlarmWindow *)AlarmWindow_Window->IsWindowVisible())
					{
						AlarmWindow_Window->ShowWindow(SW_SHOW);
					}
				}
				else//Not show
				{
					if((CBacnetAlarmWindow *)AlarmWindow_Window->IsWindowVisible())
					{
						AlarmWindow_Window->ShowWindow(SW_HIDE);
					}
				}
			}
		}
		break;
	default:
		break;
	}
	Old_connect_results = connect_results;

	CFormView::OnTimer(nIDEvent);
}


void CDialogCM5_BacNet::OnTcnSelchangeBacMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int selected = m_bac_main_tab.GetCurSel();
	static int old_selected_item = WINDOW_PROGRAM;
	switch(old_selected_item)
	{
	case WINDOW_PROGRAM:
		((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Unreg_Hotkey();
		break;
	case WINDOW_SCREEN:
		((BacnetScreen*)pDialog[WINDOW_SCREEN])->Unreg_Hotkey();
		break;
	case WINDOW_WEEKLY:
		((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Unreg_Hotkey();
		break;
	case WINDOW_ANNUAL:
		((BacnetAnnualRoutine*)pDialog[WINDOW_ANNUAL])->Unreg_Hotkey();
		break;
	case WINDOW_MONITOR:
		((CBacnetMonitor*)pDialog[WINDOW_MONITOR])->Unreg_Hotkey();
		break;
	default:
		break;
	}

	for (int i=0;i<10;i++)
	{
		if(i==selected)
		{
			pDialog[i]->ShowWindow(SW_SHOW);
			switch(i)
			{
			case WINDOW_INPUT:
				//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_INPUT);
				((CBacnetInput *)pDialog[i])->Fresh_Input_List(NULL,NULL);
				g_hwnd_now = m_input_dlg_hwnd;
				break;
			case WINDOW_OUTPUT:
				//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_OUTPUT);
				((CBacnetOutput *)pDialog[i])->Fresh_Output_List(NULL,NULL);
				g_hwnd_now = m_output_dlg_hwnd;
					break;
			case WINDOW_VARIABLE:
				((CBacnetVariable *)pDialog[i])->Fresh_Variable_List(NULL,NULL);
				g_hwnd_now = m_variable_dlg_hwnd;
				break;
			case WINDOW_PROGRAM:
				((CBacnetProgram *)pDialog[i])->Fresh_Program_List(NULL,NULL);
				((CBacnetProgram*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_pragram_dlg_hwnd;
				break;
			case WINDOW_CONTROLLER:
				//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_CONTROLLER);
				((BacnetController *)pDialog[i])->Fresh_Controller_List(NULL,NULL);
				g_hwnd_now = m_controller_dlg_hwnd;
				break;
			case WINDOW_SCREEN:
				((BacnetScreen *)pDialog[i])->Fresh_Screen_List(NULL,NULL);
				((BacnetScreen*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_screen_dlg_hwnd;
				break;
			case WINDOW_WEEKLY:
				((BacnetWeeklyRoutine *)pDialog[i])->Fresh_Weekly_List(NULL,NULL);
				((BacnetWeeklyRoutine*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_weekly_dlg_hwnd;
				break;
			case WINDOW_ANNUAL:
				((BacnetAnnualRoutine *)pDialog[i])->Fresh_Annual_Routine_List(NULL,NULL);
				((BacnetAnnualRoutine*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_annual_dlg_hwnd;
				break;
			case WINDOW_MONITOR:
				((CBacnetMonitor *)pDialog[i])->Fresh_Monitor_List(NULL,NULL);
				((CBacnetMonitor *)pDialog[i])->Fresh_Monitor_Input_List(NULL,NULL);
				((CBacnetMonitor*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_monitor_dlg_hwnd;
				break;
			case WINDOW_ALARMLOG:
				//增加刷新list和改变当前window 的hwnd;
				PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALARMLOG);
				((CBacnetAlarmLog *)pDialog[i])->Fresh_Alarmlog_List(NULL,NULL);
				g_hwnd_now = m_alarmlog_dlg_hwnd;
				break;
			}
			
//			pDialog[i]->OnInitDialog();
		}
		else
		{
#if 0
			switch(i)
			{
			case WINDOW_PROGRAM:
				((CBacnetProgram*)pDialog[i])->Unreg_Hotkey();
				break;
			case WINDOW_SCREEN:
				((BacnetScreen*)pDialog[i])->Unreg_Hotkey();
				break;
			case WINDOW_WEEKLY:
				((BacnetWeeklyRoutine*)pDialog[i])->Unreg_Hotkey();
				break;
			case WINDOW_ANNUAL:
				((BacnetAnnualRoutine*)pDialog[i])->Unreg_Hotkey();
				break;
			case WINDOW_MONITOR:
				((CBacnetMonitor*)pDialog[i])->Unreg_Hotkey();
				break;
			}
#endif
			pDialog[i]->ShowWindow(SW_HIDE);
		}
	}
	if(old_selected_item!= selected)
		old_selected_item = selected;
	*pResult = 0;
}


BOOL CDialogCM5_BacNet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_TAB) 
	{
		m_cur_tab_sel = m_bac_main_tab.GetCurSel();
		m_cur_tab_sel = (++m_cur_tab_sel)%9;
		m_bac_main_tab.SetCurSel(m_cur_tab_sel);

		for (int i=0;i<9;i++)
		{
			if(i==m_cur_tab_sel)
			{
				pDialog[i]->ShowWindow(SW_SHOW);
				switch(i)
				{
				case WINDOW_INPUT:
					((CBacnetInput *)pDialog[i])->Fresh_Input_List(NULL,NULL);
					g_hwnd_now = m_input_dlg_hwnd;
					break;
				case WINDOW_OUTPUT:
					((CBacnetOutput *)pDialog[i])->Fresh_Output_List(NULL,NULL);
					g_hwnd_now = m_output_dlg_hwnd;
					break;
				case WINDOW_VARIABLE:
					((CBacnetVariable *)pDialog[i])->Fresh_Variable_List(NULL,NULL);
					g_hwnd_now = m_variable_dlg_hwnd;
					break;
				case WINDOW_PROGRAM:
					((CBacnetProgram *)pDialog[i])->Fresh_Program_List(NULL,NULL);
					((CBacnetProgram*)pDialog[i])->Reg_Hotkey();
					g_hwnd_now = m_pragram_dlg_hwnd;
					break;
				case WINDOW_CONTROLLER:
					((BacnetController *)pDialog[i])->Fresh_Controller_List(NULL,NULL);
					g_hwnd_now = m_controller_dlg_hwnd;
					break;
				case WINDOW_SCREEN:
					((BacnetScreen *)pDialog[i])->Fresh_Screen_List(NULL,NULL);
					((BacnetScreen*)pDialog[i])->Reg_Hotkey();
					g_hwnd_now = m_screen_dlg_hwnd;
					break;
				case WINDOW_WEEKLY:
					((BacnetWeeklyRoutine *)pDialog[i])->Fresh_Weekly_List(NULL,NULL);
					((BacnetWeeklyRoutine*)pDialog[i])->Reg_Hotkey();
					g_hwnd_now = m_weekly_dlg_hwnd;
					break;
				case WINDOW_ANNUAL:
					((BacnetAnnualRoutine *)pDialog[i])->Fresh_Annual_Routine_List(NULL,NULL);
					((BacnetAnnualRoutine*)pDialog[i])->Reg_Hotkey();
					g_hwnd_now = m_annual_dlg_hwnd;
					break;
				case WINDOW_MONITOR:
					((CBacnetMonitor *)pDialog[i])->Fresh_Monitor_List(NULL,NULL);
					((CBacnetMonitor *)pDialog[i])->Fresh_Monitor_Input_List(NULL,NULL);
					((CBacnetMonitor*)pDialog[i])->Reg_Hotkey();
					g_hwnd_now = m_monitor_dlg_hwnd;
					break;
				}

				//			pDialog[i]->OnInitDialog();
			}
			else
			{
				switch(i)
				{
				case WINDOW_PROGRAM:
					((CBacnetProgram*)pDialog[i])->Unreg_Hotkey();
					break;
				case WINDOW_SCREEN:
					((BacnetScreen*)pDialog[i])->Unreg_Hotkey();
					break;
				case WINDOW_WEEKLY:
					((BacnetWeeklyRoutine*)pDialog[i])->Unreg_Hotkey();
					break;
				case WINDOW_ANNUAL:
					((BacnetAnnualRoutine*)pDialog[i])->Unreg_Hotkey();
					break;
				case WINDOW_MONITOR:
					((CBacnetMonitor*)pDialog[i])->Unreg_Hotkey();
					break;
				}
				pDialog[i]->ShowWindow(SW_HIDE);
			}
		}

		return TRUE;
	}
	else if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_DOWN)
	{
		m_cur_tab_sel = m_bac_main_tab.GetCurSel();
		pDialog[m_cur_tab_sel]->SetFocus();
	}

	return CFormView::PreTranslateMessage(pMsg);
}
