#pragma once
#include "global_define.h"
 #include "bado/BADO.h"

extern HWND g_hwnd_now;
//extern   CString   g_database_path; //When Customer use 
extern const CString   g_database_path;                   //When Engineer use

extern Login_Person g_login_user;
extern CBADO g_database_operator; 
extern vector<Product_Table_Struct> g_product_table;
extern Product_Table_Struct g_current_product;
extern vector<Register_Table_Struct> g_registerlist_table;
extern Register_Table_Struct g_current_register;
extern BOOL g_Is_Database_Changed;
extern vector<Login_Person> g_users_DB;