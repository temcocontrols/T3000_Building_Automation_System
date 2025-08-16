// CBacnetBuildingRoomEditor.cpp: Implementation file
// CBacnetBuildingRoomEditor.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CBacnetBuildingRoomEditor.h"
#include "afxdialogex.h"

extern CString cs_bm_ini;
// CBacnetBuildingRoomEditor dialog
// CBacnetBuildingRoomEditor 对话框

IMPLEMENT_DYNAMIC(CBacnetBuildingRoomEditor, CDialogEx)

CBacnetBuildingRoomEditor::CBacnetBuildingRoomEditor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BACNET_BUILDING_EDIT_FLOOR, pParent)
{

}

CBacnetBuildingRoomEditor::~CBacnetBuildingRoomEditor()
{
}

void CBacnetBuildingRoomEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BM_FLOOR, m_static_floor);
	DDX_Control(pDX, IDC_STATIC_BM_ROOM, m_static_room);
	DDX_Control(pDX, IDC_LIST_BM_FLOOR, m_bm_floor_list);
	DDX_Control(pDX, IDC_LIST_BM_ROOM, m_bm_room_list);
}


BEGIN_MESSAGE_MAP(CBacnetBuildingRoomEditor, CDialogEx)
	ON_MESSAGE(WM_REFRESH_BAC_FLOOR_LIST, Fresh_Floor_List)
	ON_MESSAGE(WM_REFRESH_BAC_ROOM_LIST, Fresh_Room_List)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED, Fresh_EditCallBack_Item)
	ON_MESSAGE(WM_LIST_FLOOR_CHANGED, Fresh_Floor_Item)
	ON_MESSAGE(WM_LIST_ROOM_CHANGED, Fresh_Room_Item)
	
	ON_BN_CLICKED(IDC_BUTTON_BM_FLOOR_ADD, &CBacnetBuildingRoomEditor::OnBnClickedButtonBmFloorAdd)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BM_FLOOR, &CBacnetBuildingRoomEditor::OnNMClickListBmFloor)
	ON_NOTIFY(NM_CLICK, IDC_LIST_BM_ROOM, &CBacnetBuildingRoomEditor::OnNMClickListBmRoom)
END_MESSAGE_MAP()



LRESULT CBacnetBuildingRoomEditor::Fresh_Floor_Item(WPARAM wParam, LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	CString New_CString = m_bm_floor_list.GetItemText(Changed_Item, Changed_SubItem);
	if (New_CString.IsEmpty())
		return 0;

	if (Changed_Item > bm_floor.size() + 1)
	{
		return 1;
	}
	else if (Changed_Item == bm_floor.size())
	{
		//int last_index = 0;
		//last_index = bm_floor.back().xindex + 1;
		// New item added
		//新增的项目
		//vector < RoomInfo> bm_room;
		FloorInfo temp_floor;
		temp_floor.csName = New_CString;
		temp_floor.xindex = Changed_Item;
		bm_floor.push_back(temp_floor);
	}
	else
	{
		bm_floor.at(Changed_Item).csName = New_CString;
	}
	// Save to local cache
	//保存至本地缓存;
	CString temp_total_index;
	for (int i = 0; i < bm_floor.size(); i++)
	{
		CString temp;
		if (i != 0)
			temp_total_index = temp_total_index + _T(",");
		
		temp.Format(_T("%d"), bm_floor.at(i).xindex);
		temp_total_index = temp_total_index + temp;
	}
	// Modify total
	//WritePrivateProfileStringW(_T("FloorInfo"), _T("FloorIndex"), temp_total_index, cs_bm_ini); //修改总
	
    
	//PostMessage(WM_REFRESH_BAC_FLOOR_LIST,NULL,NULL);
	//PostMessage(WM_REFRESH_BAC_ROOM_LIST, NULL, NULL);
	return 1;
}

LRESULT CBacnetBuildingRoomEditor::Fresh_Room_Item(WPARAM wParam, LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	return 1;
}
// CBacnetBuildingRoomEditor message handlers
// CBacnetBuildingRoomEditor 消息处理程序
// For a dialog with multiple Lists, can only send Changed messages by confirming focus
//对于一个对话框里面有多个List 只能通过确认焦点 来发送Changed 消息;
LRESULT CBacnetBuildingRoomEditor::Fresh_EditCallBack_Item(WPARAM wParam, LPARAM lParam)
{
	CWnd* window_focus = GetFocus();
	if (window_focus == NULL)
		return 0;
	if (GetFocus()->GetDlgCtrlID() == IDC_LIST_BM_FLOOR)
	{
		PostMessage(WM_LIST_FLOOR_CHANGED, wParam, lParam);
	}
	else if (GetFocus()->GetDlgCtrlID() == IDC_LIST_BM_ROOM)
	{
		PostMessage(WM_LIST_ROOM_CHANGED, wParam, lParam);
	}

	return 0;
}


BOOL CBacnetBuildingRoomEditor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here
	// TODO:  在此添加额外的初始化
	InitialUI();
	Initial_List();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // Exception: OCX property pages should return FALSE
				  // 异常: OCX 属性页应返回 FALSE
}

void CBacnetBuildingRoomEditor::InitialUI()
{
	m_static_floor.SetWindowTextW(_T("Floor"));
	m_static_floor.textColor(RGB(0, 0, 255));
	//m_static_floor.bkColor(RGB(0,255,255));
	m_static_floor.setFont(20, 16, NULL, _T("Arial"));

	m_static_room.SetWindowTextW(_T("Room"));
	m_static_room.textColor(RGB(0, 0, 255));
	//m_static_room.bkColor(RGB(0,255,255));
	m_static_room.setFont(20, 16, NULL, _T("Arial"));
}

void CBacnetBuildingRoomEditor::Initial_List()
{
	//m_bm_floor_list
	while (m_bm_floor_list.DeleteColumn(0));

	m_bm_floor_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_bm_floor_list.SetExtendedStyle(m_bm_floor_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_bm_floor_list.InsertColumn(0, _T(" "), 30, ListCtrlEx::CheckBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bm_floor_list.InsertColumn(1, _T("Floor Name"), 130, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bm_floor_list.SetListHwnd(this->m_hWnd);
	m_bm_floor_list.SetWhetherShowBkCol(false);
	while (m_bm_room_list.DeleteColumn(0));

	m_bm_room_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_bm_room_list.SetExtendedStyle(m_bm_floor_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_bm_room_list.InsertColumn(0, _T(" "), 30, ListCtrlEx::CheckBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bm_room_list.InsertColumn(1, _T("Room Name"), 130, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_bm_room_list.SetListHwnd(this->m_hWnd);
	m_bm_room_list.SetWhetherShowBkCol(false);
	Fresh_Floor_List(0, 0);
	Fresh_Room_List(0, 0);
}

LRESULT CBacnetBuildingRoomEditor::Fresh_Floor_List(WPARAM wParam, LPARAM lParam)
{
	m_bm_floor_list.DeleteAllItems();
	for (int i = 0; i < bm_floor.size(); i++)
	{
		CString temp_floor;
		temp_floor = bm_floor.at(i).csName;
		m_bm_floor_list.InsertItem(i, _T(" "));
		m_bm_floor_list.SetItemText(i, 1, temp_floor);
		m_bm_floor_list.SetCellEnabled(i, 0, false);
	}
	return 0;
}

LRESULT CBacnetBuildingRoomEditor::Fresh_Room_List(WPARAM wParam, LPARAM lParam)
{
	m_bm_room_list.DeleteAllItems();
	for (int i = 0; i < bm_room.size(); i++)
	{
		CString temp_room;
		temp_room = bm_room.at(i).csName;
		m_bm_room_list.InsertItem(i, _T(" "));
		m_bm_room_list.SetItemText(i, 1, temp_room);
		m_bm_room_list.SetCellEnabled(i, 0, false);
	}
	return 0;
}



void CBacnetBuildingRoomEditor::OnBnClickedButtonBmFloorAdd()
{
	// TODO: Add your control notification handler code here
	// TODO: 在此添加控件通知处理程序代码
	int ncount = m_bm_floor_list.GetItemCount();
	if (ncount > bm_floor.size())
	{
		MessageBox(_T("Unnamed items have been added to the list"));
		return;
	}
	m_bm_floor_list.InsertItem(ncount , _T(""));
	m_bm_floor_list.SetCellEnabled(ncount, 0, false);
}


void CBacnetBuildingRoomEditor::OnNMClickListBmFloor(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	DWORD dwPos = GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_bm_floor_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	int nItem = m_bm_floor_list.SubItemHitTest(&lvinfo);

	// If click area exceeds maximum row number, the click is invalid
	if (lvinfo.iItem > m_bm_floor_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if (lvinfo.iItem < 0)
		return;
	if (nItem != -1)
	{

		m_bm_floor_list.SetCellChecked(nItem, 0, 1);
		program_list_line = nItem;
		for (int i = 0; i < m_bm_floor_list.GetItemCount(); ++i)
		{
			if (i == nItem)
				continue;
			m_bm_floor_list.SetCellChecked(i, 0, FALSE);
		}
	}

}


void CBacnetBuildingRoomEditor::OnNMClickListBmRoom(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	DWORD dwPos = GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_bm_room_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	int nItem = m_bm_room_list.SubItemHitTest(&lvinfo);
	if (nItem != -1)
	{
		m_bm_room_list.SetCellChecked(nItem, 0, 1);
		program_list_line = nItem;
		for (int i = 0; i < m_bm_room_list.GetItemCount(); ++i)
		{
			if (i == nItem)
				continue;
			m_bm_room_list.SetCellChecked(i, 0, FALSE);
		}
	}
}
