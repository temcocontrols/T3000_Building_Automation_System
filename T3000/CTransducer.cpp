// CTransducer.cpp: 
//

#include "stdafx.h"
#include "T3000.h"
#include "CTransducer.h"
#include "global_function.h"
#include "MainFrm.h"
// CTransducer
#define WM_TRANDUCER_UPDATEUI  WM_USER + 509
IMPLEMENT_DYNCREATE(CTransducer, CFormView)

extern int CANVAS_TOP_X ;  //
extern int CANVAS_TOP_Y;
extern int CANVAS_BOTTOM_X ;
extern int CANVAS_BOTTOM_Y ;
extern int ORIGIN_X ; //
extern int ORIGIN_Y ;

extern int Y_AXIS_TOP_X ;  //Y
extern int Y_AXIS_TOP_Y ;
extern int Y_AXIS_ARROW_LEFT_X ;  //Y
extern int Y_AXIS_ARROW_LEFT_Y ;
extern int Y_AXIS_ARROW_RIGHT_X ; // Y
extern int Y_AXIS_ARROW_RIGHT_Y ;
extern int X_AXIS_RIGHT_X ;  //XX
extern int X_AXIS_RIGHT_Y ;				//XY
extern int X_AXIS_MAX_VALUE_X ; //XX
extern int X_AXIS_MAX_VALUE_Y ; //XY
extern int Y_AXIS_MAX_VALUE_X ;
extern int Y_AXIS_MAX_VALUE_Y ;
extern int X_AXIS_ARROW_TOP_X ; //X
extern int X_AXIS_ARROW_TOP_Y ;
extern int X_AXIS_ARROW_BOTTOM_X ; //X
extern int X_AXIS_ARROW_BOTTOM_Y ;

int MODBUS_SWITCH_OUTPUT_MODE=   17;
int MODBUS_SWITCH_TEMP_RANGE=   18;
int MODBUS_SWITCH_HUMI_RANGE= 19  ;
int MODBUS_TEMPERATURE_TYPE	=	23;
int MODBUS_TEMPERATURE		= 34  ;
int MODBUS_HUMIDITY			=35	  ;
int MODBUS_TEMPERATURE_OFFSET=  36 ;
int MODBUS_HUMIDITY_OFFSET  =  37 ;
int MODBUS_DEWPOINT			=41	  ;
int MODBUS_ENTHALPY			=  42 ;
int MODBUS_ABSOLUTE_HUMI	=44	  ;


//
int NEW_MODBUS_SWITCH_OUTPUT_MODE = 17;
int NEW_MODBUS_SWITCH_TEMP_RANGE = 18;
int NEW_MODBUS_SWITCH_HUMI_RANGE = 20;
int NEW_MODBUS_TEMPERATURE_UNIT = 21;
int NEW_MODBUS_TEMPERATURE_OFFSET = 35;
int NEW_MODBUS_HUMIDITY_OFFSET = 36;
int NEW_MODBUS_TEMPERATURE_C = 37;
int NEW_MODBUS_TEMPERATURE_F = 38;
int NEW_MODBUS_HUMIDITY = 39;
int NEW_MODBUS_DEWPOINT = 41;
int NEW_MODBUS_ENTHALPY = 42;
int NEW_MODBUS_ABSOLUTE_HUMI = 44;
int NEW_MODBUS_DEF_CUSTOMER_TEMP = 100;
int NEW_MODBUS_CURRENT_MIN_TEMP = 101;
int NEW_MODBUS_CURRENT_MAX_TEMP = 102;
int NEW_MODBUS_VOLTAGE_MIN_TEMP = 103;
int NEW_MODBUS_VOLTAGE_MAX_TEMP = 104;
int NEW_MODBUS_TEMP_MIN = 105;
int NEW_MODBUS_TEMP_MAX = 106;
int NEW_MODBUS_OUTPUT_CUR_TEMP = 107;
int NEW_MODBUS_OUTPUT_VOL_TEMP = 108;
int NEW_MODBUS_DEF_CUSTOMER_HUM = 110;
int NEW_MODBUS_CURRENT_MIN_HUM = 111;
int NEW_MODBUS_CURRENT_MAX_HUM = 112;
int NEW_MODBUS_VOLTAGE_MIN_HUM = 113;
int NEW_MODBUS_VOLTAGE_MAX_HUM = 114;
int NEW_MODBUS_HUM_MIN = 115;
int NEW_MODBUS_HUM_MAX = 116;
int NEW_MODBUS_OUTPUT_CUR_HUM = 117;
int NEW_MODBUS_OUTPUT_VOL_HUM = 118;


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
	ON_CBN_SELCHANGE(IDC_COMBO_TEMP_RANGE, &CTransducer::OnCbnSelchangeComboTempRange)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_DEFAULT_T, &CTransducer::OnBnClickedRadioDefaultT)
	ON_BN_CLICKED(IDC_RADIO_USER_DEFINED_T, &CTransducer::OnBnClickedRadioUserDefinedT)
	ON_BN_CLICKED(IDC_RADIO_DEFAULT_H, &CTransducer::OnBnClickedRadioDefaultH)
	ON_BN_CLICKED(IDC_RADIO_USER_DEFINED_H, &CTransducer::OnBnClickedRadioUserDefinedH)

	ON_EN_KILLFOCUS(IDC_EDIT_VOLATGE_MIN_T, &CTransducer::OnEnKillfocusEditVolatgeMinT)
	ON_EN_KILLFOCUS(IDC_EDIT_VOLATGE_MAX_T, &CTransducer::OnEnKillfocusEditVolatgeMaxT)
	ON_EN_KILLFOCUS(IDC_EDIT_CURRENT_MIN_T, &CTransducer::OnEnKillfocusEditCurrentMinT)
	ON_EN_KILLFOCUS(IDC_EDIT_CURRENT_MAX_T, &CTransducer::OnEnKillfocusEditCurrentMaxT)
	ON_EN_KILLFOCUS(IDC_EDIT_MIN_T, &CTransducer::OnEnKillfocusEditMinT)
	ON_EN_KILLFOCUS(IDC_EDIT_MAX_T, &CTransducer::OnEnKillfocusEditMaxT)
	ON_EN_KILLFOCUS(IDC_EDIT_VOLATGE_MIN_H, &CTransducer::OnEnKillfocusEditVolatgeMinH)
	ON_EN_KILLFOCUS(IDC_EDIT_VOLATGE_MAX_H, &CTransducer::OnEnKillfocusEditVolatgeMaxH)
	ON_EN_KILLFOCUS(IDC_EDIT_CURRENT_MIN_H, &CTransducer::OnEnKillfocusEditCurrentMinH)
	ON_EN_KILLFOCUS(IDC_EDIT_CURRENT_MAX_H, &CTransducer::OnEnKillfocusEditCurrentMaxH)
	ON_EN_KILLFOCUS(IDC_EDIT_MIN_H, &CTransducer::OnEnKillfocusEditMinH)
	ON_EN_KILLFOCUS(IDC_EDIT_MAX_H, &CTransducer::OnEnKillfocusEditMaxH)
END_MESSAGE_MAP()













// CTransducer 

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


// CTransducer 
HANDLE h_xducer_thread = NULL;

void CTransducer::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 
}


void CTransducer::InitialPaintParameter()
{
     CANVAS_TOP_X = 550 + 50;  //
	 CANVAS_TOP_Y = 0 + 50;
	 CANVAS_BOTTOM_X = 1210 + 50;
	 CANVAS_BOTTOM_Y = 420 + 100;
	 ORIGIN_X = 600 + 50; //
	 ORIGIN_Y = 400 + 50;
	 Y_AXIS_TOP_X = ORIGIN_X;  //Y
	 Y_AXIS_TOP_Y = CANVAS_TOP_Y + 10;
	 Y_AXIS_ARROW_LEFT_X = Y_AXIS_TOP_X - 5;  //Y
	 Y_AXIS_ARROW_LEFT_Y = Y_AXIS_TOP_Y + 10;
	 Y_AXIS_ARROW_RIGHT_X = Y_AXIS_TOP_X + 5; // Y
	 Y_AXIS_ARROW_RIGHT_Y = Y_AXIS_TOP_Y + 10;

	 X_AXIS_RIGHT_X = CANVAS_BOTTOM_X - 10;  //XX
	 X_AXIS_RIGHT_Y = ORIGIN_Y;				//XY

	 X_AXIS_MAX_VALUE_X = X_AXIS_RIGHT_X - 100; //XX
	 X_AXIS_MAX_VALUE_Y = X_AXIS_RIGHT_Y; //XY

	 Y_AXIS_MAX_VALUE_X = Y_AXIS_TOP_X;
	 Y_AXIS_MAX_VALUE_Y = Y_AXIS_TOP_Y + 50;

	 X_AXIS_ARROW_TOP_X = X_AXIS_RIGHT_X - 10; //X
	 X_AXIS_ARROW_TOP_Y = X_AXIS_RIGHT_Y - 5;
	 X_AXIS_ARROW_BOTTOM_X = X_AXIS_RIGHT_X - 10; //X
	 X_AXIS_ARROW_BOTTOM_Y = X_AXIS_RIGHT_Y + 5;
}

void CTransducer::Fresh()
{
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	pFrame->SetWindowTextW(cs_special_name + CurrentT3000Version);

	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TRANSDUCER_TEMPERATURE); // Picture Control 
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
	//
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	//ture
	p->SetBitmap(bitmap);

	((CComboBox*)GetDlgItem(IDC_COMBO_TEMP_RANGE))->ResetContent();
	for (int x = 0; x < (sizeof(TransducerTempRange) / sizeof(TransducerTempRange[0])); x++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_TEMP_RANGE))->AddString(TransducerTempRange[x]);
	}
	if (product_register_value[4] >= 25)
	{
		is_new_firmware = true; //er  
	}
	else
		is_new_firmware = false; //er  

	if (h_xducer_thread == NULL)
	{
		h_xducer_thread = CreateThread(NULL, NULL, Update_xducer_Thread, this, NULL, NULL);
		CloseHandle(h_xducer_thread);
	}
	InitialPaintParameter();
		UpdateUI(NULL,NULL);
	return;
}

DWORD WINAPI Update_xducer_Thread(LPVOID lPvoid)
{
	CTransducer* mparent = (CTransducer*)lPvoid;
	while (mparent->IsWindowVisible())
	{
		Read_Multi(g_tstat_id, &product_register_value[0], 0, 100, 5);
		Read_Multi(g_tstat_id, &product_register_value[100], 100, 100, 5);
		PostMessage(mparent->m_hWnd, WM_TRANDUCER_UPDATEUI, NULL, NULL);
		Sleep(6000);
	}
	h_xducer_thread = NULL;
	return 1;
}

void CTransducer::EnableNewUI(bool flag)
{
	if (flag)
	{
		GetDlgItem(IDC_EDIT_VOLATGE_MIN_T)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_VOLATGE_MAX_T)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_CURRENT_MIN_T)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_CURRENT_MAX_T)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_MIN_T)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_MAX_T)->EnableWindow(1);
		GetDlgItem(IDC_STATIC_OUT_CURRENT_T)->EnableWindow(1);
		GetDlgItem(IDC_STATIC_OUT_VOLTAGE_T)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_VOLATGE_MIN_H)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_VOLATGE_MAX_H)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_CURRENT_MIN_H)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_CURRENT_MAX_H)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_MIN_H)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_MAX_H)->EnableWindow(1);
		GetDlgItem(IDC_STATIC_OUT_CURRENT_H)->EnableWindow(1);
		GetDlgItem(IDC_STATIC_OUT_VOLTAGE_H)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_EDIT_VOLATGE_MIN_T)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_VOLATGE_MAX_T)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_CURRENT_MIN_T)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_CURRENT_MAX_T)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_MIN_T)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_MAX_T)->EnableWindow(0);
		GetDlgItem(IDC_STATIC_OUT_CURRENT_T)->EnableWindow(0);
		GetDlgItem(IDC_STATIC_OUT_VOLTAGE_T)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_VOLATGE_MIN_H)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_VOLATGE_MAX_H)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_CURRENT_MIN_H)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_CURRENT_MAX_H)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_MIN_H)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_MAX_H)->EnableWindow(0);
		GetDlgItem(IDC_STATIC_OUT_CURRENT_H)->EnableWindow(0);
		GetDlgItem(IDC_STATIC_OUT_VOLTAGE_H)->EnableWindow(0);
	}
}

LRESULT CTransducer::UpdateUI(WPARAM wParam, LPARAM lParam)
{
	if (is_new_firmware)
	{
		//Read_Multi(g_tstat_id, product_register_value, 0, 100, 2);
		//Read_Multi(g_tstat_id, &product_register_value[100], 100, 100, 2);
		EnableNewUI(1);
		if (product_register_value[NEW_MODBUS_SWITCH_TEMP_RANGE] < (sizeof(TransducerTempRange) / sizeof(TransducerTempRange[0])))
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_TEMP_RANGE))->SetCurSel(product_register_value[NEW_MODBUS_SWITCH_TEMP_RANGE]);
		}
		if (product_register_value[NEW_MODBUS_TEMPERATURE_UNIT] == 0)
		{
			((CButton*)GetDlgItem(IDC_RADIO_TRANSDUCER_DEG_C))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_TRANSDUCER_DEG_F))->SetCheck(0);
			GetDlgItem(IDC_STATIC_TRANSDUCER_TEMP_UNITS)->SetWindowText(_T("Deg.C"));
			GetDlgItem(IDC_STATIC_TRANSDUCER_TEMP_UNITS2)->SetWindowText(_T("Deg.C"));
			GetDlgItem(IDC_STATIC_T_UNIT)->SetWindowText(_T("Temperature\r\n       Deg.C"));
			
			CString temp_value;
			temp_value.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_TEMPERATURE_C] / 10.0);
			GetDlgItem(IDC_STATIC_TEMPERATURE)->SetWindowText(temp_value);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_RADIO_TRANSDUCER_DEG_C))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_TRANSDUCER_DEG_F))->SetCheck(1);
			GetDlgItem(IDC_STATIC_TRANSDUCER_TEMP_UNITS)->SetWindowText(_T("Deg.F"));
			GetDlgItem(IDC_STATIC_TRANSDUCER_TEMP_UNITS2)->SetWindowText(_T("Deg.F"));
			GetDlgItem(IDC_STATIC_T_UNIT)->SetWindowText(_T("Temperature\r\n       Deg.F"));
			CString temp_value;
			temp_value.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_TEMPERATURE_F] / 10.0);
			GetDlgItem(IDC_STATIC_TEMPERATURE)->SetWindowText(temp_value);
		}

		if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0)
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
		CString hum_abs_value;
		CString hum_enthalpy_value;
		CString hum_dew_value;
		if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_DEWPOINT)
		{
			((CButton*)GetDlgItem(IDC_RADIO_DEW))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_ENTHALPY))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ABS))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_REAL))->SetCheck(0);
		}
		else if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_ENTHALPY)
		{
			((CButton*)GetDlgItem(IDC_RADIO_DEW))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ENTHALPY))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_ABS))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_REAL))->SetCheck(0);
		}
		else if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_ABSOLUTE)
		{
			((CButton*)GetDlgItem(IDC_RADIO_DEW))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ENTHALPY))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ABS))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_REAL))->SetCheck(0);
		}
		else if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_REAL)
		{
			((CButton*)GetDlgItem(IDC_RADIO_DEW))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ENTHALPY))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_ABS))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_REAL))->SetCheck(1);
		}
		hum_dew_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_DEWPOINT] / 10.0);
		hum_enthalpy_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_ENTHALPY] / 10.0);
		hum_abs_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_ABSOLUTE_HUMI] / 10.0);
		hum_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_HUMIDITY] / 10.0);
		GetDlgItem(IDC_STATIC_TRANSDUCER_DEW_VALUE)->SetWindowText(hum_dew_value);
		GetDlgItem(IDC_STATIC_TRANSDUCER_ENTHALPY_VALUE2)->SetWindowText(hum_enthalpy_value);
		GetDlgItem(IDC_STATIC_TRANSDUCER_ABS_VALUE)->SetWindowText(hum_abs_value);
		GetDlgItem(IDC_STATIC_TRANSDUCER_HUM_VALUE)->SetWindowText(hum_value);


		short temp_offset;
		short hum_offset;
		temp_offset = product_register_value[NEW_MODBUS_TEMPERATURE_OFFSET];
		hum_offset = product_register_value[NEW_MODBUS_HUMIDITY_OFFSET];
		CString cs_temp_offset;
		CString cs_hum_offset;
		cs_temp_offset.Format(_T("%.1f"), temp_offset / 10.0);
		cs_hum_offset.Format(_T("%.1f"), hum_offset / 10.0);
		GetDlgItem(IDC_EDIT_TEMP_OFFSET)->SetWindowText(cs_temp_offset);
		GetDlgItem(IDC_EDIT_HUM_OFFSET)->SetWindowText(cs_hum_offset);
		GetDlgItem(IDC_BUTTON_DONE)->SetFocus();

		CString temp_v_min_value;
		CString temp_v_max_value;
		CString temp_c_min_value;
		CString temp_c_max_value;
		CString temp_t_min_value;
		CString temp_t_max_value;
		CString temp_output_c_value;
		CString temp_output_v_value;
		temp_v_min_value.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_VOLTAGE_MIN_TEMP]/10.0);
		temp_v_max_value.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_VOLTAGE_MAX_TEMP] / 10.0);
		temp_c_min_value.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_CURRENT_MIN_TEMP] / 10.0);
		temp_c_max_value.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_CURRENT_MAX_TEMP] / 10.0);
		//if (product_register_value[NEW_MODBUS_TEMP_MIN] > 0x7fff)
		//{
		//	temp_t_min_value.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_TEMP_MIN] / 10.0);
		//	Sleep(1);
		//}
		//else
		temp_t_min_value.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_TEMP_MIN] / 10.0);
		temp_t_max_value.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_TEMP_MAX] / 10.0);
		temp_output_c_value.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_OUTPUT_CUR_TEMP] / 10.0);
		temp_output_v_value.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_OUTPUT_VOL_TEMP] / 10.0);

		GetDlgItem(IDC_EDIT_VOLATGE_MIN_T)->SetWindowTextW(temp_v_min_value);
		GetDlgItem(IDC_EDIT_VOLATGE_MAX_T)->SetWindowTextW(temp_v_max_value);
		GetDlgItem(IDC_EDIT_CURRENT_MIN_T)->SetWindowTextW(temp_c_min_value);
		GetDlgItem(IDC_EDIT_CURRENT_MAX_T)->SetWindowTextW(temp_c_max_value);
		GetDlgItem(IDC_EDIT_MIN_T)->SetWindowTextW(temp_t_min_value);
		GetDlgItem(IDC_EDIT_MAX_T)->SetWindowTextW(temp_t_max_value);
		GetDlgItem(IDC_STATIC_OUT_CURRENT_T)->SetWindowTextW(temp_output_c_value);
		GetDlgItem(IDC_STATIC_OUT_VOLTAGE_T)->SetWindowTextW(temp_output_v_value);
		if (product_register_value[NEW_MODBUS_DEF_CUSTOMER_TEMP] == 0)
		{
			GetDlgItem(IDC_EDIT_VOLATGE_MIN_T)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_VOLATGE_MAX_T)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_CURRENT_MIN_T)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_CURRENT_MAX_T)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_MIN_T)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_MAX_T)->EnableWindow(false);
			((CButton*)GetDlgItem(IDC_RADIO_DEFAULT_T))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_USER_DEFINED_T))->SetCheck(0);
		}
		else
		{
			if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0) //ma
			{
				GetDlgItem(IDC_EDIT_CURRENT_MIN_T)->EnableWindow(1);
				GetDlgItem(IDC_EDIT_CURRENT_MAX_T)->EnableWindow(1);
				GetDlgItem(IDC_EDIT_VOLATGE_MIN_T)->EnableWindow(0);
				GetDlgItem(IDC_EDIT_VOLATGE_MAX_T)->EnableWindow(0);
			}
			else
			{
				GetDlgItem(IDC_EDIT_CURRENT_MIN_T)->EnableWindow(0);
				GetDlgItem(IDC_EDIT_CURRENT_MAX_T)->EnableWindow(0);
				GetDlgItem(IDC_EDIT_VOLATGE_MIN_T)->EnableWindow(1);
				GetDlgItem(IDC_EDIT_VOLATGE_MAX_T)->EnableWindow(1);
			}



			GetDlgItem(IDC_EDIT_MIN_T)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_MAX_T)->EnableWindow(1);
			((CButton*)GetDlgItem(IDC_RADIO_DEFAULT_T))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_USER_DEFINED_T))->SetCheck(1);
		}
		CString humidity_v_min_value;
		CString humidity_v_max_value;
		CString humidity_c_min_value;
		CString humidity_c_max_value;
		CString humidity_t_min_value;
		CString humidity_t_max_value;
		CString humidity_output_c_value;
		CString humidity_output_v_value;
		humidity_v_min_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_VOLTAGE_MIN_HUM] / 10.0);
		humidity_v_max_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_VOLTAGE_MAX_HUM] / 10.0);
		humidity_c_min_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_CURRENT_MIN_HUM] / 10.0);
		humidity_c_max_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_CURRENT_MAX_HUM] / 10.0);
		humidity_t_min_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_HUM_MIN] / 10.0);
		humidity_t_max_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_HUM_MAX] / 10.0);
		humidity_output_c_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_OUTPUT_CUR_HUM] / 10.0);
		humidity_output_v_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_OUTPUT_VOL_HUM] / 10.0);

		GetDlgItem(IDC_EDIT_VOLATGE_MIN_H)->SetWindowTextW(humidity_v_min_value);
		GetDlgItem(IDC_EDIT_VOLATGE_MAX_H)->SetWindowTextW(humidity_v_max_value);
		GetDlgItem(IDC_EDIT_CURRENT_MIN_H)->SetWindowTextW(humidity_c_min_value);
		GetDlgItem(IDC_EDIT_CURRENT_MAX_H)->SetWindowTextW(humidity_c_max_value);
		GetDlgItem(IDC_EDIT_MIN_H)->SetWindowTextW(humidity_t_min_value);
		GetDlgItem(IDC_EDIT_MAX_H)->SetWindowTextW(humidity_t_max_value);
		GetDlgItem(IDC_STATIC_OUT_CURRENT_H)->SetWindowTextW(humidity_output_c_value);
		GetDlgItem(IDC_STATIC_OUT_VOLTAGE_H)->SetWindowTextW(humidity_output_v_value);

		if (product_register_value[NEW_MODBUS_DEF_CUSTOMER_HUM] == 0)
		{
			GetDlgItem(IDC_EDIT_VOLATGE_MIN_H)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_VOLATGE_MAX_H)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_CURRENT_MIN_H)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_CURRENT_MAX_H)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_MIN_H)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_MAX_H)->EnableWindow(false);
			((CButton*)GetDlgItem(IDC_RADIO_DEFAULT_H))->SetCheck(1);
			((CButton*)GetDlgItem(IDC_RADIO_USER_DEFINED_H))->SetCheck(0);
		}
		else
		{
			if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0) //ma
			{
				GetDlgItem(IDC_EDIT_VOLATGE_MIN_H)->EnableWindow(0);
				GetDlgItem(IDC_EDIT_VOLATGE_MAX_H)->EnableWindow(0);
				GetDlgItem(IDC_EDIT_CURRENT_MIN_H)->EnableWindow(1);
				GetDlgItem(IDC_EDIT_CURRENT_MAX_H)->EnableWindow(1);
			}
			else
			{
				GetDlgItem(IDC_EDIT_VOLATGE_MIN_H)->EnableWindow(1);
				GetDlgItem(IDC_EDIT_VOLATGE_MAX_H)->EnableWindow(1);
				GetDlgItem(IDC_EDIT_CURRENT_MIN_H)->EnableWindow(0);
				GetDlgItem(IDC_EDIT_CURRENT_MAX_H)->EnableWindow(0);
			}

			GetDlgItem(IDC_EDIT_MIN_H)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_MAX_H)->EnableWindow(1);
			((CButton*)GetDlgItem(IDC_RADIO_DEFAULT_H))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_RADIO_USER_DEFINED_H))->SetCheck(1);
		}
	}
	else
	{
		EnableNewUI(0);
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
		cs_temp_offset.Format(_T("%.1f"), temp_offset / 10.0);
		cs_hum_offset.Format(_T("%.1f"), hum_offset / 10.0);
		GetDlgItem(IDC_EDIT_TEMP_OFFSET)->SetWindowText(cs_temp_offset);
		GetDlgItem(IDC_EDIT_HUM_OFFSET)->SetWindowText(cs_hum_offset);
		GetDlgItem(IDC_BUTTON_DONE)->SetFocus();
	}
	Invalidate(1);
	return 0;
}

void CTransducer::OnBnClickedRadioTransducerDegC()
{
	// TODO: 
	if (is_new_firmware)
	{
		if (product_register_value[NEW_MODBUS_TEMPERATURE_UNIT] == 0)
			return;
		product_register_value[NEW_MODBUS_TEMPERATURE_UNIT] = 0;
		unsigned int temp_value = 0;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_TEMPERATURE_UNIT, temp_value,
			product_register_value[NEW_MODBUS_TEMPERATURE_UNIT], this->m_hWnd, IDC_RADIO_TRANSDUCER_DEG_C, _T(" Temperature type  "));
	}
	else
	{
		if (product_register_value[MODBUS_TEMPERATURE_TYPE] == 0)
			return;
		product_register_value[MODBUS_TEMPERATURE_TYPE] = 0;
		unsigned int temp_value = 0;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_TEMPERATURE_TYPE, temp_value,
			product_register_value[MODBUS_TEMPERATURE_TYPE], this->m_hWnd, IDC_RADIO_TRANSDUCER_DEG_C, _T(" Temperature type  "));
	}
	
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnBnClickedRadioTransducerDegF()
{
	// TODO: 
	if (is_new_firmware)
	{
		if (product_register_value[NEW_MODBUS_TEMPERATURE_UNIT] == 1)
			return;
		product_register_value[NEW_MODBUS_TEMPERATURE_UNIT] = 1;
		unsigned int temp_value = 1;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_TEMPERATURE_UNIT, temp_value,
			product_register_value[NEW_MODBUS_TEMPERATURE_UNIT], this->m_hWnd, IDC_RADIO_TRANSDUCER_DEG_C, _T(" Temperature type  "));
	}
	else
	{
		if (product_register_value[MODBUS_TEMPERATURE_TYPE] == 1)
			return;
		product_register_value[MODBUS_TEMPERATURE_TYPE] = 1;
		unsigned int temp_value = 1;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_TEMPERATURE_TYPE, temp_value,
			product_register_value[MODBUS_TEMPERATURE_TYPE], this->m_hWnd, IDC_RADIO_TRANSDUCER_DEG_C, _T(" Temperature type  "));
	}
	
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}



void CTransducer::OnEnKillfocusEditTempOffset()
{
	// TODO: 
	if (is_new_firmware)
	{
		CString strText;
		GetDlgItem(IDC_EDIT_TEMP_OFFSET)->GetWindowText(strText);
		unsigned short nValue = (unsigned short)(_wtof(strText) * 10);

		if (product_register_value[NEW_MODBUS_TEMPERATURE_OFFSET] == nValue)	//Add this to judge weather this value need to change.
			return;
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_TEMPERATURE_OFFSET, nValue,
			product_register_value[NEW_MODBUS_TEMPERATURE_OFFSET], this->m_hWnd, IDC_EDIT_DTERM, _T("Temperature offset "));
	}
	else
	{
		CString strText;
		GetDlgItem(IDC_EDIT_TEMP_OFFSET)->GetWindowText(strText);
		unsigned short nValue = (unsigned short)(_wtof(strText) * 10);

		if (product_register_value[MODBUS_TEMPERATURE_OFFSET] == nValue)	//Add this to judge weather this value need to change.
			return;
		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_TEMPERATURE_OFFSET, nValue,
			product_register_value[MODBUS_TEMPERATURE_OFFSET], this->m_hWnd, IDC_EDIT_DTERM, _T("Temperature offset "));
	}

	
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnEnKillfocusEditHumOffset()
{
	// TODO: 
	if (is_new_firmware)
	{
		CString strText;
		GetDlgItem(IDC_EDIT_HUM_OFFSET)->GetWindowText(strText);
		unsigned short nValue = (unsigned short)(_wtof(strText) * 10);

		if (product_register_value[NEW_MODBUS_HUMIDITY_OFFSET] == nValue)	//Add this to judge weather this value need to change.
			return;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_HUMIDITY_OFFSET, nValue,
			product_register_value[NEW_MODBUS_HUMIDITY_OFFSET], this->m_hWnd, IDC_EDIT_DTERM, _T("Humidity offset "));
	}
	else
	{
		CString strText;
		GetDlgItem(IDC_EDIT_HUM_OFFSET)->GetWindowText(strText);
		unsigned short nValue = (unsigned short)(_wtof(strText) * 10);

		if (product_register_value[MODBUS_HUMIDITY_OFFSET] == nValue)	//Add this to judge weather this value need to change.
			return;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_HUMIDITY_OFFSET, nValue,
			product_register_value[MODBUS_HUMIDITY_OFFSET], this->m_hWnd, IDC_EDIT_DTERM, _T("Humidity offset "));
	}

	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnBnClickedRadioTransducer420Ma()
{
	// TODO: 
	if (is_new_firmware)
	{
		if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0)
			return;
		product_register_value[MODBUS_TEMPERATURE_TYPE] = 0;
		unsigned int temp_value = 0;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_SWITCH_OUTPUT_MODE, temp_value,
			product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE], this->m_hWnd, IDC_RADIO_TRANSDUCER_4_20_MA, _T(" Output Mode  "));
	}
	else
	{
		if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 0)
			return;
		product_register_value[MODBUS_TEMPERATURE_TYPE] = 0;
		unsigned int temp_value = 0;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_SWITCH_OUTPUT_MODE, temp_value,
			product_register_value[MODBUS_SWITCH_OUTPUT_MODE], this->m_hWnd, IDC_RADIO_TRANSDUCER_4_20_MA, _T(" Output Mode  "));
	}
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnBnClickedRadioTransducer010v()
{
	// TODO: 
	if (is_new_firmware)
	{
		if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 1)
			return;
		product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] = 1;
		unsigned int temp_value = 1;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_SWITCH_OUTPUT_MODE, temp_value,
			product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE], this->m_hWnd, IDC_RADIO_TRANSDUCER_4_20_MA, _T(" Output Mode  "));
	}
	else
	{
		if (product_register_value[MODBUS_SWITCH_OUTPUT_MODE] == 1)
			return;
		product_register_value[MODBUS_TEMPERATURE_TYPE] = 1;
		unsigned int temp_value = 1;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_SWITCH_OUTPUT_MODE, temp_value,
			product_register_value[MODBUS_SWITCH_OUTPUT_MODE], this->m_hWnd, IDC_RADIO_TRANSDUCER_4_20_MA, _T(" Output Mode  "));
	}
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnBnClickedRadioDew()
{
	// TODO: 
	if (is_new_firmware)
	{
		if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_DEWPOINT)
			return;
		product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] = HUM_DEWPOINT;
		unsigned int temp_value = HUM_DEWPOINT;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_SWITCH_HUMI_RANGE, temp_value,
			product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE], this->m_hWnd, IDC_RADIO_DEW, _T(" Humidity range  "));
	}
	else
	{
		if (product_register_value[MODBUS_SWITCH_HUMI_RANGE] == HUM_DEWPOINT)
			return;
		product_register_value[MODBUS_SWITCH_HUMI_RANGE] = HUM_DEWPOINT;
		unsigned int temp_value = HUM_DEWPOINT;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_SWITCH_HUMI_RANGE, temp_value,
			product_register_value[MODBUS_SWITCH_HUMI_RANGE], this->m_hWnd, IDC_RADIO_DEW, _T(" Humidity range  "));
	}

	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnBnClickedRadioEnthalpy()
{
	// TODO: 
	if (is_new_firmware)
	{
		if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_ENTHALPY)
			return;
		product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] = HUM_ENTHALPY;
		unsigned int temp_value = HUM_ENTHALPY;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_SWITCH_HUMI_RANGE, temp_value,
			product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE], this->m_hWnd, IDC_RADIO_ENTHALPY, _T(" Humidity range  "));
	}
	else
	{

		if (product_register_value[MODBUS_SWITCH_HUMI_RANGE] == HUM_ENTHALPY)
			return;
		product_register_value[MODBUS_SWITCH_HUMI_RANGE] = HUM_ENTHALPY;
		unsigned int temp_value = HUM_ENTHALPY;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_SWITCH_HUMI_RANGE, temp_value,
			product_register_value[MODBUS_SWITCH_HUMI_RANGE], this->m_hWnd, IDC_RADIO_ENTHALPY, _T(" Humidity range  "));
	}
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnBnClickedRadioAbs()
{
	// TODO: 
	if (is_new_firmware)
	{
		if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_ABSOLUTE)
			return;
		product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] = HUM_ABSOLUTE;
		unsigned int temp_value = HUM_ABSOLUTE;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_SWITCH_HUMI_RANGE, temp_value,
			product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE], this->m_hWnd, IDC_RADIO_ABS, _T(" Humidity range  "));
	}
	else
	{
		if (product_register_value[MODBUS_SWITCH_HUMI_RANGE] == HUM_ABSOLUTE)
			return;
		product_register_value[MODBUS_SWITCH_HUMI_RANGE] = HUM_ABSOLUTE;
		unsigned int temp_value = HUM_ABSOLUTE;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_SWITCH_HUMI_RANGE, temp_value,
			product_register_value[MODBUS_SWITCH_HUMI_RANGE], this->m_hWnd, IDC_RADIO_ABS, _T(" Humidity range  "));
	}
	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnBnClickedRadioReal()
{
	// TODO: 
	if (is_new_firmware)
	{
		if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_REAL)
			return;
		product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] = HUM_REAL;
		unsigned int temp_value = HUM_REAL;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_SWITCH_HUMI_RANGE, temp_value,
			product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE], this->m_hWnd, IDC_RADIO_REAL, _T(" Humidity range  "));
	}
	else
	{
		if (product_register_value[MODBUS_SWITCH_HUMI_RANGE] == HUM_REAL)
			return;
		product_register_value[MODBUS_SWITCH_HUMI_RANGE] = HUM_REAL;
		unsigned int temp_value = HUM_REAL;

		Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, MODBUS_SWITCH_HUMI_RANGE, temp_value,
			product_register_value[MODBUS_SWITCH_HUMI_RANGE], this->m_hWnd, IDC_RADIO_REAL, _T(" Humidity range  "));
	}

	Sleep(500);
	PostMessage(WM_TRANDUCER_UPDATEUI, NULL, NULL);
}


void CTransducer::OnCbnSelchangeComboTempRange()
{
	// TODO: 

	int n_value = 0;
	CString temp_string;
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_TEMP_RANGE))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_TEMP_RANGE))->GetLBText(nSel, temp_string);
	if (temp_string.CompareNoCase(TransducerTempRange[0]) == 0)
	{
		n_value = 0;
	}
	else if (temp_string.CompareNoCase(TransducerTempRange[1]) == 0)
	{
		n_value = 1;
	}
	else if (temp_string.CompareNoCase(TransducerTempRange[2]) == 0)
	{
		n_value = 2;
	}
	else if (temp_string.CompareNoCase(TransducerTempRange[3]) == 0)
	{
		n_value = 3;
	}
	else if (temp_string.CompareNoCase(TransducerTempRange[4]) == 0)
	{
		n_value = 4;
	}
	else if (temp_string.CompareNoCase(TransducerTempRange[5]) == 0)
	{
		n_value = 5;
	}
	else if (temp_string.CompareNoCase(TransducerTempRange[6]) == 0)
	{
		n_value = 6;
	}
	else if (temp_string.CompareNoCase(TransducerTempRange[7]) == 0)
	{
		n_value = 7;
	}
	if (write_one(g_tstat_id, NEW_MODBUS_SWITCH_TEMP_RANGE, n_value) > 0)
	{
		product_register_value[NEW_MODBUS_SWITCH_TEMP_RANGE] = n_value;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set temperature range Success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Set temperature range Timeout"));
	}

}



void CTransducer::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 
					   // rmView::OnPaint()
	unsigned short temp_temperature = 0;
	if (product_register_value[NEW_MODBUS_TEMPERATURE_UNIT] == 0)
		temp_temperature  = product_register_value[NEW_MODBUS_TEMPERATURE_C];
	else
		temp_temperature = product_register_value[NEW_MODBUS_TEMPERATURE_F];
	if (is_new_firmware)
	{
		Pen* myRectangle_pen;
		Graphics* mygraphics;
		Pen* myRectangle_pen3;
		Pen* myRectangle_pen4;
		Pen* myRectangle_pen5;
		Pen* myRectangle_pen_hun;
		myRectangle_pen = new Pen(Color(255, 0, 0, 0));
		CRect myrect(CANVAS_TOP_X, CANVAS_TOP_Y, CANVAS_BOTTOM_X, CANVAS_BOTTOM_Y);
		CMemDC memDC(dc, &myrect);
		memDC.GetDC().FillSolidRect(&myrect, RGB(230, 230, 230));

		mygraphics = new Graphics(memDC.GetDC());

		//mygraphics->DrawRectangle(myRectangle_pen, 610, 20, 890, 400);
		mygraphics->DrawLine(myRectangle_pen, ORIGIN_X, ORIGIN_Y, Y_AXIS_TOP_X, Y_AXIS_TOP_Y);
		mygraphics->DrawLine(myRectangle_pen, ORIGIN_X, ORIGIN_Y, X_AXIS_RIGHT_X, X_AXIS_RIGHT_Y);

		mygraphics->DrawLine(myRectangle_pen, Y_AXIS_TOP_X, Y_AXIS_TOP_Y, Y_AXIS_ARROW_LEFT_X, Y_AXIS_ARROW_LEFT_Y); //Y
		mygraphics->DrawLine(myRectangle_pen, Y_AXIS_TOP_X, Y_AXIS_TOP_Y, Y_AXIS_ARROW_RIGHT_X, Y_AXIS_ARROW_RIGHT_Y);

		mygraphics->DrawLine(myRectangle_pen, X_AXIS_RIGHT_X, X_AXIS_RIGHT_Y, X_AXIS_ARROW_TOP_X, X_AXIS_ARROW_TOP_Y); //X
		mygraphics->DrawLine(myRectangle_pen, X_AXIS_RIGHT_X, X_AXIS_RIGHT_Y, X_AXIS_ARROW_BOTTOM_X, X_AXIS_ARROW_BOTTOM_Y);

		CString cs_show_info;
		CString cs_show_info2;

		FontFamily  UnitfontFamily(_T("Arial"));

		if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 1)
		{
			cs_show_info = _T("0 V");
			cs_show_info2 = _T("10 V");
		}
		else if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0)
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
		short default_min_pascal = 0;
		CString cs_show_info3;
		CString cs_show_info4;
#pragma region for temp
		if (product_register_value[NEW_MODBUS_DEF_CUSTOMER_TEMP] == 0)
		{
			pointF.X = ORIGIN_X;
			pointF.Y = ORIGIN_Y + 5;
			if (product_register_value[NEW_MODBUS_SWITCH_TEMP_RANGE] == 0)
			{
				cs_show_info3 = _T("0 ");
				cs_show_info4 = _T("100 Deg.C");
				default_min_pascal = 0;
				default_max_pascal = 1000;
			}
			else if (product_register_value[NEW_MODBUS_SWITCH_TEMP_RANGE] == 1)
			{
				cs_show_info3 = _T("-20 ");
				cs_show_info4 = _T("80 Deg.C");
				default_min_pascal = -200;
				default_max_pascal = 800;
			}
			else if (product_register_value[NEW_MODBUS_SWITCH_TEMP_RANGE] == 2)
			{
				cs_show_info3 = _T("0 ");
				cs_show_info4 = _T("50 Deg.C");
				default_min_pascal = 0;
				default_max_pascal = 500;
			}
			else if (product_register_value[NEW_MODBUS_SWITCH_TEMP_RANGE] == 3)
			{
				cs_show_info3 = _T("-50 ");
				cs_show_info4 = _T("50 Deg.C");
				default_min_pascal = -500;
				default_max_pascal = 500;
			}
			else if (product_register_value[NEW_MODBUS_SWITCH_TEMP_RANGE] == 4)
			{
				cs_show_info3 = _T("-20 ");
				cs_show_info4 = _T("140 Deg.F");
				default_min_pascal = -200;
				default_max_pascal = 1400;
			}
			else if (product_register_value[NEW_MODBUS_SWITCH_TEMP_RANGE] == 5)
			{
				cs_show_info3 = _T("0 ");
				cs_show_info4 = _T("100 Deg.F");
				default_min_pascal = 0;
				default_max_pascal = 1000;
			}
			else if (product_register_value[NEW_MODBUS_SWITCH_TEMP_RANGE] == 6)
			{
				cs_show_info3 = _T("40 ");
				cs_show_info4 = _T("90 Deg.F");
				default_min_pascal = 400;
				default_max_pascal = 900;
			}
			else if (product_register_value[NEW_MODBUS_SWITCH_TEMP_RANGE] == 7)
			{
				cs_show_info3 = _T("-40 ");
				cs_show_info4 = _T("140 Deg.F");
				default_min_pascal = -400;
				default_max_pascal = 1400;
			}
		}
		else
		{
			cs_show_info3.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_TEMP_MIN] / 10.0);
			pointF.X = ORIGIN_X;
			pointF.Y = ORIGIN_Y + 10;

			CString temp;
			temp.Format(_T("%.1f"), (short)product_register_value[NEW_MODBUS_TEMP_MAX] / 10.0);
			if (product_register_value[NEW_MODBUS_TEMPERATURE_UNIT] == 0)
				cs_show_info4 = temp + _T(" Deg.C");
			else
				cs_show_info4 = temp + _T(" Deg.F");
			default_min_pascal = product_register_value[NEW_MODBUS_TEMP_MIN];
			default_max_pascal = product_register_value[NEW_MODBUS_TEMP_MAX];

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
		myRectangle_pen_hun = new Pen(Color(255, 255, 255, 0), 2.5);

		if (product_register_value[NEW_MODBUS_DEF_CUSTOMER_TEMP] == 0)
		{
			mygraphics->DrawLine(myRectangle_pen4, ORIGIN_X, ORIGIN_Y, X_AXIS_MAX_VALUE_X, Y_AXIS_MAX_VALUE_Y);
		}
		else
		{
			PointF      cus_point1;
			PointF      cus_point2;
			CString first_point_y_value;
			CString second_point_y_value;

			if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0) //4-20ma
			{
				cus_point1.X = ORIGIN_X;
				cus_point1.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_CURRENT_MIN_TEMP] / 200;

				first_point_y_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_CURRENT_MIN_TEMP] / 10.0);
				second_point_y_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_CURRENT_MAX_TEMP] / 10.0);
				mygraphics->DrawString(first_point_y_value, -1, &unitfont, cus_point1, &txt_color_brush);

				cus_point2.X = X_AXIS_MAX_VALUE_X;
				cus_point2.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_CURRENT_MAX_TEMP] / 200;
				mygraphics->DrawString(second_point_y_value, -1, &unitfont, cus_point2, &txt_color_brush);

				mygraphics->DrawLine(myRectangle_pen4, cus_point1.X, cus_point1.Y, cus_point2.X, cus_point2.Y);
			}
			else if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 1) // 0 - 10 V
			{
				cus_point1.X = ORIGIN_X;// ORIGIN_X - (X_AXIS_MAX_VALUE_X - ORIGIN_X) / (product_register_value[MODBUS_PASCAL_MAX] - product_register_value[MODBUS_PASCAL_MIN]) * product_register_value[MODBUS_PASCAL_MIN];
				cus_point1.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_VOLTAGE_MIN_TEMP] / 100;

				first_point_y_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_VOLTAGE_MIN_TEMP] / 10.0);
				second_point_y_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_VOLTAGE_MAX_TEMP] / 10.0);
				mygraphics->DrawString(first_point_y_value, -1, &unitfont, cus_point1, &txt_color_brush);

				cus_point2.X = X_AXIS_MAX_VALUE_X;
				cus_point2.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_VOLTAGE_MAX_TEMP] / 100;
				mygraphics->DrawString(second_point_y_value, -1, &unitfont, cus_point2, &txt_color_brush);

				mygraphics->DrawLine(myRectangle_pen4, cus_point1.X, cus_point1.Y, cus_point2.X, cus_point2.Y);
			}
		}
		PointF value_position;
		if (product_register_value[NEW_MODBUS_DEF_CUSTOMER_TEMP] == 0)
		{
			if (temp_temperature > default_max_pascal)
				value_position.X = X_AXIS_MAX_VALUE_X;
			else
				value_position.X = ORIGIN_X + (X_AXIS_MAX_VALUE_X - ORIGIN_X) * (temp_temperature - default_min_pascal) / (default_max_pascal - default_min_pascal);
			//value_position.X = ORIGIN_X + (default_max_pascal) * product_register_value[MODBUS_DIFF_PRESSURE_VALUE] / (X_AXIS_MAX_VALUE_X - ORIGIN_X);
			if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0) //4- 20 ma
			{
				//value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[MODBUS_OUTPUT_CUR] / 200;
				value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * (product_register_value[NEW_MODBUS_OUTPUT_CUR_TEMP] - 40) / (200 - 40);
			}
			else
			{
				value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_OUTPUT_VOL_TEMP] / 100;
			}
		}
		else
		{
			if (product_register_value[NEW_MODBUS_TEMP_MAX] == product_register_value[NEW_MODBUS_TEMP_MIN])
			{

			}
			else
			{
				if ((short)temp_temperature < (short)product_register_value[NEW_MODBUS_TEMP_MIN])
					value_position.X = ORIGIN_X;
				else if ((short)temp_temperature > (short)product_register_value[NEW_MODBUS_TEMP_MAX])
					value_position.X = X_AXIS_MAX_VALUE_X;
				else
					value_position.X = ORIGIN_X + (X_AXIS_MAX_VALUE_X - ORIGIN_X) * ((short)temp_temperature - (short)product_register_value[NEW_MODBUS_TEMP_MIN]) / ((short)product_register_value[NEW_MODBUS_TEMP_MAX] - (short)product_register_value[NEW_MODBUS_TEMP_MIN]);
				if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0) //4- 20 ma
				{
					value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_OUTPUT_CUR_TEMP] / 200;
				}
				else
				{
					value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_OUTPUT_VOL_TEMP] / 100;
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
		if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0) //4- 20 ma
		{
			if (product_register_value[NEW_MODBUS_TEMPERATURE_UNIT] == 0)
				temp_value.Format(_T("(%.1f Deg.C, %.1f ma)"), (short)temp_temperature /10.0, product_register_value[NEW_MODBUS_OUTPUT_CUR_TEMP] / 10.0);
			else
				temp_value.Format(_T("(%.1f Deg.F, %.1f ma)"), (short)temp_temperature / 10.0, product_register_value[NEW_MODBUS_OUTPUT_CUR_TEMP] / 10.0);
		}
		else
		{
			if (product_register_value[NEW_MODBUS_TEMPERATURE_UNIT] == 0)
				temp_value.Format(_T("(%.1f Deg.C, %.1f V)"), (short)temp_temperature /10.0, product_register_value[NEW_MODBUS_OUTPUT_VOL_TEMP] / 10.0);
			else
				temp_value.Format(_T("(%.1f Deg.F, %.1f V)"), (short)temp_temperature / 10.0, product_register_value[NEW_MODBUS_OUTPUT_VOL_TEMP] / 10.0);
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
#pragma endregion


#pragma region for_hum
		if (product_register_value[NEW_MODBUS_DEF_CUSTOMER_HUM] == 0)
		{
			pointF.X = ORIGIN_X;
			pointF.Y = ORIGIN_Y + 30;
			if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_DEWPOINT)
			{
				cs_show_info3 = _T("-20 ");
				cs_show_info4 = _T("80 ");
				default_min_pascal = -200;
				default_max_pascal = 800;
			}
			else if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_ENTHALPY)
			{
				cs_show_info3 = _T("0 ");
				cs_show_info4 = _T("85 ");
				default_min_pascal = 0;
				default_max_pascal = 850;
			}
			else if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_ABSOLUTE)
			{
				cs_show_info3 = _T("0 ");
				cs_show_info4 = _T("50 Kg/m3");
				default_min_pascal = 0;
				default_max_pascal = 500;
			}
			else if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_REAL)
			{
				cs_show_info3 = _T("0 ");
				cs_show_info4 = _T("100 %");
				default_min_pascal = 0;
				default_max_pascal = 1000;
			}
		}
		else
		{
			cs_show_info3.Format(_T("%d"), product_register_value[NEW_MODBUS_HUM_MIN]);
			pointF.X = ORIGIN_X;
			pointF.Y = ORIGIN_Y + 30;

			CString temp;
			temp.Format(_T("%.1f"), product_register_value[NEW_MODBUS_HUM_MAX] / 10.0);
			cs_show_info4 = temp ;
			default_min_pascal = product_register_value[NEW_MODBUS_HUM_MIN];
			default_max_pascal = product_register_value[NEW_MODBUS_HUM_MAX];

		}
		SolidBrush  hun_txt_color_brush(Color(255, 255, 180, 0));
		mygraphics->DrawString(cs_show_info3, -1, &unitfont, pointF, &hun_txt_color_brush);

		pointF.X = X_AXIS_RIGHT_X - 100;
		pointF.Y = ORIGIN_Y + 30;
		mygraphics->DrawString(cs_show_info4, -1, &unitfont, pointF, &hun_txt_color_brush);





		if (product_register_value[NEW_MODBUS_DEF_CUSTOMER_HUM] == 0)
		{
			mygraphics->DrawLine(myRectangle_pen_hun, ORIGIN_X, ORIGIN_Y, X_AXIS_MAX_VALUE_X, Y_AXIS_MAX_VALUE_Y);
		}
		else
		{
			PointF      cus_point1;
			PointF      cus_point2;
			CString first_point_y_value;
			CString second_point_y_value;

			if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0) //4-20ma
			{
				cus_point1.X = ORIGIN_X;
				cus_point1.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_CURRENT_MIN_HUM] / 200;

				first_point_y_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_CURRENT_MIN_HUM] / 10.0);
				second_point_y_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_CURRENT_MAX_HUM] / 10.0);
				mygraphics->DrawString(first_point_y_value, -1, &unitfont, cus_point1, &txt_color_brush);

				cus_point2.X = X_AXIS_MAX_VALUE_X;
				cus_point2.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_CURRENT_MAX_HUM] / 200;
				mygraphics->DrawString(second_point_y_value, -1, &unitfont, cus_point2, &txt_color_brush);

				mygraphics->DrawLine(myRectangle_pen_hun, cus_point1.X, cus_point1.Y, cus_point2.X, cus_point2.Y);
			}
			else if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 1) // 0 - 10 V
			{
				cus_point1.X = ORIGIN_X;// ORIGIN_X - (X_AXIS_MAX_VALUE_X - ORIGIN_X) / (product_register_value[MODBUS_PASCAL_MAX] - product_register_value[MODBUS_PASCAL_MIN]) * product_register_value[MODBUS_PASCAL_MIN];
				cus_point1.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_VOLTAGE_MIN_HUM] / 100;

				first_point_y_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_VOLTAGE_MIN_HUM] / 10.0);
				second_point_y_value.Format(_T("%.1f"), product_register_value[NEW_MODBUS_VOLTAGE_MAX_HUM] / 10.0);
				mygraphics->DrawString(first_point_y_value, -1, &unitfont, cus_point1, &txt_color_brush);

				cus_point2.X = X_AXIS_MAX_VALUE_X;
				cus_point2.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_VOLTAGE_MAX_HUM] / 100;
				mygraphics->DrawString(second_point_y_value, -1, &unitfont, cus_point2, &txt_color_brush);

				mygraphics->DrawLine(myRectangle_pen_hun, cus_point1.X, cus_point1.Y, cus_point2.X, cus_point2.Y);
			}
		}
		int n_value = 0;
		if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_DEWPOINT)
			n_value = product_register_value[NEW_MODBUS_DEWPOINT];
		else if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_ENTHALPY)
			n_value = product_register_value[NEW_MODBUS_ENTHALPY];
		else if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_ABSOLUTE)
			n_value = product_register_value[NEW_MODBUS_ABSOLUTE_HUMI];
		else if (product_register_value[NEW_MODBUS_SWITCH_HUMI_RANGE] == HUM_REAL)
			n_value = product_register_value[NEW_MODBUS_HUMIDITY];
		if (product_register_value[NEW_MODBUS_DEF_CUSTOMER_HUM] == 0)
		{
			if (n_value > default_max_pascal)
				value_position.X = X_AXIS_MAX_VALUE_X;
			else
				value_position.X = ORIGIN_X + (X_AXIS_MAX_VALUE_X - ORIGIN_X) * (n_value - default_min_pascal) / (default_max_pascal - default_min_pascal);
			//value_position.X = ORIGIN_X + (default_max_pascal) * product_register_value[MODBUS_DIFF_PRESSURE_VALUE] / (X_AXIS_MAX_VALUE_X - ORIGIN_X);
			if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0) //4- 20 ma
			{
				//value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[MODBUS_OUTPUT_CUR] / 200;
				value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * (product_register_value[NEW_MODBUS_OUTPUT_CUR_HUM] - 40) / (200 - 40);
			}
			else
			{
				value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_OUTPUT_VOL_HUM] / 100;
			}
		}
		else
		{
			if (product_register_value[NEW_MODBUS_HUM_MAX] == product_register_value[NEW_MODBUS_HUM_MIN])
			{

			}
			else
			{
				if (n_value < product_register_value[NEW_MODBUS_HUM_MIN])
					value_position.X = ORIGIN_X;
				else if (n_value > product_register_value[NEW_MODBUS_HUM_MAX])
					value_position.X = X_AXIS_MAX_VALUE_X;
				else
					value_position.X = ORIGIN_X + (X_AXIS_MAX_VALUE_X - ORIGIN_X) * (n_value - product_register_value[NEW_MODBUS_HUM_MIN]) / (product_register_value[NEW_MODBUS_HUM_MAX] - product_register_value[NEW_MODBUS_HUM_MIN]);
				if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0) //4- 20 ma
				{
					value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_OUTPUT_CUR_HUM] / 200;
				}
				else
				{
					value_position.Y = ORIGIN_Y - (ORIGIN_Y - Y_AXIS_MAX_VALUE_Y) * product_register_value[NEW_MODBUS_OUTPUT_VOL_HUM] / 100;
				}
			}
		}
		//Rect PointRect;

		PointRect.X = value_position.X - 1;
		PointRect.Y = value_position.Y - 1;
		PointRect.Width = 3;
		PointRect.Height = 3;
		myRectangle_pen5 = new Pen(Color(255, 255, 0, 0), 3);
		mygraphics->DrawRectangle(myRectangle_pen5, PointRect);
		//CString temp_value;
		if (product_register_value[NEW_MODBUS_SWITCH_OUTPUT_MODE] == 0) //4- 20 ma
		{
			temp_value.Format(_T("(%.1f , %.1f ma)"), n_value / 10.0, product_register_value[NEW_MODBUS_OUTPUT_CUR_HUM] / 10.0);
		}
		else
		{
			temp_value.Format(_T("(%.1f , %.1f V)"), n_value / 10.0, product_register_value[NEW_MODBUS_OUTPUT_VOL_HUM] / 10.0);
		}
	
		if (PointRect.X < ORIGIN_X + 50)
		{
			temppoint_value.X = PointRect.X + 50;
			temppoint_value.Y = PointRect.Y - 20;
		}
		else
		{
			temppoint_value.X = PointRect.X - 60;
			temppoint_value.Y = PointRect.Y - 20;
		}
		mygraphics->DrawString(temp_value, -1, &Valueunitfont, temppoint_value, &hun_txt_color_brush);

		PointF      point_line1;
		point_line1.X = ORIGIN_X + 100; //
		point_line1.Y = CANVAS_BOTTOM_Y - 20;
		PointF      point_line2;
		point_line2.X = ORIGIN_X + 150;
		point_line2.Y = CANVAS_BOTTOM_Y - 20;
		mygraphics->DrawLine(myRectangle_pen, point_line1.X, point_line1.Y, point_line2.X, point_line2.Y);
		temp_value = _T("Temperature");
		temppoint_value.X = point_line2.X + 10;
		temppoint_value.Y = point_line2.Y - 10;
		mygraphics->DrawString(temp_value, -1, &Valueunitfont, temppoint_value, &txt_color_brush);

		point_line1.X = ORIGIN_X + 300;//
		point_line1.Y = CANVAS_BOTTOM_Y - 20;
		point_line2.X = ORIGIN_X + 350;
		point_line2.Y = CANVAS_BOTTOM_Y - 20;
		mygraphics->DrawLine(myRectangle_pen_hun, point_line1.X, point_line1.Y, point_line2.X, point_line2.Y);
		temp_value = _T("Humidity");
		temppoint_value.X = point_line2.X + 10;
		temppoint_value.Y = point_line2.Y - 10;
		mygraphics->DrawString(temp_value, -1, &Valueunitfont, temppoint_value, &hun_txt_color_brush);
#pragma endregion



		delete myRectangle_pen_hun;
		delete myRectangle_pen;
		delete mygraphics;
		delete myRectangle_pen3;
		delete myRectangle_pen4;
		delete myRectangle_pen5;
	}
}


void CTransducer::OnBnClickedRadioDefaultT()
{
	// TODO: 
	if (product_register_value[NEW_MODBUS_DEF_CUSTOMER_TEMP] == 0)
		return;
	product_register_value[NEW_MODBUS_DEF_CUSTOMER_TEMP] = 0;
	unsigned int temp_value = 0;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_DEF_CUSTOMER_TEMP, temp_value,
		product_register_value[NEW_MODBUS_DEF_CUSTOMER_TEMP], this->m_hWnd, IDC_RADIO_TRANSDUCER_DEG_C, _T(" Temperature Output  "));
}


void CTransducer::OnBnClickedRadioUserDefinedT()
{
	// TODO: 
	if (product_register_value[NEW_MODBUS_DEF_CUSTOMER_TEMP] == 1)
		return;
	product_register_value[NEW_MODBUS_DEF_CUSTOMER_TEMP] = 1;
	unsigned int temp_value = 1;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_DEF_CUSTOMER_TEMP, temp_value,
		product_register_value[NEW_MODBUS_DEF_CUSTOMER_TEMP], this->m_hWnd, IDC_RADIO_TRANSDUCER_DEG_C, _T(" Temperature Output  "));
}


void CTransducer::OnBnClickedRadioDefaultH()
{
	// TODO: 
	if (product_register_value[NEW_MODBUS_DEF_CUSTOMER_HUM] == 0)
		return;
	product_register_value[NEW_MODBUS_DEF_CUSTOMER_HUM] = 0;
	unsigned int temp_value = 0;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_DEF_CUSTOMER_HUM, temp_value,
		product_register_value[NEW_MODBUS_DEF_CUSTOMER_HUM], this->m_hWnd, IDC_RADIO_TRANSDUCER_DEG_C, _T(" Hum Output  "));
}


void CTransducer::OnBnClickedRadioUserDefinedH()
{
	// TODO: 
	if (product_register_value[NEW_MODBUS_DEF_CUSTOMER_HUM] == 1)
		return;
	product_register_value[NEW_MODBUS_DEF_CUSTOMER_HUM] = 1;
	unsigned int temp_value = 1;

	Post_Thread_Message(MY_WRITE_ONE, g_tstat_id, NEW_MODBUS_DEF_CUSTOMER_HUM, temp_value,
		product_register_value[NEW_MODBUS_DEF_CUSTOMER_HUM], this->m_hWnd, IDC_RADIO_TRANSDUCER_DEG_C, _T(" Hum Output  "));
}


void CTransducer::OnEnKillfocusEditVolatgeMinT()
{
	// TODO: 
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_VOLATGE_MIN_T, temp_cstring_min);
	unsigned int temp_voltage_value_min = unsigned int(_wtof(temp_cstring_min) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_VOLATGE_MAX_T, temp_cstring_max);
	unsigned int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((temp_voltage_value_min < 0) || (temp_voltage_value_min > 100) || (temp_voltage_value_min >= temp_value_max))
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between 0 and 10 "));
		return;
	}

	if (write_one(g_tstat_id, NEW_MODBUS_VOLTAGE_MIN_TEMP, temp_voltage_value_min) > 0)
	{
		product_register_value[NEW_MODBUS_VOLTAGE_MIN_TEMP] = temp_voltage_value_min;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CTransducer::OnEnKillfocusEditVolatgeMaxT()
{
	// TODO: 
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_VOLATGE_MIN_T, temp_cstring_min);
	unsigned int temp_voltage_value_min = unsigned int(_wtof(temp_cstring_min) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_VOLATGE_MAX_T, temp_cstring_max);
	unsigned int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((temp_value_max < 0) || (temp_value_max > 100) || (temp_voltage_value_min >= temp_value_max))
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between 0 and 10 "));
		return;
	}

	if (write_one(g_tstat_id, NEW_MODBUS_VOLTAGE_MAX_TEMP, temp_value_max) > 0)
	{
		product_register_value[NEW_MODBUS_VOLTAGE_MAX_TEMP] = temp_value_max;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CTransducer::OnEnKillfocusEditCurrentMinT()
{
	// TODO: 
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_CURRENT_MIN_T, temp_cstring_min);
	unsigned int temp_current_value_min = unsigned int(_wtof(temp_cstring_min) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_CURRENT_MAX_T, temp_cstring_max);
	unsigned int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((temp_current_value_min < 40) || (temp_current_value_min > 200) || (temp_current_value_min >= temp_value_max))
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between 4 and 20 "));
		return;
	}

	if (write_one(g_tstat_id, NEW_MODBUS_CURRENT_MIN_TEMP, temp_current_value_min) > 0)
	{
		product_register_value[NEW_MODBUS_CURRENT_MIN_TEMP] = temp_current_value_min;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CTransducer::OnEnKillfocusEditCurrentMaxT()
{
	// TODO: 
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_CURRENT_MIN_T, temp_cstring_min);
	unsigned int temp_current_value_min = unsigned int(_wtof(temp_cstring_min) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_CURRENT_MAX_T, temp_cstring_max);
	unsigned int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((temp_value_max < 40) || (temp_value_max > 200) || (temp_current_value_min >= temp_value_max))
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between 4 and 20 "));
		return;
	}

	if (write_one(g_tstat_id, NEW_MODBUS_CURRENT_MAX_TEMP, temp_value_max) > 0)
	{
		product_register_value[NEW_MODBUS_CURRENT_MAX_TEMP] = temp_value_max;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CTransducer::OnEnKillfocusEditMinT()
{
	// TODO: 
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_MIN_T, temp_cstring_min);
	//unsigned short temp_temperature_value_min = unsigned int(_wtof(temp_cstring_min) * 10);
	unsigned short temp_temperature_value_min = _wtof(temp_cstring_min) * 10;
	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_MAX_T, temp_cstring_max);
	int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((temp_temperature_value_min < 0xfe0b) && (temp_temperature_value_min > 1000))
	{
		MessageBox(_T("Minimum and maximum values must be between -50 Deg.C and 100 Deg.C"));
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between -50 Deg.C and 100 Deg.C"));
		return;
	}

	//if (((temp_temperature_value_min <= 0xffff) && ((temp_temperature_value_min > 0xfe0b))) ||
	//	((temp_temperature_value_min > 1000) && (temp_temperature_value_min < 0x7fff)) || (temp_temperature_value_min >= temp_value_max))
	//{
	//	SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between -50 and 100 "));
	//	return;
	//}

	if (write_one(g_tstat_id, NEW_MODBUS_TEMP_MIN, temp_temperature_value_min) > 0)
	{
		product_register_value[NEW_MODBUS_TEMP_MIN] = temp_temperature_value_min;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
	GetDlgItem(IDC_BUTTON_DONE)->SetFocus();
}


void CTransducer::OnEnKillfocusEditMaxT()
{
	// TODO: 
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_MIN_T, temp_cstring_min);
	unsigned short temp_temperature_value_min = _wtof(temp_cstring_min) * 10;

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_MAX_T, temp_cstring_max);
	unsigned short temp_value_max = unsigned short(_wtof(temp_cstring_max) * 10);

	if ((temp_temperature_value_min < 0xfe0b) && (temp_temperature_value_min > 1000) ||
		 ((short)temp_temperature_value_min >= (short)temp_value_max))
	{
		MessageBox(_T("Minimum and maximum values must be between -50 Deg.C and 100 Deg.C"));
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between -50 Deg.C and 100 Deg.C"));
		return;
	}

	//if ((temp_value_max < -500) || (temp_value_max > 1000) || (temp_temperature_value_min >= temp_value_max))
	//{
	//	SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between -50 and 100 "));
	//	return;
	//}

	if (write_one(g_tstat_id, NEW_MODBUS_TEMP_MAX, temp_value_max) > 0)
	{
		product_register_value[NEW_MODBUS_TEMP_MAX] = temp_value_max;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
	GetDlgItem(IDC_BUTTON_DONE)->SetFocus();
}


void CTransducer::OnEnKillfocusEditVolatgeMinH()
{
	// TODO: 
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_VOLATGE_MIN_H, temp_cstring_min);
	int hum_voltage_value_min = unsigned int(_wtof(temp_cstring_min) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_VOLATGE_MAX_H, temp_cstring_max);
	int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((hum_voltage_value_min < 0) || (hum_voltage_value_min > 1000) || (hum_voltage_value_min >= temp_value_max))
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between 0 and 10 "));
		return;
	}

	if (write_one(g_tstat_id, NEW_MODBUS_VOLTAGE_MIN_HUM, hum_voltage_value_min) > 0)
	{
		product_register_value[NEW_MODBUS_VOLTAGE_MIN_HUM] = hum_voltage_value_min;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CTransducer::OnEnKillfocusEditVolatgeMaxH()
{
	// TODO: 
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_VOLATGE_MIN_H, temp_cstring_min);
	int hum_voltage_value_min = unsigned int(_wtof(temp_cstring_min) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_VOLATGE_MAX_H, temp_cstring_max);
	int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((temp_value_max < 0) || (temp_value_max > 1000) || (hum_voltage_value_min >= temp_value_max))
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between 0 and 10 "));
		return;
	}

	if (write_one(g_tstat_id, NEW_MODBUS_VOLTAGE_MAX_HUM, temp_value_max) > 0)
	{
		product_register_value[NEW_MODBUS_VOLTAGE_MAX_HUM] = temp_value_max;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CTransducer::OnEnKillfocusEditCurrentMinH()
{
	// TODO: 
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_CURRENT_MIN_H, temp_cstring_min);
	int hum_c_value_min = unsigned int(_wtof(temp_cstring_min) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_CURRENT_MAX_H, temp_cstring_max);
	int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((hum_c_value_min < 40) || (hum_c_value_min > 200) || (hum_c_value_min >= temp_value_max))
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between 4 and 20 "));
		return;
	}

	if (write_one(g_tstat_id, NEW_MODBUS_CURRENT_MIN_HUM, hum_c_value_min) > 0)
	{
		product_register_value[NEW_MODBUS_CURRENT_MIN_HUM] = hum_c_value_min;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CTransducer::OnEnKillfocusEditCurrentMaxH()
{
	// TODO: 
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_CURRENT_MIN_H, temp_cstring_min);
	int hum_c_value_min = unsigned int(_wtof(temp_cstring_min) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_CURRENT_MAX_H, temp_cstring_max);
	int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if ((temp_value_max < 40) || (temp_value_max > 200) || (hum_c_value_min >= temp_value_max))
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values must be between 4 and 20 "));
		return;
	}

	if (write_one(g_tstat_id, NEW_MODBUS_CURRENT_MAX_HUM, temp_value_max) > 0)
	{
		product_register_value[NEW_MODBUS_CURRENT_MAX_HUM] = temp_value_max;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CTransducer::OnEnKillfocusEditMinH()
{
	// TODO: 
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_MIN_H, temp_cstring_min);
	int hum_value_min = unsigned int(_wtof(temp_cstring_min) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_MAX_H, temp_cstring_max);
	int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if (hum_value_min >= temp_value_max)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values error "));
		return;
	}

	if (write_one(g_tstat_id, NEW_MODBUS_HUM_MIN, hum_value_min) > 0)
	{
		product_register_value[NEW_MODBUS_HUM_MIN] = hum_value_min;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


void CTransducer::OnEnKillfocusEditMaxH()
{
	// TODO: 
	CString temp_cstring_min;
	GetDlgItemTextW(IDC_EDIT_MIN_H, temp_cstring_min);
	int hum_value_min = unsigned int(_wtof(temp_cstring_min) * 10);

	CString temp_cstring_max;
	GetDlgItemTextW(IDC_EDIT_MAX_H, temp_cstring_max);
	int temp_value_max = unsigned int(_wtof(temp_cstring_max) * 10);

	if (hum_value_min >= temp_value_max)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Minimum and maximum values error "));
		return;
	}

	if (write_one(g_tstat_id, NEW_MODBUS_HUM_MAX, temp_value_max) > 0)
	{
		product_register_value[NEW_MODBUS_HUM_MAX] = temp_value_max;
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data success"));
	}
	else
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Write data Timeout"));
	}
}


BOOL CTransducer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			CWnd* temp_focus = GetFocus();
			GetDlgItem(IDC_BUTTON_DONE)->SetFocus();
			temp_focus->SetFocus();
			return 1;
		}
	}
	return CFormView::PreTranslateMessage(pMsg);
}
