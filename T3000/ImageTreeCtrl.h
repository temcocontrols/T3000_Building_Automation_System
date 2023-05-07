#pragma once
//*************************************888***********************888
//this class should  only be used in this project.because the level is limited in this project.
//*************************************888***********************888

// CImageTreeCtrl
 #include <map>
#include "CBacnetBuildingManagement.h"






class CImageTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CImageTreeCtrl)

public:
	CBacnetBuildingManagement* m_BMpoint;
	CImageTreeCtrl();
	virtual ~CImageTreeCtrl();
	virtual void	DisplayContextMenu(CPoint & point);
	void    DisplayContextOtherMenu(CPoint & point);
	void    BMContextMenu(CPoint& point , BM_nodeinfo nodeinfo);
/*	virtual void	ExtendContextMenu(CMenu & menu);*/
	afx_msg void	OnContextCmd(UINT uID);
	void  SetVirtualTreeItem(HTREEITEM virtual_item);
	virtual bool BM_Rename(HTREEITEM hItem);
	virtual bool BM_Add_Groups(HTREEITEM hItem);
	virtual bool BM_Add_Nodes(HTREEITEM hItem);
	virtual bool BM_Add_Module(HTREEITEM hItem);
	virtual bool BM_Add_Inputs(HTREEITEM hItem);
	virtual bool BM_Add_Outputs(HTREEITEM hItem);
	virtual bool BM_Add_Variable(HTREEITEM hItem);
	void BM_Adds(HTREEITEM hItem, int nfunction, int ntype);
	void BM_Property(HTREEITEM hItem);
	virtual bool BM_IO_Mapping(HTREEITEM hItem);
	virtual bool BM_Delete(HTREEITEM hItem);
	virtual bool SortByConnection(HTREEITEM hItem)  ;
	virtual bool SortByFloor(HTREEITEM hItem)  ;
	virtual bool PingDevice(HTREEITEM hItem) ;
	virtual bool BM_Communicate(HTREEITEM hItem);
	virtual bool ShowEquipmentView(HTREEITEM hItem);
	virtual bool SyncToController(HTREEITEM hItem);
	bool HandleAddVirtualDevice(HTREEITEM);
	bool HandleAddCustomDevice(HTREEITEM hItem);
    bool HandleAddRemoteDevice(HTREEITEM);
    //bool HandleAddThirdPartBacnetDevice(HTREEITEM);
     virtual bool    DoDeleteItem(HTREEITEM);
	 virtual bool ProjectPointView(HTREEITEM hItem);
	 virtual bool	DoEditLabel(HTREEITEM);
protected:
	DECLARE_MESSAGE_MAP()
	virtual bool	HandleKeyDown(WPARAM wParam, LPARAM lParam);
	
    
protected:
typedef bool (CImageTreeCtrl::*method)(HTREEITEM);
enum EDropHint {
	DROP_BELOW,
	DROP_ABOVE,
	DROP_CHILD,
	DROP_NODROP
};
typedef std::map<bool, method>				shiftmap_t;
typedef std::map<bool, shiftmap_t>			ctrlmap_t;
typedef std::map<int, ctrlmap_t>			keymap_t;

typedef std::map<bool, HCURSOR>				cursctrlmap_t;
typedef std::map<EDropHint, cursctrlmap_t>	cursormap_t;

typedef std::map<UINT, method>				cmdmap_t;

keymap_t	m_Keymap;
cmdmap_t	m_Commandmap;
public:
	int GetOnlineDevice();
BOOL UpdateDataToDB_Floor();
BOOL UpdateDataToDB_Connect();
//void Inial_ProductName_map();
//int PanelName_Map(int product_type);
bool CanInsertItem(HTREEITEM hItem);
bool CanDeleteItem(HTREEITEM hItem);
	virtual bool	CanEditLabel(HTREEITEM hItem);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	// Override to control the setting of text for a label. Check the
	// pszText member of the TVITEM structure. The default implementation
	// always returns true. The <item> contains the new text.
	virtual bool	CanSetLabelText(TVITEM & item);
	BOOL SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage);
	int get_item_level(HTREEITEM hItem);
	void turn_item_image(HTREEITEM hItem,bool state=true);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	//virtual bool	CanSetLabelText(TVITEM & item);
	//////////////////////////////////////////////////////////////////////////
	// added by zgq; 2011-06-14
	// add a subnet node
	HTREEITEM InsertSubnetItem(LPTVINSERTSTRUCT lpInsertStruct);
	// add a floor node
	HTREEITEM InsertFloorItem(LPTVINSERTSTRUCT lpInsertStruct);
	// add a room node 
	HTREEITEM InsertRoomItem(LPTVINSERTSTRUCT lpInsertStruct);
	// add a device node
	HTREEITEM InsertDeviceItem(LPTVINSERTSTRUCT lpInsertStruct);

	//void FillBkGroudColor(HTREEITEM& hSelItem);
public:
	int m_nSubnetItemData;
	int m_nFloorItemData;
	int m_nRoomItemData;
	int m_nDeviceItemData;
	int	  m_level;
	CString m_name_old;
	CString m_name_new;
	HTREEITEM m_hSelItem;


protected:
	struct Color_Font
	{
		COLORREF color;
		LOGFONT logfont;
	};
	CMap< void*, void*, Color_Font, Color_Font& > m_mapColorFont ;
	HTREEITEM old_hItem;
public:
	void SetItemFont(HTREEITEM hItem, LOGFONT& logfont);
	void SetItemBold(HTREEITEM hItem, BOOL bBold);
	void SetItemColor(HTREEITEM hItem, COLORREF color);
	BOOL GetItemFont(HTREEITEM hItem, LOGFONT * plogfont);
	BOOL GetItemBold(HTREEITEM hItem);
	void SetSelectItem(HTREEITEM hItem);
	COLORREF GetItemColor(HTREEITEM hItem);
	afx_msg void OnPaint();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	void CheckClickNode(HTREEITEM hItem, BM_nodeinfo & nodeinfo); //给管理模式使用的功能
	void SetSelectSerialNumber(unsigned int nserial);
	unsigned int GetSelectSerialNumber();
	unsigned int m_serial_number;
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	bool is_focus ;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	HTREEITEM m_flashed_tree_item;
	CString offline_mode_string;
	void StopFlashItem();
	void FlashSelectItem(HTREEITEM hItem);
	HTREEITEM m_virtual_tree_item;
	bool tree_offline_mode;
	void SetTreeOfflineMode(bool b_value);
    virtual void PreSubclassWindow();
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

    afx_msg BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);

	afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	CString strmyTipText;

	private:
		CImageList	m_ImageList;		//节点图标
		CImageList* m_pDragImage;		//拖动是的图像列表
		DWORD		m_dwDrawStart;		//开始拖动时刻
		HTREEITEM	m_hDragSrc;			//被拖动节点
		HTREEITEM	m_hDragDist;		//接收拖动的节点
		bool		m_bDraged;			//拖动标识符
		UINT		m_nMsMoveTimerID;	//检测鼠标移动的定时器
		UINT		m_nScrollTimerID;	//滚动检测定时器
		UINT		m_nTicks;			//计算滚动历时
		CPoint		m_ptMouseMove;		//鼠标位置

		afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		LRESULT OnRetriveCheckState(WPARAM sParam, LPARAM lParam);

		int DragBranch(HTREEITEM horgitem, HTREEITEM hdesitem);
		HTREEITEM CopyBranch(HTREEITEM hChildBruch, HTREEITEM hNewParent, HTREEITEM hInsertAfter);
		HTREEITEM CopyItem(HTREEITEM hItem, HTREEITEM hParent, HTREEITEM hInsertAfter);

		public:
			afx_msg BOOL OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
			//afx_msg void OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
			afx_msg void OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);

};

#define TVINSERV_SMALL_CASE        {tvInsert.item.iImage=35;tvInsert.item.iSelectedImage=35;}//Air flow
#define TVINSERV_CO2_NODE        {tvInsert.item.iImage=18;tvInsert.item.iSelectedImage=18;}// Co2Node 小方盒 使用CO2 的图标
#define TREE_IMAGE_INPUT_ONLINE   37
#define TREE_IMAGE_INPUT_OFFLINE  38
#define TREE_IMAGE_INPUT_UNKNOWN  39

#define TREE_IMAGE_OUTPUT_ONLINE   40
#define TREE_IMAGE_OUTPUT_OFFLINE  41
#define TREE_IMAGE_OUTPUT_UNKNOWN  42

#define TREE_IMAGE_VARIABLE_ONLINE   43
#define TREE_IMAGE_VARIABLE_OFFLINE  44
#define TREE_IMAGE_VARIABLE_UNKNOWN  45

#define TREE_IMAGE_PROGRAM_ONLINE		46
#define TREE_IMAGE_PROGRAM_OFFLINE		47
#define TREE_IMAGE_PROGRAM_UNKNOWN		48
#define TREE_IMAGE_PID_ONLINE			49
#define TREE_IMAGE_PID_OFFLINE			50
#define TREE_IMAGE_PID_UNKNOWN			51
#define TREE_IMAGE_SCREEN_ONLINE		52
#define TREE_IMAGE_SCREEN_OFFLINE		53
#define TREE_IMAGE_SCREEN_UNKNOWN		54
#define TREE_IMAGE_SCH_ONLINE		55
#define TREE_IMAGE_SCH_OFFLINE		56
#define TREE_IMAGE_SCH_UNKNOWN		57
#define TREE_IMAGE_HOLIDAY_ONLINE		58
#define TREE_IMAGE_HOLIDAY_OFFLINE		59
#define TREE_IMAGE_HOLIDAY_UNKNOWN		60
#define TREE_IMAGE_TRENDLOG_ONLINE		61
#define TREE_IMAGE_TRENDLOG_OFFLINE		62
#define TREE_IMAGE_TRENDLOG_UNKNOWN		63

