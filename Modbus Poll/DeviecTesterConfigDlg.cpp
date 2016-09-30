// DeviecTesterConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "DeviecTesterConfigDlg.h"
#include "afxdialogex.h"
#include "AddDeviceDlg.h"


// CDeviecTesterConfigDlg dialog

IMPLEMENT_DYNAMIC(CDeviecTesterConfigDlg, CDialogEx)

CDeviecTesterConfigDlg::CDeviecTesterConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviecTesterConfigDlg::IDD, pParent)
{

}

CDeviecTesterConfigDlg::~CDeviecTesterConfigDlg()
{
}

void CDeviecTesterConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_devices_list);
	DDX_Control(pDX, IDC_LIST_LOG, m_list_log);
}


BEGIN_MESSAGE_MAP(CDeviecTesterConfigDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDeviecTesterConfigDlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DEVICE, &CDeviecTesterConfigDlg::OnNMDblclkListDevice)
END_MESSAGE_MAP()


// CDeviecTesterConfigDlg message handlers


BOOL CDeviecTesterConfigDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_devices_list.InsertColumn(0,L"Item",LVCFMT_CENTER,100);
	m_devices_list.InsertColumn(1,L"Modbus ID",LVCFMT_CENTER,100);
	m_devices_list.InsertColumn(2,L"From",LVCFMT_CENTER,100);
	m_devices_list.InsertColumn(3,L"Length",LVCFMT_CENTER,100);
	m_devices_list.InsertColumn(4,L"Count",LVCFMT_CENTER,100);
	m_devices_list.InsertColumn(5,L"Repeat",LVCFMT_CENTER,100);
	m_devices_list.SetExtendedStyle(LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);



	m_list_log.InsertColumn(0,L"Modbus ID",LVCFMT_CENTER,100);
	m_list_log.InsertColumn(1,L"From",LVCFMT_CENTER,100);
	m_list_log.InsertColumn(2,L"Length",LVCFMT_CENTER,100);
	m_list_log.InsertColumn(3,L"Times",LVCFMT_CENTER,100);
	m_list_log.InsertColumn(4,L"Result",LVCFMT_CENTER,100);
	m_list_log.SetExtendedStyle(LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);








	return TRUE; 
	 // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDeviecTesterConfigDlg::OnBnClickedOk()
{
	 
	//CDialogEx::OnOK();
	device_node temp_node;
	for (int item = 0 ;item<m_devices_list.GetItemCount();item++)
	{
		temp_node.modbus_id = _wtoi(m_devices_list.GetItemText(item,1));
		temp_node.from= _wtoi(m_devices_list.GetItemText(item,2));
		temp_node.length = _wtoi(m_devices_list.GetItemText(item,3));
		temp_node.count=_wtoi(m_devices_list.GetItemText(item,4));
		temp_node.repeat = _wtoi(m_devices_list.GetItemText(item,5));
		m_vec_device.push_back(temp_node);
	}


	
}


void CDeviecTesterConfigDlg::OnNMDblclkListDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	long lRow,lCol;
	 
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_devices_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_devices_list.SubItemHitTest(&lvinfo);
	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;
	 
	if (-1==lRow)
	{
		CAddDeviceDlg dlg;
		 
		if (IDOK==dlg.DoModal())
		{
			int Item = m_devices_list.GetItemCount();
			CString Str;
			Str.Format(_T("%d"),Item);
			m_devices_list.InsertItem(Item,Str);
			Str.Format(_T("%d"),dlg.m_modbus_id);
			m_devices_list.SetItemText(Item,1,Str);
			Str.Format(_T("%d"),dlg.m_from);
			m_devices_list.SetItemText(Item,2,Str);
			Str.Format(_T("%d"),dlg.m_length);
			m_devices_list.SetItemText(Item,3,Str);
			Str.Format(_T("%d"),dlg.m_count);
			m_devices_list.SetItemText(Item,4,Str);
			Str.Format(_T("%d"),dlg.m_repeat);
			m_devices_list.SetItemText(Item,5,Str);


		}

	}
	else
	{
		CAddDeviceDlg dlg;
		dlg.m_modbus_id = _wtoi(m_devices_list.GetItemText(lRow,1));
		dlg.m_from = _wtoi(m_devices_list.GetItemText(lRow,2));
		dlg.m_length = _wtoi(m_devices_list.GetItemText(lRow,3));
		dlg.m_count=_wtoi(m_devices_list.GetItemText(lRow,4));
		dlg.m_repeat = _wtoi(m_devices_list.GetItemText(lRow,5));

		if (IDOK==dlg.DoModal())
		{
			int Item = m_devices_list.GetItemCount();
			CString Str;
			Str.Format(_T("%d"),Item);
			m_devices_list.InsertItem(Item,Str);
			Str.Format(_T("%d"),dlg.m_modbus_id);
			m_devices_list.SetItemText(Item,1,Str);
			Str.Format(_T("%d"),dlg.m_from);
			m_devices_list.SetItemText(Item,2,Str);
			Str.Format(_T("%d"),dlg.m_length);
			m_devices_list.SetItemText(Item,3,Str);
			Str.Format(_T("%d"),dlg.m_count);
			m_devices_list.SetItemText(Item,4,Str);
			Str.Format(_T("%d"),dlg.m_repeat);
			m_devices_list.SetItemText(Item,5,Str);


		}
	}




	*pResult = 0;
}
