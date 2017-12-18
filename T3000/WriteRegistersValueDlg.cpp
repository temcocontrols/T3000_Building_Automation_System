// WriteRegistersValueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "WriteRegistersValueDlg.h"
#include "afxdialogex.h"
#include "ProductRegisterListView.h"
#include "EnterValueDlg.h"
#include "Enter_Value_BinaryDlg.h"
extern CustomProductTable_T g_current_Node;


// CWriteRegistersValueDlg dialog

IMPLEMENT_DYNAMIC(CWriteRegistersValueDlg, CDialogEx)

CWriteRegistersValueDlg::CWriteRegistersValueDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWriteRegistersValueDlg::IDD, pParent)
  
{

    m_string_address = _T("");
    m_string_counts = _T("");
    m_string_dataformats = _T("");
    m_string_functions = _T("");
    m_sting_property = _T("");
    m_string_type = _T("");
    m_bool_single =TRUE;
}

CWriteRegistersValueDlg::~CWriteRegistersValueDlg()
{
}

void CWriteRegistersValueDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_ADDRESS, m_string_address);
    DDX_Text(pDX, IDC_EDIT_COUNTS, m_string_counts);
    DDX_Text(pDX, IDC_EDIT_DATAFORMATS, m_string_dataformats);
    //  DDX_Control(pDX, IDC_EDIT_FUNCTIONS, m_string_functions);
    DDX_Text(pDX, IDC_EDIT_FUNCTIONS, m_string_functions);
    DDX_Text(pDX, IDC_EDIT_PROPERTY, m_sting_property);
    DDX_Text(pDX, IDC_EDIT_TYPE, m_string_type);
    DDX_Control(pDX, IDC_LIST_VALUE_READ, m_list_value_read);
    DDX_Control(pDX, IDC_LIST_VALUE_SET, m_list_value_set);
    //DDX_Check(pDX, IDC_RADIO_SINGLE, m_bool_single);
    //DDX_Check(pDX, IDC_RADIO_MULTIPLE, m_bool_single);
    DDX_Control(pDX, IDC_RADIO_SINGLE, m_button_single);
    DDX_Control(pDX, IDC_RADIO_MULTIPLE, m_button_multi);
}


BEGIN_MESSAGE_MAP(CWriteRegistersValueDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CWriteRegistersValueDlg::OnBnClickedOk)
    ON_LBN_DBLCLK(IDC_LIST_VALUE_SET, &CWriteRegistersValueDlg::OnLbnDblclkListValueSet)
    ON_BN_CLICKED(IDC_RADIO_SINGLE, &CWriteRegistersValueDlg::OnBnClickedRadioSingle)
    ON_BN_CLICKED(IDC_RADIO_MULTIPLE, &CWriteRegistersValueDlg::OnBnClickedRadioMultiple)
    ON_BN_CLICKED(IDCWRITEONE, &CWriteRegistersValueDlg::OnBnClickedCwriteone)
END_MESSAGE_MAP()


void CWriteRegistersValueDlg::OnBnClickedOk()
{
    unsigned short Start_Address = g_current_Node.Reg_ID;
     unsigned short Data_Buffer[100];
     for(int i=0;i<g_current_Node.Counts_Number;i++){
         Data_Buffer[i]=Get_Data_Unsigned(i);
         if (m_bool_single) //�õ�дȥд
         {
            int ret = write_one(g_tstat_id,Start_Address+i,Data_Buffer[i]);
            if (ret<0)
            {
              //  m_list_value_set.col
              m_list_value_set.SetItemData(i,RGB(255,0,0));
            }
         } 
         
     }
     if (!m_bool_single)
      {
            int ret = Write_Multi_org_short(g_tstat_id,Data_Buffer,Start_Address,g_current_Node.Counts_Number,5);
            if (ret<0)
            {
                for(int i=0;i<g_current_Node.Counts_Number;i++){
                    m_list_value_set.SetItemData(i,RGB(255,0,0));
                } 
            }
      }
      int Count_Number =    g_current_Node.Counts_Number;
     int ret= Read_Multi(g_tstat_id,Data_Buffer,Start_Address,Count_Number); 
     CString strValue = _T("");
     CString strTemp;
     m_list_value_read.ResetContent();
     if (m_string_dataformats.CompareNoCase(_T("Signed"))==0)
     { 
         for (int len=0;len<Count_Number;len++)
         {
             strTemp.Format(_T("%d"),(short)Data_Buffer[len]);
             int index = m_list_value_read.GetCount(); 

             m_list_value_read.InsertString(index,strTemp);

         }
     }
     else if (m_string_dataformats.CompareNoCase(_T("Unsigned"))==0)
     { 
         for (int len=0;len<Count_Number;len++)
         {
             strTemp.Format(_T("%d"),(unsigned short)Data_Buffer[len]);
             int index = m_list_value_read.GetCount(); 

             m_list_value_read.InsertString(index,strTemp);

         }
     }
     else if (m_string_dataformats.CompareNoCase(_T("Hex"))==0)
     { 
         for (int len=0;len<Count_Number;len++)
         {
             strTemp.Format(_T("0x%04X"),Data_Buffer[len]);
             int index = m_list_value_read.GetCount(); 

             m_list_value_read.InsertString(index,strTemp);

         }
     } 
     else if (m_string_dataformats.CompareNoCase(_T("Binary"))==0)
     { 
         for (int len=0;len<Count_Number;len++)
         {
             bitset<16> BitRegValue(Data_Buffer[len]);
              
             strTemp = BitRegValue.to_string().c_str();
             int index = m_list_value_read.GetCount(); 

             m_list_value_read.InsertString(index,strTemp);

         }
     } 
     else if (m_string_dataformats.CompareNoCase(_T("Char"))==0)
     { 
         for (int len=0;len<Count_Number;len++)
         {
             
             strTemp.Format(_T("%c%c"),(Data_Buffer[len]&0xFF00)>>8,Data_Buffer[len]&0x00FF);
             
             int index = m_list_value_read.GetCount(); 

             m_list_value_read.InsertString(index,strTemp);  

         }
     }
     else
     {
         for (int len=0;len<Count_Number;len++)
         {
             strTemp.Format(_T("%d"),Data_Buffer[len]);
             int index = m_list_value_read.GetCount(); 

             m_list_value_read.InsertString(index,strTemp); 

         }
     }

}


BOOL CWriteRegistersValueDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

   /* m_string_counts.Format(_T("%d"),g_current_Node.Counts_Number);
    m_string_dataformats.Format(_T("%s"),g_current_Node.DataFormat);
    m_string_address.Format(_T("%d"),g_current_Node.Reg_ID);
    m_string_functions.Format(_T("%s"),g_current_Node.Reg_Description);
    m_sting_property.Format(_T("%s"),g_current_Node.Property);
    m_string_type.Format(_T("%s"),g_current_Node.Para_Type);*/
    CStringArray ValueAarry;
    SplitCStringA(ValueAarry,g_current_Node.Value,_T(","));
    m_list_value_set.ResetContent();
    for (int i=0;i<(int)ValueAarry.GetCount();i++)
    {
       m_list_value_set.AddString(ValueAarry.GetAt(i)); 
    }
    m_list_value_set.SetSel(0);
    
    UpdateData(FALSE);
     SetRadioButton();
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CWriteRegistersValueDlg::OnLbnDblclkListValueSet()
{
    CString itemstr;
    int index=m_list_value_set.GetCurSel();
    if (index<0)
    {
        return;
    }
    if (m_string_dataformats.CompareNoCase(_T("Hex"))==0)
    {

        m_list_value_set.GetText(index,itemstr);
        itemstr.Delete(0,2);  
        CEnterValueDlg dlg;
        dlg.m_string_value = itemstr;

        if (IDOK == dlg.DoModal())
        {  
            itemstr = dlg.m_string_value;
            itemstr.MakeLower();
            int big =  itemstr.Find(_T("0x"));
            if(big<0){
                CString strTemp;
                strTemp.Format(_T("0x%s"),itemstr);
                itemstr = strTemp;
            }
            m_list_value_set.DeleteString(index);
            m_list_value_set.InsertString(index,itemstr);
        }
    }
    else if (m_string_dataformats.CompareNoCase(_T("Binary"))==0)
    {

        m_list_value_set.GetText(index,itemstr); 

        CString strTemp;
        CEnter_Value_BinaryDlg dlg;
        for (int i = 0;i< 16 ;i++)
        {
            strTemp.Format(_T("%c"),itemstr.GetAt(i));
            if (_wtoi(strTemp)==1)
            {
                dlg.m_Binary_Value[i]=TRUE ;
            }
            else
            {
                dlg.m_Binary_Value[i]=FALSE ;
            }

        }
        if (IDOK == dlg.DoModal())
        {  
            itemstr = dlg.m_string_value;


            m_list_value_set.DeleteString(index);
            m_list_value_set.InsertString(index,itemstr);
        }
    }
    else if (m_string_dataformats.CompareNoCase(_T("Char"))==0)
    {

        m_list_value_set.GetText(index,itemstr);  
        CEnterValueDlg dlg;
        dlg.m_string_value = itemstr;

        if (IDOK == dlg.DoModal())
        {  
            itemstr = dlg.m_string_value;
            if (itemstr.GetLength()>2)
            { 
                itemstr.Delete(2,itemstr.GetLength());
            }

            m_list_value_set.DeleteString(index);
            m_list_value_set.InsertString(index,itemstr);
        }
    }
    else{

        m_list_value_set.GetText(index,itemstr);  
        CEnterValueDlg dlg;
        dlg.m_string_value = itemstr;
        if (IDOK == dlg.DoModal())
        {  
            itemstr = dlg.m_string_value;
            m_list_value_set.DeleteString(index);
            m_list_value_set.InsertString(index,itemstr);
        }
    }

}


void CWriteRegistersValueDlg::OnBnClickedRadioSingle()
{
  
   m_bool_single = TRUE;
   SetRadioButton();
}


void CWriteRegistersValueDlg::OnBnClickedRadioMultiple()
{
    m_bool_single = FALSE;
    SetRadioButton();
}
void CWriteRegistersValueDlg::SetRadioButton(){
    if (m_bool_single)
    {
        m_button_single.SetCheck(TRUE);
        m_button_multi.SetCheck(FALSE);
    } 
    else
    {
        m_button_single.SetCheck(FALSE);
        m_button_multi.SetCheck(TRUE);
    }
}
unsigned short CWriteRegistersValueDlg::Get_Data_Unsigned(int index){
    int ret=0;
    CString itemstr;
    m_list_value_set.GetText(index,itemstr);
    
    CString StrValue=itemstr;
     
    if (m_string_dataformats.CompareNoCase(_T("Hex"))==0)
    {
        StrToIntEx(StrValue.GetBuffer(),STIF_SUPPORT_HEX,&ret);
    }
    else if (m_string_dataformats.CompareNoCase(_T("Binary"))==0)
    {
        int US_temp=0;
        for (int i=0;i<16;i++)
        {
            if (StrValue.GetAt(i)=='1')
            {
                US_temp=1<<i;
                ret+=US_temp;
            }
        }
    }
    else if (m_string_dataformats.CompareNoCase(_T("Char"))==0)
    {
            if (StrValue.GetLength()>=2)
            {
                int US_temp=0;
                char charTemp;
                charTemp=StrValue.GetAt(0); 
                US_temp=((short)charTemp<<8);
                ret+=US_temp;
                charTemp=StrValue.GetAt(1); 
                US_temp=(short)charTemp;
                ret+=US_temp;
            }
            else if (StrValue.GetLength()<1)
            {
                int US_temp=0;
                char charTemp;
                charTemp=' '; 
                US_temp=((short)charTemp<<8);
                ret+=US_temp;
                charTemp=' '; 
                US_temp=(short)charTemp;
                ret+=US_temp;
            }
            else{
                int US_temp=0;
                char charTemp;
                charTemp=StrValue.GetAt(0); 
                US_temp=((short)charTemp<<8);
                ret+=US_temp;
                charTemp=' '; 
                US_temp=(short)charTemp;
                ret+=US_temp;
            }
              
             
        
    }
    else
    {
       StrToIntEx(StrValue.GetBuffer(),STIF_DEFAULT,&ret);
    }
    return (unsigned short)ret;
}


void CWriteRegistersValueDlg::OnBnClickedCwriteone()
{
       unsigned short Data_Buffer[10];
       int index = m_list_value_set.GetCurSel();
       unsigned short writedata=Get_Data_Unsigned(index);
      int Start_Address = g_current_Node.Reg_ID;
       int ret = write_one(g_tstat_id,Start_Address+index,writedata);
       ret = -1;
       if (ret<0)
       {
           m_list_value_set.SetItemData(index,RGB(255,0,0));
       }
     
       int Count_Number =    g_current_Node.Counts_Number;
       ret= Read_Multi(g_tstat_id,Data_Buffer,Start_Address,Count_Number); 
       CString strValue = _T("");
       CString strTemp;
       m_list_value_read.ResetContent();
       if (m_string_dataformats.CompareNoCase(_T("Signed"))==0)
       { 
           for (int len=0;len<Count_Number;len++)
           {
               strTemp.Format(_T("%d"),(short)Data_Buffer[len]);
               int index = m_list_value_read.GetCount(); 

               m_list_value_read.InsertString(index,strTemp);

           }
       }
       else if (m_string_dataformats.CompareNoCase(_T("Unsigned"))==0)
       { 
           for (int len=0;len<Count_Number;len++)
           {
               strTemp.Format(_T("%d"),(unsigned short)Data_Buffer[len]);
               int index = m_list_value_read.GetCount(); 

               m_list_value_read.InsertString(index,strTemp);

           }
       }
       else if (m_string_dataformats.CompareNoCase(_T("Hex"))==0)
       { 
           for (int len=0;len<Count_Number;len++)
           {
               strTemp.Format(_T("0x%04X"),Data_Buffer[len]);
               int index = m_list_value_read.GetCount(); 

               m_list_value_read.InsertString(index,strTemp);

           }
       } 
       else if (m_string_dataformats.CompareNoCase(_T("Binary"))==0)
       { 
           for (int len=0;len<Count_Number;len++)
           {
               bitset<16> BitRegValue(Data_Buffer[len]);

               strTemp = BitRegValue.to_string().c_str();
               int index = m_list_value_read.GetCount(); 

               m_list_value_read.InsertString(index,strTemp);

           }
       } 
       else if (m_string_dataformats.CompareNoCase(_T("Char"))==0)
       { 
           for (int len=0;len<Count_Number;len++)
           {

               strTemp.Format(_T("%c%c"),(Data_Buffer[len]&0xFF00)>>8,Data_Buffer[len]&0x00FF);

               int index = m_list_value_read.GetCount(); 

               m_list_value_read.InsertString(index,strTemp);  

           }
       }
       else
       {
           for (int len=0;len<Count_Number;len++)
           {
               strTemp.Format(_T("%d"),Data_Buffer[len]);
               int index = m_list_value_read.GetCount(); 

               m_list_value_read.InsertString(index,strTemp); 

           }
       }
}
