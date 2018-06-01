#include "StdAfx.h"
#include "ConfigFileHandler.h"
#include "Global_Struct.h"
#include "ISPDlg.h"
#include <locale.h>
#include "global_function.h"
extern CString g_strExePath; 

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


	 	m_configfile_path=GetExePath(true)+_T("T3000_config.ini");

		m_ISPTool_Section = L"ISPTool_Setting";
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

void CConfigFileHandler::SetParentWnd(CWnd* pWnd)
{
	m_pParentWnd = pWnd;
}

void CConfigFileHandler::SetConfigFilePath(const CString& strFilePath)
{
	ASSERT(strFilePath.GetLength() > 0);
	m_strFilePath = strFilePath;
}



// Create config file
// If already exists don¡¯t create a new one
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
			m_pFile->WriteString( CString(c_strCfgLastFlashFileItem) + strHexFilePath);	
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
			CString strTips = _T(" Cannot create a new config file. Please try again.");
			((CISPDlg*)m_pParentWnd)->UpdateStatusInfo(strTips, FALSE);
			
			m_pFile->Close();

			return FALSE;
		}
	}
	m_pFile->Close();
	return TRUE;

}

//Config ini
BOOL CConfigFileHandler::CreateConfigFile()
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
				 CFileFind fFind;
                    if(!fFind.FindFile(m_configfile_path))
                    {
						WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgLastFlashFileItem),strHexFileName,m_configfile_path);
						WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgLastFlashMethodItem),strFlashMethod,m_configfile_path);
						WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultAddrItem),strModbusID,m_configfile_path);
						WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultComItem),strComPort,m_configfile_path);
						WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultIPItem),strIP,m_configfile_path);
						WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultIPPortItem),strIPPort,m_configfile_path);
						WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultBaudrateItem),strBaudrate,m_configfile_path);
						WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgNote),strSub_not,m_configfile_path);
						WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgSubID),strSub_ID,m_configfile_path);
						 
                    }
// 					else
// 					{
// 					}
                     
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





	WriteToCfgFile();


}

void CConfigFileHandler::ReadTStatPageConfig(
	CString& strHexFileName,
	CString& strFlashMethod,
	CString& strFlashTypePage,
	CString& strModbusID,
	CString& strComPort,
	CString& strBaudrate)
{
	ASSERT(m_pFile);
	{
		ReadFromCfgFile();
			
			strHexFileName = m_szCfgFile[CV_TstatLastFlashFile];
			strFlashMethod = m_szCfgFile[CV_TstatLastFlashMethod];
			strFlashTypePage = m_szCfgFile[CV_TstatLastFlashPage];
			strModbusID = m_szCfgFile[CV_TstatDeAddr];
			strComPort = m_szCfgFile[CV_TstatDeCOM];	
			strBaudrate = m_szCfgFile[CV_TstatDeBaudrate];	
// 			strIP = m_szCfgFile[8];
// 			strIPPort = m_szCfgFile[9];		
		
			// Take valid data
			TCHAR c = ':';
			strHexFileName = strHexFileName.Mid(strHexFileName.Find(c)+2);
			strFlashMethod = strFlashMethod.Mid(strFlashMethod.Find(c)+2);
			strFlashTypePage = strFlashTypePage.Mid(strFlashTypePage.Find(c)+2);
			strModbusID = strModbusID.Mid(strModbusID.Find(c)+2);
			strComPort = strComPort.Mid(strComPort.Find(c)+2);
			strBaudrate = strBaudrate.Mid(strBaudrate.Find(c)+2);
		
// 			strIP = strIP.Mid(strIP.Find(c)+2);
// 			strIPPort = strIPPort.Mid(strIPPort.Find(c)+2);
		}

}


void CConfigFileHandler::WriteNCPageConfig(const CString& strFlashFileName,
										   const CString& strFlashType,
										   const CString& strIP,
										   const CString& strIPPort)
{
	ASSERT(m_pFile);

	//m_szCfgFile[10] =(CString(c_strCfgLightCtrlSection) + _T("\n"));	
	// hex file path
	CString strHexFilePath = strFlashFileName;
	// section
	m_szCfgFile[CV_NC_Section] = (CString(c_strCfgNCSection));
	// file
	m_szCfgFile[CV_NCLastFlashFile] = (CString(c_strCfgLastFlashFileItem) + strHexFilePath);
	// type
	m_szCfgFile[CV_NCLastFlashMethod] = (CString(c_strCfgLastFlashMethodItem) + strFlashType);				
	// IP
	m_szCfgFile[CV_NCDeIP] = (CString(c_strCfgDefaultIPItem) + strIP);
	// Port
	m_szCfgFile[CV_NCDeIPPort] = (CString( c_strCfgDefaultIPPortItem) + strIPPort);

	WriteToCfgFile();

}


void CConfigFileHandler::ReadNCPageConfig( CString& strFlashFileName,
										  CString& strFlashType,
										  CString& strIP,
										  CString& strIPPort)
{
	ASSERT(m_pFile);

	ReadFromCfgFile();
	strFlashFileName = m_szCfgFile[CV_NCLastFlashFile];
	strFlashType = m_szCfgFile[CV_NCLastFlashMethod];
	strIP = m_szCfgFile[CV_NCDeIP];
	strIPPort = m_szCfgFile[CV_NCDeIPPort];	

	// Take Valid Data
	TCHAR c = ':';
	strFlashFileName = strFlashFileName.Mid(strFlashFileName.Find(c)+2);
	strFlashType = strFlashType.Mid(strFlashType.Find(c)+2);
	strIP = strIP.Mid(strIP.Find(c)+2);
	strIPPort = strIPPort.Mid(strIPPort.Find(c)+2);
}




void CConfigFileHandler::WriteLCPageConfig(const CString& strFlashFileName,
				   const CString& strFlashType,
				   const CString& strIP,
				   const CString& strIPPort,
				   const CString& strMDBID)
{
	ASSERT(m_pFile);
		
	//m_szCfgFile[10] =(CString(c_strCfgLightCtrlSection) + _T("\n"));	
	// hex file path
	CString strHexFilePath = strFlashFileName;
	m_szCfgFile[CV_LCLastFlashFile] = (CString(c_strCfgLastFlashFileItem) + strHexFilePath);
	// type
	m_szCfgFile[CV_LCLastFlashType] = (CString(c_strCfgLastFlashLCTypeItem) + strFlashType);				
	// IP
	m_szCfgFile[CV_LCDeIP] = (CString(c_strCfgDefaultIPItem) + strIP);
	// Port
	m_szCfgFile[CV_LCDeIPPort] = (CString( c_strCfgDefaultIPPortItem) + strIPPort);
	// MDB ID
	m_szCfgFile[CV_LCDMDBID] = (CString( c_strCfgLastFlashMDBIDItem) + strMDBID);
	WriteToCfgFile();
	
}


void CConfigFileHandler::ReadLCPageConfig(	CString& strFlashFileName,
					  CString& strFlashType,
					  CString& strIP,
					  CString& strIPPort,
					  CString& strMDBID)
{
	ASSERT(m_pFile);

		    ReadFromCfgFile();
			strFlashFileName = m_szCfgFile[CV_LCLastFlashFile];
			strFlashType = m_szCfgFile[CV_LCLastFlashType];
			strIP = m_szCfgFile[CV_LCDeIP];
			strIPPort = m_szCfgFile[CV_LCDeIPPort];	
			strMDBID = m_szCfgFile[CV_LCDMDBID];	

			// Take valid data
			TCHAR c = ':';
			strFlashFileName = strFlashFileName.Mid(strFlashFileName.Find(c)+2);
			strFlashType = strFlashType.Mid(strFlashType.Find(c)+2);
			strIP = strIP.Mid(strIP.Find(c)+2);
			strIPPort = strIPPort.Mid(strIPPort.Find(c)+2);
			strMDBID = strMDBID.Mid(strMDBID.Find(c)+2);
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

	
	WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgLastFlashFileItem),HexFileName,m_configfile_path);
	WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgLastFlashMethodItem),FlashMethod,m_configfile_path);
	WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultAddrItem),DeviceID,m_configfile_path);
	WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultComItem),COMPORT,m_configfile_path);
	WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultIPItem),IP,m_configfile_path);
	WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultIPPortItem),IPPort,m_configfile_path);
	WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultBaudrateItem),Baudrate,m_configfile_path);
	WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgNote),subnot,m_configfile_path);
	WritePrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgSubID),subID,m_configfile_path);
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
	//ASSERT(m_pFile);
	  
	//ReadFromCfgFile();
	GetPrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgLastFlashFileItem),L"",HexFileName.GetBuffer(MAX_PATH),MAX_PATH,m_configfile_path);
	HexFileName.ReleaseBuffer();
	GetPrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgLastFlashMethodItem),L"",FlashMethod.GetBuffer(MAX_PATH),MAX_PATH,m_configfile_path);
	FlashMethod.ReleaseBuffer();
	GetPrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultAddrItem),L"",DeviceID.GetBuffer(MAX_PATH),MAX_PATH,m_configfile_path);
	DeviceID.ReleaseBuffer();
	GetPrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultComItem),L"",COMPORT.GetBuffer(MAX_PATH),MAX_PATH,m_configfile_path);
	COMPORT.ReleaseBuffer();
	GetPrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultIPItem),L"",IP.GetBuffer(MAX_PATH),MAX_PATH,m_configfile_path);
	IP.ReleaseBuffer();
	GetPrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultIPPortItem),L"",IPPort.GetBuffer(MAX_PATH),MAX_PATH,m_configfile_path);
	IPPort.ReleaseBuffer();
	GetPrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgDefaultBaudrateItem),L"",BD.GetBuffer(MAX_PATH),MAX_PATH,m_configfile_path);
	BD.ReleaseBuffer();
	GetPrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgNote),L"",subnote.GetBuffer(MAX_PATH),MAX_PATH,m_configfile_path);
	subnote.ReleaseBuffer();
	GetPrivateProfileStringW(m_ISPTool_Section,CString(c_strCfgSubID),L"",subID.GetBuffer(MAX_PATH),MAX_PATH,m_configfile_path);
	subID.ReleaseBuffer();


}

int CConfigFileHandler::GetFlashTimes()
{
	int FlashTimes = GetPrivateProfileInt(m_ISPTool_Section,CString(L"EnableTestFlash"),0,m_configfile_path);
	if(FlashTimes == 0)
	{
		WritePrivateProfileStringW(m_ISPTool_Section,_T("EnableTestFlash"),_T("0"),m_configfile_path);
	}

	return FlashTimes;
}
void CConfigFileHandler::WriteToCfgFile()
{
	     setlocale(LC_CTYPE,("chs"));
		if(m_pFile->Open(m_strFilePath.GetString(),CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate ))
		{
			for (UINT i = 0; i < m_szCfgFile.size(); i++)
			{
				TRACE(m_szCfgFile[i]);
				m_pFile->WriteString(m_szCfgFile[i]+_T("\n"));	
			}	

			m_pFile->Close();
		}
		else
		{
				ASSERT(0);
				m_pFile->Close();
				CString strTips = _T(" Cannot create a config file. Please try again.");
				//UpdateStatusInfo(strTips, FALSE);
				((CISPDlg*)m_pParentWnd)->UpdateStatusInfo(strTips, FALSE);
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
			CString strTips = _T(" Cannot create a new config file. Please try again.");
			//UpdateStatusInfo(strTips, FALSE);
			((CISPDlg*)m_pParentWnd)->UpdateStatusInfo(strTips, FALSE);
			return FALSE;
		}
	}
	else
		return FALSE;

}







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Manipulating the product model file, database of all preconfigured products and their properties

int CConfigFileHandler::GetProductModel(const CString& strPMFileName, map<int, CString>& szPMMap)
{
	CStdioFile file;
	if (!file.Open(strPMFileName.GetString(),CStdioFile::modeRead))
	{
		if(CreatePMFile(file, strPMFileName))
		{
			ReadPMFile(file, szPMMap);
			file.Close();
		}
		else
		{
			return -1;
		}
	}
	else
	{
		ReadPMFile(file, szPMMap);

		file.Close();
	}

	return szPMMap.size();
}



BOOL CConfigFileHandler::CreatePMFile(CStdioFile& file, const CString& strPMFileName)
{
	if (file.Open(strPMFileName.GetString(),CStdioFile::modeCreate|CStdioFile::modeReadWrite))
	{
		for(int i = 0; i < c_nModelNum; i++)
		{
			CString strModel = g_ProductModel[i];

			file.WriteString(strModel+_T("\n"));
		}	
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}


int CConfigFileHandler::ReadPMFile(CStdioFile& file,  map<int, CString>& szPMMap)
{
	TCHAR cFlag= '=';

	szPMMap.clear();

	CString strLine;
	CString strModel;
	CString strModelValue;

	while(file.ReadString(strLine))
	{
		int nPos = strLine.Find(cFlag);
		if (nPos <= 0)
		{
			continue;
		}
		strModel = strLine.Mid(0, nPos);
		strModelValue = strLine.Mid(nPos+1);
		int nValue = _wtoi(strModelValue);
		szPMMap[nValue] =strModel ;
	}
	
	return szPMMap.size();

}


//////////////////////////////////////////////////////////////////////////
void CConfigFileHandler::GetFlashSNParam(const CString& strFileName, int &nHWVersion, int & nModelValue)
{
	CStdioFile file;
	if (!file.Open(strFileName.GetString(),CStdioFile::modeRead))
	{
		if(CreateFlashSNFile(file, strFileName))
		{
			ReadFlashSNFile(file, nHWVersion, nModelValue);
			file.Close();
		}
		else
		{
			return;
		}
	}
	else
	{
		ReadFlashSNFile(file,  nHWVersion, nModelValue);

		file.Close();
	}
}

BOOL CConfigFileHandler::CreateFlashSNFile(CStdioFile& file, const CString& strFileName)
{
	if (file.Open(strFileName.GetString(),CStdioFile::modeCreate|CStdioFile::modeReadWrite))
	{
		file.WriteString(CString(c_strFlashHWVersion) + _T("1\n"));	
		file.WriteString(CString(c_strFlashSNModel) + _T("1\n"));	
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}


void CConfigFileHandler::ReadFlashSNFile(CStdioFile& file, int &nHWVersion, int & nModelValue)
{
	CString strTemp;
	TCHAR c = ':';	
	
	file.ReadString(strTemp);
	CString strHWV = strTemp.Mid(strTemp.Find(c)+2);
	file.ReadString(strTemp);
	nHWVersion = _wtoi(strHWV);
	CString strModelValue = strTemp.Mid(strTemp.Find(c)+2);
	nModelValue = _wtoi(strModelValue);
}


void CConfigFileHandler::SaveFlashSNParam(const CString& strFileName, int &nHWVersion, int & nModelValue)
{
	CStdioFile file;
	if (file.Open(strFileName, CStdioFile::modeCreate|CStdioFile::modeNoTruncate | CStdioFile::modeReadWrite))
	{
		CString strTemp;
		strTemp.Format(_T("%d"), nHWVersion);
		file.WriteString(CString(c_strFlashHWVersion) + strTemp + _T("\n"));	
		strTemp.Format(_T("%d"), nModelValue);
		file.WriteString(CString(c_strFlashSNModel) + strTemp + _T("\n"));	

		file.Close();
	}	
}



//////////////////////////////////////////////////////////////////////////
// mac addr

void CConfigFileHandler::GetUnusedMacAddr(const CString& strFileName, BYTE* szMac)
{

}

void CConfigFileHandler::SetUsedMacAddr(const CString& strFileName, BYTE* szMac)
{

}
