#pragma once
#include "../SQLiteDriver/CppSQLite3.h"
const int T3000_5ABCDFG_LED_ADDRESS = 1;
const int T3000_5EH_LCD_ADDRESS = 2;
const int T3000_6_ADDRESS = 3;
const int T3000_T3_MODULES=10;
const int CS3000 = 11;
const int POWER_METER_MODULE = 73;

//UCHAR Mdb_Adress_Map;

class T3000RegAddress
{
public:
	T3000RegAddress(void);
	~T3000RegAddress(void);
	CppSQLite3Table m_table;
	CppSQLite3Query m_q;
	CppSQLite3DB m_SqliteDBT3000;
	bool MatchMoudleAddress(void);
	bool Change_Register_Table(void);

};

//int _G(char *str,int mMdb_Adress_Map=1);
int _P(char *str,int mMdb_Adress_Map=1);

float my_get_tstat_version(unsigned short tstat_id);
float my_get_curtstat_version();
  BOOL DelDirW(CString strSrcPath);
