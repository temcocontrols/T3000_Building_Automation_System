// ModbusToBacnetRouterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ModbusToBacnetRouterDlg.h"
#include "afxdialogex.h"


// CModbusToBacnetRouterDlg dialog

IMPLEMENT_DYNAMIC(CModbusToBacnetRouterDlg, CDialogEx)

CModbusToBacnetRouterDlg::CModbusToBacnetRouterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MODBUS_BACNET_ROUTER, pParent)
{

}

CModbusToBacnetRouterDlg::~CModbusToBacnetRouterDlg()
{
}

void CModbusToBacnetRouterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_datalist);
	DDX_Control(pDX, IDC_COMBO_PRODUCTNAME, m_combox_productname);
	DDX_Control(pDX, IDC_COMBO_REV, m_combox_rev);
}


BEGIN_MESSAGE_MAP(CModbusToBacnetRouterDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BLXFILE, &CModbusToBacnetRouterDlg::OnBnClickedButtonBlxfile)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CModbusToBacnetRouterDlg::OnNMClickList1)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED, Fresh_Input_Item)
END_MESSAGE_MAP()


// CModbusToBacnetRouterDlg message handlers


BOOL CModbusToBacnetRouterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	g_hwnd_now = this->m_hWnd;
	m_vecYesNo.push_back(L"NO");
	m_vecYesNo.push_back(L"YES");
	m_vecDataRW.push_back(L"R_Only");
	m_vecDataRW.push_back(L"R/W");

	m_vecVariableType.push_back(L"_4_Holding_Register");
	m_vecVariableType.push_back(L"_0_Coil_Bit");
	m_vecVariableType.push_back(L"_3_Input_Register");
	m_vecVariableType.push_back(L"_1_Input_Bit");
	
	
	


	m_GroupUnits.push_back(L"Other");
	m_GroupUnits.push_back(L"Volumetric_Flow");
	m_GroupUnits.push_back(L"Volume");
	m_GroupUnits.push_back(L"Velocity");
	m_GroupUnits.push_back(L"Torque");
	m_GroupUnits.push_back(L"Time");
	m_GroupUnits.push_back(L"Temperature");
	m_GroupUnits.push_back(L"Pressure");
	m_GroupUnits.push_back(L"Power");
	m_GroupUnits.push_back(L"Mass_Flow");
	m_GroupUnits.push_back(L"Mass");
	m_GroupUnits.push_back(L"Light");
	m_GroupUnits.push_back(L"Length");
	m_GroupUnits.push_back(L"Humidity");
	m_GroupUnits.push_back(L"Frequency");
	m_GroupUnits.push_back(L"Force");
	m_GroupUnits.push_back(L"Entropy");
	m_GroupUnits.push_back(L"Enthalpy");
	m_GroupUnits.push_back(L"Energy");
	m_GroupUnits.push_back(L"Electrical");
	m_GroupUnits.push_back(L"Currency");
	m_GroupUnits.push_back(L"Area");
	m_GroupUnits.push_back(L"Acceleration");
	m_GroupUnits.push_back(L"Bacnet_Units");

	 
	
	
	
	

	m_vecDataRW.push_back(L"R_Only");
	m_vecDataRW.push_back(L"R_W");


	for (int i = 0;i < 20;i++)
	{
		ModbusToBacnetRouter tmpnode;
		tmpnode.Poll_YES_NO=L"YES";
		tmpnode.Bacnet_Object_Name.Format(_T("BacnetObjectName%d"), i + 1);
		tmpnode.Modbus_Variable_Type = _T("_4_Holding_Register");
		tmpnode.From1_To_65536 = _T("1");
		tmpnode.Data_Format = _T("16 Bit Unsigned Interger");
		tmpnode.Bit_1 = _T("None");
		tmpnode.Low_Actual = _T("0");
		tmpnode.High_Actual = _T("1");
		tmpnode.Low_Scale = _T("0");
		tmpnode.High_Scale = _T("1");
		tmpnode.Read_Only_Or_RW = _T("R");
		tmpnode.Bacnet_Type = _T("AI");
		tmpnode.Bacnet_Object_Description.Format(_T("Banet Object Description%d"), i + 1);
		tmpnode.COV_Increment = _T("0");
		tmpnode.Unit_Group = _T("Other");
		tmpnode.Unit_Value = _T("NO_UNITS");
		tmpnode.Grouping_YES_NO = _T("YES");
		tmpnode.Update_On_Reconnect = _T("NO");
		m_vecMd2BT.push_back(tmpnode);
	}
	
	
	m_vecMd2BT[0].Bacnet_Object_Name = _T("Serial Number");
	m_vecMd2BT[0].From1_To_65536 = _T("1");
	m_vecMd2BT[0].Data_Format = _T("32 Bit Unsigned Integer HI_LO");
	m_vecMd2BT[0].Bacnet_Object_Description = _T("Serial Number");

	m_vecMd2BT[1].Bacnet_Object_Name = _T("Firmware Version");
	m_vecMd2BT[1].From1_To_65536 = _T("5");
	m_vecMd2BT[1].Data_Format = _T("16 Bit Unsigned Integer HI_LO");
	m_vecMd2BT[1].Bacnet_Object_Description = _T("Serial Number");

	m_vecMd2BT[2].Bacnet_Object_Name = _T("Address");
	m_vecMd2BT[2].From1_To_65536 = _T("7");
	m_vecMd2BT[2].Data_Format = _T("16 Bit Unsigned Integer HI_LO");
	m_vecMd2BT[2].Bacnet_Object_Description = _T("Software Version");

	m_vecMd2BT[3].Bacnet_Object_Name = _T("Product Model");
	m_vecMd2BT[3].From1_To_65536 = _T("8");
	m_vecMd2BT[3].Data_Format = _T("16 Bit Unsigned Integer HI_LO");
	m_vecMd2BT[3].Bacnet_Object_Description = _T("Modbus device address");


	m_vecMd2BT[4].Bacnet_Object_Name = _T("Hardware Revision");
	m_vecMd2BT[4].From1_To_65536 = _T("9");
	m_vecMd2BT[4].Data_Format = _T("16 Bit Unsigned Integer HI_LO");
	m_vecMd2BT[4].Bacnet_Object_Description = _T("Product model");

	m_datalist.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_datalist.SetExtendedStyle(m_datalist.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
	m_datalist.InsertColumn(0, _T("Item"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_datalist.InsertColumn(1, _T("Poll YES / NO"), 120, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(2, _T("Bacnet Object Name"), 200, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(3, _T("Modbus Variable Type"), 200, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(4, _T("1 to 65536"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(5, _T("Data Format	"), 200, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(6, _T("Bit #"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(7, _T("Low Actual"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(8, _T("High Actual"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(9, _T("Low Scale"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(10, _T("High Scale"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(11, _T("R/W"), 60, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(12, _T("Bacnet Type"), 120, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(13, _T("Bacnet Object Description"), 200, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(14, _T("COV Increment"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(15, _T("Unit Group"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(16, _T("Unit Value"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(17, _T("Grouping YES / NO"), 200, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(18, _T("Update On Reconnect"), 200, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
			
	for (int i = 0;i < m_vecMd2BT.size();i++)
	{
		CString strTmp;
		strTmp.Format(_T("%d"), i + 1);
		m_datalist.InsertItem(i, strTmp);

		m_datalist.SetItemText(i, 1, m_vecMd2BT[i].Poll_YES_NO);
		if (ListCtrlEx::ComboBox == m_datalist.GetColumnType(1))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa = 0;aaa <m_vecYesNo.size() ;aaa++)
			{
				strlist.push_back(m_vecYesNo[aaa]);
			}
			m_datalist.SetCellStringList(i, 1, strlist);
		}

		m_datalist.SetItemText(i, 2, m_vecMd2BT[i].Bacnet_Object_Name);
		m_datalist.SetItemText(i, 3, m_vecMd2BT[i].Modbus_Variable_Type);
		if (ListCtrlEx::ComboBox == m_datalist.GetColumnType(3))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa = 0;aaa < m_vecVariableType.size();aaa++)
			{
				strlist.push_back(m_vecVariableType[aaa]);
			}
			m_datalist.SetCellStringList(i, 3, strlist);
		}

		m_datalist.SetItemText(i, 4, m_vecMd2BT[i].From1_To_65536 );
		 
		m_datalist.SetItemText(i, 5, m_vecMd2BT[i].Data_Format );
	 
		m_datalist.SetItemText(i, 6, m_vecMd2BT[i].Bit_1);
		m_datalist.SetItemText(i, 7, m_vecMd2BT[i].Low_Actual);
		m_datalist.SetItemText(i, 8, m_vecMd2BT[i].High_Actual);
		m_datalist.SetItemText(i, 9, m_vecMd2BT[i].Low_Scale );
		m_datalist.SetItemText(i, 10, m_vecMd2BT[i].High_Scale);
		m_datalist.SetItemText(i, 11, m_vecMd2BT[i].Read_Only_Or_RW);
		//m_vecDataRW= GetRWByVariableDataFormat(m_vecMd2BT[i].Modbus_Variable_Type, m_vecMd2BT[i].Data_Format);
		/*if (ListCtrlEx::ComboBox == m_datalist.GetColumnType(11))
			{
				ListCtrlEx::CStrList strlist;
				for (int aaa = 0;aaa < m_vecDataRW.size();aaa++)
				{
					strlist.push_back(m_vecDataRW[aaa]);
				}
				m_datalist.SetCellStringList(i, 5, strlist);
			}*/
		/*m_vecBacnetType = GetBacnetType(m_vecMd2BT[i].Modbus_Variable_Type,m_vecMd2BT[i].Data_Format,m_vecMd2BT[i].Read_Only_Or_RW);
		if (ListCtrlEx::ComboBox == m_datalist.GetColumnType(12))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa = 0;aaa < m_vecBacnetType.size();aaa++)
			{
				strlist.push_back(m_vecBacnetType[aaa]);
			}
			m_datalist.SetCellStringList(i, 12, strlist);
		}*/
		m_datalist.SetItemText(i, 12, m_vecMd2BT[i].Bacnet_Type);
		
		m_datalist.SetItemText(i, 13, m_vecMd2BT[i].Bacnet_Object_Description);
		m_datalist.SetItemText(i, 14, m_vecMd2BT[i].COV_Increment);

		m_datalist.SetItemText(i, 15, m_vecMd2BT[i].Unit_Group);
		if (ListCtrlEx::ComboBox == m_datalist.GetColumnType(15))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa = 0;aaa < m_GroupUnits.size();aaa++)
			{
				strlist.push_back(m_GroupUnits[aaa]);
			}
			m_datalist.SetCellStringList(i, 15, strlist);
		}
		m_datalist.SetItemText(i, 16, m_vecMd2BT[i].Unit_Value);
		/*m_UnitValue = GetUnitValueByGroupUnit(m_vecMd2BT[i].Unit_Group);
		if(ListCtrlEx::ComboBox == m_datalist.GetColumnType(16))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa = 0;aaa < m_UnitValue.size();aaa++)
			{
				strlist.push_back(m_UnitValue[aaa]);
			}
			m_datalist.SetCellStringList(i, 16, strlist);
		}*/

		m_datalist.SetItemText(i, 17, m_vecMd2BT[i].Grouping_YES_NO);

		if (ListCtrlEx::ComboBox == m_datalist.GetColumnType(17))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa = 0;aaa < m_vecYesNo.size();aaa++)
			{
				strlist.push_back(m_vecYesNo[aaa]);
			}
			m_datalist.SetCellStringList(i, 17, strlist);
		}
		m_datalist.SetItemText(i, 18, m_vecMd2BT[i].Update_On_Reconnect);
		if (ListCtrlEx::ComboBox == m_datalist.GetColumnType(18))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa = 0;aaa < m_vecYesNo.size();aaa++)
			{
				strlist.push_back(m_vecYesNo[aaa]);
			}
			m_datalist.SetCellStringList(i, 18, strlist);
		}
	}
			
	m_combox_productname.SetWindowTextW(_T("PS100"));
	m_combox_rev.SetWindowTextW(_T("A"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

vector<CString> CModbusToBacnetRouterDlg::GetUnitValueByGroupUnit(CString strGroupUnit)
{
	vector<CString> vecTmp;
	if (strGroupUnit.CompareNoCase(L"Acceleration")==0)
	{
		vecTmp.clear();
		vecTmp.push_back(L"METERS_PER_SECOND_PER_SECOND");
	}
	else if (strGroupUnit.CompareNoCase(L"Area") == 0)
	{
		vecTmp.clear();
		vecTmp.push_back(L"SQUARE_METERS");
		vecTmp.push_back(L"SQUARE_FEET");
		vecTmp.push_back(L"SQUARE_INCHES");
		vecTmp.push_back(L"SQUARE_CENTIMETERS");
	}
	else if (strGroupUnit.CompareNoCase(L"Currency") == 0)
	{
		vecTmp.clear();
		vecTmp.push_back(L"CURRENCY1");
		vecTmp.push_back(L"CURRENCY2");
		vecTmp.push_back(L"CURRENCY3");
		vecTmp.push_back(L"CURRENCY4");
		vecTmp.push_back(L"CURRENCY5");
		vecTmp.push_back(L"CURRENCY6");
		vecTmp.push_back(L"CURRENCY7");
		vecTmp.push_back(L"CURRENCY8");
		vecTmp.push_back(L"CURRENCY9");
		vecTmp.push_back(L"CURRENCY10");
	}
	else if (strGroupUnit.CompareNoCase(L"Electrical") == 0)
	{
		vecTmp.clear();
		vecTmp.push_back(L"MILLIAMPERES");
		vecTmp.push_back(L"AMPERES");
		vecTmp.push_back(L"OHMS");
		vecTmp.push_back(L"VOLTS");
		vecTmp.push_back(L"KILOVOLTS");
		vecTmp.push_back(L"MEGAVOLTS");
		vecTmp.push_back(L"VOLT_AMPERES");
		vecTmp.push_back(L"KILOVOLT_AMPERES");
		vecTmp.push_back(L"MEGAVOLT_AMPERES");
		vecTmp.push_back(L"VOLT_AMPERES_REACTIVE");

		vecTmp.push_back(L"MEGAVOLT_AMPERES_REACTIVE");
		vecTmp.push_back(L"DEGREES_PHASE");
		vecTmp.push_back(L"POWER_FACTOR");
		vecTmp.push_back(L"KILOHMS");
		vecTmp.push_back(L"MEGOHMS");
		vecTmp.push_back(L"MILLIVOLTS");
		vecTmp.push_back(L"MILLIOHMS");
		vecTmp.push_back(L"AMPERES_PER_METER");
		vecTmp.push_back(L"AMPERES_PER_SQUARE_METER");
		vecTmp.push_back(L"AMPERES_SQUARE_METERS");

		vecTmp.push_back(L"FARADS");
		vecTmp.push_back(L"HENRYS");
		vecTmp.push_back(L"OHM_METERS");
		vecTmp.push_back(L"SIEMENS");
		vecTmp.push_back(L"SIEMENS_PER_METER");
		vecTmp.push_back(L"TESLAS");
		vecTmp.push_back(L"VOLTS_PER_DEGREE_KELVIN");
		vecTmp.push_back(L"VOLTS_PER_METER");
		vecTmp.push_back(L"WEBERS");
		vecTmp.push_back(L"MICRO_SIEMENS");
		 
		vecTmp.push_back(L"DECIBELS");
		vecTmp.push_back(L"DECIBELS_MILLIVOLT");
		vecTmp.push_back(L"DECIBELS_VOLT");
		vecTmp.push_back(L"MILLISIEMENS");
	}
	else if (strGroupUnit.CompareNoCase(L"Energy") == 0)
	{
		vecTmp.clear();
		vecTmp.push_back(L"JOULES");
		vecTmp.push_back(L"KILOJOULES");
		vecTmp.push_back(L"WATT_HOURS");
		vecTmp.push_back(L"KILOWATT_HOURS");
		vecTmp.push_back(L"THERM");
		vecTmp.push_back(L"TON_HOR");
		vecTmp.push_back(L"KILOJOULES_PER_KILOGRAM");
		vecTmp.push_back(L"MEGAJOULES");
		vecTmp.push_back(L"MEGAWATT_HOURS");
		vecTmp.push_back(L"KILO_BTUS");
		vecTmp.push_back(L"MEGA_BTUS");
		vecTmp.push_back(L"WATT_HOURS_REACTIVE");
		vecTmp.push_back(L"KILOWATT_HOURS_REACTIVE");
		vecTmp.push_back(L"MEGAWATT_HOURS_REACTIVE");
	}					   
	else if (strGroupUnit.CompareNoCase(L"Enthalpy") == 0)
	{
		vecTmp.clear();
		 

		vecTmp.push_back(L"JOULES_PER_KILOGRAM_DRY_AIR");
			vecTmp.push_back(L"BTU_PER_POUND_DRY_AIR");
				vecTmp.push_back(L"BTUS_PER_POUND");
					vecTmp.push_back(L"KILOJOULES_PER_KILOGRAM_DRY_AIR");
						vecTmp.push_back(L"MEGAJOULES_PER_KILOGRAM_DRY_AIR");

	}
	else if (strGroupUnit.CompareNoCase(L"Entropy") == 0)
	{
		vecTmp.clear();
 

		vecTmp.push_back(L"JOULES_PER_DEGREE_KELVIN");
			vecTmp.push_back(L"JOULES_PER_KILOGRAM_DEGREE_KELVIN");
				vecTmp.push_back(L"KILOJOULES_PER_DEGREE_KELVIN");
					vecTmp.push_back(L"MEGAJOULES_PER_DEGREE_KELVIN");
	}
	else if (strGroupUnit.CompareNoCase(L"Force") == 0)
	{
		vecTmp.clear();
		vecTmp.push_back(L"NEWTON");
	}
	else if (strGroupUnit.CompareNoCase(L"Frequency") == 0)
	{
		vecTmp.clear();
	 


		vecTmp.push_back(L"CYCLES_PER_HOUR");
			vecTmp.push_back(L"CYCLES_PER_MINUTE");
				vecTmp.push_back(L"HERTZ");
					vecTmp.push_back(L"KILOHERTZ");
						vecTmp.push_back(L"MEGAHERTZ");
							vecTmp.push_back(L"PER_HOUR");

	}
	else if (strGroupUnit.CompareNoCase(L"Humidity") == 0)
	{
		vecTmp.clear();
		vecTmp.push_back(L"GRAMS_OF_WATER_PER_KILOGRAM_DRY_AIR");
		vecTmp.push_back(L"PERCENT_RELATIVE_HUMIDITY");
	}
	else if (strGroupUnit.CompareNoCase(L"Length") == 0)
	{
		vecTmp.clear();
		vecTmp.push_back(L"MILLIMETER");
			vecTmp.push_back(L"METERS");
				vecTmp.push_back(L"INCH");
					vecTmp.push_back(L"FEET");
						vecTmp.push_back(L"CENTIMETERS");
							vecTmp.push_back(L"KILOMETERS");
								vecTmp.push_back(L"MICROMETERS");
	}
	else if (strGroupUnit.CompareNoCase(L"Light") == 0)
	{
		vecTmp.clear();
		 


		vecTmp.push_back(L"WATTS_PER_SQUARE_FOOT");
			vecTmp.push_back(L"WATTS_PER_SQUARE_METER");
				vecTmp.push_back(L"LUMEN");
					vecTmp.push_back(L"LUX");
						vecTmp.push_back(L"FOOT_CANDLES");
							vecTmp.push_back(L"CANDELAS");
								vecTmp.push_back(L"CANDELAS_PER_SQUARE_METER");
	}
	else if (strGroupUnit.CompareNoCase(L"Mass") == 0)
	{
		vecTmp.clear();

		vecTmp.push_back(L"KILOGRAMS");
			vecTmp.push_back(L"POUNDS_MASS");
				vecTmp.push_back(L"TONS");
					vecTmp.push_back(L"GRAMS");
						vecTmp.push_back(L"MILLIGRAMS");


		 
	}
	else if (strGroupUnit.CompareNoCase(L"Mass_Flow") == 0)
	{
		vecTmp.clear();

	 
		
		vecTmp.push_back(L"KILOGRAMS_PER_SECOND");
			vecTmp.push_back(L"KILOGRAMS_PER_MINUTE");
				vecTmp.push_back(L"KILOGRAMS_PER_HOUR");
					vecTmp.push_back(L"POUNDS_MASS_PER_MINUTE");
						vecTmp.push_back(L"POUNDS_MASS_PER_HOUR");
							vecTmp.push_back(L"POUNDS_MASS_PER_SECOND");
								vecTmp.push_back(L"GRAMS_PER_SECOND");
									vecTmp.push_back(L"GRAMS_PER_MINUTE");
										vecTmp.push_back(L"TONS_PER_HOUR");


	}
	else if (strGroupUnit.CompareNoCase(L"Power") == 0)
	{
		vecTmp.clear();
 


		vecTmp.push_back(L"WATTS");
			vecTmp.push_back(L"KILOWATTS");
				vecTmp.push_back(L"MEGAWATTS");
					vecTmp.push_back(L"BTU_PER_HOUR");
						vecTmp.push_back(L"HORSEPOWER");
							vecTmp.push_back(L"TONS_REFRIGERATIONS");
								vecTmp.push_back(L"MILLIWATTS");
									vecTmp.push_back(L"KILO_BTUS_PER_HOUR");

	}
	else if (strGroupUnit.CompareNoCase(L"Pressure") == 0)
	{
		vecTmp.clear();
		 
		 
		vecTmp.push_back(L"PASCALS");
			vecTmp.push_back(L"KILOPASCAL");
				vecTmp.push_back(L"BAR");
			vecTmp.push_back(L"POUNDS_FORCE_PER_SQUARE_INCH");
			vecTmp.push_back(L"CENTIMETERS_OF_WATER");
			vecTmp.push_back(L"INCHES_OF_WATER");
			vecTmp.push_back(L"MILLIMETERS_OF_MERCURY");
			vecTmp.push_back(L"CENTIMETERS_OF_MERCURY");
			vecTmp.push_back(L"INCHES_OF_MERCURY");
			vecTmp.push_back(L"HECTOPASCALS");
			vecTmp.push_back(L"MILLIBARS");
			vecTmp.push_back(L"MILLIMETERS_OF_WATER");
	}
	else if (strGroupUnit.CompareNoCase(L"Temperature") == 0)
	{
		vecTmp.clear();
		    vecTmp.push_back(L"DEGREES_CELSIUS");
			vecTmp.push_back(L"DEGREES_KELVIN");
			vecTmp.push_back(L"DEGREES_FAHRENHEIT");
			vecTmp.push_back(L"DEGREE_DAYS_CELSIUS");
			vecTmp.push_back(L"DEGREE_DAYS_FAHRENHEIT");
			vecTmp.push_back(L"DELTA_DEGREE_FAHRENHEIT");
			vecTmp.push_back(L"DELTA_DEGREE_KELVIN");
			vecTmp.push_back(L"DEGREE_KELVIN_PER_HOUR");
			vecTmp.push_back(L"DEGREE_KELVIN_PER_MINUTE");
	}
	else if (strGroupUnit.CompareNoCase(L"Time") == 0)
	{
		vecTmp.clear();

		    vecTmp.push_back(L"YEAR");
			vecTmp.push_back(L"MONTH");
			vecTmp.push_back(L"WEEK");
			vecTmp.push_back(L"DAY");
			vecTmp.push_back(L"HOUR");
			vecTmp.push_back(L"MINUTE");
			vecTmp.push_back(L"SECOND");
			vecTmp.push_back(L"HUNDREDTHS_SECONDS");
			vecTmp.push_back(L"MILLISECONDS");
	}
	else if (strGroupUnit.CompareNoCase(L"Torque") == 0)
	{
		vecTmp.clear();
		vecTmp.push_back(L"NEWTON_METERS");
	}
	else if (strGroupUnit.CompareNoCase(L"Velocity") == 0)
	{
		vecTmp.clear();

		    vecTmp.push_back(L"METERS_PER_SECOND");
			vecTmp.push_back(L"KILOMETERS_PER_HOUR");
			vecTmp.push_back(L"FEET_PER_SECOND");
			vecTmp.push_back(L"FEET_PER_MINUTE");
			vecTmp.push_back(L"MILES_PER_HOUR");
			vecTmp.push_back(L"MILLIMETERS_PER_SECOND");
			vecTmp.push_back(L"MILLIMETERS_PER_MINUTE");
			vecTmp.push_back(L"METERS_PER_MINUTE");
			vecTmp.push_back(L"METERS_PER_HOUR");

	}
	else if (strGroupUnit.CompareNoCase(L"Volume") == 0)
	{
		vecTmp.clear();

	 


		    vecTmp.push_back(L"CUBIC_FEET");
			vecTmp.push_back(L"CUBIC_METERS");
			vecTmp.push_back(L"IMPERIAL_GALLONS");
			vecTmp.push_back(L"LITERS");
			vecTmp.push_back(L"US_GALLONS");
			vecTmp.push_back(L"MILLILITERS");

	}
	else if (strGroupUnit.CompareNoCase(L"Volumetric_Flow") == 0)
	{
		vecTmp.clear();
		    vecTmp.push_back(L"CUBIC_FEET_PER_MINUTE");
			vecTmp.push_back(L"CUBIC_METERS_PER_SECOND");
			vecTmp.push_back(L"IMPERIAL_GALLONS_PER_MINUTE");
			vecTmp.push_back(L"LITERS_PER_SECOND");
			vecTmp.push_back(L"LITERS_PER_MINUTE");
			vecTmp.push_back(L"US_GALLONS_PER_MINUTE");
			vecTmp.push_back(L"CUBIC_MATERS_PER_HOUR");
			vecTmp.push_back(L"LITERS_PER_HOUR");
			vecTmp.push_back(L"CUBIC_FEET_PER_SECOND");
			vecTmp.push_back(L"CUBIC_METERS_PER_MINUTE");
			vecTmp.push_back(L"CUBIC_FEET_PER_HOUR");
			vecTmp.push_back(L"US_GALLONS_PER_HOUR");
			vecTmp.push_back(L"MILLILITERS_PER_SECOND");

	}
	else if (strGroupUnit.CompareNoCase(L"Other") == 0)
	{
		vecTmp.clear();
		    vecTmp.push_back(L"DEGREES_ANGULAR");
			vecTmp.push_back(L"DEGREES_CELSIUS_PER_HOUR");
			vecTmp.push_back(L"DEGREES_CELSIUS_PER_MINUTE");
			vecTmp.push_back(L"DEGREES_FAHRENHEIT_PER_HOUR");
			vecTmp.push_back(L"DEGREES_FAHRENHEIT_PER_MINUTE");
			vecTmp.push_back(L"NO_UNITS");
			vecTmp.push_back(L"PARTS_PER_MILLION");
			vecTmp.push_back(L"PARTS_PER_BILLION");
			vecTmp.push_back(L"PERCENT");
			vecTmp.push_back(L"PERCENT_PER_SECOND");
			vecTmp.push_back(L"PER_MINUTE");
			vecTmp.push_back(L"PER_SECOND");
			vecTmp.push_back(L"PSI_PER_DEGREE_FAHRENHEIT");
			vecTmp.push_back(L"RADIANS");
			vecTmp.push_back(L"REVOLUTIONS_PER_MINUTE");
			vecTmp.push_back(L"KILOWATT_HOURS_PER_SQUARE_METER");
			vecTmp.push_back(L"KILOWATT_HOURS_PER_SQUARE_FOOT");
			vecTmp.push_back(L"MEGAJOULES_PER_SQUARE_METER");
			vecTmp.push_back(L"MEGAJOULES_PER_SQUARE_FOOT");
			vecTmp.push_back(L"WATTS_PER_SQUARE_METER_DEGREE_KELVIN");
			vecTmp.push_back(L"PERCENT_OBSCURATION_PER_FOOT");
			vecTmp.push_back(L"PERCENT_OBSCURATION_PER_METER");
			vecTmp.push_back(L"JOULE_SECONDS");
			vecTmp.push_back(L"RADIANS_PER_SECOND");
			vecTmp.push_back(L"SQUARE_METERS_PER_NEWTON");
			vecTmp.push_back(L"KILOGRAMS_PER_CUBIC_METER");
			vecTmp.push_back(L"NEWTON_SECONDS");
			vecTmp.push_back(L"NEWTONS_PER_METER");
			vecTmp.push_back(L"WATTS_PER_METER_PER_DEGREE_KELVIN");
			vecTmp.push_back(L"PER_MILE");
			vecTmp.push_back(L"GRAMS_PER_GRAM");
			vecTmp.push_back(L"KILOGRAMS_PER_KILOGRAM");
			vecTmp.push_back(L"GRAMS_PER_KILOGRAM");
			vecTmp.push_back(L"MILLIGRAMS_PER_GRAM");
			vecTmp.push_back(L"MILLIGRAMS_PER_KILOGRAM");
			vecTmp.push_back(L"GRAMS_PER_MILLILITER");
			vecTmp.push_back(L"GRAMS_PER_LITER");
			vecTmp.push_back(L"MILLIGRAMS_PER_LITER");
			vecTmp.push_back(L"MICROGRAMS_PER_LITER");
			vecTmp.push_back(L"GRAMS_PER_CUBIC_METER");
			vecTmp.push_back(L"MILLIGRAMS_PER_CUBIC_METER");
			vecTmp.push_back(L"MICROGRAMS_PER_CUBIC_METER");
			vecTmp.push_back(L"NANOGRAMS_PER_CUBIC_METER");
			vecTmp.push_back(L"GRAMS_PER_CUBIC_CENTIMETER");
			vecTmp.push_back(L"BECQUERELS");
			vecTmp.push_back(L"KILOBECQUERELS");
			vecTmp.push_back(L"KILOBECQUERELS");
			vecTmp.push_back(L"MEGABECQUERELS");
			vecTmp.push_back(L"GRAY");
			vecTmp.push_back(L"MILLIGRAY");
			vecTmp.push_back(L"MICROGRAY");
			vecTmp.push_back(L"SIEVERTS");
			vecTmp.push_back(L"MILLISIEVERTS");
			vecTmp.push_back(L"MICROSIEVERTS");
			vecTmp.push_back(L"MICROSIEVERTS_PER_HOUR");
			vecTmp.push_back(L"DECIBELS_A");
			vecTmp.push_back(L"NEPHELOMETRIC_TURBIDITY_UNIT");
			vecTmp.push_back(L"PH");
			vecTmp.push_back(L"GRAMS_PER_SQUARE_METER");
			vecTmp.push_back(L"MINUTES_PER_DEGREE_KELVIN");
	}
	else
	{
		vecTmp.clear();
	}
	 
	return vecTmp;
}
vector<CString> CModbusToBacnetRouterDlg::GetDataFormatByVariable(CString strVariable)
{
	vector<CString> vecTmp;
	if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0)
	{
		  

		   vecTmp.clear();
		    vecTmp.push_back(L"16 Bit Unsigned Integer");
			vecTmp.push_back(L"16 Bit Signed Integer");
			vecTmp.push_back(L"32 Bit Unsigned Integer HI_LO");
			vecTmp.push_back(L"32 Bit Signed Integer HI_LO");
			vecTmp.push_back(L"32 Bit Unsigned Integer LO_HI");
			vecTmp.push_back(L"32 Bit Signed Integer LO_HI");
			vecTmp.push_back(L"Floating Point HI_LO");
			vecTmp.push_back(L"Floating Point LO_HI");
			vecTmp.push_back(L"_16 Bit   Bit Pick");
			vecTmp.push_back(L"_32 Bit   Bit Pick HI_LO");
			vecTmp.push_back(L"_32 Bit   Bit Pick LO_HI");
   }
	else if (strVariable.CompareNoCase(L"_0_Coil_Bit") == 0 || strVariable.CompareNoCase(L"_1_Input_Bit") == 0)
	{
		vecTmp.clear();
		vecTmp.push_back(L"bit");
	}
	else if (strVariable.CompareNoCase(L"_3_Input_Register") == 0)
	{
		vecTmp.clear();
	 
		    vecTmp.push_back(L"16 Bit Unsigned Integer");
			vecTmp.push_back(L"16 Bit Signed Integer");
			vecTmp.push_back(L"32 Bit Unsigned Integer HI_LO");
			vecTmp.push_back(L"32 Bit Signed Integer HI_LO");
			vecTmp.push_back(L"32 Bit Unsigned Integer LO_HI");
			vecTmp.push_back(L"32 Bit Signed Integer LO_HI");
			vecTmp.push_back(L"Floating Point HI_LO");
			vecTmp.push_back(L"Floating Point LO_HI");
			vecTmp.push_back(L"_16 Bit   Bit Pick");
			vecTmp.push_back(L"_32 Bit   Bit Pick HI_LO");
			vecTmp.push_back(L"_32 Bit   Bit Pick LO_HI");

	}
	else if (strVariable.CompareNoCase(L"Area") == 0)
	{
		vecTmp.clear();
		vecTmp.push_back(L"SQUARE_METERS");
		vecTmp.push_back(L"SQUARE_FEET");
		vecTmp.push_back(L"SQUARE_INCHES");
		vecTmp.push_back(L"SQUARE_CENTIMETERS");
	}
	 
	return vecTmp;
	 
}
vector<CString> CModbusToBacnetRouterDlg::GetRWByVariableDataFormat(CString strVariable, CString strDataFormat)
{


	vector<CString> vecTmp;
	if (
		(strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"16 Bit Unsigned Integer") == 0) ||
		(strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"16 Bit Signed Integer") == 0) ||
		(strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"32 Bit Unsigned Integer HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"32 Bit Signed Integer HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"32 Bit Signed Integer LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"32 Bit Unsigned Integer LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"Floating Point HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"Floating Point LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"_16 Bit Bit Pick") == 0) ||
		(strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"_0_Coil_BitBit") == 0)
		)
	{
		vecTmp.push_back(L"R_W");
		vecTmp.push_back(L"R_Only");
	}
	else if (
		(strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"16 Bit Unsigned Integer") == 0) ||
		(strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"16 Bit Signed Integer") == 0) ||
		(strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"32 Bit Unsigned Integer HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"32 Bit Signed Integer HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"32 Bit Signed Integer LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"32 Bit Unsigned Integer LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"Floating Point HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"Floating Point LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"_16 Bit Bit Pick") == 0) ||
		(strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"_1_Input_BitBits") == 0)
		)
	{
		vecTmp.push_back(L"R_Only");
	}
	 
	return vecTmp;
}
vector<CString> CModbusToBacnetRouterDlg::GetBacnetType(CString strVariable, CString strDataFormat, CString RW)
{
	vector<CString> vecTmp;
	if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"16 Bit Unsigned Integer") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"AO");
		/*  Future 1
		  Future 2
		  Future 3
		  Future 4
		  Future 5*/
	}
	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"16 Bit Unsigned Integer") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		/*Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"16 Bit Signed Integer") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"AO");
		/*Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"16 Bit Signed Integer") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		/*Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"_4_Holding_Register32") == 0 && strDataFormat.CompareNoCase(L"Bit Unsigned Integer HI_LO") == 0 && RW.CompareNoCase(L"R_W") == 0)
	  {
		  vecTmp.push_back(L"AO");
			  vecTmp.push_back(L"UVRW");
			 /* Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register32") == 0 && strDataFormat.CompareNoCase(L"Bit Unsigned Integer HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
			  vecTmp.push_back(L"UVRO");
			  /*Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register32") == 0 && strDataFormat.CompareNoCase(L"Bit Signed Integer HI_LO") == 0 && RW.CompareNoCase(L"R_W") == 0)
	  {
		  vecTmp.push_back(L"AO");
			  vecTmp.push_back(L"IVRW");
			  /* Future 2
			   Future 3
			   Future 4
			   Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register32") == 0 && strDataFormat.CompareNoCase(L"Bit Signed Integer HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
			  vecTmp.push_back(L"IVRO");
			 /* Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register32") == 0 && strDataFormat.CompareNoCase(L"Bit Signed Integer LO_HI") == 0 && RW.CompareNoCase(L"R_W") == 0)
	  {
		  vecTmp.push_back(L"AO");
			  vecTmp.push_back(L"IVRW");
			  /*Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register32") == 0 && strDataFormat.CompareNoCase(L"Bit Signed Integer LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
			  vecTmp.push_back(L"IVRO");
			 /* Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register32") == 0 && strDataFormat.CompareNoCase(L"Bit Unsigned Integer LO_HI") == 0 && RW.CompareNoCase(L"R_W") == 0)
	  {
		  vecTmp.push_back(L"AO");
			  vecTmp.push_back(L"UVRW");
			/*  Future 2
			  Future 3
			  Future 4
			  Future 5*/
	 
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register32") == 0 && strDataFormat.CompareNoCase(L"Bit Unsigned Integer LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
			  vecTmp.push_back(L"UVRO");
			 /* Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }

	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"Floating Point HI_LO") == 0 && RW.CompareNoCase(L"R_W") == 0)
		  {
			  vecTmp.push_back(L"AO");
				 /* Future 1
				  Future 2
				  Future 3
				  Future 4
				  Future 5*/
		  }

	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"Floating Point HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
			  {
				  vecTmp.push_back(L"AI");
					 /* Future 1
					  Future 2
					  Future 3
					  Future 4
					  Future 5*/
			  }

	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"Floating Point LO_HI") == 0 && RW.CompareNoCase(L"R_W") == 0)
				  {
					  vecTmp.push_back(L"AO");
						/*  Future 1
						  Future 2
						  Future 3
						  Future 4
						  Future 5*/
				  }
			  
	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"Floating Point LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
							 ///* Future 1
							 // Future 2
							 // Future 3
							 // Future 4
							 // Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"_16 Bit Bit Pick") == 0 && RW.CompareNoCase(L"R_W") == 0)
	  {
		  vecTmp.push_back(L"BO");
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"_16 Bit Bit Pick") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"BI");
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick HI_LO") == 0 && RW.CompareNoCase(L"R_W") == 0)
	  {
		  vecTmp.push_back(L"BO");
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"BI");
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick LO_HI") == 0 && RW.CompareNoCase(L"R_W") == 0)
	  {
		  vecTmp.push_back(L"BO");
	  }
	else if (strVariable.CompareNoCase(L"_4_Holding_Register") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"BI");
	  }
	else if (strVariable.CompareNoCase(L"_0_Coil_Bit") == 0 && strDataFormat.CompareNoCase(L"Bit") == 0 && RW.CompareNoCase(L"R_W") == 0)
	  {
		  vecTmp.push_back(L"BO");
	  }
	else if (strVariable.CompareNoCase(L"_0_Coil_Bit") == 0 && strDataFormat.CompareNoCase(L"Bit") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"BI");
	  }
	else if (strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"16 Bit Unsigned Integer") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
			 /* Future 1
			  Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"16 Bit Signed Integer") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
			 /* Future 1
			  Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"32 Bit Unsigned Integer HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
		  vecTmp.push_back(L"UVRO");
			  /*Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"32 Bit Signed Integer HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
	      vecTmp.push_back(L"IVRO");
			 /* Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"32 Bit Signed Integer LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
			  vecTmp.push_back(L"IVRO");
			/*  Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"32 Bit Unsigned Integer LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
			  vecTmp.push_back(L"UVRO");
			 /* Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"Floating Point HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
			 /* Future 1
			  Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"Floating Point LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"AI");
			 /* Future 1
			  Future 2
			  Future 3
			  Future 4
			  Future 5*/
	  }
	else if (strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"_16 Bit Bit Pick") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"BI");
	  }

	else if (strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"BI");
	  }

	else if (strVariable.CompareNoCase(L"_3_Input_Register") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"BI");
	  }
	else if (strVariable.CompareNoCase(L"_1_Input_Bit") == 0 && strDataFormat.CompareNoCase(L"Bit") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	  {
		  vecTmp.push_back(L"BI");
	  }
	else
	  {
	  }

	 
	return vecTmp;
}

void CModbusToBacnetRouterDlg::OnBnClickedButtonBlxfile()
{
	 
}


void CModbusToBacnetRouterDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString temp_cstring;
	long lRow, lCol;
	m_datalist.Set_Edit(true);
	DWORD dwPos = GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_datalist.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
    m_datalist.SubItemHitTest(&lvinfo);
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;
	//5=数据格式
	if (lRow<m_vecMd2BT.size()&&lCol==5)
	{
		m_vecDataFormat = GetDataFormatByVariable(m_vecMd2BT[lRow].Modbus_Variable_Type);
	 
		if (ListCtrlEx::ComboBox == m_datalist.GetColumnType(5))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa = 0;aaa < m_vecDataFormat.size();aaa++)
			{
				strlist.push_back(m_vecDataFormat[aaa]);
			}
			m_datalist.SetCellStringList(lRow, 5, strlist);
		}
	}
	//11=读写属性的判断
	if (lRow < m_vecMd2BT.size() && lCol == 11)
	{
		m_vecDataRW = GetRWByVariableDataFormat(m_vecMd2BT[lRow].Modbus_Variable_Type, m_vecMd2BT[lRow].Data_Format);
		if (ListCtrlEx::ComboBox == m_datalist.GetColumnType(11))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa = 0;aaa < m_vecDataRW.size();aaa++)
			{
				strlist.push_back(m_vecDataRW[aaa]);
			}
			m_datalist.SetCellStringList(lRow, 11, strlist);
		}
	}
	//12=Bacnet Type 判断
	if (lRow < m_vecMd2BT.size() && lCol == 12)
	{
		m_vecBacnetType = GetBacnetType(m_vecMd2BT[lRow].Modbus_Variable_Type, m_vecMd2BT[lRow].Data_Format, m_vecMd2BT[lRow].Read_Only_Or_RW);
		if (ListCtrlEx::ComboBox == m_datalist.GetColumnType(12))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa = 0;aaa < m_vecBacnetType.size();aaa++)
			{
				strlist.push_back(m_vecBacnetType[aaa]);
			}
			m_datalist.SetCellStringList(lRow, 12, strlist);
		}
	}
	//
	if (lRow < m_vecMd2BT.size() && lCol == 16)
	{
		m_UnitValue = GetUnitValueByGroupUnit(m_vecMd2BT[lRow].Unit_Group);
		if (ListCtrlEx::ComboBox == m_datalist.GetColumnType(16))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa = 0;aaa < m_UnitValue.size();aaa++)
			{
				strlist.push_back(m_UnitValue[aaa]);
			}
			m_datalist.SetCellStringList(lRow, 16, strlist);
		}
	}
	*pResult = 0;
}
LRESULT CModbusToBacnetRouterDlg::Fresh_Input_Item(WPARAM wParam, LPARAM lParam)
{
	int cmp_ret;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	if (Changed_Item>m_vecMd2BT.size())
	{
		m_datalist.SetItemText(Changed_Item, Changed_SubItem, _T(""));
	   return 0;

	}

	CString temp_task_info;
	CString New_CString = m_datalist.GetItemText(Changed_Item, Changed_SubItem);
	CString cstemp_value;
	if (Changed_SubItem==1)
	{
		m_vecMd2BT[Changed_Item].Poll_YES_NO = New_CString;
	}
	if (Changed_SubItem == 2)
	{
		m_vecMd2BT[Changed_Item].Bacnet_Object_Name = New_CString;
		
	}
	if (Changed_SubItem == 3)
	{
		m_vecMd2BT[Changed_Item].Modbus_Variable_Type = New_CString;
		
	}
	if (Changed_SubItem == 4)
	{
		m_vecMd2BT[Changed_Item].From1_To_65536 = New_CString;
		
	}
	if (Changed_SubItem == 5)
	{
		m_vecMd2BT[Changed_Item].Data_Format = New_CString;
	}
	if (Changed_SubItem == 6)
	{
		m_vecMd2BT[Changed_Item].Bit_1 = New_CString;
	}
	if (Changed_SubItem == 7)
	{
		m_vecMd2BT[Changed_Item].Low_Actual = New_CString;
	}
	if (Changed_SubItem == 8)
	{
		m_vecMd2BT[Changed_Item].High_Actual = New_CString;
	}
	if (Changed_SubItem == 9)
	{
		m_vecMd2BT[Changed_Item].Low_Scale = New_CString;
	}
	if (Changed_SubItem == 10)
	{
		m_vecMd2BT[Changed_Item].High_Scale = New_CString;
	}
	if (Changed_SubItem == 11)
	{
		m_vecMd2BT[Changed_Item].Read_Only_Or_RW = New_CString;
	}
	if (Changed_SubItem == 12)
	{
		m_vecMd2BT[Changed_Item].Read_Only_Or_RW = New_CString;
	}
	if (Changed_SubItem == 13)
	{
		m_vecMd2BT[Changed_Item].Bacnet_Object_Description = New_CString;
	}
	if (Changed_SubItem == 14)
	{
		m_vecMd2BT[Changed_Item].COV_Increment = New_CString;
	}
	if (Changed_SubItem == 15)
	{
		m_vecMd2BT[Changed_Item].Unit_Group = New_CString;
	}
	if (Changed_SubItem == 16)
	{
		m_vecMd2BT[Changed_Item].Unit_Value = New_CString;
	}
	if (Changed_SubItem == 17)
	{
		m_vecMd2BT[Changed_Item].Grouping_YES_NO = New_CString;
	}
	if (Changed_SubItem == 18)
	{
		m_vecMd2BT[Changed_Item].Update_On_Reconnect = New_CString;
	}
}