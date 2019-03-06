// BuildingConfigration.cpp : implementation file
//
#include "stdafx.h"
#include "T3000.h"
#include "BuildingConfigration.h"
#include "afxdialogex.h"
#include "BacnetRemotePortWarning.h"
 
#include <iostream>
#include <windows.h>
#include <io.h>
#include "FileOperations.h"
#include "BuildingConfigEditDlg.h"
#include "BacnetAddRemoteDevice.h"
#include "global_function.h"
#include "ConnectRemoteServer.h"
#include "RemotePtpLogin.h"
#include <ctime>
#include <iostream>
#include "../SQLiteDriver/sqlite3.h"
using namespace std;
// CBuildingConfigration dialog
#define WM_FRESH_DB  WM_USER + 1014
#define PATH_MAX 256
IMPLEMENT_DYNAMIC(CBuildingConfigration, CDialogEx)


const int INDEX_MODBUS_485 = 0;
const int INDEX_MODBUS_TCP = 1;
const int INDEX_BACNET_MSTP = 2;
const int INDEX_REMOTE_DEVICE = 3;
const int INDEX_AUTO = 4;
extern unsigned int try_connect_serial;
bool ptp_cancel_login = false;

CBuildingConfigration::CBuildingConfigration(CWnd* pParent /*=NULL*/)
    : CDialogEx(CBuildingConfigration::IDD, pParent)
{
    m_bChanged=FALSE;
	 
	 
}

CBuildingConfigration::~CBuildingConfigration()
{
}

void CBuildingConfigration::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_BUILDING_CONFIG, m_building_config_list);
    DDX_Control(pDX, IDC_BUILDING_BUTTON_ADD, m_addBuildingButton);
    DDX_Control(pDX, IDC_BUILDING_BUTTON_DELETE, m_deleteBuildingButton);
}

BEGIN_MESSAGE_MAP(CBuildingConfigration, CDialogEx)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Building_Config_Item)
    ON_BN_CLICKED(IDC_BUILDING_BUTTON_ADD, &CBuildingConfigration::OnBnClickedBuildingButtonAdd)
    ON_BN_CLICKED(IDC_BUILDING_BUTTON_DELETE, &CBuildingConfigration::OnBnClickedBuildingButtonDelete)
    ON_WM_SIZE()
    ON_NOTIFY(NM_CLICK, IDC_LIST_BUILDING_CONFIG, &CBuildingConfigration::OnNMClickListBuildingConfig)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_BUILDING_CONFIG, &CBuildingConfigration::OnNMRClickListBuildingConfig)
    ON_COMMAND(ID_BUILDINGCONFIG_SELECT, &CBuildingConfigration::OnBuildingconfigSelect)
    ON_COMMAND(ID_BUILDINGCONFIG_UNSELECT, &CBuildingConfigration::OnBuildingconfigUnselect)
    ON_COMMAND(ID_BUILDINGCONFIG_DELETE, &CBuildingConfigration::OnBuildingconfigDelete)
    ON_BN_CLICKED(IDOK, &CBuildingConfigration::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BUILDING_CONFIG, &CBuildingConfigration::OnNMDblclkListBuildingConfig)
END_MESSAGE_MAP()


// CBuildingConfigration message handlers


BOOL CBuildingConfigration::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    Deal_BuildingPath();
   
    
    GetSerialComPortNumber1(m_szBuildingComs);
	m_SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
    m_building_config_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    //m_building_config_list.SetExtendedStyle(m_building_config_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
    m_building_config_list.SetExtendedStyle(m_building_config_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_building_config_list.InsertColumn(BC_ITEM, _T(""), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    m_building_config_list.InsertColumn(BC_MAINNAME, _T("Building"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_building_config_list.InsertColumn(BC_PROTOCOL, _T("Protocol"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_building_config_list.InsertColumn(BC_IPADDRESS, _T("IP/Domain/Tel#/SerialNumber"), 150, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_building_config_list.InsertColumn(BC_IPPORT, _T("Modbus TCP Port"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_building_config_list.InsertColumn(BC_COMPORT, _T("COM Port"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_building_config_list.InsertColumn(BC_BAUDRATE, _T("Baud Rate"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_building_config_list.InsertColumn(BC_BUILDINGPATH, _T("Building Path"), 600, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

    m_building_config_hwnd = this->m_hWnd;
    g_hwnd_now = m_building_config_hwnd;


    CRect list_rect,win_rect;
    m_building_config_list.GetWindowRect(list_rect);
    ScreenToClient(&list_rect);
    ::GetWindowRect(m_input_dlg_hwnd,win_rect);
    m_building_config_list.Set_My_WindowRect(win_rect);
    m_building_config_list.Set_My_ListRect(list_rect);


    Initial_Building_List();
    

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBuildingConfigration::PreTranslateMessage(MSG* pMsg)
{
    
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)
    {
        CRect list_rect,win_rect;
        m_building_config_list.GetWindowRect(list_rect);
        ScreenToClient(&list_rect);
        ::GetWindowRect(m_pragram_dlg_hwnd,win_rect);
        m_building_config_list.Set_My_WindowRect(win_rect);
        m_building_config_list.Set_My_ListRect(list_rect);

        m_building_config_list.Get_clicked_mouse_position();
        return TRUE;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}


void CBuildingConfigration::Initial_Building_List()
{

    CppSQLite3DB SqliteDBBuilding;
    CppSQLite3Query q;
    LoadBuildingConfigDB();
    TraverseFolder(g_strBuildingFolder,m_vecdbfile);
    for (int i=0; i<(int)m_vecdbfile.size(); i++)
    {
        int index=m_vecdbfile.at(i).Find(_T("Database"));
        m_vecdbfile.at(i).Delete(0,index);

    }
    Building_Config BCTemp;
    CStringArray ArrayFileName;
    BOOL Is_The_Same=TRUE;
    for (int i=0; i<m_vecdbfile.size(); i++)
    {
        for (int j=0; j<m_BuildNameLst.size(); j++)
        {
            if (m_vecdbfile.at(i).CompareNoCase(m_BuildNameLst.at(j).BuildingPath)==0)
            {
                Is_The_Same=TRUE;
                break;
            }
            else
            {
                Is_The_Same=FALSE;
            }
        }
        if (Is_The_Same)
        {
            continue;
        }
        //1.拷贝到当前目录下：新的Building 加入到数据库中
        //2.把数据库的名字改掉了：把数据库的路径更改掉
        //3.把Building的目录名字改掉了：当成一个新的Building
        CString sql;
        _variant_t temp_variant;
        CString Building_Name=L"";
        CString DB_Building_Name=L"";
        if (!Is_The_Same)
        {
            SplitCStringA(ArrayFileName,m_vecdbfile.at(i),L".");
            if (ArrayFileName[ArrayFileName.GetSize()-1].CompareNoCase(L"db")!=0)
            {
                continue;
            }
#if 1 //校驗一下看看是否是Building的Database
            CString strpath = GetExePath(true)+m_vecdbfile.at(i);
            SqliteDBBuilding.open((UTF8MBSTR)strpath);
             
            if (!SqliteDBBuilding.tableExists("ALL_NODE"))
            {

                 SqliteDBBuilding.closedb();
                continue;
            }
            else
            {

                sql.Format(_T("Select  distinct mainbuilding_name  from  ALL_NODE"));
                q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
                if(!q.eof())
                {
                        DB_Building_Name=q.getValuebyName(L"MainBuilding_Name");
                }
                 

            }
#endif
            SplitCStringA(ArrayFileName,m_vecdbfile.at(i),L"\\");
            if (ArrayFileName.GetSize()<2)
            {
                break;
            }

            Building_Name=ArrayFileName[ArrayFileName.GetSize()-2];
            if(DB_Building_Name.CompareNoCase(Building_Name)!=0)
            {
                sql.Format(_T("Select * from ALL_NODE"));
                q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
                while(!q.eof())
                {

                    sql.Format(_T("update ALL_NODE set MainBuilding_Name  = '%s' ,Building_Name = '%s'  where  MainBuilding_Name  = '%s'"),Building_Name,Building_Name,DB_Building_Name);
                    SqliteDBBuilding.execDML((UTF8MBSTR)sql);
                   q.nextRow();

                }
               
            }
           SqliteDBBuilding.closedb();


            //}

            BCTemp.MainBuildingName=Building_Name;
            BCTemp.Sub_NetName=Building_Name;
            BCTemp.BuildingPath=m_vecdbfile.at(i);
            BCTemp.b_selected=FALSE;

            //更新ALL_NODE 里面的数据

			CppSQLite3DB SqliteDBT3000;
			SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
            

            sql.Format(_T("Select *  from  Building  where  Main_BuildingName='%s' "),BCTemp.MainBuildingName);
           q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);
            if (q.eof())//有表但是没有对应序列号的值
            {
                 
                for (int j=0; j<m_BuildNameLst.size(); j++)
                {
                    if (BCTemp.MainBuildingName.CompareNoCase(m_BuildNameLst.at(j).MainBuildingName)==0)
                    {
                        Is_The_Same=TRUE;
                        break;

                    }
                    else
                    {
                        Is_The_Same=FALSE;
                    }
                }
                if (!Is_The_Same)
                {
                    sql.Format(_T("delete   from Building_ALL where Building_Name = '%s' "),BCTemp.MainBuildingName.GetBuffer());
                    SqliteDBT3000.execDML((UTF8MBSTR)sql);

                    sql.Format(_T("Insert into Building_ALL(Building_Name,Default_Build) values('%s','%d')"),BCTemp.MainBuildingName.GetBuffer(),0);
                    SqliteDBT3000.execDML((UTF8MBSTR)sql);

                    sql.Format(_T("Insert into Building(Main_BuildingName,Building_Name,Default_SubBuilding,Building_Path) values('%s','%s','%d','%s')"),BCTemp.MainBuildingName.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),0,BCTemp.BuildingPath.GetBuffer());
                    SqliteDBT3000.execDML((UTF8MBSTR)sql);
                }
                else
                {
                    sql.Format(_T("delete   from Building_ALL where Building_Name = '%s' "),BCTemp.MainBuildingName.GetBuffer());
                    SqliteDBT3000.execDML((UTF8MBSTR)sql);

                    sql.Format(_T("Insert into Building_ALL(Building_Name,Default_Build) values('%s','%d')"),BCTemp.MainBuildingName.GetBuffer(),0);
                    SqliteDBT3000.execDML((UTF8MBSTR)sql);

                    sql.Format(_T("update Building set Building_Path = '%s' ,Building_Name = '%s'  where  Main_BuildingName = '%s' "),BCTemp.BuildingPath.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),BCTemp.MainBuildingName.GetBuffer());

                    SqliteDBT3000.execDML((UTF8MBSTR)sql);

                }

            }
            else
            {
 
            }
            SqliteDBT3000.closedb();
            //更新到数据库之后，加入到Vector列表
            m_BuildNameLst.push_back(BCTemp);

        }

    }
    //重新加载数据
    LoadBuildingConfigDB();
    m_building_config_list.DeleteAllItems();
    for (int i=0; i<(int)m_BuildNameLst.size(); i++)
    {
        CString temp_item;
        temp_item.Format(_T("%d"),i+1);
        m_building_config_list.InsertItem(i,_T(""));

        m_building_config_list.SetItemText(i,BC_MAINNAME,m_BuildNameLst.at(i).MainBuildingName);


        if(m_BuildNameLst.at(i).Protocol.CompareNoCase(_T("Auto")) == 0) // AUTO时 需要使能的表格
        {
            m_building_config_list.SetCellEnabled(i,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(i,BC_IPPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_COMPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_BAUDRATE,0);
        }

        if(m_BuildNameLst.at(i).Protocol.CompareNoCase(_T("Remote Device")) == 0)   // == REMOTE_DEVICE  需要使能的表格
        {
            m_building_config_list.SetCellEnabled(i,BC_IPADDRESS,1);
            m_building_config_list.SetCellEnabled(i,BC_IPPORT,1);
            m_building_config_list.SetCellEnabled(i,BC_COMPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_BAUDRATE,0);
        }

		if ((m_BuildNameLst.at(i).Protocol.CompareNoCase(_T("Modbus 485")) == 0) ||     //   MODBUS_485
			(m_BuildNameLst.at(i).Protocol.CompareNoCase(_T("Bacnet MSTP")) == 0))      //   BACNET_MSTP
		{
            m_building_config_list.SetCellEnabled(i,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(i,BC_IPPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_COMPORT,1);
            m_building_config_list.SetCellEnabled(i,BC_BAUDRATE,1);
        }
        if(m_BuildNameLst.at(i).b_selected)
            m_building_config_list.SetItemText(i,BC_ITEM,_T("Selected"));
        else
            m_building_config_list.SetItemText(i,BC_ITEM,_T(""));


        CString m_show_protocol;
		m_show_protocol = m_BuildNameLst.at(i).Protocol;
        //if(m_BuildNameLst.at(i).protocol_index == INDEX_MODBUS_485)
        //    m_show_protocol = _T("Modbus 485");
        //else if(m_BuildNameLst.at(i).protocol_index == INDEX_MODBUS_TCP)
        //    m_show_protocol = _T("Modbus TCP");
        //else if(m_BuildNameLst.at(i).protocol_index == INDEX_BACNET_MSTP)
        //    m_show_protocol = _T("Bacnet MSTP");
        //else if(m_BuildNameLst.at(i).protocol_index == INDEX_REMOTE_DEVICE)
        //    m_show_protocol = _T("Remote Device");
        //else
        //    m_show_protocol = _T("Auto");

        m_building_config_list.SetItemText(i,BC_PROTOCOL,m_show_protocol);
        m_building_config_list.SetItemText(i,BC_IPADDRESS,m_BuildNameLst.at(i).IPAddress_Domain);
        m_building_config_list.SetItemText(i,BC_IPPORT,m_BuildNameLst.at(i).IP_Port);
        m_building_config_list.SetItemText(i,BC_COMPORT,m_BuildNameLst.at(i).Comport);
        m_building_config_list.SetItemText(i,BC_BAUDRATE,m_BuildNameLst.at(i).BaudRate);
        m_building_config_list.SetItemText(i,BC_BUILDINGPATH,m_BuildNameLst.at(i).BuildingPath);

        if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_PROTOCOL))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0; j<(int)sizeof(Building_Protocol)/sizeof(Building_Protocol[0]); j++)
            {
                strlist.push_back(Building_Protocol[j]);
            }
            m_building_config_list.SetCellStringList(i, BC_PROTOCOL, strlist);
        }

        if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_BAUDRATE))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0; j<(int)sizeof(Building_Baudrate)/sizeof(Building_Baudrate[0]); j++)
            {
                strlist.push_back(Building_Baudrate[j]);
            }
            m_building_config_list.SetCellStringList(i, BC_BAUDRATE, strlist);
        }


        if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_COMPORT))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0; j<m_szBuildingComs.size(); j++)
            {
                strlist.push_back(m_szBuildingComs.at(j));
            }
            m_building_config_list.SetCellStringList(i, BC_COMPORT, strlist);
        }

// 		for (int x=0;x<BC_COL_NUMBER;x++)
// 		{
// 			if((i%2)==0)
// 				m_building_config_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
// 			else
// 				m_building_config_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
// 		}
    }

    int last_new_item = (int)m_BuildNameLst.size();
    CString temp_cs;
    temp_cs.Format(_T("%d"),last_new_item + 1);
    m_building_config_list.InsertItem(last_new_item,_T(""));
// 	for (int x=0;x<BC_COL_NUMBER;x++)
// 	{
// 		if((last_new_item%2)==0)
// 			m_building_config_list.SetItemBkColor(last_new_item,x,LIST_ITEM_DEFAULT_BKCOLOR);
// 		else
// 			m_building_config_list.SetItemBkColor(last_new_item,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
// 	}

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_PROTOCOL))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<(int)sizeof(Building_Protocol)/sizeof(Building_Protocol[0]); j++)
        {
            strlist.push_back(Building_Protocol[j]);
        }
        m_building_config_list.SetCellStringList(last_new_item, BC_PROTOCOL, strlist);
    }

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_BAUDRATE))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<(int)sizeof(Building_Baudrate)/sizeof(Building_Baudrate[0]); j++)
        {
            strlist.push_back(Building_Baudrate[j]);
        }
        m_building_config_list.SetCellStringList(last_new_item, BC_BAUDRATE, strlist);
    }

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_COMPORT))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<m_szBuildingComs.size(); j++)
        {
            strlist.push_back(m_szBuildingComs.at(j));
        }
        m_building_config_list.SetCellStringList(last_new_item, BC_COMPORT, strlist);
    }



}

void CBuildingConfigration::Fresh_List_Row()
{
    LoadBuildingConfigDB();

    m_building_config_list.ShowWindow(SW_HIDE);

    m_building_config_list.DeleteAllItems();

    //while ( m_building_config_list.DeleteColumn (0)) ;

    //m_building_config_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    ////m_building_config_list.SetExtendedStyle(m_building_config_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
    //m_building_config_list.SetExtendedStyle(m_building_config_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    //m_building_config_list.InsertColumn(BC_ITEM, _T(""), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    //m_building_config_list.InsertColumn(BC_MAINNAME, _T("Main Building"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    ////m_building_config_list.InsertColumn(BC_SUBNAME, _T("Sub Net"), 0, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_building_config_list.InsertColumn(BC_PROTOCOL, _T("Protocol"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_building_config_list.InsertColumn(BC_IPADDRESS, _T("IP Address / Domain"), 150, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_building_config_list.InsertColumn(BC_IPPORT, _T("IP Port"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_building_config_list.InsertColumn(BC_COMPORT, _T("COM Port"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_building_config_list.InsertColumn(BC_BAUDRATE, _T("Baud Rate"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_building_config_list.InsertColumn(BC_BUILDINGPATH, _T("Building Path"), 600, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

    for (int i=0; i<(int)m_BuildNameLst.size(); i++)
    {
        CString temp_item;
        temp_item.Format(_T("%d"),i+1);
        m_building_config_list.InsertItem(i,_T(""));

        m_building_config_list.SetItemText(i,BC_MAINNAME,m_BuildNameLst.at(i).MainBuildingName);
  
		if (m_BuildNameLst.at(i).Protocol.CompareNoCase(_T("Auto")) == 0) // AUTO时 需要使能的表格
		{
			m_building_config_list.SetCellEnabled(i, BC_IPADDRESS, 0);
			m_building_config_list.SetCellEnabled(i, BC_IPPORT, 0);
			m_building_config_list.SetCellEnabled(i, BC_COMPORT, 0);
			m_building_config_list.SetCellEnabled(i, BC_BAUDRATE, 0);
		}

		if (m_BuildNameLst.at(i).Protocol.CompareNoCase(_T("Modbus TCP")) == 0)     //   MODBUS_TCP
		{
			m_building_config_list.SetCellEnabled(i, BC_IPADDRESS, 1);
			m_building_config_list.SetCellEnabled(i, BC_IPPORT, 1);
			m_building_config_list.SetCellEnabled(i, BC_COMPORT, 0);
			m_building_config_list.SetCellEnabled(i, BC_BAUDRATE, 0);
		}

		if ((m_BuildNameLst.at(i).Protocol.CompareNoCase(_T("Modbus 485")) == 0) ||    //   MODBUS_485
			(m_BuildNameLst.at(i).Protocol.CompareNoCase(_T("Modbus TCP")) == 0) )    //    MODBUS_TCP
        {
            m_building_config_list.SetCellEnabled(i,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(i,BC_IPPORT,0);
            m_building_config_list.SetCellEnabled(i,BC_COMPORT,1);
            m_building_config_list.SetCellEnabled(i,BC_BAUDRATE,1);
        }
        if(m_BuildNameLst.at(i).b_selected)
            m_building_config_list.SetItemText(i,BC_ITEM,_T("Selected"));
        else
            m_building_config_list.SetItemText(i,BC_ITEM,_T(""));
        m_building_config_list.SetItemText(i,BC_PROTOCOL, m_BuildNameLst.at(i).Protocol);
        m_building_config_list.SetItemText(i,BC_IPADDRESS,m_BuildNameLst.at(i).IPAddress_Domain);
        m_building_config_list.SetItemText(i,BC_IPPORT,m_BuildNameLst.at(i).IP_Port);
        m_building_config_list.SetItemText(i,BC_COMPORT,m_BuildNameLst.at(i).Comport);
        m_building_config_list.SetItemText(i,BC_BAUDRATE,m_BuildNameLst.at(i).BaudRate);
        m_building_config_list.SetItemText(i,BC_BUILDINGPATH,m_BuildNameLst.at(i).BuildingPath);

        if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_PROTOCOL))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0; j<(int)sizeof(Building_Protocol)/sizeof(Building_Protocol[0]); j++)
            {
                strlist.push_back(Building_Protocol[j]);
            }
            m_building_config_list.SetCellStringList(i, BC_PROTOCOL, strlist);
        }

        if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_BAUDRATE))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0; j<(int)sizeof(Building_Baudrate)/sizeof(Building_Baudrate[0]); j++)
            {
                strlist.push_back(Building_Baudrate[j]);
            }
            m_building_config_list.SetCellStringList(i, BC_BAUDRATE, strlist);
        }


        if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_COMPORT))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0; j<m_szBuildingComs.size(); j++)
            {
                strlist.push_back(m_szBuildingComs.at(j));
            }
            m_building_config_list.SetCellStringList(i, BC_COMPORT, strlist);
        }

    }

    int last_new_item = (int)m_BuildNameLst.size();
    CString temp_cs;
    temp_cs.Format(_T("%d"),last_new_item + 1);
    m_building_config_list.InsertItem(last_new_item,_T(""));
    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_PROTOCOL))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<(int)sizeof(Building_Protocol)/sizeof(Building_Protocol[0]); j++)
        {
            strlist.push_back(Building_Protocol[j]);
        }
        m_building_config_list.SetCellStringList(last_new_item, BC_PROTOCOL, strlist);
    }

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_BAUDRATE))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<(int)sizeof(Building_Baudrate)/sizeof(Building_Baudrate[0]); j++)
        {
            strlist.push_back(Building_Baudrate[j]);
        }
        m_building_config_list.SetCellStringList(last_new_item, BC_BAUDRATE, strlist);
    }

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_COMPORT))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<m_szBuildingComs.size(); j++)
        {
            strlist.push_back(m_szBuildingComs.at(j));
        }
        m_building_config_list.SetCellStringList(last_new_item, BC_COMPORT, strlist);
    }

    m_building_config_list.ShowWindow(SW_SHOW);
}
void CBuildingConfigration::Update_Building()
{
    Building_Config BuildingTemp;
	CppSQLite3DB SqliteDBT3000;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
    BuildingTemp.BuildingPath=m_building_config_list.GetItemText(m_changedRow,BC_BUILDINGPATH);
    BuildingTemp.Protocol=m_building_config_list.GetItemText(m_changedRow,BC_PROTOCOL);
    BuildingTemp.IPAddress_Domain=m_building_config_list.GetItemText(m_changedRow,BC_IPADDRESS);
    BuildingTemp.IP_Port=m_building_config_list.GetItemText(m_changedRow,BC_IPPORT);
    BuildingTemp.Comport=m_building_config_list.GetItemText(m_changedRow,BC_COMPORT);
    BuildingTemp.BaudRate=m_building_config_list.GetItemText(m_changedRow,BC_BAUDRATE);
    try
    {
        CString strSql;
        strSql.Format(_T("update Building set Protocal='%s',Ip_Address='%s',Ip_Port='%s',Com_Port='%s',Braudrate='%s' where Building_Path='%s'  "),
                      BuildingTemp.Protocol,BuildingTemp.IPAddress_Domain,BuildingTemp.IP_Port,BuildingTemp.Comport,BuildingTemp.BaudRate,BuildingTemp.BuildingPath);
        SqliteDBT3000.execDML((UTF8MBSTR)strSql);

    }
    catch (...)
    {
        AfxMessageBox(_T("Failed"));
    }
    SqliteDBT3000.closedb();



    //Fresh_List();
    //PostMessage(WM_FRESH_DB,0,0);
}
void CBuildingConfigration::Deal_BuildingPath()
{
	CppSQLite3DB SqliteDBT3000;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
	CppSQLite3Query q;
	CString strSql;
	strSql=_T("select * from Building");
    q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
    _variant_t temp_variant;
    CString pathfull;
    CString pathnew;
    try
    {
        while(!q.eof())
        {
            
			pathfull = q.getValuebyName(L"Building_Path");
            pathnew=pathfull;
            int index=pathnew.Find(_T("Database"));
            pathnew.Delete(0,index);

            strSql.Format(_T("update Building set Building_Path='%s' where Building_Path='%s'"),pathnew,pathfull);
            SqliteDBT3000.execDML((UTF8MBSTR)strSql);
            q.nextRow();
        }
    }
    catch (...)
    {
        AfxMessageBox(_T("Failed"));
    }
    SqliteDBT3000.closedb();
}




LRESULT CBuildingConfigration::Fresh_Building_Config_Item(WPARAM wParam,LPARAM lParam)
{
	 

    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam;
	int sqlID;
    m_changedRow=Changed_Item;
    m_changedCol=Changed_SubItem;
    CString strTips;
    CString cs_temp = m_building_config_list.GetItemText(Changed_Item,Changed_SubItem);
    CString dbpath = m_building_config_list.GetItemText(Changed_Item,BC_BUILDINGPATH);
	if (m_changedRow<m_BuildNameLst.size())
	{
	   sqlID = m_BuildNameLst.at(m_changedRow).ID;
	}
	 
    Building_Config BCTemp;
    BOOL Is_The_Same=TRUE;
    if (cs_temp.IsEmpty())
    {
        m_building_config_list.Set_Edit(FALSE);
        return 0;
    }
    if(Changed_SubItem == BC_MAINNAME)
    {
        CString IP = m_building_config_list.GetItemText(m_changedRow,BC_IPADDRESS);
        CString Port = m_building_config_list.GetItemText(m_changedRow,BC_IPPORT);
        CString ComPort = m_building_config_list.GetItemText(m_changedRow,BC_COMPORT);
        CString Baudrate = m_building_config_list.GetItemText(m_changedRow,BC_BAUDRATE);
        CString Protocol = m_building_config_list.GetItemText(m_curRow,BC_PROTOCOL);

        LoadBuildingConfigDB();
        BOOL Is_Changed=FALSE;
        CString path=m_building_config_list.GetItemText(Changed_Item,BC_BUILDINGPATH);
        if (path.IsEmpty())
        {
            Is_Changed=FALSE;
        }
        else
        {
            m_select_text=m_BuildNameLst.at(Changed_Item).MainBuildingName;
            if (!path.IsEmpty())
            {

                Is_Changed=TRUE;
            }
        }
        if (m_select_text.CompareNoCase(cs_temp)==0)
        {
            return 0;
        }

        if (Is_Changed)
        {

			CString m_select = m_building_config_list.GetItemText(m_curRow, BC_ITEM);
			 
			if (!m_select.IsEmpty())
			{
				AfxMessageBox(_T("The current building is using,Can't Change it !"));
				m_building_config_list.SetItemText(m_curRow, m_curCol, m_select_text);
				return 0;
			}

            if (m_select_text.CompareNoCase(cs_temp)==0)
            {
                return 0;
            }
            for (int i=0; i<m_BuildNameLst.size(); i++)
            {
                if (cs_temp.CompareNoCase(m_BuildNameLst.at(i).MainBuildingName)==0)
                {
                    strTips.Format(_T("%s can't create,it existed!"),cs_temp);
                    MessageBox(strTips);

                    m_building_config_list.SetItemText(Changed_Item,BC_MAINNAME,m_select_text);
                    // Initial_Building_List();
                    return 0;
                }
            }
            CString filefoldSource,fileSource;
            CString NewfilefoldSource,NewfileSource;

            fileSource = g_strExePth + path;
            filefoldSource = fileSource;
            PathRemoveFileSpec(filefoldSource.GetBuffer (256));
            filefoldSource.ReleaseBuffer ();
            NewfileSource=filefoldSource+L"\\"+cs_temp+L".db";
            filefoldSource= filefoldSource+L"\\";

            NewfilefoldSource = g_strExePth+CString("Database\\Buildings\\")+cs_temp+CString("\\");
//   			char source[255];
// 			char desr[255];
 			
  			
  			  
		  
 			CFileOperation      fp;
             if (fp.CheckPath (fileSource)>0)
             {
				strTips.Format(L"Can't rename from %s to %s", fileSource, NewfileSource);

// 				WideCharToMultiByte(CP_ACP, 0, fileSource.GetBuffer(), -1, source, 255, NULL, NULL);
// 
// 				WideCharToMultiByte(CP_ACP, 0, NewfileSource.GetBuffer(), -1, desr, 255, NULL, NULL);


				 if (!fp.Rename(fileSource, NewfileSource))
				 {
					 AfxMessageBox(strTips);
					 m_building_config_list.SetItemText(m_curRow, m_curCol, m_select_text);
					 return 0;
				 }
// 				if (rename(source, desr)<0)
// 				{
// 					AfxMessageBox(strTips);
// 					return 0;
// 				}
				strTips.Format(L"Can't rename from %s to %s", filefoldSource, NewfilefoldSource);
// 				WideCharToMultiByte(CP_ACP, 0, filefoldSource.GetBuffer(), -1, source, 255, NULL, NULL);
// 
// 				WideCharToMultiByte(CP_ACP, 0, NewfilefoldSource.GetBuffer(), -1, desr, 255, NULL, NULL);


// 				if (rename(source, desr) < 0)
// 				{
// 					AfxMessageBox(strTips);
// 					return 0;
// 				}
                 if (!fp.Rename(filefoldSource, NewfilefoldSource))
                 {
 					AfxMessageBox(strTips);
					m_building_config_list.SetItemText(m_curRow, m_curCol, m_select_text);
 					return 0;
                 }
				
                
                
            }

            NewfileSource=NewfilefoldSource+cs_temp+L".db";
            NewfileSource.Delete (0,g_strExePth.GetLength ());
            CString sql;
            try
            {
				CppSQLite3DB SqliteDBT3000;
				SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
				CppSQLite3Query q;
                sql.Format(_T("Select * from Building where Building_Path='%s'"),path);
                q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);
                if (!q.eof())//有表但是没有对应序列号的值
                {
                     

                    sql.Format(_T("Select * from Building "));
                    q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);;
                    if (!q.eof())
                    {
                       
						sql.Format(L"Delete from Building_ALL where Building_Name= '%s' ", cs_temp);
						SqliteDBT3000.execDML((UTF8MBSTR)sql);

                        sql.Format(_T("update Building_ALL set Building_Name = '%s' where Building_Name = '%s' "),cs_temp,m_select_text);
                        SqliteDBT3000.execDML((UTF8MBSTR)sql);


						sql.Format(L"Delete from Building where Main_BuildingName= '%s' ", cs_temp);
						SqliteDBT3000.execDML((UTF8MBSTR)sql);

                        sql.Format(_T("update Building set Main_BuildingName = '%s' ,Building_Name = '%s' , Building_Path = '%s'  where  Building_Path = '%s' "),cs_temp,cs_temp,NewfileSource,path);
						SqliteDBT3000.execDML((UTF8MBSTR)sql);
                        
                    }





                }
				q.finalize();
                SqliteDBT3000.closedb();

				m_BuildNameLst.at(m_curRow).MainBuildingName = cs_temp;
				m_BuildNameLst.at(m_curRow).Sub_NetName = cs_temp;

				 

				 
			
				
            }
            catch(_com_error *e)
            {
                AfxMessageBox(_T("Operator Failed"));
            }
            m_bChanged=TRUE;
            m_building_config_list.SetItemText (Changed_Item,BC_BUILDINGPATH,NewfileSource);
            _variant_t temp_variant;

           
            //修改ALL_NODE 里面的BuilingName .
#if 0    //校驗一下看看是否是Building的Database

			CString DB_Building_Name = L"";
			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Query q;

            CString BuildingPath = GetExePath(true)+ NewfileSource;
             SqliteDBBuilding.open((UTF8MBSTR)BuildingPath);
            
            if (!SqliteDBBuilding.tableExists("ALL_NODE"))
            {

                SqliteDBBuilding.closedb();
                AfxMessageBox(_T("The Database of the building is error."));
                return 0;
            }
            else
            {

                  sql.Format(_T("Select  distinct mainbuilding_name  from  ALL_NODE"));
                  q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
                   if(!q.eof())
                   {

				   	DB_Building_Name = q.getValuebyName(L"MainBuilding_Name");
                   }
            	   

            }


			sql.Format(_T("Select * from ALL_NODE"));
			q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
			while (!q.eof())
			{

				sql.Format(_T("update ALL_NODE set MainBuilding_Name  = '%s' ,Building_Name = '%s'  where  MainBuilding_Name  = '%s'"), cs_temp, cs_temp, DB_Building_Name);
				SqliteDBBuilding.execDML((UTF8MBSTR)sql);
				q.nextRow();

			}

			 if (SqliteDBBuilding.open((UTF8MBSTR)BuildingPath) == SQLITE_OK)
			 {
				 SqliteDBBuilding.closedb();
			 }
			


#endif


 

           


        }
        else
        {
            for (int i=0; i<m_BuildNameLst.size(); i++)
            {
                if (cs_temp.CompareNoCase(m_BuildNameLst.at(i).MainBuildingName)==0)
                {
                    strTips.Format(_T("%s can't create,it existed!"),cs_temp);
                    MessageBox(strTips);
                    m_building_config_list.SetItemText(Changed_Item,Changed_SubItem,L"");
                    return 0;
                }
            }

            m_building_config_list.SetItemText(Changed_Item,Changed_SubItem,cs_temp);
            // m_building_config_list.SetItemText(Changed_Item,BC_SUBNAME,cs_temp);


            CString filebuildingPath=g_strBuildingFolder+cs_temp+_T("\\");
            // CString strDestFileName=g_strBuildingFolder+BCTemp.MainBuildingName+_T("\\");
            CreateDirectory(filebuildingPath,NULL);
            filebuildingPath+=cs_temp+_T(".db");

            DeleteFile(filebuildingPath);
            HANDLE hFind;//
            WIN32_FIND_DATA wfd;//
            //create building db file

            hFind = FindFirstFile(filebuildingPath, &wfd);//
            if (hFind==INVALID_HANDLE_VALUE)
            {

                HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_BUILDINGDB1), _T("BUILDINGDB"));
                HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);


                LPVOID lpExe = LockResource(hGlobal);
                CFile file;
                if(file.Open(filebuildingPath, CFile::modeCreate | CFile::modeWrite))
                    file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));
                file.Close();
                ::UnlockResource(hGlobal);
                ::FreeResource(hGlobal);
            }//
            int index=filebuildingPath.Find(_T("Database"));
            filebuildingPath.Delete(0,index);

            BCTemp.b_selected=FALSE;
            BCTemp.Sub_NetName=cs_temp;
            BCTemp.MainBuildingName=cs_temp;
            BCTemp.BuildingPath=filebuildingPath;
            if(Protocol.CompareNoCase(_T("Auto"))==0||Protocol.IsEmpty())
            {
                BCTemp.Protocol=_T("Auto");
                BCTemp.IPAddress_Domain = NO_APPLICATION;
                BCTemp.IP_Port = NO_APPLICATION;
                BCTemp.Comport = NO_APPLICATION;
                BCTemp.BaudRate = NO_APPLICATION;
            }
            else
            {
                BCTemp.Protocol=Protocol;
                BCTemp.IPAddress_Domain = IP;
                BCTemp.IP_Port = Port;
                BCTemp.Comport =ComPort ;
                BCTemp.BaudRate = Baudrate;
            }

            Is_The_Same=FALSE;
#if 1
            try
            {
				 
				CppSQLite3DB SqliteDBT3000;
				CppSQLite3Query q;
				SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
                CString sql;
                sql.Format(_T("Select * from Building where Building_Path='%s'"),BCTemp.BuildingPath);
                q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);
                if (q.eof())//有表但是没有对应序列号的值
                {
                     
                    for (int j=0; j<m_BuildNameLst.size(); j++)
                    {
                        if (BCTemp.MainBuildingName.CompareNoCase(m_BuildNameLst.at(j).MainBuildingName)==0)
                        {
                            Is_The_Same=TRUE;
                            break;

                        }
                        else
                        {
                            Is_The_Same=FALSE;
                        }
                    }
                    if (!Is_The_Same)
                    {
                        sql.Format(_T("delete   from Building_ALL where Building_Name = '%s' "),BCTemp.MainBuildingName.GetBuffer());
                       SqliteDBT3000.execDML((UTF8MBSTR)sql);

                        sql.Format(_T("Insert into Building_ALL(Building_Name,Default_Build) values('%s','%d')"),BCTemp.MainBuildingName.GetBuffer(),0);
                        SqliteDBT3000.execDML((UTF8MBSTR)sql);

                        sql.Format(_T("Insert into Building(Main_BuildingName,Building_Name,Protocal,Default_SubBuilding,Building_Path,Com_Port,Ip_Address,IP_Port,Braudrate) values('%s','%s','%s','%d','%s' ,'%s','%s','%s','%s')"),
                                   BCTemp.MainBuildingName.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),BCTemp.Protocol.GetBuffer(),0,BCTemp.BuildingPath.GetBuffer(),
                                   BCTemp.Comport,
                                   BCTemp.IPAddress_Domain,
                                   BCTemp.IP_Port,
                                   BCTemp.BaudRate);
                        SqliteDBT3000.execDML((UTF8MBSTR)sql);

                    }
                    else
                    {
                        sql.Format(_T("delete   from Building_ALL where Building_Name = '%s' "),BCTemp.MainBuildingName.GetBuffer());
                         
						SqliteDBT3000.execDML((UTF8MBSTR)sql);


                        sql.Format(_T("Insert into Building_ALL(Building_Name,Default_Build) values('%s','%d')"),BCTemp.MainBuildingName.GetBuffer(),0);
                        SqliteDBT3000.execDML((UTF8MBSTR)sql);


                        sql.Format(_T("update Building set Building_Path = '%s' ,Building_Name = '%s' ,Protocal = '%s' where  Main_BuildingName = '%s' "),BCTemp.BuildingPath.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),BCTemp.Protocol,BCTemp.MainBuildingName.GetBuffer());

                        SqliteDBT3000.execDML((UTF8MBSTR)sql);

                    }

				}
				

				 
				sql.Format(_T("Select * from Building where Building_Path='%s'"),BCTemp.BuildingPath);
				q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);
				if (!q.eof())
				{
				  sqlID = q.getIntField("ID");
				}
				 

			    SqliteDBT3000.closedb();
				 
			 
				BCTemp.ID = sqlID;

                m_BuildNameLst.push_back(BCTemp);
                /*	 int last_new_item = (int)m_BuildNameLst.size();
                	 CString temp_cs;
                	 temp_cs.Format(_T("%d"),last_new_item + 1);
                	 m_building_config_list.InsertItem(last_new_item,_T(""));*/



                int last_new_item = (int)m_BuildNameLst.size();
                CString temp_cs;
                temp_cs.Format(_T("%d"),last_new_item + 1);
                m_building_config_list.InsertItem(last_new_item,_T(""));
// 				 for (int x=0;x<BC_COL_NUMBER;x++)
// 				 {
// 					 if((last_new_item%2)==0)
// 						 m_building_config_list.SetItemBkColor(last_new_item,x,LIST_ITEM_DEFAULT_BKCOLOR);
// 					 else
// 						 m_building_config_list.SetItemBkColor(last_new_item,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
// 				 }

                if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_PROTOCOL))
                {
                    ListCtrlEx::CStrList strlist;
                    for (int j=0; j<(int)sizeof(Building_Protocol)/sizeof(Building_Protocol[0]); j++)
                    {
                        strlist.push_back(Building_Protocol[j]);
                    }
                    m_building_config_list.SetCellStringList(last_new_item, BC_PROTOCOL, strlist);
                }

                if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_BAUDRATE))
                {
                    ListCtrlEx::CStrList strlist;
                    for (int j=0; j<(int)sizeof(Building_Baudrate)/sizeof(Building_Baudrate[0]); j++)
                    {
                        strlist.push_back(Building_Baudrate[j]);
                    }
                    m_building_config_list.SetCellStringList(last_new_item, BC_BAUDRATE, strlist);
                }

                if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_COMPORT))
                {
                    ListCtrlEx::CStrList strlist;
                    for (int j=0; j<m_szBuildingComs.size(); j++)
                    {
                        strlist.push_back(m_szBuildingComs.at(j));
                    }
                    m_building_config_list.SetCellStringList(last_new_item, BC_COMPORT, strlist);
                }


            }
            catch(_com_error *e)
            {
                AfxMessageBox(_T("Operator Failed"));
            }

            //更新到数据库之后，加入到Vector列表


#endif


            m_building_config_list.SetItemText(Changed_Item,BC_BUILDINGPATH,filebuildingPath);
            m_building_config_list.SetItemText(Changed_Item,BC_PROTOCOL,BCTemp.Protocol);


            m_building_config_list.SetItemText(Changed_Item,BC_IPADDRESS,BCTemp.IPAddress_Domain);
            m_building_config_list.SetItemText(Changed_Item,BC_IPPORT,BCTemp.IP_Port);
            m_building_config_list.SetItemText(Changed_Item,BC_COMPORT,BCTemp.Comport);
            m_building_config_list.SetItemText(Changed_Item,BC_BAUDRATE,BCTemp.BaudRate);

            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPPORT,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_COMPORT,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_BAUDRATE,0);

        }
        //m_changedRow=m_curRow;
        //PostMessage(WM_FRESH_DB,0,0);


    }
    else if(Changed_SubItem == BC_PROTOCOL)
    {
        CString IP = m_building_config_list.GetItemText(m_changedRow,BC_IPADDRESS);
        CString Port = m_building_config_list.GetItemText(m_changedRow,BC_IPPORT);
        CString ComPort = m_building_config_list.GetItemText(m_changedRow,BC_COMPORT);
        CString Baudrate = m_building_config_list.GetItemText(m_changedRow,BC_BAUDRATE);




		if (cs_temp.CompareNoCase(_T("Auto")) == 0)    //只有填写远程的时候需要填写IP地址和端口;
        {
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPPORT,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_COMPORT,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_BAUDRATE,0);

            m_building_config_list.SetItemText(m_changedRow,BC_IPADDRESS,NO_APPLICATION);
            m_building_config_list.SetItemText(m_changedRow,BC_IPPORT,NO_APPLICATION);
            m_building_config_list.SetItemText(m_changedRow,BC_COMPORT,NO_APPLICATION);
            m_building_config_list.SetItemText(m_changedRow,BC_BAUDRATE,NO_APPLICATION);

        }
		else if((cs_temp.CompareNoCase(_T("Modbus TCP")) == 0)  ||   
			    (cs_temp.CompareNoCase(_T("Remote Device")) == 0))      
        {
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPADDRESS,1);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPPORT,1);

            m_building_config_list.SetCellEnabled(Changed_Item,BC_COMPORT,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_BAUDRATE,0);
            if (!dbpath.IsEmpty())
            {
                GetPrivateProfileString(m_BuildNameLst.at(Changed_Item).MainBuildingName,_T("Remote_IP"),_T("192.168.0.3"),IP.GetBuffer(MAX_PATH),MAX_PATH,g_achive_device_name_path);
                GetPrivateProfileString(m_BuildNameLst.at(Changed_Item).MainBuildingName,_T("Remote_Port"),_T("10000"),Port.GetBuffer(MAX_PATH),MAX_PATH,g_achive_device_name_path);
            }
            else
            {
				IP = _T("newfirmware.com");
				Port = _T("502");
            }
			if(Changed_Item>= m_BuildNameLst.size())
			{
				return 0;
			}
			  WritePrivateProfileStringW(m_BuildNameLst.at(Changed_Item).MainBuildingName,_T("Remote_IP"),IP,g_achive_device_name_path);
            m_building_config_list.SetItemText(m_changedRow,BC_COMPORT,NO_APPLICATION);
            m_building_config_list.SetItemText(m_changedRow,BC_BAUDRATE,NO_APPLICATION);

            m_building_config_list.SetItemText(m_changedRow,BC_IPADDRESS,IP);
            m_building_config_list.SetItemText(m_changedRow,BC_IPPORT,Port);

			if (GetPrivateProfileInt(_T("Setting"), _T("ENABLE_PORT_WARNING"), 1, g_cstring_ini_path) == 1)
			{
				CBacnetRemotePortWarning Dlg;
				Dlg.DoModal();
			}
        }
		if ((cs_temp.CompareNoCase(_T("Modbus 485")) == 0) ||   
			(cs_temp.CompareNoCase(_T("Bacnet MSTP")) == 0))      
        {
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPADDRESS,0);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_IPPORT,0);

            m_building_config_list.SetCellEnabled(Changed_Item,BC_COMPORT,1);
            m_building_config_list.SetCellEnabled(Changed_Item,BC_BAUDRATE,1);


            if (ComPort.CompareNoCase(NO_APPLICATION)==0)
            {
                ComPort  = _T("COM1");
                Baudrate = _T("19200");
                m_building_config_list.SetItemText(m_changedRow,BC_IPADDRESS,NO_APPLICATION);
                m_building_config_list.SetItemText(m_changedRow,BC_IPPORT,NO_APPLICATION);
                m_building_config_list.SetItemText(m_changedRow,BC_COMPORT,NO_APPLICATION);
                m_building_config_list.SetItemText(m_changedRow,BC_BAUDRATE,NO_APPLICATION);
                m_building_config_list.SetItemText(m_changedRow,BC_COMPORT,ComPort);
                m_building_config_list.SetItemText(m_changedRow,BC_BAUDRATE,Baudrate);
            }
        }

        if (!dbpath.IsEmpty())
        {

            Update_Building();
            // return 0;
        }
        else
        {
            return 0;
        }




    }
    else if(Changed_SubItem == BC_IPADDRESS)
    {
		CString  temp_protocol = 	m_building_config_list.GetItemText(Changed_Item,BC_PROTOCOL);
		if(temp_protocol.CompareNoCase(_T("Remote Device")) == 0 )
		{
			CString temp_serial_number = m_building_config_list.GetItemText(Changed_Item,BC_IPADDRESS);
			temp_serial_number.Trim();
			int input_length  =	temp_serial_number.GetLength();

			bool  serial_is_all_digital = AllCharactorIsDigital(temp_serial_number);
			 WritePrivateProfileStringW(m_BuildNameLst.at(Changed_Item).MainBuildingName,_T("Remote_IP"),temp_serial_number,g_achive_device_name_path);
			if((input_length <=8) && (serial_is_all_digital))	//说明输入的全是数字，是序列号.
			{
				CString temp_message;
				temp_message.Format(_T("Do you want to connect the remote device (serial number is %s)") ,temp_serial_number);
				if(IDYES == MessageBox(temp_message,_T("Notice"),MB_YESNO | MB_ICONINFORMATION))
				{
					try_connect_serial =    _wtoi(temp_serial_number)  ;//  (unsigned int)atoi(temp_serial_number);
					ptp_cancel_login = false;
					CRemotePtpLogin ptplogin;
					ptplogin.DoModal();
					if(ptp_cancel_login)
						return 0;
					CConnectRemoteServer Connectdlg;
					Connectdlg.DoModal();
					return 0;
				}
			}

		}

        if (dbpath.IsEmpty())
        {
            return 0;
        }
        CString temp_ip;
        temp_ip = m_building_config_list.GetItemText(m_changedRow,BC_IPADDRESS);

        WritePrivateProfileStringW(m_BuildNameLst.at(Changed_Item).MainBuildingName,_T("Remote_IP"),temp_ip,g_achive_device_name_path);



        Update_Building();

        CBacnetAddRemoteDevice RemoteDlg;
        RemoteDlg.DoModal();
    }
    else if(Changed_SubItem == BC_IPPORT)
    {
        if (dbpath.IsEmpty())
        {
            return 0;
        }
        CString temp_port;
        temp_port=m_building_config_list.GetItemText(m_changedRow,BC_IPPORT);
        WritePrivateProfileStringW(m_BuildNameLst.at(Changed_Item).MainBuildingName,_T("Remote_Port"),temp_port,g_achive_device_name_path);
        Update_Building();
    }
    else if(Changed_SubItem == BC_COMPORT)
    {
        if (dbpath.IsEmpty())
        {
            return 0;
        }
        Update_Building();
    }
    else if(Changed_SubItem == BC_BAUDRATE)
    {
        if (dbpath.IsEmpty())
        {
            return 0;
        }
        Update_Building();
    }
    else if (Changed_SubItem == BC_BUILDINGPATH)
    {
        cs_temp = m_building_config_list.GetItemText(Changed_Item,Changed_SubItem);
    }
    return 0;
}


void CBuildingConfigration::LoadBuildingConfigDB()
{

    //m_buildNameLst.clear();
    m_BuildNameLst.clear();
    CString strSql;
    //strSql.Format(_T("select * from Building where Main_BuildingName = '%s'"),m_strMainBuildingName); order by Main_BuildingName
    strSql.Format(_T("select * from Building "));
    m_q = m_SqliteDBT3000.execQuery((UTF8MBSTR)strSql);

    CString str_temp;
    str_temp.Empty();

	int id =1;
    _variant_t temp_variant;
    while(!m_q.eof())
    {
        Building_Config temp_building; //先用临时building 接收新值;
        int temp_value=0;
        temp_value=m_q.getIntField("Default_SubBuilding");
        if(temp_value==1)//def building;
        {
            temp_building.b_selected = true;
        }
        else
        {
            temp_building.b_selected = false;
        }
        _variant_t temp_variant;

		


        temp_building.MainBuildingName=m_q.getValuebyName(L"Main_BuildingName");//
        

		if (!temp_building.MainBuildingName.IsEmpty())
		{
			temp_building.ID=m_q.getIntField("ID");//
			 
		}

        temp_building.Sub_NetName=m_q.getValuebyName(L"Building_Name");//
         

        temp_building.Protocol=m_q.getValuebyName(L"Protocal");//
         



         temp_building.IPAddress_Domain=m_q.getValuebyName(L"Ip_Address");//
         

        temp_building.IP_Port=m_q.getValuebyName(L"Ip_Port");//
       

        temp_building.Comport=m_q.getValuebyName(L"Com_Port");//
         

        temp_building.BaudRate=m_q.getValuebyName(L"Braudrate");//
        
        //Building_Path
        temp_building.BuildingPath=m_q.getValuebyName(L"Building_Path");//
         
        int index=temp_building.BuildingPath.Find(_T("Database"));
        temp_building.BuildingPath.Delete(0,index);


		temp_building.country=m_q.getValuebyName(L"country");//
		 

		temp_building.state=m_q.getValuebyName(L"state");//
		 

		temp_building.city=m_q.getValuebyName(L"city");//
		 


		temp_building.street=m_q.getValuebyName(L"street");//
	 

	
	 

		temp_building.EngineeringUnits=m_q.getValuebyName(L"EngineeringUnits");//
		 

	 

		 
        m_BuildNameLst.push_back(temp_building);

        m_q.nextRow();//
    }

    
}

BOOL IsNum(CString str)
{
    int n=str.GetLength();
    for(int i=0; i<n; i++)
        if (str[i]<'0'||str[i]>'9')
            return FALSE;
    return TRUE;
}


//BOOL ValidAddress(CString sAddress)
//{
//	int nPos;
//	UINT n1,n2,n3,n4;
//	CString sTemp=sAddress;
//	n1=_wtoi(sTemp);
//	nPos=sTemp.Find(_T("."));
//	if(nPos==-1) return false;
//	sTemp=sTemp.Mid(nPos+1);
//
//	n2=_wtoi(sTemp);
//	nPos=sTemp.Find(_T("."));
//	if(nPos==-1) return false;
//	sTemp=sTemp.Mid(nPos+1);
//	n3=_wtoi(sTemp);
//	nPos=sTemp.Find(_T("."));
//	if(nPos==-1) return false;
//	sTemp=sTemp.Mid(nPos+1);
//	n4=_wtoi(sTemp);
//	if(n1<0 ||n1>255) return false;
//	if(n2<0 ||n2>255) return false;
//	if(n3<0 ||n3>255) return false;
//	if(n4<0 ||n4>255) return false;
//	return TRUE;
//}

void CBuildingConfigration::OnBnClickedBuildingButtonAdd()
{
    
    bool is_domain = false;
    int last_count = m_building_config_list.GetItemCount() - 1;
    CString strMainBuildName;
    CString strSubBuildingName;
    CString strProtocol;
    CString strIP;
    CString strIpPort;
    CString strCOMPort;
    CString strCOMPortBaud;
    strMainBuildName =  m_building_config_list.GetItemText(last_count,BC_MAINNAME);
//	strSubBuildingName = m_building_config_list.GetItemText(last_count,BC_SUBNAME);
    strProtocol =  m_building_config_list.GetItemText(last_count,BC_PROTOCOL);
    strIP = m_building_config_list.GetItemText(last_count,BC_IPADDRESS);
    strIpPort = m_building_config_list.GetItemText(last_count,BC_IPPORT);
    strCOMPort = m_building_config_list.GetItemText(last_count,BC_COMPORT);
    strCOMPortBaud = m_building_config_list.GetItemText(last_count,BC_BAUDRATE);
    if(strMainBuildName.IsEmpty())
    {
        MessageBox(_T("Warning!Main Building is empty!"));
        return;
    }
    else if(strSubBuildingName.IsEmpty())
    {
        MessageBox(_T("Warning!Sub Net is empty!"));
        return;
    }
    else if(strProtocol.IsEmpty())
    {
        MessageBox(_T("Warning!Protocol is empty!"));
        return;
    }

    if(strProtocol.CompareNoCase(_T("Auto")) == 0)
    {

    }
    else if((strProtocol.CompareNoCase(_T("Modbus TCP")) == 0) ||
            (strProtocol.CompareNoCase(_T("Remote Device")) == 0))
    {
        if((strIP.IsEmpty()) || (strIpPort.IsEmpty()))
        {
            MessageBox(_T("Warning!IP or domain is empty!"));
            return;
        }
        CStringArray temparray;
        SplitCStringA(temparray,strIP,_T("."));
        if((temparray.GetSize()==4))	//有3个  . 4段
        {
            if((IsNum(temparray.GetAt(0))) && (IsNum(temparray.GetAt(1))) && (IsNum(temparray.GetAt(2))))
            {
                if(ValidAddress(strIP) == false)
                {
                    MessageBox(_T("Warning!IP address error!"));
                    return;
                }

            }
            else	//否则判断为 域名;
            {
                is_domain = true;
            }
        }
        else	//判断为 域名;
        {
            is_domain = true;
        }

        if(is_domain)
        {
            char temp_domain[200];
            WideCharToMultiByte( CP_ACP, 0, strIP.GetBuffer(), -1, temp_domain, 255, NULL, NULL );
            strIP.ReleaseBuffer();
            hostent* host = gethostbyname(temp_domain);
            if(host == NULL)
            {
                if(IDYES == MessageBox(_T("The domain is unreachable .Continue?"),_T("Warning"),MB_YESNO))
                {

                }
                else
                {
                    return;
                }
            }
        }

    }
    else
    {
        if(strCOMPort.IsEmpty())
        {
            MessageBox(_T("Warning!Com port is empty!"));
            return;
        }
        else if(strCOMPortBaud.IsEmpty())
        {
            MessageBox(_T("Warning!Baudrate is empty!"));
            return;
        }
    }



    CString strSql;
    strSql.Format(_T("Select * from Building_ALL where Building_Name = '%s'"),strMainBuildName);
   m_q = m_SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
    _variant_t temp_variant_name;
    bool findMainBuilding=false;
    if(!m_q.eof())		//If it's not empty , means the input Building has exist in Main Building.
    {
        findMainBuilding=true;
    }

   

    if(!findMainBuilding)
    {
        strSql.Format(_T("insert into Building_ALL (Building_Name,Telephone,Address) values('"+strMainBuildName+"','"+ _T("") +"','"+_T("") +"')"));
        m_SqliteDBT3000.execDML((UTF8MBSTR)strSql);
    }
    //---------------------------------------------------------------------------------------------



    try
    {
        BOOL bDefault =FALSE;
        strSql.Format(_T("insert into Building (Main_BuildingName,Building_Name,Protocal,Com_Port,Ip_Address,Ip_Port,Braudrate) values('"+strMainBuildName+"','"+strSubBuildingName+"','"+strProtocol+"','"+strCOMPort+"','"+strIP+"','"+strIpPort+"','"+strCOMPortBaud+"')"));
        m_SqliteDBT3000.execDML((UTF8MBSTR)strSql);
    }
    catch(_com_error *e)
    {
        AfxMessageBox(e->ErrorMessage());
    }
    //Update_Recorder();




}


void CBuildingConfigration::OnBnClickedBuildingButtonDelete()
{
    m_SqliteDBT3000.closedb();

    CDialogEx::OnCancel();
}


void CBuildingConfigration::OnSize(UINT nType, int cx, int cy)
{


    if (nType==SIZE_RESTORED)
    {

        CRect ViewRect;
        GetClientRect(&ViewRect);
        //TRACE(_T(" View:T=%d,B=%d,L=%d,R=%d\n"),ViewRect.top,ViewRect.bottom,ViewRect.left,ViewRect.right);
        // m_MsDataGrid.SetWindowPos(this,ViewRect.top,ViewRect.left,ViewRect.Width(),ViewRect.Height(),SWP_SHOWWINDOW|SWP_NOZORDER);
        if (m_building_config_list.GetSafeHwnd())
        {
            m_building_config_list.MoveWindow(CRect(ViewRect.left,ViewRect.top+70,ViewRect.Width(),ViewRect.Height()),TRUE);
        }


    }

    CDialogEx::OnSize(nType, cx, cy);

    
}


void CBuildingConfigration::OnNMClickListBuildingConfig(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    
    CString temp_cstring;
    long lRow,lCol;
    m_building_config_list.Set_Edit(true);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_building_config_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_building_config_list.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;
    m_curCol=lCol;
    m_curRow=lRow;
    CString Strprotcol=m_building_config_list.GetItemText(lRow,BC_PROTOCOL);
    BOOL Is_The_Same=TRUE;
    m_select_text=m_building_config_list.GetItemText(lRow,lCol);
    if(lRow>m_building_config_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
        return;
    if(lRow<0)
        return;
    CString New_CString;
    CString temp_task_info;

	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Query q;
    CString temp1;
    CStringArray temparray;
    Building_Config BCTemp;

    CString sql;
    _variant_t temp_variant;
    CString Building_Name=L"";
    CString DB_Building_Name=L"";

     
     if (BC_BUILDINGPATH==lCol)//当点击选择一个数据库的时候，把Building的名字已经数据库的信息都获取过来
    {
        m_building_config_list.Set_Edit(FALSE);

        CString path=m_building_config_list.GetItemText(lRow,lCol);
        if (!path.IsEmpty())
        {

            return;
        }

        CStringArray  ArrayFileName;
        CString strFilter = _T("Access Database file|*.db|all file|*.*||");
        CFileDialog dlg(true,_T("Open image file"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
        if(IDOK==dlg.DoModal())
        {
            CString strdatabasefile=dlg.GetPathName();
            CString databasefilename=dlg.GetFileName();
            SplitCStringA(ArrayFileName,strdatabasefile,L".");
            if (ArrayFileName[ArrayFileName.GetSize()-1].CompareNoCase(L"db")!=0)
            {
                MessageBox(_T("Please select a access file"));

                return;
            }
#if 1 //校驗一下看看是否是Building的Database
            
			SqliteDBBuilding.open((UTF8MBSTR)strdatabasefile);
            if (!SqliteDBBuilding.tableExists("ALL_NODE"))
            {

                SqliteDBBuilding.closedb();
                AfxMessageBox(_T("The file is not a correct building database."));
                return;
                //continue;
            }
            else
            {

                sql.Format(_T("Select  distinct mainbuilding_name  from  ALL_NODE"));
				q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);

                if(!q.eof())
                {
                     

					DB_Building_Name = q.getValuebyName(L"MainBuilding_Name");
				}
				 

            }
#endif
            for (int j=0; j<m_BuildNameLst.size(); j++)
            {
                if (DB_Building_Name.CompareNoCase(m_BuildNameLst.at(j).MainBuildingName)==0)
                {
                    Is_The_Same=TRUE;
                    break;

                }
                else
                {
                    Is_The_Same=FALSE;
                }
            }
            if (Is_The_Same)
            {
                CString strTips;
                strTips.Format(_T("The building:%s,Exists"),DB_Building_Name);
                AfxMessageBox(strTips);
                return;
            }
            SplitCStringA(ArrayFileName,strdatabasefile,L"\\");
            if (ArrayFileName.GetSize()<2)
            {
                MessageBox(_T("file path error"));
                return;
            }
 

            Building_Name=ArrayFileName[ArrayFileName.GetSize()-2];
            BCTemp.MainBuildingName=DB_Building_Name;
            BCTemp.Sub_NetName=DB_Building_Name;
            CString strDestFileName=g_strBuildingFolder+BCTemp.MainBuildingName+_T("\\");

            if (!CreateDirectory(strDestFileName,NULL))
            {
                CString strTips;
                strTips.Format(_T("%s  Create Failed"),strDestFileName);
                AfxMessageBox(strTips);
                return;
            }

            strDestFileName=g_strBuildingFolder+BCTemp.MainBuildingName+_T("\\")+databasefilename;

            if(!CopyFile(strdatabasefile,strDestFileName,FALSE))
            {
                CString strTips;
                strTips.Format(_T("Copy file form %s to %s Failed"),strdatabasefile,strDestFileName);
                AfxMessageBox(strTips);
                return;
            }



            BCTemp.BuildingPath=strDestFileName;
            int index= BCTemp.BuildingPath.Find(_T("Database"));
            BCTemp.BuildingPath.Delete(0,index);

            BCTemp.b_selected=FALSE;

            m_building_config_list.SetItemText(lRow,lCol,strDestFileName);
//			 m_building_config_list.SetItemText(lRow,BC_SUBNAME,BCTemp.MainBuildingName);
            m_building_config_list.SetItemText(lRow,BC_MAINNAME,BCTemp.MainBuildingName);

            //更新数据库
#if 1
            try
            {
				CppSQLite3DB SqliteDBT3000;
				SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
				CppSQLite3Query q;
                CString sql;
                sql.Format(_T("Select * from Building where Building_Path='%s'"),BCTemp.BuildingPath);
                q = SqliteDBT3000.execQuery((UTF8MBSTR)sql);
                if (q.eof())//有表但是没有对应序列号的值
                {
                   
                    for (int j=0; j<m_BuildNameLst.size(); j++)
                    {
                        if (BCTemp.MainBuildingName.CompareNoCase(m_BuildNameLst.at(j).MainBuildingName)==0)
                        {
                            Is_The_Same=TRUE;
                            break;

                        }
                        else
                        {
                            Is_The_Same=FALSE;
                        }
                    }
                    if (!Is_The_Same)
                    {
                        sql.Format(_T("Insert into Building_ALL(Building_Name,Default_Build) values('%s','%d')"),
                                   BCTemp.MainBuildingName.GetBuffer(),0);
                       SqliteDBT3000.execDML((UTF8MBSTR)sql);

                        sql.Format(_T("Insert into Building(Main_BuildingName,Building_Name,Default_SubBuilding,Building_Path) values('%s','%s','%d','%s')"),
                                   BCTemp.MainBuildingName.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),0,BCTemp.BuildingPath.GetBuffer());
                        SqliteDBT3000.execDML((UTF8MBSTR)sql);
                    }
                    else
                    {
                        sql.Format(_T("update Building set Building_Path = '%s' ,Building_Name = '%s'  where  Main_BuildingName = '%s' "),
                                   BCTemp.BuildingPath.GetBuffer(),BCTemp.MainBuildingName.GetBuffer(),BCTemp.MainBuildingName.GetBuffer());

                        SqliteDBT3000.execDML((UTF8MBSTR)sql);

                    }

                }
                SqliteDBT3000.closedb();
                m_BuildNameLst.push_back(BCTemp);
                int last_new_item = (int)m_BuildNameLst.size();
                CString temp_cs;
                temp_cs.Format(_T("%d"),last_new_item + 1);
                m_building_config_list.InsertItem(last_new_item,_T(""));
            }
            catch(_com_error *e)
            {
                AfxMessageBox(_T("Operator Failed"));
            }
            //更新到数据库之后，加入到Vector列表
#endif



        }


    }
//	else if(BC_SUBNAME==lCol)
// 	{
// 	m_building_config_list.Set_Edit(FALSE);
// 	return;
// 	}
    else if(lCol== BC_MAINNAME)
    {

    }
    else if(lCol== BC_PROTOCOL)
    {
// 		if ((m_BuildNameLst.size()-1)<lRow)
// 		{
// 		    m_building_config_list.Set_Edit(FALSE);
// 			return;
// 		}

    }
    else if(lCol == BC_IPADDRESS)
    {


// 		if ((m_BuildNameLst.size()-1)<lRow)
// 		{
// 			m_building_config_list.Set_Edit(FALSE);
// 			return;
// 		}

    }
    else if(lCol == BC_IPPORT)
    {
// 		if ((m_BuildNameLst.size()-1)<lRow)
// 		{
// 			m_building_config_list.Set_Edit(FALSE);
// 			return;
// 		}

    }
    else if(lCol == BC_COMPORT)
    {
// 		if ((m_BuildNameLst.size()-1)<lRow)
// 		{
// 			m_building_config_list.Set_Edit(FALSE);
// 			return;
// 		}

    }
    else if(lCol == BC_BAUDRATE)
    {
// 		if ((m_BuildNameLst.size()-1)<lRow)
// 		{
// 			m_building_config_list.Set_Edit(FALSE);
// 			return;
// 		}

    }



    *pResult = 0;
}


void CBuildingConfigration::OnNMRClickListBuildingConfig(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    CString temp_cstring;
    long lRow,lCol;
    m_building_config_list.Set_Edit(true);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_building_config_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_building_config_list.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;

    m_curRow=lRow;
    m_curCol=lCol;
    if(lRow>m_building_config_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
        return;
    if(lRow<0)
        return;
    CString New_CString;
    CString temp_task_info;

    CString temp1;
    CStringArray temparray;
    m_select_text=m_building_config_list.GetItemText(lRow,lCol);

    if (BC_ITEM==lCol)
    {
        m_changedCol=m_curCol;
        m_changedRow=m_curRow;
        CMenu menu;
        menu.LoadMenu(IDR_BUILDING_CONFIG);
        CMenu *pmenu=menu.GetSubMenu(0);
        CPoint point;
        GetCursorPos(&point);
        pmenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN ,point.x,point.y,this);

    }
    else
    {
        return;
    }
    *pResult = 0;
}


void CBuildingConfigration::OnBuildingconfigSelect()
{
    int count=m_building_config_list.GetRowCount();
	
    if (m_curRow==count-1)
    {
        return;
    }

	int ID = m_BuildNameLst.at(m_curRow).ID;
    CString m_select,m_strMainBuildingName,currentpath, sql;
    m_strMainBuildingName=m_building_config_list.GetItemText(m_curRow,BC_MAINNAME);
    m_select=m_building_config_list.GetItemText(m_curRow,m_curCol);
	currentpath = m_building_config_list.GetItemText(m_curRow, BC_BUILDINGPATH);
    if (!m_select.IsEmpty())
    {
        AfxMessageBox(_T("you have already Selected.\n don't need to select ,again!"));
        return;
	}
	
	CppSQLite3DB SqliteDBT3000;
	 
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
    try
    {

        CString execute_str=_T("update Building set Default_SubBuilding = 0 where Default_SubBuilding = 1");
        SqliteDBT3000.execDML((UTF8MBSTR)execute_str);
        execute_str.Format(_T("update Building set Default_SubBuilding =  1 where  Main_BuildingName= '%s'"),m_strMainBuildingName);
        SqliteDBT3000.execDML((UTF8MBSTR)execute_str);
	    
		 

    }
    catch (_com_error *e)
    {
        AfxMessageBox(_T("Operator Failed"));
    }
    SqliteDBT3000.closedb();
    //int count=m_building_config_list.GetRowCount();
    for (int row=0; row<count-1; row++)
    {
        if (row!=m_curRow)
        {
            m_building_config_list.SetItemText(row,m_curCol,L"");
        }
        else
        {
            m_building_config_list.SetItemText(m_curRow,m_curCol,L"Selected");
        }

    }

    //Fresh_List();
    //Initial_Building_List();



	//修改ALL_NODE 里面的BuilingName .
#if 1    //校驗一下看看是否是Building的Database

	CString DB_Building_Name = L"";
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Query q;

	CString BuildingPath = GetExePath(true) + currentpath;
	SqliteDBBuilding.open((UTF8MBSTR)BuildingPath);

	if (!SqliteDBBuilding.tableExists("ALL_NODE"))
	{

		SqliteDBBuilding.closedb();
		AfxMessageBox(_T("The Database of the building is error."));
		return ;
	}
	else
	{

		sql.Format(_T("Select  distinct mainbuilding_name  from  ALL_NODE"));
		q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
		if (!q.eof())
		{

			DB_Building_Name = q.getValuebyName(L"MainBuilding_Name");
		}


	}


	sql.Format(_T("Select * from ALL_NODE"));
	q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
	while (!q.eof())
	{

		sql.Format(_T("update ALL_NODE set MainBuilding_Name  = '%s' ,Building_Name = '%s' "), m_strMainBuildingName, m_strMainBuildingName);
		SqliteDBBuilding.execDML((UTF8MBSTR)sql);
		q.nextRow();

	}

	if (SqliteDBBuilding.open((UTF8MBSTR)BuildingPath) == SQLITE_OK)
	{
		SqliteDBBuilding.closedb();
	}



#endif


    m_bChanged=TRUE;
}


void CBuildingConfigration::OnBuildingconfigUnselect()
{


    CString m_select,m_strMainBuildingName;

    m_strMainBuildingName=m_building_config_list.GetItemText(m_curRow,BC_MAINNAME);
    m_select=m_building_config_list.GetItemText(m_curRow,m_curCol);
    if (m_select.IsEmpty())
    {
        AfxMessageBox(_T("you have already Unselected.\ndon't need to unselect ,again!"));
        return;
    }
   CppSQLite3DB SqliteDBT3000;
   SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
    try
    {

        CString execute_str=_T("update Building set Default_SubBuilding = 0 where Default_SubBuilding =  1");
       SqliteDBT3000.execDML((UTF8MBSTR)execute_str);
    }
    catch (_com_error *e)
    {
        AfxMessageBox(_T("Operator Failed"));
    }
    SqliteDBT3000.closedb();
    m_bChanged=TRUE;
    //m_building_config_list.SetItemText(m_curRow,m_curCol,L"");
//	Fresh_List();
    Initial_Building_List();
}


void CBuildingConfigration::OnBuildingconfigDelete()
{

    CString m_select,m_strMainBuildingName,filename;

    m_strMainBuildingName=m_building_config_list.GetItemText(m_curRow,BC_MAINNAME);
    m_select=m_building_config_list.GetItemText(m_curRow,m_curCol);
	int sqlID  = m_BuildNameLst.at(m_curRow).ID;
    CString buildingPath=m_building_config_list.GetItemText(m_curRow,BC_BUILDINGPATH);
    if(m_curRow>=(m_building_config_list.GetRowCount()-1) || m_curRow <0 )
    {
        return;
    }
    if (!m_select.IsEmpty())
    {
        AfxMessageBox(_T("The current building is using,Can't delete it !"));
        return;
    }

   CppSQLite3DB SqliteDBT3000;
   SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
    CString strSql;
    strSql.Format(_T("delete   from Building where  Main_BuildingName='%s'"),m_strMainBuildingName);
    try
    {


        CString strTemp;
        strTemp.Format(_T("Are you sure to delete the building:'%s'"),m_strMainBuildingName);
        if(AfxMessageBox(strTemp,MB_OKCANCEL)==IDOK)
        {
             SqliteDBT3000.execDML((UTF8MBSTR)strSql);
        }
    }
    catch (_com_error *e)
    {
        AfxMessageBox(_T("Operator Failed"));
    }
    SqliteDBT3000.closedb();

	int ID = m_BuildNameLst.at(m_curRow).ID;

	 
	 

    CString PathTemp;
    PathTemp=GetExePath(true)+buildingPath;
    buildingPath=PathTemp;
    CStringArray  ArrayFileName;
    SplitCStringA(ArrayFileName,buildingPath,L"\\");
    m_BuildNameLst.clear();
    filename=L"";
    for (int i=0; i<ArrayFileName.GetSize()-1; i++)
    {
        filename+=ArrayFileName[i];
        filename+=L"\\";
    }
    //DeleteFile(buildingPath);
    DeleteDirectory(filename);
    m_building_config_list.DeleteItem(m_curRow);
    /*LoadBuildingConfigDB();
    Initial_Building_List();*/
    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_PROTOCOL))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<(int)sizeof(Building_Protocol)/sizeof(Building_Protocol[0]); j++)
        {
            strlist.push_back(Building_Protocol[j]);
        }
        m_building_config_list.SetCellStringList(m_curRow, BC_PROTOCOL, strlist);
    }

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_BAUDRATE))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<(int)sizeof(Building_Baudrate)/sizeof(Building_Baudrate[0]); j++)
        {
            strlist.push_back(Building_Baudrate[j]);
        }
        m_building_config_list.SetCellStringList(m_curRow, BC_BAUDRATE, strlist);
    }

    if(ListCtrlEx::ComboBox == m_building_config_list.GetColumnType(BC_COMPORT))
    {
        ListCtrlEx::CStrList strlist;
        for (int j=0; j<m_szBuildingComs.size(); j++)
        {
            strlist.push_back(m_szBuildingComs.at(j));
        }
        m_building_config_list.SetCellStringList(m_curRow, BC_COMPORT, strlist);
    }

    m_bChanged=TRUE;
    // Fresh_List();
    //Initial_Building_List();
    //PostMessage(WM_FRESH_DB,0,0);
}


LRESULT CBuildingConfigration::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    
    if (message==WM_FRESH_DB)
    {
        Fresh_List_Row();
        // Initial_Building_List();
    }
    return CDialogEx::WindowProc(message, wParam, lParam);
}


void CBuildingConfigration::OnBnClickedOk()
{
    
    //CDialogEx::OnOK();
}


void CBuildingConfigration::OnNMDblclkListBuildingConfig(NMHDR *pNMHDR, LRESULT *pResult)
{

   LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CString temp_cstring;
	long lRow,lCol;
	m_building_config_list.Set_Edit(true);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_building_config_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_building_config_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;
	m_curCol=lCol;
	m_curRow=lRow;
	CString Strprotcol=m_building_config_list.GetItemText(lRow,BC_PROTOCOL);
	BOOL Is_The_Same=TRUE;
	m_select_text=m_building_config_list.GetItemText(lRow,lCol);
	if(lRow>m_building_config_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;
	if (m_curRow>=m_BuildNameLst.size())//点的行超过了数据的size
	{
		return;
	}
    CBuildingConfigEditDlg dlg;
	dlg.m_currentBuilding = m_BuildNameLst.at(m_curRow);
	if (dlg.DoModal() == IDOK)
	 {
		 
		m_BuildNameLst.at(m_curRow) = dlg.m_currentBuilding;
		CppSQLite3DB SqliteDBT3000;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
	 
		try
		{
			_variant_t temp_var;
		 
			CString SqlText;
			SqlText.Format(_T("Update Building Set \
			 country = '%s' ,\
			 state = '%s' ,\
			 city = '%s' ,\
			 street = '%s' ,\
			 EngineeringUnits = '%s'\
			 where Building_Path = '%s' \
			  "),
			  m_BuildNameLst.at(m_curRow).country,
			  m_BuildNameLst.at(m_curRow).state,
			  m_BuildNameLst.at(m_curRow).city,
			  m_BuildNameLst.at(m_curRow).street,
			  m_BuildNameLst.at(m_curRow).EngineeringUnits,
			  m_BuildNameLst.at(m_curRow).BuildingPath
			  );
			  
			SqliteDBT3000.execDML((UTF8MBSTR)SqlText);
			SqliteDBT3000.closedb();
		 
			AfxMessageBox(_T("Update Successfully"));

		}
		catch(CException* e)
		{

			e->GetErrorMessage(_T("Exception"), 0);
		}
	 }
	  
	*pResult = 0;
}
