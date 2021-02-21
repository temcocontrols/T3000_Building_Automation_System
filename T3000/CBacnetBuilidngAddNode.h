#pragma once


// CBacnetBuilidngAddNode 对话框

class CBacnetBuilidngAddNode : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetBuilidngAddNode)

public:
	CBacnetBuilidngAddNode(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBacnetBuilidngAddNode();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_BUILDING_HANDLE_NODES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void SetParameter(int nfunction, int ntype);
	void InitalUI();
	int m_function;
	int m_type;
	afx_msg void OnBnClickedButtonBmNodeOk();
	afx_msg void OnBnClickedButtonManageBuild();
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
	int m_BM_AddDlg_resault;
}BM_dlg_ret;

