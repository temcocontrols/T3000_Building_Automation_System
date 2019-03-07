// ScanDbWaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ScanDbWaitDlg.h"
#include "TStatScanner.h"

// CScanDbWaitDlg dialog
CRect Scan_rect;
extern HANDLE * hScanTCPData ; //用于网络多网络同时扫描
IMPLEMENT_DYNAMIC(CScanDbWaitDlg, CDialog)

CScanDbWaitDlg::CScanDbWaitDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CScanDbWaitDlg::IDD, pParent)
{
    m_pScaner = NULL;
    g_bCancelScan=FALSE;
    n_time_count = 0;
}

CScanDbWaitDlg::~CScanDbWaitDlg()
{
}

void CScanDbWaitDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_INFOSTATIC, m_strInfopromp);
    DDX_Control(pDX, IDC_LIST_SCAN, m_scan_com_list);
    DDX_Control(pDX, IDC_INFOSTATIC, m_waiting_title);
}


BEGIN_MESSAGE_MAP(CScanDbWaitDlg, CDialog)
    ON_BN_CLICKED(IDOK, &CScanDbWaitDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CScanDbWaitDlg::OnBnClickedCancel)
    ON_BN_CLICKED(IDC_EXITBUTTON, &CScanDbWaitDlg::OnBnClickedExitbutton)
    ON_WM_TIMER()

    ON_WM_SIZE()
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
    
    //OnOK();
}

void CScanDbWaitDlg::OnBnClickedCancel()
{
    
    //OnCancel();
    ;
}

extern HANDLE * hScanComData ; //用于串口多线程同时扫描
void CScanDbWaitDlg::OnBnClickedExitbutton()
{
    
    g_bCancelScan=TRUE;


	//pScanner->m_bNetScanFinish = TRUE; // at this time, two thread end, all scan end
	TerminateThread(hwait_scan_thread, 0);
    TerminateThread(m_pScaner->m_pScanTCP_to_485Thread, 0);
    for (int j = 0; j < m_pScaner->m_szComs.size(); j++)
    {
        if (hScanComData[j] != NULL)
        {
            TerminateThread(hScanComData[j], 0);
            hScanComData[j] = NULL;
        }
    }
    if (m_pScaner->m_pScanBacnetIPThread != NULL)
    {
        TerminateThread(m_pScaner->m_pScanBacnetIPThread, 0);
        
    }
    for (int i = 0; i < controller_counter; i++)
    {
        if (hScanTCPData[i] != NULL)
        {
            TerminateThread(hScanTCPData[i], 0);
            hScanTCPData[i] = NULL;
        }
    }
    m_pScaner->StopScan();
    Sleep(1000);
	m_pScaner->SendScanEndMsg();

    OnCancel();
    //CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    //::PostMessage(pFrame->m_hWnd,WM_MYMSG_REFRESHBUILDING,0,0);
	//scaning_mode = false;
}
HWND scan_wait_dlg;
BOOL CScanDbWaitDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_waiting_title.textColor(RGB(20, 20, 20));
    //m_edit_display.bkColor(RGB(255, 255, 255));
    m_waiting_title.setFont(20, 14, NULL, _T("Arial"));

    g_bCancelScan=FALSE;
    SetTimer(1,100,NULL);
    SetTimer(2,200,NULL);
    Initial_List();
    GetDlgItem(IDC_STATIC_SCAN_PIC)->GetWindowRect(Scan_rect);
    ScreenToClient(Scan_rect);
	scan_wait_dlg = this->m_hWnd;
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
                if (m_pScaner->m_saving_data == false)
                    m_strInfopromp = strTip;
                else
                {
                    n_time_count++;
                    int ncount = n_time_count / 20;
                    if (ncount >= 3)
                        ncount = 3;
                    m_strInfopromp = Scan_Ret_Info[ncount] + m_strDot;
                }
                
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
                case SCAN_STATUS_DETECTING:
                    m_scan_com_list.SetItemText(x, SCAN_STATUS, _T("Detecting"));
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



void CScanDbWaitDlg::Initial_List()
{
    m_scan_com_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
    //m_scan_com_list.SetExtendedStyle(m_scan_com_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
    m_scan_com_list.SetExtendedStyle(m_scan_com_list.GetExtendedStyle() |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
    m_scan_com_list.InsertColumn(SCAN_MODE, _T("Scanning Mode"), 120, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
    //m_scan_com_list.InsertColumn(SCAN_BAUDRATE, _T("Baudrate"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    //m_scan_com_list.InsertColumn(SCAN_SKIP, _T("Skip"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_scan_com_list.InsertColumn(SCAN_STATUS, _T("Status"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_scan_com_list.InsertColumn(SCAN_FOUND, _T("Reply"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
    m_scan_com_list.InsertColumn(SCAN_NOTES, _T("Notes"), 480, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

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
    //m_scan_com_list.SetItemText(0,SCAN_SKIP,_T("No"));
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
            //m_scan_com_list.SetItemText(i*NUMBER_BAUDRATE +baudrate+1,SCAN_SKIP,_T("No"));
            m_scan_com_list.SetItemText(i*NUMBER_BAUDRATE +baudrate+1,SCAN_STATUS,_T("Wait"));
            m_scan_com_list.SetItemText(i*NUMBER_BAUDRATE +baudrate+1,SCAN_FOUND,_T("0"));
        }

        for (int x=0; x<SCAN_ALL_COLUME; x++)
        {
            m_scan_com_list.SetItemBkColor(i*NUMBER_BAUDRATE +1,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
            m_scan_com_list.SetItemBkColor(i*NUMBER_BAUDRATE + 2,x,LIST_ITEM_DEFAULT_BKCOLOR);

        }

    }

  
    //scan_mode = _T("Bacnet MSTP");
    //m_scan_com_list.InsertItem(ncount*NUMBER_BAUDRATE + 1,scan_mode);
    ////m_scan_com_list.SetItemText(i*2,SCAN_BAUDRATE,_T("9600"));
    //m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 1,SCAN_SKIP,_T("No"));
    //m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 1,SCAN_STATUS,_T("Wait"));
    //m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 1,SCAN_FOUND,_T("0"));

    //scan_mode = _T("Remote Device");
    //m_scan_com_list.InsertItem(ncount*NUMBER_BAUDRATE + 2,scan_mode);
    //m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 2,SCAN_SKIP,_T("No"));
    //m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 2,SCAN_STATUS,_T("Wait"));
    //m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 2,SCAN_FOUND,_T("0"));

	scan_mode = _T(" ");
	m_scan_com_list.InsertItem(ncount*NUMBER_BAUDRATE + 1,scan_mode);
    m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 1, SCAN_MODE, _T("Bacnet     MSTP"));
	m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 1,SCAN_STATUS, _T("Wait"));
	m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 1,SCAN_FOUND, _T("0"));
	if(b_remote_connection)
	{
		scan_mode = _T("Remote IP Device");
	}
	else
		scan_mode = _T("");
	m_scan_com_list.InsertItem(ncount*NUMBER_BAUDRATE + 2,scan_mode);
	//m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 2,SCAN_SKIP,_T(" "));
	m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 2,SCAN_STATUS,_T(" "));
	m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 2,SCAN_FOUND,_T(" "));

	//if(b_remote_connection)
	//{
	//	 m_scan_com_list.SetItemText(ncount*NUMBER_BAUDRATE + 2,SCAN_SKIP,_T("No"));
	//}

    for (int x=0; x<m_scan_info.size(); x++)
    {
#if 0	

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

		m_scan_com_list.SetItemText(x,SCAN_SKIP,_T("No"));
#endif
    }

    for (int j = 0; j < 100; j++)
    {
        m_scan_com_list.InsertItem(ncount*NUMBER_BAUDRATE + 3 + j, scan_mode);
    }
    

}


void CScanDbWaitDlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    CRect temp_mynew_rect;
    ::GetWindowRect(this->m_hWnd, &temp_mynew_rect);	//获取 view的窗体大小;

    ::SetWindowPos(m_scan_com_list.m_hWnd, NULL, temp_mynew_rect.left, temp_mynew_rect.top, temp_mynew_rect.Width() -30, temp_mynew_rect.Height(), SWP_NOMOVE);
}
