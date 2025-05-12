// CBacnetBuildingMain.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CBacnetBuildingMain.h"
#include "MainFrm.h"

// CBacnetBuildingMain
extern CMainFrame* pFrame_BM;
extern CString cs_bm_ini;
IMPLEMENT_DYNCREATE(CBacnetBuildingMain, CFormView)

CBacnetBuildingMain::CBacnetBuildingMain()
	: CFormView(IDD_DIALOG_BACNET_BUILDING_MAIN)
{

}

CBacnetBuildingMain::~CBacnetBuildingMain()
{
}

void CBacnetBuildingMain::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_BUILDING_MAIN, m_main_building_tab);
}

BEGIN_MESSAGE_MAP(CBacnetBuildingMain, CFormView)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_BUILDING_MAIN, &CBacnetBuildingMain::OnTcnSelchangeTabBuildingMain)
END_MESSAGE_MAP()


// CBacnetBuildingMain 诊断

#ifdef _DEBUG
void CBacnetBuildingMain::AssertValid() const
{
    CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBacnetBuildingMain::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CBacnetBuildingMain::InitBuildingFloor()
{
    bm_floor.clear();
    bm_room.clear();
    CString temp_floor_manage;
    temp_floor_manage = _T("FloorInfo");
    CString csfloortotalindex;
    GetPrivateProfileString(temp_floor_manage, _T("FloorIndex"), _T(""), csfloortotalindex.GetBuffer(MAX_PATH), MAX_PATH, cs_bm_ini);
    csfloortotalindex.ReleaseBuffer();
    if (csfloortotalindex.IsEmpty())
    {
        FloorInfo temp;
        temp.csName = _T("Default Floor");
        temp.xindex = 0;
        bm_floor.push_back(temp);
        RoomInfo temproom;
        temproom.nFloor = temp;
        temproom.csName = _T("Default Room");
        temproom.yindex = 0;
        bm_room.push_back(temproom);
    }
    else
    {
        Sleep(1);
    }



}

void CBacnetBuildingMain::Fresh()
{
    pFrame_BM = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
    cs_bm_ini = g_strExePth + _T("Database\\Buildings\\") + pFrame_BM->m_strCurMainBuildingName + _T("\\BuildingManagement.ini");
    InitBuildingFloor();
    //添加代码 加载楼层房间的 名字 
    static int inital_once = false;
    if (inital_once == false)
    {
        InitialTab();
        inital_once = 1;
    }
    m_page_building_info.Fresh();
    m_page_building_io.Fresh();
   // InitScrollbar();
}

void CBacnetBuildingMain::OnHTreeItemClick(NMHDR* pNMHDR, LRESULT* pResult)
{
    pFrame_BM->OnHTreeItemClick(pNMHDR, pResult);
}



void CBacnetBuildingMain::InitialTab()
{
    m_main_building_tab.InsertItem(BUILDING_IO, _T("IO Points    "));
    m_main_building_tab.InsertItem(BUILDING_BASIC, _T("IO Information      "));

    //m_main_building_tab.InsertItem(BUILDING_COMMUNICATION, _T("Communication          "));

    //m_main_building_tab.InsertItem(SETTING_WIFI, _T("Wifi           "));
    //创建两个对话框
    m_page_building_info.Create(IDD_DIALOG_BACNET_BUILDING_MANAGEMENT, &m_main_building_tab);
    m_page_building_io.Create(IDD_DIALOG_BACNET_BUILDING_IO_POINT, &m_main_building_tab);
   
    



    //设定在Tab内显示的范围
    CRect rc;
    m_main_building_tab.GetClientRect(rc);
    rc.top += 21;
    rc.bottom -= 2;
    rc.left += 2;
    rc.right -= 4;
    m_page_building_info.MoveWindow(&rc);
    m_page_building_io.MoveWindow(&rc);
#if 1
    //m_page_wifi.MoveWindow(&rc);
    //把对话框对象指针保存起来
    building_tab_dialog[BUILDING_IO] = &m_page_building_io;
    building_tab_dialog[BUILDING_BASIC] = &m_page_building_info;


    //tab_dialog[SETTING_WIFI] = &m_page_wifi;
    //显示初始页面
    building_tab_dialog[BUILDING_IO]->ShowWindow(SW_SHOW);
    building_tab_dialog[BUILDING_BASIC]->ShowWindow(SW_HIDE);


    //tab_dialog[SETTING_WIFI]->ShowWindow(SW_HIDE);
    //保存当前选择
    m_CurSelTab = 0;
#endif
}

void CBacnetBuildingMain::InitScrollbar()
{
    SCROLLINFO scrollinfo;
    GetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
    scrollinfo.nPage = 20;    //设置滑块大小
    scrollinfo.nMax = 75;     //设置滚动条的最大位置0--75
    SetScrollInfo(SB_VERT, &scrollinfo, SIF_ALL);
}


void CBacnetBuildingMain::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    // TODO: 在此添加专用代码和/或调用基类
}


void CBacnetBuildingMain::OnTcnSelchangeTabBuildingMain(NMHDR* pNMHDR, LRESULT* pResult)
{
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;

    building_tab_dialog[m_CurSelTab]->ShowWindow(SW_HIDE);
    //得到新的页面索引
    m_CurSelTab = m_main_building_tab.GetCurSel();
    building_tab_dialog[m_CurSelTab]->ShowWindow(SW_SHOW);
}
