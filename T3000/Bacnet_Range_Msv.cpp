// Bacnet_Range_Msv.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "Bacnet_Range_Msv.h"
#include "afxdialogex.h"
#include "global_function.h"

// CBacnet_Range_Msv 对话框

IMPLEMENT_DYNAMIC(CBacnet_Range_Msv, CDialogEx)

CBacnet_Range_Msv::CBacnet_Range_Msv(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BACNET_RANGE_MSV, pParent)
{

}

CBacnet_Range_Msv::~CBacnet_Range_Msv()
{
}

void CBacnet_Range_Msv::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_RANGE_MSV, m_range_msv_list);
}


BEGIN_MESSAGE_MAP(CBacnet_Range_Msv, CDialogEx)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED, Fresh_Msv_Item)
    ON_MESSAGE(WM_REFRESH_BAC_MSV_LIST, Fresh_Msv_List)
END_MESSAGE_MAP()


// CBacnet_Range_Msv 消息处理程序


BOOL CBacnet_Range_Msv::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    Initial_List();
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


BOOL CBacnet_Range_Msv::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类

    return CDialogEx::PreTranslateMessage(pMsg);
}

void CBacnet_Range_Msv::Initial_List()
{
    m_range_msv_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
    m_range_msv_list.SetExtendedStyle(m_range_msv_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_range_msv_list.InsertColumn(CUSTOMER_MSV_RANGE_NUM, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    m_range_msv_list.InsertColumn(CUSTOMER_MSV_RANGE_DESCRIPTION, _T("Description"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_range_msv_list.InsertColumn(CUSTOMER_MSV_RANGE_VALUE, _T("Value"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_range_msv_list.SetListHwnd(this->m_hWnd);
    m_msv_dlg_hwnd = this->m_hWnd;
    for (int i = 0;i < 8;i++)
    {
        CString temp_item;


        temp_item.Format(_T("%d"), i + 1);
        m_range_msv_list.InsertItem(i, temp_item);
        
        if (m_msv_data.at(msv_range_tbl_line).msv_data[i].status == 1)
        {
            CString temp_des;
            MultiByteToWideChar(CP_ACP, 0, (char *)m_msv_data.at(msv_range_tbl_line).msv_data[i].msv_name, (int)strlen((char *)m_msv_data.at(msv_range_tbl_line).msv_data[i].msv_name) + 1,
                temp_des.GetBuffer(MAX_PATH), MAX_PATH);
            temp_des.ReleaseBuffer();
            m_range_msv_list.SetItemText(i, CUSTOMER_MSV_RANGE_DESCRIPTION, temp_des);

            CString temp_value;
            temp_value.Format(_T("%u"), m_msv_data.at(msv_range_tbl_line).msv_data[i].msv_value);
            m_range_msv_list.SetItemText(i, CUSTOMER_MSV_RANGE_VALUE, temp_value);
        }
        else
        {
            m_range_msv_list.SetItemText(i, CUSTOMER_MSV_RANGE_DESCRIPTION, _T(""));
            m_range_msv_list.SetItemText(i, CUSTOMER_MSV_RANGE_VALUE, _T(""));
            continue;
        }

    }

    Fresh_Msv_List(NULL,NULL);
}


LRESULT CBacnet_Range_Msv::Fresh_Msv_List(WPARAM wParam, LPARAM lParam)
{
    int Fresh_Item;
    int isFreshOne = (int)lParam;

    for (int i = 0;i<(int)m_msv_data.size();i++)
    {
        CString temp_value;
        CString temp_des;


        if (m_msv_data.at(msv_range_tbl_line).msv_data[i].status == 1)
        {
            MultiByteToWideChar(CP_ACP, 0, (char *)m_msv_data.at(msv_range_tbl_line).msv_data[i].msv_name, (int)strlen((char *)m_msv_data.at(msv_range_tbl_line).msv_data[i].msv_name) + 1,
                temp_des.GetBuffer(MAX_PATH), MAX_PATH);
            temp_des.ReleaseBuffer();

            m_range_msv_list.SetItemText(i, CUSTOMER_MSV_RANGE_DESCRIPTION, temp_des);

            temp_value.Format(_T("%u"), m_msv_data.at(msv_range_tbl_line).msv_data[i].msv_value);
            m_range_msv_list.SetItemText(i, CUSTOMER_MSV_RANGE_VALUE, temp_value);
        }
        else
        {
            break;
        }




    }

    return 0;
}


LRESULT CBacnet_Range_Msv::Fresh_Msv_Item(WPARAM wParam, LPARAM lParam)
{
    int cmp_ret;//compare if match it will 0;
    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam;


    CString temp_task_info;
    CString New_CString = m_range_msv_list.GetItemText(Changed_Item, Changed_SubItem);


    if (Changed_SubItem == CUSTOMER_MSV_RANGE_DESCRIPTION)
    {
        CString cs_temp = m_range_msv_list.GetItemText(Changed_Item, Changed_SubItem);
        if (cs_temp.GetLength() >= STR_MSV_NAME_LENGTH)	//长度不能大于结构体定义的长度;
        {
            MessageBox(_T("Length can not higher than 20"), _T("Warning"), MB_OK | MB_ICONINFORMATION);
            PostMessage(WM_REFRESH_BAC_PROGRAM_LIST, NULL, NULL);
            return 0;
        }
        cs_temp.MakeUpper();


        char cTemp1[255];
        memset(cTemp1, 0, 255);
        WideCharToMultiByte(CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
        memcpy_s(m_msv_data.at(msv_range_tbl_line).msv_data[Changed_Item].msv_name, STR_MSV_NAME_LENGTH, cTemp1, STR_MSV_NAME_LENGTH);
        if (cs_temp.IsEmpty())
        {
            m_msv_data.at(msv_range_tbl_line).msv_data[Changed_Item].status = 0;
        }
        else
        {
            m_msv_data.at(msv_range_tbl_line).msv_data[Changed_Item].status = 1;
        }
    }

    if (Changed_SubItem == CUSTOMER_MSV_RANGE_VALUE)
    {
        CString temp_cs = m_range_msv_list.GetItemText(Changed_Item, Changed_SubItem);
        if (temp_cs.IsEmpty())
        {
            m_msv_data.at(msv_range_tbl_line).msv_data[Changed_Item].status = 0;
        }
        else
        {
            m_msv_data.at(msv_range_tbl_line).msv_data[Changed_Item].status = 1;
        }
        int temp_int = _wtoi(temp_cs) ;
        m_msv_data.at(msv_range_tbl_line).msv_data[Changed_Item].msv_value = temp_int;
    }
    int write_ret = 0;
    write_ret = Write_Private_Data_Blocking(WRITE_MSV_COMMAND, msv_range_tbl_line, msv_range_tbl_line);

    Get_Msv_Table_Name(msv_range_tbl_line);
    //cmp_ret = memcmp(&m_temp_program_data[Changed_Item], &m_Program_data.at(Changed_Item), sizeof(Str_program_point));
    //if (cmp_ret != 0)
    //{
    //    m_range_msv_list.SetItemBkColor(Changed_Item, Changed_SubItem, LIST_ITEM_CHANGED_BKCOLOR);
    //    temp_task_info.Format(_T("Write Program List Item%d .Changed to \"%s\" "), Changed_Item + 1, New_CString);
    //    Post_Write_Message(g_bac_instance, WRITEPROGRAM_T3000, Changed_Item, Changed_Item, sizeof(Str_program_point), m_pragram_dlg_hwnd, temp_task_info, Changed_Item, Changed_SubItem);
    //}

    return 0;
}

