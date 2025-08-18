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
	/// <summary>
	/// Set DB Path
	/// </summary>
	/// <param name="dbpath"></param>
    void SetDBPath(CString dbpath);
	/// <summary>
	/// initial ADO Connection
	/// </summary>
	BOOL OnInitADOConn();	
	// 连接数据库 - Connect to the database
	/// <summary>
	/// Open a Record set
	/// </summary>
	_RecordsetPtr& OpenRecordset(CString sql);  //打开记录集 - Open a record set
	void CloseRecordset();						//关闭记录集 - Close the record set
	void CloseConn();							//关闭数据库连接 - Close the database connection
	UINT GetRecordCount(_RecordsetPtr pRecordset);//获得记录数 - Get the number of records

	void Createtable(CString strSQL);//自动创建表 - Automatically create a table
	bool IsHaveTable(CBADO ado, CString strTableName);//判断表是否存在 - Check if the table exists
	void DeleteDB();
    void RenameTable(CString TableName_Old,CString TableName_New);
};



