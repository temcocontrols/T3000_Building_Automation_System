// GroupScheduleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "GroupScheduleDlg.h"
#include "afxdialogex.h"
#include "../SQLiteDriver/CppSQLite3.h"

// CGroupScheduleDlg dialog

IMPLEMENT_DYNAMIC(CGroupScheduleDlg, CDialogEx)

CGroupScheduleDlg::CGroupScheduleDlg(CWnd* pParent /*=NULL*/,CString minipanelip)
	: CDialogEx(IDD_DIALOG_MINIPANEL_GROUP_SCHEDULE, pParent)
{
	m_Minipanel_IP = minipanelip;
}

CGroupScheduleDlg::~CGroupScheduleDlg()
{
}

void CGroupScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_datalist);
}


BEGIN_MESSAGE_MAP(CGroupScheduleDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CGroupScheduleDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CGroupScheduleDlg message handlers


void CGroupScheduleDlg::Initial_datalist()
{ 
	for (int i = 0;i < (int)m_Weekly_data.size();i++)
	{
		CString temp_des;

		MultiByteToWideChar(CP_ACP, 0, (char *)m_Weekly_data.at(i).description, (int)strlen((char *)m_Weekly_data.at(i).description) + 1,
			temp_des.GetBuffer(MAX_PATH), MAX_PATH);
		temp_des.ReleaseBuffer();

		m_ScheduleName.push_back(temp_des);
	}

	//const CString ArrySchedule[] =
	//{
	//	_T("Schedule1"),
	//	_T("Schedule2"),
	//	_T("Schedule3"),
	//	_T("Schedule4"),
	//	_T("Schedule5"),
	//	_T("Schedule6"),
	//	_T("Schedule7"),
	//	_T("Schedule8")
	//};

	m_datalist.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_datalist.SetExtendedStyle(m_datalist.GetExtendedStyle() | LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));
	m_datalist.InsertColumn(0, _T("Item"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_datalist.InsertColumn(1, _T("Device ID"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(2, _T("Online"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(3, _T("Zone Name"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(4, _T("Schedule"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(5, _T("Status"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(6, _T("A/M"), 90, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_datalist.InsertColumn(7, _T("Value"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

	/*ListCtrlEx::CStrList Device_Strlist;
	*/
	ListCtrlEx::CStrList Schedule_Strlist;
	for (int j = 0;j < (int)m_ScheduleName.size();j++)
	{
		if (m_ScheduleName[j].IsEmpty())
		{
			m_ScheduleName[j].Format(_T("Schedudle%d"), j);
		}
		Schedule_Strlist.push_back(m_ScheduleName[j]);
	}
	CString strTemp;
	for (int i = 0;i < 254;i++)
	{
		strTemp.Format(_T("%d"), i + 1);
		m_datalist.InsertItem(i, strTemp);
	}
	for (int j = 0;j < (int)m_vecTstatID.size();j++)
	{
		 
		m_datalist.SetItemText(j, 1, m_vecTstatID[j].TstatID);
		CString strTemp;
		if (m_vecTstatID[j].OnlineStatus)
		{
			strTemp = L"Online";
		} 
		else
		{
			strTemp = L"Offline";
		}
		m_datalist.SetItemText(j, 2, strTemp);
		/*strTemp.Format(_T("Zone%d"), j);*/
		m_datalist.SetItemText(j, 3, m_vecTstatID[j].TstatName);
		m_datalist.SetItemText(j, 4, m_ScheduleName[j]);

		if (ListCtrlEx::ComboBox == m_datalist.GetColumnType(4))
		{
			m_datalist.SetCellStringList(j, 4, Schedule_Strlist);
		}

	}
	m_datalist.SetItemText(0, 5, L"ON");
	m_datalist.SetItemText(0, 6, L"MAN");
	m_datalist.SetItemText(0, 7, L"OFF");

	m_datalist.SetItemText(1, 5, L"ON");
	m_datalist.SetItemText(1, 6, L"Auto");
	m_datalist.SetItemText(1, 7, L"ON");

	m_datalist.SetItemText(2, 1, L"252");
	m_datalist.SetItemText(2, 2, L"Offline");
	m_datalist.SetItemText(2, 3, L"Zone2");
	m_datalist.SetItemText(2, 4, L"Not Assigned");
	m_datalist.SetItemText(2, 5, L"N/A");
	m_datalist.SetItemText(2, 6, L"N/A");
	m_datalist.SetItemText(2, 7, L"N/A");

	m_datalist.SetItemBkColor(2, -1, RGB(190, 190, 190));
	/*m_datalist.SetItemText(0, 1, L"8");
	m_datalist.SetItemText(0, 2, L"Schedule1");
	m_datalist.SetItemText(0, 3, L"ON");
	m_datalist.SetItemText(0, 4, L"Auto");
	m_datalist.SetItemText(0, 5, L"ON");

	m_datalist.SetItemText(1, 1, L"9");
	m_datalist.SetItemText(1, 2, L"Schedule1");
	m_datalist.SetItemText(1, 3, L"ON");
	m_datalist.SetItemText(1, 4, L"Auto");
	m_datalist.SetItemText(1, 5, L"ON");
	
	m_datalist.SetItemText(2, 1, L"10");
	m_datalist.SetItemText(2, 2, L"Schedule3");
	m_datalist.SetItemText(2, 3, L"OFF");
	m_datalist.SetItemText(2, 4, L"Man");
	m_datalist.SetItemText(2, 5, L"OFF");

	m_datalist.SetItemText(3, 1, L"11");
	m_datalist.SetItemText(3, 2, L"Schedule3");
	m_datalist.SetItemText(3, 3, L"OFF");
	m_datalist.SetItemText(3, 4, L"Man");
	m_datalist.SetItemText(3, 5, L"OFF");

	m_datalist.SetItemText(4, 1, L"12");
	m_datalist.SetItemText(4, 2, L"Schedule3");
	m_datalist.SetItemText(4, 3, L"OFF");
	m_datalist.SetItemText(4, 4, L"Man");
	m_datalist.SetItemText(4, 5, L"OFF");

	m_datalist.SetItemText(5, 1, L"13");
	m_datalist.SetItemText(5, 2, L"Schedule3");
	m_datalist.SetItemText(5, 3, L"OFF");
	m_datalist.SetItemText(5, 4, L"Man");
	m_datalist.SetItemText(5, 5, L"OFF");

	m_datalist.SetItemText(6, 1, L"17");
	m_datalist.SetItemText(6, 2, L"Schedule4");
	m_datalist.SetItemText(6, 3, L"ON");
	m_datalist.SetItemText(6, 4, L"Auto");
	m_datalist.SetItemText(6, 5, L"ON");

	m_datalist.SetItemText(7, 1, L"18");
	m_datalist.SetItemText(7, 2, L"Schedule4");
	m_datalist.SetItemText(7, 3, L"ON");
	m_datalist.SetItemText(7, 4, L"Auto");
	m_datalist.SetItemText(7, 5, L"ON");

	m_datalist.SetItemText(8, 1, L"20");
	m_datalist.SetItemText(8, 2, L"Schedule4");
	m_datalist.SetItemText(8, 3, L"ON");
	m_datalist.SetItemText(8, 4, L"Auto");
	m_datalist.SetItemText(8, 5, L"ON");*/

}


BOOL CGroupScheduleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CppSQLite3DB m_SqliteDBT3000;
	CppSQLite3Query m_q;
	m_SqliteDBT3000.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
	CString strSql;
	//strSql.Format(_T("select * from Building where Main_BuildingName = '%s'"),m_strMainBuildingName); order by Main_BuildingName
	strSql.Format(_T("Select Product_ID ,Online_Status,Product_name from ALL_NODE Where Bautrate = '%s' and (Product_class_ID = 9 OR Product_class_ID = 7)"),m_Minipanel_IP);
	m_q = m_SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
	TSTatInfoNode tmpnode;
	while (!m_q.eof())
	{
		strSql = m_q.getValuebyName(L"Product_ID");
		tmpnode.TstatID = strSql;
		tmpnode.OnlineStatus = m_q.getIntField("Online_Status");
		strSql = m_q.getValuebyName(L"Product_name");
		tmpnode.TstatName = strSql;
		m_vecTstatID.push_back(tmpnode);
		m_q.nextRow();
	}
	//sort(m_vecTstatID.begin(), m_vecTstatID.end());
	Initial_datalist();

	return TRUE;   
}


void CGroupScheduleDlg::OnBnClickedButtonSave()
{
	 
}
