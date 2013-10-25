#pragma once
class  CADO
{
public:
	CADO(void);
	~CADO(void);
public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr  m_pRecordset;

	CString g_strOrigDatabaseFilePath;
	CString	g_strDatabasefilepath;
	CString g_strExePth;
	CString g_strImgeFolder;
public:
	void OnInitADOConn();						// 连接数据库
	_RecordsetPtr& OpenRecordset(CString sql);  //打开记录集
	void CloseRecordset();						//关闭记录集
	void CloseConn();							//关闭数据库连接
	UINT GetRecordCount(_RecordsetPtr pRecordset);//获得记录数

	void Createtable(CString strSQL);//自动创建表
	bool IsHaveTable(CADO ado, CString strTableName);//判断表是否存在
	void DeleteDB();
};



