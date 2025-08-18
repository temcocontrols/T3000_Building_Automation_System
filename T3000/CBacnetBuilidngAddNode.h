﻿#pragma once


// CBacnetBuilidngAddNode dialog
// CBacnetBuilidngAddNode 对话框

class CBacnetBuilidngAddNode : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetBuilidngAddNode)

public:
	// Standard constructor
	CBacnetBuilidngAddNode(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBacnetBuilidngAddNode();

// Dialog data
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_BUILDING_HANDLE_NODES };
#endif

protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetParameter(int nfunction, int ntype);
	void InitalUI();
	// Show different input/output counts through examples
	void ResetMoudleCount(int nmoudle_type); //通过例子 展示不同的输入输出个数;
	int m_function;
	int m_type;
	afx_msg void OnBnClickedButtonBmNodeOk();
	afx_msg void OnBnClickedButtonManageBuild();
	afx_msg void OnCbnSelchangeComboBmMoudle();
};

#define FUNCTION_BM_ADD         0
#define FUNCTION_BM_DELETE      1
#define FUNCTION_BM_RENAME      2



typedef struct _BM_dlg_ret
{
	int m_BM_ret_function;
	int m_BM_ret_type;
	int m_BM_ret_count;
	CString m_BM_ret_name;
	int m_bm_add_dlg_resault;
}BM_dlg_ret;

