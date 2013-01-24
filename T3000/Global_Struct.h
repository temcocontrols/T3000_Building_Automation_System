/********************************************************************
	created:	2011/04/07
	created:	7:4:2011   10:23
	filename: 	e:\MyWork\ISP\ISP 4.3\ISP\Global_Struct.h
	file path:	e:\MyWork\ISP\ISP 4.3\ISP
	file base:	Global_Struct
	file ext:	h
	author:		ZGQ
	
	purpose:	 全局常量，以及结构体的声明
*********************************************************************/
#pragma once


const	 int c_nSplashTime = 2000;	// splash window的显示时间
//const TCHAR c_strCfgFileName[] = _T("config.txt");				//	配置文件名称，用于保存用户设置
const TCHAR c_strLogoFileName[] = _T("ISPLogo.jpg");			// logo picture file name

const CString c_strSNRecordFileName = _T("e:\\serial_records.txt");
//////////////////////////////////////////////////////////////////////////
// for setting file




const DWORD c_nHexFileBufLen = 0xFFFF;   // hex 最大64k
const DWORD c_nBinFileBufLen = 0x7FFFF;  // bin 文件最大512k



#define WM_REPLACE_STATUSINFO		WM_USER+1000 
#define WM_ADD_STATUSINFO				WM_USER+1001 
#define WM_FLASH_FINISH					WM_USER+1002

//#define WM_TCP_STATUSINFO				WM_USER+1003


#define MaxAddressNum   255
#define MaxRegisterNum  320
typedef unsigned char		TS_UC   ;
typedef char					TS_C    ;
typedef unsigned short		TS_US   ;
typedef short					TS_S    ;
typedef unsigned int			TS_UI   ;
typedef int						TS_I    ;
typedef long					TS_L    ;
typedef bool					TS_BOOL ;



//extern CString g_strExePath;



///////////////////////////////////////////////////////////////////////////
// for product model file
const TCHAR c_strProductModelFileName[] = _T("ProductModel.cfg");				//	Product Model 配置文件名称


const int c_nModelNum = 27;
const CString g_ProductModel[c_nModelNum] = 
{ _T("TStat5B=1"),
_T("TStat5A=2"),
_T("TStat5B2=3"),
_T("TStat5C=4"),
_T("TStat5D=12"),
_T("TStat5E=16"),
_T("TStat5F=17"),
_T("TStat5G=18"),
_T("TStat5H=19"),
_T("TStat6=6"),
_T("TStat7=7"),
_T("T3-8AIOD=20"),
_T("T3-8IOA=21"),
_T("T3-32AI=22"),
_T("T3-8AI160=23"),
_T("ZigBee=24"),
_T("FlexDriver=25"),
_T("T3-8I13O=26"),
_T("T3-Performance=27"),
_T("T3-4AO=28"),
_T("T3-6CT=29"),
_T("Solar=30"),
_T("MiniPanel=150"),
_T("CM5=50"),
_T("NC=100"),
_T("OR485=102"),
_T("LC=120")

// 		PM_TSTAT5A = 2,   // product_model=2;strcpy(file_product_model, g_strTstat5a);break;//Tstat5A
// 		PM_TSTAT5B = 1,  //1:  product_model=1;strcpy(file_product_model,  g_strTstat5b);break;//Tstat5B	
// 		PM_TSTAT5B2 = 3, //case 2:	 product_model=3;strcpy(file_product_model, g_strTstat5b2);break;//Tstat5B	
// 		case 3:	 product_model=4;strcpy(file_product_model, g_strTstat5c);break;//Tstat5C
// 		case 4:  product_model=12;strcpy(file_product_model, g_strTstat5d);break;//Tstat5D	
// 		case 5:	 product_model=NET_WORK_CONT_PRODUCT_MODEL;strcpy(file_product_model,g_strnetWork);break;//Net Work Controller
// 		case 6:	 product_model=LED_PRODUCT_MODEL;strcpy(file_product_model,"Led");break;//Led
// 		case 7:	 product_model=SOLAR_PRODUCT_MODEL;strcpy(file_product_model,"Solar");break;//Solar
// 		case 8:	 product_model=ZIGBEE_PRODUCT_MODEL;strcpy(file_product_model,"ZigBee");break;//ZigBee
// 		case 9:	 product_model=T3_8IOA_PRODUCT_MODEL;strcpy(file_product_model,"T3-8IOA");break;
// 		case 10:	 product_model=T3_8IOD_PRODUCT_MODEL;strcpy(file_product_model,"T3-8IOD");break;
// 		case 11:	 product_model=T3_32I_PRODUCT_MODEL;strcpy(file_product_model,"T3-32I");break;
// 		case 12:	 product_model=T3_8I_16O_PRODUCT_MODEL;strcpy(file_product_model,"T3-8I-16O");break;
};










///////////////////////////////////////////////////////////////////////////
// for flash sn settings file

const TCHAR c_strFlashSNCfgFileName[] = _T("Flash_SN.cfg");				//	保存FLASH SN 的用户设定

const TCHAR c_strFlashHWVersion[] = _T("Hardware Verson : ");
const TCHAR c_strFlashSNModel[] = _T("Product Model : ");
 