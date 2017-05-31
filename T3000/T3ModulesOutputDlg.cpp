// T3ModulesOutputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3ModulesOutputDlg.h"
#include "afxdialogex.h"
#include "global_function.h"

// CT3ModulesOutputDlg dialog

DWORD WINAPI _ReadMultiRegisters_T3_Output(LPVOID pParam)
{
    CT3ModulesOutputDlg* pFrame=(CT3ModulesOutputDlg*)(pParam);
    CString g_strT3000LogString;
    int heatbeat  = 0;
    while(pFrame->IsWindowVisible ())
    {
        if (!is_connect())
        {
            Sleep(1000);
            continue;
        }
        if (pFrame->m_isstop)
        {
          //  
            heatbeat ++;
            Sleep(1000);
            if (heatbeat >10)
            {
                pFrame->m_isstop = FALSE;
                heatbeat =0;
            }
            continue;
        }
        for(int i=0; i<4; i++)
        {
            int multy_ret = 0;
            if (pFrame->m_isstop)
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
    return 0;

    return 0;
}
IMPLEMENT_DYNAMIC(CT3ModulesOutputDlg, CFormView)

CT3ModulesOutputDlg::CT3ModulesOutputDlg()
    : CFormView(CT3ModulesOutputDlg::IDD)
{
    hFirstThread = NULL;
    m_isstop = FALSE;
}

CT3ModulesOutputDlg::~CT3ModulesOutputDlg()
{
    if(hFirstThread != NULL)
        TerminateThread(hFirstThread, 0);
}

void CT3ModulesOutputDlg::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_T3OUTPUTS, m_outputlist);
}
#ifdef _DEBUG
void CT3ModulesOutputDlg::AssertValid() const
{
    CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CT3ModulesOutputDlg::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


BEGIN_MESSAGE_MAP(CT3ModulesOutputDlg, CFormView)

    ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Input_List)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Change_Input_Item)
    ON_NOTIFY(NM_CLICK, IDC_LIST_T3OUTPUTS, &CT3ModulesOutputDlg::OnNMClickList_output)

    ON_NOTIFY(NM_DBLCLK, IDC_LIST_T3OUTPUTS, &CT3ModulesOutputDlg::OnNMDblclkListT3outputs)
END_MESSAGE_MAP()


// CT3ModulesOutputDlg message handlers

void CT3ModulesOutputDlg::Fresh()
{
    CString strTemp;
    g_hwnd_now =  this->m_hWnd;
    m_sn = m_sn=product_register_value[0]+product_register_value[1]*256
                +product_register_value[2]*256*256+product_register_value[3]*256*256*256;
    m_outputlist.ShowWindow(SW_HIDE);
    m_outputlist.DeleteAllItems();
    while(m_outputlist.DeleteColumn(0));
    if (product_register_value[7] == PM_T34AO)
    {
        m_outputlist.ModifyStyle(0,LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_outputlist.SetExtendedStyle(m_outputlist.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_outputlist.InsertColumn(0, _T("Label"), 180, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_outputlist.InsertColumn(1, _T("Switch Status"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_outputlist.InsertColumn(2, _T("Value"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

        for(int i=1; i<9; i++)
        {
            strTemp.Format(_T("Digit Output%d"),i);
            m_outputlist.InsertItem(i-1,strTemp);
            //strTemp.Format(_T("%d"),product_register_value[100+i-1]);
            if (product_register_value[100+i-1] == 0)
            {
                strTemp = _T("Off");
            }
            else
            {
                strTemp = _T("On");
            }
            m_outputlist.SetItemText(i-1,2,strTemp);

        }
        for(int i=9; i<13; i++)
        {
            strTemp.Format(_T("Analog Output%d"),i);
            m_outputlist.InsertItem(i-1,strTemp);
            // strTemp.Format(_T("%d"),product_register_value[108+(i-8)-1]);
            if (product_register_value[108+(i-8)-1] == 0)
            {
                strTemp = _T("Off");
            }
            else
            {
                strTemp = _T("On");
            }
            m_outputlist.SetItemText(i-1,2,strTemp);
        }

        bitset<16> BitSwitchValue(product_register_value[116]);
        int SwitchValue[12];
        for (int i=0; i<4; i++)
        {
            SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
        }
        bitset<16> BitSwitchValue1(product_register_value[117]);
        for (int i=4; i<8; i++)
        {
            SwitchValue[i]=BitSwitchValue1[2*(i-4)]+BitSwitchValue1[2*(i-4)+1]*2;
        }
        bitset<16> BitSwitchValue2(product_register_value[118]);
        for (int i=8; i<12; i++)
        {
            SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
        }
        for(int i = 1; i<13; i++)
        {
            m_outputlist.SetItemText(i-1,1,STRING_SWITCH_STATUS[SwitchValue[i-1]]);

            if (SwitchValue[i-1] == 0)
            {
                m_outputlist.SetItemText(i-1,2,L"Off"); 
            }
            else if (SwitchValue[i-1] == 1)
            {
                m_outputlist.SetItemText(i-1,2,L"On"); 
            }
			else
			{
				if (i<9)
				{
					if (product_register_value[100+i-1] == 0)
					{
						strTemp = _T("Off");
					}
					else
					{
						strTemp = _T("On");
					}
					
				} 
				else
				{
					strTemp.Format(_T("%d"),product_register_value[100+i-1]);
				}

				m_outputlist.SetItemText(i-1,2,strTemp);
			}

        }

    }
    else if (product_register_value[7] == PM_T3IOA)
    {
        m_outputlist.ModifyStyle(0,LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_outputlist.SetExtendedStyle(m_outputlist.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_outputlist.InsertColumn(0, _T("Label"), 180, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_outputlist.InsertColumn(2, _T("Switch Status"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_outputlist.InsertColumn(1, _T("Value"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

        bitset<16> BitSwitchValue(product_register_value[116]);
        int SwitchValue[8];
        for (int i=0; i<8; i++)
        {
            SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
        }

        for(int i=1; i<9; i++)
        {
            strTemp= Get_Table_Name(m_sn,_T("Output"),i);
            m_outputlist.InsertItem(i,strTemp);

            //  strTemp.Format(_T("%d"),product_register_value[100+i-1]);
            if (product_register_value[100+i-1] == 0)
            {
                strTemp = _T("Off");
            }
            else
            {
                strTemp = _T("On");
            }
            m_outputlist.SetItemText(i-1,1,strTemp);

            m_outputlist.SetItemText(i-1,2,STRING_SWITCH_STATUS[SwitchValue[i-1]]);

            if (SwitchValue[i-1] == 0)
            {
                m_outputlist.SetItemText(i-1,1,L"Off"); 
            }
            if (SwitchValue[i-1] == 1)
            {
                m_outputlist.SetItemText(i-1,1,L"On"); 
            }
        }

    }
    else if (product_register_value[7] == PM_T38AI8AO6DO)
    {
        m_outputlist.ModifyStyle(0,LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_outputlist.SetExtendedStyle(m_outputlist.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_outputlist.InsertColumn(0, _T("Label"), 180, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_outputlist.InsertColumn(2, _T("Switch Status"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_outputlist.InsertColumn(1, _T("Value"), 90, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

        bitset<16> BitSwitchValue1(product_register_value[114]);
        bitset<16> BitSwitchValue2(product_register_value[115]);
        int SwitchValue[16];
        for (int i=0; i<8; i++)
        {
            SwitchValue[i]=BitSwitchValue1[2*i]+BitSwitchValue1[2*i+1]*2;
            SwitchValue[8+i]=BitSwitchValue2[2*i]+BitSwitchValue2[2*i+1]*2;
        }

        for(int i=1; i<15; i++)
        {

           

			if (i<9)
			{
				strTemp= Get_Table_Name(m_sn,_T("Analog_Output"),i);
				m_outputlist.InsertItem(i,strTemp);


				if (SwitchValue[i-1] == 0)
				{
					m_outputlist.SetItemText(i-1,1,L"Off"); 
				}
				else if (SwitchValue[i-1] == 1)
				{
					m_outputlist.SetItemText(i-1,1,L"On"); 
				}
				else 
				{
// 					if (product_register_value[100+i-1] == 0)
// 					{
// 						strTemp = _T("Off");
// 					}
// 					else
// 					{
// 						strTemp = _T("On");
// 					}
					strTemp.Format(_T("%d"),product_register_value[100+i-1]);
				}

				m_outputlist.SetItemText(i-1,1,strTemp);

			}
			else
			{
				strTemp= Get_Table_Name(m_sn,_T("Digital_Output"),i);
				m_outputlist.InsertItem(i,strTemp);

				if (SwitchValue[i-1] == 0)
				{
					m_outputlist.SetItemText(i-1,1,L"Off"); 
				}
				else if (SwitchValue[i-1] == 1)
				{
					m_outputlist.SetItemText(i-1,1,L"On"); 
				}
				else 
				{
					if (product_register_value[100+i-1] == 0)
					{
						strTemp = _T("Off");
					}
					else
					{
						strTemp = _T("On");
					}
				}
				
				m_outputlist.SetItemText(i-1,1,strTemp);
			}

		 
			 m_outputlist.SetItemText(i-1,2,STRING_SWITCH_STATUS[SwitchValue[i-1]]);
			
            

        }

    }
    else if (product_register_value[7] == PM_T38I13O)
    {
        m_outputlist.ModifyStyle(0,LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_outputlist.SetExtendedStyle(m_outputlist.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_outputlist.InsertColumn(0, _T("Label"), 180, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_outputlist.InsertColumn(1, _T("Output Value"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_outputlist.InsertColumn(2, _T("Light Switch"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_outputlist.InsertColumn(3, _T("Auto/Manual"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_outputlist.InsertColumn(4, _T("Switch Status"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

        for(int i=1; i<14; i++)
        {
            strTemp = Get_Table_Name(m_sn,_T("Output"),i);
            m_outputlist.InsertItem(i-1,strTemp);
        }
        bitset<16> BitSwitchValue(product_register_value[116]);
        int SwitchValue[13];
        for (int i=0; i<8; i++)
        {
            SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
        }

        bitset<16> BitSwitchValue2(product_register_value[117]);
        for (int i=8; i<13; i++)
        {
            SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
        }


        CString CstresultDO;
        for(int i = 1; i<=13; i++)
        {

            // CstresultDO.Format(_T("%d"),product_register_value[100+i-1]);
            if (product_register_value[100+i-1] == 0)
            {
                CstresultDO = _T("Off");
            }
            else
            {
                CstresultDO = _T("On");
            }
            m_outputlist.SetItemText(i-1,1,CstresultDO);

            if (product_register_value[216+i-1]>0)
            {
                CstresultDO=Get_Table_Name(m_sn,_T("Input"),product_register_value[216+i-1]);
            }
            else
            {
                CstresultDO=_T("UNUSED");
            }
            m_outputlist.SetItemText(i-1,2,CstresultDO);
            if (((product_register_value[215]>>(i-1))&0x01)==1)
            {
                CstresultDO=_T("Manual");
            }
            else
            {
                CstresultDO=_T("Auto");
            }
            m_outputlist.SetItemText(i-1,3,CstresultDO);

            m_outputlist.SetItemText(i-1,4,STRING_SWITCH_STATUS[SwitchValue[i-1]]);



            if (SwitchValue[i-1] == 0)
            {
                m_outputlist.SetItemText(i-1,1,L"Off"); 
            }
            if (SwitchValue[i-1] == 1)
            {
                m_outputlist.SetItemText(i-1,1,L"On"); 
            }
        }

    }
    else if (product_register_value[7] == PM_T36CT)
    {
        m_outputlist.ModifyStyle(0,LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_outputlist.SetExtendedStyle(m_outputlist.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
        m_outputlist.InsertColumn(0, _T("Label"), 180, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_outputlist.InsertColumn(2, _T("Switch Status"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_outputlist.InsertColumn(1, _T("Value"), 90, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

        for(int i=1; i<6; i++)
        {
            strTemp= Get_Table_Name(m_sn,_T("Output"),i);
            m_outputlist.InsertItem(i,strTemp);
        }
        CString CstresultDO;
        int b0,b1,b;
        for(int i = 1; i<=5; i++)
        {

            CstresultDO.Format(_T("%d"),product_register_value[100+i-1]);
            m_outputlist.SetItemText(i-1,2,CstresultDO);
            b0=Get_Bit_FromRegister(product_register_value[124],2*(i-1)+1);
            b1=Get_Bit_FromRegister(product_register_value[124],2*(i-1)+2);
            b=b1*2+b0;

            if (i==5)
            {
                b0=Get_Bit_FromRegister(product_register_value[124],1);
                b1=Get_Bit_FromRegister(product_register_value[124],2);
                b=b1*2+b0;

            }
            if (b==0)
            {
                CstresultDO=_T("OFF");
            }
            else if (b==1)
            {
                CstresultDO=_T("ON");
            }
            else if (b==2)
            {
                CstresultDO=_T("AUTO");
            }
            else
            {
                CstresultDO=_T("");
            }
            m_outputlist.SetItemText(i-1,1,CstresultDO);

        }
    }


    m_outputlist.ShowWindow(SW_SHOW);


    if(hFirstThread != NULL)
        TerminateThread(hFirstThread, 0);
    hFirstThread=NULL;
    if (!hFirstThread)
    {
        hFirstThread = CreateThread(NULL,NULL,_ReadMultiRegisters_T3_Output,this,NULL,0);
    }
}

LRESULT CT3ModulesOutputDlg::Fresh_Input_List(WPARAM wParam,LPARAM lParam)
{
    m_sn=product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;
    CString strTemp;
    int fresh_type = (int)wParam;
    int fresh_row =(int) lParam;
    if (fresh_type == 0)
    {
        m_outputlist.DeleteAllItems();
        if (product_register_value[7] == PM_T34AO)
        {
            for(int i=1; i<9; i++)
            {
                strTemp.Format(_T("Digit Output%d"),i);
                m_outputlist.InsertItem(i-1,strTemp);
                // strTemp.Format(_T("%d"),product_register_value[100+i-1]);
                if (product_register_value[100+i-1] == 0)
                {
                    strTemp = _T("Off");
                }
                else
                {
                    strTemp = _T("On");
                }
                m_outputlist.SetItemText(i-1,2,strTemp);

            }
            for(int i=9; i<13; i++)
            {
                strTemp.Format(_T("Analog Output%d"),i);
                m_outputlist.InsertItem(i-1,strTemp);
                strTemp.Format(_T("%d"),product_register_value[108+(i-8)-1]);
                if (product_register_value[108+(i-8)-1] == 0)
                {
                    strTemp = _T("Off");
                }
                else
                {
                    strTemp = _T("On");
                }
                m_outputlist.SetItemText(i-1,2,strTemp);
            }

            bitset<16> BitSwitchValue(product_register_value[116]);
            int SwitchValue[12];
            for (int i=0; i<4; i++)
            {
                SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
            }
            bitset<16> BitSwitchValue1(product_register_value[117]);
            for (int i=4; i<8; i++)
            {
                SwitchValue[i]=BitSwitchValue1[2*(i-4)]+BitSwitchValue1[2*(i-4)+1]*2;
            }
            bitset<16> BitSwitchValue2(product_register_value[118]);
            for (int i=8; i<12; i++)
            {
                SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
            }
            for(int i = 1; i<13; i++)
            {
                m_outputlist.SetItemText(i-1,1,STRING_SWITCH_STATUS[SwitchValue[i-1]]);
				if (SwitchValue[i-1] == 0)
				{
					m_outputlist.SetItemText(i-1,2,L"Off"); 
				}
				else if (SwitchValue[i-1] == 1)
				{
					m_outputlist.SetItemText(i-1,2,L"On"); 
				}
				else
				{
					if (i<9)
					{
						if (product_register_value[100+i-1] == 0)
						{
							strTemp = _T("Off");
						}
						else
						{
							strTemp = _T("On");
						}

					} 
					else
					{
						strTemp.Format(_T("%d"),product_register_value[100+i-1]);
					}

					m_outputlist.SetItemText(i-1,2,strTemp);
				}
            }
        }
        else if (product_register_value[7] == PM_T3IOA)
        {
            bitset<16> BitSwitchValue(product_register_value[116]);
            int SwitchValue[8];
            for (int i=0; i<8; i++)
            {
                SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
            }

            for(int i=1; i<9; i++)
            {
                strTemp= Get_Table_Name(m_sn,_T("Output"),i);
                m_outputlist.InsertItem(i,strTemp);

                //  strTemp.Format(_T("%d"),product_register_value[100+i-1]);
                if (product_register_value[100+i-1] == 0)
                {
                    strTemp = _T("Off");
                }
                else
                {
                    strTemp = _T("On");
                }
                m_outputlist.SetItemText(i-1,1,strTemp);

                m_outputlist.SetItemText(i-1,2,STRING_SWITCH_STATUS[SwitchValue[i-1]]);
                if (SwitchValue[i-1] == 0)
                {
                    m_outputlist.SetItemText(i-1,1,L"Off"); 
                }
                if (SwitchValue[i-1] == 1)
                {
                    m_outputlist.SetItemText(i-1,1,L"On"); 
                }
            }
        }
        else if (product_register_value[7] == PM_T38I13O)
        {
            for(int i=1; i<14; i++)
            {
                strTemp = Get_Table_Name(m_sn,_T("Output"),i);
                m_outputlist.InsertItem(i-1,strTemp);
            }
            bitset<16> BitSwitchValue(product_register_value[116]);
            int SwitchValue[13];
            for (int i=0; i<8; i++)
            {
                SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
            }

            bitset<16> BitSwitchValue2(product_register_value[117]);
            for (int i=8; i<13; i++)
            {
                SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
            }


            CString CstresultDO;
            for(int i = 1; i<=13; i++)
            {

                //CstresultDO.Format(_T("%d"),product_register_value[100+i-1]);
                if (product_register_value[100+i-1] == 0)
                {
                    strTemp = _T("Off");
                }
                else
                {
                    strTemp = _T("On");
                }
                m_outputlist.SetItemText(i-1,1,strTemp);

                if (product_register_value[216+i-1]>0)
                {
                    CstresultDO=Get_Table_Name(m_sn,_T("Input"),product_register_value[216+i-1]);
                }
                else
                {
                    CstresultDO=_T("UNUSED");
                }
                m_outputlist.SetItemText(i-1,2,CstresultDO);
                if (((product_register_value[215]>>(i-1))&0x01)==1)
                {
                    CstresultDO=_T("Manual");
                }
                else
                {
                    CstresultDO=_T("Auto");
                }
                m_outputlist.SetItemText(i-1,3,CstresultDO);

                m_outputlist.SetItemText(i-1,4,STRING_SWITCH_STATUS[SwitchValue[i-1]]);
                if (SwitchValue[i-1] == 0)
                {
                    m_outputlist.SetItemText(i-1,1,L"Off"); 
                }
                if (SwitchValue[i-1] == 1)
                {
                    m_outputlist.SetItemText(i-1,1,L"On"); 
                }
            }

        }
        else if (product_register_value[7] == PM_T36CT)
        {
            for(int i=1; i<6; i++)
            {
                strTemp= Get_Table_Name(m_sn,_T("Output"),i);
                m_outputlist.InsertItem(i,strTemp);
            }
            CString CstresultDO;
            int b0,b1,b;
            for(int i = 1; i<=5; i++)
            {

                CstresultDO.Format(_T("%d"),product_register_value[100+i-1]);
                m_outputlist.SetItemText(i-1,2,CstresultDO);
                b0=Get_Bit_FromRegister(product_register_value[124],2*(i-1)+1);
                b1=Get_Bit_FromRegister(product_register_value[124],2*(i-1)+2);
                b=b1*2+b0;

                if (i==5)
                {
                    b0=Get_Bit_FromRegister(product_register_value[124],1);
                    b1=Get_Bit_FromRegister(product_register_value[124],2);
                    b=b1*2+b0;

                }
                if (b==0)
                {
                    CstresultDO=_T("OFF");
                }
                else if (b==1)
                {
                    CstresultDO=_T("ON");
                }
                else if (b==2)
                {
                    CstresultDO=_T("AUTO");
                }
                else
                {
                    CstresultDO=_T("");
                }
                m_outputlist.SetItemText(i-1,1,CstresultDO);

            }
        }
        else if (product_register_value[7] == PM_T38AI8AO6DO)
        {
			bitset<16> BitSwitchValue1(product_register_value[114]);
			bitset<16> BitSwitchValue2(product_register_value[115]);
			int SwitchValue[16];
			for (int i=0; i<8; i++)
			{
				SwitchValue[i]=BitSwitchValue1[2*i]+BitSwitchValue1[2*i+1]*2;
				SwitchValue[8+i]=BitSwitchValue2[2*i]+BitSwitchValue2[2*i+1]*2;
			}

			for(int i=1; i<15; i++)
			{

				

				if (i<9)
				{
					strTemp= Get_Table_Name(m_sn,_T("Analog_Output"),i);
					m_outputlist.InsertItem(i,strTemp);


					if (SwitchValue[i-1] == 0)
					{
						m_outputlist.SetItemText(i-1,1,L"Off"); 
					}
					else if (SwitchValue[i-1] == 1)
					{
						m_outputlist.SetItemText(i-1,1,L"On"); 
					}
					else 
					{
						// 					if (product_register_value[100+i-1] == 0)
						// 					{
						// 						strTemp = _T("Off");
						// 					}
						// 					else
						// 					{
						// 						strTemp = _T("On");
						// 					}
						strTemp.Format(_T("%d"),product_register_value[100+i-1]);
					}

					m_outputlist.SetItemText(i-1,1,strTemp);

				}
				else
				{
					strTemp= Get_Table_Name(m_sn,_T("Digital_Output"),i);
					m_outputlist.InsertItem(i,strTemp);

					if (SwitchValue[i-1] == 0)
					{
						m_outputlist.SetItemText(i-1,1,L"Off"); 
					}
					else if (SwitchValue[i-1] == 1)
					{
						m_outputlist.SetItemText(i-1,1,L"On"); 
					}
					else 
					{
						if (product_register_value[100+i-1] == 0)
						{
							strTemp = _T("Off");
						}
						else
						{
							strTemp = _T("On");
						}
					}

					m_outputlist.SetItemText(i-1,1,strTemp);
				}

                m_outputlist.SetItemText(i-1,2,STRING_SWITCH_STATUS[SwitchValue[i-1]]);
            }

        }
    }




    return 0;
}

LRESULT CT3ModulesOutputDlg::Change_Input_Item(WPARAM wParam,LPARAM lParam)
{

    int lRow = (int)wParam;
    int lCol = (int)lParam;
    CString strText = m_outputlist.GetItemText (lRow,lCol);
    if (product_register_value[7] == PM_T34AO)
    {
		if (lCol == 2)
		{
			bitset<16> BitSwitchValue(product_register_value[116]);
			int SwitchValue[12];
			for (int i=0; i<4; i++)
			{
				SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
			}
			bitset<16> BitSwitchValue1(product_register_value[117]);
			for (int i=4; i<8; i++)
			{
				SwitchValue[i]=BitSwitchValue1[2*(i-4)]+BitSwitchValue1[2*(i-4)+1]*2;
			}
			bitset<16> BitSwitchValue2(product_register_value[118]);
			for (int i=8; i<12; i++)
			{
				SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
			}
		}
    }
    else if (product_register_value[7] == PM_T3IOA)
    {

    }
    else if (product_register_value[7] == PM_T38I13O)
    {

    }
    else if (product_register_value[7] == PM_T36CT)
    {

    }
    else if (product_register_value[7] == PM_T38AI8AO6DO)
    {
        if (lCol == 1)
        {
            bitset<16> BitSwitchValue1(product_register_value[114]);
            bitset<16> BitSwitchValue2(product_register_value[115]);
            int SwitchValue[16];
            for (int i=0; i<8; i++)
            {
                SwitchValue[i]=BitSwitchValue1[2*i]+BitSwitchValue1[2*i+1]*2;
                SwitchValue[8+i]=BitSwitchValue2[2*i]+BitSwitchValue2[2*i+1]*2;
            }
            if (SwitchValue[lRow] ==2)
            {
                int OutputValue = _wtoi (strText);
                if (OutputValue!=product_register_value[100+lRow])
                {
					if (lRow<8)
					{
						int ret = write_one (g_tstat_id,100+lRow,OutputValue,1);
						if (ret>0)
						{
							product_register_value[100+lRow] = OutputValue;
						}
					}
                    
                }

                PostMessage (WM_REFRESH_BAC_INPUT_LIST,0,0);
            }
            else
            {
                m_outputlist.Set_Edit (false);
                return 0;
            }


        }

    }
    return 0;
}
/*
   @1:Different Product responce to the different functionality
   @2:
*/
void CT3ModulesOutputDlg::OnNMClickList_output(NMHDR *pNMHDR, LRESULT *pResult)
{

    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    
    CString temp_cstring;
    long lRow,lCol;
    m_outputlist.Set_Edit(true);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_outputlist.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_outputlist.SubItemHitTest(&lvinfo);
    BOOL Is_Range = FALSE;
    int Range_Address = -1;
    int Value_Address = -1;
    int Value_Length = 0;
    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;
	m_isstop = TRUE;
    if(lRow>m_outputlist.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
    {
       
        return;
    }
    if(lRow<0)
    {
        
        return;
    }

    if (product_register_value[7] == PM_T34AO)
    {




        bitset<16> BitSwitchValue(product_register_value[116]);
        int SwitchValue[12];
        for (int i=0; i<4; i++)
        {
            SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
        }
        bitset<16> BitSwitchValue1(product_register_value[117]);
        for (int i=4; i<8; i++)
        {
            SwitchValue[i]=BitSwitchValue1[2*(i-4)]+BitSwitchValue1[2*(i-4)+1]*2;
        }
        bitset<16> BitSwitchValue2(product_register_value[118]);
        for (int i=8; i<12; i++)
        {
            SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
        }

        if (lCol == 2)
        {   // m_isstop =TRUE;
            m_outputlist.Set_Edit (false);
            if (SwitchValue[lRow] == 2)
            {
                if (product_register_value[100+lRow] == 0)
                {
                    int ret = write_one (g_tstat_id,100+lRow,1,1);
                    if (ret > 0)
                    {
                        product_register_value[100+lRow] = 1;
                        m_outputlist.SetItemText (lRow,lCol,L"On");
                    }
                }
                else
                {
                    int ret =  write_one (g_tstat_id,100+lRow,0,1);
                    if (ret > 0)
                    {
                        product_register_value[100+lRow] = 0;
                        m_outputlist.SetItemText (lRow,lCol,L"Off");
                    }
                }
            }
        }
    }
    else if (product_register_value[7] == PM_T3IOA)
    {


        bitset<16> BitSwitchValue(product_register_value[116]);
        int SwitchValue[8];
        for (int i=0; i<8; i++)
        {
            SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
        }


        if (lCol == 1)
        {
            if (SwitchValue[lRow] == 2)
            {
                if (product_register_value[100+lRow] == 0)
                {
                    int ret = write_one (g_tstat_id,100+lRow,1,1);
                    if (ret > 0)
                    {
                        product_register_value[100+lRow] = 1;
                        m_outputlist.SetItemText (lRow,lCol,L"On");
                    }
                }
                else
                {
                    int ret =  write_one (g_tstat_id,100+lRow,0,1);
                    if (ret > 0)
                    {
                        product_register_value[100+lRow] = 0;
                        m_outputlist.SetItemText (lRow,lCol,L"Off");
                    }
                }
            }
        }
    }
    else if (product_register_value[7] == PM_T38AI8AO6DO)
    {


        bitset<16> BitSwitchValue1(product_register_value[114]);
        bitset<16> BitSwitchValue2(product_register_value[115]);
        int SwitchValue[16];
        for (int i=0; i<8; i++)
        {
            SwitchValue[i]=BitSwitchValue1[2*i]+BitSwitchValue1[2*i+1]*2;
            SwitchValue[8+i]=BitSwitchValue2[2*i]+BitSwitchValue2[2*i+1]*2;
        }

        if (lCol == 1)
        {
            if (SwitchValue[lRow] == 2)
            {
				if (lRow>7)
				{
					if (product_register_value[100+lRow] == 0)
					{
						int ret = write_one (g_tstat_id,100+lRow,1,1);
						if (ret > 0)
						{
							product_register_value[100+lRow] = 1;
							m_outputlist.SetItemText (lRow,lCol,L"On");
						}
					}
					else
					{
						int ret =  write_one (g_tstat_id,100+lRow,0,1);
						if (ret > 0)
						{
							product_register_value[100+lRow] = 0;
							m_outputlist.SetItemText (lRow,lCol,L"Off");
						}
					}
				}
            }
        }

    }
    else if (product_register_value[7] == PM_T38I13O)
    {
        bitset<16> BitSwitchValue(product_register_value[116]);
        int SwitchValue[13];
        for (int i=0; i<8; i++)
        {
            SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
        }

        bitset<16> BitSwitchValue2(product_register_value[117]);
        for (int i=8; i<13; i++)
        {
            SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
        }
        if (lCol == 1)
        {
            if (SwitchValue[lRow] == 2)
            {
                if (product_register_value[100+lRow] == 0)
                {
                    int ret = write_one (g_tstat_id,100+lRow,1,1);
                    if (ret > 0)
                    {
                        product_register_value[100+lRow] = 1;
                        m_outputlist.SetItemText (lRow,lCol,L"On");
                    }
                }
                else
                {
                    int ret =  write_one (g_tstat_id,100+lRow,0,1);
                    if (ret > 0)
                    {
                        product_register_value[100+lRow] = 0;
                        m_outputlist.SetItemText (lRow,lCol,L"Off");
                    }
                }
            }
        }

    }
    else if (product_register_value[7] == PM_T36CT)
    {




    }

 
}



void CT3ModulesOutputDlg::OnNMDblclkListT3outputs(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    
    *pResult = 0;
}


BOOL CT3ModulesOutputDlg::PreTranslateMessage(MSG* pMsg)
{
    
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
    {
        CRect list_rect,win_rect;
        m_outputlist.GetWindowRect(list_rect);
        ScreenToClient(&list_rect);
        ::GetWindowRect(this->m_hWnd,win_rect);
        m_outputlist.Set_My_WindowRect(win_rect);
        m_outputlist.Set_My_ListRect(list_rect);

        m_outputlist.Get_clicked_mouse_position();
        return TRUE;
    }
    return CFormView::PreTranslateMessage(pMsg);
}
