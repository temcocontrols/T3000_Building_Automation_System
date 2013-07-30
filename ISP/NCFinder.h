#pragma once

class CNCFinder
{
public:
	CNCFinder(void);
	virtual ~CNCFinder(void);

	int GetFindNCIP(vector<DWORD>& szIPs);
	bool FindNC();

protected:
	DWORD GetIPFromBuf(char* pBuf, int nLen);
private:

	vector<DWORD>			m_szIPs;
};
