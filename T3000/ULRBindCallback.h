
class CBindCallback : public IBindStatusCallback
{
public:
    CBindCallback();
    virtual ~CBindCallback();

    // Accept handle for displaying progress window
    //������ʾ���ȴ��ڵľ��
    Dowmloadfile* m_pdlg;

    // IBindStatusCallback methods. All methods except OnProgress return E_NOTIMPL
    //IBindStatusCallback�ķ���������OnProgress     �����������������E_NOTIMPL 

    STDMETHOD(OnStartBinding)
        (DWORD dwReserved,
            IBinding __RPC_FAR *pib)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetPriority)
        (LONG __RPC_FAR *pnPriority)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(OnLowResource)
        (DWORD reserved)
    {
        return E_NOTIMPL;
    }

    // OnProgress is here
    //OnProgress������
    STDMETHOD(OnProgress)
        (ULONG ulProgress,
            ULONG ulProgressMax,
            ULONG ulStatusCode,
            LPCWSTR wszStatusText);

    STDMETHOD(OnStopBinding)
        (HRESULT hresult,
            LPCWSTR szError)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(GetBindInfo)
        (DWORD __RPC_FAR *grfBINDF,
            BINDINFO __RPC_FAR *pbindinfo)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(OnDataAvailable)
        (DWORD grfBSCF,
            DWORD dwSize,
            FORMATETC __RPC_FAR *pformatetc,
            STGMEDIUM __RPC_FAR *pstgmed)
    {
        return E_NOTIMPL;
    }

    STDMETHOD(OnObjectAvailable)
        (REFIID riid,
            IUnknown __RPC_FAR *punk)
    {
        return E_NOTIMPL;
    }

    // IUnknown methods. IE will not call these methods
    // IUnknown����.IE ���������Щ������

    STDMETHOD_(ULONG, AddRef)()
    {
        return 0;
    }

    STDMETHOD_(ULONG, Release)()
    {
        return 0;
    }

    STDMETHOD(QueryInterface)
        (REFIID riid,
            void __RPC_FAR *__RPC_FAR *ppvObject)
    {
        return E_NOTIMPL;
    }
};

// Only need to implement OnProgress method, class implementation:
//ֻ��ʵ��OnProgress���������ʵ�֣�
CBindCallback::CBindCallback()
{

}

CBindCallback::~CBindCallback()
{

}

// Only need to implement OnProgress member
//////��ʵ��OnProgress��Ա����

LRESULT CBindCallback::OnProgress(ULONG ulProgress,
ULONG ulProgressMax,
ULONG ulSatusCode,
LPCWSTR szStatusText)
{
CProgressCtrl* m_prg = (CProgressCtrl*)m_pdlg->GetDlgItem(IDC_PROGRESS_FTP_DOWNLOAD);
m_prg->SetRange32(0,ulProgressMax);
m_prg->SetPos(ulProgress);

CString szText;
szText.Format(_T("Download %d%%"), (int)(ulProgress * 100.0 / ulProgressMax));
(m_pdlg->GetDlgItem(IDC_STATIC_FTP_PERSENT))->SetWindowText(szText);

return S_OK;
}
