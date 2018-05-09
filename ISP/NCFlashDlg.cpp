// NCFlashDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ISP.h"
#include "NCFlashDlg.h"
#include "ISPDlg.h"
#include "globle_function.h"
#include "BinFileParser.h"
#include "TFTPServer.h"
extern bool auto_flash_mode;
// CNCFlashDlg dialog

IMPLEMENT_DYNAMIC(CNCFlashDlg, CDialog)

CNCFlashDlg::CNCFlashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNCFlashDlg::IDD, pParent)
{
	m_nFlashTypeFlag = 1;
	m_pTFTPServer = NULL;
}

CNCFlashDlg::~CNCFlashDlg()
{
	if (m_pTFTPServer)
	{
		delete m_pTFTPServer;
		m_pTFTPServer = NULL;
	}
}

void CNCFlashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_NC, m_IPAddr);
}


BEGIN_MESSAGE_MAP(CNCFlashDlg, CDialog)
	ON_MESSAGE(WM_REPLACE_STATUSINFO, OnReplaceStatusInfo)
	ON_MESSAGE(WM_ADD_STATUSINFO, OnAddStatusInfo)
	ON_MESSAGE(WM_FLASH_FINISH, OnFlashFinish)
	ON_BN_CLICKED(IDC_BUTTON_PING, &CNCFlashDlg::OnBnClickedButtonPing)
END_MESSAGE_MAP()


// CNCFlashDlg message handlers


BOOL CNCFlashDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  Add extra initialization here

	InitFromConfigFile();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CNCFlashDlg::FileValidation(const CString& strFileName)
{

//	BOOL CTstatFlashDlg::FileValidation(const CString& strFileName)
//	{
		if (m_nFlashTypeFlag==0 && !HexFileValidation(strFileName))
		{
			CString strTips;
			//strTips.Format(_T("%s isn't a HEX file."), strFileName);
			strTips = _T("To Update over Com port, please select a *.HEX file");
			if(!auto_flash_mode)
				AfxMessageBox(strTips);
			UpdateStatusInfo(strTips, FALSE);
			return FALSE;
		}
		else if(m_nFlashTypeFlag && !BinFileValidation(strFileName))
		{
			CString strTips;
			//strTips.Format(_T("%s isn't a BIN file."), strFileName);
			strTips = _T("To Updating over Ethernet, please select a *.BIN file");
			if(!auto_flash_mode)
				AfxMessageBox(strTips);
			UpdateStatusInfo(strTips, FALSE);
			return FALSE;
		}

		return TRUE;
	
}

void CNCFlashDlg::UpdateStatusInfo(const CString& strInfo, BOOL bReplace)
{
	((CISPDlg*)GetParent())->UpdateStatusInfo(strInfo, bReplace);	
}

void CNCFlashDlg::InitFromConfigFile()
{
	CString strTips = _T("Config file : ") + ((CISPDlg*)GetParent())->m_strCfgFilePath+ _T(".");
	UpdateStatusInfo(strTips, FALSE);

	//============
	CString strHexFileName;
	CString strFlashMethod;
	CString strIP;
	CString strIPPort;

	((CISPDlg*)GetParent())->m_cfgFileHandler.ReadNCPageConfig(
		strHexFileName,
		strFlashMethod,
		strIP,
		strIPPort);

	//============
	CWnd* pWnd = GetDlgItem(IDC_EDIT_NCPORT);
	pWnd->SetWindowText(strIPPort);

	pWnd = GetDlgItem(IDC_IPADDRESS_NC);
	pWnd->SetWindowText(strIP);
	

	m_strFlashFileName = strHexFileName;

	//UpdateData(false);
}

DWORD CNCFlashDlg::GetIPAddress()
{
	DWORD dwIP=0;
	m_IPAddr.GetAddress(dwIP);

	return dwIP;
}


BOOL CNCFlashDlg::Flash()
{
	m_strFlashFileName = ((CISPDlg*)(GetParent()))->GetFlashFileName();

	if(!FileValidation(m_strFlashFileName))
	{
		return FALSE;
	}

	if (m_nFlashTypeFlag == 1)
	{
		FlashByEthernet();
	}

	return TRUE;
}


void CNCFlashDlg::SaveToConfigFile(CConfigFileHandler& cfgFileHandler)
{
		// hex file path
		CString strHexFilePath = m_strFlashFileName;//((CISPDlg*)GetParent())->GetFlashFileName();
		
		CString strType = c_strFlashMethodEthernet;
		//
		CString strIP;
		m_IPAddr.GetWindowText(strIP);

		//
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_NCPORT);
		CString strPort;
		pEdit->GetWindowText(strPort);
		//		
		cfgFileHandler.WriteNCPageConfig(
			strHexFilePath,
			strType,
			strIP,
			strPort);
}

void CNCFlashDlg::FlashByEthernet()
{
	if (m_pTFTPServer)
	{
		delete m_pTFTPServer;
		m_pTFTPServer = NULL;
	}
	if (m_pFileBuffer)
	{
		delete[] m_pFileBuffer;
		m_pFileBuffer = NULL;
	}

	CBinFileParser* pBinFile = new CBinFileParser;
	pBinFile->SetBinFileName(m_strFlashFileName);
	m_pFileBuffer = new char[c_nBinFileBufLen];
	ZeroMemory(m_pFileBuffer, c_nBinFileBufLen);
	int nDataSize = pBinFile->GetBinFileBuffer(m_pFileBuffer, c_nBinFileBufLen);

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

		m_pTFTPServer->SetDataSource((BYTE*)m_pFileBuffer, nDataSize);
		//pServer->SetServerInfoWnd((CWnd*)(&(pDlg->m_ctlServerInfo)));
		m_pTFTPServer->FlashByEthernet();
		// disable flash button
		((CISPDlg*)GetParent())->EnableFlash(FALSE);	
	}
	else
	{
		CString strTips1 =_T("Error: The file is not a properly formatted BIN file.");
		UpdateStatusInfo(strTips1, FALSE);
		CString strTips2 =_T("Please reselect a right file.");
		UpdateStatusInfo(strTips2, FALSE);
		if(!auto_flash_mode)
			AfxMessageBox(strTips1+strTips2, MB_OK);
	}

	delete pBinFile;
}



afx_msg LRESULT CNCFlashDlg::OnAddStatusInfo(WPARAM wParam, LPARAM lParam)
{
	CString strInfo = CString(LPTSTR(lParam));
	UpdateStatusInfo(strInfo, false);
	WCHAR* szTemp = (LPTSTR(lParam));
	delete [] szTemp;



	//UpdateStatusInfo(g_strFlashInfo, false);
	return 1;
}

afx_msg LRESULT CNCFlashDlg::OnReplaceStatusInfo(WPARAM wParam, LPARAM lParam)
{
	CString strInfo = CString(LPTSTR(lParam));
	UpdateStatusInfo(strInfo, true);
	WCHAR* szTemp = (LPTSTR(lParam));
	delete[] szTemp;

	//UpdateStatusInfo(g_strFlashInfo, true);
	return 1;
}

LRESULT CNCFlashDlg::OnFlashFinish(WPARAM wParam, LPARAM lParam)
{
	int nRet = lParam; // 线程中有消息弹出，这里就不弹了

	// flash完了，释放资源

	if (m_pTFTPServer)
	{
		delete m_pTFTPServer;
		m_pTFTPServer = NULL;
	}
	if (m_pFileBuffer)
	{
		delete[] m_pFileBuffer;
		m_pFileBuffer = NULL;
	}

	//	CWnd* pBtn = GetDlgItem(IDC_BUTTON_FLASH);
	//	pBtn->EnableWindow(TRUE);

	((CISPDlg*)GetParent())->EnableFlash(TRUE);	
	return 1;
}

void CNCFlashDlg::OnBnClickedButtonPing()
{
	// TODO: Add your control notification handler code here
	CString strIP;
	GetDlgItem(IDC_IPADDRESS_NC)->GetWindowText(strIP);

	if (strIP.GetLength() <= 6)
	{
		if(!auto_flash_mode)
			AfxMessageBox(_T("Please Input a right IP address."));
		return;
	}
	((CISPDlg*)GetParent())->OnTestPing(strIP);	
}
