// T3RangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "T3RangeDlg.h"
#include "afxdialogex.h"



CString Range_Units[15] =
{
    _T("-"),
    _T("TYPE2 10K C"),
    _T("TYPE2 10K F"),
    _T("0-100%"),
    _T("ON/OFF"),
    _T("OFF/ON"),
    _T("Pulse Count (Slow 1Hz)"),
    _T("Lighting Control"),
    _T("TYPE3 10K C"),
    _T("TYPE3 10K F"),
    _T("NO USE"),
    _T("0-5V"),
    _T("0-10V"),
    _T("4-20 ma"),
    _T("Pulse Count (Fast 100Hz)")
};

// CT3RangeDlg dialog

IMPLEMENT_DYNAMIC(CT3RangeDlg, CDialog)

CT3RangeDlg::CT3RangeDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CT3RangeDlg::IDD, pParent)
    , m_current_range(0)
{
    m_speed_count = 0;
}

CT3RangeDlg::~CT3RangeDlg()
{
}

void CT3RangeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_RANGE_UNITE_SHOW, m_show_unit);
    DDX_Control(pDX, IDC_STATIC_SELECT_RANGE, m_rang_pic);
    DDX_Control(pDX, IDC_STATIC_RANGE_ENTER_UNITS, m_static_range_units_select);
    DDX_Text(pDX, IDC_EDIT_RANGE_SELECT, m_current_range);

}


BEGIN_MESSAGE_MAP(CT3RangeDlg, CDialog)

    ON_BN_CLICKED(IDC_RADIO_T_0, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_1, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_2, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_3, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_4, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_5, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_6, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_7, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_8, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_9, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_10, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_11, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_12, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_13, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_BN_CLICKED(IDC_RADIO_T_14, &CT3RangeDlg::OnBnClickedClick_Radio)
    ON_EN_KILLFOCUS(IDC_EDIT_RANGE_SELECT, &CT3RangeDlg::OnEnKillfocusEditRangeSelect)
END_MESSAGE_MAP()


// CT3RangeDlg message handlers


BOOL CT3RangeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    
    m_show_unit.ShowWindow(FALSE);
    m_show_unit.SetWindowTextW(_T(""));
    m_show_unit.textColor(RGB(255,0,0));
    m_show_unit.setFont(24,12,NULL,_T("Arial"));
    m_input_Analog_select=m_current_range;
    Initial_window();


    ((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetFocus();

    if (m_speed_count == 1)
    {
        GetDlgItem (IDC_RADIO_T_6)->SetWindowTextW(CString("6 ")+Range_Units[6]);
        GetDlgItem (IDC_RADIO_T_14)->SetWindowTextW(CString("14 ")+Range_Units[14]);
        GetDlgItem (IDC_RADIO_T_6)->EnableWindow (FALSE);
        GetDlgItem (IDC_RADIO_T_14)->EnableWindow (TRUE);
    }
    else
    {
        GetDlgItem (IDC_RADIO_T_6)->SetWindowTextW(CString("6 ")+Range_Units[6]);
        GetDlgItem (IDC_RADIO_T_14)->SetWindowTextW(CString("14 ")+Range_Units[14]);
        GetDlgItem (IDC_RADIO_T_6)->EnableWindow (TRUE);
        GetDlgItem (IDC_RADIO_T_14)->EnableWindow (FALSE);
    }
    return TRUE;
    // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CT3RangeDlg::Initial_window()
{
    CString temp_cs;
    m_show_unit.ShowWindow(TRUE);
    temp_cs.Format(_T("%d"),m_input_Analog_select);
    ((CEdit *)GetDlgItem(IDC_EDIT_RANGE_SELECT))->SetWindowTextW(temp_cs);
    for (int i=IDC_RADIO_T_0; i<IDC_STATIC_SPLIT_WINDOW; i++)
    {
        ((CButton *)GetDlgItem(i))->SetCheck(0);
    }
    ((CButton *)GetDlgItem(IDC_RADIO_T_0+m_input_Analog_select))->SetCheck(1);


    m_show_unit.SetWindowTextW(Range_Units[m_input_Analog_select]);


    CRect c13;
    GetDlgItem(IDC_RADIO_T_0 + m_input_Analog_select)->GetWindowRect(c13);   //获取控件的位置 ，饼调整位置;
    ScreenToClient(c13);
    m_rang_pic.SetWindowPos(NULL,c13.left - 40,c13.top - 4,0,0,SWP_NOZORDER|SWP_NOSIZE);
    m_rang_pic.Invalidate(TRUE);
}

LRESULT CT3RangeDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    
    if (message==WM_T3_RANGE)
    {

        Initial_window();
    }
    return CDialog::WindowProc(message, wParam, lParam);
}
BOOL   CT3RangeDlg::PreTranslateMessage(MSG* pMsg)
{




    if(pMsg->message == WM_KEYDOWN  )
    {
        if(pMsg->wParam == VK_RETURN)
        {
            if(GetFocus()==GetDlgItem(IDC_EDIT_RANGE_SELECT))
            {
                CWnd *temp_focus=GetFocus();	//Maurice require ,click enter and the cursor still in this edit or combobox.
                GetDlgItem(IDC_EDIT1_TEST)->SetFocus();
                temp_focus->SetFocus();
            }



            return 1;
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}
void CT3RangeDlg::OnBnClickedClick_Radio()
{
    m_input_Analog_select = 0;
    for (int i=IDC_RADIO_T_0; i<=IDC_RADIO_T_14; i++,m_input_Analog_select++)
    {
        if (((CButton *)GetDlgItem(i))->GetCheck()==1)
        {
            break;
        }
    }

    m_current_range=m_input_Analog_select;
    PostMessage(WM_T3_RANGE,0,0);
}


void CT3RangeDlg::OnEnKillfocusEditRangeSelect()
{
    UpdateData();
    m_input_Analog_select = m_current_range;
    PostMessage(WM_T3_RANGE,0,0);
}
