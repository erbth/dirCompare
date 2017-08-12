#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include "ignoring_testing.h"

using namespace CPPUNIT_NS;

class IgnoringTest : public TestFixture
{
	CPPUNIT_TEST_SUITE(IgnoringTest);

	CPPUNIT_TEST(noWildcardsEqual);
	CPPUNIT_TEST(noWildcardsPatternLonger);
	CPPUNIT_TEST(noWildcardsTextLonger);

	CPPUNIT_TEST(oneWildcardBeginMatch);
	CPPUNIT_TEST(oneWildcardBeginMismatch);
	CPPUNIT_TEST(oneWildcardEndMatch);
	CPPUNIT_TEST(oneWildcardEndMismatch);
	CPPUNIT_TEST(oneWildcardMiddleMatch);
	CPPUNIT_TEST(oneWildcardMiddleMismatchBefore);
	CPPUNIT_TEST(oneWildcardMiddleMismatchAfter);

	CPPUNIT_TEST_SUITE_END();

public:
	void noWildcardsEqual()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "test string ö")
			== true);
	}

	void noWildcardsPatternLonger()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "test string öa")
			== false);
	}

	void noWildcardsTextLonger()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string öa", "test string ö")
			== false);
	}

	void oneWildcardBeginMatch()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "*g ö")
			== true);
	}

	void oneWildcardBeginMismatch()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "*g_ö")
			== false);
	}

	void oneWildcardEndMatch()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "test*")
			== true);
	}

	void oneWildcardEndMismatch()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "tesd*")
			== false);
	}

	void oneWildcardMiddleMatch()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "tes*g ö")
			== true);
	}

	void oneWildcardMiddleMismatchBefore()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "tesd*g ö")
			== false);
	}

	void oneWildcardMiddleMismatchAfter()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "tes*g ä")
			== false);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(IgnoringTest);

int main(int argc, char** argv)
{
	TestResult testResult;
	
	TestResultCollector collectedResults;
	testResult.addListener(&collectedResults);

	BriefTestProgressListener progress;
	testResult.addListener(&progress);

	TestRunner testRunner;
	testRunner.addTest(TestFactoryRegistry::getRegistry().makeTest());
	testRunner.run(testResult);

	CompilerOutputter compilerOutputter(&collectedResults, std::cerr);
	compilerOutputter.write();

	return collectedResults.wasSuccessful() ? 0 : 1;
}
