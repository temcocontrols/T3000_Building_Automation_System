#include "ULRBindCallback.h"
void CUrlDownloadToFileCallbackTestDlg::DownloadButton()//下载按钮,也可以为线程
{
CBindCallback cbc;
cbc.m_pdlg = this;

(this->GetDlgItem(IDC_START))->EnableWindow(FALSE);//禁用下载按钮

//在url后添加随机数，防止从IE缓存中读取。url后加随机数不会影响下载的。
//如果想要从缓存中提取那么就把下面的注释掉
CString szRand;
szRand.Format(_T("?skq=%d"),GetTickCount());
szUrl += szRand;

if(S_OK == URLDownloadToFile(NULL,szURL,szPath,0,&cbc))//szURL,szPath为全局变量
MessageBox("finished");

}

