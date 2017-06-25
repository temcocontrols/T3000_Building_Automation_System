// BatchSNDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BatchSNDlg.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "excel9.h"
DWORD WINAPI _Batch_Flash_SN(LPVOID pParam)
{
    CBatchSNDlg* pView=(CBatchSNDlg*)(pParam);
   
    for (int i = 0;i<pView->m_modbus_IDs.size();i++)
    {
        int IntSN=pView->GetNewSerialNumber();
                write_one(pView->m_modbus_IDs.at(i).ModbusID,2026,123,5);
        int ret=write_one(pView->m_modbus_IDs.at(i).ModbusID,2027,IntSN,5);
        if (ret>0)
        {
              pView->WriteSerialNumber(IntSN);
              pView->m_modbus_IDs.at(i).SN = IntSN;
              pView->m_modbus_IDs.at(i).Result = TRUE; 
        } 
        else
        {    
             pView->m_modbus_IDs.at(i).SN = -1;
             pView->m_modbus_IDs.at(i).Result = FALSE;
        } 
        SendMessage(g_hwnd_now,WM_REFRESH_BAC_INPUT_LIST,2,i);
    }
     //PostMessage(g_hwnd_now,WM_REFRESH_BAC_INPUT_LIST,0,0);
    return 1;
}
// CBatchSNDlg dialog

IMPLEMENT_DYNAMIC(CBatchSNDlg, CDialogEx)

CBatchSNDlg::CBatchSNDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBatchSNDlg::IDD, pParent)
    , m_intStart(1)
    , m_intEnd(254)
{

}

CBatchSNDlg::~CBatchSNDlg()
{
}

void CBatchSNDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_products_list);
    DDX_Text(pDX, IDC_EDIT_START, m_intStart);
    DDV_MinMaxUInt(pDX, m_intStart, 1, 254);
    DDX_Text(pDX, IDC_EDIT_END, m_intEnd);
	DDV_MinMaxUInt(pDX, m_intEnd, 1, 254);
}


BEGIN_MESSAGE_MAP(CBatchSNDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CBatchSNDlg::OnBnClickedOk)
    ON_EN_KILLFOCUS(IDC_EDIT_START, &CBatchSNDlg::OnEnKillfocusEditStart)
    ON_EN_KILLFOCUS(IDC_EDIT_END, &CBatchSNDlg::OnEnKillfocusEditEnd)
    ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Input_List)
    
    ON_NOTIFY(NM_CLICK, IDC_LIST_T3INPUTS, &CBatchSNDlg::OnNMClickList_Input)
    ON_BN_CLICKED(IDC_CALIBRATION_POINT_REPORT, &CBatchSNDlg::OnBnClickedCalibrationPointReport)
END_MESSAGE_MAP()


// CBatchSNDlg message handlers


void CBatchSNDlg::OnBnClickedOk()
{
    
   // CDialogEx::OnOK();
   CString ModbusID;
   Batch_SN Temp;
   for (int i=0;i<m_products_list.GetRowCount();i++)
   {    ModbusID = m_products_list.GetItemText(i,0);
        Temp.ModbusID = _wtoi(ModbusID);
        m_modbus_IDs.push_back(Temp);
   }
   if(hFirstThread != NULL)
       TerminateThread(hFirstThread, 0);
   hFirstThread=NULL;
   if (!hFirstThread)
   {
       hFirstThread = CreateThread(NULL,NULL,_Batch_Flash_SN,this,NULL,0);
   }
   
}

unsigned long CBatchSNDlg::GetNewSerialNumber(){
    int nSerialNumber;		
    CStdioFile file;
    CString filepath;
    filepath=g_strExePth+_T("Sensor Serial Record.txt");
    if(!file.Open(filepath, CFile::shareDenyNone|CFile::modeReadWrite))
    {

        return -1;
    }

    //----------------------------get the serialnumber--------------------------------------
    ///////////////////// read a line from file to the lastest line
    CString strLine,strTemp;	


    while(file.ReadString(strTemp))
    {
        strLine = strTemp;		
    }	
    strTemp = strLine.Mid(0, strLine.Find(_T(',')));
    nSerialNumber = _wtoi(strTemp);//the serialnumber
    file.Close();
    return nSerialNumber+1;
}
void CBatchSNDlg::WriteSerialNumber(unsigned long SN){
    CStdioFile file;
    CString filepath;
    CString SerialDate;
    CTime time = CTime::GetCurrentTime();

    SerialDate.Format(_T("\n%d,%d-%d-%d  %d:%d:%d"),SN,time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());

    filepath=g_strExePth+_T("Sensor Serial Record.txt");
    if(!file.Open(filepath, CFile::shareDenyNone|CFile::modeReadWrite))
    {
        return ;
    }

    file.SeekToEnd();

    file.WriteString(SerialDate.GetBuffer());
    //  file.Flush();
    file.Close();
}
BOOL CBatchSNDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
       CString strTemp;
    UpdateData(TRUE);
    m_products_list.ModifyStyle(0,LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    m_products_list.SetExtendedStyle(m_products_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
    m_products_list.InsertColumn(0, _T("Modbus ID"), 180, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    m_products_list.InsertColumn(1, _T("Sensor SN"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_products_list.InsertColumn(2, _T("Result"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    g_hwnd_now = this->m_hWnd;
    for (int i = m_intStart ; i<=m_intEnd ;i++)
    {   
        strTemp.Format(_T("%d"),i);
        m_products_list.InsertItem(i-m_intStart,strTemp);
    }
     
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CBatchSNDlg::OnEnKillfocusEditStart()
{
  UpdateData(TRUE);
  PostMessage(WM_REFRESH_BAC_INPUT_LIST,1,0); 
}


void CBatchSNDlg::OnEnKillfocusEditEnd()
{
 UpdateData(TRUE);
 PostMessage(WM_REFRESH_BAC_INPUT_LIST,1,0);    
}
LRESULT CBatchSNDlg::Fresh_Input_List(WPARAM wParam,LPARAM lParam){
        int fresh_type = (int)wParam;
        int row = (int)lParam;
       
        CString strTemp;
        if (fresh_type == 0)
        {
             m_products_list.DeleteAllItems();
            for (int i=0;i<(int)m_modbus_IDs.size();i++)
            {   
            strTemp.Format(_T("%d"),m_modbus_IDs.at(i).ModbusID);
            m_products_list.InsertItem(i,strTemp);
            strTemp.Format(_T("%d"),m_modbus_IDs.at(i).SN);
            m_products_list.SetItemText(i,1,strTemp);
            if (m_modbus_IDs.at(i).Result)
            {
                m_products_list.SetItemText(i,2,_T("OK"));
            } 
            else
            {
                m_products_list.SetItemText(i,2,_T("Fail"));
            }

            }
        }
        else if(fresh_type == 1){
         m_products_list.DeleteAllItems();
            for (int i = m_intStart ; i<=m_intEnd ;i++)
            {   
                strTemp.Format(_T("%d"),i);
                m_products_list.InsertItem(i-m_intStart,strTemp);
            }
        } 

        else if (fresh_type == 2)
        {
            strTemp.Format(_T("%d"),m_modbus_IDs.at(row).ModbusID);
            m_products_list.SetItemText(row,0,strTemp);
            strTemp.Format(_T("%d"),m_modbus_IDs.at(row).SN);
            m_products_list.SetItemText(row,1,strTemp);
            if (m_modbus_IDs.at(row).Result)
            {
                m_products_list.SetItemText(row,2,_T("OK"));
            } 
            else
            {
                m_products_list.SetItemText(row,2,_T("Fail"));
            }
        }
        return 0;
}

 

void CBatchSNDlg::OnNMClickList_Input(NMHDR *pNMHDR, LRESULT *pResult){
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    CString temp_cstring;
    long lRow,lCol;
    m_products_list.Set_Edit(true);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_products_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_products_list.SubItemHitTest(&lvinfo);
    BOOL Is_Range = FALSE;
    int Range_Address = -1;
    int Value_Address = -1;
    int Value_Length = 0;
    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;

    if(lRow>m_products_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
        return;
    if(lRow<0)
        return;
    int IntSN=GetNewSerialNumber();
     //
    write_one(m_modbus_IDs.at(lRow).ModbusID,2026,123,5);
    int ret=write_one(m_modbus_IDs.at(lRow).ModbusID,2027,IntSN,5);
    if (ret>0)
    {
        WriteSerialNumber(IntSN);
        m_modbus_IDs.at(lRow).SN = IntSN;
        m_modbus_IDs.at(lRow).Result = TRUE;
    } 
    else
    {    
        m_modbus_IDs.at(lRow).SN = -1;
        m_modbus_IDs.at(lRow).Result = FALSE;
    } 
    PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);
}


void CBatchSNDlg::OnBnClickedCalibrationPointReport()
{
    CString  Product_Head_File_Name;
    CString strFilter;
    CString strFilename;
    CString strTemp;
    CString RegisterName;
    CString RegisterID;
    CString logstr;
    _variant_t  temp_variant;
    _Application app;    
    Workbooks books;
    _Workbook book;
    Worksheets sheets;
    _Worksheet sheet;
    Range range;
    Range rgMyRge1, rgMyRge2; 	
    COleVariant covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
   
     int CurrentRow;


    if (!app.CreateDispatch(_T("Excel.Application"),NULL)) 
    { 
        AfxMessageBox(_T("Create Excel false!")); 
        return;
    } 
    //遍历所有行  
    strFilename=g_strExePth+_T("Calibration Sensor Report.xls");

    books.AttachDispatch(app.GetWorkbooks()); 
    book.AttachDispatch(books.Add(_variant_t(strFilename)));

    sheets.AttachDispatch(book.GetWorksheets());	
    sheet.AttachDispatch(sheets.GetItem(_variant_t("Sheet1")));

    range.AttachDispatch(sheet.GetCells()); 
    CString ContentData;
    CTime time = CTime::GetCurrentTime();
   
    ContentData.Format(_T("Calibration Date"));
    range.SetItem(_variant_t((long)(1)),_variant_t((long)(1)),_variant_t(ContentData));
   
    ContentData.Format(_T("%d-%d-%d"),time.GetYear(),time.GetMonth(),time.GetDay());
    range.SetItem(_variant_t((long)(1)),_variant_t((long)(3)),_variant_t(ContentData));
 
    range.AttachDispatch(sheet.GetCells());
 
    for (int i=0;i<(int)m_modbus_IDs.size();i++)
    {  
        if (!m_modbus_IDs.at(i).Result)
        {
        continue;
        }
        CurrentRow = 3 + 7*i;
        unsigned short databuffer[30];
        Read_Multi(m_modbus_IDs.at(i).ModbusID,&databuffer[0],2000,30);
        ContentData = _T("Serial Number");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(1)),_variant_t(ContentData));
        ContentData.Format(_T("%d"),databuffer[2027 - 2000]);
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(2)),_variant_t(ContentData));

        ContentData = _T("Points");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(3)),_variant_t(ContentData));
        ContentData.Format(_T("%d"),databuffer[2003 - 2000]);
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(4)),_variant_t(ContentData));
         ++CurrentRow;
        ContentData = _T("Temperature");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(1)),_variant_t(ContentData));
        ContentData.Format(_T("%0.1f"),((float)((short)databuffer[2000 - 2000]))/10);
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(2)),_variant_t(ContentData));

        ContentData = _T("Humidity");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(3)),_variant_t(ContentData));
        ContentData.Format(_T("%0.1f"),((float)((short)databuffer[2001 - 2000]))/10);
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(4)),_variant_t(ContentData));

        
        ++CurrentRow;
        ContentData = _T("RH");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(1)),_variant_t(ContentData));

        for (int j = 0;j<10;j++)
        {
            ContentData.Format(_T("%0.1f"),((float)((short)databuffer[2004 - 2000 + 2*j]))/10);
            range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(2+j)),_variant_t(ContentData));
        }
        ++CurrentRow;
        ContentData = _T("Frequency");
        range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(1)),_variant_t(ContentData));
        for (int j = 0;j<10;j++)
        {
            ContentData.Format(_T("%d"),databuffer[2005 - 2000 + 2*j]);
            range.SetItem(_variant_t((long)(CurrentRow)),_variant_t((long)(2+j)),_variant_t(ContentData));
        } 
 

    }

    AfxMessageBox(_T("Export Completely"));

    app.SetVisible(true); 
    range.ReleaseDispatch(); 
    sheet.ReleaseDispatch(); 
    sheets.ReleaseDispatch(); 
    book.ReleaseDispatch(); 
    books.ReleaseDispatch();
    app.ReleaseDispatch(); 
}


BOOL CBatchSNDlg::PreTranslateMessage(MSG* pMsg)
{
    
    if(pMsg->message == WM_KEYDOWN  )
    {
        if(pMsg->wParam == VK_RETURN)
        {
            CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
            GetDlgItem(IDC_EDIT_TEST)->SetFocus();
            temp_focus->SetFocus();

            return 1;
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
