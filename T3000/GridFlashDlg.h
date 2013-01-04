#include "msflexgrid1.h"
#include "afxwin.h"
#include "DialogInfo.h"
#pragma once


// CGridFlashDlg dialog
typedef struct _grid_flash
{
	int ID;//tstat ID
	unsigned int serialnumber;	//the serialnumber
	int baudrate;//9600 or 19200
	short hardware_revisin;//6 register of tstat
	float software_version;//7 register of tstat
	CString hex_file_path;//the hex file path
	bool flash_or_no;//selected
	bool save_settings;
	unsigned short device;  //product model

	int EpSize;
	int ISPversion;
}grid_flash;

class CGridFlashDlg : public CDialog
{
	DECLARE_DYNAMIC(CGridFlashDlg)

public:
	CGridFlashDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGridFlashDlg();

// Dialog Data
	enum { IDD = IDD_GRID_FLASH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
	void ClickMsflexgrid1();
	DECLARE_EVENTSINK_MAP()
public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOpenbutton();

	void SuspendRefreshTreeviewThread();		
	void ContinueRefreshTreeviewThread();
	void Read_FromTable();
public:
		
	CString file_directory;	
	CString log_file_path;//log file path
	CString saved_path;//saved file path
	CWinThread* m_pBackCheckThread;
	CWinThread* pThread_flash;

private:
	CString m_strHexFile;
	int row_row;
	int row_col;

	int inforline;//消息的行数
public:
	//CComboBox m_subNetList;
	CMsflexgrid m_FlexGrid;
	vector<grid_flash> m_grid_flash;

	CButton m_AllCheckBtn;
	CEdit m_hexFilePathNameEdit;
	CString m_strLogFilePath;
 

	afx_msg void OnBnClickedChecktoall();
	afx_msg void OnBnClickedFlashbutton();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();

	BOOL m_bStopLoadingfile;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListBox m_infoListBox;
	CString m_strInfoText;
	BOOL m_bFinished;
	afx_msg void OnDestroy();
	CButton m_EnableSaveBtn;
	BOOL m_bEnableSaveConfig;
	afx_msg void OnBnClickedCheck1();
};