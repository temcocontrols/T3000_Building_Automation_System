// Config_Routines.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "Config_Routines.h"
#include "global_function.h"
#include "Schedule_grid.h"



// CConfig_Routines 对话框
#define SIZEOF_DESCRIPTION 3
#define GRID_ROW_NUMBER 254
#define GRID_COL_NUMBER 6
#define SCHEDULE_ZERO	"N/A"
#define SCHEDULE_HEAD	"WR"




IMPLEMENT_DYNAMIC(CConfig_Routines, CDialog)
CConfig_Routines::CConfig_Routines(CWnd* pParent /*=NULL*/,CString strtype)
	: CDialog(CConfig_Routines::IDD, pParent)
	, m_sChange(_T(""))
{
	m_strtype = strtype;
	row_row=1;
}

CConfig_Routines::~CConfig_Routines()
{
}

void CConfig_Routines::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	DDX_Control(pDX, IDC_EDIT1, m_Change);
	DDX_Control(pDX, IDC_COMBO1, m_combo_col_row);
	DDX_Text(pDX, IDC_EDIT1, m_sChange);DDX_Control(pDX, IDOK, m_exit_button);
	DDX_Control(pDX, IDC_BUTTON1, m_clear_row_button);
}


BEGIN_MESSAGE_MAP(CConfig_Routines, CDialog)
	ON_EN_SETFOCUS(IDC_EDIT1, OnEnSetfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnEnKillfocusEdit1)
	ON_CBN_SETFOCUS(IDC_COMBO1, OnCbnSetfocusCombo1)
	ON_CBN_KILLFOCUS(IDC_COMBO1, OnCbnKillfocusCombo1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CConfig_Routines::OnBnClickedCancel)
END_MESSAGE_MAP()

	//read_addr(temp_description,36,(i-1)*36+1);
// CConfig_Routines 消息处理程序
void CConfig_Routines::read_addr(description3 *temp_description,int number,unsigned char addr2)
{	
	description3 * temp2=temp_description;
	if(m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
	{
		unsigned short temp_buffer[SIZEOF_DESCRIPTION*40];//分两次读
		for (int i=0;i<2;i++)
		{
		 Read_Multi(g_tstat_id,&temp_buffer[SIZEOF_DESCRIPTION*20*i],GROUP_SCHEDULE,SIZEOF_DESCRIPTION*20);
		}
		
		int i=0;
		char *p;
		description3 temp;
		for(int j=0;j<number;j++)
		{
			p=(char *)&temp;///////////////*****pointer   attention
			for(i=0;i<SIZEOF_DESCRIPTION;i++)
			{
				(*p)=(char)temp_buffer[j*SIZEOF_DESCRIPTION+i];//3 is size of 
				p++;
			}
			*temp_description=temp;
			temp_description++;		
		}
		temp_description=temp2;//back up the point

	}
	else
	{
		unsigned short temp_buffer[SIZEOF_DESCRIPTION*36];
		Read_Multi(g_tstat_id,temp_buffer,MODBUS_ID_FIRST + ID_SIZE * (addr2-1),SIZEOF_DESCRIPTION*number);
		int i=0;
		char *p;
		description3 temp;
		for(int j=0;j<number;j++)
		{
			p=(char *)&temp;///////////////*****pointer   attention
			for(i=0;i<SIZEOF_DESCRIPTION;i++)
			{
				(*p)=(char)temp_buffer[j*SIZEOF_DESCRIPTION+i];//3 is size of 
				p++;
			}
			*temp_description=temp;
			temp_description++;		
		}
		temp_description=temp2;//back up the point
	}

}
void CConfig_Routines::fresh_one_row(unsigned char addr2)
{
	description3 temp_description;
	read_addr(&temp_description,1,addr2);
	put_row_col(temp_description,addr2);
}
void CConfig_Routines::put_row_col(description3 temp,unsigned char t_addr)
{
	if(temp.flag==0xff )
	{
		for(int i=1;i<=GRID_COL_NUMBER;i++)
			m_FlexGrid.put_TextMatrix(t_addr,i,_T(""));
	}
	else
	{
		CString str;
		if(temp.schedul1==0)
			str=SCHEDULE_ZERO;
		else
		{
			str.Format(_T("%d"),temp.schedul1);		
			str=(CString)SCHEDULE_HEAD+str;
		}
		m_FlexGrid.put_TextMatrix(t_addr,3,str);

		if(temp.schedul2==0)
			str=SCHEDULE_ZERO;
		else
		{
			str.Format(_T("%d"),temp.schedul2);		
			str=(CString) SCHEDULE_HEAD+str;
		}
		m_FlexGrid.put_TextMatrix(t_addr,5,str);

		if(temp.flag & 128)
			str=_T("MAN");
		else
			str=_T("AUTO");
        m_FlexGrid.put_TextMatrix(t_addr,1,str);
		if(str==_T("AUTO"))
		{
			m_FlexGrid.put_Row(t_addr);
			m_FlexGrid.put_Col(2);
			m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
			m_FlexGrid.put_Col(1);//^-^
		}

		if(temp.flag & 64)
			str=_T("ON");
		else
			str=_T("OFF");
        m_FlexGrid.put_TextMatrix(t_addr,2,str);
		if(temp.flag & 32)
			str=_T("ON");
		else
			str=_T("OFF");
       m_FlexGrid.put_TextMatrix(t_addr,4,str);
		if(temp.flag & 16)
			str=_T("ON");
		else
			str=_T("OFF");
        m_FlexGrid.put_TextMatrix(t_addr,6,str);

//lsc_add:below
		m_FlexGrid.put_Row(t_addr);
		m_FlexGrid.put_Col(4);
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
		m_FlexGrid.put_Col(6);
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
	}
}
void CConfig_Routines::clear_addr(unsigned char addr2)
{
	CString str_temp;
	description3 temp_descri;
	unsigned char *p;
	p=(unsigned char *) &temp_descri;
	for(int i=0;i<SIZEOF_DESCRIPTION;i++)
		*(p++)=0xFF;
	p=(unsigned char *) &temp_descri;
	if(m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
		Write_Multi(g_tstat_id,p,GROUP_SCHEDULE+(addr2-1)*SIZEOF_DESCRIPTION,SIZEOF_DESCRIPTION);
	else
		Write_Multi(g_tstat_id,p,MODBUS_ID_FIRST+(addr2-1)*SIZEOF_DESCRIPTION,SIZEOF_DESCRIPTION);
}

void CConfig_Routines::write_addr(unsigned char addr2)
{
	CString str_temp;
	description3 temp_descri;
	str_temp=m_FlexGrid.get_TextMatrix(addr2,3);//3
	int i=0;
	if(str_temp==SCHEDULE_ZERO)
		str_temp=_T("0");
	else
		str_temp=str_temp.Right(str_temp.GetLength()-2);
	temp_descri.schedul1=_wtoi(str_temp);
	str_temp=m_FlexGrid.get_TextMatrix(addr2,5);//5
	if(str_temp==SCHEDULE_ZERO)
		str_temp=_T("0");
	else
		str_temp=str_temp.Right(str_temp.GetLength()-2);
	temp_descri.schedul2=_wtoi(str_temp);
	temp_descri.flag=0;
	str_temp=m_FlexGrid.get_TextMatrix(addr2,6);//6
	if(str_temp==_T("ON"))
		temp_descri.flag=temp_descri.flag | 16;
	str_temp=m_FlexGrid.get_TextMatrix(addr2,4);//4
	if(str_temp==_T("ON"))
		temp_descri.flag=temp_descri.flag | 32;
	str_temp=m_FlexGrid.get_TextMatrix(addr2,2);//4
	if(str_temp==_T("ON"))
		temp_descri.flag=temp_descri.flag | 64;
	str_temp=m_FlexGrid.get_TextMatrix(addr2,1);//4
	if(str_temp==_T("MAN"))
		temp_descri.flag=temp_descri.flag | 128;
	unsigned char *p;
	p=(unsigned char *) &temp_descri;
	if(m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
		Write_Multi(g_tstat_id,p,GROUP_SCHEDULE + ID_SIZE * (addr2-1),SIZEOF_DESCRIPTION);
	else
		Write_Multi(g_tstat_id,p,MODBUS_ID_FIRST + ID_SIZE * (addr2-1),SIZEOF_DESCRIPTION);
	NET_WORK_SLEEP_BETWEEN_WRITE_READ
	read_addr(&temp_descri,1,addr2);
	put_row_col(temp_descri,addr2);
}


void CConfig_Routines::load_grid()
{
	

	CString str;
	int intemp;
	if(m_strtype.CompareNoCase(_T("Lightingcontroller")) == 0)
	{
		m_FlexGrid.put_Cols(8);
		m_FlexGrid.put_Rows(41);
		m_FlexGrid.put_TextMatrix(0,0,_T("Num"));
		 
		m_FlexGrid.put_TextMatrix(0,1,_T("A/M"));
		m_FlexGrid.put_TextMatrix(0,2,_T("Value"));
		m_FlexGrid.put_TextMatrix(0,3,_T("Schedul1"));
		m_FlexGrid.put_TextMatrix(0,4,_T("state1"));
		m_FlexGrid.put_TextMatrix(0,5,_T("Schedul2"));
		m_FlexGrid.put_TextMatrix(0,6,_T("state2"));
		m_FlexGrid.put_TextMatrix(0,7,_T("Group Name"));
	    intemp = 40;

	}
	else
	{
		m_FlexGrid.put_TextMatrix(0,0,_T("Num"));
		m_FlexGrid.put_TextMatrix(0,1,_T("A/M"));
		m_FlexGrid.put_TextMatrix(0,2,_T("Value"));
		m_FlexGrid.put_TextMatrix(0,3,_T("Schedul1"));
		m_FlexGrid.put_TextMatrix(0,4,_T("state1"));
		m_FlexGrid.put_TextMatrix(0,5,_T("Schedul2"));
		m_FlexGrid.put_TextMatrix(0,6,_T("state2"));
		intemp = 254;
	
	}
	
	//for(int i=1;i<=GRID_ROW_NUMBER;i++)
	for(int i=1;i<=intemp;i++)
	{
	   if (m_strtype.CompareNoCase(_T("Lightingcontroller"))!=0)
	   {
		   str.Format(_T("%d"),i);
		   m_FlexGrid.put_TextMatrix(i,0,str);
		   m_FlexGrid.put_Row(i);
	   } 
	   else
	   {
		   str.Format(_T("%d"),i);
		   m_FlexGrid.put_TextMatrix(i,0,str);
		   str=Get_Table_Name(m_sn,_T("Group"),i);
		   m_FlexGrid.put_TextMatrix(i,7,str);
		   m_FlexGrid.put_Row(i);
	   }
		

	}
	m_FlexGrid.put_ColWidth(7,1000);
	if(m_strtype.CompareNoCase(_T("Lightingcontroller")) != 0)//IS NC
	{
		description3 temp_description[36];

		for(unsigned short i=1;i<=7;i++)
		{    //////////////////////////////////////////////////get information from network
			read_addr(temp_description,36,(i-1)*36+1);
			for(int j=0;j<36;j++)
				put_row_col(temp_description[j],(i-1)*36+j+1);
		}
		read_addr(temp_description,2,253);
		put_row_col(temp_description[0],253);
		put_row_col(temp_description[1],254);
		m_FlexGrid.put_Row(g_tstat_id);
	}
	else
	{
		//description3 temp_description[36];
		   description3 temp_description[40];
		  read_addr(temp_description,40,1);
	
			for(int j=0;j<40;j++)
				put_row_col(temp_description[j],j+1);

	}





	for(int i=1;i<=GRID_COL_NUMBER;i++)
	{
		m_FlexGrid.put_Col(i);
        m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_COLOR);
	}
	m_FlexGrid.put_Row(1);
	m_FlexGrid.put_Col(1);

}


/// <summary>
/// select group routines
/// </summary>
void CConfig_Routines::on_select()
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

	if(row_row==g_tstat_id )
		return;
	else if(row_col==2)
	{
		CString temp=m_FlexGrid.get_TextMatrix(row_row,1);
		if(temp==_T("AUTO"))
			return ;
	}
	if(row_col==4 || row_col==6)
		return;
	if(row_col==1)
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
	else if( row_col==2 )
	{
		m_combo_col_row.ResetContent();//clear
		m_combo_col_row.AddString(_T("OFF"));
		m_combo_col_row.AddString(_T("ON"));

		m_combo_col_row.ShowWindow(SW_SHOW);//显示控件
		m_combo_col_row.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_combo_col_row.SelectString(-1,strValue); //内容全选。方便直接修改		
		m_combo_col_row.SetFocus(); //获取焦点
	}
	else if (row_col==7&&row_row!=0)
	{
		m_Change.ShowWindow(SW_SHOW); //显示控件
		m_Change.SetWindowText(strValue); //显示文本
		m_Change.SetFocus(); //获取焦点
		m_Change.SetSel(0,-1); //全选
		m_Change.MoveWindow(rc); //移动到选中格的位置，覆盖
	}
	else
	{
		m_Change.ShowWindow(SW_SHOW); //显示控件
		m_Change.SetWindowText(strValue); //显示文本
		m_Change.SetFocus(); //获取焦点
		m_Change.SetSel(0,-1); //全选
		m_Change.MoveWindow(rc); //移动到选中格的位置，覆盖
	}	

}

BOOL CConfig_Routines::OnInitDialog()
{
	CDialog::OnInitDialog();
	unsigned short Data[4];
	if (!is_connect())
	{
	AfxMessageBox(_T("Disconnection,Please connect!"));
	}
	Read_Multi(g_tstat_id,Data,0,4);
	m_sn=Data[0]+Data[1]*256+Data[2]*256*256+Data[3]*256*256*256;

	load_grid();
//	SetPaneString(_T("Finish"));
	NET_WORK_SET_TIMER
	return TRUE;  
	// return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BEGIN_EVENTSINK_MAP(CConfig_Routines, CDialog)
	ON_EVENT(CConfig_Routines, IDC_MSFLEXGRID1, DISPID_CLICK, ClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void CConfig_Routines::ClickMsflexgrid1()
{
	
	on_select();		
}

void CConfig_Routines::OnEnSetfocusEdit1()
{
	 
	NET_WORK_KILL_TIMER
	UpdateData(true);
	m_before_focus_string=m_sChange;
}

void CConfig_Routines::OnEnKillfocusEdit1()
{
	
	UpdateData(true);
	if(row_col==3 || row_col==5)
	{
		if(m_sChange.GetLength()==1 || m_sChange.GetLength()==2)
		{
			if(_wtoi(m_sChange)>16 || _wtoi(m_sChange)<0)
			{
				AfxMessageBox(_T("Wrong input!"));
				m_Change.SetFocus();
				return ;
			}
			m_sChange=(CString)SCHEDULE_HEAD+m_sChange;
		}
		else 
			m_sChange.MakeUpper();

		m_FlexGrid.put_TextMatrix(row_row,row_col,m_sChange);//设置文本信息
		m_Change.ShowWindow(SW_HIDE); //隐藏文本控件
		UpdateData(false);
		if(m_before_focus_string!=m_sChange)
			write_addr(row_row);
	}
	else if (row_col==7)
	{
		unsigned char lable_buffer[GROUP_LABLE_SIZE]={0};
		for (int k=0;k<m_sChange.GetLength();k++)
		{
			if (k>=10)
				break;
			lable_buffer[k]=(char)m_sChange.GetAt(k);
		}
		Write_Multi(g_tstat_id,lable_buffer,GROUP_DESCRIPTION_REG+(row_row-1)*GROUP_LABLE_SIZE,GROUP_LABLE_SIZE);
		Insert_Update_Table_Name(m_sn,_T("Group"),row_row,m_sChange);
		m_FlexGrid.put_TextMatrix(row_row,row_col,m_sChange);  
	}
	NET_WORK_SET_TIMER
}

void CConfig_Routines::OnCbnSetfocusCombo1()
{
	
	NET_WORK_KILL_TIMER
	UpdateData(true);
	CString str_temp;
	m_combo_col_row.GetWindowText(str_temp);
	m_before_focus_string=str_temp;

}

void CConfig_Routines::OnCbnKillfocusCombo1()
{
	
	UpdateData(true);
	CString str_temp;
	m_combo_col_row.GetWindowText(str_temp);
	m_FlexGrid.put_TextMatrix(row_row,row_col,str_temp);
	m_combo_col_row.ShowWindow(SW_HIDE);
	UpdateData(false);
	if(m_before_focus_string!=str_temp)
		write_addr(row_row);
	if(row_col==1)
	{
		m_FlexGrid.put_Row(row_row);
		m_FlexGrid.put_Col(2);
		if(str_temp==_T("AUTO"))
			m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
		else
			m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_WHITE_COLOR);
		m_FlexGrid.put_Col(1);//^-^
	}
	NET_WORK_SET_TIMER
}

void CConfig_Routines::tab_move()
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

void CConfig_Routines::shift_tab_move()
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

void CConfig_Routines::left_move()
{
	shift_tab_move();
}

void CConfig_Routines::right_move()
{
	tab_move();
}

void CConfig_Routines::up_move()
{
	long lRow = m_FlexGrid.get_RowSel();//获取点击的行号
	long lCol = m_FlexGrid.get_ColSel(); //获取点击的列号	
	if (lRow!=1)
		m_FlexGrid.put_Row(--lRow);
	GRID_UP_MOVE
	on_select();////////////////////////
}

void CConfig_Routines::down_move()
{
	long lRow =m_FlexGrid.get_RowSel();//获取点击的行号	
	long lCol = m_FlexGrid.get_ColSel(); //获取点击的列号	
	if (lRow!=m_FlexGrid.get_Rows()-1)
		m_FlexGrid.put_Row(++lRow);
	GRID_DOWN_MOVE
	on_select();////////////////////////
}

BOOL CConfig_Routines::PreTranslateMessage(MSG* pMsg)
{
	
/*	if(pMsg->message == WM_RBUTTONDOWN )
	{		//
			CMenu menu;
			menu.LoadMenu(IDR_TREE_POPUP_MENU);
			CMenu *pmenu=menu.GetSubMenu(4);	
			CPoint point=pMsg->pt;
	//		ClientToScreen(&point);
	//		ScreenToClient(&point);
			pmenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN ,point.x,point.y,this);
			return true;
	}
	*/
	if(pMsg->message == WM_KEYDOWN  )
	{		
		if(pMsg->wParam == VK_ESCAPE )
			return true;//use esc kaypad can't destroy this dialog
		MESSAGE_DIRECTION_CONTROL
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

void CConfig_Routines::OnDestroy()
{
	CDialog::OnDestroy();
	
	NET_WORK_KILL_TIMER
}
void CConfig_Routines::OnBnClickedButton1()
{
	
	//clear one row
	CString str;
	str.Format(_T("Are you sure to clear %d"),row_row);
	if(IDOK==AfxMessageBox(str,MB_OKCANCEL))
	{
		clear_addr(row_row);
		m_FlexGrid.put_Row(row_row);
		m_FlexGrid.put_Col(1);
		m_FlexGrid.put_TextMatrix(row_row,1,_T(""));
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_WHITE_COLOR);
		m_FlexGrid.put_Col(2);
		m_FlexGrid.put_TextMatrix(row_row,2,_T(""));
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_WHITE_COLOR);
		m_FlexGrid.put_Col(3);
		m_FlexGrid.put_TextMatrix(row_row,3,_T(""));
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_WHITE_COLOR);
		m_FlexGrid.put_Col(4);
		m_FlexGrid.put_TextMatrix(row_row,4,_T(""));
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
		m_FlexGrid.put_Col(5);
		m_FlexGrid.put_TextMatrix(row_row,5,_T(""));
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_WHITE_COLOR);
		m_FlexGrid.put_Col(6);
		m_FlexGrid.put_TextMatrix(row_row,6,_T(""));
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
	}
}

void CConfig_Routines::OnTimer(UINT nIDEvent)
{
	
	load_grid();
	CDialog::OnTimer(nIDEvent);
}

void CConfig_Routines::OnBnClickedCancel()
{
	
	OnCancel();
}
