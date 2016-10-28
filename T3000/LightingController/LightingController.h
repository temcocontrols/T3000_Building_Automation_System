

#pragma once
#include "afxwin.h"
#include "../msflexgrid1.h"
#include "../LightingController/LightingSet.h"
#include <map>
#include "../LightingController/configure.h"
// #include <vector>
// #include <algorithm>


#include "afxdtctl.h"
#include "atlcomtime.h"
#include "afxwin.h"
#define  WM_WRITE_MESSAGE WM_USER+1050

typedef struct   LIGHTINTGCONTROLLER
{
	int iaddress;
	CString CStvalue;
}lightingcontroller;

typedef struct OUTPUTNAMESTATUS
{
	CString Name;
	CString Status;
}ONS;

typedef struct Output_Mapping_Struct{
	int Address;
	int Card_ID;
	int OuputNo;
	int OutputNameAddress;
} Mapping_Struct;

// CLightingController form view

class CLightingController : public CFormView
{
	DECLARE_DYNCREATE(CLightingController)

protected:
public:
	CLightingController();           // protected constructor used by dynamic creation
	virtual ~CLightingController();

public:
	enum { IDD = IDD_DIALOG_LIGHTINGCONTROLLER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void Fresh();
	void Fresh_OutputGrid(BOOL Input_Group);
	void Fresh_System();
	unsigned int m_inaddress;
	int m_inSerialNum;
	float m_flFirmware;
	int m_inHardware;
	CString m_CStrModel;
	int m_inBaudrate;
	int m_sn;
	int m_input_output;
	void Fresh_ListBox();
	void ShowLighContDlg();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString get_OutputName(int card,int output);
	//////////////////////////////////////////////
	vector<Mapping_Struct> m_input_output_mapping;
	vector<Mapping_Struct> m_group_output_mapping;
	////////////////////////////////////////////////

	CString Read_OutputName(int Address);
	void Read_Input_Group_Mapping(BOOL Input_Group);
	CString m_datetime;
	unsigned short LightCregister[512];
	BOOL prodtopcData();//采集数据
	BOOL checkDB(CString DBname,CString strSQL);//决定是否创建数据库
	 
 
 

	int comnum;//读取串口号
	vector<lightingcontroller>veclightingcontroller;
	lightingcontroller m_veclightingcontroller;

	BOOL m_ArrayOutput[96];
	BOOL InitializeArray(BOOL*ARRAY);
	


	
	CListBox m_ListBox;
	afx_msg void OnLbnDblclkListInput();
	afx_msg void OnLbnSelchangeListInput();


	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);


	WORD* ConvertOutput();
	WORD WordTemp[6];

	CMsflexgrid m_msflexgrid1to96;

	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_comboBox;
	CString m_strcomboBox;

	afx_msg LRESULT OnWriteMessage(WPARAM wParam,LPARAM lParam);

	
	DECLARE_EVENTSINK_MAP()
	void ClickMsflexgrid1();

	void ClickMsflexgrid4();
	 long row,col;
	 void DblClickMsflexgrid1();
	 CEdit m_editName;
	 afx_msg void OnEnKillfocusEditModifyname();
	 afx_msg void OnEnKillfocusEditTimeSaveDelay();
	 //存放输出名称Status
	 vector<ONS>m_vecONS;
	 ONS m_structONS;
  

 



	 //DB and interface data struct
// 	 typedef struct OUT_NUM
// 	 {
// 		 CString addr;
// 		 int      output_no;
// 
// 	 }stru_outnum;
	
//typedef vector<stru_outnum> VECTORT_OUTPUT;
public:
	 typedef struct OUT_PARAMETER
	 {
		 CString outname;
		 CString outstatus;
	 }stru_outparameter;
	 stru_outparameter m_stru_outparam;

	 lightingcontroller m_strutoutaddr;
	
	typedef vector<lightingcontroller> VECTOR_OUT_ADDR;
	VECTOR_OUT_ADDR m_vecaddrinout;

	typedef vector<stru_outparameter> VECTOR_OUT_PARAM;
	VECTOR_OUT_PARAM m_vecoutparam;

public:
	typedef std::map<int,CString> MAP_OUT_ADDRESS;//int表示output板的顺序 CString是output的地址
	MAP_OUT_ADDRESS m_mapoutputaddress;

	

	typedef std::map<CString,MAP_OUT_ADDRESS> MAP_INT_OUT;
	MAP_INT_OUT m_mapinout;


	typedef std::map<CString,VECTOR_OUT_PARAM> MAP_OUT_PARAM;
	MAP_OUT_PARAM m_mapoutputparam;

	int m_outputsum;

	BYTE senddata[100];
	

	int m_nCurRow;
	int m_nCurCol;
	int m_GridNo;
	CString m_oldname;
	CString m_newname;
	afx_msg void OnSetmappingAddoutputbarod();
	afx_msg void OnSetmappingPrevious();
	afx_msg void OnSetmappingNext();
	afx_msg void OnSetmappingRead();
	afx_msg void OnSetmapSettingsave();
	afx_msg void OnSetmappingSand();
	afx_msg void OnBnClickedButtonLightingcontorlWeeklys();
	afx_msg void OnBnClickedButtonLightingcontorlAnnuals();
	afx_msg void OnBnClickedButtonLightingcontorlGroups();
	afx_msg void OnBnClickedButtonLightingcontorlSyncwithPC();
	afx_msg void OnBnClickedButtonSaveAll();
	afx_msg void OnBnClickedButtonConfigureOutputName();
	afx_msg void OnCbnSelchangeValuecombo();

	WORD lightingController_time[8];
	
	COleDateTime m_date;
	COleDateTime m_time;

	void Automationflexrow();
	afx_msg void OnBnClickedButtonAdd();
    afx_msg void OnSave();
    afx_msg void OnRefresh();
    afx_msg void OnReset();
	CComboBox m_comboboxpanel;
	CComboBox m_comboboxouputboard;
	CComboBox m_comboboxoutput;
	CString m_strcomboboxpanel;
	CString m_strcomboboxoutputboard;
	CString m_strcomboboxoutput;


	typedef struct ADDOUTPUTS 
	{
		CString strpanel;
		CString stroutputboard;
		CString stroutputno;
		CString stroutputname;
	}structaddoutputs;
	structaddoutputs m_structaddoutputs;

	typedef vector<structaddoutputs> vecaddoutputs;
	vecaddoutputs m_vecaddoutputs;

	map<CString,vecaddoutputs> m_mapaddoutputs;
	unsigned short SerialNum[60];

	BOOL GetSerialComm(vector<CString>& szComm);//读了串口
	vector<CString> m_szComm;

	int outputno;//output端口号

	 



	 afx_msg void OnBnClickedButtonApply();
	 BOOL CheckSettingChanged();
	 void Fresh_Inputs();
	 CIPAddressCtrl			m_ip_addressCtrl;
	 CIPAddressCtrl			m_subnet_addressCtrl;
	 CIPAddressCtrl			m_gateway_addressCtrl;
	 CEdit m_listenPortEdit;
	 int					m_nListenPort;
	 afx_msg void OnBnClickedButtonReboot();
	 afx_msg void OnBnClickedButtonResetToFactory();

	 afx_msg void OnBnClickedCheckEnableEdit();
	 CButton					m_ReadOnlyCheckBtn;
	 CComboBox m_ipModelComBox;
	 afx_msg void OnBnClickedButtonConfigure();
	 
	 CComboBox m_timeserver;
	 CEdit m_macaddress;
	BOOL m_light[32];
	CMsflexgrid m_inputs_grid;
	CComboBox m_combox_controler;
	CWinThread * LC_Thread;
	CEdit m_savedelaytime;
};



