#pragma once

#include "Resource.h"
// CWriteMultipleCoilsDlg dialog

class CWriteMultipleCoilsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWriteMultipleCoilsDlg)

public:
	CWriteMultipleCoilsDlg(UINT Slave_ID = 1, UINT address = 0, UINT quantity = 0, unsigned short *databuffer = NULL, UINT dataformat = 0, BOOL Base_0 = FALSE, CWnd* pParent = NULL);   // standard constructor
	virtual ~CWriteMultipleCoilsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WRITE_MULTIPLE_COILS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	UINT m_address;
	UINT m_slave_id;
	UINT m_quantity;
	UINT m_relative_address;
	CCheckListBox m_ListBox_registers;
	unsigned short *m_DataBuffer;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
