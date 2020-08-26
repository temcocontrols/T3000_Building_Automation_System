/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2007 Steve Karg

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to:
 The Free Software Foundation, Inc.
 59 Temple Place - Suite 330
 Boston, MA  02111-1307, USA.

 As a special exception, if other files instantiate templates or
 use macros or inline functions from this file, or you compile
 this file and link it with other works to produce a work based
 on this file, this file does not by itself cause the resulting
 work to be covered by the GNU General Public License. However
 the source code for this file must still be made available in
 accordance with section (3) of the GNU General Public License.

 This exception does not invalidate any other reasons why a work
 based on this file might be covered by the GNU General Public
 License.
 -------------------------------------------
####COPYRIGHTEND####*/
#include "datalink.h"
#include "dlmstp.h"
#include "bip.h"
#include "bvlc.h"
#include "ptp.h"

#include <string.h>
#include <stdio.h>
/** @file datalink.c  Optional run-time assignment of datalink transport */

#if defined(BACDL_ALL) || defined FOR_DOXYGEN
/* Function pointers - point to your datalink */

/** Function template to Initialize the DataLink services at the given interface.
 * @ingroup DLTemplates
 * 
 * @note For Linux, ifname is eth0, ath0, arc0, ttyS0, and others.
         For Windows, ifname is the COM port or dotted ip address of the interface.
         
 * @param ifname [in] The named interface to use for the network layer.
 * @return True if the interface is successfully initialized,
 *         else False if the initialization fails.
 */
int m_protocol = 3;

bool(*datalink_init) (char *ifname);

/** Function template to send a packet via the DataLink.
 * @ingroup DLTemplates
 *
 * @param dest [in] Destination address.
 * @param npdu_data [in] The NPDU header (Network) information.
 * @param pdu [in] Buffer of data to be sent - may be null.
 * @param pdu_len [in] Number of bytes in the pdu buffer.
 * @return Number of bytes sent on success, negative number on failure.
 */
#if 1
int datalink_send_pdu (
    BACNET_ADDRESS * dest,
    BACNET_NPDU_DATA * npdu_data,
    uint8_t * pdu,
    unsigned pdu_len)
{
	//m_protocol = 4;
	if(m_protocol == 2)
	{
		dlmstp_send_pdu(dest,npdu_data,pdu,pdu_len);
	}
	else if(m_protocol == 3)
	{
		bip_send_pdu(dest,npdu_data,pdu,pdu_len);
	}
	else if(m_protocol == 4)
	{
		//ptp_send_pdu(dest,npdu_data,pdu,pdu_len);
	}
}

__declspec(dllexport) uint16_t datalink_receive (BACNET_ADDRESS * src, uint8_t * pdu,
    uint16_t max_pdu, unsigned timeout)
{
	if(m_protocol == 3)
	{
		return	bip_receive(src,pdu,max_pdu,timeout);
	}
	else if(m_protocol == 2)
	{
		return dlmstp_receive(src, pdu, max_pdu, 3000);

#if 0
        uint16_t temp_value;
        temp_value =  dlmstp_receive(src, pdu, max_pdu, 3000);


        if (temp_value == 0)
            return temp_value;

        BACNET_ADDRESS temp1;
        memcpy(&temp1, src, sizeof(BACNET_ADDRESS));

        static int n_count = 0;
        n_count++;
        //remove("C:\\log1.txt");
        FILE *st_dout;
        st_dout = fopen("C:\\log1.txt", "a+");
        //fprintf(st_dout, "Count = %d Start\r\n", n_count);

        char tempbuff[200] = { 0 };
//        sprintf(tempbuff, "%u ", n_count);
        char * temp_print_test = NULL;
        temp_print_test = (char *)&temp1;
        for (int i = 0; i< sizeof(BACNET_ADDRESS); i++)
        {
            char temp_char[10] = {0};
            sprintf(temp_char, "%02x ", (unsigned char)*(temp_print_test+i));
            strcat(tempbuff, temp_char);
        }
        fprintf(st_dout, "%s\r\n", tempbuff);
        fflush(st_dout);
        fclose(st_dout);

        return temp_value;
#endif
	}
	
}
#endif
/** Function template to close the DataLink services and perform any cleanup.
 * @ingroup DLTemplates
 */
//void (
//    *datalink_cleanup) (
//    void);

void set_datalink_protocol(int nprotocol)
{
	m_protocol = nprotocol;
}

 void datalink_get_broadcast_address (
    BACNET_ADDRESS * dest)
{
	//and so on
	if(m_protocol == 2)
	{
		dlmstp_get_broadcast_address(dest);
	}
	else if(m_protocol == 3)
	{
		bip_get_broadcast_address(dest);
	}
}

 void datalink_get_my_address (
	BACNET_ADDRESS * my_address)
{
	//if(protocal == BAC_MSTP || protocal == BAC_PTP)
	if((m_protocol == 2) || (m_protocol == 4))	//如果协议时MSTP 或者PTP  就采用mstp的 地址;
	{
		dlmstp_get_my_address(my_address);
	}
	else if(m_protocol == 3)
	{
		bip_get_my_address(my_address);
	}
}

//void datalink_set(
//    char *datalink_string)
//{
//    if (strcasecmp("bip", datalink_string) == 0) {
//        datalink_init = bip_init;
//        datalink_send_pdu = bip_send_pdu;
//        datalink_receive = bip_receive;
//        datalink_cleanup = bip_cleanup;
//        datalink_get_broadcast_address = bip_get_broadcast_address;
//        datalink_get_my_address = bip_get_my_address;
//    } else if (strcasecmp("ethernet", datalink_string) == 0) {
//        datalink_init = ethernet_init;
//        datalink_send_pdu = ethernet_send_pdu;
//        datalink_receive = ethernet_receive;
//        datalink_cleanup = ethernet_cleanup;
//        datalink_get_broadcast_address = ethernet_get_broadcast_address;
//        datalink_get_my_address = ethernet_get_my_address;
//    } else if (strcasecmp("arcnet", datalink_string) == 0) {
//        datalink_init = arcnet_init;
//        datalink_send_pdu = arcnet_send_pdu;
//        datalink_receive = arcnet_receive;
//        datalink_cleanup = arcnet_cleanup;
//        datalink_get_broadcast_address = arcnet_get_broadcast_address;
//        datalink_get_my_address = arcnet_get_my_address;
//    } else if (strcasecmp("mstp", datalink_string) == 0) {
//        datalink_init = dlmstp_init;
//        datalink_send_pdu = dlmstp_send_pdu;
//        datalink_receive = dlmstp_receive;
//        datalink_cleanup = dlmstp_cleanup;
//        datalink_get_broadcast_address = dlmstp_get_broadcast_address;
//        datalink_get_my_address = dlmstp_get_my_address;
//    }
//}
#endif
