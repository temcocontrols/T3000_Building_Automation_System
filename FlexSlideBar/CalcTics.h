#pragma once

//////////////////////////////////////////////////////////////////////////
// 


class CCalcTics
{
public:
	CCalcTics(void);
	~CCalcTics(void);

	CCalcTics(int nLength, int nNum);
	void SetLength(int nLength);
	void SetTicNum(int nNum);

	int GetTicArray(vector<int>& szTics);
	int CalcTicArray(vector<int>& szTics);

protected:
	
	int								m_nLength;
	int								m_nTicNum;

	vector<int>				m_szTics;

};
