// WriteMultiRegistersDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Modbus Poll.h"
#include "WriteMultiRegistersDlg.h"
#include "afxdialogex.h"
#include "EnterValueDlg.h"
#include "global_function.h"
#include "Enter_Value_BinaryDlg.h"
#include "ModbusDllForVC.h"
// CWriteMultiRegistersDlg dialog

IMPLEMENT_DYNAMIC(CWriteMultiRegistersDlg, CDialogEx)

CWriteMultiRegistersDlg::CWriteMultiRegistersDlg(UINT Slave_ID,UINT address,UINT quantity,unsigned short *databuffer,UINT dataformat,BOOL Base_0,CWnd* pParent)
	: CDialogEx(CWriteMultiRegistersDlg::IDD, pParent)
{

	//  m_slave_id = 0;
	m_relative_address=m_address =address;
	m_slave_id = Slave_ID;
	m_quantity =quantity;
	m_dataformate=dataformat;
	m_DataBuffer=databuffer;
	m_base0=Base_0;
}

CWriteMultiRegistersDlg::~CWriteMultiRegistersDlg()
{

}

void CWriteMultiRegistersDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
 
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_relative_address);

	DDX_Text(pDX, IDC_EDIT_ID, m_slave_id);
	DDX_Text(pDX, IDC_EDIT_QUANTITY, m_quantity);
	DDX_Control(pDX, IDC_LIST3, m_ListBox_registers);
}


BEGIN_MESSAGE_MAP(CWriteMultiRegistersDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CWriteMultiRegistersDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_ADDRESS, &CWriteMultiRegistersDlg::OnEnKillfocusEditAddress)
	ON_EN_KILLFOCUS(IDC_EDIT_QUANTITY, &CWriteMultiRegistersDlg::OnEnKillfocusEditQuantity)
	ON_LBN_DBLCLK(IDC_LIST3, &CWriteMultiRegistersDlg::OnLbnDblclkList3)
	ON_EN_KILLFOCUS(IDC_EDIT_ID, &CWriteMultiRegistersDlg::OnEnKillfocusEditId)
	ON_BN_CLICKED(IDC_BUTTON1, &CWriteMultiRegistersDlg::OnBnClickedButton1)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CWriteMultiRegistersDlg message handlers
BOOL CWriteMultiRegistersDlg::OnInitDialog(){
	
	UpdateData(FALSE);
	CString title;
	if (m_dataformate==0)
	{
		title=_T("16: Write multiple registers(Signed)");
	} 
	else if (m_dataformate==1)
	{
		title=_T("16: Write multiple registers(Unsigned)");
	} 
	else if (m_dataformate==2)
	{
		title=_T("16: Write multiple registers(Hex)");
	} 
	else if (m_dataformate==3)
	{
		title=_T("16: Write multiple registers(Binary)");
	} 
	else 
	{
		title=_T("16: Write multiple registers");
	} 
	SetWindowTextW(title);
	FreshListbox();

	return CDialogEx::OnInitDialog();
}
void CWriteMultiRegistersDlg::FreshListbox(){
	
	CString data;
	m_ListBox_registers.ResetContent();
	for(int i=0;i<m_quantity;i++){
		m_ListBox_registers.InsertString(i,Get_Data_No_Address(i));
		
	}
}
void CWriteMultiRegistersDlg::OnBnClickedOk()
{
	OnLbnDblclkList3();
	 
	//CDialogEx::OnOK();
}

CString CWriteMultiRegistersDlg::Get_Data_No_Address(int index){
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

unsigned short CWriteMultiRegistersDlg::Get_Data_Unsigned(int index){
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
int CWriteMultiRegistersDlg::Get_Reg_Add(int index){
	if (m_base0!=0)
	{
		return 1+index+m_relative_address;
	} 
	else
	{
		return index+m_relative_address;
	}
}

void CWriteMultiRegistersDlg::OnEnKillfocusEditId()
{
	UpdateData(TRUE);
}

void CWriteMultiRegistersDlg::OnEnKillfocusEditAddress()
{
	 UpdateData(TRUE);
	 if (m_address>m_relative_address)
	 {
		 AfxMessageBox(_T("Input Error!"));
		 return;
	 }
	 FreshListbox();
}


void CWriteMultiRegistersDlg::OnEnKillfocusEditQuantity()
{
	 UpdateData(TRUE);
	 FreshListbox();
}

void CWriteMultiRegistersDlg::Transfer_Data(int index){
	

}
void CWriteMultiRegistersDlg::OnLbnDblclkList3()
{
    int sel=m_ListBox_registers.GetCurSel();
	
	if (m_dataformate<3)
	{
		int index=0;
		CString itemstr;
		  index=m_ListBox_registers.GetCurSel();
		  if (index==-1)
		  {
			  index=0;
		  }
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
		if (index==-1)
		{
			index=0;
		}
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


//∂‡–¥
void CWriteMultiRegistersDlg::OnBnClickedButton1()
{	
int Send_length;
int Rev_length;
CString temp;
unsigned char rev_back_rawData[300],send_data[100];
 CString m_Tx,m_Rx;

 
	 UpdateData(TRUE);

	// unsigned short *Data_Buffer;
	// Data_Buffer=new unsigned short [m_quantity];
	 
	  unsigned short Data_Buffer[10];
	 CString itemstr;
	 unsigned short Ret_Value;
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
	 else
	 {
        AfxMessageBox(_T("OK!"));
	 }

}


void CWriteMultiRegistersDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialogEx::OnPaint() for painting messages
	m_ListBox_registers.SetCurSel(0);

}
