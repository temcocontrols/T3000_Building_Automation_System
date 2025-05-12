// CommunicationTrafficDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "CommunicationTrafficDlg.h"
#include "afxdialogex.h"
 
#include "global_variable_extern.h"

// CCommunicationTrafficDlg dialog

IMPLEMENT_DYNAMIC(CCommunicationTrafficDlg, CDialogEx)

CCommunicationTrafficDlg::CCommunicationTrafficDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCommunicationTrafficDlg::IDD, pParent)
{

}

CCommunicationTrafficDlg::~CCommunicationTrafficDlg()
{

}

void CCommunicationTrafficDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_INFOR, m_DataList);
    DDX_Control(pDX, IDC_COPY, m_btn_copy);
    DDX_Control(pDX, IDC_SAVE, m_btn_save);
    DDX_Control(pDX, IDC_CONTINUE, m_btn_stop);
}


BEGIN_MESSAGE_MAP(CCommunicationTrafficDlg, CDialogEx)
	ON_WM_SIZE()
	ON_MESSAGE(WM_TRAFFIC_DATA_WINDOW,OnTrafficDataWindow)
	ON_BN_CLICKED(IDOK, &CCommunicationTrafficDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CONTINUE, &CCommunicationTrafficDlg::OnBnClickedContinue)
	ON_BN_CLICKED(IDC_CLEAR, &CCommunicationTrafficDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_SAVE, &CCommunicationTrafficDlg::OnBnClickedSave)
//	ON_BN_CLICKED(IDC_CONTINUE2, &CCommunicationTrafficDlg::OnBnClickedContinue2)
ON_BN_CLICKED(IDC_COPY, &CCommunicationTrafficDlg::OnBnClickedCopy)
ON_WM_CLOSE()
//ON_WM_SHOWWINDOW()
ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CCommunicationTrafficDlg message handlers
BOOL CCommunicationTrafficDlg::OnInitDialog(){
	m_is_pause = false;
    h_data_traffic_window=this->m_hWnd;
    //if (!m_is_pause)
    //{
    //    m_btn_copy.EnableWindow(FALSE);
    //    m_btn_save.EnableWindow(FALSE);
    //    m_btn_stop.EnableWindow(FALSE);
    //    m_btn_stop.SetWindowText(_T("Stop"));
    //}
    //else
    //{
    //    m_btn_stop.SetWindowText(_T("Continue"));
    //}
return	CDialogEx::OnInitDialog();
}

void CCommunicationTrafficDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType==SIZE_RESTORED)
	{

		CRect ViewRect;
		GetClientRect(&ViewRect);
		TRACE(_T(" View:T=%d,B=%d,L=%d,R=%d\n"),ViewRect.top,ViewRect.bottom,ViewRect.left,ViewRect.right);
		 
		if (m_DataList.GetSafeHwnd())
		{
			m_DataList.MoveWindow(CRect(0,40,ViewRect.Width(),ViewRect.Height()),TRUE);
		}

	}
}


afx_msg LRESULT CCommunicationTrafficDlg::OnTrafficDataWindow(WPARAM wParam, LPARAM lParam)
{
	if(m_is_pause||!g_online)
		return 0;
	CString temp_cs;
	temp_cs.Format(_T("%s"),(wchar_t *)wParam);
    m_DataList.AddString(temp_cs);
    m_DataList.SetTopIndex(m_DataList.GetCount() - 1);
	return 0;
}


void CCommunicationTrafficDlg::OnBnClickedOk()
{
	
	//CDialogEx::OnOK();
	 g_is_show_Data_Traffic_Window=FALSE;
	 m_DataList.ResetContent();
	 ShowWindow(SW_HIDE);
}


void CCommunicationTrafficDlg::OnBnClickedContinue()
{
// 	if(m_is_pause)
// 	{
// 		m_is_pause = false;
// 		SetDlgItemTextW(IDC_CONTINUE,_T("Stop"));
// 	}
// 	else
// 	{
// 		m_is_pause = true;
// 		SetDlgItemTextW(IDC_CONTINUE,_T("Continue"));
// 	}
    m_is_pause=!m_is_pause;
    if (!m_is_pause)
    {
        m_btn_copy.EnableWindow(FALSE);
        m_btn_save.EnableWindow(FALSE);
        // m_btn_stop.EnableWindow(FALSE);
        m_btn_stop.SetWindowText(_T("Stop"));
    }
    else
    {
        m_btn_copy.EnableWindow(TRUE);
        m_btn_save.EnableWindow(TRUE);
        m_btn_stop.SetWindowText(_T("Continue"));
    }

}


void CCommunicationTrafficDlg::OnBnClickedClear()
{
	 m_DataList.ResetContent();
}


void CCommunicationTrafficDlg::OnBnClickedSave()
{
    if (m_is_pause)
    {
    if (m_DataList.GetCount()!=0)
    {
        CString strFilter = _T("save File|*.txt|all File|*.*||");
        CFileDialog dlg(FALSE,_T("Save......"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
        dlg.DoModal();
         
       CString m_strLogFileName ;
       m_strLogFileName=dlg.GetPathName();
       CStdioFile* m_plogFile=new CStdioFile;
        if(m_plogFile->Open(m_strLogFileName.GetString(),CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate ))
        {
            for (int i=0;i<m_DataList.GetCount();i++)
            {
                CString logInfo;
                m_DataList.GetText(i,logInfo);
                m_plogFile->WriteString(logInfo+_T("\n"));
            }

            m_plogFile->Close();
        }


        delete m_plogFile;
        //打开文件
        //LPCSTR filename=m_strLogFileName.GetBuffer();
        //ShellExecute(NULL, "open","Log_info.txt",NULL, NULL, SW_SHOWNORMAL); 
        ShellExecute(this->m_hWnd, _T("open"), m_strLogFileName, NULL, NULL, SW_SHOWNORMAL);
    }
    }
    
	 
}

void CCommunicationTrafficDlg::OnBnClickedCopy()
{
//--------------------------------------------------------------------------------------------------------------------

   
    CArray <int, int> aryListSel;
    int nCount= m_DataList.GetSelCount();
    aryListSel.SetSize(nCount);
    m_DataList.GetSelItems(nCount, aryListSel.GetData()); 

  //  aryListSel中存的就是选中列的index值，如果需要取内容，加以下语句：
        CString str,content;
        for (int i= nCount-1; i>= 0; i--)
        {
            m_DataList.GetText(aryListSel.GetAt(i), (CString&)str);
            str+=_T("\n");
            content==str;
        }


//--------------------------------------------------------------------------------------------------------------------
     
    HGLOBAL hClip; 
    //定义一个HGLOBAL句柄变量用来指向分配的内存块
    if (OpenClipboard())
    {
        EmptyClipboard();                            //将剪贴板内容清空
        hClip=GlobalAlloc(GMEM_MOVEABLE,content.GetLength()+1); 
        //在堆上分配可移动的内存块，程序返回一个内存句柄
        char * buff;                                 //定义指向字符型的指针变量
        buff=(char*)GlobalLock(hClip);
        //对分配的内存块进行加锁，将内存块句柄转化成一个指针,并将相应的引用计数器加1
        strcpy(buff,(char*)content.GetBuffer());
        //将用户输入的数据复制到指针变量中，实际上就是复制到分配的内存块中
        GlobalUnlock(hClip);
        //数据写入完毕，进行解锁操作，并将引用计数器数字减1
        SetClipboardData(CF_TEXT,hClip);
        //将存放有数据的内存块放入剪贴板的资源管理中
        CloseClipboard();
        //关闭剪贴板，释放剪贴板资源的占用权
      
    }
//--------------------------------------------------------------------------------------------------------------------
}


void CCommunicationTrafficDlg::OnClose()
{
	 
	g_is_show_Data_Traffic_Window=FALSE;
	 m_DataList.ResetContent();
	ShowWindow(SW_HIDE);
	//CDialogEx::OnClose();
}


//void CCommunicationTrafficDlg::OnShowWindow(BOOL bShow, UINT nStatus)
//{
//    CDialogEx::OnShowWindow(bShow, nStatus);
//
//    
//}


void CCommunicationTrafficDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialogEx::OnShowWindow(bShow, nStatus);
    if (!m_is_pause)
    {
        m_btn_copy.EnableWindow(FALSE);
        m_btn_save.EnableWindow(FALSE);
       // m_btn_stop.EnableWindow(FALSE);
        m_btn_stop.SetWindowText(_T("Stop"));
    }
    else
    {
        m_btn_copy.EnableWindow(TRUE);
        m_btn_save.EnableWindow(TRUE);
        m_btn_stop.SetWindowText(_T("Continue"));
    }
    
}
