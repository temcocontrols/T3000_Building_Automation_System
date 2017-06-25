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
	int Count = m_List_Box.GetCount();//获取增加的地址总数量
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
			//m_List_Box.InsertString(m_outputboardsum+1,strtmp);//这样写的话，界面上看不到。但将这行代码放在for循环中则可以
			//m_List_Box.AddString(m_straddress);//注意要将控件中的sort这项设为fase，否则增加的这一项不是放在listbox的最后面，按字母或数字顺序的
			m_List_Box.AddString(strtmp);
			m_List_Box.SetCurSel(m_outputboardsum);//索引从0开始,这里选中的是新增加的这一项
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

		// 	AddString( "xxx ")---------在listbox的最后位置添加 
		// 	InsertString(index, "xxx ")----------在指定的index位置添加 "xxx " 
		// 	CString   str; 
		// 	GetText(index,str)--------获取在index处的文本，保存在str中
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

// 	theApp.pCWnd   =   this;   //这样写 
// 
// 	//   这样写也可以，ID必须是对话框上的控件，得到的指针也是控件类的指针 
// 	theApp.pCWnd   =   GetDlgItem(B的ID);

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
			m_List_Box.InsertString(i,strtmp);//这样写的话，界面上看不到。但将这行代码放在for循环中则可以
			//m_List_Box.AddString(m_straddress);//注意要将控件中的sort这项设为fase，否则增加的这一项不是放在listbox的最后面，按字母或数字顺序的
			//m_List_Box.AddString(strtmp);
			//m_List_Box.SetCurSel(m_outputboardsum);//索引从0开始,这里选中的是新增加的这一项
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

		// 	AddString( "xxx ")---------在listbox的最后位置添加 
		// 	InsertString(index, "xxx ")----------在指定的index位置添加 "xxx " 
		// 	CString   str; 
		// 	GetText(index,str)--------获取在index处的文本，保存在str中
	}
	else
		AfxMessageBox(_T("Please enter modbus address from 1 to 200."));

}
}