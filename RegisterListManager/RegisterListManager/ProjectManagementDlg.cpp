// ProjectManagementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegisterListManager.h"
#include "ProjectManagementDlg.h"
#include "afxdialogex.h"
#include "UserManagermentDlg.h"


// CProjectManagementDlg dialog

IMPLEMENT_DYNAMIC(CProjectManagementDlg, CDialogEx)

CProjectManagementDlg::CProjectManagementDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProjectManagementDlg::IDD, pParent)
{

}

CProjectManagementDlg::~CProjectManagementDlg()
{
}

void CProjectManagementDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_project_list);
}


BEGIN_MESSAGE_MAP(CProjectManagementDlg, CDialogEx)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDOK, &CProjectManagementDlg::OnBnClickedOk)
    ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Lists)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Change_Item_List)

END_MESSAGE_MAP()


// CProjectManagementDlg message handlers


void CProjectManagementDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (nType==SIZE_RESTORED)
    {
        CRect ViewRect;
        GetClientRect(&ViewRect);
        if (m_project_list.GetSafeHwnd())
        {
            m_project_list.MoveWindow(CRect(0,0,ViewRect.Width(),ViewRect.Height()),TRUE);
        }
        
    }
}


BOOL CProjectManagementDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    CRect ViewRect;
    GetClientRect(&ViewRect);
    if (m_project_list.GetSafeHwnd())
    {
        m_project_list.MoveWindow(CRect(0,0,ViewRect.Width(),ViewRect.Height()),TRUE);
    }
     g_hwnd_now = this->m_hWnd;
         Load_ProductList();
        Load_Users(); 
       Initial_List();
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CProjectManagementDlg::Load_ProductList(){
    CString StrSQL;
    CString StrTemp;
    Product_Table_Struct temp_product;
    _variant_t temp_variant  ;
    g_product_table.clear();
    StrSQL= _T("Select * from ProductsTypeRegisterTables");
    g_database_operator.m_pRecordset = g_database_operator.OpenRecordset(StrSQL);
    while(VARIANT_FALSE==g_database_operator.m_pRecordset->EndOfFile){
        temp_variant=g_database_operator.m_pRecordset->GetCollect("ProductType");//
        if(temp_variant.vt!=VT_NULL)
            StrTemp=temp_variant;
        else
            StrTemp=_T("");

        temp_product.ProductType =StrTemp;

        temp_variant=g_database_operator.m_pRecordset->GetCollect("TableName");//
        if(temp_variant.vt!=VT_NULL)
            StrTemp=temp_variant;
        else
            StrTemp=_T("");
        temp_product.TableName =StrTemp;

        temp_variant=g_database_operator.m_pRecordset->GetCollect("Col_RegName");//
        if(temp_variant.vt!=VT_NULL)
            StrTemp=temp_variant;
        else
            StrTemp=_T("");
        temp_product.Col_RegName =StrTemp;

        temp_variant=g_database_operator.m_pRecordset->GetCollect("Col_RegAddress");//
        if(temp_variant.vt!=VT_NULL)
            StrTemp=temp_variant;
        else
            StrTemp=_T("");
        temp_product.Col_RegAddress =StrTemp;

        temp_variant=g_database_operator.m_pRecordset->GetCollect("Col_RegDescription");//
        if(temp_variant.vt!=VT_NULL)
            StrTemp=temp_variant;
        else
            StrTemp=_T("");
        temp_product.Col_RegDescription =StrTemp;


        temp_variant=g_database_operator.m_pRecordset->GetCollect("ProductName");//
        if(temp_variant.vt!=VT_NULL)
            StrTemp=temp_variant;
        else
            StrTemp=_T("");
        temp_product.Product_Name =StrTemp;

        temp_variant=g_database_operator.m_pRecordset->GetCollect("user_name_login");//
        if(temp_variant.vt!=VT_NULL)
            StrTemp=temp_variant;
        else
            StrTemp=_T("");  
        temp_product.user_name_login =StrTemp;

        g_product_table.push_back(temp_product);

        g_database_operator.m_pRecordset->MoveNext();

    }
    g_database_operator.CloseRecordset();  
}
void CProjectManagementDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    //CDialogEx::OnOK();

}

void CProjectManagementDlg::Initial_List(){
    CString strTemp;
    m_project_list.ShowWindow(SW_HIDE);
    m_project_list.DeleteAllItems();
    while ( m_project_list.DeleteColumn (0)) ;
    m_project_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    m_project_list.SetExtendedStyle(m_project_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
    m_project_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    m_project_list.InsertColumn(1, _T("Model NO"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_project_list.InsertColumn(2, _T("Product Name"), 150, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_project_list.InsertColumn(3, _T("Projecter"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    for (int i = 0;i<(int)g_product_table.size();i++)
    {
        strTemp.Format(_T("%d"),i+1);
        m_project_list.InsertItem(i,strTemp);
        m_project_list.SetItemText(i,1,g_product_table.at(i).ProductType);
        m_project_list.SetItemText(i,2,g_product_table.at(i).Product_Name);
        m_project_list.SetItemText(i,3,g_product_table.at(i).user_name_login);
    }
  

    for (int i = 0 ; i<(int)g_product_table.size();i++ )
    {
        if(ListCtrlEx::ComboBox == m_project_list.GetColumnType(3))
        {
            ListCtrlEx::CStrList strlist;
            for (int j=0;j<(int)g_users_DB.size();j++)
            {
                if (g_users_DB.at(j).Person_Roles.CompareNoCase(_T("user"))!=0)
                {
                    strlist.push_back(g_users_DB.at(j).Person_Name);
                }
             
            }
            strlist.push_back(_T("New..."));
            m_project_list.SetCellStringList(i, 3, strlist);		
        }
    }
    m_project_list.ShowWindow(SW_SHOW);
}

void CProjectManagementDlg::Load_Users(){
    CString StrSQL;
    Login_Person   Temp_User;
    g_users_DB.clear();  
    StrSQL.Format(_T("Select * from users "));
    g_database_operator.m_pRecordset = g_database_operator.OpenRecordset(StrSQL);
    _variant_t temp_variant  ;
    while(VARIANT_FALSE==g_database_operator.m_pRecordset->EndOfFile){

        temp_variant=g_database_operator.m_pRecordset->GetCollect("user_name_login");//
        if(temp_variant.vt!=VT_NULL)
            Temp_User.Person_Name=temp_variant;
        else
            Temp_User.Person_Name=_T("");  


        temp_variant=g_database_operator.m_pRecordset->GetCollect("roles");//
        if(temp_variant.vt!=VT_NULL)
            Temp_User.Person_Roles=temp_variant;
        else
            Temp_User.Person_Roles=_T("");  
        g_users_DB.push_back(Temp_User);
        g_database_operator.m_pRecordset->MoveNext();
    }
    g_database_operator.CloseRecordset(); 
}
LRESULT CProjectManagementDlg::Change_Item_List(WPARAM wParam,LPARAM lParam){

    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam; 
    BOOL Is_Changed=FALSE;
    CString temp_task_info;

    CString New_CString =  m_project_list.GetItemText(Changed_Item,Changed_SubItem);
    CString cstemp_value;
    CString StrSQL;
  
    //if (Changed_SubItem == 1)//Product Type
    //{
    //    if (Changed_Item >= (int)g_users_DB.size())
    //    {
    //        StrSQL.Format(_T("INSERT INTO users (user_name_login,roles) VALUES ('%s', 'user')"),New_CString);
    //        Is_Changed=TRUE;
    //    }
    //    else
    //    {
    //         
    //    }


    //    if (Is_Changed)
    //    {
    //        g_database_operator.m_pConnection->Execute(StrSQL.GetString(),NULL,adCmdText);
    //        // AfxMessageBox(_T("Execute successfully!")) ;
    //    }

    //}

    if (Changed_SubItem == 3)
    {
       if (New_CString.CompareNoCase(_T("New..."))==0)
       {
           CUserManagermentDlg dlg;
           dlg.DoModal();
             g_hwnd_now = this->m_hWnd;
           PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
           return 0;
       }
      if(g_product_table.at(Changed_Item).user_name_login.CompareNoCase(New_CString)!=0)
      {
         StrSQL.Format(_T("Update   ProductsTypeRegisterTables Set user_name_login = '%s' Where ProductType = %d "),
                                New_CString,
                           _wtoi(g_product_table.at(Changed_Item).ProductType));
        Is_Changed=TRUE;
      }
    }
    if (Is_Changed)
    {
        g_database_operator.m_pConnection->Execute(StrSQL.GetString(),NULL,adCmdText);
        PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
    }
   



    return 0;
}


LRESULT CProjectManagementDlg::Fresh_Lists(WPARAM wParam,LPARAM lParam){
    CString strTemp;
    int fresh_input = (int) wParam;

    //  m_data_loading.ShowWindow(TRUE);
    if (fresh_input == 0)
    {     
        Load_ProductList();
        Load_Users(); 
        m_project_list.DeleteAllItems();
        for (int i = 0;i<(int)g_product_table.size();i++)
        {
            strTemp.Format(_T("%d"),i+1);
            m_project_list.InsertItem(i,strTemp);
            m_project_list.SetItemText(i,1,g_product_table.at(i).ProductType);
            m_project_list.SetItemText(i,2,g_product_table.at(i).Product_Name);
            m_project_list.SetItemText(i,3,g_product_table.at(i).user_name_login);
        }
        for (int i = 0 ; i<(int)g_product_table.size();i++ )
        {
            if(ListCtrlEx::ComboBox == m_project_list.GetColumnType(3))
            {
                ListCtrlEx::CStrList strlist;
                for (int j=0;j<(int)g_users_DB.size();j++)
                {
                    if (g_users_DB.at(j).Person_Roles.CompareNoCase(_T("user"))!=0)
                    {
                        strlist.push_back(g_users_DB.at(j).Person_Name);
                    }

                }
                strlist.push_back(_T("New..."));
                m_project_list.SetCellStringList(i, 3, strlist);		
            }
        }
        return 1;
    }



    return 0;
}