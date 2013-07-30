/********************************************************************
	created:	2012/01/11
	created:	11:1:2012   15:05
	filename: 	e:\MyWork\ISP\ISP 4.3\ISP\Header\FlashBase.h
	file path:	e:\MyWork\ISP\ISP 4.3\ISP\Header
	file base:	FlashBase
	file ext:	h
	author:		zgq
	
	purpose:	 flash���ܵĻ��࣬�ṩһ�㳣�õĽӿڡ������豸��ͬ��flash��ʽ
				 ʹ�ò�ͬ���࣬����������̳С�
*********************************************************************/


#pragma once

class CFlashBase
{
public:
	CFlashBase(void);
	virtual ~CFlashBase(void);

	// ���flash file�ľ���·�����ļ�����
	CString		GetFlashFileName();											
	void			SetFlashFileName(const CString& strFlieName);		// 

	// 
	int 			GetFileBuffer(char* pBuffer);
	char* 		CreateFileBuffer(int nSize);
	void			ReleaseFileBuffer();
		
	// 


	








	//////////////////////////////////////////////////////////////////////////
	// ��Ҫ���صĺ���
	//////////////////////////////////////////////////////////////////////////
	// ����ļ���ʽ
	virtual BOOL FileValidation(const CString& strFileName);
	virtual BOOL Flash();

	




protected:
	CString		m_strFlashFileName;		// flash file name

	char*			m_pFileBuffer;				// �ļ����ݻ�����������Ҫˢ�µ��ļ�
	int				m_nBufferSize;			// size

};
