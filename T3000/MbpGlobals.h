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
int optionsDisplay = 0;
int optionsHideAliasColumn = 0;
int optionsAddressInCell = 0;

int curConf = 0;

int pollSlaveId[MAX_GRIDS] = {1, 1, 1, 1, 1};
int pollFunction[MAX_GRIDS] = {0};
int pollAddress[MAX_GRIDS] = {0};
int pollQuantity[MAX_GRIDS] = {10, 10, 10, 10, 10};
int pollScanRate[MAX_GRIDS] = {1000, 1000, 1000, 1000, 1000};
int pollRows[MAX_GRIDS] = {0};
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

int putDataInGrid1;
int putDataInGrid2;
int putDataInGrid3;
int putDataInGrid4;
int putDataInGrid5;

int tapDataMode;

#endif

