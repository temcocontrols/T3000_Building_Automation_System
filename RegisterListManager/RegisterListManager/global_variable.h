#pragma once
#include "global_define.h"
#include "bado/BADO.h"
#include "global_function.h"

HWND      g_hwnd_now;
const CString   g_database_path = _T("Z:\\TemcoSoftware\\DOCS\\T3000.mdb");
//CString   g_database_path ;//=GetExePath(true)+ _T("\\T3000.mdb");
Login_Person g_login_user;
CBADO g_database_operator; 
BOOL g_Is_Database_Changed;
vector<Product_Table_Struct> g_product_table;
Product_Table_Struct g_current_product;
vector<Register_Table_Struct> g_registerlist_table;
Register_Table_Struct g_current_register;
 vector<Login_Person> g_users_DB;