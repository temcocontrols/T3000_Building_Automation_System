// BacnetProgramEdit.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetProgramEdit.h"
#include "afxdialogex.h"

#include "CM5/ud_str.h"
#include "Bacnet_Include.h"
#include "globle_function.h"

extern int program_list_line;

// CBacnetProgramEdit dialog
 char editbuf[25000];
 extern char my_display[1024];
 extern int Encode_Program();
 extern int my_lengthcode;

extern void  init_info_table( void );
extern void Init_table_bank();

extern int renumvar;

extern char *desassembler_program();

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
}


BEGIN_MESSAGE_MAP(CBacnetProgramEdit, CDialogEx)
	ON_MESSAGE(WM_HOTKEY,&CBacnetProgramEdit::OnHotKey)//快捷键消息映射手动加入
	ON_MESSAGE(WM_REFRESH_BAC_PROGRAM_RICHEDIT,Fresh_Program_RichEdit)
	ON_MESSAGE(MY_RESUME_DATA, ProgramResumeMessageCallBack)
//	ON_BN_CLICKED(IDC_BUTTON_PROGRAM_SEND, &CBacnetProgramEdit::OnBnClickedButtonProgramSend)
	ON_COMMAND(ID_SEND, &CBacnetProgramEdit::OnSend)
	ON_WM_CLOSE()
	ON_COMMAND(ID_CLEAR, &CBacnetProgramEdit::OnClear)
	ON_COMMAND(ID_LOADFILE, &CBacnetProgramEdit::OnLoadfile)
	ON_COMMAND(ID_SAVEFILE, &CBacnetProgramEdit::OnSavefile)
	ON_EN_SETFOCUS(IDC_RICHEDIT2_PROGRAM, &CBacnetProgramEdit::OnEnSetfocusRichedit2Program)
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
	else if(wParam == KEY_F5)
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
	return 0;
}


BOOL CBacnetProgramEdit::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

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
	_tcscpy(cf.szFaceName ,_T("SimSun-ExtB"));
	//	strcpy(cf.szFaceName ,_T("隶书")); //设置字体
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSelectionCharFormat(cf);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetDefaultCharFormat(cf); 


	//((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->PasteSpecial(CF_TEXT);
	((CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->PostMessage(WM_VSCROLL, SB_BOTTOM,0);

	g_invoke_id = GetPrivateData(1234,READPROGRAMCODE_T3000,program_list_line,program_list_line,100);
	if(g_invoke_id>=0)
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);

	RegisterHotKey(GetSafeHwnd(),KEY_F2,NULL,VK_F2);//F2键
	RegisterHotKey(GetSafeHwnd(),KEY_F3,NULL,VK_F3);
	RegisterHotKey(GetSafeHwnd(),KEY_F5,NULL,VK_F5);
	RegisterHotKey(GetSafeHwnd(),KEY_F6,NULL,VK_F6);

	init_info_table();
	Init_table_bank();
	m_program_edit_hwnd = this->m_hWnd;
	g_hwnd_now = m_program_edit_hwnd;
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
LRESULT CBacnetProgramEdit::Fresh_Program_RichEdit(WPARAM wParam,LPARAM lParam)
{
	char * temp_point;
	temp_point = desassembler_program();
	if(temp_point == NULL)
		return 1;
	CString temp;


	int  len = 0;
	len =  strlen(my_display); //str.length();
	int  unicodeLen = ::MultiByteToWideChar( CP_ACP,0, my_display,-1,NULL,0 );  
	::MultiByteToWideChar( CP_ACP,  0,my_display,-1,temp.GetBuffer(MAX_PATH),unicodeLen ); 
	temp.ReleaseBuffer();
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(-1,-1);
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->ReplaceSel(temp);
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->SetSel(-1,-1);
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
	bool msg_result=WRITE_FAIL;
	msg_result = MKBOOL(wParam);
	if(msg_result)
	{
		SetPaneString(0,_T("Bacnet operation success!"));
		MessageBox(_T("Bacnet operation success!"));
	}
	else
	{
		SetPaneString(0,_T("Bacnet operation fail!"));
		MessageBox(_T("Bacnet operation fail!"));
	}
	if(pInvoke)
		delete pInvoke;
	return 0;
}


void CBacnetProgramEdit::OnSend()
{
	// TODO: Add your command handler code here
	renumvar = 1;

	CString tempcs;
	((CRichEditCtrl *)GetDlgItem(IDC_RICHEDIT2_PROGRAM))->GetWindowTextW(tempcs);

	char*     pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_ACP,0,tempcs,-1,NULL,0,NULL,NULL );
	memset( ( void* )editbuf, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_ACP,0,tempcs,-1,editbuf,iTextLen,NULL,NULL );
	Encode_Program();


	g_invoke_id =WritePrivateData(1234,WRITEPROGRAMCODE_T3000,0,0,my_lengthcode);
	if(g_invoke_id>=0)
		Post_Invoke_ID_Monitor_Thread(MY_INVOKE_ID,g_invoke_id,this->m_hWnd);


}


void CBacnetProgramEdit::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	UnregisterHotKey(GetSafeHwnd(),KEY_F2);//注销F2键
	UnregisterHotKey(GetSafeHwnd(),KEY_F3);
	UnregisterHotKey(GetSafeHwnd(),KEY_F5);
	UnregisterHotKey(GetSafeHwnd(),KEY_F6);
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
	DWORD dwFileLen;
	CFileDialog dlg(true,_T("*.txt"),_T(" "),OFN_HIDEREADONLY ,_T("txt files (*.txt)|*.txt|All Files (*.*)|*.*||"),NULL,0);
	if(IDOK==dlg.DoModal())
	{
		FilePath=dlg.GetPathName();

		//Write_Position = pParent->myapp_path + cs_file_time;
		char *pBuf;
		DWORD dwFileLen;
		
		
		CFile file(FilePath,CFile::modeCreate |CFile::modeReadWrite |CFile::modeNoTruncate);

		dwFileLen=file.GetLength();
		pBuf= new char[dwFileLen+1];
		pBuf[dwFileLen]=0;

		file.SeekToBegin();
		file.Read(pBuf,dwFileLen);
		file.Close();


		CString ReadBuffer;

		int  len = 0;
		len = strlen(pBuf);
		int  unicodeLen = ::MultiByteToWideChar( CP_ACP,0, pBuf,-1,NULL,0 );  
		::MultiByteToWideChar( CP_ACP,  0,pBuf,-1,ReadBuffer.GetBuffer(MAX_PATH),unicodeLen );  
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
	if(MessageBox(_T("Do you want to exit the programming?"),_T("Prompting"),MB_ICONINFORMATION | MB_YESNO) == IDYES)
		CDialogEx::OnCancel();
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
