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
#include "InvalidFile.h"
#include "InvalidDirectory.h"
#include "LinuxFile.h"
#include "LinuxDirectory.h"
#include "LinuxComparisonFactory.h"
#include "ComparisonContext.h"
#include "ignoring_testing.h"

class LinuxSimpleFileComparisonTest : public TestFixture
{
	CPPUNIT_TEST_SUITE(LinuxSimpleFileComparisonTest);

	CPPUNIT_TEST(both_valid);
	CPPUNIT_TEST(one_invalid);
	CPPUNIT_TEST(both_invalid);

	CPPUNIT_TEST_SUITE_END();

	shared_ptr<stringstream> log;
	shared_ptr<SystemParameters> sp;
	shared_ptr<ComparisonStrategyFactory> csf;
	shared_ptr<FileComparisonStrategy> fileSimple;

public:
	void setUp() override
	{
		sp = make_shared<SystemParameters>();
		csf = make_shared<LinuxComparisonFactory>(sp);
		fileSimple = csf->createFileStrategy("simple");

		log = make_shared<stringstream>();
		sp->setLog(log);
	}

	void both_valid()
	{
		log->str(string());
		auto f = make_shared<LinuxFile>("testing", sp);

		CPPUNIT_ASSERT(fileSimple->compare(f, f) == true);
		CPPUNIT_ASSERT(log->str() == string());
	}

	void one_invalid()
	{
		log->str(string());

		auto f1 = make_shared<InvalidFile>("test", sp);
		auto f2 = make_shared<LinuxFile>("testing", sp);

		f1->setErrorMessage("test message");


		CPPUNIT_ASSERT(fileSimple->compare(f1, f2) == false);

		stringstream reference;
		reference << "Unable to open file \"test\" in directory 1: "
			"test message" << endl;
		CPPUNIT_ASSERT(log->str() == reference.str());


		log->str(string());
		CPPUNIT_ASSERT(fileSimple->compare(f2, f1) == false);

		reference.str(string());
		reference << "Unable to open file \"test\" in directory 2: "
			"test message" << endl;
		CPPUNIT_ASSERT(log->str() == reference.str());
	}

	void both_invalid()
	{
		log->str(string());
		auto f = make_shared<InvalidFile>("test", sp);

		f->setErrorMessage("test message");

		CPPUNIT_ASSERT(fileSimple->compare(f, f) == false);

		stringstream reference;
		reference << "Unable to open file \"test\" in directory 1: "
			"test message" << endl <<
			"Unable to open file \"test\" in directory 2: "
			"test message" << endl;
		CPPUNIT_ASSERT(log->str() == reference.str());
	}
};

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
		shared_ptr<File> f = factory->createFile("testing");

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
		shared_ptr<Directory> d = factory->createDirectory(".");

		CPPUNIT_ASSERT(dynamic_pointer_cast<InvalidItem>(d) == nullptr);
		CPPUNIT_ASSERT(dynamic_pointer_cast<LinuxDirectory>(d) != nullptr);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(LinuxItemFactoryTest);
CPPUNIT_TEST_SUITE_REGISTRATION(LinuxSimpleFileComparisonTest);