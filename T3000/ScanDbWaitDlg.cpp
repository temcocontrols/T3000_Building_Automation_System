// ScanDbWaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ScanDbWaitDlg.h"
#include "TStatScanner.h"

// CScanDbWaitDlg dialog
CRect Scan_rect;
IMPLEMENT_DYNAMIC(CScanDbWaitDlg, CDialog)

CScanDbWaitDlg::CScanDbWaitDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CScanDbWaitDlg::IDD, pParent)
    , m_strNetScanInfo(_T(""))
    , m_strComScanInfo(_T(""))
{
    m_pScaner = NULL;
    g_bCancelScan=FALSE;
    m_strNetScanInfo = _T("Net Scan : Start UDP Scan.");
}

CScanDbWaitDlg::~CScanDbWaitDlg()
{
}

void CScanDbWaitDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_INFOSTATIC, m_strInfopromp);
    DDX_Text(pDX, IDC_INFO_NET, m_strNetScanInfo);
    DDX_Text(pDX, IDC_INFO_COM, m_strComScanInfo);
    DDX_Control(pDX, IDC_LIST_SCAN, m_scan_com_list);
}


BEGIN_MESSAGE_MAP(CScanDbWaitDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CScanDbWaitDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CScanDbWaitDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_EXITBUTTON, &CScanDbWaitDlg::OnBnClickedExitbutton)
    ON_WM_TIMER()
    ON_MESSAGE(WM_NETSCANINFO, OnNetScanInfo)
    ON_MESSAGE(WM_COMSCANINFO, OnComScanInfo)
    ON_MESSAGE(WM_BACNETCOMSCANINFO, OnBacnetComScanInfo)

END_MESSAGE_MAP()


void CScanDbWaitDlg::SetTStatScaner(CTStatScanner* pScanner)
{
    ASSERT(pScanner);
    m_pScaner = pScanner;
// SetTimer(1,100,NULL);
}
// CScanDbWaitDlg message handlers
void CScanDbWaitDlg::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    //OnOK();
}

void CScanDbWaitDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    //OnCancel();
    ;
}

void CScanDbWaitDlg::OnBnClickedExitbutton()
{
    // TODO: Add your control notification handler code here
    g_bCancelScan=TRUE;
    m_pScaner->StopScan();
    OnCancel();
    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    ::PostMessage(pFrame->m_hWnd,WM_MYMSG_REFRESHBUILDING,0,0);
}

BOOL CScanDbWaitDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    g_bCancelScan=FALSE;
    SetTimer(1,100,NULL);
    SetTimer(2,200,NULL);
    Initial_List();
    GetDlgItem(IDC_STATIC_SCAN_PIC)->GetWindowRect(Scan_rect);
    ScreenToClient(Scan_rect);

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CScanDbWaitDlg::OnTimer(UINT_PTR nIDEvent)
{
    static int nCount = 0;
    static int icon_count = 0;
    if(nIDEvent == 1)
    {
        if (m_pScaner)
        {
            if(!m_pScaner->IsAllScanFinished())
            {
                if (nCount++ > 30)
                {
                    nCount = 0;
                }
                if (icon_count++ > 21)
                {
                    icon_count = 0;
                }

                switch(icon_count)
                {
                case 0:
                    GetDlgItem(IDC_STATIC_SCAN_PIC)->SetWindowPos(NULL,Scan_rect.left,Scan_rect.top,0,0,SWP_NOZORDER|SWP_NOSIZE);
                    break;
                case 3:
                    GetDlgItem(IDC_STATIC_SCAN_PIC)->SetWindowPos(NULL,Scan_rect.left + 5,Scan_rect.top + 5,0,0,SWP_NOZORDER|SWP_NOSIZE);
                    break;
                case 6:
                    GetDlgItem(IDC_STATIC_SCAN_PIC)->SetWindowPos(NULL,Scan_rect.left + 10,Scan_rect.top + 10,0,0,SWP_NOZORDER|SWP_NOSIZE);
                    break;
                case 9:
                    GetDlgItem(IDC_STATIC_SCAN_PIC)->SetWindowPos(NULL,Scan_rect.left + 5,Scan_rect.top + 15,0,0,SWP_NOZORDER|SWP_NOSIZE);
                    break;
                case 12:
                    GetDlgItem(IDC_STATIC_SCAN_PIC)->SetWindowPos(NULL,Scan_rect.left,Scan_rect.top + 20,0,0,SWP_NOZORDER|SWP_NOSIZE);
                    break;
                case 15:
                    GetDlgItem(IDC_STATIC_SCAN_PIC)->SetWindowPos(NULL,Scan_rect.left -5,Scan_rect.top +15,0,0,SWP_NOZORDER|SWP_NOSIZE);
                    break;
                case 18:
                    GetDlgItem(IDC_STATIC_SCAN_PIC)->SetWindowPos(NULL,Scan_rect.left -10,Scan_rect.top +10,0,0,SWP_NOZORDER|SWP_NOSIZE);
                    break;
                case 21:
                    GetDlgItem(IDC_STATIC_SCAN_PIC)->SetWindowPos(NULL,Scan_rect.left -5,Scan_rect.top +5,0,0,SWP_NOZORDER|SWP_NOSIZE);
                    break;
                default:
                    break;
                }

                switch(nCount)
                {
                case 5:
                    m_strDot = _T(".");
                    break;
                case 10:
                    m_strDot = _T("..");
                    break;
                case 15:
                    m_strDot = _T("...");
                    break;
                case 20:
                    m_strDot = _T("....");
                    break;
                case 25:
                    m_strDot = _T(".....");
                    break;
                case 30:
                    m_strDot = _T("......");
                    break;

                    //default:strDot = _T("......");break;
                }
                CString strTemp;
                CString strTip;
                strTip.Format(_T("T3000 is scanning, please wait%s"), m_strDot);

                m_strInfopromp=strTip;

                UpdateData(FALSE);
            }
        }


        if (m_pScaner)
        {
            if (m_pScaner->IsAllScanFinished())
            {
                KillTimer(1);
                g_ScnnedNum=0;
                SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Scan finished!"));
                CScanDbWaitDlg::OnOK();
            }
        }
    }

    if(nIDEvent == 2)
    {
        for (int x=0; x<m_scan_info.size(); x++)
        {
            int ret_val = memcmp(&m_scan_info_buffer.at(x),&m_scan_info.at(x),sizeof(Scan_Info));
            if(ret_val == 0)
                continue;



            CString temp_notes;
            MultiByteToWideChar( CP_ACP, 0, m_scan_info.at(x).scan_notes,(int)strlen(m_scan_info.at(x).scan_notes)+1,
                                 temp_notes.GetBuffer(MAX_PATH), MAX_PATH );
            temp_notes.ReleaseBuffer();
            if(!temp_notes.IsEmpty())
                m_scan_com_list.SetItemText(x,SCAN_NOTES,temp_notes);

            CString temp_number;
            temp_number.Format(_T("%u"),m_scan_info.at(x).scan_found);
            m_scan_com_list.SetItemText(x,SCAN_FOUND,temp_number);
            if(m_scan_info.at(x).scan_status != m_scan_info_buffer.at(x).scan_status)
            {
                switch(m_scan_info.at(x).scan_status)
                {
                case SCAN_STATUS_WAIT:
                    m_scan_com_list.SetItemText(x,SCAN_STATUS,_T("Wait"));
                    break;
                case SCAN_STATUS_RUNNING:
                    m_scan_com_list.SetItemText(x,SCAN_STATUS,_T("Running"));
                    m_scan_com_list.SetItemBkColor(x,SCAN_STATUS,RGB(0,255,255));
                    break;
                case SCAN_STATUS_FINISHED:
                    m_scan_com_list.SetItemText(x,SCAN_STATUS,_T("Finished"));
                    m_scan_com_list.SetItemBkColor(x,SCAN_STATUS,RGB(0,255,0));
                    break;
                case SCAN_STATUS_FAILED:
                    m_scan_com_list.SetItemText(x,SCAN_STATUS,_T("Failed"));
                    m_scan_com_list.SetItemBkColor(x,SCAN_STATUS,RGB(255,0,0));
                    break;
                case SCAN_STATUS_SKIP:
                    m_scan_com_list.SetItemText(x,SCAN_STATUS,_T("Skip"));
                    break;
                default:
                    break;
                }
            }


            memcpy(&m_scan_info_buffer.at(x),&m_scan_info.at(x),sizeof(Scan_Info));
        }


    }
    CDialog::OnTimer(nIDEvent);
}
void CScanDbWaitDlg::SetPromtText(CString strInfo)
{


    CString* pstrInfo = new CString(strInfo);
    ::SendMessage(MainFram_hwd,WM_SHOW_PANNELINFOR,WPARAM(pstrInfo),LPARAM(3));
    UpdateData(FALSE);
}


LRESULT CScanDbWaitDlg::OnNetScanInfo(WPARAM wParam, LPARAM lParam)
{
    CString* pStr = (CString*)(wParam);
    m_strNetScanInfo = _T("Net Scan : ")  + *pStr;


    CWnd* pWnd = GetDlgItem(IDC_INFO_NET);
    pWnd->SetWindowText(m_strNetScanInfo);


    delete pStr;
    return 1;
}


LRESULT CScanDbWaitDlg::OnBacnetComScanInfo(WPARAM wParam, LPARAM lParam)
{
    CString* pStr = (CString*)(wParam);
    m_strComScanInfo = _T("BacnetIP Scan : ") + *pStr;

    //UpdateData(FALSE);

    CWnd* pWnd = GetDlgItem(IDC_INFO_MSTP);
    pWnd->SetWindowText(m_strComScanInfo);

    delete pStr;
    return 1;
}
LRESULT CScanDbWaitDlg::OnComScanInfo(WPARAM wParam, LPARAM lParam)
{
    CString* pStr = (CString*)(wParam);
    m_strComScanInfo = _T("Com Port Scan : ") + *pStr;

    //UpdateData(FALSE);

    CWnd* pWnd = GetDlgItem(IDC_INFO_COM);
    pWnd->SetWindowText(m_strComScanInfo);

    delete pStr;
    return 1;
}


void CScanDbWaitDlg::Initial_List()
{
    m_scan_com_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    //m_scan_com_list.SetExtendedStyle(m_scan_com_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
    m_scan_com_list.SetExtendedStyle(m_scan_com_list.GetExtendedStyle() |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_scan_com_list.InsertColumn(SCAN_MODE, _T("Scanning Mode"), 120, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    //m_scan_com_list.InsertColumn(SCAN_BAUDRATE, _T("Baudrate"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_scan_com_list.InsertColumn(SCAN_SKIP, _T("Skip"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_scan_com_list.InsertColumn(SCAN_STATUS, _T("Status"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_scan_com_list.InsertColumn(SCAN_FOUND, _T("Reply"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_scan_com_list.InsertColumn(SCAN_NOTES, _T("Notes"), 280, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

    m_scan_dlg_hwnd = this->m_hWnd;
    g_hwnd_now = m_scan_dlg_hwnd;

    m_scan_com_list.SetWhetherShowBkCol(false);

    CRect list_rect,win_rect;
    m_scan_com_list.GetWindowRect(list_rect);
    ScreenToClient(&list_rect);
    ::GetWindowRect(m_scan_dlg_hwnd,win_rect);
    m_scan_com_list.Set_My_WindowRect(win_rect);
    m_scan_com_list.Set_My_ListRect(list_rect);
    m_scan_com_list.Dont_DT_Left();
    m_scan_com_list.DeleteAllItems();
    vector <CString> temp_serialport;
    GetSerialComPortNumber1(temp_serialport);
    int ncount = temp_serialport.size();





    CString scan_mode;
    scan_mode = _T("Ethernet Scan");
    m_scan_com_list.InsertItem(0,scan_mode);
    //m_scan_com_list.SetItemText(i*2,SCAN_BAUDRATE,_T("9600"));
    m_scan_com_list.SetItemText(0,SCAN_SKIP,_T("No"));
    m_scan_com_list.SetItemText(0,SCAN_STATUS,_T("Wait"));
    m_scan_com_list.SetItemText(0,SCAN_FOUND,_T("0"));

    //scan_mode = _T("Bacnet IP scan");
    //m_scan_com_list.InsertItem( 1,scan_mode);
    //m_scan_com_list.SetItemText( 1,SCAN_SKIP,_T("No"));
    //m_scan_com_list.SetItemText( 1,SCAN_STATUS,_T("Wait"));
    //m_scan_com_list.SetItemText( 1,SCAN_FOUND,_T("0"));


    for (int x=0; x<SCAN_ALL_COLUME; x++)
    {
        m_scan_com_list.SetItemBkColor( 1,x,LIST_ITEM_DEFAULT_BKCOLOR);

    }


    //m_scan_info.clear();
    //Scan_Info temp_scan_info;
    for (int i=0; i<ncount; i++)
    {
        CString temp_cs;
        int temp_port;
        temp_cs = temp_serialport.at(i).Right(temp_serialport.at(i).GetLength() - 3);
        temp_port = _wtoi(temp_cs);
        CString scan_mode;
        //scan_mode = temp_serialport.at(i) +_T("        ") + _T("  9600");
        //m_scan_com_list.InsertItem(i*2 +1,scan_mode);
        ////m_scan_com_list.SetItemText(i*2,SCAN_BAUDRATE,_T("9600"));
        //m_scan_com_list.SetItemText(i*2 +1,SCAN_SKIP,_T("No"));
        //m_scan_com_list.SetItemText(i*2 +1,SCAN_STATUS,_T("Wait"));
        //m_scan_com_list.SetItemText(i*2 +1,SCAN_FOUND,_T("0"));

        //scan_mode = temp_serialport.at(i) +_T("        ")+ _T("19200");
        //m_scan_com_list.InsertItem(i*2+2,scan_mode);
        ////m_scan_com_list.SetItemText(i*2+1,SCAN_BAUDRATE,_T("19200"));
        //m_scan_com_list.SetItemText(i*2+2,SCAN_SKIP,_T("No"));
        //m_scan_com_list.SetItemText(i*2+2,SCAN_STATUS,_T("Wait"));
        //m_scan_com_list.SetItemText(i*2+2,SCAN_FOUND,_T("0"));

        for(int baudrate = 0; baudrate<NUMBER_BAUDRATE; baudrate++)
        {
            scan_mode = temp_serialport.at(i) +_T("        ") + c_strBaudate[baudrate];
            m_scan_com_list.InsertItem(i*NUMBER_BAUDRATE+baudrate+1,scan_mode);
             //m_scan_com_list.SetItemText(i*2,SCAN_BAUDRATE,_T("9600"));
            m_scan_com_list.SetItemText(i*NUMBER_BAUDRATE +baudrate+1,SCAN_SKIP,_T("No"));
            m_scan_com_list.SetItemText(i*NUMBER_BAUDRATE +baudrate+1,SCAN_STATUS,_T("Wait"));
            m_scan_com_list.SetItemText(i*NUMBER_BAUDRATE +baudrate+1,SCAN_FOUND,_T("0"));
        }

        for (int x=0; x<SCAN_ALL_COLUME; x++)
        {
            m_scan_com_list.SetItemBkColor(i*NUMBER_BAUDRATE +1,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
            m_scan_com_list.SetItemBkColor(i*NUMBER_BAUDRATE + 2,x,LIST_ITEM_DEFAULT_BKCOLOR);

        }

    }

  
    scan_mode = _T("Bacnet MSTP");
    m_scan_com_list.InsertItem(ncount*NUMBER_BAUDRATE + 1,scan_mode);
    //m_scan_com_list.SetItemText(i*2,SCAN_BAUDRATE,_T("9600"));
    m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 1,SCAN_SKIP,_T("No"));
    m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 1,SCAN_STATUS,_T("Wait"));
    m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 1,SCAN_FOUND,_T("0"));

    scan_mode = _T("Remote Device");
    m_scan_com_list.InsertItem(ncount*NUMBER_BAUDRATE + 2,scan_mode);
    m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 2,SCAN_SKIP,_T("No"));
    m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 2,SCAN_STATUS,_T("Wait"));
    m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 2,SCAN_FOUND,_T("0"));


    for (int x=0; x<m_scan_info.size(); x++)
    {
#if 0	//不扫描的东西  也不显示 skip.很多小白客户 不看手册 反应给老毛 。老毛一句话 Auto.脑残。

        if(m_scan_info.at(x).scan_skip)
        {
            m_scan_com_list.SetItemText(x,SCAN_SKIP,_T("Yes"));
            m_scan_com_list.SetItemTextColor(x,SCAN_SKIP,RGB(255,0,0));

            m_scan_com_list.SetItemBkColor(x,SCAN_STATUS,RGB(255,0,0));
            m_scan_com_list.SetItemText(x,SCAN_STATUS,_T("Skip"));

        }
        else
        {
            m_scan_com_list.SetItemText(x,SCAN_SKIP,_T("No"));
        }
#endif
		m_scan_com_list.SetItemText(x,SCAN_SKIP,_T("No"));

    }


}
