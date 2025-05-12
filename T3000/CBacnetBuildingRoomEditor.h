#pragma once
#include "CM5/ListCtrlEx.h"
#include "CM5\CStatic\staticex.h"
// CBacnetBuildingRoomEditor 对话框

class CBacnetBuildingRoomEditor : public CDialogEx
{
	DECLARE_DYNAMIC(CBacnetBuildingRoomEditor)

public:
	CBacnetBuildingRoomEditor(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBacnetBuildingRoomEditor();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BACNET_BUILDING_EDIT_FLOOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStaticEx m_static_floor;
	CStaticEx m_static_room;
	ListCtrlEx::CListCtrlEx m_bm_floor_list;
	ListCtrlEx::CListCtrlEx m_bm_room_list;
	void InitialUI();
	void Initial_List();
	afx_msg LRESULT Fresh_Floor_List(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Room_List(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_EditCallBack_Item(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Floor_Item(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Fresh_Room_Item(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonBmFloorAdd();
	afx_msg void OnNMClickListBmFloor(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickListBmRoom(NMHDR* pNMHDR, LRESULT* pResult);
};
