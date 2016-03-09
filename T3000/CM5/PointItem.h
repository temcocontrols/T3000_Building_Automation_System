#pragma once
#ifndef _Point_ITEM___
#define _Point_ITEM___

#include <Windows.h>

struct MyPoint
{
	int x;
	int y;
};
class CPointItem
{
public:
	CPointItem(void);
	~CPointItem(void);


void SetPoint(MyPoint point);  //设置此ItemPoint的坐标
MyPoint GetPoint() const;		//获取 坐标;

void SetIndex(int nIndex);	//设置此ItemPoint的序号;
int GetIndex() const;

void SetNext(CPointItem *pnext);
CPointItem* GetNext(void) const;
void SetPointValue(int nvalue);
int GetPointValue();


bool			m_link_to_next;//连接至下一个点;

protected:
	MyPoint			m_point;
	int				m_index;
	CPointItem		*m_pNextItem;
	int				m_value;

};

#endif

