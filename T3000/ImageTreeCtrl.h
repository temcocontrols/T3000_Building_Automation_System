#pragma once
//*************************************888***********************888
//this class should  only be used in this project.because the level is limited in this project.
//*************************************888***********************888

// CImageTreeCtrl
 #include <map>
class CImageTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CImageTreeCtrl)

public:
	CImageTreeCtrl();
	virtual ~CImageTreeCtrl();
	virtual void	DisplayContextMenu(CPoint & point);
	void    DisplayContextOtherMenu(CPoint & point);
/*	virtual void	ExtendContextMenu(CMenu & menu);*/
	afx_msg void	OnContextCmd(UINT uID);
	void  SetVirtualTreeItem(HTREEITEM virtual_item);
	virtual bool SortByConnection(HTREEITEM hItem)  ;
	virtual bool SortByFloor(HTREEITEM hItem)  ;
	virtual bool PingDevice(HTREEITEM hItem) ;
	bool HandleAddVirtualDevice(HTREEITEM);
	bool HandleAddCustomDevice(HTREEITEM hItem);
    bool HandleAddRemoteDevice(HTREEITEM);
     virtual bool    DoDeleteItem(HTREEITEM);

	
protected:
	DECLARE_MESSAGE_MAP()
	virtual bool	HandleKeyDown(WPARAM wParam, LPARAM lParam);
	virtual bool	DoEditLabel(HTREEITEM);
    
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
	BOOL is_connection_by_image(int nImage,int nSelectedImage);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	BOOL Retofline(HTREEITEM hItem);//tree0412
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

};


