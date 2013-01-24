#pragma once


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
	BOOL Retofline(HTREEITEM hItem);//


	HTREEITEM InsertSubnetItem(LPTVINSERTSTRUCT lpInsertStruct);

	HTREEITEM InsertFloorItem(LPTVINSERTSTRUCT lpInsertStruct);

	HTREEITEM InsertRoomItem(LPTVINSERTSTRUCT lpInsertStruct);

	HTREEITEM InsertDeviceItem(LPTVINSERTSTRUCT lpInsertStruct);


private:
	int m_nSubnetItemData;
	int m_nFloorItemData;
	int m_nRoomItemData;
	int m_nDeviceItemData;

};


