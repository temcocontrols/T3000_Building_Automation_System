#pragma once

#include "DownloadSocket.h"
#include "afxwin.h"
// Dowmloadfile dialog




class Dowmloadfile : public CDialogEx
{
	DECLARE_DYNAMIC(Dowmloadfile)

public:
	Dowmloadfile(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dowmloadfile();

// Dialog Data
	enum { IDD = IDD_DIALOG_DOWNLOAD_FILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DownloadSocket TCP_File_Socket;
	afx_msg void OnBnClickedButtonStartDownload();
	afx_msg LRESULT DownloadFileMessage(WPARAM wParam,LPARAM lParam);
	CListBox m_download_info;
	static DWORD WINAPI   DownLoadFileProcess(LPVOID lpVoid);
};
