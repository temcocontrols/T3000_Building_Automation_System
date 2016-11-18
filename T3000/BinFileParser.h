#pragma once
#include "global_struct.h"
class CBinFileParser
{
public:
	CBinFileParser(void);
	~CBinFileParser(void);


	void SetBinFileName(const CString& strFileName);
	int GetBinFileBuffer(char* pBuf, int nBufLen);
    CString GetBinInfor();
    CString         m_strASIX;
    CString         m_strProductName;
	Bin_Info        global_fileInfor;
protected:
	BOOL VerifyBinFile();
private:

	CString			m_strFileName;
     
	//int					m_nBufLen;
};
