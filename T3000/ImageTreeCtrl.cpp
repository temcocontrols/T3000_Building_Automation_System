// ImageTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "ImageTreeCtrl.h"
#include "../SQLiteDriver/CppSQLite3.h"
#include "MainFrm.h"
#include "global_function.h"
#include "CBacnetBuilidngAddNode.h"
#include "BacnetAddRemoteDevice.h"
#include "CBacnetBuildingCommunicate.h"
#include "CBacnetBuildingProperty.h"
extern CString cs_bm_ini;
// CImageTreeCtrl
enum ECmdHandler {
	ID_RENAME = 1,
	ID_DELETE,
	ID_ADD_SIBLING,
	ID_ADD_CHILD,
	ID_ADD_ROOT,
	ID_SORT_LEVEL,
	ID_SORT_LEVELANDBELOW,
	ID_SORT_BY_CONNECTION,
	ID_SORT_BY_FLOOR,
	ID_PROJECT_POINT_VIEW,
	ID_PING_CMD,
	ID_ADD_VIRTUAL_DEVICE,
	ID_ADD_CUSTOM_DEVICE,
    ID_ADD_REMOTE_DEVICE,
	ID_BM_RENAME,
	ID_BM_DELETE,
	ID_BM_ADD_GROUPS,
	ID_BM_ADD_NODES,
	ID_BM_ADD_MODULE,
	ID_BM_ADD_INPUT,
	ID_BM_ADD_OUTPUT,
	ID_BM_ADD_VARIABLE,
	ID_BM_IO_PROPERTY,
	ID_BM_BUILDING_COMMUNICATE,
	ID_BM_VIRTUAL_TREE,
	ID_SAVE_INTO_TREE_DEVICE,
	
};
int ID_MAX_CMD = 1000;
enum ERightDragHandler {
	ID_DRAG_COPY = 15,
	ID_DRAG_MOVE,
	ID_DRAG_CANCEL,

	ID_MAX_DRH
};
extern BM_dlg_ret dlg_ret;
BM_nodeinfo operation_nodeinfo;
 DWORD WINAPI _Background_Write_Name(LPVOID pParam){
  CImageTreeCtrl* dlg=(CImageTreeCtrl*)(pParam);
     CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
     for (int i=0;i< pFrame->m_product.size();i++){
         if (dlg->m_hSelItem == pFrame->m_product.at(i).product_item)
         {  
              CString strIPAddress;
              int IPPort=0;
              int ComPort=0;
              int brandrate = 0;
              int ModbusID=0;
             CString strSql,temp_serial;
            unsigned int sn=pFrame->m_product.at(i).serial_number;
            temp_serial.Format(_T("%u"),sn);
            int  int_product_type = pFrame->m_product.at(i).product_class_id;
            
            //if (product_register_value[714] == 0x56)
            //{
                int communicationType = pFrame->m_product.at(i).protocol;  
                ModbusID = pFrame->m_product.at(i).product_id;
                SetCommunicationType(communicationType);
                if (communicationType==0)
                {
                    
                    ComPort =  pFrame->m_product.at(i).ncomport;
                    brandrate = pFrame->m_product.at(i).baudrate;
                   
                    if (open_com(ComPort))
                    {
                        Change_BaudRate(brandrate);
                        
                        char cTemp1[16];
                        memset(cTemp1,0,16);
                        WideCharToMultiByte( CP_ACP, 0, dlg->m_name_new.GetBuffer(), -1, cTemp1, 16, NULL, NULL ); 
                        unsigned char Databuffer[16];
                        memcpy_s(Databuffer,16,cTemp1,16);
                        if (Write_Multi(ModbusID,Databuffer,715,16,10)>0)
                        {
                             WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("0"),g_achive_device_name_path);
                        }
                        
                        
                        close_com();

                    }
                } 
                else
                {
                    strIPAddress = pFrame->m_product.at(i).BuildingInfo.strIp;
                    IPPort = pFrame->m_product.at(i).ncomport;
                    if (Open_Socket2(strIPAddress,IPPort))
                    {
                        if(dlg->m_name_new.GetLength()> 17)	//长度不能大于结构体定义的长度;
                        {
                            dlg->m_name_new.Delete(16,dlg->m_name_new.GetLength()-16);
                        }

                        char cTemp1[16];
                        memset(cTemp1,0,16);
                        WideCharToMultiByte( CP_ACP, 0, dlg->m_name_new.GetBuffer(), -1, cTemp1, 16, NULL, NULL ); 
                        unsigned char Databuffer[16];
                        memcpy_s(Databuffer,16,cTemp1,16);
                        if (Write_Multi(ModbusID,Databuffer,715,16,10)>0)
                        {
                             
                                 
                                WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("0"),g_achive_device_name_path);
                           
                        }
                    }
                      
                }

                
            //} 

            break;
         }
         }

         return 1;
 }
IMPLEMENT_DYNAMIC(CImageTreeCtrl, CTreeCtrl)
CImageTreeCtrl::CImageTreeCtrl()
{
	m_dwDrawStart = 0;
	m_nMsMoveTimerID = 0;
	m_nScrollTimerID = 0;
	m_nTicks = 0;
	m_bDraged = false;
	m_pDragImage = NULL;


	m_nSubnetItemData = 9000;
	m_nFloorItemData = 1000;
	m_nRoomItemData = 2000;
	m_nDeviceItemData = 3000;
	m_Keymap[VK_F2][false][false] = &CImageTreeCtrl::DoEditLabel;
    m_Keymap[VK_DELETE][false][false] = &CImageTreeCtrl::DoDeleteItem;
	m_Commandmap[ID_RENAME]		        = &CImageTreeCtrl::DoEditLabel;
	m_Commandmap[ID_DELETE]             = &CImageTreeCtrl::DoDeleteItem;
	m_Commandmap[ID_SORT_BY_CONNECTION]		     = &CImageTreeCtrl::SortByConnection;
	m_Commandmap[ID_SORT_BY_FLOOR]		        = &CImageTreeCtrl::SortByFloor;
	m_Commandmap[ID_PROJECT_POINT_VIEW] = &CImageTreeCtrl::ProjectPointView;
	
	m_Commandmap[ID_PING_CMD]		        = &CImageTreeCtrl::PingDevice;
	m_Commandmap[ID_ADD_VIRTUAL_DEVICE]     = &CImageTreeCtrl::HandleAddVirtualDevice;
	m_Commandmap[ID_ADD_CUSTOM_DEVICE]      = &CImageTreeCtrl::HandleAddCustomDevice;
    m_Commandmap[ID_ADD_REMOTE_DEVICE] =      &CImageTreeCtrl::HandleAddRemoteDevice;
    //m_Commandmap[ID_ADD_BACNET_DEVICE] =      &CImageTreeCtrl::HandleAddThirdPartBacnetDevice;
	m_Commandmap[ID_BM_RENAME] = &CImageTreeCtrl::DoEditLabel;
    m_Commandmap[ID_BM_ADD_GROUPS] = &CImageTreeCtrl::BM_Add_Groups;
	m_Commandmap[ID_BM_ADD_NODES] = &CImageTreeCtrl::BM_Add_Nodes;
	m_Commandmap[ID_BM_ADD_MODULE] = &CImageTreeCtrl::BM_Add_Module;
	m_Commandmap[ID_BM_ADD_INPUT] = &CImageTreeCtrl::BM_Add_Inputs;
	m_Commandmap[ID_BM_ADD_OUTPUT] = &CImageTreeCtrl::BM_Add_Outputs;
	m_Commandmap[ID_BM_ADD_VARIABLE] = &CImageTreeCtrl::BM_Add_Variable;
	m_Commandmap[ID_BM_IO_PROPERTY] = &CImageTreeCtrl::BM_IO_Mapping;
	m_Commandmap[ID_BM_DELETE] = &CImageTreeCtrl::BM_Delete;
	m_Commandmap[ID_BM_BUILDING_COMMUNICATE] = &CImageTreeCtrl::BM_Communicate;
	m_Commandmap[ID_BM_VIRTUAL_TREE] = &CImageTreeCtrl::ShowEquipmentView;
	m_Commandmap[ID_SAVE_INTO_TREE_DEVICE] = &CImageTreeCtrl::SyncToController;
	
	old_hItem = NULL;
	m_serial_number = 0;
	is_focus = false;
	tree_offline_mode = false;
	m_virtual_tree_item = NULL;
    Inial_ProductName_map();
    Initial_Instance_Reg_Map();
    m_hSelItem = NULL;

}

CImageTreeCtrl::~CImageTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CImageTreeCtrl, CTreeCtrl)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND_RANGE(ID_RENAME, ID_MAX_CMD-1, OnContextCmd)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()

    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
    ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)

	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CImageTreeCtrl::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CImageTreeCtrl::OnBegindrag)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//ON_MESSAGE(WM_RETRIEVE_CHECKSTATE, &CImageTreeCtrl::OnRetriveCheckState)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, &CImageTreeCtrl::OnNMClick)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CImageTreeCtrl::OnTvnEndlabeledit)

	//ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CImageTreeCtrl::OnTvnEndlabeledit)

END_MESSAGE_MAP()
void CImageTreeCtrl::OnContextCmd(UINT id) {
	HTREEITEM hCur = GetSelectedItem();
	if (id >= 100)
	{
		int original_serial = 0;
		CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
		for (UINT i = 0; i < pFrame->m_product.size(); i++)
		{
			if (hCur == pFrame->m_product.at(i).product_item)
			{
				original_serial = pFrame->m_product.at(i).serial_number;
				break;
			}
		}
		if (original_serial == 0)
		{
			MessageBox(_T("Sync to controller failed , please select a device first!"));
			return;
		}
		::PostMessage(MainFram_hwd, WM_WRITE_INTO_NEW_DEVICE, WPARAM(original_serial), LPARAM(m_online_serial.at(id - 100).nserialnumber));
		Sleep(1);
		return;
	}
	method fnc = m_Commandmap[id];
	if(fnc) {
		(this->*fnc)(hCur);
		return;
	}
	ASSERT(false);
}
bool CImageTreeCtrl::DoEditLabel(HTREEITEM hItem) 
{
	HTREEITEM hSelectItem = NULL;
	if (b_building_management_flag == SYS_DB_BUILDING_MODE)
	{
		if (operation_nodeinfo.node_type == TYPE_BM_POINT_LIST)
		{
			if (m_BMpoint->BuildingNode.h_treeitem != NULL)
			{
				hSelectItem = m_BMpoint->BuildingNode.h_treeitem;
			}
		}
		else if (operation_nodeinfo.node_type == TYPE_BM_GROUP)
		{
			if (m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->h_treeitem != NULL)
				hSelectItem = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->h_treeitem;
		}
		else if (operation_nodeinfo.node_type == TYPE_BM_NODES)
		{
			if (m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->h_treeitem != NULL)
				hSelectItem = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->h_treeitem;
		}
		else if ((operation_nodeinfo.node_type == TYPE_BM_INPUT) ||
			(operation_nodeinfo.node_type == TYPE_BM_OUTPUT) ||
			(operation_nodeinfo.node_type == TYPE_BM_VARIABLE))
		{
			if (m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->pchild[operation_nodeinfo.child_io]->h_treeitem != NULL)
				hSelectItem = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->pchild[operation_nodeinfo.child_io]->h_treeitem;
		}
		
		return hSelectItem ? (EditLabel(hSelectItem) != 0) : false;
		//BM_Rename(hItem);
		//return 0;
	}
	m_hSelItem=hItem;


	//root 节点和root 下一级的节点不允许更改名字;
	HTREEITEM root_item = CImageTreeCtrl::GetRootItem();
	
	if(hItem == root_item)
	{
		return false;
	}
	if(CImageTreeCtrl::ItemHasChildren(root_item))
	{
		HTREEITEM hChildItem =	CImageTreeCtrl::GetChildItem(root_item);
		while(hChildItem != NULL)
		{
			if(hChildItem == hItem)
			{
				return false;
				break;
			}
			hChildItem = CImageTreeCtrl::GetNextItem(hChildItem, TVGN_NEXT);


		}
	}



	return hItem ? (EditLabel(hItem) != 0) : false;
}



bool CImageTreeCtrl::SyncToController(HTREEITEM hItem)
{
	return 1;
}

bool CImageTreeCtrl::PingDevice(HTREEITEM hItem) 
{
	::PostMessage(MainFram_hwd, WM_PING_MESSAGE,(WPARAM)hItem,NULL);
//	m_strPingIP = strIP;

	return true;
}

#include "BacnetAddVirtualDevice.h"
bool CImageTreeCtrl::HandleAddVirtualDevice(HTREEITEM)
{
	CBacnetAddVirtualDevice popdlg;
	popdlg.DoModal();
	return true;
}
#include "ARDDlg.h"
bool CImageTreeCtrl::HandleAddCustomDevice(HTREEITEM hItem)
{
    bool findhitem = false;
    int hitem_index = 0;
    CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
    for (UINT i = 0; i < pFrame->m_product.size(); i++)
    {
        if (hItem == pFrame->m_product.at(i).product_item)
        {
            findhitem = true;
            hitem_index = i;
            break;
        }
    }

	CARDDlg popdlg;
	popdlg.DoModal();
    ::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING, 0, 0);
	return true;
}


bool CImageTreeCtrl::HandleAddRemoteDevice(HTREEITEM)
{
    CBacnetAddRemoteDevice RemoteDlg;
    RemoteDlg.DoModal();
    CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
    ::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING, 0, 0);
    return true;
}

//bool CImageTreeCtrl::HandleAddThirdPartBacnetDevice(HTREEITEM)
//{
//    //CBacnetAddRemoteDevice RemoteDlg;
//    //RemoteDlg.DoModal();
//    //CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
//    //::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING, 0, 0);
//    return true;
//}

void reset_dlg_data()
{
	dlg_ret.m_BM_ret_count = 0;
	dlg_ret.m_bm_add_dlg_resault = 0;
	dlg_ret.m_BM_ret_function = 255;
	dlg_ret.m_BM_ret_name.Empty();
	dlg_ret.m_BM_ret_type = 255;
}

bool CImageTreeCtrl::ShowEquipmentView(HTREEITEM hItem)
{
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if (b_building_management_flag == SYS_DB_BUILDING_MODE)
	{
		b_building_management_flag = SYS_NORMAL_MODE;
		pFrame->ClearBuilding();
		DeleteAllItems();
		pFrame->m_product.clear();
		pFrame->ScanTstatInDB();
	}
	else if (b_building_management_flag == SYS_NORMAL_MODE)
	{
		b_building_management_flag = SYS_DB_BUILDING_MODE;
		pFrame->ClearBuilding();
		pFrame->m_pTreeViewCrl->DeleteAllItems();
		pFrame->m_product.clear();
		pFrame->SwitchToPruductType(DLG_DIALOG_BUILDING_MANAGEMENT);
	}





	return 0;


}

bool CImageTreeCtrl::BM_Communicate(HTREEITEM hItem)
{
	CBacnetBuildingCommunicate dlg;
	dlg.DoModal();
	return 0;
}

bool CImageTreeCtrl::BM_Delete(HTREEITEM hItem)
{
	//operation_nodeinfo
	//DeleteItem(myiterator->product_item);

	HTREEITEM htree_delete = NULL; //需要删除的节点;
	if (operation_nodeinfo.node_type == TYPE_BM_POINT_LIST)
	{
		htree_delete = m_BMpoint->BuildingNode.h_treeitem;

	}
	else if (operation_nodeinfo.node_type == TYPE_BM_GROUP)
	{
		htree_delete = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->h_treeitem;
		delete m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group];
	}
	else if (operation_nodeinfo.node_type == TYPE_BM_NODES)
	{
		htree_delete = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->h_treeitem;
		delete m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device];
	}
	else if ((operation_nodeinfo.node_type == TYPE_BM_INPUT) ||
		(operation_nodeinfo.node_type == TYPE_BM_OUTPUT) ||
		(operation_nodeinfo.node_type == TYPE_BM_VARIABLE))
	{
		htree_delete = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->pchild[operation_nodeinfo.child_io]->h_treeitem;
		delete m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->pchild[operation_nodeinfo.child_io];

		Sleep(1);
	}
	else
	{
		return 0;
	}
	DeleteItem(htree_delete);

	m_BMpoint->SaveAllIntoIniFile();
	m_BMpoint->UpdateList();
	m_BMpoint->LoadFileShowToList();

	return true;
}



bool CImageTreeCtrl::BM_Add_Groups(HTREEITEM hItem)
{
	BM_Adds(hItem, FUNCTION_BM_ADD, TYPE_BM_GROUP);
	return true;
}
bool CImageTreeCtrl::BM_Add_Nodes(HTREEITEM hItem)
{
	BM_Adds(hItem, FUNCTION_BM_ADD, TYPE_BM_NODES);
	return true;
}
bool CImageTreeCtrl::BM_Add_Module(HTREEITEM hItem)
{
	BM_Adds(hItem, FUNCTION_BM_ADD, TYPE_BM_MODULE);
	return true;
}

void CImageTreeCtrl::BM_Property(HTREEITEM hItem)
{
	//确定选中的节点是Node 还是IO  并且是哪一个Node 哪一个IO
	Sleep(1);
}

void CImageTreeCtrl::BM_Adds(HTREEITEM hItem,int nfunction, int ntype)
{
	reset_dlg_data();
	CBacnetBuilidngAddNode Dlg;
	Dlg.SetParameter(nfunction, ntype);
	//Dlg.SetParameter(FUNCTION_BM_ADD, TYPE_BM_INPUT);
	Dlg.DoModal();

	if (dlg_ret.m_bm_add_dlg_resault == 1)
	{
		if ((ntype == TYPE_BM_INPUT) || (ntype == TYPE_BM_OUTPUT) || (ntype == TYPE_BM_VARIABLE))
		{
			//operation_nodeinfo.
			HTREEITEM htree_node = NULL; //Input 的父节点
			htree_node = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->h_treeitem;
			int exist_count; // 此前的哥哥节点;
			exist_count = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->m_child_count;
			CBacnetBMD* temp_node = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]; //ABC123 的节点
			TV_INSERTSTRUCT tvInsert;
			for (int i = 0; i < dlg_ret.m_BM_ret_count; i++)
			{
				CString temp_number;
				if (dlg_ret.m_BM_ret_count > 1)   //当添加的节点个数不唯一的时候才加上数字后缀
					temp_number.Format(_T("%d"), exist_count + 1 + i);
				else
					temp_number.Empty();
				CString strinfo = dlg_ret.m_BM_ret_name + temp_number;
				tvInsert.hParent = htree_node; // 指定父句柄
				tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
				tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strinfo;
				tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
				if (ntype == TYPE_BM_INPUT)
				{
					temp_node->m_input_count++;
					tvInsert.item.iImage = TREE_IMAGE_INPUT_UNKNOWN; tvInsert.item.iSelectedImage = TREE_IMAGE_INPUT_UNKNOWN;
				}
				else if (ntype == TYPE_BM_OUTPUT)
				{
					temp_node->m_output_count++;
					tvInsert.item.iImage = TREE_IMAGE_OUTPUT_ONLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_OUTPUT_ONLINE;
				}
				else if (ntype == TYPE_BM_VARIABLE)
				{
					temp_node->m_variable_count++;
					tvInsert.item.iImage = TREE_IMAGE_VARIABLE_OFFLINE; tvInsert.item.iSelectedImage = TREE_IMAGE_VARIABLE_OFFLINE;
				}
				HTREEITEM hTreeIOList = NULL;
				hTreeIOList = InsertSubnetItem(&tvInsert);//插入PointList
				m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->m_child_count++;

				//因为已经有 old 个兄节点了，只能在后面创建
				if (temp_node->pchild[exist_count + i] == NULL)
					temp_node->pchild[exist_count + i] = new CBacnetBMD;
				CBacnetBMD* io_node = temp_node->pchild[exist_count + i];
				io_node->hParent = temp_node->h_treeitem;
				io_node->h_treeitem = hTreeIOList;
				io_node->m_child_count = 0;
				io_node->m_csName = strinfo;
				io_node->m_index = exist_count + i;
				//io_node->m_node_type = TYPE_BM_INPUT;
				io_node->m_node_type = ntype;
				io_node->pfather = temp_node;

				//新增节点，添加信息，保存到数据库;
			}
			if (htree_node != NULL)
				Expand(htree_node, TVE_EXPAND);
		}
		else if (ntype == TYPE_BM_NODES)
		{
			HTREEITEM hgroup_node = NULL; //Input 的父节点
			hgroup_node = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->h_treeitem;
			int exist_device_count; // 此前的哥哥节点;
			exist_device_count = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->m_child_count;
			CBacnetBMD* temp_group = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]; //ABC Group 的节点
			TV_INSERTSTRUCT tvInsert;
			for (int i = 0; i < dlg_ret.m_BM_ret_count; i++)
			{
				CString temp_number;
				if (dlg_ret.m_BM_ret_count > 1)   //当添加的节点个数不唯一的时候才加上数字后缀
					temp_number.Format(_T("%d"), exist_device_count + 1 + i);
				else
					temp_number.Empty();
				CString strinfo = dlg_ret.m_BM_ret_name + temp_number;
				tvInsert.hParent = hgroup_node; // 指定父句柄
				tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
				tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strinfo;
				tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式

				//这里选择device的图标
				//if (i == 0)
				//	TVINSERV_MINIPANEL   //这里到时候要判断到底是什么设备
				//else if (i == 1)
				//	TVINSERV_TSTAT8
				//else if (i == 2)
				//	TVINSERV_T3ARM
				//else
					TVINSERV_MINIPANEL


				HTREEITEM hTreeDeviceList = NULL;
				hTreeDeviceList = InsertSubnetItem(&tvInsert);//插入DeviceList
				temp_group->m_child_count++;

				//因为已经有 old 个兄节点了，只能在后面创建
				if (temp_group->pchild[exist_device_count + i] == NULL)
					temp_group->pchild[exist_device_count + i] = new CBacnetBMD;
				CBacnetBMD* device_node = temp_group->pchild[exist_device_count + i];
				device_node->hParent = temp_group->h_treeitem;
				device_node->h_treeitem = hTreeDeviceList;
				device_node->m_child_count = 0;
				device_node->m_csName = strinfo;
				device_node->m_index = exist_device_count + i;
				//io_node->m_node_type = TYPE_BM_INPUT;
				device_node->m_node_type = ntype;
				device_node->pfather = temp_group;
				//新增节点，添加信息，保存到数据库;
			}
			if (hgroup_node != NULL)
				Expand(hgroup_node, TVE_EXPAND);
			Sleep(1);
		}
		else if (ntype == TYPE_BM_GROUP)
		{
		HTREEITEM hpointlist_node = NULL; //Group 的父节点
		hpointlist_node = m_BMpoint->BuildingNode.h_treeitem;
		int exist_group_count; // 此前的哥哥节点;
		exist_group_count = m_BMpoint->BuildingNode.m_child_count;
		CBacnetBMD* temp_root = &(m_BMpoint->BuildingNode); //Group 的节点
		TV_INSERTSTRUCT tvInsert;
		for (int i = 0; i < dlg_ret.m_BM_ret_count; i++)
		{
			CString temp_number;
			if (dlg_ret.m_BM_ret_count > 1)   //当添加的节点个数不唯一的时候才加上数字后缀
				temp_number.Format(_T("%d"), exist_group_count + 1 + i);
			else
				temp_number.Empty();
			CString strinfo = dlg_ret.m_BM_ret_name + temp_number;
			tvInsert.hParent = hpointlist_node; // 指定父句柄
			tvInsert.item.mask = ITEM_MASK; // 指定TV_ITEM结构对象
			tvInsert.item.pszText = (LPTSTR)(LPCTSTR)strinfo;
			tvInsert.hInsertAfter = TVI_LAST; // 项目插入方式
			TVINSERV_ROOM

				HTREEITEM hTreeGroupList = NULL;
			hTreeGroupList = InsertSubnetItem(&tvInsert);//插入DeviceList
			temp_root->m_child_count++;

			//因为已经有 old 个兄节点了，只能在后面创建
			if (temp_root->pchild[exist_group_count + i] == NULL)
				temp_root->pchild[exist_group_count + i] = new CBacnetBMD;
			CBacnetBMD* group_node = temp_root->pchild[exist_group_count + i];
			group_node->hParent = temp_root->h_treeitem;
			group_node->h_treeitem = hTreeGroupList;
			group_node->m_child_count = 0;
			group_node->m_csName = strinfo;
			group_node->m_index = exist_group_count + i;
			group_node->m_node_type = ntype;
			group_node->pfather = temp_root;
			//新增节点，添加信息，保存到数据库;
		}
		if (hpointlist_node != NULL)
			Expand(hpointlist_node, TVE_EXPAND);
		Sleep(1);

		}
		m_BMpoint->BuildingNode.UpdateCount();

		Invalidate();
		m_BMpoint->SaveAllIntoIniFile();
		m_BMpoint->UpdateList();

	}
}

bool CImageTreeCtrl::BM_Add_Inputs(HTREEITEM hItem)
{
	BM_Adds(hItem, FUNCTION_BM_ADD, TYPE_BM_INPUT);

	return true;
}

bool CImageTreeCtrl::BM_Add_Outputs(HTREEITEM hItem)
{
	BM_Adds(hItem, FUNCTION_BM_ADD, TYPE_BM_OUTPUT);
	return true;
}

bool CImageTreeCtrl::BM_Add_Variable(HTREEITEM hItem)
{
	BM_Adds(hItem, FUNCTION_BM_ADD, TYPE_BM_VARIABLE);
	return true;
}

bool CImageTreeCtrl::BM_IO_Mapping(HTREEITEM hItem)
{
	//先删除本group下其他兄弟节点下面的 所有数据


	CBacnetBMD* group_point;
	CBacnetBMD* node_point;
	CBacnetBMD* source_point;

	group_point = m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group];
	

	for (int i = 0; i < group_point->m_child_count; i++)
	{
		node_point = group_point->pchild[i];
		if (node_point->m_index == operation_nodeinfo.child_device)
			continue;
		source_point = group_point->pchild[operation_nodeinfo.child_device];
		for (int j = 0; node_point->m_child_count!=0; j = 0)  //循环删除兄弟  节点下面的 子节点
		{
			delete node_point->pchild[j];
			DeleteItem(node_point->h_treeitem);
		}

		//添加一模一样的   兄弟  节点下面的 子节点
		for (int k = 0; k < source_point->m_child_count; k++)
		{
			node_point->m_child_count++;


			node_point->pchild[k] = new CBacnetBMD;
			CBacnetBMD* io_node = node_point->pchild[k];
			io_node->hParent = node_point->h_treeitem;
			//io_node->h_treeitem = hTreeIOList;
			io_node->m_child_count = 0;
			io_node->m_csName = source_point->pchild[k]->m_csName;
			io_node->m_index = k;
			//io_node->m_node_type = TYPE_BM_INPUT;
			io_node->m_node_type = source_point->pchild[k]->m_node_type;
			io_node->pfather = node_point;
		}

	}

	m_BMpoint->BuildingNode.UpdateCount();


	m_BMpoint->SaveAllIntoIniFile();
	m_BMpoint->UpdateList();
	m_BMpoint->LoadFileShowToList();

	BM_Property(hItem);
	CBacnetBuildingProperty PropertyDlg;
	PropertyDlg.DoModal();
	return true;
}


bool CImageTreeCtrl::BM_Rename(HTREEITEM hItem)
{
	AfxMessageBox(_T("Add Rename"));
	Sleep(1);
	return true;
}


bool CImageTreeCtrl::ProjectPointView(HTREEITEM hItem)
{
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	pFrame->OnDatabaseBuildingManagement();
	return 0;
}
bool CImageTreeCtrl::SortByConnection(HTREEITEM hItem) 
{
	if(product_sort_way != SORT_BY_CONNECTION)
	{
		WritePrivateProfileStringW(_T("Setting"),_T("ProductSort"),_T("1"),g_cstring_ini_path);
		product_sort_way = SORT_BY_CONNECTION;
		CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		::PostMessage(pFrame->m_hWnd,WM_MYMSG_REFRESHBUILDING,0,0);
	}

	return true;
}

bool CImageTreeCtrl::SortByFloor(HTREEITEM hItem) 
{
	if(product_sort_way != SORT_BY_BUILDING_FLOOR)
	{
		WritePrivateProfileStringW(_T("Setting"),_T("ProductSort"),_T("2"),g_cstring_ini_path);
		product_sort_way = SORT_BY_BUILDING_FLOOR;
		CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
		::PostMessage(pFrame->m_hWnd,WM_MYMSG_REFRESHBUILDING,0,0);
	}
	return true;
}

int CImageTreeCtrl::GetOnlineDevice()
{
	m_online_serial.clear();
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;

	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	;
	CString building_path;
	building_path = g_strBuildingFolder +  pFrame->m_strCurSubBuldingName + _T("\\") + pFrame->m_strCurSubBuldingName + _T(".db");
	SqliteDBT3000.open((UTF8MBSTR)building_path);

	CString strSql;
	CString str_temp_name;
	CString str_temp_serialnumber;
	strSql.Format(_T("select * from ALL_NODE where Online_Status = 1 and (Product_class_ID = 74 or Product_class_ID = 88 or Product_class_ID = 35)"));
	q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);


	while (!q.eof())
	{
		Str_online_serialnumber temp;
		str_temp_name.Empty();
		str_temp_name = q.getValuebyName(L"Product_name");
		str_temp_serialnumber.Empty();
		str_temp_serialnumber = q.getValuebyName(L"Serial_ID");
		temp.online_name = str_temp_name;
		temp.nserialnumber = _wtoi(str_temp_serialnumber);
		q.nextRow();
		m_online_serial.push_back(temp);
	}
	return 1;
}

BOOL CImageTreeCtrl::UpdateDataToDB_Floor(){
	
    
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
    SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

 
	try 
	{
		////////////////////////////////////////////////////////////////////////////////////////////
		//获取数据库名称及路径
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//连接数据库
	    
		CString strSql;   BOOL is_exist=FALSE;	  CString str_temp;
		switch (m_level)
		{
		case 0:  //Subnet
			{


				//strSql.Format(_T("select * from Building where Main_BuildingName = '%s'"),m_strMainBuildingName);
				strSql.Format(_T("select * from Building order by Main_BuildingName"));
				//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
				q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
				while(!q.eof())
				{	   	
				str_temp.Empty();
				str_temp=q.getValuebyName(L"Building_Name");
				if (str_temp.Compare(m_name_new)==0)
				{	   
					is_exist=TRUE;
					break;
				}			
				q.nextRow();
				}
				 
				if (!is_exist)	 //更新的名字在数据库中查找不到的
				{
 
					 strSql.Format(_T("update Building_ALL set Building_Name='%s' where Building_Name='%s' "),m_name_new,m_name_old);

					SqliteDBT3000.execDML((UTF8MBSTR)strSql);

					strSql.Format(_T("update Building set Building_Name='%s',Main_BuildingName='%s' where Building_Name='%s'"),m_name_new,m_name_new,m_name_old);
					//MessageBox(strSql);
					SqliteDBT3000.execDML((UTF8MBSTR)strSql);


					strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' order by Building_Name"),m_name_old);
				     q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
				   
					while(!q.eof())
					{
						strSql.Format(_T("update ALL_NODE set Building_Name='%s',MainBuilding_Name='%s' where Building_Name='%s'"),m_name_new,m_name_new,m_name_old);
						SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
						q.nextRow();
					}

				 
				
				}
				else
				{

					return FALSE;
				}

				break;	
			}
		case 1:		//Floor
			{	 
			CString subnetname;
			     strSql=_T("select * from Building where Default_SubBuilding=1");
				 
				q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
				 while(!q.eof())
				 {	   	
				  subnetname.Empty();
				  subnetname= q.getValuebyName(L"Building_Name");	
				  q.nextRow();
				 }
				 
				 
 
				strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' order by Building_Name"),subnetname,m_name_new);
				//m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
				 
				 q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
				while(!q.eof())
				{	   	 str_temp.Empty();
				str_temp=q.getValuebyName(L"Floor_name");
				if (str_temp.Compare(m_name_new)==0)
				{is_exist=TRUE;
				break;
				}			
				q.nextRow();
				}
				 
				{  
				strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' order by Building_Name"),subnetname,m_name_old);
			 
				 q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
				while(!q.eof())
				{
					strSql.Format(_T("update ALL_NODE set Floor_name='%s' where Floor_name='%s'"),m_name_new,m_name_old);
					SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
					q.nextRow();
				}
				 
				}

				 
				break;
			}
		case 2:		//Room
			{
				//Subnet
				HTREEITEM root=GetRootItem();
				CString subnetname=GetItemText(root);
				//Floor
				HTREEITEM parentnode=GetParentItem(m_hSelItem);
				CString Floorname=GetItemText(parentnode);
				 
			 
				strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' and Room_name='%s' order by Building_Name"),subnetname,Floorname,m_name_old);
				 q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
				while(!q.eof())
				{
					strSql.Format(_T("update ALL_NODE set Room_name='%s' where Room_name='%s'"),m_name_new,m_name_old);
					 
					SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
					q.nextRow();
				}
		 
				 
				 


				break;
			}
		case 3:		//Device Name Leaf
			{
             #if 1
                if (m_name_old.CompareNoCase(m_name_new)==0)
                {
                    return FALSE;
                }
                CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
                for (int i=0;i< pFrame->m_product.size();i++){
                    if (m_hSelItem == pFrame->m_product.at(i).product_item)
                    {     

                        CString strIPAddress;
                        int IPPort=0;
                        int ComPort=0;
                        int brandrate = 0;
                        int ModbusID=0;
                        CString strSql,temp_serial;
                        int sn=pFrame->m_product.at(i).serial_number;
                        temp_serial.Format(_T("%d"),sn);
                        int  int_product_type = pFrame->m_product.at(i).product_class_id;


                        if((int_product_type == PM_MINIPANEL) || (int_product_type == PM_CM5) || (int_product_type == PM_MINIPANEL_ARM) || (int_product_type == PM_ESP32_T3_SERIES))//||(int_product_type == PM_TSTAT6)
                        {
                            WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
                            WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
                        }
                       
                        if (FALSE)    //
                        {
                            int communicationType = pFrame->m_product.at(i).protocol;  
                            SetCommunicationType(communicationType); 
							ModbusID = pFrame->m_product.at(i).product_id;
                            if (communicationType==0)
                            {
                                ComPort =  pFrame->m_product.at(i).ncomport;
                                brandrate = pFrame->m_product.at(i).baudrate;
                               

                                if (open_com(ComPort))
                                {
                                    Change_BaudRate(brandrate);
                                    if(m_name_new.GetLength()> 17)	//长度不能大于结构体定义的长度;
                                    {
                                        m_name_new.Delete(16,m_name_new.GetLength()-16);
                                    }

                                    char cTemp1[16];
                                    memset(cTemp1,0,16);
                                    WideCharToMultiByte( CP_ACP, 0, m_name_new.GetBuffer(), -1, cTemp1, 16, NULL, NULL ); 
                                    unsigned char Databuffer[16];
                                    memcpy_s(Databuffer,16,cTemp1,16);
                                    if (Write_Multi(ModbusID,Databuffer,715,16,10)<0)
                                    {
                                        if( int_product_type == PM_TSTAT6) 
                                        {
                                            WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
                                            WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
                                        }
                                    }


                                    close_com();

                                }
                            } 
                            else
                            {
                                strIPAddress = pFrame->m_product.at(i).BuildingInfo.strIp;
                                IPPort = _wtoi(pFrame->m_product.at(i).BuildingInfo.strIpPort);
                                if (Open_Socket2(strIPAddress,IPPort))
                                {
                                    if(m_name_new.GetLength()> 17)	//长度不能大于结构体定义的长度;
                                    {
                                        m_name_new.Delete(16,m_name_new.GetLength()-16);
                                    }

                                    char cTemp1[16];
                                    memset(cTemp1,0,16);
                                    WideCharToMultiByte( CP_ACP, 0, m_name_new.GetBuffer(), -1, cTemp1, 16, NULL, NULL ); 
                                    unsigned char Databuffer[16];
                                    memcpy_s(Databuffer,16,cTemp1,16);
                                    if (Write_Multi(ModbusID,Databuffer,715,16,10)<0)
                                    {
                                        if( int_product_type == PM_TSTAT6) 
                                        {
                                            WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
                                            WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
                                        }
                                    }
                                }                              

                            }
                        }            
                        
                        strSql.Format(_T("update ALL_NODE set Product_name='%s' where Product_name='%s' and Serial_ID='%s'"),m_name_new,m_name_old,temp_serial);
                        
                         SqliteDBT3000.execDML((UTF8MBSTR)strSql);
                        
                        if (product_register_value[714]==0x56)
                        {
                            if(m_name_new.GetLength()> 16)	//长度不能大于结构体定义的长度;
                            {
                                m_name_new.Delete(16,m_name_new.GetLength()-16);
                            }
                            WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
                            WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
                            CreateThread(NULL,NULL,_Background_Write_Name,this,NULL,0); 
                        }

                        return TRUE;
                    }
                }
            #endif

				break;
			}
		}
	}
	catch(_com_error e)
	{
		 
		return FALSE;
		//m_pCon->Close();
	}
	 SqliteDBT3000.closedb();
	 SqliteDBBuilding.closedb();
	CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
	::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);

	return TRUE;
}

//typedef map<int, int> panelname_map;
//panelname_map g_panelname_map;
//
//void CImageTreeCtrl::Inial_ProductName_map()
//{
//    g_panelname_map.insert(map<int,int>::value_type(STM32_PRESSURE_NET,901));
//    g_panelname_map.insert(map<int, int>::value_type(STM32_PRESSURE_RS485, 901));
//
//}
//
//int CImageTreeCtrl::PanelName_Map(int product_type)
//{
//    map<int, int>::iterator iter;
//    int test1;
//    iter = g_panelname_map.find(product_type);
//    if (iter != g_panelname_map.end())
//    {
//        test1 = g_panelname_map.at(product_type);
//        return test1;
//    }
//   
//    return 715; // 如果没有默认按照从715 开始 8个寄存器.
//}

BOOL CImageTreeCtrl::UpdateDataToDB_Connect(){
       if (m_level==0)
       {
           return UpdateDataToDB_Floor();
       }
       if (m_name_old.CompareNoCase(m_name_new)==0)
       {
          return FALSE;
       }


       int ccc = PanelName_Map(22);

       ccc = PanelName_Map(99);

    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    for (int i=0;i< pFrame->m_product.size();i++){
      if (m_hSelItem == pFrame->m_product.at(i).product_item)
      {     
          
              CString strIPAddress;
              int IPPort=0;
              int ComPort=0;
              int brandrate = 0;
              int ModbusID=0;
             CString strSql,temp_serial;
            int sn=pFrame->m_product.at(i).serial_number;
            temp_serial.Format(_T("%d"),sn);
            int  int_product_type = pFrame->m_product.at(i).product_class_id;
            int panel_name_start_reg = 0;  //获取对应产品号
            panel_name_start_reg = PanelName_Map(int_product_type);


             WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
             WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);

            

                int communicationType = pFrame->m_product.at(i).protocol;  
                ModbusID = pFrame->m_product.at(i).product_id;
                SetCommunicationType(communicationType);
                if (communicationType==0)
                {
                    
                    ComPort =  pFrame->m_product.at(i).ncomport;
                    brandrate = pFrame->m_product.at(i).baudrate;
                   
                    if (open_com(ComPort))
                    {
                        Change_BaudRate(brandrate);
                        
                        char cTemp1[16];
                        memset(cTemp1,0,16);
                        WideCharToMultiByte( CP_ACP, 0, m_name_new.GetBuffer(), -1, cTemp1, 16, NULL, NULL ); 
                        unsigned char Databuffer[16];
                        memcpy_s(Databuffer,16,cTemp1,16);
                        if (Write_Multi(ModbusID,Databuffer, panel_name_start_reg,16,10)<0)
                        {
                            
                        }
                        
                        
                        close_com();

                    }
                } 
                else
                {
                    strIPAddress = pFrame->m_product.at(i).BuildingInfo.strIp;
                    IPPort = pFrame->m_product.at(i).ncomport;
                    if (Open_Socket2(strIPAddress,IPPort))
                    {
                        if(m_name_new.GetLength()> 17)	//长度不能大于结构体定义的长度;
                        {
                            m_name_new.Delete(16,m_name_new.GetLength()-16);
                        }

                        char cTemp1[16];
                        memset(cTemp1,0,16);
                        WideCharToMultiByte( CP_ACP, 0, m_name_new.GetBuffer(), -1, cTemp1, 16, NULL, NULL ); 
                        unsigned char Databuffer[16];
                        memcpy_s(Databuffer,16,cTemp1,16);
                        if (Write_Multi(ModbusID,Databuffer, panel_name_start_reg,16,10)<0)
                        {
                                WritePrivateProfileStringW(temp_serial,_T("NewName"),m_name_new,g_achive_device_name_path);
                                WritePrivateProfileStringW(temp_serial,_T("WriteFlag"),_T("1"),g_achive_device_name_path);
                          
                        }
                    }
                      
                }

			CppSQLite3DB SqliteDBBuilding;
			CppSQLite3Table table;
			CppSQLite3Query q;
			SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

            strSql.Format(_T("update ALL_NODE set Product_name='%s' where Product_name='%s' and Serial_ID='%s'"),m_name_new,m_name_old,temp_serial);
            SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
            SqliteDBBuilding.closedb();

            if (m_name_new.GetLength() > 16)	//长度不能大于结构体定义的长度;
            {
                m_name_new.Delete(16, m_name_new.GetLength() - 16);
            }
            WritePrivateProfileStringW(temp_serial, _T("NewName"), m_name_new, g_achive_device_name_path);
            WritePrivateProfileStringW(temp_serial, _T("WriteFlag"), _T("1"), g_achive_device_name_path);
            CreateThread(NULL, NULL, _Background_Write_Name, this, NULL, 0);

            return TRUE;
      }
    }
    
    return FALSE; 
}
bool CImageTreeCtrl::DoDeleteItem(HTREEITEM hItem)
{
	    CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);




         m_level=get_item_level(hItem);
         m_name_old=GetItemText(hItem);
		 CppSQLite3DB SqliteDBBuilding;
		 CppSQLite3Table table;
		 CppSQLite3Query q;
		 SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

 
        try 
    {
        ////////////////////////////////////////////////////////////////////////////////////////////
        //获取数据库名称及路径
        /////////////////////////////////////////////////////////////////////////////////////////////////
        //连接数据库

        CString strSql;   BOOL is_exist=FALSE;	  CString str_temp;
        if(m_level >= 2)
        {
            HTREEITEM root=GetRootItem();
            CString subnetname=GetItemText(root);
            //Room
            HTREEITEM parentnode=GetParentItem(hItem);
            CString Roomname=GetItemText(parentnode);
            //Floor
            HTREEITEM floornode=GetParentItem(parentnode);
            CString Floorname=GetItemText(floornode);
             

			vector <tree_product>::iterator myiterator;


			for (myiterator=pFrame->m_product.begin();myiterator!=pFrame->m_product.end();)
			{
				if (hItem == myiterator->product_item)
				{
					CString strSql, temp_serial;
					unsigned int sn = myiterator->serial_number;
					temp_serial.Format(_T("%u"), sn); 
					strSql.Format(_T("delete   from ALL_NODE  where  Serial_ID='%s' "), temp_serial);
					SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
					DeleteItem(myiterator->product_item);
					myiterator = pFrame->m_product.erase(myiterator);

					break;
				}
				else
				{
					++myiterator;
				}
			}

#if 0
			for (int i = 0; i < pFrame->m_product.size(); i++) {
				if (hItem == pFrame->m_product.at(i).product_item)
				{
					CString strSql, temp_serial;
					unsigned int sn = pFrame->m_product.at(i).serial_number;
					temp_serial.Format(_T("%u"), sn); 
					strSql.Format(_T("delete   from ALL_NODE  where  Serial_ID='%s' "), temp_serial);
					SqliteDBBuilding.execDML((UTF8MBSTR)strSql);

					DeleteItem(pFrame->m_product.at(i).product_item);
				}
			}
#endif
           /*strSql.Format(_T("select * from ALL_NODE where  Product_name='%s' order by Building_Name"),m_name_old);
                
				q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);

                while(!q.eof())
                {
                    CString temp_serial;
                  
                    temp_serial = q.getValuebyName(L"Serial_ID");
                
                    strSql.Format(_T("delete   from ALL_NODE  where  Serial_ID='%s'"),temp_serial);
                    SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
                    q.nextRow();
                }*/
                
               
              
        }
        
        }
        catch(_com_error e)
        {
             
            SqliteDBBuilding.closedb();
            return FALSE;
            //m_pCon->Close();
        }
		SqliteDBBuilding.closedb();
       
		
		
		
		
//        ::PostMessage(pFrame->m_hWnd, WM_MYMSG_REFRESHBUILDING,0,0);
        return true;
}
// CImageTreeCtrl 消息处理程序
bool CImageTreeCtrl::HandleKeyDown(WPARAM wParam, LPARAM lParam) {
	bool bCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
	bool bShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
	  m_hSelItem = GetSelectedItem();
	m_level=get_item_level(m_hSelItem);
	m_name_old=GetItemText(m_hSelItem);
	if(GetEditControl() == 0) {
		method fnc = m_Keymap[int(wParam)][bCtrl][bShift];
		if(fnc)
			return (this->*fnc)(m_hSelItem);
	}

	// under some circumstances we need to check some more keys
	switch(int(wParam)) {
	case VK_ESCAPE:
		/*if(m_pDragData) {
			DragStop();
			return true;
		}*/
		/*FALLTHRU*/

	case VK_RETURN:
		if(GetEditControl() != 0) {
			GetEditControl()->SendMessage(WM_KEYDOWN, wParam, lParam);

			return true;
		}
		break;

	default:
		break;
	}

	return false;
}
BOOL CImageTreeCtrl::PreTranslateMessage(MSG* pMsg) {
	switch(pMsg->message) {
	case WM_KEYDOWN:
		if(HandleKeyDown(pMsg->wParam, pMsg->lParam))
			return true;
		break;

	default:
		break;
	}
	return CTreeCtrl::PreTranslateMessage(pMsg);
}
void CImageTreeCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	*pResult = 1;

	if(CanEditLabel(pTVDispInfo->item.hItem))
		*pResult = 0;
}
void CImageTreeCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	TVITEM & item = pTVDispInfo->item;
	*pResult = 1;
	if (b_building_management_flag == SYS_DB_BUILDING_MODE)
	{
		CString temp_cs;
		temp_cs = item.pszText;
		temp_cs.Trim();
		if (temp_cs.IsEmpty()) //在不变更的情况下， 会是 一个空的字符;
			return;
		//if(m_BMpoint->BuildingNode)
		//operation_nodeinfo
		if (operation_nodeinfo.node_type == TYPE_BM_POINT_LIST)
		{
			if (m_BMpoint->BuildingNode.h_treeitem != NULL)
				m_BMpoint->BuildingNode.m_csName = temp_cs;
		}
		else if (operation_nodeinfo.node_type == TYPE_BM_GROUP)
		{
			if (m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->h_treeitem != NULL)
				m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->m_csName = temp_cs;
		}
		else if (operation_nodeinfo.node_type == TYPE_BM_NODES)
		{
			if (m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->h_treeitem != NULL)
				m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->m_csName = temp_cs;
		}
		else if ((operation_nodeinfo.node_type == TYPE_BM_INPUT) ||
				 (operation_nodeinfo.node_type == TYPE_BM_OUTPUT) ||
				 (operation_nodeinfo.node_type == TYPE_BM_VARIABLE))
		{
			if (m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->pchild[operation_nodeinfo.child_io]->h_treeitem != NULL)
				m_BMpoint->BuildingNode.pchild[operation_nodeinfo.child_group]->pchild[operation_nodeinfo.child_device]->pchild[operation_nodeinfo.child_io]->m_csName = temp_cs;
		}
		else
		{
			return;
		}
		m_BMpoint->SaveAllIntoIniFile();
		return;
	}
	if(item.pszText && CanSetLabelText(item)) {
	    if (m_name_new.IsEmpty())
	    {
		item.pszText=m_name_old.GetBuffer();
		SetItem(&item);
		*pResult = 0;
		 return;
	    }
	    
         if (product_sort_way == SORT_BY_BUILDING_FLOOR )
        {  
            if(!UpdateDataToDB_Floor()){
                item.pszText=m_name_old.GetBuffer();
            }
        }
        else if (product_sort_way ==SORT_BY_CONNECTION)
        {
             if(!UpdateDataToDB_Connect()){
               item.pszText=m_name_old.GetBuffer();
             }
        } 
		else
		{
		item.pszText=m_name_old.GetBuffer();
		}
		SetItem(&item);
		*pResult = 0;
	}
}
bool CImageTreeCtrl::CanEditLabel(HTREEITEM hItem) {
	//TRACE1(_T("CEditTreeCtrl::CanEditLabel('%s')\n"), LPCTSTR(GetItemText(hItem)));
	return true;
}
bool CImageTreeCtrl::CanSetLabelText(TVITEM & item) {
	//TRACE1(_T("CEditTreeCtrl::CanSetLabelText('%s')\n"), item.pszText);
	m_name_new=item.pszText;
	return true;
}
BOOL CImageTreeCtrl::SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
{
	BOOL bReturn=CTreeCtrl::SetItemImage( hItem, nImage, nSelectedImage );
	return bReturn;
}
int CImageTreeCtrl::get_item_level(HTREEITEM hItem)
{//return value 0,
	int r_v=0;
	HTREEITEM hParentItem=hItem;
	do{
		hParentItem=GetParentItem(hParentItem);
		r_v++;
	}while(hParentItem!=NULL);
	r_v--;//^-^	
	return r_v;
}

void CImageTreeCtrl::turn_item_image(HTREEITEM hItem,bool state)
{
	int brother_nImage,brother_nSelectedImage;
	GetItemImage(hItem,brother_nImage,brother_nSelectedImage);

	switch(brother_nImage)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:	
		break;
	case 6:
	case 8:
	case 10:
	case 12:
	case 14:
	case 16:
	case 18:
	case 20:
    case 22:
    case 24:
    case 26:
    case 28:
    case 30:
    case 32:
	case 35:
	case 46:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
	break;
	case 7:
	case 9:
	case 11:
	case 13:
	case 15:
	case 17:
	case 19:
	case 21:
    case 23:
    case 25: //Add by Fandu . Fix the problem.When Tstat8 as a subnet device connect to the T3 controller ,It's online status is abnormal. 
    case 27:
    case 29:
    case 31:
    case 33:
	case 36:
	case 47:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;
	}
	SetItemImage(hItem,brother_nImage,brother_nSelectedImage);
}


int CImageTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;



	

	return 0;
}
bool CImageTreeCtrl::CanInsertItem(HTREEITEM hItem) {
	TRACE(_T("CEditTreeCtrl::CanInsertItem('%s')\n"), (hItem && hItem != TVI_ROOT) ? LPCTSTR(GetItemText(hItem)) : _T("<Root>"));
	return true;
}
void CImageTreeCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	DisplayContextMenu(point);
}

bool CImageTreeCtrl::CanDeleteItem(HTREEITEM hItem) {
	TRACE(_T("CEditTreeCtrl::CanDeleteItem('%s')\n"), LPCTSTR(GetItemText(hItem)));
	return true;
}
HTREEITEM CImageTreeCtrl::InsertSubnetItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nSubnetItemData++);
	return hti;
}

// add a floor node
HTREEITEM CImageTreeCtrl::InsertFloorItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nFloorItemData++);
	return hti;
}
// add a room node 
HTREEITEM CImageTreeCtrl::InsertRoomItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nRoomItemData++);
	return hti;
}
// add a device node
HTREEITEM CImageTreeCtrl::InsertDeviceItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nDeviceItemData++);
	return hti;
}


#pragma region color_function
//This function add by Fance ,used for when some click ,the device will change color
//and user will know which one is selected
//add time  2014 01 06
//添加设置字体、颜色、粗体的函数
	void CImageTreeCtrl::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		cf.color = (COLORREF)-1;
	cf.logfont = logfont;
	m_mapColorFont[hItem] = cf;
}

void CImageTreeCtrl::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
	SetItemState( hItem, bBold ? TVIS_BOLD: 0, TVIS_BOLD );
}

void CImageTreeCtrl::SetSelectSerialNumber(unsigned int nserial)
{
	m_serial_number = nserial;
}

unsigned int CImageTreeCtrl::GetSelectSerialNumber()
{
	return m_serial_number;
}

void CImageTreeCtrl::SetTreeOfflineMode(bool b_value)
{
	tree_offline_mode = b_value;
}


void CImageTreeCtrl::FlashSelectItem(HTREEITEM hItem)
{
	if(tree_offline_mode == false)
		return;
    if (hItem == NULL)
        return;
	static int flash_count = 1;
	flash_count = flash_count %3  + 1;
	if(flash_count == 1)
	{
		SetItemBold(hItem,1);
		SetItemColor( hItem, RGB(255,0,0));
	}
	else if(flash_count == 2)
	{
		SetItemBold(hItem,1);
		SetItemColor( hItem, RGB(0,255,0));
	}
	else if(flash_count == 3)
	{
		SetItemBold(hItem,1);
		SetItemColor( hItem, RGB(0,0,255));
	}
}

void CImageTreeCtrl::StopFlashItem()
{
	SetItemBold(old_hItem,0);
	SetItemColor( old_hItem, RGB(0,0,0));
}

void CImageTreeCtrl::SetSelectItem(HTREEITEM hItem)
{
	if(old_hItem != hItem)
	{
		if(old_hItem!=NULL)
		{
			SetItemBold(old_hItem,0);
			SetItemColor( old_hItem, RGB(0,0,0));
		}
		old_hItem = hItem;
	}

	SetItemBold(hItem,1);
	SetItemColor( hItem, RGB(255,0,0));
	m_hSelItem = hItem;
	offline_mode_string = GetItemText(m_hSelItem);
}

void CImageTreeCtrl::SetItemColor(HTREEITEM hItem, COLORREF color)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		cf.logfont.lfFaceName[0] = '/0';
	cf.color = color;
	m_mapColorFont[hItem] = cf;
	Invalidate();
}

BOOL CImageTreeCtrl::GetItemFont(HTREEITEM hItem, LOGFONT * plogfont)
{
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		return FALSE;
	if( cf.logfont.lfFaceName[0] == '/0' ) 
		return FALSE;
	*plogfont = cf.logfont;
	return TRUE;

}

BOOL CImageTreeCtrl::GetItemBold(HTREEITEM hItem)
{
	return GetItemState( hItem, TVIS_BOLD ) & TVIS_BOLD;
}

COLORREF CImageTreeCtrl::GetItemColor(HTREEITEM hItem)
{
	// Returns (COLORREF)-1 if color was not set
	Color_Font cf;
	if( !m_mapColorFont.Lookup( hItem, cf ) )
		return (COLORREF)-1;
	return cf.color;

}



// void CImageTreeCtrl::FillBkGroudColor(HTREEITEM& hSelItem)
// {
// 	CRect rc;
// 	GetItemRect(hSelItem, &rc,TRUE);
// 	CDC* pDC = GetDC();
// 
// 	CBrush brush;
// 	LOGBRUSH lb;
// 	lb.lbColor = GetSysColor(COLOR_HIGHLIGHT);
// 	lb.lbStyle = BS_SOLID;
// 	brush.CreateBrushIndirect(&lb);
// 
// 	pDC->SetBkMode(OPAQUE);
// 	pDC->FillRect(&rc, &brush);
// }


//添加WM_PAINT 信息监控，重载OnPaint()函数来实现绘制

void CImageTreeCtrl::OnPaint()
{
try
{
	//获取当前绘制对象的DC
	CPaintDC dc(this);

	// 使用将要绘制的对象的DC创建一个memory DC
	//memory device context的概念：是在内存中创建一个结构来反映一个显示（屏幕区域、窗口、
	//打印机等）的表面。可以用来先在内存中准备好要显示的图像，从而实现双缓存，提高刷新
	//速度减少刷新时产生的闪烁。
	CDC memDC;
	//从当前DC创建内存对象
	memDC.CreateCompatibleDC( &dc );

	//定义CRect对象，用来确定区域
	CRect rcClip, rcClient;
	//获取当前对象的边界区域
	dc.GetClipBox( &rcClip );
	//获取当前对象的用户区域
	GetClientRect(&rcClient);

	// Select a compatible bitmap into the memory DC
	//创建一个bmp文件，作为memDC的内容
	//该文件的大小与用于区域相同
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
	memDC.SelectObject( &bitmap );

	// Set clip region to be same as that in paint DC
	//通过对象的边界区域创建CRgn对象
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcClip );

	//rcClip.bottom = rcClip.bottom + 10;

	memDC.SelectClipRgn(&rgn);
	rgn.DeleteObject();

	// First let the control do its default drawing.
	//首先让控件自己进行默认的绘制，绘制到内存中
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	//获取树状控件的第一个节点
	HTREEITEM hItem = GetFirstVisibleItem();

	//遍历这棵树
	int n = GetVisibleCount()+1;
	while( hItem && n--)
	{
		CRect rect;

		// Do not meddle with selected items or drop highlighted items
		//不对选中的节点和实行拖放功能的节点进行操作
		//UINT selflag = TVIS_DROPHILITED;// | TVIS_SELECTED;
		UINT selflag;
		if(is_focus)
			selflag = TVIS_DROPHILITED | TVIS_SELECTED;
		else
			selflag = TVIS_DROPHILITED;

		//定义字体、颜色
		Color_Font cf;


		//设置字体
		if ( !(GetItemState( hItem, selflag ) & selflag )
			&& m_mapColorFont.Lookup( hItem, cf ))
		{
			CFont *pFontDC;
			CFont fontDC;
			LOGFONT logfont;

			if( cf.logfont.lfFaceName[0] != '/0' )
			{
				//用户定义了字体
				logfont = cf.logfont;
			}
			else
			{
				// 用户没有定义，使用系统字体
				CFont *pFont = GetFont();
				pFont->GetLogFont( &logfont );
			}

			//用户是否设定节点为加粗
			if( GetItemBold( hItem ) )
				logfont.lfWeight = 700;
			//创建字体
			fontDC.CreateFontIndirect( &logfont );
			pFontDC = memDC.SelectObject( &fontDC );

			//设置字体颜色
			if( cf.color != (COLORREF)-1 )
				memDC.SetTextColor( cf.color );

			//获取节点文字
			CString sItem = GetItemText( hItem );

			//获取节点区域
			GetItemRect( hItem, &rect, TRUE );
			//rect.bottom = rect.bottom + 2;
			//设置背景色为系统色
			memDC.FillSolidRect(&rect,GetSysColor( COLOR_WINDOW ));//clr);

			memDC.SetBkColor( GetSysColor( COLOR_WINDOW ) );
			//向内存中的图片写入内容,为该节点的内容
			memDC.TextOut( rect.left+2, rect.top+1, sItem );
			if(tree_offline_mode)
			{
				if(hItem == m_hSelItem)
					memDC.TextOut( rect.right + 10, rect.top+1, _T("->Offline Mode") );
				
#if 0
				CBitmap bmp;
				if (bmp.LoadBitmap(IDB_BITMAP_ALARM_RED))
				{
					BITMAP bmpInfo;
					bmp.GetBitmap(&bmpInfo);

					// Create an in-memory DC compatible with the
					// display DC we're using to paint
					CDC dcMemory;
					dcMemory.CreateCompatibleDC(&memDC);

					// Select the bitmap into the in-memory DC
					CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);

					// Find a centerpoint for the bitmap in the client area
					CRect rect_bit;
					GetClientRect(&rect_bit);
					int nX = rect_bit.left + (rect_bit.Width() - bmpInfo.bmWidth) / 2;
					int nY = rect_bit.top + (rect_bit.Height() - bmpInfo.bmHeight) / 2;

					// Copy the bits from the in-memory DC into the on-
					// screen DC to actually do the painting. Use the centerpoint
					// we computed for the target offset.
					memDC.BitBlt(rect.right, rect.top, bmpInfo.bmWidth, bmpInfo.bmHeight, &dcMemory, 
						0, 0, SRCCOPY);

					dcMemory.SelectObject(pOldBitmap);

				}
#endif
			}

			
			memDC.SelectObject( pFontDC );
		}
		hItem = GetNextVisibleItem( hItem );
	}
	

	//TRACE(_T("Fresh...ImageTreeCtrl\r\n"));
	dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC,
		rcClip.left, rcClip.top, SRCCOPY );
		memDC.DeleteDC();
		bitmap.DeleteObject();
}

catch (CException* e)
{
 TRACE(_T("Exception...ImageTreeCtrl\r\n"));
return;
}

	

}




void CImageTreeCtrl::OnKillFocus(CWnd* pNewWnd)
{
//	PostMessage(WM_PAINT,NULL,NULL);
	is_focus = false;
	//TRACE(_T("Lose focus.................\n"));
	CTreeCtrl::OnKillFocus(pNewWnd);

	
}

void CImageTreeCtrl::SetVirtualTreeItem(HTREEITEM virtual_item)
{
	m_virtual_tree_item = virtual_item;
}

void CImageTreeCtrl::BMContextMenu(CPoint& point, BM_nodeinfo nodeinfo)
{
	CPoint pt(point);
	ScreenToClient(&pt);
	UINT flags = 0;
	HTREEITEM hItem = HitTest(pt, &flags);

	CMenu menu;
	VERIFY(menu.CreatePopupMenu());
	if(b_building_management_flag == SYS_DB_BUILDING_MODE)
		VERIFY(menu.AppendMenu(MF_STRING, ID_BM_VIRTUAL_TREE, _T("Show ShowEquipment View")));

	VERIFY(menu.AppendMenu(MF_STRING, ID_BM_RENAME, _T("Rename")));

	CMenu Menu;
	Menu.CreatePopupMenu();


	VERIFY(menu.AppendMenu(MF_STRING, ID_BM_DELETE, _T("Delete")));

	operation_nodeinfo = nodeinfo; //保存 操作的节点;

	CMenu SubMenu;
	SubMenu.CreatePopupMenu();
	if (b_building_management_flag == SYS_DB_BUILDING_MODE)
	{
		if (nodeinfo.node_type == TYPE_BM_POINT_LIST)
		{
			menu.AppendMenu(MF_STRING, ID_BM_BUILDING_COMMUNICATE, _T("Communication"));
			SubMenu.AppendMenu(MF_STRING, ID_BM_ADD_GROUPS, _T("Add Groups"));
			SubMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_BM_ADD_NODES, _T("Add Nodes"));
			SubMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_BM_ADD_INPUT, _T("Add Inputs"));
			SubMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_BM_ADD_OUTPUT, _T("Add Outputs"));
			SubMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_BM_ADD_VARIABLE, _T("Add Variable"));
		}
		else if (nodeinfo.node_type == TYPE_BM_GROUP)
		{
			SubMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_BM_ADD_GROUPS, _T("Add Groups"));
			SubMenu.AppendMenu(MF_STRING, ID_BM_ADD_NODES, _T("Add Nodes"));
		    SubMenu.AppendMenu(MF_STRING, ID_BM_ADD_MODULE, _T("Add Module"));
			SubMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_BM_ADD_INPUT, _T("Add Inputs"));
			SubMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_BM_ADD_OUTPUT, _T("Add Outputs"));
			SubMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_BM_ADD_VARIABLE, _T("Add Variable"));
		}
		else if ((nodeinfo.node_type == TYPE_BM_NODES) ||
			(nodeinfo.node_type == TYPE_BM_INPUT) ||
			(nodeinfo.node_type == TYPE_BM_OUTPUT) ||
			(nodeinfo.node_type == TYPE_BM_VARIABLE))
		{
			SubMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_BM_ADD_GROUPS, _T("Add Groups"));
			SubMenu.AppendMenu(MF_STRING | MF_DISABLED, ID_BM_ADD_NODES, _T("Add Nodes"));
			SubMenu.AppendMenu(MF_STRING, ID_BM_ADD_INPUT, _T("Add Inputs"));
			SubMenu.AppendMenu(MF_STRING, ID_BM_ADD_OUTPUT, _T("Add Outputs"));
			SubMenu.AppendMenu(MF_STRING, ID_BM_ADD_VARIABLE, _T("Add Variable"));
			SubMenu.AppendMenu(MF_STRING, ID_BM_IO_PROPERTY, _T("Property Setting"));
			VERIFY(menu.AppendMenu(MF_STRING, ID_BM_IO_PROPERTY, _T("Map to others")));
		}
		else
		{
			VERIFY(menu.AppendMenu(MF_STRING, ID_ADD_VIRTUAL_DEVICE, _T("Add Virtual Device")));
		}
	}
	//else if (b_building_management_flag == SYS_VIRTUAL_MODE)
	//{
	//	SubMenu.AppendMenu(MF_STRING, ID_ADD_VIRTUAL_DEVICE, _T("Add Virtual Device"));
	//}
	

	menu.AppendMenu(MF_STRING | MF_POPUP, (UINT_PTR)SubMenu.m_hMenu, _T("Add"));


	if (menu.GetMenuItemCount() > 0)
		menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CImageTreeCtrl::DisplayContextOtherMenu(CPoint & point) {
		CPoint pt(point);
		ScreenToClient(&pt);
		UINT flags;
		HTREEITEM hItem = HitTest(pt, &flags);
		bool bOnItem = (flags & TVHT_ONITEM) != 0;


		CMenu menu;
		VERIFY(menu.CreatePopupMenu());
		
	    VERIFY(menu.AppendMenu(MF_STRING, ID_PROJECT_POINT_VIEW, _T("Project Point View")));
		VERIFY(menu.AppendMenu(MF_STRING, ID_SORT_BY_CONNECTION, _T("Sort By Connection")));
        VERIFY(menu.AppendMenu(MF_STRING, ID_SORT_BY_FLOOR, _T("Sort By Floor")));
        VERIFY(menu.AppendMenu(MF_STRING, ID_ADD_CUSTOM_DEVICE, _T("Add Modbus Device")));
        VERIFY(menu.AppendMenu(MF_STRING, ID_ADD_REMOTE_DEVICE, _T("Add Remote Device")));
        VERIFY(menu.AppendMenu(MF_STRING, ID_ADD_VIRTUAL_DEVICE, _T("Add Virtual Device")));
        
        if (menu.GetMenuItemCount() > 0)
            menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}



void CImageTreeCtrl::DisplayContextMenu(CPoint & point) {
	CPoint pt(point);
	ScreenToClient(&pt);
	UINT flags;
	HTREEITEM hItem = HitTest(pt, &flags);
	bool bOnItem = (flags & TVHT_ONITEM) != 0;
	GetOnlineDevice();
	CMenu menu;
	VERIFY(menu.CreatePopupMenu());
	if(bOnItem) 
	{
		if(CanEditLabel(hItem))
		{
			VERIFY(menu.AppendMenu(MF_STRING, ID_RENAME, _T("Rename\tF2")));
            VERIFY(menu.AppendMenu(MF_STRING, ID_DELETE, _T("Delete\tDel")));
			VERIFY(menu.AppendMenu(MF_STRING, ID_SORT_BY_CONNECTION, _T("Sort By Connection")));
			VERIFY(menu.AppendMenu(MF_STRING, ID_SORT_BY_FLOOR, _T("Sort By Floor")));
			VERIFY(menu.AppendMenu(MF_STRING, ID_PING_CMD, _T("Ping")));
            VERIFY(menu.AppendMenu(MF_STRING, ID_ADD_CUSTOM_DEVICE, _T("Add Modbus Device")));
			CMenu subMenu;
			subMenu.CreateMenu();
			
			for (int i = 0; i < m_online_serial.size(); i++)
			{
				subMenu.AppendMenu(MF_STRING, 100 + i, m_online_serial.at(i).online_name);
			}

			//VERIFY(menu.AppendMenu(MF_POPUP, (UINT)subMenu.m_hMenu, _T("Sync to Controller")));
		}
	}

	//ExtendContextMenu(menu);

	// maybe the menu is empty...
	if(menu.GetMenuItemCount() > 0)
		menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CImageTreeCtrl::CheckClickNode(HTREEITEM hItem ,BM_nodeinfo& nodeinfo) //给管理模式使用的功能
{

	if (hItem == m_BMpoint->BuildingNode.h_treeitem)
	{
		nodeinfo.child_device = -1;
		nodeinfo.child_group = -1;
		nodeinfo.child_io = -1;
		nodeinfo.node_type = TYPE_BM_POINT_LIST;
		return;
	}
	int temp_group_count = m_BMpoint->BuildingNode.m_child_count;
	for (int i = 0; i < temp_group_count; i++)
	{
		CBacnetBMD* temp_group_point = NULL;
		temp_group_point = m_BMpoint->BuildingNode.pchild[i];
		if (hItem == temp_group_point->h_treeitem)
		{
			nodeinfo.child_group = i;
			nodeinfo.child_device = -1;
			nodeinfo.child_io = -1;
			nodeinfo.node_type = temp_group_point->m_node_type;
			return;
		}

		for (int j = 0; j < temp_group_point->m_child_count; j++) //判断GROUP 有几个 子节点
		{
			CBacnetBMD* temp_device_point = NULL;
			temp_device_point = temp_group_point->pchild[j];   //这里得到类似
			if (hItem == temp_device_point->h_treeitem)
			{
				nodeinfo.child_group = i;
				nodeinfo.child_device = j;
				nodeinfo.child_io = -1;
				nodeinfo.node_type = temp_device_point->m_node_type ;
				return;
			}

			for (int z = 0; z < temp_device_point->m_child_count; z++)
			{
				CBacnetBMD* temp_io_point = NULL;
				temp_io_point = temp_device_point->pchild[z];   //IO节点
				if (hItem == temp_io_point->h_treeitem)
				{
					nodeinfo.child_group = i;
					nodeinfo.child_device = j;
					nodeinfo.child_io = z;
					nodeinfo.node_type = temp_io_point->m_node_type;
					return;
				}

			}
		}


		
	}

	nodeinfo.child_device = -1;
	nodeinfo.child_group = -1;
	nodeinfo.child_io = -1;
	nodeinfo.node_type = TYPE_BM_VIRTUAL_DEVICE;
	return;


}
//#include "CBacnetBMD.h"
void CImageTreeCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 1;
	UINT flags;
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	HTREEITEM hItem = HitTest(point, &flags);
	
	if (b_building_management_flag == SYS_NORMAL_MODE)
	{
		HTREEITEM root_item = CImageTreeCtrl::GetRootItem();
		if (CImageTreeCtrl::ItemHasChildren(root_item))
		{
			HTREEITEM hChildItem = CImageTreeCtrl::GetChildItem(root_item);
			while (hChildItem != NULL)
			{
				if (hChildItem == hItem)
				{
					hItem = NULL;
					break;
				}
				hChildItem = CImageTreeCtrl::GetNextItem(hChildItem, TVGN_NEXT);


			}
		}
		if (root_item == hItem)
			hItem = NULL;
		if (hItem != NULL)
		{
			if (hItem && (flags & TVHT_ONITEM) && !(flags & TVHT_ONITEMRIGHT))
				SelectItem(hItem);
			ClientToScreen(&point);
			DisplayContextMenu(point);
		}
		else
		{
			ClientToScreen(&point);
			DisplayContextOtherMenu(point);
		}
	}
	else
	{
		int a = m_BMpoint->nGroupCount;
		BM_nodeinfo temp_node;
		CheckClickNode(hItem, temp_node);
		ClientToScreen(&point);
		BMContextMenu(point, temp_node);
		Sleep(1);
	}


}
#pragma endregion

void CImageTreeCtrl::OnSetFocus(CWnd* pOldWnd)
{
	is_focus = true;
	//TRACE(_T("Get focus.................\n"));
	CTreeCtrl::OnSetFocus(pOldWnd);

	
}


void CImageTreeCtrl::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_nMsMoveTimerID)
	{
		KillTimer(m_nMsMoveTimerID);
		m_nMsMoveTimerID = 0;
		HTREEITEM hItem = NULL;
		UINT uFlag = 0;
		hItem = HitTest(m_ptMouseMove, &uFlag);
		if (hItem && m_bDraged)
		{
			SelectItem(hItem);
			Expand(m_hDragDist, TVE_EXPAND);
}
	}
	//滚动响应
	else if (nIDEvent == m_nScrollTimerID)
	{
		m_nTicks++;
		CPoint pt;
		GetCursorPos(&pt);
		CRect clientRt;
		GetClientRect(&clientRt);
		ClientToScreen(&clientRt);

		HTREEITEM hItem = GetFirstVisibleItem();

		//向上滚动
		if (pt.y < (clientRt.top + 10))
		{
			//响应延迟
			if (0 == (m_nTicks % 4))
			{
				CImageList::DragShowNolock(FALSE);
				SendMessage(WM_VSCROLL, SB_LINEUP);
				if (hItem)
				{
					SelectDropTarget(hItem);
					m_hDragDist = hItem;
				}
				CImageList::DragShowNolock(TRUE);
			}
		}
		else if (pt.y > (clientRt.bottom - 10))
		{
			//响应延迟
			if (0 == (m_nTicks % 4))
			{
				CImageList::DragShowNolock(FALSE);
				SendMessage(WM_VSCROLL, SB_LINEDOWN);
				UINT count = GetVisibleCount();
				for (size_t i = 0; i < count; i++)
				{
					hItem = GetNextVisibleItem(hItem);
				}
				if (hItem)
				{
					SelectDropTarget(hItem);
					m_hDragDist = hItem;
				}
				CImageList::DragShowNolock(TRUE);
			}
		}
	}
	else
		CTreeCtrl::OnTimer(nIDEvent);
#if 0
	if(tree_offline_mode == false)
	{
		KillTimer(1);
	}
	else
	FlashSelectItem(m_hSelItem);
	//CString temp_item_text ;
	//temp_item_text = offline_mode_string + _T("  Offline Mode");
	//SetItemText(m_hSelItem,temp_item_text);
	CTreeCtrl::OnTimer(nIDEvent);
#endif
}


void CImageTreeCtrl::PreSubclassWindow()
{
    // TODO: 在此添加专用代码和/或调用基类

    CTreeCtrl::PreSubclassWindow();

    EnableToolTips(TRUE);

}


INT_PTR CImageTreeCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
    // TODO: 在此添加专用代码和/或调用基类
    RECT rect;

    UINT nFlags;
    HTREEITEM hitem = HitTest(point, &nFlags);
    if (nFlags & TVHT_ONITEMICON)
    {
        CImageList *pImg = GetImageList(TVSIL_NORMAL);
        IMAGEINFO imageinfo;
        pImg->GetImageInfo(0, &imageinfo);

        GetItemRect(hitem, &rect, TRUE);
        rect.right = rect.left - 2;
        rect.left -= (imageinfo.rcImage.right + 2);

        pTI->hwnd = m_hWnd;
        pTI->uId = (UINT)hitem;
        pTI->lpszText = LPSTR_TEXTCALLBACK;
        pTI->rect = rect;
        return pTI->uId;
    }
    else if (nFlags & TVHT_ONITEMLABEL)
    {
        GetItemRect(hitem, &rect, TRUE);

        pTI->hwnd = m_hWnd;

        pTI->uId = (UINT_PTR)hitem;

        pTI->lpszText = LPSTR_TEXTCALLBACK;

        pTI->rect = rect;

        return pTI->uId;
    }
    else if (nFlags & TVHT_ONITEMSTATEICON)
    {
        CImageList *pImg = GetImageList(TVSIL_NORMAL);
        IMAGEINFO imageinfo;
        pImg->GetImageInfo(0, &imageinfo);

        GetItemRect(hitem, &rect, TRUE);
        rect.right = rect.left - (imageinfo.rcImage.right + 2);

        pImg = GetImageList(TVSIL_STATE);
        rect.left = rect.right - imageinfo.rcImage.right;

        pTI->hwnd = m_hWnd;
        pTI->uId = (UINT)hitem;
        pTI->lpszText = LPSTR_TEXTCALLBACK;
        pTI->rect = rect;

        // return value should be different from that used for item icon
        return pTI->uId * 2;
    }
    return -1;

    return CTreeCtrl::OnToolHitTest(point, pTI);
}
char tooltipchar[1024];

BOOL CImageTreeCtrl::OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
    // need to handle both ANSI and UNICODE versions of the message

	AFX_MODULE_THREAD_STATE* pThreadState = AfxGetModuleThreadState();
	CToolTipCtrl* pToolTip = pThreadState->m_pToolTip;
	pToolTip->SetMaxTipWidth(350);
	//pToolTip->SetDelayTime(5000);
    TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
    TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
    UINT nID = pNMHDR->idFrom;




    // Do not process the message from built in tooltip
    if (nID == (UINT)m_hWnd &&
        ((pNMHDR->code == TTN_NEEDTEXTA && pTTTA->uFlags & TTF_IDISHWND) ||
        (pNMHDR->code == TTN_NEEDTEXTW && pTTTW->uFlags & TTF_IDISHWND)))
        return FALSE;

    // Get the mouse position
    const MSG* pMessage;
    CPoint pt;
    pMessage = GetCurrentMessage();
    ASSERT(pMessage);
    pt = pMessage->pt;
    ScreenToClient(&pt);

    UINT nFlags;
    HTREEITEM hitem = HitTest(pt, &nFlags);

	bool find_match_device = false;
	tree_product tooltip_product;
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	for (UINT i = 0; i < pFrame->m_product.size(); i++)
	{
		if (hitem == pFrame->m_product.at(i).product_item)
		{
			//memcpy(&tooltip_product, &pFrame->m_product.at(i), sizeof(tree_product));
			tooltip_product = pFrame->m_product.at(i);
			find_match_device = true;
			break;
		}
	}

	if (!find_match_device)
	{
		return true;
	}

	CString temp_bacnet_info;
	CString temp_tcp_info;
	CString temp_rs485_info;
	CString temp_parent_info;
	CString temp_virtual_device;
	if (tooltip_product.object_instance != 0)
	{
		temp_bacnet_info.Format(_T("Bacnet Object Instance : %u \nBacnet MSTP Mac ID : %u \n\n"),tooltip_product.object_instance, tooltip_product.product_id );
	}
	if (tooltip_product.m_ext_info.virtual_device == 1)
	{
		temp_virtual_device = _T("Virtual device\n");
	}
	else if (!tooltip_product.BuildingInfo.strIp.Trim().IsEmpty())
	{
		temp_tcp_info.Format(_T("IP address : %s \nPort : %u \nModbus  ID : %d \nPC IP address : %s\n\n"),
			tooltip_product.BuildingInfo.strIp, tooltip_product.ncomport, tooltip_product.product_id, tooltip_product.NetworkCard_Address);
	}
	else
	{
		temp_rs485_info.Format(_T("RS485 Information \nComPort :Com%d\nBaudrate :%d\nModbus  ID : %d \n"), 
			tooltip_product.ncomport, tooltip_product.baudrate, tooltip_product.product_id);
	}

	if (tooltip_product.note_parent_serial_number != 0)
	{
		temp_parent_info.Format(_T("Parent node SerialNumber :%u\n"), tooltip_product.note_parent_serial_number);
	}

	strmyTipText.Format(_T("Panel Name : %s \nProduct Type : %s \nSeiralNumber :%u \nPanel Number : %u\n\n"),
tooltip_product.NameShowOnTree.GetBuffer(), GetProductName(tooltip_product.product_class_id).GetBuffer(), tooltip_product.serial_number, tooltip_product.panel_number);
	strmyTipText = strmyTipText + temp_bacnet_info + temp_tcp_info + temp_rs485_info + temp_parent_info + temp_virtual_device;
	int length111 = strTipText.GetLength();
    //strTipText.Format(_T("%s"), GetItemText((HTREEITEM)nID));  //get item text

    DWORD dw = (DWORD)GetItemData((HTREEITEM)nID); //get item data

    //CString* ItemData = (CString*)dw; //CAST item data
#if 0
    if (ItemData != NULL)

    {

        //CString s = ItemData; //pure virtual function

        strTipText = CString(_T(" ")) + *ItemData  + _T("\r\n Test"); //add node text to node data text

    }
#endif
#ifndef _UNICODE

    if (pNMHDR->code == TTN_NEEDTEXTA)

        lstrcpyn(pTTTA->szText, strTipText, 80);

    else

        _mbstowcsz(pTTTW->szText, strTipText, 80);

#else
	//for (int i = 0; i < 1024; i++)
	//{
	//	tooltipchar[i] = 0x30 + i % 10;
	//}
	//pTTTA->lpszText = tooltipchar;

	//WideCharToMultiByte(CP_ACP, 0, strTipText.GetBuffer(), -1, tooltipchar, 1024, 0, 0);
	//if (pNMHDR->code == TTN_NEEDTEXTA)
	//	pTTTA->lpszText = (LPSTR)(LPWSTR)(LPCWSTR)tooltipchar;
	//else
	//	pTTTW->lpszText = (LPWSTR)(LPCWSTR)strTipText;

	pTTTW->lpszText = (LPWSTR)(LPCWSTR)strmyTipText;

#if 0
    if (pNMHDR->code == TTN_NEEDTEXTA)

        _wcstombsz(pTTTA->szText, strTipText, 80);

    else

        lstrcpyn(pTTTW->szText, strTipText, 80);

   // CString TestAA;
  //  TestAA = _T("AA");
   // lstrcatW(pTTTW->szText, TestAA);
    //lstrcpyn(pTTTW->szText, TestAA, 80);
#endif
#endif

#if 0
    if (nFlags & TVHT_ONITEMICON)
    {
        int nImage, nSelImage;
        GetItemImage((HTREEITEM)nID, nImage, nSelImage);
        strTipText.Format(_T("Image : %d11111111111\r\n22222222222222\r\n33333333333"), nImage);
    }
    else
    {
        strTipText.Format(_T("State : %d"), GetItemState((HTREEITEM)nID,
            TVIS_STATEIMAGEMASK));
    }


#ifndef _UNICODE
    if (pNMHDR->code == TTN_NEEDTEXTA)
        lstrcpyn(pTTTA->szText, strTipText, 80);
    else
        _mbstowcsz(pTTTW->szText, strTipText, 80);
#else
    if (pNMHDR->code == TTN_NEEDTEXTA)
        _wcstombsz(pTTTA->szText, strTipText, 80);
    else
        lstrcpyn(pTTTW->szText, strTipText, 80);
#endif
#endif
    *pResult = 0;

    return TRUE; // message was handled
}


void CImageTreeCtrl::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//*pResult = 0;


	*pResult = CDRF_DODEFAULT;
	NMTVCUSTOMDRAW* plvoid = reinterpret_cast<NMTVCUSTOMDRAW*>(pNMHDR);
#if 1
	if (CDDS_PREPAINT == plvoid->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == plvoid->nmcd.dwDrawStage)
	{
		COLORREF crText, crBkgnd;
		if (b_building_management_flag == SYS_DB_BUILDING_MODE)
		{
			if (plvoid->iLevel == 1)  //判断节点所在的层次，根节点在第0层
			{
				if (plvoid->nmcd.lItemlParam == TREE_LP_VIRTUAL_DEVICE) //在添加节点时设置了节点的lParam属性，在这里就利用来判定具体是哪个节点
				{
					crText = RGB(200, 200, 200);
					crBkgnd = RGB(66, 66, 66);

					plvoid->clrText = crText;  //设置文字颜色
					plvoid->clrTextBk = crBkgnd;  //设置背景颜色
				}
				else
				{
					crText = RGB(0, 0, 0);
					crBkgnd = RGB(255, 255, 255);
				}
			}
		}
		else
		{
			if (plvoid->iLevel == 2)  //判断节点所在的层次，根节点在第0层
			{
				if (plvoid->nmcd.lItemlParam == TREE_LP_VIRTUAL_DEVICE) //在添加节点时设置了节点的lParam属性，在这里就利用来判定具体是哪个节点
				{
					crText = RGB(200, 200, 200);
					crBkgnd = RGB(66, 66, 66);

					plvoid->clrText = crText;  //设置文字颜色
					plvoid->clrTextBk = crBkgnd;  //设置背景颜色
				}
				else if (plvoid->nmcd.lItemlParam == 2000)
				{
					crText = RGB(0, 0, 0);
					crBkgnd = RGB(125, 125, 0);

					plvoid->clrText = crText;
					plvoid->clrTextBk = crBkgnd;
				}
				else
				{
					crText = RGB(0, 0, 0);
					crBkgnd = RGB(255, 255, 255);
				}
			}
			else
			{
				crText = RGB(0, 0, 0);
				crBkgnd = RGB(255, 255, 255);
			}
		}
		*pResult = CDRF_DODEFAULT;
	}
#endif 
}

#define   DRAG_DELAY   100
void CImageTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if ((GetTickCount() - m_dwDrawStart) < DRAG_DELAY)
		return;
	m_hDragSrc = pNMTreeView->itemNew.hItem;
	m_hDragDist = NULL;

	if (!m_pDragImage)
	{
		m_pDragImage = CreateDragImage(m_hDragSrc);
	}
	else
		return;

	if (!m_pDragImage)
		return;

	m_bDraged = true;
	m_pDragImage->BeginDrag(0, CPoint(8, 8));

	CPoint pt = pNMTreeView->ptDrag;
	ClientToScreen(&pt);
	m_pDragImage->DragEnter(this, pt);
	SetCapture();
	//设置滚动检测计时器
	m_nScrollTimerID = SetTimer(2, 50, NULL);
}

void CImageTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_dwDrawStart = GetTickCount();

	// 	CPoint pt;
	// 	GetCursorPos(&pt); 
	// 	ScreenToClient(&pt);
	// 	HTREEITEM hItem = HitTest(pt,&nFlags);
	// 	//点击图标区域（复选框）
	// 	if(hItem && (nFlags & TVHT_ONITEMSTATEICON))
	// 	{
	// 		SelectItem(hItem);
	// 		HTREEITEM hSelect = GetSelectedItem();
	// 		PostMessage(WM_RETRIEVE_CHECKSTATE,0,reinterpret_cast<LPARAM>(hItem)); 
	// 	}	

	CTreeCtrl::OnLButtonDown(nFlags, point);

}

void CImageTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	if (m_bDraged)
	{
		m_bDraged = false;
		//释放窗口锁定
		CImageList::DragLeave(this);
		//终止拖动
		CImageList::EndDrag();
		//释放鼠标捕捉，将鼠标操作交还给操作系统
		ReleaseCapture();
		if (m_pDragImage)
		{
			delete m_pDragImage;
			m_pDragImage = NULL;
		}

		SelectDropTarget(NULL);
		//未拖动
		if (m_hDragSrc == m_hDragDist)
		{
			KillTimer(m_nScrollTimerID);
			return;
		}
		//展开目标节点
		Expand(m_hDragDist, TVE_EXPAND);
		//节点拷贝，禁止父节点到子节点的拖动
		HTREEITEM hParent = m_hDragDist;
		while (hParent = GetParentItem(hParent))
		{
			if (hParent == m_hDragSrc)
			{
				AfxMessageBox(_T("禁止向子节点拷贝"));
				KillTimer(m_nScrollTimerID);
				return;
			}
		}
		DragBranch(m_hDragSrc, m_hDragDist);
		//HTREEITEM hNewItem = CopyBranch(m_hDragSrc, m_hDragDist, TVI_LAST);
		//删除被拖动节点
		//DeleteItem(m_hDragSrc);
		//SelectItem(hNewItem);
		KillTimer(m_nScrollTimerID);
	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CImageTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nMsMoveTimerID)
	{
		KillTimer(m_nMsMoveTimerID);
		m_nMsMoveTimerID = 0;
	}
	m_nMsMoveTimerID = SetTimer(1, 500, NULL);	//0.5秒展开节点组
	m_ptMouseMove = point;

	if (m_bDraged)
	{
		CImageList::DragMove(point);

		//设置鼠标经过节点的节点高亮显示
		CImageList::DragShowNolock(FALSE);	//擦除拖动痕迹
		HTREEITEM hItem;
		if (hItem = HitTest(point, &nFlags))
		{
			SelectDropTarget(hItem);
			m_hDragDist = hItem;
		}
		CImageList::DragShowNolock(TRUE);	//显示当前拖动图像列表
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}


//拖动一个节点至另一个节点
int CImageTreeCtrl::DragBranch(HTREEITEM horgitem, HTREEITEM hdesitem)
{
	//检查org 属于哪一个item
	//m_BMpoint->
	CBacnetBMD *pointlistnode = &m_BMpoint->BuildingNode;
	TRACE(_T("%s\r\n"), pointlistnode->m_csName);
	int index_group = -1;
	int index_device = -1;
	int index_io = -1;
	int index_type = TYPE_BM_POINT_LIST;
	int index_io_type = TREE_MAX_TYPE;
	for (int i = 0; i < pointlistnode->m_child_count; i++)
	{

		CBacnetBMD* groupnode = pointlistnode->pchild[i];
		TRACE(_T("%s\r\n"), groupnode->m_csName);
		if (horgitem == groupnode->h_treeitem)
		{
			index_group = i;
			index_device = -1;
			index_io = -1;
			index_type = TYPE_BM_GROUP;
			goto endtreesearch;
		}
		for (int j = 0; j < groupnode->m_child_count; j++)
		{		
			CBacnetBMD* devicenode = groupnode->pchild[j];
			TRACE(_T("%s\r\n"), devicenode->m_csName);
			if (horgitem == devicenode->h_treeitem)
			{
				index_group = i;
				index_device = j;
				index_io = -1;
				index_type = TYPE_BM_NODES;
				goto endtreesearch;
			}
			for (int k = 0; k < devicenode->m_child_count; k++)
			{
				CBacnetBMD* deviceio = devicenode->pchild[k];
				TRACE(_T("%s\r\n"), deviceio->m_csName);
				if (horgitem == deviceio->h_treeitem)
				{
					index_group = i;
					index_device = j;
					index_io = k;
					index_type = TYPE_BM_IO;
					index_io_type = deviceio->m_node_type;
					goto endtreesearch;
				}
			}
		}
	}
	return -1; //原始tree item 不是Point list中的 节点

endtreesearch:
	TRACE(_T("%d,%d,%d\r\n"), index_group, index_device, index_io);

	int find_des = 0;
	int index_des = 0;
	int index_des_io_type = TREE_MAX_TYPE;
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	for (UINT i = 0; i < pFrame->m_product.size(); i++)
	{
		if (hdesitem == pFrame->m_product.at(i).product_item)
		{
			index_des = i;
			index_des_io_type = TREE_MAX_TYPE;
			find_des = 1;
			goto enddestreesearch;
		}

		for (int j = 0; j < TREE_MAX_TYPE; j++)
		{
			if (hdesitem == pFrame->m_product.at(i).sub_io_info[j].h_tree_item)
			{
				if((pFrame->m_product.at(i).sub_io_info[j].already_use < pFrame->m_product.at(i).sub_io_info[j].capacity) &&
					pFrame->m_product.at(i).sub_io_info[j].capacity > 0)
					{
						index_des = i;
						index_des_io_type = pFrame->m_product.at(i).sub_io_info[j].h_item_type;
						find_des = 1;
						goto enddestreesearch;
					}
			}
		}

		Sleep(1);
	}


enddestreesearch:
	if (find_des)
	{
		//确认赋予给哪一个IO  并且需要变更 个数 以及 更新数据库的个数;


		TRACE(_T("%d\r\n"), index_des);
		CString temp_lpAppname;

		temp_lpAppname.Format(_T("Group%d"), index_group);
		CString section_io_status;
		section_io_status.Format(_T("IO_%d_%d_Status"), index_device, index_io);
		CString status_value;
		status_value.Format(_T("%d"), TREE_IO_ONLINE);
		WritePrivateProfileStringW(temp_lpAppname, section_io_status, status_value, cs_bm_ini);

		CString cs_property_section;
		CString cs_property_value;
		cs_property_section.Format(_T("IO_%d_%d_property"), index_device, index_io);
		cs_property_value.Format(_T(""));
		//存储格式 例如      序列号,122IN4
		CString des_points_value;
		CString temp_type;
		temp_type = cssub_io_type[index_des_io_type];//index_des_io_type
		//打开缓存数据库 ，确认放置在哪个IO  暂时没有完成 ，待定;

		cs_property_value.Format(_T("%u,%d%s"), pFrame->m_product.at(index_des).serial_number,pFrame->m_product.at(index_des).panel_number, temp_type);
		WritePrivateProfileStringW(temp_lpAppname, cs_property_section, cs_property_value, cs_bm_ini);


	}
	return 0;
}

HTREEITEM CImageTreeCtrl::CopyBranch(HTREEITEM hChildBruch, HTREEITEM hNewParent, HTREEITEM hInsertAfter)
{
#if 0
	HTREEITEM hChild = NULL;
	//先将拖动节点树的根节点拷贝至目标节点下
	HTREEITEM hInsert = CopyItem(hChildBruch, hNewParent, hInsertAfter);
	//递归调用，拷贝孩子节点
	hChild = GetChildItem(hChildBruch);
	while (hChild)
	{
		CopyBranch(hChild, hInsert, hInsertAfter);
		//获取兄弟节点
		hChild = GetNextSiblingItem(hChild);
	}
	return hInsert;
#endif
	return 0;
	
}
HTREEITEM CImageTreeCtrl::CopyItem(HTREEITEM hItem, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
	HTREEITEM hInsert;
#if 0
	TVINSERTSTRUCT tvItemST;
	CString strText;

	//获取插入节点源信息
	tvItemST.item.hItem = hItem;
	tvItemST.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	GetItem(&tvItemST.item);
	strText = GetItemText(hItem);
	tvItemST.item.cchTextMax = strText.GetLength() + 1;
	tvItemST.item.pszText = strText.LockBuffer();

	//插入节点
	tvItemST.hParent = hParent;
	tvItemST.hInsertAfter = hInsertAfter;
	tvItemST.item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_SELECTEDIMAGE;
	hInsert = InsertItem(&tvItemST);
	strText.ReleaseBuffer();

	//为节点绑定数据用以标识
	SetItemData(hInsert, GetItemData(hInsert));
	//设置节点状态
	SetItemState(hInsert, GetItemState(hInsert, TVIS_STATEIMAGEMASK), TVIS_STATEIMAGEMASK);
#endif
	return hInsert;
}


BOOL CImageTreeCtrl::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	return FALSE;
}

#if 1
void CImageTreeCtrl::OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	CString strName = pTVDispInfo->item.pszText;
	HTREEITEM hItem = GetSelectedItem();
	if (0 == strName.GetLength())
	{
		return;
	}
	if (hItem != NULL)
	{
		SetItemText(hItem, strName);
	}
}
#endif