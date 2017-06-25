// BoatMonitorViewer.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BoatMonitorViewer.h"


// CBoatMonitorViewer

IMPLEMENT_DYNCREATE(CBoatMonitorViewer, CFormView)

CBoatMonitorViewer::CBoatMonitorViewer()
	: CFormView(CBoatMonitorViewer::IDD)
{

}

CBoatMonitorViewer::~CBoatMonitorViewer()
{
}

void CBoatMonitorViewer::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ZIGBEETYPE, m_combox_zigbee);
    DDX_Control(pDX, IDC_COMBO_CHANNEL, m_combox_channel);
    DDX_Control(pDX, IDC_BRANDRATE, m_combox_brandrate);
    DDX_Control(pDX, IDC_LIST1, m_zigbee_list);
}

BEGIN_MESSAGE_MAP(CBoatMonitorViewer, CFormView)
END_MESSAGE_MAP()


// CBoatMonitorViewer diagnostics

#ifdef _DEBUG
void CBoatMonitorViewer::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBoatMonitorViewer::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


void CBoatMonitorViewer::Fresh()
{

    
    CString   strTemp;
    strTemp.Format(_T("%d"),product_register_value[6]);
    GetDlgItem(IDC_ID_EDIT)->SetWindowTextW(strTemp);
     strTemp = _T("BoatMonitor");
    GetDlgItem(IDC_MODEL_EDIT)->SetWindowTextW(strTemp);
    
    if (product_register_value[21] == 0)
    {
        m_combox_brandrate.SetCurSel(0);
    } 
    else
    {
        m_combox_brandrate.SetCurSel(1);
    }
     
    strTemp.Format(_T("%d"),product_register_value[22]);	
    GetDlgItem(IDC_EDIT_ZIGBEE_ID)->SetWindowText(strTemp);
    m_combox_zigbee.ResetContent();
    m_combox_zigbee.AddString(_T("Coordinator"));
    m_combox_zigbee.AddString(_T("Router"));
    if (product_register_value[23]<2)
    {
        m_combox_zigbee.SetCurSel(product_register_value[23]);
    }
    m_combox_channel.ResetContent();
    for (int channel=11;channel<27;channel++)
    {
        strTemp.Format(_T("%d"),channel);
        m_combox_channel.AddString(strTemp);
    }

    int ChannelValue=product_register_value[25]+product_register_value[24]*65535;
    int ChannelNO=0;
    if (ChannelValue==0x04000000)
    {
        ChannelNO=26;
    }
    else if (ChannelValue==0x02000000)
    {
        ChannelNO=25;
    }
    else if (ChannelValue==0x01000000)
    {
        ChannelNO=24;
    }
    else if (ChannelValue==0x00800000)
    {
        ChannelNO=23;
    }
    else if (ChannelValue==0x00400000)
    {
        ChannelNO=22;
    }
    else if (ChannelValue==0x00200000)
    {
        ChannelNO=21;
    }
    else if (ChannelValue==0x00100000)
    {
        ChannelNO=20;
    }
    else if (ChannelValue==0x00080000)
    {
        ChannelNO=19;
    }
    else if (ChannelValue==0x00040000)
    {
        ChannelNO=18;
    }
    else if (ChannelValue==0x00020000)
    {
        ChannelNO=17;
    }
    else if (ChannelValue==0x00010000)
    {
        ChannelNO=16;
    }
    else if (ChannelValue==0x00008000)
    {
        ChannelNO=15;
    }
    else if (ChannelValue==0x00004000)
    {
        ChannelNO=14;
    }
    else if (ChannelValue==0x00002000)
    {
        ChannelNO=13;
    }
    else if (ChannelValue==0x00001000)
    {
        ChannelNO=12;
    }
    else if (ChannelValue==0x07FFF8000)
    {
        ChannelNO=11;
    }
    else
    {
        ChannelNO=0;
    }
    if (ChannelNO>0)
    {
        ChannelNO-=11;
    }
    m_combox_channel.SetCurSel(ChannelNO);

    strTemp.Format(_T("%d"),product_register_value[26]);
    GetDlgItem(IDC_EDIT_ZIGBEE_SV)->SetWindowText(strTemp);
    strTemp=_T("");
    for (int Address=36;Address<52;Address++)
    {
        CString StrValue;
        StrValue.Format(_T("%02X"),product_register_value[Address]);
        strTemp+=StrValue;
    }

    GetDlgItem(IDC_EDIT_ZIGBEE_SK)->SetWindowText(strTemp);
    strTemp.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X"),product_register_value[27],product_register_value[28],product_register_value[29],product_register_value[30],
    product_register_value[31],product_register_value[32],product_register_value[33],product_register_value[34]);
    GetDlgItem(IDC_EDIT_ZIGBEE_MAC)->SetWindowText(strTemp);

    strTemp.Format(_T("%d"),product_register_value[52]);
    GetDlgItem(IDC_EDIT_NUMBER)->SetWindowText(strTemp);
     Initial_List();

}

// CBoatMonitorViewer message handlers


void CBoatMonitorViewer::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
                                                                                                                  
    
}
void CBoatMonitorViewer::Initial_List(){
int Number = product_register_value[52];
    if (Number<=0)
    {
        return;
    }
    
    m_zigbee_list.DeleteAllItems();
    while ( m_zigbee_list.DeleteColumn (0)) ;
        m_zigbee_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_zigbee_list.SetExtendedStyle(m_zigbee_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
        m_zigbee_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_zigbee_list.InsertColumn(1, _T("Modbus IDs"), 140, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_zigbee_list.InsertColumn(2, _T("Voltage"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_zigbee_list.InsertColumn(3, _T("Switch Status"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_zigbee_list.InsertColumn(4, _T("Temperature 1"), 120, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_zigbee_list.InsertColumn(5, _T("Temperature 2"), 120, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_zigbee_list.InsertColumn(6, _T("Signal Strength"), 120, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
         g_hwnd_now = this->m_hWnd;
        if (Number<=0)
        {
            return;
        }
        CString strTemp;
        //m_zigbee_list.DeleteAllItems();
        for (int i=0;i<Number;i++)
        {
            strTemp.Format(_T("%d"),i+1);
            m_zigbee_list.InsertItem(i,strTemp);
            for (int x=0;x<7;x++)
            {
                if((i%2)==0)
                    m_zigbee_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
                else
                    m_zigbee_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
            }
            strTemp.Format(_T("%d"),product_register_value[53+i]);
            m_zigbee_list.SetItemText(i,1,strTemp);
            strTemp.Format(_T("%d"),product_register_value[53+Number*1+i]);
            m_zigbee_list.SetItemText(i,2,strTemp);
            strTemp.Format(_T("%d"),product_register_value[53+Number*2+i]);
            m_zigbee_list.SetItemText(i,3,strTemp);
            strTemp.Format(_T("%d"),product_register_value[53+Number*3+i]);
            m_zigbee_list.SetItemText(i,4,strTemp);
            strTemp.Format(_T("%d"),product_register_value[53+Number*4+i]);
            m_zigbee_list.SetItemText(i,5,strTemp);
            strTemp.Format(_T("%d"),product_register_value[53+Number*5+i]);
            m_zigbee_list.SetItemText(i,6,strTemp);
           
        }

       

     
        

    CRect list_rect,win_rect;
    m_zigbee_list.GetWindowRect(list_rect);
    ScreenToClient(&list_rect);
    ::GetWindowRect(g_hwnd_now,win_rect);
    m_zigbee_list.Set_My_WindowRect(win_rect);
    m_zigbee_list.Set_My_ListRect(list_rect);
    
}