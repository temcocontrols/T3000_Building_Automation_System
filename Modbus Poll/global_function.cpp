

#include "stdafx.h"
#include "global_function.h"
#include "Windows.h"
#include "ModbusDllForVC.h"
#include "gloab_define.h"
 
#include "global_variable.h"
#define DELAY_TIME	 10	//MS
#define Modbus_Serial	0
#define	Modbus_TCP	1



BOOL GetSerialComPortNumber1(vector<CString>& szComm)
{
	LPCTSTR strRegEntry = _T("HARDWARE\\DEVICEMAP\\SERIALCOMM\\");

	HKEY   hKey;   
	LONG   lReturnCode=0;   
	lReturnCode=::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegEntry, 0, KEY_READ, &hKey);   

	if(lReturnCode==ERROR_SUCCESS)   
	{  
		DWORD dwIndex = 0;
		WCHAR lpValueName[MAX_PATH];
		ZeroMemory(lpValueName, MAX_PATH);
		DWORD lpcchValueName = MAX_PATH; 
		DWORD lpReserved = 0; 
		DWORD lpType = REG_SZ; 
		BYTE		lpData[MAX_PATH]; 
		ZeroMemory(lpData, MAX_PATH);
		DWORD lpcbData = MAX_PATH;
		dwIndex = 0;
		while(RegEnumValue(	hKey, dwIndex, lpValueName, &lpcchValueName, 0, &lpType, lpData, &lpcbData ) != ERROR_NO_MORE_ITEMS)
		{   
			//TRACE("Registry's   Read!");   
			dwIndex++;

			lpcchValueName = MAX_PATH; 
			//lpValueName[0] = '\0'; 
			
			CString strValueName= CString(lpValueName);
		
			WCHAR ch[MAX_PATH];
			ZeroMemory(ch, MAX_PATH);
			memcpy(ch, lpData, lpcbData);
			CString str = CString(ch);
			szComm.push_back(str);

			ZeroMemory(lpData, MAX_PATH);
			lpcbData = MAX_PATH;

		}   
		::RegCloseKey(hKey);   		   

		return TRUE;
	}

	return FALSE;   
}
void SplitCStringA(CStringArray &saArray, CString sSource, CString sToken)
{
	CString sTempSource, sTempSplitted;

	sTempSource = sSource;

	int nPos = sTempSource.Find(sToken);

	//--if there are no token in the string, then add itself and return.
	if(nPos == -1)
		saArray.Add(sTempSource);
	else
	{
		while(sTempSource.GetLength() > 0)
		{
			nPos = sTempSource.Find(sToken);
			if(nPos == -1)
			{
				saArray.Add(sTempSource.Trim());
				break;
			}
			else if(nPos == 0)
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
BOOL Get_Bit_FromRegister(unsigned short RegisterValue,unsigned short Position){
 
int postionvalue=1;
  postionvalue=postionvalue<<(Position-1);
  postionvalue= RegisterValue&postionvalue;
  BOOL ret=postionvalue>>(Position-1);
  return ret;
}

CString get_ExePath(){

	TCHAR exeFullPath[MAX_PATH+1]; //
	GetModuleFileName(NULL, exeFullPath, MAX_PATH); //
	(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;//
	g_strExEPath=exeFullPath;//
	g_configfile_path=g_strExEPath+_T("T3000_config.ini");
	return g_strExEPath;
}

void Traffic_Data(CString &nCString)
{
	static int count = 0;
	CTime print_time=CTime::GetCurrentTime();
	CString str=print_time.Format("%H:%M:%S    ");
	PrintText[count].Empty();
	PrintText[count] =str + nCString;
	SendMessage(h_data_traffic_window,WM_TRAFFIC_DATA_WINDOW,(WPARAM)PrintText[count].GetBuffer(),NULL);
	count = (count ++) % 90;
}


 void SetPaneString(int nIndext,CString str)
{

	CMFCStatusBar * pStatusBar=NULL;
	if(AfxGetMainWnd()->GetActiveWindow()==NULL)//if this function is called by a thread ,return 
		return;
	pStatusBar = (CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	pStatusBar->SetPaneText(nIndext,str.GetString());
}

  CString GetProductName(int ModelID)
 {
	 CString strProductName;
	 switch(ModelID)
	 {
	 case PM_TSTAT5A:
		 strProductName="TStat5A";
		 break;
	 case PM_TSTAT5B:
		 strProductName="TStat5B";
		 break;
	 case PM_TSTAT5B2:
		 strProductName="TStat5B2";
		 break;
	 case PM_TSTAT5C:
		 strProductName="TStat5C";
		 break;
	 case PM_TSTAT5D:
		 strProductName="TStat5D";
		 break;
	 case PM_TSTAT5E:
		 strProductName="TStat5E";
		 break;
	 case PM_TSTAT5F:
		 strProductName="TStat5F";
		 break;
	 case PM_TSTAT5G:
		 strProductName="TStat5G";
		 break;
	 case PM_TSTAT5H:
		 strProductName="TStat5H";
		 break;
	 case PM_TSTAT6:
		 strProductName="TStat6";
		 break;
	 case PM_TSTAT5i:
		 strProductName="TStat5i";
		 break;
	 case PM_TSTAT7:
		 strProductName="TStat7";
		 break;
	 case PM_NC:
		 strProductName="NC";
		 break;
	 case PM_CM5:
		 strProductName ="CM5";
		 break;
		 //20120424
	 case PM_LightingController:
		 strProductName = "LC";
		 break;
	 case  PM_CO2_NET:
		 strProductName = "CO2 Net";
		 break;
	 case  PM_CO2_RS485:
		 strProductName = "CO2";
		 break;
	 case  PM_CO2_NODE:
		strProductName = "CO2 Node";
		break;
	 case PM_TSTAT6_HUM_Chamber:
		 strProductName = "HumCharmber";
		 break;

	 case PM_T3PT10 :
		 strProductName="T3-PT10";
		 break;
	 case PM_T3IOA :
		 strProductName="T3-8IOA";
		 break;
	 case PM_T332AI :
		 strProductName="T3-32AI";
		 break;
	 case  PM_T38AI16O :
		 strProductName="T3-8AI160";
		 break;
	 case PM_T38I13O :
		 strProductName="T3-8I13O";
		 break;
	 case PM_T3PERFORMANCE :
		 strProductName="T3-Performance";
		 break;
	 case PM_T34AO :
		 strProductName="T3-4AO";
		 break;
	 case PM_T36CT :
		 strProductName="T3-6CT";
		 break;
	 case PM_MINIPANEL:
		 strProductName="MiniPanel";
		 break;
	case  PM_PRESSURE:
		strProductName="Pressure Sensor";
		break;
	case PM_HUM_R:
		strProductName="HUM-R";
		break;
	case PM_TSTATRUNAR:
			strProductName="TStatRunar";
			break;
	case PM_T322AIVG:
		strProductName="T322AIVG";
		break;
	case PM_T38IOVG:
		strProductName="T38IOVG";
		break;
	case PM_T3PTVG:
		strProductName="T3PTVG";
		break;
	 default:
		 strProductName="TStat";
		 break;
	 }
	 return strProductName;
 }