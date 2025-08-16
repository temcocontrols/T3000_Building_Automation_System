// CBacnetBuildingProperty.cpp: Implementation file
// CBacnetBuildingProperty.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CBacnetBuildingProperty.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CBacnetBuildingProperty dialog
// CBacnetBuildingProperty 对话框
extern BM_nodeinfo operation_nodeinfo;
IMPLEMENT_DYNAMIC(CBacnetBuildingProperty, CDialogEx)

CBacnetBuildingProperty::CBacnetBuildingProperty(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BACNET_BUILDING_PROPERTY, pParent)
{

}

CBacnetBuildingProperty::~CBacnetBuildingProperty()
{
}

void CBacnetBuildingProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DB_PROPERTY, m_property_list);
}


BEGIN_MESSAGE_MAP(CBacnetBuildingProperty, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DB_PROPERTY_OK, &CBacnetBuildingProperty::OnBnClickedButtonDbPropertyOk)
END_MESSAGE_MAP()


// CBacnetBuildingProperty message handlers
// CBacnetBuildingProperty 消息处理程序


BOOL CBacnetBuildingProperty::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: Add extra initialization here
	// TODO:  在此添加额外的初始化
	Initial_List();

	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	CBacnetBuildingManagement* temp_BMpoint;
	temp_BMpoint = pFrame->m_pTreeViewCrl->m_BMpoint;
	CString cs_group_name;
	CString cs_category_name;
	CBacnetBMD* group_point;
	CBacnetBMD* node_point;
	group_point = temp_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group];
	cs_group_name = group_point->m_csName;
	node_point = group_point->pchild[operation_nodeinfo.child_device];
	cs_category_name = node_point->m_csName;
	GetDlgItem(IDC_EDIT_GROUP_NAME)->SetWindowTextW(cs_group_name);
	GetDlgItem(IDC_EDIT_CATEGORY_NAME)->SetWindowTextW(cs_category_name);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // Exception: OCX property pages should return FALSE
				  // 异常: OCX 属性页应返回 FALSE
}


void CBacnetBuildingProperty::OnBnClickedButtonDbPropertyOk()
{
	// TODO: Add your control notification handler code here
	// TODO: 在此添加控件通知处理程序代码
}

const int PROPERTY_NUM = 0;
const int PROPERTY_TYPE = 1;
const int PROPERTY_COUNT = 2;

void CBacnetBuildingProperty::Initial_List()
{
	m_property_list.ShowWindow(SW_HIDE);
	m_property_list.DeleteAllItems();
	while (m_property_list.DeleteColumn(0));

	m_property_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	//m_property_list.SetExtendedStyle(m_property_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_property_list.SetExtendedStyle(m_property_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_property_list.InsertColumn(PROPERTY_NUM, _T("Num"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_property_list.InsertColumn(PROPERTY_TYPE, _T("Item Type"), 150, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_property_list.InsertColumn(PROPERTY_COUNT, _T("Count"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);


	m_property_list.SetListHwnd(this->m_hWnd);
	CRect list_rect, win_rect;
	m_property_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_pragram_dlg_hwnd, win_rect);
	m_property_list.Set_My_WindowRect(win_rect);
	m_property_list.Set_My_ListRect(list_rect);

	m_property_list.DeleteAllItems();
#if 0
	for (int i = 0; i < (int)m_Program_data.size(); i++)
	{
		CString temp_item, temp_value, temp_cal, temp_filter, temp_status, temp_lable;
		CString temp_des;
		CString temp_units;

		if (i >= program_item_limit_count)
			break;


		temp_item.Format(_T("%d"), i + 1);
		m_property_list.InsertItem(i, temp_item);
		m_property_list.SetCellEnabled(i, 0, 0);
		m_property_list.SetCellChecked(0, 0, 1);// default will set the first one checked
		if (ListCtrlEx::ComboBox == m_property_list.GetColumnType(PROGRAM_AUTO_MANUAL))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(Auto_Manual[0]);
			strlist.push_back(Auto_Manual[1]);
			m_property_list.SetCellStringList(i, PROGRAM_AUTO_MANUAL, strlist);
		}

		if (ListCtrlEx::ComboBox == m_property_list.GetColumnType(PROGRAM_STATUS))
		{
			ListCtrlEx::CStrList strlist;
			strlist.push_back(ProgramStatus[0]);
			strlist.push_back(ProgramStatus[1]);
			m_property_list.SetCellStringList(i, PROGRAM_STATUS, strlist);
		}

		for (int x = 0; x < PROGRAM_COL_NUMBER; x++)
		{
			if ((i % 2) == 0)
				m_property_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_property_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
		}

	}
	m_property_list.InitListData();
	m_property_list.SetCellChecked(0, 0, 1);
#endif
	m_property_list.ShowWindow(SW_SHOW);
}
