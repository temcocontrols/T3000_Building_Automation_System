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
#define RANG_FIELD 5
#define FUN_FIELD 6
#define CUST_FIELD 7

#define TOTAL_COLS 8
#define TOTAL_ROWS 7
// CInputSetDlg dialog

#define ANALOG_RANG_NUMBER 6
CString analog_range_0[ANALOG_RANG_NUMBER]={_T("UNUSED"),_T("10KC Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};

CString analog_range_1[ANALOG_RANG_NUMBER]={_T("UNUSER"),_T("10KF Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};
CString INPUT_FUNS[7]={_T("Normal"),_T("Freeze Protect"),_T("Occupancy Sensor"),_T("Sweep Off"),_T("Clock"),_T("Changeover Mode"),_T("Outside Temp")};

//CString g_str5ERange2[5]={_T("Raw"),_T("Thermistor"),_T("On/Off"),_T("Off/On"),_T("%")};
IMPLEMENT_DYNAMIC(CInputSetDlg, CDialog)

CInputSetDlg::CInputSetDlg(CWnd* pParent /*=NULL*/)
: CDialog(CInputSetDlg::IDD, pParent)
{
	m_nCurRow=m_nCurCol=0;
}

CInputSetDlg::~CInputSetDlg()
{

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
END_MESSAGE_MAP()


// CInputSetDlg message handlers

void CInputSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
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
	m_nModel=multi_register_value[MODBUS_PRODUCT_MODEL];
	switch (m_nModel)
	{
		case 3:m_inRows=4;break; // 5B
		case 2:
		case 1:m_inRows=3;break; // 5A
		case 4:m_inRows=5;break; // 5C
		case PM_TSTAT7:
		case PM_PRESSURE:
		case 12:m_inRows=5;break; // 5D 同 TStat7
		case PM_TSTAT6:	
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

	m_FlexGrid.put_TextMatrix(0,RANG_FIELD,_T("Range"));
	m_FlexGrid.put_ColWidth(RANG_FIELD,1200);

	m_FlexGrid.put_TextMatrix(0,FUN_FIELD,_T("Function"));
	m_FlexGrid.put_ColWidth(FUN_FIELD,1400);

	m_FlexGrid.put_TextMatrix(0,CUST_FIELD,_T("Custom Tables"));
	m_FlexGrid.put_ColWidth(CUST_FIELD,1100);

 
	UINT uint_temp=GetOEMCP();
	//get system is for chinese or english
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
	if (m_nModel == 16 || m_nModel == PM_TSTAT6 ) // 5E与其他的不同
	{
		Init5EGrid();
		return TRUE;
	}
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
	if (m_nModel == 16 || m_nModel == PM_TSTAT6 )
	{
		Init5EGrid();
		return;
	}

	CString strUnit=GetTempUnit();
	CString strTemp;	
	int nValue=0;
	CString strAuto=_T("Auto");
	CString strman=_T("Manual");
//row 1:	
	if(m_FlexGrid.get_Row()>=1)
	{
		strTemp.Format(_T("%.1f"),multi_register_value[101]/10.0);
		strTemp=strTemp+strUnit;
		m_FlexGrid.put_TextMatrix(1,VALUE_FIELD,strTemp);
		m_FlexGrid.put_TextMatrix(1,AM_FIELD,NO_APPLICATION);
		m_FlexGrid.put_TextMatrix(1,CAL_FIELD,_T("Adjust..."));
		m_FlexGrid.put_TextMatrix(1,RANG_FIELD,strUnit);
		m_FlexGrid.put_TextMatrix(1,FUN_FIELD,NO_APPLICATION);
		m_FlexGrid.put_TextMatrix(1,CUST_FIELD,NO_APPLICATION);
	}
	//SetCellAlignment
	//m_FlexGrid.SetColAlignment(1,4);
	//m_FlexGrid.SetColAlignment=4;
	//m_FlexGrid.put_ColAlignment(1,4);

//row 2:
	strTemp.Empty();
	strUnit=GetTempUnit(multi_register_value[188], 1);

	if(multi_register_value[188]==4||multi_register_value[188]==1)
	{	
		strTemp.Format(_T("%.1f"),(float)multi_register_value[180]/10);
		strTemp=strTemp+strUnit;
	}
	if(multi_register_value[188]==0)
	{
		strTemp.Format(_T("%d"),multi_register_value[180]);
		strTemp=strTemp;
	}
	if(multi_register_value[188]==2)
	{
		strTemp.Format(_T("%d"),multi_register_value[180]);
		strTemp=strTemp+_T("%");
	}
	if(multi_register_value[188]==3 || multi_register_value[188]==5)
	{
		if(multi_register_value[180]==0)
			strTemp=_T("Off");
		if(multi_register_value[180]==1)
			strTemp=_T("On");
	}
	if (multi_register_value[188]==6)
	{
		strTemp=_T("UNUSED");
	}
	if(m_FlexGrid.get_Row()>=2)
	{
		m_FlexGrid.put_TextMatrix(2,VALUE_FIELD,strTemp);
		//int nValue=0;
		//CString strAuto=_T("Auto");
		//CString strman=_T("Manual");
		nValue=multi_register_value[309];
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

		nValue=multi_register_value[188];
		strUnit=GetTempUnit(nValue, 2);
		if(nValue>=0)
		{
			//
			if(multi_register_value[121]==0)
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
		nValue=multi_register_value[298];
		strTemp=INPUT_FUNS[nValue];
		m_FlexGrid.put_TextMatrix(2,FUN_FIELD,strTemp);

		if(multi_register_value[188]==4)
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
		if(multi_register_value[189]==4||multi_register_value[189]==1)
		{
			strTemp.Format(_T("%.1f"),(float)multi_register_value[181]/10);	
			strTemp=strTemp+strUnit;
		}
		if (multi_register_value[189]==0||multi_register_value[189]==2)
		{
			strTemp.Format(_T("%d"),multi_register_value[181]);
			strTemp=strTemp+strUnit;
		}
		if(multi_register_value[189]==3 || multi_register_value[189]==5)
		{
			if(multi_register_value[181]==0)
				strTemp=_T("Off");
			if(multi_register_value[181]==1)
				strTemp=_T("On");
		}
		m_FlexGrid.put_TextMatrix(3,VALUE_FIELD,strTemp);

		nValue=multi_register_value[309];
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
		nValue=multi_register_value[189];
		if(nValue>=0)
		{
			//
			if(multi_register_value[121]==0)
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
		nValue=multi_register_value[299];
		strTemp=INPUT_FUNS[nValue];
		m_FlexGrid.put_TextMatrix(3,FUN_FIELD,strTemp);

		if(multi_register_value[189]==4)
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
		if (multi_register_value[190]==0)
		{
			CString strTemp2;
			if (multi_register_value[311]==0)
			{
				strTemp=_T("Off");
			}else
			{
				strTemp=_T("On");
			}
			strTemp2=_T("On/Off");
			m_FlexGrid.put_TextMatrix(4,RANG_FIELD,strTemp2);
		}
		if (multi_register_value[190]==1)
		{
			CString strTemp2;
			if (multi_register_value[311]==1)
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

		nValue=multi_register_value[309];
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
		nValue=multi_register_value[300];
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

	Fresh_Grid();
}

void CInputSetDlg::OnBnClickedDownbutton()
{
	if (m_nModel == 16 || m_nModel == PM_TSTAT6 ) // for 5E
	{
		OnBnClickedDownbuttonFor5E();

		Fresh_Grid();
		return;
	}

	Fresh_Grid();
}

void CInputSetDlg::OnCbnKillfocusAmcombo()
{
	m_AmCombox.ShowWindow(SW_HIDE);
}

void CInputSetDlg::OnCbnSelchangeAmcombo()
{
	if (m_nModel == 16 || m_nModel == PM_TSTAT6 )
	{
		OnCbnSelchangeAmcomboFor5E();
		return;
	}
	
	

}
void CInputSetDlg::add_analog_rang(int i)
{

}
void CInputSetDlg::OnCbnSelchangeInputfuncombo()
{
	if(m_nCurRow==2&&m_nCurCol==FUN_FIELD)
	{
		int nchange=0;
		nchange=m_inputFinCombox.GetCurSel();
		write_one(g_tstat_id,298,nchange);
	}


}

void CInputSetDlg::OnCbnKillfocusInputfuncombo()
{
	m_inputFinCombox.ShowWindow(SW_HIDE);
}

void CInputSetDlg::OnBnClickedCustombutton()
{

}

void CInputSetDlg::OnBnKillfocusCustombutton()
{
	//m_customBtn.ShowWindow(SW_HIDE);
}

void CInputSetDlg::OnBnClickedExit()
{
	CDialog::OnOK();
}

void CInputSetDlg::OnEnKillfocusInvalueedit()
{
	if(m_nModel == 16 || m_nModel == PM_TSTAT6)
	{
		OnEnKillfocusInvalueeditFor5E();
		return;
	}


}

void CInputSetDlg::OnCbnKillfocusValuecombo()
{
	//if (m_nModel == 16 || m_nModel == PM_TSTAT6)alex-zhang
	//{
	//	OnCbnKillfocusValuecombo();
	//	return;
	//}
	m_valueCombx.ShowWindow(SW_HIDE);
	int nItem=0;
	nItem=m_valueCombx.GetCurSel();

}

void CInputSetDlg::OnBnClickedRefreshbutton()
{
#if 1
	int i;




#endif
	Fresh_Grid();
}

void CInputSetDlg::OnEnKillfocusInputnameedit()
{
	CString strText;
	m_inputNameEdt.GetWindowText(strText);
	m_inputNameEdt.ShowWindow(SW_HIDE);
	int lRow = m_FlexGrid.get_RowSel();//
	int lCol = m_FlexGrid.get_ColSel(); //
	

}


void CInputSetDlg::Init5EGrid()
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
				strTemp.Format(_T("%.1f"),multi_register_value[216]/10.0);
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
				
			CString strValueUnit=GetTempUnit(multi_register_value[359+i-2], 1);
			{
				if(multi_register_value[359+i-2]==1)
				{				
					fValue=float(multi_register_value[367+i-2]/10.0);
					strTemp.Format(_T("%.1f"),fValue);	
					
					strTemp +=strValueUnit;
				}
				else if(multi_register_value[359+i-2]==3 || multi_register_value[359+i-2]==5) // On/Off or Off/On ==1 On ==0 Off
				{						
					int nValue=(multi_register_value[367+i-2]);
					if (nValue == 1)
					{
						strTemp = _T("On");
					}
					else
					{
						strTemp = _T("Off");
					}					
				}
				else if (multi_register_value[359+i-2]==4 )  // custom sensor
				{					
					fValue=float(multi_register_value[367+i-2]/10.0);
					strTemp.Format(_T("%.1f"), (float)fValue/10.0);	
					strTemp +=strValueUnit;
				}
				else if(multi_register_value[359+i-2]==2)
				{
					nValue=multi_register_value[367+i-2];
					strTemp.Format(_T("%d%%"), nValue);
				}
				else if (multi_register_value[359+i-2]==6)
				{
					
				}
				else
				{
				//	strTemp.Format(_T("%d"),multi_register_value[367+i-2]);
					strTemp=_T("UNUSED");
				}						
				m_FlexGrid.put_TextMatrix(i,VALUE_FIELD,strTemp);
				strTemp.Empty();
			}


			
			if(i>1)//( i== 2 || i == 3)
			{
				nValue=multi_register_value[309];
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
				int nItem=multi_register_value[359+i-2];
				if(nItem>=0 && nItem<ANALOG_RANG_NUMBER)//
				{
					strTemp=analog_range_0[nItem];
				}
				m_FlexGrid.put_TextMatrix(i,RANG_FIELD,strTemp);
			}
		

			
			if(i== 2 || i==3)
			{
				nValue=multi_register_value[298+i-2];
				if (nValue>=0&&nValue<7)//tstat6
				{		
					strTemp=INPUT_FUNS[nValue];
					m_FlexGrid.put_TextMatrix(i,FUN_FIELD,strTemp);
				}

			}
			else
			{
				m_FlexGrid.put_Col(FUN_FIELD);
				m_FlexGrid.put_Row(i);
				//m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
			}

			// column 6 custom tables
			if( i== 2 || i==3)
			{
				if(multi_register_value[188]==4)
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
				//m_FlexGrid.put_CellBackColor(DISABLE_COLOR_CELL);				
			}
		}
		else  // another lower version of software
		{
			int nValue;

			if(multi_register_value[341+i-2]==1)
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


void CInputSetDlg::OnClick5EGrid(int nRow, int nCol, CRect rcCell)
{


	Fresh_Grid();
}

void CInputSetDlg::OnBnClickedDownbuttonFor5E()
{

}

void CInputSetDlg::OnCbnSelchangeAmcomboFor5E()
{
	int nValue = multi_register_value[309];


}

void CInputSetDlg::OnEnKillfocusInvalueeditFor5E()
{
	m_inValueEdit.ShowWindow(SW_HIDE);
	CString strText;
	m_inValueEdit.GetWindowText(strText);
	if(strText.IsEmpty())
		return;

}


// On/Off select on == write reg 1, off == 0
void CInputSetDlg::OnCbnKillfocusValuecomboFor5E()
{

	Fresh_Grid();
}

void CInputSetDlg::OnCbnSelchangeRangCombo()
{

}

void CInputSetDlg::OnCbnKillfocusRangCombo()
{

}

void CInputSetDlg::OnBnClickedUpbutton()
{

}