#pragma once
#include <SDKDDKVer.h>
#include <afxdtctl.h>
#include <vector>

class CMultipleMonthCalCtrl : public CMonthCalCtrl
{
	DECLARE_DYNAMIC(CMultipleMonthCalCtrl)

public:
	CMultipleMonthCalCtrl();
	virtual ~CMultipleMonthCalCtrl();

	static void   RegisterControl();
	static void   UnregisterControl();

	void    SetOriginalColors();

	void    EnableMultiselect(int maxSelectCount = 0);
	void    DisableMultiselect();

	std::vector<SYSTEMTIME>	
	        GetSelection() const;

	/* This method set last date as first month in calendar.
	 * Use SetRange() after for select need range.
	 */
	void	SelectDate(const SYSTEMTIME & date);
	void	SelectDates(const std::vector<SYSTEMTIME> & dates);
	void	UnselectAll();
    void    MONTHCAL_GetSelDay_Month(int * nmonth, int* nday);
	BOOL	SetDayState(int count, MONTHDAYSTATE * states);

protected:

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
};


/*
* Utilities for T3000
*/

/*
 * ToSystemTimes
 * inputs:
 * int monthCount - returned value from CMonthCalCtrl::GetMonthRange(,,GMR_DAYSTATE)
 * MONTHDAYSTATE * states - array for states. Count of element equals monthCount
 * const SYSTEMTIME & start - first output value from CMonthCalCtrl::GetMonthRange(THIS_VALUE,,GMR_DAYSTATE)
 * output:
 * std::vector<SYSTEMTIME> - vector for CMultipleMonthCalCtrl::SelectDates();
 */
std::vector<SYSTEMTIME> ToSystemTimes(int monthCount, MONTHDAYSTATE * states, const SYSTEMTIME & start);

/*
* ToDayStates
* inputs:
* const std::vector<SYSTEMTIME> & days - vector contains needed SYSTEMTIME;
* output:
* int & mCount - count for CMonthCalCtrl::SetDayState();
* SYSTEMTIME & start - first element for CMonthCalCtrl::SetRange();
* MONTHDAYSTATE* - array for CMonthCalCtrl::SetDayState();
*/
MONTHDAYSTATE*    ToDayStates(const std::vector<SYSTEMTIME> & days, int & mCount, SYSTEMTIME & start);

/*
* GetAllYearDaysForDayOfWeek
* inputs:
* int year - needed year
* int dayOfWeek - needed day of week. Default - sunday. 0 - sunday, 6 - monday
* output:
* std::vector<SYSTEMTIME> - vector for CMultipleMonthCalCtrl::SelectDates();
*/
std::vector<SYSTEMTIME>   GetAllYearDaysForDayOfWeek(int year, int dayOfWeek = 0);