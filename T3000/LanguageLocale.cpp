// LanguageLocale.cpp: implementation of the CLanguageLocale class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "T3000.h"
#include "LanguageLocale.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLanguageLocale::CLanguageLocale()
{
	m_nDataSource = LOADSTRING_FROM_RC;
}

CLanguageLocale::~CLanguageLocale()
{

}

bool CLanguageLocale::SetLanguage(int nID)
{
	bool bFound = false;
	if(m_nDataSource == LOADSTRING_FROM_RC)
	{
		switch (nID)
		{
		/*case LANGUAGE_CHS:
			bFound = SetThreadLocale(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED))==TRUE;
			break;*/
		case LANGUAGE_PERSONAL:
			bFound = SetThreadLocale(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_TRADITIONAL))==TRUE;
			break;
		case LANGUAGE_ENGLISH:
			bFound = SetThreadLocale(MAKELANGID(LANG_ENGLISH,SUBLANG_DEFAULT))==TRUE;
			break;
		}
	}
	else
	{
		CFileFind find;
		BOOL bRet = find.FindFile(m_strPath + "\\*.ini");
		bool bFound = false;
		int nLanguage = 0;
		gStrFileName.Empty();
		
		while(bRet)
		{
			bRet = find.FindNextFile();
			if(find.IsDots() || find.IsDirectory()) continue;
			
			CString szFilePath = find.GetFilePath();
			nLanguage = GetPrivateProfileInt(_T("Setting"),_T("Language"),-1,szFilePath);		
			if(nLanguage == nID)
			{
				gStrFileName = szFilePath;
				bFound = true;
				break;
			}		
		}
		find.Close();

	}
	return bFound;
}

void CLanguageLocale::AddCatalogLookupPath(LPCTSTR szPath)
{
	m_strPath = szPath;
	m_nDataSource = LOADSTRING_FROM_INI;
}

CString gLoadString(LPCTSTR szID)
{
	CString szValue;
	GetPrivateProfileString(SECTION_NAME,szID,_T("Not found"),
		gBufferString,BUFF_SIZE,gStrFileName);
	return CString(gBufferString);
}

CString gLoadString(CString section,CString keyname){
	CString szValue;
	GetPrivateProfileString(section,keyname,_T("Not found"),
		gBufferString,BUFF_SIZE,gStrFileName);
	return CString(gBufferString);
}
bool gSetDialogStrings(HWND hWnd,UINT uDlgID)
{
	CString szKey,szText;
	CString szDefault = _T("");

	
	//读对话框标题
	szKey.Format(_T("IDD%d_Title"),uDlgID);
	if(GetPrivateProfileString(SECTION_NAME,szKey,szDefault,
		gBufferString,BUFF_SIZE,gStrFileName) != 0)
	{
		::SetWindowText(hWnd,gBufferString);
	}
	
	//写入各个子控件的标题文字
	HWND hSubWnd = GetWindow(hWnd,GW_CHILD);
	while(::IsWindow(hSubWnd))
	{
		szKey.Format(_T("IDD%d_%d"),uDlgID,GetDlgCtrlID(hSubWnd));
		if(GetPrivateProfileString(SECTION_NAME,szKey,szDefault,
			gBufferString,BUFF_SIZE,gStrFileName) != 0)
		{
			SetWindowText(hSubWnd,gBufferString);
		}
		
		hSubWnd = GetWindow(hSubWnd,GW_HWNDNEXT);
	}
	
	return true;
}

bool gSetFormViewStrings(HWND hWnd,UINT uDlgID)
{
	CString szseactionname,szKey,szText;
	CString szDefault = _T("");

	 
	szseactionname.Format(_T("%s_%d"),T3000_VIEW,uDlgID);
	//读对话框标题
	/*szKey.Format(_T("IDD%d_Title"),uDlgID);
	if(GetPrivateProfileString(szseactionname,szKey,szDefault,
		gBufferString,BUFF_SIZE,gStrFileName) != 0)
	{
		::SetWindowText(hWnd,gBufferString);
	}*/

	//写入各个子控件的标题文字
	HWND hSubWnd = GetWindow(hWnd,GW_CHILD);
	while(::IsWindow(hSubWnd))
	{
		szKey.Format(_T("IDD%d_%d"),uDlgID,GetDlgCtrlID(hSubWnd));
		if(GetPrivateProfileString(szseactionname,szKey,szDefault,
			gBufferString,BUFF_SIZE,gStrFileName) != 0)
		{
			SetWindowText(hSubWnd,gBufferString);
		}

		hSubWnd = GetWindow(hSubWnd,GW_HWNDNEXT);
	}

	return true;
}
bool gGetDialogStrings(HWND hWnd,UINT uDlgID)
{
	CString szseactionname ,szKey,szText;
	 int ID;
	//写入对话框标题
	szseactionname.Format(_T("%s_%d"),T3000_DLG,uDlgID);
	szKey.Format(_T("IDD%d_Title"),uDlgID);
	GetWindowText(hWnd,gBufferString,BUFF_SIZE);
	WritePrivateProfileString(szseactionname,szKey,gBufferString,gStrFileName);
	
	//写入各个子控件的标题文字
	HWND hSubWnd = GetWindow(hWnd,GW_CHILD);
	 
	while(hSubWnd != NULL)
	{   ID=GetDlgCtrlID(hSubWnd);
	    if (ID>1)
	    {
			szKey.Format(_T("IDD%d_%d"),uDlgID,ID);
			GetWindowText(hSubWnd,gBufferString,BUFF_SIZE);
			szText.Format(_T("%s"),gBufferString);
			if (szText.GetLength()>1)
			{
			WritePrivateProfileString(szseactionname,szKey,gBufferString,gStrFileName);
			}
			
	    } 
		hSubWnd = GetWindow(hSubWnd,GW_HWNDNEXT);
	}	

	return true;
}
bool gGetFormViewStrings(HWND hWnd,UINT uDlgID)
{
	CString szseactionname ,szKey,szText;
	 int ID;
	//写入对话框标题
	/*szKey.Format(_T("IDD%d_Title"),uDlgID);
	GetWindowText(hWnd,gBufferString,BUFF_SIZE);
	WritePrivateProfileString(SECTION_NAME,szKey,gBufferString,gStrFileName);*/
	szseactionname.Format(_T("%s_%d"),T3000_VIEW,uDlgID);
	//写入各个子控件的标题文字
	HWND hSubWnd = GetWindow(hWnd,GW_CHILD);
	 
	while(hSubWnd != NULL)
	{   ID=GetDlgCtrlID(hSubWnd);
	    if (ID>1)
	    {
			szKey.Format(_T("IDD%d_%d"),uDlgID,ID);
			GetWindowText(hSubWnd,gBufferString,BUFF_SIZE);
			szText.Format(_T("%s"),gBufferString);
			if (szText.GetLength()>1)
			{
			WritePrivateProfileString(szseactionname,szKey,gBufferString,gStrFileName);
			}
			
	    } 
		hSubWnd = GetWindow(hSubWnd,GW_HWNDNEXT);
	}	

	return true;
}

void InitMenu(HMENU menu,CString KeyName) 
{ 
    if ( menu==NULL || ( !::IsMenu(menu))) 
        return ; 
    CString tempname(_T(""));
	tempname=KeyName;
    CString szText; 
    int nCount,subCount, id; 
 
    nCount =GetMenuItemCount(menu);// menu->GetMenuItemCount();   //获取该层菜单数 
    for(int i=0;i<nCount;i++)                //遍历所有菜单项 
    { 
        //查看子菜单   如果有子菜单，返回0，否则返回子菜单项数目，如果是子菜单项，返回资源中的菜单COMMAND ID 
		 KeyName=_T("");
		 KeyName=tempname;
        id= GetMenuItemID(menu,i); 
             
         if (id!=-1)
         {
		   
		    szText.Format(_T("_%d"),id);
			 KeyName+=szText;
			//ModifyMenu(menu, i, MF_BYPOSITION|MF_STRING, id, gLoadString(KeyName));
         } 
		 else
		 {
		 
		  szText.Format(_T("_%d"),i);
		   KeyName+=szText;
		 // ModifyMenu(menu, i, MF_BYPOSITION|MF_STRING, i, gLoadString(KeyName));
		 }
        CString name=gLoadString(KeyName);
		if (name.CompareNoCase(_T("Not found"))!=0)
		{
		  ModifyMenu(menu, i, MF_BYPOSITION|MF_STRING, id, name);
		}
	  
		 

		 


		//ModifyMenu(menu, i, MF_BYPOSITION|MF_STRING, i, gLoadString(KeyName));
        if( id==-1 ) 
        { 
            subCount =GetMenuItemCount(GetSubMenu(menu,i)); 
            if ( subCount>0 ) 
                InitMenu(GetSubMenu(menu,i),KeyName); 
        } 
    } 
} 


bool gSetMenuStrings(HWND hwnd)
{
	CString szKey;
	HMENU hSubMenu = NULL;
	 
	HMENU hMenu =GetMenu(hwnd);//p_Menubar->GetHMenu();
	
	if(hMenu == NULL)
	{		
		return false;
	}
	int nCount = GetMenuItemCount(hMenu);
	for (int i=0;i<nCount;i++)
	{
		szKey.Format(_T("MENU_%d"),i);
		CString name=gLoadString(szKey);
		if (name.CompareNoCase(_T("Not found"))!=0)
		{
			ModifyMenu(hMenu, i, MF_BYPOSITION|MF_STRING, i,name);
		}

		hSubMenu = GetSubMenu(hMenu,i);
		if(hSubMenu != NULL)
		{   
			InitMenu(hSubMenu,szKey);
		}
	}

	//p_Menubar->DrawMenuBar();
	DrawMenuBar(hwnd);
	
	return false;
}

bool gGetMenuStrings(HWND hWnd)
{
	CString szKey;
	HMENU hSubMenu = NULL;
	HMENU hMenu = GetMenu(hWnd);

	if(hMenu == NULL)
	{		
		return false;
	}



	return false;
}
void ExportMenu(CMenu*menu,CString KeyName) 
{ 
	if ( menu==NULL || ( !::IsMenu(menu->m_hMenu))) 
		return ; 

	CString szText, szSection, szKeyName, szKeyValue; 
	int nCount,subCount; 

	 
	nCount= menu->GetMenuItemCount();    //获取该层菜单数 

	for(int i=0;i<nCount;i++ )               //遍历所有菜单项 
	{ 
		menu->GetMenuString (i, szText, MF_BYPOSITION);//获取菜单文字 
		int nMenuID = GetMenuItemID(menu->GetSubMenu(i)->m_hMenu,i);

		if ( szText!=_T("")) 
		{ 
			//导出到配置文件中 
			if (nMenuID!=-1)
			{
			szKeyName.Format(_T("_%d"),nMenuID); 
			}
			else
			{
			szKeyName.Format(_T("_%d"),nCount); 
			}
			
			KeyName+=szKeyName;
			szKeyValue.Format(_T("%s"),szText); 

			WritePrivateProfileString(SECTION_NAME,KeyName,gBufferString,gStrFileName);

		} 

		//查看是否有子菜单项 
		
		if ( nMenuID==-1 ) 
		{ 
			subCount =menu->GetSubMenu(i)->GetMenuItemCount(); 
			if ( subCount>0 ) 
				ExportMenu( menu->GetSubMenu(i),KeyName); 
		} 

	} 
} 




 
