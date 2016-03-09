// BacnetAnalogCusRang.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetAnalogCusRang.h"
#include "afxdialogex.h"
#include "globle_function.h"



int max_unit = 0xffffffff;
int min_unit = 0x7fffffff;

// CBacnetAnalogCusRang dialog
HWND temp_gloab_hwnd =NULL;
IMPLEMENT_DYNAMIC(CBacnetAnalogCusRang, CDialogEx)

CBacnetAnalogCusRang::CBacnetAnalogCusRang(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetAnalogCusRang::IDD, pParent)
{

}

CBacnetAnalogCusRang::~CBacnetAnalogCusRang()
{
}

void CBacnetAnalogCusRang::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_DIALOG_BACNET_RANGE_LIST, m_analog_cus_range_list);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_1, m_cus_analog_range_ctrl_0);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_2, m_cus_analog_range_ctrl_1);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_3, m_cus_analog_range_ctrl_2);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_4, m_cus_analog_range_ctrl_3);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_5, m_cus_analog_range_ctrl_4);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_6, m_cus_analog_range_ctrl_5);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_7, m_cus_analog_range_ctrl_6);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_8, m_cus_analog_range_ctrl_7);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_9, m_cus_analog_range_ctrl_8);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_10, m_cus_analog_range_ctrl_9);
	DDX_Control(pDX, IDC_SLIDER_BAC_DEF_11, m_cus_analog_range_ctrl_10);
}


BEGIN_MESSAGE_MAP(CBacnetAnalogCusRang, CDialogEx)
	 ON_WM_VSCROLL()
	ON_MESSAGE(MY_RESUME_DATA, AnalogRangeTblMessageCallBack)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_AnalogCusRange_Item)
	ON_MESSAGE(WM_REFRESH_BAC_ANALOGCUSRANGE_LIST,Fresh_AnalogCusRange_List)
	ON_WM_CLOSE()
	  ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
END_MESSAGE_MAP()


// CBacnetAnalogCusRang message handlers
LRESULT  CBacnetAnalogCusRang::AnalogRangeTblMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if(msg_result)
	{
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}
	else
	{
		memcpy_s(&m_Program_data.at(pInvoke->mRow),sizeof(Str_program_point),&m_temp_program_data[pInvoke->mRow],sizeof(Str_program_point));//还原没有改对的值
		PostMessage(WM_REFRESH_BAC_PROGRAM_LIST,pInvoke->mRow,REFRESH_ON_ITEM);
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
	}
	if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
		m_analog_cus_range_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
	else
		m_analog_cus_range_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);
	m_analog_cus_range_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);

	Fresh_AnalogCusRange_List(0,0);

	if(pInvoke)
		delete pInvoke;
	return 0;
}

BOOL CBacnetAnalogCusRang::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_tooltips.Create(GetDlgItem(IDC_SLIDER_BAC_DEF_1));

	m_tooltips.Activate(TRUE);

	// TODO:  Add extra initialization here
	Initial_List();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CBacnetAnalogCusRang::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	Fresh_AnalogCusRange_Item(0,0);
	g_hwnd_now =	temp_gloab_hwnd;
	analog_cus_range_dlg = NULL;
	CDialogEx::OnCancel();
}


void CBacnetAnalogCusRang::Initial_List()
{
	m_analog_cus_range_list.ShowWindow(SW_HIDE);
	m_analog_cus_range_list.DeleteAllItems();
	while ( m_analog_cus_range_list.DeleteColumn (0)) ;

	m_analog_cus_range_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_analog_cus_range_list.SetExtendedStyle(m_analog_cus_range_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_analog_cus_range_list.SetExtendedStyle(m_analog_cus_range_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_analog_cus_range_list.InsertColumn(0, _T("Voltage"), 50, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	m_analog_cus_range_list.InsertColumn(1, _T("Value"), 100, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByString);

	temp_gloab_hwnd = g_hwnd_now;
	g_hwnd_now = this->m_hWnd;
	analog_cus_range_dlg = this->m_hWnd;

	CRect list_rect,win_rect;
	m_analog_cus_range_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_input_dlg_hwnd,win_rect);
	m_analog_cus_range_list.Set_My_WindowRect(win_rect);
	m_analog_cus_range_list.Set_My_ListRect(list_rect);


	//m_analog_cus_range_list.DeleteAllItems();
	for (int i=0;i<11;i++)
	{
		CString temp_item,temp_value,temp_cal,temp_filter,temp_status,temp_lable;
		CString temp_des;
		CString temp_units;

		if(i>=input_item_limit_count)	//vector的大小始终不变 ,用次变量来 约束 要显示的 item 数量;
			break;

		temp_item.Format(_T("%.1f"),i*0.5);
		m_analog_cus_range_list.InsertItem(i,temp_item);



		for (int x=0;x<2;x++)
		{
			if((i%2)==0)
				m_analog_cus_range_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_analog_cus_range_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}
	}
	m_analog_cus_range_list.InitListData();
	m_analog_cus_range_list.ShowWindow(SW_SHOW);
}



LRESULT CBacnetAnalogCusRang::Fresh_AnalogCusRange_Item(WPARAM wParam,LPARAM lParam)
{
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	CString cs_info;

	max_unit = 0xffffffff;
	min_unit = 0x7fffffff;

	for (int i=0;i<11;i++)//只要有修改1个，就重新将界面上所有的值 读到 内存 并写入 设备;
	{
			CString cs_temp = m_analog_cus_range_list.GetItemText(i,ANALOG_CUS_RANGE_RBL_VALUE);
			float temp_value =_wtof(cs_temp);
			if((temp_value >6553.5) || (temp_value <0))
			{
				m_analog_cus_range_list.SetItemText(Changed_Item,0,_T(""));
				SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Value Out of range"));
				continue;
			}
			m_analog_custmer_range.at(analog_range_tbl_line).dat[i].value = (unsigned short)(temp_value * 10);

			cs_temp = m_analog_cus_range_list.GetItemText(i,ANALOG_CUS_RANGE_RBL_UNIT);
			int temp_value1 =_wtoi(cs_temp);
			m_analog_custmer_range.at(analog_range_tbl_line).dat[i].unit = temp_value1;
	}



	CString temp_task_info;
	temp_task_info.Format(_T("Update Analog Range  "));
	Post_Write_Message(g_bac_instance,WRITEANALOG_CUS_TABLE_T3000,analog_range_tbl_line,analog_range_tbl_line,sizeof(Str_table_point),analog_cus_range_dlg ,temp_task_info,Changed_Item,Changed_SubItem);


	return 0;
}

LRESULT CBacnetAnalogCusRang::Fresh_AnalogCusRange_List(WPARAM wParam,LPARAM lParam)
{
	// Str_in_point Get_Str_in_Point(int index);
	max_unit = 0xffffffff;
    min_unit = 0x7fffffff;

	for (int i=0;i<11;i++)
	{
		CString n_value_2byte;
		CString n_unite_4byte;
		n_value_2byte.Format(_T("%.1f"),((float)m_analog_custmer_range.at(analog_range_tbl_line).dat[i].value)/10);
		n_unite_4byte.Format(_T("%d"),m_analog_custmer_range.at(analog_range_tbl_line).dat[i].unit);
		m_analog_cus_range_list.SetItemText(i,0,n_value_2byte);
		m_analog_cus_range_list.SetItemText(i,1,n_unite_4byte);

		if(m_analog_custmer_range.at(analog_range_tbl_line).dat[i].unit > max_unit)
			max_unit = m_analog_custmer_range.at(analog_range_tbl_line).dat[i].unit;

		if(m_analog_custmer_range.at(analog_range_tbl_line).dat[i].unit < min_unit)
			min_unit =  m_analog_custmer_range.at(analog_range_tbl_line).dat[i].unit;
	}
	SetSlideRange();

	return 0;
}

void CBacnetAnalogCusRang::SetSlideRange()
{
	if(max_unit == min_unit)
	{
		max_unit = min_unit + 1;
	}
	  m_cus_analog_range_ctrl_0.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_1.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_2.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_3.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_4.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_5.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_6.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_7.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_8.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_9.SetRange(min_unit,max_unit);	
	  m_cus_analog_range_ctrl_10.SetRange(min_unit,max_unit);	

	  m_cus_analog_range_ctrl_0.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[0].unit-min_unit));
	  m_cus_analog_range_ctrl_1.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[1].unit-min_unit));
	  m_cus_analog_range_ctrl_2.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[2].unit-min_unit));
	  m_cus_analog_range_ctrl_3.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[3].unit-min_unit));
	  m_cus_analog_range_ctrl_4.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[4].unit-min_unit));
	  m_cus_analog_range_ctrl_5.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[5].unit-min_unit));
	  m_cus_analog_range_ctrl_6.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[6].unit-min_unit));
	  m_cus_analog_range_ctrl_7.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[7].unit-min_unit));
	  m_cus_analog_range_ctrl_8.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[8].unit-min_unit));
	  m_cus_analog_range_ctrl_9.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[9].unit-min_unit));
	  m_cus_analog_range_ctrl_10.SetPos(max_unit - (m_analog_custmer_range.at(analog_range_tbl_line).dat[10].unit-min_unit));
	  UpdateData();
}


void CBacnetAnalogCusRang::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	g_hwnd_now =	temp_gloab_hwnd;
	analog_cus_range_dlg = NULL;
	CDialogEx::OnClose();
}


BOOL CBacnetAnalogCusRang::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if((pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN)  || (pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}



void CBacnetAnalogCusRang::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{


	int CurPos;
	int temp_value;
	//float fvalue; 

	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_0.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_0.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(0,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_1));
		m_tooltips.Update();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_1.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_1.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(1,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_2));
		m_tooltips.Update();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_2.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_2.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(2,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_3));
		m_tooltips.Update();
	}	
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_3.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_3.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(3,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_4));
		m_tooltips.Update();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_4.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_4.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(4,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_5));
		m_tooltips.Update();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_5.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_5.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(5,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_6));
		m_tooltips.Update();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_6.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_6.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(6,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_7));
		m_tooltips.Update();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_7.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_7.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(7,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_8));
		m_tooltips.Update();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_8.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_8.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(8,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_9));
		m_tooltips.Update();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_9.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_9.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(9,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_10));
		m_tooltips.Update();
	}
	if(pScrollBar->m_hWnd==m_cus_analog_range_ctrl_10.m_hWnd)
	{
		CurPos= m_cus_analog_range_ctrl_10.GetPos();
		temp_value=max_unit + min_unit - CurPos;
		CString strtext;
		strtext.Format(_T("%d"),temp_value);
		m_tipvalue = temp_value;
		m_analog_cus_range_list.SetItemText(10,1,strtext);
		m_tooltips.SetParent(GetDlgItem(IDC_SLIDER_BAC_DEF_11));
		m_tooltips.Update();
	}




	//}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CBacnetAnalogCusRang::OnToolTipNotify(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
	UINT nID =pNMHDR->idFrom;
	if (pTTT->uFlags & TTF_IDISHWND)
	{
		// idFrom is actually the HWND of the tool
		nID = ::GetDlgCtrlID((HWND)nID);
		if(nID)
		{
			//  这里就是你要显示的Tooltips，可以根据需要来定制
			CString strToolTips;
			strToolTips.Format(_T("%d"), m_tipvalue);
			pTTT->lpszText=strToolTips.AllocSysString();

			pTTT->hinst = NULL;
			return(TRUE);
		}
	}

	return(FALSE);
}
