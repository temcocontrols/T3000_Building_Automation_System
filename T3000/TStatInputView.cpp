// TStatInputView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TStatInputView.h"
#include "BacnetInput.h"
#include "global_function.h"
 
 
#include <bitset>
#include "BuildTable1.h"
#include "TstatRangeDlg.h"
 
#include "MainFrm.h"
#include "Tstat_HelpDoc.h"
// CTStatInputView
UINT BackMainUIFresh_TstatInput(LPVOID pParam)
{

    CTStatInputView* pdlg = (CTStatInputView*)pParam;
    int multy_ret = 0;
    Sleep( 3000);
    while(1)
    {
        if(pdlg->IsWindowVisible())
        {


            if ((g_protocol == PROTOCOL_BACNET_IP) || (g_protocol == MODBUS_BACNET_MSTP) || (g_protocol == PROTOCOL_BIP_TO_MSTP))
            {
                break; 
            }
            Sleep(2000);

            if (!is_connect())
            {

                continue;
            }
            if (!no_mouse_keyboard_event_enable_refresh)
            {
                continue;
            }
            if (g_tstat_id==255)
            {
                continue;
            }
            if (g_fresh_T3000_background)
            {
                continue;
            }
            if(no_mouse_keyboard_event_enable_refresh) 
            {
                if (MODBUS_INTERNAL_THERMISTOR>0)
                {
                    multy_ret =  Read_Multi(g_tstat_id,&multi_register_value[MODBUS_INTERNAL_THERMISTOR],MODBUS_INTERNAL_THERMISTOR,2);
                    if (multy_ret>0)
                    {
                        //memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
                        for (int i=0;i<2;i++)
                        {
                            product_register_value[MODBUS_INTERNAL_THERMISTOR+i]= multi_register_value[MODBUS_INTERNAL_THERMISTOR+i];
                        }
                    }
                }

            }
            if (!no_mouse_keyboard_event_enable_refresh)
            {
                continue;
            }
            if(no_mouse_keyboard_event_enable_refresh) 
            {
                if (MODBUS_EXTERNAL_SENSOR_0>0)
                {
                    multy_ret = Read_Multi(g_tstat_id,&multi_register_value[MODBUS_EXTERNAL_SENSOR_0],MODBUS_EXTERNAL_SENSOR_0,5);
                    if (multy_ret>0)
                    {
                        //memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
                        for (int i=0;i<5;i++)
                        {
                            product_register_value[MODBUS_EXTERNAL_SENSOR_0+i]= multi_register_value[MODBUS_EXTERNAL_SENSOR_0+i];
                        }
                    }
                }

            }
            if (!no_mouse_keyboard_event_enable_refresh)
            {
                continue;
            }
            if(no_mouse_keyboard_event_enable_refresh) 
            {
                if (MODBUS_TEMPRATURE_CHIP>0)
                {
                    multy_ret = Read_Multi(g_tstat_id,&multi_register_value[MODBUS_TEMPRATURE_CHIP],MODBUS_TEMPRATURE_CHIP,20);
                    if (multy_ret>0)
                    {
                        //memcpy_s(product_register_value,sizeof(product_register_value),multi_register_value,sizeof(multi_register_value));
                        for (int i=0;i<20;i++)
                        {
                            product_register_value[MODBUS_TEMPRATURE_CHIP+i]= multi_register_value[MODBUS_TEMPRATURE_CHIP+i];
                        }
                    }
                }

            }
            CString achive_file_path;
            CString temp_serial;
            temp_serial.Format(_T("%d.prog"),g_selected_serialnumber);
            achive_file_path = g_achive_folder + _T("\\") + temp_serial;
            Save_Product_Value_Cache(achive_file_path);

            LoadTstat_InputData();
            
            PostMessage(g_hwnd_now,WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);

        }
        else
        {
            if (pdlg->m_Fresh_BackgroundThreadHandle!=NULL)
            {
                TerminateThread(pdlg->m_Fresh_BackgroundThreadHandle->m_hThread,0);
                pdlg->m_Fresh_BackgroundThreadHandle=NULL;
            }
            break;
        }
    }
    return 0;
}

IMPLEMENT_DYNCREATE(CTStatInputView, CFormView)

CTStatInputView::CTStatInputView()
	: CFormView(CTStatInputView::IDD)
{
    m_Fresh_BackgroundThreadHandle = NULL;
}

CTStatInputView::~CTStatInputView()
{
    if (m_Fresh_BackgroundThreadHandle!=NULL)
    {
        BOOL bRet = TerminateThread(m_Fresh_BackgroundThreadHandle->m_hThread,0);
        m_Fresh_BackgroundThreadHandle=NULL;
    }
}

void CTStatInputView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_input_list);
}

BEGIN_MESSAGE_MAP(CTStatInputView, CFormView)
    
	ON_MESSAGE(MY_RESUME_DATA, InputMessageCallBack)
    ON_MESSAGE(WM_REFRESH_BAC_INPUT_LIST,Fresh_Input_List)
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Input_Item)	
    ON_NOTIFY(NM_CLICK, IDC_LIST1, &CTStatInputView::OnNMClickList1)
    ON_WM_SIZE()

	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CTStatInputView diagnostics

#ifdef _DEBUG
void CTStatInputView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTStatInputView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG
void CTStatInputView::Fresh_One_Item(int row){
	m_input_list.SetItemText(row,1,m_tstat_input_data.at(row).InputName.StrValue);
	m_input_list.SetItemText(row,2,m_tstat_input_data.at(row).AM.StrValue);
	m_input_list.SetItemText(row,3,m_tstat_input_data.at(row).Value.StrValue);
	m_input_list.SetItemText(row,4,m_tstat_input_data.at(row).Unit.StrValue);
	m_input_list.SetItemText(row,5,m_tstat_input_data.at(row).Range.StrValue);
	m_input_list.SetItemText(row,6,_T("Adjust..."));
	m_input_list.SetItemText(row,7,m_tstat_input_data.at(row).Filter.StrValue);
	m_input_list.SetItemText(row,8,m_tstat_input_data.at(row).Function.StrValue);
	m_input_list.SetItemText(row,9,m_tstat_input_data.at(row).CustomTable.StrValue);
}
LRESULT  CTStatInputView::InputMessageCallBack(WPARAM wParam, LPARAM lParam)
{
    int msg_result=(int) wParam;
    LoadTstat_InputData();
    CString temp_task_info;
    CString Show_Results;
    if (msg_result==1||msg_result==2)//单写
    {
        _MessageWriteOneInfo_List *pInvoke =(_MessageWriteOneInfo_List *)lParam;
        CString temp_cs = pInvoke->Changed_Name;
        if (msg_result==1)
        {
            Show_Results = temp_cs + _T(" Success!");
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results); 
        }
        else
        {
            Show_Results = temp_cs + _T("Fail!");
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
        }
        if (product_type == CS3000)
        {
            LoadInputData_CS3000();
        }


        Fresh_One_Item(pInvoke->mRow);

        if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
            m_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
        else
            m_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);

        m_input_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


        if(pInvoke)
            delete pInvoke;
    }
    if (msg_result==3||msg_result==4)//多写
    { 
        _MessageWriteMultiInfo_List *pInvoke =(_MessageWriteMultiInfo_List *)lParam;

        CString temp_cs = pInvoke->Changed_Name;
        if (msg_result==3){
            Show_Results = temp_cs + _T("Success!");
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);   
        }
        else{
            Show_Results = temp_cs + _T("Fail!");
            SetPaneString(BAC_SHOW_MISSION_RESULTS,Show_Results);
        }




        Fresh_One_Item(pInvoke->mRow);

        //  PostMessage(WM_REFRESH_BAC_INPUT_LIST,0,0);


        if((pInvoke->mRow%2)==0)	//恢复前景和 背景 颜色;
            m_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
        else
            m_input_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);

        m_input_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


        if(pInvoke)
            delete pInvoke;

    }
    return 0; 
}

LRESULT CTStatInputView::Fresh_Input_List(WPARAM wParam,LPARAM lParam)
{
    
    CString strTemp;
    //
    for(int i=0;i<(int)m_tstat_input_data.size();i++)
    {
        if (i > 8)
        {
            bitset<16> module_type(product_register_value[20]);
            if (module_type.at(1) == true)
            {
                b_hum_sensor = true;
            }
            else
            {
                b_hum_sensor = false;
            }
            if ((i == 9) && (b_hum_sensor == false))
            {
                continue;
            }

            if (module_type.at(2) == true)
            {
                b_co2_sensor = true;
            }
            else
            {
                b_co2_sensor = false;
            }

            if ((i == 10) && (b_co2_sensor == false))
            {
                continue;
            }

            if (module_type.at(3) == true)
            {
                b_lux_sensor = true;
            }
            else
            {
                b_lux_sensor = false;
            }


            if ((i == 11) && (b_lux_sensor == false))
            {
                continue;
            }
        }
        m_input_list.SetItemText(i,1,m_tstat_input_data.at(i).InputName.StrValue);
        m_input_list.SetItemText(i,2,m_tstat_input_data.at(i).AM.StrValue);
        m_input_list.SetItemText(i,3,m_tstat_input_data.at(i).Value.StrValue);
        m_input_list.SetItemText(i,4,m_tstat_input_data.at(i).Unit.StrValue);
        m_input_list.SetItemText(i,5,m_tstat_input_data.at(i).Range.StrValue);
        m_input_list.SetItemText(i, 6, m_tstat_input_data.at(i).Calibration.StrValue);
        //m_input_list.SetItemText(i,6,_T("Adjust..."));
        m_input_list.SetItemText(i,7,m_tstat_input_data.at(i).Filter.StrValue);
        m_input_list.SetItemText(i,8,m_tstat_input_data.at(i).Function.StrValue);
        m_input_list.SetItemText(i,9,m_tstat_input_data.at(i).CustomTable.StrValue);
    }
    m_input_list.GetFocus();
    return 0;  
}

LRESULT CTStatInputView::Fresh_Input_Item(WPARAM wParam,LPARAM lParam)
{ 
    int Changed_Item = (int)wParam;
	int Changed_SubItem = (int)lParam;
	CString New_CString =  m_input_list.GetItemText(Changed_Item,Changed_SubItem);
	
    if ((Changed_Item == 9) && (b_hum_sensor == false))
    {
        return 0;
    }

    if ((Changed_Item == 10) && (b_co2_sensor == false))
    {
        return 0;
    }

    if ((Changed_Item == 11) && (b_lux_sensor == false))
    {
        return 0;
    }

	BOOL IS_SEND=FALSE;
	_MessageWriteOneInfo_List  *pwrite_info = new _MessageWriteOneInfo_List;
	pwrite_info->list_type=LIST_TYPE_INPUT_TSTAT;
	if(Changed_SubItem == 1)
	{
		if (product_type==CS3000)
		{
			IS_SEND=FALSE;
			m_input_list.Set_Edit(false);
			return 0;
		}
        if (Changed_Item > 8)
        {
            IS_SEND=FALSE;
            m_input_list.Set_Edit(false);
            return 0;
        }
        
		CString cs_temp = m_input_list.GetItemText(Changed_Item,Changed_SubItem);
		if(cs_temp.GetLength()> STR_IN_LABEL)	//长度不能大于结构体定义的长度;
		{
			/* MessageBox(_T("Length can not greater than 8"),_T("Warning"));
            PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
            return 0; */
            cs_temp.Delete(8,cs_temp.GetLength()-8);
		}

		if (m_tstat_input_data.at(Changed_Item).InputName.StrValue.CompareNoCase(cs_temp)==0)
		{
			PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
			return 0; 
		}

	   char cTemp1[8];
	   memset(cTemp1,0,8);
	   WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 8, NULL, NULL );

	   _MessageWriteMultiInfo_List *pwrite_info = new _MessageWriteMultiInfo_List;
	   pwrite_info->device_id=g_tstat_id;
	   pwrite_info->Changed_Name.Format(_T("Change Inputname%d from %s to %s"),Changed_Item,m_tstat_input_data.at(Changed_Item).InputName.StrValue,cs_temp);
	   pwrite_info->hwnd=g_hwnd_now;
	   pwrite_info->mRow=Changed_Item;
	   pwrite_info->mCol=Changed_SubItem;
	   pwrite_info->Start_Address=m_tstat_input_data.at(Changed_Item).InputName.regAddress;
	   memcpy_s(pwrite_info->RegValue,STR_IN_LABEL-1,cTemp1,STR_IN_LABEL-1);
	   m_input_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
	   if(!PostThreadMessage(nThreadID,MY_WRITE_MULTI_LIST,(WPARAM)pwrite_info,NULL))//post thread msg
	   {
		   return FALSE;
	   }
	   else
	   {
		   return TRUE;
	   }

	}
	if(Changed_SubItem== 3)//Manual + 模拟量
	{
	   if (m_tstat_input_data.at(Changed_Item).Value.StrValue.CompareNoCase(New_CString)==0)
	   {
		   m_input_list.Set_Edit(false);
		   return 0;
	   }
	   
	   int SendValue=0;
	   float new_fvalue=_wtof(New_CString);
	   if (product_type == CS3000)
	   {
		   if (product_register_value[7]==PM_CS_RSM_AC||product_register_value[7]==PM_CS_SM_AC)
		   {
			   SendValue = (int)new_fvalue*100;
		   } 
		   else
		   {
			   SendValue = (int)new_fvalue*10;
		   }

	   } 
	   else
	   {
		   if (m_tstat_input_data.at(Changed_Item).Range.StrValue.CompareNoCase(_T("0-100%"))==0)
		   {
			   SendValue=(int)new_fvalue;
		   }
		   else{
                if (Changed_Item == 10)
                {
                  SendValue=(int)new_fvalue;
                }
                else{
                SendValue=(int)new_fvalue*10;
                }
			   
		   }
	   }
	   pwrite_info->Changed_Name.Format(_T("%s's Value,From %s to %s"),m_tstat_input_data.at(Changed_Item).InputName.StrValue,m_tstat_input_data.at(Changed_Item).Value.StrValue,New_CString);
	   pwrite_info->address=m_tstat_input_data.at(Changed_Item).Value.regAddress;
	   pwrite_info->new_value=SendValue;
	   IS_SEND=TRUE;
	}
	if (Changed_SubItem==5)
	{   
	    if(New_CString.IsEmpty()||m_tstat_input_data.at(Changed_Item).Value.StrValue.CompareNoCase(New_CString)==0)
	    {
		m_input_list.SetItemText(Changed_Item,Changed_SubItem,L"Adjust...");
		return 0;
	    }
	    
		int SendValue=0;
		float new_fvalue=_wtof(New_CString);
		if (product_type == CS3000)
		{
			  if(product_register_value[7]==PM_CS_RSM_AC||product_register_value[7]==PM_CS_SM_AC)
			 {
			   SendValue = (int)new_fvalue*100;
			 } 
			 else
			 {
			   SendValue = (int)new_fvalue*10;
			 }
			
		} 
		else
		{
			if (m_tstat_input_data.at(Changed_Item).Range.StrValue.CompareNoCase(_T("0-100%"))==0)
			{
				SendValue=(int)new_fvalue;
			}
			else{
				SendValue=(int)new_fvalue*10;
			}
		}

		pwrite_info->Changed_Name.Format(_T("Calibration %s 's Value,From %s to %s"),m_tstat_input_data.at(Changed_Item).InputName.StrValue,m_tstat_input_data.at(Changed_Item).Value.StrValue,New_CString);
		pwrite_info->address=m_tstat_input_data.at(Changed_Item).Value.regAddress;
		pwrite_info->new_value=SendValue;
		IS_SEND=TRUE;
	}
	if (Changed_SubItem == 6)
	{
		if (New_CString.IsEmpty()||m_tstat_input_data.at(Changed_Item).Value.StrValue.CompareNoCase(New_CString)==0)
		{
		   m_input_list.Set_Edit(false);
		   //m_input_list.SetItemText(Changed_Item,Changed_SubItem,L"Adjust...");
		   return 0;
		}
        int SendValue=0;
        SendValue=_wtoi(New_CString)*10;
        pwrite_info->Changed_Name.Format(_T(" %s 's Value,From %s to %s"),m_tstat_input_data.at(Changed_Item).InputName.StrValue,m_tstat_input_data.at(Changed_Item).Value.StrValue,New_CString);
        pwrite_info->address=m_tstat_input_data.at(Changed_Item).Value.regAddress;
        pwrite_info->new_value=SendValue;
		IS_SEND=TRUE;
	}
	if (Changed_SubItem == 7)
	{
		if (New_CString.IsEmpty()||m_tstat_input_data.at(Changed_Item).Filter.StrValue.CompareNoCase(New_CString)==0)
		{
			m_input_list.Set_Edit(false);
			//m_input_list.SetItemText(Changed_Item,Changed_SubItem,L"Adjust...");
			return 0;
		}
		int SendValue=0;
		SendValue=_wtoi(New_CString);
		pwrite_info->Changed_Name.Format(_T(" %s 's Filter,From %s to %s"),m_tstat_input_data.at(Changed_Item).InputName.StrValue,m_tstat_input_data.at(Changed_Item).Filter.StrValue,New_CString);
		pwrite_info->address=m_tstat_input_data.at(Changed_Item).Filter.regAddress;
		pwrite_info->new_value=SendValue;
		IS_SEND=TRUE;
	}
	if (Changed_SubItem == 8)
	{
	     if (m_tstat_input_data.at(Changed_Item).Function.StrValue.CompareNoCase(New_CString)==0)
	     {
		  m_input_list.Set_Edit(false);
		  return 0;
	     }
		 int func=0;
		 for (int i=0;i<13;i++)
		 {
		   if (New_CString.CompareNoCase(INPUT_FUNS[i])==0)
		   {
		     func=i;
			 break;
		   }
		   
		 }
		 
		 pwrite_info->Changed_Name.Format(_T("%s 's Function,From %s to %s"),m_tstat_input_data.at(Changed_Item).InputName.StrValue,m_tstat_input_data.at(Changed_Item).Function.StrValue,New_CString);
		 pwrite_info->address=m_tstat_input_data.at(Changed_Item).Function.regAddress;
		 pwrite_info->new_value=func;
	     IS_SEND = TRUE;
	    
	}
	if (Changed_SubItem == 5) 
	{
	  if (product_type==CS3000)
	  {
	    if (m_tstat_input_data.at(Changed_Item).Range.Reg_Property==REG_READ_ONLY)
	    {
			IS_SEND=FALSE;
			m_input_list.Set_Edit(false);
			return 0;
	    }
	    int sendvalue=-1;
	    for (int i=0;i<3;i++)
	    {
		  if (New_CString.CompareNoCase(CS3000_INPUT_RANGE[i])==0)
		  {
		     sendvalue=i;
			 break;
		  }
		  
	    }
	    if (sendvalue!=-1)
	    {
			pwrite_info->Changed_Name.Format(_T("%s 's Range,From %s to %s"),m_tstat_input_data.at(Changed_Item).InputName.StrValue,m_tstat_input_data.at(Changed_Item).Range.StrValue,New_CString);
			pwrite_info->address=m_tstat_input_data.at(Changed_Item).Range.regAddress;
			pwrite_info->new_value=sendvalue;
			 IS_SEND = TRUE;
	    }
	    
	  }
	}
	
	if (IS_SEND)
	{	
		pwrite_info->device_id=g_tstat_id;
		pwrite_info->hwnd=g_hwnd_now;
		pwrite_info->mCol=Changed_SubItem;
		pwrite_info->mRow=Changed_Item;
		m_input_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
		if(!PostThreadMessage(nThreadID,MY_WRITE_ONE_LIST,(WPARAM)pwrite_info,NULL))//post thread msg
		{
			return FALSE ;
		}
		else
		{
			return TRUE ;
		}
	}
   return 0;
}

void CTStatInputView::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    CString temp_cstring;
    long lRow,lCol;
    m_input_list.Set_Edit(true);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_input_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_input_list.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;


    if(lRow>m_input_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
        return;
    if(lRow<0)
        return;

    //判断有无此传感器 没有 就不显示，并且不可操作.
    if ((lRow == 9) && (b_hum_sensor == false))
    {
        return ;
    }

    if ((lRow == 10) && (b_co2_sensor == false))
    {
        return ;
    }

    if ((lRow == 11) && (b_lux_sensor == false))
    {
        return ;
    }

    BOOL IS_SEND=FALSE;
    CString temp1;
    CStringArray temparray;
    _MessageWriteOneInfo_List  *pwrite_info = new _MessageWriteOneInfo_List;
    pwrite_info->list_type=LIST_TYPE_INPUT_TSTAT;
    if(lCol == 1)
    {
        if (product_type==CS3000||lRow==8||lRow==9||lRow==10||lRow==11)
        {
            m_input_list.Set_Edit(false);
            IS_SEND=FALSE;
            return;
            //goto ENDCLICK;
        }   
    }
    if (lCol == 3)
    {
        if (product_type == T3000_6_ADDRESS)
        {
            if (m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(NO_APPLICATION)==0)
            {
                IS_SEND=FALSE;
                m_input_list.Set_Edit(false);
                return;
            }
            if (lRow == 12)    //LUX:不可以编辑
            {
                m_input_list.Set_Edit(false);
                return ;
            }

            if (lRow>=9)
            {
                m_input_list.Set_Edit(true);

            }
            else{
                if (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(NO_APPLICATION)==0)
                {
                    IS_SEND=FALSE;
                    m_input_list.Set_Edit(false);
                    return;
                }
                if (m_tstat_input_data.at(lRow).AM.StrValue.CompareNoCase(_T("Auto"))==0)
                {
                    IS_SEND=FALSE;
                    m_input_list.Set_Edit(false);
                    return;
                }
                //Auto && 开关量 --不可以写值
                if (m_tstat_input_data.at(lRow).AM.StrValue.CompareNoCase(_T("Auto"))!=0&&
                    (
                    m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("On"))==0||
                    m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Off"))==0||
                    m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Occupied"))==0||
                    m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Unoccupied"))==0||
                    m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Open"))==0||
                    m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Close"))==0
                    )
                    )
                {
                    //Manual 的开关量
                    if (
                        m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("On"))==0||

                        m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Occupied"))==0||

                        m_tstat_input_data.at(lRow).Value.StrValue.CompareNoCase(_T("Open"))==0

                        ){
                            pwrite_info->Changed_Name.Format(_T("%s's Value,From 1 to 0"),lRow,m_tstat_input_data.at(lRow).InputName.StrValue);
                            pwrite_info->address=m_tstat_input_data.at(lRow).Value.regAddress;
                            pwrite_info->new_value=0;
                    }
                    else{
                        pwrite_info->Changed_Name.Format(_T("%s's Value,From 0 to 1"),lRow,m_tstat_input_data.at(lRow).InputName.StrValue);
                        pwrite_info->address=m_tstat_input_data.at(lRow).Value.regAddress;
                        pwrite_info->new_value=1;
                    }  
                    IS_SEND=TRUE;
                    m_input_list.Set_Edit(false);
                } 

            }
        }
    }
	//Unit
    if (lCol == 4)
    {
        IS_SEND=FALSE;
        return;
    }
	//Range
    if (lCol == 5) 
    {
        if(m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(NO_APPLICATION)==0){
            m_input_list.Set_Edit(false);
            IS_SEND=FALSE;
            return;
        }
        if (product_type != CS3000)
        {
            if (lRow==8)
            {
                if (m_tstat_input_data.at(lRow).Range.RegValue==0)
                {
                    pwrite_info->Changed_Name.Format(_T("%s's Range,From 0 to 1"),m_tstat_input_data.at(lRow).InputName.StrValue);
                    pwrite_info->address=m_tstat_input_data.at(lRow).Range.regAddress;
                    pwrite_info->new_value=1;
                }
                else
                {
                    pwrite_info->Changed_Name.Format(_T("%s's Range,From 1 to 0"),m_tstat_input_data.at(lRow).InputName.StrValue);
                    pwrite_info->address=m_tstat_input_data.at(lRow).Range.regAddress;
                    pwrite_info->new_value=0;

                }
                IS_SEND=TRUE;
            }

            if(lRow>=0&&lRow<=7)
            {
                CTstatRangeDlg   dlg;
                int rangevalue=m_tstat_input_data.at(lRow).Range.RegValue;
                dlg.m_current_range = rangevalue&0x7F;
                for (int i=0;i<15;i++)
                {
                    if (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(analog_range_TSTAT6[i])==0)
                    {
                        dlg.m_current_range=i;
                        break;
                    }
                }



                dlg.m_10v = rangevalue>>7;


                if (IDOK==dlg.DoModal())
                {   
                    int realRange,dbRange;
                    int range=dlg.m_current_range;
                    //m_input_list.SetItemText(lRow,lCol,analog_range[range]);
                    realRange=range;
                    rangevalue = dlg.m_10v<<7;

                    dbRange=range;
                    pwrite_info->Changed_Name.Format(_T("%s's Range,From %s to %s"),m_tstat_input_data.at(lRow).InputName.StrValue,
                        m_tstat_input_data.at(lRow).Range.StrValue,
                        analog_range_TSTAT6[range]);
                    pwrite_info->address=m_tstat_input_data.at(lRow).Range.regAddress;
                    pwrite_info->new_value=realRange+rangevalue;
                    pwrite_info->db_value=dbRange;
                    IS_SEND=TRUE;
                }
                else{
                    IS_SEND=FALSE;
                }

            }
        }

    }
	//CAL
    if (lCol == 6)
    {

        if (product_type != CS3000){
            //开关量和不可用，无应用
            if((m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(NO_APPLICATION)==0)||
                (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("UNUSED"))==0)||
                (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("On/Off"))==0)||
                (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("Off/On"))==0)||
                (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("Occupied/Unoccupied"))==0)||
                (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("Unoccupied/Occupied"))==0)||
                (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("Close/Open"))==0)||
                (m_tstat_input_data.at(lRow).Range.StrValue.CompareNoCase(_T("Open/Close"))==0)
                )
            {
                m_input_list.Set_Edit(false);
                IS_SEND=FALSE;
                return;
            }
        }
        else{
            //模拟量
            m_input_list.SetItemText(lRow,lCol,L"");
            m_input_list.Set_Edit(TRUE);
        }

    }
	//AM
    if (lCol == 2)
    {
        if (m_tstat_input_data.at(lRow).AM.StrValue.CompareNoCase(NO_APPLICATION)==0)
        {
            IS_SEND=FALSE;
            return;
        }

        if (lRow==8||lRow==9||lRow==10||lRow==11)
        {	 
            if(m_tstat_input_data.at(lRow).AM.StrValue.CompareNoCase(_T("Manual"))!=0)
            {

                pwrite_info->Changed_Name.Format(_T("%s,From %s to Manual"),m_tstat_input_data.at(lRow).InputName.StrValue,m_tstat_input_data.at(lRow).AM.StrValue);
                pwrite_info->address=m_tstat_input_data.at(lRow).AM.regAddress;
                pwrite_info->new_value=1;

            } 
            else
            {
                pwrite_info->Changed_Name.Format(_T("%s,From %s to Auto"),m_tstat_input_data.at(lRow).InputName.StrValue,m_tstat_input_data.at(lRow).AM.StrValue);
                pwrite_info->address=m_tstat_input_data.at(lRow).AM.regAddress;
                pwrite_info->new_value=0;
            }   
        }
        if (lRow>=0&&lRow<=7)
        {

            int AM=1;
            bitset<16> RegValue(m_tstat_input_data.at(lRow).AM.RegValue);
            if(m_tstat_input_data.at(lRow).AM.StrValue.CompareNoCase(_T("Manual"))!=0)
            {

                pwrite_info->Changed_Name.Format(_T("%s,From %s to Manual"),m_tstat_input_data.at(lRow).InputName.StrValue,m_tstat_input_data.at(lRow).AM.StrValue);
                pwrite_info->address=m_tstat_input_data.at(lRow).AM.regAddress;   
                RegValue[lRow]=true;
                pwrite_info->new_value=(short)RegValue.to_ulong();

            } 
            else
            {
                pwrite_info->Changed_Name.Format(_T("%s,From %s to Auto"),m_tstat_input_data.at(lRow).InputName.StrValue,m_tstat_input_data.at(lRow).AM.StrValue);
                pwrite_info->address=m_tstat_input_data.at(lRow).AM.regAddress;
                RegValue[lRow]=FALSE;
                pwrite_info->new_value=(short)RegValue.to_ulong();
            }   
        }


        IS_SEND=TRUE;


    }
	//Custom Table
     if (lCol == 9)
     {
 //         if (m_tstat_input_data.at(lRow).CustomTable.StrValue.CompareNoCase(NO_APPLICATION)==0)

 //         {

 //             IS_SEND=FALSE;

 //             return;

 //         }

 //         else if (m_tstat_input_data.at(lRow).CustomTable.StrValue.CompareNoCase(L"Custom1...")==0)

 //         {

 //            // CBuildTable1 Dlg(2);


 //           //  Dlg.DoModal();

 //         }

 //         else if (m_tstat_input_data.at(lRow).CustomTable.StrValue.CompareNoCase(L"Custom2...")==0)

 //         {

 //            // CBuildTable1 Dlg(3);


 //             //Dlg.DoModal();

 //         }

 
     }
	 //Filter
    if (lCol == 8)
    {
        if(m_tstat_input_data.at(lRow).Filter.StrValue.CompareNoCase(NO_APPLICATION)==0){
            m_input_list.Set_Edit(false);
            IS_SEND=FALSE;
            return;
        }

    }
	//Function
    if (lCol == 8)
    { 
        if(m_tstat_input_data.at(lRow).Function.StrValue.CompareNoCase(NO_APPLICATION)==0){
            m_input_list.Set_Edit(false);
            IS_SEND=FALSE;
            return;
        }

        if (lRow>=0&&lRow<=7)
        {
        }
        else{
            m_input_list.Set_Edit(false);
            return;
        }

    }


    if (IS_SEND)
    {	
        if (pwrite_info->address==NO_ADDRESS)
        {
            m_input_list.Set_Edit(false);
            return;
        }

        pwrite_info->device_id=g_tstat_id;
        pwrite_info->hwnd=g_hwnd_now;
        pwrite_info->mCol=lCol;
        pwrite_info->mRow=lRow;
        m_input_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
        if(!PostThreadMessage(nThreadID,MY_WRITE_ONE_LIST,(WPARAM)pwrite_info,NULL))//post thread msg
        {
            return  ;
        }
        else
        {
            return  ;
        }
    }
    else{
        if (pwrite_info)
        {
            delete pwrite_info;
        }

    }
}
void CTStatInputView::Fresh()
{
    b_hum_sensor = false;
    b_co2_sensor = false;
    b_lux_sensor = false;
  Initial_ListFor_Tstat();
  PostMessage(WM_REFRESH_BAC_INPUT_LIST,NULL,NULL);
  if (m_Fresh_BackgroundThreadHandle!=NULL)
  {
      BOOL bRet = TerminateThread(m_Fresh_BackgroundThreadHandle->m_hThread,0);
      m_Fresh_BackgroundThreadHandle=NULL;
  }
  m_Fresh_BackgroundThreadHandle = AfxBeginThread(BackMainUIFresh_TstatInput,this);
}
void CTStatInputView::Initial_ListFor_Tstat(){
    m_input_list.ShowWindow(SW_HIDE);
    m_input_list.DeleteAllItems();
    while ( m_input_list.DeleteColumn (0)) ;

    if (product_type==CS3000)
    {
        m_input_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
        m_input_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_input_list.InsertColumn(1, _T("Full Label"), 140, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(2, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(3, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(4, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(5, _T("Range"), 100, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(6, _T("Calibration"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(7, _T("Filter"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(8, _T("Function"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(9, _T("Custom Table"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

        CString strTemp;
        //m_input_list.DeleteAllItems();
        for (int i=0;i<=1;i++)
        {
            strTemp.Format(_T("%d"),i+1);
            m_input_list.InsertItem(i,strTemp);
            for (int x=0;x<INPUT_COL_NUMBER;x++)
            {
                if((i%2)==0)
                    m_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
                else
                    m_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
            }
        }

        for (int i=0;i<=1;i++)
        {
            if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(5))
            {
                ListCtrlEx::CStrList strlist;
                for (int j=0;j<3;j++)
                {
                    strlist.push_back(CS3000_INPUT_RANGE[j]);
                }
                m_input_list.SetCellStringList(i, 5, strlist);		
            }
        } 
    }
    else
    {
        m_input_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_input_list.SetExtendedStyle(m_input_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
        m_input_list.InsertColumn(0, _T("NUM"), 50, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_input_list.InsertColumn(1, _T("Full Label"), 140, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(2, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(3, _T("Value"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(4, _T("Units"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(5, _T("Range"), 100, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(6, _T("Calibration"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(7, _T("Filter"), 80, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(8, _T("Function"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_input_list.InsertColumn(9, _T("Custom Table"), 0, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);

        CString strTemp;
        //m_input_list.DeleteAllItems();
        for (int i=0;i<=11;i++)
        {
            strTemp.Format(_T("%d"),i+1);
            m_input_list.InsertItem(i,strTemp);
            for (int x=0;x<INPUT_COL_NUMBER;x++)
            {
                if((i%2)==0)
                    m_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
                else
                    m_input_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
            }
        }

        for (int i=0;i<=7;i++)
        {
            if(ListCtrlEx::ComboBox == m_input_list.GetColumnType(8))
            {
                ListCtrlEx::CStrList strlist;
                for (int j=0;j<(int)sizeof(INPUT_FUNS)/sizeof(INPUT_FUNS[0]);j++)
                {
                    strlist.push_back(INPUT_FUNS[j]);
                }
                m_input_list.SetCellStringList(i, 8, strlist);		
            }
        }

    }
    g_hwnd_now = this->m_hWnd;
  
    CRect list_rect,win_rect;
    m_input_list.GetWindowRect(list_rect);
    ScreenToClient(&list_rect);
    ::GetWindowRect(g_hwnd_now,win_rect);
    m_input_list.Set_My_WindowRect(win_rect);
    m_input_list.Set_My_ListRect(list_rect);
    m_input_list.ShowWindow(SW_SHOW);
}

void CTStatInputView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    CRect ViewRect;
    GetClientRect(&ViewRect);

    if (m_input_list.GetSafeHwnd())
    {
        m_input_list.MoveWindow(CRect(0,0,ViewRect.Width(),ViewRect.Height()),TRUE);
    }
   
}
 
void CTStatInputView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);

    if (nType==SIZE_RESTORED)
    {

        CRect ViewRect;
        GetClientRect(&ViewRect);
         
        if (m_input_list.GetSafeHwnd())
        {
            m_input_list.MoveWindow(CRect(0,0,ViewRect.Width(),ViewRect.Height()),TRUE);
        }

    }

 
}


BOOL CTStatInputView::PreTranslateMessage(MSG* pMsg)
{
    
    if((pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN))
    {
        CRect list_rect,win_rect;
        m_input_list.GetWindowRect(list_rect);
        ScreenToClient(&list_rect);
        ::GetWindowRect(this->m_hWnd,win_rect);
        m_input_list.Set_My_WindowRect(win_rect);
        m_input_list.Set_My_ListRect(list_rect);

        m_input_list.Get_clicked_mouse_position();
        return TRUE;
    }
    CMainFrame* pFrame=(CMainFrame*)(AfxGetApp()->m_pMainWnd);
    if (pFrame->m_pDialogInfo != NULL &&pFrame->m_pDialogInfo->IsWindowVisible())
    {
        if (pMsg->message == WM_LBUTTONDOWN||pMsg->message == WM_RBUTTONDOWN)
        {
            //  pMain->m_pDialogInfo->ShowWindow(SW_HIDE);
            ::PostMessage(MainFram_hwd,WM_LBUTTONDOWN,0,0);
        }
    }
    return CFormView::PreTranslateMessage(pMsg);
}

	
BOOL CTStatInputView::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: Add your message handler code here and/or call default


	HWND hWnd;

	if (pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szTstatHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
	else
		hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szTstatHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_INPUTTABLE_TSTAT);

	return (hWnd != NULL);

	
	return CFormView::OnHelpInfo(pHelpInfo);
}
