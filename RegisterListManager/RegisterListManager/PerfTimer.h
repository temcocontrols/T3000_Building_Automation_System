#pragma once

#ifdef _DEBUG
#define TraceFnTime(szFunc) DebugTimer aTimer(szFunc)
#else
#define TraceFnTime(szFunc)
#endif

//////////////////////////////////////////////////////////////////////////
// using the difference of the CPU tick count to divide the CPU frequency per second
// time_span = (difference of CPU tick count) / (CPU frequency)  : (seconds)
//////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CPerfTimer
{
private:
	LARGE_INTEGER m_liFreq;
	LARGE_INTEGER m_liStart;
	LARGE_INTEGER m_liEnd;
public:
	CPerfTimer(void)//: //m_liFreq.QuadPart(0)//, m_liStart.QuadPart(0), m_liEnd.QuadPart(0)
	{
		QueryPerformanceFrequency(&m_liFreq);
	}
	~CPerfTimer(void)
	{};
public:
	inline void Start()
	{
		QueryPerformanceCounter(&m_liStart);
	}

	inline void Stop()
	{
		QueryPerformanceCounter(&m_liEnd);
	};

	inline double GetTimeSpan() // time unit is second
	{
		return (double)(m_liEnd.QuadPart - m_liStart.QuadPart)/(double)m_liFreq.QuadPart;
	}
};

class AFX_EXT_CLASS DebugTimer
{
public:
	explicit DebugTimer(LPCTSTR lpszFuncName)
		:m_szFunction(lpszFuncName)
	{ m_thePerfTimer.Start(); }
	~DebugTimer(){ m_thePerfTimer.Stop(); TRACE2("%s: %.12f\r\n", m_szFunction, m_thePerfTimer.GetTimeSpan());}
private:
	CPerfTimer m_thePerfTimer;
	LPCTSTR m_szFunction;
};

