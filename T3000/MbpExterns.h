#ifndef _MBPEXTERNS_
#define _MBPEXTERNS_

#include "MbpDefines.h"

extern int noOfGridTables;

extern int optionsSlaveId;
extern int optionsFunction;
extern int optionsAddress;
extern int optionsQuantity;
extern int optionsScanRate;
extern int optionsRows;
extern int optionsSingleFunction;
extern int optionsDisplay;
extern int optionsHideAliasColumn;
extern int optionsAddressInCell;

extern int curConf;

extern int pollSlaveId[MAX_GRIDS];
extern int pollFunction[MAX_GRIDS];
extern int pollAddress[MAX_GRIDS];
extern int pollQuantity[MAX_GRIDS];
extern unsigned int pollScanRate[MAX_GRIDS];
extern int pollRows[MAX_GRIDS];
extern int pollSingleFunction[MAX_GRIDS];
extern int pollDisplay[MAX_GRIDS];
extern int pollHideAliasColumn[MAX_GRIDS];
extern int pollAddressInCell[MAX_GRIDS];
extern int pollConfigured[MAX_GRIDS];
extern int pollShown[MAX_GRIDS];

// mapper variables
extern short int *grid1Data;
extern short int *grid2Data;
extern short int *grid3Data;
extern short int *grid4Data;
extern short int *grid5Data;

extern int dataFlowStarted[5];
extern int putDataInGrid[5];
extern char putDataNow[5];
extern unsigned int commCount[5];
extern int tapDataMode;
extern int MbPollOpen;
extern unsigned int mbpollTotalCount[5];
extern unsigned int mbpollErrCount[5];
//extern CString* regNames[5];
extern CString regNames;
extern int connectionSuccessful;
extern int executeOnce[5];
extern int trafficSlotNo;
extern bool boolTrafficWindowOpen;
extern char sendData[1024];
extern char recvData[1024];
extern int sendDataLen;
extern int recvDataLen;
extern int insertDataString;
extern unsigned int trafficStringIndex;
extern int autoScroll;
extern unsigned int packetCount;

extern void LoadReadOneData(int val, unsigned char device_var,unsigned short address);
extern void LoadWriteOneData(int val, unsigned char device_var,unsigned short address);
extern void LoadReadMultiData(unsigned char device_var,unsigned short *put_data_into_here, unsigned short start_address,int length);
extern void LoadWriteMultiData(unsigned char device_var,unsigned char *to_write, unsigned short start_address,int length);

#endif

