
class CBindCallback : public IBindStatusCallback  
{
public:
CBindCallback();
virtual ~CBindCallback();

//
CUpdateDlg* m_pdlg;

//IBindStatusCallbackOnProgress     E_NOTIMPL 
    
STDMETHOD(OnStartBinding)
( DWORD dwReserved,
IBinding __RPC_FAR *pib)
{ return E_NOTIMPL; }

STDMETHOD(GetPriority)
( LONG __RPC_FAR *pnPriority)
{ return E_NOTIMPL; }

STDMETHOD(OnLowResource)
( DWORD reserved)
{ return E_NOTIMPL; }

//OnProgress
STDMETHOD(OnProgress)
( ULONG ulProgress,    
ULONG ulProgressMax,
ULONG ulStatusCode,
LPCWSTR wszStatusText);

STDMETHOD(OnStopBinding)
( HRESULT hresult,
LPCWSTR szError)
{ return E_NOTIMPL; }

STDMETHOD(GetBindInfo)
( DWORD __RPC_FAR *grfBINDF,
BINDINFO __RPC_FAR *pbindinfo)
{ return E_NOTIMPL; }

STDMETHOD(OnDataAvailable)
( DWORD grfBSCF,
DWORD dwSize,
FORMATETC __RPC_FAR *pformatetc,
STGMEDIUM __RPC_FAR *pstgmed)
{ return E_NOTIMPL; }

STDMETHOD(OnObjectAvailable)
( REFIID riid,
IUnknown __RPC_FAR *punk)
{ return E_NOTIMPL; }

// IUnknown.IE 

STDMETHOD_(ULONG,AddRef)()
{ return 0; }

STDMETHOD_(ULONG,Release)()
{ return 0; }

STDMETHOD(QueryInterface)
( REFIID riid,
void __RPC_FAR *__RPC_FAR *ppvObject)
{ return E_NOTIMPL; }
};

//OnProgress
CBindCallback::CBindCallback()
{

}

CBindCallback::~CBindCallback()
{

}

//////OnProgress

LRESULT CBindCallback::OnProgress(ULONG ulProgress,
ULONG ulProgressMax,
ULONG ulSatusCode,
LPCWSTR szStatusText)
{
CProgressCtrl* m_prg = (CProgressCtrl*)m_pdlg->GetDlgItem(IDC_PROGRESS_UPDATE);
m_prg->SetRange32(0,ulProgressMax);
m_prg->SetPos(ulProgress);

CString szText;
szText.Format(_T("Download %d%%"), (int)(ulProgress * 100.0 / ulProgressMax));
(m_pdlg->GetDlgItem(IDC_STATIC_PACKAGE))->SetWindowText(szText);

return S_OK;
}
