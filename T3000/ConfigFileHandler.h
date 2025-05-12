/********************************************************************
	created:	2011/12/26
	created:	26:12:2011   13:14
	filename: 	e:\MyWork\ISP\ISP 4.3\ISP\ConfigFileHandler.h
	file path:	e:\MyWork\ISP\ISP 4.3\ISP
	file base:	ConfigFileHandler
	file ext:	h
	author:		ZGQ
	
	purpose:	∂¡–¥≈‰÷√Œƒº˛
*********************************************************************/

#pragma once

class CConfigFileHandler
{
public:
	CConfigFileHandler(void);
	virtual ~CConfigFileHandler(void);

	BOOL CreateConfigFile(const CString& strFilePath);

	BOOL CreateConfigFile();
	void SetConfigFilePath(const CString& strFilePath);
		
	void WriteTStatPageConfig(
		const CString& strHexFileName,
		const CString& strFlashMethod,
		const CString& strFlashTypePage,
		const CString& strModbusID,
		const CString& strComPort,
		const CString& strBaudrate);
	void ReadTStatPageConfig(	CString& strHexFileName,
		CString& strFlashMethod,
		CString& strModbusID,
		CString& strFlashTypePage,
		CString& strComPort,
		CString& strBaudrate);

	void WriteNCPageConfig(const CString& strFlashFileName,
		const CString& strFlashType,
		const CString& strIP,
		const CString& strIPPort);
	void ReadNCPageConfig(	CString& strFlashFileName,
		CString& strFlashType,
		CString& strIP,
		CString& strIPPort);


	void WriteLCPageConfig(const CString& strFlashFileName,
		const CString& strFlashType,
		const CString& strIP,
		const CString& strIPPort,
		 const CString& strMDBID);
	
		void ReadLCPageConfig(CString& strFlashFileName,
		CString& strFlashType,
		CString& strIP,
		CString& strIPPort,
		CString& strMDBID);

	void SetParentWnd(CWnd* pParent);
	
	
	//////////////////////////////////////////////////////////////////////////
	// for product model 
//	int GetProductModel(const CString& strPMFileName, map<int, CString>& szPMMap);

	//////////////////////////////////////////////////////////////////////////
	// for sn flash 
	//void GetFlashSNParam(int &nHWVersion, int & nModelValue);
	
	void GetFlashSNParam(const CString& strFileName, int &nHWVersion, int & nModelValue);
	void SaveFlashSNParam(const CString& strFileName, int &nHWVersion, int & nModelValue);

	//////////////////////////////////////////////////////////////////////////
	// for mac address file
	void GetUnusedMacAddr(const CString& strFileName, BYTE* szMac);
	void SetUsedMacAddr(const CString& strFileName, BYTE* szMac);
	

	void WriteToCfgFile(
		const CString& HexFileName,
		const CString& FlashMethod,
		const CString& DeviceID,
		const CString& COMPORT,
		const CString& Baudrate,
		const CString& IP,
		const CString& IPPort,
		const CString& subnot,
		const CString& subID);
	void ReadFromCfgFileForAll(
		CString& HexFileName,
		CString& FlashMethod,
		CString& DeviceID,
		CString& COMPORT,
		CString& Baudrate,
		CString& IP,
		CString& IPPort,
		CString& subnote,
		CString& subID);

		int GetFlashTimes();

protected:
	void WriteToCfgFile();
	BOOL ReadFromCfgFile();
	
protected:
	BOOL CreatePMFile(CStdioFile& file , const CString& strPMFileName);
	//int ReadPMFile(CStdioFile& file,  map<int,CString>& szPMMap);

	BOOL CreateFlashSNFile(CStdioFile& file, const CString& strFileName);
	void ReadFlashSNFile(CStdioFile& file, int &nHWVersion, int & nModelValue);

private:
	CStdioFile*					m_pFile;
	CString						m_strFilePath;
	CWnd*						m_pParentWnd;
	vector<CString>			m_szCfgFile;
	
	CString m_configfile_path;
	CString m_ISPTool_Section;

};
