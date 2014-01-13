#include "StdAfx.h"
#include "../ado/ADO.h"
//#define FOR_DATABASE_CONNECT					_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")
#define FOR_DATABASE_CONNECT					_T("Provider=Microsoft Office 12.0 Access Database Engine OLE DB Provider;Data Source=")

//Provider=Microsoft Office 12.0 Access Database Engine OLE DB Provider
//#define FOR_DATABASE_CONNECT					_T("Driver={Microsoft Access Driver (*.mdb, *.accdb)};Dbq=")
CADO::CADO(void)
{
	g_strDatabasefilepath = _T("");
	g_strOrigDatabaseFilePath=_T("");
	g_strDatabasefilepath=_T("");
	g_strExePth=_T("");
	g_strImgeFolder=_T("");
}

CADO::~CADO(void)
{
}
void CADO::DeleteDB(){
CString filePath=g_strExePth+_T("Database\\T3000.mdb");
DeleteFile(filePath);
}
void CADO::OnInitADOConn()
{
	::CoInitialize(NULL);
	try 
	{
////////////////////////////////////////////////////////////////////////////////////////////
					//获取数据库名称及路径
		TCHAR exeFullPath[MAX_PATH+1]; //
		GetModuleFileName(NULL, exeFullPath, MAX_PATH); 
		(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
		g_strDatabasefilepath=exeFullPath;
		g_strExePth=g_strDatabasefilepath;
		CreateDirectory(g_strExePth+_T("Database"),NULL);//creat database folder;
		g_strOrigDatabaseFilePath=g_strExePth+_T("t3000.mdb");
		g_strDatabasefilepath+=_T("Database\\t3000.mdb");


		HANDLE hFind;
		WIN32_FIND_DATA wfd;
		hFind = FindFirstFile(g_strDatabasefilepath, &wfd);
		if (hFind==INVALID_HANDLE_VALUE)
		{
			CopyFile(g_strOrigDatabaseFilePath,g_strDatabasefilepath,FALSE);
		}
		else
		{

		}
		FindClose(hFind);


		g_strDatabasefilepath=(CString)FOR_DATABASE_CONNECT+g_strDatabasefilepath;
		g_strImgeFolder=g_strExePth+_T("Database\\image\\");
		CreateDirectory(g_strImgeFolder,NULL);

/////////////////////////////////////////////////////////////////////////////////////////////////
						//连接数据库


		m_pConnection.CreateInstance("ADODB.Connection");
	 
		m_pConnection->Open(g_strDatabasefilepath.GetString(),"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}


}

_RecordsetPtr& CADO::OpenRecordset( CString sql )
{
	ASSERT(!sql.IsEmpty());
	try 
	{
		m_pRecordset.CreateInstance(_uuidof(Recordset));
		m_pRecordset->Open(_bstr_t(sql),m_pConnection.GetInterfacePtr(),
			adOpenDynamic,adLockOptimistic,adCmdText);

	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	return m_pRecordset;
}

void CADO::CloseRecordset()
{
	if (m_pRecordset->GetState() == adStateOpen)
	{
		m_pRecordset->Close();
	}

}

void CADO::CloseConn()
{
	if (m_pConnection->State)
	{m_pConnection->Close();
	}
	m_pConnection=NULL;
	::CoUninitialize();


}

UINT CADO::GetRecordCount( _RecordsetPtr pRecordset )
{
	int nCount = 0;
	try 
	{
		pRecordset->MoveFirst();
	}
	catch(...)
	{

		return 0;
	}

	if (pRecordset->FirstOfFile)
		return 0;
	while(!pRecordset->EndOfFile)
	{   
		pRecordset->MoveNext();
		nCount = nCount + 1;
	}
	pRecordset->MoveFirst();
	return nCount;

}

void CADO::Createtable( CString strSQL )
{
	CADO m_ado;
	m_ado.OnInitADOConn();
	m_ado.m_pRecordset = m_ado.OpenRecordset(strSQL);

	m_ado.CloseRecordset();
	m_ado.CloseConn();

}

/*------------------------------------fun-------------------------------------------------------- 
*   Functions           -   IsHaveTable(_ConnectionPtr   pConnection,   CString   strTableName) 
*   Parameter:         -   pConnection：数据库对象； 
-   strTableName：数据库中是否有此表？ 
*   Return   Value:   -  ，TRUE bool。FALSE：无表：有表 
*   Description:     -   从数据库中查表看是否有要打开的表[strTableName]，有无表，给出提示返回FALSE； 
-   因而在应用此函数时，只需决定是否要继续进行，不需再给出提示。 
*   Author:               -   lishancai 
*   Date:                   -   2011-10-1 
***********************************************************************************************/ 

bool CADO::IsHaveTable( CADO ado, CString strTableName )
{
	
	ado.m_pRecordset = NULL;//数据库表指针 
	bool   bIsHaveNo   =   FALSE;//是否有表？默认无表 

	//pConnection:指向数据库 
	ado.m_pRecordset   =   ado.m_pConnection-> OpenSchema(adSchemaTables);//指向所有的表 

	CString   strMsg; 


	while(!(ado.m_pRecordset-> EndOfFile))//指针是否已经指向最后一条记录？ 
	{ 
		_bstr_t   table_name   =   ado.m_pRecordset-> Fields-> GetItem(_T("TABLE_NAME"))-> Value;//得到表的名称 

		if(strTableName   ==   (LPCSTR)  table_name)//表名判断是否相同？ 
		{ 
			bIsHaveNo   =   TRUE;//有表了 
			break; 
		} 

		ado.m_pRecordset-> MoveNext(); 
	} 


	if(bIsHaveNo   ==   FALSE)//无表给出提示，并返回FALSE 
	{ 
		//strMsg.Format(_T( "数据库中没有查到下的表：:   %s。\n请先确认数据库的有效性！ "),   strTableName); 
		//MessageBox(NULL,   strMsg,   _T( "数据库表没有查到！ "),   MB_OK   |   MB_ICONWARNING); 
		//	MessageBox(_T("Products表不存在！"));
		return   FALSE; 
	} 
	else//若有表才可进一步进行 
	{ 
		return   TRUE; 
	}

}