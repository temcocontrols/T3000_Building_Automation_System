// 5EInputDialog.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "5EInputDialog.h"
#include "globle_function.h"


// C5EInputDialog dialog
CString g_str5ERange[5]={_T("Raw"),_T("Thermistor"),_T("On/Off"),_T("Off/On"),_T("%")};
IMPLEMENT_DYNAMIC(C5EInputDialog, CDialog)

C5EInputDialog::C5EInputDialog(CWnd* pParent /*=NULL*/)
	: CDialog(C5EInputDialog::IDD, pParent)
{
	m_nCurRow=0;
}

C5EInputDialog::~C5EInputDialog()
{
}
void C5EInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_5EMSFLEXGRID, m_flexGrd);
	DDX_Control(pDX, IDC_5EVALUEEDIT, m_valueEdt);
	DDX_Control(pDX, IDC_5ERANGECOMBO, m_rangeCombx);
}


BEGIN_MESSAGE_MAP(C5EInputDialog, CDialog)
	ON_BN_CLICKED(IDOK, &C5EInputDialog::OnBnClickedOk)
	ON_CBN_SETFOCUS(IDC_5ERANGECOMBO, &C5EInputDialog::OnCbnSetfocus5erangecombo)
	ON_CBN_KILLFOCUS(IDC_5ERANGECOMBO, &C5EInputDialog::OnCbnKillfocus5erangecombo)
	ON_EN_SETFOCUS(IDC_5EVALUEEDIT, &C5EInputDialog::OnEnSetfocus5evalueedit)
	ON_EN_KILLFOCUS(IDC_5EVALUEEDIT, &C5EInputDialog::OnEnKillfocus5evalueedit)
	ON_BN_CLICKED(ID_EXIT, &C5EInputDialog::OnBnClickedExit)
	ON_CBN_SELCHANGE(IDC_5ERANGECOMBO, &C5EInputDialog::OnCbnSelchange5erangecombo)
	ON_CBN_SELENDOK(IDC_5ERANGECOMBO, &C5EInputDialog::OnCbnSelendok5erangecombo)
END_MESSAGE_MAP()


// C5EInputDialog message handlers

BOOL C5EInputDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_fFirmwareVersion=get_curtstat_version();
	m_flexGrd.put_ColAlignment(0,4);
	m_flexGrd.put_ColAlignment(1,4);
	m_flexGrd.put_ColAlignment(2,4);

	m_flexGrd.put_ColWidth(0,1200);
	m_flexGrd.put_ColWidth(1,1200);
	m_flexGrd.put_ColWidth(2,1200);
	ReloadDataValue();
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void C5EInputDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void C5EInputDialog::ReloadDataValue()
{
	m_flexGrd.put_TextMatrix(0,0,_T("Name"));
	m_flexGrd.put_TextMatrix(0,1,_T("Value"));
	m_flexGrd.put_TextMatrix(0,2,_T("Range"));
	m_flexGrd.put_TextMatrix(0,3,_T("Function"));
	m_valueEdt.ShowWindow(SW_HIDE);
	m_rangeCombx.ShowWindow(SW_HIDE);

	m_flexGrd.put_TextMatrix(1,0,g_strInName1);
	m_flexGrd.put_TextMatrix(2,0,g_strInName2);
	m_flexGrd.put_TextMatrix(3,0,g_strInName3);
	m_flexGrd.put_TextMatrix(4,0,g_strInName4);
	m_flexGrd.put_TextMatrix(5,0,g_strInName5);
	m_flexGrd.put_TextMatrix(6,0,g_strInName6);
	m_flexGrd.put_TextMatrix(7,0,g_strInName7);
	m_flexGrd.put_TextMatrix(8,0,g_strInName8);
	CString strTemp;
	for(int i=1;i<=8;i++)
	{

		for(int k=0;k<=2;k++)
		{
			if (i%2==1)
			{
				m_flexGrd.put_Row(i);m_flexGrd.put_Col(k);m_flexGrd.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_flexGrd.put_Row(i);m_flexGrd.put_Col(k);m_flexGrd.put_CellBackColor(COLOR_CELL);
			}
		}



		CString strTemp;
		if(m_fFirmwareVersion>33.3)
		{
			int nValue;

			if (i == 1)
			{
				float fTemp=(float)(multi_register_value[101]/10.0);
				strTemp.Format(_T("%.1f °C"),fTemp);
			}
			else
			{
				
				if(multi_register_value[359+i-1]==1)
				{				
					nValue=(int)(multi_register_value[367+i-1]/10.0);
					strTemp.Format(_T("%.1f"),nValue);							
				}
				else
				{
					strTemp.Format(_T("%d"),multi_register_value[367+i-1]);

				}
			}
				m_flexGrd.put_TextMatrix(i,1,strTemp);
				strTemp.Empty();
				int nItem=multi_register_value[359+i-1];
				if(nItem>=0&&nItem<=4)
				{
					strTemp=g_str5ERange[nItem];
				}
				m_flexGrd.put_TextMatrix(i,2,strTemp);
				

		}
		else
		{
			int nValue;
		
			if(multi_register_value[341+i-1]==1)
			{
				nValue=(int)(multi_register_value[349+i-1]/10.0);
				strTemp.Format(_T("%.1f"),nValue);
			}
			else
			{
				strTemp.Format(_T("%d"),multi_register_value[349+i-1]);
			}
			m_flexGrd.put_TextMatrix(i,1,strTemp);
	
	    	strTemp.Empty();
			int nItem=multi_register_value[341+i-1];
			if(nItem>=0&&nItem<=4)
			{
				strTemp=g_str5ERange[nItem];
			}
			m_flexGrd.put_TextMatrix(i,2,strTemp);
	
		}

	}
}


BEGIN_EVENTSINK_MAP(C5EInputDialog, CDialog)
	ON_EVENT(C5EInputDialog, IDC_5EMSFLEXGRID, DISPID_CLICK, C5EInputDialog::Click5emsflexgrid, VTS_NONE)
END_EVENTSINK_MAP()

void C5EInputDialog::Click5emsflexgrid()
{
	
	long lRow,lCol;
	lRow = m_flexGrd.get_RowSel();//获取点击的行号	
	lCol = m_flexGrd.get_ColSel(); //获取点击的列号

	CRect rect;
	m_flexGrd.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	CDC* pDC =GetDC();

	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_flexGrd.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_flexGrd.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_flexGrd.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_flexGrd.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rcCell(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rcCell.OffsetRect(rect.left+1,rect.top+1);
	ReleaseDC(pDC);

	CString strValue = m_flexGrd.get_TextMatrix(lRow,lCol);
	if(lCol==2)
	{
		m_nCurRow=lRow;
		m_rangeCombx.MoveWindow(&rcCell,1); //移动到选中格的位置
		m_rangeCombx.BringWindowToTop();
		m_rangeCombx.ShowWindow(SW_SHOW);//显示控件
		m_rangeCombx.SetFocus(); //获取焦点
	}
}

void C5EInputDialog::OnCbnSetfocus5erangecombo()
{
	// TODO: Add your control notification handler code here
}

void C5EInputDialog::OnCbnKillfocus5erangecombo()
{

}

void C5EInputDialog::OnEnSetfocus5evalueedit()
{
	// TODO: Add your control notification handler code here
}

void C5EInputDialog::OnEnKillfocus5evalueedit()
{
	// TODO: Add your control notification handler code here
}

BOOL C5EInputDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_RETURN )
	{ 
		GetDlgItem(ID_EXIT)->SetFocus();
		return true; 
	} 
	return CDialog::PreTranslateMessage(pMsg);
}

void C5EInputDialog::OnBnClickedExit()
{
	CDialog::OnCancel();
}

void C5EInputDialog::OnCbnSelchange5erangecombo()
{
	
}

void C5EInputDialog::OnCbnSelendok5erangecombo()
{
	int nIndext=m_rangeCombx.GetCurSel();
	if(m_nCurRow>0&&m_nCurRow<=8)
	{
		if(m_fFirmwareVersion>33.3)
		{
		   write_one(g_tstat_id,359+m_nCurRow-1,nIndext);
		}
		else
		{
			write_one(g_tstat_id,341+m_nCurRow-1,nIndext);
		}
	}
	ReloadDataValue();
}
