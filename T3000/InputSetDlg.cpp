// InputSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "InputSetDlg.h"
#include "globle_function.h"
#include "BuildTable1.h"
#define INDEX_FIELD 0
#define NAME_FIELD 1
#define VALUE_FIELD 2
#define AM_FIELD 3
#define CAL_FIELD 4
#define FILTER 5
#define RANG_FIELD 6
#define FUN_FIELD 7
#define CUST_FIELD 8

#define TOTAL_COLS 8+1
#define TOTAL_ROWS 7
// CInputSetDlg dialog


#define ANALOG_RANG_NUMBER 6
// CString analog_range_0[ANALOG_RANG_NUMBER]={_T("Raw"),_T("10KC Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};
// CString analog_range_1[ANALOG_RANG_NUMBER]={_T("Raw"),_T("10KF Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};
CString analog_range_0[ANALOG_RANG_NUMBER]={_T("UNUSED"),_T("10KC Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};
CString analog_range_1[ANALOG_RANG_NUMBER]={_T("UNUSED"),_T("10KF Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};
CString INPUT_FUNS[8]={_T("Normal"),_T("Freeze Protect"),_T("Occupancy Sensor"),_T("Sweep Off"),_T("Clock"),_T("Changeover Mode"),_T("Outside Temp"),_T("Airflow")};

//CString g_str5ERange2[5]={_T("Raw"),_T("Thermistor"),_T("On/Off"),_T("Off/On"),_T("%")};
IMPLEMENT_DYNAMIC(CInputSetDlg, CDialog)

CInputSetDlg::CInputSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputSetDlg::IDD, pParent)
	, m_filterValue(0)
	//, m_filterValue(0)
{
	m_nCurRow=m_nCurCol=0;
	 InputThread=NULL;
	 b_is_fresh = false;

	 
}

CInputSetDlg::~CInputSetDlg()
{
	if(InputThread!=NULL)
	{
		TerminateThread(InputThread, 0);
	}
	InputThread = NULL;
	b_is_fresh = false;
}

void CInputSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	DDX_Control(pDX, IDC_RANG_COMBO, m_RangCombox);
	DDX_Control(pDX, IDC_UPBUTTON, m_upButton);
	DDX_Control(pDX, IDC_DOWNBUTTON, m_downButton);
	DDX_Control(pDX, IDC_AMCOMBO, m_AmCombox);
	DDX_Control(pDX, IDC_INPUTFUNCOMBO, m_inputFinCombox);
	DDX_Control(pDX, IDC_CUSTOMBUTTON, m_customBtn);
	DDX_Control(pDX, IDC_INVALUEEDIT, m_inValueEdit);
	DDX_Control(pDX, IDC_VALUECOMBO, m_valueCombx);
	DDX_Control(pDX, IDC_INPUTNAMEEDIT, m_inputNameEdt);
	DDX_Control(pDX, IDC_FILTER, m_Filter);
	DDX_Text(pDX, IDC_FILTER, m_filterValue);
	DDV_MinMaxUInt(pDX, m_filterValue, 0, 100);
	DDX_Control(pDX, IDC_FILTER, m_Filter);
	DDX_Text(pDX, IDC_FILTER, m_filterValue);
}


BEGIN_MESSAGE_MAP(CInputSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CInputSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CInputSetDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_RANG_COMBO, &CInputSetDlg::OnCbnSelchangeRangCombo)
	ON_CBN_KILLFOCUS(IDC_RANG_COMBO, &CInputSetDlg::OnCbnKillfocusRangCombo)
	ON_BN_CLICKED(IDC_UPBUTTON, &CInputSetDlg::OnBnClickedUpbutton)
	ON_BN_CLICKED(IDC_DOWNBUTTON, &CInputSetDlg::OnBnClickedDownbutton)
	ON_CBN_KILLFOCUS(IDC_AMCOMBO, &CInputSetDlg::OnCbnKillfocusAmcombo)
 	ON_CBN_SELCHANGE(IDC_AMCOMBO, &CInputSetDlg::OnCbnSelchangeAmcombo)
	ON_CBN_SELCHANGE(IDC_INPUTFUNCOMBO, &CInputSetDlg::OnCbnSelchangeInputfuncombo)
	ON_CBN_KILLFOCUS(IDC_INPUTFUNCOMBO, &CInputSetDlg::OnCbnKillfocusInputfuncombo)
	ON_BN_CLICKED(IDC_CUSTOMBUTTON, &CInputSetDlg::OnBnClickedCustombutton)
	ON_BN_KILLFOCUS(IDC_CUSTOMBUTTON, &CInputSetDlg::OnBnKillfocusCustombutton)
	ON_BN_CLICKED(IDEXIT, &CInputSetDlg::OnBnClickedExit)
	ON_EN_KILLFOCUS(IDC_INVALUEEDIT, &CInputSetDlg::OnEnKillfocusInvalueedit)
	ON_CBN_KILLFOCUS(IDC_VALUECOMBO, &CInputSetDlg::OnCbnKillfocusValuecombo)
	ON_BN_CLICKED(IDC_REFRESHBUTTON, &CInputSetDlg::OnBnClickedRefreshbutton)
	ON_EN_KILLFOCUS(IDC_INPUTNAMEEDIT, &CInputSetDlg::OnEnKillfocusInputnameedit)
	ON_MESSAGE(WM_REFRESH_INPUTDLG, &CInputSetDlg::DealMessage)//Add by Fan
	ON_WM_CLOSE()
	ON_EN_KILLFOCUS(IDC_FILTER, &CInputSetDlg::OnEnKillfocusFilter)
END_MESSAGE_MAP()

// CInputSetDlg message handlers
const int REFRESH_GRID=1;
const int ENABLE_REFRESH_BUTTON=2;
LRESULT  CInputSetDlg::DealMessage(WPARAM wParam,LPARAM lParam)
{
	int command = wParam;
	switch(command)
	{
	case REFRESH_GRID:
		Fresh_Grid();
		break;
	case ENABLE_REFRESH_BUTTON:
		GetDlgItem(IDC_REFRESHBUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDEXIT)->EnableWindow(TRUE);
		break;
	default:
		break;
	}

	return 0;
}




void CInputSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

//Add by Fan  2013 04 01
DWORD WINAPI CInputSetDlg::StartRefresh(LPVOID lpVoid)
{
	CInputSetDlg *pParent = (CInputSetDlg *)lpVoid;
	int i;
	
	register_critical_section.Lock();
	pParent->b_is_fresh = true;
	for( i=0;i<16;i++)
	{
		Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64);
	}
	register_critical_section.Unlock();
	memset(product_register_value,0,sizeof(product_register_value));
	memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));//
	//if ((multi_register_value[7] == 6)||(multi_register_value[7] == 7))//tstat6
	//{
	//	//multi_register_value[]列表交换。
	//	memset(pParent->tempchange,0,sizeof(pParent->tempchange));
	//	int index = 0;

	//	for (int i = 0;i<512;i++)
	//	{
	//		index = reg_tstat6[i];
	//		pParent->tempchange[index] = multi_register_value[i];
	//	}
	//	memcpy_s(multi_register_value,sizeof(multi_register_value),pParent->tempchange,sizeof(pParent->tempchange));
	//}

	pParent->PostMessage(WM_REFRESH_INPUTDLG,REFRESH_GRID,0);
	pParent->PostMessage(WM_REFRESH_INPUTDLG,ENABLE_REFRESH_BUTTON,0);
	pParent->b_is_fresh = false;
	pParent->InputThread=NULL;
	return 0;
}


BOOL CInputSetDlg::OnInitDialog()
{
	int m_inRows=0x00;
	CString RowTitle;
	CDialog::OnInitDialog();
	m_inputNameEdt.ShowWindow(SW_HIDE);
	m_inValueEdit.ShowWindow(SW_HIDE);
	m_valueCombx.ShowWindow(SW_HIDE);

	m_RangCombox.ShowWindow(SW_HIDE);
	m_upButton.SetImage(IDB_UPBMP);
	m_downButton.SetImage(IDB_DOWNBMP);
	m_upButton.SetWindowText(_T(""));
	m_downButton.SetWindowText(_T(""));
	m_upButton.SizeToContent();
	m_downButton.SizeToContent();
	m_upButton.Invalidate();
	m_downButton.Invalidate();
	m_downButton.ShowWindow(SW_HIDE);
	m_upButton.ShowWindow(SW_HIDE);
	m_AmCombox.ShowWindow(SW_HIDE);
	m_inputFinCombox.ShowWindow(SW_HIDE);
	m_customBtn.ShowWindow(SW_HIDE);

//	m_FlexGrid.CellAlignment(1)=flexAlignCenterCenter;
	m_nModel=product_register_value[MODBUS_PRODUCT_MODEL];
	switch (m_nModel)
	{
		case 3:m_inRows=4;break; // 5B
		case 2:
		case 1:m_inRows=3;break; // 5A
		case 4:m_inRows=5;break; // 5C
		case PM_TSTAT7:
		case PM_PRESSURE:
		case 12:m_inRows=5;break; // 5D 同 TStat7
		case PM_TSTAT6:	m_inRows=12;break;
		case 16:m_inRows=10;break; // 5E
		case 17:m_inRows=5;break; // 5F
		case 18:m_inRows=5;break; // 5G
		case 19:m_inRows=9;break; // 5H
		default:
			break;
	}
	if (m_inRows == 0)
	{
		AfxMessageBox(_T("Please make sure you select the correct model!"), MB_ICONWARNING);
		return FALSE;
	}


	m_FlexGrid.put_Rows(m_inRows);

	m_FlexGrid.put_Cols(TOTAL_COLS);
	//m_FlexGrid.put_Rows(TOTAL_ROWS);
	m_FlexGrid.put_TextMatrix(0,INDEX_FIELD,_T(""));
	m_FlexGrid.put_ColWidth(INDEX_FIELD,400);

	m_FlexGrid.put_TextMatrix(0,NAME_FIELD,_T("Input Name"));
	m_FlexGrid.put_ColWidth(NAME_FIELD,1300);

	m_FlexGrid.put_TextMatrix(0,VALUE_FIELD,_T("Value"));
	m_FlexGrid.put_ColWidth(VALUE_FIELD, 800);
	
	m_FlexGrid.put_TextMatrix(0,AM_FIELD,_T("Auto/Man"));
	m_FlexGrid.put_ColWidth(AM_FIELD,1000);

	m_FlexGrid.put_TextMatrix(0,CAL_FIELD,_T("Calibration"));
	m_FlexGrid.put_ColWidth(CAL_FIELD,750);	
	if ((product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7))
	{
		m_FlexGrid.put_TextMatrix(0,FILTER,_T("Filter"));
		m_FlexGrid.put_ColWidth(FILTER,750);
	} 
	else
	{
		m_FlexGrid.put_TextMatrix(0,FILTER,_T("Filter"));
		m_FlexGrid.put_ColWidth(FILTER,750);
	    m_FlexGrid.put_ColWidth(FILTER,0);
	}
		
	

	m_FlexGrid.put_TextMatrix(0,RANG_FIELD,_T("Range"));
	m_FlexGrid.put_ColWidth(RANG_FIELD,1200);

	m_FlexGrid.put_TextMatrix(0,FUN_FIELD,_T("Function"));
	m_FlexGrid.put_ColWidth(FUN_FIELD,1400);

	m_FlexGrid.put_TextMatrix(0,CUST_FIELD,_T("Custom Tables"));
	m_FlexGrid.put_ColWidth(CUST_FIELD,1100);

	/*
	m_FlexGrid.put_TextMatrix(1,0,_T("Internal"));
	m_FlexGrid.put_TextMatrix(2,0,_T("Input 1"));
	m_FlexGrid.put_TextMatrix(3,0,_T("Input 2"));
	m_FlexGrid.put_TextMatrix(4,0,_T("Input 3"));
	m_FlexGrid.put_TextMatrix(5,0,_T("Input 4"));
	m_FlexGrid.put_TextMatrix(6,0,_T("Input 5"));
	*/
	/*m_FlexGrid.put_TextMatrix(1,0,g_strInName1);
	m_FlexGrid.put_TextMatrix(2,0,g_strInName2);
	m_FlexGrid.put_TextMatrix(3,0,g_strInName3);
	m_FlexGrid.put_TextMatrix(4,0,g_strInName4);
	m_FlexGrid.put_TextMatrix(5,0,g_strInName5);
	m_FlexGrid.put_TextMatrix(6,0,g_strInName6);*/
	
	//get unit stringlist
	UINT uint_temp=GetOEMCP();//get system is for chinese or english
	if(uint_temp!=936 && uint_temp!=950)
	{

		CString strTemp;
		strTemp.Format(_T("%cC"),176);
		m_strUnitList.push_back(strTemp);
		strTemp.Format(_T("%cF"),176);
		m_strUnitList.push_back(strTemp);
	}
	else//chinese.
	{
		CString strTemp;
		strTemp=_T("°C");
		m_strUnitList.push_back(strTemp);
		strTemp=_T("°F");
		m_strUnitList.push_back(strTemp);
	}

	//Add by Fan,  add a icon to  the button 2013 03 29
	hIcon   = AfxGetApp()->LoadIcon(IDI_ICON_REFRESH);
	((CButton *)GetDlgItem(IDC_REFRESHBUTTON))->SetIcon(hIcon);
	hIcon_Exit = AfxGetApp()->LoadIcon(IDI_ICON_EXIT);
	((CButton *)GetDlgItem(IDEXIT))->SetIcon(hIcon_Exit);

	if(m_nModel == 16)
	{
		Init_not_5ABCD_Grid();
		return true;
	}
	if (m_nModel==PM_TSTAT6)
	{
	   InitGridtstat6();
	   return TRUE;
	}

//	if (m_nModel == 16 || m_nModel == PM_TSTAT6 ) // 5E与其他的不同
	////if (m_nModel == PM_TSTAT6 ) // 5E与其他的不同
	////{
	////	Init5EGrid();
	////	return TRUE;
	////}

	////if (m_nModel == PM_TSTAT5E)
	////{
	////	InitGrid5EnoTSTAT6();
	////	return TRUE;
	////}


	for(int i=1;i<m_inRows;i++)
	{
		CString strIndex;
		strIndex.Format(_T("%d"), i);
		m_FlexGrid.put_TextMatrix(i,INDEX_FIELD,strIndex);
		switch(i)
		{
			case 1:
				RowTitle=g_strSensorName;
				break;
			case 2:
				RowTitle=g_strInName1;
				break;
			case 3:
				RowTitle=g_strInName2;
				break;
			case 4:
				RowTitle=g_strInName3;
				break;
			case 5:
				RowTitle=g_strInName4;
				break;
			case 6:
				RowTitle=g_strInName5;
				break;
		}
		m_FlexGrid.put_TextMatrix(i,1,RowTitle);
		m_FlexGrid.put_RowHeight(i,270);
		for(int k=0;k<TOTAL_COLS;k++)
		{
			m_FlexGrid.put_ColAlignment(k,4);
			if (i%2==1)
			{
				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(COLOR_CELL);
			}
		}
	}
	


	Fresh_Grid();
	// TODO:  Add extra initialization here





	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CInputSetDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
if(pMsg->lParam == WM_CLOSE)
	Sleep(1);
	if(pMsg->wParam == VK_RETURN )
	{ 
		GetDlgItem(IDEXIT)->SetFocus();
		return true; 
	} 
	return CDialog::PreTranslateMessage(pMsg);
}

void CInputSetDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CInputSetDlg::Fresh_Grid()
{
	if(m_nModel == 16)
	{
		Init_not_5ABCD_Grid();
		return ;
	}
	if (m_nModel==PM_TSTAT6)
	{
		InitGridtstat6();
		return ;
	}
	//Merge Init5EGrid InitGrid5EnoTSTAT6 to One function  Init_not_5ABCD_Grid; Fance
	//if (m_nModel == PM_TSTAT6 )
	//{
	//	Init5EGrid();
	//	return;
	//}

	//if (m_nModel == PM_TSTAT5E)
	//{
	//	InitGrid5EnoTSTAT6();
	//	return;
	//}

	CString strUnit=GetTempUnit();
	CString strTemp;	
	int nValue=0;
	CString strAuto=_T("Auto");
	CString strman=_T("Manual");
//row 1:	
	if(m_FlexGrid.get_Row()>=1)
	{
		strTemp.Format(_T("%.1f"),product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);//101
		strTemp=strTemp+strUnit;
		m_FlexGrid.put_TextMatrix(1,VALUE_FIELD,strTemp);
		m_FlexGrid.put_TextMatrix(1,AM_FIELD,NO_APPLICATION);
		m_FlexGrid.put_TextMatrix(1,CAL_FIELD,_T("Adjust..."));
		m_FlexGrid.put_TextMatrix(1,RANG_FIELD,strUnit);
		m_FlexGrid.put_TextMatrix(1,FUN_FIELD,NO_APPLICATION);
		m_FlexGrid.put_TextMatrix(1,CUST_FIELD,NO_APPLICATION);
	}

//row 2:
	strTemp.Empty();
	strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN1], 1);//188

	if(product_register_value[MODBUS_ANALOG_IN1]==4||product_register_value[MODBUS_ANALOG_IN1]==1)//188
	{	
		strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_0]/10);	//180
		strTemp=strTemp+strUnit;
	}
	if(product_register_value[MODBUS_ANALOG_IN1]==0)
	{
// 		strTemp.Format(_T("%d"),multi_register_value[180]);
// 		strTemp=strTemp;
		strTemp=_T("UNUSED");//2.5.0.98
	}
	if(product_register_value[MODBUS_ANALOG_IN1]==2)
	{
		strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_0]);//180
		strTemp=strTemp+_T("%");
	}
	if(product_register_value[MODBUS_ANALOG_IN1]==3 || product_register_value[MODBUS_ANALOG_IN1]==5)
	{
		if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)//180
			strTemp=_T("Off");
		if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)
			strTemp=_T("On");
	}
	if(m_FlexGrid.get_Row()>=2)
	{
		m_FlexGrid.put_TextMatrix(2,VALUE_FIELD,strTemp);
		//int nValue=0;
		//CString strAuto=_T("Auto");
		//CString strman=_T("Manual");
		nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//309
		if((nValue & 0x01)==1)
		{
			m_FlexGrid.put_TextMatrix(2,AM_FIELD,strman);	
			m_FlexGrid.put_Col(VALUE_FIELD);
			//m_FlexGrid.put_Row(2);
			m_FlexGrid.put_CellBackColor(COLOR_CELL);
		}
		else
		{
			m_FlexGrid.put_TextMatrix(2,AM_FIELD,strAuto);
			m_FlexGrid.put_Col(VALUE_FIELD);
			//m_FlexGrid.put_Row(2);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(2,CAL_FIELD,_T("Adjust..."));
		strTemp.Empty();

		nValue=product_register_value[MODBUS_ANALOG_IN1];//188
		strUnit=GetTempUnit(nValue, 2);
		if(nValue>=0)
		{
			//
			if(product_register_value[MODBUS_DEGC_OR_F]==0)//121
			{
				strTemp=analog_range_0[nValue];
			}
			else
			{
				strTemp=analog_range_1[nValue];
			}
			m_FlexGrid.put_TextMatrix(2,RANG_FIELD,strTemp);
		}
		strTemp.Empty();
		nValue=product_register_value[MODBUS_ANALOG1_FUNCTION];//298
		strTemp=INPUT_FUNS[nValue];
		m_FlexGrid.put_TextMatrix(2,FUN_FIELD,strTemp);

		if(product_register_value[MODBUS_ANALOG_IN1]==4)//188
		{
			m_FlexGrid.put_TextMatrix(2,CUST_FIELD,_T("Custom..."));
		}
		else
		{
			m_customBtn.ShowWindow(SW_HIDE);
			m_FlexGrid.put_TextMatrix(2,CUST_FIELD,NO_APPLICATION);
		}
	}
//Row 3:
	if(m_FlexGrid.get_Row()>=3)
	{
		strTemp.Empty();
		if(product_register_value[MODBUS_ANALOG_IN2]==4||product_register_value[MODBUS_ANALOG_IN2]==1)//189
		{
			strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_1]/10);	//181
			strTemp=strTemp+strUnit;
		}
		//if (multi_register_value[189]==0||multi_register_value[189]==2)
		if (product_register_value[MODBUS_ANALOG_IN2]==2)//2.5.0.98		//189
		{
			strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_1]);//181
			strTemp=strTemp+strUnit;
		}
		if(product_register_value[MODBUS_ANALOG_IN2]==3 || product_register_value[MODBUS_ANALOG_IN2]==5)	//189
		{
			if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)//181
				strTemp=_T("Off");
			if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)//181
				strTemp=_T("On");
		}
		if (product_register_value[MODBUS_ANALOG_IN2] == 0)	//189
		{
			strTemp = _T("UNUSED");//2.5.0.98
		}
		m_FlexGrid.put_TextMatrix(3,VALUE_FIELD,strTemp);

		nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//309
		if(((int)(nValue& 0x02)>>1)==1)
		{
			m_FlexGrid.put_TextMatrix(3,AM_FIELD,strman);
			m_FlexGrid.put_Col(VALUE_FIELD);
			//m_FlexGrid.put_Row(3);
			m_FlexGrid.put_CellBackColor(RGB(255,255,255));
		}
		else
		{
			m_FlexGrid.put_TextMatrix(3,AM_FIELD,strAuto);
			m_FlexGrid.put_Col(VALUE_FIELD);
			//m_FlexGrid.put_Row(3);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(3,CAL_FIELD,_T("Adjust..."));
		strTemp.Empty();
		nValue=product_register_value[MODBUS_ANALOG_IN2];	//189
		if(nValue>=0)
		{
			//
			if(product_register_value[MODBUS_DEGC_OR_F]==0)//121
			{
				strTemp=analog_range_0[nValue];
			}
			else
			{
				strTemp=analog_range_1[nValue];
			}
			m_FlexGrid.put_TextMatrix(3,RANG_FIELD,strTemp);
		}
		strTemp.Empty();
		nValue=product_register_value[MODBUS_ANALOG2_FUNCTION];	//299
		strTemp=INPUT_FUNS[nValue];
		m_FlexGrid.put_TextMatrix(3,FUN_FIELD,strTemp);

		if(product_register_value[MODBUS_ANALOG_IN2]==4)	//189
		{
			m_FlexGrid.put_TextMatrix(3,CUST_FIELD,_T("Custom..."));
		}
		else
		{
			m_customBtn.ShowWindow(SW_HIDE);
			m_FlexGrid.put_TextMatrix(3,CUST_FIELD,NO_APPLICATION);
		}
	}
//row 4:
	if(m_FlexGrid.get_Row()>=4)
	{	
		if (product_register_value[MODBUS_DIGITAL_IN1]==0)//190 
		{
			CString strTemp2;
			if (product_register_value[311]==0)
			{
				strTemp=_T("Off");
			}else
			{
				strTemp=_T("On");
			}
			strTemp2=_T("On/Off");
			m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
		}
		if (product_register_value[MODBUS_DIGITAL_IN1]==1)//190
		{
			CString strTemp2;
			if (product_register_value[MODBUS_DIGITAL_INPUT]==1)	//311
			{
				strTemp=_T("Off");

			}else
			{
				strTemp=_T("On");
			}
			strTemp2=_T("Off/On");
			m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
		}
		m_FlexGrid.put_TextMatrix(4,VALUE_FIELD,strTemp);

		nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//309
		if(((int)((nValue & 0x04)>>2)==1))
		{
			m_FlexGrid.put_TextMatrix(4,AM_FIELD,strman);
			m_FlexGrid.put_Col(VALUE_FIELD);
			m_FlexGrid.put_Row(4);
			m_FlexGrid.put_CellBackColor(COLOR_CELL);
		}
		else
		{
			m_FlexGrid.put_TextMatrix(4,AM_FIELD,strAuto);
			m_FlexGrid.put_Col(VALUE_FIELD);
			m_FlexGrid.put_Row(4);
			m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);
		}
		m_FlexGrid.put_TextMatrix(4,CAL_FIELD,NO_APPLICATION);
		
		strTemp.Empty();
		nValue=product_register_value[MODBUS_DIGITAL1_FUNCTION];	//300
		if(nValue>=0)
			strTemp=INPUT_FUNS[nValue];
		m_FlexGrid.put_TextMatrix(4,FUN_FIELD,strTemp);
		m_FlexGrid.put_TextMatrix(4,CUST_FIELD,NO_APPLICATION);
	}
}

BEGIN_EVENTSINK_MAP(CInputSetDlg, CDialog)
	ON_EVENT(CInputSetDlg, IDC_MSFLEXGRID1, DISPID_CLICK, CInputSetDlg::ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void CInputSetDlg::ClickMsflexgrid1()
{
	long lRow,lCol;
	
	lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
	if(lRow>m_FlexGrid.get_Rows()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow == 0) //如果点击标题行，也无效
		return;
	CRect rect;
	m_FlexGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息
	CString strValue = m_FlexGrid.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;
	
	if (m_nModel == PM_TSTAT6)
	{
		OnClickTstat6Grid(m_nCurRow, m_nCurCol, rc);
		return;
	}
	if (m_nModel == PM_TSTAT5E)
	{
		ClickMsflexgrid5E(m_nCurRow, m_nCurCol, rc);
		return;
	}
	
	if(lCol==VALUE_FIELD)
	{
		WORD wAM=multi_register_value[309];
		
		if(lRow==1)
		{
			m_inValueEdit.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_inValueEdit.ShowWindow(SW_SHOW);
			
			m_inValueEdit.BringWindowToTop();
			//m_RangCombox.SelectString(-1,strValue);
			m_inValueEdit.SetWindowText(strValue);
			m_inValueEdit.SetFocus(); //获取焦点
		}
		if(lRow==2 && (wAM & 0x01))
		{
			if(product_register_value[MODBUS_ANALOG_IN1]==3)//On OFF
			{
				m_valueCombx.ResetContent();
				m_valueCombx.InsertString(0,_T("Off"));
				m_valueCombx.InsertString(1,_T("On"));

				m_valueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_valueCombx.ShowWindow(SW_SHOW);
				m_valueCombx.BringWindowToTop();
				//m_valueCombx.SelectString(-1,strValue);
				m_valueCombx.SetFocus(); //获取焦点
			}
			else
			{
				m_inValueEdit.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_inValueEdit.ShowWindow(SW_SHOW);
				
				m_inValueEdit.BringWindowToTop();
				//m_RangCombox.SelectString(-1,strValue);
				m_inValueEdit.SetWindowText(strValue);
				m_inValueEdit.SetFocus(); //获取焦点
			}
		}
		if(lRow==3 && (wAM & 0x02))
		{
			if(multi_register_value[189]==3)//On OFF
			{
				m_valueCombx.ResetContent();
				m_valueCombx.InsertString(0,_T("Off"));
				m_valueCombx.InsertString(1,_T("On"));

				m_valueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_valueCombx.ShowWindow(SW_SHOW);
				m_valueCombx.BringWindowToTop();
				m_valueCombx.SelectString(-1,strValue);
				m_valueCombx.SetFocus(); //获取焦点
			}
			else
			{
				m_inValueEdit.MoveWindow(rc);//移动到选中格的位置，覆盖
				m_inValueEdit.ShowWindow(SW_SHOW);
				
				m_inValueEdit.BringWindowToTop();
				m_inValueEdit.SetWindowText(strValue);
				m_inValueEdit.SetFocus(); //获取焦点
			}
		}
		if(lRow==4 && (wAM & 0x04))
		{
	
			m_valueCombx.ResetContent();
			if(multi_register_value[190]==0)//On/OFF
			{
				m_valueCombx.InsertString(0,_T("On"));
				m_valueCombx.InsertString(1,_T("Off"));
			}
			else
			{
				m_valueCombx.InsertString(0,_T("Off"));
				m_valueCombx.InsertString(1,_T("On"));
			}
				m_valueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
				m_valueCombx.ShowWindow(SW_SHOW);
				m_valueCombx.BringWindowToTop();
				m_valueCombx.SelectString(-1,strValue);
				m_valueCombx.SetFocus(); //获取焦点
		}
		
	}
	if(lCol==RANG_FIELD)
	{	if(lRow==1)
		{
			m_RangCombox.ResetContent();
			for(UINT i=0;i<m_strUnitList.size();i++)
			{
				m_RangCombox.AddString(m_strUnitList.at(i));
			}
			m_RangCombox.ShowWindow(SW_SHOW);//显示控件
			m_RangCombox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_RangCombox.BringWindowToTop();
			m_RangCombox.SelectString(-1,strValue);
			m_RangCombox.SetFocus(); //获取焦点
		}

		if(((lRow==2)&&(product_register_value[MODBUS_TEMP_SELECT]!=1))||(lRow==3))
		{
			//Raw;10K Therm;0-100%;ON/OFF;Custom Sensor;Off/On;
			m_RangCombox.ResetContent();
			if(multi_register_value[121]==0)
			{
				for(int i=0;i<ANALOG_RANG_NUMBER;i++)
				{
					m_RangCombox.AddString(analog_range_0[i]);
				}
			}
			if(multi_register_value[121]==1)
			{
				for(int i=0;i<ANALOG_RANG_NUMBER;i++)
				{
					m_RangCombox.AddString(analog_range_1[i]);
				}
			}

			m_RangCombox.ShowWindow(SW_SHOW);//显示控件
			m_RangCombox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_RangCombox.BringWindowToTop();
			m_RangCombox.SelectString(-1,strValue);
			m_RangCombox.SetFocus(); //获取焦点
		}
		if(lRow==4) 
		{
			m_RangCombox.ResetContent();
			m_RangCombox.AddString(_T("On/Off"));
			m_RangCombox.AddString(_T("Off/On"));

			m_RangCombox.ShowWindow(SW_SHOW);//显示控件
			m_RangCombox.MoveWindow(rc); //移动到选中格的位置，覆盖
			m_RangCombox.BringWindowToTop();
			m_RangCombox.SelectString(-1,strValue);
			m_RangCombox.SetFocus(); //获
		}

	}

	if(lCol==CAL_FIELD)
	{
		CRect rcUp(rc.left,rc.top,rc.left+rc.Width()/2,rc.bottom);
		CRect rcDown(rc.left+rc.Width()/2,rc.top,rc.right,rc.bottom);
		m_upButton.MoveWindow(rcUp);
		m_upButton.ShowWindow(SW_SHOW);
		m_upButton.BringWindowToTop();
		m_downButton.MoveWindow(rcDown);
		m_downButton.ShowWindow(SW_SHOW);
		m_downButton.BringWindowToTop();

		if(lRow==2)
		{
			if(product_register_value[MODBUS_ANALOG_IN1]==3||product_register_value[MODBUS_ANALOG_IN1]==5)
			{
					
				m_downButton.ShowWindow(SW_HIDE);
				m_upButton.ShowWindow(SW_HIDE);
			}
			else
			{
				m_downButton.ShowWindow(SW_SHOW);
				m_upButton.ShowWindow(SW_SHOW);
			}

		}
		if(lRow==3)
		{
			if(multi_register_value[189]==3||multi_register_value[189]==5)
			{
				m_downButton.ShowWindow(SW_HIDE);
				m_upButton.ShowWindow(SW_HIDE);
			}
			else
			{
				m_downButton.ShowWindow(SW_SHOW);
				m_upButton.ShowWindow(SW_SHOW);
			}

		}
		if(lRow==4)
		{
			m_downButton.ShowWindow(SW_HIDE);
			m_upButton.ShowWindow(SW_HIDE);
	
		}
	}
	if(lCol==AM_FIELD&&lRow!=1)
	{
		m_AmCombox.ShowWindow(SW_SHOW);//显示控件
		m_AmCombox.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_AmCombox.BringWindowToTop();
		m_AmCombox.SelectString(-1,strValue);
		m_AmCombox.SetFocus(); //获取焦点
	}
	if(FUN_FIELD==lCol&&lRow!=1)
	{
		if(lRow==2||lRow==3)
		{
			m_inputFinCombox.ResetContent();
			for(int i=0;i<8;i++)
				m_inputFinCombox.AddString(INPUT_FUNS[i]);
		}
		if(lRow==4)
		{
			m_inputFinCombox.ResetContent();
			for(int i=0;i<6;i++)
				m_inputFinCombox.AddString(INPUT_FUNS[i]);
		}

		
		m_inputFinCombox.ShowWindow(SW_SHOW);//显示控件
		m_inputFinCombox.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_inputFinCombox.BringWindowToTop();
		m_inputFinCombox.SelectString(-1,strValue);
		m_inputFinCombox.SetFocus(); //获取焦点
		
	}
	if(lRow==2&&CUST_FIELD==lCol&&product_register_value[MODBUS_ANALOG_IN1]==4)
	{
		m_customBtn.ShowWindow(SW_SHOW);
		m_customBtn.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_customBtn.BringWindowToTop();
		m_customBtn.SetFocus(); //获取焦点
	}
	if(lRow==3&&CUST_FIELD==lCol&&multi_register_value[189]==4)
	{
		m_customBtn.ShowWindow(SW_SHOW);
		m_customBtn.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_customBtn.BringWindowToTop();
		m_customBtn.SetFocus(); //获取焦点
	}
	if(lCol==NAME_FIELD)
	{
		m_inputNameEdt.MoveWindow(&rc,1);
		m_inputNameEdt.ShowWindow(SW_SHOW);
		m_inputNameEdt.SetWindowText(strValue);
		m_inputNameEdt.SetFocus();
		int nLenth=strValue.GetLength();
		m_inputNameEdt.SetSel(nLenth,nLenth); //全选//

	}

}

void CInputSetDlg::OnCbnSelchangeRangCombo()
{
//	if( m_nModel == 16 || m_nModel == PM_TSTAT6 )
	if(m_nModel == PM_TSTAT6 || m_nModel == PM_TSTAT7|| m_nModel == PM_TSTAT5E)  //tstat6
	{
		OnCbnSelchangeRangComboFor5E();
	
		return;
	}
	m_downButton.ShowWindow(SW_HIDE);
	m_upButton.ShowWindow(SW_HIDE);

	if(m_nCurRow==1&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		int ret=write_one(g_tstat_id,121,nindext);
		 if (ret>0)
		 {
		  product_register_value[121]=nindext;
		 }
		 else
		 {
		  AfxMessageBox(_T("Fail"));
		 }
	}

	if(m_nCurRow==2&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		int ret=write_one(g_tstat_id,MODBUS_ANALOG_IN1,nindext);
		 if (ret>0)
		 {  
		 if(product_register_value[MODBUS_ANALOG_IN1]==4)
		 {
			 m_FlexGrid.put_TextMatrix(2,CUST_FIELD,_T("Custom..."));
		 }
		 else
		 {
			 m_customBtn.ShowWindow(SW_HIDE);
			 m_FlexGrid.put_TextMatrix(2,CUST_FIELD,NO_APPLICATION);
		 }
		 } 
		 else
		 {
		   AfxMessageBox(_T("Try again"));
		 }
		
	}
	if(m_nCurRow==3&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		int ret=write_one(g_tstat_id,189,nindext);
		if (ret>0)
		{if(multi_register_value[189]==4)
		{
			m_FlexGrid.put_TextMatrix(3,CUST_FIELD,_T("Custom..."));
		}
		else
		{
			m_customBtn.ShowWindow(SW_HIDE);
			m_FlexGrid.put_TextMatrix(3,CUST_FIELD,NO_APPLICATION);
		}
		} 
		else
		{
		AfxMessageBox(_T("Try again"));
		}
		
	}
	if(m_nCurRow==4&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		int ret=write_one(g_tstat_id,190,nindext);
		if (ret>0)
		{
		product_register_value[190]=nindext;
		}
		else
		{
		 AfxMessageBox(_T("Try again"));
		}

	}
	Fresh_Grid();
}

void CInputSetDlg::OnCbnKillfocusRangCombo()
{
	m_RangCombox.ShowWindow(SW_HIDE);
	// TODO: Add your control notification handler code here
}

void CInputSetDlg::OnBnClickedUpbutton()
{
	if (m_nModel == 16 || m_nModel == PM_TSTAT6 ) // for 5E
	{
		OnBnClickedUpbuttonFor5E();

		Fresh_Grid();
		return;
	}

	//the code below is for tstat5 ABCE use,this register address is special for others,so can't be changed. 
	//comments by Fance.
	if(m_nCurRow==1&&m_nCurCol==CAL_FIELD)
	{
		int nValue=product_register_value[101]+1;
		int ret=write_one(g_tstat_id,101,nValue);
		if (ret>0)
		{
		 product_register_value[101]=nValue;
		}
	}
	if(m_nCurRow==2&& m_nCurCol==CAL_FIELD)
	{
		int ret=write_one(g_tstat_id,180,product_register_value[180]+1);
		if (ret>0)
		{
		product_register_value[180]=product_register_value[180]+1;
		}
	}
	if(m_nCurRow==3&& m_nCurCol==CAL_FIELD)
	{
		int ret=write_one(g_tstat_id,181,product_register_value[181]+1);
		if (ret>0)
		{
		   product_register_value[181]=product_register_value[181]+1;
		}
	}
	Fresh_Grid();
}


//The button has hide ,unnecessary to change.
void CInputSetDlg::OnBnClickedDownbutton()
{
	if (m_nModel == 16 || m_nModel == PM_TSTAT6 ) // for 5E
	{
		OnBnClickedDownbuttonFor5E();

		Fresh_Grid();
		return;
	}

	//the code below is for tstat5 ABCE use,this register address is special for others,so can't be changed. 
	//comments by Fance.
	if(m_nCurRow==1&&m_nCurCol==CAL_FIELD)
	{
		int nValue=product_register_value[101]-1;
		int ret=write_one(g_tstat_id,101,nValue);
		if (ret>0)
		{
		product_register_value[101]-=1;
		}
	}
	if(m_nCurRow==2&& m_nCurCol==CAL_FIELD)
	{
	    int nvalue=product_register_value[180]-1;
		int ret=write_one(g_tstat_id,180,product_register_value[180]-1);
		if (ret>0)
		{
		product_register_value[180]-=1;
		}
	}
	if(m_nCurRow==3&& m_nCurCol==CAL_FIELD)
	{
		 
		int nvalue=product_register_value[181]-1;
		int ret=write_one(g_tstat_id,181,product_register_value[181]-1);
		if (ret>0)
		{
			product_register_value[181]-=1;
		}
	}
	Fresh_Grid();
}

void CInputSetDlg::OnCbnKillfocusAmcombo()
{
	m_AmCombox.ShowWindow(SW_HIDE);
}

void CInputSetDlg::add_analog_rang(int i)
{

}


void CInputSetDlg::OnCbnSelchangeInputfuncombo()
{
	//	298	167	1	Low byte	W/R	Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	299	168	1	Low byte	W/R	Analog input2 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	300	169	1	Low byte	W/R	(future)Analog input3 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	301	170	1	Low byte	W/R	(future)Analog input4 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	302	171	1	Low byte	W/R	(future)Analog input5 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	303	172	1	Low byte	W/R	(future)Analog input6 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	304	173	1	Low byte	W/R	(future)Analog input7 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
	//	305	174	1	Low byte	W/R	(future)Analog input8 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.


	int ret = 0;
	if (product_register_value[MODBUS_PRODUCT_MODEL] ==6)
	{
		if(m_nCurCol==FUN_FIELD)
		{
			int nchange=0;
			nchange=m_inputFinCombox.GetCurSel();
			ret=write_one(g_tstat_id,MODBUS_ANALOG1_FUNCTION+ m_nCurRow -2,nchange);//298 167 
			product_register_value[MODBUS_ANALOG1_FUNCTION+ m_nCurRow -2] = nchange;	//298 167
		}
	}
	else
	{
		if((m_nCurRow>=2)&&(m_nCurRow<=4))
		{
			int nchange=0;
			nchange=m_inputFinCombox.GetCurSel();
			ret=write_one(g_tstat_id,MODBUS_ANALOG1_FUNCTION+ m_nCurRow -2,nchange);//298 167 
			product_register_value[MODBUS_ANALOG1_FUNCTION+ m_nCurRow -2] = nchange;	//298 167
		}
	}
	if(ret<0)
	{
		AfxMessageBox(_T("Access Fail!Please try again!"));
	}
	else
	{
		AfxMessageBox(_T("Change success!"));
	}
	Fresh_Grid();

}

void CInputSetDlg::OnCbnKillfocusInputfuncombo()
{
	m_inputFinCombox.ShowWindow(SW_HIDE);
}

void CInputSetDlg::OnBnClickedCustombutton()
{
	if(m_nCurRow==2&&m_nCurCol==CUST_FIELD)
	{
		CBuildTable1 Dlg(2);
		Dlg.DoModal();
	}
	if(m_nCurRow==3&&m_nCurCol==CUST_FIELD)
	{
		CBuildTable1 Dlg(3);
		Dlg.DoModal();
	}
}

void CInputSetDlg::OnBnKillfocusCustombutton()
{
	//m_customBtn.ShowWindow(SW_HIDE);
}

void CInputSetDlg::OnBnClickedExit()
{
	OnCancel();
	//CDialog::OnOK();
}

void CInputSetDlg::OnEnKillfocusInvalueedit()
{
	if(m_nModel == 16 || m_nModel == PM_TSTAT6)
	{
		OnEnKillfocusInvalueeditFor5E();	//5E 以及更高的版本 不让改这一项，点击不会弹出 Edit框。;
		return;
	}
	int ret=0;

		m_inValueEdit.ShowWindow(SW_HIDE);
		CString strText;
		m_inValueEdit.GetWindowText(strText);
		if(strText.IsEmpty())
			return;
		int nValue=_wtoi(strText);
		if(m_nCurCol==VALUE_FIELD&&m_nCurRow==1)
		{
			ret=write_one(g_tstat_id,MODBUS_TEMPRATURE_CHIP,nValue*10);	//101
			if(ret<0)
			{
				AfxMessageBox(_T("Write Fail!Please try again!"));
				return;
			}
		}
		if(m_nCurCol==VALUE_FIELD&&m_nCurRow==2)
		{
			if(product_register_value[MODBUS_ANALOG_IN1]==4||product_register_value[MODBUS_ANALOG_IN1]==1)//188
			{
				write_one(g_tstat_id,MODBUS_EXTERNAL_SENSOR_0,nValue*10);//180
			}
			if (product_register_value[MODBUS_ANALOG_IN1]==0||product_register_value[MODBUS_ANALOG_IN1]==2)//188
			{
				write_one(g_tstat_id,MODBUS_EXTERNAL_SENSOR_0,nValue);	//180
			}//On/Off 通过combox选择。;
	
		}
		if(m_nCurCol==VALUE_FIELD&&m_nCurRow==3)
		{
			
			if(product_register_value[MODBUS_ANALOG_IN2]==4||product_register_value[MODBUS_ANALOG_IN2]==1)	//189
			{
				write_one(g_tstat_id,MODBUS_EXTERNAL_SENSOR_1,nValue*10);	//181
			}
			if (product_register_value[MODBUS_ANALOG_IN2]==0||product_register_value[MODBUS_ANALOG_IN2]==2)	//189
			{
				write_one(g_tstat_id,MODBUS_EXTERNAL_SENSOR_1,nValue);	//181;
			}
		}
	Fresh_Grid();

}
//On/OFF Off=0,On=1;
//OFF/ON  Off=1, On=0;
void CInputSetDlg::OnCbnKillfocusValuecombo()
{
	if (m_nModel == 16 || m_nModel == PM_TSTAT6)
	{
		//OnCbnKillfocusValuecombo;//这里原来有错

// 		m_valueCombx.ShowWindow(SW_HIDE);
// 		int nItem=0;
// 		nItem=m_valueCombx.GetCurSel();
// 		if(m_nCurCol==VALUE_FIELD&&m_nCurRow==2)//找不到对应的tstat6
// 		{
// 			if(nItem==0)//off
// 				write_one(g_tstat_id,180,0);
// 			if(nItem==1)
// 				write_one(g_tstat_id,180,1);
// 		}
// 		if(m_nCurCol==VALUE_FIELD&&m_nCurRow==3)
// 		{
// 			if(nItem==0)//off
// 				write_one(g_tstat_id,181,0);
// 			if(nItem==1)
// 				write_one(g_tstat_id,181,1);
// 		}
// 		if(m_nCurCol==VALUE_FIELD&&m_nCurRow==4)
// 		{
// 			if (multi_register_value[190]==0)
// 			{
// 				if(nItem==0)//off
// 					write_one(g_tstat_id,311,0);
// 				else
// 					write_one(g_tstat_id,311,1);
// 			}
// 			if (multi_register_value[190]==1)
// 			{
// 
// 				if(nItem==0)
// 					write_one(g_tstat_id,311,1);//
// 				else
// 					write_one(g_tstat_id,311,0);//
// 			}
// 		}
// 		Fresh_Grid();
		return;
	}
	m_valueCombx.ShowWindow(SW_HIDE);
	int nItem=0;
	nItem=m_valueCombx.GetCurSel();
	if(m_nCurCol==VALUE_FIELD&&m_nCurRow==2)
	{
		if(nItem==0)//off
			write_one(g_tstat_id,180,0);
		if(nItem==1)
			write_one(g_tstat_id,180,1);
	}
	if(m_nCurCol==VALUE_FIELD&&m_nCurRow==3)
	{
		if(nItem==0)//off
			write_one(g_tstat_id,181,0);
		if(nItem==1)
			write_one(g_tstat_id,181,1);
	}
	if(m_nCurCol==VALUE_FIELD&&m_nCurRow==4)
	{
		if (multi_register_value[190]==0)
		{
			if(nItem==0)//off
				write_one(g_tstat_id,311,0);
			else
				write_one(g_tstat_id,311,1);
		}
		if (multi_register_value[190]==1)
		{

			if(nItem==0)
				write_one(g_tstat_id,311,1);//
			else
				write_one(g_tstat_id,311,0);//
		}
	}
		Fresh_Grid();
}

void CInputSetDlg::OnBnClickedRefreshbutton()
{

	GetDlgItem(IDC_REFRESHBUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDEXIT)->EnableWindow(FALSE);
	if(InputThread==NULL)
		InputThread = CreateThread(NULL,NULL,StartRefresh,this,NULL,NULL);

	/*

#if 1
	int i;
	register_critical_section.Lock();
	for(i=0;i<16;i++)
	{
		Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64);
	}
	register_critical_section.Unlock();
	memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));//
	if ((multi_register_value[7] == 6)||(multi_register_value[7] == 7))//tstat6
	{
			//multi_register_value[]列表交换。
			memset(tempchange,0,sizeof(tempchange));
			int index = 0;

			for (int i = 0;i<512;i++)
			{
				index = reg_tstat6[i];
				tempchange[index] = multi_register_value[i];
			}
			memcpy(multi_register_value,tempchange,sizeof(multi_register_value));
	}

#endif
	Fresh_Grid();
	*/
}

void CInputSetDlg::OnEnKillfocusInputnameedit()
{
	CString strText;
	m_inputNameEdt.GetWindowText(strText);
	m_inputNameEdt.ShowWindow(SW_HIDE);
	int lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	int lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
	
	CString strInName;
	if(lCol!=NAME_FIELD||lRow==0)
		return;
	switch (lRow)
	{
		case 1:
			strInName=g_strSensorName;
			break;
		case 2:
			strInName=g_strInName1;
			break;
		case 3:
			strInName=g_strInName2;
			break;
		case 4:
			strInName=g_strInName3;
			break;
		case 5:
			strInName=g_strInName4;
			break;
		case 6:
			strInName=g_strInName5;
			break;
		case 7:
			strInName=g_strInName6;
			break;
		case 8:
			strInName=g_strInName7;
			break;
		case 9:
			strInName=g_strInName8;
			break;
	}

	if(strText.CompareNoCase(strInName)==0)
		return;
	
	if ((multi_register_value[7]==PM_TSTAT5G)||(multi_register_value[7]==PM_TSTAT5E)||(multi_register_value[7]==PM_TSTAT6)||(multi_register_value[7]==PM_TSTAT7))
	{
		strText.TrimRight();
		unsigned char p[8];//input+input1

		for(int i=0;i<8;i++)
		{
			if(i<strText.GetLength())
				p[i]=strText.GetAt(i);
			else
				p[i]=' ';
		}
		if (strText.GetLength()>8)
		{
			AfxMessageBox(_T(">8 chars"));
		} 
		else
		{
			if (Write_Multi(g_tstat_id,p,MODBUS_AI1_CHAR1+4*(lRow-2),8)>0)
			{

			} 
			else
			{
				AfxMessageBox(_T("Error"));
				return;
			}

		}
	}

	try
	{

		//if(g_serialNum>0&&multi_register_value[6]>0)
		if(multi_register_value[6]>0)
		{
			_ConnectionPtr m_ConTmp;
			_RecordsetPtr m_RsTmp;
			m_ConTmp.CreateInstance("ADODB.Connection");
			m_RsTmp.CreateInstance("ADODB.Recordset");
			m_ConTmp->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);

			CString strSerial;
			strSerial.Format(_T("%d"),g_serialNum);

			CString strsql;
			strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
			m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
			if(VARIANT_FALSE==m_RsTmp->EndOfFile)//update
			{			
				CString strField;
				switch (lRow)
				{

				case 2:
					strField=_T("INPUT1");
					break;
				case 3:
					strField=_T("INPUT2");
					break;
				case 4:
					strField=_T("INPUT3");
					break;
				case 5:
					strField=_T("INPUT4");
					break;
				case 6:
					strField=_T("INPUT5");
					break;
				case 7:
					strField=_T("INPUT6");
					break;
				case 8:
					strField=_T("INPUT7");
					break;
				case 9:
					strField=_T("INPUT8");
					break;
				}

				try
				{

					CString str_temp;
					str_temp.Format(_T("update IONAME set "+strField+" = '"+strText+"' where SERIAL_ID = '"+strSerial+"'"));
					//AfxMessageBox(str_temp );
					m_ConTmp->Execute(str_temp.GetString(),NULL,adCmdText);
					m_FlexGrid.put_TextMatrix(lRow,lCol,strText);
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}

			}
			else//inerst
			{
				switch (lRow)
				{
				case 2:
					g_strInName1=strText;
					break;
				case 3:
					g_strInName2=strText;
					break;
				case 4:
					g_strInName3=strText;
					break;
				case 5:
					g_strInName4=strText;
					break;
				case 6:
					g_strInName5=strText;
					break;
				case 7:
					g_strInName6=strText;
					break;
				case 8:
					g_strInName7=strText;
					break;
				case 9:
					g_strInName8=strText;
					break;
				}

				CString g_strInName9;
				//g_strInName9 = _T("input9");Humidity Sensor
				g_strInName9 = _T("Humidity Sensor");
				CString	str_temp;
				str_temp.Format(_T("insert into IONAME values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')"),
					strSerial,
					g_strInName1,
					g_strInName2,
					g_strInName3,
					g_strInName4,
					g_strInName5,
					g_strInName6,
					g_strInName7,
					g_strOutName1,
					g_strOutName2,
					g_strOutName3,
					g_strOutName4,
					g_strOutName5,
					g_strOutName6,
					g_strOutName7,
					g_strInName8,
					g_strInHumName,
					g_strSensorName
					);
				try
				{

					m_ConTmp->Execute(str_temp.GetString(),NULL,adCmdText);
					m_FlexGrid.put_TextMatrix(lRow,lCol,strText);
				}
				catch(_com_error *e)
				{
					AfxMessageBox(e->ErrorMessage());
				}
			}

			switch (lRow)
			{
			case 2:
				g_strInName1=strText;
				break;
			case 3:
				g_strInName2=strText;
				break;
			case 4:
				g_strInName3=strText;
				break;
			case 5:
				g_strInName4=strText;
				break;
			case 6:
				g_strInName5=strText;
				break;
			case 7:
				g_strInName6=strText;
				break;
			case 8:
				g_strInName7=strText;
				break;
			case 9:
				g_strInName8=strText;
				break;
			}
			if(m_RsTmp->State) 
				m_RsTmp->Close(); 
			if(m_ConTmp->State)
				m_ConTmp->Close();	
		}	

	}
	catch (...)
	{


	}
	
}


// #define INDEX_FIELD 0
// #define NAME_FIELD 1
// #define VALUE_FIELD 2
// #define AM_FIELD 3
// #define CAL_FIELD 4
// #define RANG_FIELD 5
// #define FUN_FIELD 6
// #define CUST_FIELD 7
// 
// #define TOTAL_COLS 8
// #define TOTAL_ROWS 7
 
void CInputSetDlg::InitGridtstat6()
   {
	   if((product_register_value[20]&2)==2)
	   {
	       m_disable_hum=TRUE;
	   }
	   else
	   {
		   m_disable_hum=FALSE;
	   }
	   if((product_register_value[20]&4)==4)
	   {
		   m_disable_CO2=TRUE;
	   }
	   else
	   {
		   m_disable_CO2=FALSE;
	   }
	   m_FlexGrid.put_TextMatrix(1,INDEX_FIELD,_T("1"));
	   m_FlexGrid.put_TextMatrix(2,INDEX_FIELD,_T("2"));
	   m_FlexGrid.put_TextMatrix(3,INDEX_FIELD,_T("3"));
	   m_FlexGrid.put_TextMatrix(4,INDEX_FIELD,_T("4"));
	   m_FlexGrid.put_TextMatrix(5,INDEX_FIELD,_T("5"));
	   m_FlexGrid.put_TextMatrix(6,INDEX_FIELD,_T("6"));
	   m_FlexGrid.put_TextMatrix(7,INDEX_FIELD,_T("7"));
	   m_FlexGrid.put_TextMatrix(8,INDEX_FIELD,_T("8"));
	   m_FlexGrid.put_TextMatrix(9,INDEX_FIELD,_T("9")); 
	   m_FlexGrid.put_TextMatrix(10,INDEX_FIELD,_T("10"));
	   m_FlexGrid.put_TextMatrix(11,INDEX_FIELD,_T("11"));

	   m_FlexGrid.put_TextMatrix(1,NAME_FIELD,g_strSensorName);
	   m_FlexGrid.put_TextMatrix(2,NAME_FIELD,g_strInName1);
	   m_FlexGrid.put_TextMatrix(3,NAME_FIELD,g_strInName2);
	   m_FlexGrid.put_TextMatrix(4,NAME_FIELD,g_strInName3);
	   m_FlexGrid.put_TextMatrix(5,NAME_FIELD,g_strInName4);
	   m_FlexGrid.put_TextMatrix(6,NAME_FIELD,g_strInName5);
	   m_FlexGrid.put_TextMatrix(7,NAME_FIELD,g_strInName6);
	   m_FlexGrid.put_TextMatrix(8,NAME_FIELD,g_strInName7);
	   m_FlexGrid.put_TextMatrix(9,NAME_FIELD,g_strInName8); 
	   m_FlexGrid.put_TextMatrix(10,NAME_FIELD,g_strInHumName);
	   m_FlexGrid.put_TextMatrix(11,NAME_FIELD,g_strInCO2);
	    
	   
	   CString strUnit=GetTempUnit();
	   CString strTemp;	
	   int nValue=0;
	   //float nValue=0;
	   CString strAuto=_T("Auto");
	   CString strman=_T("Manual");


	   for(int i=1;i<=9;i++)
	   {	
		   CString strIndex;
		   strIndex.Format(_T("%d"), i);
		   m_FlexGrid.put_TextMatrix(i,INDEX_FIELD,strIndex);

		   for(int k=0;k<=8;k++)
		   {
			   m_FlexGrid.put_ColAlignment(k,4);
			   if (i%2==1)
			   {
				   m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(255,255,255));
			   }
			   else
			   {
				   m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(COLOR_CELL);
			   }
		   }

		   CString strTemp;
		   m_fFirmwareVersion = get_curtstat_version();
		   if(m_fFirmwareVersion>33.3)
		   {
			   float fValue;

			   // Row1
			   if(i==1)
			   {
				   //strTemp.Format(_T("%.1f"),multi_register_value[216]/10.0);
				   //216	130	2	Full	W/R	Internal Thermistor Sensor - Shows the filtered, calibrated value of the internal thermistor sensor
				   if(product_register_value[MODBUS_PRODUCT_MODEL] ==  16)	//5E 的这里和其他寄存器不一样，所以没办法合并;
				   {
					   strTemp.Format(_T("%.1f"),product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);//216

				   }
				   else if ((product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7))
				   {
					   strTemp.Format(_T("%.1f"),product_register_value[130]/10.0);
				   }
				   else
				   {
					   strTemp.Format(_T("%.1f"),product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);  //121
				   }


				   strTemp=strTemp+strUnit;
				   m_FlexGrid.put_TextMatrix(1,VALUE_FIELD,strTemp);
				   m_FlexGrid.put_TextMatrix(1,AM_FIELD,NO_APPLICATION);
				   m_FlexGrid.put_TextMatrix(1,CAL_FIELD,_T("Adjust..."));
				   m_FlexGrid.put_TextMatrix(1,RANG_FIELD,strUnit);
				   m_FlexGrid.put_TextMatrix(1,FUN_FIELD,NO_APPLICATION);
				   m_FlexGrid.put_TextMatrix(1,CUST_FIELD,NO_APPLICATION);

				   m_FlexGrid.put_Col(FILTER);
				   m_FlexGrid.put_Row(i);
				   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);

				   continue;
			   }



			   //////////////////////////////////////////////////////////////////////////
			   // column 1  Value


			   // 
			   // 						359	122	1	Low byte	W/R	ANALOG INPUT1 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						2	123	1	Low byte	W/R	ANALOG INPUT2 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						3	124	1	Low byte	W/R	ANALOG INPUT3 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						4	125	1	Low byte	W/R	ANALOG INPUT4 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						5	126	1	Low byte	W/R	ANALOG INPUT5 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						6	127	1	Low byte	W/R	ANALOG INPUT6 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						7	128	1	Low byte	W/R	ANALOG INPUT7 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			   // 						8	129	1	Low byte	W/R	ANALOG INPUT8 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON


			   // 
			   // 						367	131	2	Full	W/R	Analog input1 value
			   // 						10	132	2	Full	W/R	Analog input2 value
			   // 						11	133	2	Full	W/R	Analog input3 value
			   // 						12	134	2	Full	W/R	Analog input4 value
			   // 						13	135	2	Full	W/R	Analog input5 value
			   // 						14	136	2	Full	W/R	Analog input6 value
			   // 						15	137	2	Full	W/R	Analog input7 value
			   // 						16	138	2	Full	W/R	Analog input8 value




			   CString strValueUnit=GetTempUnit(product_register_value[MODBUS_ANALOG1_RANGE+i-2], 1); //5e=359   122
			   {
				   if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==1)	//359  122
				   {				
					   fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);	//367   131
					   strTemp.Format(_T("%.1f"),fValue);	

					   strTemp +=strValueUnit;
				   }
				   else if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==3 || product_register_value[MODBUS_ANALOG1_RANGE+i-2]==5) // On/Off or Off/On ==1 On ==0 Off   359  122
				   {						
					   int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-2]); //367  131
					   if (nValue == 0)
					   {
						   strTemp = _T("Off");
					   }
					   else
					   {
						   strTemp = _T("On");
					   }					
				   }
				   else if (product_register_value[MODBUS_ANALOG1_RANGE+i-2]==4 )  // custom sensor	359 122
				   {					
					   fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);	//367  131
					   strTemp.Format(_T("%.1f"), (float)fValue/10.0);	
					   strTemp +=strValueUnit;
				   }
				   else if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==2)	//359 122
				   {
					   nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-2];		//367  131
					   strTemp.Format(_T("%0.1f%%"),  (float)nValue);
				   }
				   else
				   {
					   //strTemp.Format(_T("%d"),multi_register_value[367+i-2]);//lsc
					   strTemp.Format(_T("UNUSED"));

				   }						
				   m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);
				   strTemp.Empty();
			   }

			   //309	141	2	Full	W/R	Input auto/ manual enable.
			   // column 2  Auto/Manual // Ö»ÓÐIN1£¬2²ÅÓÐ,internal sensor Ã»ÓÐ£¬´óÓÚ1 // 
			   if(i>1)//( i== 2 || i == 3)
			   {
				   nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//309    141
				   BYTE bFilter=0x01;
				   bFilter = bFilter<< (i-2);
				   if((nValue & bFilter))
				   {
					   m_FlexGrid.put_TextMatrix(i,AM_FIELD,strman);			
				   }
				   else
				   {
					   m_FlexGrid.put_TextMatrix(i,AM_FIELD,strAuto);				
				   }
			   }
			   else
			   {
				   m_FlexGrid.put_Col(AM_FIELD);
				   m_FlexGrid.put_Row(i);
				   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
			   }

			   // column 3  Calibrate			
			    
				   m_FlexGrid.put_TextMatrix(i,CAL_FIELD,_T("Adjust..."));				
			    
			 
				strTemp.Format(_T("%d"),product_register_value[141+i]);
			    m_FlexGrid.put_TextMatrix(i,FILTER,strTemp);		
			   // column 4  Range 
			   //if( i >= 2 )
			   {
				   int nItem=product_register_value[MODBUS_ANALOG1_RANGE+i-2];		//359  122
				   if(nItem>=0 && nItem<=5)
				   {
					   strTemp=analog_range_0[nItem];
				   }
				   m_FlexGrid.put_TextMatrix(i,RANG_FIELD,strTemp);
			   }

			   //298	167	1	Low byte	W/R	Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			   // column 5 Function Ö»ÓÐIN1£¬2²ÅÓÐ


			   // 				298	167	1	Low byte	W/R	Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			   // 				299	168	1	Low byte	W/R	Analog input2 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			   // 				169	1	Low byte	W/R	(future)Analog input3 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			   // 				170	1	Low byte	W/R	(future)Analog input4 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			   // 				171	1	Low byte	W/R	(future)Analog input5 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			   // 				172	1	Low byte	W/R	(future)Analog input6 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			   // 				173	1	Low byte	W/R	(future)Analog input7 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			   // 				174	1	Low byte	W/R	(future)Analog input8 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.


			   nValue=product_register_value[MODBUS_ANALOG1_FUNCTION+i-2];		//298   167
			   if(product_register_value[MODBUS_PRODUCT_MODEL] ==  6)
			   {
				   if (nValue>=0&&nValue<8)//tstat6
				   {		
					   strTemp=INPUT_FUNS[nValue];
					   m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				   }
			   }
			   else
			   {
				   if(i== 2 || i==3)
				   {
					   nValue=product_register_value[298+i-2];
					   strTemp=INPUT_FUNS[nValue];
					   m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				   }
				   else
				   {
					   m_FlexGrid.put_Col(FUN_FIELD);
					   m_FlexGrid.put_Row(i);
					   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
				   }
			   }
			   if(product_register_value[MODBUS_PRODUCT_MODEL] ==  6)
			   {
				   m_customBtn.ShowWindow(SW_HIDE);
				   m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
			   }
			   else
			   {
				   if( i== 2 || i==3)
				   {
					   if(product_register_value[MODBUS_ANALOG_IN1]==4)	//188
					   {
						   m_FlexGrid.put_TextMatrix(i,CUST_FIELD,_T("Custom..."));
					   }
					   else
					   {
						   m_customBtn.ShowWindow(SW_HIDE);
						   m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
					   }
				   }
				   else
				   {
					   m_FlexGrid.put_Col(CUST_FIELD);
					   m_FlexGrid.put_Row(i);
					   m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);	
					   			
				   }
			   }
				
			  /* for (int i=1;i<=8;i++)
			   {*/
				  
				  

					 
				 
			   /*}*/
			  

		   }
		   else  // another lower version of software	,The version too low, can't be changed .  Fance.	
		   {
			   int nValue;

			   if(product_register_value[341+i-2]==1)	//341
			   {
				   nValue=(int)(product_register_value[349+i-2]/10.0);
				   strTemp.Format(_T("%.1f"),nValue);
			   }
			   else
			   {
				   strTemp.Format(_T("%d"),product_register_value[349+i-2]);
			   }
			   m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);

			   strTemp.Empty();
			   int nItem=product_register_value[341+i-2];
			   if(nItem>=0&&nItem<=4)
			   {
				   strTemp=analog_range_0[nItem];					
			   }
			   m_FlexGrid.put_TextMatrix(i,AM_FIELD,strTemp);
		   }

	   }
	   	 CString temp;
	   if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
	   {

		   temp.Format(_T("%0.1f%%"),(float)product_register_value[MODBUS_TSTAT6_HUM_AVALUE]/10.0);
		    m_FlexGrid.put_TextMatrix(10,AM_FIELD,strAuto);
	   }
	   else
	   {
	       m_FlexGrid.put_TextMatrix(10,AM_FIELD,strman);
		   temp.Format(_T("%0.1f%%"),(float)product_register_value[MODBUS_TSTAT6_HUM_MVALUE]/10);

	   }


	   m_FlexGrid.put_TextMatrix(10,VALUE_FIELD,temp);
	   m_FlexGrid.put_TextMatrix(10,CAL_FIELD,_T("Adjust..."));
	   if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
	   {     strUnit=_T("ppm");
		   m_FlexGrid.put_TextMatrix(11,AM_FIELD,strAuto);
		   temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_AVALUE]);
		   temp+=strUnit;
		   m_FlexGrid.put_TextMatrix(11,VALUE_FIELD,temp);
	   }
	   else
	   {
		   m_FlexGrid.put_TextMatrix(11,AM_FIELD,strman);
		   temp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_MVALUE]);
		    temp+=strUnit;
		   m_FlexGrid.put_TextMatrix(11,VALUE_FIELD,temp);
	   }	
	   m_FlexGrid.put_TextMatrix(11,CAL_FIELD,_T("Adjust..."));
   }
void CInputSetDlg::Init_not_5ABCD_Grid()
{
	m_FlexGrid.put_TextMatrix(1,NAME_FIELD,g_strSensorName);
	m_FlexGrid.put_TextMatrix(2,NAME_FIELD,g_strInName1);
	m_FlexGrid.put_TextMatrix(3,NAME_FIELD,g_strInName2);
	m_FlexGrid.put_TextMatrix(4,NAME_FIELD,g_strInName3);
	m_FlexGrid.put_TextMatrix(5,NAME_FIELD,g_strInName4);
	m_FlexGrid.put_TextMatrix(6,NAME_FIELD,g_strInName5);
	m_FlexGrid.put_TextMatrix(7,NAME_FIELD,g_strInName6);
	m_FlexGrid.put_TextMatrix(8,NAME_FIELD,g_strInName7);
	m_FlexGrid.put_TextMatrix(9,NAME_FIELD,g_strInName8);

	CString strUnit=GetTempUnit();
	CString strTemp;	
	int nValue=0;
	//float nValue=0;
	CString strAuto=_T("Auto");
	CString strman=_T("Manual");


	for(int i=1;i<=9;i++)
	{	
		CString strIndex;
		strIndex.Format(_T("%d"), i);
		m_FlexGrid.put_TextMatrix(i,INDEX_FIELD,strIndex);

		for(int k=0;k<=6;k++)
		{
			m_FlexGrid.put_ColAlignment(k,4);
			if (i%2==1)
			{
				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(COLOR_CELL);
			}
		}

		CString strTemp;
		m_fFirmwareVersion = get_curtstat_version();
		if(m_fFirmwareVersion>33.3)
		{
			float fValue;

			// Row1
			if(i==1)
			{
				//strTemp.Format(_T("%.1f"),multi_register_value[216]/10.0);
				//216	130	2	Full	W/R	Internal Thermistor Sensor - Shows the filtered, calibrated value of the internal thermistor sensor
				if(product_register_value[MODBUS_PRODUCT_MODEL] ==  16)	//5E 的这里和其他寄存器不一样，所以没办法合并;
				{
					strTemp.Format(_T("%.1f"),product_register_value[MODBUS_INTERNAL_THERMISTOR]/10.0);//216

				}
				else if ((product_register_value[7]==PM_TSTAT6)||(product_register_value[7]==PM_TSTAT7))
				{
				   strTemp.Format(_T("%.1f"),product_register_value[130]/10.0);
				}
				else
				{
					strTemp.Format(_T("%.1f"),product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);  //121
				}

				strTemp=strTemp+strUnit;
				m_FlexGrid.put_TextMatrix(1,VALUE_FIELD,strTemp);
				m_FlexGrid.put_TextMatrix(1,AM_FIELD,NO_APPLICATION);
				m_FlexGrid.put_TextMatrix(1,CAL_FIELD,_T("Adjust..."));
				m_FlexGrid.put_TextMatrix(1,RANG_FIELD,strUnit);
				m_FlexGrid.put_TextMatrix(1,FUN_FIELD,NO_APPLICATION);
				m_FlexGrid.put_TextMatrix(1,CUST_FIELD,NO_APPLICATION);
				continue;
			}



			//////////////////////////////////////////////////////////////////////////
			// column 1  Value


			// 
			// 						359	122	1	Low byte	W/R	ANALOG INPUT1 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			// 						2	123	1	Low byte	W/R	ANALOG INPUT2 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			// 						3	124	1	Low byte	W/R	ANALOG INPUT3 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			// 						4	125	1	Low byte	W/R	ANALOG INPUT4 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			// 						5	126	1	Low byte	W/R	ANALOG INPUT5 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			// 						6	127	1	Low byte	W/R	ANALOG INPUT6 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			// 						7	128	1	Low byte	W/R	ANALOG INPUT7 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			// 						8	129	1	Low byte	W/R	ANALOG INPUT8 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON


			// 
			// 						367	131	2	Full	W/R	Analog input1 value
			// 						10	132	2	Full	W/R	Analog input2 value
			// 						11	133	2	Full	W/R	Analog input3 value
			// 						12	134	2	Full	W/R	Analog input4 value
			// 						13	135	2	Full	W/R	Analog input5 value
			// 						14	136	2	Full	W/R	Analog input6 value
			// 						15	137	2	Full	W/R	Analog input7 value
			// 						16	138	2	Full	W/R	Analog input8 value




			CString strValueUnit=GetTempUnit(product_register_value[MODBUS_ANALOG1_RANGE+i-2], i-1); //5e=359   122
			{
				if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==1)	//359  122
				{				
					fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);	//367   131
					strTemp.Format(_T("%.1f"),fValue);	

					strTemp +=strValueUnit;
				}
				else if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==3 || product_register_value[MODBUS_ANALOG1_RANGE+i-2]==5) // On/Off or Off/On ==1 On ==0 Off   359  122
				{						
					int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+i-2]); //367  131
					if (nValue == 1)
					{
						strTemp = _T("On");
					}
					else
					{
						strTemp = _T("Off");
					}					
				}
				else if (product_register_value[MODBUS_ANALOG1_RANGE+i-2]==4 )  // custom sensor	359 122
				{					
					fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+i-2]/10.0);	//367  131
					strTemp.Format(_T("%.1f"), (float)fValue/10.0);	
					strTemp +=strValueUnit;
				}
				else if(product_register_value[MODBUS_ANALOG1_RANGE+i-2]==2)	//359 122
				{
					nValue=product_register_value[MODBUS_ANALOG_INPUT1+i-2];		//367  131
					strTemp.Format(_T("%0.1f%%"),  (float)nValue);
				}
				else
				{
					//strTemp.Format(_T("%d"),multi_register_value[367+i-2]);//lsc
					strTemp.Format(_T("UNUSED"));

				}						
				m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);
				strTemp.Empty();
			}

			//309	141	2	Full	W/R	Input auto/ manual enable.
			// column 2  Auto/Manual // Ö»ÓÐIN1£¬2²ÅÓÐ,internal sensor Ã»ÓÐ£¬´óÓÚ1 // 
			if(i>1)//( i== 2 || i == 3)
			{
				nValue=product_register_value[MODBUS_INPUT_MANU_ENABLE];//309    141
				BYTE bFilter=0x01;
				bFilter = bFilter<< (i-2);
				if((nValue & bFilter))
				{
					m_FlexGrid.put_TextMatrix(i,AM_FIELD,strman);			
				}
				else
				{
					m_FlexGrid.put_TextMatrix(i,AM_FIELD,strAuto);				
				}
			}
			else
			{
				m_FlexGrid.put_Col(AM_FIELD);
				m_FlexGrid.put_Row(i);
				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
			}

			// column 3  Calibrate			
			{
				m_FlexGrid.put_TextMatrix(i,CAL_FIELD,_T("Adjust..."));				
			}

			// column 4  Range 
			//if( i >= 2 )
			{
				int nItem=product_register_value[MODBUS_ANALOG1_RANGE+i-2];		//359  122
				if(nItem>=0 && nItem<=5)
				{
					strTemp=analog_range_0[nItem];
				}
				m_FlexGrid.put_TextMatrix(i,RANG_FIELD,strTemp);
			}

			//298	167	1	Low byte	W/R	Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// column 5 Function Ö»ÓÐIN1£¬2²ÅÓÐ


			// 				298	167	1	Low byte	W/R	Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				299	168	1	Low byte	W/R	Analog input2 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				169	1	Low byte	W/R	(future)Analog input3 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				170	1	Low byte	W/R	(future)Analog input4 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				171	1	Low byte	W/R	(future)Analog input5 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				172	1	Low byte	W/R	(future)Analog input6 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				173	1	Low byte	W/R	(future)Analog input7 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.
			// 				174	1	Low byte	W/R	(future)Analog input8 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.


			nValue=product_register_value[MODBUS_ANALOG1_FUNCTION+i-2];		//298   167
			if(product_register_value[MODBUS_PRODUCT_MODEL] ==  6)
			{
				if (nValue>=0&&nValue<8)//tstat6
				{		
					strTemp=INPUT_FUNS[nValue];
					m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				}
			}
			else
			{
				if(i== 2 || i==3)
				{
					nValue=multi_register_value[298+i-2];
					strTemp=INPUT_FUNS[nValue];
					m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				}
				else
				{
				m_FlexGrid.put_Col(FUN_FIELD);
				m_FlexGrid.put_Row(i);
				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
				}
			}

			//	}
			// 			else
			// 			{
			// 				m_FlexGrid.put_Col(FUN_FIELD);
			// 				m_FlexGrid.put_Row(i);
			// 				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
			// 			}
			// column 6 custom tables
			if(product_register_value[MODBUS_PRODUCT_MODEL] ==  6)
			{
				m_customBtn.ShowWindow(SW_HIDE);
				m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
			}else
			{
				if( i== 2 || i==3)
				{
					if(product_register_value[MODBUS_ANALOG_IN1]==4)	//188
					{
						m_FlexGrid.put_TextMatrix(i,CUST_FIELD,_T("Custom..."));
					}
					else
					{
						m_customBtn.ShowWindow(SW_HIDE);
						m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
					}
				}
				else
				{
					m_FlexGrid.put_Col(CUST_FIELD);
					m_FlexGrid.put_Row(i);
					m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
				}
			}

		}
		else  // another lower version of software	,The version too low, can't be changed .  Fance.	
		{
			int nValue;

			if(multi_register_value[341+i-2]==1)	//341
			{
				nValue=(int)(multi_register_value[349+i-2]/10.0);
				strTemp.Format(_T("%.1f"),nValue);
			}
			else
			{
				strTemp.Format(_T("%d"),multi_register_value[349+i-2]);
			}
			m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);

			strTemp.Empty();
			int nItem=multi_register_value[341+i-2];
			if(nItem>=0&&nItem<=4)
			{
				strTemp=analog_range_0[nItem];					
			}
			m_FlexGrid.put_TextMatrix(i,AM_FIELD,strTemp);
		}
	}	

}


void CInputSetDlg::OnClickTstat6Grid(int nRow, int nCol, CRect rcCell)
{
    CString txt=m_FlexGrid.get_TextMatrix(nRow,nCol);
	if (txt.IsEmpty())
	{
	return;
	}
    
 	if (nCol == INDEX_FIELD)// && nRow == 1) // 序号都不能改
 	{
 		return;
 	}
// 	if (nCol == NAME_FIELD  && nRow > 3)  // 只有2个输入
// 	{
// 		return;
// 	}
// 	if (nRow > 3  && (nCol == VALUE_FIELD  || nCol == RANG_FIELD  || nCol == FUN_FIELD  )) 
// 	{
// 		return;
// 	}
	if (nRow == 1 && (nCol == VALUE_FIELD  || nCol == FUN_FIELD || nCol == AM_FIELD ))
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	if (nCol == NAME_FIELD)
	{
		if ((nRow== 1)||(nRow==10)||(nRow==11))
			return;

		CString strValue = m_FlexGrid.get_TextMatrix(nRow,nCol);
		m_inputNameEdt.MoveWindow(rcCell); //移动到选中格的位置，覆盖
		m_inputNameEdt.ShowWindow(SW_SHOW);

		m_inputNameEdt.BringWindowToTop();
		//m_RangCombox.SelectString(-1,strValue);

		m_inputNameEdt.SetWindowText(strValue);
		m_inputNameEdt.SetFocus(); //获取焦点
	}
	//////////////////////////////////////////////////////////////////////////
	//
	if (nCol == VALUE_FIELD)
	{
		//309	141	2	Full	W/R	Input auto/ manual enable.
		int nValue;// = multi_register_value[309];
		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
		{
			nValue = multi_register_value[141];
		}
		else
		{
			nValue = multi_register_value[309];

		}


		int nFilter = 0x01;
		nFilter = nFilter << (m_nCurRow - 1);
		CString strValue = m_FlexGrid.get_TextMatrix(nRow,nCol);
		if (nValue & nFilter)
		{
			//
			int nRangeValue = multi_register_value[359+m_nCurRow-2];


			if (nRangeValue == 3)  // 如果是on/off，用combo
			{	
				m_valueCombx.ResetContent();

				m_valueCombx.InsertString(0,_T("On"));
				m_valueCombx.InsertString(1,_T("Off"));

				m_valueCombx.MoveWindow(rcCell); //移动到选中格的位置，覆盖
				m_valueCombx.ShowWindow(SW_SHOW);
				m_valueCombx.BringWindowToTop();
				//m_valueCombx.SelectString(-1,strValue);
				m_valueCombx.SetFocus(); //获取焦点
				m_valueCombx.SetWindowText(strValue);
			}
			else // 如果是值，用edit
			{		
				m_inValueEdit.MoveWindow(rcCell); //移动到选中格的位置，覆盖
				m_inValueEdit.ShowWindow(SW_SHOW);

				m_inValueEdit.BringWindowToTop();
				//m_RangCombox.SelectString(-1,strValue);
		
				m_inValueEdit.SetWindowText(strValue);
				m_inValueEdit.SetFocus(); //获取焦点

			}		
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//
	CString strAuto=_T("Auto");
	CString strman=_T("Manual");
	if(nCol==AM_FIELD) // A/M
	{
		m_AmCombox.MoveWindow(&rcCell,1); //移动到选中格的位置
		m_AmCombox.BringWindowToTop();
		m_AmCombox.ShowWindow(SW_SHOW);//显示控件
		m_AmCombox.SetFocus(); //获取焦点
		CString strTemp;

		//309	141	2	Full	W/R	Input auto/ manual enable.
		BYTE nValue;// =BYTE(multi_register_value[309]);
	
		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
		{
			nValue=BYTE(multi_register_value[141]);
		}else
		{
			nValue=BYTE(multi_register_value[309]);
		}
		BYTE nFilter = 0x01;
		nFilter = nFilter << (nRow-1);
		if(nValue & nFilter)
		{
			strTemp = strman;			
		}
		else
		{
			strTemp = strAuto;				
		}
		m_AmCombox.SetWindowText(strTemp);
	}

	//////////////////////////////////////////////////////////////////////////
	if(nCol==CAL_FIELD)    // calibrate buttons
	{	
		CRect rcUp(rcCell.left,rcCell.top,rcCell.left+rcCell.Width()/2,rcCell.bottom);
		CRect rcDown(rcCell.left+rcCell.Width()/2,rcCell.top,rcCell.right,rcCell.bottom);
		m_upButton.MoveWindow(rcUp);
		m_upButton.ShowWindow(SW_SHOW);
		m_upButton.BringWindowToTop();
		m_downButton.MoveWindow(rcDown);
		m_downButton.ShowWindow(SW_SHOW);
		m_downButton.BringWindowToTop();

		if(nRow==1)
		{
// 			int nRegisterNum = 110;
// 			if (multi_register_value[111] == 2)
// 			{
// 				nRegisterNum = 317;
// 			}
// 
// 			if(multi_register_value[188]==3)
// 			{
// 				m_downButton.ShowWindow(SW_HIDE);
// 				m_upButton.ShowWindow(SW_HIDE);
// 			}
// 			else
// 			{
				m_downButton.ShowWindow(SW_SHOW);
				m_upButton.ShowWindow(SW_SHOW);
//			}
		}
		else //(nRow==2)
		{
			//359	122	1	Low byte	W/R	ANALOG INPUT1 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON

			int nRange;// = multi_register_value[359+nRow-2];
			
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				nRange= product_register_value[122+nRow-2];
			}else
			{
				nRange = multi_register_value[359+nRow-2];

			}

			
			if( nRange == 3 || nRange == 5)
			{

				m_downButton.ShowWindow(SW_HIDE);
				m_upButton.ShowWindow(SW_HIDE);
			}
			else
			{
				////309	141	2	Full	W/R	Input auto/ manual enable.
				BYTE nValue;// =BYTE(multi_register_value[309]);
			
				if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
				{
					nValue =BYTE(product_register_value[141]);

				}else
				{
					nValue =BYTE(multi_register_value[309]);
				}

				
				BYTE nFilter = 0x01;
				nFilter = nFilter << (nRow-1);
				if(nValue & nFilter) // only sel Auto, can calibrate
				{
					m_downButton.ShowWindow(SW_HIDE);
					m_upButton.ShowWindow(SW_HIDE);
				}
				else
				{
					m_downButton.ShowWindow(SW_SHOW);
					m_upButton.ShowWindow(SW_SHOW);
				}
			}
		}		
	}
	if(nCol==FILTER)
	{

		if ((nRow== 1)||((nRow==10))||((nRow==11)))
			return;

		CString strValue = m_FlexGrid.get_TextMatrix(nRow,nCol);
		m_Filter.MoveWindow(rcCell); //移动到选中格的位置，覆盖
		m_Filter.ShowWindow(SW_SHOW);

		m_Filter.BringWindowToTop();
		//m_RangCombox.SelectString(-1,strValue);

		m_Filter.SetWindowText(strValue);
		m_Filter.SetFocus(); //获取焦点
	}
	//////////////////////////////////////////////////////////////////////////
	if(nCol==RANG_FIELD) 
	{
	if ((nRow==10)||(nRow==11))
	{
	return;
	}
		m_RangCombox.MoveWindow(&rcCell,1); //移动到选中格的位置
		m_RangCombox.BringWindowToTop();
		m_RangCombox.ShowWindow(SW_SHOW);//显示控件
		m_RangCombox.SetFocus(); //获取焦点
	
		CString strTemp;
		if(nRow == 1) // use 121
		{	
			m_RangCombox.ResetContent();
			for(UINT i=0;i<m_strUnitList.size();i++)
			{
				m_RangCombox.AddString(m_strUnitList.at(i));
			}
			//121	104	1	Low byte	W/R(Reboot after write)	DEGC_OR_F, engineering units, Deg C = 0, Deg F = 1

			int nValue;// = multi_register_value[121];//tatat6 :104
			
			if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
			{
				nValue = product_register_value[104];
			}else
			{
				nValue = multi_register_value[121];
			}
			if (nValue==0||nValue==1)//tstat6
			strTemp = m_strUnitList[nValue];
		}
		else
		{
			m_RangCombox.ResetContent();	
			//359	122	1	Low byte	W/R	ANALOG INPUT1 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
			//121	104	1	Low byte	W/R(Reboot after write)	DEGC_OR_F, engineering units, Deg C = 0, Deg F = 1
			int nValue;// = multi_register_value[359+nRow-2];
			if (product_register_value[7] ==6)
			{
				nValue = product_register_value[122+nRow-2];
				if(product_register_value[104]==0)
				{
					for(int i=0;i<ANALOG_RANG_NUMBER;i++)
					{
						m_RangCombox.AddString(analog_range_0[i]);
					}	
					strTemp = analog_range_0[nValue];
				}
				if(product_register_value[104]==1)
				{
					for(int i=0;i<ANALOG_RANG_NUMBER;i++)
					{
						m_RangCombox.AddString(analog_range_1[i]);
					}
					strTemp = analog_range_1[nValue];
				}
			}else
			{
				nValue = multi_register_value[359+nRow-2];
				if(multi_register_value[121]==0)
				{
					for(int i=0;i<ANALOG_RANG_NUMBER;i++)
					{
						m_RangCombox.AddString(analog_range_0[i]);
					}	
					strTemp = analog_range_0[nValue];
				}
				if(multi_register_value[121]==1)
				{
					for(int i=0;i<ANALOG_RANG_NUMBER;i++)
					{
						m_RangCombox.AddString(analog_range_1[i]);
					}
					strTemp = analog_range_1[nValue];
				}	
			}

		}
		m_RangCombox.SetWindowText(strTemp);
	}

	//////////////////////////////////////////////////////////////////////////
	if(nCol == FUN_FIELD)
	{
		m_inputFinCombox.ResetContent();
		for(int i=0;i<8;i++)
			m_inputFinCombox.AddString(INPUT_FUNS[i]);
		
		m_inputFinCombox.ShowWindow(SW_SHOW);//显示控件
		m_inputFinCombox.MoveWindow(rcCell); //移动到选中格的位置，覆盖
		m_inputFinCombox.BringWindowToTop();
		//m_inputFinCombox.SelectString(-1,strValue);
		m_inputFinCombox.SetFocus(); //获取焦点
		//298	167	1	Low byte	W/R	Analog input1 function selection. 0, normal; 1, freeze protect sensor input; 2, occupancy sensor input; 3, sweep off mode; 4, clock mode; 5, change over mode.

		int nValue;//=multi_register_value[298+nRow-2];

		if ((product_register_value[7] == 6)||(product_register_value[7] == 7))
		{
			nValue=product_register_value[167+nRow-2];
		}else
		{
			nValue=multi_register_value[298+nRow-2];
		}
		CString strTemp(INPUT_FUNS[nValue]);
		m_inputFinCombox.SetWindowText(strTemp);

	}

	//////////////////////////////////////////////////////////////////////////
	//if(nCol == CUST_FIELD)
	//{
	//	if(nRow==2&&product_register_value[MODBUS_ANALOG1_RANGE]==4)///没找到TSTAT6对应值
	//	{
	//		m_customBtn.ShowWindow(SW_SHOW);
	//		m_customBtn.MoveWindow(rcCell); //移动到选中格的位置，覆盖
	//		m_customBtn.BringWindowToTop();
	//		m_customBtn.SetFocus(); //获取焦点
	//	}
	//	if(nRow==3&&product_register_value[MODBUS_ANALOG2_RANGE]==4)
	//	{
	//		m_customBtn.ShowWindow(SW_SHOW);
	//		m_customBtn.MoveWindow(rcCell); //移动到选中格的位置，覆盖
	//		m_customBtn.BringWindowToTop();
	//		m_customBtn.SetFocus(); //获取焦点
	//	}
	//}
}


// 359	122	1	Low byte	W/R	ANALOG INPUT1 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 2	123	1	Low byte	W/R	ANALOG INPUT2 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 3	124	1	Low byte	W/R	ANALOG INPUT3 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 4	125	1	Low byte	W/R	ANALOG INPUT4 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 5	126	1	Low byte	W/R	ANALOG INPUT5 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 6	127	1	Low byte	W/R	ANALOG INPUT6 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 7	128	1	Low byte	W/R	ANALOG INPUT7 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
// 8	129	1	Low byte	W/R	ANALOG INPUT8 RANGE. 0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON

//121	104	1	Low byte	W/R(Reboot after write)	DEGC_OR_F, engineering units, Deg C = 0, Deg F = 1


void CInputSetDlg::OnCbnSelchangeRangComboFor5E()
{	
	if(m_nCurRow==1&&m_nCurCol==RANG_FIELD)
	{
		int nindext=m_RangCombox.GetCurSel();
		//if ((multi_register_value[7] == 6)||(multi_register_value[7] == 7))//tstat6
		int ret=write_one(g_tstat_id,MODBUS_DEGC_OR_F,nindext);//121  104 
		if(ret<0)
		{
			AfxMessageBox(_T("Write error!Please try again!"));
			return;
		}
	}




	if(m_nCurRow >= 2 && m_nCurCol==RANG_FIELD)
	{

		vector<int> vet_Row;
		for (int i=0;i<8;i++)
		{
			if (product_register_value[MODBUS_ANALOG1_RANGE+i]==4)
			{   
				vet_Row.push_back(i);
			}
		}
		if (vet_Row.size()>=2)
		{
			int nindext=m_RangCombox.GetCurSel();
			if (nindext!=4)
			{
				int ret=write_one(g_tstat_id,MODBUS_ANALOG1_RANGE+m_nCurRow-2,nindext);	//5e=359  6=122
				if(ret<0)
				{
					AfxMessageBox(_T("Write Fail!Please try again!"));
					return;
				}
				product_register_value[MODBUS_ANALOG1_RANGE+m_nCurRow-2] = nindext;
				Fresh_Grid();
			}
			//if ((multi_register_value[7] == 6)||(multi_register_value[7] == 7))//tstat6
			else
			{
				AfxMessageBox(_T("More than 2 rows for custom sensor"));
				
			}

			return;


		}
		 






		int nindext=m_RangCombox.GetCurSel();
		//if ((multi_register_value[7] == 6)||(multi_register_value[7] == 7))//tstat6
		int ret=write_one(g_tstat_id,MODBUS_ANALOG1_RANGE+m_nCurRow-2,nindext);	//5e=359  6=122
		if(ret<0)
		{
			AfxMessageBox(_T("Write Fail!Please try again!"));
			return;
		}
		product_register_value[MODBUS_ANALOG1_RANGE+m_nCurRow-2] = nindext;



		//Sleep(2000);
		/*int retread = read_one(g_tstat_id,MODBUS_ANALOG_INPUT1+m_nCurRow-2,5);
		if(retread<0)
		MessageBox(_T("Read Register Fail!Please try it again!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		else
		product_register_value[MODBUS_ANALOG_INPUT1+m_nCurRow-2] =retread;*/
	}




	//OnBnClickedRefreshbutton();//stat6 TSTAT6点击这时，再去刷新则会出现错误，崩溃.
	Fresh_Grid();
}

//Recode by Fance ,the old one is too many and misellaneous with trifles
void CInputSetDlg::OnBnClickedUpbuttonFor5E()
{
	
	if(m_nCurRow==1&&m_nCurCol==CAL_FIELD)
	{
		int nValue=product_register_value[MODBUS_TEMPRATURE_CHIP]+1;	//101 121
		write_one(g_tstat_id,MODBUS_TEMPRATURE_CHIP,nValue);//101  121
		product_register_value[MODBUS_TEMPRATURE_CHIP] = nValue;	//101  121
	}
	else if (m_nCurRow<10)
	{
		int nAddr =MODBUS_ANALOG_INPUT1+m_nCurRow-2;	//367  131
		write_one(g_tstat_id, nAddr, product_register_value[nAddr]+1);
		product_register_value[MODBUS_ANALOG_INPUT1] = product_register_value[nAddr]+1;//367  131
	}


	else if (m_nCurRow==10)
	{
		if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
		{
			int nValue=product_register_value[MODBUS_TSTAT6_HUM_AVALUE]+1;
			if (nValue<0)
			{
				return;
			}
			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_AVALUE,nValue);
			if (ret>0)
			{
				product_register_value[MODBUS_TSTAT6_HUM_AVALUE]=nValue;
			}

		}
		else 
		{

			int nValue=product_register_value[MODBUS_TSTAT6_HUM_MVALUE]+1;
			if (nValue<0)
			{
				return;
			}
			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_MVALUE,nValue);
			if (ret>0)
			{
				product_register_value[MODBUS_TSTAT6_HUM_MVALUE]=nValue;
			}   
		}
		}
	else if (m_nCurRow==11)
	{
		if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
		{      


			int nValue=product_register_value[MODBUS_TSTAT6_CO2_AVALUE]+1;
			if (nValue<0)
			{
				return;
			}
			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_AVALUE,nValue);
			if (ret>0)
			{
				product_register_value[MODBUS_TSTAT6_CO2_AVALUE]=nValue;
			}   

		}
		else
		{


			int nValue=product_register_value[MODBUS_TSTAT6_CO2_MVALUE]+1;
			if (nValue<0)
			{
				return;
			}
			int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_MVALUE,nValue);
			if (ret>0)
			{
				product_register_value[MODBUS_TSTAT6_CO2_MVALUE]=nValue;
			} 

		}

	}


	Fresh_Grid();
}


void CInputSetDlg::OnBnClickedDownbuttonFor5E()
{
	


	 


		if(m_nCurRow==1&&m_nCurCol==CAL_FIELD)
		{
			int nValue=product_register_value[MODBUS_TEMPRATURE_CHIP]-1;	//101 121
			write_one(g_tstat_id,MODBUS_TEMPRATURE_CHIP,nValue);//101  121
			product_register_value[MODBUS_TEMPRATURE_CHIP] = nValue;	//101  121
		}
		else if (m_nCurRow<10)
		{
			int nAddr =MODBUS_ANALOG_INPUT1+m_nCurRow-2;	//367  131
			write_one(g_tstat_id, nAddr, product_register_value[nAddr]-1);
			product_register_value[MODBUS_ANALOG_INPUT1] = product_register_value[nAddr]-1;//367  131
		}
		else if (m_nCurRow==10)
		{
			if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
			{
			     int nValue=product_register_value[MODBUS_TSTAT6_HUM_AVALUE]-1;
				 if (nValue<0)
				 {
				   return;
				 }
				 int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_AVALUE,nValue);
				 if (ret>0)
				 {
				 product_register_value[MODBUS_TSTAT6_HUM_AVALUE]=nValue;
				 }

			}
			else
			{
				 
				int nValue=product_register_value[MODBUS_TSTAT6_HUM_MVALUE]-1;
				if (nValue<0)
				{
					return;
				}
				int ret=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_MVALUE,nValue);
				if (ret>0)
				{
					product_register_value[MODBUS_TSTAT6_HUM_MVALUE]=nValue;
				}   
			}
		}
		else if (m_nCurRow==11)
		{
			if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
			{      


				int nValue=product_register_value[MODBUS_TSTAT6_CO2_AVALUE]-1;
				if (nValue<0)
				{
					return;
				}
				int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_AVALUE,nValue);
				if (ret>0)
				{
					product_register_value[MODBUS_TSTAT6_CO2_AVALUE]=nValue;
				}   

			}
			else
			{
				 

				int nValue=product_register_value[MODBUS_TSTAT6_CO2_MVALUE]-1;
				if (nValue<0)
				{
					return;
				}
				int ret=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_MVALUE,nValue);
				if (ret>0)
				{
					product_register_value[MODBUS_TSTAT6_CO2_MVALUE]=nValue;
				} 

			}

		}
	Fresh_Grid();
}

void CInputSetDlg::OnCbnSelchangeAmcombo()
{
	
	//309	141	2	Full	W/R	Input auto/ manual enable.
	 
	 if (m_nCurRow==10)
	{
		if(m_AmCombox.GetCurSel()!=product_register_value[MODBUS_TSTAT6_HUM_AM])
		{
		  int nRet=write_one(g_tstat_id,MODBUS_TSTAT6_HUM_AM,m_AmCombox.GetCurSel());
		  if (nRet<0)
		  {
		  AfxMessageBox(_T("Access fail!Retry it later!"));
		  }
		}

	}
	else if(m_nCurRow==11)
	{
		if(m_AmCombox.GetCurSel()!=product_register_value[MODBUS_TSTAT6_CO2_AM])
		{
			int nRet=write_one(g_tstat_id,MODBUS_TSTAT6_CO2_AM,m_AmCombox.GetCurSel());
			if (nRet<0)
			{
				AfxMessageBox(_T("Access fail!Retry it later!"));
			}
		}   
	}
	else

	{
		int	nValue = product_register_value[MODBUS_INPUT_MANU_ENABLE];
		if(m_AmCombox.GetCurSel()==1)
		{ // Manual
			int nFilter = 0x01;
			nFilter = nFilter << (m_nCurRow - 2);
			nValue = nValue | nFilter;
			m_downButton.ShowWindow(SW_HIDE);
			m_upButton.ShowWindow(SW_HIDE);
		}
		else
		{ // Auto
			int nFilter = 0xFE;
			nFilter = nFilter << (m_nCurRow - 2);
			nValue = nValue & nFilter;
		}	
		product_register_value[MODBUS_INPUT_MANU_ENABLE] = nValue;
		int nRet = write_one(g_tstat_id,MODBUS_INPUT_MANU_ENABLE,nValue);//309  141
		if(nRet<0)
		{
			AfxMessageBox(_T("Access fail!Retry it later!"));
		}
	}


	Fresh_Grid();
}


void CInputSetDlg::OnEnKillfocusInvalueeditFor5E()
{
	m_inValueEdit.ShowWindow(SW_HIDE);
	CString strText;
	m_inValueEdit.GetWindowText(strText);
	if(strText.IsEmpty())
		return;
	int nValue=_wtoi(strText);

	if(m_nCurCol==VALUE_FIELD)
	{
		write_one(g_tstat_id,367+m_nCurRow-2,nValue*10);
	}

	Fresh_Grid();
}


// On/Off select on == write reg 1, off == 0
void CInputSetDlg::OnCbnKillfocusValuecomboFor5E()
{
	m_valueCombx.ShowWindow(SW_HIDE);
	int nItem=0;
	nItem=m_valueCombx.GetCurSel();

	if (nItem == 0) // select on
	{
		write_one(g_tstat_id,367+m_nCurRow-2,1);
	}
	else
	{
		write_one(g_tstat_id,367+m_nCurRow-2,0);
	}
	Fresh_Grid();
}

void CInputSetDlg::ClickMsflexgrid5E( int nRow, int nCol, CRect rcCell )
{
	if (nCol == INDEX_FIELD)
	// && nRow == 1) // 序号都不能改
	{
		return;
	}

	//lsc20120828
// 	if (nCol == NAME_FIELD  && nRow > 3)  // 只有2个输入
// 	{
// 		return;
// 	}
// 	if (nRow > 3  && (nCol == VALUE_FIELD  || nCol == RANG_FIELD  || nCol == FUN_FIELD  )) 
// 	{
// 		return;
// 	}
// 	if (nRow == 1 && (nCol == VALUE_FIELD  || nCol == FUN_FIELD || nCol == AM_FIELD || nCol == CAL_FIELD))
// 	{
// 		return;
// 	}

	//////////////////////////////////////////////////////////////////////////
	//
	if (nCol == NAME_FIELD)
	{
		CString strValue = m_FlexGrid.get_TextMatrix(nRow,nCol);
		m_inputNameEdt.MoveWindow(rcCell); //移动到选中格的位置，覆盖
		m_inputNameEdt.ShowWindow(SW_SHOW);

		m_inputNameEdt.BringWindowToTop();
		//m_RangCombox.SelectString(-1,strValue);

		m_inputNameEdt.SetWindowText(strValue);
		m_inputNameEdt.SetFocus(); //获取焦点
	}
	//////////////////////////////////////////////////////////////////////////
	//
	if (nCol == VALUE_FIELD)
	{
		int nValue = multi_register_value[309];
		int nFilter = 0x01;
		nFilter = nFilter << (m_nCurRow - 1);
		CString strValue = m_FlexGrid.get_TextMatrix(nRow,nCol);
		if (nValue & nFilter)
		{
			int nRangeValue = multi_register_value[359+m_nCurRow-2];
			if (nRangeValue == 3)  // 如果是on/off，用combo
			{	
				m_valueCombx.ResetContent();

				m_valueCombx.InsertString(0,_T("On"));
				m_valueCombx.InsertString(1,_T("Off"));

				m_valueCombx.MoveWindow(rcCell); //移动到选中格的位置，覆盖
				m_valueCombx.ShowWindow(SW_SHOW);
				m_valueCombx.BringWindowToTop();
				//m_valueCombx.SelectString(-1,strValue);
				m_valueCombx.SetFocus(); //获取焦点
				m_valueCombx.SetWindowText(strValue);
			}
			else // 如果是值，用edit
			{		
				m_inValueEdit.MoveWindow(rcCell); //移动到选中格的位置，覆盖
				m_inValueEdit.ShowWindow(SW_SHOW);

				m_inValueEdit.BringWindowToTop();
				//m_RangCombox.SelectString(-1,strValue);

				m_inValueEdit.SetWindowText(strValue);
				m_inValueEdit.SetFocus(); //获取焦点

			}		
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//
	CString strAuto=_T("Auto");
	CString strman=_T("Manual");
	if(nCol==AM_FIELD) // A/M
	{
		m_AmCombox.MoveWindow(&rcCell,1); //移动到选中格的位置
		m_AmCombox.BringWindowToTop();
		m_AmCombox.ShowWindow(SW_SHOW);//显示控件
		m_AmCombox.SetFocus(); //获取焦点
		CString strTemp;
		BYTE nValue =BYTE(multi_register_value[309]);
		BYTE nFilter = 0x01;
		nFilter = nFilter << (nRow-1);
		if(nValue & nFilter)
		{
			strTemp = strman;			
		}
		else
		{
			strTemp = strAuto;				
		}
		m_AmCombox.SetWindowText(strTemp);
	}

	//////////////////////////////////////////////////////////////////////////
	if(nCol==CAL_FIELD)    // calibrate buttons
	{	
		CRect rcUp(rcCell.left,rcCell.top,rcCell.left+rcCell.Width()/2,rcCell.bottom);
		CRect rcDown(rcCell.left+rcCell.Width()/2,rcCell.top,rcCell.right,rcCell.bottom);
		m_upButton.MoveWindow(rcUp);
		m_upButton.ShowWindow(SW_SHOW);
		m_upButton.BringWindowToTop();
		m_downButton.MoveWindow(rcDown);
		m_downButton.ShowWindow(SW_SHOW);
		m_downButton.BringWindowToTop();

		if(nRow==1)
		{
			// 			int nRegisterNum = 110;
			// 			if (multi_register_value[111] == 2)
			// 			{
			// 				nRegisterNum = 317;
			// 			}
			// 
			// 			if(multi_register_value[188]==3)
			// 			{
			// 				m_downButton.ShowWindow(SW_HIDE);
			// 				m_upButton.ShowWindow(SW_HIDE);
			// 			}
			// 			else
			// 			{
			m_downButton.ShowWindow(SW_SHOW);
			m_upButton.ShowWindow(SW_SHOW);
			//			}
		}
		else //(nRow==2)
		{
			int nRange = multi_register_value[359+nRow-2];
			if( nRange == 3 || nRange == 5)
			{

				m_downButton.ShowWindow(SW_HIDE);
				m_upButton.ShowWindow(SW_HIDE);
			}
			else
			{
				BYTE nValue =BYTE(multi_register_value[309]);
				BYTE nFilter = 0x01;
				nFilter = nFilter << (nRow-1);
				if(nValue & nFilter) // only sel Auto, can calibrate
				{
					m_downButton.ShowWindow(SW_HIDE);
					m_upButton.ShowWindow(SW_HIDE);
				}
				else
				{
					m_downButton.ShowWindow(SW_SHOW);
					m_upButton.ShowWindow(SW_SHOW);
				}
			}
		}		
	}

	//////////////////////////////////////////////////////////////////////////
	if(nCol==RANG_FIELD) 
	{	
		m_RangCombox.MoveWindow(&rcCell,1); //移动到选中格的位置
		m_RangCombox.BringWindowToTop();
		m_RangCombox.ShowWindow(SW_SHOW);//显示控件
		m_RangCombox.SetFocus(); //获取焦点

		CString strTemp;
		if(nRow == 1) // use 121
		{	
			m_RangCombox.ResetContent();
			for(UINT i=0;i<m_strUnitList.size();i++)
			{
				m_RangCombox.AddString(m_strUnitList.at(i));
			}
			int nValue = multi_register_value[121];
			strTemp = m_strUnitList[nValue];
		}
		else
		{
			m_RangCombox.ResetContent();	
			int nValue = multi_register_value[359+nRow-2];
			if(multi_register_value[121]==0)
			{
				for(int i=0;i<ANALOG_RANG_NUMBER;i++)
				{
					m_RangCombox.AddString(analog_range_0[i]);
				}	
				strTemp = analog_range_0[nValue];
			}
			if(multi_register_value[121]==1)
			{
				for(int i=0;i<ANALOG_RANG_NUMBER;i++)
				{
					m_RangCombox.AddString(analog_range_1[i]);
				}
				strTemp = analog_range_1[nValue];
			}	
		}
		m_RangCombox.SetWindowText(strTemp);
	}

	//////////////////////////////////////////////////////////////////////////
	if(nCol == FUN_FIELD)
	{
		m_inputFinCombox.ResetContent();
		for(int i=0;i<8;i++)
			m_inputFinCombox.AddString(INPUT_FUNS[i]);

		m_inputFinCombox.ShowWindow(SW_SHOW);//显示控件
		m_inputFinCombox.MoveWindow(rcCell); //移动到选中格的位置，覆盖
		m_inputFinCombox.BringWindowToTop();
		//m_inputFinCombox.SelectString(-1,strValue);
		m_inputFinCombox.SetFocus(); //获取焦点

		int nValue=product_register_value[298+nRow-2];
		CString strTemp(INPUT_FUNS[nValue]);
		m_inputFinCombox.SetWindowText(strTemp);

	}

	//////////////////////////////////////////////////////////////////////////
	if(nCol == CUST_FIELD)
	{
		if(nRow==2&&product_register_value[MODBUS_ANALOG_IN1]==4)
		{
			m_customBtn.ShowWindow(SW_SHOW);
			m_customBtn.MoveWindow(rcCell); //移动到选中格的位置，覆盖
			m_customBtn.BringWindowToTop();
			m_customBtn.SetFocus(); //获取焦点
		}
		if(nRow==3&&multi_register_value[189]==4)
		{
			m_customBtn.ShowWindow(SW_SHOW);
			m_customBtn.MoveWindow(rcCell); //移动到选中格的位置，覆盖
			m_customBtn.BringWindowToTop();
			m_customBtn.SetFocus(); //获取焦点
		}
	}
	
}
//Annul by Fance, merge this function.
//void CInputSetDlg::InitGrid5EnoTSTAT6()
//{
//
//	m_FlexGrid.put_TextMatrix(1,NAME_FIELD,g_strSensorName);
//	m_FlexGrid.put_TextMatrix(2,NAME_FIELD,g_strInName1);
//	m_FlexGrid.put_TextMatrix(3,NAME_FIELD,g_strInName2);
//	m_FlexGrid.put_TextMatrix(4,NAME_FIELD,g_strInName3);
//	m_FlexGrid.put_TextMatrix(5,NAME_FIELD,g_strInName4);
//	m_FlexGrid.put_TextMatrix(6,NAME_FIELD,g_strInName5);
//	m_FlexGrid.put_TextMatrix(7,NAME_FIELD,g_strInName6);
//	m_FlexGrid.put_TextMatrix(8,NAME_FIELD,g_strInName7);
//	m_FlexGrid.put_TextMatrix(9,NAME_FIELD,g_strInName8);
//
//	CString strUnit=GetTempUnit();
//	CString strTemp;	
//	int nValue=0;
//	CString strAuto=_T("Auto");
//	CString strman=_T("Manual");
//
//
//	for(int i=1;i<=9;i++)
//	{	
//		CString strIndex;
//		strIndex.Format(_T("%d"), i);
//		m_FlexGrid.put_TextMatrix(i,INDEX_FIELD,strIndex);
//
//		for(int k=0;k<=6;k++)
//		{
//			m_FlexGrid.put_ColAlignment(k,4);
//			if (i%2==1)
//			{
//				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(RGB(255,255,255));
//			}
//			else
//			{
//				m_FlexGrid.put_Row(i);m_FlexGrid.put_Col(k);m_FlexGrid.put_CellBackColor(COLOR_CELL);
//			}
//		}
//
//		CString strTemp;
//		m_fFirmwareVersion = get_curtstat_version();
//		if(m_fFirmwareVersion>33.3)
//		{
//			float fValue;
//
//			// Row1
//			if(i==1)
//			{
//				strTemp.Format(_T("%.1f"),multi_register_value[216]/10.0);
//				strTemp=strTemp+strUnit;
//				m_FlexGrid.put_TextMatrix(1,VALUE_FIELD,strTemp);
//				m_FlexGrid.put_TextMatrix(1,AM_FIELD,NO_APPLICATION);
//				m_FlexGrid.put_TextMatrix(1,CAL_FIELD,_T("Adjust..."));
//				m_FlexGrid.put_TextMatrix(1,RANG_FIELD,strUnit);
//				m_FlexGrid.put_TextMatrix(1,FUN_FIELD,NO_APPLICATION);
//				m_FlexGrid.put_TextMatrix(1,CUST_FIELD,NO_APPLICATION);
//				continue;
//			}
//
//			//////////////////////////////////////////////////////////////////////////
//			// column 1  Value
//
//			CString strValueUnit=GetTempUnit(multi_register_value[359+i-2], 1);
//			{
//				if(multi_register_value[359+i-2]==1)
//				{				
//					fValue=float(multi_register_value[367+i-2]/10.0);
//					strTemp.Format(_T("%.1f"),fValue);	
//
//					strTemp +=strValueUnit;
//				}
//				else if(multi_register_value[359+i-2]==3 || multi_register_value[359+i-2]==5) // On/Off or Off/On ==1 On ==0 Off
//				{						
//					int nValue=(multi_register_value[367+i-2]);
//					if (nValue == 1)
//					{
//						strTemp = _T("On");
//					}
//					else
//					{
//						strTemp = _T("Off");
//					}					
//				}
//				else if (multi_register_value[359+i-2]==4 )  // custom sensor
//				{					
//					fValue=float(multi_register_value[367+i-2]/10.0);
//					strTemp.Format(_T("%.1f"), (float)fValue/10.0);	
//					strTemp +=strValueUnit;
//				}
//				else if(multi_register_value[359+i-2]==2)
//				{
//					nValue=multi_register_value[367+i-2];
//					strTemp.Format(_T("%d%%"), nValue);
//				}
//				else
//				{
//					//strTemp.Format(_T("%d"),multi_register_value[367+i-2]);
//					strTemp=_T("UNUSED");
//
//				}						
//				m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);
//				strTemp.Empty();
//			}
//
//
//			// column 2  Auto/Manual // 只有IN1，2才有,internal sensor 没有，大于1 // 
//			if(i>1)//( i== 2 || i == 3)
//			{
//				nValue=multi_register_value[309];
//				BYTE bFilter=0x01;
//				bFilter = bFilter<< (i-2);
//				if((nValue & bFilter))
//				{
//					m_FlexGrid.put_TextMatrix(i,AM_FIELD,strman);			
//				}
//				else
//				{
//					m_FlexGrid.put_TextMatrix(i,AM_FIELD,strAuto);				
//				}
//			}
//			else
//			{
//				m_FlexGrid.put_Col(AM_FIELD);
//				m_FlexGrid.put_Row(i);
//				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
//			}
//
//			// column 3  Calibrate			
//			{
//				m_FlexGrid.put_TextMatrix(i,CAL_FIELD,_T("Adjust..."));				
//			}
//
//			// column 4  Range 
//			//if( i >= 2 )
//			{
//				int nItem=multi_register_value[359+i-2];
//				if(nItem>=0 && nItem<=5)
//				{
//					strTemp=analog_range_0[nItem];
//				}
//				m_FlexGrid.put_TextMatrix(i,RANG_FIELD,strTemp);
//			}
//
//
//			// column 5 Function 只有IN1，2才有
//			if(i== 2 || i==3)
//			{
//				nValue=multi_register_value[298+i-2];
//				strTemp=INPUT_FUNS[nValue];
//				m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
//			}
//			else
//			{
//				m_FlexGrid.put_Col(FUN_FIELD);
//				m_FlexGrid.put_Row(i);
//				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
//			}
//
//			// column 6 custom tables
//			if( i== 2 || i==3)
//			//if(i!=9)
//			{
//				if(multi_register_value[188]==4)
//				{
//					m_FlexGrid.put_TextMatrix(i,CUST_FIELD,_T("Custom..."));
//				}
//				else
//				{
//					m_customBtn.ShowWindow(SW_HIDE);
//					m_FlexGrid.put_TextMatrix(i,CUST_FIELD,NO_APPLICATION);
//				}
//			}
//			else
//			{
//				m_FlexGrid.put_Col(CUST_FIELD);
//				m_FlexGrid.put_Row(i);
//				m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
//			}
//		}
//		else  // another lower version of software
//		{
//			int nValue;
//
//			if(multi_register_value[341+i-2]==1)
//			{
//				nValue=(int)(multi_register_value[349+i-2]/10.0);
//				strTemp.Format(_T("%.1f"),nValue);
//			}
//			else
//			{
//				strTemp.Format(_T("%d"),multi_register_value[349+i-2]);
//			}
//			m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);
//
//			strTemp.Empty();
//			int nItem=multi_register_value[341+i-2];
//			if(nItem>=0&&nItem<=4)
//			{
//				strTemp=analog_range_0[nItem];					
//			}
//			m_FlexGrid.put_TextMatrix(i,AM_FIELD,strTemp);
//		}
//	}	
//}

void CInputSetDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(b_is_fresh==true)
	{
		AfxMessageBox(_T("It's refreshing ,can not exit at this time!"));
		return;
	}

	CDialog::OnClose();
}


void CInputSetDlg::OnEnKillfocusFilter()
{
	UpdateData(TRUE);  CString str;
	int ret=write_one(g_tstat_id,141+m_nCurRow,m_filterValue);
	if (ret>0)
	{
	  product_register_value[141+m_nCurRow]=m_filterValue;
	  
	  str.Format(_T("%d"),m_filterValue);
	  m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,str);

	} 
	else
	{
	   m_filterValue=product_register_value[141+m_nCurRow];
	   str.Format(_T("%d"),m_filterValue);
	   m_FlexGrid.put_TextMatrix(m_nCurRow,m_nCurCol,str);
	}
	UpdateData(FALSE);
}
