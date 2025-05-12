#include "stdafx.h"
#include "CBacnetBMD.h"

CBacnetBMD::CBacnetBMD(void)
{
	memset(pchild, 0, 255);
	m_node_type = 0;
	m_input_count    = 0;  //输入的个数;
	m_output_count   = 0; //输出的个数;
	m_variable_count = 0; //变量的个数;
	pfather = NULL;
	m_index = 0;
	h_treeitem = NULL;
	hParent = NULL;
	m_csName.Empty();
	m_floor = 0;
	m_room = 0;
	m_property = { 0 };
}

CBacnetBMD::~CBacnetBMD(void)
{
	//删除子节点
	while (m_child_count != 0)
	{
		delete pchild[0];
		Sleep(1);
	}


	if (pfather == NULL)
		return;

	//重新排序 ，兄弟节点;
	for (int j = m_index; j < pfather->m_child_count; j++)
	{
		pfather->pchild[j] = pfather->pchild[j + 1];
		if(j < pfather->m_child_count - 1)
			pfather->pchild[j]->m_index = j;
	}

	if (pfather->m_child_count > 0)
		pfather->m_child_count--;

}

void CBacnetBMD::UpdateCount()
{
	int temp_input = 0;
	int temp_output = 0;
	int temp_variable = 0;
	for (int i = 0; i < m_child_count; i++)
	{
		if (pchild[i] != NULL)
		{
			if ((pchild[i]->m_node_type == 255)|| //TYPE_BM_POINT_LIST  
				(pchild[i]->m_node_type == 0) )  //TYPE_BM_GROUP
			{
				pchild[i]->UpdateCount();

			}

			temp_input = temp_input + pchild[i]->m_input_count;
			temp_output = temp_output + pchild[i]->m_output_count;
			temp_variable = temp_variable + pchild[i]->m_variable_count;


		}
	}
	m_input_count = temp_input;
	m_output_count = temp_output;
	m_variable_count = temp_variable;
}