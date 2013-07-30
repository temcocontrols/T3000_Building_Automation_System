/********************************************************************
	created:	2011/03/25
	created:	25:3:2011   11:40
	filename: 	e:\My NewTest\TestJPG\TestPicture\TestPicture\JPGLoader.h
	file path:	e:\My NewTest\TestJPG\TestPicture\TestPicture
	file base:	JPGLoader
	file ext:	h
	author:		ZGQ
	
	purpose:	加载JPG文件，并显示到指定的窗口
*********************************************************************/


#pragma once

class JPGLoader
{
public:
	JPGLoader(void);
	virtual ~JPGLoader(void);

	void SetJPGFileName(const CString& strJPG);
	void SetDisplayWnd(CWnd* pWnd);

	int LoadPic();
	int ShowJPG();

	void GetPicSize(CSize& size);
	

protected:


private:

	int							m_nHeight;
	int							m_nWidth;
	CString					m_strJPGFileName;
	CWnd*					m_pDisplayWnd;
	CComQIPtr<IPicture> m_picture;
};
