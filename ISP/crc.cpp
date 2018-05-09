
#include "stdafx.h"
#include "crc.h"
//---------------------------------------------------------------------------

unsigned short CRC16(unsigned char *puchMsg, unsigned char usDataLen)
{
        unsigned char uchCRCHi = 0xFF ; /* high byte of CRC initialized */
        unsigned char uchCRCLo = 0xFF ; /* low byte of CRC initialized */
        unsigned uIndex ; /* will index into CRC lookup table */
        while (usDataLen--) /* pass through message buffer */
        {
                uIndex = uchCRCHi ^ *puchMsg++ ; /* calculate the CRC */
                uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
                uchCRCLo = auchCRCLo[uIndex] ;
        }
        return (uchCRCHi << 8 | uchCRCLo) ;
}


//---------------------------------------------------------------------------
