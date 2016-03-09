#include "StdAfx.h"
#include "FlashBase.h"

CFlashBase::CFlashBase(void)
{
	m_pFileBuffer = NULL;
	m_nBufferSize = 0;
}

CFlashBase::~CFlashBase(void)
{
	ReleaseFileBuffer();
}


void CFlashBase::ReleaseFileBuffer()
{
	if (m_pFileBuffer)
	{
		delete []m_pFileBuffer;
		m_pFileBuffer = NULL;
	}
}

CString CFlashBase::GetFlashFileName()
{
	return m_strFlashFileName;
}


void CFlashBase::SetFlashFileName(const CString& strFlieName)
{
	m_strFlashFileName = strFlieName;
}


int CFlashBase::GetFileBuffer(char* pBuffer)
{
	pBuffer = m_pFileBuffer;
	return m_nBufferSize;
}

char* CFlashBase::CreateFileBuffer(int nSize)
{
	ASSERT(nSize > 0);
	if (m_pFileBuffer)
	{
		delete []m_pFileBuffer;
		m_pFileBuffer=NULL;
	}
	
	
	m_pFileBuffer = new char[nSize];
	
	return m_pFileBuffer;
}

BOOL CFlashBase::FileValidation(const CString& strFileName)
{
	return FALSE;
}

BOOL CFlashBase::Flash()
{
	return FALSE;
}