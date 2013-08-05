#pragma once
#include "msflexgrid1.h"
#include "afxwin.h"
#include "afxcmn.h"

// CDisplayConfig dialog

class CDisplayConfig : public CDialog
{
	DECLARE_DYNAMIC(CDisplayConfig)

public:
	CDisplayConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDisplayConfig();

// Dialog Data
	enum { IDD = IDD_DISPLAY_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CMsflexgrid m_FlexGrid1;
	DECLARE_EVENTSINK_MAP()
	void DblClickInputMsflexgrid();
	void Fresh_Grid();
	BOOL WriteCharsToReg(unsigned char *to_write,unsigned short start_address,int length);
	BOOL ReadCharsFromReg(unsigned short *put_data_into_here,unsigned short start_address,int length);
	//Write_Multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times/* =3 */)
private:
	int m_display_number;
	int m_nCurRow;
	int m_nCurCol;
	CComboBox m_ItemValueCombx;

public:
  
	afx_msg void OnEnKillfocusInput9();
//	afx_msg void OnEnHscrollInput9();
//	afx_msg void OnEnChangeInput9();
	afx_msg void OnCbnSelchangeSeqCom();
 
 CString GetTextFromReg(unsigned short reg);
 CString GetTextReg(unsigned short reg);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSave2();
	afx_msg void OnBnClickedSaveLine2();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedSaveInput1();
	afx_msg void OnBnClickedSaveInput2();
	afx_msg void OnBnClickedSaveInput3();
	afx_msg void OnBnClickedSaveInput4();
	afx_msg void OnBnClickedSaveInput5();
	afx_msg void OnBnClickedSaveInput6();
	afx_msg void OnBnClickedSaveInput7();
	afx_msg void OnBnClickedSaveInput8();
	CProgressCtrl m_progress;
	afx_msg void OnBnClickedFreshTable();
	afx_msg void OnBnClickedFresh();
	CString GetSel(int ID);
	};
