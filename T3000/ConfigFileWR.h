#pragma once



class CConfigFileWR
{
public:
	CConfigFileWR(void);
	~CConfigFileWR(void);
	BOOL CreateConfigFile(const CString& strFilePath);
	void SetConfigFilePath(const CString& strFilePath);
	void WriteToCfgFile();
	BOOL ReadFromCfgFile();
vector<CString>			m_szCfgFile;
private:
	CStdioFile*					m_pFile;
	CString						m_strFilePath;
	
};
