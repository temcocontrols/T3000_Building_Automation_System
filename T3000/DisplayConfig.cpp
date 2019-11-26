// DisplayConfig.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "DisplayConfig.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "MainFrm.h"
#include <bitset>
// CDisplayConfig dialog
#define  THE_CHAR_LENGTH 8

IMPLEMENT_DYNAMIC(CDisplayConfig, CDialog)
CDisplayConfig::CDisplayConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDisplayConfig::IDD, pParent)
{
m_EDIT_ID=-1;
/*m_display_number=15;*/
}

CDisplayConfig::~CDisplayConfig()
{
}
BOOL CDisplayConfig::OnInitDialog()
{
	CDialog::OnInitDialog();
    if (product_register_value[7]==PM_TSTAT5E||product_register_value[7] == PM_PM5E || product_register_value[7] == PM_PM5E_ARM)
    {
        MODBUS_LINE1_CHAR1=MODBUS_UI_LINE1_CHAR1;
        MODBUS_LINE2_CHAR1=MODBUS_UI_LINE2_CHAR1;
    }
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

	Fresh_Checks();
	return TRUE;
}
void CDisplayConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_MSFLEXGRID, m_FlexGrid1);
	DDX_Text(pDX, IDC_INPUT_9, m_display_number);
	DDX_Control(pDX, IDC_SEQ_COM, m_ItemValueCombx);
	DDX_Control(pDX, IDC_PROCESS, m_progress);
	DDX_Control(pDX, IDC_CHECK_ICON_AUTO, m_am_check);
    DDX_Control(pDX, IDC_CHECK_ICON_MANUAL, m_man_check);
    DDX_Control (pDX,IDC_CHECK_ICON_OUTPUT,m_output_check);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK3, m_check3);
	DDX_Control(pDX, IDC_CHECK4, m_check4);
	DDX_Control(pDX, IDC_CHECK5, m_check5);
	DDX_Control(pDX, IDC_CHECK6, m_check6);
	DDX_Control(pDX, IDC_CHECK7, m_check7);
	DDX_Control(pDX, IDC_CHECK8, m_check8);


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
ON_EN_SETFOCUS(IDC_INPUT_1, &CDisplayConfig::OnSetfocusInput1)
ON_EN_SETFOCUS(IDC_INPUT_2, &CDisplayConfig::OnSetfocusInput2)
ON_EN_SETFOCUS(IDC_INPUT_3, &CDisplayConfig::OnSetfocusInput3)
ON_EN_SETFOCUS(IDC_INPUT_4, &CDisplayConfig::OnSetfocusInput4)
ON_EN_SETFOCUS(IDC_INPUT_5, &CDisplayConfig::OnSetfocusInput5)
ON_EN_SETFOCUS(IDC_INPUT_6, &CDisplayConfig::OnSetfocusInput6)
ON_EN_SETFOCUS(IDC_INPUT_7, &CDisplayConfig::OnSetfocusInput7)
ON_EN_SETFOCUS(IDC_INPUT_8, &CDisplayConfig::OnSetfocusInput8)
ON_EN_SETFOCUS(IDC_LINE_1, &CDisplayConfig::OnSetfocusLine1)
ON_EN_SETFOCUS(IDC_LINE_2, &CDisplayConfig::OnSetfocusLine2)
ON_BN_CLICKED(IDC_CHECK_ICON_AUTO, &CDisplayConfig::OnBnClickedCheckIconAm)
ON_BN_CLICKED(IDC_CHECK_ICON_MANUAL, &CDisplayConfig::OnBnClickedCheckIconMan)
ON_BN_CLICKED(IDC_CHECK2, &CDisplayConfig::OnBnClickedCheck2)
ON_BN_CLICKED(IDC_CHECK3, &CDisplayConfig::OnBnClickedCheck3)
ON_BN_CLICKED(IDC_CHECK4, &CDisplayConfig::OnBnClickedCheck4)
ON_BN_CLICKED(IDC_CHECK5, &CDisplayConfig::OnBnClickedCheck5)
ON_BN_CLICKED(IDC_CHECK6, &CDisplayConfig::OnBnClickedCheck6)
ON_BN_CLICKED(IDC_CHECK7, &CDisplayConfig::OnBnClickedCheck7)
ON_BN_CLICKED(IDC_CHECK8, &CDisplayConfig::OnBnClickedCheck8)
ON_EN_KILLFOCUS(IDC_LINE_1, &CDisplayConfig::OnEnKillfocusLine1)
ON_EN_KILLFOCUS(IDC_LINE_2, &CDisplayConfig::OnEnKillfocusLine2)
ON_BN_CLICKED(IDC_CHECK_ICON_OUTPUT, &CDisplayConfig::OnBnClickedCheckIconOutput)
ON_BN_CLICKED(IDC_CHECK_H0, &CDisplayConfig::OnBnClickedCheckH0)
ON_BN_CLICKED(IDC_CHECK_C0, &CDisplayConfig::OnClickedCheckC0)
ON_BN_CLICKED(IDC_CHECK_C1, &CDisplayConfig::OnClickedCheckC1)
ON_BN_CLICKED(IDC_CHECK_C2, &CDisplayConfig::OnClickedCheckC2)
ON_BN_CLICKED(IDC_CHECK_C3, &CDisplayConfig::OnClickedCheckC3)
ON_BN_CLICKED(IDC_CHECK_C4, &CDisplayConfig::OnClickedCheckC4)
ON_BN_CLICKED(IDC_CHECK_C5, &CDisplayConfig::OnClickedCheckC5)
ON_BN_CLICKED(IDC_CHECK_C6, &CDisplayConfig::OnClickedCheckC6)
ON_BN_CLICKED(IDC_CHECK_H1, &CDisplayConfig::OnClickedCheckH1)
ON_BN_CLICKED(IDC_CHECK_H2, &CDisplayConfig::OnClickedCheckH2)
ON_BN_CLICKED(IDC_CHECK_H3, &CDisplayConfig::OnClickedCheckH3)
ON_BN_CLICKED(IDC_CHECK_H4, &CDisplayConfig::OnClickedCheckH4)
ON_BN_CLICKED(IDC_CHECK_H5, &CDisplayConfig::OnClickedCheckH5)
ON_BN_CLICKED(IDC_CHECK_H6, &CDisplayConfig::OnClickedCheckH6)
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
		selection=g_strInName1;
		break;
	case 4:
		selection=g_strInName2;
		break;
	case 5:
		selection=g_strInName3;
		break;
	case 6:
		selection=g_strInName4;
		break;
	case 7:
		selection=g_strInName5;
		break;
	case 8:
		selection=g_strInName6;
		break;
	case 9:
		selection=g_strInName7;
		break;
	case 10:
		selection=g_strInName8;
		break;
	case 11:
		selection=_T("MODE");
		break;
	case 12:
		selection=_T("Custom Message");
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
        bool is_user_infor=false;
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
                if (product_register_value[MODBUS_DISP_ITEM_TEMPERATURE+i-1]==12)
                {
                     is_user_infor = true;
                }
			}

            if (is_user_infor)
            {
                GetDlgItem(IDC_STATIC_USER_INFOR)->ShowWindow(TRUE);
                GetDlgItem(IDC_STATIC_LINE1)->ShowWindow(TRUE);
                GetDlgItem(IDC_STATIC_LINE2)->ShowWindow(TRUE);
                GetDlgItem(IDC_LINE_1)->ShowWindow(TRUE);
                GetDlgItem(IDC_LINE_2)->ShowWindow(TRUE);
            }
            else
            {
                GetDlgItem(IDC_STATIC_USER_INFOR)->ShowWindow(FALSE);
                GetDlgItem(IDC_STATIC_LINE1)->ShowWindow(FALSE);
                GetDlgItem(IDC_STATIC_LINE2)->ShowWindow(FALSE);
                GetDlgItem(IDC_LINE_1)->ShowWindow(FALSE);
                GetDlgItem(IDC_LINE_2)->ShowWindow(FALSE);
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
		m_ItemValueCombx.AddString(g_strInName1);
		m_ItemValueCombx.AddString(g_strInName2);
		m_ItemValueCombx.AddString(g_strInName3);
		m_ItemValueCombx.AddString(g_strInName4);
		m_ItemValueCombx.AddString(g_strInName5);
		m_ItemValueCombx.AddString(g_strInName6);
		m_ItemValueCombx.AddString(g_strInName7);
		m_ItemValueCombx.AddString(g_strInName8);
		m_ItemValueCombx.AddString(_T("MODE"));
		m_ItemValueCombx.AddString(_T("Custom Message"));
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

CDialog::OnCancel();
}
void CDisplayConfig::OnBnClickedOk()
	{ 
  CString temp;
 	 
//
 	 int reg=MODBUS_LCD_ROTATE_ENABLE; 
 	 int m_display_number=product_register_value[reg];
//
//	 
 	GetDlgItem(IDC_LINE_1)->SetWindowText(GetTextReg(MODBUS_LINE1_CHAR1));
	
 	GetDlgItem(IDC_LINE_2)->SetWindowText(GetTextReg(MODBUS_LINE2_CHAR1)); 

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
		   p[i]=0;
	   }
	   //input1
   for(int i=0;i<THE_CHAR_LENGTH;i++)
	   {
	   if(i<input2.GetLength())
		   p[i+THE_CHAR_LENGTH]=input2.GetAt(i);
	   else
		   p[i+THE_CHAR_LENGTH]=0;
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
		p[i]=0;
	}
//input1
for(int i=0;i<THE_CHAR_LENGTH;i++)
	{
	if(i<input2.GetLength())
		p[i+THE_CHAR_LENGTH]=input2.GetAt(i);
	else
		p[i+THE_CHAR_LENGTH]=0;
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
 //OnBnClickedSaveInput1();
 m_progress.SetPos(30);
 //OnBnClickedSaveInput2();
 m_progress.SetPos(40);
 //OnBnClickedSaveInput3();
 m_progress.SetPos(50);
 //OnBnClickedSaveInput4();
 m_progress.SetPos(60);
 //OnBnClickedSaveInput5();
 m_progress.SetPos(70);
 //OnBnClickedSaveInput6();
 m_progress.SetPos(80);
// OnBnClickedSaveInput7();
 m_progress.SetPos(90);
// OnBnClickedSaveInput8();
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
		p[i]=0;
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
			p[i]=0;
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
			p[i]=0;
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
			p[i]=0;
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
			p[i]=0;
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
			p[i]=0;
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
			p[i]=0;
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
			p[i]=0;
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
			Read_Multi(g_tstat_id,&multi_register_value[i*100],i*100,100);	
			m_progress.SetPos(i);
		}
		register_critical_section.Unlock();
		m_progress.ShowWindow(FALSE);
		memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
	Fresh_Grid();
	}


 


BOOL CDisplayConfig::PreTranslateMessage(MSG* pMsg)
{
	
	if(WM_KEYDOWN == pMsg->message)
	{

		CEdit* pEdit = (CEdit*)GetDlgItem(m_EDIT_ID);

		ASSERT(pEdit);

		if(pMsg->hwnd == pEdit->GetSafeHwnd() && VK_RETURN == pMsg->wParam)
		{
			switch (m_EDIT_ID)
			{
			  case IDC_INPUT_1:{
			  OnBnClickedSaveInput1();
			  }break;
			  case IDC_INPUT_2:{
			  OnBnClickedSaveInput2();
			  }break;
			  case IDC_INPUT_3:{
			  OnBnClickedSaveInput3();
			 }break;
			  case IDC_INPUT_4:{
			  OnBnClickedSaveInput4();
			 }break;
			  case IDC_INPUT_5:{
			  OnBnClickedSaveInput5();
			 }break;
			  case IDC_INPUT_6:{
			  OnBnClickedSaveInput6();
			 }break;
			  case IDC_INPUT_7:{
			  OnBnClickedSaveInput7();
			 }break;
			  case IDC_INPUT_8:{
			  OnBnClickedSaveInput8();
			 }break;
			 case IDC_LINE_1:{
			 OnBnClickedSaveLine2();
			 }break;
			 case IDC_LINE_2:{
			 OnBnClickedSaveLine2();
			 }break;
			}
        }
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CDisplayConfig::OnSetfocusInput1()
{
	m_EDIT_ID=IDC_INPUT_1; 
}


void CDisplayConfig::OnSetfocusInput2()
{
	m_EDIT_ID=IDC_INPUT_2; 
}


void CDisplayConfig::OnSetfocusInput3()
{
	m_EDIT_ID=IDC_INPUT_3; 
}


void CDisplayConfig::OnSetfocusInput4()
{
	m_EDIT_ID=IDC_INPUT_4; 
}


void CDisplayConfig::OnSetfocusInput5()
{
	m_EDIT_ID=IDC_INPUT_5; 
}


void CDisplayConfig::OnSetfocusInput6()
{
	m_EDIT_ID=IDC_INPUT_6; 
}


void CDisplayConfig::OnSetfocusInput7()
{
	m_EDIT_ID=IDC_INPUT_7; 
}


void CDisplayConfig::OnSetfocusInput8()
{
	m_EDIT_ID=IDC_INPUT_8; 
}


void CDisplayConfig::OnSetfocusLine1()
{	
	m_EDIT_ID=IDC_LINE_1;
}


void CDisplayConfig::OnSetfocusLine2()
{
	m_EDIT_ID=IDC_LINE_2;
}
void CDisplayConfig::Fresh_Checks(){
	int AM=product_register_value[728];
	if (AM==0)
	{
	  m_am_check.SetCheck(1);
      m_man_check.SetCheck(0);
      m_output_check.SetCheck (0);
	  GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
	  GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
	  GetDlgItem(IDC_CHECK4)->EnableWindow(FALSE);
	  GetDlgItem(IDC_CHECK5)->EnableWindow(FALSE);
	  GetDlgItem(IDC_CHECK6)->EnableWindow(FALSE);
	  GetDlgItem(IDC_CHECK7)->EnableWindow(FALSE);
	  GetDlgItem(IDC_CHECK8)->EnableWindow(FALSE);
	  
      GetDlgItem(IDC_CHECK_H0)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_H1)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_H2)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_H3)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_H4)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_H5)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_H6)->EnableWindow(FALSE);

      GetDlgItem(IDC_CHECK_C0)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_C1)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_C2)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_C3)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_C4)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_C5)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_C6)->EnableWindow(FALSE);
	}
	else if(AM == 1)
	{
	   m_am_check.SetCheck(0);
       m_man_check.SetCheck(1);
       m_output_check.SetCheck (0);
	  GetDlgItem(IDC_CHECK2)->EnableWindow(TRUE);
	  GetDlgItem(IDC_CHECK3)->EnableWindow(TRUE);
	  GetDlgItem(IDC_CHECK4)->EnableWindow(TRUE);
	  GetDlgItem(IDC_CHECK5)->EnableWindow(TRUE);
	  GetDlgItem(IDC_CHECK6)->EnableWindow(TRUE);
	  GetDlgItem(IDC_CHECK7)->EnableWindow(TRUE);
	  GetDlgItem(IDC_CHECK8)->EnableWindow(TRUE);


      GetDlgItem(IDC_CHECK_H0)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_H1)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_H2)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_H3)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_H4)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_H5)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_H6)->EnableWindow(FALSE);

      GetDlgItem(IDC_CHECK_C0)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_C1)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_C2)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_C3)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_C4)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_C5)->EnableWindow(FALSE);
      GetDlgItem(IDC_CHECK_C6)->EnableWindow(FALSE);

	}
	else{
        m_am_check.SetCheck(0);
        m_man_check.SetCheck(0);
        m_output_check.SetCheck (1);
        GetDlgItem(IDC_CHECK2)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK4)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK5)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK6)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK7)->EnableWindow(FALSE);
        GetDlgItem(IDC_CHECK8)->EnableWindow(FALSE);


        GetDlgItem(IDC_CHECK_H0)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_H1)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_H2)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_H3)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_H4)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_H5)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_H6)->EnableWindow(TRUE);

        GetDlgItem(IDC_CHECK_C0)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_C1)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_C2)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_C3)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_C4)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_C5)->EnableWindow(TRUE);
        GetDlgItem(IDC_CHECK_C6)->EnableWindow(TRUE);
    }
    bitset<16> IconValue(product_register_value[729]);
	m_check2.SetCheck(IconValue[0]);
	m_check3.SetCheck(IconValue[1]);
	m_check4.SetCheck(IconValue[2]);
	m_check5.SetCheck(IconValue[3]);
	m_check6.SetCheck(IconValue[4]);
	m_check7.SetCheck(IconValue[5]);
	m_check8.SetCheck(IconValue[6]);

    bitset<16> IcoHeat(product_register_value[765]);
    ((CButton*)GetDlgItem(IDC_CHECK_H0))->SetCheck (IcoHeat[0]);
    ((CButton*)GetDlgItem(IDC_CHECK_H1))->SetCheck (IcoHeat[1]);
    ((CButton*)GetDlgItem(IDC_CHECK_H2))->SetCheck (IcoHeat[2]);
    ((CButton*)GetDlgItem(IDC_CHECK_H3))->SetCheck (IcoHeat[3]);
    ((CButton*)GetDlgItem(IDC_CHECK_H4))->SetCheck (IcoHeat[4]);
    ((CButton*)GetDlgItem(IDC_CHECK_H5))->SetCheck (IcoHeat[5]);
    ((CButton*)GetDlgItem(IDC_CHECK_H6))->SetCheck (IcoHeat[6]);
    bitset<16> IcoCool(product_register_value[766]);
    ((CButton*)GetDlgItem(IDC_CHECK_C0))->SetCheck (IcoCool[0]);
    ((CButton*)GetDlgItem(IDC_CHECK_C1))->SetCheck (IcoCool[1]);
    ((CButton*)GetDlgItem(IDC_CHECK_C2))->SetCheck (IcoCool[2]);
    ((CButton*)GetDlgItem(IDC_CHECK_C3))->SetCheck (IcoCool[3]);
    ((CButton*)GetDlgItem(IDC_CHECK_C4))->SetCheck (IcoCool[4]);
    ((CButton*)GetDlgItem(IDC_CHECK_C5))->SetCheck (IcoCool[5]);
    ((CButton*)GetDlgItem(IDC_CHECK_C6))->SetCheck (IcoCool[6]);
}


void CDisplayConfig::Write_Setting(){
	bitset<16> IconValue(product_register_value[729]);
	IconValue.set(0,m_check2.GetCheck());
	IconValue.set(1,m_check3.GetCheck());
	IconValue.set(2,m_check4.GetCheck());
	IconValue.set(3,m_check5.GetCheck());
	IconValue.set(4,m_check6.GetCheck());
	IconValue.set(5,m_check7.GetCheck());
	IconValue.set(6,m_check8.GetCheck());
	int Reg729=IconValue.to_ulong();
	int ret=write_one(g_tstat_id,729,Reg729);
	if (ret>0)
	{
	 product_register_value[729] = Reg729;
	}
	else
	{
	AfxMessageBox(_T("Write Setting Fail"));
	}

	Fresh_Checks();
}
void CDisplayConfig::OnBnClickedCheck2()
{
	Write_Setting();
}


void CDisplayConfig::OnBnClickedCheck3()
{
	Write_Setting();
}


void CDisplayConfig::OnBnClickedCheck4()
{
	Write_Setting();
}


void CDisplayConfig::OnBnClickedCheck5()
{
	Write_Setting();
}


void CDisplayConfig::OnBnClickedCheck6()
{
	Write_Setting();
}


void CDisplayConfig::OnBnClickedCheck7()
{
	Write_Setting();
}


void CDisplayConfig::OnBnClickedCheck8()
{
	Write_Setting();
}


void CDisplayConfig::OnEnKillfocusLine1()
{
      OnBnClickedSaveLine2();
}


void CDisplayConfig::OnEnKillfocusLine2()
{
    OnBnClickedSaveLine2();
}
void CDisplayConfig::OnBnClickedCheckIconAm()
{
    int AM=product_register_value[728];
    if (AM==1||AM==2)
    {
        int ret=write_one(g_tstat_id,728,0);
        if (ret>0)
        {
            product_register_value[728]=0;
        }
        else
        {
            AfxMessageBox(_T("Try again"));
        }
        Fresh_Checks();
    } 
}
void CDisplayConfig::OnBnClickedCheckIconMan()
{
    int AM=product_register_value[728];
    if (AM==0||AM==2)
    {
        int ret=write_one(g_tstat_id,728,1);
        if (ret>0)
        {
            product_register_value[728]=1;
        }
        else
        {
            AfxMessageBox(_T("Try again"));
        }
        Fresh_Checks();
    }   
}

void CDisplayConfig::OnBnClickedCheckIconOutput()
{
    int AM=product_register_value[728];
    if (AM==0||AM==1)
    {
        int ret=write_one(g_tstat_id,728,2);
        if (ret>0)
        {
            product_register_value[728]=2;
        }
        else
        {
            AfxMessageBox(_T("Try again"));
        }
        Fresh_Checks();
    } 
}


void CDisplayConfig::OnBnClickedCheckH0()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H0))->SetCheck (1);
    ((CButton*)GetDlgItem(IDC_CHECK_C0))->SetCheck (0);
     SaveOutputSetting();   
}
void CDisplayConfig::SaveOutputSetting ()
{
    bitset<16> IcoHeat(product_register_value[765]);
    IcoHeat[0]=((CButton*)GetDlgItem(IDC_CHECK_H0))->GetCheck ();
    IcoHeat[1]=((CButton*)GetDlgItem(IDC_CHECK_H1))->GetCheck ();
    IcoHeat[2]=((CButton*)GetDlgItem(IDC_CHECK_H2))->GetCheck ();
    IcoHeat[3]=((CButton*)GetDlgItem(IDC_CHECK_H3))->GetCheck ();
    IcoHeat[4]=((CButton*)GetDlgItem(IDC_CHECK_H4))->GetCheck ();
    IcoHeat[5]=((CButton*)GetDlgItem(IDC_CHECK_H5))->GetCheck ();
    IcoHeat[6]=((CButton*)GetDlgItem(IDC_CHECK_H6))->GetCheck ();

    bitset<16> IcoCool(product_register_value[766]);
    IcoCool[0]=((CButton*)GetDlgItem(IDC_CHECK_C0))->GetCheck ();
    IcoCool[1]=((CButton*)GetDlgItem(IDC_CHECK_C1))->GetCheck ();
    IcoCool[2]=((CButton*)GetDlgItem(IDC_CHECK_C2))->GetCheck ();
    IcoCool[3]=((CButton*)GetDlgItem(IDC_CHECK_C3))->GetCheck ();
    IcoCool[4]=((CButton*)GetDlgItem(IDC_CHECK_C4))->GetCheck ();
    IcoCool[5]=((CButton*)GetDlgItem(IDC_CHECK_C5))->GetCheck ();
    IcoCool[6]=((CButton*)GetDlgItem(IDC_CHECK_C6))->GetCheck ();

    write_one (g_tstat_id,765,IcoHeat.to_ulong ());
    write_one (g_tstat_id,766,IcoCool.to_ulong ());

}


void CDisplayConfig::OnClickedCheckC0()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H0))->SetCheck (0);
    ((CButton*)GetDlgItem(IDC_CHECK_C0))->SetCheck (1);
    SaveOutputSetting();
}


void CDisplayConfig::OnClickedCheckC1()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H1))->SetCheck (0);
    ((CButton*)GetDlgItem(IDC_CHECK_C1))->SetCheck (1);
    SaveOutputSetting();
}


void CDisplayConfig::OnClickedCheckC2()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H2))->SetCheck (0);
    ((CButton*)GetDlgItem(IDC_CHECK_C2))->SetCheck (1);
    SaveOutputSetting();
}


void CDisplayConfig::OnClickedCheckC3()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H3))->SetCheck (0);
    ((CButton*)GetDlgItem(IDC_CHECK_C3))->SetCheck (1);
    SaveOutputSetting();
}


void CDisplayConfig::OnClickedCheckC4()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H4))->SetCheck (0);
    ((CButton*)GetDlgItem(IDC_CHECK_C4))->SetCheck (1);
    SaveOutputSetting();
}


void CDisplayConfig::OnClickedCheckC5()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H5))->SetCheck (0);
    ((CButton*)GetDlgItem(IDC_CHECK_C5))->SetCheck (1);
    SaveOutputSetting();
}


void CDisplayConfig::OnClickedCheckC6()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H6))->SetCheck (0);
    ((CButton*)GetDlgItem(IDC_CHECK_C6))->SetCheck (1);
    SaveOutputSetting();
}


void CDisplayConfig::OnClickedCheckH1()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H1))->SetCheck (1);
    ((CButton*)GetDlgItem(IDC_CHECK_C1))->SetCheck (0);
    SaveOutputSetting(); 
}


void CDisplayConfig::OnClickedCheckH2()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H2))->SetCheck (1);
    ((CButton*)GetDlgItem(IDC_CHECK_C2))->SetCheck (0);
    SaveOutputSetting(); 
}


void CDisplayConfig::OnClickedCheckH3()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H3))->SetCheck (1);
    ((CButton*)GetDlgItem(IDC_CHECK_C3))->SetCheck (0);
    SaveOutputSetting(); 
}


void CDisplayConfig::OnClickedCheckH4()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H4))->SetCheck (1);
    ((CButton*)GetDlgItem(IDC_CHECK_C4))->SetCheck (0);
    SaveOutputSetting(); 
}


void CDisplayConfig::OnClickedCheckH5()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H5))->SetCheck (1);
    ((CButton*)GetDlgItem(IDC_CHECK_C5))->SetCheck (0);
    SaveOutputSetting(); 
}


void CDisplayConfig::OnClickedCheckH6()
{
    ((CButton*)GetDlgItem(IDC_CHECK_H6))->SetCheck (1);
    ((CButton*)GetDlgItem(IDC_CHECK_C6))->SetCheck (0);
    SaveOutputSetting(); 
}
