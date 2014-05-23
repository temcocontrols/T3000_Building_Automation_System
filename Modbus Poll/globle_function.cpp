
#include "stdafx.h"
#include "globle_function.h"
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
	g_configfile_path=g_strExEPath+_T("config.ini");
	return g_strExEPath;
}

void Traffic_Data(CString &nCString)
{
	static int count = 0;
	CTime print_time=CTime::GetCurrentTime();
	CString str=print_time.Format("%H:%M:%S    ");
	PrintText[count].Empty();
	PrintText[count] =str + nCString;
	PostMessage(h_data_traffic_window,WM_TRAFFIC_DATA_WINDOW,(WPARAM)PrintText[count].GetBuffer(),NULL);
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