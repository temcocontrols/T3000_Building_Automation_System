#pragma once
#include "msflexgrid1.h"
#include "afxwin.h"
<<<<<<< HEAD
=======
#include "afxcmn.h"
>>>>>>> 81517f4cbbed910a44d63749dcc3bff4741a0ddf

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
<<<<<<< HEAD
	CString m_Choice_Array[14];
public:
=======

public:
    int Get_RegID(int Model_ID,CString Name);
>>>>>>> 81517f4cbbed910a44d63749dcc3bff4741a0ddf
	afx_msg void OnEnKillfocusInput9();
//	afx_msg void OnEnHscrollInput9();
//	afx_msg void OnEnChangeInput9();
	afx_msg void OnCbnSelchangeSeqCom();
<<<<<<< HEAD
 BOOL UpdateTextToReg(CString input_str,UINT Reg);
 CString GetTextFromReg(UINT reg);
//	afx_msg void OnEnSetfocusLine1();
//	afx_msg void OnEnKillfocusLine1();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSave2();
	afx_msg void OnBnClickedSaveLine2();
=======
 BOOL UpdateTextToReg(CString input_str,UINT Reg,int lenght);
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
>>>>>>> 81517f4cbbed910a44d63749dcc3bff4741a0ddf
	};
