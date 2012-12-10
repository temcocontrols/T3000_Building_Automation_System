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



// CImageTreeCtrl 消息处理程序



BOOL CImageTreeCtrl::SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
{
	//tree0412 屏蔽下面原有的
#if 0
	BOOL bReturn=CTreeCtrl::SetItemImage( hItem, nImage, nSelectedImage );

	HTREEITEM hParentItem;//,hBrotherItem;	
	static int dd=0;
	//查找子节点，没有就结束
	hParentItem=GetParentItem(hItem);
	int brother_nImage,brother_nSelectedImage;
	if(hParentItem)
	{	 
//Subnet,Floor_xx,Room_xx 只有当全部设备没有连接时才显示“没连接图标”，否则不改变这三个图标
#if 0  //LSC
		//have parentitem 
		if(!is_connection_by_image(nImage,nSelectedImage))
		{//this is disconnection
			GetItemImage(hParentItem,brother_nImage,brother_nSelectedImage);
			if(is_connection_by_image(brother_nImage,brother_nSelectedImage))
				SetItemImage(hParentItem,6,brother_nSelectedImage);
			return bReturn;////////////////////unconnection return ;
		}	

		/////////////prev
		hBrotherItem=GetPrevSiblingItem(hItem);
		if(hBrotherItem)
			GetItemImage(hBrotherItem,brother_nImage,brother_nSelectedImage);
		while(hBrotherItem)
		{
			if(!is_connection_by_image(brother_nImage,brother_nSelectedImage))
			{//one brother is disconnection
				GetItemImage(hParentItem,brother_nImage,brother_nSelectedImage);
				if(is_connection_by_image(brother_nImage,brother_nSelectedImage))
					SetItemImage(hParentItem,6,brother_nSelectedImage);
				return bReturn;////////////////////unconnection return ;
			}			
			hBrotherItem=GetPrevSiblingItem(hBrotherItem);
			if(hBrotherItem)
				GetItemImage(hBrotherItem,brother_nImage,brother_nSelectedImage);
		}	
		/////////////next
		hBrotherItem=GetNextSiblingItem(hItem);
		if(hBrotherItem)
			GetItemImage(hBrotherItem,brother_nImage,brother_nSelectedImage);
		while(hBrotherItem)
		{
			if(!is_connection_by_image(brother_nImage,brother_nSelectedImage))
			{//one brother is disconnection
				GetItemImage(hParentItem,brother_nImage,brother_nSelectedImage);
				if(is_connection_by_image(brother_nImage,brother_nSelectedImage))
					SetItemImage(hParentItem,6,brother_nSelectedImage);
				return bReturn;////////////////////unconnection return ;
			}
			hBrotherItem=GetNextSiblingItem(hBrotherItem);
			if(hBrotherItem)
				GetItemImage(hBrotherItem,brother_nImage,brother_nSelectedImage);
		}		
#endif
		//every brother is connection
//		GetItemImage(hParentItem,brother_nImage,brother_nSelectedImage);
// 		if(!is_connection_by_image(brother_nImage,brother_nSelectedImage))
// 			SetItemImage(hParentItem,brother_nSelectedImage,brother_nSelectedImage);
	}
	return bReturn;
#endif
//tree0412

//tree0412
	BOOL bReturn=CTreeCtrl::SetItemImage( hItem, nImage, nSelectedImage );
	return bReturn;
//tree0412
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
//tree0412  //屏蔽下面原有的
#if 0
	int brother_nImage,brother_nSelectedImage;
	GetItemImage(hItem,brother_nImage,brother_nSelectedImage);
	
	HTREEITEM hselItem = GetSelectedItem();
 	if (state)  // online
	{
		int nCurImage = brother_nImage == 6 ? brother_nSelectedImage : brother_nImage;
	
		SetItemImage(hItem, nCurImage, nCurImage);		
	
	}
	else // offline
	{
		if (hselItem == hItem) //sel
		{
			SetItemImage(hItem, brother_nImage, 6);		
		}
		else  // unselected
		{
			SetItemImage(hItem, 6, brother_nImage);	
		}
		
	}

	/*
	if(brother_nImage==0 && brother_nSelectedImage==0 && state==false)						
		SetItemImage(hItem, 6, brother_nSelectedImage);								
	else if(brother_nImage==6 && brother_nSelectedImage==0 && state==true)							
		SetItemImage(hItem,brother_nImage-6,brother_nSelectedImage);						

	if(brother_nImage==1 && brother_nSelectedImage==1 && state==false)						
		SetItemImage(hItem,6,brother_nSelectedImage);					
	else if(brother_nImage==6 && brother_nSelectedImage==1 && state==true)						
		SetItemImage(hItem,brother_nImage-5,brother_nSelectedImage);					

// 	if(brother_nImage==2 && brother_nSelectedImage==2 && state==false)						
// 		SetItemImage(hItem,6,brother_nSelectedImage);					
// 	else if(brother_nImage==6 && brother_nSelectedImage==2 && state==true)						
// 		SetItemImage(hItem,brother_nImage-4,brother_nSelectedImage);					
	if(brother_nImage==2 && brother_nSelectedImage==2 && state==false)						
		SetItemImage(hItem,6,brother_nSelectedImage);					
	else if(brother_nImage==6 && brother_nSelectedImage==2 && state==true)						
		SetItemImage(hItem,brother_nImage-4,brother_nSelectedImage);					

	if(brother_nImage==3 && brother_nSelectedImage==3 && state==false)						
		SetItemImage(hItem,6,brother_nSelectedImage);					
	else if(brother_nImage==6 && brother_nSelectedImage==3 && state==true)						
		SetItemImage(hItem,brother_nImage-3,brother_nSelectedImage);					

	if(brother_nImage==4 && brother_nSelectedImage==4 && state==false)						
		SetItemImage(hItem,6,brother_nSelectedImage);					
	else if(brother_nImage==6 && brother_nSelectedImage==4 && state==true)						
		SetItemImage(hItem,brother_nImage-2,brother_nSelectedImage);					

	if(brother_nImage==5 && brother_nSelectedImage==5 && state==false)						
		SetItemImage(hItem,6,brother_nSelectedImage);					
	else if(brother_nImage==6 && brother_nSelectedImage==5 && state==true)						
		SetItemImage(hItem,brother_nImage-1,brother_nSelectedImage);	
	//	*/
#endif
//tree0412

//tree0412
	int brother_nImage,brother_nSelectedImage;
	GetItemImage(hItem,brother_nImage,brother_nSelectedImage);

// 	if(state == false)&&(brother_nImage!= 7)
// 		SetItemImage(hItem,brother_nImage+1,brother_nSelectedImage+1);
// 	else
// 		SetItemImage(hItem,brother_nImage-1,brother_nSelectedImage-1);
	/*
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
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
		break;
	case 7:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;
	case 8:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
		break;
	case 9:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;
	case 10:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
		break;
	case 11:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;
	case 12:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
		break;
	case 13:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;
	case 14:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
		break;
	case 15:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;
	case 16:
		if(state == false)
		{
			brother_nImage++;
			brother_nSelectedImage++;
		}
		break;
	case 17:
		if(state == true)
		{
			brother_nImage--;
			brother_nSelectedImage--;
		}
		break;

	}
	*/

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
{//return value :true is connection,false is unconnection
//tree0412 //屏蔽下面原有的
#if 0
	if (nImage == 6)		
		return false;	
	else		
		return true;
#endif
//tree0412

//tree0412
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

//tree0412

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

// add a floor node
HTREEITEM CImageTreeCtrl::InsertFloorItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nFloorItemData++);
	return hti;
}
// add a room node 
HTREEITEM CImageTreeCtrl::InsertRoomItem(LPTVINSERTSTRUCT lpInsertStruct)
{
	HTREEITEM hti = InsertItem(lpInsertStruct);
	SetItemData(hti,m_nRoomItemData++);
	return hti;
}
// add a device node
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

// void CImageTreeCtrl::FillBkGroudColor(HTREEITEM& hSelItem)
// {
// 	CRect rc;
// 	GetItemRect(hSelItem, &rc,TRUE);
// 	CDC* pDC = GetDC();
// 
// 	CBrush brush;
// 	LOGBRUSH lb;
// 	lb.lbColor = GetSysColor(COLOR_HIGHLIGHT);
// 	lb.lbStyle = BS_SOLID;
// 	brush.CreateBrushIndirect(&lb);
// 
// 	pDC->SetBkMode(OPAQUE);
// 	pDC->FillRect(&rc, &brush);
// }

