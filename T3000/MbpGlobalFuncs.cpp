#include "stdafx.h"
#include "MbpExterns.h"
#include "globle_function.h"

int reflectAppStatus = 0;
short int testVar = 0;

int Read_One_t(unsigned char device_var,unsigned short address, int slot)
{
	int retVal = -100;
	int executeMBFunc = 0;
	static int readCount = 0;
	
	if (MbPollOpen == 1)
	{
		executeMBFunc = 1;
	}
	else
	{
		executeMBFunc = 0;
	}

	if (executeMBFunc == 1)
	{
		if ((trafficSlotNo == slot) && (trafficSlotNo != -1))
		{
			retVal = Read_One_log(device_var, address, &sendData[0], &recvData[0], &sendDataLen, &recvDataLen);
			insertDataString = 1;
		}
		else
		{
			retVal = Read_One(device_var, address);
		}

		LoadReadOneData(retVal, device_var, address);
		mbpollTotalCount[slot]++;
		if (retVal < 0)
		{
			mbpollErrCount[slot]++;
		}
		/*CString temp;
		readCount++;
		temp.Format(_T("addr = %d count = %d"), address, readCount); 
		MessageBox(NULL, temp, L"Read_One", MB_OK);*/
	}
	return retVal;
}

int Write_One_t(unsigned char device_var, unsigned short address, unsigned short val, int slot)
{
	int retVal = -100;
	int executeMBFunc = 0;
	static int writeCount = 0;

	if (MbPollOpen == 1)
	{
		executeMBFunc = 1;
	}
	else
	{
		executeMBFunc = 0;
	}

	if (executeMBFunc == 1)
	{
		//LoadWriteOneData(val, device_var, address);
		if ((trafficSlotNo == slot) && (trafficSlotNo != -1))
		{
			retVal = Write_One_log(device_var, address, val, &sendData[0], &recvData[0], &sendDataLen, &recvDataLen);
			insertDataString = 1;
		}
		else
		{
			retVal = Write_One(device_var, address, val);
		}

		mbpollTotalCount[slot]++;
		if (retVal < 0)
		{
			mbpollErrCount[slot]++;
		}
		/*CString temp;
		writeCount++;
		temp.Format(_T("addr = %d count = %d ret = %d"), address, writeCount, retVal); 
		MessageBox(NULL, temp, L"Write_One", MB_OK);*/
	}
	return retVal;
}

int read_multi_t(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length,int slot)
{
	int retVal = -100;
	int executeMBFunc = 0;
	static int readCount = 0;

	if (MbPollOpen == 1)
	{
		executeMBFunc = 1;
	}
	else
	{
		executeMBFunc = 0;
	}

	if (executeMBFunc == 1)
	{
		if ((trafficSlotNo == slot) && (trafficSlotNo != -1))
		{
			retVal =  read_multi_log(device_var, put_data_into_here, start_address, length, &sendData[0], &recvData[0], &sendDataLen, &recvDataLen);
			insertDataString = 1;
		}
		else
		{
			retVal =  read_multi(device_var, put_data_into_here, start_address, length);
		}

		if (!(retVal < 0))
		{
			LoadReadMultiData(device_var, put_data_into_here, start_address, length);
		}
		mbpollTotalCount[slot]++;
		if (retVal < 0)
		{
			mbpollErrCount[slot]++;
		}
		/*CString temp;
		readCount++;
		temp.Format(_T("addr = %d count = %d ret = %d"), start_address, readCount, retVal); 
		MessageBox(NULL, temp, L"read_multi", MB_OK);*/
	}
	return retVal;
}

int write_multi_t(unsigned char device_var,unsigned char *to_write,unsigned short start_address,int length, int slot)
{
	int retVal = -100;
	int executeMBFunc = 0;
	static int writeCount = 0;
	
	if (MbPollOpen == 1)
	{
		executeMBFunc = 1;
	}
	else
	{
		executeMBFunc = 0;
	}

	if (executeMBFunc == 1)
	{
		//LoadWriteMultiData(device_var, to_write, start_address, length);
		if ((trafficSlotNo == slot) && (trafficSlotNo != -1))
		{
			retVal = write_multi_log(device_var, to_write, start_address, length, &sendData[0], &recvData[0], &sendDataLen, &recvDataLen);
			insertDataString = 1;
		}
		else
		{
			retVal = write_multi(device_var, to_write, start_address, length);
		}

		mbpollTotalCount[slot]++;
		if (retVal < 0)
		{
			mbpollErrCount[slot]++;
		}
		/*CString temp;
		writeCount++;
		temp.Format(_T("addr = %d count = %d ret = %d"), start_address, writeCount, retVal); 
		MessageBox(NULL, temp, L"write_multi", MB_OK);*/
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
		gridData = NULL;
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
		gridData = NULL;
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
								*((short int*)gridData + k) = *((short int*)put_data_into_here + k);
								k++;
							}
						}
					}
				}
			}
		}
		gridData = NULL;
	}
}

void LoadWriteMultiData(unsigned char device_var, unsigned char *to_write, unsigned short start_address,int length)
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
								*((short int*)gridData + k) = *((short int*)to_write + k);
								k++;
							}
						}
					}
				}
			}
		}
		gridData = NULL;
	}
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