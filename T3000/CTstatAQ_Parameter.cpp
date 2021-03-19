// CTstatAQ_Parameter.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CTstatAQ_Parameter.h"
#include "afxdialogex.h"
#include "global_function.h"
extern CString jpg_airlab;
// CTstatAQ_Parameter 对话框

IMPLEMENT_DYNAMIC(CTstatAQ_Parameter, CDialogEx)

CTstatAQ_Parameter::CTstatAQ_Parameter(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_AIRLAB_PARAMETER, pParent)
{

}

CTstatAQ_Parameter::~CTstatAQ_Parameter()
{
}

void CTstatAQ_Parameter::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTstatAQ_Parameter, CDialogEx)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_AIRLAB_LINE1, &CTstatAQ_Parameter::OnCbnSelchangeComboAirlabLine1)
	ON_CBN_SELCHANGE(IDC_COMBO_AIRLAB_LINE2, &CTstatAQ_Parameter::OnCbnSelchangeComboAirlabLine2)
	ON_CBN_SELCHANGE(IDC_COMBO_AIRLAB_LINE3, &CTstatAQ_Parameter::OnCbnSelchangeComboAirlabLine3)
	ON_CBN_SELCHANGE(IDC_COMBO_AIRLAB_LINE4, &CTstatAQ_Parameter::OnCbnSelchangeComboAirlabLine4)
	ON_BN_CLICKED(IDC_RADIO1, &CTstatAQ_Parameter::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTstatAQ_Parameter::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON_DONE, &CTstatAQ_Parameter::OnBnClickedButtonDone)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CTstatAQ_Parameter::OnBnClickedButtonDefault)
END_MESSAGE_MAP()


// CTstatAQ_Parameter 消息处理程序


BOOL CTstatAQ_Parameter::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitialUI();
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CTstatAQ_Parameter::InitialUI()
{
	if (product_register_value[TSTAT_AQ_BLANK_SCREEN] == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(0);
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE1))->ResetContent();

	((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE1))->AddString(AirLab_LCD[0]);
	((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE1))->AddString(AirLab_LCD[1]);
	((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE1))->AddString(AirLab_LCD[HIDE_IN_ARRAY]);


	((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE2))->ResetContent();
	for (int j = 0; j < sizeof(AirLab_LCD) / sizeof(AirLab_LCD[0]); j++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE2))->AddString(AirLab_LCD[j]);
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE3))->ResetContent();
	for (int j = 0; j < sizeof(AirLab_LCD) / sizeof(AirLab_LCD[0]); j++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE3))->AddString(AirLab_LCD[j]);
	}

	((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE4))->ResetContent();
	for (int j = 0; j < sizeof(AirLab_LCD) / sizeof(AirLab_LCD[0]); j++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE4))->AddString(AirLab_LCD[j]);
	}

	if ((product_register_value[TATAT_AQ_CONFIG_LINE1] >= 0) && (product_register_value[TATAT_AQ_CONFIG_LINE1] <= 3))
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE1))->SetWindowText(AirLab_LCD[product_register_value[TATAT_AQ_CONFIG_LINE1]]);
	}
	else if (product_register_value[TATAT_AQ_CONFIG_LINE1] == 20)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE1))->SetWindowText(AirLab_LCD[4]);
	}

	if ((product_register_value[TATAT_AQ_CONFIG_LINE2] >= 0) && (product_register_value[TATAT_AQ_CONFIG_LINE2] <= 3))
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE2))->SetWindowText(AirLab_LCD[product_register_value[TATAT_AQ_CONFIG_LINE2]]);
	}
	else if (product_register_value[TATAT_AQ_CONFIG_LINE2] == 20)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE2))->SetWindowText(AirLab_LCD[4]);
	}

	if ((product_register_value[TATAT_AQ_CONFIG_LINE3] >= 0) && (product_register_value[TATAT_AQ_CONFIG_LINE3] <= 3))
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE3))->SetWindowText(AirLab_LCD[product_register_value[TATAT_AQ_CONFIG_LINE3]]);
	}
	else if (product_register_value[TATAT_AQ_CONFIG_LINE3] == 20)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE3))->SetWindowText(AirLab_LCD[4]);
	}

	if ((product_register_value[TATAT_AQ_CONFIG_LINE4] >= 0) && (product_register_value[TATAT_AQ_CONFIG_LINE4] <= 3))
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE4))->SetWindowText(AirLab_LCD[product_register_value[TATAT_AQ_CONFIG_LINE4]]);
	}
	else if (product_register_value[TATAT_AQ_CONFIG_LINE4] == 20)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE4))->SetWindowText(AirLab_LCD[4]);
	}


}

void CTstatAQ_Parameter::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	int cx = 0;
	int cy = 0;
	CImage image;
	CRect rect;
	image.Load(jpg_airlab);
	cx = image.GetWidth();
	cy = image.GetHeight();

	//获取Picture Control控件的大小  
	GetDlgItem(IDC_STATIC_AIRLAB)->GetWindowRect(&rect);
	//将客户区选中到控件表示的矩形区域内  
	ScreenToClient(&rect);
	//窗口移动到控件表示的区域  
	GetDlgItem(IDC_STATIC_AIRLAB)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_STATIC_AIRLAB);
	pWnd->GetClientRect(&rect);
	CDC* pDc = NULL;
	pDc = pWnd->GetDC();
	image.Draw(pDc->m_hDC, rect);
	ReleaseDC(pDc);
}


void CTstatAQ_Parameter::OnCbnSelchangeComboAirlabLine1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_string;
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE1))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE1))->GetLBText(nSel, temp_string);
	int nvalue = 0;
	for (int i = 0; i < sizeof(AirLab_LCD) / sizeof(AirLab_LCD[0]); i++)
	{
		if (temp_string.CompareNoCase(AirLab_LCD[i]) == 0)
		{
			if(i == HIDE_IN_ARRAY)
				nvalue = 20;
			else
				nvalue = i;
			break;
		}
	}
	product_register_value[TATAT_AQ_CONFIG_LINE1] = nvalue;
	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CONFIG_LINE1, nvalue,
		product_register_value[TATAT_AQ_CONFIG_LINE1], this->m_hWnd, IDC_COMBO_AIRLAB_LINE1, _T(" Line 1 "));

}


void CTstatAQ_Parameter::OnCbnSelchangeComboAirlabLine2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_string;
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE2))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE2))->GetLBText(nSel, temp_string);
	int nvalue = 0;
	for (int i = 0; i < sizeof(AirLab_LCD) / sizeof(AirLab_LCD[0]); i++)
	{
		if (temp_string.CompareNoCase(AirLab_LCD[i]) == 0)
		{
			if (i == HIDE_IN_ARRAY)
				nvalue = 20;
			else
				nvalue = i;
			break;
		}
	}
	product_register_value[TATAT_AQ_CONFIG_LINE2] = nvalue;
	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CONFIG_LINE2, nvalue,
		product_register_value[TATAT_AQ_CONFIG_LINE2], this->m_hWnd, IDC_COMBO_AIRLAB_LINE2, _T(" Line 2 "));
}


void CTstatAQ_Parameter::OnCbnSelchangeComboAirlabLine3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_string;
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE3))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE3))->GetLBText(nSel, temp_string);
	int nvalue = 0;
	for (int i = 0; i < sizeof(AirLab_LCD) / sizeof(AirLab_LCD[0]); i++)
	{
		if (temp_string.CompareNoCase(AirLab_LCD[i]) == 0)
		{
			if (i == HIDE_IN_ARRAY)
				nvalue = 20;
			else
				nvalue = i;
			break;
		}
	}
	product_register_value[TATAT_AQ_CONFIG_LINE3] = nvalue;
	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CONFIG_LINE3, nvalue,
		product_register_value[TATAT_AQ_CONFIG_LINE3], this->m_hWnd, IDC_COMBO_AIRLAB_LINE3, _T(" Line 3 "));
}


void CTstatAQ_Parameter::OnCbnSelchangeComboAirlabLine4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_string;
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE4))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_AIRLAB_LINE4))->GetLBText(nSel, temp_string);
	int nvalue = 0;
	for (int i = 0; i < sizeof(AirLab_LCD) / sizeof(AirLab_LCD[0]); i++)
	{
		if (temp_string.CompareNoCase(AirLab_LCD[i]) == 0)
		{
			if (i == HIDE_IN_ARRAY)
				nvalue = 20;
			else
				nvalue = i;
			break;
		}
	}
	product_register_value[TATAT_AQ_CONFIG_LINE4] = nvalue;
	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CONFIG_LINE4, nvalue,
		product_register_value[TATAT_AQ_CONFIG_LINE4], this->m_hWnd, IDC_COMBO_AIRLAB_LINE4, _T(" Line 4 "));
}



void CTstatAQ_Parameter::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	product_register_value[TSTAT_AQ_BLANK_SCREEN] = 0;
	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_BLANK_SCREEN, 0,
		product_register_value[TSTAT_AQ_BLANK_SCREEN], this->m_hWnd, IDC_RADIO1, _T(" default normal display "));
}


void CTstatAQ_Parameter::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	product_register_value[TSTAT_AQ_BLANK_SCREEN] = 1;
	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TSTAT_AQ_BLANK_SCREEN, 1,
		product_register_value[TSTAT_AQ_BLANK_SCREEN], this->m_hWnd, IDC_RADIO1, _T(" blank screen "));
}


void CTstatAQ_Parameter::OnBnClickedButtonDone()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CTstatAQ_Parameter::OnBnClickedButtonDefault()
{
	// TODO: 在此添加控件通知处理程序代码
	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CONFIG_LINE1, 0,
		product_register_value[TATAT_AQ_CONFIG_LINE1], this->m_hWnd, NULL, _T(" Line 1 "));

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CONFIG_LINE2, 1,
		product_register_value[TATAT_AQ_CONFIG_LINE2], this->m_hWnd, NULL, _T(" Line 2 "));
	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CONFIG_LINE3, 2,
		product_register_value[TATAT_AQ_CONFIG_LINE3], this->m_hWnd, NULL, _T(" Line 3 "));
	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, TATAT_AQ_CONFIG_LINE4, 3,
		product_register_value[TATAT_AQ_CONFIG_LINE4], this->m_hWnd, NULL, _T(" Line 4 "));
	PostMessage(WM_CLOSE, NULL, NULL);
}
