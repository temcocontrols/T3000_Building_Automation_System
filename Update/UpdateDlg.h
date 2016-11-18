
// UpdateDlg.h : header file
//

#pragma once
#include "..\t3000/CM5/CStatic/staticex.h"
#include "afxwin.h"
#include "afxcmn.h"

// CUpdateDlg dialog
class CUpdateDlg : public CDialogEx
{
// Construction
public:
	CUpdateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_UPDATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
public:
	HBITMAP hBitmap;
	void InitialStatic();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStaticEx m_static_download;
	CStaticEx m_static_uncompress;
	CStaticEx m_static_install;
	CStaticEx m_static_finish;
	CStatic m_static_pic_step;
	CStaticEx m_static_persent;
	CProgressCtrl m_progress;
	CStaticEx m_static_checkupdate;
	CStaticEx m_static_cs_info;
	afx_msg void OnBnClickedButtonCancel();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void InitialTemcoLogo();
	bool DeleteDirectory( CString DirName);
	BOOL CUpdateDlg::DownloadFileHttp(const CString& strFileURLInServer, //待下载文件的URL
		const CString & strFileLocalFullPath);//存放到本地的路径
	afx_msg void OnBnClickedCheckOpenT3000();
};

#define UPDATE_STEP_IDLE		 0

#define UPDATE_STEP_CHECK_NEWVERSION 1
#define UPDATE_STEP_DOWNLOAD     2
#define UPDATE_STEP_UNCOMPRESS	 3
#define UPDATE_STEP_INSTALL		 4
#define UPDATE_STEP_DONE		 5
#define UPDATE_STEP_READY_TO_CLOASE		 255