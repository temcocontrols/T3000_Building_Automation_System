// unit_test.cpp : Run unit test of modbus device read code
//

#include "stdafx.h"

// some globals referenced by code to be tested

typedef int                 BOOL;
BOOL g_bEnableRefreshTreeView;
 CCriticalSection  register_critical_section;
int g_llTxCount = 0;
int g_llRxCount = 0;
unsigned short multi_register_value[1024]={-1};

// Stub out some MFC routines referenced by code to be tested

class CMFCStatusBar {
public:
	void SetPaneText( int p1, const wchar_t* c ) { }
};
class AfxMainWndStub
{
public:
	int GetActiveWindow() { return NULL; }
	CMFCStatusBar * GetDescendantWindow(int p ) {
		CMFCStatusBar * stub = new CMFCStatusBar;
		return stub;
	}

};

void SetPaneString( int p1, const CString& p2 ) {}

// access the modbus DLL

#pragma comment(lib,"ModbusDllforVc")
#define INPUT extern "C" __declspec(dllimport)
INPUT int Read_One(unsigned char device_var,unsigned short address);
INPUT int read_multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length);
INPUT int Write_One(unsigned char device_var,unsigned short address,unsigned short value);
INPUT bool open_com(int m_com);

// the code to be tested

#include "../../T3000/modbus_read_write.cpp"

// the test code

/**  Before running this test, some setup is needed.

A null modem is required between COM4 and another unused port.
( I use http://www.aggsoft.com/virtual-null-modem.htm )

A modbus station simulator running on the other end of the null modem.
( My modbus simulator is here: https://github.com/JamesBremner/simodbus )

*/
TEST( modbus_read_one_value )
{
	int value;
	unsigned char device_var = 1;
	unsigned short address = 1;
	int retry_times = 1;

	int ret = modbus_read_one_value( 
				value, 
				device_var,
				address, 
				retry_times );
	// since we ran without opening the port, we should get error return -1
	CHECK_EQUAL( -1, ret );

	// open COM4

	open_com( 4 );
	ret = modbus_read_one_value( 
				value, 
				device_var,
				address, 
				retry_times );
	// the port is now open, so no error return
	CHECK_EQUAL( 0, ret );

	// the simulator always returns a value of 1
	CHECK_EQUAL( 1, value );

	//try addressing a different device
	device_var = 11;
	ret = modbus_read_one_value( 
				value, 
				device_var,
				address, 
				retry_times );
	// the port is now open, so no error return
	CHECK_EQUAL( 0, ret );

	// the simulator always returns a value of 1
	CHECK_EQUAL( 1, value );

}

TEST( modbus_read_multi_value )
{
	unsigned short put_data_into_here[100];
	unsigned char device_var = 1;
	unsigned short start_address = 1;
	int length = 10;
	int retry_times = 1;

	int ret = modbus_read_multi_value( 
		put_data_into_here,
		device_var,
		start_address,
		length,
		retry_times );

	// check we got all the data we reuested
	CHECK_EQUAL( 10, ret);
	// check the values were good
	for( int k = 0; k < 10; k++ ) {
		CHECK_EQUAL( k+1, put_data_into_here[k] );
	}
	// check global counters
	CHECK_EQUAL( 4,  g_llTxCount);
	CHECK_EQUAL( 3, g_llRxCount ); 


}
/**

  Perform one register writes as fast as possible

  @param[in] pParam pointer to value to write

  @return number of writes successfuilly completed

*/
static UINT __cdecl write_one_thread( LPVOID pParam )
{
	unsigned char device_var = 1;
	unsigned short address = 1;
	int retry_times = 1;
	short value = *(short*)pParam;

	int k;
	for( k = 0; k < 100; k++ ) {
		register_critical_section.Lock();
		int ret = write_one(device_var, address, value, retry_times);
		register_critical_section.Unlock();
		if( ret != 1 ) {
			printf("write_one error %d\n",ret);
			return k;
		}
	}
	printf("Completed %d writes\n",k);

	return k;
}
/**

  test if write_one is thread safe

*/
TEST(  write_one )
{
	// basic sanity check
	unsigned char device_var = 1;
	unsigned short address = 1;
	int retry_times = 1;
	short value = 123;
	int ret = write_one(device_var, address, value, retry_times);
	// apparently 1 indicates no errors
	CHECK_EQUAL( 1, ret );

	// start two threads, both doing writes
	value = 0x11;
	CWinThread* pThread = AfxBeginThread(write_one_thread,&value);
	pThread->m_bAutoDelete = false;
	CHECK( pThread );
	short value2 = 0x22;
	CWinThread* pThread2 = AfxBeginThread(write_one_thread,&value2);
	pThread2->m_bAutoDelete = false;
	CHECK( pThread2 );

	// wait for the threads to complete
	HANDLE threads[2];
	threads[0] = pThread->m_hThread;
	threads[1] = pThread2->m_hThread;
	 WaitForMultipleObjects(2, threads, true, INFINITE);

	 // check that all writes were completed successfully
	DWORD exit_code;
	GetExitCodeThread( threads[0],&exit_code );
	CHECK_EQUAL(100,exit_code );
	GetExitCodeThread( threads[1],&exit_code );
	CHECK_EQUAL(100,exit_code );

	delete pThread;
	delete pThread2;


}
int _tmain(int argc, _TCHAR* argv[])
{
	return raven::set::UnitTest::RunAllTests();
}

