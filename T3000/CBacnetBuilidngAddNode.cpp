// CBacnetBuilidngAddNode.cpp: 实现文件
//
#include "stdafx.h"

#include "T3000.h"
#include "CBacnetBuilidngAddNode.h"
#include "afxdialogex.h"
#include "CBacnetBuildingRoomEditor.h"
#include "CBacnetBMD.h"
BM_dlg_ret dlg_ret;
// CBacnetBuilidngAddNode 对话框

CString BM_Cstring_Type[]
{
	_T("Group"),
	_T("Node"),
	_T("Input"),
	_T("Output"),
	_T("Variable"),
	_T("Moudle")
};
const int MOUDLE_FAN_1 = 0;
const int MOUDLE_FAN_2 = 1;
CString BM_Cstring_Moudle[]
{
	_T("Fan Example 1"),
	_T("Fan Example 2")
};



CString BM_Cstring_Function[]
{
	_T("Add"),
	_T("Delete"),
	_T("Rename")
};

IMPLEMENT_DYNAMIC(CBacnetBuilidngAddNode, CDialogEx)

CBacnetBuilidngAddNode::CBacnetBuilidngAddNode(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_BACNET_BUILDING_HANDLE_NODES, pParent)
{
	m_function = 255;
	m_type = 255;
}

CBacnetBuilidngAddNode::~CBacnetBuilidngAddNode()
{
}

void CBacnetBuilidngAddNode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBacnetBuilidngAddNode, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BM_NODE_OK, &CBacnetBuilidngAddNode::OnBnClickedButtonBmNodeOk)
	ON_BN_CLICKED(IDC_BUTTON_MANAGE_BUILD, &CBacnetBuilidngAddNode::OnBnClickedButtonManageBuild)
	ON_CBN_SELCHANGE(IDC_COMBO_BM_MOUDLE, &CBacnetBuilidngAddNode::OnCbnSelchangeComboBmMoudle)
END_MESSAGE_MAP()


// CBacnetBuilidngAddNode 消息处理程序

void CBacnetBuilidngAddNode::SetParameter(int nfunction,int ntype)
{
	m_function = nfunction;
	m_type = ntype;
}

void CBacnetBuilidngAddNode::ResetMoudleCount(int nmoudle_type)
{
	switch (nmoudle_type)
	{
	case MOUDLE_FAN_1:
		break;
	case MOUDLE_FAN_2:
		break;
	default:
		break;
	}
}

void CBacnetBuilidngAddNode::InitalUI()
{
	((CComboBox*)GetDlgItem(IDC_COMBO_BM_FUNCTION))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_COMBO_BM_TYPE))->ResetContent();

	((CComboBox*)GetDlgItem(IDC_COMBO_BM_FUNCTION))->AddString(_T("Add"));

	for (int i = 0; i < sizeof(BM_Cstring_Type) / sizeof(BM_Cstring_Type[0]); i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_BM_TYPE))->AddString(BM_Cstring_Type[i]);
	}
	for (int i = 0; i < sizeof(BM_Cstring_Moudle) / sizeof(BM_Cstring_Moudle[0]); i++)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_BM_MOUDLE))->AddString(BM_Cstring_Moudle[i]);
	}

	

	if (m_function != 255)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_BM_FUNCTION))->EnableWindow(false);
		if (m_function == FUNCTION_BM_ADD)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_BM_FUNCTION))->SetWindowText(BM_Cstring_Function[m_function]);
		}
	}
	if (m_type != 255)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_BM_TYPE))->EnableWindow(false);
		if ((m_type < sizeof(BM_Cstring_Type)) && m_type >= 0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_BM_TYPE))->SetWindowText(BM_Cstring_Type[m_type].GetBuffer());
		}

		if (m_type == TYPE_BM_MODULE)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_BM_MOUDLE))->SetWindowText(BM_Cstring_Moudle[0]);
			((CComboBox*)GetDlgItem(IDC_COMBO_BM_MOUDLE))->EnableWindow(1);
			//GetDlgItem(IDC_EDIT_BN_INPUT_COUNT)
		}
		else
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_BM_MOUDLE))->EnableWindow(0);
			

		}
	}

	GetDlgItem(IDC_EDIT_BM_COUNT)->SetWindowText(_T("1"));
	CString temp_name;
	switch (m_type)
	{
	case TYPE_BM_INPUT:
		temp_name = _T("IN");
		break;
	case TYPE_BM_OUTPUT:
		temp_name = _T("OUT");
		break;
	case TYPE_BM_VARIABLE:
		temp_name = _T("VAR");
		break;
	default :
		break;
	}
	GetDlgItem(IDC_EDIT_BM_NODE_NAME)->SetWindowText(temp_name);
}

BOOL CBacnetBuilidngAddNode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitalUI();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CBacnetBuilidngAddNode::OnBnClickedButtonBmNodeOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp_csfunction;
	GetDlgItem(IDC_COMBO_BM_FUNCTION)->GetWindowTextW(temp_csfunction);
	CString temp_cstype;
	GetDlgItem(IDC_COMBO_BM_TYPE)->GetWindowTextW(temp_cstype);
	CString temp_cscount;
	GetDlgItem(IDC_EDIT_BM_COUNT)->GetWindowTextW(temp_cscount);
	CString temp_csname;
	GetDlgItem(IDC_EDIT_BM_NODE_NAME)->GetWindowTextW(temp_csname);
	temp_csname.Trim();
	for (int i = 0; i < sizeof(BM_Cstring_Function) / sizeof(BM_Cstring_Function[0]); i++)
	{
		if (temp_csfunction.CompareNoCase(BM_Cstring_Function[i]) == 0)
		{
			dlg_ret.m_BM_ret_function = i;
			break;
		}
	}

	for (int i = 0; i < sizeof(BM_Cstring_Type) / sizeof(BM_Cstring_Type[0]); i++)
	{
		if (temp_csfunction.CompareNoCase(BM_Cstring_Type[i]) == 0)
		{
			dlg_ret.m_BM_ret_type = i;
			break;
		}
	}

	dlg_ret.m_BM_ret_count = _wtoi(temp_cscount);
	if ((dlg_ret.m_BM_ret_count == 0) || (dlg_ret.m_BM_ret_count > 255))
	{
		MessageBox(_T("Invalid number of nodes added (normal value is 1-255)"));
		dlg_ret.m_bm_add_dlg_resault = 0;
		return;
	}
	
	if ((dlg_ret.m_BM_ret_type == TYPE_BM_INPUT) ||
		(dlg_ret.m_BM_ret_type == TYPE_BM_OUTPUT) ||
		(dlg_ret.m_BM_ret_type == TYPE_BM_VARIABLE))
	{
		if ((temp_csname.GetLength() > 9) || temp_csname.IsEmpty())
		{
			MessageBox(_T("Invalid name length, the length of the name should be less than 9"));
			dlg_ret.m_bm_add_dlg_resault = 0;
			return;
		}
		else
			dlg_ret.m_BM_ret_name = temp_csname;
	}
	else
	{
		if ((temp_csname.GetLength() > 30) || temp_csname.IsEmpty())
		{
			MessageBox(_T("Invalid name length, the length of the name should be less than 30"));
			dlg_ret.m_bm_add_dlg_resault = 0;
			return;
		}
		else
			dlg_ret.m_BM_ret_name = temp_csname;
	}

	dlg_ret.m_bm_add_dlg_resault = 1;
	PostMessage(WM_CLOSE,NULL,NULL);
	
}


void CBacnetBuilidngAddNode::OnBnClickedButtonManageBuild()
{
	// TODO: 在此添加控件通知处理程序代码
	CBacnetBuildingRoomEditor dlg;
	dlg.DoModal();
}


void CBacnetBuilidngAddNode::OnCbnSelchangeComboBmMoudle()
{
	// TODO: 在此添加控件通知处理程序代码
	int example_index = 0;
	CString temp_string;
	int nSel = ((CComboBox*)GetDlgItem(IDC_COMBO_BM_MOUDLE))->GetCurSel();
	((CComboBox*)GetDlgItem(IDC_COMBO_BM_MOUDLE))->GetLBText(nSel, temp_string);
	for (int i = 0; i < sizeof(BM_Cstring_Moudle) / sizeof(BM_Cstring_Moudle[0]); i++)
	{
		if (temp_string.CompareNoCase(BM_Cstring_Moudle[i]) == 0)
		{
			example_index = i;
			break;
		}
	}
	CString input_count;
	CString output_count;
	CString var_count;
	CString prg_count;
	if (example_index == 0)
	{
		input_count = _T("4");
		output_count = _T("7");
		var_count = _T("25");
		prg_count = _T("1");
	}
	else
	{
		input_count = _T("4");
		output_count = _T("7");
		var_count = _T("25");
		prg_count = _T("1");
	}
	GetDlgItem(IDC_EDIT_BN_INPUT_COUNT)->SetWindowTextW(input_count);
	GetDlgItem(IDC_EDIT_BN_OUTPUT_COUNT)->SetWindowTextW(output_count);
	GetDlgItem(IDC_EDIT_BN_VARIABLE_COUNT)->SetWindowTextW(var_count);
	GetDlgItem(IDC_EDIT_BN_PROGRAM_COUNT)->SetWindowTextW(prg_count);
	GetDlgItem(IDC_EDIT_BN_SCHEDULE_COUNT)->SetWindowTextW(_T("0"));
	GetDlgItem(IDC_EDIT_BN_HOLIDAY_COUNT)->SetWindowTextW(_T("0"));

}
