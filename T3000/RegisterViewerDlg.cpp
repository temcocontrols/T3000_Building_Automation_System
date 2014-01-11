// RegisterViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "RegisterViewerDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ado/ADO.h"
#include "globle_function.h"
#include "TemcoDataGrid.h"

#define BACK_CELL RGB(139,137,137)
#define DIMGREY RGB(176,196,222)
#define WRITE_COLOR RGB(255,255,255)
 

#define SECTION _T("CRegisterViewerDlg_REGISTERLISTGIRD")
UINT BackRegisterViewerFreshProc(LPVOID pParam);
// CRegisterViewerDlg dialog
#define ROWS 50
#define COLS 7
IMPLEMENT_DYNAMIC(CRegisterViewerDlg, CDialog)

CRegisterViewerDlg::CRegisterViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegisterViewerDlg::IDD, pParent)
{
	  m_recordcount=0;
	  m_Datapages=1;
	  m_curPage=1;
	  m_CanEdit=FALSE;
}

CRegisterViewerDlg::~CRegisterViewerDlg()
{
}

void CRegisterViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_MSFLEXGRID, m_registerlistGrid);
	DDX_Control(pDX, IDC_COLNAME, m_ColName);
	DDX_Control(pDX, IDC_CELLEDIT, m_inNameEdt);
}


BEGIN_MESSAGE_MAP(CRegisterViewerDlg, CDialog)
	ON_BN_CLICKED(IDC_LEFT, &CRegisterViewerDlg::OnBnClickedLeft)
	ON_BN_CLICKED(IDC_RIGHT, &CRegisterViewerDlg::OnBnClickedRight)
	ON_BN_CLICKED(IDC_BEGIN, &CRegisterViewerDlg::OnBnClickedBegin)
	ON_BN_CLICKED(IDC_END, &CRegisterViewerDlg::OnBnClickedEnd)
	ON_WM_CTLCOLOR()
//	ON_BN_CLICKED(IDC_ADDNEWPRODUCT, &CRegisterViewerDlg::OnBnClickedAddnewproduct)
ON_COMMAND(ID_MAIN_NEW, &CRegisterViewerDlg::OnMainNew)
//ON_WM_MOUSEHWHEEL()
ON_WM_MOUSEWHEEL()
ON_EN_KILLFOCUS(IDC_CELLEDIT, &CRegisterViewerDlg::OnEnKillfocusCelledit)
ON_BN_CLICKED(IDC_SAVETODB, &CRegisterViewerDlg::OnBnClickedSavetodb)
ON_BN_CLICKED(IDOK, &CRegisterViewerDlg::OnBnClickedOk)
ON_WM_DESTROY()
END_MESSAGE_MAP()

BOOL CRegisterViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
//	m_registerlistGrid.put_BackColorSel(SINGLE_CLICKED);
	Fresh_GridCol();
	LoadDataFromDB();
	m_curPage=1;
	Show_ColorData();

	
	GetDlgItem(IDC_STATIC_PRODUCTNAME)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_PRODUCTNAME)->ShowWindow(FALSE);
	GetDlgItem(IDC_SAVETODB)->ShowWindow(FALSE);
	GetDlgItem(IDC_PRODUCTNAME)->ShowWindow(TRUE);
	GetDlgItem(IDC_PRODUCTNAME_T)->ShowWindow(TRUE);
	GetDlgItem(IDC_LEFT)->ShowWindow(TRUE);
	GetDlgItem(IDC_BEGIN)->ShowWindow(TRUE);
	GetDlgItem(IDC_RIGHT)->ShowWindow(TRUE);
	GetDlgItem(IDC_END)->ShowWindow(TRUE);
	GetDlgItem(IDC_PAGES)->ShowWindow(TRUE);
	pParamBackFresh=AfxBeginThread(BackRegisterViewerFreshProc,this);
	pParamBackFresh->m_bAutoDelete =FALSE;
	return TRUE; 
}

void CRegisterViewerDlg::LoadDataFromDB()
{_variant_t temp_var;
	CADO ado;
	ado.OnInitADOConn();
	if (ado.IsHaveTable(ado,_T("T3000_Register_Address_By_ID")))
	{  
		CString sql,temp;
		DBRegister tempstruct;
		sql.Format(_T("Select * from T3000_Register_Address_By_ID order by Register_Address"));
		ado.m_pRecordset=ado.OpenRecordset(sql);
		 
		 
		
		while (!ado.m_pRecordset->EndOfFile)//有表但是没有对应序列号的值
		{
		    if (product_type==T3000_5ABCDFG_LED_ADDRESS)
		    {
				tempstruct.Register_Address=ado.m_pRecordset->GetCollect(_T("Register_Address"));
				tempstruct.AddressName=ado.m_pRecordset->GetCollect(_T("TSTAT5_LED_AddressName"));
				tempstruct.DataType=ado.m_pRecordset->GetCollect(_T("TSTAT5_LED_DATATYPE"));
				tempstruct.length=ado.m_pRecordset->GetCollect(_T("TSTAT5_LED_LEN"));
				temp_var=ado.m_pRecordset->GetCollect(_T("TSTAT5_LED_DESCRIPTION"));
				if (temp_var.vt==VT_NULL)
				{
				tempstruct.Description=_T("");
				} 
				else
				{
				tempstruct.Description=temp_var;
				}
				temp_var=ado.m_pRecordset->GetCollect(_T("TSTAT5_LED_Operation"));
				if (temp_var.vt==VT_NULL)
				{
					tempstruct.Description=_T("");
				} 
				else
				{
					tempstruct.Operation=temp_var;
				}
			/*	tempstruct.Description=(CString)ado.m_pRecordset->GetCollect(_T("TSTAT5_LED_DESCRIPTION"));*/
				 
		    } 
		    else if (product_type==T3000_5EH_LCD_ADDRESS)
		    {
			    _variant_t vartemp;
				tempstruct.Register_Address=ado.m_pRecordset->GetCollect(_T("Register_Address"));
				tempstruct.AddressName=ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_AddressName"));
				tempstruct.DataType=ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_DATATYPE"));
				tempstruct.length=ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_LEN"));
				//tempstruct.Description=(CString)ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_DESCRIPTION"));
				 vartemp=ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_DESCRIPTION"));
				if (vartemp.vt==VT_NULL)
				{
				tempstruct.Description=_T("");
				} 
				else
				{
				tempstruct.Description=vartemp;
				} 
				temp_var=ado.m_pRecordset->GetCollect(_T("TSTAT5_LCD_Operation"));
				if (temp_var.vt==VT_NULL)
				{
					tempstruct.Description=_T("");
				} 
				else
				{
					tempstruct.Operation=temp_var;
				}
		    }
			else if (product_type==T3000_6_ADDRESS)
		    {
				tempstruct.Register_Address=ado.m_pRecordset->GetCollect(_T("Register_Address"));
				tempstruct.AddressName=ado.m_pRecordset->GetCollect(_T("TSTAT6_AddressName"));
				tempstruct.DataType=ado.m_pRecordset->GetCollect(_T("TSTAT6_DATATYPE"));
				tempstruct.length=ado.m_pRecordset->GetCollect(_T("TSTAT6_LEN"));
				temp_var=ado.m_pRecordset->GetCollect(_T("TSTAT6_DESCRIPTION")); 
				 if (temp_var.vt==VT_NULL)
				 {
				  tempstruct.Description=_T("");
				 } 
				 else
				 {
				 tempstruct.Description=temp_var;
				 }
				 temp_var=ado.m_pRecordset->GetCollect(_T("TSTAT6_Operation"));
				 if (temp_var.vt==VT_NULL)
				 {
					 tempstruct.Description=_T("");
				 } 
				 else
				 {
					 tempstruct.Operation=temp_var;
				 }
		    }
			if (tempstruct.AddressName.CompareNoCase(_T("RESERVED"))==0)
			{
			ado.m_pRecordset->MoveNext();
			continue;
			}
			m_VecregisterData.push_back(tempstruct);
			m_recordcount++;
			ado.m_pRecordset->MoveNext();
		}
	} 
	ado.CloseRecordset();
	ado.CloseConn();
	CString temp;
	m_Datapages=(m_recordcount/ROWS)+1;
	temp.Format(_T("Product Name"));
	GetDlgItem(IDC_PRODUCTNAME)->SetWindowText(temp);
	
	GetDlgItem(IDC_PRODUCTNAME_T)->SetWindowText(GetProductName(product_register_value[7]));
	temp.Format(_T("%d/%d"),m_curPage,m_Datapages);
	GetDlgItem(IDC_PAGES)->SetWindowText(temp);
}
void CRegisterViewerDlg::ShowCurPage(){
CString temp;
temp.Format(_T("%d/%d"),m_curPage,m_Datapages);
GetDlgItem(IDC_PAGES)->SetWindowText(temp);
 
int startAdd=(m_curPage-1)*ROWS;
int endAdd=m_curPage*ROWS;
if (m_curPage==m_Datapages)
{
endAdd=m_recordcount;//m_recordcount-(m_curPage-1)*ROWS+1;
} 
else
{
  endAdd=m_curPage*ROWS;
}

for (int i=startAdd;i<endAdd;i++)
{
	 
     
    
	temp.Format(_T("%d"),m_VecregisterData[i].Register_Address);  
	m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_regno,temp);

	if (product_register_value[m_VecregisterData[i].Register_Address]==65535)
	{
	temp=_T("0");
	}
	else
	{
	temp.Format(_T("%d"),product_register_value[m_VecregisterData[i].Register_Address]);
	}
	
	m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_value,temp);
	m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_name,m_VecregisterData[i].AddressName);
	m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_type,m_VecregisterData[i].DataType);
	temp.Format(_T("%d"),m_VecregisterData[i].length);
	m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_length,temp);
	m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_description,m_VecregisterData[i].Description);
	m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_Operation,m_VecregisterData[i].Operation);
}

 
}

void CRegisterViewerDlg::Show_ColorData(){
	
	CString temp;
	temp.Format(_T("%d/%d"),m_curPage,m_Datapages);
	GetDlgItem(IDC_PAGES)->SetWindowText(temp);

	int startAdd=(m_curPage-1)*ROWS;
	int endAdd=m_curPage*ROWS;
	if (m_curPage==m_Datapages)
	{
		endAdd=m_recordcount;//m_recordcount-(m_curPage-1)*ROWS+1;
	} 
	else
	{
		endAdd=m_curPage*ROWS;
	}
	for (int i=startAdd;i<endAdd;i++)
	{
		 
			for (int j=0;j<m_registerlistGrid.get_Cols();j++)
			{
				m_registerlistGrid.put_Row(i%ROWS+1);
				m_registerlistGrid.put_Col(j);
				m_registerlistGrid.put_CellBackColor(WRITE_COLOR);
			}
		 
	}
	for (int i=startAdd;i<endAdd;i++)
	{
		if ((m_VecregisterData[i].Operation.CompareNoCase(_T("R"))==0)&&(m_VecregisterData[i].Operation.CompareNoCase(_T("/W"))!=0))
		{
			for (int j=0;j<m_registerlistGrid.get_Cols();j++)
			{
				m_registerlistGrid.put_Row(i%ROWS+1);
				m_registerlistGrid.put_Col(j);
				m_registerlistGrid.put_CellBackColor(DIMGREY);
			}
		} 


		temp.Format(_T("%d"),m_VecregisterData[i].Register_Address);  
		m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_regno,temp);

		if (product_register_value[m_VecregisterData[i].Register_Address]==65535)
		{
			temp=_T("0");
		}
		else
		{
			temp.Format(_T("%d"),product_register_value[m_VecregisterData[i].Register_Address]);
		}

		m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_value,temp);
		m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_name,m_VecregisterData[i].AddressName);
		m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_type,m_VecregisterData[i].DataType);
		temp.Format(_T("%d"),m_VecregisterData[i].length);
		m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_length,temp);
		m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_description,m_VecregisterData[i].Description);
		m_registerlistGrid.put_TextMatrix(i%ROWS+1,m_Operation,m_VecregisterData[i].Operation);
	}

	 
}

void CRegisterViewerDlg::OnBnClickedLeft()
{
	if (m_curPage>1)
	{
	
	--m_curPage;
	Show_ColorData();
	m_registerlistGrid.put_TopRow(0);
	}
}



void CRegisterViewerDlg::OnBnClickedRight()
{
	// TODO: Add your control notification handler code here
	if (m_curPage<m_Datapages)
	{
	  ++m_curPage;
	  Show_ColorData();
	  m_registerlistGrid.put_TopRow(0);
	}
}


void CRegisterViewerDlg::OnBnClickedBegin()
{
	// TODO: Add your control notification handler code here
	m_curPage=1;
	Show_ColorData();
	m_registerlistGrid.put_TopRow(0);
}


void CRegisterViewerDlg::OnBnClickedEnd()
{
	// TODO: Add your control notification handler code here
	m_curPage=m_Datapages;
	Show_ColorData();
	m_registerlistGrid.put_TopRow(0);
}
BEGIN_EVENTSINK_MAP(CRegisterViewerDlg, CDialog)
	ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, DISPID_MOUSEDOWN, CRegisterViewerDlg::MouseDownInputMsflexgrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, DISPID_MOUSEMOVE, CRegisterViewerDlg::MouseMoveInputMsflexgrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, DISPID_MOUSEUP, CRegisterViewerDlg::MouseUpInputMsflexgrid, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, DISPID_CLICK, CRegisterViewerDlg::ClickInputMsflexgrid, VTS_NONE)
//	ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, 73, CRegisterViewerDlg::ScrollInputMsflexgrid, VTS_NONE)
//ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, 73, CRegisterViewerDlg::ScrollInputMsflexgrid, VTS_NONE)
ON_EVENT(CRegisterViewerDlg, IDC_INPUT_MSFLEXGRID, DISPID_DBLCLICK, CRegisterViewerDlg::DblClickInputMsflexgrid, VTS_NONE)
END_EVENTSINK_MAP()


void CRegisterViewerDlg::MouseDownInputMsflexgrid(short Button, short Shift, long x, long y)
{
	// TODO: Add your message handler code here
	if (Button&0x1)
	{
		m_SelCurCol=m_registerlistGrid.get_ColSel();
		m_SelCurRow=m_registerlistGrid.get_RowSel();
		 
		if (m_SelCurRow!=0)
		{
			return;
		}
		m_CurColName=m_registerlistGrid.get_TextMatrix(0,m_SelCurCol);
		m_ColName.SetWindowText(m_CurColName);
		m_ColName.ShowWindow(SW_SHOW);

		m_registerlistGrid.put_BackColorSel(m_registerlistGrid.get_GridColorFixed());
	}
	
}


void CRegisterViewerDlg::MouseMoveInputMsflexgrid(short Button, short Shift, long x, long y)
{
    
 
	 if (Button&0x1) // 1 - vbLeftButton 
	 {
		 if (m_SelCurRow!=0)
		 {
			 return;
		 }
			CRect rect;
			m_registerlistGrid.GetWindowRect(&rect);                 //获取FlexGrid控件的窗口矩形
			ScreenToClient(&rect);                                    //转换为客户区矩形
			CDC* pDC=GetDC();
			//MSFlexGrid 控件的函数的长度单位是“缇(twips)”，需要将其转化为像素，1440 缇 = 1 英寸
			//计算象素点和缇的转换比例
			int nTwipsPerDotX=1440/pDC->GetDeviceCaps(LOGPIXELSX);
			int nTwipsPerDotY=1440/pDC->GetDeviceCaps(LOGPIXELSY);

			//x = m_registerlistGrid.get_ColPos(lCol)/nTwipsPerDotX;
			y = m_registerlistGrid.get_RowPos(0)/nTwipsPerDotY;
			//long x = m_outputname.get_ColPos(lCol)/nTwipsPerDotX;
			int left=m_registerlistGrid.get_CellLeft()/nTwipsPerDotY-1;
			int top=m_registerlistGrid.get_CellTop()/nTwipsPerDotX-1;

			long width = m_registerlistGrid.get_ColWidth(m_SelCurCol)/nTwipsPerDotX+1;
			long height = m_registerlistGrid.get_RowHeight(m_SelCurRow)/nTwipsPerDotY+1;
			//形成选中个所在的矩形区域
			CRect rc(x,y,x+width,y+height);
			//转换成相对对话框的坐标
			rc.OffsetRect(rect.left,rect.top);
			m_ColName.MoveWindow(rc);      
			m_registerlistGrid.put_BackColorSel(m_registerlistGrid.get_GridColor());  
		 
		
	 }
	                     //改变大小并移到选中格位置 
}


void CRegisterViewerDlg::MouseUpInputMsflexgrid(short Button, short Shift, long x, long y)
{
	// TODO: Add your message handler code here
	if (Button&0x1)
	{  if (m_SelCurRow!=0)
	{
		return;
	}
		int row = m_registerlistGrid.get_MouseRow();
		int col = m_registerlistGrid.get_MouseCol();

		m_registerlistGrid.put_ColPosition(m_SelCurCol,col);
		m_registerlistGrid.put_BackColorSel(m_registerlistGrid.get_GridColorFixed());
		m_ColName.ShowWindow(SW_HIDE);
		Write_GridCol();
	}

}

 

void CRegisterViewerDlg::Fresh_GridCol(){
    
     m_registerlistGrid.Clear();
	 m_VecregisterData.clear();
	CString inifilepath=g_strExePth;
	inifilepath+=_T("\\Config\\GRIDCONFIG.ini");
	m_Inifile.SetFileName(inifilepath);
	m_regno=m_Inifile.GetProfileInt(SECTION,_T("RegNo"),0);
	m_value=m_Inifile.GetProfileInt(SECTION,_T("Value"),1);
	m_name=m_Inifile.GetProfileInt(SECTION,_T("Name"),2);
	m_type=m_Inifile.GetProfileInt(SECTION,_T("Data Type"),3);
	m_length=m_Inifile.GetProfileInt(SECTION,_T("Data Length"),4);
	m_description=m_Inifile.GetProfileInt(SECTION,_T("Description"),5);
	m_Operation=m_Inifile.GetProfileInt(SECTION,_T("Operation"),6);

	m_registerlistGrid.put_Rows(ROWS+1);
	m_registerlistGrid.put_Cols(COLS);
	m_registerlistGrid.put_TextMatrix(0,m_regno,_T("RegNo"));
	m_registerlistGrid.put_Row(0);
	m_registerlistGrid.put_Col(m_regno);
	m_registerlistGrid.put_CellBackColor(BACK_CELL);
	 
	m_registerlistGrid.put_TextMatrix(0,m_value,_T("Value"));
	if (m_CanEdit)
	{
	m_registerlistGrid.put_ColWidth(m_value,0);
	}
	
	m_registerlistGrid.put_Row(0);
	m_registerlistGrid.put_Col(m_value);
	m_registerlistGrid.put_CellBackColor(BACK_CELL);

 
	m_registerlistGrid.put_TextMatrix(0,m_name,_T("Name"));
	m_registerlistGrid.put_ColWidth(m_name,3500);
	m_registerlistGrid.put_Row(0);
	m_registerlistGrid.put_Col(m_name);
	m_registerlistGrid.put_CellBackColor(BACK_CELL);

	m_registerlistGrid.put_TextMatrix(0,m_type,_T("Data Type"));
	m_registerlistGrid.put_Row(0);
	m_registerlistGrid.put_Col(m_type);
	m_registerlistGrid.put_CellBackColor(BACK_CELL);

	m_registerlistGrid.put_TextMatrix(0,m_length,_T("Data Length"));
	m_registerlistGrid.put_Row(0);
	m_registerlistGrid.put_Col(m_length);
	m_registerlistGrid.put_CellBackColor(BACK_CELL);

	m_registerlistGrid.put_TextMatrix(0,m_description,_T("Description"));
	m_registerlistGrid.put_Row(0);
	m_registerlistGrid.put_Col(m_description);
	m_registerlistGrid.put_CellBackColor(BACK_CELL);
	 
	m_registerlistGrid.put_TextMatrix(0,m_Operation,_T("Operation"));
	m_registerlistGrid.put_Row(0);
	m_registerlistGrid.put_Col(m_Operation);
	m_registerlistGrid.put_CellBackColor(BACK_CELL);
	
}

void CRegisterViewerDlg::Write_GridCol(){
CString name;
for (int i=0;i<6;i++)
{
name=m_registerlistGrid.get_TextMatrix(0,i);
m_Inifile.WriteProfileInt(SECTION,name,i);
}
}

HBRUSH CRegisterViewerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	switch(pWnd->GetDlgCtrlID())
	{
	case IDC_COLNAME:
		{
			pDC->SetTextColor(RGB(255,255,255));
			pDC->SetBkColor(RGB(0,255,255));
			pDC->SetBkMode(TRANSPARENT);
			HBRUSH B = CreateSolidBrush(RGB(0,0,0));
			return (HBRUSH)B;
		}
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


//void CRegisterViewerDlg::OnBnClickedAddnewproduct()
//{
//	 
//}


void CRegisterViewerDlg::OnMainNew()
{
     m_CanEdit=TRUE;
	 GetDlgItem(IDC_STATIC_PRODUCTNAME)->ShowWindow(TRUE);
	 GetDlgItem(IDC_EDIT_PRODUCTNAME)->ShowWindow(TRUE);
	 GetDlgItem(IDC_SAVETODB)->ShowWindow(TRUE);
	 GetDlgItem(IDC_PRODUCTNAME)->ShowWindow(FALSE);
	 GetDlgItem(IDC_PRODUCTNAME_T)->ShowWindow(FALSE);
	 GetDlgItem(IDC_LEFT)->ShowWindow(FALSE);
	 GetDlgItem(IDC_BEGIN)->ShowWindow(FALSE);
	 GetDlgItem(IDC_RIGHT)->ShowWindow(FALSE);
	 GetDlgItem(IDC_END)->ShowWindow(FALSE);
	 GetDlgItem(IDC_PAGES)->ShowWindow(FALSE);
	 Fresh_GridCol();
}


void CRegisterViewerDlg::ClickInputMsflexgrid()
{
	 
	 m_inNameEdt.ShowWindow(SW_HIDE);
	 long lRow,lCol;
	 lRow = m_registerlistGrid.get_RowSel();//获取点击的行号	
	 lCol = m_registerlistGrid.get_ColSel(); //获取点击的列号
	 TRACE(_T("Click input grid!\n"));

	 CRect rect;
	 m_registerlistGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	 ScreenToClient(rect); //转换为客户区矩形	
	 CDC* pDC =GetDC();

	 int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	 int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	 //计算选中格的左上角的坐标(象素为单位)
	 long y = m_registerlistGrid.get_RowPos(lRow)/nTwipsPerDotY;
	 long x = m_registerlistGrid.get_ColPos(lCol)/nTwipsPerDotX;
	 //计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	 long width = m_registerlistGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	 long height = m_registerlistGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	 //形成选中个所在的矩形区域
	 CRect rcCell(x,y,x+width,y+height);
	 //转换成相对对话框的坐标
	 rcCell.OffsetRect(rect.left+1,rect.top+1);
	 ReleaseDC(pDC);
	 CString strValue = m_registerlistGrid.get_TextMatrix(lRow,lCol);
	 m_oldname=strValue;
	 m_SelCurCol=lCol;
	 m_SelCurRow=lRow;

	 if (!m_CanEdit)
	 {
		 /* m_registerlistGrid.put_Row(m_SelCurRow);
		 m_registerlistGrid.put_Col(m_SelCurCol);    
		 unsigned long color=m_registerlistGrid.get_CellBackColor();
		 if (BACK_CELL==color)
		 {
		 m_registerlistGrid.put_Row(m_SelCurRow);
		 m_registerlistGrid.put_Col(m_SelCurCol);
		 m_registerlistGrid.put_CellBackColor(WRITE_COLOR);

		 } 
		 else
		 {
		 m_registerlistGrid.put_Row(m_SelCurRow);
		 m_registerlistGrid.put_Col(m_SelCurCol);
		 m_registerlistGrid.put_CellBackColor(BACK_CELL);
		 }*/

		 return;
	 }
	 m_inNameEdt.MoveWindow(&rcCell,1);
	 m_inNameEdt.ShowWindow(SW_SHOW);
	 m_inNameEdt.SetWindowText(strValue);
	 m_inNameEdt.SetFocus();
	 m_inNameEdt.SetCapture();//LSC
	 int nLenth=strValue.GetLength();
	 m_inNameEdt.SetSel(nLenth,nLenth); //全选//
}


//void CRegisterViewerDlg::ScrollInputMsflexgrid()
//{
//	 
//}


//void CRegisterViewerDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// This feature requires Windows Vista or greater.
//	// The symbol _WIN32_WINNT must be >= 0x0600.
//	// TODO: Add your message handler code here and/or call default
//	RECT rc;
//	m_registerlistGrid.GetWindowRect(&rc);
//	CRect rect(rc);
//	if(rect.PtInRect(pt))
//	{
//		long lTopRow = m_registerlistGrid.get_TopRow();
//		lTopRow = (zDelta<0) ? (lTopRow+1) : (lTopRow-1);
//		if(lTopRow<1 || lTopRow>m_registerlistGrid.get_Rows())
//		{
//			 return  ;
//		}
//		m_registerlistGrid.put_TopRow(lTopRow);
//	}
//	CDialog::OnMouseHWheel(nFlags, zDelta, pt);
//}


//void CRegisterViewerDlg::ScrollInputMsflexgrid()
//{
//	
//}


BOOL CRegisterViewerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	RECT rc;
	m_registerlistGrid.GetWindowRect(&rc);
	CRect rect(rc);
	if(rect.PtInRect(pt))
	{
		long lTopRow = m_registerlistGrid.get_TopRow();
		lTopRow = (zDelta<0) ? (lTopRow+1) : (lTopRow-1);
		if(lTopRow<0 || lTopRow>m_registerlistGrid.get_Rows())
		{
			return TRUE ;
		}
		m_registerlistGrid.put_TopRow(lTopRow);
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CRegisterViewerDlg::OnEnKillfocusCelledit()
{
	 
}


void CRegisterViewerDlg::DblClickInputMsflexgrid()
{
	
	m_inNameEdt.ShowWindow(SW_HIDE);
	long lRow,lCol;
	lRow = m_registerlistGrid.get_RowSel();//获取点击的行号	
	lCol = m_registerlistGrid.get_ColSel(); //获取点击的列号
	TRACE(_T("Click input grid!\n"));

	CRect rect;
	m_registerlistGrid.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_registerlistGrid.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_registerlistGrid.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_registerlistGrid.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_registerlistGrid.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);
	CString strValue = m_registerlistGrid.get_TextMatrix(lRow,lCol);
	m_oldname=strValue;
	m_SelCurCol=lCol;
	m_SelCurRow=lRow;

	m_regno=m_Inifile.GetProfileInt(SECTION,_T("RegNo"),0);
	m_value=m_Inifile.GetProfileInt(SECTION,_T("Value"),1);
	m_name=m_Inifile.GetProfileInt(SECTION,_T("Name"),2);
	m_type=m_Inifile.GetProfileInt(SECTION,_T("Data Type"),3);
	//m_length=m_Inifile.GetProfileInt(SECTION,_T("Data Length"),4);
	m_description=m_Inifile.GetProfileInt(SECTION,_T("Description"),5);
	m_Operation=m_Inifile.GetProfileInt(SECTION,_T("Operation"),6);
	 
// 		m_registerlistGrid.put_Row(m_SelCurRow);
// 		m_registerlistGrid.put_Col(m_SelCurCol);
// 		m_registerlistGrid.put_CellBackColor(WRITE_COLOR);
		 if (m_CanEdit)
		 {
		 return;
		 }
	 
   if ((m_SelCurCol==m_value)&&m_SelCurRow!=0)
   {
      CString reg_no=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_regno);
	  CString reg_value=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_value);
	  CString reg_name=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_name);
	  CString reg_description=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_description);
	  CString reg_Type=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_type);
	  CString reg_Operation=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_Operation);
	  m_WriteValueDlg.Set(0,reg_Type,reg_name,reg_no,reg_value,reg_description,reg_Operation);
	  m_WriteValueDlg.DoModal();
   }
   if ((m_SelCurCol==m_description)&&m_SelCurRow!=0)
   {
	   CString reg_no=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_regno);
	   CString reg_value=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_value);
	   CString reg_name=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_name);
	   CString reg_description=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_description);
	   CString reg_Type=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_type);
	   CString reg_Operation=m_registerlistGrid.get_TextMatrix(m_SelCurRow,m_Operation);
	   m_WriteValueDlg.Set(1,reg_Type,reg_name,reg_no,reg_value,reg_description,reg_Operation);
	   m_WriteValueDlg.DoModal();
   }
}


void CRegisterViewerDlg::OnBnClickedSavetodb()
{
	 
}


void CRegisterViewerDlg::OnBnClickedOk()
{
	 
}

UINT BackRegisterViewerFreshProc(LPVOID pParam)
{
	CRegisterViewerDlg* pdlg = (CRegisterViewerDlg*)pParam;

	//if ((pdlg->strparamode.CompareNoCase(_T("Tstat6")) == 0)||(pdlg->strparamode.CompareNoCase(_T("Tstat7")) == 0))
	//	pdlg->Refresh6();
	//else
	//	pdlg->Refresh();
	while(1)
	{
		Sleep(1000);
	 
	pdlg->ShowCurPage();
	}

	return 0;
}

void CRegisterViewerDlg::OnDestroy()
{


	DWORD dwCode;
	if (pParamBackFresh!=NULL)
	{
		GetExitCodeThread(pParamBackFresh->m_hThread,&dwCode);
		if (dwCode==STILL_ACTIVE)
		{
			Sleep(200);

			TerminateThread(pParamBackFresh->m_hThread,dwCode);
			delete pParamBackFresh;
		}
	}
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}
