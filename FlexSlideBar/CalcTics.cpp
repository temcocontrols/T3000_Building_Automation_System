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
	// 获得分布情况模型，两种刻度尺寸插入间隔，多余的刻度分布在两端
	ASSERT(nHighNum + nLowNum == m_nTicNum);
	
	int nGap=0;
	int nStartNum, nEndNum;
	int nEndValue = 0; // 端值，即两端应该用小值还是大值
	int nBigNum, nSmallNum;
	int nBigVal, nSmallVal; // 与bignum对应，数量多的刻度对应的刻度值


	if (nHighNum == 0) // 只需要一种刻度值
	{
		nStartNum = 0;
		nEndNum = 0;
		nBigNum = nLowNum;nBigVal = nLow;	// 只有一种
		nSmallNum = nLowNum;nSmallVal = nLow;
	}
	else if (nLowNum > nHighNum) 
	{
		nGap = nLowNum / nHighNum;
		int nTempNum = nLowNum - (nGap * nHighNum);
		nStartNum = nTempNum /2;
		nEndNum = nTempNum/2 + nTempNum%2;
		nEndValue = nLow; // 谁多用谁
		nBigNum = nLowNum;nBigVal = nLow; // 谁大谁用来主循环
		nSmallNum = nHighNum;nSmallVal = nHigh;
	}
	else
	{
		nGap = nHighNum / nLowNum;
		int nTempNum = nHighNum - (nGap * nLowNum);
		nStartNum = nTempNum /2;
		nEndNum = nTempNum/2 + nTempNum%2;
		nEndValue = nHigh;// 谁多用谁
		nBigNum = nHighNum;nBigVal = nHigh;// 谁大谁用来主循环
		nSmallNum = nLowNum;nSmallVal = nLow;
	}
		
	//////////////////////////////////////////////////////////////////////////
	// 生成刻度数组, 注意，算的时候是间隔，数组里是刻度，所以会多一

	// 头部
	
	int nSumTic = 0;	
	m_szTics.push_back(nSumTic);
	for (int i = 0; i < nStartNum; i++)
	{
		nSumTic  += nEndValue;
		m_szTics.push_back(nSumTic);
	}

	// 中间
	
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

	// 结尾
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

