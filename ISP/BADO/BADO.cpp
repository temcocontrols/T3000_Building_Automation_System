#include "StdAfx.h"
#include "../resource.h"
#include "BADO.h"
//#include "../globle_function.h"
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
	//g_strDatabasefilepath = _T("");
	//g_strOrigDatabaseFilePath=_T("");
	//g_strDatabasefilepath=_T("");
	//g_strExePth=_T("");
	//g_strImgeFolder=_T("");
	m_dbfilepath=_T("");
	m_dbImgeFolder=L"";
}

CBADO::~CBADO(void)
{
}
void CBADO::DeleteDB(){
 
DeleteFile(m_dbfilepath);
}
BOOL CBADO::OnInitADOConn()
{
    CString StrTips;
    HANDLE hFind;//
    WIN32_FIND_DATA wfd;//
    //hFind = FindFirstFile(m_dbfilepath, &wfd);//
    //if (hFind == INVALID_HANDLE_VALUE)//˵����ǰĿ¼����t3000.mdb
    //{
    //    // 	StrTips.Format(_T("%s\n The datebase file disappeared.T3000 help you create a default datebase of your current building."),m_dbfilepath);
    //    // 	AfxMessageBox(StrTips);
    //    CStringArray  ArrayFileName;
    //    SplitCStringA(ArrayFileName, m_dbfilepath, L"\\");
    //    CString filename = L"";
    //    for (int i = 0;i < ArrayFileName.GetSize() - 1;i++)
    //    {
    //        filename += ArrayFileName[i];
    //        filename += L"\\";
    //    }
    //    CreateDirectory(filename, NULL);
    //    m_dbImgeFolder = filename + _T("image");
    //    CreateDirectory(m_dbImgeFolder, NULL);

    //    hFind = FindFirstFile(m_dbfilepath, &wfd);//
    //    if (hFind == INVALID_HANDLE_VALUE)//˵����ǰĿ¼��û��building���ݿ�Ļ����ʹ���һ��
    //    {

    //        HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_DB_SOURCE), _T("BUILDING_DB"));
    //        HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);


    //        LPVOID lpExe = LockResource(hGlobal);
    //        CFile file;
    //        if (file.Open(m_dbfilepath, CFile::modeCreate | CFile::modeWrite))
    //            file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));
    //        file.Close();
    //        ::UnlockResource(hGlobal);
    //        ::FreeResource(hGlobal);



    //    }//
    //    FindClose(hFind);

    //    //return FALSE;
    //}
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
                                // Connect to database
                                //�������ݿ�


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
void CBADO::SetDBPath(CString dbpath){
m_dbfilepath=dbpath;
}
_RecordsetPtr& CBADO::OpenRecordset( CString sql )
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

void CBADO::CloseRecordset()
{
	if (m_pRecordset->State)
	{
	  m_pRecordset->Close();
	}
	

}

void CBADO::CloseConn()
{
	if (m_pConnection->State)
	{m_pConnection->Close();
	}
	m_pConnection=NULL;
	::CoUninitialize();


}

UINT CBADO::GetRecordCount( _RecordsetPtr pRecordset )
{
	int nCount = 0;
	try 
	{
		if(pRecordset->EndOfFile)
		{
			Sleep(1);
			return 0;
		}
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

void CBADO::Createtable( CString strSQL )
{
	CBADO m_ado;
	m_ado.OnInitADOConn();
	m_ado.m_pRecordset = m_ado.OpenRecordset(strSQL);

	m_ado.CloseRecordset();
	m_ado.CloseConn();

}

/*------------------------------------fun-------------------------------------------------------- 
*   Functions           -   IsHaveTable(_ConnectionPtr   pConnection,   CString   strTableName) 
*   Parameter:         -   pConnection: Database object; 
*   Parameter:         -   pConnection�����ݿ���� 
-   strTableName: Is there this table in the database?
-   strTableName�����ݿ����Ƿ��д˱� 
*   Return   Value:   -   TRUE: table exists, FALSE: no table
*   Return   Value:   -  ��TRUE bool��FALSE���ޱ��б� 
*   Description:     -   Query table from database to see if table [strTableName] to be opened exists, if no table, give prompt and return FALSE; 
*   Description:     -   �����ݿ��в���Ƿ���Ҫ�򿪵ı�[strTableName]�����ޱ�������ʾ����FALSE�� 
-   Therefore, when applying this function, you only need to decide whether to continue, no need to give prompt again.
-   �����Ӧ�ô˺���ʱ��ֻ������Ƿ�Ҫ�������У������ٸ�����ʾ�� 
*   Author:               -   lishancai 
*   Date:                   -   2011-10-1 
***********************************************************************************************/ 

bool CBADO::IsHaveTable( CBADO ado, CString strTableName )
{
	
	ado.m_pRecordset = NULL;//���ݿ��ָ�� 
	bool   bIsHaveNo   =   FALSE;//�Ƿ��б�Ĭ���ޱ� 

	//pConnection:ָ�����ݿ� 
	ado.m_pRecordset   =   ado.m_pConnection-> OpenSchema(adSchemaTables);//ָ�����еı� 

	CString   strMsg; 


	while(!(ado.m_pRecordset-> EndOfFile))//ָ���Ƿ��Ѿ�ָ�����һ����¼�� 
	{ 
		_bstr_t   table_name   =   ado.m_pRecordset-> Fields-> GetItem(_T("TABLE_NAME"))-> Value;//�õ�������� 

   
		if(strTableName   ==   (LPCSTR)  table_name)//�����ж��Ƿ���ͬ�� 
		{ 
			bIsHaveNo   =   TRUE;//�б��� 
			break; 
		} 

		ado.m_pRecordset-> MoveNext(); 
	} 


	if(bIsHaveNo   ==   FALSE)
    //�ޱ������ʾ��������FALSE 
	{ 
		//strMsg.Format(_T( "���ݿ���û�в鵽�µı�:   %s��\n����ȷ�����ݿ����Ч�ԣ� "),   strTableName); 
		//MessageBox(NULL,   strMsg,   _T( "���ݿ��û�в鵽�� "),   MB_OK   |   MB_ICONWARNING); 
		//	MessageBox(_T("Products�����ڣ�"));
		return   FALSE; 
	} 
	else//���б�ſɽ�һ������ 
	{ 
		return   TRUE; 
	}

}

void CBADO::RenameTable(CString TableName_Old,CString TableName_New)
{
    
}