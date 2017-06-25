// C:\Documents and Settings\software\My Documents\Visual Studio Projects\NWT3000_first\Tstat Manager\LedsDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "LedsDialog.h"
#include "global_function.h"
#include "global_variable_extern.h"

/////////////////////////////////////////////////////////////////////////////
// CLedsDialog dialog


	char *m_led_value[32]={//to 对应the mobus.h中的表示led内容的enum
		    "NONE",
			"RELAY1",
			"RELAY2",
			"RELAY3",
			"RELAY4",
			"RELAY5",
			"COASTING",
			"COOLING1",
			"COOLING2",
			"COOLING3",
			"HEATING1",
			"HEATING2",
			"HEATING3",
			"COOLING MODE",
			"HEATING MODE",
			"FAN OFF",
			"FAN LOW",
			"FAN MED",
			"FAN HI",
			"FAN AUTO",
			"HEAT1 OR COOL1",
			"HEAT2 OR COOL2",
			"HEAT3 OR COOL3",
			"COOL 1, 2, OR 3",
			"HEAT 1, 2, OR 3",
			"OCCUPANCY",
			"COOL OR HEAT 1,2,3",
			"COOL OR HEAT 2,3"
			"FAN_BUTTON",
			"UTIL",
			"HOLD",
			"MODE_BUTTON",
			};
// CLedsDialog 对话框

IMPLEMENT_DYNAMIC(CLedsDialog, CDialog)
CLedsDialog::CLedsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLedsDialog::IDD, pParent)
{
	m_address = g_tstat_id;
	m_led1_value = _T("");
	m_led2_value = _T("");
	m_led3_value = _T("");
	m_led4_value = _T("");
	m_led5_value = _T("");
	m_led6_value = _T("");
	m_led7_value = _T("");

}

CLedsDialog::~CLedsDialog()
{
}

void CLedsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADDRESS, m_address);
	DDX_CBString(pDX, IDC_LED1_COMBO, m_led1_value);
	DDX_CBString(pDX, IDC_LED2_COMBO, m_led2_value);
	DDX_CBString(pDX, IDC_LED3_COMBO, m_led3_value);
	DDX_CBString(pDX, IDC_LED4_COMBO, m_led4_value);
	DDX_CBString(pDX, IDC_LED5_COMBO, m_led5_value);
	DDX_CBString(pDX, IDC_LED6_COMBO, m_led6_value);
	DDX_CBString(pDX, IDC_LED7_COMBO, m_led7_value);
	DDX_Control(pDX, IDC_LED1_COMBO, m_led1Box);
	DDX_Control(pDX, IDC_LED2_COMBO, m_led2Box);
	DDX_Control(pDX, IDC_LED3_COMBO, m_led3Box);
	DDX_Control(pDX, IDC_LED4_COMBO, m_led4Box);
	DDX_Control(pDX, IDC_LED5_COMBO, m_led5Box);
	DDX_Control(pDX, IDC_LED6_COMBO, m_led6Box);
	DDX_Control(pDX, IDC_LED7_COMBO, m_led7Box);
}


BEGIN_MESSAGE_MAP(CLedsDialog, CDialog)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)	
	ON_WM_TIMER()

	ON_CBN_SELCHANGE(IDC_LED1_COMBO, OnSelendokLed1Combo)
	ON_CBN_SELCHANGE(IDC_LED2_COMBO, OnSelendokLed2Combo)
	ON_CBN_SELCHANGE(IDC_LED3_COMBO, OnSelendokLed3Combo)
	ON_CBN_SELCHANGE(IDC_LED4_COMBO, OnSelendokLed4Combo)
	ON_CBN_SELCHANGE(IDC_LED5_COMBO, OnSelendokLed5Combo)
	ON_CBN_SELCHANGE(IDC_LED6_COMBO, OnSelendokLed6Combo)
	ON_CBN_SELCHANGE(IDC_LED7_COMBO, OnSelendokLed7Combo)

	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CLedsDialog::OnBnClickedCancel()
{
	OnCancel();
}

void CLedsDialog::OnBnClickedOk()
{

}


void CLedsDialog::OnRefresh()
{
	int nItem;

	nItem=multi_register_value[204];
	if(nItem>=0)
		m_led1Box.SetCurSel(nItem);
	nItem=multi_register_value[205];
	if(nItem>=0)
		m_led2Box.SetCurSel(nItem);
	nItem=multi_register_value[206];
	if(nItem>=0)
		m_led3Box.SetCurSel(nItem);
	nItem=multi_register_value[207];
	if(nItem>=0)
		m_led4Box.SetCurSel(nItem);

	nItem=multi_register_value[208];
	if(nItem>=0)
		m_led5Box.SetCurSel(nItem);
	nItem=multi_register_value[209];
	if(nItem>=0)
		m_led6Box.SetCurSel(nItem);
	nItem=multi_register_value[210];
	if(nItem>=0)
		m_led7Box.SetCurSel(nItem);

}

void CLedsDialog::OnTimer(UINT nIDEvent) 
{

	CDialog::OnTimer(nIDEvent);
}

void CLedsDialog::OnSetfocusLed1Combo() 
{
}

void CLedsDialog::OnSetfocusLed2Combo() 
{
}

void CLedsDialog::OnSetfocusLed3Combo() 
{
}

void CLedsDialog::OnSetfocusLed4Combo() 
{

}

void CLedsDialog::OnSetfocusLed5Combo() 
{
}

void CLedsDialog::OnSetfocusLed6Combo() 
{

}

void CLedsDialog::OnSetfocusLed7Combo() 
{

}

void CLedsDialog::OnSelendokLed1Combo() 
{
	CComboBox *p_led1=(CComboBox *)GetDlgItem(IDC_LED1_COMBO);	
	write_one(g_tstat_id,204,p_led1->GetCurSel());//0
	
}

void CLedsDialog::OnSelendokLed2Combo() 
{

	CComboBox *p_led2=(CComboBox *)GetDlgItem(IDC_LED2_COMBO);	
	write_one(g_tstat_id,205,p_led2->GetCurSel());//0
	
}

void CLedsDialog::OnSelendokLed3Combo() 
{
	CComboBox *p_led3=(CComboBox *)GetDlgItem(IDC_LED3_COMBO);	
	write_one(g_tstat_id,206,p_led3->GetCurSel());//0
	
}

void CLedsDialog::OnSelendokLed4Combo() 
{
	CComboBox *p_led4=(CComboBox *)GetDlgItem(IDC_LED4_COMBO);	
	write_one(g_tstat_id,207,p_led4->GetCurSel());//0
	
}

void CLedsDialog::OnSelendokLed5Combo() 
{
	CComboBox *p_led5=(CComboBox *)GetDlgItem(IDC_LED5_COMBO);	
	write_one(g_tstat_id,208,p_led5->GetCurSel());//0
	
}

void CLedsDialog::OnSelendokLed6Combo() 
{
	CComboBox *p_led6=(CComboBox *)GetDlgItem(IDC_LED6_COMBO);	
	write_one(g_tstat_id,209,p_led6->GetCurSel());//0
	
}

void CLedsDialog::OnSelendokLed7Combo() 
{
	CComboBox *p_led7=(CComboBox *)GetDlgItem(IDC_LED7_COMBO);	
	write_one(g_tstat_id,210,p_led7->GetCurSel());//0	
}

void CLedsDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void CLedsDialog::OnDestroy()
{
	CDialog::OnDestroy();

}

BOOL CLedsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	OnRefresh();
	return TRUE;  // return TRUE unless you set the focus to a control
}

