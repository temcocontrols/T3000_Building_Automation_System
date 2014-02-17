// DisplayConfig.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "DisplayConfig.h"
#include "afxdialogex.h"
#include "globle_function.h"

// CDisplayConfig dialog
#define  THE_CHAR_LENGTH 8

IMPLEMENT_DYNAMIC(CDisplayConfig, CDialog)
CDisplayConfig::CDisplayConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayConfig::IDD, pParent)
{
/*m_display_number=15;*/
}

CDisplayConfig::~CDisplayConfig()
{
}
BOOL CDisplayConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_display_number=1;
	m_FlexGrid1.put_Rows(1);
	m_FlexGrid1.put_Cols(2);
	m_FlexGrid1.put_TextMatrix(0,0,_T("Seq_NO"));
	m_FlexGrid1.put_TextMatrix(0,1,_T("Display_Choice"));
	m_FlexGrid1.put_ColWidth(0,800);
	m_FlexGrid1.put_ColWidth(1,2200);
	UpdateData(FALSE);
	OnBnClickedOk();
	Fresh_Grid();
	return TRUE;
}
void CDisplayConfig::DoDataExchange(CDataExchange* pDX)
{
CDialog::DoDataExchange(pDX);
DDX_Control(pDX, IDC_INPUT_MSFLEXGRID, m_FlexGrid1);
DDX_Text(pDX, IDC_INPUT_9, m_display_number);
DDX_Control(pDX, IDC_SEQ_COM, m_ItemValueCombx);
DDX_Control(pDX, IDC_PROCESS, m_progress);
}
BEGIN_MESSAGE_MAP(CDisplayConfig, CDialog)
	ON_BN_CLICKED(IDOK, &CDisplayConfig::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_INPUT_9, &CDisplayConfig::OnEnKillfocusInput9)
ON_CBN_SELCHANGE(IDC_SEQ_COM, &CDisplayConfig::OnCbnSelchangeSeqCom)
//ON_EN_KILLFOCUS(IDC_LINE_1, &CDisplayConfig::OnEnKillfocusLine1)
ON_BN_CLICKED(IDCANCEL, &CDisplayConfig::OnBnClickedCancel)
ON_BN_CLICKED(IDC_SAVE2, &CDisplayConfig::OnBnClickedSave2)
ON_BN_CLICKED(IDC_SAVE_LINE2, &CDisplayConfig::OnBnClickedSaveLine2)
ON_BN_CLICKED(IDC_SAVE, &CDisplayConfig::OnBnClickedSave)
ON_BN_CLICKED(IDC_SAVE_INPUT1, &CDisplayConfig::OnBnClickedSaveInput1)
ON_BN_CLICKED(IDC_SAVE_INPUT2, &CDisplayConfig::OnBnClickedSaveInput2)
ON_BN_CLICKED(IDC_SAVE_INPUT3, &CDisplayConfig::OnBnClickedSaveInput3)
ON_BN_CLICKED(IDC_SAVE_INPUT4, &CDisplayConfig::OnBnClickedSaveInput4)
ON_BN_CLICKED(IDC_SAVE_INPUT5, &CDisplayConfig::OnBnClickedSaveInput5)
ON_BN_CLICKED(IDC_SAVE_INPUT6, &CDisplayConfig::OnBnClickedSaveInput6)
ON_BN_CLICKED(IDC_SAVE_INPUT7, &CDisplayConfig::OnBnClickedSaveInput7)
ON_BN_CLICKED(IDC_SAVE_INPUT8, &CDisplayConfig::OnBnClickedSaveInput8)
ON_BN_CLICKED(IDC_FRESH_TABLE, &CDisplayConfig::OnBnClickedFreshTable)
//ON_BN_CLICKED(IDC_FRESH, &CDisplayConfig::OnBnClickedFresh)
END_MESSAGE_MAP()


 
CString CDisplayConfig::GetTextReg(unsigned short reg)
{
	CString str_temp=_T("");
	unsigned short temp_buffer[4];
	unsigned short temp_buffer_Char[THE_CHAR_LENGTH];
	unsigned char p[THE_CHAR_LENGTH+1]={'\0'};
	 
		temp_buffer[0]=product_register_value[reg];
		temp_buffer[1]=product_register_value[reg+1];
		temp_buffer[2]=product_register_value[reg+2];
		temp_buffer[3]=product_register_value[reg+3];
	 

	   if (temp_buffer[0]==0||temp_buffer[0]==65535)
	   {
	     return str_temp;
	   }
   unsigned short Hi_Char,Low_Char;

	for (int i=0;i<4;i++)
		{
		Hi_Char=temp_buffer[i];
		Hi_Char=Hi_Char&0xff00;
		Hi_Char=Hi_Char>>8;
		Low_Char=temp_buffer[i];
		Low_Char=Low_Char&0x00ff;
		temp_buffer_Char[2*i]=Hi_Char;
		temp_buffer_Char[2*i+1]=Low_Char;
		} 

	for (int i=0;i<THE_CHAR_LENGTH;i++)
		{
		p[i] =(unsigned char)temp_buffer_Char[i];
		}
	str_temp.Format(_T("%c%c%c%c%c%c%c%c"),p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);	
 
	 
	return str_temp;
	}


CString CDisplayConfig::GetTextFromReg(unsigned short reg){
	CString str_temp;
	unsigned short temp_buffer[4];
	unsigned short temp_buffer_Char[THE_CHAR_LENGTH];
	unsigned char p[THE_CHAR_LENGTH+1]={'\0'};
	if (Read_Multi(g_tstat_id,temp_buffer,reg,4))
	{  unsigned short Hi_Char,Low_Char;

	for (int i=0;i<4;i++)
	{
		Hi_Char=temp_buffer[i];
		Hi_Char=Hi_Char&0xff00;
		Hi_Char=Hi_Char>>8;
		Low_Char=temp_buffer[i];
		Low_Char=Low_Char&0x00ff;
		temp_buffer_Char[2*i]=Hi_Char;
		temp_buffer_Char[2*i+1]=Low_Char;
	} 

	for (int i=0;i<THE_CHAR_LENGTH;i++)
	{
		p[i] =(unsigned char)temp_buffer_Char[i];

	}

	str_temp.Format(_T("%c%c%c%c%c%c%c%c"),p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);


	} 
	else
	{
		AfxMessageBox(_T("Reading Error"));
	}
	return str_temp;
}

BEGIN_EVENTSINK_MAP(CDisplayConfig, CDialog)
	ON_EVENT(CDisplayConfig, IDC_INPUT_MSFLEXGRID, DISPID_DBLCLICK, CDisplayConfig::DblClickInputMsflexgrid, VTS_NONE)
END_EVENTSINK_MAP()
CString CDisplayConfig::GetSel(int ID)
{CString selection;
//m_ItemValueCombx.AddString(_T("temperature"));
//m_ItemValueCombx.AddString(_T("setpoint"));
//m_ItemValueCombx.AddString(_T("AI1"));
//m_ItemValueCombx.AddString(_T("AI2"));
//m_ItemValueCombx.AddString(_T("AI3"));
//m_ItemValueCombx.AddString(_T("AI4"));
//m_ItemValueCombx.AddString(_T("AI5"));
//m_ItemValueCombx.AddString(_T("AI6"));
//m_ItemValueCombx.AddString(_T("AI7"));
//m_ItemValueCombx.AddString(_T("AI8"));
//m_ItemValueCombx.AddString(_T("MODE"));
//m_ItemValueCombx.AddString(_T("USER INFO"));
//m_ItemValueCombx.AddString(_T("CLOCK DATE"));
//m_ItemValueCombx.AddString(_T("CLOCK TIME"));
  switch(ID)
  { case 0:
      selection=_T("None");
	  break;
    case 1:
	 selection=_T("temperature");
	 break;
	case 2:
		selection=_T("setpoint");
		break;
	case 3:
		selection=_T("AI1");
		break;
	case 4:
		selection=_T("AI2");
		break;
	case 5:
		selection=_T("AI3");
		break;
	case 6:
		selection=_T("AI4");
		break;
	case 7:
		selection=_T("AI5");
		break;
	case 8:
		selection=_T("AI6");
		break;
	case 9:
		selection=_T("AI7");
		break;
	case 10:
		selection=_T("AI8");
		break;
	case 11:
		selection=_T("MODE");
		break;
	case 12:
		selection=_T("USER INFO");
		break;
	case 13:
		selection=_T("CLOCK DATE");
		break;
	case 14:
		selection=_T("CLOCK TIME");
		break;
	case 15:
		selection=_T("CO2");
		break;
	case 16:
		selection=_T("Humidity");
		break;
	default:
		selection=_T("None");
		break;
  }
  return selection;
}
void CDisplayConfig::Fresh_Grid(){
	 
		m_display_number=product_register_value[MODBUS_LCD_ROTATE_ENABLE];//newtstat6[Get_RegID(multi_register_value[7],_T("Display Number"))];
		int display_number=1;
		if (m_display_number>=0)
		{
			display_number=m_display_number+1;
			m_FlexGrid1.put_Rows(display_number);
			m_FlexGrid1.put_Cols(2);
			m_FlexGrid1.put_TextMatrix(0,0,_T("Seq_NO"));
			m_FlexGrid1.put_TextMatrix(0,1,_T("Display_Choice"));
			m_FlexGrid1.put_ColWidth(0,800);
			m_FlexGrid1.put_ColWidth(1,2200);
			for (int i=1;i<=m_display_number;i++)
			{
				CString str_No;
				str_No.Format(_T("%d"),i);
				m_FlexGrid1.put_TextMatrix(i,0,str_No);
				m_FlexGrid1.put_TextMatrix(i,1,GetSel(product_register_value[MODBUS_DISP_ITEM_TEMPERATURE+i-1]));
			}
		} 
		else
		{
			AfxMessageBox(_T("Reading Error.Try again."));
		}
	
	 
	
	 

}
void CDisplayConfig::DblClickInputMsflexgrid()
{
if(g_OutPutLevel==1)
	return;
	long lRow,lCol;
	lRow = m_FlexGrid1.get_RowSel();//获取点击的行号	
	lCol = m_FlexGrid1.get_ColSel(); //获取点击的列号
	if((lCol==0))
		return;
	if(lRow>m_FlexGrid1.get_Rows()) //如果点击区超过最大行号，则点击是无效的
		return; 
	if(lRow == 0) //如果点击标题行，也无效
		return;
	CRect rect;
	m_FlexGrid1.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_FlexGrid1.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_FlexGrid1.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_FlexGrid1.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_FlexGrid1.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息
	CString strValue = m_FlexGrid1.get_TextMatrix(lRow,lCol);
	m_nCurRow=lRow;
	m_nCurCol=lCol;
	 
	 
	if(lCol==1)
		{
		/*
		lcd rotate enable, 
		decide how many items will be shown on rotate mode,range0 to 21.
		Display item select:0:none 
		1:temperature 
		2:setpoint 
		3:AI1 
		4:AI2 
		5:AI3 
		6:AI4 
		7:AI5 
		8:AI6 
		9:AI7 
		10:AI8 
		11:MODE 
		12:USER INFO 
		13:CLOCK DATE 
		14:CLOCK TIME (TBD:OUTPUT)
		*/
		m_ItemValueCombx.ResetContent();
		m_ItemValueCombx.AddString(_T("None"));
		m_ItemValueCombx.AddString(_T("temperature"));
		m_ItemValueCombx.AddString(_T("setpoint"));
		m_ItemValueCombx.AddString(_T("AI1"));
		m_ItemValueCombx.AddString(_T("AI2"));
		m_ItemValueCombx.AddString(_T("AI3"));
		m_ItemValueCombx.AddString(_T("AI4"));
		m_ItemValueCombx.AddString(_T("AI5"));
		m_ItemValueCombx.AddString(_T("AI6"));
		m_ItemValueCombx.AddString(_T("AI7"));
		m_ItemValueCombx.AddString(_T("AI8"));
		m_ItemValueCombx.AddString(_T("MODE"));
		m_ItemValueCombx.AddString(_T("USER INFO"));
		m_ItemValueCombx.AddString(_T("CLOCK DATE"));
		m_ItemValueCombx.AddString(_T("CLOCK TIME"));
		m_ItemValueCombx.AddString(_T("CO2"));
		m_ItemValueCombx.AddString(_T("Humidity"));
		m_ItemValueCombx.ShowWindow(SW_SHOW);//显示控件
		m_ItemValueCombx.MoveWindow(rc); //移动到选中格的位置，覆盖
		m_ItemValueCombx.BringWindowToTop();
		m_ItemValueCombx.SelectString(-1,strValue); //内容全选。方便直接修改		
		m_ItemValueCombx.SetFocus(); //获取焦点
		}
}
 
void CDisplayConfig::OnEnKillfocusInput9()
{
   UpdateData(TRUE);
   //写寄存器
   /*int jugde=m_display_number+1*/
   if (m_display_number>14||m_display_number<0)
   {
	   AfxMessageBox(_T("Input Error"));
   } 
   else
   {
	   int ret=write_one(g_tstat_id,MODBUS_LCD_ROTATE_ENABLE,m_display_number);
	   if (ret>0)
	   {
		   
		 product_register_value[MODBUS_LCD_ROTATE_ENABLE]=m_display_number;
		   

		   Fresh_Grid();
	   } 
	   else
	   {
		   AfxMessageBox(_T("Try again."));
	   }

   }
  
}

void CDisplayConfig::OnCbnSelchangeSeqCom()
{
   
  int ret=write_one(g_tstat_id,MODBUS_DISP_ITEM_TEMPERATURE+m_nCurRow-1,m_ItemValueCombx.GetCurSel());
  product_register_value[MODBUS_DISP_ITEM_TEMPERATURE+m_nCurRow-1]=m_ItemValueCombx.GetCurSel();
 
  Fresh_Grid();
}
 
BOOL CDisplayConfig::WriteCharsToReg(unsigned char *to_write,unsigned short start_address,int length){
       

    int ret=Write_Multi(g_tstat_id,to_write,start_address,length);
	if (ret==1)
	{
	  return TRUE;
	} 
	else
	{
	  return FALSE;
	}
	
}
BOOL CDisplayConfig::ReadCharsFromReg(unsigned short *put_data_into_here,unsigned short start_address,int length){

int ret=Read_Multi(g_tstat_id,put_data_into_here,start_address,length);
if (ret>0)
{
  return TRUE;
} 
else
{
  return FALSE;
}
}
void CDisplayConfig::OnBnClickedCancel()
{
// TODO: Add your control notification handler code here
CDialog::OnCancel();
}
void CDisplayConfig::OnBnClickedOk()
	{ 
	 // CString temp;
	 CString input1,input2,input3,input4,input5,input6,input7,input8;

	 int reg=MODBUS_LCD_ROTATE_ENABLE; 
	 int m_display_number=product_register_value[reg];

	 
	GetDlgItem(IDC_LINE_1)->SetWindowText(GetTextReg(MODBUS_LINE1_CHAR1));
	 
	//UpdateData(FALSE);
	
	GetDlgItem(IDC_LINE_2)->SetWindowText(GetTextReg(MODBUS_LINE2_CHAR1)); 
 
	//UpdateData(FALSE);
	input1=GetTextReg(MODBUS_AI1_CHAR1);
	GetDlgItem(IDC_INPUT_1)->SetWindowText(input1);
	 
	//UpdateData(FALSE);
	input2=GetTextReg(MODBUS_AI2_CHAR1);
	GetDlgItem(IDC_INPUT_2)->SetWindowText(input2);
	 
	//UpdateData(FALSE);
	input3=GetTextReg(MODBUS_AI3_CHAR1);
	GetDlgItem(IDC_INPUT_3)->SetWindowText(input3);
	 
	//UpdateData(FALSE);
	input4=GetTextReg(MODBUS_AI4_CHAR1);
	GetDlgItem(IDC_INPUT_4)->SetWindowText(input4);
	 
	//UpdateData(FALSE);
	input5=GetTextReg(MODBUS_AI5_CHAR1);
	GetDlgItem(IDC_INPUT_5)->SetWindowText(input5);
	 
	//UpdateData(FALSE);
	input6=GetTextReg(MODBUS_AI6_CHAR1);
	GetDlgItem(IDC_INPUT_6)->SetWindowText(input6);
	 input7=GetTextReg(MODBUS_AI7_CHAR1);
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_7)->SetWindowText(input7);
	 input8=GetTextReg(MODBUS_AI8_CHAR1);
	//UpdateData(FALSE);
	GetDlgItem(IDC_INPUT_8)->SetWindowText(input8);
	CString temp;
	temp.Format(_T("%d"),product_register_value[MODBUS_LCD_ROTATE_ENABLE]);
	GetDlgItem(IDC_INPUT_9)->SetWindowText(temp);
	Fresh_Grid();
	temp.Format(_T("%d"),get_serialnumber());

	_ConnectionPtr m_pCon;
	_RecordsetPtr m_pRs;
	::CoInitialize(NULL);
	m_pCon.CreateInstance("ADODB.Connection");
	m_pRs.CreateInstance(_T("ADODB.Recordset"));
	m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	try
	{
		CString strSql;BOOL is_exist=FALSE;	  CString str_temp;
		 
	 
			CString SerialNo,Input1,Input2,Input3,Input4,Input5,Input6,Input7,Input8;
			SerialNo=temp;
			 
			Input1=input1;
			Input2=input2;
			Input3=input3;
			Input4=input4;
			Input5=input5;
			Input6=input6;
			Input7=input7;
			Input8=input8;

			strSql.Format(_T("update IONAME set INPUT1='%s',INPUT2='%s',INPUT3='%s',INPUT4='%s',INPUT5='%s',INPUT6='%s',INPUT7='%s',INPUT8='%s' where SERIAL_ID='%s'"),Input1,Input2,Input3,Input4,Input5,Input6,Input7,Input8,SerialNo);
			m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
			 
			g_strInName1=Input1;
			g_strInName2=Input2;
			g_strInName3=Input3;
			g_strInName4=Input4;
			g_strInName5=Input5;
			g_strInName6=Input6;
			g_strInName7=Input7;
			g_strInName8=Input8;

		 
	}
	catch(_com_error e)
	{
		/* AfxMessageBox(e.Description());*/
		//MessageBox(m_name_new+_T("  has been here\n Please change another name!"));


		m_pCon->Close();
		return ;
	}
	m_pCon->Close(); 
  
    }
	//Write Line1,Line2
void CDisplayConfig::OnBnClickedSave2()
{
   CString input;
   GetDlgItem(IDC_LINE_1)->GetWindowText(input);
   input.TrimRight();
   CString input2;
   GetDlgItem(IDC_LINE_2)->GetWindowText(input2);
   input2.TrimRight();

   unsigned char p[16];//input+input1
   //input
   for(int i=0;i<THE_CHAR_LENGTH;i++)
	   {
	   if(i<input.GetLength())
		   p[i]=input.GetAt(i);
	   else
		   p[i]=' ';
	   }
	   //input1
   for(int i=0;i<THE_CHAR_LENGTH;i++)
	   {
	   if(i<input2.GetLength())
		   p[i+THE_CHAR_LENGTH]=input2.GetAt(i);
	   else
		   p[i+THE_CHAR_LENGTH]=' ';
	   }
   if (input.GetLength()>16)
	 {
	   GetDlgItem(IDC_LINE1)->SetWindowText(_T(">16 chars"));
	 } 
   else
	   {
        if (Write_Multi(g_tstat_id,p,MODBUS_LINE1_CHAR1,16))
        {
		   GetDlgItem(IDC_LINE1)->SetWindowText(_T("OK"));
        } 
        else
        {
		   GetDlgItem(IDC_LINE1)->SetWindowText(_T("Error"));
        }
        
       }
}


void CDisplayConfig::OnBnClickedSaveLine2()
{
CString input;
GetDlgItem(IDC_LINE_1)->GetWindowText(input);
input.TrimRight();
CString input2;
GetDlgItem(IDC_LINE_2)->GetWindowText(input2);
input2.TrimRight();

unsigned char p[16];//input+input1
//input
for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
	if(i<input.GetLength())
		p[i]=input.GetAt(i);
	else
		p[i]=' ';
	}
//input1
for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
	if(i<input2.GetLength())
		p[i+THE_CHAR_LENGTH]=input2.GetAt(i);
	else
		p[i+THE_CHAR_LENGTH]=' ';
	}
if (input.GetLength()>16)
	{
	GetDlgItem(IDC_LINE1)->SetWindowText(_T(">16 chars"));
	} 
else
	{
	if (Write_Multi(g_tstat_id,p,MODBUS_LINE1_CHAR1,16))
		{
		GetDlgItem(IDC_LINE2)->SetWindowText(_T("OK"));
		} 
	else
		{
		GetDlgItem(IDC_LINE2)->SetWindowText(_T("Error"));
		}

	}
}

void CDisplayConfig::OnBnClickedSave()
{
m_progress.ShowWindow(TRUE);
m_progress.SetRange(0,100);
m_progress.SetPos(0);
 OnBnClickedSave2();
 m_progress.SetPos(10);
 OnBnClickedSaveLine2();
 m_progress.SetPos(20);
 OnBnClickedSaveInput1();
 m_progress.SetPos(30);
 OnBnClickedSaveInput2();
 m_progress.SetPos(40);
 OnBnClickedSaveInput3();
 m_progress.SetPos(50);
 OnBnClickedSaveInput4();
 m_progress.SetPos(60);
 OnBnClickedSaveInput5();
 m_progress.SetPos(70);
 OnBnClickedSaveInput6();
 m_progress.SetPos(80);
 OnBnClickedSaveInput7();
 m_progress.SetPos(90);
 OnBnClickedSaveInput8();
 m_progress.SetPos(100);
 m_progress.ShowWindow(FALSE);


}


void CDisplayConfig::OnBnClickedSaveInput1()
{
CString input;
GetDlgItem(IDC_INPUT_1)->GetWindowText(input);
input.TrimRight();
unsigned char p[8];//input+input1
//input
for(int i=0;i<THE_CHAR_LENGTH;i++)
{
	if(i<input.GetLength())
		p[i]=input.GetAt(i);
	else
		p[i]=' ';
}
if (input.GetLength()>8)
	{
	GetDlgItem(IDC_INPUT1)->SetWindowText(_T(">8 chars"));
	} 
else
	{
	if (Write_Multi(g_tstat_id,p,MODBUS_AI1_CHAR1,8))
		{
		GetDlgItem(IDC_INPUT1)->SetWindowText(_T("OK"));
		} 
	else
		{
		GetDlgItem(IDC_INPUT1)->SetWindowText(_T("Error"));
		}

	}

}


void CDisplayConfig::OnBnClickedSaveInput2()
	{
	CString input;
	GetDlgItem(IDC_INPUT_2)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
		{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
		}
	if (input.GetLength()>8)
		{
		GetDlgItem(IDC_INPUT2)->SetWindowText(_T(">8 chars"));
		} 
	else
		{
		if (Write_Multi(g_tstat_id,p,MODBUS_AI2_CHAR1,8))
			{
			GetDlgItem(IDC_INPUT2)->SetWindowText(_T("OK"));
			} 
		else
			{
			GetDlgItem(IDC_INPUT2)->SetWindowText(_T("Error"));
			}

		}
	}


void CDisplayConfig::OnBnClickedSaveInput3()
	{
	CString input;
	GetDlgItem(IDC_INPUT_3)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
		{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
		}
	if (input.GetLength()>8)
		{
		GetDlgItem(IDC_INPUT3)->SetWindowText(_T(">8 chars"));
		} 
	else
		{
		if (Write_Multi(g_tstat_id,p,MODBUS_AI3_CHAR1,8))
			{
			GetDlgItem(IDC_INPUT3)->SetWindowText(_T("OK"));
			} 
		else
			{
			GetDlgItem(IDC_INPUT3)->SetWindowText(_T("Error"));
			}

		}
	}


void CDisplayConfig::OnBnClickedSaveInput4()
	{
	CString input;
	GetDlgItem(IDC_INPUT_4)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
		{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
		}
	if (input.GetLength()>8)
		{
		GetDlgItem(IDC_INPUT4)->SetWindowText(_T(">8 chars"));
		} 
	else
		{
		if (Write_Multi(g_tstat_id,p,MODBUS_AI4_CHAR1,8))
			{
			GetDlgItem(IDC_INPUT4)->SetWindowText(_T("OK"));
			} 
		else
			{
			GetDlgItem(IDC_INPUT4)->SetWindowText(_T("Error"));
			}

		}
	}


void CDisplayConfig::OnBnClickedSaveInput5()
	{
	CString input;
	GetDlgItem(IDC_INPUT_5)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
		{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
		}
	if (input.GetLength()>8)
		{
		GetDlgItem(IDC_INPUT5)->SetWindowText(_T(">8 chars"));
		} 
	else
		{
		if (Write_Multi(g_tstat_id,p,MODBUS_AI5_CHAR1,8))
			{
			GetDlgItem(IDC_INPUT5)->SetWindowText(_T("OK"));
			} 
		else
			{
			GetDlgItem(IDC_INPUT5)->SetWindowText(_T("Error"));
			}

		}
	}


void CDisplayConfig::OnBnClickedSaveInput6()
	{
	CString input;
	GetDlgItem(IDC_INPUT_6)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
		{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
		}
	if (input.GetLength()>8)
		{
		GetDlgItem(IDC_INPUT6)->SetWindowText(_T(">8 chars"));
		} 
	else
		{
		if (Write_Multi(g_tstat_id,p,MODBUS_AI6_CHAR1,8))
			{
			GetDlgItem(IDC_INPUT6)->SetWindowText(_T("OK"));
			} 
		else
			{
			GetDlgItem(IDC_INPUT6)->SetWindowText(_T("Error"));
			}

		}
	}


void CDisplayConfig::OnBnClickedSaveInput7()
	{
	CString input;
	GetDlgItem(IDC_INPUT_7)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
		{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
		}
	if (input.GetLength()>8)
		{
		GetDlgItem(IDC_INPUT7)->SetWindowText(_T(">8 chars"));
		} 
	else
		{
		if (Write_Multi(g_tstat_id,p,MODBUS_AI7_CHAR1,8))
			{
			GetDlgItem(IDC_INPUT7)->SetWindowText(_T("OK"));
			} 
		else
			{
			GetDlgItem(IDC_INPUT7)->SetWindowText(_T("Error"));
			}

		}
	}


void CDisplayConfig::OnBnClickedSaveInput8()
	{
	CString input;
	GetDlgItem(IDC_INPUT_8)->GetWindowText(input);
	input.TrimRight();
	unsigned char p[8];//input+input1
	//input
	for(int i=0;i<THE_CHAR_LENGTH;i++)
		{
		if(i<input.GetLength())
			p[i]=input.GetAt(i);
		else
			p[i]=' ';
		}
	if (input.GetLength()>8)
		{
		GetDlgItem(IDC_INPUT8)->SetWindowText(_T(">8 chars"));
		} 
	else
		{
		if (Write_Multi(g_tstat_id,p,MODBUS_AI8_CHAR1,8))
			{
			GetDlgItem(IDC_INPUT8)->SetWindowText(_T("OK"));
			} 
		else
			{
			GetDlgItem(IDC_INPUT8)->SetWindowText(_T("Error"));
			}

		}

	}


void CDisplayConfig::OnBnClickedFreshTable()
	{
	// TODO: Add your control notification handler code here
	    m_progress.ShowWindow(TRUE);
		m_progress.SetRange(0,10);
		register_critical_section.Lock();
		for(int i=0;i<10;i++) //Modify by Fance , tstat 6 has more register than 512;
		{
			if(g_tstat_id_changed)
			{
				continue;
			}
			if(g_bPauseMultiRead)
			{
				continue;
			}
			Read_Multi(g_tstat_id,&multi_register_value[i*64],i*64,64);	
			m_progress.SetPos(i);
		}
		register_critical_section.Unlock();
		m_progress.ShowWindow(FALSE);
		memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
	Fresh_Grid();
	}


 
