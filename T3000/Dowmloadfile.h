#pragma once

#include "DownloadSocket.h"
#include "afxwin.h"
// Dowmloadfile dialog
#include "CM5/CStatic/staticex.h"



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
	static DWORD WINAPI  isp_thread(LPVOID lpVoid);
	afx_msg void OnClose();
	virtual void OnCancel();
	BOOL IsNetDevice(int DevType);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonFileDownloadOnly();
	void Start_Download();
    void Start_Download_Ftp();
	unsigned char m_download_product_type;
	afx_msg void OnBnClickedButtonUpdateT3000();
	void AutoFlashFirmware();
	void CreateProductFolderMap();
	CString GetProdcutFtpPath(int nproductid);
    static DWORD WINAPI  FtpDownloadThread(LPVOID lpVoid);
    CStaticEx m_static_persent;
    CString download_file_name;
    void Update_File();
};


