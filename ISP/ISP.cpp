// ISP.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ISP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


HINSTANCE hResOld; // �ɵ���Դ���
HINSTANCE hDll; // DLL��Դ���
//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CISPApp

BEGIN_MESSAGE_MAP(CISPApp, CWinApp)
END_MESSAGE_MAP()


// CISPApp ����

CISPApp::CISPApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CISPApp ����

CISPApp theApp;


// CISPApp ��ʼ��

BOOL CISPApp::InitInstance()
{
	CWinApp::InitInstance();
	hDll = m_hInstance;
	return TRUE;
}
