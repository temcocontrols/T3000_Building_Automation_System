/**************************************************************************
*
* Copyright (C) 2005 Steve Karg <skarg@users.sourceforge.net>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*********************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "config.h"
#include "txbuf.h"
#include "bacdef.h"
#include "bacdcode.h"
#include "apdu.h"
#include "npdu.h"
#include "abort.h"
/*#include "arf.h" */
/* demo objects */
#include "ptransfer.h"
#include "mydata.h"
#if defined(BACFILE)
#include "bacfile.h"
#endif
#include "handlers.h"
#include "h_pt_a.h"
/** @file h_pt_a.c  Handles Confirmed Private Transfer Acknowledgment. */
#include "bacstr.h"
#include "ptp.h"
extern uint8_t IOBufferPT[300]; /* Somewhere to build the encoded result block for Private Transfers */

//#include "..\..\BacNetDllforVc\include\ud_str.h"
//#include "..\gloab_variable_extern.h"
//Str_in_point Private_data[100];
int Private_data_length;
extern HWND BacNet_hwd;
static void DecodeBlock(
    char cBlockNum,
    uint8_t * pData)
{
    int iLen;
    uint32_t ulTemp;
    int tag_len;
    uint8_t tag_number;
    uint32_t len_value_type;
    BACNET_CHARACTER_STRING bsName;
    DATABLOCK Response;

    iLen = 0;

    if (cBlockNum >= MY_MAX_BLOCK)
        return;

    tag_len =
        decode_tag_number_and_value(&pData[iLen], &tag_number,
        &len_value_type);
    iLen += tag_len;
    if (tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT)
        return;

    iLen += decode_unsigned(&pData[iLen], len_value_type, &ulTemp);
    Response.cMyByte1 = (char) ulTemp;

    tag_len =
        decode_tag_number_and_value(&pData[iLen], &tag_number,
        &len_value_type);
    iLen += tag_len;
    if (tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT)
        return;

    iLen += decode_unsigned(&pData[iLen], len_value_type, &ulTemp);
    Response.cMyByte2 = (char) ulTemp;

    tag_len =
        decode_tag_number_and_value(&pData[iLen], &tag_number,
        &len_value_type);
    iLen += tag_len;
    if (tag_number != BACNET_APPLICATION_TAG_REAL)
        return;

    iLen += decode_real(&pData[iLen], &Response.fMyReal);

    tag_len =
        decode_tag_number_and_value(&pData[iLen], &tag_number,
        &len_value_type);
    iLen += tag_len;
    if (tag_number != BACNET_APPLICATION_TAG_CHARACTER_STRING)
        return;

    iLen += decode_character_string(&pData[iLen], len_value_type, &bsName);
    strncpy((char *) Response.sMyString, characterstring_value(&bsName),
        MY_MAX_STR);
    Response.sMyString[MY_MAX_STR] = '\0';      /* Make sure it is nul terminated */

    printf("Private Transfer Read Block Response\n");
    printf("Data Block: %d\n", (int) cBlockNum);
    printf("  First Byte  : %d\n", (int) Response.cMyByte1);
    printf("  Second Byte : %d\n", (int) Response.cMyByte2);
    printf("  Real        : %f\n", Response.fMyReal);
    printf("  String      : %s\n\n", Response.sMyString);
}



//#if 0
//bool CM5ProcessPTA(
//    BACNET_PRIVATE_TRANSFER_DATA * data)
//{
//	int i;
//	 int block_length;
//	  char *my_temp_point;
//	  int temp_struct_value;
//
//
//    int iLen;   /* Index to current location in data */
//    uint32_t uiErrorCode;
//    char cBlockNumber;
//    uint32_t ulTemp;
//    int tag_len;
//    uint8_t tag_number;
//    uint32_t len_value_type;
//	BACNET_OCTET_STRING Temp_CS;
//    iLen = 0;
//
//    /* Error code is returned for read and write operations */
//
//    tag_len =
//        decode_tag_number_and_value(&data->serviceParameters[iLen],
//        &tag_number, &len_value_type);
//    iLen += tag_len;
//	 if (tag_number != BACNET_APPLICATION_TAG_OCTET_STRING) {
//   /* if (tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT) {*/
//#if PRINT_ENABLED
//        printf("CPTA: Bad Encoding!\n");
//#endif
//        return 0;
//    }
//    //iLen +=
//    //    decode_unsigned(&data->serviceParameters[iLen], len_value_type,
//    //    &uiErrorCode);
//	 decode_octet_string(&data->serviceParameters[iLen], len_value_type,&Temp_CS);
//	 if((len_value_type - 6)%44!=0)
//		 return 0;	//得到的结构长度错误;
//
//	 block_length=(len_value_type - 6)/44;
//	 Private_data_length = block_length;
//	 my_temp_point = (char *)Temp_CS.value+6;
//	 for (i=0;i<block_length;i++)
//	 {
//		 memcpy_s( Private_data[i].description,21,my_temp_point,21);
//		 my_temp_point=my_temp_point+21;
//		 memcpy_s(Private_data[i].label,9,my_temp_point,9);
//		 my_temp_point=my_temp_point+9;
//
//		 temp_struct_value = my_temp_point[0]<<24 | my_temp_point[1]<<16 | my_temp_point[2]<<8 | my_temp_point[3];
//		 Private_data[i].value = temp_struct_value;
//		 //memcpy_s(Private_data[i].value,4,temp_struct_value,4);
//		 my_temp_point=my_temp_point+4;
//		 Private_data[i].filter = *(my_temp_point++);
//		 Private_data[i].decom	= *(my_temp_point++);
//		 Private_data[i].sen_on	= *(my_temp_point++);;
//		 Private_data[i].sen_off = *(my_temp_point++);
//		 Private_data[i].control = *(my_temp_point++);
//		 Private_data[i].auto_manual = *(my_temp_point++);
//		 Private_data[i].digital_analog = *(my_temp_point++);
//		 Private_data[i].calibration_sign = *(my_temp_point++);
//		 Private_data[i].calibration_increment = *(my_temp_point++);
//		 Private_data[i].unused = *(my_temp_point++);
//		 Private_data[i].calibration = *(my_temp_point++);
//		 Private_data[i].range = *(my_temp_point++);
//		// memcpy_s(Private_data[i].filter,1,my_temp_point,1);
//		/* my_temp_point=my_temp_point+1;
//		 memcpy_s(Private_data[i].decom,1,my_temp_point,1);
//		 my_temp_point=my_temp_point+1;
//		 memcpy_s(Private_data[i].sen_on,1,my_temp_point,1);
//		 my_temp_point=my_temp_point+1;
//		 memcpy_s(Private_data[i].sen_off,1,my_temp_point,1);
//		 my_temp_point=my_temp_point+1;
//		 memcpy_s(Private_data[i].control,1,my_temp_point,1);
//		 my_temp_point=my_temp_point+1;
//		 memcpy_s(Private_data[i].auto_manual,1,my_temp_point,1);
//		 my_temp_point=my_temp_point+1;
//
//		 memcpy_s((void *)Private_data[i].digital_analog,1,my_temp_point,1);
//		 my_temp_point=my_temp_point+1;
//
//		 memcpy_s((void *)Private_data[i].calibration_sign,1,my_temp_point,1);
//		 my_temp_point=my_temp_point+1;
//		 memcpy_s((void *)Private_data[i].calibration_increment,1,my_temp_point,1);
//		 my_temp_point=my_temp_point+1;
//
//		 memcpy_s((void *)Private_data[i].unused,1,my_temp_point,1);
//		 my_temp_point=my_temp_point+1;
//
//		 memcpy_s((void *)Private_data[i].calibration,1,my_temp_point,1);
//		 my_temp_point=my_temp_point+1;
//		 memcpy_s((void *)Private_data[i].range,1,my_temp_point,1);
//		 my_temp_point=my_temp_point+1;*/
//	 }
////		::PostMessage(BacNet_hwd,WM_USER + 200,NULL,NULL);
////	 memcpy_s((void *)Private_data,1,Temp_CS.value+47,1);
//	// Private_data.description
//	 return 1;
//	// decode_character_string(&data->serviceParameters[iLen], len_value_type,&Temp_CS);
//	// iLen +=decode_context_character_string(&data->serviceParameters[iLen],tag_number,&Temp_CS);
//    if (data->serviceNumber == MY_SVC_READ) {   /* Read I/O block so should be full block of data or error */
//        /* Decode the error type and if necessary block number and then fetch the info */
//
//        if (uiErrorCode == MY_ERR_OK) {
//            /* Block Number */
//            tag_len =
//                decode_tag_number_and_value(&data->serviceParameters[iLen],
//                &tag_number, &len_value_type);
//            iLen += tag_len;
//            if (tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT) {
//#if PRINT_ENABLED
//                printf("CPTA: Bad Encoding!\n");
//#endif
//                return 0;
//            }
//
//            iLen +=
//                decode_unsigned(&data->serviceParameters[iLen], len_value_type,
//                &ulTemp);
//            cBlockNumber = (char) ulTemp;
//            DecodeBlock(cBlockNumber, &data->serviceParameters[iLen]);
//        } else {        /* Read error */
//            printf
//                ("Private Transfer read operation returned error code: %lu\n",
//                (unsigned long) uiErrorCode);
//            return 0;
//        }
//    } else {    /* Write I/O block - should just be an OK type message */
//        printf("Private Transfer write operation returned error code: %lu\n",
//            (unsigned long) uiErrorCode);
//    }
//}
//#endif

#if 0
 void ProcessPTA(
    BACNET_PRIVATE_TRANSFER_DATA * data)
{
	int i;
	 int block_length;
	  char *my_temp_point;
	  int temp_struct_value;


    int iLen;   /* Index to current location in data */
    uint32_t uiErrorCode;
    char cBlockNumber;
    uint32_t ulTemp;
    int tag_len;
    uint8_t tag_number;
    uint32_t len_value_type;
	BACNET_OCTET_STRING Temp_CS;
    iLen = 0;

    /* Error code is returned for read and write operations */

    tag_len =
        decode_tag_number_and_value(&data->serviceParameters[iLen],
        &tag_number, &len_value_type);
    iLen += tag_len;
	 if (tag_number != BACNET_APPLICATION_TAG_OCTET_STRING) {
   /* if (tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT) {*/
#if PRINT_ENABLED
        printf("CPTA: Bad Encoding!\n");
#endif
        return;
    }
    //iLen +=
    //    decode_unsigned(&data->serviceParameters[iLen], len_value_type,
    //    &uiErrorCode);
	 decode_octet_string(&data->serviceParameters[iLen], len_value_type,&Temp_CS);

	 if((len_value_type - 6)%44!=0)
		 return;	//得到的结构长度错误;

	 block_length=(len_value_type - 6)/44;
	 Private_data_length = block_length;
	 my_temp_point = (char *)Temp_CS.value+6;
	 for (i=0;i<block_length;i++)
	 {
		 memcpy_s( Private_data[i].description,21,my_temp_point,21);
		 my_temp_point=my_temp_point+21;
		 memcpy_s(Private_data[i].label,9,my_temp_point,9);
		 my_temp_point=my_temp_point+9;

		 temp_struct_value = my_temp_point[0]<<24 | my_temp_point[1]<<16 | my_temp_point[2]<<8 | my_temp_point[3];
//		 Private_data[i].value = temp_struct_value;
		 //memcpy_s(Private_data[i].value,4,temp_struct_value,4);
		 my_temp_point=my_temp_point+4;
		 Private_data[i].filter = *(my_temp_point++);
		 Private_data[i].decom	= *(my_temp_point++);
		 Private_data[i].sen_on	= *(my_temp_point++);;
		 Private_data[i].sen_off = *(my_temp_point++);
		 Private_data[i].control = *(my_temp_point++);
		 Private_data[i].auto_manual = *(my_temp_point++);
		 Private_data[i].digital_analog = *(my_temp_point++);
		 Private_data[i].calibration_sign = *(my_temp_point++);
		 Private_data[i].calibration_increment = *(my_temp_point++);
		 Private_data[i].unused = *(my_temp_point++);
		 Private_data[i].calibration = *(my_temp_point++);
		 Private_data[i].range = *(my_temp_point++);
		// memcpy_s(Private_data[i].filter,1,my_temp_point,1);
		/* my_temp_point=my_temp_point+1;
		 memcpy_s(Private_data[i].decom,1,my_temp_point,1);
		 my_temp_point=my_temp_point+1;
		 memcpy_s(Private_data[i].sen_on,1,my_temp_point,1);
		 my_temp_point=my_temp_point+1;
		 memcpy_s(Private_data[i].sen_off,1,my_temp_point,1);
		 my_temp_point=my_temp_point+1;
		 memcpy_s(Private_data[i].control,1,my_temp_point,1);
		 my_temp_point=my_temp_point+1;
		 memcpy_s(Private_data[i].auto_manual,1,my_temp_point,1);
		 my_temp_point=my_temp_point+1;

		 memcpy_s((void *)Private_data[i].digital_analog,1,my_temp_point,1);
		 my_temp_point=my_temp_point+1;

		 memcpy_s((void *)Private_data[i].calibration_sign,1,my_temp_point,1);
		 my_temp_point=my_temp_point+1;
		 memcpy_s((void *)Private_data[i].calibration_increment,1,my_temp_point,1);
		 my_temp_point=my_temp_point+1;

		 memcpy_s((void *)Private_data[i].unused,1,my_temp_point,1);
		 my_temp_point=my_temp_point+1;

		 memcpy_s((void *)Private_data[i].calibration,1,my_temp_point,1);
		 my_temp_point=my_temp_point+1;
		 memcpy_s((void *)Private_data[i].range,1,my_temp_point,1);
		 my_temp_point=my_temp_point+1;*/
	 }
//		::PostMessage(BacNet_hwd,WM_USER + 200,NULL,NULL);
//	 memcpy_s((void *)Private_data,1,Temp_CS.value+47,1);
	// Private_data.description
	 return;
	// decode_character_string(&data->serviceParameters[iLen], len_value_type,&Temp_CS);
	// iLen +=decode_context_character_string(&data->serviceParameters[iLen],tag_number,&Temp_CS);
    if (data->serviceNumber == MY_SVC_READ) {   /* Read I/O block so should be full block of data or error */
        /* Decode the error type and if necessary block number and then fetch the info */

        if (uiErrorCode == MY_ERR_OK) {
            /* Block Number */
            tag_len =
                decode_tag_number_and_value(&data->serviceParameters[iLen],
                &tag_number, &len_value_type);
            iLen += tag_len;
            if (tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT) {
#if PRINT_ENABLED
                printf("CPTA: Bad Encoding!\n");
#endif
                return;
            }

            iLen +=
                decode_unsigned(&data->serviceParameters[iLen], len_value_type,
                &ulTemp);
            cBlockNumber = (char) ulTemp;
            DecodeBlock(cBlockNumber, &data->serviceParameters[iLen]);
        } else {        /* Read error */
            printf
                ("Private Transfer read operation returned error code: %lu\n",
                (unsigned long) uiErrorCode);
            return;
        }
    } else {    /* Write I/O block - should just be an OK type message */
        printf("Private Transfer write operation returned error code: %lu\n",
            (unsigned long) uiErrorCode);
    }
}

#endif
// void ProcessPTA(
//	 BACNET_PRIVATE_TRANSFER_DATA * data)
// {
//	 int iLen;   /* Index to current location in data */
//	 uint32_t uiErrorCode;
//	 char cBlockNumber;
//	 uint32_t ulTemp;
//	 int tag_len;
//	 uint8_t tag_number;
//	 uint32_t len_value_type;
//
//	 iLen = 0;
//
//	 /* Error code is returned for read and write operations */
//
//	 tag_len =
//		 decode_tag_number_and_value(&data->serviceParameters[iLen],
//		 &tag_number, &len_value_type);
//	 iLen += tag_len;
//	 if (tag_number != BACNET_APPLICATION_TAG_CHARACTER_STRING) {
//		 /* if (tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT) {*/
//#if PRINT_ENABLED
//		 printf("CPTA: Bad Encoding!\n");
//#endif
//		 return;
//	 }
//	 iLen +=
//		 decode_unsigned(&data->serviceParameters[iLen], len_value_type,
//		 &uiErrorCode);
//
//	 if (data->serviceNumber == MY_SVC_READ) {   /* Read I/O block so should be full block of data or error */
//		 /* Decode the error type and if necessary block number and then fetch the info */
//
//		 if (uiErrorCode == MY_ERR_OK) {
//			 /* Block Number */
//			 tag_len =
//				 decode_tag_number_and_value(&data->serviceParameters[iLen],
//				 &tag_number, &len_value_type);
//			 iLen += tag_len;
//			 if (tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT) {
//#if PRINT_ENABLED
//				 printf("CPTA: Bad Encoding!\n");
//#endif
//				 return;
//			 }
//
//			 iLen +=
//				 decode_unsigned(&data->serviceParameters[iLen], len_value_type,
//				 &ulTemp);
//			 cBlockNumber = (char) ulTemp;
//			 DecodeBlock(cBlockNumber, &data->serviceParameters[iLen]);
//		 } else {        /* Read error */
//			 printf
//				 ("Private Transfer read operation returned error code: %lu\n",
//				 (unsigned long) uiErrorCode);
//			 return;
//		 }
//	 } else {    /* Write I/O block - should just be an OK type message */
//		 printf("Private Transfer write operation returned error code: %lu\n",
//			 (unsigned long) uiErrorCode);
//	 }
// }



/*
 * This is called when we receive a private transfer packet ack.
 * We parse the response which the remote application generated
 * and decide what to do next...
 */

void handler_conf_private_trans_ack(
    uint8_t * service_request,
    uint16_t service_len,
    BACNET_ADDRESS * src,
    BACNET_CONFIRMED_SERVICE_ACK_DATA * service_data)
{
    BACNET_PRIVATE_TRANSFER_DATA data;
    int len;

/*
 * Note:
 * We currently don't look at the source address and service data
 * but we probably should to verify that the ack is oneit is what
 * we were expecting. But this is just to silence some compiler
 * warnings from Borland.
 */
    src = src;
    service_data = service_data;

    len = 0;



#if PRINT_ENABLED
    printf("Received Confirmed Private Transfer Ack!\n");
#endif

    len = ptransfer_decode_service_request(service_request, service_len, &data);        /* Same decode for ack as for service request! */
    if (len < 0) {
#if PRINT_ENABLED
        printf("cpta: Bad Encoding!\n");
#endif
    }
#if 0	// fance
    ProcessPTA(&data);  /* See what to do with the response */
#endif
    return;
}

#if 0
void PTErrorHandler(
    BACNET_ADDRESS * src,
    uint8_t invoke_id,
    BACNET_ERROR_CLASS error_class,
    BACNET_ERROR_CODE error_code)
{
    /* FIXME: verify src and invoke id */
    (void) src;
    (void) invoke_id;
    printf("BACnet Error: %s: %s\r\n",
        bactext_error_class_name((int) error_class),
        bactext_error_code_name((int) error_code));
    Error_Detected = true;
}
#endif
