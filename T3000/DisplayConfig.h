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
	void Write_Setting();
	BOOL WriteCharsToReg(unsigned char *to_write,unsigned short start_address,int length);
	BOOL ReadCharsFromReg(unsigned short *put_data_into_here,unsigned short start_address,int length);
	//Write_Multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length,int retry_times/* =3 */)
private:
	int m_display_number;
	int m_nCurRow;
	int m_nCurCol;
	CComboBox m_ItemValueCombx;
	UINT m_EDIT_ID;
public:
  
	afx_msg void OnEnKillfocusInput9();
//	afx_msg void OnEnHscrollInput9();
//	afx_msg void OnEnChangeInput9();
	afx_msg void OnCbnSelchangeSeqCom();
    
	void Fresh_Checks();
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetfocusInput1();
	afx_msg void OnSetfocusInput2();
	afx_msg void OnSetfocusInput3();
	afx_msg void OnSetfocusInput4();
	afx_msg void OnSetfocusInput5();
	afx_msg void OnSetfocusInput6();
	afx_msg void OnSetfocusInput7();
	afx_msg void OnSetfocusInput8();
	afx_msg void OnSetfocusLine1();
	afx_msg void OnSetfocusLine2();
	afx_msg void OnBnClickedCheckIconAm();
    afx_msg void OnBnClickedCheckIconMan();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	CButton m_am_check;
    CButton m_man_check;
    CButton m_output_check;
	CButton m_check2;
	CButton m_check3;
	CButton m_check4;
	CButton m_check5;
	CButton m_check6;
	CButton m_check7;
	CButton m_check8;
    void SaveOutputSetting();
    afx_msg void OnEnKillfocusLine1();
    afx_msg void OnEnKillfocusLine2();
    afx_msg void OnBnClickedCheckIconOutput();
    afx_msg void OnBnClickedCheckH0();
     
    afx_msg void OnClickedCheckC0();
    afx_msg void OnClickedCheckC1();
    afx_msg void OnClickedCheckC2();
    afx_msg void OnClickedCheckC3();
    afx_msg void OnClickedCheckC4();
    afx_msg void OnClickedCheckC5();
    afx_msg void OnClickedCheckC6();
    afx_msg void OnClickedCheckH1();
    afx_msg void OnClickedCheckH2();
    afx_msg void OnClickedCheckH3();
    afx_msg void OnClickedCheckH4();
    afx_msg void OnClickedCheckH5();
    afx_msg void OnClickedCheckH6();
};
