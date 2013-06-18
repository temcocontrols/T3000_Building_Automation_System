// ImageTreeCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "T3000.h"
#include "ImageTreeCtrl.h"



// CImageTreeCtrl
enum ECmdHandler {
	ID_RENAME = 1,
	ID_DELETE,
	ID_ADD_SIBLING,
	ID_ADD_CHILD,
	ID_ADD_ROOT,
	ID_SORT_LEVEL,
	ID_SORT_LEVELANDBELOW,

	ID_MAX_CMD
};

enum ERightDragHandler {
	ID_DRAG_COPY = 15,
	ID_DRAG_MOVE,
	ID_DRAG_CANCEL,

	ID_MAX_DRH
};

IMPLEMENT_DYNAMIC(CImageTreeCtrl, CTreeCtrl)
CImageTreeCtrl::CImageTreeCtrl()
{
	m_nSubnetItemData = 9000;
	m_nFloorItemData = 1000;
	m_nRoomItemData = 2000;
	m_nDeviceItemData = 3000;
	m_Keymap[VK_F2][false][false] = &CImageTreeCtrl::DoEditLabel;

	//m_Keymap[VK_INSERT][true ][false] = &CImageTreeCtrl::DoInsertChild;
	//m_Keymap[VK_INSERT][false][true ] = &CImageTreeCtrl::DoInsertRoot;
	//m_Keymap[VK_INSERT][false][false] = &CImageTreeCtrl::DoInsertSibling;
	//m_Keymap[VK_DELETE][false][false] = &CImageTreeCtrl::DoDeleteItem;
	//m_Keymap['S'      ][true ][false] = &CImageTreeCtrl::DoSortCurrentLevel;
	//m_Keymap['S'      ][true ][true ] = &CImageTreeCtrl::DoSortCurrentLevelAndBelow;

	m_Commandmap[ID_RENAME]		        = &CImageTreeCtrl::DoEditLabel;
	//m_Commandmap[ID_ADD_CHILD]          = &CImageTreeCtrl::DoInsertChild;
	//m_Commandmap[ID_ADD_ROOT]           = &CImageTreeCtrl::DoInsertRoot;
	//m_Commandmap[ID_ADD_SIBLING]        = &CImageTreeCtrl::DoInsertSibling;
	//m_Commandmap[ID_DELETE]             = &CImageTreeCtrl::DoDeleteItem;
	//m_Commandmap[ID_SORT_LEVEL]         = &CImageTreeCtrl::DoSortCurrentLevel;
	//m_Commandmap[ID_SORT_LEVELANDBELOW] = &CImageTreeCtrl::DoSortCurrentLevelAndBelow;
}

CImageTreeCtrl::~CImageTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CImageTreeCtrl, CTreeCtrl)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
END_MESSAGE_MAP()

bool CImageTreeCtrl::DoEditLabel(HTREEITEM hItem) {
m_hSelItem=hItem;
	return hItem ? (EditLabel(hItem) != 0) : false;
}
BOOL CImageTreeCtrl::UpdateDataToDB(){
	_ConnectionPtr m_pCon;
	_RecordsetPtr m_pRs;
	::CoInitialize(NULL);
	try 
	{
		////////////////////////////////////////////////////////////////////////////////////////////
		//��ȡ���ݿ����Ƽ�·��
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//�������ݿ�
		m_pCon.CreateInstance("ADODB.Connection");
		m_pRs.CreateInstance(_T("ADODB.Recordset"));
		m_pCon->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
		CString strSql;   BOOL is_exist=FALSE;	  CString str_temp;
		switch (m_level)
		{
		case 0:  //Subnet
			{


				//strSql.Format(_T("select * from Building where Main_BuildingName = '%s'"),m_strMainBuildingName);
				strSql.Format(_T("select * from Building order by Main_BuildingName"));
				m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);

				while(VARIANT_FALSE==m_pRs->EndOfFile)
				{	   	 str_temp.Empty();
				str_temp=m_pRs->GetCollect("Building_Name");
				if (str_temp.Compare(m_name_new)==0)
				{	   is_exist=TRUE;
				break;
				}			
				m_pRs->MoveNext();
				}
				m_pRs->Close();
				if (!is_exist)	 //���µ����������ݿ��в��Ҳ�����
				{
					////////////�ȸ���Building��/////////////////////
					//CString strSql;
					strSql.Format(_T("update Building set Building_Name='%s' where Building_Name='%s'"),m_name_new,m_name_old);
					//MessageBox(strSql);
					m_pCon->Execute(strSql.GetString(),NULL,adCmdText);

					strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' order by Building_Name"),m_name_old);
					m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
					while(VARIANT_FALSE==m_pRs->EndOfFile)
					{
						strSql.Format(_T("update ALL_NODE set Building_Name='%s' where Building_Name='%s'"),m_name_new,m_name_old);
						m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
						m_pRs->MoveNext();
					}

					m_pRs->Close();
					return TRUE;
				}
				else
				{

					return FALSE;
				}

				break;	
			}
		case 1:		//Floor
			{	 // strSql=_T("select * from Building where Default_SubBuilding=-1");
				// m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
				HTREEITEM parentnode=GetParentItem(m_hSelItem);

				CString subnetname=GetItemText(parentnode);//m_pRs->GetCollect("Building_Name");

				//  m_pRs->Close();
				strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' order by Building_Name"),subnetname,m_name_new);
				m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);

				while(VARIANT_FALSE==m_pRs->EndOfFile)
				{	   	 str_temp.Empty();
				str_temp=m_pRs->GetCollect("Floor_name");
				if (str_temp.Compare(m_name_new)==0)
				{is_exist=TRUE;
				break;
				}			
				m_pRs->MoveNext();
				}
				m_pRs->Close();

				if (is_exist)
				{
					return FALSE;
				} 
				else
				{  	strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' order by Building_Name"),subnetname,m_name_old);
				m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
				while(VARIANT_FALSE==m_pRs->EndOfFile)
				{
					strSql.Format(_T("update ALL_NODE set Floor_name='%s' where Floor_name='%s'"),m_name_new,m_name_old);
					m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
					m_pRs->MoveNext();
				}
				m_pRs->Close();
				return TRUE;
				}


				break;
			}
		case 2:		//Room
			{
				//Subnet
				HTREEITEM root=GetRootItem();
				CString subnetname=GetItemText(root);
				//Floor
				HTREEITEM parentnode=GetParentItem(m_hSelItem);
				CString Floorname=GetItemText(parentnode);
				strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' and Room_name='%s' order by Building_Name"),subnetname,Floorname,m_name_new);
				m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);

				while(VARIANT_FALSE==m_pRs->EndOfFile)
				{	   	 str_temp.Empty();
				str_temp=m_pRs->GetCollect("Room_name");
				if (str_temp.Compare(m_name_new)==0)
				{	   is_exist=TRUE;
				break;
				}			
				m_pRs->MoveNext();
				}
				m_pRs->Close();


				if (is_exist)
				{
					return FALSE;
				} 
				else
				{strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' and Room_name='%s' order by Building_Name"),subnetname,Floorname,m_name_old);
				m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);

				while(VARIANT_FALSE==m_pRs->EndOfFile)
				{
					strSql.Format(_T("update ALL_NODE set Room_name='%s' where Room_name='%s'"),m_name_new,m_name_old);
					m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
					m_pRs->MoveNext();
				}
				m_pRs->Close();

				return TRUE; 
				}


				break;
			}
		case 3:		//Device Name Leaf
			{
				//Subnet
				HTREEITEM root=GetRootItem();
				CString subnetname=GetItemText(root);
				//Room
				HTREEITEM parentnode=GetParentItem(m_hSelItem);
				CString Roomname=GetItemText(parentnode);
				//Floor
				HTREEITEM floornode=GetParentItem(parentnode);
				CString Floorname=GetItemText(floornode);
				strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' and Room_name='%s' and Product_name='%s' order by Building_Name"),subnetname,Floorname,Roomname,m_name_new);
				m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);

				while(VARIANT_FALSE==m_pRs->EndOfFile)
				{	   	 str_temp.Empty();
				str_temp=m_pRs->GetCollect("Product_name");
				if (str_temp.Compare(m_name_new)==0)
				{	   is_exist=TRUE;
				break;
				}			
				m_pRs->MoveNext();
				}

				m_pRs->Close();	

				if (is_exist)
				{
					return FALSE;
				}
				else
				{
					strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' and Room_name='%s' and Product_name='%s' order by Building_Name"),subnetname,Floorname,Roomname,m_name_old);
					m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
					while(VARIANT_FALSE==m_pRs->EndOfFile)
					{
						strSql.Format(_T("update ALL_NODE set Product_name='%s' where Product_name='%s'"),m_name_new,m_name_old);
						m_pCon->Execute(strSql.GetString(),NULL,adCmdText);
						m_pRs->MoveNext();
					}
					m_pRs->Close();
					return TRUE;
				}

				break;
			}
		}
	}
	catch(_com_error e)
	{
		/* AfxMessageBox(e.Description());*/
		//MessageBox(m_name_new+_T("  has been here\n Please change another name!"));

		return FALSE;
		m_pCon->Close();
	}
	m_pCon->Close();  
	return TRUE;
}
// CImageTreeCtrl ��Ϣ�������
bool CImageTreeCtrl::HandleKeyDown(WPARAM wParam, LPARAM lParam) {
	bool bCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
	bool bShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
	HTREEITEM m_hSelItem = GetSelectedItem();
	m_level=get_item_level(m_hSelItem);
	m_name_old=GetItemText(m_hSelItem);
	if(GetEditControl() == 0) {
		method fnc = m_Keymap[int(wParam)][bCtrl][bShift];
		if(fnc)
			return (this->*fnc)(m_hSelItem);
	}

	// under some circumstances we need to check some more keys
	switch(int(wParam)) {
	case VK_ESCAPE:
		/*if(m_pDragData) {
			DragStop();
			return true;
		}*/
		/*FALLTHRU*/

	case VK_RETURN:
		if(GetEditControl() != 0) {
			GetEditControl()->SendMessage(WM_KEYDOWN, wParam, lParam);

			return true;
		}
		break;

	default:
		break;
	}

	return false;
}
BOOL CImageTreeCtrl::PreTranslateMessage(MSG* pMsg) {
	switch(pMsg->message) {
	case WM_KEYDOWN:
		if(HandleKeyDown(pMsg->wParam, pMsg->lParam))
			return true;
		break;

	default:
		break;
	}
	return CTreeCtrl::PreTranslateMessage(pMsg);
}
void CImageTreeCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	*pResult = 1;

	if(CanEditLabel(pTVDispInfo->item.hItem))
		*pResult = 0;
}
void CImageTreeCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	TVITEM & item = pTVDispInfo->item;
	*pResult = 1;

	if(item.pszText && CanSetLabelText(item)) {
		if (UpdateDataToDB())
		{
		// item.pszText=m_name_new.GetBuffer();
		} 
		else
		{
		item.pszText=m_name_old.GetBuffer();
		}
		SetItem(&item);
		*pResult = 0;
	}
}
bool CImageTreeCtrl::CanEditLabel(HTREEITEM hItem) {
	//TRACE1(_T("CEditTreeCtrl::CanEditLabel('%s')\n"), LPCTSTR(GetItemText(hItem)));
	return true;
}
bool CImageTreeCtrl::CanSetLabelText(TVITEM & item) {
	//TRACE1(_T("CEditTreeCtrl::CanSetLabelText('%s')\n"), item.pszText);
	m_name_new=item.pszText;
	return true;
}
BOOL CImageTreeCtrl::SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
{
	//tree0412 ��������ԭ�е�
#if 0
	BOOL bReturn=CTreeCtrl::SetItemImage( hItem, nImage, nSelectedImage );

	HTREEITEM hParentItem;//,hBrotherItem;	
	static int dd=0;
	//�����ӽڵ㣬û�оͽ���
	hParentItem=GetParentItem(hItem);
	int brother_nImage,brother_nSelectedImage;
	if(hParentItem)
	{	 
//Subnet,Floor_xx,Room_xx ֻ�е�ȫ���豸û������ʱ����ʾ��û����ͼ�ꡱ�����򲻸ı�������ͼ��
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
//tree0412  //��������ԭ�е�
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
	case 18:
	case 20:
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
	case 19:
	case 21:
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
//tree0412 //��������ԭ�е�
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
	return false;
}
int CImageTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;



	// TODO:  Add your specialized creation code here

	return 0;
}
bool CImageTreeCtrl::CanInsertItem(HTREEITEM hItem) {
	TRACE(_T("CEditTreeCtrl::CanInsertItem('%s')\n"), (hItem && hItem != TVI_ROOT) ? LPCTSTR(GetItemText(hItem)) : _T("<Root>"));
	return true;
}
void CImageTreeCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: Add your message handler code here

	
	CPoint pt(point);
	ScreenToClient(&pt);
	UINT flags;
	HTREEITEM hItem = HitTest(pt, &flags);
	bool bOnItem = (flags & TVHT_ONITEM) != 0;

	CMenu add;
	VERIFY(add.CreatePopupMenu());
	if(bOnItem) {
		if(CanInsertItem(GetParentItem(hItem)))
			VERIFY(add.AppendMenu(MF_STRING, ID_ADD_SIBLING, _T("New Sibling\tINS")));
		if(CanInsertItem(hItem))
			VERIFY(add.AppendMenu(MF_STRING, ID_ADD_CHILD, _T("New Child Item\tCtrl+INS")));
	}
	if(CanInsertItem(0))
		VERIFY(add.AppendMenu(MF_STRING, ID_ADD_ROOT, _T("New Root Item\tShift+INS")));

	CMenu sort;
	VERIFY(sort.CreatePopupMenu());
	VERIFY(sort.AppendMenu(MF_STRING, ID_SORT_LEVEL, _T("Current Level\tCtrl+S")));
	VERIFY(sort.AppendMenu(MF_STRING, ID_SORT_LEVELANDBELOW, _T("Current Level And Below\tCtrl+Shift+S")));

	CMenu menu;
	VERIFY(menu.CreatePopupMenu());
	if(bOnItem) {
		if(CanEditLabel(hItem))
			VERIFY(menu.AppendMenu(MF_STRING, ID_RENAME, _T("Rename\tF2")));
		if(CanDeleteItem(hItem))
			VERIFY(menu.AppendMenu(MF_STRING, ID_DELETE, _T("Delete\tDEL")));
	}
	if(add.GetMenuItemCount() > 0)
		VERIFY(menu.AppendMenu(MF_POPUP, UINT(add.GetSafeHmenu()), _T("Add")));
	if(bOnItem) {
		if(menu.GetMenuItemCount() > 0)
			VERIFY(menu.AppendMenu(MF_SEPARATOR));
		VERIFY(menu.AppendMenu(MF_POPUP, UINT(sort.GetSafeHmenu()), _T("Sort")));
	}

	

	// maybe the menu is empty...
	if(menu.GetMenuItemCount() > 0)
		menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	

}

bool CImageTreeCtrl::CanDeleteItem(HTREEITEM hItem) {
	TRACE(_T("CEditTreeCtrl::CanDeleteItem('%s')\n"), LPCTSTR(GetItemText(hItem)));
	return true;
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

