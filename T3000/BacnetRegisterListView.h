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
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void Test_Read_Excel();
    ListCtrlEx::CListCtrlEx m_register_view;
    void Initial_List(int nrow,int ncol);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    
};
