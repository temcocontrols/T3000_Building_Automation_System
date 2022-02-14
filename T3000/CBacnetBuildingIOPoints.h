#pragma once

#include "CM5/ListCtrlEx.h"
// CBacnetBuildingIOPoints 对话框

class CBacnetBuildingIOPoints : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetBuildingIOPoints)

public:
	CBacnetBuildingIOPoints(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBacnetBuildingIOPoints();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_BUILDING_IO_POINT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void Fresh();
	void Initial_List();
	ListCtrlEx::CListCtrlEx m_io_list;
	LRESULT Fresh_Building_IO_List(WPARAM wParam, LPARAM lParam);
	LRESULT Change_Building_IO_Item(WPARAM wParam, LPARAM lParam);
	void Fresh_Building_IO_Test_List();
};

const int BM_IO_ITEM = 0;
const int BM_IO_GROUP_NAME = 1;
const int BM_IO_CATEGORY_NAME = 2;
const int BM_IO_HW_POINT = 3;
const int BM_IO_POINT = 4;
const int BM_IO_TYPE = 5;
const int BM_IO_STATUS = 6; // 0 offline   1  online   2 在线未知
