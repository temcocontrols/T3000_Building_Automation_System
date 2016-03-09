// DialogCM5.cpp : implementation file
//
///test
#include "stdafx.h"
#include "../T3000.h"
#include "DialogCM5.h"
#include <cmath>
#define  WM_ENABLE_REFRESHBUTTON WM_USER+200

const int REFRESH_GRID=1;
const int ENABLE_REFRESH_BUTTON=2;

UINT _ReadMulti_CM5Registers(LPVOID pParam){
	CDialogCM5 *pParent = (CDialogCM5 *)pParam;
	int i;
	register_critical_section.Lock();
	pParent->b_is_fresh = true;
	for(i=0;i<80;i++)
	{	
		Read_Multi(g_tstat_id,&multi_register_value_tcp[i*100],i*100,100);	
	}
	register_critical_section.Unlock();
	pParent->ShowDialogData();
	//memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
	pParent->PostMessage(WM_ENABLE_REFRESHBUTTON,ENABLE_REFRESH_BUTTON,0);
	 
	//pParent->Fresh_Grid();
	pParent->CM5_OutputThread=NULL;
	pParent->b_is_fresh = false;
	return 0;
}
//#ifndef _CM5DB
#define  THE_CHAR_LENGTH 8
int timer = 3;
static BOOL  ifcreatedb = TRUE,initparam = TRUE; 
CString AI_Range[6]={_T("UNUSED"),_T("10KF Therm"),_T("0-100%"),_T("On/Off"),_T("Custom Sensor"),_T("Off/On")};
IMPLEMENT_DYNCREATE(CDialogCM5, CFormView)

CDialogCM5::CDialogCM5()
	: CFormView(CDialogCM5::IDD)
	, m_IDaddressCM5(0)
	, m_nSerialNumCM5(0)
	, m_firmwareVersionCM5(0)
	, m_hardware_versionCM5(0)
	, m_strDateCM5(_T(""))
	, m_strTime(_T(""))
	, m_heating(_T(""))
	, m_cooling(_T(""))
	, m_coating(_T(""))
	, m_result(_T(""))
	, m_priorityinterval(0)
	, m_countprioritytime(_T(""))
	, m_static_datetime(_T(""))
	, m_combo2_cstring(_T(""))

	,m_cellrow(1)
	,m_cellcol(1)
	,m_cellcolout(1)
	,m_cellrowout(1)
	,num(0)		
	,m_switch(0)

	, m_bootloaderversion(_T(""))
	, m_nListenPort(_T(""))
	, m_Display_Num(0)
	, m_Interval(0)
	, m_dts1(0)
{
b_is_fresh=FALSE;
CM5_OutputThread=NULL;
}

CDialogCM5::~CDialogCM5()
{
}

void CDialogCM5::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_IDaddressCM5);
	DDX_Text(pDX, IDC_EDIT_SERIAL, m_nSerialNumCM5);
	DDX_Text(pDX, IDC_EDIT_FIRMWARE, m_firmwareVersionCM5);
	DDX_Text(pDX, IDC_EDIT_HARDWARE, m_hardware_versionCM5);
	DDX_Text(pDX, IDC_EDIT_DATE, m_strDateCM5);
	DDX_Text(pDX, IDC_EDIT_TIME, m_strTime);
	DDX_Text(pDX, IDC_EDIT_HEAT, m_heating);
	DDX_Text(pDX, IDC_EDIT_COOL, m_cooling);
	DDX_Text(pDX, IDC_EDIT_COAST, m_coating);
	DDX_Text(pDX, IDC_EDIT_RESULT, m_result);
	DDX_Control(pDX, IDC_COMBO_MODE, m_combo_prioritymode);
	DDX_Text(pDX, IDC_EDIT_DURATION, m_priorityinterval);
	DDX_Text(pDX, IDC_EDIT_TIMELEFT, m_countprioritytime);
	DDX_Control(pDX, IDC_ADDBUILDING_MSFLEXGRID, m_Output_GridCM5_);
	DDX_Control(pDX, IDC_INPUT_MSFLEXGRID, m_msflexgrid_input);
	DDX_Control(pDX, IDC_OUTPUT_MSFLEXGRID, m_msflexgridoutput);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_edit_input);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_edit_output);
	DDX_Control(pDX, IDC_COMBO_INPUTOUTPUT, m_combobox2_input);
	DDX_Text(pDX, IDC_STATIC_DATETIME, m_static_datetime);
	DDX_CBString(pDX, IDC_COMBO_INPUTOUTPUT, m_combo2_cstring);
	DDX_Text(pDX, IDC_EDIT_BLDVERSION, m_bootloaderversion);
	DDX_Control(pDX, IDC_BAUDRATE_COMBX, m_Baudrate);
	DDX_Control(pDX, IDC_IPMODEL_COMBO, m_ipModelComBox);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ip_addressCtrl);
	DDX_Control(pDX, IDC_IPADDRESS2, m_subnet_addressCtrl);
	DDX_Control(pDX, IDC_IPADDRESS3, m_gateway_addressCtrl);
	DDX_Text(pDX, IDC_EDIT8, m_nListenPort);
	DDX_Control(pDX, IDC_MAC_ADDRESS, m_Mac_Address);
	DDX_Control(pDX, IDC_CHECK_DEMO, m_checkdemo);
	DDX_Control(pDX, IDC_COMBO_TYPE_INOUTPUT, m_combox_enable);
	DDX_Text(pDX, IDC_NUM, m_Display_Num);
	DDV_MinMaxInt(pDX, m_Display_Num, 0, 10);
	DDX_Text(pDX, IDC_INTERVAL, m_Interval);
	//DDX_Text(pDX, IDC_DTS1, m_dts1);
	DDX_Text(pDX, IDC_DTS1, m_dts1);
	DDX_Text(pDX, IDC_DTS1, m_dts2);
	DDX_Text(pDX, IDC_DTS1, m_dts3);
	DDX_Text(pDX, IDC_DTS1, m_dts4);
	DDX_Text(pDX, IDC_DTS1, m_dts5);
	DDX_Text(pDX, IDC_DTS1, m_dts6);
	DDX_Text(pDX, IDC_DTS1, m_dts7);
	DDX_Text(pDX, IDC_DTS1, m_dts8);
	DDX_Text(pDX, IDC_DTS1, m_dts9);
	DDX_Text(pDX, IDC_DTS1, m_dts10);
	DDX_Control(pDX, IDC_PROGRESS, m_progressctrl);
	DDX_Control(pDX, IDC_SUBTSTAT_INPUT, m_subtstatgrid);
	DDX_Control(pDX, IDC_AI_Grid, m_DIGrid);
}

BEGIN_MESSAGE_MAP(CDialogCM5, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogCM5::OnBnClickedButtonSyncwithpc)
	ON_BN_CLICKED(IDC_SAVE_CONFIG, &CDialogCM5::OnBnClickedButtonSaveConfig)
	ON_BN_CLICKED(IDC_BUTTON2, &CDialogCM5::OnBnClickedButtonweeklyschedule)
	ON_BN_CLICKED(IDC_REFRESH_ALL, &CDialogCM5::OnBnClickedButtonRefreshAll)
	ON_BN_CLICKED(IDC_BUTTON3, &CDialogCM5::OnBnClickedButtonannualschedule)
	ON_BN_CLICKED(IDC_BUTTON4, &CDialogCM5::OnBnClickedButtonidschedule)
	ON_BN_CLICKED(IDC_BUTTON5, &CDialogCM5::OnBnClickedButtoninputtype)
	ON_BN_CLICKED(IDC_CHECK_DEMO, &CDialogCM5::OnBnClickedButtonCheckDemo)
	ON_EN_KILLFOCUS(IDC_EDIT_INPUT, &CDialogCM5::OnEnKillfocusEditInput)
	ON_EN_KILLFOCUS(IDC_EDIT_OUTPUT, &CDialogCM5::OnEnKillfocusEditoutput)
	//------
	ON_EN_KILLFOCUS(IDC_NUM, &CDialogCM5::OnEnKillfocusEditNum)
	ON_EN_KILLFOCUS(IDC_INTERVAL, &CDialogCM5::OnEnKillfocusEditInterval)
	ON_EN_KILLFOCUS(IDC_DTS1, &CDialogCM5::OnEnKillfocusEditDTS1)
	ON_EN_KILLFOCUS(IDC_DTS2, &CDialogCM5::OnEnKillfocusEditDTS2)
	ON_EN_KILLFOCUS(IDC_DTS3, &CDialogCM5::OnEnKillfocusEditDTS3)
	ON_EN_KILLFOCUS(IDC_DTS4, &CDialogCM5::OnEnKillfocusEditDTS4)
	ON_EN_KILLFOCUS(IDC_DTS5, &CDialogCM5::OnEnKillfocusEditDTS5)
	ON_EN_KILLFOCUS(IDC_DTS6, &CDialogCM5::OnEnKillfocusEditDTS6)
	ON_EN_KILLFOCUS(IDC_DTS7, &CDialogCM5::OnEnKillfocusEditDTS7)
	ON_EN_KILLFOCUS(IDC_DTS8, &CDialogCM5::OnEnKillfocusEditDTS8)
	ON_EN_KILLFOCUS(IDC_DTS9, &CDialogCM5::OnEnKillfocusEditDTS9)
	ON_EN_KILLFOCUS(IDC_DTS10, &CDialogCM5::OnEnKillfocusEditDTS10)
	//------
	ON_WM_CLOSE()
	ON_WM_TIMER()

	ON_CBN_SELCHANGE(IDC_COMBO_INPUTOUTPUT, &CDialogCM5::OnCbnSelchangeCombx)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_INOUTPUT, &CDialogCM5::OnCbnSelchangeType)

	ON_MESSAGE(WM_ENABLE_REFRESHBUTTON, &CDialogCM5::DealWithMessage)
END_MESSAGE_MAP()


// CDialogCM5 diagnostics

#ifdef _DEBUG
void CDialogCM5::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDialogCM5::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}


#endif
#endif //_DEBUG
void CDialogCM5::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if(g_bPauseMultiRead)
		g_bPauseMultiRead=FALSE;
	//CDialog::OnClose();

}
LRESULT  CDialogCM5::DealWithMessage(WPARAM wParam,LPARAM lParam)
{
	int command = wParam;
	switch(command)
	{
	case ENABLE_REFRESH_BUTTON:
		GetDlgItem(IDC_REFRESH_ALL)->EnableWindow(TRUE);
		break;
	default:
		break;
	}

	return 0;
}
void CDialogCM5::Fresh()
{

/*
Comment by:Alex
Date:2013/04/10
Purpose:
重写 if 0 掉
*/
SetTimer(DATATIME_TIMER,1000,NULL);

 
	if(!g_bPauseMultiRead)
		g_bPauseMultiRead=TRUE;
ShowDialogData();
UpdateData(FALSE);
}
void CDialogCM5::OnBnClickedButtonSaveConfig()
{

}

//Write_Multi(g_tstat_id,p,7050,504);
// CDialogCM5 message handlers
void CDialogCM5::OnCbnSelchangeCombx(){
BeginWaitCursor();
CString strTemp;
int lRow = m_DIGrid.get_RowSel();	
int lCol = m_DIGrid.get_ColSel();
//if ((strText.GetLength()<=0)||strText.CompareNoCase(m_tempGridString)==0)
//{
//	return;
//}
int sel=m_combobox2_input.GetCurSel();
		m_combobox2_input.ShowWindow(FALSE);
int ret=write_one(g_tstat_id,143+lRow,sel);
if (ret>0)
{
	strTemp=AI_Range[sel];
	multi_register_value_tcp[143+lRow]=sel;
	//m_DIGrid.put_TextMatrix(i,4,strTemp);
} 
else
{
	strTemp=AI_Range[multi_register_value_tcp[143+lRow]];
	//m_DIGrid.put_TextMatrix(i,4,strTemp);
}
   m_DIGrid.put_TextMatrix(lRow,lCol,strTemp);
EndWaitCursor();
}
void CDialogCM5::OnCbnSelchangeType(){
	BeginWaitCursor();
	CString strTemp;
	int lRow = m_subtstatgrid.get_RowSel();	
	int lCol = m_subtstatgrid.get_ColSel();
	//if ((strText.GetLength()<=0)||strText.CompareNoCase(m_tempGridString)==0)
	//{
	//	return;
	//}
	int sel=m_combox_enable.GetCurSel();
	m_combox_enable.ShowWindow(FALSE);
	int ret=write_one(g_tstat_id,173+lRow,sel);
	if (ret>0)
	{
		if (sel==0)
		{
		   strTemp=_T("UNUSED");
		}
		else if (sel==1)
		{
		   strTemp=_T("Normal");
		}
		else
		{

		strTemp=_T("Switch");
		}
		multi_register_value_tcp[173+lRow]=sel;
		//m_DIGrid.put_TextMatrix(i,4,strTemp);
	} 
	else
	{
		 
		//m_DIGrid.put_TextMatrix(i,4,strTemp);
		if (multi_register_value_tcp[173+lRow]==0)
		{
			strTemp=_T("UNUSED");
		}
		else if (multi_register_value_tcp[173+lRow]==1)
		{
			strTemp=_T("Normal");
		}
		else
		{

			strTemp=_T("Switch");
		}
	}
	m_subtstatgrid.put_TextMatrix(lRow,lCol,strTemp);


	ShowDialogData();
	EndWaitCursor();
}
void CDialogCM5::OnBnClickedButtonSyncwithpc()
{
	// 		200	1	second
	// 		201	1	minute
	// 		202	1	hour
	// 		203	1	day
	// 		204	1	week, 0=Sunday; 1=Monday; and so on.
	// 		205	1	month
	// 		206	1	lower byte of the year, for example, here should be 98 in year 1998
	// 		207	1	higher byte of the year, for example, here should be 19 in the year 1998

	KillTimer(DATATIME_TIMER);
	//179 180 181 182
	CString tempyear,tempmonth,tempday,temptime,tempminute;
	CTime now;
	now = CTime::GetCurrentTime();
	tempyear = now.Format(_T("%Y"));
	tempmonth = now.Format(_T("%m"));
	tempday   = now.Format(_T("%d"));
	temptime  = now.Format(_T("%H"));
	tempminute= now.Format(_T("%M"));

	int year,month,day,time,minute;

	year  = atoi((char*)LPCTSTR(tempyear));
	month = atoi((char*)LPCTSTR(tempmonth));
	day   = atoi((char*)LPCTSTR(tempday));
	time  = atoi((char*)LPCTSTR(temptime));
	minute = atoi((char*)LPCTSTR(tempminute));


	year = _ttoi(tempyear);
	month = _ttoi(tempmonth);
	day   = _ttoi(tempday);
	time  = _ttoi(temptime);
	minute = _ttoi(tempminute);

	int yearh,yearl;
	yearl = year%100;
	int tempy = year&0xFF00;
	tempy = tempy>>16;
	yearh = tempy;

	//write_one(g_tstat_id,201,20);
	write_one(g_tstat_id,201,yearl);
	write_one(g_tstat_id,202,month);
	write_one(g_tstat_id,204,day);

	write_one(g_tstat_id,205,time);
	write_one(g_tstat_id,206,minute);
	 
	SetTimer(DATATIME_TIMER,1000,NULL);
}

void CDialogCM5::OnBnClickedButtonweeklyschedule()
{
	KillTimer(DATATIME_TIMER);
	if(g_NetWorkLevel==1)
		return;

	g_bPauseMultiRead=TRUE;
	Weekly_Routines dlg;
	dlg.DoModal();

	g_bPauseMultiRead=FALSE;
	SetTimer(DATATIME_TIMER,1000,NULL);


}
void CDialogCM5::OnBnClickedButtonRefreshAll(){
	GetDlgItem(IDC_REFRESH_ALL)->EnableWindow(FALSE);

	if(CM5_OutputThread==NULL)
		CM5_OutputThread =AfxBeginThread(_ReadMulti_CM5Registers,this);
}
void CDialogCM5::OnBnClickedButtonannualschedule()
{
	KillTimer(DATATIME_TIMER);
	if(g_NetWorkLevel==1)
		return;
	g_bPauseMultiRead=TRUE;
	Annual_Routines dlg;
	dlg.DoModal();
	g_bPauseMultiRead=FALSE;
	SetTimer(DATATIME_TIMER,1000,NULL);


}

void CDialogCM5::OnBnClickedButtonidschedule()
{
	KillTimer(DATATIME_TIMER);
	if(g_NetWorkLevel==1)
		return;

	g_bPauseMultiRead=TRUE;
	CConfig_Routines dlg;
	dlg.DoModal();
	g_bPauseMultiRead=FALSE;
	SetTimer(DATATIME_TIMER,1000,NULL);
}

void CDialogCM5::OnBnClickedButtoninputtype()
{
	 
}
void CDialogCM5::OnBnClickedButtonCheckDemo()
{
   if (m_checkdemo.GetCheck()!=0)
   {
     write_one(g_tstat_id,31,1);
   }
   else
   {
     write_one(g_tstat_id,31,0);
   }
}
int  CDialogCM5::Get_RegID(int lRow)
{  int ID;
if (m_type==0)//output  rows==10
{
	ID=7050+(lRow-1)*7;
	//7120+(lRow-1)*7;
} 
else if (m_type==1)//rows==8  sub tstat
{
	ID=7120+(lRow-1)*7;
}
else if (m_type==2)//rows==8//
{
	ID=7176+(lRow-1)*7;
}
else if (m_type==3)//rows==10
{
	ID=7232+(lRow-1)*7;
}
return ID;
}
//获取一个第八位数据，s:system:进制，n;num:这个数是几位的，第N位是1还是0
//n>=N
BOOL CDialogCM5::Get_Data_Bit(UINT Data,int n,int N)
{
  //Data=Data&0FFx;//去掉高八位数据
  //下面是算幂 2的N
  //最大的num位数的数是？ 2~num-1
 // AfxMessageBox(_T("OK"));
  UINT num=1;
  for (int i=1;i<=n;i++)
  {
    num=num*2;
  }
  num=num-1;//最大的num位数
  Data=Data&num;//取得这个n位数是什么？
  num=1;
  for (int i=1;i<N;i++)
  {
	  num=num*2;
  }
  if (num==(Data&num))//说明是1 
  {
   return TRUE;
  } 
  else
  {
   return FALSE;
  }
}
CString CDialogCM5::GetTextReg(unsigned short reg,BOOL Is_Read){
	CString str_temp;
	unsigned short temp_buffer[7];
	unsigned short temp_buffer_Char[THE_INPUT_OUTPUT_LENGTH];
	unsigned char p[THE_INPUT_OUTPUT_LENGTH+1]={'\0'};
	
	if (Is_Read)
	{
		 Read_Multi(g_tstat_id,temp_buffer,reg,7);
	} 
	else
	{
	   for (int i=0;i<7;i++)
	   {
	     temp_buffer[i]=multi_register_value_tcp[reg+i];
	   }
	
	}

	unsigned short Hi_Char,Low_Char;

	for (int i=0;i<7;i++)
	{
		Hi_Char=temp_buffer[i];
		Hi_Char=Hi_Char&0xff00;
		Hi_Char=Hi_Char>>8;
		Low_Char=temp_buffer[i];
		Low_Char=Low_Char&0x00ff;
		temp_buffer_Char[2*i]=Hi_Char;
		temp_buffer_Char[2*i+1]=Low_Char;
	} 

	for (int i=0;i<THE_INPUT_OUTPUT_LENGTH;i++)
	{
		p[i] =(unsigned char)temp_buffer_Char[i];

	}

	str_temp.Format(_T("%c%c%c%c%c%c%c%c%c%c%c%c%c%c"),p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],p[10],p[11],p[12],p[13]);
	return str_temp;
}
void CDialogCM5::OnEnKillfocusEditInput()
{

		//m_msflexgrid_input.put_TextMatrix(lRow,lCol,strText);
	
	/*CommentBy Alex
	2013/04/12
	Purpose:
	写入寄存器中
	步骤：
	1.判断输入的字符数是不是大于14个 有七个寄存器
	2.使用多写写入对应的寄存器中
	*/
	
	CString strText;
	m_edit_input.GetWindowText(strText); 
	m_edit_input.ShowWindow(SW_HIDE); 
 
	

	if (m_type==2)
	{
	
	int lRow = m_msflexgrid_input.get_RowSel();	
	int lCol = m_msflexgrid_input.get_ColSel(); 

 
	if ((strText.GetLength()<=0)||strText.CompareNoCase(m_tempGridString)==0)
	{
		return;
	}
	if (strText.GetLength()>THE_INPUT_OUTPUT_LENGTH)
	{
	  AfxMessageBox(_T("The chars>14"));
	  return;
	}
	unsigned char p[14];
	
	 WideCharToMultiByte( CP_ACP, 0, strText.GetBuffer(), -1,(LPSTR)p, 256, NULL, NULL );
	  //if null ,replace by ' '.
	 for(int i=strText.GetLength();i<THE_INPUT_OUTPUT_LENGTH;i++)
	{
	 
			p[i]=' ';
	} 
	//table 3,4
	if (Write_Multi(g_tstat_id,p,Get_RegID(lRow),THE_INPUT_OUTPUT_LENGTH)>0)
	{
		 /* if (m_type==2)
		  {
			  m_msflexgrid_input.put_TextMatrix(lRow,lCol,strText);
		  } 
		  else
		  {
			  m_DIGrid.put_TextMatrix(lRow,lCol,strText);
		  }*/
		  
	} 
	else
	{  
		/*if (m_type==2)
		{
			m_msflexgrid_input.put_TextMatrix(lRow,lCol,strText);
		} 
		else
		{
			m_DIGrid.put_TextMatrix(lRow,lCol,strText);
		}
	      m_msflexgrid_input.put_TextMatrix(lRow,lCol,_T(""));*/
		strText=_T("");
	   AfxMessageBox(_T("Try again!"));
	}
		m_msflexgrid_input.put_TextMatrix(lRow,lCol,strText);
	} 
	else//m_type==3
	{
	 int lRow = m_DIGrid.get_RowSel();	
	 int lCol = m_DIGrid.get_ColSel(); 

 		//一般的空的或者和输入之前相等的返回
	if ((strText.GetLength()<=0)||strText.CompareNoCase(m_tempGridString)==0)
	{
		return;
	}	
	 
     if (lCol==1)
     {
		if (strText.GetLength()>THE_INPUT_OUTPUT_LENGTH)
	{
		AfxMessageBox(_T("The chars>14"));
		return;
	}
	     unsigned char p[14];
	//if null ,replace by ' '.
	/*    for(int i=0;i<THE_INPUT_OUTPUT_LENGTH;i++)
	{
		if(i<strText.GetLength())
			p[i]=strText.GetAt(i);
		else
			p[i]=' ';
	}*/

		 WideCharToMultiByte( CP_ACP, 0, strText.GetBuffer(), -1, (LPSTR)p, 256, NULL, NULL );
		 //if null ,replace by ' '.
		 for(int i=strText.GetLength();i<THE_INPUT_OUTPUT_LENGTH;i++)
		 {

			 p[i]=' ';
		 } 
	//table 3,4

	    if (Write_Multi(g_tstat_id,p,Get_RegID(lRow),THE_INPUT_OUTPUT_LENGTH)>0)
	{
			/* if (m_type==2)
			{
				m_msflexgrid_input.put_TextMatrix(lRow,lCol,strText);
			} 
			else
			{
				m_DIGrid.put_TextMatrix(lRow,lCol,strText);
			}*/
		  
	} 
	    else
	{  
		/*if (m_type==2)
		{
			m_msflexgrid_input.put_TextMatrix(lRow,lCol,strText);
		} 
		else
		{
			m_DIGrid.put_TextMatrix(lRow,lCol,strText);
		}
			m_msflexgrid_input.put_TextMatrix(lRow,lCol,_T(""));*/
		strText=_T("");
		AfxMessageBox(_T("Try again!"));
	}
    }
	 else if (lCol==2)	 //134~143
	 {
	   	 int Value=_wtoi(strText);
		 int ret=write_one(g_tstat_id,133+lRow,Value);
		 if (ret>0)
		 {
		   multi_register_value_tcp[133+lRow]=Value;
			//m_DIGrid.put_TextMatrix(lRow,lCol,strText);
			 
		 }
		 else
		 {
		   	strText=_T("0");
		 }
	 }
	 else if (lCol==5)
	 {
		 int Value=_wtoi(strText);
		 int ret=write_one(g_tstat_id,153+lRow,Value);
		 if (ret>0)
		 {
			 multi_register_value_tcp[153+lRow]=Value;
			 //m_DIGrid.put_TextMatrix(lRow,lCol,strText);

		 }
		 else
		 {
			 strText=_T("0");
		 }
	 }
	 else if (lCol==6)
	 {
		 int Value=_wtoi(strText);
		 int ret=write_one(g_tstat_id,163+lRow,Value);
		 if (ret>0)
		 {
			 multi_register_value_tcp[163+lRow]=Value;
			 //m_DIGrid.put_TextMatrix(lRow,lCol,strText);

		 }
		 else
		 {
			 strText=_T("0");
		 }
	 }
		
		
		m_DIGrid.put_TextMatrix(lRow,lCol,strText);	 
	}


	CRect rcCell(0,0,1,1);
	m_edit_input.MoveWindow(&rcCell,1);
}
/////////////////////////////////////////////
void CDialogCM5::Show_DisplayEdit(int num)
{
  switch (num)
  {
  case 0:
  {
  	GetDlgItem(IDC_DTS1)->ShowWindow(FALSE);
	GetDlgItem(IDC_DTS2)->ShowWindow(FALSE);
	GetDlgItem(IDC_DTS3)->ShowWindow(FALSE);
	GetDlgItem(IDC_DTS4)->ShowWindow(FALSE);
	GetDlgItem(IDC_DTS5)->ShowWindow(FALSE);
	GetDlgItem(IDC_DTS6)->ShowWindow(FALSE);
	GetDlgItem(IDC_DTS7)->ShowWindow(FALSE);
	GetDlgItem(IDC_DTS8)->ShowWindow(FALSE);
	GetDlgItem(IDC_DTS9)->ShowWindow(FALSE);
	GetDlgItem(IDC_DTS10)->ShowWindow(FALSE);
  }
  break;
  case 1:
  {
	  GetDlgItem(IDC_DTS1)->ShowWindow(TRUE);
	  GetDlgItem(IDC_DTS2)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS3)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS4)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS5)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS6)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS7)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS8)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS9)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS10)->ShowWindow(FALSE);
  }
	  break;
  case 2:
  {
	  GetDlgItem(IDC_DTS1)->ShowWindow(TRUE);
	  GetDlgItem(IDC_DTS2)->ShowWindow(TRUE);
	  GetDlgItem(IDC_DTS3)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS4)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS5)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS6)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS7)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS8)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS9)->ShowWindow(FALSE);
	  GetDlgItem(IDC_DTS10)->ShowWindow(FALSE);
  }
	  break;
  case 3:
	  {
		  GetDlgItem(IDC_DTS1)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS2)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS3)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS4)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS5)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS6)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS7)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS8)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS9)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS10)->ShowWindow(FALSE);
	  }
	  break;
  case 4:
	  {
		  GetDlgItem(IDC_DTS1)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS2)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS3)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS4)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS5)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS6)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS7)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS8)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS9)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS10)->ShowWindow(FALSE);
	  }
	  break;
  case 5:
	  {
		  GetDlgItem(IDC_DTS1)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS2)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS3)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS4)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS5)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS6)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS7)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS8)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS9)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS10)->ShowWindow(FALSE);
	  }
	  break;
  case 6:
	  {
		  GetDlgItem(IDC_DTS1)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS2)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS3)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS4)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS5)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS6)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS7)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS8)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS9)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS10)->ShowWindow(FALSE);
	  }
	  break;
  case 7:
	  {
		  GetDlgItem(IDC_DTS1)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS2)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS3)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS4)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS5)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS6)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS7)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS8)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS9)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS10)->ShowWindow(FALSE);
	  }
	  break;
  case 8:
	  {
		  GetDlgItem(IDC_DTS1)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS2)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS3)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS4)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS5)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS6)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS7)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS8)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS9)->ShowWindow(FALSE);
		  GetDlgItem(IDC_DTS10)->ShowWindow(FALSE);
	  }
	  break;
  case 9:
	  {
		  GetDlgItem(IDC_DTS1)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS2)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS3)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS4)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS5)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS6)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS7)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS8)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS9)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS10)->ShowWindow(FALSE);
	  }
	  break;
  case 10:
	  {
		  GetDlgItem(IDC_DTS1)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS2)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS3)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS4)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS5)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS6)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS7)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS8)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS9)->ShowWindow(TRUE);
		  GetDlgItem(IDC_DTS10)->ShowWindow(TRUE);
	  }
	  break;
  
  }
}
void CDialogCM5::OnEnKillfocusEditNum()
{
   UpdateData(TRUE);
   int ret=write_one(g_tstat_id,7302,m_Display_Num);
   if (ret>0)
   {
    Show_DisplayEdit(m_Display_Num);
	multi_register_value_tcp[7302]=m_Display_Num;
   }
   else
   {   
	   m_Display_Num=multi_register_value_tcp[7302];
	   UpdateData(FALSE);
	   AfxMessageBox(_T("Try again"));
   }
}
void CDialogCM5::OnEnKillfocusEditInterval()
{
	UpdateData(TRUE);
	int ret=write_one(g_tstat_id,7303,m_Interval);
	if (ret>0)
	{
		multi_register_value_tcp[7303]=m_Interval;
	} 
	else
	{
		m_Interval=multi_register_value_tcp[7303];
		UpdateData(FALSE);
		AfxMessageBox(_T("Try again"));
	}

}
void CDialogCM5::OnEnKillfocusEditDTS1()
{
	CString dts;
	GetDlgItem(IDC_DTS1)->GetWindowText(dts);
	int sequence=(int)_wtoi(dts);
	int ret=write_one(g_tstat_id,7304,sequence);
	if (ret>0)
	{
		multi_register_value_tcp[7304]=sequence;
	} 
	else
	{
	 
		                                                        
		AfxMessageBox(_T("Try again"));
	}
}

void CDialogCM5::OnEnKillfocusEditDTS2()
{
	CString dts;
	GetDlgItem(IDC_DTS2)->GetWindowText(dts);
	int sequence=(int)_wtoi(dts);
	int ret=write_one(g_tstat_id,7305,sequence);
	if (ret>0)
	{
		multi_register_value_tcp[7305]=sequence;
	} 
	else
	{
	 

		AfxMessageBox(_T("Try again"));
	}
}
void CDialogCM5::OnEnKillfocusEditDTS3()
{
	CString dts;
	GetDlgItem(IDC_DTS3)->GetWindowText(dts);
	int sequence=_wtoi(dts);
	int ret=write_one(g_tstat_id,7306,sequence);
	if (ret>0)
	{
		multi_register_value_tcp[7306]=sequence;
	} 
	else
	{


		AfxMessageBox(_T("Try again"));
	}
}
void CDialogCM5::OnEnKillfocusEditDTS4()
{
	CString dts;
	GetDlgItem(IDC_DTS4)->GetWindowText(dts);
	int sequence=_wtoi(dts);
	int ret=write_one(g_tstat_id,7307,sequence);
	if (ret>0)
	{
		multi_register_value_tcp[7307]=sequence;
	} 
	else
	{


		AfxMessageBox(_T("Try again"));
	}
}
void CDialogCM5::OnEnKillfocusEditDTS5()
{
	CString dts;
	GetDlgItem(IDC_DTS5)->GetWindowText(dts);
	int sequence=_wtoi(dts);
	int ret=write_one(g_tstat_id,7308,sequence);
	if (ret>0)
	{
		multi_register_value_tcp[7308]=sequence;
	} 
	else
	{

		AfxMessageBox(_T("Try again"));
	}

}
void CDialogCM5::OnEnKillfocusEditDTS6()
{
	CString dts;
	GetDlgItem(IDC_DTS6)->GetWindowText(dts);
	int sequence=_wtoi(dts);
	int ret=write_one(g_tstat_id,7309,sequence);
	if (ret>0)
	{
		multi_register_value_tcp[7309]=sequence;
	} 
	else
	{

		AfxMessageBox(_T("Try again"));
	}
}
void CDialogCM5::OnEnKillfocusEditDTS7()
{
	CString dts;
	GetDlgItem(IDC_DTS7)->GetWindowText(dts);
	int sequence=_wtoi(dts);
	int ret=write_one(g_tstat_id,7310,sequence);
	if (ret>0)
	{
		multi_register_value_tcp[7310]=sequence;
	} 
	else
	{

		AfxMessageBox(_T("Try again"));
	}
}
void CDialogCM5::OnEnKillfocusEditDTS8()
{ 
	CString dts;
	GetDlgItem(IDC_DTS8)->GetWindowText(dts);
	int sequence=_wtoi(dts);
	int ret=write_one(g_tstat_id,7311,sequence);
	if (ret>0)
	{
		multi_register_value_tcp[7311]=sequence;
	} 
	else
	{

		AfxMessageBox(_T("Try again"));
	}
}
void CDialogCM5::OnEnKillfocusEditDTS9()
{

	CString dts;
	GetDlgItem(IDC_DTS9)->GetWindowText(dts);
	int sequence=_wtoi(dts);
	int ret=write_one(g_tstat_id,7312,sequence);
	if (ret>0)
	{
		multi_register_value_tcp[7312]=sequence;
	} 
	else
	{

		AfxMessageBox(_T("Try again"));
	}
}
void CDialogCM5::OnEnKillfocusEditDTS10()
{

	CString dts;
	GetDlgItem(IDC_DTS8)->GetWindowText(dts);
	int sequence=_wtoi(dts);
	int ret=write_one(g_tstat_id,7311,sequence);
	if (ret>0)
	{
		multi_register_value_tcp[7311]=sequence;
	} 
	else
	{

		AfxMessageBox(_T("Try again"));
	}
}
////////////////////////////////////////////////
void CDialogCM5::OnEnKillfocusEditoutput()
{
//CString strText;
//m_edit_output.GetWindowText(strText);
//m_edit_output.ShowWindow(SW_HIDE);  
//int lRow = m_msflexgridoutput.get_RowSel();	
//int lCol = m_msflexgridoutput.get_ColSel(); 
//m_msflexgridoutput.put_TextMatrix(lRow,lCol,strText);

	// TODO: Add your control notification handler code here

	CString strText;
	m_edit_output.GetWindowText(strText);
	m_edit_output.ShowWindow(SW_HIDE); 
	if (m_type==0)
	{
 
		int lRow = m_msflexgridoutput.get_RowSel();	
		int lCol = m_msflexgridoutput.get_ColSel(); 


		if ((strText.GetLength()<=0)||strText.CompareNoCase(m_tempGridString)==0)
		{
			return;
		}
		if (strText.GetLength()>THE_INPUT_OUTPUT_LENGTH)
		{
			AfxMessageBox(_T("The chars>14"));
			return;
		}
		unsigned char p[14];
		//if null ,replace by ' '.	
		//for(int i=0;i<THE_INPUT_OUTPUT_LENGTH;i++)
		//{
		//	if(i<strText.GetLength())
		//		{//p[i]=strText.GetAt(i);
		//	//char cTemp[256];
		//	WideCharToMultiByte( CP_ACP, 0, strText.GetAt(i), -1, p, 256, NULL, NULL );
		//		}

		//	else
		//		p[i]=' ';
		//}
		WideCharToMultiByte( CP_ACP, 0, strText.GetBuffer(), -1,(LPSTR)p, 256, NULL, NULL );
		//if null ,replace by ' '.
		for(int i=strText.GetLength();i<THE_INPUT_OUTPUT_LENGTH;i++)
		{

			p[i]=' ';
		} 
		if (Write_Multi(g_tstat_id,p,Get_RegID(lRow),THE_INPUT_OUTPUT_LENGTH)>0)
		{
			//m_msflexgridoutput.put_TextMatrix(lRow,lCol,strText);

		} 
		else
		{  
			//m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T(""));
			strText=_T("");
			AfxMessageBox(_T("Try again!"));
		}
		m_msflexgridoutput.put_TextMatrix(lRow,lCol,strText);
	} 
	else
	{
		int lRow = m_subtstatgrid.get_RowSel();	
		int lCol = m_subtstatgrid.get_ColSel(); 


		if ((strText.GetLength()<=0)||strText.CompareNoCase(m_tempGridString)==0)
		{
			return;
		}
		if (strText.GetLength()>THE_INPUT_OUTPUT_LENGTH)
		{
			AfxMessageBox(_T("The chars>14"));
			return;
		}
		unsigned char p[14];
		//if null ,replace by ' '.
		/*for(int i=0;i<THE_INPUT_OUTPUT_LENGTH;i++)
		{
			if(i<strText.GetLength())
				p[i]=strText.GetAt(i);
			else
				p[i]=' ';
		}*/
		WideCharToMultiByte( CP_ACP, 0, strText.GetBuffer(), -1, (LPSTR)p, 256, NULL, NULL );
		//if null ,replace by ' '.
		for(int i=strText.GetLength();i<THE_INPUT_OUTPUT_LENGTH;i++)
		{

			p[i]=' ';
		} 
		if (Write_Multi(g_tstat_id,p,Get_RegID(lRow),THE_INPUT_OUTPUT_LENGTH)>0)
		{
			//m_msflexgridoutput.put_TextMatrix(lRow,lCol,strText);

		} 
		else
		{  
			//m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T(""));
			strText=_T("");
			AfxMessageBox(_T("Try again!"));
		}
		m_subtstatgrid.put_TextMatrix(lRow,lCol,strText);
	}



	

	CRect rcCell(0,0,1,1);
	m_edit_output.MoveWindow(&rcCell,1);
}

void CDialogCM5::OnTimer(UINT_PTR nIDEvent)
{
	
 
	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CView* pNewView = pMain->m_pViews[4];
	CView* pActiveView = pMain->GetActiveView();

	if ( pNewView != pActiveView )    // current is CM5 dialog
	{
		KillTimer(MiniPanelTimer);
	}



	static int nCount1 = 0;

	nCount1++;
//	TRACE(_T("nCount1 = %d\n"),nCount1);


	if (DATATIME_TIMER == nIDEvent)
	{
		//CTime now;
		//now = CTime::GetCurrentTime();
		//m_static_datetime = now.Format(_T("%A,%B %d,%Y  %H:%M:%S"));		//?"Saturday, April 12, 2012"		
		CTime time = CTime::GetCurrentTime();

		CString strtime = time.Format(_T("%I:%M:%S %p"));
		//CString strtime = time.Format(_T("%m/%d/%Y %H:%M:%S %a"));
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TIME);
		pEdit->SetWindowText(strtime);

		CString strDate = time.Format(_T("%A, %b %d, %Y"));
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DATE);
		pEdit->SetWindowText(strDate);
		 /* read_one(g_tstat_id,104);*/
		//UpdateData(FALSE);
 
//			if (nCount1% timer == 0)
//			{
//				nCount1 = 0;
////				TRACE(_T("start GetData\n"));
//				BOOL flag = GetData();
//					ShowDialogData();
//				if (flag)
//				{
////					TRACE(_T("start ShowDialogData\n"));
//				
//				}
//				timer = 60*2;
//
//			}
	

	} 
 
	CFormView::OnTimer(nIDEvent);
	
}

void CDialogCM5::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
    m_progressctrl.ShowWindow(FALSE);
	CString index;

	//============================================================================================列表框设置（界面中间部份列表框）
	m_Output_GridCM5_.put_Rows(9); //行
	m_Output_GridCM5_.put_Cols(16);
	//set row high
	m_Output_GridCM5_.put_WordWrap(TRUE);
	m_Output_GridCM5_.put_RowHeight(0,500);
	//title middle show
	for (int n=0;n<=9;n++)
	{
		m_Output_GridCM5_.put_ColAlignment(n,4);
	}
	//设置列宽
	m_Output_GridCM5_.put_ColWidth(0,400);
	m_Output_GridCM5_.put_ColWidth(1,800);
	m_Output_GridCM5_.put_ColWidth(2,1200);
	m_Output_GridCM5_.put_ColWidth(3 ,800);
	m_Output_GridCM5_.put_ColWidth(4 ,1000);
	m_Output_GridCM5_.put_ColWidth(5 ,1000);
	m_Output_GridCM5_.put_ColWidth(6 ,1000);
	m_Output_GridCM5_.put_ColWidth(7 ,1000);
	m_Output_GridCM5_.put_ColWidth(8 ,1000);
	m_Output_GridCM5_.put_ColWidth(9 ,1200);
	m_Output_GridCM5_.put_ColWidth(10,1200);
	m_Output_GridCM5_.put_ColWidth(11,1200);
	m_Output_GridCM5_.put_ColWidth(12,1200);
	m_Output_GridCM5_.put_ColWidth(13,1200);
	m_Output_GridCM5_.put_ColWidth(14,1200);

	m_Output_GridCM5_.put_TextMatrix(0,0,_T("No"));
	m_Output_GridCM5_.put_TextMatrix(0,1,_T("Device\nName"));
	m_Output_GridCM5_.put_TextMatrix(0,2,_T("Modbus ID\nAddress"));
	m_Output_GridCM5_.put_TextMatrix(0,3,_T("SN"));
	m_Output_GridCM5_.put_TextMatrix(0,4 ,_T("Cooling\nSetpoint"));
	m_Output_GridCM5_.put_TextMatrix(0,5 ,_T("Heating\nSetpoint"));
	m_Output_GridCM5_.put_TextMatrix(0,6 ,_T("Setpoint"));
	m_Output_GridCM5_.put_TextMatrix(0,7 ,_T("Room\nTemperture"));
	m_Output_GridCM5_.put_TextMatrix(0,8 ,_T("Mode"));
	m_Output_GridCM5_.put_TextMatrix(0,9 ,_T("Night Heating\nSetpoint"));
	m_Output_GridCM5_.put_TextMatrix(0,10 ,_T("Night Cooling\nSetpoint"));
	m_Output_GridCM5_.put_TextMatrix(0,11,_T("Occupied"));
	m_Output_GridCM5_.put_TextMatrix(0,12,_T("Output Status"));
	m_Output_GridCM5_.put_TextMatrix(0,13,_T("Night Heating\n DB"));
	m_Output_GridCM5_.put_TextMatrix(0,14,_T("Night Cooling\n DB"));
	m_Output_GridCM5_.put_TextMatrix(0,15,_T("Override Value"));
	//彩色显示
	for(int i=1;i<9;i++)
	{

		//		for(int k=0;k<=12;k++)
		index.Format(_T("%d"),i);
		m_Output_GridCM5_.put_TextMatrix(i,0,index);
		for(int k=0;k<=9;k++)
		{


			if (i%2==1)
			{
				m_Output_GridCM5_.put_Row(i);
				m_Output_GridCM5_.put_Col(k);
				m_Output_GridCM5_.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_Output_GridCM5_.put_Row(i);
				m_Output_GridCM5_.put_Col(k);
				m_Output_GridCM5_.put_CellBackColor(COLOR_CELL);
			}
		}
	}

	//============================================================================================================界面Output部份列表框

	//设置行/列数量
	m_msflexgridoutput.put_Rows(11);
	m_msflexgridoutput.put_Cols(5);
	//设置列宽	
	m_msflexgridoutput.put_ColWidth(0,400);
	m_msflexgridoutput.put_ColWidth(1,1500);
	m_msflexgridoutput.put_ColWidth(4,700);

	//显示横标题
	m_msflexgridoutput.put_TextMatrix(0,1,_T("Name"));
	m_msflexgridoutput.put_TextMatrix(0,2,_T("Value"));
	m_msflexgridoutput.put_TextMatrix(0,3,_T("Auto/Man"));
	//m_msflexgridoutput.put_TextMatrix(0,4,_T("Range"));
	m_msflexgridoutput.put_TextMatrix(0,4,_T("Enable"));



	//居中显示
	for (int col=0;col<5;col++)
	{ 
		m_msflexgridoutput.put_ColAlignment(col,4);
	}

	//彩色显示
	for(int i=1;i<11;i++)		//排数量
	{
		index.Format(_T("%d"),i);
		m_msflexgridoutput.put_TextMatrix(i,0,index);
		for(int k=0;k<5;k++)	//列数量
		{
			if (i%2==1)
			{
				m_msflexgridoutput.put_Row(i);
				m_msflexgridoutput.put_Col(k);
				m_msflexgridoutput.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_msflexgridoutput.put_Row(i);
				m_msflexgridoutput.put_Col(k);
				m_msflexgridoutput.put_CellBackColor(COLOR_CELL);
			}
		}
	}

	//------------------sub tstat--------------------------------
	//设置排/行数量
	m_subtstatgrid.put_Cols(4);
	m_subtstatgrid.put_Rows(9);
	m_subtstatgrid.put_ColWidth(0,400);
	m_subtstatgrid.put_ColWidth(1,1500);
	 
	//显示横标题
	m_subtstatgrid.put_TextMatrix(0,1,_T("Name"));
	m_subtstatgrid.put_TextMatrix(0,2,_T("Type"));
	m_subtstatgrid.put_TextMatrix(0,3,_T("Value"));
	for (int i=1;i<9;i++)
	{
		index.Format(_T("%d"),i);
		m_subtstatgrid.put_TextMatrix(i,0,index);
		for(int k=0;k<4;k++)	//列数量
		{
			if (i%2==1)
			{
				m_subtstatgrid.put_Row(i);
				m_subtstatgrid.put_Col(k);
				m_subtstatgrid.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_subtstatgrid.put_Row(i);
				m_subtstatgrid.put_Col(k);
				m_subtstatgrid.put_CellBackColor(COLOR_CELL);
			}
		}
	}
	//======================================>>DI<<===============界面Input部份列表框

	//设置排/行数量
	m_msflexgrid_input.put_Cols(4);
	m_msflexgrid_input.put_Rows(9);
	//显示横标题
	m_msflexgrid_input.put_TextMatrix(0,1,_T("Name"));
	m_msflexgrid_input.put_TextMatrix(0,2,_T("Value"));
	m_msflexgrid_input.put_TextMatrix(0,3,_T("Auto/Man"));
	//m_msflexgrid_input.put_TextMatrix(0,4,_T("Range"));
	//m_msflexgrid_input.put_TextMatrix(0,4,_T("Enable"));
	//设置列宽	
	m_msflexgrid_input.put_ColWidth(0,400);
	m_msflexgrid_input.put_ColWidth(1,1500);
	//m_msflexgrid_input.put_ColWidth(4,700);

	//居中显示
	for (int col=0;col<4;col++)
	{ 
		m_msflexgrid_input.put_ColAlignment(col,4);
	}

	//彩色显示
	//CString index;
	for(int i=1;i<9;i++)		//排数量
	{
		index.Format(_T("%d"),i);
		m_msflexgrid_input.put_TextMatrix(i,0,index);
		for(int k=0;k<4;k++)	//列数量
		{
			if (i%2==1)
			{
				m_msflexgrid_input.put_Row(i);
				m_msflexgrid_input.put_Col(k);
				m_msflexgrid_input.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_msflexgrid_input.put_Row(i);
				m_msflexgrid_input.put_Col(k);
				m_msflexgrid_input.put_CellBackColor(COLOR_CELL);
			}
		}
	}


	//////////////////////////------------------------------AI
	m_DIGrid.put_Cols(8);
	m_DIGrid.put_Rows(11);
	m_DIGrid.put_ColWidth(0,400);
	m_DIGrid.put_ColWidth(1,1500);
	m_DIGrid.put_ColWidth(7,700);
	//显示横标题
	m_DIGrid.put_TextMatrix(0,1,_T("Name"));
	m_DIGrid.put_TextMatrix(0,2,_T("Value"));
	m_DIGrid.put_TextMatrix(0,3,_T("Auto/Man"));
	m_DIGrid.put_TextMatrix(0,4,_T("Range"));
	m_DIGrid.put_TextMatrix(0,5,_T("Filter"));
	m_DIGrid.put_TextMatrix(0,6,_T("Calibration"));
	m_DIGrid.put_TextMatrix(0,7,_T("Enable"));

	for(int i=1;i<11;i++)		//排数量
	{
		index.Format(_T("%d"),i);
		m_DIGrid.put_TextMatrix(i,0,index);
		for(int k=0;k<8;k++)	//列数量
		{
			if (i%2==1)
			{
				m_DIGrid.put_Row(i);
				m_DIGrid.put_Col(k);
				m_DIGrid.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_DIGrid.put_Row(i);
				m_DIGrid.put_Col(k);
				m_DIGrid.put_CellBackColor(COLOR_CELL);
			}
		}
	}


	UpdateData(FALSE);


	m_combobox2_input.ShowWindow(SW_HIDE);
 	m_edit_input.ShowWindow(SW_HIDE); 
 	m_edit_output.ShowWindow(SW_HIDE);	
}

BOOL CDialogCM5::PreTranslateMessage(MSG* pMsg)
{
 
	
	return CFormView::PreTranslateMessage(pMsg);
}
void CDialogCM5::UpdateGrid(){
	CString index;

	//============================================================================================列表框设置（界面中间部份列表框）
	m_Output_GridCM5_.put_Rows(9); //行
	m_Output_GridCM5_.put_Cols(16);
	//set row high
	m_Output_GridCM5_.put_WordWrap(TRUE);
	m_Output_GridCM5_.put_RowHeight(0,500);
	//title middle show
	for (int n=0;n<=9;n++)
	{
		m_Output_GridCM5_.put_ColAlignment(n,4);
	}
	//设置列宽
	m_Output_GridCM5_.put_ColWidth(0,400);
	m_Output_GridCM5_.put_ColWidth(1,800);
	m_Output_GridCM5_.put_ColWidth(2,1200);
	m_Output_GridCM5_.put_ColWidth(3 ,800);
	m_Output_GridCM5_.put_ColWidth(4 ,1000);
	m_Output_GridCM5_.put_ColWidth(5 ,1000);
	m_Output_GridCM5_.put_ColWidth(6 ,1000);
	m_Output_GridCM5_.put_ColWidth(7 ,1000);
	m_Output_GridCM5_.put_ColWidth(8 ,1000);
	m_Output_GridCM5_.put_ColWidth(9 ,1200);
	m_Output_GridCM5_.put_ColWidth(10,1200);
	m_Output_GridCM5_.put_ColWidth(11,1200);
	m_Output_GridCM5_.put_ColWidth(12,1200);
	m_Output_GridCM5_.put_ColWidth(13,1200);
	m_Output_GridCM5_.put_ColWidth(14,1200);

	m_Output_GridCM5_.put_TextMatrix(0,0,_T("No"));
	m_Output_GridCM5_.put_TextMatrix(0,1,_T("Device\nName"));
	m_Output_GridCM5_.put_TextMatrix(0,2,_T("Modbus ID\nAddress"));
	m_Output_GridCM5_.put_TextMatrix(0,3,_T("SN"));
	m_Output_GridCM5_.put_TextMatrix(0,4 ,_T("Cooling\nSetpoint"));
	m_Output_GridCM5_.put_TextMatrix(0,5 ,_T("Heating\nSetpoint"));
	m_Output_GridCM5_.put_TextMatrix(0,6 ,_T("Setpoint"));
	m_Output_GridCM5_.put_TextMatrix(0,7 ,_T("Room\nTemperture"));
	m_Output_GridCM5_.put_TextMatrix(0,8 ,_T("Mode"));
	m_Output_GridCM5_.put_TextMatrix(0,9 ,_T("Night Heating\nSetpoint"));
	m_Output_GridCM5_.put_TextMatrix(0,10 ,_T("Night Cooling\nSetpoint"));
	m_Output_GridCM5_.put_TextMatrix(0,11,_T("Occupied"));
	m_Output_GridCM5_.put_TextMatrix(0,12,_T("Output Status"));
	m_Output_GridCM5_.put_TextMatrix(0,13,_T("Night Heating\n DB"));
	m_Output_GridCM5_.put_TextMatrix(0,14,_T("Night Cooling\n DB"));
	m_Output_GridCM5_.put_TextMatrix(0,15,_T("Override Value"));
	//彩色显示
	for(int i=1;i<9;i++)
	{

		//		for(int k=0;k<=12;k++)
		index.Format(_T("%d"),i);
		m_Output_GridCM5_.put_TextMatrix(i,0,index);
		for(int k=0;k<=9;k++)
		{


			if (i%2==1)
			{
				m_Output_GridCM5_.put_Row(i);
				m_Output_GridCM5_.put_Col(k);
				m_Output_GridCM5_.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_Output_GridCM5_.put_Row(i);
				m_Output_GridCM5_.put_Col(k);
				m_Output_GridCM5_.put_CellBackColor(COLOR_CELL);
			}
		}
	} 
}
void CDialogCM5::ShowDialogData()
{


	//=======================================================initialization====================================================================
	int cooling_temp;
	int coasting_temp;
	int heating_temp;

	float tstat_version2;
	
	//============================================================System Parameters===============================================================
	// 		200	1	second
	// 		201	1	minute
	// 		202	1	hour
	// 		203	1	day
	// 		204	1	week, 0=Sunday; 1=Monday; and so on.
	// 		205	1	month
	// 		206	1	lower byte of the year, for example, here should be 98 in year 1998
	// 		207	1	higher byte of the year, for example, here should be 19 in the year 1998

	//	m_strDate.Format(_T("%d%d-%d-%d"),cm5_register_value[207],cm5_register_value[206],cm5_register_value[205],cm5_register_value[203]);
	 
	
		/*
		Comment by:Alex
		Date:2013/04/10
		Purpose:

		*/
		//m_strDateCM5.Format(_T("%02d%02d-%02d-%02d"),20,multi_register_value_tcp[201],multi_register_value_tcp[202],multi_register_value_tcp[204]);//临时演示
		//m_strTime.Format(_T("%02d:%02d:%02d"),multi_register_value_tcp[205],multi_register_value_tcp[206],multi_register_value_tcp[207]);
		m_IDaddressCM5=multi_register_value_tcp[6];	

		UINT nSelectSerialNumber =multi_register_value_tcp[0]+multi_register_value_tcp[1]*256+multi_register_value_tcp[2]*256*256+multi_register_value_tcp[3]*256*256*256;// cm5_register_value[0]+cm5_register_value[1]*256+cm5_register_value[2]*256*256+cm5_register_value[3]*256*256*256;
		m_nSerialNumCM5=nSelectSerialNumber;
		tstat_version2 = (float)(multi_register_value_tcp[5]*256+product_register_value[4]);	
		tstat_version2 /=10;
		m_firmwareVersionCM5 = tstat_version2;
		m_hardware_versionCM5=multi_register_value_tcp[8];
		m_bootloaderversion.Format(_T("%d"),multi_register_value_tcp[30]);

		if (multi_register_value_tcp[128]==1)
		{
		  m_Baudrate.SetWindowText(_T("19200"));
		} 
		else
		{
		  m_Baudrate.SetWindowText(_T("9600"));
		}
	//}
	//================================================Parameter=========================================================

	// 	176	1	priority mode. 0--OFF  1--TIMER   2--ON
	// 	177	1	priority period . Range is 1 - 99min
	// 	178	1	priority time left

	if (multi_register_value_tcp[176]!=0||multi_register_value_tcp[176]!=1||multi_register_value_tcp[176]!=2)
		m_combo_prioritymode.SetCurSel(0);
	else
		m_combo_prioritymode.SetCurSel(multi_register_value_tcp[176]);
	m_priorityinterval = multi_register_value_tcp[177];
	m_countprioritytime.Format(_T("%d"),multi_register_value_tcp[178]);
	
	if (multi_register_value_tcp[31]!=1)
	{
	 m_checkdemo.SetCheck(0);
	} 
	else
	{
	 m_checkdemo.SetCheck(1);
	}
	/*
	Comment by:Alex
	Date:2013/04/10
	Purpose:
	显示Tstat的状态统计
	*/
	m_result.Format(_T("%d"),multi_register_value_tcp[29]);
	m_coating.Format(_T("%d"),multi_register_value_tcp[26]);
	m_heating.Format(_T("%d"),multi_register_value_tcp[27]);
	m_cooling.Format(_T("%d"),multi_register_value_tcp[28]);

	/*
	Comment by:Alex
	Date:2013/04/10
	Purpose:
	Tcp infomation
	*/
	if(multi_register_value_tcp[106]==0)
	{
		m_ipModelComBox.SetCurSel(0);
	}
	if(multi_register_value_tcp[106]==1)
	{
		m_ipModelComBox.SetCurSel(1);
	}
	m_nListenPort.Format(_T("%d"),multi_register_value_tcp[120]);
	
	CString mac_address;
	mac_address.Format(_T("%02x-%02x-%02x-%02x-%02x-%02x"),multi_register_value_tcp[100],multi_register_value_tcp[101],multi_register_value_tcp[102],multi_register_value_tcp[103],multi_register_value_tcp[104],multi_register_value_tcp[105]);
	m_Mac_Address.SetWindowText(mac_address);

	m_ip_addressCtrl.SetAddress((BYTE)multi_register_value_tcp[107],(BYTE)multi_register_value_tcp[108],(BYTE)multi_register_value_tcp[109],(BYTE)multi_register_value_tcp[110]);
	m_subnet_addressCtrl.SetAddress((BYTE)multi_register_value_tcp[111],(BYTE)multi_register_value_tcp[112],(BYTE)multi_register_value_tcp[113],(BYTE)multi_register_value_tcp[114]);
	m_gateway_addressCtrl.SetAddress((BYTE)multi_register_value_tcp[115],(BYTE)multi_register_value_tcp[116],(BYTE)multi_register_value_tcp[117],(BYTE)multi_register_value_tcp[118]);
	/////
	m_Display_Num=multi_register_value_tcp[7302];
	m_Interval=multi_register_value_tcp[7303];
	UpdateData(FALSE);
	Show_DisplayEdit(multi_register_value_tcp[7302]);

	//==========================================显示界面中间列表框中的数据==============================================

	//	    5671 to 5678	8	cooling setpoint value of Sub Tstats(register380)
	//		5679 to 5686	8	heating setpoint value of Sub Tstats(register136)
	//		5687 to 5694	8	setpoint value of Sub Tstats(register135)
	//		5695 to 5702	8	room temperture value of Sub Tstats(register101)
	//		5703 to 5710	8	mode of Sub Tstats(register107)
	//		5711 to 5718	8	output state of Sub Tstats(register108)

	//		5735 to 5742	8	night heatting setpoint value of Sub Tstats(register182)
	//		5743 to 5750	8	night cooling setpoint value of Sub Tstats(register183)
	//		5751 to 5758	8	tstat product model of Sub Tstats(register7)
	
	_subnetwork.clear();
	for (int i = 0;i<8;i++)
	{

	    m_subetwork.m_Occupied=(multi_register_value_tcp[5670]&(unsigned short)pow((double)2,i));//1,2,4,8,16,32,64,128
		m_subetwork.m_coolingSet.Format(_T("%.1f°C"),(float)multi_register_value_tcp[5671+i]/10);
		m_subetwork.m_heatingSet.Format(_T("%.1f°C"),(float)multi_register_value_tcp[5671+i+8]);
		m_subetwork.m_setpoint.Format(_T("%.1f°C"),(float)multi_register_value_tcp[5671+i+16]); 
		m_subetwork.m_roomTemper.Format(_T("%.1f°C"),(float)multi_register_value_tcp[5671+i+24]/10); 
		m_subetwork.m_mode = multi_register_value_tcp[5671+i+32];     
		m_subetwork.m_outputState.Format(_T("%.1f"),(float)multi_register_value_tcp[5671+i+40]/10); 
		m_subetwork.m_nightHeatingDB.Format(_T("%.1f°C"),(float)multi_register_value_tcp[5671+i+48]);
		m_subetwork.m_nightCoolingDB.Format(_T("%.1f°C"),(float)multi_register_value_tcp[5671+i+56]); 
		m_subetwork.m_nightHeating.Format(_T("%.1f°C"),(float)multi_register_value_tcp[5671+i+64]);
		m_subetwork.m_nightCooling.Format(_T("%.1f°C"),(float)multi_register_value_tcp[5671+i+72]); 
		m_subetwork.m_tstatProduct = multi_register_value_tcp[5671+i+80]; 
		m_subetwork.m_overridevalue.Format(_T("%d"),multi_register_value_tcp[5671+i+88]);
		m_subetwork.m_SerialNumber.Format(_T("%d"),multi_register_value_tcp[5671+i+96]+multi_register_value_tcp[5671+i+96+1]*256+multi_register_value_tcp[5671+i+96+2]*256*256+multi_register_value_tcp[5671+i+96+3]*256*256*256);
		 m_subetwork.m_modbusaddr.Format(_T("%d"),multi_register_value_tcp[18+i]);
		_subnetwork.push_back(m_subetwork);

	}

	
	CString m_num,strtemp1,strtemp2;
	coasting_temp = 0;
	heating_temp = 0;
	cooling_temp = 0;
	m_Output_GridCM5_.Clear();
	 

	UpdateGrid();
	int type;
	for (int i=1;i<9;i++)
	{

	    type=multi_register_value_tcp[173+i];
		if (type!=1)
		{
		 continue;
		}
		int ret = _subnetwork.at(i-1).m_modbusaddr.Compare(_T("0"));
		if(ret == 0)
			continue;
		m_num.Format(_T("%d"),i);
		m_Output_GridCM5_.put_TextMatrix(i,0,m_num);//第一列：序号

		switch(_subnetwork.at(i-1).m_tstatProduct)
		{
		case 1:strtemp1=g_strTstat5b;break;
		case 2:strtemp1=g_strTstat5a;break;
		case 3:strtemp1=g_strTstat5b;break;
		case 4:strtemp1=g_strTstat5c;break;
		case 12:			strtemp1=g_strTstat5d;break;
		case 100:		strtemp1=g_strnetWork;break;
		case NET_WORK_OR485_PRODUCT_MODEL:strtemp1=g_strOR485;break;
		case 17:strtemp1=g_strTstat5f;break;
		case 18:strtemp1=g_strTstat5g;break;
		case 16:strtemp1=g_strTstat5e;break;
		case 19:strtemp1=g_strTstat5h;break;
		case 6:strtemp1=g_strTstat6;break;
		case 7:strtemp1=g_strTstat7;break;
		case 13:
		case 14:break;
		default:strtemp1=_T("Tstat");break;
		}
		m_Output_GridCM5_.put_TextMatrix(i,1,strtemp1);//Device Name
		m_Output_GridCM5_.put_TextMatrix(i,2,_subnetwork.at(i-1).m_modbusaddr);//Address
		m_Output_GridCM5_.put_TextMatrix(i,3,_subnetwork.at(i-1).m_SerialNumber);//SN
		m_Output_GridCM5_.put_TextMatrix(i,4,_subnetwork.at(i-1).m_coolingSet);
		m_Output_GridCM5_.put_TextMatrix(i,5,_subnetwork.at(i-1).m_heatingSet);
		m_Output_GridCM5_.put_TextMatrix(i,6,_subnetwork.at(i-1).m_setpoint);
		m_Output_GridCM5_.put_TextMatrix(i,7,_subnetwork.at(i-1).m_roomTemper);

		switch (_subnetwork.at(i-1).m_mode)		 
		{
		case 0:
			strtemp2.Format(_T("%s"),_T("COASTING"));
			coasting_temp++;
			break;
		case 1:
			strtemp2.Format(_T("%s"),_T("COOLING1"));
			cooling_temp++;
			break;
		case 2:
			strtemp2.Format(_T("%s"),_T("COOLING2"));	
			cooling_temp++;
			break;
		case 3:
			strtemp2.Format(_T("%s"),_T("COOLING3"));
			cooling_temp++;
			break;
		case 4:
			strtemp2.Format(_T("%s"),_T("HEATING1"));
			heating_temp++;
			break;
		case 5:
			strtemp2.Format(_T("%s"),_T("HEATING2"));
			heating_temp++;
			break;
		case 6:
			strtemp2.Format(_T("%s"),_T("HEATING3"));
			heating_temp++;
			break;

		default:
			strtemp2.Format(_T("%s"),_T("COASTING"));
			coasting_temp++;
			break;
		}
		m_Output_GridCM5_.put_TextMatrix(i,8,strtemp2);

		m_Output_GridCM5_.put_TextMatrix(i,9,_subnetwork.at(i-1).m_nightHeating);
		m_Output_GridCM5_.put_TextMatrix(i,10,_subnetwork.at(i-1).m_nightCooling);
		if  (
		    (_subnetwork.at(i-1).m_Occupied==1)
		  ||(_subnetwork.at(i-1).m_Occupied==2)
		  ||(_subnetwork.at(i-1).m_Occupied==4)
		  ||(_subnetwork.at(i-1).m_Occupied==8)
		  ||(_subnetwork.at(i-1).m_Occupied==16)
		  ||(_subnetwork.at(i-1).m_Occupied==32)
		  ||(_subnetwork.at(i-1).m_Occupied==64)
		  ||(_subnetwork.at(i-1).m_Occupied==128)
		  )   
		{
		 m_Output_GridCM5_.put_TextMatrix(i,11,_T("Online"));
		} 
		else
		{
		 m_Output_GridCM5_.put_TextMatrix(i,11,_T("Offline"));
		}
		m_Output_GridCM5_.put_TextMatrix(i,12,_subnetwork.at(i-1).m_outputState);
		m_Output_GridCM5_.put_TextMatrix(i,13,_subnetwork.at(i-1).m_nightHeatingDB);
		m_Output_GridCM5_.put_TextMatrix(i,14,_subnetwork.at(i-1).m_nightCoolingDB);
		m_Output_GridCM5_.put_TextMatrix(i,15,_subnetwork.at(i-1).m_overridevalue);
	}
		/*
		Comment by:Alex
		Date:2013/04/23
		Purpose:
		Initial Grid Table
		*/
		
	CString strTemp;
	BOOL bit;
	m_type=0;
	UINT reg184=multi_register_value_tcp[184];
	UINT reg188=multi_register_value_tcp[188];
	UINT reg191=multi_register_value_tcp[191];
	for(int i=1;i<11;i++)		//排数量 output 
	{
		//index.Format(_T("%d"),i);_T("12345671234567")
		m_msflexgridoutput.put_TextMatrix(i,1,GetTextReg(Get_RegID(i),FALSE));
		bit=Get_Data_Bit(reg184,10,i);
		if (bit)
		{
		 m_msflexgridoutput.put_TextMatrix(i,2,_T("ON"));
		} 
		else
		{
		 m_msflexgridoutput.put_TextMatrix(i,2,_T("OFF"));
		}
		bit=Get_Data_Bit(reg191,10,i);
		if (bit)
		{
			m_msflexgridoutput.put_TextMatrix(i,3,_T("Manual"));
		} 
		else
		{
			m_msflexgridoutput.put_TextMatrix(i,3,_T("Auto"));
		}
		
		bit=Get_Data_Bit(reg188,10,i);
		if (bit)
		{
			m_msflexgridoutput.put_TextMatrix(i,4,_T("ON"));
		} 
		else
		{
			m_msflexgridoutput.put_TextMatrix(i,4,_T("OFF"));
		}
		//184:Value 
		//191	1	output auto or manual feature    
		//188	1	digital input enable, only ON/OFF (output 1 - 10)DI Enable

	}
	m_type=1;
//	int type;

	for(int i=1;i<9;i++)		//排数量 Sub Tstat
	{
		//index.Format(_T("%d"),i);_T("12345671234567")
		m_subtstatgrid.put_TextMatrix(i,1,GetTextReg(Get_RegID(i),FALSE));
	
		type=multi_register_value_tcp[173+i];
		if (type==0)
		{
		   strTemp=_T("UNUSED");
		} 
		else if (type==1)
		{
		  strTemp=_T("Normal");
		} 
		else
		{
		  strTemp=_T("Switch");
		}
		 m_subtstatgrid.put_TextMatrix(i,2,strTemp);//Address

		 if (type!=0)//Tstat
		 {
			 int ret = _subnetwork.at(i-1).m_modbusaddr.Compare(_T("0"));
			 if (ret==0)
			 {
				 m_subtstatgrid.put_TextMatrix(i,3,_T("UNUSED"));//Address
			 }
			 else
			 {
				 m_subtstatgrid.put_TextMatrix(i,3,_subnetwork.at(i-1).m_modbusaddr);//Address
			 }
		 } 
		 else//Value
		 {
		 }
		 
	    
	}
	m_type=2;
	UINT reg183=multi_register_value_tcp[183];
	UINT reg190=multi_register_value_tcp[190];
	
	for(int i=1;i<9;i++)		//排数量,input_DI
	{
		m_msflexgrid_input.put_TextMatrix(i,1,GetTextReg(Get_RegID(i),FALSE));
		//m_msflexgrid_input.put_TextMatrix(i,1,_T("12345671234567"));//FALSE-不需要读取对应寄存器中的值，使用内存数组中的数据
		//183 Value
		//190	1	digital input auto or manual feature  DI-A/M
		bit=Get_Data_Bit(reg183,8,i);
		if (bit)
		{
			m_msflexgrid_input.put_TextMatrix(i,2,_T("ON"));
		} 
		else
		{
			m_msflexgrid_input.put_TextMatrix(i,2,_T("OFF"));
		}
		bit=Get_Data_Bit(reg190,8,i);
		if (bit)
		{
			m_msflexgrid_input.put_TextMatrix(i,3,_T("Manual"));
		} 
		else
		{
			m_msflexgrid_input.put_TextMatrix(i,3,_T("Auto"));
		}


	}
	m_type=3;
	UINT reg189=multi_register_value_tcp[189];
	UINT reg192=multi_register_value_tcp[192];
	
	for(int i=1;i<11;i++)		//排数量,input AI
	{
	/*
	
	134 to 143	10	value of Input1 - input10 
	144 to 153	10	range of Input1 - input10
	range of Input1 - input10 ANALOG INPUT1 RANGE.
	0 = raw data, 1 = thermistor, 2 = %, 3 = ON/OFF, 4 = N/A, 5 = OFF/ON
	154 to 163	10	input filter
	164 to 173	10	input calibration
	189      	1	analog input enable, only ON/OFF(AI 1 - 10)  AI-Enable
	192	1	analog input auto or manual feature

	*/
		m_DIGrid.put_TextMatrix(i,1,GetTextReg(Get_RegID(i),FALSE));
		//m_msflexgrid_input.put_TextMatrix(i,1,_T("12345671234567"));//FALSE-不需要读取对应寄存器中的值，使用内存数组中的数据
	    strTemp.Format(_T("%d"),multi_register_value_tcp[134+i-1]);//Value
		m_DIGrid.put_TextMatrix(i,2,strTemp);

		 //strTemp.Format(_T("%0.1f°C"),multi_register_value_tcp[144+i-1]/10);//Range
		 strTemp=AI_Range[multi_register_value_tcp[143+i]];
		 m_DIGrid.put_TextMatrix(i,4,strTemp);
 		  
		strTemp.Format(_T("%d"),multi_register_value_tcp[154+i-1]);//filter
		m_DIGrid.put_TextMatrix(i,5,strTemp);
		strTemp.Format(_T("%d"),multi_register_value_tcp[164+i-1]);//Calibration
		m_DIGrid.put_TextMatrix(i,6,strTemp);
		//A/M  ,Enable
		bit=Get_Data_Bit(reg189,10,i);
		if (bit)
		{
			m_DIGrid.put_TextMatrix(i,7,_T("ON"));
		} 
		else
		{
			m_DIGrid.put_TextMatrix(i,7,_T("OFF"));
		}
		bit=Get_Data_Bit(reg192,10,i);
		if (bit)
		{
			m_DIGrid.put_TextMatrix(i,3,_T("Manual"));
		} 
		else
		{
			m_DIGrid.put_TextMatrix(i,3,_T("Auto"));
		}
	   
	}
	 
}



BEGIN_EVENTSINK_MAP(CDialogCM5, CFormView)
ON_EVENT(CDialogCM5, IDC_INPUT_MSFLEXGRID, DISPID_CLICK, CDialogCM5::ClickInputMsflexgrid, VTS_NONE)
ON_EVENT(CDialogCM5, IDC_INPUT_MSFLEXGRID, DISPID_DBLCLICK, CDialogCM5::DblClickInputMsflexgrid, VTS_NONE)
ON_EVENT(CDialogCM5, IDC_OUTPUT_MSFLEXGRID, DISPID_CLICK, CDialogCM5::ClickOutputMsflexgrid, VTS_NONE)
ON_EVENT(CDialogCM5, IDC_OUTPUT_MSFLEXGRID, DISPID_DBLCLICK, CDialogCM5::DblClickOutputMsflexgrid, VTS_NONE)
ON_EVENT(CDialogCM5, IDC_SUBTSTAT_INPUT, DISPID_CLICK, CDialogCM5::ClickSubTstatgrid, VTS_NONE)
ON_EVENT(CDialogCM5, IDC_AI_Grid, DISPID_CLICK, CDialogCM5::ClickInputMsflexgrid2, VTS_NONE)	 
ON_EVENT(CDialogCM5, IDC_AI_Grid, DISPID_DBLCLICK, CDialogCM5::DblClickInputMsflexgrid2, VTS_NONE)
END_EVENTSINK_MAP()

	/*
	Date:2013/04/24
	Purpose:
		  click DI
	*/
void CDialogCM5::ClickInputMsflexgrid()
{
	KillTimer(DATATIME_TIMER);
	m_combobox2_input.ShowWindow(SW_HIDE);
	UpdateData(FALSE);
	m_type=2;
	/*m_switch = 1;
	m_Input_Output=TRUE;
	// TODO: Add your message handler code here
	//get_RowSel();这是获取总的行数
	long row = m_msflexgrid_input.get_RowSel();
	long col = m_msflexgrid_input.get_ColSel();
	if ((col==2&&row>10)||col==3)
	{
		if ((m_cellcol == 2&&m_cellrow>10)||m_cellcol == 3)//原来是否有标志背景单元格
		{
			if (m_cellrow%2 == 1)
			{
				m_msflexgrid_input.put_Row(m_cellrow);m_msflexgrid_input.put_Col(m_cellcol);m_msflexgrid_input.put_CellBackColor(RGB(255,255,255));//单数行
			}else
			{
				m_msflexgrid_input.put_Row(m_cellrow);m_msflexgrid_input.put_Col(m_cellcol);m_msflexgrid_input.put_CellBackColor(COLOR_CELL);//双数行
			}
		}

		m_msflexgrid_input.put_Row(row);m_msflexgrid_input.put_Col(col);m_msflexgrid_input.put_CellBackColor(RGB(100,100,150));


	}
	else
	{
		if ((m_cellcol == 2&&m_cellrow>10)||m_cellcol == 3)
		{

			if (m_cellrow%2 == 1)
			{
				m_msflexgrid_input.put_Row(m_cellrow);m_msflexgrid_input.put_Col(m_cellcol);m_msflexgrid_input.put_CellBackColor(RGB(255,255,255));
			}else
			{
				m_msflexgrid_input.put_Row(m_cellrow);m_msflexgrid_input.put_Col(m_cellcol);m_msflexgrid_input.put_CellBackColor(COLOR_CELL);
			}
		}
		if (col == 4)
		{
			m_msflexgrid_input.put_Row(row);m_msflexgrid_input.put_Col(col);m_msflexgrid_input.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);//第5列灰色显示

		}else if (row%2 == 1)
		{
			m_msflexgrid_input.put_Row(row);m_msflexgrid_input.put_Col(col);m_msflexgrid_input.put_CellBackColor(RGB(255,255,255));
		}else
		{
			m_msflexgrid_input.put_Row(row);m_msflexgrid_input.put_Col(col);m_msflexgrid_input.put_CellBackColor(COLOR_CELL);
		}


	}
	m_cellrow = row;
	m_cellcol = col;		*/															 

	//使用edition控件形式显示

	long lRow,lCol;
	lRow = m_msflexgrid_input.get_RowSel();
	lCol = m_msflexgrid_input.get_ColSel(); 

	if(lCol == 1)
	{
		CRect rect;
		m_msflexgrid_input.GetWindowRect(rect); //
		ScreenToClient(rect); //
		CDC* pDC =GetDC();

		int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
		int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
		
		long y = m_msflexgrid_input.get_RowPos(lRow)/nTwipsPerDotY;
		long x = m_msflexgrid_input.get_ColPos(lCol)/nTwipsPerDotX;
		
		long width = m_msflexgrid_input.get_ColWidth(lCol)/nTwipsPerDotX+1;
		long height = m_msflexgrid_input.get_RowHeight(lRow)/nTwipsPerDotY+1;
		
		CRect rcCell(x,y,x+width,y+height);
		
		rcCell.OffsetRect(rect.left+1,rect.top+1);
		ReleaseDC(pDC);
		CString strValue = m_msflexgrid_input.get_TextMatrix(lRow,lCol);
		m_tempGridString=_T("");
		m_tempGridString=strValue;
			if(1==lCol)		
			{		
				m_edit_input.MoveWindow(&rcCell,1);
				m_edit_input.ShowWindow(SW_SHOW);	
				m_edit_input.SetWindowText(strValue);	
				m_edit_input.SetFocus();
				m_edit_input.SetCapture();//使随后的鼠标输入都被发送到这个CWnd 
				int nLenth=strValue.GetLength();	
				m_edit_input.SetSel(nLenth,nLenth); 

			}	
	
	}


	SetTimer(DATATIME_TIMER,1000,NULL);
}
	/*
	Date:2013/04/24
	Purpose:
	  click AI
	*/
void CDialogCM5::ClickInputMsflexgrid2()
{
	KillTimer(DATATIME_TIMER);
	m_combobox2_input.ShowWindow(SW_HIDE);
	UpdateData(FALSE);
	m_type=3;
	 

	long lRow,lCol;
	lRow = m_DIGrid.get_RowSel();
	lCol = m_DIGrid.get_ColSel(); 

	 
		CRect rect;
		m_DIGrid.GetWindowRect(rect); //
		ScreenToClient(rect); //
		CDC* pDC =GetDC();

		int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
		int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
		
		long y = m_DIGrid.get_RowPos(lRow)/nTwipsPerDotY;
		long x = m_DIGrid.get_ColPos(lCol)/nTwipsPerDotX;
		
		long width = m_DIGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
		long height = m_DIGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
		
		CRect rcCell(x,y,x+width,y+height);
		
		rcCell.OffsetRect(rect.left+1,rect.top+1);
		ReleaseDC(pDC);
		CString strValue = m_DIGrid.get_TextMatrix(lRow,lCol);
		m_tempGridString=_T("");
		m_tempGridString=strValue;
			if((1==lCol)||(2==lCol)||(5==lCol)||(6==lCol))		
			{		
				m_edit_input.MoveWindow(&rcCell,1);
				m_edit_input.ShowWindow(SW_SHOW);	
				m_edit_input.SetWindowText(strValue);	
				m_edit_input.SetFocus();
				m_edit_input.SetCapture();//使随后的鼠标输入都被发送到这个CWnd 
				int nLenth=strValue.GetLength();	
				m_edit_input.SetSel(nLenth,nLenth); 

			}
			else if (4==lCol)
			{
				m_combobox2_input.ResetContent();

				for(int i=0;i<6;i++)
				{
					m_combobox2_input.AddString(AI_Range[i]);
				}

				m_combobox2_input.MoveWindow(rcCell); //移动到选中格的位置，覆盖
				m_combobox2_input.ShowWindow(SW_SHOW);
				m_combobox2_input.BringWindowToTop();
				//m_valueCombx.SelectString(-1,strValue);
				m_combobox2_input.SetFocus(); //获取焦点
				m_combobox2_input.SetWindowText(strValue);
			}
				
	
	 


	SetTimer(DATATIME_TIMER,1000,NULL);
}
   	/*
   	Date:2013/04/24
   	Purpose:
   	   double click DI 
   	*/
void CDialogCM5::DblClickInputMsflexgrid()
{
	//183 Value
	//190	1	digital input auto or manual feature  DI-A/M

#if 1
	KillTimer(DATATIME_TIMER);
	m_combobox2_input.ShowWindow(SW_HIDE);
	UpdateData(FALSE);

	long lRow = m_msflexgrid_input.get_RowSel();//获取点击的行号	
	long lCol = m_msflexgrid_input.get_ColSel(); //获取点击的列号

	if (lCol == 2)
	{

		CString strgrid = m_msflexgrid_input.get_TextMatrix(lRow,lCol+1);
		CString m_strauto = _T("Manual");
		int retauto = m_strauto.Collate(strgrid);


		if (retauto == 0)
		{
			unsigned short temp =1;				
			CString strValue = m_msflexgrid_input.get_TextMatrix(lRow,lCol);
			CString m_stron = _T("OFF");
			int ret = m_stron.Collate(strValue);



			

				//参数写入设备
				//1-代表ON            0-代表OFF
				//1-代表手动Manual    0-代表自动Auto

			//183 Value
			//190	1	digital input auto or manual feature  DI-A/M

				int m_lRow= lRow-11;
				temp = temp<<m_lRow;				
				if (ret == 0)
				{

					temp =temp|multi_register_value_tcp[183];

					int ww = write_one(g_tstat_id,183,temp);
					TRACE(_T("No:%d = write_one(on/off) is %d\n"),lRow,ww);
					if (ww>0)
					{
						m_msflexgrid_input.put_TextMatrix(lRow,lCol,_T("ON"));
						multi_register_value_tcp[183]=temp;
					}else
					{
						m_msflexgrid_input.put_TextMatrix(lRow,lCol,_T("OFF"));
					}
				}
				else
				{

					temp = ~temp;
					temp =temp&multi_register_value_tcp[183];

					int ww = write_one(g_tstat_id,183,temp,1);
					TRACE(_T("No:%d = write_one(on/off) is %d\n"),lRow,temp);
					if (ww>0)
					{
						m_msflexgrid_input.put_TextMatrix(lRow,lCol,_T("OFF"));
						multi_register_value_tcp[183]=temp;
					}
					else
					{
						m_msflexgrid_input.put_TextMatrix(lRow,lCol,_T("ON"));
					}
				}

			
			

		}
		else
		{
		   AfxMessageBox(_T("Auto can't Change"));
		}
	}
	else if (lCol == 3)
	{
		CString strgrid = m_msflexgrid_input.get_TextMatrix(lRow,lCol);
		CString m_strauto = _T("Auto");
		int retauto = m_strauto.Collate(strgrid);
		if (retauto == 0)
		{
			unsigned short temp =1;
			 
				//	182	2	auto/manual feature of analog input 1 - 10
				temp = temp<<(lRow-1);
				temp =temp|multi_register_value_tcp[190];

				int ww = write_one(g_tstat_id,190,temp,1);
				TRACE(_T("No:%d = write_one is %d\n"),lRow,ww);
				if (ww>0)
				{
					m_msflexgrid_input.put_TextMatrix(lRow,lCol,_T("Manual"));
					multi_register_value_tcp[190]=temp;
				}
				else
					{m_msflexgrid_input.put_TextMatrix(lRow,lCol,_T("Auto")); }
			 



		}else
		{
			unsigned short temp =1;

			 
				//	182	2	auto/manual feature of analog input 1 - 10
				temp = temp<<(lRow-1);
				temp = ~temp;
				temp =temp&multi_register_value_tcp[190];

				int ww = write_one(g_tstat_id,190,temp,1);
				TRACE(_T("No:%d = write_one is %d\n"),lRow,ww);
				if (ww>0)
				{
					m_msflexgrid_input.put_TextMatrix(lRow,lCol,_T("Auto"));
					multi_register_value_tcp[190]=temp;
				}
				else
					{m_msflexgrid_input.put_TextMatrix(lRow,lCol,_T("Manual"));	}
			 
		}
	}
	 
	SetTimer(DATATIME_TIMER,1000,NULL);
#endif
}
	/*
	Date:2013/04/24
	Purpose:
	  单击output
	*/
void CDialogCM5::ClickOutputMsflexgrid()
{
	KillTimer(DATATIME_TIMER);
	m_combobox2_input.ShowWindow(SW_HIDE);
	UpdateData(FALSE);
	m_type=0;
	m_switch = 2;
	
	long row = m_msflexgridoutput.get_RowSel();
	long col = m_msflexgridoutput.get_ColSel();
	
	//if (col==2||col==3)
	//{
	//	if (m_cellcolout==2||m_cellcolout==3)
	//	{
	//		if (m_cellrowout%2 == 1)
	//		{
	//			m_msflexgridoutput.put_Row(m_cellrowout);
	//			m_msflexgridoutput.put_Col(m_cellcolout);
	//			m_msflexgridoutput.put_CellBackColor(RGB(255,255,255));//单数行
	//		}else
	//		{
	//			m_msflexgridoutput.put_Row(m_cellrowout);
	//			m_msflexgridoutput.put_Col(m_cellcolout);
	//			m_msflexgridoutput.put_CellBackColor(COLOR_CELL);//双数行
	//		}


	//	}

	//	m_msflexgridoutput.put_Row(row);
	//	m_msflexgridoutput.put_Col(col);
	//	m_msflexgridoutput.put_CellBackColor(RGB(100,100,150));

	//}
	//else
	//{
	//	if (m_cellcolout==2||m_cellcolout==3)
	//	{
	//		if (m_cellrowout%2 == 1)
	//		{
	//			m_msflexgridoutput.put_Row(m_cellrowout);m_msflexgridoutput.put_Col(m_cellcolout);m_msflexgridoutput.put_CellBackColor(RGB(255,255,255));
	//		}else
	//		{
	//			m_msflexgridoutput.put_Row(m_cellrowout);m_msflexgridoutput.put_Col(m_cellcolout);m_msflexgridoutput.put_CellBackColor(COLOR_CELL);
	//		}
	//	}
	//	if (col == 4)
	//	{
	//		m_msflexgridoutput.put_Row(row);m_msflexgridoutput.put_Col(col);m_msflexgridoutput.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);//第5列灰色显示
	//	}else if (row%2 == 1)
	//	{
	//		m_msflexgridoutput.put_Row(row);m_msflexgridoutput.put_Col(col);m_msflexgridoutput.put_CellBackColor(RGB(255,255,255));
	//	}else
	//	{
	//		m_msflexgridoutput.put_Row(row);m_msflexgridoutput.put_Col(col);m_msflexgridoutput.put_CellBackColor(COLOR_CELL);
	//	}

	//}

	m_cellrowout = row;
	m_cellcolout = col;



	//使用edition控件形式显示

	long lRow,lCol;
	lRow = m_msflexgridoutput.get_RowSel();
	lCol = m_msflexgridoutput.get_ColSel(); 

	if(lCol == 1)
	{
		CRect rect;
		m_msflexgridoutput.GetWindowRect(rect); 
		ScreenToClient(rect); 
		CDC* pDC =GetDC();

		int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
		int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
		
		long y = m_msflexgridoutput.get_RowPos(lRow)/nTwipsPerDotY;
		long x = m_msflexgridoutput.get_ColPos(lCol)/nTwipsPerDotX;
		
		long width = m_msflexgridoutput.get_ColWidth(lCol)/nTwipsPerDotX+1;
		long height = m_msflexgridoutput.get_RowHeight(lRow)/nTwipsPerDotY+1;
		
		CRect rcCell(x,y,x+width,y+height);
		
		rcCell.OffsetRect(rect.left+1,rect.top+1);
		ReleaseDC(pDC);
		CString strValue = m_msflexgridoutput.get_TextMatrix(lRow,lCol);
		m_tempGridString=_T("");
		m_tempGridString=strValue;

		if(1==lCol)		
		{		
			m_edit_output.MoveWindow(&rcCell,1);
			m_edit_output.ShowWindow(SW_SHOW);	
			m_edit_output.SetWindowText(strValue);	
			m_edit_output.SetFocus();	
			m_edit_output.SetCapture();
			int nLenth=strValue.GetLength();	
			m_edit_output.SetSel(nLenth,nLenth); 	

		}		
	}


	SetTimer(DATATIME_TIMER,1000,NULL);

}
	/*
	Date:2013/04/24
	Purpose:
	单击sub Tstat
	*/
void CDialogCM5::ClickSubTstatgrid()
{
	KillTimer(DATATIME_TIMER);
	m_combobox2_input.ShowWindow(SW_HIDE);
	UpdateData(FALSE);

	m_type=1;
	//使用edition控件形式显示

	long lRow,lCol;
	lRow = m_subtstatgrid.get_RowSel();
	lCol = m_subtstatgrid.get_ColSel(); 

	
		CRect rect;
		m_subtstatgrid.GetWindowRect(rect); 
		ScreenToClient(rect); 
		CDC* pDC =GetDC();

		int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
		int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;

		long y = m_subtstatgrid.get_RowPos(lRow)/nTwipsPerDotY;
		long x = m_subtstatgrid.get_ColPos(lCol)/nTwipsPerDotX;

		long width = m_subtstatgrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
		long height = m_subtstatgrid.get_RowHeight(lRow)/nTwipsPerDotY+1;

		CRect rcCell(x,y,x+width,y+height);

		rcCell.OffsetRect(rect.left+1,rect.top+1);
		ReleaseDC(pDC);
		CString strValue = m_subtstatgrid.get_TextMatrix(lRow,lCol);
		m_tempGridString=_T("");
		m_tempGridString=strValue;
		if(1==lCol)		
		{		
			m_edit_output.MoveWindow(&rcCell,1);
			m_edit_output.ShowWindow(SW_SHOW);	
			m_edit_output.SetWindowText(strValue);	
			m_edit_output.SetFocus();	
			m_edit_output.SetCapture();
			int nLenth=strValue.GetLength();	
			m_edit_output.SetSel(nLenth,nLenth); 	

		}		
	
	if (lCol==2)
	{
		m_combox_enable.MoveWindow(&rcCell,1);
		m_combox_enable.ResetContent();
		m_combox_enable.AddString(_T("UNUSED"));
		m_combox_enable.AddString(_T("Normal"));
		m_combox_enable.AddString(_T("Switch"));
		m_combox_enable.ShowWindow(TRUE);
		m_combox_enable.SetWindowText(strValue);
		m_combox_enable.SetFocus();
		m_combox_enable.SetCapture();
	}

	SetTimer(DATATIME_TIMER,1000,NULL);
}
	/*
	Date:2013/04/24
	Purpose:
	  双击output
	*/
void CDialogCM5::DblClickOutputMsflexgrid()
{

	//184:Value 
	//191	1	output auto or manual feature    
	//188	1	digital input enable, only ON/OFF (output 1 - 10)DI Enable
	KillTimer(DATATIME_TIMER);
	m_combobox2_input.ShowWindow(SW_HIDE);
	UpdateData(FALSE);

	long lRow = m_msflexgridoutput.get_RowSel();//获取点击的行号	
	long lCol = m_msflexgridoutput.get_ColSel(); //获取点击的列号

	if (lCol == 2)
	{

		CString strgrid = m_msflexgridoutput.get_TextMatrix(lRow,lCol+1);
		CString m_strauto = _T("Manual");
		int retauto = m_strauto.Collate(strgrid);


		if (retauto == 0)
		{
			unsigned short temp =1;				
			CString strValue = m_msflexgridoutput.get_TextMatrix(lRow,lCol);
			CString m_stron = _T("OFF");
			int ret = m_stron.Collate(strValue);

				//参数写入设备
				//1-代表ON            0-代表OFF
				//1-代表手动Manual    0-代表自动Auto
				//174	1	Digital output1 - 8, valves/pumps

				int m_lRow= lRow;
				//temp = temp<<m_lRow;
				UINT num=1;
				for (int i=1;i<m_lRow;i++)
				{
				  num=num*2;
				}
								
				if (ret == 0)//OFF->ON
				{

					temp=multi_register_value_tcp[184];
					temp=temp+num;//对应的位0->1

					int ww = write_one(g_tstat_id,184,temp);
					//TRACE(_T("No:%d = write_one(on/off) is %d\n"),lRow,ww);
					if (ww>0)
					{
						m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T("ON"));
						multi_register_value_tcp[184]=temp;
					}else
					{
						m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T("OFF"));
					}
				}

				else//ON->OFF
				{

					temp=multi_register_value_tcp[184];
					temp=temp-num;//对应的位1->0

					int ww = write_one(g_tstat_id,184,temp,1);
					//TRACE(_T("No:%d = write_one(on/off) is %d\n"),lRow,temp);
					if (ww>0)
					{
						m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T("OFF"));
						multi_register_value_tcp[184]=temp;
					}
					else
					{
						m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T("ON"));
					}
				}

			

		}//Mannul 
		else
		{
		  AfxMessageBox(_T("Auto can't change"));
		}
	}
	else if (lCol == 3)
	{
		CString strgrid = m_msflexgridoutput.get_TextMatrix(lRow,lCol);
		CString m_strauto = _T("Auto");
		int retauto = m_strauto.Collate(strgrid);
		if (retauto == 0)
		{
			unsigned short temp =1;


			//   181	2	auto/manual feature of output 1 - 10

			temp = temp<<(lRow-1);
			temp =temp|multi_register_value_tcp[191];

			int ww = write_one(g_tstat_id,191,temp);
			TRACE(_T("No:%d = write_one is %d\n"),lRow,ww);
			if (ww>0)
			{
				m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T("Manual"));
				multi_register_value_tcp[191]=temp;
			}
			else
				m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T("Auto"));





		}else
		{
			unsigned short temp =1;

			//   181	2	auto/manual feature of output 1 - 10
			temp = temp<<(lRow-1);
			temp = ~temp;
			temp =temp&multi_register_value_tcp[191];

			int ww = write_one(g_tstat_id,191,temp,1);
			TRACE(_T("No:%d = write_one is %d\n"),lRow,ww);
			if (ww>0)
			{
				m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T("Auto"));
				multi_register_value_tcp[191]=temp;
			}
			else
				m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T("Manual"));

		}
	}
	else if (lCol==4)
	{
		unsigned short temp =1;				
		CString strValue = m_msflexgridoutput.get_TextMatrix(lRow,lCol);
		CString m_stron = _T("OFF");
		int ret = m_stron.Collate(strValue);

		//参数写入设备
		//1-代表ON            0-代表OFF
		//1-代表手动Manual    0-代表自动Auto
		//174	1	Digital output1 - 8, valves/pumps

		int m_lRow= lRow;
		//temp = temp<<m_lRow;
		UINT num=1;
		for (int i=1;i<m_lRow;i++)
		{
			num=num*2;
		}

		if (ret == 0)//OFF->ON
		{

			temp=multi_register_value_tcp[188];
			temp=temp+num;//对应的位0->1

			int ww = write_one(g_tstat_id,188,temp);
			//TRACE(_T("No:%d = write_one(on/off) is %d\n"),lRow,ww);
			if (ww>0)
			{
				m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T("ON"));
				multi_register_value_tcp[188]=temp;
			}else
			{
				m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T("OFF"));
			}
		}

		else//ON->OFF
		{

			temp=multi_register_value_tcp[188];
			temp=temp-num;//对应的位1->0

			int ww = write_one(g_tstat_id,188,temp,1);
			//TRACE(_T("No:%d = write_one(on/off) is %d\n"),lRow,temp);
			if (ww>0)
			{
				m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T("OFF"));
				multi_register_value_tcp[188]=temp;
			}
			else
			{
				m_msflexgridoutput.put_TextMatrix(lRow,lCol,_T("ON"));
			}
		}
	}
	SetTimer(DATATIME_TIMER,1000,NULL);
}
	/*
	Date:2013/04/24
	Purpose:
	  双击AI
	*/
void CDialogCM5::DblClickInputMsflexgrid2()	  //	   双击AI
{
	KillTimer(DATATIME_TIMER);
	m_combobox2_input.ShowWindow(SW_HIDE);
	UpdateData(FALSE);

	long lRow = m_DIGrid.get_RowSel();//获取点击的行号	
	long lCol = m_DIGrid.get_ColSel(); //获取点击的列号

   if (lCol == 3)
	{
		CString strgrid = m_DIGrid.get_TextMatrix(lRow,lCol);
		CString m_strauto = _T("Auto");
		int retauto = m_strauto.Collate(strgrid);
		if (retauto == 0)
		{
			unsigned short temp =1;


			//   181	2	auto/manual feature of output 1 - 10

			temp = temp<<lRow-1;
			temp =temp|multi_register_value_tcp[192];

			int ww = write_one(g_tstat_id,192,temp);
			 
			if (ww>0)
			{
				m_DIGrid.put_TextMatrix(lRow,lCol,_T("Manual"));
				multi_register_value_tcp[192]=temp;
			}
			else
				m_DIGrid.put_TextMatrix(lRow,lCol,_T("Auto"));





		}else
		{
			unsigned short temp =1;

			//   181	2	auto/manual feature of output 1 - 10
			temp = temp<<lRow-1;
			temp = ~temp;
			temp =temp&multi_register_value_tcp[192];

			int ww = write_one(g_tstat_id,192,temp,1);
			TRACE(_T("No:%d = write_one is %d\n"),lRow,ww);
			if (ww>0)
			{
				m_DIGrid.put_TextMatrix(lRow,lCol,_T("Auto"));
				multi_register_value_tcp[192]=temp;
			}
			else
				m_DIGrid.put_TextMatrix(lRow,lCol,_T("Manual"));

		}
	}
   else if (lCol==7)
   {
	   unsigned short temp =1;				
	   CString strValue = m_DIGrid.get_TextMatrix(lRow,lCol);
	   CString m_stron = _T("OFF");
	   int ret = m_stron.Collate(strValue);

	   //参数写入设备
	   //1-代表ON            0-代表OFF
	   //1-代表手动Manual    0-代表自动Auto
	   //174	1	Digital output1 - 8, valves/pumps

	   int m_lRow= lRow;
	   //temp = temp<<m_lRow;
	   UINT num=1;
	   for (int i=1;i<m_lRow;i++)
	   {
		   num=num*2;
	   }

	   if (ret == 0)//OFF->ON
	   {

		   temp=multi_register_value_tcp[189];
		   temp=temp+num;//对应的位0->1

		   int ww = write_one(g_tstat_id,189,temp);
		   //TRACE(_T("No:%d = write_one(on/off) is %d\n"),lRow,ww);
		   if (ww>0)
		   {
			   m_DIGrid.put_TextMatrix(lRow,lCol,_T("ON"));
			   multi_register_value_tcp[189]=temp;
		   }else
		   {
			   m_DIGrid.put_TextMatrix(lRow,lCol,_T("OFF"));
		   }
	   }

	   else//ON->OFF
	   {

		   temp=multi_register_value_tcp[189];
		   temp=temp-num;//对应的位1->0

		   int ww = write_one(g_tstat_id,189,temp,1);
		   //TRACE(_T("No:%d = write_one(on/off) is %d\n"),lRow,temp);
		   if (ww>0)
		   {
			   m_DIGrid.put_TextMatrix(lRow,lCol,_T("OFF"));
			   multi_register_value_tcp[189]=temp;
		   }
		   else
		   {
			   m_DIGrid.put_TextMatrix(lRow,lCol,_T("ON"));
		   }
	   }
   }
	SetTimer(DATATIME_TIMER,1000,NULL);
}