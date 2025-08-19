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
        // Open file
        //���ļ�
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

  //  aryListSel stores the index values of selected columns, if you need to get the content, add the following statement:
  //  aryListSel�д�ľ���ѡ���е�indexֵ�������Ҫȡ���ݣ���������䣺
        CString str,content;
        for (int i= nCount-1; i>= 0; i--)
        {
            m_DataList.GetText(aryListSel.GetAt(i), (CString&)str);
            str+=_T("\n");
            content==str;
        }


//--------------------------------------------------------------------------------------------------------------------
     
    HGLOBAL hClip; 
    // Define a HGLOBAL handle variable to point to the allocated memory block
    //����һ��HGLOBAL�����������ָ�������ڴ��
    if (OpenClipboard())
    {
        // Clear clipboard contents
        EmptyClipboard();                            //���������������
        hClip=GlobalAlloc(GMEM_MOVEABLE,content.GetLength()+1); 
        // Allocate movable memory block on heap, program returns a memory handle
        //�ڶ��Ϸ�����ƶ����ڴ�飬���򷵻�һ���ڴ���
        // Define pointer variable pointing to character type
        char * buff;                                 //����ָ���ַ��͵�ָ�����
        buff=(char*)GlobalLock(hClip);
        // Lock the allocated memory block, convert memory block handle to a pointer, and increment the corresponding reference counter by 1
        //�Է�����ڴ����м��������ڴ����ת����һ��ָ��,������Ӧ�����ü�������1
        strcpy(buff,(char*)content.GetBuffer());
        // Copy user input data to pointer variable, actually copying to the allocated memory block
        //���û���������ݸ��Ƶ�ָ������У�ʵ���Ͼ��Ǹ��Ƶ�������ڴ����
        GlobalUnlock(hClip);
        // Data writing completed, perform unlock operation and decrease reference counter by 1
        //����д����ϣ����н����������������ü��������ּ�1
        SetClipboardData(CF_TEXT,hClip);
        // Put the memory block containing data into clipboard resource management
        //����������ݵ��ڴ�������������Դ������
        CloseClipboard();
        // Close clipboard, release clipboard resource occupancy
        //�رռ����壬�ͷż�������Դ��ռ��Ȩ
      
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
