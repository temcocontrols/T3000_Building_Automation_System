// TstatFlashDlg.cpp : implementation file
//

#include "stdafx.h"
#include <fstream>
#include "ISP.h"
#include "TstatFlashDlg.h"
#include "global_function.h"
#include "ISPDlg.h"
#include "HexFileParser.h"
#include "BinFileParser.h"
#include "ComWriter.h"
#include "TFTPServer.h"
#include "FlashSN.h"

// CTstatFlashDlg dialog
extern bool auto_flash_mode;
extern CString g_strFlashInfo;
 

IMPLEMENT_DYNAMIC(CTstatFlashDlg, CDialog)

CTstatFlashDlg::CTstatFlashDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTstatFlashDlg::IDD, pParent)
{
	m_pComWriter = NULL;
//	m_pTFTPServer = NULL;
	m_nFlashTypeFlag = 0;
 
}

CTstatFlashDlg::~CTstatFlashDlg()
{
	if (m_pComWriter)
	{
		delete m_pComWriter;
		m_pComWriter = NULL;
	}

// 	if (m_pFileBuf)
// 	{
// 		delete []m_pFileBuf;
// 		m_pFileBuf = NULL;
// 	}
// 	if (m_pTFTPServer)
// 	{
// 		delete m_pTFTPServer;
// 		m_pTFTPServer = NULL;
// 	}

}

void CTstatFlashDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM, m_cbxCom);
}


BEGIN_MESSAGE_MAP(CTstatFlashDlg, CDialog)

	ON_MESSAGE(WM_REPLACE_STATUSINFO, OnReplaceStatusInfo)
	ON_MESSAGE(WM_ADD_STATUSINFO, OnAddStatusInfo)
	ON_MESSAGE(WM_FLASH_FINISH, OnFlashFinish)
END_MESSAGE_MAP()


// CTstatFlashDlg message handlers
BOOL CTstatFlashDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	InitCombox();

	InitFromConfigFile();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void CTstatFlashDlg::InitCombox()
{
	vector<CString> szComms;
	BOOL bRet = GetSerialComm_ISP(szComms);
	if (bRet && szComms.size()>0)
	{
		m_cbxCom.Clear();
		for (UINT i = 0; i < szComms.size(); i++)
		{
			m_cbxCom.AddString(szComms[i]);
		}
	}
	else  // can not get com port name from register, then add com port manually
	{
		CString strTips = _T("Can not find any Com port on your computer.");
		UpdateStatusInfo(strTips, FALSE);
		m_cbxCom.Clear();
		for (UINT i = 1; i < 9; i++)
		{
			CString strCom;
			strCom.Format(_T("COM%d"), i);
			m_cbxCom.AddString(strCom);
		}
	}
}



void CTstatFlashDlg::InitFromConfigFile()
{
	//-----------------------------------------------------------------------------------------------------------

	m_cbxCom.SetCurSel(0);
	//m_strCfgFilePath=m_strExePath + c_strCfgFileName;
	CString strTips = _T("Config file : ") + ((CISPDlg*)GetParent())->m_strCfgFilePath+ _T(".");
	UpdateStatusInfo(strTips, FALSE);

	//============
	CString strHexFileName;
	CString strFlashMethod;
	CString strFlashTypePage;
	CString strModbusID;
	CString strComPort;
	CString strBaudrate;
	CString strIP;
	CString strIPPort;

	((CISPDlg*)GetParent())->m_cfgFileHandler.ReadTStatPageConfig(
		strHexFileName,
		strFlashMethod,
		strFlashTypePage,
		strModbusID,
		strComPort,
		strBaudrate);

	//============


// pWnd = GetDlgItem(IDC_EDIT_FILEPATH);
// pWnd->SetWindowText(strHexFileName);
	if(strFlashTypePage.CompareNoCase(CString(c_strCfgTstatSection)) == 0)
	{
		((CISPDlg*)GetParent())->SetFlashFileName(strHexFileName);
		//((CISPDlg*)GetParent())->SetCurTabSel(0);
		m_strFlashFileName = strHexFileName;
	}
	else if(strFlashTypePage.CompareNoCase(CString(c_strCfgNCSection)) == 0)
	{
		//((CISPDlg*)GetParent())->//SetCurTabSel(1);
		
	}
	else if(strFlashTypePage.CompareNoCase(CString(c_strCfgLightCtrlSection)) == 0)
	{
		//((CISPDlg*)GetParent())->SetCurTabSel(2);

	}
	

// 	CWnd* pWnd = GetDlgItem(IDC_EDIT_PORT);
// 	pWnd->SetWindowText(strIPPort);

// 	pWnd = GetDlgItem(IDC_IPADDRESS1);
// 	pWnd->SetWindowText(strIP);

	CWnd*pWnd = GetDlgItem(IDC_EDIT_MDBID);
	pWnd->SetWindowText(strModbusID);

	pWnd = GetDlgItem(IDC_COMBO_COM);
	pWnd->SetWindowText(strComPort);

	// 	pWnd = GetDlgItem(IDC_COMBO_BAUDRATE);
	// 	pWnd->SetWindowText(m_strBaudrate);
	pWnd = GetDlgItem(IDC_EDIT_BAUDRATE);
	pWnd->SetWindowText(strBaudrate);


	m_strFlashFileName = strHexFileName;

	//UpdateData(false);

}


////////////////////////////////////////////////////////////////////////////
// 参数 BOOL, =TRUE replace the current line, =FALSE add a new line
void CTstatFlashDlg::UpdateStatusInfo(const CString& strInfo, BOOL bReplace)
{
	((CISPDlg*)GetParent())->UpdateStatusInfo(strInfo, bReplace);	
}




int CTstatFlashDlg::GetFlashType()
{
	return m_nFlashTypeFlag;
}


int CTstatFlashDlg::GetModbusID(vector<int>& szMdbIDs)
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
				if(!auto_flash_mode)
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

		
int	CTstatFlashDlg::GetComPortNo()
{
	CString strComPort; 
	GetDlgItem(IDC_COMBO_COM)->GetWindowText(strComPort);
	CString c = strComPort.Mid(3);
	int nPort = _wtoi (c.GetBuffer());
	return nPort;
}

CString CTstatFlashDlg::GetIPAddressStr()
{
	CString strIP;
	GetDlgItem(IDC_IPADDRESS1)->GetWindowText(strIP);

	return strIP;	
}

DWORD CTstatFlashDlg::GetIPAddress()
{
	DWORD dwIP=0;


	return dwIP;
}

CString CTstatFlashDlg::GetIPPort()
{	
	CString strPort;
	GetDlgItem(IDC_EDIT_PORT)->GetWindowText(strPort);

	return strPort;	
}

void CTstatFlashDlg::SaveToConfigFile(CConfigFileHandler& cfgFileHandler)
{
		// hex file path
		CString strHexFilePath = m_strFlashFileName;//((CISPDlg*)GetParent())->GetFlashFileName();
		
		// method
 		CString strType;
// 		CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_SELCOM);		
// 		
// 		if (pBtn->GetCheck() > 0) // ethernet
		{	
			strType = c_strFlashMethodCom;				
		}
// 		else
// 		{
// 			strType = c_strFlashMethodEthernet;
// 		}

		// id
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MDBID);		
		CString strMID;
		pEdit->GetWindowText(strMID);
		strMID.Trim();
		strMID.TrimLeft(_T("0"));
	
		// com port
		CString strComPort;
		m_cbxCom.GetWindowText(strComPort);
		
		// Baudrate
		CString strBaudrate = _T("19200");
		//
		//CString strIP;
		//m_IPAddr.GetWindowText(strIP);
		//
		//pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PORT);
		//CString strPort;
		//pEdit->GetWindowText(strPort);

		// 
		CString strFlashPage;
		strFlashPage = GetCurSelPageStr();
		
		//		
		cfgFileHandler.WriteTStatPageConfig(strHexFilePath,
			strType,
			strFlashPage,
			strMID,
			strComPort,
			strBaudrate);
		
}


BOOL CTstatFlashDlg::Flash()
{
	//GetFlashParameter();


	m_strFlashFileName = ((CISPDlg*)(GetParent()))->GetFlashFileName();

	if(!FileValidation(m_strFlashFileName))
	{
		return FALSE;
	}

	if (m_nFlashTypeFlag == 0)
	{
		if(!ValidMdbIDString())
		{
			return FALSE;
		}

		if(!GetModbusID(m_szMdbIDs))
		{
			return FALSE;		
		}

	//	FlashSN();
		FlashByCom();
	}
	else
	{

	}

	return TRUE;
}

BOOL CTstatFlashDlg::FileValidation(const CString& strFileName)
{
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


// 验证输入的Modbus ID是有效的
BOOL CTstatFlashDlg::ValidMdbIDString()
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
				if(!auto_flash_mode)
					AfxMessageBox(strTips);
				return FALSE;	
			}			
		}
	}
	return TRUE;
}



void CTstatFlashDlg::FlashByCom()
{
	if (m_pComWriter)
	{
		delete m_pComWriter;
		m_pComWriter = NULL;
	}

	if (m_pFileBuffer)
	{
		delete[] m_pFileBuffer;
		m_pFileBuffer= NULL;
	}

	CHexFileParser* pHexFile = new CHexFileParser;
	pHexFile->SetFileName(m_strFlashFileName);

	m_pFileBuffer = new char[c_nHexFileBufLen];
	memset(m_pFileBuffer, 0xFF, c_nHexFileBufLen);
	int nDataSize = pHexFile->GetHexFileBuffer(m_pFileBuffer, c_nHexFileBufLen);

	if(nDataSize > 0)
	{
		CString strTips = _T("Hex file verified okay.");
		UpdateStatusInfo(strTips, FALSE);

		m_pComWriter = new CComWriter;
		m_pComWriter->SetModbusID(m_szMdbIDs);

		m_pComWriter->SetHexFileType(pHexFile->GetHexFileFormatType());

		CString strBaudrate; GetDlgItem(IDC_EDIT_BAUDRATE)->GetWindowText(strBaudrate);
		int nBautrate = _wtoi (strBaudrate.GetBuffer());
		m_pComWriter->SetBautrate(nBautrate);
		int nPort = GetComPortNo();
		m_pComWriter->SetParentWnd(this);

		m_pComWriter->SetComPortNO(nPort);
		if(pHexFile->GetHexFileFormatType() == HEXFILE_DATA)
		{
			m_pComWriter->SetFileBuffer((TS_UC*)m_pFileBuffer, nDataSize);
		}
		else if(pHexFile->GetHexFileFormatType() == HEXFILE_LINERADDR)
		{
			vector<int> szFlags;
			pHexFile->GetExtendHexFileSectionFlag(szFlags);
			m_pComWriter->SetExtendHexFileParam(szFlags, (TS_UC*)m_pFileBuffer);
		}

		int nRet = m_pComWriter->BeginWirteByCom();

		// disable flash button
		if (nRet != 0) // 表示开始写了
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
		if(!auto_flash_mode)
			AfxMessageBox(strTips1+strTips2, MB_OK);

	}
	delete pHexFile;
}



afx_msg LRESULT CTstatFlashDlg::OnAddStatusInfo(WPARAM wParam, LPARAM lParam)
{
	CString strInfo = CString(LPTSTR(lParam));
	UpdateStatusInfo(strInfo, false);
	WCHAR* szTemp = (LPTSTR(lParam));
	delete [] szTemp;



	//UpdateStatusInfo(g_strFlashInfo, false);
 	return 1;
}

afx_msg LRESULT CTstatFlashDlg::OnReplaceStatusInfo(WPARAM wParam, LPARAM lParam)
{
	CString strInfo = CString(LPTSTR(lParam));
	UpdateStatusInfo(strInfo, true);
 	WCHAR* szTemp = (LPTSTR(lParam));
 	delete[] szTemp;

	//UpdateStatusInfo(g_strFlashInfo, true);
 	return 1;
}

LRESULT CTstatFlashDlg::OnFlashFinish(WPARAM wParam, LPARAM lParam)
{
	int nRet = lParam; // 线程中有消息弹出，这里就不弹了

	// flash完了，释放资源
	if (m_pComWriter)
	{
		delete m_pComWriter;
		m_pComWriter = NULL;
	}
	if (m_pFileBuffer)
	{
		delete[] m_pFileBuffer;
		m_pFileBuffer = NULL;
	}

//	CWnd* pBtn = GetDlgItem(IDC_BUTTON_FLASH);
//	pBtn->EnableWindow(TRUE);
		
	((CISPDlg*)GetParent())->EnableFlash(TRUE);	

	//FlashSN();
	if (((CISPDlg*)GetParent())->m_bShowSN	)
	{
		FlashSN();
	}
	
	return 1;
}


CString CTstatFlashDlg::GetCurSelPageStr()
{	
	return ((CISPDlg*)GetParent())->GetCurSelPageStr();

}




void CTstatFlashDlg::FlashSN()
{
	CFlashSN* pFlashSN = new CFlashSN;
	int nComport = GetComPortNo();

	pFlashSN->SetComport(nComport);
	pFlashSN->SetFlashTpye(0);

	int nHWVerison;
//	int nProductModel;
	CString  strProductModel;
    int ID;
	((CISPDlg*)GetParent())->GetFlashSNParam(nHWVerison, strProductModel,ID);



	pFlashSN->SetFlashParam((static_cast<const CString>(strProductModel)), nHWVerison,ID,0,_T(""));

	pFlashSN->StartWriteSN();

	
	delete pFlashSN;
}
