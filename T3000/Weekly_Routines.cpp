// Weekly_Routines.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "Weekly_Routines.h"
#include "globle_function.h"
#include "WeeklyRout_InsertDia.h"



// Weekly_Routines �Ի���
#define GRID_ROW_NUMBER 20
#define GRID_COL_NUMBER 7
#define FULL_LABEL_NUMBER 20
#define LABEL_NUMBER 8
#define HOLIDAY_ZERO	"N/A"
#define HOLIDAY_HEAD	"AR"
IMPLEMENT_DYNAMIC(Weekly_Routines, CDialog)
Weekly_Routines::Weekly_Routines(CWnd* pParent /*=NULL*/, CString strtype )
	: CDialog(Weekly_Routines::IDD, pParent)
	, m_sChange(_T(""))
{
	row_row=1;
	m_strproducttype =strtype;
}

// Weekly_Routines::Weekly_Routines(CWnd* pParent /*=NULL*/, CString strtype )
// {
// 	row_row=1;
// 	m_sChange = _T("");
// 	m_strproducttype = _T("");
// }
Weekly_Routines::~Weekly_Routines()
{
}

void Weekly_Routines::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID1, m_FlexGrid);
	DDX_Control(pDX, IDC_EDIT1, m_Change);
	DDX_Control(pDX, IDC_COMBO1, m_combo_col_row);
	DDX_Text(pDX, IDC_EDIT1, m_sChange);
	DDX_Control(pDX, IDOK, m_exit_button);
	DDX_Control(pDX, IDC_CLEAR, m_clear_row_button);
	DDX_Control(pDX, IDC_HOLIDAYCOMBX, m_holidayCombx);
}


BEGIN_MESSAGE_MAP(Weekly_Routines, CDialog)
	ON_EN_SETFOCUS(IDC_EDIT1, OnEnSetfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, OnEnKillfocusEdit1)
	ON_CBN_SETFOCUS(IDC_COMBO1, OnCbnSetfocusCombo1)
	ON_CBN_KILLFOCUS(IDC_COMBO1, OnCbnKillfocusCombo1)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLEAR, OnBnClickedClear)
	ON_WM_TIMER()
//	ON_COMMAND(ID_WEEKLY_TIMESET,OnWeeklySet)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_HOLIDAYCOMBX, &Weekly_Routines::OnCbnSelchangeHolidaycombx)
	ON_CBN_KILLFOCUS(IDC_HOLIDAYCOMBX, &Weekly_Routines::OnCbnKillfocusHolidaycombx)
END_MESSAGE_MAP()


// Weekly_Routines ��Ϣ�������
description Weekly_Routines::read_addr(unsigned char addr2)
{
	description temp_description;
	memset(&temp_description,0,sizeof(description));

	unsigned short temp_buffer[WR_DESCRIPTION_SIZE];
	memset(&temp_buffer,0,sizeof(temp_buffer));
	if (m_strproducttype.CompareNoCase(_T("Lightingcontroller")) == 0)
		 Read_Multi(254,temp_buffer,4608+addr2*WR_DESCRIPTION_SIZE,WR_DESCRIPTION_SIZE);
	else
		Read_Multi(g_tstat_id,temp_buffer,MODBUS_WR_DESCRIP_FIRST+addr2*WR_DESCRIPTION_SIZE,WR_DESCRIPTION_SIZE);

	int i=0;
	char *p;
	p=(char *)&temp_description;///////////////*****pointer   attention
	for(i=0;i<WR_DESCRIPTION_SIZE;i++)
		(*(p++))=(char)temp_buffer[i];
	return temp_description;
}
void Weekly_Routines::put_row_col(description temp,unsigned char t_addr)
{
	if(temp.flag==0xff && temp.holiday1==0xff && temp.holiday2==0xff)
	{
		for(int i=1;i<=GRID_COL_NUMBER;i++)
			m_FlexGrid.put_TextMatrix(t_addr,i,_T(""));
	}
	else
	{
		WCHAR wszlabel[21];  
		CString str;

		
	   MultiByteToWideChar( CP_ACP, 0, temp.full_label,
        strlen(temp.full_label)+1, wszlabel,   
		  sizeof(wszlabel)/sizeof(wszlabel[0]) );
		
		str.Format(_T("%s"),wszlabel);
		m_FlexGrid.put_TextMatrix(t_addr,1,str);

//		str.Format("%s",temp.label);
//		m_FlexGrid.put_TextMatrix(t_addr,8,str);

		if(temp.holiday1==0)
			str=HOLIDAY_ZERO;
		else
		{
			str.Format(_T("%d"),temp.holiday1);		
			str=(CString)HOLIDAY_HEAD+str;
		}
		m_FlexGrid.put_TextMatrix(t_addr,4,str);

		if(temp.holiday2==0)
			str=HOLIDAY_ZERO;
		else
		{
			str.Format(_T("%d"),temp.holiday2);		
			str=(CString)HOLIDAY_HEAD+str;
		}
		m_FlexGrid.put_TextMatrix(t_addr,6,str);

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

		if(temp.flag & 32)
			str=_T("ON");
		else
			str=_T("OFF");
        m_FlexGrid.put_TextMatrix(t_addr,5,str);

		if(temp.flag & 16)
			str=_T("ON");
		else
			str=_T("OFF");
        m_FlexGrid.put_TextMatrix(t_addr,7,str);
	}
}
void Weekly_Routines::clear_addr(unsigned char addr2)
{
	CString str_temp;
	description temp_descri;
	unsigned char *p;
	p=(unsigned char *) &temp_descri;
	for(int i=0;i<WR_DESCRIPTION_SIZE;i++)
		*(p++)=0xFF;
	p=(unsigned char *) &temp_descri;
	if(m_strproducttype.CompareNoCase(_T("Lightingcontroller")) == 0)
		Write_Multi(254,p,4608+(addr2-1)*WR_DESCRIPTION_SIZE,WR_DESCRIPTION_SIZE);
	else
		Write_Multi(g_tstat_id,p,MODBUS_WR_DESCRIP_FIRST+(addr2-1)*WR_DESCRIPTION_SIZE,WR_DESCRIPTION_SIZE);
}
void Weekly_Routines::write_addr(unsigned char addr2)
{
	CString str_temp;
	description temp_descri;
	memset(&temp_descri,0,sizeof(description));
	str_temp=m_FlexGrid.get_TextMatrix(addr2,1);//1
	str_temp.Trim();
	int i=0;
	for(i=1;i<=FULL_LABEL_NUMBER ;i++)
	{
		if(i<str_temp.GetLength()+1)
            temp_descri.full_label[i-1]=str_temp.GetAt(i-1);
		else
			temp_descri.full_label[i-1]=0;
	}
	temp_descri.full_label[FULL_LABEL_NUMBER-1]=0;
//	str_temp=m_FlexGrid.get_TextMatrix(addr2,8);//8
//	for(i=1;i<=LABEL_NUMBER;i++)
//	{
//		if(i<str_temp.GetLength()+1)
//            temp_descri.label[i-1]=str_temp.GetAt(i-1);
//		else
//			temp_descri.label[i-1]=0;
//	}
	str_temp=m_FlexGrid.get_TextMatrix(addr2,4);//4
	if(str_temp==HOLIDAY_ZERO)
		str_temp="0";
	else
		str_temp=str_temp.Right(str_temp.GetLength()-2);
	temp_descri.holiday1=(unsigned char)_wtoi(str_temp);
	str_temp=m_FlexGrid.get_TextMatrix(addr2,6);//6
	if(str_temp==HOLIDAY_ZERO)
		str_temp="0";
	else
		str_temp=str_temp.Right(str_temp.GetLength()-2);
	temp_descri.holiday2=(unsigned char)_wtoi(str_temp);
	temp_descri.flag=0;
	str_temp=m_FlexGrid.get_TextMatrix(addr2,7);//7
	if(str_temp.CompareNoCase(_T("ON"))==0)
		temp_descri.flag=temp_descri.flag | 16;
	str_temp=m_FlexGrid.get_TextMatrix(addr2,5);//7
	if(str_temp.CompareNoCase(_T("ON"))==0)
		temp_descri.flag=temp_descri.flag | 32;
	str_temp=m_FlexGrid.get_TextMatrix(addr2,3);//7
	if(str_temp.CompareNoCase(_T("ON"))==0)
		temp_descri.flag=temp_descri.flag | 64;
	str_temp=m_FlexGrid.get_TextMatrix(addr2,2);//7
	if(str_temp.CompareNoCase(_T("MAN"))==0)
		temp_descri.flag=temp_descri.flag | 128;
	unsigned char *p;
	p=(unsigned char *) &temp_descri;
	if(m_strproducttype.CompareNoCase(_T("Lightingcontroller")) == 0)
		Write_Multi(254,p,4608+(addr2-1)*WR_DESCRIPTION_SIZE,WR_DESCRIPTION_SIZE);
	else
		Write_Multi(g_tstat_id,p,MODBUS_WR_DESCRIP_FIRST+(addr2-1)*WR_DESCRIPTION_SIZE,WR_DESCRIPTION_SIZE);
	NET_WORK_SLEEP_BETWEEN_WRITE_READ
	temp_descri=read_addr(addr2-1);
	put_row_col(temp_descri,addr2);
}


void Weekly_Routines::load_grid()
{

	CString strInfo;
	strInfo.Format(_T("Weekly_Routines::load_grid()"));			
	SetPaneString(2, strInfo);

	m_FlexGrid.put_TextMatrix(0,0,_T("NO"));
	m_FlexGrid.put_TextMatrix(0,1,_T("Full_Label"));
	m_FlexGrid.put_TextMatrix(0,2,_T("A/M"));
	m_FlexGrid.put_TextMatrix(0,3,_T("Value"));
	m_FlexGrid.put_TextMatrix(0,4,_T("Holiday1"));
	m_FlexGrid.put_TextMatrix(0,5,_T("State1"));
	m_FlexGrid.put_TextMatrix(0,6,_T("Holiday2"));
	m_FlexGrid.put_TextMatrix(0,7,_T("State2"));

	CString temp;
	for(int i=1;i<21;i++)
	{
		temp.Format(_T("%d"),i);
		m_FlexGrid.put_TextMatrix(i,0,temp);
		m_FlexGrid.put_Row(i);
		m_FlexGrid.put_Col(5);
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
		m_FlexGrid.put_Col(7);
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
	}
	m_FlexGrid.put_ColWidth(0,400);
	//if(read_one(g_tstat_id,7)!=PM_NC)
	//	return ;
// 	if(read_one(g_tstat_id,7)!=PM_NC)
// 		return ;
	description temp_description;
	for(unsigned char i=1;i<=GRID_ROW_NUMBER;i++)
	{//////////////////////////////////////////////////get information from network
// 		CString strInfo;
// 		strInfo.Format(_T("load_grid():read_addr(i-1):%d"),i);			
// 		SetPaneString(2, strInfo);
		temp_description=read_addr(i-1);
// 		strInfo.Format(_T("load_grid():put_row_col:%d"),i);			
// 		SetPaneString(2, strInfo);
		put_row_col(temp_description,i);
	}
}
void Weekly_Routines::on_select()
{
	long lRow,lCol;
	lRow = m_FlexGrid.get_RowSel();//��ȡ������к�	
	lCol = m_FlexGrid.get_ColSel(); //��ȡ������к�
	UpdateData(false);
	if(lRow==0 || lCol==0)
		return;
	CRect rect;
	m_FlexGrid.GetWindowRect(rect); //��ȡ���ؼ��Ĵ��ھ���
	ScreenToClient(rect); //ת��Ϊ�ͻ�������	
	// MSFlexGrid�ؼ��ĺ����ĳ��ȵ�λ��"�(twips)"��
	//��Ҫ����ת��Ϊ���أ�1440�= 1Ӣ��
	CDC* pDC =GetDC();
	//�������ص��羵�ת������
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//����ѡ�и�����Ͻǵ�����(����Ϊ��λ)
	long y = m_FlexGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//����ѡ�и�ĳߴ�(����Ϊ��λ)����1��ʵ�ʵ����У����ּ�1��Ч������
	long width = m_FlexGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//�γ�ѡ�и����ڵľ�������
	CRect rc(x,y,x+width,y+height);
	//ת������ԶԻ��������
	rc.OffsetRect(rect.left+1,rect.top+1);
	//��ȡѡ�и���ı���Ϣ
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
		if(temp.CompareNoCase(_T("AUTO"))==0)
			return ;
	}
	if(row_col==5 || row_col==7)
		return;
	if(row_col==2)
	{
		m_combo_col_row.ResetContent();//clear
		m_combo_col_row.AddString(_T("AUTO"));
		m_combo_col_row.AddString(_T("MAN"));
		m_combo_col_row.BringWindowToTop();
		m_combo_col_row.ShowWindow(SW_SHOW);//��ʾ�ؼ�
		m_combo_col_row.MoveWindow(rc); //�ƶ���ѡ�и��λ�ã�����
		m_combo_col_row.SelectString(-1,strValue); //����ȫѡ������ֱ���޸�		
		m_combo_col_row.SetFocus(); //��ȡ����
	}
	else if(row_col==3)
	{
		m_combo_col_row.ResetContent();//clear
		m_combo_col_row.AddString(_T("OFF"));
		m_combo_col_row.AddString(_T("ON"));

		m_combo_col_row.BringWindowToTop();
		m_combo_col_row.ShowWindow(SW_SHOW);//��ʾ�ؼ�
		m_combo_col_row.MoveWindow(rc); //�ƶ���ѡ�и��λ�ã�����
		m_combo_col_row.SelectString(-1,strValue); //����ȫѡ������ֱ���޸�		
		m_combo_col_row.SetFocus(); //��ȡ����
	}
	else if(row_col==4||row_col==6)
	{
		m_holidayCombx.BringWindowToTop();
		m_holidayCombx.ShowWindow(SW_SHOW);
			
		m_holidayCombx.MoveWindow(rc); //�ƶ���ѡ�и��λ�ã�����
		m_holidayCombx.SetFocus(); //��ȡ����

	}
	else
	{
		m_Change.BringWindowToTop();
		m_Change.ShowWindow(SW_SHOW); //��ʾ�ؼ�
		m_Change.SetWindowText(strValue); //��ʾ�ı�
		m_Change.SetFocus(); //��ȡ����
		//m_Change.SetSel(0,-1); //ȫѡ
		m_Change.MoveWindow(rc); //�ƶ���ѡ�и��λ�ã�����
	}	
}

BOOL Weekly_Routines::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_holidayCombx.ShowWindow(SW_HIDE);
	 m_strInfo=_T("Double clicking row number cell or hit 'Insert' key to insert data.");
	 
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

// 	 CString strInfo;
// 	 strInfo.Format(_T("Weekly_Routines::OnInitDialog()"));			
// 	 SetPaneString(2, strInfo);


	load_grid();
	NET_WORK_SET_TIMER
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
BEGIN_EVENTSINK_MAP(Weekly_Routines, CDialog)
	ON_EVENT(Weekly_Routines, IDC_MSFLEXGRID1, DISPID_CLICK, ClickMsflexgrid1, VTS_NONE)
	ON_EVENT(Weekly_Routines, IDC_MSFLEXGRID1, DISPID_KEYDOWN, KeyDownMsflexgrid1, VTS_PI2 VTS_I2)
//	ON_EVENT(Weekly_Routines, IDC_MSFLEXGRID1, DISPID_MOUSEDOWN, MouseDownMsflexgrid1, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(Weekly_Routines, IDC_MSFLEXGRID1, DISPID_DBLCLICK, DblClickMsflexgrid1, VTS_NONE)
END_EVENTSINK_MAP()

void Weekly_Routines::ClickMsflexgrid1()
{
	// TODO: �ڴ˴������Ϣ����������
	on_select();		
}

void Weekly_Routines::OnEnSetfocusEdit1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NET_WORK_KILL_TIMER
	UpdateData(true);
	m_before_focus_string=m_sChange;
}

void Weekly_Routines::OnEnKillfocusEdit1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(true);
	/*
	if(row_col==4 || row_col==6)
	{
		if(m_sChange.GetLength()==1 || m_sChange.GetLength()==2)
		{			
			if(_wtoi(m_sChange)>20 || _wtoi(m_sChange)<0)
			{
				AfxMessageBox(_T("Wrong input!"));
				m_Change.SetFocus();
				return;
			}
			m_sChange=(CString)HOLIDAY_HEAD+m_sChange;
		}
		else 
			m_sChange.MakeUpper();
	}
	*/
	m_Change.GetWindowText(m_sChange);
	m_FlexGrid.put_TextMatrix(row_row,row_col,m_sChange);//�����ı���Ϣ
	m_Change.ShowWindow(SW_HIDE); //�����ı��ؼ�
	//UpdateData(false);
	if(m_before_focus_string!=m_sChange)
		write_addr(row_row);
	NET_WORK_SET_TIMER
}

void Weekly_Routines::OnCbnSetfocusCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NET_WORK_KILL_TIMER
	UpdateData(true);
	CString str_temp;
	m_combo_col_row.GetWindowText(str_temp);
	m_before_focus_string=str_temp;
}

void Weekly_Routines::OnCbnKillfocusCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strLabel=m_FlexGrid.get_TextMatrix(row_row,1);
	if(strLabel.IsEmpty())
		return;

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
		if(str_temp.CompareNoCase(_T("AUTO"))==0)
			m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
		else
			m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_WHITE_COLOR);
		m_FlexGrid.put_Col(2);
	}
	NET_WORK_SET_TIMER
}
void Weekly_Routines::tab_move()
{
	long lRow = m_FlexGrid.get_RowSel();//��ȡ������к�	
	long lCol = m_FlexGrid.get_ColSel(); //��ȡ������к�	
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

void Weekly_Routines::shift_tab_move()
{
	long lRow = m_FlexGrid.get_RowSel();//��ȡ������к�	
	long lCol = m_FlexGrid.get_ColSel(); //��ȡ������к�	
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

void Weekly_Routines::left_move()
{
	shift_tab_move();
}

void Weekly_Routines::right_move()
{
	tab_move();
}

void Weekly_Routines::up_move()
{
	long lRow = m_FlexGrid.get_RowSel();//��ȡ������к�
	long lCol = m_FlexGrid.get_ColSel(); //��ȡ������к�	
	if (lRow!=1)
		m_FlexGrid.put_Row(--lRow);
	GRID_UP_MOVE
	on_select();////////////////////////
}

void Weekly_Routines::down_move()
{
	long lRow =m_FlexGrid.get_RowSel();//��ȡ������к�	
	long lCol = m_FlexGrid.get_ColSel(); //��ȡ������к�	
	if (lRow!=m_FlexGrid.get_Rows()-1)
		m_FlexGrid.put_Row(++lRow);
	GRID_DOWN_MOVE
/*	if(lRow==m_FlexGrid.get_Rows()-1) 
	{ 
		int top_row=0; 
		top_row = m_FlexGrid.get_TopRow(); 
		m_FlexGrid.put_TopRow(top_row+1); 
	} 
	else if(!m_FlexGrid.get_RowIsVisible(lRow+1)) 
	{ 
		int top_row=0; 
		top_row = m_FlexGrid.get_TopRow(); 
		m_FlexGrid.put_TopRow(top_row+1); 
	}
*/
	on_select();////////////////////////
}

BOOL Weekly_Routines::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if(pMsg->message == WM_KEYDOWN  )
	{	
//#if 1	//�ĳ�˫��
		if(pMsg->wParam == VK_INSERT )
		{				
			EXIT_BUTTON_GET_FOCUS		//important,use this to settimer(1),so next��NET_WORK_KILL_TIMER��will be used
			Sleep(50);
			NET_WORK_KILL_TIMER
			CString strtmp= _T("LightingController");
			if(m_strproducttype.CompareNoCase(_T("LightingController")) == 0)
			{
				WeeklyRout_InsertDia dlg(row_row,strtmp);
				dlg.DoModal();
			}	
			else
			{
				WeeklyRout_InsertDia dlg(row_row);
				dlg.DoModal();
			}
			NET_WORK_SET_TIMER
			return true;
		}
//#endif
		if(pMsg->wParam==VK_F1)
		{
			long lRow = m_FlexGrid.get_RowSel();//��ȡ������к�	
			long lCol = m_FlexGrid.get_ColSel(); //��ȡ������к�
			if(lRow==0 || lCol==0)
				return true;
			row_row=lRow;
			row_col=lCol;
			WeeklyRout_InsertDia dlg(row_row);
					dlg.DoModal();
			NET_WORK_SET_TIMER
			return true;
		}

		if(pMsg->wParam==VK_F5)
		{
			load_grid();
		}

		if(pMsg->wParam == VK_ESCAPE )
			return true;//use esc kaypad can't destroy this dialog

		MESSAGE_DIRECTION_CONTROL  //         ^-^
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void Weekly_Routines::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
	NET_WORK_KILL_TIMER
}

void Weekly_Routines::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	OnOK();
}

void Weekly_Routines::OnBnClickedClear()
{
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
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_WHITE_COLOR);
		m_FlexGrid.put_Col(5);
		m_FlexGrid.put_TextMatrix(row_row,5,_T(""));
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
		m_FlexGrid.put_Col(6);
		m_FlexGrid.put_TextMatrix(row_row,6,_T(""));
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_WHITE_COLOR);
		m_FlexGrid.put_Col(7);
		m_FlexGrid.put_TextMatrix(row_row,7,_T(""));
		m_FlexGrid.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
	}
}
void Weekly_Routines::OnTimer(UINT nIDEvent)
{
	load_grid();
	CDialog::OnTimer(nIDEvent);
}

void Weekly_Routines::KeyDownMsflexgrid1(short* KeyCode, short Shift)
{
	// TODO: �ڴ˴������Ϣ����������
	//AfxMessageBox("OK");
}
void Weekly_Routines::OnWeeklySet()
{
	WeeklyRout_InsertDia dlg(row_row);
			dlg.DoModal();
}

void Weekly_Routines::DblClickMsflexgrid1()
{
	// TODO: �ڴ˴������Ϣ����������
#if 1
	long lRow = m_FlexGrid.get_RowSel();//��ȡ������к�	
	long lCol = m_FlexGrid.get_ColSel(); //��ȡ������к�
	if(lRow==0 || lCol==0)
		return ;
	row_row=lRow;
	row_col=lCol;


	WeeklyRout_InsertDia dlg(row_row);
			dlg.DoModal();
#endif
// 
// 	CWeeklySchedule dlg;
// 	dlg.DoModal();
}

void Weekly_Routines::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect outRc=CRect(6,15,650,40);
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(m_strInfo,&outRc,DT_WORDBREAK|DT_VCENTER|DT_CENTER);
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

void Weekly_Routines::OnCbnSelchangeHolidaycombx()
{
	// TODO: Add your control notification handler code here
}

void Weekly_Routines::OnCbnKillfocusHolidaycombx()
{
	/*
	if(row_col==4 || row_col==6)
	{
		if(m_sChange.GetLength()==1 || m_sChange.GetLength()==2)
		{			
			if(_wtoi(m_sChange)>20 || _wtoi(m_sChange)<0)
			{
				AfxMessageBox(_T("Wrong input!"));
				m_Change.SetFocus();
				return;
			}
			m_sChange=(CString)HOLIDAY_HEAD+m_sChange;
		}
		else 
			m_sChange.MakeUpper();
	}
	*/
	CString strLabel=m_FlexGrid.get_TextMatrix(row_row,1);
	if(strLabel.IsEmpty())
		return;

	m_holidayCombx.GetWindowText(m_sChange);
	m_FlexGrid.put_TextMatrix(row_row,row_col,m_sChange);//�����ı���Ϣ
	m_holidayCombx.ShowWindow(SW_HIDE); //�����ı��ؼ�
	write_addr(row_row);
	NET_WORK_SET_TIMER
}
