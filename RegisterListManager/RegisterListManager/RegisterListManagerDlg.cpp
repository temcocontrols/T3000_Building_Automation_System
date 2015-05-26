
// RegisterListManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegisterListManager.h"
#include "RegisterListManagerDlg.h"
#include "afxdialogex.h"
#include "global_define.h"
#include "RegisterInforDlg.h"
#include "global_function.h"
#include "ChangePSWDlg.h"
#include "UserManagermentDlg.h"
#include "ProjectManagementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//    afx_msg void OnUpdateSystemChange(CCmdUI *pCmdUI);
//    afx_msg void OnUpdateSystemProjectmanagerment(CCmdUI *pCmdUI);
//    afx_msg void OnSystemUsersmanagerment();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//    ON_UPDATE_COMMAND_UI(ID_SYSTEM_CHANGE, &CAboutDlg::OnUpdateSystemChange)
//    ON_UPDATE_COMMAND_UI(ID_SYSTEM_PROJECTMANAGERMENT, &CAboutDlg::OnUpdateSystemProjectmanagerment)
//    ON_COMMAND(ID_SYSTEM_USERSMANAGERMENT, &CAboutDlg::OnSystemUsersmanagerment)
END_MESSAGE_MAP()


// CRegisterListManagerDlg dialog




CRegisterListManagerDlg::CRegisterListManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegisterListManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRegisterListManagerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_PRODUCT, m_Product_ListCtrl);
    DDX_Control(pDX, IDC_LIST_REGISTERLIST, m_RegisterListCtrl);
    DDX_Control(pDX, IDC_STATIC_PRODUCT, m_label_product);
    DDX_Control(pDX, IDC_STATIC_REGISTERLIST, m_label_registerlist);
    DDX_Control(pDX, IDC_STATIC_LOADING, m_data_loading);
}

BEGIN_MESSAGE_MAP(CRegisterListManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_SIZE()
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_PRODUCT, &CRegisterListManagerDlg::OnNMDblclkListProduct)

    ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Lists)

    ON_NOTIFY(NM_DBLCLK, IDC_LIST_REGISTERLIST, &CRegisterListManagerDlg::OnNMDblclkListRegisterlist)
    ON_UPDATE_COMMAND_UI(ID_SYSTEM_CHANGE, &CRegisterListManagerDlg::OnUpdateSystemChange)
//    ON_COMMAND(ID_SYSTEM_PROJECTMANAGERMENT, &CRegisterListManagerDlg::OnSystemProjectmanagerment)
ON_UPDATE_COMMAND_UI(ID_SYSTEM_PROJECTMANAGERMENT, &CRegisterListManagerDlg::OnUpdateSystemProjectmanagerment)
ON_UPDATE_COMMAND_UI(ID_SYSTEM_USERSMANAGERMENT, &CRegisterListManagerDlg::OnUpdateSystemUsersmanagerment)
ON_COMMAND(ID_SYSTEM_EXIT, &CRegisterListManagerDlg::OnSystemExit)
ON_COMMAND(ID_SYSTEM_EXPORTREGISTERLISTS, &CRegisterListManagerDlg::OnSystemExportregisterlists)
ON_COMMAND(ID_SYSTEM_CHANGE, &CRegisterListManagerDlg::OnSystemChange)
ON_COMMAND(ID_SYSTEM_USERSMANAGERMENT, &CRegisterListManagerDlg::OnSystemUsersmanagerment)
ON_COMMAND(ID_SYSTEM_PROJECTMANAGERMENT, &CRegisterListManagerDlg::OnSystemProjectmanagerment)
ON_BN_CLICKED(IDC_MFCBUTTON_EXPORT_HEAD_FILE, &CRegisterListManagerDlg::OnBnClickedMfcbuttonExportHeadFile)
ON_BN_CLICKED(IDC_MFCBUTTON_PASSWORD, &CRegisterListManagerDlg::OnBnClickedMfcbuttonPassword)
ON_BN_CLICKED(IDC_MFCBUTTON_USERSMANAGERMENT, &CRegisterListManagerDlg::OnBnClickedMfcbuttonUsersmanagerment)
ON_BN_CLICKED(IDC_MFCBUTTON_PROJECTMANAGERMENT, &CRegisterListManagerDlg::OnBnClickedMfcbuttonProjectmanagerment)
END_MESSAGE_MAP()


// CRegisterListManagerDlg message handlers

BOOL CRegisterListManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
      m_data_loading.ShowWindow(SW_HIDE);

     m_data_loading.SetWindowTextW(_T("Data is loading......"));
     m_data_loading.textColor(RGB(255,0,0));
     //m_current_product.bkColor(RGB(255,255,255));
     m_data_loading.setFont(30,10,NULL,_T("Arial"));

    g_hwnd_now = this->m_hWnd;
    

    CRect ViewRect;
    GetClientRect(&ViewRect);


    if (m_RegisterListCtrl.GetSafeHwnd())
    {
        m_RegisterListCtrl.MoveWindow(CRect(260,40,ViewRect.Width(),ViewRect.Height()),TRUE);
    }
     if (m_Product_ListCtrl.GetSafeHwnd())
     {
         m_Product_ListCtrl.MoveWindow(CRect(0,40,255,ViewRect.Height()),TRUE); 
     }

     m_label_product.SetWindowTextW(_T("Product Model List"));
     m_label_product.textColor(RGB(0,0,255));
     //m_current_product.bkColor(RGB(255,255,255));
     m_label_product.setFont(30,10,NULL,_T("Arial"));

     m_label_registerlist.SetWindowTextW(_T("Registers List"));
     m_label_registerlist.textColor(RGB(0,0,255));
     //m_current_product.bkColor(RGB(255,255,255));
     m_label_registerlist.setFont(30,10,NULL,_T("Arial"));


      

	// TODO: Add extra initialization here
     Load_ProductList();
     Initial_ProductList();
     g_current_product = g_product_table.at(0);
     Load_RegisterList();
     Initial_RegistersList();

    

	 return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRegisterListManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRegisterListManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRegisterListManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRegisterListManagerDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    if (nType==SIZE_RESTORED)
    {
    CRect ViewRect;
    GetClientRect(&ViewRect);
    if (m_RegisterListCtrl.GetSafeHwnd())
    {
        m_RegisterListCtrl.MoveWindow(CRect(260,40,ViewRect.Width(),ViewRect.Height()),TRUE);
    }
    if (m_Product_ListCtrl.GetSafeHwnd())
    {
        m_Product_ListCtrl.MoveWindow(CRect(0,40,255,ViewRect.Height()),TRUE); 
    }
   }
    // TODO: Add your message handler code here
}

void CRegisterListManagerDlg::Load_ProductList(){
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
void CRegisterListManagerDlg::Load_RegisterList(){
    CString StrSQL;
    CString StrTemp;
    Register_Table_Struct temp_register;
    _variant_t temp_variant  ;                   
    g_registerlist_table.clear();
    StrSQL.Format(_T("Select * from %s"),
        g_current_product.TableName);
     g_database_operator.m_pRecordset = g_database_operator.OpenRecordset(StrSQL);
     while(VARIANT_FALSE==g_database_operator.m_pRecordset->EndOfFile){
         temp_variant=g_database_operator.m_pRecordset->GetCollect(g_current_product.Col_RegAddress.GetBuffer());//
         if(temp_variant.vt!=VT_NULL)
             StrTemp=temp_variant;
         else
             StrTemp=_T("");

         temp_register.Reg_Add =StrTemp;

         temp_variant=g_database_operator.m_pRecordset->GetCollect(g_current_product.Col_RegName.GetBuffer());//
         if(temp_variant.vt!=VT_NULL)
             StrTemp=temp_variant;
         else
             StrTemp=_T("");

         temp_register.Reg_Name =StrTemp;

         temp_variant=g_database_operator.m_pRecordset->GetCollect(g_current_product.Col_RegDescription.GetBuffer());//
         if(temp_variant.vt!=VT_NULL)
             StrTemp=temp_variant;
         else
             StrTemp=_T("");

         temp_register.Reg_Dsp =StrTemp;
         if (temp_register.Reg_Name.IsEmpty()|| temp_register.Reg_Name.CompareNoCase(_T("RESERVED")) == 0)
         {
             g_database_operator.m_pRecordset->MoveNext();
         }
         else
         {
             g_registerlist_table.push_back(temp_register);
             g_database_operator.m_pRecordset->MoveNext();
         }
         
     }


}
void CRegisterListManagerDlg::Initial_ProductList(){

    CString strTemp;
    m_Product_ListCtrl.ShowWindow(SW_HIDE);
    m_Product_ListCtrl.DeleteAllItems();
    while ( m_Product_ListCtrl.DeleteColumn (0)) ;
    m_Product_ListCtrl.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    m_Product_ListCtrl.SetExtendedStyle(m_Product_ListCtrl.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
   // m_Product_ListCtrl.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    m_Product_ListCtrl.InsertColumn(0, _T("Model NO"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_Product_ListCtrl.InsertColumn(1, _T("Product Name"), 200, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    for (int i = 0;i<(int)g_product_table.size();i++)
    {
        
      //  m_Product_ListCtrl.InsertItem(i,strTemp);
        m_Product_ListCtrl.InsertItem(i,g_product_table.at(i).ProductType);
        m_Product_ListCtrl.SetItemText(i,1,g_product_table.at(i).Product_Name);
    }
     m_Product_ListCtrl.ShowWindow(SW_SHOW);
}
void CRegisterListManagerDlg::Initial_RegistersList(){
    
    m_RegisterListCtrl.ShowWindow(SW_HIDE);
    m_RegisterListCtrl.DeleteAllItems();
    while ( m_RegisterListCtrl.DeleteColumn (0)) ;
    m_RegisterListCtrl.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    m_RegisterListCtrl.SetExtendedStyle(m_RegisterListCtrl.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
    m_RegisterListCtrl.InsertColumn(0, _T("Address"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_RegisterListCtrl.InsertColumn(1, _T("Name"), 350, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_RegisterListCtrl.InsertColumn(2, _T("Description"), 1000, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

    for (int i=0;i<(int)g_registerlist_table.size();i++)
    {
        m_RegisterListCtrl.InsertItem(i,g_registerlist_table.at(i).Reg_Add);
        m_RegisterListCtrl.SetItemText(i,1,g_registerlist_table.at(i).Reg_Name);
        m_RegisterListCtrl.SetItemText(i,2,g_registerlist_table.at(i).Reg_Dsp);
    }
     m_RegisterListCtrl.ShowWindow(SW_SHOW);
}


void CRegisterListManagerDlg::OnNMDblclkListProduct(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
     
    CString temp_cstring;
    long lRow,lCol;
    m_Product_ListCtrl.Set_Edit(false);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_Product_ListCtrl.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_Product_ListCtrl.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;
    

    if(lRow>m_Product_ListCtrl.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
        return;
    if(lRow<0)
        return;
     if (lCol == 0)
    {
        g_current_product = g_product_table.at(lRow);
        Load_RegisterList();
        SendMessage(WM_REFRESH_BAC_INPUT_LIST,1,0); 
    }
    

    *pResult = 0;
}
LRESULT CRegisterListManagerDlg::Fresh_Lists(WPARAM wParam,LPARAM lParam){
    CString strTemp;
    int fresh_input = (int) wParam;
 
   //  m_data_loading.ShowWindow(TRUE);
    if (fresh_input == 0)
    {     
         m_Product_ListCtrl.ShowWindow(SW_HIDE);
         m_data_loading.ShowWindow(SW_SHOW);
         m_Product_ListCtrl.DeleteAllItems();
         for (int i = 0;i<(int)g_product_table.size();i++)
         {
             m_Product_ListCtrl.InsertItem(i,g_product_table.at(i).ProductType);
             m_Product_ListCtrl.SetItemText(i,1,g_product_table.at(i).Product_Name);
         }
         m_data_loading.ShowWindow(SW_HIDE);
          m_Product_ListCtrl.ShowWindow(SW_SHOW);
        return 1;
    }

    if (fresh_input == 1)
    { m_RegisterListCtrl.ShowWindow(SW_HIDE);
      m_data_loading.ShowWindow(SW_SHOW);
     m_RegisterListCtrl.DeleteAllItems();
     for (int i=0;i<(int)g_registerlist_table.size();i++)
     {
              m_RegisterListCtrl.InsertItem(i,g_registerlist_table.at(i).Reg_Add);
              m_RegisterListCtrl.SetItemText(i,1,g_registerlist_table.at(i).Reg_Name);
              m_RegisterListCtrl.SetItemText(i,2,g_registerlist_table.at(i).Reg_Dsp);
     }
      m_data_loading.ShowWindow(SW_HIDE);
      m_RegisterListCtrl.ShowWindow(SW_SHOW);
    }
    //  m_data_loading.ShowWindow(FALSE);
    return 0;
}

void CRegisterListManagerDlg::OnNMDblclkListRegisterlist(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    CString temp_cstring;
    long lRow,lCol;
    m_RegisterListCtrl.Set_Edit(false);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_RegisterListCtrl.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_RegisterListCtrl.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;


    if(lRow>m_RegisterListCtrl.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
        return;
    if(lRow<0)
        return;
        g_current_register = g_registerlist_table.at(lRow);
       CRegisterInforDlg dlg;
       dlg.m_current_index = lRow;
       if (dlg.DoModal() == IDCANCEL)
       {
            if (g_Is_Database_Changed)
            {
                Load_RegisterList();
                SendMessage(WM_REFRESH_BAC_INPUT_LIST,1,0);
            }
             
       }
    *pResult = 0;
}


//void CAboutDlg::OnUpdateSystemChange(CCmdUI *pCmdUI)
//{
//    pCmdUI->Enable(g_login_user.Person_Roles.CompareNoCase(L"engineer") == 0);
//}


//void CAboutDlg::OnUpdateSystemProjectmanagerment(CCmdUI *pCmdUI)
//{
//   pCmdUI->Enable(g_login_user.Person_Roles.CompareNoCase(L"admin") == 0);
//}


//void CAboutDlg::OnSystemUsersmanagerment()
//{
//   pCmdUI->Enable(g_login_user.Person_Roles.CompareNoCase(L"admin") == 0) ;
//}


void CRegisterListManagerDlg::OnUpdateSystemChange(CCmdUI *pCmdUI)
{
    //pCmdUI->Enable(g_login_user.Person_Roles.CompareNoCase(L"engineer") == 0);
   // pCmdUI->Enable(FALSE);
}

 


void CRegisterListManagerDlg::OnUpdateSystemProjectmanagerment(CCmdUI *pCmdUI)
{
    //pCmdUI->Enable(g_login_user.Person_Roles.CompareNoCase(L"admin") == 0);
}


void CRegisterListManagerDlg::OnUpdateSystemUsersmanagerment(CCmdUI *pCmdUI)
{
   //pCmdUI->Enable(g_login_user.Person_Roles.CompareNoCase(L"admin") == 0);
}


void CRegisterListManagerDlg::OnSystemExit()
{
    CDialogEx::OnOK();
}


void CRegisterListManagerDlg::OnSystemExportregisterlists()
{
      CStdioFile* m_pFile;
      CString logstr=_T("");
      CString  Product_Head_File_Name;
      m_pFile = new CStdioFile;//txt
      CString HeadFold = GetExePath(true);
      HeadFold += _T("Product Head File");
      CreateDirectory(HeadFold,NULL);
      Product_Head_File_Name =HeadFold;
     // Product_Head_File_Name   = HeadFold+ g_current_product.Product_Name;
      Product_Head_File_Name.Format(_T("%s\\%s.h"),HeadFold,g_current_product.Product_Name);
      
      m_pFile->Open(Product_Head_File_Name.GetString(),CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate );
      logstr=_T("/*\n");
      m_pFile->WriteString(logstr);
      logstr.Format(_T("*\n  *	%s.h\n *	\n *	Written by %s\n *	 */\n"),g_current_product.Product_Name,g_current_product.user_name_login);
      m_pFile->WriteString(logstr);
      logstr=_T("#ifndef __REGISTER_LIST_H__\n");
      m_pFile->WriteString(logstr);
      logstr=_T("#define	__REGISTER_LIST_H__\n");
      m_pFile->WriteString(logstr);
      logstr=_T("typedef enum \n{");
      m_pFile->WriteString(logstr);
      for (int i= 0;i<(int)g_registerlist_table.size();i++)
      {
        if (i == (int)g_registerlist_table.size()-1)
        {
            logstr.Format(_T("		%s = %s      /*%s */\n"),
                g_registerlist_table.at(i).Reg_Name,
                g_registerlist_table.at(i).Reg_Add,
                g_registerlist_table.at(i).Reg_Dsp);
            m_pFile->WriteString(logstr);
        }
        else
        {
          logstr.Format(_T("		%s = %s,      /*%s */\n"),
          g_registerlist_table.at(i).Reg_Name,
          g_registerlist_table.at(i).Reg_Add,
          g_registerlist_table.at(i).Reg_Dsp);
           m_pFile->WriteString(logstr);
        }
      }
      logstr=_T("}SLAVE_MODBUS_LIST;\n\n\n#endif");
      m_pFile->WriteString(logstr);
      m_pFile->Flush();
      m_pFile->Close();
      delete m_pFile;
      m_pFile = NULL; 
      AfxMessageBox(_T("Execute successfully!")) ;  
}


void CRegisterListManagerDlg::OnSystemChange()
{   CString StrTemp;
    //AfxMessageBox(_T("OK"));
    BOOL Can_GO;
    if (g_login_user.Person_Roles.CompareNoCase(L"engineer")==0)
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
        StrTemp.Format(_T("User Name:'%s'  No Permission !"),g_login_user.Person_Name);
        AfxMessageBox(StrTemp);
        return;
    }
    if (Can_GO)
    {
        CChangePSWDlg dlg;
        dlg.DoModal();
    }
     
}


void CRegisterListManagerDlg::OnSystemUsersmanagerment()
{      CString StrTemp;
    BOOL Can_GO;
    if (g_login_user.Person_Roles.CompareNoCase(L"admin")==0)
    {
        Can_GO = TRUE;
    }
    else
    {
        Can_GO = FALSE;
    }
    if (!Can_GO)
    {
        StrTemp.Format(_T("User Name:'%s'  No Permission !"),g_login_user.Person_Name);
        AfxMessageBox(StrTemp);
        return;
    }
    if (Can_GO)
    {
        CUserManagermentDlg dlg;
        dlg.DoModal();
    }
   
    g_hwnd_now = this->m_hWnd;
}


void CRegisterListManagerDlg::OnSystemProjectmanagerment()
{
    CString StrTemp;
    BOOL Can_GO;
     if (g_login_user.Person_Roles.CompareNoCase(L"admin")==0)
     {
         Can_GO = TRUE;
     }
     else
     {
         Can_GO = FALSE;
     }
     if (!Can_GO)
     {
         StrTemp.Format(_T("User Name:'%s'  No Permission !"),g_login_user.Person_Name);
         AfxMessageBox(StrTemp);
         return;
     }
     if (Can_GO)
     {
        CProjectManagementDlg dlg;
        dlg.DoModal();
    }

    g_hwnd_now = this->m_hWnd; 
}


void CRegisterListManagerDlg::OnBnClickedMfcbuttonExportHeadFile()
{
   OnSystemExportregisterlists();
}


void CRegisterListManagerDlg::OnBnClickedMfcbuttonPassword()
{
    OnSystemChange();
}


void CRegisterListManagerDlg::OnBnClickedMfcbuttonUsersmanagerment()
{
   OnSystemUsersmanagerment();
}


void CRegisterListManagerDlg::OnBnClickedMfcbuttonProjectmanagerment()
{
   OnSystemProjectmanagerment();
}
