#pragma once
#include "CM5/ListCtrlEx.h"
#include "Excel\CApplication0.h"
#include "Excel\CRange0.h" 
#include "Excel\CWorkbook0.h" 
#include "Excel\CWorkbooks0.h" 
#include "Excel\CWorksheet0.h" 
#include "Excel\CWorksheets0.h"
#include "afxcmn.h"
// CBacnetRegisterListView 对话框

class CBacnetRegisterListView : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetRegisterListView)

public:
	CBacnetRegisterListView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBacnetRegisterListView();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REGISTER_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CString cs_clicked_cstring; //要修改的数据原始值;
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void Test_Read_Excel();
    void InitialListData();
    void Initial_List();

    void AddDataIntoList();
    ListCtrlEx::CListCtrlEx m_register_view;
    void Initial_List(int nrow,int ncol);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg LRESULT Fresh_Register_List(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT Fresh_Register_Item(WPARAM wParam, LPARAM lParam);
    int m_record_count;
    int m_max_reg;
    static DWORD WINAPI  ReadRegDataThreadfun(LPVOID lpVoid);
    
    afx_msg void OnClose();
    afx_msg void OnLvnItemchangedListRegisterView(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickListRegisterView(NMHDR *pNMHDR, LRESULT *pResult);
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
