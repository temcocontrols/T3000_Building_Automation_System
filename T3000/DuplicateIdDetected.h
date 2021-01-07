#pragma once
#include "CM5\CStatic\staticex.h"
#include "afxwin.h"
#include "../SQLiteDriver/CppSQLite3.h"
#include "global_variable_extern.h"
// CDuplicateIdDetected dialog

class CDuplicateIdDetected : public CDialogEx
{
	DECLARE_DYNAMIC(CDuplicateIdDetected)

public:
	CDuplicateIdDetected(int temp,CWnd* pParent = NULL);

	CDuplicateIdDetected(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDuplicateIdDetected();

// Dialog Data
	enum { IDD = IDD_DIALOG_DUPLICATE_ID };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStaticEx m_static_title;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void Initial_static();
	CStaticEx m_network_title;
	CStaticEx m_dup_network_id;
	CStaticEx m_dup_current_id;
	CStaticEx m_dup_propsed_id;
	CStaticEx m_dup_serial_number;
	CStaticEx m_dup_model;
	CStaticEx m_dup_user_test;
	CStaticEx m_dup_in_database;
	CStaticEx m_dup_id_from_database;

	CStaticEx m_current_id_1;
	CStaticEx m_current_id_2;
	CStaticEx m_edit_propsed_id_1;
	CStaticEx m_edit_propsed_id_2;
	CStaticEx m_static_serial_1;
	CStaticEx m_static_serial_2;
	CStaticEx m_device_model_1;
	CStaticEx m_device_model_2;
	CStaticEx m_user_text_1;
	CStaticEx m_user_text_2;
	CStaticEx m_device_yes_no_1;
	CStaticEx m_device_yes_no_2;
	CStaticEx m_id_from_db_1;
	CStaticEx m_id_from_db_2;
	CStaticEx m_device1_title;
	CStaticEx m_device2_title;
	afx_msg void OnBnClickedButtonDuplicateDone();
	afx_msg void OnBnClickedButtonDuplicateCancel();
	CStaticEx m_duplicate_ret_info;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
    CppSQLite3DB m_SqliteDBBuilding;
    CppSQLite3Table m_table;
    CppSQLite3Query m_q;

	int duplicate_mode;
    BOOL ChangeNetDeviceIP(CString soldIP, CString newstrIP, unsigned int nSerialNum);

    CString original_ip1;
    CString original_ip2;
    CString cschaned_ip1;
    CString cschaned_ip2;

    bool ip1_need_change;
    bool ip2_need_change;
    afx_msg void OnClose();
    virtual void OnCancel();
};
