// CTransducer.cpp: 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "CTransducer.h"
#include "global_function.h"
#include "MainFrm.h"
// CTransducer
#define WM_TRANDUCER_UPDATEUI  WM_USER + 509
IMPLEMENT_DYNCREATE(CTransducer, CFormView)

CTransducer::CTransducer()
	: CFormView(IDD_DIALOG_TRANSDUCER)
{

}

CTransducer::~CTransducer()
{
}

void CTransducer::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTransducer, CFormView)
	ON_BN_CLICKED(IDC_RADIO_TRANSDUCER_DEG_C, &CTransducer::OnBnClickedRadioTransducerDegC)
	ON_BN_CLICKED(IDC_RADIO_TRANSDUCER_DEG_F, &CTransducer::OnBnClickedRadioTransducerDegF)
	ON_EN_KILLFOCUS(IDC_EDIT_TEMP_OFFSET, &CTransducer::OnEnKillfocusEditTempOffset)
	ON_EN_KILLFOCUS(IDC_EDIT_HUM_OFFSET, &CTransducer::OnEnKillfocusEditHumOffset)
	ON_BN_CLICKED(IDC_RADIO_TRANSDUCER_4_20_MA, &CTransducer::OnBnClickedRadioTransducer420Ma)
	ON_BN_CLICKED(IDC_RADIO_TRANSDUCER_0_10V, &CTransducer::OnBnClickedRadioTransducer010v)
	ON_BN_CLICKED(IDC_RADIO_DEW, &CTransducer::OnBnClickedRadioDew)
	ON_BN_CLICKED(IDC_RADIO_ENTHALPY, &CTransducer::OnBnClickedRadioEnthalpy)
	ON_BN_CLICKED(IDC_RADIO_ABS, &CTransducer::OnBnClickedRadioAbs)
	ON_BN_CLICKED(IDC_RADIO_REAL, &CTransducer::OnBnClickedRadioReal)
	ON_MESSAGE(WM_TRANDUCER_UPDATEUI, UpdateUI)
END_MESSAGE_MAP()


// CTransducer 诊断

#ifdef _DEBUG
void CTransducer::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTransducer::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTransducer 消息处理程序


void CTransducer::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
}

void CTransducer::Fresh()
{
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	pFrame->SetWindowTextW(cs_special_name + CurrentT3000Version);

	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TRANSDUCER_TEMPERATURE); // 得到 Picture Control 句柄 ;
	CString icon_temperature;
	CString ApplicationFolder;
	CString AQ_image_fordor;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	AQ_image_fordor = ApplicationFolder + _T("\\ResourceFile");
	icon_temperature = AQ_image_fordor + _T("\\temperature.bmp");
	HBITMAP bitmap;
	bitmap = (HBITMAP)LoadImage(AfxGetInstanceHandle(), icon_temperature, IMAGE_BITMAP, 40, 120, LR_LOADFROMFILE);
	CStatic* p = (CStatic*)GetDlgItem(IDC_STATIC_TRANSDUCER_TEMPERATURE);
	//设置静态控件窗口风格为位图居中显示  
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	//将图片设置到Picture控件上  
	p->SetBitmap(bitmap);

	((CComboBox*)GetDlgItem(IDC_COMBO_TEMP_RANGE))->ResetContent();
	for (int x = 0; x < (sizeof(TransducerTempRange) / sizeof(TransducerTempRange[0])); x++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_TEMP_RANGE))->AddString(TransducerTempRange[x]);
	}

	UpdateUI(NULL,NULL);
	return;
}

LRESULT CTransducer::UpdateUI(WPARAM wParam, LPARAM lParam)
{

	Read_Multi(g_tstat_id, product_register_value, 0, 100, 2);

	if (product_register_value[MODBUS_SWITCH_TEMP_RANGE] < (sizeof(TransducerTempRange) / sizeof(TransducerTempRange[0])))
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_TEMP_RANGE))->SetCurSel(product_register_value[MODBUS_SWITCH_TEMP_RANGE]);
	}
	if (product_register_value[MODBUS_TEMPERATURE_TYPE] == 0)
	{
		((CButton*)GetDlgItem(IDC_RADIO_TRANSDUCER_DEG_C))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_TRANSDUCER_DEG_F))->SetCheck(0);
		GetDlgItem(IDC_STATIC_TRANSDUCER_TEMP_UNITS)->SetWindowText(_T("Deg.C"));
		GetDlgItem(IDC_STATIC_TRANSDUCER_TEMP_UNITS2)->SetWindowText(_T("Deg.C"));
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_TRANSDUCER_DEG_C))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_TRANSDUCER_DEG_F))->SetCheck(1);
		GetDlgItem(IDC_STATIC_TRANSDUCER_TEMP_UNITS)->SetWindowText(_T("Deg.F"));
		GetDlgItem(IDC_STATIC_TRANSDUCER_TEMP_UNITS2)->SetWindowText(_T("Deg.F"));
	}

	if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 0)
	{
		((CButton*)GetDlgItem(IDC_RADIO_TRANSDUCER_4_20_MA))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_TRANSDUCER_0_10V))->SetCheck(0);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_TRANSDUCER_4_20_MA))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_TRANSDUCER_0_10V))->SetCheck(1);
	}

	CString hum_value;
	if (product_register_value[MODBUS_SWITCH_HUMI_RANGE] == HUM_DEWPOINT)
	{
		hum_value.Format(_T("%.1f"), product_register_value[MODBUS_DEWPOINT] / 10.0);
		((CButton*)GetDlgItem(IDC_RADIO_DEW))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_ENTHALPY))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_ABS))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_REAL))->SetCheck(0);
	}
	else if (product_register_value[MODBUS_SWITCH_HUMI_RANGE] == HUM_ENTHALPY)
	{
		hum_value.Format(_T("%.1f"), product_register_value[MODBUS_ENTHALPY] / 10.0);
		((CButton*)GetDlgItem(IDC_RADIO_DEW))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_ENTHALPY))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_ABS))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_REAL))->SetCheck(0);
	}
	else if (product_register_value[MODBUS_SWITCH_HUMI_RANGE] == HUM_ABSOLUTE)
	{
		hum_value.Format(_T("%.1f"), product_register_value[MODBUS_ABSOLUTE_HUMI] / 10.0);
		((CButton*)GetDlgItem(IDC_RADIO_DEW))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_ENTHALPY))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_ABS))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_REAL))->SetCheck(0);
	}
	else if (product_register_value[MODBUS_SWITCH_HUMI_RANGE] == HUM_REAL)
	{
		hum_value.Format(_T("%.1f"), product_register_value[MODBUS_HUMIDITY] / 10.0);
		((CButton*)GetDlgItem(IDC_RADIO_DEW))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_ENTHALPY))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_ABS))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_REAL))->SetCheck(1);
	}

	GetDlgItem(IDC_STATIC_TRANSDUCER_HUM_VALUE)->SetWindowText(hum_value);
	CString temp_value;
	temp_value.Format(_T("%.1f"), product_register_value[MODBUS_TEMPERATURE] / 10.0);
	GetDlgItem(IDC_STATIC_TEMPERATURE)->SetWindowText(temp_value);
	
	short temp_offset;
	short hum_offset;
	temp_offset = product_register_value[MODBUS_TEMPERATURE_OFFSET];
	hum_offset = product_register_value[MODBUS_HUMIDITY_OFFSET];
	CString cs_temp_offset;
	CString cs_hum_offset;
	cs_temp_offset.Format(_T("%.1f"), temp_offset/10.0);
	cs_hum_offset.Format(_T("%.1f"), hum_offset/10.0);
	GetDlgItem(IDC_EDIT_TEMP_OFFSET)->SetWindowText(cs_temp_offset);
	GetDlgItem(IDC_EDIT_HUM_OFFSET)->SetWindowText(cs_hum_offset);
	GetDlgItem(IDC_BUTTON_DONE)->SetFocus();
	return 0;
}

void CTransducer::OnBnClickedRadioTransducerDegC()
{
	// TODO: 在此添加控件通知处理程序代码
	if (product_register_value[MODBUS_TEMPERATURE_TYPE] == 0)
		return;
	product_register_value[MODBUS_TEMPERATURE_TYPE] = 0;
	unsigned int temp_value = 0;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_TEMPERATURE_TYPE, temp_value,
		product_register_value[MODBUS_TEMPERATURE_TYPE], this->m_hWnd, IDC_RADIO_TRANSDUCER_DEG_C, _T(" Temperature type  "));
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnBnClickedRadioTransducerDegF()
{
	// TODO: 在此添加控件通知处理程序代码
	if (product_register_value[MODBUS_TEMPERATURE_TYPE] == 1)
		return;
	product_register_value[MODBUS_TEMPERATURE_TYPE] = 1;
	unsigned int temp_value = 1;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_TEMPERATURE_TYPE, temp_value,
		product_register_value[MODBUS_TEMPERATURE_TYPE], this->m_hWnd, IDC_RADIO_TRANSDUCER_DEG_C, _T(" Temperature type  "));
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}



void CTransducer::OnEnKillfocusEditTempOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strText;
	GetDlgItem(IDC_EDIT_TEMP_OFFSET)->GetWindowText(strText);
	unsigned short nValue = (unsigned short)(_wtof(strText)*10);

	if (product_register_value[MODBUS_TEMPERATURE_OFFSET] == nValue)	//Add this to judge weather this value need to change.
		return;


	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_TEMPERATURE_OFFSET, nValue,
		product_register_value[MODBUS_TEMPERATURE_OFFSET], this->m_hWnd, IDC_EDIT_DTERM, _T("Temperature offset "));
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnEnKillfocusEditHumOffset()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strText;
	GetDlgItem(IDC_EDIT_HUM_OFFSET)->GetWindowText(strText);
	unsigned short nValue = (unsigned short)(_wtof(strText) * 10);

	if (product_register_value[MODBUS_HUMIDITY_OFFSET] == nValue)	//Add this to judge weather this value need to change.
		return;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_HUMIDITY_OFFSET, nValue,
		product_register_value[MODBUS_HUMIDITY_OFFSET], this->m_hWnd, IDC_EDIT_DTERM, _T("Humidity offset "));
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnBnClickedRadioTransducer420Ma()
{
	// TODO: 在此添加控件通知处理程序代码
	if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 0)
		return;
	product_register_value[MODBUS_TEMPERATURE_TYPE] = 0;
	unsigned int temp_value = 0;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_SWITCH_OUTPUT_MODE, temp_value,
		product_register_value[MODBUS_SWITCH_OUTPUT_MODE], this->m_hWnd, IDC_RADIO_TRANSDUCER_4_20_MA, _T(" Output Mode  "));
}


void CTransducer::OnBnClickedRadioTransducer010v()
{
	// TODO: 在此添加控件通知处理程序代码
	if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 1)
		return;
	product_register_value[MODBUS_TEMPERATURE_TYPE] = 1;
	unsigned int temp_value = 1;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_SWITCH_OUTPUT_MODE, temp_value,
		product_register_value[MODBUS_SWITCH_OUTPUT_MODE], this->m_hWnd, IDC_RADIO_TRANSDUCER_4_20_MA, _T(" Output Mode  "));
}


void CTransducer::OnBnClickedRadioDew()
{
	// TODO: 在此添加控件通知处理程序代码
	if (product_register_value[MODBUS_SWITCH_HUMI_RANGE] == HUM_DEWPOINT)
		return;
	product_register_value[MODBUS_SWITCH_HUMI_RANGE] = HUM_DEWPOINT;
	unsigned int temp_value = HUM_DEWPOINT;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_SWITCH_HUMI_RANGE, temp_value,
		product_register_value[MODBUS_SWITCH_HUMI_RANGE], this->m_hWnd, IDC_RADIO_DEW, _T(" Humidity range  "));
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnBnClickedRadioEnthalpy()
{
	// TODO: 在此添加控件通知处理程序代码
	if (product_register_value[MODBUS_SWITCH_HUMI_RANGE] == HUM_ENTHALPY)
		return;
	product_register_value[MODBUS_SWITCH_HUMI_RANGE] = HUM_ENTHALPY;
	unsigned int temp_value = HUM_ENTHALPY;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_SWITCH_HUMI_RANGE, temp_value,
		product_register_value[MODBUS_SWITCH_HUMI_RANGE], this->m_hWnd, IDC_RADIO_ENTHALPY, _T(" Humidity range  "));
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnBnClickedRadioAbs()
{
	// TODO: 在此添加控件通知处理程序代码
	if (product_register_value[MODBUS_SWITCH_HUMI_RANGE] == HUM_ABSOLUTE)
		return;
	product_register_value[MODBUS_SWITCH_HUMI_RANGE] = HUM_ABSOLUTE;
	unsigned int temp_value = HUM_ABSOLUTE;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_SWITCH_HUMI_RANGE, temp_value,
		product_register_value[MODBUS_SWITCH_HUMI_RANGE], this->m_hWnd, IDC_RADIO_ABS, _T(" Humidity range  "));
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnBnClickedRadioReal()
{
	// TODO: 在此添加控件通知处理程序代码
	if (product_register_value[MODBUS_SWITCH_HUMI_RANGE] == HUM_REAL)
		return;
	product_register_value[MODBUS_SWITCH_HUMI_RANGE] = HUM_REAL;
	unsigned int temp_value = HUM_REAL;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_SWITCH_HUMI_RANGE, temp_value,
		product_register_value[MODBUS_SWITCH_HUMI_RANGE], this->m_hWnd, IDC_RADIO_REAL, _T(" Humidity range  "));
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}
