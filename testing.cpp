#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include "ignoring_testing.h"

#include <memory>
#include <string>
#include "ItemFactory.h"
#include "LinuxItemFactory.h"
#include "File.h"
#include "Directory.h"
#include "InvalidItem.h"
#include "LinuxFile.h"
#include "LinuxDirectory.h"

using namespace CPPUNIT_NS;

class LinuxItemFactoryTest : public TestFixture
{
	CPPUNIT_TEST_SUITE(LinuxItemFactoryTest);

	CPPUNIT_TEST(nonexisting_file);
	CPPUNIT_TEST(existing_file);
	CPPUNIT_TEST(nonexisting_directory);
	CPPUNIT_TEST(existing_directory);

	CPPUNIT_TEST_SUITE_END();

private:
	shared_ptr<ItemFactory> factory;
	shared_ptr<SystemParameters> sp;

public:
	void setUp() override
	{
		sp = make_shared<SystemParameters>();
		factory = make_shared<LinuxItemFactory>(sp);
	}

	void nonexisting_file()
	{
		shared_ptr<File> f = factory->createFile("LinuxItemFactory.cpp1");

		CPPUNIT_ASSERT(dynamic_pointer_cast<InvalidItem>(f) != nullptr);
		CPPUNIT_ASSERT(dynamic_pointer_cast<LinuxFile>(f) == nullptr);

		auto invalid = dynamic_pointer_cast<InvalidItem>(f);

		CPPUNIT_ASSERT(invalid->getErrorDescription() ==
			"Unable to open file \"LinuxItemFactory.cpp1\": "
			"No such file or directory");
	}

	void existing_file()
	{
		shared_ptr<File> f = factory->createFile("LinuxItemFactory.cpp");

		CPPUNIT_ASSERT(dynamic_pointer_cast<InvalidItem>(f) == nullptr);
		CPPUNIT_ASSERT(dynamic_pointer_cast<LinuxFile>(f) != nullptr);
	}

	void nonexisting_directory()
	{
		shared_ptr<Directory> d = factory->createDirectory("docsd");

		CPPUNIT_ASSERT(dynamic_pointer_cast<InvalidItem>(d) != nullptr);
		CPPUNIT_ASSERT(dynamic_pointer_cast<LinuxDirectory>(d) == nullptr);

		auto invalid = dynamic_pointer_cast<InvalidItem>(d);

		CPPUNIT_ASSERT(invalid->getErrorDescription() ==
			"Unable to open directory \"docsd\": "
			"No such file or directory");
	}

	void existing_directory()
	{
		shared_ptr<Directory> d = factory->createDirectory("doc");

		CPPUNIT_ASSERT(dynamic_pointer_cast<InvalidItem>(d) == nullptr);
		CPPUNIT_ASSERT(dynamic_pointer_cast<LinuxDirectory>(d) != nullptr);
	}
};

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
	CPPUNIT_TEST(oneWildcardPerformance);

	CPPUNIT_TEST(wildcardsPatternLonger);

	CPPUNIT_TEST(twoWildcardsMatch);
	CPPUNIT_TEST(twoWildcardsMismatch);

	CPPUNIT_TEST(fourWildcardsMatch);
	CPPUNIT_TEST(fourWildcardsMismatch);
	CPPUNIT_TEST(fourWildcardsPerformance);

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

	void oneWildcardPerformance()
	{
		volatile bool result;

		clock_t begin = clock();

		for (int i = 0; i < 1000000; i++)
		{
			result = ignoring_match_test("test string ö", "tes*g ö");
		}

		clock_t end = clock();

		double elapsed_seconds = double(end - begin) / CLOCKS_PER_SEC;

		if (elapsed_seconds > 1.0)
		{
			CPPUNIT_FAIL("performance test took too long");
		}

		if (elapsed_seconds < 1e-3)
		{
			CPPUNIT_FAIL("performance test too fast, propably something went wrong");
		}
	}

	void wildcardsPatternLonger()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "te_st s*tri*ng ö")
			== false);
	}

	void twoWildcardsMatch()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "tes*string*")
			== true);
	}

	void twoWildcardsMismatch()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "tes*strink*")
			== false);
	}

	void fourWildcardsMatch()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "te* **g*ö")
			== true);
	}

	void fourWildcardsMismatch()
	{
		CPPUNIT_ASSERT(
			ignoring_match_test("test string ö", "te* *s*g_*ö")
			== false);
	}

	void fourWildcardsPerformance()
	{
		volatile bool result;

		char textSingle[] = "test string ö";
		char text[(100 * (sizeof(textSingle) / sizeof(char) - 1) + 4) * sizeof(char)];

		char* p = text;

		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < sizeof(textSingle) / sizeof(char) - 1; j++)
			{
				*p++ = textSingle[j];
			}
		}

		*p++ = 'a';
		*p++ = ' ';
		*p++ = 'o';
		*p++ = '\0';

		clock_t begin = clock();

		for (int i = 0; i < 100000; i++)
		{
			result = ignoring_match_test(text, "te* **a*o");
		}

		clock_t end = clock();

		double elapsed_seconds = double(end - begin) / CLOCKS_PER_SEC;

		if (elapsed_seconds > 5.0)
		{
			CPPUNIT_FAIL("performance test took too long");
		}

		if (elapsed_seconds < 5e-3)
		{
			CPPUNIT_FAIL("performance test too fast, propably something went wrong");
		}
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(IgnoringTest);
CPPUNIT_TEST_SUITE_REGISTRATION(LinuxItemFactoryTest);

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
