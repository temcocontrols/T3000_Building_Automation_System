// ProductRegisterListView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ProductRegisterListView.h"
#include "../SQLiteDriver/CppSQLite3.h"
#include <bitset>
#include "WriteRegistersValueDlg.h"


#define _NUM_ITEM 0
#define _NUM_POLLYESNO 1
#define _NUM_NAME 2
#define _NUM_FUNCTION 3
#define _NUM_MODBUSADDRESS 4
#define _NUM_LENGTH 5
#define _NUM_VALUE 6
#define _NUM_DATAFORMAT 7
#define _NUM_BIT 8
#define _NUM_Low_Actual 9
#define _NUM_High_Actual 10
#define _NUM_Low_Scale 11
#define _NUM_High_Scale 12
#define _NUM_RW 13
#define _NUM_DEFAULT 14
#define _NUM_Bacnet_Type 15
#define _NUM_Bacnet_Object_Description 16
#define _NUM_COV_Increment 17
#define _NUM_Unit_Group 18
#define _NUM_Unit_Value 19
#define _NUM_Grouping_Y_N 20
#define _NUM_Update_On_Reconnect 21
#define _NUM_Modbus_DESCRIPTION 22

CustomProductTable_T g_current_Node;
// CProductRegisterListView
UINT _Background_Read(LPVOID pParam) {
	CProductRegisterListView* Parent = (CProductRegisterListView*)(pParam);
	while (TRUE) {
		if (!Parent->IsWindowVisible())
		{
			g_bPauseMultiRead = FALSE;
			return 0;
		}
		if (!is_connect())
		{
			Sleep(1000);
			continue;
		}
		if (!Parent->m_isReading)
		{
			Sleep(1000);
			continue;
		}
		int ModbusID = Parent->m_current_tree_node.product_id;

		for (int i = 0; i<(int)Parent->m_register_data_sheet.size(); i++)
		{
			PostMessage(g_hwnd_now, WM_REFRESH_BAC_INPUT_LIST, 2U, i);
			int Count_Number = Parent->m_register_data_sheet.at(i).Counts_Number;
			unsigned short Start_Address = Parent->m_register_data_sheet.at(i).Reg_ID;
			unsigned short function_code = Parent->m_register_data_sheet.at(i).function_code;
			if (Count_Number > 0)
			{
				unsigned short  DataBuffer[100];

				while (!Parent->m_isReading)
				{
					Sleep(500);
					continue;
				}
				Sleep(Parent->m_UINT_delay_items);
				// Read_Multi(ModbusID,DataBuffer,Start_Address,Count_Number)

#if 1
				unsigned char rev_back_rawData[300], send_data[100];
				int Send_length;
				int Rev_length;
				// int ret = Modbus_Standard_Read(ModbusID,DataBuffer,  function_code, Start_Address, Count_Number);
				int ret = Modbus_Standard_Read(ModbusID, &DataBuffer[0], function_code, Start_Address, Count_Number, &send_data[0], &rev_back_rawData[0], &Send_length, &Rev_length);

				if (ret>0)
				{
					/*
					说明：这里是把读到的值转化成字符串，值与值之间用逗号隔开
					然后把值赋值给结构体中的Value，在发出消息，把值显示出来
					*/
					/* 16 Bit Unsigned Integer
					16 Bit Signed Integer
					32 Bit Unsigned Integer HI_LO
					32 Bit Signed Integer HI_LO
					32 Bit Unsigned Integer LO_HI
					32 Bit Signed Integer LO_HI
					Floating Point HI_LO
					Floating Point LO_HI
					_16 Bit   Bit Pick
					_32 Bit   Bit Pick HI_LO
					_32 Bit   Bit Pick LO_HI
					Bit*/


					CString strValue = _T("");
					CString strTemp;
					if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("16 Bit Signed Integer")) == 0)
					{

						for (int len = 0; len<Count_Number; len++)
						{
							strTemp.Format(_T("%d"), (short)DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("16 Bit Unsigned Integer")) == 0)
					{
						for (int len = 0; len<Count_Number; len++)
						{
							strTemp.Format(_T("%d"), DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("32 Bit Unsigned Integer HI_LO")) == 0)
					{

						for (int len = 0; len < Count_Number; len++)
						{
							strTemp.Format(_T("%d"), DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("32 Bit Signed Integer HI_LO")) == 0)
					{
						for (int len = 0; len < Count_Number; len++)
						{
							strTemp.Format(_T("%d"), DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("32 Bit Unsigned Integer LO_HI")) == 0)
					{
						for (int len = 0; len < Count_Number; len++)
						{
							strTemp.Format(_T("%d"), DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("32 Bit Signed Integer LO_HI")) == 0)
					{
						for (int len = 0; len < Count_Number; len++)
						{
							strTemp.Format(_T("%d"), DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("Floating Point HI_LO")) == 0)
					{
						for (int len = 0; len < Count_Number; len++)
						{
							strTemp.Format(_T("%d"), DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("Floating Point LO_HI")) == 0)
					{
						for (int len = 0; len < Count_Number; len++)
						{
							strTemp.Format(_T("%d"), DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("_16 Bit   Bit Pick")) == 0)
					{
						for (int len = 0; len < Count_Number; len++)
						{
							strTemp.Format(_T("%d"), DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("_32 Bit   Bit Pick HI_LO")) == 0)
					{
						for (int len = 0; len < Count_Number; len++)
						{
							strTemp.Format(_T("%d"), DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("_32 Bit   Bit Pick LO_HI")) == 0)
					{
						for (int len = 0; len < Count_Number; len++)
						{
							strTemp.Format(_T("%d"), DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					else if (Parent->m_register_data_sheet.at(i).DataFormat.CompareNoCase(_T("Bit")) == 0)
					{
						for (int len = 0; len < Count_Number; len++)
						{
							strTemp.Format(_T("%d"), DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					else
					{
						for (int len = 0; len < Count_Number; len++)
						{
							strTemp.Format(_T("%d"), DataBuffer[len]);
							strValue += strTemp;
							if (len + 1 != Count_Number)
							{
								strValue += _T(",");
							}
						}
					}
					Parent->m_register_data_sheet.at(i).Value = strValue;
					PostMessage(g_hwnd_now, WM_REFRESH_BAC_INPUT_LIST, 1U, i);
				}
#endif

			}

		}

		Sleep(Parent->m_UINT_delay_loop);
	}

}
IMPLEMENT_DYNCREATE(CProductRegisterListView, CFormView)

CProductRegisterListView::CProductRegisterListView()
	: CFormView(CProductRegisterListView::IDD)
	, m_UINT_delay_loop(2000)
	, m_UINT_delay_items(500)
{

	m_BackgroundTreadHandle = NULL;

	m_short_counts = 1;
	m_string_paratypes = _T("");
	m_sort_type = 0;//默认排序
	m_value_format = 0;

	m_vecDataRW.clear();
	m_vecDataRW.push_back(L"R_Only");
	m_vecDataRW.push_back(L"R_W");
	m_vecYesNo.clear();
	m_vecYesNo.push_back(L"NO");
	m_vecYesNo.push_back(L"YES");


	m_vecVariableType.push_back(L"Read_Coils");
	m_vecVariableType.push_back(L"Read_Discrete_Inputs");
	m_vecVariableType.push_back(L"Read_Holding_Registers");
	m_vecVariableType.push_back(L"Read_Input_Registers");

	m_vecVariableType.push_back(L"Write_Single_Coil");
	m_vecVariableType.push_back(L"Write_Single_Register");
	m_vecVariableType.push_back(L"Write_Multiple_Coils");
	m_vecVariableType.push_back(L"Write_Multiple_Registers");


	m_GroupUnits.clear();
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

}

CProductRegisterListView::~CProductRegisterListView()
{

}

void CProductRegisterListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CUSTOM_LIST, m_register_list);
	DDX_Control(pDX, IDC_COMBO_DATAFORMAT, m_combox_dataformat);
	DDX_Control(pDX, IDC_COMBO_PROPERTY, m_combox_property);
	DDX_Control(pDX, IDC_EDIT_COUNTS, m_edit_counts);
	DDX_CBString(pDX, IDC_COMBO_DATAFORMAT, m_string_dataformat);
	DDX_CBString(pDX, IDC_COMBO_PROPERTY, m_string_property);
	DDX_Text(pDX, IDC_EDIT_COUNTS, m_short_counts);
	DDX_Control(pDX, IDC_COMBO_PARA_TYPES, m_combox_paratypes);
	DDX_CBString(pDX, IDC_COMBO_PARA_TYPES, m_string_paratypes);
	DDX_Control(pDX, IDC_UPBUTTON, m_upButton);
	DDX_Control(pDX, IDC_DOWNBUTTON, m_downButton);
	DDX_Text(pDX, IDC_EDIT_DELAY_LOOP, m_UINT_delay_loop);
	DDX_Text(pDX, IDC_EDIT_DELAY_ITEMS, m_UINT_delay_items);

	DDX_Control(pDX, IDC_COMBO_PRODUCT_LIST, m_combox_productname);
}

BEGIN_MESSAGE_MAP(CProductRegisterListView, CFormView)
	ON_WM_SIZE()
	ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST, Fresh_Input_List)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED, Change_Input_Item)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CUSTOM_LIST, &CProductRegisterListView::OnNMClickList_output)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_READ_DEVICE, &CProductRegisterListView::OnBnClickedReadDevice)
	ON_BN_CLICKED(IDC_BUTTON4, &CProductRegisterListView::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_DOWNBUTTON, &CProductRegisterListView::OnBnClickedDownbutton)
	ON_BN_CLICKED(IDC_UPBUTTON, &CProductRegisterListView::OnBnClickedUpbutton)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_DELAY_LOOP, &CProductRegisterListView::OnEnChangeEditDelayLoop)
	ON_EN_CHANGE(IDC_EDIT_DELAY_ITEMS, &CProductRegisterListView::OnEnChangeEditDelayItems)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CUSTOM_LIST, &CProductRegisterListView::OnNMDblclkListCustomList)
	ON_CBN_SELCHANGE(IDC_COMBO_PRODUCT_LIST, &CProductRegisterListView::OnCbnSelchangeComboProductList)
	ON_BN_CLICKED(IDC_BUTTON_EXPORTEXCELFILE, &CProductRegisterListView::OnBnClickedButtonExportexcelfile)
END_MESSAGE_MAP()


// CProductRegisterListView diagnostics

#ifdef _DEBUG
void CProductRegisterListView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CProductRegisterListView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


void CProductRegisterListView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (nType == SIZE_RESTORED)
	{

		CRect ViewRect;
		GetClientRect(&ViewRect);
		//TRACE(_T(" View:T=%d,B=%d,L=%d,R=%d\n"),ViewRect.top,ViewRect.bottom,ViewRect.left,ViewRect.right);
		// m_MsDataGrid.SetWindowPos(this,ViewRect.top,ViewRect.left,ViewRect.Width(),ViewRect.Height(),SWP_SHOWWINDOW|SWP_NOZORDER);
		if (custom_bacnet_register_listview)
		{
			if (m_register_list.GetSafeHwnd())
			{
				// m_register_list.MoveWindow(CRect(0,50,ViewRect.Width(),ViewRect.Height()),TRUE);
				m_register_list.MoveWindow(CRect(0, 30, ViewRect.Width(), ViewRect.Height()), TRUE);
			}
		}
		else
		{
			if (m_register_list.GetSafeHwnd())
			{
				// m_register_list.MoveWindow(CRect(0,50,ViewRect.Width(),ViewRect.Height()),TRUE);
				m_register_list.MoveWindow(CRect(0, 0, ViewRect.Width(), ViewRect.Height()), TRUE);
			}
		}


	}

}

void CProductRegisterListView::SetStart(BOOL Start)
{
	m_isReading = Start;
}
void CProductRegisterListView::Fresh(void)
{
	//Import_CustomProductTable();
	close_com();
	g_bPauseMultiRead = TRUE;
	m_upButton.SetImage(IDB_UPBMP);
	m_downButton.SetImage(IDB_DOWNBMP);
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);
	if (!custom_bacnet_register_listview)
	{
		m_current_tree_node = pFrame->m_current_tree_node;
		GetDlgItem(IDC_STATIC_PRODUCTNAME)->ShowWindow(FALSE);
		GetDlgItem(IDC_COMBO_PRODUCT_LIST)->ShowWindow(FALSE);

	}
	else
	{
		GetDlgItem(IDC_STATIC_PRODUCTNAME)->ShowWindow(TRUE);
		GetDlgItem(IDC_COMBO_PRODUCT_LIST)->ShowWindow(TRUE);
	}
	GetDlgItem(IDC_READ_DEVICE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_DELAYBETWEENLOOPS)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_DELAY_LOOP)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_DELAY_BETWEENITEMS)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_DELAY_ITEMS)->ShowWindow(FALSE);

	GetDlgItem(IDC_STATIC_DEFAULT_PSETTING)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_SORT)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_ASC)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_DESC)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON4)->ShowWindow(FALSE);


	GetDlgItem(IDC_UPBUTTON)->ShowWindow(FALSE);
	GetDlgItem(IDC_DOWNBUTTON)->ShowWindow(FALSE);

	GetDlgItem(IDC_STATIC_COUNT_NUMBER)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_COUNTS)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_PROPERTY)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_DATAFORMAT)->ShowWindow(FALSE);

	GetDlgItem(IDC_COMBO_PROPERTY)->ShowWindow(FALSE);
	GetDlgItem(IDC_COMBO_DATAFORMAT)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EXPORTEXCELFILE)->ShowWindow(FALSE);
	if (offline_mode)
	{
		GetDlgItem(IDC_BUTTON_EXPORTEXCELFILE)->ShowWindow(TRUE);
		GetDlgItem(IDC_READ_DEVICE)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_DELAYBETWEENLOOPS)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_DELAY_LOOP)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_DELAY_BETWEENITEMS)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_DELAY_ITEMS)->ShowWindow(FALSE);

		GetDlgItem(IDC_STATIC_DEFAULT_PSETTING)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_SORT)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_ASC)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_DESC)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON4)->ShowWindow(FALSE);


		GetDlgItem(IDC_UPBUTTON)->ShowWindow(FALSE);
		GetDlgItem(IDC_DOWNBUTTON)->ShowWindow(FALSE);

		GetDlgItem(IDC_STATIC_COUNT_NUMBER)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_COUNTS)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_PROPERTY)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_DATAFORMAT)->ShowWindow(FALSE);

		GetDlgItem(IDC_COMBO_PROPERTY)->ShowWindow(FALSE);
		GetDlgItem(IDC_COMBO_DATAFORMAT)->ShowWindow(FALSE);
	}
	else
	{

		/* m_isReading=*/
		pFrame->ConnectDevice(m_current_tree_node);
		m_isReading = TRUE;
		if (m_isReading)
		{
			GetDlgItem(IDC_READ_DEVICE)->SetWindowText(_T("Stop Read"));
		}
		else
		{
			GetDlgItem(IDC_READ_DEVICE)->SetWindowText(_T("Start Read"));
		}
	}

	m_combox_dataformat.ResetContent();
	for (int i = 0; i<m_vecDataFormat.size(); i++)
	{
		m_combox_dataformat.AddString(m_vecDataFormat[i]);
	}
	m_combox_dataformat.SetCurSel(0);


	for (pidname_map::iterator iter = product_map.begin(); iter != product_map.end(); iter++)
	{
		m_combox_productname.AddString(iter->second);
	}

	//GetProductName()
	LoadDataSheet();
	Initial_List();

	if ((!offline_mode) && (!custom_bacnet_register_listview))
	{
		/*if (m_BackgroundTreadHandle)
		{
			BOOL bRet = TerminateThread(m_BackgroundTreadHandle->m_hThread, 0);
			m_BackgroundTreadHandle = NULL;
		}
		m_BackgroundTreadHandle = AfxBeginThread(_Background_Read, this);*/
	}

}

//#define _NUM_POLLYESNO 0
//#define _NUM_NAME 1
//#define _NUM_FUNCTION 2
//#define _NUM_MODBUSADDRESS 3
//#define _NUM_LENGTH 4
//#define _NUM_VALUE 5
//#define _NUM_DATAFORMAT 6
//#define _NUM_BIT 7
//#define _NUM_Low_Actual 8
//#define _NUM_High_Actual 9
//#define _NUM_Low_Scale 10
//#define _NUM_High_Scale 11
//#define _NUM_RW 12
//#define _NUM_DEFAULT 13
//#define _NUM_Bacnet_Type 14
//#define _NUM_Bacnet_Object_Description 15
//#define _NUM_COV_Increment 16
//#define _NUM_Unit_Group 17
//#define _NUM_Unit_Value 18
//#define _NUM_Modbus_DESCRIPTION 19
//#define _NUM_Grouping_Y_N 20
//#define _NUM_Update_On_Reconnect 21
//#define _NUM_Modbus_DESCRIPTION 22

void CProductRegisterListView::Initial_List(void)
{
	g_hwnd_now = this->m_hWnd;
	CString strTemp;
	m_register_list.ShowWindow(SW_HIDE);
	m_register_list.DeleteAllItems();
	while (m_register_list.DeleteColumn(0));
	m_register_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_register_list.SetExtendedStyle(m_register_list.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
	m_register_list.InsertColumn(_NUM_ITEM, _T("Item"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_POLLYESNO, _T("Poll Y/N"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_NAME, _T("Bacnet Object Name"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_FUNCTION, _T("Modbus Variable Type"), 120, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_MODBUSADDRESS, _T("Modbus Address"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_register_list.InsertColumn(_NUM_LENGTH, _T("Length"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_VALUE, _T("Value"), 90, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_DATAFORMAT, _T("Data Format"), 90, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_BIT, _T("Bit #"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_Low_Actual, _T("Low Actual"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_High_Actual, _T("High Actual"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_Low_Scale, _T("Low Scale"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_High_Scale, _T("High Scale"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_RW, _T("R/W"), 90, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_DEFAULT, _T("Default"), 90, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_Bacnet_Type, _T("Bacnet Type"), 120, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_Bacnet_Object_Description, _T("Bacnet Object Description"), 200, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_COV_Increment, _T("COV Increment"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_Unit_Group, _T("Unit Group"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_Unit_Value, _T("Unit Value"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_Grouping_Y_N, _T("Grouping YES / NO"), 200, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_Update_On_Reconnect, _T("Update On Reconnect"), 200, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_register_list.InsertColumn(_NUM_Modbus_DESCRIPTION, _T("Register Description"), 200, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

	ShowDataInList();
}
void CProductRegisterListView::ShowDataInList()
{
	CString strTemp;
	for (int i = 0; i<(int)m_register_data_sheet.size(); i++)
	{
		strTemp.Format(_T("%d"), i + 1);
		m_register_list.InsertItem(i, strTemp);
		m_register_list.SetItemText(i, _NUM_POLLYESNO, m_register_data_sheet.at(i).Poll_YES_NO);


		m_register_list.SetItemText(i, _NUM_NAME, m_register_data_sheet.at(i).Bacnet_Object_Name);

		strTemp.Format(_T("%s"), GetFunctionName(m_register_data_sheet.at(i).function_code));
		m_register_list.SetItemText(i, _NUM_FUNCTION, strTemp);


		strTemp.Format(_T("%d"), m_register_data_sheet.at(i).Reg_ID);
		m_register_list.SetItemText(i, _NUM_MODBUSADDRESS, strTemp);
		strTemp.Format(_T("%d"), m_register_data_sheet.at(i).Counts_Number);
		m_register_list.SetItemText(i, _NUM_LENGTH, strTemp);

		m_register_list.SetItemText(i, _NUM_DATAFORMAT, m_register_data_sheet.at(i).DataFormat);

		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Bit_1);
		m_register_list.SetItemText(i, _NUM_BIT, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Low_Actual);
		m_register_list.SetItemText(i, _NUM_Low_Actual, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).High_Actual);
		m_register_list.SetItemText(i, _NUM_High_Actual, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Low_Scale);
		m_register_list.SetItemText(i, _NUM_Low_Scale, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).High_Scale);
		m_register_list.SetItemText(i, _NUM_High_Scale, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Read_Only_Or_RW);
		m_register_list.SetItemText(i, _NUM_RW, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Default);
		m_register_list.SetItemText(i, _NUM_DEFAULT, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Bacnet_Type);
		m_register_list.SetItemText(i, _NUM_Bacnet_Type, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Bacnet_Object_Description);
		m_register_list.SetItemText(i, _NUM_Bacnet_Object_Description, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).COV_Increment);
		m_register_list.SetItemText(i, _NUM_COV_Increment, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Unit_Group);
		m_register_list.SetItemText(i, _NUM_Unit_Group, strTemp);


		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Unit_Value);
		m_register_list.SetItemText(i, _NUM_Unit_Value, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Grouping_YES_NO);
		m_register_list.SetItemText(i, _NUM_Grouping_Y_N, strTemp);


		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Update_On_Reconnect);
		m_register_list.SetItemText(i, _NUM_Update_On_Reconnect, strTemp);


		strTemp.Format(_T("%s"), m_register_data_sheet.at(i).Reg_Description);
		m_register_list.SetItemText(i, _NUM_Modbus_DESCRIPTION, strTemp);

	}

	m_register_list.ShowWindow(SW_SHOW);
}
void CProductRegisterListView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}
void CProductRegisterListView::Import_CustomProductTable()
{
	CString StrSql;
	CustomProductTable_T Struc_Temp;
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q, q1;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);


	StrSql.Format(_T("Select * From ProductsTypeRegisterTables"));
	q = SqliteDBT3000.execQuery((UTF8MBSTR)StrSql);
	while (!q.eof())
	{
		CString TableName = q.getValuebyName(L"TableName");

		if (TableName.IsEmpty())
		{
			q.nextRow();
			continue;
		}
		int modelid = q.getIntField("ProductType");
		CString RegName = q.getValuebyName(L"Col_RegName");
		CString RegID = q.getValuebyName(L"Col_RegAddress");
		if (modelid != 21)
		{
			q.nextRow();
			continue;
		}
		/*if (modelid>=1&&modelid<=19)
		{
		StrSql.Format(_T("Select %s,%s, From %s "), RegName, RegID, TableName);
		}
		else
		{*/
		StrSql.Format(_T("Select %s,%s From %s "), RegName, RegID, TableName);

		/*}*/


		q1 = SqliteDBT3000.execQuery((UTF8MBSTR)StrSql);
		while (!q1.eof())
		{
			CString ModbusName = q1.getValuebyName(RegName);
			CString ModbusID = q1.getValuebyName(RegID);
			CString comments;
			if (ModbusName.CompareNoCase(L"RESERVED") == 0 || ModbusName.IsEmpty())
			{
				q1.nextRow();
				continue;
			}


			StrSql.Format(_T("Insert Into CustomProductTable(ModelNo,Para_Type,Reg_ID,SN) Values (%d,'%s',%d,'%s') ")
				, modelid
				, ModbusName
				, _wtoi(ModbusID)
				, GetGUID()
			);

			TRACE(StrSql);
			TRACE(L"\n");

			SqliteDBT3000.execDML((UTF8MBSTR)StrSql);

			q1.nextRow();
		}
		q1.finalize();
		break;
		q.nextRow();
	}
	q.finalize();
	SqliteDBT3000.closedb();


}
void CProductRegisterListView::LoadDataSheet() {
	CString StrSql;
	_variant_t temp_variant;
	int ProductModel = m_current_tree_node.product_class_id;
	CustomProductTable_T Struc_Temp;
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q;

	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

    if (ProductModel == PM_MINIPANEL_ARM)
        ProductModel = PM_MINIPANEL;

	if (m_sort_type == 0)
	{
		StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  "), ProductModel);
	}
	else if (m_sort_type == 1)
	{
		StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  ORDER BY Reg_ID DESC"), ProductModel);
	}
	else if (m_sort_type == 2)
	{
		StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  ORDER BY Reg_ID ASC"), ProductModel);
	}
	else
	{
		StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  "), ProductModel);
		//  StrSql.Format(_T("Select * From CustomProductTable Where ModelNo = %d  "), ProductModel);
	}
	// 

	m_register_data_sheet.clear();

	q = SqliteDBT3000.execQuery((UTF8MBSTR)StrSql);
	while (!q.eof())
	{


		Struc_Temp.ModelNo = q.getIntField("ModelNo", -1);//
		Struc_Temp.Poll_YES_NO = q.getValuebyName(L"Poll_YES_NO", L"Yes");
		Struc_Temp.Bacnet_Object_Name = q.getValuebyName(L"Bacnet_Object_Name", L"Object Name");//
		Struc_Temp.Reg_ID = q.getIntField("Reg_ID", -1);//
		Struc_Temp.Reg_Description = q.getValuebyName(L"Reg_Description", L"Description");//
		Struc_Temp.function_code = q.getIntField("Function_Code", 3);
		Struc_Temp.Counts_Number = q.getIntField("Counts_Number", 1);//
		Struc_Temp.DataFormat = q.getValuebyName(L"DataFormat", L"16 Bit Unsigned Integer");//
		Struc_Temp.SN = q.getValuebyName(L"SN", GetGUID());
		Struc_Temp.Default = q.getValuebyName(L"Default1", L"");//  
		Struc_Temp.Bit_1 = q.getValuebyName(L"Bit_1", L"NONE");//
		Struc_Temp.Low_Actual = q.getValuebyName(L"Low_Actual", L"0");//
		Struc_Temp.High_Actual = q.getValuebyName(L"High_Actual", L"1");//
		Struc_Temp.Low_Scale = q.getValuebyName(L"Low_Scale", L"0");//
		Struc_Temp.High_Scale = q.getValuebyName(L"High_Scale", L"1");//
		Struc_Temp.Read_Only_Or_RW = q.getValuebyName(L"Read_Only_Or_RW", L"R_Only");//
		Struc_Temp.Bacnet_Type = q.getValuebyName(L"Bacnet_Type", L"AI");//
		Struc_Temp.Bacnet_Object_Description = q.getValuebyName(L"Bacnet_Object_Description", L"");//
		Struc_Temp.COV_Increment = q.getValuebyName(L"COV_Increment", L"0");//
		Struc_Temp.Unit_Group = q.getValuebyName(L"Unit_Group", L"Other");//
		Struc_Temp.Unit_Value = q.getValuebyName(L"Unit_Value", L"NO UNITS");//
		Struc_Temp.Grouping_YES_NO = q.getValuebyName(L"Grouping_YES_NO", L"YES");//
		Struc_Temp.Update_On_Reconnect = q.getValuebyName(L"Update_On_Reconnect", L"NO");//

		if (Struc_Temp.SN.IsEmpty())
		{
			Struc_Temp.SN = GetGUID();
			StrSql.Format(_T("Update  CustomProductTable  Set SN = '%s' Where ModelNo = %d and Reg_ID = %d and Function_Code = %d"),
				Struc_Temp.SN, Struc_Temp.ModelNo, Struc_Temp.Reg_ID, Struc_Temp.function_code);
			SqliteDBT3000.execDML((UTF8MBSTR)StrSql);
		}

		m_register_data_sheet.push_back(Struc_Temp);

		q.nextRow();
	}

	SqliteDBT3000.closedb();
}
LRESULT CProductRegisterListView::Fresh_Input_List(WPARAM wParam, LPARAM lParam) {

	CString strTemp;
	int fresh_type = (int)wParam;
	int fresh_row = (int)lParam;
	if (fresh_type == 0)
	{
		LoadDataSheet();
		g_hwnd_now = this->m_hWnd;
		m_register_list.DeleteAllItems();
		ShowDataInList();
	}
	if (fresh_type == 1)
	{
		m_register_list.SetItemText(fresh_row, _NUM_POLLYESNO, m_register_data_sheet.at(fresh_row).Poll_YES_NO);


		m_register_list.SetItemText(fresh_row, _NUM_NAME, m_register_data_sheet.at(fresh_row).Bacnet_Object_Name);

		strTemp.Format(_T("%s"), GetFunctionName(m_register_data_sheet.at(fresh_row).function_code));
		m_register_list.SetItemText(fresh_row, _NUM_FUNCTION, strTemp);


		strTemp.Format(_T("%d"), m_register_data_sheet.at(fresh_row).Reg_ID);
		m_register_list.SetItemText(fresh_row, _NUM_MODBUSADDRESS, strTemp);
		strTemp.Format(_T("%d"), m_register_data_sheet.at(fresh_row).Counts_Number);
		m_register_list.SetItemText(fresh_row, _NUM_LENGTH, strTemp);

		m_register_list.SetItemText(fresh_row, _NUM_DATAFORMAT, m_register_data_sheet.at(fresh_row).DataFormat);

		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).Bit_1);
		m_register_list.SetItemText(fresh_row, _NUM_BIT, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).Low_Actual);
		m_register_list.SetItemText(fresh_row, _NUM_Low_Actual, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).High_Actual);
		m_register_list.SetItemText(fresh_row, _NUM_High_Actual, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).Low_Scale);
		m_register_list.SetItemText(fresh_row, _NUM_Low_Scale, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).High_Scale);
		m_register_list.SetItemText(fresh_row, _NUM_High_Scale, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).Read_Only_Or_RW);
		m_register_list.SetItemText(fresh_row, _NUM_RW, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).Default);
		m_register_list.SetItemText(fresh_row, _NUM_DEFAULT, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).Bacnet_Type);
		m_register_list.SetItemText(fresh_row, _NUM_Bacnet_Type, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).Bacnet_Object_Description);
		m_register_list.SetItemText(fresh_row, _NUM_Bacnet_Object_Description, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).COV_Increment);
		m_register_list.SetItemText(fresh_row, _NUM_COV_Increment, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).Unit_Group);
		m_register_list.SetItemText(fresh_row, _NUM_Unit_Group, strTemp);


		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).Unit_Value);
		m_register_list.SetItemText(fresh_row, _NUM_Unit_Value, strTemp);
		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).Grouping_YES_NO);
		m_register_list.SetItemText(fresh_row, _NUM_Grouping_Y_N, strTemp);


		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).Update_On_Reconnect);
		m_register_list.SetItemText(fresh_row, _NUM_Update_On_Reconnect, strTemp);


		strTemp.Format(_T("%s"), m_register_data_sheet.at(fresh_row).Reg_Description);
		m_register_list.SetItemText(fresh_row, _NUM_Modbus_DESCRIPTION, strTemp);

	}
	if (fresh_type == 2)
	{
		m_register_list.SetItemBkColor(fresh_row, -1, RGB(50, 50, 180));
	}
	if (fresh_type == 3)
	{
		m_register_list.SetItemBkColor(fresh_row, -1, RGB(0, 0, 0));
	}
	return 0;
}

LRESULT CProductRegisterListView::Change_Input_Item(WPARAM wParam, LPARAM lParam) {

	UpdateData(TRUE);
	int lRow = (int)wParam;
	int lCol = (int)lParam;
	CString StrSql;

	if (lRow >= m_register_data_sheet.size())
	{
		return 0;
	}
	CustomProductTable_T tmp = m_register_data_sheet.at(lRow);

	tmp.Poll_YES_NO = m_register_list.GetItemText(lRow, _NUM_POLLYESNO);
	tmp.Bacnet_Object_Name = m_register_list.GetItemText(lRow, _NUM_NAME);//CString  Bacnet_Object_Name;
	tmp.function_code = GetFunctionCode(m_register_list.GetItemText(lRow, _NUM_FUNCTION));
	CString strAddress = m_register_list.GetItemText(lRow, _NUM_MODBUSADDRESS);
	if (strAddress.IsEmpty())
	{
		AfxMessageBox(L"Reg Address can't be null");
		return 0;
	}
	tmp.Reg_ID = _wtoi(strAddress);
	tmp.Counts_Number = _wtoi(m_register_list.GetItemText(lRow, _NUM_LENGTH));;

	tmp.DataFormat = m_register_list.GetItemText(lRow, _NUM_DATAFORMAT);
	tmp.Bit_1 = m_register_list.GetItemText(lRow, _NUM_BIT);
	tmp.Low_Actual = m_register_list.GetItemText(lRow, _NUM_Low_Actual);;
	tmp.High_Actual = m_register_list.GetItemText(lRow, _NUM_High_Actual);;
	tmp.Low_Scale = m_register_list.GetItemText(lRow, _NUM_Low_Scale);;
	tmp.High_Scale = m_register_list.GetItemText(lRow, _NUM_High_Scale);;
	tmp.Read_Only_Or_RW = m_register_list.GetItemText(lRow, _NUM_RW);;
	tmp.Bacnet_Type = m_register_list.GetItemText(lRow, _NUM_Bacnet_Type);;
	tmp.Bacnet_Object_Description = m_register_list.GetItemText(lRow, _NUM_Bacnet_Object_Description);;
	tmp.COV_Increment = m_register_list.GetItemText(lRow, _NUM_COV_Increment);;
	tmp.Unit_Group = m_register_list.GetItemText(lRow, _NUM_Unit_Group);;
	tmp.Unit_Value = m_register_list.GetItemText(lRow, _NUM_Unit_Value);;
	tmp.Grouping_YES_NO = m_register_list.GetItemText(lRow, _NUM_Grouping_Y_N);;
	tmp.Update_On_Reconnect = m_register_list.GetItemText(lRow, _NUM_Update_On_Reconnect);;
	tmp.Default = m_register_list.GetItemText(lRow, _NUM_DEFAULT);;
	tmp.Reg_Description = m_register_list.GetItemText(lRow, _NUM_Modbus_DESCRIPTION);;

	int Int_Counts_Number = tmp.Counts_Number;
	if (Int_Counts_Number>100)
	{
		AfxMessageBox(_T("Counts Number should be less than 100"));
		StrSql.Format(_T("%d"), m_register_data_sheet.at(lRow).Counts_Number);
		m_register_list.SetItemText(lRow, lCol, StrSql);
		return 0;
	}
	//CustomProductTable_T Struct_Temp;

	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

	StrSql.Format(_T("Update  CustomProductTable  Set Poll_YES_NO = '%s' ,Bacnet_Object_Name = '%s' ,Reg_ID = %d ,Counts_Number = %d ,Reg_Description = '%s' ,Function_Code=%d,DataFormat='%s',Default1='%s',Bit_1='%s',Low_Actual='%s'  ,High_Actual='%s'  ,Low_Scale='%s'  ,High_Scale='%s'  ,Read_Only_Or_RW='%s',Bacnet_Type='%s',Bacnet_Object_Description='%s',COV_Increment='%s',Unit_Group='%s',Unit_Value='%s'  ,Grouping_YES_NO='%s'  ,Update_On_Reconnect='%s'  Where SN = '%s' "),
		tmp.Poll_YES_NO, tmp.Bacnet_Object_Name, tmp.Reg_ID
		, tmp.Counts_Number, tmp.Reg_Description, tmp.function_code,
		tmp.DataFormat, tmp.Default, tmp.Bit_1, tmp.Low_Actual,
		tmp.High_Actual, tmp.Low_Scale, tmp.High_Scale,
		tmp.Read_Only_Or_RW, tmp.Bacnet_Type, tmp.Bacnet_Object_Description,
		tmp.COV_Increment, tmp.Unit_Group, tmp.Unit_Value, tmp.Grouping_YES_NO,
		tmp.Update_On_Reconnect, tmp.SN
	);

	SqliteDBT3000.execDML((UTF8MBSTR)StrSql);
	m_register_data_sheet.at(lRow) = tmp;

	PostMessage(WM_REFRESH_BAC_INPUT_LIST, 1U, lRow);

	SqliteDBT3000.closedb();



	return 0;
}

/*
@1:Different Product responce to the different functionality
@2:
*/


void CProductRegisterListView::OnDestroy()
{
	g_bPauseMultiRead = FALSE;
	if (m_BackgroundTreadHandle)
	{
		if (WaitForSingleObject(m_BackgroundTreadHandle->m_hThread, 3000) == WAIT_OBJECT_0)
		{

		}
		else
		{
			BOOL bRet = TerminateThread(m_BackgroundTreadHandle->m_hThread, 0);
			//delete m_pFreshMultiRegisters;
			m_BackgroundTreadHandle = NULL;
		}

	}
	close_com();
	CFormView::OnDestroy();


}


void CProductRegisterListView::OnBnClickedReadDevice()
{

	if (m_isReading)
	{

		m_isReading = FALSE;
		GetDlgItem(IDC_READ_DEVICE)->SetWindowText(_T("Start Read"));
	}
	else
	{
		m_isReading = TRUE;
		GetDlgItem(IDC_READ_DEVICE)->SetWindowText(_T("Stop Read"));
	}
}


void CProductRegisterListView::OnBnClickedButton4()
{
	if (m_sort_type != 0)    //防止已经是默认排序了，还狂点默认排序
	{
		m_sort_type = 0;
		SendMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
		if (m_BackgroundTreadHandle)
		{
			BOOL bRet = TerminateThread(m_BackgroundTreadHandle->m_hThread, 0);
			m_BackgroundTreadHandle = NULL;
		}
		m_BackgroundTreadHandle = AfxBeginThread(_Background_Read, this);
	}


}


void CProductRegisterListView::OnBnClickedDownbutton()
{

	if (m_sort_type != 1)
	{
		m_sort_type = 1;
		SendMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
		if (m_BackgroundTreadHandle)
		{
			BOOL bRet = TerminateThread(m_BackgroundTreadHandle->m_hThread, 0);
			m_BackgroundTreadHandle = NULL;
		}
		m_BackgroundTreadHandle = AfxBeginThread(_Background_Read, this);
	}

}


void CProductRegisterListView::OnBnClickedUpbutton()
{

	if (m_sort_type != 2)
	{
		m_sort_type = 2;
		SendMessage(WM_REFRESH_BAC_INPUT_LIST, 0, 0);
		if (m_BackgroundTreadHandle)
		{
			BOOL bRet = TerminateThread(m_BackgroundTreadHandle->m_hThread, 0);
			m_BackgroundTreadHandle = NULL;
		}
		m_BackgroundTreadHandle = AfxBeginThread(_Background_Read, this);
	}
}


void CProductRegisterListView::OnTimer(UINT_PTR nIDEvent)
{

	if (nIDEvent == 1)
	{
		//UpdateData();
	}
	CFormView::OnTimer(nIDEvent);
}


void CProductRegisterListView::OnEnChangeEditDelayLoop()
{

	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData();
	// 
}


void CProductRegisterListView::OnEnChangeEditDelayItems()
{

	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData();
	// 
}





int CProductRegisterListView::GetFunctionCode(CString FunctionName)
{
	if (m_vecVariableType[0].CompareNoCase(FunctionName) == 0)
	{
		return 1;
	}
	if (m_vecVariableType[1].CompareNoCase(FunctionName) == 0)
	{
		return 2;
	}
	if (m_vecVariableType[2].CompareNoCase(FunctionName) == 0)
	{
		return 3;
	}
	if (m_vecVariableType[3].CompareNoCase(FunctionName) == 0)
	{
		return 4;
	}
	if (m_vecVariableType[4].CompareNoCase(FunctionName) == 0)
	{
		return 5;
	}
	if (m_vecVariableType[5].CompareNoCase(FunctionName) == 0)
	{
		return 6;
	}
	if (m_vecVariableType[6].CompareNoCase(FunctionName) == 0)
	{
		return 15;
	}
	if (m_vecVariableType[7].CompareNoCase(FunctionName) == 0)
	{
		return 16;
	}

}

CString CProductRegisterListView::GetFunctionName(int FunctionCode)
{
	if (FunctionCode == 1)
	{
		return m_vecVariableType[0];
	}
	if (FunctionCode == 2)
	{
		return m_vecVariableType[1];
	}

	if (FunctionCode == 3)
	{
		return m_vecVariableType[2];
	}
	if (FunctionCode == 4)
	{
		return m_vecVariableType[3];
	}
	if (FunctionCode == 5)
	{
		return m_vecVariableType[4];
	}
	if (FunctionCode == 6)
	{
		return m_vecVariableType[5];
	}
	if (FunctionCode == 15)
	{
		return m_vecVariableType[6];
	}
	if (FunctionCode == 16)
	{
		return m_vecVariableType[7];
	}
}

int CProductRegisterListView::GetCaculateCode(CString CalName)
{
	/*if (m_array_caculate[0].CompareNoCase(CalName) == 0)
	{
	return 0;
	}
	if (m_array_caculate[1].CompareNoCase(CalName) == 0)
	{
	return 1;
	}
	if (m_array_caculate[2].CompareNoCase(CalName) == 0)
	{
	return 2;
	}*/
	return 0;
}

CString CProductRegisterListView::GetCalName(int calcode)
{
	/*if (calcode == 0)
	{
	return m_array_caculate[0];
	}
	if (calcode == 1)
	{
	return m_array_caculate[1];
	}

	if (calcode == 2)
	{
	return m_array_caculate[2];
	}*/
	return L"";
}


vector<CString> CProductRegisterListView::GetUnitValueByGroupUnit(CString strGroupUnit)
{
	vector<CString> vecTmp;
	if (strGroupUnit.CompareNoCase(L"Acceleration") == 0)
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
vector<CString> CProductRegisterListView::GetDataFormatByVariable(CString strVariable)
{
	vector<CString> vecTmp;
	if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0)
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
	else if (strVariable.CompareNoCase(L"Read_Coils") == 0 || strVariable.CompareNoCase(L"Read_Discrete_Inputs") == 0)
	{
		vecTmp.clear();
		vecTmp.push_back(L"bit");
	}
	else if (strVariable.CompareNoCase(L"Read_Input_Registers") == 0)
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
	else
	{
		vecTmp.clear();
	}

	return vecTmp;

}
vector<CString> CProductRegisterListView::GetRWByVariableDataFormat(CString strVariable, CString strDataFormat)
{


	vector<CString> vecTmp;
	if (
		(strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"16 Bit Unsigned Integer") == 0) ||
		(strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"16 Bit Signed Integer") == 0) ||
		(strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"32 Bit Unsigned Integer HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"32 Bit Signed Integer HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"32 Bit Signed Integer LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"32 Bit Unsigned Integer LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Floating Point HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Floating Point LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"_16 Bit Bit Pick") == 0) ||
		(strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"Read_Coils") == 0)
		)
	{
		vecTmp.push_back(L"R_W");
		vecTmp.push_back(L"R_Only");
	}
	else if (
		(strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"16 Bit Unsigned Integer") == 0) ||
		(strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"16 Bit Signed Integer") == 0) ||
		(strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"32 Bit Unsigned Integer HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"32 Bit Signed Integer HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"32 Bit Signed Integer LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"32 Bit Unsigned Integer LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"Floating Point HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"Floating Point LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"_16 Bit Bit Pick") == 0) ||
		(strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick HI_LO") == 0) ||
		(strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick LO_HI") == 0) ||
		(strVariable.CompareNoCase(L"Read_Discrete_Inputs") == 0)
		)
	{
		vecTmp.push_back(L"R_Only");
	}

	return vecTmp;
}
vector<CString> CProductRegisterListView::GetBacnetType(CString strVariable, CString strDataFormat, CString RW)
{
	vector<CString> vecTmp;
	if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"16 Bit Unsigned Integer") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"AO");
		/*  Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"16 Bit Unsigned Integer") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		/*Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"16 Bit Signed Integer") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"AO");
		/*Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"16 Bit Signed Integer") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		/*Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Bit Unsigned Integer HI_LO") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"AO");
		vecTmp.push_back(L"UVRW");
		/* Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Bit Unsigned Integer HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		vecTmp.push_back(L"UVRO");
		/*Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Bit Signed Integer HI_LO") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"AO");
		vecTmp.push_back(L"IVRW");
		/* Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Bit Signed Integer HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		vecTmp.push_back(L"IVRO");
		/* Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Bit Signed Integer LO_HI") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"AO");
		vecTmp.push_back(L"IVRW");
		/*Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Bit Signed Integer LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		vecTmp.push_back(L"IVRO");
		/* Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Bit Unsigned Integer LO_HI") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"AO");
		vecTmp.push_back(L"UVRW");
		/*  Future 2
		Future 3
		Future 4
		Future 5*/

	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Bit Unsigned Integer LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		vecTmp.push_back(L"UVRO");
		/* Future 2
		Future 3
		Future 4
		Future 5*/
	}

	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Floating Point HI_LO") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"AO");
		/* Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}

	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Floating Point HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		/* Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}

	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Floating Point LO_HI") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"AO");
		/*  Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}

	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"Floating Point LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		///* Future 1
		// Future 2
		// Future 3
		// Future 4
		// Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"_16 Bit Bit Pick") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"BO");
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"_16 Bit Bit Pick") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"BI");
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick HI_LO") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"BO");
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"BI");
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick LO_HI") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"BO");
	}
	else if (strVariable.CompareNoCase(L"Read_Holding_Registers") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"BI");
	}
	else if (strVariable.CompareNoCase(L"Read_Coils") == 0 && strDataFormat.CompareNoCase(L"Bit") == 0 && RW.CompareNoCase(L"R_W") == 0)
	{
		vecTmp.push_back(L"BO");
	}
	else if (strVariable.CompareNoCase(L"Read_Coils") == 0 && strDataFormat.CompareNoCase(L"Bit") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"BI");
	}
	else if (strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"16 Bit Unsigned Integer") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		/* Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"16 Bit Signed Integer") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		/* Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"32 Bit Unsigned Integer HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		vecTmp.push_back(L"UVRO");
		/*Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"32 Bit Signed Integer HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		vecTmp.push_back(L"IVRO");
		/* Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"32 Bit Signed Integer LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		vecTmp.push_back(L"IVRO");
		/*  Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"32 Bit Unsigned Integer LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		vecTmp.push_back(L"UVRO");
		/* Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"Floating Point HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		/* Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"Floating Point LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"AI");
		/* Future 1
		Future 2
		Future 3
		Future 4
		Future 5*/
	}
	else if (strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"_16 Bit Bit Pick") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"BI");
	}

	else if (strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick HI_LO") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"BI");
	}

	else if (strVariable.CompareNoCase(L"Read_Input_Registers") == 0 && strDataFormat.CompareNoCase(L"_32 Bit Bit Pick LO_HI") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"BI");
	}
	else if (strVariable.CompareNoCase(L"Read_Discrete_Inputs") == 0 && strDataFormat.CompareNoCase(L"Bit") == 0 && RW.CompareNoCase(L"R_Only") == 0)
	{
		vecTmp.push_back(L"BI");
	}
	else
	{
	}


	return vecTmp;
}


void CProductRegisterListView::OnNMClickList_output(NMHDR *pNMHDR, LRESULT *pResult) {

	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString temp_cstring;
	long lRow, lCol;
	m_register_list.Set_Edit(true);
	DWORD dwPos = GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_register_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	int nItem = m_register_list.SubItemHitTest(&lvinfo);
	BOOL Is_Range = FALSE;
	int Range_Address = -1;
	int Value_Address = -1;
	int Value_Length = 0;
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;

	if (lRow>m_register_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if (lRow<0)
		return;
	if (lRow<m_register_data_sheet.size())
	{
		if (lCol == _NUM_VALUE)
		{
			if (offline_mode)
			{
				return;
			}
			g_current_Node = m_register_data_sheet.at(lRow);
			if (g_current_Node.Read_Only_Or_RW.CompareNoCase(_T("Read Only")) != 0)
			{
				m_isReading = FALSE;

				CWriteRegistersValueDlg dlg;
				dlg.DoModal();
				m_isReading = TRUE;
			}

		}

		if (lCol == _NUM_POLLYESNO)
		{
			if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(_NUM_POLLYESNO))
			{
				ListCtrlEx::CStrList strlist;
				for (int aaa = 0; aaa <m_vecYesNo.size(); aaa++)
				{
					strlist.push_back(m_vecYesNo[aaa]);
				}
				m_register_list.SetCellStringList(lRow, _NUM_POLLYESNO, strlist);
			}
		}
		if (lCol == _NUM_FUNCTION)
		{
			if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(_NUM_FUNCTION))
			{
				ListCtrlEx::CStrList strlist;
				for (int aaa = 0; aaa < m_vecVariableType.size(); aaa++)
				{
					strlist.push_back(m_vecVariableType[aaa]);
				}
				m_register_list.SetCellStringList(lRow, _NUM_FUNCTION, strlist);
			}
		}
		if (lCol == _NUM_Unit_Group)
		{
			if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(_NUM_Unit_Group))
			{
				ListCtrlEx::CStrList strlist;
				for (int aaa = 0; aaa < m_GroupUnits.size(); aaa++)
				{
					strlist.push_back(m_GroupUnits[aaa]);
				}
				m_register_list.SetCellStringList(lRow, _NUM_Unit_Group, strlist);
			}
		}


		//5=数据格式
		if (lCol == _NUM_DATAFORMAT)
		{
			m_vecDataFormat = GetDataFormatByVariable(GetFunctionName(m_register_data_sheet[lRow].function_code));

			if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(_NUM_DATAFORMAT))
			{
				ListCtrlEx::CStrList strlist;
				for (int aaa = 0; aaa < m_vecDataFormat.size(); aaa++)
				{
					strlist.push_back(m_vecDataFormat[aaa]);
				}
				m_register_list.SetCellStringList(lRow, _NUM_DATAFORMAT, strlist);
			}
		}
		//11=读写属性的判断
		if (lCol == _NUM_RW)
		{
			m_vecDataRW = GetRWByVariableDataFormat(GetFunctionName(m_register_data_sheet[lRow].function_code), m_register_data_sheet[lRow].DataFormat);
			if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(_NUM_RW))
			{
				ListCtrlEx::CStrList strlist;
				for (int aaa = 0; aaa < m_vecDataRW.size(); aaa++)
				{
					strlist.push_back(m_vecDataRW[aaa]);
				}
				m_register_list.SetCellStringList(lRow, _NUM_RW, strlist);
			}
		}
		//12=Bacnet Type 判断
		if (lCol == _NUM_Bacnet_Type)
		{
			m_vecBacnetType = GetBacnetType(GetFunctionName(m_register_data_sheet[lRow].function_code), m_register_data_sheet[lRow].DataFormat, m_register_data_sheet[lRow].Read_Only_Or_RW);
			if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(_NUM_Bacnet_Type))
			{
				ListCtrlEx::CStrList strlist;
				for (int aaa = 0; aaa < m_vecBacnetType.size(); aaa++)
				{
					strlist.push_back(m_vecBacnetType[aaa]);
				}
				m_register_list.SetCellStringList(lRow, _NUM_Bacnet_Type, strlist);
			}
		}
		//
		if (lCol == _NUM_Unit_Value)
		{
			m_UnitValue = GetUnitValueByGroupUnit(m_register_data_sheet[lRow].Unit_Group);
			if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(_NUM_Unit_Value))
			{
				ListCtrlEx::CStrList strlist;
				for (int aaa = 0; aaa < m_UnitValue.size(); aaa++)
				{
					strlist.push_back(m_UnitValue[aaa]);
				}
				m_register_list.SetCellStringList(lRow, _NUM_Unit_Value, strlist);
			}
		}

		if (lCol == _NUM_Grouping_Y_N)
		{
			if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(_NUM_Grouping_Y_N))
			{
				ListCtrlEx::CStrList strlist;
				for (int aaa = 0; aaa < m_vecYesNo.size(); aaa++)
				{
					strlist.push_back(m_vecYesNo[aaa]);
				}
				m_register_list.SetCellStringList(lRow, _NUM_Grouping_Y_N, strlist);
			}
		}

		if (lCol == _NUM_Update_On_Reconnect)
		{
			if (ListCtrlEx::ComboBox == m_register_list.GetColumnType(_NUM_Update_On_Reconnect))
			{
				ListCtrlEx::CStrList strlist;
				for (int aaa = 0; aaa < m_vecYesNo.size(); aaa++)
				{
					strlist.push_back(m_vecYesNo[aaa]);
				}
				m_register_list.SetCellStringList(lRow, _NUM_Update_On_Reconnect, strlist);
			}
		}


	}
}
void CProductRegisterListView::OnNMDblclkListCustomList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString temp_cstring;
	long lRow, lCol;
	m_register_list.Set_Edit(true);
	DWORD dwPos = GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_register_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	int nItem = m_register_list.SubItemHitTest(&lvinfo);
	BOOL Is_Range = FALSE;
	int Range_Address = -1;
	int Value_Address = -1;
	int Value_Length = 0;
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;

	if (lRow<0 || lRow > m_register_list.GetItemCount())
	{
		CustomProductTable_T tmp;

		tmp.ModelNo = m_current_tree_node.product_class_id;
		tmp.Poll_YES_NO = L"YES";
		tmp.Bacnet_Object_Name = L"BNameDemo";//CString  Bacnet_Object_Name;
		tmp.function_code = 3;
		tmp.Reg_ID = -1;
		tmp.Counts_Number = 1;
		tmp.Value = L"0";
		tmp.DataFormat = L"16 Bit Unsigned Integer";
		tmp.Bit_1 = L"NONE";
		tmp.Low_Actual = L"0";
		tmp.High_Actual = L"1";
		tmp.Low_Scale = L"0";
		tmp.High_Scale = L"1";
		tmp.Read_Only_Or_RW = L"R_Only";
		tmp.Bacnet_Type = L"AI";
		tmp.Bacnet_Object_Description = L"Description";
		tmp.COV_Increment = L"0";
		tmp.Unit_Group = L"Other";
		tmp.Unit_Value = L"NO_UNITS";
		tmp.Grouping_YES_NO = L"Yes";
		tmp.Update_On_Reconnect = "NO";
		tmp.Default = L"Default";
		tmp.Reg_Description = L"Description";
		tmp.SN = GetGUID();
		int row = m_register_list.GetRowCount();
		FreshOneRowInGrid(row, tmp, 0);
		m_register_data_sheet.push_back(tmp);

		CppSQLite3DB SqliteDBT3000;
		CppSQLite3Query q;
		SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);
		CString StrSql;
		StrSql.Format(_T("Insert Into  CustomProductTable(ModelNo,Poll_YES_NO,Bacnet_Object_Name,Reg_ID,Reg_Description,Function_Code,Counts_Number,DataFormat,SN,Default1,Bit_1,Low_Actual,High_Actual,Low_Scale,High_Scale,Read_Only_Or_RW,Bacnet_Type,Bacnet_Object_Description,COV_Increment,Unit_Group,Unit_Value,Grouping_YES_NO,Update_On_Reconnect) \
                        Values(%d,'%s','%s',%d,'%s',%d,%d,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')"),
			tmp.ModelNo, tmp.Poll_YES_NO, tmp.Bacnet_Object_Name, tmp.Reg_ID
			, tmp.Reg_Description, tmp.Counts_Number, tmp.function_code,
			tmp.DataFormat, tmp.SN, tmp.Default, tmp.Bit_1, tmp.Low_Actual,
			tmp.High_Actual, tmp.Low_Scale, tmp.High_Scale,
			tmp.Read_Only_Or_RW, tmp.Bacnet_Type, tmp.Bacnet_Object_Description,
			tmp.COV_Increment, tmp.Unit_Group, tmp.Unit_Value, tmp.Grouping_YES_NO,
			tmp.Update_On_Reconnect);
		SqliteDBT3000.execDML((UTF8MBSTR)StrSql);
		SqliteDBT3000.closedb();
	}


	*pResult = 0;
}
void CProductRegisterListView::FreshOneRowInGrid(int Row, CustomProductTable_T tp, int operator_Grid)
{
	CString strTemp;
	if (operator_Grid == 0)//插入到最后一行
	{
		strTemp.Format(_T("%d"), Row + 1);
		m_register_list.InsertItem(Row, strTemp);
		m_register_list.SetItemText(Row, _NUM_POLLYESNO, tp.Poll_YES_NO);


		m_register_list.SetItemText(Row, _NUM_NAME, tp.Bacnet_Object_Name);

		strTemp.Format(_T("%s"), GetFunctionName(tp.function_code));
		m_register_list.SetItemText(Row, _NUM_FUNCTION, strTemp);


		strTemp.Format(_T("%d"), tp.Reg_ID);
		m_register_list.SetItemText(Row, _NUM_MODBUSADDRESS, strTemp);
		strTemp.Format(_T("%d"), tp.Counts_Number);
		m_register_list.SetItemText(Row, _NUM_LENGTH, strTemp);

		m_register_list.SetItemText(Row, _NUM_DATAFORMAT, tp.DataFormat);

		strTemp.Format(_T("%s"), tp.Bit_1);
		m_register_list.SetItemText(Row, _NUM_BIT, strTemp);
		strTemp.Format(_T("%s"), tp.Low_Actual);
		m_register_list.SetItemText(Row, _NUM_Low_Actual, strTemp);
		strTemp.Format(_T("%s"), tp.High_Actual);
		m_register_list.SetItemText(Row, _NUM_High_Actual, strTemp);
		strTemp.Format(_T("%s"), tp.Low_Scale);
		m_register_list.SetItemText(Row, _NUM_Low_Scale, strTemp);
		strTemp.Format(_T("%s"), tp.High_Scale);
		m_register_list.SetItemText(Row, _NUM_High_Scale, strTemp);
		strTemp.Format(_T("%s"), tp.Read_Only_Or_RW);
		m_register_list.SetItemText(Row, _NUM_RW, strTemp);
		strTemp.Format(_T("%s"), tp.Default);
		m_register_list.SetItemText(Row, _NUM_DEFAULT, strTemp);
		strTemp.Format(_T("%s"), tp.Bacnet_Type);
		m_register_list.SetItemText(Row, _NUM_Bacnet_Type, strTemp);
		strTemp.Format(_T("%s"), tp.Bacnet_Object_Description);
		m_register_list.SetItemText(Row, _NUM_Bacnet_Object_Description, strTemp);
		strTemp.Format(_T("%s"), tp.COV_Increment);
		m_register_list.SetItemText(Row, _NUM_COV_Increment, strTemp);
		strTemp.Format(_T("%s"), tp.Unit_Group);
		m_register_list.SetItemText(Row, _NUM_Unit_Group, strTemp);


		strTemp.Format(_T("%s"), tp.Unit_Value);
		m_register_list.SetItemText(Row, _NUM_Unit_Value, strTemp);
		strTemp.Format(_T("%s"), tp.Grouping_YES_NO);
		m_register_list.SetItemText(Row, _NUM_Grouping_Y_N, strTemp);


		strTemp.Format(_T("%s"), tp.Update_On_Reconnect);
		m_register_list.SetItemText(Row, _NUM_Update_On_Reconnect, strTemp);


		strTemp.Format(_T("%s"), tp.Reg_Description);
		m_register_list.SetItemText(Row, _NUM_Modbus_DESCRIPTION, strTemp);
	}
	else if (operator_Grid == 1)//更新改行的数据
	{

	}
	else if (operator_Grid == 2)//删除改行的数据
	{

	}
}

void CProductRegisterListView::OnCbnSelchangeComboProductList()
{
	CString strProductName;
	int index = m_combox_productname.GetCurSel();
	m_combox_productname.GetLBText(index, strProductName);

	//m_combox_productname.GetLBText()
	for (pidname_map::iterator iter = product_map.begin(); iter != product_map.end(); iter++)
	{
		//m_combox_productname.AddString(iter->second);
		if (strProductName.CompareNoCase(iter->second) == 0)
		{
			m_current_tree_node.product_class_id = iter->first;
		}
	}

	LoadDataSheet();
	Initial_List();
}


#include "excel9.h"
void CProductRegisterListView::OnBnClickedButtonExportexcelfile()
{
	CString  Product_Head_File_Name;
	CString strFilter;
	CString strFilename;
	CString strTemp;
	CString RegisterName;
	CString RegisterID;
	CString logstr;
	_variant_t  temp_variant;
	_Application app;
	Workbooks books;
	_Workbook book;
	Worksheets sheets;
	_Worksheet sheet;
	Range range;
	Range rgMyRge1, rgMyRge2;
	COleVariant covTrue((short)TRUE), covFalse((short)FALSE), covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	int CurrentRow;


	if (!app.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("Create Excel false!"));
		return;
	}
	//遍历所有行  
	strFilename = g_strExePth + _T("RegisterListTemplate.xls");

	books.AttachDispatch(app.GetWorkbooks());
	book.AttachDispatch(books.Add(_variant_t(strFilename)));

	sheets.AttachDispatch(book.GetWorksheets());
	sheet.AttachDispatch(sheets.GetItem(_variant_t("Sheet1")));

	range.AttachDispatch(sheet.GetCells());
	CString ContentData;
	CTime time = CTime::GetCurrentTime();


	CString strProductName;
	int index = m_combox_productname.GetCurSel();
	m_combox_productname.GetLBText(index, strProductName);



	range.SetItem(_variant_t((long)(1)), _variant_t((long)(1)), _variant_t(strProductName));


	strProductName = L"Item";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_ITEM + 1)), _variant_t(strProductName));
	strProductName = L"Poll Y/N";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_POLLYESNO + 1)), _variant_t(strProductName));
	strProductName = L"Bacnet Object Name";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_NAME + 1)), _variant_t(strProductName));
	strProductName = L"Modbus Variable Type";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_FUNCTION + 1)), _variant_t(strProductName));
	strProductName = L"Modbus Address";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_MODBUSADDRESS + 1)), _variant_t(strProductName));
	strProductName = L"Length";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_LENGTH + 1)), _variant_t(strProductName));
	strProductName = L"Value";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_VALUE + 1)), _variant_t(strProductName));
	strProductName = L"Data Format";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_DATAFORMAT + 1)), _variant_t(strProductName));
	strProductName = L"Bit #";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_BIT + 1)), _variant_t(strProductName));
	strProductName = L"Low Actual";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_Low_Actual + 1)), _variant_t(strProductName));
	strProductName = L"High Actual";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_High_Actual + 1)), _variant_t(strProductName));
	strProductName = L"Low Scale";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_Low_Scale + 1)), _variant_t(strProductName));
	strProductName = L"High Scale";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_High_Scale + 1)), _variant_t(strProductName));
	strProductName = L"R/W";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_RW + 1)), _variant_t(strProductName));
	strProductName = L"Default";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_DEFAULT + 1)), _variant_t(strProductName));
	strProductName = L"Bacnet Type";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_Bacnet_Type + 1)), _variant_t(strProductName));
	strProductName = L"Bacnet Object Description";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_Bacnet_Object_Description + 1)), _variant_t(strProductName));
	strProductName = L"COV Increment";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_COV_Increment + 1)), _variant_t(strProductName));
	strProductName = L"Unit Group";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_Unit_Group + 1)), _variant_t(strProductName));
	strProductName = L"Unit Value";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_Unit_Value + 1)), _variant_t(strProductName));
	strProductName = L"Grouping YES / NO";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_Grouping_Y_N + 1)), _variant_t(strProductName));
	strProductName = L"Update On Reconnect";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_Update_On_Reconnect + 1)), _variant_t(strProductName));
	strProductName = L"Register Description";
	range.SetItem(_variant_t((long)(2)), _variant_t((long)(_NUM_Modbus_DESCRIPTION + 1)), _variant_t(strProductName));

	range.AttachDispatch(sheet.GetCells());

	for (int i = 0; i < (int)m_register_list.GetRowCount(); i++)
	{
		CurrentRow = 3 + i;
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_ITEM);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_ITEM + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_POLLYESNO);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_POLLYESNO + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_NAME);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_NAME + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_FUNCTION);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_FUNCTION + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_MODBUSADDRESS);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_MODBUSADDRESS + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_LENGTH);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_LENGTH + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_VALUE);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_VALUE + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_DATAFORMAT);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_DATAFORMAT + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_BIT);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_BIT + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_Low_Actual);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_Low_Actual + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_High_Actual);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_High_Actual + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_Low_Scale);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_Low_Scale + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_High_Scale);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_High_Scale + 1)), _variant_t(strProductName));

		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_RW);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_RW + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_DEFAULT);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_DEFAULT + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_Bacnet_Type);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_Bacnet_Type + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_Bacnet_Object_Description);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_Bacnet_Object_Description + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_COV_Increment);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_COV_Increment + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_Unit_Group);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_Unit_Group + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_Unit_Value);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_Unit_Value + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_Grouping_Y_N);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_Grouping_Y_N + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_Update_On_Reconnect);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_Update_On_Reconnect + 1)), _variant_t(strProductName));
		strProductName = m_register_list.GetItemText(CurrentRow, _NUM_Modbus_DESCRIPTION);
		range.SetItem(_variant_t((long)(CurrentRow)), _variant_t((long)(_NUM_Modbus_DESCRIPTION + 1)), _variant_t(strProductName));



	}

	AfxMessageBox(_T("Export Completely"));

	app.SetVisible(true);
	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();
	app.ReleaseDispatch();
}
