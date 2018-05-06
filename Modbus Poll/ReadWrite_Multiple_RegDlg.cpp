// ReadWrite_Multiple_RegDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "ReadWrite_Multiple_RegDlg.h"
#include "afxdialogex.h"

#include "EnterValueDlg.h"
#include "global_function.h"
#include "Enter_Value_BinaryDlg.h"
#include "ModbusDllForVC.h"
// CReadWrite_Multiple_RegDlg dialog

IMPLEMENT_DYNAMIC(CReadWrite_Multiple_RegDlg, CDialogEx)

CReadWrite_Multiple_RegDlg::CReadWrite_Multiple_RegDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReadWrite_Multiple_RegDlg::IDD, pParent)
{

	m_result_address = 0;
	m_result_quantity = 0;
}

CReadWrite_Multiple_RegDlg::~CReadWrite_Multiple_RegDlg()
{
}

void CReadWrite_Multiple_RegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_relative_address);

	DDX_Text(pDX, IDC_EDIT_ID, m_slave_id);
	DDX_Text(pDX, IDC_EDIT_QUANTITY, m_quantity);
	DDX_Control(pDX, IDC_LIST3, m_ListBox_registers);
	DDX_Control(pDX, IDC_LIST1, m_result_registers);
	DDX_Text(pDX, IDC_EDIT1, m_result_address);
	DDX_Text(pDX, IDC_EDIT2, m_result_quantity);
}


BEGIN_MESSAGE_MAP(CReadWrite_Multiple_RegDlg, CDialogEx)
	ON_LBN_DBLCLK(IDC_LIST3, &CReadWrite_Multiple_RegDlg::OnLbnDblclkList3)
	ON_BN_CLICKED(IDOK, &CReadWrite_Multiple_RegDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_ID, &CReadWrite_Multiple_RegDlg::OnEnKillfocusEditId)
	ON_EN_KILLFOCUS(IDC_EDIT_ADDRESS, &CReadWrite_Multiple_RegDlg::OnEnKillfocusEditAddress)
	ON_EN_KILLFOCUS(IDC_EDIT_QUANTITY, &CReadWrite_Multiple_RegDlg::OnEnKillfocusEditQuantity)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CReadWrite_Multiple_RegDlg::OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CReadWrite_Multiple_RegDlg::OnEnKillfocusEdit2)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CReadWrite_Multiple_RegDlg::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CReadWrite_Multiple_RegDlg message handlers
BOOL CReadWrite_Multiple_RegDlg::OnInitDialog(){

	UpdateData(FALSE);
	CString title;
	if (m_dataformate==0)
	{
		title=_T("23: Write multiple registers(Signed)");
	} 
	else if (m_dataformate==1)
	{
		title=_T("23: Write multiple registers(Unsigned)");
	} 
	else if (m_dataformate==2)
	{
		title=_T("23: Write multiple registers(Hex)");
	} 
	else if (m_dataformate==3)
	{
		title=_T("23: Write multiple registers(Binary)");
	} 
	else 
	{
		title=_T("23: Write multiple registers");
	} 
	SetWindowTextW(title);
	FreshListbox();

	return CDialogEx::OnInitDialog();
}
void CReadWrite_Multiple_RegDlg::FreshListbox(){

	CString data;
	m_ListBox_registers.ResetContent();
	for(int i=0;i<m_quantity;i++){
		m_ListBox_registers.InsertString(i,Get_Data_No_Address(i));

	}

	m_ListBox_registers.SetCurSel(0);
}
CString CReadWrite_Multiple_RegDlg::Get_Data_No_Address(int index){
	static CString tempStr;
	short int tempVal1;
	unsigned short int tempVal2;
	unsigned short int tempVal3;
	unsigned short int tempVal4;
	unsigned short int bitPosition = 0x8000;
	CString strToReturn;
	int pos=index+(m_relative_address-m_address);
	short int val=(short int )m_DataBuffer[pos];
	switch (m_dataformate)
	{
	case 0:		/* Signed */
		tempVal1 = (short int)val;
		strToReturn.Format(_T("%06d"), tempVal1);
		if (m_base0==0){
			strToReturn.Format(_T("%d"), tempVal1);
		}
		break;
	case 1:		/* Unsigned */
		tempVal2 = (unsigned short int)val;
		strToReturn.Format(_T("%06d"), tempVal2);
		if (m_base0==0){
			strToReturn.Format(_T("%d"), tempVal2);
		}
		break;
	case 2:		/* Hex */
		tempVal3 = (unsigned short int)val;
		strToReturn.Format(_T("0x%04X"), tempVal3);
		break;
	case 3:		/* Binary */
		tempVal4 = (unsigned short int)val;
		tempStr = L"";
		for (int i = 0; i < 16; i++)
		{   

			if ((tempVal4 & bitPosition) == 0)
			{
				tempStr = tempStr + L"0";
				m_Binary_Value[i]=FALSE;
			}
			else
			{
				tempStr = tempStr + L"1";
				m_Binary_Value[i]=TRUE;
			}
			bitPosition = bitPosition >> 1;
			if ((i+1)%4==0)
			{
				tempStr+=L" ";
			}
		}
		strToReturn = tempStr;
		break;
	default: 
		tempVal2 = (unsigned short int)val;
		strToReturn.Format(_T("%06d"), tempVal2);
		break;
	}
	CString ret;
	if (TRUE/*m_base0!=0*/)
	{
		ret.Format(_T("%03d=%s"),Get_Reg_Add(index),strToReturn.GetBuffer());
	} 
	else
	{
		ret.Format(_T("%s"),strToReturn.GetBuffer());
	}


	return ret;
}


int CReadWrite_Multiple_RegDlg::Get_Reg_Add(int index){
	if (m_base0!=0)
	{
		return 1+index+m_relative_address;
	} 
	else
	{
		return index+m_relative_address;
	}
}
void CReadWrite_Multiple_RegDlg::OnLbnDblclkList3()
{	
	int sel=m_ListBox_registers.GetCurSel();
	if (m_dataformate<3)
	{
		CString itemstr;
		int index=m_ListBox_registers.GetCurSel();
		m_ListBox_registers.GetText(index,itemstr);
		CStringArray StrBuffer;
		SplitCStringA(StrBuffer,itemstr,_T("="));

		CEnterValueDlg dlg;
		dlg.m_StrValue=StrBuffer[1];
		dlg.m_dataformate=m_dataformate;
		if (IDOK==dlg.DoModal())
		{
			StrBuffer[1]=dlg.m_StrValue;
		}
		itemstr.Format(_T("%s=%s"),StrBuffer[0].GetBuffer(),StrBuffer[1].GetBuffer());
		m_ListBox_registers.DeleteString(index);
		m_ListBox_registers.InsertString(index,itemstr);

	}
	else if(m_dataformate==3)
	{
		int index=m_ListBox_registers.GetCurSel();

		CString strTemp;CString itemstr;
		m_ListBox_registers.GetText(index,itemstr);

		CStringArray StrBuffer;
		SplitCStringA(StrBuffer,itemstr,_T("="));
		CString Data=StrBuffer[1];

		int len=Data.GetLength();
		int j=15;
		for (int i=0;i<len;i++)
		{
			char temp=Data.GetAt(i);
			if (temp==' ')
			{
				continue;
			} 
			else{
				if (temp!='0')
				{
					m_Binary_Value[j]=TRUE;
				} 
				else
				{
					m_Binary_Value[j]=FALSE;
				}
				--j;
			}

		}
		CEnter_Value_BinaryDlg dlg;
		memcpy_s(dlg.m_Binary_Value,sizeof(dlg.m_Binary_Value),m_Binary_Value,sizeof(m_Binary_Value));
		if (IDOK==dlg.DoModal()){

			memcpy_s(m_Binary_Value,sizeof(m_Binary_Value),dlg.m_Binary_Value,sizeof(dlg.m_Binary_Value));
		} 
		CString StrData=_T("");
		j=0;
		for (int len=15;len>=0;len--)
		{
			char temp;

			if (m_Binary_Value[len])
			{
				temp='1';
			} 
			else
			{
				temp='0';
			}
			StrData.AppendChar(temp);
			if (((++j)%4)==0)
			{
				temp=' ';
				StrData.AppendChar(temp);
			} 
		}
		itemstr.Format(_T("%s=%s"),StrBuffer[0].GetBuffer(),StrData.GetBuffer());
		m_ListBox_registers.DeleteString(index);
		m_ListBox_registers.InsertString(index,itemstr);
	}

	m_ListBox_registers.SetCurSel(sel);
	m_ListBox_registers.SetFocus();
}
unsigned short CReadWrite_Multiple_RegDlg::Get_Data_Unsigned(int index){
	int ret=0;
	CString itemstr;
	m_ListBox_registers.GetText(index,itemstr);
	CStringArray StrBuffer;
	SplitCStringA(StrBuffer,itemstr,_T("="));
	CString StrValue=StrBuffer[1];
	if (m_dataformate<=1)
	{
		StrToIntEx(StrValue.GetBuffer(),STIF_DEFAULT,&ret);
	}
	else if (m_dataformate==2)
	{
		StrToIntEx(StrValue.GetBuffer(),STIF_SUPPORT_HEX,&ret);
	}
	else if (m_dataformate==3)
	{
		int US_temp=0;
		for (int i=0;i<16;i++)
		{
			if (StrValue.GetAt(i)=='1')
			{
				US_temp=1<<i;
				ret+=US_temp;
			}
		}
	}
	return (unsigned short)ret;
}

void CReadWrite_Multiple_RegDlg::OnBnClickedOk()
{
	 
	 UpdateData(TRUE);
	 OnLbnDblclkList3();

	//CDialogEx::OnOK();
}


void CReadWrite_Multiple_RegDlg::OnEnKillfocusEditId()
{
	UpdateData(TRUE);
	FreshListbox();
}


void CReadWrite_Multiple_RegDlg::OnEnKillfocusEditAddress()
{
	UpdateData(TRUE);
	FreshListbox();
	
}


void CReadWrite_Multiple_RegDlg::OnEnKillfocusEditQuantity()
{
	UpdateData(TRUE);
	FreshListbox();
	
}


void CReadWrite_Multiple_RegDlg::OnEnKillfocusEdit1()
{
	UpdateData(TRUE);
	FreshListbox();
	
}


void CReadWrite_Multiple_RegDlg::OnEnKillfocusEdit2()
{
   UpdateData(TRUE);
   FreshListbox();
	
}


void CReadWrite_Multiple_RegDlg::OnBnClickedButtonSend()
{
	int Send_length;
	int Rev_length;
	CString temp;
	unsigned char rev_back_rawData[300],send_data[100];
	CString m_Tx,m_Rx;


	UpdateData(TRUE);

	unsigned short *Data_Buffer;
	Data_Buffer=new unsigned short [m_quantity];
	CString itemstr;
	 
	for(int i=0;i<m_quantity;i++){
		Data_Buffer[i]=Get_Data_Unsigned(i);
       
	}
	register_critical_section.Lock();
	int ret=write_multi_Short_log(m_slave_id,Data_Buffer,m_address,m_quantity,&send_data[0],&rev_back_rawData[0],&Send_length,&Rev_length);
	register_critical_section.Unlock();

	++g_Tx_Rx;
	temp.Format(_T("%06d--"),g_Tx_Rx);
	m_Tx+=temp;

	for (int i=0;i<Send_length;i++)
	{
		temp.Format(_T("%02X "),send_data[i]);
		m_Tx+=temp;
	}
	Traffic_Data(m_Tx);
	++g_Tx_Rx;
	temp.Format(_T("%06d--"),g_Tx_Rx);
	m_Rx+=temp;
	for(int i=0;i<Rev_length;i++){
		temp.Format(_T("%02X "),rev_back_rawData[i]);
		m_Rx+=temp;
	}
	Traffic_Data(m_Rx);
	if (ret>0)
	{
		AfxMessageBox(_T("Write OK!"));
		return;
	}
	

	if (ret<0)
	{
		AfxMessageBox(_T("Write Fail,Try,again!"));
		return;
	} 
}
