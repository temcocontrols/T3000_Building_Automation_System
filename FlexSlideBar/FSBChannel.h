#pragma once
#include <vector>
using namespace std;


class CTripleLinkageOpt;
class CMultiThumbOpt;
struct RC_CLR
{
	CRect rc;
	COLORREF clr;
};


class CFSBChannel
{
public:
	CFSBChannel(void);
	virtual ~CFSBChannel(void);

	void Draw(CDC* pDC);

	void SetRect(CRect rc);
	CRect GetRect();

	void SetWidth(int nWidth);
	int GetWidth();

	void SetHeight(int nHeight);
	int GetHeight();

	void SetColor(COLORREF clr);
	COLORREF GetColor();

	int SetRCArray(vector<RC_CLR>& szRcs);

	friend class CTripleLinkageOpt;
	friend class CMultiThumbOpt;
protected:
// 	int					m_nPosX;		// 应该为channel中心点位置
// 	int					m_nPosY;

	CRect				m_rc;

	int					m_nWidth;
	int					m_nHeight;

	COLORREF		m_clr;

	vector<RC_CLR*>			m_szRC;

};
