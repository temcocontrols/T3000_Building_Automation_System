 // LightingSet.cpp : implementation file
//

#include "stdafx.h"
#include "../T3000.h"
#include "../LightingController/LightingSet.h"


// CLightingSet dialog

IMPLEMENT_DYNAMIC(CLightingSet, CDialog)

CLightingSet::CLightingSet(CWnd* pParent /*=NULL*/)
	: CDialog(CLightingSet::IDD, pParent)
	, m_outputboardsum(0)
	, m_intaddress(1)
{

}

CLightingSet::~CLightingSet()
{
}

void CLightingSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LCSET, m_List_Box);
	DDX_Text(pDX, IDC_EDIT1, m_outputboardsum);
	DDX_Text(pDX, IDC_EDIT4, m_intaddress);
}


BEGIN_MESSAGE_MAP(CLightingSet, CDialog)
	ON_BN_CLICKED(IDOK, &CLightingSet::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CLightingSet::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLightingSet::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_Modify, &CLightingSet::OnBnClickedButtonmodify)
	ON_LBN_SELCHANGE(IDC_LIST_LCSET, &CLightingSet::OnLbnSelchangeListLcset)

END_MESSAGE_MAP()


// CLightingSet message handlers

void CLightingSet::OnBnClickedOk()
{
	int Count = m_List_Box.GetCount();//��ȡ���ӵĵ�ַ������ - // Get the total number of addresses added
	m_mapoutputaddress.clear();

	if(Count!= 0)
	{
		for (int i = 0;i<Count;i++)
		{
			CString strtmp;
			m_List_Box.GetText(i,strtmp);
			m_mapoutputaddress.insert(MAP_OUTPUTADDRESS::value_type(i+1,strtmp));
		}
	}

	OnOK();
}

void CLightingSet::OnBnClickedButton1()//add
{
	UpdateData();
// 	if (m_straddress.CompareNoCase(_T(""))!=0)
// 	{

	int index=0;
	CString strtmp;
	if (m_intaddress>0&&m_intaddress<201)
	{
		
		strtmp.Format(_T("%d"),m_intaddress);
		if((m_List_Box.FindString(index,strtmp)) == LB_ERR)
		{
			//����д�Ļ��������Ͽ��������������д������forѭ��������� - // This way, it cannot be seen on the interface, but it can be seen if this line of code is placed in the for loop
			//m_List_Box.InsertString(m_outputboardsum+1,strtmp);
			//ע��Ҫ���ؼ��е�sort������Ϊfase���������ӵ���һ��Ƿ���listbox������棬����ĸ������˳��� - // Note that the sort option in the control should be set to false, otherwise the added item will not be placed at the end of the listbox, sorted by letters or numbers
			//m_List_Box.AddString(m_straddress);
			m_List_Box.AddString(strtmp);
			m_List_Box.SetCurSel(m_outputboardsum);//������0��ʼ,����ѡ�е��������ӵ���һ�� - // The index starts from 0, and the newly added item is selected here
			m_outputboardsum+=1;
			m_intaddress++;
		
		}
		else
		{
			AfxMessageBox(_T("address already exists!"));
			m_List_Box.GetText(m_outputboardsum-1,strtmp);
			m_intaddress = _ttoi(strtmp);
			m_intaddress++;
		}
			UpdateData(FALSE);

		// 	AddString( "xxx ")---------��listbox�����λ����� - // AddString("xxx ") - Add "xxx " at the end of the listbox
		// 	InsertString(index, "xxx ")----------��ָ����indexλ����� "xxx " - // Insert "xxx " at the specified index position
		// 	CString   str; 
		// 	GetText(index,str)--------��ȡ��index�����ı���������str�� - // GetText(index, str) - Get the text at index and store it in str
	}
	else
		AfxMessageBox(_T("Please enter modbus address from 1 to 200."));
//	}
	
}

BOOL CLightingSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	
//	m_outputboardsum = 1;
// 	for (int i = 0;i<5;i++)
// 	{
// 		m_List_Box.InsertString(i,_T("ddd"));
// 	}
	
//	m_List_Box.AddString()
	//UpdateData(FALSE);

// 	theApp.pCWnd   =   this;   //����д - Write like this
// 
// 	//   ����дҲ���ԣ�ID�����ǶԻ����ϵĿؼ����õ���ָ��Ҳ�ǿؼ����ָ��  - //   // This can also be written, the ID must be the control on the dialog box, and the pointer obtained is also a pointer to the control class
// 	theApp.pCWnd   =   GetDlgItem(B��ID);

// 	CWnd*pCWnd  = (CLightingController)GetDlgItem(IDD_DIALOG_LIGHTINGCONTROLLER);
// 	m_mapoutputaddress = pCWnd->m_mapoutaddress;

//  	CLightingController dlg;
//  	m_mapoutputaddress = dlg.m_mapoutaddress;
//		m_intaddress = dlg.m_inaddress;

// 	CMainFrame* pMain = (CMainFrame*)AfxGetApp()->m_pMainWnd;
// 	CLightingController* pCLightingController = (CLightingController*)pMain;
// 	m_mapoutputaddress = pCLightingController->m_mapoutaddress;
// 	m_intaddress = pCLightingController->m_inaddress;


	MAP_OUTPUTADDRESS::iterator iter;
	CString strtmp;
	for (iter = m_mapoutputaddress.begin();iter != m_mapoutputaddress.end();iter++)
	{
		m_List_Box.AddString(iter->second);
		m_outputboardsum++;
		strtmp = iter->second;
	}

	//m_intaddress = atoi((LPSTR)(LPCTSTR)strtmp);
	m_intaddress = _ttoi(strtmp);
	if (m_intaddress != 0)
		m_List_Box.SetCurSel(m_outputboardsum-1);
	else
		m_intaddress = 1;
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLightingSet::OnBnClickedButton2() //delete
{
	int flg = m_List_Box.GetCount();
	if (flg !=0)
	{
		CString strtmp;
		int index = m_List_Box.GetCurSel();
		m_List_Box.DeleteString(index);
		m_outputboardsum-=1;
		if (flg != 1)
		{	
			if (index == 0)
				index = m_outputboardsum;
			m_List_Box.SetCurSel(index-1);
			m_List_Box.GetText(index-1,strtmp);
		//	m_intaddress = atoi((LPSTR)(LPCTSTR)strtmp);
			m_intaddress = _ttoi(strtmp);
		}else
		{
			m_intaddress = 1;

		}

		UpdateData(FALSE);
	}
	else
	{
		AfxMessageBox(_T("There are no items can be deleted!"));
	}
}

void CLightingSet::OnLbnSelchangeListLcset()
{
	CString strtmp;
	int index = m_List_Box.GetCurSel();
	m_List_Box.GetText(index,strtmp);
	m_intaddress = _ttoi(strtmp);
	UpdateData(FALSE);
}
void CLightingSet::OnBnClickedButtonmodify()
{
	UpdateData();
	int i = m_List_Box.GetCurSel();
	if (i != -1)
	{
	
	int index=0;
	CString strtmp;
	if (m_intaddress>0&&m_intaddress<201)
	{

		strtmp.Format(_T("%d"),m_intaddress);
		if((m_List_Box.FindString(index,strtmp)) == LB_ERR)
		{
			
			m_List_Box.DeleteString(i);
			m_List_Box.InsertString(i,strtmp);//����д�Ļ��������Ͽ��������������д������forѭ��������� - // This way, it cannot be seen on the interface, but it can be seen if this line of code is placed in the for loop
			//m_List_Box.AddString(m_straddress);//ע��Ҫ���ؼ��е�sort������Ϊfase���������ӵ���һ��Ƿ���listbox������棬����ĸ������˳��� - // Note that the sort option in the control should be set to false, otherwise the added item will not be placed at the end of the listbox, sorted by letters or numbers
			//m_List_Box.AddString(strtmp);
			//m_List_Box.SetCurSel(m_outputboardsum);//������0��ʼ,����ѡ�е��������ӵ���һ�� - // The index starts from 0, and the newly added item is selected here
			//m_outputboardsum+=1;
			//m_intaddress++;

		}
		else
		{
			AfxMessageBox(_T("address already exists!"));
			//m_List_Box.GetText(m_outputboardsum-1,strtmp);
			//m_intaddress = _ttoi(strtmp);
			//m_intaddress++;
		}
		UpdateData(FALSE);

		// 	AddString( "xxx ")---------��listbox�����λ����� - // AddString("xxx ") - Add "xxx " at the end of the listbox
		// 	InsertString(index, "xxx ")----------��ָ����indexλ����� "xxx " - // Insert "xxx " at the specified index position
		// 	CString   str; 
		// 	GetText(index,str)--------��ȡ��index�����ı���������str�� - // GetText(index, str) - Get the text at index and store it in str
	}
	else
		AfxMessageBox(_T("Please enter modbus address from 1 to 200."));

}
}