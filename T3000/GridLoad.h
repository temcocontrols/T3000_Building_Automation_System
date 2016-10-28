#include "msflexgrid1.h"
#include "afxwin.h"

#pragma once


// CGridLoad dialog
typedef struct _grid_Load
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
}GRID_LOAD;

class CGridLoad : public CDialog
{
	DECLARE_DYNAMIC(CGridLoad)

public:
	CGridLoad(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGridLoad();

// Dialog Data
	enum { IDD = IDD_GRID_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton4();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOpenbutton();
	afx_msg void OnBnClickedConfigbutton();
	afx_msg void OnBnClickedAllcheck();
	//afx_msg void OnCbnSelchangeSublist();
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	afx_msg void OnBnClickedBrowsetlogbtn();
	afx_msg void OnBnClickedCancelloadbutton();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	void ClickMsflexgrid1();
	DECLARE_EVENTSINK_MAP()


	
	//void _LoadFile2Tstat(int nTstatID,load_file_every_step &load_file_one_time,TCHAR* fn,CFile *p_log_file);
public:
	BOOL CheckLoadFileForTStat(GRID_LOAD& glItem);
	BOOL CheckLoadFileForNC(GRID_LOAD& glItem);
protected:
	void initRadio();
	void LoadDeviceToGrid();



private:
	CString m_strHexFile;
	int row_row;
	int row_col;
public:
	CEdit			m_hexFilePathNameEdit;
	vector<GRID_LOAD> m_grid_load;
	CString		m_strLogFilePath;//saved_path;//saved file path
	BOOL			m_bStopLoadingfile;
	CWinThread* m_pLoadBackCheckThread;
	CWinThread *m_ploadThread;
	CMsflexgrid	m_FlexGrid;
	CComboBox m_SubListCtrl;
	CButton		m_AllCheckBtn;
	CListBox		m_infoListBox;
	CString		m_strInfoText;
	BOOL			m_bTstatLoadFinished;
	BOOL			m_bNCLoadFinished;
	//BOOL			m_bExit;					// 是否退出程序
	BOOL			m_bProductType;			// TRUE = TSTAT, FALSE = NC
	CEvent		m_eTstatLoadFinish;		// 标记Tstatload 结束，NC可以开始
public:
	//void _change_showing_text_variable(CString str);




};
