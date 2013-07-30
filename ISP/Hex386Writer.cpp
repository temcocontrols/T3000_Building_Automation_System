#include "StdAfx.h"
#include "Hex386Writer.h"


extern UINT run_back_ground_flash_thread(LPVOID pParam);
extern int flash_a_tstat(BYTE m_ID, unsigned int the_max_register_number_parameter, TS_UC *register_data_orginal, LPVOID pParam);

CHex386Writer::CHex386Writer(void)
{

}

CHex386Writer::~CHex386Writer(void)
{

}
