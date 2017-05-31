// Annual_Routines.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "Annual_Routines.h"

#include "Schedule_grid.h"

#include "AnnualRout_InsertDia.h"
#include "global_function.h"
// Annual_Routines 对话框
#define GRID_ROW_NUMBER 16
#define GRID_COL_NUMBER 3
#define FULL_LABEL_NUMBER 20
#define LABEL_NUMBER 8



IMPLEMENT_DYNAMIC(Annual_Routines, CDialog)
Annual_Routines::Annual_Routines(CWnd* pParent /*=NULL*/,CString strtype)
	: CDialog(Annual_Routines::IDD, pParent)
	, m_sChange(_T(""))
{
	row_row=1;
	m_strtype = strtype;
}
Annual_Routines::~Annual_Routines()
{

}

void Annual_Routines::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	DDX_Control(pDX, IDC_EDIT1, m_Change);
	DDX_Control(pDX, IDC_COMBO1, m_combo_col_row);
	DDX_Text(pDX, IDC_EDIT1, m_sChange);
	DDX_Control(pDX, IDOK, m_exit_button);
	DDX_Control(pDX, IDC_BUTTON1, m_clear_row_button);
}


BEGIN_MESSAGE_MAP(Annual_Routines, CDialog)
	ON_EN_SETFOCUS(IDC_EDIT1, OnEnSetfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnEnKillfocusEdit1)
	ON_CBN_SETFOCUS(IDC_COMBO1, OnCbnSetfocusCombo1)
	ON_CBN_KILLFOCUS(IDC_COMBO1, OnCbnKillfocusCombo1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Annual_Routines 消息处理程序
description2 Annual_Routines::read_addr(unsigned char addr2)
{
	description2 temp_description;
	memset(&temp_description,0,sizeof(description2));

	unsigned short temp_buffer[AR_DESCRIPTION_SIZE];
	memset(&temp_buffer[0],0,sizeof(temp_buffer));
	if(m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
		Read_Multi(g_tstat_id,temp_buffer,ANNUAL_ROUTINE_DESCRIPTION+addr2*AR_DESCRIPTION_SIZE,AR_DESCRIPTION_SIZE);
	else
		Read_Multi(g_tstat_id,temp_buffer,MODBUS_AR_DESCRIP_FIRST+addr2*AR_DESCRIPTION_SIZE,AR_DESCRIPTION_SIZE);
	int i=0;
	char *p;
	
	p=(char *)&temp_description;///////////////*****pointer   attention
	for(i=0;i<AR_DESCRIPTION_SIZE;i++)
	{
		(*p)=(char)temp_buffer[i];
		p++;
	}
	return temp_description;
}
void Annual_Routines::put_row_col(description2 temp,unsigned char t_addr)
{
	if(temp.flag==0xff )
	{
		for(int i=1;i<=GRID_COL_NUMBER;i++)
			m_FlexGrid.put_TextMatrix(t_addr,i,_T(""));
	}
	else
	{
		/*
		CString str;
		str.Format(_T("%s"),temp.full_label);
		m_FlexGrid.put_TextMatrix(t_addr,1,str);
		*/
		WCHAR wszlabel[21];  
		CString str;
	   MultiByteToWideChar( CP_ACP, 0, temp.full_label,
        strlen(temp.full_label)+1, wszlabel,   
		  sizeof(wszlabel)/sizeof(wszlabel[0]) );
		
		str.Format(_T("%s"),wszlabel);
		m_FlexGrid.put_TextMatrix(t_addr,1,str);

//		str.Format("%s",temp.label);
//		m_FlexGrid.put_TextMatrix(t_addr,4,str);

		if(temp.flag & 128)
			str=_T("MAN");
		else
			str=_T("AUTO");
        m_FlexGrid.put_TextMatrix(t_addr,2,str);
		if(str==_T("AUTO"))
		{
			m_FlexGrid.put_Row(t_addr);
			m_FlexGrid.put_Col(3);
			m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
			m_FlexGrid.put_Col(2);
		}

		if(temp.flag & 64)
			str=_T("ON");
		else
			str=_T("OFF");
        m_FlexGrid.put_TextMatrix(t_addr,3,str);
	}
}
void Annual_Routines::clear_addr(unsigned char addr2)
{
	CString str_temp;
	description temp_descri;
	memset(&temp_descri,0,sizeof(description));
	unsigned char *p;
	p=(unsigned char *) &temp_descri;
	for(int i=0;i<AR_DESCRIPTION_SIZE;i++)
		*(p++)=0xFF;
	p=(unsigned char *) &temp_descri;
	if(m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
		Write_Multi(g_tstat_id,p,ANNUAL_ROUTINE_DESCRIPTION+(addr2-1)*AR_DESCRIPTION_SIZE,AR_DESCRIPTION_SIZE);
	else
		Write_Multi(g_tstat_id,p,MODBUS_AR_DESCRIP_FIRST+(addr2-1)*AR_DESCRIPTION_SIZE,AR_DESCRIPTION_SIZE);
}

void Annual_Routines::write_addr(unsigned char addr2)
{
	CString str_temp;
	description2 temp_descri;
	memset(&temp_descri,0,sizeof(description2));
	str_temp=m_FlexGrid.get_TextMatrix(addr2,1);//1
	int i=0;
	for(i=1;i<=FULL_LABEL_NUMBER ;i++)
	{
		if(i<str_temp.GetLength()+1)
            temp_descri.full_label[i-1]=str_temp.GetAt(i-1);
		else
			temp_descri.full_label[i-1]=0;
	}
	temp_descri.full_label[FULL_LABEL_NUMBER-1]=0;

//	str_temp=m_FlexGrid.get_TextMatrix(addr2,4);//4
//	for(i=1;i<=LABEL_NUMBER;i++)
//	{
//		if(i<str_temp.GetLength()+1)
  //          temp_descri.label[i-1]=str_temp.GetAt(i-1);
	//	else
	//		temp_descri.label[i-1]=0;
//	}
	temp_descri.flag=0;
	str_temp=m_FlexGrid.get_TextMatrix(addr2,3);//3
	if(str_temp==_T("ON"))
		temp_descri.flag=temp_descri.flag | 64;
	str_temp=m_FlexGrid.get_TextMatrix(addr2,2);//2
	if(str_temp==_T("MAN"))
		temp_descri.flag=temp_descri.flag | 128;
	unsigned char *p;
	p=(unsigned char *) &temp_descri;
	if(m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
		Write_Multi(g_tstat_id,p,ANNUAL_ROUTINE_DESCRIPTION+(addr2-1)*AR_DESCRIPTION_SIZE,AR_DESCRIPTION_SIZE);
	else
		Write_Multi(g_tstat_id,p,MODBUS_AR_DESCRIP_FIRST+(addr2-1)*AR_DESCRIPTION_SIZE,AR_DESCRIPTION_SIZE);
	NET_WORK_SLEEP_BETWEEN_WRITE_READ
	temp_descri=read_addr(addr2-1);
	put_row_col(temp_descri,addr2);
}


void Annual_Routines::load_grid()
{
	m_FlexGrid.put_TextMatrix(0,0,_T("Num"));
	m_FlexGrid.put_TextMatrix(0,1,_T("Full_Label"));
	m_FlexGrid.put_TextMatrix(0,2,_T("A/m"));
	m_FlexGrid.put_TextMatrix(0,3,_T("Value"));
//	m_FlexGrid.put_TextMatrix(0,4,"Label");

	m_FlexGrid.put_TextMatrix(1,0,_T("1"));
	m_FlexGrid.put_TextMatrix(2,0,_T("2"));
	m_FlexGrid.put_TextMatrix(3,0,_T("3"));
	m_FlexGrid.put_TextMatrix(4,0,_T("4"));
	m_FlexGrid.put_TextMatrix(5,0,_T("5"));
	m_FlexGrid.put_TextMatrix(6,0,_T("6"));
	m_FlexGrid.put_TextMatrix(7,0,_T("7"));
	m_FlexGrid.put_TextMatrix(8,0,_T("8"));
	m_FlexGrid.put_TextMatrix(9,0,_T("9"));
	m_FlexGrid.put_TextMatrix(10,0,_T("10"));
	m_FlexGrid.put_TextMatrix(11,0,_T("11"));
	m_FlexGrid.put_TextMatrix(12,0,_T("12"));
	m_FlexGrid.put_TextMatrix(13,0,_T("13"));
	m_FlexGrid.put_TextMatrix(14,0,_T("14"));
	m_FlexGrid.put_TextMatrix(15,0,_T("15"));
	m_FlexGrid.put_TextMatrix(16,0,_T("16"));

	m_FlexGrid.put_ColWidth(0,400);

	description2 temp_description;
	memset(&temp_description,0,sizeof(description2));
	for(unsigned char i=1;i<=GRID_ROW_NUMBER;i++)
	{//////////////////////////////////////////////////get information from network
		temp_description=read_addr(i-1);
		put_row_col(temp_description,i);
	}
}
void Annual_Routines::on_select()
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
	/////////////////////for view button
	CString t_str;
	t_str.Format(_T("Clear %d Row"),row_row);
	m_clear_row_button.SetWindowText(t_str);

	if(row_col==3)
	{
		CString temp=m_FlexGrid.get_TextMatrix(row_row,2);
		if(temp==_T("AUTO"))
			return ;
	}
	if(row_col==2)
	{
		m_combo_col_row.ResetContent();//clear
		m_combo_col_row.AddString(_T("AUTO"));
		m_combo_col_row.AddString(_T("MAN"));
	
		m_combo_col_row.BringWindowToTop();

		m_combo_col_row.ShowWindow(SW_SHOW);//显示控件
		m_combo_col_row.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_combo_col_row.SelectString(-1,strValue); //内容全选。方便直接修改		
		m_combo_col_row.SetFocus(); //获取焦点
	}
	else if(row_col==3 )
	{
		m_combo_col_row.ResetContent();//clear
		m_combo_col_row.AddString(_T("OFF"));
		m_combo_col_row.AddString(_T("ON"));

		m_combo_col_row.BringWindowToTop();
		m_combo_col_row.ShowWindow(SW_SHOW);//显示控件
		m_combo_col_row.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_combo_col_row.SelectString(-1,strValue); //内容全选。方便直接修改		
		m_combo_col_row.SetFocus(); //获取焦点

	}
	else
	{
		m_Change.BringWindowToTop();
		m_Change.ShowWindow(SW_SHOW); //显示控件
		m_Change.SetWindowText(strValue); //显示文本
		m_Change.SetFocus(); //获取焦点
		m_Change.SetSel(0,-1); //全选
		m_Change.MoveWindow(rc); //移动到选中格的位置，覆盖
	}	
}

BOOL Annual_Routines::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	load_grid();
	NET_WORK_SET_TIMER
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BEGIN_EVENTSINK_MAP(Annual_Routines, CDialog)
	ON_EVENT(Annual_Routines, IDC_MSFLEXGRID1, DISPID_CLICK, ClickMsflexgrid1, VTS_NONE)
	ON_EVENT(Annual_Routines, IDC_MSFLEXGRID1, DISPID_DBLCLICK, DblClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void Annual_Routines::ClickMsflexgrid1()
{
	
	on_select();		
}

void Annual_Routines::OnEnSetfocusEdit1()
{
	
	NET_WORK_KILL_TIMER
	UpdateData(true);
	m_before_focus_string=m_sChange;
}

void Annual_Routines::OnEnKillfocusEdit1()
{
	
	NET_WORK_SET_TIMER
	UpdateData(true);
	m_FlexGrid.put_TextMatrix(row_row,row_col,m_sChange);//设置文本信息
	m_Change.ShowWindow(SW_HIDE); //隐藏文本控件
	UpdateData(false);
	if(m_before_focus_string!=m_sChange)
		write_addr(row_row);
}

void Annual_Routines::OnCbnSetfocusCombo1()
{
	
	NET_WORK_KILL_TIMER
	UpdateData(true);
	CString str_temp;
	m_combo_col_row.GetWindowText(str_temp);
	m_before_focus_string=str_temp;
}

void Annual_Routines::OnCbnKillfocusCombo1()
{
	CString strLabel=m_FlexGrid.get_TextMatrix(row_row,1);
	if(strLabel.IsEmpty())
		return;

	
	NET_WORK_SET_TIMER
	UpdateData(true);
	CString str_temp;
	m_combo_col_row.GetWindowText(str_temp);
	m_FlexGrid.put_TextMatrix(row_row,row_col,str_temp);
	m_combo_col_row.ShowWindow(SW_HIDE);
	UpdateData(false);
	if(m_before_focus_string!=str_temp)
		write_addr(row_row);
	if(row_col==2)
	{
		m_FlexGrid.put_Row(row_row);
		m_FlexGrid.put_Col(3);
		if(str_temp=="AUTO")
			m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
		else
			m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_WHITE_COLOR);
		m_FlexGrid.put_Col(2);
	}
}

void Annual_Routines::tab_move()
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

void Annual_Routines::shift_tab_move()
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

void Annual_Routines::left_move()
{
	shift_tab_move();
}

void Annual_Routines::right_move()
{
	tab_move();
}

void Annual_Routines::up_move()
{
	long lRow = m_FlexGrid.get_RowSel();//获取点击的行号
	long lCol = m_FlexGrid.get_ColSel(); //获取点击的列号	
	if (lRow!=1)
		m_FlexGrid.put_Row(--lRow);
	GRID_UP_MOVE
	on_select();////////////////////////
}

void Annual_Routines::down_move()
{
	long lRow =m_FlexGrid.get_RowSel();//获取点击的行号	
	long lCol = m_FlexGrid.get_ColSel(); //获取点击的列号	
	if (lRow!=m_FlexGrid.get_Rows()-1)
		m_FlexGrid.put_Row(++lRow);
	GRID_DOWN_MOVE
	on_select();////////////////////////
}

BOOL Annual_Routines::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message == WM_KEYDOWN  )
	{		
		if(pMsg->wParam == VK_INSERT )
		{				
			EXIT_BUTTON_GET_FOCUS
			Sleep(50);
			NET_WORK_KILL_TIMER
			if(m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
			{
				AnnualRout_InsertDia dlg(row_row,_T("Lightingcontroller"));
				dlg.DoModal();	
			}else
			{
				AnnualRout_InsertDia dlg(row_row);
				dlg.DoModal();
			}

			NET_WORK_SET_TIMER
			return true;
		}
		if(pMsg->wParam == VK_ESCAPE )
			return true;//use esc kaypad can't destroy this dialog
		MESSAGE_DIRECTION_CONTROL      //^-^
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

void Annual_Routines::OnDestroy()
{
	CDialog::OnDestroy();
	
	NET_WORK_KILL_TIMER
}

void Annual_Routines::OnBnClickedOk()
{
	
	OnOK();
}

void Annual_Routines::OnBnClickedButton1()
{
	
	CString str;
	str.Format(_T("Are you sure to clear %d"),row_row);
	if(IDOK==AfxMessageBox(str,MB_OKCANCEL))
	{
		clear_addr(row_row);
		m_FlexGrid.put_Row(row_row);
		m_FlexGrid.put_TextMatrix(row_row,1,_T(""));
		m_FlexGrid.put_TextMatrix(row_row,2,_T(""));
		m_FlexGrid.put_TextMatrix(row_row,3,_T(""));
		m_FlexGrid.put_Col(3);
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_WHITE_COLOR);
	}
}

void Annual_Routines::OnTimer(UINT nIDEvent)
{
	
	load_grid();
	CDialog::OnTimer(nIDEvent);
}

void Annual_Routines::DblClickMsflexgrid1()
{
	long lRow = m_FlexGrid.get_RowSel();//获取点击的行号	
	long lCol = m_FlexGrid.get_ColSel(); //获取点击的列号
	if(lRow==0 || lCol==0)
		return ;
	row_row=lRow;
	row_col=lCol;


	AnnualRout_InsertDia dlg(row_row);
			dlg.DoModal();
}
