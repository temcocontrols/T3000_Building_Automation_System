#pragma once

#include "CM5/ListCtrlEx.h"
// CBacnetArray 对话框

class CBacnetArray : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetArray)

public:
	CBacnetArray(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBacnetArray();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_ARRAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void Reset_Array_Rect();
	bool window_max; // 标记是否已经最大化;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	ListCtrlEx::CListCtrlEx m_array_list;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void Initial_List();
	afx_msg LRESULT Fresh_Array_List(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Array_Item(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ArrayMessageCallBack(WPARAM wParam, LPARAM lParam);
};

const int ARRAY_NUM = 0;
const int ARRAY_LABLE = 1;
const int ARRAY_SIZE = 2;
const int ARRAY_VALUE = 3;
