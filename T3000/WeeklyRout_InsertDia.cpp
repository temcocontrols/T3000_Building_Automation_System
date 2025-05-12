// WeeklyRout_InsertDia.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "WeeklyRout_InsertDia.h"
#include "schedule.h"
#include "global_function.h"
#include "schedule.h"



// WeeklyRout_InsertDia 对话框
#define GRID_ROW_NUMBER 9
#define GRID_COL_NUMBER 10



IMPLEMENT_DYNAMIC(WeeklyRout_InsertDia, CDialog)
WeeklyRout_InsertDia::WeeklyRout_InsertDia(unsigned char row,CString strtype,CWnd* pParent /*=NULL*/)
	: CDialog(WeeklyRout_InsertDia::IDD, pParent)
	, m_sChange(_T(""))
	, m_mon(FALSE)
	, m_tue(FALSE)
	, m_wed(FALSE)
	, m_thu(FALSE)
	, m_fri(FALSE)
	, m_sat(FALSE)
	, m_sun(FALSE)
	, m_hol1(FALSE)
	, m_hol2(FALSE)
{
	m_addr=row;
	m_strtype = strtype;
}

WeeklyRout_InsertDia::~WeeklyRout_InsertDia()
{

}
void WeeklyRout_InsertDia::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	DDX_Control(pDX, IDC_COMBO1, m_combo_col_row);
	DDX_Control(pDX, IDC_EDIT1, m_Change);
	DDX_Text(pDX, IDC_EDIT1, m_sChange);
	DDX_Control(pDX, IDOK, m_exit_button);
	DDX_Check(pDX, IDC_CHECK1, m_mon);
	DDX_Check(pDX, IDC_CHECK2, m_tue);
	DDX_Check(pDX, IDC_CHECK3, m_wed);
	DDX_Check(pDX, IDC_CHECK4, m_thu);
	DDX_Check(pDX, IDC_CHECK5, m_fri);
	DDX_Check(pDX, IDC_CHECK6, m_sat);
	DDX_Check(pDX, IDC_CHECK7, m_sun);
	DDX_Check(pDX, IDC_CHECK8, m_hol1);
	DDX_Check(pDX, IDC_CHECK9, m_hol2);
	DDX_Control(pDX, IDC_CHECK1, m_mon_ctrl);
	DDX_Control(pDX, IDC_CHECK2, m_tue_ctrl);
	DDX_Control(pDX, IDC_CHECK3, m_wed_ctrl);
	DDX_Control(pDX, IDC_CHECK4, m_thu_ctrl);
	DDX_Control(pDX, IDC_CHECK5, m_fri_ctrl);
	DDX_Control(pDX, IDC_CHECK6, m_sat_ctrl);
	DDX_Control(pDX, IDC_CHECK7, m_sun_ctrl);
	DDX_Control(pDX, IDC_CHECK8, m_hol1_ctrl);
	DDX_Control(pDX, IDC_CHECK9, m_hol2_ctrl);
}
void WeeklyRout_InsertDia::load_grid()
{
	m_FlexGrid.put_TextMatrix(0,1,_T("Mon"));
	m_FlexGrid.put_TextMatrix(0,2,_T("Tue"));
	m_FlexGrid.put_TextMatrix(0,3,_T("Wed"));
	m_FlexGrid.put_TextMatrix(0,4,_T("Thu"));
	m_FlexGrid.put_TextMatrix(0,5,_T("Fri"));
	m_FlexGrid.put_TextMatrix(0,6,_T("Sat"));
	m_FlexGrid.put_TextMatrix(0,7,_T("Sun"));
	m_FlexGrid.put_TextMatrix(0,8,_T("Holiday1"));
	m_FlexGrid.put_TextMatrix(0,9,_T("Holiday2"));

	m_FlexGrid.put_TextMatrix(1,0,_T("ON"));
	m_FlexGrid.put_TextMatrix(2,0,_T("OFF"));
	m_FlexGrid.put_TextMatrix(3,0,_T("ON"));
	m_FlexGrid.put_TextMatrix(4,0,_T("OFF"));
	m_FlexGrid.put_TextMatrix(5,0,_T("ON"));
	m_FlexGrid.put_TextMatrix(6,0,_T("OFF"));
	m_FlexGrid.put_TextMatrix(7,0,_T("ON"));
	m_FlexGrid.put_TextMatrix(8,0,_T("OFF"));

	m_FlexGrid.put_ColWidth(0,400);

	unsigned short p[72]={0};
	memset(p,0,sizeof(p));
	if (m_strtype.CompareNoCase(_T("LightingController")) == 0)
		Read_Multi(g_tstat_id,p,WEEKLY_ROUTINE_ON_TIME + WR_TIME_SIZE*(m_addr-1),0x48);//由上到下，由左到右，from up to lower ,from left to right
	else
		Read_Multi(g_tstat_id,p,MODBUS_WR_ONTIME_FIRST + WR_TIME_SIZE*(m_addr-1),0x48);//由上到下，由左到右，from up to lower ,from left to right
	CString str;
	int i=0,j=0,k=0;
	for(i=1;i<GRID_COL_NUMBER;i++)
		for(j=1;j<GRID_ROW_NUMBER;j++)
		{
			if(j%2==1)
			{
				k=(i-1)*(GRID_ROW_NUMBER-1)+j-1;//1
				int itemp1,itemp2;
				if(p[k]==255)
					itemp1=0;
				else
					itemp1=p[k];
				if(p[k+1]==255)
					itemp2=0;
				else
					itemp2=p[k+1];
				str.Format(_T("%d:%d"),itemp1,itemp2);
				inspect_input_string(j,i,str);///////////////inspect
				m_FlexGrid.put_TextMatrix(j,i,str);
			}
		}
		
	if (m_strtype.CompareNoCase(_T("LightingController")) == 0)
		Read_Multi(g_tstat_id,p,WEEKLY_ROUTINE_OFF_TIME + WR_TIME_SIZE*(m_addr-1),0x48);//由上到下，由左到右，from up to lower ,from left to right
	else
		Read_Multi(g_tstat_id,p,MODBUS_WR_OFFTIME_FIRST + WR_TIME_SIZE*(m_addr-1),0x48);//由上到下，由左到右，from up to lower ,from left to right
	for(i=1;i<GRID_COL_NUMBER;i++)
		for(j=1;j<GRID_ROW_NUMBER;j++)
		{
			if(j%2==0)
			{
				k=(i-1)*(GRID_ROW_NUMBER-1)+j-2;//2
				int itemp1,itemp2;
				if(p[k]==255)
					itemp1=0;
				else
					itemp1=p[k];
				if(p[k+1]==255)
					itemp2=0;
				else
					itemp2=p[k+1];
				str.Format(_T("%d:%d"),itemp1,itemp2);
				inspect_input_string(j,i,str);///////////////inspect
				m_FlexGrid.put_TextMatrix(j,i,str);
			}
		}
		
}
void WeeklyRout_InsertDia::get_two_value_from_one_grid(CString str2,unsigned char &a,unsigned char &b)
{
	int i=str2.Find(':');
	a=_wtoi(str2.Left(i));
	b=_wtoi(str2.Right(str2.GetLength()-i-1));
}
void WeeklyRout_InsertDia::write_grid()
{
	UpdateData(true);
	unsigned char p[72];
	CString str;
	unsigned char left_value=0,right_value=0;
	int i=0,j=0,k=0;
	
	for(i=1;i<GRID_COL_NUMBER;i++)
		for(j=1;j<GRID_ROW_NUMBER;j++)
		{//write On:
			if(j%2==1)
			{
				str=m_FlexGrid.get_TextMatrix(j,i);
				get_two_value_from_one_grid(str,left_value,right_value);
				p[k]=left_value;
				p[k+1]=right_value;
				k+=2;
			}
		}

		if (m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
			Write_Multi(g_tstat_id,p,WEEKLY_ROUTINE_ON_TIME + WR_TIME_SIZE*(m_addr-1),0x48);
		else
			Write_Multi(g_tstat_id,p,MODBUS_WR_ONTIME_FIRST + WR_TIME_SIZE*(m_addr-1),0x48);
	k=0;//////////////////////////////////clear
	for(int i=0;i<72;i++)
	{
		p[i]=0;
	}
	
	for(i=1;i<GRID_COL_NUMBER;i++)
		for(j=1;j<GRID_ROW_NUMBER;j++)
		{
			if(j%2==0)//write Off
			{
				str=m_FlexGrid.get_TextMatrix(j,i);
				get_two_value_from_one_grid(str,left_value,right_value);
				p[k]=left_value;
				p[k+1]=right_value;
				k+=2;
			}
		}
//	for(int i=0;i<72;i++)
//		p[i]=0;
	Sleep(10);
	if (m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
		Write_Multi(g_tstat_id,p,WEEKLY_ROUTINE_OFF_TIME + WR_TIME_SIZE*(m_addr-1),0x48);
	else
		Write_Multi(g_tstat_id,p,MODBUS_WR_OFFTIME_FIRST + WR_TIME_SIZE*(m_addr-1),0x48);
	
}
void WeeklyRout_InsertDia::on_select()
{
	long lRow,lCol;
	lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
	UpdateData(false);
	if(lRow==0 || lCol==0)
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
	row_row=lRow;
	row_col=lCol;
	disable_one_day(row_col);//////////////////gray one day

	m_Change.ShowWindow(SW_SHOW); //显示控件
	m_Change.SetWindowText(strValue); //显示文本
	m_Change.SetFocus(); //获取焦点
	m_Change.SetSel(0,-1); //全选
	m_Change.MoveWindow(rc); //移动到选中格的位置，覆盖
}


BEGIN_MESSAGE_MAP(WeeklyRout_InsertDia, CDialog)
	ON_EN_SETFOCUS(IDC_EDIT1, OnEnSetfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnEnKillfocusEdit1)
	ON_CBN_SETFOCUS(IDC_COMBO1, OnCbnSetfocusCombo1)
	ON_CBN_KILLFOCUS(IDC_COMBO1, OnCbnKillfocusCombo1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_COPY_TO, OnBnClickedCopyTo)
	ON_BN_CLICKED(IDCANCEL, &WeeklyRout_InsertDia::OnBnClickedCancel)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDOK, &WeeklyRout_InsertDia::OnBnClickedOk)
END_MESSAGE_MAP()

// WeeklyRout_InsertDia 消息处理程序
BOOL WeeklyRout_InsertDia::OnInitDialog()
{
	CDialog::OnInitDialog();


	
	CString str;
	str.Format(_T("Weekly Routines %d"),m_addr);
	this->SetWindowText(str);
	load_grid();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BEGIN_EVENTSINK_MAP(WeeklyRout_InsertDia, CDialog)
	ON_EVENT(WeeklyRout_InsertDia, IDC_MSFLEXGRID1, DISPID_CLICK, ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void WeeklyRout_InsertDia::ClickMsflexgrid1()
{
	
	on_select();
}

void WeeklyRout_InsertDia::OnEnSetfocusEdit1()
{
	
	UpdateData(true);
	//m_before_focus_string=m_sChange;
}
bool WeeklyRout_InsertDia::inspect_input_string(int Row,int Col,CString &str)
{
	CString str_left,str_right;
	int momm_pos=str.Find(':');
	str_left=str.Left(momm_pos);
	str_left.Format(_T("%d"),_wtoi(str_left));
	if(str_left.GetLength()==1)
		str_left=_T("0")+str_left;
	else if(str_left.GetLength()==0)
		str_left=_T("00");
	str_right=str.Right(str.GetLength()-momm_pos-1);
	str_right.Format(_T("%d"),_wtoi(str_right));
	if(str_right.GetLength()==1)
		str_right=_T("0")+str_right;
	else if(str_right.GetLength()==0)
		str_right=_T("00");
	str=str_left+_T(":")+str_right;
	if(_wtoi(str_left)<0 || _wtoi(str_right)<0 || _wtoi(str_left)>23 || _wtoi(str_right)>59)
	{
		str_left.Format(_T("Wrong Time Grid at Row:%d,Col:%d"),Row,Col);
		AfxMessageBox(str_left);
		return false;
	}
	return true;
}
void WeeklyRout_InsertDia::OnEnKillfocusEdit1()
{
	
	UpdateData(true);
	inspect_input_string(row_row,row_col,m_sChange);///////////////inspect
	m_FlexGrid.put_TextMatrix(row_row,row_col,m_sChange);//设置文本信息
	m_Change.ShowWindow(SW_HIDE); //隐藏文本控件
	UpdateData(false);
}

void WeeklyRout_InsertDia::OnCbnSetfocusCombo1()
{
	
	UpdateData(true);
}

void WeeklyRout_InsertDia::OnCbnKillfocusCombo1()
{
	
	UpdateData(true);
	CString str_temp;
	m_combo_col_row.GetWindowText(str_temp);
	inspect_input_string(row_row,row_col,str_temp);///////////////inspect
	m_FlexGrid.put_TextMatrix(row_row,row_col,str_temp);
	m_combo_col_row.ShowWindow(SW_HIDE);
	UpdateData(false);
}
void WeeklyRout_InsertDia::OnDestroy()
{
	CDialog::OnDestroy();
	
}
void WeeklyRout_InsertDia::OnCancel()
{
	
	CDialog::OnCancel();
}
BOOL WeeklyRout_InsertDia::DestroyWindow()
{
	
	write_grid();//////////////////////////////////////////////write_multi only ont times
	return CDialog::DestroyWindow();
}
void WeeklyRout_InsertDia::tab_move()
{
	long lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	long lCol = m_FlexGrid.get_ColSel(); //获取点击的列号	
	if(lRow==m_FlexGrid.get_Rows()-1 && lCol==m_FlexGrid.get_Cols()-1)
	{
	}
	else if (lCol==m_FlexGrid.get_Cols()-1)
	{
		lRow++;
		lCol=1;
		m_FlexGrid.put_Row(lRow);
		m_FlexGrid.put_Col(1);		
	}
	else
	{
		m_FlexGrid.put_Row(lRow);
		m_FlexGrid.put_Col(++lCol);		
	}			
	GRID_DOWN_MOVE	
	on_select();////////////////////////		
}

void WeeklyRout_InsertDia::shift_tab_move()
{
	long lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	long lCol = m_FlexGrid.get_ColSel(); //获取点击的列号	
	if(lRow==1 && lCol==1)
	{
	}
	else if (lCol==1)
	{		
		m_FlexGrid.put_Row(--lRow);
		m_FlexGrid.put_Col(m_FlexGrid.get_Cols()-1);		
	}
	else
	{
		m_FlexGrid.put_Row(lRow);
		m_FlexGrid.put_Col(--lCol);		
	}	
	GRID_UP_MOVE
	on_select();////////////////////////
}

void WeeklyRout_InsertDia::left_move()
{
	shift_tab_move();
}

void WeeklyRout_InsertDia::right_move()
{
	tab_move();
}

void WeeklyRout_InsertDia::up_move()
{
	long lRow = m_FlexGrid.get_RowSel();//获取点击的行号
	long lCol = m_FlexGrid.get_ColSel(); //获取点击的列号	
	if (lRow!=1)
		m_FlexGrid.put_Row(--lRow);
	GRID_UP_MOVE
	on_select();////////////////////////
}

void WeeklyRout_InsertDia::down_move()
{
	long lRow =m_FlexGrid.get_RowSel();//获取点击的行号	
	long lCol = m_FlexGrid.get_ColSel(); //获取点击的列号	
	if (lRow!=m_FlexGrid.get_Rows()-1)
		m_FlexGrid.put_Row(++lRow);
	GRID_DOWN_MOVE
	on_select();////////////////////////
}

BOOL WeeklyRout_InsertDia::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message == WM_KEYDOWN  )
	{		
		if(pMsg->wParam == VK_ESCAPE )
			return true;//use esc kaypad can't destroy this dialog
		MESSAGE_DIRECTION_CONTROL  //         ^-^
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void WeeklyRout_InsertDia::OnBnClickedCopyTo()
{//COPY TO BUTTON
	
	UpdateData(true);
	if(m_mon)
		col_copy_to_col(row_col,1);
	if(m_tue)
		col_copy_to_col(row_col,2);
	if(m_wed)
		col_copy_to_col(row_col,3);
	if(m_thu)
		col_copy_to_col(row_col,4);
	if(m_fri)
		col_copy_to_col(row_col,5);
	if(m_sat)
		col_copy_to_col(row_col,6);
	if(m_sun)
		col_copy_to_col(row_col,7);
	if(m_hol1)
		col_copy_to_col(row_col,8);
	if(m_hol2)
		col_copy_to_col(row_col,9);
}
void WeeklyRout_InsertDia::col_copy_to_col(int base_col,int order_col)
{//copy from one col to another col
	CString str;
	for(int i=1;i<9;i++)
	{
		str=m_FlexGrid.get_TextMatrix(i,base_col);
		m_FlexGrid.put_TextMatrix(i,order_col,str);
	}
}
void WeeklyRout_InsertDia::disable_one_day(int day_number)
{
	switch(day_number)
	{
	case 1:m_mon=false;m_mon_ctrl.EnableWindow(false);m_tue_ctrl.EnableWindow(true);m_wed_ctrl.EnableWindow(true);m_thu_ctrl.EnableWindow(true);m_fri_ctrl.EnableWindow(true);m_sat_ctrl.EnableWindow(true);m_sun_ctrl.EnableWindow(true);m_hol1_ctrl.EnableWindow(true);m_hol2_ctrl.EnableWindow(true);break;
	case 2:m_tue=false;m_mon_ctrl.EnableWindow(true);m_tue_ctrl.EnableWindow(false);m_wed_ctrl.EnableWindow(true);m_thu_ctrl.EnableWindow(true);m_fri_ctrl.EnableWindow(true);m_sat_ctrl.EnableWindow(true);m_sun_ctrl.EnableWindow(true);m_hol1_ctrl.EnableWindow(true);m_hol2_ctrl.EnableWindow(true);break;
	case 3:m_wed=false;m_mon_ctrl.EnableWindow(true);m_tue_ctrl.EnableWindow(true);m_wed_ctrl.EnableWindow(false);m_thu_ctrl.EnableWindow(true);m_fri_ctrl.EnableWindow(true);m_sat_ctrl.EnableWindow(true);m_sun_ctrl.EnableWindow(true);m_hol1_ctrl.EnableWindow(true);m_hol2_ctrl.EnableWindow(true);break;
	case 4:m_thu=false;m_mon_ctrl.EnableWindow(true);m_tue_ctrl.EnableWindow(true);m_wed_ctrl.EnableWindow(true);m_thu_ctrl.EnableWindow(false);m_fri_ctrl.EnableWindow(true);m_sat_ctrl.EnableWindow(true);m_sun_ctrl.EnableWindow(true);m_hol1_ctrl.EnableWindow(true);m_hol2_ctrl.EnableWindow(true);break;
	case 5:m_fri=false;m_mon_ctrl.EnableWindow(true);m_tue_ctrl.EnableWindow(true);m_wed_ctrl.EnableWindow(true);m_thu_ctrl.EnableWindow(true);m_fri_ctrl.EnableWindow(false);m_sat_ctrl.EnableWindow(true);m_sun_ctrl.EnableWindow(true);m_hol1_ctrl.EnableWindow(true);m_hol2_ctrl.EnableWindow(true);break;
	case 6:m_sat=false;m_mon_ctrl.EnableWindow(true);m_tue_ctrl.EnableWindow(true);m_wed_ctrl.EnableWindow(true);m_thu_ctrl.EnableWindow(true);m_fri_ctrl.EnableWindow(true);m_sat_ctrl.EnableWindow(false);m_sun_ctrl.EnableWindow(true);m_hol1_ctrl.EnableWindow(true);m_hol2_ctrl.EnableWindow(true);break;
	case 7:m_sun=false;m_mon_ctrl.EnableWindow(true);m_tue_ctrl.EnableWindow(true);m_wed_ctrl.EnableWindow(true);m_thu_ctrl.EnableWindow(true);m_fri_ctrl.EnableWindow(true);m_sat_ctrl.EnableWindow(true);m_sun_ctrl.EnableWindow(false);m_hol1_ctrl.EnableWindow(true);m_hol2_ctrl.EnableWindow(true);break;
	case 8:m_hol1=false;m_mon_ctrl.EnableWindow(true);m_tue_ctrl.EnableWindow(true);m_wed_ctrl.EnableWindow(true);m_thu_ctrl.EnableWindow(true);m_fri_ctrl.EnableWindow(true);m_sat_ctrl.EnableWindow(true);m_sun_ctrl.EnableWindow(true);m_hol1_ctrl.EnableWindow(false);m_hol2_ctrl.EnableWindow(true);break;
	case 9:m_hol2=false;m_mon_ctrl.EnableWindow(true);m_tue_ctrl.EnableWindow(true);m_wed_ctrl.EnableWindow(true);m_thu_ctrl.EnableWindow(true);m_fri_ctrl.EnableWindow(true);m_sat_ctrl.EnableWindow(true);m_sun_ctrl.EnableWindow(true);m_hol1_ctrl.EnableWindow(true);m_hol2_ctrl.EnableWindow(false);break;
	default:m_mon_ctrl.EnableWindow(true);m_tue_ctrl.EnableWindow(true);m_wed_ctrl.EnableWindow(true);m_thu_ctrl.EnableWindow(true);m_fri_ctrl.EnableWindow(true);m_sat_ctrl.EnableWindow(true);m_sun_ctrl.EnableWindow(true);m_hol1_ctrl.EnableWindow(true);m_hol2_ctrl.EnableWindow(true);
	}
	UpdateData(false);
}
void WeeklyRout_InsertDia::OnBnClickedCancel()
{
	
	OnCancel();
}
void WeeklyRout_InsertDia::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	 
	CDialog::OnLButtonDblClk(nFlags, point);
}

void WeeklyRout_InsertDia::OnBnClickedOk()
{
	
	OnOK();
}
