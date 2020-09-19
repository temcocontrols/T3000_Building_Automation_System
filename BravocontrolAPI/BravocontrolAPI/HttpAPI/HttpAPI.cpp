// BravocontrolAPI.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "HttpAPI.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT
using namespace std;

#include "httplib.h"
#include <iostream>
#include "..\..\..\T3000\HttpApiDefine.h"

//This function coded by Fance,used to split the cstring to each part.
void SplitCStringA(CStringArray &saArray, CString sSource, CString sToken)
{
    CString sTempSource, sTempSplitted;

    sTempSource = sSource;

    int nPos = sTempSource.Find(sToken);

    //--if there are no token in the string, then add itself and return.
    if (nPos == -1)
        saArray.Add(sTempSource);
    else
    {
        while (sTempSource.GetLength() > 0)
        {
            nPos = sTempSource.Find(sToken);
            if (nPos == -1)
            {
                saArray.Add(sTempSource.Trim());
                break;
            }
            else if (nPos == 0)
            {
                sTempSource = sTempSource.Mid(sToken.GetLength(), sTempSource.GetLength());
                continue;
            }
            else
            {
                sTempSplitted = sTempSource.Mid(0, nPos);
                saArray.Add(sTempSplitted.Trim());
                sTempSource = sTempSource.Mid(nPos + sToken.GetLength(), sTempSource.GetLength());
            }
        }
    }

}
Http_key k_productID;
Http_key k_serialNumber;
Http_key k_ipAddress;
Http_key k_ipPort;
Http_key k_hardwareVer;
Http_key k_softwareVer;
Http_key k_lastCommunicationDate;
Http_key k_createdAt;
Http_key k_updatedAt;
void Initial_GetProductID(StrGetProductID &sGetPIDInfo)
{
    sGetPIDInfo.k_productID.KeyName = _T("productID");
    sGetPIDInfo.k_serialNumber.KeyName = _T("serialNumber");
    sGetPIDInfo.k_ipAddress.KeyName = _T("ipAddress");
    sGetPIDInfo.k_ipPort.KeyName = _T("ipPort");
    sGetPIDInfo.k_hardwareVer.KeyName = _T("hardwareVer");
    sGetPIDInfo.k_softwareVer.KeyName = _T("softwareVer");
    sGetPIDInfo.k_lastCommunicationDate.KeyName = _T("lastCommunicationDate");
    sGetPIDInfo.k_createdAt.KeyName = _T("createdAt");
    sGetPIDInfo.k_updatedAt.KeyName = _T("updatedAt");
}

void Initial_GetFileInfo(StrGetFileInfo &sGetFileInfo)
{
    sGetFileInfo.k_file_name.KeyName = _T("\"name\"");
    sGetFileInfo.k_file_size.KeyName = _T("\"size\"");
    sGetFileInfo.k_version.KeyName = _T("\"version\"");
    sGetFileInfo.k_updatedAt.KeyName = _T("\"updatedAt\"");
}

void Initial_PostProductID(StrPostProductID &sPostPIDInfo)
{
    sPostPIDInfo.k_serialNumber.KeyName = _T("serialNumber");
    sPostPIDInfo.k_ipAddress.KeyName = _T("ipAddress");
    sPostPIDInfo.k_ipPort.KeyName = _T("ipPort");
    sPostPIDInfo.k_hardwareVer.KeyName = _T("hardwareVer");
    sPostPIDInfo.k_softwareVer.KeyName = _T("softwareVer");
}


OUTPUT int https_get_file(int npid, StrGetFileInfo &sGetFileInfo)
{
    Initial_GetFileInfo(sGetFileInfo);
    CString temp_pid;
    temp_pid.Format(_T("%d"), npid);
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    httplib::SSLClient cli("api.bravocontrols.com");
#else
    httplib::Client cli("api.bravocontrols.com");
#endif

    CString apiPath = _T("/customerProduct/") + temp_pid + _T("/firmware");
    char cTemp1[255];
    memset(cTemp1, 0, 255);
    WideCharToMultiByte(CP_ACP, 0, apiPath.GetBuffer(), -1, cTemp1, 255, NULL, NULL);


    const char *apiPathWithID = cTemp1;
    auto res = cli.Get(apiPathWithID);
    if (res)
    {
        string nret = res->body;
        CString cstr(nret.c_str());
        int n_find_ret = 0;
        //n_find_ret = cstr.Find(_T("No product found!"));
        //if (n_find_ret != -1)
        //{
        //    sGetPIDInfo.cs_results = cstr;
        //    return -2;
        //}
        CStringArray temp_array;
        SplitCStringA(temp_array, cstr, _T(","));

        for (int i = 0; i < temp_array.GetSize(); i++)
        {
            CString temp_cs;
            temp_cs = temp_array.GetAt(i);
            n_find_ret = 0;


            if (sGetFileInfo.k_version.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetFileInfo.k_version.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, _T(":"));
                    if (temp_array_key.GetSize() == 2)  //必须是分成两段
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(1);
                        sGetFileInfo.k_version.nStatus = VALUE_TYPE_FLOAT;
                        sGetFileInfo.k_version.tValue.cs_value = cs_part2;
                        sGetFileInfo.k_version.tValue.cs_value.Trim(_T("\""));
                        sGetFileInfo.k_version.tValue.f_value = _wtof(sGetFileInfo.k_version.tValue.cs_value);
                    }
                    else
                    {
                        sGetFileInfo.k_version.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }


            if (sGetFileInfo.k_file_name.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetFileInfo.k_file_name.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, sGetFileInfo.k_file_name.KeyName + _T(":"));
                    if (temp_array_key.GetSize() >= 1)  //时间不用强制分成两段 ， 因为里面含有冒号
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(temp_array_key.GetSize() - 1);
                        sGetFileInfo.k_file_name.nStatus = VALUE_TYPE_CSTRING;
                        sGetFileInfo.k_file_name.tValue.cs_value = cs_part2;
                        sGetFileInfo.k_file_name.tValue.cs_value.Trim(_T("\""));
                    }
                    else
                    {
                        sGetFileInfo.k_file_name.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }

            if (sGetFileInfo.k_file_size.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetFileInfo.k_file_size.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, sGetFileInfo.k_file_size.KeyName + _T(":"));
                    if (temp_array_key.GetSize() >= 2)  //时间不用强制分成两段 ， 因为里面含有冒号
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(1);
                        sGetFileInfo.k_file_size.nStatus = VALUE_TYPE_FLOAT;
                        sGetFileInfo.k_file_size.tValue.cs_value = cs_part2;
                        sGetFileInfo.k_file_size.tValue.cs_value.Trim(_T("\""));
                        sGetFileInfo.k_file_size.tValue.f_value = _wtof(sGetFileInfo.k_file_size.tValue.cs_value);
                    }
                    else
                    {
                        sGetFileInfo.k_file_size.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }

            if (sGetFileInfo.k_updatedAt.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetFileInfo.k_updatedAt.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, sGetFileInfo.k_updatedAt.KeyName + _T("\":"));
                    if (temp_array_key.GetSize() >= 2)  //时间不用强制分成两段 ， 因为里面含有冒号
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(1);
                        sGetFileInfo.k_updatedAt.nStatus = VALUE_TYPE_CSTRING;
                        sGetFileInfo.k_updatedAt.tValue.cs_value = cs_part2;
                        sGetFileInfo.k_updatedAt.tValue.cs_value.Trim(_T("\""));
                    }
                    else
                    {
                        sGetFileInfo.k_updatedAt.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }




        }

        Sleep(1);
        //cout << res->body << endl;
    }
    else
    {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        auto result = cli.get_openssl_verify_result();
        if (result)
        {
            Sleep(1);
            //cout << "verify error: " << X509_verify_cert_error_string(result) << endl;
        }
        return -1;
#endif
    }
    return 1;

}

OUTPUT int https_get(int npid, StrGetProductID &sGetPIDInfo)
//OUTPUT int https_get(void)
{
    Initial_GetProductID(sGetPIDInfo);
    CString temp_pid;
    temp_pid.Format(_T("%d"), npid);
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    httplib::SSLClient cli("api.bravocontrols.com");
#else
    httplib::Client cli("api.bravocontrols.com");
#endif

    CString apiPath = _T("/customerProduct/") + temp_pid;
    //CString apiPath = _T("/customerProduct/") + temp_pid + _T("/software");

    char cTemp1[255];
    memset(cTemp1, 0, 255);
    WideCharToMultiByte(CP_ACP, 0, apiPath.GetBuffer(), -1, cTemp1, 255, NULL, NULL);


    const char *apiPathWithID = cTemp1;
    auto res = cli.Get(apiPathWithID);
    if (res) 
    {
        string nret = res->body;
        CString cstr(nret.c_str()); 
        int n_find_ret = 0;
        n_find_ret = cstr.Find(_T("No product found!"));
        if (n_find_ret != -1)
        {
            sGetPIDInfo.cs_results = cstr;
            return -2;
        }
        CStringArray temp_array; 
        SplitCStringA(temp_array, cstr, _T(","));

        for (int i = 0; i < temp_array.GetSize(); i++)
        {
            CString temp_cs;
            temp_cs = temp_array.GetAt(i);
            n_find_ret = 0;

            if (sGetPIDInfo.k_productID.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetPIDInfo.k_productID.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, _T(":"));
                    if (temp_array_key.GetSize() == 2)  //必须是分成两段
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(1);
                        sGetPIDInfo.k_productID.nStatus = VALUE_TYPE_INT;
                        sGetPIDInfo.k_productID.tValue.cs_value = cs_part2;
                        sGetPIDInfo.k_productID.tValue.n_value = _wtoi(cs_part2);
                    }
                    else
                    {
                        sGetPIDInfo.k_productID.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }

            if (sGetPIDInfo.k_serialNumber.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetPIDInfo.k_serialNumber.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, _T(":"));
                    if (temp_array_key.GetSize() == 2)  //必须是分成两段
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(1);
                        sGetPIDInfo.k_serialNumber.nStatus = VALUE_TYPE_INT;
                        sGetPIDInfo.k_serialNumber.tValue.cs_value = cs_part2;
                        sGetPIDInfo.k_serialNumber.tValue.n_value = _wtoi(cs_part2);
                    }
                    else
                    {
                        sGetPIDInfo.k_serialNumber.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }

            if (sGetPIDInfo.k_ipAddress.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetPIDInfo.k_ipAddress.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, _T(":"));
                    if (temp_array_key.GetSize() == 2)  //必须是分成两段
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(1);
                        sGetPIDInfo.k_ipAddress.nStatus = VALUE_TYPE_CSTRING;
                        sGetPIDInfo.k_ipAddress.tValue.cs_value = cs_part2;
                        sGetPIDInfo.k_ipAddress.tValue.cs_value.Trim(_T("\""));
                    }
                    else
                    {
                        sGetPIDInfo.k_ipAddress.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }

            if (sGetPIDInfo.k_ipPort.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetPIDInfo.k_ipPort.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, _T(":"));
                    if (temp_array_key.GetSize() == 2)  //必须是分成两段
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(1);
                        sGetPIDInfo.k_ipPort.nStatus = VALUE_TYPE_INT;
                        sGetPIDInfo.k_ipPort.tValue.cs_value = cs_part2;
                        sGetPIDInfo.k_ipPort.tValue.n_value = _wtoi(cs_part2);
                    }
                    else
                    {
                        sGetPIDInfo.k_ipPort.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }

            if (sGetPIDInfo.k_hardwareVer.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetPIDInfo.k_hardwareVer.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, _T(":"));
                    if (temp_array_key.GetSize() == 2)  //必须是分成两段
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(1);
                        sGetPIDInfo.k_hardwareVer.nStatus = VALUE_TYPE_FLOAT;
                        sGetPIDInfo.k_hardwareVer.tValue.cs_value = cs_part2;
                        sGetPIDInfo.k_hardwareVer.tValue.cs_value.Trim(_T("\""));
                        sGetPIDInfo.k_hardwareVer.tValue.f_value = _wtof(sGetPIDInfo.k_hardwareVer.tValue.cs_value);
                    }
                    else
                    {
                        sGetPIDInfo.k_hardwareVer.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }

            if (sGetPIDInfo.k_softwareVer.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetPIDInfo.k_softwareVer.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, _T(":"));
                    if (temp_array_key.GetSize() == 2)  //必须是分成两段
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(1);
                        sGetPIDInfo.k_softwareVer.nStatus = VALUE_TYPE_FLOAT;
                        sGetPIDInfo.k_softwareVer.tValue.cs_value = cs_part2;
                        sGetPIDInfo.k_softwareVer.tValue.cs_value.Trim(_T("\""));
                        sGetPIDInfo.k_softwareVer.tValue.f_value = _wtof(sGetPIDInfo.k_softwareVer.tValue.cs_value);
                    }
                    else
                    {
                        sGetPIDInfo.k_softwareVer.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }

            if (sGetPIDInfo.k_lastCommunicationDate.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetPIDInfo.k_lastCommunicationDate.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, sGetPIDInfo.k_lastCommunicationDate.KeyName + _T("\":"));
                    if (temp_array_key.GetSize() >= 2)  //时间不用强制分成两段 ， 因为里面含有冒号
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(1);
                        sGetPIDInfo.k_lastCommunicationDate.nStatus = VALUE_TYPE_CSTRING;
                        sGetPIDInfo.k_lastCommunicationDate.tValue.cs_value = cs_part2;
                        sGetPIDInfo.k_lastCommunicationDate.tValue.cs_value.Trim(_T("\""));
                    }
                    else
                    {
                        sGetPIDInfo.k_lastCommunicationDate.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }

            if (sGetPIDInfo.k_createdAt.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetPIDInfo.k_createdAt.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, sGetPIDInfo.k_createdAt.KeyName + _T("\":"));
                    if (temp_array_key.GetSize() >= 2)  //时间不用强制分成两段 ， 因为里面含有冒号
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(1);
                        sGetPIDInfo.k_createdAt.nStatus = VALUE_TYPE_CSTRING; 
                        sGetPIDInfo.k_createdAt.tValue.cs_value = cs_part2;
                        sGetPIDInfo.k_createdAt.tValue.cs_value.Trim(_T("\""));
                    }
                    else
                    {
                        sGetPIDInfo.k_createdAt.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }

            if (sGetPIDInfo.k_updatedAt.nStatus == 0)
            {
                n_find_ret = temp_cs.Find(sGetPIDInfo.k_updatedAt.KeyName);
                if (n_find_ret != -1) //找到了对应的关键字
                {
                    //解析出对应的值;
                    CStringArray temp_array_key;
                    SplitCStringA(temp_array_key, temp_cs, sGetPIDInfo.k_updatedAt.KeyName + _T("\":"));
                    if (temp_array_key.GetSize() >= 2)  //时间不用强制分成两段 ， 因为里面含有冒号
                    {
                        CString cs_part2;
                        cs_part2 = temp_array_key.GetAt(1);
                        sGetPIDInfo.k_updatedAt.nStatus = VALUE_TYPE_CSTRING;
                        sGetPIDInfo.k_updatedAt.tValue.cs_value = cs_part2;
                        sGetPIDInfo.k_updatedAt.tValue.cs_value.Trim(_T("\""));
                    }
                    else
                    {
                        sGetPIDInfo.k_updatedAt.nStatus = VALUE_TYPE_ANALYSIS_ERROR;
                    }
                    continue;
                }
            }




        }

        Sleep(1);
        //cout << res->body << endl;
    }
    else
    {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        auto result = cli.get_openssl_verify_result();
        if (result)
        {
            Sleep(1);
            //cout << "verify error: " << X509_verify_cert_error_string(result) << endl;
        }
        return -1;
#endif
    }
    return 1;
}


OUTPUT int https_post(int npid, StrPostProductID &sPostPIDInfo)
{
    Initial_PostProductID(sPostPIDInfo);
    CString temp_pid;
    temp_pid.Format(_T("%d"), npid);
    CString temp_serialNumber;
    CString temp_ipaddress;
    CString temp_ipport;
    CString temp_hw_ver;
    CString temp_sf_ver;

    CString total_string;

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    httplib::SSLClient cli("api.bravocontrols.com");
#else
    httplib::Client cli("api.bravocontrols.com");
#endif

    CString apiPath = _T("/customerProduct/") + temp_pid;
    //const char *apiPathWithID = apiPath.c_str();

    char cTemp1[255];
    memset(cTemp1, 0, 255);
    WideCharToMultiByte(CP_ACP, 0, apiPath.GetBuffer(), -1, cTemp1, 255, NULL, NULL);
    const char *apiPathWithID = cTemp1;

    CString cs_params;
    CString temp1;
    cs_params.Format(_T("{"));
    int need_douhao = 0;
    if (sPostPIDInfo.k_serialNumber.nStatus != 0)
    {
        temp1.Format(_T("\"%s\":%d"), sPostPIDInfo.k_serialNumber.KeyName, sPostPIDInfo.k_serialNumber.tValue.n_value);
        need_douhao = 1;
        cs_params = cs_params + temp1;
    }

    if (sPostPIDInfo.k_ipAddress.nStatus != 0)
    {

        temp1.Format(_T("\"%s\":\"%s\""), sPostPIDInfo.k_ipAddress.KeyName, sPostPIDInfo.k_ipAddress.tValue.cs_value);
        if (need_douhao)
        {
            temp1 = _T(",") + temp1;
        }
        need_douhao = 1;
        cs_params = cs_params + temp1;
    }

    if (sPostPIDInfo.k_ipPort.nStatus != 0)
    {
        temp1.Format(_T("\"%s\":%d"), sPostPIDInfo.k_ipPort.KeyName, sPostPIDInfo.k_ipPort.tValue.n_value);
        if (need_douhao)
        {
            temp1 = _T(",") + temp1;
        }
        need_douhao = 1;
        cs_params = cs_params + temp1;
    }

    if (sPostPIDInfo.k_hardwareVer.nStatus != 0)
    {

        temp1.Format(_T("\"%s\":\"%s\""), sPostPIDInfo.k_hardwareVer.KeyName, sPostPIDInfo.k_hardwareVer.tValue.cs_value);
        if (need_douhao)
        {
            temp1 = _T(",") + temp1;
        }
        need_douhao = 1;
        cs_params = cs_params + temp1;
    }

    if (sPostPIDInfo.k_softwareVer.nStatus != 0)
    {

        temp1.Format(_T("\"%s\":\"%s\""), sPostPIDInfo.k_softwareVer.KeyName, sPostPIDInfo.k_softwareVer.tValue.cs_value);
        if (need_douhao)
        {
            temp1 = _T(",") + temp1;
        }
        need_douhao = 1;
        cs_params = cs_params + temp1;
    }
    
    if (need_douhao == 0)
    {
        return -1;
    }
    cs_params = cs_params + _T("}");
    std::string params = ((CStringA)CW2A(cs_params)).GetBuffer();

    //string params = "{"
    //    "\"serialNumber\":\"" + serialNumber + "\","
    //    "\"ipPort\":\"" + ipPort + "\","
    //    "\"ipAddress\":\"" + ipAddress + "\""
    //    "}";
    auto res = cli.Post(apiPathWithID, params, "application/json");
    if (res) 
    {
        // cout << res->status << endl;
        // cout << res->get_header_value("Content-Type") << endl;
        cout << res->body << endl;
    }
    else 
    {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        auto result = cli.get_openssl_verify_result();
        if (result) 
        {
            cout << "verify error: " << X509_verify_cert_error_string(result) << endl;
        }
#endif
    }
    return 1;
}

#if 1
int https_put(void) {
    string id;
    string serialNumber;
    string ipPort;
    string ipAddress;
    cout << "Please enter the product id you want to update : ";
    cin >> id;
    cout << "Please enter the new product ip address : ";
    cin >> ipAddress;
    cout << "Please enter the new product serial number : ";
    cin >> serialNumber;
    cout << "Please enter the new product ip port : ";
    cin >> ipPort;
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    httplib::SSLClient cli("api.bravocontrols.com");
#else
    httplib::Client cli("api.bravocontrols.com");
#endif
    string apiPath = "/software/T3000/" + id;
    const char *apiPathWithID = apiPath.c_str();
    string params = "{"
        "\"serialNumber\":\"" + serialNumber + "\","
        "\"ipPort\":\"" + ipPort + "\","
        "\"ipAddress\":\"" + ipAddress + "\""
        "}";
    auto res = cli.Post(apiPathWithID, params, "application/json");
    if (res) {
        // cout << res->status << endl;
        // cout << res->get_header_value("Content-Type") << endl;
        cout << res->body << endl;
    }
    else {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        auto result = cli.get_openssl_verify_result();
        if (result) {
            cout << "verify error: " << X509_verify_cert_error_string(result) << endl;
        }
#endif
    }

    return 0;
}
#endif


