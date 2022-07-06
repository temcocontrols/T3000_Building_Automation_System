#pragma once
#include "global_define.h"
#define TYPE_BM_POINT_LIST  255
#define TYPE_BM_GROUP       0
#define TYPE_BM_NODES       1
#define TYPE_BM_INPUT       2
#define TYPE_BM_OUTPUT      3
#define TYPE_BM_VARIABLE    4
#define TYPE_BM_MODULE      5
#define TYPE_BM_VIRTUAL_DEVICE      6
class CBacnetBMD
{
public:
	CBacnetBMD(void);
	~CBacnetBMD(void);
	CString m_csName;  //节点名字;
	int m_index;  //节点的序号;
	int m_child_count;  //子节点的个数;
	int m_input_count;  //输入的个数;
	int m_output_count; //输出的个数;
	int m_variable_count; //变量的个数;
	int m_node_type;   //节点类型 是group 还是device 还是IO  还是 input
	Str_points m_property;
	HTREEITEM hParent;
	HTREEITEM h_treeitem;
	CBacnetBMD* pfather;
	CBacnetBMD* pchild[255];
	void UpdateCount();
	int m_floor;  //指示Build view 中 隶属于哪一 层楼
	int m_room;   //指示Build view 中 隶属于哪一 个房间
};

