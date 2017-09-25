// TempHumSensorForm.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TempHumSensorForm.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CTempHumSensorForm dialog
#define  HUM_SENSOR_CHANGEING_MESSAGE  WM_USER + 1012

#define  HUM_SENSOR_NOCHANGE_MESSAGE  WM_USER + 1013



DWORD WINAPI _BackFreshing_TempHumSensorForm(LPVOID pParam)
{
	CTempHumSensorForm* dlg=(CTempHumSensorForm*)(pParam);

	Sleep(1000);


	while(dlg->IsWindowVisible())
	{

		if (!is_connect())
		{
			Sleep(1000);
			continue;
		}
		if (dlg->m_EDIT_ID!=0)
		{
			Sleep(1000);
			continue;
		}
		if (g_tstat_id==255||g_tstat_id==0)
		{
		Sleep(1000);
		continue;
		}
		Sleep(2000);
		 

		for(int i=0;i<5;i++){
			if (dlg->m_EDIT_ID!=0)
			{
				Sleep(1000);
				continue;
			}
		 
			int multy_ret = 0;
			multy_ret = Read_Multi(g_tstat_id,&multi_register_value[i*100],i*100,100);
			 
			Sleep(100);
			if(multy_ret<0)		 
				break;
		}
		memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
		CString achive_file_path;
		CString temp_serial;
		temp_serial.Format(_T("%d.prog"),g_selected_serialnumber);
		achive_file_path = g_achive_folder + _T("\\") + temp_serial;
		Save_Product_Value_Cache(achive_file_path);
		::PostMessage(dlg->m_hWnd,HUM_SENSOR_CHANGEING_MESSAGE,0,0);

	}



	return 1;
}


IMPLEMENT_DYNCREATE(CTempHumSensorForm, CFormView)

	CTempHumSensorForm::CTempHumSensorForm()
	:CFormView(CTempHumSensorForm::IDD)
//	, m_mschart(0)
{
m_EDIT_ID=0;
}

CTempHumSensorForm::~CTempHumSensorForm()
{
	if(hFirstThread != NULL)
		TerminateThread(hFirstThread, 0);
}

void CTempHumSensorForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_SENSOR_TABLE, m_sensor_table);
	/*DDX_Control(pDX, IDC_MSCHART1, m_mschart); */
	//  DDX_Control(pDX, IDC_COMBO_RANGE1, m_outrange1);
	DDX_Control(pDX, IDC_COMBO_RANGE2, m_combox_range2);
	DDX_Control(pDX, IDC_COMBO_AM1, m_combox_am1);
	DDX_Control(pDX, IDC_COMBO_AM2, m_combox_am2);
	DDX_Control(pDX, IDC_COMBO_AM3, m_combox_am3);
	DDX_Control(pDX, IDC_COMBO_AM4, m_combox_am4);
	DDX_Control(pDX, IDC_COMBO_RANGE1, m_combox_range1);
	DDX_Control(pDX, IDC_COMBO_UNITS, m_combox_unit);
	DDX_Control(pDX, IDC_EDIT_HUM_VALUE, m_edit_hum);
	DDX_Control(pDX, IDC_EDIT_TEMP_VALUE, m_edit_temp);
	DDX_Control(pDX, IDC_EDIT_HUM_FILTER, m_edit_hum_filter);
	DDX_Control(pDX, IDC_EDIT_HUM_SIGNAL, m_edit_hum_signal);
	DDX_Control(pDX, IDC_EDIT_TEMP_FILTER, m_edit_temp_filter);
	DDX_Control(pDX, IDC_EDIT_TEMP_SIGNAL, m_edit_temp_signal);
	DDX_Control(pDX, IDC_EDIT_FRE, m_Edit_Fre);
	DDX_Control(pDX, IDC_LIST_USER, m_user_list);
	DDX_Control(pDX, IDC_LIST_FACTORY, m_factory_list);
	DDX_Control(pDX, IDC_CHECK_DEFAULT, m_check_default);
	DDX_Control(pDX, IDC_CHECK_USER, m_check_user);
}


BEGIN_MESSAGE_MAP(CTempHumSensorForm, CFormView)
ON_MESSAGE(MY_RESUME_DATA, ResumeMessageCallBack)
//ON_EN_KILLFOCUS(IDC_EDIT_TEMP_VALUE, &CTempHumSensorForm::OnEnKillfocusEditTempValue)
ON_CBN_SELCHANGE(IDC_COMBO_UNITS, &CTempHumSensorForm::OnCbnSelchangeComboUnits)
//ON_EN_KILLFOCUS(IDC_EDIT_HUM_VALUE, &CTempHumSensorForm::OnEnKillfocusEditHumValue)
ON_CBN_SELCHANGE(IDC_COMBO_AM1, &CTempHumSensorForm::OnCbnSelchangeComboAm1)
ON_CBN_SELCHANGE(IDC_COMBO_AM2, &CTempHumSensorForm::OnCbnSelchangeComboAm2)
ON_CBN_SELCHANGE(IDC_COMBO_RANGE1, &CTempHumSensorForm::OnCbnSelchangeComboRange1)
ON_CBN_SELCHANGE(IDC_COMBO_RANGE2, &CTempHumSensorForm::OnCbnSelchangeComboRange2)
ON_WM_DESTROY()
ON_EN_SETFOCUS(IDC_EDIT_TEMP_VALUE, &CTempHumSensorForm::OnEnSetfocusEditTempValue)
ON_EN_SETFOCUS(IDC_EDIT_HUM_VALUE, &CTempHumSensorForm::OnEnSetfocusEditHumValue)
//ON_EN_KILLFOCUS(IDC_EDIT_TEMP_FILTER, &CTempHumSensorForm::OnEnKillfocusEditTempFilter)
//ON_EN_KILLFOCUS(IDC_EDIT_HUM_FILTER, &CTempHumSensorForm::OnEnKillfocusEditHumFilter)
ON_EN_SETFOCUS(IDC_EDIT_TEMP_FILTER, &CTempHumSensorForm::OnEnSetfocusEditTempFilter)
ON_EN_SETFOCUS(IDC_EDIT_HUM_FILTER, &CTempHumSensorForm::OnEnSetfocusEditHumFilter)
ON_BN_CLICKED(IDC_BUTTON_HUM_DEFAULT, &CTempHumSensorForm::OnBnClickedButtonHumDefault)
ON_BN_CLICKED(IDC_BUTTON_TEMP_DEFAULT, &CTempHumSensorForm::OnBnClickedButtonTempDefault)
ON_BN_CLICKED(IDD_USER_TABLE_ADD, &CTempHumSensorForm::OnBnClickedUserTableAdd)
ON_BN_CLICKED(IDD_USER_TABLE_DEL, &CTempHumSensorForm::OnBnClickedUserTableDel)

 ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_User_Item)
 ON_BN_CLICKED(IDC_CHECK_DEFAULT, &CTempHumSensorForm::OnBnClickedCheckDefault)
 ON_BN_CLICKED(IDC_CHECK_USER, &CTempHumSensorForm::OnBnClickedCheckUser)
END_MESSAGE_MAP()
LRESULT  CTempHumSensorForm::ResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	UINT temp_id;
	_MessageWriteOneInfo *Write_Struct_feedback =(_MessageWriteOneInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	vector <int>::iterator Iter;
	if(msg_result)
	{
		int indexid = -1;
		for (int i=0;i<(int)Change_Color_ID.size();i++)
		{
			if(Change_Color_ID.at(i)!=Write_Struct_feedback->CTRL_ID)
				continue;
			else
				indexid = i;
		}

		if(indexid!=-1)
		{
			Iter = Change_Color_ID.begin()+indexid;
			Change_Color_ID.erase(Iter);
		}

		CString temp;
		temp.Format(_T("Change \"%s\" value from %d to %d success!"),
			Write_Struct_feedback->Changed_Name,
			Write_Struct_feedback->old_value,
			Write_Struct_feedback->new_value);
		temp_id = Write_Struct_feedback->CTRL_ID;
		SetPaneString(1,temp);
		product_register_value[Write_Struct_feedback->address]= Write_Struct_feedback->new_value;
		if(Write_Struct_feedback!=NULL)
			delete Write_Struct_feedback;

	}
	else
	{
		CString temp;
		temp.Format(_T("Change \"%s\" value from %d to %d Fail!"),
			Write_Struct_feedback->Changed_Name,
			Write_Struct_feedback->old_value,
			Write_Struct_feedback->new_value);

		temp_id = Write_Struct_feedback->CTRL_ID;
		SetPaneString(1,temp);
		Beep(10,100);
		product_register_value[Write_Struct_feedback->address]= Write_Struct_feedback->old_value;
		//GetDlgItem(Write_Struct_feedback->CTRL_ID)->SetWindowTextW(_T(""));

		int indexid = -1;
		for (int i=0;i<(int)Change_Color_ID.size();i++)
		{
			if(Change_Color_ID.at(i)!=Write_Struct_feedback->CTRL_ID)
				continue;
			else
				indexid = i;
		}
		Iter = Change_Color_ID.begin()+indexid;
		if(indexid!=-1)
		{
			Iter = Change_Color_ID.begin()+indexid;
			Change_Color_ID.erase(Iter);
		}

		if(Write_Struct_feedback!=NULL)
		{
			delete Write_Struct_feedback;
		}
	}
//	Reflesh_ParameterDlg();
	GetDlgItem(temp_id)->Invalidate();

	return 0;
}

void CTempHumSensorForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
#if 1	
	//设置排/行数量
	m_sensor_table.put_Cols(3);
	m_sensor_table.put_Rows(11);//包括标题栏
	//显示横标题
	m_sensor_table.put_TextMatrix(0,0,_T(""));
	m_sensor_table.put_TextMatrix(0,1,_T("Frequency"));
	m_sensor_table.put_TextMatrix(0,2,_T("RH"));
 




	 




	//显示纵标题
	CString str;
	for(int i=1;i<11;i++)
	{

		str.Format(_T("%d"),i);

		m_sensor_table.put_TextMatrix(i,0,str);	 
	}






#endif  
   m_combox_range1.ResetContent();
   m_combox_range1.AddString(L"0-10V");
   m_combox_range1.AddString(L"0-5V");
   m_combox_range1.AddString(L"4-20mA");

   m_combox_range2.ResetContent();
   m_combox_range2.AddString(L"0-10V");
   m_combox_range2.AddString(L"0-5V");
   m_combox_range2.AddString(L"4-20mA");

   m_combox_unit.ResetContent();
   m_combox_unit.AddString(L"C");
   m_combox_unit.AddString(L"F");

   m_combox_am1.ResetContent();
   m_combox_am1.AddString(L"Auto");
   m_combox_am1.AddString(L"Manual");

   m_combox_am2.ResetContent();
   m_combox_am2.AddString(L"Auto");
   m_combox_am2.AddString(L"Manual");

   m_combox_am3.ResetContent();
   m_combox_am3.AddString(L"Auto");
   m_combox_am3.AddString(L"Manual");

   m_combox_am4.ResetContent();
   m_combox_am4.AddString(L"Auto");
   m_combox_am4.AddString(L"Manual");

  

}


void CTempHumSensorForm::Fresh(){
	 
Changing_Data();
NoChange_Data();

Initial_UserList();
Initial_FactoryList();
ShowChecks();

if(hFirstThread != NULL)
	TerminateThread(hFirstThread, 0);
hFirstThread=NULL;
if (!hFirstThread)
{
	hFirstThread = CreateThread(NULL,NULL,_BackFreshing_TempHumSensorForm,this,NULL,0);
}
}

void CTempHumSensorForm::Changing_Data(){
CString StrTemp;
	if (product_register_value[MODBUS_OUTPUT1_SCALE]<=3&&product_register_value[MODBUS_OUTPUT1_SCALE]>=1)
	{
		m_combox_range1.SetCurSel(product_register_value[MODBUS_OUTPUT1_SCALE]-1);
	}
	else
	{
		m_combox_range1.SetCurSel(0);
	}
	if (product_register_value[MODBUS_OUTPUT2_SCALE]<=3&&product_register_value[MODBUS_OUTPUT2_SCALE]>=1)
	{
		m_combox_range2.SetCurSel(product_register_value[MODBUS_OUTPUT2_SCALE]-1);
	}
	else
	{
		m_combox_range2.SetCurSel(0);
	}
	if (product_register_value[MODBUS_DEGC_OR_F]>=0&&product_register_value[MODBUS_DEGC_OR_F]<=1)
	{
	  m_combox_unit.SetCurSel(product_register_value[MODBUS_DEGC_OR_F]);

	}
	else
	{
	   m_combox_unit.SetCurSel(0);
	}

	RegisterValue AMValue;
	 for (int i=0;i<16;i++)
	 {
	  AMValue.BOOLRegValue[i]=FALSE;
	 }
	   AMValue.RegValue=product_register_value[MODBUS_OUTPUT_CALIBRATION_AUTO_MANUAL];
	   m_combox_am1.SetCurSel(AMValue.BOOLRegValue[0]);
	   m_combox_am3.SetCurSel(AMValue.BOOLRegValue[0]);
	   m_combox_am2.SetCurSel(AMValue.BOOLRegValue[1]);
	   m_combox_am4.SetCurSel(AMValue.BOOLRegValue[1]);
	     
	   float offsetvalue,Vtemp,Vhum;
	   float Temp,HUM;
	   offsetvalue=((float)((short)product_register_value[MODBUS_OUTPUT_TEMP_SENSOR_RANGE]));
	   if (AMValue.BOOLRegValue[0]==0)//Auto
	   {
		   if (product_register_value[MODBUS_DEGC_OR_F]==0)
		   {
		       Temp=((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]))/10;
			   /*StrTemp.Format(_T("%.1f"),((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]))/10);
			   m_edit_temp.SetWindowTextW(StrTemp);*/
		   } 
		   else
		   {
		       Temp=((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP_F]))/10;
			   /*StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_TEMPRATURE_CHIP_F]);
			   m_edit_temp.S etWindowTextW(StrTemp);*/
		   }
	   }
	   else//Manual 
	   {
	      Temp=((float)((short)product_register_value[MODBUS_OUTPUT_MANUAL_VALUE_TEM]))/10;
		  
	   }
	   StrTemp.Format(_T("%.1f"),Temp);
	   m_edit_temp.SetWindowTextW(StrTemp);

	   if (AMValue.BOOLRegValue[0]==0)//Auto
	   {   
		   if (product_register_value[MODBUS_OUTPUT1_SCALE]==1)
		   {
			   Temp=((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]));
			   Vtemp=(Temp+offsetvalue)/100;
			   if (Vtemp>10)
			   {
				   Vtemp=10;
			   }
			   if (Vtemp<0)
			   {
				   Vtemp=0;
			   }
			   StrTemp.Format(_T("%.2f v"),Vtemp);
		   }
		   else if (product_register_value[MODBUS_OUTPUT1_SCALE]==2)
		   {
			   Temp=((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]));
			   Vtemp=(Temp+offsetvalue)/200;
			   if (Vtemp>5)
			   {
				   Vtemp=5;
			   }
			   if (Vtemp<0)
			   {
				   Vtemp=0;
			   }
			   StrTemp.Format(_T("%.2f v"),Vtemp);
		   }
		   else if (product_register_value[MODBUS_OUTPUT1_SCALE]==3)
		   {
			   if (product_register_value[MODBUS_DEGC_OR_F]==0)
			   {
				   Temp=((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP]));
				   Vtemp=((Temp+offsetvalue)/1000)*16+4;
				   if (Vtemp>20)
				   {
					   Vtemp=20;
				   }
				   if (Vtemp<4)
				   {
					   Vtemp=4;
				   }

			   } 
			   else
			   {
				   Temp=((float)((short)product_register_value[MODBUS_TEMPRATURE_CHIP_F]));
				   Vtemp=((Temp+offsetvalue)/1000)*16+4;
				   if (Vtemp>20)
				   {
					   Vtemp=20;
				   }
				   if (Vtemp<4)
				   {
					   Vtemp=4;
				   }


			   }
			   StrTemp.Format(_T("%.2f ma"),Vtemp);
		   }
		   else{
			   StrTemp=_T("0");
		   }

	   }
	   else
	   { 
	      Temp=((float)((short)product_register_value[MODBUS_OUTPUT_MANUAL_VALUE_TEM]));
		   if (product_register_value[MODBUS_OUTPUT1_SCALE]==1)
		   {
			   Vtemp=(Temp+offsetvalue)/100;
			   if (Vtemp>10)
			   {
				   Vtemp=10;
			   }
			   if (Vtemp<0)
			   {
				   Vtemp=0;
			   }
			   StrTemp.Format(_T("%.2f v"),Vtemp);
		   }
		   else if (product_register_value[MODBUS_OUTPUT1_SCALE]==2)
		   {
			   Vtemp=(Temp+offsetvalue)/200;
			   if (Vtemp>5)
			   {
				   Vtemp=5;
			   }
			   if (Vtemp<0)
			   {
				   Vtemp=0;
			   }
			   StrTemp.Format(_T("%.2f v"),Vtemp);
		   }
		   else if (product_register_value[MODBUS_OUTPUT1_SCALE]==3)
		   {
				   Vtemp=((Temp+offsetvalue)/1000)*16+4;
				   if (Vtemp>20)
				   {
					   Vtemp=20;
				   }
				   if (Vtemp<4)
				   {
					   Vtemp=4;
				   }
			   StrTemp.Format(_T("%.2f ma"),Vtemp);
		   }
		   else{
			   StrTemp=_T("0");
		   }
	   }

	    m_edit_temp_signal.SetWindowTextW(StrTemp);




		CString humsignal;
	   if (AMValue.BOOLRegValue[1]==0)
	   {
		  /* StrTemp.Format(_T("%d"),(short)product_register_value[MODUBS_HUMIDITY_RH]);
		   m_edit_hum.SetWindowTextW(StrTemp);*/
		   HUM=((float)((short)product_register_value[MODUBS_HUMIDITY_RH]))/10;
		   if (product_register_value[MODBUS_OUTPUT1_SCALE]==1)
		   {
		   Vhum=HUM/10;
		   if (Vhum>10)
		   {
			   Vhum=10;
		   }
		   if (Vhum<0)
		   {
			   Vhum=0;
		   }
		   humsignal.Format(_T("%.2fv"),Vhum);
		   }
		   else if (product_register_value[MODBUS_OUTPUT1_SCALE]==2)
		   {
		    Vhum=HUM/20;
			if (Vhum>5)
			{
				Vhum=5;
			}
			if (Vhum<0)
			{
				Vhum=0;
			}
			 humsignal.Format(_T("%.2fv"),Vhum);
		   }
		   else if (product_register_value[MODBUS_OUTPUT1_SCALE]==3)
		   {
		    Vhum=(HUM/100)*16+4;
			if (Vhum>20)
			{
				Vhum=20;
			}
			if (Vhum<4)
			{
				Vhum=4;
			}
			 humsignal.Format(_T("%.2fma"),Vhum);
		   }
		   else
		   {
		   humsignal=_T("0");
		   }
	   }
	   else
	   {

	      HUM=((float)((short)product_register_value[MODBUS_OUTPUT_MANUAL_VALUE_HUM]))/10;
		  if (product_register_value[MODBUS_OUTPUT1_SCALE]==1)
		  {
			  Vhum=HUM/10;
			  if (Vhum>10)
			  {
				  Vhum=10;
			  }
			  if (Vhum<0)
			  {
				  Vhum=0;
			  }
			  humsignal.Format(_T("%.2fv"),Vhum);
		  }
		  else if (product_register_value[MODBUS_OUTPUT1_SCALE]==2)
		  {
			  Vhum=HUM/20;
			  if (Vhum>5)
			  {
				  Vhum=5;
			  }
			  if (Vhum<0)
			  {
				  Vhum=0;
			  }
			  humsignal.Format(_T("%.2fv"),Vhum);
		  }
		  else if (product_register_value[MODBUS_OUTPUT1_SCALE]==3)
		  {
			  Vhum=(HUM/100)*16+4;
			  if (Vhum>20)
			  {
				  Vhum=20;
			  }
			  if (Vhum<4)
			  {
				  Vhum=4;
			  }
			  humsignal.Format(_T("%.2fma"),Vhum);
		  }
	   }
	    StrTemp.Format(_T("%.1f"),HUM);
		m_edit_hum.SetWindowTextW(StrTemp);
		m_edit_hum_signal.SetWindowTextW(humsignal);

 
	   StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_TEMP_FILTER]);
	   m_edit_temp_filter.SetWindowTextW(StrTemp);
	   StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUM_FILTER]);
	   m_edit_hum_filter.SetWindowTextW(StrTemp);
	 

 
}
void CTempHumSensorForm::NoChange_Data(){
CString StrTemp;
StrTemp.Format(_T("%d"),product_register_value[MODBUS_ADDRESS]);
GetDlgItem(IDC_ID_EDIT)->SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_SERIALNUMBER_LOWORD]+product_register_value[MODBUS_SERIALNUMBER_LOWORD+1]*256+product_register_value[MODBUS_SERIALNUMBER_HIWORD]*256*256+product_register_value[MODBUS_SERIALNUMBER_HIWORD+1]*256*256*256);
GetDlgItem(IDC_SERIALNUM_EDIT)->SetWindowTextW(StrTemp);
StrTemp.Format(_T("%.1f"),(float)(product_register_value[MODBUS_VERSION_NUMBER_LO]+product_register_value[MODBUS_VERSION_NUMBER_HI]*256)/10);
GetDlgItem(IDC_FIRMWARV_EDIT)->SetWindowTextW(StrTemp);
StrTemp.Format(_T("%s"),GetProductName(product_register_value[MODBUS_PRODUCT_MODEL]));
GetDlgItem(IDC_MODEL_EDIT)->SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_HARDWARE_REV]);
GetDlgItem(IDC_HARDWAREV_EDIT)->SetWindowTextW(StrTemp);
StrTemp.Format(_T("%d"),product_register_value[MODBUS_PIC_VERSION]);
GetDlgItem(IDC_PIC_EDIT)->SetWindowTextW(StrTemp);

CString str;
for(int i=1;i<11;i++)
{

	str.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+2*(i-1)]);

	m_sensor_table.put_TextMatrix(i,1,str);	 

	str.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+2*(i-1)+1]);
	m_sensor_table.put_TextMatrix(i,2,str);	
}
	  str.Format(_T("%d"),product_register_value[374]);
  m_Edit_Fre.SetWindowText(str);


}


LRESULT CTempHumSensorForm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	
	if (message==HUM_SENSOR_CHANGEING_MESSAGE)
	{
		Changing_Data();
	}
	return CFormView::WindowProc(message, wParam, lParam);
}


void CTempHumSensorForm::OnEnKillfocusEditTempValue()
{

   CString StrTemp;
   m_edit_temp.GetWindowTextW(StrTemp);
   unsigned short RegAddress,RegValue;
   RegValue=(unsigned short)(_wtof(StrTemp)*10);

   RegisterValue AMValue;
   for (int i=0;i<16;i++)
   {
	   AMValue.BOOLRegValue[i]=FALSE;
   }
   AMValue.RegValue=product_register_value[MODBUS_OUTPUT_CALIBRATION_AUTO_MANUAL];


    if (AMValue.BOOLRegValue[0]==0)//Auto
	   {
		   if (product_register_value[MODBUS_DEGC_OR_F]==0)
		   {
		      RegAddress=MODBUS_TEMPRATURE_CHIP;
		      
			   
		   } 
		   else
		   {
		    RegAddress=MODBUS_TEMPRATURE_CHIP_F;
		        
		   }
	   }
	   else//Manual 
	   {
	       
		   RegAddress=MODBUS_OUTPUT_MANUAL_VALUE_TEM;
		  
	   }

	   
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAddress,RegValue,
		product_register_value[RegAddress],this->m_hWnd,IDC_EDIT_TEMP_VALUE,_T("Temperature"));

		m_EDIT_ID=0;

}


void CTempHumSensorForm::OnCbnSelchangeComboUnits()
{

   
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_DEGC_OR_F,m_combox_unit.GetCurSel(),
		product_register_value[MODBUS_DEGC_OR_F],this->m_hWnd,0,_T("Unit"));
}


void CTempHumSensorForm::OnEnKillfocusEditHumValue()
{

	CString StrTemp;
	m_edit_hum.GetWindowTextW(StrTemp);
	unsigned short RegAddress,RegValue;
	RegValue=(unsigned short)(_wtof(StrTemp)*10);

	RegisterValue AMValue;
	for (int i=0;i<16;i++)
	{
		AMValue.BOOLRegValue[i]=FALSE;
	}
	AMValue.RegValue=product_register_value[MODBUS_OUTPUT_CALIBRATION_AUTO_MANUAL];


	  if (AMValue.BOOLRegValue[1]==0)
	   {
		  /* StrTemp.Format(_T("%d"),(short)product_register_value[MODUBS_HUMIDITY_RH]);
		   m_edit_hum.SetWindowTextW(StrTemp);*/
		   RegAddress=MODUBS_HUMIDITY_RH;
		 //  HUM=((float)((short)product_register_value[MODUBS_HUMIDITY_RH]))/10;
		 }
		 else
		 {
			RegAddress=MODBUS_OUTPUT_MANUAL_VALUE_HUM;
		 }


	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,RegAddress,RegValue,
		product_register_value[RegAddress],this->m_hWnd,IDC_EDIT_HUM_VALUE,_T("Humidity"));
		m_EDIT_ID=0;
}


void CTempHumSensorForm::OnCbnSelchangeComboAm1()
{
   m_EDIT_ID=1;
	RegisterValue AMValue;
	for (int i=0;i<16;i++)
	{
		AMValue.BOOLRegValue[i]=FALSE;
	}
	AMValue.RegValue=product_register_value[MODBUS_OUTPUT_CALIBRATION_AUTO_MANUAL];
	int cur=m_combox_am1.GetCurSel();
	if (cur==0)
	{
	 AMValue.BOOLRegValue[0]=FALSE;
	}
	else{
	AMValue.BOOLRegValue[0]=TRUE;
	}
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_OUTPUT_CALIBRATION_AUTO_MANUAL,AMValue.RegValue,
		product_register_value[MODBUS_OUTPUT_CALIBRATION_AUTO_MANUAL],this->m_hWnd,0,_T("Auto/Manual"));
		m_EDIT_ID=0;
}


void CTempHumSensorForm::OnCbnSelchangeComboAm2()
{
m_EDIT_ID=1;
	RegisterValue AMValue;
	for (int i=0;i<16;i++)
	{
		AMValue.BOOLRegValue[i]=FALSE;
	}
	AMValue.RegValue=product_register_value[MODBUS_OUTPUT_CALIBRATION_AUTO_MANUAL];
	int cur=m_combox_am2.GetCurSel();
	if (cur==0)
	{
		AMValue.BOOLRegValue[1]=FALSE;
	}
	else{
		AMValue.BOOLRegValue[1]=TRUE;
	}
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_OUTPUT_CALIBRATION_AUTO_MANUAL,AMValue.RegValue,
		product_register_value[MODBUS_OUTPUT_CALIBRATION_AUTO_MANUAL],this->m_hWnd,0,_T("Auto/Manual"));
		m_EDIT_ID=0;
}


void CTempHumSensorForm::OnCbnSelchangeComboRange1()
{
	
}


void CTempHumSensorForm::OnCbnSelchangeComboRange2()
{
	
}


void CTempHumSensorForm::OnDestroy()
{

	if(hFirstThread != NULL)
		TerminateThread(hFirstThread, 0);
	hFirstThread=NULL;
	CFormView::OnDestroy();

	
}


void CTempHumSensorForm::OnEnSetfocusEditTempValue()
{
	m_EDIT_ID=IDC_EDIT_TEMP_VALUE; 
   
}


void CTempHumSensorForm::OnEnSetfocusEditHumValue()
{
	m_EDIT_ID=IDC_EDIT_HUM_VALUE;
}


BOOL CTempHumSensorForm::PreTranslateMessage(MSG* pMsg)
{
	
	if(WM_KEYDOWN == pMsg->message)
	{

		CEdit* pEdit = (CEdit*)GetDlgItem(m_EDIT_ID);

		ASSERT(pEdit);

		if(pMsg->hwnd == pEdit->GetSafeHwnd() && VK_RETURN == pMsg->wParam)
		{

			switch (m_EDIT_ID)
			{
			case IDC_EDIT_TEMP_VALUE:
				{
					//OnEnKillfocusEditFilter();
					OnEnKillfocusEditTempValue();
				}
				break;
			case IDC_EDIT_HUM_VALUE:
				{
				//	OnEnKillfocusPressureSensor();
					OnEnKillfocusEditHumValue();
				}
				break;
			case IDC_EDIT_TEMP_FILTER:
				{
					//	OnEnKillfocusPressureSensor();
					 
					OnEnKillfocusEditTempFilter();
				}
				break;
			case IDC_EDIT_HUM_FILTER:
				{
					//	OnEnKillfocusPressureSensor();
					OnEnKillfocusEditHumFilter();
				}
				break;
			default:
				{
					return -1;
				}
			}

			return TRUE;

		}

	}

	return CFormView::PreTranslateMessage(pMsg);
}


void CTempHumSensorForm::OnEnKillfocusEditTempFilter()
{

	/*StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_TEMP_FILTER]);
	m_edit_temp_filter.SetWindowTextW(StrTemp);
	StrTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUM_FILTER]);
	m_edit_hum_filter.SetWindowTextW(StrTemp);*/
	if (m_EDIT_ID==0)
	{
	return;
	}
	GetDlgItem(m_EDIT_ID)->GetWindowTextW(m_newvalue);
	if (m_newvalue.CompareNoCase(m_oldvalue)==0)
	{
	return;
	}
	int RegValue=_wtoi(m_newvalue);
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_TEMP_FILTER,RegValue,
		product_register_value[MODBUS_TEMP_FILTER],this->m_hWnd,IDC_EDIT_TEMP_FILTER,_T("Temperature Filter"));
	m_EDIT_ID=0;
}


void CTempHumSensorForm::OnEnKillfocusEditHumFilter()
{
	if (m_EDIT_ID==0)
    {
        return;
	}
	GetDlgItem(m_EDIT_ID)->GetWindowTextW(m_newvalue);
	if (m_newvalue.CompareNoCase(m_oldvalue)==0)
	{
		return;
	}
	int RegValue=_wtoi(m_newvalue);
	Post_Thread_Message(MY_WRITE_ONE,g_tstat_id,MODBUS_HUM_FILTER,RegValue,
		product_register_value[MODBUS_HUM_FILTER],this->m_hWnd,IDC_EDIT_HUM_VALUE,_T("Humidity Filter"));
	m_EDIT_ID=0;
}


void CTempHumSensorForm::OnEnSetfocusEditTempFilter()
{
	 m_EDIT_ID=IDC_EDIT_TEMP_FILTER;
	 GetDlgItem(m_EDIT_ID)->GetWindowTextW(m_oldvalue);
}


void CTempHumSensorForm::OnEnSetfocusEditHumFilter()
{
	 m_EDIT_ID=IDC_EDIT_HUM_FILTER;
	 GetDlgItem(m_EDIT_ID)->GetWindowTextW(m_oldvalue);
}


void CTempHumSensorForm::OnBnClickedButtonHumDefault()
{
	write_one(g_tstat_id,MODBUS_HUM_OFFSET,0);
	/*if (ret<0)
	{
	 AfxMessageBox(_T())
	}*/
}
void CTempHumSensorForm::Initial_UserList(){

	BOOL is_grad = TRUE;
	if (product_register_value[454] == 1)
	{
		is_grad = FALSE;
	}
	int rows = product_register_value[455];
	if (rows < 0)
	{
		rows = 10;
	}

	CString strTemp;
	m_user_list.ShowWindow(SW_HIDE);
	m_user_list.DeleteAllItems();
	while ( m_user_list.DeleteColumn (0)) ;

	m_user_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_user_list.SetExtendedStyle(m_user_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
	m_user_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_user_list.InsertColumn(1, _T("Frequency"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_user_list.InsertColumn(2, _T("Humidity(%)"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
	g_hwnd_now = this->m_hWnd;

	CString index;
	for (int i=0;i< rows;i++)
	{
		index.Format(_T("%d"),i+1);
		m_user_list.InsertItem(i,index);
	}

	//	CString strTemp;
	for (int row=0;row<rows;row++)
	{
		

		strTemp.Format(_T("%d"), product_register_value[456+2*row+1]);
		m_user_list.SetItemText(row,1,strTemp);

		strTemp.Format(_T("%0.1f"), ((float)((unsigned short)product_register_value[456+2*row]))/10);
		m_user_list.SetItemText(row,2,strTemp);

		if (is_grad)
		{
			m_user_list.SetCellEnabled(row,0,FALSE);
			m_user_list.SetCellEnabled(row,1,FALSE);
			m_user_list.SetItemBkColor(row,0,DISABLE_COLOR_CELL);
			m_user_list.SetItemBkColor(row,1,DISABLE_COLOR_CELL);
			m_user_list.SetItemBkColor(row,2,DISABLE_COLOR_CELL); 
		}
		else
		{
			m_user_list.SetCellEnabled(row,0,TRUE);
			m_user_list.SetCellEnabled(row,1,TRUE);
			m_user_list.SetItemBkColor(row,0,RGB(255,255,255));
			m_user_list.SetItemBkColor(row,1,RGB(255,255,255));
			m_user_list.SetItemBkColor(row,2,RGB(255,255,255)); 

		}
	}
	m_user_list.ShowWindow(TRUE);
}
void CTempHumSensorForm::Initial_FactoryList(){

	m_factory_list.ShowWindow(SW_HIDE);
	m_factory_list.DeleteAllItems();
	while ( m_factory_list.DeleteColumn (0)) ;

	m_factory_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	m_factory_list.SetExtendedStyle(m_factory_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));	
	m_factory_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
	m_factory_list.InsertColumn(1, _T("Frequency"), 70, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	m_factory_list.InsertColumn(2, _T("Humidity(%)"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
	g_hwnd_now = this->m_hWnd;


	CString index;
	for (int i=1;i< 11;i++)
	{
		index.Format(_T("%d"),i);
		m_factory_list.InsertItem(i-1,index);
	}

	CString strTemp;
	for (int row=1;row<11;row++)
	{

		strTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+2*(row-1)]);
		m_factory_list.SetItemText(row-1,1,strTemp); 


		strTemp.Format(_T("%d"),(short)product_register_value[MODBUS_HUMCOUNT1_H+2*(row-1)+1]);
		m_factory_list.SetItemText(row-1,2,strTemp);

		m_factory_list.SetCellEnabled(row-1,0,FALSE);
		m_factory_list.SetCellEnabled(row-1,1,FALSE);
		m_factory_list.SetItemBkColor(row-1,0,DISABLE_COLOR_CELL);
		m_factory_list.SetItemBkColor(row-1,1,DISABLE_COLOR_CELL);
		m_factory_list.SetItemBkColor(row-1,2,DISABLE_COLOR_CELL);  


	}
	m_factory_list.ShowWindow(TRUE);
}
void CTempHumSensorForm::OnBnClickedButtonTempDefault()
{
	write_one(g_tstat_id,MODBUS_TEMP_OFFSET,0);
}


void CTempHumSensorForm::OnBnClickedUserTableAdd()
{
	if (product_register_value[455] >= 10)
	{
		AfxMessageBox(_T("Can't Add,Current Point can't be more than 10!"));
		return;
	}
	m_EDIT_ID = 1;
	int PointNum = product_register_value[455]+1;
	int ret = write_one(g_tstat_id,455,PointNum);
	int Read_PointNum = read_one(g_tstat_id,455);
	int RegAddress;
	if (ret>0 || Read_PointNum == PointNum)
	{
		++product_register_value[455];
		RegAddress=456+2*(PointNum-1);
		int ret=write_one(g_tstat_id,RegAddress,0);
	 
	}
	else
	{
		AfxMessageBox(_T("Add Fail,Try again!"));
		m_EDIT_ID = 0;
		return;
	}

	unsigned short Data[20];
	ret = Read_Multi(g_tstat_id,Data,456,20);
	if (ret>0)
	{
		for (int i = 0 ; i < 20 ; i++)
		{
			product_register_value[456 + i] = Data[ i ];
		}
	}
	m_EDIT_ID = 0;
	Initial_UserList();
}

void CTempHumSensorForm::ShowChecks(){
	BOOL is_grad = TRUE;
	if (product_register_value[454] == 1)
	{
		is_grad = FALSE;
	}
	if (is_grad)
	{
		m_check_default.SetCheck(TRUE);
		m_check_user.SetCheck(FALSE);
	} 
	else
	{
		m_check_default.SetCheck(FALSE);
		m_check_user.SetCheck(TRUE);
	}

	Initial_UserList();
}
void CTempHumSensorForm::OnBnClickedUserTableDel()
{
	if (product_register_value[455] <= 0)
	{
		AfxMessageBox(_T("Can't Add,Current Point can't be less than 0!"));
		return;
	}
	m_EDIT_ID = 1;
	int ret = write_one(g_tstat_id,455,product_register_value[455]-1);
	if (ret>0)
	{
		--product_register_value[455];

		int RegAddress;
		//int row =m_sensor_calibrate_table.get_Rows();
		int row =m_user_list.GetItemCount();
		row -= 1;//末尾行
		RegAddress=456+2*row;
		int ret=write_one(g_tstat_id,RegAddress,0);


	}
	else
	{
		AfxMessageBox(_T("Add Fail,Try again!"));
		m_EDIT_ID = 0;
		return;
	}

	unsigned short Data[20];
	ret = Read_Multi(g_tstat_id,Data,456,20);
	if (ret>0)
	{
		for (int i = 0 ; i < 20 ; i++)
		{
			product_register_value[456 + i] = Data[ i ];
		}
	}
	m_EDIT_ID = 0;
	Initial_UserList();
}

 



LRESULT CTempHumSensorForm::Fresh_User_Item(WPARAM wParam,LPARAM lParam){
	
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;  
	CString temp_task_info;
	 	 m_EDIT_ID = 1;
		if (Changed_SubItem == 2)
		{
			 
			CString New_CString =  m_user_list.GetItemText(Changed_Item,Changed_SubItem);
			int ItemValue = (int)(_wtof(New_CString)*10);
			int RegAddress;



			if(New_CString.IsEmpty())
			{
			 
				Initial_UserList();  
				return FALSE;
			}



			RegAddress=456+2*Changed_Item;

			if(product_register_value[RegAddress]==ItemValue)	//Add this to judge weather this value need to change.
			{
			   
				Initial_UserList();  
				return FALSE;
			}

			int ret=write_one(g_tstat_id,RegAddress,ItemValue);
			if (ret>0)
			{
				product_register_value[RegAddress]=ItemValue;		 
			}
			else
			{
				AfxMessageBox(_T("Write Fail!"));
				m_user_list.Set_Edit(false);
				return false;
			}

		 
			unsigned short Data[20];
			ret = Read_Multi(g_tstat_id,Data,456,20);
			if (ret>0)
			{
				for (int i = 0 ; i < 20 ; i++)
				{
					product_register_value[456 + i] = Data[ i ];
				}
			}
			Initial_UserList(); 

		 ;
		}
 		m_EDIT_ID = 0;


	return 0;
}

void CTempHumSensorForm::OnBnClickedCheckDefault()
{
	if (product_register_value[454] == 0)//说明是选中状态
	{

		ShowChecks();
		return;
	}



	int ret=write_one(g_tstat_id,454,0);
	if (ret>0)
	{
		product_register_value[454] = 0;

	}
	else
	{
		AfxMessageBox(_T("Setting Fail!"));
	}
	ShowChecks();
}


void CTempHumSensorForm::OnBnClickedCheckUser()
{
	if (product_register_value[454] == 1)//说明是选中状态
	{

		ShowChecks();
		return;
	}



	int ret=write_one(g_tstat_id,454,1);
	if (ret>0)
	{
		product_register_value[454] = 1;

	}
	else
	{
		AfxMessageBox(_T("Setting Fail!"));
	}
	ShowChecks();
}
