#pragma once
const int VALUE_TYPE_ANALYSIS_ERROR = -1;
const int VALUE_TYPE_INVALID = 0;
const int VALUE_TYPE_CSTRING = 1;
const int VALUE_TYPE_INT = 2;
const int VALUE_TYPE_FLOAT = 3;

typedef struct
{
    CString  cs_value;
    int      n_value;
    float    f_value;

}ValueType;

typedef struct
{
    CString KeyName;
    int nStatus;  // -1     
    ValueType tValue;
}Http_key;

#define GETP_ID         0
#define GETP_FIRMWARE   1
#define GETP_SOFTWARE   2
#define GETP_LASTUPDATE 3
#define GETP_CREATED    4
#define GETP_UPDATED    5
#define GETP_MAX_KEY    6

typedef struct
{
    Http_key key[GETP_MAX_KEY];
    CString cs_results;
}StrGetProductID;


#define GETF_VERSION        0
#define GETF_FILENAME       1
#define GETF_FILESIZE       2
#define GETF_UPDATED        3
#define GETF_MAX_KEY        4
typedef struct
{
    Http_key key[GETF_MAX_KEY];
}StrGetFileInfo;

typedef struct
{
    Http_key k_serialNumber;
    Http_key k_ipAddress;
    Http_key k_ipPort;
    Http_key k_hardwareVer;
    Http_key k_softwareVer;
}StrPostProductID;




