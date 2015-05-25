// BacnetEditLabel.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetEditLabel.h"
#include "globle_function.h"
#include "afxdialogex.h"

#define WM_EDIT_CHANGE_VALUE WM_USER + 1115
extern int Station_NUM;
extern int pointtotext(char *buf,Point_Net *point);
// CBacnetEditLabel dialog
extern char *ispoint(char *token,int *num_point,byte *var_type, byte *point_type, int *num_panel, int *num_net, int network, byte panel, int *netpresent);
int GetInputLabel(int index,CString &ret_label);
int GetInputFullLabel(int index,CString &ret_full_label);
//int GetInputValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M);
int GetInputValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M,int &digital_value);
int GetOutputLabel(int index,CString &ret_label);
int GetOutputFullLabel(int index,CString &ret_full_label);
//int GetOutputValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M);
int GetOutputValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M,int &digital_value);

int GetVariableLabel(int index,CString &ret_label);
int GetVariableFullLabel(int index,CString &ret_full_label);

//int GetVariableValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M);
int GetVariableValue(int index ,CString &ret_cstring,CString &ret_unit,CString &Auto_M,int &digital_value);


int GetScreenLabel(int index,CString &ret_label);
int GetScreenFullLabel(int index,CString &ret_full_label);

IMPLEMENT_DYNAMIC(CBacnetEditLabel, CDialogEx)

CBacnetEditLabel::CBacnetEditLabel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetEditLabel::IDD, pParent)
{

}

CBacnetEditLabel::~CBacnetEditLabel()
{
}

void CBacnetEditLabel::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_MFCCOLORBUTTON_COLOR, m_bkClrBtn);
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_LABEL_POINT, m_static_point);
	DDX_Control(pDX, IDC_STATIC_LABEL, m_static_label);
	DDX_Control(pDX, IDC_STATIC_FULL_LABEL, m_static_full_label);
	DDX_Control(pDX, IDC_STATIC_VALUE, m_static_value);
	DDX_Control(pDX, IDC_STATIC_DISPLAY, m_static_display);
	DDX_Control(pDX, IDC_STATIC_TEXT_COLOR, m_static_txtcol);
	DDX_Control(pDX, IDC_EDIT_LABEL_VALUE, m_edit_value);
	DDX_Control(pDX, IDC_STATIC_EDIT_LABEL_AUTO_MANUAL, m_edit_auto_manual);
	DDX_Control(pDX, IDC_STATIC_EDIT_LABEL_DISPLAY, m_edit_display);
	DDX_Control(pDX, IDC_STATIC_ICON_PATH, m_static_icon_path);
	DDX_Control(pDX, IDC_STATIC_TEXT_PLACE, m_static_text_place);
	DDX_Control(pDX, IDC_STATIC_ICON_SIZE, m_static_icon_size);
	DDX_Control(pDX, IDC_EDIT_ICON_PATH, m_edit_icon_path);
	DDX_Control(pDX, IDC_STATIC_EDIT_TEXT_PLACE, m_edit_text_place);
	DDX_Control(pDX, IDC_STATIC_EDIT_ICON_SIZE, m_edit_icon_size);
	DDX_Control(pDX, IDC_EDIT_ICON_PATH2, m_edit_icon2_path);
}


BEGIN_MESSAGE_MAP(CBacnetEditLabel, CDialogEx)
	ON_MESSAGE(WM_EDIT_CHANGE_VALUE, &CBacnetEditLabel::Change_Value)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON_COLOR, &CBacnetEditLabel::OnBnClickedMfccolorbuttonColor)
	ON_STN_CLICKED(IDC_STATIC_EDIT_LABEL_AUTO_MANUAL, &CBacnetEditLabel::OnStnClickedStaticEditLabelAutoManual)
	ON_MESSAGE(MY_RESUME_DATA, MessageCallBack)

	ON_STN_CLICKED(IDC_STATIC_EDIT_LABEL_DISPLAY, &CBacnetEditLabel::OnStnClickedStaticEditLabelDisplay)
	ON_WM_CLOSE()
	ON_STN_CLICKED(IDC_STATIC_EDIT_TEXT_PLACE, &CBacnetEditLabel::OnStnClickedStaticEditTextPlace)
	ON_STN_CLICKED(IDC_STATIC_EDIT_ICON_SIZE, &CBacnetEditLabel::OnStnClickedStaticEditIconSize)
	//ON_EN_CHANGE(IDC_EDIT_ICON_PATH, &CBacnetEditLabel::OnEnChangeEditIconPath)
	ON_STN_CLICKED(IDC_EDIT_ICON_PATH, &CBacnetEditLabel::OnStnClickedEditIconPath)
	ON_BN_CLICKED(IDC_BUTTON_LABEL_EXIT, &CBacnetEditLabel::OnBnClickedButtonLabelExit)
	ON_STN_CLICKED(IDC_EDIT_ICON_PATH2, &CBacnetEditLabel::OnStnClickedEditIconPath2)
END_MESSAGE_MAP()


// CBacnetEditLabel message handlers


BOOL CBacnetEditLabel::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_edit_label = this->m_hWnd;





	POINT lpPoint;
	GetCursorPos(&lpPoint);
	//::GetWindowRect(BacNet_hwd,&userlogin_rect);	//获取 view的窗体大小;
	CRect temprect;
	::GetWindowRect(m_screenedit_dlg_hwnd,&temprect);	//获取 view的窗体大小;
	if((lpPoint.x < temprect.left) || (lpPoint.y < temprect.top))
	{	
		PostMessage(WM_CLOSE,NULL,NULL);
		return 1;
	}
	CRect add_temprect;
	::GetWindowRect(m_edit_label,&add_temprect);	
	MoveWindow(lpPoint.x,lpPoint.y,add_temprect.Width(),add_temprect.Height(),1);

	//((CComboBox *)GetDlgItem(IDC_COMBO_EDIT_LABEL_AUTO))->AddString(_T("Auto"));
	//((CComboBox *)GetDlgItem(IDC_COMBO_EDIT_LABEL_AUTO))->AddString(_T("Manual"));
	//
	//((CComboBox *)GetDlgItem(IDC_COMBO_EDIT_LABEL_DISPLAY))->AddString(_T("Value"));
	//((CComboBox *)GetDlgItem(IDC_COMBO_EDIT_LABEL_DISPLAY))->AddString(_T("Full Description"));
	//((CComboBox *)GetDlgItem(IDC_COMBO_EDIT_LABEL_DISPLAY))->AddString(_T("Label"));

	Initial_UI();

	GetDlgItem(IDC_BUTTON_LABEL_EXIT)->SetFocus();
	return false;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetEditLabel::Initial_UI()
{
	m_edit_auto_manual.SetWindowTextW(_T("Auto"));
	m_edit_auto_manual.textColor(RGB(255,0,0));
	m_edit_auto_manual.bkColor(RGB(255,255,255));
	m_edit_auto_manual.setFont(18,10,NULL,_T("Arial"));

	m_edit_display.SetWindowTextW(_T("Value"));
	m_edit_display.textColor(RGB(255,0,0));
	m_edit_display.bkColor(RGB(255,255,255));
	m_edit_display.setFont(18,10,NULL,_T("Arial"));

	// TODO:  Add extra initialization here
	m_bkClrBtn.EnableAutomaticButton(_T("Automatic"), m_bkColor);
	m_bkClrBtn.EnableOtherButton(_T("Other"));
	m_bkClrBtn.SetColor((COLORREF)-1);
	m_bkClrBtn.SetColumnsNumber(10);



	m_static_point.SetWindowTextW(_T(""));
	m_static_point.textColor(RGB(120,50,200));
	//m_static.bkColor(RGB(0,255,255));
	m_static_point.setFont(20,15,NULL,_T("Arial"));

	m_static_label.SetWindowTextW(_T(""));
	m_static_label.textColor(RGB(120,50,200));
	//m_static.bkColor(RGB(0,255,255));
	m_static_label.setFont(20,15,NULL,_T("Arial"));

	m_static_full_label.SetWindowTextW(_T(""));
	m_static_full_label.textColor(RGB(120,50,200));
	//m_static.bkColor(RGB(0,255,255));
	m_static_full_label.setFont(20,15,NULL,_T("Arial"));


	m_static_value.SetWindowTextW(_T("Value     :"));
	m_static_value.textColor(RGB(0,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_static_value.setFont(18,10,NULL,_T("Arial"));

	m_static_display.SetWindowTextW(_T("Display    :"));
	m_static_display.textColor(RGB(0,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_static_display.setFont(18,10,NULL,_T("Arial"));

	m_static_txtcol.SetWindowTextW(_T("Text Color :"));
	m_static_txtcol.textColor(RGB(0,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_static_txtcol.setFont(18,10,NULL,_T("Arial"));

	m_edit_value.SetWindowTextW(_T(""));
	m_edit_value.textColor(RGB(255,0,0));
	m_edit_value.bkColor(RGB(255,255,255));
	m_edit_value.setFont(18,10,NULL,_T("Arial"));


	m_static_icon_path.SetWindowTextW(_T("Icon name :"));
	m_static_icon_path.textColor(RGB(0,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_static_icon_path.setFont(18,10,NULL,_T("Arial"));

	m_static_text_place.SetWindowTextW(_T("Text place :"));
	m_static_text_place.textColor(RGB(0,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_static_text_place.setFont(18,10,NULL,_T("Arial"));

	m_static_icon_size.SetWindowTextW(_T("Icon size :"));
	m_static_icon_size.textColor(RGB(0,0,255));
	//m_static.bkColor(RGB(0,255,255));
	m_static_icon_size.setFont(18,10,NULL,_T("Arial"));


	m_edit_icon_path.SetWindowTextW(_T(""));
	m_edit_icon_path.textColor(RGB(255,0,0));
	m_edit_icon_path.bkColor(RGB(255,255,255));
	m_edit_icon_path.setFont(18,10,NULL,_T("Arial"));

	m_edit_icon2_path.SetWindowTextW(_T(""));
	m_edit_icon2_path.textColor(RGB(255,0,0));
	m_edit_icon2_path.bkColor(RGB(255,255,255));
	m_edit_icon2_path.setFont(18,10,NULL,_T("Arial"));

	m_edit_text_place.SetWindowTextW(_T("Bottom"));
	m_edit_text_place.textColor(RGB(255,0,0));
	m_edit_text_place.bkColor(RGB(255,255,255));
	m_edit_text_place.setFont(18,10,NULL,_T("Arial"));

	m_edit_icon_size.SetWindowTextW(_T("Normal"));
	m_edit_icon_size.textColor(RGB(255,0,0));
	m_edit_icon_size.bkColor(RGB(255,255,255));
	m_edit_icon_size.setFont(18,10,NULL,_T("Arial"));
}

BOOL CBacnetEditLabel::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if(GetFocus()->GetDlgCtrlID() == IDC_EDIT_LABEL_VALUE)
			{
				if((label_info.nSub_Panel == Station_NUM) && (label_info.nMain_Panel == Station_NUM))
					PostMessage(WM_EDIT_CHANGE_VALUE,CHANGE_VALUE,NULL);

				return 0;
			}
		}
		else if(pMsg->wParam == VK_ESCAPE)
		{
			PostMessage(WM_CLOSE,NULL,NULL);
			return 0;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
LRESULT CBacnetEditLabel::Change_Value(WPARAM wParam,LPARAM lParam)
{
	int ncommand = (int)wParam;
	CString show_temp;
	CStringArray temparray;
	CString temp_unit;
	switch(label_info.nPoint_type)
	{
	case 1: //input
		{
			if(ncommand == CHANGE_AUTO_MANUAL)
			{
				if(m_AutoManual.CompareNoCase(_T("Manual")) == 0)
				{
					m_Input_data.at(label_info.nPoint_number).auto_manual = 0;	//Auto = 0;
					show_temp = _T("Auto");	
					m_edit_value.EnableWindow(false);
				}
				else
				{
					m_Input_data.at(label_info.nPoint_number).auto_manual = 1;	//Manual = 1;
					show_temp = _T("Manual");
					m_edit_value.EnableWindow(true);
					
				}
				m_edit_auto_manual.SetWindowTextW(show_temp);
			}
			else if(ncommand == CHANGE_VALUE)
			{
				if(m_Input_data.at(label_info.nPoint_number).digital_analog == BAC_UNITS_ANALOG)
				{
					CString temp_analog_value;
					GetDlgItemTextW(IDC_EDIT_LABEL_VALUE,temp_analog_value);
					int temp_int = (int)(_wtof(temp_analog_value) * 1000);
					m_Input_data.at(label_info.nPoint_number).value = temp_int;

					show_temp = temp_analog_value;
				}
				else if(m_Input_data.at(label_info.nPoint_number).digital_analog == BAC_UNITS_DIGITAL)
				{

					if((m_Input_data.at(label_info.nPoint_number).range < 23) &&(m_Input_data.at(label_info.nPoint_number).range !=0))
						temp_unit = Digital_Units_Array[m_Input_data.at(label_info.nPoint_number).range];
					else if((m_Input_data.at(label_info.nPoint_number).range >=23) && (m_Input_data.at(label_info.nPoint_number).range <= 30))
					{
						if(receive_customer_unit)
							temp_unit = temp_unit_no_index[m_Input_data.at(label_info.nPoint_number).range - 23];
						else
						{
							return 0;
						}
					}
					else
						return 0;
					SplitCStringA(temparray,temp_unit,_T("/"));

					if(m_Input_data.at(label_info.nPoint_number).control == 0)
					{
						m_Input_data.at(label_info.nPoint_number).control = 1;
						show_temp = temparray.GetAt(1);
					}
					else
					{
						m_Input_data.at(label_info.nPoint_number).control = 0;
						show_temp = temparray.GetAt(0);
					}
					m_edit_value.SetWindowTextW(show_temp);
				}
				else
					return 0;
			}
			else
			{
				return 0;
			}



			CString temp_task_info;
			temp_task_info.Format(_T("Write Input List Item%d .Changed to \"%s\" "),label_info.nPoint_number,show_temp);
			Post_Write_Message(g_bac_instance,WRITEINPUT_T3000,label_info.nPoint_number,label_info.nPoint_number,sizeof(Str_in_point),m_edit_label ,temp_task_info);

		}
		break;
	case 0: //Output
		{
			if(ncommand == CHANGE_AUTO_MANUAL)
			{
				if(m_AutoManual.CompareNoCase(_T("Manual")) == 0)
				{
					m_Output_data.at(label_info.nPoint_number).auto_manual = 0;	//Auto = 0;
					show_temp = _T("Auto");	
					m_edit_value.EnableWindow(false);
				}
				else
				{
					m_Output_data.at(label_info.nPoint_number).auto_manual = 1;	//Manual = 1;
					show_temp = _T("Manual");
					m_edit_value.EnableWindow(true);

				}
				m_edit_auto_manual.SetWindowTextW(show_temp);
			}
			else if(ncommand == CHANGE_VALUE)
			{
				if(m_Output_data.at(label_info.nPoint_number).digital_analog == BAC_UNITS_ANALOG)
				{
					CString temp_analog_value;
					GetDlgItemTextW(IDC_EDIT_LABEL_VALUE,temp_analog_value);
					int temp_int = (int)(_wtof(temp_analog_value) * 1000);
					m_Output_data.at(label_info.nPoint_number).value = temp_int;

					show_temp = temp_analog_value;
				}
				else if(m_Output_data.at(label_info.nPoint_number).digital_analog == BAC_UNITS_DIGITAL)
				{
					if((m_Output_data.at(label_info.nPoint_number).range < 23) &&(m_Output_data.at(label_info.nPoint_number).range !=0))
						temp_unit = Digital_Units_Array[m_Output_data.at(label_info.nPoint_number).range];
					else if((m_Output_data.at(label_info.nPoint_number).range >=23) && (m_Output_data.at(label_info.nPoint_number).range <= 30))
					{
						if(receive_customer_unit)
							temp_unit = temp_unit_no_index[m_Output_data.at(label_info.nPoint_number).range - 23];
						else
						{
							return 0;
						}
					}
					else
						return 0;

					SplitCStringA(temparray,temp_unit,_T("/"));
					if(m_Output_data.at(label_info.nPoint_number).control == 0)
					{
						m_Output_data.at(label_info.nPoint_number).control = 1;
						show_temp = temparray.GetAt(1);
					}
					else
					{
						m_Output_data.at(label_info.nPoint_number).control = 0;
						show_temp = temparray.GetAt(0);
					}
					m_edit_value.SetWindowTextW(show_temp);
				}
				else
				{
					return 0;
				}
				
			}

			CString temp_task_info;
			temp_task_info.Format(_T("Write Output List Item%d .Changed to \"%s\" "),label_info.nPoint_number,show_temp);
			Post_Write_Message(g_bac_instance,WRITEOUTPUT_T3000,label_info.nPoint_number,label_info.nPoint_number,sizeof(Str_out_point),m_edit_label ,temp_task_info);
		}
		break;
	case 2://variable
		{
			if(ncommand == CHANGE_AUTO_MANUAL)
			{
				if(m_AutoManual.CompareNoCase(_T("Manual")) == 0)
				{
					m_Variable_data.at(label_info.nPoint_number).auto_manual = 0;	//Auto = 0;
					show_temp = _T("Auto");	
					m_edit_value.EnableWindow(false);
				}
				else
				{
					m_Variable_data.at(label_info.nPoint_number).auto_manual = 1;	//Manual = 1;
					show_temp = _T("Manual");
					m_edit_value.EnableWindow(true);

				}
				m_edit_auto_manual.SetWindowTextW(show_temp);
			}
			else if(ncommand == CHANGE_VALUE)
			{
				if(m_Variable_data.at(label_info.nPoint_number).digital_analog == BAC_UNITS_ANALOG)
				{
					CString temp_analog_value;
					GetDlgItemTextW(IDC_EDIT_LABEL_VALUE,temp_analog_value);
					int temp_int = (int)(_wtof(temp_analog_value) * 1000);
					m_Variable_data.at(label_info.nPoint_number).value = temp_int;

					show_temp = temp_analog_value;
				}
				else if(m_Variable_data.at(label_info.nPoint_number).digital_analog == BAC_UNITS_DIGITAL)
				{
					if((m_Variable_data.at(label_info.nPoint_number).range < 23) &&(m_Variable_data.at(label_info.nPoint_number).range !=0))
						temp_unit = Digital_Units_Array[m_Variable_data.at(label_info.nPoint_number).range];
					else if((m_Variable_data.at(label_info.nPoint_number).range >=23) && (m_Variable_data.at(label_info.nPoint_number).range <= 30))
					{
						if(receive_customer_unit)
							temp_unit = temp_unit_no_index[m_Variable_data.at(label_info.nPoint_number).range - 23];
						else
						{
							return 0;
						}
					}
					else
						return 0;
					SplitCStringA(temparray,temp_unit,_T("/"));



					if(m_Variable_data.at(label_info.nPoint_number).control == 0)
					{
						m_Variable_data.at(label_info.nPoint_number).control = 1;
						show_temp = temparray.GetAt(1);
					}
					else
					{
						m_Variable_data.at(label_info.nPoint_number).control = 0;
						show_temp = temparray.GetAt(0);
					}

					m_edit_value.SetWindowTextW(show_temp);
				}
				else
				{
					return 0;
				}
			}

			CString temp_task_info;
			temp_task_info.Format(_T("Write Variable List Item%d .Changed to \"%s\" "),label_info.nPoint_number,show_temp);
			Post_Write_Message(g_bac_instance,WRITEVARIABLE_T3000,label_info.nPoint_number,label_info.nPoint_number,sizeof(Str_variable_point),m_edit_label ,temp_task_info);
		}
		break;
	default:
		break;
	}


	return 0;
}

void CBacnetEditLabel::OnBnClickedMfccolorbuttonColor()
{
	// TODO: Add your control notification handler code here
		COLORREF color = m_bkClrBtn.GetColor();
		if (color == -1)
		{
			color = m_bkClrBtn.GetAutomaticColor();
		}
		m_bkColor=color;
		label_info.nclrTxt = color;
}

void CBacnetEditLabel::FreshWindow(Bacnet_Label_Info &temp_info)
{
	this->ShowWindow(SW_SHOW);
	memcpy(&label_info,&temp_info,sizeof(Bacnet_Label_Info));
	memcpy(&label_info_buffer,&temp_info,sizeof(Bacnet_Label_Info));
	
	 m_original_name.Empty();
	 m_label_name.Empty();
	 m_full_label_name.Empty();
	 m_value.Empty();
	 m_AutoManual.Empty();
	 m_unit.Empty();
	 m_display_cs.Empty();
	 digital_status = -1;
	 CString temp_icon_path;
	 MultiByteToWideChar( CP_ACP, 0, (char *)temp_info.ico_name,(int)strlen((char *)temp_info.ico_name)+1, 
		 temp_icon_path.GetBuffer(MAX_PATH), MAX_PATH );
	 temp_icon_path.ReleaseBuffer();	
	 m_edit_icon_path.SetWindowTextW(temp_icon_path);

	 MultiByteToWideChar( CP_ACP, 0, (char *)temp_info.ico_name_2,(int)strlen((char *)temp_info.ico_name_2)+1, 
		 temp_icon_path.GetBuffer(MAX_PATH), MAX_PATH );
	 temp_icon_path.ReleaseBuffer();	
	 m_edit_icon2_path.SetWindowTextW(temp_icon_path);

	switch(temp_info.nDisplay_Type)
	{
	case LABEL_SHOW_VALUE:
	case LABEL_SHOW_FULL_DESCRIPTION:
	case LABEL_SHOW_LABEL:
	case LABEL_ICON_VALUE:
	case LABEL_ICON_FULL_DESCRIPTION:
	case LABEL_ICON_LABEL:
		{
			m_edit_display.SetWindowTextW(Label_Display_Array[temp_info.nDisplay_Type]);
			m_display_cs = Label_Display_Array[temp_info.nDisplay_Type];
		}
		break;
	default:
		{
			MessageBox(_T("Point display type error!"));
			PostMessage(WM_CLOSE,NULL,NULL);
			return ;
		}
		break;
	}

	if((temp_info.nDisplay_Type == LABEL_SHOW_VALUE) ||
		(temp_info.nDisplay_Type == LABEL_SHOW_FULL_DESCRIPTION) ||
		(temp_info.nDisplay_Type == LABEL_SHOW_LABEL))
	{
		ChangeWindowPos(0);
	}

	switch(temp_info.ntext_place)
	{
	case LABEL_TEXT_BOTTOM:
	case LABEL_TEXT_LEFT:
	case LABEL_TEXT_TOP:
	case LABEL_TEXT_RIGHT:
		{
			m_edit_text_place.SetWindowTextW(Label_Text_Place[temp_info.ntext_place]);
			//m_display_cs = Label_Display_Array[temp_info.nDisplay_Type];
		}
		break;
	default:
		{
			MessageBox(_T("Point text position error!"));
			PostMessage(WM_CLOSE,NULL,NULL);
			return ;
		}
		break;
	}

	switch(temp_info.n_iconsize)
	{
	case LABEL_ICON_SMALL:
	case LABEL_ICON_NORMAL:
	case LABEL_ICON_LARGE:
		{
			m_edit_icon_size.SetWindowTextW(Label_ICON_SIZE[temp_info.n_iconsize]);
			//m_display_cs = Label_Display_Array[temp_info.nDisplay_Type];
		}
		break;
	default:
		{
			MessageBox(_T("Point icon size error!"));
			PostMessage(WM_CLOSE,NULL,NULL);
			return ;
		}
		break;
	}
	

	POINT lpPoint;
	GetCursorPos(&lpPoint);
	//::GetWindowRect(BacNet_hwd,&userlogin_rect);	//获取 view的窗体大小;
	CRect temprect;
	::GetWindowRect(m_screenedit_dlg_hwnd,&temprect);	//获取 view的窗体大小;
	if((lpPoint.x < temprect.left) || (lpPoint.y < temprect.top))
	{	
		PostMessage(WM_CLOSE,NULL,NULL);
		return ;
	}
	CRect add_temprect;
	::GetWindowRect(m_edit_label,&add_temprect);	

	MoveWindow(lpPoint.x,lpPoint.y,add_temprect.Width(),add_temprect.Height(),1);
	GetDlgItem(IDC_EDIT_LABEL_VALUE)->SetFocus();

	if((temp_info.nMain_Panel != Station_NUM)/* || (temp_info.nSub_Panel != Station_NUM)*/)
	{
		PostMessage(WM_CLOSE,NULL,NULL);
		return ;
	}

	if(temp_info.nSub_Panel != Station_NUM)
	{
		Point_Net temp_point;
		temp_point.panel = Station_NUM;
		temp_point.sub_panel = temp_info.nSub_Panel;
		temp_point.point_type = temp_info.nPoint_type;
		temp_point.number = temp_info.nPoint_number;
		temp_point.network = 1;
		char original_point[30];
		memset(original_point,0,30);
		pointtotext(original_point,&temp_point);
		CString temp_des;
		MultiByteToWideChar( CP_ACP, 0, original_point, (int)strlen(original_point)+1, 
			temp_des.GetBuffer(MAX_PATH), MAX_PATH );
		temp_des.ReleaseBuffer();
		m_original_name = temp_des;
		m_edit_value.EnableWindow(false);
		m_edit_auto_manual.SetWindowTextW(_T("Auto"));
		m_static_point.SetWindowTextW(m_original_name);
		
		bool found_index = false;
		for (int i=0;i<(int)m_remote_point_data.size();i++)
		{
			if(m_remote_point_data.at(i).point.sub_panel == 0 )
			{
				break;
			}
			if((m_remote_point_data.at(i).point.sub_panel == temp_point.sub_panel) &&
				(m_remote_point_data.at(i).point.point_type == temp_point.point_type) &&
				(m_remote_point_data.at(i).point.number == temp_point.number))
			{
				m_value.Format(_T("%d"),(m_remote_point_data.at(i).point_value));
				found_index = true;
				break;
			}
		}

		if(found_index == false)
		{
			m_value.Format(_T("N/A"));
		}
		m_edit_value.SetWindowTextW(m_value);

	}
	else
	{
		switch(temp_info.nPoint_type)
		{
		case 1: //input
			{
				if(temp_info.nPoint_number >= BAC_INPUT_ITEM_COUNT)
				{
					PostMessage(WM_CLOSE,NULL,NULL);
					return ;
				}
				else
				{
					m_original_name.Format(_T("%d-IN%d"),temp_info.nMain_Panel,temp_info.nPoint_number + 1);
					GetInputLabel(temp_info.nPoint_number,m_label_name);
					GetInputFullLabel(temp_info.nPoint_number,m_full_label_name);

					m_static_point.SetWindowTextW(m_original_name);
					m_static_label.SetWindowTextW(m_label_name);
					m_static_full_label.SetWindowTextW(m_full_label_name);
					GetInputValue(temp_info.nPoint_number,m_value,m_unit,m_AutoManual,digital_status);
					if(m_AutoManual.CompareNoCase(_T("M")) == 0)
					{			
						m_AutoManual = _T("Manual");	
						m_edit_value.EnableWindow(true);
					}
					else
					{
						m_AutoManual = _T("Auto");
						m_edit_value.EnableWindow(false);
					}
					m_edit_auto_manual.SetWindowTextW(m_AutoManual);
					m_edit_value.SetWindowTextW(m_value);
				}
			}
			break;
		case 0: //output
			{
				if(temp_info.nPoint_number >= BAC_OUTPUT_ITEM_COUNT)
				{
					PostMessage(WM_CLOSE,NULL,NULL);
					return ;
				}
				else
				{
					m_original_name.Format(_T("%d-OUT%d"),temp_info.nMain_Panel,temp_info.nPoint_number + 1);
					GetOutputLabel(temp_info.nPoint_number,m_label_name);
					GetOutputFullLabel(temp_info.nPoint_number,m_full_label_name);

					m_static_point.SetWindowTextW(m_original_name);
					m_static_label.SetWindowTextW(m_label_name);
					m_static_full_label.SetWindowTextW(m_full_label_name);
					GetOutputValue(temp_info.nPoint_number,m_value,m_unit,m_AutoManual,digital_status);
					if(m_AutoManual.CompareNoCase(_T("M")) == 0)
					{			
						m_AutoManual = _T("Manual");	
						m_edit_value.EnableWindow(true);
					}
					else
					{
						m_AutoManual = _T("Auto");
						m_edit_value.EnableWindow(false);
					}
					m_edit_auto_manual.SetWindowTextW(m_AutoManual);
					m_edit_value.SetWindowTextW(m_value);
				}
			}
			break;
		case 2: //VARIABLE
			{
				if(temp_info.nPoint_number >= BAC_VARIABLE_ITEM_COUNT)
				{
					PostMessage(WM_CLOSE,NULL,NULL);
					return ;
				}
				else
				{
					m_original_name.Format(_T("%d-VAR%d"),temp_info.nMain_Panel,temp_info.nPoint_number + 1);
					GetVariableLabel(temp_info.nPoint_number,m_label_name);
					GetVariableFullLabel(temp_info.nPoint_number,m_full_label_name);

					m_static_point.SetWindowTextW(m_original_name);
					m_static_label.SetWindowTextW(m_label_name);
					m_static_full_label.SetWindowTextW(m_full_label_name);
					GetVariableValue(temp_info.nPoint_number,m_value,m_unit,m_AutoManual,digital_status);
					if(m_AutoManual.CompareNoCase(_T("M")) == 0)
					{			
						m_AutoManual = _T("Manual");	
						m_edit_value.EnableWindow(true);
					}
					else
					{
						m_AutoManual = _T("Auto");
						m_edit_value.EnableWindow(false);
					}
					m_edit_auto_manual.SetWindowTextW(m_AutoManual);
					m_edit_value.SetWindowTextW(m_value);
				}
			}
			break;
		case 10:	//Screen  GRP
			{
				if(temp_info.nPoint_number >= BAC_SCREEN_COUNT)
				{
					PostMessage(WM_CLOSE,NULL,NULL);
					return ;
				}
				else
				{
					m_original_name.Format(_T("%d-GRP%d"),temp_info.nMain_Panel,temp_info.nPoint_number + 1);
					GetScreenLabel(temp_info.nPoint_number,m_label_name);
					GetScreenFullLabel(temp_info.nPoint_number,m_full_label_name);
					m_static_point.SetWindowTextW(m_original_name);
					m_static_label.SetWindowTextW(m_label_name);
					m_static_full_label.SetWindowTextW(m_full_label_name);
					m_edit_value.SetWindowTextW(m_original_name);
					m_edit_value.EnableWindow(false);
					CString temp_icon_path_screen;
					if(temp_info.nPoint_number < 16)
					{
						MultiByteToWideChar( CP_ACP, 0, (char *)m_screen_data.at(temp_info.nPoint_number).picture_file,(int)strlen((char *)m_screen_data.at(temp_info.nPoint_number).picture_file)+1, 
							temp_icon_path_screen.GetBuffer(MAX_PATH), MAX_PATH );
						temp_icon_path_screen.ReleaseBuffer();	
					}
					else
					{
						temp_icon_path_screen.Empty();
					}


					m_edit_icon_path.SetWindowTextW(temp_icon_path_screen);
					m_edit_icon_path.EnableWindow(false);

				}
			}
			break;
		default:
			{
				PostMessage(WM_CLOSE,NULL,NULL);
				return ;
			}
			break;
		}
		if((digital_status == 0) || (digital_status == 1))
		{
			m_edit_icon2_path.ShowWindow(true);
		}
		else
			m_edit_icon2_path.ShowWindow(false);

	}

	m_bkClrBtn.SetColor(label_info.nclrTxt);
	GetDlgItem(IDC_BUTTON_LABEL_EXIT)->SetFocus();
}


void CBacnetEditLabel::OnStnClickedStaticEditLabelAutoManual()
{
	// TODO: Add your control notification handler code here
	if((label_info.nSub_Panel == Station_NUM) && (label_info.nMain_Panel == Station_NUM))
		PostMessage(WM_EDIT_CHANGE_VALUE,CHANGE_AUTO_MANUAL,NULL);
	return;
}

// CBacnetInput message handlers
LRESULT  CBacnetEditLabel::MessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	CString temp_cs = pInvoke->task_info;
	if(msg_result)
	{
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);

		//MessageBox(Show_Results);
	}
	else
	{
		//memcpy_s(&m_Input_data.at(label_info.nPoint_number),sizeof(Str_in_point),&label_info,sizeof(Str_in_point));//还原没有改对的值
		//PostMessage(WM_REFRESH_BAC_INPUT_LIST,pInvoke->mRow,REFRESH_ON_ITEM);//这里调用 刷新线程重新刷新会方便一点;
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
		MessageBox(Show_Results);
	}

	if(pInvoke)
		delete pInvoke;
	return 0;
}




void CBacnetEditLabel::OnStnClickedStaticEditLabelDisplay()
{
	// TODO: Add your control notification handler code here
	if(label_info.nDisplay_Type == LABEL_SHOW_VALUE)
	{
		label_info.nDisplay_Type = LABEL_SHOW_FULL_DESCRIPTION;
		m_edit_display.SetWindowTextW(Label_Display_Array[label_info.nDisplay_Type]);
		ChangeWindowPos(0);
	}
	else if(label_info.nDisplay_Type == LABEL_SHOW_FULL_DESCRIPTION)
	{
		label_info.nDisplay_Type = LABEL_SHOW_LABEL;
		m_edit_display.SetWindowTextW(Label_Display_Array[label_info.nDisplay_Type]);
		ChangeWindowPos(0);
	}
	else if(label_info.nDisplay_Type == LABEL_SHOW_LABEL)
	{
		label_info.nDisplay_Type = LABEL_ICON_VALUE;
		m_edit_display.SetWindowTextW(Label_Display_Array[label_info.nDisplay_Type]);
		ChangeWindowPos(1);
	}
	else if(label_info.nDisplay_Type == LABEL_ICON_VALUE)
	{
		label_info.nDisplay_Type = LABEL_ICON_FULL_DESCRIPTION;
		m_edit_display.SetWindowTextW(Label_Display_Array[label_info.nDisplay_Type]);
		ChangeWindowPos(1);
	}
	else if(label_info.nDisplay_Type == LABEL_ICON_FULL_DESCRIPTION)
	{
		label_info.nDisplay_Type = LABEL_ICON_LABEL;
		m_edit_display.SetWindowTextW(Label_Display_Array[label_info.nDisplay_Type]);
		ChangeWindowPos(1);
	}
	else if(label_info.nDisplay_Type == LABEL_ICON_LABEL)
	{
		label_info.nDisplay_Type = LABEL_SHOW_VALUE;
		m_edit_display.SetWindowTextW(Label_Display_Array[label_info.nDisplay_Type]);
		ChangeWindowPos(0);
	}
}

void CBacnetEditLabel::ChangeWindowPos(bool nshow)
{
	CRect temprec;
	::GetWindowRect(m_edit_label,&temprec);	

	if(nshow)
	{
			MoveWindow(temprec.left,temprec.top,530,340);
			GetDlgItem(IDC_EDIT_ICON_PATH)->ShowWindow(1);
			GetDlgItem(IDC_STATIC_EDIT_TEXT_PLACE)->ShowWindow(1);
			GetDlgItem(IDC_STATIC_EDIT_ICON_SIZE)->ShowWindow(1);
			GetDlgItem(IDC_STATIC_ICON_PATH)->ShowWindow(1);
			GetDlgItem(IDC_STATIC_TEXT_PLACE)->ShowWindow(1);
			GetDlgItem(IDC_STATIC_ICON_SIZE)->ShowWindow(1);
			GetDlgItem(IDC_STATIC_ICON_GROUP)->ShowWindow(1);
	}
	else
	{
		MoveWindow(temprec.left,temprec.top,530,210);
		GetDlgItem(IDC_EDIT_ICON_PATH)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_EDIT_TEXT_PLACE)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_EDIT_ICON_SIZE)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_ICON_PATH)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_TEXT_PLACE)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_ICON_SIZE)->ShowWindow(0);
		GetDlgItem(IDC_STATIC_ICON_GROUP)->ShowWindow(0);
	}
}
void CBacnetEditLabel::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if((label_info.nclrTxt != label_info_buffer.nclrTxt) ||
		(label_info.nDisplay_Type != label_info_buffer.nDisplay_Type) ||
		(label_info.ntext_place != label_info_buffer.ntext_place) ||
		(label_info.n_iconsize != label_info_buffer.n_iconsize) ||
		(memcmp(label_info.ico_name , label_info_buffer.ico_name,10) != 0) ||
		(memcmp(label_info.ico_name_2 , label_info_buffer.ico_name_2,10) != 0))
	{
		Bacnet_Label_Info * temp_info = new Bacnet_Label_Info;
		memcpy(temp_info,&label_info,sizeof(Bacnet_Label_Info));		 
		 ::PostMessage(m_screenedit_dlg_hwnd,EDIT_LABEL_MESSAGE,(WPARAM)temp_info,NULL);
	}

	CDialogEx::OnClose();
}


void CBacnetEditLabel::OnStnClickedStaticEditTextPlace()
{
	// TODO: Add your control notification handler code here
	if(label_info.ntext_place == LABEL_TEXT_BOTTOM)
	{
		label_info.ntext_place = LABEL_TEXT_LEFT;
		m_edit_text_place.SetWindowTextW(Label_Text_Place[label_info.ntext_place]);
	}
	else if(label_info.ntext_place == LABEL_TEXT_LEFT)
	{
		label_info.ntext_place = LABEL_TEXT_TOP;
		m_edit_text_place.SetWindowTextW(Label_Text_Place[label_info.ntext_place]);
	}
	else if(label_info.ntext_place == LABEL_TEXT_TOP)
	{
		label_info.ntext_place = LABEL_TEXT_RIGHT;
		m_edit_text_place.SetWindowTextW(Label_Text_Place[label_info.ntext_place]);
	}
	else if(label_info.ntext_place == LABEL_TEXT_RIGHT)
	{
		label_info.ntext_place = LABEL_TEXT_BOTTOM;
		m_edit_text_place.SetWindowTextW(Label_Text_Place[label_info.ntext_place]);
	}

}


void CBacnetEditLabel::OnStnClickedStaticEditIconSize()
{
	// TODO: Add your control notification handler code here
	if(label_info.n_iconsize == LABEL_ICON_SMALL)
	{
		label_info.n_iconsize = LABEL_ICON_NORMAL;
		m_edit_icon_size.SetWindowTextW(Label_ICON_SIZE[label_info.n_iconsize]);
	}
	else if(label_info.n_iconsize == LABEL_ICON_NORMAL)
	{
		label_info.n_iconsize = LABEL_ICON_LARGE;
		m_edit_icon_size.SetWindowTextW(Label_ICON_SIZE[label_info.n_iconsize]);
	}
	else if(label_info.n_iconsize == LABEL_ICON_LARGE)
	{
		label_info.n_iconsize = LABEL_ICON_SMALL;
		m_edit_icon_size.SetWindowTextW(Label_ICON_SIZE[label_info.n_iconsize]);
	}
}

#if 0
void CBacnetEditLabel::OnEnChangeEditIconPath()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString FilePath;
	CString image_fordor;
	CString ApplicationFolder;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	image_fordor = ApplicationFolder + _T("\\Database\\image");
	SetCurrentDirectoryW(image_fordor);
	//选择图片,如果选的不在database目录下就copy一份过来;如果在的话就重命名，因为文件名长度不能超过10个字节;
	CString strFilter = _T("Ico file;jpg file;bmp file;png file|*.ico;*.jpg;*.bmp;*.png||");
	CFileDialog dlg(true,_T("Ico"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
	if(IDOK==dlg.DoModal())
	{
		FilePath=dlg.GetPathName();
		CString FileName;
		GetFileNameFromPath(FilePath,FileName);

		CString temp1;
		temp1 = FilePath;
		PathRemoveFileSpec(temp1.GetBuffer(MAX_PATH));
		temp1.ReleaseBuffer();

		if(FileName.GetLength() >9)
		{
			FileName = FileName.Right(9);
		}
		CString new_file_path;
		new_file_path = image_fordor + _T("\\") + FileName;
		if(temp1.CompareNoCase(image_fordor) != 0)//如果就在当前目录就不用copy过来了;
		{
			CopyFile(FilePath,new_file_path,false);
		}
		else
		{
			CFile::Rename(FilePath, new_file_path);
		}
		

		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, FileName.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(label_info.ico_name,10,cTemp1,10);
		m_edit_icon_path.SetWindowTextW(FileName);
		
	}
}
#endif


void CBacnetEditLabel::OnStnClickedEditIconPath()
{
	// TODO: Add your control notification handler code here
	CString FilePath;
	CString image_fordor;
	CString ApplicationFolder;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	image_fordor = ApplicationFolder + _T("\\Database\\image");
	SetCurrentDirectoryW(image_fordor);
	//选择图片,如果选的不在database目录下就copy一份过来;如果在的话就重命名，因为文件名长度不能超过10个字节;
	CString strFilter = _T("Ico file;jpg file;bmp file;png file|*.ico;*.jpg;*.bmp;*.png||");
	CFileDialog dlg(true,_T("Ico"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
	if(IDOK==dlg.DoModal())
	{
		FilePath=dlg.GetPathName();
		CString FileName;
		GetFileNameFromPath(FilePath,FileName);

		CString temp1;
		temp1 = FilePath;
		PathRemoveFileSpec(temp1.GetBuffer(MAX_PATH));
		temp1.ReleaseBuffer();

		if(FileName.GetLength() >9)
		{
			FileName = FileName.Right(9);
		}
		CString new_file_path;
		new_file_path = image_fordor + _T("\\") + FileName;
		if(temp1.CompareNoCase(image_fordor) != 0)//如果就在当前目录就不用copy过来了;
		{
			CopyFile(FilePath,new_file_path,false);
		}
		else
		{
			CFile::Rename(FilePath, new_file_path);
		}


		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, FileName.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(label_info.ico_name,10,cTemp1,10);
		m_edit_icon_path.SetWindowTextW(FileName);

	}
}


void CBacnetEditLabel::OnBnClickedButtonLabelExit()
{
	// TODO: Add your control notification handler code here
	PostMessage(WM_CLOSE,NULL,NULL);
}


void CBacnetEditLabel::OnStnClickedEditIconPath2()
{
	// TODO: Add your control notification handler code here
	CString FilePath;
	CString image_fordor;
	CString ApplicationFolder;
	GetModuleFileName(NULL, ApplicationFolder.GetBuffer(MAX_PATH), MAX_PATH);
	PathRemoveFileSpec(ApplicationFolder.GetBuffer(MAX_PATH));
	ApplicationFolder.ReleaseBuffer();
	image_fordor = ApplicationFolder + _T("\\Database\\image");
	SetCurrentDirectoryW(image_fordor);
	//选择图片,如果选的不在database目录下就copy一份过来;如果在的话就重命名，因为文件名长度不能超过10个字节;
	CString strFilter = _T("Ico file;jpg file;bmp file;png file|*.ico;*.jpg;*.bmp;*.png||");
	CFileDialog dlg(true,_T("Ico"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,strFilter);
	if(IDOK==dlg.DoModal())
	{
		FilePath=dlg.GetPathName();
		CString FileName;
		GetFileNameFromPath(FilePath,FileName);

		CString temp1;
		temp1 = FilePath;
		PathRemoveFileSpec(temp1.GetBuffer(MAX_PATH));
		temp1.ReleaseBuffer();

		if(FileName.GetLength() >9)
		{
			FileName = FileName.Right(9);
		}
		CString new_file_path;
		new_file_path = image_fordor + _T("\\") + FileName;
		if(temp1.CompareNoCase(image_fordor) != 0)//如果就在当前目录就不用copy过来了;
		{
			CopyFile(FilePath,new_file_path,false);
		}
		else
		{
			CFile::Rename(FilePath, new_file_path);
		}


		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, FileName.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(label_info.ico_name_2,10,cTemp1,10);
		m_edit_icon2_path.SetWindowTextW(FileName);

	}

}
