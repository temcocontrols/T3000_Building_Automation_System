#include "ULRBindCallback.h"
void CUrlDownloadToFileCallbackTestDlg::DownloadButton()//,
{
CBindCallback cbc;
cbc.m_pdlg = this;

(this->GetDlgItem(IDC_START))->EnableWindow(FALSE);//

//urlIEurl
//
CString szRand;
szRand.Format(_T("?skq=%d"),GetTickCount());
szUrl += szRand;

if(S_OK == URLDownloadToFile(NULL,szURL,szPath,0,&cbc))//szURL,szPath
MessageBox("finished");

}

