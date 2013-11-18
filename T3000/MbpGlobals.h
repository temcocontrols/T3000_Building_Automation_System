#ifndef _MBPGLOBALS_
#define _MBPGLOBALS_

#include "MbpDefines.h"

int noOfGridTables;

int optionsSlaveId = 1;
int optionsFunction = 0;
int optionsAddress = 0;
int optionsQuantity = 10;
int optionsScanRate = 1000;
int optionsRows = 10;
int optionsSingleFunction = 0;
int optionsDisplay = 0;
int optionsHideAliasColumn = 0;
int optionsAddressInCell = 0;

int curConf = 0;

int pollSlaveId[MAX_GRIDS] = {1, 1, 1, 1, 1};
int pollFunction[MAX_GRIDS] = {0};
int pollAddress[MAX_GRIDS] = {0};
int pollQuantity[MAX_GRIDS] = {10, 10, 10, 10, 10};
unsigned int pollScanRate[MAX_GRIDS] = {1000, 1000, 1000, 1000, 1000};
int pollRows[MAX_GRIDS] = {0};
int pollSingleFunction[MAX_GRIDS] = {0};
int pollDisplay[MAX_GRIDS] = {0};
int pollHideAliasColumn[MAX_GRIDS] = {0};
int pollAddressInCell[MAX_GRIDS] = {0};
int pollConfigured[MAX_GRIDS] = {0};
int pollShown[MAX_GRIDS] = {0};

// mapper variables
short int *grid1Data = NULL;
short int *grid2Data = NULL;
short int *grid3Data = NULL;
short int *grid4Data = NULL;
short int *grid5Data = NULL;

unsigned short prevData1[200] = {0};
unsigned short prevData2[200] = {0};
unsigned short prevData3[200] = {0};
unsigned short prevData4[200] = {0};
unsigned short prevData5[200] = {0};

int firstTime1 = 1;
int firstTime2 = 1;
int firstTime3 = 1;
int firstTime4 = 1;
int firstTime5 = 1;

int dataFlowStarted[5] = {0};
int putDataInGrid[5] = {0};
char putDataNow[5] = {0};
unsigned int commCount[5] = {0};
//int tapDataMode = 1;		// no tapping required
int tapDataMode = 0;
int MbPollOpen = 0;
unsigned int mbpollTotalCount[5] = {0};
unsigned int mbpollErrCount[5] = {0};
//CString* regNames[5] = {0};
CString regNames;
int connectionSuccessful = 0;
int executeOnce[5] = {0};
int trafficSlotNo = -1;
bool boolTrafficWindowOpen = FALSE;
unsigned char sendData[1024] = {0};
unsigned char recvData[1024] = {0};
int sendDataLen = 0;
int recvDataLen = 0;
int insertDataString = 0;
unsigned int trafficStringIndex = 0;
int autoScroll = 0;
unsigned int packetCount = 0;
DWORD hThreadID;
DWORD hDispThreadID;
DWORD hMbCommThreadID;
HANDLE ghSemaphore;
HANDLE displayThreadSemaphore;
HANDLE mbCommThreadSemaphore;
unsigned int temporaryCount;
HANDLE hDispThread;
HANDLE hMbCommThread;
int showedRadioButton = 0;
int runPoll[5] = {1,0,0,0,0};
CString aliasString;
CString regValueString;
CString regAddrString;
int editableValueField;
CString registerNames1 = L"";
CString registerNames2 = L"";
CString registerNames3 = L"";
CString registerNames4 = L"";

CMenu* mbpMenu;
int menuChecked = 0;
int mbPollFirstTime = 1;

#endif

