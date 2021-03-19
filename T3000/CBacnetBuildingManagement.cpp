// CBacnetBuildingManagement.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CBacnetBuildingManagement.h"
#include "MainFrm.h"
CMainFrame* pFrame_BM;
// CBacnetBuildingManagement
CString cs_bm_ini;
IMPLEMENT_DYNCREATE(CBacnetBuildingManagement, CDialogEx)

CBacnetBuildingManagement::CBacnetBuildingManagement()
	: CDialogEx(IDD_DIALOG_BACNET_BUILDING_MANAGEMENT)
{
	memset(n_group_count, 0, 255);
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
	b_Building_Management_Flag = true;
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
		n_group_count[i] = GetPrivateProfileInt(temp_lpAppname, _T("GroupChildCount"), 0,  cs_bm_ini);
		CString temp_group_count;
		//temp_group_count.Format(_T("%d"), n_group_count[i]);
		m_root_list.SetItemText(i, 1, groupname[i]);
		//m_root_list.SetItemText(i, 2, temp_group_count);

	}
	TreeInital();
}

void CBacnetBuildingManagement::ShowListToTree()
{
	nGroupCount = m_root_list.GetItemCount();
	for (int i = 0; i < nGroupCount; i++)
	{
		CString temp_group_count;
		groupname[i] = m_root_list.GetItemText(i, 1);
		temp_group_count = m_root_list.GetItemText(i, 2);
		n_group_count[i] = _wtoi(temp_group_count);
	}

	if (nGroupCount == 0)
	{
		MessageBox(_T("Please add device"));
		return;
	}

	TreeInital();
}


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
		BuildingNode.pchild[i]->m_child_count = n_group_count[i];
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
				section_io_name.Format(_T("IO_%d_%d_Name"), j, z);
				section_io_type.Format(_T("IO_%d_%d_Type"), j, z);
				CString io_name;
				int io_type;
				GetPrivateProfileString(temp_lpAppname, section_io_name, _T(""), io_name.GetBuffer(MAX_PATH), MAX_PATH, cs_bm_ini);
				io_name.ReleaseBuffer();
				io_type = GetPrivateProfileInt(temp_lpAppname, section_io_type, 0, cs_bm_ini);

				tvInsert.hParent = hTreeABC123; // 指定父句柄
				tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
				tvInsert.item.pszText = (LPTSTR)(LPCTSTR)io_name;
				tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式

				//暂时用input
				if (io_type == TYPE_BM_INPUT)
				{
					temp_bmd_point->m_input_count++;
					tvInsert.item.iImage = TREE_IMAGE_INPUT_UNKNOWN; tvInsert.item.iSelectedImage = TREE_IMAGE_INPUT_UNKNOWN;
				}
				else if (io_type == TYPE_BM_OUTPUT)
				{
					temp_bmd_point->m_output_count++;
					tvInsert.item.iImage = TREE_IMAGE_OUTPUT_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_OUTPUT_ONLINE;
				}
				else if (io_type == TYPE_BM_VARIABLE)
				{
					temp_bmd_point->m_variable_count++;
					tvInsert.item.iImage = TREE_IMAGE_VARIABLE_OFFLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_VARIABLE_OFFLINE;
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
