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
	// Node name
	CString m_csName;  //�ڵ�����;
	// Node index
	int m_index;  //�ڵ�����;
	// Number of child nodes
	int m_child_count;  //�ӽڵ�ĸ���;
	// Number of inputs
	int m_input_count;  //����ĸ���;
	// Number of outputs
	int m_output_count; //����ĸ���;
	// Number of variables
	int m_variable_count; //�����ĸ���;
	// Node type: group, device, IO, or input
	int m_node_type;   //�ڵ����� ��group ����device ����IO  ���� input
	Str_points m_property;
	HTREEITEM hParent;
	HTREEITEM h_treeitem;
	CBacnetBMD* pfather;
	CBacnetBMD* pchild[255];
	void UpdateCount();
	// Indicates which floor it belongs to in Build view
	int m_floor;  //ָʾBuild view �� ��������һ ��¥
	// Indicates which room it belongs to in Build view
	int m_room;   //ָʾBuild view �� ��������һ ������
};

