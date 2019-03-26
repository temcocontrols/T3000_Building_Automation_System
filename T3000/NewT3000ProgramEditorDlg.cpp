// NewT3000ProgramEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "NewT3000ProgramEditorDlg.h"
 
 
#include "Resource.h"


#include "BacnetProgram.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "global_function.h"
#include "global_define.h"
#include "BacnetProgramSetting.h"
#include "BacnetProgramDebug.h"

/*
1.可以支持从MINIPANEL中读取program code 
2.添加了C#的program Editor 控件用来显示 program code
3.添加了语法解析部分，用来显示不同的颜色标准
*/

extern CString program_string;
extern CString AnalysisString;
extern char my_display[10240];
extern HWND mParent_Hwnd;
extern void copy_data_to_ptrpanel(int Data_type);
extern char editbuf[25000];
extern char *desassembler_program();
extern bool show_upper;
IMPLEMENT_DYNAMIC(CNewT3000ProgramEditorDlg, CDialogEx)

CNewT3000ProgramEditorDlg::CNewT3000ProgramEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_NEW_T3000_PRG_EDIT, pParent)
{

}

CNewT3000ProgramEditorDlg::~CNewT3000ProgramEditorDlg()
{
}

void CNewT3000ProgramEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_FORMS1, m_programeditor);
	
}


BEGIN_MESSAGE_MAP(CNewT3000ProgramEditorDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewT3000ProgramEditorDlg message handlers


BOOL CNewT3000ProgramEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//CString ShowProgramText;
	//ShowProgramText.Format(_T("Panel :  %u      Program  :  %u "), Station_NUM, program_list_line + 1);
	////ShowProgramText = ShowProgramText + temp_label;
	//SetWindowText(ShowProgramText);
	CString ShowProgramText;
	CString temp_label;
	MultiByteToWideChar(CP_ACP, 0, (char *)m_Program_data.at(program_list_line).label, (int)strlen((char *)m_Program_data.at(program_list_line).label) + 1,
		temp_label.GetBuffer(MAX_PATH), MAX_PATH);
	temp_label.ReleaseBuffer();
	if (temp_label.IsEmpty())
	{
		temp_label.Format(_T("PRG%d"), program_list_line + 1);
	}

	ShowProgramText.Format(_T("Panel :  %u      Program  :  %u      Name  :  "), Station_NUM, program_list_line + 1);
	ShowProgramText = ShowProgramText + temp_label;
	SetWindowText(ShowProgramText);
//	((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Unreg_Hotkey();
	 

	
	//my_panel = bac_gloab_panel; //Set the panel number
	AnalysisString.Empty();

	 

#if 0
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);
 
	CString strSql;
	strSql.Format(_T("SELECT COUNT(*) FROM sqlite_master where type = 'table' and name = 'PRG_CODE'"));
	q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
	if(!q.eof()) {
		strSql.Format(_T("Select * From PRG_CODE where Station_NUM=%d AND program_list_line=%d"), Station_NUM, program_list_line);
		q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
		if (!q.eof()) {
			strcode = q.getValuebyName(L"CODE");
		}
	}
	
	SqliteDBT3000.closedb();
#endif
	mParent_Hwnd = g_hwnd_now;
	//prg_color_change = false;

	m_program_edit_hwnd = this->m_hWnd;
	g_hwnd_now = m_program_edit_hwnd;

	copy_data_to_ptrpanel(TYPE_ALL);
	memset(my_display, 0, sizeof(my_display));
	 
	char * temp_point;
	temp_point = desassembler_program();
	if (temp_point == NULL)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Decode Error!"));
		return 1;
	}
	SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Decode success!"));
	show_upper = (DWORD)GetPrivateProfileInt(_T("Program_IDE_Color"), _T("Upper Case"), 1, g_cstring_ini_path);
	CString temp;
	int  len = 0;
	len = strlen(my_display); //str.length();
	int  unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, my_display, -1, NULL, 0);
	::MultiByteToWideChar(CP_ACP, 0, my_display, -1, temp.GetBuffer(unicodeLen), unicodeLen);
	temp.ReleaseBuffer();
	CString temp1 = temp;
	if (show_upper)
	{
		temp1.MakeUpper();
	}
	else
		temp1.MakeLower();

	temp1.Replace(_T("\r\n"), _T(" \r\n"));
	temp1.Replace(_T("\("), _T(" \( "));
	temp1.Replace(_T("\)"), _T(" \) "));
	temp1.Replace(_T("  "), _T(" "));
	temp1.Replace(_T("  "), _T(" "));
	temp1.Replace(_T("  "), _T(" "));
	//temp1 = "110 BTUSUP = 19.253.REG108 / 10" ;
		 
	SetAllPoints_BACnetDevice();
	// TODO:  Add extra initialization here
    //m_programeditor.put_Code(temp1);
	m_programeditor.SendCode_T3000(temp1);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewT3000ProgramEditorDlg::SetAllPoints_BACnetDevice()
{

	for (int i = 0; i < BAC_INPUT_ITEM_COUNT; i++)
	{
		CString  AutoManual, Value, Label;
		CString FullLabel;
		CString Units;
		//LPCTSTR Label, LPCTSTR AutoManual, LPCTSTR Value, LPCTSTR FullLabel, LPCTSTR Units,
		if (m_Input_data.at(i).auto_manual == 0)
		{
			AutoManual= _T("Auto");
		}
		else
		{
			AutoManual = _T("Manual");
		}
		if (m_Input_data.at(i).digital_analog == BAC_UNITS_ANALOG)
		{
			if ((m_Input_data.at(i).range >= 20) && (m_Input_data.at(i).range <= 24))
				Units=Analog_Customer_Units[m_Input_data.at(i).range - 20];
			else if (m_Input_data.at(i).range < (sizeof(Input_List_Analog_Units) / sizeof(Input_List_Analog_Units[0])))
				Units =  Input_List_Analog_Units[m_Input_data.at(i).range];
			else if (m_Input_data.at(i).range < (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
				 Units = _T("");
			else
				Units  =_T("Unused");

			if (m_Input_data.at(i).range == 0)
				Units = _T("Unused");
			else if (m_Input_data.at(i).range < (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
			{
				if (bacnet_device_type == PM_T3PT12)
				{
					if (m_Input_data.at(i).range == 1)
						Units = _T("PT-100 Deg.C");
					else if (m_Input_data.at(i).range == 2)
						Units = _T("PT-100 Deg.F");
					else if (m_Input_data.at(i).range == 5)
						Units = _T("PT-1000 Deg.C");
					else if (m_Input_data.at(i).range == 6)
						Units =_T("PT-1000 Deg.F");
					else
					{
						if (m_Input_data.at(i).range <= (sizeof(Input_Analog_Units_Array) / sizeof(Input_Analog_Units_Array[0])))
							Units = Input_Analog_Units_Array[m_Input_data.at(i).range];
					}

				}
				else
					Units = Input_Analog_Units_Array[m_Input_data.at(i).range];
			}
			else
				Units = _T("Out of range");





		 
			float temp_float_value;
			temp_float_value = ((float)m_Input_data.at(i).value) / 1000;
			Value.Format(_T("%.2f"), temp_float_value);
		}
		else if (m_Input_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{
			if (m_Input_data.at(i).range == 0)
			{
				CString cstemp_value1;
				float temp_float_value1;
				temp_float_value1 = ((float)m_Input_data.at(i).value) / 1000;
				Value.Format(_T("%.2f"), temp_float_value1);
				Units= Digital_Units_Array[0];
			}
			else if (m_Input_data.at(i).range <= 22)
				Units = Digital_Units_Array[m_Input_data.at(i).range];
			else if ((m_Input_data.at(i).range >= 23) && (m_Input_data.at(i).range <= 30))
			{
				if (receive_customer_unit)
					Units = Custom_Digital_Range[m_Input_data.at(i).range - 23];
				else
					Units = Digital_Units_Array[0];
			}
			else
				Units = Digital_Units_Array[0];
			 

			if ((m_Input_data.at(i).range > 30) || (m_Input_data.at(i).range == 0))
			{

			}
			else
			{

				CStringArray temparray;

				if ((m_Input_data.at(i).range < 23) && (m_Input_data.at(i).range != 0))
					Units = Digital_Units_Array[m_Input_data.at(i).range];
				else if ((m_Input_data.at(i).range >= 23) && (m_Input_data.at(i).range <= 30))
				{
					if (receive_customer_unit)
						Units = Custom_Digital_Range[m_Input_data.at(i).range - 23];
				}

				SplitCStringA(temparray, Units, _T("/"));
				if ((temparray.GetSize() == 2))
				{
					if (m_Input_data.at(i).control == 0)
						Value= temparray.GetAt(0);
					else
						Value=temparray.GetAt(1);
				}
			}

		}

		MultiByteToWideChar(CP_ACP, 0, (char *)m_Input_data.at(i).description, (int)strlen((char *)m_Input_data.at(i).description) + 1,
			FullLabel.GetBuffer(MAX_PATH), MAX_PATH);
		FullLabel.ReleaseBuffer();

		 
		MultiByteToWideChar(CP_ACP, 0, (char *)m_Input_data.at(i).label, (int)strlen((char *)m_Input_data.at(i).label) + 1,
			Label.GetBuffer(MAX_PATH), MAX_PATH);
		Label.ReleaseBuffer();

		if (Label.GetLength()>1)
		{
			m_programeditor.AddControlPoint(1, Label, AutoManual, Value, FullLabel, Units, i);
		}

	}

	for (int i = 0; i < BAC_OUTPUT_ITEM_COUNT; i++)
	{
		CString  AutoManual, Value, Label;
		CString FullLabel;
		CString Units;
		MultiByteToWideChar(CP_ACP, 0, (char *)m_Output_data.at(i).description, (int)strlen((char *)m_Output_data.at(i).description) + 1,
			FullLabel.GetBuffer(MAX_PATH), MAX_PATH);
		FullLabel.ReleaseBuffer();
		if (m_Output_data.at(i).auto_manual == 0)	//In output table if it is auto ,the value can't be edit by user
		{
			AutoManual= _T("Auto");
		}
		else
		{
			AutoManual= _T("Manual");
		}
		if (m_Output_data.at(i).digital_analog == BAC_UNITS_ANALOG)
		{
			 
			if (m_Output_data.at(i).range == 0)
				Units= _T("Unused");
			else if (m_Output_data.at(i).range < (sizeof(OutPut_List_Analog_Range) / sizeof(OutPut_List_Analog_Range[0])))
				Units = OutPut_List_Analog_Range[m_Output_data.at(i).range];
			else
				Units = _T("Out of range");

			if (m_Output_data.at(i).range < (sizeof(OutPut_List_Analog_Units) / sizeof(OutPut_List_Analog_Units[0])))
				Units = OutPut_List_Analog_Units[m_Output_data.at(i).range];
			else
				Units = _T("Unused");
			Value.Format(_T("%.2f"), ((float)m_Output_data.at(i).value) / 1000);
			 

		}
		else if (m_Output_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{
			if (m_Output_data.at(i).range == 0)
			{
			 
				Value.Format(_T("%.2f"), ((float)m_Output_data.at(i).value) / 1000);
				Units = Digital_Units_Array[0];
			}
			else if (m_Output_data.at(i).range <= 22)
				Units = Digital_Units_Array[m_Output_data.at(i).range];
			else if ((m_Output_data.at(i).range >= 23) && (m_Output_data.at(i).range <= 30))
			{
				if (receive_customer_unit)
					Units = Custom_Digital_Range[m_Output_data.at(i).range - 23];
				else
					Units = Digital_Units_Array[0];
			}
			else
				Units = Digital_Units_Array[0];
 
			if ((m_Output_data.at(i).range > 30) || (m_Output_data.at(i).range == 0))
			{
				//m_output_list.SetItemText(i,OUTPUT_UNITE,Digital_Units_Array[0]);
			}
			else
			{

				CStringArray temparray;

				if ((m_Output_data.at(i).range < 23) && (m_Output_data.at(i).range != 0))
					Units = Digital_Units_Array[m_Output_data.at(i).range];
				else if ((m_Output_data.at(i).range >= 23) && (m_Output_data.at(i).range <= 30))
				{
					if (receive_customer_unit)
						Units = Custom_Digital_Range[m_Output_data.at(i).range - 23];
				}
				else
				{
					 
				}

				SplitCStringA(temparray, Units, _T("/"));
				if ((temparray.GetSize() == 2))
				{
					if (m_Output_data.at(i).control == 0)
						Value= temparray.GetAt(0);
					else
						Value=temparray.GetAt(1);
				}
			}
		}
		 
		MultiByteToWideChar(CP_ACP, 0, (char *)m_Output_data.at(i).label, (int)strlen((char *)m_Output_data.at(i).label) + 1,
			Label.GetBuffer(MAX_PATH), MAX_PATH);
		Label.ReleaseBuffer();
		if (Label.GetLength()>1)
		{
			m_programeditor.AddControlPoint(2, Label, AutoManual, Value, FullLabel, Units, i);
		}
		
	}

	for (int i = 0; i < BAC_VARIABLE_ITEM_COUNT; i++)
	{
		CString  AutoManual, Value, Label;
		CString FullLabel;
		CString Units;
		MultiByteToWideChar(CP_ACP, 0, (char *)m_Variable_data.at(i).description, (int)strlen((char *)m_Variable_data.at(i).description) + 1,
			FullLabel.GetBuffer(MAX_PATH), MAX_PATH);
		FullLabel.ReleaseBuffer();
		if (m_Variable_data.at(i).auto_manual == 0)
		{
			AutoManual= _T("Auto");
		}
		else
		{
			AutoManual=_T("Manual");
		}
		if (m_Variable_data.at(i).digital_analog == BAC_UNITS_DIGITAL)
		{

			if ((m_Variable_data.at(i).range == 0) || (m_Variable_data.at(i).range > 30))
			{
			 
				float temp_float_value1;
				temp_float_value1 = ((float)m_Variable_data.at(i).value) / 1000;
				Value.Format(_T("%.3f"), temp_float_value1);
			}
			else
			{
				 
				CStringArray temparray;

				if ((m_Variable_data.at(i).range < 23) && (m_Variable_data.at(i).range != 0))
					Units = Digital_Units_Array[m_Variable_data.at(i).range];
				else if ((m_Variable_data.at(i).range >= 23) && (m_Variable_data.at(i).range <= 30))
				{
					if (receive_customer_unit)
						Units = Custom_Digital_Range[m_Variable_data.at(i).range - 23];
				}
				else
				{
					Units = Digital_Units_Array[0];
					 
				}

				SplitCStringA(temparray, Units, _T("/"));
				if ((temparray.GetSize() == 2))
				{
					if (m_Variable_data.at(i).control == 0)
						Value= temparray.GetAt(0);
					else
						Value=temparray.GetAt(1);
					 
				}

			}
		}
		else
		{
			if (m_Variable_data.at(i).range == 20)	//如果是时间;
			{
				Units=Variable_Analog_Units_Array[m_Variable_data.at(i).range];
				char temp_char[50];
				int time_seconds = m_Variable_data.at(i).value / 1000;
				intervaltotextfull(temp_char, time_seconds, 0, 0);
				 
				MultiByteToWideChar(CP_ACP, 0, temp_char, strlen(temp_char) + 1,
					Value.GetBuffer(MAX_PATH), MAX_PATH);
				Value.ReleaseBuffer();
			}
			else if (m_Variable_data.at(i).range < sizeof(Variable_Analog_Units_Array) / sizeof(Variable_Analog_Units_Array[0]))
			{
				Units= Variable_Analog_Units_Array[m_Variable_data.at(i).range];
				float temp_float_value;
				temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
				Value.Format(_T("%.3f"), temp_float_value);
			}
			else if ((m_Variable_data.at(i).range >= 34) && (m_Variable_data.at(i).range <= 38))
			{
				Units= Analog_Variable_Units[m_Variable_data.at(i).range - 34];

				 
				float temp_float_value;
				temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
				Value.Format(_T("%.3f"), temp_float_value); 
			}
			else
			{
				Units=Variable_Analog_Units_Array[0];
				float temp_float_value;
				temp_float_value = ((float)m_Variable_data.at(i).value) / 1000;
				Value.Format(_T("%.3f"), temp_float_value);	 
			}
		}
		MultiByteToWideChar(CP_ACP, 0, (char *)m_Variable_data.at(i).label, (int)strlen((char *)m_Variable_data.at(i).label) + 1,
			Label.GetBuffer(MAX_PATH), MAX_PATH);
		Label.ReleaseBuffer();
		if (Label.GetLength() > 1)
		{
			m_programeditor.AddControlPoint(0, Label, AutoManual, Value, FullLabel, Units, i);
		}
	}

	for (int i = 0; i < BAC_PROGRAM_ITEM_COUNT; i++)
	{
		CString  AutoManual, Value, Label;
		CString FullLabel;
		CString Units;
		MultiByteToWideChar(CP_ACP, 0, (char *)m_Program_data.at(i).description, (int)strlen((char *)m_Program_data.at(i).description) + 1,
			FullLabel.GetBuffer(MAX_PATH), MAX_PATH);
		FullLabel.ReleaseBuffer();
		if (m_Program_data.at(i).auto_manual == 0)
			AutoManual= _T("Auto");
		else
			AutoManual= _T("Manual");

		 
		MultiByteToWideChar(CP_ACP, 0, (char *)m_Program_data.at(i).label, (int)strlen((char *)m_Program_data.at(i).label) + 1,
			Label.GetBuffer(MAX_PATH), MAX_PATH);
		Label.ReleaseBuffer();
		if (Label.GetLength() > 1)
		{
			m_programeditor.AddControlPoint(3, Label, AutoManual, Value, FullLabel, Units, i);
		}
	}

	for (int i = 0; i < BAC_PID_COUNT; i++)
	{
		 
	}

	for (int i = 0; i < BAC_SCREEN_COUNT; i++)
	{
		 
	}

	for (int i = 0; i < BAC_GRPHIC_LABEL_COUNT; i++)
	{
		 
	}

	for (int i = 0; i < BAC_USER_LOGIN_COUNT; i++)
	{
		 
	}

	for (int i = 0; i < BAC_CUSTOMER_UNITS_COUNT; i++)
	{
		 
	}

	for (int i = 0; i < BAC_ALALOG_CUSTMER_RANGE_TABLE_COUNT; i++)
	{
		 
	}

	 


	for (int i = 0; i < BAC_SCHEDULE_COUNT; i++)
	{
		CString  AutoManual, Value, Label;
		CString FullLabel;
		CString Units;

		MultiByteToWideChar(CP_ACP, 0, (char *)m_Weekly_data.at(i).description, (int)strlen((char *)m_Weekly_data.at(i).description) + 1,
			FullLabel.GetBuffer(MAX_PATH), MAX_PATH);
		FullLabel.ReleaseBuffer();
		 
		if (m_Weekly_data.at(i).auto_manual == 0)
		{
			AutoManual= _T("Auto");
			 
		}
		else
		{
			AutoManual= _T("Manual");
			 
		}
		if (m_Weekly_data.at(i).value == 0)
			Value=("OFF");
		else
			Value= _T("ON");

	 

		 
		MultiByteToWideChar(CP_ACP, 0, (char *)m_Weekly_data.at(i).label, (int)strlen((char *)m_Weekly_data.at(i).label) + 1,
			Label.GetBuffer(MAX_PATH), MAX_PATH);
		Label.ReleaseBuffer();
		if (Label.GetLength() > 1)
		{
			m_programeditor.AddControlPoint(4, Label, AutoManual, Value, FullLabel, Units, i);
		}
	}

	for (int i = 0; i < BAC_HOLIDAY_COUNT; i++)
	{
		CString  AutoManual, Value, Label;
		CString FullLabel;
		CString Units;
		MultiByteToWideChar(CP_ACP, 0, (char *)m_Annual_data.at(i).description, (int)strlen((char *)m_Annual_data.at(i).description) + 1,
			FullLabel.GetBuffer(MAX_PATH), MAX_PATH);
		FullLabel.ReleaseBuffer();
		 
		if (m_Annual_data.at(i).auto_manual == 0)
		{
			AutoManual= _T("Auto");
			 
		}
		else
		{
			AutoManual= _T("Manual");
			 
		}


		if (m_Annual_data.at(i).value == 0)
			Value= _T("OFF");
		else
			Value= _T("ON");



	 
		MultiByteToWideChar(CP_ACP, 0, (char *)m_Annual_data.at(i).label, (int)strlen((char *)m_Annual_data.at(i).label) + 1,
			Label.GetBuffer(MAX_PATH), MAX_PATH);
		Label.ReleaseBuffer();
		if (Label.GetLength() > 1)
		{
			m_programeditor.AddControlPoint(5, Label, AutoManual, Value, FullLabel, Units, i);
		}
	}

	for (int i = 0; i < BAC_MONITOR_COUNT; i++)
	{
		 
	}

	for (int i = 0; i < BAC_WEEKLYCODE_ROUTINES_COUNT; i++)
	{
		 
	}

	for (int i = 0; i < BAC_HOLIDAY_COUNT; i++)
	{
		 
	}

	for (int i = 0; i < BAC_PROGRAMCODE_ITEM_COUNT; i++)
	{
		 
	}

	 
}


