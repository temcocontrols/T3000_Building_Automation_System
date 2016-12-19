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

protected:

	DECLARE_MESSAGE_MAP()
public:

};


