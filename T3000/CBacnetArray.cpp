// CBacnetArray.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CBacnetArray.h"
#include "afxdialogex.h"
#include "global_function.h"

// CBacnetArray 对话框

IMPLEMENT_DYNAMIC(CBacnetArray, CDialogEx)

CBacnetArray::CBacnetArray(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BACNET_ARRAY, pParent)
{
	window_max = true;
}

CBacnetArray::~CBacnetArray()
{
}
//WM_REFRESH_BAC_ARRAY_LIST
void CBacnetArray::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ARRAY, m_array_list);
}


BEGIN_MESSAGE_MAP(CBacnetArray, CDialogEx)
	ON_WM_SIZE()
	ON_MESSAGE(WM_LIST_ITEM_CHANGED, Fresh_Array_Item)
	ON_MESSAGE(WM_REFRESH_BAC_ARRAY_LIST, Fresh_Array_List)
	ON_MESSAGE(MY_RESUME_DATA, ArrayMessageCallBack)
END_MESSAGE_MAP()


// CBacnetArray 消息处理程序


BOOL CBacnetArray::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW(_T("Array"));
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEFAULT_ARRAY);
	SetIcon(m_hIcon, TRUE);
	// TODO:  在此添加额外的初始化
	Initial_List();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CBacnetArray::Initial_List()
{
	m_array_list.ShowWindow(SW_HIDE);
	m_array_list.DeleteAllItems();
	while (m_array_list.DeleteColumn(0));

	m_array_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	//m_array_list.SetExtendedStyle(m_array_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_array_list.SetExtendedStyle(m_array_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_array_list.InsertColumn(0, _T("Item"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_array_list.InsertColumn(1, _T("Array Name"), 150, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_array_list.InsertColumn(2, _T("Length"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_array_list.InsertColumn(3, _T("Value"), 200, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_array_list.Setlistcolcharlimit(1, 9);
	m_array_dlg_hwnd = this->m_hWnd;
	m_array_list.SetListHwnd(this->m_hWnd);
	CRect list_rect, win_rect;
	m_array_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_pragram_dlg_hwnd, win_rect);
	m_array_list.Set_My_WindowRect(win_rect);
	m_array_list.Set_My_ListRect(list_rect);

	m_array_list.DeleteAllItems();

	for (int i = 0; i < (int)m_Array_data.size(); i++)
	{
		CString temp_item, temp_value, temp_cal, temp_filter, temp_status, temp_lable;
		CString temp_des;
		CString temp_units;

		if (i > BAC_ARRAY_ITEM_COUNT)
			break;


		temp_item.Format(_T("%d"), i + 1);
		m_array_list.InsertItem(i, temp_item);



		for (int x = 0; x < 3; x++)
		{
			if ((i % 2) == 0)
				m_array_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_array_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
		}

	}

	m_array_list.InitListData();
	m_array_list.ShowWindow(SW_SHOW);
}


LRESULT CBacnetArray::Fresh_Array_List(WPARAM wParam, LPARAM lParam)
{
	int Fresh_Item;
	int isFreshOne = (int)lParam;
	if (isFreshOne == REFRESH_ON_ITEM)
	{
		Fresh_Item = (int)wParam;
	}
	else
	{
		if (m_array_list.IsDataNewer((char*)&m_Array_data.at(0), sizeof(Str_array_point) * BAC_ARRAY_ITEM_COUNT))
		{
			//避免list 刷新时闪烁;在没有数据变动的情况下不刷新List;
			m_array_list.SetListData((char*)&m_Array_data.at(0), sizeof(Str_array_point) * BAC_ARRAY_ITEM_COUNT);
		}
		else
		{
			return 0;
		}
	}
	for (int i = 0; i < (int)m_Array_data.size(); i++)
	{


		if (i >= BAC_ARRAY_ITEM_COUNT)
			break;

		if (isFreshOne)
		{
			i = Fresh_Item;
		}
		CString temp_label;
		CString temp_value;
		MultiByteToWideChar(CP_ACP, 0, (char*)m_Array_data.at(i).label, (int)strlen((char*)m_Array_data.at(i).label) + 1,
			temp_label.GetBuffer(MAX_PATH), MAX_PATH);
		temp_label.ReleaseBuffer();
		temp_label = temp_label.Left(STR_ARRAY_NAME_LENGTH).Trim();

		m_array_list.SetItemText(i, 1, temp_label);


		temp_value.Format(_T("%d"), (unsigned short)m_Array_data.at(i).array_size);
		m_array_list.SetItemText(i, 2, temp_value);

		if (isFreshOne)
		{
			break;
		}

	}

	//copy_data_to_ptrpanel(TYPE_PROGRAM);
#ifdef LOCAL_DB_FUNCTION
	//if (selected_product_Node.serial_number != 0)
	//	WriteDeviceDataIntoAccessDB(BAC_PRG, program_item_limit_count, selected_product_Node.serial_number);
#endif

	return 0;
}



LRESULT CBacnetArray::Fresh_Array_Item(WPARAM wParam, LPARAM lParam)
{
	int cmp_ret;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	memcpy_s(&m_temp_array_data[Changed_Item], sizeof(Str_array_point), &m_Array_data.at(Changed_Item), sizeof(Str_array_point));


	CString temp_task_info;
	CString New_CString = m_array_list.GetItemText(Changed_Item, Changed_SubItem);


	if (Changed_SubItem == ARRAY_LABLE)
	{
		CString cs_temp = m_array_list.GetItemText(Changed_Item, Changed_SubItem);
		if (cs_temp.GetLength() > STR_ARRAY_NAME_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not higher than 9"), _T("Warning"), MB_OK | MB_ICONINFORMATION);
			PostMessage(WM_REFRESH_BAC_ARRAY_LIST, NULL, NULL);
			return 0;
		}
		cs_temp.MakeUpper();
		//if (Check_Label_Exsit(cs_temp))
		//{
		//	PostMessage(WM_REFRESH_BAC_PROGRAM_LIST, Changed_Item, REFRESH_ON_ITEM);
		//	return 0;
		//}

		char cTemp1[255];
		memset(cTemp1, 0, 255);
		WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
		memcpy_s(m_Array_data.at(Changed_Item).label, STR_ARRAY_NAME_LENGTH+1, cTemp1, STR_ARRAY_NAME_LENGTH+1);
	}
	if (Changed_SubItem == ARRAY_SIZE)
	{
		CString cs_temp = m_array_list.GetItemText(Changed_Item, Changed_SubItem);
		int  temp2 = _wtoi(cs_temp);
		if ((temp2 < 0) || (temp2 > 65535))
		{
			MessageBox(_T("Please Input an value between 0 - 65535"), _T("Warning"), MB_OK);
			PostMessage(WM_REFRESH_BAC_ARRAY_LIST, NULL, NULL);//这里调用 刷新线程重新刷新会方便一点;
			return 0;
		}
		m_Array_data.at(Changed_Item).array_size = (unsigned short)temp2;
	}
	cmp_ret = memcmp(&m_temp_array_data[Changed_Item], &m_Array_data.at(Changed_Item), sizeof(Str_array_point));
	if (cmp_ret != 0)
	{
		m_array_list.SetItemBkColor(Changed_Item, Changed_SubItem, LIST_ITEM_CHANGED_BKCOLOR);
		temp_task_info.Format(_T("Write Array Item%d .Changed to \"%s\" "), Changed_Item + 1, New_CString);
		Post_Write_Message(g_bac_instance, WRITEARRAY_T3000, Changed_Item, Changed_Item, sizeof(Str_array_point), m_array_dlg_hwnd, temp_task_info, Changed_Item, Changed_SubItem);
	}
	return 0;
}


void CBacnetArray::Reset_Array_Rect()
{

	CRect temp_mynew_rect;
	::GetWindowRect(BacNet_hwd, &temp_mynew_rect);	//获取 view的窗体大小;

	CRect temp_window;
	GetWindowRect(&temp_window);

	if (window_max)
	{
		CRect temp_mynew_rect;
		::GetWindowRect(BacNet_hwd, &temp_mynew_rect);	//获取 view的窗体大小;
		::SetWindowPos(this->m_hWnd, NULL, temp_mynew_rect.left, temp_mynew_rect.top, temp_mynew_rect.Width(), temp_mynew_rect.Height() - DELTA_HEIGHT, NULL);
	}
	else if ((temp_window.Width() <= temp_mynew_rect.Width()) && (temp_window.Height() <= temp_mynew_rect.Height()))
	{
		::SetWindowPos(this->m_hWnd, NULL, temp_mynew_rect.left, temp_mynew_rect.top, 0, 0, SWP_NOSIZE);
	}
	else
		::SetWindowPos(this->m_hWnd, NULL, temp_mynew_rect.left + 60, temp_mynew_rect.top + 60, 700, 700, NULL);


	return;

}


BOOL CBacnetArray::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		CRect list_rect, win_rect;
		m_array_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_array_dlg_hwnd, win_rect);
		m_array_list.Set_My_WindowRect(win_rect);
		m_array_list.Set_My_ListRect(list_rect);

		m_array_list.Get_clicked_mouse_position();

		CWnd* temp_focus = GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
		GetDlgItem(IDC_BUTTON_VARIABLE_READ)->SetFocus();
		temp_focus->SetFocus();
		return 1;
	}
	else if (pMsg->message == WM_NCLBUTTONDBLCLK)
	{
		if (!window_max)
		{
			window_max = true;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd, &temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd, NULL, temp_mynew_rect.left, temp_mynew_rect.top, temp_mynew_rect.Width(), temp_mynew_rect.Height(), SWP_SHOWWINDOW);
		}
		else
		{
			window_max = false;
			CRect temp_mynew_rect;
			::GetWindowRect(BacNet_hwd, &temp_mynew_rect);	//获取 view的窗体大小;
			::SetWindowPos(this->m_hWnd, NULL, temp_mynew_rect.left + 60, temp_mynew_rect.top + 60, 500, 700, SWP_SHOWWINDOW);
		}


		return 1;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetArray::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rc;
	GetClientRect(rc);
	if (m_array_list.m_hWnd != NULL)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		m_array_list.MoveWindow(rc.left, rc.top, rc.Width(), rc.Height() - 80);
	}
}


LRESULT  CBacnetArray::ArrayMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo* pInvoke = (_MessageInvokeIDInfo*)lParam;
	bool msg_result = WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if (msg_result)
	{
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS, Show_Results);
	}
	else
	{
		memcpy_s(&m_Array_data.at(pInvoke->mRow), sizeof(Str_array_point), &m_temp_array_data[pInvoke->mRow], sizeof(Str_array_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_ARRAY_LIST, pInvoke->mRow, REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS, Show_Results);
	}
	if ((pInvoke->mRow % 2) == 0)	//恢复前景和 背景 颜色;
		m_array_list.SetItemBkColor(pInvoke->mRow, pInvoke->mCol, LIST_ITEM_DEFAULT_BKCOLOR, 0);
	else
		m_array_list.SetItemBkColor(pInvoke->mRow, pInvoke->mCol, LIST_ITEM_DEFAULT_BKCOLOR_GRAY, 0);
	m_array_list.RedrawItems(pInvoke->mRow, pInvoke->mRow);


	if (pInvoke)
		delete pInvoke;
	return 0;
}
