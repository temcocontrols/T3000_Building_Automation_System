// NewT3000ProgramEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ControlBasicEditorView.h"
 
 
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
extern char mycode[2000];
extern char *desassembler_program();
extern int Encode_Program();
extern bool show_upper;
extern int error;
extern int renumvar;
extern int my_lengthcode;
IMPLEMENT_DYNAMIC(ControlBasicEditorView, CDialogEx)

ControlBasicEditorView::ControlBasicEditorView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONTROLBASIC_EDITOR_VIEW, pParent)
{

}

ControlBasicEditorView::~ControlBasicEditorView()
{
}

void ControlBasicEditorView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_FORMS1, m_programeditor);
	
}


BEGIN_MESSAGE_MAP(ControlBasicEditorView, CDialogEx)
	ON_COMMAND(ID_SEND34071, &ControlBasicEditorView::OnSend)
	ON_COMMAND(ID_CLEAR34070, &ControlBasicEditorView::OnClear)
	ON_COMMAND(ID_LOADFILE34072, &ControlBasicEditorView::OnLoadfile)
	ON_COMMAND(ID_SAVEFILE34073, &ControlBasicEditorView::OnSavefile)
	ON_COMMAND(ID_REFRESH34074, &ControlBasicEditorView::OnRefresh)
	ON_COMMAND(ID_SETTINGS34075, &ControlBasicEditorView::OnProgramIdeSettings)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CNewT3000ProgramEditorDlg message handlers


BOOL ControlBasicEditorView::OnInitDialog()
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

void ControlBasicEditorView::SetAllPoints_BACnetDevice()
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
void ControlBasicEditorView::OnSend()
{
	//reset the program buffer
	memset(program_code[program_list_line], 0, 2000);

	renumvar = 0;
	error = -1; //Default no error;
	CString tempcs;
	/*((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetWindowTextW(tempcs);*/
	tempcs = m_programeditor.get_Code();
	tempcs.MakeUpper();
	//	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP, 0, tempcs, -1, NULL, 0, NULL, NULL);
	memset((void*)editbuf, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, tempcs, -1, editbuf, iTextLen, NULL, NULL);

	//2017/ 12 / 04   dufan 不允许使用 在THEN 中嵌套使用IF ，否则 解码时会出错。暂时找不出更好的解决办法。
	if (tempcs.Find(_T("THEN IF")) != -1)
	{
		MessageBox(_T("Don't allowed use nested 'IF'."));
		return;
	}

	Encode_Program();
	if (error == -1)
	{

		TRACE(_T("Encode_Program length is %d ,copy length is %d\r\n"), program_code_length[program_list_line], my_lengthcode);

		if (my_lengthcode > 1920)
		{
			MessageBox(_T("Encode Program Code Length is too large"));
			return;
		}


		if ((Device_Basic_Setting.reg.panel_type == PM_MINIPANEL) && (my_lengthcode > 9))// 
		{

			int total_program_size = 0;
			for (int i = 0;i < (int)m_Program_data.size();i++)
			{
				if (i == program_list_line)
				{
					total_program_size = total_program_size + my_lengthcode;
					continue;
				}
				if (m_Program_data.at(i).bytes != 0)
					total_program_size = total_program_size + (m_Program_data.at(i).bytes / 400 + 1) * 400;
			}

			if (total_program_size >= 10000)
			{
				CString temp_message;
				temp_message.Format(_T("Send failed!\r\nThere is not enough storage space!\r\nTotal size 10000 bytes.\r\Need %d "), total_program_size);
				MessageBox(temp_message);
				return;
			}
		}



		memset(program_code[program_list_line], 0, 2000);
		memcpy_s(program_code[program_list_line], my_lengthcode, mycode, my_lengthcode);
		program_code_length[program_list_line] = program_code[program_list_line][1] * 256 + (unsigned char)program_code[program_list_line][0];
		bac_program_size = program_code_length[program_list_line];// my_lengthcode;
		bac_free_memory = 2000 - bac_program_size;


		int npart = (my_lengthcode / 401) + 1;

		bool b_program_status = true;

		if (g_protocol == PROTOCOL_BIP_TO_MSTP)
		{
			for (int j = 0;j < npart;j++)
			{
				int n_ret = 0;
				n_ret = WriteProgramData_Blocking(g_bac_instance, WRITEPROGRAMCODE_T3000, program_list_line, program_list_line, j);
				if (n_ret < 0)
				{
					MessageBox(_T("Write Program Code Timeout!"));
					return;
				}
			}
			b_program_status = true;
		}
		else
		{
			for (int j = 0;j < npart;j++)
			{
				int send_status = true;
				int resend_count = 0;
				int temp_invoke_id = -1;
				do
				{
					resend_count++;
					if (resend_count > 5)
					{
						send_status = false;
						b_program_status = false;
						MessageBox(_T("Write Program Code Timeout!"));
						return;
					}
					temp_invoke_id = WriteProgramData(g_bac_instance, WRITEPROGRAMCODE_T3000, program_list_line, program_list_line, j);

					Sleep(SEND_COMMAND_DELAY_TIME);
				} while (temp_invoke_id < 0);

				if (send_status)
				{
					for (int i = 0;i < 3000;i++)
					{
						Sleep(1);
						if (tsm_invoke_id_free(temp_invoke_id))
						{
							//MessageBox(_T("Operation success!"),_T("Information"),MB_OK);
							//return;
							goto	program_part_success;
						}
					}
					b_program_status = false;
					MessageBox(_T("Write Program Code Timeout!"));
					return;

				program_part_success:
					continue;
				}
			}
		}

		if (b_program_status)
		{
			CString temp_string;
			temp_string.Format(_T("Resource Compile succeeded.\r\nTotal size 2000 bytes.\r\nAlready used %d"), bac_program_size);



			CTime temp_time = CTime::GetCurrentTime();
			CString str_g_serialNum;
			CString str_txt_file = g_achive_folder_temp_txt;
			str_g_serialNum.Format(_T("%u_prg%d"), g_serialNum, program_list_line + 1);
			CString temp_time_format = temp_time.Format(_T("%y_%m_%d %H_%M_%S"));
			str_txt_file = str_txt_file + _T("\\") + str_g_serialNum + _T("    ") + temp_time_format + _T(".txt");
			CString Write_Buffer;
			CString FilePath;
			FilePath = str_txt_file;
			CFileFind temp_find;


			GetDlgItemText(IDC_RICHEDIT2_PROGRAM, Write_Buffer);
			CString temp_write_buf;
			temp_write_buf = Write_Buffer;
			temp_write_buf.Trim();
			if ((temp_write_buf.GetLength() != 0) && (program_string.CompareNoCase(Write_Buffer) != 0))
			{
				char*     readytowrite;
				int    iTextLen;
				iTextLen = WideCharToMultiByte(CP_ACP, 0, Write_Buffer, -1, NULL, 0, NULL, NULL);
				readytowrite = new char[iTextLen + 1];
				memset((void*)readytowrite, 0, sizeof(char) * (iTextLen + 1));
				::WideCharToMultiByte(CP_ACP, 0, Write_Buffer, -1, readytowrite, iTextLen, NULL, NULL);



				CFile file(FilePath, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
				file.SeekToEnd();
				int write_length = strlen(readytowrite);
				file.Write(readytowrite, write_length + 1);
				file.Flush();
				file.Close();
				delete[] readytowrite;
			}




			MessageBox(temp_string);
		}

		//m_information_window.ResetContent();
		//m_information_window.InsertString(0, _T("Resource Compile succeeded."));
		m_Program_data.at(program_list_line).bytes = bac_program_size;
		//Initial_static();
		//CString temp1;
		//((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetWindowTextW(temp1);
		//m_edit_changed = false;
		//program_string = temp1;
	}
	else
	{
		//m_information_window.ResetContent();
		//CString cstring_error;

		//int len = strlen(mesbuf);
// 		int  unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, mesbuf, -1, NULL, 0);
// 		::MultiByteToWideChar(CP_ACP, 0, mesbuf, -1, cstring_error.GetBuffer(2000), unicodeLen);
// 		cstring_error.ReleaseBuffer();


		//		MessageBox(cstring_error);
// 		CStringArray  Error_info;
// 		SplitCStringA(Error_info, cstring_error, _T("\r\n"));//Split the CString with "\r\n" and then add to the list.(Fance)
// 		Sleep(1);
		//m_information_window.ResetContent();
		//for (int i = 0;i < (int)Error_info.GetSize();i++)
		//{
			//m_information_window.InsertString(i, Error_info.GetAt(i));
		//}

		//MessageBox(_T("Errors,program NOT Sent!"));
	}
	//UpdateDataProgramText();
}
void ControlBasicEditorView::OnClear()
{
    
	  m_programeditor.Clear_BASICControl();
}
void ControlBasicEditorView::OnLoadfile()
{



	CString FilePath;
	CString ReadBuffer;

	CFileDialog dlg(true, _T("*.txt"), _T(" "), OFN_HIDEREADONLY, _T("txt files (*.txt)|*.txt|All Files (*.*)|*.*||"), NULL, 0);
	if (IDOK == dlg.DoModal())
	{
		FilePath = dlg.GetPathName();

		//Write_Position = pParent->myapp_path + cs_file_time;
		char *pBuf;
		DWORD dwFileLen;


		CFile file(FilePath, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);

		dwFileLen = (DWORD)file.GetLength();
		pBuf = new char[dwFileLen + 1];
		pBuf[dwFileLen] = 0;

		file.SeekToBegin();
		file.Read(pBuf, dwFileLen);
		file.Close();

		CString ReadBuffer;

		int  len = 0;
		len = strlen(pBuf);
		int  unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, pBuf, -1, NULL, 0);
		::MultiByteToWideChar(CP_ACP, 0, pBuf, -1, ReadBuffer.GetBuffer(unicodeLen), unicodeLen);
		ReadBuffer.ReleaseBuffer();

		delete[] pBuf;
		/*((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetWindowTextW(ReadBuffer);
		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(unicodeLen, unicodeLen);*/
		m_programeditor.SendCode_T3000(ReadBuffer);

	}
}

void ControlBasicEditorView::OnSavefile()
{

	CFileDialog dlg(false, _T("*.txt"), _T(" "), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("txt files (*.txt)|*.txt|All Files (*.*)|*.*||"), NULL, 0);
	if (IDOK == dlg.DoModal())
	{
		CString Write_Buffer;
		CString FilePath;
		FilePath = dlg.GetPathName();
		CFileFind temp_find;
		if (temp_find.FindFile(FilePath))
		{
			DeleteFile(FilePath);
		}


		Write_Buffer=m_programeditor.get_Code();
		//GetDlgItemText(IDC_RICHEDIT2_PROGRAM, Write_Buffer);
		//char *readytowrite = 
		char*     readytowrite;
		int    iTextLen;
		iTextLen = WideCharToMultiByte(CP_ACP, 0, Write_Buffer, -1, NULL, 0, NULL, NULL);
		readytowrite = new char[iTextLen + 1];
		memset((void*)readytowrite, 0, sizeof(char) * (iTextLen + 1));
		::WideCharToMultiByte(CP_ACP, 0, Write_Buffer, -1, readytowrite, iTextLen, NULL, NULL);



		CFile file(FilePath, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
		file.SeekToEnd();
		int write_length = strlen(readytowrite);
		file.Write(readytowrite, write_length + 1);
		file.Flush();
		file.Close();
		delete[] readytowrite;
	}

}
void ControlBasicEditorView::OnRefresh()
{


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
		//return 0;
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

	AfxMessageBox(_T("Refresh Done"));

}

void ControlBasicEditorView::OnProgramIdeSettings()
{

	/*CBacnetProgramSetting ProgramSettingdlg;
	ProgramSettingdlg.DoModal();
	CString tempcs;
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetWindowTextW(tempcs);
	if (show_upper)
	{
		tempcs.MakeUpper();
	}
	else
	{
		tempcs.MakeLower();
	}
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetWindowTextW(tempcs);
	Syntax_analysis();*/
	m_programeditor.Setting_BASICControl();
}

void ControlBasicEditorView::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(rc);
	if (m_programeditor.m_hWnd != NULL)
	{
		::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		//m_program_list.MoveWindow(&rc);
		m_programeditor.MoveWindow(rc.left, rc.top, rc.Width(), rc.Height());

		//GetDlgItem(IDC_BUTTON_PROGRAM_EDIT)->MoveWindow(rc.left + 20 ,rc.bottom - 60 , 120,50);
	}

}