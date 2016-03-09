/*

Copyright (c) 2010 James Bremner, Raven's Point Consulting, ravenspoint@yahoo.com

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

This software is a modified version of UnitTest++ v1.3.  For a description
of the modicications, see http://ravenspoint.com/blog/index.php?entry=entry100612-063557
In accordance with the UnitTest++ licence, here is a copy of the copyright and permission
notice:

Copyright (c) 2006 Noel Llopis and Charles Nicholson

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/


#include "stdafx.h"
#include "cutest.h"

#include <windows.h>

#include <functional>
#include <algorithm>

#ifdef UNITTEST_POSIX
    #include "Posix/SignalTranslator.h"
#endif
namespace raven { namespace set {
namespace UnitTest {

std::vector< Test* >& Test::GetTestList()
{
	static std::vector< Test* > s_list;
    return s_list;
}

Test::Test(char const* testName, char const* suiteName, char const* filename, int const lineNumber)
    : m_details(testName, suiteName, filename, lineNumber)
    , m_timeConstraintExempt(false)
{
}

Test::~Test()
{
}

void Test::Run(TestResults* testResults)
{
	// check that test belongs to current suite
	if( ! testResults->IsSuite( m_details.suiteName ) )
		return;

	int testTimeInMs = 0;
	try
	{
		Timer testTimer;
		testTimer.Start();

		testResults->OnTestStart(m_details);

#ifdef UNITTEST_POSIX
		UNITTEST_THROW_SIGNALS
#endif
			RunImpl(*testResults);

		testTimeInMs = testTimer.GetTimeInMs();
		if (testResults->GetMaxTestTime() > 0 &&
			testTimeInMs > testResults->GetMaxTestTime() &&
			!m_timeConstraintExempt)
		{
			MemoryOutStream stream;
			stream << "Global time constraint failed. Expected under " << testResults->GetMaxTestTime() <<
				"ms but took " << testTimeInMs << "ms.";
			testResults->OnTestFailure(m_details, stream.GetText());
		}
	}
	catch (AssertException const& e)
	{
		testResults->OnTestFailure( TestDetails(m_details.testName, m_details.suiteName, e.Filename(), e.LineNumber()), e.what());
	}
	catch (std::exception const& e)
	{
		MemoryOutStream stream;
		stream << "Unhandled exception: " << e.what();
		testResults->OnTestFailure(m_details, stream.GetText());
	}
	catch (...)
	{
		testResults->OnTestFailure(m_details, "Unhandled exception: Crash!");
	}

	testResults->OnTestFinish(m_details, testTimeInMs/1000.0f);

}


void Test::RunImpl(TestResults&) const
{
}

/**

  Run All Tests in a list

  @param[in] reporter to be used to display results
  @param[in] list of tests to be run
  @param[in] SuiteName
  @param[in] maxTestTimeInMs maximum time for individual test

  Called without any parameters, defaul to run all tests in any suite
  without a time constraint and use the standard report.

*/

int RunAllTests(TestReporter& reporter,
				std::vector< Test* > const& list,
				char const* suiteName,
				int const maxTestTimeInMs )
{
    TestResults result(&reporter);
	result.SetMaxTestTime( maxTestTimeInMs );
	result.SetSuiteName( suiteName );

	for_each(
		list.begin(),
		list.end(),
		std::bind2nd( std::mem_fun1( &Test::Run ), &result ) );

    reporter.ReportSummary(result.GetTotalTestCount(),
		result.GetFailedTestCount(),
		result.GetFailureCount(),
		result.GetElapsedSeconds());

    return result.GetFailureCount();
}


TestResults::TestResults(TestReporter* reporter)
    : m_testReporter(reporter)
    , m_totalTestCount(0)
    , m_failedTestCount(0)
    , m_failureCount(0)
    , m_currentTestFailed(false)
{
	m_OverallTimer.Start();
}

void TestResults::OnTestStart(TestDetails const& test)
{
    ++m_totalTestCount;
    m_currentTestFailed = false;
    if (m_testReporter)
        m_testReporter->ReportTestStart(test);
}
   void TestResults::OnTestFailure(TestDetails const& test, char const* failure)
{
    ++m_failureCount;
    if (!m_currentTestFailed)
    {
        ++m_failedTestCount;
        m_currentTestFailed = true;
    }

    if (m_testReporter)
        m_testReporter->ReportFailure(test, failure);
}
    void TestResults::OnTestFinish(TestDetails const& test, float secondsElapsed)
{
    if (m_testReporter)
        m_testReporter->ReportTestFinish(test, secondsElapsed);
}

void TestReporterStdout::ReportFailure(TestDetails const& details, char const* failure)
{
#ifdef __APPLE__
    char const* const errorFormat = "%s:%d: error: Failure in %s: %s\n";
#else
    char const* const errorFormat = "%s(%d): error: Failure in %s: %s\n";
#endif
    std::printf(errorFormat, details.filename, details.lineNumber, details.testName, failure);
}

void TestReporterStdout::ReportTestStart(TestDetails const& /*test*/)
{
}

void TestReporterStdout::ReportTestFinish(TestDetails const& /*test*/, float)
{
}

void TestReporterStdout::ReportSummary(int const totalTestCount, int const failedTestCount,
                                       int const failureCount, float secondsElapsed)
{
    if (failureCount > 0)
        std::printf("FAILURE: %d out of %d tests failed (%d failures).\n", failedTestCount, totalTestCount, failureCount);
    else
        std::printf("Success: %d tests passed.\n", totalTestCount);
    std::printf("Test time: %.2f seconds.\n", secondsElapsed);
}

} } }

Timer::Timer()
    : m_startTime(0)
{
    m_threadId = ::GetCurrentThread();
    DWORD_PTR systemMask;
    ::GetProcessAffinityMask(GetCurrentProcess(), &m_processAffinityMask, &systemMask);

    ::SetThreadAffinityMask(m_threadId, 1);
	::QueryPerformanceFrequency(reinterpret_cast< LARGE_INTEGER* >(&m_frequency));
    ::SetThreadAffinityMask(m_threadId, m_processAffinityMask);
}

void Timer::Start()
{
    m_startTime = GetTime();
}

int Timer::GetTimeInMs() const
{
    __int64 const elapsedTime = GetTime() - m_startTime;
	double const seconds = double(elapsedTime) / double(m_frequency);
	return int(seconds * 1000.0f);
}

__int64 Timer::GetTime() const
{
    LARGE_INTEGER curTime;
    ::SetThreadAffinityMask(m_threadId, 1);
	::QueryPerformanceCounter(&curTime);
    ::SetThreadAffinityMask(m_threadId, m_processAffinityMask);
    return curTime.QuadPart;
}



void TimeHelpers::SleepMs(int const ms)
{
	::Sleep(ms);
}



