// IONameConfig.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "IONameConfig.h"
#include "afxdialogex.h"
#include "../SQLiteDriver/CppSQLite3.h"

// CIONameConfig dialog

IMPLEMENT_DYNAMIC(CIONameConfig, CDialog)

CIONameConfig::CIONameConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CIONameConfig::IDD, pParent)
{
	 m_changed=FALSE;
	 m_Clicked=FALSE;
}

CIONameConfig::~CIONameConfig()
{
}

void CIONameConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_IOTABLE, m_IONametable);
	DDX_Control(pDX, IDC_NAMEEDIT, m_nameEditor);
}


BEGIN_MESSAGE_MAP(CIONameConfig, CDialog)
	ON_EN_KILLFOCUS(IDC_NAMEEDIT, &CIONameConfig::OnEnKillfocusNameedit)
	ON_BN_CLICKED(IDOK, &CIONameConfig::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CIONameConfig::OnBnClickedCancel)
END_MESSAGE_MAP()


// CIONameConfig message handlers

BOOL CIONameConfig::OnInitDialog()
{
	 CDialog::OnInitDialog();
	

	GetAllIOName();

	return TRUE;
	
}BEGIN_EVENTSINK_MAP(CIONameConfig, CDialog)
	ON_EVENT(CIONameConfig, IDC_MSFLEXGRID_IOTABLE, DISPID_CLICK, CIONameConfig::ClickMsflexgridIotable, VTS_NONE)
	END_EVENTSINK_MAP()


void CIONameConfig::ClickMsflexgridIotable()
{
    m_Clicked=TRUE;
	long lRow,lCol;
	lRow = m_IONametable.get_RowSel();//获取点击的行号	
	lCol = m_IONametable.get_ColSel(); //获取点击的列号
	if(lRow==0)
		return;
	CRect rect;
	m_IONametable.GetWindowRect(rect); //获取表格控件的窗口矩形
	ScreenToClient(rect); //转换为客户区矩形	
	// MSFlexGrid控件的函数的长度单位是"缇(twips)"，
	//需要将其转化为像素，1440缇= 1英寸
	CDC* pDC =GetDC();
	//计算象素点和缇的转换比例
	int nTwipsPerDotX = 1440 / pDC->GetDeviceCaps(LOGPIXELSX) ;
	int nTwipsPerDotY = 1440 / pDC->GetDeviceCaps(LOGPIXELSY) ;
	//计算选中格的左上角的坐标(象素为单位)
	long y = m_IONametable.get_RowPos(lRow)/nTwipsPerDotY;
	long x = m_IONametable.get_ColPos(lCol)/nTwipsPerDotX;
	//计算选中格的尺寸(象素为单位)。加1是实际调试中，发现加1后效果更好
	long width = m_IONametable.get_ColWidth(lCol)/nTwipsPerDotX+1;
	long height = m_IONametable.get_RowHeight(lRow)/nTwipsPerDotY+1;
	//形成选中个所在的矩形区域
	CRect rc(x,y,x+width,y+height);
	//转换成相对对话框的坐标
	rc.OffsetRect(rect.left+1,rect.top+1);
	//获取选中格的文本信息	
	CString strValue = m_IONametable.get_TextMatrix(lRow,lCol);
	m_oldname=strValue;
	m_nCurRow=lRow;
	m_nCurCol=lCol;

	if(m_nCurCol==0)
	{
		//if(m_nCurRow<m_IONametable.get_Rows()-1)
			return;
	}
	else
	{
		 m_nameEditor.ShowWindow(SW_SHOW);
		 m_nameEditor.SetWindowText(strValue); //显示文本
		  		 
		 m_nameEditor.SetFocus();
		 int nLenth=strValue.GetLength();
		 
		 m_nameEditor.SetFocus(); //获取焦点
		 m_nameEditor.MoveWindow(rc); //移动到选中格的位置，覆盖
	}
	
}

void CIONameConfig::GetAllIOName()
{
	try
	{  
	    m_IONametable.Clear();
		m_IONametable.put_Cols(19);
		
		CppSQLite3DB SqliteDBBuilding;
		CppSQLite3Table table;
		CppSQLite3Query q;
		SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

		/*CString strSerial;
		strSerial.Format(_T("%d"),g_serialNum);
		strSerial.Trim();*/
		CString strsql;
		strsql.Format(_T("select * from IONAME"));
		//m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	
		q = SqliteDBBuilding.execQuery((UTF8MBSTR)strsql);
		table = SqliteDBBuilding.getTable((UTF8MBSTR)strsql);
		int RecordCount=table.numRows();
		m_IONametable.put_Rows(RecordCount+1); 
		m_IONametable.put_TextMatrix(0,0,_T("NO"));
		m_IONametable.put_TextMatrix(0,1,_T("SERIAL_ID"));
		m_IONametable.put_TextMatrix(0,2,_T("SENSORNAME"));
		m_IONametable.put_TextMatrix(0,3,_T("INPUT1"));
		m_IONametable.put_TextMatrix(0,4,_T("INPUT2"));
		m_IONametable.put_TextMatrix(0,5,_T("INPUT3"));
		m_IONametable.put_TextMatrix(0,6,_T("INPUT4"));
		m_IONametable.put_TextMatrix(0,7,_T("INPUT5"));
		m_IONametable.put_TextMatrix(0,8,_T("INPUT6"));
		m_IONametable.put_TextMatrix(0,9,_T("INPUT7"));
		m_IONametable.put_TextMatrix(0,10,_T("INPUT8"));
		m_IONametable.put_TextMatrix(0,11,_T("INPUT9"));
		
		m_IONametable.put_TextMatrix(0,12,_T("OUTPUT1"));
		m_IONametable.put_TextMatrix(0,13,_T("OUTPUT2"));
		m_IONametable.put_TextMatrix(0,14,_T("OUTPUT3"));
		m_IONametable.put_TextMatrix(0,15,_T("OUTPUT4"));
		m_IONametable.put_TextMatrix(0,16,_T("OUTPUT5"));
		m_IONametable.put_TextMatrix(0,17,_T("OUTPUT6"));
		m_IONametable.put_TextMatrix(0,18,_T("OUTPUT7"));

	
		int row=0;	CString index;
		while(!q.eof())
		{
		    index.Format(_T("%d"),++row);
			m_IONametable.put_TextMatrix(row,0,index);

			CString str_temp;
			str_temp.Empty();
			_variant_t temp_variant;

		 
			//g_strSensorName=str_temp;
			str_temp = q.getValuebyName(L"SERIAL_ID");
			m_IONametable.put_TextMatrix(row,1,str_temp);


			 
				str_temp = q.getValuebyName(L"SENSORNAME",L"SENSORNAME");
			m_IONametable.put_TextMatrix(row,2,str_temp);


			 
			//g_strInName1=str_temp;
			str_temp = q.getValuebyName(L"INPUT1",L"Input 1");
			 m_IONametable.put_TextMatrix(row,3,str_temp);

			  
			str_temp = q.getValuebyName(L"INPUT2",L"Input 2");
			 m_IONametable.put_TextMatrix(row,4,str_temp);

			 

				str_temp = q.getValuebyName(L"INPUT3",L"Input 3");
			  m_IONametable.put_TextMatrix(row,5,str_temp);

		 
				str_temp = q.getValuebyName(L"INPUT4",L"Input 4");
		  m_IONametable.put_TextMatrix(row,6,str_temp);

		 
				str_temp = q.getValuebyName(L"INPUT5",L"Input 5");
		  m_IONametable.put_TextMatrix(row,7,str_temp);

		 
				str_temp = q.getValuebyName(L"INPUT6",L"Input 6");
			  m_IONametable.put_TextMatrix(row,8,str_temp);

			 
				str_temp = q.getValuebyName(L"INPUT7",L"Input 7");
		  m_IONametable.put_TextMatrix(row,9,str_temp);

			 
				str_temp = q.getValuebyName(L"INPUT8",L"Input 8");
		  m_IONametable.put_TextMatrix(row,10,str_temp);

		 
				str_temp = q.getValuebyName(L"INPUT9",L"Humidity Sensor");
			  m_IONametable.put_TextMatrix(row,11,str_temp);

			 


		 

			str_temp = q.getValuebyName(L"OUTPUT1",L"Output 1");
			 m_IONametable.put_TextMatrix(row,12,str_temp);

			 
				str_temp = q.getValuebyName(L"OUTPUT2",L"Output 2");
			 m_IONametable.put_TextMatrix(row,13,str_temp);

							

	 
				 str_temp = q.getValuebyName(L"OUTPUT3",L"Output 3");
			 m_IONametable.put_TextMatrix(row,14,str_temp);
			
			 
				str_temp = q.getValuebyName(L"OUTPUT4",L"Output 4");
			 m_IONametable.put_TextMatrix(row,15,str_temp);

			str_temp = q.getValuebyName(L"OUTPUT5",L"Output 5");
			 m_IONametable.put_TextMatrix(row,16,str_temp);

			 
				str_temp = q.getValuebyName(L"OUTPUT6",L"Output 6");
			 m_IONametable.put_TextMatrix(row,17,str_temp);

			 
				str_temp = q.getValuebyName(L"OUTPUT7",L"Output 7");
			 m_IONametable.put_TextMatrix(row,18,str_temp);

			 

			





			 q.nextRow();
			
		}
		
		 SqliteDBBuilding.closedb();
	}
	catch (...)
	{

	}
}


void CIONameConfig::OnEnKillfocusNameedit()
{
    m_nameEditor.GetWindowText(m_newname);
	m_IONametable.put_TextMatrix(m_nCurRow,m_nCurCol,m_newname);
	if (m_oldname.CompareNoCase(m_newname)==0)
	{
	 return;//只要是相同的，都不进行改变
	} 
	else
	{
		CppSQLite3DB SqliteDBBuilding;
		CppSQLite3Table table;
		CppSQLite3Query q;
		SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

		try
		{
		    CString strSql;BOOL is_exist=FALSE;	  CString str_temp;
			if (m_nCurCol==1)//SerialNo
			{
			  strSql.Format(_T("select * from IONAME where SERIAL_ID='%s'"),m_newname);
			 // m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
			  q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);

			  if(q.eof())
			  {
			   is_exist=TRUE;
			  }
			 
				//m_pRs->Close();
			  if (is_exist)
			  {
			   strSql.Format(_T("Serial No=%s exist!"),m_newname);
			    AfxMessageBox(strSql);

				return;
			  }
			  else
			  {
				  strSql.Format(_T("update IONAME set SERIAL_ID='%s' where SERIAL_ID='%s'"),m_newname,m_oldname);
				  SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
				  m_changed=TRUE;
				// GetAllIOName();  
			  }
			} 
			else//
			{
			   CString SerialNo,SName,Input1,Input2,Input3,Input4,Input5,Input6,Input7,Input8,Input9,Output1,Output2,Output3,Output4,Output5,Output6,Output7;
			   SerialNo=m_IONametable.get_TextMatrix(m_nCurRow,1);
			   SName=m_IONametable.get_TextMatrix(m_nCurRow,2);
			   Input1=m_IONametable.get_TextMatrix(m_nCurRow,3);
			   Input2=m_IONametable.get_TextMatrix(m_nCurRow,4);
			   Input3=m_IONametable.get_TextMatrix(m_nCurRow,5);
			   Input4=m_IONametable.get_TextMatrix(m_nCurRow,6);
			   Input5=m_IONametable.get_TextMatrix(m_nCurRow,7);
			   Input6=m_IONametable.get_TextMatrix(m_nCurRow,8);
			   Input7=m_IONametable.get_TextMatrix(m_nCurRow,9);
			   Input8=m_IONametable.get_TextMatrix(m_nCurRow,10);
			   Input9=m_IONametable.get_TextMatrix(m_nCurRow,11);
			   Output1=m_IONametable.get_TextMatrix(m_nCurRow,12);
			   Output2=m_IONametable.get_TextMatrix(m_nCurRow,13);
			   Output3=m_IONametable.get_TextMatrix(m_nCurRow,14);
			   Output4=m_IONametable.get_TextMatrix(m_nCurRow,15);
			   Output5=m_IONametable.get_TextMatrix(m_nCurRow,16);
			   Output6=m_IONametable.get_TextMatrix(m_nCurRow,17);
			   Output7=m_IONametable.get_TextMatrix(m_nCurRow,18);

			   strSql.Format(_T("update IONAME set SENSORNAME='%s', INPUT1='%s',INPUT2='%s',INPUT3='%s',INPUT4='%s',INPUT5='%s',INPUT6='%s',INPUT7='%s',INPUT8='%s',INPUT9='%s',OUTPUT1='%s',OUTPUT2='%s',OUTPUT3='%s',OUTPUT4='%s',OUTPUT5='%s',OUTPUT6='%s',OUTPUT7='%s'  where SERIAL_ID='%s'"),SName,Input1,Input2,Input3,Input4,Input5,Input6,Input7,Input8,Input9,Output1,Output2,Output3,Output4,Output5,Output6,Output7,SerialNo);
			  SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
			   m_changed=TRUE;
			   

			}
		}
		catch(_com_error e)
		{
			/* AfxMessageBox(e.Description());*/
			//MessageBox(m_name_new+_T("  has been here\n Please change another name!"));

			SqliteDBBuilding.closedb();
			//bado.m_pConnection->Close();
			return ;
		}
		SqliteDBBuilding.closedb();
	 	 GetAllIOName(); 
	}
}


void CIONameConfig::OnBnClickedOk()
{
	
	//CDialog::OnOK();
	if (m_Clicked)
	{
		OnEnKillfocusNameedit();
		GetAllIOName();
	}
	
}

void CIONameConfig::GetIOName()
{
	try
	{


		CppSQLite3DB SqliteDBBuilding;
		CppSQLite3Table table;
		CppSQLite3Query q;
		SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);


		CString strSerial;
		strSerial.Format(_T("%d"),g_serialNum);
		strSerial.Trim();
		CString strsql;
		strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
		//m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);	
		q = SqliteDBBuilding.execQuery((UTF8MBSTR)strsql);
		if(!q.eof())
		{	
			CString str_temp;
			str_temp.Empty();
			_variant_t temp_variant;

		 
				str_temp = q.getValuebyName(L"SENSORNAME",L"Internal Sensor");
			   g_strSensorName=str_temp;

			 
			str_temp = q.getValuebyName(L"INPUT1",L"Input 1");
			g_strInName1=str_temp;

		 
				str_temp = q.getValuebyName(L"INPUT2",L"Input 2");
			g_strInName2=str_temp;

			 
				str_temp = q.getValuebyName(L"INPUT3",L"Input 3");
			g_strInName3=str_temp;

			 
				str_temp = q.getValuebyName(L"INPUT4",L"Input 4");
			g_strInName4=str_temp;

			 
				str_temp = q.getValuebyName(L"INPUT5",L"Input 5");
			g_strInName5=str_temp;

			 
				str_temp = q.getValuebyName(L"INPUT6",L"Input 6");
			g_strInName6=str_temp;

			 
				str_temp = q.getValuebyName(L"INPUT7",L"Input 7");
			g_strInName7=str_temp;

			 
				str_temp = q.getValuebyName(L"INPUT8",L"Input 8");
			g_strInName8=str_temp;

			 

				str_temp = q.getValuebyName(L"INPUT9",L"Humidity Sensor");
			g_strInHumName=str_temp;

		 

				str_temp = q.getValuebyName(L"OUTPUT1",L"Output 1");
			g_strOutName1=str_temp;

		 

				str_temp = q.getValuebyName(L"OUTPUT2",L"Output 2");
			g_strOutName2=str_temp;

	 

				str_temp = q.getValuebyName(L"OUTPUT3",L"Output 3");
			g_strOutName3=str_temp;

		 

				str_temp = q.getValuebyName(L"OUTPUT4",L"Output 4");
			g_strOutName4=str_temp;

			 

				str_temp = q.getValuebyName(L"OUTPUT5",L"Output 5");
			g_strOutName5=str_temp;

			 
				str_temp = q.getValuebyName(L"OUTPUT6",L"Output 6");
			g_strOutName6=str_temp;

		 
				str_temp = q.getValuebyName(L"OUTPUT7",L"Output 7");
			g_strOutName7=str_temp;

			g_strInHumName = _T("Humidity Sensor");
			g_strInCO2=_T("CO2 Sensor");
		}
		else
		{
			g_strSensorName =_T("Internal Temp Sensor");
			g_strInName1=_T("Input 1");
			g_strInName2=_T("Input 2");
			g_strInName3=_T("Input 3");
			g_strInName4=_T("Input 4");
			g_strInName5=_T("Input 5");
			g_strInName6=_T("Input 6");
			g_strInName7=_T("Input 7");
			g_strInName8=_T("Input 8");

			g_strInHumName = _T("Humidity Sensor");
			g_strInCO2=_T("CO2 Sensor");

			g_strOutName1=_T("Output 1");
			g_strOutName2=_T("Output 2");
			g_strOutName3=_T("Output 3");
			g_strOutName4=_T("Output 4");
			g_strOutName5=_T("Output 5");
			g_strOutName6=_T("Output 6");
			g_strOutName7=_T("Output 7");
			g_strOutName8=_T("Output 8");


		}
		SqliteDBBuilding.closedb();
		 	

	}
	catch (...)
	{

	}



}

void CIONameConfig::OnBnClickedCancel()
{
	
	if (m_changed)
	{
	GetIOName();
	}
	//更新全局变量IOName的值
	CDialog::OnCancel();
}
