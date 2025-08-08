// BacnetScreen.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetScreen.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "global_function.h"
#include "global_define.h"
#include "MainFrm.h"
#include "BacnetScreenEdit.h"
#include "Class/md5.h"

#include "BacnetWebView.h"

#include "JsonHead.h"
#include "BacnetWebView.h"
#include <chrono>
#include <tuple>
#include <map>
int webview_run_server();
extern CString GetUserAppDataPath(LPCTSTR lpFolderName = NULL);
extern HANDLE h_create_webview_server_thread;
CBacnetScreenEdit * ScreenEdit_Window = NULL;
extern vector <MSG> My_Receive_msg;
extern CCriticalSection MyCriticalSection;
HANDLE h_read_screenlabel_thread = NULL;
HANDLE h_write_pic_thread = NULL;
HANDLE h_get_pic_thread = NULL;
CString Change_File_Path;
extern tree_product selected_product_Node; // 选中的设备信息;
IMPLEMENT_DYNAMIC(BacnetScreen, CDialogEx)

BacnetScreen::BacnetScreen(CWnd* pParent /*=NULL*/)
	: CDialogEx(BacnetScreen::IDD, pParent)
{
	window_max = true;
}

BacnetScreen::~BacnetScreen()
{
}

void BacnetScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SCREEN, m_screen_list);
}


BEGIN_MESSAGE_MAP(BacnetScreen, CDialogEx)
	ON_MESSAGE(MY_RESUME_DATA, ScreenCallBack)
	ON_MESSAGE(WM_REFRESH_BAC_SCREEN_LIST,Fresh_Screen_List)	
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Screen_Item)	
	ON_NOTIFY(NM_CLICK, IDC_LIST_SCREEN, &BacnetScreen::OnNMClickListScreen)
	ON_MESSAGE(WM_HOTKEY,&BacnetScreen::OnHotKey)//快捷键消息映射手动加入
	ON_MESSAGE(WM_SCREENEDIT_CLOSE,&BacnetScreen::Screeenedit_close_handle)//快捷键消息映射手动加入
	ON_BN_CLICKED(IDC_BUTTON_GRAPHIC_INSERT, &BacnetScreen::OnBnClickedInsert)
	ON_BN_CLICKED(IDC_WEBVIEW_BUTTON, &BacnetScreen::OnBnClickedWebViewShow)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_SCREEN_DATA, &BacnetScreen::OnBnClickedClearScreenData)

	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SCREEN, &BacnetScreen::OnNMDblclkListScreen)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(HANDLE_JSON_DATA, Handle_Json_Data)
//	ON_WM_DESTROY()
END_MESSAGE_MAP()


DWORD WINAPI  BacnetScreen::WritePictureThread(LPVOID lpVoid)
{
	BacnetScreen *pParent = (BacnetScreen *)lpVoid;
	pParent->WritePicFileFunction(Change_File_Path,screen_list_line,pParent->pic_filename);
	h_write_pic_thread = NULL;
	return 0;
}


DWORD WINAPI  BacnetScreen::GetPictureThread(LPVOID lpVoid)
{
	BacnetScreen *pParent = (BacnetScreen *)lpVoid;

	CString temp_image_fordor;
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	CString temp_now_building_name= g_strCurBuildingDatabasefilePath;
	PathRemoveFileSpec(temp_now_building_name.GetBuffer(MAX_PATH));
	temp_now_building_name.ReleaseBuffer();
	temp_image_fordor = temp_now_building_name  + _T("\\image");

	pParent->GetPicFileFunction(screen_list_line,temp_image_fordor);
	h_get_pic_thread = NULL;
	return 0;
}


// BacnetScreen message handlers

DWORD WINAPI  BacnetScreen::ReadScreenThreadfun(LPVOID lpVoid)
{
	//Write_Config_Info
	BacnetScreen *pParent = (BacnetScreen *)lpVoid;

	LoadOnlinePanelData();
	if(pParent->read_screen_label())
	{
		bac_read_which_list = BAC_READ_GRAPHIC_LABEL_INFO;
		bac_graphic_label_read_results = true;
		::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,NULL,NULL);
	}
	h_read_screenlabel_thread = NULL;
	return 0;


}

bool BacnetScreen::read_screen_label()
{
	//一直读到 下一个全是0 的 无效label;
	for (int i=0;i<BAC_GRPHIC_LABEL_GROUP;i++)
	{
		int end_temp_instance = 0;
		int ret_n;
		end_temp_instance = BAC_READ_GRPHIC_LABEL_REMAINDER + (BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i ;
		if(end_temp_instance >= BAC_GRPHIC_LABEL_COUNT)
			end_temp_instance = BAC_GRPHIC_LABEL_COUNT - 1;
		CString n_temp_result;
		ret_n = GetPrivateData_Blocking(g_bac_instance,READ_GRPHIC_LABEL_COMMAND,(BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i,end_temp_instance,sizeof(Str_label_point));
		if(ret_n)
		{
			n_temp_result.Format(_T("Read Graphic Label From %d to %d success."),(BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i,end_temp_instance);
			SetPaneString(BAC_SHOW_MISSION_RESULTS,n_temp_result);
			Sleep(10);
			if(b_stop_read_grp_label)
			{
				n_temp_result.Format(_T("Reading GRP%d: complete"),screen_list_line + 1);
				SetPaneString(BAC_SHOW_MISSION_RESULTS,n_temp_result);
				Sleep(10);
				break;
			}
		}
		else
		{
			n_temp_result.Format(_T("Read Graphic Label From %d to %d failed."),(BAC_READ_GRPHIC_LABEL_GROUP_NUMBER)*i,end_temp_instance);
			SetPaneString(BAC_SHOW_MISSION_RESULTS,n_temp_result);
			return false;
		}

		
	}


	return true;
}


int write_webview_data(int device_instance,int panel_id,int nsel_serialnumber,int viewitem, int msg_source = 0)
{
	int total_write_json_item_count = 0;
	CString Mession_ret;
	CString des_file;
	int end_temp_instance = 0;
	//m_json_item_data
	FILE* fp;
	unsigned int element = 0;

	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	CString image_fordor = g_strExePth + CString("Database\\Buildings\\") + pFrame->m_strCurMainBuildingName + _T("\\image");
	CString temp_item;
	if (msg_source == 0)
	{
		temp_item.Format(_T("%u_%d.zip"), g_selected_serialnumber, screen_list_line);
		
	}
	else if (msg_source == 1) //浏览器
	{
		temp_item.Format(_T("%u_%d.zip"), nsel_serialnumber, viewitem);
	}
	else
	{
		return -1;
	}
	des_file = image_fordor + _T("\\") + temp_item;




	char temp_des_file[MAX_PATH] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, des_file.GetBuffer(), -1, temp_des_file, MAX_PATH, NULL, NULL);

	fp = fopen(temp_des_file, "rb");
	while (!feof(fp))
	{
		element++;
		char c;
		c = getc(fp);

	}

	char* temp_buffer = new char[element];
	unsigned int data_point = 0;
	fseek(fp, 0L, SEEK_SET);
	while (!feof(fp))
	{
		temp_buffer[data_point] = getc(fp);
		data_point++;
	}
	if (element != data_point)
	{
		delete temp_buffer;
		return -3;
	}
	fclose(fp);



	//如果是采用以前的旧格式还需要改标志位,先从设备读出来，如果不是2 就改成2;
	if (GetPrivateDataSaveSPBlocking(device_instance, READ_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), 1) > 0)
	{
		memcpy(&g_Device_Basic_Setting[panel_id], &s_Basic_Setting, sizeof(Str_Setting_Info));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Read data timeout!"));
		return -1;
	}

	if (g_Device_Basic_Setting[panel_id].reg.pro_info.firmware0_rev_main * 10 + g_Device_Basic_Setting[panel_id].reg.pro_info.firmware0_rev_sub < WEBVIEW_JSON_FEATURE)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("The device doesn't support this feature . need update firmware!"));
		return -1;
	}


	if (g_Device_Basic_Setting[panel_id].reg.webview_json_flash != 2)
	{
		//如果之前是用旧的格式存储的，才改标志位为2.标志位已经为2的 就不用多此一举了
		g_Device_Basic_Setting[panel_id].reg.webview_json_flash = 2;

		if (WritePrivateData_Blocking(device_instance, WRITE_SETTING_COMMAND, 0, 0, 5, (char*)&g_Device_Basic_Setting[panel_id].reg) <= 0)
		{
			CString temp_task_info;
			temp_task_info.Format(_T("Write into device timeout!"));
			return -2;
		}
	}

	if (msg_source == 0)
	{
		m_json_screen_data.at(screen_list_line).file_data.version_high = 1;
		m_json_screen_data.at(screen_list_line).file_data.version_low = 1;
		m_json_screen_data.at(screen_list_line).file_data.zip_size = element;
		int write_screen_results = Write_Private_Data_Blocking(WRITE_JSON_SCREEN, screen_list_line, screen_list_line);
		if (write_screen_results < 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout!"));
			//这里要加回滚机制，写入失败，要把之前的数据还原回去;
			delete temp_buffer;
			return -2;
		}
		//写入的时候 需要获取目前所有的 size 已经存储的 位置;
		total_write_json_item_count = element / 200 + 1;
		for (int i = 0; i < total_write_json_item_count; i++)
		{
			if (i != (total_write_json_item_count - 1))
				memcpy(&m_json_item_data.at(10 * screen_list_line + i).all, temp_buffer + i * 200, 200);
			else
			{
				int last_size = element % 200;
				memcpy(&m_json_item_data.at(10 * screen_list_line + i).all, temp_buffer + i * 200, last_size);
			}
		}

		int temp_json_item_group = (total_write_json_item_count + BAC_READ_JSON_ITEM_GROUP_NUMBER - 1) / BAC_READ_JSON_ITEM_GROUP_NUMBER;
		if (total_write_json_item_count != 0)
		{
			for (int i = 0; i < temp_json_item_group; i++)
			{
				end_temp_instance = BAC_READ_JSON_ITEM_REMAINDER + (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i + screen_list_line * 10;
				if (end_temp_instance >= screen_list_line * 10 + 9)
					end_temp_instance = screen_list_line * 10 + 9;
				int start_index = screen_list_line * 10 + (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i;

				if (Write_Private_Data_Blocking(WRITE_JSON_ITEM, start_index, end_temp_instance) > 0)
				{
					Mession_ret.Format(_T("Write webview data from %d to %d success."), start_index, end_temp_instance);
					SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
					//write_success_count++;
					if (!offline_mode)
						Sleep(SEND_COMMAND_DELAY_TIME);
				}
				else
				{
					Mession_ret.Format(_T("Write webview data from %d to %d timeout."), start_index, end_temp_instance);
					SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
				}
			}
		}
	}
	else if (msg_source == 1)
	{
		g_json_screen_data.at(panel_id).at(viewitem).file_data.version_high = 1;
		g_json_screen_data.at(panel_id).at(viewitem).file_data.version_low = 1;
		g_json_screen_data.at(panel_id).at(viewitem).file_data.zip_size = element;
		int write_screen_results = WritePrivateData_Blocking(device_instance, WRITE_JSON_SCREEN, viewitem, viewitem, 5, (char*)&g_json_screen_data.at(panel_id).at(viewitem));
		if (write_screen_results < 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout!"));
			//这里要加回滚机制，写入失败，要把之前的数据还原回去;
			delete temp_buffer;
			return -2;
		}
		//写入的时候 需要获取目前所有的 size 已经存储的 位置;
		total_write_json_item_count = element / 200 + 1;
		for (int i = 0; i < total_write_json_item_count; i++)
		{
			if (i != (total_write_json_item_count - 1))
				memcpy(&g_json_item_data.at(panel_id).at(10 * viewitem + i).all, temp_buffer + i * 200, 200);
			else
			{
				int last_size = element % 200;
				memcpy(&g_json_item_data.at(panel_id).at(10 * viewitem + i).all, temp_buffer + i * 200, last_size);
			}
		}

		int temp_json_item_group = (total_write_json_item_count + BAC_READ_JSON_ITEM_GROUP_NUMBER - 1) / BAC_READ_JSON_ITEM_GROUP_NUMBER;
		if (total_write_json_item_count != 0)
		{
			for (int i = 0; i < temp_json_item_group; i++)
			{
				end_temp_instance = BAC_READ_JSON_ITEM_REMAINDER + (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i + viewitem * 10;
				if (end_temp_instance >= viewitem * 10 + 9)
					end_temp_instance = viewitem * 10 + 9;
				int start_index = viewitem * 10 + (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i;

				if (WritePrivateData_Blocking(device_instance, WRITE_JSON_ITEM, start_index, end_temp_instance,5, (char *)&g_json_item_data.at(panel_id).at(start_index).all) > 0)
				{
					Mession_ret.Format(_T("Write webview data from %d to %d success."), start_index, end_temp_instance);
					SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
					//write_success_count++;
					if (!offline_mode)
						Sleep(SEND_COMMAND_DELAY_TIME);
				}
				else
				{
					Mession_ret.Format(_T("Write webview data from %d to %d timeout."), start_index, end_temp_instance);
					SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
					return -3;
				}
			}
		}
	}
	else
	{
		return -4;
	}
	
	delete temp_buffer;
}

extern int save_button_click ;
LRESULT  BacnetScreen::Handle_Json_Data(WPARAM wParam, LPARAM lParam)
{
	//screen_list_line = 0;
	if (save_button_click == 0)
		return 0;
	int total_write_json_item_count = 0;
	CString Mession_ret;
	CString des_file;
	int end_temp_instance = 0;
	//m_json_item_data
	FILE* fp;
	unsigned int element = 0;

	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	CString image_fordor = g_strExePth + CString("Database\\Buildings\\") + pFrame->m_strCurMainBuildingName + _T("\\image");
	CString temp_item;

	temp_item.Format(_T("%u_%d.zip"), g_selected_serialnumber, screen_list_line);
	des_file = image_fordor + _T("\\") + temp_item;



	char temp_des_file[MAX_PATH] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, des_file.GetBuffer(), -1, temp_des_file, MAX_PATH, NULL, NULL);

	fp = fopen(temp_des_file, "rb");
	while (!feof(fp))
	{
		element++;
		char c;
		c = getc(fp);

	}

	char* temp_buffer = new char[element];
	unsigned int data_point = 0;
	fseek(fp, 0L, SEEK_SET);
	while (!feof(fp))
	{
		temp_buffer[data_point] = getc(fp);
		data_point++;
	}
	if (element != data_point)
	{
		delete temp_buffer;
		return 0;
	}
	fclose(fp);
	if (Device_Basic_Setting.reg.webview_json_flash != 2)
	{
		//如果之前是用旧的格式存储的，才改标志位为2.标志位已经为2的 就不用多此一举了
		Device_Basic_Setting.reg.webview_json_flash = 2;
		if (Write_Private_Data_Blocking(WRITE_SETTING_COMMAND, 0, 0) <= 0)
		{
			CString temp_task_info;
			temp_task_info.Format(_T("Write into device timeout!"));
			SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_task_info);
		}
	}


	m_json_screen_data.at(screen_list_line).file_data.version_high = 1;
	m_json_screen_data.at(screen_list_line).file_data.version_low = 1;
	m_json_screen_data.at(screen_list_line).file_data.zip_size = element;
	int write_screen_results = Write_Private_Data_Blocking(WRITE_JSON_SCREEN, screen_list_line, screen_list_line);
	if (write_screen_results < 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout!"));
		//这里要加回滚机制，写入失败，要把之前的数据还原回去;
		delete temp_buffer;
		return -2;
	}
	//写入的时候 需要获取目前所有的 size 已经存储的 位置;
	total_write_json_item_count = element / 200 + 1;
	for (int i = 0; i < total_write_json_item_count; i++)
	{
		if (i != (total_write_json_item_count - 1))
			memcpy(&m_json_item_data.at(10 * screen_list_line + i).all, temp_buffer + i * 200, 200);
		else
		{
			int last_size = element % 200;
			memcpy(&m_json_item_data.at(10 * screen_list_line + i).all, temp_buffer + i * 200, last_size);
		}
	}

	int temp_json_item_group = (total_write_json_item_count + BAC_READ_JSON_ITEM_GROUP_NUMBER - 1) / BAC_READ_JSON_ITEM_GROUP_NUMBER;
	if (total_write_json_item_count != 0)
	{
		for (int i = 0; i < temp_json_item_group; i++)
		{
			end_temp_instance = BAC_READ_JSON_ITEM_REMAINDER + (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i + screen_list_line * 10;
			if (end_temp_instance >= screen_list_line * 10 + 9)
				end_temp_instance = screen_list_line * 10 + 9;
			int start_index = screen_list_line * 10 + (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i;

			if (Write_Private_Data_Blocking(WRITE_JSON_ITEM, start_index, end_temp_instance) > 0)
			{
				Mession_ret.Format(_T("Write webview data from %d to %d success."), start_index, end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
				//write_success_count++;
				if (!offline_mode)
					Sleep(SEND_COMMAND_DELAY_TIME);
			}
			else
			{
				Mession_ret.Format(_T("Write webview data from %d to %d timeout."), start_index, end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
			}
		}
		//g_progress_persent = write_success_count * 100 / write_total_count;
	}
	delete temp_buffer;
	return 1;


}

LRESULT  BacnetScreen::ScreenCallBack(WPARAM wParam, LPARAM lParam)
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
		memcpy_s(&m_screen_data.at(pInvoke->mRow),sizeof(Control_group_point),&m_temp_screen_data[pInvoke->mRow],sizeof(Control_group_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_SCREEN_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		//AfxMessageBox(Show_Results);
		//MessageBox(_T("Bacnet operation fail!"));
	}
	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_screen_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_screen_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_screen_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


	if(pInvoke)
		delete pInvoke;
	return 0;
}


BOOL BacnetScreen::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
	{

		CRect list_rect,win_rect;
		m_screen_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_screen_dlg_hwnd,win_rect);
		m_screen_list.Set_My_WindowRect(win_rect);
		m_screen_list.Set_My_ListRect(list_rect);

		m_screen_list.Get_clicked_mouse_position();
		return TRUE;
	}
	if((pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_DELETE) ||
		(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_BACK))
	{
		int temp_item = -1;
		int temp_sub_item = -1;
		m_screen_list.Get_Selected_Item(temp_item,temp_sub_item);
		if((temp_item >=0) && (temp_item < BAC_SCREEN_COUNT))
		{
			if(temp_sub_item == SCREEN_PIC_FILE)
			{
				CString temp_task_info;
				memset(&m_screen_data.at(temp_item).picture_file,0,11);
				m_screen_list.SetItemText(temp_item,temp_sub_item,_T(""));
					m_screen_list.SetItemBkColor(temp_item,temp_sub_item,LIST_ITEM_CHANGED_BKCOLOR);
					temp_task_info.Format(_T("Write Screen List Item%d .Delete picture file "),temp_item + 1);
					Post_Write_Message(g_bac_instance,WRITESCREEN_T3000,temp_item,temp_item,sizeof(Control_group_point),m_screen_dlg_hwnd ,temp_task_info,temp_item,temp_sub_item);

			}
		}
	}
	if(pMsg->message==WM_NCLBUTTONDBLCLK)
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
	else if ((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F2)) //老毛要求按F2立刻刷新值;
	{
		::PostMessage(BacNet_hwd, WM_FRESH_CM_LIST, MENU_CLICK, TYPE_SCREENS);
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

//在删除这个对话框之前 先尝试保存 操作中的 label;
LRESULT BacnetScreen::Screeenedit_close_handle(WPARAM wParam,LPARAM lParam)
{
    //当用户关闭这个窗口时，清空所以的队列，即便有正常的消息，也清空，否则100多条消息 全堵在这里
    MyCriticalSection.Lock();
    My_Receive_msg.clear();
    MyCriticalSection.Unlock();

	Reg_Hotkey();
	if(ScreenEdit_Window)
	{
		bool saving_ret = false;
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Saving data , please wait!"));
		saving_ret = ScreenEdit_Window->UpdateDeviceLabelFlash();
		//if(((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_SQLITE_INFO)))
		//{
		//	CString temp_info;
		//	temp_info.Format(_T("Close sqlit DB"));
		//	DFTrace(temp_info);
		//}
		if(saving_ret)
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Saving data success!"));
		else
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Saving data failed!"));
		//ScreenEdit_Window->SqliteDBT3000.closedb();
		delete ScreenEdit_Window;
		ScreenEdit_Window = NULL;
		
		
	}




	return 0;
}

bool BacnetScreen::run_old_graphic_screen()
{

	if ((Device_Basic_Setting.reg.webview_json_flash == 2) &&//这里要判断是2
		(Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= WEBVIEW_JSON_FEATURE)) //643 版本会有这个功能
	{
		if (IDNO == MessageBox(_T("Switching to 'Graphic Classical View' will erase all data from 'Webview Graphic'. Are you sure you want to erase and switch to 'Classical View'"), _T(""), MB_YESNO | MB_ICONINFORMATION))
		{
			return 0;
		}
	}
	else
	{
		MessageBox(_T("'Graphic Classical View' have some data in it, If you want use 'Webview Graphic' ,you should click 'Clear All Graphic Data' button!"));
	}
	if (h_read_screenlabel_thread == NULL)
	{
		h_read_screenlabel_thread = CreateThread(NULL, NULL, ReadScreenThreadfun, this, NULL, NULL);
	}
}

LRESULT BacnetScreen::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if (wParam==KEY_INSERT)
	{
		for (int i=0;i<m_screen_list.GetItemCount();++i)
		{
			if(m_screen_list.GetCellChecked(i,0))
			{
				screen_list_line = i;
				break;
			}
		}



		//按下快捷键或者原始grpahic按键 显示旧的UI
		//int nret = CheckOldGraphic();
		//if(nret == 0)
			run_old_graphic_screen();
		//else
		//	PostMessage(WM_COMMAND, MAKEWPARAM(IDC_WEBVIEW_BUTTON, BN_CLICKED), NULL);

		return 0;
		
	}
	return 0;
}



BOOL BacnetScreen::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW(_T("Graphic"));
	
	Initial_List();
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_GRAPHIC);
	SetIcon(m_hIcon,TRUE);
	PostMessage(WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);

	ShowWindow(FALSE);
//	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//F2键
	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void BacnetScreen::OnOK()
{
	

	CDialogEx::OnOK();
}


void BacnetScreen::Initial_List()
{
	m_screen_list.ShowWindow(SW_HIDE);
	m_screen_list.DeleteAllItems();
	while ( m_screen_list.DeleteColumn (0)) ;

	m_screen_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_screen_list.SetExtendedStyle(m_screen_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_screen_list.InsertColumn(SCREEN_NUM, _T("Graphic"), 60, ListCtrlEx::CheckBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_screen_list.InsertColumn(SCREEN_DESCRIPTION, _T("Full Label"), 180, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_screen_list.InsertColumn(SCREEN_LABEL, _T("Label"), 120, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_screen_list.InsertColumn(SCREEN_PIC_FILE, _T("Picture File"), 140, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	//m_screen_list.InsertColumn(SCREEN_MODE, _T("Mode"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_screen_list.InsertColumn(SCREEN_ELEMENT_COUNT, _T("Element Count"), 110, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_screen_list.Setlistcolcharlimit(SCREEN_DESCRIPTION,STR_SCREEN_DESCRIPTION_LENGTH -1);
	m_screen_list.Setlistcolcharlimit(SCREEN_LABEL,STR_SCREEN_LABLE_LENGTH-1);

	m_screen_dlg_hwnd = this->m_hWnd;
	m_screen_list.SetListHwnd(this->m_hWnd);
	
	CRect list_rect,win_rect;
	m_screen_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_screen_dlg_hwnd,win_rect);
	m_screen_list.Set_My_WindowRect(win_rect);
	m_screen_list.Set_My_ListRect(list_rect);


	m_screen_list.DeleteAllItems();
	for (int i=0;i<(int)m_screen_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

		if(i>=screen_item_limit_count)
			break;

		temp_item.Format(_T("%d"),i+1);
		m_screen_list.InsertItem(i,temp_item);
		m_screen_list.SetCellEnabled(i,SCREEN_NUM,0);
		m_screen_list.SetCellChecked(0,0,1);// default will set the first one checked


		for (int x=0;x<SCREEN_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_screen_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_screen_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

	}
	m_screen_list.InitListData();
	m_screen_list.ShowWindow(SW_SHOW);
}

LRESULT BacnetScreen::Fresh_Screen_List(WPARAM wParam, LPARAM lParam)
{
	int Fresh_Item;
	int isFreshOne = (int)lParam;

	if (isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	//else
	//{
	//	if(m_screen_list.IsDataNewer((char *)&m_screen_data.at(0),sizeof(Control_group_point) * BAC_SCREEN_COUNT))
	//	{
	//		//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
	//		m_screen_list.SetListData((char *)&m_screen_data.at(0),sizeof(Control_group_point) * BAC_SCREEN_COUNT);
	//	}
	//	else
	//	{
	//		return 0;
	//	}
	//}

	for (int i = 0; i < (int)m_screen_data.size(); i++)
	{
		CString temp_des, temp_label, temp_pic_file;

		if (i >= screen_item_limit_count)
			break;

		if (isFreshOne)
		{
			i = Fresh_Item;
		}

		MultiByteToWideChar(CP_ACP, 0, (char*)m_screen_data.at(i).description, (int)strlen((char*)m_screen_data.at(i).description) + 1,
			temp_des.GetBuffer(MAX_PATH), MAX_PATH);
		temp_des.ReleaseBuffer();
		temp_des = temp_des.Left(STR_SCREEN_DESCRIPTION_LENGTH).Trim();
		m_screen_list.SetItemText(i, SCREEN_DESCRIPTION, temp_des);

		MultiByteToWideChar(CP_ACP, 0, (char*)m_screen_data.at(i).label, (int)strlen((char*)m_screen_data.at(i).label) + 1,
			temp_label.GetBuffer(MAX_PATH), MAX_PATH);
		temp_label.ReleaseBuffer();
		temp_label = temp_label.Left(STR_SCREEN_LABLE_LENGTH).Trim();
		m_screen_list.SetItemText(i, SCREEN_LABEL, temp_label);


		MultiByteToWideChar(CP_ACP, 0, (char*)m_screen_data.at(i).picture_file, (int)strlen((char*)m_screen_data.at(i).picture_file) + 1,
			temp_pic_file.GetBuffer(MAX_PATH), MAX_PATH);
		temp_pic_file.ReleaseBuffer();

		m_screen_list.SetItemText(i, SCREEN_PIC_FILE, temp_pic_file);

		//m_screen_list.SetItemText(i,SCREEN_MODE,_T("Graphic"));
		CString cs_refresh_time;
		if(Device_Basic_Setting.reg.webview_json_flash == 2)
			cs_refresh_time.Format(_T("%d"), m_screen_data.at(i).webview_element_count);
		else
			cs_refresh_time.Format(_T("%d"), m_screen_data.at(i).old_type_element_count);
		m_screen_list.SetItemText(i, SCREEN_ELEMENT_COUNT, cs_refresh_time);
		if (isFreshOne)
		{
			break;
		}
	}
#ifdef LOCAL_DB_FUNCTION
	if (selected_product_Node.serial_number != 0)
		WriteDeviceDataIntoAccessDB(BAC_GRP, screen_item_limit_count, selected_product_Node.serial_number);
#endif

	return 0;
}

LRESULT BacnetScreen::Fresh_Screen_Item(WPARAM wParam, LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	CString temp_task_info;
	CString New_CString = m_screen_list.GetItemText(Changed_Item, Changed_SubItem);
	CString cstemp_value;

	memcpy_s(&m_temp_screen_data[Changed_Item], sizeof(Control_group_point), &m_screen_data.at(Changed_Item), sizeof(Control_group_point));

	if (Changed_SubItem == SCREEN_DESCRIPTION)
	{


		CString cs_temp = m_screen_list.GetItemText(Changed_Item, Changed_SubItem);
		if (cs_temp.GetLength() >= STR_SCREEN_DESCRIPTION_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Warning"), _T("Length can not higher than 20"));
			PostMessage(WM_REFRESH_BAC_SCREEN_LIST, NULL, NULL);
			return 0;
		}
		if (Check_FullLabel_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_SCREEN_LIST, Changed_Item, REFRESH_ON_ITEM);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1, 0, 255);
		WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
		memcpy_s(m_screen_data.at(Changed_Item).description, STR_SCREEN_DESCRIPTION_LENGTH, cTemp1, STR_SCREEN_DESCRIPTION_LENGTH);
	}
	else if (Changed_SubItem == SCREEN_LABEL)
	{
		CString cs_temp = m_screen_list.GetItemText(Changed_Item, Changed_SubItem);
		if (cs_temp.GetLength() >= STR_SCREEN_LABLE_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 8"), _T("Warning"));
			PostMessage(WM_REFRESH_BAC_SCREEN_LIST, NULL, NULL);
			return 0;
		}
		cs_temp.MakeUpper();
		if (Check_Label_Exsit(cs_temp))
		{
			PostMessage(WM_REFRESH_BAC_SCREEN_LIST, Changed_Item, REFRESH_ON_ITEM);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1, 0, 255);
		WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
		memcpy_s(m_screen_data.at(Changed_Item).label, STR_SCREEN_LABLE_LENGTH, cTemp1, STR_SCREEN_LABLE_LENGTH);
	}
	else if (Changed_SubItem == SCREEN_PIC_FILE)
	{

	}
	else if (Changed_SubItem == SCREEN_ELEMENT_COUNT)
	{
		int temp_value;
		if ((New_CString.GetLength() >= 4) || (_wtoi(New_CString) > 255))
		{
			MessageBox(_T("Please input a value between 0 - 255"), _T("Warning"), MB_OK | MB_ICONINFORMATION);
			PostMessage(WM_REFRESH_BAC_SCREEN_LIST, NULL, NULL);//restore the list.
			return 0;
		}
		temp_value = _wtoi(New_CString);
		m_screen_data.at(Changed_Item).old_type_element_count = temp_value;
	}
	int cmp_ret = memcmp(&m_temp_screen_data[Changed_Item], &m_screen_data.at(Changed_Item), sizeof(Control_group_point));
	if (cmp_ret != 0)
	{
		m_screen_list.SetItemBkColor(Changed_Item, Changed_SubItem, LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Screen List Item%d .Changed to \"%s\" "), Changed_Item + 1, New_CString);
		Post_Write_Message(g_bac_instance, WRITESCREEN_T3000, Changed_Item, Changed_Item, sizeof(Control_group_point), m_screen_dlg_hwnd, temp_task_info, Changed_Item, Changed_SubItem);

	}
	return 0;
}




void BacnetScreen::OnNMDblclkListScreen(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int device_obj_instance = g_bac_instance;
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
	m_screen_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_screen_list.SubItemHitTest(&lvinfo);
	if(nItem!=-1)
	{ 
		m_screen_list.SetCellChecked(nItem,0,1);
		screen_list_line = nItem;
		for (int i=0;i<m_screen_list.GetItemCount();++i)
		{
			if(i == nItem)
				continue;
			m_screen_list.SetCellChecked(i,0,FALSE);
		}
	}
	int lRow = 0;
	int lCol = 0;
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_screen_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的;
		return;
	if(lRow<0)
		return;
	m_screen_list.Set_Edit(true);
	CString New_CString;
	CString temp_task_info;

	if(lCol == SCREEN_PIC_FILE)
	{
		TRACE(_T("Double"));
		CString FilePath;
		CString image_fordor;
		CString ApplicationFolder;
		GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
		PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
		ApplicationFolder.ReleaseBuffer();
		//image_fordor = ApplicationFolder + _T("\\Database\\image");
		CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		CString temp_now_building_name= g_strCurBuildingDatabasefilePath;
		PathRemoveFileSpec(temp_now_building_name.GetBuffer(MAX_PATH));
		temp_now_building_name.ReleaseBuffer();
		image_fordor = temp_now_building_name  + _T("\\image");
		
		WIN32_FIND_DATA fd;
		BOOL ret = FALSE;
		HANDLE hFind = FindFirstFile(image_fordor, &fd);
		if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			//目录存在
			ret = TRUE;
		}
		else
		{
			SECURITY_ATTRIBUTES attrib;
			attrib.bInheritHandle = FALSE;
			attrib.lpSecurityDescriptor = NULL;
			attrib.nLength = sizeof(SECURITY_ATTRIBUTES);

			CreateDirectory( image_fordor, &attrib);
		}



		SetCurrentDirectoryW(image_fordor);
		//选择图片,如果选的不在database目录下就copy一份过来;如果在的话就重命名，因为文件名长度不能超过10个字节;
		CString strFilter = _T("jpg file;bmp file;png file|*.jpg;*.bmp;*.png|all File|*.*||");
		CFileDialog dlg(true,_T("bmp"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
		if(IDOK==dlg.DoModal())
		{
			FilePath=dlg.GetPathName();
			CString FileName;
			GetFileNameFromPath(FilePath,FileName);
			
			CString temp1;
			temp1 = FilePath;
			PathRemoveFileSpec(temp1.GetBuffer(MAX_PATH));
			temp1.ReleaseBuffer();

			//The next part of code change file name length larger than 11 and rename it  move to image folder.
			// Eg : T3000Building.bmp  ->  T3000B.bmp
			CString extension_name;
			extension_name = PathFindExtension(FileName.GetBuffer());
			int extension_length = extension_name.GetLength();
			if(extension_length > 10)
				return;
			PathRemoveExtension(FileName.GetBuffer(MAX_PATH));
			FileName.ReleaseBuffer();
			if(FileName.GetLength() >=11 - extension_length)
			{
				FileName = FileName.Left(10 - extension_length);
				FileName = FileName + extension_name;
			}
			CString new_file_path;
			new_file_path = image_fordor + _T("\\") + FileName;
			Change_File_Path = new_file_path;
			if(temp1.CompareNoCase(image_fordor) != 0)//如果就在当前目录就不用copy过来了;
			{
				CopyFile(FilePath,new_file_path,false);
			}
			else
			{
				CFile::Rename(FilePath, new_file_path);
			}
			memcpy_s(&m_temp_screen_data[lRow],sizeof(Control_group_point),&m_screen_data.at(lRow),sizeof(Control_group_point));

			char cTemp1[255];
			memset(cTemp1,0,255);
			WideCharToMultiByte( CP_ACP, 0, FileName.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
			memcpy_s(m_screen_data.at(lRow).picture_file,STR_SCREEN_PIC_FILE_LENGTH,cTemp1,STR_SCREEN_PIC_FILE_LENGTH);

			m_screen_list.SetItemText(lRow,SCREEN_PIC_FILE,FileName);
			New_CString = FileName;
			pic_filename = FileName;

			//if(h_write_pic_thread==NULL)
			//{
			//	if(Device_Basic_Setting.reg.sd_exist)
			//		h_write_pic_thread =CreateThread(NULL,NULL,WritePictureThread,this,NULL, NULL);
			//}

		}
	}
	else
	{
		for (int i=0;i<m_screen_list.GetItemCount();++i)
		{
			if(m_screen_list.GetCellChecked(i,0))
			{
				screen_list_line = i;
				break;
			}
		}

		PostMessage(WM_COMMAND, MAKEWPARAM(IDC_WEBVIEW_BUTTON, BN_CLICKED), NULL);
#if 0
		if(h_read_screenlabel_thread==NULL)
		{
			h_read_screenlabel_thread =CreateThread(NULL,NULL,ReadScreenThreadfun,this,NULL, NULL);
		}
#endif
		*pResult = 0;
		return;
	}

	//m_screen_list.Set_Edit(false);

	int cmp_ret = memcmp(&m_temp_screen_data[lRow],&m_screen_data.at(lRow),sizeof(Control_group_point));
	if(cmp_ret!=0)
	{
		m_screen_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Screen List Item%d .Changed to \"%s\" "),lRow + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITESCREEN_T3000,lRow,lRow,sizeof(Control_group_point),m_screen_dlg_hwnd ,temp_task_info,lRow,lCol);

	}


	*pResult = 0;
}


void BacnetScreen::OnClose()
{
	ShowWindow(FALSE);
	return;
	 
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
	KillTimer(1);
	CDialogEx::OnClose();
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
}

void BacnetScreen::OnCancel()
{
	
//	KillTimer(1);
//	m_screen_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
//	CDialogEx::OnCancel();
}

void BacnetScreen::OnTimer(UINT_PTR nIDEvent)
{
	 
	if(g_protocol == PROTOCOL_BIP_TO_MSTP)
	{
		PostMessage(WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
	}
	else if((this->IsWindowVisible()) && (Gsm_communication == false) &&  ((this->m_hWnd  == ::GetActiveWindow()) || (bacnet_view_number == TYPE_SCREENS))  )	//GSM连接时不要刷新;
	{
	PostMessage(WM_REFRESH_BAC_SCREEN_LIST,NULL,NULL);
	if(bac_select_device_online)
		Post_Refresh_Message(g_bac_instance,READSCREEN_T3000,0,BAC_SCREEN_COUNT - 1,sizeof(Control_group_point),BAC_SCREEN_GROUP);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void BacnetScreen::Reg_Hotkey()
{
	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//Insert键
}

void BacnetScreen::Unreg_Hotkey()
{
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
}

void BacnetScreen::OnBnClickedInsert()
{
#if 0

	int test_count2 = GetPrivateData_Blocking(g_bac_instance, READ_JSON_ITEM, 4, 4, sizeof(Str_item_Json));
	Sleep(1);
	return;
	int test_count = GetPrivateData_Blocking(g_bac_instance, READ_JSON_SCREEN,3, 3, sizeof(Str_t3_screen_Json));
	Sleep(1);

	return;
	using json = nlohmann::json;
	char text[] = R"(
    {
     "activeItemIndex":14,
    "customObjectsCount":0,
    "elementGuidelines":[

    ],
    "groupCount":9,
    "itemsCount":24,
    "selectedTargets":[

    ],
"items":[
        {
            "active":true,
            "group":9,
            "height":53,
            "id":1,
            "rotate":0,
            "scaleX":1,
            "scaleY":1,
            "settings":{
                "active":true,
                "bgColor":"inherit",
                "fontSize":16,
                "inAlarm":false,
                "t3EntryDisplayField":"control",
                "textColor":"inherit",
                "titleColor":"inherit"
            },
            "t3Entry":{
                "auto_manual":0,
                "calibration_h":0,
                "calibration_l":0,
                "calibration_sign":1,
                "command":"144IN7",
                "control":1,
                "description":"IN7",
                "digital_analog":1,
                "filter":5,
                "id":"IN7",
                "index":6,
                "label":"IN7",
                "pid":144,
                "range":3,
                "type":"INPUT",
                "unit":3,
                "value":-40000
            },
            "title":null,
            "translate":[
                22,
                167,
                0,
                0
            ],
            "width":65
        },
        {
            "active":false,
            "group":3,
            "height":77,
            "id":2,
            "rotate":2,
            "scaleX":111,
            "scaleY":11,
            "settings":{
                "active":false,
                "bgColor":"inherit",
                "fontSize":16,
                "inAlarm":false,
                "t3EntryDisplayField":"control",
                "textColor":"inherit",
                "titleColor":"inherit"
            },
            "t3Entry":{
                "auto_manual":1,
                "command":"144OUT3",
                "control":0,
                "description":"OUT2",
                "digital_analog":1,
                "high_voltage":0,
                "hw_switch_status":1,
                "id":"OUT1",
                "index":8,
                "label":"OUT9",
                "low_voltage":0,
                "pid":144,
                "range":0,
                "type":"OUTPUT",
                "value":0
            },
            "title":null,
            "translate":[
                117,
                114,
                0,
                0
            ],
            "type":"Fan",
            "width":15,
            "zindex":1
        },
        {
            "active":false,
            "group":2,
            "height":77,
            "id":2,
            "rotate":0,
            "scaleX":1,
            "scaleY":1,
            "settings":{
                "active":false,
                "bgColor":"inherit",
                "fontSize":16,
                "inAlarm":false,
                "t3EntryDisplayField":"control",
                "textColor":"inherit",
                "titleColor":"inherit"
            },
            "t3Entry":{
                "auto_manual":0,
                "command":"144OUT9",
                "control":0,
                "description":"OUT9",
                "digital_analog":1,
                "high_voltage":0,
                "hw_switch_status":1,
                "id":"OUT9",
                "index":8,
                "label":"OUT9",
                "low_voltage":0,
                "pid":144,
                "range":0,
                "type":"OUTPUT",
                "value":0
            },
            "title":null,
            "translate":[
                167,
                154,
                0,
                0
            ],
            "type":"Fan",
            "width":65,
            "zindex":1
        }
      ],   
    "version":"0.4.4",
    "viewportTransform":{
        "scale":1,
        "x":0,
        "y":0
    } 
}
    )";

	// parse and serialize JSON
	//json j_complete = json::parse(text);



#if 0
	json j;
	j["name"] = "Ned Flanders";
	j["address"] = "744 Evergreen Terrace";
	j["age"] = 60;

	auto p = j.template get<ns::person>();

	std::cout << p.name << " (" << p.age << ") lives in " << p.address << std::endl;
#endif
	// JSON string representing the data
#if 1
	//Test use this json file
	CString fn = _T("C:\\Work\\T3000_Building_Automation_System\\T3000 Output\\debug\\Database\\Buildings\\Default_Building\\image\\178157_0.txt");//这里如果用VS可能要来个强制转换(char*)"a.txt"

	CFile filelib;
	filelib.Open(fn, CFile::modeRead, NULL);
	DWORD len = filelib.GetLength();

	WCHAR* nlibbuff = new WCHAR[len + 1];
	memset(nlibbuff, 0, 2 * (len + 1));
	filelib.Read(nlibbuff, len * 2 + 1);   //Read( void* lpBuf, UINT nCount ) lpBuf是用于接收读取到的数据的Buf指针nCount是从文件读取的字节数
	wstring nbuff_wstring(nlibbuff);
	string file_content(nbuff_wstring.begin(), nbuff_wstring.end());
	//tempjson["library"] = file_content;
	filelib.Close();
	delete nlibbuff;

	json jsonData = json::parse(nbuff_wstring);
#endif
	//json jsonData = json::parse(text);

	// Convert JSON data to the struct using the constructor
	Str_Json Str_MyJson(jsonData);

	// Access and print the struct members
	//std::cout << "Name: " << person.name << std::endl;
	//std::cout << "Age: " << person.age << std::endl;
	//std::cout << "Occupation: " << person.occupation << std::endl;
#endif
	PostMessage(WM_HOTKEY,KEY_INSERT,NULL);
}

int GetScreenLabel(int index,CString &ret_label)
{
	if(index >= BAC_SCREEN_COUNT)
	{
		ret_label.Empty();
		return -1;
	}
	int i = index;
	CString temp_des2;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(i).label, (int)strlen((char *)m_screen_data.at(i).label)+1, 
		ret_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_label.ReleaseBuffer();

	if(ret_label.IsEmpty())
	{
		ret_label.Format(_T("GRP%d"),index + 1);
	}

	return 1;
}

int GetScreenFullLabel(int index,CString &ret_full_label)
{
	if(index >= BAC_SCREEN_COUNT)
	{
		ret_full_label.Empty();
		return -1;
	}
	int i = index;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(i).description, (int)strlen((char *)m_screen_data.at(i).description)+1, 
		ret_full_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_full_label.ReleaseBuffer();

	if(ret_full_label.IsEmpty())
	{
		ret_full_label.Format(_T("GRP%d"),index + 1);
	}

	return 1;
}


int BacnetScreen::WritePicFileFunction(CString ChooseFilePath,unsigned char screen_index,CString pic_filename)
{

	//TRACE(_T("Test"));
	//CString ChooseFilePath;
	//CFileDialog dlg(true,_T("*.*"),_T(" "),OFN_HIDEREADONLY ,_T("Pic files (*.*)|*.*||"),NULL,0);
	//if(IDOK!=dlg.DoModal())
	//	return ;
	//ChooseFilePath=dlg.GetPathName();
	//MD5是一个32bit的值 按字符存的;




#pragma region read file buffer
	pic_sd_struct temp_pic;
	memset(&temp_pic,0,sizeof(pic_sd_struct));
	CFile Filetxt;//用来读取位图文件
	DWORD FileLen=0;//位图的长度
	char* FileBuff;//用于存放位图信息

	//unsigned int pic_file_size = 0;
	string temp_md5;// = MD5(ifstream(ChooseFilePath)).toString();
	CString MD5_value;
	MD5_value = temp_md5.c_str();
	CString temp_show;
	temp_show.Format(_T("The File MD5 is :"));


	if(!Filetxt.Open(ChooseFilePath,CFile::modeRead))//打开文件
	{
		//MessageBox(NULL,"打开文本信息失败!",NULL, MB_OK);
		return false;
	}
	FileLen=Filetxt.GetLength();//得到位图的长度
	FileBuff=new char[FileLen+1];//给位图文件申请内在空间
	DWORD DwPic=Filetxt.GetLength();
	memset(FileBuff,0,FileLen+1);//初始化位图文件的空间
	if(!FileBuff)//判断位图空间是否申请成功
	{
		return false;
	}
	if(Filetxt.Read(FileBuff,FileLen)!=FileLen)//读取文本信息，存入到FileBuff中去
	{
		return false;
	}
	temp_pic.pic_file_size = FileLen;
	//unsigned int total_packet = 0;
	unsigned int temp_filepack = 0;
	unsigned int last_packet_data_size = 0;
	if(FileLen % 400 == 0)
	{
		temp_filepack = FileLen / 400;
		last_packet_data_size = 400;
	}
	else
	{
		temp_filepack =  FileLen / 400  + 1;
		last_packet_data_size = FileLen % 400;
	}

	temp_pic.total_packet = temp_filepack + 1; //其中 1是 第一包的头 传 MD5值和 文件总包数.;

	//char crc_cal[4];
	temp_pic.crc_cal[0] = 0x55 ; temp_pic.crc_cal[1] = 0xff ;temp_pic.crc_cal[2] = 0x55 ;temp_pic.crc_cal[3] = 0xff ; //第一包里面用 前4个字节 0x55ff55ff来 区别旧版本 不回复的信息;
	//char md5_32byte[33];
	memset(temp_pic.md5_32byte,0,33);
	WideCharToMultiByte( CP_ACP, 0, MD5_value.GetBuffer(), -1, temp_pic.md5_32byte, 255, NULL, NULL );

	if(pic_filename.GetLength() >= 11)
		pic_filename = pic_filename.Left(10);
	WideCharToMultiByte( CP_ACP, 0, pic_filename.GetBuffer(), -1, temp_pic.file_name, 255, NULL, NULL );

	char send_buffer[400];
	memset(send_buffer,0 , 400);
	CString temp_cs_show;
	int device_obj_instance = g_bac_instance;
	for (int z=0;z<temp_pic.total_packet;z++)
	{
		memset(send_buffer,0,400);
		if(z == 0)
		{
			memcpy(send_buffer,&temp_pic,sizeof(pic_sd_struct));

			//memcpy(send_buffer,temp_pic.crc_cal,4);
			//memcpy(send_buffer+4,&temp_pic.total_packet,4);
			//memcpy(send_buffer+8,&temp_pic.pic_file_size,4);		
			//memcpy(send_buffer+12,temp_pic.md5_32byte,33);
		}
		else if(z < temp_pic.total_packet - 1)
		{
			memcpy(send_buffer,FileBuff + (z-1)*400,400);
		}
		else
		{
			memcpy(send_buffer,FileBuff + (z-1)*400,last_packet_data_size);
		}
		if(WriteBacnetPictureData_Blocking(device_obj_instance,screen_index,z,temp_pic.total_packet,(unsigned char *)send_buffer)< 0)
		{
			MessageBox(_T("Write picture file into SD disk timeout."));
			return false;

		}
		g_progress_persent = z * 100 /temp_pic.total_packet;
		
		temp_cs_show.Format(_T("Saving picture file into SD disk .(%u/%u)"),z*400,(temp_pic.total_packet)*400+last_packet_data_size);
		SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_cs_show);

	}
	g_progress_persent = 100;
	temp_cs_show.Format(_T("Saving picture file into SD disk success."));
	SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_cs_show);
	return true;
#pragma endregion read file buffer

}



int BacnetScreen::GetPicFileFunction(unsigned char screen_index ,CString temp_image_folder)
{

	//TRACE(_T("Test"));
	//CString ChooseFilePath;
	//CFileDialog dlg(true,_T("*.*"),_T(" "),OFN_HIDEREADONLY ,_T("Pic files (*.*)|*.*||"),NULL,0);
	//if(IDOK!=dlg.DoModal())
	//	return ;
	//ChooseFilePath=dlg.GetPathName();
	//MD5是一个32bit的值 按字符存的;
	pic_sd_struct temp_picture;
	memset(&temp_picture,0,sizeof(pic_sd_struct));
	//char md5_value[33];
	//unsigned int total_packet = 0;
	//unsigned int total_pic_size = 0;
	unsigned int last_packet_size = 0;
	//memset(md5_value,0,33);
	memset(&m_picture_head,0,20);
	memset(picture_data_buffer,0,400);
	char * temp_point = picture_data_buffer;
	int device_obj_instance = g_bac_instance;
	if(GetPictureBlockData_Blocking(device_obj_instance,screen_index,0,0) > 0)
	{
		if((m_picture_head.seg_index != 0) || (m_picture_head.index != screen_list_line)  || (m_picture_head.total_seg != 0))
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Get picture file from SD card failed!"));
			return 0;
		}
		else if(((unsigned char)temp_point[0]!= 0x55) || ((unsigned char)temp_point[1]!= 0xff) || ((unsigned char)temp_point[2]!= 0x55) ||((unsigned char)temp_point[3]!= 0xff))
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Get picture file from SD card failed!CRC error!"));
			return 0;
		}
		temp_point = temp_point + 4;
		temp_picture.total_packet = ((unsigned char)temp_point[3])<<24 | ((unsigned char)temp_point[2]<<16) | ((unsigned char)temp_point[1])<<8 | ((unsigned char)temp_point[0]);
		temp_point = temp_point + 4;
		temp_picture.pic_file_size = ((unsigned char)temp_point[3])<<24 | ((unsigned char)temp_point[2]<<16) | ((unsigned char)temp_point[1])<<8 | ((unsigned char)temp_point[0]);
		temp_point = temp_point + 4;
		memcpy(temp_picture.md5_32byte,temp_point,33);
		temp_point = temp_point + 33;
		memcpy(temp_picture.file_name,temp_point,11);
	}
	else
	{
		AfxMessageBox(_T("Read Data Timeout"));
		return 0;
	}

	//比对MD5 与设备的是否一致;
	CString temp_pic_md5_cs;
	MultiByteToWideChar( CP_ACP, 0, temp_picture.md5_32byte, (int)strlen((char *)temp_picture.md5_32byte)+1, 
		temp_pic_md5_cs.GetBuffer(MAX_PATH), MAX_PATH );
	temp_pic_md5_cs.ReleaseBuffer();

	CString temp_file_name;
	MultiByteToWideChar( CP_ACP, 0, temp_picture.file_name, (int)strlen((char *)temp_picture.file_name)+1, 
		temp_file_name.GetBuffer(MAX_PATH), MAX_PATH );
	temp_file_name.ReleaseBuffer();



	CString Picture_path;
	CString PicFileTips;
	CString temp_image_folde;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(screen_list_line).picture_file, (int)strlen((char *)m_screen_data.at(screen_list_line).picture_file)+1, 
		PicFileTips.GetBuffer(MAX_PATH), MAX_PATH );
	PicFileTips.ReleaseBuffer();

	CString temp_now_building_name= g_strCurBuildingDatabasefilePath;
	PathRemoveFileSpec(temp_now_building_name.GetBuffer(MAX_PATH));
	temp_now_building_name.ReleaseBuffer();
	temp_image_folde = temp_now_building_name  + _T("\\image");

	Picture_path=temp_image_folde + _T("\\") + PicFileTips ;//_T("sample1.bmp");

	CFileFind temp_find;
	if(temp_find.FindFile(Picture_path))
	{
		string temp_md5;// = MD5(ifstream(Picture_path)).toString();
		CString MD5_value;
		MD5_value = temp_md5.c_str();
		if(MD5_value.CompareNoCase(temp_pic_md5_cs) == 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Picture already exsit in image folder!"));
			return 0;
		}
		else
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Picture changed , ready to update!"));
		}
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("No picture found in image folder , ready to download!"));
	}
	
	if(temp_picture.pic_file_size % 400 == 0)
	{
		last_packet_size = 400;
	}
	else
	{
		last_packet_size = temp_picture.pic_file_size % 400;
	}


	char *ReadBuff=new char[temp_picture.pic_file_size+1];//给位图文件申请内在空间;
	if(ReadBuff == NULL)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("No enough memory!"));
		return 0;
	}
	memset(ReadBuff,0,temp_picture.pic_file_size+1);



	for (int i=1;i<temp_picture.total_packet;i++)
	{
		memset(picture_data_buffer,0,400);
		if(GetPictureBlockData_Blocking(g_bac_instance,screen_index,i,i) > 0)
		{
			if(i!= (temp_picture.total_packet-1))
				memcpy(ReadBuff + (i-1)*400,picture_data_buffer,400);
			else
				memcpy(ReadBuff + (i-1)*400,picture_data_buffer,last_packet_size);	//最后一包只copy 剩余的 字节;

			CString temp_cs_complet;
			temp_cs_complet.Format(_T("Read picture file %d / %d"), i*400  ,temp_picture.pic_file_size);
			SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_cs_complet);
			g_progress_persent = i * 100 /temp_picture.total_packet;
		}
		else
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read picture file timeout , please try again"));
		}
	}

	CString temp_cs_finished;
	temp_cs_finished.Format(_T("Read picture file complete"));
	SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_cs_finished);
	g_progress_persent = 100;
#pragma region write_file_buffer22

	HANDLE hFile;
	CString write_file_path;
	write_file_path = temp_image_folder + _T("\\") + temp_file_name;
	
	CFileFind temp_find_pic;
	if(temp_find_pic.FindFile(write_file_path))
	{
		DeleteFile(write_file_path);
	}


	hFile=CreateFile(write_file_path,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD dWrites;
	WriteFile(hFile,ReadBuff,temp_picture.pic_file_size,&dWrites,NULL);
	CloseHandle(hFile);  
	if(ReadBuff)
	{
		delete ReadBuff;
		ReadBuff = NULL;
	}

#pragma endregion write_file_buffer22


	



		return 1;
#if 0
#pragma region read file buffer
	CFile Filetxt;//用来读取位图文件
	DWORD FileLen=0;//位图的长度
	char* FileBuff;//用于存放位图信息


	string temp_md5 = MD5(ifstream( ChooseFilePath )).toString();
	CString MD5_value;
	MD5_value = temp_md5.c_str();
	CString temp_show;
	temp_show.Format(_T("The File MD5 is :"));


	if(!Filetxt.Open(ChooseFilePath,CFile::modeRead))//打开文件
	{
		//MessageBox(NULL,"打开文本信息失败!",NULL, MB_OK);
		return false;
	}
	FileLen=Filetxt.GetLength();//得到位图的长度
	FileBuff=new char[FileLen+1];//给位图文件申请内在空间
	DWORD DwPic=Filetxt.GetLength();
	memset(FileBuff,0,FileLen+1);//初始化位图文件的空间
	if(!FileBuff)//判断位图空间是否申请成功
	{
		return false;
	}
	if(Filetxt.Read(FileBuff,FileLen)!=FileLen)//读取文本信息，存入到FileBuff中去
	{
		return false;
	}

	//unsigned int total_packet = 0;
	unsigned int temp_filepack = 0;
	unsigned int last_packet_data_size = 0;
	if(FileLen % 400 == 0)
	{
		temp_filepack = FileLen / 400;
		last_packet_data_size = 400;
	}
	else
	{
		temp_filepack =  FileLen / 400  + 1;
		last_packet_data_size = FileLen % 400;
	}

	total_packet = temp_filepack + 1; //其中 1是 第一包的头 传 MD5值和 文件总包数.;


	char md5_32byte[33];
	memset(md5_32byte,0,33);
	WideCharToMultiByte( CP_ACP, 0, MD5_value.GetBuffer(), -1, md5_32byte, 255, NULL, NULL );

	char send_buffer[400];
	memset(send_buffer,0 , 400);
	CString temp_cs_show;
	for (int z=0;z<total_packet;z++)
	{
		memset(send_buffer,0,400);
		if(z == 0)
		{
			memcpy(send_buffer,&total_packet,4);
			memcpy(send_buffer+4,md5_32byte,32);
		}
		else if(z < total_packet -1)
		{
			memcpy(send_buffer,FileBuff + z*400,400);
		}
		else
		{
			memcpy(send_buffer,FileBuff + z*400,last_packet_data_size);
		}
		if(WriteBacnetPictureData_Blocking(g_bac_instance,screen_index,z,total_packet,(unsigned char *)send_buffer)< 0)
		{
			MessageBox(_T("Write pocture file into SD disk timeout."));
			return false;

		}
		g_progress_persent = z * 100 /total_packet;

		temp_cs_show.Format(_T("Saving picture file into SD disk .(%u/%u)"),z*400,total_packet*400+last_packet_data_size);
		SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_cs_show);

	}
	g_progress_persent = 100;
	temp_cs_show.Format(_T("Saving picture file into SD disk success."));
	SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_cs_show);
	return true;
#pragma endregion read file buffer

#endif
	//#pragma region write_file_buffer22
	//
	//	HANDLE hFile;
	//	hFile=CreateFile(_T("D:\\Test.ico"),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	//	DWORD dWrites;
	//	WriteFile(hFile,FileBuff,FileLen,&dWrites,NULL);
	//	CloseHandle(hFile);  
	//	if(FileBuff)
	//		delete FileBuff;
	//
	//#pragma endregion write_file_buffer22


}

void BacnetScreen::Reset_Screen_Rect()
{

	CRect temp_mynew_rect;
	::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;

	CRect temp_window;
	GetWindowRect(&temp_window);

	if(window_max)
	{
		CRect temp_mynew_rect;
		::GetWindowRect(BacNet_hwd,&temp_mynew_rect);	//获取 view的窗体大小;
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,temp_mynew_rect.Width(),temp_mynew_rect.Height() - DELTA_HEIGHT, NULL);
	}
	else if((temp_window.Width() <= temp_mynew_rect.Width() ) && (temp_window.Height() <= temp_mynew_rect.Height()))
	{
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left,temp_mynew_rect.top,0,0,SWP_NOSIZE );
	}
	else
		::SetWindowPos(this->m_hWnd,NULL,temp_mynew_rect.left + 90,temp_mynew_rect.top + 70,500,700, NULL);


	return;

}


void BacnetScreen::OnSysCommand(UINT nID, LPARAM lParam)
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


void BacnetScreen::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	
	CRect rc;
	GetClientRect(rc);
	if(m_screen_list.m_hWnd != NULL)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0,0, 0,0,  SWP_NOSIZE | SWP_NOMOVE);
		//m_program_list.MoveWindow(&rc);
		m_screen_list.MoveWindow(rc.left,rc.top,rc.Width(),rc.Height() - 80);

		GetDlgItem(IDC_BUTTON_GRAPHIC_INSERT)->MoveWindow(rc.left + 20 ,rc.bottom - 60 , 120,50);
		GetDlgItem(IDC_WEBVIEW_BUTTON)->MoveWindow(rc.left + 150, rc.bottom - 60, 120, 50);
		GetDlgItem(IDC_BUTTON_CLEAR_SCREEN_DATA)->MoveWindow(rc.left + 280, rc.bottom - 60, 120, 50);
		
	}
}



void BacnetScreen::OnNMClickListScreen(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	*pResult = 0;

	
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
	m_screen_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_screen_list.SubItemHitTest(&lvinfo);
	if(nItem!=-1)
	{ 
		m_screen_list.SetCellChecked(nItem,0,1);
		screen_list_line = nItem;
		for (int i=0;i<m_screen_list.GetItemCount();++i)
		{
			if(i == nItem)
				continue;
			m_screen_list.SetCellChecked(i,0,FALSE);
		}
	}
	int lRow = 0;
	int lCol = 0;
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;



	return;
	if(lRow>m_screen_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的;
		return;
	if(lRow<0)
		return;
	m_screen_list.Set_Edit(true);
	CString New_CString;
	CString temp_task_info;

	if(lCol == SCREEN_PIC_FILE)
	{
		TRACE(_T("click"));
		//CString cs_temp = m_screen_list.GetItemText(lRow,lCol);
		//CFileFind temp_find;
		//bool find_full_name = false;
		//bool find_relatice_path = false;
		//find_full_name = temp_find.FindFile(cs_temp);
		//if(find_full_name)
		//{

		//}
		//else
		//{
		//	CString folder_temp_file;
		//	folder_temp_file = g_strBuildingFolder + _T("\\image\\") + cs_temp;
		//	CFileFind find_relatice_file;
		//	find_relatice_path = find_relatice_file.FindFile(folder_temp_file);
		//	if(find_relatice_path == false)
		//	{
		//		MessageBox(_T("Please input an valid path or double click to select file."));
		//		m_screen_list.SetItemText(lRow,lCol,_T(""));

		//	}
		//}
	}
	else
	{
		m_screen_list.Set_Edit(false);
	}
	*pResult = 0;
}

int check_webview_runtime()
{
	LPCTSTR strRegEntry1 = _T("SOFTWARE\\WOW6432Node\\Microsoft\\EdgeUpdate\\Clients\\{F3017226-FE2A-4295-8BDF-00C3A9A7E4C5}\\");
	int nret_1 = 0;
	int nret_2 = 0;
	LPBYTE userversion_get = new BYTE[100];
	DWORD type_1 = REG_SZ;
	DWORD cbDate_1 = 80;
	HKEY   hKey;
	LONG   lReturnCode = 0;

	lReturnCode = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegEntry1, 0, KEY_READ, &hKey);

	if (lReturnCode != ERROR_SUCCESS)
	{
		nret_1 = 0;
	}
	else
	{
		LONG return1 = ::RegQueryValueEx(hKey, _T("pv"), NULL, &type_1, userversion_get, &cbDate_1);
		if (return1 != ERROR_SUCCESS)
		{
			nret_1 = 0;
		}
		else
			return 1;
	}


	LPCTSTR strRegEntry2 = _T("Software\\Microsoft\\EdgeUpdate\\Clients\\{F3017226-FE2A-4295-8BDF-00C3A9A7E4C5}\\");
	lReturnCode = ::RegOpenKeyEx(HKEY_CURRENT_USER, strRegEntry2, 0, KEY_READ, &hKey);

	if (lReturnCode != ERROR_SUCCESS)
	{
		nret_2 = 0;
	}
	else
	{
		LONG return2 = ::RegQueryValueEx(hKey, _T("pv"), NULL, &type_1, userversion_get, &cbDate_1);
		if (return2 != ERROR_SUCCESS)
		{
			nret_2 = 0;
		}
		else
			return 1;
	}


	if ((nret_1 == 0) && (nret_2 == 0))
		return 0;
	else
		return 1;
}

//系统版本太低 使用旧版本
//检查旧版本的是否有数据，如果有数据就使用旧版本的
//否则直接使用新版
/*
*	OS 		data in device	firmware	    use webview
	old		no				old				no
	new		no				old				no
	old		yes				old				no
	new		yes				old				no
	old		no				new				no
	old		yes				new				no
	new		no				new				yes
	new		yes				new				no

*/
int BacnetScreen::CheckOldGraphic()
{
	int os_pass = 0;
	int firmware_pass = 0;
	int nversion = check_webview_runtime();
	if (nversion == 0)
	{
		os_pass = 0;
		return 0;
	}

	if((Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub < WEBVIEW_JSON_FEATURE))
	{
		firmware_pass = 0;
		return 0;
	}
	int ret_n = 0;
     //判断每个里面有没有元素，没有元素就可以使用;
	for (int i = 0; i < (int)m_screen_data.size(); i++)
	{
		if (m_screen_data.at(i).old_type_element_count != 0)
			return 0;
	}

	return 1;
 

}


void BacnetScreen::OnBnClickedClearScreenData()
{
	if (IDYES != MessageBox(_T("Clicking the yes button will clear all graphic data. Are you sure to clear it?"), _T("Warning"), MB_YESNOCANCEL))
	{
		return;
	}
	int nret = CheckOldGraphic();
	//if (nret == 0)
	{
		for (int i = 0; i < (int)m_screen_data.size(); i++)
		{
			m_screen_data.at(i).old_type_element_count = 0;
			m_screen_data.at(i).webview_element_count = 0;
		}

		int end_temp_instance = 0;
		for (int i = 0; i < BAC_SCREEN_GROUP; i++)
		{
			end_temp_instance = BAC_READ_SCREEN_REMAINDER + (BAC_READ_SCREEN_GROUP_NUMBER)*i;
			if (end_temp_instance >= BAC_SCREEN_COUNT)
				end_temp_instance = BAC_SCREEN_COUNT - 1;
			if (Write_Private_Data_Blocking(WRITESCREEN_T3000, (BAC_READ_SCREEN_GROUP_NUMBER)*i, end_temp_instance) > 0)
			{
				CString Mession_ret;
				Mession_ret.Format(_T("Write screen from %d to %d success."), (BAC_READ_SCREEN_GROUP_NUMBER)*i, end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
			}
		}

		m_graphic_label_data.clear();
		for (int i = 0; i < BAC_GRPHIC_LABEL_COUNT; i++)
		{
			Str_label_point temp_label_point = { 0 };
			memset(&temp_label_point, 0, sizeof(Str_label_point));
			m_graphic_label_data.push_back(temp_label_point);
		}

		for (int i = 0; i < BAC_GRPHIC_LABEL_GROUP - 1; i++)
		{
			int ret_return = 0;
			ret_return = Write_Private_Data_Blocking(WRITE_GRPHIC_LABEL_COMMAND, i * BAC_READ_GRPHIC_LABEL_GROUP_NUMBER, BAC_READ_GRPHIC_LABEL_GROUP_NUMBER * (i + 1) - 1);
			if (ret_return < 0)
				return;
			Sleep(50);
		}
	}
}

DWORD WINAPI  BacnetScreen::CreateWebServerThreadfun(LPVOID lpVoid)
{
	BacnetScreen* pParent = (BacnetScreen*)lpVoid;
	webview_run_server();
	h_create_webview_server_thread = NULL;
	return 0;

}

void BacnetScreen::OnBnClickedWebViewShow()
{
	// This thread will not exit when it is running properly, and will exit if run_server executes abnormally, terminating the thread.
	//CString appDataMyAppPath = GetUserAppDataPath(_T("T3000"));
	//appDataMyAppPath = appDataMyAppPath + _T("\\EBWebView");
	//DeleteDirectory(appDataMyAppPath);
	if (h_create_webview_server_thread == NULL)
	{
		h_create_webview_server_thread = CreateThread(NULL, NULL, CreateWebServerThreadfun, this, NULL, NULL);
	}

	int nret = CheckOldGraphic();
	if (nret == 0)
	{
		PostMessage(WM_HOTKEY, KEY_INSERT, NULL);
		return;
	}


	if (screen_list_line > 7)
	{
		CString temp_cs;
		temp_cs.Format(_T("The index you selected is %d, currently only 1-8 is supported."), screen_list_line + 1);
		MessageBox(temp_cs);
		SetPaneString(BAC_SHOW_MISSION_RESULTS, temp_cs);
		return;
	}
	int nversion = check_webview_runtime();
	if (nversion == 0)
	{

		int Answer;

		Answer = AfxMessageBox(L"To use this feature properly,Please download the latest WebView RumTime.\r\n https://developer.microsoft.com/en-us/microsoft-edge/webview2/", MB_YESNO | MB_ICONWARNING);


		if (Answer == IDYES)
		{
			ShellExecute(this->m_hWnd, _T("open"), _T("https://developer.microsoft.com/en-us/microsoft-edge/webview2/"), NULL, NULL, SW_SHOWNORMAL);
			ShellExecute(this->m_hWnd, _T("open"), _T("https://msedge.sf.dl.delivery.mp.microsoft.com/filestreamingservice/files/161df913-7a16-4b44-aa98-2a0239f227d7/MicrosoftEdgeWebView2RuntimeInstallerX86.exe"), NULL, NULL, SW_SHOWNORMAL);
			return;
		}

	}

	int read_ret = Read_Webview_Data();
	if (read_ret <= 0)
	{
		if (read_ret == -2)
		{
		    SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Read data success! The data length is 0 ."));
		}
		else 
			SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Read data timeout!"));
	}
	else
	{

	}

	Unreg_Hotkey();
	LoadOnlinePanelData();
	Sleep(200);

	const TCHAR szFilter[] = _T("HTML File (*.html)|*.html");

	{

		CString webviewUrl = _T("http://localhost:9103/");
		CString webviewTitle = _T("HVAC Drawer");
		

		auto webviewwindow = new BacnetWebViewAppWindow(IDM_CREATION_MODE_WINDOWED, wstring(webviewUrl), wstring(webviewTitle));
		auto result = BacnetWebViewAppWindow::RunMessagePump();
		try
		{
			delete webviewwindow;
		}
		catch (const std::exception&)
		{

		}
		
	}
}


//从浏览器来的命令调用这个函数 ，需要重新与对应的panel建立连接，并且要判断 设备版本号 ，将数据写入设备中
int Write_Webview_Data_Special(int panelid, UINT nserialnumber, int nscreenindex,  int element_count)
{
	// 使用静态变量存储上次调用的时间和参数
	static std::map<std::tuple<int, UINT, int>, std::chrono::steady_clock::time_point> last_write_call_times;

	// 当前时间
	auto now = std::chrono::steady_clock::now();
	auto current_params = std::make_tuple(panelid, nserialnumber, nscreenindex);

	// 检查是否在十秒内针对同一参数调用
	if (last_write_call_times.find(current_params) != last_write_call_times.end()) {
		auto last_call_time = last_write_call_times[current_params];
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - last_call_time).count();
		if (duration < 10)
		{
			TRACE(_T("在十秒内调用，直接返回\r\n"));
			return -3; // 在十秒内调用，直接返回
		}
	}
	// 更新上次调用时间
	last_write_call_times[current_params] = now;

	int handle_device_instance = 0;
	for (int i = 0; i < g_bacnet_panel_info.size(); i++)
	{
		if ((nserialnumber == g_bacnet_panel_info.at(i).nseiral_number) && (panelid == g_bacnet_panel_info.at(i).panel_number))
		{
			handle_device_instance = g_bacnet_panel_info.at(i).object_instance;
			break;
		}
	}

	if (handle_device_instance == 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Can't find the device instance!"));
		return -1;
	}
	g_screen_data[panelid].at(nscreenindex).webview_element_count = element_count;
	int n_ret = WritePrivateData_Blocking(handle_device_instance, WRITESCREEN_T3000, nscreenindex, nscreenindex, 5, (char*)&g_screen_data.at(panelid).at(nscreenindex));

	if (n_ret > 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write element count success."));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write element count timeout."));
		return -1;
	}

	int nret2 = write_webview_data(handle_device_instance, panelid, nserialnumber, nscreenindex, 1);
	if (nret2 < 0)
	{
		return -2;
	}
	TRACE(_T("write_webview_data success\r\n"));
	//if (GetPrivateDataSaveSPBlocking(handle_device_instance, READ_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), 1) > 0)
	//{
	//	memcpy(&g_Device_Basic_Setting[panelid], &s_Basic_Setting, sizeof(Str_Setting_Info));
	//}
	//else
	//{
	//	SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Read data timeout!"));
	//	return -1;
	//}


}


//从浏览器来的命令调用这个函数 ，需要重新与对应的panel建立连接，并且要判断 设备版本号 ，将数据读取到额外的全局变量中
int Read_Webview_Data_Special(int panelid,UINT nserialnumber,int nscreenindex)
{
	// 使用静态变量存储上次调用的时间和参数
	static std::map<std::tuple<int, UINT, int>, std::chrono::steady_clock::time_point> last_call_times;

	// 当前时间
	auto now = std::chrono::steady_clock::now();
	auto current_params = std::make_tuple(panelid, nserialnumber, nscreenindex);

	// 检查是否在十秒内针对同一参数调用
	if (last_call_times.find(current_params) != last_call_times.end()) {
		auto last_call_time = last_call_times[current_params];
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - last_call_time).count();
		if (duration < 20) 
		{
			TRACE(_T("在十秒内调用，直接返回\r\n"));
			return 1; // 在十秒内调用，直接返回
		}
	}
	// 更新上次调用时间
	last_call_times[current_params] = now;



	//g_Device_Basic_Setting
	int handle_device_instance = 0;
	for (int i = 0; i < g_bacnet_panel_info.size(); i++)
	{
		if ((nserialnumber == g_bacnet_panel_info.at(i).nseiral_number) && (panelid == g_bacnet_panel_info.at(i).panel_number))
		{
			handle_device_instance = g_bacnet_panel_info.at(i).object_instance;
			break;
		}
	}

	if(handle_device_instance == 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Can't find the device instance!"));
		return -1;
	}


	if (GetPrivateDataSaveSPBlocking(handle_device_instance, READ_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info)) > 0)
	{
		memcpy(&g_Device_Basic_Setting[panelid], &s_Basic_Setting, sizeof(Str_Setting_Info));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Read data timeout!"));
		return -1;
	}

	if(g_Device_Basic_Setting[panelid].reg.pro_info.firmware0_rev_main * 10 + g_Device_Basic_Setting[panelid].reg.pro_info.firmware0_rev_sub < WEBVIEW_JSON_FEATURE)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("The device doesn't support this feature . need update firmware!"));
		return -1;
	}


	//
	CString temp_item_out;
	CString  des_file_out;
	temp_item_out.Format(_T("%u_%d_out.zip"), nserialnumber, nscreenindex);
	des_file_out = selected_image_fordor + _T("\\") + temp_item_out;
	char temp_des_file_out[MAX_PATH] = { 0 };
	WideCharToMultiByte(CP_ACP, 0, des_file_out.GetBuffer(), -1, temp_des_file_out, MAX_PATH, NULL, NULL);

	int test_count2 = GetPrivateDataSaveSPBlocking(handle_device_instance, READ_JSON_SCREEN, nscreenindex, nscreenindex, sizeof(Str_t3_screen_Json));
	if (test_count2 < 0)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Read data timeout!"));
		return -1;
	}
	memcpy(&g_json_screen_data[panelid].at(nscreenindex), &s_json_screen_data, sizeof(Str_t3_screen_Json));

	int data_point_length = g_json_screen_data[panelid].at(nscreenindex).file_data.zip_size;
	if (data_point_length == 0)
		return 1;
#if 1
	char* temp_buffer = new char[data_point_length];
	memset(temp_buffer, 0, data_point_length);
	int read_counts = 10;
	int temp_json_item_group = (read_counts + BAC_READ_JSON_ITEM_GROUP_NUMBER - 1) / BAC_READ_JSON_ITEM_GROUP_NUMBER;
	CString Mession_ret;
	int end_temp_instance = 0;
	for (int i = 0; i < temp_json_item_group; i++) //这里暂时是从0 index 开始读取的
	{
		end_temp_instance = BAC_READ_JSON_ITEM_REMAINDER + (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i + nscreenindex * 10;
		if (end_temp_instance >= nscreenindex * 10 + 9)
			end_temp_instance = nscreenindex * 10 + 9;

		int start_index = nscreenindex * 10 + (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i;
		if (GetPrivateDataSaveSPBlocking(handle_device_instance, READ_JSON_ITEM, start_index, end_temp_instance, sizeof(Str_item_Json)) > 0)
		{
			Mession_ret.Format(_T("Read webview data from %d to %d success."), start_index, end_temp_instance);
			SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
			//write_success_count++;
			if (!offline_mode)
				Sleep(SEND_COMMAND_DELAY_TIME);
		}
		else
		{
			Mession_ret.Format(_T("Read webview data from %d to %d timeout."), start_index, end_temp_instance);
			SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
			return -3;
		}

	}

	char* temp_char = temp_buffer;
	int already_copy = 0;
	for (int j = nscreenindex * 10; j < nscreenindex * 10 + 10; j++) //暂时只考虑第0个开始;
	{
		if (data_point_length <= 200)
		{
			memcpy(temp_char, s_json_item_data[j].all, data_point_length);
			already_copy = already_copy + data_point_length;
			break;
		}
		else
		{
			int last_length = data_point_length % 200;
			if (already_copy != (data_point_length - last_length))
			{
				memcpy(temp_char, s_json_item_data[j].all, 200);
				temp_char = temp_char + 200;
				already_copy = already_copy + 200;
			}
			else
			{
				memcpy(temp_char, s_json_item_data[j].all, last_length);
				temp_char = temp_char + last_length;
				already_copy = already_copy + last_length;
				break;
			}
		}

	}

	FILE* fpw;
	fpw = fopen(temp_des_file_out, "wb+");
	fwrite(temp_buffer, sizeof(char), data_point_length, fpw);
	fflush(fpw);
	fclose(fpw);
	free(temp_buffer);
	bool ret_unzip = UnzipItem(des_file_out, selected_image_fordor);
	if (!ret_unzip)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Unzip file failed!"));
		return -4;
	}

#endif
	return 1;
}

int  BacnetScreen::Read_Webview_Data()
{
	if ((Device_Basic_Setting.reg.webview_json_flash == 2) &&//这里要判断是2
		(Device_Basic_Setting.reg.pro_info.firmware0_rev_main * 10 + Device_Basic_Setting.reg.pro_info.firmware0_rev_sub >= WEBVIEW_JSON_FEATURE)) //643 版本会有这个功能
	{
		CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
		CString image_fordor = g_strExePth + CString("Database\\Buildings\\") + pFrame->m_strCurMainBuildingName + _T("\\image");
		CString temp_item_out;
		CString  des_file_out;
		temp_item_out.Format(_T("%u_%d_out.zip"), g_selected_serialnumber, screen_list_line);
		des_file_out = image_fordor + _T("\\") + temp_item_out;
		char temp_des_file_out[MAX_PATH] = { 0 };
		WideCharToMultiByte(CP_ACP, 0, des_file_out.GetBuffer(), -1, temp_des_file_out, MAX_PATH, NULL, NULL);

		int test_count2 = GetPrivateData_Blocking(g_bac_instance, READ_JSON_SCREEN, screen_list_line, screen_list_line, sizeof(Str_t3_screen_Json));
		if (test_count2 < 0)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Read data timeout!"));
			return -1;
		}
		int data_point_length = m_json_screen_data.at(screen_list_line).file_data.zip_size;
		if (data_point_length == 0)
			return -2;

		char* temp_buffer = new char[data_point_length];
		memset(temp_buffer, 0, data_point_length);
		int read_counts = 10;
		int temp_json_item_group = (read_counts + BAC_READ_JSON_ITEM_GROUP_NUMBER - 1) / BAC_READ_JSON_ITEM_GROUP_NUMBER;
		CString Mession_ret;
		int end_temp_instance = 0;
		for (int i = 0; i < temp_json_item_group; i++) //这里暂时是从0 index 开始读取的
		{
			end_temp_instance = BAC_READ_JSON_ITEM_REMAINDER + (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i + screen_list_line * 10;
			if (end_temp_instance >= screen_list_line * 10 + 9)
				end_temp_instance = screen_list_line * 10 + 9;

			int start_index = screen_list_line * 10 + (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i;
			if (GetPrivateData_Blocking(g_bac_instance, READ_JSON_ITEM, start_index, end_temp_instance, sizeof(Str_item_Json)) > 0)
			{
				Mession_ret.Format(_T("Read webview data from %d to %d success."), start_index, end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
				//write_success_count++;
				if (!offline_mode)
					Sleep(SEND_COMMAND_DELAY_TIME);
			}
			else
			{
				Mession_ret.Format(_T("Read webview data from %d to %d timeout."), start_index, end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
				return -3;
			}

		}

		char* temp_char = temp_buffer;
		int already_copy = 0;
		for (int j = screen_list_line * 10; j < screen_list_line * 10 + 10; j++) //暂时只考虑第0个开始;
		{
			if (data_point_length <= 200)
			{
				memcpy(temp_char, m_json_item_data.at(j).all, data_point_length);
				already_copy = already_copy + data_point_length;
				break;
			}
			else
			{
				int last_length = data_point_length % 200;
				if (already_copy != (data_point_length - last_length))
				{
					memcpy(temp_char, m_json_item_data.at(j).all, 200);
					temp_char = temp_char + 200;
					already_copy = already_copy + 200;
				}
				else
				{
					memcpy(temp_char, m_json_item_data.at(j).all, last_length);
					temp_char = temp_char + last_length;
					already_copy = already_copy + last_length;
					break;
				}
			}

		}

#if 0
		int read_counts = data_point_length / 200 + 1; //暂时只考虑第0个开始;
		int temp_json_item_group = (read_counts + BAC_READ_JSON_ITEM_GROUP_NUMBER - 1) / BAC_READ_JSON_ITEM_GROUP_NUMBER;
		CString Mession_ret;
		int end_temp_instance = 0;
		for (int i = 0; i < temp_json_item_group; i++) //这里暂时是从0 index 开始读取的
		{
			end_temp_instance = BAC_READ_JSON_ITEM_REMAINDER + (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i;
			if (end_temp_instance >= read_counts)
				end_temp_instance = read_counts;
			if (GetPrivateData_Blocking(g_bac_instance, READ_JSON_ITEM, (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i, end_temp_instance, sizeof(Str_item_Json)) > 0)
			{
				Mession_ret.Format(_T("Read webview data from %d to %d success."), (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i, end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
				//write_success_count++;
				if (!offline_mode)
					Sleep(SEND_COMMAND_DELAY_TIME);
			}
			else
			{
				Mession_ret.Format(_T("Read webview data from %d to %d timeout."), (BAC_READ_JSON_ITEM_GROUP_NUMBER)*i, end_temp_instance);
				SetPaneString(BAC_SHOW_MISSION_RESULTS, Mession_ret);
				return -2;
			}
		}

		char* temp_char = temp_buffer;
		for (int j = 0; j < read_counts; j++) //暂时只考虑第0个开始;
		{
			if (data_point_length <= 200)
			{
				memcpy(temp_char, m_json_item_data.at(j).all, data_point_length);
				break;
			}
			else
			{
				int last_length = data_point_length % 200;
				if (j != read_counts - 1)
				{
					memcpy(temp_char, m_json_item_data.at(j).all, 200);
					temp_char = temp_char + 200;
				}
				else
				{
					memcpy(temp_char, m_json_item_data.at(j).all, last_length);
					temp_char = temp_char + last_length;
					break;
				}
			}

		}
#endif


		FILE* fpw;
		fpw = fopen(temp_des_file_out, "wb+");
		fwrite(temp_buffer, sizeof(char), data_point_length, fpw);
		fflush(fpw);
		fclose(fpw);
		free(temp_buffer);
		bool ret_unzip = UnzipItem(des_file_out, image_fordor);
		if (!ret_unzip)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Unzip file failed!"));
		}
	}
	return 1;
}

