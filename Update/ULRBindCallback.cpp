#include "ULRBindCallback.h"
void CUrlDownloadToFileCallbackTestDlg::DownloadButton()//���ذ�ť,Ҳ����Ϊ�߳� - Download button, can also be a thread
{
CBindCallback cbc;
cbc.m_pdlg = this;

(this->GetDlgItem(IDC_START))->EnableWindow(FALSE);//�������ذ�ť - Disable download button

//��url��������������ֹ��IE�����ж�ȡ��url������������Ӱ�����صġ�- Add a random number after the URL to prevent reading from the IE cache. Adding a random number after the URL does not affect the download.
//�����Ҫ�ӻ�������ȡ��ô�Ͱ������ע�͵� - If you want to extract from the cache, uncomment the line below
CString szRand;
szRand.Format(_T("?skq=%d"),GetTickCount());
szUrl += szRand;

if(S_OK == URLDownloadToFile(NULL,szURL,szPath,0,&cbc))//szURL,szPathΪȫ�ֱ��� - szURL, szPath are global variables

MessageBox("finished");

}

