#pragma once
#include "afxcmn.h"
#include "CM5\CStatic\staticex.h"
#include "CM5/ListCtrlEx.h"
// CBacnetProperty dialog
// CBacnetProperty �Ի���

class CBacnetProperty : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetProperty)

public:
	// Standard constructor
	CBacnetProperty(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBacnetProperty();

// Dialog data
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_PROPERTY };
#endif

protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    ListCtrlEx::CListCtrlEx m_bacnet_property_list;
    void Initial_List();
    void SetParameter(int nBACNET_PROPERTY_ID);
    void SetBacnetReadString(CString temp_cs);
    void SetTitle(CString temp_title);
    int n_property_id;
    CString cs_bacnet_string;
    CString cs_window_title;
};
