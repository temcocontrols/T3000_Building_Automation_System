// BacnetIOConfig.cpp : implementation file
// Programming by Fance  2017 02 

#include "stdafx.h"
#include "T3000.h"
#include "BacnetIOConfig.h"
#include "afxdialogex.h"
#include "global_function.h"

// CBacnetIOConfig dialog

IMPLEMENT_DYNAMIC(CBacnetIOConfig, CDialogEx)

CBacnetIOConfig::CBacnetIOConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetIOConfig::IDD, pParent)
{
}

CBacnetIOConfig::~CBacnetIOConfig()
{
}

void CBacnetIOConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_IOCONFIG, m_ext_io_config_list);
}


BEGIN_MESSAGE_MAP(CBacnetIOConfig, CDialogEx)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Extio_Item)
	ON_MESSAGE(WM_REFRESH_BAC_EXTIO_LIST,Fresh_Extio_List)
	ON_BN_CLICKED(IDC_BUTTON_EXTIO_ADD, &CBacnetIOConfig::OnBnClickedButtonExtioAdd)
	ON_BN_CLICKED(IDC_BUTTON_EXTIO_DELETE_SEL, &CBacnetIOConfig::OnBnClickedButtonExtioDeleteSel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_IOCONFIG, &CBacnetIOConfig::OnNMClickListIoconfig)
	ON_BN_CLICKED(IDC_BUTTON_EXT_OK, &CBacnetIOConfig::OnBnClickedButtonExtOk)
	ON_BN_CLICKED(IDC_BUTTON_EXT_CANCEL, &CBacnetIOConfig::OnBnClickedButtonExtCancel)
END_MESSAGE_MAP()


// CBacnetIOConfig message handlers


BOOL CBacnetIOConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	Initial_List();
	return TRUE; // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CBacnetIOConfig::PreTranslateMessage(MSG* pMsg)
{
	
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CRect list_rect, win_rect;
		m_ext_io_config_list.GetWindowRect(list_rect);
		ScreenToClient(&list_rect);
		::GetWindowRect(m_ext_io_dlg_hwmd, win_rect);
		m_ext_io_config_list.Set_My_WindowRect(win_rect);
		m_ext_io_config_list.Set_My_ListRect(list_rect);

		m_ext_io_config_list.Get_clicked_mouse_position();
		return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CBacnetIOConfig::ExtproductIDisValiable(unsigned char temp_id)
{
	if ((temp_id != PM_T322AI) &&
		(temp_id != PM_T38AI8AO6DO) &&
        (temp_id != PM_T3PT12) &&
		(temp_id != PM_MINIPANEL)&&
		(temp_id != PM_MINIPANEL_ARM)
		)
	{
		return false;
	}
	else
	{
		return TRUE;
	}
}

void CBacnetIOConfig::Initial_List()
{
	m_ext_io_config_list.ShowWindow(SW_HIDE);
	m_ext_io_config_list.DeleteAllItems();
	while (m_ext_io_config_list.DeleteColumn(0));

	m_ext_io_config_list.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_ext_io_config_list.SetExtendedStyle(m_ext_io_config_list.GetExtendedStyle() | LVS_EX_GRIDLINES & (~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_ext_io_config_list.InsertColumn(EXTIO_NUM, _T("Num"), 40, ListCtrlEx::CheckBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_ext_io_config_list.InsertColumn(EXTIO_HARDWARE, _T("Hardware"), 100, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_ext_io_config_list.InsertColumn(EXTIO_PORT, _T("Port"), 100, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_ext_io_config_list.InsertColumn(EXTIO_ID, _T("ID"), 60, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_ext_io_config_list.InsertColumn(EXTIO_LAST_CONTACT, _T("Last Contact"), 120, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_ext_io_config_list.InsertColumn(EXTIO_INPUTS_IO, _T("Inputs"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_ext_io_config_list.InsertColumn(EXTIO_OUTPUTS_IO, _T("Outputs"), 80, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByString);
	m_ext_io_config_list.SetWhetherShowBkCol(false);


	m_ext_io_dlg_hwmd = this->m_hWnd;
	m_ext_io_config_list.SetListHwnd(this->m_hWnd);
	for (int i = 0; i < (int)m_extio_config_data.size(); i++)
	{
		CString temp_item, temp_value, temp_cal, temp_filter, temp_status, temp_lable;
		CString temp_des;
		CString temp_units;

		if (ExtproductIDisValiable(m_extio_config_data.at(i).reg.product_id) == false)
		{
			break;
		}

		//temp_item.Format(_T("%d"),i+1);
		temp_item.Format(_T(" "), i + 1);
		m_ext_io_config_list.InsertItem(i, temp_item);


		if (ListCtrlEx::ComboBox == m_ext_io_config_list.GetColumnType(EXTIO_HARDWARE))
		{
			ListCtrlEx::CStrList strlist;
			for (int aaa = 0; aaa < (int)sizeof(ExtIO_Product) / sizeof(ExtIO_Product[0]); aaa++)
			{
				strlist.push_back(ExtIO_Product[aaa]);
			}
			m_ext_io_config_list.SetCellStringList(i, EXTIO_HARDWARE, strlist);
		}

		if (ListCtrlEx::ComboBox == m_ext_io_config_list.GetColumnType(EXTIO_PORT))
		{
			ListCtrlEx::CStrList strlist;
			for (int m = 0; m < (int)(sizeof(ExtIO_Port) / sizeof(ExtIO_Port[0])); m++)
			{
				strlist.push_back(ExtIO_Port[m]);
			}
			m_ext_io_config_list.SetCellStringList(i, EXTIO_PORT, strlist);
		}

		for (int x = 0; x < EXTIO_COL_NUMBER; x++)
		{
			if ((i % 2) == 0)
				m_ext_io_config_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_ext_io_config_list.SetItemBkColor(i, x, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
		}
	}

    if (m_ext_io_config_list.GetItemCount() == 0)
    {
        m_ext_io_config_list.ShowWindow(SW_SHOW);
        return;
    }

	m_ext_io_config_list.SetItemText(0, EXTIO_PORT,_T("N/A"));
	m_ext_io_config_list.SetItemText(0, EXTIO_ID,_T("N/A"));
	m_ext_io_config_list.SetItemText(0, EXTIO_LAST_CONTACT,_T("N/A"));
	m_ext_io_config_list.SetCellEnabled(0, EXTIO_NUM, 0);
	m_ext_io_config_list.SetCellEnabled(0, EXTIO_HARDWARE, 0);
	m_ext_io_config_list.SetCellEnabled(0, EXTIO_PORT, 0);
	m_ext_io_config_list.SetCellEnabled(0, EXTIO_ID, 0);

	m_ext_io_config_list.InitListData();
	m_ext_io_config_list.ShowWindow(SW_SHOW);
	PostMessage(WM_REFRESH_BAC_EXTIO_LIST,NULL,NULL);
}
extern void Getminitypename(unsigned char nmini_type, CString &ret_name);

LRESULT CBacnetIOConfig::Fresh_Extio_List(WPARAM wParam, LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);
	int Fresh_Item;
	int isFreshOne = (int)lParam;


	for (int i = 0; i < (int)m_extio_config_data.size(); i++)
	{
		CString temp_item;
		temp_item.Format(_T("%d"), i + 1);
		CString temp_port;
		CString temp_product_name;
		CString temp_modbus_id;
		CString temp_input_start;
		CString temp_input_end;
		CString temp_output_start;
		CString temp_output_end;
		CString temp_last_contact;

		if (ExtproductIDisValiable(m_extio_config_data.at(i).reg.product_id) == false)
		{
			return 0;
		}

		if ((m_extio_config_data.at(i).reg.port != 0) &&
			(m_extio_config_data.at(i).reg.port != 1) &&
			(m_extio_config_data.at(i).reg.port != 2))
		{
			//
			continue;
		}
		if (i == 0)
		{
			temp_port = _T("N/A");
			temp_modbus_id = _T("N/A");
			temp_last_contact = _T("N/A");
		}
		else
		{
			temp_port = ExtIO_Port[m_extio_config_data.at(i).reg.port];
			temp_modbus_id.Format(_T("%u"), m_extio_config_data.at(i).reg.modbus_id);
			temp_last_contact = _T("");
		}
        if ((i == 0) && Bacnet_Private_Device(m_extio_config_data.at(i).reg.product_id))
        {

            Getminitypename(Device_Basic_Setting.reg.mini_type, temp_product_name);

        }
        else
		    temp_product_name = GetProductName(m_extio_config_data.at(i).reg.product_id);

		temp_input_start.Format(_T("%u"), m_extio_config_data.at(i).reg.input_start);
		temp_input_end.Format(_T("%u"), m_extio_config_data.at(i).reg.input_end);
		temp_output_start.Format(_T("%u"), m_extio_config_data.at(i).reg.output_start);
		temp_output_end.Format(_T("%u"), m_extio_config_data.at(i).reg.output_end);

		CTime time_last_contact;
		time_last_contact = m_extio_config_data.at(i).reg.last_contact_time;
		CString temp_cs_time;
		unsigned long m_time_monitor_now = time(NULL);
		if ((m_extio_config_data.at(i).reg.last_contact_time < m_time_monitor_now - 604800) ||
			(m_extio_config_data.at(i).reg.last_contact_time > m_time_monitor_now))
			temp_cs_time = _T("N/A");
		else
			temp_cs_time = time_last_contact.Format(_T("%y/%m/%d %H:%M:%S"));
		m_ext_io_config_list.SetItemText(i, EXTIO_LAST_CONTACT, temp_cs_time);

		CString temp_in;
		CString temp_out;
		temp_in.Format(_T("%s-%s"), temp_input_start, temp_input_end);

        temp_out.Format(_T("%s-%s"), temp_output_start, temp_output_end);
        for (int j = 0; j < sizeof(ExtIO_ProductId) / sizeof(ExtIO_ProductId[0]); j++)
        {
            if (m_extio_config_data.at(i).reg.product_id == ExtIO_ProductId[j])
            {
                if (ExtIO_OUTPUT_COUNT[j] == 0)
                {
                    temp_out.Format(_T("N/A"));
                }
                break;
            }
        }



		m_ext_io_config_list.SetItemText(i, EXTIO_NUM, temp_item);
		m_ext_io_config_list.SetItemText(i, EXTIO_HARDWARE, temp_product_name);
		m_ext_io_config_list.SetItemText(i, EXTIO_PORT, temp_port);
		m_ext_io_config_list.SetItemText(i, EXTIO_ID, temp_modbus_id);
		m_ext_io_config_list.SetItemText(i, EXTIO_INPUTS_IO, temp_in);
		m_ext_io_config_list.SetItemText(i, EXTIO_OUTPUTS_IO, temp_out);
	}
	return 0;
}

void CBacnetIOConfig::CaculateIOCount(int nitem)
{
	int temp_list_count = m_ext_io_config_list.GetItemCount();

	for (int i = nitem; i < temp_list_count; i++)
	{
		//CString temp_pidtype =  m_ext_io_config_list.GetItemText(i,EXTIO_HARDWARE);
		int pid_sequence = 0;

        //遍历ext产品，判断选中的是哪一个扩展设备;
        for (int j = 0; j < sizeof(ExtIO_ProductId)/sizeof(ExtIO_ProductId[0]); j++)
        {
            if (m_extio_config_data.at(i).reg.product_id == ExtIO_ProductId[j])
            {
                pid_sequence = j;
                break;
            }
        }

		m_ext_io_config_list.SetItemText(i, EXTIO_HARDWARE, ExtIO_Product[pid_sequence]);

		m_extio_config_data.at(i).reg.product_id = ExtIO_ProductId[pid_sequence];
		m_extio_config_data.at(i).reg.input_start = m_extio_config_data.at(i - 1).reg.input_end + 1;
		m_extio_config_data.at(i).reg.input_end = m_extio_config_data.at(i - 1).reg.input_end + ExtIO_INPUT_COUNT[pid_sequence];;
		CString temp_input;
		temp_input.Format(_T("%u-%u"), m_extio_config_data.at(i).reg.input_start, m_extio_config_data.at(i).reg.input_end);
		m_ext_io_config_list.SetItemText(i, EXTIO_INPUTS_IO, temp_input);


		CString temp_output;
		if (ExtIO_OUTPUT_COUNT[pid_sequence] == 0) // 没有输出output的情况
		{
			m_extio_config_data.at(i).reg.output_end = m_extio_config_data.at(i - 1).reg.output_end;
			temp_output.Format(_T("N/A"));
		}
		else
		{
			m_extio_config_data.at(i).reg.output_start = m_extio_config_data.at(i - 1).reg.output_end + 1;
			m_extio_config_data.at(i).reg.output_end = m_extio_config_data.at(i - 1).reg.output_end + ExtIO_OUTPUT_COUNT[pid_sequence];
			temp_output.Format(_T("%u-%u"), m_extio_config_data.at(i).reg.output_start, m_extio_config_data.at(i).reg.output_end);
		}


		m_ext_io_config_list.SetItemText(i, EXTIO_OUTPUTS_IO, temp_output);
	}

	return;
}


LRESULT CBacnetIOConfig::Fresh_Extio_Item(WPARAM wParam, LPARAM lParam)
{
	int cmp_ret;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	if (Changed_Item == 0)
	{
		return 0;
	}

	if (Changed_SubItem == EXTIO_HARDWARE)
	{
		CString temp_pidtype = m_ext_io_config_list.GetItemText(Changed_Item, Changed_SubItem);
		for (int i = 0; i < sizeof(ExtIO_Product) / sizeof(ExtIO_Product[0]); i++)
		{
			if (temp_pidtype.CompareNoCase(ExtIO_Product[i]) == 0)
			{
				m_extio_config_data.at(Changed_Item).reg.product_id = ExtIO_ProductId[i];
				break;
			}
		}

		CaculateIOCount(Changed_Item);
	}
	else if (Changed_SubItem == EXTIO_ID)
	{
		CString temp_modbusid = m_ext_io_config_list.GetItemText(Changed_Item, Changed_SubItem);
		int int_modbus_id = _wtoi(temp_modbusid);
		if ((int_modbus_id <= 0) || (int_modbus_id >= 255))
		{
			return 0;
		}

		m_extio_config_data.at(Changed_Item).reg.modbus_id = (unsigned char)int_modbus_id;
	}
	else if (Changed_SubItem == EXTIO_PORT)
	{
		CString temp_porttype = m_ext_io_config_list.GetItemText(Changed_Item, Changed_SubItem);
		for (int i = 0; i < sizeof(ExtIO_Port) / sizeof(ExtIO_Port[0]); i++)
		{
			if (temp_porttype.CompareNoCase(ExtIO_Port[i]) == 0)
			{
				m_extio_config_data.at(Changed_Item).reg.port = i;
				break;
			}
		}
	}


	return 0;
}


void CBacnetIOConfig::OnBnClickedButtonExtioAdd()
{
	
	int ext_io_size = m_ext_io_config_list.GetItemCount();
	if (ext_io_size >= 12)
	{
		MessageBox(_T("Can't add more expansion I/O."));
		return;
	}

	CString temp_item;
	temp_item.Format(_T("%d"), ext_io_size + 1);
	m_ext_io_config_list.InsertItem(ext_io_size, temp_item);


	if (ListCtrlEx::ComboBox == m_ext_io_config_list.GetColumnType(EXTIO_HARDWARE))
	{
		ListCtrlEx::CStrList strlist;
		for (int aaa = 0; aaa < (int)sizeof(ExtIO_Product) / sizeof(ExtIO_Product[0]); aaa++)
		{
			strlist.push_back(ExtIO_Product[aaa]);
		}
		m_ext_io_config_list.SetCellStringList(ext_io_size, EXTIO_HARDWARE, strlist);
	}

	if (ListCtrlEx::ComboBox == m_ext_io_config_list.GetColumnType(EXTIO_PORT))
	{
		ListCtrlEx::CStrList strlist;
		for (int m = 0; m < (int)(sizeof(ExtIO_Port) / sizeof(ExtIO_Port[0])); m++)
		{
			strlist.push_back(ExtIO_Port[m]);
		}
		m_ext_io_config_list.SetCellStringList(ext_io_size, EXTIO_PORT, strlist);
	}

	m_ext_io_config_list.SetItemText(ext_io_size, EXTIO_HARDWARE, ExtIO_Product[0]);
	m_extio_config_data.at(ext_io_size).reg.product_id = ExtIO_ProductId[0];
	m_extio_config_data.at(ext_io_size).reg.input_start = m_extio_config_data.at(ext_io_size - 1).reg.input_end + 1;
	m_extio_config_data.at(ext_io_size).reg.input_end = m_extio_config_data.at(ext_io_size - 1).reg.input_end + ExtIO_INPUT_COUNT[0];;
	CString temp_input;
	temp_input.Format(_T("%u-%u"), m_extio_config_data.at(ext_io_size).reg.input_start, m_extio_config_data.at(ext_io_size).reg.input_end);
	m_ext_io_config_list.SetItemText(ext_io_size, EXTIO_INPUTS_IO, temp_input);

	m_extio_config_data.at(ext_io_size).reg.output_start = m_extio_config_data.at(ext_io_size - 1).reg.output_end + 1;
	m_extio_config_data.at(ext_io_size).reg.output_end = m_extio_config_data.at(ext_io_size - 1).reg.output_end + ExtIO_OUTPUT_COUNT[0];
	CString temp_output;
	temp_output.Format(_T("%u-%u"), m_extio_config_data.at(ext_io_size).reg.output_start, m_extio_config_data.at(ext_io_size).reg.output_end);
	m_ext_io_config_list.SetItemText(ext_io_size, EXTIO_OUTPUTS_IO, temp_output);

	m_extio_config_data.at(ext_io_size).reg.port = 2;
	m_ext_io_config_list.SetItemText(ext_io_size, EXTIO_PORT, ExtIO_Port[2]);

	for (int x = 0; x < EXTIO_COL_NUMBER; x++)
	{
		if ((ext_io_size % 2) == 0)
			m_ext_io_config_list.SetItemBkColor(ext_io_size, x, LIST_ITEM_DEFAULT_BKCOLOR);
		else
			m_ext_io_config_list.SetItemBkColor(ext_io_size, x, LIST_ITEM_DEFAULT_BKCOLOR_GRAY);
	}
}


void CBacnetIOConfig::OnBnClickedButtonExtioDeleteSel()
{
	
	int size_of_list = m_ext_io_config_list.GetItemCount();
	for (int i = 1; i < size_of_list; i++)
	{
		BOOL check_ret = false;
		check_ret = m_ext_io_config_list.GetCellChecked(i, EXTIO_NUM);
		if (check_ret)
		{
			Str_Extio_point temp_data;
			memset(&m_extio_config_data.at(i), 0, sizeof(Str_Extio_point));
			for (int z = i + 1; z < size_of_list; z++)
			{
				memcpy(&m_extio_config_data.at(z - 1), &m_extio_config_data.at(z), sizeof(Str_Extio_point));
			}
			memset(&m_extio_config_data.at(size_of_list - 1), 0, sizeof(Str_Extio_point));
			CaculateIOCount(i);
			m_ext_io_config_list.DeleteItem(size_of_list - 1);
			PostMessage(WM_REFRESH_BAC_EXTIO_LIST,NULL,NULL);
			break;
		}
	}
	size_of_list = m_ext_io_config_list.GetItemCount();

	for (int i = size_of_list; i < BAC_EXTIO_COUNT; i++)
	{
		memset(&m_extio_config_data.at(i), 0, sizeof(Str_Extio_point));
	}
}


void CBacnetIOConfig::OnNMClickListIoconfig(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_ext_io_config_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;
	int nItem = m_ext_io_config_list.SubItemHitTest(&lvinfo);
	int nsubItem = lvinfo.iSubItem;
	if (nItem == 0)
	{
		*pResult = 0;
		return;
	}
	if (nItem != -1)
	{
		if (nsubItem != EXTIO_NUM)
			m_ext_io_config_list.SetCellChecked(nItem, 0, 1);
		ext_io_list_line = nItem;
		for (int i = 0; i < m_ext_io_config_list.GetItemCount(); ++i)
		{
			if (i == nItem)
				continue;
			m_ext_io_config_list.SetCellChecked(i, 0,FALSE);
		}
	}

	*pResult = 0;
}


void CBacnetIOConfig::OnBnClickedButtonExtOk()
{
	
	if (Write_Private_Data_Blocking(WRITEEXT_IO_T3000, 0, BAC_EXTIO_COUNT - 1) <= 0)
	{
		MessageBox(_T("Write data failed!"));
		return;
	}
	MessageBox(_T("Success!"));
}


void CBacnetIOConfig::OnBnClickedButtonExtCancel()
{
	
	PostMessage(WM_CLOSE,NULL,NULL);
}
