// NewTstatSchedulesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "NewTstatSchedulesDlg.h"
#include "afxdialogex.h"
#include "globle_function.h"


// CNewTstatSchedulesDlg dialog

IMPLEMENT_DYNAMIC(CNewTstatSchedulesDlg, CDialogEx)

CNewTstatSchedulesDlg::CNewTstatSchedulesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewTstatSchedulesDlg::IDD, pParent)
{
	 
		m_curRow = 0;
		m_curCol = 0;
}

CNewTstatSchedulesDlg::~CNewTstatSchedulesDlg()
{
}

void CNewTstatSchedulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, WeeeklyList);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_start_time);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_end_time);
	DDX_Control(pDX, IDC_DATETIMEPICKER1_SCHEDUAL, m_schedual_time_picker);
}


BEGIN_MESSAGE_MAP(CNewTstatSchedulesDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CNewTstatSchedulesDlg::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CNewTstatSchedulesDlg::OnBnClickedButtonDelete)
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Input_Item)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CNewTstatSchedulesDlg::OnNMClickList1)
	ON_NOTIFY(NM_KILLFOCUS, IDC_DATETIMEPICKER1_SCHEDUAL, &CNewTstatSchedulesDlg::OnNMKillfocusDatetimepicker1Schedual)
	ON_BN_CLICKED(IDC_BUTTON_SCHEDULE_COPY_BTN, &CNewTstatSchedulesDlg::OnBnClickedButtonScheduleCopyBtn)
END_MESSAGE_MAP()


// CNewTstatSchedulesDlg message handlers





BOOL CNewTstatSchedulesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_strScheduleMode[0]=L"";
	m_strScheduleMode[1]=L"Home";
	m_strScheduleMode[2]=L"Work";
	m_strScheduleMode[3]=L"Sleep";
	Schedule_Node SN;
	 
	SN.Hour			=6;
	SN.Minite		=30;
	SN.Monday		=1;
	SN.Tuesday		=1;
	SN.Wednesday	=1;
	SN.Thursday		=1;
	SN.Friday		=1;
	SN.Saturday		=1;
	SN.Sunday		=1;
	m_ScheduleList.push_back(SN);
	SN.Hour			=9;
	SN.Minite		=30;
	SN.Monday		=2;
	SN.Tuesday		=2;
	SN.Wednesday	=2;
	SN.Thursday		=2;
	SN.Friday		=2;
	SN.Saturday		=1;
	SN.Sunday		=1;
	m_ScheduleList.push_back(SN);
	SN.Hour			=18;
	SN.Minite		=0;
	SN.Monday		=1;
	SN.Tuesday		=1;
	SN.Wednesday	=1;
	SN.Thursday		=1;
	SN.Friday		=1;
	SN.Saturday		=1;
	SN.Sunday		=1;
	m_ScheduleList.push_back(SN);
	SN.Hour			=20;
	SN.Minite		=0;
	SN.Monday		=3;
	SN.Tuesday		=3;
	SN.Wednesday	=3;
	SN.Thursday		=3;
	SN.Friday		=3;
	SN.Saturday		=3;
	SN.Sunday		=3;
	m_ScheduleList.push_back(SN);
	 



	m_COLScheduleMode[0] = RGB(238,44,44);
	m_COLScheduleMode[1] = RGB(153,50,204);
	m_COLScheduleMode[2] = RGB(143,188,143);
	m_COLScheduleMode[3] = RGB(238,44,44);


 
	WeeeklyList.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	WeeeklyList.SetExtendedStyle(WeeeklyList.GetExtendedStyle() |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	WeeeklyList.InsertColumn(0, _T("Item"), 40, ListCtrlEx::Normal, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	WeeeklyList.InsertColumn(1, _T("Time"), 80, ListCtrlEx::EditBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	WeeeklyList.InsertColumn(2, _T("Monday"), 80, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByDigit);
	WeeeklyList.InsertColumn(3, _T("Tuesday"), 80, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	WeeeklyList.InsertColumn(4, _T("Wednesday"), 80, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	WeeeklyList.InsertColumn(5, _T("Thursday"), 80, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	WeeeklyList.InsertColumn(6, _T("Friday"), 80, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	WeeeklyList.InsertColumn(7, _T("Saturday"), 80, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	WeeeklyList.InsertColumn(8, _T("Sunday"), 80, ListCtrlEx::ComboBox, LVCFMT_LEFT, ListCtrlEx::SortByString);
	WeeeklyList.SetBkColor(RGB(205,201,201)) ;
	
	 
	g_hwnd_now = this->m_hWnd;
	
	ListCtrlEx::CStrList strlist;
	strlist.clear();
	strlist.push_back(L"");
	strlist.push_back(L"Home");
	strlist.push_back(L"Work");
	strlist.push_back(L"Sleep");
	list<Schedule_Node>::iterator it;
	int index=0;
	for (it = m_ScheduleList.begin();it!=m_ScheduleList.end();++it)
	{
		 
		 
		CString strTime;
		int Item=index+1;
		strTime.Format(_T("%d"),Item);
		WeeeklyList.InsertItem(index,strTime);
		strTime.Format(_T("%02d:%02d"),it->Hour,it->Minite);
		WeeeklyList.SetItemBkColor(index,0,RGB(255,255,255),0);
		WeeeklyList.SetItemText(index,1,strTime);
		WeeeklyList.SetItemBkColor(index,1,RGB(255,255,255),0);
		WeeeklyList.SetWhetherShowBkCol(false);
		//设置Combox
		for (int col = 2;col<9;col++)
		{
			WeeeklyList.SetCellStringList(index, col, strlist);
			
		}
		WeeeklyList.SetItemText(index,2,m_strScheduleMode[it->Monday]);
		WeeeklyList.SetItemBkColor(index,2,m_COLScheduleMode[it->Monday],0);
		WeeeklyList.SetItemText(index,3,m_strScheduleMode[it->Tuesday]);
		WeeeklyList.SetItemBkColor(index,3,m_COLScheduleMode[it->Tuesday],0);
		WeeeklyList.SetItemText(index,4,m_strScheduleMode[it->Wednesday]);
		WeeeklyList.SetItemBkColor(index,4,m_COLScheduleMode[it->Wednesday],0);
		WeeeklyList.SetItemText(index,5,m_strScheduleMode[it->Thursday]);
		WeeeklyList.SetItemBkColor(index,5,m_COLScheduleMode[it->Thursday],0);
		WeeeklyList.SetItemText(index,6,m_strScheduleMode[it->Friday]);
		WeeeklyList.SetItemBkColor(index,6,m_COLScheduleMode[it->Friday],0);
		WeeeklyList.SetItemText(index,7,m_strScheduleMode[it->Saturday]);
		WeeeklyList.SetItemBkColor(index,7,m_COLScheduleMode[it->Saturday],0);
		WeeeklyList.SetItemText(index,8,m_strScheduleMode[it->Sunday]);
		WeeeklyList.SetItemBkColor(index,8,m_COLScheduleMode[it->Sunday],0);

		index++;
	 
	}
	
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CNewTstatSchedulesDlg::Fresh_List()
{
	ListCtrlEx::CStrList strlist;
	strlist.clear();
	strlist.push_back(L"");
	strlist.push_back(L"Home");
	strlist.push_back(L"Work");
	strlist.push_back(L"Sleep");
	 
	list<Schedule_Node>::iterator it;
	int index=0;
	for (it = m_ScheduleList.begin();it!=m_ScheduleList.end();++it)
	{


		CString strTime;
		int Item=index+1;
		strTime.Format(_T("%d"),Item);
		WeeeklyList.SetItemText(index,0,strTime);
		WeeeklyList.SetItemBkColor(index,0,RGB(255,255,255),0);
		strTime.Format(_T("%02d:%02d"),it->Hour,it->Minite);
		/*WeeeklyList.InsertItem(index++,L"");*/
		WeeeklyList.SetItemText(index,1,strTime);
		WeeeklyList.SetItemBkColor(index,1,RGB(255,255,255),0);
		WeeeklyList.SetWhetherShowBkCol(false);
		//设置Combox
		for (int col = 2;col<9;col++)
		{
			WeeeklyList.SetCellStringList(index, col, strlist);

		}
		WeeeklyList.SetItemText(index,2,m_strScheduleMode[it->Monday]);
		WeeeklyList.SetItemBkColor(index,2,m_COLScheduleMode[it->Monday],0);
		it->Col_Monday = m_COLScheduleMode[it->Monday];
		if (it->Monday == 0&&index!=0)
		{
			COLORREF PreviousScheduleColor=GetItemColor(index-1,2); 
			WeeeklyList.SetItemBkColor(index,2,PreviousScheduleColor,0);
			it->Col_Monday = PreviousScheduleColor;
		}
		  
		WeeeklyList.SetItemText(index,3,m_strScheduleMode[it->Tuesday]);
		WeeeklyList.SetItemBkColor(index,3,m_COLScheduleMode[it->Tuesday],0);
		
		it->Col_Tuesday = m_COLScheduleMode[it->Tuesday];
		if (it->Tuesday == 0&&index!=0)
		{
			COLORREF PreviousScheduleColor=GetItemColor(index-1,3); 
			WeeeklyList.SetItemBkColor(index,3,PreviousScheduleColor,0);
			it->Col_Tuesday = PreviousScheduleColor;
		}

		WeeeklyList.SetItemText(index,4,m_strScheduleMode[it->Wednesday]);
		WeeeklyList.SetItemBkColor(index,4,m_COLScheduleMode[it->Wednesday],0);

		it->Col_Wednesday = m_COLScheduleMode[it->Wednesday];
		if (it->Wednesday == 0&&index!=0)
		{
			COLORREF PreviousScheduleColor=GetItemColor(index-1,4); 
			WeeeklyList.SetItemBkColor(index,4,PreviousScheduleColor,0);
			it->Col_Wednesday = PreviousScheduleColor;
		}

		WeeeklyList.SetItemText(index,5,m_strScheduleMode[it->Thursday]);
		WeeeklyList.SetItemBkColor(index,5,m_COLScheduleMode[it->Thursday],0);

		it->Col_Thursday = m_COLScheduleMode[it->Thursday];
		if (it->Thursday == 0&&index!=0)
		{
			COLORREF PreviousScheduleColor=GetItemColor(index-1,5); 
			WeeeklyList.SetItemBkColor(index,5,PreviousScheduleColor,0);
			it->Col_Thursday = PreviousScheduleColor;
		}

		WeeeklyList.SetItemText(index,6,m_strScheduleMode[it->Friday]);
		WeeeklyList.SetItemBkColor(index,6,m_COLScheduleMode[it->Friday],0);

		it->Col_Friday = m_COLScheduleMode[it->Friday];
		if (it->Friday == 0&&index!=0)
		{
			COLORREF PreviousScheduleColor=GetItemColor(index-1,6); 
			WeeeklyList.SetItemBkColor(index,6,PreviousScheduleColor,0);
			it->Col_Friday = PreviousScheduleColor;
		}

		WeeeklyList.SetItemText(index,7,m_strScheduleMode[it->Saturday]);
		WeeeklyList.SetItemBkColor(index,7,m_COLScheduleMode[it->Saturday],0);

		it->Col_Saturday = m_COLScheduleMode[it->Saturday];
		if (it->Saturday == 0&&index!=0)
		{
			COLORREF PreviousScheduleColor=GetItemColor(index-1,7); 
			WeeeklyList.SetItemBkColor(index,7,PreviousScheduleColor,0);
			it->Col_Saturday = PreviousScheduleColor;
		}

		WeeeklyList.SetItemText(index,8,m_strScheduleMode[it->Sunday]);
		WeeeklyList.SetItemBkColor(index,8,m_COLScheduleMode[it->Sunday],0);
		it->Col_Sunday = m_COLScheduleMode[it->Sunday];
		if (it->Sunday == 0&&index!=0)
		{
			COLORREF PreviousScheduleColor=GetItemColor(index-1,8); 
			WeeeklyList.SetItemBkColor(index,8,PreviousScheduleColor,0);
			it->Col_Sunday = PreviousScheduleColor;
		}

		index++;
	}
}

int CNewTstatSchedulesDlg::getvalue(CString modelname)
{
	if (modelname.CompareNoCase(m_strScheduleMode[0])==0)
	{
		return 0;
	}
	else if (modelname.CompareNoCase(m_strScheduleMode[1])==0)
	{
		return 1;
	}
	else if (modelname.CompareNoCase(m_strScheduleMode[2])==0)
	{
		return 2;
	}
	else if (modelname.CompareNoCase(m_strScheduleMode[3])==0)
	{
		return 3;
	}
	 
	return -1;
	 
}

int CNewTstatSchedulesDlg::GetValueItem(int Row,int Col)
{
	list<Schedule_Node>::iterator it = GetNode(Row);
	int ReturnValue = -1;
	switch (Col)
	{
	case 1:
	     ReturnValue = it->Hour*60+it->Minite;
		 break;
	case 2:
		ReturnValue = it->Monday;
		break;
	case 3:
		ReturnValue = it->Tuesday;
		break;
	case 4:
		ReturnValue = it->Wednesday;
		break;
	case 5:
		ReturnValue = it->Thursday;
		break;
	case 6:
		ReturnValue = it->Friday;
		break;
	case 7:
		ReturnValue = it->Saturday;
		break;
	case 8:
		ReturnValue = it->Sunday;
		break;
	default:
		ReturnValue = -1;
	}
	return ReturnValue;
}
COLORREF CNewTstatSchedulesDlg::GetItemColor(int Row,int Col)
{
	list<Schedule_Node>::iterator it = GetNode(Row);
	COLORREF ReturnValue = -1;
	switch (Col)
	{
	case 2:
		ReturnValue = it->Col_Monday;
		break;
	case 3:
		ReturnValue = it->Col_Tuesday;
		break;
	case 4:
		ReturnValue = it->Col_Wednesday;
		break;
	case 5:
		ReturnValue = it->Col_Thursday;
		break;
	case 6:
		ReturnValue = it->Col_Friday;
		break;
	case 7:
		ReturnValue = it->Col_Saturday;
		break;
	case 8:
		ReturnValue = it->Col_Sunday;
		break;
	default:
		ReturnValue = -1;
	}
	return ReturnValue;
	
}
void CNewTstatSchedulesDlg::SetValueItem(int Row,int Col,int Val)
{
	int position=0;
	 
	for (list<Schedule_Node>::iterator index = m_ScheduleList.begin();index !=m_ScheduleList.end() ;++index)
	{
		if (position == Row)
		{

			switch (Col)
			{
			case 1:
				   index->Hour = Val/60;
				   index->Minite = Val - index->Hour*60;
				   break;
			case 2:
				   index->Monday = Val;
				break;		 
			case 3:			 
				   index->Tuesday = Val ;
				break;		 
			case 4:			 
				   index->Wednesday = Val;
				break;		 
			case 5:			 
				   index->Thursday = Val;
				break;		 
			case 6:			 
				   index->Friday = Val;
				break;		 
			case 7:			 
				   index->Saturday = Val;
				break;		 
			case 8:			 
				index->Sunday = Val;
				break;		 
			
			}
			break;
		}
		position++;

	} 
}
int CNewTstatSchedulesDlg::GetDayScheduleCount(int Col)
{
	int ScheduleCount = 0;
	for (list<Schedule_Node>::iterator index = m_ScheduleList.begin();index !=m_ScheduleList.end() ;++index)
	{
		 
		 if (Col == 2 && index->Monday!=0)
		 {	
			 ScheduleCount++; 
		 } 
		 else  if (Col == 3 && index->Tuesday!=0)
		 {
			ScheduleCount++; 
		 }
		 else  if (Col == 4 && index->Wednesday!=0)
		 {
		  ScheduleCount++; 
		 }
		 else  if (Col == 5 && index->Thursday!=0)
		 {
		  ScheduleCount++; 
		 }
		 else  if (Col == 6 && index->Friday!=0)
		 {
		  ScheduleCount++; 
		 }
		 else  if (Col == 7 && index->Saturday!=0)
		 {
		  ScheduleCount++; 
		 }
		 else  if (Col == 8 && index->Sunday!=0)
		 {
		  ScheduleCount++; 
		 }
	} 

	return ScheduleCount;
}
LRESULT CNewTstatSchedulesDlg::Fresh_Input_Item(WPARAM wParam,LPARAM lParam)
{ 
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	CString New_CString =  WeeeklyList.GetItemText(Changed_Item,Changed_SubItem);
	BOOL is_change = FALSE;
	if (Changed_SubItem>1)
	{
		int tempvalue = getvalue(New_CString); 
		if (tempvalue==0||(GetDayScheduleCount(Changed_SubItem)<=6))
		{
			if (GetValueItem(Changed_Item,Changed_SubItem)!=tempvalue)
			{
				SetValueItem(Changed_Item,Changed_SubItem,tempvalue);
				is_change = TRUE;
			}
		}
		else
		{
			AfxMessageBox(L"Can't be more than 6 !");
			is_change = TRUE;
		}

		
	}
	if (Changed_SubItem == 1)
	{
		CStringArray temparray;
		SplitCStringA(temparray,New_CString,_T(":"));
		if (temparray.GetSize()<=1)
		{
			AfxMessageBox(_T("The time that you input is not correct!"));
		   is_change = TRUE;
		}
		int Hour = _wtoi(temparray[0]);
		int minute = _wtoi(temparray[1]);
		int tempvalue = Hour*60+minute;
		
		if (GetValueItem(Changed_Item,Changed_SubItem)!=tempvalue)
		{
			SetValueItem(Changed_Item,Changed_SubItem,tempvalue);
			is_change = TRUE;
		}
		 
	}
	 
		Fresh_List();
	 

	return 0;
}

BOOL CNewTstatSchedulesDlg::Insert_Schdule(Schedule_Node SR,int POS)
{
	 int insert_time =SR.Hour*60+SR.Minite;
	BOOL is_exsit = FALSE;
	for (list<Schedule_Node>::iterator index = m_ScheduleList.begin();index !=m_ScheduleList.end() ;++index)
	{
		int ST=index->Hour*60+index->Minite;

		if (ST == insert_time)
		{
			is_exsit = TRUE;
		}
	}
	int position=0;
	if (!is_exsit)
	{
		for (list<Schedule_Node>::iterator index = m_ScheduleList.begin();index !=m_ScheduleList.end() ;++index)
		{
			if (position == POS)
			{
			    m_ScheduleList.insert(index,SR);
				return TRUE;
			}
			position++;
		} 
	}

	return FALSE;
}

BOOL CNewTstatSchedulesDlg::Delete_Schdule(int POS)
{
        int position=0;
	 
		for (list<Schedule_Node>::iterator index = m_ScheduleList.begin();index !=m_ScheduleList.end() ;++index)
		{
			if (position == POS)
			{
			   // m_ScheduleList.insert(index,SR);
				m_ScheduleList.erase(index);
				return TRUE;
			}
			position++;
		} 
	 
	return FALSE;
}
list<Schedule_Node>::iterator CNewTstatSchedulesDlg::GetNode(int POS){
	int position=0;
	list<Schedule_Node>::iterator returnnode = m_ScheduleList.end();
	for (list<Schedule_Node>::iterator index = m_ScheduleList.begin();index !=m_ScheduleList.end() ;++index)
	{
		if (position == POS)
		{
			 
			returnnode= index;
		}
		position++;

	} 
	return returnnode;
}  
void CNewTstatSchedulesDlg::OnBnClickedButtonInsert()
{
	/*if (WeeeklyList.GetRowCount()>=6)
	{
		AfxMessageBox(_T("Can't insert!"));
	}
	else
	{*/
		
		Schedule_Node temp_node ;
		list<Schedule_Node>::iterator it_node = GetNode(m_curRow);
		if (it_node != m_ScheduleList.end())
		{
			int min = it_node->Hour*60+it_node->Minite;
			min-=30;
			temp_node.Hour = min/60;
			temp_node.Minite = min - temp_node.Hour*60;
			 
			
			temp_node.Monday		=0;
			temp_node.Tuesday		=0;
			temp_node.Wednesday		=0;
			temp_node.Thursday		=0;
			temp_node.Friday		=0;
			temp_node.Saturday		=0;
			temp_node.Sunday		=0;
			if(!Insert_Schdule(temp_node,m_curRow))
			{
				CString strTemp;
				strTemp.Format(_T("Time %02d:%02d existed!"),temp_node.Hour,temp_node.Minite);
				AfxMessageBox(strTemp);
				return;
			}


		}
		else
		{
			temp_node.Hour = 6;
			temp_node.Minite = 30;


			temp_node.Monday		=0;
			temp_node.Tuesday		=0;
			temp_node.Wednesday		=0;
			temp_node.Thursday		=0;
			temp_node.Friday		=0;
			temp_node.Saturday		=0;
			temp_node.Sunday		=0;
			m_ScheduleList.push_back(temp_node);
		}
		WeeeklyList.InsertItem(m_curRow,L"");
		Fresh_List();
	/*}*/

}


void CNewTstatSchedulesDlg::OnBnClickedButtonDelete()
{
	if (WeeeklyList.GetRowCount()>0)
	{
		WeeeklyList.DeleteItem(m_curRow);
		Delete_Schdule(m_curRow);
	}
}

void CNewTstatSchedulesDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CString temp_cstring;
	long lRow,lCol;
	 
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	WeeeklyList.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=WeeeklyList.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>WeeeklyList.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;

	m_curRow = lRow;
	m_curCol = lCol;
	if (m_curCol == 0)
	{
		Fresh_List();
		WeeeklyList.SetItemBkColor(lRow,-1,LIST_ITEM_SELECTED);
	}

	else if (m_curCol>1)
	{
		CString New_CString =  WeeeklyList.GetItemText(lRow,lCol);
		if(GetDayScheduleCount(lCol)>=6&&New_CString.IsEmpty())
		{
			WeeeklyList.Set_Edit(false);
			/*AfxMessageBox(L"Can't be more than 6 !");
			is_change = TRUE;*/
			MessageBox(_T("Can't be more than 6 tasks!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
		}
		else
		{
			WeeeklyList.Set_Edit(true);
		}
	} 
	else if (m_curCol == 1)
	{
		/*int Time= GetValueItem(lRow,lCol);
		m_hour = Time/60;
		m_minute = Time - m_hour*60;
		WeeeklyList.SetItemText(lRow,lCol,L"");
		m_schedual_time_picker.ShowWindow(SW_SHOW);
		CRect list_rect,win_rect;
		WeeeklyList.GetWindowRect(list_rect);
		GetWindowRect(win_rect);
		CRect myrect;
		WeeeklyList.GetSubItemRect(lRow,lCol,LVIR_BOUNDS,myrect);
		myrect.left = myrect.left + list_rect.left - win_rect.left;
		myrect.right = myrect.right + list_rect.left - win_rect.left;
		myrect.top = myrect.top + 11;
		myrect.bottom = myrect.bottom + 13;

		m_schedual_time_picker.BringWindowToTop();
		m_schedual_time_picker.MoveWindow(myrect);

		CTime TimeTemp(2016,1,1,m_hour,m_minute,0);

		m_schedual_time_picker.SetFormat(_T("HH:mm"));
		m_schedual_time_picker.SetTime(&TimeTemp);
		m_schedual_time_picker.SetFocus();*/
	}
	else
	{
		
	}

	*pResult = 0;
}


void CNewTstatSchedulesDlg::OnNMKillfocusDatetimepicker1Schedual(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CTime temp_time;CString temp_cs;
	int chour,cmin;
	m_schedual_time_picker.GetTime(temp_time);
	chour = temp_time.GetHour();
	cmin = temp_time.GetMinute();

	*pResult = 0;
}


BOOL CNewTstatSchedulesDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetFocus()->GetDlgCtrlID() ==IDC_LIST1 )
		{
			if (pMsg->wParam == VK_DELETE)
			{
				OnBnClickedButtonDelete();
				return TRUE;
			}
			else if (pMsg->wParam == VK_INSERT)
			{
				OnBnClickedButtonInsert();
				return TRUE;
			}
			else if (pMsg->wParam == VK_RETURN)
			{
				 
					CRect list_rect,win_rect;
					WeeeklyList.GetWindowRect(list_rect);
					ScreenToClient(&list_rect);
					::GetWindowRect(this->m_hWnd,win_rect);
					WeeeklyList.Set_My_WindowRect(win_rect);
					WeeeklyList.Set_My_ListRect(list_rect);
					WeeeklyList.Get_clicked_mouse_position();
				 
				return TRUE;
			}
		}
		

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CNewTstatSchedulesDlg::OnBnClickedButtonScheduleCopyBtn()
{
	if (m_curRow == -1)
	{
		AfxMessageBox(L"Please select one row to copy!");
		return;
	}
	 list<Schedule_Node>::iterator it_node = GetNode(m_curRow);
	 Schedule_Node insertNode ;
	 int min = it_node->Hour*60+it_node->Minite;
	 min-=30;
	 insertNode.Hour = min/60;
	 insertNode.Minite = min - insertNode.Hour*60;


	 insertNode.Monday		=it_node->Monday;
	 insertNode.Tuesday		=it_node->Tuesday;
	 insertNode.Wednesday		=it_node->Wednesday;
	 insertNode.Thursday		=it_node->Thursday;
	 insertNode.Friday		=it_node->Friday;
	 insertNode.Saturday		=it_node->Saturday;
	 insertNode.Sunday		=it_node->Sunday;
	 if(!Insert_Schdule(insertNode,m_curRow))
	 {
		 CString strTemp;
		 strTemp.Format(_T("Time %02d:%02d existed!"),insertNode.Hour,insertNode.Minite);
		 AfxMessageBox(strTemp);
		 return;
	 }
	 WeeeklyList.InsertItem(m_curRow,L"");
	 Fresh_List();
}
