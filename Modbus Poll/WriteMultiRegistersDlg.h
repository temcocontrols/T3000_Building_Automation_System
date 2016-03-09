#pragma once


// CWriteMultiRegistersDlg dialog

class CWriteMultiRegistersDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWriteMultiRegistersDlg)

public:
	CWriteMultiRegistersDlg(UINT Slave_ID=1,UINT address=0,UINT quantity=0,unsigned short *databuffer=NULL, UINT dataformat=0,BOOL Base_0=FALSE,CWnd* pParent = NULL);   // standard constructor
	virtual ~CWriteMultiRegistersDlg();
	virtual BOOL OnInitDialog();
	
	enum { IDD = IDD_DIALOG_WRITE_MULTIPLE_REGISTERS };
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
	afx_msg void OnBnClickedOk();

	void FreshListbox();
	CString Get_Data_No_Address(int index);
	unsigned short Get_Data_Unsigned(int index);
	int Get_Reg_Add(int index);
	void Transfer_Data(int index);
	
	afx_msg void OnEnKillfocusEditAddress();
	afx_msg void OnEnKillfocusEditQuantity();
	afx_msg void OnLbnDblclkList3();
	afx_msg void OnEnKillfocusEditId();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnPaint();
};
