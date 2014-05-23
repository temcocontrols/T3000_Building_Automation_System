#include "StdAfx.h"
#include "FlashSN.h"
#include "Global_Struct.h"
#include <fstream>
CFlashSN::CFlashSN(void)
{
	m_nComPort = 1;
	m_nMBID = 255; 
//	m_dwIPAddr = 0;
	m_nIPPort = 0;

	m_nFlashType = 0;   // 0= com, 1=ethernet
	m_nSerialNumber = 0;
	
	m_nProductModel = 1;
	m_nHardWareVersion = 0;
}
CFlashSN::~CFlashSN(void)
{

}
void CFlashSN::SetComport(int nComport)
{
	m_nComPort = nComport;
}
void CFlashSN::SetIPPort(int nIPPort)
{
	m_nIPPort = nIPPort;
}
void CFlashSN::SetIPAddress(CString dwIPAddr)
{
	m_dwIPAddr = dwIPAddr;
}
void CFlashSN::SetFlashTpye(int nFlashType)
{
	m_nFlashType = nFlashType;
}
BOOL CFlashSN::StartWriteSN()
{
	if (m_nFlashType == 0)
	{
		FlashTstatSN();
	}
    else
    {
        FlashNCSN();
    }
	

	return TRUE;
}



// BOOL CFlashSN::NeedNewSN()
// {
// 	return FALSE;
// }

void CFlashSN::SetFlashParam(const CString	& strProductModel, int nHardWareVersion,int nProductID)
{
	m_strProductModel = strProductModel;
 
	m_nHardWareVersion = nHardWareVersion;
	m_nProductModel=nProductID;
}


unsigned long CFlashSN::GetNewSerialNumber()
{
	int nSerialNumber;		
	CStdioFile file;
			
	if(!file.Open(c_strSNRecordFileName, CFile::shareDenyNone|CFile::modeReadWrite))
	{
		 AfxMessageBox(_T("Error: Cannot find serial records file.\n Check Z:\Serial_Records\serial_records.txt"));
		return -1;
	}

	//----------------------------get the serialnumber--------------------------------------
	///////////////////// read a line from file to the lastest line
	CString strLine,strTemp;	


	while(file.ReadString(strTemp))
	{
		strLine = strTemp;		
	}	
	strTemp = strLine.Mid(0, strLine.Find(_T(',')));
	nSerialNumber = _wtoi(strTemp);//the serialnumber
	file.Close();
	return nSerialNumber+1;
}
void CFlashSN::SetMBID(int ID){
m_nMBID=ID;
}


void CFlashSN::FlashTstatSN()
{
	m_nSerialNumber=GetNewSerialNumber();// get last serialnumber of the file
	if(m_nSerialNumber==-1)//can't fine the serialnumber file on z drive
	{
		AfxMessageBox(_T("WARNING : Can't find serial number file on Z driver."));
		return;
	}

	unsigned int loword,hiword;
	loword=m_nSerialNumber & 0xffff;
	hiword=(m_nSerialNumber >> 16) & 0xffff;
	if (open_com(m_nComPort))
	{
     /*   unsigned short Serial[4];
        int i=0;
        while(i<5){
        int ret=read_multi(m_nMBID,Serial,0,4);
        if (ret>0)
        {
        break;
        }
        ++i;
        }*/
        
		//int nSerialNumber = 
		//	Read_One(m_nMBID,0) +
		//	Read_One(m_nMBID,1) * 256 +
		//	Read_One(m_nMBID,2) * 65536 +           // 256 * 256 
		//	Read_One(m_nMBID,3) * 16777216 ;// 256 *256 *256

       /* if (nSerialNumber!=0)
        {*/
        if (m_nProductModel!=102)
        {
            Write_One(m_nMBID,16,142);
            Sleep(10*1000);
        }

      /*  }*/
		
	if(Write_One(m_nMBID,0,loword)>0 && Write_One(m_nMBID,2,hiword)>0)
	{		
		int nSWVersion=Read_One(m_nMBID, 4);//tstat version			
		if(nSWVersion >= 240 && nSWVersion < 250)
			nSWVersion /=10;
		else 
		{
			nSWVersion = Read_One(m_nMBID,5)*256+Read_One(m_nMBID,4);	
			nSWVersion /=10;
		}//tstat_version		

		if(nSWVersion >= 25)
		{
			Write_One(m_nMBID,7,m_nProductModel);//write product_model register
			Write_One(m_nMBID,8,m_nHardWareVersion);//write hardware_rev register
		}
		int year,month,day;
		char software_version[8]={'\0'};
		time_t curtime;
		struct tm* today = NULL;
		curtime = time(NULL);
		today = localtime(&curtime);
	//	localtime_s(today, &curtime);
		year = today->tm_year + 1900;
		month = today->tm_mon + 1;
		day = today->tm_mday;
		//		getTime(&year,&month,&day);//get the month,day,year of now							
		float fl_temp=(float)nSWVersion;
		CString f_l_temp;
		f_l_temp.Format(_T("%.1f"),fl_temp);
		for(int j=0;j<f_l_temp.GetLength();j++)
			software_version[j]=(char)f_l_temp.GetAt(j);

		//fstream tstat;

		////tstat.open(_T("z:\\Serial_Records\\serial_records.txt"),ios_base::out | ios_base::app);
		//tstat.open(_T("e:\\serial_records.txt"),ios_base::out | ios_base::app);
		//if(tstat.is_open())
		//{
		//	tstat	<<endl<<m_nSerialNumber                    //serialnumber
		//			<<_T(", ")<<m_strProductModel                //product_model
		//			<<_T(", ")<<m_nHardWareVersion           //hardware version
		//			<<_T(", ")<<nSWVersion				        //software version
		//			<<_T(", ")<<month<<_T("-")<<day<<_T("-")<<year;        //the time									
		//}
		//tstat.close();
		CString stringlog;
		stringlog.Format(_T("%d,%s,%d,%s,%d-%d-%d"),
			m_nSerialNumber,
			m_strProductModel,
			m_nHardWareVersion,
			f_l_temp,
			month,
			day,
			year);
		CStdioFile seriallogfile;
		if(seriallogfile.Open(c_strSNRecordFileName.GetString(),CFile::modeReadWrite | CFile::shareDenyNone))
		{

			seriallogfile.SeekToEnd();

			seriallogfile.WriteString(_T("\n")+stringlog);



			seriallogfile.Close();
		}
		else
		{
			ASSERT(0);
			seriallogfile.Close();

		}
		CString index;
		index.Format(_T("SN=%d have been written,sucessfully."),m_nSerialNumber);
		AfxMessageBox(index);
	}
	else
	{
		m_nSerialNumber = 
			Read_One(m_nMBID,0 ) +
			Read_One(m_nMBID,1) * 256 +
			Read_One(m_nMBID,2) * 65536 +           // 256 * 256 
			Read_One(m_nMBID , 3) * 16777216 ;// 256 *256 *256
		CString index;
		index.Format(_T("SN:%d have not been overwritten."),m_nSerialNumber);
		AfxMessageBox(index);
	}
	
	close_com();
	}
	else
	{
		AfxMessageBox(_T("COM Can't Open"));
	}

	 
}


void CFlashSN::SetConfigFileHandler(CConfigFileHandler* pFileHandler)
{
	m_pFileHandler = pFileHandler;
}
void CFlashSN::FlashNCSN()
{
	m_nSerialNumber=GetNewSerialNumber();// get last serialnumber of the file
	unsigned int loword,hiword,loword0,loword1,hiword2,hiword3;
	loword=m_nSerialNumber & 0xffff;

	hiword=(m_nSerialNumber >> 16) & 0xffff;

	loword0=loword&0xff;
	loword1=(loword>>8)&0xff;

	hiword2=hiword&0xff;
	hiword3=(hiword>>8)&0xff;
	if(m_nSerialNumber==-1)//can't fine the serialnumber file on z drive
	{
		AfxMessageBox(_T("WARNING : Can't find serial number file on Z driver."));
		return;
	}
	
	if (!Open_Socket2(m_dwIPAddr,m_nIPPort))
	{
	  CString strmessage;
	  strmessage.Format(_T("Can't Open %s:%d"),m_dwIPAddr,m_nIPPort);
	  AfxMessageBox(strmessage);
	} 
	
	else
	{

		if((Write_One(255,0,loword0)>0) && (Write_One(255,1,loword1)>0)&&(Write_One(255,2,hiword2)>0) && (Write_One(255,3,hiword3)>0))
		{		
			int nSWVersion=Read_One(255, 4);//tstat version			
			if(nSWVersion >= 240 && nSWVersion < 250)
				nSWVersion /=10;
			else 
			{
				nSWVersion = Read_One(255,5)*256+Read_One(255,4);	
				nSWVersion /=10;
			}//tstat_version		

			if(nSWVersion >= 25)
			{
				Write_One(255,7,m_nProductModel);//write product_model register
				Write_One(255,8,m_nHardWareVersion);//write hardware_rev register
			}
			int year,month,day;
			char software_version[8]={'\0'};
			time_t curtime;
			struct tm* today = NULL;
			curtime = time(NULL);
			today = localtime(&curtime);
			//	localtime_s(today, &curtime);
			year = today->tm_year + 1900;
			month = today->tm_mon + 1;
			day = today->tm_mday;
			//		getTime(&year,&month,&day);//get the month,day,year of now							
			float fl_temp=(float)nSWVersion;
			CString f_l_temp;
			f_l_temp.Format(_T("%.1f"),fl_temp);
			for(int j=0;j<f_l_temp.GetLength();j++)
				software_version[j]=(char)f_l_temp.GetAt(j);

			//fstream tstat;

			////tstat.open(_T("z:\\Serial_Records\\serial_records.txt"),ios_base::out | ios_base::app);
			//tstat.open(_T("e:\\serial_records.txt"),ios_base::out | ios_base::app);
			//if(tstat.is_open())
			//{
			//	tstat	<<endl<<m_nSerialNumber                    //serialnumber
			//			<<_T(", ")<<m_strProductModel                //product_model
			//			<<_T(", ")<<m_nHardWareVersion           //hardware version
			//			<<_T(", ")<<nSWVersion				        //software version
			//			<<_T(", ")<<month<<_T("-")<<day<<_T("-")<<year;        //the time									
			//}
			//tstat.close();
			CString stringlog;
			stringlog.Format(_T("%d,%s,%d,%s,%d-%d-%d"),
				m_nSerialNumber,
				m_strProductModel,
				m_nHardWareVersion,
				f_l_temp,
				month,
				day,
				year);
			CStdioFile seriallogfile;
			if(seriallogfile.Open(c_strSNRecordFileName.GetString(),CFile::modeReadWrite | CFile::shareDenyNone))
			{

				seriallogfile.SeekToEnd();

				seriallogfile.WriteString(_T("\n")+stringlog);



				seriallogfile.Close();
			}
			else
			{
				ASSERT(0);
				seriallogfile.Close();

			}
			CString index;
			index.Format(_T("SN=%d have been written,sucessfully."),m_nSerialNumber);
			AfxMessageBox(index);
		}
		else
		{
			m_nSerialNumber = 
				Read_One(255,0 ) +
				Read_One(255,1) * 256 +
				Read_One(255,2) * 65536 +           // 256 * 256 
				Read_One(255 , 3) * 16777216 ;// 256 *256 *256
			CString index;
			index.Format(_T("SN:%d have not been overwritten."),m_nSerialNumber);
			AfxMessageBox(index);
		}
	   close_com;
	}

}
 
int CFlashSN::GetSerialNumber()
{

 return m_nSerialNumber;
}