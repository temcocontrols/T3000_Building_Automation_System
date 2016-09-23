// ProductRegisterListView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ProductRegisterListView.h"
#include "ado/ADO.h"
#include <bitset>
#include "WriteRegistersValueDlg.h"
   CustomProductTable_T g_current_Node;
// CProductRegisterListView
  UINT _Background_Read(LPVOID pParam){
    CProductRegisterListView* Parent=(CProductRegisterListView*)(pParam);
    while(TRUE){
        if (!Parent->IsWindowVisible())
        {
           g_bPauseMultiRead = FALSE; 
           return 0 ;
        }
        if (!is_connect())
        {
           Sleep(1000);
           continue;
        }
        if (!Parent->m_isReading)
        {
            Sleep(1000);
            continue;
        }
       int ModbusID=Parent->m_current_tree_node.product_id;
            
        for (int i =0;i<(int)Parent->m_register_data_sheet.size();i++)
        {
              PostMessage(g_hwnd_now,WM_REFRESH_BAC_INPUT_LIST,2U,i);
             int Count_Number = Parent->m_register_data_sheet.at(i).Counts_Number;
             unsigned short Start_Address = Parent->m_register_data_sheet.at(i).Reg_ID;
            
             if (Count_Number > 0)
             {            
              unsigned short  DataBuffer[100];
            
                 while (!Parent->m_isReading)
                 {
                     Sleep(500);
                     continue;
                 }
                 Sleep(Parent->m_UINT_delay_items);
                 
                 #if 1
                int ret= Read_Multi(ModbusID,DataBuffer,Start_Address,Count_Number);
                if (ret>0)
                {
                    /*
                    说明：这里是把读到的值转化成字符串，值与值之间用逗号隔开
                    然后把值赋值给结构体中的Value，在发出消息，把值显示出来
                    */ 
                    CString strValue = _T("");
                    CString strTemp;
                    if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("Signed"))==0)
                    { 
						if (Parent->m_value_format == 2)//Raw
						{
							int datavalue = 0 ;
							for (int len=0;len<Count_Number;len++)
							{
								datavalue+=((int)pow((long double)256,Count_Number-len-1))*(short)DataBuffer[len];
							}	
							strValue.Format(_T("%d"),datavalue);

						}
						else if (Parent->m_value_format == 1)
						{
							int datavalue = 0 ;
							for (int len=0;len<Count_Number;len++)
							{
								datavalue+=((int)pow((long double)256,len))*(short)DataBuffer[len];
							}	
							strValue.Format(_T("%d"),datavalue);
						}
						else
						{
							for (int len=0;len<Count_Number;len++)
							{
								strTemp.Format(_T("%d"),(short)DataBuffer[len]);
								strValue+=strTemp;
								if (len+1!=Count_Number)
								{
									strValue+=_T(",");
								} 
							}
						}



                    }
                    else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("Unsigned"))==0)
                    { 
						if (Parent->m_value_format == 2)//Raw
						{
							int datavalue = 0 ;
							for (int len=0;len<Count_Number;len++)
							{
								datavalue+=((int)pow((long double)256,Count_Number-len-1))*DataBuffer[len];
							}	
							strValue.Format(_T("%d"),datavalue);

						}
						else if (Parent->m_value_format == 1)
						{
							int datavalue = 0 ;
							for (int len=0;len<Count_Number;len++)
							{
								datavalue+=((int)pow((long double)256,len))*DataBuffer[len];
							}	
							strValue.Format(_T("%d"),datavalue);
						}
						else
						{
							for (int len=0;len<Count_Number;len++)
							{
								strTemp.Format(_T("%d"),DataBuffer[len]);
								strValue+=strTemp;
								if (len+1!=Count_Number)
								{
									strValue+=_T(",");
								} 
							}
						}

                    }
                    else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("Hex"))==0)
                    { 
                        
						if (Parent->m_value_format == 2)//Raw
						{
							int datavalue = 0 ;
							for (int len=0;len<Count_Number;len++)
							{
								datavalue+=((int)pow((long double)256,Count_Number-len-1))*DataBuffer[len];
							}	
							strValue.Format(_T("0x%X"),datavalue);

						}
						else if (Parent->m_value_format == 1)
						{
							int datavalue = 0 ;
							for (int len=0;len<Count_Number;len++)
							{
								datavalue+=((int)pow((long double)256,len))*DataBuffer[len];
							}	
							strValue.Format(_T("0x%X"),datavalue);
						}
						else
						{
							for (int len=0;len<Count_Number;len++)
							{
								strTemp.Format(_T("0x%04X"),DataBuffer[len]);
								strValue+=strTemp;
								if (len+1!=Count_Number)
								{
									strValue+=_T(",");
								} 
							}
						}
                    } 
                    else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("Binary"))==0)
                    { 
                        for (int len=0;len<Count_Number;len++)
                        {
                            bitset<16> BitRegValue(DataBuffer[len]);
                           // strTemp.Format(_T("%d"),DataBuffer[len]);
                          //  strTemp.Format(_T("%s"),BitRegValue.to_string().c_str());
                            strTemp = BitRegValue.to_string().c_str();
                            strValue+=strTemp;
                            if (len+1!=Count_Number)
                            {
                                strValue+=_T(",");
                            } 

                        }
                    } 
                    else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("Char"))==0)
                    { 
                        for (int len=0;len<Count_Number;len++)
                        {
                            strTemp.Format(_T("%c"),(DataBuffer[len]&0xFF00)>>8);
                            strValue+=strTemp;
                            strTemp.Format(_T("%c"),DataBuffer[len]&0x00FF);
                            strValue+=strTemp;
                             if (len+1!=Count_Number)
                             {
                                 strValue+=_T(",");
                             } 

                        }
                    }
                    else
                    {
                        for (int len=0;len<Count_Number;len++)
                        {
                            strTemp.Format(_T("%d"),DataBuffer[len]);
                            strValue+=strTemp;
                            if (len+1!=Count_Number)
                            {
                                strValue+=_T(",");
                            } 

                        }
                    }
                    Parent->m_register_data_sheet.at(i).Value = strValue;
                     PostMessage(g_hwnd_now,WM_REFRESH_BAC_INPUT_LIST,1U,i);  
                }
                #endif
              
             }
             
        }
      
        Sleep(Parent->m_UINT_delay_loop);
    }
    
  }
IMPLEMENT_DYNCREATE(CProductRegisterListView, CFormView)

CProductRegisterListView::CProductRegisterListView()
	: CFormView(CProductRegisterListView::IDD)
    , m_UINT_delay_loop(2000)
    , m_UINT_delay_items(500)
{
   
    m_BackgroundTreadHandle = NULL;
    m_string_dataformat = _T("Signed");
    m_string_property = _T("Read Only");
    m_short_counts = 1;
    m_string_paratypes = _T("");
    m_sort_type= 0;//默认排序
	m_value_format = 0;
}

CProductRegisterListView::~CProductRegisterListView()
{
}

void CProductRegisterListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CUSTOM_LIST, m_register_list);
	DDX_Control(pDX, IDC_COMBO_DATAFORMAT, m_combox_dataformat);
	DDX_Control(pDX, IDC_COMBO_PROPERTY, m_combox_property);
	DDX_Control(pDX, IDC_EDIT_COUNTS, m_edit_counts);
	DDX_CBString(pDX, IDC_COMBO_DATAFORMAT, m_string_dataformat);
	DDX_CBString(pDX, IDC_COMBO_PROPERTY, m_string_property);
	DDX_Text(pDX, IDC_EDIT_COUNTS, m_short_counts);
	DDX_Control(pDX, IDC_COMBO_PARA_TYPES, m_combox_paratypes);
	DDX_CBString(pDX, IDC_COMBO_PARA_TYPES, m_string_paratypes);
	DDX_Control(pDX,IDC_UPBUTTON , m_upButton);
	DDX_Control(pDX,IDC_DOWNBUTTON , m_downButton);
	DDX_Text(pDX, IDC_EDIT_DELAY_LOOP, m_UINT_delay_loop);
	DDX_Text(pDX, IDC_EDIT_DELAY_ITEMS, m_UINT_delay_items);
	DDX_Control(pDX, IDC_COMBO_DATA_FORMAT, m_combox_valueformat);
}

BEGIN_MESSAGE_MAP(CProductRegisterListView, CFormView)
    ON_WM_SIZE()
    ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Input_List)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Change_Input_Item)
    ON_NOTIFY(NM_CLICK, IDC_LIST_CUSTOM_LIST, &CProductRegisterListView::OnNMClickList_output)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_READ_DEVICE, &CProductRegisterListView::OnBnClickedReadDevice)
    ON_BN_CLICKED(IDC_BUTTON4, &CProductRegisterListView::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_DOWNBUTTON, &CProductRegisterListView::OnBnClickedDownbutton)
    ON_BN_CLICKED(IDC_UPBUTTON, &CProductRegisterListView::OnBnClickedUpbutton)
    ON_WM_TIMER()
    ON_EN_CHANGE(IDC_EDIT_DELAY_LOOP, &CProductRegisterListView::OnEnChangeEditDelayLoop)
    ON_EN_CHANGE(IDC_EDIT_DELAY_ITEMS, &CProductRegisterListView::OnEnChangeEditDelayItems)
	ON_CBN_SELCHANGE(IDC_COMBO_DATA_FORMAT, &CProductRegisterListView::OnCbnSelchangeComboDataFormat)
END_MESSAGE_MAP()


// CProductRegisterListView diagnostics

#ifdef _DEBUG
void CProductRegisterListView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CProductRegisterListView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

 
void CProductRegisterListView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);
    if (nType==SIZE_RESTORED)
    {

        CRect ViewRect;
        GetClientRect(&ViewRect);
        //TRACE(_T(" View:T=%d,B=%d,L=%d,R=%d\n"),ViewRect.top,ViewRect.bottom,ViewRect.left,ViewRect.right);
        // m_MsDataGrid.SetWindowPos(this,ViewRect.top,ViewRect.left,ViewRect.Width(),ViewRect.Height(),SWP_SHOWWINDOW|SWP_NOZORDER);
        if (m_register_list.GetSafeHwnd())
        {
            m_register_list.MoveWindow(CRect(0,50,ViewRect.Width(),ViewRect.Height()),TRUE);
        }

    }
    
}


void CProductRegisterListView::Fresh(void)
{
    close_com();
    g_bPauseMultiRead = TRUE;
    m_upButton.SetImage(IDB_UPBMP);
    m_downButton.SetImage(IDB_DOWNBMP);
    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    m_current_tree_node = pFrame->m_current_tree_node; 
    m_isReading=pFrame->ConnectDevice(m_current_tree_node);
    if (m_isReading)
    {
        GetDlgItem(IDC_READ_DEVICE)->SetWindowText(_T("Stop Read"));
    } 
    else
    {
        GetDlgItem(IDC_READ_DEVICE)->SetWindowText(_T("Start Read"));
    }
	m_combox_valueformat.ResetContent();
	m_combox_valueformat.AddString(L"Raw Data");
	m_combox_valueformat.AddString(L"ASC");
	m_combox_valueformat.AddString(L"DEC");
	m_combox_valueformat.SetCurSel(0);
    LoadDataSheet();
    Initial_List();
    SetTimer(1,10,NULL);
    if (m_BackgroundTreadHandle) 
    {
            BOOL bRet = TerminateThread(m_BackgroundTreadHandle->m_hThread,0);   
            m_BackgroundTreadHandle=NULL;
    }
    m_BackgroundTreadHandle = AfxBeginThread(_Background_Read,this);  
}


void CProductRegisterListView::Initial_List(void)
{
    g_hwnd_now =  this->m_hWnd;
    CString strTemp;
    m_register_list.ShowWindow(SW_HIDE);
    m_register_list.DeleteAllItems();
    while(m_register_list.DeleteColumn(0));
    m_register_list.ModifyStyle(0,LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    m_register_list.SetExtendedStyle(m_register_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
    m_register_list.InsertColumn(0, _T("Parameters Address"), 180, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    m_register_list.InsertColumn(1, _T("Parameters Type"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_register_list.InsertColumn(2, _T("Counts Numbers"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_register_list.InsertColumn(3, _T("Function"), 230, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_register_list.InsertColumn(4, _T("Property"), 90, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_register_list.InsertColumn(5, _T("Value"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_register_list.InsertColumn(6, _T("Data Format"), 90, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    for (int i = 0;i<(int)m_register_data_sheet.size();i++)
    {   
        strTemp.Format(_T("%d"),m_register_data_sheet.at(i).Reg_ID);
        m_register_list.InsertItem(i,strTemp);
        m_register_list.SetItemText(i,1,m_register_data_sheet.at(i).Para_Type);
        strTemp.Format(_T("%d"),m_register_data_sheet.at(i).Counts_Number);
        m_register_list.SetItemText(i,2, strTemp);
        strTemp.Format(_T("%s"),m_register_data_sheet.at(i).Reg_Description);
        m_register_list.SetItemText(i,3, strTemp);
        strTemp.Format(_T("%s"),m_register_data_sheet.at(i).Property);
        m_register_list.SetItemText(i,4, strTemp);
        strTemp.Format(_T("%s"),m_register_data_sheet.at(i).Value);
        m_register_list.SetItemText(i,5, strTemp);
        strTemp.Format(_T("%s"),m_register_data_sheet.at(i).DataFormat);
        m_register_list.SetItemText(i,6, strTemp);
        if(ListCtrlEx::ComboBox == m_register_list.GetColumnType(4))
        {
            ListCtrlEx::CStrList strlist;

            strlist.push_back(_T("Read Only"));
            strlist.push_back(_T("R/W"));
            m_register_list.SetCellStringList(i, 4, strlist);		
        }  //Signed;Unsigned;Hex;Binary
        if(ListCtrlEx::ComboBox == m_register_list.GetColumnType(6))
        {
            ListCtrlEx::CStrList strlist;

            strlist.push_back(_T("Signed"));
            strlist.push_back(_T("Unsigned"));
            strlist.push_back(_T("Hex"));
            strlist.push_back(_T("Binary"));
            strlist.push_back(_T("Char"));
            m_register_list.SetCellStringList(i, 6, strlist);		
        }
    }
    m_register_list.InsertItem(m_register_data_sheet.size(),_T(""));
    if(ListCtrlEx::ComboBox == m_register_list.GetColumnType(4))
    {
        ListCtrlEx::CStrList strlist;
        strlist.push_back(_T("Read Only"));
        strlist.push_back(_T("R/W"));
        m_register_list.SetCellStringList(m_register_data_sheet.size(), 4, strlist);		
    }
    if(ListCtrlEx::ComboBox == m_register_list.GetColumnType(6))
    {
        ListCtrlEx::CStrList strlist;

        strlist.push_back(_T("Signed"));
        strlist.push_back(_T("Unsigned"));
        strlist.push_back(_T("Hex"));
        strlist.push_back(_T("Binary"));
        strlist.push_back(_T("Char"));
        m_register_list.SetCellStringList(m_register_data_sheet.size(), 6, strlist);		
    }
    m_register_list.ShowWindow(SW_SHOW);
}

void CProductRegisterListView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();  
}

void CProductRegisterListView::LoadDataSheet(){
    CString StrSql;
    _variant_t temp_variant ;
   int ProductModel = m_current_tree_node.product_class_id;
   CustomProductTable_T Struc_Temp;
   CADO ado;
   ado.OnInitADOConn(); 
   if (m_sort_type == 0)
   {  
    StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  "),ProductModel);
   }
   else if (m_sort_type == 1)
   {
       StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  ORDER BY Reg_ID DESC"),ProductModel);
   }
   else if(m_sort_type == 2){
    StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  ORDER BY Reg_ID ASC"),ProductModel);
   }
   else
   {
    StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  "),ProductModel);
   }
                                                      // 
  
    m_register_data_sheet.clear();
   ado.m_pRecordset = ado.OpenRecordset(StrSql);
   
   while(VARIANT_FALSE==ado.m_pRecordset->EndOfFile)
   {
       temp_variant=ado.m_pRecordset->GetCollect("ModelNo");//
       if(temp_variant.vt!=VT_NULL)
           Struc_Temp.ModelNo=temp_variant;
       else
           Struc_Temp.ModelNo=-1;

       temp_variant=ado.m_pRecordset->GetCollect("Reg_Description");//
       if(temp_variant.vt!=VT_NULL)
           Struc_Temp.Reg_Description=temp_variant;
       else
           Struc_Temp.Reg_Description=_T("");

       temp_variant=ado.m_pRecordset->GetCollect("Reg_ID");//
       if(temp_variant.vt!=VT_NULL)
           Struc_Temp.Reg_ID=temp_variant;
       else
           Struc_Temp.Reg_ID=-1;

       temp_variant=ado.m_pRecordset->GetCollect("Para_Type");//
       if(temp_variant.vt!=VT_NULL)
           Struc_Temp.Para_Type=temp_variant;
       else
           Struc_Temp.Para_Type=_T("");

       temp_variant=ado.m_pRecordset->GetCollect("Counts_Number");//
       if(temp_variant.vt!=VT_NULL)
           Struc_Temp.Counts_Number=temp_variant;
       else
           Struc_Temp.Counts_Number=-1;

       temp_variant=ado.m_pRecordset->GetCollect("Property");//
       if(temp_variant.vt!=VT_NULL)
           Struc_Temp.Property=temp_variant;
       else
           Struc_Temp.Property=_T("");
            
       temp_variant=ado.m_pRecordset->GetCollect("DataFormat");//
       if(temp_variant.vt!=VT_NULL)
           Struc_Temp.DataFormat=temp_variant;
       else
           Struc_Temp.DataFormat=_T("Unsigned");
                
           m_register_data_sheet.push_back(Struc_Temp);
          
           ado.m_pRecordset->MoveNext();
   }

   ado.CloseRecordset();
   ado.CloseConn();
}
LRESULT CProductRegisterListView::Fresh_Input_List(WPARAM wParam,LPARAM lParam){
  	
        CString strTemp;
     int fresh_type = (int)wParam;
     int fresh_row =(int) lParam;
     if (fresh_type == 0)
     {
       LoadDataSheet();
       g_hwnd_now =  this->m_hWnd;
       CString strTemp;
      
       m_register_list.DeleteAllItems();
      
        
       for (int i = 0;i<(int)m_register_data_sheet.size();i++)
       {   
           strTemp.Format(_T("%d"),m_register_data_sheet.at(i).Reg_ID);
           m_register_list.InsertItem(i,strTemp);
           m_register_list.SetItemText(i,1,m_register_data_sheet.at(i).Para_Type);
           strTemp.Format(_T("%d"),m_register_data_sheet.at(i).Counts_Number);
           m_register_list.SetItemText(i,2, strTemp);
           strTemp.Format(_T("%s"),m_register_data_sheet.at(i).Reg_Description);
           m_register_list.SetItemText(i,3, strTemp);
           strTemp.Format(_T("%s"),m_register_data_sheet.at(i).Property);
           m_register_list.SetItemText(i,4, strTemp);
           strTemp.Format(_T("%s"),m_register_data_sheet.at(i).Value);
           m_register_list.SetItemText(i,5, strTemp);
           strTemp.Format(_T("%s"),m_register_data_sheet.at(i).DataFormat);
           m_register_list.SetItemText(i,6, strTemp);
           if(ListCtrlEx::ComboBox == m_register_list.GetColumnType(4))
           {
               ListCtrlEx::CStrList strlist;

               strlist.push_back(_T("Read Only"));
               strlist.push_back(_T("R/W"));
               m_register_list.SetCellStringList(i, 4, strlist);		
           }
           if(ListCtrlEx::ComboBox == m_register_list.GetColumnType(6))
           {
               ListCtrlEx::CStrList strlist;

               strlist.push_back(_T("Signed"));
               strlist.push_back(_T("Unsigned"));
               strlist.push_back(_T("Hex"));
               strlist.push_back(_T("Binary"));
               strlist.push_back(_T("Char"));
               m_register_list.SetCellStringList(i, 6, strlist);		
           }
       }

        m_register_list.InsertItem(m_register_data_sheet.size(),_T("")); 
        if(ListCtrlEx::ComboBox == m_register_list.GetColumnType(4))
        {
            ListCtrlEx::CStrList strlist;
            strlist.push_back(_T("Read Only"));
            strlist.push_back(_T("R/W"));
            m_register_list.SetCellStringList(m_register_data_sheet.size(), 4, strlist);		
        } 
        if(ListCtrlEx::ComboBox == m_register_list.GetColumnType(6))
        {
            ListCtrlEx::CStrList strlist;

            strlist.push_back(_T("Signed"));
            strlist.push_back(_T("Unsigned"));
            strlist.push_back(_T("Hex"));
            strlist.push_back(_T("Binary"));
            strlist.push_back(_T("Char"));
            m_register_list.SetCellStringList(m_register_data_sheet.size(), 6, strlist);		
        }      
     }
     if (fresh_type == 1)
     {
         strTemp.Format(_T("%d"),m_register_data_sheet.at(fresh_row).Reg_ID);
         m_register_list.SetItemText(fresh_row,0,strTemp);
         m_register_list.SetItemText(fresh_row,1,m_register_data_sheet.at(fresh_row).Para_Type);
         strTemp.Format(_T("%d"),m_register_data_sheet.at(fresh_row).Counts_Number);
         m_register_list.SetItemText(fresh_row,2, strTemp);
         strTemp.Format(_T("%s"),m_register_data_sheet.at(fresh_row).Reg_Description);
         m_register_list.SetItemText(fresh_row,3, strTemp);
         strTemp.Format(_T("%s"),m_register_data_sheet.at(fresh_row).Property);
         m_register_list.SetItemText(fresh_row,4, strTemp);
         strTemp.Format(_T("%s"),m_register_data_sheet.at(fresh_row).Value);
         m_register_list.SetItemText(fresh_row,5, strTemp);
         strTemp.Format(_T("%s"),m_register_data_sheet.at(fresh_row).DataFormat);
         m_register_list.SetItemText(fresh_row,6, strTemp);
         m_register_list.SetItemBkColor(fresh_row,-1,RGB(255,255,255));
     }
     if (fresh_type == 2)
     {
       // m_register_list.SetItemTextColor(fresh_row,-1,RGB(0,0,0));
        m_register_list.SetItemBkColor(fresh_row,-1,RGB(50,50,180));
     }
     if (fresh_type == 3)
     {
       m_register_list.SetItemBkColor(fresh_row,-1,RGB(0,0,0));
     }
       
   
      
        
    return 0;
}

LRESULT CProductRegisterListView::Change_Input_Item(WPARAM wParam,LPARAM lParam){

    UpdateData(TRUE);
    int lRow = (int)wParam;
    int lCol = (int)lParam; 
    CString StrSql;
    BOOL is_last =   lRow == m_register_data_sheet.size() ;
    CString Reg_ID = m_register_list.GetItemText(lRow,0);
    CString Para_Type = m_register_list.GetItemText(lRow,1);
    CString Counts_Number = m_register_list.GetItemText(lRow,2);
    CString Reg_Description = m_register_list.GetItemText(lRow,3);
    CString Property = m_register_list.GetItemText(lRow,4);
    CString StrDataFormat = m_register_list.GetItemText(lRow,6);
    int ProductModel = m_current_tree_node.product_class_id;
    int Int_Counts_Number=_wtoi(Counts_Number);
    if (Int_Counts_Number>100)
    {   
        
         AfxMessageBox(_T("Counts Number should be less than 100"));
         StrSql.Format(_T("%d"),m_register_data_sheet.at(lRow).Counts_Number);
         m_register_list.SetItemText(lRow,lCol,StrSql);
        return 0;
    }
    CustomProductTable_T Struct_Temp;
  /*  if (lCol == 0)
    {*/
    CADO ado;
    ado.OnInitADOConn();
       if (is_last)
       {
          if (Reg_ID.IsEmpty())//最后一行，发现Address 是空的，就给出提示，确保Address必须存在，而且不能重复
          {
            AfxMessageBox(_T("Parameters Address cann't be null"));
            m_register_list.SetItemText(lRow,lCol,_T(""));
            return 0;
          }
          
         if (lCol == 0){
            
             StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d and Reg_ID = %d "),ProductModel,_wtoi(Reg_ID));
             ado.m_pRecordset = ado.OpenRecordset(StrSql);
             if(VARIANT_FALSE==ado.m_pRecordset->EndOfFile){
                 CString strTips;
                 strTips.Format(_T("Address=%s,Exists!"),Reg_ID);
                 AfxMessageBox(strTips);
                 ado.CloseRecordset();
                 ado.CloseConn();
                 m_register_list.SetItemText(lRow,lCol,_T(""));
                 return 0;
             }
             ado.CloseRecordset();
             StrSql.Format(_T("Insert INTO CustomProductTable(ModelNo,Reg_ID,Reg_Description,Para_Type,Property,Counts_Number,DataFormat) VALUES (%d,%d,'','','%s',%d,'%s')")
                                ,ProductModel,_wtoi(Reg_ID),m_string_property,m_short_counts,m_string_dataformat);
             ado.m_pConnection->Execute(StrSql.GetString(),NULL,adCmdText);
             
              Struct_Temp.ModelNo =  ProductModel;
              Struct_Temp.Reg_ID =  _wtoi(Reg_ID);
              Struct_Temp.Property=m_string_property;
              Struct_Temp.Counts_Number = m_short_counts;
              Struct_Temp.DataFormat = m_string_dataformat;
              m_register_data_sheet.push_back(Struct_Temp);

            m_register_list.InsertItem(m_register_data_sheet.size(),_T("")); //插入一行
            if(ListCtrlEx::ComboBox == m_register_list.GetColumnType(4))
            {
                ListCtrlEx::CStrList strlist;
                strlist.push_back(_T("Read Only"));
                strlist.push_back(_T("R/W"));
                m_register_list.SetCellStringList(m_register_data_sheet.size(), 4, strlist);		
            }
            if(ListCtrlEx::ComboBox == m_register_list.GetColumnType(6))
            {
                ListCtrlEx::CStrList strlist;

                strlist.push_back(_T("Signed"));
                strlist.push_back(_T("Unsigned"));
                strlist.push_back(_T("Hex"));
                strlist.push_back(_T("Binary"));
                strlist.push_back(_T("Char"));
                m_register_list.SetCellStringList(m_register_data_sheet.size(), 6, strlist);		
            }
             PostMessage(WM_REFRESH_BAC_INPUT_LIST,1U,lRow) ;
         }
          
       } 
       else
       {
            BOOL m_ischange = TRUE;
            if (lCol == 0)
            {
                if (Reg_ID.IsEmpty())
                {
                     StrSql.Format(_T("Delete * From CustomProductTable Where ModelNo = %d and Reg_ID = %d"), ProductModel,m_register_data_sheet.at(lRow).Reg_ID); 
                     m_ischange =FALSE;   
                }
                else
                {
                    StrSql.Format(_T("Update  CustomProductTable  Set Reg_ID = %d Where ModelNo = %d and Reg_ID = %d "),
                         _wtoi(Reg_ID),m_register_data_sheet.at(lRow).ModelNo,m_register_data_sheet.at(lRow).Reg_ID);
                }
                
            }
            else
            {
                StrSql.Format(_T("Update  CustomProductTable  Set Para_Type = '%s' ,Counts_Number = %d ,Property = '%s' ,Reg_Description = '%s',DataFormat='%s'  Where ModelNo = %d and Reg_ID = %d"),
                    Para_Type,_wtoi(Counts_Number),Property,Reg_Description,StrDataFormat,ProductModel,_wtoi(Reg_ID));
            }
             ado.m_pConnection->Execute(StrSql.GetString(),NULL,adCmdText);
             if (m_ischange)
             {
                m_register_data_sheet.at(lRow).Para_Type = Para_Type ;
                m_register_data_sheet.at(lRow).Counts_Number = _wtoi(Counts_Number) ;
                m_register_data_sheet.at(lRow).Property =   Property;
                m_register_data_sheet.at(lRow).Reg_Description = Reg_Description;
                m_register_data_sheet.at(lRow).Reg_ID =  _wtoi(Reg_ID);
                m_register_data_sheet.at(lRow).DataFormat = StrDataFormat;
                PostMessage(WM_REFRESH_BAC_INPUT_LIST,1U,lRow) ;
             } 
             else
             {
                PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0); 
             }
                    
       }
       ado.CloseConn();
       //LoadDataSheet();
       
   
    return 0;
}
/*
   @1:Different Product responce to the different functionality
   @2:
*/           
void CProductRegisterListView::OnNMClickList_output(NMHDR *pNMHDR, LRESULT *pResult){

    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    CString temp_cstring;
    long lRow,lCol;
    m_register_list.Set_Edit(true);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_register_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_register_list.SubItemHitTest(&lvinfo);
    BOOL Is_Range = FALSE;
    int Range_Address = -1;
    int Value_Address = -1;
    int Value_Length = 0;
    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;

    if(lRow>m_register_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
        return;
    if(lRow<0)
        return; 

    if (lCol == 5)
    {  
        g_current_Node = m_register_data_sheet.at(lRow);
        if (g_current_Node.Property.CompareNoCase(_T("Read Only"))!=0)
        {
            m_isReading = FALSE;
             
            CWriteRegistersValueDlg dlg;
            dlg.DoModal();
            m_isReading = TRUE; 
        }
        
    }
    
}

void CProductRegisterListView::OnDestroy()
{
    g_bPauseMultiRead = FALSE;
    if (m_BackgroundTreadHandle) 
    {
        if (WaitForSingleObject(m_BackgroundTreadHandle->m_hThread, 3000) == WAIT_OBJECT_0)
        {

        }
        else
        {		
            BOOL bRet = TerminateThread(m_BackgroundTreadHandle->m_hThread,0);
            //delete m_pFreshMultiRegisters;
            m_BackgroundTreadHandle=NULL;
        }

    }
    close_com();
    CFormView::OnDestroy();

    // TODO: Add your message handler code here
}


void CProductRegisterListView::OnBnClickedReadDevice()
{
  
    if (m_isReading)
    {
        
        m_isReading = FALSE;
         GetDlgItem(IDC_READ_DEVICE)->SetWindowText(_T("Start Read"));
    } 
    else
    {
       m_isReading = TRUE;
       GetDlgItem(IDC_READ_DEVICE)->SetWindowText(_T("Stop Read"));
    } 
}


void CProductRegisterListView::OnBnClickedButton4()
{
    if (m_sort_type != 0)    //防止已经是默认排序了，还狂点默认排序
    {
     m_sort_type = 0;
     SendMessage(WM_REFRESH_BAC_INPUT_LIST,0,0); 
     if (m_BackgroundTreadHandle) 
     {
         BOOL bRet = TerminateThread(m_BackgroundTreadHandle->m_hThread,0);   
         m_BackgroundTreadHandle=NULL;
     }
     m_BackgroundTreadHandle = AfxBeginThread(_Background_Read,this);
    }
    
    
}


void CProductRegisterListView::OnBnClickedDownbutton()
{
  
    if (m_sort_type != 1)
    {
        m_sort_type = 1;
        SendMessage(WM_REFRESH_BAC_INPUT_LIST,0,0); 
        if (m_BackgroundTreadHandle) 
        {
            BOOL bRet = TerminateThread(m_BackgroundTreadHandle->m_hThread,0);   
            m_BackgroundTreadHandle=NULL;
        }
        m_BackgroundTreadHandle = AfxBeginThread(_Background_Read,this);
    }

}


void CProductRegisterListView::OnBnClickedUpbutton()
{
   
    if (m_sort_type != 2)
    {
        m_sort_type = 2;
        SendMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
        if (m_BackgroundTreadHandle) 
        {
            BOOL bRet = TerminateThread(m_BackgroundTreadHandle->m_hThread,0);   
            m_BackgroundTreadHandle=NULL;
        }
        m_BackgroundTreadHandle = AfxBeginThread(_Background_Read,this); 
    }
}


void CProductRegisterListView::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
      if (nIDEvent == 1)
      {
        //UpdateData();
      }
    CFormView::OnTimer(nIDEvent);
}


void CProductRegisterListView::OnEnChangeEditDelayLoop()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CFormView::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
       UpdateData();
    // TODO:  Add your control notification handler code here
}


void CProductRegisterListView::OnEnChangeEditDelayItems()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CFormView::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
      UpdateData();
    // TODO:  Add your control notification handler code here
}


void CProductRegisterListView::OnCbnSelchangeComboDataFormat()
{
	 m_value_format=m_combox_valueformat.GetCurSel();
}
