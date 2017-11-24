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
END_MESSAGE_MAP()


// CModbusToBacnetRouterDlg message handlers


BOOL CModbusToBacnetRouterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	for (int i = 0;i < 255;i++)
	{
		ModbusToBacnetRouter tmpnode;
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
	m_datalist.InsertColumn(1, _T("Poll YES / NO"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(2, _T("Bacnet Object Name"), 120, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(3, _T("Modbus Variable Type"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(4, _T("1 to 65536"), 120, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(5, _T("Data Format	"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(6, _T("Bit #"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(7, _T("High Actual"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(8, _T("Low Scale"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(9, _T("High Scale"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(10, _T("Read Only or R_W"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(11, _T("Bacnet Type(AI etc.)"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(12, _T("Bacnet Object Description"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(13, _T("COV Increment"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(14, _T("Unit Group"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(15, _T("Unit Value"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(16, _T("Grouping YES / NO"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(17, _T("Update On Reconnect"), 120, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
			
	for (int i = 0;i < m_vecMd2BT.size();i++)
	{
		CString strTmp;
		strTmp.Format(_T("%d"), i + 1);
		m_datalist.InsertItem(i, strTmp);

		m_datalist.SetItemText(i, 1, m_vecMd2BT[i].Bacnet_Object_Name);
		m_datalist.SetItemText(i, 2, m_vecMd2BT[i].Modbus_Variable_Type);
		m_datalist.SetItemText(i, 3, m_vecMd2BT[i].From1_To_65536 );
		m_datalist.SetItemText(i, 4, m_vecMd2BT[i].Data_Format );
		m_datalist.SetItemText(i, 5, m_vecMd2BT[i].Bit_1);
		m_datalist.SetItemText(i, 6, m_vecMd2BT[i].Low_Actual );
		m_datalist.SetItemText(i, 7, m_vecMd2BT[i].High_Actual );
		m_datalist.SetItemText(i, 8, m_vecMd2BT[i].Low_Scale );
		m_datalist.SetItemText(i, 9, m_vecMd2BT[i].High_Scale );
		m_datalist.SetItemText(i, 10, m_vecMd2BT[i].Read_Only_Or_RW );
		m_datalist.SetItemText(i, 11, m_vecMd2BT[i].Bacnet_Type );
		m_datalist.SetItemText(i, 12, m_vecMd2BT[i].Bacnet_Object_Description);
		m_datalist.SetItemText(i, 13, m_vecMd2BT[i].COV_Increment);
		m_datalist.SetItemText(i, 14, m_vecMd2BT[i].Unit_Group );
		m_datalist.SetItemText(i, 15, m_vecMd2BT[i].Unit_Value );
		m_datalist.SetItemText(i, 16, m_vecMd2BT[i].Grouping_YES_NO );
		m_datalist.SetItemText(i, 17, m_vecMd2BT[i].Update_On_Reconnect );
	}
			
	m_combox_productname.SetWindowTextW(_T("PS100"));
	m_combox_rev.SetWindowTextW(_T("A"));
		
			
			
		
			
			
			
			
			
			
			
			
			
		

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CModbusToBacnetRouterDlg::OnBnClickedButtonBlxfile()
{
	// TODO: Add your control notification handler code here
}
