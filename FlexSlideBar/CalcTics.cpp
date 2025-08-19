#include "StdAfx.h"
#include "CalcTics.h"
#include <algorithm>

CCalcTics::CCalcTics(void)
{
}

CCalcTics::~CCalcTics(void)
{
}


CCalcTics::CCalcTics(int nLength, int nNum)
{
	SetLength(nLength);
	SetTicNum(nNum);
	CalcTicArray(m_szTics);
}



void CCalcTics::SetLength(int nLength)
{
	m_nLength = nLength;
}


void CCalcTics::SetTicNum(int nNum)
{
	m_nTicNum = nNum;
}

int CCalcTics::GetTicArray(vector<int>& szTics)
{
	szTics.insert(szTics.end(), m_szTics.begin(), m_szTics.end());
	return szTics.size();
}


int CCalcTics::CalcTicArray(vector<int>& szTics)
{
	int nLow = 0;
	int nHigh = 0;

	int nLowNum = 0;
	int nHighNum = 0;


	nLow = m_nLength/m_nTicNum;
	nHighNum = m_nLength - m_nTicNum*nLow;
	nHigh = nLow + 1;

	nLowNum = m_nTicNum - nHighNum;

	//////////////////////////////////////////////////////////////////////////
	// Get distribution model, two scale sizes insert intervals, excess scales distributed at both ends
	// ��÷ֲ����ģ�ͣ����̶ֿȳߴ������������Ŀ̶ȷֲ�������
	ASSERT(nHighNum + nLowNum == m_nTicNum);
	
	int nGap=0;
	int nStartNum, nEndNum;
	// End value, whether the ends should use small or large values
	int nEndValue = 0; // ��ֵ��������Ӧ����Сֵ���Ǵ�ֵ
	int nBigNum, nSmallNum;
	// Corresponding to bignum, the scale value corresponding to the more numerous scales
	int nBigVal, nSmallVal; // ��bignum��Ӧ��������Ŀ̶ȶ�Ӧ�Ŀ̶�ֵ


	// Only need one type of scale value
	if (nHighNum == 0) // ֻ��Ҫһ�̶ֿ�ֵ
	{
		nStartNum = 0;
		nEndNum = 0;
		// Only one type
		nBigNum = nLowNum;nBigVal = nLow;	// ֻ��һ��
		nSmallNum = nLowNum;nSmallVal = nLow;
	}
	else if (nLowNum > nHighNum) 
	{
		nGap = nLowNum / nHighNum;
		int nTempNum = nLowNum - (nGap * nHighNum);
		nStartNum = nTempNum /2;
		nEndNum = nTempNum/2 + nTempNum%2;
		// Use whichever is more numerous
		nEndValue = nLow; // ˭����˭
		// Whichever is bigger is used for the main loop
		nBigNum = nLowNum;nBigVal = nLow; // ˭��˭������ѭ��
		nSmallNum = nHighNum;nSmallVal = nHigh;
	}
	else
	{
		nGap = nHighNum / nLowNum;
		int nTempNum = nHighNum - (nGap * nLowNum);
		nStartNum = nTempNum /2;
		nEndNum = nTempNum/2 + nTempNum%2;
		// Use whichever is more numerous
		nEndValue = nHigh;// ˭����˭
		// Whichever is bigger is used for the main loop
		nBigNum = nHighNum;nBigVal = nHigh;// ˭��˭������ѭ��
		nSmallNum = nLowNum;nSmallVal = nLow;
	}
		
	//////////////////////////////////////////////////////////////////////////
	// Generate scale array, note that when calculating it's intervals, but the array contains scales, so there will be one more
	// ���ɿ̶�����, ע�⣬���ʱ���Ǽ�����������ǿ̶ȣ����Ի��һ

	// Header
	// ͷ��
	
	int nSumTic = 0;	
	m_szTics.push_back(nSumTic);
	for (int i = 0; i < nStartNum; i++)
	{
		nSumTic  += nEndValue;
		m_szTics.push_back(nSumTic);
	}

	// Middle
	// �м�
	
	int nGapCount = 0;
	for (int i = 0; i < (nBigNum-nStartNum-nEndNum); i++)
	{
		nSumTic  += nBigVal;
		m_szTics.push_back(nSumTic);
		if (++nGapCount == nGap)
		{
			nSumTic  += nSmallVal;
			m_szTics.push_back(nSumTic);
			nGapCount = 0;
		}
	}

	// End
	// ��β
	for (int i = 0; i < nEndNum; i++)
	{	
		nSumTic  += nEndValue;
		m_szTics.push_back(nSumTic);
	}

	//m_szTics._Reverse(0, m_szTics.size());

	reverse(m_szTics.begin(), m_szTics.end());

	//m_szTics.reverse();

	szTics = m_szTics;
	return m_szTics.size();
}

