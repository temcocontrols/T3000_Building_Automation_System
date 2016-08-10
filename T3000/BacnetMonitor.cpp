// BacnetMonitor.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetMonitor.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
#include "gloab_define.h"

#include "BacnetGraphic.h"
#include "BacnetWait.h"
#include "BADO/BADO.h"
#define  WM_MONITOR_USER_MESSAGE WM_USER + 902
extern char *ispoint_ex(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network,unsigned char & sub_panel, byte panel , int *netpresent);
extern char *look_label_ex(int panel,int sub_panel, int point_type, int num, int network);
CBacnetGraphic * GraphicWindow = NULL;
extern CString InputLable[15];

// CBacnetMonitor dialog
static int old_monitor_line = -1;
//extern int Station_NUM;
extern BacnetWait *WaitDlg;
HANDLE h_read_monitordata_thread = NULL;
Str_MISC Device_Misc_Data_Old;
unsigned char read_monitor_sd_ret = false;
bool read_temp_local_tem_package = true; //开始点的时候只读最后10包并保存为临时数据;


IMPLEMENT_DYNAMIC(CBacnetMonitor, CDialogEx)
extern char *ispoint(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network, byte panel, int *netpresent);
CBacnetMonitor::CBacnetMonitor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetMonitor::IDD, pParent)
{

}

CBacnetMonitor::~CBacnetMonitor()
{
}

void CBacnetMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MONITOR, m_monitor_list);
	DDX_Control(pDX, IDC_LIST_MONITOR_INPUT, m_monitor_input_list);
	DDX_Control(pDX, IDC_DATETIMEPICKER_MONITOR, m_monitor_time_picker);
}


BEGIN_MESSAGE_MAP(CBacnetMonitor, CDialogEx)
	ON_MESSAGE(WM_HOTKEY,&CBacnetMonitor::OnHotKey)//快捷键消息映射手动加入
	ON_MESSAGE(MY_RESUME_DATA, MonitorMessageCallBack)
	ON_MESSAGE(WM_REFRESH_BAC_MONITOR_LIST,Fresh_Monitor_List)
	ON_MESSAGE(WM_REFRESH_BAC_MONITOR_INPUT_LIST,Fresh_Monitor_Input_List)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_MCallBack_Item)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MONITOR, &CBacnetMonitor::OnNMClickListMonitor)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MONITOR, &CBacnetMonitor::OnNMDblclkListMonitor)
	ON_MESSAGE(WM_LIST_MONITOR_CHANGED,Fresh_Monitor_Item)
	ON_MESSAGE(WM_LIST_MONITOR_INPUT_CHANGED,Fresh_Monitor_Input_Item)
	ON_WM_TIMER()
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATETIMEPICKER_MONITOR, &CBacnetMonitor::OnNMKillfocusDatetimepickerMonitor)
	ON_BN_CLICKED(IDC_BTN_MONITOR_GRAPHIC, &CBacnetMonitor::OnBnClickedBtnMonitorGraphic)
	ON_BN_CLICKED(IDC_BUTTON_MONITOR_DEL_ALL, &CBacnetMonitor::OnBnClickedBtnMonitorDeleteAll)
	ON_BN_CLICKED(IDC_BUTTON_MONITOR_DEL_SEL, &CBacnetMonitor::OnBnClickedBtnMonitorDeleteSelected)
	ON_BN_CLICKED(IDC_BUTTON_MONITOR_DEL_LOCAL, &CBacnetMonitor::OnBnClickedBtnMonitorDeleteLocal)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_MONITOR, &CBacnetMonitor::OnNMSetfocusListMonitor)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST_MONITOR_INPUT, &CBacnetMonitor::OnNMSetfocusListMonitorInput)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_MONITOR_USER_MESSAGE,&CBacnetMonitor::Graphic_Window_Own_Message)


	ON_WM_HELPINFO()
END_MESSAGE_MAP()




LRESULT CBacnetMonitor::Graphic_Window_Own_Message(WPARAM wParam, LPARAM lParam)
{
	int commandtype = wParam ;
	switch(commandtype)
	{
	case MONITOR_MESSAGE_CREATE:
		//break;
		if(GraphicWindow == NULL)
		{
			GraphicWindow = new CBacnetGraphic;
			GraphicWindow->Create(IDD_DIALOG_BACNET_GRAPHIC, this);
			GraphicWindow->ShowWindow(SW_SHOW);
		}
		else
		{
			::PostMessage(GraphicWindow->m_hWnd,WM_SYSCOMMAND,SC_RESTORE,0);
		}
		
		break;
	case MONITOR_MESSAGE_DELETE:

		break;
	default:
		break;
	}

	return 0;
}
// CBacnetMonitor message handlers

LRESULT  CBacnetMonitor::MonitorMessageCallBack(WPARAM wParam, LPARAM lParam)
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
	}
	else
	{
		memcpy_s(&m_monitor_data.at(pInvoke->mRow),sizeof(Str_monitor_point),&m_temp_monitor_data[pInvoke->mRow],sizeof(Str_monitor_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_MONITOR_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		PostMessage(WM_REFRESH_BAC_MONITOR_INPUT_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}
	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
	{
		if(pInvoke->mRow <= BAC_MONITOR_COUNT)
		m_monitor_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
		m_monitor_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	}
	else
	{
		if(pInvoke->mRow <= BAC_MONITOR_COUNT)
		m_monitor_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
		m_monitor_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	}

	m_monitor_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);
	m_monitor_input_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


	if(pInvoke)
		delete pInvoke;
	return 0;
}

BOOL CBacnetMonitor::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	old_monitor_line = -1;
	// TODO:  Add extra initialization here
	Initial_List();
	PostMessage(WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
	PostMessage(WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);

	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);

	SetTimer(1,BAC_LIST_REFRESH_TIME,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetMonitor::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN  )
	{
		if(pMsg->wParam == VK_RETURN)
		{

			CRect list_rect,win_rect;
			CWnd * window_focus =	GetFocus();
			if(window_focus == NULL)
				return CDialogEx::PreTranslateMessage(pMsg);
			if(GetFocus()->GetDlgCtrlID() == IDC_LIST_MONITOR)
			{
				m_monitor_list.GetWindowRect(list_rect);
				ScreenToClient(&list_rect);
				::GetWindowRect(m_monitor_dlg_hwnd,win_rect);
				m_monitor_list.Set_My_WindowRect(win_rect);
				m_monitor_list.Set_My_ListRect(list_rect);
				m_monitor_list.Get_clicked_mouse_position();
				GetDlgItem(IDC_LIST_MONITOR)->SetFocus();
			}
			else if(GetFocus()->GetDlgCtrlID() == IDC_LIST_MONITOR_INPUT)
			{
				m_monitor_input_list.GetWindowRect(list_rect);
				ScreenToClient(&list_rect);
				::GetWindowRect(m_monitor_dlg_hwnd,win_rect);
				m_monitor_input_list.Set_My_WindowRect(win_rect);
				m_monitor_input_list.Set_My_ListRect(list_rect);
				m_monitor_input_list.Get_clicked_mouse_position();

			}
			else if(GetFocus()->GetDlgCtrlID() == IDC_DATETIMEPICKER_MONITOR)
			{
				GetDlgItem(IDC_LIST_MONITOR)->SetFocus();
			}

			return 1;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetMonitor::Initial_List()
{
	m_monitor_list.ShowWindow(SW_HIDE);
	m_monitor_list.DeleteAllItems();
	while ( m_monitor_list.DeleteColumn (0)) ;

	//LVS_EX_FULLROWSELECT
	m_monitor_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_monitor_list.SetExtendedStyle(m_monitor_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_monitor_list.InsertColumn(MONITOR_NUM, _T("NUM"), 60, ListCtrlEx::CheckBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_monitor_list.InsertColumn(MONITOR_LABEL, _T("Label"), 160, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_monitor_list.InsertColumn(MONITOR_INTERVAL, _T("Interval"), 120, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_monitor_list.InsertColumn(MONITOR_STATUS, _T("Status"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_monitor_list.InsertColumn(MONITOR_DATA_SIZE, _T("Data Size (KB)"), 130, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);

	m_monitor_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_monitor_dlg_hwnd;

	m_monitor_list.DeleteAllItems();
	for (int i=0;i<(int)m_monitor_data.size();i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;
		temp_item.Format(_T("%d"),i+1);
		m_monitor_list.InsertItem(i,temp_item);
		m_monitor_list.SetCellEnabled(i,0,0);//禁用num，只是用来显示;

		for (int x=0;x<MONITOR_COL_NUMBER;x++)
		{
			if((i%2)==0)
				m_monitor_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_monitor_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}

	}
	m_monitor_list.SetCellChecked(0,0,1);//default select the first one;
	m_monitor_input_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_monitor_list.SetExtendedStyle(m_monitor_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_monitor_input_list.SetExtendedStyle(m_monitor_input_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_monitor_input_list.InsertColumn(MONITOR_NUM, _T("Num"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_monitor_input_list.InsertColumn(1, _T("Input"), 90, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_monitor_input_list.DeleteAllItems();
	for (int i=0;i<MAX_POINTS_IN_MONITOR;i++)
	{
		CString temp_item;
		temp_item.Format(_T("%d"),i+1);
		m_monitor_input_list.InsertItem(i,temp_item);

		for (int x=0;x<2;x++)
		{
			if((i%2)==0)
				m_monitor_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_monitor_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}
	}
	m_monitor_input_list.InitListData();
	m_monitor_input_list.SetWhetherShowBkCol(false);
	m_monitor_list.ShowWindow(SW_SHOW);
	//m_monitor_list.SetItemTextColor(2,MONITOR_LABEL,RGB(0,255,0));
}
extern char *look_label(int panel, int point_type, int num, int network);
extern int pointtotext(char *buf,Point_Net *point);
LRESULT CBacnetMonitor::Fresh_Monitor_Input_List(WPARAM wParam,LPARAM lParam)
{
	int temp_input_count = 0;
	int temp_analog_count = 0;
	char q[30];
	if(monitor_list_line == BAC_MONITOR_COUNT - 1)
	{
		m_monitor_input_list.SetItemText(0,1,System_Log[0]);
		m_monitor_input_list.SetItemText(1,1,System_Log[1]);
		m_monitor_input_list.SetItemText(2,1,System_Log[2]);
		m_monitor_input_list.SetItemText(3,1,System_Log[3]);
		m_monitor_input_list.SetItemText(4,1,System_Log[4]);
		m_monitor_input_list.SetItemText(5,1,System_Log[5]);
		m_monitor_input_list.SetItemText(6,1,System_Log[6]);
		m_monitor_input_list.SetItemText(7,1,System_Log[7]);
		m_monitor_input_list.SetItemText(8,1,System_Log[8]);

		m_monitor_input_list.SetItemText(9,1,_T(""));
		m_monitor_input_list.SetItemText(10,1,_T(""));
		m_monitor_input_list.SetItemText(11,1,_T(""));
		m_monitor_input_list.SetItemText(12,1,_T(""));
		m_monitor_input_list.SetItemText(13,1,_T(""));
		return 1;
	}
	for (int i=0;i<MAX_POINTS_IN_MONITOR;i++)
	{
		CString temp_des;
		char tempchar[50];
		char *temppoint = NULL;
		byte temp_panel = m_monitor_data.at(monitor_list_line).inputs[i].panel;
		byte temp_point_type = m_monitor_data.at(monitor_list_line).inputs[i].point_type;
		//point type 取的时候减1  ,设备里 out = 1，in = 2 ，var = 3; 取出来是 out = 0; in = 1, var = 2;
		if(temp_point_type > 0)
			temp_point_type = temp_point_type - 1;


		unsigned temp_network = m_monitor_data.at(monitor_list_line).inputs[i].network;
		byte lowbyte_point_type = temp_point_type & 0x1F;	//高3位用于 存放

		if(((temp_panel == 0) || (m_monitor_data.at(monitor_list_line).inputs[i].sub_panel == 0)) ||(lowbyte_point_type > ENUM_AR_DATA))
		{
			m_monitor_data.at(monitor_list_line).inputs[i].network = 0;	//发 现panel 是0  就说明这个数据是无效的,先设置为初始化值;
			m_monitor_data.at(monitor_list_line).inputs[i].number = 0;
			m_monitor_data.at(monitor_list_line).inputs[i].panel = 0;
			m_monitor_data.at(monitor_list_line).inputs[i].point_type = 0;
			m_monitor_data.at(monitor_list_line).inputs[i].sub_panel = 0;
			m_monitor_input_list.SetItemText(i,1,_T(""));
			continue;
		}
		


		Point_Net temp_point;
		temp_point.network = m_monitor_data.at(monitor_list_line).inputs[i].network;
		temp_point.number = m_monitor_data.at(monitor_list_line).inputs[i].number ;//- 1;
		temp_point.panel = m_monitor_data.at(monitor_list_line).inputs[i].panel;

		temp_point.point_type = temp_point_type ;//temp_point_type;
		temp_point.sub_panel = m_monitor_data.at(monitor_list_line).inputs[i].sub_panel;

		memset(q,0,30);
		pointtotext(q,&temp_point);
		temppoint = q;
		char buf[100];
		memset(buf,0,100);
		if((m_monitor_data.at(monitor_list_line).inputs[i].panel == Station_NUM) && (m_monitor_data.at(monitor_list_line).inputs[i].sub_panel == Station_NUM))
		{
			char *temp_label =NULL;
			byte point_type,var_type;
			int num_point,num_panel,num_net,k;
			unsigned char sub_panel = 0;
			int my_network = 0;
			temp_label = ispoint_ex(q,&num_point,&var_type, &point_type, &num_panel, &num_net, my_network,sub_panel, Station_NUM, &k);
			if(temp_label != NULL)
			{
				strcpy(buf,temp_label);
				temppoint= buf;
			}
			else
				temppoint = NULL;
		}

		if(temppoint == NULL)
		{
			m_monitor_input_list.SetItemText(i,1,_T(""));
			continue;
		}

		memset(tempchar,0,50);
		memcpy_s(tempchar,50,temppoint,strlen(temppoint));

		MultiByteToWideChar( CP_ACP, 0, tempchar, (int)strlen(tempchar)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();

		m_monitor_input_list.SetItemText(i,1,temp_des);

	}
	return 0;
}

Str_monitor_point temp_monitor_data_analog;
Str_monitor_point temp_monitor_data_digital;
void CBacnetMonitor::Set_Input_Range_And_count()
{
	int temp_input_count = 0;
	int temp_analog_count = 0;
	int temp_digital_count = 0;
	memset(&temp_monitor_data_analog,0,sizeof(Str_monitor_point));
	memset(&temp_monitor_data_digital,0,sizeof(Str_monitor_point));


	for (int i=0;i<MAX_POINTS_IN_MONITOR;i++)
	{
		byte temp_sub_panel = m_monitor_data.at(monitor_list_line).inputs[i].sub_panel;
		byte temp_panel = m_monitor_data.at(monitor_list_line).inputs[i].panel;
		byte temp_point_type = m_monitor_data.at(monitor_list_line).inputs[i].point_type;
		byte temp_number = 0;
		if(m_monitor_data.at(monitor_list_line).inputs[i].number > 0)
			temp_number =  m_monitor_data.at(monitor_list_line).inputs[i].number ;//- 1;

		unsigned temp_network = m_monitor_data.at(monitor_list_line).inputs[i].network;
		if((temp_panel == 0) || (temp_sub_panel == 0))
			continue;

		//如果不是，就说明是远程的点;
		if((temp_sub_panel!=Station_NUM) || (temp_panel != Station_NUM))
		{
			temp_input_count ++;
			temp_analog_count ++;
			temp_monitor_data_analog.inputs[temp_analog_count - 1].panel =  temp_panel;
			temp_monitor_data_analog.inputs[temp_analog_count - 1].point_type = temp_point_type;
			temp_monitor_data_analog.inputs[temp_analog_count - 1].number = temp_number ;
			temp_monitor_data_analog.inputs[temp_analog_count - 1].network = temp_network;
			temp_monitor_data_analog.inputs[temp_analog_count - 1].sub_panel = temp_sub_panel;
			temp_monitor_data_analog.range[temp_analog_count - 1] = 0;
			continue;
		}

		if( temp_point_type == ENUM_OUT + 1)
		{
			temp_input_count ++;
			if(m_Output_data.at(temp_number).digital_analog == 1)
			{
				temp_analog_count ++;

				temp_monitor_data_analog.inputs[temp_analog_count - 1].panel =  temp_panel;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].point_type = temp_point_type;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].number = temp_number ;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].network = temp_network;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].sub_panel = temp_sub_panel;
			}
			else
			{
				temp_digital_count ++;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].panel =  temp_panel;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].point_type = temp_point_type;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].number = temp_number ;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].network = temp_network;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].sub_panel = temp_sub_panel;
			}
				temp_monitor_data_analog.range[temp_analog_count - 1] = m_Output_data.at(temp_number).range;
		}
		else if(temp_point_type == ENUM_IN + 1)
		{
			temp_input_count++;
			if(m_Input_data.at(temp_number).digital_analog == 1)
			{
				temp_analog_count ++;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].panel =  temp_panel;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].point_type = temp_point_type;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].number = temp_number ;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].network = temp_network;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].sub_panel = temp_sub_panel;
			}
			else
			{
				temp_digital_count ++;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].panel =  temp_panel;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].point_type = temp_point_type;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].number = temp_number ;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].network = temp_network;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].sub_panel = temp_sub_panel;
			}
			temp_monitor_data_analog.range[temp_analog_count - 1] = m_Input_data.at(temp_number).range;
		}
		else if(temp_point_type == ENUM_VAR + 1)
		{
			temp_input_count++;
			if(m_Variable_data.at(temp_number).digital_analog == 1)
			{
				temp_analog_count ++;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].panel =  temp_panel;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].point_type = temp_point_type;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].number = temp_number ;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].network = temp_network;
				temp_monitor_data_analog.inputs[temp_analog_count - 1].sub_panel = temp_sub_panel;
			}
			else
			{
				temp_digital_count ++;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].panel =  temp_panel;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].point_type = temp_point_type;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].number = temp_number ;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].network = temp_network;
				temp_monitor_data_digital.inputs[temp_digital_count - 1].sub_panel = temp_sub_panel;
			}
			temp_monitor_data_analog.range[temp_analog_count - 1] = m_Variable_data.at(temp_number).range;
		}

	}
	int test1 = sizeof(Point_Net);
	memset(m_monitor_data.at(monitor_list_line).inputs,0,MAX_POINTS_IN_MONITOR * sizeof(Point_Net));
	memcpy_s(m_monitor_data.at(monitor_list_line).inputs,temp_analog_count*sizeof(Point_Net),temp_monitor_data_analog.inputs,temp_analog_count*sizeof(Point_Net));
	memcpy_s(&m_monitor_data.at(monitor_list_line).inputs[temp_analog_count]  ,temp_digital_count * sizeof(Point_Net),&temp_monitor_data_digital.inputs[0],temp_digital_count * sizeof(Point_Net));

	return ;
}


LRESULT CBacnetMonitor::Fresh_Monitor_Input_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	CString temp_task_info;
	CString New_CString =  m_monitor_input_list.GetItemText(Changed_Item,Changed_SubItem);

	char *label=NULL/*,test*/;
	byte point_type,var_type;
	int k,num_net,num_point,num_panel;
	var_type = num_point = point_type = 0;
	k=0;
	memcpy_s(&m_temp_monitor_data[monitor_list_line],sizeof(Str_monitor_point),&m_monitor_data.at(monitor_list_line),sizeof(Str_monitor_point));
	New_CString.MakeUpper();
	New_CString.Trim();
	if(New_CString.IsEmpty())
	{
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].network = 1;//目前不知道network 怎么处理;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].number = 0;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].panel = 0;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].point_type = 0;
		Set_Input_Range_And_count();	//update the monitor structure analog and totol input number;
		int cmp_ret = memcmp(&m_temp_monitor_data[monitor_list_line],&m_monitor_data.at(monitor_list_line),sizeof(Str_monitor_point));
		if(cmp_ret!=0)
		{
			m_monitor_input_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
			temp_task_info.Format(_T("Write Monitor List Item%d .Changed to empty "),monitor_list_line + 1);
			Post_Write_Message(g_bac_instance,WRITEMONITOR_T3000,monitor_list_line,monitor_list_line,sizeof(Str_monitor_point),m_monitor_dlg_hwnd,temp_task_info,Changed_Item,Changed_SubItem);
		}
		PostMessage(WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
		return 0;
	}

	char cTemp1[255];
	memset(cTemp1,0,255);
	WideCharToMultiByte( CP_ACP, 0, New_CString, -1, cTemp1, 255, NULL, NULL );

	//目前只支持在本Panel下面寻找 各个Label 和值;
	//label=ispoint(cTemp1,&num_point,&var_type,&point_type,&num_panel,&num_net,0/*my_network*/,Station_NUM,&k);
	unsigned char sub_panel_number = 0;
	 label=ispoint_ex(cTemp1,&num_point,&var_type,&point_type,&num_panel,&num_net,0,sub_panel_number,Station_NUM,&k);
	if(label!=NULL)
	{
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].network = 1;//目前不知道network 怎么处理;
		if(num_point > 0)
			num_point = num_point - 1;
		//point type 存的时候加1  ,原始 out = 0，in = 1 ，var = 2; 存进去是 out = 1; in = 2, var = 3;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].number = num_point;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].panel = num_panel;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].sub_panel = sub_panel_number;
		m_monitor_data.at(monitor_list_line).inputs[Changed_Item].point_type = point_type + 1;
		char temp_input[250];
		memset(temp_input,0,250);
		strcpy(temp_input,label);

		CString temp_monitor_input;
		temp_monitor_input.Empty();
		MultiByteToWideChar( CP_ACP, 0, temp_input, (int)strlen(temp_input)+1, 
			temp_monitor_input.GetBuffer(MAX_PATH), MAX_PATH );
		temp_monitor_input.ReleaseBuffer();
		


		Set_Input_Range_And_count();	//update the monitor structure analog and totol input number;
		m_monitor_input_list.SetItemText(Changed_Item,1,temp_monitor_input);

		for (int i=0 ; i< MAX_POINTS_IN_MONITOR;i++)
		{
			if((m_temp_monitor_data[monitor_list_line].inputs[i].panel != m_temp_monitor_data[monitor_list_line].inputs[i].sub_panel) ||
				(m_temp_monitor_data[monitor_list_line].inputs[i].panel != Station_NUM))
			{
				m_temp_monitor_data[monitor_list_line].range[i] = 0;
				continue;
			}
			switch(m_temp_monitor_data[monitor_list_line].inputs[i].point_type)
			{
			case BAC_VAR + 1://Variable
				{
					if(m_temp_monitor_data[monitor_list_line].inputs[i].number < BAC_VARIABLE_ITEM_COUNT)
						m_monitor_data[monitor_list_line].range[i] = m_Variable_data.at(m_temp_monitor_data[monitor_list_line].inputs[i].number).range;
				}
				break;
			case BAC_IN + 1://Input
				{
					if(m_temp_monitor_data[monitor_list_line].inputs[i].number < BAC_INPUT_ITEM_COUNT)
						m_monitor_data[monitor_list_line].range[i] = m_Input_data.at(m_temp_monitor_data[monitor_list_line].inputs[i].number).range;
				}
				break;
			case BAC_OUT + 1://Output
				{
					if(m_temp_monitor_data[monitor_list_line].inputs[i].number < BAC_OUTPUT_ITEM_COUNT)
						m_monitor_data[monitor_list_line].range[i] = m_Output_data.at(m_temp_monitor_data[monitor_list_line].inputs[i].number).range;
				}
				break;
			default:
				m_monitor_data[monitor_list_line].range[i] = 0;
				break;
			}
		}

		//这里应该是 monitor_list_line，就是要改变的多少项 而不是右边的input label的项目;
		int cmp_ret = memcmp(&m_temp_monitor_data[monitor_list_line],&m_monitor_data.at(monitor_list_line),sizeof(Str_monitor_point));


		if(cmp_ret!=0)
		{
			m_monitor_input_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Monitor List Item%d .Changed to \"%s\" "),monitor_list_line + 1,temp_monitor_input);
		Post_Write_Message(g_bac_instance,WRITEMONITOR_T3000,monitor_list_line,monitor_list_line,sizeof(Str_monitor_point),m_monitor_dlg_hwnd,temp_task_info,Changed_Item,Changed_SubItem);
		}
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Error!Can't find the point!"));
	}
	PostMessage(WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
	return 0;
}
LRESULT CBacnetMonitor::Fresh_Monitor_Item(WPARAM wParam,LPARAM lParam)
{
	int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	CString temp_task_info;
	CString New_CString =  m_monitor_list.GetItemText(Changed_Item,Changed_SubItem);
	//先保存 原来的值，等结束的时候来比对，看是否有改变，有改变就进行写动作;
	memcpy_s(&m_temp_monitor_data[Changed_Item],sizeof(Str_monitor_point),&m_monitor_data.at(Changed_Item),sizeof(Str_monitor_point));



	if(Changed_SubItem == MONITOR_LABEL)
	{
		if(New_CString.GetLength()>= STR_MONITOR_LABEL_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not greater than 8"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
			return 0;
		}
		New_CString.MakeUpper();
		if(Check_Label_Exsit(New_CString))
		{
			PostMessage(WM_REFRESH_BAC_MONITOR_LIST,Changed_Item,REFRESH_ON_ITEM);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, New_CString, -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_monitor_data.at(Changed_Item).label,STR_IN_LABEL,cTemp1,STR_IN_LABEL);
	}


	cmp_ret = memcmp(&m_temp_monitor_data[Changed_Item],&m_monitor_data.at(Changed_Item),sizeof(Str_monitor_point));
	if(cmp_ret!=0)
	{
		m_monitor_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Monitor List Item%d .Changed to \"%s\" "),Changed_Item + 1,New_CString);
		Post_Write_Message(g_bac_instance,WRITEMONITOR_T3000,Changed_Item,Changed_Item,sizeof(Str_monitor_point),m_monitor_dlg_hwnd,temp_task_info,Changed_Item,Changed_SubItem);
	}
	return 0;
}

LRESULT CBacnetMonitor::Fresh_Monitor_List(WPARAM wParam,LPARAM lParam)
{
	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if(isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		bool pack_data_new = false ;
		bool monitor_list_data_new = false;
		if(memcmp(&Device_Misc_Data_Old,&Device_Misc_Data,sizeof(Str_MISC)) != 0 )
		{
			memcpy_s(&Device_Misc_Data_Old,sizeof(Str_MISC),&Device_Misc_Data,sizeof(Str_MISC));
			pack_data_new = true;
		}
		if(m_monitor_list.IsDataNewer((char *)&m_monitor_data.at(0),sizeof(Str_monitor_point) * BAC_MONITOR_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			monitor_list_data_new = true;
			m_monitor_list.SetListData((char *)&m_monitor_data.at(0),sizeof(Str_monitor_point) * BAC_MONITOR_COUNT);
		}

		if((monitor_list_data_new == false) && (pack_data_new == false))
		{
			return 0 ;
		}
	}

	for (int i=0;i<(int)m_monitor_data.size();i++)
	{
		CString temp_des;
		CString temp_time;
		CString temp_length;
		CString temp_units;
		CString temp_analog_package;
		CString temp_digital_package;
		CString temp_data_szie;
		char temp_char[50];

		if(isFreshOne)
		{
			i = Fresh_Item;
		}

		MultiByteToWideChar( CP_ACP, 0, (char *)m_monitor_data.at(i).label, (int)strlen((char *)m_monitor_data.at(i).label)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_monitor_list.SetItemText(i,MONITOR_LABEL,temp_des);

		int temp_interval_time;
		temp_interval_time = m_monitor_data.at(i).hour_interval_time * 60 * 60 +
							 m_monitor_data.at(i).minute_interval_time * 60 +
							 m_monitor_data.at(i).second_interval_time;
		intervaltotextfull(temp_char,temp_interval_time,0,0);
		MultiByteToWideChar( CP_ACP, 0, temp_char, (int)strlen(temp_char)+1, 
			temp_time.GetBuffer(MAX_PATH), MAX_PATH );
		temp_time.ReleaseBuffer();
		m_monitor_list.SetItemText(i,MONITOR_INTERVAL,temp_time);


		if(m_monitor_data.at(i).status == 1)
		{
			m_monitor_list.SetItemText(i,MONITOR_STATUS,_T("ON"));
			m_monitor_list.SetItemTextColor(i,MONITOR_STATUS,RGB(0,0,255),false);
		}
		else if(m_monitor_data.at(i).status == 0)
		{
			m_monitor_list.SetItemText(i,MONITOR_STATUS,_T("OFF"));
			m_monitor_list.SetItemTextColor(i,MONITOR_STATUS,RGB(255,0,0),false);
		}






		if((Device_Misc_Data.reg.flag[0] != 0x55) || (Device_Misc_Data.reg.flag[1] != 0xff))
		{
			temp_analog_package = _T("0");
			temp_digital_package = _T("0");
			temp_data_szie =  _T("0");
		}
		else
		{
			temp_analog_package.Format(_T("%u"),Device_Misc_Data.reg.monitor_analog_block_num[i]);
			temp_digital_package.Format(_T("%u"),Device_Misc_Data.reg.monitor_digital_block_num[i]);

			temp_data_szie.Format(_T("%.1f"),((float)((unsigned long)Device_Misc_Data.reg.monitor_analog_block_num[i] + (unsigned long)Device_Misc_Data.reg.monitor_digital_block_num[i]))*400/1000);
		}

		m_monitor_list.SetItemText(i,MONITOR_DATA_SIZE,temp_data_szie);

		if(isFreshOne)
		{
			break;
		}

	}
	return 0;
}
//对于一个对话框里面有多个List 只能通过确认焦点 来发送Changed 消息;
LRESULT CBacnetMonitor::Fresh_MCallBack_Item(WPARAM wParam,LPARAM lParam)
{
	//if (!::IsWindow(m_hWnd))
	//{
	//	return 0;
	//}
	CWnd * window_focus =	GetFocus();
	if(window_focus == NULL)
		return 0;
	if(GetFocus()->GetDlgCtrlID() == IDC_LIST_MONITOR)
	{
		PostMessage(WM_LIST_MONITOR_CHANGED,wParam,lParam);

	}
	else if(GetFocus()->GetDlgCtrlID() == IDC_LIST_MONITOR_INPUT)
	{
		PostMessage(WM_LIST_MONITOR_INPUT_CHANGED,wParam,lParam);

	}

	return 0;
}


void CBacnetMonitor::OnNMDblclkListMonitor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	OnBnClickedBtnMonitorGraphic();
	*pResult = 0;
}



void CBacnetMonitor::OnNMClickListMonitor(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString temp_task_info;
	// TODO: Add your control notification handler code here
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_monitor_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	m_monitor_list.Set_Edit(true);
	int nItem=m_monitor_list.SubItemHitTest(&lvinfo);
	if(nItem!=-1)
	{
		m_monitor_list.SetCellChecked(nItem,0,1); 
		monitor_list_line = nItem;	//记录点击的是第几项;
		if(old_monitor_line != monitor_list_line) //避免反复刷新右边的 INPUT list;
		{
			PostMessage(WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
			old_monitor_line = monitor_list_line;
		}
		
		for (int i=0;i<m_monitor_list.GetItemCount();++i)
		{
			if(i == nItem)
				continue;
			m_monitor_list.SetCellChecked(i,0,FALSE);
		}
	}
	long lRow,lCol;
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;
	if(lRow>m_monitor_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;


	if(lRow == BAC_MONITOR_COUNT - 1)
	{
		m_monitor_list.Set_Edit(false);
		m_monitor_input_list.Set_Edit(false);
		SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Can't edit the last monitor , it is for trend log system value."));
		return;
	}
	else
	{
		m_monitor_input_list.Set_Edit(true);
	}

	memcpy_s(&m_temp_monitor_data[lRow],sizeof(Str_monitor_point),&m_monitor_data.at(lRow),sizeof(Str_monitor_point));

	if(lCol == MONITOR_STATUS)
	{
		m_monitor_list.Set_Edit(false);
		if(m_monitor_data.at(lRow).status == 0)
		{
			m_monitor_list.SetItemTextColor(lRow,MONITOR_STATUS,RGB(0,0,255),false);
			m_monitor_data.at(lRow).status = 1;
			m_monitor_list.SetItemText(lRow,MONITOR_STATUS,_T("ON"));
			temp_task_info.Format(_T("Write Monitor List Item%d .Changed to \"ON\" "),lRow + 1);
		}
		else
		{
			m_monitor_list.SetItemTextColor(lRow,MONITOR_STATUS,RGB(255,0,0),false);
			m_monitor_data.at(lRow).status = 0;
			m_monitor_list.SetItemText(lRow,MONITOR_STATUS,_T("OFF"));
			temp_task_info.Format(_T("Write Monitor List Item%d .Changed to \"OFF\" "),lRow + 1);
		}
		int cmp_ret = memcmp(&m_temp_monitor_data[lRow],&m_monitor_data.at(lRow),sizeof(Str_monitor_point));
		if(cmp_ret!=0)
		{
			m_monitor_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
			Post_Write_Message(g_bac_instance,WRITEMONITOR_T3000,lRow,lRow,sizeof(Str_monitor_point),m_monitor_dlg_hwnd,temp_task_info,lRow,lCol);
		}
	}
	else if(lCol == MONITOR_INTERVAL)
	{
		m_row = lRow;
		m_col = lCol;

		m_monitor_list.Set_Edit(false);
		m_monitor_time_picker.ShowWindow(SW_SHOW);
		CRect list_rect,win_rect;
		m_monitor_list.GetWindowRect(list_rect);
		GetWindowRect(win_rect);
		CRect myrect;
		m_monitor_list.GetSubItemRect(lRow,lCol,LVIR_BOUNDS,myrect);


		myrect.left = myrect.left + list_rect.left - win_rect.left + 1;
		myrect.right = myrect.right + list_rect.left - win_rect.left + 1;
		myrect.top = myrect.top + 10;
		myrect.bottom = myrect.bottom + 12;

		myrect.top = myrect.top + 20;
		myrect.bottom = myrect.bottom + 20;
		m_monitor_time_picker.BringWindowToTop();
		m_monitor_time_picker.MoveWindow(myrect);


		CString Temp_CString =  m_monitor_list.GetItemText(lRow,lCol);

		CStringArray TEMPCS;
		int temp_hour,temp_minute,temp_second;
		SplitCStringA(TEMPCS, Temp_CString, _T(":"));

		if((int)TEMPCS.GetSize() <3)
		{
			temp_hour = 0;
			temp_minute = 0;
			temp_second = 0;
			CTime TimeTemp(2013,1,1,temp_hour,temp_minute,0);
			m_monitor_time_picker.SetFormat(_T("HH:mm"));
			m_monitor_time_picker.SetTime(&TimeTemp);
			m_monitor_time_picker.SetFocus();
		}
		else
		{
			temp_hour = _wtoi(TEMPCS.GetAt(0));
			temp_minute = _wtoi(TEMPCS.GetAt(1));
			temp_second = _wtoi(TEMPCS.GetAt(2));
			if(temp_hour >=24)
				temp_hour = 0;
			if(temp_minute >=60)
				temp_minute = 0;
			if(temp_second >=60)
				temp_second = 15;
			CTime TimeTemp(2013,1,1,temp_hour,temp_minute,temp_second);
			m_monitor_time_picker.SetFormat(_T("HH:mm:ss"));
			m_monitor_time_picker.SetTime(&TimeTemp);
			m_monitor_time_picker.SetFocus();
		}
		m_monitor_list.SetItemText(lRow,lCol,_T(""));
		m_monitor_time_picker.Invalidate();
		SetTimer(2,100,NULL);
	}


	
	*pResult = 0;
}


void CBacnetMonitor::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:

		if(this->IsWindowVisible() && (bac_select_device_online))
		{
		PostMessage(WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
		PostMessage(WM_REFRESH_BAC_MONITOR_INPUT_LIST,NULL,NULL);
		Post_Refresh_Message(g_bac_instance,READMONITOR_T3000,0,BAC_MONITOR_COUNT - 1,sizeof(Str_monitor_point),BAC_MONITOR_GROUP);
		Post_Refresh_Message(g_bac_instance,READ_MISC,0,0,sizeof(Str_MISC),1);
		}
		break;
	case 2:
		KillTimer(2);
		m_monitor_time_picker.Invalidate();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CBacnetMonitor::OnNMKillfocusDatetimepickerMonitor(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	memcpy_s(&m_temp_monitor_data[m_row],sizeof(Str_monitor_point),&m_monitor_data.at(m_row),sizeof(Str_monitor_point));

	CTime temp_time;CString temp_cs;
	int chour,cmin,csecond;
	m_monitor_time_picker.GetTime(temp_time);
	chour = temp_time.GetHour();
	cmin = temp_time.GetMinute();
	csecond = temp_time.GetSecond();


	if((chour == 0) &&(cmin	==0) &&(csecond == 0))
	{
		temp_cs.Empty();
	}
	else
		temp_cs.Format(_T("%02d:%02d:%02d"),chour,cmin,csecond);
	m_monitor_list.SetItemText(m_row,m_col,temp_cs);

	m_monitor_time_picker.ShowWindow(SW_HIDE);

	m_monitor_data.at(m_row).hour_interval_time = chour;
	m_monitor_data.at(m_row).minute_interval_time = cmin;
	m_monitor_data.at(m_row).second_interval_time = csecond;


	CString temp_task_info;

	int cmp_ret = memcmp(&m_temp_monitor_data[m_row],&m_monitor_data.at(m_row),sizeof(Str_monitor_point));
	if(cmp_ret!=0)
	{
		m_monitor_list.SetItemBkColor(m_row,m_col,LIST_ITEM_CHANGED_BKCOLOR);
	temp_task_info.Format(_T("Write Monitor Interval Time Item%d .Changed Time to \"%s\" "),m_row + 1,temp_cs);
	Post_Write_Message(g_bac_instance,WRITEMONITOR_T3000,m_row,m_row,sizeof(Str_monitor_point),m_monitor_dlg_hwnd,temp_task_info,m_row,m_col);
	}

}

void CBacnetMonitor::Check_New_DB()
{
	CTime tm;
	tm=CTime::GetCurrentTime();//获取系统日期
	int month_of_day = 0;
	month_of_day = tm.GetDay();
	//CString temp_cs_week;
	//temp_cs_week.Format(_T("%u"),(month_of_day  /7) + 1);
	CString cs_serial_number;
	cs_serial_number.Format(_T("\\%u"),g_selected_serialnumber);

	CString temp_folder;
	temp_folder= g_achive_folder_temp_db + cs_serial_number;
	int ret = FALSE;
	WIN32_FIND_DATA fd;
	HANDLE hFind_folder;
	hFind_folder = FindFirstFile(temp_folder, &fd);
	if ((hFind_folder != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		//目录存在
		ret = TRUE;

	}
	FindClose(hFind_folder);
	if(ret == false)
	{
		SECURITY_ATTRIBUTES attrib;
		attrib.bInheritHandle = FALSE;
		attrib.lpSecurityDescriptor = NULL;
		attrib.nLength = sizeof(SECURITY_ATTRIBUTES);

		CreateDirectory( temp_folder, &attrib);
	}



	CString mdb_name;
	mdb_name.Format(_T("\\%u_%u_%u_monitor%u.mdb"),g_selected_serialnumber,tm.GetYear(),tm.GetMonth(),monitor_list_line);
	g_achive_monitor_datatbase_path = temp_folder + mdb_name;


	CString FilePath_Monitor;
	HANDLE hFind_Monitor;//
	WIN32_FIND_DATA wfd_monitor;//
	hFind_Monitor = FindFirstFile(g_achive_monitor_datatbase_path, &wfd_monitor);//
	if (hFind_Monitor==INVALID_HANDLE_VALUE)//说明当前目录下无MonitorData.mdb
	{
		//没有找到就创建一个默认的数据库
		FilePath_Monitor= g_achive_monitor_datatbase_path;
		HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_MONITOR_DB1), _T("MONITOR_DB"));   
		HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   
		LPVOID lpExe = LockResource(hGlobal);   
		CFile file;
		if(file.Open(FilePath_Monitor, CFile::modeCreate | CFile::modeWrite))    
			file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
		file.Close();    
		::UnlockResource(hGlobal);   
		::FreeResource(hGlobal);
	}  //
	FindClose(hFind_Monitor);//
}

void CBacnetMonitor::OnBnClickedBtnMonitorGraphic()
{
	// TODO: Add your control notification handler code here
	Check_New_DB();
	for (int i=0;i<14;i++)
	{
		InputLable[i] =	m_monitor_input_list.GetItemText(i,1);	
	}
	if(WaitDlg==NULL)
	{
		WaitDlg = new BacnetWait((int)BAC_WAIT_READ_MONITOR_DATA);

		WaitDlg->Create(IDD_DIALOG_BACNET_WAIT,this);
		WaitDlg->ShowWindow(SW_SHOW);

		RECT RECT_SET1;
		::GetClientRect(BacNet_hwd,&RECT_SET1);
		ClientToScreen(&RECT_SET1);
		WaitDlg->MoveWindow(RECT_SET1.left + 50,RECT_SET1.bottom - 30,800,20);
	}

	if(h_read_monitordata_thread==NULL)
	{
		int ret_misc = GetPrivateData_Blocking(g_bac_instance,READ_MISC,0,0,sizeof(Str_MISC));
		if(ret_misc < 0)
		{
			MessageBox(_T("Read data failed! Please try again!"));
			return;
		}
		CString temp_serial;
		temp_serial.Format(_T("%u"),g_selected_serialnumber);

		CString temp_cs_modify_index;
		temp_cs_modify_index.Format(_T("Monitor_%d"),monitor_list_line);

		CString temp_db_ini_folder;
		temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");

		int temp_time_num;
		temp_time_num = GetPrivateProfileInt(temp_serial,temp_cs_modify_index,0,temp_db_ini_folder);

		if(temp_time_num < Device_Misc_Data.reg.operation_time[monitor_list_line])
		{
			//需要更新数据库;
			//if(IDYES == MessageBox(_T("Trend log data saved in SD disk has changed , Do you want synchronization."),_T("Notice"),MB_YESNOCANCEL | MB_ICONINFORMATION))
			//{
				CString temp_operation_time;
				CString temp_monitor_index;
				CString temp_monitor_digital_index;
				temp_monitor_index.Format(_T("Index_%d"),monitor_list_line);
				temp_monitor_digital_index.Format(_T("Digital_Index_%d"),monitor_list_line);
				temp_operation_time.Format(_T("%u"),Device_Misc_Data.reg.operation_time[monitor_list_line]);
				WritePrivateProfileString(temp_serial,temp_cs_modify_index,temp_operation_time,temp_db_ini_folder);
				WritePrivateProfileString(temp_serial,temp_monitor_index,NULL,g_cstring_ini_path);
				WritePrivateProfileString(temp_serial,temp_monitor_digital_index,NULL,g_cstring_ini_path);
			//}
		}






		CString strSql;
		CBADO monitor_bado;
		monitor_bado.SetDBPath(g_achive_monitor_datatbase_path);	//删除里面的临时数据;
		monitor_bado.OnInitADOConn(); 
		strSql.Format(_T("delete * from MonitorData where Flag=1"),g_selected_serialnumber,monitor_list_line);
		monitor_bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	
		monitor_bado.CloseConn();

		//WritePrivateProfileString(_T("Setting"),_T("MonitorValueIgnoreMax"),_T("10000000"),g_cstring_ini_path);

		h_read_monitordata_thread =CreateThread(NULL,NULL,Readmonitorthreadfun,this,NULL, NULL);
	}

}

void CBacnetMonitor::OnBnClickedBtnMonitorDeleteAll()
{
	memset(monitor_database_flag ,1,24);
	if(IDYES != MessageBox(_T("Do you want to delete all device monitor data which saved in SD card?"),_T("Warning"),MB_YESNO))
		return;
	if(Write_Private_Data_Blocking(DELETE_MONITOR_DATABASE,0,0) > 0)
	{
		CString temp_serial;
		CString temp_monitor_index;
		CString temp_monitor_digital_index;
		temp_serial.Format(_T("%u"),g_selected_serialnumber);
		WritePrivateProfileString(temp_serial,NULL,NULL,g_cstring_ini_path);
		//for (int i=0;i<12;i++)
		//{
		//	temp_monitor_index.Format(_T("Index_%d"),i);
		//	temp_monitor_digital_index.Format(_T("Digital_Index_%d"),i);

		//	temp_serial.Format(_T("%u"),g_selected_serialnumber);

		//	WritePrivateProfileString(temp_serial,temp_monitor_index,NULL,g_cstring_ini_path);
		//	WritePrivateProfileString(temp_serial,temp_monitor_digital_index,NULL,g_cstring_ini_path);
		//}
		CString strSql;
		CBADO monitor_bado;
		monitor_bado.SetDBPath(g_achive_monitor_datatbase_path);	//暂时不创建新数据库
		monitor_bado.OnInitADOConn(); 
		strSql=_T("delete * from MonitorData");
		monitor_bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	
		monitor_bado.CloseConn();

		for (int z=0;z<12;z++)
		{
			Device_Misc_Data.reg.monitor_analog_block_num[z] = 0;
			Device_Misc_Data.reg.monitor_digital_block_num[z] = 0;
		}
		PostMessage(WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
		MessageBox(_T("Delete Monitor Data : OK !"));
	}
	else
	{
		MessageBox(_T("Delete Monitor Data : Timeout !"));
	}
}

//IDC_BUTTON_MONITOR_DEL_LOCAL
void CBacnetMonitor::OnBnClickedBtnMonitorDeleteLocal()
{
	CString temp_cs;
	temp_cs.Format(_T("Do you want delete local data saved in T3000?"),monitor_list_line + 1);
	if(IDYES != MessageBox(temp_cs,_T("Warning"),MB_YESNO))
		return;

	CString temp_serial;
	temp_serial.Format(_T("%u"),g_selected_serialnumber);
	WritePrivateProfileString(temp_serial,NULL,NULL,g_cstring_ini_path);



	CString strSql;
	CBADO monitor_bado;
	monitor_bado.SetDBPath(g_achive_monitor_datatbase_path);	//暂时不创建新数据库
	monitor_bado.OnInitADOConn(); 
	strSql=_T("delete * from MonitorData");
	monitor_bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	
	monitor_bado.CloseConn();
	MessageBox(_T("Delete Monitor Data : OK !"));
}
void CBacnetMonitor::OnBnClickedBtnMonitorDeleteSelected()
{
//monitor_list_line
	CString temp_cs;
	temp_cs.Format(_T("Do you want to delete monitor %d data which saved in SD card?"),monitor_list_line + 1);
	if(IDYES != MessageBox(temp_cs,_T("Warning"),MB_YESNO))
		return;

	int size_flag = sizeof(monitor_database_flag)/sizeof(monitor_database_flag[0])	;
	if(monitor_list_line < (size_flag/2) )
	{
		memset(monitor_database_flag ,0,24);
		monitor_database_flag[2* monitor_list_line] = 1;
		monitor_database_flag[2 *monitor_list_line + 1] = 1;

		if(Write_Private_Data_Blocking(DELETE_MONITOR_DATABASE,0,0) > 0)
		{
			CString temp_serial;
			CString temp_monitor_index;
			CString temp_monitor_digital_index;
			temp_monitor_index.Format(_T("Index_%d"),monitor_list_line);
			temp_monitor_digital_index.Format(_T("Digital_Index_%d"),monitor_list_line);

			temp_serial.Format(_T("%u"),g_selected_serialnumber);

			CString strSql;
			CBADO monitor_bado;
			monitor_bado.SetDBPath(g_achive_monitor_datatbase_path);	//暂时不创建新数据库
			monitor_bado.OnInitADOConn(); 
			strSql=_T("delete * from MonitorData");
			monitor_bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	
			monitor_bado.CloseConn();


			WritePrivateProfileString(temp_serial,temp_monitor_index,NULL,g_cstring_ini_path);
			WritePrivateProfileString(temp_serial,temp_monitor_digital_index,NULL,g_cstring_ini_path);



			CString temp_cs_modify_index;
			temp_cs_modify_index.Format(_T("Monitor_%d"),monitor_list_line);

			//每次读完Monitor的 值 记录下读写的时间,和Minipanel 比对 ,若minipanel 有进行删减的动作
			CString temp_db_ini_folder;
			temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");

			CTime temp_start = CTime::GetCurrentTime();
			unsigned long end_long_time = temp_start.GetTime();
			CString temp_time_num;
			temp_time_num.Format(_T("%u"),end_long_time);
			WritePrivateProfileStringW(temp_serial,temp_cs_modify_index,temp_time_num,temp_db_ini_folder);
			//下面还要加写的动作;
			Device_Misc_Data.reg.operation_time[monitor_list_line] = end_long_time;


			Device_Misc_Data.reg.monitor_analog_block_num[monitor_list_line] = 0;
			Device_Misc_Data.reg.monitor_digital_block_num[monitor_list_line] = 0;

			if(Write_Private_Data_Blocking(WRITE_MISC,0,0) > 0 )
			{
				PostMessage(WM_REFRESH_BAC_MONITOR_LIST,NULL,NULL);
				MessageBox(_T("Delete Monitor Data : OK !"));
			}
			else
			{
				MessageBox(_T("Delete Monitor Data : Timeout !"));
			}
			
		}
		else
		{
			MessageBox(_T("Delete Monitor Data : Timeout !"));
		}
	}
}




unsigned char read_monitordata(int digtal_or_analog)
{
	timestart = time(NULL);
	MonitorUpdateData temp;
	memset(&temp,0,sizeof(MonitorUpdateData));
	//temp.nsize = 10000;
	//timesec1970 = (unsigned long)time(NULL);
	//temp.oldest_time =timesec1970 - 43200;
	//temp.most_recent_time = timestart + 43200;


	//unsigned long nTime = temp.oldest_time;   //nTime类型为unsigned long
	//unsigned long nTime1 = temp.most_recent_time;  //nTime类型为unsigned long
	//CTime objTime(nTime);   //CTime类型变量直接获取时间值
	//CTime objTime1(nTime1);   //CTime类型变量直接获取时间值
	////string strTime;    //格式化string变量
	////strTime.Format("d-d-d d:d:d", objTime.GetYear(), objTime.GetMonth(), objTime.GetDay(),objTime.GetHour(), objTime.GetMinute(), objTime.GetSecond());

	//g_Print.Format(_T("Read Start Time %d-%d-%d %d:%d:%d   %d"), objTime.GetYear(), objTime.GetMonth(), objTime.GetDay(),objTime.GetHour(), objTime.GetMinute(), objTime.GetSecond(),temp.oldest_time);
	//DFTrace(g_Print);
	//g_Print.Format(_T("Read End Time %d-%d-%d %d:%d:%d   %d"), objTime1.GetYear(), objTime1.GetMonth(), objTime1.GetDay(),objTime1.GetHour(), objTime1.GetMinute(), objTime1.GetSecond(),temp.most_recent_time);
	//DFTrace(g_Print);
	int ret = -2;
	int send_count = 0;
	//int time_count = 0;//用于动态计时只要有回复继续下一包;.
	int recieve_flag = 0;
	CString cs_my_temp;
	cs_my_temp = _T("");
	//if(WaitDlg)
	//{
	//	WaitDlg->Set_Show_String(cs_my_temp);
	//	WaitDlg->Set_Pos(0);
	//}
	//else
	//	return false;

	m_monitor_head.total_seg = 0;
	m_monitor_head.seg_index = 0;
	for (int i=0;i<5;i++)
	{
		recieve_flag = false;
		ret = GetMonitorBlockData(g_bac_instance,READMONITORDATA_T3000,monitor_list_line,digtal_or_analog,0,0,&temp);	
		if(ret < 0)
		{
			Sleep(SEND_COMMAND_DELAY_TIME);
			continue;
		}
		for(int time_loop=0;time_loop<2000;time_loop++)
		{
			Sleep(1);
			if(tsm_invoke_id_free(ret))
			{
				recieve_flag = true;
				break;
			}
		} 
		if(recieve_flag)
		{
			break;
		}
		else if((recieve_flag == false) &&( i == 5))
		{
			g_progress_persent = 0;
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read monitor data timeout!"));
			return MONITOR_READ_TIMEOUT;
		}
	}

	CString temp_db_ini_folder;
	temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");
	CString temp_serial;
	CString temp_monitor_index;
	if(digtal_or_analog == BAC_UNITS_ANALOG)
		temp_monitor_index.Format(_T("Index_%d"),monitor_list_line);
	else
		temp_monitor_index.Format(_T("Digital_Index_%d"),monitor_list_line);

	temp_serial.Format(_T("%u"),g_selected_serialnumber);
	int temp_index = GetPrivateProfileInt(temp_serial,temp_monitor_index,-1,temp_db_ini_folder) + 1;

	if(m_monitor_head.total_seg == 0)
		return MONITOR_READ_NO_DATA;

	if(temp_index > m_monitor_head.total_seg)
		return true;

	int temp_value = 0;
	if(read_temp_local_tem_package)
	{
		if((m_monitor_head.total_seg > TEMP_DATA_READ_COUNT) && (temp_index < m_monitor_head.total_seg - TEMP_DATA_READ_COUNT))
			temp_value = m_monitor_head.total_seg - TEMP_DATA_READ_COUNT;
		else
			temp_value = temp_index;
	}
	else
	{
		temp_value = temp_index;
	}



	for (int read_index= temp_value ;read_index<=m_monitor_head.total_seg;read_index++)
	{
		if(Device_Basic_Setting.reg.sd_exist != 2)
		{
			if(m_monitor_head.total_seg >= 2)
			{
			   break;
			}
		}

		cs_my_temp.Format(_T("Read Data %d / %d"),read_index - temp_index,m_monitor_head.total_seg - temp_index);
		SetPaneString(BAC_SHOW_MISSION_RESULTS,cs_my_temp);

		//WaitDlg->Set_Show_String(cs_my_temp);
		int pos = 100;
		if(m_monitor_head.total_seg != temp_index)
		 pos = ((read_index - temp_index) * 100 ) / (m_monitor_head.total_seg - temp_index);
		if(pos > 100)
			pos = 100;

		g_progress_persent = pos;
		//WaitDlg->Set_Pos(pos);
		for (int i=0;i<5;i++)
		{
			recieve_flag = false;
			CString temp_cs1;
			temp_cs1.Format(_T("Read index = %x , Digital_Analog = %d"),read_index,digtal_or_analog);
			//DFTrace(temp_cs1);
			ret = GetMonitorBlockData(g_bac_instance,READMONITORDATA_T3000,monitor_list_line,digtal_or_analog,m_monitor_head.total_seg,read_index,&temp);	
			if(ret < 0)
			{
				Sleep(SEND_COMMAND_DELAY_TIME);
				continue;
			}
			for(int time_loop=0;time_loop<1500;time_loop++)
			{
				Sleep(1);
				if(tsm_invoke_id_free(ret))
				{
					recieve_flag = true;
					break;
				}
			} 
			if(recieve_flag)
			{
				break;
			}
			else if((recieve_flag == false) &&( i == 5))
			{
				g_progress_persent = 0;
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read monitor data timeout!"));
				return MONITOR_READ_TIMEOUT;
			}
		}
		if(recieve_flag)
		{
			continue;
		}
		else
		{
			g_progress_persent = 0;
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read monitor data timeout!"));
			return false;
		}

	}
	SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Read monitor data : OK!"));
	g_progress_persent = 100;
	return true;
}




DWORD WINAPI  CBacnetMonitor::Readmonitorthreadfun(LPVOID lpVoid)
{
	//Write_Config_Info
	CBacnetMonitor *pParent = (CBacnetMonitor *)lpVoid;
	read_monitor_sd_ret = false;
	unsigned char analog_ret = false;
	unsigned char digital_ret = false;
	read_temp_local_tem_package = true;
	analog_ret = read_monitordata(BAC_UNITS_ANALOG) ;

	

	digital_ret =  read_monitordata(BAC_UNITS_DIGITAL) ;


	if((analog_ret == MONITOR_READ_TIMEOUT) || (digital_ret == MONITOR_READ_TIMEOUT))
	{
		read_monitor_sd_ret = MONITOR_READ_TIMEOUT;
	}
	else if((analog_ret == MONITOR_READ_NO_DATA) && (digital_ret == MONITOR_READ_NO_DATA))
	{
		read_monitor_sd_ret = MONITOR_READ_NO_DATA;
	}
	else
	{
		read_monitor_sd_ret = MONITOR_READ_SUCCESS;
	}
	read_temp_local_tem_package = false;

	pParent->PostMessage(WM_MONITOR_USER_MESSAGE,MONITOR_MESSAGE_CREATE,0);


readendthread:
	
	if(WaitDlg)
	{
		WaitDlg->Set_Pos(100);
		WaitDlg = NULL;
	}
	h_read_monitordata_thread = NULL;
	return 0;


}





void CBacnetMonitor::OnNMSetfocusListMonitor(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	//Fance
	//两个list 要只有一个 的select 处于显示状态;
	int my_raw=0;
	int my_col=0;
	m_monitor_input_list.Get_Selected_Item(my_raw,my_col);

	m_monitor_list.SetWhetherShowBkCol(true);
	m_monitor_input_list.SetWhetherShowBkCol(false);

	if((my_raw%2)==0)
		m_monitor_input_list.SetItemBkColor(my_raw,my_col,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_monitor_input_list.SetItemBkColor(my_raw,my_col,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);	
	m_monitor_input_list.RedrawItems(my_raw,my_raw);

	m_monitor_list.Get_Selected_Item(my_raw,my_col);//重新在获取那个是select
	m_monitor_list.SetItemBkColor(my_raw,my_col,LIST_ITEM_SELECTED,0);
	m_monitor_list.RedrawItems(my_raw,my_raw);



	*pResult = 0;
}


void CBacnetMonitor::OnNMSetfocusListMonitorInput(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here

	int my_raw=0;
	int my_col=0;
	m_monitor_list.SetWhetherShowBkCol(false);
	m_monitor_input_list.SetWhetherShowBkCol(true);
	m_monitor_list.Get_Selected_Item(my_raw,my_col);
	TRACE(_T("Kill focus monitor\r\n"));
	if((my_raw%2)==0)
		m_monitor_list.SetItemBkColor(my_raw,my_col,LIST_ITEM_DEFAULT_BKCOLOR,1);
	else
		m_monitor_list.SetItemBkColor(my_raw,my_col,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,1);	

	m_monitor_list.RedrawItems(my_raw,my_raw);

	m_monitor_input_list.Get_Selected_Item(my_raw,my_col);//重新在获取那个是select
	m_monitor_input_list.SetItemBkColor(my_raw,my_col,LIST_ITEM_SELECTED,0);
	m_monitor_input_list.RedrawItems(my_raw,my_raw);


	*pResult = 0;
}

void CBacnetMonitor::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(1);
	m_monitor_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
	CDialogEx::OnClose();
}

void CBacnetMonitor::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
//	KillTimer(1);
//	m_monitor_dlg_hwnd = NULL;
	::PostMessage(BacNet_hwd,WM_DELETE_NEW_MESSAGE_DLG,DELETE_WINDOW_MSG,0);
//	CDialogEx::OnCancel();
}

void CBacnetMonitor::Reg_Hotkey()
{
	RegisterHotKey(GetSafeHwnd(),KEY_INSERT,NULL,VK_INSERT);//Insert键
}

void CBacnetMonitor::Unreg_Hotkey()
{
	UnregisterHotKey(GetSafeHwnd(),KEY_INSERT);
}

//This function code by Fance du for receive the monitor data;
int handle_read_monitordata_ex(char *npoint,int nlength)
{
	char * my_temp_point = npoint;
	char * temp_print = my_temp_point;


	temp_print = my_temp_point;

	CString n_temp_print;
	n_temp_print.Format(_T("Monitor Rx (%03d): "),nlength);
	CString temp_char;





	int temp_flag = 0;
	int temp_sd_exsit = 0;
	
	//如果不存在SD卡 就认为接到的都是临时数据;麻痹后面临时数据也要存起来，当宝似的;导致Access 数据库超过50万笔数据检索都要10秒;
	if(Device_Basic_Setting.reg.sd_exist == 2)
		temp_sd_exsit = 1;
	else
		temp_sd_exsit = 0;

	bool analog_data = true;//1为Analog;


	m_monitor_head.total_seg =  (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
	my_temp_point = my_temp_point + 2;
	m_monitor_head.command = *(my_temp_point++);
	m_monitor_head.index = *(my_temp_point++);
	m_monitor_head.type = *(my_temp_point++);
	m_monitor_head.conm_args.nsize = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
	my_temp_point = my_temp_point + 4;
	m_monitor_head.conm_args.oldest_time = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
	my_temp_point = my_temp_point + 4;
	m_monitor_head.conm_args.most_recent_time = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
	my_temp_point = my_temp_point + 4;
	m_monitor_head.special = *(my_temp_point++);
	
	//m_monitor_head.total_seg = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
	//my_temp_point = my_temp_point + 2;
	m_monitor_head.seg_index = (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0]; 
	my_temp_point = my_temp_point + 2;

	//在调试界面中打印出接收到得字符;
	if(((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_MONITOR_DATA_ONLY)) /*&& (m_monitor_head.special == 0)*/)
	{
		for (int i = 0; i< nlength ; i++)
		{
			temp_char.Format(_T("%02x"),(unsigned char)*temp_print);
			temp_char.MakeUpper();
			temp_print ++;
			n_temp_print = n_temp_print + temp_char + _T(" ");
			if(i==19)
				n_temp_print = n_temp_print + _T("---");
		}
		DFTrace(n_temp_print);
	}

	if(nlength!= 420)	//每包必发420个字节;
		return 0;
	if(m_monitor_head.type == BAC_UNITS_ANALOG)
		analog_data = true;
	else if(m_monitor_head.type == BAC_UNITS_DIGITAL)
		analog_data = false;
	else
		return 0;

	CString temp_db_ini_folder;
	temp_db_ini_folder = g_achive_folder + _T("\\MonitorIndex.ini");
	CString temp_serial;
	CString temp_monitor_index;
	if(analog_data)
		temp_monitor_index.Format(_T("Index_%d"),monitor_list_line);
	else
		temp_monitor_index.Format(_T("Digital_Index_%d"),monitor_list_line);
	temp_serial.Format(_T("%u"),g_selected_serialnumber);
	int temp_index = GetPrivateProfileInt(temp_serial,temp_monitor_index,-1,temp_db_ini_folder);
	if(temp_index == -1)
	{
		WritePrivateProfileStringW(temp_serial,temp_monitor_index,_T("0"),temp_db_ini_folder);
	}
	else if((temp_sd_exsit == 0) && (temp_index > 0)) //如果 sd 卡还不存在;
	{
		WritePrivateProfileString(temp_serial,temp_monitor_index,NULL,temp_db_ini_folder);
	}


	if((temp_index >= m_monitor_head.seg_index) && (temp_index != 0))
		return 1;
	if(temp_index >= m_monitor_head.total_seg)	//如果数据库里面的 index 已经比设备里面的 多，就说明已经读过了;不需要再往数据库里面存了;
		return 1;

	CBADO monitor_bado;
	monitor_bado.SetDBPath(g_achive_monitor_datatbase_path);	//暂时不创建新数据库
	monitor_bado.OnInitADOConn(); 
	int loop_count = 400/(sizeof(Str_mon_element));
	for (int i=0;i<loop_count;i++)
	{
		Str_mon_element temp_data;
		temp_data.index = *(my_temp_point++); 
		temp_data.time = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);

		//{

		//}
		my_temp_point = my_temp_point + 4;
		temp_data.point.number = *(my_temp_point++); 
		temp_data.point.point_type = *(my_temp_point++); 

		temp_data.point.panel = *(my_temp_point++); 
		temp_data.point.sub_panel = *(my_temp_point++); 
		temp_data.point.network = 1;my_temp_point ++; //*(my_temp_point++);  目前不考虑 多个net 的情况 ;
		
		temp_data.value = ((unsigned char)my_temp_point[0])<<24 | ((unsigned char)my_temp_point[1]<<16) | ((unsigned char)my_temp_point[2])<<8 | ((unsigned char)my_temp_point[3]);
		my_temp_point = my_temp_point + 4;

		temp_data.mark =  (unsigned char)my_temp_point[1]<<8 | (unsigned char)my_temp_point[0];
		my_temp_point = my_temp_point + 2;
		if(monitor_list_line != temp_data.index)
		{
			if(((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_MONITOR_DATA_ONLY)))
			{
				if(i!= (loop_count -1))
				{
					n_temp_print.Format(_T("monitor_list_line != temp_data.index"));
					DFTrace(n_temp_print);
				}

			}
			continue;
		}
		if(temp_data.mark != 0x0A0D)	//0d0a
			break;
		if((temp_data.time == 0) ) //说明后面是无用的数据;填充的是0
		{
			if(((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_MONITOR_DATA_ONLY)))
			{
				if(i!= (loop_count -1))
				{
				n_temp_print.Format(_T("temp_data.time = 0"));
				DFTrace(n_temp_print);
				}
			}
			break;
		}
		if(temp_data.point.point_type > 10)
		{
			if(((debug_item_show == DEBUG_SHOW_ALL) || (debug_item_show == DEBUG_SHOW_MONITOR_DATA_ONLY)))
			{
				if(i!= (loop_count -1))
				{
				n_temp_print.Format(_T("temp_data.point.point_type > 10"));
				DFTrace(n_temp_print);
				}
			}
			break;
		}

		if(temp_data.point.panel != Station_NUM)	//如果数据point 不是本panel 估计就是传错了值;
			continue;
		if((temp_data.time < 1420041600)  || (temp_data.time > 1735660800))	//时间范围 2015-1-1  ->2049-12-30  ，不在此时间的数据无效;
		{
			continue;
		}



		CString Label_Des;
		CString temp_type;
		temp_type.Format(_T("%u_%u_%u_%u_%u"),temp_data.point.number,temp_data.point.point_type,temp_data.point.panel,temp_data.point.sub_panel,temp_data.point.network);
		CString temp_type_word;
		if(temp_data.point.point_type == 1)
		{
			temp_type_word = _T("OUT");
		}
		else if(temp_data.point.point_type == 2)
		{
			temp_type_word = _T("IN");
		}
		else if(temp_data.point.point_type == 3)
		{
			if((temp_data.point.panel == temp_data.point.sub_panel) && (temp_data.point.panel == Station_NUM))
			temp_type_word = _T("VAR");
			else
			temp_type_word = _T("REG");
		}
		else
		{
			temp_type_word = _T("NA");
		}
		if((temp_data.point.panel == temp_data.point.sub_panel) && (temp_data.point.panel == Station_NUM))
		{
			Label_Des.Format(_T("%s%u"),temp_type_word,temp_data.point.number+1);
		}
		else
			Label_Des.Format(_T("%u-%u-%s%u"),temp_data.point.panel,temp_data.point.sub_panel,temp_type_word,temp_data.point.number+1);


		CString temp_value_test;
		temp_value_test.Format(_T("%d"),temp_data.value);
			
		if(m_monitor_head.special == 1)
		{
			temp_flag = 1;
		}
		else
		{

				temp_flag = 0;

				//CString strTime;
				//time_t scale_time2 ;
				//CTime time_scaletime1;
				//scale_time2 = temp_data.time ;
				//time_scaletime1 = scale_time2;
				//strTime = time_scaletime1.Format("  %m/%d %H:%M:%S  ");
				//strTime = strTime + temp_value_test;
				//CString ret_ret_ret;
				//ret_ret_ret.Format(_T("%s  %d\r\n"),strTime,temp_flag);

				//TRACE(ret_ret_ret);
		}




		CString display_time;

		CTime time_scaletime;
		time_t scale_time  = temp_data.time;
		time_scaletime = scale_time;
		display_time.Empty();
		display_time=time_scaletime.Format("%y-%m-%d %H:%M:%S");

		//COleDateTime updateTime;
		//updateTime = scale_time;
		//updateTime.Format(_T("%Y-%m-%d %H:%M:%S"));

		CString strSql;
		strSql.Format(_T("insert into MonitorData values('%s',#%s#,%u,%d,%u,%u,'%s')"),temp_type,display_time,temp_data.time,temp_data.value,  analog_data ,temp_flag,Label_Des);
		//strSql.Format(_T("insert into MonitorData values('%s',%d,%u,%u,%u,'%s','%s')"),temp_type,temp_data.value,temp_data.time , analog_data ,temp_flag,display_time,Label_Des);
		monitor_bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	

		

	}
	bool delete_temp_db_data = false;

	if((m_monitor_head.special != 1) && (m_monitor_head.seg_index != 0))
	{
		CString temp_write_index;
		temp_write_index.Format(_T("%d"),m_monitor_head.seg_index);
		if((temp_index + 1) == m_monitor_head.seg_index )
		{
			if(read_temp_local_tem_package == false)
			{
				if(temp_sd_exsit)
				{
					if(m_monitor_head.seg_index == 65535)
						WritePrivateProfileStringW(temp_serial,temp_monitor_index,_T("0"),temp_db_ini_folder);//如果panel 存了快两个字节的包,就要清零;
					else
						WritePrivateProfileStringW(temp_serial,temp_monitor_index,temp_write_index,temp_db_ini_folder);
				}
			}
		}
		else if(m_monitor_head.seg_index == 1)
		{
			if(temp_sd_exsit)
				WritePrivateProfileStringW(temp_serial,temp_monitor_index,temp_write_index,temp_db_ini_folder);
		}
		delete_temp_db_data = true;
		//DFTrace(temp_write_index);
	}


	monitor_bado.CloseConn();

	if(delete_temp_db_data)
	{
		CString strSql;
		CBADO monitor_bado;
		monitor_bado.SetDBPath(g_achive_monitor_datatbase_path);	//暂时不创建新数据库 
		monitor_bado.OnInitADOConn(); 
		if(analog_data)
			strSql.Format(_T("delete * from MonitorData where Flag=1 and Analog_Digital=1"));
		else
			strSql.Format(_T("delete * from MonitorData where Flag=1 and Analog_Digital=0"));
		monitor_bado.m_pConnection->Execute(strSql.GetString(),NULL,adCmdText);	
		monitor_bado.CloseConn();

	}



	return 1;

}

int GetAmonLabel(int index,CString &ret_label)
{
	if(index >= BAC_MONITOR_COUNT)
	{
		ret_label.Empty();
		return -1;
	}
	int i = index;
	CString temp_des2;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_monitor_data.at(i).label, (int)strlen((char *)m_monitor_data.at(i).label)+1, 
		ret_label.GetBuffer(MAX_PATH), MAX_PATH );
	ret_label.ReleaseBuffer();

	return 1;
}


// CBacnetProgramEdit message handlers
LRESULT CBacnetMonitor::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if (wParam==KEY_INSERT)
	{
		OnBnClickedBtnMonitorGraphic();
	}
	return 0;
}

BOOL CBacnetMonitor::OnHelpInfo(HELPINFO* pHelpInfo)
{ 

	//if (g_protocol==PROTOCOL_BACNET_IP){
		HWND hWnd;

		if(pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle,theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd =  ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile,HH_HELP_CONTEXT, IDH_TOPIC_6_9_TREND_LOGS);
		return (hWnd != NULL);
// 	}
// 	else{
// 		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
// 	}

	return CDialogEx::OnHelpInfo(pHelpInfo);
}