// BacnetProgramEdit.cpp : implementation file
// This File coded by Fance used to encode and decode programming.20130820
#include "stdafx.h"
#include "T3000.h"
#include "BacnetProgramEdit.h"
#include "BacnetProgram.h"
#include "afxdialogex.h"
#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "global_function.h"
#include "global_define.h"
#include "BacnetProgramSetting.h"
#include "BacnetProgramDebug.h"
#include "MainFrm.h"

// ===================== ?????????????????÷Ï??????????????????? =====================
// ???????????????????????????????100%????????????????
struct func_table {
	char func_name[15];
	unsigned char tok;
};
struct commands {
	char command[15];
	unsigned char tok;
};

// extern ??????????????????????????????????
extern func_table func_table[];
extern commands table[];
// ==========================================================================================

// ???????????????????CStringArray??Sort?????????
static int CALLBACK CompareCString(const void* pElement1, const void* pElement2)
{
	const CString* pStr1 = (const CString*)pElement1;
	const CString* pStr2 = (const CString*)pElement2;
	return pStr1->CompareNoCase(*pStr2);
}

bool need_syntax = false;
// Selected device information
extern tree_product selected_product_Node; // ??§Ö??õô???;
extern CBacnetProgramEdit* ProgramEdit_Window;
#define  WM_RICHEDIT_RIGHT_CLICK  WM_USER + 1001
extern char* ispoint_ex(char* token, int* num_point, byte* var_type, byte* point_type, int* num_panel, int* num_net, int network, unsigned char& sub_panel, byte panel, int* netpresent);
extern void clear_local_var();
CBacnetProgramDebug* Program_Debug_Window = NULL;
extern int error;
extern char* pmes;
// Controls clicking the refresh button and then updates all data colors
int refresh_program_text_color = false;  //????????¡ã??????????§Ö????????;
// Used for highlighting keywords
vector <Str_char_pos_color> m_prg_label_error_color;	//????highlight ???????;
// Used for highlighting keywords
vector <Str_char_pos_color> m_prg_char_color;	//????highlight ???????;
// Used to prevent flickering caused by frequent interface updates
vector <Str_char_pos_color> buffer_prg_char_color; //????????????????????????????;
CString program_string;
CString AnalysisString;
char editbuf[25000];
extern char my_display[10240];
extern int Encode_Program();
extern int my_lengthcode;
extern char mycode[2000];
extern void  init_info_table(void);
extern void Init_table_bank();
extern char mesbuf[1024];
extern int renumvar;
extern char* desassembler_program();
extern void copy_data_to_ptrpanel(int Data_type);
extern void check_high_light();
CString high_light_string;
HWND mParent_Hwnd;
bool show_upper;
bool program_re_line_number;
DWORD prg_text_color;
DWORD prg_label_color;
DWORD prg_command_color;
DWORD prg_local_var_color;
DWORD prg_function_color;
CString prg_character_font;
bool prg_color_change;
extern unsigned int debug_point_main;
extern unsigned int point_number;
extern unsigned int point_type;
POINT right_click_Point;
#define USE_PROMOT_PROGRAM 0

IMPLEMENT_DYNAMIC(CBacnetProgramEdit, CDialogEx)

CBacnetProgramEdit::CBacnetProgramEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetProgramEdit::IDD, pParent)
{
	Run_once_mutex = false;
	m_bAutoCompleteVisible = false;
	m_nInputStartPos = 0;
	m_bInAutoCompleteLogic = false; // ???????????????????
}

CBacnetProgramEdit::~CBacnetProgramEdit()
{
	m_autoCompleteFont.DeleteObject();
}

void CBacnetProgramEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFORMATION_WINDOW, m_information_window);
	DDX_Control(pDX, IDC_STATIC_POOL_SIZE, m_pool_size);
	DDX_Control(pDX, IDC_STATIC_SIZE, m_program_size);
	DDX_Control(pDX, IDC_STATIC_FREE_MEMORY, m_free_memory);
}

BEGIN_MESSAGE_MAP(CBacnetProgramEdit, CDialogEx)
	ON_MESSAGE(WM_HOTKEY, &CBacnetProgramEdit::OnHotKey)//?????????????????
	ON_MESSAGE(WM_REFRESH_BAC_PROGRAM_RICHEDIT, Fresh_Program_RichEdit)
	ON_MESSAGE(MY_RESUME_DATA, ProgramResumeMessageCallBack)
	ON_COMMAND(ID_SEND, &CBacnetProgramEdit::OnSend)
	ON_WM_CLOSE()
	ON_COMMAND(ID_CLEAR, &CBacnetProgramEdit::OnClear)
	ON_COMMAND(ID_LOADFILE, &CBacnetProgramEdit::OnLoadfile)
	ON_COMMAND(ID_SAVEFILE, &CBacnetProgramEdit::OnSavefile)
	ON_EN_SETFOCUS(IDC_RICHEDIT2_PROGRAM, &CBacnetProgramEdit::OnEnSetfocusRichedit2Program)
	ON_COMMAND(ID_REFRESH, &CBacnetProgramEdit::OnRefresh)
	ON_WM_HELPINFO()
	ON_COMMAND(ID_PROGRAM_IDE_SETTINGS, &CBacnetProgramEdit::OnProgramIdeSettings)
	ON_WM_TIMER()
	//	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_PROPERTIES_GOTODEFINITION, &CBacnetProgramEdit::OnPropertiesGotodefinition)
	ON_BN_CLICKED(IDC_BUTTON_PROGRAM_EDIT_HELP, &CBacnetProgramEdit::OnBnClickedButtonProgramEditHelp)
	ON_WM_SIZE()
	ON_COMMAND(ID_RENUMBER, &CBacnetProgramEdit::OnRenumber)
	// ========== ???????????????????? ==========
	ON_EN_CHANGE(IDC_RICHEDIT2_PROGRAM, &CBacnetProgramEdit::OnEnChangeRichedit2Program)
	ON_NOTIFY(EN_SELCHANGE, IDC_RICHEDIT2_PROGRAM, &CBacnetProgramEdit::OnEnSelchangeRichedit2Program)
	ON_LBN_DBLCLK(IDC_AUTOCOMPLETE_LIST, &CBacnetProgramEdit::OnLbnDblclkAutocompleteList)
	ON_LBN_SELCHANGE(IDC_AUTOCOMPLETE_LIST, &CBacnetProgramEdit::OnLbnSelchangeAutocompleteList)
	ON_WM_DESTROY()
	// ========== ??????????§Ò????????? ==========
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

// ?????RichEdit???¦Ë??£???????WM_NOTIFY?·Ú??
// ?????RichEdit???¦Ë??£???????WM_NOTIFY?·Ú??
void CBacnetProgramEdit::OnEnSelchangeRichedit2Program(NMHDR* pNMHDR, LRESULT* pResult)
{
	// ????????pResult????????????RichEdit?????
	*pResult = 0;
	// ?????????????????????
	if (m_bInAutoCompleteLogic) return;

	CRichEditCtrl* pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM);
	if (!pRichEdit || !IsWindow(pRichEdit->GetSafeHwnd()) || !IsWindow(GetSafeHwnd()))
	{
		HideAutoComplete();
		return;
	}

	// ?????????¦Ë??
	long nCurSelStart, nCurSelEnd;
	pRichEdit->GetSel(nCurSelStart, nCurSelEnd);

	// 1. ?????????????????????
	if (nCurSelStart != nCurSelEnd)
	{
		HideAutoComplete();
		return;
	}

	// 2. ?????????????????????
	if (nCurSelStart <= 0)
	{
		HideAutoComplete();
		return;
	}

	// 3. ????????????????????1????????§Ø??????????????????????
	CString strPrevChar;
	pRichEdit->GetTextRange(nCurSelStart - 1, nCurSelStart, strPrevChar);
	const CString strDelimiters = _T(" \r\n??"); // ????\r\n???????

	// 4. ????????????????????
	if (strDelimiters.Find(strPrevChar) >= 0)
	{
		HideAutoComplete();
		return;
	}
}
void CBacnetProgramEdit::OnDestroy()
{
	CDialogEx::OnDestroy();

	// ??????????????????????????????????????????????
	HideAutoComplete();
	if (IsWindow(m_autoCompleteList.GetSafeHwnd()))
	{
		m_autoCompleteList.DestroyWindow();
	}
	// ???????????
	m_autoCompleteFont.DeleteObject();
}

// CBacnetProgramEdit message handlers
LRESULT CBacnetProgramEdit::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	if (Run_once_mutex == true)	//If already exist some function run,it can't show another one;
		return -1;
	if (wParam == KEY_F2)
	{
		Run_once_mutex = true;
		OnSend();
		Run_once_mutex = false;
	}
	else if (wParam == KEY_F3)
	{
		Run_once_mutex = true;
		OnClear();
		Run_once_mutex = false;
	}
	else if (wParam == KEY_F7)
	{
		Run_once_mutex = true;
		OnLoadfile();
		Run_once_mutex = false;
	}
	else if (wParam == KEY_F6)
	{
		Run_once_mutex = true;
		OnSavefile();
		Run_once_mutex = false;
	}
	else if (wParam == KEY_F8)
	{
		Run_once_mutex = true;
		OnRefresh();
		Run_once_mutex = false;
	}
	else if (wParam == KEY_F9)
	{
		Run_once_mutex = true;
		OnRenumber();
		Run_once_mutex = false;
	}
	return 0;
}

void CBacnetProgramEdit::Initial_static()
{
	CString temp_cs_size, temp_cs_free;
	m_pool_size.SetWindowTextW(_T("2000"));
	m_pool_size.textColor(RGB(255, 0, 0));
	//m_static.bkColor(RGB(0,255,255));
	m_pool_size.setFont(15, 10, NULL, _T("Arial"));
	temp_cs_size.Format(_T("%d"), bac_program_size);
	m_program_size.SetWindowTextW(temp_cs_size);
	m_program_size.textColor(RGB(255, 0, 0));
	//m_static.bkColor(RGB(0,255,255));
	m_program_size.setFont(15, 10, NULL, _T("Arial"));
	temp_cs_free.Format(_T("%d"), bac_free_memory);
	m_free_memory.SetWindowTextW(temp_cs_free);
	m_free_memory.textColor(RGB(255, 0, 0));
	//m_static.bkColor(RGB(0,255,255));
	m_free_memory.setFont(15, 10, NULL, _T("Arial"));
}

extern char my_panel;
void CBacnetProgramEdit::SetRicheditFont(long nStartchar, long nEndchar, DWORD nColor)
{
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask |= CFM_BOLD;
	cf.dwEffects &= ~CFE_BOLD;
	//cf.dwEffects|=~CFE_BOLD; //???ÈÉ?????cf.dwEffects&=~CFE_BOLD;
	cf.dwMask |= CFM_ITALIC;
	cf.dwEffects &= ~CFE_ITALIC;
	//cf.dwEffects|=~CFE_ITALIC; //§Ò?ÈÉ?????cf.dwEffects&=~CFE_ITALIC;
	cf.dwMask |= CFM_UNDERLINE;
	cf.dwEffects &= ~CFE_UNDERLINE;
	//cf.dwEffects|=~CFE_UNDERLINE; //§Ò?ÈÉ?????cf.dwEffects&=~CFE_UNDERLINE;
	cf.dwMask |= CFM_COLOR;
	cf.crTextColor = nColor;//RGB(0,0,255); //???????
	cf.dwMask |= CFM_SIZE;
	cf.yHeight = 250; //??????
	cf.dwMask |= CFM_FACE;
	_tcscpy(cf.szFaceName, prg_character_font);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(nStartchar, nEndchar);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSelectionCharFormat(cf);
}

void CBacnetProgramEdit::GetColor()
{
	prg_text_color = (DWORD)GetPrivateProfileInt(_T("Program_IDE_Color"), _T("Text Color"), DEFAULT_PRG_TEXT_COLOR, g_cstring_ini_path);
	prg_label_color = (DWORD)GetPrivateProfileInt(_T("Program_IDE_Color"), _T("Label Color"), DEFAULT_PRG_LABEL_COLOR, g_cstring_ini_path);
	prg_command_color = (DWORD)GetPrivateProfileInt(_T("Program_IDE_Color"), _T("Command Color"), DEFAULT_PRG_COMMAND_COLOR, g_cstring_ini_path);
	prg_function_color = (DWORD)GetPrivateProfileInt(_T("Program_IDE_Color"), _T("Function Color"), DEFAULT_PRG_FUNCTION_COLOR, g_cstring_ini_path);
	prg_local_var_color = (DWORD)GetPrivateProfileInt(_T("Program_IDE_Color"), _T("LOVAL_VAR Color"), DEFAULT_PRG_LOCAL_VAR_COLOR, g_cstring_ini_path);
	program_re_line_number = (DWORD)GetPrivateProfileInt(_T("Program_IDE_Color"), _T("Redefine_LineNumber"), 1, g_cstring_ini_path);
	show_upper = (DWORD)GetPrivateProfileInt(_T("Program_IDE_Color"), _T("Upper Case"), 1, g_cstring_ini_path);
	GetPrivateProfileString(_T("Program_IDE_Color"), _T("Text Font"), _T("Arial"), prg_character_font.GetBuffer(MAX_PATH), MAX_PATH, g_cstring_ini_path);
	prg_character_font.ReleaseBuffer();

	bool found_font = false;
	for (int i = 0; i < (sizeof(Program_Fonts) / sizeof(Program_Fonts[0])); i++)
	{
		if (prg_character_font.CompareNoCase(Program_Fonts[i]) == 0)
		{
			found_font = true;
			break;
		}
	}
	if (!found_font)
	{
		prg_character_font.Format(_T("Arial"));
		WritePrivateProfileString(_T("Program_IDE_Color"), _T("Text Font"), _T("Arial"), g_cstring_ini_path);
	}
}

BOOL CBacnetProgramEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ========== ????????????RichEdit??????? ==========
	CRichEditCtrl* pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM);
	if (pRichEdit && IsWindow(pRichEdit->GetSafeHwnd()))
	{
		// 1. ???? RichEdit ??????????????? EN_CHANGE ??????????
		// ENM_CHANGE: ?????????????
		// ENM_SELCHANGE: ???????????? (???)
		pRichEdit->SetEventMask(ENM_CHANGE | ENM_SELCHANGE);
	}

	clear_local_var();
	CString ShowProgramText;
	CString temp_label;
	MultiByteToWideChar(CP_ACP, 0, (char*)m_Program_data.at(program_list_line).label, (int)strlen((char*)m_Program_data.at(program_list_line).label) + 1,
		temp_label.GetBuffer(MAX_PATH), MAX_PATH);
	temp_label.ReleaseBuffer();
	if (temp_label.IsEmpty())
	{
		temp_label.Format(_T("PRG%d"), program_list_line + 1);
	}
	ShowProgramText.Format(_T("Panel :  %u      Program  :  %u      Name  :  "), Station_NUM, program_list_line + 1);
	ShowProgramText = ShowProgramText + temp_label;
	SetWindowText(ShowProgramText);
	((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Unreg_Hotkey();
	prg_character_font.Empty();

	m_edit_changed = false;
	my_panel = bac_gloab_panel; //Set the panel number
	AnalysisString.Empty();
	GetDlgItem(IDC_RICHEDIT2_PROGRAM)->SetFocus();
	GetColor();
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask |= CFM_BOLD;
	cf.dwEffects &= ~CFE_BOLD;
	//cf.dwEffects|=~CFE_BOLD; //???ÈÉ?????cf.dwEffects&=~CFE_BOLD;
	cf.dwMask |= CFM_ITALIC;
	cf.dwEffects &= ~CFE_ITALIC;
	//cf.dwEffects|=~CFE_ITALIC; //§Ò?ÈÉ?????cf.dwEffects&=~CFE_ITALIC;
	cf.dwMask |= CFM_UNDERLINE;
	cf.dwEffects &= ~CFE_UNDERLINE;
	//cf.dwEffects|=~CFE_UNDERLINE; //§Ò?ÈÉ?????cf.dwEffects&=~CFE_UNDERLINE;
	cf.dwMask |= CFM_COLOR;
	cf.crTextColor = prg_text_color;//RGB(0,0,255); //???????
	cf.dwMask |= CFM_SIZE;
	cf.yHeight = 250; //??????
	cf.dwMask |= CFM_FACE;
	//_tcscpy(cf.szFaceName ,_T("SimSun-ExtB"));
	//_tcscpy(cf.szFaceName ,_T("Times New Roman"));
	//	strcpy(cf.szFaceName ,_T("????")); //????????
	_tcscpy(cf.szFaceName, prg_character_font);
	//_tcscpy(cf.szFaceName ,_T("NSimSun"));
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSelectionCharFormat(cf);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetDefaultCharFormat(cf);
	//((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->PasteSpecial(CF_TEXT);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
	RegisterHotKey(GetSafeHwnd(), KEY_F2, NULL, VK_F2);//F2??
	RegisterHotKey(GetSafeHwnd(), KEY_F3, NULL, VK_F3);
	RegisterHotKey(GetSafeHwnd(), KEY_F7, NULL, VK_F7);
	RegisterHotKey(GetSafeHwnd(), KEY_F6, NULL, VK_F6);
	RegisterHotKey(GetSafeHwnd(), KEY_F8, NULL, VK_F8);
	RegisterHotKey(GetSafeHwnd(), KEY_F9, NULL, VK_F9);
	Initial_static();
	init_info_table();
	Init_table_bank();
	mParent_Hwnd = g_hwnd_now;
	prg_color_change = false;
	m_program_edit_hwnd = this->m_hWnd;
	g_hwnd_now = m_program_edit_hwnd;
	copy_data_to_ptrpanel(TYPE_ALL);
	memset(my_display, 0, sizeof(my_display));
	PostMessage(WM_REFRESH_BAC_PROGRAM_RICHEDIT, NULL, NULL);
	if (show_upper)
	{
		BYTE Keystatus[256] = { 0 };
		GetKeyboardState(Keystatus);
		Keystatus[VK_CAPITAL] = 1;
		SetKeyboardState(Keystatus);
		PostMessage(WM_KEYDOWN, VK_CAPITAL, 0);
	}
	SetTimer(2, 1000, NULL);
	::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
#if  !USE_PROMOT_PROGRAM
	GetDlgItem(IDC_LIST_PROMPT)->ShowWindow(0);
#endif

	// ========== ??????????§Ò????????? ==========
// 1. ????RichEdit?????????????§¹
	//CRichEditCtrl* pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM);
	if (!pRichEdit || !IsWindow(pRichEdit->GetSafeHwnd()))
	{
		AfxMessageBox(_T("RichEdit??????????????ID??????"));
	}
	else
	{
		// 2. ??????????????
		InitAutoCompleteKeywords();
		// §µ??????????????
		ASSERT(m_autoCompleteKeywords.size() > 0);
		TRACE(_T("???????????????????%d???????\n"), (int)m_autoCompleteKeywords.size());

		// 3. ????????????????????this???????????
		DWORD dwStyle = WS_CHILD | WS_BORDER | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT | WS_VSCROLL
			| LBS_OWNERDRAWFIXED | LBS_HASSTRINGS; // ??????????????+?????????
		BOOL bListCreateOK = m_autoCompleteList.Create(
			dwStyle,
			CRect(0, 0, 350, 200),
			this,
			IDC_AUTOCOMPLETE_LIST
		);

		if (!bListCreateOK)
		{
			AfxMessageBox(_T("?????§Ò?????????"));
		}
		else
		{
			m_autoCompleteList.SetItemHeight(0, 20);
			// ???????????????
			LOGFONT lf;
			ZeroMemory(&lf, sizeof(LOGFONT));
			_tcscpy(lf.lfFaceName, prg_character_font);
			lf.lfHeight = -MulDiv(10, GetDeviceCaps(GetDC()->GetSafeHdc(), LOGPIXELSY), 72);
			m_autoCompleteFont.DeleteObject();
			m_autoCompleteFont.CreateFontIndirect(&lf);
			m_autoCompleteList.SetFont(&m_autoCompleteFont);
			// ???????
			m_autoCompleteList.ShowWindow(SW_HIDE);
			m_bAutoCompleteVisible = false;
		}
	}

	// ?????§Ù????????????????????
	return FALSE;
}

LRESULT CBacnetProgramEdit::Fresh_Program_RichEdit(WPARAM wParam, LPARAM lParam)
{

	//	CString temp2;
	char* temp_point;
	try
	{
		temp_point = desassembler_program();
	}
	catch (...)
	{
		temp_point = NULL;
	}

	if (temp_point == NULL)
	{
		SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Decode Error!"));
		return 1;
	}
	SetPaneString(BAC_SHOW_MISSION_RESULTS, _T("Decode success!"));
	Initial_static();
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
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetWindowTextW(temp1);
	m_edit_changed = false;
	program_string = temp1;
#ifdef LOCAL_DB_FUNCTION
	if (selected_product_Node.serial_number != 0)
		WriteDeviceDataIntoAccessDB(READPROGRAMCODE_T3000, program_list_line, selected_product_Node.serial_number);
#endif
	UpdateDataProgramText();
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(-1, -1);
	return 0;
}

void CBacnetProgramEdit::OnOK()
{

	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetFocus();
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(-1, -1);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->ReplaceSel(_T("\n"));
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetFocus();
	/*CDialogEx::OnOK();*/
}

LRESULT  CBacnetProgramEdit::ProgramResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo* pInvoke = (_MessageInvokeIDInfo*)lParam;
	CString temp_cs = pInvoke->task_info;
	bool msg_result = WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	if (msg_result)
	{
		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS, Show_Results);
		CString Edit_Buffer;
		GetDlgItemText(IDC_RICHEDIT2_PROGRAM, Edit_Buffer);
		program_string = Edit_Buffer;
		MessageBox(Show_Results);
	}
	else
	{
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS, Show_Results);
		//#ifdef SHOW_ERROR_MESSAGE
		MessageBox(Show_Results);
		//#endif
	}
	if (pInvoke)
		delete pInvoke;
	return 0;
}

void CBacnetProgramEdit::Delete_Repeat_Char_Vec()
{
}

// syntax  call  encode function  if some error ,show the error text with red color .
void CBacnetProgramEdit::Syntax_analysis()
{
	int value_test = ((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetScrollPos(SB_VERT);
	//TRACE(_T("SB_VERT = %d\r\n"),value_test);
	//((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->ShowScrollBar(SB_BOTH,0);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetReadOnly(TRUE);
	memset(program_code[program_list_line], 0, 2000);
	renumvar = program_re_line_number;
	error = -1; //Default no error;
	CString tempcs;
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetWindowTextW(tempcs);
	tempcs.MakeUpper();
	int    iTextLen;
	iTextLen = WideCharToMultiByte(CP_ACP, 0, tempcs, -1, NULL, 0, NULL, NULL);
	memset((void*)editbuf, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, tempcs, -1, editbuf, iTextLen, NULL, NULL);
	Encode_Program();
	long temp_sel_str = 0;
	long temp_sel_end = 0;
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetSel(temp_sel_str, temp_sel_end); //?????????????????;
	//((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->ShowScrollBar(SB_VERT,FALSE);
	UpdateDataProgramText();

	m_prg_label_error_color.clear();
	if (error != -1)
	{
		m_information_window.ResetContent();
		CString cstring_error;
		int len = strlen(mesbuf);
		int  unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, mesbuf, -1, NULL, 0);
		::MultiByteToWideChar(CP_ACP, 0, mesbuf, -1, cstring_error.GetBuffer(2000), unicodeLen);
		cstring_error.ReleaseBuffer();
		CStringArray  Error_info;
		SplitCStringA(Error_info, cstring_error, _T("\r\n"));//Split the CString with "\r\n" and then add to the list.(Fance)
		m_information_window.ResetContent();
		tempcs.Replace(_T("\r\n"), _T("\n"));
		for (int i = 0; i < (int)Error_info.GetSize(); i++)
		{
			m_information_window.InsertString(i, Error_info.GetAt(i));
			CString temp_error_info;
			temp_error_info = Error_info.GetAt(i);

			if (temp_error_info.Find(_T("not a variable")) >= 0)
			{
				CStringArray temp_array;
				CString ret_error_cstring;
				SplitCStringA(temp_array, temp_error_info, _T(":"));
				if (temp_array.GetSize() == 2)
				{
					ret_error_cstring = temp_array.GetAt(1);
					ret_error_cstring.Trim();
					ret_error_cstring.MakeUpper();
					int char_length = ret_error_cstring.GetLength();
					int	find_char_pos = tempcs.Find(ret_error_cstring);
					int temp_start_pos = find_char_pos;
					int temp_stop_pos = find_char_pos + char_length;
					Str_char_pos_color temp_pos_color;
					temp_pos_color.startpos = temp_start_pos;
					temp_pos_color.endpos = temp_stop_pos;
					temp_pos_color.ncolor = DEFAULT_PRG_ERROR_LABEL_COLOR;
					temp_pos_color.key_type = KEY_ERROR_LABEL;
					m_prg_label_error_color.push_back(temp_pos_color);
				}
				else
					continue;
			}
		}
	}
	else
		m_information_window.ResetContent();
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->HideSelection(TRUE, FALSE);
	//SetRicheditFont(0,-1,prg_text_color);
	for (int i = 0; i < m_prg_label_error_color.size(); i++)
	{
		SetRicheditFont(m_prg_label_error_color.at(i).startpos, m_prg_label_error_color.at(i).endpos, m_prg_label_error_color.at(i).ncolor);
	}
	SetRicheditFont(0, 0, prg_text_color);
	//if(temp_sel_str!= temp_sel_end)
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(temp_sel_end, temp_sel_end); //????????????;
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->HideSelection(FALSE, FALSE);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetReadOnly(FALSE);
	value_test = ((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetScrollPos(SB_VERT, value_test, 1);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SendMessage(WM_VSCROLL, 0, 0);
	program_code_length[program_list_line] = my_lengthcode;
	//Invalidate(0);
	//not a variable
}

void CBacnetProgramEdit::OnSend()
{
	//reset the program buffer
	memset(program_code[program_list_line], 0, 2000);
	renumvar = program_re_line_number;
	error = -1; //Default no error;
	CString tempcs;
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetWindowTextW(tempcs);
	tempcs.MakeUpper();
	//	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte(CP_ACP, 0, tempcs, -1, NULL, 0, NULL, NULL);
	memset((void*)editbuf, 0, sizeof(char) * (iTextLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, tempcs, -1, editbuf, iTextLen, NULL, NULL);
	//2017/ 12 / 04   dufan ????????? ??THEN ????????IF ?????? ???????????????????????????????
	if (tempcs.Find(_T("THEN IF")) != -1)
	{
		MessageBox(_T("Don't allowed use nested 'IF'."));
		return;
	}
	Encode_Program();
	if (error == -1)
	{

		TRACE(_T("Encode_Program length is %d ,copy length is %d\r\n"), program_code_length[program_list_line], my_lengthcode);
		if (my_lengthcode > 1990)
		{
			MessageBox(_T("Encode Program Code Length is too large"));
			return;
		}
		if ((Device_Basic_Setting.reg.panel_type == PM_MINIPANEL) && (my_lengthcode > 9))// 
		{
			int total_program_size = 0;
			for (int i = 0; i < (int)m_Program_data.size(); i++)
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
		if ((my_lengthcode > program_code_length[program_list_line]) &&
			my_lengthcode < 2000)
		{
			program_code_length[program_list_line] = my_lengthcode;
		}
		if (program_code_length[program_list_line] >= 9)
		{
			bac_program_size = program_code_length[program_list_line] - 9;// my_lengthcode;
			bac_free_memory = 2000 - bac_program_size;
		}
		else
		{
			bac_program_size = program_code_length[program_list_line];// my_lengthcode;
			bac_free_memory = 2000 - bac_program_size;
		}
		int npart = (my_lengthcode / 401) + 1;
		bool b_program_status = true;
		if (g_protocol == PROTOCOL_BIP_TO_MSTP)
		{
			for (int j = 0; j < npart; j++)
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
			for (int j = 0; j < npart; j++)
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
						MessageBox(_T("Write Program Code Timeout."));
						return;
					}
					temp_invoke_id = WriteProgramData(g_bac_instance, WRITEPROGRAMCODE_T3000, program_list_line, program_list_line, j);
					Sleep(SEND_COMMAND_DELAY_TIME);
					if ((g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER) && (temp_invoke_id >= 0))
					{
						break;
					}

				} while (temp_invoke_id < 0);
				if ((g_protocol_support_ptp == PROTOCOL_MB_PTP_TRANSFER) && (temp_invoke_id >= 0))
				{
					continue;
				}
				if (send_status)
				{
					for (int i = 0; i < 3000; i++)
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
					MessageBox(_T("Write Program Code Timeout!!"));
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
				char* readytowrite;
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
		m_information_window.ResetContent();
		m_information_window.InsertString(0, _T("Resource Compile succeeded."));
		m_Program_data.at(program_list_line).bytes = bac_program_size;
		Initial_static();
		CString temp1;
		((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetWindowTextW(temp1);
		m_edit_changed = false;
		program_string = temp1;
	}
	else
	{
		m_information_window.ResetContent();
		CString cstring_error;
		int len = strlen(mesbuf);
		int  unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, mesbuf, -1, NULL, 0);
		::MultiByteToWideChar(CP_ACP, 0, mesbuf, -1, cstring_error.GetBuffer(2000), unicodeLen);
		cstring_error.ReleaseBuffer();
		//		MessageBox(cstring_error);
		CStringArray  Error_info;
		SplitCStringA(Error_info, cstring_error, _T("\r\n"));//Split the CString with "\r\n" and then add to the list.(Fance)
		Sleep(1);
		m_information_window.ResetContent();
		for (int i = 0; i < (int)Error_info.GetSize(); i++)
		{
			m_information_window.InsertString(i, Error_info.GetAt(i));
		}
		MessageBox(_T("Errors,program NOT Sent!"));
	}
	UpdateDataProgramText();
	//PostMessage(WM_REFRESH_BAC_PROGRAM_RICHEDIT, NULL, NULL);
}

void CBacnetProgramEdit::OnClose()
{
	// ??????????????
	HideAutoComplete();
	if (IsWindow(m_autoCompleteList.GetSafeHwnd()))
	{
		m_autoCompleteList.DestroyWindow();
	}

	UnregisterHotKey(GetSafeHwnd(), KEY_F2);//???F2??
	UnregisterHotKey(GetSafeHwnd(), KEY_F3);
	UnregisterHotKey(GetSafeHwnd(), KEY_F7);
	UnregisterHotKey(GetSafeHwnd(), KEY_F6);
	UnregisterHotKey(GetSafeHwnd(), KEY_F8);
	g_hwnd_now = mParent_Hwnd;
	if (m_pragram_dlg_hwnd != NULL)
		::PostMessage(m_pragram_dlg_hwnd, WM_REFRESH_BAC_PROGRAM_LIST, NULL, NULL);
	CDialogEx::OnClose();
}

void CBacnetProgramEdit::OnClear()
{

	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetWindowTextW(_T(""));
}

void CBacnetProgramEdit::OnLoadfile()
{

	CString FilePath;
	CString ReadBuffer;
	CFileDialog dlg(true, _T("*.txt"), _T(" "), OFN_HIDEREADONLY, _T("txt files (*.txt)|*.txt|All Files (*.*)|*.*||"), NULL, 0);
	if (IDOK == dlg.DoModal())
	{
		FilePath = dlg.GetPathName();
		//Write_Position = pParent->myapp_path + cs_file_time;
		char* pBuf;
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
		((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetWindowTextW(ReadBuffer);
		((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(unicodeLen, unicodeLen);
	}
}

void CBacnetProgramEdit::OnSavefile()
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
		GetDlgItemText(IDC_RICHEDIT2_PROGRAM, Write_Buffer);
		//char *readytowrite = 
		char* readytowrite;
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

void CBacnetProgramEdit::OnCancel()
{
	// ??????????????
	HideAutoComplete();
	if (IsWindow(m_autoCompleteList.GetSafeHwnd()))
	{
		m_autoCompleteList.DestroyWindow();
	}

	CString Edit_Buffer;
	GetDlgItemText(IDC_RICHEDIT2_PROGRAM, Edit_Buffer);
	if (program_string.CompareNoCase(Edit_Buffer) == 0)
	{
		((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Reg_Hotkey();
		CDialogEx::OnCancel();
	}
	else
	{
		if (MessageBox(_T("Do you want to exit the programming without saving?"), _T("Prompting"), MB_ICONINFORMATION | MB_YESNO) == IDYES)
		{
			((CBacnetProgram*)pDialog[WINDOW_PROGRAM])->Reg_Hotkey();
			CDialogEx::OnCancel();
		}
	}
}

//Fance 
//This part of code is use to make sure when the rich edit loss focus,and get focus later,it not select none of the character
//Move the mouse point to the end of the text.
void CBacnetProgramEdit::OnEnSetfocusRichedit2Program()
{
	// ????????????§³§Õ??????????????????EN_CHANGE???
	if (show_upper)
	{
		BYTE Keystatus[256] = { 0 };
		GetKeyboardState(Keystatus);
		Keystatus[VK_CAPITAL] = 1;
		SetKeyboardState(Keystatus);
	}
	else
	{
		BYTE Keystatus[256] = { 0 };
		GetKeyboardState(Keystatus);
		Keystatus[VK_CAPITAL] = 0;
		SetKeyboardState(Keystatus);
	}

	// ??????????????????¦Â
	CString temp;
	GetDlgItemText(IDC_RICHEDIT2_PROGRAM, temp);
	int length = temp.GetLength();
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(length, length);
}

void CBacnetProgramEdit::OnRefresh()
{
	refresh_program_text_color = true;
	copy_data_to_ptrpanel(TYPE_ALL);
	memset(mycode, 0, 2000);
	for (int x = 0; x < 5; x++)
	{
		int send_status = true;
		int resend_count = 0;
		int temp_invoke_id = -1;
		do
		{
			resend_count++;
			if (resend_count > RESEND_COUNT)
				return;
			temp_invoke_id = GetProgramData(g_bac_instance, program_list_line, program_list_line, x);
			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (temp_invoke_id < 0);
		Sleep(SEND_COMMAND_DELAY_TIME);
		if (send_status)
		{
			for (int i = 0; i < 2000; i++)
			{
				Sleep(1);
				if (tsm_invoke_id_free(temp_invoke_id))
				{
					goto	dlg_part_success;
				}
			}
			return;
		dlg_part_success:
			continue;
		}
	}
	Sleep(100);
	PostMessage(WM_REFRESH_BAC_PROGRAM_RICHEDIT, NULL, NULL);
}

BOOL CBacnetProgramEdit::OnHelpInfo(HELPINFO* pHelpInfo)
{

	if (g_protocol == PROTOCOL_BACNET_IP) {
		HWND hWnd;
		if (pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
		else
			hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_9_PROGRAMMING_A_CONTROLLER);
		return (hWnd != NULL);
	}
	else {
		::HtmlHelp(NULL, theApp.m_szHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OVERVIEW);
	}
	return CDialogEx::OnHelpInfo(pHelpInfo);
}

void CBacnetProgramEdit::OnProgramIdeSettings()
{

	CBacnetProgramSetting ProgramSettingdlg;
	ProgramSettingdlg.DoModal();
	CString tempcs;
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetWindowTextW(tempcs);
	if (show_upper)
	{
		tempcs.MakeUpper();
	}
	else
	{
		tempcs.MakeLower();
	}
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetWindowTextW(tempcs);
	refresh_program_text_color = true;
	need_syntax = true; //??????
	Syntax_analysis();
}

int CBacnetProgramEdit::Bacnet_Show_Debug(CString& retselstring)
{
	CString Select_string;
	Select_string = ((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetSelText();
	Select_string.Trim();
	if (Select_string.GetLength() == 0)
	{
		long sel_start = 0;
		long sel_end = 0;
		((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetSel(sel_start, sel_end);
		long caculate_start = 0;
		long caculate_end = 0;
		long temp_start = sel_start;
		long temp_end = sel_end;
		CString temp_txt;
		if (temp_start > 0)
		{
			temp_start--;
			do
			{
				int sel_number_charactor;
				sel_number_charactor = ((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetTextRange(temp_start, temp_end, temp_txt);
				if ((temp_txt.CompareNoCase(_T(" ")) == 0) || (sel_number_charactor == 0)) //?????????? ???????????????????;
				{
					caculate_start = temp_start + 1;
					break;
				}
				temp_start--;
				temp_end--;
			} while (temp_start > 0);
		}
		temp_start = sel_start;
		temp_end = sel_end;
		if (temp_end > 0)
		{
			temp_end++;
			do
			{
				int sel_number_charactor;
				sel_number_charactor = ((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetTextRange(temp_start, temp_end, temp_txt);
				if ((temp_txt.CompareNoCase(_T(" ")) == 0) || (sel_number_charactor == 0)) //?????????? ???????????????????;
				{
					caculate_end = temp_end - 1;
					break;
				}
				temp_start++;
				temp_end++;
			} while (temp_start > 0);
		}
		if (caculate_end > caculate_start)
		{
			((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetTextRange(caculate_start, caculate_end, temp_txt);
			((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(caculate_start, caculate_end);
			temp_txt.Trim();
			Select_string = temp_txt;
		}
		else
			return -1;
		//	MessageBox(_T("No Character selected .Please select label"),_T("Notice"),MB_OK);
	}
	retselstring = Select_string;
	if (Select_string.GetLength() > 10)
	{
		return false;
	}

	char temp_point[20];
	memset(temp_point, 0, 20);
	WideCharToMultiByte(CP_ACP, 0, Select_string.GetBuffer(), -1, temp_point, 20, NULL, NULL);
	int temp_number = -1;
	byte temp_value_type = -1;
	byte temp_point_type = -1;
	int temp_panel = -1;
	int temp_net = -1;
	int k = 0;
	unsigned char sub_panel = -1;
	char* tempcs = NULL;
	tempcs = ispoint_ex(temp_point, &temp_number, &temp_value_type, &temp_point_type, &temp_panel, &temp_net, 0, sub_panel, Station_NUM, &k);
	if (tempcs == NULL)
		return false;
	// Remote Modbus points (e.g. 150.12.MB_REG56) are executed by VirtualProgramEngine
	// over Modbus TCP; they are not opened in the local point debug window.
	bool is_remote_modbus =
		(temp_point_type == MB_REG) ||
		(temp_point_type == COIL_REG) ||
		(temp_point_type == DIS_INPUT_REG) ||
		(temp_point_type == INPUT_REG) ||
		(temp_point_type == BAC_HOLDING_FLOAT_ABCD) ||
		(temp_point_type == BAC_HOLDING_FLOAT_CDAB) ||
		(temp_point_type == BAC_HOLDING_FLOAT_BADC) ||
		(temp_point_type == BAC_HOLDING_FLOAT_DCBA) ||
		(temp_point_type == BAC_INPUT_FLOAT_ABCD) ||
		(temp_point_type == BAC_INPUT_FLOAT_CDAB) ||
		(temp_point_type == BAC_INPUT_FLOAT_BADC) ||
		(temp_point_type == BAC_INPUT_FLOAT_DCBA);
	if (is_remote_modbus)
	{
		debug_point_main = temp_panel;
		point_number = temp_number; // MB_REG is base-0
		point_type = temp_point_type;
		return true;
	}
	if (temp_panel != Station_NUM)
	{
		MessageBox(_T("The remote panel is not supported at this time. \r\nThis feature will be added soon."));
		return -2;
	}
	else
		debug_point_main = Station_NUM;
	point_number = temp_number - 1;
	point_type = temp_point_type;
	//fandu ??? ???? input output debug ???????????????????
	switch (point_type)
	{
	case BAC_OUT:
	case BAC_IN:
	case BAC_PID:
	case BAC_VAR:
	case BAC_SCH:
	case BAC_HOL:
		if ((point_number >= BAC_OUTPUT_ITEM_COUNT) && (point_type == BAC_OUT))
			break;
		if ((point_number >= BAC_INPUT_ITEM_COUNT) && (point_type == BAC_IN))
			break;
		if ((point_number >= BAC_PID_COUNT) && (point_type == BAC_PID))
			break;
		if ((point_number >= BAC_VARIABLE_ITEM_COUNT) && (point_type == BAC_VAR))
			break;
		if ((point_number >= BAC_VARIABLE_ITEM_COUNT) && (point_type == BAC_SCH))
			break;
		if ((point_number >= BAC_VARIABLE_ITEM_COUNT) && (point_type == BAC_HOL))
			break;
		if (Program_Debug_Window != NULL)
		{
			delete Program_Debug_Window;
			Program_Debug_Window = NULL;
		}
		if (Program_Debug_Window == NULL)
		{
			Program_Debug_Window = new CBacnetProgramDebug;
			Program_Debug_Window->Create(IDD_DIALOG_BACNET_PROGRAM_DEBUG, this);
		}
		Program_Debug_Window->Initial_List(point_type);
		Program_Debug_Window->SetWindowTextW(Select_string);
		Program_Debug_Window->ShowWindow(TRUE);
		return true;
		break;
	default:
		break;
	}
	return true;
}

void CBacnetProgramEdit::OnTimer(UINT_PTR nIDEvent)
{

	switch (nIDEvent)
	{
	case 1:
	{
		if (need_syntax)
		{
			KillTimer(1);
			Syntax_analysis();
			need_syntax = false;
		}
	}
	break;
	case 2:
	{
		//if(this->GetParent()->IsTopParentActive())
		//{
		//	::SetWindowPos(this->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		//}
	}
	break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CBacnetProgramEdit::UpdateDataProgramText()
{
	CString Edit_Buffer;
	GetDlgItemText(IDC_RICHEDIT2_PROGRAM, Edit_Buffer);
	if ((AnalysisString.CompareNoCase(Edit_Buffer) != 0) || (refresh_program_text_color == true))
	{
		refresh_program_text_color = false;
		AnalysisString = Edit_Buffer;
		CString tempcs;
		((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetWindowTextW(tempcs);
		tempcs.MakeUpper();
		high_light_string = tempcs;
		check_high_light();
		long temp_sel_str = 0;
		long temp_sel_end = 0;
		((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetSel(temp_sel_str, temp_sel_end); //?????????????????;
		((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->HideSelection(TRUE, FALSE);
		SetRicheditFont(0, -1, prg_text_color);
		//int value_test = ((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetScrollPos(SB_VERT);
		//TRACE(_T("SB_VERT = %d\r\n"),value_test);
		for (int i = 0; i < m_prg_char_color.size(); i++)
		{
			SetRicheditFont(m_prg_char_color.at(i).startpos, m_prg_char_color.at(i).endpos, m_prg_char_color.at(i).ncolor);
		}
		//SetRicheditFont(0,0,prg_text_color);111111111
		((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(temp_sel_end, temp_sel_end); //????????????;
		((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->HideSelection(FALSE, FALSE);
	}
	else
	{
		TRACE(_T("The same\r\n"));
	}
	SetBackFont();
	return;
}

void CBacnetProgramEdit::SetBackFont()
{
	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask |= CFM_BOLD;
	cf.dwEffects &= ~CFE_BOLD;
	//cf.dwEffects|=~CFE_BOLD; //???ÈÉ?????cf.dwEffects&=~CFE_BOLD;
	cf.dwMask |= CFM_ITALIC;
	cf.dwEffects &= ~CFE_ITALIC;
	//cf.dwEffects|=~CFE_ITALIC; //§Ò?ÈÉ?????cf.dwEffects&=~CFE_ITALIC;
	cf.dwMask |= CFM_UNDERLINE;
	cf.dwEffects &= ~CFE_UNDERLINE;
	//cf.dwEffects|=~CFE_UNDERLINE; //§Ò?ÈÉ?????cf.dwEffects&=~CFE_UNDERLINE;
	cf.dwMask |= CFM_COLOR;
	cf.crTextColor = prg_text_color;//RGB(0,0,255); //???????
	cf.dwMask |= CFM_SIZE;
	cf.yHeight = 250; //??????
	cf.dwMask |= CFM_FACE;
	//_tcscpy(cf.szFaceName ,_T("SimSun-ExtB"));
	//_tcscpy(cf.szFaceName ,_T("Times New Roman"));
	//	strcpy(cf.szFaceName ,_T("????")); //????????
	_tcscpy(cf.szFaceName, prg_character_font);
	//_tcscpy(cf.szFaceName ,_T("NSimSun"));
	//((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSelectionCharFormat(cf);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetDefaultCharFormat(cf);
};
BOOL CBacnetProgramEdit::PreTranslateMessage(MSG* pMsg)
{
	// ========== ???????????????????????????? ==========
	if (m_bAutoCompleteVisible && IsWindow(m_autoCompleteList.GetSafeHwnd()))
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			switch (pMsg->wParam)
			{
			case VK_DOWN: // ???????§Ý?????????
			{
				int nCurSel = m_autoCompleteList.GetCurSel();
				int nCount = m_autoCompleteList.GetCount();
				if (nCurSel < nCount - 1) m_autoCompleteList.SetCurSel(nCurSel + 1);
				return TRUE; // ????????????????RichEdit
			}
			case VK_UP: // ???????§Ý?????????
			{
				int nCurSel = m_autoCompleteList.GetCurSel();
				if (nCurSel > 0) m_autoCompleteList.SetCurSel(nCurSel - 1);
				return TRUE;
			}
			case VK_RETURN: // ???????????
			case VK_TAB:    // Tab????????
				OnAutoCompleteSelect();
				return TRUE;
			case VK_ESCAPE: // ESC??????????
				HideAutoComplete();
				return TRUE;
			}
		}
	}

	if (pMsg->message == WM_KILLFOCUS)
	{
		HWND hNewFocusWnd = (HWND)pMsg->wParam;
		CWnd* pNewFocusWnd = NULL;
		if (hNewFocusWnd != NULL)
		{
			pNewFocusWnd = CWnd::FromHandle(hNewFocusWnd);
		}

		CWnd* pRichEdit = GetDlgItem(IDC_RICHEDIT2_PROGRAM);
		BOOL bNeedHide = TRUE;

		// ???????????????§Ò????????????????????
		if (pNewFocusWnd != NULL)
		{
			if (pNewFocusWnd->GetSafeHwnd() == m_autoCompleteList.GetSafeHwnd() ||
				(pRichEdit && pNewFocusWnd->GetSafeHwnd() == pRichEdit->GetSafeHwnd()) ||
				pNewFocusWnd->GetSafeHwnd() == this->GetSafeHwnd())
			{
				bNeedHide = FALSE;
			}
		}

		if (bNeedHide)
		{
			HideAutoComplete();
		}
	}

	// ========== ??????????§Õ?????????? ==========
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_INSERT)
	{
		Run_once_mutex = true;
		CString temp_sel_cstring;
		if (Bacnet_Show_Debug(temp_sel_cstring) == 0)
		{
			CString temp_message;
			temp_message.Format(_T("A definition for the symbol '%s' could not be located."), temp_sel_cstring);
			MessageBox(temp_message, _T("Message"), MB_ICONWARNING);
		}
		Run_once_mutex = false;
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN)
	{
		if (GetFocus())
		{
			if (IDC_RICHEDIT2_PROGRAM == GetFocus()->GetDlgCtrlID())
			{
#if USE_PROMOT_PROGRAM
				CRichEditCtrl* pEditCtrl = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM);
				if (pEditCtrl)
				{
					long start, end;
					pEditCtrl->GetSel(start, end);
					CString input;
					pEditCtrl->GetTextRange(0, start, input);
					if (!input.IsEmpty())
					{
						CStringArray temp_array;
						SplitCStringA(temp_array, input, _T(" ")); //??????????????????????????????;
						if (temp_array.GetSize() > 0)
						{
							input = temp_array.GetAt(temp_array.GetSize() - 1);
						}
						else
						{

						}
					}
					// ?????????????
					input.Remove(_T('\n'));
					input.Remove(_T('\r'));
					input.Remove(_T(' '));
					TRACE(input + _T("\r\n"));
					((CListBox*)GetDlgItem(IDC_LIST_PROMPT))->ResetContent();
					((CListBox*)GetDlgItem(IDC_LIST_PROMPT))->AddString(input);
				}
#endif	

				// ????A?????????§Ó????????????????/??—¨?????????????
				// ?????????????????????OnSend / OnRenumber / OnProgramIdeSettings??

			}

		}


	}
	else if (pMsg->message == WM_RBUTTONDOWN)
	{
		if (GetFocus())
		{
			if (IDC_RICHEDIT2_PROGRAM == GetFocus()->GetDlgCtrlID())
			{
				//right_click_Point.x = pMsg->pt.x;
				//right_click_Point.y = pMsg->pt.y;
				//POINT lpPoint;
				//GetCursorPos(&lpPoint);
				//SetCursorPos(lpPoint.x , lpPoint.y);
				//mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
				//mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);

				//PostMessage(WM_RICHEDIT_RIGHT_CLICK,NULL,NULL);

				CString Select_string;
				Select_string = ((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetSelText();
				Select_string.Trim();

				if (Select_string.GetLength() != 0)
				{
					CMenu menu;
					menu.LoadMenu(IDR_MENU_PROGRAMEDIT_RIGHT_CLICK);
					CMenu* pPopup = menu.GetSubMenu(0);
					CPoint point;
					point.x = pMsg->pt.x;
					point.y = pMsg->pt.y;
					pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
				}
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
void CBacnetProgramEdit::OnPropertiesGotodefinition()
{

	PostMessage(WM_KEYDOWN, VK_INSERT, 1);
}

void CBacnetProgramEdit::OnBnClickedButtonProgramEditHelp()
{
	// TODO: ??????????????????????
	CString temp_demo_prog;
	temp_demo_prog = g_strExePth + _T("ResourceFile\\DemoPrg.txt");
	ShellExecute(this->m_hWnd, _T("open"), temp_demo_prog, NULL, NULL, SW_SHOWNORMAL);
	temp_demo_prog = g_strExePth + _T("T3000_Help.chm");
	ShellExecute(this->m_hWnd, _T("open"), temp_demo_prog, NULL, NULL, SW_SHOWNORMAL);
}

void CBacnetProgramEdit::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rc;
	GetClientRect(rc);
	//GetDlgItem(IDC_BUTTON_PROGRAM_EDIT)->MoveWindow(rc.left + 20, rc.bottom - 60, 120, 50);
	TRACE(_T("cx = %d , cy = %d\r\n"), cx, cy);
	if (m_information_window.m_hWnd != NULL)
	{
		((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->MoveWindow(rc.left, rc.top, rc.Width(), rc.Height() - 120);
		if (cy > 120)
			//m_information_window.MoveWindow(rc.left, cy - 120, rc.Width() - 580, 100);
			m_information_window.MoveWindow(rc.left, cy - 120, rc.Width() - 360, 120);
		CRect rcCtrl;
		GetDlgItem(IDC_BUTTON_PROGRAM_EDIT_HELP)->GetWindowRect(rcCtrl);
		ScreenToClient(rcCtrl);
#if USE_PROMOT_PROGRAM
		GetDlgItem(IDC_LIST_PROMPT)->SetWindowPos(NULL, cx - 570, cy - 120, 0, 0, SWP_NOSIZE);
#endif
		GetDlgItem(IDC_BUTTON_PROGRAM_EDIT_HELP)->SetWindowPos(NULL, cx - 350, cy - 100, 0, 0, SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_PRG_GROUP)->SetWindowPos(NULL, cx - 250, cy - 120, 0, 0, SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_POOL_1)->SetWindowPos(NULL, cx - 245, cy - 100, 0, 0, SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_SIZE_1)->SetWindowPos(NULL, cx - 245, cy - 75, 0, 0, SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_FREE_1)->SetWindowPos(NULL, cx - 245, cy - 50, 0, 0, SWP_NOSIZE);
		m_pool_size.SetWindowPos(NULL, cx - 140, cy - 100, 0, 0, SWP_NOSIZE);
		m_program_size.SetWindowPos(NULL, cx - 140, cy - 75, 0, 0, SWP_NOSIZE);
		m_free_memory.SetWindowPos(NULL, cx - 140, cy - 50, 0, 0, SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_POOL_2)->SetWindowPos(NULL, cx - 85, cy - 100, 0, 0, SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_SIZE_2)->SetWindowPos(NULL, cx - 85, cy - 75, 0, 0, SWP_NOSIZE);
		GetDlgItem(IDC_STATIC_FREE_2)->SetWindowPos(NULL, cx - 85, cy - 50, 0, 0, SWP_NOSIZE);
		if (cx < 1100)
		{
			//GetDlgItem(IDC_STATIC_PRG_GROUP)->ShowWindow(SW_HIDE);
			//GetDlgItem(IDC_STATIC_POOL_1)->ShowWindow(SW_HIDE);
			//GetDlgItem(IDC_STATIC_SIZE_1)->ShowWindow(SW_HIDE);
			//GetDlgItem(IDC_STATIC_FREE_1)->ShowWindow(SW_HIDE);
			//m_pool_size.ShowWindow(SW_HIDE);
			//m_program_size.ShowWindow(SW_HIDE);
			//m_free_memory.ShowWindow(SW_HIDE);
			//GetDlgItem(IDC_STATIC_POOL_2)->ShowWindow(SW_HIDE);
			//GetDlgItem(IDC_STATIC_SIZE_2)->ShowWindow(SW_HIDE);
			//GetDlgItem(IDC_STATIC_FREE_2)->ShowWindow(SW_HIDE);
		}

	}
	Invalidate(1);
	// TODO: ??????????????????????
}

void CBacnetProgramEdit::OnRenumber()
{
	// TODO: ????????????????????
	program_re_line_number = 1;
	Syntax_analysis();
	PostMessage(WM_REFRESH_BAC_PROGRAM_RICHEDIT, NULL, NULL);
	return;
}

void CBacnetProgramEdit::InitAutoCompleteKeywords()
{
	m_autoCompleteKeywords.clear();

	// 1. ?????????????
	AutoCompleteKeyword key;
	key.text = _T("AC1_L_IN");
	key.type = LIST_COLOR_TYPE_CUSTOM;
	BOOL bExists = FALSE;
	for (int j = 0; j < (int)m_autoCompleteKeywords.size(); j++)
	{
		if (m_autoCompleteKeywords[j].text.CompareNoCase(key.text) == 0)
		{
			bExists = TRUE;
			break;
		}
	}
	if (!bExists)
	{
		m_autoCompleteKeywords.push_back(key);
	}

	// 2. ???? func_table ???ï…???????§Ü????????????TYPE_FUNC??
	for (int i = 0; func_table[i].func_name[0] != '\0'; i++)
	{
		key.text = CA2T(func_table[i].func_name);
		key.text.Trim();
		key.type = LIST_COLOR_TYPE_FUNC;
		if (key.text.IsEmpty()) continue;

		bExists = FALSE;
		for (int j = 0; j < (int)m_autoCompleteKeywords.size(); j++)
		{
			if (m_autoCompleteKeywords[j].text.CompareNoCase(key.text) == 0)
			{
				bExists = TRUE;
				break;
			}
		}
		if (!bExists)
		{
			m_autoCompleteKeywords.push_back(key);
		}
	}

	// 3. ???? table ???ï…?????????????????????TYPE_COMMAND??
	for (int i = 0; table[i].command[0] != '\0'; i++)
	{
		key.text = CA2T(table[i].command);
		key.text.Trim();
		key.type = LIST_COLOR_TYPE_COMMAND;
		if (key.text.IsEmpty()) continue;

		bExists = FALSE;
		for (int j = 0; j < (int)m_autoCompleteKeywords.size(); j++)
		{
			if (m_autoCompleteKeywords[j].text.CompareNoCase(key.text) == 0)
			{
				bExists = TRUE;
				break;
			}
		}
		if (!bExists)
		{
			m_autoCompleteKeywords.push_back(key);
		}
	}

	// 4. ???? m_Input_data ???ï…????????????? label???????TYPE_INPUT??
	for (int i = 0; i < (int)m_Input_data.size(); i++)
	{
		key.text = CA2T((char*)m_Input_data[i].label);
		key.text.Trim();
		key.type = LIST_COLOR_TYPE_INPUT;
		if (!key.text.IsEmpty() && key.text.GetLength() > 3)
		{
			bExists = FALSE;
			for (int j = 0; j < (int)m_autoCompleteKeywords.size(); j++)
			{
				if (m_autoCompleteKeywords[j].text.CompareNoCase(key.text) == 0)
				{
					bExists = TRUE;
					break;
				}
			}
			if (!bExists)
			{
				m_autoCompleteKeywords.push_back(key);
			}
		}
	}

	// 5. ???? m_Output_data ???ï…????????????? label???????TYPE_OUTPUT??
	for (int i = 0; i < (int)m_Output_data.size(); i++)
	{
		key.text = CA2T((char*)m_Output_data[i].label);
		key.text.Trim();
		key.type = LIST_COLOR_TYPE_OUTPUT;
		if (!key.text.IsEmpty() && key.text.GetLength() > 3)
		{
			bExists = FALSE;
			for (int j = 0; j < (int)m_autoCompleteKeywords.size(); j++)
			{
				if (m_autoCompleteKeywords[j].text.CompareNoCase(key.text) == 0)
				{
					bExists = TRUE;
					break;
				}
			}
			if (!bExists)
			{
				m_autoCompleteKeywords.push_back(key);
			}
		}
	}

	// 6. ???? m_Variable_data ???ï…????????????? label???????TYPE_VARIABLE??
	for (int i = 0; i < (int)m_Variable_data.size(); i++)
	{
		key.text = CA2T((char*)m_Variable_data[i].label);
		key.text.Trim();
		key.type = LIST_COLOR_TYPE_VARIABLE;
		if (!key.text.IsEmpty() && key.text.GetLength() > 3)
		{
			bExists = FALSE;
			for (int j = 0; j < (int)m_autoCompleteKeywords.size(); j++)
			{
				if (m_autoCompleteKeywords[j].text.CompareNoCase(key.text) == 0)
				{
					bExists = TRUE;
					break;
				}
			}
			if (!bExists)
			{
				m_autoCompleteKeywords.push_back(key);
			}
		}
	}
}
void CBacnetProgramEdit::OnEnChangeRichedit2Program()
{
	// ???????????????????????????????????????????????????EN_CHANGE???
	if (m_bInAutoCompleteLogic) return;
	m_bInAutoCompleteLogic = true;

	// 1. ?????§¹???§µ??
	CRichEditCtrl* pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM);
	if (!pRichEdit || !IsWindow(pRichEdit->GetSafeHwnd()) || !IsWindow(GetSafeHwnd()))
	{
		HideAutoComplete();
		m_bInAutoCompleteLogic = false;
		return;
	}

	// 2. ??????¦Ë???????????????????????
	long nCurSelStart, nCurSelEnd;
	pRichEdit->GetSel(nCurSelStart, nCurSelEnd);
	if (nCurSelStart != nCurSelEnd)
	{
		HideAutoComplete();
		m_bInAutoCompleteLogic = false;
		return;
	}

	// 3. ???????????????????????????200????????????????????20???????????????????????????
	const int MAX_SCAN_LENGTH = 200;
	int nScanStart = max(0, nCurSelStart - MAX_SCAN_LENGTH);
	CString strScanText;
	pRichEdit->GetTextRange(nScanStart, nCurSelStart, strScanText); // ??????????§³??????????????

	// 4. ?????????????????????????\r??????\n????????????/?????????
	const CString strDelimiters = _T(" \r\n??");
	const int nScanTextLen = strScanText.GetLength();

	// 5. ???????????????????200???????????????
	int nWordStartInScan = nScanTextLen - 1;
	while (nWordStartInScan >= 0)
	{
		TCHAR ch = strScanText.GetAt(nWordStartInScan);
		if (strDelimiters.Find(ch) >= 0) break; // ???????????
		nWordStartInScan--;
	}
	nWordStartInScan++; // ????????????????????

	// 6. ????????????????§Ö????¦Ë??
	const int nWordLen = nScanTextLen - nWordStartInScan;
	m_nInputStartPos = nScanStart + nWordStartInScan;

	// 7. ???????<=0????????????????????????????????
	if (nWordLen <= 0 || m_nInputStartPos < 0 || (m_nInputStartPos + nWordLen) > nCurSelStart)
	{
		HideAutoComplete();
		m_bInAutoCompleteLogic = false;
		return;
	}

	// 8. ??????????????
	m_strCurrentInput = strScanText.Mid(nWordStartInScan, nWordLen);
	if (m_strCurrentInput.IsEmpty() || m_strCurrentInput.GetLength() < 2)
	{
		HideAutoComplete();
		m_bInAutoCompleteLogic = false;
		return;
	}

	// 9. ????????
	MatchAutoCompleteKeywords(m_strCurrentInput);

	// 10. ???/???????
	if (m_autoCompleteList.GetCount() > 0)
	{
		// ????????RichEdit????????????????
		CPoint ptCursor = pRichEdit->GetCharPos(nCurSelStart);
		pRichEdit->ClientToScreen(&ptCursor); // ?????????
		ShowAutoComplete(ptCursor);
	}
	else
	{
		HideAutoComplete();
	}

	// ?????????
	m_bInAutoCompleteLogic = false;
}
void CBacnetProgramEdit::MatchAutoCompleteKeywords(CString strInput)
{
	m_autoCompleteList.ResetContent();
	if (strInput.IsEmpty() || strInput.GetLength() < 2) return;

	// ????§Õ??????????§Õ????????????§³§Õ???
	CString strInputUpper = strInput;
	strInputUpper.MakeUpper();
	const int nInputLen = strInputUpper.GetLength();
	int nMatchCount = 0;

	// ????????????????
	const int nTotalKeywords = (int)m_autoCompleteKeywords.size();
	for (int i = 0; i < nTotalKeywords; i++)
	{
		CString strKeyword = m_autoCompleteKeywords[i].text;
		if (strKeyword.GetLength() < nInputLen) continue;

		// ?????
		if (strKeyword.Left(nInputLen).CompareNoCase(strInputUpper) == 0)
		{
			// ????????????§Ò?
			int nIndex = m_autoCompleteList.AddString(strKeyword);
			// ???????????????§Ò???????????
			m_autoCompleteList.SetItemData(nIndex, m_autoCompleteKeywords[i].type);
			nMatchCount++;
			if (nMatchCount >= MAX_AUTOCOMPLETE_COUNT) break;
		}
	}

	// ?????§Ö???????
	if (m_autoCompleteList.GetCount() > 0)
	{
		m_autoCompleteList.SetCurSel(0);
	}
}
void CBacnetProgramEdit::ShowAutoComplete(CPoint point)
{
	// 1. ?????§¹??§µ??
	int nItemCount = m_autoCompleteList.GetCount();
	if (nItemCount <= 0 || !IsWindow(m_autoCompleteList.GetSafeHwnd()) || !IsWindow(GetSafeHwnd()))
	{
		HideAutoComplete();
		return;
	}

	// 2. ?????§Ò????
	const int nItemHeight = m_autoCompleteList.GetItemHeight(0);
	const int nListWidth = 350;
	int nListHeight = nItemCount * nItemHeight + 4;
	if (nListHeight > 400) nListHeight = 400; // ?????????

	// 3. ?????????????????????
	ScreenToClient(&point);

	// 4. ???????????????????????????????????????
	CRect rcDialogClient;
	GetClientRect(&rcDialogClient);
	rcDialogClient.bottom -= 130;

	// 5. ???????????????????????????????????????????????????????????????
	int nShowTop = point.y + 20;
	// ????????????????
	if (nShowTop + nListHeight > rcDialogClient.bottom)
	{
		nShowTop = point.y - nListHeight - 5;
	}
	// ???????????????????????
	if (nShowTop < rcDialogClient.top)
	{
		nShowTop = rcDialogClient.top + 5;
	}

	// 6. ??????????????????????
	int nShowLeft = point.x;
	if (nShowLeft + nListWidth > rcDialogClient.right)
	{
		nShowLeft = rcDialogClient.right - nListWidth - 5;
	}
	if (nShowLeft < rcDialogClient.left)
	{
		nShowLeft = rcDialogClient.left + 5;
	}

	// 7. ?????§Ò?????¦Ë??
	CRect rcList(nShowLeft, nShowTop, nShowLeft + nListWidth, nShowTop + nListHeight);
	m_autoCompleteList.MoveWindow(rcList);

	// 8. ???????????????????
	if (!m_bAutoCompleteVisible)
	{
		m_autoCompleteList.SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		m_autoCompleteList.ShowWindow(SW_SHOWNOACTIVATE);
		m_bAutoCompleteVisible = true;
	}
}

void CBacnetProgramEdit::HideAutoComplete()
{
	// ?§µ?ï…???????????????
	if (!IsWindow(GetSafeHwnd()))
	{
		m_bAutoCompleteVisible = false;
		return;
	}
	if (!IsWindow(m_autoCompleteList.GetSafeHwnd()))
	{
		m_bAutoCompleteVisible = false;
		return;
	}
	// ???????????????????
	m_autoCompleteList.ShowWindow(SW_HIDE);
	m_bAutoCompleteVisible = false;
}

void CBacnetProgramEdit::OnAutoCompleteSelect()
{
	int nCurSel = m_autoCompleteList.GetCurSel();
	if (nCurSel < 0) return;

	// ?????§Ö?????
	CString strSelected;
	m_autoCompleteList.GetText(nCurSel, strSelected);

	CRichEditCtrl* pRichEdit = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM);
	if (!pRichEdit || !IsWindow(pRichEdit->GetSafeHwnd())) return;

	// ?I?????????????????
	long nCurSelEnd;
	pRichEdit->GetSel(nCurSelEnd, nCurSelEnd);
	pRichEdit->SetSel(m_nInputStartPos, nCurSelEnd);
	pRichEdit->ReplaceSel(strSelected);

	// ??????????????¦Â
	long nNewPos = m_nInputStartPos + strSelected.GetLength();
	pRichEdit->SetSel(nNewPos, nNewPos);

	// ?????????
	HideAutoComplete();
}

void CBacnetProgramEdit::OnLbnDblclkAutocompleteList()
{
	OnAutoCompleteSelect();
}

void CBacnetProgramEdit::OnLbnSelchangeAutocompleteList()
{
	// ??§Ò£??????????????????????
}

void CBacnetProgramEdit::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// ??????????§Ò??????
	if (nIDCtl != IDC_AUTOCOMPLETE_LIST)
	{
		CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
		return;
	}

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	int nItem = lpDrawItemStruct->itemID;
	UINT nState = lpDrawItemStruct->itemState;
	CRect rcItem = lpDrawItemStruct->rcItem;

	// 1. ???????
	COLORREF crBkColor = RGB(255, 255, 255); // ?????????
	if (nState & ODS_SELECTED)
	{
		crBkColor = GetSysColor(COLOR_HIGHLIGHT); // ?????????????
	}
	pDC->FillSolidRect(rcItem, crBkColor);

	// 2. ???????
	if (nItem >= 0 && nItem < m_autoCompleteList.GetCount())
	{
		CString strText;
		m_autoCompleteList.GetText(nItem, strText);
		KeywordType type = (KeywordType)m_autoCompleteList.GetItemData(nItem);

		// ??????????????????????????????????????
		COLORREF crTextColor = prg_text_color; // ?????
		switch (type)
		{
		case LIST_COLOR_TYPE_FUNC:
			crTextColor = prg_function_color; // ?????????
			break;
		case LIST_COLOR_TYPE_COMMAND:
			crTextColor = prg_command_color; // ???????
			break;
		case LIST_COLOR_TYPE_INPUT:
			crTextColor = RGB(128, 0, 128); // ????????
			break;
		case LIST_COLOR_TYPE_OUTPUT:
			crTextColor = RGB(255, 128, 0); // ????????
			break;
		case LIST_COLOR_TYPE_VARIABLE:
			crTextColor = RGB(128, 64, 0); // ?????????
			break;
		}

		// ???????????
		if (nState & ODS_SELECTED)
		{
			crTextColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
		}

		// ????????????
		pDC->SelectObject(&m_autoCompleteFont);
		pDC->SetTextColor(crTextColor);
		pDC->SetBkMode(TRANSPARENT);

		// ????????????????????§µ?
		CRect rcText = rcItem;
		rcText.left += 5; // ????
		pDC->DrawText(strText, rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}

	// 3. ????????
	if (nState & ODS_FOCUS)
	{
		pDC->DrawFocusRect(rcItem);
	}
}

void CBacnetProgramEdit::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// ??????????§Ò?
	if (nIDCtl == IDC_AUTOCOMPLETE_LIST)
	{
		lpMeasureItemStruct->itemHeight = 20; // ?????????ItemHeight???
	}
	else
	{
		CDialogEx::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
	}
}
