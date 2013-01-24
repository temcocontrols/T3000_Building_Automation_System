#pragma once
//*************************************888***********************888
//this class should  only be used in this project.because the level is limited in this project.
//*************************************888***********************888

// CImageTreeCtrl

class CImageTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CImageTreeCtrl)

public:
	CImageTreeCtrl();
	virtual ~CImageTreeCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage);
	int get_item_level(HTREEITEM hItem);
	void turn_item_image(HTREEITEM hItem,bool state=true);
	BOOL is_connection_by_image(int nImage,int nSelectedImage);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	BOOL Retofline(HTREEITEM hItem);//tree0412

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
private:
	int m_nSubnetItemData;
	int m_nFloorItemData;
	int m_nRoomItemData;
	int m_nDeviceItemData;

};


