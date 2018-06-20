// TstatIconSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "TstatIconSetting.h"
#include "afxdialogex.h"
#include "global_function.h"

// CTstatIconSetting 对话框

IMPLEMENT_DYNAMIC(CTstatIconSetting, CDialogEx)

CTstatIconSetting::CTstatIconSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TSTAT_ICON_SETTING, pParent)
{

}

CTstatIconSetting::~CTstatIconSetting()
{
}

void CTstatIconSetting::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_tstat_icon_list);
}


BEGIN_MESSAGE_MAP(CTstatIconSetting, CDialogEx)
    ON_NOTIFY(NM_CLICK, IDC_LIST1, &CTstatIconSetting::OnNMClickList1)
    ON_BN_CLICKED(IDC_BUTTON_ICON_OK, &CTstatIconSetting::OnBnClickedButtonIconOk)
END_MESSAGE_MAP()


// CTstatIconSetting 消息处理程序


BOOL CTstatIconSetting::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    Initial_List();
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


BOOL CTstatIconSetting::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类

    return CDialogEx::PreTranslateMessage(pMsg);
}

void CTstatIconSetting::Initial_List()
{
    m_tstat_icon_list.ShowWindow(SW_HIDE);
    m_tstat_icon_list.DeleteAllItems();
    while (m_tstat_icon_list.DeleteColumn(0));

    m_tstat_icon_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
    //m_tstat_icon_list.SetExtendedStyle(m_tstat_icon_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
    m_tstat_icon_list.SetExtendedStyle(m_tstat_icon_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_tstat_icon_list.InsertColumn(TSTAT_ICON_NAME, _T("   "), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    m_tstat_icon_list.InsertColumn(TSTAT_ICON_NA, _T("N/A"), 40, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_tstat_icon_list.InsertColumn(TSTAT_ICON_K1, _T("K1"), 40, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_tstat_icon_list.InsertColumn(TSTAT_ICON_K2, _T("K2"), 40, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_tstat_icon_list.InsertColumn(TSTAT_ICON_K3, _T("K3"), 40, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_tstat_icon_list.InsertColumn(TSTAT_ICON_K4, _T("K4"), 40, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_tstat_icon_list.InsertColumn(TSTAT_ICON_K5, _T("K5"), 40, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_tstat_icon_list.InsertColumn(TSTAT_ICON_AO1, _T("AO1"), 40, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_tstat_icon_list.InsertColumn(TSTAT_ICON_AO2, _T("AO2"), 40, ListCtrlEx::CheckBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

    m_pragram_dlg_hwnd = this->m_hWnd;
    //g_hwnd_now = m_pragram_dlg_hwnd;
    m_tstat_icon_list.SetListHwnd(this->m_hWnd);
    m_tstat_icon_list.SetWhetherShowBkCol(false);
    m_tstat_icon_list.DeleteAllItems();
    

    m_tstat_icon_list.InsertItem(0, _T("Day Icon"));
    m_tstat_icon_list.InsertItem(1, _T("Night"));
    m_tstat_icon_list.InsertItem(2, _T("Occupied"));
    m_tstat_icon_list.InsertItem(3, _T("Unoccupied"));
    m_tstat_icon_list.InsertItem(4, _T("Heat"));
    m_tstat_icon_list.InsertItem(5, _T("Cool"));
    m_tstat_icon_list.InsertItem(6, _T("FAN1"));
    m_tstat_icon_list.InsertItem(7, _T("FAN2"));
    m_tstat_icon_list.InsertItem(8, _T("FAN3"));

    for (int i = 0; i < 9; i++)
    {
        if ((product_register_value[783 + i] >= 0) && (product_register_value[783 + i] <= 7))
        {
            for (int j = 1; j < MAX_COL_COUNT; j++)
            {
                if (product_register_value[783 + i] + 1 == j)
                {
                    m_tstat_icon_list.SetCellChecked(i, j, 1);
                }
                else
                {
                    m_tstat_icon_list.SetCellChecked(i, j, 0);
                }
            }
            
        }
    }
    //m_tstat_icon_list.SetCellChecked(0, 0, 1);
    m_tstat_icon_list.ShowWindow(SW_SHOW);
}


void CTstatIconSetting::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    DWORD dwPos = GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point(LOWORD(dwPos), HIWORD(dwPos));
    m_tstat_icon_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt = point;
    lvinfo.flags = LVHT_ABOVE;
    int nItem = m_tstat_icon_list.SubItemHitTest(&lvinfo);
    long lRow, lCol;
    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;

    if (nItem != -1)
    {
        //m_tstat_icon_list.SetCellChecked(lRow, lCol, 1);
        for (int i = 1;i<MAX_COL_COUNT;i++)
        {
            if (i == lCol)
                continue;
            m_tstat_icon_list.SetCellChecked(lRow, i, FALSE);
        }
    }


    *pResult = 0;
}


void CTstatIconSetting::OnBnClickedButtonIconOk()
{
    // TODO: 在此添加控件通知处理程序代码
    unsigned short temp_product_register_value[10];
    memset(temp_product_register_value, 0, 20);
    for (int i = 0; i < 9; i++)
    {
        bool found_check = false;
        for (int j = 1;j < MAX_COL_COUNT;j++)
        {
            bool b_checked = false;
            b_checked = m_tstat_icon_list.GetCellChecked(i, j);
            if (b_checked)
            {
                found_check = true;
                temp_product_register_value[i] = j - 1;
                break;
            }
        }

        if (found_check == false)
        {
            temp_product_register_value[i] = 0;
        }

        if (temp_product_register_value[i] != product_register_value[783 + i])
        {
            int ret = write_one(g_tstat_id, 783 + i, temp_product_register_value[i]);
            if (ret < 0)
            {
                MessageBox(_T("Write data timeout"));
                return;
            }
            product_register_value[783 + i] = temp_product_register_value[i];
        }
    }
    PostMessage(WM_CLOSE, NULL, NULL);


}
