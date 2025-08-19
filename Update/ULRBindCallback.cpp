#include "ULRBindCallback.h"
void CUrlDownloadToFileCallbackTestDlg::DownloadButton()//下载按钮,也可以为线程 - Download button, can also be a thread
{
CBindCallback cbc;
cbc.m_pdlg = this;

(this->GetDlgItem(IDC_START))->EnableWindow(FALSE);//禁用下载按钮 - Disable download button

//在url后添加随机数，防止从IE缓存中读取。url后加随机数不会影响下载的。- Add a random number after the URL to prevent reading from the IE cache. Adding a random number after the URL does not affect the download.
//如果想要从缓存中提取那么就把下面的注释掉 - If you want to extract from the cache, uncomment the line below
CString szRand;
szRand.Format(_T("?skq=%d"),GetTickCount());
szUrl += szRand;

if(S_OK == URLDownloadToFile(NULL,szURL,szPath,0,&cbc))//szURL,szPath为全局变量 - szURL, szPath are global variables

MessageBox("finished");

}

