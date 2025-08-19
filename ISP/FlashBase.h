/********************************************************************
	created:	2012/01/11
	created:	11:1:2012   15:05
	filename: 	e:\MyWork\ISP\ISP 4.3\ISP\Header\FlashBase.h
	file path:	e:\MyWork\ISP\ISP 4.3\ISP\Header
	file base:	FlashBase
	file ext:	h
	author:		zgq
	
	purpose:	Flash function base class, providing commonly used interfaces. Different devices use different flash methods with different classes, but all inherit from here.
	purpose:	 flash功能的基类，提供一般常用的接口。各个设备不同的flash方式
				 使用不同的类，但都从这里继承。
*********************************************************************/


#pragma once

class CFlashBase
{
public:
	CFlashBase(void);
	virtual ~CFlashBase(void);

	// 获得flash file的绝对路径和文件名。  // Get the absolute path and filename of the flash file
	CString		GetFlashFileName();											
	void			SetFlashFileName(const CString& strFlieName);		// 

	// 
	int 			GetFileBuffer(char* pBuffer);
	char* 		CreateFileBuffer(int nSize);
	void			ReleaseFileBuffer();
		
	// 


	








	//////////////////////////////////////////////////////////////////////////
	// 需要重载的函数  // Functions that need to be overloaded
	//////////////////////////////////////////////////////////////////////////
	// 检查文件格式  // Check file format
	virtual BOOL FileValidation(const CString& strFileName);
	virtual BOOL Flash();

	




protected:
	CString		m_strFlashFileName;		// flash file name

	char*			m_pFileBuffer;				// 文件内容缓冲区，保存要刷新的文件  // File content buffer, saves files to be flashed
	int				m_nBufferSize;			// size

};
