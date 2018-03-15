#pragma once
#include "afxwin.h"

#include "CM5/CStatic/staticex.h"
// CBacnetEditLabel dialog

class CBacnetEditLabel : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetEditLabel)

public:
	CBacnetEditLabel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetEditLabel();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_EDIT_LABEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CMFCColorButton m_bkClrBtn;
	COLORREF m_bkColor;
	afx_msg void OnBnClickedMfccolorbuttonColor();
	void FreshWindow(Bacnet_Label_Info &temp_info);
	CString m_original_name;
	CString m_label_name;
	CString m_full_label_name;
	CString m_value;
	int digital_status;
	CString m_AutoManual;
	CString m_unit;
	CString m_display_cs;
	CStaticEx m_static_point;
	CStaticEx m_static_label;
	CStaticEx m_static_full_label;
	CStaticEx m_static_value;
	CStaticEx m_static_display;
	CStaticEx m_static_txtcol;
	CStaticEx m_edit_value;
	CStaticEx m_edit_auto_manual;
	CStaticEx m_edit_display;
	Bacnet_Label_Info label_info;
	Bacnet_Label_Info label_info_buffer;
	afx_msg void OnStnClickedStaticEditLabelAutoManual();
	afx_msg LRESULT  MessageCallBack(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Change_Value(WPARAM wParam,LPARAM lParam);
	afx_msg void OnStnClickedStaticEditLabelDisplay();
	afx_msg void OnClose();
	void Initial_UI();
	void ChangeWindowPos(bool nshow);
	CStaticEx m_static_icon_path;
	CStaticEx m_static_text_place;
	CStaticEx m_static_icon_size;
	CStaticEx m_edit_icon_path;
	CStaticEx m_edit_text_place;
	CStaticEx m_edit_icon_size;
	afx_msg void OnStnClickedStaticEditTextPlace();
	afx_msg void OnStnClickedStaticEditIconSize();
	//afx_msg void OnEnChangeEditIconPath();
	afx_msg void OnStnClickedEditIconPath();
	afx_msg void OnBnClickedButtonLabelExit();
	CStaticEx m_edit_icon2_path;
	afx_msg void OnStnClickedEditIconPath2();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	bool m_allow_change;
    int horizontalDPI;
    int verticalDPI;
};

const int CHANGE_AUTO_MANUAL = 1;
const int CHANGE_VALUE = 2;
