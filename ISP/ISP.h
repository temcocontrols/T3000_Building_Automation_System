// ISP.h : ISP DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CISPApp
// �йش���ʵ�ֵ���Ϣ������� ISP.cpp
//

class CISPApp : public CWinApp
{
public:
	CISPApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
