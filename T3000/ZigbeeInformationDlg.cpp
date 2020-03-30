// ZigbeeInformationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "ZigbeeInformationDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "TstatZigbeeLogic.h"

// CZigbeeInformationDlg dialog

IMPLEMENT_DYNAMIC(CZigbeeInformationDlg, CDialogEx)

CZigbeeInformationDlg::CZigbeeInformationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ZIGBEE, pParent)
{

}

CZigbeeInformationDlg::~CZigbeeInformationDlg()
{
}

void CZigbeeInformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ZIGBEE_TSTATS_TABLE, m_zigbee_tstat_table);
	DDX_Control(pDX, IDC_COMBO_ZIGBEETYPE, m_combox_zigbee);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_combox_channel);
}


BEGIN_MESSAGE_MAP(CZigbeeInformationDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_TOPOLOGICAL, &CZigbeeInformationDlg::OnBnClickedBtnTopological)
	ON_BN_CLICKED(IDC_BUTTON_ZIGBEE_REBOOT, &CZigbeeInformationDlg::OnBnClickedButtonZigbeeReboot)
END_MESSAGE_MAP()


// CZigbeeInformationDlg message handlers


BOOL CZigbeeInformationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString strTemp;
	if ((product_register_value[7] == PM_TSTAT6 || product_register_value[7] == PM_TSTAT8 || product_register_value[7] == PM_TSTAT9
		|| (product_register_value[7] == PM_TSTAT8_WIFI) || (product_register_value[7] == PM_TSTAT8_OCC) || (product_register_value[7] == PM_TSTAT7_ARM) || (product_register_value[7] == PM_TSTAT8_220V)
		) )
	{

		

		GetDlgItem(IDC_STATIC_ZIGBEE_INFORMATION)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ZIGBEE_ID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_ZIGBEE_ID)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ZIGBEE_TYPE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_ZIGBEETYPE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ZIGBEE_CHANNEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_COMBO_CHANNEL)->ShowWindow(SW_SHOW);



		GetDlgItem(IDC_STATIC_ZIGBEE_SV)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_ZIGBEE_SV)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_ZIGBEE_REBOOT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ZIGBEE_SK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_ZIGBEE_SK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_ZIGBEE_MAC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_ZIGBEE_MAC)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_NUMBER_TSSTATS)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_ZIGBEE_TSTATS_TABLE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_TOPOLOGICAL)->ShowWindow(SW_SHOW);

		strTemp.Format(_T("%d"), product_register_value[21]);
		GetDlgItem(IDC_EDIT_ZIGBEE_ID)->SetWindowText(strTemp);
		m_combox_zigbee.ResetContent();
		m_combox_zigbee.AddString(_T("Coordinator"));
		m_combox_zigbee.AddString(_T("Router"));
		if (product_register_value[22] < 2)
		{
			m_combox_zigbee.SetCurSel(product_register_value[22]);
		}
		m_combox_channel.ResetContent();
		for (int channel = 11; channel < 27; channel++)
		{
			strTemp.Format(_T("%d"), channel);
			m_combox_channel.AddString(strTemp);
		}

		int ChannelValue = product_register_value[24] + product_register_value[23] * 65535;
		int ChannelNO = 0;
		if (ChannelValue == 0x04000000)
		{
			ChannelNO = 26;
		}
		else if (ChannelValue == 0x02000000)
		{
			ChannelNO = 25;
		}
		else if (ChannelValue == 0x01000000)
		{
			ChannelNO = 24;
		}
		else if (ChannelValue == 0x00800000)
		{
			ChannelNO = 23;
		}
		else if (ChannelValue == 0x00400000)
		{
			ChannelNO = 22;
		}
		else if (ChannelValue == 0x00200000)
		{
			ChannelNO = 21;
		}
		else if (ChannelValue == 0x00100000)
		{
			ChannelNO = 20;
		}
		else if (ChannelValue == 0x00080000)
		{
			ChannelNO = 19;
		}
		else if (ChannelValue == 0x00040000)
		{
			ChannelNO = 18;
		}
		else if (ChannelValue == 0x00020000)
		{
			ChannelNO = 17;
		}
		else if (ChannelValue == 0x00010000)
		{
			ChannelNO = 16;
		}
		else if (ChannelValue == 0x00008000)
		{
			ChannelNO = 15;
		}
		else if (ChannelValue == 0x00004000)
		{
			ChannelNO = 14;
		}
		else if (ChannelValue == 0x00002000)
		{
			ChannelNO = 13;
		}
		else if (ChannelValue == 0x00001000)
		{
			ChannelNO = 12;
		}
		else if (ChannelValue == 0x07FFF8000)
		{
			ChannelNO = 11;
		}
		else
		{
			ChannelNO = 0;
		}
		if (ChannelNO > 0)
		{
			ChannelNO -= 11;
		}
		m_combox_channel.SetCurSel(ChannelNO);

		strTemp.Format(_T("%d"), product_register_value[25]);
		GetDlgItem(IDC_EDIT_ZIGBEE_SV)->SetWindowText(strTemp);
		strTemp = _T("");
		for (int Address = 35; Address < 51; Address++)
		{
			CString StrValue;
			StrValue.Format(_T("%02X"), product_register_value[Address]);
			strTemp += StrValue;
		}

		GetDlgItem(IDC_EDIT_ZIGBEE_SK)->SetWindowText(strTemp);
		strTemp.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X-%02X-%02X"), product_register_value[26], product_register_value[27], product_register_value[28], product_register_value[29], product_register_value[30], product_register_value[31], product_register_value[32], product_register_value[33]);
		GetDlgItem(IDC_EDIT_ZIGBEE_MAC)->SetWindowText(strTemp);

		if (product_register_value[51] > 0)
		{
			m_zigbee_tstat_table.put_Cols(3);
			m_zigbee_tstat_table.put_Rows(product_register_value[51] + 1);
			m_zigbee_tstat_table.put_TextMatrix(0, 0, _T("Index"));
			m_zigbee_tstat_table.put_TextMatrix(0, 1, _T("TstatIDs"));
			m_zigbee_tstat_table.put_TextMatrix(0, 2, _T("Signal Strength"));
			for (int ids = 0; ids < product_register_value[51]; ids++)
			{
				strTemp.Format(_T("%d"), ids + 1);
				m_zigbee_tstat_table.put_TextMatrix(ids + 1, 0, strTemp);


				strTemp.Format(_T("%d"), product_register_value[52 + ids]);
				m_zigbee_tstat_table.put_TextMatrix(ids + 1, 1, strTemp);

				char strength = product_register_value[52 + product_register_value[51] + ids];

				strTemp.Format(_T("%d"), strength);

				m_zigbee_tstat_table.put_TextMatrix(ids + 1, 2, strTemp);

			}
		}
		else
		{
			m_zigbee_tstat_table.put_Cols(3);
			m_zigbee_tstat_table.put_Rows(1);
			m_zigbee_tstat_table.put_TextMatrix(0, 0, _T("Index"));
			m_zigbee_tstat_table.put_TextMatrix(0, 1, _T("TstatIDs"));
			m_zigbee_tstat_table.put_TextMatrix(0, 2, _T("Signal Strength"));
		}




	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}



void CZigbeeInformationDlg::OnBnClickedBtnTopological()
{
	int temp_pause = b_pause_refresh_tree;	//resume all the status.
	int temp_g_id = g_tstat_id;
	int temp_value = b_pause_refresh_tree;
	bool temp_pasueread = g_bPauseMultiRead;
	b_pause_refresh_tree = BTNTOPOLOGICAL;
	b_pause_refresh_tree = true;
	g_bPauseMultiRead = true;
	CTstatZigbeeLogic dlg;
	dlg.DoModal();
	b_pause_refresh_tree = temp_pause;
	g_tstat_id = temp_g_id;
	b_pause_refresh_tree = temp_value;
	g_bPauseMultiRead = temp_pasueread;
}


void CZigbeeInformationDlg::OnBnClickedButtonZigbeeReboot()
{
	int ret = write_one(g_tstat_id, 34, 1);
	if (ret < 0)
	{
		AfxMessageBox(_T("Reboot it ,again"));
	}
}
