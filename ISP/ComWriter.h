/********************************************************************
	created:	2011/04/08
	created:	8:4:2011   9:59
	filename: 	e:\MyWork\ISP\ISP 4.3\ISP\ComWriter.h
	file path:	e:\MyWork\ISP\ISP 4.3\ISP
	file base:	ComWriter
	file ext:	h
	author:		zgq
	purpose:	Class for flashing through serial port. Mainly used by TStat.
	purpose:	通过串口烧录的类。主要是TStat使用。
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
	// 设置主界面窗口指针
	void SetParentWnd(CWnd* pWnd);
	//////////////////////////////////////////////////////////////////////////
	// Valid content read from hex file, memory space, content length
	// 从hex文件中读取的有效内容，内存空间，内容长度
	void SetFileBuffer(TS_UC* pBuf, int nLen);
	//////////////////////////////////////////////////////////////////////////
	// Set serial port number
	// 设置串口号
	void SetComPortNO(int nComPortNO);
	//////////////////////////////////////////////////////////////////////////
	// Set ID
	// 设置ID
	void SetModbusID(vector<int>& szMdbIds);
	//////////////////////////////////////////////////////////////////////////
	// Set baud rate
	// 设置波特率
	void SetBautrate(int nBautrate);

	//////////////////////////////////////////////////////////////////////////
	// Set hex file format type
	// 设置hex 文件格式类型
	void SetHexFileType(HEXFILE_FORMAT nHexFileType);
	//////////////////////////////////////////////////////////////////////////
	// Set extended hex file parameters
	// 设置扩展hex 文件的参数。
	void SetExtendHexFileParam(vector<int>& szFlags, TS_UC* pBuf);
	//////////////////////////////////////////////////////////////////////////
	// Output print status information
	// 输出打印状态信息
	void OutPutsStatusInfo(const CString& strInfo, BOOL bReplace=FALSE);
	//////////////////////////////////////////////////////////////////////////
	// Start writing to serial port
	// 开始写串口
	int BeginWirteByCom();
	int WirteExtendHexFileByCom();
    int WirteExtendHexFileByCom_RAM();
	void WriteFinish(int nFlashFlag);
	//***************************Release Func*********************//
  //  void Show_Device_Information(int comport ,int ID);
	//////////////////////////////////////////////////////////////////////////
	// End flash immediately
	// 立即结束flash
	BOOL EndFlash();
	int Fix_Tstat10_76800_baudrate();
    int UpdataDeviceInformation(int& ID);
	BOOL UpdataDeviceInformation_ex(unsigned short device_productID);
	//////////////////////////////////////////////////////////////////////////
	// Start writing hex through TCP
	// 开始写hex，通过TCP
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
        CString					m_strRepairHexFileName; // 修复的固件路径;
		int					m_nBufLen;
        int                 Is_Ram;
		int					m_nComPort;
		 
		// Baud rate
		int					m_nBautrate;			// 波特率
        int                 m_index_Baudrate;
		int                 m_FlashTimes;
		CString m_hexbinfilepath;
		CWinThread*	m_pWorkThread;
		// All Modbus IDs that need to be flashed
		vector<int>		m_szMdbIDs;			// 所有的需要flash的Modbus ID
		int             m_com_flash_binfile;
		BOOL				m_bStopWrite;			// =false - write ; =true - stop write
		// Buffer for storing extended file format hex files
		TS_UC*			m_pExtendFileBuffer;// 用于保存扩展文件格式的hex 文件的缓冲区。
		// All hex file segment markers, each element represents the valid byte length of each hex extended file format segment
		vector<int>		m_szHexFileFlags;		// 所有的Hex文件段的标记，每个元素表示每个hex扩展文件格式的段的有效字节的长度。

		CString			m_strIPAddr;
		int				m_nIPPort;
		// Continue burning from how many packets; currently only supports product 88 ESP32 chip boards
		int             continue_com_flash_count; // 继续从多少包开始烧入; 目前只支持产品为88 的 ESP32 芯片的板子;
		unsigned short  update_firmware_info[6];
		// Whether it is a flash sub-device
		int             m_subnet_flash; //是否是flash子设备
};
