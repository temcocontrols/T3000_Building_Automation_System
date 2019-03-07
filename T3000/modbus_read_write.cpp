

/**

Read one value from a modbus device

@param[out]  value		the value read, cast to an integer
@param[in]   device_var	the modbus device address
@param[in]   address		the offset of the value to be read in the device
@param[in]   retry_times	the number of times to retry on read failure before giving up

@return  0 if there were no errors

This DOES lock the register_critical_section

  */
int modbus_read_one_value( 
				int& value, 
				unsigned char device_var,
				unsigned short address, 
				int retry_times )
{

	BOOL EnableRefreshTreeView_original_value = g_bEnableRefreshTreeView;

	/** This will prevent a refresh of the tree view while we are doing the read
	 It seems to be a sort of home made mutex.
	 There are all sorts of probems with this, including the fact that the tree refresh
	 is abandoned, not just delayed.
	 But it is used in over 40 places in the rest of the code, so leave it alone
	 */
	g_bEnableRefreshTreeView = false;

	// clear the value read from the device
	value = 0;

	// clear the error return flag
	int error_ret = 0;

	// clear the value/error flag from the modbus DLL
	int j = 0;

	// loop over number of retries requested
	for(int i=0;i<retry_times;i++)
	{
		// ensure no other threads attempt to access modbus communications
		CSingleLock singleLock(&register_critical_section);
		singleLock.Lock();

		// call the modbus DLL method
		j=Read_One(device_var,address);

		// free the modbus communications for other threads
		singleLock.Unlock();

		// increment the number of transmissions we have done
		g_llTxCount++;

		// check for no connection error
		if( j == -1 ) {

			error_ret = j;

			// give up
			break;
		}

		// check for other errors
		if ( j == -2 || j == -3 ) {

			error_ret = j;

			// try again, if more retries available
			continue;
		}


		// increment the number or replies we have received
		g_llRxCount++;

		// set value that we received
		value = (int) j;
		error_ret = j;

		// we have a good value, so no need to try again
		break;

	}

	// Special handling for a particular address  101
	// ( This probably should not be done here! 
	// Presumably it is done because the old version
	// returned a negative value as an error flag
	// and it was neccessary to elude that check )
	if( address == 101 ) {
		if( value > 32767) {
			// assume this is a temperature below zero
			value = 0 - 65535 + value;
		}
	}

	// check for running in the main GUI thread
	if( AfxGetMainWnd()->GetActiveWindow() != NULL ) {

		// construct status message string
		CString str;
		str.Format(_T("Addr:%d [Tx=%d Rx=%d Err=%d]"), 
			device_var, g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);

		//Display it
		((CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(0,str.GetString());

	}

	/**  Restore original value of tree refresh flag

	Note that we do an OR here, so if the flag has been set true somewhere else
	then we will not set it false if it was false when we started.

	*/
	g_bEnableRefreshTreeView |= EnableRefreshTreeView_original_value;

	return error_ret;

}

int Set_Communication_Count(bool b_transmission,int bac_instanceid)
{
	if(b_transmission)
	{
		g_llTxCount++;g_llRxCount++;
	}
	else
	{
		g_llTxCount++;
	}
	if( AfxGetMainWnd()->GetActiveWindow() != NULL ) 
	{

		// construct status message string
		CString str;
		str.Format(_T("Instance ID:%d [Tx=%d Rx=%d Err=%d]"), 
			bac_instanceid, g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);

		//Display it
		((CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(0,str.GetString());

	}
	return 0;
}

/**

  Read multiple values from a modbus device

  @param[out]  put_data_into_here	the values read
  @param[in]   device_var			the modbus device address
  @param[in]   start_address		the offset of thefirt value to be read in the device
  @param[in]   length				number of values to be read
  @param[in]   retry_times			the number of times to retry on read failure before giving up

  @return  negative if there were errors, otherwise length of data read

  This does NOT lock the register_critical_section

  */
int read_multi(unsigned char device_var,unsigned short *put_data_into_here,unsigned short start_address,int length);

int modbus_read_multi_value( 
		unsigned short *put_data_into_here,
		unsigned char device_var,
		unsigned short start_address,
		int length,
		int retry_times )
{
	BOOL EnableRefreshTreeView_original_value = g_bEnableRefreshTreeView;

	/** This will prevent a refresh of the tree view while we are doing the read
	 It seems to be a sort of home made mutex.
	 There are all sorts of probems with this, including the fact that the tree refresh
	 is abandoned, not just delayed.
	 But it is used in over 40 places in the rest of the code, so leave it alone
	 */
	 
	g_bEnableRefreshTreeView = false;
	//TRACE(L"\nMulti -Read -Function -Enter\n");
	int error=0;
	//CSingleLock singleLock(&register_critical_section);
	register_critical_section.Lock() ;
	//TRACE(L"\nMulti -Read -Lock\n");
	for(int i=0;i<retry_times;i++)
	{

		// call the modbus DLL method
		

		// call the modbus DLL method
		error=read_multi(device_var,put_data_into_here,start_address,length);
		       
		// free the modbus communications for other threads
	
		

		// increment the number of transmissions we have done
		g_llTxCount++;

		// accept any return other than -2
		if( error >= 0 )
		{
			// increment the number or replies we have received
			g_llRxCount++;
			break;
		}
	}
	  	register_critical_section.Unlock();
	//TRACE(L"\nMulti -Read -UNLock\n");
	// check for running in the main GUI thread
	if( AfxGetMainWnd()->GetActiveWindow() != NULL ) {

		// construct status message string
		CString str;
		str.Format(_T("Addr:%d [Tx=%d Rx=%d Err=%d]"), 
			device_var, g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);

		//Display it
		((CMFCStatusBar *) AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR))->SetPaneText(0,str.GetString());

	}

	/**  Restore original value of tree refresh flag

	Note that we do an OR here, so if the flag has been set true somewhere else
	then we will not set it false if it was false when we started.

	*/
	g_bEnableRefreshTreeView |= EnableRefreshTreeView_original_value;


	return error;
}




int write_one_org(unsigned char device_var,unsigned short address,short value,int retry_times)
{//retry 
// 	CString str;
// 	str.Format(_T("ID :%d Writing %d"),device_var,address);
// 	SetPaneString(0,str);

    //2018 0606 在底层公共读写函数增加对不同协议的处理
    if (g_protocol == PROTOCOL_MSTP_TO_MODBUS)
    {
        int n_ret = 0;
        for (int i = 0; i < retry_times; i++)
        {
            g_llTxCount++;
            n_ret = WritePrivateBacnetToModbusData(g_mstp_deviceid, address, 1, (unsigned short *)(&value));
            if (n_ret >= 0)
            {
                g_llRxCount++;
                return n_ret;
            }
            Sleep(1000);
        }
        return n_ret;
    }



	short temp_value=value;
	if(address==101 && temp_value<0)
	{//for the temperature is below zero;;;;;;;;-23.3
		temp_value=65535+temp_value;
	}
	int j=0;
	for(int i=0;i<retry_times;i++)
	{
		//register_critical_section.Lock();
		j=Write_One(device_var,address,temp_value);
		multi_register_value[address]=value;//mark***********************
		//register_critical_section.Unlock();

		if(j!=-2 && j!=-3)
		{
			CString str;
			if (j == -1) // no connetiong
			{
				str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);
			}
			else
			{
				str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, ++g_llRxCount, g_llTxCount-g_llRxCount);
			}
			SetPaneString(0,str);
			return j;//return right success
		}
	}
	CString str;
	str.Format(_T("Addr:%d [Tx=%d Rx=%d : Err=%d]"), device_var, ++g_llTxCount, g_llRxCount, g_llTxCount-g_llRxCount);
	SetPaneString(0,str);
	return j;
}
int write_one(unsigned char device_var,unsigned short address,short value,int retry_times)
{
    //2018 0606 在底层公共读写函数增加对不同协议的处理
    if ((g_protocol == PROTOCOL_MSTP_TO_MODBUS) || (g_protocol == PROTOCOL_BIP_T0_MSTP_TO_MODBUS))
    {
        int n_ret = 0;
        for (int i = 0; i < retry_times; i++)
        {
            g_llTxCount++;
            n_ret = WritePrivateBacnetToModbusData(g_mstp_deviceid, address, 1, (unsigned short *)(&value));
            if (n_ret >= 0)
            {
                g_llRxCount++;
                return n_ret;
            }
            Sleep(1000);
        }
        return n_ret;
    }

	BOOL bTemp = g_bEnableRefreshTreeView;
	g_bEnableRefreshTreeView = FALSE;
	int j = write_one_org(device_var,address,value,retry_times);
	g_bEnableRefreshTreeView |= bTemp;
	return j;
}

int write_one_multy_thread(unsigned char device_var, unsigned short address, short value, int retry_times,int nindex)
{
    short temp_value = value;
    int j = 0;
    for (int i = 0;i<retry_times;i++)
    {
        j = Write_One_Multy_Thread(device_var, address, temp_value, nindex);
        if (j != -2 && j != -3)
        {
            return j;//return right success
        }
    }
    return j;
}