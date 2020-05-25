// T3ModulesView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3ModulesView.h"
#include "global_function.h"
#include "T3RTDView.h"
#include "T3RangeDlg.h"

extern CString Range_Units[15];

// CT3ModulesView
DWORD WINAPI _ReadMultiRegisters_T3_Input(LPVOID pParam)
{
    CT3ModulesView* pFrame=(CT3ModulesView*)(pParam);
    CString g_strT3000LogString;
    int heatbeat  = 0;
    while(pFrame->IsWindowVisible ())
    {
        if (!is_connect())
        {
            Sleep(1000);
            continue;
        }
        if (pFrame->m_ThreadStop)
        {
            heatbeat ++;
            Sleep(6000);
            if (heatbeat >10)
            {
                pFrame->m_ThreadStop = FALSE;
                heatbeat =0;
            }
            continue;

        }
        for(int i=0; i<4; i++)
        {
            int multy_ret = 0;
            if (pFrame->m_ThreadStop)
            {
                break;
            }
            multy_ret = Read_Multi(g_tstat_id,&product_register_value[i*100],i*100,100);

            Sleep(100);
            if(multy_ret<0)
                break;
        }
        PostMessage(g_hwnd_now,WM_REFRESH_BAC_INPUT_LIST,0,0);
    }
    pFrame->hFirstThread = NULL;
    return 0;

}
IMPLEMENT_DYNCREATE(CT3ModulesView, CFormView)

CT3ModulesView::CT3ModulesView()
    : CFormView(CT3ModulesView::IDD)
{
    hFirstThread = NULL;
    m_ThreadStop = FALSE;
}

CT3ModulesView::~CT3ModulesView()
{
    if(hFirstThread != NULL)
        TerminateThread(hFirstThread, 0);
}

void CT3ModulesView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_T3INPUTS, m_T3_Input_List);
}

BEGIN_MESSAGE_MAP(CT3ModulesView, CFormView)
    ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Input_List)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Change_Input_Item)
    ON_NOTIFY(NM_CLICK, IDC_LIST_T3INPUTS, &CT3ModulesView::OnNMClickList_Input)
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CT3ModulesView diagnostics

#ifdef _DEBUG
void CT3ModulesView::AssertValid() const
{
    CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CT3ModulesView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CT3ModulesView message handlers


void CT3ModulesView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

}
CString CT3ModulesView::GetValue(int regValue,int RangeValue)
{
    CString strresult;
    if (0==RangeValue)
    {
        strresult.Format(_T("%d"),regValue);

    }
    else if (1==RangeValue)
    {
        strresult.Format(_T("%.1f C"),(float)regValue/10.0);
    }
    else if (2==RangeValue)
    {
        strresult=_T("10K F");
        strresult.Format(_T("%.1f F"),(float)regValue/10.0);
    }
    else if (3==RangeValue)
    {

        strresult.Format(_T("%.1f"),(float)regValue);
        strresult+=_T("%");
    }
    else if (4==RangeValue||5==RangeValue)
    {
        //strresult.Format(_T("%.1f F"),(float)regValue/10.0);
        if (regValue==0)
        {
            strresult=_T("OFF");
        }
        else
        {
            strresult=_T("ON");
        }
    }
//     else if (5==RangeValue)
//     {
//         if (regValue==0)
//         {
//             strresult=_T("ON");
//         }
//         else
//         {
//             strresult=_T("OFF");
//         }
//     }
    else if (6==RangeValue)
    {
        strresult.Format(_T("%d"),regValue);
    }
    else if (7==RangeValue)
    {
        strresult.Format(_T("%d"),regValue);
    }
    else if (8==RangeValue)
    {
        //strresult=_T("TYPE3 10K C");
        strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
    }
    else if (9==RangeValue)
    {
        /*strresult=_T("TYPE3 10K F");*/
        strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
    }
    else if (10==RangeValue)
    {
        strresult=_T("0");
        //strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
    }
    else if (11==RangeValue)
    {
        //strresult=_T("0-5V");
        strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
    }
    else if (12==RangeValue)
    {
        //strresult=_T("0-10V");
        strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
    }
    else if (13==RangeValue)
    {
        //strresult=_T("0-20I");
        strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
    }
    return strresult;
}
void CT3ModulesView::Fresh()
{

    g_hwnd_now = this->m_hWnd;
    m_sn=product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;
    CString strTemp;
    m_T3_Input_List.ShowWindow(SW_HIDE);
    m_T3_Input_List.DeleteAllItems();
    while ( m_T3_Input_List.DeleteColumn (0)) ;
    if (product_register_value[7]==PM_T34AO)
    {
        m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_T3_Input_List.InsertColumn(0, _T("Label"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        //m_T3_Input_List.InsertColumn(1, _T("Full Label"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(1, _T("Value"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(2, _T("Date Stamp"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(3, _T("Range"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(4, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

        for (int i=0; i<10; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }


        //==================================================Input============================================================
        //T3-4AO
        //address: input 119~138  INPUT1-10 119(high word) 120(low word)
        //address:Switch
        //Range Setting for each input  200
        /*
        m_comboxRange.AddString(_T("RAW DATA"));
        m_comboxRange.AddString(_T("10K C"));
        m_comboxRange.AddString(_T("10K F"));
        m_comboxRange.AddString(_T("0-100%"));
        m_comboxRange.AddString(_T("ON/OFF"));
        m_comboxRange.AddString(_T("OFF/ON"));
        m_comboxRange.AddString(_T("Pulse Input"));
        m_comboxRange.AddString(_T("Lighting Control"));
        m_comboxRange.AddString(_T("TYPE3 10K C"));
        m_comboxRange.AddString(_T("TYPE3 10K F"));
        m_comboxRange.AddString(_T("NO USE"));
        m_comboxRange.AddString(_T("0-5V"));
        m_comboxRange.AddString(_T("0-10V"));
        m_comboxRange.AddString(_T("0-20I"));
        */
        CString strresult;
        int regValue;
        for(int i = 1; i<=10; i++)
        {
            regValue=product_register_value[119+2*(i-1)]*65535+product_register_value[120+2*(i-1)];
            int RangeValue    =   product_register_value[200+i-1];

            m_T3_Input_List.SetItemText(i-1,1,strresult);
            strresult.Format(_T("%4d/%4d/%4d  %2d:%2d"),product_register_value[139+3*(i-1)],product_register_value[140+3*(i-1)],product_register_value[141+3*(i-1)],product_register_value[142+3*(i-1)],product_register_value[143+3*(i-1)]);
            strresult=GetValue(regValue,RangeValue);
            m_T3_Input_List.SetItemText(i-1,2,strresult);



            if (RangeValue>=0&&RangeValue<=13)
            {
                strresult = Range_Units[RangeValue] ;
            }
            m_T3_Input_List.SetItemText(i-1,3,strresult);
            strresult.Format(_T("%d"),product_register_value[210+i-1]);
            m_T3_Input_List.SetItemText(i-1,4,strresult);
        }
    }
    else if (product_register_value[7]==PM_T38I13O)
    {
        m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_T3_Input_List.InsertColumn(0, _T("Label"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        //m_T3_Input_List.InsertColumn(1, _T("Full Label"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(1, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(2, _T("Date Stamp"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(3, _T("Range"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(4, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(5, _T("Lighting Time"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(6, _T("Time Left"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

        for (int i=0; i<8; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }
        // Input
        CString strresult;
        int regValue;
        for(int i = 1; i<=8; i++)
        {


            strresult.Format(_T("%4d/%4d/%4d  %2d:%2d"),product_register_value[134+3*(i-1)]
                             ,product_register_value[135+3*(i-1)]
                             ,product_register_value[136+3*(i-1)]
                             ,product_register_value[137+3*(i-1)]
                             ,product_register_value[138+3*(i-1)]);

            m_T3_Input_List.SetItemText(i-1,2,strresult);

            int RangeValue=product_register_value[183+i-1];

            if (RangeValue>=0&&RangeValue<=13)
            {
                strresult = Range_Units[RangeValue] ;
            }

            m_T3_Input_List.SetItemText(i-1,3,strresult);


            regValue=(short)product_register_value[119+2*(i-1)];

            strresult=GetValue(regValue,RangeValue);

            m_T3_Input_List.SetItemText(i-1,1,strresult);


            strresult.Format(_T("%d"),product_register_value[191+i-1]);
            m_T3_Input_List.SetItemText(i-1,4,strresult);

            strresult.Format(_T("%d min"),product_register_value[199+i-1]);
            m_T3_Input_List.SetItemText(i-1,5,strresult);

            strresult.Format(_T("%d min"),product_register_value[207+i-1]);
            m_T3_Input_List.SetItemText(i-1,6,strresult);
        }

    }
    else if (product_register_value[7]==PM_T38AI8AO6DO)
    {
        m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_T3_Input_List.InsertColumn(0, _T("Label"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_T3_Input_List.InsertColumn(1, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(2, _T("Range"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(3, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

        for (int i=0; i<8; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }

        CString strresult;
        int regValue;
        for(int i = 1; i<=8; i++)
        {
            int RangeValue =   product_register_value[225+i-1];
            if (RangeValue>=0&&RangeValue<=13)
            {
                strresult = Range_Units[RangeValue] ;
            }
            m_T3_Input_List.SetItemText(i-1,2,strresult);

            //regValue=(short)product_register_value[116+i-1];
			if (RangeValue!=6)
			{
				regValue=product_register_value[116+2*i-1];
			}
			else
			{
				regValue=product_register_value[116+2*(i-1)]*65536+product_register_value[116+2*i-1];
			}
            strresult = GetValue(regValue,RangeValue);
            m_T3_Input_List.SetItemText(i-1,1,strresult);
            strresult.Format(_T("%d"),product_register_value[200+i-1]);
            m_T3_Input_List.SetItemText(i-1,3,strresult);
        }
    }
    else if (product_register_value[7]==PM_T36CT)
    {
        m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_T3_Input_List.InsertColumn(0, _T("Label"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_T3_Input_List.InsertColumn(1, _T("Value"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(2, _T("Range"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(3, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

        for (int i=0; i<6; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }
        for (int i=6; i<16; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Analog_Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }
        CString strresult;
        int regValue;
        for(int i = 1; i<=6; i++)
        {
            int RangeValue=  product_register_value[126+i-1] ;
            if (RangeValue>=0&&RangeValue<=13)
            {
                strresult = Range_Units[RangeValue] ;
            }
            m_T3_Input_List.SetItemText(i-1,2,strresult);

            regValue=product_register_value[108+i-1];
            strresult = GetValue(regValue,RangeValue);
            m_T3_Input_List.SetItemText(i-1,1,strresult);

            strresult.Format(_T("%d"),product_register_value[142+i-1]);
            m_T3_Input_List.SetItemText(i-1,3,strresult);
        }


        for(int i = 7; i<=16; i++)
        {
            regValue=product_register_value[114+i-7];
            if (0==product_register_value[132+i-7])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (1==product_register_value[132+i-7])
            {
                regValue=(short)product_register_value[114+i-7];

                strresult.Format(_T("%.1f C"),(float)regValue/10.0);
            }
            else if (2==product_register_value[132+i-7])
            {

                regValue=(short)product_register_value[114+i-7];
                strresult=_T("10K F");
                strresult.Format(_T("%.1f F"),(float)regValue/10.0);
            }
            else if (3==product_register_value[132+i-7])
            {

                strresult.Format(_T("%.1f"),(float)regValue);
                strresult+=_T("%");
            }
            else if (4==product_register_value[132+i-7])
            {
                //strresult.Format(_T("%.1f F"),(float)regValue/10.0);
                if (regValue==0)
                {
                    strresult=_T("OFF");
                }
                else
                {
                    strresult=_T("ON");
                }
            }
            else if (5==product_register_value[132+i-7])
            {
                if (regValue==0)
                {
                    strresult=_T("ON");
                }
                else
                {
                    strresult=_T("OFF");
                }
            }
            else if (6==product_register_value[132+i-7])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (7==product_register_value[132+i-7])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (8==product_register_value[132+i-7])
            {
                //strresult=_T("TYPE3 10K C");
                strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (9==product_register_value[132+i-7])
            {
                /*strresult=_T("TYPE3 10K F");*/
                strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
            }
            else if (10==product_register_value[132+i-7])
            {
                strresult=_T("0");
                //strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (11==product_register_value[132+i-7])
            {
                //strresult=_T("0-5V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (12==product_register_value[132+i-1])
            {
                //strresult=_T("0-10V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (13==product_register_value[132+i-7])
            {

                strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
            }

            m_T3_Input_List.SetItemText(i-1,1,strresult);

            strresult.Format(_T("%d"),product_register_value[132+i-7]);


            if (product_register_value[132+i-7]>=0&&product_register_value[132+i-7]<=13)
            {
                strresult = Range_Units[product_register_value[132+i-7]] ;
            }
            m_T3_Input_List.SetItemText(i-1,2,strresult);
            strresult.Format(_T("%d"),product_register_value[148+i-7]);
            m_T3_Input_List.SetItemText(i-1,3,strresult);

        }

    }
    else if (product_register_value[7]==PM_T3IOA)
    {
        m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_T3_Input_List.InsertColumn(0, _T("Label"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_T3_Input_List.InsertColumn(1, _T("Value"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(2, _T("Range"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(3, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

        for (int i=0; i<8; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }

        CString strresult;
        int regValue;
        for(int i = 1; i<=8; i++)
        {
            int RangeValue =   product_register_value[118+i-1];
            if (RangeValue>=0&&RangeValue<=13)
            {
                strresult = Range_Units[RangeValue] ;
            }
            m_T3_Input_List.SetItemText(i-1,2,strresult);
            regValue=(short)product_register_value[108+i-1];
            strresult = GetValue(regValue,RangeValue);
            m_T3_Input_List.SetItemText(i-1,1,strresult);
            strresult.Format(_T("%d"),product_register_value[126+i-1]);
            m_T3_Input_List.SetItemText(i-1,3,strresult);
        }
    }
    else if (product_register_value[7]==PM_T332AI)
    {
        m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_T3_Input_List.InsertColumn(0, _T("Label"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_T3_Input_List.InsertColumn(1, _T("Value"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(2, _T("Range"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(3, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        for (int i=0; i<32; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }

        CString strresult;
        int regValue;
        for(int i = 1; i<=32; i++)
        {



            int RangeValue = product_register_value[228+i-1];
            if (RangeValue>=0&&RangeValue<=13)
            {
                strresult = Range_Units[RangeValue] ;
            }

            m_T3_Input_List.SetItemText(i-1,2,strresult);

            regValue=(short)product_register_value[100+i-1];
            //regValue=(short)product_register_value[INPUT1_PULSE_COUNT_LOW+2*(i-1)];


            strresult = GetValue(regValue,RangeValue);
            m_T3_Input_List.SetItemText(i-1,1,strresult);

            strresult.Format(_T("%d"),product_register_value[260+i-1]);
            m_T3_Input_List.SetItemText(i-1,3,strresult);
        }
    }
    else if (product_register_value[7]==PM_T3PT10)
    {
        m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_T3_Input_List.InsertColumn(0, _T("Number"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_T3_Input_List.InsertColumn(1, _T("Name"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(2, _T("Value"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(3, _T("Range"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(4, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(5, _T("Status"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

        for (int i=1; i<11; i++)
        {
            strTemp.Format(_T("%d"),i);
            m_T3_Input_List.InsertItem(i-1,strTemp);
            strTemp=Get_Table_Name(m_sn,_T("PT_Channel"),i+1);
            m_T3_Input_List.SetItemText(i-1,1,strTemp);

        }
        CString strresult;
        int regValue;
        if (product_register_value[13]==0)//Float
        {
            int accuracy=product_register_value[18];
            for(int i = 1; i<=10; i++)
            {
                if (0==product_register_value[228+i-1])
                {
                    strresult=_T("C");
                }
                else if (1==product_register_value[228+i-1])
                {
                    strresult=_T("F");
                }
                else if (2==product_register_value[228+i-1])
                {
                    strresult=_T("Ω");
                }
                m_T3_Input_List.SetItemText(i-1,3,strresult);

                DataFormat mydata;
                CString unit=strresult;
                regValue=0;
                regValue=product_register_value[101+2*(i-1)];
                int Data=regValue;
                int Data1=regValue&0x00FF;
                int Data2=(regValue&0xFF00)>>8;
                mydata.charData[3]=Data2;
                mydata.charData[2]=Data1;

                regValue=0;
                regValue=product_register_value[100+2*(i-1)];
                Data=regValue;
                Data1=regValue&0x00FF;
                Data2=(regValue&0xFF00)>>8;
                mydata.charData[1]=Data2;
                mydata.charData[0]=Data1;
                if (1==accuracy)
                {
                    strresult.Format(_T("%0.1f"),mydata.floatData);
                }
                else if (2==accuracy)
                {
                    strresult.Format(_T("%0.2f"),mydata.floatData);
                }
                else if (3==accuracy)
                {
                    strresult.Format(_T("%0.3f"),mydata.floatData);
                }
                else if (4==accuracy)
                {
                    strresult.Format(_T("%0.4f"),mydata.floatData);
                }
                else if (5==accuracy)
                {
                    strresult.Format(_T("%0.5f"),mydata.floatData);
                }
                else if (6==accuracy)
                {
                    strresult.Format(_T("%0.6f"),mydata.floatData);
                }


                strresult+=unit;

                m_T3_Input_List.SetItemText(i-1,2,strresult);
                strresult.Format(_T("%d"),product_register_value[238+i-1]);

                m_T3_Input_List.SetItemText(i-1,4,strresult);
                if (0==product_register_value[248+i-1])
                {
                    strresult=_T("Pass");
                }
                else if (1==product_register_value[248+i-1])
                {
                    strresult=_T("Unconnect");
                }
                else if (2==product_register_value[248+i-1])
                {
                    strresult=_T("Unconnect");
                }

                m_T3_Input_List.SetItemText(i-1,5,strresult);
            }
        }
        else//Int
        {
            for(int i = 1; i<=10; i++)
            {
                if (0==product_register_value[228+i-1])
                {
                    strresult=_T("C");
                }
                else if (1==product_register_value[228+i-1])
                {
                    strresult=_T("F");
                }
                else if (2==product_register_value[228+i-1])
                {
                    strresult=_T("Ω");
                }

                m_T3_Input_List.SetItemText(i-1,3,strresult);
                CString unit=strresult;
                regValue=product_register_value[101+2*(i-1)];
                strresult.Format(_T("%0.1f"),((float)regValue)/10);
                strresult+=unit;

                m_T3_Input_List.SetItemText(i-1,2,strresult);
                strresult.Format(_T("%d"),product_register_value[238+i-1]);

                m_T3_Input_List.SetItemText(i-1,4,strresult);
                if (0==product_register_value[248+i-1])
                {
                    strresult=_T("Pass");
                }
                else if (1==product_register_value[248+i-1])
                {
                    strresult=_T("Short");
                }
                else if (2==product_register_value[248+i-1])
                {
                    strresult=_T("Unconnect");
                }

                m_T3_Input_List.SetItemText(i-1,5,strresult);
            }
        }
    }
    else if (product_register_value[7]==PM_T322AI)
    {
        m_T3_Input_List.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_T3_Input_List.SetExtendedStyle(m_T3_Input_List.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_T3_Input_List.InsertColumn(0, _T("Number"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_T3_Input_List.InsertColumn(1, _T("Name"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(2, _T("Value"), 90, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(3, _T("Range"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(4, _T("Filter"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_T3_Input_List.InsertColumn(5, _T("Calibration"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        for (int i=0; i<22; i++)
        {
            strTemp.Format(_T("%d"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.SetItemText(i,1,strTemp);
        }
        CString strresult;
        int regValue;
        for(int i = 1; i<=22; i++)
        {



            int RangeValue =   product_register_value[225+i-1];
            int RangeValue1=RangeValue;
            if (i<=11)
            {
                if (RangeValue1 == 6)
                {
                    RangeValue1 = 14;
                }
                if (RangeValue1>=0&&RangeValue1<=14)
                {
                    strresult = Range_Units[RangeValue1] ;
                }
            }
            else
            {
                if (RangeValue1>=0&&RangeValue1<=14)
                {
                    strresult = Range_Units[RangeValue1] ;
                }
            }


            m_T3_Input_List.SetItemText(i-1,3,strresult);

//             if (i<=11)
//             {
            if (RangeValue!=6)
            {
                regValue=product_register_value[100+2*i-1];
            }
            else
            {
                regValue=product_register_value[100+2*(i-1)]*65536+product_register_value[100+2*i-1];
            }
//             }
//             else
//             {
//                 regValue=product_register_value[122+i-12];
//             }



            strresult = GetValue(regValue,RangeValue);
            m_T3_Input_List.SetItemText(i-1,2,strresult);

            strresult.Format(_T("%d"),product_register_value[200+i-1]);
            m_T3_Input_List.SetItemText(i-1,4,strresult);

            strresult.Format(_T("%d"),product_register_value[250+i-1]);
            m_T3_Input_List.SetItemText(i-1,5,strresult);
        }
    }
    else
    {
        //AfxMessageBox(_T("This Product is not T3 Modules!")) ;
    }
    m_T3_Input_List.ShowWindow(SW_SHOW);

    if (!hFirstThread)
    {
        hFirstThread = CreateThread(NULL,NULL,_ReadMultiRegisters_T3_Input,this,NULL,0);
        CloseHandle(hFirstThread);
        hFirstThread = NULL;
    }

}

LRESULT CT3ModulesView::Fresh_Input_List(WPARAM wParam,LPARAM lParam)
{
    m_sn=product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;
    CString strTemp;

    m_T3_Input_List.DeleteAllItems();

    if (product_register_value[7]==PM_T34AO)
    {
        for (int i=0; i<10; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }
        CString strresult;
        int regValue;
        for(int i = 1; i<=10; i++)
        {
            regValue=product_register_value[119+2*(i-1)]*65535+product_register_value[120+2*(i-1)];
            if (0==product_register_value[200+i-1])
            {
                strresult.Format(_T("%d"),regValue);

            }
            else if (1==product_register_value[200+i-1])
            {
                strresult.Format(_T("%.1f C"),(float)regValue/10.0);
            }
            else if (2==product_register_value[200+i-1])
            {
                strresult=_T("10K F");
                strresult.Format(_T("%.1f F"),(float)regValue/10.0);
            }
            else if (3==product_register_value[200+i-1])
            {
                strresult.Format(_T("%.1f"),(float)regValue);
                strresult+=_T("%");
            }
            else if (4==product_register_value[200+i-1])
            {
                //strresult.Format(_T("%.1f F"),(float)regValue/10.0);
                if (regValue==0)
                {
                    strresult=_T("OFF");
                }
                else
                {
                    strresult=_T("ON");
                }
            }
            else if (5==product_register_value[200+i-1])
            {
                if (regValue==0)
                {
                    strresult=_T("ON");
                }
                else
                {
                    strresult=_T("OFF");
                }
            }
            else if (6==product_register_value[200+i-1])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (7==product_register_value[200+i-1])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (8==product_register_value[200+i-1])
            {
                //strresult=_T("TYPE3 10K C");
                strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (9==product_register_value[200+i-1])
            {
                /*strresult=_T("TYPE3 10K F");*/
                strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
            }
            else if (10==product_register_value[200+i-1])
            {
                strresult=_T("0");
                //strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (11==product_register_value[200+i-1])
            {
                //strresult=_T("0-5V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (12==product_register_value[200+i-1])
            {
                //strresult=_T("0-10V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (13==product_register_value[200+i-1])
            {
                //strresult=_T("0-20I");
                strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
            }

            m_T3_Input_List.SetItemText(i-1,1,strresult);
            strresult.Format(_T("%4d/%4d/%4d  %2d:%2d"),product_register_value[139+3*(i-1)],product_register_value[140+3*(i-1)],product_register_value[141+3*(i-1)],product_register_value[142+3*(i-1)],product_register_value[143+3*(i-1)]);

            m_T3_Input_List.SetItemText(i-1,2,strresult);

            int RangeValue =   product_register_value[200+i-1];
            strresult = Range_Units[0];
            if (RangeValue>=0&&RangeValue<=13)
            {
                strresult = Range_Units[RangeValue] ;
            }


            m_T3_Input_List.SetItemText(i-1,3,strresult);
            strresult.Format(_T("%d"),product_register_value[210+i-1]);
            m_T3_Input_List.SetItemText(i-1,4,strresult);
        }
    }
    else if (product_register_value[7]==PM_T38I13O)
    {


        for (int i=0; i<8; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }
        // Input
        CString strresult;
        int regValue;
        for(int i = 1; i<=8; i++)
        {


            strresult.Format(_T("%4d/%4d/%4d  %2d:%2d"),product_register_value[134+3*(i-1)]
                             ,product_register_value[135+3*(i-1)]
                             ,product_register_value[136+3*(i-1)]
                             ,product_register_value[137+3*(i-1)]
                             ,product_register_value[138+3*(i-1)]);

            m_T3_Input_List.SetItemText(i-1,2,strresult);

            int RangeValue =   product_register_value[183+i-1];
            strresult = Range_Units[0];
            if (RangeValue>=0&&RangeValue<=13)
            {
                strresult = Range_Units[RangeValue] ;
            }

            m_T3_Input_List.SetItemText(i-1,3,strresult);


            regValue=(short)product_register_value[119+2*(i-1)];

            if (0==product_register_value[183+i-1])
            {
                strresult.Format(_T("%d"),regValue);

            }
            else if (1==product_register_value[183+i-1])
            {
                strresult.Format(_T("%.1f C"),(float)regValue/10.0);
            }
            else if (2==product_register_value[183+i-1])
            {
                strresult=_T("10K F");
                strresult.Format(_T("%.1f F"),(float)regValue/10.0);
            }
            else if (3==product_register_value[183+i-1])
            {

                strresult.Format(_T("%.1f"),(float)regValue);
                strresult+=_T("%");
            }
            else if (4==product_register_value[183+i-1])
            {
                //strresult.Format(_T("%.1f F"),(float)regValue/10.0);
                if (regValue==0)
                {
                    strresult=_T("OFF");
                }
                else
                {
                    strresult=_T("ON");
                }
            }
            else if (5==product_register_value[183+i-1])
            {
                if (regValue==0)
                {
                    strresult=_T("ON");
                }
                else
                {
                    strresult=_T("OFF");
                }
            }
            else if (6==product_register_value[183+i-1])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (7==product_register_value[183+i-1])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (8==product_register_value[183+i-1])
            {
                //strresult=_T("TYPE3 10K C");
                strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (9==product_register_value[183+i-1])
            {
                /*strresult=_T("TYPE3 10K F");*/
                strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
            }
            else if (10==product_register_value[183+i-1])
            {
                strresult=_T("0");
                //strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (11==product_register_value[183+i-1])
            {
                //strresult=_T("0-5V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (12==product_register_value[183+i-1])
            {
                //strresult=_T("0-10V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (13==product_register_value[183+i-1])
            {
                //strresult=_T("0-20I");
                strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
            }

            m_T3_Input_List.SetItemText(i-1,1,strresult);


            strresult.Format(_T("%d"),product_register_value[191+i-1]);
            m_T3_Input_List.SetItemText(i-1,4,strresult);

            strresult.Format(_T("%d min"),product_register_value[199+i-1]);
            m_T3_Input_List.SetItemText(i-1,5,strresult);

            strresult.Format(_T("%d min"),product_register_value[207+i-1]);
            m_T3_Input_List.SetItemText(i-1,6,strresult);
        }

    }
    else if (product_register_value[7]==PM_T36CT)
    {


        for (int i=0; i<6; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }
        for (int i=6; i<16; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Analog_Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }
        CString strresult;
        int regValue;
        for(int i = 1; i<=6; i++)
        {
            regValue=product_register_value[108+i-1];


            if (0==product_register_value[126+i-1])
            {
                strresult.Format(_T("%d"),regValue);

            }
            else if (1==product_register_value[126+i-1])
            {
                strresult.Format(_T("%.1f C"),(float)regValue/10.0);
            }
            else if (2==product_register_value[126+i-1])
            {
                strresult=_T("10K F");
                strresult.Format(_T("%.1f F"),(float)regValue/10.0);
            }
            else if (3==product_register_value[126+i-1])
            {

                strresult.Format(_T("%.1f"),(float)regValue);
                strresult+=_T("%");
            }
            else if (4==product_register_value[126+i-1])
            {
                //strresult.Format(_T("%.1f F"),(float)regValue/10.0);
                if (regValue==0)
                {
                    strresult=_T("OFF");
                }
                else
                {
                    strresult=_T("ON");
                }
            }
            else if (5==product_register_value[126+i-1])
            {
                if (regValue==0)
                {
                    strresult=_T("ON");
                }
                else
                {
                    strresult=_T("OFF");
                }
            }
            else if (6==product_register_value[126+i-1])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (7==product_register_value[126+i-1])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (8==product_register_value[126+i-1])
            {
                //strresult=_T("TYPE3 10K C");
                strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (9==product_register_value[126+i-1])
            {
                /*strresult=_T("TYPE3 10K F");*/
                strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
            }
            else if (10==product_register_value[126+i-1])
            {
                strresult=_T("0");
                //strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (11==product_register_value[126+i-1])
            {
                //strresult=_T("0-5V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (12==product_register_value[126+i-1])
            {
                //strresult=_T("0-10V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (13==product_register_value[126+i-1])
            {
                //strresult=_T("0-20I");
                strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
            }


            m_T3_Input_List.SetItemText(i-1,1,strresult);



            int RangeValue =   product_register_value[126+i-1];
            strresult = Range_Units[0];
            if (RangeValue>=0&&RangeValue<=13)
            {
                strresult = Range_Units[RangeValue] ;
            }
            m_T3_Input_List.SetItemText(i-1,2,strresult);
            strresult.Format(_T("%d"),product_register_value[142+i-1]);

            m_T3_Input_List.SetItemText(i-1,3,strresult);
        }


        for(int i = 7; i<=16; i++)
        {
            regValue=product_register_value[114+i-7];
            if (0==product_register_value[132+i-7])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (1==product_register_value[132+i-7])
            {
                regValue=(short)product_register_value[114+i-7];

                strresult.Format(_T("%.1f C"),(float)regValue/10.0);
            }
            else if (2==product_register_value[132+i-7])
            {

                regValue=(short)product_register_value[114+i-7];
                strresult=_T("10K F");
                strresult.Format(_T("%.1f F"),(float)regValue/10.0);
            }
            else if (3==product_register_value[132+i-7])
            {

                strresult.Format(_T("%.1f"),(float)regValue);
                strresult+=_T("%");
            }
            else if (4==product_register_value[132+i-7])
            {
                //strresult.Format(_T("%.1f F"),(float)regValue/10.0);
                if (regValue==0)
                {
                    strresult=_T("OFF");
                }
                else
                {
                    strresult=_T("ON");
                }
            }
            else if (5==product_register_value[132+i-7])
            {
                if (regValue==0)
                {
                    strresult=_T("ON");
                }
                else
                {
                    strresult=_T("OFF");
                }
            }
            else if (6==product_register_value[132+i-7])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (7==product_register_value[132+i-7])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (8==product_register_value[132+i-7])
            {
                //strresult=_T("TYPE3 10K C");
                strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (9==product_register_value[132+i-7])
            {
                /*strresult=_T("TYPE3 10K F");*/
                strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
            }
            else if (10==product_register_value[132+i-7])
            {
                strresult=_T("0");
                //strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (11==product_register_value[132+i-7])
            {
                //strresult=_T("0-5V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (12==product_register_value[132+i-1])
            {
                //strresult=_T("0-10V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (13==product_register_value[132+i-7])
            {

                strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
            }

            m_T3_Input_List.SetItemText(i-1,1,strresult);

            strresult.Format(_T("%d"),product_register_value[132+i-7]);
            if (0==product_register_value[132+i-1])
            {
                strresult=_T("RAW DATA");
            }
            else if (1==product_register_value[132+i-7])
            {
                strresult=_T("10K C");
            }
            else if (2==product_register_value[132+i-7])
            {
                strresult=_T("10K F");
            }
            else if (3==product_register_value[132+i-7])
            {
                strresult=_T("0-100%");
            }
            else if (4==product_register_value[132+i-7])
            {
                strresult=_T("ON/OFF");
            }
            else if (5==product_register_value[132+i-7])
            {
                strresult=_T("OFF/ON");
            }
            else if (6==product_register_value[132+i-7])
            {
                strresult=_T("Pulse Input");
            }
            else if (7==product_register_value[132+i-7])
            {
                strresult=_T("Lighting Control");
            }
            else if (8==product_register_value[132+i-7])
            {
                strresult=_T("TYPE3 10K C");
            }
            else if (9==product_register_value[132+i-7])
            {
                strresult=_T("TYPE3 10K F");
            }
            else if (10==product_register_value[132+i-7])
            {
                strresult=_T("NO USE");
            }
            else if (11==product_register_value[132+i-7])
            {
                strresult=_T("0-5V");
            }
            else if (12==product_register_value[132+i-7])
            {
                strresult=_T("0-10V");
            }
            else if (13==product_register_value[132+i-7])
            {
                strresult=_T("0-20 ma");
            }


            m_T3_Input_List.SetItemText(i-1,2,strresult);
            strresult.Format(_T("%d"),product_register_value[148+i-7]);
            m_T3_Input_List.SetItemText(i-1,3,strresult);

        }

    }
    else if (product_register_value[7]==PM_T3IOA)
    {

        for (int i=0; i<8; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }

        CString strresult;
        int regValue;
        for(int i = 1; i<=8; i++)
        {
            regValue=(short)product_register_value[108+i-1];


            if (0==product_register_value[118+i-1])
            {
                strresult.Format(_T("%d"),regValue);

            }
            else if (1==product_register_value[118+i-1])
            {
                strresult.Format(_T("%.1f C"),(float)regValue/10.0);
            }
            else if (2==product_register_value[118+i-1])
            {
                strresult=_T("10K F");
                strresult.Format(_T("%.1f F"),(float)regValue/10.0);
            }
            else if (3==product_register_value[118+i-1])
            {

                strresult.Format(_T("%.1f"),(float)regValue);
                strresult+=_T("%");
            }
            else if (4==product_register_value[118+i-1])
            {
                //strresult.Format(_T("%.1f F"),(float)regValue/10.0);
                if (regValue==0)
                {
                    strresult=_T("OFF");
                }
                else
                {
                    strresult=_T("ON");
                }
            }
            else if (5==product_register_value[118+i-1])
            {
                if (regValue==0)
                {
                    strresult=_T("ON");
                }
                else
                {
                    strresult=_T("OFF");
                }
            }
            else if (6==product_register_value[118+i-1])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (7==product_register_value[118+i-1])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (8==product_register_value[118+i-1])
            {
                //strresult=_T("TYPE3 10K C");
                strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (9==product_register_value[118+i-1])
            {
                /*strresult=_T("TYPE3 10K F");*/
                strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
            }
            else if (10==product_register_value[118+i-1])
            {
                strresult=_T("0");
                //strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (11==product_register_value[118+i-1])
            {
                //strresult=_T("0-5V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (12==product_register_value[118+i-1])
            {
                //strresult=_T("0-10V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (13==product_register_value[118+i-1])
            {
                //strresult=_T("0-20I");
                strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
            }



            m_T3_Input_List.SetItemText(i-1,1,strresult);

            strresult.Format(_T("%d"),product_register_value[118+i-1]);
            if (0==product_register_value[118+i-1])
            {
                strresult=_T("RAW DATA");
            }
            else if (1==product_register_value[118+i-1])
            {
                strresult=_T("10K C");
            }
            else if (2==product_register_value[118+i-1])
            {
                strresult=_T("10K F");
            }
            else if (3==product_register_value[118+i-1])
            {
                strresult=_T("0-100%");
            }
            else if (4==product_register_value[118+i-1])
            {
                strresult=_T("ON/OFF");
            }
            else if (5==product_register_value[118+i-1])
            {
                strresult=_T("OFF/ON");
            }
            else if (6==product_register_value[118+i-1])
            {
                strresult=_T("Pulse Input");
            }
            else if (7==product_register_value[118+i-1])
            {
                strresult=_T("Lighting Control");
            }
            else if (8==product_register_value[118+i-1])
            {
                strresult=_T("TYPE3 10K C");
            }
            else if (9==product_register_value[118+i-1])
            {
                strresult=_T("TYPE3 10K F");
            }
            else if (10==product_register_value[118+i-1])
            {
                strresult=_T("NO USE");
            }
            else if (11==product_register_value[118+i-1])
            {
                strresult=_T("0-5V");
            }
            else if (12==product_register_value[118+i-1])
            {
                strresult=_T("0-10V");
            }
            else if (13==product_register_value[118+i-1])
            {
                strresult=_T("0-20 ma");
            }


            m_T3_Input_List.SetItemText(i-1,2,strresult);
            strresult.Format(_T("%d"),product_register_value[126+i-1]);
            m_T3_Input_List.SetItemText(i-1,3,strresult);
        }
    }
    else if (product_register_value[7]==PM_T38AI8AO6DO)
    {

        for (int i=0; i<8; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }

        CString strresult;
        int regValue;
        for(int i = 1; i<=8; i++)
        {
            int RangeValue =   product_register_value[225+i-1];
            if (RangeValue>=0&&RangeValue<=13)
            {
                strresult = Range_Units[RangeValue] ;
            }
            m_T3_Input_List.SetItemText(i-1,2,strresult);
            if (RangeValue!=6)
            {
                regValue=product_register_value[116+2*i-1];
            }
            else
            {
                regValue=product_register_value[116+2*(i-1)]*65536+product_register_value[116+2*i-1];
            }
            strresult = GetValue(regValue,RangeValue);
            m_T3_Input_List.SetItemText(i-1,1,strresult);
            strresult.Format(_T("%d"),product_register_value[200+i-1]);
            m_T3_Input_List.SetItemText(i-1,3,strresult);
        }
    }
    else if (product_register_value[7]==PM_T332AI)
    {

        for (int i=0; i<32; i++)
        {
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
        }

        CString strresult;
        int regValue;
        for(int i = 1; i<=32; i++)
        {

            int RangeValue =   product_register_value[228+i-1];
            strresult = Range_Units[0];
            if (RangeValue>=0&&RangeValue<=13)
            {
                strresult = Range_Units[RangeValue] ;
            }

            m_T3_Input_List.SetItemText(i-1,2,strresult);



            regValue=product_register_value[100+i-1];
            //regValue=(short)product_register_value[INPUT1_PULSE_COUNT_LOW+2*(i-1)];

            if (0==product_register_value[228+i-1])
            {
                strresult.Format(_T("%d"),regValue);

            }
            else if (1==product_register_value[228+i-1])
            {
                strresult.Format(_T("%.1f C"),(float)regValue/10.0);
            }
            else if (2==product_register_value[228+i-1])
            {
                strresult=_T("10K F");
                strresult.Format(_T("%.1f F"),(float)regValue/10.0);
            }
            else if (3==product_register_value[228+i-1])
            {

                strresult.Format(_T("%.1f"),(float)regValue);
                strresult+=_T("%");
            }
            else if (4==product_register_value[228+i-1])
            {
                //strresult.Format(_T("%.1f F"),(float)regValue/10.0);
                if (regValue==0)
                {
                    strresult=_T("OFF");
                }
                else
                {
                    strresult=_T("ON");
                }
            }
            else if (5==product_register_value[228+i-1])
            {
                if (regValue==0)
                {
                    strresult=_T("ON");
                }
                else
                {
                    strresult=_T("OFF");
                }
            }
            else if (6==product_register_value[228+i-1])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (7==product_register_value[228+i-1])
            {
                strresult.Format(_T("%d"),regValue);
            }
            else if (8==product_register_value[228+i-1])
            {
                //strresult=_T("TYPE3 10K C");
                strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (9==product_register_value[228+i-1])
            {
                /*strresult=_T("TYPE3 10K F");*/
                strresult.Format(_T("%0.1f F"),(float)regValue/10.0);
            }
            else if (10==product_register_value[228+i-1])
            {
                strresult=_T("0");
                //strresult.Format(_T("%0.1f C"),(float)regValue/10.0);
            }
            else if (11==product_register_value[228+i-1])
            {
                //strresult=_T("0-5V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (12==product_register_value[228+i-1])
            {
                //strresult=_T("0-10V");
                strresult.Format(_T("%0.1f V"),(float)regValue/1000.0);
            }
            else if (13==product_register_value[228+i-1])
            {
                //strresult=_T("0-20I");
                strresult.Format(_T("%0.1f ma"),(float)regValue/1000.0);
            }

            m_T3_Input_List.SetItemText(i-1,1,strresult);
            strresult.Format(_T("%d"),product_register_value[260+i-1]);
            m_T3_Input_List.SetItemText(i-1,3,strresult);
        }
    }
    else if (product_register_value[7]==PM_T3PT10)
    {
        for (int i=1; i<11; i++)
        {
            strTemp.Format(_T("%d"),i);
            m_T3_Input_List.InsertItem(i-1,strTemp);
            strTemp=Get_Table_Name(m_sn,_T("PT_Channel"),i+1);
            m_T3_Input_List.SetItemText(i-1,1,strTemp);

        }
        CString strresult;
        int regValue;
        if (product_register_value[13]==0)//Float
        {
            int accuracy=product_register_value[18];
            for(int i = 1; i<=10; i++)
            {
                if (0==product_register_value[228+i-1])
                {
                    strresult=_T("C");
                }
                else if (1==product_register_value[228+i-1])
                {
                    strresult=_T("F");
                }
                else if (2==product_register_value[228+i-1])
                {
                    strresult=_T("Ω");
                }
                m_T3_Input_List.SetItemText(i-1,3,strresult);

                DataFormat mydata;
                CString unit=strresult;
                regValue=0;
                regValue=product_register_value[101+2*(i-1)];
                int Data=regValue;
                int Data1=regValue&0x00FF;
                int Data2=(regValue&0xFF00)>>8;
                mydata.charData[3]=Data2;
                mydata.charData[2]=Data1;

                regValue=0;
                regValue=product_register_value[100+2*(i-1)];
                Data=regValue;
                Data1=regValue&0x00FF;
                Data2=(regValue&0xFF00)>>8;
                mydata.charData[1]=Data2;
                mydata.charData[0]=Data1;
                if (1==accuracy)
                {
                    strresult.Format(_T("%0.1f"),mydata.floatData);
                }
                else if (2==accuracy)
                {
                    strresult.Format(_T("%0.2f"),mydata.floatData);
                }
                else if (3==accuracy)
                {
                    strresult.Format(_T("%0.3f"),mydata.floatData);
                }
                else if (4==accuracy)
                {
                    strresult.Format(_T("%0.4f"),mydata.floatData);
                }
                else if (5==accuracy)
                {
                    strresult.Format(_T("%0.5f"),mydata.floatData);
                }
                else if (6==accuracy)
                {
                    strresult.Format(_T("%0.6f"),mydata.floatData);
                }


                strresult+=unit;

                m_T3_Input_List.SetItemText(i-1,2,strresult);
                strresult.Format(_T("%d"),product_register_value[238+i-1]);

                m_T3_Input_List.SetItemText(i-1,4,strresult);
                if (0==product_register_value[248+i-1])
                {
                    strresult=_T("Pass");
                }
                else if (1==product_register_value[248+i-1])
                {
                    strresult=_T("Unconnect");
                }
                else if (2==product_register_value[248+i-1])
                {
                    strresult=_T("Unconnect");
                }

                m_T3_Input_List.SetItemText(i-1,5,strresult);
            }
        }
        else//Int
        {
            for(int i = 1; i<=10; i++)
            {
                if (0==product_register_value[228+i-1])
                {
                    strresult=_T("C");
                }
                else if (1==product_register_value[228+i-1])
                {
                    strresult=_T("F");
                }
                else if (2==product_register_value[228+i-1])
                {
                    strresult=_T("Ω");
                }

                m_T3_Input_List.SetItemText(i-1,3,strresult);
                CString unit=strresult;
                regValue=product_register_value[101+2*(i-1)];
                strresult.Format(_T("%0.1f"),((float)regValue)/10);
                strresult+=unit;

                m_T3_Input_List.SetItemText(i-1,2,strresult);
                strresult.Format(_T("%d"),product_register_value[238+i-1]);

                m_T3_Input_List.SetItemText(i-1,4,strresult);
                if (0==product_register_value[248+i-1])
                {
                    strresult=_T("Pass");
                }
                else if (1==product_register_value[248+i-1])
                {
                    strresult=_T("Short");
                }
                else if (2==product_register_value[248+i-1])
                {
                    strresult=_T("Unconnect");
                }

                m_T3_Input_List.SetItemText(i-1,5,strresult);
            }
        }
    }
    else if (product_register_value[7]==PM_T322AI)
    {

        for (int i=0; i<22; i++)
        {
            strTemp.Format(_T("%d"),i+1);
            m_T3_Input_List.InsertItem(i,strTemp);
            strTemp=Get_Table_Name(m_sn,_T("Input"),i+1);
            m_T3_Input_List.SetItemText(i,1,strTemp);
        }

        CString strresult;
        int regValue;
        for(int i = 1; i<=22; i++)
        {



            int RangeValue =   product_register_value[225+i-1];
            int RangeValue1 = RangeValue;
            if (i<=11)
            {
                if (RangeValue1 == 6)
                {
                    RangeValue1 =14;
                }
                if (RangeValue1>=0&&RangeValue1<=14)
                {
                    strresult = Range_Units[RangeValue1] ;
                }
            }
            else
            {
                if (RangeValue1>=0&&RangeValue1<=14)
                {
                    strresult = Range_Units[RangeValue1] ;
                }
            }


            m_T3_Input_List.SetItemText(i-1,3,strresult);
              
            if (RangeValue!=6)
            {
                regValue=product_register_value[100+2*i-1];
            }
            else
            {
                regValue=product_register_value[100+2*(i-1)]*65536+product_register_value[100+2*i-1];
            }

            strresult = GetValue(regValue,RangeValue);
            m_T3_Input_List.SetItemText(i-1,2,strresult);

            strresult.Format(_T("%d"),product_register_value[200+i-1]);
            m_T3_Input_List.SetItemText(i-1,4,strresult);

            strresult.Format(_T("%d"),product_register_value[250+i-1]);
            m_T3_Input_List.SetItemText(i-1,5,strresult);
        }
    }
    else
    {
        //AfxMessageBox(_T("This Product is not T3 Modules!")) ;
    }


    return 0;
}


LRESULT CT3ModulesView::Change_Input_Item(WPARAM wParam,LPARAM lParam)
{

    int lRow = (int)wParam;
    int lCol = (int)lParam;
    CString temp_task_info;
    CString New_CString =  m_T3_Input_List.GetItemText(lRow,lCol);
    int Value=_wtoi(New_CString);
    if (product_register_value[7]==PM_T34AO)
    {
        if (lCol == 0)
        {
            Insert_Update_Table_Name(m_sn,_T("Input"),lRow+1,New_CString);
        }
        if (lCol == 1)	  //Value
        {
            int regvalue=product_register_value[200+lRow];
            if (regvalue>13)
            {
                regvalue=0;
            }
            Value=Value*Get_Unit_Process(Range_Units[regvalue]);

            int low,high;

            low=Value%65535;
            high=Value/65535;
            // int ret1=write_one(g_tstat_id,INPUT1_PULSE_COUNT_HIGHT+2*(m_currow-1),high);
            //regValue=product_register_value[119+2*(i-1)]*65535+product_register_value[120+2*(i-1)];
            int ret1=write_one(g_tstat_id,119+2*lRow,high);
            int ret2=write_one(g_tstat_id,120+2*lRow,low);

            if (ret1>0&&ret2>0)
            {
                product_register_value[119+2*lRow]=high;
                product_register_value[120+2*lRow]=low;
                PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
            }
            else
            {
                AfxMessageBox(_T("Try again"));
            }
        }

        if (lCol == 4)	//Filter
        {
            int regvalue=product_register_value[210+lRow];
            if (Value!=regvalue)
            {
                int ret1=write_one(g_tstat_id,210+lRow,Value);
                if (ret1>0)
                {
                    product_register_value[210+lRow]=Value;
                    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                }
                else
                {
                    AfxMessageBox(_T("Try again"));
                }
            }
        }

    }
    if (product_register_value[7]==PM_T322AI)
    {
        if (lCol == 1)
        {
            Insert_Update_Table_Name(m_sn,_T("Input"),lRow+1,New_CString);
        }
        if (lCol == 2)	  //Value
        {
            int RangeValue=(short)product_register_value[225+lRow];
            if (RangeValue>14||RangeValue<0)
            {
                RangeValue=0;
            }
            Value=Value*Get_Unit_Process(Range_Units[RangeValue]);

            int low,high;

            low=Value%65535;
            high=Value/65535;

            // int ret1=write_one(g_tstat_id,INPUT1_PULSE_COUNT_HIGHT+2*(m_currow-1),high);
            //regValue=product_register_value[119+2*(i-1)]*65535+product_register_value[120+2*(i-1)];

            int ret1=write_one(g_tstat_id,100+2*lRow,high);
            int ret2=write_one(g_tstat_id,101+2*lRow,low);

            if (ret1>0&&ret2>0)
            {
                product_register_value[100+2*lRow]=high;
                product_register_value[101+2*lRow]=low;
                PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
            }
            else
            {
                AfxMessageBox(_T("Try again"));
            }
        }

        if (lCol == 4)	//Filter
        {
            int regvalue=product_register_value[200+lRow];
            if (Value!=regvalue)
            {
                int ret1=write_one(g_tstat_id,200+lRow,Value);
                if (ret1>0)
                {
                    product_register_value[200+lRow]=Value;
                    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                }
                else
                {
                    AfxMessageBox(_T("Try again"));
                }
            }
        }

        if (lCol == 5)
        {
            int regvalue=product_register_value[250+lRow];
            if (Value!=regvalue)
            {
                int ret1=write_one(g_tstat_id,250+lRow,Value);
                if (ret1>0)
                {
                    product_register_value[250+lRow]=Value;
                    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                }
                else
                {
                    AfxMessageBox(_T("Try again"));
                }
            }
        }

    }
    else if (product_register_value[7]==PM_T38I13O)
    {

        if (lCol == 0)
        {
            Insert_Update_Table_Name(m_sn,_T("Input"),lRow+1,New_CString);
        }
        if (lCol==1)//Value
        {


            int regvalue=product_register_value[183+lRow];
            if (regvalue>13)
            {
                regvalue=0;
            }
            Value=Value*Get_Unit_Process(Range_Units[regvalue]);

            int low;

            low=Value%65535;
            // int ret1=write_one(g_tstat_id,INPUT1_PULSE_COUNT_HIGHT+2*(m_currow-1),high);
            int ret2=write_one(g_tstat_id,119+2*lRow,low);
            if (ret2>0)
            {
                product_register_value[119+2*lRow]=low;
                PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
            }
            else
            {
                AfxMessageBox(_T("Try again"));
            }


        }
        if (lCol==4)	//Filter
        {
            int regvalue=product_register_value[191+lRow];
            if (Value!=regvalue)
            {
                int ret1=write_one(g_tstat_id,191+lRow,Value);
                if (ret1>0)
                {
                    product_register_value[191+lRow]=Value;
                    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                }
                else
                {
                    AfxMessageBox(_T("Try again"));
                }
            }
        }
        if (lCol==5) //Lighting Time
        {
            int regvalue=product_register_value[199+lRow];
            if (Value!=regvalue)
            {
                int ret1=write_one(g_tstat_id,199+lRow,Value);
                if (ret1>0)
                {
                    product_register_value[199+lRow]=Value;
                    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                }
                else
                {
                    AfxMessageBox(_T("Try again"));
                }
            }
        }
    }
    else if (product_register_value[7]==PM_T36CT)
    {
        if (lCol == 0)
        {
            Insert_Update_Table_Name(m_sn,_T("Input"),lRow+1,New_CString);
        }
    }
    else if (product_register_value[7]==PM_T3IOA)
    {
        if (lCol == 0)
        {
            Insert_Update_Table_Name(m_sn,_T("Input"),lRow+1,New_CString);
        }
        if (lCol == 3)	//Filter
        {
            int regvalue=product_register_value[126+lRow];
            if (Value!=regvalue)
            {
                int ret1=write_one(g_tstat_id,126+lRow,Value);
                if (ret1>0)
                {
                    product_register_value[126+lRow]=Value;
                    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                }
                else
                {
                    AfxMessageBox(_T("Try again"));
                }
            }
        }
    }
    else if (product_register_value[7]==PM_T38AI8AO6DO)
    {
        if (lCol == 0)
        {
            Insert_Update_Table_Name(m_sn,_T("Input"),lRow+1,New_CString);
        }
        if (lCol == 1)	  //Value
        {
            int RangeValue=(short)product_register_value[225+lRow];
            if (RangeValue>14||RangeValue<0)
            {
                RangeValue=0;
            }
            Value=Value*Get_Unit_Process(Range_Units[RangeValue]);

            int ret2=write_one(g_tstat_id,116+2*lRow,Value);
            if (ret2>0)
            {

                product_register_value[116+2*lRow]=Value;
                PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
            }
            else
            {
                AfxMessageBox(_T("Try again"));
            }
        }
        if (lCol == 3)	//Filter
        {
            int regvalue=product_register_value[200+lRow];
            if (Value!=regvalue)
            {
                int ret1=write_one(g_tstat_id,200+lRow,Value);
                if (ret1>0)
                {
                    product_register_value[200+lRow]=Value;
                    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                }
                else
                {
                    AfxMessageBox(_T("Try again"));
                }
            }
        }
    }
    else if (product_register_value[7]==PM_T332AI)
    {
        if (lCol == 0)	//Label
        {
            Insert_Update_Table_Name(m_sn,_T("Input"),lRow+1,New_CString);
        }
        if (lCol == 1)	  //Value
        {
            int regvalue=product_register_value[228+lRow];
            if (regvalue>13)
            {
                regvalue=0;
            }
            Value=Value*Get_Unit_Process(Range_Units[regvalue]);

            int low;

            low=Value%65535;
            // int ret1=write_one(g_tstat_id,INPUT1_PULSE_COUNT_HIGHT+2*(m_currow-1),high);
            int ret2=write_one(g_tstat_id,100+lRow,low);
            if (ret2>0)
            {
                product_register_value[100+lRow]=low;
                PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
            }
            else
            {
                AfxMessageBox(_T("Try again"));
            }
        }
        if (lCol == 3)	//Filter
        {
            int regvalue=product_register_value[260+lRow];
            if (Value!=regvalue)
            {
                int ret1=write_one(g_tstat_id,260+lRow,Value);
                if (ret1>0)
                {
                    product_register_value[260+lRow]=Value;
                    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
                }
                else
                {
                    AfxMessageBox(_T("Try again"));
                }
            }
        }
    }
    else if (product_register_value[7]==PM_T3PT10)
    {
        if (lCol == 1)
        {
            Insert_Update_Table_Name(m_sn,_T("Input"),lRow+1,New_CString);
        }
    }
    else
    {

    }
    return 0;
}

void CT3ModulesView::OnNMClickList_Input(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    CString temp_cstring;
    long lRow,lCol;
    m_T3_Input_List.Set_Edit(false);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_T3_Input_List.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_T3_Input_List.SubItemHitTest(&lvinfo);
    BOOL Is_Range = FALSE;
    int Range_Address = -1;
    int Value_Address = -1;
    int Value_Length = 0;
    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;
    
    if(lRow>m_T3_Input_List.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
        return;
    if(lRow<0)
        return;
    int speedcount = 0;
    m_ThreadStop = TRUE;

   
    if (product_register_value[7]==PM_T34AO)
    {
        if (lCol == 3)
        {
            Is_Range = TRUE;
            Range_Address = 200;
            Value_Address = 119;
            Value_Length = 20;
        }
    }
    else if (product_register_value[7]==PM_T38I13O)
    {
        if (lCol == 3)
        {
            Is_Range = TRUE;
            Range_Address = 183;
            Value_Address = 119;
            Value_Length = 16;
        }

    }
    else if (product_register_value[7]==PM_T36CT)
    {
        if (lCol == 2)
        {
            Is_Range = TRUE;
            Range_Address = 126;
            Value_Address = 108;
            Value_Length = 8;
        }
    }
    else if (product_register_value[7]==PM_T3IOA)
    {
        if (lCol == 2)
        {
            Is_Range = TRUE;
            Range_Address = 118;
            Value_Address = 108;
            Value_Length = 8;
        }
    }
    else if (product_register_value[7]==PM_T38AI8AO6DO)
    {
        if (lCol == 2)
        {
            Is_Range = TRUE;
            Range_Address = 225;
            Value_Address = 116;
            Value_Length = 8;
        }
    }
    else if (product_register_value[7]==PM_T332AI)
    {
        if (lCol == 2)
        {
            Is_Range = TRUE;
            Range_Address = 228;
            Value_Address = 100;
            Value_Length = 32;
        }
    }
    else if (product_register_value[7]==PM_T322AI)
    {
        if (lCol == 3)
        {
            Is_Range = TRUE;
            Range_Address = 225;
            Value_Address = 100;
            Value_Length = 44;
        }
        if (lRow<=10)
        {
            speedcount =1;
        }

    }
    else if (product_register_value[7]==PM_T3PT10)
    {
          m_T3_Input_List.Set_Edit(false);
          return;
    }
    else
    {

    }

    if (Is_Range)
    {

        CT3RangeDlg rangedlg;
        rangedlg.m_speed_count = speedcount;
        rangedlg.m_current_range =product_register_value[Range_Address+lRow];
        if (rangedlg.m_current_range == 6 && speedcount==1)
        {
            rangedlg.m_current_range = 14;
        }
        if (IDOK==rangedlg.DoModal())
        {
            int range=rangedlg.m_current_range;
            if (speedcount == 1&&range==14)
            {
                range = 6;
            }
            if (range>=0&&range<=13)
            {
                int ret=write_one(g_tstat_id,Range_Address+lRow,range);
                if (ret>0)
                {
                    product_register_value[Range_Address+lRow]=range;
                }
                Sleep(500);
                Read_Multi(g_tstat_id,&product_register_value[Value_Address],Value_Address,Value_Length);
                PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
            }
        }
    }
}

void CT3ModulesView::OnDestroy()
{
    if(hFirstThread != NULL)
        TerminateThread(hFirstThread, 0);
    hFirstThread=NULL;

    CFormView::OnDestroy();
}
