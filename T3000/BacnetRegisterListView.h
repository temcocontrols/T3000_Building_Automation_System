#pragma once
#include "CM5/ListCtrlEx.h"
#include "Excel\CApplication0.h"
#include "Excel\CRange0.h" 
#include "Excel\CWorkbook0.h" 
#include "Excel\CWorkbooks0.h" 
#include "Excel\CWorksheet0.h" 
#include "Excel\CWorksheets0.h"
#include "afxcmn.h"
// CBacnetRegisterListView dialog
// CBacnetRegisterListView �Ի���

class CBacnetRegisterListView : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetRegisterListView)

public:
    // Standard constructor
	CBacnetRegisterListView(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBacnetRegisterListView();

// Dialog data
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REGISTER_LIST };
#endif

protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    // Original data value to be modified
    CString cs_clicked_cstring; //Ҫ�޸ĵ�����ԭʼֵ;
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void Test_Read_Excel();
    void InitialListData();
    void Initial_List();
    CButton m_DeleteListAdd;
    CButton m_DeleteListButton;
    void AddDataIntoList();
    ListCtrlEx::CListCtrlEx m_register_view;
    void Initial_List(int nrow,int ncol);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT Fresh_Register_List(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT Fresh_Register_Item(WPARAM wParam, LPARAM lParam);

    int m_record_count;
    int m_max_reg;
    int m_third_party_db;
    int m_device_mode; //����� �������豸 ��Ϊ1    
    static DWORD WINAPI  ReadRegDataThreadfun(LPVOID lpVoid);
    void SetDeviceMode(int n_device_mode);
    afx_msg void OnClose();
    afx_msg void OnLvnItemchangedListRegisterView(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickListRegisterView(NMHDR *pNMHDR, LRESULT *pResult);
    void ShowThirdPartyWindow(int nCmdShow);
    afx_msg void OnBnClickedButtonCreateDb();
    afx_msg void OnBnClickedButtonThirdSave();
    afx_msg void OnNMRClickListRegisterView(NMHDR* pNMHDR, LRESULT* pResult);
    void UpdateThirdUI();
    void OnBnClickedListDelete();
    void OnBnClickedListAdd();
    vector <CString> vector_third_db_name;
    void QueryTable(CString n_device_db_name);
    void QueryAllDbDeviceName();
    afx_msg void OnCbnSelchangeComboRegisterDb();
    afx_msg void OnBnClickedButtonDeleteSelectedDb();
};
int Read_Data_From_DB(int* m_max_retreg);
const int REGISTER_LIST_ID = 0;
const int REGISTER_LIST_ADDRESS = 1;
const int REGUSTER_LIST_OPERATION = 2;
const int REGISTER_LIST_REG_LENGTH = 3;
const int REGISTER_LIST_REG_NAME = 4;
const int REGISTER_LIST_VALUE = 5;
const int REGISTER_LIST_DATA_FORMAT = 6;
const int REGISTER_LIST_DESCRIPTION = 7;
const int REGISTER_LIST_UNIT = 8;
