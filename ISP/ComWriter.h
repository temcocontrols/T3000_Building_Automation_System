/********************************************************************
	created:	2011/04/08
	created:	8:4:2011   9:59
	filename: 	e:\MyWork\ISP\ISP 4.3\ISP\ComWriter.h
	file path:	e:\MyWork\ISP\ISP 4.3\ISP
	file base:	ComWriter
	file ext:	h
	author:		zgq
	purpose:	ͨ��������¼���ࡣ��Ҫ��TStatʹ�á�
*********************************************************************/
#pragma once

#include "HexFileParser.h"
#include "globle_function.h"


class CComWriter
{
public:
	CComWriter(void);
	virtual ~CComWriter(void);
	
	//////////////////////////////////////////////////////////////////////////
	// ���������洰��ָ��
	void SetParentWnd(CWnd* pWnd);
	//////////////////////////////////////////////////////////////////////////
	// ��hex�ļ��ж�ȡ����Ч���ݣ��ڴ�ռ䣬���ݳ���
	void SetFileBuffer(TS_UC* pBuf, int nLen);
	//////////////////////////////////////////////////////////////////////////
	// ���ô��ں�
	void SetComPortNO(int nComPortNO);
	//////////////////////////////////////////////////////////////////////////
	// ����ID
	void SetModbusID(vector<int>& szMdbIds);
	//////////////////////////////////////////////////////////////////////////
	// ���ò�����
	void SetBautrate(int nBautrate);

	//////////////////////////////////////////////////////////////////////////
	// ����hex �ļ���ʽ����
	void SetHexFileType(HEXFILE_FORMAT nHexFileType);
	//////////////////////////////////////////////////////////////////////////
	// ������չhex �ļ��Ĳ�����
	void SetExtendHexFileParam(vector<int>& szFlags, TS_UC* pBuf);
	//////////////////////////////////////////////////////////////////////////
	// �����ӡ״̬��Ϣ
	void OutPutsStatusInfo(const CString& strInfo, BOOL bReplace=FALSE);
	//////////////////////////////////////////////////////////////////////////
	// ��ʼд����
	int BeginWirteByCom();
	int WirteExtendHexFileByCom();

	void WriteFinish(int nFlashFlag);
	//***************************Release Func*********************//
  //  void Show_Device_Information(int comport ,int ID);
	//////////////////////////////////////////////////////////////////////////
	// ��������flash
	BOOL EndFlash();


	//////////////////////////////////////////////////////////////////////////
	// ��ʼдhex��ͨ��TCP
	int BeginWirteByTCP();
	void SetIPAddr(const CString& strIPAddr);
	void SetIPPort(int nIPPort);
protected:
	BOOL WriteCommandtoReset();
	BOOL StopWrite();

public:
		HEXFILE_FORMAT	m_nHexFileType;
		CWnd*			m_pParentWnd;
		TS_UC*			m_pFileBuffer;
		int					m_nBufLen;

		int					m_nComPort;
		 
		int					m_nBautrate;			// ������

		CWinThread*	m_pWorkThread;
		vector<int>		m_szMdbIDs;			// ���е���Ҫflash��Modbus ID
		
		BOOL				m_bStopWrite;			// =false - write ; =true - stop write
		TS_UC*			m_pExtendFileBuffer;// ���ڱ�����չ�ļ���ʽ��hex �ļ��Ļ�������
		vector<int>		m_szHexFileFlags;		// ���е�Hex�ļ��εı�ǣ�ÿ��Ԫ�ر�ʾÿ��hex��չ�ļ���ʽ�Ķε���Ч�ֽڵĳ��ȡ�

		CString			m_strIPAddr;
		int				m_nIPPort;

};
