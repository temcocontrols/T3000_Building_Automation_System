#pragma once
#include "global_define.h"
#define TYPE_BM_POINT_LIST  255
#define TYPE_BM_GROUP       100
#define TYPE_BM_NODES       101
#define TYPE_BM_MODULE      102
#define TYPE_BM_VIRTUAL_DEVICE      103
#define TYPE_BM_IO          104
//#define TYPE_BM_INPUT       2
//#define TYPE_BM_OUTPUT      3
//#define TYPE_BM_VARIABLE    4

class CBacnetBMD
{
public:
	CBacnetBMD(void);
	~CBacnetBMD(void);
	CString m_csName;  //;
	int m_index;  //;
	int m_child_count;  //;
	int m_input_count;  //;
	int m_output_count; //;
	int m_variable_count; //;
	int m_node_type;   // group device IO   input
	Str_points m_property;
	HTREEITEM hParent;
	HTREEITEM h_treeitem;
	CBacnetBMD* pfather;
	CBacnetBMD* pchild[255];
	void UpdateCount();
	int m_floor;  //Build view   
	int m_room;   //Build view   
};

