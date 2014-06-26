// BacnetProgramEdit.cpp : implementation file
// This File coded by Fance used to encode and decode programming.20130820

#include "stdafx.h"
#include "T3000.h"
#include "BacnetProgramEdit.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"
#include "gloab_define.h"
extern int error;
extern char *pmes;
//extern int program_list_line;

// CBacnetProgramEdit dialog
CString program_string;
 char editbuf[25000];
 extern char my_display[10240];
 extern int Encode_Program();
 extern int my_lengthcode;
 extern char mycode[1024];

extern void  init_info_table( void );
extern void Init_table_bank();
extern char mesbuf[1024];
extern int renumvar;

extern char *desassembler_program();
extern void copy_data_to_ptrpanel(int Data_type);
HWND mParent_Hwnd;

IMPLEMENT_DYNAMIC(CBacnetProgramEdit, CDialogEx)

CBacnetProgramEdit::CBacnetProgramEdit(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetProgramEdit::IDD, pParent)
{
	Run_once_mutex = false;
}

CBacnetProgramEdit::~CBacnetProgramEdit()
{
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
	ON_MESSAGE(WM_HOTKEY,&CBacnetProgramEdit::OnHotKey)//快捷键消息映射手动加入
	ON_MESSAGE(WM_REFRESH_BAC_PROGRAM_RICHEDIT,Fresh_Program_RichEdit)
	ON_MESSAGE(MY_RESUME_DATA, ProgramResumeMessageCallBack)
	ON_COMMAND(ID_SEND, &CBacnetProgramEdit::OnSend)
	ON_WM_CLOSE()
	ON_COMMAND(ID_CLEAR, &CBacnetProgramEdit::OnClear)
	ON_COMMAND(ID_LOADFILE, &CBacnetProgramEdit::OnLoadfile)
	ON_COMMAND(ID_SAVEFILE, &CBacnetProgramEdit::OnSavefile)
	ON_EN_SETFOCUS(IDC_RICHEDIT2_PROGRAM, &CBacnetProgramEdit::OnEnSetfocusRichedit2Program)
	ON_COMMAND(ID_REFRESH, &CBacnetProgramEdit::OnRefresh)
END_MESSAGE_MAP()


// CBacnetProgramEdit message handlers
LRESULT CBacnetProgramEdit::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if(Run_once_mutex == true)	//If already exist some function run,it can't show another one;
		return -1;
	if (wParam==KEY_F2)
	{
		Run_once_mutex = true;
		OnSend();
		Run_once_mutex = false;
	}
	else if(wParam == KEY_F3)
	{
		Run_once_mutex = true;
		OnClear();
		Run_once_mutex = false;
	}
	else if(wParam == KEY_F7)
	{
		Run_once_mutex = true;
		OnLoadfile();
		Run_once_mutex = false;
	}
	else if(wParam == KEY_F6)
	{
		Run_once_mutex = true;
		OnSavefile();
		Run_once_mutex = false;
	}
	else if(wParam == KEY_F8)
	{
		Run_once_mutex = true;
		OnRefresh();
		Run_once_mutex = false;
	}
	return 0;
}

void CBacnetProgramEdit::Initial_static()
{
	CString temp_cs_size,temp_cs_free;
	m_pool_size.SetWindowTextW(_T("26624"));
	m_pool_size.textColor(RGB(255,0,0));
	//m_static.bkColor(RGB(0,255,255));
	m_pool_size.setFont(15,10,NULL,_T("Arial"));

	temp_cs_size.Format(_T("%d"),bac_program_size);
	m_program_size.SetWindowTextW(temp_cs_size);
	m_program_size.textColor(RGB(255,0,0));
	//m_static.bkColor(RGB(0,255,255));
	m_program_size.setFont(15,10,NULL,_T("Arial"));

	temp_cs_free.Format(_T("%d"),bac_free_memory);
	m_free_memory.SetWindowTextW(temp_cs_free);
	m_free_memory.textColor(RGB(255,0,0));
	//m_static.bkColor(RGB(0,255,255));
	m_free_memory.setFont(15,10,NULL,_T("Arial"));
}
extern char my_panel;
BOOL CBacnetProgramEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_edit_changed = false;
	my_panel = bac_gloab_panel; //Set the panel number
	GetDlgItem(IDC_RICHEDIT2_PROGRAM)->SetFocus();

	CHARFORMAT cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask|=CFM_BOLD;

	cf.dwEffects&=~CFE_BOLD;
	//cf.dwEffects|=~CFE_BOLD; //粗体，取消用cf.dwEffects&=~CFE_BOLD;
	cf.dwMask|=CFM_ITALIC;
	cf.dwEffects&=~CFE_ITALIC;
	//cf.dwEffects|=~CFE_ITALIC; //斜体，取消用cf.dwEffects&=~CFE_ITALIC;
	cf.dwMask|=CFM_UNDERLINE;
	cf.dwEffects&=~CFE_UNDERLINE;
	//cf.dwEffects|=~CFE_UNDERLINE; //斜体，取消用cf.dwEffects&=~CFE_UNDERLINE;
	cf.dwMask|=CFM_COLOR;
	cf.crTextColor = RGB(0,0,255); //设置颜色
	cf.dwMask|=CFM_SIZE;
	cf.yHeight =300; //设置高度
	cf.dwMask|=CFM_FACE;
	//_tcscpy(cf.szFaceName ,_T("SimSun-ExtB"));
	//_tcscpy(cf.szFaceName ,_T("Times New Roman"));
	//	strcpy(cf.szFaceName ,_T("隶书")); //设置字体
	_tcscpy(cf.szFaceName ,_T("Arial"));
	
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSelectionCharFormat(cf);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetDefaultCharFormat(cf); 


	//((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->PasteSpecial(CF_TEXT);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->PostMessage(WM_VSCROLL, SB_BOTTOM,0);
	//int retry_count =0;
	//do 
	//{
	//	retry_count ++;
	//	g_invoke_id = GetPrivateData(g_bac_instance,READPROGRAMCODE_T3000,program_list_line,program_list_line,100);
	//	Sleep(200);
	//} while ((retry_count<10)&&(g_invoke_id<0));
	
	//if(g_invoke_id>=0)
	//{
	//	CString temp_cs_show;
	//	temp_cs_show.Format(_T("Task ID = %d. Read program code from item %d "),g_invoke_id,program_list_line);
	//	Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd,temp_cs_show);
	//}
		//Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);

	RegisterHotKey(GetSafeHwnd(),KEY_F2,NULL,VK_F2);//F2键
	RegisterHotKey(GetSafeHwnd(),KEY_F3,NULL,VK_F3);
	RegisterHotKey(GetSafeHwnd(),KEY_F7,NULL,VK_F7);
	RegisterHotKey(GetSafeHwnd(),KEY_F6,NULL,VK_F6);
	RegisterHotKey(GetSafeHwnd(),KEY_F8,NULL,VK_F8);
	Initial_static();

	init_info_table();
	Init_table_bank();

	mParent_Hwnd = g_hwnd_now;

	m_program_edit_hwnd = this->m_hWnd;
	g_hwnd_now = m_program_edit_hwnd;

	copy_data_to_ptrpanel(TYPE_ALL);
	memset(my_display,0,sizeof(my_display));
	PostMessage(WM_REFRESH_BAC_PROGRAM_RICHEDIT,NULL,NULL);
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
LRESULT CBacnetProgramEdit::Fresh_Program_RichEdit(WPARAM wParam,LPARAM lParam)
{
	//return 0;
	//	CString temp2;
	char * temp_point;
 	temp_point = desassembler_program();
	if(temp_point == NULL)
	{
		MessageBox(_T("ERROR!!!!Decode Error!!!!!!!!!!!!!"));
		
		return 1;
	}
		
	CString temp;


	int  len = 0;
	len =  strlen(my_display); //str.length();
	int  unicodeLen = ::MultiByteToWideChar( CP_ACP,0, my_display,-1,NULL,0 );  
	::MultiByteToWideChar( CP_ACP,  0,my_display,-1,temp.GetBuffer(unicodeLen),unicodeLen ); 
	temp.ReleaseBuffer();
	CString temp1 = temp;
	

	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetWindowTextW(temp1);
	m_edit_changed = false;
	program_string = temp1;
	//((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(-1,-1);
	//((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->ReplaceSel(temp1);
	//((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(-1,-1);
	return 0;
}

void CBacnetProgramEdit::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetFocus();

	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(-1,-1);
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->ReplaceSel(_T("\n"));
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetFocus();

	/*CDialogEx::OnOK();*/
}


LRESULT  CBacnetProgramEdit::ProgramResumeMessageCallBack(WPARAM wParam, LPARAM lParam)
{
	_MessageInvokeIDInfo *pInvoke =(_MessageInvokeIDInfo *)lParam;
	CString temp_cs = pInvoke->task_info;
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	CString Show_Results;
	if(msg_result)
	{
		//CString temp_ok;
		//temp_ok = _T("Bacnet operation success!   Request ID:") +  temp_cs;
		m_Program_data.at(program_list_line).bytes =	program_code_length[program_list_line] - PRIVATE_HEAD_LENGTH;

		Show_Results = temp_cs + _T("Success!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);


		CString Edit_Buffer;
		GetDlgItemText(IDC_RICHEDIT2_PROGRAM,Edit_Buffer);
		program_string = Edit_Buffer;
		MessageBox(Show_Results);
//#endif
	}
	else
	{
		Show_Results = temp_cs + _T("Fail!");
		SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
//#ifdef SHOW_ERROR_MESSAGE
		MessageBox(Show_Results);
//#endif
	}
	if(pInvoke)
		delete pInvoke;
	return 0;
}


void CBacnetProgramEdit::OnSend()
{
	// TODO: Add your command handler code here
	renumvar = 1;
	error = -1; //Default no error;
	CString tempcs;
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetWindowTextW(tempcs);

//	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_ACP,0,tempcs,-1,NULL,0,NULL,NULL );
	memset( ( void* )editbuf, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_ACP,0,tempcs,-1,editbuf,iTextLen,NULL,NULL );
	Encode_Program();
	if(error == -1)
	{
		TRACE(_T("Encode_Program length is %d ,copy length is %d\r\n"),program_code_length[program_list_line],my_lengthcode + 10);
		if(my_lengthcode <400)
			memcpy_s(program_code[program_list_line],my_lengthcode,mycode,my_lengthcode);
		else
		{
			CString temp_mycs;//如果npdu的长度过长，大于 400了，目前是通过换一个page来存;
			temp_mycs.Format(_T("Encode Program Code Length is %d,Please Try to code in another page."),my_lengthcode);
			MessageBox(temp_mycs);
			memset(program_code[program_list_line],0,400);
			return;
		}


		g_invoke_id =WritePrivateData(g_bac_instance,WRITEPROGRAMCODE_T3000,program_list_line,program_list_line/*,my_lengthcode*/);
		if(g_invoke_id>=0)
		{
			CString temp_cs_show;
			temp_cs_show.Format(_T("Write program code to item %d "),program_list_line);
			Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd/*BacNet_hwd*/,temp_cs_show);
		}
		m_information_window.ResetContent();
		m_information_window.InsertString(0,_T("Resource Compile succeeded."));

	}
	else
	{
		m_information_window.ResetContent();
		CString cstring_error;

		int len = strlen(mesbuf);
		int  unicodeLen = ::MultiByteToWideChar( CP_ACP,0, mesbuf,-1,NULL,0 ); 
		::MultiByteToWideChar( CP_ACP,  0,mesbuf,-1,cstring_error.GetBuffer(MAX_PATH),unicodeLen );  
		cstring_error.ReleaseBuffer();


//		MessageBox(cstring_error);
		CStringArray  Error_info;  
		SplitCStringA(Error_info,cstring_error,_T("\r\n"));//Split the CString with "\r\n" and then add to the list.(Fance)
		Sleep(1);
		m_information_window.ResetContent();
		for (int i=0;i<(int)Error_info.GetSize();i++)
		{
			m_information_window.InsertString(i,Error_info.GetAt(i));
		}

		MessageBox(_T("Errors,program NOT Sent!"));
	}

}





void CBacnetProgramEdit::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	UnregisterHotKey(GetSafeHwnd(),KEY_F2);//注销F2键
	UnregisterHotKey(GetSafeHwnd(),KEY_F3);
	UnregisterHotKey(GetSafeHwnd(),KEY_F7);
	UnregisterHotKey(GetSafeHwnd(),KEY_F6);
	UnregisterHotKey(GetSafeHwnd(),KEY_F8);

	g_hwnd_now = mParent_Hwnd;
	if(m_pragram_dlg_hwnd!=NULL)
		::PostMessage(m_pragram_dlg_hwnd,WM_REFRESH_BAC_PROGRAM_LIST,NULL,NULL);
	CDialogEx::OnClose();
}


void CBacnetProgramEdit::OnClear()
{
	// TODO: Add your command handler code here
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetWindowTextW(_T(""));
}


void CBacnetProgramEdit::OnLoadfile()
{
	// TODO: Add your command handler code here
	CString FilePath;
	CString ReadBuffer;

	CFileDialog dlg(true,_T("*.txt"),_T(" "),OFN_HIDEREADONLY ,_T("txt files (*.txt)|*.txt|All Files (*.*)|*.*||"),NULL,0);
	if(IDOK==dlg.DoModal())
	{
		FilePath=dlg.GetPathName();

		//Write_Position = pParent->myapp_path + cs_file_time;
		char *pBuf;
		DWORD dwFileLen;
		
		
		CFile file(FilePath,CFile::modeCreate |CFile::modeReadWrite |CFile::modeNoTruncate);

		dwFileLen=(DWORD)file.GetLength();
		pBuf= new char[dwFileLen+1];
		pBuf[dwFileLen]=0;

		file.SeekToBegin();
		file.Read(pBuf,dwFileLen);
		file.Close();

		CString ReadBuffer;

		int  len = 0;
		len = strlen(pBuf);
		int  unicodeLen = ::MultiByteToWideChar( CP_ACP,0, pBuf,-1,NULL,0 );  
		::MultiByteToWideChar( CP_ACP,  0,pBuf,-1,ReadBuffer.GetBuffer(unicodeLen),unicodeLen );  
		ReadBuffer.ReleaseBuffer();
		
		delete[] pBuf;
		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetWindowTextW(ReadBuffer);
		((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(unicodeLen,unicodeLen);

	}
}


void CBacnetProgramEdit::OnSavefile()
{
	// TODO: Add your command handler code here
	CFileDialog dlg(false,_T("*.txt"),_T(" "),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("txt files (*.txt)|*.txt|All Files (*.*)|*.*||"),NULL,0);
	if(IDOK==dlg.DoModal())
	{
		CString Write_Buffer;
		CString FilePath;
		GetDlgItemText(IDC_RICHEDIT2_PROGRAM,Write_Buffer);
		//char *readytowrite = 
		char*     readytowrite;
		int    iTextLen;
		iTextLen = WideCharToMultiByte( CP_ACP,0,Write_Buffer,-1,NULL,0,NULL,NULL );
		readytowrite = new char[iTextLen + 1];
		memset( ( void* )readytowrite, 0, sizeof( char ) * ( iTextLen + 1 ) );
		::WideCharToMultiByte( CP_ACP,0,Write_Buffer,-1,readytowrite,iTextLen,NULL,NULL );

		FilePath=dlg.GetPathName();

		CFile file(FilePath,CFile::modeCreate |CFile::modeReadWrite |CFile::modeNoTruncate);
		file.SeekToEnd();
		int write_length = strlen(readytowrite);
		file.Write(readytowrite,write_length + 1);
		file.Flush();
		file.Close();
		delete[] readytowrite;
	}
	
}


void CBacnetProgramEdit::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	
	CString Edit_Buffer;
	GetDlgItemText(IDC_RICHEDIT2_PROGRAM,Edit_Buffer);
	if(program_string.CompareNoCase(Edit_Buffer) == 0)
	{
		CDialogEx::OnCancel();
	}
	else
	{
		if(MessageBox(_T("Do you want to exit the programming without saving?"),_T("Prompting"),MB_ICONINFORMATION | MB_YESNO) == IDYES)
			CDialogEx::OnCancel();
	}

}

//Fance 
//This part of code is use to make sure when the rich edit loss focus,and get focus later,it not select none of the character
//Move the mouse point to the end of the text.
void CBacnetProgramEdit::OnEnSetfocusRichedit2Program()
{
	// TODO: Add your control notification handler code here
	CString temp;
	GetDlgItemText(IDC_RICHEDIT2_PROGRAM,temp);
	int length = temp.GetLength();
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(length,length);
}


void CBacnetProgramEdit::OnRefresh()
{
	// TODO: Add your command handler code here
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetWindowTextW(_T(""));
	if(m_Program_data.at(program_list_line).bytes <450)
		g_invoke_id = GetPrivateData(g_bac_instance,READPROGRAMCODE_T3000,program_list_line,program_list_line,m_Program_data.at(program_list_line).bytes + 10);
	else
	{
		g_invoke_id = GetPrivateData(g_bac_instance,READPROGRAMCODE_T3000,program_list_line,program_list_line, 10);
	}
	if(g_invoke_id>=0)
	{
		CString temp_cs_show;
		temp_cs_show.Format(_T("Task ID = %d. Read program code from item %d "),g_invoke_id,program_list_line);
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,BacNet_hwd,temp_cs_show);
	}
}
