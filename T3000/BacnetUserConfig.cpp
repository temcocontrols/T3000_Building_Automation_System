// BacnetUserConfig.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BacnetUserConfig.h"
#include "afxdialogex.h"

#include "global_function.h"
// CBacnetUserConfig dialog
extern bool show_user_list_window ;
IMPLEMENT_DYNAMIC(CBacnetUserConfig, CDialogEx)

CBacnetUserConfig::CBacnetUserConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBacnetUserConfig::IDD, pParent)
{

}

CBacnetUserConfig::~CBacnetUserConfig()
{
}

void CBacnetUserConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USER_LIST, m_user_config_list);
}


BEGIN_MESSAGE_MAP(CBacnetUserConfig, CDialogEx)
	ON_MESSAGE(WM_REFRESH_BAC_USER_NAME_LIST,Fresh_User_List)	
	ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_User_Item)	
	ON_NOTIFY(NM_CLICK, IDC_USER_LIST, &CBacnetUserConfig::OnNMClickUserList)
	ON_BN_CLICKED(IDC_CHECK_USERLIST_INFO, &CBacnetUserConfig::OnBnClickedCheckUserlistInfo)
	ON_BN_CLICKED(IDC_BUTTON_USER_OK, &CBacnetUserConfig::OnBnClickedButtonUserOk)
	ON_BN_CLICKED(IDC_BUTTON_USER_DELETE, &CBacnetUserConfig::OnBnClickedButtonUserDelete)
END_MESSAGE_MAP()


// CBacnetUserConfig message handlers


BOOL CBacnetUserConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_slected_item = -1;	//初始化时 没有选择，从0 开始选择	;
	ok_button_stage = 0;
	
	Initial_List();
	Enable_Window_Stage(HIDE_ALL);
	show_user_list_window = false;	//避免点击左边list的时候 也弹出 配置对话框，只有在 点userlist 按键时弹出;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBacnetUserConfig::Enable_Window_Stage(int nstage)
{
	switch(nstage)
	{
	case HIDE_ENTER_ORIGINAL_PASSWORD:
		{
			GetDlgItem(IDC_STATIC_USER_GROUP_BOX)->EnableWindow(1);
			GetDlgItem(IDC_STATIC_USER_NAME)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_USER_NAME)->EnableWindow(1);
			GetDlgItem(IDC_STATIC_USER_ACCESS_LEVEL)->EnableWindow(1);
			GetDlgItem(IDC_COMBO_ACCESS_LEVEL)->EnableWindow(1);

			GetDlgItem(IDC_STATIC_USER_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_STATIC_TYPE_CHANGE)->EnableWindow(false);

			GetDlgItem(IDC_STATIC_USER_NEW_PASSWORD)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_USER_NEW_PASSWORD)->EnableWindow(1);
			GetDlgItem(IDC_STATIC_USER_RETRY_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_RETYPE_PASSWORD)->EnableWindow(false);

			GetDlgItem(IDC_BUTTON_USER_DELETE)->EnableWindow(true);

			GetDlgItem(IDC_STATIC_USER_NEW_PASSWORD)->SetFocus();
		}
		break;
	case ENTER_ORIGINAL_PASSWORD:
		{

			GetDlgItem(IDC_STATIC_USER_GROUP_BOX)->EnableWindow(1);
			GetDlgItem(IDC_STATIC_USER_NAME)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_USER_NAME)->EnableWindow(1);
			GetDlgItem(IDC_STATIC_USER_ACCESS_LEVEL)->EnableWindow(1);
			GetDlgItem(IDC_COMBO_ACCESS_LEVEL)->EnableWindow(1);

			GetDlgItem(IDC_STATIC_USER_PASSWORD)->EnableWindow(1);
			GetDlgItem(IDC_EDIT_USER_PASSWORD)->EnableWindow(1);
			GetDlgItem(IDC_STATIC_TYPE_CHANGE)->EnableWindow(1);

			GetDlgItem(IDC_STATIC_USER_NEW_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_NEW_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_STATIC_USER_RETRY_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_RETYPE_PASSWORD)->EnableWindow(false);

			GetDlgItem(IDC_BUTTON_USER_DELETE)->EnableWindow(false);

			GetDlgItem(IDC_EDIT_USER_PASSWORD)->SetFocus();
		}
		break;
	case ENTER_NEW_PASSWORD:
		{
			GetDlgItem(IDC_STATIC_USER_NEW_PASSWORD)->EnableWindow(true);
			GetDlgItem(IDC_EDIT_USER_NEW_PASSWORD)->EnableWindow(true);
			GetDlgItem(IDC_STATIC_USER_RETRY_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_RETYPE_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_NEW_PASSWORD)->SetFocus();

			GetDlgItem(IDC_BUTTON_USER_DELETE)->EnableWindow(true);
			
		}
		break;
	case RETYPE_NEW_PASSWORD:
		{
			GetDlgItem(IDC_STATIC_USER_NEW_PASSWORD)->EnableWindow(true);
			GetDlgItem(IDC_EDIT_USER_NEW_PASSWORD)->EnableWindow(true);
			GetDlgItem(IDC_STATIC_USER_RETRY_PASSWORD)->EnableWindow(true);
			GetDlgItem(IDC_EDIT_USER_RETYPE_PASSWORD)->EnableWindow(true);

			GetDlgItem(IDC_EDIT_USER_RETYPE_PASSWORD)->SetFocus();
			GetDlgItem(IDC_BUTTON_USER_DELETE)->EnableWindow(true);
		}
		break;
	case HIDE_ALL:
		{
			
			GetDlgItem(IDC_STATIC_USER_GROUP_BOX)->EnableWindow(false);
			GetDlgItem(IDC_STATIC_USER_NAME)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_NAME)->EnableWindow(false);
			GetDlgItem(IDC_STATIC_USER_ACCESS_LEVEL)->EnableWindow(false);
			GetDlgItem(IDC_COMBO_ACCESS_LEVEL)->EnableWindow(false);

			GetDlgItem(IDC_STATIC_USER_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_STATIC_TYPE_CHANGE)->EnableWindow(false);
			GetDlgItem(IDC_STATIC_USER_NEW_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_NEW_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_STATIC_USER_RETRY_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_RETYPE_PASSWORD)->EnableWindow(false);

			GetDlgItem(IDC_BUTTON_USER_DELETE)->EnableWindow(false);

			GetDlgItem(IDC_USER_LIST)->SetFocus();
		}
		break;
	default:
		{
			GetDlgItem(IDC_STATIC_USER_NEW_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_NEW_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_STATIC_USER_RETRY_PASSWORD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_USER_RETYPE_PASSWORD)->EnableWindow(false);
		}
		break;
	}
}


BOOL CBacnetUserConfig::PreTranslateMessage(MSG* pMsg)
{
	
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			if(this->IsWindowVisible())
			{
				if((GetFocus()->GetDlgCtrlID() == IDC_EDIT_USER_PASSWORD) ||
					(GetFocus()->GetDlgCtrlID() == IDC_EDIT_USER_NEW_PASSWORD) ||
					(GetFocus()->GetDlgCtrlID() == IDC_EDIT_USER_RETYPE_PASSWORD))
				{
					::PostMessage(GetDlgItem(IDC_BUTTON_USER_OK)->m_hWnd,WM_LBUTTONDOWN,NULL,NULL);
					::PostMessage(GetDlgItem(IDC_BUTTON_USER_OK)->m_hWnd,WM_LBUTTONUP,NULL,NULL);
					return 0;
				}
				else if((GetFocus()->GetDlgCtrlID() == IDC_USER_LIST))
				{
					CRect list_rect,win_rect;
					m_user_config_list.GetWindowRect(list_rect);
					ScreenToClient(&list_rect);
					::GetWindowRect(m_user_config_hwnd,win_rect);
					m_user_config_list.Set_My_WindowRect(win_rect);
					m_user_config_list.Set_My_ListRect(list_rect);

					m_user_config_list.Get_clicked_mouse_position();
					return TRUE;
				}
				else
				{
					return 0;
				}
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CBacnetUserConfig::Initial_List()
{
	m_user_config_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
	//m_user_config_list.SetExtendedStyle(m_user_config_list.GetExtendedStyle() |LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_user_config_list.SetExtendedStyle(m_user_config_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
	m_user_config_list.InsertColumn(USERLIST_NAME, _T("User  Name"), 150, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);

	m_user_config_hwnd = this->m_hWnd;
	g_hwnd_now = m_user_config_hwnd;

	CRect list_rect,win_rect;
	m_user_config_list.GetWindowRect(list_rect);
	ScreenToClient(&list_rect);
	::GetWindowRect(m_user_config_hwnd,win_rect);
	m_user_config_list.Set_My_WindowRect(win_rect);
	m_user_config_list.Set_My_ListRect(list_rect);


	m_user_config_list.DeleteAllItems();
	for (int i=0;i<(int)m_user_login_data.size();i++)
	{
		CString temp_item,temp_dig_off,temp_dig_on,temp_dir_inv;
		temp_item.Format(_T("%d"),i+1);
		//m_user_config_list.InsertItem(i,temp_item);
		m_user_config_list.InsertItem(i,_T(""));

		CString temp_user_name;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_user_login_data.at(i).name, (int)strlen((char *)m_user_login_data.at(i).name)+1, 
			temp_user_name.GetBuffer(MAX_PATH), MAX_PATH );
		temp_user_name.ReleaseBuffer();
		if(temp_user_name.IsEmpty())
		{
			m_user_config_list.SetItemText(i,USERLIST_NAME,_T(""));
		}
		if(temp_user_name.GetLength()>= STR_USER_NAME_LENGTH)
			temp_user_name = temp_user_name.Left(STR_USER_NAME_LENGTH - 1);
		m_user_config_list.SetItemText(i,USERLIST_NAME,temp_user_name);

		for (int x=0;x<1;x++)
		{
			if((i%2)==0)
				m_user_config_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
			else
				m_user_config_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
		}
	}

	if(Device_Basic_Setting.reg.user_name == 2)
	{
		((CButton *)GetDlgItem(IDC_CHECK_USERLIST_INFO))->SetCheck(true);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_CHECK_USERLIST_INFO))->SetCheck(false);
	}
}


LRESULT CBacnetUserConfig::Fresh_User_List(WPARAM wParam,LPARAM lParam)
{

	for (int i=0;i<(int)m_user_login_data.size();i++)
	{
		CString temp_user_name;
		MultiByteToWideChar( CP_ACP, 0, (char *)m_user_login_data.at(i).name, (int)strlen((char *)m_user_login_data.at(i).name)+1, 
			temp_user_name.GetBuffer(MAX_PATH), MAX_PATH );
		temp_user_name.ReleaseBuffer();
		if(temp_user_name.IsEmpty())
		{
			m_user_config_list.SetItemText(i,USERLIST_NAME,_T(""));
		}
		if(temp_user_name.GetLength()>= STR_USER_NAME_LENGTH)
			temp_user_name = temp_user_name.Left(STR_USER_NAME_LENGTH - 1);
		m_user_config_list.SetItemText(i,USERLIST_NAME,temp_user_name);
	}

	return 0;
}


LRESULT CBacnetUserConfig::Fresh_User_Item(WPARAM wParam,LPARAM lParam)
{
	//int cmp_ret ;//compare if match it will 0;
	int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;

	return 0;
	if(Changed_SubItem == USERLIST_NAME)
	{
		CString cs_temp = m_user_config_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()>= STR_USER_NAME_LENGTH)	//长度不能大于结构体定义的长度;
		{
			MessageBox(_T("Length can not greater than 16"),_T("Warning"));
			PostMessage(WM_REFRESH_BAC_USER_NAME_LIST,NULL,NULL);
			return 0;
		}
		char cTemp1[255];
		memset(cTemp1,0,255);
		WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 255, NULL, NULL );
		memcpy_s(m_user_login_data.at(Changed_Item).name,STR_USER_NAME_LENGTH,cTemp1,STR_USER_NAME_LENGTH);

	}
	return 0;
}





void CBacnetUserConfig::OnNMClickUserList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	*pResult = 0;

	CString temp_cstring;
	long lRow,lCol;
	m_user_config_list.Set_Edit(false);
	DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
	CPoint point( LOWORD(dwPos), HIWORD(dwPos));
	m_user_config_list.ScreenToClient(&point);
	LVHITTESTINFO lvinfo;
	lvinfo.pt=point;
	lvinfo.flags=LVHT_ABOVE;
	int nItem=m_user_config_list.SubItemHitTest(&lvinfo);

	lRow = lvinfo.iItem;
	lCol = lvinfo.iSubItem;


	if(lRow>m_user_config_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
		return;
	if(lRow<0)
		return;

	m_slected_item = lRow;
	if(m_slected_item >= BAC_USER_LOGIN_COUNT)
		return;
	Refresh_parameter(m_slected_item);

}

void CBacnetUserConfig::Refresh_parameter(UINT index)
{
	CString temp_password;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_user_login_data.at(index).password, (int)strlen((char *)m_user_login_data.at(index).password)+1, 
		temp_password.GetBuffer(MAX_PATH), MAX_PATH );
	temp_password.ReleaseBuffer();
	if(temp_password.IsEmpty())
	{
		GetDlgItem(IDC_EDIT_USER_PASSWORD)->SetWindowText(_T(""));
		//MessageBox(_T("No password in this item,you can create a new one!"));
	}
	else if(temp_password.GetLength()>= STR_USER_PASSWORD_LENGTH)
	{
		temp_password = temp_password.Left(STR_USER_NAME_LENGTH - 1);
		GetDlgItem(IDC_EDIT_USER_PASSWORD)->SetWindowText(temp_password);
	}


	CString temp_user_name;
	MultiByteToWideChar( CP_ACP, 0, (char *)m_user_login_data.at(index).name, (int)strlen((char *)m_user_login_data.at(index).name)+1, 
		temp_user_name.GetBuffer(MAX_PATH), MAX_PATH );
	temp_user_name.ReleaseBuffer();
	if(temp_user_name.IsEmpty())
	{
		GetDlgItem(IDC_EDIT_USER_NAME)->SetWindowText(_T(""));
	}
	if(temp_user_name.GetLength()>= STR_USER_NAME_LENGTH)
		temp_user_name = temp_user_name.Left(STR_USER_NAME_LENGTH - 1);
	GetDlgItem(IDC_EDIT_USER_NAME)->SetWindowText(temp_user_name);

	((CComboBox *)GetDlgItem(IDC_COMBO_ACCESS_LEVEL))->ResetContent();
	((CComboBox *)GetDlgItem(IDC_COMBO_ACCESS_LEVEL))->AddString(_T("View only"));
	((CComboBox *)GetDlgItem(IDC_COMBO_ACCESS_LEVEL))->AddString(_T("Full access"));
	((CComboBox *)GetDlgItem(IDC_COMBO_ACCESS_LEVEL))->AddString(_T("Graphic Mode"));
	((CComboBox *)GetDlgItem(IDC_COMBO_ACCESS_LEVEL))->AddString(_T("Routine Mode"));
	if(m_user_login_data.at(index).access_level == 1)
	{
		GetDlgItem(IDC_COMBO_ACCESS_LEVEL)->SetWindowText(_T("View only"));
	}
	else if(m_user_login_data.at(index).access_level == 2)
	{
		GetDlgItem(IDC_COMBO_ACCESS_LEVEL)->SetWindowText(_T("Full access"));
	}
	else if(m_user_login_data.at(index).access_level == 3)
	{
		GetDlgItem(IDC_COMBO_ACCESS_LEVEL)->SetWindowText(_T("Graphic Mode"));
	}
	else if(m_user_login_data.at(index).access_level == 4)
	{
		GetDlgItem(IDC_COMBO_ACCESS_LEVEL)->SetWindowText(_T("Routine Mode"));
	}
	else
	{
		GetDlgItem(IDC_COMBO_ACCESS_LEVEL)->SetWindowText(_T("Full access"));
	}

	if((!temp_user_name.IsEmpty()) && (!temp_password.IsEmpty()))
	{
		GetDlgItem(IDC_EDIT_USER_PASSWORD)->SetWindowTextW(_T(""));
		GetDlgItem(IDC_EDIT_USER_NEW_PASSWORD)->SetWindowTextW(_T(""));
		GetDlgItem(IDC_EDIT_USER_RETYPE_PASSWORD)->SetWindowTextW(_T(""));
		ok_button_stage = stage_enter_original_password;
		Enable_Window_Stage(ENTER_ORIGINAL_PASSWORD);
	}
	else
	{
		GetDlgItem(IDC_EDIT_USER_PASSWORD)->SetWindowTextW(_T(""));
		GetDlgItem(IDC_EDIT_USER_NEW_PASSWORD)->SetWindowTextW(_T(""));
		GetDlgItem(IDC_EDIT_USER_RETYPE_PASSWORD)->SetWindowTextW(_T(""));
		ok_button_stage = stage_enter_new_password;
		Enable_Window_Stage(HIDE_ENTER_ORIGINAL_PASSWORD);
	}

}


void CBacnetUserConfig::OnBnClickedCheckUserlistInfo()
{
	
	if(!((CButton *)GetDlgItem(IDC_CHECK_USERLIST_INFO))->GetCheck())
	{
		((CButton *)GetDlgItem(IDC_CHECK_USERLIST_INFO))->SetCheck(false);

		Device_Basic_Setting.reg.user_name = 1;
		//CString temp_task_info;
		//temp_task_info.Format(_T("Disable user list feature "));
		//Post_Write_Message(g_bac_instance,(int8_t)WRITE_SETTING_COMMAND,0,0,sizeof(Str_Setting_Info),this->m_hWnd,temp_task_info);

	}
	else
	{
		bool any_user_valid = false;
		bool any_user_administrator = false;
		//检测是否里面存在有效的账号密码;
		for (int i=0;i<(int)m_user_login_data.size();i++)
		{
			CString temp_user_name;
			MultiByteToWideChar( CP_ACP, 0, (char *)m_user_login_data.at(i).name, (int)strlen((char *)m_user_login_data.at(i).name)+1, 
				temp_user_name.GetBuffer(MAX_PATH), MAX_PATH );
			temp_user_name.ReleaseBuffer();

			CString temp_password;
			MultiByteToWideChar( CP_ACP, 0, (char *)m_user_login_data.at(i).password, (int)strlen((char *)m_user_login_data.at(i).password)+1, 
				temp_password.GetBuffer(MAX_PATH), MAX_PATH );
			temp_password.ReleaseBuffer();
			if(!temp_user_name.IsEmpty())
			{
				if(!temp_password.IsEmpty())
				{
					if(m_user_login_data.at(i).access_level == LOGIN_SUCCESS_FULL_ACCESS)
					{
						any_user_administrator = true;
						break;
					}
					any_user_valid = true;	//User表里面 存在可用的 账号密码;
				}
			}		
		}

		if(any_user_administrator)
		{
			((CButton *)GetDlgItem(IDC_CHECK_USERLIST_INFO))->SetCheck(true);
			Device_Basic_Setting.reg.user_name = 2;
		}
		else if(any_user_valid)
		{
			((CButton *)GetDlgItem(IDC_CHECK_USERLIST_INFO))->SetCheck(false);
			Device_Basic_Setting.reg.user_name = 1;
			MessageBox(_T("User List don't contain any user have full access level!\r\nPlease add it and then enable this feature!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			return;
		}
		else
		{
			((CButton *)GetDlgItem(IDC_CHECK_USERLIST_INFO))->SetCheck(false);
			Device_Basic_Setting.reg.user_name = 1;
			MessageBox(_T("User List don't contain any valid user name and password!\r\nPlease add it and then enable this feature!"),_T("Warning"),MB_OK | MB_ICONINFORMATION);
			return;
		}

		

	}

	int temp_invoke_id = -1;
	int send_status = true;
	int	resend_count = 0;
	for (int z=0;z<3;z++)
	{
		do 
		{
			resend_count ++;
			if(resend_count>5)
			{
				send_status = false;
				break;
			}
			temp_invoke_id = WritePrivateData(g_bac_instance,WRITE_SETTING_COMMAND,0,0);

			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);
		if(send_status)
		{
			Sleep(1000);
			if(tsm_invoke_id_free(temp_invoke_id))
			{
				ok_button_stage = 0;
				PostMessage(WM_REFRESH_BAC_USER_NAME_LIST,NULL,NULL);
				Enable_Window_Stage(HIDE_ALL);
				MessageBox(_T("Operation success!"),_T("Information"),MB_OK);
				return;
			}
			else
				continue;
		}
	}

}


void CBacnetUserConfig::OnBnClickedButtonUserOk()
{
	
	char temp_buffer[255];
	int compare_ret = 0;
	CString temp_enter_original;
	CString first_pw;
	CString second_pw;
	CString temp_user_name;
	CString access_string;
	switch(ok_button_stage)
	{
	case stage_enter_original_password://检查原始密码是否正确;
		{
			//m_user_login_data.at(m_slected_item).password
			GetDlgItemTextW(IDC_EDIT_USER_NAME,temp_user_name);
			GetDlgItemTextW(IDC_EDIT_USER_PASSWORD,temp_enter_original);
			if(temp_user_name.IsEmpty())
			{
				MessageBox(_T("user name is empty"),_T("Warning"),MB_OK);
				return;
			}
			if(temp_user_name.GetLength()>=STR_USER_NAME_LENGTH)
			{
				MessageBox(_T("User name is too long!"),_T("Warning"),MB_OK);
				return;
			}
			if(temp_enter_original.IsEmpty())
			{
				MessageBox(_T("Password is empty"),_T("Warning"),MB_OK);
				return;
			}
			memset(temp_buffer,0,255);
			WideCharToMultiByte(CP_ACP,NULL,temp_enter_original.GetBuffer(),-1,temp_buffer,255,NULL,NULL);
			compare_ret = strcmp(temp_buffer,m_user_login_data.at(m_slected_item).password);
			if(compare_ret == 0)
			{
				ok_button_stage = stage_enter_new_password;
				Enable_Window_Stage(ENTER_NEW_PASSWORD);
			}
			else
			{
				SetDlgItemTextW(IDC_EDIT_USER_PASSWORD,_T(""));
				MessageBox(_T("User name or password error"),_T("Warning"),MB_OK);	
				GetDlgItem(IDC_EDIT_USER_PASSWORD)->SetFocus();
				return;
			}
		}
		break;
	case stage_enter_new_password:
		{
			GetDlgItemTextW(IDC_EDIT_USER_NEW_PASSWORD,first_pw);
			if(first_pw.IsEmpty())
			{
				MessageBox(_T("Password is empty"),_T("Warning"),MB_OK);
				return;
			}
			if(first_pw.GetLength()>=STR_USER_PASSWORD_LENGTH)
			{
				MessageBox(_T("Password too long!"),_T("Warning"),MB_OK);
				return;
			}
			ok_button_stage = stage_retype_password;
			Enable_Window_Stage(RETYPE_NEW_PASSWORD);
		}
		break;
	case  stage_retype_password:
		{
			GetDlgItemTextW(IDC_EDIT_USER_NEW_PASSWORD,first_pw);
			GetDlgItemTextW(IDC_EDIT_USER_RETYPE_PASSWORD,second_pw);
			GetDlgItemTextW(IDC_EDIT_USER_NAME,temp_user_name);
			GetDlgItemTextW(IDC_COMBO_ACCESS_LEVEL,access_string);
			
			if(second_pw.IsEmpty())
			{
				MessageBox(_T("Password is empty"),_T("Warning"),MB_OK);
				return;
			}
			if(second_pw.GetLength()>=STR_USER_PASSWORD_LENGTH)
			{
				MessageBox(_T("Password too long!"),_T("Warning"),MB_OK);
				return;
			}

			if(first_pw.CompareNoCase(second_pw) == 0)
			{
				if(temp_user_name.IsEmpty())
				{
					MessageBox(_T("user name is empty"),_T("Warning"),MB_OK);
					return;
				}
				memset(temp_buffer,0,255);
				WideCharToMultiByte(CP_ACP,NULL,temp_user_name.GetBuffer(),-1,temp_buffer,255,NULL,NULL);
				memcpy(m_user_login_data.at(m_slected_item).name,temp_buffer,STR_USER_NAME_LENGTH);

				if(access_string.CompareNoCase(_T("View only")) == 0 )
				{
					m_user_login_data.at(m_slected_item).access_level = 1;
				}
				else if(access_string.CompareNoCase(_T("Graphic Mode")) == 0)
				{
					m_user_login_data.at(m_slected_item).access_level = 3;
				}
				else if(access_string.CompareNoCase(_T("Routine Mode")) == 0)
				{
					m_user_login_data.at(m_slected_item).access_level = 4;
				}
				else
					m_user_login_data.at(m_slected_item).access_level = 2;
				//_T("View only"));
				//_T("Full access"));

				memset(temp_buffer,0,255);
				WideCharToMultiByte(CP_ACP,NULL,second_pw.GetBuffer(),-1,temp_buffer,255,NULL,NULL);
				memcpy(m_user_login_data.at(m_slected_item).password,temp_buffer,STR_USER_PASSWORD_LENGTH);

				//Post_Write_Message(g_bac_instance,WRITEUSER_T3000,m_slected_item,m_slected_item,sizeof(Str_userlogin_point),m_user_config_hwnd ,_T(""),m_slected_item,m_slected_item);
				int temp_invoke_id = -1;
				int send_status = true;
				int	resend_count = 0;
				for (int z=0;z<3;z++)
				{
					do 
					{
						resend_count ++;
						if(resend_count>5)
						{
							send_status = false;
							break;
						}
						temp_invoke_id = WritePrivateData(g_bac_instance,WRITEUSER_T3000,m_slected_item,m_slected_item);

						Sleep(SEND_COMMAND_DELAY_TIME);
					} while (g_invoke_id<0);
					if(send_status)
					{
						Sleep(1000);
						if(tsm_invoke_id_free(temp_invoke_id))
						{
							ok_button_stage = 0;
							PostMessage(WM_REFRESH_BAC_USER_NAME_LIST,NULL,NULL);
							Enable_Window_Stage(HIDE_ALL);
							MessageBox(_T("Operation success!"),_T("Information"),MB_OK);
							return;
						}
						else
							continue;
					}
				}



			}
			else
			{
				MessageBox(_T("Password is not the same!"),_T("Warning"),MB_OK);
				return;
			}
		}
		break;
	default:
		break;
	}
}


void CBacnetUserConfig::OnBnClickedButtonUserDelete()
{
	
	if((m_slected_item<0) || (m_slected_item >= BAC_USER_LOGIN_COUNT ))
		return;

	memset(&m_user_login_data.at(m_slected_item),0,sizeof(Str_userlogin_point));



	int temp_invoke_id = -1;
	int send_status = true;
	int	resend_count = 0;
	for (int z=0;z<3;z++)
	{
		do 
		{
			resend_count ++;
			if(resend_count>5)
			{
				send_status = false;
				break;
			}
			temp_invoke_id = WritePrivateData(g_bac_instance,WRITEUSER_T3000,m_slected_item,m_slected_item);

			Sleep(SEND_COMMAND_DELAY_TIME);
		} while (g_invoke_id<0);
		if(send_status)
		{
			Sleep(1000);
			if(tsm_invoke_id_free(temp_invoke_id))
			{
				ok_button_stage = 0;
				PostMessage(WM_REFRESH_BAC_USER_NAME_LIST,NULL,NULL);
				Enable_Window_Stage(HIDE_ALL);
				MessageBox(_T("Delete success!"));
				return;
			}
			else
				continue;
		}
	}

	MessageBox(_T("Timeout!Please try again!"));

}
