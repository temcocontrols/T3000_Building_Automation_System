// ProductRegisterListView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ProductRegisterListView.h"
 #include "../SQLiteDriver/CppSQLite3.h"
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
			 unsigned short function_code = Parent->m_register_data_sheet.at(i).function_code;
             if (Count_Number > 0)
             {            
              unsigned short  DataBuffer[100];
            
                 while (!Parent->m_isReading)
                 {
                     Sleep(500);
                     continue;
                 }
                 Sleep(Parent->m_UINT_delay_items);
                 // Read_Multi(ModbusID,DataBuffer,Start_Address,Count_Number)

                #if 1
				 unsigned char rev_back_rawData[300], send_data[100];
				 int Send_length;
				 int Rev_length;
				// int ret = Modbus_Standard_Read(ModbusID,DataBuffer,  function_code, Start_Address, Count_Number);
				 int ret = Modbus_Standard_Read(ModbusID, &DataBuffer[0], function_code, Start_Address, Count_Number, &send_data[0], &rev_back_rawData[0], &Send_length, &Rev_length);

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
						if (Parent->m_register_data_sheet.at(i).caculate == 2)//Raw
						{
							int datavalue = 0 ;
							for (int len=0;len<Count_Number;len++)
							{
								datavalue+=((int)pow((long double)256,Count_Number-len-1))*(short)DataBuffer[len];
							}	
							strValue.Format(_T("%d"),datavalue);

						}
						else if (Parent->m_register_data_sheet.at(i).caculate == 1)
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
						if (Parent->m_register_data_sheet.at(i).caculate == 2)//Raw
						{
							int datavalue = 0 ;
							for (int len=0;len<Count_Number;len++)
							{
								datavalue+=((int)pow((long double)256,Count_Number-len-1))*DataBuffer[len];
							}	
							strValue.Format(_T("%d"),datavalue);

						}
						else if (Parent->m_register_data_sheet.at(i).caculate == 1)
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
                        
						if (Parent->m_register_data_sheet.at(i).caculate == 2)//Raw
						{
							int datavalue = 0 ;
							for (int len=0;len<Count_Number;len++)
							{
								datavalue+=((int)pow((long double)256,Count_Number-len-1))*DataBuffer[len];
							}	
							strValue.Format(_T("0x%X"),datavalue);

						}
						else if (Parent->m_register_data_sheet.at(i).caculate == 1)
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
 	m_array_modbus_function[0] = L"Read Coils";
 	m_array_modbus_function[1] = L"Read Discrete Inputs";
 	m_array_modbus_function[2] = L"Read Holding Registers";
 	m_array_modbus_function[3] = L"Read Input Registers";
	m_array_modbus_function[4] = L"Write Single Coil";
	m_array_modbus_function[5] = L"Write Single Register";
	m_array_modbus_function[6] = L"Write Multiple Coils";
	m_array_modbus_function[7] = L"Write Multiple Registers";

	m_array_caculate[0] = L"Raw Data";
	m_array_caculate[1] = L"ASC";
	m_array_caculate[2] = L"DEC";
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

void CProductRegisterListView::SetStart(BOOL Start)
{
	m_isReading = Start;
}
void CProductRegisterListView::Fresh(void)
{
	//Import_CustomProductTable();
    close_com();
    g_bPauseMultiRead = TRUE;
    m_upButton.SetImage(IDB_UPBMP);
    m_downButton.SetImage(IDB_DOWNBMP);
    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    m_current_tree_node = pFrame->m_current_tree_node; 
    /* m_isReading=*/
	pFrame->ConnectDevice(m_current_tree_node);
    m_isReading = TRUE;
    if (m_isReading)
    {
        GetDlgItem(IDC_READ_DEVICE)->SetWindowText(_T("Stop Read"));
    } 
    else
    {
        GetDlgItem(IDC_READ_DEVICE)->SetWindowText(_T("Start Read"));
    }
 
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
	m_register_list.InsertColumn(0, _T("Function"), 230, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_register_list.InsertColumn(1, _T("Modbus Address"), 180, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    m_register_list.InsertColumn(2, _T("Description"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_register_list.InsertColumn(3, _T("Length"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    
    m_register_list.InsertColumn(4, _T("R/W"), 90, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_register_list.InsertColumn(5, _T("Value"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_register_list.InsertColumn(6, _T("Data Format"), 90, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(7, _T("Caculation"), 90, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    for (int i = 0;i<(int)m_register_data_sheet.size();i++)
    {   
        
		strTemp.Format(_T("%s"), GetFunctionName(m_register_data_sheet.at(i).function_code));
        m_register_list.InsertItem(i,strTemp);
		strTemp.Format(_T("%d"), m_register_data_sheet.at(i).Reg_ID);
        m_register_list.SetItemText(i,1, strTemp);
        m_register_list.SetItemText(i,2, m_register_data_sheet.at(i).Para_Type);
		strTemp.Format(_T("%d"), m_register_data_sheet.at(i).Counts_Number);
        m_register_list.SetItemText(i,3, strTemp);
        strTemp.Format(_T("%s"),m_register_data_sheet.at(i).Property);
        m_register_list.SetItemText(i,4, strTemp);
        strTemp.Format(_T("%s"),m_register_data_sheet.at(i).Value);
        m_register_list.SetItemText(i,5, strTemp);
        strTemp.Format(_T("%s"),m_register_data_sheet.at(i).DataFormat);
        m_register_list.SetItemText(i,6, strTemp);
		strTemp.Format(_T("%s"),GetCalName(m_register_data_sheet.at(i).caculate) );
		m_register_list.SetItemText(i, 7, strTemp);

		if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(0))
		{
			ListCtrlEx::CStrList strlist;
			for (int i=0;i<4;i++)
			{
				strlist.push_back(m_array_modbus_function[i]);
			}
 			m_register_list.SetCellStringList(i, 0, strlist);
		}   

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

		if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(7))
		{
			ListCtrlEx::CStrList strlist;
			for (int i = 0; i < 3; i++)
			{
				strlist.push_back(m_array_caculate[i]);
			}
			m_register_list.SetCellStringList(i, 7, strlist);
		}
    }
   
	m_register_list.InsertItem(m_register_data_sheet.size(), GetFunctionName(3));
	//m_register_list.SetItemText(m_register_data_sheet.size(), 3, GetFunctionName(3));
	if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(0))
	{
		ListCtrlEx::CStrList strlist;
		for (int i = 0; i < 4; i++)
		{
			strlist.push_back(m_array_modbus_function[i]);
		}
		m_register_list.SetCellStringList(m_register_data_sheet.size(), 0, strlist);
	}

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
	if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(7))
	{
		ListCtrlEx::CStrList strlist;
		for (int i = 0; i < 3; i++)
		{
			strlist.push_back(m_array_caculate[i]);
		}
		m_register_list.SetCellStringList(m_register_data_sheet.size(), 7, strlist);
	}
    m_register_list.ShowWindow(SW_SHOW);
}

void CProductRegisterListView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();  
}
void CProductRegisterListView::Import_CustomProductTable()
{
	CString StrSql;
	CustomProductTable_T Struc_Temp;
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q,q1;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);


	StrSql.Format(_T("Select * From ProductsTypeRegisterTables"));
	q = SqliteDBT3000.execQuery((UTF8MBSTR)StrSql);
	while (!q.eof())
	{
		     CString TableName = q.getValuebyName(L"TableName");
			 
			 if (TableName.IsEmpty())
			 {
				 q.nextRow();
				 continue;
			 }
		     int modelid = q.getIntField("ProductType");
			 CString RegName = q.getValuebyName(L"Col_RegName");
			 CString RegID = q.getValuebyName(L"Col_RegAddress");
			 if (modelid !=21 )
			 {
				 q.nextRow();
				 continue;
			 }
			/*if (modelid>=1&&modelid<=19)
			{
				StrSql.Format(_T("Select %s,%s, From %s "), RegName, RegID, TableName);
			} 
			else
			{*/
				StrSql.Format(_T("Select %s,%s From %s "), RegName, RegID, TableName);

			/*}*/
			 
			 
			 q1 = SqliteDBT3000.execQuery((UTF8MBSTR)StrSql);
			 while (!q1.eof())
			 {
				 CString ModbusName = q1.getValuebyName(RegName);
				 CString ModbusID = q1.getValuebyName(RegID);
				 CString comments;
				  if (ModbusName.CompareNoCase(L"RESERVED")==0||ModbusName.IsEmpty())
				  {
					  q1.nextRow();
					  continue;
				  }
			   

				StrSql.Format(_T("Insert Into CustomProductTable(ModelNo,Para_Type,Reg_ID,SN) Values (%d,'%s',%d,'%s') ")
					, modelid
					, ModbusName
					, _wtoi(ModbusID)
					, GetGUID()
				);

			    TRACE(StrSql);
				TRACE(L"\n");
			    
				SqliteDBT3000.execDML((UTF8MBSTR)StrSql);
				 
				q1.nextRow();
			 }
			 q1.finalize();
			 break;
			 q.nextRow();
	}
	q.finalize();
	SqliteDBT3000.closedb();

	 
}
void CProductRegisterListView::LoadDataSheet(){
    CString StrSql;
    _variant_t temp_variant ;
   int ProductModel = m_current_tree_node.product_class_id;
   CustomProductTable_T Struc_Temp;
   CppSQLite3DB SqliteDBT3000;
   CppSQLite3Query q;
   SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

   if (m_sort_type == 0)
   {  
        StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  "),ProductModel);
   }
   else if (m_sort_type == 1)
   {
       StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  ORDER BY Reg_ID DESC"),ProductModel);
   }
   else if(m_sort_type == 2)
   {
      StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  ORDER BY Reg_ID ASC"),ProductModel);
   }
   else
   {
      StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  "),ProductModel);
	//  StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  "), ProductModel);
   }
                                                      // 
  
    m_register_data_sheet.clear();
  
   q = SqliteDBT3000.execQuery((UTF8MBSTR)StrSql);
   while(!q.eof())
   {
	   Struc_Temp.SN = q.getValuebyName(L"SN");

	   Struc_Temp.ModelNo = q.getIntField("ModelNo", -1);//
	   Struc_Temp.Reg_Description = q.getValuebyName(L"Reg_Description");//
	   Struc_Temp.Reg_ID = q.getIntField("Reg_ID", -1);//
	   Struc_Temp.Para_Type = q.getValuebyName(L"Para_Type");//
	   Struc_Temp.Counts_Number = q.getIntField("Counts_Number");//
	   Struc_Temp.Property = q.getValuebyName(L"Property");//  
	   Struc_Temp.DataFormat = q.getValuebyName(L"DataFormat");//
	   Struc_Temp.function_code = q.getIntField("Function_Code");
	   Struc_Temp.caculate = q.getIntField("Caculate");

	   if (Struc_Temp.SN.IsEmpty())
	   {
		   Struc_Temp.SN = GetGUID();
		   StrSql.Format(_T("Update  CustomProductTable  Set SN = '%s' Where ModelNo = %d and Reg_ID = %d and Function_Code = %d  "), 
			   Struc_Temp.SN, Struc_Temp.ModelNo, Struc_Temp.Reg_ID, Struc_Temp.function_code);
		   SqliteDBT3000.execDML((UTF8MBSTR)StrSql);
	   }
	           
       m_register_data_sheet.push_back(Struc_Temp);
          
       q.nextRow();
   }

    SqliteDBT3000.closedb();
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
		   strTemp.Format(_T("%s"), GetFunctionName(m_register_data_sheet.at(i).function_code));
		   m_register_list.InsertItem(i, strTemp);
		   strTemp.Format(_T("%d"), m_register_data_sheet.at(i).Reg_ID);
		   m_register_list.SetItemText(i, 1, strTemp);
		   m_register_list.SetItemText(i, 2, m_register_data_sheet.at(i).Para_Type);
		   strTemp.Format(_T("%d"), m_register_data_sheet.at(i).Counts_Number);
		   m_register_list.SetItemText(i, 3, strTemp);
		   strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Property);
		   m_register_list.SetItemText(i, 4, strTemp);
		   strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Value);
		   m_register_list.SetItemText(i, 5, strTemp);
		   strTemp.Format(_T("%s"), m_register_data_sheet.at(i).DataFormat);
		   m_register_list.SetItemText(i, 6, strTemp);
		   strTemp.Format(_T("%s"), GetCalName(m_register_data_sheet.at(i).caculate));
		   m_register_list.SetItemText(i, 7, strTemp);
		   if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(0))
		   {
			   ListCtrlEx::CStrList strlist;
			   for (int i = 0; i < 4; i++)
			   {
				   strlist.push_back(m_array_modbus_function[i]);
			   }
			   m_register_list.SetCellStringList(i, 0, strlist);
		   }

		   if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(4))
		   {
			   ListCtrlEx::CStrList strlist;

			   strlist.push_back(_T("Read Only"));
			   strlist.push_back(_T("R/W"));
			   m_register_list.SetCellStringList(i, 4, strlist);
		   }  //Signed;Unsigned;Hex;Binary
		   if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(6))
		   {
			   ListCtrlEx::CStrList strlist;

			   strlist.push_back(_T("Signed"));
			   strlist.push_back(_T("Unsigned"));
			   strlist.push_back(_T("Hex"));
			   strlist.push_back(_T("Binary"));
			   strlist.push_back(_T("Char"));
			   m_register_list.SetCellStringList(i, 6, strlist);
		   }
		   if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(7))
		   {
			   ListCtrlEx::CStrList strlist;
			   for (int i = 0; i < 3; i++)
			   {
				   strlist.push_back(m_array_caculate[i]);
			   }
			   m_register_list.SetCellStringList(i, 7, strlist);
		   }
       }

	   m_register_list.InsertItem(m_register_data_sheet.size(), GetFunctionName(3));
	   if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(0))
	   {
		   ListCtrlEx::CStrList strlist;
		   for (int i = 0; i < 4; i++)
		   {
			   strlist.push_back(m_array_modbus_function[i]);
		   }
		   m_register_list.SetCellStringList(m_register_data_sheet.size(), 0, strlist);
	   }
	   if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(4))
	   {
		   ListCtrlEx::CStrList strlist;
		   strlist.push_back(_T("Read Only"));
		   strlist.push_back(_T("R/W"));
		   m_register_list.SetCellStringList(m_register_data_sheet.size(), 4, strlist);
	   }
	   if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(6))
	   {
		   ListCtrlEx::CStrList strlist;

		   strlist.push_back(_T("Signed"));
		   strlist.push_back(_T("Unsigned"));
		   strlist.push_back(_T("Hex"));
		   strlist.push_back(_T("Binary"));
		   strlist.push_back(_T("Char"));
		   m_register_list.SetCellStringList(m_register_data_sheet.size(), 6, strlist);
	   }
	   if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(7))
	   {
		   ListCtrlEx::CStrList strlist;
		   for (int i = 0; i < 3; i++)
		   {
			   strlist.push_back(m_array_caculate[i]);
		   }
		   m_register_list.SetCellStringList(m_register_data_sheet.size(), 7, strlist);
	   }
     }
     if (fresh_type == 1)
     {
         
		 strTemp.Format(_T("%s"),GetFunctionName(m_register_data_sheet.at(fresh_row).function_code));
         m_register_list.SetItemText(fresh_row,0,strTemp);
		 strTemp.Format(_T("%d"), m_register_data_sheet.at(fresh_row).Reg_ID);
         m_register_list.SetItemText(fresh_row,1, strTemp);

         m_register_list.SetItemText(fresh_row,2, m_register_data_sheet.at(fresh_row).Para_Type);
		 strTemp.Format(_T("%d"), m_register_data_sheet.at(fresh_row).Counts_Number);
         m_register_list.SetItemText(fresh_row,3, strTemp);
         strTemp.Format(_T("%s"),m_register_data_sheet.at(fresh_row).Property);
         m_register_list.SetItemText(fresh_row,4, strTemp);
         strTemp.Format(_T("%s"),m_register_data_sheet.at(fresh_row).Value);
         m_register_list.SetItemText(fresh_row,5, strTemp);
         strTemp.Format(_T("%s"),m_register_data_sheet.at(fresh_row).DataFormat);
         m_register_list.SetItemText(fresh_row,6, strTemp);

		 strTemp.Format(_T("%s"),GetCalName(m_register_data_sheet.at(fresh_row).caculate));
		 m_register_list.SetItemText(fresh_row, 7, strTemp);
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
	BOOL is_last = FALSE;
	if (lRow == m_register_data_sheet.size())
	{
		is_last = TRUE;
	}
 

	CString Function_Name = m_register_list.GetItemText(lRow, 0);
	CString Reg_ID = m_register_list.GetItemText(lRow, 1);
    CString Para_Type = m_register_list.GetItemText(lRow,2);
    CString Counts_Number = m_register_list.GetItemText(lRow,3);
	
    CString Property = m_register_list.GetItemText(lRow,4);
    CString StrDataFormat = m_register_list.GetItemText(lRow,6);
	CString strCalName = m_register_list.GetItemText(lRow,7);
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
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
       if (is_last)
       {

         if (lCol == 1){
            
             StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d and Reg_ID = %d and Function_Code = %d "),ProductModel,_wtoi(Reg_ID),GetFunctionCode(Function_Name));
             q = SqliteDBT3000.execQuery((UTF8MBSTR)StrSql);
             if(!q.eof()){
                 CString strTips;
                 strTips.Format(_T("Address=%s,Exists!"),Reg_ID);
                 AfxMessageBox(strTips);
                SqliteDBT3000.closedb();
                 m_register_list.SetItemText(lRow,lCol,_T(""));
                 return 0;
             }
			 CString strGUID = GetGUID();
             StrSql.Format(_T("Insert INTO CustomProductTable(ModelNo,Reg_ID,Function_Code,Para_Type,Property,Counts_Number,DataFormat,Caculate,SN) VALUES (%d,%d,%d,'','%s',%d,'%s',%d,'%s')")
                                ,ProductModel,_wtoi(Reg_ID), GetFunctionCode(Function_Name),m_string_property,m_short_counts,m_string_dataformat, 0,strGUID);
             SqliteDBT3000.execDML((UTF8MBSTR)StrSql);
             
              Struct_Temp.ModelNo =  ProductModel;
              Struct_Temp.Reg_ID =  _wtoi(Reg_ID);
              Struct_Temp.Property=m_string_property;
              Struct_Temp.Counts_Number = m_short_counts;
              Struct_Temp.DataFormat = m_string_dataformat;
			  Struct_Temp.function_code = GetFunctionCode(Function_Name);
			  Struct_Temp.SN = strGUID;
			  Struct_Temp.caculate = 0;
              m_register_data_sheet.push_back(Struct_Temp);

            m_register_list.InsertItem(m_register_data_sheet.size(),_T("")); //插入一行
			m_register_list.SetItemText(m_register_data_sheet.size(), 0, GetFunctionName(3));
			m_register_list.SetItemText(m_register_data_sheet.size(), 7, GetCalName(0));
			if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(0))
			{
				ListCtrlEx::CStrList strlist;
				for (int i = 0; i < 4; i++)
				{
					strlist.push_back(m_array_modbus_function[i]);
				}
				m_register_list.SetCellStringList(m_register_data_sheet.size(), 0, strlist);
			}
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
			if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(7))
			{
				ListCtrlEx::CStrList strlist;
				for (int i = 0; i < 3; i++)
				{
					strlist.push_back(m_array_caculate[i]);
				}
				m_register_list.SetCellStringList(m_register_data_sheet.size(), 7, strlist);
			}
             PostMessage(WM_REFRESH_BAC_INPUT_LIST,1U,lRow) ;
         }
          
       } 
       else
       {
            BOOL m_ischange = TRUE;
            if (lCol == 1)
            {
                if (Reg_ID.IsEmpty())
                {
                     StrSql.Format(_T("Delete   From CustomProductTable Where SN = '%s' "), m_register_data_sheet.at(lRow).SN);
                     m_ischange =FALSE;   
                }
                else
                {
                    StrSql.Format(_T("Update  CustomProductTable  Set Reg_ID = %d Where SN = '%s'  "),
                         _wtoi(Reg_ID),m_register_data_sheet.at(lRow).SN);
                }
                
            }
            else
            {
                 StrSql.Format(_T("Update  CustomProductTable  Set Para_Type = '%s' ,Counts_Number = %d ,Property = '%s' ,Function_Code = %d ,DataFormat='%s',Caculate=%d  Where SN = '%s' "),
                     Para_Type,_wtoi(Counts_Number),Property,GetFunctionCode(Function_Name),StrDataFormat,GetCaculateCode(strCalName),m_register_data_sheet.at(lRow).SN);
            }
             SqliteDBT3000.execDML((UTF8MBSTR)StrSql);
             if (m_ischange)
             {
                m_register_data_sheet.at(lRow).Para_Type = Para_Type ;
                m_register_data_sheet.at(lRow).Counts_Number = _wtoi(Counts_Number) ;
                m_register_data_sheet.at(lRow).Property =   Property;
				m_register_data_sheet.at(lRow).function_code = GetFunctionCode(Function_Name);
                m_register_data_sheet.at(lRow).Reg_ID =  _wtoi(Reg_ID);
                m_register_data_sheet.at(lRow).DataFormat = StrDataFormat;
				m_register_data_sheet.at(lRow).caculate = GetCaculateCode(strCalName);
                PostMessage(WM_REFRESH_BAC_INPUT_LIST,1U,lRow) ;
             } 
             else
             {
                PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0); 
             }
                    
       }
       SqliteDBT3000.closedb();
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

int CProductRegisterListView::GetFunctionCode(CString FunctionName)
{
	if (m_array_modbus_function[0].CompareNoCase(FunctionName)==0)
	{
		return 1;
	}
	if (m_array_modbus_function[1].CompareNoCase(FunctionName) == 0)
	{
		return 2;
	}
	if (m_array_modbus_function[2].CompareNoCase(FunctionName) == 0)
	{
		return 3;
	}
	if (m_array_modbus_function[3].CompareNoCase(FunctionName) == 0)
	{
		return 4;
	}
	if (m_array_modbus_function[4].CompareNoCase(FunctionName) == 0)
	{
		return 5;
	}
	if (m_array_modbus_function[5].CompareNoCase(FunctionName) == 0)
	{
		return 6;
	}
	if (m_array_modbus_function[6].CompareNoCase(FunctionName) == 0)
	{
		return 15;
	}
	if (m_array_modbus_function[7].CompareNoCase(FunctionName) == 0)
	{
		return 16;
	}
 
}

CString CProductRegisterListView::GetFunctionName(int FunctionCode)
{
	if (FunctionCode == 1)
	{
		return m_array_modbus_function[0];
	}
	if (FunctionCode == 2)
	{
		return m_array_modbus_function[1];
	}
	 
	if (FunctionCode == 3)
	{
		return m_array_modbus_function[2];
	}
	if (FunctionCode == 4)
	{
		return m_array_modbus_function[3];
	}
	if (FunctionCode == 5)
	{
		return m_array_modbus_function[4];
	}
	if (FunctionCode == 6)
	{
		return m_array_modbus_function[5];
	}
	if (FunctionCode == 15)
	{
		return m_array_modbus_function[6];
	}
	if (FunctionCode == 16)
	{
		return m_array_modbus_function[7];
	}
}

int CProductRegisterListView::GetCaculateCode(CString CalName)
{
	if (m_array_caculate[0].CompareNoCase(CalName) == 0)
	{
		return 0;
	}
	if (m_array_caculate[1].CompareNoCase(CalName) == 0)
	{
		return 1;
	}
	if (m_array_caculate[2].CompareNoCase(CalName) == 0)
	{
		return 2;
	}
}

CString CProductRegisterListView::GetCalName(int calcode)
{
	if (calcode == 0)
	{
		return m_array_caculate[0];
	}
	if (calcode == 1)
	{
		return m_array_caculate[1];
	}

	if (calcode == 2)
	{
		return m_array_caculate[2];
	}
}