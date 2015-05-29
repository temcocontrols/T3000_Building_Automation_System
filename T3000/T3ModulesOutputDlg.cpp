// T3ModulesOutputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3ModulesOutputDlg.h"
#include "afxdialogex.h"
#include "globle_function.h"

// CT3ModulesOutputDlg dialog

IMPLEMENT_DYNAMIC(CT3ModulesOutputDlg, CDialogEx)

CT3ModulesOutputDlg::CT3ModulesOutputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CT3ModulesOutputDlg::IDD, pParent)
{

}

CT3ModulesOutputDlg::~CT3ModulesOutputDlg()
{

}

void CT3ModulesOutputDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_T3OUTPUTS, m_outputlist);
}


BEGIN_MESSAGE_MAP(CT3ModulesOutputDlg, CDialogEx)

    ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Input_List)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Change_Input_Item)
    ON_NOTIFY(NM_CLICK, IDC_LIST_T3OUTPUTS, &CT3ModulesOutputDlg::OnNMClickList_output)

END_MESSAGE_MAP()


// CT3ModulesOutputDlg message handlers

void CT3ModulesOutputDlg::Fresh(){
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
        m_outputlist.InsertColumn(0, _T("Label"), 180, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_outputlist.InsertColumn(1, _T("Switch Status"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_outputlist.InsertColumn(2, _T("Value"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

        for(int i=1;i<9;i++)
        {
            strTemp.Format(_T("Digit Output%d"),i);
            m_outputlist.InsertItem(i-1,strTemp);
            strTemp.Format(_T("%d"),product_register_value[100+i-1]);
            m_outputlist.SetItemText(i-1,2,strTemp); 
            	
        }
        for(int i=9;i<13;i++)
        {
            strTemp.Format(_T("Analog Output%d"),i);
            m_outputlist.InsertItem(i-1,strTemp);
            strTemp.Format(_T("%d"),product_register_value[108+(i-8)-1]);
             m_outputlist.SetItemText(i-1,2,strTemp); 	
        }

        bitset<16> BitSwitchValue(product_register_value[116]);
        int SwitchValue[12];
        for (int i=0;i<4;i++)
        {
            SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
        }
        bitset<16> BitSwitchValue1(product_register_value[117]);
        for (int i=4;i<8;i++)
        {
            SwitchValue[i]=BitSwitchValue1[2*(i-4)]+BitSwitchValue1[2*(i-4)+1]*2;
        }
        bitset<16> BitSwitchValue2(product_register_value[118]);
        for (int i=8;i<12;i++)
        {
            SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
        }
        for(int i = 1;i<13;i++)
        {  
            m_outputlist.SetItemText(i-1,1,STRING_SWITCH_STATUS[SwitchValue[i-1]]); 
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
        for (int i=0;i<8;i++)
        {
            SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
        }

        for(int i=1;i<9;i++)
        {
            strTemp= Get_Table_Name(m_sn,_T("Output"),i);
            m_outputlist.InsertItem(i,strTemp);

            strTemp.Format(_T("%d"),product_register_value[100+i-1]);
            m_outputlist.SetItemText(i-1,1,strTemp);

            m_outputlist.SetItemText(i-1,2,STRING_SWITCH_STATUS[SwitchValue[i-1]]);	
        }

  }
  else if (product_register_value[7] == PM_T38I13O)
  {
      m_outputlist.ModifyStyle(0,LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
      m_outputlist.SetExtendedStyle(m_outputlist.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
      m_outputlist.InsertColumn(0, _T("Label"), 180, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
      m_outputlist.InsertColumn(1, _T("Output Value"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
      m_outputlist.InsertColumn(2, _T("Light Switch"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
      m_outputlist.InsertColumn(2, _T("Auto/Manual"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
      m_outputlist.InsertColumn(4, _T("Switch Status"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

      for(int i=1;i<14;i++)
      {
          strTemp = Get_Table_Name(m_sn,_T("Output"),i);
          m_outputlist.InsertItem(i-1,strTemp);
      }
      bitset<16> BitSwitchValue(product_register_value[116]);
      int SwitchValue[13];
      for (int i=0;i<8;i++)
      {
          SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
      }

      bitset<16> BitSwitchValue2(product_register_value[117]);
      for (int i=8;i<13;i++)
      {
          SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
      }


      CString CstresultDO;
      for(int i = 1;i<=13;i++)
      {  

          CstresultDO.Format(_T("%d"),product_register_value[100+i-1]);
       
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
      }

  }
  else if (product_register_value[7] == PM_T36CT)
  {
      m_outputlist.ModifyStyle(0,LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
      m_outputlist.SetExtendedStyle(m_outputlist.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
      m_outputlist.InsertColumn(0, _T("Label"), 180, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
      m_outputlist.InsertColumn(2, _T("Switch Status"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
      m_outputlist.InsertColumn(1, _T("Value"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

      for(int i=1;i<6;i++)
      {
          strTemp= Get_Table_Name(m_sn,_T("Output"),i);
          m_outputlist.InsertItem(i,strTemp);
      }
      CString CstresultDO;
      int b0,b1,b;
      for(int i = 1;i<=5;i++)
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
}

LRESULT CT3ModulesOutputDlg::Fresh_Input_List(WPARAM wParam,LPARAM lParam){
    m_sn=product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;	
    CString strTemp;
     int fresh_type = (int)wParam;
     int fresh_row =(int) lParam;
     if (fresh_type == 0)
     {         m_outputlist.DeleteAllItems();
         if (product_register_value[7] == PM_T34AO)
         {
             for(int i=1;i<9;i++)
             {
                 strTemp.Format(_T("Digit Output%d"),i);
                 m_outputlist.InsertItem(i-1,strTemp);
                 strTemp.Format(_T("%d"),product_register_value[100+i-1]);
                 m_outputlist.SetItemText(i-1,2,strTemp); 

             }
             for(int i=9;i<13;i++)
             {
                 strTemp.Format(_T("Analog Output%d"),i);
                 m_outputlist.InsertItem(i-1,strTemp);
                 strTemp.Format(_T("%d"),product_register_value[108+(i-8)-1]);
                 m_outputlist.SetItemText(i-1,2,strTemp); 	
             }

             bitset<16> BitSwitchValue(product_register_value[116]);
             int SwitchValue[12];
             for (int i=0;i<4;i++)
             {
                 SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
             }
             bitset<16> BitSwitchValue1(product_register_value[117]);
             for (int i=4;i<8;i++)
             {
                 SwitchValue[i]=BitSwitchValue1[2*(i-4)]+BitSwitchValue1[2*(i-4)+1]*2;
             }
             bitset<16> BitSwitchValue2(product_register_value[118]);
             for (int i=8;i<12;i++)
             {
                 SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
             }
             for(int i = 1;i<13;i++)
             {  
                 m_outputlist.SetItemText(i-1,1,STRING_SWITCH_STATUS[SwitchValue[i-1]]); 
             }
         } 
         else if (product_register_value[7] == PM_T3IOA)
         {
             bitset<16> BitSwitchValue(product_register_value[116]);
             int SwitchValue[8];
             for (int i=0;i<8;i++)
             {
                 SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
             }

             for(int i=1;i<9;i++)
             {
                 strTemp= Get_Table_Name(m_sn,_T("Output"),i);
                 m_outputlist.InsertItem(i,strTemp);

                 strTemp.Format(_T("%d"),product_register_value[100+i-1]);
                 m_outputlist.SetItemText(i-1,1,strTemp);

                 m_outputlist.SetItemText(i-1,2,STRING_SWITCH_STATUS[SwitchValue[i-1]]);	
             }
         }
         else if (product_register_value[7] == PM_T38I13O)
         {
             for(int i=1;i<14;i++)
             {
                 strTemp = Get_Table_Name(m_sn,_T("Output"),i);
                 m_outputlist.InsertItem(i-1,strTemp);
             }
             bitset<16> BitSwitchValue(product_register_value[116]);
             int SwitchValue[13];
             for (int i=0;i<8;i++)
             {
                 SwitchValue[i]=BitSwitchValue[2*i]+BitSwitchValue[2*i+1]*2;
             }

             bitset<16> BitSwitchValue2(product_register_value[117]);
             for (int i=8;i<13;i++)
             {
                 SwitchValue[i]=BitSwitchValue2[2*(i-8)]+BitSwitchValue2[2*(i-8)+1]*2;
             }


             CString CstresultDO;
             for(int i = 1;i<=13;i++)
             {  

                 CstresultDO.Format(_T("%d"),product_register_value[100+i-1]);

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
             }

         }
         else if (product_register_value[7] == PM_T36CT)
         {
             for(int i=1;i<6;i++)
             {
                 strTemp= Get_Table_Name(m_sn,_T("Output"),i);
                 m_outputlist.InsertItem(i,strTemp);
             }
             CString CstresultDO;
             int b0,b1,b;
             for(int i = 1;i<=5;i++)
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
     }
       
   
      
        
    return 0;
}

LRESULT CT3ModulesOutputDlg::Change_Input_Item(WPARAM wParam,LPARAM lParam){

    int lRow = (int)wParam;
    int lCol = (int)lParam;  
 
    if (product_register_value[7] == PM_T34AO)
    {

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
    return 0;
}
/*

*/           
void CT3ModulesOutputDlg::OnNMClickList_output(NMHDR *pNMHDR, LRESULT *pResult){
    
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    CString temp_cstring;
    long lRow,lCol;
     
}

