#pragma once


// CReadWrite_Multiple_RegDlg dialog

class CReadWrite_Multiple_RegDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReadWrite_Multiple_RegDlg)

public:
	CReadWrite_Multiple_RegDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReadWrite_Multiple_RegDlg();
	virtual BOOL OnInitDialog();
// Dialog Data
	enum { IDD = IDD_DIALOG_R_W_MULTIPLE_REGISTERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	UINT m_address;
	UINT m_slave_id;
	UINT m_quantity;
	UINT m_dataformate;
	UINT m_relative_address;
	unsigned short *m_DataBuffer;
	BOOL m_base0;
	BOOL m_Binary_Value[16];
	CListBox m_ListBox_registers;
	afx_msg void OnLbnDblclkList3();
	afx_msg void OnBnClickedOk();
	CListBox m_result_registers;
	UINT m_result_address;
	UINT m_result_quantity;
	afx_msg void OnEnKillfocusEditId();
	afx_msg void OnEnKillfocusEditAddress();
	afx_msg void OnEnKillfocusEditQuantity();
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnKillfocusEdit2();



	void FreshListbox();
	CString Get_Data_No_Address(int index);
	int Get_Reg_Add(int index);
	unsigned short Get_Data_Unsigned(int index);
	afx_msg void OnBnClickedButtonSend();
};
