// LightCtrlFlashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ISP.h"
#include "LightCtrlFlashDlg.h"
#include "global_function.h"
#include "ISPDlg.h"
#include "HexFileParser.h"
#include "BinFileParser.h"
#include "ComWriter.h"
#include "TCPFlasher.h"
//#include "TFTPServer.h"

// CLightCtrlFlashDlg dialog

IMPLEMENT_DYNAMIC(CLightCtrlFlashDlg, CDialog)

CLightCtrlFlashDlg::CLightCtrlFlashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightCtrlFlashDlg::IDD, pParent)
{
	m_nFlashLCType = 0;

	m_pTCPFlasher = NULL;
	m_pTFTPServer = NULL;

	m_pFileBuf = NULL;

}

CLightCtrlFlashDlg::~CLightCtrlFlashDlg()
{
	if(m_pTCPFlasher)
	{
		delete m_pTCPFlasher;
	}

	if(m_pTFTPServer)
	{
		delete m_pTFTPServer;
	}

	if (m_pFileBuf)
	{
		delete m_pFileBuf;
	}
}

void CLightCtrlFlashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_LC, m_IPAddrLC);
}


BEGIN_MESSAGE_MAP(CLightCtrlFlashDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_MAIN, &CLightCtrlFlashDlg::OnBnClickedRadioMain)
	ON_BN_CLICKED(IDC_RADIO_INPUT, &CLightCtrlFlashDlg::OnBnClickedRadioInput)
	ON_BN_CLICKED(IDC_OUTPUT, &CLightCtrlFlashDlg::OnBnClickedOutput)
	ON_MESSAGE(WM_REPLACE_STATUSINFO, OnReplaceStatusInfo)
	ON_MESSAGE(WM_ADD_STATUSINFO, OnAddStatusInfo)
	ON_MESSAGE(WM_FLASH_FINISH, OnFlashFinish)
END_MESSAGE_MAP()


// CLightCtrlFlashDlg message handlers

void CLightCtrlFlashDlg::OnBnClickedRadioMain()
{
	m_nFlashLCType = 0;	
	GetDlgItem(IDC_EDIT_MDBID)->EnableWindow(FALSE);
}

void CLightCtrlFlashDlg::OnBnClickedRadioInput()
{
	m_nFlashLCType = 1;	
	GetDlgItem(IDC_EDIT_MDBID)->EnableWindow(TRUE);
}

void CLightCtrlFlashDlg::OnBnClickedOutput()
{
	m_nFlashLCType = 2;
	GetDlgItem(IDC_EDIT_MDBID)->EnableWindow(TRUE);
}




void CLightCtrlFlashDlg::FlashByEthernet()
{
	if (m_pTFTPServer)
	{
		delete m_pTFTPServer;
		m_pTFTPServer = NULL;
	}
	if (m_pFileBuf)
	{
		delete[] m_pFileBuf;
		m_pFileBuf = NULL;
	}

	CBinFileParser* pBinFile = new CBinFileParser;
	pBinFile->SetBinFileName(m_strFlashFileName);
	m_pFileBuf = new char[c_nBinFileBufLen];
	ZeroMemory(m_pFileBuf, c_nBinFileBufLen);
	int nDataSize = pBinFile->GetBinFileBuffer(m_pFileBuf, c_nBinFileBufLen);

	if(nDataSize > 0)
	{
		CString strTips = _T("Firmware bin file verified okay.");
		UpdateStatusInfo(strTips, FALSE);

		m_pTFTPServer=new TFTPServer;
		DWORD dwIP = GetIPAddress();
		//m_IPAddr.GetAddress(dwIP);


		m_pTFTPServer->SetParentDlg(this);
		m_pTFTPServer->SetClientPort(69);
		m_pTFTPServer->SetClientIP(dwIP);
		m_pTFTPServer->SetFileName(m_strFlashFileName);

		m_pTFTPServer->SetDataSource((BYTE*)m_pFileBuf, nDataSize);
		//pServer->SetServerInfoWnd((CWnd*)(&(pDlg->m_ctlServerInfo)));
		m_pTFTPServer->FlashByEthernet();
		// disable flash button
		((CISPDlg*)GetParent())->EnableFlash(FALSE);	
	}
	else
	{
		CString strTips1 =_T("The file is not a properly formatted BIN file.");
		UpdateStatusInfo(strTips1, FALSE);
		CString strTips2 =_T("Please reselect a right file.");
		UpdateStatusInfo(strTips2, FALSE);
		AfxMessageBox(strTips1+strTips2, MB_OK);
	}

	delete pBinFile;
}


afx_msg LRESULT CLightCtrlFlashDlg::OnAddStatusInfo(WPARAM wParam, LPARAM lParam)
{
	CString strInfo = CString(LPTSTR(lParam));
	UpdateStatusInfo(strInfo, false);
	WCHAR* szTemp = (LPTSTR(lParam));
	delete [] szTemp;



	//UpdateStatusInfo(g_strFlashInfo, false);
	return 1;
}

afx_msg LRESULT CLightCtrlFlashDlg::OnReplaceStatusInfo(WPARAM wParam, LPARAM lParam)
{
	CString strInfo = CString(LPTSTR(lParam));
	UpdateStatusInfo(strInfo, true);
	WCHAR* szTemp = (LPTSTR(lParam));
	delete[] szTemp;

	//UpdateStatusInfo(g_strFlashInfo, true);
	return 1;
}

LRESULT CLightCtrlFlashDlg::OnFlashFinish(WPARAM wParam, LPARAM lParam)
{
	int nRet = lParam; // 线程中有消息弹出，这里就不弹了

	// flash完了，释放资源

	if (m_pTCPFlasher)
	{
		delete m_pTCPFlasher;
		m_pTCPFlasher = NULL;
	}
	if (m_pTFTPServer)
	{
		delete m_pTFTPServer;
		m_pTFTPServer = NULL;
	}
	if (m_pFileBuf)
	{
		delete[] m_pFileBuf;
		m_pFileBuf = NULL;
	}

	//	CWnd* pBtn = GetDlgItem(IDC_BUTTON_FLASH);
	//	pBtn->EnableWindow(TRUE);

	((CISPDlg*)GetParent())->EnableFlash(TRUE);	
	return 1;
}

BOOL CLightCtrlFlashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	InitFromConfigFile();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



DWORD CLightCtrlFlashDlg::GetIPAddress()
{
	DWORD dwIP=0;
	m_IPAddrLC.GetAddress(dwIP);

	return dwIP;
}

CString CLightCtrlFlashDlg::GetIPAddressStr()
{
	CString strIP;
	m_IPAddrLC.GetWindowText(strIP);

	return strIP;
}


void CLightCtrlFlashDlg::InitFromConfigFile()
{
	//-----------------------------------------------------------------------------------------------------------
	//CString strTips = _T("Config file : ") + ((CISPDlg*)GetParent())->m_strCfgFilePath+ _T(".");
	//UpdateStatusInfo(strTips, FALSE);

	//============
	CString strHexFileName;
	CString strFlashType;
	CString strIP;
	CString strIPPort;
	CString strMDBID;

	((CISPDlg*)GetParent())->m_cfgFileHandler.ReadLCPageConfig(
		strHexFileName,
		strFlashType,
		strIP,
		strIPPort,
		strMDBID);

	//============

	m_strFlashFileName = strHexFileName;
	
	CWnd* pWnd = GetDlgItem(IDC_EDIT_LCIPPORT);
	pWnd->SetWindowText(strIPPort);

	pWnd = GetDlgItem(IDC_IPADDRESS_LC);
	pWnd->SetWindowText(strIP);

	pWnd = GetDlgItem(IDC_EDIT_MDBID);
	pWnd->SetWindowText(strMDBID);

	if (strFlashType.CompareNoCase(CString(c_strCfgLastFlashLCTypeMain)) == 0)
	{
		CButton* pRadio1= (CButton*)GetDlgItem(IDC_RADIO_MAIN);
		pRadio1->SetCheck(1);
		m_nFlashLCType = 0;
		GetDlgItem(IDC_EDIT_MDBID)->EnableWindow(FALSE);
	}
	else if (strFlashType.CompareNoCase(CString(c_strCfgLastFlashLCTypeInput)) == 0)
	{
		CButton* pRadio1= (CButton*)GetDlgItem(IDC_RADIO_INPUT);
		pRadio1->SetCheck(1);
		m_nFlashLCType = 1;
	}	
	else if (strFlashType.CompareNoCase(CString(c_strCfgLastFlashLCTypeOutput)) == 0)
	{
		CButton* pRadio1= (CButton*)GetDlgItem(IDC_RADIO_OUTPUT);
		pRadio1->SetCheck(1);
		m_nFlashLCType = 2;
	}
	else 
	{
		CButton* pRadio1= (CButton*)GetDlgItem(IDC_RADIO_MAIN);
		pRadio1->SetCheck(1);
	}

	//UpdateData(false);
}


void CLightCtrlFlashDlg::SaveToConfigFile(CConfigFileHandler& cfgFileHandler)
{
	// hex file path
	CString strHexFilePath = m_strFlashFileName;//((CISPDlg*)GetParent())->GetFlashFileName();

	//
	CString strIP;
	m_IPAddrLC.GetWindowText(strIP);
	//
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_LCIPPORT);
	CString strPort;
	pEdit->GetWindowText(strPort);

	CString strType = GetFlashTypeStr();
	// 
// 	CString strFlashPage;
// 	strFlashPage = GetCurSelPageStr();

	//		
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MDBID);
	CString strMDBID;
	pEdit->GetWindowText(strMDBID);

	cfgFileHandler.WriteLCPageConfig(strHexFilePath,
		strType,
		strIP,
		strPort,
		strMDBID);

}




////////////////////////////////////////////////////////////////////////////
// 参数 BOOL, =TRUE replace the current line, =FALSE add a new line
void CLightCtrlFlashDlg::UpdateStatusInfo(const CString& strInfo, BOOL bReplace)
{
	((CISPDlg*)GetParent())->UpdateStatusInfo(strInfo, bReplace);	
}




CString CLightCtrlFlashDlg::GetFlashTypeStr()
{
	CString strType;
	if ( m_nFlashLCType == 0)
	{
		strType = CString(c_strCfgLastFlashLCTypeMain);
	}
	else if( m_nFlashLCType == 1)
	{
		strType = CString(c_strCfgLastFlashLCTypeInput);
	}
	else if( m_nFlashLCType == 2)
	{
		strType = CString(c_strCfgLastFlashLCTypeOutput);
	}

	return strType;
}
	

BOOL CLightCtrlFlashDlg::FileValidation(const CString& strFileName)
{
	if (m_nFlashLCType!=0 && !HexFileValidation(strFileName))
	{
		CString strTips;
		//strTips.Format(_T("%s isn't a HEX file."), strFileName);
		strTips = _T("To update Input or Output board, please select a *.HEX file");

		AfxMessageBox(strTips);
		UpdateStatusInfo(strTips, FALSE);
		return FALSE;
	}
	else if(m_nFlashLCType == 0 && !BinFileValidation(strFileName))
	{
		CString strTips;
		//strTips.Format(_T("%s isn't a BIN file."), strFileName);
		strTips = _T("To update Main controller board , please select a *.BIN file");
		AfxMessageBox(strTips);
		UpdateStatusInfo(strTips, FALSE);
		return FALSE;
	}

	return TRUE;

	
}

BOOL CLightCtrlFlashDlg::Flash()
{
// 	AfxMessageBox(_T("Sorry, Updating firmware for Lighting Controller is still developing."));
// 
// 	return FALSE;
	m_strFlashFileName = ((CISPDlg*)(GetParent()))->GetFlashFileName();
	if(!FileValidation(m_strFlashFileName))
	{
		return FALSE;
	}


	if (m_nFlashLCType == 0)  // main board 
	{
			FlashByEthernet();
	}
	else if(m_nFlashLCType == 1)  // input board
	{
		if(!ValidMdbIDString())
		{
			return FALSE;
		}
		GetModbusID(m_szMdbIDs);
		OnFlashInputBoard();
	}
	else if(m_nFlashLCType == 2)  // output board
	{
		if(!ValidMdbIDString())
		{
			return FALSE;
		}
		GetModbusID(m_szMdbIDs);
		OnFlashOutputBoard();
	}

	return TRUE;
}



void CLightCtrlFlashDlg::FlashMainBoard()
{
	m_strFlashFileName = ((CISPDlg*)(GetParent()))->GetFlashFileName();

	if(FileValidation(m_strFlashFileName))
	{
		FlashByEthernet();
	}
}




void CLightCtrlFlashDlg::OnFlashInputBoard()
{
/*
	m_strFlashFileName = ((CISPDlg*)(GetParent()))->GetFlashFileName();
	
	if (m_pTCPFlasher)
	{
		delete m_pTCPFlasher;
		m_pTCPFlasher = NULL;
	}

	if (m_pFileBuf)
	{
		delete[] m_pFileBuf;
		m_pFileBuf= NULL;
	}

	CHexFileParser* pHexFile = new CHexFileParser;
	pHexFile->SetFileName(m_strFlashFileName);

	m_pFileBuf = new char[c_nHexFileBufLen];
	memset(m_pFileBuf, 0xFF, c_nHexFileBufLen);
	int nDataSize = pHexFile->GetHexFileBuffer(m_pFileBuf, c_nHexFileBufLen);

	if(nDataSize > 0)
	{
		CString strTips = _T("Hex file verified okay.");
		UpdateStatusInfo(strTips, FALSE);

		m_pTCPFlasher = new CTCPFlasher;

		m_pTCPFlasher->SetParentWnd(this);
		m_pTCPFlasher->SetIPAddr(GetIPAddress());
		CString strIPPort;
		GetDlgItem(IDC_EDIT_LCIPPORT)->GetWindowText(strIPPort);
		int nIPPort = _wtoi(strIPPort);
		m_pTCPFlasher->SetIPPort(nIPPort);		
		m_pTCPFlasher->SetFileBuffer(m_pFileBuf, nDataSize);

		BOOL  nRet = m_pTCPFlasher->StartFlash();

		// disable flash button
		if (nRet) // 表示开始写了
		{
			((CISPDlg*)GetParent())->EnableFlash(FALSE);
		}
	}
	else
	{
		CString strTips1 =_T("File error: The file is not a properly formatted HEX file.");
		UpdateStatusInfo(strTips1, FALSE);
		CString strTips2 =_T("Please reselect a right file.");
		UpdateStatusInfo(strTips2, FALSE);
		AfxMessageBox(strTips1+strTips2, MB_OK);

	}
	delete pHexFile;
*/		
}



void CLightCtrlFlashDlg::OnFlashOutputBoard()
{
	m_strFlashFileName = ((CISPDlg*)(GetParent()))->GetFlashFileName();

	if (m_pTCPFlasher)
	{
		delete m_pTCPFlasher;
		m_pTCPFlasher = NULL;
	}

	if (m_pFileBuf)
	{
		delete[] m_pFileBuf;
		m_pFileBuf= NULL;
	}

	CHexFileParser* pHexFile = new CHexFileParser;
	pHexFile->SetFileName(m_strFlashFileName);

	m_pFileBuf = new char[c_nHexFileBufLen];
	memset(m_pFileBuf, 0xFF, c_nHexFileBufLen);
	int nDataSize = pHexFile->GetHexFileBuffer(m_pFileBuf, c_nHexFileBufLen);


	//////////////////////////////////////////////////////////////////////////
	if(nDataSize > 0)
	{
		CString strTips = _T("Hex file verified okay.");
		UpdateStatusInfo(strTips, FALSE);

	//////////////////////////////////////////////////////////////////////////
	

		m_pTCPFlasher = new CComWriter;
		m_pTCPFlasher->SetParentWnd(this);
		m_pTCPFlasher->SetIPAddr(GetIPAddressStr());
		CString strIPPort;
		GetDlgItem(IDC_EDIT_LCIPPORT)->GetWindowText(strIPPort);
		int nIPPort = _wtoi(strIPPort);
		m_pTCPFlasher->SetIPPort(nIPPort);		

		m_pTCPFlasher->SetModbusID(m_szMdbIDs);
		
		m_pTCPFlasher->SetFileBuffer((TS_UC*)m_pFileBuf, nDataSize);
		
		m_pTCPFlasher->BeginWirteByTCP();

	}
	else
	{
		CString strTips1 =_T("File error: The file is not a properly formatted HEX file.");
		UpdateStatusInfo(strTips1, FALSE);
		CString strTips2 =_T("Please reselect a right file.");
		UpdateStatusInfo(strTips2, FALSE);
		AfxMessageBox(strTips1+strTips2, MB_OK);

	}

	delete pHexFile;
}

// 验证输入的Modbus ID是有效的
BOOL CLightCtrlFlashDlg::ValidMdbIDString()
{
	CString strModbusID;
	GetDlgItem(IDC_EDIT_MDBID)->GetWindowText(strModbusID);

	for(int i = 0; i < strModbusID.GetLength(); i++)
	{
		if(!(strModbusID.GetAt(i) >='0' && strModbusID.GetAt(i)<='9'))
		{
			if (strModbusID.GetAt(i) != ';')
			{
				CString strTips =_T("Error: Modbus ID invalidation."); 
				UpdateStatusInfo(strTips, FALSE);
				AfxMessageBox(strTips);
				return FALSE;	
			}			
		}
	}
	return TRUE;
}


int CLightCtrlFlashDlg::GetModbusID(vector<int>& szMdbIDs)
{
	szMdbIDs.clear();
	TCHAR c = ';';
	CString strSrc;
	GetDlgItem(IDC_EDIT_MDBID)->GetWindowText(strSrc);
	int nPos =0;
	while(strSrc.GetLength() > 0)
	{
		nPos = strSrc.Find(c);

		if (nPos != 0)
		{
			if (nPos < 0)
			{
				nPos = strSrc.GetLength();
			}
			CString strID = strSrc.Left(nPos);
			int nID = _wtoi(strID);//.GetString());

			if (!(nID > 0 && nID <= 255))
			{
				CString strTips;
				strTips.Format(_T("Error : Wrong Modbus ID : %d. Please input right ID."), nID);
				UpdateStatusInfo(strTips, FALSE);
				AfxMessageBox(strTips);
				return FALSE;
			}
			szMdbIDs.push_back(nID);
		}
		// 		else if(nPos == 0) // 在第一位
		// 		{
		// 		}

		if (nPos < 0 || (nPos == strSrc.GetLength())) // 找不到或者位于最后一位
		{
			return TRUE;
		}
		strSrc = strSrc.Mid(nPos+1);
	}

	return TRUE;
}