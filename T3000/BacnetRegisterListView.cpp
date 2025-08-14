﻿// BacnetRegisterListView.cpp : ??????
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetRegisterListView.h"
#include "afxdialogex.h"
#include "BADO/BADO.h"
#include "global_function.h"
// CBacnetRegisterListView
#include "MainFrm.h"

IMPLEMENT_DYNAMIC(CBacnetRegisterListView, CDialogEx)

//CBacnetRegisterListView::CBacnetRegisterListView(int nmode = 0,CWnd* pParent )
//    : CDialogEx(IDD_DIALOG_REGISTER_LIST, pParent)
//{
//    m_max_reg = 200;
//}

CString m_third_db_path;
CBacnetRegisterListView::CBacnetRegisterListView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_REGISTER_LIST, pParent)
{
    m_third_party_db = 0;
    m_max_reg = 100;
    m_record_count = 0;
    m_device_mode = 0;
}

CBacnetRegisterListView::~CBacnetRegisterListView()
{
}
void CBacnetRegisterListView::SetDeviceMode(int n_device_mode)
{
    m_device_mode = n_device_mode;
}

void CBacnetRegisterListView::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_REGISTER_VIEW, m_register_view);
    //DDX_Control(pDX, ID_THIRD_LIST_DELETE, m_DeleteListButton);
    //DDX_Control(pDX, ID_THIRD_LIST_AND,    m_DeleteListAdd);
}

#define  WM_REFRESH_REGISTER_LIST WM_USER + 203

BEGIN_MESSAGE_MAP(CBacnetRegisterListView, CDialogEx)
    ON_WM_SIZE()
    ON_MESSAGE(WM_REFRESH_REGISTER_LIST, Fresh_Register_List)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED, Fresh_Register_Item)
    ON_WM_CLOSE()
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_REGISTER_VIEW, &CBacnetRegisterListView::OnLvnItemchangedListRegisterView)
    ON_NOTIFY(NM_CLICK, IDC_LIST_REGISTER_VIEW, &CBacnetRegisterListView::OnNMClickListRegisterView)
    ON_BN_CLICKED(IDC_BUTTON_CREATE_DB, &CBacnetRegisterListView::OnBnClickedButtonCreateDb)
    ON_BN_CLICKED(IDC_BUTTON_THIRD_SAVE, &CBacnetRegisterListView::OnBnClickedButtonThirdSave)
    ON_NOTIFY(NM_RCLICK, IDC_LIST_REGISTER_VIEW, &CBacnetRegisterListView::OnNMRClickListRegisterView)
    ON_COMMAND(ID_THIRD_LIST_DELETE, &CBacnetRegisterListView::OnBnClickedListDelete)
    ON_COMMAND(ID_THIRD_LIST_AND, &CBacnetRegisterListView::OnBnClickedListAdd)
    ON_CBN_SELCHANGE(IDC_COMBO_REGISTER_DB, &CBacnetRegisterListView::OnCbnSelchangeComboRegisterDb)
    ON_BN_CLICKED(IDC_BUTTON_DELETE_SELECTED_DB, &CBacnetRegisterListView::OnBnClickedButtonDeleteSelectedDb)
END_MESSAGE_MAP()

void CBacnetRegisterListView::OnBnClickedListDelete()
{
    // Add code to pop up message box
    //添加代码弹出消息框
    // Delete the row clicked by mouse in m_register_view
    //删除 m_register_view 鼠标点击的这一行

    MessageBox(_T("Delete"));

}

void CBacnetRegisterListView::OnBnClickedListAdd()
{
    // Add a new row to m_register_view
    //新增m_register_view 一行
    m_register_view.InsertItem(m_register_view.GetItemCount(), _T(" "));
    //MessageBox(_T("Add"));
}

HANDLE h_read_reg_date_thread = NULL;
typedef union
{
    float floatvalue;
    char chardata[4];
}Float_Char_Data;

DWORD WINAPI  CBacnetRegisterListView::ReadRegDataThreadfun(LPVOID lpVoid)
{
    //Write_Config_Info
    CBacnetRegisterListView *pParent = (CBacnetRegisterListView *)lpVoid;

    


    //m_max_reg
    int section_part = 0;
    section_part = pParent->m_max_reg / 100 + 1;
    int n_error_count = 0;
    for (int i = 0; i < section_part; i++)
    {
        int temp_read_ret = 0;
        temp_read_ret = Read_Multi(g_tstat_id, &product_register_value[i * 100], i * 100, 100);
        if (temp_read_ret < 0)
            n_error_count++;
        if (n_error_count >= 3)
        {
            ::PostMessage(pParent->m_hWnd, WM_REFRESH_REGISTER_LIST, NULL, 1);
            h_read_reg_date_thread = NULL;
            return 1;
        }
        if ((product_register_value[7] == PM_MINIPANEL) || 
            (product_register_value[7] == PM_MINIPANEL_ARM) ||
            (product_register_value[7] == PM_ESP32_T3_SERIES)
            )
        {
            if (i == 4)
                i = 68; //
            //如果是T3 中间很多都不用读;
        }
    }
    ::PostMessage(pParent->m_hWnd,WM_REFRESH_REGISTER_LIST, NULL, 1);

    h_read_reg_date_thread = NULL;
    return 0;


}

BOOL CBacnetRegisterListView::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	m_third_db_path = g_strExePth + _T("Database\\Buildings\\") + pFrame->m_strCurMainBuildingName + _T("\\ThirdPartyRegistersListDB.mdb");
	InitialListData();
	if (m_device_mode == 0)
	{
		if (h_read_reg_date_thread == NULL)
		{
			h_read_reg_date_thread = CreateThread(NULL, NULL, ReadRegDataThreadfun, this, NULL, NULL);
		}
	}
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
        rect.Width() - 30, rect.Height() - 65, NULL);
}


void CBacnetRegisterListView::InitialListData()
{
    Initial_List();
    if (m_device_mode)
    {

    }
    else
    {
        m_record_count = Read_Data_From_DB(&m_max_reg);
        if (m_record_count <= 0)
        {
			MessageBox(_T("The register list for the corresponding product was not found !"));
			return;
		}
		AddDataIntoList();
		PostMessage(WM_REFRESH_REGISTER_LIST, NULL, NULL);
    }
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


str_data_format data_format[100];

str_register_db_data * register_dbdata = NULL;

//void CBacnetRegisterListView::Read_Data_From_DB()
int Read_Data_From_DB(int* m_max_retreg)
{
    int m_max_reg = 0;
    int temp_record_count = 0;
    int m_data_format_count;
    CBADO monitor_bado;
    CString strSql;
    _variant_t temp_variant;
    CString cs_register_db_path;
    CString ApplicationFolder;
    GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
    PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
    ApplicationFolder.ReleaseBuffer();
    cs_register_db_path = ApplicationFolder + _T("\\ResourceFile\\RegistersListDB.mdb");

    monitor_bado.SetDBPath(cs_register_db_path);	//暂时不创建新数据库
    monitor_bado.OnInitADOConn();

    int temp_count = 0;
    strSql.Format(_T("select * from DataFormat"));
    monitor_bado.m_pRecordset = monitor_bado.OpenRecordset(strSql);
    m_data_format_count = monitor_bado.GetRecordCount(monitor_bado.m_pRecordset);
    if (m_data_format_count <= 0)
    {
        monitor_bado.CloseRecordset();//Ffff add
        return -1;
    }

    while (VARIANT_FALSE == monitor_bado.m_pRecordset->EndOfFile)
    {
        CString temp_cs;

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
        //PostMessage(WM_CLOSE, NULL, NULL);
        return -2;
    }

    strSql.Format(_T("select * from  %s"), Sheet_Name);
    monitor_bado.m_pRecordset = monitor_bado.OpenRecordset(strSql);
    temp_record_count = monitor_bado.GetRecordCount(monitor_bado.m_pRecordset);


    if (temp_record_count <= 0)
    {
        monitor_bado.CloseRecordset();//Ffff add
        //MessageBox(_T("The register list for the corresponding product was not found !"));
        SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("The register list for the corresponding product was not found !"));
        return -3;
    }

    register_dbdata = new str_register_db_data[temp_record_count];




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

        if (register_dbdata[temp_count].m_register_address < 10000) //不想读几万个，等那么久
        {
            if (m_max_reg <= register_dbdata[temp_count].m_register_address)
                m_max_reg = register_dbdata[temp_count].m_register_address;
        }
        temp_count++;
        monitor_bado.m_pRecordset->MoveNext();
    }
    *m_max_retreg = m_max_reg;
    monitor_bado.CloseRecordset();
    monitor_bado.CloseConn();
    return temp_record_count;
}

void CBacnetRegisterListView::ShowThirdPartyWindow(int nCmdShow)
{
    GetDlgItem(IDC_STATIC_THIRD1)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_EDIT_REGISTER_DB_NAME)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_STATIC_REGISTER_SELECT)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_COMBO_REGISTER_DB)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_BUTTON_CREATE_DB)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_BUTTON_DELETE_SELECTED_DB)->ShowWindow(nCmdShow);
    GetDlgItem(IDC_BUTTON_THIRD_SAVE)->ShowWindow(nCmdShow);
}

struct str_third_db_str
{
    int reg_address;
    CString reg_operation;
    int reg_length;
    CString reg_name;
    CString reg_data_formate;
    CString reg_description;
    CString reg_unit;
};

vector <str_third_db_str> m_thir_db_data;

void CBacnetRegisterListView::QueryTable(CString n_device_db_name)
{
    m_thir_db_data.clear();
    CBADO third_db_bado;
    third_db_bado.SetDBPath(m_third_db_path);	//暂时不创建新数据库
    third_db_bado.OnInitADOConn();
    int temp_record_count = 0;
    CString strSql;
    strSql.Format(_T("select * from ThirdPartyRegister where Device_DB_Name = '%s' ORDER BY Register_Address ASC"), n_device_db_name);
    third_db_bado.m_pRecordset = third_db_bado.OpenRecordset(strSql);
    temp_record_count = third_db_bado.GetRecordCount(third_db_bado.m_pRecordset);
    if (temp_record_count <= 0)
    {
        third_db_bado.CloseConn();
        return;
    }
    _variant_t temp_variant;
    while (VARIANT_FALSE == third_db_bado.m_pRecordset->EndOfFile)
    {
        str_third_db_str temp_str;
        temp_variant = third_db_bado.m_pRecordset->GetCollect("Register_Address");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_str.reg_address = temp_variant;
        }
        else
        {
            third_db_bado.m_pRecordset->MoveNext();
            continue;
        }

        temp_variant = third_db_bado.m_pRecordset->GetCollect("Operation");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_str.reg_operation = temp_variant;
        }
        else
        {
            third_db_bado.m_pRecordset->MoveNext();
            continue;
        }

        temp_variant = third_db_bado.m_pRecordset->GetCollect("Register_Length");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_str.reg_length = temp_variant;
        }
        else
        {
            third_db_bado.m_pRecordset->MoveNext();
            continue;
        }

        temp_variant = third_db_bado.m_pRecordset->GetCollect("Register_Name");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_str.reg_name = temp_variant;
        }
        else
        {
            third_db_bado.m_pRecordset->MoveNext();
            continue;
        }

        temp_variant = third_db_bado.m_pRecordset->GetCollect("Data_Format");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_str.reg_data_formate = temp_variant;
        }
        else
        {
            third_db_bado.m_pRecordset->MoveNext();
            continue;
        }

        temp_variant = third_db_bado.m_pRecordset->GetCollect("Description");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_str.reg_description = temp_variant;
        }
        else
        {
            third_db_bado.m_pRecordset->MoveNext();
            continue;
        }

        temp_variant = third_db_bado.m_pRecordset->GetCollect("Unit");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_str.reg_unit = temp_variant;
        }
        else
        {
            third_db_bado.m_pRecordset->MoveNext();
            continue;
        }
        m_thir_db_data.push_back(temp_str);
        third_db_bado.m_pRecordset->MoveNext();
    }
    third_db_bado.CloseRecordset();//Ffff add
    third_db_bado.CloseConn();

    PostMessage(WM_REFRESH_REGISTER_LIST, NULL, NULL);
    GetDlgItem(IDC_EDIT_REGISTER_DB_NAME)->SetWindowTextW(n_device_db_name);
}
void CBacnetRegisterListView::QueryAllDbDeviceName()
{
    vector_third_db_name.clear();
    CBADO monitor_bado;
    monitor_bado.SetDBPath(m_third_db_path);	//暂时不创建新数据库
    monitor_bado.OnInitADOConn();
    CString strSql;
    strSql.Format(_T("select DISTINCT Device_DB_Name from ThirdPartyRegister "));
    unsigned int temp_record_count = 0;
    monitor_bado.m_pRecordset = monitor_bado.OpenRecordset(strSql);
    temp_record_count = monitor_bado.GetRecordCount(monitor_bado.m_pRecordset);
    if (temp_record_count <= 0)
    {
        monitor_bado.CloseConn();
        return;
    }
    int temp_count = 0;
    _variant_t temp_variant;
    CString temp_device_db_name;
    int monitor_value = 0;
    while (VARIANT_FALSE == monitor_bado.m_pRecordset->EndOfFile)
    {
        temp_variant = monitor_bado.m_pRecordset->GetCollect("Device_DB_Name");//
        if (temp_variant.vt != VT_NULL)
        {
            temp_device_db_name = temp_variant;
            vector_third_db_name.push_back(temp_device_db_name);
        }
        else
        {
            temp_device_db_name.Empty();
            monitor_bado.m_pRecordset->MoveNext();
            continue;
        }

        temp_count++;
        monitor_bado.m_pRecordset->MoveNext();
    }
    monitor_bado.CloseRecordset();//Ffff add
    monitor_bado.CloseConn();
}

void CBacnetRegisterListView::Initial_List()
{
    m_register_view.ShowWindow(SW_HIDE);
    m_register_view.DeleteAllItems();
    while (m_register_view.DeleteColumn(0));

    m_register_view.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);


    if (m_device_mode)
    {
        vector_third_db_name.clear();
        m_register_view.SetExtendedStyle(m_register_view.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));//Not allow full row select.
        m_register_view.InsertColumn(REGISTER_LIST_ID, _T("ID"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
        m_register_view.InsertColumn(REGISTER_LIST_ADDRESS, _T("Address"), 60, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGUSTER_LIST_OPERATION, _T("Operation"), 130, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGISTER_LIST_REG_LENGTH, _T("Length"), 50, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGISTER_LIST_REG_NAME, _T("Register Name"), 140, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGISTER_LIST_VALUE, _T("Value"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGISTER_LIST_DATA_FORMAT, _T("Data Format"), 140, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGISTER_LIST_DESCRIPTION, _T("Description"), 350, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGISTER_LIST_UNIT, _T("Unit"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
        ShowThirdPartyWindow(1);
    }
    else
    {
        m_register_view.SetExtendedStyle(m_register_view.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));//Not allow full row select.
        m_register_view.InsertColumn(REGISTER_LIST_ID, _T("ID"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
        m_register_view.InsertColumn(REGISTER_LIST_ADDRESS, _T("Address"), 60, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGUSTER_LIST_OPERATION, _T("Operation"), 130, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGISTER_LIST_REG_LENGTH, _T("Length"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGISTER_LIST_REG_NAME, _T("Register Name"), 140, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGISTER_LIST_VALUE, _T("Value"), 70, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGISTER_LIST_DATA_FORMAT, _T("Data Format"), 140, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGISTER_LIST_DESCRIPTION, _T("Description"), 350, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
        m_register_view.InsertColumn(REGISTER_LIST_UNIT, _T("Unit"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
        ShowThirdPartyWindow(0);
    }
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

    
	if (m_device_mode)
	{
		//判断文件是否存在
		if (PathFileExists(m_third_db_path))
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("The register list for the corresponding product was not found !"));
			
            QueryAllDbDeviceName();
			//清空下面的combobox
			((CComboBox*)GetDlgItem(IDC_COMBO_REGISTER_DB))->ResetContent();
			//循环将vector_third_db_name 的内容添加到combobox中
            if (vector_third_db_name.size() > 0)
            {
                for (size_t i = 0; i < vector_third_db_name.size(); i++)
                {
                    ((CComboBox*)GetDlgItem(IDC_COMBO_REGISTER_DB))->AddString(vector_third_db_name.at(i));
                }
                ((CComboBox*)GetDlgItem(IDC_COMBO_REGISTER_DB))->SetCurSel(0);
            }
		}
        if (vector_third_db_name.size() > 0)
            QueryTable(vector_third_db_name.at(0));

	}
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

LRESULT CBacnetRegisterListView::Fresh_Register_Item(WPARAM wParam, LPARAM lParam)
{
    int cmp_ret;//compare if match it will 0;
    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam;
    if (Changed_SubItem == REGISTER_LIST_VALUE)
    {
        CString temp_task_info;
        CString New_CString = m_register_view.GetItemText(Changed_Item, REGISTER_LIST_VALUE);
        if (cs_clicked_cstring.CompareNoCase(New_CString) == 0)
        {
            return 0;
        }
        CString CStringAddress = m_register_view.GetItemText(Changed_Item, REGISTER_LIST_ADDRESS);

        CString CStringOperation = m_register_view.GetItemText(Changed_Item, REGUSTER_LIST_OPERATION);
        CString CStringDataFormat = m_register_view.GetItemText(Changed_Item, REGISTER_LIST_DATA_FORMAT);
        
        if ((CStringOperation.CompareNoCase(_T("03_06 Read Holding and Write Single")) == 0) ||
            (CStringOperation.CompareNoCase(_T("06 Write Single Register")) == 0))
        {
            unsigned short m_value = 0;
            int nret = 0;
            if (CStringDataFormat.CompareNoCase(RegisterView_Format[REGISTER_16_BIT_SIGNED_INTEGER_DIV_100]) == 0)
            {
                m_value = ((float)_wtof(New_CString)) * 100;
                if ((m_value < -32768) || (m_value > 32767))
                {
                    MessageBox(_T("Please enter the required data!"));
                    return 1;
                }
            }
            else  if (CStringDataFormat.CompareNoCase(RegisterView_Format[REGISTER_16_BIT_UNSIGNED_INTEGER_DIV_100]) == 0)
            {
                m_value = ((float)_wtof(New_CString)) * 100;
                if ((m_value < 0) || (m_value > 65535))
                {
                    MessageBox(_T("Please enter the required data!"));
                    return 1;
                }
            }
            else if (CStringDataFormat.CompareNoCase(RegisterView_Format[REGISTER_16_BIT_SIGNED_INTEGER_DIV_10]) == 0)
            {
               m_value = ((float)_wtof(New_CString)) * 10;
               if ((m_value < -32768) || (m_value > 32767))
               {
                   MessageBox(_T("Please enter the required data!"));
                   return 1;
               }
            }
            else  if (CStringDataFormat.CompareNoCase(RegisterView_Format[REGISTER_16_BIT_UNSIGNED_INTEGER_DIV_10]) == 0)
            {
                m_value = ((float)_wtof(New_CString)) * 10;
                if ((m_value < 0) || (m_value > 65535))
                {
                    MessageBox(_T("Please enter the required data!"));
                    return 1;
                }
            }
            else if (CStringDataFormat.CompareNoCase(RegisterView_Format[REGISTER_8_BIT_UNSIGNED_INTEGER]) == 0)
            {
                m_value = _wtoi(New_CString);
                if ((m_value < 0) || (m_value >= 256))
                {
                    MessageBox(_T("Please enter the required data!"));
                    return 1;
                }
            }
            else if (CStringDataFormat.CompareNoCase(RegisterView_Format[REGISTER_8_BIT_SIGNED_INTEGER]) == 0)
            {
                short m_value_temp = 0;
                m_value_temp = _wtoi(New_CString);
                if ((m_value_temp < -128) || (m_value_temp > 127))
                {
                    MessageBox(_T("Please enter the required data!"));
                    return 1;
                }
                m_value = m_value_temp;
            }
            else if (CStringDataFormat.CompareNoCase(RegisterView_Format[REGISTER_16_BIT_UNSIGNED_INTEGER]) == 0)
            {
                m_value = _wtoi(New_CString);
                if ((m_value < 0) || (m_value > 0xffff))
                {
                    MessageBox(_T("Please enter the required data!"));
                    return 1;
                }
            }
            else if (CStringDataFormat.CompareNoCase(RegisterView_Format[REGISTER_16_BIT_SIGNED_INTEGER]) == 0)
            {
                short m_value_temp = 0;
                m_value_temp = _wtoi(New_CString);
                if ((m_value_temp < -32768) || (m_value_temp > 32767))
                {
                    MessageBox(_T("Please enter the required data!"));
                    return 1;
                }
                m_value = m_value_temp;
            }
            else
            {
                return 1;
            }
            nret = write_one(g_tstat_id, _wtoi(CStringAddress), m_value);
            if (nret < 0)
            {
                MessageBox(_T("Write data timeout!"));
            }
            else
            {
                MessageBox(_T("Write data success!"));
            }
            //unsigned short
        }

    }
    return 1;
}

LRESULT CBacnetRegisterListView::Fresh_Register_List(WPARAM wParam, LPARAM lParam)
{
    if (m_device_mode)
    {
        m_register_view.DeleteAllItems();

        for (int i = 0; i < m_thir_db_data.size(); i++)
        {
            CString temp_id;
			temp_id.Format(_T("%d"), i + 1);
            m_register_view.InsertItem(i, temp_id);
			CString temp_register_address;
			temp_register_address.Format(_T("%d"), m_thir_db_data.at(i).reg_address);

			CString temp_register_length;
			temp_register_length.Format(_T("%d"), m_thir_db_data.at(i).reg_length);

            CString temp_register_operation = m_thir_db_data.at(i).reg_operation;
            CString temp_register_name = m_thir_db_data.at(i).reg_name;
            CString temp_register_data_format = m_thir_db_data.at(i).reg_data_formate;
            CString temp_register_description = m_thir_db_data.at(i).reg_description;
            CString temp_register_unit = m_thir_db_data.at(i).reg_unit;
            m_register_view.SetItemText(i, REGISTER_LIST_ID, temp_id);
            m_register_view.SetItemText(i, REGISTER_LIST_ADDRESS, temp_register_address);
            m_register_view.SetItemText(i, REGUSTER_LIST_OPERATION, temp_register_operation);
            m_register_view.SetItemText(i, REGISTER_LIST_REG_LENGTH, temp_register_length);
            m_register_view.SetItemText(i, REGISTER_LIST_REG_NAME, temp_register_name);
            m_register_view.SetItemText(i, REGISTER_LIST_DATA_FORMAT, temp_register_data_format);
            m_register_view.SetItemText(i, REGISTER_LIST_DESCRIPTION, temp_register_description);
            m_register_view.SetItemText(i, REGISTER_LIST_UNIT, temp_register_unit);
        }
        return 1;
	}
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
        unsigned short product_temp_value = 0;
        if (n_start_add >= 20000)
        {
            product_temp_value = 0;
        }
        else
        {
            product_temp_value = product_register_value[n_start_add];
        }

        n_value.Format(_T("%d"), product_temp_value);
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
            if (register_dbdata[i].m_register_length == 2)
            {
                signed int n_temp_int = 0;
                n_temp_int = product_register_value[n_start_add] * 65536 + product_register_value[n_start_add + 1];
                n_value.Format(_T("%d"), n_temp_int);
            }
            else if (register_dbdata[i].m_register_length == 4) //长度为4的是针对 序列号的，序列号是奇葩 占用8个字节 ;
            {
                unsigned int n_temp_int = 0;
                n_temp_int = product_register_value[n_start_add] + product_register_value[n_start_add + 1] * 256 + product_register_value[n_start_add + 2] * 256 * 256 + product_register_value[n_start_add + 3] * 256 * 256 * 256;
                n_value.Format(_T("%u"), n_temp_int);
            }
            break;

        }
        case REGISTER_32_BIT_SIGNED_INTEGER_LO_HI: 
        {  
            if (register_dbdata[i].m_register_length == 2)
            {
                signed int n_temp_int = 0;
                n_temp_int = product_register_value[n_start_add]  + product_register_value[n_start_add + 1] * 65536;
                n_value.Format(_T("%d"), n_temp_int);
            }
            break;
        }
        case REGISTER_FLOATING_HI_LO_DIV_10: 
        {  //Done
            if (register_dbdata[i].m_register_length == 2)
            {
                unsigned short temp1 = 0;
                unsigned short temp2 = 0;
                temp1 = product_register_value[n_start_add];
                temp2 = product_register_value[n_start_add + 1];
                float n_temp_float = 0;
                n_temp_float = ((float)(temp1 * 65536 + temp2)) / 10.0;
                n_value.Format(_T("%.1f"), n_temp_float);
            }
            break;
        }
        case REGISTER_FLOATING_LO_HI_DIV_10: 
        {  //Done
            if (register_dbdata[i].m_register_length == 2)
            {
                unsigned short temp1 = 0;
                unsigned short temp2 = 0;
                temp1 = product_register_value[n_start_add];
                temp2 = product_register_value[n_start_add + 1];
                float n_temp_float = 0;
                n_temp_float = ((float)(temp1 + temp2 * 65536)) / 10.0;
                n_value.Format(_T("%.1f"), n_temp_float);
            }
            break;
        }
        case REGISTER_FLOATING_HI_LO_DIV_100: 
        {  
            //Done
            if (register_dbdata[i].m_register_length == 2)
            {
                unsigned short temp1 = 0;
                unsigned short temp2 = 0;
                temp1 = product_register_value[n_start_add];
                temp2 = product_register_value[n_start_add + 1];
                float n_temp_float = 0;
                n_temp_float = ((float)(temp1 * 65536 + temp2)) / 100.0;
                n_value.Format(_T("%.2f"), n_temp_float);
            }
            break;
        }
        case REGISTER_FLOATING_LO_HI_DIV_100: 
        {  
            //Done
            if (register_dbdata[i].m_register_length == 2)
            {
                unsigned short temp1 = 0;
                unsigned short temp2 = 0;
                temp1 = product_register_value[n_start_add];
                temp2 = product_register_value[n_start_add + 1];
                float n_temp_float = 0;
                n_temp_float = ((float)(temp1 + temp2 * 65536)) / 100.0;
                n_value.Format(_T("%.2f"), n_temp_float);
            }
            break;
        }
        case REGISTER_FLOATING_HI_LO_DIV_1000: 
        {  //Done
            if (register_dbdata[i].m_register_length == 2)
            {
                unsigned short temp1 = 0;
                unsigned short temp2 = 0;
                temp1 = product_register_value[n_start_add];
                temp2 = product_register_value[n_start_add + 1];
                float n_temp_float = 0;
                n_temp_float = ((float)(temp1 * 65536 + temp2))/1000.0;
                n_value.Format(_T("%.3f"), n_temp_float);
            }
            break;
        }
        case REGISTER_FLOATING_LO_HI_DIV_1000: 
        {   //Done
            if (register_dbdata[i].m_register_length == 2)
            {
                unsigned short temp1 = 0;
                unsigned short temp2 = 0;
                temp1 = product_register_value[n_start_add];
                temp2 = product_register_value[n_start_add + 1];
                float n_temp_float = 0;
                n_temp_float = ((float)(temp1  + temp2 * 65536)) / 1000.0;
                n_value.Format(_T("%.3f"), n_temp_float);
            }
            break;
        }
        case REGISTER_CHARACTER_STRING_HI_LO: 
        {  
                CString temp_string;
                char temp_char[255];
                int ntemplength = register_dbdata[i].m_register_length;
                unsigned short temp_reg[255];
                memset(temp_char, 0, 255);
                memset(temp_reg, 0, 255);
                for (int i = 0; i < ntemplength; i++)
                {
                    temp_reg[i] = htons(product_register_value[n_start_add + i]);
                    //product_register_value[n_start_add] 
                }
                memcpy_s(temp_char, 255, temp_reg, ntemplength * 2);
                MultiByteToWideChar(CP_ACP, 0, (char*)temp_char, (int)strlen(temp_char) + 1,temp_string.GetBuffer(ntemplength * 2 + 1), ntemplength * 2 + 1);
                temp_string.ReleaseBuffer();
                temp_string.Trim();
                n_value = temp_string;                   
        }
        break;
        case REGISTER_CHARACTER_STRING_LO_HI: 
        {  
            CString temp_string;
            char temp_char[255];
            int ntemplength = register_dbdata[i].m_register_length;
            unsigned short temp_reg[255];
            memset(temp_char, 0, 255);
            memset(temp_reg, 0, 255);
            for (int i = 0; i < ntemplength; i++)
            {
                temp_reg[i] = product_register_value[n_start_add + i];
                //product_register_value[n_start_add] 
            }
            memcpy_s(temp_char, 255, temp_reg, ntemplength * 2);
            MultiByteToWideChar(CP_ACP, 0, (char*)temp_char, (int)strlen(temp_char) + 1, temp_string.GetBuffer(ntemplength * 2 + 1), ntemplength * 2 + 1);
            temp_string.ReleaseBuffer();
            temp_string.Trim();
            n_value = temp_string;
            break;
        }
        case REGISTER_16_BIT_UNSIGNED_INTEGER_DIV_10:
        {
            float n_temp_float = 0;
            n_temp_float = ((float)product_register_value[n_start_add])/10.0 ;
            n_value.Format(_T("%.1f"), n_temp_float);
            break;
        }
        case REGISTER_16_BIT_SIGNED_INTEGER_DIV_10:
        {
            int n_temp_float = 0;
            if (product_register_value[n_start_add] > 32767)
            {
                n_value.Format(_T("-%.1f"), (float)(65536 - product_register_value[n_start_add]) / 10.0);
            }
            else
            {
                n_value.Format(_T("%.1f"), (float)product_register_value[n_start_add] / 10.0 );
            }

            
            break;
        }
        case REGISTER_16_BIT_UNSIGNED_INTEGER_DIV_100:
        {
            float n_temp_float = 0;
            n_temp_float = ((float)product_register_value[n_start_add]) / 100.0;
            n_value.Format(_T("%.2f"), n_temp_float);
            break;
        }
        case REGISTER_16_BIT_SIGNED_INTEGER_DIV_100:
        {
            int n_temp_float = 0;
            if (product_register_value[n_start_add] > 32767)
            {
                n_value.Format(_T("-%.2f"), (float)(65536 - product_register_value[n_start_add]) / 100.0);
            }
            else
            {
                n_value.Format(_T("%.2f"), (float)product_register_value[n_start_add] / 100.0);
            }


            break;
        }
        case REGISTER_2REG_FLOAT_ABCD:
        {
            int ntemplength = register_dbdata[i].m_register_length;
            if (ntemplength != 2)
            {
                break;
            }
            Float_Char_Data temp_data_abcd;
            temp_data_abcd.chardata[0] = product_register_value[n_start_add + 1] % 256;
            temp_data_abcd.chardata[1] = product_register_value[n_start_add + 1] / 256;
            temp_data_abcd.chardata[2] = product_register_value[n_start_add] % 256;  
            temp_data_abcd.chardata[3] = product_register_value[n_start_add] / 256;  
            n_value.Format(_T("%.2f"), (float)temp_data_abcd.floatvalue);

        }
            break;
        case REGISTER_2REG_FLOAT_CDAB :
        {
            int ntemplength = register_dbdata[i].m_register_length;
            if (ntemplength != 2)
            {
                break;
            }
            Float_Char_Data temp_data_abcd;
            temp_data_abcd.chardata[2] = product_register_value[n_start_add + 1] % 256;
            temp_data_abcd.chardata[3] = product_register_value[n_start_add + 1] / 256;
            temp_data_abcd.chardata[0] = product_register_value[n_start_add] % 256;
            temp_data_abcd.chardata[1] = product_register_value[n_start_add] / 256;
            n_value.Format(_T("%.2f"), (float)temp_data_abcd.floatvalue);

        }
            break;
        case REGISTER_2REG_FLOAT_BADC:
        {
            int ntemplength = register_dbdata[i].m_register_length;
            if (ntemplength != 2)
            {
                break;
            }
            Float_Char_Data temp_data_abcd;
            temp_data_abcd.chardata[0] = product_register_value[n_start_add + 1] / 256;
            temp_data_abcd.chardata[1] = product_register_value[n_start_add + 1] % 256;
            temp_data_abcd.chardata[2] = product_register_value[n_start_add] / 256;
            temp_data_abcd.chardata[3] = product_register_value[n_start_add] % 256;
            n_value.Format(_T("%.2f"), (float)temp_data_abcd.floatvalue);

        }
            break;
        case REGISTER_2REG_FLOAT_DCBA:
        {
            int ntemplength = register_dbdata[i].m_register_length;
            if (ntemplength != 2)
            {
                break;
            }
            Float_Char_Data temp_data_abcd;
            temp_data_abcd.chardata[3] = product_register_value[n_start_add + 1] % 256;
            temp_data_abcd.chardata[2] = product_register_value[n_start_add + 1] / 256;
            temp_data_abcd.chardata[1] = product_register_value[n_start_add] % 256;
            temp_data_abcd.chardata[0] = product_register_value[n_start_add] / 256;
            n_value.Format(_T("%.2f"), (float)temp_data_abcd.floatvalue);

        }
            break;
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
            if ((cs_operation.CompareNoCase(_T("03_06 Read Holding and Write Single")) == 0) ||
                (cs_operation.CompareNoCase(_T("06 Write Single Register")) == 0))
            {
                m_register_view.SetCellEnabled(i, REGISTER_LIST_VALUE, 1);
                //unsigned short
            }

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


void CBacnetRegisterListView::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialogEx::OnClose();
}


void CBacnetRegisterListView::OnLvnItemchangedListRegisterView(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
}


void CBacnetRegisterListView::OnNMClickListRegisterView(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
    long lRow, lCol;
    m_register_view.Set_Edit(true);
    DWORD dwPos = GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point(GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
    m_register_view.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt = point;
    lvinfo.flags = LVHT_ABOVE;
    int nItem = m_register_view.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;


    if (lRow>m_register_view.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
        return;
    if (lRow<0)
        return;
    if (m_device_mode == 1)
        return;
    if(lCol != REGISTER_LIST_VALUE)
        return;

    CString CStringOperation = m_register_view.GetItemText(lRow, REGUSTER_LIST_OPERATION);
    CString CStringValue = m_register_view.GetItemText(lRow, REGISTER_LIST_VALUE);

    if ((CStringOperation.CompareNoCase(_T("03_06 Read Holding and Write Single")) == 0) ||
        (CStringOperation.CompareNoCase(_T("06 Write Single Register")) == 0))
    {
        m_register_view.Set_Edit(true);
        cs_clicked_cstring = CStringValue;
    }
    else
        return;
}


void CBacnetRegisterListView::OnBnClickedButtonCreateDb()
{
    // TODO: 在此添加控件通知处理程序代码
    int n_ret = 0;
    CString FilePath;

    FilePath = g_strExePth + _T("ResourceFile\\ThirdPartyRegistersListDB.mdb");
    n_ret = Create_DeviceDatabase(m_third_db_path, FilePath);
    if(n_ret)
		MessageBox(_T("Create Database Success!"));



        CString temp_item;
        temp_item.Format(_T("%d"),  1);
        m_register_view.InsertItem(0, temp_item);

}


void CBacnetRegisterListView::OnBnClickedButtonThirdSave()
{
    // TODO: 在此添加控件通知处理程序代码
    CBADO monitor_bado;
    monitor_bado.SetDBPath(m_third_db_path);	//暂时不创建新数据库
    monitor_bado.OnInitADOConn();
    CString temp_db_name; CString temp_operation; CString temp_reg_name; CString temp_data_formate; CString temp_description; CString temp_unit;
    GetDlgItemText(IDC_EDIT_REGISTER_DB_NAME, temp_db_name);
    int temp_address = _wtoi( m_register_view.GetItemText(0, REGISTER_LIST_ADDRESS));
    temp_operation = m_register_view.GetItemText(0, REGUSTER_LIST_OPERATION);
    int temp_length = _wtoi(m_register_view.GetItemText(0, REGISTER_LIST_REG_LENGTH));


    temp_reg_name = m_register_view.GetItemText(0, REGISTER_LIST_REG_NAME);
    temp_data_formate = m_register_view.GetItemText(0, REGISTER_LIST_DATA_FORMAT);
    temp_description = m_register_view.GetItemText(0, REGISTER_LIST_DESCRIPTION);
    temp_unit = m_register_view.GetItemText(0, REGISTER_LIST_UNIT);

    CString strSql;
    strSql.Format(_T("insert into ThirdPartyRegister (Device_DB_Name,Register_Address,Operation,Register_Length,Register_Name,Data_Format,Description,Unit) values('%s',%u,'%s',%u,'%s','%s','%s','%s')"),temp_db_name, temp_address,temp_operation,temp_length,temp_reg_name, temp_data_formate,temp_description,temp_unit);
    monitor_bado.m_pConnection->Execute(strSql.GetString(), NULL, adCmdText);
    monitor_bado.CloseConn();
}

void CBacnetRegisterListView::UpdateThirdUI()
{

}


void CBacnetRegisterListView::OnNMRClickListRegisterView(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    CString temp_cstring;
    long lRow, lCol;
    m_register_view.Set_Edit(true);
    DWORD dwPos = GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point(GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
    m_register_view.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt = point;
    lvinfo.flags = LVHT_ABOVE;
    int nItem = m_register_view.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;

    if (lRow > m_register_view.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
        return;
    if (lRow < 0)
        return;


    CMenu menu;
    menu.LoadMenu(IDR_THIRD_REG_MENU_POP);
    CMenu* pmenu = menu.GetSubMenu(0);
    //CPoint point;
    GetCursorPos(&point);
    pmenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);

    *pResult = 0;
}



void CBacnetRegisterListView::OnCbnSelchangeComboRegisterDb()
{
    // TODO: 在此添加控件通知处理程序代码
    CString temp_string;
    int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_REGISTER_DB))->GetCurSel();
    ((CComboBox*)GetDlgItem(IDC_COMBO_REGISTER_DB))->GetLBText(nSel, temp_string);
    QueryTable(temp_string);
}


void CBacnetRegisterListView::OnBnClickedButtonDeleteSelectedDb()
{
    // TODO: 在此添加控件通知处理程序代码
    CBADO monitor_bado;
    monitor_bado.SetDBPath(m_third_db_path);	//暂时不创建新数据库
    monitor_bado.OnInitADOConn();
    CString temp_db_name; 
    GetDlgItemText(IDC_EDIT_REGISTER_DB_NAME, temp_db_name);

    CString strSql;
    strSql.Format(_T("delete * from ThirdPartyRegister where Device_DB_Name = '%s'") , temp_db_name);
    monitor_bado.m_pConnection->Execute(strSql.GetString(), NULL, adCmdText);
    monitor_bado.CloseConn();
    QueryAllDbDeviceName();
    
    //清空下面的combobox
    ((CComboBox*)GetDlgItem(IDC_COMBO_REGISTER_DB))->ResetContent();
    //循环将vector_third_db_name 的内容添加到combobox中
    for (size_t i = 0; i < vector_third_db_name.size(); i++)
    {
        ((CComboBox*)GetDlgItem(IDC_COMBO_REGISTER_DB))->AddString(vector_third_db_name.at(i));
    }
    if (vector_third_db_name.size() > 0)
    {
        ((CComboBox*)GetDlgItem(IDC_COMBO_REGISTER_DB))->SetCurSel(0);
        QueryTable(vector_third_db_name.at(0));
    }
}