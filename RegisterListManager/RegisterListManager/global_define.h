#pragma once
#include "global_variable_extern.h"

const COLORREF LIST_ITEM_CHANGED_BKCOLOR = RGB(255,0,0);
const COLORREF LIST_ITEM_DEFAULT_BKCOLOR = GetSysColor(COLOR_WINDOW);
const COLORREF LIST_ITEM_DEFAULT_BKCOLOR_GRAY = RGB(235,235,235);
const COLORREF LIST_ITEM_SELECTED = RGB(150,150,200);

struct Login_Person{
    CString Person_Name;
    CString Person_Roles;
};
struct Product_Table_Struct{
    CString ProductType;
    CString TableName;
    CString Col_RegName;
    CString Col_RegAddress;
    CString Col_RegDescription;
    CString Product_Name;
    CString user_name_login;
};

struct Register_Table_Struct{
    CString Reg_Add;
    CString Reg_Name;
    CString Reg_Dsp;
};