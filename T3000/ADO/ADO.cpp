#include "StdAfx.h"
#include "../ado/ADO.h"
#define FOR_DATABASE_CONNECT					_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")

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

void CADO::OnInitADOConn()
{
	::CoInitialize(NULL);
	try 
	{
////////////////////////////////////////////////////////////////////////////////////////////
					//��ȡ���ݿ����Ƽ�·��
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
						//�������ݿ�


		m_pConnection.CreateInstance("ADODB.Connection");
		_bstr_t strConnect = "DRIVER={Microsoft Access Driver (*.mdb)};\
							 uid=;pwd=;DBQ=t3000.mdb;";
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
	m_pConnection->Close();
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
*   Parameter:         -   pConnection�����ݿ���� 
-   strTableName�����ݿ����Ƿ��д˱� 
*   Return   Value:   -  ��TRUE bool��FALSE���ޱ��б� 
*   Description:     -   �����ݿ��в���Ƿ���Ҫ�򿪵ı�[strTableName]�����ޱ�������ʾ����FALSE�� 
-   �����Ӧ�ô˺���ʱ��ֻ������Ƿ�Ҫ�������У������ٸ�����ʾ�� 
*   Author:               -   lishancai 
*   Date:                   -   2011-10-1 
***********************************************************************************************/ 

bool CADO::IsHaveTable( CADO ado, CString strTableName )
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


	if(bIsHaveNo   ==   FALSE)//�ޱ������ʾ��������FALSE 
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