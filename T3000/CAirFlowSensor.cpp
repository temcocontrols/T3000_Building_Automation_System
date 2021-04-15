// CAirFlowSensor.cpp: 实现文件
//
#include "stdafx.h"
#include "T3000.h"
#include "CAirFlowSensor.h"
#include "global_function.h"
#include "MainFrm.h"
HANDLE h_airflow_thread = NULL;
CString Resource_folder;
CString png_airflow_shape_cylinder;
CString png_airflow_shape_rectangular;
// CAirFlowSensor
#define     WM_TSTAT_AIRFLOW_THREAD_READ                WM_USER + 503
IMPLEMENT_DYNCREATE(CAirFlowSensor, CFormView)

CAirFlowSensor::CAirFlowSensor()
	: CFormView(IDD_DIALOG_AIRFLOW)
{

}

CAirFlowSensor::~CAirFlowSensor()
{
}

void CAirFlowSensor::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAirFlowSensor, CFormView)
	ON_MESSAGE(WM_TSTAT_AIRFLOW_THREAD_READ, UpdateUI)
	ON_CBN_SELCHANGE(IDC_COMBO_AIRFLOW_MODE, &CAirFlowSensor::OnCbnSelchangeComboAirflowMode)
	ON_CBN_SELCHANGE(IDC_COMBO_AIRFLOW_RANGE, &CAirFlowSensor::OnCbnSelchangeComboAirflowRange)
	ON_EN_KILLFOCUS(IDC_EDIT_AIRFLOW_DIF_LENGTH, &CAirFlowSensor::OnEnKillfocusEditAirflowDifLength)
	ON_EN_KILLFOCUS(IDC_EDIT_AIRFLOW_DIF_WIDTH, &CAirFlowSensor::OnEnKillfocusEditAirflowDifWidth)
	ON_CBN_SELCHANGE(IDC_COMBO_AIRFLOW_UNIT, &CAirFlowSensor::OnCbnSelchangeComboAirflowUnit)
	ON_BN_CLICKED(IDC_BUTTON_AIRFLOW_DONE, &CAirFlowSensor::OnBnClickedButtonAirflowDone)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_CIRCULAR, &CAirFlowSensor::OnBnClickedRadioCircular)
	ON_BN_CLICKED(IDC_RADIO_RECTANGULAR, &CAirFlowSensor::OnBnClickedRadioRectangular)
	ON_EN_KILLFOCUS(IDC_EDIT_AIRFLOW_RADIUS, &CAirFlowSensor::OnEnKillfocusEditAirflowRadius)
	ON_BN_CLICKED(IDC_RADIO_UNIT_M, &CAirFlowSensor::OnBnClickedRadioUnitM)
	ON_BN_CLICKED(IDC_RADIO_UNIT_IN, &CAirFlowSensor::OnBnClickedRadioUnitIn)
END_MESSAGE_MAP()


// CAirFlowSensor 诊断

#ifdef _DEBUG
void CAirFlowSensor::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAirFlowSensor::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAirFlowSensor 消息处理程序


void CAirFlowSensor::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
}

void CAirFlowSensor::InitialUI()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_MODE))->ResetContent();
    ((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_MODE))->AddString(AirFlowMode[0]);
	((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_MODE))->AddString(AirFlowMode[1]);

	((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_RANGE))->ResetContent();
	for (int z = 0; z < sizeof(AirFlowRange) / sizeof(AirFlowRange[0]); z++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_RANGE))->AddString(AirFlowRange[z]);
	}
	
	((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_UNIT))->ResetContent();
	for (int z = 0; z < sizeof(AirFlowUnit) / sizeof(AirFlowUnit[0]); z++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_UNIT))->AddString(AirFlowUnit[z]);
	}
	

}


LRESULT CAirFlowSensor::UpdateUI(WPARAM wParam, LPARAM lParam)
{
	UpdateUserInterface();
	return 0;
}

void CAirFlowSensor::UpdateUserInterface()
{
	if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 0)
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_MODE))->SetWindowText(AirFlowMode[0]);
	else if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 1)
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_MODE))->SetWindowText(AirFlowMode[1]);
	else
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_MODE))->SetWindowText(_T(""));

	if (product_register_value[MODBUS_SWITCH_DP_RANGE] < 4)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_RANGE))->SetWindowText(AirFlowRange[product_register_value[MODBUS_SWITCH_DP_RANGE]]);
	}
	else
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_RANGE))->SetWindowText(_T(""));
	
	if (product_register_value[MODBUS_FLOW_UNIT] < 3)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_UNIT))->SetWindowText(AirFlowUnit[product_register_value[MODBUS_FLOW_UNIT]]);
	}
	else
		((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_UNIT))->SetWindowText(_T(""));


	CString cs_pressure_value;
	CString cs_length;
	CString cs_width;
	CString cs_speed;
	CString cs_flow_value;
	cs_pressure_value.Format(_T("%d"), (short)product_register_value[MODBUS_DIFF_PRESSURE_VALUE]);
	cs_length.Format(_T("%u"), product_register_value[MODBUS_FLOW_LENGTH]);
	cs_width.Format(_T("%u"), product_register_value[MODBUS_FLOW_WIDTH]);
	cs_speed.Format(_T("%u"), product_register_value[MODBUS_FLOW_SPEED]);

	unsigned int temp_flow_value;
	temp_flow_value = product_register_value[MODBUS_FLOW_HI] * 65536 + product_register_value[MODBUS_FLOW_LO];
	cs_flow_value.Format(_T("%u"), temp_flow_value);

	GetDlgItem(IDC_EDIT_AIRFLOW_DIF_PRESSURE_VALUE)->SetWindowText(cs_pressure_value);
	GetDlgItem(IDC_EDIT_AIRFLOW_DIF_LENGTH)->SetWindowText(cs_length);
	GetDlgItem(IDC_EDIT_AIRFLOW_DIF_WIDTH)->SetWindowText(cs_width);
	GetDlgItem(IDC_EDIT_AIRFLOW_SPEED)->SetWindowText(cs_speed);
	GetDlgItem(IDC_EDIT_AIRFLOW_VALUE)->SetWindowText(cs_flow_value);
	
	CString temp_radius;
	temp_radius.Format(_T("%u"), product_register_value[MODBUS_FLOW_RADIUS]);
	GetDlgItem(IDC_EDIT_AIRFLOW_RADIUS)->SetWindowText(temp_radius);

	if (product_register_value[MODBUS_FLOW_SHAPE] == 1) //The shape of the channel, 0: square; 1: round
	{
		((CButton*)GetDlgItem(IDC_RADIO_CIRCULAR))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_RECTANGULAR))->SetCheck(0);
		m_shape_type = 1;
		show_duct_picture = png_airflow_shape_cylinder;
		GetDlgItem(IDC_EDIT_AIRFLOW_RADIUS)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_AIRFLOW_DIF_LENGTH)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_AIRFLOW_DIF_WIDTH)->EnableWindow(false);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_CIRCULAR))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_RECTANGULAR))->SetCheck(1);
		m_shape_type = 0;
		show_duct_picture = png_airflow_shape_rectangular;

		GetDlgItem(IDC_EDIT_AIRFLOW_RADIUS)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_AIRFLOW_DIF_LENGTH)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_AIRFLOW_DIF_WIDTH)->EnableWindow(true);
	}

	if (product_register_value[MODBUS_FLOW_SHAPE_UNIT] == 1)
	{
		((CButton*)GetDlgItem(IDC_RADIO_UNIT_M))->SetCheck(0);
		((CButton*)GetDlgItem(IDC_RADIO_UNIT_IN))->SetCheck(1);
		GetDlgItem(IDC_STATIC_CIRCULAR_UNIT)->SetWindowText(_T("inch"));
		GetDlgItem(IDC_STATIC_RECTANGULAR_UNIT_LENGTH)->SetWindowText(_T("inch"));
		GetDlgItem(IDC_STATIC_RECTANGULAR_UNIT_WIDTH)->SetWindowText(_T("inch"));
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_UNIT_M))->SetCheck(1);
		((CButton*)GetDlgItem(IDC_RADIO_UNIT_IN))->SetCheck(0);
		GetDlgItem(IDC_STATIC_CIRCULAR_UNIT)->SetWindowText(_T("cm"));
		GetDlgItem(IDC_STATIC_RECTANGULAR_UNIT_LENGTH)->SetWindowText(_T("cm"));
		GetDlgItem(IDC_STATIC_RECTANGULAR_UNIT_WIDTH)->SetWindowText(_T("cm"));
	}


}

void CAirFlowSensor::Fresh()
{
	// TODO: 在此处添加实现代码.
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	pFrame->SetWindowTextW(cs_special_name + CurrentT3000Version);
	InitialUI();


	CString ApplicationFolder;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	Resource_folder = ApplicationFolder + _T("\\ResourceFile");
	png_airflow_shape_cylinder = Resource_folder + _T("\\cylinder.png");
	png_airflow_shape_rectangular = Resource_folder + _T("\\rectangular_tube.png");

	if (h_airflow_thread == NULL)
	{
		h_airflow_thread = CreateThread(NULL, NULL, Update_Airflow_Thread, this, NULL, NULL);
		CloseHandle(h_airflow_thread);
	}

	UpdateUserInterface();
	return ;
}



DWORD WINAPI Update_Airflow_Thread(LPVOID lPvoid)
{
	CAirFlowSensor* mparent = (CAirFlowSensor*)lPvoid;
	while (mparent->IsWindowVisible())
	{
		Read_Multi(g_tstat_id, &product_register_value[0], 0, 100, 5);
		PostMessage(mparent->m_hWnd, WM_TSTAT_AIRFLOW_THREAD_READ, NULL, NULL);
		Sleep(6000);
	}
	h_airflow_thread = NULL;
	return 1;
}


void CAirFlowSensor::OnCbnSelchangeComboAirflowMode()
{
	// TODO: 在此添加控件通知处理程序代码
	int n_value = 0;
	CString temp_string;
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_MODE))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_MODE))->GetLBText(nSel, temp_string);
	if (temp_string.CompareNoCase(AirFlowMode[0]) == 0)
	{
		n_value = 0;
	}
	else if (temp_string.CompareNoCase(AirFlowMode[1]) == 0)
	{
		n_value = 1;
	}
	else if (temp_string.CompareNoCase(AirFlowMode[2]) == 0)
	{
		n_value = 2;
	}

	if (write_one(g_tstat_id, MODBUS_SWITCH_OUTPUT_MODE, n_value) > 0)
	{
		product_register_value[MODBUS_SWITCH_OUTPUT_MODE] = n_value;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Airflow Output Mode Success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Airflow Output Mode Timeout"));
	}
}


void CAirFlowSensor::OnCbnSelchangeComboAirflowRange()
{
	// TODO: 在此添加控件通知处理程序代码
	int n_value = 0;
	CString temp_string;
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_RANGE))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_RANGE))->GetLBText(nSel, temp_string);
	if (temp_string.CompareNoCase(AirFlowRange[0]) == 0)
	{
		n_value = 0;
	}
	else if (temp_string.CompareNoCase(AirFlowRange[1]) == 0)
	{
		n_value = 1;
	}
	else if (temp_string.CompareNoCase(AirFlowRange[2]) == 0)
	{
		n_value = 2;
	}
	else if (temp_string.CompareNoCase(AirFlowRange[3]) == 0)
	{
		n_value = 3;
	}

	if (write_one(g_tstat_id, MODBUS_SWITCH_DP_RANGE, n_value) > 0)
	{
		product_register_value[MODBUS_SWITCH_DP_RANGE] = n_value;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Airflow Range Success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Airflow Range Timeout"));
	}


}

void CAirFlowSensor::OnCbnSelchangeComboAirflowUnit()
{
	// TODO: 在此添加控件通知处理程序代码
	int n_value = 0;
	CString temp_string;
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_UNIT))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_AIRFLOW_UNIT))->GetLBText(nSel, temp_string);
	if (temp_string.CompareNoCase(AirFlowUnit[0]) == 0)
	{
		n_value = 0;
	}
	else if (temp_string.CompareNoCase(AirFlowUnit[1]) == 0)
	{
		n_value = 1;
	}
	else if (temp_string.CompareNoCase(AirFlowUnit[2]) == 0)
	{
		n_value = 2;
	}

	if (write_one(g_tstat_id, MODBUS_FLOW_UNIT, n_value) > 0)
	{
		product_register_value[MODBUS_FLOW_UNIT] = n_value;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Airflow Unit Success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Airflow Unit Timeout"));
	}

}

void CAirFlowSensor::OnEnKillfocusEditAirflowDifLength()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_AIRFLOW_DIF_LENGTH, temp_cstring);
	unsigned int temp_value = unsigned int(_wtoi(temp_cstring));

	if (write_one(g_tstat_id, MODBUS_FLOW_LENGTH, temp_value) > 0)
	{
		product_register_value[MODBUS_FLOW_LENGTH] = temp_value;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Length Success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Length Timeout"));
	}

}


void CAirFlowSensor::OnEnKillfocusEditAirflowDifWidth()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_AIRFLOW_DIF_WIDTH, temp_cstring);
	unsigned int temp_value = unsigned int(_wtoi(temp_cstring));
	if (write_one(g_tstat_id, MODBUS_FLOW_WIDTH, temp_value) > 0)
	{
		product_register_value[MODBUS_FLOW_WIDTH] = temp_value;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Width Success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set Width Timeout"));
	}

}





void CAirFlowSensor::OnBnClickedButtonAirflowDone()
{
	// TODO: 在此添加控件通知处理程序代码
	
}


void CAirFlowSensor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CFormView::OnPaint()
	//CString png_airflow_shape_cylinder;
	//CString png_airflow_shape_rectangular;
	int cx = 0;
	int cy = 0;
	CImage image;
	CRect rect;
	image.Load(show_duct_picture);
	cx = image.GetWidth();
	cy = image.GetHeight();

	//获取Picture Control控件的大小  
	GetDlgItem(IDC_STATIC_SHAPE)->GetWindowRect(&rect);
	//将客户区选中到控件表示的矩形区域内  
	ScreenToClient(&rect);
	//窗口移动到控件表示的区域  
	GetDlgItem(IDC_STATIC_SHAPE)->MoveWindow(rect.left, rect.top, cx, cy, TRUE);
	CWnd* pWnd = NULL;
	pWnd = GetDlgItem(IDC_STATIC_SHAPE);
	pWnd->GetClientRect(&rect);
	CDC* pDc = NULL;
	pDc = pWnd->GetDC();
	image.Draw(pDc->m_hDC, rect);
	ReleaseDC(pDc);
}


void CAirFlowSensor::OnBnClickedRadioCircular()
{
	// TODO: 在此添加控件通知处理程序代码
	if (write_one(g_tstat_id, MODBUS_FLOW_SHAPE, 1) > 0)
	{
		product_register_value[MODBUS_FLOW_SHAPE] = 1;
	}
	else
	{
		MessageBox(_T("Write data timeout!"));
	}
	PostMessage( WM_TSTAT_AIRFLOW_THREAD_READ, NULL, NULL);
	Invalidate();
}


void CAirFlowSensor::OnBnClickedRadioRectangular()
{
	// TODO: 在此添加控件通知处理程序代码
	if (write_one(g_tstat_id, MODBUS_FLOW_SHAPE, 0) > 0)
	{
		product_register_value[MODBUS_FLOW_SHAPE] = 0;
	}
	else
	{
		MessageBox(_T("Write data timeout!"));
	}
	PostMessage(WM_TSTAT_AIRFLOW_THREAD_READ, NULL, NULL);
	Invalidate();
}


void CAirFlowSensor::OnEnKillfocusEditAirflowRadius()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_AIRFLOW_RADIUS, temp_cstring);
	unsigned int temp_value = unsigned int(_wtoi(temp_cstring));

	if (write_one(g_tstat_id, MODBUS_FLOW_RADIUS, temp_value) > 0)
	{
		product_register_value[MODBUS_FLOW_RADIUS] = temp_value;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set radius Success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set radius Timeout"));
	}
}


void CAirFlowSensor::OnBnClickedRadioUnitM()
{
	// TODO: 在此添加控件通知处理程序代码
	if (write_one(g_tstat_id, MODBUS_FLOW_SHAPE_UNIT, 0) > 0)
	{
		product_register_value[MODBUS_FLOW_SHAPE_UNIT] = 0;
	}
	else
	{
		MessageBox(_T("Write data timeout!"));
	}
	PostMessage(WM_TSTAT_AIRFLOW_THREAD_READ, NULL, NULL);
}


void CAirFlowSensor::OnBnClickedRadioUnitIn()
{
	// TODO: 在此添加控件通知处理程序代码
	if (write_one(g_tstat_id, MODBUS_FLOW_SHAPE_UNIT, 1) > 0)
	{
		product_register_value[MODBUS_FLOW_SHAPE_UNIT] = 0;
	}
	else
	{
		MessageBox(_T("Write data timeout!"));
	}
	PostMessage(WM_TSTAT_AIRFLOW_THREAD_READ, NULL, NULL);
}
