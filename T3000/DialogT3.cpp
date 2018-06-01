// DialogT3.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "DialogT3.h"



// CDialogT3

IMPLEMENT_DYNCREATE(CDialogT3, CFormView)

CDialogT3::CDialogT3()
	: CFormView(CDialogT3::IDD)
	, m_address(0)
	, m_firmware(0)
	, m_serial(0)
	, m_hardware(0)
	, m_model(_T(""))
	, m_picVersion(0)
{

}

CDialogT3::~CDialogT3()
{
}

void CDialogT3::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_T3ADDRESS, m_address);
	DDX_Text(pDX, IDC_EDIT_T3FIRMWARE, m_firmware);
	DDX_Text(pDX, IDC_EDIT_T3SERIAL, m_serial);
	DDX_Text(pDX, IDC_EDIT_T3HARDWARE, m_hardware);
	DDX_Text(pDX, IDC_EDIT_T3MODEL, m_model);
	DDX_Text(pDX, IDC_EDIT_T3PICVERSION, m_picVersion);
	DDX_Control(pDX, IDC_MSFLEXGRID_INPUT, m_msflexgrid_input);
	DDX_Control(pDX, IDC_MSFLEXGRID_OUTPUT, m_msflexgrid_output);
}

BEGIN_MESSAGE_MAP(CDialogT3, CFormView)
END_MESSAGE_MAP()


// CDialogT3 diagnostics

#ifdef _DEBUG
void CDialogT3::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDialogT3::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}


#endif
#endif //_DEBUG


// CDialogT3 message handlers

void CDialogT3::OnInitialUpdate()
{
	return CFormView::OnInitialUpdate();
	//============================================================================================================界面Input部份列表框


	//显示横标题
	m_msflexgrid_input.put_TextMatrix(0,1,_T("Input Name"));
	m_msflexgrid_input.put_TextMatrix(0,2,_T("Value"));
	//m_msflexgrid_input.put_TextMatrix(0,3,_T("Hand/Off/Auto"));
	m_msflexgrid_input.put_TextMatrix(0,3,_T("Input Filter"));
	m_msflexgrid_input.put_TextMatrix(0,4,_T("Range"));
	m_msflexgrid_input.put_TextMatrix(0,5,_T("Function"));


	//设置排/行数量
	m_msflexgrid_input.put_Cols(6);
	m_msflexgrid_input.put_Rows(9);

	//设置列宽	
	m_msflexgrid_input.put_ColWidth(0,400);
	//m_msflexgrid_input.put_ColWidth(3,1500);

	//居中显示
	for (int col=0;col<6;col++)
	{ 
		m_msflexgrid_input.put_ColAlignment(col,4);
	}

	//彩色显示
	for(int i=1;i<9;i++)		//排数量
	{

		for(int k=0;k<6;k++)	//列数量
		{
			if (i%2==1)
			{
				m_msflexgrid_input.put_Row(i);m_msflexgrid_input.put_Col(k);m_msflexgrid_input.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_msflexgrid_input.put_Row(i);m_msflexgrid_input.put_Col(k);m_msflexgrid_input.put_CellBackColor(COLOR_CELL);
			}
		}
	}




	//第3，4列11-26灰色显示
	for (int i=1;i<9;i++)
	{

		m_msflexgrid_input.put_TextMatrix(i,4,_T("°C"));	  
		m_msflexgrid_input.put_Row(i);
		m_msflexgrid_input.put_Col(4);
		m_msflexgrid_input.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);	   //灰色
	}


	//显示纵标题
	CString str;
	for(int i=1;i<=8;i++)
	{

		str.Format(_T("%d"),i);

		m_msflexgrid_input.put_TextMatrix(i,0,str);	 

		str = _T("Input ")+str;
		m_msflexgrid_input.put_TextMatrix(i,1,str);
		m_msflexgrid_input.put_TextMatrix(i,5,_T("Default"));	

	}

	//显示纵标题
	str =_T("");
	for(int i=11;i<9;i++)
	{

		str.Format(_T("%d"),i);

		m_msflexgrid_input.put_TextMatrix(i,0,str);
		str = _T("Input ")+str;
		m_msflexgrid_input.put_TextMatrix(i,1,str);

		m_msflexgrid_input.put_TextMatrix(i,3,_T("Auto"));
		m_msflexgrid_input.put_TextMatrix(i,4,_T("On/Off"));
		m_msflexgrid_input.put_TextMatrix(i,5,_T("Default"));	


	}


	//unsigned char writevalue[1];
	//writevalue[0] = 0;
	//int flg = Write_Multi(g_tstat_id,writevalue,144,10);
	//TRACE(_T("flg=%d\n"),flg);
	//ASSERT(flg>0);


	//============================================================================================================界面Output部份列表框

	//设置行/列数量
	m_msflexgrid_output.put_Rows(14);
	m_msflexgrid_output.put_Cols(6);
	//设置列宽	
	m_msflexgrid_output.put_ColWidth(0,400);
	m_msflexgrid_output.put_ColWidth(3,1500);

	//显示横标题
	m_msflexgrid_output.put_TextMatrix(0,1,_T("Input Name"));
	m_msflexgrid_output.put_TextMatrix(0,2,_T("Value"));
	m_msflexgrid_output.put_TextMatrix(0,3,_T("Auto/Manual"));
	m_msflexgrid_output.put_TextMatrix(0,4,_T("Range"));
	m_msflexgrid_output.put_TextMatrix(0,5,_T("Function"));



	//居中显示
	for (int col=0;col<6;col++)
	{ 
		m_msflexgrid_output.put_ColAlignment(col,4);
	}

	//彩色显示
	for(int i=1;i<14;i++)		//排数量
	{

		for(int k=0;k<6;k++)	//列数量
		{
			if (i%2==1)
			{
				m_msflexgrid_output.put_Row(i);m_msflexgrid_output.put_Col(k);m_msflexgrid_output.put_CellBackColor(RGB(255,255,255));
			}
			else
			{
				m_msflexgrid_output.put_Row(i);m_msflexgrid_output.put_Col(k);m_msflexgrid_output.put_CellBackColor(COLOR_CELL);
			}
		}
	}
	//显示纵标题
	CString str_output;
	for(int i=1;i<=13;i++)
	{

		str_output.Format(_T("%d"),i);
		m_msflexgrid_output.put_TextMatrix(i,0,str_output);	
		str_output = _T("Output ")+str_output;
		m_msflexgrid_output.put_TextMatrix(i,1,str_output);

		//m_msflexgrid_output.put_TextMatrix(i,3,_T("Off"));
		m_msflexgrid_output.put_TextMatrix(i,4,_T("On/Off"));
		m_msflexgrid_output.put_TextMatrix(i,5,_T("Default"));
		m_msflexgrid_output.put_Row(i);
		m_msflexgrid_output.put_Col(4);
		m_msflexgrid_output.put_CellBackColor(FLEXGRID_CELL_GRAY_COLOR);
	}


}

void CDialogT3::Fresh()
{

	ShowDialogData();
}

void CDialogT3::ShowDialogData()
{

	m_address= multi_register_value[6];
	m_firmware= multi_register_value[5];
	m_serial= get_serialnumber(); //Address 0-3
	m_hardware= multi_register_value[4];
	if (multi_register_value[7] == 20)
		m_model= _T("T3-8AI13O");
	m_picVersion= multi_register_value[9];

	UpdateData(FALSE); 

		//183					1		Range Setting for each input.
		//184					1		183 correspond to input1,
		//185					1		184 correspond to input2, etc.
		//186					1		0 = raw data,
		//187					1		1 = 10K Celsius,
		//188					1		2 = 10K Fahrenheit,
		//189					1		3 = 0-100%
		//190					1		4 = ON/OFF,
		//								5 = OFF/ON
		//								6 = Pulse Input
		//								7 = Lighting Control
	for (int i = 183;i<=190;i++)
	{	
		int retRangSet =write_one(g_tstat_id,i,1);
		TRACE(_T("retRangSet=%d\n"),retRangSet);
	}
	


	CString strinput,stroutput,strfilter;
	//Input Register address 118-133  eg:input1-> 118 high word   119 low word
	int numrow = 1;
	for (int i = 1;i<=16;i++)
	{
		int temphigh = multi_register_value[i+117]<<16;
		i++;
		int templow = multi_register_value[i+117]&0x0000ffff;
		int temp = temphigh|templow;

		strinput.Format(_T("%d°C"),temp);
		m_msflexgrid_input.put_TextMatrix(numrow,2,strinput);
		//Input Filter address 191-198
		strfilter.Format(_T("%d"),multi_register_value[190+numrow]);
		m_msflexgrid_input.put_TextMatrix(numrow,3,strfilter);
		numrow++;
		
	

	}

	//Output Register address 100-112
	for (int i = 1;i<=13;i++)
	{
		if (multi_register_value[i+99] == 0)
		{
			m_msflexgrid_output.put_TextMatrix(i,2,_T("Off"));
		} 
		else if(multi_register_value[i+99] > 0)
		{
			m_msflexgrid_output.put_TextMatrix(i,2,_T("On"));
		}

	}

	//显示Auto/Manual
	//116	2	Switch Status, 1 Register output 1-8
	//117	2	Switch Status, 2 Register output 9-13
	//当output全按至HAND时，116 117的二进制值：
	//					 high 8 bit   |	  low 8 bit							
	//HAND INPUT		 1   2   3   4   5   6  7    8			  9  10  11  12  13
//register:116 117     0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1			0 1 0 1 0 1 0 1 0 1 0 0 0 0 0 0


    //当output全按至AUTO时，116 117的二进制值：
	//AUTO INPUT	   1   2   3   4   5   6   7   8			9  10  11  12  13
//register;116 117	   1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0			1 0 1 0 1 0 1 0 1 0 0 0 0 0 0 0

	int temp = 1; 

	for (int i = 8;i>=1;i--)
	{
		//Manual
		if((multi_register_value[116]&temp)!= 0)
			m_msflexgrid_output.put_TextMatrix(i,3,_T("Manual"));
			temp = temp<<1;
		if ((multi_register_value[116]&temp)!=0)
			m_msflexgrid_output.put_TextMatrix(i,3,_T("Auto"));
		temp = temp<<1;
	}

	temp = 1;
	temp = temp<<6;
	for (int i = 13;i>=9;i--)
	{
		
		//Manual
		if((multi_register_value[117]&temp)!= 0)
			m_msflexgrid_output.put_TextMatrix(i,3,_T("Manual"));	
		temp = temp<<1;
		if ((multi_register_value[117]&temp)!=0)
			m_msflexgrid_output.put_TextMatrix(i,3,_T("Auto"));
		temp = temp<<1;
	}

}