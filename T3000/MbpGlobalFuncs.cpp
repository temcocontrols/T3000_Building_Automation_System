#include "stdafx.h"
#include "MbpExterns.h"
#include "globle_function.h"

int reflectAppStatus = 0;
short int testVar = 0;

void testFunc()
{
	static int a = 0;
	a++;
	srand(time(NULL));

	if (putDataInGrid1 == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			*((short int*)grid1Data + i) = (short int)rand();
		}
		*((short int*)grid1Data + 4) = (short int)testVar;
	}
}

int Read_One(unsigned char device_var,unsigned short address)
{
	int retVal;
	retVal = Read_One_tap(device_var, address);
	if (tapDataMode == 1)
	{
		LoadReadOneData(retVal, device_var, address);
	}
	return retVal;
}

int Write_One(unsigned char device_var, unsigned short address, unsigned short val)
{
	int retVal;
	retVal = Write_One_tap(device_var, address, val);
	if ((!(retVal < 0)) && (tapDataMode == 1))
	{
		LoadWriteOneData(val, device_var, address);
	}
	return retVal;
}

int read_multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length)
{
	int retVal;
	retVal =  read_multi_tap(device_var, put_data_into_here, start_address, length);
	if ((!(retVal < 0)) && (tapDataMode == 1))
	{
		LoadReadMultiData(device_var, put_data_into_here, start_address, length);
	}
	return retVal;
}

int write_multi(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length)
{
	int retVal;
	retVal = write_multi_tap(device_var, to_write, start_address, length);
	if ((!(retVal < 0)) && (tapDataMode == 1))
	{
		LoadWriteMultiData(device_var, to_write, start_address, length);
	}
	return retVal;
}

void LoadReadOneData(int val, unsigned char device_var,unsigned short address)
{

	short* gridData = NULL;
	for (int i = 0; i < 5; i++)
	{
		if (i == 0) gridData = grid1Data;
		if (i == 1) gridData = grid2Data;
		if (i == 2) gridData = grid3Data;
		if (i == 3) gridData = grid4Data;
		if (i == 4) gridData = grid5Data;
		if (gridData != NULL)
		{
			if ((pollConfigured[i] == 1) && (pollShown[i] == 1))
			{
				if ((pollSlaveId[i] == device_var) && (pollQuantity[i] == 1))
				{
					if (pollFunction[i] == 2)
					{
						if (pollAddress[i] == address)
						{
							*((short int*)gridData) = (short int)val;
						}
					}
				}
			}
		}
		gridData == NULL;
	}
}

void LoadWriteOneData(int val, unsigned char device_var,unsigned short address)
{
	short* gridData = NULL;
	for (int i = 0; i < 5; i++)
	{
		if (i == 0) gridData = grid1Data;
		if (i == 1) gridData = grid2Data;
		if (i == 2) gridData = grid3Data;
		if (i == 3) gridData = grid4Data;
		if (i == 4) gridData = grid5Data;
		if (gridData != NULL)
		{
			if ((pollConfigured[i] == 1) && (pollShown[i] == 1))
			{
				if ((pollSlaveId[i] == device_var) && (pollQuantity[i] == 1))
				{
					if (pollFunction[i] == 5)
					{
						if (pollAddress[i] == address)
						{
							*((short int*)gridData) = (short int)val;
						}
					}
				}
			}
		}
		gridData == NULL;
	}
	
}

void LoadReadMultiData(unsigned char device_var,unsigned short *put_data_into_here, unsigned short start_address,int length)
{
	short* gridData = NULL;
	int outsideRange = 1;
	int k = 0;

	for (int i = 0; i < 5; i++)
	{
		if (i == 0) gridData = grid1Data;
		if (i == 1) gridData = grid2Data;
		if (i == 2) gridData = grid3Data;
		if (i == 3) gridData = grid4Data;
		if (i == 4) gridData = grid5Data;
		if (gridData != NULL)
		{
			if ((pollConfigured[i] == 1) && (pollShown[i] == 1))
			{
				if ((pollSlaveId[i] == device_var) && (pollQuantity[i] != 0))
				{
					if (pollFunction[i] == 2)
					{
						for (short int j = start_address; j <= (start_address + length); j++)
						{
							if ((j == (pollAddress[i])) && (outsideRange == 1))
							{
								outsideRange = 0;
							}
							if (outsideRange == 0)
							{
								*((short int*)gridData + k) = *((short int*)put_data_into_here + j);
								k++;
							}
						}
						
						/*if (pollAddress[i] == address)
						{
							*((short int*)gridData) = (short int)val;
						}*/
					}
				}
			}
		}
		gridData == NULL;
	}
}

void LoadWriteMultiData(unsigned char device_var,unsigned char *to_write, unsigned short start_address,int length)
{
	short* gridData = NULL;
	int outsideRange = 1;
	int k = 0;

	for (int i = 0; i < 5; i++)
	{
		if (i == 0) gridData = grid1Data;
		if (i == 1) gridData = grid2Data;
		if (i == 2) gridData = grid3Data;
		if (i == 3) gridData = grid4Data;
		if (i == 4) gridData = grid5Data;
		if (gridData != NULL)
		{
			if ((pollConfigured[i] == 1) && (pollShown[i] == 1))
			{
				if ((pollSlaveId[i] == device_var) && (pollQuantity[i] != 0))
				{
					if (pollFunction[i] == 7)
					{
						for (short int j = start_address; j <= (start_address + length); j++)
						{
							if ((j == (pollAddress[i])) && (outsideRange == 1))
							{
								outsideRange = 0;
							}
							if (outsideRange == 0)
							{
								*((short int*)gridData + k) = *((short int*)to_write + j);
								k++;
							}
						}
					}
				}
			}
		}
		gridData == NULL;
	}
}


