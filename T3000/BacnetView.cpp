// DialogCM5_BacNet.cpp : implementation file
// DialogCM5 Bacnet programming by Fance 2013 05 01
/*
2015 - 04 - 09
Update by Fance
1>> Load T3 serial register description . It can show in Minipanel remote point normally.
2>> All label can not have a same name . inputs label outputs label ...

2015 - 04 -08
Update by Fance
1>> Fix Screen Edit Label window , ICON name is not show correctly; GRP graphic not show correctly.


2015 - 04 - 03
Update by Fance
1>> Fix Monitor inputs can't show normally.
2>> Change the point type of ScreenEdit window.
3>> Screen window now can use left double click to full screen or cancel it.
4>> Program full label disappear when write then last item's full label.

2015 - 04 - 02
Update by Fance
1>>  Fixed the label in screen window can't show normally in the first few seconds. 
2>>  When add or edit label in screen window ,it will write data into minipanel right now.Before we write into device when exit the window. 
3>>  Fixed the  monitor input list crash when user change focus to other application. 
4>>  Error handle with th label point . Before "VAR-3" "3--VAR" will make the t3000 crash.  

2015 - 03 - 18
Update by Fance
1. For minipanel , user can login ,different level can access different data. Such as graphic mode only access graphic data.
2. DYNDNS service.
3. sync time. time zone.
4. Change point type.


2015 - 03 - 18
Update by Fance
1.Bacnet monitor finished.
2.Building config support remote connection.
3.Remote building scan.


2015 - 01 - 17
Update by Fance
1. Merge code. Database change!

2015 - 01 - 15
Update by Fance
1. Change the config icon and bacnet setting icon;
2. Bacnet setting UI change. Panel number and panel name change to normal font.

2015 - 01 - 12
Update by Fance
1. Optimize Bacnet read and write config .If user click new menu to read or write , don't interrupt the task. 
2. When the user delete some device , and the interface is show this device , it will exit and show the default building.

2015 - 01 - 07
Update by Fance
1. Bacnet program code -> If STATION_NUM == Mainpanel == SubPanel  .  243-243-VAR10 ----> Show VAR10 .
2. Program code will read 400 bytes all the time , no matter judge by the size of the program.
3. Fix the bugs all the bacnet list can't response the item change message.

2015 - 01 - 04
Update by Fance
1. ScreenEdit now support  go back to the last screen one by one.
2. Delete the tab in bacnet main screen. Add status bar icon menu.
3. Save the product status in DB every 30 seconds.
4. Bacnet input , Calibration change to int16.

.....
.....
.....


2014 - 11 - 18
Update by Fance
1.Add Bacnet device user login feature.
2.Recode the Screen and label ,make the graphic more beautiful.

2014-07-18
Update by Fance
1. Improve the quality of communication. 
2. Improve the network scan feature.
3. T3000 will scan device via serial port with baud rate both “9600” and “19200”.Before we only use 19200.
4. Add feature user can add device not through the scan command.
5. Bacnet programming feature support both lower-case and upper-case now.
6. All the Bacnet list (input/output/…), it will auto scroll, when user press up or down , the list item will automatically up and down.
7. Hide the graphic useless button. Add button feature to load factory default.(For Bacnet device)
8. Improve code to support read TSTAT6 through Zigbee module. Now it read smoothly.


2014-07-03
Update by Fance
1.All the Listctrl, it will auto scroll,when user press up or down , the list item will automatically up and down.
2.Hide the graphic useless button.(Maurice required)

2014-06-19
Update by Fance
1.Control Basic Editor: When finished editing and the users' program has been sent successfully, there is no need to show the exit Y/N confirmation dialog. Just exit right out with the 'Esc' key. 
2.Control Basic Editor: Update the error messages after each send of the program. 
3.Device Discovery: Fixed misc bugs related to auto discovery of bacnet devices and changes in the IP address. Sync the changes properly with the database. 
4.On line status of the nodes and the lower connection status bar now synchronized better. 
5.When user changes the bacnet "Auto/Manual" field the "value" column will refresh instantly, before it was slow.
6. Match number of inputs of the controller to the hardware model, ie if it has 16 Inptus T3000 will show only 16 inputs for the LB model, 32inputs for the LB model and so on. 
7. Change the multi flash update interface, TBD : Auto flash and support pause and resume.
8. Multi-flash update tool can now correctly handle 64k and 128k thermostats. 
9. Add feature that user can access the remote bacnet device by internet at a remote IP address. 
10. Add the bacnet interface to show the device software version , hardware version ,boot loader version,and other information.





2014-04-22
Update by Fance
1.Auto adjust different device show different Unit type.Take CM5 for example .
  CM5 contains 10 internal analog inputs and 8 internal digital inputs,So the inputs1 to input10,the "range" column only show "Analog",inputs11 to inputs18 only show "Digital"


2014-04-21
Update by Fance
1.The analog trend logging feature has been added. Digital data still to be done. 
2.Support to open the USB device "Testo-345" ,and read it's data.
TBD: add a small GUI screen and integrate to trend logs and other features
3.Add advanced setup screen for bacnet devices: IP address and others. 
4.Fix the modbus DLL .If the device disconnected , the "read_one" and
"write_one" used to fail if the node goes off line. Now it will automatically reconnect to the device.
5.Add a screen to show the Zigbee network RSSI signal strength now. TBD: add graphical version of zigbee network. 
6.Fix the bacnet panel number, max before was 32, now supports up to 255nodes on each rs485/mstp subnet and 255 on the main bacnet IP network

2014-04-16
Update by Fance
1.Fix the modbus DLL .If the device disconnected , the  "read_one" and "write_one" function will error until you connect again.
2.Add a new setting interface in the bacnet view.
2014-04-11
Update by Fance
1.Get the monitor data from the bacnet device.
2.Draw the graphic on the screen.

2014-03-28
Update by Fance
1.Update the Graphic view class.Now all the position and parameter can changed more easier. 
2014-03-27
Update by Fance
1.Finished the function ,Open the Testo USB port and read the Testo-435 data.Put the two function into the Dll,named "DllFunction.cpp"
2.Finished the Zigbee RSSI List , all the data will show on the list normally;

2014-03-20
Update by Fance
1.Change the graphic static,from staticex to own draw.
2.Change the ud_str.h ,some of the monitor struct.
2014-03-19
Update by Fance
1.Update the progress bar ,change the position to the button of the view.
2.Add read monitor data function.
2014-03-18
Update by Fance
1.Net cable disconnected ,the scan result also show the last connected device . ->Fix it.
2.If the Co2 external sensor count is a invalid value , the CO2 Window will crash . ->Fix it.
2014-03-17
Update by Fance
1.Support read Zigbee Tstat.(Not finished)
2014-03-14
Update by Fance
1.Program IDE user interface , panel upper 32 can't send program command,
	eg:  IF 36-OUT1 >5 THEN 87-IN2 = 3   (This shows 36-OUT1 is not defind)
	Fix this problem.
2.About program TIME-ON , TIME-OFF INTERVAL command ,now it can use normally.
3.Debug window don't always create when user don't needed.It will create when the user press SHIFT + 'D'
2014-03-07
Update by Fance
1.Allow the user to change the IP address and subnet and gateway.
2.Optimize the building graphic , user can add label (inputs ,outputs,variable) and monitor this label on the building view.
3.The device of CO2 show a wrong firmware version. Fix it.
4.Fix part of code which may crash the T3000.
2014-02-21
Update by Fance
1.BacnetTstat background color ,white and gray interval.
2.ISP tool in T3000 can't work , because the port 4321 is already bind by the T3000 .Already fix the problem
3.Modify the debug window hot key to shift + D

2014-02-20
Update by Fance
1.Code the BacnetScreenEdit.cpp , user can add PIC file and label in the pic file.user can monitor the value in the graphic.
2.Fix the BacnetScreenEdit.cpp memory leak.
3.Fix the BacnetScreen.cpp . No longer show  Text mode.
4.Fix the Bacnet object instance , T3000 no longer has the static instance ,use a random value.

2014-02-10
	Update by Fance
	1.TreeView update , support refresh different  com and network device.the Treeview of the device no longer
	  display a error status.

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
#include "globle_function.h"

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
#include "BacnetTstat.h"
#include "BacnetSetting.h"
#include "BacnetCustomerDigitalRange.h"
#include "BacnetUserlogin.h"
#include "BacnetUserConfig.h"
#include "MainFrm.h"
#include "BacnetScreenEdit.h"
#include "BacnetRemotePoint.h"
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
CBacnetTstat *Tstat_Window = NULL;
CBacnetSetting * Setting_Window = NULL;
CBacnetUserlogin* User_Login_Window = NULL;
CBacnetRemotePoint* Remote_Point_Window = NULL;

extern CBacnetScreenEdit * ScreenEdit_Window;
bool m_is_remote_device = false;
HANDLE hconnect_modbus_thread = NULL;
static int old_selected_item = WINDOW_PROGRAM; // 用于保存 上一个选中的 TAB; 用于 快捷键的操作;
extern CBacnetAlarmWindow * AlarmWindow_Window;
extern bool is_in_scan_mode;
extern char mycode[2000];
int click_resend_time = 0;//当点击的时候，要切换device时 发送whois的次数;
// CDialogCM5_BacNet
CString IP_ADDRESS;
_Refresh_Info Bacnet_Refresh_Info;
CString remote_ip_address;
int bacnet_view_number = TYPE_INPUT;
extern CString SaveConfigFilePath; //用来将资料存放至数据库，临时文件的存放目录;
extern SOCKET my_sokect;
extern bool show_user_list_window ;
//#define WM_SEND_OVER     WM_USER + 1287
// int m_Input_data_length;
extern void  init_info_table( void );
extern void Init_table_bank();


IMPLEMENT_DYNCREATE(CDialogCM5_BacNet, CFormView)

CDialogCM5_BacNet::CDialogCM5_BacNet()
	: CFormView(CDialogCM5_BacNet::IDD)
	, m_bac_obtain_ip_mode(0)
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


	DDX_Control(pDX, IDC_BAC_MAINTAB, m_bac_main_tab);
}

BEGIN_MESSAGE_MAP(CDialogCM5_BacNet, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDialogCM5_BacNet::OnBnClickedButtonTest)
	ON_MESSAGE(WM_FRESH_CM_LIST,Fresh_UI)
	ON_MESSAGE(MY_RESUME_DATA, AllMessageCallBack)
	ON_MESSAGE(WM_DELETE_NEW_MESSAGE_DLG,BacnetView_Message_Handle)	

	
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
LRESULT CDialogCM5_BacNet::BacnetView_Message_Handle(WPARAM wParam,LPARAM lParam)
{
	int message_type = wParam;
	int n_lparam = (int)lParam;
	switch(message_type)
	{
	case 0:
		{
		if(WaitDlg!=NULL)
		{
			Sleep(50);
			if(WaitDlg)
				delete WaitDlg;
			WaitDlg = NULL;
		}
			if(bac_read_which_list == BAC_READ_ALL_LIST)
			{
				if(bac_read_all_results)
				{
					CString temp_file;
					CString temp_serial;
					temp_serial.Format(_T("%d.prg"),g_selected_serialnumber);
					temp_file = g_achive_folder + _T("\\") + temp_serial;
					//SaveBacnetConfigFile(temp_file);
					SaveBacnetConfigFile_Cache(temp_file);
				}

					
				return 0;
			}

			if(bac_read_which_list == BAC_READ_SVAE_CONFIG)
			{
				if(bac_read_all_results)
					::PostMessageW(MainFram_hwd,MY_BAC_CONFIG_READ_RESULTS,1,NULL);
				else
				{
					TRACE(_T("Failed\r\n"));
				}
				read_write_bacnet_config = false;
				return 0;
			}
			//if(bac_read_which_list != BAC_READ_USER_LOGIN_INFO)
			//{
			//	m_bac_main_tab.SetFocus();
			//}


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
						if(Input_Window->IsWindowVisible())
							::PostMessage(m_input_dlg_hwnd,WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
					}
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//Input_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_INPUT);
					//Input_Window->m_input_list.SetFocus();
				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Inputs list read time out!"));
					//MessageBox(_T("Inputs list read time out!"));	
				}
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
						if(Output_Window->IsWindowVisible())
							::PostMessage(m_output_dlg_hwnd,WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
					}
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//Output_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_OUTPUT);
					//Output_Window->m_output_list.SetFocus();

				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Outputs list read time out!"));
					//MessageBox(_T("Outputs list read time out!"));
				}
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
						if(Variable_Window->IsWindowVisible())
							::PostMessage(m_variable_dlg_hwnd,WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
					}
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//Variable_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_VARIABLE);

				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Variable list read time out!"));
					//MessageBox(_T("Variable list read time out!"));
				}
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
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//Program_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_PROGRAM);

				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Program list read time out!"));
					//MessageBox(_T("Program list read time out!"));
				}
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
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//WeeklyRoutine_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_WEEKLY);
				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Weekly list read time out!"));
					//MessageBox(_T("Weekly list read time out!"));
				}
				return 0;
			}
			if(bac_read_which_list == BAC_READ_CUSTOMER_UNITS)
			{
				read_customer_unit = true;	//标志已经读过 单位了 不必再读，旧版本的设备 可能读不到;
				if(bac_customer_unit_read_results)
				{
					CBacnetCustomerDigitalRange Dlg;
					Dlg.DoModal();
				}
				return 0;
			}

			if(bac_read_which_list == BAC_READ_USER_LOGIN_INFO)
			{

				if((bac_user_login_read_results) && (show_user_list_window))
				{
					CBacnetUserConfig Dlg;
					Dlg.DoModal();
				}
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
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//AnnualRoutine_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_ANNUAL);
				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Annual Routine list read time out!"));
					//MessageBox(_T("Annual Routine list read time out!"));
				}
				return 0;
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
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//Controller_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_CONTROLLER);
				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Controller list read time out!"));
					//MessageBox(_T("Controller list read time out!"));
				}
				return 0;
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
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//Screen_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_SCREEN);
				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Screens list read time out!"));
					//MessageBox(_T("Screens list read time out!"));
				}
				return 0;
			}

			if(bac_read_which_list == BAC_READ_TSTAT_LIST)
			{
				if(bac_tstat_read_results)
				{
					if(Tstat_Window == NULL)
					{
						Tstat_Window = new CBacnetTstat;
						Tstat_Window->Create(IDD_DIALOG_BACNET_TSTAT,this);
					}
					else
					{
						::PostMessage(m_tstat_dlg_hwnd,WM_REFRESH_BAC_TSTAT_LIST,NULL,NULL);
					}
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//Tstat_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_TSTAT);

				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Tstat list read time out!"));
					//MessageBox(_T("Tstat list read time out!"));
				}
				return 0;
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
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//Monitor_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel(WINDOW_MONITOR);

				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Monitor list read time out!"));
					//MessageBox(_T("Monitor list read time out!"));
				}
				return 0;
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
					//for (int i=0;i<WINDOW_TAB_COUNT;i++)
					//{
					//	pDialog[i]->ShowWindow(SW_HIDE);
					//}
					//AlarmLog_Window->ShowWindow(SW_SHOW);
					//m_bac_main_tab.SetCurSel((WINDOW_ALARMLOG));
				}
				else
				{
					if(hwait_thread)
					{
						TerminateThread(hwait_thread,0);
						hwait_thread = NULL;
					}
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("AlarmLog read time out!"));
					//MessageBox(_T("AlarmLog read time out!"));
				}
				return 0;
			}



			if(bac_read_which_list == BAC_READ_GRAPHIC_LABEL_INFO)
			{
				if(bac_graphic_label_read_results)
				{
					need_read_bacnet_graphic_label_flag = false;
					bac_cm5_graphic = true;
					Screen_Window->Unreg_Hotkey();

					if(ScreenEdit_Window != NULL)
					{
						delete ScreenEdit_Window;
						ScreenEdit_Window = NULL;
					}
					ScreenEdit_Window = new CBacnetScreenEdit;
					ScreenEdit_Window->Create(IDD_DIALOG_BACNET_SCREENS_EDIT,this);	
					ScreenEdit_Window->ShowWindow(SW_SHOW);
				}
				else
				{
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read data time out!"));
					//MessageBox(_T("Read data time out!"));
				}

				return 0;
			}

			if(bac_read_which_list == BAC_READ_WEEKLTCODE_LIST)
			{
				if(bac_weeklycode_read_results)
				{
					CBacnetScheduleTime Dlg;
					Dlg.DoModal();
				}
				else
				{
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Weekly schedual time read time out!"));
					//MessageBox(_T("Weekly schedual time read time out!"));
				}

				return 0;
			}
								
			if(bac_read_which_list == BAC_READ_ANNUALCODE_LIST)
			{
				if(bac_annualcode_read_results)
				{
					AnnualRout_InsertDia Dlg;
					Dlg.DoModal();
				}
				else
				{
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Annual day time read time out!"));
					//MessageBox(_T("Annual day time read time out!"));
				}

				return 0;
			}


			if((bac_read_which_list == BAC_READ_BASIC_SETTING_COMMAND) || (bac_read_which_list == BAC_READ_ALL_LIST))
			{
				if(bac_basic_setting_read_results)
				{
					TRACE(_T("Read Setting success\r\n"));
					//PostMessage(WM_FRESH_CM_LIST,READ_SETTING_COMMAND,NULL);
				}
			}
			}
		break;
	case DELETE_WINDOW_MSG:
	//	m_bac_main_tab.SetFocus();
	//	m_bac_main_tab.SetCurSel(m_cur_tab_sel);
		break;
	case START_BACNET_TIMER:
		{
			DFTrace(_T("Connect to the device use the modbus ip and port success!"));
			if(m_is_remote_device)
			{
				Send_WhoIs_remote_ip(remote_ip_address);
			}
			else
			{
				Send_WhoIs_Global(g_bac_instance, g_bac_instance);
			}
			//MessageBox(_T("Test2"));

			SetTimer(4,500,NULL);
			click_resend_time = 6;
		}
		break;
	case CONNECT_TO_MODBUS_FAILED:
		{
			CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
			pFrame->m_pTreeViewCrl->turn_item_image(selected_tree_item ,false);
			MessageBox(_T("Connect to the device failed! No response from the TCP server!\
				\r\nPlease Check the connection!\r\n"),_T("Notice"),MB_OK | MB_ICONINFORMATION);;
		}
		break;
	case PASSWORD_OK_INITIAL_UI:
		{
			m_user_level = n_lparam;
			Initial_Some_UI(n_lparam);
		}
		break;
	case SHOW_PROGRAM_IDE:
		{
			if(bac_read_which_list == BAC_READ_PROGRAMCODE_LIST)
			{
				if(bac_programcode_read_results)
				{
					bac_read_which_list = -1;
					bac_programcode_read_results = false;
					CBacnetProgramEdit Dlg;
					Dlg.DoModal();
				}

				return 0;
			}
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
			{
				Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = true;
			}
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

		for (int i=0;i<BAC_BASIC_SETTING_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_BasicSetting_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_BasicSetting_Info[i].task_result = true;
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

		//for (int i=0;i<BAC_TSTAT_GROUP;i++)
		//{
		//	if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Tstat_Info[i].invoke_id)
		//		Bacnet_Refresh_Info.Read_Tstat_Info[i].task_result = true;
		//}

		for (int i=0;i<BAC_CUSTOMER_UNIT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Customer_unit_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Customer_unit_Info[i].task_result = true;
		}

		for (int i=0;i<BAC_GRPHIC_LABEL_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].task_result = true;
		}

		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//TRACE(Show_Results);
		//TRACE(_T("\r\n"));
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
			{
				Bacnet_Refresh_Info.Read_Variable_Info[i].task_result = false;
			}
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

		for (int i=0;i<BAC_BASIC_SETTING_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_BasicSetting_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_BasicSetting_Info[i].task_result = false;
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
		//for (int i=0;i<BAC_TSTAT_GROUP;i++)
		//{
		//	if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Tstat_Info[i].invoke_id)
		//		Bacnet_Refresh_Info.Read_Tstat_Info[i].task_result = false;
		//}

		for (int i=0;i<BAC_CUSTOMER_UNIT_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Customer_unit_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Customer_unit_Info[i].task_result = false;
		}

		for (int i=0;i<BAC_GRPHIC_LABEL_GROUP;i++)
		{
			if(pInvoke->Invoke_ID==Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].invoke_id)
				Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].task_result = false;
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

static bool initial_once = true;
void CDialogCM5_BacNet::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//Fresh();//Fance
	Initial_All_Point();
	
	Tab_Initial();
	initial_once = false;
	// TODO: Add your specialized code here and/or call the base class
}

void CDialogCM5_BacNet::Tab_Initial()
{
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//为Tab Control增加两个页面;
	m_bac_main_tab.InsertItem(WINDOW_INPUT, _T("&Input   "));
	m_bac_main_tab.InsertItem(WINDOW_OUTPUT, _T("&Output   "));
	m_bac_main_tab.InsertItem(WINDOW_VARIABLE, _T("&Variable   "));
	m_bac_main_tab.InsertItem(WINDOW_PROGRAM, _T("&Program   "));
	m_bac_main_tab.InsertItem(WINDOW_CONTROLLER, _T("&Controller   "));
	m_bac_main_tab.InsertItem(WINDOW_SCREEN, _T("&Screen   "));
	m_bac_main_tab.InsertItem(WINDOW_WEEKLY, _T("&Weekly Routine   "));
	m_bac_main_tab.InsertItem(WINDOW_ANNUAL, _T("&Annual Routine   "));
	m_bac_main_tab.InsertItem(WINDOW_MONITOR, _T("&Monitor   "));
	m_bac_main_tab.InsertItem(WINDOW_ALARMLOG, _T("Alarm &Log   "));
	m_bac_main_tab.InsertItem(WINDOW_TSTAT, _T("&Tstat   "));
	m_bac_main_tab.InsertItem(WINDOW_SETTING, _T("Setting   "));
	m_bac_main_tab.InsertItem(12, _T("Remote Point   "));
	

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
	pDialog[WINDOW_TSTAT] = Tstat_Window = new CBacnetTstat;
	pDialog[WINDOW_SETTING] = Setting_Window = new CBacnetSetting;
	pDialog[WINDOW_USER_LOGIN] =  User_Login_Window = new CBacnetUserlogin;
	pDialog[WINDOW_REMOTE_POINT] =  Remote_Point_Window = new CBacnetRemotePoint;
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
	Tstat_Window->Create(IDD_DIALOG_BACNET_TSTAT,&m_bac_main_tab);
	Setting_Window->Create(IDD_DIALOG_BACNET_SETTING,&m_bac_main_tab);
	User_Login_Window->Create(IDD_DIALOG_BACNET_USER_LOGIN, this);
	Remote_Point_Window->Create(IDD_DIALOG_BACNET_REMOTE_POINT,&m_bac_main_tab);
	//设定在Tab内显示的范围;
	CRect rc;
	m_bac_main_tab.GetClientRect(rc);
	//rc.top += 20;
	rc.top -= 20;

	//rc.bottom -= 8;
	//rc.left += 8;
	//rc.right -= 8;

	for (int i=0;i<WINDOW_TAB_COUNT;i++)
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
	pDialog[WINDOW_TSTAT]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_SETTING]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_USER_LOGIN]->ShowWindow(SW_HIDE);
	pDialog[WINDOW_REMOTE_POINT]->ShowWindow(SW_HIDE);
	g_hwnd_now = m_input_dlg_hwnd;
	Input_Window->m_input_list.SetFocus();
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
	m_Tstat_data.clear();
	m_customer_unit_data.clear();
	m_user_login_data.clear();
	//vector <Str_TstatInfo_point> m_Tstat_data;
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
		memset(&temp_out,0,sizeof(temp_out));
		//memset(temp_out.description,0,21);
		//memset(temp_out.label,0,9);
		m_Output_data.push_back(temp_out);
	}
	for (int i=0;i<BAC_VARIABLE_ITEM_COUNT;i++)
	{
		Str_variable_point temp_variable;
		memset(&temp_variable,0,sizeof(temp_variable));
		//memset(temp_variable.description,0,21);
		//memset(temp_variable.label,0,9);
		m_Variable_data.push_back(temp_variable);
	}
	for(int i=0;i<BAC_PROGRAM_ITEM_COUNT;i++)
	{
		Str_program_point temp_program;
		memset(&temp_program,0,sizeof(temp_program));
		//memset(temp_program.description,0,21);
		//memset(temp_program.label,0,9);
		temp_program.bytes = 400;//初始化时默认为400的长度，避免读不到数据;
		m_Program_data.push_back(temp_program);
	}
	for(int i=0;i<BAC_WEEKLY_ROUTINES_COUNT;i++)
	{
		Str_weekly_routine_point temp_weekly;
		memset(&temp_weekly,0,sizeof(temp_weekly));
		m_Weekly_data.push_back(temp_weekly);
	}
	for (int i=0;i<BAC_ANNUAL_ROUTINES_COUNT;i++)
	{
		Str_annual_routine_point temp_annual;
		memset(&temp_annual,0,sizeof(temp_annual));
		m_Annual_data.push_back(temp_annual);
	}

	for (int i=0;i<BAC_WEEKLY_ROUTINES_COUNT;i++)
	{
		Str_schedual_time_point temp_schedual;
		memset(&temp_schedual,0,sizeof(temp_schedual));
		m_Schedual_Time_data.push_back(temp_schedual);
	}

	for (int i=0;i<BAC_CONTROLLER_COUNT;i++)
	{
		Str_controller_point temp_controller;
		memset(&temp_controller,0,sizeof(temp_controller));
		m_controller_data.push_back(temp_controller);
	}
	for (int i=0;i<BAC_SCREEN_COUNT;i++)
	{
		Control_group_point temp_screen;
		memset(&temp_screen,0,sizeof(temp_screen));
		m_screen_data.push_back(temp_screen);
	}
	for (int i=0;i<BAC_MONITOR_COUNT;i++)
	{
		Str_monitor_point temp_monitor;
		memset(&temp_monitor,0,sizeof(temp_monitor));
		m_monitor_data.push_back(temp_monitor);
	}

	for (int i=0;i<BAC_ALARMLOG_COUNT;i++)
	{
		Alarm_point temp_alarmpoint;
		memset(&temp_alarmpoint,0,sizeof(temp_alarmpoint));
		//memset(temp_alarmpoint.alarm_message,0,ALARM_MESSAGE_SIZE);
		m_alarmlog_data.push_back(temp_alarmpoint);
	}
	for(int i=0;i<BAC_TSTAT_COUNT;i++)
	{
		Str_TstatInfo_point temp_tststpoint;
		memset(&temp_tststpoint,0,sizeof(temp_tststpoint));
		temp_tststpoint.product_model = 255;  //default 255  means no device;
		m_Tstat_data.push_back(temp_tststpoint);
		
	}

	for (int i=0;i<BAC_CUSTOMER_UNITS_COUNT;i++)
	{
		Str_Units_element temp_customer_units;
		memset(&temp_customer_units,0,sizeof(Str_Units_element));
		m_customer_unit_data.push_back(temp_customer_units);
	}
	
	for (int i=0;i<BAC_USER_LOGIN_COUNT;i++)
	{
		Str_userlogin_point temp_user_login;
		memset(&temp_user_login,0,sizeof(Str_userlogin_point));
		m_user_login_data.push_back(temp_user_login);
	}
	for(int i=0;i<BAC_GRPHIC_LABEL_COUNT;i++)
	{
		Str_label_point temp_label_point;
		memset(&temp_label_point,0,sizeof(Str_label_point));
		m_graphic_label_data.push_back(temp_label_point);
	}
	for (int i=0;i<BAC_REMOTE_POINT_COUNT;i++)
	{
		Str_remote_point temp_remote_point;
		memset(&temp_remote_point,0,sizeof(Str_remote_point));
		m_remote_point_data.push_back(temp_remote_point);
	}
}
//__declspec(dllexport) HANDLE	Get_RS485_Handle();


//INPUT int test_function_return_value();
void CDialogCM5_BacNet::Fresh()
{
	if (g_protocol!=PROTOCOL_BACNET_IP)
	{
		return;
	}
//	g_NEED_MULTI_READ = FALSE;
	read_write_bacnet_config = false;
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if(last_serial_number != pFrame->m_product.at(selected_product_index).serial_number) //如果上次的设备不是一样的就需要重读 Graphic label;
	{
		last_serial_number = pFrame->m_product.at(selected_product_index).serial_number;
		need_read_bacnet_graphic_label_flag = true;
	}

	if(initial_once)
	{
		initial_once = false;
		Tab_Initial();
	}
	m_user_level = LOGIN_SUCCESS_FULL_ACCESS;
	if(pFrame->m_product.at(selected_product_index).protocol == PROTOCOL_GSM)
	//if(0)	//GSM连接
	{
		Gsm_communication = true;
		g_bac_instance = pFrame->m_product.at(selected_product_index).hardware_version;
		bac_gloab_panel = g_mac = pFrame->m_product.at(selected_product_index).software_version;
		bacnet_device_type = SMALL_MINIPANEL;
		//Output_Window->Reload_Unit_Type();
		Inital_Tab_Loaded_Parameter();
		BacNet_hwd = this->m_hWnd;
		Station_NUM = bac_gloab_panel;
		if(initial_once)
		{
			initial_once = false;
			Tab_Initial();
		}


		PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,BAC_READ_INPUT_LIST);
		Input_Window->ShowWindow(SW_SHOW);
		m_bac_main_tab.SetCurSel(WINDOW_INPUT);
		if((CBacnetAlarmWindow *)AlarmWindow_Window->IsWindowVisible())
		{
			AlarmWindow_Window->ShowWindow(SW_HIDE);
		}
		return;
	}
	bip_setgsm(false);
	Gsm_communication = false;
	static bool initial_once_ip = false;
#ifndef test_ptp
	if(initial_once_ip == false)
	{
		initial_once_ip = true;
		g_gloab_bac_comport = 0;
		set_datalink_protocol(2);
	Initial_bac(g_gloab_bac_comport);
	}
	else
	{
		bip_set_socket(my_sokect);
	}

	//
	//SetTimer(1,500,NULL);
	SetTimer(2,60000,NULL);//定时器2用于间隔发送 whois;不知道设备什么时候会被移除;
	SetTimer(3,1000,NULL); //Check whether need  show Alarm dialog.
#endif
	BacNet_hwd = this->m_hWnd;

#ifndef test_ptp
	CString temp_cs;
	temp_cs.Format(_T("%d"),g_bac_instance);
	GetDlgItem(IDC_STATIC_CM_DEVICE_ID)->SetWindowTextW(temp_cs);
	temp_cs.Format(_T("%d"),g_mac);
	GetDlgItem(IDC_STATIC_CM5_MAC)->SetWindowTextW(temp_cs);
	Station_NUM = g_mac;

	
	CString nconnectionip = pFrame->m_product.at(selected_product_index).BuildingInfo.strIp;
	int nport =	pFrame->m_product.at(selected_product_index).ncomport;

	g_CommunicationType = 1;
	SetCommunicationType(1);
	int ret = 0;
	if (ValidAddress(nconnectionip)==FALSE)  // 验证NC的IP
	{

		CString StringIP;
		if(!GetIPbyHostName(nconnectionip,StringIP))
		{
			AfxMessageBox(_T("Can not get a validate IP adreess from the domain name!"));
			return ;
		}
		nconnectionip = StringIP;
	}
	for (int i=0;i<3;i++)
	{
		ret = Open_Socket2(nconnectionip,nport);
		if(ret)
		{
			TRACE(_T("Reconnected ~~~~~~~~\r\n"));
			break;
		}
	}
	
	if(ret)
	{
		::PostMessage(MainFram_hwd,MY_RX_TX_COUNT,1,0);
		int multy_ret = 0;
		unsigned short temp_buffer[3];
		memset(temp_buffer,0,3);
		multy_ret = Read_Multi(g_tstat_id,temp_buffer,34,3);
		int temp_instance;
		int temp_mac;
		if(multy_ret<=0)
		{
			pFrame->m_pTreeViewCrl->turn_item_image(selected_tree_item ,false);
			::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,CONNECT_TO_MODBUS_FAILED,0);
		}

		temp_instance = temp_buffer[1];
		temp_mac = temp_buffer[2];


		if(pFrame->m_product.at(selected_product_index).product_class_id == PM_CM5)
			bacnet_device_type = PRODUCT_CM5;
		else
		{
			int ret = 0;
			ret = temp_buffer[0];
			//ret = read_one(g_tstat_id,34,5);
			if(ret == 1)
				bacnet_device_type = BIG_MINIPANEL;
			else if(ret == 2)
				bacnet_device_type = SMALL_MINIPANEL;
			else
				bacnet_device_type = PRODUCT_CM5;
		}
		//temp_instance = read_one(g_tstat_id,35,5);
		//temp_mac = read_one(g_tstat_id,36,5);
		if((temp_instance<=0) || (temp_mac <=0))
		{
			pFrame->m_pTreeViewCrl->turn_item_image(selected_tree_item ,false);
			::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,CONNECT_TO_MODBUS_FAILED,0);
		}
		else//更新数据库;如果是通过modbus扫描到得bacnet设备，因为不知道它的instance ID和panel number.所有读到后更新数据库;
		{
			bac_gloab_panel = g_mac;
			if((g_bac_instance != temp_instance) || (g_mac != temp_mac))
			{
				g_bac_instance = temp_instance;
				g_mac = temp_mac;
				pFrame->m_product.at(selected_product_index).hardware_version = (float)g_bac_instance;
				pFrame->m_product.at(selected_product_index).software_version = (float)g_mac;
				CString strSql;
				CString str_serialid;
				CString str_baudrate;
				CString hw_instance;
				CString sw_mac;
				hw_instance.Format(_T("%d"),g_bac_instance);
				sw_mac.Format(_T("%d"),g_mac);
				str_serialid.Format(_T("%d"),pFrame->m_product.at(selected_product_index).serial_number);
				str_baudrate =pFrame->m_product.at(selected_product_index).BuildingInfo.strIp;
				//TRACE(_T("update ALL_NODE set Software_Ver =\r\n"));
				CBADO bado;
				bado.SetDBPath(g_strCurBuildingDatabasefilePath);
				bado.OnInitADOConn(); 

				try
				{
					//pFrame->m_pCon.CreateInstance(_T("ADODB.Connection"));
					//pFrame->m_pCon->Open(g_strDatabasefilepath.GetString(),_T(""),_T(""),adModeUnknown);
					strSql.Format(_T("update ALL_NODE set Hardware_Ver ='%s' where Serial_ID = '%s' and Bautrate = '%s'"),hw_instance,str_serialid,str_baudrate);
					bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);		
					strSql.Format(_T("update ALL_NODE set Software_Ver ='%s' where Serial_ID = '%s' and Bautrate = '%s'"),sw_mac,str_serialid,str_baudrate);
					bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);		
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
				//if(pFrame->m_pCon->State)
				//	pFrame->m_pCon->Close();	
				bado.CloseConn();
			}
			::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,START_BACNET_TIMER,0);
			CString temp_info;
			temp_info.Format(_T("Device is connected!"));
			CString* pstrInfo = new CString(temp_info); 
			::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
			bac_gloab_panel = g_mac = Station_NUM = temp_mac ;
			g_bac_instance = temp_instance;
		}
	}
	else
	{
		::PostMessage(MainFram_hwd,MY_RX_TX_COUNT,0,0);
		pFrame->m_product.at(selected_product_index).status_last_time[0] = false;//没有读到的话就将左边的list和状态都设置为false;
		pFrame->m_product.at(selected_product_index).status_last_time[1] = false;
		pFrame->m_product.at(selected_product_index).status_last_time[2] = false;
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,CONNECT_TO_MODBUS_FAILED,0);
	}



#endif

	//prevent when user doesn't click the bacnet device,the view also initial ,It's a waste of resource;
#if 1

	

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


LRESULT CDialogCM5_BacNet::Fresh_UI(WPARAM wParam,LPARAM lParam)
{
	int command_type = wParam;
	int button_click = 0;
	CString temp_cs;
	CTime	TimeTemp;
	switch(command_type)
	{
	case WM_COMMAND_WHO_IS:
		temp_cs.Format(_T("%d"),g_bac_instance);
		GetDlgItem(IDC_STATIC_CM_DEVICE_ID)->SetWindowTextW(temp_cs);
		temp_cs.Format(_T("%d"),g_mac);
		GetDlgItem(IDC_STATIC_CM5_MAC)->SetWindowTextW(temp_cs);
		//Station_NUM = g_mac;

		break;

	case MENU_CLICK:

		button_click = lParam;
		tsm_free_all_invoke_id();//每次点击的时候都将所有INVOKE ID 清零;
		if(button_click == WRITEPRGFLASH_COMMAND)
		{
			WriteFlash();
		}
		else
		{
			if(read_write_bacnet_config == false)
				Show_Wait_Dialog_And_SendMessage(button_click);
		}
		break;
	case TYPE_SVAE_CONFIG:
		read_write_bacnet_config = true;
		tsm_free_all_invoke_id();//每次点击的时候都将所有INVOKE ID 清零;
		Show_Wait_Dialog_And_SendMessage(TYPE_SVAE_CONFIG);
		break;
	default: 
		break;
	}

	
	return 0;
}




void CDialogCM5_BacNet::Show_Wait_Dialog_And_SendMessage(int read_list_type)
{
	bac_read_which_list = read_list_type;
	if(WaitDlg==NULL)
	{
		//if(bac_read_which_list != BAC_READ_CUSTOMER_UNITS)
		//{
			if(bac_read_which_list == BAC_READ_SVAE_CONFIG)
			{
				WaitDlg = new BacnetWait((int)BAC_WAIT_READ_DATA_WRITE_CONFIG);//如果是保存为ini文件 就要读取全部的data;
			}
			else
			{
				WaitDlg = new BacnetWait((int)BAC_WAIT_NORMAL_READ);
			}
			WaitDlg->Create(IDD_DIALOG_BACNET_WAIT,this);
			WaitDlg->ShowWindow(SW_SHOW);


			RECT RECT_SET1;
			GetClientRect(&RECT_SET1);
			ClientToScreen(&RECT_SET1);
			WaitDlg->MoveWindow(RECT_SET1.left + 50,RECT_SET1.bottom - 19,800,20);
			//RECT RECT_SET1;
			//GetWindowRect(&RECT_SET1);
			//WaitDlg->MoveWindow(RECT_SET1.left+100,RECT_SET1.top+400,560/*RECT_SET1.left+270*//*RECT_SET1.right/2+20*/,100);
		//}
		
	}

	//::PostMessage(BacNet_hwd,WM_SEND_OVER,0,0);
	if(hwait_thread==NULL)
	{
		hwait_thread =CreateThread(NULL,NULL,Send_read_Command_Thread,this,NULL, NULL);

	}
}



static int resend_count=0;
//DWORD WINAPI  CDialogCM5_BacNet::Send_read_Command_Thread(LPVOID lpVoid)
DWORD WINAPI  Send_read_Command_Thread(LPVOID lpVoid)
{
	CDialogCM5_BacNet *pParent = (CDialogCM5_BacNet *)lpVoid;
	bac_read_all_results = false;
	int end_temp_instance = 0;
	bac_programcode_read_results = false;
	for (int i=0;i<BAC_GRPHIC_LABEL_GROUP;i++)
	{
		if(bac_read_which_list == BAC_READ_GRAPHIC_LABEL_INFO)
		{
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].invoke_id = -1;
			bac_graphic_label_read_results = 0;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].timeout_count = 0;
		}
	}
	if(bac_read_which_list == BAC_READ_REMOTE_POINT_INFO)
	{
		for (int i=0;i<BAC_REMOTE_POINT_GROUP;i++)
		{
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].invoke_id = -1;
			bac_remote_point_read_results = 0;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].timeout_count = 0;
		}
	}

	for (int i=0;i<BAC_USER_LOGIN_GROUP;i++)
	{
		if(bac_read_which_list == BAC_READ_USER_LOGIN_INFO)
		{
			Bacnet_Refresh_Info.Read_UserLogin[i].command = 0;
			Bacnet_Refresh_Info.Read_UserLogin[i].device_id = 0;
			Bacnet_Refresh_Info.Read_UserLogin[i].start_instance =0;
			Bacnet_Refresh_Info.Read_UserLogin[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_UserLogin[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_UserLogin[i].resend_count =0;
			Bacnet_Refresh_Info.Read_UserLogin[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_UserLogin[i].invoke_id = -1;
			bac_user_login_read_results = 0;
			Bacnet_Refresh_Info.Read_UserLogin[i].timeout_count = 0;
		}
	}

	
	for (int i=0;i<BAC_CUSTOMER_UNIT_GROUP;i++)
	{
		if(bac_read_which_list == BAC_READ_CUSTOMER_UNITS)
		{
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].resend_count =0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].invoke_id = -1;
			bac_customer_unit_read_results = 0;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].timeout_count = 0;
		}
	}


	//for (int i=0;i<BAC_TSTAT_GROUP;i++)
	//{
	//	if((bac_read_which_list == BAC_READ_TSTAT_LIST) || (bac_read_which_list == BAC_READ_ALL_LIST))
	//	{
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].command = 0;
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].device_id = 0;
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].start_instance =0;
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].end_instance = 0;
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].has_resend_yes_or_no = 0;
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].resend_count =0;
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].task_result = BAC_RESULTS_UNKONW;
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].invoke_id = -1;
	//		bac_tstat_read_results = 0;
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].timeout_count = 0;
	//	}
	//}
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

	for (int i=0;i<BAC_BASIC_SETTING_GROUP;i++)//prg文件暂时不保存这些基本设置的东西，像IP地址和mac地址;
	{
		if((bac_read_which_list == BAC_READ_BASIC_SETTING_COMMAND) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].command = 0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].device_id = 0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].start_instance =0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].end_instance = 0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].has_resend_yes_or_no = 0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].resend_count =0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].task_result = BAC_RESULTS_UNKONW;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].invoke_id = -1;
			bac_basic_setting_read_results = 0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].timeout_count = 0;
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
		if((bac_read_which_list == BAC_READ_INPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
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
		if((bac_read_which_list == BAC_READ_OUTPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
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
		if((bac_read_which_list == BAC_READ_VARIABLE_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) ||(bac_read_which_list ==TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
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

	for (int i=0;i<BAC_GRPHIC_LABEL_GROUP;i++)
	{
		if(bac_read_which_list == BAC_READ_GRAPHIC_LABEL_INFO)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_GRPHIC_LABEL_REMAINDER + (BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_GRPHIC_LABEL_COUNT)
					end_temp_instance = BAC_GRPHIC_LABEL_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,READ_GRPHIC_LABEL_COMMAND,(BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i,
					end_temp_instance,sizeof(Str_label_point));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].command = READ_GRPHIC_LABEL_COMMAND;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].start_instance = (BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Graphic Label From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Label_Graphic_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	if(bac_read_which_list == BAC_READ_REMOTE_POINT_INFO)
	{
		for (int i=0;i<BAC_REMOTE_POINT_GROUP;i++)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_REMOTE_POINT_GROUP_REMAINDER + (BAC_REMOTE_POINT_GROUP_NUMBER*i) ;
				if(end_temp_instance >= BAC_REMOTE_POINT_COUNT)
					end_temp_instance = BAC_REMOTE_POINT_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,READ_REMOTE_POINT,(BAC_REMOTE_POINT_GROUP_NUMBER)*i,
					end_temp_instance,sizeof(Str_remote_point));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].command = READ_REMOTE_POINT;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].start_instance = (BAC_REMOTE_POINT_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_Remote_Point_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Remote point From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Remote_Point_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Remote_Point_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}



	for (int i=0;i<BAC_USER_LOGIN_GROUP;i++)
	{
		if(bac_read_which_list == BAC_READ_USER_LOGIN_INFO)
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_USER_LOGIN_INFO_REMAINDER + (BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_USER_LOGIN_COUNT)
					end_temp_instance = BAC_USER_LOGIN_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,READUSER_T3000,(BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i,
					end_temp_instance,sizeof(Str_userlogin_point));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_UserLogin[i].command = READUSER_T3000;
			Bacnet_Refresh_Info.Read_UserLogin[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_UserLogin[i].start_instance = (BAC_READ_USER_LOGIN_INFO_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_UserLogin[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_UserLogin[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Tstat From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_UserLogin[i].start_instance,
				Bacnet_Refresh_Info.Read_UserLogin[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


	for (int i=0;i<BAC_CUSTOMER_UNIT_GROUP;i++)
	{
		if(bac_read_which_list == BAC_READ_CUSTOMER_UNITS) 
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,READUNIT_T3000,(BAC_READ_GROUP_NUMBER)*i,
					3+(BAC_READ_GROUP_NUMBER)*i,sizeof(Str_Units_element));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].command = READUNIT_T3000;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].start_instance = (BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].end_instance =3+(BAC_READ_GROUP_NUMBER)*i;
			//Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance =2+(BAC_READ_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Customer_unit_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Customer units From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Customer_unit_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Customer_unit_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}





	//for (int i=0;i<BAC_TSTAT_GROUP;i++)
	//{
	//	if((bac_read_which_list == BAC_READ_TSTAT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST))
	//	{
	//		resend_count = 0;
	//		do 
	//		{
	//			resend_count ++;
	//			if(resend_count>RESEND_COUNT)
	//				goto myend;
	//			end_temp_instance = BAC_READ_TSTAT_REMAINDER + (BAC_READ_TSTAT_GROUP_NUMBER)*i ;
	//			if(end_temp_instance >= BAC_TSTAT_COUNT)
	//				end_temp_instance = BAC_TSTAT_COUNT - 1;

	//			g_invoke_id = GetPrivateData(g_bac_instance,READTSTAT_T3000,(BAC_READ_TSTAT_GROUP_NUMBER)*i,
	//				end_temp_instance,sizeof(Str_TstatInfo_point));
	//			Sleep(SEND_COMMAND_DELAY_TIME);	
	//		} while (g_invoke_id<0);

	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].command = READTSTAT_T3000;
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].device_id = g_bac_instance;
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].start_instance = (BAC_READ_TSTAT_GROUP_NUMBER)*i;
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].end_instance = end_temp_instance;
	//		Bacnet_Refresh_Info.Read_Tstat_Info[i].invoke_id = g_invoke_id;
	//		CString temp_cs;
	//		temp_cs.Format(_T("Task ID = %d. Read Tstat From %d to %d "),g_invoke_id,
	//			Bacnet_Refresh_Info.Read_Tstat_Info[i].start_instance,
	//			Bacnet_Refresh_Info.Read_Tstat_Info[i].end_instance);
	//		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
	//	}
	//}

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
				end_temp_instance = BAC_READ_MONOTOR_REMAINDER + (BAC_READ_MONITOR_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_MONITOR_COUNT)
					end_temp_instance = BAC_MONITOR_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,READMONITOR_T3000,(BAC_READ_MONITOR_GROUP_NUMBER)*i,
					end_temp_instance,
				//	2+(BAC_READ_GROUP_NUMBER)*i,
					sizeof(Str_monitor_point));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Monitor_Info[i].command = READMONITOR_T3000;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].start_instance = (BAC_READ_MONITOR_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Monitor_Info[i].end_instance = end_temp_instance;
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
				end_temp_instance = BAC_READ_SCREEN_REMAINDER + (BAC_READ_SCREEN_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_SCREEN_COUNT)
					end_temp_instance = BAC_SCREEN_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,READSCREEN_T3000,(BAC_READ_SCREEN_GROUP_NUMBER)*i,
					end_temp_instance,
					sizeof(Control_group_point));
				Sleep(SEND_COMMAND_DELAY_TIME);	
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Screen_Info[i].command = READSCREEN_T3000;
			Bacnet_Refresh_Info.Read_Screen_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Screen_Info[i].start_instance = (BAC_READ_SCREEN_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Screen_Info[i].end_instance = end_temp_instance;
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
		//如果是在读Setting 界面的时候也要同时读 Time，因为time在setting界面值;
		if((bac_read_which_list == BAC_READ_TIME_COMMAND) ||(bac_read_which_list == BAC_READ_BASIC_SETTING_COMMAND)|| (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
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



	for (int i=0;i<BAC_BASIC_SETTING_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_BASIC_SETTING_COMMAND) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list == TYPE_SVAE_CONFIG))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				g_invoke_id = GetPrivateData(g_bac_instance,
					READ_SETTING_COMMAND,0,
					0,
					sizeof(Str_Setting_Info));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].command = READ_SETTING_COMMAND;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].start_instance = 0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].end_instance =0;
			Bacnet_Refresh_Info.Read_BasicSetting_Info[i].invoke_id = g_invoke_id;

			CString temp_cs_show;
			temp_cs_show.Format(_T("Task ID = %d. Read Basic Setting "),g_invoke_id);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);

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
				end_temp_instance = BAC_READ_WEEKLY_ROUTINES_REMAINDER + (BAC_READ_WEEKLY_ROUTINES_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_WEEKLY_ROUTINES_COUNT)
					end_temp_instance = BAC_WEEKLY_ROUTINES_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,
					READWEEKLYROUTINE_T3000,(BAC_READ_WEEKLY_ROUTINES_GROUP_NUMBER)*i,
					end_temp_instance,
					sizeof(Str_weekly_routine_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Weekly_Info[i].command = READWEEKLYROUTINE_T3000;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].start_instance = (BAC_READ_WEEKLY_ROUTINES_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Weekly_Info[i].end_instance =end_temp_instance;
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
				end_temp_instance = BAC_READ_ANNUAL_ROUTINES_REMAINDER + (BAC_READ_ANNUAL_ROUTINES_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_ANNUAL_ROUTINES_COUNT)
					end_temp_instance = BAC_ANNUAL_ROUTINES_COUNT - 1;

				g_invoke_id = GetPrivateData(g_bac_instance,
					READANNUALROUTINE_T3000,(BAC_READ_ANNUAL_ROUTINES_GROUP_NUMBER)*i,
					end_temp_instance,
					sizeof(Str_annual_routine_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Annual_Info[i].command = READANNUALROUTINE_T3000;
			Bacnet_Refresh_Info.Read_Annual_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance = (BAC_READ_ANNUAL_ROUTINES_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance =end_temp_instance;
			Bacnet_Refresh_Info.Read_Annual_Info[i].invoke_id = g_invoke_id;


			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Annual Routine Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Annual_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Annual_Info[i].end_instance);
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
				end_temp_instance = BAC_READ_ALARMLOG_REMAINDER + (BAC_READ_ALARMLOG_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_ALARMLOG_COUNT)
					end_temp_instance = BAC_ALARMLOG_COUNT - 1;
				g_invoke_id = GetPrivateData(g_bac_instance,READALARM_T3000,(BAC_READ_ALARMLOG_GROUP_NUMBER)*i,
					end_temp_instance,sizeof(Alarm_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
				//if(g_invoke_id<0)
				//	TRACE(_T("Input g_invoke_id = %d ,resend count = %d\r\n"),g_invoke_id,resend_count);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].command = READALARM_T3000;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].start_instance = (BAC_READ_ALARMLOG_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_AlarmLog_Info[i].invoke_id = g_invoke_id;


			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Alarm Log List %d"),g_invoke_id,
				Bacnet_Refresh_Info.Read_AlarmLog_Info[i].start_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}



	for (int i=0;i<BAC_INPUT_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_INPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_INPUT_REMAINDER + (BAC_READ_INPUT_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_INPUT_ITEM_COUNT)
					end_temp_instance = BAC_INPUT_ITEM_COUNT - 1;
				g_invoke_id = GetPrivateData(g_bac_instance,READINPUT_T3000,(BAC_READ_INPUT_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_in_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
				//if(g_invoke_id<0)
				//	TRACE(_T("Input g_invoke_id = %d ,resend count = %d\r\n"),g_invoke_id,resend_count);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Input_Info[i].command = READINPUT_T3000;
			Bacnet_Refresh_Info.Read_Input_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Input_Info[i].start_instance = (BAC_READ_INPUT_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Input_Info[i].end_instance =end_temp_instance;
			Bacnet_Refresh_Info.Read_Input_Info[i].invoke_id = g_invoke_id;
			if(Bacnet_Refresh_Info.Read_Input_Info[i].end_instance >= BAC_INPUT_ITEM_COUNT)
				Bacnet_Refresh_Info.Read_Input_Info[i].end_instance = BAC_INPUT_ITEM_COUNT - 1;

			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Input List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Input_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Input_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_OUTPUT_GROUP;i++)
	{

		if((bac_read_which_list == BAC_READ_OUTPUT_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
		{
			resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_OUTPUT_REMAINDER + (BAC_READ_OUTPUT_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_OUTPUT_ITEM_COUNT)
					end_temp_instance = BAC_OUTPUT_ITEM_COUNT - 1;
				g_invoke_id = GetPrivateData(g_bac_instance,
					READOUTPUT_T3000,(BAC_READ_OUTPUT_GROUP_NUMBER)*i,
					end_temp_instance,
					sizeof(Str_out_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Output_Info[i].command = READOUTPUT_T3000;
			Bacnet_Refresh_Info.Read_Output_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Output_Info[i].start_instance = (BAC_READ_OUTPUT_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Output_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_Output_Info[i].invoke_id = g_invoke_id;

			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Output List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Output_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Output_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}

	for (int i=0;i<BAC_VARIABLE_GROUP;i++)
	{
		if((bac_read_which_list == BAC_READ_VARIABLE_LIST) || (bac_read_which_list ==BAC_READ_ALL_LIST) || (bac_read_which_list ==TYPE_SVAE_CONFIG) || (bac_read_which_list == BAC_READ_IN_OUT_VAR_LIST))
		{
			int resend_count = 0;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				end_temp_instance = BAC_READ_VARIABLE_REMAINDER + (BAC_READ_VARIABLE_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_VARIABLE_ITEM_COUNT)
					end_temp_instance = BAC_VARIABLE_ITEM_COUNT - 1;
				g_invoke_id = GetPrivateData(g_bac_instance,READVARIABLE_T3000,(BAC_READ_VARIABLE_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_variable_point));
				//TRACE(_T("g_invoke_id = %d\r\n",g_invoke_id));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Variable_Info[i].command = READVARIABLE_T3000;
			Bacnet_Refresh_Info.Read_Variable_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Variable_Info[i].start_instance = (BAC_READ_VARIABLE_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Variable_Info[i].end_instance = end_temp_instance;
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
				end_temp_instance = BAC_READ_PROGRAM_REMAINDER + (BAC_READ_PROGRAM_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_PROGRAM_ITEM_COUNT)
					end_temp_instance = BAC_PROGRAM_ITEM_COUNT - 1;
				g_invoke_id = GetPrivateData(g_bac_instance,READPROGRAM_T3000,(BAC_READ_PROGRAM_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_program_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Program_Info[i].command = READPROGRAM_T3000;
			Bacnet_Refresh_Info.Read_Program_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Program_Info[i].start_instance = (BAC_READ_PROGRAM_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Program_Info[i].end_instance = end_temp_instance;
			Bacnet_Refresh_Info.Read_Program_Info[i].invoke_id = g_invoke_id;
			CString temp_cs;
			temp_cs.Format(_T("Task ID = %d. Read Program List Item From %d to %d "),g_invoke_id,
				Bacnet_Refresh_Info.Read_Program_Info[i].start_instance,
				Bacnet_Refresh_Info.Read_Program_Info[i].end_instance);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs);
		}
	}


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
				end_temp_instance = BAC_READ_CONTROLLER_REMAINDER + (BAC_READ_CONTROLLER_GROUP_NUMBER)*i ;
				if(end_temp_instance >= BAC_CONTROLLER_COUNT)
					end_temp_instance = BAC_CONTROLLER_COUNT - 1;
				g_invoke_id = GetPrivateData(g_bac_instance,READCONTROLLER_T3000,(BAC_READ_CONTROLLER_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_controller_point));
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (g_invoke_id<0);

			Bacnet_Refresh_Info.Read_Controller_Info[i].command = READCONTROLLER_T3000;
			Bacnet_Refresh_Info.Read_Controller_Info[i].device_id = g_bac_instance;
			Bacnet_Refresh_Info.Read_Controller_Info[i].start_instance = (BAC_READ_CONTROLLER_GROUP_NUMBER)*i;
			Bacnet_Refresh_Info.Read_Controller_Info[i].end_instance = end_temp_instance;
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



#if 1
	if(bac_read_which_list == BAC_READ_PROGRAMCODE_LIST)
	{
		memset(mycode,0,2000);



		for (int x=0;x<5;x++)
		{
			int send_status = true;
			int resend_count = 0;
			int temp_invoke_id = -1;
			do 
			{
				resend_count ++;
				if(resend_count>RESEND_COUNT)
					goto myend;
				temp_invoke_id = GetProgramData(g_bac_instance,program_list_line,program_list_line,x);
				Sleep(SEND_COMMAND_DELAY_TIME);
			} while (temp_invoke_id<0);

			Sleep(SEND_COMMAND_DELAY_TIME);
			if(send_status)
			{
				for (int i=0;i<2000;i++)
				{
					Sleep(1);
					if(tsm_invoke_id_free(temp_invoke_id))
					{
						goto	part_success;
					}
				}
				goto myend;

part_success:
				continue;
			}



		}

		bac_programcode_read_results = true;
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,SHOW_PROGRAM_IDE,0);


		//::PostMessage(m_program_edit_hwnd,WM_REFRESH_BAC_PROGRAM_RICHEDIT,NULL,NULL);

	}
	else if(bac_read_which_list ==TYPE_SVAE_CONFIG)
	{
		for (int z=0;z<BAC_PROGRAM_ITEM_COUNT;z++)
		{
			memset(program_code[z],0,2000);		 //清零;
			for (int x=0;x<5;x++)
			{
				int send_status = true;
				int resend_count = 0;
				int temp_invoke_id = -1;
				do 
				{
					resend_count ++;
					if(resend_count>RESEND_COUNT)
						goto myend;
					temp_invoke_id = GetProgramData(g_bac_instance,z,z,x);
					Sleep(SEND_COMMAND_DELAY_TIME);
				} while (temp_invoke_id<0);


				if(send_status)
				{
					for (int i=0;i<2000;i++)
					{
						Sleep(1);
						if(tsm_invoke_id_free(temp_invoke_id))
						{
							goto	program_part_success;
						}
					}
					Bacnet_Refresh_Info.Read_Programcode_Info[z].task_result = BAC_RESULTS_FAIL;
					goto myend;
					
program_part_success:
					

					
					continue;
				}



			}
			Bacnet_Refresh_Info.Read_Programcode_Info[z].task_result = BAC_RESULTS_OK;
		}
		//bac_programcode_read_results = true;
	}
#endif
	//if( bac_read_which_list == BAC_READ_ALL_LIST)
	//{
	//	g_CommunicationType = 1;
	//	SetCommunicationType(1);
	//	CreateThread(NULL,NULL,ConnectToDevice,NULL,NULL,NULL);
	//}
	
	hwait_thread = NULL;

	return 0;
		//::PostMessage(BacNet_hwd,WM_SEND_OVER,0,0);

myend:	hwait_thread = NULL;
		//AfxMessageBox(_T("Send Command Timeout!!!!!!!!!!"));
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,0,0);
	return 0;
}






void CDialogCM5_BacNet::SetConnected_IP(LPCTSTR myip)
{
	IP_ADDRESS.Empty();
	IP_ADDRESS.Format(_T("%s"),myip);
	//IP_ADDRESS = myip;
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





void CDialogCM5_BacNet::Set_Device_Type(bool is_remote_device)
{
	m_is_remote_device = is_remote_device;
}

void CDialogCM5_BacNet::Set_remote_device_IP(LPCTSTR ipaddress)
{
	remote_ip_address.Format(_T("%s"),ipaddress);
}


void CDialogCM5_BacNet::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	//bool connect_results = Get_MSTP_Connect_Status();
	//static bool Old_connect_results = false;
	bool is_connected = false;
	switch(nIDEvent)
	{
	case 2:
		{
			if(this->IsWindowVisible())
			{
				if(!Gsm_communication)
					m_bac_scan_com_data.clear();
				if(g_bac_instance>0)
					Send_WhoIs_Global(-1, -1);

				if(m_is_remote_device)
					Send_WhoIs_remote_ip(remote_ip_address);

				if(bac_select_device_online)
				{
					CString temp_file;
					CString temp_serial;
					temp_serial.Format(_T("%d.prg"),g_selected_serialnumber);
					temp_file = g_achive_folder + _T("\\") + temp_serial;
					//SaveBacnetConfigFile(temp_file);
					SaveBacnetConfigFile_Cache(temp_file);
				}
			}

		}
		break;
	case 3:
		{
			if((AlarmWindow_Window!=NULL) && ((CBacnetAlarmWindow *)AlarmWindow_Window->m_hWnd !=NULL))
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
	case 4:
		{
			click_resend_time --;
			if(m_is_remote_device)
				Send_WhoIs_remote_ip(remote_ip_address);
			else
				Send_WhoIs_Global(-1, -1);
			bool find_exsit = false;
			for (int i=0;i<(int)m_bac_scan_com_data.size();i++)
			{
				if(m_bac_scan_com_data.at(i).device_id == g_bac_instance)
				{
					find_exsit = true;
					KillTimer(4);
					break;
				}
			}


			if(find_exsit)
			{
				int temp_invoke_id = -1;
				int send_status = true;
				int	resend_count = 0;
				for (int z=0;z<3;z++)
				{
					do 
					{
						resend_count ++;
						if(resend_count>5)
						{
							send_status = false;
							break;
						}
						temp_invoke_id = GetPrivateData(
							g_bac_instance,
							READ_SETTING_COMMAND,
							0,
							0,
							sizeof(Str_Setting_Info));		

						Sleep(SEND_COMMAND_DELAY_TIME);
					} while (temp_invoke_id<0);
					if(send_status)
					{
						Sleep(1000);
						if(tsm_invoke_id_free(temp_invoke_id))
						{
							is_connected = true;
							//Device_Basic_Setting.reg.user_name = 2;
							if(Device_Basic_Setting.reg.user_name == 2) //Enable user name
							{
								m_bac_main_tab.ShowWindow(false);
								PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,BAC_READ_USER_LOGIN_INFO);
								User_Login_Window->ShowWindow(SW_NORMAL);

								::PostMessage(m_user_login_hwnd,MY_REDRAW_WINDOW,NULL,NULL);
								return;
							}
							else
							{
								m_user_level = LOGIN_SUCCESS_FULL_ACCESS;
							}
							break;
						}
						else
							continue;
					}

				}
				if(is_connected)	
				{
					Initial_Some_UI(LOGIN_SUCCESS_FULL_ACCESS);
					break;
				}
				else
				{
					bac_select_device_online = false;
					KillTimer(4);
					CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
					pFrame->m_pTreeViewCrl->turn_item_image(selected_tree_item ,false);
					pFrame->m_product.at(selected_product_index).status_last_time[0] = false;
					pFrame->m_product.at(selected_product_index).status_last_time[1] = false;
					pFrame->m_product.at(selected_product_index).status_last_time[2] = false;

					MessageBox(_T("Read Setting information timeout! Please try again!"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
					break;
				}



			}

			if(!find_exsit)
			{
				if(click_resend_time == 0)
				{
					bac_select_device_online = false;
					KillTimer(4);
					//SetPaneString(2,_T("Offline"));
					CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
					pFrame->m_pTreeViewCrl->turn_item_image(selected_tree_item ,false);
					pFrame->m_product.at(selected_product_index).status_last_time[0] = false;
					pFrame->m_product.at(selected_product_index).status_last_time[1] = false;
					pFrame->m_product.at(selected_product_index).status_last_time[2] = false;
					
					MessageBox(_T("No response! Please Check the connection!"),_T("Notice"),MB_OK | MB_ICONINFORMATION);;
				}
				else
				{
					TRACE(_T("Resend Who is count = %d\r\n"),6 - click_resend_time);
				}
			}

			
		}
		break;
	default:
		break;
	}
	//Old_connect_results = connect_results;

	CFormView::OnTimer(nIDEvent);
}

void	CDialogCM5_BacNet::Initial_Some_UI(int ntype)
{


	//老毛一会让这样改一会让那样改，受不了;先用宏定义标记起来;
#ifdef NEED_ANALOG_DIGITAL_ONLY
	Input_Window->Reload_Unit_Type();
#endif
	Output_Window->Reload_Unit_Type();
	Inital_Tab_Loaded_Parameter();
	//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_READ_CUSTOMER_UNIT);//先读下cutomer unit ，在range里面要显示出来;


	CString temp_cstring;
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	pFrame->m_pTreeViewCrl->turn_item_image(selected_tree_item ,true);

	CString achive_file_path;
	CString temp_serial;
	temp_serial.Format(_T("%d.prg"),g_selected_serialnumber);
	achive_file_path = g_achive_folder + _T("\\") + temp_serial;

	if(m_user_level == LOGIN_SUCCESS_GRAPHIC_MODE)
	{
		//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SCREENS);

		if(LoadBacnetConfigFile_Cache(achive_file_path) < 0 )
			PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALL);
		m_bac_main_tab.SetCurSel(WINDOW_SCREEN);
		for (int i=0;i<WINDOW_TAB_COUNT;i++)
		{
			if(i==WINDOW_SCREEN)
			{
				pDialog[i]->ShowWindow(SW_SHOW);
			}
			else
			{
				pDialog[i]->ShowWindow(SW_HIDE);
			}
		}

		((BacnetScreen *)pDialog[WINDOW_SCREEN])->Fresh_Screen_List(NULL,NULL);
		((BacnetScreen*)pDialog[WINDOW_SCREEN])->Reg_Hotkey();
		g_hwnd_now = m_screen_dlg_hwnd;
		bacnet_view_number = TYPE_SCREENS;
		old_selected_item = WINDOW_SCREEN;
	}
	else if(m_user_level == LOGIN_SUCCESS_ROUTINE_MODE)
	{
		if(LoadBacnetConfigFile_Cache(achive_file_path) < 0 )
			PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,BAC_READ_ALL_LIST);
		m_bac_main_tab.SetCurSel(WINDOW_WEEKLY);
		for (int i=0;i<WINDOW_TAB_COUNT;i++)
		{
			if(i==WINDOW_WEEKLY)
			{
				pDialog[i]->ShowWindow(SW_SHOW);
			}
			else
			{
				pDialog[i]->ShowWindow(SW_HIDE);
			}
		}

		((BacnetWeeklyRoutine *)pDialog[WINDOW_WEEKLY])->Fresh_Weekly_List(NULL,NULL);
		((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Reg_Hotkey();
		g_hwnd_now = m_weekly_dlg_hwnd;
		bacnet_view_number = TYPE_WEEKLY;
		old_selected_item = WINDOW_WEEKLY;
	}
	else
	{	
		if(LoadBacnetConfigFile_Cache(achive_file_path) < 0 )
			PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,BAC_READ_ALL_LIST);
		//if(LoadBacnetConfigFile(false,achive_file_path) < 0 )
		//	PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,BAC_READ_ALL_LIST);
		Set_Tab_Loaded_Parameter(WINDOW_INPUT);
		Set_Tab_Loaded_Parameter(WINDOW_OUTPUT);
		Set_Tab_Loaded_Parameter(WINDOW_VARIABLE);
	}
		//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,bacnet_view_number);
		
	//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SVAE_CONFIG);
	//Post_Write_Message(g_bac_instance,CONNECTED_WITH_DEVICE,0,0,sizeof(Str_connected_point),BacNet_hwd ,_T("Connect with device"));
	if(m_bac_main_tab.IsWindowVisible() == false)
		m_bac_main_tab.ShowWindow(true);
	//m_bac_main_tab.SetFocus();
	//m_bac_main_tab.SetCurSel(0);
	//for (int i=0;i<WINDOW_TAB_COUNT;i++)
	//{	
	//	if(i!=0)
	//		pDialog[i]->ShowWindow(SW_HIDE);
	//	else
	//		pDialog[i]->ShowWindow(SW_NORMAL);
	//}

	switch(bacnet_view_number)
	{
	case TYPE_INPUT:
		Input_Window->m_input_list.SetFocus();
		::PostMessage(m_input_dlg_hwnd, WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
		break;
	case TYPE_OUTPUT:
		Output_Window->m_output_list.SetFocus();
		::PostMessage(m_output_dlg_hwnd, WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
		break;
	case TYPE_VARIABLE:
		Variable_Window->m_variable_list.SetFocus();
		::PostMessage(m_variable_dlg_hwnd, WM_REFRESH_BAC_VARIABLE_LIST,NULL,NULL);
		break;
	case TYPE_PROGRAM:
		Program_Window->m_program_list.SetFocus();
		::PostMessage(m_pragram_dlg_hwnd, WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
		break;
	case TYPE_CONTROLLER:
		Controller_Window->m_controller_list.SetFocus();
		::PostMessage(m_controller_dlg_hwnd, WM_REFRESH_BAC_CONTROLLER_LIST,NULL,NULL);
		break;
	case TYPE_SCREENS:
		Screen_Window->m_screen_list.SetFocus();
		::PostMessage(m_screen_dlg_hwnd, WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
		break;
	case TYPE_WEEKLY:
		WeeklyRoutine_Window->m_weeklyr_list.SetFocus();
		::PostMessage(m_weekly_dlg_hwnd, WM_REFRESH_BAC_WEEKLY_LIST,NULL,NULL);
		break;
	case TYPE_ANNUAL:
		AnnualRoutine_Window->m_annualr_list.SetFocus();
		::PostMessage(m_annual_dlg_hwnd, WM_REFRESH_BAC_ANNUAL_LIST,NULL,NULL);
		break;
	case TYPE_MONITOR:
		Monitor_Window->m_monitor_list.SetFocus();
		::PostMessage(m_monitor_dlg_hwnd, WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
		::PostMessage(m_monitor_dlg_hwnd, WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
		break;
	case TYPE_ALARMLOG:
		AlarmLog_Window->m_alarmlog_list.SetFocus();
		::PostMessage(m_alarmlog_dlg_hwnd, WM_REFRESH_BAC_ALARMLOG_LIST,NULL,NULL);
		break;
	case TYPE_READ_REMOTE_POINT_INFO:
		Remote_Point_Window->m_remote_point_list.SetFocus();
		::PostMessage(m_remote_point_hwnd, WM_REFRESH_BAC_REMOTE_POINT_LIST,NULL,NULL);
		break;
	default:
		break;
	}

	bac_select_device_online = true;
	CString temp_serial_number;
	temp_serial_number.Format(_T("%u"),g_selected_serialnumber);
	
	int temp_write_flag = GetPrivateProfileInt(temp_serial_number,_T("WriteFlag"),0,g_achive_device_name_path);

	if(Device_Basic_Setting.reg.en_panel_name == 1)
	{
		if(temp_write_flag)	//本地的 name 比较新  更新至 设备; 
		{
			CString Write_String;
			char temp_buffer[20];
			GetPrivateProfileStringW(temp_serial_number,_T("NewName"),_T(""),Write_String.GetBuffer(MAX_PATH),MAX_PATH,g_achive_device_name_path);
			if(Write_String.GetLength()>=20)
			{
				Write_String = Write_String.Left(19);
			}
			WideCharToMultiByte( CP_ACP, 0, Write_String.GetBuffer(), -1, temp_buffer, 255, NULL, NULL );
			memcpy_s(Device_Basic_Setting.reg.panel_name,20,temp_buffer,20);
			if(Write_Private_Data_Blocking(WRITE_SETTING_COMMAND,0,0) <= 0)
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Update panel name failed!"));
			}
			else
			{
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Update panel name success!"));
				WritePrivateProfileStringW(temp_serial_number,NULL,NULL,g_achive_device_name_path);
			}
		}
		else	//设备的比较新 更新至本地;
		{
			CString temp_device_panel_name;
			MultiByteToWideChar( CP_ACP, 0, Device_Basic_Setting.reg.panel_name, (int)strlen(Device_Basic_Setting.reg.panel_name)+1,temp_device_panel_name.GetBuffer(MAX_PATH), MAX_PATH );
			temp_device_panel_name.ReleaseBuffer();
			if(!temp_device_panel_name.IsEmpty())
			{
				if(temp_device_panel_name.CompareNoCase(pFrame->m_product.at(selected_product_index).NameShowOnTree) != 0)
				{
					CBADO bado;
					bado.SetDBPath(g_strCurBuildingDatabasefilePath);
					bado.OnInitADOConn(); 

					//pFrame->m_pCon.CreateInstance("ADODB.Connection");
					//pFrame->m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

					CString strSql;
					strSql.Format(_T("update ALL_NODE set Product_name='%s' where Serial_ID ='%s'"),temp_device_panel_name,temp_serial_number);
					 bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);

					//if(pFrame->m_pCon->State)
					//	pFrame->m_pCon->Close();	
					if(selected_product_index < pFrame->m_product.size())
					{
						pFrame->m_pTreeViewCrl->SetItemText(pFrame->m_product.at(selected_product_index).product_item,temp_device_panel_name);
						pFrame->m_product.at(selected_product_index).NameShowOnTree = temp_device_panel_name;
					}

					bado.CloseConn();
				}
			}

			
		}
	}


}
void	CDialogCM5_BacNet::Set_Tab_Loaded_Parameter(int ntab)
{
	ASSERT(ntab < WINDOW_TAB_COUNT);
	tab_loaded[ntab] = true;
}

void CDialogCM5_BacNet::Inital_Tab_Loaded_Parameter()
{
	for (int i=0;i<WINDOW_TAB_COUNT;i++)
	{
		tab_loaded[i] = false;
	}
	read_customer_unit = false;
	receive_customer_unit = false;
}

void CDialogCM5_BacNet::OnTcnSelchangeBacMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	
	int selected = m_bac_main_tab.GetCurSel();



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

	if(m_user_level == LOGIN_SUCCESS_GRAPHIC_MODE)	//如果是图片模式只让显示图片;
	{
		if(selected != WINDOW_SCREEN)
		{
			for (int i=0;i<WINDOW_TAB_COUNT;i++)
			{
				if(i==WINDOW_SCREEN)
				{
					pDialog[i]->ShowWindow(SW_SHOW);
				}
				else
				{
					pDialog[i]->ShowWindow(SW_HIDE);
				}
			}
			if(!tab_loaded[WINDOW_SCREEN])
			{
				PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SCREENS);
				Set_Tab_Loaded_Parameter(WINDOW_SCREEN);
			}
			((BacnetScreen *)pDialog[WINDOW_SCREEN])->Fresh_Screen_List(NULL,NULL);
			((BacnetScreen*)pDialog[WINDOW_SCREEN])->Reg_Hotkey();
			g_hwnd_now = m_screen_dlg_hwnd;
			bacnet_view_number = TYPE_SCREENS;
			m_bac_main_tab.SetCurSel(WINDOW_SCREEN);
			MessageBox(_T("Insufficient User Permissions"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			Invalidate(TRUE);
		}
		return;
	}
	else if(m_user_level == LOGIN_SUCCESS_ROUTINE_MODE)
	{
		if((selected != WINDOW_WEEKLY) && (selected != WINDOW_ANNUAL))
		{
			for (int i=0;i<WINDOW_TAB_COUNT;i++)
			{
				if(i==WINDOW_WEEKLY)
				{
					pDialog[i]->ShowWindow(SW_SHOW);
				}
				else
				{
					pDialog[i]->ShowWindow(SW_HIDE);
				}
			}
			if(!tab_loaded[WINDOW_WEEKLY])
			{
				PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_WEEKLY);
				Set_Tab_Loaded_Parameter(WINDOW_WEEKLY);
			}
			((BacnetWeeklyRoutine *)pDialog[WINDOW_WEEKLY])->Fresh_Weekly_List(NULL,NULL);
			((BacnetWeeklyRoutine*)pDialog[WINDOW_WEEKLY])->Reg_Hotkey();
			g_hwnd_now = m_weekly_dlg_hwnd;
			bacnet_view_number = TYPE_WEEKLY;
			m_bac_main_tab.SetCurSel(WINDOW_WEEKLY);
			MessageBox(_T("Insufficient User Permissions"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			Invalidate(TRUE);
			return;
		}
		
	}

	for (int i=0;i<WINDOW_TAB_COUNT;i++)
	{
		if(i==selected)
		{
			pDialog[i]->ShowWindow(SW_SHOW);
			switch(i)
			{
			case WINDOW_INPUT:
				//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_INPUT);
				if(!tab_loaded[WINDOW_INPUT])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_INPUT);
					Set_Tab_Loaded_Parameter(WINDOW_INPUT);
				}
				((CBacnetInput *)pDialog[i])->Fresh_Input_List(NULL,NULL);
				g_hwnd_now = m_input_dlg_hwnd;
				bacnet_view_number = TYPE_INPUT;
				break;
			case WINDOW_OUTPUT:
				//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_OUTPUT);
				if(!tab_loaded[WINDOW_OUTPUT])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_OUTPUT);
					Set_Tab_Loaded_Parameter(WINDOW_OUTPUT);
				}

				((CBacnetOutput *)pDialog[i])->Fresh_Output_List(NULL,NULL);
				g_hwnd_now = m_output_dlg_hwnd;
				bacnet_view_number = TYPE_OUTPUT;
				break;
			case WINDOW_VARIABLE:
				if(!tab_loaded[WINDOW_VARIABLE])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_VARIABLE);
					Set_Tab_Loaded_Parameter(WINDOW_VARIABLE);
				}
				((CBacnetVariable *)pDialog[i])->Fresh_Variable_List(NULL,NULL);
				g_hwnd_now = m_variable_dlg_hwnd;
				bacnet_view_number = TYPE_VARIABLE;
				break;
			case WINDOW_PROGRAM:
				if(!tab_loaded[WINDOW_PROGRAM])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_PROGRAM);
					Set_Tab_Loaded_Parameter(WINDOW_PROGRAM);
				}
#if 0
				if(!tab_loaded[WINDOW_PROGRAM])
				{
					CString temp_applicationFolder;
					GetModuleFileName(NULL, temp_applicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
					PathRemoveFileSpec(temp_applicationFolder.GetBuffer(MAX_PATH));
					temp_applicationFolder.ReleaseBuffer();
					//AutoFlashConfigPath = ApplicationFolder + _T("//AutoFlashFile.ini");


					SaveConfigFilePath.Empty();
					CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
					unsigned int temp_serial_number = pFrame->m_product.at(selected_product_index).serial_number;//以序列号的文件名保存;
					CString temp_cs;
					temp_cs.Format(_T("%u.prg"),temp_serial_number);
					SaveConfigFilePath = temp_applicationFolder + _T("\\") + temp_cs;


					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SVAE_CONFIG);
					//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALL);
					Set_Tab_Loaded_Parameter(WINDOW_PROGRAM);
				}
#endif
				((CBacnetProgram *)pDialog[i])->Fresh_Program_List(NULL,NULL);
				((CBacnetProgram*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_pragram_dlg_hwnd;
				bacnet_view_number = TYPE_PROGRAM;
				break;
			case WINDOW_CONTROLLER:
				if(!tab_loaded[WINDOW_CONTROLLER])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_CONTROLLER);
					Set_Tab_Loaded_Parameter(WINDOW_CONTROLLER);
				}
				//PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_CONTROLLER);
				((BacnetController *)pDialog[i])->Fresh_Controller_List(NULL,NULL);
				g_hwnd_now = m_controller_dlg_hwnd;
				bacnet_view_number = TYPE_CONTROLLER;
				break;
			case WINDOW_SCREEN:
				if(!tab_loaded[WINDOW_SCREEN])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SCREENS);
					Set_Tab_Loaded_Parameter(WINDOW_SCREEN);
				}
				((BacnetScreen *)pDialog[i])->Fresh_Screen_List(NULL,NULL);
				((BacnetScreen*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_screen_dlg_hwnd;
				bacnet_view_number = TYPE_SCREENS;
				break;
			case WINDOW_WEEKLY:
				if(!tab_loaded[WINDOW_WEEKLY])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_WEEKLY);
					Set_Tab_Loaded_Parameter(WINDOW_WEEKLY);
				}
				((BacnetWeeklyRoutine *)pDialog[i])->Fresh_Weekly_List(NULL,NULL);
				((BacnetWeeklyRoutine*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_weekly_dlg_hwnd;
				bacnet_view_number = TYPE_WEEKLY;
				break;
			case WINDOW_ANNUAL:
				if(!tab_loaded[WINDOW_ANNUAL])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ANNUAL);
					Set_Tab_Loaded_Parameter(WINDOW_ANNUAL);
				}
				((BacnetAnnualRoutine *)pDialog[i])->Fresh_Annual_Routine_List(NULL,NULL);
				((BacnetAnnualRoutine*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_annual_dlg_hwnd;
				bacnet_view_number = TYPE_ANNUAL;
				break;
			case WINDOW_MONITOR:
				if(!tab_loaded[WINDOW_MONITOR])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_MONITOR);
					Set_Tab_Loaded_Parameter(WINDOW_MONITOR);
				}
				((CBacnetMonitor *)pDialog[i])->Fresh_Monitor_List(NULL,NULL);
				((CBacnetMonitor *)pDialog[i])->Fresh_Monitor_Input_List(NULL,NULL);
				((CBacnetMonitor*)pDialog[i])->Reg_Hotkey();
				g_hwnd_now = m_monitor_dlg_hwnd;
				bacnet_view_number = TYPE_MONITOR;
				break;
			case WINDOW_ALARMLOG:
				//增加刷新list和改变当前window 的hwnd;
				if(!tab_loaded[WINDOW_ALARMLOG])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_ALARMLOG);
					Set_Tab_Loaded_Parameter(WINDOW_ALARMLOG);
				}

				((CBacnetAlarmLog *)pDialog[i])->Fresh_Alarmlog_List(NULL,NULL);
				g_hwnd_now = m_alarmlog_dlg_hwnd;
				bacnet_view_number = TYPE_ALARMLOG;
				break;
			case WINDOW_TSTAT:

				if(!tab_loaded[WINDOW_TSTAT])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_TSTAT);
					Set_Tab_Loaded_Parameter(WINDOW_TSTAT);
				}
				((CBacnetTstat *)pDialog[i])->Fresh_Tstat_List(NULL,NULL);
				
				g_hwnd_now = m_tstat_dlg_hwnd;
				bacnet_view_number = TYPE_TSTAT;
				break;
			case WINDOW_SETTING:
				if(!tab_loaded[WINDOW_SETTING])
				{
					PostMessage(WM_FRESH_CM_LIST,MENU_CLICK,TYPE_SETTING);
					Set_Tab_Loaded_Parameter(WINDOW_SETTING);
				}
				((CBacnetSetting *)pDialog[i])->Fresh_Setting_UI(READ_SETTING_COMMAND,NULL);
				g_hwnd_now = m_setting_dlg_hwnd;
				bacnet_view_number = TYPE_SETTING;
				break;
			}
			
//			pDialog[i]->OnInitDialog();
		}
		else
		{
			pDialog[i]->ShowWindow(SW_HIDE);
		}
	}

	pDialog[selected]->SetFocus();

	if(old_selected_item!= selected)
		old_selected_item = selected;
	*pResult = 0;
}


BOOL CDialogCM5_BacNet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_TAB) 
	{
		if(m_bac_main_tab.IsWindowVisible() == false)
		{
			if(User_Login_Window->IsWindowVisible())
				User_Login_Window->SetFocus();
			return 1;
		}
		if(m_user_level == LOGIN_SUCCESS_GRAPHIC_MODE)	//如果是图片模式只让显示图片;
		{
			return 1;
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
