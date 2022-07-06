// CBacnetBuildingIOPoints.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CBacnetBuildingIOPoints.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CBacnetBuildingIOPoints 对话框

IMPLEMENT_DYNAMIC(CBacnetBuildingIOPoints, CDialogEx)

CBacnetBuildingIOPoints::CBacnetBuildingIOPoints(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BACNET_BUILDING_IO_POINT, pParent)
{

}

CBacnetBuildingIOPoints::~CBacnetBuildingIOPoints()
{
}

void CBacnetBuildingIOPoints::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BM_IO_POINTS, m_io_list);
}


BEGIN_MESSAGE_MAP(CBacnetBuildingIOPoints, CDialogEx)
	ON_MESSAGE(WM_REFRESH_BAC_BUILDING_IO_LIST, Fresh_Building_IO_List)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,         Change_Building_IO_Item)
END_MESSAGE_MAP()

HWND h_db_io_hwnd;

void CBacnetBuildingIOPoints::Fresh()
{
	Initial_List();
	Fresh_Building_IO_List(NULL,NULL);
	h_db_io_hwnd = this->m_hWnd;
}
// CBacnetBuildingIOPoints 消息处理程序

void CBacnetBuildingIOPoints::Initial_List()
{
	m_io_list.ShowWindow(SW_HIDE);
	m_io_list.DeleteAllItems();
	while (m_io_list.DeleteColumn(0));

	m_io_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	//m_io_list.SetExtendedStyle(m_io_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_io_list.SetExtendedStyle(m_io_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_io_list.InsertColumn(BM_IO_ITEM, _T("Num"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_io_list.InsertColumn(BM_IO_GROUP_NAME, _T("Group"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_io_list.InsertColumn(BM_IO_CATEGORY_NAME, _T("Category"), 120, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_io_list.InsertColumn(BM_IO_HW_POINT, _T("Items"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_io_list.InsertColumn(BM_IO_POINT, _T("Points"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_io_list.InsertColumn(BM_IO_TYPE, _T("Type"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_io_list.InsertColumn(BM_ID_PRODUCT, _T("Related Products"), 180, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	
	m_io_list.InsertColumn(BM_IO_STATUS, _T("Status"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_pragram_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_pragram_dlg_hwnd;
	m_io_list.SetListHwnd(this->m_hWnd);
	CRect list_rect, win_rect;
	m_io_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_pragram_dlg_hwnd, win_rect);
	m_io_list.Set_My_WindowRect(win_rect);
	m_io_list.Set_My_ListRect(list_rect);
	m_io_list.ShowWindow(SW_SHOW);

}

struct ALL_PRODUCT_NAME_NODE {
	int id;
	CString name;
};
vector <ALL_PRODUCT_NAME_NODE> pid_name_map;
int get_key_name(int nfunction, CString& temp_cs);
LRESULT CBacnetBuildingIOPoints::Fresh_Building_IO_List(WPARAM wParam, LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);
	int Fresh_Item;
	int isFreshOne = (int)lParam;

	CBacnetBMD* temp_main_point = NULL;
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	CBacnetBuildingManagement* temp_BMpoint;
	temp_BMpoint = pFrame->m_pTreeViewCrl->m_BMpoint;
	temp_main_point = &temp_BMpoint->BuildingNode;
	m_bm_io_data.clear();
	if (temp_main_point->m_child_count != 0)
	{
		for (int i = 0; i < temp_main_point->m_child_count; i++)
		{
			//区分有多少个Group
			CBacnetBMD* temp_group = NULL;
			temp_group = temp_main_point->pchild[i];
			if (temp_group->m_child_count == 0)
				continue;
			else
			{
				for (int j = 0; j < temp_group->m_child_count; j++)
				{
					//区分有多少节点
					CBacnetBMD* temp_node = NULL;
					temp_node = temp_group->pchild[j];
					if (temp_node->m_child_count == 0)
						continue;
					else
					{
						for (int k = 0; k < temp_node->m_child_count; k++)
						{
							//区分有多少个IO
							CBacnetBMD* temp_io = NULL;
							temp_io = temp_node->pchild[k];
							Str_BM_IO temp_bm_io;
							temp_bm_io.group_index = i;
							temp_bm_io.category_index = j;
							temp_bm_io.hw_index = k;
							temp_bm_io.Group_Name = temp_group->m_csName;
							temp_bm_io.Category_Name = temp_node->m_csName;
							temp_bm_io.HW_Point_Name = temp_io->m_csName;
							temp_bm_io.type = temp_io->m_node_type;
							temp_bm_io.nproperty = temp_io->m_property;
							if (temp_bm_io.type == TYPE_BM_INPUT)
								temp_bm_io.type_string = _T("input");
							else if (temp_bm_io.type == TYPE_BM_OUTPUT)
								temp_bm_io.type_string = _T("output");
							else if (temp_bm_io.type == TYPE_BM_VARIABLE)
								temp_bm_io.type_string = _T("variable");
							CString temp_point;
							if (temp_bm_io.nproperty.n_option == 1)
							{
								CString temp_type;
								get_key_name(temp_bm_io.nproperty.pan_str.ntype, temp_type);

								if (temp_bm_io.nproperty.pan_str.sub_panel == 0)
									temp_point.Format(_T("%d%s%d"), temp_bm_io.nproperty.pan_str.main_panel, temp_type.GetBuffer(), temp_bm_io.nproperty.pan_str.object_number);
								else 
									temp_point.Format(_T("%d.%d.%s%d"), temp_bm_io.nproperty.pan_str.main_panel, temp_bm_io.nproperty.pan_str.sub_panel, temp_type.GetBuffer(), temp_bm_io.nproperty.pan_str.object_number);
								
							}
							else if (temp_bm_io.nproperty.n_option == 2)
							{

								CString temp_type;
								get_key_name(temp_bm_io.nproperty.ins_str.ntype, temp_type);
								temp_point.Format(_T("%d%s%d"), temp_bm_io.nproperty.ins_str.device_instance, temp_type.GetBuffer(), temp_bm_io.nproperty.ins_str.object_number);
							}
							temp_bm_io.cs_property_name = temp_point;
							m_bm_io_data.push_back(temp_bm_io);
						}
					}
				}
			}
		}
	}
	m_io_list.DeleteAllItems();


	for (int i = 0; i < (int)m_bm_io_data.size(); i++)
	{
		CString temp_item;
		temp_item.Format(_T("%d"), i + 1);
		m_io_list.InsertItem(i, temp_item);
		m_io_list.SetItemText(i, BM_IO_ITEM, temp_item);
		m_io_list.SetItemText(i, BM_IO_GROUP_NAME, m_bm_io_data.at(i).Group_Name);
		m_io_list.SetItemText(i, BM_IO_CATEGORY_NAME, m_bm_io_data.at(i).Category_Name);
		m_io_list.SetItemText(i, BM_IO_HW_POINT, m_bm_io_data.at(i).HW_Point_Name);
		CString temp_status;
		if (m_bm_io_data.at(i).nstatus == 1)
			temp_status = _T("Online");
		else if (m_bm_io_data.at(i).nstatus == 2)
			temp_status = _T("Online but not available");
		else
			temp_status = _T("Offline");
		m_io_list.SetItemText(i, BM_IO_STATUS, temp_status);

		m_io_list.SetItemText(i, BM_IO_POINT, m_bm_io_data.at(i).cs_property_name);
		m_io_list.SetItemText(i, BM_IO_TYPE, m_bm_io_data.at(i).type_string);

		if (ListCtrlEx::ComboBox == m_io_list.GetColumnType(BM_ID_PRODUCT))
		{
			ListCtrlEx::CStrList strlist;
			for (int j = 0; j < pid_name_map.size(); j++)
			{
				strlist.push_back(pid_name_map.at(j).name);
			}
			m_io_list.SetCellStringList(i, BM_ID_PRODUCT, strlist);
		}



	}


	return 0;
}



void product_list()
{

#if 0
	m_product_iocount.clear();
	CString AllData;
	CString product_list_path;
	product_list_path = GetExePath(true) + _T("ResourceFile\\ProductMode.ini");
	int ncount = 0;
	ncount = GetPrivateProfileInt(_T("ProductName"), _T("nCount"),0, product_list_path);
	for (int i = 0; i < ncount; i++)
	{
		Str_product_io_count temp = {0};
		CString temp_index;
		temp_index.Format(_T("%d"), i + 1);
		GetPrivateProfileStringW(_T("ProductName"), temp_index, _T(""), AllData.GetBuffer(MAX_PATH * 10), MAX_PATH * 10, product_list_path);
		AllData.ReleaseBuffer();
		if (AllData.IsEmpty())
		{
			continue;
		}

		CStringArray temparray;
		SplitCStringA(temparray, AllData,_T(","));
		if(temparray.GetSize() != 6)
			continue;

		temp.cs_name = temparray.GetAt(0);
		temp.ai_count = _wtoi(temparray.GetAt(1));
		temp.bi_count = 0;
		temp.input_count = temp.ai_count + temp.bi_count;
		temp.ao_count = _wtoi(temparray.GetAt(2));
		temp.bo_count = _wtoi(temparray.GetAt(3));
		temp.output_count = temp.ao_count + temp.bo_count;
		temp.pid = _wtoi(temparray.GetAt(4));
		temp.sub_pid = _wtoi(temparray.GetAt(5));

		m_product_iocount.push_back(temp);
	}
#endif
}
extern CString cs_bm_ini;
LRESULT CBacnetBuildingIOPoints::Change_Building_IO_Item(WPARAM wParam, LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	int temp_count = m_io_list.GetItemCount();
	if (Changed_Item >= temp_count)
		return 1;
	CString New_CString = m_io_list.GetItemText(Changed_Item, Changed_SubItem);
	if (Changed_SubItem == BM_IO_POINT)
	{
		if (New_CString.IsEmpty())
			return 1;
		char temp_io_point[MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, New_CString.GetBuffer(), -1, temp_io_point, MAX_PATH, NULL, NULL);
		Str_points temp_p = {0};
		decode_point(temp_io_point, temp_p);
		if (temp_p.n_option == 0)
		{
			m_io_list.SetItemText(Changed_Item, Changed_SubItem, _T(""));
			return 0;
		}
		m_bm_io_data.at(Changed_Item).nproperty = temp_p;
		CString temp_lpAppname;
		temp_lpAppname.Format(_T("Group%d"), m_bm_io_data.at(Changed_Item).group_index);
		CString temp_array_value;

		for (int x = 0; x < (int)sizeof(Str_points); x++)
		{
			CString temp_char;
			if (x != 0)
				temp_array_value = temp_array_value + _T(",");

			temp_char.Format(_T("%02x"), (unsigned char)*((char*)&m_bm_io_data.at(Changed_Item).nproperty + x));
			temp_array_value = temp_array_value + temp_char;
		}
		temp_array_value.MakeUpper();
		CString temp_keyname;
		temp_keyname.Format(_T("nPropertyArray_%d_%d"), m_bm_io_data.at(Changed_Item).category_index, m_bm_io_data.at(Changed_Item).hw_index);
		WritePrivateProfileStringW(temp_lpAppname, temp_keyname, temp_array_value, cs_bm_ini);


		Sleep(1);
	}
	return 1;
}