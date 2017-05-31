// Enter_Value_BinaryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "Enter_Value_BinaryDlg.h"
#include "afxdialogex.h"


// CEnter_Value_BinaryDlg dialog

IMPLEMENT_DYNAMIC(CEnter_Value_BinaryDlg, CDialogEx)

CEnter_Value_BinaryDlg::CEnter_Value_BinaryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnter_Value_BinaryDlg::IDD, pParent)
{

}

CEnter_Value_BinaryDlg::~CEnter_Value_BinaryDlg()
{
}

void CEnter_Value_BinaryDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECK1, m_bool_check1);
    //  DDX_Check(pDX, IDC_CHECK10, m_m_bool_check2);
    DDX_Check(pDX, IDC_CHECK10, m_bool_check10);
    DDX_Check(pDX, IDC_CHECK11, m_bool_check11);
    DDX_Check(pDX, IDC_CHECK12, m_bool_check12);
    DDX_Check(pDX, IDC_CHECK13, m_bool_check13);
    DDX_Check(pDX, IDC_CHECK14, m_bool_check14);
    DDX_Check(pDX, IDC_CHECK15, m_bool_check15);
    DDX_Check(pDX, IDC_CHECK16, m_bool_check16);
    DDX_Check(pDX, IDC_CHECK2, m_bool_check2);
    DDX_Check(pDX, IDC_CHECK3, m_bool_check3);
    DDX_Check(pDX, IDC_CHECK4, m_bool_check4);
    DDX_Check(pDX, IDC_CHECK5, m_bool_check5);
    DDX_Check(pDX, IDC_CHECK6, m_bool_check6);
    DDX_Check(pDX, IDC_CHECK7, m_bool_check7);
    DDX_Check(pDX, IDC_CHECK8, m_bool_check8);
    DDX_Check(pDX, IDC_CHECK9, m_bool_check9);
}


BEGIN_MESSAGE_MAP(CEnter_Value_BinaryDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CEnter_Value_BinaryDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CEnter_Value_BinaryDlg message handlers


BOOL CEnter_Value_BinaryDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    
      m_bool_check1 =  m_Binary_Value[0];
      m_bool_check2 =  m_Binary_Value[1];
      m_bool_check3 =  m_Binary_Value[2];
      m_bool_check4 =  m_Binary_Value[3];
      m_bool_check5 =  m_Binary_Value[4];
      m_bool_check6 =  m_Binary_Value[5];
      m_bool_check7 =  m_Binary_Value[6];
      m_bool_check8 =  m_Binary_Value[7];
      m_bool_check9 =  m_Binary_Value[8];
      m_bool_check10 = m_Binary_Value[9];
      m_bool_check11 = m_Binary_Value[10];
      m_bool_check12 = m_Binary_Value[11];
      m_bool_check13 = m_Binary_Value[12];
      m_bool_check14 = m_Binary_Value[13];
      m_bool_check15 = m_Binary_Value[14];
      m_bool_check16 = m_Binary_Value[15];
      UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}


void CEnter_Value_BinaryDlg::OnBnClickedOk()
{
    
     UpdateData(TRUE);
      m_Binary_Value[0]=m_bool_check1;
      m_Binary_Value[1]=m_bool_check2;
      m_Binary_Value[2]=m_bool_check3;
      m_Binary_Value[3]=m_bool_check4;
      m_Binary_Value[4]=m_bool_check5;
      m_Binary_Value[5]=m_bool_check6;
      m_Binary_Value[6]=m_bool_check7;
      m_Binary_Value[7]=m_bool_check8;
      m_Binary_Value[8]=m_bool_check9;
      m_Binary_Value[9]=m_bool_check10;
     m_Binary_Value[10]=m_bool_check11;
     m_Binary_Value[11]=m_bool_check12;
     m_Binary_Value[12]=m_bool_check13;
     m_Binary_Value[13]=m_bool_check14;
     m_Binary_Value[14]=m_bool_check15;
     m_Binary_Value[15]=m_bool_check16;
     m_string_value =_T("");
     for(int i=0;i<16;i++){
         if (m_Binary_Value[i])
         {
             m_string_value+=_T("1");
         }
         else
         {
             m_string_value+=_T("0");
         }
     }
    CDialogEx::OnOK();
}
