// LCOutNameConfig.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "LCOutNameConfig.h"
#include "afxdialogex.h"
#include "schedule.h"
#include "global_function.h"
 #include "../SQLiteDriver/CppSQLite3.h"

// CLCOutNameConfig dialog
 
IMPLEMENT_DYNAMIC(CLCOutNameConfig, CDialogEx)

CLCOutNameConfig::CLCOutNameConfig(int sn,CWnd* pParent)
	: CDialogEx(CLCOutNameConfig::IDD, pParent)
{
m_sn=sn;
}

CLCOutNameConfig::~CLCOutNameConfig()
{
}

void CLCOutNameConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_MSFLEXGRID, m_outputname);
	DDX_Control(pDX, IDC_EDIT1, m_editName);
}


BEGIN_MESSAGE_MAP(CLCOutNameConfig, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CLCOutNameConfig::OnEnKillfocusEdit1)
END_MESSAGE_MAP()


// CLCOutNameConfig message handlers

BOOL CLCOutNameConfig::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString outputname;
	//m_version=get_tstat_version(g_tstat_id);
	m_outputname.put_Cols(25);
	m_outputname.put_Rows(21);
	for (int col=1;col<=24;col++)
	{
	  outputname.Format(_T("Output%d"),col);
	  m_outputname.put_TextMatrix(0,col,outputname);
	}
	for (int row=1;row<=20;row++)
	{
	 outputname.Format(_T("Card%d"),row);
	 m_outputname.put_TextMatrix(row,0,outputname);
	}
 	to_fresh();
   // AfxBeginThread(_Read_LightingOutputName,this);
	return TRUE; 
	 // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLCOutNameConfig::to_fresh(){
	 
	/*if (LoadFromDB())
	{
	return;
	}*/
	 
	for (int i=0;i<20;i++)
	{
	     Read_FromOneCard(i);
	} 
	// LoadFromDB();
}
void CLCOutNameConfig::Read_FromOneCard(int Input_CardNo){
	CString temp;	
	struct_LCName structtemp;
	int i=0;
	unsigned short temp_buffer[INPUT_DESCRIPTION_LENGTH]={0};
	unsigned char temp_charbuffer[INPUT_DESCRIPTION_LENGTH];
	unsigned short temp_buffer_out[INPUT_DESCRIPTION_LENGTH]={0};
	unsigned char temp_charbuffer_out[INPUT_DESCRIPTION_LENGTH];
	int progress;
	int tt;
	 
	for (i=0;i<24;i++)
	{
		Read_Multi(g_tstat_id,temp_buffer_out,OUTPUT_DESCRIPTION+(Input_CardNo*24+i)*INPUT_DESCRIPTION_LENGTH,INPUT_DESCRIPTION_LENGTH);
		if(temp_buffer_out[0]==255)
		{
			for(int itt=0;itt<INPUT_DESCRIPTION_LENGTH;itt++)
				temp_buffer_out[itt]=0;
		}
		for(int itt=0;itt<INPUT_DESCRIPTION_LENGTH;itt++)
		{temp_charbuffer_out[itt] =(unsigned char) temp_buffer_out[itt];}
		temp=(CString)temp_charbuffer_out;
		//m_outputname.put_TextMatrix(Input_CardNo+1,i+1,temp);
		structtemp.card=Input_CardNo+1;
		structtemp.out=i+1;
		structtemp.name=temp;
		//m_vecLcName.push_back(structtemp);	 
		 //Insert_Update_OutputName(m_sn,Input_CardNo+1,i+1,temp); 
		 m_outputname.put_TextMatrix(Input_CardNo+1,i+1,temp);
	}
	 
		 
}

 

BEGIN_EVENTSINK_MAP(CLCOutNameConfig, CDialogEx)
	ON_EVENT(CLCOutNameConfig, IDC_INPUT_MSFLEXGRID, DISPID_CLICK, CLCOutNameConfig::ClickInputMsflexgrid, VTS_NONE)
	END_EVENTSINK_MAP()


void CLCOutNameConfig::ClickInputMsflexgrid()
{
    m_editName.ShowWindow(SW_HIDE);  
	long lCol=m_outputname.get_ColSel();         //获取点击的行号
	long lRow=m_outputname.get_RowSel();      //获取点击的列号
	m_curcol=lCol;
	m_currow=lRow;
	if(lRow>m_outputname.get_Rows() || lRow==0)              //判断点击是否有效
		return;

	CRect rect;
	m_outputname.GetWindowRect(&rect);                 //获取FlexGrid控件的窗口矩形
	ScreenToClient(&rect);                                    //转换为客户区矩形
	CDC* pDC=GetDC();
	//MSFlexGrid 控件的函数的长度单位是“缇(twips)”，需要将其转化为像素，1440 缇 = 1 英寸
	//计算象素点和缇的转换比例
	int nTwipsPerDotX=1440/pDC->GetDeviceCaps(LOGPIXELSX);
	int nTwipsPerDotY=1440/pDC->GetDeviceCaps(LOGPIXELSY);
	//计算选中格的左上角的坐标（象素为单位）
	long y = m_outputname.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_outputname.get_ColPos(lCol)/nTwipsPerDotX;

	int left=m_outputname.get_CellLeft()/nTwipsPerDotY-1;
	int top=m_outputname.get_CellTop()/nTwipsPerDotX-1;
	//long w= m_msflexgrid1to96.get_ColWidth(lCol);
	//long h= m_msflexgrid1to96.get_RowHeight(lRow);
	/*m_msflexgrid1to96.get_CellWidth()*/
	//计算选中格的尺寸（象素为单位）。加1是实际调试中，发现加1后效果更好
	long width = m_outputname.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_outputname.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left,rect.top);

	CString strValue=m_outputname.get_TextMatrix(lRow,lCol);       //获取单元格内容
	m_stroldername=strValue;
	m_editName.ShowWindow(SW_SHOW);                  //显示控件
	m_editName.MoveWindow(rc);                            //改变大小并移到选中格位置
	m_editName.SetWindowText(strValue);                   //显示文本
	m_editName.SetFocus();   
}


void CLCOutNameConfig::OnEnKillfocusEdit1()
{
	CString strvalue;
	int j=m_currow-1;
	int i=m_curcol-1;
	m_editName.GetWindowText(strvalue);
	if (m_stroldername.CompareNoCase(strvalue)!=0)
	{
		unsigned char out_lable_buffer[GROUP_LABLE_SIZE]={0};
		for (int k=0;k<strvalue.GetLength();k++)
		{
			if (k>=10)
				break;
			out_lable_buffer[k]=(char)strvalue.GetAt(k);
		}
		Write_Multi(g_tstat_id,out_lable_buffer,OUTPUT_DESCRIPTION+(j*20+i)*INPUT_DESCRIPTION_LENGTH,INPUT_DESCRIPTION_LENGTH);
		//Insert_Update_OutputName(m_sn,m_currow,m_curcol,strvalue); 
		m_outputname.put_TextMatrix(m_currow,m_curcol,strvalue);
	}
	
}

BOOL CLCOutNameConfig::LoadFromDB(){
BOOL ret=FALSE;
 
CppSQLite3DB SqliteDBBuilding;
CppSQLite3Table table;
CppSQLite3Query q;
SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
if (SqliteDBBuilding.tableExists("LCNameConfigure"))
{
	CString sql,temp;
	struct_LCName tempstruct;
	sql.Format(_T("Select * from LCNameConfigure where SN=%d"),m_sn);
	q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
 
	while (!q.eof())//有表但是没有对应序列号的值
	{
	  tempstruct.name=q.getValuebyName(_T("OutputName"));
	  temp=q.getValuebyName(_T("Card"));
	  tempstruct.card=_wtoi(temp);
	  temp=q.getValuebyName(_T("Output"));
	  tempstruct.out=_wtoi(temp);
	  m_vecLcName.push_back(tempstruct);
	  q.nextRow();
	  ret=TRUE;
	}
} 
SqliteDBBuilding.closedb();
if(ret)
{
	CString name;
	for (int i=0;i<20;i++)
	{
		for (int j=0;j<24;j++)
		{
			name=get_OutputName(i+1,j+1);
			m_outputname.put_TextMatrix(i+1,j+1,name);
		}
	}
}
return ret;
}

CString CLCOutNameConfig::get_OutputName(int card,int output){
	 
	CString Table_Name(_T(""));
	vector<struct_LCName>::iterator lc_iterator;
	for (lc_iterator=m_vecLcName.begin();lc_iterator!=m_vecLcName.end();lc_iterator++)
	{
	  if ((lc_iterator->card==card)&&(lc_iterator->out==output))
	  {
	     Table_Name= lc_iterator->name;
		 break;
	  }
	}
	return Table_Name;
}
void CLCOutNameConfig::Insert_Update_OutputName(int sn,int card,int output ,CString outputname){
	 
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

	CString sql;
	sql.Format(_T("Select * from LCNameConfigure where SN=%d and  Card=%d and Output=%d"),sn,card,output);
	q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);

	if (!q.eof())//有表但是没有对应序列号的值
	{
		sql.Format(_T("update LCNameConfigure set OutputName = '%s' where SN=%d and  Card=%d and Output=%d "),outputname.GetBuffer(),sn,card,output);
		 SqliteDBBuilding.execDML((UTF8MBSTR)sql);
	}
	else
	{
		 
		sql.Format(_T("Insert into LCNameConfigure(SN,Card,Output,OutputName) values(%d,%d,%d,'%s')"),sn,card,output,outputname.GetBuffer());
		SqliteDBBuilding.execDML((UTF8MBSTR)sql);
	}

	SqliteDBBuilding.closedb();
}


void CLCOutNameConfig::Update_IntoDB(){

}