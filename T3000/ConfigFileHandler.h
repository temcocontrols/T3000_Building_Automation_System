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

protected:
	void WriteToCfgFile();
	BOOL ReadFromCfgFile();


private:
	CStdioFile*					m_pFile;
	CString						m_strFilePath;
	 
	vector<CString>				m_szCfgFile;
	CString						g_strExePath;
	

};
