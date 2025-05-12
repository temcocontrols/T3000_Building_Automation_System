// AirflowSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "AirflowSettingDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CAirflowSettingDlg dialog

IMPLEMENT_DYNAMIC(CAirflowSettingDlg, CDialogEx)

CAirflowSettingDlg::CAirflowSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAirflowSettingDlg::IDD, pParent)
{

}

CAirflowSettingDlg::~CAirflowSettingDlg()
{
}

void CAirflowSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAX_SUPPLY, m_Editor_maxsupply);
}


BEGIN_MESSAGE_MAP(CAirflowSettingDlg, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT_PID2OFFSETPOINT2, &CAirflowSettingDlg::OnEnKillfocusEditPid2offsetpoint2)
	ON_EN_KILLFOCUS(IDC_EDIT_PID2OFFSETPOINT4, &CAirflowSettingDlg::OnEnKillfocusEditPid2offsetpoint4)
	ON_EN_KILLFOCUS(IDC_EDIT_PID2OFFSETPOINT3, &CAirflowSettingDlg::OnEnKillfocusEditPid2offsetpoint3)
	ON_EN_KILLFOCUS(IDC_EDIT_PID2OFFSETPOINT5, &CAirflowSettingDlg::OnEnKillfocusEditPid2offsetpoint5)
	ON_BN_CLICKED(IDOK, &CAirflowSettingDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_MAX_SUPPLY, &CAirflowSettingDlg::OnEnKillfocusMaxSupply)
	ON_EN_KILLFOCUS(IDC_MIN_SUPPLY, &CAirflowSettingDlg::OnEnKillfocusMinSupply)
END_MESSAGE_MAP()


// CAirflowSettingDlg message handlers
BOOL CAirflowSettingDlg::Get_Data_Bit(UINT Data,int n,int N)
{
	//Data=Data&0FFx;//去掉高八位数据
	//下面是算幂 2的N
	//最大的num位数的数是？ 2~num-1
	// AfxMessageBox(_T("OK"));
	UINT num=1;
	for (int i=1;i<=n;i++)
	{
		num=num*2;
	}
	num=num-1;//最大的num位数
	Data=Data&num;//取得这个n位数是什么 ？
	num=1;
	for (int i=1;i<N;i++)
	{
		num=num*2;
	}
	if (num==(Data&num))//说明是1 
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}
BOOL CAirflowSettingDlg::OnInitDialog()
{
	  is_have_AVASensor=product_register_value[MODBUS_VAV_CONTROL]&&2;
	Fresh();
CDialogEx::OnInitDialog();
	return TRUE;
}
void CAirflowSettingDlg::Fresh(){
	if(!g_unint)	//t5= 121;t6=104
	{

		m_strUnit=_T("°F");
	}
	else 
	{

		m_strUnit=_T("°C");
	}
	CString strUnit;CString temp,strTemp;
	
	if (is_have_AVASensor)
	{
		strUnit=_T("%");
		temp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_AIRFLOW_SETPOINT]/10.0);
		temp+=strUnit;
		GetDlgItem(IDC_EDIT_PID2OFFSETPOINT2)->SetWindowText(temp);
		temp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_MAX_AIRFLOW_HEATING]/10.0);
		temp+=strUnit;
		GetDlgItem(IDC_EDIT_PID2OFFSETPOINT4)->SetWindowText(temp);
		temp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_MAX_AIRFLOW_COOLING]/10.0);
		temp+=strUnit;
		GetDlgItem(IDC_EDIT_PID2OFFSETPOINT3)->SetWindowText(temp);
		temp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_MIN_AIRFLOW]/10.0);
		temp+=strUnit;
		GetDlgItem(IDC_EDIT_PID2OFFSETPOINT5)->SetWindowText(temp);
	}
	else
	{

	   strUnit=GetTempUnit(product_register_value[MODBUS_ANALOG_IN1], 1);
	   temp.Format(_T("%d"),product_register_value[MODBUS_AIRFLOW_SETPOINT]);
	   temp+=strUnit;
	   GetDlgItem(IDC_EDIT_PID2OFFSETPOINT2)->SetWindowText(temp);
	   temp.Format(_T("%d"),product_register_value[MODBUS_MAX_AIRFLOW_HEATING]);
	   temp+=strUnit;
	   GetDlgItem(IDC_EDIT_PID2OFFSETPOINT4)->SetWindowText(temp);
	   temp.Format(_T("%d"),product_register_value[MODBUS_MAX_AIRFLOW_COOLING]);
	   temp+=strUnit;
	   GetDlgItem(IDC_EDIT_PID2OFFSETPOINT3)->SetWindowText(temp);
	   temp.Format(_T("%d"),product_register_value[MODBUS_MIN_AIRFLOW]);
	   temp+=strUnit;
	   GetDlgItem(IDC_EDIT_PID2OFFSETPOINT5)->SetWindowText(temp);
	}
	
	
	temp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_MAX_SUPPLY_SETPOINT]/10.0);
	temp+=m_strUnit;
	GetDlgItem(IDC_MAX_SUPPLY)->SetWindowText(temp);

	temp.Format(_T("%0.1f"),(float)product_register_value[MODBUS_MIN_SUPPLY_SETPOINT]/10.0);
	temp+=m_strUnit;
	GetDlgItem(IDC_MIN_SUPPLY)->SetWindowText(temp);

}
void CAirflowSettingDlg::OnEnKillfocusEditPid2offsetpoint2()
{
	CString temp;
	GetDlgItem(IDC_EDIT_PID2OFFSETPOINT2)->GetWindowText(temp);
	int Val=_wtoi(temp);
	if (Val==product_register_value[MODBUS_AIRFLOW_SETPOINT])
	{
	return;
	}
	int ret=write_one(g_tstat_id,MODBUS_AIRFLOW_SETPOINT,Val);
	if (ret>0)
	{
	 product_register_value[MODBUS_AIRFLOW_SETPOINT]=Val;
	 temp.Format(_T("%d"),product_register_value[MODBUS_AIRFLOW_SETPOINT]);
	 GetDlgItem(IDC_EDIT_PID2OFFSETPOINT2)->SetWindowText(temp);
	} 
	else
	{
	AfxMessageBox(_T("Write Fail"));
	}
	Fresh();
}


void CAirflowSettingDlg::OnEnKillfocusEditPid2offsetpoint4()
{
	CString temp;
	GetDlgItem(IDC_EDIT_PID2OFFSETPOINT4)->GetWindowText(temp);
	
	if (is_have_AVASensor)
	{
		int Val=(int)(_wtof(temp)*10);
		if (Val==product_register_value[MODBUS_MAX_AIRFLOW_HEATING])
		{
			return;
		}
		int ret=write_one(g_tstat_id,MODBUS_MAX_AIRFLOW_HEATING,Val);
		if (ret>0)
		{
			product_register_value[MODBUS_MAX_AIRFLOW_HEATING]=Val;

		} 
		else
		{
			AfxMessageBox(_T("Write Fail"));
		}
		
	} 
	else
	{int Val=_wtoi(temp);
		if (Val==product_register_value[MODBUS_MAX_AIRFLOW_HEATING])
		{
			return;
		}
		int ret=write_one(g_tstat_id,MODBUS_MAX_AIRFLOW_HEATING,Val);
		if (ret>0)
		{
			product_register_value[MODBUS_MAX_AIRFLOW_HEATING]=Val;

		} 
		else
		{
			AfxMessageBox(_T("Write Fail"));
		}
	}
	
	Fresh();
}


void CAirflowSettingDlg::OnEnKillfocusEditPid2offsetpoint3()
{
	CString temp;
	GetDlgItem(IDC_EDIT_PID2OFFSETPOINT3)->GetWindowText(temp);
	if (is_have_AVASensor)
	{
		int Val=(int)(_wtof(temp)*10);
		if (Val==product_register_value[MODBUS_MAX_AIRFLOW_COOLING])
		{
			return;
		}
		int ret=write_one(g_tstat_id,MODBUS_MAX_AIRFLOW_COOLING,Val);
		if (ret>0)
		{
			product_register_value[MODBUS_MAX_AIRFLOW_COOLING]=Val;
			 
		} 
		else
		{
			AfxMessageBox(_T("Write Fail"));
		}
	}
	else
	{
		int Val=_wtoi(temp);
		if (Val==product_register_value[MODBUS_MAX_AIRFLOW_COOLING])
		{
			return;
		}
		int ret=write_one(g_tstat_id,MODBUS_MAX_AIRFLOW_COOLING,Val);
		if (ret>0)
		{
			product_register_value[MODBUS_MAX_AIRFLOW_COOLING]=Val;
			 
		} 
		else
		{
			AfxMessageBox(_T("Write Fail"));
		}
	}
	
	Fresh();
}


void CAirflowSettingDlg::OnEnKillfocusEditPid2offsetpoint5()
{
	CString temp;
	GetDlgItem(IDC_EDIT_PID2OFFSETPOINT5)->GetWindowText(temp);

	if (is_have_AVASensor)
	{
		int Val=(int)(_wtof(temp)*10);
		if (Val==product_register_value[MODBUS_MIN_AIRFLOW])
		{
			return;
		}
		int ret=write_one(g_tstat_id,MODBUS_MIN_AIRFLOW,Val);
		if (ret>0)
		{
			product_register_value[MODBUS_MIN_AIRFLOW]=Val;

		} 
		else
		{
			AfxMessageBox(_T("Write Fail"));
		}	
	}
	else
	{
		int Val=_wtoi(temp);
		if (Val==product_register_value[MODBUS_MIN_AIRFLOW])
		{
			return;
		}
		int ret=write_one(g_tstat_id,MODBUS_MIN_AIRFLOW,Val);
		if (ret>0)
		{
			product_register_value[MODBUS_MIN_AIRFLOW]=Val;
			 
		} 
		else
		{
			AfxMessageBox(_T("Write Fail"));
		}	
	}
	
	Fresh(); 
}


void CAirflowSettingDlg::OnBnClickedOk()
{
	
	CDialogEx::OnOK();
}


void CAirflowSettingDlg::OnEnKillfocusMaxSupply()
{
	CString temp;
	GetDlgItem(IDC_MAX_SUPPLY)->GetWindowText(temp);
	int Val=(int)(_wtof(temp)*10);
	if (Val==product_register_value[MODBUS_MAX_SUPPLY_SETPOINT])
	{
		return;
	}
	int ret=write_one(g_tstat_id,MODBUS_MAX_SUPPLY_SETPOINT,Val);
	if (ret>0)
	{
		product_register_value[MODBUS_MAX_SUPPLY_SETPOINT]=Val;
		temp.Format(_T("%d"),product_register_value[MODBUS_MAX_SUPPLY_SETPOINT]);
		GetDlgItem(IDC_MAX_SUPPLY)->SetWindowText(temp);
	} 
	else
	{
		AfxMessageBox(_T("Write Fail"));
	}	
	Fresh();	 
}


void CAirflowSettingDlg::OnEnKillfocusMinSupply()
{
	CString temp;
	GetDlgItem(IDC_MIN_SUPPLY)->GetWindowText(temp);
	int Val=(int)(_wtof(temp)*10);
	if (Val==product_register_value[MODBUS_MIN_SUPPLY_SETPOINT])
	{
		return;
	}
	int ret=write_one(g_tstat_id,MODBUS_MIN_SUPPLY_SETPOINT,Val);
	if (ret>0)
	{
		product_register_value[MODBUS_MIN_SUPPLY_SETPOINT]=Val;
		temp.Format(_T("%d"),product_register_value[MODBUS_MIN_SUPPLY_SETPOINT]);
		GetDlgItem(IDC_MIN_SUPPLY)->SetWindowText(temp);
	} 
	else
	{
		AfxMessageBox(_T("Write Fail"));
	}	
	Fresh(); 
}
