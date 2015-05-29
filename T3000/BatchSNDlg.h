#pragma once
 #include "CM5/ListCtrlEx.h"


// CBatchSNDlg dialog

class CBatchSNDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBatchSNDlg)

public:
	CBatchSNDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBatchSNDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

    ListCtrlEx::CListCtrlEx m_products_list;
    UINT m_intStart;
    UINT m_intEnd;
    afx_msg void OnBnClickedOk();
    virtual BOOL OnInitDialog();
    unsigned long GetNewSerialNumber();
    void WriteSerialNumber(unsigned long SN);
    afx_msg void OnEnKillfocusEditStart();
    afx_msg void OnEnKillfocusEditEnd();
    vector<Batch_SN> m_modbus_IDs;
    HANDLE hFirstThread;
    afx_msg LRESULT Fresh_Input_List(WPARAM wParam,LPARAM lParam);
/*    afx_msg LRESULT Change_Input_Item(WPARAM wParam,LPARAM lParam);*/
    afx_msg void OnNMClickList_Input(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedCalibrationPointReport();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
