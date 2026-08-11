#pragma once
#include "afxdialogex.h"

class CWireGuardConfig : public CDialogEx
{
    DECLARE_DYNAMIC(CWireGuardConfig)

public:
    CWireGuardConfig(CWnd* pParent = nullptr);
    virtual ~CWireGuardConfig();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG_WIREGUARD_CONFIG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonGeneratePrivateKey();
    afx_msg void OnBnClickedButtonGeneratePublicKey();
    afx_msg void OnBnClickedButtonRegenerate();
    afx_msg void OnBnClickedCheckEnableWireguard();
    afx_msg void OnBnClickedCheckEnablePreSharedKey();
    afx_msg void OnBnClickedButtonWriteWireguardCfg();
    afx_msg void OnBnClickedButtonCancelWireguardCfg();

private:
    void    LoadFromDevice();       // 从 Device_Wireguard_Point 加载到界面 / Load from Device_Wireguard_Point to UI
    void    SaveToDevice();         // 从界面保存到 Device_Wireguard_Point / Save from UI to Device_Wireguard_Point
    void    UpdateUIState();        // 更新控件启用状态 / Update control enable state
    BOOL    ValidateInput();        // 输入校验 / Input validation

    CString GeneratePrivateKey();   // 生成WireGuard私钥 / Generate WG private key
    CString GeneratePublicKey(const CString& strPrivateKey);  // 从私钥派生公钥 / Derive public key from private key
    CString GeneratePSK();          // 生成预共享密钥 / Generate pre-shared key

    BOOL    GenerateRandomBytes(BYTE* pBuffer, int nLen);  // 加密安全随机字节 / Cryptographically secure random bytes
    CString Base64Encode(const BYTE* pData, int nLen);     // Base64编码 / Base64 encoding
};
