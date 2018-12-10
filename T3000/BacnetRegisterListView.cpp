// BacnetRegisterListView.cpp : ??????
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetRegisterListView.h"
#include "afxdialogex.h"


// CBacnetRegisterListView

IMPLEMENT_DYNAMIC(CBacnetRegisterListView, CDialogEx)

CBacnetRegisterListView::CBacnetRegisterListView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_REGISTER_LIST, pParent)
{

}

CBacnetRegisterListView::~CBacnetRegisterListView()
{
}

void CBacnetRegisterListView::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_REGISTER_VIEW, m_register_view);
}


BEGIN_MESSAGE_MAP(CBacnetRegisterListView, CDialogEx)
    ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL CBacnetRegisterListView::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    Test_Read_Excel();
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
