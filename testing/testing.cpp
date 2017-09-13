/** This file is part of dirCompare
 *
 * Copyright 2017 Thomas Erbesdobler <t.erbesdobler@team103.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

#include <memory>
#include <string>
#include <sstream>
#include "ItemFactory.h"
#include "LinuxItemFactory.h"
#include "File.h"
#include "Directory.h"
#include "StubDirectory.h"
#include "InvalidFile.h"
#include "InvalidDirectory.h"
#include "ComparisonContext.h"
#include "SimpleDirectoryComparison.h"
#include "StubFileComparisonStrategy.h"
#include "ignoring_testing.h"
#include "platform.h"

#if TARGET_PLATFORM == WINDOWS_WIN32
#include "win32_testing.h"
#endif

using namespace CPPUNIT_NS;
using namespace std;

class SimpleDirectoryComparisonTest : public TestFixture
{
	CPPUNIT_TEST_SUITE(SimpleDirectoryComparisonTest);

	CPPUNIT_TEST(both_valid);
	CPPUNIT_TEST(one_invalid);
	CPPUNIT_TEST(both_invalid);

	CPPUNIT_TEST_SUITE_END();

	shared_ptr<stringstream> log;
	shared_ptr<SystemParameters> sp;
	shared_ptr<FileComparisonStrategy> fileStub;
	shared_ptr<DirectoryComparisonStrategy> dirSimple;
	shared_ptr<ComparisonContext> compCtx;

public:
	void setUp() override
	{
		sp = make_shared<SystemParameters>();
		fileStub = make_shared<StubFileComparisonStrategy>(sp);
		dirSimple = make_shared<SimpleDirectoryComparison>(sp);
		compCtx = make_shared<ComparisonContext>(sp);

		compCtx->setFileComparisonStrategy(fileStub);
		compCtx->setDirectoryComparisonStrategy(dirSimple);

		log = make_shared<stringstream>();
		sp->setLog(log);
	}

	void both_valid()
	{
		log->str(string());
		auto d = make_shared<StubDirectory>("stub", sp);

		CPPUNIT_ASSERT(dirSimple->compare(d, d) == true);
		CPPUNIT_ASSERT(log->str() == string());
	}

	void one_invalid()
	{
		log->str(string());
		
		auto d1 = make_shared<InvalidDirectory>("test", sp);
		auto d2 = make_shared<StubDirectory>("stub", sp);

		d1->setErrorMessage("test message");


		CPPUNIT_ASSERT(dirSimple->compare(d1, d2) == false);

		stringstream reference;
		reference << "Unable to open directory \"test\" in directory 1: "
			"test message" << endl;
		CPPUNIT_ASSERT(log->str() == reference.str());


		log->str(string());
		CPPUNIT_ASSERT(dirSimple->compare(d2, d1) == false);

		reference.str(string());
		reference << "Unable to open directory \"test\" in directory 2: "
			"test message" << endl;
		CPPUNIT_ASSERT(log->str() == reference.str());
	}

	void both_invalid()
	{
		log->str(string());
		auto d = make_shared<InvalidDirectory>("test", sp);

		d->setErrorMessage("test message");

		CPPUNIT_ASSERT(dirSimple->compare(d, d) == false);

		stringstream reference;
		reference << "Unable to open directory \"test\" in directory 1: "
			"test message" << endl <<
			"Unable to open directory \"test\" in directory 2: "
			"test message" << endl;
		CPPUNIT_ASSERT(log->str() == reference.str());
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

		if (elapsed_seconds > 10.0)
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

		if (elapsed_seconds > 20.0)
		{
			CPPUNIT_FAIL("performance test took too long");
		}

		if (elapsed_seconds < 5e-3)
		{
			CPPUNIT_FAIL("performance test too fast, propably something went wrong");
		}
	}
};

// CPPUNIT_TEST_SUITE_REGISTRATION(IgnoringTest);
// CPPUNIT_TEST_SUITE_REGISTRATION(SimpleDirectoryComparisonTest);

#if TARGET_PLATFORM == WINDOWS_WIN32
CPPUNIT_TEST_SUITE_REGISTRATION(Win32FileTest);
#endif

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

	CompilerOutputter compilerOutputter(&collectedResults, cerr);
	compilerOutputter.write();

	return collectedResults.wasSuccessful() ? 0 : 1;
}
