// NewImportDBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "NewImportDBDlg.h"
#include "afxdialogex.h"
#include "ImportDatabaseDlg.h"

// CNewImportDBDlg dialog

IMPLEMENT_DYNAMIC(CNewImportDBDlg, CDialog)

CNewImportDBDlg::CNewImportDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewImportDBDlg::IDD, pParent)
{

}

CNewImportDBDlg::~CNewImportDBDlg()
{
}

void CNewImportDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNewImportDBDlg, CDialog)
	ON_BN_CLICKED(IDC_IMPORT, &CNewImportDBDlg::OnBnClickedImport)
	ON_BN_CLICKED(IDC_Create, &CNewImportDBDlg::OnBnClickedCreate)
END_MESSAGE_MAP()


// CNewImportDBDlg message handlers


void CNewImportDBDlg::OnBnClickedImport()
{
	CImportDatabaseDlg dlg;
	dlg.DoModal();
}


void CNewImportDBDlg::OnBnClickedCreate()
{
	CString temp_dir;
	BROWSEINFOW blinfo; 
	ITEMIDLIST *itemlist;
	CString FilePath;
	blinfo.hwndOwner=NULL;             //this->m_hWnd;
	blinfo.pidlRoot=NULL;
	blinfo.pszDisplayName=temp_dir.GetBuffer(MAX_PATH);
	temp_dir.ReleaseBuffer();
	blinfo.lpszTitle=_T("Please enter directory");

	blinfo.ulFlags=BIF_STATUSTEXT|BIF_USENEWUI|BIF_RETURNONLYFSDIRS;
	blinfo.lpfn=NULL;
	blinfo.lParam=0;
	blinfo.iImage=0;
	itemlist=SHBrowseForFolder(&blinfo);
	if(itemlist==NULL)
		return ;
	if(!SHGetPathFromIDListW(itemlist,FilePath.GetBuffer(MAX_PATH)))
		return;
	FilePath.ReleaseBuffer();

	FilePath=FilePath+_T("T3000.mdb");
	HRSRC hrSrc = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_T3000_DATABASE), _T("DB"));   
	HGLOBAL hGlobal = LoadResource(AfxGetResourceHandle(), hrSrc);   


	LPVOID lpExe = LockResource(hGlobal);   
	CFile file;
	if(file.Open(FilePath, CFile::modeCreate | CFile::modeWrite))    
		file.Write(lpExe, (UINT)SizeofResource(AfxGetResourceHandle(), hrSrc));    
	file.Close();    
	::UnlockResource(hGlobal);   
	::FreeResource(hGlobal); 

	CString file_name=g_strExePth+_T("start.ini");
	CStdioFile* pFile=new CStdioFile;
	pFile->Open(file_name,CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate);
	pFile->WriteString(FilePath);
	pFile->Close();

	OnOK();
}
