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
	// Connect to database
	BOOL OnInitADOConn();						// 连接数据库
	// Open recordset
	_RecordsetPtr& OpenRecordset(CString sql);  //打开记录集
	// Close recordset
	void CloseRecordset();						//关闭记录集
	// Close database connection
	void CloseConn();							//关闭数据库连接
	// Get record count
	UINT GetRecordCount(_RecordsetPtr pRecordset);//获得记录数

	// Automatically create table
	void Createtable(CString strSQL);//自动创建表
	// Check if table exists
	bool IsHaveTable(CBADO ado, CString strTableName);//判断表是否存在
	void DeleteDB();
    void RenameTable(CString TableName_Old,CString TableName_New);
};



