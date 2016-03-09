#pragma once

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
protected:
	BOOL VerifyBinFile();
private:

	CString			m_strFileName;
     
	//int					m_nBufLen;
};
