// BuildingConfigEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T3000.h"
#include "BuildingConfigEditDlg.h"
#include "afxdialogex.h"


// CBuildingConfigEditDlg dialog

IMPLEMENT_DYNAMIC(CBuildingConfigEditDlg, CDialogEx)

CBuildingConfigEditDlg::CBuildingConfigEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBuildingConfigEditDlg::IDD, pParent)
{

	m_strBuildingName = _T("");
	m_strCity = _T("");
	m_strCountryName = _T("");
	m_strElevation = _T("");
	m_strLatitude = _T("");
	m_strlongitude = _T("");
	m_strstate = _T("");
	m_strstreet = _T("");
	m_strZip = _T("");
}

CBuildingConfigEditDlg::~CBuildingConfigEditDlg()
{
}

void CBuildingConfigEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BUILDINGNAME, m_strBuildingName);
	DDX_Text(pDX, IDC_EDIT_CITY, m_strCity);
	DDX_Text(pDX, IDC_EDIT_COUNTRYNAME, m_strCountryName);
	//  DDX_Control(pDX, IDC_EDIT_ELE, m_strElevation);
	DDX_Text(pDX, IDC_EDIT_ELE, m_strElevation);
	DDX_Text(pDX, IDC_EDIT_LAT, m_strLatitude);
	DDX_Text(pDX, IDC_EDIT_LON, m_strlongitude);
	DDX_Text(pDX, IDC_EDIT_STATE, m_strstate);
	DDX_Text(pDX, IDC_EDIT_STREET1, m_strstreet);
	DDX_Text(pDX, IDC_EDIT_ZIP, m_strZip);
}


BEGIN_MESSAGE_MAP(CBuildingConfigEditDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBuildingConfigEditDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CBuildingConfigEditDlg message handlers


void CBuildingConfigEditDlg::OnBnClickedOk()
{
	 
	 UpdateData(TRUE);

	 m_currentBuilding.MainBuildingName = m_strBuildingName;
	 m_currentBuilding.city = m_strCity;
	 m_currentBuilding.country = m_strCountryName;
	 m_currentBuilding.Elevation = m_strElevation;
	 m_currentBuilding.Latitude = m_strLatitude;
	 m_currentBuilding.Longitude = m_strlongitude;
	 m_currentBuilding.state = m_strstate;
	 m_currentBuilding.street = m_strstreet;
	 m_currentBuilding.Zip = _wtoi(m_strZip);

	 CDialog::OnOK();
}

void CBuildingConfigEditDlg::BoundDataUpdate()
{
	m_strBuildingName = m_currentBuilding.MainBuildingName;
	m_strCity = m_currentBuilding.city;
	m_strCountryName = m_currentBuilding.country;	 
	m_strElevation = m_currentBuilding.Elevation;
	m_strLatitude = m_currentBuilding.Latitude;
	m_strlongitude = m_currentBuilding.Longitude;
	m_strstate = m_currentBuilding.state;
	m_strstreet = m_currentBuilding.street;
	m_strZip.Format(_T("%d"),m_currentBuilding.Zip);

	UpdateData(FALSE);
}
BOOL CBuildingConfigEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	BoundDataUpdate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
