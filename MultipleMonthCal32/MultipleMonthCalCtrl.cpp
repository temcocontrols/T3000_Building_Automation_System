#include "multiplemonthcal.h"
#include "MultipleMonthCalCtrl.h"

IMPLEMENT_DYNAMIC(CMultipleMonthCalCtrl, CMonthCalCtrl)

CMultipleMonthCalCtrl::CMultipleMonthCalCtrl()
{
}

CMultipleMonthCalCtrl::~CMultipleMonthCalCtrl()
{
}

void   CMultipleMonthCalCtrl::RegisterControl()
{
	MONTHCAL_Register();
}

void CMultipleMonthCalCtrl::UnregisterControl()
{
	MONTHCAL_Unregister();
}

void   CMultipleMonthCalCtrl::SetOriginalColors()
{
	//SetWindowTheme(m_hWnd, L" ", L" ");
	COLORREF black = RGB(0, 0, 0);
	COLORREF gray = RGB(211, 211, 211);
	COLORREF white = RGB(255, 255, 255);
	COLORREF monthTitleColor = RGB(219, 238, 244);
	COLORREF monthTitleTextColor = RGB(87, 108, 113);
	COLORREF selectBgColor = RGB(152, 194, 206);
	COLORREF selectTextColor = RGB(10, 65, 122);

	SetColor(MCSC_BACKGROUND, white);
	SetColor(MCSC_TEXT, black);
	SetColor(MCSC_TITLEBK, monthTitleColor);
	SetColor(MCSC_TITLETEXT, monthTitleTextColor);
	SetColor(MCSC_MONTHBK, white);
	SetColor(MCSC_SELECTEDTEXT, selectTextColor);
	SetColor(MCSC_SELECTEDBK, selectBgColor);
	SetColor(MCSC_ABBREVIATIONSTEXT, black);
	SetColor(MCSC_ABBREVIATIONSBK, white);
	SetColor(MCSC_ABBREVIATIONSLINE, gray);

	//Hide trailing dates
	SetColor(MCSC_TRAILINGTEXT, white);
}

void   CMultipleMonthCalCtrl::EnableMultiselect(int maxSelectCount)
{
	SetMaxSelCount(maxSelectCount);
}

void   CMultipleMonthCalCtrl::DisableMultiselect()
{
	EnableMultiselect(1);
}

std::vector<SYSTEMTIME> 
        CMultipleMonthCalCtrl::GetSelection() const
{
    ASSERT(m_hWnd != NULL);
	SELECTION_INFO info;
	BOOL ret = (BOOL)::SendMessage(m_hWnd, MCM_GETCURSEL, 0, (LPARAM)&info);
	std::vector<SYSTEMTIME> dates;
	if (ret != FALSE)
	{
		LPSELECTION_ITEM item = info.first;
		while (item)
		{
			dates.push_back(item->date);
			item = item->next;
		}
	}

	return dates;
}

void    CMultipleMonthCalCtrl::SelectDate(const SYSTEMTIME & date)
{
    ::SendMessage(m_hWnd, MCM_SETCURSEL, 0, (LPARAM)&date);
}

void    CMultipleMonthCalCtrl::SelectDates(const std::vector<SYSTEMTIME>& dates)
{
	for (size_t i = 0; i < dates.size(); ++i)
	{
		SelectDate(dates[i]);
	}
}

void    CMultipleMonthCalCtrl::UnselectAll()
{
	::SendMessage(m_hWnd, MCM_SETCURSEL, 0, 0);
}

BEGIN_MESSAGE_MAP(CMultipleMonthCalCtrl, CMonthCalCtrl)
END_MESSAGE_MAP()
