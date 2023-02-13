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
	ON_BN_CLICKED(IDC_RADIO_DEFAULT, &CAirFlowSensor::OnBnClickedRadioDefault)
	ON_BN_CLICKED(IDC_RADIO_USER_DEFINED, &CAirFlowSensor::OnBnClickedRadioUserDefined)
	ON_EN_KILLFOCUS(IDC_EDIT_VOLATGE_MIN, &CAirFlowSensor::OnEnKillfocusEditVolatgeMin)
	ON_EN_KILLFOCUS(IDC_EDIT_VOLATGE_MAX, &CAirFlowSensor::OnEnKillfocusEditVolatgeMax)
	ON_EN_KILLFOCUS(IDC_EDIT_CURRENT_MIN, &CAirFlowSensor::OnEnKillfocusEditCurrentMin)
	ON_EN_KILLFOCUS(IDC_EDIT_CURRENT_MAX, &CAirFlowSensor::OnEnKillfocusEditCurrentMax)
	ON_EN_KILLFOCUS(IDC_EDIT_PASCAL_MIN, &CAirFlowSensor::OnEnKillfocusEditPascalMin)
	ON_EN_KILLFOCUS(IDC_EDIT_PASCAL_MAX, &CAirFlowSensor::OnEnKillfocusEditPascalMax)
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
	Invalidate(1);
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


		if (product_register_value[MODBUS_FIRMWARE_VERSION] < 25)
		{
			GetDlgItem(IDC_EDIT_VOLATGE_MIN)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_VOLATGE_MAX)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_CURRENT_MIN)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_CURRENT_MAX)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_PASCAL_MIN)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_PASCAL_MAX)->EnableWindow(false);
			GetDlgItem(IDC_RADIO_DEFAULT)->EnableWindow(false);
			GetDlgItem(IDC_RADIO_USER_DEFINED)->EnableWindow(false);
			((CButton*)GetDlgItem(IDC_RADIO_DEFAULT))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_USER_DEFINED))->SetCheck(0);
		}
		else
		{
			if (product_register_value[MODBUS_DEF_CUSTOMER] == 0)
			{
				GetDlgItem(IDC_EDIT_VOLATGE_MIN)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_VOLATGE_MAX)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_CURRENT_MIN)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_CURRENT_MAX)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_PASCAL_MIN)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_PASCAL_MAX)->EnableWindow(false);
				((CButton*)GetDlgItem(IDC_RADIO_DEFAULT))->SetCheck(1);
				((CButton*)GetDlgItem(IDC_RADIO_USER_DEFINED))->SetCheck(0);
			}
			else
			{
				if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 0)
				{
					GetDlgItem(IDC_EDIT_CURRENT_MIN)->EnableWindow(1);
					GetDlgItem(IDC_EDIT_CURRENT_MAX)->EnableWindow(1);
					GetDlgItem(IDC_EDIT_VOLATGE_MIN)->EnableWindow(0);
					GetDlgItem(IDC_EDIT_VOLATGE_MAX)->EnableWindow(0);
				}
				else if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 1)
				{
					GetDlgItem(IDC_EDIT_CURRENT_MIN)->EnableWindow(0);
					GetDlgItem(IDC_EDIT_CURRENT_MAX)->EnableWindow(0);
					GetDlgItem(IDC_EDIT_VOLATGE_MIN)->EnableWindow(1);
					GetDlgItem(IDC_EDIT_VOLATGE_MAX)->EnableWindow(1);
				}

				GetDlgItem(IDC_EDIT_PASCAL_MIN)->EnableWindow(1);
				GetDlgItem(IDC_EDIT_PASCAL_MAX)->EnableWindow(1);
				((CButton*)GetDlgItem(IDC_RADIO_DEFAULT))->SetCheck(0);
				((CButton*)GetDlgItem(IDC_RADIO_USER_DEFINED))->SetCheck(1);

				CString current_min; CString current_max;
				CString voltage_min; CString voltage_max;
				CString pascal_min;  CString pascal_max;
				current_min.Format(_T("%.1f"), product_register_value[MODBUS_CURRENT_MIN] / 10.0);
				current_max.Format(_T("%.1f"), product_register_value[MODBUS_CURRENT_MAX] / 10.0);
				voltage_min.Format(_T("%.1f"), product_register_value[MODBUS_VOLTAGE_MIN] / 10.0);
				voltage_max.Format(_T("%.1f"), product_register_value[MODBUS_VOLTAGE_MAX] / 10.0);
				pascal_min.Format(_T("%u"), product_register_value[MODBUS_PASCAL_MIN]);
				pascal_max.Format(_T("%u"), product_register_value[MODBUS_PASCAL_MAX]);
				GetDlgItem(IDC_EDIT_CURRENT_MIN)->SetWindowText(current_min);
				GetDlgItem(IDC_EDIT_CURRENT_MAX)->SetWindowText(current_max);
				GetDlgItem(IDC_EDIT_VOLATGE_MIN)->SetWindowText(voltage_min);
				GetDlgItem(IDC_EDIT_VOLATGE_MAX)->SetWindowText(voltage_max);
				GetDlgItem(IDC_EDIT_PASCAL_MIN)->SetWindowText(pascal_min);
				GetDlgItem(IDC_EDIT_PASCAL_MAX)->SetWindowText(pascal_max);
			}

			if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 0)
			{
				CString current_output;
				current_output.Format(_T("%.1f"), product_register_value[MODBUS_OUTPUT_CUR] / 10.0);
				((CStatic*)GetDlgItem(IDC_STATIC_OUT_CURRENT))->SetWindowText(current_output);
				((CStatic*)GetDlgItem(IDC_STATIC_OUT_VOLTAGE))->SetWindowText(_T(""));
			}
			else if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 1)
			{
				CString voltage_output;
				voltage_output.Format(_T("%.1f"), product_register_value[MODBUS_OUTPUT_VOL] / 10.0);
				((CStatic*)GetDlgItem(IDC_STATIC_OUT_VOLTAGE))->SetWindowText(voltage_output);
				((CStatic*)GetDlgItem(IDC_STATIC_OUT_CURRENT))->SetWindowText(_T(""));
			}
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
		if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 0)
		{
			GetDlgItem(IDC_EDIT_CURRENT_MIN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_CURRENT_MAX)->EnableWindow(1);
		}
		else if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 1)
		{
			GetDlgItem(IDC_EDIT_VOLATGE_MIN)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_VOLATGE_MAX)->EnableWindow(1);
		}
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



int CANVAS_TOP_X = 550;  //画布
int CANVAS_TOP_Y = 0;
int CANVAS_BOTTOM_X = 1210;
int CANVAS_BOTTOM_Y = 420;

int ORIGIN_X = 600; //原点
int ORIGIN_Y = 400;


int Y_AXIS_TOP_X = ORIGIN_X;  //Y轴最上面点
int Y_AXIS_TOP_Y = CANVAS_TOP_Y + 10;
int Y_AXIS_ARROW_LEFT_X = Y_AXIS_TOP_X - 5;  //Y轴的箭头左边
int Y_AXIS_ARROW_LEFT_Y = Y_AXIS_TOP_Y + 10;
int Y_AXIS_ARROW_RIGHT_X = Y_AXIS_TOP_X + 5; // Y轴的箭头右边
int Y_AXIS_ARROW_RIGHT_Y = Y_AXIS_TOP_Y + 10;

int X_AXIS_RIGHT_X = CANVAS_BOTTOM_X - 10;  //X轴最右边  X坐标
int X_AXIS_RIGHT_Y = ORIGIN_Y;				//X轴最右边  Y坐标

int X_AXIS_MAX_VALUE_X = X_AXIS_RIGHT_X - 100; //X轴最大点  X坐标
int X_AXIS_MAX_VALUE_Y = X_AXIS_RIGHT_Y ; //X轴最大点  Y坐标

int Y_AXIS_MAX_VALUE_X = Y_AXIS_TOP_X;
int Y_AXIS_MAX_VALUE_Y = Y_AXIS_TOP_Y + 50;

int X_AXIS_ARROW_TOP_X = X_AXIS_RIGHT_X - 10; //X轴的箭头上边
int X_AXIS_ARROW_TOP_Y = X_AXIS_RIGHT_Y - 5;
int X_AXIS_ARROW_BOTTOM_X = X_AXIS_RIGHT_X - 10; //X轴的箭头下边
int X_AXIS_ARROW_BOTTOM_Y = X_AXIS_RIGHT_Y + 5;



//600,10,300,400)
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


	if (product_register_value[MODBUS_FIRMWARE_VERSION] >= 25)
	{
		Pen* myRectangle_pen;
		Graphics* mygraphics;
		Pen* myRectangle_pen3;
		Pen* myRectangle_pen4;
		Pen* myRectangle_pen5;
		myRectangle_pen = new Pen(Color(255, 0, 0, 0));
		CRect myrect(CANVAS_TOP_X, CANVAS_TOP_Y, CANVAS_BOTTOM_X, CANVAS_BOTTOM_Y);
		CMemDC memDC(dc, &myrect);
		memDC.GetDC().FillSolidRect(&myrect, RGB(230, 230, 230));

		mygraphics = new Graphics(memDC.GetDC());

		//mygraphics->DrawRectangle(myRectangle_pen, 610, 20, 890, 400);
		mygraphics->DrawLine(myRectangle_pen, ORIGIN_X, ORIGIN_Y, Y_AXIS_TOP_X, Y_AXIS_TOP_Y);
		mygraphics->DrawLine(myRectangle_pen, ORIGIN_X, ORIGIN_Y, X_AXIS_RIGHT_X, X_AXIS_RIGHT_Y);

		mygraphics->DrawLine(myRectangle_pen, Y_AXIS_TOP_X, Y_AXIS_TOP_Y, Y_AXIS_ARROW_LEFT_X, Y_AXIS_ARROW_LEFT_Y); //画 Y轴 箭头
		mygraphics->DrawLine(myRectangle_pen, Y_AXIS_TOP_X, Y_AXIS_TOP_Y, Y_AXIS_ARROW_RIGHT_X, Y_AXIS_ARROW_RIGHT_Y);

		mygraphics->DrawLine(myRectangle_pen, X_AXIS_RIGHT_X, X_AXIS_RIGHT_Y, X_AXIS_ARROW_TOP_X, X_AXIS_ARROW_TOP_Y); //画 X轴 箭头
		mygraphics->DrawLine(myRectangle_pen, X_AXIS_RIGHT_X, X_AXIS_RIGHT_Y, X_AXIS_ARROW_BOTTOM_X, X_AXIS_ARROW_BOTTOM_Y);

		CString cs_show_info;
		CString cs_show_info2;

		FontFamily  UnitfontFamily(_T("Arial"));

		if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 1)
		{
			cs_show_info = _T("0 V");
			cs_show_info2 = _T("10 V");
		}
		else if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 0)
		{
			cs_show_info = _T("4 ma");
			cs_show_info2 = _T("20 ma");
		}

		PointF      pointF(ORIGIN_X - 30, ORIGIN_Y - 15);
		Gdiplus::Font        unitfont(&UnitfontFamily, 12, FontStyleRegular, UnitPixel);
		SolidBrush  txt_color_brush(Color(255, 0, 0, 0));
		mygraphics->DrawString(cs_show_info, -1, &unitfont, pointF, &txt_color_brush);

		pointF.X = ORIGIN_X - 40;
		pointF.Y = CANVAS_TOP_Y + 40;
		mygraphics->DrawString(cs_show_info2, -1, &unitfont, pointF, &txt_color_brush);


		unsigned short default_max_pascal = 0;
		unsigned short default_min_pascal = 0;
		CString cs_show_info3;
		CString cs_show_info4;
		if (product_register_value[MODBUS_DEF_CUSTOMER] == 0)
		{

			cs_show_info3 = _T("0 Pascal");
			pointF.X = ORIGIN_X;
			pointF.Y = ORIGIN_Y + 5;


			if (product_register_value[MODBUS_SWITCH_DP_RANGE] == 0)
			{
				cs_show_info4 = _T("50 Pascal");
				default_max_pascal = 50;
			}
			else if (product_register_value[MODBUS_SWITCH_DP_RANGE] == 1)
			{
				cs_show_info4 = _T("100 Pascal");
				default_max_pascal = 100;
			}
			else if (product_register_value[MODBUS_SWITCH_DP_RANGE] == 2)
			{
				cs_show_info4 = _T("250 Pascal");
				default_max_pascal = 250;
			}
			else if (product_register_value[MODBUS_SWITCH_DP_RANGE] == 3)
			{
				cs_show_info4 = _T("500 Pascal");
				default_max_pascal = 500;
			}
		}
		else
		{
			cs_show_info3.Format(_T("%d"), product_register_value[MODBUS_PASCAL_MIN]);
			pointF.X = ORIGIN_X;
			pointF.Y = ORIGIN_Y + 5;

			CString temp;
			temp.Format(_T("%d"), product_register_value[MODBUS_PASCAL_MAX]);
			cs_show_info4 = temp + _T(" Pascal");
			default_min_pascal = product_register_value[MODBUS_PASCAL_MIN];
			default_max_pascal = product_register_value[MODBUS_PASCAL_MAX];

		}

		mygraphics->DrawString(cs_show_info3, -1, &unitfont, pointF, &txt_color_brush);

		pointF.X = X_AXIS_RIGHT_X - 100;
		pointF.Y = ORIGIN_Y + 5;
		mygraphics->DrawString(cs_show_info4, -1, &unitfont, pointF, &txt_color_brush);


		myRectangle_pen3 = new Pen(Color(255, 0, 0, 0));
		Gdiplus::REAL dashVals[2] = { 3.0f,3.0f };
		myRectangle_pen3->SetDashPattern(dashVals, 2);
		myRectangle_pen3->SetDashCap(DashCap(2));
		mygraphics->DrawLine(myRectangle_pen3, X_AXIS_MAX_VALUE_X, X_AXIS_MAX_VALUE_Y, X_AXIS_MAX_VALUE_X, Y_AXIS_MAX_VALUE_Y);
		mygraphics->DrawLine(myRectangle_pen3, Y_AXIS_MAX_VALUE_X, Y_AXIS_MAX_VALUE_Y, X_AXIS_MAX_VALUE_X, Y_AXIS_MAX_VALUE_Y);


		myRectangle_pen4 = new Pen(Color(255, 50, 50, 50), 2.5);


		if (product_register_value[MODBUS_DEF_CUSTOMER] == 0)
		{
			mygraphics->DrawLine(myRectangle_pen4, ORIGIN_X, ORIGIN_Y, X_AXIS_MAX_VALUE_X, Y_AXIS_MAX_VALUE_Y);
		}
		else
		{
			PointF      cus_point1;
			PointF      cus_point2;
			CString first_point_y_value;
			CString second_point_y_value;

			if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 0) //4-20ma
			{
				cus_point1.X = ORIGIN_X;
				cus_point1.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[MODBUS_CURRENT_MIN] / 200;

				first_point_y_value.Format(_T("%.1f"), product_register_value[MODBUS_CURRENT_MIN] / 10.0);
				second_point_y_value.Format(_T("%.1f"), product_register_value[MODBUS_CURRENT_MAX] / 10.0);
				mygraphics->DrawString(first_point_y_value, -1, &unitfont, cus_point1, &txt_color_brush);

				cus_point2.X = X_AXIS_MAX_VALUE_X;
				cus_point2.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[MODBUS_CURRENT_MAX] / 200;
				mygraphics->DrawString(second_point_y_value, -1, &unitfont, cus_point2, &txt_color_brush);

				mygraphics->DrawLine(myRectangle_pen4, cus_point1.X, cus_point1.Y, cus_point2.X, cus_point2.Y);
			}
			else if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 1) // 0 - 10 V
			{
				cus_point1.X = ORIGIN_X;// ORIGIN_X - (X_AXIS_MAX_VALUE_X - ORIGIN_X) / (product_register_value[MODBUS_PASCAL_MAX] - product_register_value[MODBUS_PASCAL_MIN]) * product_register_value[MODBUS_PASCAL_MIN];
				cus_point1.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[MODBUS_VOLTAGE_MIN] / 100;

				first_point_y_value.Format(_T("%.1f"), product_register_value[MODBUS_VOLTAGE_MIN] / 10.0);
				second_point_y_value.Format(_T("%.1f"), product_register_value[MODBUS_VOLTAGE_MAX] / 10.0);
				mygraphics->DrawString(first_point_y_value, -1, &unitfont, cus_point1, &txt_color_brush);

				cus_point2.X = X_AXIS_MAX_VALUE_X;
				cus_point2.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[MODBUS_VOLTAGE_MAX] / 100;
				mygraphics->DrawString(second_point_y_value, -1, &unitfont, cus_point2, &txt_color_brush);

				mygraphics->DrawLine(myRectangle_pen4, cus_point1.X, cus_point1.Y, cus_point2.X, cus_point2.Y);
			}
		}
		PointF value_position;
		if (product_register_value[MODBUS_DEF_CUSTOMER] == 0)
		{
			if (product_register_value[MODBUS_DIFF_PRESSURE_VALUE] > default_max_pascal)
				value_position.X = X_AXIS_MAX_VALUE_X;
			else
				value_position.X = ORIGIN_X + (X_AXIS_MAX_VALUE_X - ORIGIN_X) * (product_register_value[MODBUS_DIFF_PRESSURE_VALUE]) / default_max_pascal;
			//value_position.X = ORIGIN_X + (default_max_pascal) * product_register_value[MODBUS_DIFF_PRESSURE_VALUE] / (X_AXIS_MAX_VALUE_X - ORIGIN_X);
			if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 0) //4- 20 ma
			{
				//value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[MODBUS_OUTPUT_CUR] / 200;
				value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * (product_register_value[MODBUS_OUTPUT_CUR] - 40) / (200 - 40);
			}
			else
			{
				value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[MODBUS_OUTPUT_VOL] / 100;
			}
		}
		else
		{
			if (product_register_value[MODBUS_PASCAL_MAX] == product_register_value[MODBUS_PASCAL_MIN])
			{

			}
			else
			{
				if (product_register_value[MODBUS_DIFF_PRESSURE_VALUE] < product_register_value[MODBUS_PASCAL_MIN])
					value_position.X = ORIGIN_X;
				else if (product_register_value[MODBUS_DIFF_PRESSURE_VALUE] > product_register_value[MODBUS_PASCAL_MAX])
					value_position.X = X_AXIS_MAX_VALUE_X;
				else
					value_position.X = ORIGIN_X + (X_AXIS_MAX_VALUE_X - ORIGIN_X) * (product_register_value[MODBUS_DIFF_PRESSURE_VALUE] - product_register_value[MODBUS_PASCAL_MIN]) / (product_register_value[MODBUS_PASCAL_MAX] - product_register_value[MODBUS_PASCAL_MIN]);
				if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 0) //4- 20 ma
				{
					value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[MODBUS_OUTPUT_CUR] / 200;
				}
				else
				{
					value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[MODBUS_OUTPUT_VOL] / 100;
				}
			}
		}
		Rect PointRect;

		PointRect.X = value_position.X - 1;
		PointRect.Y = value_position.Y - 1;
		PointRect.Width = 3;
		PointRect.Height = 3;
		myRectangle_pen5 = new Pen(Color(255, 255, 0, 0), 3);
		mygraphics->DrawRectangle(myRectangle_pen5, PointRect);
		CString temp_value;
		if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 0) //4- 20 ma
		{
			temp_value.Format(_T("(%d Pascal, %.1f ma)"), product_register_value[MODBUS_DIFF_PRESSURE_VALUE], product_register_value[MODBUS_OUTPUT_CUR] / 10.0);
		}
		else
		{
			temp_value.Format(_T("(%d Pascal, %.1f V)"), product_register_value[MODBUS_DIFF_PRESSURE_VALUE], product_register_value[MODBUS_OUTPUT_VOL] / 10.0);
		}
		PointF temppoint_value;
		SolidBrush  value_color_brush(Color(255, 255, 0, 0));
		Gdiplus::Font        Valueunitfont(&UnitfontFamily, 16, FontStyleRegular, UnitPixel);
		if (PointRect.X < ORIGIN_X + 50)
		{
			temppoint_value.X = PointRect.X + 50;
			temppoint_value.Y = PointRect.Y - 20;
		}
		else
		{
			temppoint_value.X = PointRect.X + 20;
			temppoint_value.Y = PointRect.Y + 5;
		}
		mygraphics->DrawString(temp_value, -1, &Valueunitfont, temppoint_value, &value_color_brush);
		delete myRectangle_pen;
		delete mygraphics;
		delete myRectangle_pen3;
		delete myRectangle_pen4;
		delete myRectangle_pen5;
	}

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


void CAirFlowSensor::OnBnClickedRadioDefault()
{
	// TODO: 在此添加控件通知处理程序代码
	if (write_one(g_tstat_id, MODBUS_DEF_CUSTOMER, 0) > 0)
	{
		product_register_value[MODBUS_DEF_CUSTOMER] = 0;
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout!"));
		return;
	}
	PostMessage(WM_TSTAT_AIRFLOW_THREAD_READ, NULL, NULL);
}


void CAirFlowSensor::OnBnClickedRadioUserDefined()
{
	// TODO: 在此添加控件通知处理程序代码
	if (write_one(g_tstat_id, MODBUS_DEF_CUSTOMER, 1) > 0)
	{
		product_register_value[MODBUS_DEF_CUSTOMER] = 1;
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data timeout!"));
		return;
	}
	PostMessage(WM_TSTAT_AIRFLOW_THREAD_READ, NULL, NULL);
}


void CAirFlowSensor::OnEnKillfocusEditVolatgeMin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_VOLATGE_MIN, temp_cstring);
	unsigned int temp_value = unsigned int(_wtof(temp_cstring) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_VOLATGE_MAX, temp_cstring_max);
	unsigned int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((temp_value > 100) || (temp_value >= temp_value_max))
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between 0 and 10 volts"));
		return;
	}
	if (write_one(g_tstat_id, MODBUS_VOLTAGE_MIN, temp_value) > 0)
	{
		product_register_value[MODBUS_VOLTAGE_MIN] = temp_value;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CAirFlowSensor::OnEnKillfocusEditVolatgeMax()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_VOLATGE_MIN, temp_cstring);
	unsigned int temp_value = unsigned int(_wtof(temp_cstring) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_VOLATGE_MAX, temp_cstring_max);
	unsigned int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((temp_value_max > 100) || (temp_value_max <= temp_value))
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between 0 and 10 volts"));
		return;
	}

	if (write_one(g_tstat_id, MODBUS_VOLTAGE_MAX, temp_value_max) > 0)
	{
		product_register_value[MODBUS_VOLTAGE_MAX] = temp_value_max;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CAirFlowSensor::OnEnKillfocusEditCurrentMin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_CURRENT_MIN, temp_cstring);
	unsigned int temp_value_min = unsigned int(_wtof(temp_cstring) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_CURRENT_MAX, temp_cstring_max);
	unsigned int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((temp_value_min < 40) || (temp_value_min > 200) || (temp_value_min >= temp_value_max))
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between 4 and 20 "));
		return;
	}

	if (write_one(g_tstat_id, MODBUS_CURRENT_MIN, temp_value_min) > 0)
	{
		product_register_value[MODBUS_CURRENT_MIN] = temp_value_min;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CAirFlowSensor::OnEnKillfocusEditCurrentMax()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_CURRENT_MIN, temp_cstring);
	unsigned int temp_value_min = unsigned int(_wtof(temp_cstring) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_CURRENT_MAX, temp_cstring_max);
	unsigned int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((temp_value_max < 40) || (temp_value_max > 200) || (temp_value_max <= temp_value_min ))
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between 4 and 20 "));
		return;
	}


	if (write_one(g_tstat_id, MODBUS_CURRENT_MAX, temp_value_max) > 0)
	{
		product_register_value[MODBUS_CURRENT_MAX] = temp_value_max;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CAirFlowSensor::OnEnKillfocusEditPascalMin()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_cstring;
	GetDlgItemTextW(IDC_EDIT_PASCAL_MIN, temp_cstring);
	unsigned int temp_value_min = unsigned int(_wtoi(temp_cstring));

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_PASCAL_MAX, temp_cstring_max);
	unsigned int temp_value_max = unsigned int(_wtoi(temp_cstring_max));
	if (temp_value_min >= temp_value_max)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("The minimum value cannot be greater than the maximum value"));
		return;
	}
	if (write_one(g_tstat_id, MODBUS_PASCAL_MIN, temp_value_min) > 0)
	{
		product_register_value[MODBUS_PASCAL_MIN] = temp_value_min;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CAirFlowSensor::OnEnKillfocusEditPascalMax()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_PASCAL_MAX, temp_cstring_min);
	unsigned int temp_value_min = unsigned int(_wtoi(temp_cstring_min));

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_PASCAL_MAX, temp_cstring_max);
	unsigned int temp_value_max = unsigned int(_wtoi(temp_cstring_max));

	if (temp_value_min >= temp_value_max)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("The minimum value cannot be greater than the maximum value"));
		return;
	}

	if (write_one(g_tstat_id, MODBUS_PASCAL_MAX, temp_value_max) > 0)
	{
		product_register_value[MODBUS_PASCAL_MAX] = temp_value_max;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


BOOL CAirFlowSensor::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			CWnd* temp_focus = GetFocus();
			GetDlgItem(IDC_BUTTON_AIRFLOW_DONE)->SetFocus();
			temp_focus->SetFocus();
			return 1;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}
