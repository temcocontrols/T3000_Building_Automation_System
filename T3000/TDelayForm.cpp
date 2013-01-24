// Tstat Manager\TDelayForm.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "TDelayForm.h"
#include "globle_function.h"
#include "global_variable_extern.h"
// TDelayForm 对话框

IMPLEMENT_DYNAMIC(TDelayForm, CDialog)
TDelayForm::TDelayForm(CWnd* pParent /*=NULL*/)
	: CDialog(TDelayForm::IDD, pParent)
{
	m_address = g_tstat_id;
	m_output1_delay_off = _T("");
	m_output1_delay_on = _T("");
	m_output2_delay_off = _T("");
	m_output3_delay_off = _T("");
	m_output4_delay_off = _T("");
	m_output5_delay_off = _T("");
	m_output2_delay_on = _T("");
	m_output3_delay_on = _T("");
	m_output4_delay_on = _T("");
	m_output5_delay_on = _T("");
	m_cycling = _T("");
	m_change_over = _T("");
}

TDelayForm::~TDelayForm()
{
}

void TDelayForm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	///DDX_Text(pDX, IDC_ADDRESS, m_address);
	DDX_Text(pDX, IDC_OUTPUT1_DELAY_OFF_EDIT, m_output1_delay_off);
	DDX_Text(pDX, IDC_OUTPUT1_DELAY_ON_EDIT, m_output1_delay_on);
	DDX_Text(pDX, IDC_OUTPUT2_DELAY_OFF_EDIT, m_output2_delay_off);
	DDX_Text(pDX, IDC_OUTPUT3_DELAY_OFF_EDIT, m_output3_delay_off);
	DDX_Text(pDX, IDC_OUTPUT4_DELAY_OFF_EDIT, m_output4_delay_off);
	DDX_Text(pDX, IDC_OUTPUT5_DELAY_OFF_EDIT, m_output5_delay_off);
	DDX_Text(pDX, IDC_OUTPUT2_DELAY_ON_EDIT, m_output2_delay_on);
	DDX_Text(pDX, IDC_OUTPUT3_DELAY_ON_EDIT, m_output3_delay_on);
	DDX_Text(pDX, IDC_OUTPUT4_DELAY_ON_EDIT, m_output4_delay_on);
	DDX_Text(pDX, IDC_OUTPUT5_DELAY_ON_EDIT, m_output5_delay_on);
	DDX_Text(pDX, IDC_CYCLING_EDIT, m_cycling);
	DDX_Text(pDX, IDC_CHANGE_OVER_EDIT, m_change_over);
}


BEGIN_MESSAGE_MAP(TDelayForm, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_FRESHBUTTON, OnFresh)
	ON_EN_KILLFOCUS(IDC_OUTPUT1_DELAY_OFF_EDIT, OnKillfocusOutput1DelayOffEdit)
	ON_EN_KILLFOCUS(IDC_OUTPUT2_DELAY_OFF_EDIT, OnKillfocusOutput2DelayOffEdit)
	ON_EN_KILLFOCUS(IDC_OUTPUT3_DELAY_OFF_EDIT, OnKillfocusOutput3DelayOffEdit)
	ON_EN_KILLFOCUS(IDC_OUTPUT4_DELAY_OFF_EDIT, OnKillfocusOutput4DelayOffEdit)
	ON_EN_KILLFOCUS(IDC_OUTPUT5_DELAY_OFF_EDIT, OnKillfocusOutput5DelayOffEdit)
	ON_EN_KILLFOCUS(IDC_OUTPUT1_DELAY_ON_EDIT, OnKillfocusOutput1DelayOnEdit)
	ON_EN_KILLFOCUS(IDC_OUTPUT2_DELAY_ON_EDIT, OnKillfocusOutput2DelayOnEdit)
	ON_EN_KILLFOCUS(IDC_OUTPUT3_DELAY_ON_EDIT, OnKillfocusOutput3DelayOnEdit)
	ON_EN_KILLFOCUS(IDC_OUTPUT4_DELAY_ON_EDIT, OnKillfocusOutput4DelayOnEdit)
	ON_EN_KILLFOCUS(IDC_OUTPUT5_DELAY_ON_EDIT, OnKillfocusOutput5DelayOnEdit)
	ON_EN_KILLFOCUS(IDC_CYCLING_EDIT, OnKillfocusCyclingEdit)
	ON_EN_KILLFOCUS(IDC_CHANGE_OVER_EDIT, OnKillfocusChangeOverEdit)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_EN_SETFOCUS(IDC_OUTPUT1_DELAY_OFF_EDIT, OnSetfocusOutput1DelayOffEdit)
	ON_EN_SETFOCUS(IDC_OUTPUT2_DELAY_OFF_EDIT, OnSetfocusOutput2DelayOffEdit)
	ON_EN_SETFOCUS(IDC_OUTPUT3_DELAY_OFF_EDIT, OnSetfocusOutput3DelayOffEdit)
	ON_EN_SETFOCUS(IDC_OUTPUT4_DELAY_OFF_EDIT, OnSetfocusOutput4DelayOffEdit)
	ON_EN_SETFOCUS(IDC_OUTPUT5_DELAY_OFF_EDIT, OnSetfocusOutput5DelayOffEdit)
	ON_EN_SETFOCUS(IDC_OUTPUT1_DELAY_ON_EDIT, OnSetfocusOutput1DelayOnEdit)
	ON_EN_SETFOCUS(IDC_OUTPUT2_DELAY_ON_EDIT, OnSetfocusOutput2DelayOnEdit)
	ON_EN_SETFOCUS(IDC_OUTPUT3_DELAY_ON_EDIT, OnSetfocusOutput3DelayOnEdit)
	ON_EN_SETFOCUS(IDC_OUTPUT4_DELAY_ON_EDIT, OnSetfocusOutput4DelayOnEdit)
	ON_EN_SETFOCUS(IDC_OUTPUT5_DELAY_ON_EDIT, OnSetfocusOutput5DelayOnEdit)
	ON_EN_SETFOCUS(IDC_CYCLING_EDIT, OnSetfocusCyclingEdit)
	ON_EN_SETFOCUS(IDC_CHANGE_OVER_EDIT, OnSetfocusChangeOverEdit)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// TDelayForm 消息处理程序

void TDelayForm::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
}

void TDelayForm::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
int TDelayForm::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}

void TDelayForm::OnFresh() 
{
	// TODO: Add your control notification handler code here
			m_output1_delay_off.Format(_T("%d"),multi_register_value[191]);
			m_output2_delay_off.Format(_T("%d"), multi_register_value[192]);
			m_output3_delay_off.Format(_T("%d"), multi_register_value[193]);
			m_output4_delay_off.Format(_T("%d"), multi_register_value[194]);
			m_output5_delay_off.Format(_T("%d"), multi_register_value[195]);

			m_output1_delay_on.Format(_T("%d"),multi_register_value[196]);
			m_output2_delay_on.Format(_T("%d"),multi_register_value[197]);
			m_output3_delay_on.Format(_T("%d"),multi_register_value[198]);
			m_output4_delay_on.Format(_T("%d"),multi_register_value[199]);
			m_output5_delay_on.Format(_T("%d"),multi_register_value[200]);

			m_cycling.Format(_T("%d"),multi_register_value[201]);
			m_change_over.Format(_T("%d"),multi_register_value[202]);
		UpdateData(false);
///////////////////////////////////////////////////////////////////////////
	
}

void TDelayForm::OnWriteChanges() 
{//do nothing ,is default button ,to explain ENTER
	// TODO: Add your control notification handler code here

}
/*
int TDelayForm::float_CString_to_atoi(CString a,char *p)
{//turn float string to atoi
	//attention the number of char must below 30
	a.Remove('.');
	char *w=p;
	for(int b=0;b<a.GetLength();b++)
		*w++ = a.GetAt(b);
	*w++ = '\0';
	w=p;
	return (atoi(w));
}
*/
void TDelayForm::OnKillfocusOutput1DelayOffEdit() 
{
	UpdateData();
	unsigned short m_get_to_write;
	if(m_output1_delay_off.IsEmpty())
		return;
	m_get_to_write=_wtoi(m_output1_delay_off);
	write_one(g_tstat_id,191,m_get_to_write);//0
}

void TDelayForm::OnKillfocusOutput2DelayOffEdit() 
{
	UpdateData();
	unsigned short m_get_to_write;
	if(m_output2_delay_off.IsEmpty())
		return;
	m_get_to_write=_wtoi(m_output2_delay_off);
	write_one(g_tstat_id,192,m_get_to_write);
	
}

void TDelayForm::OnKillfocusOutput3DelayOffEdit() 
{
	UpdateData();
	unsigned short m_get_to_write;
	if(m_output3_delay_off.IsEmpty())
		return;
	m_get_to_write=_wtoi(m_output3_delay_off);
	write_one(g_tstat_id,193,m_get_to_write);
}

void TDelayForm::OnKillfocusOutput4DelayOffEdit() 
{
	UpdateData();
	unsigned short m_get_to_write;
	
	if(m_output4_delay_off.IsEmpty())
		return;
	m_get_to_write=_wtoi(m_output4_delay_off);
	write_one(g_tstat_id,194,m_get_to_write);
	
}

void TDelayForm::OnKillfocusOutput5DelayOffEdit() 
{
	UpdateData();
	unsigned short m_get_to_write;		
	if(m_output5_delay_off.IsEmpty())
		return;
	m_get_to_write=_wtoi(m_output5_delay_off);
	write_one(g_tstat_id,195,m_get_to_write);
}

void TDelayForm::OnKillfocusOutput1DelayOnEdit() 
{
	UpdateData();
	unsigned short m_get_to_write;
	if(m_output1_delay_on.IsEmpty())
		return;
	m_get_to_write=_wtoi(m_output1_delay_on);
	int n=write_one(g_tstat_id,196,m_get_to_write);
}

void TDelayForm::OnKillfocusOutput2DelayOnEdit() 
{
	UpdateData();
	unsigned short m_get_to_write;
	if(m_output2_delay_on.IsEmpty())
		return;
	m_get_to_write=_wtoi(m_output2_delay_on);
	int n=write_one(g_tstat_id,197,m_get_to_write);
}

void TDelayForm::OnKillfocusOutput3DelayOnEdit() 
{
	UpdateData();
	unsigned short m_get_to_write;
	if(m_output3_delay_on.IsEmpty())
		return;
	m_get_to_write=_wtoi(m_output3_delay_on);
	int n=write_one(g_tstat_id,198,m_get_to_write);
}

void TDelayForm::OnKillfocusOutput4DelayOnEdit() 
{
	UpdateData();
	unsigned short m_get_to_write;		
	if(m_output4_delay_on.IsEmpty())
		return;
	m_get_to_write=_wtoi(m_output4_delay_on);

	int n=write_one(g_tstat_id,199,m_get_to_write);
}

void TDelayForm::OnKillfocusOutput5DelayOnEdit() 
{
	UpdateData();
	unsigned short m_get_to_write;		
	if(m_output5_delay_on.IsEmpty())
		return;
	m_get_to_write=_wtoi(m_output5_delay_on);
	int n=write_one(g_tstat_id,200,m_get_to_write);
}

void TDelayForm::OnKillfocusCyclingEdit() 
{
	UpdateData();
	unsigned short m_get_to_write;	
	if(m_cycling.IsEmpty())
		return;
	m_get_to_write=_wtoi(m_cycling);
	int n=write_one(g_tstat_id,201,m_get_to_write);
}

void TDelayForm::OnKillfocusChangeOverEdit() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	unsigned short m_get_to_write;	
	if(m_change_over.IsEmpty())
		return;
	m_get_to_write=_wtoi(m_change_over);
	int n=write_one(g_tstat_id,202,m_get_to_write);
}

void TDelayForm::OnTimer(UINT nIDEvent) 
{
//	OnFresh();
	CDialog::OnTimer(nIDEvent);
}

void TDelayForm::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(1);
	//close_com();
}

void TDelayForm::OnSetfocusOutput1DelayOffEdit() 
{
}

void TDelayForm::OnSetfocusOutput2DelayOffEdit() 
{
}

void TDelayForm::OnSetfocusOutput3DelayOffEdit() 
{

}

void TDelayForm::OnSetfocusOutput4DelayOffEdit() 
{
}

void TDelayForm::OnSetfocusOutput5DelayOffEdit() 
{

}

void TDelayForm::OnSetfocusOutput1DelayOnEdit() 
{

}

void TDelayForm::OnSetfocusOutput2DelayOnEdit() 
{
}

void TDelayForm::OnSetfocusOutput3DelayOnEdit() 
{
}

void TDelayForm::OnSetfocusOutput4DelayOnEdit() 
{
}

void TDelayForm::OnSetfocusOutput5DelayOnEdit() 
{
}

void TDelayForm::OnSetfocusCyclingEdit() 
{
}

void TDelayForm::OnSetfocusChangeOverEdit() 
{
}

void TDelayForm::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	OnFresh();

}
BOOL TDelayForm::OnInitDialog()
{
	CDialog::OnInitDialog();
		
	OnFresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
