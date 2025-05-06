/********************************************************************
	created:	2011/04/08
	created:	8:4:2011   9:59
	filename: 	e:\MyWork\ISP\ISP 4.3\ISP\ComWriter.h
	file path:	e:\MyWork\ISP\ISP 4.3\ISP
	file base:	ComWriter
	file ext:	h
	author:		zgq
	purpose:	TStat
*********************************************************************/
#pragma once

#include "HexFileParser.h"
#include "global_function.h"


class CComWriter
{
public:
	CComWriter(void);
	virtual ~CComWriter(void);
	
	//////////////////////////////////////////////////////////////////////////
	// 
	void SetParentWnd(CWnd* pWnd);
	//////////////////////////////////////////////////////////////////////////
	// hex
	void SetFileBuffer(TS_UC* pBuf, int nLen);
	//////////////////////////////////////////////////////////////////////////
	// 
	void SetComPortNO(int nComPortNO);
	//////////////////////////////////////////////////////////////////////////
	// ID
	void SetModbusID(vector<int>& szMdbIds);
	//////////////////////////////////////////////////////////////////////////
	// 
	void SetBautrate(int nBautrate);

	//////////////////////////////////////////////////////////////////////////
	// hex 
	void SetHexFileType(HEXFILE_FORMAT nHexFileType);
	//////////////////////////////////////////////////////////////////////////
	// hex 
	void SetExtendHexFileParam(vector<int>& szFlags, TS_UC* pBuf);
	//////////////////////////////////////////////////////////////////////////
	// 
	void OutPutsStatusInfo(const CString& strInfo, BOOL bReplace=FALSE);
	//////////////////////////////////////////////////////////////////////////
	// 
	int BeginWirteByCom();
	int WirteExtendHexFileByCom();
    int WirteExtendHexFileByCom_RAM();
	void WriteFinish(int nFlashFlag);
	//***************************Release Func*********************//
  //  void Show_Device_Information(int comport ,int ID);
	//////////////////////////////////////////////////////////////////////////
	// flash
	BOOL EndFlash();
    int UpdataDeviceInformation(int& ID);
	BOOL UpdataDeviceInformation_ex(unsigned short device_productID);
	//////////////////////////////////////////////////////////////////////////
	// hexTCP
	int BeginWirteByTCP();
	void SetIPAddr(const CString& strIPAddr);
	void SetIPPort(int nIPPort);
    void SetHexInfor(Bin_Info temp);
protected:
	BOOL WriteCommandtoReset();
	BOOL StopWrite();

public:
Bin_Info m_hexinfor;
		HEXFILE_FORMAT	m_nHexFileType;
		CWnd*			m_pParentWnd;
		TS_UC*			m_pFileBuffer;
        char*			m_pFileRepairBuffer;
        CString					m_strRepairHexFileName; // ;
		int					m_nBufLen;
        int                 Is_Ram;
		int					m_nComPort;
		 
		int					m_nBautrate;			// 
        int                 m_index_Baudrate;
		int                 m_FlashTimes;
		CString m_hexbinfilepath;
		CWinThread*	m_pWorkThread;
		vector<int>		m_szMdbIDs;			// flashModbus ID
		int             m_com_flash_binfile;
		BOOL				m_bStopWrite;			// =false - write ; =true - stop write
		TS_UC*			m_pExtendFileBuffer;// hex 
		vector<int>		m_szHexFileFlags;		// Hexhex

		CString			m_strIPAddr;
		int				m_nIPPort;
		int             continue_com_flash_count; // ; 88  ESP32 ;
		unsigned short  update_firmware_info[6];
		int             m_subnet_flash; //flash
};
