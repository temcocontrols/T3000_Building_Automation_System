// BacnetProperty.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetProperty.h"
#include "afxdialogex.h"
#include "global_function.h"

// CBacnetProperty 对话框

IMPLEMENT_DYNAMIC(CBacnetProperty, CDialogEx)

CBacnetProperty::CBacnetProperty(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BACNET_PROPERTY, pParent)
{

}

CBacnetProperty::~CBacnetProperty()
{
}

void CBacnetProperty::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_BACNET_PROPERTY, m_bacnet_property_list);
}


BEGIN_MESSAGE_MAP(CBacnetProperty, CDialogEx)
END_MESSAGE_MAP()


// CBacnetProperty 消息处理程序


BOOL CBacnetProperty::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    Initial_List();
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CBacnetProperty::SetParameter(int nBACNET_PROPERTY_ID)
{
    n_property_id = nBACNET_PROPERTY_ID;
}

void CBacnetProperty::SetBacnetReadString(CString temp_cs)
{
    cs_bacnet_string = temp_cs;
}

void CBacnetProperty::SetTitle(CString temp_title)
{
    cs_window_title = temp_title;
}

void CBacnetProperty::Initial_List()
{
    m_bacnet_property_list.ShowWindow(SW_HIDE);
    m_bacnet_property_list.DeleteAllItems();
    while (m_bacnet_property_list.DeleteColumn(0));



    if (n_property_id == PROP_PRIORITY_ARRAY)
    {
        m_bacnet_property_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
        //m_bacnet_property_list.SetExtendedStyle(m_bacnet_property_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
        m_bacnet_property_list.SetExtendedStyle(m_bacnet_property_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
        m_bacnet_property_list.InsertColumn(0, _T("Priority"), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
        m_bacnet_property_list.InsertColumn(1, _T("Description"), 300, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_bacnet_property_list.InsertColumn(2, _T(" "), 50, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_bacnet_property_list.Setlistcolcharlimit(1, STR_PROGRAM_DESCRIPTION_LENGTH - 1);
        m_bacnet_property_list.Setlistcolcharlimit(6, STR_PROGRAM_LABEL_LENGTH - 1);

        m_bacnet_property_list.SetListHwnd(this->m_hWnd);
        CRect list_rect, win_rect;
        m_bacnet_property_list.GetWindowRect(list_rect);
        ScreenToClient(&list_rect);
        ::GetWindowRect(m_pragram_dlg_hwnd, win_rect);
        m_bacnet_property_list.Set_My_WindowRect(win_rect);
        m_bacnet_property_list.Set_My_ListRect(list_rect);

        m_bacnet_property_list.DeleteAllItems();

        CStringArray temp_array;

        cs_bacnet_string.Replace(_T("{"), _T(" "));
        cs_bacnet_string.Replace(_T("}"), _T(" "));
        SplitCStringA(temp_array, cs_bacnet_string, _T(","));

        CString temp_item;
        CString temp_pri[16];
        if ((temp_array.GetSize() == 16))
        {
            for (int j = 0; j < 16; j++)
            {
                temp_pri[j] = temp_array.GetAt(j);
            }
        }

        for (int i = 0;i < (int)17;i++)
        {
            temp_item.Format(_T("%d"), i+1);// Output_Priority_Description_Arry[i]
            m_bacnet_property_list.InsertItem(i, temp_item);
            m_bacnet_property_list.SetItemText(i, 1, Output_Priority_Description_Arry[i]);
            if(i != 16)
                m_bacnet_property_list.SetItemText(i, 2, temp_pri[i]);
            for (int x = 0;x<3;x++)
            {
                if ((i % 2) == 0)
                    m_bacnet_property_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR);
                else
                    m_bacnet_property_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
            }
        }
        SetWindowText(cs_window_title);
    }


    m_bacnet_property_list.ShowWindow(SW_SHOW);
    
}

