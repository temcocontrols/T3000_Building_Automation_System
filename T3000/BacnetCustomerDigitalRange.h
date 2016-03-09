#pragma once


// CBacnetCustomerDigitalRange dialog
#include "CM5/ListCtrlEx.h"
#include "afxcmn.h"
class CBacnetCustomerDigitalRange : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetCustomerDigitalRange)

public:
	CBacnetCustomerDigitalRange(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBacnetCustomerDigitalRange();

// Dialog Data
	enum { IDD = IDD_DIALOG_BACNET_CUSTOMER_RANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	ListCtrlEx::CListCtrlEx m_customer_dig_range_list;
	void Initial_List();
	afx_msg LRESULT Fresh_Customer_Digital_Range_List(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT Fresh_Customer_Digital_Range_Item(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNMClickListCustomerDigitalRange(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

};

const int CUSTOMER_DIGITAL_RANGE_NUM = 0;
const int CUSTOMER_DIGITAL_RANGE_OFF = 1;
const int CUSTOMER_DIGITAL_RANGE_ON = 2;
const int CUSTOMER_DIGITAL_RANGE_DIRECT_INV = 3;

const int CUSTOMER_DIGITAL_RANGE_COL_NUMBER = 4;
