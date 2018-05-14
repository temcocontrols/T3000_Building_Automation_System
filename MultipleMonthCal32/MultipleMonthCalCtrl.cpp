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

BOOL CMultipleMonthCalCtrl::SetDayState(int count, MONTHDAYSTATE* states)
{
	SYSTEMTIME start, end;
	GetMonthRange(&start, &end, GMR_DAYSTATE);
	SelectDates(ToSystemTimes(count, states, start));

	return CMonthCalCtrl::SetDayState(count, states);
	//return TRUE;
}

BEGIN_MESSAGE_MAP(CMultipleMonthCalCtrl, CMonthCalCtrl)
END_MESSAGE_MAP()


BOOL CMultipleMonthCalCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// initialize common controls
	//VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_DATE_REG));

	CWnd* pWnd = this;
	return pWnd->Create(MULTIPLEMONTHCAL_CLASS, NULL, dwStyle, rect, pParentWnd, nID);
}

/*
 * Utilities for T3000
 */
int     MONTHCAL_CalculateDayOfWeek(SYSTEMTIME *date, BOOL inplace);
int     MONTHCAL_MonthLength(int month, int year);
void    MONTHCAL_GetMonth(SYSTEMTIME *date, INT months);

void     MONTHCAL_GetSel_Month_Day(int * nmonth, int* nday);

void CMultipleMonthCalCtrl::MONTHCAL_GetSelDay_Month(int * nmonth, int* nday)
{
    MONTHCAL_GetSel_Month_Day(nmonth, nday);

}


int     getDifference(const SYSTEMTIME & start, const SYSTEMTIME & end)
{
	return ((int)(end.wYear * 12) + end.wMonth) - ((int)(start.wYear * 12) + start.wMonth);
}

std::vector<SYSTEMTIME> ToSystemTimes(int monthCount, MONTHDAYSTATE * states, const SYSTEMTIME & start)
{
	std::vector<SYSTEMTIME> days;

	SYSTEMTIME systime;
	::GetSystemTime(&systime);
	SYSTEMTIME day = systime;
	day.wYear = start.wYear;
	for (int i = 0; i < monthCount; ++i)
	{
		day = start;
		MONTHCAL_GetMonth(&day, i);
		for (int j = 0; j < 31; ++j)
		{
			day.wDay = j + 1;

			if (states[i] & (1 << j))
			{
				days.push_back(day);
			}
		}
	}

	return days;
}

MONTHDAYSTATE*    ToDayStates(const std::vector<SYSTEMTIME> & days, int & mCount, SYSTEMTIME & start)
{
	if (days.empty())
	{
		return 0;
	}

	SYSTEMTIME systime;
	::GetSystemTime(&systime);

	int minMonth = days[0].wYear * 12 + days[0].wMonth - 1;
	int maxMonth = minMonth;
	for (size_t i = 0; i < days.size(); ++i)
	{
		minMonth = min(minMonth, days[i].wYear * 12 + days[i].wMonth - 1);
		maxMonth = max(maxMonth, days[i].wYear * 12 + days[i].wMonth - 1);
	}
	SYSTEMTIME minDate = systime;
	minDate.wYear = minMonth / 12;
	minDate.wMonth = 1 + ( minMonth % 12 );

	SYSTEMTIME maxDate = systime;
	maxDate.wYear = maxMonth / 12;
	maxDate.wMonth = 1 + (maxMonth % 12);

	int count = getDifference(minDate, maxDate) + 1;

	MONTHDAYSTATE * states = (MONTHDAYSTATE*)malloc(count * sizeof(MONTHDAYSTATE));
	start = minDate;
	mCount = count;

	for (int i = 0; i < count; ++i)
	{
		states[i] = 0;
	}
	for (size_t i = 0; i < days.size(); ++i)
	{
		int diff = getDifference(minDate, days[i]);
		states[diff] ^= 1 << (days[i].wDay - 1);
	}

	return states;
}

std::vector<SYSTEMTIME>   GetAllYearDaysForDayOfWeek(int year, int dayOfWeek)
{
	std::vector<SYSTEMTIME> days;

	SYSTEMTIME systime;
	::GetSystemTime(&systime);
	SYSTEMTIME date = systime;
	date.wYear = year;
	for (int i = 0; i < 12; ++i)
	{
		date.wMonth = 1 + i;
		int length = MONTHCAL_MonthLength(date.wMonth, date.wYear);
		for (int j = 0; j < length; ++j)
		{
			date.wDay = 1 + j;
			MONTHCAL_CalculateDayOfWeek(&date, TRUE);
			if (date.wDayOfWeek == 0)
				days.push_back(date);
		}
	}

	return days;
}
