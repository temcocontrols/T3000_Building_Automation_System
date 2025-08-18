//*********common.cpp**********
#include "stdafx.h"
#include "common.h"
#include "crc.h"
#include <bitset>
using namespace std;

#define TRUE_OR_FALSE	true
#define SLEEP_TIME		50
// Serial port delay, must be 600, when set to 500 or smaller, NC scan TStat is prone to failure
//#define  LATENCY_TIME_COM	60      // �����ӳ٣�����600����Ϊ500���Сʱ��ͨ��NC scan TStat����ʧ��
//#define  LATENCY_TIME_NET	100

extern TS_US LATENCY_TIME_COM ;
extern  TS_US LATENCY_TIME_NET ;
//******************************extern variable
 
CString g_cs_com_section_mulwrite = _T("Write");
CString g_cs_com_section_read = _T("Read");
 
// Serial port handle
extern HANDLE m_hSerial;//���ھ�� // Serial port handle
extern HANDLE m_com_h_serial[100];
// For multi-threaded concurrent overlapped read/write operations
extern OVERLAPPED m_com_osRead[100], m_com_osWrite[100], m_com_osMulWrite[100]; // ���ڶ��߳�ͬʱ�ص���/д
// For overlapped read/write operations
extern OVERLAPPED m_osRead, m_osWrite, m_osMulWrite; // �����ص���/д - For overlapped read/write
extern TS_UC  gval[13];//the data that get from com
extern TS_UC  serinumber_in_dll[4];//only read_one function ,when read 10,
extern TS_UC  pval[13];//the data that send from com
extern TS_UC  multi_read_val[256];//the register value is put into here,by multi_read function//the number must less 256
static int baudrate_in_dll=0;
static unsigned char com_data_bit_in_dll = 8;
static unsigned char com_stop_bit_in_dll = ONESTOPBIT;
static unsigned char com_parity_bit_in_dll = NOPARITY;
static int open_com_port_number_in_dll=65535;
static int old_or_new_scan_protocal_in_dll=1;//1==new protocal;2==old protocal
extern 	SOCKET m_hSocket;
extern SOCKET m_bip_socket;
extern  SOCKET m_hSocket_for_list;
// For multi-threaded scanning
extern 	SOCKET m_tcp_hSocket[256];  //���ڶ��߳�ɨ�� - For multi-threaded scanning
int g_Commu_type=0;//0:serial modus//
CString last_connected_ip;
int last_connected_port = 0;
int last_communication_type = 0;
int successful_baudrate = 19200;
int successful_com_port = 0;
//CMutex scan_mutex;

unsigned short trans_id = 0;
unsigned short trans_high_id = 0;
unsigned short trans_low_id = 0;

CStdioFile* g_fileScanLog = NULL;
CStdioFile* g_fileScanNetLog=NULL;

const int ArrayBaudate[BAUDRATENUMBER] = {
    9600,
    19200,
    38400,
    57600,
    76800,
    115200
    };
OUTPUT void SetLastSuccessBaudrate(int nbaudrate)
{
    successful_baudrate = nbaudrate;
}

OUTPUT void SetLastOpenedComport(int ncom_port)
{
    successful_com_port = ncom_port;
}

OUTPUT void SetLastCommunicationType(int n_commnication_type)
{
    last_communication_type = n_commnication_type;
}


OUTPUT int GetLastSuccessBaudrate()
{
    return successful_baudrate;
}

OUTPUT int GetLastOpenedComport()
{
    return open_com_port_number_in_dll;
}

OUTPUT int GetLastCommunicationType()
{
    return last_communication_type;
}

OUTPUT void SetCommunicationType(int nType)
{
    g_Commu_type=nType;
}

OUTPUT int GetCommunicationType(void)
{
    return g_Commu_type;
}


 

OUTPUT void close_com_nocritical(int ncomport)
{
    if (g_Commu_type == 0)
    {
        if (m_com_h_serial[ncomport] != NULL)
        {
            // Close serial port
            //�رմ��� - Close serial port
            CloseHandle(m_com_h_serial[ncomport]);
            m_com_h_serial[ncomport] = NULL;
        }
        m_hSerial = NULL;
        baudrate_in_dll = 0;
        open_com_port_number_in_dll = 65535;
    }
    if (g_Commu_type == 1)
    {
        if (m_hSocket != INVALID_SOCKET)
        {
            ::closesocket(m_hSocket);
            m_hSocket = NULL;
        }
    }

}

OUTPUT void close_com()
{
    if(g_Commu_type==0)
    {
        if(m_hSerial != NULL)
        {
            // Close serial port
            //�رմ���
            CloseHandle(m_hSerial);
            m_hSerial = NULL;
        }
        baudrate_in_dll=0;
        open_com_port_number_in_dll=65535;
    }
    if (g_Commu_type==1)
    {
        if (m_hSocket!=INVALID_SOCKET)
        {
            ::closesocket(m_hSocket);
            m_hSocket=NULL;
        }
    }

}

OUTPUT int CheckTstatOnline2(TS_UC devLo,TS_UC devHi)
{
    if(g_Commu_type==0)
    {
        //the second time
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        // Clear serial port buffer
        //��մ��ڻ�����
        //the return value is the register address
        if(devLo<1 || devHi>254)
            return -5;
        //the input inspect
        for(int i=0; i<13; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_UC  pval[6];
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = 255;
        pval[1] = 25;  //put comments here,
        pval[2] = devHi;
        pval[3] = devLo;
        crc = CRC16(pval,4);
        pval[4] = (crc >>8) & 0xff;
        pval[5] = crc & 0xff;
        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear buffer
        int fState=WriteFile(m_hSerial,// ��� - Handle
                             pval,// ���ݻ�������ַ - Data buffer address
                             6,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                             &m_osWrite);
        if(!fState)// ��֧���ص�
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
                //			if(GetLastError()==ERROR_IO_INCOMPLETE)
                //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number=0;
        }

        //	TRACE("%d T:%x %x %x %x %x %x\n",ddd,pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
        //CloseHandle(m_osWrite.hEvent);
        ///////////////////////////up is write
        Sleep(LATENCY_TIME_COM);//because that scan have a delay lower 75ms
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0;
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - Handle
                        gval,// ���ݻ�������ַ - Data buffer address
                        13,// ���ݴ�С - Data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص�
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        //CloseHandle(m_osRead.hEvent);
        /*
        CStdioFile default_file;
        CString saved_path="C:\\modbus_scan_data.txt";
        CString a_line;
        if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
        {
        default_file.SeekToEnd();
        a_line.Format("T:%x %x %x %x %x %x",pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
        default_file.WriteString(a_line+"\n");
        a_line.Format("R:%x %x %x %x %x %x %x %x %x %x %x %x %x",gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
        default_file.WriteString(a_line+"\n");
        default_file.Flush();
        default_file.Close();
        }
        */
        //	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);



        if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
        {
            //old scan protocal
            old_or_new_scan_protocal_in_dll=2;
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -4;              //no response ,no connection
            }
            // added by zgq; find this situation: t3000 can find a comport,
            //which don't connect a tstat, but write file to the com, will receive the same data with send,
            // infact the com port don't work fine. But it never give you a wrong data.
            if(gval[0]==pval[0] && gval[1]==pval[1] && gval[2]==pval[2] && gval[3]==pval[3] && gval[4]==pval[4] && gval[5]==pval[5])
            {
                Sleep(SLEEP_TIME);
                return -4;
            }

            //////////////////////////////////////////////////////////////////////////
            if(gval[5]!=0 || gval[6]!=0)//to inspect
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -3;
            }
            if((gval[0]!=pval[0]) || (gval[1]!=25))
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
            crc=CRC16(gval,3);
            if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
        }
        else
        {
            // new scan protocal,if many old tstat ,get into here ,scan result is oK too.
            old_or_new_scan_protocal_in_dll=1;
            Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
            if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect
                return -3;
            if((gval[0]!=pval[0]) || (gval[1]!=25))
                return -2;
            crc=CRC16(gval,7);
            if( gval[7]!=((crc>>8) & 0xff) )
                return -2;
            if(gval[8]!=(crc & 0xff))
                return -2;
        }
        //here is different with CheckTstatOnline() function
        //	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
        //	TRACE("%d ^-^ ^-^ %d^\n",ddd,gval[2]);
        /*
        if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
        {
        default_file.SeekToEnd();
        a_line.Format("exist:%d",gval[2]);
        default_file.WriteString(a_line+"\n");
        default_file.Flush();
        default_file.Close();
        }*/
        //	return gval[2];
    }

    if(g_Commu_type==1)
    {
        //the second time
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ����� - Clear serial port buffer
        //the return value is the register address

        if(devLo<1 || devHi>254)
            return -5;
        //the input inspect
        for(int i=0; i<13; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer

        TS_UC  pval[10];
        //	TS_US crc;
        //	DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0]=1;
        pval[1]=2;
        pval[2]=3;
        pval[3]=4;
        pval[4]=5;
        pval[5]=6;

        pval[6] = 255;
        pval[7] = 25;
        pval[8] = devHi;
        pval[9] = devLo;

        /*
        crc = CRC16(pval,4);
        pval[4] = (crc >>8) & 0xff;
        pval[5] = crc & 0xff;
        */

        if (m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }
        ::send(m_hSocket,(char*)pval,sizeof(pval),0);

        //Sleep(SLEEP_TIME+8);
        Sleep(LATENCY_TIME_NET);
        TS_UC  rvData[19];
        for(int i=0; i<19; i++)
            rvData[i]=0;
        int nRecv = ::recv(m_hSocket, (char*)rvData, sizeof(rvData), 0);
        if (nRecv>0)
        {
            memcpy(gval,(void*)&rvData[6],sizeof(rvData));
        }
        if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
        {
            //old scan protocal
            old_or_new_scan_protocal_in_dll=2;
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
            {
                Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
                return -4;              //no response ,no connection
            }
            if(gval[5]!=0 || gval[6]!=0)//to inspect
            {
                Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
                return -3;
            }
            if((gval[0]!=255) || (gval[1]!=25))
            {
                //		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
            /*
            crc=CRC16(gval,3);
            if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
            {
            	//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
            	return -2;
            }
            */

        }
        else
        {
            // new scan protocal,if many old tstat ,get into here ,scan result is oK too.
            old_or_new_scan_protocal_in_dll=1;
            Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble


            //if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)//to inspect
            if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect  //zgq: same with com port scan
            {
                //Sleep(SLEEP_TIME+8);
                return -3;

            }

            if((gval[0]!=255) || (gval[1]!=25))
                return -2;

            /*
            if (gval[0]==0&&gval[0]==0&&gval[0]==0)
            {
            	return -3;
            }
            */

            /*
            crc=CRC16(gval,7);
            if( gval[7]!=((crc>>8) & 0xff) )
            	return -2;
            if(gval[8]!=(crc & 0xff))
            	return -2;
            	*/
        }
        //	return gval[2];
    }
    //Modify by Fance , both "if"  and "else" all return gval[2] ,so it can be moved to this ;
    //the warning not all control paths return a value will be disappeared.
    return gval[2];
}

OUTPUT int CheckTstatOnline(TS_UC devLo,TS_UC devHi)
{
    int the_return_value;
    if(g_Commu_type==0)
    {
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ�����
        //the return value is the register address
        //Sleep(50);       //must use this function to slow computer
        if(devLo<1 || devHi>254)
            return -5;
        if(m_hSerial==NULL)
        {
            return -1;
        }
        int the_return_value2=0;
        the_return_value=CheckTstatOnline2(devLo,devHi);
        //down is inspect result first scan
        if(the_return_value==-4)
        {
            the_return_value=CheckTstatOnline2(devLo,devHi);
            return the_return_value;
        }
        if(old_or_new_scan_protocal_in_dll==1)
        {
            //new protocal
            if(the_return_value>0)
            {
                the_return_value2=CheckTstatOnline2(devLo,devHi);
                if(the_return_value2!=-4)
                    the_return_value=the_return_value2;
            }
            return the_return_value;
        }
        else if(old_or_new_scan_protocal_in_dll==2)
        {
            //old protocal
            if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
                return the_return_value;
            int i=0;
            do
            {
                the_return_value=CheckTstatOnline2(devLo,devHi);
                if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
                    return the_return_value;
                else if(the_return_value>0)
                    i++;
            }
            while(i<3);
            return the_return_value;
        }
        //	if(the_return_value>0)
        //		TRACE("^-^ ^-^ %d\n",the_return_value);
        //return the_return_value;
    }

    if(g_Commu_type==1)//
    {
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ�����
        //the return value is the register address
        //Sleep(50);       //must use this function to slow computer
        if(devLo<1 || devHi>254)
            return -5;
        if (m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }
        int the_return_value2=0;
        the_return_value=CheckTstatOnline2(devLo,devHi);
        //down is inspect result first scan
        if(the_return_value==-4)
        {
            the_return_value=CheckTstatOnline2(devLo,devHi);
            return the_return_value;
        }
        if(old_or_new_scan_protocal_in_dll==1)
        {
            //new protocal
            if(the_return_value>0)
            {
                the_return_value2=CheckTstatOnline2(devLo,devHi);
                if(the_return_value2!=-4)
                    the_return_value=the_return_value2;
            }
            return the_return_value;
        }
        else if(old_or_new_scan_protocal_in_dll==2)
        {
            //old protocal
// 			if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
// 				return the_return_value;
// 			int i=0;
// 			do{
// 				the_return_value=CheckTstatOnline2(devLo,devHi);
// 				if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
// 					return the_return_value;
// 				else if(the_return_value>0)
// 					i++;
// 			}while(i<3);
            if(the_return_value>0)
            {
                the_return_value2=CheckTstatOnline2(devLo,devHi);
                if(the_return_value2!=-4)
                    the_return_value=the_return_value2;
            }
            return the_return_value;
        }
        //	if(the_return_value>0)
        //		TRACE("^-^ ^-^ %d\n",the_return_value);
        //return the_return_value;
    }
    //Modify by Fance , both "if"  and "else" all return the_return_value ,so it can be moved to this ;
    //the warning not all control paths return a value will be disappeared.
    return the_return_value;
}

OUTPUT bool Change_BaudRate_NoCretical(int new_baudrate,int ncomport)
{

    /// Configure serial port
    ///���ô��� - Configure serial port
    //if(new_baudrate!=9600 && new_baudrate!=19200)
    //	return false;
    BOOL iscorrect = FALSE;
    for (int i = 0;i< BAUDRATENUMBER;i++)
    {
        if (new_baudrate == ArrayBaudate[i])
        {
            iscorrect = TRUE;
            break;
        }
    }
    if (!iscorrect)
    {
        return FALSE;
    }
    //if (baudrate_in_dll == new_baudrate)
    //    return true;
    //else
    //    baudrate_in_dll = new_baudrate;
    DCB  PortDCB;
    PortDCB.DCBlength = sizeof(DCB);
    // Ĭ�ϴ��ڲ��� - Default serial port parameters
    int i = 0;
    bool successful = false;//true==do it success;false==do it failure
    for (i = 0;i < 10;i++)
    {
        if (GetCommState(m_com_h_serial[ncomport], &PortDCB))
        {
            successful = true;
            break;
        }
        else
        {
            int aaaab = 0;
            aaaab = ::GetLastError();
            Sleep(10);
        }
    }
    if (successful == false)
        return false;
    //not to change the baudate
    PortDCB.BaudRate = new_baudrate; // baud//attention ,if it is wrong,can't write the com
    PortDCB.ByteSize = 8;     // Number of bits/byte, 4-8 
    PortDCB.Parity = NOPARITY;
    PortDCB.StopBits = ONESTOPBIT;
    successful = false;
    for (i = 0;i<10;i++)
        if (SetCommState(m_com_h_serial[ncomport], &PortDCB))
        {
            ///L"���ô���ʧ��"; - Configuration serial port failed
            successful = true;
            break;
        }
    return(successful);
}


OUTPUT bool Change_BaudRate(int new_baudrate)
{

    /// Configure serial port
    ///���ô���
    //if(new_baudrate!=9600 && new_baudrate!=19200)
    //	return false;
    BOOL iscorrect= FALSE;
    for (int i = 0;i< BAUDRATENUMBER;i++)
    {
        if (new_baudrate == ArrayBaudate[i])
        {
            iscorrect = TRUE;
            break;
        }
    }
    if (!iscorrect)
    {
        return FALSE;
    }
    if(baudrate_in_dll==new_baudrate)
        return true;
    else
        baudrate_in_dll=new_baudrate;
    DCB  PortDCB;    
    PortDCB.DCBlength = sizeof(DCB); 
    // Ĭ�ϴ��ڲ���
    int i=0;
    bool successful=false;//true==do it success;false==do it failure
    for(i=0;i<10;i++)
        if(GetCommState(m_hSerial, &PortDCB))
        {
            successful=true;
            break;
        }
        if(successful==false)
            return false;
        //not to change the baudate

        PortDCB.BaudRate = new_baudrate; // baud//attention ,if it is wrong,can't write the com
        PortDCB.ByteSize = com_data_bit_in_dll;// 8;     // Number of bits/byte, 4-8 
        PortDCB.Parity = com_parity_bit_in_dll;// NOPARITY;
        PortDCB.StopBits = com_stop_bit_in_dll;// ONESTOPBIT;
        successful=false;
        for(i=0;i<10;i++)
            if(SetCommState(m_hSerial, &PortDCB))
            {
                ///L"���ô���ʧ��"; - Configuration serial port failed
                successful=true;
                break;
            }
            return(successful);
}
OUTPUT bool SetComm_Timeouts(LPCOMMTIMEOUTS lpCommTimeouts)
{
    return MKBOOL(SetCommTimeouts(m_hSerial,lpCommTimeouts));
}
//socket dll.
OUTPUT bool Open_Socket(CString strIPAdress)
{
    if(g_Commu_type==0)
        return false;

    int nNetTimeout=3000;//1 second.
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2, 2);
    if(::WSAStartup(sockVersion, &wsaData) != 0)
    {
        //	AfxMessageBox(_T("Init Socket failed!"));
        return FALSE;
    }
    if (m_hSocket!=INVALID_SOCKET)
    {

        ::closesocket(m_hSocket);
    }
    if (m_hSocket!=NULL)
    {
        ::closesocket(m_hSocket);
        m_hSocket=NULL;
    }

    m_hSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_hSocket == INVALID_SOCKET)
    {
        //	AfxMessageBox(_T("Create socket failed!"));
        return FALSE;
    }
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6001);
    // ע�⣬����Ҫ��д����������TCPServer�������ڻ�����IP��ַ
    //servAddr.sin_addr.S_un.S_addr =inet_addr("192.168.0.28");
    servAddr.sin_addr.S_un.S_addr =inet_addr((LPSTR)(LPCTSTR)strIPAdress);
    //	u_long ul=1;
    //	ioctlsocket(m_hSocket,FIONBIO,(u_long*)&ul);
    //����ʱ��
    setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
    //����ʱ��
    setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));
    if(::connect(m_hSocket,(sockaddr*)&servAddr, sizeof(servAddr)) == -1)
    {
        //	AfxMessageBox(_T(" Failed connect() \n"));
        ::closesocket(m_hSocket);
        m_hSocket=NULL;
        return FALSE;
    }
    return TRUE;
}

//socket dll.
OUTPUT bool Open_Socket2(CString strIPAdress,short nPort)
{
    //if(g_Commu_type==0)
    //	return false;

    int nNetTimeout=3000;//1 second.
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2, 2);

    if (m_hSocket!=INVALID_SOCKET)
    {
        ::closesocket(m_hSocket);
        m_hSocket=NULL;
    }

    if(::WSAStartup(sockVersion, &wsaData) != 0)
    {
        //AfxMessageBox(_T("Init Socket failed!"));
        m_hSocket=NULL;
        return FALSE;
    }
    int sockfd;
    sockfd = m_hSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(m_hSocket == INVALID_SOCKET)
    {
        //	AfxMessageBox(_T("Create socket failed!"));
        m_hSocket=NULL;
        return FALSE;
    }
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(nPort);
    USES_CONVERSION;
    //char pTemp[20];
    //pTemp=W2A(strIPAdress);




    //servAddr.sin_addr.S_un.S_addr =inet_addr("192.168.0.28");
//	servAddr.sin_addr.S_un.S_addr =inet_addr((LPSTR)(LPCTSTR)strIPAdress);
    servAddr.sin_addr.S_un.S_addr = (inet_addr(W2A(strIPAdress)));
    //	u_long ul=1;
    //	ioctlsocket(m_hSocket,FIONBIO,(u_long*)&ul);
    //����ʱ��
    setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&nNetTimeout,sizeof(int));
    //����ʱ��
    setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&nNetTimeout,sizeof(int));

    BOOL bDontLinger = FALSE;
    setsockopt(m_hSocket, SOL_SOCKET, SO_DONTLINGER, (const char*)&bDontLinger, sizeof(BOOL));  //20200214 ����ֱ�ӹر��׽���
    //****************************************************************************
    // Fance added, don't use blocking mode, if device is offline, it often takes 10+ seconds to wait
    // Fance added ,��Ҫ��������ģʽ������豸������ �����Ե� Ҫ��10���� 
    // Change to non-blocking mode, if connection is not established after 2.5 seconds, consider it as connection failure
    //��Ϊ�������� 2.5���û�����Ͼ� ������ʧ��;
    int error = -1;
    int len;
    len = sizeof(int);
    timeval tm;
    fd_set set;
    unsigned long ul = 1;
    ioctlsocket(m_hSocket, FIONBIO, &ul); //����Ϊ������ģʽ - Set to non-blocking mode
    bool ret = false;
    if( connect(m_hSocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        // 4s if unable to connect, consider it as failure, don't retry anymore
        tm.tv_sec = 4; //4s ������Ӳ��Ͼ� ��ʧ�� ����Ҫ����retry��; 
        tm.tv_usec = 0;
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        if( select(sockfd+1, NULL, &set, NULL, &tm) > 0)
        {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
            if(error == 0)
            {
                ret = true;
            }
            else
                ret = false;
        }
        else
        {
            ret = false;
        }
    }
    else ret = true;
    ul = 0;
    ioctlsocket(sockfd, FIONBIO, &ul); //����Ϊ����ģʽ - Set to blocking mode
    //****************************************************************************

    if(ret)
    {
        last_connected_ip = strIPAdress;
        last_connected_port = nPort;
        return true;
    }
    else
    {
        return false;
    }

    if(::connect(m_hSocket,(sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        DWORD dwErr = WSAGetLastError();
        //AfxMessageBox(_T(" Failed connect() \n"));
        ::closesocket(m_hSocket);
        m_hSocket=NULL;
        return FALSE;
    }
    last_connected_ip = strIPAdress;
    last_connected_port = nPort;
    return TRUE;
}



OUTPUT bool Open_Socket2_multy_thread(CString strIPAdress, short nPort,int nindex)
{
    
    SOCKET m_hSocket = NULL;
    int nNetTimeout = 3000;//1 second.
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2, 2);

    if (m_tcp_hSocket[nindex] != INVALID_SOCKET)
    {
        ::closesocket(m_tcp_hSocket[nindex]);
        m_tcp_hSocket[nindex] = NULL;
    }

    if (::WSAStartup(sockVersion, &wsaData) != 0)
    {
        //AfxMessageBox(_T("Init Socket failed!"));
        m_tcp_hSocket[nindex] = NULL;
        return FALSE;
    }
    int sockfd;
    sockfd = m_tcp_hSocket[nindex] = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    m_hSocket = m_tcp_hSocket[nindex];
    if (m_hSocket == INVALID_SOCKET)
    {
        //	AfxMessageBox(_T("Create socket failed!"));
        m_hSocket = NULL;
        return FALSE;
    }
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(nPort);
    USES_CONVERSION;
    //char pTemp[20];
    //pTemp=W2A(strIPAdress);




    //servAddr.sin_addr.S_un.S_addr =inet_addr("192.168.0.28");
    //	servAddr.sin_addr.S_un.S_addr =inet_addr((LPSTR)(LPCTSTR)strIPAdress);
    servAddr.sin_addr.S_un.S_addr = (inet_addr(W2A(strIPAdress)));
    //	u_long ul=1;
    //	ioctlsocket(m_hSocket,FIONBIO,(u_long*)&ul);
    //����ʱ�� - Set send timeout
    setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
    //����ʱ�� - Set receive timeout
    setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));

    //****************************************************************************
    // Fance added, don't use blocking mode, if device is offline, it often takes 10+ seconds to wait
    // Fance added ,��Ҫ��������ģʽ������豸������ �����Ե� Ҫ��10����
    // Change to non-blocking mode, if connection is not established after 2.5 seconds, consider it as connection failure
    //��Ϊ�������� 2.5���û�����Ͼ� ������ʧ��;
    int error = -1;
    int len;
    len = sizeof(int);
    timeval tm;
    fd_set set;
    unsigned long ul = 1;
    ioctlsocket(m_hSocket, FIONBIO, &ul); //����Ϊ������ģʽ - Set to non-blocking mode
    bool ret = false;
    if (connect(m_hSocket, (struct sockaddr *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        //4.5s ������Ӳ��Ͼ� ��ʧ�� ����Ҫ����retry��;
        tm.tv_sec = 4;// 4.5s if unable to connect, consider it as failure, don't retry anymore    
        tm.tv_usec = 0;
        FD_ZERO(&set);
        FD_SET(sockfd, &set);
        if (select(sockfd + 1, NULL, &set, NULL, &tm) > 0)
        {
            getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char *)&error, (socklen_t *)&len);
            if (error == 0)
            {
                ret = true;
            }
            else
                ret = false;
        }
        else
        {
            ret = false;
        }
    }
    else ret = true;
    ul = 0;
    ioctlsocket(sockfd, FIONBIO, &ul); //����Ϊ����ģʽ - Set to blocking mode
                                       //****************************************************************************

    if (ret)
    {
        last_connected_ip = strIPAdress;
        last_connected_port = nPort;
        return true;
    }
    else
    {
        return false;
    }

    if (::connect(m_hSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        DWORD dwErr = WSAGetLastError();
        //AfxMessageBox(_T(" Failed connect() \n"));
        ::closesocket(m_hSocket);
        m_hSocket = NULL;
        return FALSE;
    }
    last_connected_ip = strIPAdress;
    last_connected_port = nPort;
    return TRUE;
}


////////////////////////////////////////////////////////////////////////////
// Connect has two scenarios:
// Connect ��Ϊ���������
// 1. Serial port opens and works normally, but device communication fails (power off, disconnected, etc.).
// 1�����ڴ򿪹��������������豸ͨ��ʧ�ܣ��ϵ磬���ߵȵȣ���
// Manifests as normal serial port operations, but no communication data.
// ����Ϊ�Դ��ڲ�������������û��ͨ�����ݡ�
// 2. Serial port fails to open or works abnormally (USB serial port unplugged, occupied, etc.).
// 2�����ڴ�ʧ�ܻ�����������USB���ڰγ���ռ�õȵȣ���
// Manifests as inability to operate the serial port.
// ����Ϊ�޷��������ڡ�

OUTPUT bool is_connect()
{
    if (g_Commu_type==0)
    {
        if(m_hSerial==NULL)
            return FALSE;
        else
            return TRUE;
    }
    else if (g_Commu_type==1)
    {
        if(m_hSocket==NULL)
            return false;//disconnected
        else
            return true;//connected
    }
    return false; //add by Fance
}

//device_var:priduct ID,address:a regster;


OUTPUT int write_multi_Short(unsigned char device_var,unsigned short *to_write,unsigned short start_address,int length)
{
    if(g_Commu_type==0)//
    {
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        HCURSOR hc;//load mouse cursor
        TS_UC data_to_write[600]= {0};
        data_to_write[0]=device_var;
        data_to_write[1]=0x10;
        data_to_write[2]=start_address >> 8 & 0xff;
        data_to_write[3]=start_address & 0xff;
        data_to_write[4]=0;
        data_to_write[5]=length&0xff;
        data_to_write[6]=length*2;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
        for(int i=0; i<length; i++)
        {
            data_to_write[7+i*2] =to_write[i]  >> 8 & 0xff;
            data_to_write[7+i*2+1]=to_write[i] & 0xff;

        }


        TS_US crc=CRC16(data_to_write,2*length+7);
        data_to_write[length*2+7]=crc>>8 & 0xff;
        data_to_write[length*2+8]=crc & 0xff;

        CString traceStr=_T("Send:");
        CString temp;
        for (int i=0; i<2*length+9; i++)
        {
            temp.Format(_T("%02X "),data_to_write[i]);
            traceStr+=temp;
        }
        //TRACE(_T("%s"),traceStr.GetBuffer());

        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //length is the data length,if you want to write 128 bite,the length == 128
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));

        
        if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_mulwrite))==NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0 ;
        ///////////////////////////////////////////////////////send the to read message
        int fState=WriteFile(m_hSerial,// ��� - Handle
                             data_to_write,// ���ݻ�������ַ - Data buffer address
                             length*2+9,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Number of bytes sent
                             &m_osMulWrite);
        if(!fState)// ��֧���ص� - Overlapped not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        Sleep(LATENCY_TIME_COM);
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - Handle
                        gval,// ���ݻ�������ַ - Data buffer address
                        8,// ���ݴ�С - Data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - Overlapped not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        traceStr=_T("Rev:");
        for(int i=0; i<8; i++)
        {
            temp.Format(_T("%02X "),gval[i]);
            traceStr+=temp;
        }
        //TRACE(_T("%s"),traceStr);
        ///////////////////////////////////////////////////////////
        for(int i=0; i<6; i++)
            if(gval[i]!=*(data_to_write+i))
                return -2;
        crc=CRC16(gval,6);
        if(gval[6]!=((crc>>8)&0xff))
            return -2;
        if(gval[7]!=(crc & 0xff))
            return -2;
        return 1;
    }
    if(g_Commu_type==1)//tcp.
    {
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        HCURSOR hc;//load mouse cursor
        TS_UC data_to_write[600]= {'\0'};
        TS_UC data_back_write[600]= {'\0'};
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        data_to_write[0]= trans_high_id;
        data_to_write[1]= trans_low_id;
        data_to_write[2]=0;
        data_to_write[3]=0;
        data_to_write[4]=0;
        data_to_write[5]=6;

        data_to_write[6]=device_var;
        data_to_write[7]=0x10;
        data_to_write[8]=start_address >> 8 & 0xff;
        data_to_write[9]=start_address & 0xff;
        data_to_write[10]=0;
        data_to_write[11]=length&0xff;
        data_to_write[12]=length*2;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
        for(int i=0; i<length; i++)
        {
            data_to_write[13+i*2] =to_write[i]  >> 8 & 0xff;
            data_to_write[13+i*2+1]=to_write[i] & 0xff;
        }


        //	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
        //	TS_US crc=CRC16(data_to_write,i+7);
        //	data_to_write[i+7]=crc>>8 & 0xff;
        //	data_to_write[i+8]=crc & 0xff;
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
//length is the data length,if you want to write 128 bite,the length == 128
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        int n=::send(m_hSocket,(char *)data_to_write,13+2*length,0);
        int nRecv = ::recv(m_hSocket, (char *)data_back_write,13, 0);
        if (nRecv < 0)
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ� - The remote host forcibly closed an existing connection.
            {
                if (last_connected_port != 0)
                    Open_Socket2(last_connected_ip, last_connected_port);
            }
            return -2;
        }
        //	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
        for(int i=0; i<6; i++)
            if(data_back_write[i+6]!=*(data_to_write+i+6))
                return -2;
        return 1;

    }
    return -1;
}
OUTPUT int write_multi_Short_log(TS_UC device_var,TS_US *to_write,TS_US start_address,TS_US length,//��������� - Parameter command part
                                 unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, //���ͺͽ��ܵ�ԭʼ���� - Send and receive raw data
                                 int* sendDataLength, int* recvDataLength) //���ͺͽ������ݳ��� - Send and receive data length
{
    if(g_Commu_type==0)//
    {
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        HCURSOR hc;//load mouse cursor
        TS_UC data_to_write[600]= {0};
        data_to_write[0]=device_var;
        data_to_write[1]=0x10;
        data_to_write[2]=start_address >> 8 & 0xff;
        data_to_write[3]=start_address & 0xff;
        data_to_write[4]= length >> 8 & 0xff;
        data_to_write[5]=length&0xff;
        data_to_write[6]=length*2;
		//128 is better ,if you send more than 128, 
		//the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
        for(int i=0; i<length; i++)
        {
//                 unsigned char UC_Value=to_write[i];
//                 data_to_write[7+i*2] =UC_Value>>8&0xff;
// 				data_to_write[7+i*2+1]=UC_Value& 0xff;


            data_to_write[7+i*2] =to_write[i]  >> 8 & 0xff;
            data_to_write[7+i*2+1]=to_write[i] & 0xff;


        }


        TS_US crc=CRC16(data_to_write,2*length+7);
        data_to_write[length*2+7]=crc>>8 & 0xff;
        data_to_write[length*2+8]=crc & 0xff;

        //CString traceStr=_T("Send:");
        //CString temp;
        //for (int i=0;i<2*length+9;i++)
        //{
        //	temp.Format(_T("%02X "),data_to_write[i]);
        //	traceStr+=temp;
        //}
        *sendDataLength = 2*length+9;
        for (int i = 0; i < *sendDataLength; i++)
        {
            *((char*)put_senddate_into_here + i) = data_to_write[i];
        }

        //	TRACE(_T("%s"),traceStr.GetBuffer());

        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //length is the data length,if you want to write 128 bite,the length == 128
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
        if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_mulwrite))==NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0 ;
        ///////////////////////////////////////////////////////send the to read message
        int fState=WriteFile(m_hSerial,// ��� - Handle
                             data_to_write,// ���ݻ�������ַ - Data buffer address
                             length*2+9,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Number of bytes sent
                             &m_osMulWrite);
        if(!fState)// ��֧���ص� - Overlapped not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        Sleep(LATENCY_TIME_COM);
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - Handle
                        gval,// ���ݻ�������ַ - Data buffer address
                        8,// ���ݴ�С - Data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - Overlapped not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        //traceStr=_T("Rev:");
        //for(int i=0;i<8;i++){
        //	temp.Format(_T("%02X "),gval[i]);
        //	traceStr+=temp;
        //}
        //TRACE(_T("%s"),traceStr);
        *recvDataLength=8;
        for(int i=0; i<8; i++)
        {
            *((char*)put_revdata_into_here + i) = gval[i];
        }
        //  *((char*)put_senddate_into_here + i) = data_to_write[i];
        ///////////////////////////////////////////////////////////
        for(int i=0; i<6; i++)
        {
            if(gval[i]!=*(data_to_write+i))
                return -2;
        }
        crc=CRC16(gval,6);
        if(gval[6]!=((crc>>8)&0xff))
            return -2;
        if(gval[7]!=(crc & 0xff))
            return -2;
        return 1;
    }
    if(g_Commu_type==1)//tcp.
    {
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        HCURSOR hc;//load mouse cursor
        TS_UC data_to_write[600]= {'\0'};
        TS_UC data_back_write[600]= {'\0'};
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        data_to_write[0]= trans_high_id;
        data_to_write[1]= trans_low_id;
        data_to_write[2]=0;
        data_to_write[3]=0;
        data_to_write[4]=0;
        data_to_write[5]=6;

        data_to_write[6]=device_var;
        data_to_write[7]=0x10;
        data_to_write[8]=start_address >> 8 & 0xff;
        data_to_write[9]=start_address & 0xff;
        data_to_write[10]=0;
        data_to_write[11]=length&0xff;
        data_to_write[12]=length*2;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
        for(int i=0; i<length; i++)
        {
            data_to_write[13+i*2] =to_write[i]  >> 8 & 0xff;
            data_to_write[13+i*2+1]=to_write[i] & 0xff;
        }

        *sendDataLength = 2*length+13;
        for (int i = 0; i < *sendDataLength; i++)
        {
            *((char*)put_senddate_into_here + i) = data_to_write[i];
        }
        //	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
        //	TS_US crc=CRC16(data_to_write,i+7);
        //	data_to_write[i+7]=crc>>8 & 0xff;
        //	data_to_write[i+8]=crc & 0xff;
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
//length is the data length,if you want to write 128 bite,the length == 128
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� // Number of bytes sent
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        int n=::send(m_hSocket,(char *)data_to_write,13+2*length,0);
        int nRecv = ::recv(m_hSocket, (char *)data_back_write,13, 0);
        if (nRecv < 0)
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ�- The remote host forcibly closed an existing connection.
            {
                if (last_connected_port != 0)
                {
                    Open_Socket2(last_connected_ip, last_connected_port);
                    return -1;
                }
            }
        }
        *recvDataLength=13;
        for(int i=0; i<13; i++)
        {
            *((char*)recvDataLength + i) = data_back_write[i];
        }
        if(nRecv<0)
        {
            return -2;
        }

        //	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
        for(int i=0; i<6; i++)
            if(data_back_write[i+6]!=*(data_to_write+i+6))
                return -2;
        return 1;

    }
    return -1;
}
OUTPUT int write_multi_Coil_log(TS_UC device_var, TS_BOOL *to_write, TS_US start_address, TS_US length,//��������� - Parameter command part
	unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, //���ͺͽ��ܵ�ԭʼ���� - Send and receive raw data
	int* sendDataLength, int* recvDataLength) //���ͺͽ������ݳ��� - Send and receive data length
{
	unsigned short ToSendData[16];
	int DataLength = 0;
	if (length%8==0)
	{
		DataLength = length / 8;

	} 
	else
	{
		DataLength = length / 8 + 1;
	}
	bitset<8> BitReg;
	for (int index = 0;index< DataLength;index++)
	{
		//��ʼ��BitReg - Initialize BitReg
		for (int i=0;i<8;i++)
		{
			BitReg[i] = false;
		}
		 
		for (int i = 0; i < 8; i++)
		{
			if (index * 8 + i < length)
			{
				BitReg[i] = to_write[index * 8 + i];
			}
			
		}
		ToSendData[index] = BitReg.to_ulong();
	 
	}

	if (g_Commu_type == 0)//
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { 0 };
		data_to_write[0] = device_var;
		data_to_write[1] = 0x0F;
		data_to_write[2] = start_address >> 8 & 0xff;
		data_to_write[3] = start_address & 0xff;
		data_to_write[4] = length >> 8 & 0xff;
		data_to_write[5] = length & 0xff;

		data_to_write[6] = DataLength * 2;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		
		for (int i = 0; i<DataLength; i++)
		{
		


			data_to_write[7 + i * 2] = ToSendData[i] >> 8 & 0xff;
			data_to_write[7 + i * 2 + 1] = ToSendData[i] & 0xff;


		}

		TS_US crc = CRC16(data_to_write, 2 * DataLength + 7);
		data_to_write[DataLength * 2 + 7] = crc >> 8 & 0xff;
		data_to_write[DataLength * 2 + 8] = crc & 0xff;

		//CString traceStr=_T("Send:");
		//CString temp;
		//for (int i=0;i<2*length+9;i++)
		//{
		//	temp.Format(_T("%02X "),data_to_write[i]);
		//	traceStr+=temp;
		//}
		*sendDataLength = 2 * DataLength + 9;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_write[i];
		}

		//	TRACE(_T("%s"),traceStr.GetBuffer());

		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� // Number of bytes already sent
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, g_cs_com_section_mulwrite)) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;
		///////////////////////////////////////////////////////send the to read message
		int fState = WriteFile(m_hSerial,// ��� // Handle
			data_to_write,// ���ݻ�������ַ // Data buffer address
			length * 2 + 9,// ���ݴ�С // Data size
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� // Returns number of bytes sent
			&m_osMulWrite);
		if (!fState)// ��֧���ص� // Overlapped not supported
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ� // Wait
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, g_cs_com_section_read)) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		Sleep(LATENCY_TIME_COM);
		////////////////////////////////////////////////clear com error
		fState = ReadFile(m_hSerial,// ��� // Handle
			gval,// ���ݻ�������ַ // Data buffer address
			8,// ���ݴ�С // Data size
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� // Returns number of bytes sent
			&m_osRead);
		if (!fState)// ��֧���ص� // Overlapped not supported
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ� // Wait
			}
			else
				m_had_send_data_number = 0;
		}
		//traceStr=_T("Rev:");
		//for(int i=0;i<8;i++){
		//	temp.Format(_T("%02X "),gval[i]);
		//	traceStr+=temp;
		//}
		//TRACE(_T("%s"),traceStr);
		*recvDataLength = 8;
		for (int i = 0; i<8; i++)
		{
			*((char*)put_revdata_into_here + i) = gval[i];
		}
		//  *((char*)put_senddate_into_here + i) = data_to_write[i];
		///////////////////////////////////////////////////////////
		for (int i = 0; i<6; i++)
		{
			if (gval[i] != *(data_to_write + i))
				return -2;
		}
		crc = CRC16(gval, 6);
		if (gval[6] != ((crc >> 8) & 0xff))
			return -2;
		if (gval[7] != (crc & 0xff))
			return -2;
		return 1;
	}
	if (g_Commu_type == 1)//tcp.
	{
		//the return value ,-2 is wrong
		//the return value == -1 ,no connecting
		HCURSOR hc;//load mouse cursor
		TS_UC data_to_write[600] = { '\0' };
		TS_UC data_back_write[600] = { '\0' };
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
		data_to_write[0] = trans_high_id;
		data_to_write[1] = trans_low_id;
		data_to_write[2] = 0;
		data_to_write[3] = 0;
		data_to_write[4] = 0;
		data_to_write[5] = 6;

		data_to_write[6] = device_var;
		data_to_write[7] = 0x10;
		data_to_write[8] = start_address >> 8 & 0xff;
		data_to_write[9] = start_address & 0xff;
		data_to_write[10] = length >> 8 & 0xff;;
		data_to_write[11] = length & 0xff;
		data_to_write[12] = DataLength * 2;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
		 
		for (int i = 0; i < DataLength; i++)
		{
			data_to_write[13 + i * 2] = ToSendData[i] >> 8 & 0xff;
			data_to_write[13 + i * 2 + 1] = ToSendData[i] & 0xff;
		}
		*sendDataLength = 2 * DataLength + 13;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_write[i];
		}
		//	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
		//	TS_US crc=CRC16(data_to_write,i+7);
		//	data_to_write[i+7]=crc>>8 & 0xff;
		//	data_to_write[i+8]=crc & 0xff;
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//length is the data length,if you want to write 128 bite,the length == 128
		//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		int n = ::send(m_hSocket, (char *)data_to_write, 13 + 2 * length, 0);
		//Sleep(LATENCY_TIME_NET);
		int nRecv = ::recv(m_hSocket, (char *)data_back_write, 13, 0);

		*recvDataLength = 13;
		for (int i = 0; i<13; i++)
		{
			*((char*)recvDataLength + i) = data_back_write[i];
		}
		if (nRecv<0)
		{
			return -2;
		}

		//	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
		for (int i = 0; i<6; i++)
			if (data_back_write[i + 6] != *(data_to_write + i + 6))
				return -2;
		return 1;

	}
	return -1;
}

OUTPUT int Read_One(TS_UC device_var,TS_US address)
{

//	CSingleLock singlock(&scan_mutex);
//	singlock.Lock();
    int nTemp=-1;
    if(g_Commu_type==0)//serial
    {
        //address        the register
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        //return value == -3 ,no response
        //��մ��ڻ�����
        //TS_UC  gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];
        for(int i=0; i<11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� // Number of bytes already sent
        pval[0] = device_var;
        pval[1] = 3;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        pval[4] = 0;//(val>>8) & 0xff;//number hi
        pval[5] = 1;//val & 0xff;//number lo
        crc = CRC16(pval,6);
        pval[6] = (crc >>8) & 0xff;
        pval[7] = crc & 0xff;
        if(m_hSerial==NULL)
        {
            return -1;
        }

        ////////////////////////////////////////////////////////////overlapped declare
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        BOOL bRet = ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ///////////////////////////////////////////////////////send the to read message
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0;

        int fState=WriteFile(m_hSerial,// ��� // Handle
                             pval,	// ���ݻ�������ַ // Data buffer address
                             8,		// ���ݴ�С // Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� // Returns number of bytes sent
                             &m_osWrite);
        if(!fState)// ��֧���ص� // Overlapped not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ� // Wait
                //			if(GetLastError()==ERROR_IO_PENDING)
                //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number=0;
        }
        //////////////////////////////////////////the message had send ,now to read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;

        ////////////////////////////////////////////////clear com error
        Sleep(LATENCY_TIME_COM);
        if(address==10)
        {
            serinumber_in_dll[0]=serinumber_in_dll[1]=serinumber_in_dll[2]=serinumber_in_dll[3]=0;//this line is for new protocal
            fState=ReadFile(m_hSerial,// ��� - Handle
                            gval,// ���ݻ�������ַ - Data buffer address
                            11,// ���ݴ�С - Data size
                            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                            &m_osRead);
        }
        else
        {
            fState=ReadFile(m_hSerial,// ��� - Handle
                            gval,// ���ݻ�������ַ - Data buffer address
                            7,// ���ݴ�С - Data size
                            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                            &m_osRead);
        }
        if(!fState)// ��֧���ص� - Overlapped not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        if(address!=10)
        {
            //old protocal
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 )
                return -3;
            if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )
                return -2;
            crc=CRC16(gval,5);
            if(gval[5]!=((crc>>8)&0xff))
                return -2;
            if(gval[6]!=(crc & 0xff))
                return -2;
        }
        else
        {
            if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)
            {
                //new protocal
                if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=6 )//6
                    return -2;
                crc=CRC16(gval,9);
                if(gval[9]!=((crc>>8)&0xff))
                    return -2;
                if(gval[10]!=(crc & 0xff))
                    return -2;
                serinumber_in_dll[0]=gval[5];
                serinumber_in_dll[1]=gval[6];
                serinumber_in_dll[2]=gval[7];
                serinumber_in_dll[3]=gval[8];
                //stay serialnumber
                //			TRACE("R:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
            }
            else
            {
                //old protocal
                if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )//2
                    return -2;
                crc=CRC16(gval,5);
                if(gval[5]!=((crc>>8)&0xff))
                    return -2;
                if(gval[6]!=(crc & 0xff))
                    return -2;
            }
        }
        nTemp=gval[3];
        if(nTemp==255)
            nTemp=-1;
        return (nTemp*256+gval[4]);
    }
    if(g_Commu_type==1)//tcp.
    {
        //address        the register
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        //return value == -3 ,no response
        //��մ��ڻ�����
        //TS_UC  gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];

        /*
        struct DataPack
        {
        	TS_UC  empty[6];
        	TS_UC  pval[13];

        };
        */
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        TS_UC data[12];

        data[0]= trans_high_id;
        data[1]= trans_low_id;
        data[2]=0;
        data[3]=0;
        data[4]=0;
        data[5]=6;
        data[6]=device_var;
        data[7]=3;
        data[8]=address>>8 & 0xFF ;
        data[9] = address & 0xFF;
        data[10]=0;
        data[11]=1;

//		DataPack dataInfo;
        for(int i=0; i<11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        //TS_US crc;
        //DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent

        pval[0] = device_var;
        pval[1] = 3;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        pval[4] = 0;//(val>>8) & 0xff;//number hi
        pval[5] = 1;//val & 0xff;//number lo
        //	crc = CRC16(pval,6);
        //	pval[6] = (crc >>8) & 0xff;
        //	pval[7] = crc & 0xff;

        //memcpy((char*)&data[6],(char*)&pval[0],sizeof(pval));

        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        int nLen = sizeof(data);
        //int xx=::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        int xx=::send(m_hSocket,(char*)&data,sizeof(data),0);

        if(xx < 0)
        {
            if(last_connected_port!=0)
                Open_Socket2(last_connected_ip,last_connected_port);
        }
        ////////////////////////////////////////////////clear com error
        if(address==10)
        {
            serinumber_in_dll[0]=serinumber_in_dll[1]=serinumber_in_dll[2]=serinumber_in_dll[3]=0;//this line is for new protocal
        }

        Sleep(LATENCY_TIME_NET);//Sleep(SLEEP_TIME);
        //DataPack rvData;
        //Sleep(SLEEP_TIME+10);

        TS_UC rvData[17];
        //int nRecv = ::recv(m_hSocket, (char*)gval, sizeof(gval), 0);
        int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);

        if (nRecv < 0)
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ� - Error code: The remote host forcibly closed an existing connection.
            {
                if (last_connected_port != 0)
                {
                    Open_Socket2(last_connected_ip, last_connected_port);
                    return -1;
                }
            }
        }
        //gval=rvData.pval;
        memcpy((void*)gval,(void*)&rvData[6],13/*sizeof(gval)*/);

        if(address!=10)
        {
            //old protocal
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0/* && gval[5]==0 && gval[6]==0*/ )
                return -3;
            if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )
                return -2;

            /*;
            crc=CRC16(gval,5);
            if(gval[5]!=((crc>>8)&0xff))
            	return -2;
            if(gval[6]!=(crc & 0xff))
            	return -2;
            */

        }
        else
        {
            if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)
            {
                //new protocal
                if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=6 )//6
                    return -2;
                /*
                crc=CRC16(gval,9);
                if(gval[9]!=((crc>>8)&0xff))
                	return -2;
                if(gval[10]!=(crc & 0xff))
                	return -2;
                	*/
                serinumber_in_dll[0]=gval[5];
                serinumber_in_dll[1]=gval[6];
                serinumber_in_dll[2]=gval[7];
                serinumber_in_dll[3]=gval[8];//stay serialnumber
                //			TRACE("R:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
            }
            else
            {
                //old protocal
                if(gval[0]!=pval[0] || gval[1]!=gval[1] || gval[2]!=2 )//2
                    return -2;
                /*
                crc=CRC16(gval,5);
                if(gval[5]!=((crc>>8)&0xff))
                	return -2;
                if(gval[6]!=(crc & 0xff))
                	return -2;
                	*/
            }
        }
        nTemp = gval[3];
        //2018 09 26 Fandu  ���� // Blocked/Masked
        //if(nTemp==255)
        //    nTemp=-1;
        return (gval[3] * 256 + gval[4]);
    }
    return -1; //add by Fance
//	singlock.Unlock();
}

OUTPUT int Read_One_log(TS_UC device_var,TS_US address,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength)
{

//	CSingleLock singlock(&scan_mutex);
//	singlock.Lock();
    int nTemp=-1;
    if(g_Commu_type==0)//serial
    {
        //address        the register
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        //return value == -3 ,no response
        //��մ��ڻ�����
        //TS_UC  gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];
        for(int i=0; i<11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 3;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        pval[4] = 0;//(val>>8) & 0xff;//number hi
        pval[5] = 1;//val & 0xff;//number lo
        crc = CRC16(pval,6);
        pval[6] = (crc >>8) & 0xff;
        pval[7] = crc & 0xff;
        if(m_hSerial==NULL)
        {
            return -1;
        }

        ////////////////////////////////////////////////////////////overlapped declare
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        BOOL bRet = ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ///////////////////////////////////////////////////////send the to read message
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0;

        *sendDataLength = 8;
        for (int i = 0; i < *sendDataLength; i++)
        {
            *((char*)put_senddate_into_here + i) = pval[i];
        }

        int fState=WriteFile(m_hSerial,// ��� - Handle
                             pval,	// ���ݻ�������ַ - Data buffer address
                             8,		// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                             &m_osWrite);
        if(!fState)// ��֧���ص� - Overlapped not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
                //			if(GetLastError()==ERROR_IO_PENDING)
                //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number=0;
        }
        //////////////////////////////////////////the message had send ,now to read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;

        ////////////////////////////////////////////////clear com error
        Sleep(LATENCY_TIME_COM);
        if(address==10)
        {
            serinumber_in_dll[0]=serinumber_in_dll[1]=serinumber_in_dll[2]=serinumber_in_dll[3]=0;//this line is for new protocal
            fState=ReadFile(m_hSerial,// ��� - Handle
                            gval,// ���ݻ�������ַ - Data buffer address
                            11,// ���ݴ�С - Data size
                            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                            &m_osRead);
        }
        else
        {
            fState=ReadFile(m_hSerial,// ��� - Handle
                            gval,// ���ݻ�������ַ - Data buffer address
                            7,// ���ݴ�С - Data size
                            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                            &m_osRead);
        }
        if(!fState)// ��֧���ص� - Overlapped not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }

        *recvDataLength = m_had_send_data_number;
        for (int i = 0; i < *recvDataLength; i++)
        {
            *((char*)put_revdata_into_here + i) = gval[i];
        }

        if(address!=10)
        {
            //old protocal
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 )
                return -3;
            if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )
                return -2;
            crc=CRC16(gval,5);
            if(gval[5]!=((crc>>8)&0xff))
                return -2;
            if(gval[6]!=(crc & 0xff))
                return -2;
        }
        else
        {
            if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)
            {
                //new protocal
                if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=6 )//6
                    return -2;
                crc=CRC16(gval,9);
                if(gval[9]!=((crc>>8)&0xff))
                    return -2;
                if(gval[10]!=(crc & 0xff))
                    return -2;
                serinumber_in_dll[0]=gval[5];
                serinumber_in_dll[1]=gval[6];
                serinumber_in_dll[2]=gval[7];
                serinumber_in_dll[3]=gval[8];//stay serialnumber
                //			TRACE("R:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
            }
            else
            {
                //old protocal
                if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )//2
                    return -2;
                crc=CRC16(gval,5);
                if(gval[5]!=((crc>>8)&0xff))
                    return -2;
                if(gval[6]!=(crc & 0xff))
                    return -2;
            }
        }

        nTemp=gval[3];
        if(nTemp==255)
            nTemp=-1;
        return (nTemp*256+gval[4]);
    }
    if(g_Commu_type==1)//tcp.
    {
        //address        the register
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        //return value == -3 ,no response
        //��մ��ڻ�����
        //TS_UC  gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];

        /*
        struct DataPack
        {
        	TS_UC  empty[6];
        	TS_UC  pval[13];

        };
        */
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        TS_UC data[12];

        data[0]= trans_high_id;
        data[1]= trans_low_id;
        data[2]=0;
        data[3]=0;
        data[4]=0;
        data[5]=6;



        ++g_data_to_send[1];
        if (g_data_to_send[1]%256==0)
        {
            ++g_data_to_send[0];
            g_data_to_send[1]=0;
        }



        data[0]=g_data_to_send[0];
        data[1]=g_data_to_send[1];
        data[2]=g_data_to_send[2];
        data[3]=g_data_to_send[3];
        data[4]=g_data_to_send[4];
        data[5]=g_data_to_send[5];


        data[6]=device_var;
        data[7]=3;
        data[8]=address>>8 & 0xFF ;
        data[9] = address & 0xFF;
        data[10]=0;
        data[11]=1;

//		DataPack dataInfo;
        for(int i=0; i<11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        //TS_US crc;
        //DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� // - Number of bytes sent

        pval[0] = device_var;
        pval[1] = 3;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        pval[4] = 0;//(val>>8) & 0xff;//number hi
        pval[5] = 1;//val & 0xff;//number lo
        //	crc = CRC16(pval,6);
        //	pval[6] = (crc >>8) & 0xff;
        //	pval[7] = crc & 0xff;

        //memcpy((char*)&data[6],(char*)&pval[0],sizeof(pval));

        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        int nLen = sizeof(data);
        //int xx=::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        *sendDataLength = nLen;
        for (int i = 0; i < *sendDataLength; i++)
        {
            *((char*)put_senddate_into_here + i) = data[i];
        }

        int xx=::send(m_hSocket,(char*)&data,sizeof(data),0);


        ////////////////////////////////////////////////clear com error
        if(address==10)
        {
            serinumber_in_dll[0]=serinumber_in_dll[1]=serinumber_in_dll[2]=serinumber_in_dll[3]=0;//this line is for new protocal
        }

        Sleep(LATENCY_TIME_NET);//Sleep(SLEEP_TIME);
        //DataPack rvData;
        //Sleep(SLEEP_TIME+10);

        TS_UC rvData[17];
        //int nRecv = ::recv(m_hSocket, (char*)gval, sizeof(gval), 0);
        int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);
        //gval=rvData.pval;
        *recvDataLength = nRecv;
        for (int i = 0; i < *recvDataLength; i++)
        {
            *((char*)put_revdata_into_here + i) = rvData[i];
        }

        memcpy((void*)gval,(void*)&rvData[6],13/*sizeof(gval)*/);

        /*
        xx=::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        nRecv = ::recv(m_hSocket, (char*)gval, sizeof(gval), 0);*/

        int nerro= WSAGetLastError();

        if(address!=10)
        {
            //old protocal
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0/* && gval[5]==0 && gval[6]==0*/ )
                return -3;
            if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )
                return -2;

            /*;
            crc=CRC16(gval,5);
            if(gval[5]!=((crc>>8)&0xff))
            	return -2;
            if(gval[6]!=(crc & 0xff))
            	return -2;
            */

        }
        else
        {
            if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)
            {
                //new protocal
                if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=6 )//6
                    return -2;
                /*
                crc=CRC16(gval,9);
                if(gval[9]!=((crc>>8)&0xff))
                	return -2;
                if(gval[10]!=(crc & 0xff))
                	return -2;
                	*/
                serinumber_in_dll[0]=gval[5];
                serinumber_in_dll[1]=gval[6];
                serinumber_in_dll[2]=gval[7];
                serinumber_in_dll[3]=gval[8];//stay serialnumber
                //			TRACE("R:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
            }
            else
            {
                //old protocal
                if(gval[0]!=pval[0] || gval[1]!=gval[1] || gval[2]!=2 )//2
                    return -2;
                /*
                crc=CRC16(gval,5);
                if(gval[5]!=((crc>>8)&0xff))
                	return -2;
                if(gval[6]!=(crc & 0xff))
                	return -2;
                	*/
            }
        }
        nTemp=gval[3];
        if(nTemp==255)
            nTemp=-1;
        return (gval[3]*256+gval[4]);
    }
    return -1; //add by Fance
//	singlock.Unlock();
}

OUTPUT int Write_One_Multy_Thread(TS_UC device_var, TS_US address, TS_US val,int nindex)
{
    if (g_Commu_type == 0)
    {
        //address        the register
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -3 , no response
        //��մ��ڻ�����

        //		gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];
        for (int i = 0; i <= 11; i++)
            gval[i] = 0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address >> 8 & 0xFF;
        pval[3] = address & 0xFF;
        if (address == 10 && (serinumber_in_dll[0] != 0 || serinumber_in_dll[1] != 0 || serinumber_in_dll[2] != 0 || serinumber_in_dll[3] != 0))
            pval[4] = 0x55;/////////////////////////////new protocal or write_one 10
        else
            pval[4] = (val >> 8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo
        if (address != 10)
        {
            crc = CRC16(pval, 6);
            pval[6] = (crc >> 8) & 0xff;
            pval[7] = crc & 0xff;
        }
        else
        {
            if (serinumber_in_dll[0] == 0 && serinumber_in_dll[1] == 0 && serinumber_in_dll[2] == 0 && serinumber_in_dll[3] == 0)
            {
                crc = CRC16(pval, 6);
                pval[6] = (crc >> 8) & 0xff;
                pval[7] = crc & 0xff;
            }
            else
            {
                pval[6] = serinumber_in_dll[0];
                pval[7] = serinumber_in_dll[1];
                pval[8] = serinumber_in_dll[2];
                pval[9] = serinumber_in_dll[3];
                crc = CRC16(pval, 10);
                pval[10] = (crc >> 8) & 0xff;
                pval[11] = crc & 0xff;
            }
        }

        if (m_hSerial == NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0;
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
        int fState;
        Sleep(50);
        if (address != 10)
        {
            fState = WriteFile(m_hSerial,// ��� - Handle
                pval,// ���ݻ�������ַ - Data buffer address
                8,// ���ݴ�С - Data size
                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                &m_osWrite);
        }
        else
        {
            //==10 register
            if (serinumber_in_dll[0] == 0 && serinumber_in_dll[1] == 0 && serinumber_in_dll[2] == 0 && serinumber_in_dll[3] == 0)
            {
                //old protocal
                fState = WriteFile(m_hSerial,// ��� - Handle
                    pval,// ���ݻ�������ַ - Data buffer address
                    8,// ���ݴ�С - Data size
                    &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                    &m_osWrite);
            }
            else
            {
                //new protocal
                fState = WriteFile(m_hSerial,// ��� - Handle
                    pval,// ���ݻ�������ַ - Data buffer address
                    12,// ���ݴ�С - Data size
                    &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                    &m_osWrite);
            }
        }
        if (!fState)// ��֧���ص� - Overlapped not supported
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
                                                                                                   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
                                                                                                   //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number = 0;
        }
        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if ((m_osRead.hEvent = CreateEvent(NULL, true, false, g_cs_com_section_read)) == NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0;
        ////////////////////////////////////////////////clear com error
        Sleep(LATENCY_TIME_COM);
        if (address != 10)
        {
            fState = ReadFile(m_hSerial,// ��� - Handle
                gval,// ���ݻ�������ַ - Data buffer address
                8,// ���ݴ�С - Data size
                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                &m_osRead);
        }
        else
        {
            if (serinumber_in_dll[0] == 0 && serinumber_in_dll[1] == 0 && serinumber_in_dll[2] == 0 && serinumber_in_dll[3] == 0)
            {
                //old protocal
                fState = ReadFile(m_hSerial,// ��� - Handle
                    gval,// ���ݻ�������ַ - Data buffer address
                    8,// ���ݴ�С - Data size
                    &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                    &m_osRead);
            }
            else
            {
                //new protocal
                fState = ReadFile(m_hSerial,// ��� - Handle
                    gval,// ���ݻ�������ַ - Data buffer address
                    12,// ���ݴ�С - Data size
                    &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                    &m_osRead);
            }
        }
        if (!fState)// ��֧���ص� - Overlapped not supported
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number = 0;
        }
        if (address != 10)
        {
            if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 && gval[6] == 0 && gval[7] == 0)
                return -3;
            for (int i = 0; i<8; i++)
                if (gval[i] != pval[i])
                    return -2;
        }
        else
        {
            if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 && gval[6] == 0 && gval[7] == 0)
                return -3;
            if (serinumber_in_dll[0] == 0 && serinumber_in_dll[1] == 0 && serinumber_in_dll[2] == 0 && serinumber_in_dll[3] == 0)
            {
                //old protocal
                for (int i = 0; i<8; i++)
                    if (gval[i] != pval[i])
                        return -2;
            }
            else
            {
                //new protocal
                //			crc=CRC16(gval,10);
                //			if(gval[6]!=((crc>>8)&0xff))
                //				return -2;
                //			if(gval[7]!=(crc & 0xff))
                //				return -2;
                //			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
                if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 && gval[6] == 0 && gval[7] == 0 && gval[8] == 0 && gval[9] == 0 && gval[10] == 0 && gval[11] == 0)
                    return -3;
                for (int i = 0; i<12; i++)
                    if (gval[i] != pval[i])
                        return -2;
            }
        }
        return 1;
    }

    if (g_Commu_type == 1)//tcp.
    {
        SOCKET m_hSocket = NULL;
        m_hSocket = m_tcp_hSocket[nindex];
        //address        the register
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -3 , no response
        //��մ��ڻ�����

        //TS_UC data[12];
        TS_UC data[16];
        // 		TS_UC* data=NULL;
        int nSendNum = 12;
        if (address == 10)
        {
            //data = new TS_UC[16];
            nSendNum = 16;
        }
        else
        {
            //data = new TS_UC[12];
            nSendNum = 12;
        }
        ZeroMemory(data, nSendNum);
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        data[0] = trans_high_id;
        data[1] = trans_low_id;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        data[5] = 6;
        //		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        data[6] = device_var;
        data[7] = 6;
        data[8] = address >> 8 & 0xFF;
        data[9] = address & 0xFF;
        if (address == 10 && (serinumber_in_dll[0] != 0 || serinumber_in_dll[1] != 0 || serinumber_in_dll[2] != 0 || serinumber_in_dll[3] != 0))
            data[10] = 0x55;/////////////////////////////new protocal or write_one 10
        else
            data[10] = (val >> 8) & 0xff;//number hi
        data[11] = val & 0xff;//number lo

        if (address == 10)
        {

            if (serinumber_in_dll[0] == 0 && serinumber_in_dll[1] == 0 && serinumber_in_dll[2] == 0 && serinumber_in_dll[3] == 0)
            {
            }
            else
            {
                //* uncommend by zgq; we use net work scan tstat through NC
                data[12] = serinumber_in_dll[0];
                data[13] = serinumber_in_dll[1];
                data[14] = serinumber_in_dll[2];
                data[15] = serinumber_in_dll[3];
                //*/
            }
        }


        for (int i = 0; i <= 11; i++)
            gval[i] = 0;/////////////////////////////////////////clear buffer
                        //	TS_US crc;
                        //	DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� // - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address >> 8 & 0xFF;
        pval[3] = address & 0xFF;
        if (address == 10 && (serinumber_in_dll[0] != 0 || serinumber_in_dll[1] != 0 || serinumber_in_dll[2] != 0 || serinumber_in_dll[3] != 0))
            pval[4] = 0x55;/////////////////////////////new protocal or write_one 10
        else
            pval[4] = (val >> 8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo

        if (m_hSocket == INVALID_SOCKET)
        {
            return -1;
        }

        //::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        //::send(m_hSocket,(char*)data,sizeof(data),0);
        int nRet = ::send(m_hSocket, (char*)data, nSendNum, 0);
        if (nRet < 0)	//Add by Fance ,if device is dosconnected , we need to connect the device again;
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ� - Error code: The remote host forcibly closed an existing connection.
            {
                if (last_connected_port != 0)
                    Open_Socket2_multy_thread(last_connected_ip, last_connected_port, nindex);
            }
        }
        //Sleep(SLEEP_TIME+10);
        Sleep(LATENCY_TIME_NET);
        //Sleep(300);
        TS_UC rvdata[17];
        int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

        int nErr = WSAGetLastError();

        memcpy((void*)&gval[0], (void*)&rvdata[6], 13/*sizeof(gval)*/);
        if (address != 10)
        {
            if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 /*&& gval[6]==0 && gval[7]==0*/)
                return -3;
            for (int i = 0; i<5; i++)
                if (gval[i] != pval[i])
                    return -2;
        }
        else
        {
            if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0/* && gval[6]==0 && gval[7]==0*/)
                return -3;
            if (serinumber_in_dll[0] == 0 && serinumber_in_dll[1] == 0 && serinumber_in_dll[2] == 0 && serinumber_in_dll[3] == 0)
            {
                //old protocal
                for (int i = 0; i<5; i++)
                    if (gval[i] != pval[i])
                        return -2;
            }
            else
            {
                //new protocal
                //			crc=CRC16(gval,10);
                //			if(gval[6]!=((crc>>8)&0xff))
                //				return -2;
                //			if(gval[7]!=(crc & 0xff))
                //				return -2;
                //			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
                if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 /*&& gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0*/)
                    return -3;
                for (int i = 0; i<5; i++)
                    if (gval[i] != pval[i])
                        return -2;
            }
        }

        //delete []data;
        return 1;
    }
    return -1;//add by Fance
              ///////////////////////////////////////////////////////////
}

OUTPUT int Write_One(TS_UC device_var,TS_US address,TS_US val)
{
    if (g_Commu_type==0)
    {
        //address        the register
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -3 , no response
        //��մ��ڻ�����

        //		gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];
        for(int i=0; i<=11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
            pval[4]=0x55;/////////////////////////////new protocal or write_one 10
        else
            pval[4] = (val>>8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo
        if(address!=10)
        {
            crc = CRC16(pval,6);
            pval[6] = (crc >>8) & 0xff;
            pval[7] = crc & 0xff;
        }
        else
        {
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                crc = CRC16(pval,6);
                pval[6] = (crc >>8) & 0xff;
                pval[7] = crc & 0xff;
            }
            else
            {
                pval[6]=serinumber_in_dll[0];
                pval[7]=serinumber_in_dll[1];
                pval[8]=serinumber_in_dll[2];
                pval[9]=serinumber_in_dll[3];
                crc = CRC16(pval,10);
                pval[10] = (crc >>8) & 0xff;
                pval[11] = crc & 0xff;
            }
        }

        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        int fState;
        Sleep(50);
        if(address!=10)
        {
            fState=WriteFile(m_hSerial,// ��� - Handle
                             pval,// ���ݻ�������ַ - Data buffer address
                             8,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                             &m_osWrite);
        }
        else
        {
            //==10 register
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                fState=WriteFile(m_hSerial,// ��� - Handle
                                 pval,// ���ݻ�������ַ - Data buffer address
                                 8,// ���ݴ�С - Data size
                                 &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                                 &m_osWrite);
            }
            else
            {
                //new protocal
                fState=WriteFile(m_hSerial,// ��� - Handle
                                 pval,// ���ݻ�������ַ - Data buffer address
                                 12,// ���ݴ�С - Data size
                                 &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                                 &m_osWrite);
            }
        }
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
                //			if(GetLastError()==ERROR_IO_INCOMPLETE)
                //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        Sleep(LATENCY_TIME_COM);
        if(address!=10)
        {
            fState=ReadFile(m_hSerial,// ��� - Handle
                            gval,// ���ݻ�������ַ - Data buffer address
                            8,// ���ݴ�С - Data size
                            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                            &m_osRead);
        }
        else
        {
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                fState=ReadFile(m_hSerial,// ��� - Handle
                                gval,// ���ݻ�������ַ - Data buffer address
                                8,// ���ݴ�С - Data size
                                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                                &m_osRead);
            }
            else
            {
                //new protocal
                fState=ReadFile(m_hSerial,// ��� - Handle
                                gval,// ���ݻ�������ַ - Data buffer address
                                12,// ���ݴ�С - Data size
                                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                                &m_osRead);
            }
        }
        if(!fState)// ��֧���ص�
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        if(address!=10)
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0)
                return -3;
            for(int i=0; i<8; i++)
                if(gval[i]!=pval[i])
                    return -2;
        }
        else
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0)
                return -3;
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                for(int i=0; i<8; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
            else
            {
                //new protocal
                //			crc=CRC16(gval,10);
                //			if(gval[6]!=((crc>>8)&0xff))
                //				return -2;
                //			if(gval[7]!=(crc & 0xff))
                //				return -2;
                //			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
                if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0)
                    return -3;
                for(int i=0; i<12; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
        }
        return 1;
    }

    if(g_Commu_type==1)//tcp.
    {
        //address        the register
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -3 , no response
        //��մ��ڻ�����

        //TS_UC data[12];
        TS_UC data[16];
// 		TS_UC* data=NULL;
        int nSendNum = 12;
        if (address==10)
        {
            //data = new TS_UC[16];
            nSendNum = 16;
        }
        else
        {
            //data = new TS_UC[12];
            nSendNum = 12;
        }
        ZeroMemory(data, nSendNum);
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        data[0]= trans_high_id;
        data[1]= trans_low_id;
        data[2]=0;
        data[3]=0;
        data[4]=0;
        data[5]=6;
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        data[6] = device_var;
        data[7] = 6;
        data[8] = address>>8 & 0xFF ;
        data[9] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
            data[10]=0x55;/////////////////////////////new protocal or write_one 10
        else
            data[10] = (val>>8) & 0xff;//number hi
        data[11] = val & 0xff;//number lo

        if(address==10)
        {

            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
            }
            else
            {
                //* uncommend by zgq; we use net work scan tstat through NC
                data[12]=serinumber_in_dll[0];
                data[13]=serinumber_in_dll[1];
                data[14]=serinumber_in_dll[2];
                data[15]=serinumber_in_dll[3];
                //*/
            }
        }


        for(int i=0; i<=11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        //	TS_US crc;
        //	DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
            pval[4]=0x55;/////////////////////////////new protocal or write_one 10
        else
            pval[4] = (val>>8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo

        /*
        for(int i=0;i<=11;i++)
        	gval[i]=0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
        	pval[4]=0x55;/////////////////////////////new protocal or write_one 10
        else
        	pval[4] = (val>>8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo
        if(address!=10)
        {
        	crc = CRC16(pval,6);
        	pval[6] = (crc >>8) & 0xff;
        	pval[7] = crc & 0xff;
        }
        else
        {
        	if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
        	{
        		crc = CRC16(pval,6);
        		pval[6] = (crc >>8) & 0xff;
        		pval[7] = crc & 0xff;
        	}
        	else
        	{
        		pval[6]=serinumber_in_dll[0];
        		pval[7]=serinumber_in_dll[1];
        		pval[8]=serinumber_in_dll[2];
        		pval[9]=serinumber_in_dll[3];
        		crc = CRC16(pval,10);
        		pval[10] = (crc >>8) & 0xff;
        		pval[11] = crc & 0xff;
        	}
        }
        */
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        //::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        //::send(m_hSocket,(char*)data,sizeof(data),0);
        int nRet = ::send(m_hSocket,(char*)data,nSendNum,0);
        if(nRet < 0)	//Add by Fance ,if device is dosconnected , we need to connect the device again;
        {
            if(last_connected_port!=0)
                Open_Socket2(last_connected_ip,last_connected_port);
        }
        //Sleep(SLEEP_TIME+10);
        Sleep(LATENCY_TIME_NET);
        //Sleep(300);
        TS_UC rvdata[17];
        int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

        int nErr = WSAGetLastError();

        memcpy((void*)&gval[0],(void*)&rvdata[6],13/*sizeof(gval)*/);
        if(address!=10)
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 /*&& gval[6]==0 && gval[7]==0*/)
                return -3;
            for(int i=0; i<5; i++)
                if(gval[i]!=pval[i])
                    return -2;
        }
        else
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0/* && gval[6]==0 && gval[7]==0*/)
                return -3;
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                for(int i=0; i<5; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
            else
            {
                //new protocal
                //			crc=CRC16(gval,10);
                //			if(gval[6]!=((crc>>8)&0xff))
                //				return -2;
                //			if(gval[7]!=(crc & 0xff))
                //				return -2;
                //			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
                if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 /*&& gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0*/)
                    return -3;
                for(int i=0; i<5; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
        }

        //delete []data;
        return 1;
    }
    return -1;//add by Fance
    ///////////////////////////////////////////////////////////
}

OUTPUT int Write_One_log(TS_UC device_var,TS_US address,TS_US val,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength)
{
    if (g_Commu_type==0)
    {
        //address        the register
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -3 , no response
        //��մ��ڻ�����

        //		gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];
        for(int i=0; i<=11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
            pval[4]=0x55;/////////////////////////////new protocal or write_one 10
        else
            pval[4] = (val>>8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo
        if(address!=10)
        {
            crc = CRC16(pval,6);
            pval[6] = (crc >>8) & 0xff;
            pval[7] = crc & 0xff;
        }
        else
        {
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                crc = CRC16(pval,6);
                pval[6] = (crc >>8) & 0xff;
                pval[7] = crc & 0xff;
            }
            else
            {
                pval[6]=serinumber_in_dll[0];
                pval[7]=serinumber_in_dll[1];
                pval[8]=serinumber_in_dll[2];
                pval[9]=serinumber_in_dll[3];
                crc = CRC16(pval,10);
                pval[10] = (crc >>8) & 0xff;
                pval[11] = crc & 0xff;
            }
        }

        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        int fState;
        Sleep(50);
        if(address!=10)
        {
            *sendDataLength = 8;
            for (int i = 0; i < *sendDataLength; i++)
            {
                *((char*)put_senddate_into_here + i) = pval[i];
            }

            fState=WriteFile(m_hSerial,// ��� - Handle
                             pval,// ���ݻ�������ַ - Data buffer address
                             8,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                             &m_osWrite);
        }
        else
        {
            //==10 register
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                *sendDataLength = 8;
                for (int i = 0; i < *sendDataLength; i++)
                {
                    *((char*)put_senddate_into_here + i) = pval[i];
                }
                fState=WriteFile(m_hSerial,// ��� - Handle
                                 pval,// ���ݻ�������ַ - Data buffer address
                                 8,// ���ݴ�С - Data size
                                 &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                                 &m_osWrite);
            }
            else
            {
                //new protocal
                *sendDataLength = 12;
                for (int i = 0; i < *sendDataLength; i++)
                {
                    *((char*)put_senddate_into_here + i) = pval[i];
                }

                fState=WriteFile(m_hSerial,// ��� - Handle
                                 pval,// ���ݻ�������ַ - Data buffer address
                                 12,// ���ݴ�С - Data size
                                 &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                                 &m_osWrite);
            }
        }
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ� - Wait
                //			if(GetLastError()==ERROR_IO_INCOMPLETE)
                //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number=0;
        }

        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        Sleep(LATENCY_TIME_COM);
        if(address!=10)
        {
            fState=ReadFile(m_hSerial,// ��� - Handle
                            gval,// ���ݻ�������ַ - Data buffer address
                            8,// ���ݴ�С - Data size
                            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                            &m_osRead);
        }
        else
        {
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                fState=ReadFile(m_hSerial,// ��� - Handle
                                gval,// ���ݻ�������ַ - Data buffer address
                                8,// ���ݴ�С - Data size
                                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                                &m_osRead);
            }
            else
            {
                //new protocal
                fState=ReadFile(m_hSerial,// ��� - Handle
                                gval,// ���ݻ�������ַ - Data buffer address
                                12,// ���ݴ�С - Data size
                                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                                &m_osRead);
            }
        }
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }

        *recvDataLength = m_had_send_data_number;
        for (int i = 0; i < *recvDataLength; i++)
        {
            *((char*)put_revdata_into_here + i) = gval[i];
        }

        if(address!=10)
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0)
                return -3;
            for(int i=0; i<8; i++)
                if(gval[i]!=pval[i])
                    return -2;
        }
        else
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0)
                return -3;
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                for(int i=0; i<8; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
            else
            {
                //new protocal
                //			crc=CRC16(gval,10);
                //			if(gval[6]!=((crc>>8)&0xff))
                //				return -2;
                //			if(gval[7]!=(crc & 0xff))
                //				return -2;
                //			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
                if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0)
                    return -3;
                for(int i=0; i<12; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
        }
        return 1;
    }

    if(g_Commu_type==1)//tcp.
    {
        //address        the register
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -3 , no response
        //��մ��ڻ����� - Clear serial port buffer

        //TS_UC data[12];
        TS_UC data[16];
// 		TS_UC* data=NULL;
        int nSendNum = 12;
        if (address==10)
        {
            //data = new TS_UC[16];
            nSendNum = 16;
        }
        else
        {
            //data = new TS_UC[12];
            nSendNum = 12;
        }
        ZeroMemory(data, nSendNum);


        ++g_data_to_send[1];
        if (g_data_to_send[1]%256==0)
        {
            ++g_data_to_send[0];
            g_data_to_send[1]=0;
        }

        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;

        data[0]= trans_high_id;
        data[1]= trans_low_id;
        data[2]=g_data_to_send[2];
        data[3]=g_data_to_send[3];
        data[4]=g_data_to_send[4];
        data[5]=g_data_to_send[5];
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        data[6] = device_var;
        data[7] = 6;
        data[8] = address>>8 & 0xFF ;
        data[9] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
            data[10]=0x55;/////////////////////////////new protocal or write_one 10
        else
            data[10] = (val>>8) & 0xff;//number hi
        data[11] = val & 0xff;//number lo

        if(address==10)
        {

            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
            }
            else
            {
                //* uncommend by zgq; we use net work scan tstat through NC
                data[12]=serinumber_in_dll[0];
                data[13]=serinumber_in_dll[1];
                data[14]=serinumber_in_dll[2];
                data[15]=serinumber_in_dll[3];
                //*/
            }
        }


        for(int i=0; i<=11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        //	TS_US crc;
        //	DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
            pval[4]=0x55;/////////////////////////////new protocal or write_one 10
        else
            pval[4] = (val>>8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo

        /*
        for(int i=0;i<=11;i++)
        	gval[i]=0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
        	pval[4]=0x55;/////////////////////////////new protocal or write_one 10
        else
        	pval[4] = (val>>8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo
        if(address!=10)
        {
        	crc = CRC16(pval,6);
        	pval[6] = (crc >>8) & 0xff;
        	pval[7] = crc & 0xff;
        }
        else
        {
        	if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
        	{
        		crc = CRC16(pval,6);
        		pval[6] = (crc >>8) & 0xff;
        		pval[7] = crc & 0xff;
        	}
        	else
        	{
        		pval[6]=serinumber_in_dll[0];
        		pval[7]=serinumber_in_dll[1];
        		pval[8]=serinumber_in_dll[2];
        		pval[9]=serinumber_in_dll[3];
        		crc = CRC16(pval,10);
        		pval[10] = (crc >>8) & 0xff;
        		pval[11] = crc & 0xff;
        	}
        }
        */
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        //::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        //::send(m_hSocket,(char*)data,sizeof(data),0);
        *sendDataLength = nSendNum;
        for (int i = 0; i < *sendDataLength; i++)
        {
            *((char*)put_senddate_into_here + i) = data[i];
        }

        int nRet = ::send(m_hSocket,(char*)data,nSendNum,0);

        //Sleep(SLEEP_TIME+10);
        Sleep(LATENCY_TIME_NET);
        //Sleep(300);
        TS_UC rvdata[17];
        int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

        *recvDataLength = nRecv;
        for (int i = 0; i < *recvDataLength; i++)
        {
            *((char*)put_revdata_into_here + i) = rvdata[i];
        }

        int nErr = WSAGetLastError();

        memcpy((void*)&gval[0],(void*)&rvdata[6],13/*sizeof(gval)*/);
        if(address!=10)
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 /*&& gval[6]==0 && gval[7]==0*/)
                return -3;
            for(int i=0; i<5; i++)
                if(gval[i]!=pval[i])
                    return -2;
        }
        else
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0/* && gval[6]==0 && gval[7]==0*/)
                return -3;
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                for(int i=0; i<5; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
            else
            {
                //new protocal
                //			crc=CRC16(gval,10);
                //			if(gval[6]!=((crc>>8)&0xff))
                //				return -2;
                //			if(gval[7]!=(crc & 0xff))
                //				return -2;
                //			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
                if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 /*&& gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0*/)
                    return -3;
                for(int i=0; i<5; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
        }

        //delete []data;
        return 1;
    }
    return -1;//add by Fance
    ///////////////////////////////////////////////////////////
}
//MF 0x05 Write Coil
OUTPUT int Write_Coil_log(TS_UC device_var, TS_US address, TS_BOOL val, unsigned char *put_senddate_into_here, unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength)
{
	if (g_Commu_type == 0)
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//��մ��ڻ�����

		//		gval[8]={'\0'};//the data that get
		//      TS_UC  pval[9];
		for (int i = 0; i <= 11; i++)
			gval[i] = 0;/////////////////////////////////////////clear buffer
		TS_US crc;
		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
		pval[0] = device_var;
		pval[1] = 5;
		pval[2] = address >> 8 & 0xFF;
		pval[3] = address & 0xFF;
		if (val)
		{
			pval[4] = 0xFF;
		} 
		else
		{
			pval[4] = 0x00;
		}
		pval[5] = 0x00;
		 
			crc = CRC16(pval, 6);
			pval[6] = (crc >> 8) & 0xff;
			pval[7] = crc & 0xff;
	 

		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));
		if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
			return -2;
		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
		int fState;
		Sleep(50);
		 
			*sendDataLength = 8;
			for (int i = 0; i < *sendDataLength; i++)
			{
				*((char*)put_senddate_into_here + i) = pval[i];
			}

			fState = WriteFile(m_hSerial,// ��� - Handle
				pval,// ���ݻ�������ַ - Data buffer address
				8,// ���ݴ�С - Data size
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
				&m_osWrite);


		if (!fState)// ��֧���ص� - Overlapping not supported
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
																								   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
																								   //				AfxMessageBox("wrong1");
			}
			else
				m_had_send_data_number = 0;
		}

		///////////////////////////up is write
		/////////////**************down is read
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, g_cs_com_section_read)) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error
		Sleep(LATENCY_TIME_COM);

			fState = ReadFile(m_hSerial,// ��� - Handle
				gval,// ���ݻ�������ַ - Data buffer address
				8,// ���ݴ�С - Data size
				&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
				&m_osRead);


		if (!fState)// ��֧���ص� - Does not support overlap
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
			}
			else
				m_had_send_data_number = 0;
		}

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = gval[i];
		}

		 
			if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 && gval[6] == 0 && gval[7] == 0)
				return -3;
			for (int i = 0; i<8; i++)
				if (gval[i] != pval[i])
					return -2;
		 
		 
		return 1;
	}

	if (g_Commu_type == 1)//tcp.
	{
		//address        the register
		//val         the value that you want to write to the register
		//the return value == -1 ,no connecting
		//the return value == -3 , no response
		//��մ��ڻ�����

		//TS_UC data[12];
		TS_UC data[16];
		// 		TS_UC* data=NULL;
		int nSendNum = 12;
		if (address == 10)
		{
			//data = new TS_UC[16];
			nSendNum = 16;
		}
		else
		{
			//data = new TS_UC[12];
			nSendNum = 12;
		}
		ZeroMemory(data, nSendNum);


		++g_data_to_send[1];
		if (g_data_to_send[1] % 256 == 0)
		{
			++g_data_to_send[0];
			g_data_to_send[1] = 0;
		}

        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        data[0] = trans_high_id;
        data[1] = trans_low_id;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        data[5] = 6;

		//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
		data[6] = device_var;
		data[7] = 5;
		data[8] = address >> 8 & 0xFF;
		data[9] = address & 0xFF;
		 

		if (val)
		{
			data[10] = 0xFF;
		}
		else
		{
			data[10] = 0x00;
		}
		data[11] = 0x00;

		for (int i = 0; i <= 11; i++)
			gval[i] = 0;
		/////////////////////////////////////////clear buffer
						//	TS_US crc;
						//	DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
		pval[0] = device_var;
		pval[1] = 5;
		pval[2] = address >> 8 & 0xFF;
		pval[3] = address & 0xFF;
		if (val)
		{
			pval[4] = 0xFF;
		}
		else
		{
			pval[4] = 0x00;
		}
		pval[5] = 0x00;
							  
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}

		//::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
		//::send(m_hSocket,(char*)data,sizeof(data),0);
		*sendDataLength = nSendNum;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data[i];
		}

		int nRet = ::send(m_hSocket, (char*)data, nSendNum, 0);

		//Sleep(SLEEP_TIME+10);
		Sleep(LATENCY_TIME_NET);
		//Sleep(300);
		TS_UC rvdata[17];
		int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = rvdata[i];
		}

		int nErr = WSAGetLastError();

		memcpy((void*)&gval[0], (void*)&rvdata[6], 13/*sizeof(gval)*/);
		
			if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 )
				return -3;
			for (int i = 0; i<5; i++)
				if (gval[i] != pval[i])
					return -2;
		
		

		
		return 1;
	}
	return -1;//add by Fance
			  ///////////////////////////////////////////////////////////
}

#if 1
OUTPUT int read_multi(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,int length)
{
    if(g_Commu_type==0)
    {
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting

        TS_UC to_send_data[300]= {'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        TS_UC data_to_send[8]= {'\0'}; //the array to used in writefile()
        data_to_send[0]=device_var;//slave address
        data_to_send[1]=3;//function multiple
        data_to_send[2]=start_address>>8 & 0xff;//starting address hi
        data_to_send[3]=start_address & 0xff;//starting address lo
        data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[5]=length & 0xff;//quantity of registers lo
        TS_US crc=CRC16(data_to_send,6);
        data_to_send[6]=(crc>>8) & 0xff;
        data_to_send[7]=crc & 0xff;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
        if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_mulwrite))==NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0 ;
        ///////////////////////////////////////////////////////send the to read message



        int fState=WriteFile(m_hSerial,// ��� - Handle
                             data_to_send,// ���ݻ�������ַ - Data buffer address
                             8,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                             &m_osMulWrite);
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read

        Sleep(LATENCY_TIME_COM);
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - Handle
                        to_send_data,// ���ݻ�������ַ - Data buffer address
                        length*2+5,// ���ݴ�С - Data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
            return -2;
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
            return -2;
        if(to_send_data[length*2+4]!=(crc & 0xff))
            return -2;
        for(int i=0; i<length; i++)
            put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
        return length;
    }
    if(g_Commu_type==1)//tcp.
    {
        /*
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting
        TS_UC to_send_data[700]={'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        TS_UC data_to_send[8]={'\0'};//the array to used in writefile()
        data_to_send[0]=device_var;//slave address
        data_to_send[1]=3;//function multiple
        data_to_send[2]=start_address>>8 & 0xff;//starting address hi
        data_to_send[3]=start_address & 0xff;//starting address lo
        data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[5]=length & 0xff;//quantity of registers lo
        TS_US crc=CRC16(data_to_send,6);
        data_to_send[6]=(crc>>8) & 0xff;
        data_to_send[7]=crc & 0xff;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSocket==INVALID_SOCKET)
        {
        	return -1;
        }
        ::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),MSG_OOB);

        int nn=sizeof(to_send_data);
        int nRecv = ::recv(m_hSocket, (char*)to_send_data, length*2+5, 0);

        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
        	return -2;
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
        	return -2;
        if(to_send_data[length*2+4]!=(crc & 0xff))
        	return -2;
        for(int i=0;i<length;i++)
        	put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
        return length;
        */
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting

        TS_UC to_send_data[700]= {'\0'};
        TS_UC to_Reive_data[700]= {'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        TS_UC data_to_send[12]= {'\0'}; //the array to used in writefile()
        data_to_send[0]= trans_high_id;
        data_to_send[1]= trans_low_id;
        data_to_send[2]=0;
        data_to_send[3]=0;
        data_to_send[4]=0;
        data_to_send[5]=6;



        data_to_send[6]=device_var;//slave address
        data_to_send[7]=3;//function multiple
        data_to_send[8]=start_address>>8 & 0xff;//starting address hi
        data_to_send[9]=start_address & 0xff;//starting address lo
        data_to_send[10]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[11]=length & 0xff;//quantity of registers lo

        //TS_US crc=CRC16(data_to_send,6);
        //data_to_send[6]=(crc>>8) & 0xff;
        //data_to_send[7]=crc & 0xff;/

//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        ::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),0);

        Sleep(LATENCY_TIME_NET);
        int nn=sizeof(to_Reive_data);

        unsigned long bytes_available;
        ioctlsocket(m_hSocket,FIONREAD,&bytes_available);
        if (bytes_available == 0) return -1;
        int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length*2+12, 0);

        if (nRecv < 0)
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ� - Error code 10054: The remote host forcibly closed an existing connection.
            {
                if (last_connected_port != 0)
                {
                    Open_Socket2(last_connected_ip, last_connected_port);
                    return -1;
                }
            }
        }

        memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
            return -2;
        /*
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
        	return -2;

        if(to_send_data[length*2+4]!=(crc & 0xff))
        	return -2;
        	*/
        for(int i=0; i<length; i++)
            put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
        return length;
    }
    return -1;
}

#endif

OUTPUT int read_multi_log(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,TS_US length,
	                      unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here,
	                       int* sendDataLength, int* recvDataLength)
{
    if(g_Commu_type==0)
    {
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting

        TS_UC to_send_data[300]= {'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        TS_UC data_to_send[8]= {'\0'}; //the array to used in writefile()
        data_to_send[0]=device_var;//slave address
        data_to_send[1]=3;//function multiple
        data_to_send[2]=start_address>>8 & 0xff;//starting address hi
        data_to_send[3]=start_address & 0xff;//starting address lo
        data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[5]=length & 0xff;//quantity of registers lo
        TS_US crc=CRC16(data_to_send,6);
        data_to_send[6]=(crc>>8) & 0xff;
        data_to_send[7]=crc & 0xff;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
        if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_mulwrite))==NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0;

        *sendDataLength = 8;
        for (int i = 0; i < *sendDataLength; i++)
        {
            *((char*)put_senddate_into_here + i) = data_to_send[i];
        }

        ///////////////////////////////////////////////////////send the to read message
        int fState=WriteFile(m_hSerial,// ��� - Handle
                             data_to_send,// ���ݻ�������ַ - Data buffer address
                             8,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                             &m_osMulWrite);
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read

        Sleep(LATENCY_TIME_COM);
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -3;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - Handle
                        to_send_data,// ���ݻ�������ַ - Data buffer address
                        length*2+5,// ���ݴ�С - Data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }

        *recvDataLength = m_had_send_data_number;
        for (int i = 0; i < *recvDataLength; i++)
        {
            *((char*)put_revdata_into_here + i) = to_send_data[i];
        }

        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
            return -4;
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
            return -5;
        if(to_send_data[length*2+4]!=(crc & 0xff))
            return -5;
        for(int i=0; i<length; i++)
            put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];


        /*for (int i=0;i<8;i++)
        {
        	put_senddate_into_here[i]=data_to_send[i];
        }
        for (int i=0;i<2*length+5;i++)
        {
        	put_revdata_into_here[i]=to_send_data[i];
        }*/
        return length;
    }
    if(g_Commu_type==1)//tcp.
    {
        /*
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting
        TS_UC to_send_data[700]={'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        TS_UC data_to_send[8]={'\0'};//the array to used in writefile()
        data_to_send[0]=device_var;//slave address
        data_to_send[1]=3;//function multiple
        data_to_send[2]=start_address>>8 & 0xff;//starting address hi
        data_to_send[3]=start_address & 0xff;//starting address lo
        data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[5]=length & 0xff;//quantity of registers lo
        TS_US crc=CRC16(data_to_send,6);
        data_to_send[6]=(crc>>8) & 0xff;
        data_to_send[7]=crc & 0xff;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSocket==INVALID_SOCKET)
        {
        	return -1;
        }
        ::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),MSG_OOB);

        int nn=sizeof(to_send_data);
        int nRecv = ::recv(m_hSocket, (char*)to_send_data, length*2+5, 0);

        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
        	return -2;
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
        	return -2;
        if(to_send_data[length*2+4]!=(crc & 0xff))
        	return -2;
        for(int i=0;i<length;i++)
        	put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
        return length;
        */
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting

        TS_UC to_send_data[700]= {'\0'};
        TS_UC to_Reive_data[700]= {'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        TS_UC data_to_send[12]= {'\0'}; //the array to used in writefile()
        ++g_data_to_send[1];
        if (g_data_to_send[1]%256==0)
        {
            ++g_data_to_send[0];
            g_data_to_send[1]=0;
        }

        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;

        data_to_send[0]= trans_high_id;
        data_to_send[1]= trans_low_id;

        data_to_send[2]=g_data_to_send[2];
        data_to_send[3]=g_data_to_send[3];

        data_to_send[4]=g_data_to_send[4];
        data_to_send[5]=g_data_to_send[5];



        data_to_send[6]=device_var;//slave address
        data_to_send[7]=3;//function multiple
        data_to_send[8]=start_address>>8 & 0xff;//starting address hi
        data_to_send[9]=start_address & 0xff;//starting address lo
        data_to_send[10]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[11]=length & 0xff;//quantity of registers lo

        //TS_US crc=CRC16(data_to_send,6);
        //data_to_send[6]=(crc>>8) & 0xff;
        //data_to_send[7]=crc & 0xff;/

//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        *sendDataLength = sizeof(data_to_send);
        for (int i = 0; i < *sendDataLength; i++)
        {
            *((char*)put_senddate_into_here + i) = data_to_send[i];
        }

        ::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),0);

        Sleep(LATENCY_TIME_NET);
        int nn=sizeof(to_Reive_data);
        int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length*2+12, 0);

        if (nRecv < 0)
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ� - Error code 10054: The remote host forcibly closed an existing connection.
            {
                if (last_connected_port != 0)
                {
                    Open_Socket2(last_connected_ip, last_connected_port);
                    return -1;
                }
            }
        }

        *recvDataLength = nRecv;
        for (int i = 0; i < *recvDataLength; i++)
        {
            *((char*)put_revdata_into_here + i) = to_Reive_data[i];
        }

        memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
            return -2;
        /*
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
        	return -2;

        if(to_send_data[length*2+4]!=(crc & 0xff))
        	return -2;
        	*/
        for(int i=0; i<length; i++)
            put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
        /*for (int i=6;i<12;i++)
        {
          put_senddate_into_here[i]=data_to_send[i];
        }
        for (int i=0;i<2*length+3;i++)
        {
          put_revdata_into_here[i]=to_send_data[i];
        }*/
        return length;
    }
    return -1;
}

OUTPUT int write_multi(TS_UC device_var,TS_UC *to_write,TS_US start_address,TS_US length)
{
    if(g_Commu_type==0)//
    {
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        HCURSOR hc;//load mouse cursor
        TS_UC data_to_write[600]= {'\0'};
        data_to_write[0]=device_var;
        data_to_write[1]=0x10;
        data_to_write[2]=start_address >> 8 & 0xff;
        data_to_write[3]=start_address & 0xff;
        data_to_write[4]=0;
        data_to_write[5]=length;
        data_to_write[6]=length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
        for(int i=0; i<length; i++)
            data_to_write[7+i]=to_write[i];
        //	TS_US crc=CRC16(data_to_write,i+7);
        //	data_to_write[i+7]=crc>>8 & 0xff;
        //	data_to_write[i+8]=crc & 0xff;

        TS_US crc=CRC16(data_to_write,length+7);
        data_to_write[length+7]=crc>>8 & 0xff;
        data_to_write[length+8]=crc & 0xff;

        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //length is the data length,if you want to write 128 bite,the length == 128
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
        if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_mulwrite))==NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0 ;
        ///////////////////////////////////////////////////////send the to read message
        int fState=WriteFile(m_hSerial,// ��� - Handle
                             data_to_write,// ���ݻ�������ַ - Data buffer address
                             length+9,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                             &m_osMulWrite);
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        Sleep(LATENCY_TIME_COM);
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - Handle
                        gval,// ���ݻ�������ַ - Data buffer address
                        8,// ���ݴ�С - Data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////////////////////////////////////
        for(int i=0; i<6; i++)
            if(gval[i]!=*(data_to_write+i))
                return -2;
        crc=CRC16(gval,6);
        if(gval[6]!=((crc>>8)&0xff))
            return -2;
        if(gval[7]!=(crc & 0xff))
            return -2;
        return 1;
    }
    if(g_Commu_type==1)//tcp.
    {
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        HCURSOR hc;//load mouse cursor
        TS_UC data_to_write[600]= {'\0'};
        TS_UC data_back_write[600]= {'\0'};
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        data_to_write[0]= trans_high_id;
        data_to_write[1]= trans_low_id;
        data_to_write[2]=0;
        data_to_write[3]=0;
        data_to_write[4]=0;
        data_to_write[5]=6;

        data_to_write[6]=device_var;
        data_to_write[7]=0x10;
        data_to_write[8]=start_address >> 8 & 0xff;
        data_to_write[9]=start_address & 0xff;
        data_to_write[10]=0;
        data_to_write[11]=length;
        data_to_write[12]=length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
        for(int i=0; i<length; i++)
            data_to_write[13+i]=to_write[i];


        //	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
        //	TS_US crc=CRC16(data_to_write,i+7);
        //	data_to_write[i+7]=crc>>8 & 0xff;
        //	data_to_write[i+8]=crc & 0xff;
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //length is the data length,if you want to write 128 bite,the length == 128
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        int n=::send(m_hSocket,(char *)data_to_write,13+length,0);
        int nRecv = ::recv(m_hSocket, (char *)data_back_write,13, 0);
        if(nRecv<0)
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)    //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ� 10060 ��ʱ��ᳬʱ; - Error code 10054: The remote host forcibly closed an existing connection. 10060 sometimes times out;
            {
                if (last_connected_port != 0)
                {
                    Open_Socket2(last_connected_ip, last_connected_port);
                    return -1;
                }
            }
            return -2;
        }
        //	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
        for(int i=0; i<6; i++)
            if(data_back_write[i+6]!=*(data_to_write+i+6))
                return -2;
        return 1;

    }
    return -1;
}

OUTPUT int write_multi_log(TS_UC device_var,TS_UC *to_write,TS_US start_address,TS_US length,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here, int* sendDataLength, int* recvDataLength)
{
    if(g_Commu_type==0)//
    {
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        HCURSOR hc;//load mouse cursor
        TS_UC data_to_write[600]= {'\0'};
        data_to_write[0]=device_var;
        data_to_write[1]=0x10;
        data_to_write[2]=start_address >> 8 & 0xff;
        data_to_write[3]=start_address & 0xff;
        data_to_write[4]=0;
        data_to_write[5]=length;
        data_to_write[6]=length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
        for(int i=0; i<length; i++)
            data_to_write[7+i]=to_write[i];
        //	TS_US crc=CRC16(data_to_write,i+7);
        //	data_to_write[i+7]=crc>>8 & 0xff;
        //	data_to_write[i+8]=crc & 0xff;

        TS_US crc=CRC16(data_to_write,length+7);
        data_to_write[length+7]=crc>>8 & 0xff;
        data_to_write[length+8]=crc & 0xff;

        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //length is the data length,if you want to write 128 bite,the length == 128
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
        if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_mulwrite))==NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0 ;

        *sendDataLength = length + 9;
        for (int i = 0; i < *sendDataLength; i++)
        {
            *((char*)put_senddate_into_here + i) = data_to_write[i];
        }

        ///////////////////////////////////////////////////////send the to read message
        int fState=WriteFile(m_hSerial,// ��� - Handle
                             data_to_write,// ���ݻ�������ַ - Data buffer address
                             length+9,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                             &m_osMulWrite);
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        Sleep(LATENCY_TIME_COM);
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - Handle
                        gval,// ���ݻ�������ַ - Data buffer address
                        8,// ���ݴ�С - Data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - Overlapping not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }

        *recvDataLength = m_had_send_data_number;
        for (int i = 0; i < *recvDataLength; i++)
        {
            *((char*)put_revdata_into_here + i) = gval[i];
        }

        ///////////////////////////////////////////////////////////
        for(int i=0; i<6; i++)
            if(gval[i]!=*(data_to_write+i))
                return -2;
        crc=CRC16(gval,6);
        if(gval[6]!=((crc>>8)&0xff))
            return -2;
        if(gval[7]!=(crc & 0xff))
            return -2;
        return 1;
    }
    if(g_Commu_type==1)//tcp.
    {
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        HCURSOR hc;//load mouse cursor
        TS_UC data_to_write[600]= {'\0'};
        TS_UC data_back_write[600]= {'\0'};


        ++g_data_to_send[1];
        if (g_data_to_send[1]%256==0)
        {
            ++g_data_to_send[0];
            g_data_to_send[1]=0;
        }

        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;

        data_to_write[0]= trans_high_id;
        data_to_write[1]= trans_low_id;
        data_to_write[2]=g_data_to_send[2];
        data_to_write[3]=g_data_to_send[3];
        data_to_write[4]=g_data_to_send[4];
        data_to_write[5]=g_data_to_send[5];



        data_to_write[6]=device_var;
        data_to_write[7]=0x10;
        data_to_write[8]=start_address >> 8 & 0xff;
        data_to_write[9]=start_address & 0xff;
        data_to_write[10]=0;
        data_to_write[11]=length;
        data_to_write[12]=length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
        for(int i=0; i<length; i++)
            data_to_write[13+i]=to_write[i];


        //	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
        //	TS_US crc=CRC16(data_to_write,i+7);
        //	data_to_write[i+7]=crc>>8 & 0xff;
        //	data_to_write[i+8]=crc & 0xff;
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //length is the data length,if you want to write 128 bite,the length == 128
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        *sendDataLength = 13 + length;
        for (int i = 0; i < *sendDataLength; i++)
        {
            *((char*)put_senddate_into_here + i) = data_to_write[i];
        }

        int n=::send(m_hSocket,(char *)data_to_write,13+length,0);
        int nRecv = ::recv(m_hSocket, (char *)data_back_write,13, 0);

        *recvDataLength = nRecv;
        for (int i = 0; i < *recvDataLength; i++)
        {
            *((char*)put_revdata_into_here + i) = data_back_write[i];
        }

        if(nRecv<0)
        {
            return -2;
        }
        //	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
        for(int i=0; i<6; i++)
            if(data_back_write[i+6]!=*(data_to_write+i+6))
                return -2;
        return 1;

    }
    return -1;
}

OUTPUT int NetController_CheckTstatOnline(TS_UC devLo,TS_UC devHi)
{
    if(g_Commu_type==0)
    {
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ����� - Clear serial port buffer
        //the return value is the register address

        //Sleep(150);       //must use this function to slow computer
        if(devLo<1 || devHi>254)
            return -5;
        if(m_hSerial==NULL)
        {
            return -1;
        }
        int the_return_value;
        int the_return_value2=0;
        the_return_value=NetController_CheckTstatOnline2(devLo,devHi);
        //down is inspect result first scan
        if(the_return_value==-4)
        {
            the_return_value=NetController_CheckTstatOnline2(devLo,devHi);
            return the_return_value;
        }
        if(old_or_new_scan_protocal_in_dll==1)
        {
            //new protocal
            if(the_return_value>0)
            {
                the_return_value2=NetController_CheckTstatOnline2(devLo,devHi);
                if(the_return_value2!=-4)
                    the_return_value=the_return_value2;
            }
            return the_return_value;
        }
        else if(old_or_new_scan_protocal_in_dll==2)
        {
            //old protocal
            if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
                return the_return_value;
            int i=0;
            do
            {
                the_return_value=NetController_CheckTstatOnline2(devLo,devHi);
                if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
                    return the_return_value;
                else if(the_return_value>0)
                    i++;
            }
            while(i<3);
            return the_return_value;
        }
        //	if(the_return_value>0)
        //		TRACE("^-^ ^-^ %d\n",the_return_value);
        return the_return_value;
    }

    if(g_Commu_type==1)//tcp
    {
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ�����
        //the return value is the register address
        //Sleep(50);       //must use this function to slow computer
        if(devLo<1 || devHi>254)
            return -5;
        if (m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }
        int the_return_value;
        int the_return_value2=0;
        the_return_value=NetController_CheckTstatOnline2(devLo,devHi);
        //down is inspect result first scan
        if(the_return_value==-4)
        {
            the_return_value=NetController_CheckTstatOnline2(devLo,devHi);
            return the_return_value;
        }
        if(old_or_new_scan_protocal_in_dll==1)
        {
            //new protocal
            if(the_return_value>0)
            {
                the_return_value2=NetController_CheckTstatOnline2(devLo,devHi);
                if(the_return_value2!=-4)
                    the_return_value=the_return_value2;
            }
            return the_return_value;
        }
        else if(old_or_new_scan_protocal_in_dll==2)
        {
            //old protocal
            if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
                return the_return_value;
            int i=0;
            do
            {
                the_return_value=NetController_CheckTstatOnline2(devLo,devHi);
                if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
                    return the_return_value;
                else if(the_return_value>0)
                    i++;
            }
            while(i<3);
            return the_return_value;
        }
        //	if(the_return_value>0)
        //		TRACE("^-^ ^-^ %d\n",the_return_value);
        return the_return_value;
    }

    return -1;
}

OUTPUT int NetController_CheckTstatOnline2(TS_UC devLo,TS_UC devHi)
{
    if(g_Commu_type==0)
    {
        //the second time
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ�����
        //the return value is the register address

        if(devLo<1 || devHi>254)
            return -5;
        //the input inspect
        for(int i=0; i<13; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_UC  pval[6];
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = 255;
        pval[1] = 26;  //put comments here,
        pval[2] = devHi;
        pval[3] = devLo;
        crc = CRC16(pval,4);
        pval[4] = (crc >>8) & 0xff;
        pval[5] = crc & 0xff;

        if(m_hSerial==NULL)
        {
            return -1;
        }

        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear buffer
        int fState=WriteFile(m_hSerial,// ��� - Handle
                             pval,// ���ݻ�������ַ - Data buffer address
                             6,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Number of bytes sent
                             &m_osWrite);
        if(!fState)//��֧���ص�
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
                //			if(GetLastError()==ERROR_IO_INCOMPLETE)
                //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number=0;
        }
        //	TRACE("%d T:%x %x %x %x %x %x\n",ddd,pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
        //CloseHandle(m_osWrite.hEvent);
        ///////////////////////////up is write
        Sleep(LATENCY_TIME_COM);//SLEEP_TIME);//because that scan have a delay lower 75ms
        //	Sleep(300);
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0;
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - Handle
                        gval,// ���ݻ�������ַ - Data buffer address
                        13,// ���ݴ�С - Data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - Overlapping not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        //CloseHandle(m_osRead.hEvent);
        /*
        CStdioFile default_file;
        CString saved_path="C:\\modbus_scan_data.txt";
        CString a_line;
        if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
        {
        default_file.SeekToEnd();
        a_line.Format("T:%x %x %x %x %x %x",pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
        default_file.WriteString(a_line+"\n");
        a_line.Format("R:%x %x %x %x %x %x %x %x %x %x %x %x %x",gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
        default_file.WriteString(a_line+"\n");
        default_file.Flush();
        default_file.Close();
        }
        */
        //	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
        if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
        {
            //old scan protocal
            old_or_new_scan_protocal_in_dll=2;
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -4;              //no response ,no connection
            }
            if(gval[5]!=0 || gval[6]!=0)//to inspect
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -3;
            }
            if((gval[0]!=pval[0]) || (gval[1]!=26))
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
            crc=CRC16(gval,3);
            if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
        }
        else
        {
            // new scan protocal,if many old tstat ,get into here ,scan result is oK too.
            old_or_new_scan_protocal_in_dll=1;
            Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
            if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect
                return -3;
            if((gval[0]!=pval[0]) || (gval[1]!=25))
                return -2;
            crc=CRC16(gval,7);
            if( gval[7]!=((crc>>8) & 0xff) )
                return -2;
            if(gval[8]!=(crc & 0xff))
                return -2;
        }
        //here is different with CheckTstatOnline() function
        //	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
        //	TRACE("%d ^-^ ^-^ %d^\n",ddd,gval[2]);
        /*
        if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
        {
        default_file.SeekToEnd();
        a_line.Format("exist:%d",gval[2]);
        default_file.WriteString(a_line+"\n");
        default_file.Flush();
        default_file.Close();
        }*/
        return gval[2];
    }

    if(g_Commu_type==1)
    {
        //the second time
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ�����
        //the return value is the register address
        if(devLo<1 || devHi>254)
            return -5;
        //the input inspect

        for(int i=0; i<13; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_UC  pval[10];
//		TS_US crc;
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent


        pval[0]=1;
        pval[1]=2;
        pval[2]=3;
        pval[3]=4;
        pval[4]=5;
        pval[5]=6;
        pval[6] = 255;
        pval[7] = 26;
        pval[8] = devHi;
        pval[9] = devLo;
        /*
        crc = CRC16(pval,4);
        pval[4] = (crc >>8) & 0xff;
        pval[5] = crc & 0xff;
        */
        if (m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }
        ::send(m_hSocket,(char*)pval,sizeof(pval),0);
        Sleep(LATENCY_TIME_NET);//Sleep(SLEEP_TIME+8);

        TS_UC  rvdata[19];
        for(int i=0; i<19; i++)
            rvdata[i]=0;
        int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);
        if (nRecv>0)
        {
            //�쳣:
            memcpy(gval,(void*)&rvdata[6],sizeof(rvdata)-6);
        }
        if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
        {
            //old scan protocal
            old_or_new_scan_protocal_in_dll=2;
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
            {
                Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
                return -4;              //no response ,no connection
            }
            if(gval[5]!=0 || gval[6]!=0)//to inspect
            {
                Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
                return -3;
            }
            if((gval[0]!=255) || (gval[1]!=26))
            {
                //		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
        }
        else
        {
            // new scan protocal,if many old tstat ,get into here ,scan result is oK too.
            old_or_new_scan_protocal_in_dll=1;
            //	Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
            if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect
                return -3;
            if((gval[0]!=255) || (gval[1]!=26))
                return -2;
        }

        /*
        old_or_new_scan_protocal_in_dll=1;

        if((gval[0]!=pval[6]) || (gval[1]!=26))
        	return -2;
        if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0)
        {
        	//	Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
        	return -4;              //no response ,no connection
        }
        */
        return gval[2];
    }
    return -1;
}

//also return the serial communication handle;
OUTPUT HANDLE GetCommunicationHandle()
{

    /*
    if(g_Commu_type==1&&m_hSocket!=NULL)
    	return m_hSocket;
    else
    	return NULL;
    	*/
    if(g_Commu_type==1)
        return (HANDLE)m_hSocket;
    if(g_Commu_type==0)
        return m_hSerial;

    return NULL;
}

OUTPUT BOOL bTCPDisconnected()
{
    return TRUE;
}

OUTPUT void SetComnicationHandle(int nType,HANDLE hCommunication)
{
    close_com();
    if(g_Commu_type==0)
    {

    }
    if(g_Commu_type==1)
    {
        m_hSocket=(SOCKET)hCommunication;
    }

}


OUTPUT bool open_com(int m_com ,unsigned char com_data_bit,unsigned char com_stop_bit ,unsigned char com_parity_bit)
{
    //open com ,if you want to open "com1",the m_com equal 0;if you want to open "com2",the m_com equal 1
    //you will get the handle to com,m_hSerial,is a extern variable
    //the return value ,true is ok,false is failure
    if ((open_com_port_number_in_dll == m_com) && 
        (com_data_bit_in_dll == com_data_bit ) &&
        (com_stop_bit_in_dll == com_stop_bit) &&
        (com_parity_bit_in_dll == com_parity_bit))
    {
        return true;///////////////////////////same com port ,opened by multi times,it's badly.
    }
    if (m_hSerial != NULL)
    {
        if (m_com_h_serial[m_com] != NULL)
            m_com_h_serial[m_com] = NULL;
        //�رմ��� - Close serial port
        CloseHandle(m_hSerial);
        m_hSerial = NULL;
    }
    /////////////////////////////////////////////////////////////////////����Ĵ���ͨ�Ų���
    // 	LPCSTR lpComNum[6];
    // 	ZeroMemory(lpComNum, )

    CString strCom;
    if (m_com <= 9 && m_com >0)
    {
        strCom.Format(_T("%d"), m_com);
        strCom = _T("COM") + strCom + _T(":");
    }
    else if (m_com > 9)
    {
        strCom.Format(_T("%d"), m_com);
        strCom = _T("\\\\.\\COM") + strCom;
    }

    m_hSerial = CreateFile(strCom, //strCom,//���ھ�����򿪴��� - Serial port handle, open serial port
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д - FILE_FLAG_OVERLAPPED is another form, indicating asynchronous communication, allowing simultaneous read and write; 0 is synchronous read and write
        NULL);


    if (m_hSerial == INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hSerial);
        m_hSerial = NULL;
        return false;
    }
    //if(!SetupComm(m_hSerial, 1024*32, 1024*9))
    if (!SetupComm(m_hSerial, 1024 * 64, 1024 * 32))
    {
        int errornum = GetLastError();
        CloseHandle(m_hSerial);
        m_hSerial = NULL;
    }
    DCB  PortDCB;
    PortDCB.DCBlength = sizeof(DCB);
    // Ĭ�ϴ��ڲ���
    if (!GetCommState(m_hSerial, &PortDCB))
    {
        CloseHandle(m_hSerial);
        m_hSerial = NULL;
        return false;
    }
    //not to change the baudate
    PortDCB.BaudRate = 19200; // baud//attention ,if it is wrong,can't write the com
    PortDCB.ByteSize = com_data_bit;// 8;     // Number of bits/byte, 4-8
    PortDCB.Parity = com_parity_bit;// NOPARITY;
    PortDCB.StopBits = com_stop_bit;// ONESTOPBIT;
    if (!SetCommState(m_hSerial, &PortDCB))
    {
        return false;
    }
    COMMTIMEOUTS CommTimeouts;
    GetCommTimeouts(m_hSerial, &CommTimeouts);
    CommTimeouts.ReadIntervalTimeout = 160;
    CommTimeouts.ReadTotalTimeoutMultiplier = 20;
    CommTimeouts.ReadTotalTimeoutConstant = 360;
    CommTimeouts.WriteTotalTimeoutMultiplier = 20;
    CommTimeouts.WriteTotalTimeoutConstant = 200;

#if 0 //lsc 
    CommTimeouts.ReadIntervalTimeout = 1000;
    CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
    CommTimeouts.ReadTotalTimeoutConstant = 1000;
    CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
    CommTimeouts.WriteTotalTimeoutConstant = 1000;
#endif


    if (!SetCommTimeouts(m_hSerial, &CommTimeouts))
    {
        return false;
    }
    open_com_port_number_in_dll = m_com;
    baudrate_in_dll = 19200;
    com_data_bit_in_dll = com_data_bit;
    com_stop_bit_in_dll = com_stop_bit;
    com_parity_bit_in_dll = com_parity_bit;

    g_cs_com_section_mulwrite.Format(_T("MulWrite%u"), m_com);
    g_cs_com_section_read.Format(_T("Read%u"), m_com);
    return true;
}


OUTPUT bool open_com_nocretical(int m_com, int default_baudrate)
{
    //open com ,if you want to open "com1",the m_com equal 0;if you want to open "com2",the m_com equal 1
    //you will get the handle to com,m_hSerial,is a extern variable
    //the return value ,true is ok,false is failure
    //if(open_com_port_number_in_dll==m_com)
    //{
    //    Change_BaudRate(19200);
    //    return true;///////////////////////////same com port ,opened by multi times,it's badly.
    //}
    if(m_com_h_serial[m_com] != NULL)
    {
        //�رմ���
        CloseHandle(m_com_h_serial[m_com]);
        m_com_h_serial[m_com] = NULL;
    }
    /////////////////////////////////////////////////////////////////////����Ĵ���ͨ�Ų��� - Added serial communication part
// 	LPCSTR lpComNum[6];
// 	ZeroMemory(lpComNum, )

    CString strCom;
     if (m_com <=9 && m_com >0)
     {
         strCom.Format(_T("%d"),m_com);
         strCom = _T("COM")+strCom+_T(":");
     }
     else if(m_com > 9)
     {
        strCom.Format(_T("%d"),m_com);
        strCom = _T("\\\\.\\COM")+strCom;
    }

     m_hSerial = CreateFile(strCom, //strCom,//���ھ�����򿪴��� - Serial port handle, open serial port
         GENERIC_READ | GENERIC_WRITE,
         0,
         NULL,
         OPEN_EXISTING,
         FILE_FLAG_OVERLAPPED,//FILE_FLAG_OVERLAPPED,���������ʽ����ʾ�����첽ͨ�ţ���ͬʱ��д;0Ϊͬ����д - Another form, indicates asynchronous communication, can read and write simultaneously; 0 for synchronous read/write
         NULL);


    if(m_hSerial == INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hSerial);
        m_hSerial = NULL;
        m_com_h_serial[m_com] = NULL;
        return false;
    }
    m_com_h_serial[m_com] = m_hSerial;
    //if(!SetupComm(m_hSerial, 1024*32, 1024*9))
    if(!SetupComm(m_hSerial, 1024*64, 1024*32))
    {
        int abcd = GetLastError();
        CloseHandle(m_hSerial);
        m_hSerial = NULL;
        m_com_h_serial[m_com] = NULL;
        return false;
    }
    DCB  PortDCB;
    PortDCB.DCBlength = sizeof(DCB);
    // Ĭ�ϴ��ڲ��� - Default serial port parameters
    if(!GetCommState(m_hSerial, &PortDCB))
    {
        CloseHandle(m_hSerial);
        m_hSerial = NULL;
        m_com_h_serial[m_com] = NULL;
        return false;
    }
    //not to change the baudate
    PortDCB.BaudRate = default_baudrate; // baud//attention ,if it is wrong,can't write the com
    PortDCB.ByteSize = 8;     // Number of bits/byte, 4-8
    PortDCB.Parity = NOPARITY;
    PortDCB.StopBits = ONESTOPBIT;
    if (! SetCommState(m_hSerial, &PortDCB))
    {
        return false;
    }
    COMMTIMEOUTS CommTimeouts;
    GetCommTimeouts(m_hSerial, &CommTimeouts);
    CommTimeouts.ReadIntervalTimeout = 160;
    CommTimeouts.ReadTotalTimeoutMultiplier = 20;
    CommTimeouts.ReadTotalTimeoutConstant = 360;
    CommTimeouts.WriteTotalTimeoutMultiplier = 20;
    CommTimeouts.WriteTotalTimeoutConstant = 200;

//���� - Test
    //CommTimeouts.ReadIntervalTimeout = 0;
    //CommTimeouts.ReadTotalTimeoutMultiplier = 0;
    //CommTimeouts.ReadTotalTimeoutConstant = 0;
    //CommTimeouts.WriteTotalTimeoutMultiplier = 20;
    //CommTimeouts.WriteTotalTimeoutConstant = 200;


    if (!SetCommTimeouts(m_hSerial, &CommTimeouts))
    {
        return false;
    }
    //open_com_port_number_in_dll=m_com;
    //baudrate_in_dll=19200;

    return true;
}

OUTPUT int Read_One2(TS_UC device_var,TS_US address, bool bComm_Type)
{

//	CSingleLock singlock(&scan_mutex);
//	singlock.Lock();
    int nTemp=-1;
    if(bComm_Type==0)//serial
    {
        //address        the register
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        //return value == -3 ,no response
        //��մ��ڻ�����
        //TS_UC  gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];
        for(int i=0; i<11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
        pval[0] = device_var;
        pval[1] = 3;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        pval[4] = 0;//(val>>8) & 0xff;//number hi
        pval[5] = 1;//val & 0xff;//number lo
        crc = CRC16(pval,6);
        pval[6] = (crc >>8) & 0xff;
        pval[7] = crc & 0xff;
        if(m_hSerial==NULL)
        {
            return -1;
        }

        ////////////////////////////////////////////////////////////overlapped declare
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ///////////////////////////////////////////////////////send the to read message
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0 ;

        int fState=WriteFile(m_hSerial,// ��� - Handle
                             pval,// ���ݻ�������ַ - Data buffer address
                             8,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                             &m_osWrite);
        if(!fState)// I/Oδ��ɻ�ʧ�� - I/O incomplete or failed
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
                //			if(GetLastError()==ERROR_IO_PENDING)
                //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number=0;
        }
        //////////////////////////////////////////the message had send ,now to read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;

        ////////////////////////////////////////////////clear com error
        Sleep(LATENCY_TIME_COM);
        if(address==10)
        {
            serinumber_in_dll[0]=serinumber_in_dll[1]=serinumber_in_dll[2]=serinumber_in_dll[3]=0;//this line is for new protocal
            fState=ReadFile(m_hSerial,// ��� - handle
                            gval,// ���ݻ�������ַ - data buffer address
                            11,// ���ݴ�С - data size
                            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                            &m_osRead);
        }
        else
        {
            fState=ReadFile(m_hSerial,// ��� - handle
                            gval,// ���ݻ�������ַ - data buffer address
                            7,// ���ݴ�С - data size
                            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                            &m_osRead);
        }
        if(!fState)// ��֧���ص� - overlapping not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        if(address!=10)
        {
            //old protocal
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 )
                return -3;
            if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )
                return -2;
            crc=CRC16(gval,5);
            if(gval[5]!=((crc>>8)&0xff))
                return -2;
            if(gval[6]!=(crc & 0xff))
                return -2;
        }
        else
        {
            if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)
            {
                //new protocal
                if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=6 )//6
                    return -2;
                crc=CRC16(gval,9);
                if(gval[9]!=((crc>>8)&0xff))
                    return -2;
                if(gval[10]!=(crc & 0xff))
                    return -2;
                serinumber_in_dll[0]=gval[5];
                serinumber_in_dll[1]=gval[6];
                serinumber_in_dll[2]=gval[7];
                serinumber_in_dll[3]=gval[8];//stay serialnumber
                //			TRACE("R:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
            }
            else
            {
                //old protocal
                if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )//2
                    return -2;
                crc=CRC16(gval,5);
                if(gval[5]!=((crc>>8)&0xff))
                    return -2;
                if(gval[6]!=(crc & 0xff))
                    return -2;
            }
        }
        nTemp=gval[3];
        if(nTemp==255)
            nTemp=-1;
        return (nTemp*256+gval[4]);
    }
    if(bComm_Type==1)//tcp.
    {
        //address        the register
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        //return value == -3 ,no response
        //��մ��ڻ�����
        //TS_UC  gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];

        /*
        struct DataPack
        {
        	TS_UC  empty[6];
        	TS_UC  pval[13];

        };
        */
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        TS_UC data[12];

        data[0]= trans_high_id;
        data[1]= trans_low_id;
        data[2]=0;
        data[3]=0;
        data[4]=0;
        data[5]=6;
        data[6]=device_var;
        data[7]=3;
        data[8]=address>>8 & 0xFF ;
        data[9] = address & 0xFF;
        data[10]=0;
        data[11]=1;

//		DataPack dataInfo;
        for(int i=0; i<11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
//		TS_US crc;
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - number of bytes sent

        pval[0] = device_var;
        pval[1] = 3;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        pval[4] = 0;//(val>>8) & 0xff;//number hi
        pval[5] = 1;//val & 0xff;//number lo
        //	crc = CRC16(pval,6);
        //	pval[6] = (crc >>8) & 0xff;
        //	pval[7] = crc & 0xff;

        //memcpy((char*)&data[6],(char*)&pval[0],sizeof(pval));

        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        int nLen = sizeof(data);
        //int xx=::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        int xx=::send(m_hSocket,(char*)&data,sizeof(data),0);


        ////////////////////////////////////////////////clear com error
        if(address==10)
        {
            serinumber_in_dll[0]=serinumber_in_dll[1]=serinumber_in_dll[2]=serinumber_in_dll[3]=0;//this line is for new protocal
        }

        Sleep(LATENCY_TIME_NET);//Sleep(SLEEP_TIME);
        //DataPack rvData;
        //Sleep(SLEEP_TIME+10);

        TS_UC rvData[17];
        //int nRecv = ::recv(m_hSocket, (char*)gval, sizeof(gval), 0);
        int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);

        if (nRecv < 0)
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ� - Error code, the remote host forcibly closed an existing connection.
            {
                if (last_connected_port != 0)
                {
                    Open_Socket2(last_connected_ip, last_connected_port);
                    return -1;
                }
            }
        }
        //gval=rvData.pval;
        memcpy((void*)gval,(void*)&rvData[6],13/*sizeof(gval)*/);

        /*
        xx=::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        nRecv = ::recv(m_hSocket, (char*)gval, sizeof(gval), 0);*/

        int nerro= WSAGetLastError();

        if(address!=10)
        {
            //old protocal
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0/* && gval[5]==0 && gval[6]==0*/ )
                return -3;
            if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )
                return -2;

            /*;
            crc=CRC16(gval,5);
            if(gval[5]!=((crc>>8)&0xff))
            	return -2;
            if(gval[6]!=(crc & 0xff))
            	return -2;
            */

        }
        else
        {
            if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)
            {
                //new protocal
                if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=6 )//6
                    return -2;
                /*
                crc=CRC16(gval,9);
                if(gval[9]!=((crc>>8)&0xff))
                	return -2;
                if(gval[10]!=(crc & 0xff))
                	return -2;
                	*/
                serinumber_in_dll[0]=gval[5];
                serinumber_in_dll[1]=gval[6];
                serinumber_in_dll[2]=gval[7];
                serinumber_in_dll[3]=gval[8];//stay serialnumber
                //			TRACE("R:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
            }
            else
            {
                //old protocal
                if(gval[0]!=pval[0] || gval[1]!=gval[1] || gval[2]!=2 )//2
                    return -2;
                /*
                crc=CRC16(gval,5);
                if(gval[5]!=((crc>>8)&0xff))
                	return -2;
                if(gval[6]!=(crc & 0xff))
                	return -2;
                	*/
            }
        }
        nTemp=gval[3];
        if(nTemp==255)
            nTemp=-1;
        return (gval[3]*256+gval[4]);
    }
    return -1;
//	singlock.Unlock();
}

OUTPUT int Write_One2(TS_UC device_var,TS_US address,TS_US val, bool bComm_Type)
{
    if (bComm_Type==0)
    {
        //address        the register
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -3 , no response
        //��մ��ڻ����� - Clear serial port buffer

        //		gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];
        for(int i=0; i<=11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes of data already sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
            pval[4]=0x55;/////////////////////////////new protocal or write_one 10
        else
            pval[4] = (val>>8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo
        if(address!=10)
        {
            crc = CRC16(pval,6);
            pval[6] = (crc >>8) & 0xff;
            pval[7] = crc & 0xff;
        }
        else
        {
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                crc = CRC16(pval,6);
                pval[6] = (crc >>8) & 0xff;
                pval[7] = crc & 0xff;
            }
            else
            {
                pval[6]=serinumber_in_dll[0];
                pval[7]=serinumber_in_dll[1];
                pval[8]=serinumber_in_dll[2];
                pval[9]=serinumber_in_dll[3];
                crc = CRC16(pval,10);
                pval[10] = (crc >>8) & 0xff;
                pval[11] = crc & 0xff;
            }
        }

        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        int fState;
        Sleep(50);
        if(address!=10)
        {
            fState=WriteFile(m_hSerial,// ��� - handle
                             pval,// ���ݻ�������ַ - data buffer address
                             8,// ���ݴ�С - data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                             &m_osWrite);
        }
        else
        {
            //==10 register
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                fState=WriteFile(m_hSerial,// ��� - handle
                                 pval,// ���ݻ�������ַ - data buffer address
                                 8,// ���ݴ�С - data size
                                 &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                                 &m_osWrite);
            }
            else
            {
                //new protocal
                fState=WriteFile(m_hSerial,// ��� - handle
                                 pval,// ���ݻ�������ַ - data buffer address
                                 12,// ���ݴ�С - data size
                                 &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                                 &m_osWrite);
            }
        }
        if(!fState)// ��֧���ص� - overlapping not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
                //			if(GetLastError()==ERROR_IO_INCOMPLETE)
                //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        Sleep(LATENCY_TIME_COM);
        if(address!=10)
        {
            fState=ReadFile(m_hSerial,// ��� - handle
                            gval,// ���ݻ�������ַ - data buffer address
                            8,// ���ݴ�С - data size
                            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                            &m_osRead);
        }
        else
        {
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                fState=ReadFile(m_hSerial,// ��� - handle
                                gval,// ���ݻ�������ַ - data buffer address
                                8,// ���ݴ�С - data size
                                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                                &m_osRead);
            }
            else
            {
                //new protocal
                fState=ReadFile(m_hSerial,// ��� - handle
                                gval,// ���ݻ�������ַ - data buffer address
                                12,// ���ݴ�С - data size
                                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                                &m_osRead);
            }
        }
        if(!fState)// ��֧���ص� - overlapping not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        if(address!=10)
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0)
                return -3;
            for(int i=0; i<8; i++)
                if(gval[i]!=pval[i])
                    return -2;
        }
        else
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0)
                return -3;
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                for(int i=0; i<8; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
            else
            {
                //new protocal
                //			crc=CRC16(gval,10);
                //			if(gval[6]!=((crc>>8)&0xff))
                //				return -2;
                //			if(gval[7]!=(crc & 0xff))
                //				return -2;
                //			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
                if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0)
                    return -3;
                for(int i=0; i<12; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
        }
        return 1;
    }

    if(bComm_Type==1)//tcp.
    {
        //address        the register
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -3 , no response
        //��մ��ڻ����� - Clear serial port buffer

        //TS_UC data[12];
        TS_UC data[16];
// 		TS_UC* data=NULL;
        int nSendNum = 12;
        if (address==10)
        {
            //data = new TS_UC[16];
            nSendNum = 16;
        }
        else
        {
            //data = new TS_UC[12];
            nSendNum = 12;
        }
        ZeroMemory(data, nSendNum);
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        data[0]= trans_high_id;
        data[1]= trans_low_id;
        data[2]=0;
        data[3]=0;
        data[4]=0;
        data[5]=6;
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - number of bytes sent
        data[6] = device_var;
        data[7] = 6;
        data[8] = address>>8 & 0xFF ;
        data[9] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
            data[10]=0x55;/////////////////////////////new protocal or write_one 10
        else
            data[10] = (val>>8) & 0xff;//number hi
        data[11] = val & 0xff;//number lo

        if(address==10)
        {

            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
            }
            else
            {
                //* uncommend by zgq; we use net work scan tstat through NC
                data[12]=serinumber_in_dll[0];
                data[13]=serinumber_in_dll[1];
                data[14]=serinumber_in_dll[2];
                data[15]=serinumber_in_dll[3];
                //*/
            }
        }


        for(int i=0; i<=11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
//		TS_US crc;
        //	DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
            pval[4]=0x55;/////////////////////////////new protocal or write_one 10
        else
            pval[4] = (val>>8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo

        /*
        for(int i=0;i<=11;i++)
        	gval[i]=0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
        	pval[4]=0x55;/////////////////////////////new protocal or write_one 10
        else
        	pval[4] = (val>>8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo
        if(address!=10)
        {
        	crc = CRC16(pval,6);
        	pval[6] = (crc >>8) & 0xff;
        	pval[7] = crc & 0xff;
        }
        else
        {
        	if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
        	{
        		crc = CRC16(pval,6);
        		pval[6] = (crc >>8) & 0xff;
        		pval[7] = crc & 0xff;
        	}
        	else
        	{
        		pval[6]=serinumber_in_dll[0];
        		pval[7]=serinumber_in_dll[1];
        		pval[8]=serinumber_in_dll[2];
        		pval[9]=serinumber_in_dll[3];
        		crc = CRC16(pval,10);
        		pval[10] = (crc >>8) & 0xff;
        		pval[11] = crc & 0xff;
        	}
        }
        */
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        //::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        //::send(m_hSocket,(char*)data,sizeof(data),0);
        int nRet = ::send(m_hSocket,(char*)data,nSendNum,0);

        //Sleep(SLEEP_TIME+10);
        Sleep(LATENCY_TIME_NET+100);
        //Sleep(300);
        TS_UC rvdata[17];
        int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);
        if (nRecv < 0)
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ�- The remote host forcibly closed an existing connection.
            {
                if (last_connected_port != 0)
                {
                    Open_Socket2(last_connected_ip, last_connected_port);
                    return -1;
                }
            }
        }
        int nErr = WSAGetLastError();

        memcpy((void*)&gval[0],(void*)&rvdata[6],13/*sizeof(gval)*/);
        if(address!=10)
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 /*&& gval[6]==0 && gval[7]==0*/)
                return -3;
            for(int i=0; i<5; i++)
                if(gval[i]!=pval[i])
                    return -2;
        }
        else
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0/* && gval[6]==0 && gval[7]==0*/)
                return -3;
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                for(int i=0; i<5; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
            else
            {
                //new protocal
                //			crc=CRC16(gval,10);
                //			if(gval[6]!=((crc>>8)&0xff))
                //				return -2;
                //			if(gval[7]!=(crc & 0xff))
                //				return -2;
                //			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
                if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 /*&& gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0*/)
                    return -3;
                for(int i=0; i<5; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
        }

        //delete []data;
        return 1;
    }
    return -1;
    ///////////////////////////////////////////////////////////
}

OUTPUT int NetController_CheckTstatOnline_a(TS_UC devLo,TS_UC devHi, bool bComm_Type)
{
    if(bComm_Type==0)
    {
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ����� - Clear serial port buffer
        //the return value is the register address

        //Sleep(150);       //must use this function to slow computer
        if(devLo<1 || devHi>254)
            return -5;
        if(m_hSerial==NULL)
        {
            return -1;
        }
        int the_return_value;
        int the_return_value2=0;
        the_return_value=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
        //down is inspect result first scan
        if(the_return_value==-4)
        {
            the_return_value=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
            return the_return_value;
        }
        if(old_or_new_scan_protocal_in_dll==1)
        {
            //new protocal
            if(the_return_value>0)
            {
                the_return_value2=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
                if(the_return_value2!=-4)
                    the_return_value=the_return_value2;
            }
            return the_return_value;
        }
        else if(old_or_new_scan_protocal_in_dll==2)
        {
            //old protocal
            if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
                return the_return_value;
            int i=0;
            do
            {
                the_return_value=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
                if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
                    return the_return_value;
                else if(the_return_value>0)
                    i++;
            }
            while(i<3);
            return the_return_value;
        }
        //	if(the_return_value>0)
        //		TRACE("^-^ ^-^ %d\n",the_return_value);
        return the_return_value;
    }

    if(bComm_Type==1)//tcp
    {
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ�����
        //the return value is the register address
        //Sleep(50);       //must use this function to slow computer
        if(devLo<1 || devHi>254)
            return -5;
        if (m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }
        int the_return_value;
        int the_return_value2=0;
        the_return_value=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
        //down is inspect result first scan
        if(the_return_value==-4)
        {
            the_return_value=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
            return the_return_value;
        }
        if(old_or_new_scan_protocal_in_dll==1)
        {
            //new protocal
            if(the_return_value>0)
            {
                the_return_value2=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
                if(the_return_value2!=-4)
                    the_return_value=the_return_value2;
            }
            return the_return_value;
        }
        else if(old_or_new_scan_protocal_in_dll==2)
        {
            //old protocal
            if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
                return the_return_value;
            int i=0;
            do
            {
                the_return_value=NetController_CheckTstatOnline2_a(devLo,devHi, bComm_Type);
                if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
                    return the_return_value;
                else if(the_return_value>0)
                    i++;
            }
            while(i<3);
            return the_return_value;
        }
        //	if(the_return_value>0)
        //		TRACE("^-^ ^-^ %d\n",the_return_value);
        return the_return_value;
    }

    return -1;
}

OUTPUT int NetController_CheckTstatOnline2_a(TS_UC devLo,TS_UC devHi, bool bComm_Type)
{
    CString strlog;
    if(bComm_Type==0)
    {
        //the second time
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ����� - Clear serial port buffer
        //the return value is the register address
        strlog.Format(_T("Scan From ID=%d To ID=%d"),devLo,devHi);
        //WriteLogFile(strlog);
        if(devLo<1 || devHi>254)
            return -5;
        //the input inspect
        for(int i=0; i<13; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_UC  pval[6];
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - number of bytes sent
        pval[0] = 255;
        pval[1] = 26;  //put comments here,
        pval[2] = devHi;
        pval[3] = devLo;
        crc = CRC16(pval,4);
        pval[4] = (crc >>8) & 0xff;
        pval[5] = crc & 0xff;

        if(m_hSerial==NULL)
        {
            return -1;
        }

        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear buffer
        int fState=WriteFile(m_hSerial,// ��� - handle
                             pval,// ���ݻ�������ַ - data buffer address
                             6,// ���ݴ�С - data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                             &m_osWrite);
        if(!fState)//��֧���ص� - overlapping not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
                //			if(GetLastError()==ERROR_IO_INCOMPLETE)
                //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number=0;
        }
        //	TRACE("%d T:%x %x %x %x %x %x\n",ddd,pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
        //CloseHandle(m_osWrite.hEvent);
        ///////////////////////////up is write
        Sleep(LATENCY_TIME_COM);//SLEEP_TIME);//because that scan have a delay lower 75ms
        //	Sleep(300);
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0;
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - handle
                        gval,// ���ݻ�������ַ - data buffer address
                        13,// ���ݴ�С - data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - overlapping not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }


        CString strCom;
        strCom.Format(_T("Com=%d : "), open_com_port_number_in_dll);

        //WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
        CString filelog;
        filelog=_T("Send a scan command to any devices: ");
        //SaveBufferToLogFile(pval, 6);


        for (int i = 0; i < 6; i++)
        {
            int nValue = pval[i];
            CString strValue;
            strValue.Format(_T("%0x, "), nValue);
            //g_fileScanLog->WriteString(strValue);
            filelog+=strValue;
        }
        //WriteLogFile(filelog);
        filelog.Empty();
        filelog=_T("Recv Data : ");
        for (int i = 0; i < 13; i++)
        {
            int nValue = gval[i];
            CString strValue;
            strValue.Format(_T("%0x, "), nValue);
            //g_fileScanLog->WriteString(strValue);
            filelog+=strValue;
        }
        //WriteLogFile(filelog);
        int index=filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
        if(index==-1)
        {
            
        }
        else
        {
            
        }
        //SaveBufferToLogFile(gval, 13);









        if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
        {
            //old scan protocal
            old_or_new_scan_protocal_in_dll=2;
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -4;              //no response ,no connection
            }
            if(gval[5]!=0 || gval[6]!=0)//to inspect
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -3;
            }
            if((gval[0]!=pval[0]) || (gval[1]!=26))
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
            crc=CRC16(gval,3);
            if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
        }
        else
        {
            // new scan protocal,if many old tstat ,get into here ,scan result is oK too.
            old_or_new_scan_protocal_in_dll=1;
            Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
            if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect
                return -3;
            if((gval[0]!=pval[0]) || (gval[1]!=25))
                return -2;
            crc=CRC16(gval,7);
            if( gval[7]!=((crc>>8) & 0xff) )
                return -2;
            if(gval[8]!=(crc & 0xff))
                return -2;
        }
        //here is different with CheckTstatOnline() function
        //	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
        //	TRACE("%d ^-^ ^-^ %d^\n",ddd,gval[2]);
        /*
        if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
        {
        default_file.SeekToEnd();
        a_line.Format("exist:%d",gval[2]);
        default_file.WriteString(a_line+"\n");
        default_file.Flush();
        default_file.Close();
        }*/

        return gval[2];
    }

    if(bComm_Type==1)
    {
        //the second time
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ����� - Clear serial port buffer
        //the return value is the register address
        strlog.Format(_T("Scan From ID=%d To ID=%d"),devLo,devHi);
        //NET_WriteLogFile(strlog);
        if(devLo<1 || devHi>254)
            return -5;
        //the input inspect

        for(int i=0; i<13; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_UC  pval[10];
//		TS_US crc;
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - number of bytes sent


        pval[0]=1;
        pval[1]=2;
        pval[2]=3;
        pval[3]=4;
        pval[4]=5;
        pval[5]=6;
        pval[6] = 255;
        pval[7] = 26;
        pval[8] = devHi;
        pval[9] = devLo;
        /*
        crc = CRC16(pval,4);
        pval[4] = (crc >>8) & 0xff;
        pval[5] = crc & 0xff;
        */
        if (m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }
        ::send(m_hSocket,(char*)pval,sizeof(pval),0);
        Sleep(LATENCY_TIME_NET);//Sleep(SLEEP_TIME+8);

        TS_UC  rvdata[19];
        for(int i=0; i<19; i++)
            rvdata[i]=0;
        int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);
        if (nRecv>0)
        {
            //�쳣:
            memcpy(gval,(void*)&rvdata[6],sizeof(rvdata)-6);
        }


        //NET_WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
        CString filelog;
        filelog=_T("Send a scan command to any devices: ");

        for (int i = 0; i < 10; i++)
        {
            int nValue = pval[i];
            CString strValue;
            strValue.Format(_T("%0X, "), nValue);
            //g_fileScanLog->WriteString(strValue);
            filelog+=strValue;
        }
        //NET_WriteLogFile(filelog);
        filelog.Empty();
        //NET_WriteLogFile(_T("Recv Data : "));
        for (int i = 0; i < 19; i++)
        {
            int nValue = rvdata[i];
            CString strValue;
            strValue.Format(_T("%0X, "), nValue);
            //g_fileScanLog->WriteString(strValue);
            filelog+=strValue;
        }

        //NET_WriteLogFile(filelog);

        int index=filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
        if(index==-1)
        {
            //NET_WriteLogFile(_T(">>More than one device answer......"));
        }
        else
        {
            //NET_WriteLogFile(_T(">>No one device answer......"));
        }

        if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
        {
            //old scan protocal
            old_or_new_scan_protocal_in_dll=2;
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
            {
                Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
                return -4;              //no response ,no connection
            }
            if(gval[5]!=0 || gval[6]!=0)//to inspect
            {
                Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
                return -3;
            }
            if((gval[0]!=255) || (gval[1]!=26))
            {
                //		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
        }
        else
        {
            // new scan protocal,if many old tstat ,get into here ,scan result is oK too.
            old_or_new_scan_protocal_in_dll=1;
            //	Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
            if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect
                return -3;
            if((gval[0]!=255) || (gval[1]!=26))
                return -2;
        }

        /*
        old_or_new_scan_protocal_in_dll=1;

        if((gval[0]!=pval[6]) || (gval[1]!=26))
        	return -2;
        if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0)
        {
        	//	Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
        	return -4;              //no response ,no connection
        }
        */
        return gval[2];
    }
    return -1;
}



OUTPUT int read_multi2(TS_UC device_var, TS_US *put_data_into_here, TS_US start_address, int length, bool bComm_Type)
{
    if (bComm_Type == 0)
    {
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting

        TS_UC to_send_data[300] = { '\0' };
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL, IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        TS_UC data_to_send[8] = { '\0' }; //the array to used in writefile()
        data_to_send[0] = device_var;//slave address
        data_to_send[1] = 3;//function multiple
        data_to_send[2] = start_address >> 8 & 0xff;//starting address hi
        data_to_send[3] = start_address & 0xff;//starting address lo
        data_to_send[4] = length >> 8 & 0xff;//quantity of registers hi
        data_to_send[5] = length & 0xff;//quantity of registers lo
        TS_US crc = CRC16(data_to_send, 6);
        data_to_send[6] = (crc >> 8) & 0xff;
        data_to_send[7] = crc & 0xff;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes of data already sent
        if (m_hSerial == NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
                                                                                            ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
        if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, g_cs_com_section_mulwrite)) == NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0;
        ///////////////////////////////////////////////////////send the to read message
        int fState = WriteFile(m_hSerial,// ��� - Handle
            data_to_send,// ���ݻ�������ַ - Data buffer address
            8,// ���ݴ�С - Data size
            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
            &m_osMulWrite);
        if (!fState)// ��֧���ص� - Does not support overlap
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ� - Wait
            }
            else
                m_had_send_data_number = 0;
        }
        ///////////////////////////up is write
        /////////////**************down is read

        Sleep(LATENCY_TIME_COM);
        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if ((m_osRead.hEvent = CreateEvent(NULL, true, false, g_cs_com_section_read)) == NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0;
        ////////////////////////////////////////////////clear com error
        fState = ReadFile(m_hSerial,// ��� - Handle
            to_send_data,// ���ݻ�������ַ - Data buffer address
            length * 2 + 5,// ���ݴ�С - Data size
            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
            &m_osRead);
        if (!fState)// ��֧���ص� - Does not support overlap
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ� - Wait
            }
            else
                m_had_send_data_number = 0;
        }
        ///////////////////////////////////////////////////////////
        if (to_send_data[0] != device_var || to_send_data[1] != 3 || to_send_data[2] != length * 2)
            return -2;
        crc = CRC16(to_send_data, length * 2 + 3);
        if (to_send_data[length * 2 + 3] != ((crc >> 8) & 0xff))
            return -2;
        if (to_send_data[length * 2 + 4] != (crc & 0xff))
            return -2;
        for (int i = 0; i<length; i++)
            put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];
        return length;
    }
    if (bComm_Type == 1)//tcp.
    {
        /*
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting
        TS_UC to_send_data[700]={'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        TS_UC data_to_send[8]={'\0'};//the array to used in writefile()
        data_to_send[0]=device_var;//slave address
        data_to_send[1]=3;//function multiple
        data_to_send[2]=start_address>>8 & 0xff;//starting address hi
        data_to_send[3]=start_address & 0xff;//starting address lo
        data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[5]=length & 0xff;//quantity of registers lo
        TS_US crc=CRC16(data_to_send,6);
        data_to_send[6]=(crc>>8) & 0xff;
        data_to_send[7]=crc & 0xff;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes of data already sent
        if(m_hSocket==INVALID_SOCKET)
        {
        return -1;
        }
        ::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),MSG_OOB);

        int nn=sizeof(to_send_data);
        int nRecv = ::recv(m_hSocket, (char*)to_send_data, length*2+5, 0);

        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
        return -2;
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
        return -2;
        if(to_send_data[length*2+4]!=(crc & 0xff))
        return -2;
        for(int i=0;i<length;i++)
        put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
        return length;
        */
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting

        TS_UC to_send_data[700] = { '\0' };
        TS_UC to_Reive_data[700] = { '\0' };
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL, IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        TS_UC data_to_send[12] = { '\0' }; //the array to used in writefile()
        data_to_send[0] = trans_high_id;
        data_to_send[1] = trans_low_id;
        data_to_send[2] = 0;
        data_to_send[3] = 0;
        data_to_send[4] = 0;
        data_to_send[5] = 6;



        data_to_send[6] = device_var;//slave address
        data_to_send[7] = 3;//function multiple
        data_to_send[8] = start_address >> 8 & 0xff;//starting address hi
        data_to_send[9] = start_address & 0xff;//starting address lo
        data_to_send[10] = length >> 8 & 0xff;//quantity of registers hi
        data_to_send[11] = length & 0xff;//quantity of registers lo

                                         //TS_US crc=CRC16(data_to_send,6);
                                         //data_to_send[6]=(crc>>8) & 0xff;
                                         //data_to_send[7]=crc & 0xff;/

                                         //		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
        if (m_hSocket == INVALID_SOCKET)
        {
            return -1;
        }

        ::send(m_hSocket, (char*)data_to_send, sizeof(data_to_send), 0);

        Sleep(LATENCY_TIME_NET);
        int nn = sizeof(to_Reive_data);
        int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length * 2 + 12, 0);

        memcpy((void*)&to_send_data[0], (void*)&to_Reive_data[6], sizeof(to_Reive_data));
        ///////////////////////////////////////////////////////////
        if (to_send_data[0] != device_var || to_send_data[1] != 3 || to_send_data[2] != length * 2)
            return -2;
        /*
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
        return -2;

        if(to_send_data[length*2+4]!=(crc & 0xff))
        return -2;
        */
        for (int i = 0; i<length; i++)
            put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];
        return length;
    }
    return -1;
}


OUTPUT int Write_One2_nocretical(TS_UC device_var, TS_US address, TS_US val, bool bComm_Type, int ncomport)
{
    if (bComm_Type == 0)
    {
        HANDLE m_hSerial = NULL;
        m_hSerial = m_com_h_serial[ncomport];
        //address        the register
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -3 , no response
        //��մ��ڻ�����

        //		gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];
        for (int i = 0; i <= 11; i++)
            gval[i] = 0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes of data already sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address >> 8 & 0xFF;
        pval[3] = address & 0xFF;
        if (address == 10 && (serinumber_in_dll[0] != 0 || serinumber_in_dll[1] != 0 || serinumber_in_dll[2] != 0 || serinumber_in_dll[3] != 0))
            pval[4] = 0x55;/////////////////////////////new protocal or write_one 10
        else
            pval[4] = (val >> 8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo
        if (address != 10)
        {
            crc = CRC16(pval, 6);
            pval[6] = (crc >> 8) & 0xff;
            pval[7] = crc & 0xff;
        }
        else
        {
            if (serinumber_in_dll[0] == 0 && serinumber_in_dll[1] == 0 && serinumber_in_dll[2] == 0 && serinumber_in_dll[3] == 0)
            {
                crc = CRC16(pval, 6);
                pval[6] = (crc >> 8) & 0xff;
                pval[7] = crc & 0xff;
            }
            else
            {
                pval[6] = serinumber_in_dll[0];
                pval[7] = serinumber_in_dll[1];
                pval[8] = serinumber_in_dll[2];
                pval[9] = serinumber_in_dll[3];
                crc = CRC16(pval, 10);
                pval[10] = (crc >> 8) & 0xff;
                pval[11] = crc & 0xff;
            }
        }

        if (m_hSerial == NULL)
        {
            return -1;
        }
        CString nSection;
        nSection.Format(_T("ComMulWrite_%d"), ncomport);
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_com_osWrite[ncomport], 0, sizeof(OVERLAPPED));
        if ((m_com_osWrite[ncomport].hEvent = CreateEvent(NULL, true, false, nSection)) == NULL)
            return -2;
        m_com_osWrite[ncomport].Offset = 0;
        m_com_osWrite[ncomport].OffsetHigh = 0;
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
        int fState;
        Sleep(50);
        if (address != 10)
        {
            fState = WriteFile(m_hSerial,// ��� - Handle
                pval,// ���ݻ�������ַ - Data buffer address
                8,// ���ݴ�С - Data size
                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                &m_com_osWrite[ncomport]);
        }
        else
        {
            //==10 register
            if (serinumber_in_dll[0] == 0 && serinumber_in_dll[1] == 0 && serinumber_in_dll[2] == 0 && serinumber_in_dll[3] == 0)
            {
                //old protocal
                fState = WriteFile(m_hSerial,// ��� - Handle
                    pval,// ���ݻ�������ַ - Data buffer address
                    8,// ���ݴ�С - Data size
                    &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                    &m_com_osWrite[ncomport]);
            }
            else
            {
                //new protocal
                fState = WriteFile(m_hSerial,// ��� - Handle
                    pval,// ���ݻ�������ַ - Data buffer address
                    12,// ���ݴ�С - Data size
                    &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                    &m_com_osWrite[ncomport]);
            }
        }
        if (!fState)// ��֧���ص� - Does not support overlap
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_com_osWrite[ncomport], &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
                                                                                                   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
                                                                                                   //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number = 0;
        }
        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        memset(&m_com_osWrite[ncomport], 0, sizeof(OVERLAPPED));
        if ((m_com_osWrite[ncomport].hEvent = CreateEvent(NULL, true, false, nSection)) == NULL)
            return -2;
        m_com_osWrite[ncomport].Offset = 0;
        m_com_osWrite[ncomport].OffsetHigh = 0;
        ////////////////////////////////////////////////clear com error
        Sleep(LATENCY_TIME_COM);
        if (address != 10)
        {
            fState = ReadFile(m_hSerial,// ��� - Handle
                gval,// ���ݻ�������ַ - Data buffer address
                8,// ���ݴ�С - Data size
                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                &m_com_osWrite[ncomport]);
        }
        else
        {
            if (serinumber_in_dll[0] == 0 && serinumber_in_dll[1] == 0 && serinumber_in_dll[2] == 0 && serinumber_in_dll[3] == 0)
            {
                //old protocal
                fState = ReadFile(m_hSerial,// ��� - Handle
                    gval,// ���ݻ�������ַ - Data buffer address
                    8,// ���ݴ�С - Data size
                    &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                    &m_com_osWrite[ncomport]);
            }
            else
            {
                //new protocal
                fState = ReadFile(m_hSerial,// ��� - Handle
                    gval,// ���ݻ�������ַ - Data buffer address
                    12,// ���ݴ�С - Data size
                    &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                    &m_com_osWrite[ncomport]);
            }
        }
        if (!fState)// ��֧���ص� - Does not support overlap
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_com_osWrite[ncomport], &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number = 0;
        }
        if (address != 10)
        {
            if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 && gval[6] == 0 && gval[7] == 0)
                return -3;
            for (int i = 0; i<8; i++)
                if (gval[i] != pval[i])
                    return -2;
        }
        else
        {
            if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 && gval[6] == 0 && gval[7] == 0)
                return -3;
            if (serinumber_in_dll[0] == 0 && serinumber_in_dll[1] == 0 && serinumber_in_dll[2] == 0 && serinumber_in_dll[3] == 0)
            {
                //old protocal
                for (int i = 0; i<8; i++)
                    if (gval[i] != pval[i])
                        return -2;
            }
            else
            {
                //new protocal
                //			crc=CRC16(gval,10);
                //			if(gval[6]!=((crc>>8)&0xff))
                //				return -2;
                //			if(gval[7]!=(crc & 0xff))
                //				return -2;
                //			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
                if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 && gval[6] == 0 && gval[7] == 0 && gval[8] == 0 && gval[9] == 0 && gval[10] == 0 && gval[11] == 0)
                    return -3;
                for (int i = 0; i<12; i++)
                    if (gval[i] != pval[i])
                        return -2;
            }
        }
        return 1;
    }

    if (bComm_Type == 1)//tcp.
    {
        //address        the register
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -3 , no response
        //��մ��ڻ�����

        //TS_UC data[12];
        TS_UC data[16];
        // 		TS_UC* data=NULL;
        int nSendNum = 12;
        if (address == 10)
        {
            //data = new TS_UC[16];
            nSendNum = 16;
        }
        else
        {
            //data = new TS_UC[12];
            nSendNum = 12;
        }
        ZeroMemory(data, nSendNum);

        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        data[0] = trans_high_id;
        data[1] = trans_low_id;
        data[2] = 0;
        data[3] = 0;
        data[4] = 0;
        data[5] = 6;

        //		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        data[6] = device_var;
        data[7] = 6;
        data[8] = address >> 8 & 0xFF;
        data[9] = address & 0xFF;
        if (address == 10 && (serinumber_in_dll[0] != 0 || serinumber_in_dll[1] != 0 || serinumber_in_dll[2] != 0 || serinumber_in_dll[3] != 0))
            data[10] = 0x55;/////////////////////////////new protocal or write_one 10
        else
            data[10] = (val >> 8) & 0xff;//number hi
        data[11] = val & 0xff;//number lo

        if (address == 10)
        {

            if (serinumber_in_dll[0] == 0 && serinumber_in_dll[1] == 0 && serinumber_in_dll[2] == 0 && serinumber_in_dll[3] == 0)
            {
            }
            else
            {
                //* uncommend by zgq; we use net work scan tstat through NC
                data[12] = serinumber_in_dll[0];
                data[13] = serinumber_in_dll[1];
                data[14] = serinumber_in_dll[2];
                data[15] = serinumber_in_dll[3];
                //*/
            }
        }


        for (int i = 0; i <= 11; i++)
            gval[i] = 0;/////////////////////////////////////////clear buffer
                        //		TS_US crc;
                        //	DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address >> 8 & 0xFF;
        pval[3] = address & 0xFF;
        if (address == 10 && (serinumber_in_dll[0] != 0 || serinumber_in_dll[1] != 0 || serinumber_in_dll[2] != 0 || serinumber_in_dll[3] != 0))
            pval[4] = 0x55;/////////////////////////////new protocal or write_one 10
        else
            pval[4] = (val >> 8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo

                             /*
                             for(int i=0;i<=11;i++)
                             gval[i]=0;/////////////////////////////////////////clear buffer
                             TS_US crc;
                             DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
                             pval[0] = device_var;
                             pval[1] = 6;
                             pval[2] = address>>8 & 0xFF ;
                             pval[3] = address & 0xFF;
                             if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
                             pval[4]=0x55;/////////////////////////////new protocal or write_one 10
                             else
                             pval[4] = (val>>8) & 0xff;//number hi
                             pval[5] = val & 0xff;//number lo
                             if(address!=10)
                             {
                             crc = CRC16(pval,6);
                             pval[6] = (crc >>8) & 0xff;
                             pval[7] = crc & 0xff;
                             }
                             else
                             {
                             if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
                             {
                             crc = CRC16(pval,6);
                             pval[6] = (crc >>8) & 0xff;
                             pval[7] = crc & 0xff;
                             }
                             else
                             {
                             pval[6]=serinumber_in_dll[0];
                             pval[7]=serinumber_in_dll[1];
                             pval[8]=serinumber_in_dll[2];
                             pval[9]=serinumber_in_dll[3];
                             crc = CRC16(pval,10);
                             pval[10] = (crc >>8) & 0xff;
                             pval[11] = crc & 0xff;
                             }
                             }
                             */
        if (m_hSocket == INVALID_SOCKET)
        {
            return -1;
        }

        //::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        //::send(m_hSocket,(char*)data,sizeof(data),0);
        int nRet = ::send(m_hSocket, (char*)data, nSendNum, 0);

        //Sleep(SLEEP_TIME+10);
        Sleep(LATENCY_TIME_NET + 100);
        //Sleep(300);
        TS_UC rvdata[17];
        int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);
        if (nRecv < 0)
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ�- The remote host forcibly closed an existing connection.
            {
                if (last_connected_port != 0)
                {
                    Open_Socket2(last_connected_ip, last_connected_port);
                    return -1;
                }
            }
        }
        int nErr = WSAGetLastError();

        memcpy((void*)&gval[0], (void*)&rvdata[6], 13/*sizeof(gval)*/);
        if (address != 10)
        {
            if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 /*&& gval[6]==0 && gval[7]==0*/)
                return -3;
            for (int i = 0; i<5; i++)
                if (gval[i] != pval[i])
                    return -2;
        }
        else
        {
            if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0/* && gval[6]==0 && gval[7]==0*/)
                return -3;
            if (serinumber_in_dll[0] == 0 && serinumber_in_dll[1] == 0 && serinumber_in_dll[2] == 0 && serinumber_in_dll[3] == 0)
            {
                //old protocal
                for (int i = 0; i<5; i++)
                    if (gval[i] != pval[i])
                        return -2;
            }
            else
            {
                //new protocal
                //			crc=CRC16(gval,10);
                //			if(gval[6]!=((crc>>8)&0xff))
                //				return -2;
                //			if(gval[7]!=(crc & 0xff))
                //				return -2;
                //			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
                if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 /*&& gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0*/)
                    return -3;
                for (int i = 0; i<5; i++)
                    if (gval[i] != pval[i])
                        return -2;
            }
        }

        //delete []data;
        return 1;
    }
    return -1;
    ///////////////////////////////////////////////////////////
}



OUTPUT int read_multi2_nocretical(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,int length,bool bComm_Type, int ncomport)
{
    if(bComm_Type==0)
    {
        HANDLE m_hSerial = NULL;
        m_hSerial = m_com_h_serial[ncomport];
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting

        TS_UC to_send_data[300]= {'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        TS_UC data_to_send[8]= {'\0'}; //the array to used in writefile()
        data_to_send[0]=device_var;//slave address
        data_to_send[1]=3;//function multiple
        data_to_send[2]=start_address>>8 & 0xff;//starting address hi
        data_to_send[3]=start_address & 0xff;//starting address lo
        data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[5]=length & 0xff;//quantity of registers lo
        TS_US crc=CRC16(data_to_send,6);
        data_to_send[6]=(crc>>8) & 0xff;
        data_to_send[7]=crc & 0xff;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ////////////////////////////////////////////////////////////overlapped declare
        CString nSection;
        nSection.Format(_T("MulWrite_%d"),ncomport);

        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
        if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false, nSection))==NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0 ;
        ///////////////////////////////////////////////////////send the to read message
        int fState=WriteFile(m_hSerial,// ��� - Handle
                             data_to_send,// ���ݻ�������ַ - Data buffer address
                             8,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                             &m_osMulWrite);
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read

        Sleep(LATENCY_TIME_COM);
        CString nReadSection;
        nReadSection.Format(_T("MulRead_%d"), ncomport);
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, nReadSection))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - Handle
                        to_send_data,// ���ݻ�������ַ - Data buffer address
                        length*2+5,// ���ݴ�С - Data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
            return -2;
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
            return -2;
        if(to_send_data[length*2+4]!=(crc & 0xff))
            return -2;
        for(int i=0; i<length; i++)
            put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
        return length;
    }
    if(bComm_Type==1)//tcp.
    {
        /*
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting
        TS_UC to_send_data[700]={'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        TS_UC data_to_send[8]={'\0'};//the array to used in writefile()
        data_to_send[0]=device_var;//slave address
        data_to_send[1]=3;//function multiple
        data_to_send[2]=start_address>>8 & 0xff;//starting address hi
        data_to_send[3]=start_address & 0xff;//starting address lo
        data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[5]=length & 0xff;//quantity of registers lo
        TS_US crc=CRC16(data_to_send,6);
        data_to_send[6]=(crc>>8) & 0xff;
        data_to_send[7]=crc & 0xff;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSocket==INVALID_SOCKET)
        {
        	return -1;
        }
        ::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),MSG_OOB);

        int nn=sizeof(to_send_data);
        int nRecv = ::recv(m_hSocket, (char*)to_send_data, length*2+5, 0);

        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
        	return -2;
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
        	return -2;
        if(to_send_data[length*2+4]!=(crc & 0xff))
        	return -2;
        for(int i=0;i<length;i++)
        	put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
        return length;
        */
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting
        SOCKET m_hSocket = NULL;
        m_hSocket = m_tcp_hSocket[ncomport];
        TS_UC to_send_data[700]= {'\0'};
        TS_UC to_Reive_data[700]= {'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        TS_UC data_to_send[12]= {'\0'}; //the array to used in writefile()
        data_to_send[0]= trans_high_id;
        data_to_send[1]= trans_low_id;
        data_to_send[2]=0;
        data_to_send[3]=0;
        data_to_send[4]=0;
        data_to_send[5]=6;



        data_to_send[6]=device_var;//slave address
        data_to_send[7]=3;//function multiple
        data_to_send[8]=start_address>>8 & 0xff;//starting address hi
        data_to_send[9]=start_address & 0xff;//starting address lo
        data_to_send[10]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[11]=length & 0xff;//quantity of registers lo

        //TS_US crc=CRC16(data_to_send,6);
        //data_to_send[6]=(crc>>8) & 0xff;
        //data_to_send[7]=crc & 0xff;/

//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        ::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),0);

        Sleep(LATENCY_TIME_NET);
        int nn=sizeof(to_Reive_data);
        int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length*2+12, 0);

        memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
            return -2;
        /*
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
        	return -2;

        if(to_send_data[length*2+4]!=(crc & 0xff))
        	return -2;
        	*/
        for(int i=0; i<length; i++)
            put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
        return length;
    }
    return -1;
}




OUTPUT int CheckTstatOnline2_a_nocretical(TS_UC devLo, TS_UC devHi, bool bComm_Type,int ncomport)
{
    CString strlog;


    if (bComm_Type == 0)
    {
        //the second time
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value == -6 , the bus has bannet protocol,scan stop;
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ����� - Clear serial port buffer
        //the return value is the register address
        strlog.Format(_T("Com Scan:  From ID=%d To ID=%d"), devLo, devHi);
        //WriteLogFile(strlog);

        if (devLo<1 || devHi>254)
            return -5;
        //the input inspect
        for (int i = 0; i<13; i++)
            gval[i] = 0;/////////////////////////////////////////clear buffer
        TS_UC  pval[6];
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = 255;
        pval[1] = 25;  //put comments here,
        pval[2] = devHi;
        pval[3] = devLo;
        crc = CRC16(pval, 4);
        pval[4] = (crc >> 8) & 0xff;
        pval[5] = crc & 0xff;

        HANDLE m_hSerial = NULL;

        if (m_com_h_serial[ncomport] == NULL)
        {
            return -1;
        }
        m_hSerial = m_com_h_serial[ncomport];

        ////////////////////////////////////////////////////////////overlapped declare
        CString nsection;
        nsection.Format(_T("Write_%d"), ncomport);
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if ((m_osWrite.hEvent = CreateEvent(NULL, true, false, nsection)) == NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0;
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear buffer
        int fState = WriteFile(m_hSerial,// ��� - Handle
            pval,// ���ݻ�������ַ - Data buffer address
            6,// ���ݴ�С - Data size
            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
            &m_osWrite);
        if (!fState)// ��֧���ص� - Does not support overlap
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
                                                                                                   //			if(GetLastError()==ERROR_IO_INCOMPLETE)
                                                                                                   //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number = 0;
        }

        //	TRACE("%d T:%x %x %x %x %x %x\n",ddd,pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
        //CloseHandle(m_osWrite.hEvent);
        ///////////////////////////up is write
        Sleep(LATENCY_TIME_COM);//because that scan have a delay lower 75ms
                                /////////////**************down is read
        CString nsection_read;
        nsection_read.Format(_T("Read_%d"), ncomport);
        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if ((m_osRead.hEvent = CreateEvent(NULL, true, false, nsection_read)) == NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0;
        ////////////////////////////////////////////////clear com error
        fState = ReadFile(m_hSerial,// ��� - handle
            gval,// ���ݻ�������ַ - data buffer address
            13,// ���ݴ�С - data size
            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
            &m_osRead);
        if (!fState)// ��֧���ص� - overlapping not supported
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number = 0;
        }



        //WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
        CString filelog;
        filelog = _T("Send a scan command to any devices: ");



        for (int i = 0; i < 6; i++)
        {
            int nValue = pval[i];
            CString strValue;
            strValue.Format(_T("%0X, "), nValue);
            //g_fileScanLog->WriteString(strValue);
            filelog += strValue;
        }

        //WriteLogFile(filelog);
        filelog.Empty();
        filelog = _T("Recv Data : ");
        for (int i = 0; i < 13; i++)
        {
            int nValue = gval[i];
            CString strValue;
            strValue.Format(_T("%0X, "), nValue);
            //g_fileScanLog->WriteString(strValue);
            filelog += strValue;
            if ((nValue == 0x55) && ((i + 1)<13))	//Ϊ�˷�ֹ���У����2�����ڵ� 55 ff  ��������55 ff - To prevent misjudgment, detect 2 cycles of 55 ff;
            {
                if ((gval[i + 1] == 0xff) && (i + 8)<13)
                {
                    if ((gval[i + 8] == 0x55) && (i + 9)<13)
                    {
                        if (gval[i + 9] == 0xff)
                            return -6;//��������bacnetЭ�飬����ɨ�� - BACnet protocol exists on the bus, end scanning;
                    }

                }
            }
        }

        //WriteLogFile(filelog);
        int index = filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
        if (index == -1)
        {
            //WriteLogFile(_T(">>More than one device answer......"));
        }
        else
        {
            //WriteLogFile(_T(">>No one device answer......"));
        }

        if (gval[7] == 0 && gval[8] == 0 && gval[9] == 0 && gval[10] == 0 && gval[11] == 0 && gval[12] == 0)  //modify by fance .���ɨ��У��պ�Ϊ0��ɨ������ bug - Fixed bug where scan verification happens to be 0 and cannot be scanned;
                                                                                                              //if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
        {
            //old scan protocal
            old_or_new_scan_protocal_in_dll = 2;
            if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0)
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -4;              //no response ,no connection


            }
            // added by zgq; find this situation: t3000 can find a comport,
            //which don't connect a tstat, but write file to the com, will receive the same data with send,
            // infact the com port don't work fine. But it never give you a wrong data.
            if (gval[0] == pval[0] && gval[1] == pval[1] && gval[2] == pval[2] && gval[3] == pval[3] && gval[4] == pval[4] && gval[5] == pval[5])
            {


                Sleep(SLEEP_TIME);
                return -4;
            }

            //////////////////////////////////////////////////////////////////////////
            if (gval[5] != 0 || gval[6] != 0)//to inspect
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -3;
            }
            if ((gval[0] != pval[0]) || (gval[1] != 25))
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
            crc = CRC16(gval, 3);
            if ((gval[3] != ((crc >> 8) & 0xff)) || (gval[4] != (crc & 0xff)))
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
        }
        else
        {

            // new scan protocal,if many old tstat ,get into here ,scan result is oK too.
            old_or_new_scan_protocal_in_dll = 1;
            Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
            if ((devLo == devHi) && ((gval[9] != 0 || gval[10] != 0 || gval[11] != 0 || gval[12] != 0)))  //���2�ַ� ��ʼID һ�� ���յ��������ݣ�˵�����������д���;
                return -6;
            if (gval[9] != 0 || gval[10] != 0 || gval[11] != 0 || gval[12] != 0)//to inspect
                return -3;
            if ((gval[0] != pval[0]) || (gval[1] != 25))
                return -2;
            crc = CRC16(gval, 7);
            if (gval[7] != ((crc >> 8) & 0xff))
                return -2;
            if (gval[8] != (crc & 0xff))
                return -2;

        }


        //here is different with CheckTstatOnline() function
        //	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
        //	TRACE("%d ^-^ ^-^ %d^\n",ddd,gval[2]);
        /*
        if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
        {
        default_file.SeekToEnd();
        a_line.Format("exist:%d",gval[2]);
        default_file.WriteString(a_line+"\n");
        default_file.Flush();
        default_file.Close();
        }*/
        return gval[2];
    }

    if (bComm_Type == 1)
    {
        //the second time
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ����� - Clear serial port buffer
        //the return value is the register address
        SOCKET m_hSocket = NULL;
        m_hSocket = m_tcp_hSocket[ncomport];
        strlog.Format(_T("NET Scan:  From ID=%d To ID=%d"), devLo, devHi);
        //NET_WriteLogFile(strlog);
        if (devLo<1 || devHi>254)
            return -5;
        //the input inspect
        for (int i = 0; i<13; i++)
            gval[i] = 0;/////////////////////////////////////////clear buffer

        TS_UC  pval[10];
        //		TS_US crc;
        //		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
        pval[0] = 1;
        pval[1] = 2;
        pval[2] = 3;
        pval[3] = 4;
        pval[4] = 5;
        pval[5] = 6;

        pval[6] = 255;
        pval[7] = 25;
        pval[8] = devHi;
        pval[9] = devLo;

        /*
        crc = CRC16(pval,4);
        pval[4] = (crc >>8) & 0xff;
        pval[5] = crc & 0xff;
        */

        if (m_hSocket == INVALID_SOCKET)
        {
            return -1;
        }
        int nRet = ::send(m_hSocket, (char*)pval, sizeof(pval), 0);//scan ɨNC�е�TSTAT

                                                                   //Sleep(SLEEP_TIME+8);
        Sleep(LATENCY_TIME_NET + 100);
        // 		TS_UC  rvData[19];
        // 		for(int i=0;i<19;i++)
        // 			rvData[i]=0;

        TS_UC  rvData[100];
        for (int i = 0; i<100; i++)
            rvData[i] = 0;


        int nRecv = ::recv(m_hSocket, (char*)rvData, sizeof(rvData), 0);
        if (nRecv>0)
        {
            memcpy(gval, (void*)&rvData[6], sizeof(rvData));
        }

        //		static int num = 0;
        //NET_WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
        CString filelog;
        filelog = _T("Send a scan command to any devices: ");


        for (int i = 0; i < 10; i++)
        {
            int nValue = pval[i];
            CString strValue;
            strValue.Format(_T("%0X, "), nValue);
            //g_fileScanLog->WriteString(strValue);
            filelog += strValue;
        }

        //NET_WriteLogFile(filelog);
        filelog.Empty();

        filelog = _T("Recv Data : ");
        for (int i = 0; i <13; i++)
        {
            int nValue = gval[i];
            CString strValue;
            strValue.Format(_T("%0X, "), nValue);
            //g_fileScanLog->WriteString(strValue);
            filelog += strValue;
        }
        ////NET_WriteLogFile(_T("Recv Data:"));
        //NET_WriteLogFile(filelog);
        int index = filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
        if (index == -1)
        {
            //WriteLogFile(_T(">>More than one device answer......"));
        }
        else
        {
            //WriteLogFile(_T(">>No one device answer......"));
        }

        //		if (g_fileScanLog == NULL)
        //		{
        //			g_fileScanLog = new CStdioFile;
        //		}
        //
        //	if (g_fileScanLog != NULL)
        //		{
        //			TCHAR exeFullPath[MAX_PATH+1]; //
        //			GetModuleFileName(NULL, exeFullPath, MAX_PATH);
        //			(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
        //
        //			CString g_strExePth=exeFullPath;
        //			m_strFileINI = g_strExePth + _T("ScanLog.TXT");
        //			if(g_fileScanLog->Open(m_strFileINI.GetString(),CFile::modeReadWrite))
        //			{
        //#if 1
        //                g_fileScanLog->SeekToEnd();
        //				CString strsend,strreceive;
        //				strsend = _T("Send Data:");
        //				strreceive = _T("Receive Data:");
        //				g_fileScanLog->WriteString(strsend+_T("\n"));
        //
        //				CString str;
        //				int idate;
        //				//str.Format(_T("%"),pval);
        //				//m_pFile->WriteString(str+_T("\n"));
        //				for (int i =0;i<sizeof(pval);i++)
        //				{
        //					idate =(int)pval[i];
        //					str.Format(_T("%0x"),idate);
        //
        //					g_fileScanLog->WriteString(str+_T(" "));
        //
        //				}
        //
        //				g_fileScanLog->WriteString(_T("\n"));
        //				g_fileScanLog->WriteString(strreceive+_T("\n"));//��
        //				for (int i =0;i<sizeof(rvData);i++)
        //				{
        //					idate =(int)rvData[i];
        //					str.Format(_T("%0x"),idate);
        //					g_fileScanLog->WriteString(str+_T(" "));
        //				}
        //
        //
        //
        //				g_fileScanLog->WriteString(_T("\n"));
        //#endif
        //
        //
        //				g_fileScanLog->Close();
        //			}
        //		    else
        //			{
        //			 //-----
        //			}
        //
        //
        //		}








        //if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)//// it's old code, modified by zgq:2011-12-14
        if (gval[8] == 0 && gval[9] == 0 && gval[10] == 0 && gval[11] == 0 && gval[12] == 0 && gval[3] == 0 && gval[4] == 0 && gval[5] == 0 && gval[6] == 0)
        {
            //old scan protocal
            old_or_new_scan_protocal_in_dll = 2;
            if (gval[0] == 0 && gval[1] == 0 && gval[2] == 0 && gval[3] == 0 && gval[4] == 0)
            {
                Sleep(SLEEP_TIME + 8);//be must ,if not use this ,will found some trouble
                return -4;              //no response ,no connection
            }
            if (gval[5] != 0 || gval[6] != 0)//to inspect
            {
                Sleep(SLEEP_TIME + 8);//be must ,if not use this ,will found some trouble
                return -3;
            }
            if ((gval[0] != 255) || (gval[1] != 25))
            {
                //		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
            /*
            crc=CRC16(gval,3);
            if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
            {
            //		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
            return -2;
            }
            */

        }
        else
        {
            // new scan protocal,if many old tstat ,get into here ,scan result is oK too.
            old_or_new_scan_protocal_in_dll = 1;
            Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble


                              //if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)//to inspect
            if (gval[9] != 0 || gval[10] != 0 || gval[11] != 0 || gval[12] != 0)//to inspect  //zgq: same with com port scan
            {
                //Sleep(SLEEP_TIME+8);
                return -3;

            }

            if ((gval[0] != 255) || (gval[1] != 25))
                return -2;

            /*
            if (gval[0]==0&&gval[0]==0&&gval[0]==0)
            {
            return -3;
            }
            */

            /*
            crc=CRC16(gval,7);
            if( gval[7]!=((crc>>8) & 0xff) )
            return -2;
            if(gval[8]!=(crc & 0xff))
            return -2;
            */
        }
        return gval[2];
    }
    return -1;
}

OUTPUT int CheckTstatOnline2_a(TS_UC devLo,TS_UC devHi, bool bComm_Type)
{
    CString strlog;


    if(bComm_Type==0)
    {
        //the second time
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value == -6 , the bus has bannet protocol,scan stop;
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ�����
        //the return value is the register address
        strlog.Format(_T("Com Scan:  From ID=%d To ID=%d"),devLo,devHi);
        //WriteLogFile(strlog);

        if(devLo<1 || devHi>254)
            return -5;
        //the input inspect
        for(int i=0; i<13; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_UC  pval[6];
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
        pval[0] = 255;
        pval[1] = 25;  //put comments here,
        pval[2] = devHi;
        pval[3] = devLo;
        crc = CRC16(pval,4);
        pval[4] = (crc >>8) & 0xff;
        pval[5] = crc & 0xff;
        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear buffer
        int fState=WriteFile(m_hSerial,// ��� - handle
                             pval,// ���ݻ�������ַ - data buffer address
                             6,// ���ݴ�С - data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                             &m_osWrite);
        if(!fState)// ��֧���ص� - overlapping not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ� - wait
                //			if(GetLastError()==ERROR_IO_INCOMPLETE)
                //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number=0;
        }

        //	TRACE("%d T:%x %x %x %x %x %x\n",ddd,pval[0],pval[1],pval[2],pval[3],pval[4],pval[5]);
        //CloseHandle(m_osWrite.hEvent);
        ///////////////////////////up is write
        Sleep(LATENCY_TIME_COM);//because that scan have a delay lower 75ms
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0;
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - handle
                        gval,// ���ݻ�������ַ - data buffer address
                        13,// ���ݴ�С - data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - overlapping not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ� - wait
            }
            else
                m_had_send_data_number=0;
        }



        //WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
        CString filelog;
        filelog=_T("Send a scan command to any devices: ");



        for (int i = 0; i < 6; i++)
        {
            int nValue = pval[i];
            CString strValue;
            strValue.Format(_T("%0X, "), nValue);
            //g_fileScanLog->WriteString(strValue);
            filelog+=strValue;
        }

        //WriteLogFile(filelog);
        filelog.Empty();
        filelog=_T("Recv Data : ");
        for (int i = 0; i < 13; i++)
        {
            int nValue = gval[i];
            CString strValue;
            strValue.Format(_T("%0X, "), nValue);
            //g_fileScanLog->WriteString(strValue);
            filelog+=strValue;
            if((nValue == 0x55) &&((i+1)<13))	//Ϊ�˷�ֹ���У����2�����ڵ� 55 ff  ��������55 ff - To prevent misjudgment, detect 2 cycles of 55 ff;
            {
                if((gval[i+1] == 0xff) && (i+8)<13)
                {
                    if((gval[i+8] == 0x55) && (i+9)<13)
                    {
                        if(gval[i+9] == 0xff)
                            return -6;//��������bacnetЭ�飬����ɨ�� - BACnet protocol exists on the bus, end scanning;
                    }

                }
            }
        }

        //WriteLogFile(filelog);
        int index=filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
        if(index==-1)
        {
            //WriteLogFile(_T(">>More than one device answer......"));
        }
        else
        {
            //WriteLogFile(_T(">>No one device answer......"));
        }

		if(gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)  //modify by fance .���ɨ��У��պ�Ϊ0��ɨ������ bug - Fixed bug where scan verification happens to be 0 and cannot be scanned;
        //if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)
        {
            //old scan protocal
            old_or_new_scan_protocal_in_dll=2;
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -4;              //no response ,no connection


            }
            // added by zgq; find this situation: t3000 can find a comport,
            //which don't connect a tstat, but write file to the com, will receive the same data with send,
            // infact the com port don't work fine. But it never give you a wrong data.
            if(gval[0]==pval[0] && gval[1]==pval[1] && gval[2]==pval[2] && gval[3]==pval[3] && gval[4]==pval[4] && gval[5]==pval[5])
            {


                Sleep(SLEEP_TIME);
                return -4;
            }

            //////////////////////////////////////////////////////////////////////////
            if(gval[5]!=0 || gval[6]!=0)//to inspect
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -3;
            }
            if((gval[0]!=pval[0]) || (gval[1]!=25))
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
            crc=CRC16(gval,3);
            if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
            {
                Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
        }
        else
        {

            // new scan protocal,if many old tstat ,get into here ,scan result is oK too.
            old_or_new_scan_protocal_in_dll=1;
            Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
            if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect
                return -3;
            if((gval[0]!=pval[0]) || (gval[1]!=25))
                return -2;
            crc=CRC16(gval,7);
            if( gval[7]!=((crc>>8) & 0xff) )
                return -2;
            if(gval[8]!=(crc & 0xff))
                return -2;

        }


        //here is different with CheckTstatOnline() function
        //	TRACE("%d R:%x %x %x %x %x %x %x %x %x %x %x %x %x\n",ddd,gval[0],gval[1],gval[2],gval[3],gval[4],gval[5],gval[6],gval[7],gval[8],gval[9],gval[10],gval[11],gval[12]);
        //	TRACE("%d ^-^ ^-^ %d^\n",ddd,gval[2]);
        /*
        if(default_file.Open(_T(saved_path.GetString()),CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate)!=0)
        {
        default_file.SeekToEnd();
        a_line.Format("exist:%d",gval[2]);
        default_file.WriteString(a_line+"\n");
        default_file.Flush();
        default_file.Close();
        }*/
        return gval[2];
    }

    if(bComm_Type==1)
    {
        //the second time
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ�����
        //the return value is the register address
        strlog.Format(_T("NET Scan:  From ID=%d To ID=%d"),devLo,devHi);
        //NET_WriteLogFile(strlog);
        if(devLo<1 || devHi>254)
            return -5;
        //the input inspect
        for(int i=0; i<13; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer

        TS_UC  pval[10];
//		TS_US crc;
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - number of bytes sent
        pval[0]=1;
        pval[1]=2;
        pval[2]=3;
        pval[3]=4;
        pval[4]=5;
        pval[5]=6;

        pval[6] = 255;
        pval[7] = 25;
        pval[8] = devHi;
        pval[9] = devLo;

        /*
        crc = CRC16(pval,4);
        pval[4] = (crc >>8) & 0xff;
        pval[5] = crc & 0xff;
        */

        if (m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }
        int nRet =::send(m_hSocket,(char*)pval,sizeof(pval),0);//scan ɨNC�е�TSTAT

        //Sleep(SLEEP_TIME+8);
        Sleep(LATENCY_TIME_NET+100);
// 		TS_UC  rvData[19];
// 		for(int i=0;i<19;i++)
// 			rvData[i]=0;

        TS_UC  rvData[100];
        for(int i=0; i<100; i++)
            rvData[i]=0;


        int nRecv = ::recv(m_hSocket, (char*)rvData, sizeof(rvData), 0);
        if (nRecv>0)
        {
            memcpy(gval,(void*)&rvData[6],sizeof(rvData));
        }

//		static int num = 0;
        //NET_WriteLogFile(_T(">>broad cast commnad here, fast check is any devices are alive"));
        CString filelog;
        filelog=_T("Send a scan command to any devices: ");


        for (int i = 0; i < 10; i++)
        {
            int nValue = pval[i];
            CString strValue;
            strValue.Format(_T("%0X, "), nValue);
            //g_fileScanLog->WriteString(strValue);
            filelog+=strValue;
        }

        //NET_WriteLogFile(filelog);
        filelog.Empty();

        filelog=_T("Recv Data : ");
        for (int i = 0; i <13; i++)
        {
            int nValue = gval[i];
            CString strValue;
            strValue.Format(_T("%0X, "), nValue);
            //g_fileScanLog->WriteString(strValue);
            filelog+=strValue;
        }
        ////NET_WriteLogFile(_T("Recv Data:"));
        //NET_WriteLogFile(filelog);
        int index=filelog.Find(_T("0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,"));
        if(index==-1)
        {
            //WriteLogFile(_T(">>More than one device answer......"));
        }
        else
        {
            //WriteLogFile(_T(">>No one device answer......"));
        }

//		if (g_fileScanLog == NULL)
//		{
//			g_fileScanLog = new CStdioFile;
//		}
//
//	if (g_fileScanLog != NULL)
//		{
//			TCHAR exeFullPath[MAX_PATH+1]; //
//			GetModuleFileName(NULL, exeFullPath, MAX_PATH);
//			(_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
//
//			CString g_strExePth=exeFullPath;
//			m_strFileINI = g_strExePth + _T("ScanLog.TXT");
//			if(g_fileScanLog->Open(m_strFileINI.GetString(),CFile::modeReadWrite))
//			{
//#if 1
//                g_fileScanLog->SeekToEnd();
//				CString strsend,strreceive;
//				strsend = _T("Send Data:");
//				strreceive = _T("Receive Data:");
//				g_fileScanLog->WriteString(strsend+_T("\n"));
//
//				CString str;
//				int idate;
//				//str.Format(_T("%"),pval);
//				//m_pFile->WriteString(str+_T("\n"));
//				for (int i =0;i<sizeof(pval);i++)
//				{
//					idate =(int)pval[i];
//					str.Format(_T("%0x"),idate);
//
//					g_fileScanLog->WriteString(str+_T(" "));
//
//				}
//
//				g_fileScanLog->WriteString(_T("\n"));
//				g_fileScanLog->WriteString(strreceive+_T("\n"));//��
//				for (int i =0;i<sizeof(rvData);i++)
//				{
//					idate =(int)rvData[i];
//					str.Format(_T("%0x"),idate);
//					g_fileScanLog->WriteString(str+_T(" "));
//				}
//
//
//
//				g_fileScanLog->WriteString(_T("\n"));
//#endif
//
//
//				g_fileScanLog->Close();
//			}
//		    else
//			{
//			 //-----
//			}
//
//
//		}








        //if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0)//// it's old code, modified by zgq:2011-12-14
        if(gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0 && gval[12]==0 && gval[3]==0 &&gval[4]==0 &&gval[5]==0 &&gval[6]==0)
        {
            //old scan protocal
            old_or_new_scan_protocal_in_dll=2;
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0)
            {
                Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
                return -4;              //no response ,no connection
            }
            if(gval[5]!=0 || gval[6]!=0)//to inspect
            {
                Sleep(SLEEP_TIME+8);//be must ,if not use this ,will found some trouble
                return -3;
            }
            if((gval[0]!=255) || (gval[1]!=25))
            {
                //		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
                return -2;
            }
            /*
            crc=CRC16(gval,3);
            if( (gval[3]!=((crc>>8) & 0xff)) || (gval[4]!=(crc & 0xff)))
            {
            	//		Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble
            	return -2;
            }
            */

        }
        else
        {
            // new scan protocal,if many old tstat ,get into here ,scan result is oK too.
            old_or_new_scan_protocal_in_dll=1;
            Sleep(SLEEP_TIME);//be must ,if not use this ,will found some trouble


            //if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)//to inspect
            if(gval[9]!=0 || gval[10]!=0 || gval[11]!=0 || gval[12]!=0)//to inspect  //zgq: same with com port scan
            {
                //Sleep(SLEEP_TIME+8);
                return -3;

            }

            if((gval[0]!=255) || (gval[1]!=25))
                return -2;

            /*
            if (gval[0]==0&&gval[0]==0&&gval[0]==0)
            {
            	return -3;
            }
            */

            /*
            crc=CRC16(gval,7);
            if( gval[7]!=((crc>>8) & 0xff) )
            	return -2;
            if(gval[8]!=(crc & 0xff))
            	return -2;
            	*/
        }
        return gval[2];
    }
    return -1;
}

OUTPUT int CheckTstatOnline_nocretical(TS_UC devLo, TS_UC devHi, bool bComm_Type, int ncomport)
{
    if (bComm_Type == 0)
    {
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value == -6 , the bus has bannet protocol,scan stop;
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ����� - Clear serial port buffer
        //the return value is the register address
        //Sleep(50);       //must use this function to slow computer
        if (devLo<1 || devHi>254)
            return -5;
        if (m_com_h_serial[ncomport] == NULL)
        {
            return -1;
        }

        int the_return_value;
        int the_return_value2 = 0;
        //CheckTstatOnline2_a ���Լ�¼�շ������� - CheckTstatOnline2_a can record sent and received data
        the_return_value = CheckTstatOnline2_a_nocretical(devLo, devHi, bComm_Type,ncomport);
        if (the_return_value == -6)
            return -6;
        //down is inspect result first scan
        if (the_return_value == -4)
        {
            the_return_value = CheckTstatOnline2_a_nocretical(devLo, devHi, bComm_Type, ncomport);
            return the_return_value;
        }
        if (old_or_new_scan_protocal_in_dll == 1)
        {
            //new protocal
            if (the_return_value>0)
            {
                the_return_value2 = CheckTstatOnline2_a_nocretical(devLo, devHi, bComm_Type, ncomport);
                if (the_return_value2 != -4)
                    the_return_value = the_return_value2;
            }
            return the_return_value;
        }
        else if (old_or_new_scan_protocal_in_dll == 2)
        {
            //old protocal
            if (the_return_value == -2 || the_return_value == -3 || the_return_value == -4)
                return the_return_value;
            int i = 0;
            do
            {
                the_return_value = CheckTstatOnline2_a_nocretical(devLo, devHi, bComm_Type, ncomport);
                if (the_return_value == -3 || the_return_value == -2 || the_return_value == -4)
                    return the_return_value;
                else if (the_return_value>0)
                    i++;
            } while (i<3);
            return the_return_value;
        }
        //	if(the_return_value>0)
        //		TRACE("^-^ ^-^ %d\n",the_return_value);
        return the_return_value;
    }

    if (bComm_Type == 1)//
    {
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ����� - Clear serial port buffer
        //the return value is the register address
        //Sleep(50);       //must use this function to slow computer
        SOCKET m_hSocket = NULL;
        m_hSocket = m_tcp_hSocket[ncomport];
        if (devLo<1 || devHi>254)
            return -5;
        if (m_hSocket == INVALID_SOCKET)
        {
            return -1;
        }
        int the_return_value;
        int the_return_value2 = 0;
        the_return_value = CheckTstatOnline2_a_nocretical(devLo, devHi, bComm_Type, ncomport);
        //down is inspect result first scan
        if (the_return_value == -4)
        {
            the_return_value = CheckTstatOnline2_a_nocretical(devLo, devHi, bComm_Type, ncomport);
            return the_return_value;
        }
        if (old_or_new_scan_protocal_in_dll == 1)
        {
            //new protocal
            if (the_return_value>0)
            {
                the_return_value2 = CheckTstatOnline2_a_nocretical(devLo, devHi, bComm_Type, ncomport);
                if (the_return_value2 != -4)
                    the_return_value = the_return_value2;
            }
            return the_return_value;
        }
        else if (old_or_new_scan_protocal_in_dll == 2)
        {
            //old protocal
            // 			if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
            // 				return the_return_value;
            // 			int i=0;
            // 			do{
            // 				the_return_value=CheckTstatOnline2(devLo,devHi, bComm_Type);
            // 				if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
            // 					return the_return_value;
            // 				else if(the_return_value>0)
            // 					i++;
            // 			}while(i<3);
            if (the_return_value>0)
            {
                the_return_value2 = CheckTstatOnline2_a_nocretical(devLo, devHi, bComm_Type, ncomport);
                if (the_return_value2 != -4)
                    the_return_value = the_return_value2;
            }
            return the_return_value;
        }
        //	if(the_return_value>0)
        //		TRACE("^-^ ^-^ %d\n",the_return_value);
        return the_return_value;
    }
    return -1;
}

OUTPUT int CheckTstatOnline_a(TS_UC devLo,TS_UC devHi, bool bComm_Type)
{
    if(bComm_Type==0)
    {
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value == -6 , the bus has bannet protocol,scan stop;
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ�����
        //the return value is the register address
        //Sleep(50);       //must use this function to slow computer
        if(devLo<1 || devHi>254)
            return -5;
        if(m_hSerial==NULL)
        {
            return -1;
        }
        int the_return_value;
        int the_return_value2=0;
        //CheckTstatOnline2_a ���Լ�¼�շ������� - CheckTstatOnline2_a can record the sent and received data
        the_return_value=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
        if(the_return_value == -6)
            return -6;
        //down is inspect result first scan
        if(the_return_value==-4)
        {
            the_return_value=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
            return the_return_value;
        }
        if(old_or_new_scan_protocal_in_dll==1)
        {
            //new protocal
            if(the_return_value>0)
            {
                the_return_value2=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
                if(the_return_value2!=-4)
                    the_return_value=the_return_value2;
            }
            return the_return_value;
        }
        else if(old_or_new_scan_protocal_in_dll==2)
        {
            //old protocal
            if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
                return the_return_value;
            int i=0;
            do
            {
                the_return_value=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
                if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
                    return the_return_value;
                else if(the_return_value>0)
                    i++;
            }
            while(i<3);
            return the_return_value;
        }
        //	if(the_return_value>0)
        //		TRACE("^-^ ^-^ %d\n",the_return_value);
        return the_return_value;
    }

    if(bComm_Type==1)//
    {
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -2 ,try it again
        //the return value == -3,Maybe that have more than 2 Tstat is connecting
        //the return value == -4 ,between devLo and devHi,no Tstat is connected ,
        //the return value == -5 ,the input have some trouble
        //the return value >=1 ,the devLo!=devHi,Maybe have 2 Tstat is connecting
        //��մ��ڻ�����
        //the return value is the register address
        //Sleep(50);       //must use this function to slow computer
        if(devLo<1 || devHi>254)
            return -5;
        if (m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }
        int the_return_value;
        int the_return_value2=0;
        the_return_value=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
        //down is inspect result first scan
        if(the_return_value==-4)
        {
            the_return_value=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
            return the_return_value;
        }
        if(old_or_new_scan_protocal_in_dll==1)
        {
            //new protocal
            if(the_return_value>0)
            {
                the_return_value2=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
                if(the_return_value2!=-4)
                    the_return_value=the_return_value2;
            }
            return the_return_value;
        }
        else if(old_or_new_scan_protocal_in_dll==2)
        {
            //old protocal
// 			if(the_return_value==-2 || the_return_value==-3 || the_return_value==-4)
// 				return the_return_value;
// 			int i=0;
// 			do{
// 				the_return_value=CheckTstatOnline2(devLo,devHi, bComm_Type);
// 				if(the_return_value==-3 || the_return_value==-2 || the_return_value==-4)
// 					return the_return_value;
// 				else if(the_return_value>0)
// 					i++;
// 			}while(i<3);
            if(the_return_value>0)
            {
                the_return_value2=CheckTstatOnline2_a(devLo,devHi, bComm_Type);
                if(the_return_value2!=-4)
                    the_return_value=the_return_value2;
            }
            return the_return_value;
        }
        //	if(the_return_value>0)
        //		TRACE("^-^ ^-^ %d\n",the_return_value);
        return the_return_value;
    }
    return -1;
}

OUTPUT void close_T3000_log_file()
{
    m_pFile->Close();
    delete m_pFile;
}

OUTPUT void write_T3000_log_file(CString StrTips)
{



    CCriticalSection  logfile_section;
    logfile_section.Lock();



    CTime nowtime=CTime::GetCurrentTime();
    CString time;
    time.Format(_T("%4d-%.2d-%.2d %.2d:%.2d:%.2d  >>"),nowtime.GetYear(), nowtime.GetMonth(), nowtime.GetDay(),
                nowtime.GetHour(), nowtime.GetMinute(), nowtime.GetSecond());
    CString logstr=time+StrTips;


    m_pFile->SeekToEnd();
    m_pFile->WriteString(logstr.GetBuffer());
    m_pFile->WriteString(_T("\n"));


    m_pFile->Flush();



    logfile_section.Unlock();

}

OUTPUT void Create_T3000_log_file()
{
    m_pFile = new CStdioFile;//txt

    TCHAR exeFullPath[MAX_PATH+1]; //
    GetModuleFileName(NULL, exeFullPath, MAX_PATH);
    (_tcsrchr(exeFullPath, _T('\\')))[1] = 0;
    CString g_strDatabasefilepath=exeFullPath;
    CString g_strExePth=g_strDatabasefilepath;
    m_strFileINI = g_strExePth + _T("") MY_LOG_NAME;
    m_pFile->Open(m_strFileINI.GetString(),CFile::modeReadWrite | CFile::shareDenyNone | CFile::modeCreate );

}



OUTPUT CString Get_NowTime()
{
    SYSTEMTIME st;
    CString strDate,strTime;
    GetLocalTime(&st);
    strDate.Format(_T("%4d-%2d-%2d"),st.wYear,st.wMonth,st.wDay);
    strTime.Format(_T("%2d:%2d:%2d"), st.wHour,st.wMinute,st.wSecond);
    strDate+=_T(" ");
    strDate+=strTime;


    return strDate;
}



OUTPUT int Read_One_tap(TS_UC device_var,TS_US address)
{

//	CSingleLock singlock(&scan_mutex);
//	singlock.Lock();
    int nTemp=-1;
    if(g_Commu_type==0)//serial
    {
        //address        the register
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        //return value == -3 ,no response
        //��մ��ڻ����� - Clear serial port buffer
        //TS_UC  gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];
        for(int i=0; i<11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - number of bytes sent
        pval[0] = device_var;
        pval[1] = 3;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        pval[4] = 0;//(val>>8) & 0xff;//number hi
        pval[5] = 1;//val & 0xff;//number lo
        crc = CRC16(pval,6);
        pval[6] = (crc >>8) & 0xff;
        pval[7] = crc & 0xff;
        if(m_hSerial==NULL)
        {
            return -1;
        }

        ////////////////////////////////////////////////////////////overlapped declare
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        BOOL bRet = ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ///////////////////////////////////////////////////////send the to read message
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0;

        int fState=WriteFile(m_hSerial,// ��� - handle
                             pval,	// ���ݻ�������ַ - data buffer address
                             8,		// ���ݴ�С - data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                             &m_osWrite);
        if(!fState)// ��֧���ص� - overlapping not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
                //			if(GetLastError()==ERROR_IO_PENDING)
                //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number=0;
        }
        //////////////////////////////////////////the message had send ,now to read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;

        ////////////////////////////////////////////////clear com error
        Sleep(LATENCY_TIME_COM);
        if(address==10)
        {
            serinumber_in_dll[0]=serinumber_in_dll[1]=serinumber_in_dll[2]=serinumber_in_dll[3]=0;//this line is for new protocal
            fState=ReadFile(m_hSerial,// ��� - handle
                            gval,// ���ݻ�������ַ - data buffer address
                            11,// ���ݴ�С - data size
                            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                            &m_osRead);
        }
        else
        {
            fState=ReadFile(m_hSerial,// ��� - handle
                            gval,// ���ݻ�������ַ - data buffer address
                            7,// ���ݴ�С - data size
                            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                            &m_osRead);
        }
        if(!fState)// ��֧���ص� - overlapping not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        if(address!=10)
        {
            //old protocal
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 )
                return -3;
            if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )
                return -2;
            crc=CRC16(gval,5);
            if(gval[5]!=((crc>>8)&0xff))
                return -2;
            if(gval[6]!=(crc & 0xff))
                return -2;
        }
        else
        {
            if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)
            {
                //new protocal
                if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=6 )//6
                    return -2;
                crc=CRC16(gval,9);
                if(gval[9]!=((crc>>8)&0xff))
                    return -2;
                if(gval[10]!=(crc & 0xff))
                    return -2;
                serinumber_in_dll[0]=gval[5];
                serinumber_in_dll[1]=gval[6];
                serinumber_in_dll[2]=gval[7];
                serinumber_in_dll[3]=gval[8];//stay serialnumber
                //			TRACE("R:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
            }
            else
            {
                //old protocal
                if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )//2
                    return -2;
                crc=CRC16(gval,5);
                if(gval[5]!=((crc>>8)&0xff))
                    return -2;
                if(gval[6]!=(crc & 0xff))
                    return -2;
            }
        }
        nTemp=gval[3];
        if(nTemp==255)
            nTemp=-1;
        return (nTemp*256+gval[4]);
    }
    if(g_Commu_type==1)//tcp.
    {
        //address        the register
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        //return value == -3 ,no response
        //��մ��ڻ�����
        //TS_UC  gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];

        /*
        struct DataPack
        {
        	TS_UC  empty[6];
        	TS_UC  pval[13];

        };
        */
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        TS_UC data[12];

        data[0]= trans_high_id;
        data[1]= trans_low_id;
        data[2]=0;
        data[3]=0;
        data[4]=0;
        data[5]=6;
        data[6]=device_var;
        data[7]=3;
        data[8]=address>>8 & 0xFF ;
        data[9] = address & 0xFF;
        data[10]=0;
        data[11]=1;

//		DataPack dataInfo;
        for(int i=0; i<11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        //TS_US crc;
        //DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent

        pval[0] = device_var;
        pval[1] = 3;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        pval[4] = 0;//(val>>8) & 0xff;//number hi
        pval[5] = 1;//val & 0xff;//number lo
        //	crc = CRC16(pval,6);
        //	pval[6] = (crc >>8) & 0xff;
        //	pval[7] = crc & 0xff;

        //memcpy((char*)&data[6],(char*)&pval[0],sizeof(pval));

        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        int nLen = sizeof(data);
        //int xx=::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        int xx=::send(m_hSocket,(char*)&data,sizeof(data),0);


        ////////////////////////////////////////////////clear com error
        if(address==10)
        {
            serinumber_in_dll[0]=serinumber_in_dll[1]=serinumber_in_dll[2]=serinumber_in_dll[3]=0;//this line is for new protocal
        }

        Sleep(LATENCY_TIME_NET);//Sleep(SLEEP_TIME);
        //DataPack rvData;
        //Sleep(SLEEP_TIME+10);

        TS_UC rvData[17];
        //int nRecv = ::recv(m_hSocket, (char*)gval, sizeof(gval), 0);
        int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);
        //gval=rvData.pval;
        memcpy((void*)gval,(void*)&rvData[6],13/*sizeof(gval)*/);

        /*
        xx=::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        nRecv = ::recv(m_hSocket, (char*)gval, sizeof(gval), 0);*/

        int nerro= WSAGetLastError();

        if(address!=10)
        {
            //old protocal
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0/* && gval[5]==0 && gval[6]==0*/ )
                return -3;
            if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=2 )
                return -2;

            /*;
            crc=CRC16(gval,5);
            if(gval[5]!=((crc>>8)&0xff))
            	return -2;
            if(gval[6]!=(crc & 0xff))
            	return -2;
            */

        }
        else
        {
            if(gval[7]!=0 || gval[8]!=0 || gval[9]!=0 || gval[10]!=0)
            {
                //new protocal
                if(gval[0]!=pval[0] || gval[1]!=pval[1] || gval[2]!=6 )//6
                    return -2;
                /*
                crc=CRC16(gval,9);
                if(gval[9]!=((crc>>8)&0xff))
                	return -2;
                if(gval[10]!=(crc & 0xff))
                	return -2;
                	*/
                serinumber_in_dll[0]=gval[5];
                serinumber_in_dll[1]=gval[6];
                serinumber_in_dll[2]=gval[7];
                serinumber_in_dll[3]=gval[8];//stay serialnumber
                //			TRACE("R:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
            }
            else
            {
                //old protocal
                if(gval[0]!=pval[0] || gval[1]!=gval[1] || gval[2]!=2 )//2
                    return -2;
                /*
                crc=CRC16(gval,5);
                if(gval[5]!=((crc>>8)&0xff))
                	return -2;
                if(gval[6]!=(crc & 0xff))
                	return -2;
                	*/
            }
        }
        nTemp=gval[3];
        if(nTemp==255)
            nTemp=-1;
        return (gval[3]*256+gval[4]);
    }
    return -1; //add by Fance
//	singlock.Unlock();
}

OUTPUT int Write_One_tap(TS_UC device_var,TS_US address,TS_US val)
{
    if (g_Commu_type==0)
    {
        //address        the register
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -3 , no response
        //��մ��ڻ����� - Clear serial port buffer

        //		gval[8]={'\0'};//the data that get
        //      TS_UC  pval[9];
        for(int i=0; i<=11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
            pval[4]=0x55;/////////////////////////////new protocal or write_one 10
        else
            pval[4] = (val>>8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo
        if(address!=10)
        {
            crc = CRC16(pval,6);
            pval[6] = (crc >>8) & 0xff;
            pval[7] = crc & 0xff;
        }
        else
        {
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                crc = CRC16(pval,6);
                pval[6] = (crc >>8) & 0xff;
                pval[7] = crc & 0xff;
            }
            else
            {
                pval[6]=serinumber_in_dll[0];
                pval[7]=serinumber_in_dll[1];
                pval[8]=serinumber_in_dll[2];
                pval[9]=serinumber_in_dll[3];
                crc = CRC16(pval,10);
                pval[10] = (crc >>8) & 0xff;
                pval[11] = crc & 0xff;
            }
        }

        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osWrite, 0, sizeof(OVERLAPPED));
        if((m_osWrite.hEvent = CreateEvent(NULL,true,false,_T("Write")))==NULL)
            return -2;
        m_osWrite.Offset = 0;
        m_osWrite.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        int fState;
        Sleep(50);
        if(address!=10)
        {
            fState=WriteFile(m_hSerial,// ��� - handle
                             pval,// ���ݻ�������ַ - data buffer address
                             8,// ���ݴ�С - data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                             &m_osWrite);
        }
        else
        {
            //==10 register
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                fState=WriteFile(m_hSerial,// ��� - handle
                                 pval,// ���ݻ�������ַ - data buffer address
                                 8,// ���ݴ�С - data size
                                 &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                                 &m_osWrite);
            }
            else
            {
                //new protocal
                fState=WriteFile(m_hSerial,// ��� - handle
                                 pval,// ���ݻ�������ַ - data buffer address
                                 12,// ���ݴ�С - data size
                                 &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                                 &m_osWrite);
            }
        }
        if(!fState)// ��֧���ص�
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
                //			if(GetLastError()==ERROR_IO_INCOMPLETE)
                //				AfxMessageBox("wrong1");
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error
        Sleep(LATENCY_TIME_COM);
        if(address!=10)
        {
            fState=ReadFile(m_hSerial,// ��� - handle
                            gval,// ���ݻ�������ַ - data buffer address
                            8,// ���ݴ�С - data size
                            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                            &m_osRead);
        }
        else
        {
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                fState=ReadFile(m_hSerial,// ��� - handle
                                gval,// ���ݻ�������ַ - data buffer address
                                8,// ���ݴ�С - data size
                                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                                &m_osRead);
            }
            else
            {
                //new protocal
                fState=ReadFile(m_hSerial,// ��� - handle
                                gval,// ���ݻ�������ַ - data buffer address
                                12,// ���ݴ�С - data size
                                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                                &m_osRead);
            }
        }
        if(!fState)// ��֧���ص� - Overlapped I/O not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        if(address!=10)
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0)
                return -3;
            for(int i=0; i<8; i++)
                if(gval[i]!=pval[i])
                    return -2;
        }
        else
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0)
                return -3;
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                for(int i=0; i<8; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
            else
            {
                //new protocal
                //			crc=CRC16(gval,10);
                //			if(gval[6]!=((crc>>8)&0xff))
                //				return -2;
                //			if(gval[7]!=(crc & 0xff))
                //				return -2;
                //			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
                if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 && gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0)
                    return -3;
                for(int i=0; i<12; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
        }
        return 1;
    }

    if(g_Commu_type==1)//tcp.
    {
        //address        the register
        //val         the value that you want to write to the register
        //the return value == -1 ,no connecting
        //the return value == -3 , no response
        //��մ��ڻ�����

        //TS_UC data[12];
        TS_UC data[16];
// 		TS_UC* data=NULL;
        int nSendNum = 12;
        if (address==10)
        {
            //data = new TS_UC[16];
            nSendNum = 16;
        }
        else
        {
            //data = new TS_UC[12];
            nSendNum = 12;
        }
        ZeroMemory(data, nSendNum);
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        data[0]= trans_high_id;
        data[1]= trans_low_id;
        data[2]=0;
        data[3]=0;
        data[4]=0;
        data[5]=6;
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        data[6] = device_var;
        data[7] = 6;
        data[8] = address>>8 & 0xFF ;
        data[9] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
            data[10]=0x55;/////////////////////////////new protocal or write_one 10
        else
            data[10] = (val>>8) & 0xff;//number hi
        data[11] = val & 0xff;//number lo

        if(address==10)
        {

            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
            }
            else
            {
                //* uncommend by zgq; we use net work scan tstat through NC
                data[12]=serinumber_in_dll[0];
                data[13]=serinumber_in_dll[1];
                data[14]=serinumber_in_dll[2];
                data[15]=serinumber_in_dll[3];
                //*/
            }
        }


        for(int i=0; i<=11; i++)
            gval[i]=0;/////////////////////////////////////////clear buffer
        //	TS_US crc;
        //	DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
            pval[4]=0x55;/////////////////////////////new protocal or write_one 10
        else
            pval[4] = (val>>8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo

        /*
        for(int i=0;i<=11;i++)
        	gval[i]=0;/////////////////////////////////////////clear buffer
        TS_US crc;
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        pval[0] = device_var;
        pval[1] = 6;
        pval[2] = address>>8 & 0xFF ;
        pval[3] = address & 0xFF;
        if(address==10 && (serinumber_in_dll[0]!=0 || serinumber_in_dll[1]!=0 || serinumber_in_dll[2]!=0 || serinumber_in_dll[3]!=0))
        	pval[4]=0x55;/////////////////////////////new protocal or write_one 10
        else
        	pval[4] = (val>>8) & 0xff;//number hi
        pval[5] = val & 0xff;//number lo
        if(address!=10)
        {
        	crc = CRC16(pval,6);
        	pval[6] = (crc >>8) & 0xff;
        	pval[7] = crc & 0xff;
        }
        else
        {
        	if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
        	{
        		crc = CRC16(pval,6);
        		pval[6] = (crc >>8) & 0xff;
        		pval[7] = crc & 0xff;
        	}
        	else
        	{
        		pval[6]=serinumber_in_dll[0];
        		pval[7]=serinumber_in_dll[1];
        		pval[8]=serinumber_in_dll[2];
        		pval[9]=serinumber_in_dll[3];
        		crc = CRC16(pval,10);
        		pval[10] = (crc >>8) & 0xff;
        		pval[11] = crc & 0xff;
        	}
        }
        */
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        //::send(m_hSocket,(char*)pval,sizeof(pval),MSG_OOB);
        //::send(m_hSocket,(char*)data,sizeof(data),0);
        int nRet = ::send(m_hSocket,(char*)data,nSendNum,0);

        //Sleep(SLEEP_TIME+10);
        Sleep(LATENCY_TIME_NET);
        //Sleep(300);
        TS_UC rvdata[17];
        int nRecv = ::recv(m_hSocket, (char*)rvdata, sizeof(rvdata), 0);

        int nErr = WSAGetLastError();

        memcpy((void*)&gval[0],(void*)&rvdata[6],13/*sizeof(gval)*/);
        if(address!=10)
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 /*&& gval[6]==0 && gval[7]==0*/)
                return -3;
            for(int i=0; i<5; i++)
                if(gval[i]!=pval[i])
                    return -2;
        }
        else
        {
            if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0/* && gval[6]==0 && gval[7]==0*/)
                return -3;
            if(serinumber_in_dll[0]==0 && serinumber_in_dll[1]==0 && serinumber_in_dll[2]==0 && serinumber_in_dll[3]==0)
            {
                //old protocal
                for(int i=0; i<5; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
            else
            {
                //new protocal
                //			crc=CRC16(gval,10);
                //			if(gval[6]!=((crc>>8)&0xff))
                //				return -2;
                //			if(gval[7]!=(crc & 0xff))
                //				return -2;
                //			TRACE("W:%x %x %x %x\n",serinumber_in_dll[0],serinumber_in_dll[1],serinumber_in_dll[2],serinumber_in_dll[3]);
                if(gval[0]==0 && gval[1]==0 && gval[2]==0 && gval[3]==0 && gval[4]==0 && gval[5]==0 /*&& gval[6]==0 && gval[7]==0 && gval[8]==0 && gval[9]==0 && gval[10]==0 && gval[11]==0*/)
                    return -3;
                for(int i=0; i<5; i++)
                    if(gval[i]!=pval[i])
                        return -2;
            }
        }

        //delete []data;
        return 1;
    }
    return -1;//add by Fance
    ///////////////////////////////////////////////////////////
}

OUTPUT int read_multi_tap(TS_UC device_var,TS_US *put_data_into_here,TS_US start_address,int length)
{
    if(g_Commu_type==0)
    {
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting

        TS_UC to_send_data[300]= {'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        TS_UC data_to_send[8]= {'\0'}; //the array to used in writefile()
        data_to_send[0]=device_var;//slave address
        data_to_send[1]=3;//function multiple
        data_to_send[2]=start_address>>8 & 0xff;//starting address hi
        data_to_send[3]=start_address & 0xff;//starting address lo
        data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[5]=length & 0xff;//quantity of registers lo
        TS_US crc=CRC16(data_to_send,6);
        data_to_send[6]=(crc>>8) & 0xff;
        data_to_send[7]=crc & 0xff;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
        if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_mulwrite))==NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0 ;
        ///////////////////////////////////////////////////////send the to read message



        int fState=WriteFile(m_hSerial,// ��� - Handle
                             data_to_send,// ���ݻ�������ַ - Data buffer address
                             8,// ���ݴ�С - Data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                             &m_osMulWrite);
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read

        Sleep(LATENCY_TIME_COM);
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        ////////////////////////////////////////////////clear com error

        fState=ReadFile(m_hSerial,// ��� - Handle
                        to_send_data,// ���ݻ�������ַ - Data buffer address
                        length*2+5,// ���ݴ�С - Data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - Does not support overlap
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
            return -2;
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
            return -2;
        if(to_send_data[length*2+4]!=(crc & 0xff))
            return -2;
        for(int i=0; i<length; i++)
            put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
        return length;
    }
    if(g_Commu_type==1)//tcp.
    {
        /*
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting
        TS_UC to_send_data[700]={'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        TS_UC data_to_send[8]={'\0'};//the array to used in writefile()
        data_to_send[0]=device_var;//slave address
        data_to_send[1]=3;//function multiple
        data_to_send[2]=start_address>>8 & 0xff;//starting address hi
        data_to_send[3]=start_address & 0xff;//starting address lo
        data_to_send[4]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[5]=length & 0xff;//quantity of registers lo
        TS_US crc=CRC16(data_to_send,6);
        data_to_send[6]=(crc>>8) & 0xff;
        data_to_send[7]=crc & 0xff;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ���
        if(m_hSocket==INVALID_SOCKET)
        {
        	return -1;
        }
        ::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),MSG_OOB);

        int nn=sizeof(to_send_data);
        int nRecv = ::recv(m_hSocket, (char*)to_send_data, length*2+5, 0);

        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
        	return -2;
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
        	return -2;
        if(to_send_data[length*2+4]!=(crc & 0xff))
        	return -2;
        for(int i=0;i<length;i++)
        	put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
        return length;
        */
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting

        TS_UC to_send_data[700]= {'\0'};
        TS_UC to_Reive_data[700]= {'\0'};
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        TS_UC data_to_send[12]= {'\0'}; //the array to used in writefile()
        data_to_send[0]= trans_high_id;
        data_to_send[1]= trans_low_id;
        data_to_send[2]=0;
        data_to_send[3]=0;
        data_to_send[4]=0;
        data_to_send[5]=6;



        data_to_send[6]=device_var;//slave address
        data_to_send[7]=3;//function multiple
        data_to_send[8]=start_address>>8 & 0xff;//starting address hi
        data_to_send[9]=start_address & 0xff;//starting address lo
        data_to_send[10]=length >> 8 & 0xff;//quantity of registers hi
        data_to_send[11]=length & 0xff;//quantity of registers lo

        //TS_US crc=CRC16(data_to_send,6);
        //data_to_send[6]=(crc>>8) & 0xff;
        //data_to_send[7]=crc & 0xff;/

//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - the number of data that has been sent
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        int xx=::send(m_hSocket,(char*)data_to_send,sizeof(data_to_send),0);

        if (xx < 0)
        {
            if (last_connected_port != 0)
            {
                Open_Socket2(last_connected_ip, last_connected_port);
                return -1;
            }
        }

        Sleep(LATENCY_TIME_NET);
        int nn=sizeof(to_Reive_data);
        int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length*2+12, 0);

        if (nRecv < 0)
        {
            unsigned int t1 = GetTickCount();//����ο�ʼǰȡ��ϵͳ����ʱ��(ms); - Get system running time (ms) before the program segment starts
            nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length * 2 + 12, 0);
            unsigned int t2 = GetTickCount();//����ο�ʼǰȡ��ϵͳ����ʱ��(ms); - Get system running time (ms) before the program segment starts
            if (nRecv < 0)
            {
                TRACE(_T("\r\n%u-%u , time  %u - %u = %u\r\n"), start_address, start_address+ length - 1,t2,t1,t2 - t1); //Timeout ��ʱ��;
                int nErr = WSAGetLastError();
                if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ�
                {
                    if (last_connected_port != 0)
                        Open_Socket2(last_connected_ip, last_connected_port);
                }
                Sleep(1);
                return -2;
            }

        }
        


        memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
        ///////////////////////////////////////////////////////////
        if(to_send_data[0]!=device_var || to_send_data[1]!=3 || to_send_data[2]!=length*2)
            return -2;
        /*
        crc=CRC16(to_send_data,length*2+3);
        if(to_send_data[length*2+3]!=((crc>>8) & 0xff))
        	return -2;

        if(to_send_data[length*2+4]!=(crc & 0xff))
        	return -2;
        	*/
        for(int i=0; i<length; i++)
            put_data_into_here[i]=to_send_data[3+2*i]*256+to_send_data[4+2*i];
        return length;
    }
    return -1;
}

OUTPUT int write_multi_tap(TS_UC device_var,TS_UC *to_write,TS_US start_address,int length)
{
    if(g_Commu_type==0)//
    {
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        HCURSOR hc;//load mouse cursor
        TS_UC data_to_write[600]= {'\0'};
        data_to_write[0]=device_var;
        data_to_write[1]=0x10;
        data_to_write[2]=start_address >> 8 & 0xff;
        data_to_write[3]=start_address & 0xff;
        data_to_write[4]=0;
        data_to_write[5]=length;
        data_to_write[6]=length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
        for(int i=0; i<length; i++)
            data_to_write[7+i]=to_write[i];
        //	TS_US crc=CRC16(data_to_write,i+7);
        //	data_to_write[i+7]=crc>>8 & 0xff;
        //	data_to_write[i+8]=crc & 0xff;

        TS_US crc=CRC16(data_to_write,length+7);
        data_to_write[length+7]=crc>>8 & 0xff;
        data_to_write[length+8]=crc & 0xff;

        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //length is the data length,if you want to write 128 bite,the length == 128
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - the number of data that has been sent
        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
        if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_mulwrite))==NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0 ;
        ///////////////////////////////////////////////////////send the to read message
        int fState=WriteFile(m_hSerial,// ��� - handle
                             data_to_write,// ���ݻ�������ַ - data buffer address
                             length+9,// ���ݴ�С - data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                             &m_osMulWrite);
        if(!fState)// ��֧���ص� - Overlapped I/O not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        Sleep(LATENCY_TIME_COM);
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - handle
                        gval,// ���ݻ�������ַ - data buffer address
                        8,// ���ݴ�С - data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - Overlapped I/O not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////////////////////////////////////
        for(int i=0; i<6; i++)
            if(gval[i]!=*(data_to_write+i))
                return -2;
        crc=CRC16(gval,6);
        if(gval[6]!=((crc>>8)&0xff))
            return -2;
        if(gval[7]!=(crc & 0xff))
            return -2;
        return 1;
    }
    if(g_Commu_type==1)//tcp.
    {
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        HCURSOR hc;//load mouse cursor
        TS_UC data_to_write[600]= {'\0'};
        TS_UC data_back_write[600]= {'\0'};
        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        data_to_write[0]= trans_high_id;
        data_to_write[1]= trans_low_id;
        data_to_write[2]=0;
        data_to_write[3]=0;
        data_to_write[4]=0;
        data_to_write[5]=6;

        data_to_write[6]=device_var;
        data_to_write[7]=0x10;
        data_to_write[8]=start_address >> 8 & 0xff;
        data_to_write[9]=start_address & 0xff;
        data_to_write[10]=0;
        data_to_write[11]=length;
        data_to_write[12]=length;//128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
        for(int i=0; i<length; i++)
            data_to_write[13+i]=to_write[i];


        //	1 2 3 4 5 6 2 10 0 c8 0 8 8 5 5 5 5 5 5 5 5
        //	TS_US crc=CRC16(data_to_write,i+7);
        //	data_to_write[i+7]=crc>>8 & 0xff;
        //	data_to_write[i+8]=crc & 0xff;
        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //length is the data length,if you want to write 128 bite,the length == 128
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - the number of data that has been sent
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }

        int n=::send(m_hSocket,(char *)data_to_write,13+length,0);
        int nRecv = ::recv(m_hSocket, (char *)data_back_write,13, 0);
        if(nRecv<0)
        {
            return -2;
        }
        //	memcpy((void*)&to_send_data[0],(void*)&to_Reive_data[6],sizeof(to_Reive_data));
        for(int i=0; i<6; i++)
            if(data_back_write[i+6]!=*(data_to_write+i+6))
                return -2;
        return 1;

    }
    return -1;
}

OUTPUT void SetResponseTime (TS_US Time)
{
    if (g_Commu_type == 0)
    {
        LATENCY_TIME_COM = Time;
    }
    else
    {
        LATENCY_TIME_NET = Time;
    }
}

OUTPUT int SendData (TS_US *to_write,TS_US length,unsigned char *put_senddate_into_here,unsigned char *put_revdata_into_here,int* sendDataLength, int* recvDataLength,BOOL CHECK_CRC)
{
    if(g_Commu_type==0)//
    {
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        HCURSOR hc;//load mouse cursor
        TS_UC data_to_write[600]= {'\0'}; //128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
        for(int i=0; i<length; i++)
            data_to_write[i]=to_write[i];
        TS_US crc ;
        if (CHECK_CRC)
        {
            crc=CRC16(data_to_write,length);
            data_to_write[length]=crc>>8 & 0xff;
            data_to_write[length+1]=crc & 0xff;
        }


        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //length is the data length,if you want to write 128 bite,the length == 128
        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - the number of data that has been sent
        if(m_hSerial==NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);//clear read buffer && write buffer
        ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
        if((m_osMulWrite.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_mulwrite))==NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0 ;

        if (CHECK_CRC)
        {
            length+=2;
        }

        *sendDataLength = length ;


        for (int i = 0; i < *sendDataLength; i++)
        {
            //   *((char*)put_senddate_into_here + i) = data_to_write[i];
            put_senddate_into_here[i] = data_to_write[i];
        }

        ///////////////////////////////////////////////////////send the to read message
        int fState=WriteFile(m_hSerial,// ��� - handle
                             data_to_write,// ���ݻ�������ַ - data buffer address
                             length,// ���ݴ�С - data size
                             &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                             &m_osMulWrite);
        if(!fState)// ��֧���ص� - Overlapped I/O not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osWrite,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }
        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial,&dwErrorFlags,&ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if((m_osRead.hEvent = CreateEvent(NULL,true,false, g_cs_com_section_read))==NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0 ;
        Sleep(LATENCY_TIME_COM);
        ////////////////////////////////////////////////clear com error
        fState=ReadFile(m_hSerial,// ��� - handle
                        gval,    // ���ݻ�������ַ - data buffer address
                        128,      // ���ݴ�С - data size
                        &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
                        &m_osRead);
        if(!fState)// ��֧���ص� - Overlapped I/O not supported
        {
            if(GetLastError()==ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial,&m_osRead,&m_had_send_data_number,TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number=0;
        }

        *recvDataLength = m_had_send_data_number;
        for (int i = 0; i < *recvDataLength; i++)
        {
            //*((char*)put_revdata_into_here + i) = gval[i];
            put_revdata_into_here[i]= gval[i];
        }

        ///////////////////////////////////////////////////////////


        return 1;
    }
    if(g_Commu_type==1)//tcp.
    {
        //the return value ,-2 is wrong
        //the return value == -1 ,no connecting
        HCURSOR hc;//load mouse cursor
        TS_UC data_to_write[600]= {'\0'};
        TS_UC data_back_write[600]= {'\0'};

   



        //128 is better ,if you send more than 128, the ron software will meet some trouble,because it is too long one times,can not finish on time;on time
        for(int i=0; i<length; i++)
            data_to_write[i]=to_write[i];



        TS_US crc ;
        if (CHECK_CRC)
        {
            crc=CRC16(data_to_write,length);
            data_to_write[length]=crc>>8 & 0xff;
            data_to_write[length+1]=crc & 0xff;
        }



        hc = LoadCursor(NULL,IDC_WAIT);
        hc = SetCursor(hc);
        //length is the data length,if you want to write 128 bite,the length == 128
//		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - the number of data that has been sent
        if(m_hSocket==INVALID_SOCKET)
        {
            return -1;
        }
        if (CHECK_CRC)
        {
            length+=2;
        }
        *sendDataLength = length;
        for (int i = 0; i < *sendDataLength; i++)
        {
            *((char*)put_senddate_into_here + i) = data_to_write[i];
        }

        int n=::send(m_hSocket,(char *)data_to_write,length,0);
        Sleep(LATENCY_TIME_NET);
        int nn=sizeof(data_back_write);
        int nRecv = ::recv(m_hSocket, (char*)data_back_write, nn, 0);



        *recvDataLength = nRecv;
        for (int i = 0; i < *recvDataLength; i++)
        {
            *((char*)put_revdata_into_here + i) = data_back_write[i];
        }

        if(nRecv<0)
        {
            return -2;
        }


        return 1;

    }
    return -1;

}


OUTPUT int Modbus_Standard_Read(TS_UC device_var, TS_US *put_data_into_here, int function_code, TS_US start_address, int length,
	unsigned char *put_senddate_into_here,
	unsigned char *put_revdata_into_here,
	int* sendDataLength, int* recvDataLength
)
{
	int responseLength = -2;

	 

	if (g_Commu_type == 0)
	{
		//device_var is the Tstat ID
		//the return value == -1 ,no connecting
		
		TS_UC to_send_data[300] = { '\0' };
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register
		
		TS_UC data_to_send[8] = { '\0' };    //the array to used in writefile()
		data_to_send[0] = device_var;      //slave address
		data_to_send[1] = function_code;  //function multiple
		data_to_send[2] = start_address >> 8 & 0xff;//starting address hi
		data_to_send[3] = start_address & 0xff;//starting address lo
		data_to_send[4] = length >> 8 & 0xff;//quantity of registers hi
		data_to_send[5] = length & 0xff;//quantity of registers lo
		TS_US crc = CRC16(data_to_send, 6);
		data_to_send[6] = (crc >> 8) & 0xff;
		data_to_send[7] = crc & 0xff;

		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - the number of data that has been sent
		if (m_hSerial == NULL)
		{
			return -1;
		}
		////////////////////////////////////////////////clear com error
		COMSTAT ComStat;
		DWORD dwErrorFlags;

		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
																							////////////////////////////////////////////////////////////overlapped declare
		memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
		if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, g_cs_com_section_mulwrite)) == NULL)
			return -2;
		m_osMulWrite.Offset = 0;
		m_osMulWrite.OffsetHigh = 0;
		///////////////////////////////////////////////////////send the to read message
		*sendDataLength = 8;
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_send[i];
		}


		int fState = WriteFile(m_hSerial,// ��� - handle
			data_to_send,// ���ݻ�������ַ - data buffer address
			8,// ���ݴ�С - data size
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
			&m_osMulWrite);
		if (!fState)// ��֧���ص� - Overlapped I/O not supported
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osWrite.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
			}
			else
				m_had_send_data_number = 0;
		}
		///////////////////////////up is write
		/////////////**************down is read

		Sleep(LATENCY_TIME_COM);
		ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
		memset(&m_osRead, 0, sizeof(OVERLAPPED));
		if ((m_osRead.hEvent = CreateEvent(NULL, true, false, g_cs_com_section_read)) == NULL)
			return -2;
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		////////////////////////////////////////////////clear com error

		fState = ReadFile(m_hSerial,// ��� - handle
			to_send_data,// ���ݻ�������ַ - data buffer address
			length * 2 + 5,// ���ݴ�С - data size
			&m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - number of bytes sent
			&m_osRead);
		if (!fState)// ��֧���ص� - Overlapped I/O not supported
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//WaitForSingleObject(m_osRead.hEvent,INFINITE);
				GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
			}
			else
				m_had_send_data_number = 0;
		}

		*recvDataLength = m_had_send_data_number;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = to_send_data[i];
		}


		///////////////////////////////////////////////////////////
		if (to_send_data[0] != device_var || to_send_data[1] != function_code)
			{return -2;}
		responseLength = length;
		if (function_code == 1||function_code == 2)
		{
			 
			if (length % 8 == 0)
			{
				responseLength = length / 8;
			}
			else
			{

				responseLength = length / 8 + 1;
			}
			if (to_send_data[2] != responseLength)
			{
				return -2;
			}
			crc = CRC16(to_send_data, responseLength + 3);
			if (to_send_data[responseLength + 3] != ((crc >> 8) & 0xff))
				return -2;
			if (to_send_data[responseLength  + 4] != (crc & 0xff))
				return -2;
			int count = 0;
			for (int i = 0; i < responseLength; i++)
			{
				bitset<8> BitReg(to_send_data[3 + i]);
				for (int j = 8 * i; j < 8 * (i + 1); j++)
				{
					if (count<length)
					{
						put_data_into_here[j] = BitReg[j - 8 * i];
					}
					count++;
				}
				
			}
			 
		}
		else
		{
			if (to_send_data[2] != responseLength * 2)
			{
				return -2;
			}
			crc = CRC16(to_send_data, responseLength * 2 + 3);
			if (to_send_data[responseLength * 2 + 3] != ((crc >> 8) & 0xff))
				return -2;
			if (to_send_data[responseLength * 2 + 4] != (crc & 0xff))
				return -2;
			for (int i = 0; i < responseLength; i++)
				put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];

		}
		
		
		
		
		return length;
	}
	if (g_Commu_type == 1)//tcp.
	{
		TS_UC to_send_data[700] = { '\0' };
		TS_UC to_Reive_data[700] = { '\0' };
		HCURSOR hc;//load mouse cursor
		hc = LoadCursor(NULL, IDC_WAIT);
		hc = SetCursor(hc);
		//to_send_data is the array that you want to put data into
		//length is the number of register,that you want to read
		//start_address is the start register
	 
		TS_UC data_to_send[12] = { '\0' }; //the array to used in writefile()
		++g_data_to_send[1];
		if (g_data_to_send[1] % 256 == 0)
		{
			++g_data_to_send[0];
			g_data_to_send[1] = 0;
		}

        trans_id++;
        trans_high_id = (trans_id >> 8) & 0xff;
        trans_low_id = trans_id & 0xff;
        data_to_send[0] = trans_high_id;
        data_to_send[1] = trans_low_id;
        data_to_send[2] = 0;
        data_to_send[3] = 0;
        data_to_send[4] = 0;
        data_to_send[5] = 6;

		//data_to_send[0] = 1;
		//data_to_send[1] = 2;
		//data_to_send[2] = 3;
		//data_to_send[3] = 4;
		//data_to_send[4] = 5;
		//data_to_send[5] = 6;

// 		TS_UC data_to_send[8] = { '\0' };    //the array to used in writefile()
// 		data_to_send[0] = device_var;      //slave address
// 		data_to_send[1] = function_code;  //function multiple
// 		data_to_send[2] = start_address >> 8 & 0xff;//starting address hi
// 		data_to_send[3] = start_address & 0xff;//starting address lo
// 		data_to_send[4] = length >> 8 & 0xff;//quantity of registers hi
// 		data_to_send[5] = length & 0xff;//quantity of registers lo
// 		TS_US crc = CRC16(data_to_send, 6);
// 		data_to_send[6] = (crc >> 8) & 0xff;
// 		data_to_send[7] = crc & 0xff;

		data_to_send[6] = device_var;//slave address
		data_to_send[7] = function_code;
		data_to_send[8] = start_address >> 8 & 0xff;//starting address hi
		data_to_send[9] = start_address & 0xff;//starting address lo
		data_to_send[10] = length >> 8 & 0xff;//quantity of registers hi
		data_to_send[11] = length & 0xff;//quantity of registers lo

										 //TS_US crc=CRC16(data_to_send,6);
										 //data_to_send[6]=(crc>>8) & 0xff;
										 //data_to_send[7]=crc & 0xff;/

										 //		DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - the number of data that has been sent
		if (m_hSocket == INVALID_SOCKET)
		{
			return -1;
		}
		*sendDataLength = sizeof(data_to_send);
		for (int i = 0; i < *sendDataLength; i++)
		{
			*((char*)put_senddate_into_here + i) = data_to_send[i];
		}
		::send(m_hSocket, (char*)data_to_send, sizeof(data_to_send), 0);

		Sleep(LATENCY_TIME_NET);
		int nn = sizeof(to_Reive_data);
		int nRecv = ::recv(m_hSocket, (char*)to_Reive_data, length * 2 + 12, 0);

        if (nRecv < 0)
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����� - Remote host forcibly closed an existing connection
            {
                if (last_connected_port != 0)
                    Open_Socket2(last_connected_ip, last_connected_port);
            }
            return -2;
        }
		*recvDataLength = nRecv;
		for (int i = 0; i < *recvDataLength; i++)
		{
			*((char*)put_revdata_into_here + i) = to_Reive_data[i];
		}

		memcpy((void*)&to_send_data[0], (void*)&to_Reive_data[6], sizeof(to_Reive_data));
		///////////////////////////////////////////////////////////
		if (to_send_data[0] != device_var || to_send_data[1] != function_code )
			return -2;
		 
		responseLength = length;
		if (function_code == 1 || function_code == 2)
		{

			if (length % 8 == 0)
			{
				responseLength = length / 8;
			}
			else
			{

				responseLength = length / 8 + 1;
			}
			if (to_send_data[2] != responseLength)
			{
				return -2;
			}
			 
			int count = 0;
			for (int i = 0; i < responseLength; i++)
			{
				bitset<8> BitReg(to_send_data[3 + i]);
				for (int j = 8 * i; j < 8 * (i + 1); j++)
				{
					if (count < length)
					{
						put_data_into_here[j] = BitReg[j - 8 * i];
					}
					count++;
				}

			}

		}
		else
		{
			if (to_send_data[2] != responseLength * 2)
			{
				return -2;
			}
		 
			for (int i = 0; i < responseLength; i++)
				put_data_into_here[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];

		}




		return length;
 
	}
	return -1;
}


OUTPUT bool Open_Socket_For_All(CString strIPAdress, short nPort)
{
    //if(g_Commu_type==0)
    //	return false;

    int nNetTimeout = 3000;//1 second.
    WSADATA wsaData;
    WORD sockVersion = MAKEWORD(2, 2);

    if (m_hSocket != INVALID_SOCKET)
    {
        ::closesocket(m_hSocket);
        m_hSocket = NULL;
    }

    if (::WSAStartup(sockVersion, &wsaData) != 0)
    {
        //AfxMessageBox(_T("Init Socket failed!"));
        m_hSocket = NULL;
        return FALSE;
    }

    m_hSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_hSocket == INVALID_SOCKET)
    {
        //	AfxMessageBox(_T("Create socket failed!"));
        m_hSocket = NULL;
        return FALSE;
    }
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(nPort);
    USES_CONVERSION;
    //char pTemp[20];
    //pTemp=W2A(strIPAdress);     


    //servAddr.sin_addr.S_un.S_addr =inet_addr("192.168.0.28");
    //	servAddr.sin_addr.S_un.S_addr =inet_addr((LPSTR)(LPCTSTR)strIPAdress);
    servAddr.sin_addr.S_un.S_addr = (inet_addr(W2A(strIPAdress)));
    //	u_long ul=1;
    //	ioctlsocket(m_hSocket,FIONBIO,(u_long*)&ul);
    //����ʱ��
    setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&nNetTimeout, sizeof(int));
    //����ʱ��
    setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
    if (::connect(m_hSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
    {
        DWORD dwErr = WSAGetLastError();
        //AfxMessageBox(_T(" Failed connect() \n"));
        ::closesocket(m_hSocket);
        m_hSocket = NULL;
        return FALSE;
    }
    last_connected_ip = strIPAdress;
    last_connected_port = nPort;
    return TRUE;
}


//У�������ϵ�����,�Ƿ���bacnet ��55  FF �� ����
int check_bacnet_data(unsigned char * buffer, int nlength)
{
    if (nlength <= 2)
        return -1;
    unsigned char * nlast_point = buffer + nlength - 1;
    unsigned char * npoint = buffer;
    int no_token_count = 0; //��������ֽ�û���ѵ�0x55�ĸ��� - Number of consecutive bytes without 0x55
    int token_count = 0;
    while ((npoint - buffer) < (nlength - 1))
    {
        if (no_token_count > 100)
        {
            return -2; //������bacnet 55  ff ���� - No bacnet 55 ff data on the bus
        }
        if (*npoint != 0x55)
        {
            no_token_count++;
            npoint++;
            continue;
        }

        if (*(npoint + 1) != 0xff)
        {
            no_token_count++;
            npoint++;
            continue;
        }
        npoint++;
        token_count++;
        if(token_count >= 3)  //һ֡���ݳ��� 55 FF �ĸ����ﵽ3�����ж�Ϊ�� bacnet mstp���� - If a frame of data contains 3 occurrences of 55 FF, it is determined that bacnet mstp exists
           return 1;
    }
}


int shutdown_test_comport = 0;
OUTPUT int Set_Test_Comport_Status(int command)
{
    if (command == 0)
    {
        shutdown_test_comport = 0;
    }
    else if (command == 1)
    {
        shutdown_test_comport = 1;
    }

    return 0;
}

/* Test_Comport return
    1  �����������ڵ� 55 FF ���� - Found two cycles of 55 FF
    0  �������������� - Represents no data on the bus
    -1 ������ �����Ȳ��� 8���ֽ� - Data exists but is less than 8 bytes
    -2 ���Ӻ�50��token ���� ��û�з��� 55 FF - Monitored more than 50 tokens but did not find 55 FF
    -3 ����곤�ȳ������� ��û�з��� 55 FF - Checked all lengths and did not find 55 FF
    -4 �ֶ���ֹ - Manually terminated
    -100  �����ںŴ��� 100 - Represents a serial port number greater than 100
    -101  ͬ���򿪴���ʧ�� - Failed to open serial port synchronously
    -102  ���ھ��Ϊ�㣬��Ч�Ĵ��� - Serial port handle is zero, invalid serial port
*/
//OUTPUT int Test_Comport(int comport, baudrate_def* ntest_ret)
OUTPUT int Test_Comport(int comport, baudrate_def* ntest_ret, int default_baudrate)
{
    if (comport >= 100)
        return -100;
    if (open_com_nocretical(comport) == false)
    {
        Sleep(1000);

        return -101;
    }
    bool found_bacnet_data = false;
    int n_no_data_online_count = 0;
    for (int i = 0; i < sizeof(ArrayBaudate) / sizeof(ArrayBaudate[0]); i++)
    {
        if (shutdown_test_comport)
        {
            close_com_nocritical(comport);
            return -4;
        }

        if (default_baudrate != 0) //Ĭ�ϲ��� �����ʵ������ �ͼ�����е� ������;
        {
            ntest_ret[i].ncomport = comport;
            ntest_ret[i].test_ret = 0;
            ntest_ret[i].baudrate = default_baudrate;
        }
        else
        {
            ntest_ret[i].ncomport = comport;
            ntest_ret[i].test_ret = 0;
            ntest_ret[i].baudrate = ArrayBaudate[i];
        }

        if (found_bacnet_data)
        {
            ntest_ret[i].test_ret = -2;
            continue;
        }
        //Speed up scan , if no data online ,check 2 times ,then stop.
        //if (n_no_data_online_count >= 10)
        //{
        //    ntest_ret[i].test_ret = 0;
        //    continue;
        //}
        bool change_baudrate_ret = false;
        if (default_baudrate != 0)
            change_baudrate_ret = Change_BaudRate_NoCretical(default_baudrate, comport);
        else
            change_baudrate_ret = Change_BaudRate_NoCretical(ArrayBaudate[i], comport);
        if (change_baudrate_ret == false)
        {
            DWORD nerrors;COMSTAT com_stats;
            ::ClearCommError(m_com_h_serial[comport],&nerrors, &com_stats);
            Sleep(1);
            if (default_baudrate != 0)
                break;
            continue;
        }
        unsigned char test_data[400];
        memset(test_data, 0, 400);
        //read_multi2_nocretical(255, test_data, 2000, 100, 0, comport);

        HANDLE m_hSerial = NULL;
        m_hSerial = m_com_h_serial[comport];
        //device_var is the Tstat ID
        //the return value == -1 ,no connecting

        TS_C to_send_data[600] = { '\0' };
        HCURSOR hc;//load mouse cursor
        hc = LoadCursor(NULL, IDC_WAIT);
        hc = SetCursor(hc);
        //to_send_data is the array that you want to put data into
        //length is the number of register,that you want to read
        //start_address is the start register
        TS_UC data_to_send[8] = { '\0' }; //the array to used in writefile()
        data_to_send[0] = 255;//slave address
        data_to_send[1] = 255;//function multiple
        data_to_send[2] = 0xffff >> 8 & 0xff;//starting address hi
        data_to_send[3] = 0xffff & 0xff;//starting address lo
        data_to_send[4] = 0xffff >> 8 & 0xff;//quantity of registers hi
        data_to_send[5] = 0xffff & 0xff;//quantity of registers lo
        TS_US crc = CRC16(data_to_send, 6);
        data_to_send[6] = (crc >> 8) & 0xff +20;
        data_to_send[7] = crc & 0xff + 20;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if (m_hSerial == NULL)
        {
            if (default_baudrate != 0)
                break;
            continue;
            return -102;
        }

        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;
        int fState;
#if 0
        Sleep(LATENCY_TIME_COM * 3);
        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
                                                                                            ////////////////////////////////////////////////////////////overlapped declare
        CString nSection;
        nSection.Format(_T("MulTestBacnetWrite_%d"), comport);

        memset(&m_com_osMulWrite[comport], 0, sizeof(OVERLAPPED));
        if ((m_com_osMulWrite[comport].hEvent = CreateEvent(NULL, true, false, nSection)) == NULL)
        {
            continue;
            //return -2;
        }
        m_com_osMulWrite[comport].Offset = 0;
        m_com_osMulWrite[comport].OffsetHigh = 0;
        ///////////////////////////////////////////////////////send the to read message
        fState = WriteFile(m_hSerial,// ��� - Handle
            data_to_send,// ���ݻ�������ַ - Data buffer address
            8,// ���ݴ�С - Data size
            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns the number of bytes sent
            &m_com_osMulWrite[comport]);
        if (!fState)// ��֧���ص� - Does not support overlap
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_com_osMulWrite[comport], &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number = 0;
        }
        ///////////////////////////up is write
        /////////////**************down is read

        Sleep(LATENCY_TIME_COM*3);
#endif
        CString nReadSection;
        nReadSection.Format(_T("MulTestBacnetRead_%d_%d"), comport, ntest_ret[i].baudrate);
        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        memset(&m_com_osRead[comport], 0, sizeof(OVERLAPPED));
        if ((m_com_osRead[comport].hEvent = CreateEvent(NULL, true, false, nReadSection)) == NULL)
        {
            if (default_baudrate != 0)
                break;
            continue;
            //return -2;
        }
        m_com_osRead[comport].Offset = 0;
        m_com_osRead[comport].OffsetHigh = 0;

        Sleep(LATENCY_TIME_COM * 3);
        ////////////////////////////////////////////////clear com error
        fState = ReadFile(m_hSerial,// ��� - Handle
            to_send_data,// ���ݻ�������ַ - Data buffer address
            100 * 2 + 5,// ���ݴ�С - Data size
            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns the number of bytes sent
            &m_com_osRead[comport]);
        if (!fState)// ��֧���ص� - Does not support overlap
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_com_osRead[comport], &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number = 0;
        }
        ///////////////////////////////////////////////////////////
        //if (to_send_data[0] != device_var || to_send_data[1] != 3 || to_send_data[2] != length * 2)
        //    return -2;
        //crc = CRC16(to_send_data, length * 2 + 3);
        //if (to_send_data[length * 2 + 3] != ((crc >> 8) & 0xff))
        //    return -2;
        //if (to_send_data[length * 2 + 4] != (crc & 0xff))
        //    return -2;
        memcpy(test_data, to_send_data, 200);
        //for (int i = 0; i<200; i++)
        //    test_data[i] = to_send_data[3 + 2 * i] * 256 + to_send_data[4 + 2 * i];



        if (m_had_send_data_number > 0)
        {
            n_no_data_online_count = 0;
            ntest_ret[i].test_ret = check_bacnet_data((unsigned char *)test_data, m_had_send_data_number);
            if (default_baudrate != 0)
            {
                if (ntest_ret[i].test_ret == 1)
                    found_bacnet_data = true;
                else
                    found_bacnet_data = false;
                break;
            }
            if (ntest_ret[i].test_ret != 1)
                continue;
            found_bacnet_data = true;
        }
        else
        {
            n_no_data_online_count++;
            ntest_ret[i].test_ret = 0;
            if (default_baudrate != 0)
                break;
            continue;
        }

    }
#if 0
    HANDLE m_hSerial = NULL;
    m_hSerial = m_com_h_serial[comport];

    COMSTAT  stat;
    DWORD error;

    bool found_bacnet_data = false;
    for (int i = 0; i < sizeof(ArrayBaudate) / sizeof(ArrayBaudate[0]); i++)
    {
        ntest_ret[i].baudrate = ArrayBaudate[i];
        ntest_ret[i].test_ret = 0;
        if (found_bacnet_data)
        {
            ntest_ret[i].test_ret = -2;
            continue;
        }
        Change_BaudRate_NoCretical(ArrayBaudate[i], comport);


        char buf[1024];
        memset(buf, 0, 1024);
        int buf_len = 1024;
        unsigned long r_len = 0;
        buf[0] = '\0';


        for (int j = 0;j < 30;j++)
        {
            Sleep(100);
            if (ClearCommError(m_hSerial, &error, &stat) && error > 0)    //������� - Clear errors
            {
                PurgeComm(m_hSerial, PURGE_RXABORT | PURGE_RXCLEAR); /*������뻺����*/ - Clear input buffer
                continue;
            }

            if (!stat.cbInQue)// ������������ - Buffer is empty
                continue;

            buf_len = min((int)(buf_len - 1), (int)stat.cbInQue);

            memset(&m_osRead, 0, sizeof(OVERLAPPED));
            CString nSection;
            nSection.Format(_T("ReadTestCom_%d"), comport);

            if ((m_osRead.hEvent = CreateEvent(NULL, true, false, nSection)) == NULL)
                continue;
            m_osRead.Offset = 0;
            m_osRead.OffsetHigh = 0;


            if (!ReadFile(m_hSerial, buf, buf_len, &r_len, &m_osRead)) //2000 �� ReadFile ʼ�շ��� True - Under 2000, ReadFile always returns True
            {
                if (GetLastError() == ERROR_IO_PENDING) // �����첽I/O - End asynchronous I/O
                {
                    if (!GetOverlappedResult(m_hSerial, &m_osRead, &r_len, false))
                    {
                        if (GetLastError() != ERROR_IO_INCOMPLETE)//�������� - Other errors
                            r_len = 0;
                    }
                }
                else
                    r_len = 0;
            }
            if (r_len < 20)
                continue;
            buf[r_len] = '\0';
            break;
        }

        if (r_len > 0)
        {
            ntest_ret[i].test_ret = check_bacnet_data((unsigned char *)buf, buf_len);
            if (ntest_ret[i].test_ret != 1)
                continue;
            found_bacnet_data = true;

        }

    }
#endif
    


    close_com_nocritical(comport);
    return found_bacnet_data;

}



OUTPUT int Check_Mstp_Comport(int comport, baudrate_def* ntest_ret, int default_baudrate)
{
    if (comport >= 100)
        return -100;
    if (open_com_nocretical(comport, default_baudrate) == false)
    {
        Sleep(1000);

        return -101;
    }
    // ��մ��ڻ����� - Clear serial port buffer
    PurgeComm(m_com_h_serial[comport], PURGE_RXCLEAR | PURGE_TXCLEAR);
    DWORD nerrors1; COMSTAT com_stats1;
    ::ClearCommError(m_com_h_serial[comport], &nerrors1, &com_stats1);

    bool found_bacnet_data = false;
    int n_no_data_online_count = 0;
    int found_mstp_data = false;
    for (int i = 0; i < 10; i++)
    {
        if (found_mstp_data)
            break;
        for (int i = 0; i < sizeof(ArrayBaudate) / sizeof(ArrayBaudate[0]); i++)
        {
            if (shutdown_test_comport)
            {
                close_com_nocritical(comport);
                return -4;
            }

            if (default_baudrate != 0) //Ĭ�ϲ��� �����ʵ������ �ͼ�����е� ������; - In the case where the default baud rate is not passed, all baud rates are detected.
            {
                ntest_ret[i].ncomport = comport;
                ntest_ret[i].test_ret = 0;
                ntest_ret[i].baudrate = default_baudrate;
            }
            else
            {
                ntest_ret[i].ncomport = comport;
                ntest_ret[i].test_ret = 0;
                ntest_ret[i].baudrate = ArrayBaudate[i];
            }

            if (found_bacnet_data)
            {
                found_mstp_data = 1;
                ntest_ret[i].test_ret = 1;
                break;
            }
            //Speed up scan , if no data online ,check 2 times ,then stop.
            //if (n_no_data_online_count >= 10)
            //{
            //    ntest_ret[i].test_ret = 0;
            //    continue;
            //}
#if 0
            bool change_baudrate_ret = false;
            if (default_baudrate != 0)
                change_baudrate_ret = Change_BaudRate_NoCretical(default_baudrate, comport);
            else
                change_baudrate_ret = Change_BaudRate_NoCretical(ArrayBaudate[i], comport);
            if (change_baudrate_ret == false)
            {
                DWORD nerrors; COMSTAT com_stats;
                ::ClearCommError(m_com_h_serial[comport], &nerrors, &com_stats);
                Sleep(1);
                if (default_baudrate != 0)
                    break;
                continue;
            }
#endif
            unsigned char test_data[400];
            memset(test_data, 0, 400);
            //read_multi2_nocretical(255, test_data, 2000, 100, 0, comport);

            HANDLE m_hSerial = NULL;
            m_hSerial = m_com_h_serial[comport];
            //device_var is the Tstat ID
            //the return value == -1 ,no connecting

            TS_C to_send_data[600] = { '\0' };
            HCURSOR hc;//load mouse cursor
            hc = LoadCursor(NULL, IDC_WAIT);
            hc = SetCursor(hc);
            //to_send_data is the array that you want to put data into
            //length is the number of register,that you want to read
            //start_address is the start register
            TS_UC data_to_send[8] = { '\0' }; //the array to used in writefile()
            data_to_send[0] = 255;//slave address
            data_to_send[1] = 255;//function multiple
            data_to_send[2] = 0xffff >> 8 & 0xff;//starting address hi
            data_to_send[3] = 0xffff & 0xff;//starting address lo
            data_to_send[4] = 0xffff >> 8 & 0xff;//quantity of registers hi
            data_to_send[5] = 0xffff & 0xff;//quantity of registers lo
            TS_US crc = CRC16(data_to_send, 6);
            data_to_send[6] = (crc >> 8) & 0xff + 20;
            data_to_send[7] = crc & 0xff + 20;

            DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
            if (m_hSerial == NULL)
            {
                if (default_baudrate != 0)
                    break;
                continue;
                return -102;
            }

            ////////////////////////////////////////////////clear com error
            COMSTAT ComStat;
            DWORD dwErrorFlags;
            int fState;

            memset(&m_com_osRead[comport], 0, sizeof(OVERLAPPED));
#if 0
            CString nReadSection;
            nReadSection.Format(_T("MulTestBacnetRead_%d_%d"), comport, ntest_ret[i].baudrate);
            ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
            memset(&m_com_osRead[comport], 0, sizeof(OVERLAPPED));
            if ((m_com_osRead[comport].hEvent = CreateEvent(NULL, true, false, nReadSection)) == NULL)
            {
                if (default_baudrate != 0)
                    break;
                continue;
                //return -2;
            }
#endif
            m_com_osRead[comport].Offset = 0;
            m_com_osRead[comport].OffsetHigh = 0;

            Sleep(LATENCY_TIME_COM * 5);
            ////////////////////////////////////////////////clear com error
            fState = ReadFile(m_hSerial,// ��� - Handle
                to_send_data,// ���ݻ�������ַ - Data buffer address
                100 * 2 + 5,// ���ݴ�С - Data size
                &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
                &m_com_osRead[comport]);
            if (!fState)// ��֧���ص� - Does not support overlap
            {
                if (GetLastError() == ERROR_IO_PENDING)
                {
                    //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                    BOOL result = GetOverlappedResult(m_hSerial, &m_com_osRead[comport], &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
                    if (result)
                    {
                        // �����ɹ�����������ֽ��� - Operation succeeded, process the number of bytes transferred
                        TRACE("Bytes transferred: %lu\n", m_had_send_data_number);
                    }
                    else
                    {
                        // ����ʧ�ܣ���ȡ������Ϣ - Operation failed, get error information
                        DWORD error = GetLastError();
                        TRACE("GetOverlappedResult failed with error: %lu\n", error);
                    }
                }
                else
                    m_had_send_data_number = 0;
            }
            memcpy(test_data, to_send_data, 200);
            if (m_had_send_data_number > 0)
            {
                n_no_data_online_count = 0;
                ntest_ret[i].test_ret = check_bacnet_data((unsigned char*)test_data, m_had_send_data_number);
                if (default_baudrate != 0)
                {
                    if (ntest_ret[i].test_ret == 1)
                        found_bacnet_data = true;
                    else
                        found_bacnet_data = false;
                    break;
                }
                if (ntest_ret[i].test_ret != 1)
                    continue;
                found_bacnet_data = true;
                found_mstp_data = true;
            }
            else
            {
                n_no_data_online_count++;
                ntest_ret[i].test_ret = 0;
                if (default_baudrate != 0)
                    break;
                continue;
            }

        }
        Sleep(150);
    }
    close_com_nocritical(comport);
    return found_bacnet_data;

}




#if 1

//OUTPUT void bip_to_modbus_socket(int sock_fd)
//{
//    m_bip_socket  = sock_fd;
//}


OUTPUT int read_ptp_data(unsigned char device_var, unsigned char *put_data_into_here, TS_UC command, TS_UC start_instance, TS_UC end_instance, TS_US entitysize)
{
    if (g_Commu_type == 0)
    {
        //device_var is the modbus ID
        //the return value == -1 ,no connecting
        unsigned short data_length = 0;
        TS_UC read_buffer_data[600] = { '\0' };
        TS_UC data_to_send[16] = { '\0' }; //the array to used in writefile()
        data_to_send[0] = device_var;//slave address
        data_to_send[1] = 0xFF;//function multiple
        data_to_send[2] = 0x54; //T
        data_to_send[3] = 0x45; //E
        data_to_send[4] = 0x4D; //M
        data_to_send[5] = 0x43; //C
        data_to_send[6] = 0x4F; //O

        data_to_send[7] = 0x07;  
        data_to_send[8] = 0x00;  // ��ǰ������ֻ��prg��monitorʹ��
        data_to_send[9] = command;
        data_to_send[10] = start_instance;
        data_to_send[11] = end_instance;
        data_to_send[12] = (entitysize & 0x00ff) ;
        data_to_send[13] = (entitysize & 0xff00)>>8;
        TS_US crc = CRC16(data_to_send, 14);
        data_to_send[14] = (crc >> 8) & 0xff;
        data_to_send[15] = crc & 0xff;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if (m_hSerial == NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
                                                                                            ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
        if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, _T("Write"))) == NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0;
        ///////////////////////////////////////////////////////send the to read message



        int fState = WriteFile(m_hSerial,// ��� - Handle
            data_to_send,// ���ݻ�������ַ - Data buffer address
            16,// ���ݴ�С - Data size
            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
            &m_osMulWrite);
        if (!fState)// ��֧���ص�
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number = 0;
        }
        ///////////////////////////up is write
        /////////////**************down is read

        Sleep(LATENCY_TIME_COM);
        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if ((m_osRead.hEvent = CreateEvent(NULL, true, false, g_cs_com_section_read)) == NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0;
        ////////////////////////////////////////////////clear com error
        if (command == 16)
        {
            data_length = 400;
        }
        else
        {
            data_length = (end_instance - start_instance + 1)*entitysize;
        }

        fState = ReadFile(m_hSerial,// ��� - Handle
            read_buffer_data,// ���ݻ�������ַ - Data buffer address
            data_length + 14 + 2,// ���ݴ�С - Data size
            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
            &m_osRead);
        if (!fState)// ��֧���ص� - Does not support overlap
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number = 0;
        }
        ///////////////////////////////////////////////////////////
        for (int x = 0; x < 14; x++)
        {
            if (read_buffer_data[x] != data_to_send[x])
                return -2;
        }
        crc = CRC16(read_buffer_data, data_length + 14);
        if (read_buffer_data[data_length + 14] != ((crc & 0xff00) >> 8))
            return -2;
        if (read_buffer_data[data_length + 15] != (crc & 0xff))
            return -2;

        for (int i = 0; i < data_length+7; i++)
            put_data_into_here[i] = read_buffer_data[7 + i];
        return data_length+7;
    }
    else if (g_Commu_type == 1)//tcp.
    {
        //device_var is the modbus ID
        //the return value == -1 ,no connecting
        unsigned short data_length = 0;
        TS_UC read_buffer_data[600] = { '\0' };
        TS_UC data_to_send[16 + 6] = { '\0' }; //the array to used in writefile()

        data_to_send[0] = 1;
        data_to_send[1] = 2;
        data_to_send[2] = 3;
        data_to_send[3] = 4;
        data_to_send[4] = 5;
        data_to_send[5] = 6;

        data_to_send[0+6] = device_var;//slave address
        data_to_send[1+6] = 0xFF;//function multiple
        data_to_send[2+6] = 0x54; //T
        data_to_send[3+6] = 0x45; //E
        data_to_send[4+6] = 0x4D; //M
        data_to_send[5+6] = 0x43; //C
        data_to_send[6+6] = 0x4F; //O

        data_to_send[7+6] = 0x07;
        data_to_send[8+6] = 0x00;  // ��ǰ������ֻ��prg��monitorʹ�� - These two are only for prg and monitor
        data_to_send[9+6] = command;
        data_to_send[10+6] = start_instance;
        data_to_send[11+6] = end_instance;
        data_to_send[12+6] = (entitysize & 0x00ff);
        data_to_send[13+6] = (entitysize & 0xff00) >> 8;
        TS_US crc = CRC16(data_to_send + 6 , 14);
        data_to_send[14+6] = (crc >> 8) & 0xff;
        data_to_send[15+6] = crc & 0xff;

        if (m_hSocket == INVALID_SOCKET)
        {
            return -1;
        }

        int nLen = 22;
        int xx = ::send(m_hSocket, (char*)&data_to_send, 22, 0);

        if (xx < 0)
        {
            if (last_connected_port != 0)
                Open_Socket2(last_connected_ip, last_connected_port);
        }
        ////////////////////////////////////////////////clear com error


        Sleep(LATENCY_TIME_NET);//Sleep(SLEEP_TIME);
                                //DataPack rvData;
                                //Sleep(SLEEP_TIME+10);

        TS_UC rvData[600];
        int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);

        if (nRecv < 0)
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ�- The remote host forcibly closed an existing connection.
            {
                if (last_connected_port != 0)
                {
                    Open_Socket2(last_connected_ip, last_connected_port);
                    return -1;
                }
            }
            return -3;
        }
        if (command == 16)
        {
            data_length = 400;
        }
        else
        {
            data_length = (end_instance - start_instance + 1)*entitysize;
        }

        memcpy((void*)read_buffer_data, rvData + 6, data_length + 14 + 2);// ���ݴ�С - Data size

        for (int x = 0; x < 14; x++)
        {
            if (read_buffer_data[x] != data_to_send[x+6])
                return -2;
        }

        crc = CRC16(read_buffer_data, data_length + 14);
        if (read_buffer_data[data_length + 14] != ((crc & 0xff00) >> 8))
            return -2;
        if (read_buffer_data[data_length + 15] != (crc & 0xff))
            return -2;

        for (int i = 0; i < data_length + 7; i++)
            put_data_into_here[i] = read_buffer_data[7 + i];
        return data_length + 7;
    }

    return -1;
}

#if 1
OUTPUT int write_ptp_data(unsigned char device_var, char *to_write, unsigned short nlength)
{
    if (g_Commu_type == 0)//
    {
        unsigned short data_length = 0;
        TS_UC read_buffer_data[20] = { '\0' };
        TS_UC data_to_send[600] = { '\0' }; //the array to used in writefile()
        data_to_send[0] = device_var;//slave address
        data_to_send[1] = 0xFF;//function multiple
        data_to_send[2] = 0x54; //T
        data_to_send[3] = 0x45; //E
        data_to_send[4] = 0x4D; //M
        data_to_send[5] = 0x43; //C
        data_to_send[6] = 0x4F; //O

        memcpy(&data_to_send[7], to_write, nlength);
        TS_US crc = CRC16(data_to_send,7 + nlength);
        data_to_send[nlength + 7] = (crc >> 8) & 0xff;
        data_to_send[nlength + 8] = crc & 0xff;

        DWORD m_had_send_data_number;//�Ѿ����͵����ݵ��ֽ��� - Number of bytes sent
        if (m_hSerial == NULL)
        {
            return -1;
        }
        ////////////////////////////////////////////////clear com error
        COMSTAT ComStat;
        DWORD dwErrorFlags;

        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        PurgeComm(m_hSerial, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);//clear read buffer && write buffer
                                                                                            ////////////////////////////////////////////////////////////overlapped declare
        memset(&m_osMulWrite, 0, sizeof(OVERLAPPED));
        if ((m_osMulWrite.hEvent = CreateEvent(NULL, true, false, g_cs_com_section_mulwrite)) == NULL)
            return -2;
        m_osMulWrite.Offset = 0;
        m_osMulWrite.OffsetHigh = 0;
        ///////////////////////////////////////////////////////send the to read message
        int fState = WriteFile(m_hSerial,// ��� - Handle
            data_to_send,// ���ݻ�������ַ - Data buffer address
            nlength + 9,// ���ݴ�С - Data size
            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
            &m_osMulWrite);
        if (!fState)// ��֧���ص� - Does not support overlap
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osWrite.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_osWrite, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number = 0;
        }
        ///////////////////////////up is write
        /////////////**************down is read
        ClearCommError(m_hSerial, &dwErrorFlags, &ComStat);
        memset(&m_osRead, 0, sizeof(OVERLAPPED));
        if ((m_osRead.hEvent = CreateEvent(NULL, true, false, g_cs_com_section_read)) == NULL)
            return -2;
        m_osRead.Offset = 0;
        m_osRead.OffsetHigh = 0;
        Sleep(LATENCY_TIME_COM);
        fState = ReadFile(m_hSerial,// ��� - Handle
            read_buffer_data,// ���ݻ�������ַ - Data buffer address
            7 + 7 + 2,// ���ݴ�С - Data size
            &m_had_send_data_number,// ���ط��ͳ�ȥ���ֽ��� - Returns number of bytes sent
            &m_osRead);
        if (!fState)// ��֧���ص� - Does not support overlap
        {
            if (GetLastError() == ERROR_IO_PENDING)
            {
                //WaitForSingleObject(m_osRead.hEvent,INFINITE);
                GetOverlappedResult(m_hSerial, &m_osRead, &m_had_send_data_number, TRUE_OR_FALSE);// �ȴ�
            }
            else
                m_had_send_data_number = 0;
        }

        for (int x = 0; x < 14; x++)
        {
            if (read_buffer_data[x] != data_to_send[x])
                return -2;
        }
        crc = CRC16(read_buffer_data, data_length + 14);
        if (read_buffer_data[data_length + 14] != ((crc & 0xff00) >> 8))
            return -2;
        if (read_buffer_data[data_length + 15] != (crc & 0xff))
            return -2;

        return 1;
    }
    else if (g_Commu_type == 1)//tcp.
    {
        //device_var is the modbus ID
        //the return value == -1 ,no connecting
        unsigned short data_length = 0;
        TS_UC read_buffer_data[600] = { '\0' };
        TS_UC data_to_send[600] = { '\0' }; //the array to used in writefile()

        data_to_send[0] = 1;
        data_to_send[1] = 2;
        data_to_send[2] = 3;
        data_to_send[3] = 4;
        data_to_send[4] = 5;
        data_to_send[5] = 6;

        data_to_send[0 + 6] = device_var;//slave address
        data_to_send[1 + 6] = 0xFF;//function multiple
        data_to_send[2 + 6] = 0x54; //T
        data_to_send[3 + 6] = 0x45; //E
        data_to_send[4 + 6] = 0x4D; //M
        data_to_send[5 + 6] = 0x43; //C
        data_to_send[6 + 6] = 0x4F; //O

        memcpy(&data_to_send[7 + 6], to_write, nlength);
        TS_US crc = CRC16(data_to_send + 6, 7 + nlength);
        data_to_send[nlength + 7 + 6] = (crc >> 8) & 0xff;
        data_to_send[nlength + 8 + 6] = crc & 0xff;



        if (m_hSocket == INVALID_SOCKET)
        {
            return -1;
        }

        int nLen = nlength + 8 + 6 + 1;
        int xx = ::send(m_hSocket, (char*)&data_to_send, nLen, 0);

        if (xx < 0)
        {
            if (last_connected_port != 0)
                Open_Socket2(last_connected_ip, last_connected_port);
        }
        ////////////////////////////////////////////////clear com error

        Sleep(LATENCY_TIME_NET);//Sleep(SLEEP_TIME);
                                //DataPack rvData;
                                //Sleep(SLEEP_TIME+10);

        TS_UC rvData[600];
        int nRecv = ::recv(m_hSocket, (char*)&rvData, sizeof(rvData), 0);

        if (nRecv < 0)
        {
            int nErr = WSAGetLastError();
            if (nErr == 10054)   //10054  ������   Զ������ǿ�ȹر���һ�����е����ӡ�- The remote host forcibly closed an existing connection.
            {
                if (last_connected_port != 0)
                {
                    Open_Socket2(last_connected_ip, last_connected_port);
                    return -1;
                }
            }
            return -6;
        }


        memcpy((void*)read_buffer_data, rvData + 6, nlength + 8  + 1);// ���ݴ�С - Data size

        for (int x = 0; x < 14; x++)
        {
            if (read_buffer_data[x] != data_to_send[x+6])
                return -2;
        }
        crc = CRC16(read_buffer_data,  14);
        if (read_buffer_data[ 14] != ((crc & 0xff00) >> 8))
            return -2;
        if (read_buffer_data[ 15] != (crc & 0xff))
            return -2;

        return 1;
    }


}
#endif

#endif




