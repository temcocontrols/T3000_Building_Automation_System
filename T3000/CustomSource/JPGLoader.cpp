#include "StdAfx.h"
#include "JPGLoader.h"

JPGLoader::JPGLoader(void)
{
	m_pDisplayWnd= NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	//m_picture = NULL;
}

JPGLoader::~JPGLoader(void)
{
	
}




void JPGLoader::SetJPGFileName(const CString& strJPG)
{
	m_strJPGFileName = strJPG;
}

void JPGLoader::SetDisplayWnd(CWnd* pWnd)
{
	m_pDisplayWnd = pWnd;
}

int JPGLoader::LoadPic()
{
	CString szFileName;
	szFileName.Empty();
	szFileName = m_strJPGFileName;// "D:\\ISPLogo.bmp";

	IStream *pStm;
	CFileStatus fstatus;
	CFile file;
	ULONGLONG cb;

	if (file.Open(szFileName,CFile::modeRead) && file.GetStatus(szFileName,fstatus) && ((cb = fstatus.m_size) != -1))
	{
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (SIZE_T)cb);
		LPVOID pvData =NULL;
		if (hGlobal != NULL)
		{
			pvData = GlobalLock(hGlobal);
			if (pvData != NULL)
			{
				//file.ReadHuge(pvData,cb);
				file.Read(pvData,(UINT)cb);
				GlobalUnlock(hGlobal);
				CreateStreamOnHGlobal(hGlobal,TRUE,&pStm);
			}
		}   
	}
	else
	{
		return 0;
	}


	// HRESULT Render( HDC hdc, //
	// long x, //hdc
	// long y, //hdc
	// long cx, //
	// long cy, //
	// OLE_XPOS_HIMETRIC xSrc, //
	// OLE_YPOS_HIMETRIC ySrc, //
	// OLE_XSIZE_HIMETRIC cxSrc,//
	// OLE_YSIZE_HIMETRIC cySrc,//
	// LPCRECT prcWBounds //);
	//CComQIPtr<IPicture> m_picture;
	HRESULT hr = OleLoadPicture(pStm,0,false,IID_IPicture,(void**)&m_picture);
	long a,b; 
	
	m_picture->get_Width(&a);
	m_picture->get_Height(&b);

	m_nWidth = a;
	m_nHeight = b;
	return 1;
}

int JPGLoader::ShowJPG()
{
	if (!m_picture)
	{
		return 0;
	}
	CSize sz(m_nWidth,m_nHeight);
	CDC *pdc = m_pDisplayWnd->GetDC();//GetDlgItem(IDC_STATIC)->GetDC();
	//    CDC *pdc = GetDC();
	//    pdc->HIMETRICtoDP(&sz);
	CRect rect;
	m_pDisplayWnd->GetClientRect(rect);
	//    GetDlgItem(IDC_STATIC)->GetClientRect(&rect);
	//m_picture->Render(*pdc,0,0,sz.cx,sz.cy,0,b,a,-b,&rect);
	int nRet = m_picture->Render(*pdc,rect.left,rect.top,rect.Width(),rect.Height(),0,m_nHeight,m_nWidth,-m_nHeight,&rect);

	if (nRet == S_OK)
	{
		return 1;
	}

	return nRet;

}


void JPGLoader::GetPicSize(CSize& size)
{
	if(LoadPic())
	{
		size.cx = m_nWidth;
		size.cy = m_nHeight;
	}
}
