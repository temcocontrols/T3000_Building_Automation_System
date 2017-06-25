/*
 * CppSQLite
 * Developed by Rob Groves <rob.groves@btinternet.com>
 * Maintained by NeoSmart Technologies <http://neosmart.net/>
 * See LICENSE file for copyright and license info
*/


#pragma once

#include "sqlite3.h"
#include <cstdio>
/*#include <cstring>*/
#include <string>
#define CPPSQLITE_ERROR 1000
typedef std::basic_string<TCHAR> stdstring;

class CppSQLite3Exception
{
public:

	CppSQLite3Exception(const int nErrCode,
	                    const char* szErrMess,
	                    bool bDeleteMsg = true);

	CppSQLite3Exception(const CppSQLite3Exception& e);

	virtual ~CppSQLite3Exception();

	const int errorCode() const { return mnErrCode; }

	const char* errorMessage() const { return mpszErrMess; }

	static const char* errorCodeAsString(int nErrCode);

private:

	int mnErrCode;
	char* mpszErrMess;
};


class CppSQLite3Buffer
{
public:

	CppSQLite3Buffer();

	~CppSQLite3Buffer();

	const char* format(const char* szFormat, ...);

	operator const char*() const { return mpBuf; }

	void clear();

private:

	char* mpBuf;
};


class CppSQLite3Binary
{
public:

	CppSQLite3Binary();

	~CppSQLite3Binary();

	void setBinary(const unsigned char* pBuf, int nLen);
	void setEncoded(const unsigned char* pBuf);

	const unsigned char* getEncoded();
	const unsigned char* getBinary();

	int getBinaryLength();

	unsigned char* allocBuffer(int nLen);

	void clear();

private:

	unsigned char* mpBuf;
	int mnBinaryLen;
	int mnBufferLen;
	int mnEncodedLen;
	bool mbEncoded;
};


class CppSQLite3Query
{
public:

	CppSQLite3Query();

	CppSQLite3Query(const CppSQLite3Query& rQuery);

	CppSQLite3Query(sqlite3* pDB,
	                sqlite3_stmt* pVM,
	                bool bEof,
	                bool bOwnVM = true);

	CppSQLite3Query& operator=(const CppSQLite3Query& rQuery);

	virtual ~CppSQLite3Query();

	int numFields() const;

	int fieldIndex(const char* szField) const;
	const char* fieldName(int nCol) const;

	const char* fieldDeclType(int nCol) const;
	int fieldDataType(int nCol) const;

	const char* fieldValue(int nField) const;

	CString getValuebyfield(int nField);

	int getIntField(int nField, int nNullValue = 0) const;
	int getIntField(const char* szField, int nNullValue = 0) const;

	long long getInt64Field(int nField, long long nNullValue = 0) const;
	long long getInt64Field(const char* szField, long long nNullValue = 0) const;

	double getFloatField(int nField, double fNullValue = 0.0) const;
	double getFloatField(const char* szField, double fNullValue = 0.0) const;

	const char* getStringField(int nField, const char* szNullValue = "") const;
	const char* getStringField(const char* szField, const char* szNullValue = "") const;
	CString getValuebyName(CString szField, CString DefaultString = L"");
	const unsigned char* getBlobField(int nField, int& nLen) const;
	const unsigned char* getBlobField(const char* szField, int& nLen) const;

	bool fieldIsNull(int nField) const;
	bool fieldIsNull(const char* szField) const;

	bool eof() const;

	void nextRow();

	void finalize();

	void ConvertUTF8ToString(const char* strInUTF8MB, CString& strOut);
private:

	void checkVM() const;

	sqlite3* mpDB;
	sqlite3_stmt* mpVM;
	bool mbEof;
	int mnCols;
	bool mbOwnVM;
};


class CppSQLite3Table
{
public:

	CppSQLite3Table();

	CppSQLite3Table(const CppSQLite3Table& rTable);

	CppSQLite3Table(char** paszResults, int nRows, int nCols);

	virtual ~CppSQLite3Table();

	CppSQLite3Table& operator=(const CppSQLite3Table& rTable);

	int numFields() const;

	int numRows() const;

	const char* fieldName(int nCol) const;

	const char* fieldValue(int nField) const;
	const char* fieldValue(const char* szField) const;

	int getIntField(int nField, int nNullValue = 0) const;
	int getIntField(const char* szField, int nNullValue = 0) const;

	double getFloatField(int nField, double fNullValue = 0.0) const;
	double getFloatField(const char* szField, double fNullValue = 0.0) const;

	const char* getStringField(int nField, const char* szNullValue = "") const;
	const char* getStringField(const char* szField, const char* szNullValue = "") const;
	CString getValuebyName(const char* szField, const char* szNullValue = "");
	bool fieldIsNull(int nField) const;
	bool fieldIsNull(const char* szField) const;

	void setRow(int nRow);

	void finalize();
	void ConvertUTF8ToString(const char* strInUTF8MB, CString& strOut);
private:

	void checkResults() const;

	int mnCols;
	int mnRows;
	int mnCurrentRow;
	char** mpaszResults;
};


class CppSQLite3Statement
{
public:

	CppSQLite3Statement();

	CppSQLite3Statement(const CppSQLite3Statement& rStatement);

	CppSQLite3Statement(sqlite3* pDB, sqlite3_stmt* pVM);

	virtual ~CppSQLite3Statement();

	CppSQLite3Statement& operator=(const CppSQLite3Statement& rStatement);

	int execDML();

	CppSQLite3Query execQuery();

	void bind(int nParam, const char* szValue);
	void bind(int nParam, const int nValue);
	void bind(int nParam, const long long nValue);
	void bind(int nParam, const double dwValue);
	void bind(int nParam, const unsigned char* blobValue, int nLen);
	void bindNull(int nParam);

	void reset();

	void finalize();

private:

	void checkDB() const;
	void checkVM() const;

	sqlite3* mpDB;
	sqlite3_stmt* mpVM;
};


class CppSQLite3DB
{
public:

	CppSQLite3DB();

	virtual ~CppSQLite3DB();

	/*
	SQLITE_OPEN_READONLY
Open the file read-only. The file must already exist.

SQLITE_OPEN_READWRITE
Attempt to open the file read/write. If this is not possible, open the file read-only. Opening the file read-only will not result in an error. The file must already exist.

SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE

	*/
	int open(const char* szFile);

	void closedb();

	bool tableExists(const char* szTable);
	bool tableColumnExists(const _TCHAR* szTable, const _TCHAR* szName);
	int execDML(const char* szSQL);

	CppSQLite3Query execQuery(const char* szSQL);

	int execScalar(const char* szSQL);

	CppSQLite3Table getTable(const char* szSQL);

	CppSQLite3Statement compileStatement(const char* szSQL);

	sqlite_int64 lastRowId() const;

	void interrupt() { sqlite3_interrupt(mpDB); }

	void setBusyTimeout(int nMillisecs);

	static const char* SQLiteVersion() { return SQLITE_VERSION; }

private:

	CppSQLite3DB(const CppSQLite3DB& db);
	CppSQLite3DB& operator=(const CppSQLite3DB& db);

	sqlite3_stmt* compile(const char* szSQL);

	void checkDB() const;

	sqlite3* mpDB;
	int mnBusyTimeoutMs;
};

class UTF8MBSTR
{
public:
	UTF8MBSTR(void);
	UTF8MBSTR(LPCTSTR lpStr);
	UTF8MBSTR(UTF8MBSTR& lpStr);
	virtual ~UTF8MBSTR();

	void operator =(LPCTSTR lpStr);
	void operator =(UTF8MBSTR& lpStr);
	operator char*();
	operator stdstring();

private:
	char* m_strUTF8_MultiByte;
	size_t ConvertStringToUTF8(LPCTSTR strIn, char*& strOutUTF8MB);
	static void ConvertUTF8ToString(char* strInUTF8MB, size_t len, LPTSTR& strOut);

	size_t m_iLen;
};
