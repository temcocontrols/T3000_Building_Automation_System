#include "StdAfx.h"
#include "../resource.h"
#include "../bado/BADO.h"
#include "../global_function.h"
// #ifdef _DEBUG
// #ifndef _WIN64
#define FOR_DATABASE_CONNECT					_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")
// #else
// #pragma comment(lib,"json/json_mtd_x64.lib")
// #define FOR_DATABASE_CONNECT					_T("Provider=Microsoft Office 12.0 Access Database Engine OLE DB Provider;Data Source=")
// #endif
// #else
// #ifndef _WIN64 
// #define FOR_DATABASE_CONNECT					_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")
// #else
// #define FOR_DATABASE_CONNECT					_T("Provider=Microsoft Office 12.0 Access Database Engine OLE DB Provider;Data Source=")
// #endif
// #endif

//Provider=Microsoft Office 12.0 Access Database Engine OLE DB Provider
//#define FOR_DATABASE_CONNECT					_T("Driver={Microsoft Access Driver (*.mdb, *.accdb)};Dbq=")
CBADO::CBADO(void)
{
	m_dbfilepath = _T("");
	m_dbImgeFolder = L"";
}

CBADO::~CBADO(void)
{
}

void CBADO::DeleteDB()
{
	DeleteFile(m_dbfilepath);
}

BOOL CBADO::OnInitADOConn()
{
	CString StrTips;
	HANDLE hFind;//
	WIN32_FIND_DATA wfd;//
	hFind = FindFirstFile(m_dbfilepath, &wfd);//
	if (hFind == INVALID_HANDLE_VALUE)//t3000.mdb
	{
		// 	StrTips.Format(_T("%s\n The datebase file disappeared.T3000 help you create a default datebase of your current building."),m_dbfilepath);
		// 	AfxMessageBox(StrTips);
		CStringArray ArrayFileName;
		SplitCStringA(ArrayFileName, m_dbfilepath, L"\\");
		CString filename = L"";
		for (int i = 0; i < ArrayFileName.GetSize() - 1; i++)
		{
			filename += ArrayFileName[i];
			filename += L"\\";
		}
		CreateDirectory(filename,NULL);
		m_dbImgeFolder = filename + _T("image");
		CreateDirectory(m_dbImgeFolder,NULL);

		hFind = FindFirstFile(m_dbfilepath, &wfd);//
		if (hFind == INVALID_HANDLE_VALUE)//building
		{
#if 0
		HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_BUILDING_DB2), _T("BUILDING_DB"));   
		HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   


		LPVOID lpExe = LockResource(hGlobal);   
		CFile file;
		if(file.Open(m_dbfilepath, CFile::modeCreate | CFile::modeWrite))    
			file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
		file.Close();    
		::UnlockResource(hGlobal);   
		::FreeResource(hGlobal);
#endif
		}//
		FindClose(hFind);

		//return FALSE;
	}
	::CoInitialize(NULL);
	try
	{
		////////////////////////////////////////////////////////////////////////////////////////////


		m_ConnectString = (CString)FOR_DATABASE_CONNECT + m_dbfilepath;

		// 		CStringArray  ArrayFileName;
		// 		SplitCStringA(ArrayFileName,m_dbfilepath,L"\\");
		// 		CString filename=L"";
		// 		for (int i=0;i<ArrayFileName.GetSize()-1;i++)
		// 		{
		// 			filename+=ArrayFileName[i];
		// 			filename+=L"\\";
		// 		}
		// 		m_dbImgeFolder=filename+_T("image");
		// 		CreateDirectory(m_dbImgeFolder,NULL);

		/////////////////////////////////////////////////////////////////////////////////////////////////
		//


		m_pConnection.CreateInstance("ADODB.Connection");

		m_pConnection->Open(m_ConnectString.GetString(), "", "", adModeUnknown);
	}
	catch (_com_error e)
	{
		//AfxMessageBox(e.Description());
		return FALSE;
	}

	return TRUE;
}

void CBADO::SetDBPath(CString dbpath)
{
	m_dbfilepath = dbpath;
}

/// <summary>
/// according to select sql ,to get the record set.
/// </summary>
/// <param name="sql"></param>
/// <returns></returns>
_RecordsetPtr& CBADO::OpenRecordset(CString sql)
{
	ASSERT(!sql.IsEmpty());
	try
	{
		m_pRecordset.CreateInstance(_uuidof(Recordset));
		m_pRecordset->Open(_bstr_t(sql), m_pConnection.GetInterfacePtr(),
		                   adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	return m_pRecordset;
}

/// <summary>
/// close record set
/// </summary>
void CBADO::CloseRecordset()
{
	if (m_pRecordset->State)
	{
		m_pRecordset->Close();
	}
}


/// <summary>
/// close the connection
/// </summary>
void CBADO::CloseConn()
{
	if (m_pConnection->State)
	{
		m_pConnection->Close();
	}
	m_pConnection = NULL;
	::CoUninitialize();
}

/// <summary>
/// according to recordset ,to calculate how many records in one search
/// </summary>
/// <param name="pRecordset"></param>
/// <returns></returns>
UINT CBADO::GetRecordCount(_RecordsetPtr pRecordset)
{
	int nCount = 0;
	try
	{
		if (pRecordset->EndOfFile)
		{
			Sleep(1);
			return 0;
		}
		pRecordset->MoveFirst();
	}
	catch (...)
	{
		return 0;
	}

	if (pRecordset->FirstOfFile)
		return 0;
	while (!pRecordset->EndOfFile)
	{
		pRecordset->MoveNext();
		nCount = nCount + 1;
	}
	pRecordset->MoveFirst();
	return nCount;
}



/// <summary>
/// According to Sql ,to excute 
/// </summary>
/// <param name="strSQL"></param>
void CBADO::Createtable(CString strSQL)
{
	CBADO m_ado;
	m_ado.OnInitADOConn();
	m_ado.m_pRecordset = m_ado.OpenRecordset(strSQL);

	m_ado.CloseRecordset();
	m_ado.CloseConn();
}

/*------------------------------------fun-------------------------------------------------------- 
*   Functions           -   IsHaveTable(_ConnectionPtr   pConnection,   CString   strTableName) 
*   Parameter:         -   pConnection 
-   strTableName 
*   Return   Value:   -  TRUE boolFALSE 
*   Description:     -   [strTableName]FALSE 
-    
*   Author:               -   lishancai 
*   Date:                   -   2011-10-1 
***********************************************************************************************/

bool CBADO::IsHaveTable(CBADO ado, CString strTableName)
{
	ado.m_pRecordset = NULL;// 
	bool bIsHaveNo = FALSE;// 

	//pConnection: 
	ado.m_pRecordset = ado.m_pConnection->OpenSchema(adSchemaTables);// 

	CString strMsg;


	while (!(ado.m_pRecordset->EndOfFile))// 
	{
		_bstr_t table_name = ado.m_pRecordset->Fields->GetItem(_T("TABLE_NAME"))->Value;// 


		if (strTableName == (LPCSTR)table_name)// 
		{
			bIsHaveNo = TRUE;// 
			break;
		}

		ado.m_pRecordset->MoveNext();
	}


	if (bIsHaveNo == FALSE)
	//FALSE 
	{
		//strMsg.Format(_T( ":   %s\n "),   strTableName); 
		//MessageBox(NULL,   strMsg,   _T( " "),   MB_OK   |   MB_ICONWARNING); 
		//	MessageBox(_T("Products"));
		return FALSE;
	}
	else// 
	{
		return TRUE;
	}
}


/// <summary>  
/// Rename Database Table by the function .
/// </summary> 
/// <param name="TableName_Old"></param>
/// <param name="TableName_New"></param>
void CBADO::RenameTable(CString TableName_Old, CString TableName_New)
{

}
