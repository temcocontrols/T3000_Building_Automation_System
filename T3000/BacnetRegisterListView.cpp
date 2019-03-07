// BacnetRegisterListView.cpp : ??????
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetRegisterListView.h"
#include "afxdialogex.h"
#include "BADO/BADO.h"
#include "global_function.h"
// CBacnetRegisterListView
CString cs_register_db_path;

IMPLEMENT_DYNAMIC(CBacnetRegisterListView, CDialogEx)

CBacnetRegisterListView::CBacnetRegisterListView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_REGISTER_LIST, pParent)
{
    m_max_reg = 100;
}

CBacnetRegisterListView::~CBacnetRegisterListView()
{
}

void CBacnetRegisterListView::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_REGISTER_VIEW, m_register_view);
}

#define  WM_REFRESH_REGISTER_LIST WM_USER + 203

BEGIN_MESSAGE_MAP(CBacnetRegisterListView, CDialogEx)
    ON_WM_SIZE()
    ON_MESSAGE(WM_REFRESH_REGISTER_LIST, Fresh_Register_List)
END_MESSAGE_MAP()

HANDLE h_read_reg_date_thread = NULL;


DWORD WINAPI  CBacnetRegisterListView::ReadRegDataThreadfun(LPVOID lpVoid)
{
    //Write_Config_Info
    CBacnetRegisterListView *pParent = (CBacnetRegisterListView *)lpVoid;

    


    //m_max_reg
    int section_part = 0;
    section_part = pParent->m_max_reg % 100 + 1;

    for (int i = 0; i < section_part; i++)
    {
        Read_Multi(g_tstat_id, &product_register_value[i * 100], i * 100, 100);
    }
    ::PostMessage(pParent->m_hWnd,WM_REFRESH_REGISTER_LIST, NULL, 1);

    h_read_reg_date_thread = NULL;
    return 0;


}

BOOL CBacnetRegisterListView::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    CString ApplicationFolder;
    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    ApplicationFolder.ReleaseBuffer();
    cs_register_db_path = ApplicationFolder + _T("\\ResourceFile\\RegistersListDB.mdb");
    InitialListData();
    
    if (h_read_reg_date_thread == NULL)
    {
        h_read_reg_date_thread = CreateThread(NULL, NULL, ReadRegDataThreadfun, this, NULL, NULL);
    }

    //Test_Read_Excel();
    return TRUE;  // return TRUE unless you set the focus to a control
}


BOOL CBacnetRegisterListView::PreTranslateMessage(MSG* pMsg)
{

    return CDialogEx::PreTranslateMessage(pMsg);
}


CString** CreateCStringArray(UINT colcount, UINT rowcount) 
{
    CString** strMsg = new CString*[colcount];
    for (UINT i = 0; i < colcount; i++)
    {
        strMsg[i] = new CString[rowcount];
    }
    return strMsg;
}


void DeleteCStringArray(UINT size, CString** pstr)
{
    for (unsigned int i = 0; i < size; i++)
    {
        delete[]  pstr[i];
    }
}

CString ** temp_cs;
long * col_width;
void CBacnetRegisterListView::Test_Read_Excel()
{
    CApplication0 app;
    CWorkbooks0 books;
    CWorkbook0 book;
    CWorksheets0 sheets;
    CWorksheet0 sheet;
    CRange0 range;
    CRange0 iCell;
    LPDISPATCH lpDisp;
    COleVariant vResult;
    COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
    if (!app.CreateDispatch(L"Excel.Application"))
    {
        AfxMessageBox(_T("Open excel failed!"));
        return;
    }
    CString excel_reglist_path;
    CString ApplicationFolder;
    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    ApplicationFolder.ReleaseBuffer();
    excel_reglist_path = ApplicationFolder + _T("\\ModbusBacnetRegistersList.xls");


    books.AttachDispatch(app.get_Workbooks());
    lpDisp = books.Open(excel_reglist_path,covOptional, covOptional, covOptional, covOptional, covOptional,covOptional, covOptional, covOptional, covOptional, covOptional,covOptional, covOptional, covOptional,covOptional);

    pid_reglist_map::iterator it;
    it = product_reglist_map.find(g_selected_product_id);
    if (it == product_reglist_map.end())
        return;

    CString temp_excel_name;
    temp_excel_name = product_reglist_map.at(g_selected_product_id);
    char temp_excel_tab_name[MAX_PATH];
    WideCharToMultiByte(CP_ACP, 0, temp_excel_name.GetBuffer(), -1, temp_excel_tab_name, MAX_PATH, NULL, NULL);


    book.AttachDispatch(lpDisp);
    sheets.AttachDispatch(book.get_Worksheets());
    sheet.AttachDispatch(sheets.get_Item(_variant_t(temp_excel_name)));
    lpDisp = sheets.get_Item(_variant_t(temp_excel_name));
    sheet.AttachDispatch(lpDisp);

    range.AttachDispatch(sheet.get_UsedRange());
    range.AttachDispatch(range.get_Rows());
    long iRowNum = range.get_Count();
    range.AttachDispatch(range.get_Columns());
    long iColNum = range.get_Count();

    //range.get_ColumnWidth();

   
    
    temp_cs = CreateCStringArray(iRowNum, iColNum);
    col_width = new long[iColNum];

    for (int x = 0; x < iColNum; x++)
    {
        range.AttachDispatch(sheet.get_Cells());
        range.AttachDispatch(range.get_Item(COleVariant((long)1), COleVariant((long)x + 1)).pdispVal);
        COleVariant vResult;
        vResult = range.get_ColumnWidth();
        if (vResult.vt == VT_R8)
        {
            long temp_int = 0;
            col_width[x] = vResult.dblVal;
            //temp_cs[i][j].Format(_T("%ld"), temp_int);
        }
        TRACE(_T("width %d =%d\r\n"), x+1, col_width[x]);
    }

    for (int i = 0; i < iRowNum; i++)
    {
        for (int j = 0;j < iColNum;j++)
        {
            range.AttachDispatch(sheet.get_Cells());
            range.AttachDispatch(range.get_Item(COleVariant((long)i+1), COleVariant((long)j+1)).pdispVal);
            vResult = range.get_Value2();
            range.get_Row();
            if (vResult.vt == VT_BSTR) 
            {
                temp_cs[i][j] = vResult.bstrVal;
                temp_cs[i][j].Replace(_T("\n"), _T(" "));
                temp_cs[i][j].Replace(_T("\r\n"), _T(" "));
            }
            else if (vResult.vt == VT_R8)
            {
                long temp_int = 0;
                temp_int = vResult.dblVal;
                //CString temp_cs1;
                temp_cs[i][j].Format(_T("%ld"), temp_int);
                //temp_cs[i][j] = temp_cs1;
            }

            //TRACE(_T("%03d %03d : %s\r\n"),i,j, temp_cs[i][j].GetBuffer());
        }

    }
    range.AttachDispatch(sheet.get_Cells());
    range.AttachDispatch(range.get_Item(COleVariant((long)5), COleVariant((long)4)).pdispVal);
    /*COleVariant*/ vResult = range.get_Value2();
    range.get_Row();

    
    CString str;
    if (vResult.vt == VT_BSTR) 
    {
        str = vResult.bstrVal;
    }
    else if (vResult.vt == VT_R8) 
    {
        str.Format(L"%f", vResult.dblVal);
    }

    /*else if(vResult.vt==VT_DATE)
    {
    SYSTEMTIME st;
    VariantTimeToSystemTime(&vResult.date, &st);
    }
    else if(vResult.vt==VT_EMPTY)
    {
    str="";
    }*/

//    books.Close();
    app.Quit(); 
    range.ReleaseDispatch();
    sheet.ReleaseDispatch();
    sheets.ReleaseDispatch();
    book.ReleaseDispatch();
    books.ReleaseDispatch();
    app.ReleaseDispatch();
   // MessageBox(str);

    Initial_List(iRowNum, iColNum);
    DeleteCStringArray(iRowNum, temp_cs);
    delete[] col_width;
}



void CBacnetRegisterListView::Initial_List(int nrow, int ncol)
{
    m_register_view.ShowWindow(SW_HIDE);
    m_register_view.DeleteAllItems();
    while (m_register_view.DeleteColumn(0));


    m_register_view.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
    m_register_view.SetExtendedStyle(m_register_view.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    for (int i = 0; i < ncol; i++)
    {
        m_register_view.InsertColumn(i, _T("   "), col_width[i]*6, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    }

    for (int j = 0; j < nrow; j++)
    {
        m_register_view.InsertItem(j, _T(" "));
    }

    for (int i = 0; i < nrow; i++)
    {
        for (int j = 0; j < ncol; j++)
        {
            m_register_view.SetItemText(i, j, temp_cs[i][j]);
        }

        if ((i % 2) == 0)
            m_register_view.SetItemBkColor(i, -1, LIST_ITEM_DEFAULT_BKCOLOR);
        else
            m_register_view.SetItemBkColor(i, -1, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);

    }

    m_register_view.ShowWindow(SW_SHOW);
}




void CBacnetRegisterListView::OnSize(UINT nType, int cx, int cy)
{
    CDialogEx::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码

    if (m_register_view.GetSafeHwnd() == NULL)   return;
    CRect rect;
    this->GetClientRect(&rect);
    CPoint pot = rect.TopLeft();
    m_register_view.SetWindowPos(NULL, pot.x + 10, pot.y + 10,
        rect.Width() - 30, rect.Height() - 20, NULL);
}


void CBacnetRegisterListView::InitialListData()
{
    Initial_List();
    Read_Data_From_DB();
    AddDataIntoList();
    PostMessage(WM_REFRESH_REGISTER_LIST, NULL, NULL);
}

typedef struct _str_data_format
{
    int n_id;
    int n_Data_Format_value;
    char cs_Data_Format[MAX_PATH];
    int n_operation;
    char cs_Operation[MAX_PATH];
    int n_product_id;
    char cs_Sheet_name[MAX_PATH];
}str_data_format;

struct str_register_db_data
{
    int auto_id;
    int m_register_address;
    char cs_operation[MAX_PATH];
    int m_register_length;
    char cs_register_name[MAX_PATH];
    char cs_data_format[MAX_PATH];
    char cs_description[5*MAX_PATH];
    int m_data_format;
};

str_data_format data_format[100];

str_register_db_data * register_dbdata = NULL;

void CBacnetRegisterListView::Read_Data_From_DB()
{
    CBADO monitor_bado;
    CString strSql;
    _variant_t temp_variant;
    monitor_bado.SetDBPath(cs_register_db_path);	//暂时不创建新数据库
    monitor_bado.OnInitADOConn();

    int temp_count = 0;
    strSql.Format(_T("select * from DataFormat"));
    monitor_bado.m_pRecordset = monitor_bado.OpenRecordset(strSql);
    m_data_format_count = monitor_bado.GetRecordCount(monitor_bado.m_pRecordset);
    if (m_data_format_count <= 0)
    {
        monitor_bado.CloseRecordset();//Ffff add
        return;
    }

    while (VARIANT_FALSE == monitor_bado.m_pRecordset->EndOfFile)
    {
        CString temp_cs;
        temp_variant = monitor_bado.m_pRecordset->GetCollect("Data_Format");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_cs = temp_variant;
            WideCharToMultiByte(CP_ACP, 0, temp_cs.GetBuffer(), -1, data_format[temp_count].cs_Data_Format, 255, NULL, NULL);
        }
        else
        {
            memset(data_format[temp_count].cs_Data_Format, 0, MAX_PATH);
            temp_count++;
            monitor_bado.m_pRecordset->MoveNext();
            continue;
        }

        temp_variant = monitor_bado.m_pRecordset->GetCollect("Operation");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_cs = temp_variant;
            WideCharToMultiByte(CP_ACP, 0, temp_cs.GetBuffer(), -1, data_format[temp_count].cs_Operation, 255, NULL, NULL);
        }
        else
        {
            memset(data_format[temp_count].cs_Operation, 0, MAX_PATH);
            temp_count++;
            monitor_bado.m_pRecordset->MoveNext();
            continue;
        }



        temp_variant = monitor_bado.m_pRecordset->GetCollect("Sheet_Name");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_cs = temp_variant;
            WideCharToMultiByte(CP_ACP, 0, temp_cs.GetBuffer(), -1, data_format[temp_count].cs_Sheet_name, 255, NULL, NULL);
        }
        else
        {
            memset(data_format[temp_count].cs_Sheet_name, 0, MAX_PATH);
            temp_count++;
            monitor_bado.m_pRecordset->MoveNext();
            continue;
        }

        temp_variant = monitor_bado.m_pRecordset->GetCollect("Product_ID");//
        if (temp_variant.vt != VT_NULL)
            data_format[temp_count].n_product_id = temp_variant;
        else
        {
            data_format[temp_count].n_product_id = 0;
            temp_count++;
            monitor_bado.m_pRecordset->MoveNext();
            continue;
        }


        temp_count++;
        monitor_bado.m_pRecordset->MoveNext();
    }

    CString Sheet_Name;

    for (int i = 0; i < temp_count; i++)
    {
        if (g_selected_product_id == data_format[i].n_product_id)
        {
            Sheet_Name = data_format[i].cs_Sheet_name;
        }
    }

    if (Sheet_Name.IsEmpty())
    {
        PostMessage(WM_CLOSE, NULL, NULL);
        return ;
    }

    strSql.Format(_T("select * from  %s"), Sheet_Name);
    monitor_bado.m_pRecordset = monitor_bado.OpenRecordset(strSql);
    m_record_count = monitor_bado.GetRecordCount(monitor_bado.m_pRecordset);


    if (m_record_count <= 0)
    {
        monitor_bado.CloseRecordset();//Ffff add
        return;
    }

    register_dbdata = new str_register_db_data[m_record_count];




    unsigned int logging_time = 0;
    int monitor_value = 0;
    temp_count = 0;
    while (VARIANT_FALSE == monitor_bado.m_pRecordset->EndOfFile)
    {
        CString temp_cs;
        temp_variant = monitor_bado.m_pRecordset->GetCollect("ID");//
        if (temp_variant.vt != VT_NULL)
            register_dbdata[temp_count].auto_id = temp_variant;
        else
        {
            register_dbdata[temp_count].auto_id = 0;
            temp_count++;
            monitor_bado.m_pRecordset->MoveNext();
            continue;
        }

        temp_variant = monitor_bado.m_pRecordset->GetCollect("Register_Address");//
        if (temp_variant.vt != VT_NULL)
            register_dbdata[temp_count].m_register_address = temp_variant;
        else
        {
            register_dbdata[temp_count].m_register_address = 0;
            temp_count++;
            monitor_bado.m_pRecordset->MoveNext();
            continue;
        }

        //temp_variant = monitor_bado.m_pRecordset->GetCollect("Operation");//
        //if (temp_variant.vt != VT_NULL)
        //    register_dbdata[temp_count].cs_operation = temp_variant;
        //else
        //{
        //    register_dbdata[temp_count].cs_operation.Empty();
        //    temp_count++;
        //    monitor_bado.m_pRecordset->MoveNext();
        //    continue;
        //}
        temp_variant = monitor_bado.m_pRecordset->GetCollect("Operation");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_cs = temp_variant;
            WideCharToMultiByte(CP_ACP, 0, temp_cs.GetBuffer(), -1, register_dbdata[temp_count].cs_operation, MAX_PATH, NULL, NULL);
        }
        else
        {
            memset(register_dbdata[temp_count].cs_operation, 0, MAX_PATH);
            temp_count++;
            monitor_bado.m_pRecordset->MoveNext();
            continue;
        }


        temp_variant = monitor_bado.m_pRecordset->GetCollect("Register_Length");//
        if (temp_variant.vt != VT_NULL)
            register_dbdata[temp_count].m_register_length = temp_variant;
        else
        {
            register_dbdata[temp_count].m_register_length = 0;
            temp_count++;
            monitor_bado.m_pRecordset->MoveNext();
            continue;
        }

        //temp_variant = monitor_bado.m_pRecordset->GetCollect("Register_Name");//
        //if (temp_variant.vt != VT_NULL)
        //    register_dbdata[temp_count].cs_register_name = temp_variant;
        //else
        //{
        //    register_dbdata[temp_count].cs_register_name.Empty();
        //    temp_count++;
        //    monitor_bado.m_pRecordset->MoveNext();
        //    continue;
        //}

        temp_variant = monitor_bado.m_pRecordset->GetCollect("Register_Name");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_cs = temp_variant;
            WideCharToMultiByte(CP_ACP, 0, temp_cs.GetBuffer(), -1, register_dbdata[temp_count].cs_register_name, 255, NULL, NULL);
        }
        else
        {
            memset(register_dbdata[temp_count].cs_register_name, 0, MAX_PATH);
            temp_count++;
            monitor_bado.m_pRecordset->MoveNext();
            continue;
        }


        //temp_variant = monitor_bado.m_pRecordset->GetCollect("Data_Format");//
        //if (temp_variant.vt != VT_NULL)
        //    register_dbdata[temp_count].cs_data_format = temp_variant;
        //else
        //{
        //    register_dbdata[temp_count].cs_data_format.Empty();
        //    temp_count++;
        //    monitor_bado.m_pRecordset->MoveNext();
        //    continue;
        //}

        temp_variant = monitor_bado.m_pRecordset->GetCollect("Data_Format");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_cs = temp_variant;
            register_dbdata[temp_count].m_data_format = REGISTER_UNKNOWN;
            WideCharToMultiByte(CP_ACP, 0, temp_cs.GetBuffer(), -1, register_dbdata[temp_count].cs_data_format, 255, NULL, NULL);
            for (int x = 0; x < sizeof(RegisterView_Format) / sizeof(RegisterView_Format[0]); x++)
            {
                if (temp_cs.CompareNoCase(RegisterView_Format[x]) == 0)
                {
                    register_dbdata[temp_count].m_data_format = x;
                }
            }
        }
        else
        {
            memset(register_dbdata[temp_count].cs_data_format, 0, MAX_PATH);
            register_dbdata[temp_count].m_data_format = REGISTER_UNKNOWN;
            temp_count++;
            monitor_bado.m_pRecordset->MoveNext();
            continue;
        }


        //temp_variant = monitor_bado.m_pRecordset->GetCollect("Description");//
        //if (temp_variant.vt != VT_NULL)
        //    register_dbdata[temp_count].cs_description = temp_variant;
        //else
        //{
        //    register_dbdata[temp_count].cs_description.Empty();
        //    temp_count++;
        //    monitor_bado.m_pRecordset->MoveNext();
        //    continue;
        //}


        temp_variant = monitor_bado.m_pRecordset->GetCollect("Description");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_cs = temp_variant;
            WideCharToMultiByte(CP_ACP, 0, temp_cs.GetBuffer(), -1, register_dbdata[temp_count].cs_description, 5*MAX_PATH, NULL, NULL);
        }
        else
        {
            memset(register_dbdata[temp_count].cs_description, 0, 5 * MAX_PATH);
            temp_count++;
            monitor_bado.m_pRecordset->MoveNext();
            continue;
        }


        if (m_max_reg <= register_dbdata[temp_count].m_register_address)
            m_max_reg = register_dbdata[temp_count].m_register_address;
        temp_count++;
        monitor_bado.m_pRecordset->MoveNext();
    }

    monitor_bado.CloseConn();
}



void CBacnetRegisterListView::Initial_List()
{
    m_register_view.ShowWindow(SW_HIDE);
    m_register_view.DeleteAllItems();
    while (m_register_view.DeleteColumn(0));

    m_register_view.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
    //m_register_view.SetExtendedStyle(m_register_view.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
    m_register_view.SetExtendedStyle(m_register_view.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_register_view.InsertColumn(REGISTER_LIST_ID, _T("ID"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
    m_register_view.InsertColumn(REGISTER_LIST_ADDRESS, _T("Reg_Address"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_register_view.InsertColumn(REGUSTER_LIST_OPERATION, _T("Operation"), 130, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_register_view.InsertColumn(REGISTER_LIST_REG_LENGTH, _T("Reg_Length"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_register_view.InsertColumn(REGISTER_LIST_REG_NAME, _T("Register_Name"), 140, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_register_view.InsertColumn(REGISTER_LIST_VALUE, _T("Value"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_register_view.InsertColumn(REGISTER_LIST_DATA_FORMAT, _T("Data_Format"), 140, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_register_view.InsertColumn(REGISTER_LIST_DESCRIPTION, _T("Description"), 350, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
    m_pragram_dlg_hwnd = this->m_hWnd;
    m_register_view.SetListHwnd(this->m_hWnd);
    CRect list_rect, win_rect;
    m_register_view.GetWindowRect(list_rect);
    //m_register_view.Dont_DT_Left();
    ScreenToClient(&list_rect);
    ::GetWindowRect(m_pragram_dlg_hwnd, win_rect);
    m_register_view.Set_My_WindowRect(win_rect);
    m_register_view.Set_My_ListRect(list_rect);

    m_register_view.DeleteAllItems();

    m_register_view.ShowWindow(SW_SHOW);
}

void CBacnetRegisterListView::AddDataIntoList()
{
    m_register_view.DeleteAllItems();
    for (int i = 0; i < m_record_count; i++)
    {
        CString temp_item;
        temp_item.Format(_T("%d"), i + 1);
        m_register_view.InsertItem(i, temp_item);

    }

}


LRESULT CBacnetRegisterListView::Fresh_Register_List(WPARAM wParam, LPARAM lParam)
{
    int l_param = (int)lParam;
    for (int i = 0;i < m_record_count;i++)
    {
        CString temp_id;
        temp_id.Format(_T("%d"), register_dbdata[i].auto_id);

        CString temp_register_address;
        temp_register_address.Format(_T("%d"), register_dbdata[i].m_register_address);

        CString temp_register_length;
        temp_register_length.Format(_T("%d"), register_dbdata[i].m_register_length);

        CString n_value;
        int n_start_add = register_dbdata[i].m_register_address;
        if (n_start_add >= 20000)
        {
            continue;
        }

        n_value.Format(_T("%d"), product_register_value[n_start_add]);
#if 1

        switch (register_dbdata[i].m_data_format)
        {
        case REGISTER_UNKNOWN:
        {
            break;
        }
        case REGISTER_8_BIT_UNSIGNED_INTEGER:
        {
            n_value.Format(_T("%u"), (unsigned char)product_register_value[n_start_add]);
            break;
        }
        case REGISTER_8_BIT_SIGNED_INTEGER:
        {
            n_value.Format(_T("%d"), (char)product_register_value[n_start_add]);
            break;
        }
        case REGISTER_16_BIT_UNSIGNED_INTEGER:
        {
            n_value.Format(_T("%u"), (unsigned short)product_register_value[n_start_add]);
            break;
        }
        case REGISTER_16_BIT_SIGNED_INTEGER:
        {
            n_value.Format(_T("%d"), (short)product_register_value[n_start_add]);
            break;
        }
        case REGISTER_32_BIT_UNSIGNED_INTEGER_HI_LO: 
        {  
            if (register_dbdata[i].m_register_length == 2)
            {
                unsigned short temp1 = 0;
                unsigned short temp2 = 0;
                temp1 = htons(product_register_value[n_start_add]);
                temp2 = htons(product_register_value[n_start_add + 1]);
                unsigned int n_temp_int = 0;
                n_temp_int = temp1 + temp2 * 65536;
                n_value.Format(_T("%u"), n_temp_int);
            }
            else if (register_dbdata[i].m_register_length == 4)
            {
                unsigned int n_temp_int = 0;
                n_temp_int = product_register_value[n_start_add] + product_register_value[n_start_add + 1] * 256 + product_register_value[n_start_add + 2] * 256 * 256 + product_register_value[n_start_add + 3] * 256 * 256 * 256;
                n_value.Format(_T("%u"), n_temp_int);
            }
            break;
        }
        case REGISTER_32_BIT_UNSIGNED_INTEGER_LO_HI: 
        {  
            if (register_dbdata[i].m_register_length == 2)
            {
                unsigned short temp1 = 0;
                unsigned short temp2 = 0;
                unsigned int n_temp_int = 0;
                n_temp_int = temp1 + temp2 * 65536;
                n_value.Format(_T("%u"), n_temp_int);
            }
            else if (register_dbdata[i].m_register_length == 4)
            {
                unsigned int n_temp_int = 0;
                n_temp_int = product_register_value[n_start_add] + product_register_value[n_start_add + 1] * 256 + product_register_value[n_start_add + 2] * 256 * 256 + product_register_value[n_start_add + 3] * 256 * 256 * 256;
                n_value.Format(_T("%u"), n_temp_int);
            }
            break;
        }
        case REGISTER_32_BIT_SIGNED_INTEGER_HI_LO: 
        {  
            break;
        }
        case REGISTER_32_BIT_SIGNED_INTEGER_LO_HI: 
        {  
            break;
        }
        case REGISTER_FLOATING_HI_LO_DIV_10: 
        {  
            break;
        }
        case REGISTER_FLOATING_LO_HI_DIV_10: 
        {  
            break;
        }
        case REGISTER_FLOATING_HI_LO_DIV_100: 
        {  
            break;
        }
        case REGISTER_FLOATING_LO_HI_DIV_100: 
        {  
            break;
        }
        case REGISTER_FLOATING_HI_LO_DIV_1000: 
        {  
            break;
        }
        case REGISTER_FLOATING_LO_HI_DIV_1000: 
        {  
            break;
        }
        case REGISTER_CHARACTER_STRING_HI_LO: 
        {  
            break;
        }
        case REGISTER_CHARACTER_STRING_LO_HI: 
        {  
            break;
        }
        default :
            break;
        }

#endif




        CString cs_operation;
        MultiByteToWideChar(CP_ACP, 0, (char *)register_dbdata[i].cs_operation,
            (int)strlen((char *)register_dbdata[i].cs_operation) + 1,
            cs_operation.GetBuffer(MAX_PATH), MAX_PATH);
        cs_operation.ReleaseBuffer();

        CString cs_register_name;
        MultiByteToWideChar(CP_ACP, 0, (char *)register_dbdata[i].cs_register_name,
            (int)strlen((char *)register_dbdata[i].cs_register_name) + 1,
            cs_register_name.GetBuffer(MAX_PATH), MAX_PATH);
        cs_register_name.ReleaseBuffer();

        CString cs_data_format;
        MultiByteToWideChar(CP_ACP, 0, (char *)register_dbdata[i].cs_data_format,
            (int)strlen((char *)register_dbdata[i].cs_data_format) + 1,
            cs_data_format.GetBuffer(MAX_PATH), MAX_PATH);
        cs_data_format.ReleaseBuffer();

        CString cs_description;
        MultiByteToWideChar(CP_ACP, 0, (char *)register_dbdata[i].cs_description,
            (int)strlen((char *)register_dbdata[i].cs_description) + 1,
            cs_description.GetBuffer(5*MAX_PATH), 5*MAX_PATH);
        cs_description.ReleaseBuffer();
        if (l_param != 1)
        {
            m_register_view.SetItemText(i, REGISTER_LIST_ID, temp_id);
            m_register_view.SetItemText(i, REGISTER_LIST_ADDRESS, temp_register_address);
            m_register_view.SetItemText(i, REGUSTER_LIST_OPERATION, cs_operation);
            m_register_view.SetItemText(i, REGISTER_LIST_REG_LENGTH, temp_register_length);
            m_register_view.SetItemText(i, REGISTER_LIST_REG_NAME, cs_register_name);
            m_register_view.SetItemText(i, REGISTER_LIST_VALUE, n_value);
            m_register_view.SetItemText(i, REGISTER_LIST_DATA_FORMAT, cs_data_format);
            m_register_view.SetItemText(i, REGISTER_LIST_DESCRIPTION, cs_description);
        }
        else
        {
            m_register_view.SetItemText(i, REGISTER_LIST_VALUE, n_value);
        }





        if (l_param != 1)
        {
            if ((i % 2) == 0)
                m_register_view.SetItemBkColor(i, -1, LIST_ITEM_DEFAULT_BKCOLOR);
            else
                m_register_view.SetItemBkColor(i, -1, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
        }


        //m_register_view.
    }

    return 0;
}
