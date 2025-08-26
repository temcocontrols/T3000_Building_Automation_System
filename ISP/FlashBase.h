/********************************************************************
	created:	2012/01/11
	created:	11:1:2012   15:05
	filename: 	e:\MyWork\ISP\ISP 4.3\ISP\Header\FlashBase.h
	file path:	e:\MyWork\ISP\ISP 4.3\ISP\Header
	file base:	FlashBase
	file ext:	h
	author:		zgq
	
	purpose:	Flash function base class, providing commonly used interfaces. Different devices use different flash methods with different classes, but all inherit from here.
	purpose:	 flash���ܵĻ��࣬�ṩһ�㳣�õĽӿڡ������豸��ͬ��flash��ʽ
				 ʹ�ò�ͬ���࣬����������̳С�
*********************************************************************/


#pragma once

class CFlashBase
{
public:
	CFlashBase(void);
	virtual ~CFlashBase(void);

	// ���flash file�ľ���·�����ļ�����  // Get the absolute path and filename of the flash file
	CString		GetFlashFileName();											
	void			SetFlashFileName(const CString& strFlieName);		// 

	// 
	int 			GetFileBuffer(char* pBuffer);
	char* 		CreateFileBuffer(int nSize);
	void			ReleaseFileBuffer();
		
	// 


	








	//////////////////////////////////////////////////////////////////////////
	// ��Ҫ���صĺ���  // Functions that need to be overloaded
	//////////////////////////////////////////////////////////////////////////
	// ����ļ���ʽ  // Check file format
	virtual BOOL FileValidation(const CString& strFileName);
	virtual BOOL Flash();

	




protected:
	CString		m_strFlashFileName;		// flash file name

	char*			m_pFileBuffer;				// �ļ����ݻ�����������Ҫˢ�µ��ļ�  // File content buffer, saves files to be flashed
	int				m_nBufferSize;			// size

};
