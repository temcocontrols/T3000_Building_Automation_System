// AQGraphic.cpp : implementation file
//




 

#include "stdafx.h"
#include "T3000.h"
#include "../AirQuality/AQGraphic.h"
#include "MainFrm.h"




#include "ChartCtrl/ChartLineSerie.h"
#include "ChartCtrl/ChartPointsSerie.h"
#include "ChartCtrl/ChartSurfaceSerie.h"
#include "ChartCtrl/ChartGrid.h"
#include "ChartCtrl/ChartBarSerie.h"
#include "ChartCtrl/ChartLabel.h"
#include "ChartCtrl/ChartAxisLabel.h"
#include "ChartCtrl/ChartStandardAxis.h"
#include "ChartCtrl/ChartDateTimeAxis.h"
#include "ChartCtrl/ChartCrossHairCursor.h"
#include "ChartCtrl/ChartDragLineCursor.h"


 
// CAQGraphic dialog

IMPLEMENT_DYNAMIC(CAQGraphic, CDialog)

CAQGraphic::CAQGraphic(CWnd* pParent /*=NULL*/)
	: CDialog(CAQGraphic::IDD, pParent)
{

}

CAQGraphic::~CAQGraphic()
{
}

void CAQGraphic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAQGraphic, CDialog)
END_MESSAGE_MAP()



BOOL CAQGraphic::OnInitDialog(){


return TRUE;
}
// CAQGraphic message handlers
