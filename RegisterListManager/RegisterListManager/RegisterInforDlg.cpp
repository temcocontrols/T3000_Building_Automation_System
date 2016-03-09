// RegisterInforDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegisterListManager.h"
#include "RegisterInforDlg.h"
#include "afxdialogex.h"


// CRegisterInforDlg dialog

IMPLEMENT_DYNAMIC(CRegisterInforDlg, CDialogEx)

CRegisterInforDlg::CRegisterInforDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegisterInforDlg::IDD, pParent)
{

    m_strAdd = g_current_register.Reg_Add ;
    m_strComment = g_current_register.Reg_Dsp ;
    m_strName = g_current_register.Reg_Name ;
}

CRegisterInforDlg::~CRegisterInforDlg()
{
}

void CRegisterInforDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_ADDRESS, m_strAdd);
    DDX_Text(pDX, IDC_EDIT_COMMENT, m_strComment);
    DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
}


BEGIN_MESSAGE_MAP(CRegisterInforDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CRegisterInforDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDC_BUTTON_FORWARD, &CRegisterInforDlg::OnBnClickedButtonForward)
    ON_BN_CLICKED(IDC_BUTTON_BACKWARD, &CRegisterInforDlg::OnBnClickedButtonBackward)
END_MESSAGE_MAP()


// CRegisterInforDlg message handlers


BOOL CRegisterInforDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    g_Is_Database_Changed = FALSE;
    // TODO:  Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CRegisterInforDlg::OnBnClickedOk()
{   CString StrTemp;
    BOOL Can_GO;
    if (g_login_user.Person_Roles.CompareNoCase(L"engineer")==0&&g_login_user.Person_Name.CompareNoCase(g_current_product.user_name_login)==0)
    {

        Can_GO = TRUE;

    }
    else if (g_login_user.Person_Roles.CompareNoCase(L"admin")==0)
    {
        Can_GO = TRUE;
    }
    else
    {
        Can_GO = FALSE;
    }
    if (!Can_GO)
    {
        StrTemp.Format(_T("User Name:'%s' have no permission to change anything!"),g_login_user.Person_Name);
        AfxMessageBox(StrTemp);
        return;
    }
        
    UpdateData(TRUE);
    CString StrSQL;
    
    _variant_t temp_variant  ;                   
    StrSQL.Format(_T("Select * from %s where %s =%d  "),
    g_current_product.TableName,g_current_product.Col_RegAddress,_wtoi(m_strAdd));
    g_database_operator.m_pRecordset = g_database_operator.OpenRecordset(StrSQL);
    if (VARIANT_FALSE==g_database_operator.m_pRecordset->EndOfFile) //exist the current register address
    {
        g_database_operator.CloseRecordset(); 
        if (m_strName.IsEmpty())
        {
            m_strName = _T("RESERVED");
            m_strComment = _T("RESERVED");
        }
          
        StrSQL.Format(_T("UPDATE %s SET %s = '%s' ,  %s = '%s' WHERE %s = %d "),
                            g_current_product.TableName,
                            g_current_product.Col_RegName,m_strName,
                            g_current_product.Col_RegDescription,m_strComment,
                            g_current_product.Col_RegAddress,_wtoi(m_strAdd));
        g_database_operator.m_pConnection->Execute(StrSQL.GetString(),NULL,adCmdText);
         g_Is_Database_Changed = TRUE;
        AfxMessageBox(_T("Execute successfully!")) ;
      
    }
    else
    {
        g_database_operator.CloseRecordset();
        StrSQL.Format(_T("Select * from %s where %s = '%s'  "),
            g_current_product.TableName,g_current_product.Col_RegName,m_strName);
        g_database_operator.m_pRecordset = g_database_operator.OpenRecordset(StrSQL);
        if (VARIANT_FALSE==g_database_operator.m_pRecordset->EndOfFile) //exist the current register address
        {
           CString StrAddress = g_database_operator.m_pRecordset->GetCollect(g_current_product.Col_RegAddress.GetBuffer());
           StrTemp.Format(_T("%s Exist.RegAddress:%s"),m_strName,StrAddress);
           AfxMessageBox(StrTemp);
           g_database_operator.CloseRecordset();
           return; 
        }
         g_database_operator.CloseRecordset(); 


        StrSQL.Format(_T("INSERT INTO %s (%s,%s,%s) VALUES (%d, '%s','%s')"),
                        g_current_product.TableName,
                        g_current_product.Col_RegAddress,
                        g_current_product.Col_RegName,
                        g_current_product.Col_RegDescription,
                        _wtoi(m_strAdd),
                        m_strName,
                        m_strComment);
        g_database_operator.m_pConnection->Execute(StrSQL.GetString(),NULL,adCmdText);
        g_Is_Database_Changed = TRUE;
        AfxMessageBox(_T("Execute successfully!")) ;

    }
    

}


void CRegisterInforDlg::OnBnClickedButtonForward()
{
    if (m_current_index >=0)
    {
        GetDlgItem(IDC_BUTTON_FORWARD)->EnableWindow(TRUE);

    }
    else  
    {
        GetDlgItem(IDC_BUTTON_FORWARD)->EnableWindow(FALSE);
    }

    if (m_current_index <= (int)g_registerlist_table.size()-1)
    {
        GetDlgItem(IDC_BUTTON_BACKWARD)->EnableWindow(TRUE);
    } 
    else
    {
        GetDlgItem(IDC_BUTTON_BACKWARD)->EnableWindow(FALSE);
    }
    if (m_current_index>0)
    {
        --m_current_index;
       g_current_register = g_registerlist_table.at(m_current_index);
       m_strAdd = g_current_register.Reg_Add ;
       m_strComment = g_current_register.Reg_Dsp ;
       m_strName = g_current_register.Reg_Name ;
       UpdateData(FALSE);
    }
    else
    {
         GetDlgItem(IDC_BUTTON_FORWARD)->EnableWindow(FALSE);
         GetDlgItem(IDC_BUTTON_BACKWARD)->EnableWindow(TRUE);
    }  
    
}


void CRegisterInforDlg::OnBnClickedButtonBackward()
{                                                            
    if (m_current_index >=0)
    {
        GetDlgItem(IDC_BUTTON_FORWARD)->EnableWindow(TRUE);
        
    }
    else  
    {
    GetDlgItem(IDC_BUTTON_FORWARD)->EnableWindow(FALSE);
    }

    if (m_current_index <= (int)g_registerlist_table.size()-1)
    {
      GetDlgItem(IDC_BUTTON_BACKWARD)->EnableWindow(TRUE);
    } 
    else
    {
     GetDlgItem(IDC_BUTTON_BACKWARD)->EnableWindow(FALSE);
    }
    if (m_current_index < (int)g_registerlist_table.size()-1)
    {
        ++m_current_index;
        g_current_register = g_registerlist_table.at(m_current_index);
        m_strAdd = g_current_register.Reg_Add ;
        m_strComment = g_current_register.Reg_Dsp ;
        m_strName = g_current_register.Reg_Name ;
        UpdateData(FALSE);
    }
    
}


BOOL CRegisterInforDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if (WM_KEYDOWN == pMsg->message)
    {
        if (VK_LEFT == pMsg->wParam)
        {
           OnBnClickedButtonForward();
        }
        if (VK_RIGHT == pMsg->wParam)
        {
           OnBnClickedButtonBackward();
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
