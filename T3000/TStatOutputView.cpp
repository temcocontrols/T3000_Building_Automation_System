// TStatOutputView.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "TStatOutputView.h"
#include "BacnetOutput.h"
#include "afxdialogex.h"
#include "global_function.h"
#include "global_define.h"
 
#include <bitset>
#include "MainFrm.h"
// CTStatOutputView

IMPLEMENT_DYNCREATE(CTStatOutputView, CFormView)

CTStatOutputView::CTStatOutputView()
	: CFormView(CTStatOutputView::IDD)
{
       
}

CTStatOutputView::~CTStatOutputView()
{
    
}

void CTStatOutputView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_OUTPUT, m_output_list);
}

BEGIN_MESSAGE_MAP(CTStatOutputView, CFormView)
    ON_MESSAGE(MY_RESUME_DATA, OutputMessageCallBack)
    ON_MESSAGE(WM_REFRESH_BAC_OUTPUT_LIST,Fresh_Output_List)	
    ON_MESSAGE(WM_LIST_ITEM_CHANGED,Fresh_Output_Item)	
    ON_NOTIFY(NM_CLICK, IDC_LIST_OUTPUT, &CTStatOutputView::OnNMClickListOutput)
    ON_WM_SIZE()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


// CTStatOutputView diagnostics

#ifdef _DEBUG
void CTStatOutputView::AssertValid() const
{
	//CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTStatOutputView::Dump(CDumpContext& dc) const
{
	//CFormView::Dump(dc);
}
#endif
#endif //_DEBUG



LRESULT  CTStatOutputView::OutputMessageCallBack(WPARAM wParam, LPARAM lParam)
{
    
    int msg_result=(int) wParam;

    CString temp_task_info;
    CString Show_Results;
   
    LoadTstat_OutputData();
    if (msg_result==1||msg_result==2)//单写
    {
        _MessageWriteOneInfo_List *pInvoke =(_MessageWriteOneInfo_List *)lParam;
        //pInvoke->list_type=LIST_TYPE_OUTPUT_TSTAT; 
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
        Fresh_One_Item(pInvoke->mRow);

        if((pInvoke->mRow%2)==0)//恢复前景和 背景 颜色;
            m_output_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
        else
            m_output_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);

        m_output_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


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
            m_output_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR,0);
        else
            m_output_list.SetItemBkColor(pInvoke->mRow,pInvoke->mCol,LIST_ITEM_DEFAULT_BKCOLOR_GRAY,0);

        m_output_list.RedrawItems(pInvoke->mRow,pInvoke->mRow);


        if(pInvoke)
            delete pInvoke;

    }
    
    return 0;
}
// CTStatOutputView message handlers
LRESULT CTStatOutputView::Fresh_Output_List(WPARAM wParam,LPARAM lParam)
{
    Initial_ListFor_Tstat();
    if (product_type==CS3000)
    {
        m_output_list.SetItemText(0,TStat_OutputName,m_tstat_output_data.at(0).OutputName.StrValue);
        m_output_list.SetItemText(0,TStat_AM ,m_tstat_output_data.at(0).Signal_Type.StrValue);
        m_output_list.SetItemText(0,TStat_Value ,L"0");
        m_output_list.SetItemText(0,TStat_Unit ,m_tstat_output_data.at(0).Range.StrValue);
        m_output_list.SetItemText(0,TStat_Range ,m_tstat_output_data.at(0).Value.StrValue);
    }
    else if(product_type == T3000_6_ADDRESS){
        for (int i=0;i<(int)m_tstat_output_data.size();i++)
        {
            m_output_list.SetItemText(i,TStat_OutputName,m_tstat_output_data.at(i).OutputName.StrValue);
            m_output_list.SetItemText(i,TStat_AM ,m_tstat_output_data.at(i).AM.StrValue);
            m_output_list.SetItemText(i,TStat_Value ,m_tstat_output_data.at(i).Value.StrValue);
            m_output_list.SetItemText(i,TStat_Unit ,m_tstat_output_data.at(i).Unit.StrValue);
            m_output_list.SetItemText(i,TStat_Range ,m_tstat_output_data.at(i).Range.StrValue);
            m_output_list.SetItemText(i,TStat_Funcation ,m_tstat_output_data.at(i).Function.StrValue);
            m_output_list.SetItemText(i,TStat_Interlock ,m_tstat_output_data.at(i).Interlock.StrValue);
            m_output_list.SetItemText(i,TStat_OFFON_Delay ,m_tstat_output_data.at(i).OFFON_Delay.StrValue);
            m_output_list.SetItemText(i,TStat_ONOFF_Delay ,m_tstat_output_data.at(i).ONOFF_Delay.StrValue);
            m_output_list.SetItemText(i,TStat_Signal_Type ,m_tstat_output_data.at(i).Signal_Type.StrValue);
        }
    }
    else{

    }
    return 0;
}

LRESULT CTStatOutputView::Fresh_Output_Item(WPARAM wParam,LPARAM lParam)
{
    int cmp_ret ;//compare if match it will 0;
    int Changed_Item = (int)wParam;
    int Changed_SubItem = (int)lParam;
    CString strText;
    CString temp_task_info;
    CString New_CString =  m_output_list.GetItemText(Changed_Item,Changed_SubItem);
    strText=New_CString;
    CString cstemp_value;
    int lRow=Changed_Item;
    int lCol=Changed_SubItem;
    if (New_CString.CompareNoCase(NO_APPLICATION)==0)
    {
        return 0 ;
    }
    BOOL IS_SEND=FALSE;
    _MessageWriteOneInfo_List  *pwrite_info = new _MessageWriteOneInfo_List;
    pwrite_info->list_type=LIST_TYPE_OUTPUT_TSTAT;
#if 0
    if(lCol == TStat_OutputName)
    {

        if (New_CString.CompareNoCase(m_tstat_output_data.at(Changed_Item).OutputName.StrValue)==0)
        {
            return 0;
        }
        CBADO bado;
        bado.SetDBPath(g_strCurBuildingDatabasefilePath);
        bado.OnInitADOConn(); 
        CString strSerial;
        strSerial.Format(_T("%d"),g_serialNum);

        CString strsql;
        strsql.Format(_T("select * from IONAME where SERIAL_ID = '%s'"),strSerial);
        bado.m_pRecordset=bado.OpenRecordset(strsql);
        //m_RsTmp->Open((_variant_t)strsql,_variant_t((IDispatch *)m_ConTmp,true),adOpenStatic,adLockOptimistic,adCmdText);
        if(VARIANT_FALSE==bado.m_pRecordset->EndOfFile)//update
        {

            CString strField;
            switch (lRow+1)
            {
            case 1:
                strField=_T("OUTPUT1");
                break;
            case 2:
                strField=_T("OUTPUT2");
                break;
            case 3:
                strField=_T("OUTPUT3");
                break;
            case 4:
                strField=_T("OUTPUT4");
                break;
            case 5:
                strField=_T("OUTPUT5");
                break;
            case 6:
                strField=_T("OUTPUT6");
                break;
            case 7:
                strField=_T("OUTPUT7");
                break;
            }

            try
            {

                CString str_temp;
                str_temp.Format(_T("update IONAME set "+strField+" = '"+strText+"' where SERIAL_ID = '"+strSerial+"'"));
                //AfxMessageBox(str_temp );
                bado.m_pConnection->Execute(str_temp.GetString(),NULL,adCmdText);
            }
            catch(_com_error *e)
            {
                AfxMessageBox(e->ErrorMessage());
            }
            // m_FlexGrid.put_TextMatrix(lRow,lCol,strText);

        }
        else//inerst
        {
            switch (lRow+1)
            {
            case 1:
                g_strOutName1=strText;
                break;
            case 2:
                g_strOutName2=strText;
                break;
            case 3:
                g_strOutName3=strText;
                break;
            case 4:
                g_strOutName4=strText;
                break;
            case 5:
                g_strOutName5=strText;
                break;
            case 6:
                g_strOutName6=strText;
                break;
            case 7:
                g_strOutName7=strText;
                break;
            }

            CString	str_temp;
            str_temp.Format(_T("insert into IONAME values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s')"),
                strSerial,
                g_strInName1,
                g_strInName2,
                g_strInName3,
                g_strInName4,
                g_strInName5,
                g_strInName6,
                g_strInName7,
                g_strOutName1,
                g_strOutName2,
                g_strOutName3,
                g_strOutName4,
                g_strOutName5,
                g_strOutName6,
                g_strOutName7,
                g_strInName8,
                g_strInHumName,
                g_strSensorName
                );
            try
            {

                bado.m_pConnection->Execute(str_temp.GetString(),NULL,adCmdText);
            }
            catch(_com_error *e)
            {
                AfxMessageBox(e->ErrorMessage());
            }
            // m_FlexGrid.put_TextMatrix(lRow,lCol,strText);
        }

        switch (lRow+1)
        {
        case 1:
            g_strOutName1=strText;
            break;
        case 2:
            g_strOutName2=strText;
            break;
        case 3:
            g_strOutName3=strText;
            break;
        case 4:
            g_strOutName4=strText;
            break;
        case 5:
            g_strOutName5=strText;
            break;
        case 6:
            g_strOutName6=strText;
            break;
        case 7:
            g_strOutName7=strText;
            break;
        }

        bado.CloseRecordset();
        bado.CloseConn();	

        m_tstat_output_data.at(Changed_Item).OutputName.StrValue=New_CString;

        Fresh_One_Item(Changed_Item);
    }
#endif
    if(lCol == TStat_OutputName)
    {
        if (Changed_Item > 7)
        {
            IS_SEND=FALSE;
            m_output_list.Set_Edit(false);
            return 0;
        }

        CString cs_temp = m_output_list.GetItemText(Changed_Item,Changed_SubItem);
        if(cs_temp.GetLength()> STR_IN_LABEL)	//长度不能大于结构体定义的长度;
        {
            cs_temp.Delete(8,cs_temp.GetLength()-8);
        }

        if (m_tstat_output_data.at(Changed_Item).OutputName.StrValue.CompareNoCase(cs_temp)==0)
        {
            PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
            return 0; 
        }

        char cTemp1[8];
        memset(cTemp1,0,8);
        WideCharToMultiByte( CP_ACP, 0, cs_temp.GetBuffer(), -1, cTemp1, 8, NULL, NULL );

        _MessageWriteMultiInfo_List *pwrite_info = new _MessageWriteMultiInfo_List;
        pwrite_info->device_id=g_tstat_id;
        pwrite_info->Changed_Name.Format(_T("Change outputname%d from %s to %s"),Changed_Item,m_tstat_output_data.at(Changed_Item).OutputName.StrValue,cs_temp);
        pwrite_info->hwnd=g_hwnd_now;
        pwrite_info->mRow=Changed_Item;
        pwrite_info->mCol=Changed_SubItem;
        pwrite_info->Start_Address=m_tstat_output_data.at(Changed_Item).OutputName.regAddress;
        memcpy_s(pwrite_info->RegValue,STR_IN_LABEL-1,cTemp1,STR_IN_LABEL-1);
        m_output_list.SetItemBkColor(Changed_Item,Changed_SubItem,LIST_ITEM_CHANGED_BKCOLOR);
        if(!PostThreadMessage(nThreadID,MY_WRITE_MULTI_LIST,(WPARAM)pwrite_info,NULL))//post thread msg
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    if(lCol == TStat_AM){
        if (product_type==CS3000)
        {
            if (m_tstat_output_data.at(0).Signal_Type.StrValue.CompareNoCase(New_CString)!=0)
            {
                IS_SEND=TRUE;
                pwrite_info->Changed_Name.Format(_T("%s,From %s to %s"),m_tstat_output_data.at(lRow).OutputName.StrValue,m_tstat_output_data.at(lRow).Value.StrValue,New_CString);
                pwrite_info->address=m_tstat_output_data.at(lRow).Signal_Type.regAddress;
                if (New_CString.CompareNoCase(_T("Current"))==0)
                {
                    pwrite_info->new_value=0;
                } 
                else
                {
                    pwrite_info->new_value=1;
                }
            }
        }

    }
    if(lCol == TStat_Unit){
        if (product_type==CS3000)
        {
            if (m_tstat_output_data.at(0).Range.StrValue.CompareNoCase(New_CString)!=0)
            {
                IS_SEND=TRUE;
                pwrite_info->Changed_Name.Format(_T("%s,From %s to %s"),m_tstat_output_data.at(lRow).OutputName.StrValue,m_tstat_output_data.at(lRow).Range.StrValue,New_CString);
                pwrite_info->address=m_tstat_output_data.at(lRow).Range.regAddress; 
                pwrite_info->new_value=_wtoi(New_CString);
            }
        }

    }

    if (lCol == TStat_Value){
        //if (m_tstat_output_data.at(lRow).Value.StrValue.CompareNoCase(New_CString)==0)//选的字符和内存的值相同的话，就返回
        //{
        //	m_output_list.Set_Edit(false);
        //	IS_SEND=FALSE;
        //	return 0;
        //}

        int Output_Value=(int)(_wtof(New_CString)*10);
		if(Output_Value >1000)
		{
			SetPaneString(BAC_SHOW_MISSION_RESULTS,_T("Please input value between 0 - 100 "));
			PostMessage(WM_REFRESH_BAC_OUTPUT_LIST,NULL,NULL);
			return 0;
		}
        if (m_tstat_output_data.at(lRow).Value.RegValue==Output_Value)
        {
            m_output_list.Set_Edit(false);
            IS_SEND=FALSE;
            return 0; 
        }

		IS_SEND=TRUE;
		pwrite_info->Changed_Name.Format(_T("Value,From %u to %s"),m_tstat_output_data.at(lRow).Function.RegValue,New_CString);
		pwrite_info->address=m_tstat_output_data.at(lRow).Value.regAddress;
		pwrite_info->new_value=Output_Value;


    }

    if (lCol == TStat_Range)
    {	
        if (m_tstat_output_data.at(lRow).Range.StrValue.CompareNoCase(New_CString)==0)//选的字符和内存的值相同的话，就返回
        {
            m_output_list.Set_Edit(false);
            IS_SEND=FALSE;
            return 0;
        }
        if (New_CString.CompareNoCase(_T("PWM(0-100%)"))==0)//是PWM
        {
            //检测当前的PWM状态的个数
            int PWMNUMBER =0;
            for (int i=0;i<7;i++)
            {
                if (m_tstat_output_data.at(i).Range.StrValue.CompareNoCase(_T("PWM(0-100%)"))==0)
                {
                    PWMNUMBER++;
                }
            }
            if (PWMNUMBER>=2)
            {
                AfxMessageBox(_T("PWM Mode should be less than two!"));
                m_output_list.Set_Edit(false);
                IS_SEND=FALSE;
                return 0;
            }
        }
        int RangeValue=-1;
        if (product_register_value[7]==1||product_register_value[7]==2||product_register_value[7]==3)
        {
            if (Changed_Item<3)
            {
                for (int i=0;i<3;i++)
                {
                    if (New_CString.CompareNoCase(OUTPUT_RANGE5[i])==0)
                    {
                        RangeValue=i;
                        break;
                    }
                }
            } 
            else if (Changed_Item<5)
            {
                for (int i=0;i<18;i++)
                {
                    if (New_CString.CompareNoCase(OUTPUT_ANRANGE[i])==0)
                    {
                        RangeValue=i;
                        break;
                    }
                }
            }

        }
        else
        {
            if (Changed_Item<5)
            {
                for (int i=0;i<6;i++)
                {
                    if (New_CString.CompareNoCase(OUTPUT_RANGE45[i])==0)
                    {
                        RangeValue=i;
                        break;
                    }
                }
            } 
            else if (Changed_Item<7)
            {
                for (int i=0;i<18;i++)
                {
                    if (New_CString.CompareNoCase(OUTPUT_ANRANGE[i])==0)
                    {
                        RangeValue=i;
                        break;
                    }
                }
            }
        }

        if (RangeValue==-1)
        {
            m_output_list.Set_Edit(false);
            IS_SEND=FALSE;
            return 0;
        }

        IS_SEND=TRUE;
        pwrite_info->Changed_Name.Format(_T("%s,From %s to %s"),m_tstat_output_data.at(lRow).OutputName.StrValue,m_tstat_output_data.at(lRow).Range.StrValue,New_CString);
        pwrite_info->address=m_tstat_output_data.at(lRow).Range.regAddress;
        pwrite_info->new_value=RangeValue;



    }

    if (lCol == TStat_Funcation)
    {
        if (m_tstat_output_data.at(lRow).Function.StrValue.CompareNoCase(New_CString)==0)//选的字符和内存的值相同的话，就返回
        {
            m_output_list.Set_Edit(false);
            IS_SEND=FALSE;
            return 0;
        }
        int RangeValue=-1;
        for (int i=0;i<5;i++)
        {
            if (New_CString.CompareNoCase(ONTPUT_FUNS[i])==0)
            {
                RangeValue=i;
                break;
            }
        }
        if (RangeValue==-1)
        {
            m_output_list.Set_Edit(false);
            IS_SEND=FALSE;
            return 0;
        }
        IS_SEND=TRUE;
        pwrite_info->Changed_Name.Format(_T("%s,From %s to %s"),m_tstat_output_data.at(lRow).OutputName.StrValue,m_tstat_output_data.at(lRow).Function.StrValue,New_CString);
        pwrite_info->address=m_tstat_output_data.at(lRow).Function.regAddress;
        pwrite_info->new_value=RangeValue;
    }
    if (lCol == TStat_Interlock)
    {
        if (m_tstat_output_data.at(lRow).Interlock.StrValue.CompareNoCase(New_CString)==0)//选的字符和内存的值相同的话，就返回
        {
            m_output_list.Set_Edit(false);
            IS_SEND=FALSE;
            return 0;
        }
        int InterlockValue=-1;
        for (int i=0;i<6;i++)
        {
            if (New_CString.CompareNoCase(Interlock[i])==0)
            {
                InterlockValue=i;
                break;
            }
        }
        if (InterlockValue==-1)
        {
            m_output_list.Set_Edit(false);
            IS_SEND=FALSE;
            return 0;
        }
        IS_SEND=TRUE;
        pwrite_info->Changed_Name.Format(_T("%s,From %s to %s"),m_tstat_output_data.at(lRow).OutputName.StrValue,m_tstat_output_data.at(lRow).Interlock.StrValue,New_CString);
        pwrite_info->address=m_tstat_output_data.at(lRow).Interlock.regAddress;
        pwrite_info->new_value=InterlockValue;

    }
    if (lCol == TStat_OFFON_Delay)
    {
        int OFFON_Delay_Value = _wtoi(New_CString);
        if (m_tstat_output_data.at(lRow).OFFON_Delay.RegValue==OFFON_Delay_Value)
        {
            m_output_list.Set_Edit(false);
            IS_SEND=FALSE;
            return 0; 
        }
        IS_SEND=TRUE;
        pwrite_info->Changed_Name.Format(_T("%s,From %s to %s"),m_tstat_output_data.at(lRow).OutputName.StrValue,m_tstat_output_data.at(lRow).OFFON_Delay.StrValue,New_CString);
        pwrite_info->address=m_tstat_output_data.at(lRow).OFFON_Delay.regAddress;
        pwrite_info->new_value=OFFON_Delay_Value;
    }
    if (lCol == TStat_ONOFF_Delay)
    {
        int ONOFF_Delay_Value = _wtoi(New_CString);
        if (m_tstat_output_data.at(lRow).ONOFF_Delay.RegValue==ONOFF_Delay_Value)
        {
            m_output_list.Set_Edit(false);
            IS_SEND=FALSE;
            return 0; 
        }
        IS_SEND=TRUE;
        pwrite_info->Changed_Name.Format(_T("%s,From %s to %s"),m_tstat_output_data.at(lRow).OutputName.StrValue,m_tstat_output_data.at(lRow).ONOFF_Delay.StrValue,New_CString);
        pwrite_info->address=m_tstat_output_data.at(lRow).ONOFF_Delay.regAddress;
        pwrite_info->new_value=ONOFF_Delay_Value;
    }
    if (lCol == TStat_Signal_Type)
    {
        if (m_tstat_output_data.at(lRow).Signal_Type.StrValue.CompareNoCase(New_CString)==0)//选的字符和内存的值相同的话，就返回
        {
            m_output_list.Set_Edit(false);
            IS_SEND=FALSE;
            return 0;
        }
        int RangeValue=-1;
        for (int i=0;i<18;i++)
        {
            if (New_CString.CompareNoCase(OUTPUT_ANRANGE[i])==0)
            {
                RangeValue=i;
                break;
            }
        }
        if (RangeValue==-1)
        {
            m_output_list.Set_Edit(false);
            IS_SEND=FALSE;
            return 0;
        }
        IS_SEND=TRUE;
        pwrite_info->Changed_Name.Format(_T("%s,From %s to %s"),m_tstat_output_data.at(lRow).OutputName.StrValue,m_tstat_output_data.at(lRow).Signal_Type.StrValue,New_CString);
        pwrite_info->address=m_tstat_output_data.at(lRow).Signal_Type.regAddress;
        pwrite_info->new_value=RangeValue;

    } 

    if (IS_SEND)
    {	
        pwrite_info->device_id=g_tstat_id;
        pwrite_info->hwnd=g_hwnd_now;
        pwrite_info->mCol=lCol;
        pwrite_info->mRow=lRow;
        m_output_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
        if(!PostThreadMessage(nThreadID,MY_WRITE_ONE_LIST,(WPARAM)pwrite_info,NULL))//post thread msg
        {
            return FALSE ;
        }
        else
        {
            return TRUE ;
        }
    }
    else{
        if (pwrite_info)
        {
            delete pwrite_info;
        }

    }
    return 0;
}
void CTStatOutputView::OnNMClickListOutput(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    if (product_type==CS3000){
        return;
    }
    long lRow,lCol;
    m_output_list.Set_Edit(true);
    DWORD dwPos=GetMessagePos();//Get which line is click by user.Set the check box, when user enter Insert it will jump to program dialog
    CPoint point( LOWORD(dwPos), HIWORD(dwPos));
    m_output_list.ScreenToClient(&point);
    LVHITTESTINFO lvinfo;
    lvinfo.pt=point;
    lvinfo.flags=LVHT_ABOVE;
    int nItem=m_output_list.SubItemHitTest(&lvinfo);

    lRow = lvinfo.iItem;
    lCol = lvinfo.iSubItem;


    if(lRow>m_output_list.GetItemCount()) //如果点击区超过最大行号，则点击是无效的
        return;
    if(lRow<0)
        return;
    BOOL IS_SEND=FALSE;
    CString New_CString;
    CString temp_task_info;
    CString temp1;
    CStringArray temparray;
    New_CString =  m_output_list.GetItemText(lRow,lCol);
    //if (New_CString.CompareNoCase(NO_APPLICATION)==0)
    //{
    //    m_output_list.Set_Edit(false);
    //    IS_SEND=FALSE;
    //    return ;
    //}
    _MessageWriteOneInfo_List  *pwrite_info = new _MessageWriteOneInfo_List;
    pwrite_info->list_type=LIST_TYPE_OUTPUT_TSTAT;
    if(lCol == TStat_AM)
    {
        bitset<16> RegValue(m_tstat_output_data.at(lRow).AM.RegValue);

        if (m_tstat_output_data.at(lRow).AM.StrValue.CompareNoCase(Global_String_AUTO)==0)
        {
            pwrite_info->Changed_Name.Format(_T("%s,From %s to Manual"),m_tstat_output_data.at(lRow).OutputName.StrValue,m_tstat_output_data.at(lRow).AM.StrValue);
            pwrite_info->address=m_tstat_output_data.at(lRow).AM.regAddress;
            RegValue[lRow]=true;
            pwrite_info->new_value=RegValue.to_ulong();
        } 
        else
        {
            pwrite_info->Changed_Name.Format(_T("%s,From %s to Auto"),m_tstat_output_data.at(lRow).OutputName.StrValue,m_tstat_output_data.at(lRow).AM.StrValue);
            pwrite_info->address=m_tstat_output_data.at(lRow).AM.regAddress;
            RegValue[lRow]=false;
            pwrite_info->new_value=RegValue.to_ulong();
        }
        IS_SEND=TRUE;
    }



    if (lCol == TStat_Value)
    {
        if (m_tstat_output_data.at(lRow).AM.StrValue.CompareNoCase(Global_String_AUTO)==0)
        {
            m_output_list.Set_Edit(false);
            IS_SEND=FALSE;
            return ;
        }
        if (m_tstat_output_data.at(lRow).Range.RegValue==0)
        {
            m_output_list.Set_Edit(false);
        }
        if (m_tstat_output_data.at(lRow).Range.RegValue==0)//Manual+Range=on/off
        {
			if (lRow>4)
			{
				  
				if (m_tstat_output_data.at(lRow).Value.StrValue.CompareNoCase(Global_String_NO) == 0)//如果当前的值是 ON
				{
					pwrite_info->Changed_Name.Format(_T("%s,From %s to %s"), m_tstat_output_data.at(lRow).OutputName.StrValue, m_tstat_output_data.at(lRow).Value.StrValue, Global_String_OFF);
					pwrite_info->address = m_tstat_output_data.at(lRow).Value.regAddress;
					 
					pwrite_info->new_value = 0;
				}
				else
				{
					pwrite_info->Changed_Name.Format(_T("%s,From %s to %s"), m_tstat_output_data.at(lRow).OutputName.StrValue, m_tstat_output_data.at(lRow).Value.StrValue, Global_String_NO);
					pwrite_info->address = m_tstat_output_data.at(lRow).Value.regAddress;
					 
					pwrite_info->new_value = 1;
				}
				IS_SEND = TRUE;
			}
			else
			{
				bitset<16> RegValue(m_tstat_output_data.at(lRow).Value.RegValue);
				if (m_tstat_output_data.at(lRow).Value.StrValue.CompareNoCase(Global_String_NO) == 0)//如果当前的值是 ON
				{
					pwrite_info->Changed_Name.Format(_T("%s,From %s to %s"), m_tstat_output_data.at(lRow).OutputName.StrValue, m_tstat_output_data.at(lRow).Value.StrValue, Global_String_OFF);
					pwrite_info->address = m_tstat_output_data.at(lRow).Value.regAddress;
					RegValue[lRow] = false;
					pwrite_info->new_value = RegValue.to_ulong();
				}
				else
				{
					pwrite_info->Changed_Name.Format(_T("%s,From %s to %s"), m_tstat_output_data.at(lRow).OutputName.StrValue, m_tstat_output_data.at(lRow).Value.StrValue, Global_String_NO);
					pwrite_info->address = m_tstat_output_data.at(lRow).Value.regAddress;
					RegValue[lRow] = true;
					pwrite_info->new_value = RegValue.to_ulong();
				}
				IS_SEND = TRUE;
			}
            
        }

    }
    if (lCol == TStat_Signal_Type)
    {
        if (lRow<5)
        {
            m_output_list.Set_Edit(false);
            return ;
        }
        if (m_tstat_output_data.at(lRow).Function.RegValue!=4)
        {
            m_output_list.Set_Edit(false);
            return ;
        }
    } 




    if (IS_SEND)
    {	
        pwrite_info->device_id=g_tstat_id;
        pwrite_info->hwnd=g_hwnd_now;
        pwrite_info->mCol=lCol;
        pwrite_info->mRow=lRow;
        m_output_list.SetItemBkColor(lRow,lCol,LIST_ITEM_CHANGED_BKCOLOR);
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
    *pResult = 0;
}
void CTStatOutputView::Initial_ListFor_Tstat(){
    m_output_list.ShowWindow(SW_HIDE);
    m_output_list.DeleteAllItems();
    while ( m_output_list.DeleteColumn (0)) ;

    if (product_type==CS3000)
    {
        m_output_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_output_list.SetExtendedStyle(m_output_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
        m_output_list.InsertColumn(OUTPUT_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_output_list.InsertColumn(TStat_OutputName, _T("Full Label"), 140, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_AM, _T("Input Type"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_Value, _T("Min"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_Unit, _T("Max"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_Range, _T("Signal"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        // 		m_output_list.InsertColumn(TStat_Funcation, _T("Function"), 60, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        // 		m_output_list.InsertColumn(TStat_Interlock, _T("Interlock"), 60, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        // 		m_output_list.InsertColumn(TStat_OFFON_Delay, _T("OFF->ON Delay"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        // 		m_output_list.InsertColumn(TStat_ONOFF_Delay, _T("ON->OFF Delay"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        // 		m_output_list.InsertColumn(TStat_Signal_Type, _T("Signal Type"), 70, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        CString strTemp;
        //m_input_list.DeleteAllItems();
        for (int i=0;i<m_tstat_output_data.size();i++)
        {
            strTemp.Format(_T("%d"),i+1);
            m_output_list.InsertItem(i,strTemp); 
        }
        if (ListCtrlEx::ComboBox == m_output_list.GetColumnType(TStat_AM))
        {
            ListCtrlEx::CStrList strlist;
            strlist.clear();
            strlist.push_back(_T("Current"));
            strlist.push_back(_T("Voltage"));
            m_output_list.SetCellStringList(0, TStat_AM, strlist);
        }
    }
    else if(product_type == T3000_6_ADDRESS){
        m_output_list.ModifyStyle(0, LVS_SINGLESEL|LVS_REPORT|LVS_SHOWSELALWAYS);
        m_output_list.SetExtendedStyle(m_output_list.GetExtendedStyle()  |LVS_EX_GRIDLINES&(~LVS_EX_FULLROWSELECT));//Not allow full row select.
        m_output_list.InsertColumn(OUTPUT_NUM, _T("NUM"), 40, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByDigit);
        m_output_list.InsertColumn(TStat_OutputName, _T("Full Label"), 140, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_AM, _T("Auto/Manual"), 80, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_Value, _T("Value"), 60, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_Unit, _T("Units"), 60, ListCtrlEx::Normal, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_Range, _T("Range"), 80, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_Funcation, _T("Function"), 60, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_Interlock, _T("Interlock"), 60, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_OFFON_Delay, _T("OFF->ON Delay"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_ONOFF_Delay, _T("ON->OFF Delay"), 100, ListCtrlEx::EditBox, LVCFMT_CENTER, ListCtrlEx::SortByString);
        m_output_list.InsertColumn(TStat_Signal_Type, _T("Signal Type"), 70, ListCtrlEx::ComboBox, LVCFMT_CENTER, ListCtrlEx::SortByString);

        CString strTemp;
        //m_input_list.DeleteAllItems();
        for (int i=0;i<m_tstat_output_data.size();i++)
        {
            strTemp.Format(_T("%d"),i+1);
            m_output_list.InsertItem(i,strTemp);
            for (int x=0;x<OUTPUT_COL_NUMBER;x++)
            {
                if((i%2)==0)
                    m_output_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR);
                else
                    m_output_list.SetItemBkColor(i,x,LIST_ITEM_DEFAULT_BKCOLOR_GRAY);		
            }
        }


        Initial_Combox_Show_Tstat();
    }
    else{}
    g_hwnd_now = this->m_hWnd;
   
    m_output_list.ShowWindow(SW_SHOW);
}
void   CTStatOutputView::Fresh_One_Item(int row){
    if (product_type==CS3000)
    {
        m_output_list.SetItemText(0,TStat_OutputName,m_tstat_output_data.at(0).OutputName.StrValue);
        m_output_list.SetItemText(0,TStat_AM ,m_tstat_output_data.at(0).Signal_Type.StrValue);
        m_output_list.SetItemText(0,TStat_Value ,L"0");
        m_output_list.SetItemText(0,TStat_Unit ,m_tstat_output_data.at(0).Range.StrValue);
        m_output_list.SetItemText(0,TStat_Range ,m_tstat_output_data.at(0).Value.StrValue);
        return;
    }

    m_output_list.SetItemText(row,TStat_OutputName,m_tstat_output_data.at(row).OutputName.StrValue);
    m_output_list.SetItemText(row,TStat_AM ,m_tstat_output_data.at(row).AM.StrValue);
    m_output_list.SetItemText(row,TStat_Value ,m_tstat_output_data.at(row).Value.StrValue);
    m_output_list.SetItemText(row,TStat_Unit ,m_tstat_output_data.at(row).Unit.StrValue);
    m_output_list.SetItemText(row,TStat_Range ,m_tstat_output_data.at(row).Range.StrValue);
    m_output_list.SetItemText(row,TStat_Funcation ,m_tstat_output_data.at(row).Function.StrValue);
    m_output_list.SetItemText(row,TStat_Interlock ,m_tstat_output_data.at(row).Interlock.StrValue);
    m_output_list.SetItemText(row,TStat_OFFON_Delay ,m_tstat_output_data.at(row).OFFON_Delay.StrValue);
    m_output_list.SetItemText(row,TStat_ONOFF_Delay ,m_tstat_output_data.at(row).ONOFF_Delay.StrValue);
    m_output_list.SetItemText(row,TStat_Signal_Type ,m_tstat_output_data.at(row).Signal_Type.StrValue);
}

void CTStatOutputView::Fresh()
{
    Initial_ListFor_Tstat();
    PostMessage(WM_REFRESH_BAC_OUTPUT_LIST, NULL, NULL);
}

void CTStatOutputView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    CRect ViewRect;
    GetClientRect(&ViewRect);
    if (m_output_list.GetSafeHwnd())
    {
        m_output_list.MoveWindow(CRect(0,0,ViewRect.Width(),ViewRect.Height()),TRUE);
    }
    
}


void CTStatOutputView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);

    if (nType==SIZE_RESTORED)
    {

        CRect ViewRect;
        GetClientRect(&ViewRect);
        if (m_output_list.GetSafeHwnd())
        {
            m_output_list.MoveWindow(CRect(0,0,ViewRect.Width(),ViewRect.Height()),TRUE);
        }

    }
}
void CTStatOutputView::Initial_Combox_Show_Tstat(){
    if (ListCtrlEx::ComboBox == m_output_list.GetColumnType(TStat_Range))
    {
        if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT8)||(product_register_value[7] == PM_TSTAT5i)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			){
            for (int lRow=0;lRow<7;lRow++)
            {
                ListCtrlEx::CStrList strlist;
                strlist.clear();
                if (lRow<5)
                {

                    for (int aaa=0;aaa<(int)sizeof(OUTPUT_RANGE45)/sizeof(OUTPUT_RANGE45[0]);aaa++)
                    {
                        strlist.push_back(OUTPUT_RANGE45[aaa]);
                    }

                }
                else{

                    for (int aaa=0;aaa<5;aaa++)
                    {
                        strlist.push_back(OUTPUT_ANRANGE6[aaa]);
                    }

                }

                m_output_list.SetCellStringList(lRow, TStat_Range, strlist);
            }
        }
        else if (product_register_value[7]==1||product_register_value[7]==2||product_register_value[7]==3){
            for (int lRow=0;lRow<5;lRow++)
            {
                ListCtrlEx::CStrList strlist;
                strlist.clear();
                if (lRow<3)
                {


                    for(int i=0;i<3;i++)//2.5.0.98
                    {

                        strlist.push_back(OUTPUT_RANGE5[i]);
                    }

                } 
                else
                {     
                    if (m_tstat_output_data.at(lRow).Function.RegValue==4)
                    {
                        for(int i=5;i<8;i++)//2.5.0.98
                        {
                            strlist.push_back(OUTPUT_ANRANGE[i]);
                        }
                    }
                    else
                    {
                        for(int i=0;i<4;i++)//2.5.0.98
                        {
                            strlist.push_back(OUTPUT_ANRANGE[i]);
                        }
                    }

                }
                m_output_list.SetCellStringList(lRow, TStat_Range, strlist);
            }
        }
        else if (product_register_value[7]==4||product_register_value[7]==12||product_register_value[7]==17||product_register_value[7]==18){

            for (int lRow=0;lRow<7;lRow++)
            {
                ListCtrlEx::CStrList strlist;
                strlist.clear();
                if (lRow<5)
                {
                    for(int i=0;i<3;i++)//2.5.0.98
                    {
                        strlist.push_back(OUTPUT_RANGE5[i]);
                    }
                } 
                else 
                {
                    if (lRow<7)
                    {
                        if (m_tstat_output_data.at(lRow).Function.RegValue==4)//Transducer
                        {
                            for(int i=5;i<10;i++)//2.5.0.98
                            {
                                strlist.push_back(OUTPUT_ANRANGE[i]);
                            }
                        }
                        else
                        {
                            for(int i=0;i<5;i++)//2.5.0.98
                            {
                                strlist.push_back(OUTPUT_ANRANGE[i]);
                            }
                        }

                    }
                    else
                        return;
                }
                m_output_list.SetCellStringList(lRow, TStat_Range, strlist);
            }
        }else if (product_register_value[7]==PM_TSTAT5E||product_register_value[7]==PM_PM5E||(product_register_value[7]==PM_TSTATRUNAR)){
            for (int lRow=0;lRow<7;lRow++)
            {
                ListCtrlEx::CStrList strlist;
                strlist.clear();
                if (lRow<5)
                {
                    for(int i=0;i<3;i++)//2.5.0.98
                    {
                        strlist.push_back(OUTPUT_RANGE5[i]);
                    }
                } 
                else 
                {
                    if (lRow<7)
                    {
                        if (m_tstat_output_data.at(lRow).Function.RegValue==4)//Transducer
                        {
                            for(int i=5;i<15;i++)//2.5.0.98
                            {
                                strlist.push_back(OUTPUT_ANRANGE[i]);
                            }
                        }
                        else
                        {
                            for(int i=0;i<5;i++)//2.5.0.98
                            {
                                strlist.push_back(OUTPUT_ANRANGE[i]);
                            }
                        }

                    }
                    else
                        return;
                }
                m_output_list.SetCellStringList(lRow, TStat_Range, strlist);
            }
        }
    }

    if (ListCtrlEx::ComboBox == m_output_list.GetColumnType(TStat_Funcation))
    {
        if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT8)||(product_register_value[7] == PM_TSTAT5i)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
        {
            for (int lRow=0;lRow<7;lRow++)
            {
                ListCtrlEx::CStrList strlist;
                strlist.clear();
                if (lRow<5)
                {
                    for(int i=0;i<3;i++)//2.5.0.98
                    {
                        strlist.push_back(ONTPUT_FUNS[i]);

                    }
                }
                else
                {
                    for(int i=0;i<5;i++)//2.5.0.98
                    {
                        strlist.push_back(ONTPUT_FUNS[i]);
                        //AfxMessageBox(ONTPUT_FUNS[i]);
                    }
                }

                m_output_list.SetCellStringList(lRow, TStat_Funcation, strlist);
            }
        }
        else
        {
#if 1//2.5.0.98
            int m_nModeType=product_register_value[7];
            if (m_nModeType==2||m_nModeType==3||m_nModeType==1)
            {
                for (int lRow=0;lRow<5;lRow++)
                {
                    ListCtrlEx::CStrList strlist;
                    strlist.clear();
                    if(lRow<3)
                    {
                        for(int i=0;i<4;i++)
                        {
                            strlist.push_back(ONTPUT_FUNS[i]);
                        }    
                    }
                    else if(lRow<5)
                    {

                        for(int i=0;i<5;i++)
                        {
                            strlist.push_back(ONTPUT_FUNS[i]);

                        }

                    }
                    else
                    {
                        return;
                    }
                    m_output_list.SetCellStringList(lRow, TStat_Funcation, strlist);
                }
            } 
            else
            {
                for (int lRow=0;lRow<7;lRow++)
                {
                    ListCtrlEx::CStrList strlist;
                    strlist.clear();

                    if(lRow<5)
                    {

                        for(int i=0;i<4;i++)
                        {
                            strlist.push_back(ONTPUT_FUNS[i]);
                        }

                    }
                    else
                    {

                        for(int i=0;i<5;i++) 
                        {
                            strlist.push_back(ONTPUT_FUNS[i]);
                        }

                    }
                    m_output_list.SetCellStringList(lRow, TStat_Funcation, strlist);
                }
            }

#endif
        }
    }

    if (ListCtrlEx::ComboBox == m_output_list.GetColumnType(TStat_Interlock)){
        int m_nModeType=product_register_value[7];
        if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT8)||(product_register_value[7] == PM_TSTAT5i)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			)
        {
            for (int lRow=0;lRow<7;lRow++)
            {
                ListCtrlEx::CStrList strlist;
                strlist.clear();
                for(int i=0;i<6;i++)
                {
                    strlist.push_back(Interlock[i]);
                    //AfxMessageBox(ONTPUT_FUNS[i]);
                }
                m_output_list.SetCellStringList(lRow, TStat_Interlock, strlist);

            }
        }
#if 1//2.5.0.98
        else if (m_nModeType==2||m_nModeType==3||m_nModeType==1)
        {
            for (int lRow=0;lRow<5;lRow++){
                ListCtrlEx::CStrList strlist;
                strlist.clear();
                if(lRow<3)
                {

                    for(int i=0;i<4;i++)
                    {
                        strlist.push_back(ONTPUT_FUNS[i]);
                    }
                }
                else if(lRow<5)
                {

                    for(int i=0;i<5;i++)
                    {
                        strlist.push_back(ONTPUT_FUNS[i]);

                    }

                }
                else
                {
                    return;
                }
                m_output_list.SetCellStringList(lRow, TStat_Interlock, strlist);
            }


        } 
        else
        {
            for (int lRow=0;lRow<7;lRow++)
            {
                ListCtrlEx::CStrList strlist;
                strlist.clear();
                if(lRow<5)
                {

                    for(int i=0;i<4;i++)
                        //for(int i=0;i<3;i++)//2.5.0.98
                    {
                        strlist.push_back(ONTPUT_FUNS[i]);

                    }

                }
                else
                {

                    for(int i=0;i<5;i++)
                        //for(int i=0;i<3;i++)//2.5.0.98
                    {
                        strlist.push_back(ONTPUT_FUNS[i]);

                    }

                }
                m_output_list.SetCellStringList(lRow, TStat_Interlock, strlist);
            }
        }

#endif
    }

    if (ListCtrlEx::ComboBox == m_output_list.GetColumnType(TStat_Signal_Type)){
        if ((product_register_value[7] == PM_TSTAT6)||(product_register_value[7] == PM_TSTAT7)||(product_register_value[7] == PM_TSTAT8)
			|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
			||(product_register_value[7] == PM_TSTAT5i))
        {
            for (int lRow=0;lRow<7;lRow++)
            {
                ListCtrlEx::CStrList strlist;
                strlist.clear();
                for(int i=5;i<17;i++)//2.5.0.98
                {
                    strlist.push_back(OUTPUT_ANRANGE[i]);
                }
                m_output_list.SetCellStringList(lRow, TStat_Signal_Type, strlist);

            }
        }
    }

}

BOOL CTStatOutputView::PreTranslateMessage(MSG* pMsg)
{
    
    if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
    {
        CRect list_rect,win_rect;
        m_output_list.GetWindowRect(list_rect);
        ScreenToClient(&list_rect);
        ::GetWindowRect(m_output_dlg_hwnd,win_rect);
        m_output_list.Set_My_WindowRect(win_rect);
        m_output_list.Set_My_ListRect(list_rect);

        m_output_list.Get_clicked_mouse_position();
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

#include "Tstat_HelpDoc.h"
BOOL CTStatOutputView::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: Add your message handler code here and/or call default
	

	HWND hWnd;

	if (pHelpInfo->dwContextId > 0) hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szTstatHelpFile, HH_HELP_CONTEXT, pHelpInfo->dwContextId);
	else
		hWnd = ::HtmlHelp((HWND)pHelpInfo->hItemHandle, theApp.m_szTstatHelpFile, HH_HELP_CONTEXT, IDH_TOPIC_OUTPUTTABLE_TSTAT);

	return (hWnd != NULL);
	return CFormView::OnHelpInfo(pHelpInfo);
}
