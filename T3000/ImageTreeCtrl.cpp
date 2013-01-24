// ImageTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "T3000.h"
#include "ImageTreeCtrl.h"



// CImageTreeCtrl

IMPLEMENT_DYNAMIC(CImageTreeCtrl, CTreeCtrl)
CImageTreeCtrl::CImageTreeCtrl()
{
	m_nSubnetItemData = 9000;
	m_nFloorItemData = 1000;
	m_nRoomItemData = 2000;
	m_nDeviceItemData = 3000;
}

CImageTreeCtrl::~CImageTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CImageTreeCtrl, CTreeCtrl)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()



//



BOOL CImageTreeCtrl::SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
{


	BOOL bReturn=CTreeCtrl::SetItemImage( hItem, nImage, nSelectedImage );
	return bReturn;

}
int CImageTreeCtrl::get_item_level(HTREEITEM hItem)
{//return value 0,
	int r_v=0;
	HTREEITEM hParentItem=hItem;
	do{
		hParentItem=GetParentItem(hParentItem);
		r_v++;
	}while(hParentItem!=NULL);
	r_v--;//^-^	
	return r_v;
}

void CImageTreeCtrl::turn_item_image(HTREEITEM hItem,bool state)
{
	int brother_nImage,brother_nSelectedImage;
	GetItemImage(hItem,brother_nImage,brother_nSelectedImage);

	switch(brother_nImage)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:	
		break;
	case 6:
	case 8:
	case 10:
	case 12:
	case 14:
	case 16:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
	break;
		case 7:
	case 9:
	case 11:
	case 13:
	case 15:
	case 17:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
	break;

	}
	SetItemImage(hItem,brother_nImage,brother_nSelectedImage);


//tree0412


}

BOOL CImageTreeCtrl::is_connection_by_image(int nImage,int nSelectedImage)
{


	switch(nImage)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:	
	case 6:
	case 8:
	case 10:
	case 12:
	case 14:
	case 16:
		return true;
	case 7:
	case 9:
	case 11:		
	case 13:		
	case 15:	
	case 17:
		return false;
	}



}
int CImageTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;



	// TODO:  Add your specialized creation code here

	return 0;
}

void CImageTreeCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here

	
	CMenu menu;
	menu.LoadMenu(IDR_BUILDINGPOPMENU);
	CMenu *pmenu=menu.GetSubMenu(0);	
//		ClientToScreen(&point);
//		ScreenToClient(&point);
	pmenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_LEFTALIGN ,point.x,point.y,this);
	

}


HTREEITEM CImageTreeCtrl::InsertSubnetItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nSubnetItemData++);
	return hti;
}


HTREEITEM CImageTreeCtrl::InsertFloorItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nFloorItemData++);
	return hti;
}

HTREEITEM CImageTreeCtrl::InsertRoomItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nRoomItemData++);
	return hti;
}

HTREEITEM CImageTreeCtrl::InsertDeviceItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nDeviceItemData++);
	return hti;
}

BOOL CImageTreeCtrl::Retofline( HTREEITEM hItem )//tree0412
{
	int brother_nImage,brother_nSelectedImage;
	GetItemImage(hItem,brother_nImage,brother_nSelectedImage);
	return is_connection_by_image(brother_nImage,brother_nSelectedImage);



}


