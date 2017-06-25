//#include "stdafx.h"
//#include "CppUnitTest.h"
//#include "CppUnitTestAssert.h"
//using namespace Microsoft::VisualStudio::CppUnitTestFramework;
//
//namespace T3000Test
//{		
//	TEST_CLASS(UnitTest1)
//	{
//	public:
//		
//		TEST_METHOD(TestMethod1)
//		{
//			Assert::AreEqual("T3000", "T3000", true);
//		}
//
//	};
//}


////////////////////////////////////////////////////////////  
/* USAGE EXAMPLE
// The following is an example of VSCppUnit usage.
// It includes examples of attribute metadata, fixtures,
// unit tests with assertions, and custom logging.
*/
#include "stdafx.h"
#include <CppUnitTest.h>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

BEGIN_TEST_MODULE_ATTRIBUTE()
TEST_MODULE_ATTRIBUTE(L"Date", L"2010/6/12")
END_TEST_MODULE_ATTRIBUTE()

TEST_MODULE_INITIALIZE(ModuleInitialize)
{
Logger::WriteMessage("In Module Initialize");
}

TEST_MODULE_CLEANUP(ModuleCleanup)
{
Logger::WriteMessage("In Module Cleanup");
}

TEST_CLASS(Class1)
{

public:

Class1()
{
Logger::WriteMessage("In Class1");
}

~Class1()
{
Logger::WriteMessage("In ~Class1");
}

TEST_CLASS_INITIALIZE(ClassInitialize)
{
Logger::WriteMessage("In Class Initialize");
}

TEST_CLASS_CLEANUP(ClassCleanup)
{
Logger::WriteMessage("In Class Cleanup");
}

BEGIN_TEST_METHOD_ATTRIBUTE(Method1)
TEST_OWNER(L"OwnerName")
TEST_PRIORITY(1)
END_TEST_METHOD_ATTRIBUTE()

TEST_METHOD(Method1)
{
Logger::WriteMessage("In Method1");
Assert::AreEqual(0, 0);
}

TEST_METHOD(Method2)
{
Assert::Fail(L"Fail");
}
};