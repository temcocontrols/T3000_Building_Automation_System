#pragma once

class  CBADO
{
public:
	CBADO(void);
	~CBADO(void);
public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr  m_pRecordset;
	        
	//CString g_strOrigDatabaseFilePath;
	//CString	g_strDatabasefilepath;
	//CString g_strExePth;
	//CString g_strImgeFolder;
	CString m_ConnectString;
	CString m_dbfilepath;
	CString m_dbImgeFolder;
public:
    void SetDBPath(CString dbpath);
	BOOL OnInitADOConn();						// 
	_RecordsetPtr& OpenRecordset(CString sql);  //
	void CloseRecordset();						//
	void CloseConn();							//
	UINT GetRecordCount(_RecordsetPtr pRecordset);//

	void Createtable(CString strSQL);//
	bool IsHaveTable(CBADO ado, CString strTableName);//
	void DeleteDB();
    void RenameTable(CString TableName_Old,CString TableName_New);
};



