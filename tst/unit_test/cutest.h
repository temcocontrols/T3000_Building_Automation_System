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

#ifndef UNITTEST_TEST_H
#define UNITTEST_TEST_H

    #pragma warning(disable:4127) // conditional expression is constant
	#pragma warning(disable:4702) // unreachable code
	#pragma warning(disable:4722) // destructor never returns, potential memory leak


#include <vector>
#include <sstream>


class Timer
{
public:
    Timer();
	void Start();
	int GetTimeInMs() const;

private:
    __int64 GetTime() const;

    void* m_threadId;

#if defined(_WIN64)
    unsigned __int64 m_processAffinityMask;
#else
    unsigned long m_processAffinityMask;
#endif

	__int64 m_startTime;
	__int64 m_frequency;
};


namespace TimeHelpers
{
void SleepMs (int ms);
}

namespace raven {
	namespace set {
		namespace UnitTest {

class TestResults;
class TestList;

class TestDetails
{
public:
    char const* const suiteName;
    char const* const testName;
    char const* const filename;
    int const lineNumber;

	TestDetails(char const* testName_, char const* suiteName_, char const* filename_, int lineNumber_)
    : suiteName(suiteName_)
    , testName(testName_)
    , filename(filename_)
    , lineNumber(lineNumber_)
{
}
    TestDetails(const TestDetails& details, int lineNumber_)
    : suiteName(details.suiteName)
    , testName(details.testName)
    , filename(details.filename)
    , lineNumber(lineNumber_)
{
}


    TestDetails(TestDetails const&); // Why is it public? --> http://gcc.gnu.org/bugs.html#cxx_rvalbind
private:
    TestDetails& operator=(TestDetails const&);
};



class TestReporter
{
public:
	virtual ~TestReporter() {}

    virtual void ReportTestStart(TestDetails const& test) = 0;
    virtual void ReportFailure(TestDetails const& test, char const* failure) = 0;
    virtual void ReportTestFinish(TestDetails const& test, float secondsElapsed) = 0;
    virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed) = 0;
};

class TestReporterStdout : public TestReporter
{
private:
    virtual void ReportTestStart(TestDetails const& test);
    virtual void ReportFailure(TestDetails const& test, char const* failure);
    virtual void ReportTestFinish(TestDetails const& test, float secondsElapsed);
    virtual void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed);
};



class Test
{
public:
    Test(char const* testName, char const* suiteName = "DefaultSuite", char const* filename = "", int lineNumber = 0);
    virtual ~Test();
    void Run(TestResults* testResults) ;

    TestDetails const m_details;
    mutable bool m_timeConstraintExempt;

	static std::vector< Test* >& GetTestList();

private:
    virtual void RunImpl(TestResults& testResults_) const;

    Test(Test const&);
    Test& operator =(Test const&);
};


class ListAdder
{
public:
	ListAdder(std::vector< Test* >& list, Test* test)
	{
		list.push_back( test );
	}
};


class MemoryOutStream : public std::ostringstream
{
public:
    MemoryOutStream() {}
    char const* GetText() const
{
    m_text = this->str();
    return m_text.c_str();
}

private:
    MemoryOutStream(MemoryOutStream const&);
    void operator =(MemoryOutStream const&);

    mutable std::string m_text;
};

/**

  The state of the tests

*/
class TestResults
{
public:
    explicit TestResults(TestReporter* reporter = 0);

    void OnTestStart(TestDetails const& test);

    void OnTestFailure(TestDetails const& test, char const* failure);

    void OnTestFinish(TestDetails const& test, float secondsElapsed);

    int GetTotalTestCount() const
{
    return m_totalTestCount;
}
    int GetFailedTestCount() const
{
    return m_failedTestCount;
}
    int GetFailureCount() const
{
    return m_failureCount;
}
	void SetMaxTestTime( int maxTestTimeInMs )
{
	m_MaxTestTimeInMs = maxTestTimeInMs;
}
	int GetMaxTestTime() const
{
	return m_MaxTestTimeInMs;
}
	void SetSuiteName( const char * name )
{
	m_suiteName = (char*)name;
}
	char * GetSuiteName() const
{
	return m_suiteName;
}
	bool IsSuite( const char* name )
{
    if ( ! m_suiteName )
		return 1;			// there is no current suite, so OK

	if( !std::strcmp(name, m_suiteName))
		return 1;

	return 0;
}
	float GetElapsedSeconds() const
{
	return m_OverallTimer.GetTimeInMs() / 1000.0f;
}


private:
    TestReporter* m_testReporter;
    int m_totalTestCount;
    int m_failedTestCount;
    int m_failureCount;
	int m_MaxTestTimeInMs;	// time allowed for individual test
	Timer m_OverallTimer;	// time used by all tests
	char* m_suiteName;		// current test suite name

    bool m_currentTestFailed;

    TestResults(TestResults const&);
    TestResults& operator =(TestResults const&);
};


class AssertException : public std::exception
{
public:
    AssertException(char const* description, char const* filename, int lineNumber)
    : m_lineNumber(lineNumber)
{
    strcpy_s(m_description, 512, description);
    strcpy_s(m_filename, 256, filename);
}
	virtual ~AssertException() throw() {}

    virtual char const* what() const throw()
{
    return m_description;
}

    char const* Filename() const
{
    return m_filename;
}
    int LineNumber() const
{
    return m_lineNumber;
}

private:
    char m_description[512];
    char m_filename[256];
    int m_lineNumber;
};


class TimeConstraint
{
public:
    TimeConstraint(int ms, TestResults& result, TestDetails const& details);
    ~TimeConstraint();

private:
    void operator=(TimeConstraint const&);
	TimeConstraint(TimeConstraint const&);

	Timer m_timer;
    TestResults& m_result;
    TestDetails const& m_details;
	int const m_maxMs;
};

#define UNITTEST_TIME_CONSTRAINT(ms) \
	UnitTest::TimeConstraint unitTest__timeConstraint__(ms, testResults_, UnitTest::TestDetails(m_details, __LINE__))

#define UNITTEST_TIME_CONSTRAINT_EXEMPT() do { m_timeConstraintExempt = true; } while (0)


void ReportAssert(char const* description, char const* filename, int lineNumber);

int RunAllTests(TestReporter& reporter = TestReporterStdout(),
				std::vector< Test* > const& list = Test::GetTestList(),
				char const* suiteName = 0,
				int maxTestTimeInMs = 0);

} } }
namespace raven {
	namespace set {
namespace UnitTestSuite {

    inline char const* GetSuiteName ()
    {
        return "DefaultSuite";
    }

}
	}}

#ifndef UNITTEST_POSIX
	#define UNITTEST_THROW_SIGNALS
#else
	#include "Posix/SignalTranslator.h"
#endif

#ifdef TEST
    #error UnitTest++ redefines TEST
#endif

/*
 I commented out this when moving to the raven::set namespace
 This presumable breaks the suite feature, but since I do no use it .... JB

*/
//#define SUITE(Name)                                                         \
//    namespace Name {                                                        \
//        namespace UnitTestSuite {                                           \
//            inline char const* GetSuiteName () {                            \
//                return #Name ;                                              \
//            }                                                               \
//        }                                                                   \
//    }                                                                       \
//    namespace Name

#define TEST_EX(Name, List)                                                \
    class Test##Name : public raven::set::UnitTest::Test                               \
    {                                                                      \
    public:                                                                \
	Test##Name() : Test(#Name, raven::set::UnitTestSuite::GetSuiteName(), __FILE__, __LINE__) {}  \
    private:                                                               \
        virtual void RunImpl(raven::set::UnitTest::TestResults& testResults_) const;   \
    } test##Name##Instance;                                                \
																		   \
    raven::set::UnitTest::ListAdder adder##Name (List, &test##Name##Instance);         \
																		   \
    void Test##Name::RunImpl(raven::set::UnitTest::TestResults& testResults_) const



#define TEST(Name) TEST_EX(Name, raven::set::UnitTest::Test::GetTestList())


#define TEST_FIXTURE_EX(Fixture, Name, List)                                         \
    class Fixture##Name##Helper : public Fixture									 \
	{																				 \
	public:																			 \
        Fixture##Name##Helper(UnitTest::TestDetails const& details) : m_details(details) {} \
        void RunTest(UnitTest::TestResults& testResults_);                           \
        UnitTest::TestDetails const& m_details;                                      \
    private:                                                                         \
        Fixture##Name##Helper(Fixture##Name##Helper const&);                         \
        Fixture##Name##Helper& operator =(Fixture##Name##Helper const&);             \
    };                                                                               \
																					 \
    class Test##Fixture##Name : public UnitTest::Test                                \
    {                                                                                \
    public:                                                                          \
	    Test##Fixture##Name() : Test(#Name, raven::set::UnitTestSuite::GetSuiteName(), __FILE__, __LINE__) {} \
    private:                                                                         \
        virtual void RunImpl(UnitTest::TestResults& testResults_) const;             \
    } test##Fixture##Name##Instance;                                                 \
																					 \
    UnitTest::ListAdder adder##Fixture##Name (List, &test##Fixture##Name##Instance); \
																					 \
    void Test##Fixture##Name::RunImpl(UnitTest::TestResults& testResults_) const	 \
	{																				 \
		bool ctorOk = false;														 \
		try {																		 \
			Fixture##Name##Helper fixtureHelper(m_details);							 \
			ctorOk = true;															 \
			try {																	 \
				UNITTEST_THROW_SIGNALS;												 \
				fixtureHelper.RunTest(testResults_);								 \
			} catch (UnitTest::AssertException const& e) {							 \
				testResults_.OnTestFailure(UnitTest::TestDetails(m_details.testName, m_details.suiteName, e.Filename(), e.LineNumber()), e.what()); \
			} catch (std::exception const& e) {										 \
				UnitTest::MemoryOutStream stream;									 \
				stream << "Unhandled exception: " << e.what();						 \
				testResults_.OnTestFailure(m_details, stream.GetText());			 \
			} catch (...) {	testResults_.OnTestFailure(m_details, "Unhandled exception: Crash!"); } \
		}																			 \
		catch (...) {																 \
			if (ctorOk)																 \
			{																		 \
	            testResults_.OnTestFailure(UnitTest::TestDetails(m_details, __LINE__),	 \
					"Unhandled exception while destroying fixture " #Fixture);		 \
			}																		 \
			else																	 \
			{																		 \
				testResults_.OnTestFailure(UnitTest::TestDetails(m_details, __LINE__),   \
					"Unhandled exception while constructing fixture " #Fixture);         \
			}																		 \
		}																			 \
    }                                                                                \
    void Fixture##Name##Helper::RunTest(UnitTest::TestResults& testResults_)

#define TEST_FIXTURE(Fixture,Name) TEST_FIXTURE_EX(Fixture, Name, UnitTest::Test::GetTestList())


namespace raven { namespace set {
namespace UnitTest {


template< typename Value >
bool Check(Value const value)
{
    return !!value; // doing double negative to avoid silly VS warnings
}


template< typename Expected, typename Actual >
void CheckEqual(TestResults& results, Expected const expected, Actual const actual, TestDetails const& details)
{
    if (!(expected == actual))
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected " << expected << " but was " << actual;

        results.OnTestFailure(details, stream.GetText());
    }
}

void CheckEqual(TestResults& results, char const* expected, char const* actual, TestDetails const& details);

void CheckEqual(TestResults& results, char* expected, char* actual, TestDetails const& details);

void CheckEqual(TestResults& results, char* expected, char const* actual, TestDetails const& details);

void CheckEqual(TestResults& results, char const* expected, char* actual, TestDetails const& details);

template< typename Expected, typename Actual, typename Tolerance >
bool AreClose(Expected const expected, Actual const actual, Tolerance const tolerance)
{
    return (actual >= (expected - tolerance)) && (actual <= (expected + tolerance));
}

template< typename Expected, typename Actual, typename Tolerance >
void CheckClose(TestResults& results, Expected const expected, Actual const actual, Tolerance const tolerance,
                TestDetails const& details)
{
    if (!AreClose(expected, actual, tolerance))
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected " << expected << " +/- " << tolerance << " but was " << actual;

        results.OnTestFailure(details, stream.GetText());
    }
}


template< typename Expected, typename Actual >
void CheckArrayEqual(TestResults& results, Expected const expected, Actual const actual,
                int const count, TestDetails const& details)
{
    bool equal = true;
    for (int i = 0; i < count; ++i)
        equal &= (expected[i] == actual[i]);

    if (!equal)
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected [ ";
        for (int i = 0; i < count; ++i)
            stream << expected[i] << " ";
        stream << "] but was [ ";
        for (int i = 0; i < count; ++i)
            stream << actual[i] << " ";
        stream << "]";

        results.OnTestFailure(details, stream.GetText());
    }
}

template< typename Expected, typename Actual, typename Tolerance >
bool ArrayAreClose(Expected const expected, Actual const actual, int const count, Tolerance const tolerance)
{
    bool equal = true;
    for (int i = 0; i < count; ++i)
        equal &= AreClose(expected[i], actual[i], tolerance);
    return equal;
}

template< typename Expected, typename Actual, typename Tolerance >
void CheckArrayClose(TestResults& results, Expected const expected, Actual const actual,
                   int const count, Tolerance const tolerance, TestDetails const& details)
{
    bool equal = ArrayAreClose(expected, actual, count, tolerance);

    if (!equal)
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected [ ";
        for (int i = 0; i < count; ++i)
            stream << expected[i] << " ";
        stream << "] +/- " << tolerance << " but was [ ";
        for (int i = 0; i < count; ++i)
            stream << actual[i] << " ";
        stream << "]";

        results.OnTestFailure(details, stream.GetText());
    }
}

template< typename Expected, typename Actual, typename Tolerance >
void CheckArray2DClose(TestResults& results, Expected const expected, Actual const actual,
                   int const rows, int const columns, Tolerance const tolerance, TestDetails const& details)
{
    bool equal = true;
    for (int i = 0; i < rows; ++i)
        equal &= ArrayAreClose(expected[i], actual[i], columns, tolerance);

    if (!equal)
    {
        UnitTest::MemoryOutStream stream;
        stream << "Expected [ ";
        for (int i = 0; i < rows; ++i)
        {
            stream << "[ ";
            for (int j = 0; j < columns; ++j)
                stream << expected[i][j] << " ";
            stream << "] ";
        }
        stream << "] +/- " << tolerance << " but was [ ";
        for (int i = 0; i < rows; ++i)
        {
            stream << "[ ";
            for (int j = 0; j < columns; ++j)
                stream << actual[i][j] << " ";
            stream << "] ";
        }
        stream << "]";

        results.OnTestFailure(details, stream.GetText());
    }
}

} } }

#ifdef CHECK
    #error UnitTest++ redefines CHECK
#endif

namespace raven { namespace set {
namespace UnitTest {

#define CHECK(value) \
    do \
    { \
        try { \
		if (!raven::set::UnitTest::Check(value)) \
                testResults_.OnTestFailure( raven::set::UnitTest::TestDetails(m_details, __LINE__), #value); \
        } \
        catch (...) { \
            testResults_.OnTestFailure(raven::set::UnitTest::TestDetails(m_details, __LINE__), \
                    "Unhandled exception in CHECK(" #value ")"); \
        } \
    } while (0)

#define CHECK_EQUAL(expected, actual) \
    do \
    { \
        try { \
            raven::set::UnitTest::CheckEqual(testResults_, expected, actual, raven::set::UnitTest::TestDetails(m_details, __LINE__)); \
        } \
        catch (...) { \
            testResults_.OnTestFailure(raven::set::UnitTest::TestDetails(m_details, __LINE__), \
                    "Unhandled exception in CHECK_EQUAL(" #expected ", " #actual ")"); \
        } \
    } while (0)

#define CHECK_CLOSE(expected, actual, tolerance) \
    do \
    { \
        try { \
            raven::set::UnitTest::CheckClose(testResults_, expected, actual, tolerance, raven::set::UnitTest::TestDetails(m_details, __LINE__)); \
        } \
        catch (...) { \
            testResults_.OnTestFailure(raven::set::UnitTest::TestDetails(m_details, __LINE__), \
                    "Unhandled exception in CHECK_CLOSE(" #expected ", " #actual ")"); \
        } \
    } while (0)

#define CHECK_ARRAY_EQUAL(expected, actual, count) \
    do \
    { \
        try { \
            UnitTest::CheckArrayEqual(testResults_, expected, actual, count, UnitTest::TestDetails(m_details, __LINE__)); \
        } \
        catch (...) { \
            testResults_.OnTestFailure(UnitTest::TestDetails(m_details, __LINE__), \
                    "Unhandled exception in CHECK_ARRAY_EQUAL(" #expected ", " #actual ")"); \
        } \
    } while (0)

#define CHECK_ARRAY_CLOSE(expected, actual, count, tolerance) \
    do \
    { \
        try { \
            UnitTest::CheckArrayClose(testResults_, expected, actual, count, tolerance, UnitTest::TestDetails(m_details, __LINE__)); \
        } \
        catch (...) { \
            testResults_.OnTestFailure(UnitTest::TestDetails(m_details, __LINE__), \
                    "Unhandled exception in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")"); \
        } \
    } while (0)

#define CHECK_ARRAY2D_CLOSE(expected, actual, rows, columns, tolerance) \
    do \
    { \
        try { \
            UnitTest::CheckArray2DClose(testResults_, expected, actual, rows, columns, tolerance, UnitTest::TestDetails(m_details, __LINE__)); \
        } \
        catch (...) { \
            testResults_.OnTestFailure(UnitTest::TestDetails(m_details, __LINE__), \
                    "Unhandled exception in CHECK_ARRAY_CLOSE(" #expected ", " #actual ")"); \
        } \
    } while (0)


#define CHECK_THROW(expression, ExpectedExceptionType) \
    do \
    { \
        bool caught_ = false; \
        try { expression; } \
        catch (ExpectedExceptionType const&) { caught_ = true; } \
        catch (...) {} \
        if (!caught_) \
            testResults_.OnTestFailure(UnitTest::TestDetails(m_details, __LINE__), "Expected exception: \"" #ExpectedExceptionType "\" not thrown"); \
    } while(0)

#define CHECK_ASSERT(expression) \
    CHECK_THROW(expression, UnitTest::AssertException);

}



}}

#endif