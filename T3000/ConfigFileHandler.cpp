#include "StdAfx.h"
#include "ConfigFileHandler.h"
#include "Global_Struct.h"
#include <locale.h>
#include "globle_function.h"

enum CFGFILE_VALPOS
{
	CV_Tstat_Section = 0,
	CV_Installdir =1,
	CV_TstatLastFlashFile, 
	CV_TstatLastFlashMethod,
	CV_TstatLastFlashPage,
	CV_TstatDeAddr,
	CV_TstatDeCOM,
	CV_TstatDeBaudrate,
	CV_NC_Section,
	CV_NCLastFlashFile,
	CV_NCLastFlashMethod,
	CV_NCDeIP,
	CV_NCDeIPPort,
	CV_LC_Section,
	CV_LCLastFlashFile,
	CV_LCLastFlashType,
	CV_LCDeIP,
	CV_LCDeIPPort,
	CV_LCDMDBID,
	CV_SubNot,
	CV_Sub_ID,
};

CConfigFileHandler::CConfigFileHandler(void)
{
	m_pFile = new CStdioFile;
	g_strExePath=GetExePath(true);
}

CConfigFileHandler::~CConfigFileHandler(void)
{
	m_szCfgFile.clear();
	if (m_pFile)
	{
		//m_pFile->Close();
		delete m_pFile;
	}
	
}


void CConfigFileHandler::SetConfigFilePath(const CString& strFilePath)
{
	ASSERT(strFilePath.GetLength() > 0);
	m_strFilePath = strFilePath;
}



// 创建config文件
// 如果已经存在，则不做事情
BOOL CConfigFileHandler::CreateConfigFile(const CString& strFilePath)
{

 
	ASSERT(strFilePath.GetLength() > 0);
	m_strFilePath = strFilePath;

	if (!m_pFile->Open(m_strFilePath.GetString(),CStdioFile::modeRead))
	{
		if(m_pFile->Open(m_strFilePath.GetString(),CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate ))
		{
			CString strHexFileName = _T("C:\\Program File\\");
			CString strFlashMethod = _T("COM");
			CString strFlashPage = _T("[Tstat]");
			CString strModbusID = _T("255");
			CString strComPort =  _T("COM1");
			CString strBaudrate = _T("19200");
			CString strNCFlashPage = _T("[NC]");
			CString strNCFileName = _T("C:\\Program File\\");
			CString strNCFlashMethod = _T("Ethernet");
			CString strIP = _T("192.168.0.3");
			CString strIPPort =  _T("6001");
			CString strSub_not = _T("0");
			CString strSub_ID = _T("254");
			// 
			m_pFile->WriteString( CString(c_strCfgTstatSection) + _T("\n"));	
			m_pFile->WriteString( CString(c_strCfgDefaultInstalldirItem) + g_strExePath+ _T("\n"));	

			// hex file path
			CString strHexFilePath = strHexFileName + _T("\n");
			m_pFile->WriteString( CString(c_strCfgLastFlashFileItem) + strHexFilePath+ _T("\n"));	
			// method
			m_pFile->WriteString(CString(c_strCfgLastFlashMethodItem) + strFlashMethod + _T("\n"));
			// page
			m_pFile->WriteString(CString(c_strCfgLastFlashTypePageItem) + strFlashPage + _T("\n"));					
			// id
			m_pFile->WriteString(CString(c_strCfgDefaultAddrItem) + strModbusID + _T("\n"));	
			// com port
			m_pFile->WriteString(CString(c_strCfgDefaultComItem) + strComPort + _T("\n"));	
			// Baudrate
			m_pFile->WriteString(CString(c_strCfgDefaultBaudrateItem) + strBaudrate + _T("\n"));	

			// nc section
			m_pFile->WriteString(strNCFlashPage+ _T("\n"));	
			// nc file
			m_pFile->WriteString(CString(c_strCfgLastFlashFileItem) + strNCFileName + _T("\n"));
			// nc flash type
			m_pFile->WriteString(CString(c_strCfgLastFlashTypePageItem) + strNCFlashMethod + _T("\n"));
			// IP
			m_pFile->WriteString(CString(c_strCfgDefaultIPItem) + strIP + _T("\n"));
			// Port
			m_pFile->WriteString( CString(c_strCfgDefaultIPPortItem) + strIPPort + _T("\n"));

			// Lighting Controller
			m_pFile->WriteString( CString(c_strCfgLightCtrlSection) + _T("\n"));	
			// flash file
			m_pFile->WriteString( CString(c_strCfgLastFlashFileItem) + strHexFilePath);	
			// flash device type
			m_pFile->WriteString( CString(c_strCfgLastFlashLCTypeItem) + CString(c_strCfgLastFlashLCTypeMain) + _T("\n"));	
			// IP
			m_pFile->WriteString(CString(c_strCfgDefaultIPItem) + strIP + _T("\n"));
			// Port
			m_pFile->WriteString( CString(c_strCfgDefaultIPPortItem) + strIPPort + _T("\n"));
			// MDB ID
			m_pFile->WriteString( CString(c_strCfgLastFlashMDBIDItem) + strModbusID + _T("\n"));

			m_pFile->WriteString( CString(c_strCfgNote) + strSub_not + _T("\n"));
			m_pFile->WriteString( CString(c_strCfgSubID) + strSub_ID + _T("\n"));

		}
		else
		{
			CString strTips = _T("├Cannot create a new config file. Please try again.");
		
			
			m_pFile->Close();

			return FALSE;
		}
	}
	m_pFile->Close();
	return TRUE;

}


void CConfigFileHandler::WriteTStatPageConfig(
	const CString& strHexFileName,
	const CString& strFlashMethod,
	const CString& strFlashTypePage,
	const CString& strModbusID,
	const CString& strComPort,
	const CString& strBaudrate)
{
	ASSERT(m_pFile);


	//m_szCfgFile[CV_Tstat_Section] = (CString(c_strCfgTstatSection) + _T("\n"));				
	m_szCfgFile[CV_Installdir] = (CString(c_strCfgDefaultInstalldirItem) + g_strExePath);	

	// hex file path
	CString strHexFilePath = strHexFileName;
	m_szCfgFile[CV_TstatLastFlashFile] = ( CString(c_strCfgLastFlashFileItem) + strHexFilePath);	
	// method
	m_szCfgFile[CV_TstatLastFlashMethod] =(CString(c_strCfgLastFlashMethodItem) + strFlashMethod );		
	// flash page
	m_szCfgFile[CV_TstatLastFlashPage] =(CString(c_strCfgLastFlashTypePageItem) + strFlashTypePage);			
	// id
	m_szCfgFile[CV_TstatDeAddr] =(CString(c_strCfgDefaultAddrItem) + strModbusID );	
	// com port
	m_szCfgFile[CV_TstatDeCOM] =(CString(c_strCfgDefaultComItem) + strComPort);	
	// Baudrate
	m_szCfgFile[CV_TstatDeBaudrate] =(CString(c_strCfgDefaultBaudrateItem) + strBaudrate);

// 	// NC section
// 	m_szCfgFile[CV_NC_Section] =(CString(c_strCfgNCSection));
// 	// NC flash file
// 	m_szCfgFile[CV_NCLastFlashFile] =(CString( c_strCfgDefaultIPPortItem) + strIPPort );
// 	// NC flash type
// 	m_szCfgFile[CV_NCLastFlashMethod] =(CString( c_strCfgDefaultIPPortItem) + strIPPort );
// 	// IP
// 	m_szCfgFile[CV_NCDeIP] =(CString(c_strCfgDefaultIPItem) + strIP);
// 	// Port
// 	m_szCfgFile[CV_NCDeIPPort] =(CString( c_strCfgDefaultIPPortItem) + strIPPort );

	WriteToCfgFile();


}


void CConfigFileHandler::WriteToCfgFile(
	const CString& HexFileName,
	const CString& FlashMethod,
	const CString& DeviceID,
	const CString& COMPORT,
	const CString& Baudrate,
	const CString& IP,
	const CString& IPPort,
	const CString& subnot,
	const CString& subID)
{
	m_szCfgFile[CV_Installdir] = (CString(c_strCfgDefaultInstalldirItem) + g_strExePath);	

	// hex file path
	CString strHexFilePath = HexFileName;
	m_szCfgFile[CV_TstatLastFlashFile] = ( CString(c_strCfgLastFlashFileItem) + strHexFilePath);	
	// method
	m_szCfgFile[CV_TstatLastFlashMethod] =(CString(c_strCfgLastFlashMethodItem) + FlashMethod );		
	// flash page
//	m_szCfgFile[CV_TstatLastFlashPage] =(CString(c_strCfgLastFlashTypePageItem) + strFlashTypePage);			
	// id
	m_szCfgFile[CV_TstatDeAddr] =(CString(c_strCfgDefaultAddrItem) + DeviceID );	
	// com port
	m_szCfgFile[CV_TstatDeCOM] =(CString(c_strCfgDefaultComItem) + COMPORT);	
	// Baudrate
	m_szCfgFile[CV_TstatDeBaudrate] =(CString(c_strCfgDefaultBaudrateItem) + Baudrate);
	m_szCfgFile[CV_NCDeIP] =(CString(c_strCfgDefaultIPItem) + IP);
	m_szCfgFile[CV_NCDeIPPort] =(CString(c_strCfgDefaultIPPortItem) + IPPort);

	m_szCfgFile[CV_SubNot] =(CString(c_strCfgNote) +CString(subnot));
	m_szCfgFile[CV_Sub_ID] =(CString(c_strCfgSubID) +CString(subID));

	WriteToCfgFile();

}
void CConfigFileHandler::ReadFromCfgFileForAll(
	CString& HexFileName,
	CString& FlashMethod,
	CString& DeviceID,
	CString& COMPORT,
	CString& BD,
	CString& IP,
	CString& IPPort,
	CString& subnote,
	CString& subID)
{
	ASSERT(m_pFile);
	  
	ReadFromCfgFile();
	HexFileName = m_szCfgFile[CV_TstatLastFlashFile];
	FlashMethod = m_szCfgFile[CV_TstatLastFlashMethod];
	DeviceID = m_szCfgFile[CV_TstatDeAddr];
	COMPORT = m_szCfgFile[CV_TstatDeCOM];	
	BD = m_szCfgFile[CV_TstatDeBaudrate];	
	IP=m_szCfgFile[CV_NCDeIP];
	IPPort=m_szCfgFile[CV_NCDeIPPort];
	subnote = m_szCfgFile[CV_SubNot];
	subID =  m_szCfgFile[CV_Sub_ID];
	// 取有效数据
	TCHAR c = ':';
	 
	HexFileName=HexFileName.Mid(HexFileName.Find(c)+2);
	FlashMethod=FlashMethod.Mid(FlashMethod.Find(c)+2);
	DeviceID=DeviceID.Mid(DeviceID.Find(c)+2);
	COMPORT=COMPORT.Mid(COMPORT.Find(c)+2);
	BD=BD.Mid(BD.Find(c)+2);
	IP=IP.Mid(IP.Find(c)+2);
	IPPort=IPPort.Mid(IPPort.Find(c)+2);
	subnote = subnote.Mid(subnote.Find(c)+2);
	subID = subID.Mid(subID.Find(c)+2);
}

void CConfigFileHandler::WriteToCfgFile()
{
	     setlocale(LC_CTYPE,("chs"));
		if(m_pFile->Open(m_strFilePath.GetString(),CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate ))
		{
			for (UINT i = 0; i < m_szCfgFile.size(); i++)
			{
				//TRACE(m_szCfgFile[i]);
				m_pFile->WriteString(m_szCfgFile[i]+_T("\n"));	
			}	

			m_pFile->Close();
		}
		else
		{
				ASSERT(0);
				m_pFile->Close();
				CString strTips = _T("├Cannot create a config file. Please try again.");
				 
		}
	
}


BOOL CConfigFileHandler::ReadFromCfgFile()
{
	char *old_locale=_strdup(setlocale(LC_CTYPE,NULL));
	setlocale(LC_CTYPE,"chs");

	BOOL bRet = CreateConfigFile(m_strFilePath);
	if (bRet)
	{
		if(m_pFile->Open(m_strFilePath.GetString(),CFile::modeReadWrite | CFile::shareDenyNone))
		{
			m_szCfgFile.clear();

			CString strRead;
			while(m_pFile->ReadString(strRead))
			{							
				TRACE(strRead);
				m_szCfgFile.push_back(strRead);
			}
			setlocale(LC_CTYPE,old_locale);
			free(old_locale);
			m_pFile->Close();

			return TRUE;
		}
		else
		{
			ASSERT(0);
			m_pFile->Close();
			CString strTips = _T("├Cannot create a new config file. Please try again.");
			 
			 
			return FALSE;
		}
	}
	else
		return FALSE;

}