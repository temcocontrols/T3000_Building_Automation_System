// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#include "T3000.h"
#include "MainFrm.h"
#include "WorkspaceBar.h"
#include "../SQLiteDriver/CppSQLite3.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//#include "MainFrm.h"



const int nBorderSize = 1;

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar

BEGIN_MESSAGE_MAP(CWorkspaceBar, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	//ON_NOTIFY(TVN_SELCHANGED,ID_SHELLTREE,OnFolderSeletedChanged)
	//ON_NOTIFY(TVN_KEYDOWN,ID_SHELLTREE,OnKeyDownChanged)
	ON_NOTIFY(TVN_ENDLABELEDIT, ID_SHELLTREE, OnTvnEndlabeleditTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, ID_SHELLTREE, OnTvnBeginlabeleditTree)
	ON_NOTIFY(NM_CLICK, ID_SHELLTREE, OnNMClickTree)
	ON_NOTIFY(TVN_SELCHANGED, ID_SHELLTREE, OnNMSelectChanged_Tree)

	//ON_NOTIFY(TVN_SELCHANGED,ID_SHELLTREE,OnKYDOWNClickTree)
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
//	ON_WM_RBUTTONDOWN()
//ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar construction/destruction

CWorkspaceBar::CWorkspaceBar()
{
	// 

}

CWorkspaceBar::~CWorkspaceBar()
{

}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

 BOOL CWorkspaceBar::PreTranslateMessage(MSG* pMsg)
 {
    											  
	 //if(pMsg->message == WM_KEYDOWN  )
	 //{
	 //    
		// if(pMsg->wParam == VK_F2)
		// {
		// 
		//	 

		//	
		//	
		//	m_hSelItem=m_TreeCtrl.GetSelectedItem();
		//	m_level=m_TreeCtrl.get_item_level(m_hSelItem);
		//	m_name_old=m_TreeCtrl.GetItemText(m_hSelItem);
		//	
		//	
		//	 
		//		  /* m_renamedlg.m_nodename=m_name_old;

		//	   if ((m_renamedlg.DoModal()==IDOK)||(m_renamedlg.DoModal()==IDCANCEL))
		//	   {  
		//		   if (m_renamedlg.m_nodename.IsEmpty())
		//	   {
		//	   m_name_new=m_name_old;
		//	   } 
		//	   else
		//	   {
		//	   m_name_new=m_renamedlg.m_nodename;
		//	   }
		//		
		//	   }*/
		//	  
		//	  
		//	   if (m_name_new.CompareNoCase(m_name_old)!=0)
		//	   {
		//	     if (UpdateDataToDB())
		//	     {
		//		  m_TreeCtrl.SetItemText(m_hSelItem,m_name_new);
		//	     } 
		//	     else
		//	     {
		//		 MessageBox(m_name_new+_T("  has been here\n Please change another name!"));
		//		  m_TreeCtrl.SetItemText(m_hSelItem,m_name_old);
		//	     }
		//		 
		//	   }
		//	   m_TreeCtrl.SelectItem(m_hSelItem);
		//	   m_TreeCtrl.SetFocus();
		//	    
		//	 return 1;
		// }
	 //    

	 //}	 
	 if(pMsg->message == WM_LBUTTONDOWN)
	 {
		 return 0;
	 }


   return CDockablePane::PreTranslateMessage(pMsg);
 }
   	/*
	Date:2013/05/29
	Purpose:
	parameters 1：根据层次
	2：改变的名字，有新旧比较
   	*/
BOOL CWorkspaceBar::UpdateDataToDB(){
 
	CppSQLite3DB SqliteDBT3000;
	CppSQLite3DB SqliteDBBuilding;
	CppSQLite3Table table;
	CppSQLite3Query q;
	SqliteDBT3000.open((UTF8MBSTR)g_strDatabasefilepath);

	SqliteDBBuilding.open((UTF8MBSTR)g_strCurBuildingDatabasefilePath);

	 try 
	 {
 
		 CString strSql;   BOOL is_exist=FALSE;	  CString str_temp;
         switch (m_level)
         {
         case 0:  //Subnet
		 {
				 strSql.Format(_T("select * from Building order by Main_BuildingName"));
				q = SqliteDBT3000.execQuery((UTF8MBSTR)strSql);
				 while(!q.eof())
				 {	   	 
					 str_temp.Empty();
					 str_temp= q.getValuebyName(L"Building_Name");
				     if (str_temp.Compare(m_name_new)==0)
				     {	  
				      is_exist=TRUE;
				     break;
				     }			
					 q.nextRow();
				 }
				 
				 if (!is_exist)	 //更新的名字在数据库中查找不到的
				 {
				 ////////////先更新Building表/////////////////////
					 //CString strSql;
					 strSql.Format(_T("delete   from Building_ALL where Building_Name = '%s' "),m_name_old);
					 SqliteDBT3000.execDML((UTF8MBSTR)strSql);

					 strSql.Format(_T("Insert into Building_ALL(Building_Name,Default_Build) values('%s','%d')"),m_name_new,1);
					 SqliteDBT3000.execDML((UTF8MBSTR)strSql);

					 strSql.Format(_T("update Building set Building_Name='%s',Main_BuildingName= where Building_Name='%s'"),m_name_new,m_name_new,m_name_old);
					 //MessageBox(strSql);
					 SqliteDBT3000.execDML((UTF8MBSTR)strSql);


					 strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' order by Building_Name"),m_name_old);

					 
					 q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
					 while(!q.eof())
					 {
						 strSql.Format(_T("update ALL_NODE set Building_Name='%s' where Building_Name='%s'"),m_name_new,m_name_old);
						 SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
						 q.nextRow();
					 }

					  

				 }
				 else
				 {

					 return FALSE;
				 }

				 break;	
				 
          
			 }
		 case 1:		//Floor
		 {	 
             // m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
			 HTREEITEM parentnode=m_TreeCtrl.GetParentItem(m_hSelItem);

			  CString subnetname=m_TreeCtrl.GetItemText(parentnode);//m_pRs->GetCollect("Building_Name");
	 
			 //  m_pRs->Close();
			   strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' order by Building_Name"),subnetname,m_name_new);
				  q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
				 while(!q.eof())
				 {	   	
					 str_temp.Empty();
					 str_temp=q.getValuebyName(L"Floor_name");
					 if (str_temp.Compare(m_name_new)==0)
					 {is_exist=TRUE;
					 break;
					 }			
					 q.nextRow();
				 }
			 

				if (is_exist)
				{
						return FALSE;
				} 
				else
				{  
				     strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' order by Building_Name"),subnetname,m_name_old);
				     //m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
				     q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
				     while(!q.eof())
				     {
				     	strSql.Format(_T("update ALL_NODE set Floor_name='%s' where Floor_name='%s'"),m_name_new,m_name_old);
				     	SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
				     	q.nextRow();
				     }
				 
				     return TRUE;
				}
			
		 
		 break;
         }
		 case 2:		//Room
		 {
			//Subnet
			HTREEITEM root=m_TreeCtrl.GetRootItem();
			CString subnetname=m_TreeCtrl.GetItemText(root);
		   //Floor
			HTREEITEM parentnode=m_TreeCtrl.GetParentItem(m_hSelItem);
			CString Floorname=m_TreeCtrl.GetItemText(parentnode);
			  strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' and Room_name='%s' order by Building_Name"),subnetname,Floorname,m_name_new);
			 // m_pRs->Open((_variant_t)strSql,_variant_t((IDispatch *)m_pCon,true),adOpenStatic,adLockOptimistic,adCmdText);
			 q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
			  while(q.eof())
			  {	   	 str_temp.Empty();
			  str_temp=q.getValuebyName(L"Room_name");
			  if (str_temp.Compare(m_name_new)==0)
			  {	   is_exist=TRUE;
			  break;
			  }			
			  q.nextRow();
			  }
			  

			  if (is_exist)
			  {
			  return FALSE;
			  } 
			  else
			  {
			        strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' and Room_name='%s' order by Building_Name"),subnetname,Floorname,m_name_old);
			        q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
			        while(!q.eof())
			        {
			       	  strSql.Format(_T("update ALL_NODE set Room_name='%s' where Room_name='%s'"),m_name_new,m_name_old);
			       	   SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
			       	   q.nextRow();
			        }
			        
			        return TRUE; 
			  }
			  																				  
			    
		  break;
		 }
		 case 3:		//Device Name Leaf
		 {
			 //Subnet
			 HTREEITEM root=m_TreeCtrl.GetRootItem();
			 CString subnetname=m_TreeCtrl.GetItemText(root);
			 //Room
			 HTREEITEM parentnode=m_TreeCtrl.GetParentItem(m_hSelItem);
			 CString Roomname=m_TreeCtrl.GetItemText(parentnode);
			 //Floor
			 HTREEITEM floornode=m_TreeCtrl.GetParentItem(parentnode);
			 CString Floorname=m_TreeCtrl.GetItemText(floornode);
			 strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' and Room_name='%s' and Product_name='%s' order by Building_Name"),subnetname,Floorname,Roomname,m_name_new);
			 q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
			 while(!q.eof())
			 {	   	 
			     str_temp.Empty();
			     str_temp=q.getValuebyName(L"Product_name");
			     if (str_temp.Compare(m_name_new)==0)
			     {	   is_exist=TRUE;
			     break;
			     }			
			      q.nextRow();
			     }
			 
			 
			 if (is_exist)
			 {
			 return FALSE;
			 }
			 else
			 {
				  strSql.Format(_T("select * from ALL_NODE where Building_Name='%s' and Floor_name='%s' and Room_name='%s' and Product_name='%s' order by Building_Name"),subnetname,Floorname,Roomname,m_name_old);
			 
			 q = SqliteDBBuilding.execQuery((UTF8MBSTR)strSql);
			 while(!q.eof())
			 {
				 strSql.Format(_T("update ALL_NODE set Product_name='%s' where Product_name='%s'"),m_name_new,m_name_old);
				 SqliteDBBuilding.execDML((UTF8MBSTR)strSql);
				 q.nextRow();
			 }
			  
			 return TRUE;
			 }
			 
		 break;
		 }
		 }
	 }
	 catch(_com_error e)
	 {
	 
		 SqliteDBBuilding.closedb();
		 return FALSE;
		 //m_pCon->Close();

	 }
	  SqliteDBBuilding.closedb();
	  SqliteDBT3000.closedb();
 
	return TRUE;
 }
int CWorkspaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy (0, 0,200, 600);
	const DWORD dwViewStyle =	WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER
   | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES
   | TVS_DISABLEDRAGDROP | TVS_EDITLABELS;


   m_TreeCtrl.Create(dwViewStyle,CRect(10, 10, 300, 100), this,ID_SHELLTREE);
 //  m_image_list.Create(IDB_TREENODE_BMP,16,1,RGB(255,255,255));
	CBitmap a;
	a.LoadBitmap(IDB_TREENODE_BMP);
	//m_image_list.Create(24,24,ILC_COLOR24,1,1);
	m_image_list.Create(36, 24, ILC_COLOR24, 1, 1);
	m_image_list.Add(&a,RGB(0,0,0));
    m_TreeCtrl.SetImageList(&m_image_list,TVSIL_NORMAL);

	return 0;
}
void CWorkspaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover a whole client area:
	if(m_TreeCtrl.m_hWnd!=NULL)
	{
		m_TreeCtrl.SetWindowPos (NULL, 0, 0,
		cx, cy,
		SWP_NOACTIVATE | SWP_NOZORDER);

	}
	if(m_TreeCtrl.m_hWnd!=NULL)
	{
		m_TreeCtrl.SetWindowPos(NULL,0,0,cx,cy,SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

void CWorkspaceBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	
	CRect rectTree;
	m_TreeCtrl.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,::GetSysColor (COLOR_3DSHADOW),
							::GetSysColor (COLOR_3DSHADOW));

}
void CWorkspaceBar::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}
//void CWorkspaceBar::OnKeyDownChanged(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	/*NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
//	CString strFolder=_T("");
//	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd(); */
////	pMainFrame->OnHTreeItemKeyDownChanged(pNMHDR,pResult);
//}
void CWorkspaceBar::OnFolderSeletedChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CString strFolder=_T("");
	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
	pMainFrame->OnHTreeItemSeletedChanged(pNMHDR,pResult);

}

void CWorkspaceBar::OnFolderBrowerBtn()
{

}




LRESULT CWorkspaceBar::OnNcHitTest(CPoint point)
{
	 
	// 使得 dockable pane 无法拖动了。
return 0;  
	return CDockablePane::OnNcHitTest(point);
}

void CWorkspaceBar::OnLButtonDown(UINT nFlags, CPoint point)
{
 
	CDockablePane::OnLButtonDown(nFlags, point);
}


void CWorkspaceBar::OnTvnEndlabeleditTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	
	*pResult = 0;

//	m_TreeCtrl.SetItemText(pTVDispInfo->item.hItem, pTVDispInfo->item.pszText);
	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
	pMainFrame->OnHTreeItemEndlabeledit(pNMHDR,pResult);
}

	
void CWorkspaceBar::OnTvnBeginlabeleditTree(NMHDR *pNMHDR, LRESULT *pResult)
{	
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	


	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
	*pResult = pMainFrame->OnHTreeItemBeginlabeledit(pNMHDR,pResult);
}


void CWorkspaceBar::OnNMClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	
	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
	pMainFrame->OnHTreeItemClick(pNMHDR,pResult);

}

void CWorkspaceBar::OnNMSelectChanged_Tree(NMHDR *pNMHDR, LRESULT *pResult)
{
	

	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
	pMainFrame->OnHTreeMessageSeletedChanged(pNMHDR,pResult);

}



//void CWorkspaceBar::OnKYDOWNClickTree(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	CMainFrame* pMainFrame= (CMainFrame*)AfxGetMainWnd();
//	pMainFrame->OnHTreeItemKeyDownChanged(pNMHDR,pResult);
//}


//void CWorkspaceBar::OnRButtonDown(UINT nFlags, CPoint point)
//{
//	 
//	 AfxMessageBox(_T("RButtonDown"));
//	CDockablePane::OnRButtonDown(nFlags, point);
//}


//void CWorkspaceBar::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
//{
//	AfxMessageBox(_T("Menu"));
//}
