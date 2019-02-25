// BacnetScreenEdit.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetScreenEdit.h"
#include "afxdialogex.h"
#include "T3000.h"
#include "MainFrm.h"

#include "LabelEditDlg.h"
#include "BacnetAddLabel.h"
#include "BacnetEditLabel.h"
#include "global_define.h"
extern int pointtotext(char *buf,Point_Net *point);
Str_label_point m_temp_graphic_label_data[BAC_GRPHIC_LABEL_COUNT];

static int m_bac_select_label_old = -1;
static bool click_ret_old = false;
CBacnetEditLabel * Edit_Label_Window = NULL;
CBacnetAddLabel * Add_Label_Window = NULL;
extern CBacnetScreenEdit * ScreenEdit_Window ;
int GetInputLabel(int index, CString &ret_label, Point_Net * npoint = NULL );
int GetInputFullLabel(int index, CString &ret_full_label, Point_Net * npoint = NULL);
//int GetInputValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M);
int GetInputValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M,int &digital_value);

int GetOutputLabel(int index, CString &ret_label, Point_Net * npoint = NULL);
int GetOutputFullLabel(int index, CString &ret_full_label, Point_Net * npoint = NULL);
//int GetOutputValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M);
int GetOutputValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M,int &digital_value);

int GetVariableLabel(int index,CString &ret_label, Point_Net * npoint = NULL);
int GetVariableFullLabel(int index,CString &ret_full_label, Point_Net * npoint = NULL);
//int GetVariableValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M);
int GetVariableValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M,int &digital_value);

int GetPidValue(int index,CString &Auto_M,CString &persend_data);

int GetPrgFullLabel(int index,CString &ret_full_label);
int GetPrgLabel(int index,CString &ret_label);
int GetPrgValue(int index, CString &ret_cstring);

int GetScreenLabel(int index,CString &ret_label);
int GetScreenFullLabel(int index,CString &ret_full_label);

int GetHolidayLabel(int index,CString &ret_label);
int GetHolidayFullLabel(int index,CString &ret_full_label);
int GetHolidayValue(int index, CString &Auto_M, CString &persend_data);

int GetScheduleLabel(int index,CString &ret_label);
int GetScheduleFullLabel(int index,CString &ret_full_label);
int GetScheduleValue(int index, CString &Auto_M, CString &persend_data);
int GetAmonLabel(int index,CString &ret_label);


unsigned int control_object_instance = 0;

// CBacnetScreenEdit dialog
// CGraphicView
#define XStart 0
//#define YStart 30
#define YStart 0
CRect mynew_rect;	//用来存储 窗体应该有多大;

vector <int> screnn_sequence;
HANDLE h_read_all_panel_des_thread = NULL;
HANDLE h_refresh_group_thread = NULL;

bool show_not_exsit_dlg = true;
bool screen_lock_label = false;
int screen_show_nopic = 1;
//extern int Station_NUM;
extern char *ispoint(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network, byte panel, int *netpresent);
extern char *ispoint_ex(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network,unsigned char & sub_panel, byte panel , int *netpresent);





typedef union
{
    Str_in_point  m_group_input_data;
    Str_out_point m_group_output_data;
    Str_variable_point m_group_variable_data;
    Str_program_point m_group_program_data;
    Str_weekly_routine_point m_group_schedual_data;
    Str_annual_routine_point m_group_annual_data;
    Str_controller_point  m_group_pid_data;
    Str_monitor_point     m_monitor_data;
}groupdata;

//用于刷新 group的结构;
struct _Group_Data
{
    _Graphic_Value_Info read_info;
    Point_Net           point;            /* 5 bytes*/
    groupdata           detail_data;
}Group_Data;

vector <_Group_Data> m_read_group_data; //用于刷新group screen 的结构;



IMPLEMENT_DYNAMIC(CBacnetScreenEdit, CDialogEx)

CBacnetScreenEdit::CBacnetScreenEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetScreenEdit::IDD, pParent)
{

}

CBacnetScreenEdit::~CBacnetScreenEdit()
{
}

void CBacnetScreenEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetScreenEdit, CDialogEx)
	ON_MESSAGE(WM_HOTKEY,&CBacnetScreenEdit::OnHotKey)//快捷键消息映射手动加入
	ON_BN_CLICKED(IDC_BUTTON_SCREEN_EDIT_TEST, &CBacnetScreenEdit::OnBnClickedButtonScreenEditTest)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()

	ON_WM_TIMER()
	ON_MESSAGE(DELETE_LABEL_MESSAGE,Delete_Label_Handle)
	ON_MESSAGE(ADD_LABEL_MESSAGE,Add_label_Handle)
	ON_MESSAGE(EDIT_LABEL_MESSAGE,Edit_label_Handle)
	ON_MESSAGE(MY_REDRAW_WINDOW, RedrawScreeneditWindow)
	ON_BN_CLICKED(IDC_BUTTON_SCREEN_EXIT, &CBacnetScreenEdit::OnBnClickedButtonScreenExit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CBacnetScreenEdit::OnBnClickedButtonDelete)

//	ON_WM_RBUTTONDBLCLK()
	ON_WM_LBUTTONDBLCLK()
    ON_WM_CLOSE()
END_MESSAGE_MAP()

#define  REFRESH_LABEL_DATA 2
LRESULT  CBacnetScreenEdit::Edit_label_Handle(WPARAM wParam, LPARAM lParam)
{
	int n_command = lParam;
	if(n_command == REFRESH_LABEL_DATA)
	{
		Invalidate(1);
		return true;
	}
	Bacnet_Label_Info * temp_info =  (Bacnet_Label_Info *)wParam;

	m_graphic_label_data.at(temp_info->nLabel_index).reg.nDisplay_Type = temp_info->nDisplay_Type;
	m_graphic_label_data.at(temp_info->nLabel_index).reg.nclrTxt = temp_info->nclrTxt;
	memcpy(&m_graphic_label_data.at(temp_info->nLabel_index).reg.icon_name_1,temp_info->ico_name,20);
	memcpy(&m_graphic_label_data.at(temp_info->nLabel_index).reg.icon_name_2,temp_info->ico_name_2,20);
	m_graphic_label_data.at(temp_info->nLabel_index).reg.nIcon_place = temp_info->ntext_place;
	m_graphic_label_data.at(temp_info->nLabel_index).reg.nIcon_size = temp_info->n_iconsize;

	if(temp_info!=NULL)
	{
		delete temp_info;
		temp_info = NULL;
	}
	if(UpdateDeviceLabelFlash())
	{
		memcpy_s(&m_temp_graphic_label_data,sizeof(Str_label_point) * BAC_GRPHIC_LABEL_COUNT,&m_graphic_label_data.at(0),sizeof(Str_label_point) * BAC_GRPHIC_LABEL_COUNT);
	}

	ReloadLabelsFromDB();
	if(ScreenEdit_Window->IsWindowVisible())
		ScreenEdit_Window->SetFocus();
	Invalidate(1);
	return 0;
}

LRESULT  CBacnetScreenEdit::Delete_Label_Handle(WPARAM wParam, LPARAM lParam)
{
	Bacnet_Label_Info * temp_info =  (Bacnet_Label_Info *)wParam;

	memset(&m_graphic_label_data.at(temp_info->nLabel_index),0,sizeof(Str_label_point));
	m_graphic_label_data.at(temp_info->nLabel_index).reg.label_status = EMPTY_LABEL;

	if(temp_info!=NULL)
	{
		delete temp_info;
		temp_info = NULL;
	}
	ReloadLabelsFromDB();
	if(UpdateDeviceLabelFlash())
	{
		memcpy_s(&m_temp_graphic_label_data,sizeof(Str_label_point) * BAC_GRPHIC_LABEL_COUNT,&m_graphic_label_data.at(0),sizeof(Str_label_point) * BAC_GRPHIC_LABEL_COUNT);
	}
	Invalidate(1);
	return 0;
}


LRESULT  CBacnetScreenEdit::Add_label_Handle(WPARAM wParam, LPARAM lParam)
{
	char * temp_point = (char *)wParam;
	POINT insert_point;
	memcpy(&insert_point,(void *)lParam,sizeof(POINT));

	if((strlen(temp_point) == 0) || ((strlen(temp_point) > 16)))
	{
		if(temp_point!=NULL)
		{
		  delete [] temp_point;
		  temp_point = NULL;
		}
		return 0;
	}

	int temp_number=-1;
	byte temp_value_type = -1;
	byte temp_point_type=-1;
	int temp_panel = -1;
	int temp_net = -1;
	int k=0;
	unsigned char sub_panel = -1;
	char * tempcs=NULL;
	//int temp1;
	//tempcs = ispoint(temp_point,&temp_number,&temp_value_type,&temp_point_type,&temp_panel,&temp_net,0,Station_NUM,&k);
	tempcs = ispoint_ex(temp_point,&temp_number,&temp_value_type,&temp_point_type,&temp_panel,&temp_net,0,sub_panel,Station_NUM,&k);
	if(tempcs == NULL)
	{
		MessageBox(_T("Invalid Label , Please input a label such as 'IN8' , 'Mainpanel-Subpanel-OUT9', 'Mainpanel:Subpanel:OUT9' ,'VAR100'.\r\nFor more information , please reference the user manual."));
		return 0;
	}


	if(temp_number != 0)	//Vector 里面是 0开始 , 这里如果是INPUT1  那值为1  直接减一 存起来 用;
		temp_number = temp_number - 1;

    char temp_1 = temp_point_type & 0x1F;
		if((temp_1 == 23) || (temp_1 == 24) || (temp_1 == 25) || (temp_1 == 26))
		{
			temp_number = temp_number + 1;
		}

	::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;

	int nLeft,nTop;
	//nLeft = (insert_point.x * 1000)/mynew_rect.Width();	//保存的是相对X和Y
	//nTop = (insert_point.y * 1000)/(mynew_rect.Height());
	if(m_full_screen_mode)
	{
		nLeft = (insert_point.x * 1000)/m_cxScreen;	//保存的是相对X和Y
		nTop = (insert_point.y * 1000)/m_cyScreen;
	}
	else
	{
		nLeft = (insert_point.x * 1000)/m_paint_right_limit;	//保存的是相对X和Y
		nTop = (insert_point.y * 1000)/m_paint_botton_limit;
	}


    //2018 01 10  自己的panel 如果是2  允许出现  1.1.IN1  这样的 点. 
	//if(temp_panel != Station_NUM)	//目前不支持读其他Minipanel的东西;
	//	return 0;
	/*if((sub_panel ==0) && (sub_panel >254))
		return 0;*/
	if(sub_panel >254)
		return 0;
	temp_point_type = temp_point_type + 1;

    if(temp_panel == Station_NUM)
	    AddLabel(temp_point_type ,temp_number,Station_NUM,sub_panel,nLeft,nTop);
    else
        AddLabel(temp_point_type, temp_number, temp_panel, sub_panel, nLeft, nTop);
	if(UpdateDeviceLabelFlash())
	{
		memcpy_s(&m_temp_graphic_label_data,sizeof(Str_label_point) * BAC_GRPHIC_LABEL_COUNT,&m_graphic_label_data.at(0),sizeof(Str_label_point) * BAC_GRPHIC_LABEL_COUNT);
	}
	ReloadLabelsFromDB();
	return 0;
}


void CBacnetScreenEdit::AddLabel(unsigned char point_type,uint8_t point_number,uint8_t main_panel,uint8_t sub_panel,unsigned int point_x,unsigned int point_y)
{
	nDefaultDisplayType = GetPrivateProfileInt(_T("Setting"),_T("AddLabelDefaultDisplay"),LABEL_SHOW_VALUE,g_cstring_ini_path);
	nDefaultclrTxt = GetPrivateProfileInt(_T("Setting"),_T("AddLabelDefaultColor"),RGB(0,0,255),g_cstring_ini_path);
	nDefaultTextPlace = GetPrivateProfileInt(_T("Setting"),_T("AddLabelDefaultTextPlace"),LABEL_TEXT_BOTTOM,g_cstring_ini_path);
	nDefaultIconSize = GetPrivateProfileInt(_T("Setting"),_T("AddLabelDefaultIconSize"),LABEL_ICON_NORMAL,g_cstring_ini_path);

	//WritePrivateProfileStringW(_T("Setting"),_T("AddLabelDefaultDisplay"),temp_cs,g_cstring_ini_path);
	//WritePrivateProfileStringW(_T("Setting"),_T("AddLabelDefaultColor"),temp_cs,g_cstring_ini_path);
	//WritePrivateProfileStringW(_T("Setting"),_T("AddLabelDefaultTextPlace"),temp_cs,g_cstring_ini_path);

		unsigned short label_index = 0;
		unsigned int item_insert = 0;
		for (int i=0;i<m_graphic_label_data.size();i++)
		{
			if((m_graphic_label_data.at(i).reg.label_status == NO_UNSED_LABEL) || 
				((m_graphic_label_data.at(i).reg.label_status == EMPTY_LABEL)))
			{
				item_insert = i;
				break;
			}

			//if(label_index < m_graphic_label_data.at(i).reg.nLabel_index )
			//{
			//	label_index = m_graphic_label_data.at(i).reg.nLabel_index + 1;
			//}
		}

		memset(&m_graphic_label_data.at(item_insert),0,sizeof(Str_label_point));
		m_graphic_label_data.at(item_insert).reg.label_status = ENABLE_LABEL;
		m_graphic_label_data.at(item_insert).reg.nclrTxt = nDefaultclrTxt;
		m_graphic_label_data.at(item_insert).reg.nDisplay_Type = nDefaultDisplayType;
		m_graphic_label_data.at(item_insert).reg.nIcon_place = nDefaultTextPlace;
		m_graphic_label_data.at(item_insert).reg.nIcon_size = nDefaultIconSize;

		m_graphic_label_data.at(item_insert).reg.nLabel_index = item_insert;
		m_graphic_label_data.at(item_insert).reg.nMain_Panel  = main_panel;
		m_graphic_label_data.at(item_insert).reg.nPoint_number  = point_number;
		m_graphic_label_data.at(item_insert).reg.nPoint_type = point_type;
		m_graphic_label_data.at(item_insert).reg.nPoint_x = point_x;
		m_graphic_label_data.at(item_insert).reg.nPoint_y = point_y;
		m_graphic_label_data.at(item_insert).reg.nScreen_index = screen_list_line;
		m_graphic_label_data.at(item_insert).reg.nSerialNum = m_nSerialNumber;
		m_graphic_label_data.at(item_insert).reg.nSub_Panel = sub_panel;

	//ReloadLabelsFromDB();
	Invalidate(1);
}



// CBacnetScreenEdit message handlers
LRESULT  CBacnetScreenEdit::RedrawScreeneditWindow(WPARAM wParam, LPARAM lParam)
{
	static CRect temp_rect;	
	::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;
	if(temp_rect!= mynew_rect)
	{
		MoveWindow(mynew_rect.left,mynew_rect.top,mynew_rect.Width(),mynew_rect.Height(),1);
		InitGraphic(g_serialNum,g_bac_instance,screen_list_line);
		temp_rect = mynew_rect;
		//if(((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_SQLITE_INFO)))
		//{
		//	DFTrace(_T("Test log ."));
		//}

	}


	//ReloadLabelsFromDB();
	//Invalidate();
	return 0;
}

void CBacnetScreenEdit::OnBnClickedButtonScreenEditTest()
{
	
}

#include "BacnetMonitor.h"
#include "BacnetProgram.h"
#include "BacnetWeeklyRoutine.h"
#include "BacnetAnnualRoutine.h"
extern CBacnetMonitor *Monitor_Window ;
extern CBacnetProgram *Program_Window ;
extern BacnetWeeklyRoutine *WeeklyRoutine_Window ;
extern BacnetAnnualRoutine *AnnualRoutine_Window ;
BOOL CBacnetScreenEdit::PreTranslateMessage(MSG* pMsg)
{
	


	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN) && (m_bac_select_label>=0))
	{
		if(m_bac_label_vector.at(m_bac_select_label).nPoint_type == BAC_GRP)//10 就是GRP
		{
			screen_list_line = m_bac_label_vector.at(m_bac_select_label).nPoint_number;
			if(screen_list_line<= BAC_SCREEN_COUNT - 1)
			{
				screnn_sequence.push_back(screen_list_line);
				m_bac_select_label = -1;
				InitGraphic(g_serialNum,g_bac_instance,screen_list_line);
			}
			return 0;
		}
		else if(m_bac_label_vector.at(m_bac_select_label).nPoint_type == BAC_AMON)
		{
			monitor_list_line = m_bac_label_vector.at(m_bac_select_label).nPoint_number;
			if(monitor_list_line < BAC_MONITOR_COUNT)
			{
				Monitor_Window->OnBnClickedBtnMonitorGraphic();
			}
			return 0;
		}
		else if(m_bac_label_vector.at(m_bac_select_label).nPoint_type == BAC_PRG)
		{
			program_list_line = m_bac_label_vector.at(m_bac_select_label).nPoint_number;
			if(program_list_line < BAC_PROGRAM_ITEM_COUNT)
			{
				Program_Window->OnBnClickedButtonProgramEdit();
			}
			return 0;
		}
		else if(m_bac_label_vector.at(m_bac_select_label).nPoint_type == BAC_SCH)
		{
			weekly_list_line = m_bac_label_vector.at(m_bac_select_label).nPoint_number;
			if(weekly_list_line < BAC_HOLIDAY_COUNT)
			{
				WeeklyRoutine_Window->OnBnClickedButtonWeeklyScheduleEdit();
			}
			return 0;
		}
		else if(m_bac_label_vector.at(m_bac_select_label).nPoint_type == BAC_HOL)
		{
			annual_list_line = m_bac_label_vector.at(m_bac_select_label).nPoint_number;
			if(annual_list_line < BAC_HOLIDAY_COUNT)
			{
				AnnualRoutine_Window->OnBnClickedButtonAnnualEdit();
			}
			return 0;
		}
	}

	if(pMsg->message==WM_KEYDOWN && ((pMsg->wParam==VK_ESCAPE) || (pMsg->wParam==VK_RETURN)))
	{
		if(m_bac_select_label >= 0)
		{
			m_bac_label_vector.at(m_bac_select_label).nMouse_Status = LABEL_MOUSE_NORMAL;
			SaveBacLabel(m_bac_select_label);
			//Invalidate(false);
			m_bac_select_label = -1;
			PostMessage(MY_REDRAW_WINDOW,NULL,NULL);
			
			return 0;
		}
		else if(m_full_screen_mode)
		{
			m_full_screen_mode = false;
			::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;
			MoveWindow(mynew_rect.left,mynew_rect.top,mynew_rect.Width(),mynew_rect.Height(),1);
			InitGraphic(g_serialNum,g_bac_instance,screen_list_line);
			return 0;
		}
	}

	if(pMsg->message == WM_KEYDOWN)
	{
		CPoint mypoint;
		GetCursorPos(&mypoint);
		if(pMsg->wParam == VK_NEXT)
		{
			::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;

			MoveWindow(mynew_rect.left,mynew_rect.top,mynew_rect.Width(),mynew_rect.Height(),1);
			//MoveWindow(0,0,m_cxScreen,m_cyScreen,1);
			if(screen_list_line<= BAC_SCREEN_COUNT - 1)
			{
				screen_list_line ++;
				screnn_sequence.push_back(screen_list_line);
				m_bac_select_label = -1;
				InitGraphic(g_serialNum,g_bac_instance,screen_list_line);
			}

		}
		else if(pMsg->wParam == VK_PRIOR)
		{
			::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;

			MoveWindow(mynew_rect.left,mynew_rect.top,mynew_rect.Width(),mynew_rect.Height(),1);
			//MoveWindow(0,0,m_cxScreen,m_cyScreen,1);
			if((screen_list_line>0) && (screen_list_line < BAC_SCREEN_COUNT))
			{
				screen_list_line --;
				screnn_sequence.push_back(screen_list_line);
				m_bac_select_label = -1;
				InitGraphic(g_serialNum,g_bac_instance,screen_list_line);
			}

		}
		else if(pMsg->wParam == VK_UP)
		{
			if(mypoint.y>1)
				mypoint.y=mypoint.y-1;
			else
				mypoint.y = 0;
			SetCursorPos(mypoint.x,mypoint.y);
			ScreenToClient(&mypoint);
		}
		else if(pMsg->wParam == VK_DOWN)
		{
			if(mypoint.y<m_cyScreen - 1)
				mypoint.y = mypoint.y + 1;
			else
				mypoint.y = m_cyScreen;
			SetCursorPos(mypoint.x,mypoint.y);
			ScreenToClient(&mypoint);
		}
		else if(pMsg->wParam == VK_LEFT)
		{
			if(mypoint.x>1)
				mypoint.x = mypoint.x - 1;
			else
				mypoint.x = 0;
			SetCursorPos(mypoint.x,mypoint.y);
			ScreenToClient(&mypoint);
		}
		else if(pMsg->wParam == VK_RIGHT)
		{
			if(mypoint.x<m_cxScreen)
				mypoint.x = mypoint.x + 1;
			else
				mypoint = m_cxScreen;
			SetCursorPos(mypoint.x,mypoint.y);
			ScreenToClient(&mypoint);
		}
	}

	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_DELETE) 
	{
		if(m_bac_select_label >= 0)
		{
			Bacnet_Label_Info * temp_info = new Bacnet_Label_Info;
			memcpy(temp_info,&m_bac_label_vector.at(m_bac_select_label),sizeof(Bacnet_Label_Info));
			PostMessage(DELETE_LABEL_MESSAGE,(WPARAM)temp_info,NULL);
			m_bac_select_label = -1;
		}
	}

	if(pMsg->message==WM_KEYDOWN)
	{
		CPoint mypoint;
		if(pMsg->wParam == VK_UP)
		{

			if((m_bac_select_label >=0) && (m_bac_hotkey_lb_down) && (screen_lock_label == false))
			{
				GetCursorPos(&mypoint);
				if(mypoint.y>0)
					mypoint.y--;
				SetCursorPos(mypoint.x,mypoint.y);
				ScreenToClient(&mypoint);
				if(m_bac_label_vector.at(m_bac_select_label).nPoint_y > 0)
				{
					m_bac_label_vector.at(m_bac_select_label).nPoint_y --;
				}
				Invalidate(0);
			}
		}
		else if(pMsg->wParam == VK_DOWN)
		{

			if((m_bac_select_label >=0) && (m_bac_hotkey_lb_down) && (screen_lock_label == false))
			{
				GetCursorPos(&mypoint);
				if(mypoint.y<m_cyScreen)
					mypoint.y++;
				SetCursorPos(mypoint.x,mypoint.y);
				ScreenToClient(&mypoint);

				if(m_bac_label_vector.at(m_bac_select_label).nPoint_y < m_cyScreen)
				{
					m_bac_label_vector.at(m_bac_select_label).nPoint_y ++;
				}
				Invalidate(0);
			}
		}
		else if(pMsg->wParam == VK_LEFT)
		{

			if((m_bac_select_label >=0) && (m_bac_hotkey_lb_down) && (screen_lock_label == false))
			{
				GetCursorPos(&mypoint);
				if(mypoint.x>0)
					mypoint.x--;
				SetCursorPos(mypoint.x,mypoint.y);
				ScreenToClient(&mypoint);
				if(m_bac_label_vector.at(m_bac_select_label).nPoint_x > 0)
				{
					m_bac_label_vector.at(m_bac_select_label).nPoint_x --;
				}
				Invalidate(0);
			}
		}
		else if(pMsg->wParam == VK_RIGHT)
		{

			if((m_bac_select_label >=0) && (m_bac_hotkey_lb_down) && (screen_lock_label == false))
			{
				GetCursorPos(&mypoint);
				if(mypoint.x<m_cxScreen)
					mypoint.x++;
				SetCursorPos(mypoint.x,mypoint.y);
				ScreenToClient(&mypoint);
				if(m_bac_label_vector.at(m_bac_select_label).nPoint_x < m_cxScreen)
				{
					m_bac_label_vector.at(m_bac_select_label).nPoint_x ++;
				}
				Invalidate(0);
			}

		}

	}


	return CDialogEx::PreTranslateMessage(pMsg);
}




LRESULT CBacnetScreenEdit::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	UINT fuModifiers = (UINT) LOWORD(lParam);  // key-modifier flags    
	UINT uVirtKey = (UINT) HIWORD(lParam);     // virtual-key code    

	CPoint mypoint;
	GetCursorPos(&mypoint);
	if( MOD_SHIFT == fuModifiers && VK_UP == uVirtKey )  //Screen
	{  
		if(mypoint.y>10)
			mypoint.y=mypoint.y-10;
		else
			mypoint.y = 0;
		SetCursorPos(mypoint.x,mypoint.y);
		ScreenToClient(&mypoint);
		if((m_bac_select_label >=0) && (m_bac_hotkey_lb_down))
		{
			m_bac_label_vector.at(m_bac_select_label).nPoint_x = mypoint.x;
			m_bac_label_vector.at(m_bac_select_label).nPoint_y = mypoint.y;
			Invalidate(0);
		}
	}  
	else if(MOD_SHIFT == fuModifiers && VK_DOWN == uVirtKey)//Program
	{
		if(mypoint.y<m_cyScreen - 10)
			mypoint.y = mypoint.y + 10;
		else
			mypoint.y = m_cyScreen;
		SetCursorPos(mypoint.x,mypoint.y);
		ScreenToClient(&mypoint);
		if((m_bac_select_label >=0) && (m_bac_hotkey_lb_down))
		{
			m_bac_label_vector.at(m_bac_select_label).nPoint_x = mypoint.x;
			m_bac_label_vector.at(m_bac_select_label).nPoint_y = mypoint.y;
			Invalidate(0);
		}

	}
	else if(MOD_SHIFT == fuModifiers && VK_LEFT == uVirtKey)//Input
	{
		if(mypoint.x>10)
			mypoint.x = mypoint.x - 10;
		else
			mypoint.x = 0;
		SetCursorPos(mypoint.x,mypoint.y);
		ScreenToClient(&mypoint);
		if((m_bac_select_label >=0) && (m_bac_hotkey_lb_down))
		{
			m_bac_label_vector.at(m_bac_select_label).nPoint_x = mypoint.x;
			m_bac_label_vector.at(m_bac_select_label).nPoint_y = mypoint.y;
			Invalidate(0);
		}
	}
	else if(MOD_SHIFT == fuModifiers && VK_RIGHT == uVirtKey)//Output
	{
		if(mypoint.x<m_cxScreen)
			mypoint.x = mypoint.x + 10;
		else
			mypoint = m_cxScreen;
		SetCursorPos(mypoint.x,mypoint.y);
		ScreenToClient(&mypoint);
		if((m_bac_select_label >=0) && (m_bac_hotkey_lb_down))
		{
			m_bac_label_vector.at(m_bac_select_label).nPoint_x = mypoint.x;
			m_bac_label_vector.at(m_bac_select_label).nPoint_y = mypoint.y;
			Invalidate(0);
		}

	}
	else if(MOD_SHIFT == fuModifiers && 'M' == uVirtKey)
	{
		ScreenToClient(&mypoint);
		m_bac_select_label = JudgeClickItem(mypoint);
		if(m_bac_select_label >=0)
			m_bac_hotkey_lb_down = true;
		else
			m_bac_hotkey_lb_down = false;

		return 0;

	}


	if (wParam==KEY_INSERT)	//If the cursor not in the range of the label or other objects,it will means add label
	{
		if(m_bac_select_label<0)
		{
			if(m_full_screen_mode)
			{
				Bacnet_Add_Label();
			}
			else
			{	
				ScreenToClient(&mypoint);
				if((mypoint.x > m_paint_right_limit) || (mypoint.y > m_paint_botton_limit))
				{
					SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Insert point out of range!")); 
					return 0;			
				}
				Bacnet_Add_Label();
			}		
		}
		else
			Bacnet_Edit_Label();

	}
	return 0;
}
//杜帆：很奇葩的事情是  IDI_ICON_DEFAULT_GRAPHIC  利用LoadIcon 加载出来显示的 会很清楚  其他的 用LoadImage 比较清楚;
BOOL CBacnetScreenEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	screen_lock_label  =(bool)GetPrivateProfileInt(_T("Setting"),_T("LockScreenLabel"),0,g_cstring_ini_path);

	//show no pic 如果是0的话就不在显示没有图片.
	screen_show_nopic  =  0;// (bool)GetPrivateProfileInt(_T("Setting"),_T("ScreenLabelShowNoPic"),1,g_cstring_ini_path);
	

	m_full_screen_mode = false;
	m_enable_send_remote_point = false;
	m_building_image_folder.Empty();
	CString ApplicationFolder;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	//image_fordor = ApplicationFolder + _T("\\Database\\image");


	CString temp_now_building_name= g_strCurBuildingDatabasefilePath;
	PathRemoveFileSpec(temp_now_building_name.GetBuffer(MAX_PATH));
	temp_now_building_name.ReleaseBuffer();
	m_building_image_folder = temp_now_building_name  + _T("\\image");


	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);

	//m_building_image_folder = ApplicationFolder + _T("\\Database\\Buildings\\") + pFrame->m_strCurMainBuildingName + _T("\\image");


 
//	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
	m_bac_select_label = -1;
	m_bac_lbuttondown = false;
	//暂时加入，到时候要删掉的，不在这加，debug;
	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//Insert键
	
	
	m_cxScreen=GetSystemMetrics(SM_CXSCREEN);
	m_cyScreen=GetSystemMetrics(SM_CYSCREEN);
	
	::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;
	
	MoveWindow(mynew_rect.left,mynew_rect.top,mynew_rect.Width(),mynew_rect.Height(),1);
	//MoveWindow(0,0,m_cxScreen,m_cyScreen,1);
	screnn_sequence.clear();
	screnn_sequence.push_back(screen_list_line);
	InitGraphic(g_serialNum,g_bac_instance,screen_list_line);

	SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_CROSS));//IDC_ARROW
	//SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_HAND));//IDC_ARROW
	
	//default_icon =  AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_ICON);
	default_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_DEFAULT_ICON),IMAGE_ICON,0,0,LR_LOADTRANSPARENT); 

	//default_on_icon = AfxGetApp()->LoadIcon(IDI_ICON_SWITCH_ON);
	default_on_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_SWITCH_ON),IMAGE_ICON,0,0,LR_LOADTRANSPARENT); 


	//default_off_icon = AfxGetApp()->LoadIcon(IDI_ICON_SWITCH_OFF);
	default_off_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_SWITCH_OFF),IMAGE_ICON,0,0,LR_LOADTRANSPARENT); 

	//default_input_icon = AfxGetApp()->LoadIcon(IDI_ICON_INPUT_DEFAULT);	
	default_input_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_INPUT_DEFAULT),IMAGE_ICON,0,0,LR_LOADTRANSPARENT); 

	//default_output_icon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_OUTPUT);	
	default_output_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_DEFAULT_OUTPUT),IMAGE_ICON,0,0,LR_LOADTRANSPARENT); 


	//default_variable_icon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_VARIABLE);
	default_variable_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_DEFAULT_VARIABLE),IMAGE_ICON,0,0,LR_LOADTRANSPARENT); 
//	default_program_icon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_PROGRAM);	


	default_program_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_DEFAULT_PROGRAM),IMAGE_ICON,0,0, LR_LOADTRANSPARENT);  



	//default_pid_icon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_PID);	
	default_pid_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_DEFAULT_PID),IMAGE_ICON,0,0, LR_LOADTRANSPARENT);  

	//default_holiday_icon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_HOLIDAY);	
	default_holiday_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_DEFAULT_HOLIDAY),IMAGE_ICON,0,0, LR_LOADTRANSPARENT);  

	//default_schedual_icon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_SCHEDUAL);	
	default_schedual_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_DEFAULT_SCHEDUAL),IMAGE_ICON,0,0, LR_LOADTRANSPARENT);  

	//default_trendlog_icon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_TRENDLOG);	
	default_trendlog_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_DEFAULT_TRENDLOG),IMAGE_ICON,0,0, LR_LOADTRANSPARENT);  



	default_screen_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_DEFAULT_GRAPHIC),IMAGE_ICON,0,0, LR_LOADTRANSPARENT);  
	//default_screen_icon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_GRAPHIC);	
	//default_screen_icon = (HICON)LoadImage(AfxGetInstanceHandle(),	MAKEINTRESOURCE(IDI_ICON_DEFAULT_GRAPHIC),IMAGE_ICON,0,0, LR_LOADTRANSPARENT );  
	//default_schedual_icon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_SCHEDUAL);	
	
	lock_icon = (HICON)LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_LABEL_LOCK), IMAGE_ICON,0,0,LR_LOADTRANSPARENT);  

	unlock_icon = (HICON)LoadImage(AfxGetInstanceHandle(), 	MAKEINTRESOURCE(IDI_ICON_LABEL_UNLOCK), IMAGE_ICON,0,0, LR_LOADTRANSPARENT);  


	//LoadBitmap()

#ifdef _DEBUG //debug版本   
	int nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[0],MOD_SHIFT,VK_UP); 
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey SHIFT + UP failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[1],MOD_SHIFT,VK_DOWN); 
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey SHIFT + DOWN failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[2],MOD_SHIFT,VK_LEFT); 
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey SHIFT + LEFT failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[3],MOD_SHIFT,VK_RIGHT);
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey SHIFT + RIGHT failure"));  
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey  RIGHT failure"));  
	nRet = RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[8],MOD_SHIFT,'M');
	if(!nRet)  
		AfxMessageBox(_T("RegisterHotKey SHIFT + M failure"));  
#else //release版本   
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[0],MOD_SHIFT,VK_UP); 
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[1],MOD_SHIFT,VK_DOWN); 
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[2],MOD_SHIFT,VK_LEFT); 
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[3],MOD_SHIFT,VK_RIGHT); 
	RegisterHotKey(GetSafeHwnd(),m_screenHotKeyID[8],MOD_SHIFT,'M'); 
#endif  

	
	memcpy_s(&m_temp_graphic_label_data,sizeof(Str_label_point) * BAC_GRPHIC_LABEL_COUNT,&m_graphic_label_data.at(0),sizeof(Str_label_point) * BAC_GRPHIC_LABEL_COUNT);
	
	//ReloadLabelsFromDB();
	m_screenedit_dlg_hwnd = this->m_hWnd;
	SetTimer(1,5000,NULL);
	SetTimer(2,10000,NULL);

	if(!m_bImgExist)
	{
		SetTimer(3,5000,NULL);
		show_not_exsit_dlg = true; //如果图片不存在，就在开始前几秒显示 不存在的 信息;
	}
	control_object_instance = g_bac_instance;
	Invalidate(1);
	::SetWindowPos(this->m_hWnd,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

    //if(h_read_all_panel_des_thread == NULL)
    //   h_read_all_panel_des_thread = CreateThread(NULL, NULL, ReadAllPanelThreadfun, this, NULL, NULL);

    //if(h_refresh_group_thread == NULL)
    //   h_refresh_group_thread = CreateThread(NULL, NULL, ReadGroupDataThreadfun, this, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

vector <Bacnet_RemotePoint_Info> m_remote_screen_data;

bool sort_by_panelnumber(Bacnet_RemotePoint_Info object1, Bacnet_RemotePoint_Info object2)
{
    return object1.npanelnum < object2.npanelnum;
}

DWORD WINAPI CBacnetScreenEdit::ReadGroupDataThreadfun(LPVOID lpVoid)
{
    CBacnetScreenEdit *mParent = (CBacnetScreenEdit *)lpVoid;
    Sleep(1000);
    while (1)
    {
        for (int i = 0;i < m_read_group_data.size();i++)
        {
            if (m_read_group_data.at(i).read_info.deviceid == 0)  //如果deviceid 是0 跳过刷新;并尝试根据panel获取device id;
            {
                bool find_in_other_thread = false;
                for (int j = 0; j < m_remote_screen_data.size(); j++)
                {
                    if ((m_read_group_data.at(i).point.panel == m_remote_screen_data.at(j).npanelnum)
                        && (m_remote_screen_data.at(j).npanelnum != 0))
                    {
                        find_in_other_thread = true;
                        m_read_group_data.at(i).read_info.deviceid = m_remote_screen_data.at(j).ndeviceid;
                        break;
                    }

                }
                if(find_in_other_thread == false)
                   continue;
            }
            if (GetPrivateData_Blocking(m_read_group_data.at(i).read_info.deviceid,
                m_read_group_data.at(i).read_info.command,
                m_read_group_data.at(i).read_info.value_item,
                m_read_group_data.at(i).read_info.value_item,
                m_read_group_data.at(i).read_info.entitysize,
                1) == 1)
            {
                memset(&m_read_group_data.at(i).detail_data, 0, sizeof(groupdata));
                Sleep(1);
                if (m_read_group_data.at(i).read_info.command == READINPUT_T3000)
                {
                    memcpy(&m_read_group_data.at(i).detail_data.m_group_input_data, &m_Input_data.at(m_read_group_data.at(i).read_info.value_item), sizeof(Str_in_point));
                }
                else if (m_read_group_data.at(i).read_info.command == READOUTPUT_T3000)
                {
                    memcpy(&m_read_group_data.at(i).detail_data.m_group_output_data, &m_Output_data.at(m_read_group_data.at(i).read_info.value_item), sizeof(Str_out_point));
                }
                else if (m_read_group_data.at(i).read_info.command == READVARIABLE_T3000)
                {
                    memcpy(&m_read_group_data.at(i).detail_data.m_group_variable_data, &m_Variable_data.at(m_read_group_data.at(i).read_info.value_item), sizeof(Str_variable_point));
                }
                //else if (m_read_group_data.at(i).read_info.command == READCONTROLLER_T3000)
                //else if (m_read_group_data.at(i).read_info.command == READWEEKLYROUTINE_T3000)
                //else if (m_read_group_data.at(i).read_info.command == READANNUALROUTINE_T3000)
                //else if (m_read_group_data.at(i).read_info.command == READANNUALROUTINE_T3000)
                //else if (m_read_group_data.at(i).read_info.command == READPROGRAM_T3000)
                //else if (m_read_group_data.at(i).read_info.command == READSCREEN_T3000)
                                                    
            }
        }
        Sleep(3000);

        //至少让先运行一次，以便下次打开的时候显示非常快速
        if (ScreenEdit_Window == NULL)
        {
            h_refresh_group_thread = NULL;
            return true;
        }
    }
    return true;
}


DWORD WINAPI  CBacnetScreenEdit::ReadAllPanelThreadfun(LPVOID lpVoid)
{
    //Write_Config_Info
    CBacnetScreenEdit *pParent = (CBacnetScreenEdit *)lpVoid;
    m_remote_screen_data.clear();
    while (1)
    {
        Send_WhoIs_Global(-1, -1);
        Sleep(1000);
        int ncount = address_count();
        for (int i = 0;i < ncount;i++)
        {
            unsigned int ndevice_id = 0;
            unsigned int nmax_apdu = 0;
            BACNET_ADDRESS src;
            address_get_by_index(i, &ndevice_id, &nmax_apdu, &src);

            bool find_device = false;
            for (int j = 0;j < m_remote_screen_data.size();j++)
            {
                if (m_remote_screen_data.at(j).ndeviceid == ndevice_id)
                {
                    find_device = true;
                }
            }
            if (find_device)
                continue;


            unsigned int t1 = GetTickCount();//程序段开始前取得系统运行时间(ms);
            //判断是不是他妈口的产品 
            Device_Basic_Setting.reg.object_instance = 0;
            if (GetPrivateData_Blocking(ndevice_id, READ_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), 1) > 0)
            {
                SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Read data success!"));
            }
            unsigned int t2 = GetTickCount();//程序段开始前取得系统运行时间(ms);
            Sleep(1);
            //Device_Basic_Setting.reg

            CString temp_cs;
            if (Device_Basic_Setting.reg.object_instance == ndevice_id)
            {
                TRACE(_T("%d exsit,time %d \n"), ndevice_id, t2 - t1);
                Bacnet_RemotePoint_Info temp_screen;
                temp_screen.ndeviceid = ndevice_id;
                temp_screen.npanelnum = 0;
                m_remote_screen_data.push_back(temp_screen);
            }
            else
            {
                TRACE(_T("%d not exsit,time %d\n"), ndevice_id, t2 - t1);
            }
            //是他妈口的产品就加入vector里 ，方便查询;
            Sleep(1);
        }

        //确定这些object instance 对应的panel 号
        for (int i = 0;i < m_remote_screen_data.size();i++)
        {
            if (m_remote_screen_data.at(i).npanelnum != 0)
                continue;
            if (GetPrivateData_Blocking(m_remote_screen_data.at(i).ndeviceid, READ_SETTING_COMMAND, 0, 0, sizeof(Str_Setting_Info), 1) > 0)
            {
                if (Device_Basic_Setting.reg.object_instance == m_remote_screen_data.at(i).ndeviceid)
                    m_remote_screen_data.at(i).npanelnum = Device_Basic_Setting.reg.panel_number;
                else
                    m_remote_screen_data.at(i).npanelnum = 0;
            }
            Sleep(SEND_COMMAND_DELAY_TIME);
        }
        sort(m_remote_screen_data.begin(), m_remote_screen_data.end(), sort_by_panelnumber);
        Sleep(1);
        //读取所有在线panel的label和description;

        if (ScreenEdit_Window == NULL)
        {
            h_read_all_panel_des_thread = NULL;
            return true;
        }
    }
    return true;
}

#pragma region MY_CODE


void CBacnetScreenEdit::InitGraphic(int nSerialNum,int nInstanceID,unsigned char screen_number)
{
	//m_strImgPathName=g_strImgeFolder+g_strImagePathName;
	if((screen_number <0) || (screen_number >= BAC_SCREEN_COUNT))
	{
		PostMessage(WM_CLOSE,NULL,NULL);
		return;
	}
	CString temp_info;
	temp_info.Format(_T("Screen %d"),screen_number+1);
	SetPaneString(BAC_SHOW_MISSION_RESULTS,temp_info);
	CString PicFileTips;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(screen_number).picture_file, (int)strlen((char *)m_screen_data.at(screen_number).picture_file)+1, 
		PicFileTips.GetBuffer(MAX_PATH), MAX_PATH );
	PicFileTips.ReleaseBuffer();

	m_strImgPathName=m_building_image_folder + _T("\\") + PicFileTips ;
	WIN32_FIND_DATA fData;
	HANDLE hFile=NULL; 
	hFile = FindFirstFile(m_strImgPathName,&fData);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		m_bImgExist=TRUE;
		FindClose(hFile);

		Bitmap bitmap(m_strImgPathName);
		int temp_width = bitmap.GetWidth();
		int temp_height = bitmap.GetHeight();
		float bitmap_x_y = (float)temp_width / (float)temp_height;
		float client_rect = (float)mynew_rect.Width()/ (float)mynew_rect.Height();


		if(bitmap_x_y < client_rect)
		{
			m_paint_botton_limit  = mynew_rect.Height();
			m_paint_right_limit = (int)(temp_width * ((float)m_paint_botton_limit / (float)temp_height));
		}
		else
		{
			m_paint_right_limit = mynew_rect.Width();
			m_paint_botton_limit = (int)(temp_height * ((float)m_paint_right_limit / (float)temp_width));
		}
	}
	else
	{
		m_paint_botton_limit = mynew_rect.Height();
		m_paint_right_limit = mynew_rect.Width();
		m_bImgExist=FALSE;
		FindClose(hFile);
		//自动删除相关记录，因为图象不存在，Label没有任何意思;

	}
	if(m_full_screen_mode)
	{
		m_paint_botton_limit = m_cyScreen;
		m_paint_right_limit = m_cxScreen;
	}
	m_nSerialNumber=nSerialNum;
	m_nTstatID=nInstanceID;
	//m_strScreenName=strScreenName;




	ReloadLabelsFromDB();
	this->SetFocus();
	Invalidate(1);
}



void CBacnetScreenEdit::ReloadLabelsFromDB()
{

	m_bac_label_vector.clear();
	m_graphic_refresh_data.clear();
	CString strSql;

	CString strtemp;
	strtemp.Empty();
	int nTemp;
	CString strTemp;
	int nItem = 0;//用于记录有多少个需要刷新;
	::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;

	for (int i=0;i<m_graphic_label_data.size();i++)
	{
		m_graphic_label_data.at(i).reg.nLabel_index = i;
		if(m_graphic_label_data.at(i).reg.label_status == NO_UNSED_LABEL)
		{
			break;
		}

        //dufan 如果是删掉过的label 就继续取下一个.
        if (m_graphic_label_data.at(i).reg.label_status == EMPTY_LABEL)
        {
            continue;
        }

		Bacnet_Label_Info bac_label;
		memset(&bac_label,0,sizeof(Bacnet_Label_Info));
		if((m_graphic_label_data.at(i).reg.nSerialNum == m_nSerialNumber) && (m_graphic_label_data.at(i).reg.nScreen_index == screen_list_line))
		{
			
			CString temp_icon_name;
			int temp_x_persent;
			int temp_y_persent;
			bac_label.nSerialNum = m_nSerialNumber;
			bac_label.nScreen_index = screen_list_line;

			bac_label.nLabel_index=m_graphic_label_data.at(i).reg.nLabel_index;
			bac_label.nMain_Panel = m_graphic_label_data.at(i).reg.nMain_Panel;
			bac_label.nSub_Panel = m_graphic_label_data.at(i).reg.nSub_Panel;
			bac_label.nPoint_type = m_graphic_label_data.at(i).reg.nPoint_type;

			if(bac_label.nPoint_type > 0)
				bac_label.nPoint_type = bac_label.nPoint_type - 1;
			bac_label.nPoint_number =  m_graphic_label_data.at(i).reg.nPoint_number;
			temp_x_persent = m_graphic_label_data.at(i).reg.nPoint_x;
			temp_y_persent =  m_graphic_label_data.at(i).reg.nPoint_y;
			if(temp_x_persent > 1000)
				temp_x_persent = 980;
			if(temp_y_persent > 1000)
				temp_y_persent = 980;
			if(!m_full_screen_mode)
			{
				bac_label.nPoint_x = (int)((temp_x_persent*m_paint_right_limit)/1000);
				bac_label.nPoint_y = (int)((temp_y_persent*m_paint_botton_limit)/1000);
			}
			else
			{
				int width = GetSystemMetrics ( SM_CXSCREEN );  
				int height= GetSystemMetrics ( SM_CYSCREEN ); 
				bac_label.nPoint_x = (int)((temp_x_persent*width)/1000);
				bac_label.nPoint_y = (int)((temp_y_persent*height)/1000);
			}

			bac_label.nclrTxt = m_graphic_label_data.at(i).reg.nclrTxt;
			bac_label.nDisplay_Type =m_graphic_label_data.at(i).reg.nDisplay_Type;


			memcpy(bac_label.ico_name,m_graphic_label_data.at(i).reg.icon_name_1,STR_ICON_1_NAME_LENGTH);
			memcpy(bac_label.ico_name_2,m_graphic_label_data.at(i).reg.icon_name_2,STR_ICON_2_NAME_LENGTH);

			bac_label.n_iconsize =m_graphic_label_data.at(i).reg.nIcon_size;
			bac_label.ntext_place = m_graphic_label_data.at(i).reg.nIcon_place;
			if(bac_label.n_iconsize > 2)
				bac_label.n_iconsize = 0;
			if(bac_label.ntext_place > 3)
				bac_label.n_iconsize = 0;
			bac_label.nMouse_Status = LABEL_MOUSE_NORMAL;
			m_bac_label_vector.push_back(bac_label);

			_Graphic_Value_Info temp1;
            if (bac_label.nMain_Panel == Station_NUM)
			    temp1.deviceid = g_bac_instance;//暂时只支持 读本地的 instance;
            else
            {
                temp1.deviceid = 0; //非本地的如果在远程的点中 找不大 就先  标记为0.
                for (int i = 0; i < m_remote_screen_data.size(); i++)
                {
                    if ((bac_label.nMain_Panel == m_remote_screen_data.at(i).npanelnum))
                    {
                        temp1.deviceid = m_remote_screen_data.at(i).ndeviceid;
                        break;
                    }
                }
                
                
            }
                
			temp1.value_type = bac_label.nPoint_type;
			temp1.value_item = bac_label.nPoint_number;

            //dufan 改为能够支持远程的panel.
			//if((bac_label.nMain_Panel == Station_NUM) && (bac_label.nSub_Panel == Station_NUM))  //如果是这个设备下面的 才加到自动刷新 的里面;
			//{
				if(bac_label.nPoint_type == BAC_IN)
				{
					if(temp1.value_item >= BAC_INPUT_ITEM_COUNT)
						continue;
					temp1.command = READINPUT_T3000;
					temp1.entitysize = sizeof(Str_in_point);
					m_graphic_refresh_data.push_back(temp1);
				}
				else if(bac_label.nPoint_type == BAC_OUT)
				{
					if(temp1.value_item >= BAC_OUTPUT_ITEM_COUNT)
						continue;
					temp1.command = READOUTPUT_T3000;
					temp1.entitysize = sizeof(Str_out_point);
					m_graphic_refresh_data.push_back(temp1);
				}
				else if(bac_label.nPoint_type == BAC_VAR)
				{
					if(temp1.value_item >= BAC_VARIABLE_ITEM_COUNT)
						continue;
					temp1.command = READVARIABLE_T3000;
					temp1.entitysize = sizeof(Str_variable_point);
					m_graphic_refresh_data.push_back(temp1);
				}
				else if(bac_label.nPoint_type == BAC_PID)
				{
					if(temp1.value_item >= BAC_PID_COUNT)
						continue;
					temp1.command = READCONTROLLER_T3000;
					temp1.entitysize = sizeof(Str_controller_point);
					m_graphic_refresh_data.push_back(temp1);
				}

			//}
                bool temp_exsit = false;
                for (int i = 0; i < m_read_group_data.size(); i++)
                {
                    if (memcmp(&m_read_group_data.at(i).read_info, &temp1, sizeof(_Graphic_Value_Info)) == 0)
                    {
                        temp_exsit = true;
                        break;
                    }
                }
                if (!temp_exsit)
                {
                    _Group_Data temp_group_data;
                    memset(&temp_group_data, 0, sizeof(_Group_Data));
                    memcpy(&temp_group_data.read_info, &temp1, sizeof(_Graphic_Value_Info));
                    temp_group_data.point.panel = bac_label.nMain_Panel;
                    temp_group_data.point.sub_panel = bac_label.nSub_Panel;
                    temp_group_data.point.point_type = bac_label.nPoint_type;
                    temp_group_data.point.number = bac_label.nPoint_number;
                    temp_group_data.point.network = 1;

                    m_read_group_data.push_back(temp_group_data);
                }

            if((bac_label.nMain_Panel == Station_NUM) && (bac_label.nSub_Panel != 0))
			{
				m_enable_send_remote_point = true;
			}

			nItem ++;

		}
	}
	  
}



void CBacnetScreenEdit::PainNoImageInfo(CDC* pDC)
{
	const int iOffset = 20;
	CString strInfo=_T("No image associated with this graphic screen.Click to not  see it again. ");

	CFont* pFontOld = (CFont*) pDC->SelectStockObject (DEFAULT_GUI_FONT);
	ASSERT (pFontOld != NULL);

	CRect rectClient;
	GetClientRect (&rectClient);
	pDC->FillSolidRect(&rectClient,RGB(238,245,250));

	CRect rectText = rectClient;
	rectText.DeflateRect (iOffset, iOffset);

	pDC->DrawText (strInfo, rectText, DT_CALCRECT | DT_WORDBREAK);

	rectText.OffsetRect (	(rectClient.Width () - rectText.Width () - 2 * iOffset) / 2,
		(rectClient.Height () - rectText.Height () - 2 * iOffset) / 2);

	CRect rectFrame = rectText;
	rectFrame.InflateRect (iOffset, iOffset);

	pDC->FillSolidRect (rectFrame, ::GetSysColor (COLOR_INFOBK));
	rectFrame.DeflateRect (1, 1);
	pDC->Draw3dRect (rectFrame, ::GetSysColor (COLOR_3DSHADOW), 
		::GetSysColor (COLOR_3DLIGHT));

	rectFrame.DeflateRect (2, 2);
	pDC->Draw3dRect (rectFrame, ::GetSysColor (COLOR_3DSHADOW), 
		::GetSysColor (COLOR_3DLIGHT));

	pDC->SetTextColor (::GetSysColor (COLOR_INFOTEXT));
	pDC->SetBkMode (TRANSPARENT);

	pDC->DrawText (strInfo, rectText, DT_WORDBREAK);

	pDC->SelectObject (pFontOld);

}


void CBacnetScreenEdit::Bacnet_Edit_Label(bool allow_change)
{
	if(m_bac_select_label<0)
		return;
	if(Edit_Label_Window != NULL)
	{
		delete Edit_Label_Window;
		Edit_Label_Window = NULL;
	}
	if(Edit_Label_Window ==NULL)
	{
		Edit_Label_Window = new CBacnetEditLabel;
		Edit_Label_Window->Create(IDD_DIALOG_BACNET_EDIT_LABEL, this);
	}
	//if(allow_change)
		Edit_Label_Window->m_allow_change = true;
	//else
	//	Edit_Label_Window->m_allow_change = false;
	Edit_Label_Window->FreshWindow(m_bac_label_vector.at(m_bac_select_label));
	
}

void CBacnetScreenEdit::Bacnet_Add_Label()
{
	if(Add_Label_Window != NULL)
	{
		delete Add_Label_Window;
		Add_Label_Window = NULL;
	}
	if(Add_Label_Window ==NULL)
	{
		Add_Label_Window = new CBacnetAddLabel;
		Add_Label_Window->Create(IDD_DIALOG_BACNET_ADD_LABEL, this);
	}
	Add_Label_Window->FreshWindow();
	return;
}

int CBacnetScreenEdit::JudgeClickItem(CPoint & point)
{
	bool click_ret = false;
	for (int i=0;i<m_bac_label_vector.size();i++)
	{
		int rect_x = m_bac_label_vector.at(i).nPoint_x;
		int rect_x_right = rect_x + 160;
		int rect_y = m_bac_label_vector.at(i).nPoint_y;
		int rect_y_botton = rect_y + 30;

		if((point.x > rect_x ) && (point.x < rect_x_right) && (point.y > rect_y) && (point.y < rect_y_botton))
		{
			m_bac_label_vector.at(i).nMouse_Status = LABEL_MOUSE_ON_LB_DOWN;
			m_bac_select_label = i;
			click_ret = true;
			for (int j=i+1; j<m_bac_label_vector.size();j++)
			{
				m_bac_label_vector.at(j).nMouse_Status = LABEL_MOUSE_NORMAL;
			}
			Invalidate(1);
			return m_bac_select_label;
		}
		else if((m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_VALUE) ||
			(m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_FULL_DESCRIPTION) ||
			(m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_LABEL) ||
            (m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_ONLY) ||
			(m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_SHOW_VALUE))
		{
			int temp_index = 0;
			temp_index = m_bac_label_vector.at(i).nPoint_number;
			int delt_x =0;
			int delt_y =0;
			if(m_bac_label_vector.at(i).n_iconsize == LABEL_ICON_SMALL)
			{
				delt_x = delt_y = 32;
			}
			else if(m_bac_label_vector.at(i).n_iconsize == LABEL_ICON_NORMAL)
			{
				delt_x = delt_y = 64;
			}
			else 
			{
				delt_x = delt_y = 96;
			}

			if(m_bac_label_vector.at(i).ntext_place == LABEL_TEXT_BOTTOM)
			{
				rect_x = m_bac_label_vector.at(i).nPoint_x;
				rect_y = m_bac_label_vector.at(i).nPoint_y - delt_y;
				rect_x_right = m_bac_label_vector.at(i).nPoint_x + delt_x;
				rect_y_botton = m_bac_label_vector.at(i).nPoint_y;
			}
			else if(m_bac_label_vector.at(i).ntext_place == LABEL_TEXT_RIGHT)
			{
				rect_x = m_bac_label_vector.at(i).nPoint_x - delt_x;
				rect_y = m_bac_label_vector.at(i).nPoint_y ;
				rect_x_right = m_bac_label_vector.at(i).nPoint_x ;
				rect_y_botton = m_bac_label_vector.at(i).nPoint_y + delt_y;
			}
			else if(m_bac_label_vector.at(i).ntext_place == LABEL_TEXT_TOP)
			{
				rect_x = m_bac_label_vector.at(i).nPoint_x;
				rect_y = m_bac_label_vector.at(i).nPoint_y  + 30;
				rect_x_right = m_bac_label_vector.at(i).nPoint_x + delt_x;
				rect_y_botton = m_bac_label_vector.at(i).nPoint_y + delt_y;
			}

			m_bac_label_vector.at(i).nMouse_Status = LABEL_MOUSE_NORMAL;

			if((point.x > rect_x ) && (point.x < rect_x_right) && (point.y > rect_y) && (point.y < rect_y_botton))
			{
				m_bac_label_vector.at(i).nMouse_Status = LABEL_MOUSE_ON_LB_DOWN;
				m_bac_select_label = i;
				click_ret = true;
				return m_bac_select_label;
			}
		}
	}
	return -1;
}


int CBacnetScreenEdit::HitTestEx(CPoint & point)
{
	for(int i=0;i<(int)m_RelayLabelLst.size();i++)
	{
		CRect rcItem;
		m_RelayLabelLst.at(i).plabelCtrl->GetWindowRect(&rcItem);
		ScreenToClient(&rcItem);
		if (rcItem.PtInRect(point))
		{
			return i;
		}
	}
	return -1;
}


#pragma endregion

void CBacnetScreenEdit::OnPaint()
{
	CString cs_show_info;
	CString cs_value;
	CString cs_unit;
	CString cs_auto_m;
	int dig_unit_ret = -1;
	CString cs_label;
	CString cs_full_label;
	CPaintDC dc(this); // device context for painting
	//TRACE(_T("Screen Edit Paint\r\n"));
	::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;

	
	
	// Do not call CDialogEx::OnPaint() for painting messages

	CMemDC memDC(dc,this);
	CRect rcClient;
	GetClientRect(&rcClient);
	//memDC.GetDC().FillSolidRect(&rcClient,::GetSysColor (COLOR_3DLIGHT));
	//	if(g_strImagePathName.IsEmpty())

	if(!m_bImgExist)
	{
		if((show_not_exsit_dlg) && (screen_show_nopic == 1))
		{
				PainNoImageInfo(&memDC.GetDC());
		}
		else
		{
			memDC.GetDC().FillSolidRect(&rcClient,RGB(238,245,250));

		}
	}
	else
	{	
		//	CMemDC memDC(dc,this);
		memDC.GetDC().FillSolidRect(&rcClient,RGB(202,208,216));
		Graphics graphics(memDC.GetDC());
		Bitmap bitmap(m_strImgPathName);

		if(!m_full_screen_mode)
		{
			int temp_width = bitmap.GetWidth();
			int temp_height = bitmap.GetHeight();
			float bitmap_x_y = (float)temp_width / (float)temp_height;
			float client_rect = (float)mynew_rect.Width()/ (float)mynew_rect.Height();
			if(bitmap_x_y < client_rect)
			{
				m_paint_botton_limit  = mynew_rect.Height();
				m_paint_right_limit = (int)(temp_width * ((float)m_paint_botton_limit / (float)temp_height));
			}
			else
			{
				m_paint_right_limit = mynew_rect.Width();
				m_paint_botton_limit = (int)(temp_height * ((float)m_paint_right_limit / (float)temp_width));
			}
			graphics.DrawImage(&bitmap,XStart ,YStart,m_paint_right_limit,m_paint_botton_limit - YStart);
		}
		else
		{
			int width = GetSystemMetrics ( SM_CXSCREEN );  
			int height= GetSystemMetrics ( SM_CYSCREEN ); 
			graphics.DrawImage(&bitmap,0 ,0,width,height);
		}
		//graphics.DrawImage(&bitmap,XStart ,YStart,mynew_rect.Width(),mynew_rect.Height() - YStart);
		

		//Bitmap bitmap1(h_icon_default,NULL);
		//graphics.DrawImage(&bitmap1,100 ,100,164,164);


	}



	for (int i=0;i<m_bac_label_vector.size();i++)
	{
		int read_bac_index = 0;
		bool label_invalid = false;
		cs_value.Empty();
		cs_unit.Empty();
		cs_auto_m.Empty();
		cs_label.Empty();
		cs_full_label.Empty();
		cs_show_info.Empty();
		Graphics *mygraphics;
		mygraphics = new Graphics(memDC.GetDC());

		int icon_size_x = 32;
		int icon_size_y = 32;
		PointF      pointF(0, 0);
		UCHAR B_value = (UCHAR) ((m_bac_label_vector.at(i).nclrTxt & 0xff0000) >> 16);
		UCHAR G_value = (UCHAR) ((m_bac_label_vector.at(i).nclrTxt & 0x00ff00) >> 8);
		UCHAR R_value = (UCHAR) ((m_bac_label_vector.at(i).nclrTxt & 0x0000ff));
		if(m_bac_label_vector.at(i).nMouse_Status == LABEL_MOUSE_ON_LB_DOWN)
		{
			R_value = 160;G_value = 0; B_value = 160;
		}
		SolidBrush  txt_color_brush(Color(255,R_value,G_value,B_value));

		FontFamily  UnitfontFamily(_T("Arial"));
		Gdiplus::Font        unitfont(&UnitfontFamily, 16, FontStyleRegular, UnitPixel);
		pointF.X = m_bac_label_vector.at(i).nPoint_x;
		pointF.Y = m_bac_label_vector.at(i).nPoint_y;


		if(m_bac_label_vector.at(i).n_iconsize == LABEL_ICON_SMALL )
		{
			icon_size_x = 32; icon_size_y = 32;
		}
		else if(m_bac_label_vector.at(i).n_iconsize == LABEL_ICON_NORMAL )
		{
			icon_size_x = 64; icon_size_y = 64;
		}
		else if(m_bac_label_vector.at(i).n_iconsize == LABEL_ICON_LARGE )
		{
			icon_size_x = 96; icon_size_y = 96;
		}

        if ((m_bac_label_vector.at(i).nMain_Panel == m_bac_label_vector.at(i).nSub_Panel) &&
            (m_bac_label_vector.at(i).nMain_Panel != Station_NUM))
        {
            //访问不同的 主panel 远程的点的情况;
            Point_Net temp_point;
            temp_point.panel = m_bac_label_vector.at(i).nMain_Panel;
            temp_point.sub_panel = m_bac_label_vector.at(i).nSub_Panel;
            temp_point.point_type = m_bac_label_vector.at(i).nPoint_type;
            temp_point.number = m_bac_label_vector.at(i).nPoint_number;
            temp_point.network = 1;
            char original_point[30];
            memset(original_point, 0, 30);
            pointtotext(original_point, &temp_point);
            CString temp_des;
            MultiByteToWideChar(CP_ACP, 0, original_point, (int)strlen(original_point) + 1,
                temp_des.GetBuffer(MAX_PATH), MAX_PATH);
            temp_des.ReleaseBuffer();
            cs_label = temp_des;

            bool find_value = false;
            int read_group_index = 0;
            for (int j = 0; j < m_read_group_data.size(); j++)
            {
                if ((m_read_group_data.at(j).point.panel == m_bac_label_vector.at(i).nMain_Panel) &&
                    (m_read_group_data.at(j).point.sub_panel == m_bac_label_vector.at(i).nSub_Panel) &&
                    (m_read_group_data.at(j).point.point_type == m_bac_label_vector.at(i).nPoint_type) &&
                    (m_read_group_data.at(j).point.number == m_bac_label_vector.at(i).nPoint_number))
                {
                    find_value = true;
                    read_group_index = j;
                    break;
                }
            }
            if (find_value == false)
            {
                cs_value = _T("");
                cs_unit = _T("");
                cs_auto_m = _T("");

            }
            int get_ret = 0;
            int get_label = 0;
            int get_full_label = 0;
            unsigned char temp_type = m_bac_label_vector.at(i).nPoint_type;
            switch (temp_type)
            {
            case BAC_IN://INPUT
                {
                //dufan 先将原input0 的值存起来，然后 将其他panel 的input 放入 ，方便调用 GetInputValue等函数取 解析值.
                //随后解析完在还原.
                Str_in_point temp_data_in;
                memcpy(&temp_data_in, &m_Input_data.at(0), sizeof(Str_in_point));

                memcpy(&m_Input_data.at(0), &m_read_group_data.at(read_group_index).detail_data.m_group_input_data, sizeof(Str_in_point));

                get_ret = GetInputValue(0, cs_value, cs_unit, cs_auto_m, dig_unit_ret);
                get_label = GetInputLabel(0, cs_label, &m_read_group_data.at(read_group_index).point);
                get_full_label = GetInputFullLabel(0, cs_full_label, &m_read_group_data.at(read_group_index).point);
                memcpy(&m_Input_data.at(0), &temp_data_in, sizeof(Str_in_point));
                if ((get_ret < 0) || (get_label < 0) || (get_full_label < 0))
                    label_invalid = true;
                 }
            break;
            case BAC_OUT:
                //dufan 先将原output0 的值存起来，然后 将其他panel 的output 放入 ，方便调用 GetOutputValue等函数取 解析值.
                //随后解析完在还原.
                Str_out_point temp_data_out;
                memcpy(&temp_data_out, &m_Output_data.at(0), sizeof(Str_out_point));

                memcpy(&m_Output_data.at(0), &m_read_group_data.at(read_group_index).detail_data.m_group_output_data, sizeof(Str_out_point));

                get_ret = GetOutputValue(0, cs_value, cs_unit, cs_auto_m, dig_unit_ret);
                get_label = GetOutputLabel(0, cs_label, &m_read_group_data.at(read_group_index).point);
                get_full_label = GetOutputFullLabel(0, cs_full_label, &m_read_group_data.at(read_group_index).point);
                memcpy(&m_Output_data.at(0), &temp_data_out, sizeof(Str_out_point));
                if ((get_ret < 0) || (get_label < 0) || (get_full_label < 0))
                    label_invalid = true;
                break;
            case BAC_VAR:
                //dufan 先将原variable0 的值存起来，然后 将其他panel 的variable 放入 ，方便调用 GetvariableValue等函数取 解析值.
                //随后解析完在还原.
                Str_variable_point temp_data_var;
                memcpy(&temp_data_var, &m_Variable_data.at(0), sizeof(Str_variable_point));

                memcpy(&m_Variable_data.at(0), &m_read_group_data.at(read_group_index).detail_data.m_group_variable_data, sizeof(Str_variable_point));

                get_ret = GetVariableValue(0, cs_value, cs_unit, cs_auto_m, dig_unit_ret);
                get_label = GetVariableLabel(0, cs_label, &m_read_group_data.at(read_group_index).point);
                get_full_label = GetVariableFullLabel(0, cs_full_label, &m_read_group_data.at(read_group_index).point);
                memcpy(&m_Variable_data.at(0), &temp_data_var, sizeof(Str_variable_point));
                //if ((get_ret < 0) || (get_label < 0) || (get_full_label < 0))
                //    label_invalid = true;
                break;
            default:
                break;
            }

        }
		else if((m_bac_label_vector.at(i).nSub_Panel != Station_NUM) && (m_bac_label_vector.at(i).nSub_Panel != 0))
		{
			Point_Net temp_point;
			temp_point.panel = Station_NUM;
			temp_point.sub_panel = m_bac_label_vector.at(i).nSub_Panel;
			temp_point.point_type = m_bac_label_vector.at(i).nPoint_type;
			temp_point.number = m_bac_label_vector.at(i).nPoint_number;
			temp_point.network = 1;
			char original_point[30];
			memset(original_point,0,30);
			pointtotext(original_point,&temp_point);
			CString temp_des;
			MultiByteToWideChar( CP_ACP, 0, original_point, (int)strlen(original_point)+1, 
				temp_des.GetBuffer(MAX_PATH), MAX_PATH );
			temp_des.ReleaseBuffer();
			cs_label = temp_des;
			bool found_index = false;
			for (int i=0;i<(int)m_remote_point_data.size();i++)
			{
				if(m_remote_point_data.at(i).point.sub_panel == 0 )
				{
					break;
				}
				if((m_remote_point_data.at(i).point.sub_panel == temp_point.sub_panel) &&
					(m_remote_point_data.at(i).point.point_type == temp_point.point_type + 1) &&
					(m_remote_point_data.at(i).point.number == temp_point.number))
				{
					if(m_remote_point_data.at(i).device_online == 0)
					{
						cs_value.Format(_T("%s   N/A"),temp_des,(m_remote_point_data.at(i).point_value));
					}
					else
						cs_value.Format(_T("%d"),(m_remote_point_data.at(i).point_value));
					found_index = true;
					CString temp_description;
					unsigned short dev_reg = 0;
					unsigned char high_3bit ;
					high_3bit = (unsigned char)((( m_remote_point_data.at(i).point.point_type ) & 0xE0) >> 5);
					dev_reg = high_3bit * 256 + m_remote_point_data.at(i).point.number;

					unsigned char nFlag = m_remote_point_data.at(i).product_id;
					if((nFlag == PM_TSTAT6) || (nFlag == PM_TSTAT7)|| (nFlag == PM_TSTAT5i)|| (nFlag == PM_TSTAT8) )
					{
						MultiByteToWideChar( CP_ACP, 0, (char *)TSTAT_6_ADDRESS[dev_reg].AddressName,(int)strlen((char *)TSTAT_6_ADDRESS[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
						temp_description.ReleaseBuffer();	

					}
					else if((nFlag == PM_TSTATRUNAR)||(nFlag == PM_TSTAT5E) || (nFlag == PM_TSTAT5H)||(nFlag==PM_TSTAT5G))
					{
						MultiByteToWideChar( CP_ACP, 0, (char *)TSTAT_5EH_LCD_ADDRESS[dev_reg].AddressName,(int)strlen((char *)TSTAT_5EH_LCD_ADDRESS[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
						temp_description.ReleaseBuffer();
					}
					else if((nFlag == PM_TSTAT5A) ||(nFlag == PM_TSTAT5B) ||
						(nFlag ==PM_TSTAT5C ) || (nFlag == PM_TSTAT5D) || (nFlag == PM_TSTAT5F))
					{
						MultiByteToWideChar( CP_ACP, 0, (char *)TSTAT_5ABCDFG_LED_ADDRESS[dev_reg].AddressName,(int)strlen((char *)TSTAT_5ABCDFG_LED_ADDRESS[dev_reg].AddressName)+1, temp_description.GetBuffer(MAX_PATH), MAX_PATH );
						temp_description.ReleaseBuffer();
					}
					else
					{
						temp_description.Empty();
					}
					CString cs_temp_1;
					if(!temp_description.IsEmpty())
					{
						if(temp_description.GetLength() > 7)
						{
							cs_temp_1 = temp_description.Left(7);
							if(cs_temp_1.CompareNoCase(_T("MODBUS_")) == 0)
								temp_description = temp_description.Mid(7);
						}
					}

					cs_full_label = temp_description;
					break;
				}
			}
			if(found_index == false)
			{
				cs_value.Format(_T("N/A"));
			}

			
		}
		else
		{
			//**********************************************************************
			// 得到 Value; 
			read_bac_index = m_bac_label_vector.at(i).nPoint_number;
			dig_unit_ret = -1;
			unsigned char temp_type = m_bac_label_vector.at(i).nPoint_type ;
			switch(temp_type)
			{
			case BAC_IN://INPUT
				{
					if(read_bac_index < BAC_INPUT_ITEM_COUNT)
					{		
						int get_ret = GetInputValue(read_bac_index ,cs_value,cs_unit,cs_auto_m,dig_unit_ret);
						int get_label = GetInputLabel(read_bac_index,cs_label);
						int get_full_label = GetInputFullLabel(read_bac_index,cs_full_label);
						if((get_ret <0) || (get_label < 0) || (get_full_label < 0))
							label_invalid = true;
					}
					else
					{
						label_invalid = true;
					}
				}
				break;
			case BAC_OUT://OUTPUT
				{
					if(read_bac_index < BAC_OUTPUT_ITEM_COUNT)
					{
						int get_ret_out = GetOutputValue(read_bac_index ,cs_value,cs_unit,cs_auto_m,dig_unit_ret);
						int get_label_out = GetOutputLabel(read_bac_index,cs_label);
						int get_full_label_out = GetOutputFullLabel(read_bac_index,cs_full_label);
						if((get_ret_out <0) || (get_label_out < 0) || (get_full_label_out < 0))
							label_invalid = true;
					}
					else
						label_invalid = true;
				}
				break;
			case BAC_VAR://VARIABLE
				{
					if(read_bac_index < BAC_VARIABLE_ITEM_COUNT)
					{
						int get_ret_var = GetVariableValue(read_bac_index ,cs_value,cs_unit,cs_auto_m,dig_unit_ret);
						int get_label_var = GetVariableLabel(read_bac_index,cs_label);
						int get_full_label_var = GetVariableFullLabel(read_bac_index,cs_full_label);
						if((get_ret_var <0) || (get_label_var < 0) || (get_full_label_var < 0))
							label_invalid = true;
					}
					else
						label_invalid = true;
				}
				break;
			case BAC_PID: //PID Controller
				{
					if(read_bac_index < BAC_PID_COUNT)
					{
						cs_label.Format(_T("PID%d"),read_bac_index + 1);
						cs_full_label.Format(_T("PID%d"),read_bac_index + 1);
						int get_pid_var = GetPidValue(read_bac_index,cs_auto_m,cs_value);
						if(get_pid_var < 0)
							label_invalid = true;
					}
					else
						label_invalid = true;
				}
				break;
			case BAC_SCH: //SCHEDULE
				{
					if(read_bac_index < BAC_HOLIDAY_COUNT)
					{
						int get_label_var = GetScheduleLabel(read_bac_index,cs_label);
						int get_full_label_var = GetScheduleFullLabel(read_bac_index,cs_full_label);
                        int get_ret_var = GetScheduleValue(read_bac_index, cs_auto_m, cs_value);
                        if ((get_ret_var <0) || (get_label_var < 0) || (get_full_label_var < 0))
						//if((get_label_var < 0) || (get_full_label_var < 0))
							label_invalid = true;
					}
					else
						label_invalid = true;
				}
				break;
			case BAC_HOL: //HOLIDAY
				{
					if(read_bac_index < BAC_HOLIDAY_COUNT)
					{
						int get_label_var = GetHolidayLabel(read_bac_index,cs_label);
						int get_full_label_var = GetHolidayFullLabel(read_bac_index,cs_full_label);
                        int get_ret_var = GetHolidayValue(read_bac_index, cs_auto_m, cs_value);
                        if ((get_ret_var <0) || (get_label_var < 0) || (get_full_label_var < 0))
						//if((get_label_var < 0) || (get_full_label_var < 0))
							label_invalid = true;
					}
					else
						label_invalid = true;
				}
				break;
			case BAC_PRG: //Program
				{
					if(read_bac_index < BAC_PROGRAM_ITEM_COUNT)
					{
						int get_label_prg = GetPrgLabel(read_bac_index,cs_label);
						int get_full_label_prg = GetPrgFullLabel(read_bac_index,cs_full_label);
						if((get_label_prg < 0) || (get_full_label_prg < 0))
							label_invalid = true;
					}
					else
						label_invalid = true;
				}
				break;
			case BAC_AMON:
				{
					if(read_bac_index < BAC_MONITOR_COUNT)
					{
						int get_label_amon = GetAmonLabel(read_bac_index,cs_label);
						cs_full_label = cs_label;
						int get_full_label_amon = 0;
						if((get_label_amon < 0) || (get_full_label_amon < 0))
							label_invalid = true;
					}
					else
						label_invalid = true;
				}
				break;
			case BAC_GRP:
				{
					if(read_bac_index < BAC_SCREEN_COUNT)
					{
						if(m_bac_label_vector.at(i).nDisplay_Type == LABEL_SHOW_VALUE)
							m_bac_label_vector.at(i).nDisplay_Type = LABEL_SHOW_LABEL;
						GetScreenLabel(read_bac_index,cs_label);
						GetScreenFullLabel(read_bac_index,cs_full_label);
						cs_value.Empty();
						cs_unit.Empty();
						cs_auto_m.Empty();
						label_invalid = false;
					}
					else
						label_invalid = true;
				}
				break;
			default:
				label_invalid = true;
				break;
			}
		}

		//**********************************************************************
		if(label_invalid)
		{
			cs_show_info = _T("Label Invalid");
		}
		else
		{
			switch(m_bac_label_vector.at(i).nDisplay_Type)
			{
			case LABEL_SHOW_VALUE:
				cs_show_info = cs_value + _T("  ") + cs_unit + _T("  ") + cs_auto_m;
				break;
			case LABEL_SHOW_FULL_DESCRIPTION:
				cs_show_info = cs_full_label + _T("  ") + cs_value + _T("  ") + cs_unit + _T("  ") + cs_auto_m;
				break;
			case LABEL_SHOW_LABEL:
				cs_show_info = cs_label + _T("  ") + cs_value + _T("  ") + cs_unit + _T("  ") + cs_auto_m;
				break;
			case LABEL_ICON_VALUE:
				cs_show_info = cs_value + _T("  ") + cs_unit + _T("  ") + cs_auto_m;
				break;
			case LABEL_ICON_FULL_DESCRIPTION:
				cs_show_info = cs_full_label + _T("  ") + cs_value + _T("  ") + cs_unit + _T("  ") + cs_auto_m;
				break;
			case LABEL_ICON_LABEL:
				cs_show_info = cs_label + _T("  ") + cs_value + _T("  ") + cs_unit + _T("  ") + cs_auto_m;
				break;
			case LABEL_ICON_SHOW_VALUE:
				cs_show_info = cs_value + _T("  ") + cs_unit + _T("  ") + cs_auto_m;
				break;
            case LABEL_ICON_ONLY:
                cs_show_info.Empty();
                break;
			default:
				{
					cs_show_info = _T("Label Invalid");
					break;
				}
			}
		}
		cs_show_info = cs_show_info.Trim();
		if((cs_show_info.IsEmpty()) && (m_bac_label_vector.at(i).nDisplay_Type != LABEL_ICON_VALUE))
		{
			if(m_bac_label_vector.at(i).nPoint_type == BAC_PRG)
				cs_show_info.Format(_T("PRG%u"),m_bac_label_vector.at(i).nPoint_number + 1);
			else if(m_bac_label_vector.at(i).nPoint_type == BAC_SCH)
				cs_show_info.Format(_T("SCH%u"),m_bac_label_vector.at(i).nPoint_number + 1);
			else if(m_bac_label_vector.at(i).nPoint_type == BAC_HOL)
				cs_show_info.Format(_T("HOL%u"),m_bac_label_vector.at(i).nPoint_number + 1);
			else if(m_bac_label_vector.at(i).nPoint_type == BAC_AMON)
				cs_show_info.Format(_T("AMON%u"),m_bac_label_vector.at(i).nPoint_number + 1);
			else
				cs_show_info = _T("Empty Label");
		}
		int x_point = 0;
		int y_point = 0;
		int right_point = 0;
		int botton_point = 0;
		int cstring_length = 0;
        cstring_length = cs_show_info.GetLength();
        if (cstring_length == 0)
            cstring_length = 3;
        right_point = cstring_length * 8;
		if(m_bac_label_vector.at(i).nMouse_Status == LABEL_MOUSE_ON_LB_DOWN)
		{
			Pen *myRectangle_pen;

			

			x_point = pointF.X - 2;
			y_point = pointF.Y - 2;

			botton_point =  30;

			myRectangle_pen = new Pen(Color(255,0,255,255));

			mygraphics->DrawRectangle(myRectangle_pen,x_point,y_point,right_point,botton_point);
			delete myRectangle_pen;
		}

        m_bac_label_vector.at(i).x_length = right_point;

		if((m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_LABEL) ||
			(m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_VALUE) ||
            (m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_ONLY) ||
			(m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_SHOW_VALUE) ||
			(m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_FULL_DESCRIPTION))
		{
			CString temp_cstring;
			CString temp2_cstring;
			CString icon_full_path;
			CString icon_2_full_path;
			//if((dig_unit_ret == 0) || (dig_unit_ret == 1))
			//{
				MultiByteToWideChar( CP_ACP, 0, (char *)m_bac_label_vector.at(i).ico_name,(int)strlen((char *)m_bac_label_vector.at(i).ico_name)+1, 
					temp_cstring.GetBuffer(MAX_PATH), MAX_PATH );
				temp_cstring.ReleaseBuffer();

				MultiByteToWideChar( CP_ACP, 0, (char *)m_bac_label_vector.at(i).ico_name_2,(int)strlen((char *)m_bac_label_vector.at(i).ico_name_2)+1, 
					temp2_cstring.GetBuffer(MAX_PATH), MAX_PATH );
				temp2_cstring.ReleaseBuffer();
			//}


			if(m_bac_label_vector.at(i).nPoint_type == BAC_GRP)
			{
				unsigned int temp_index = m_bac_label_vector.at(i).nPoint_number;
				if(temp_index >= BAC_SCREEN_COUNT)
				{
					icon_full_path = m_building_image_folder + _T("\\") + temp_cstring ;//_T("sample1.bmp");
					icon_2_full_path = m_building_image_folder + _T("\\") + temp2_cstring ;//_T("sample1.bmp");
				}
				else
				{
                    CString ApplicationFolder;
                    CString image_fordor;
                    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
                    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
                    ApplicationFolder.ReleaseBuffer();
                    CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
                    image_fordor = ApplicationFolder + _T("\\Database\\Buildings\\") + pFrame->m_strCurMainBuildingName + _T("\\image");
                    CFileFind temp_find;
                    if (temp_find.FindFile(image_fordor) == 0)
                    {
                        CreateDirectory(image_fordor, NULL);
                    }
                    CString new_file_path;
                    CString FileName;

                    MultiByteToWideChar(CP_ACP, 0, (char *)m_bac_label_vector.at(i).ico_name, (int)strlen((char *)m_bac_label_vector.at(i).ico_name) + 1,
                        FileName.GetBuffer(MAX_PATH), MAX_PATH);
                    FileName.ReleaseBuffer();

                    new_file_path = image_fordor + _T("\\") + FileName;

                    if (temp_find.FindFile(new_file_path) == 0)
                    {
                        MultiByteToWideChar(CP_ACP, 0, (char *)m_screen_data.at(temp_index).picture_file, (int)strlen((char *)m_screen_data.at(temp_index).picture_file) + 1,
                            temp_cstring.GetBuffer(MAX_PATH), MAX_PATH);
                        temp_cstring.ReleaseBuffer();
                    }
                    else
                    {
                        temp_cstring = FileName;
                    }



					icon_full_path = m_building_image_folder + _T("\\") + temp_cstring ;//_T("sample1.bmp");
				}

			}
			else
			{
				icon_full_path = m_building_image_folder + _T("\\")+ temp_cstring ;//_T("sample1.bmp");
				icon_2_full_path = m_building_image_folder + _T("\\")+ temp2_cstring ;//_T("sample1.bmp");
			}

			CFileFind temp_find;
			PointF icon_point;

			int with_length = 0;
			int hight_length = 0;
			cstring_length = cs_show_info.GetLength();
			right_point =  cstring_length * 10;
			botton_point =  30;


			switch(m_bac_label_vector.at(i).ntext_place)
			{
			case LABEL_TEXT_BOTTOM:
				{
					icon_point.Y = pointF.Y - icon_size_y - 2;
					icon_point.X = pointF.X;
				}
				break;
			case LABEL_TEXT_LEFT:
				{
					icon_point.X =  pointF.X + right_point  + 2;
					icon_point.Y = pointF.Y ;

				}
				break;
			case LABEL_TEXT_TOP:
				{
					icon_point.Y = pointF.Y + 30;
					icon_point.X = pointF.X;
				}
				break;
			case LABEL_TEXT_RIGHT:
				{
					icon_point.X =  pointF.X - right_point - 4;
					icon_point.Y = pointF.Y ;
				}
				break;
			default:
				break;
			}
			Graphics graphics_icon(memDC.GetDC());
			if((temp_find.FindFile(icon_full_path)) && (temp_find.FindFile(icon_2_full_path)))
			{
				if(dig_unit_ret == 0)
				{
					Bitmap icon_bitmap(icon_full_path);
					graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
				}
				else if(dig_unit_ret == 1)
				{
					Bitmap icon_bitmap(icon_2_full_path);
					graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
				}

			}
			else if(temp_find.FindFile(icon_full_path))
			{
				Bitmap icon_bitmap(icon_full_path);
				graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
			}
			else if(temp_find.FindFile(icon_2_full_path))
			{
				Bitmap icon_bitmap(icon_2_full_path);
				graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
			}
			else
			{
				if(dig_unit_ret == 0)
				{
					Bitmap icon_bitmap(default_on_icon);
					graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
				}
				else if(dig_unit_ret == 1)
				{
					Bitmap icon_bitmap(default_off_icon);
					graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
				}
				else
				{

					int temp_point_type = m_bac_label_vector.at(i).nPoint_type;
					switch(temp_point_type)
					{
					case BAC_OUT:
						{
						Bitmap icon_bitmap(default_output_icon);				
						graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
						}
						break;
					case BAC_IN:
						{
						Bitmap icon_bitmap(default_input_icon);				
						graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
						}
						break;
					case BAC_VAR:
						{
						Bitmap icon_bitmap(default_variable_icon);				
						graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
						}
						break;
					case BAC_PRG:
						{
						Bitmap icon_bitmap(default_program_icon);				
						graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
						}
						break;
					case BAC_PID:
						{
						Bitmap icon_bitmap(default_pid_icon);				
						graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
						}
						break;
					case BAC_HOL:
						{
						Bitmap icon_bitmap(default_holiday_icon);				
						graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
						}
						break;
					case BAC_SCH:
						{
						Bitmap icon_bitmap(default_schedual_icon);				
						graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
						}
						break;
					case BAC_AMON:
						{
						Bitmap icon_bitmap(default_trendlog_icon);				
						graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
						}
						break;
					case BAC_GRP:
						{
						Bitmap icon_bitmap(default_screen_icon);				
						graphics_icon.DrawImage(&icon_bitmap,(int)icon_point.X ,(int)icon_point.Y,(int)icon_size_x,(int)icon_size_y);
						}
						break;
					default:
						break;
					}

				}

			}
			


		}


		if(screen_lock_label)
		{


			Bitmap icon_bitmap2(lock_icon);
			mygraphics->DrawImage(&icon_bitmap2,0 ,0,LOCK_ICON_SIZE_X,LOCK_ICON_SIZE_Y);
		}
		else
		{
			Bitmap icon_bitmap(unlock_icon);
			mygraphics->DrawImage(&icon_bitmap,0 ,0,LOCK_ICON_SIZE_X,LOCK_ICON_SIZE_Y);
		}

        if (m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_ONLY)
        {
            cs_show_info = _T(" ");
        }
		mygraphics->DrawString(cs_show_info, -1, &unitfont, pointF, &txt_color_brush);
		delete mygraphics;
	}
	
	return;
}


void CBacnetScreenEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	 
	bool click_ret = false;
	for (int i=0;i<m_bac_label_vector.size();i++)
	{
		int rect_x = m_bac_label_vector.at(i).nPoint_x;
		//int rect_x_right = rect_x + 160;
        int rect_x_right = rect_x + m_bac_label_vector.at(i).x_length;
		int rect_y = m_bac_label_vector.at(i).nPoint_y;
		int rect_y_botton = rect_y + 30;

		int x_l_exp_value = 10;
		int x_r_exp_value = 0;
		int y_top_exp_value = 3;
		int y_btn_exp_value = 3;

		if ((m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_VALUE) ||
			(m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_FULL_DESCRIPTION) ||
			(m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_LABEL) ||
			(m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_SHOW_VALUE))
		{
			int niconsize = 0;
			if (m_bac_label_vector.at(i).n_iconsize == LABEL_ICON_SMALL)
			{
				niconsize = 32;
			}
			else if (m_bac_label_vector.at(i).n_iconsize == LABEL_ICON_NORMAL)
			{
				niconsize = 64;
			}
			else if (m_bac_label_vector.at(i).n_iconsize == LABEL_ICON_LARGE)
			{
				niconsize = 96;
			}
			if (m_bac_label_vector.at(i).ntext_place == LABEL_TEXT_BOTTOM)
			{
				y_top_exp_value = niconsize;
			}
			else if (m_bac_label_vector.at(i).ntext_place == LABEL_TEXT_TOP)
			{
				y_btn_exp_value = niconsize;
			}
			else if (m_bac_label_vector.at(i).ntext_place == LABEL_TEXT_LEFT)
			{
				x_r_exp_value = niconsize;
			}
			else if (m_bac_label_vector.at(i).ntext_place == LABEL_TEXT_RIGHT)
			{
				x_l_exp_value = niconsize;
			}
		}



		if((point.x > 0 ) && (point.x < LOCK_ICON_SIZE_X) && (point.y > 0) && (point.y < LOCK_ICON_SIZE_Y))
		{
			if(screen_lock_label)
			{
				WritePrivateProfileStringW(_T("Setting"),_T("LockScreenLabel"),_T("0"),g_cstring_ini_path);
				screen_lock_label = false;
			}
			else
			{
				WritePrivateProfileStringW(_T("Setting"),_T("LockScreenLabel"),_T("1"),g_cstring_ini_path);
				screen_lock_label = true;
			}
			Invalidate(1);
			break;
		}
		rect_x = rect_x - x_l_exp_value;
		rect_x_right = rect_x_right + x_r_exp_value;
		rect_y = rect_y - y_top_exp_value;
		rect_y_botton = rect_y_botton + y_btn_exp_value;

		if((point.x > rect_x ) && (point.x < rect_x_right) && (point.y > rect_y) && (point.y < rect_y_botton) && (screen_lock_label == false))
		{
			m_bac_label_vector.at(i).nMouse_Status = LABEL_MOUSE_ON_LB_DOWN;
			m_bac_select_label = i;
			click_ret = true;
			for (int j=i+1; j<m_bac_label_vector.size();j++)
			{
				m_bac_label_vector.at(j).nMouse_Status = LABEL_MOUSE_NORMAL;
			}
			Invalidate(1);
			break;
		}
		else if(((m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_VALUE) ||
            (m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_ONLY) ||
			(m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_SHOW_VALUE) ||
			(m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_FULL_DESCRIPTION) ||
			(m_bac_label_vector.at(i).nDisplay_Type == LABEL_ICON_LABEL)) /*&& (screen_lock_label == false)*/)
		{
			int temp_index = 0;
			temp_index = m_bac_label_vector.at(i).nPoint_number;
			int delt_x =0;
			int delt_y =0;
			if(m_bac_label_vector.at(i).n_iconsize == LABEL_ICON_SMALL)
			{
				delt_x = delt_y = 32;
			}
			else if(m_bac_label_vector.at(i).n_iconsize == LABEL_ICON_NORMAL)
			{
				delt_x = delt_y = 64;
			}
			else 
			{
				delt_x = delt_y = 96;
			}

			if(m_bac_label_vector.at(i).ntext_place == LABEL_TEXT_BOTTOM)
			{
				rect_x = m_bac_label_vector.at(i).nPoint_x;
				rect_y = m_bac_label_vector.at(i).nPoint_y - delt_y;
				rect_x_right = m_bac_label_vector.at(i).nPoint_x + delt_x;
				rect_y_botton = m_bac_label_vector.at(i).nPoint_y;

				rect_x = rect_x - x_l_exp_value;
				rect_x_right = rect_x_right + x_r_exp_value;
				rect_y = rect_y - y_top_exp_value;
				rect_y_botton = rect_y_botton + y_btn_exp_value;
			}
			else if(m_bac_label_vector.at(i).ntext_place == LABEL_TEXT_RIGHT)
			{
				rect_x = m_bac_label_vector.at(i).nPoint_x - delt_x;
				rect_y = m_bac_label_vector.at(i).nPoint_y ;
				rect_x_right = m_bac_label_vector.at(i).nPoint_x ;
				rect_y_botton = m_bac_label_vector.at(i).nPoint_y + delt_y;

				rect_x = rect_x - x_l_exp_value;
				rect_x_right = rect_x_right + x_r_exp_value;
				rect_y = rect_y - y_top_exp_value;
				rect_y_botton = rect_y_botton + y_btn_exp_value;
			}
			else if(m_bac_label_vector.at(i).ntext_place == LABEL_TEXT_TOP)
			{
				rect_x = m_bac_label_vector.at(i).nPoint_x;
				rect_y = m_bac_label_vector.at(i).nPoint_y  + 30;
				rect_x_right = m_bac_label_vector.at(i).nPoint_x + delt_x;
				rect_y_botton = m_bac_label_vector.at(i).nPoint_y + delt_y;

				rect_x = rect_x - x_l_exp_value;
				rect_x_right = rect_x_right + x_r_exp_value;
				rect_y = rect_y - y_top_exp_value;
				rect_y_botton = rect_y_botton + y_btn_exp_value;
			}

			//for (int j=i+1; j<m_bac_label_vector.size();j++)
			//{
			m_bac_label_vector.at(i).nMouse_Status = LABEL_MOUSE_NORMAL;

			//}
			if((point.x > rect_x ) && (point.x < rect_x_right) && (point.y > rect_y) && (point.y < rect_y_botton))
			{
				m_bac_label_vector.at(i).nMouse_Status = LABEL_MOUSE_ON_LB_DOWN;
				m_bac_select_label = i;
				click_ret = true;


				switch(m_bac_label_vector.at(i).nPoint_type)
				{
				case BAC_IN://INPUT
					{
						if(temp_index < BAC_INPUT_ITEM_COUNT)
						{	
							if(m_Input_data.at(temp_index).auto_manual ==  1)
							{
								if(m_Input_data.at(temp_index).control == 0)
									m_Input_data.at(temp_index).control = 1;
								else
									m_Input_data.at(temp_index).control = 0;

								CString temp_task_info;
								temp_task_info.Format(_T("Write Input List Item%d .Changed "),temp_index);
								Post_Write_Message(g_bac_instance,WRITEINPUT_T3000,temp_index,temp_index,sizeof(Str_in_point),m_screenedit_dlg_hwnd ,temp_task_info);
								m_bac_lbuttondown = false;
								Invalidate(1);
								return;
							}

						}

					}
					break;
				case BAC_OUT://OUTPUT
					{
						if(temp_index < BAC_OUTPUT_ITEM_COUNT)
						{
							if(m_Output_data.at(temp_index).auto_manual ==  1)
							{
								if(m_Output_data.at(temp_index).control == 0)
									m_Output_data.at(temp_index).control = 1;
								else
									m_Output_data.at(temp_index).control = 0;
								CString temp_task_info;
								temp_task_info.Format(_T("Write Output List Item%d .Changed"),temp_index);
								Post_Write_Message(g_bac_instance,WRITEOUTPUT_T3000,temp_index,temp_index,sizeof(Str_out_point),m_screenedit_dlg_hwnd ,temp_task_info);
								m_bac_lbuttondown = false;
								Invalidate(1);
								return;
							}

						}

					}
					break;
				case BAC_VAR://VARIABLE
					{
						if(temp_index < BAC_VARIABLE_ITEM_COUNT)
						{
							if(m_Variable_data.at(temp_index).auto_manual ==  1)
							{
								if(m_Variable_data.at(temp_index).control == 0)
									m_Variable_data.at(temp_index).control = 1;
								else
									m_Variable_data.at(temp_index).control = 0;
								CString temp_task_info;
								temp_task_info.Format(_T("Write Variable List Item%d .Changed "),temp_index);
								Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,temp_index,temp_index,sizeof(Str_variable_point),m_screenedit_dlg_hwnd ,temp_task_info);
								m_bac_lbuttondown = false;
								Invalidate(1);
								return;
							}

						}
					}
					break;
				default:
					break;
				}

				Invalidate(1);

			}


		}
		if((point.x > rect_x ) && (point.x < rect_x_right) && (point.y > rect_y) && (point.y < rect_y_botton) && (screen_lock_label == true))
		{
			if((m_bac_label_vector.at(i).nPoint_type == BAC_GRP) ||
				(m_bac_label_vector.at(i).nPoint_type == BAC_AMON)||
				(m_bac_label_vector.at(i).nPoint_type == BAC_PRG) ||
				(m_bac_label_vector.at(i).nPoint_type == BAC_SCH) ||
				(m_bac_label_vector.at(i).nPoint_type == BAC_HOL))
			{
				click_ret = true;
				m_bac_select_label = i;
				PostMessage(WM_KEYDOWN, VK_RETURN,1); 
				Sleep(1);
				PostMessage(WM_KEYUP, VK_RETURN, 1);    //释放
				break;

			}
			else if((m_bac_label_vector.at(i).nPoint_type == BAC_IN) ||
				(m_bac_label_vector.at(i).nPoint_type == BAC_OUT) ||
				(m_bac_label_vector.at(i).nPoint_type == BAC_VAR) ||
				(m_bac_label_vector.at(i).nPoint_type == BAC_PID))
			{
				//如果是点击的是锁定状态下的 IN OUT VAR  要求能改变值;但是不能改变range.
				click_ret = true;
				m_bac_select_label = i;

				Bacnet_Edit_Label(false);
			}
		}
		else
			m_bac_label_vector.at(i).nMouse_Status = LABEL_MOUSE_NORMAL;

	}
	if(!click_ret)
	{
		m_bac_select_label = -1;	//没有点击上任何label,就取消选中;
	}
	m_bac_lbuttondown = true;

	

	if(click_ret_old != click_ret)
	{
		Invalidate(1);
		click_ret_old = click_ret;
	}

	if(m_bac_select_label == -1)
	{
		SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_CROSS));
	}
	else
	{
		SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_HAND));
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CBacnetScreenEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	 
	if(m_bac_lbuttondown && (m_bac_select_label >=0))
	{
		SaveBacLabel(m_bac_select_label);
		//m_bac_select_label = -1;
	}
	m_bac_lbuttondown = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CBacnetScreenEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	 
	if((m_bac_select_label >=0) && m_bac_lbuttondown)
	{
		if(screen_lock_label)
		{
			CDialogEx::OnMouseMove(nFlags, point);
		}
		else
		{
			if(!m_full_screen_mode)
			{
				if((point.x < m_paint_right_limit) && ((point.y + 30) < m_paint_botton_limit))
				{
					m_bac_label_vector.at(m_bac_select_label).nPoint_x = point.x;
					m_bac_label_vector.at(m_bac_select_label).nPoint_y = point.y;
					Invalidate(0);
				}
			}
			else
			{
				if((point.x < m_cxScreen) && ((point.y + 30) < m_cyScreen))
				{
					m_bac_label_vector.at(m_bac_select_label).nPoint_x = point.x;
					m_bac_label_vector.at(m_bac_select_label).nPoint_y = point.y;
					Invalidate(0);
				}
			}
		}
		
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

bool CBacnetScreenEdit::UpdateDeviceLabelFlash()
{
	int ret_return = 0;

    //离线模式下 重新计算 各个标签 的个数.因为此前 在线模式下是由 T3BB 计算的。
    if (offline_mode)
    {
        for (int i = 0; i < BAC_SCREEN_COUNT; i++)
        {
            m_screen_data.at(m_graphic_label_data.at(i).reg.nScreen_index).update = 0;
        }
        for (int i = 0;i < BAC_GRPHIC_LABEL_COUNT;i++)
        {
            if ((g_selected_serialnumber == m_graphic_label_data.at(i).reg.nSerialNum) &&
                (m_graphic_label_data.at(i).reg.label_status == 1) &&
                (m_graphic_label_data.at(i).reg.nScreen_index < BAC_SCREEN_COUNT))
            {
                m_screen_data.at(m_graphic_label_data.at(i).reg.nScreen_index).update++;
            }
        }
    }


	for (int i=0;i<BAC_GRPHIC_LABEL_GROUP - 1;i++ )
	{
		int cmp_ret = memcmp(&m_temp_graphic_label_data[i*BAC_READ_GRPHIC_LABEL_GROUP_NUMBER],&m_graphic_label_data.at(i*BAC_READ_GRPHIC_LABEL_GROUP_NUMBER),sizeof(Str_label_point) * BAC_READ_GRPHIC_LABEL_GROUP_NUMBER);
		if(cmp_ret!=0)
		{
			ret_return = Write_Private_Data_Blocking(WRITE_GRPHIC_LABEL_COMMAND,i*BAC_READ_GRPHIC_LABEL_GROUP_NUMBER , BAC_READ_GRPHIC_LABEL_GROUP_NUMBER*(i +1)  - 1,control_object_instance);
			if(ret_return < 0)
				return false;
		}

	}
	
	return true;

}

void CBacnetScreenEdit::OnCancel()
{
	
	m_bac_select_label = -1;
	int nsize = screnn_sequence.size();
	if(nsize > 1)
	{
		int temp_screnn_index = screnn_sequence.at(nsize - 2);

		vector <int>::iterator Iter;

		Iter = screnn_sequence.begin() + nsize - 1;
		screnn_sequence.erase(Iter);
		

		screen_list_line = temp_screnn_index;
		InitGraphic(g_serialNum,g_bac_instance,temp_screnn_index);
		return;
	}




	SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR ,(LONG)LoadCursor(NULL , IDC_ARROW));//IDC_ARROW
#if 0
	for (int i=0;i<SCREEN_HOTKEY_COUNT;i++)
	{
		UnregisterHotKey(GetSafeHwnd(), m_screenHotKeyID[i]);   
	}
#endif
	UnregisterHotKey(GetSafeHwnd(), m_screenHotKeyID[0]);   
	UnregisterHotKey(GetSafeHwnd(), m_screenHotKeyID[1]);   
	UnregisterHotKey(GetSafeHwnd(), m_screenHotKeyID[2]);   
	UnregisterHotKey(GetSafeHwnd(), m_screenHotKeyID[3]);		
	UnregisterHotKey(GetSafeHwnd(), m_screenHotKeyID[8]);    
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
	::PostMessage(m_screen_dlg_hwnd,WM_SCREENEDIT_CLOSE,NULL,NULL);
	
	KillTimer(1);
	CDialogEx::OnCancel();
}




void CBacnetScreenEdit::OnTimer(UINT_PTR nIDEvent)
{
	 
	switch(nIDEvent)
	{
	case 1:
		{
			if(this->IsWindowVisible())
			{
				for (int i=0;i<(int)m_graphic_refresh_data.size();i++)
				{
					Post_Refresh_One_Message(m_graphic_refresh_data.at(i).deviceid,
						m_graphic_refresh_data.at(i).command,
						m_graphic_refresh_data.at(i).value_item,
						m_graphic_refresh_data.at(i).value_item,
						m_graphic_refresh_data.at(i).entitysize);
					//m_graphic_refresh_data.at(i).control_pt->Invalidate();
				}
				Invalidate(0);
			}
		}
		break;
	case 2:
		{
			if(this->IsWindowVisible())
			{
				if(m_enable_send_remote_point)
				{
					if(bac_select_device_online)
						Post_Refresh_Message(g_bac_instance,READ_REMOTE_POINT,0,BAC_REMOTE_POINT_COUNT - 1,sizeof(Str_remote_point),BAC_REMOTE_POINT_GROUP);
				}
			}
		}
		break;
	case 3:	//5秒后 设置flag  不要显示没有图片的对话框;
		{
			show_not_exsit_dlg = false;
			KillTimer(3);
			if(screen_show_nopic == 1)
			{
				if(IDYES == MessageBox(_T("No image associated with this graphic screen.Click 'YES' to not see it again"),_T("Message"),MB_YESNOCANCEL))
				{
					WritePrivateProfileStringW(_T("Setting"),_T("ScreenLabelShowNoPic"),_T("0"),g_cstring_ini_path);
					screen_show_nopic = 0;
				}
			}

		}
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);
}






void CBacnetScreenEdit::OnBnClickedButtonScreenExit()
{
	
	PostMessage(WM_CLOSE,NULL,NULL);
}


void CBacnetScreenEdit::OnBnClickedButtonDelete()
{
	
	if(m_nFoucsIndext<0)
	{
		MessageBox(_T("Please select a label first!"),_T("Notice"),MB_OK | MB_ICONINFORMATION);
		return;
	}

	if(AfxMessageBox(_T("Do you really want to delete current label?"),MB_YESNO)==IDYES)
	{
		 
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
		Label_information	label;
		label=m_RelayLabelLst.at(m_nFoucsIndext);

		try
		{
			CString strSql;
			strSql.Format(_T("delete  from Screen_Label where Serial_Num =%i and Tstat_id=%i and  Cstatic_id=%i and Tips='%s'"),label.nSerialNum,label.tstat_id,label.cstatic_id,label.strTips);
			SqliteDBT3000.execDML((UTF8MBSTR)strSql);
		}
		catch(_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());
		}
		SqliteDBT3000.closedb();

		ReloadLabelsFromDB();
		Invalidate();
		m_nFoucsIndext=-1;
	}
}




void CBacnetScreenEdit::SaveBacLabel(int nItem)
{
	if(nItem<0)
		return;
	if(nItem>=(int)m_bac_label_vector.size())
		return;
	::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;
    unsigned short temp_x_value;
    unsigned short temp_y_value;

    //m_bac_label_vector.at(nItem).nPoint_x = m_bac_label_vector.at(nItem).nPoint_x - m_bac_label_vector.at(nItem).nPoint_x % 16;
    //m_bac_label_vector.at(nItem).nPoint_y = m_bac_label_vector.at(nItem).nPoint_y - m_bac_label_vector.at(nItem).nPoint_y % 10;

    temp_x_value = (m_bac_label_vector.at(nItem).nPoint_x * 1000) / m_paint_right_limit ;
    temp_y_value = (m_bac_label_vector.at(nItem).nPoint_y * 1000) / m_paint_botton_limit ;
	m_graphic_label_data.at( m_bac_label_vector.at(nItem).nLabel_index).reg.nPoint_x = temp_x_value;
	m_graphic_label_data.at( m_bac_label_vector.at(nItem).nLabel_index).reg.nPoint_y = temp_y_value;

    Invalidate();
}





void CBacnetScreenEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	 
	if(m_bac_select_label >=0)
	{
		Bacnet_Edit_Label();
	}
	else if(!m_full_screen_mode)
	{
		m_full_screen_mode = true;
		int width = GetSystemMetrics ( SM_CXSCREEN );  
		int height= GetSystemMetrics ( SM_CYSCREEN ); 
		mynew_rect.top = 0;
		mynew_rect.left = 0;
		mynew_rect.right = width;
		mynew_rect.bottom = height;
		MoveWindow(mynew_rect.left,mynew_rect.top,mynew_rect.Width(),mynew_rect.Height(),1);
		InitGraphic(g_serialNum,g_bac_instance,screen_list_line);
	}
	else
	{
		m_full_screen_mode = false;
		::GetWindowRect(BacNet_hwd,&mynew_rect);	//获取 view的窗体大小;
		MoveWindow(mynew_rect.left,mynew_rect.top,mynew_rect.Width(),mynew_rect.Height(),1);
		InitGraphic(g_serialNum,g_bac_instance,screen_list_line);
	}


	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CBacnetScreenEdit::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialogEx::OnClose();
}
