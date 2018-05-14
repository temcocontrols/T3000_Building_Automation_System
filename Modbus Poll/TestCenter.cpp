// TestCenter.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "TestCenter.h"
#include "afxdialogex.h"
#include "ModbusDllforVc.h"
#include "global_function.h"

// TestCenter dialog

IMPLEMENT_DYNAMIC(TestCenter, CDialogEx)

TestCenter::TestCenter(CWnd* pParent /*=NULL*/)
    : CDialogEx(TestCenter::IDD, pParent)
{
    // HexArray[17] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',' '};
}

TestCenter::~TestCenter()
{

}

void TestCenter::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON_ADDLIST, m_addlist_btn);
    DDX_Control(pDX, IDC_BUTTON_COPY, m_copy_btn);
    DDX_Control(pDX, IDC_BUTTON_OPENLIST, m_openlist_btn);
    DDX_Control(pDX, IDC_BUTTON_SAVELIST, m_savelist_btn);
    DDX_Control(pDX, IDC_CHECK_ADD, m_add_check);
    DDX_Control(pDX, IDC_LIST2, m_list);
    DDX_Control(pDX, IDC_COMBO1, m_combox_send);
}


BEGIN_MESSAGE_MAP(TestCenter, CDialogEx)
    ON_BN_CLICKED(IDOK, &TestCenter::OnBnClickedOk)

END_MESSAGE_MAP()


// TestCenter message handlers

void TestCenter::OnBnClickedOk()
{
    CString sendText;
    CString temp;

    m_combox_send.GetWindowTextW(sendText);
    if (!CheckInputData(sendText))
    {
        AfxMessageBox (_T("Your input include no hex number !"));
    }

    CStringArray TextArray;
    SplitCStringA (TextArray,sendText,_T(" "));
    unsigned short *sendbuffer;
    if (TextArray.GetSize ()>0)
    {
        sendbuffer = new unsigned short(TextArray.GetSize ());
    }
    else
    {
        AfxMessageBox (_T("Your input is error!"));
        return;
    }

    for (int i = 0 ; i<TextArray.GetSize (); i++)
    {
        sendbuffer[i]=CStringToHex(TextArray[i]);
    }
    unsigned char senddata[128];
    unsigned char rvcddata[128];
    int sendlength,rvcdlength;
    register_critical_section.Lock();
    if (m_add_check.GetCheck () == 1)
    {
        SendData (sendbuffer,TextArray.GetSize (),senddata,rvcddata,&sendlength,&rvcdlength,TRUE);
    }
    else
    {
        SendData (sendbuffer,TextArray.GetSize (),senddata,rvcddata,&sendlength,&rvcdlength,FALSE);
    }
    register_critical_section.Unlock();
    CString  m_Tx=_T("Send:");
    for (int i=0; i<sendlength; i++)
    {
        temp.Format(_T("%02X "),senddata[i]);
        m_Tx+=temp;
    }
    int count = m_list.GetCount ();
    m_list.InsertString (count,m_Tx);



    m_Tx=_T("Rvcd:");
    for (int i=0; i<rvcdlength; i++)
    {
        temp.Format(_T("%02X "),rvcddata[i]);
        m_Tx+=temp;
    }
      count = m_list.GetCount ();
    m_list.InsertString (count,m_Tx);


    //CDialogEx::OnOK();
}

BOOL TestCenter::CheckInputData (CString strText)
{
    char  HexArray[17] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',' '};
    BOOL result=TRUE;
    for (int i =0; i<strText.GetLength (); i++)
    {
        char ch = strText.GetAt (i);
        BOOL one_result= FALSE;
        for (int K = 0 ; K< 17 ; K++)
        {
            if (ch == HexArray[K])
            {
                one_result = TRUE;
                break;
            }
        }

        result = one_result;

        if (!result)
        {
            return result;
        }

    }
    return result;
}
unsigned short TestCenter::CStringToHex(CString Text)
{

    unsigned short retValue = 0;
    for (int i =0; i<Text.GetLength (); i++)
    {
        char ch = Text.GetAt (i);
        unsigned short tempValue = HexCharToInt(ch);
        retValue=retValue<<4;
        retValue+=tempValue;
    }
    return retValue;
}
unsigned short TestCenter::HexCharToInt (char ch)
{
    char  HexArray[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    for (int K = 0 ; K< 16 ; K++)
    {
        if (ch == HexArray[K])
        {
            return K;
            //  break;
        }
    }

    return -1;
}



