// CBacnetBuildingManagement.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CBacnetBuildingManagement.h"
#include "MainFrm.h"
#include "global_function.h"

extern HWND h_db_io_hwnd;
CMainFrame* pFrame_BM;
// CBacnetBuildingManagement
CString cs_bm_ini;
IMPLEMENT_DYNCREATE(CBacnetBuildingManagement, CDialogEx)

CBacnetBuildingManagement::CBacnetBuildingManagement()
	: CDialogEx(IDD_DIALOG_BACNET_BUILDING_MANAGEMENT)
{
	memset(n_group_kids_count, 0, 255);
}

CBacnetBuildingManagement::~CBacnetBuildingManagement()
{
}

void CBacnetBuildingManagement::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BM_ROOT, m_root_list);
}

BEGIN_MESSAGE_MAP(CBacnetBuildingManagement, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BM_ADD, &CBacnetBuildingManagement::OnBnClickedButtonBmAdd)
	ON_BN_CLICKED(IDC_BUTTON_BM_DELETE, &CBacnetBuildingManagement::OnBnClickedButtonBmDelete)
	ON_BN_CLICKED(IDC_BUTTON_BM_DONE, &CBacnetBuildingManagement::OnBnClickedButtonBmDone)
	ON_BN_CLICKED(IDC_BUTTON_BM_SAVE, &CBacnetBuildingManagement::OnBnClickedButtonBmSave)
	ON_BN_CLICKED(IDC_BUTTON_DB_DONE, &CBacnetBuildingManagement::OnBnClickedButtonDbDone)
END_MESSAGE_MAP()


// CBacnetBuildingManagement 诊断

#ifdef _DEBUG
void CBacnetBuildingManagement::AssertValid() const
{
	CDialogEx::AssertValid();
}

#ifndef _WIN32_WCE
void CBacnetBuildingManagement::Dump(CDumpContext& dc) const
{
	CDialogEx::Dump(dc);
}
#endif
#endif //_DEBUG


// CBacnetBuildingManagement 消息处理程序


void CBacnetBuildingManagement::Fresh()
{

	pFrame_BM->SetWindowTextW(cs_special_name + CurrentT3000Version);
	Initial_List();

	LoadFileShowToList();
	// TODO: 在此处添加实现代码.
}


void CBacnetBuildingManagement::Initial_List()
{
	m_root_list.ShowWindow(SW_HIDE);
	m_root_list.DeleteAllItems();
	while (m_root_list.DeleteColumn(0));

	m_root_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	//m_root_list.SetExtendedStyle(m_root_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_root_list.SetExtendedStyle(m_root_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_root_list.InsertColumn(BM_ROOT_ITEM, _T("Item"), 80, ListCtrlEx::CheckBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_root_list.InsertColumn(BM_ROOT_GROUP_NAME, _T("Group Name"), 150, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	//m_root_list.InsertColumn(BM_ROOT_GROUP_COUNT, _T("Count"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_root_list.InsertColumn(BM_ROOT_TOTAL_INPUT, _T("Total Inputs"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_root_list.InsertColumn(BM_ROOT_TOTAL_OUTPUT, _T("Total Outputs"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_root_list.InsertColumn(BM_ROOT_TOTAL_VARIABLE, _T("Total Variable"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_pragram_dlg_hwnd = this->m_hWnd;
	//g_hwnd_now = m_pragram_dlg_hwnd;
	m_root_list.SetListHwnd(this->m_hWnd);
	CRect list_rect, win_rect;
	m_root_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_pragram_dlg_hwnd, win_rect);
	m_root_list.Set_My_WindowRect(win_rect);
	m_root_list.Set_My_ListRect(list_rect);




//	m_root_list.SetCellChecked(0, 0, 1);
	m_root_list.ShowWindow(SW_SHOW);
}


void CBacnetBuildingManagement::OnBnClickedButtonBmAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	return;
}

//int test_count = 35;
void CBacnetBuildingManagement::OnBnClickedButtonBmDelete()
{
	// TODO: 在此添加控件通知处理程序代码
	if (MessageBox(_T("This operation will clear all the configuration. Are you sure?"), _T("Notic"), MB_OKCANCEL) == IDOK)
	{
		DeleteFile(cs_bm_ini);
		Initial_List();

		LoadFileShowToList();
	}
	return;
#if 0
	CString tempcs;
	tempcs.Format(_T("%d"), test_count);
	TV_INSERTSTRUCT tvInsert;////added
	CString strBuilding = tempcs;//m_strCurSubBuldingName;//m_subNetLst.at(k).strBuildingName;

	tvInsert.hParent = TVI_ROOT; // 指定父句柄
	tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
	tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strBuilding;
	tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
	//TVINSERV_BUILDING
	tvInsert.item.iImage = test_count; tvInsert.item.iSelectedImage = test_count;
	HTREEITEM hTreePointList = NULL;
	hTreePointList = pFrame_BM->m_pTreeViewCrl->InsertSubnetItem(&tvInsert);//插入PointList
	test_count++;
#endif
}

void CBacnetBuildingManagement::SaveAllIntoIniFile()
{
	nGroupCount = BuildingNode.m_child_count;//m_root_list.GetItemCount();
	CString temp_lpAppname;
	DeleteFile(cs_bm_ini);
	CString temp_cstotal_group;
	temp_cstotal_group.Format(_T("%d"), nGroupCount);
	WritePrivateProfileStringW(_T("Setting"), _T("TotalGroup"), temp_cstotal_group, cs_bm_ini);
	for (int i = 0; i < BuildingNode.m_child_count; i++)
	{
		temp_lpAppname.Format(_T("Group%d"), i);
		WritePrivateProfileStringW(temp_lpAppname, _T("GroupName"), BuildingNode.pchild[i]->m_csName, cs_bm_ini);
		CString temp_count;
		temp_count.Format(_T("%d"), BuildingNode.pchild[i]->m_child_count);
		WritePrivateProfileStringW(temp_lpAppname, _T("GroupChildCount"), temp_count, cs_bm_ini);
        
		for (int j = 0; j < BuildingNode.pchild[i]->m_child_count; j++)
		{
			CString section_device_name;
			CString section_io_count;
			section_device_name.Format(_T("DeviceName%d"),j);
			section_io_count.Format(_T("IOCount%d"), j);
			CString temp_name;
			CString temp_count;
			temp_name = BuildingNode.pchild[i]->pchild[j]->m_csName;
			temp_count.Format(_T("%d"), BuildingNode.pchild[i]->pchild[j]->m_child_count);
			WritePrivateProfileStringW(temp_lpAppname, section_device_name, temp_name, cs_bm_ini);
			WritePrivateProfileStringW(temp_lpAppname, section_io_count, temp_count, cs_bm_ini);

			for (int z = 0; z < BuildingNode.pchild[i]->pchild[j]->m_child_count; z++)
			{
				CString section_io_name;
				CString section_io_type;
				section_io_name.Format(_T("IO_%d_%d_Name"),j,z);
				section_io_type.Format(_T("IO_%d_%d_Type"), j, z);
				CString temp_name;
				CString temp_io_type;
				temp_name = BuildingNode.pchild[i]->pchild[j]->pchild[z]->m_csName;
				temp_io_type.Format(_T("%d"), BuildingNode.pchild[i]->pchild[j]->pchild[z]->m_node_type);

				WritePrivateProfileStringW(temp_lpAppname, section_io_name, temp_name, cs_bm_ini);
				WritePrivateProfileStringW(temp_lpAppname, section_io_type, temp_io_type, cs_bm_ini);
			}
		}
	}
}

void CBacnetBuildingManagement::OnBnClickedButtonBmDone()
{
	// TODO: 在此添加控件通知处理程序代码
	//ShowListToTree();
}

void CBacnetBuildingManagement::LoadFileShowToList()
{
	b_building_management_flag = 1;
	pFrame_BM->ClearBuilding();
	pFrame_BM->m_pTreeViewCrl->DeleteAllItems();
	nGroupCount = GetPrivateProfileInt(_T("Setting"), _T("TotalGroup"), 0, cs_bm_ini);
	if (nGroupCount == 0)
	{
		TreeInital();
		return;
	}
	m_root_list.DeleteAllItems();
	for (int i = 0; i < nGroupCount; i++)
	{
		m_root_list.InsertItem(i, _T(" "));
		CString temp_lpAppname;
		temp_lpAppname.Format(_T("Group%d"), i);
		GetPrivateProfileString(temp_lpAppname, _T("GroupName"), _T(""), groupname[i].GetBuffer(MAX_PATH), MAX_PATH, cs_bm_ini);
		groupname[i].ReleaseBuffer();
		n_group_kids_count[i] = GetPrivateProfileInt(temp_lpAppname, _T("GroupChildCount"), 0,  cs_bm_ini);
		CString temp_group_count;
		//temp_group_count.Format(_T("%d"), n_group_kids_count[i]);
		m_root_list.SetItemText(i, 1, groupname[i]);
		//m_root_list.SetItemText(i, 2, temp_group_count);

	}
	TreeInital();
	::PostMessage(h_db_io_hwnd, WM_REFRESH_BAC_BUILDING_IO_LIST, NULL, NULL);
}

void CBacnetBuildingManagement::ShowListToTree()
{
	nGroupCount = m_root_list.GetItemCount();
	for (int i = 0; i < nGroupCount; i++)
	{
		CString temp_group_count;
		groupname[i] = m_root_list.GetItemText(i, 1);
		temp_group_count = m_root_list.GetItemText(i, 2);
		n_group_kids_count[i] = _wtoi(temp_group_count);
	}

	if (nGroupCount == 0)
	{
		MessageBox(_T("Please add device"));
		return;
	}

	TreeInital();
}

HTREEITEM hTreeDeviceList = NULL;
void CBacnetBuildingManagement::TreeInital()
{
	TV_INSERTSTRUCT tvInsert;////added
	CString strBuilding = _T("Point List");//m_strCurSubBuldingName;//m_subNetLst.at(k).strBuildingName;

	tvInsert.hParent = TVI_ROOT; // 指定父句柄
	tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
	tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strBuilding;
	tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
	TVINSERV_BUILDING

	HTREEITEM hTreePointList = NULL;

	hTreePointList = pFrame_BM->m_pTreeViewCrl->InsertSubnetItem(&tvInsert);//插入PointList

#pragma region 根节点显示PointList
	BuildingNode.hParent = TVI_ROOT;
	BuildingNode.h_treeitem = hTreePointList;
	BuildingNode.m_child_count = (unsigned char)nGroupCount;
	BuildingNode.m_csName = _T("Point List");
	BuildingNode.m_index = 0; //根节点 序号就是0
#pragma endregion



	for (int i = 0; i < nGroupCount; i++)
	{
		HTREEITEM hTreeGroup = NULL;
		tvInsert.hParent = hTreePointList; // 指定父句柄
		tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
		tvInsert.item.pszText = (LPTSTR)(LPCTSTR)groupname[i];
		tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
		TVINSERV_ROOM
			hTreeGroup = pFrame_BM->m_pTreeViewCrl->InsertSubnetItem(&tvInsert);//插入PointList

		if (BuildingNode.pchild[i] == NULL)
			BuildingNode.pchild[i] = new CBacnetBMD;

		BuildingNode.pchild[i]->hParent = hTreePointList;
		BuildingNode.pchild[i]->h_treeitem = hTreeGroup;
		BuildingNode.pchild[i]->m_child_count = n_group_kids_count[i];
		BuildingNode.pchild[i]->m_csName = groupname[i];
		BuildingNode.pchild[i]->m_index = i;
		BuildingNode.pchild[i]->pfather = &BuildingNode;
		if (BuildingNode.pchild[i]->m_child_count == 0)
			continue;
		CString temp_lpAppname;
		temp_lpAppname.Format(_T("Group%d"), i);
		//int temp_group_input = 0;
		//int temp_group_variable = 0;
		//int temp_group_output = 0;
		for (int j = 0; j < BuildingNode.pchild[i]->m_child_count; j++)
		{
			if (BuildingNode.pchild[i]->pchild[j] == NULL)
				BuildingNode.pchild[i]->pchild[j] = new CBacnetBMD;
			int temp_io_count = 0;
			HTREEITEM hTreeABC123 = NULL;
			CString temp_abc1_name; //这里要先从数据库读取名字，读取不到就用默认的名字;
			CString section_device_name;
			CString section_io_count;
			section_device_name.Format(_T("DeviceName%d"), j);
			section_io_count.Format(_T("IOCount%d"), j);
			BuildingNode.pchild[i]->pchild[j]->m_child_count =	GetPrivateProfileInt(temp_lpAppname, section_io_count, 0, cs_bm_ini);	
			temp_io_count = BuildingNode.pchild[i]->pchild[j]->m_child_count;
			GetPrivateProfileString(temp_lpAppname, section_device_name, _T(""), BuildingNode.pchild[i]->pchild[j]->m_csName.GetBuffer(MAX_PATH), MAX_PATH, cs_bm_ini);
			BuildingNode.pchild[i]->pchild[j]->m_csName.ReleaseBuffer();
			temp_abc1_name = BuildingNode.pchild[i]->pchild[j]->m_csName;

			tvInsert.hParent = hTreeGroup; // 指定父句柄
			tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
			tvInsert.item.pszText = (LPTSTR)(LPCTSTR)temp_abc1_name;
			tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
			if(i==0)
				TVINSERV_MINIPANEL   //这里到时候要判断到底是什么设备
			else if (i == 1)
				TVINSERV_TSTAT8
			else if (i == 2)
				TVINSERV_T3ARM
			else
				TVINSERV_MINIPANEL
			hTreeABC123 = pFrame_BM->m_pTreeViewCrl->InsertSubnetItem(&tvInsert);//插入PointList

			CBacnetBMD* temp_bmd_point = NULL;

			temp_bmd_point = BuildingNode.pchild[i]->pchild[j];
			temp_bmd_point->m_input_count = 0;
			temp_bmd_point->m_output_count = 0;
			temp_bmd_point->m_variable_count = 0;
			temp_bmd_point->hParent = hTreeGroup;
			temp_bmd_point->h_treeitem = hTreeABC123;
			temp_bmd_point->m_child_count = temp_io_count;
			temp_bmd_point->m_csName = temp_abc1_name;
			temp_bmd_point->m_index = j;
			temp_bmd_point->m_node_type = TYPE_BM_NODES;
			temp_bmd_point->pfather = BuildingNode.pchild[i];

			for (int z = 0; z < temp_bmd_point->m_child_count; z++)
			{
				if (temp_bmd_point->pchild[z] == NULL)
					temp_bmd_point->pchild[z] = new CBacnetBMD;
				CString section_io_name;
				CString section_io_type;
				CString section_io_status;
				section_io_name.Format(_T("IO_%d_%d_Name"), j, z);
				section_io_type.Format(_T("IO_%d_%d_Type"), j, z);
				section_io_status.Format(_T("IO_%d_%d_Status"), j, z);
				CString io_name;
				int io_type;
				int io_status;
				GetPrivateProfileString(temp_lpAppname, section_io_name, _T(""), io_name.GetBuffer(MAX_PATH), MAX_PATH, cs_bm_ini);
				io_name.ReleaseBuffer();
				io_type = GetPrivateProfileInt(temp_lpAppname, section_io_type, 0, cs_bm_ini);
				io_status = GetPrivateProfileInt(temp_lpAppname, section_io_status, TREE_IO_UNKNOWN, cs_bm_ini);
				CString section_property;
				section_property.Format(_T("nPropertyArray_%d_%d"), j, z);
				CString temp_property_cstring;
				GetPrivateProfileString(temp_lpAppname, section_property, _T(""), temp_property_cstring.GetBuffer(MAX_PATH), MAX_PATH, cs_bm_ini);
				temp_property_cstring.ReleaseBuffer();







				tvInsert.hParent = hTreeABC123; // 指定父句柄
				tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
				tvInsert.item.pszText = (LPTSTR)(LPCTSTR)io_name;
				tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式

				//暂时用input
				if (io_type == TYPE_BM_INPUT)
				{
					temp_bmd_point->m_input_count++;
					if (io_status == TREE_IO_UNKNOWN)
					{
						tvInsert.item.iImage = TREE_IMAGE_INPUT_UNKNOWN; tvInsert.item.iSelectedImage = TREE_IMAGE_INPUT_UNKNOWN;
					}
					else if (io_status == TREE_IO_OFFLINE)
					{
						tvInsert.item.iImage = TREE_IMAGE_INPUT_OFFLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_INPUT_OFFLINE;
					}
					else if (io_status == TREE_IO_ONLINE)
					{
						tvInsert.item.iImage = TREE_IMAGE_INPUT_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_INPUT_ONLINE;
					}
				}
				else if (io_type == TYPE_BM_OUTPUT)
				{
					temp_bmd_point->m_output_count++;
					if (io_status == TREE_IO_UNKNOWN)
					{
						tvInsert.item.iImage = TREE_IMAGE_OUTPUT_UNKNOWN; tvInsert.item.iSelectedImage = TREE_IMAGE_OUTPUT_UNKNOWN;
					}
					else if (io_status == TREE_IO_OFFLINE)
					{
						tvInsert.item.iImage = TREE_IMAGE_OUTPUT_OFFLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_OUTPUT_OFFLINE;
					}
					else if (io_status == TREE_IO_ONLINE)
					{
						tvInsert.item.iImage = TREE_IMAGE_OUTPUT_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_OUTPUT_ONLINE;
					}
				}
				else if (io_type == TYPE_BM_VARIABLE)
				{
					temp_bmd_point->m_variable_count++;
					if (io_status == TREE_IO_UNKNOWN)
					{
						tvInsert.item.iImage = TREE_IMAGE_VARIABLE_UNKNOWN; tvInsert.item.iSelectedImage = TREE_IMAGE_VARIABLE_UNKNOWN;
					}
					else if (io_status == TREE_IO_OFFLINE)
					{
						tvInsert.item.iImage = TREE_IMAGE_VARIABLE_OFFLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_VARIABLE_OFFLINE;
					}
					else if (io_status == TREE_IO_ONLINE)
					{
						tvInsert.item.iImage = TREE_IMAGE_VARIABLE_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_VARIABLE_ONLINE;
					}
				}
				HTREEITEM hTreeIO = NULL;
				hTreeIO = pFrame_BM->m_pTreeViewCrl->InsertSubnetItem(&tvInsert);//插入PointList

				CBacnetBMD* temp_io_point = NULL;

				temp_io_point = BuildingNode.pchild[i]->pchild[j]->pchild[z];
				temp_io_point->hParent = hTreeGroup;
				temp_io_point->h_treeitem = hTreeIO;
				temp_io_point->m_child_count = 0;
				temp_io_point->m_csName = io_name;
				temp_io_point->m_index = z;
				temp_io_point->m_node_type = io_type;
				temp_io_point->pfather = temp_bmd_point;

				if (temp_property_cstring.IsEmpty() == false)
				{
					CStringArray temp_array;
					SplitCStringA(temp_array, temp_property_cstring, _T(","));
					unsigned char temp_char[250];
					if (temp_array.GetSize() == sizeof(Str_points))
					{
						for (int x = 0; x < (int)sizeof(Str_points); x++)
						{
							temp_char[x] = wcstol(temp_array.GetAt(x), NULL, 16);
							//= _wtoi(temp_array.GetAt(x));
						}
						memcpy_s(&temp_io_point->m_property, sizeof(Str_points), temp_char, sizeof(Str_points));
					}

				}
				Sleep(1);


			}
			if (hTreeGroup != NULL)
				pFrame_BM->m_pTreeViewCrl->Expand(hTreeABC123, TVE_EXPAND);

			//temp_group_input = temp_group_input + temp_bmd_point->m_input_count;
			//temp_group_output = temp_group_output + temp_bmd_point->m_output_count;
			//temp_group_variable = temp_group_variable + temp_bmd_point->m_variable_count;

		}
		//BuildingNode.pchild[i]->m_input_count = temp_group_input;
		//BuildingNode.pchild[i]->m_output_count = temp_group_output;
		//BuildingNode.pchild[i]->m_variable_count = temp_group_variable;

		if (hTreeGroup != NULL)
			pFrame_BM->m_pTreeViewCrl->Expand(hTreeGroup, TVE_EXPAND);

	}
	if (hTreePointList != NULL)
		pFrame_BM->m_pTreeViewCrl->Expand(hTreePointList, TVE_EXPAND);
	BuildingNode.UpdateCount(); //递归计算每一个input和output的数量

	UpdateList();

	SetDataTreeCtrl();


	CString strvirtualdevice = _T("System List");//m_strCurSubBuldingName;//m_subNetLst.at(k).strBuildingName;
	tvInsert.hParent = TVI_ROOT; // 指定父句柄
	tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
	tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strvirtualdevice;
	tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
	TVINSERV_BUILDING



	hTreeDeviceList = pFrame_BM->m_pTreeViewCrl->InsertSubnetItem(&tvInsert);//插入PointList

	LoadDevice();
	pFrame_BM->m_pTreeViewCrl->Expand(hTreeDeviceList, TVE_EXPAND);
}

void CBacnetBuildingManagement::LoadDevice()
{
	CString strBuilding = pFrame_BM->m_strCurSubBuldingName;//m_subNetLst.at(k).strBuildingName;
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	CString strSql;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	strSql.Format(_T("select * from ALL_NODE where Building_Name = '%s' "), strBuilding);
	q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
	CString temp_parent_serialnum;
	TV_INSERTSTRUCT tvInsert;
	if (!q.eof())
	{
		while (!q.eof())//所有 设备;
		{

			tree_product m_product_temp = { 0 };
			CString temp_ext;
			temp_ext = q.getValuebyName(L"Screen_Name");
			pFrame_BM->GetExtProductInfo(m_product_temp, temp_ext);
			if (m_product_temp.m_ext_info.virtual_device == 1)
			{
				tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM | ITEM_MASK;
				tvInsert.item.lParam = TREE_LP_VIRTUAL_DEVICE;
			}
			else
			{
				tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
				//q.nextRow();
				//continue;
			}
			temp_parent_serialnum = q.getValuebyName(L"Parent_SerialNum");
			unsigned int temp_int_serial;
			temp_int_serial = (unsigned int)_wtoi(temp_parent_serialnum);
			if ((!temp_parent_serialnum.IsEmpty()) && (temp_int_serial != 0))	//说明有父节点;先插入父节点
			{
				q.nextRow();
				continue;
			}

			CString strProdcut = q.getValuebyName(L"Product_name");

			tvInsert.hParent = hTreeDeviceList;


			// tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
			tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strProdcut;
			//TRACE(strProdcut);
			tvInsert.hInsertAfter = TVI_SORT;// TVI_LAST; // 项目插入方式
			int temp_product_class_id = q.getIntField("Product_class_ID");
			if ((temp_product_class_id != PM_MINIPANEL) && (temp_product_class_id != PM_MINIPANEL_ARM) && (temp_product_class_id != PM_ESP32_T3_SERIES))//Mini Panel)
			{//不是T3 minipanel 系列
				q.nextRow();
				continue;
			}
			if (temp_product_class_id == PM_NC || temp_product_class_id == PM_SOLAR)
				TVINSERV_NET_WORK
			else if (temp_product_class_id == PM_CM5) //CM5
				TVINSERV_CMFIVE
			else if (temp_product_class_id == PM_T3_LC) //CM5
				TVINSERV_T3LC
			else if (temp_product_class_id == PM_T3PT10 ||
				temp_product_class_id == PM_T3IOA ||
				temp_product_class_id == PM_T332AI ||
				temp_product_class_id == PM_T38AI16O ||
				temp_product_class_id == PM_T38I13O ||
				temp_product_class_id == PM_T34AO ||
				temp_product_class_id == PM_T36CT) //T3
				TVINSERV_NET_WORK
			else if (
				temp_product_class_id == PM_T322AI ||
				temp_product_class_id == PM_T332AI_ARM ||
				temp_product_class_id == PM_T3PT12 ||
				temp_product_class_id == PM_T36CTA ||
				temp_product_class_id == PM_T3_LC ||
				temp_product_class_id == PM_T38AI8AO6DO
				)
				TVINSERV_T3ARM
			else if (temp_product_class_id == PM_MINIPANEL || temp_product_class_id == PM_MINIPANEL_ARM || temp_product_class_id == PM_ESP32_T3_SERIES)//Mini Panel
			{
				if (m_product_temp.m_ext_info.virtual_device == 1)
				{
					if (m_product_temp.m_ext_info.mini_type == MINIPANELARM)
						TVINSERV_MINIPANEL
					else if (m_product_temp.m_ext_info.mini_type == MINIPANELARM_LB)
						TVINSERV_T3ARM
					else if (m_product_temp.m_ext_info.mini_type == MINIPANELARM_TB)
						TVINSERV_T3ARM
					else if (m_product_temp.m_ext_info.mini_type == MINIPANELARM_NB)
						TVINSERV_T3_NANO
					else if(m_product_temp.m_ext_info.mini_type == T3_TSTAT10)
						TVINSERV_TSTAT8
				}
				else
					TVINSERV_MINIPANEL
			}
			else if (temp_product_class_id == PM_AirQuality)//AirQuality
				TVINSERV_TSTAT_DEFAULT
			else if (temp_product_class_id == PM_LightingController)//Lightingcontroller
				TVINSERV_LC          //tree0412
			else if (temp_product_class_id == PM_TSTAT7 || temp_product_class_id == PM_TSTAT7_ARM)//TSTAT7 &TSTAT6 //tree0412
				TVINSERV_LED_TSTAT7 //tree0412
			else if (temp_product_class_id == PM_TSTAT6 || temp_product_class_id == PM_TSTAT5i)
				TVINSERV_TSTAT6
			else if (temp_product_class_id == PM_TSTAT10 ||
				temp_product_class_id == PM_TSTAT8 ||
				temp_product_class_id == PM_TSTAT9 ||
				temp_product_class_id == PM_TSTAT8_WIFI ||
				temp_product_class_id == PM_TSTAT8_OCC ||
				temp_product_class_id == PM_TSTAT_AQ ||
				temp_product_class_id == PM_AIRLAB_ESP32 ||
				temp_product_class_id == PM_TSTAT8_220V)
				TVINSERV_TSTAT8
			else if (temp_product_class_id == PM_MULTI_SENSOR)
				TVINSERV_TSTAT8   //暂且用TSTAT8 的图标
			else if (temp_product_class_id == PM_ZIGBEE_REPEATER)
				TVINSERV_T3_NANO
			else if ((temp_product_class_id == PM_CO2_NET) ||
				(temp_product_class_id == PM_CO2_RS485) ||
				(temp_product_class_id == PM_PRESSURE_SENSOR) ||
				(temp_product_class_id == STM32_PRESSURE_NET) ||
				(temp_product_class_id == STM32_PRESSURE_RS485) ||
				(temp_product_class_id == STM32_CO2_NET) ||
				(temp_product_class_id == STM32_PM25) ||
				(temp_product_class_id == STM32_CO2_RS485) ||
				(temp_product_class_id == STM32_HUM_NET) ||    //2019 03 28 修复 HUM 使用默认TSTAT图标的bug
				(temp_product_class_id == STM32_HUM_RS485))
				TVINSERV_CO2
			else if (temp_product_class_id == PM_CS_SM_AC || temp_product_class_id == PM_CS_SM_DC || temp_product_class_id == PM_CS_RSM_AC || temp_product_class_id == PM_CS_RSM_DC)
				TVINSERV_CS3000
			else if (temp_product_class_id == PM_PM5E || temp_product_class_id == PM_PM5E_ARM)
				TVINSERV_PM5E
			else if (temp_product_class_id == PM_THIRD_PARTY_DEVICE)
				TVINSERV_THIRD_PARTY
			else if (temp_product_class_id == PM_AFS)
				TVINSERV_SMALL_CASE
			else if (temp_product_class_id == PWM_TEMPERATURE_TRANSDUCER)
				TVINSERV_SMALL_CASE
			else if (temp_product_class_id == STM32_CO2_NODE)
				TVINSERV_CO2_NODE
			else
				TVINSERV_TSTAT_DEFAULT

				HTREEITEM hProductItem = pFrame_BM->m_pTreeViewCrl->InsertItem(&tvInsert);


			m_product_temp.product_item = hProductItem;

			HTREEITEM hParent = pFrame_BM->m_pTreeViewCrl->GetParentItem(hProductItem);
			if (hParent != NULL)
			{
				pFrame_BM->m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);
			}



			strSql = q.getValuebyName(L"Serial_ID");
			unsigned int correct_id = (DWORD)(_wtoi64(strSql));
			m_product_temp.serial_number = correct_id;

			if (hParent != NULL)
			{
				CString temp_cs_serial;
				temp_cs_serial.Format(_T("%u"), m_product_temp.serial_number);
				//如果父节点 是要求折叠的 就不要展开;
				m_product_temp.expand = (unsigned char)GetPrivateProfileInt(temp_cs_serial, _T("Expand"), 1, g_ext_database_path); //默认是都展开的;
				if (m_product_temp.expand != 2)
					pFrame_BM->m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);
				else
					pFrame_BM->m_pTreeViewCrl->Expand(hParent, TVE_COLLAPSE); //2就折叠
			}



			strSql = q.getValuebyName(L"Product_ID");
			m_product_temp.product_id = _wtoi(strSql);


			strSql = q.getValuebyName(L"NetworkCard_Address");
			m_product_temp.NetworkCard_Address = strSql;

			strSql = q.getValuebyName(L"Product_class_ID");
			m_product_temp.product_class_id = _wtoi(strSql);

			strSql = q.getValuebyName(L"Hardware_Ver");
			m_product_temp.hardware_version = (float)_wtof(strSql);


			strSql = q.getValuebyName(L"Software_Ver");
			m_product_temp.software_version = (float)_wtof(strSql);


			strSql = q.getValuebyName(L"EPsize");
			m_product_temp.nhardware_info = (int)_wtol(strSql);


			strSql = q.getValuebyName(L"Protocol");
			m_product_temp.protocol = (int)_wtoi(strSql);


			strSql = q.getValuebyName(L"Bautrate");
			if (_wtoi(strSql) != 0)
				m_product_temp.baudrate = _wtoi(strSql);
			else
				m_product_temp.baudrate = 19200;
			if (m_product_temp.protocol == PROTOCOL_BACNET_IP)
			{
				char cTemp1[255];
				memset(cTemp1, 0, 255);
				WideCharToMultiByte(CP_ACP, 0, strSql.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
				DWORD dwIP = inet_addr(cTemp1);

				m_product_temp.baudrate = dwIP;//_wtoi(strTemp);
			}
			else
				m_product_temp.baudrate = _wtoi(strSql);
			if (m_product_temp.product_class_id == PM_NC)
			{
				m_product_temp.baudrate = 19200;
			}
			////
			int is_online = q.getIntField("Online_Status");//
			if (is_online == 1)
			{
				m_product_temp.status = is_online;
				m_product_temp.status_last_time[4] = m_product_temp.status;
			}
			else
			{
				m_product_temp.status = false;
				m_product_temp.status_last_time[4] = false;
			}
			m_product_temp.status_last_time[0] = false;
			m_product_temp.status_last_time[1] = false;
			m_product_temp.status_last_time[2] = false;
			m_product_temp.status_last_time[3] = false;


			m_product_temp.Custom = q.getValuebyName(L"Custom");


			m_product_temp.NameShowOnTree = q.getValuebyName(L"Product_name");




			m_product_temp.BuildingInfo = pFrame_BM->m_subNetLst.at(pFrame_BM->m_nCurSubBuildingIndex);
			//20120423
			if (((strSql.CompareNoCase(_T("19200")) == 0)) ||
				(strSql.CompareNoCase(_T("9600")) == 0) ||
				(strSql.CompareNoCase(_T("38400")) == 0) ||
				(strSql.CompareNoCase(_T("57600")) == 0) ||
				(strSql.CompareNoCase(_T("115200")) == 0))
			{
				m_product_temp.BuildingInfo.strIp.Empty();
				m_product_temp.BuildingInfo.strBaudRate = strSql;
			}
			else
			{
				m_product_temp.BuildingInfo.strIp = strSql;
			}

			m_product_temp.ncomport = q.getIntField("Com_Port");//

			//m_product_temp.BuildingInfo.strIpPort = _T("10000");
			m_product_temp.BuildingInfo.strIpPort.Format(_T("%d"), m_product_temp.ncomport);

			m_product_temp.strImgPathName = q.getValuebyName(L"Background_imgID");

			//CStringArray temp_array_2;
			//SplitCStringA(temp_array_2, temp_ext, _T("^"));
			//if (temp_array_2.GetSize() == 0)
			//{
			//    m_product_temp.m_ext_info.virtual_device = 0;
			//}
			//else
			//{
			//    m_product_temp.m_ext_info.virtual_device = _wtoi(temp_array_2.GetAt(0).GetString());
			//}

			m_product_temp.note_parent_serial_number = q.getIntField("Parent_SerialNum");

			m_product_temp.panel_number = q.getIntField("Panal_Number");


			m_product_temp.object_instance = q.getIntField("Object_Instance");




			device_io_status temp_device_io_status;
			GetDeviceCountTable(m_product_temp.serial_number, BAC_READ_ALL_LIST, temp_device_io_status);

			for (int x = 0; x < TREE_MAX_TYPE - 1; x++) 
			{
				CString temp_cs;
				m_product_temp.sub_io_info[x].h_item_type = x;
				m_product_temp.sub_io_info[x].capacity = temp_device_io_status.device_capacity[x];
				m_product_temp.sub_io_info[x].already_use = temp_device_io_status.device_use[x];
				switch (x)
				{
					case TREE_OUT:
					{
						if (temp_device_io_status.device_capacity[x] == 0)
							temp_cs = _T("Output( ?/? )");
						else
							temp_cs.Format(_T("Output( %d/%d )"), temp_device_io_status.device_use[x], temp_device_io_status.device_capacity[x]);
						tvInsert.item.iImage = TREE_IMAGE_OUTPUT_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_OUTPUT_ONLINE;
					}
					break;
					case TREE_IN:
					{
						if (temp_device_io_status.device_capacity[x] == 0)
							temp_cs = _T("Input( ?/? )");
						else
							temp_cs.Format(_T("Input( %d/%d )"), temp_device_io_status.device_use[x], temp_device_io_status.device_capacity[x]);
						tvInsert.item.iImage = TREE_IMAGE_INPUT_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_INPUT_ONLINE;
					}
					break;
					case TREE_VAR:
					{
						if (temp_device_io_status.device_capacity[x] == 0)
							temp_cs = _T("Variable( ?/? )");
						else
							temp_cs.Format(_T("Variable( %d/%d )"), temp_device_io_status.device_use[x], temp_device_io_status.device_capacity[x]);
						tvInsert.item.iImage = TREE_IMAGE_VARIABLE_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_VARIABLE_ONLINE;
					}
						break;
					case TREE_PID:
					{
						if (temp_device_io_status.device_capacity[x] == 0)
							temp_cs = _T("Pid( ?/? )");
						else
							temp_cs.Format(_T("Pid( %d/%d )"), temp_device_io_status.device_use[x], temp_device_io_status.device_capacity[x]);
						tvInsert.item.iImage = TREE_IMAGE_PID_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_PID_ONLINE;
					}
						break;
					case TREE_SCH:
					{
						if (temp_device_io_status.device_capacity[x] == 0)
							temp_cs = _T("Schedule( ?/? )");
						else
							temp_cs.Format(_T("Schedule( %d/%d )"), temp_device_io_status.device_use[x], temp_device_io_status.device_capacity[x]);
						tvInsert.item.iImage = TREE_IMAGE_SCH_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_SCH_ONLINE;
					}
					break;
					case TREE_HOL:
					{
						if (temp_device_io_status.device_capacity[x] == 0)
							temp_cs = _T("Holiday( ?/? )");
						else
							temp_cs.Format(_T("Holiday( %d/%d )"), temp_device_io_status.device_use[x], temp_device_io_status.device_capacity[x]);
						tvInsert.item.iImage = TREE_IMAGE_HOLIDAY_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_HOLIDAY_ONLINE;
					}
					break;
					case TREE_PRG:
					{
						if (temp_device_io_status.device_capacity[x] == 0)
							temp_cs = _T("Program( ?/? )");
						else
							temp_cs.Format(_T("Program( %d/%d )"), temp_device_io_status.device_use[x], temp_device_io_status.device_capacity[x]);
						tvInsert.item.iImage = TREE_IMAGE_PROGRAM_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_PROGRAM_ONLINE;
					}
					break;
					case TREE_SCREEN:
					{
						if (temp_device_io_status.device_capacity[x] == 0)
							temp_cs = _T("Graphic( ?/? )");
						else
							temp_cs.Format(_T("Graphic( %d/%d )"), temp_device_io_status.device_use[x], temp_device_io_status.device_capacity[x]);
						tvInsert.item.iImage = TREE_IMAGE_SCREEN_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_SCREEN_ONLINE;
					}
					break;
					case TREE_TRENDLOG:
					{
						if (temp_device_io_status.device_capacity[x] == 0)
							temp_cs = _T("Trendlog( ?/? )");
						else
							temp_cs.Format(_T("Trendlog( %d/%d )"), temp_device_io_status.device_use[x], temp_device_io_status.device_capacity[x]);
						tvInsert.item.iImage = TREE_IMAGE_TRENDLOG_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_TRENDLOG_ONLINE;
					}
					break;
				default:
					break;
				}
				tvInsert.hParent = hProductItem;
				tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
				tvInsert.item.pszText = (LPTSTR)(LPCTSTR)temp_cs;
				tvInsert.hInsertAfter = TVI_LAST;// TVI_LAST; // 项目插入方式
				HTREEITEM hsubItem = pFrame_BM->m_pTreeViewCrl->InsertItem(&tvInsert);
				m_product_temp.sub_io_info[x].h_tree_item = hsubItem;
				m_product_temp.sub_io_info[x].h_parent_item = hProductItem;
			}
			CString temp_cs_serial;
			temp_cs_serial.Format(_T("%u"), m_product_temp.serial_number);
			//如果父节点 是要求折叠的 就不要展开;
			m_product_temp.expand = (unsigned char)GetPrivateProfileInt(temp_cs_serial, _T("Expand"), 1, g_ext_database_path); //默认是都展开的;
			if (m_product_temp.expand != 2)
				pFrame_BM->m_pTreeViewCrl->Expand(hProductItem, TVE_EXPAND);
			else
				pFrame_BM->m_pTreeViewCrl->Expand(hProductItem, TVE_COLLAPSE); //2就折叠

			pFrame_BM->m_product.push_back(m_product_temp);
			q.nextRow();

		}
	}


	strSql.Format(_T("select * from ALL_NODE where Building_Name = '%s'  and  Parent_SerialNum <> '0' and Parent_SerialNum <> '' "), strBuilding);
	q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);

	if (!q.eof())//就说明这个节点下面有 挂在父节点下面的;
	{
		while (!q.eof())
		{
			_variant_t temp_variant_parent;
			CString cs_parents_serial_number;
			unsigned int uint_p_serial_number;
			cs_parents_serial_number = q.getValuebyName(L"Parent_SerialNum");

			uint_p_serial_number = _wtoi(cs_parents_serial_number);
			bool find_parents = false;
			HTREEITEM parents_item;
			for (int z = 0; z < pFrame_BM->m_product.size(); z++)
			{
				if (uint_p_serial_number == pFrame_BM->m_product.at(z).serial_number)
				{
					find_parents = true;
					parents_item = pFrame_BM->m_product.at(z).product_item;
					break;
				}
			}
			tree_product m_product_temp = { 0 };
			CString temp_ext;
			temp_ext = q.getValuebyName(L"Screen_Name");
			pFrame_BM->GetExtProductInfo(m_product_temp, temp_ext);
			//if (b_building_management_flag == SYS_VIRTUAL_MODE)
			//{
			//    if (m_product_temp.m_ext_info.virtual_device == 0)
			//    {
			//        q.nextRow();
			//        continue;
			//    }
			//}

			if (find_parents)
			{
				CString strProdcut = q.getValuebyName(L"Product_name");
				tvInsert.hParent = parents_item; // 指定父句柄
				tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
				tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strProdcut;
				//TRACE(strProdcut);
				tvInsert.hInsertAfter = TVI_SORT;// TVI_LAST; // 项目插入方式


				int temp_product_class_id = q.getIntField("Product_class_ID");
				if (temp_product_class_id == PM_NC || temp_product_class_id == PM_SOLAR)
					TVINSERV_NET_WORK
				else if (temp_product_class_id == PM_PM5E || temp_product_class_id == PM_PM5E_ARM)
					TVINSERV_PM5E
				else if (temp_product_class_id == PM_CM5) //CM5
					TVINSERV_CMFIVE
				else if (temp_product_class_id == PM_T3_LC) //CM5
					TVINSERV_T3LC
				else if (temp_product_class_id == PM_T3PT10 ||
					temp_product_class_id == PM_T3IOA ||
					temp_product_class_id == PM_T332AI ||
					temp_product_class_id == PM_T38AI16O ||
					temp_product_class_id == PM_T38I13O ||
					temp_product_class_id == PM_T34AO ||
					temp_product_class_id == PM_T36CT) //T3
					TVINSERV_NET_WORK
				else if (
					temp_product_class_id == PM_T322AI ||
					temp_product_class_id == PM_T332AI_ARM ||
					temp_product_class_id == PM_T3PT12 ||
					temp_product_class_id == PM_T36CTA ||

					temp_product_class_id == PM_T38AI8AO6DO)
					TVINSERV_T3ARM
				else if (temp_product_class_id == PM_MINIPANEL || temp_product_class_id == PM_MINIPANEL_ARM || temp_product_class_id == PM_ESP32_T3_SERIES)//Mini Panel
					TVINSERV_MINIPANEL
				else if (temp_product_class_id == PM_AirQuality) //AirQuality
					TVINSERV_TSTAT_DEFAULT
				else if (temp_product_class_id == PM_LightingController)//Lightingcontroller
					TVINSERV_LC          //tree0412
				else if (temp_product_class_id == PM_TSTAT7 || temp_product_class_id == PM_TSTAT7_ARM)//TSTAT7 &TSTAT6 //tree0412
					TVINSERV_LED_TSTAT7 //tree0412
				else if (temp_product_class_id == PM_TSTAT6 || temp_product_class_id == PM_TSTAT5i)
					TVINSERV_TSTAT6
				else if (temp_product_class_id == PM_TSTAT10 ||
					temp_product_class_id == PM_TSTAT8 ||
					temp_product_class_id == PM_TSTAT9 ||
					temp_product_class_id == PM_TSTAT8_WIFI ||
					temp_product_class_id == PM_TSTAT8_OCC ||
					temp_product_class_id == PM_TSTAT_AQ ||
					temp_product_class_id == PM_AIRLAB_ESP32 ||
					temp_product_class_id == PM_TSTAT8_220V)
					TVINSERV_TSTAT8
				else if (temp_product_class_id == PM_MULTI_SENSOR)
					TVINSERV_TSTAT8   //暂且用TSTAT8 的图标
				else if (temp_product_class_id == PM_ZIGBEE_REPEATER)
					TVINSERV_T3_NANO
				else if ((temp_product_class_id == PM_CO2_NET) || (temp_product_class_id == PM_CO2_RS485) ||
					(temp_product_class_id == PM_PRESSURE_SENSOR) ||
					(temp_product_class_id == STM32_PRESSURE_NET) ||
					(temp_product_class_id == STM32_PRESSURE_RS485) ||
					(temp_product_class_id == STM32_CO2_NET) ||
					(temp_product_class_id == STM32_PM25) ||
					(temp_product_class_id == STM32_CO2_RS485) ||
					(temp_product_class_id == STM32_HUM_NET) ||    //2019 03 28 修复 HUM 使用默认TSTAT图标的bug
					(temp_product_class_id == STM32_HUM_RS485))
					TVINSERV_CO2
				else if (temp_product_class_id == PM_CS_SM_AC || temp_product_class_id == PM_CS_SM_DC || temp_product_class_id == PM_CS_RSM_AC || temp_product_class_id == PM_CS_RSM_DC)
					TVINSERV_CS3000
				else if (temp_product_class_id == PM_THIRD_PARTY_DEVICE)
					TVINSERV_THIRD_PARTY
				else if (temp_product_class_id == PM_AFS)
					TVINSERV_SMALL_CASE
				else if (temp_product_class_id == PWM_TEMPERATURE_TRANSDUCER)
					TVINSERV_SMALL_CASE
				else if (temp_product_class_id == STM32_CO2_NODE)
					TVINSERV_CO2_NODE
				else
					TVINSERV_TSTAT_DEFAULT

					HTREEITEM hSubItem = pFrame_BM->m_pTreeViewCrl->InsertItem(&tvInsert);

				HTREEITEM hParent = pFrame_BM->m_pTreeViewCrl->GetParentItem(hSubItem);
				//if (hParent != NULL)
				//	m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);






				m_product_temp.product_item = hSubItem;

				if (hParent != NULL)
				{
					CString temp_cs_serial;
					temp_cs_serial.Format(_T("%u"), uint_p_serial_number);
					//如果父节点 是要求折叠的 就不要展开;
					unsigned char temp_expand = 0;
					temp_expand = (unsigned char)GetPrivateProfileInt(temp_cs_serial, _T("Expand"), 1, g_ext_database_path); //默认是都展开的;
					if (temp_expand != 2)
						pFrame_BM->m_pTreeViewCrl->Expand(hParent, TVE_EXPAND);
					else
						pFrame_BM->m_pTreeViewCrl->Expand(hParent, TVE_COLLAPSE); //2就折叠
				}

				strSql = q.getValuebyName(L"Serial_ID");

				long temp_serial_id = (long)(_wtoi64(strSql));
				unsigned int correct_id = (DWORD)(_wtoi64(strSql));
				//用于将以前数据库中的 负的序列号 修改为正的;Add by Fance
				//if(temp_serial_id < 0)
				//{
				//	CString wrong_serial_id;
				//	wrong_serial_id = strSql;
				//	CString correct_serial_id;
				//	CString str_temp;
				//	correct_serial_id.Format(_T("%u"),correct_id);
				//	try
				//	{
				//		str_temp.Format(_T("update ALL_NODE set Serial_ID ='%s' where Serial_ID = '%s'"),correct_serial_id, wrong_serial_id);
				//		SqliteDBBuilding.execDML((UTF8MBSTR)str_temp);
				//	}
				//	catch(_com_error *e)
				//	{
				//		AfxMessageBox(e->ErrorMessage());
				//	}
				//}
				m_product_temp.serial_number = correct_id;


				strSql = q.getValuebyName(_T("Product_ID"));
				m_product_temp.product_id = _wtoi(strSql);

				strSql = q.getValuebyName(_T("Custom"));
				m_product_temp.Custom = strSql;

				strSql = q.getValuebyName(_T("NetworkCard_Address"));
				m_product_temp.NetworkCard_Address = strSql;

				strSql = q.getValuebyName(L"Product_class_ID");
				m_product_temp.product_class_id = _wtoi(strSql);

				strSql = q.getValuebyName(L"Hardware_Ver");
				m_product_temp.hardware_version = (float)_wtof(strSql);


				strSql = q.getValuebyName(L"Software_Ver");
				m_product_temp.software_version = (float)_wtof(strSql);


				strSql = q.getValuebyName(L"EPsize");
				m_product_temp.nhardware_info = (int)_wtol(strSql);


				strSql = q.getValuebyName(L"Protocol");
				m_product_temp.protocol = (int)_wtoi(strSql);


				strSql = q.getValuebyName(L"Bautrate");
				if (_wtoi(strSql) != 0)
					m_product_temp.baudrate = _wtoi(strSql);
				else
					m_product_temp.baudrate = 19200;
				if (m_product_temp.protocol == PROTOCOL_BACNET_IP)
				{
					char cTemp1[255];
					memset(cTemp1, 0, 255);
					WideCharToMultiByte(CP_ACP, 0, strSql.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
					DWORD dwIP = inet_addr(cTemp1);

					m_product_temp.baudrate = dwIP;//_wtoi(strTemp);
				}
				else
					m_product_temp.baudrate = _wtoi(strSql);
				if (m_product_temp.product_class_id == PM_NC)
				{
					m_product_temp.baudrate = 19200;
				}
				////
				int is_online = q.getIntField("Online_Status");//
				if (is_online == 1)
				{
					m_product_temp.status = is_online;
					m_product_temp.status_last_time[4] = m_product_temp.status;

				}
				else
				{
					m_product_temp.status = false;
					m_product_temp.status_last_time[4] = false;

				}
				m_product_temp.status_last_time[0] = false;
				m_product_temp.status_last_time[1] = false;
				m_product_temp.status_last_time[2] = false;
				m_product_temp.status_last_time[3] = false;



				m_product_temp.NameShowOnTree = q.getValuebyName(L"Product_name");//



				m_product_temp.BuildingInfo = pFrame_BM->m_subNetLst.at(pFrame_BM->m_nCurSubBuildingIndex);
				//20120423
				if (((strSql.CompareNoCase(_T("19200")) == 0)) || (strSql.CompareNoCase(_T("9600")) == 0) || ((strSql.CompareNoCase(_T("38400")) == 0)))
				{
					m_product_temp.BuildingInfo.strIp.Empty();
					m_product_temp.BuildingInfo.strBaudRate = strSql;
				}
				else
				{
					m_product_temp.BuildingInfo.strIp = strSql;
				}


				m_product_temp.ncomport = q.getIntField("Com_Port");
				m_product_temp.BuildingInfo.strIpPort = _T("10000");
				m_product_temp.strImgPathName = q.getValuebyName(L"Background_imgID");

				m_product_temp.note_parent_serial_number = q.getIntField("Parent_SerialNum");


				m_product_temp.panel_number = q.getIntField("Panal_Number");


				m_product_temp.object_instance = q.getIntField("Object_Instance");


				pFrame_BM->m_product.push_back(m_product_temp);

			}
			q.nextRow();
		}
	}

}


void CBacnetBuildingManagement::UpdateList()
{
	m_root_list.DeleteAllItems();

	nGroupCount = BuildingNode.m_child_count;//m_root_list.GetItemCount();
	for (int i = 0; i < nGroupCount; i++)
	{
		CString temp_cs;

		m_root_list.InsertItem(i, _T(" "));

		CString temp_input;
		CString temp_output;
		CString temp_variable;
		temp_input.Format(_T("%d"), BuildingNode.pchild[i]->m_input_count);
		temp_output.Format(_T("%d"), BuildingNode.pchild[i]->m_output_count);
		temp_variable.Format(_T("%d"), BuildingNode.pchild[i]->m_variable_count);
		m_root_list.SetItemText(i, BM_ROOT_TOTAL_INPUT, temp_input);
		m_root_list.SetItemText(i, BM_ROOT_TOTAL_OUTPUT, temp_output);
		m_root_list.SetItemText(i, BM_ROOT_TOTAL_VARIABLE, temp_variable);

		m_root_list.SetItemText(i, BM_ROOT_GROUP_NAME, BuildingNode.pchild[i]->m_csName);
	}
}

void CBacnetBuildingManagement::OnBnClickedButtonBmSave()
{
	// TODO: 在此添加控件通知处理程序代码
	SaveAllIntoIniFile();
}

void CBacnetBuildingManagement::SetDataTreeCtrl()
{
	pFrame_BM->m_pTreeViewCrl->m_BMpoint = this;
}


void CBacnetBuildingManagement::OnBnClickedButtonDbDone()
{
	// TODO: 在此添加控件通知处理程序代码
	LoadFileShowToList();
	::PostMessage(h_db_io_hwnd,WM_REFRESH_BAC_BUILDING_IO_LIST, NULL, NULL);
}
