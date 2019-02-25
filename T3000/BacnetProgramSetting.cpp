// BacnetProgramSetting.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetProgramSetting.h"
#include "afxdialogex.h"

extern bool show_upper;
extern DWORD prg_text_color;
extern DWORD prg_label_color;
extern DWORD prg_command_color;
extern DWORD prg_local_var_color;
extern DWORD prg_function_color;
extern bool prg_color_change;
extern CString prg_character_font;
// CBacnetProgramSetting dialog

IMPLEMENT_DYNAMIC(CBacnetProgramSetting, CDialogEx)

CBacnetProgramSetting::CBacnetProgramSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetProgramSetting::IDD, pParent)
{

}

CBacnetProgramSetting::~CBacnetProgramSetting()
{
}

void CBacnetProgramSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetProgramSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBacnetProgramSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CBacnetProgramSetting message handlers


void CBacnetProgramSetting::OnBnClickedOk()
{
	
	COLORREF color_text = ((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_DEFAULT_TEXT))->GetColor();
	COLORREF color_label = ((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_LABEL))->GetColor();
	COLORREF color_function = ((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_FUNCTION))->GetColor();
	COLORREF color_command = ((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_COMMAND))->GetColor();
    COLORREF color_local_var = ((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_LOCAL_VAR))->GetColor();
    

	if(color_text == 0xffffffff)
		color_text = DEFAULT_PRG_TEXT_COLOR;
	if(color_label == 0xffffffff)
		color_label = DEFAULT_PRG_LABEL_COLOR;
	if(color_function == 0xffffffff)
		color_function = DEFAULT_PRG_FUNCTION_COLOR;
	if(color_command == 0xffffffff)
		color_command = DEFAULT_PRG_COMMAND_COLOR;
    if (color_local_var == 0xffffffff)
        color_local_var = DEFAULT_PRG_LOCAL_VAR_COLOR;

	CString temp_string;
	((CComboBox *)GetDlgItem(IDC_COMBO_PRG_SETTING_FONT))->GetWindowText(temp_string);

	int check_ret = ((CButton *)GetDlgItem(IDC_CHECK_PRG_SETTING_UPPER))->GetCheck();

	if( (color_text != prg_text_color)   || 
        (color_label != prg_label_color) || 
        (color_function != prg_function_color) || 
        (color_command != prg_command_color ) ||
        (color_local_var != prg_local_var_color) ||
        (temp_string.CompareNoCase(prg_character_font) != 0) || 
        (check_ret != show_upper) )
	{
		show_upper = check_ret;
		prg_color_change = true;
		prg_character_font = temp_string;
		prg_text_color = color_text;
		prg_label_color = color_label;
		prg_function_color = color_function;
		prg_command_color = color_command;
        prg_local_var_color = color_local_var;
		CString temp_color_text;
		CString temp_color_label;
		CString temp_color_function;
		CString temp_color_command;
        CString temp_color_local_var;
		CString temp_upper;
		temp_color_text.Format(_T("%u"),prg_text_color);
		temp_color_label.Format(_T("%u"),prg_label_color);
		temp_color_function.Format(_T("%u"),prg_function_color);
		temp_color_command.Format(_T("%u"),prg_command_color);
        temp_color_local_var.Format(_T("%u"), prg_local_var_color);
		temp_upper.Format(_T("%d"),show_upper);
		WritePrivateProfileString(_T("Program_IDE_Color"),_T("Text Color"),temp_color_text,g_cstring_ini_path);
		WritePrivateProfileString(_T("Program_IDE_Color"),_T("Label Color"),temp_color_label,g_cstring_ini_path);
		WritePrivateProfileString(_T("Program_IDE_Color"),_T("Function Color"),temp_color_function,g_cstring_ini_path);
		WritePrivateProfileString(_T("Program_IDE_Color"),_T("Command Color"),temp_color_command,g_cstring_ini_path);
        WritePrivateProfileString(_T("Program_IDE_Color"), _T("LOVAL_VAR Color"), temp_color_local_var, g_cstring_ini_path);
		WritePrivateProfileString(_T("Program_IDE_Color"),_T("Text Font"),prg_character_font,g_cstring_ini_path);
		WritePrivateProfileString(_T("Program_IDE_Color"),_T("Upper Case"),temp_upper,g_cstring_ini_path);
	}

	CDialogEx::OnOK();
}


BOOL CBacnetProgramSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_DEFAULT_TEXT))->EnableAutomaticButton(_T("Automatic"), DEFAULT_PRG_TEXT_COLOR);
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_DEFAULT_TEXT))->EnableOtherButton(_T("Other"));
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_DEFAULT_TEXT))->SetColor(prg_text_color);
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_DEFAULT_TEXT))->SetColumnsNumber(10);


	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_LABEL))->EnableAutomaticButton(_T("Automatic"), DEFAULT_PRG_LABEL_COLOR);
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_LABEL))->EnableOtherButton(_T("Other"));
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_LABEL))->SetColor(prg_label_color);
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_LABEL))->SetColumnsNumber(10);

	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_FUNCTION))->EnableAutomaticButton(_T("Automatic"), DEFAULT_PRG_FUNCTION_COLOR);
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_FUNCTION))->EnableOtherButton(_T("Other"));
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_FUNCTION))->SetColor(prg_function_color);
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_FUNCTION))->SetColumnsNumber(10);

	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_COMMAND))->EnableAutomaticButton(_T("Automatic"), DEFAULT_PRG_COMMAND_COLOR);
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_COMMAND))->EnableOtherButton(_T("Other"));
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_COMMAND))->SetColor(prg_command_color);
	((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_COMMAND))->SetColumnsNumber(10);

    ((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_LOCAL_VAR))->EnableAutomaticButton(_T("Automatic"), DEFAULT_PRG_LOCAL_VAR_COLOR);
    ((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_LOCAL_VAR))->EnableOtherButton(_T("Other"));
    ((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_LOCAL_VAR))->SetColor(prg_local_var_color);
    ((CMFCColorButton *)GetDlgItem(IDC_MFCCOLORBUTTON_LOCAL_VAR))->SetColumnsNumber(10);
    

	if(show_upper)
	{
		((CButton *)GetDlgItem(IDC_CHECK_PRG_SETTING_UPPER))->SetCheck(1);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_PRG_SETTING_UPPER))->SetCheck(0);
	}

	for(int j=0;j<sizeof(Program_Fonts)/sizeof(Program_Fonts[0]);j++)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_PRG_SETTING_FONT))->AddString(Program_Fonts[j]);
	}
	((CComboBox *)GetDlgItem(IDC_COMBO_PRG_SETTING_FONT))->SetWindowTextW(prg_character_font);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
