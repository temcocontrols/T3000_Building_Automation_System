// CBacnetBuildingIOPoints.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CBacnetBuildingIOPoints.h"
#include "afxdialogex.h"


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
END_MESSAGE_MAP()


void CBacnetBuildingIOPoints::Fresh()
{
	Initial_List();
	Fresh_Building_IO_Test_List();
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
	m_io_list.InsertColumn(BM_IO_NAME, _T("Name"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_io_list.InsertColumn(BM_IO_FULL_LABLE, _T("Full Description"), 150, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_io_list.InsertColumn(BM_IO_AUTO_MANUAL, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_io_list.InsertColumn(BM_IO_VALUE, _T("Value"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_io_list.InsertColumn(BM_IO_UNITE, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_io_list.InsertColumn(BM_IO_RANGE, _T("Range"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_io_list.InsertColumn(BM_IO_DECOM, _T("Status"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_io_list.InsertColumn(BM_IO_TYPE, _T("Type"), 100, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);

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

void CBacnetBuildingIOPoints::Fresh_Building_IO_Test_List()
{
	for (int i = 0; i < 5; i++)
	{
		CString csBM_IO_NAME;
		CString cs_IO_FULL_LABLE ;
		CString cs_IO_AUTO_MANUAL ;
		CString cs_IO_VALUE = _T("0.00");
		CString cs_IO_UNITE = _T("");
		CString cs_IO_RANGE = _T("Unused");
		CString cs_IO_DECOM = _T("Normal");
		CString cs_BM_IO_TYPE;
		csBM_IO_NAME.Format(_T("IN%d"), i + 1);
		cs_IO_FULL_LABLE.Format(_T("Input%d"), i + 1);

		if (i % 2 == 0)
		{
			cs_BM_IO_TYPE = _T("Digital");
			cs_IO_AUTO_MANUAL = _T("Auto");
		}
		else
		{
			cs_BM_IO_TYPE = _T("Analog");
			cs_IO_AUTO_MANUAL = _T("Manual");
		}



		CString temp_item;
		temp_item.Format(_T("%d"), i + 1);
		m_io_list.InsertItem(i, temp_item);
		m_io_list.SetItemText(i, BM_IO_NAME, csBM_IO_NAME);
		m_io_list.SetItemText(i, BM_IO_FULL_LABLE, cs_IO_FULL_LABLE);
		m_io_list.SetItemText(i, BM_IO_AUTO_MANUAL, cs_IO_AUTO_MANUAL);
		m_io_list.SetItemText(i, BM_IO_VALUE, cs_IO_VALUE);
		m_io_list.SetItemText(i, BM_IO_UNITE, cs_IO_UNITE);
		m_io_list.SetItemText(i, BM_IO_RANGE, cs_IO_RANGE);
		m_io_list.SetItemText(i, BM_IO_DECOM, cs_IO_DECOM);
		m_io_list.SetItemText(i, BM_IO_TYPE, cs_BM_IO_TYPE);


	}

}

LRESULT CBacnetBuildingIOPoints::Fresh_Building_IO_List(WPARAM wParam, LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);
	int Fresh_Item;
	int isFreshOne = (int)lParam;

	return 0;
}