

/**

  Read one value from a modbus device

  @param[out]  value		the value read, cast to an integer
  @param[in]   device_var	the modbus device address
  @param[in]   address		the offset of the value to be read in the device
  @param[in]   retry_times	the number of times to retry on read failure before giving up

  @return  0 if there were no errors
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
