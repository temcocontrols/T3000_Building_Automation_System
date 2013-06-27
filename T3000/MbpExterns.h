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
extern int optionsDisplay;
extern int optionsHideAliasColumn;
extern int optionsAddressInCell;

extern int curConf;

extern int pollSlaveId[MAX_GRIDS];
extern int pollFunction[MAX_GRIDS];
extern int pollAddress[MAX_GRIDS];
extern int pollQuantity[MAX_GRIDS];
extern int pollScanRate[MAX_GRIDS];
extern int pollRows[MAX_GRIDS];
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

extern int putDataInGrid1;
extern int putDataInGrid2;
extern int putDataInGrid3;
extern int putDataInGrid4;
extern int putDataInGrid5;

extern int tapDataMode;

extern void testFunc();
extern void LoadReadOneData(int val, unsigned char device_var,unsigned short address);
extern void LoadWriteOneData(int val, unsigned char device_var,unsigned short address);
extern void LoadReadMultiData(unsigned char device_var,unsigned short *put_data_into_here, unsigned short start_address,int length);
extern void LoadWriteMultiData(unsigned char device_var,unsigned char *to_write, unsigned short start_address,int length);

#endif

