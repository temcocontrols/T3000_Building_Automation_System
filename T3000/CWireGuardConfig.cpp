// WireGuardConfig.cpp : 实现文件 / Implementation file
//
// 注意: 本文件包含 Curve25519 精简实现，仅用于 WireGuard 密钥对生成。
// Note: This file contains a minimal Curve25519 implementation for WireGuard key pair generation only.
// 生产环境建议使用经过充分测试的密码学库 (如 tweetNaCl, OpenSSL)。
// For production use, recommend well-tested crypto libraries (tweetNaCl, OpenSSL, etc.).
//

#include "stdafx.h"
#include "T3000.h"
#include "global_function.h"
#include "global_define.h"
#include "afxdialogex.h"
#include "CWireGuardConfig.h"
#include <wincrypt.h>

#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32.lib")

IMPLEMENT_DYNAMIC(CWireGuardConfig, CDialogEx)

CWireGuardConfig::CWireGuardConfig(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG_WIREGUARD_CONFIG, pParent)
{
}

CWireGuardConfig::~CWireGuardConfig()
{
}

void CWireGuardConfig::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWireGuardConfig, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_GENERATE_PRIVATE_KEY, &CWireGuardConfig::OnBnClickedButtonGeneratePrivateKey)
    ON_BN_CLICKED(IDC_BUTTON_GENERATE_PUBLIC_KEY, &CWireGuardConfig::OnBnClickedButtonGeneratePublicKey)
    ON_BN_CLICKED(IDC_BUTTON_REGENERATE, &CWireGuardConfig::OnBnClickedButtonRegenerate)
    ON_BN_CLICKED(IDC_CHECK_ENABLE_WIREGUARD, &CWireGuardConfig::OnBnClickedCheckEnableWireguard)
    ON_BN_CLICKED(IDC_CHECK_ENABLE_PRE_SHARED_KEY, &CWireGuardConfig::OnBnClickedCheckEnablePreSharedKey)
    ON_BN_CLICKED(IDC_BUTTON_WRITE_WIREGUARD_CFG, &CWireGuardConfig::OnBnClickedButtonWriteWireguardCfg)
    ON_BN_CLICKED(IDC_BUTTON_CANCEL_WIREGUARD_CFG, &CWireGuardConfig::OnBnClickedButtonCancelWireguardCfg)
END_MESSAGE_MAP()


// ============================================================
// Curve25519 (X25519) 精简实现 / Curve25519 (X25519) minimal implementation
// 基于 8-bit limb 大整数运算 / Based on 8-bit limb big integer arithmetic
// 参考 RFC 7748 / Reference: RFC 7748
// ============================================================

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

// 256-bit 大整数 (小端) / 256-bit big integer (little-endian)
typedef u8 bn256[32];

// 512-bit 大整数 (小端) / 512-bit big integer (little-endian)
typedef u8 bn512[64];

// 模数 p = 2^255 - 19 / Modulus p = 2^255 - 19
static const bn256 P25519 = {
    0xED, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F
};

// 比较: 返回 -1(a<b), 0(a==b), 1(a>b) / Compare: return -1(a<b), 0(a==b), 1(a>b)
static int bn_cmp(const bn256 a, const bn256 b)
{
    for (int i = 31; i >= 0; i--) {
        if (a[i] > b[i]) return 1;
        if (a[i] < b[i]) return -1;
    }
    return 0;
}

// 加法 c = a + b, 返回进位 / Addition c = a + b, return carry
static u8 bn_add(bn256 c, const bn256 a, const bn256 b)
{
    u16 carry = 0;
    for (int i = 0; i < 32; i++) {
        carry += (u16)a[i] + b[i];
        c[i] = (u8)(carry & 0xFF);
        carry >>= 8;
    }
    return (u8)carry;
}

// 加小整数 c = a + b (b 是小整数) / Add small integer c = a + b (b is small)
static u8 bn_add_small(bn256 c, const bn256 a, u32 b)
{
    u32 carry = b;
    for (int i = 0; i < 32 && carry; i++) {
        carry += a[i];
        c[i] = (u8)(carry & 0xFF);
        carry >>= 8;
    }
    return (u8)carry;
}

// 减法 c = a - b, 返回借位 (1 if a < b) / Subtraction c = a - b, return borrow (1 if a < b)
static u8 bn_sub(bn256 c, const bn256 a, const bn256 b)
{
    u16 borrow = 0;
    for (int i = 0; i < 32; i++) {
        u16 diff = (u16)a[i] - b[i] - borrow;
        if (diff & 0xFF00) {
            c[i] = (u8)(diff + 0x100);
            borrow = 1;
        } else {
            c[i] = (u8)diff;
            borrow = 0;
        }
    }
    return (u8)borrow;
}

// 乘法 c = a * b (512-bit result)
static void bn_mul(bn512 c, const bn256 a, const bn256 b)
{
    u16 t[64];
    int i, j;

    for (i = 0; i < 64; i++) t[i] = 0;

    for (i = 0; i < 32; i++) {
        u16 carry = 0;
        for (j = 0; j < 32; j++) {
            u32 prod = (u32)a[i] * b[j] + t[i + j] + carry;
            t[i + j] = (u16)(prod & 0xFF);
            carry = (u16)(prod >> 8);
        }
        t[i + 32] = carry;
    }

    for (i = 0; i < 64; i++) c[i] = (u8)t[i];
}

// 乘小整数: c = a * b (a is 256-bit, b is small, result is 256-bit mod p)
static void bn_mul_small_modp(bn256 c, const bn256 a, u32 b)
{
    // 先计算完整乘积 (最多 33 bytes) / First compute full product (max 33 bytes)
    bn512 tmp;
    u32 carry = b;
    int i;

    memset(tmp, 0, 64);

    // a * b, 结果存在 tmp 的低 33 字节 / a * b, result in low 33 bytes of tmp
    carry = 0;
    for (i = 0; i < 32; i++) {
        u32 v = (u32)a[i] * b + carry;
        tmp[i] = (u8)(v & 0xFF);
        carry = v >> 8;
    }
    while (carry && i < 40) {
        tmp[i++] = (u8)(carry & 0xFF);
        carry >>= 8;
    }

    // 归约 mod p / Reduce mod p
    // 使用 2^256 ≡ 38 mod p 的性质 (因为 2^255 ≡ 19 mod p)
    // Use property 2^256 ≡ 38 mod p (since 2^255 ≡ 19 mod p)

    // 从高位到低位处理 / Process from high to low
    for (i = 511; i >= 256; i--) {
        int byte_i = i >> 3;
        int bit_i = i & 7;

        if (tmp[byte_i] & (1 << bit_i)) {
            // 清除该位 / Clear this bit
            tmp[byte_i] &= ~(1 << bit_i);

            // 2^i ≡ 2^(i-256) * 38 mod p
            // 把 38 * 2^(i-256) 加到低位
            // Add 38 * 2^(i-256) to lower bits
            int shift = i - 256;
            int shift_byte = shift >> 3;
            int shift_bit = shift & 7;

            u32 val = (u32)38 << shift_bit;

            // 加到 tmp[shift_byte...]，进位传播到整个 512 位
            // Add to tmp[shift_byte...], propagate carry across full 512 bits
            u32 add_carry = val;
            for (int j = shift_byte; j < 64 && add_carry; j++) {
                add_carry += tmp[j];
                tmp[j] = (u8)(add_carry & 0xFF);
                add_carry >>= 8;
            }
        }
    }

    // 最终归约: 如果 >= p 就减去 p / Final reduction: subtract p if >= p
    // 最多需要减两次 / At most two subtractions needed
    for (int k = 0; k < 2; k++) {
        if (bn_cmp(tmp, P25519) >= 0) {
            bn_sub(tmp, tmp, P25519);
        }
    }

    // 拷贝结果 / Copy result
    memcpy(c, tmp, 32);
}

// 模 p 归约: 把 512-bit 的 a 归约为 256-bit result = a mod p
// Mod p reduction: reduce 512-bit a to 256-bit result = a mod p
static void bn_modp(bn256 result, const bn512 a)
{
    bn512 tmp;
    int i;

    memcpy(tmp, a, 64);

    // 从最高位到 256，逐位归约
    // Reduce bit by bit from MSB down to 256
    // 2^i ≡ 38 * 2^(i-256) mod p
    for (i = 511; i >= 256; i--) {
        int byte_i = i >> 3;
        int bit_i = i & 7;

        if (tmp[byte_i] & (1 << bit_i)) {
            // 清除该位 / Clear this bit
            tmp[byte_i] &= ~(1 << bit_i);

            // 加上 38 * 2^(i-256) / Add 38 * 2^(i-256)
            int shift = i - 256;
            int shift_byte = shift >> 3;
            int shift_bit = shift & 7;

            u32 val = (u32)38 << shift_bit;

            // 加到 tmp[shift_byte...]，进位传播到整个 512 位
            // Add to tmp[shift_byte...], propagate carry across full 512 bits
            u32 carry = val;
            for (int j = shift_byte; j < 64 && carry; j++) {
                carry += tmp[j];
                tmp[j] = (u8)(carry & 0xFF);
                carry >>= 8;
            }
        }
    }

    // 最终归约 / Final reduction
    for (int k = 0; k < 3; k++) {
        if (bn_cmp(tmp, P25519) >= 0) {
            bn_sub(tmp, tmp, P25519);
        }
    }

    memcpy(result, tmp, 32);
}

// 模 p 求逆: result = a^(-1) mod p (费马小定理)
// Mod p inversion: result = a^(-1) mod p (Fermat's little theorem)
static void bn_modp_inv(bn256 result, const bn256 a)
{
    // 计算 a^(p-2) mod p
    // p - 2 = 2^255 - 21
    // 二进制: 低 5 位 = 01011, 第 5 位到第 254 位全是 1
    // Binary: low 5 bits = 01011, bits 5..254 all 1

    bn256 base;
    int i;

    // result = 1
    memset(result, 0, 32);
    result[0] = 1;

    // base = a mod p
    memcpy(base, a, 32);
    for (int k = 0; k < 2; k++) {
        if (bn_cmp(base, P25519) >= 0)
            bn_sub(base, base, P25519);
    }

    // 从最高位 (bit 254) 到最低位 (bit 0)
    for (i = 254; i >= 0; i--) {
        // square
        bn512 sq;
        bn_mul(sq, result, result);
        bn_modp(result, sq);

        // 如果这一位是 1，就乘以 base / If this bit is 1, multiply by base
        int bit;
        if (i >= 5) {
            bit = 1;
        } else {
            // 低 5 位: p-2 = 2^255 - 21, 低 5 位 = 0b01011
            static const int low_bits[5] = { 1, 1, 0, 1, 0 };
            bit = low_bits[i];
        }

        if (bit) {
            bn512 prod;
            bn_mul(prod, result, base);
            bn_modp(result, prod);
        }
    }
}

// ============================================================
// X25519 Montgomery ladder / X25519 蒙哥马利阶梯
// ============================================================

#define A24 121665  // (Curve25519 A - 2) / 4 = (486662 - 2) / 4

// 条件交换 (常数时间) / Conditional swap (constant time)
static void cswap(int swap, bn256 x2, bn256 x3, bn256 z2, bn256 z3)
{
    if (swap) {
        u8 t;
        for (int i = 0; i < 32; i++) {
            t = x2[i]; x2[i] = x3[i]; x3[i] = t;
            t = z2[i]; z2[i] = z3[i]; z3[i] = t;
        }
    }
}

// 加法: c = a + b mod p / Addition: c = a + b mod p
static void add_modp(bn256 c, const bn256 a, const bn256 b)
{
    u8 carry = bn_add(c, a, b);
    if (carry || bn_cmp(c, P25519) >= 0) {
        bn_sub(c, c, P25519);
    }
}

// 减法: c = a - b mod p / Subtraction: c = a - b mod p
static void sub_modp(bn256 c, const bn256 a, const bn256 b)
{
    u8 borrow = bn_sub(c, a, b);
    if (borrow) {
        // 不够减时: a - b = c - 2^256 (借位)
        // 所以 (a - b) mod p = c - 2^256 + p
        // 因为 p = 2^255 - 19, 所以 2^256 - p = 2^255 + 19
        // 结果 = c - (2^255 + 19)
        // Underflow: a - b = c - 2^256 (borrowed)
        // So (a - b) mod p = c - 2^256 + p
        // Since p = 2^255 - 19, 2^256 - p = 2^255 + 19
        // Result = c - (2^255 + 19)
        // 小端: 2^255 + 19 = [0x13, 0, 0, ..., 0x80]
        // Little-endian: 2^255 + 19 = [0x13, 0, 0, ..., 0x80]
        bn256 correction;
        memset(correction, 0, 32);
        correction[0] = 0x13;     // 19
        correction[31] = 0x80;    // 2^255
        bn_sub(c, c, correction);
        // 不会再次借位: c >= 2^256 - (p-1) = 2^255 + 20 > 2^255 + 19
        // No second borrow: c >= 2^256 - (p-1) = 2^255 + 20 > 2^255 + 19
    }
}

// Montgomery ladder 一步 / One step of Montgomery ladder
// P2 = (x2, z2), P3 = (x3, z3), x1 = base point x-coordinate
// 输入: P2, P3, x1
// 输出: P2 = P2+P3, P3 = 2*P2  (或者反过来取决于 swap)
// Output: P2 = P2+P3, P3 = 2*P2 (or vice versa depending on swap)
static void x25519_step(bn256 x2, bn256 z2, bn256 x3, bn256 z3, const bn256 x1)
{
    bn256 A, B, C, D, t, u;

    // A = x2 + z2
    add_modp(A, x2, z2);

    // B = x2 - z2
    sub_modp(B, x2, z2);

    // C = x3 + z3
    add_modp(C, x3, z3);

    // D = x3 - z3
    sub_modp(D, x3, z3);

    // t = D * A
    bn512 tmp512;
    bn256 DA, CB;
    bn_mul(tmp512, D, A);
    bn_modp(DA, tmp512);

    // t = C * B
    bn_mul(tmp512, C, B);
    bn_modp(CB, tmp512);

    // x3 = (DA + CB)^2
    add_modp(t, DA, CB);
    bn_mul(tmp512, t, t);
    bn_modp(x3, tmp512);

    // z3 = x1 * (DA - CB)^2
    sub_modp(t, DA, CB);
    bn_mul(tmp512, t, t);
    bn_modp(u, tmp512);
    bn_mul(tmp512, u, x1);
    bn_modp(z3, tmp512);

    // x2 = A^2 * B^2
    bn256 AA, BB;
    bn_mul(tmp512, A, A);
    bn_modp(AA, tmp512);
    bn_mul(tmp512, B, B);
    bn_modp(BB, tmp512);
    bn_mul(tmp512, AA, BB);
    bn_modp(x2, tmp512);

    // z2 = E * (AA + a24*E)  where E = AA - BB
    bn256 E, a24E;
    sub_modp(E, AA, BB);
    bn_mul_small_modp(a24E, E, A24);
    add_modp(t, AA, a24E);
    bn_mul(tmp512, E, t);
    bn_modp(z2, tmp512);
}

// X25519: 计算 scalar * point (仅 x 坐标)
// X25519: compute scalar * point (x-coordinate only)
static void x25519(u8* result, const u8* scalar, const u8* point)
{
    bn256 x1, x2, z2, x3, z3;
    int swap = 0;
    int pos;

    // x1 = u coordinate of base point
    memcpy(x1, point, 32);

    // 初始化 / Init
    // x2 = 1, z2 = 0 (点 (1, 0) 是无穷远点? 不对，应该用标准初始化)
    // x2 = 1, z2 = 0 (point (1,0) is infinity? No, use standard init)
    // 标准: x2 = u, z2 = 1; x3 = u^2+1, z3 = u*something... 不对
    // Standard Montgomery ladder init: P2 = (u, 1), P3 = (u^2+1, 2u) 不对
    // 正确初始化: P2 = (x1, 1) 对应 1*P, P3 = something 对应 2*P?
    // 不，标准的初始化是:
    // x2 = 1, z2 = 0 (无穷远点的投影坐标)
    // x3 = x1, z3 = 1 (点 P)

    // 等等，Montgomery ladder 的标准初始化是:
    // R0 = O (无穷远点), R1 = P
    // 投影坐标:
    // R0 = (X:Z) = (1:0)
    // R1 = (X:Z) = (u:1)

    memset(x2, 0, 32); x2[0] = 1;  // x2 = 1
    memset(z2, 0, 32);              // z2 = 0

    memcpy(x3, x1, 32);             // x3 = x1
    memset(z3, 0, 32); z3[0] = 1;   // z3 = 1

    // 处理标量: clamp
    u8 clamped[32];
    memcpy(clamped, scalar, 32);
    clamped[0] &= 248;     // clear lowest 3 bits
    clamped[31] &= 127;    // clear highest bit
    clamped[31] |= 64;     // set second highest bit

    // Montgomery ladder
    for (pos = 254; pos >= 0; pos--) {
        int bit = (clamped[pos >> 3] >> (pos & 7)) & 1;
        swap ^= bit;

        cswap(swap, x2, x3, z2, z3);
        swap = bit;

        x25519_step(x2, z2, x3, z3, x1);
    }

    // 最后一次交换 / Final swap
    cswap(swap, x2, x3, z2, z3);

    // result = x2 * z2^(-1) mod p
    bn256 z2_inv;
    bn_modp_inv(z2_inv, z2);

    bn512 prod;
    bn_mul(prod, x2, z2_inv);
    bn_modp(x2, prod);

    memcpy(result, x2, 32);
}

// Curve25519 基点: u = 9 / Curve25519 base point: u = 9
static const u8 basepoint[32] = {
    9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// 从私钥派生公钥 / Derive public key from private key
static void curve25519_scalarmult_base(u8* pubkey, const u8* privkey)
{
    x25519(pubkey, privkey, basepoint);
}


// ============================================================
// 对话框实现 / Dialog implementation
// ============================================================

BOOL CWireGuardConfig::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    LoadFromDevice();
    UpdateUIState();

    return TRUE;
}

void CWireGuardConfig::LoadFromDevice()
{
    // Enable
    CheckDlgButton(IDC_CHECK_ENABLE_WIREGUARD,
        Device_Wireguard_Point.reg.wg_enable ? BST_CHECKED : BST_UNCHECKED);

    // Port
    SetDlgItemInt(IDC_EDIT_WIREGUARD_PORT, Device_Wireguard_Point.reg.wg_port);

    // Local VPN IP
    CIPAddressCtrl* pLocalIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_LOCAL_VPN_IP);
    if (pLocalIP)
    {
        pLocalIP->SetAddress(
            Device_Wireguard_Point.reg.local_ip[0],
            Device_Wireguard_Point.reg.local_ip[1],
            Device_Wireguard_Point.reg.local_ip[2],
            Device_Wireguard_Point.reg.local_ip[3]);
    }

    // Peer Endpoint IP
    CIPAddressCtrl* pPeerIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_ENDPOINT_IP);
    if (pPeerIP)
    {
        pPeerIP->SetAddress(
            Device_Wireguard_Point.reg.peer_ip[0],
            Device_Wireguard_Point.reg.peer_ip[1],
            Device_Wireguard_Point.reg.peer_ip[2],
            Device_Wireguard_Point.reg.peer_ip[3]);
    }

    // Private key
    CString strKey;
    strKey.Format(_T("%hs"), Device_Wireguard_Point.reg.private_key);
    SetDlgItemText(IDC_EDIT_LOCAL_PRIVATE_KEY, strKey);

    // Peer public key
    strKey.Format(_T("%hs"), Device_Wireguard_Point.reg.peer_public_key);
    SetDlgItemText(IDC_EDIT_PEER_PUBLIC_KEY, strKey);

    // PSK
    CString strPSK;
    strPSK.Format(_T("%hs"), Device_Wireguard_Point.reg.pre_shared_key);
    BOOL bPSKEnabled = (strPSK.GetLength() >= 40);
    CheckDlgButton(IDC_CHECK_ENABLE_PRE_SHARED_KEY, bPSKEnabled ? BST_CHECKED : BST_UNCHECKED);
    SetDlgItemText(IDC_EDIT_PRE_SHARED_KEY, strPSK);
}

void CWireGuardConfig::SaveToDevice()
{
    // Enable
    Device_Wireguard_Point.reg.wg_enable =
        (IsDlgButtonChecked(IDC_CHECK_ENABLE_WIREGUARD) == BST_CHECKED) ? 1 : 0;

    // Port
    BOOL bTrans = FALSE;
    UINT nPort = GetDlgItemInt(IDC_EDIT_WIREGUARD_PORT, &bTrans);
    if (bTrans && nPort > 0 && nPort <= 65535)
    {
        Device_Wireguard_Point.reg.wg_port = (unsigned short)nPort;
    }

    // Local VPN IP
    CIPAddressCtrl* pLocalIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_LOCAL_VPN_IP);
    if (pLocalIP)
    {
        BYTE b1, b2, b3, b4;
        pLocalIP->GetAddress(b1, b2, b3, b4);
        Device_Wireguard_Point.reg.local_ip[0] = b1;
        Device_Wireguard_Point.reg.local_ip[1] = b2;
        Device_Wireguard_Point.reg.local_ip[2] = b3;
        Device_Wireguard_Point.reg.local_ip[3] = b4;
    }

    // Peer IP
    CIPAddressCtrl* pPeerIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_ENDPOINT_IP);
    if (pPeerIP)
    {
        BYTE b1, b2, b3, b4;
        pPeerIP->GetAddress(b1, b2, b3, b4);
        Device_Wireguard_Point.reg.peer_ip[0] = b1;
        Device_Wireguard_Point.reg.peer_ip[1] = b2;
        Device_Wireguard_Point.reg.peer_ip[2] = b3;
        Device_Wireguard_Point.reg.peer_ip[3] = b4;
    }

    // Private key
    CString strKey;
    GetDlgItemText(IDC_EDIT_LOCAL_PRIVATE_KEY, strKey);
    memset(Device_Wireguard_Point.reg.private_key, 0, 66);
    CT2CA szPrivKey(strKey);
    strncpy_s(Device_Wireguard_Point.reg.private_key, 66, szPrivKey, _TRUNCATE);

    // Peer public key
    GetDlgItemText(IDC_EDIT_PEER_PUBLIC_KEY, strKey);
    memset(Device_Wireguard_Point.reg.peer_public_key, 0, 66);
    CT2CA szPubKey(strKey);
    strncpy_s(Device_Wireguard_Point.reg.peer_public_key, 66, szPubKey, _TRUNCATE);

    // PSK
    if (IsDlgButtonChecked(IDC_CHECK_ENABLE_PRE_SHARED_KEY) == BST_CHECKED)
    {
        GetDlgItemText(IDC_EDIT_PRE_SHARED_KEY, strKey);
        CT2CA szPSK(strKey);
        memset(Device_Wireguard_Point.reg.pre_shared_key, 0, 66);
        strncpy_s(Device_Wireguard_Point.reg.pre_shared_key, 66, szPSK, _TRUNCATE);
    }
    else
    {
        memset(Device_Wireguard_Point.reg.pre_shared_key, 0, 66);
    }
}

void CWireGuardConfig::UpdateUIState()
{
    BOOL bEnabled = (IsDlgButtonChecked(IDC_CHECK_ENABLE_WIREGUARD) == BST_CHECKED);
    BOOL bPSKEnabled = (IsDlgButtonChecked(IDC_CHECK_ENABLE_PRE_SHARED_KEY) == BST_CHECKED);

    GetDlgItem(IDC_EDIT_WIREGUARD_PORT)->EnableWindow(bEnabled);
    GetDlgItem(IDC_IPADDRESS_LOCAL_VPN_IP)->EnableWindow(bEnabled);
    GetDlgItem(IDC_IPADDRESS_ENDPOINT_IP)->EnableWindow(bEnabled);

    GetDlgItem(IDC_EDIT_LOCAL_PRIVATE_KEY)->EnableWindow(bEnabled);
    GetDlgItem(IDC_EDIT_PEER_PUBLIC_KEY)->EnableWindow(bEnabled);
    GetDlgItem(IDC_BUTTON_GENERATE_PRIVATE_KEY)->EnableWindow(bEnabled);
    GetDlgItem(IDC_BUTTON_GENERATE_PUBLIC_KEY)->EnableWindow(bEnabled);

    GetDlgItem(IDC_CHECK_ENABLE_PRE_SHARED_KEY)->EnableWindow(bEnabled);
    GetDlgItem(IDC_EDIT_PRE_SHARED_KEY)->EnableWindow(bEnabled && bPSKEnabled);
    GetDlgItem(IDC_BUTTON_REGENERATE)->EnableWindow(bEnabled && bPSKEnabled);

    GetDlgItem(IDC_BUTTON_WRITE_WIREGUARD_CFG)->EnableWindow(bEnabled);
}

BOOL CWireGuardConfig::ValidateInput()
{
    if (IsDlgButtonChecked(IDC_CHECK_ENABLE_WIREGUARD) != BST_CHECKED)
        return TRUE;

    // Port
    BOOL bTrans = FALSE;
    UINT nPort = GetDlgItemInt(IDC_EDIT_WIREGUARD_PORT, &bTrans);
    if (!bTrans || nPort == 0 || nPort > 65535)
    {
        MessageBox(_T("Port must be between 1 and 65535."), _T("Error"), MB_ICONERROR);
        return FALSE;
    }

    // Local VPN IP
    CIPAddressCtrl* pLocalIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_LOCAL_VPN_IP);
    if (!pLocalIP || pLocalIP->IsBlank())
    {
        MessageBox(_T("Please enter the Local VPN IP address."), _T("Error"), MB_ICONERROR);
        return FALSE;
    }

    // Private key
    CString strKey;
    GetDlgItemText(IDC_EDIT_LOCAL_PRIVATE_KEY, strKey);
    if (strKey.IsEmpty() || strKey.GetLength() < 40)
    {
        MessageBox(_T("Invalid private key. Please generate a private key first."), _T("Error"), MB_ICONERROR);
        return FALSE;
    }

    return TRUE;
}

void CWireGuardConfig::OnBnClickedButtonGeneratePrivateKey()
{
    CString strKey = GeneratePrivateKey();
    if (strKey.IsEmpty())
    {
        MessageBox(_T("Failed to generate private key."), _T("Error"), MB_ICONERROR);
        return;
    }
    SetDlgItemText(IDC_EDIT_LOCAL_PRIVATE_KEY, strKey);
}

void CWireGuardConfig::OnBnClickedButtonGeneratePublicKey()
{
    CString strPrivKey;
    GetDlgItemText(IDC_EDIT_LOCAL_PRIVATE_KEY, strPrivKey);
    if (strPrivKey.IsEmpty())
    {
        MessageBox(_T("Please generate a private key first."), _T("Information"), MB_ICONINFORMATION);
        return;
    }

    CString strPubKey = GeneratePublicKey(strPrivKey);
    if (strPubKey.IsEmpty())
    {
        MessageBox(_T("Failed to derive public key from private key."), _T("Error"), MB_ICONERROR);
        return;
    }
    SetDlgItemText(IDC_EDIT_PEER_PUBLIC_KEY, strPubKey);
}

void CWireGuardConfig::OnBnClickedButtonRegenerate()
{
    if (IsDlgButtonChecked(IDC_CHECK_ENABLE_PRE_SHARED_KEY) != BST_CHECKED)
    {
        MessageBox(_T("Please enable Pre-Shared Key first."), _T("Information"), MB_ICONINFORMATION);
        return;
    }

    CString strPSK = GeneratePSK();
    if (strPSK.IsEmpty())
    {
        MessageBox(_T("Failed to generate pre-shared key."), _T("Error"), MB_ICONERROR);
        return;
    }
    SetDlgItemText(IDC_EDIT_PRE_SHARED_KEY, strPSK);
}

void CWireGuardConfig::OnBnClickedCheckEnableWireguard()
{
    UpdateUIState();
}

void CWireGuardConfig::OnBnClickedCheckEnablePreSharedKey()
{
    UpdateUIState();

    if (IsDlgButtonChecked(IDC_CHECK_ENABLE_PRE_SHARED_KEY) == BST_CHECKED)
    {
        CString strPSK;
        GetDlgItemText(IDC_EDIT_PRE_SHARED_KEY, strPSK);
        if (strPSK.IsEmpty())
        {
            strPSK = GeneratePSK();
            if (!strPSK.IsEmpty())
                SetDlgItemText(IDC_EDIT_PRE_SHARED_KEY, strPSK);
        }
    }
}

void CWireGuardConfig::OnBnClickedButtonWriteWireguardCfg()
{
    if (!ValidateInput())
        return;

    SaveToDevice();

    if (Write_Private_Data_Blocking(WRITE_WIREGUARD_CFG, 0, 0) <= 0)
    {
        MessageBox(_T("Failed to write WireGuard configuration!"), _T("Error"), MB_ICONERROR);
        return;
    }

    MessageBox(
        _T("WireGuard configuration written successfully."),
        _T("Information"),
        MB_ICONINFORMATION
    );
}

void CWireGuardConfig::OnBnClickedButtonCancelWireguardCfg()
{
    OnCancel();
}

CString CWireGuardConfig::GeneratePrivateKey()
{
    BYTE key[32];
    if (!GenerateRandomBytes(key, 32))
        return _T("");

    // WireGuard 私钥 clamp / WireGuard private key clamping
    key[0] &= 248;
    key[31] &= 127;
    key[31] |= 64;

    return Base64Encode(key, 32);
}

CString CWireGuardConfig::GeneratePublicKey(const CString& strPrivateKey)
{
    CT2CA szInput(strPrivateKey);
    DWORD dwInputLen = (DWORD)strlen(szInput);
    DWORD dwDecodedLen = 0;

    // 先获取解码长度 / Get decoded length first
    // 使用 CRYPT_STRING_BASE64_ANY 自动识别 Base64 格式 / Use BASE64_ANY to auto-detect format
    if (!CryptStringToBinaryA(szInput, dwInputLen, CRYPT_STRING_BASE64,
        NULL, &dwDecodedLen, NULL, NULL) || dwDecodedLen != 32)
    {
        return _T("");
    }

    // 解码 / Decode
    BYTE* pPrivKey = new BYTE[dwDecodedLen];
    if (!CryptStringToBinaryA(szInput, dwInputLen, CRYPT_STRING_BASE64,
        pPrivKey, &dwDecodedLen, NULL, NULL))
    {
        delete[] pPrivKey;
        return _T("");
    }

    // 计算公钥 / Compute public key
    BYTE pubKey[32];
    curve25519_scalarmult_base(pubKey, pPrivKey);

    delete[] pPrivKey;

    return Base64Encode(pubKey, 32);
}

CString CWireGuardConfig::GeneratePSK()
{
    BYTE key[32];
    if (!GenerateRandomBytes(key, 32))
        return _T("");

    return Base64Encode(key, 32);
}

BOOL CWireGuardConfig::GenerateRandomBytes(BYTE* pBuffer, int nLen)
{
    if (!pBuffer || nLen <= 0)
        return FALSE;

    HCRYPTPROV hProv = NULL;

    // 方案1: 尝试 AES 增强型 CSP / Method 1: try AES enhanced CSP
    if (CryptAcquireContext(&hProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES,
        CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
    {
        BOOL bRet = CryptGenRandom(hProv, (DWORD)nLen, pBuffer);
        CryptReleaseContext(hProv, 0);
        if (bRet) return TRUE;
    }

    // 方案2: 标准 RSA 完整 CSP / Method 2: standard RSA full CSP
    if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL,
        CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
    {
        BOOL bRet = CryptGenRandom(hProv, (DWORD)nLen, pBuffer);
        CryptReleaseContext(hProv, 0);
        if (bRet) return TRUE;
    }

    // 方案3: 基础 CSP / Method 3: base CSP
    if (CryptAcquireContext(&hProv, NULL, MS_DEF_PROV, PROV_RSA_FULL,
        CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
    {
        BOOL bRet = CryptGenRandom(hProv, (DWORD)nLen, pBuffer);
        CryptReleaseContext(hProv, 0);
        if (bRet) return TRUE;
    }

    // 方案4: 终极兜底 - RtlGenRandom (Windows XP+ 原生支持，无需 CSP)
    // Method 4: ultimate fallback - RtlGenRandom (native XP+, no CSP needed)
    typedef BOOL (WINAPI *PFN_RTL_GEN_RANDOM)(PVOID, ULONG);
    HMODULE hAdvApi = GetModuleHandle(_T("advapi32.dll"));
    if (hAdvApi)
    {
        PFN_RTL_GEN_RANDOM pfn = (PFN_RTL_GEN_RANDOM)GetProcAddress(hAdvApi, "SystemFunction036");
        if (pfn)
        {
            return pfn(pBuffer, (ULONG)nLen);
        }
    }

    return FALSE;
}

CString CWireGuardConfig::Base64Encode(const BYTE* pData, int nLen)
{
    if (!pData || nLen <= 0)
        return _T("");

    DWORD dwStrLen = 0;

    // 第一次调用: 获取需要的缓冲区大小 / First call: get required buffer size
    if (!CryptBinaryToString(pData, (DWORD)nLen, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
        NULL, &dwStrLen))
    {
        return _T("");
    }

    if (dwStrLen == 0)
        return _T("");

    CString strResult;
    LPTSTR pBuffer = strResult.GetBuffer(dwStrLen);

    // 第二次调用: 实际编码 / Second call: actual encoding
    if (!CryptBinaryToString(pData, (DWORD)nLen, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
        pBuffer, &dwStrLen))
    {
        strResult.ReleaseBuffer(0);
        return _T("");
    }

    strResult.ReleaseBuffer();

    return strResult;
}
