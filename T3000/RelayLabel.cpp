// RelayLabel.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "RelayLabel.h"
#include "global_function.h"
#include "global_variable_extern.h"
 #include "../SQLiteDriver/CppSQLite3.h"
 

// CRelayLabel

IMPLEMENT_DYNAMIC(CRelayLabel, CStatic)

CRelayLabel::CRelayLabel()
{
	m_bkClr=RGB(202,210,220);
}

CRelayLabel::~CRelayLabel()
{
}


BEGIN_MESSAGE_MAP(CRelayLabel, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CRelayLabel message handlers



BOOL CRelayLabel::OnEraseBkgnd(CDC* pDC)
{
	 
	//CRect rcClient;
//GetClientRect(&rcClient);
//	pDC->FillSolidRect(&rcClient,m_bkClr);
	//return 1;

	return CStatic::OnEraseBkgnd(pDC);
}

BOOL CRelayLabel::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	
	//SetTimer(1,15000,NULL);
	m_fFirmwareVersion=get_curtstat_version();
	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

void CRelayLabel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CStatic::OnPaint() for painting messages

	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(&rcClient,m_bkClr);
	dc.SetTextColor(m_bTxtClr);
	dc.DrawText(m_strValueText,&rcClient,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

}
void CRelayLabel::SetLabelInfo_General(int ndeviceid,int type,int nStatus,COLORREF textClr,COLORREF bkClr)
{
	m_bkClr = bkClr;
	m_bTxtClr = textClr;
	if(type == 0)
	{
		DispalyInputValue_General(nStatus,textClr,bkClr);
	}
	else if(type == 1)
	{
		DispalyOutputValue_General(nStatus,textClr,bkClr);
	}
	else if(type == 2)
	{
		DispalyVariableValue_General(nStatus,textClr,bkClr);
	}
}

void CRelayLabel:: SetLabelInfo(int TstatID,int input_or_output,int nItem,COLORREF textClr,COLORREF bkClr)
{
	m_bkClr=bkClr;
	m_bTxtClr=textClr;
	if(input_or_output==0)//input
	{
		DispalyInputValue(nItem,textClr,bkClr);
	}
	if(input_or_output==1)//output
	{
		DispalyOutputValue(nItem,textClr,bkClr);

	}
	if(input_or_output==2)//customed
	{
		DispalyRigesterValue(nItem,textClr,bkClr);

	}
}

void CRelayLabel::DispalyInputValue_General(int nStatus,COLORREF textClr,COLORREF bkClr)
{
	//m_Input_data.at(nStatus - 1)
		CString temp_unite;
		CString temp_value;
		int i = nStatus ;

		if(m_Input_data.at(i).digital_analog == BAC_UNITS_ANALOG)
		{
			temp_unite = Input_List_Analog_Units[m_Input_data.at(i).range];		
			temp_value.Format(_T("%d"),m_Input_data.at(i).value / 1000);
			m_strValueText = temp_value + _T(" ") + temp_unite;
		}
		else if(m_Input_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{
			CString Show_Digital;
			if((m_Input_data.at(i).range>22) || (m_Input_data.at(i).range == 0))
			{
				Show_Digital = Digital_Units_Array[0];
			}
			else
			{
				CString temp1;
				CStringArray temparray;

				temp1 = Digital_Units_Array[m_Input_data.at(i).range];
				SplitCStringA(temparray,temp1,_T("/"));
				if((temparray.GetSize()==2))
				{
					if(m_Input_data.at(i).control == 0)
						Show_Digital = temparray.GetAt(0);
					else
						Show_Digital = temparray.GetAt(1);
				}
			}
			m_strValueText = Show_Digital;
		}

		
}

void CRelayLabel::DispalyInputValue(int nStatus,COLORREF textClr,COLORREF bkClr)
{

	int nModel=product_register_value[MODBUS_PRODUCT_MODEL];
	CString strUnit=GetTempUnit();
	CString strTemp;
	strTemp.Empty();
	if(nModel==2||nModel==1||nModel==4||nModel==12||nModel==17||nModel==18||nModel==3)//A,B,C,D ,F,G
	{
		
	
		strTemp.Format(_T("%.1f"),product_register_value[101]/10.0);
		strTemp=strTemp+strUnit;

		if(nStatus==0)
		{
			
			m_strValueText=strTemp;	
	
			return;
		}


		if(nStatus==1)
		{
			strTemp.Empty();
			if(product_register_value[MODBUS_ANALOG_IN1]==4||product_register_value[MODBUS_ANALOG_IN1]==1)
			{
				strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_0]/10);
				strTemp=strTemp+strUnit;
			}
			if (product_register_value[MODBUS_ANALOG_IN1]==0||product_register_value[MODBUS_ANALOG_IN1]==2)
			{
				strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_0]);
			}
			if((product_register_value[MODBUS_ANALOG_IN1]==3)||(product_register_value[MODBUS_ANALOG_IN1]==5))
			{
				if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==1)
					strTemp=_T("On");
				if(product_register_value[MODBUS_EXTERNAL_SENSOR_0]==0)
					strTemp=_T("Off");
			}
			m_strValueText=strTemp;	
			return;
		}
		if (nStatus==2)
		{
			strTemp.Empty();
			if(product_register_value[MODBUS_ANALOG_IN2]==4||product_register_value[MODBUS_ANALOG_IN2]==1)
			{
				strTemp.Format(_T("%.1f"),(float)product_register_value[MODBUS_EXTERNAL_SENSOR_1]/10);
				strTemp=strTemp+strUnit;
			}
			if (product_register_value[MODBUS_ANALOG_IN2]==0||product_register_value[MODBUS_ANALOG_IN2]==2)
			{
				strTemp.Format(_T("%d"),product_register_value[MODBUS_EXTERNAL_SENSOR_1]);
			}
			if((product_register_value[MODBUS_ANALOG_IN2]==3)||(product_register_value[MODBUS_ANALOG_IN2]==5))
			{
				if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==1)
					strTemp=_T("On");
				if(product_register_value[MODBUS_EXTERNAL_SENSOR_1]==0)
					strTemp=_T("Off");
			}
			m_strValueText=strTemp;	
			return;
		}
		if (nStatus==3)
		{
			if (product_register_value[MODBUS_DIGITAL_IN1]==0)//190
			{
				 
				if (product_register_value[311]==1)
				{
					strTemp=_T("On");
				}
				else
				{
					strTemp=_T("Off");
				}


			}
			if (product_register_value[MODBUS_DIGITAL_IN1]==1)//190
			{
				 
				if (product_register_value[311]==0)
				{
					strTemp=_T("Off");

				}else
				{
					strTemp=_T("On");
				}

			}
			m_strValueText=strTemp;	
			return;
		}
		
	}
	strTemp.Empty();
	
	//in3 and in4 
	if(nModel==4||nModel==12||nModel==18||nModel==17)//C,D,F,G
	{
		if(nStatus==2)//input 3
		{
			strTemp.Empty();
			if(product_register_value[189]==4||product_register_value[189]==1)
			{
				strTemp.Format(_T("%.1f"),(float)product_register_value[181]/10);	
				strTemp=strTemp+strUnit;
			}
			if (product_register_value[189]==0||product_register_value[189]==2)
			{
				strTemp.Format(_T("%d"),product_register_value[181]);
			}
			if(product_register_value[189]==3)
			{
				if(product_register_value[181]==1)
					strTemp=_T("On");
				if(product_register_value[181]==0)
					strTemp=_T("Off");
			}	
			//SetWindowText(strTemp);
			m_strValueText=strTemp;	
			return;
		}


		if(nStatus==3)
		{
			strTemp.Empty();
			if (product_register_value[190]==0)
			{
				if (product_register_value[311]==0)
				{
					strTemp=_T("Off");
				}else
				{
					strTemp=_T("On");
				}
			}
			if (product_register_value[190]==1)
			{
				if (product_register_value[311]==1)
				{
					strTemp=_T("Off");

				}else
				{
					strTemp=_T("On");
				}
			}
			//SetWindowText(strTemp);
			m_strValueText=strTemp;	
			return;
		
		}
		
		
	}

	if(nModel==16||nModel==PM_PM5E || nModel == PM_PM5E_ARM)//E
	{
		if(nStatus>=0&&nStatus<8)
		{	strTemp.Empty();
			if(m_fFirmwareVersion>33.3)
			{
				int nValue;
			
				if(product_register_value[359+nStatus]==1)
				{
					nValue=(int)(product_register_value[367+nStatus]/10.0);
					strTemp.Format(_T("%.1f"),nValue);
				}
				else
				{
					strTemp.Format(_T("%d"),product_register_value[367+nStatus]);
				}
				m_strValueText=strTemp;	
				return;

			}
			else
			{
				int nValue;
				if(product_register_value[341+nStatus]==1)
				{
					nValue=(int)(product_register_value[349+nStatus]/10.0);
					strTemp.Format(_T("%.1f"),nValue);
				}
				else
				{
					strTemp.Format(_T("%d"),product_register_value[349+nStatus]);
				}
				m_strValueText=strTemp;	
				return;
			}
		}
		
	}
	
	if (nModel==PM_TSTAT6||nModel==PM_TSTAT7||nModel==PM_TSTAT5i||(product_register_value[7] == PM_TSTAT8 || (product_register_value[7] == PM_TSTAT9)
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		))
	{
	   if (nStatus==0)
	   {
		   CString strUnit=GetTempUnit();
		   strTemp.Format(_T("%.1f"),product_register_value[MODBUS_TEMPRATURE_CHIP]/10.0);  //121
		   m_strValueText=strTemp+strUnit;
	   }
	   else if (nStatus>=1&&nStatus<=8)
	   {
	      
		  int  m_crange=0;
		   int m_sn=m_sn=product_register_value[0]+product_register_value[1]*256+product_register_value[2]*256*256+product_register_value[3]*256*256*256;
		   CppSQLite3Table table;
		   CppSQLite3Query q;

		   CppSQLite3DB SqliteDBBuilding;
		   SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

		   if (SqliteDBBuilding.tableExists("Value_Range"))//有Version表
		   {
			   CString sql;
			   sql.Format(_T("Select * from Value_Range where CInputNo=%d and SN=%d"),nStatus,m_sn);
			  q = SqliteDBBuilding.execQuery((UTF8MBSTR)sql);
			   if (!q.eof())//有表但是没有对应序列号的值
			   {    
				    
				   while (!q.eof())
				   {
					   m_crange=q.getIntField("CRange");
					   q.nextRow();
				   }
				    
			   } 
			   else
			   {
				   m_crange=product_register_value[MODBUS_ANALOG1_RANGE+nStatus-1];	//189
				   
			   }
			   
		   }
		   else
		   {
			   m_crange=product_register_value[MODBUS_ANALOG1_RANGE+nStatus-1];
		   }
		   SqliteDBBuilding.closedb();

		    CString strValueUnit=GetTempUnit(product_register_value[MODBUS_ANALOG1_RANGE+nStatus-1], 1);
		   int nValue;
		   float fValue;
		   if(product_register_value[MODBUS_ANALOG1_RANGE+nStatus-1]==1)	//359  122
		   {				
			   fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+nStatus-1]/10.0);	//367   131
			   strTemp.Format(_T("%.1f"),fValue);	

			   strTemp +=strValueUnit;
		   }
		   else if(product_register_value[MODBUS_ANALOG1_RANGE+nStatus-1]==3 || product_register_value[MODBUS_ANALOG1_RANGE+nStatus-1]==5) // On/Off or Off/On ==1 On ==0 Off   359  122
		   {						
			   if (m_crange==9||m_crange==10)
			   {
				   int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+nStatus-1]); //367  131
				   if (nValue == 0)
				   {
					   strTemp = _T("Closed");
				   }
				   else
				   {
					   strTemp = _T("Open");
				   }	
			   }
			   else if (m_crange==7||m_crange==8)
			   {

				   int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+nStatus-1]); //367  131
				   if (nValue == 0)
				   {
					   strTemp = _T("Unoccupied");
				   }
				   else
				   {
					   strTemp = _T("Occupied");
				   }	

			   }
			   else
			   {

				   int nValue=(product_register_value[MODBUS_ANALOG_INPUT1+nStatus-1]); //367  131
				   if (nValue == 0)
				   {
					   strTemp = _T("Off");
				   }
				   else
				   {
					   strTemp = _T("On");
				   }	

			   }					
		   }
		   else if (product_register_value[MODBUS_ANALOG1_RANGE+nStatus-1]==4 )  // custom sensor	359 122
		   {					
			   fValue=float(product_register_value[MODBUS_ANALOG_INPUT1+nStatus-1]/10.0);	//367  131
			   strTemp.Format(_T("%.1f"), (float)fValue/10.0);	
			   strTemp +=strValueUnit;
		   }
		   else if(product_register_value[MODBUS_ANALOG1_RANGE+nStatus-1]==2)	//359 122
		   {
			   nValue=product_register_value[MODBUS_ANALOG_INPUT1+nStatus-1];		//367  131
			   strTemp.Format(_T("%0.1f%%"),  (float)nValue);
		   }
		   else
		   {
			   strTemp.Format(_T("%d"),product_register_value[MODBUS_ANALOG_INPUT1+nStatus-1]);//lsc
		   }	
           m_strValueText=strTemp;
	   }
	   else if (nStatus==9)
	   {
	      
		   if((product_register_value[20]&2)==2)
		   {
			   if (product_register_value[MODBUS_TSTAT6_HUM_AM]==0)
			   {
				   strTemp.Format(_T("%0.1f%%"),(float)product_register_value[MODBUS_TSTAT6_HUM_AVALUE]/10.0);    
			   }
			   else
			   {
				   strTemp.Format(_T("%0.1f%%"),(float)product_register_value[MODBUS_TSTAT6_HUM_MVALUE]/10);
			   }
		   }
		   else
		   {
			   strTemp=_T("NULL");
		   }
		   m_strValueText=strTemp;
	   }
	   else if (nStatus==10)
	   {
		   if((product_register_value[20]&4)==4)
		   {
			   if (product_register_value[MODBUS_TSTAT6_CO2_AM]==0)
			   {    
			    strUnit=_T("ppm");
			   
			   strTemp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_AVALUE]);
			   strTemp+=strUnit;
			   
			   }
			   else
			   {
				   
				   strTemp.Format(_T("%d"),product_register_value[MODBUS_TSTAT6_CO2_MVALUE]);
				   strTemp+=strUnit;
				   
			   }
			   
		   }
		   else
		   {
			   strTemp=_T("NULL");  
		   }

		   	m_strValueText=strTemp;
	   }
	  

	}
}

void CRelayLabel::DispalyOutputValue_General(int nStatus,COLORREF textClr,COLORREF bkClr)
{
	CString temp_unite;
	CString temp_value;
	int i = nStatus ;

	if(m_Output_data.at(i).digital_analog == BAC_UNITS_ANALOG)
	{
		temp_unite = OutPut_List_Analog_Units[m_Output_data.at(i).range];
		temp_value.Format(_T("%d"),m_Output_data.at(i).value / 1000);
		m_strValueText = temp_value + _T(" ") + temp_unite;
	}
	else if(m_Output_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
	{
		CString Show_Digital;
		if((m_Output_data.at(i).range>22) || (m_Output_data.at(i).range == 0))
		{
			Show_Digital = Digital_Units_Array[0];
		}
		else
		{
			CString temp1;
			CStringArray temparray;

			temp1 = Digital_Units_Array[m_Output_data.at(i).range];
			SplitCStringA(temparray,temp1,_T("/"));
			if((temparray.GetSize()==2))
			{
				if(m_Output_data.at(i).control == 0)
					Show_Digital =temparray.GetAt(0);
				else
					Show_Digital =temparray.GetAt(1);
			}
		}

		m_strValueText = Show_Digital;
	}
	
}

void CRelayLabel::DispalyOutputValue(int nStatus,COLORREF textClr,COLORREF bkClr)
{
	int nModel=product_register_value[MODBUS_PRODUCT_MODEL];
	CString strTemp;
	int itemp=product_register_value[108];

	//==========================================================================
	//ABCDEFG,noH

	if(nModel==2||nModel==1||nModel==4||nModel==12||nModel==16||nModel==PM_PM5E || nModel == PM_PM5E_ARM ||nModel==17||nModel==18||nModel==3)
	{
		for(int i=1;i<=3;i++)
		{
			if(nStatus==i-1)
			{
				if(itemp &( 1<<(i-1)))
					strTemp=_T("On");
				else
					strTemp=_T("Off");
					
				m_strValueText=strTemp;	
				return;
			}
	
	

		}
	}

	////----------------------------------------------------------------------
	//4,5
	if(nModel==1||nModel==4||nModel==12||nModel==16||nModel==PM_PM5E || nModel == PM_PM5E_ARM)//B,C,D,E
	{
		if(nStatus==3)
		{
			if(itemp &( 1<<3))
				strTemp=_T("On");
			else
				strTemp=_T("Off");
			m_strValueText=strTemp;		
			return;
		}
		

		if(nStatus==4)
		{
			if(itemp &( 1<<4))
				strTemp=_T("On");
			else
					strTemp=_T("Off");
			m_strValueText=strTemp;		
			return;

		}
		
	}

		//:DEG 6. 7
		int nRange=product_register_value[186];
		if(nModel==12||nModel==16||nModel==PM_PM5E || nModel == PM_PM5E_ARM ||nModel==18)
		{
			if(nStatus==5)
			{
				if(nRange==0)
				{
					itemp=product_register_value[102];
					if(itemp==0)
						strTemp=_T("Off");
					if(itemp==1)
						strTemp=_T("On");
				}
				else
				{
					//strTemp.Format(_T("%.1f"),product_register_value[102]/100.0);
						//strTemp.Format(_T("%.1f"),product_register_value[102]/100.0);
					float nvalue=0.0;
					if(nRange==1)//0-10v
					{
						//nvalue=product_register_value[102]/100 /10.0 * 100%;
						nvalue=(float)(product_register_value[102]/10.0);
					}
					if(nRange==2)//0-5v
					{
						nvalue=(float)(product_register_value[102]/5.0);
					}
					if(nRange==3)//2-10v
					{
						nvalue=(float)(product_register_value[102]/8.0);
					}
					if(nRange==4)//10-0v
					{
						nvalue=(float)((10-product_register_value[102]/100.0)/10.0 *100);
					}
					strTemp.Format(_T("%.1f%%"),nvalue);
				}
				m_strValueText=strTemp;		
				return;
			}
			
			if(nStatus==6)
			{
				nRange=product_register_value[187];
				if(nRange==0)
				{
					itemp=product_register_value[103];
					if(itemp==0)
						strTemp=_T("Off");
					if(itemp==1)
						strTemp=_T("On");
				}
				else
				{
					//strTemp.Format(_T("%.1f"),product_register_value[103]/100.0);
						float nvalue=0.0;
						if(nRange==1)//0-10v
						{
							//nvalue=product_register_value[102]/100 /10.0 * 100%;
							nvalue=(float)(product_register_value[103]/10.0);
						}
						if(nRange==2)//0-5v
						{
							nvalue=(float)(product_register_value[103]/5.0);
						}
						if(nRange==3)//2-10v
						{
							nvalue=(float)(product_register_value[103]/8.0);
						}
						if(nRange==4)//10-0v
						{
							nvalue=(float)((10-product_register_value[103]/100.0)/10.0 *100);
						}
						strTemp.Format(_T("%.1f%%"),nvalue);

				}
				m_strValueText=strTemp;		
				return;

			}
		
		
		}
			
		if(nModel==2)//A 4,5
		{
			if(nStatus==3)
			{
				nRange=product_register_value[186];
				if(nRange==0)
				{
					itemp=product_register_value[102];
					if(itemp==0)
						strTemp=_T("Off");
					if(itemp==1)
						strTemp=_T("On");
				}
				else
				{
					//strTemp.Format(_T("%.1f"),product_register_value[102]/100.0);
						//strTemp.Format(_T("%.1f"),product_register_value[102]/100.0);
					float nvalue=0.0;
					if(nRange==1)//0-10v
					{
						//nvalue=product_register_value[102]/100 /10.0 * 100%;
						nvalue=(float)(product_register_value[102]/10.0);
					}
					if(nRange==2)//0-5v
					{
						nvalue=(float)(product_register_value[102]/5.0);
					}
					if(nRange==3)//2-10v
					{
						nvalue=(float)(product_register_value[102]/8.0);
					}
					if(nRange==4)//10-0v
					{
						nvalue=(float)((10-product_register_value[102]/100.0)/10.0 *100);
					}
					strTemp.Format(_T("%.1f%%"),nvalue);
				
				}
				m_strValueText=strTemp;		
				return;
			}
		
			if(nStatus==4)
			{
				nRange=product_register_value[187];
				if(nRange==0)
				{
					itemp=product_register_value[103];
					if(itemp==0)
						strTemp=_T("Off");
					if(itemp==1)
						strTemp=_T("On");
				}
				else
				{
					//strTemp.Format(_T("%.1f"),product_register_value[103]/100.0);
						float nvalue=0.0;
					if(nRange==1)//0-10v
					{
						//nvalue=product_register_value[102]/100 /10.0 * 100%;
						nvalue=(float)(product_register_value[103]/10.0);
					}
					if(nRange==2)//0-5v
					{
						nvalue=(float)(product_register_value[103]/5.0);
					}
					if(nRange==3)//2-10v
					{
						nvalue=(float)(product_register_value[103]/8.0);
					}
					if(nRange==4)//10-0v
					{
						nvalue=(float)((10-product_register_value[103]/100.0)/10.0 *100);
					}
					strTemp.Format(_T("%.1f%%"),nvalue);
				}
				m_strValueText=strTemp;		
				return;
			}
			
		}//end A
		//F 4,5
		if(nModel==17||nModel==18)
		{
			if(nStatus==3)
			{
				int itemp=product_register_value[108];
				if(product_register_value[283]==0)
				{
					if(itemp &( 1<<3))
							strTemp=_T("On");
						else
							strTemp=_T("Off");

				}
				else
				{
					strTemp.Format(_T("%d%%"),product_register_value[348]);

				}
				m_strValueText=strTemp;		
				return;	
			}
			

			if(nStatus==4)
			{
				int itemp=product_register_value[108];
				if(product_register_value[284]==0)
				{
					if(itemp &( 1<<4))
							strTemp=_T("On");
						else
							strTemp=_T("Off");

				}
				else
				{
					strTemp.Format(_T("%d%%"),product_register_value[349]);

				}
				m_strValueText=strTemp;		
				return;	

			}
				
		}






/*



	//
	if(nModel==17)//F out4 out5
	{

	}

	if(nModel==18)//G out4 out5
	{

	}
	if(nModel==19)//H out1 out7
	{
	}
	*/

}

void CRelayLabel::DispalyVariableValue_General(int nStatus,COLORREF textClr,COLORREF bkClr)
{
	CString temp_unite;
	CString temp_value;
	int i = nStatus ;
	CString temp1;
	if(m_Variable_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
	{

		if((m_Variable_data.at(i).range>22) || (m_Variable_data.at(i).range == 0))
		{
			temp1 = Digital_Units_Array[0];
		}
		else
		{	
			CStringArray temparray;

			temp1 = Digital_Units_Array[m_Variable_data.at(i).range];
			SplitCStringA(temparray,temp1,_T("/"));
			if((temparray.GetSize()==2))
			{
				if(m_Variable_data.at(i).control == 0)
					temp1 = temparray.GetAt(0);
				else
					temp1 = temparray.GetAt(1);
			}

		}
		m_strValueText = temp1;
	}
	else
	{
		if(m_Variable_data.at(i).range == 20)	//如果是时间;
		{
			//m_variable_list.SetItemText(i,VARIABLE_UNITE,Variable_Analog_Units_Array[m_Variable_data.at(i).range]);
			char temp_char[50];
			int time_seconds = m_Variable_data.at(i).value / 1000;
			intervaltotext(temp_char,time_seconds,0,0);
			CString temp_11;
			MultiByteToWideChar( CP_ACP, 0, temp_char, strlen(temp_char) + 1, 
				temp_11.GetBuffer(MAX_PATH), MAX_PATH );
			temp_11.ReleaseBuffer();		
			//m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_11);
			temp_value = temp_11;
			//temp_value.Format(_T("%d"),m_Variable_data.at(i).value);
			//m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_value);
		}
		else if(m_Variable_data.at(i).range<=sizeof(Variable_Analog_Units_Array)/sizeof(Variable_Analog_Units_Array[0]))
		{
			//m_variable_list.SetItemText(i,VARIABLE_UNITE,Variable_Analog_Units_Array[m_Variable_data.at(i).range]);
			temp_unite = Variable_Analog_Units_Array[m_Variable_data.at(i).range];
			temp_value.Format(_T("%d"),m_Variable_data.at(i).value / 1000);
			//m_variable_list.SetItemText(i,VARIABLE_VALUE,temp_value);
		}
		m_strValueText = temp_value + _T(" ") + temp_unite;
	}
		
}

void CRelayLabel::DispalyRigesterValue(int nStatus,COLORREF textClr,COLORREF bkClr)
{
	//nStatus is rigister;
	if(nStatus<0||nStatus>=513)
		return;

	
	CString strTemp;
	strTemp.Format(_T("%d"),product_register_value[nStatus]);
	m_strValueText=strTemp;	

}
void CRelayLabel::OnTimer(UINT_PTR nIDEvent)
{
	 
	Invalidate();
	CStatic::OnTimer(nIDEvent);
}

void CRelayLabel::OnDestroy()
{
	CStatic::OnDestroy();
	KillTimer(1);
	
}
