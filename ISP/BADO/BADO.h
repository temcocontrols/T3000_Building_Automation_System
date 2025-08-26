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
	BOOL OnInitADOConn();						// �������ݿ�
	// Open recordset
	_RecordsetPtr& OpenRecordset(CString sql);  //�򿪼�¼��
	// Close recordset
	void CloseRecordset();						//�رռ�¼��
	// Close database connection
	void CloseConn();							//�ر����ݿ�����
	// Get record count
	UINT GetRecordCount(_RecordsetPtr pRecordset);//��ü�¼��

	// Automatically create table
	void Createtable(CString strSQL);//�Զ�������
	// Check if table exists
	bool IsHaveTable(CBADO ado, CString strTableName);//�жϱ��Ƿ����
	void DeleteDB();
    void RenameTable(CString TableName_Old,CString TableName_New);
};



