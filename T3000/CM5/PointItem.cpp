//PointItem Code by Fance Du 2013 11 05
#include "stdafx.h"
#include "PointItem.h"

CPointItem::CPointItem(void)
{
 	m_point.x=0;
	m_point.y=0;
	m_index = 0;
	m_pNextItem =NULL;
	m_link_to_next = true;
//	m_CPU_Loading = GetCPUpercent();
}


CPointItem::~CPointItem(void)
{
	m_pNextItem =NULL;
}

void CPointItem::SetPoint(MyPoint point)  //设置此ItemPoint的坐标
{
	m_point.x = point.x;
	m_point.y = point.y;
}

MyPoint CPointItem::GetPoint() const		//获取 坐标;
{
	MyPoint tempPoint;
	tempPoint.x=m_point.x;
	tempPoint.y=m_point.y;
	return tempPoint;
}



void CPointItem::SetIndex(int nIndex)	//设置此ItemPoint的序号;
{
	m_index = nIndex;
}

int CPointItem::GetIndex() const
{
	return m_index;
}


void CPointItem::SetNext(CPointItem *pnext)
{
	m_pNextItem = pnext;
}

CPointItem* CPointItem::GetNext(void) const
{
	return m_pNextItem;
}

int CPointItem::GetPointValue()
{
	return m_value;
}

void CPointItem::SetPointValue(int nvalue)
{
	m_value = nvalue;
}


