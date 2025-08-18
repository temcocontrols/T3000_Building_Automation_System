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
	// �������ݿ� - Connect to the database
	/// <summary>
	/// Open a Record set
	/// </summary>
	_RecordsetPtr& OpenRecordset(CString sql);  //�򿪼�¼�� - Open a record set
	void CloseRecordset();						//�رռ�¼�� - Close the record set
	void CloseConn();							//�ر����ݿ����� - Close the database connection
	UINT GetRecordCount(_RecordsetPtr pRecordset);//��ü�¼�� - Get the number of records

	void Createtable(CString strSQL);//�Զ������� - Automatically create a table
	bool IsHaveTable(CBADO ado, CString strTableName);//�жϱ��Ƿ���� - Check if the table exists
	void DeleteDB();
    void RenameTable(CString TableName_Old,CString TableName_New);
};



