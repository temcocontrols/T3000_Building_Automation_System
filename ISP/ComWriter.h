/********************************************************************
	created:	2011/04/08
	created:	8:4:2011   9:59
	filename: 	e:\MyWork\ISP\ISP 4.3\ISP\ComWriter.h
	file path:	e:\MyWork\ISP\ISP 4.3\ISP
	file base:	ComWriter
	file ext:	h
	author:		zgq
	purpose:	Class for flashing through serial port. Mainly used by TStat.
	purpose:	ͨ��������¼���ࡣ��Ҫ��TStatʹ�á�
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
	// Set main window pointer
	// ���������洰��ָ��
	void SetParentWnd(CWnd* pWnd);
	//////////////////////////////////////////////////////////////////////////
	// Valid content read from hex file, memory space, content length
	// ��hex�ļ��ж�ȡ����Ч���ݣ��ڴ�ռ䣬���ݳ���
	void SetFileBuffer(TS_UC* pBuf, int nLen);
	//////////////////////////////////////////////////////////////////////////
	// Set serial port number
	// ���ô��ں�
	void SetComPortNO(int nComPortNO);
	//////////////////////////////////////////////////////////////////////////
	// Set ID
	// ����ID
	void SetModbusID(vector<int>& szMdbIds);
	//////////////////////////////////////////////////////////////////////////
	// Set baud rate
	// ���ò�����
	void SetBautrate(int nBautrate);

	//////////////////////////////////////////////////////////////////////////
	// Set hex file format type
	// ����hex �ļ���ʽ����
	void SetHexFileType(HEXFILE_FORMAT nHexFileType);
	//////////////////////////////////////////////////////////////////////////
	// Set extended hex file parameters
	// ������չhex �ļ��Ĳ�����
	void SetExtendHexFileParam(vector<int>& szFlags, TS_UC* pBuf);
	//////////////////////////////////////////////////////////////////////////
	// Output print status information
	// �����ӡ״̬��Ϣ
	void OutPutsStatusInfo(const CString& strInfo, BOOL bReplace=FALSE);
	//////////////////////////////////////////////////////////////////////////
	// Start writing to serial port
	// ��ʼд����
	int BeginWirteByCom();
	int WirteExtendHexFileByCom();
    int WirteExtendHexFileByCom_RAM();
	void WriteFinish(int nFlashFlag);
	//***************************Release Func*********************//
  //  void Show_Device_Information(int comport ,int ID);
	//////////////////////////////////////////////////////////////////////////
	// End flash immediately
	// ��������flash
	BOOL EndFlash();
	int Fix_Tstat10_76800_baudrate();
    int UpdataDeviceInformation(int& ID);
	BOOL UpdataDeviceInformation_ex(unsigned short device_productID);
	//////////////////////////////////////////////////////////////////////////
	// Start writing hex through TCP
	// ��ʼдhex��ͨ��TCP
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
        // Repair firmware path
        CString					m_strRepairHexFileName; // �޸��Ĺ̼�·��;
		int					m_nBufLen;
        int                 Is_Ram;
		int					m_nComPort;
		 
		// Baud rate
		int					m_nBautrate;			// ������
        int                 m_index_Baudrate;
		int                 m_FlashTimes;
		CString m_hexbinfilepath;
		CWinThread*	m_pWorkThread;
		// All Modbus IDs that need to be flashed
		vector<int>		m_szMdbIDs;			// ���е���Ҫflash��Modbus ID
		int             m_com_flash_binfile;
		BOOL				m_bStopWrite;			// =false - write ; =true - stop write
		// Buffer for storing extended file format hex files
		TS_UC*			m_pExtendFileBuffer;// ���ڱ�����չ�ļ���ʽ��hex �ļ��Ļ�������
		// All hex file segment markers, each element represents the valid byte length of each hex extended file format segment
		vector<int>		m_szHexFileFlags;		// ���е�Hex�ļ��εı�ǣ�ÿ��Ԫ�ر�ʾÿ��hex��չ�ļ���ʽ�Ķε���Ч�ֽڵĳ��ȡ�

		CString			m_strIPAddr;
		int				m_nIPPort;
		// Continue burning from how many packets; currently only supports product 88 ESP32 chip boards
		int             continue_com_flash_count; // �����Ӷ��ٰ���ʼ����; Ŀǰֻ֧�ֲ�ƷΪ88 �� ESP32 оƬ�İ���;
		unsigned short  update_firmware_info[6];
		// Whether it is a flash sub-device
		int             m_subnet_flash; //�Ƿ���flash���豸
};
