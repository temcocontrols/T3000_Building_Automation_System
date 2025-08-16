#pragma once

#include "CM5/ListCtrlEx.h"
// CBacnetBuildingIOPoints dialog box
// CBacnetBuildingIOPoints 对话框

class CBacnetBuildingIOPoints : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetBuildingIOPoints)

public:
	// Standard constructor
	CBacnetBuildingIOPoints(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBacnetBuildingIOPoints();

// Dialog data
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_BUILDING_IO_POINT };
#endif

protected:
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	void Fresh();
	void Initial_List();
	ListCtrlEx::CListCtrlEx m_io_list;
	LRESULT Fresh_Building_IO_List(WPARAM wParam, LPARAM lParam);
	LRESULT Change_Building_IO_Item(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMClickListBmIoPoints(NMHDR* pNMHDR, LRESULT* pResult);
};

const int BM_IO_ITEM = 0;
const int BM_IO_PANEL = 1;
const int BM_IO_GROUP_NAME = 2;
const int BM_IO_CATEGORY_NAME = 3;
const int BM_IO_HW_POINT = 4;
const int BM_IO_POINT = 5;

const int BM_IO_FULLLABEL = 6;
const int BM_AUTO_MANUAL = 7;
const int BM_IO_VALUE = 8;
const int BM_IO_UNITS = 9;
const int BM_IO_RANGE = 10;
const int BM_IO_LABEL = 11;

//const int BM_IO_TYPE = 5;
// Product type
//const int BM_ID_PRODUCT = 6;  //产品类型
// 0 offline   1  online   2 online unknown
//const int BM_IO_STATUS = 7; // 0 offline   1  online   2 在线未知
