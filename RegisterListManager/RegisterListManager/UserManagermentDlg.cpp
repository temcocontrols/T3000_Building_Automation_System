// UserManagermentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegisterListManager.h"
#include "UserManagermentDlg.h"
#include "afxdialogex.h"


// CUserManagermentDlg dialog

IMPLEMENT_DYNAMIC(CUserManagermentDlg, CDialogEx)

CUserManagermentDlg::CUserManagermentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserManagermentDlg::IDD, pParent)
{

}

CUserManagermentDlg::~CUserManagermentDlg()
{
}

void CUserManagermentDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_user_list);
}


BEGIN_MESSAGE_MAP(CUserManagermentDlg, CDialogEx)
ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Lists)
ON_MESSAGE(WM_LIST_ITEM_CHANGED,Change_Item_List)
END_MESSAGE_MAP()


// CUserManagermentDlg message handlers


BOOL CUserManagermentDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    g_hwnd_now = this->m_hWnd;
     Load_Users();
    Initial_UserList();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserManagermentDlg::Load_Users(){
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

void CUserManagermentDlg::Initial_UserList(){
    CString strTemp;
    m_user_list.ShowWindow(SW_HIDE);
    m_user_list.DeleteAllItems();
    while ( m_user_list.DeleteColumn (0)) ;
    m_user_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    m_user_list.SetExtendedStyle(m_user_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
     m_user_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    m_user_list.InsertColumn(1, _T("User Name"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_user_list.InsertColumn(2, _T("User Role"), 200, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
    for (int i = 0;i<(int)g_users_DB.size();i++)
    {
         strTemp.Format(_T("%d"),i+1);
        m_user_list.InsertItem(i,strTemp);
        m_user_list.SetItemText(i,1,g_users_DB.at(i).Person_Name);
        m_user_list.SetItemText(i,2,g_users_DB.at(i).Person_Roles);
    }
    for (int i = (int)g_users_DB.size();i<5+(int)g_users_DB.size();i++)
    {
        strTemp.Format(_T("%d"),i+1);
        m_user_list.InsertItem(i,strTemp);
        
    }

    for (int i = 0 ; i< 5+(int)g_users_DB.size();i++ )
    {
        if(ListCtrlEx::ComboBox == m_user_list.GetColumnType(2))
        {
            ListCtrlEx::CStrList strlist;
             
            strlist.push_back(_T("admin"));
            strlist.push_back(_T("engineer"));
            strlist.push_back(_T("user"));
            m_user_list.SetCellStringList(i, 2, strlist);		
        }
    }

    m_user_list.ShowWindow(SW_SHOW);
}


LRESULT CUserManagermentDlg::Change_Item_List(WPARAM wParam,LPARAM lParam){

    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam; 
    BOOL Is_Changed=FALSE;
     CString temp_task_info;
   
        CString New_CString =  m_user_list.GetItemText(Changed_Item,Changed_SubItem);
        CString cstemp_value;
        CString StrSQL;
     
        //User Name 
        if (Changed_SubItem == 1)//Value
        {
            if (Changed_Item >= (int)g_users_DB.size())
            {
                StrSQL.Format(_T("INSERT INTO users (user_name_login,roles) VALUES ('%s', 'user')"),New_CString);
                Is_Changed=TRUE;
            }
            else
            {
                if(g_users_DB.at(Changed_Item).Person_Name.CompareNoCase(New_CString)!=0)
                {
                    if (New_CString.IsEmpty())
                    {
                       StrSQL.Format(_T("Delete From users Where  user_name_login = '%s'"),
                                     g_users_DB.at(Changed_Item).Person_Name);   
                    }
                    else
                    {
                        StrSQL.Format(_T("Update users Set user_name_login ='%s' where user_name_login = '%s' "),
                            New_CString,g_users_DB.at(Changed_Item).Person_Name);
                    }
                 
                                    Is_Changed=TRUE;
                }
            }


            if (Is_Changed)
            {
                g_database_operator.m_pConnection->Execute(StrSQL.GetString(),NULL,adCmdText);
               // AfxMessageBox(_T("Execute successfully!")) ;
            }
            
        }
 
        
        if (Changed_SubItem == 2)
        {
            if (Changed_Item >= (int)g_users_DB.size())
            {
                     AfxMessageBox(_T("Please input the user name"));
                     return 0;
            }
            else
            {
                if(g_users_DB.at(Changed_Item).Person_Roles.CompareNoCase(New_CString)!=0)
                {
                    StrSQL.Format(_T("Update users Set roles  ='%s' where user_name_login = '%s' "),
                        New_CString,g_users_DB.at(Changed_Item).Person_Name);
                    Is_Changed=TRUE;
                }
                if (Is_Changed)
                {
                    g_database_operator.m_pConnection->Execute(StrSQL.GetString(),NULL,adCmdText);
                   // AfxMessageBox(_T("Execute successfully!")) ;
                }
            }

        }
        PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
    
    

    return 0;
}


LRESULT CUserManagermentDlg::Fresh_Lists(WPARAM wParam,LPARAM lParam){
    CString strTemp;
    int fresh_input = (int) wParam;

    //  m_data_loading.ShowWindow(TRUE);
    if (fresh_input == 0)
    {     
        Load_Users();
        m_user_list.DeleteAllItems();
        for (int i = 0;i<(int)g_users_DB.size();i++)
        {
            strTemp.Format(_T("%d"),i+1);
            m_user_list.InsertItem(i,strTemp);
            m_user_list.SetItemText(i,1,g_users_DB.at(i).Person_Name);
            m_user_list.SetItemText(i,2,g_users_DB.at(i).Person_Roles);
        }
        for (int i = (int)g_users_DB.size();i<5+(int)g_users_DB.size();i++)
        {
            strTemp.Format(_T("%d"),i+1);
            m_user_list.InsertItem(i,strTemp);

        }

        for (int i = 0 ; i< 5+(int)g_users_DB.size();i++ )
        {
            if(ListCtrlEx::ComboBox == m_user_list.GetColumnType(2))
            {
                ListCtrlEx::CStrList strlist;

                strlist.push_back(_T("admin"));
                strlist.push_back(_T("engineer"));
                strlist.push_back(_T("user"));
                m_user_list.SetCellStringList(i, 2, strlist);		
            }
        }
        return 1;
    }

 
 
    return 0;
}
