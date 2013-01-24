#include "StdAfx.h"
#include "ConfigFileWR.h"
#include "Global_Struct.h"
extern CString  g_strExePth;
CConfigFileWR::CConfigFileWR(void)
{

		 

	m_pFile = new CStdioFile;

}

CConfigFileWR::~CConfigFileWR(void)
{
	m_szCfgFile.clear();
	if (m_pFile)
	{
		//m_pFile->Close();
		delete m_pFile;
	}
}
BOOL CConfigFileWR::CreateConfigFile(const CString& strFilePath){


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

			// 
			m_pFile->WriteString( CString(c_strCfgTstatSection) + _T("\n"));	
			m_pFile->WriteString( CString(c_strCfgDefaultInstalldirItem) + g_strExePth+ _T("\n"));	

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
			m_pFile->WriteString(CString(c_strCfgDeviceName));

		}
		else
		{
			CString strTips = _T("Cannot create a new config file. Please try again.");
		/*	((CISPDlg*)m_pParentWnd)->UpdateStatusInfo(strTips, FALSE);*/

			m_pFile->Close();

			return FALSE;
		}
	}
	m_pFile->Close();
	return TRUE;
}
BOOL CConfigFileWR::ReadFromCfgFile()
{

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

			m_pFile->Close();
			return TRUE;
		}
		else
		{
			ASSERT(0);
 
			return FALSE;
		}
	}
	else
		return FALSE;
}
void CConfigFileWR::WriteToCfgFile(){
   
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
		CString strTips = _T("Cannot create a config file. Please try again.");
	 
 
	}
}
void CConfigFileWR::SetConfigFilePath(const CString& strFilePath)
{
	ASSERT(strFilePath.GetLength() > 0);
	m_strFilePath = strFilePath;
}