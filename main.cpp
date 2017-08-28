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

#include <iostream>
#include <vector>
#include <memory>
#include <exception>
#include "dirCompareConfig.h"
#include "platform.h"
#include "gp_exception.h"
#include "ItemFactory.h"
#include "ComparisonStrategyFactory.h"
#include "ComparisonContext.h"
#include "Commandline.h"
#include "SystemParameters.h"

static void printHelp()
{
	cout << "This is dirCompare version " << DIR_COMPARE_VERSION_MAJOR
		<< "." << DIR_COMPARE_VERSION_MINOR << endl << endl;

	cout << "Copyright 2017 Thomas Erbesdobler <t.erbesdobler@team103.com>"
		<< endl << endl;

	cout << "Licensed under the Apache License, Version 2.0 (the \"License\");"
		<< endl <<
		"you may not use this file except in compliance with the License."
		<< endl <<
		"You may obtain a copy of the License at" << endl << endl <<
		"    http://www.apache.org/licenses/LICENSE-2.0" << endl << endl <<

	"Unless required by applicable law or agreed to in writing, software"
	<< endl <<
	"distributed under the License is distributed on an \"AS IS\" BASIS,"
	<< endl <<
	"WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied."
	<< endl <<
	"See the License for the specific language governing permissions and"
	<< endl <<
	"limitations under the License." << endl << endl;

	cout << "Parameter summary:" << endl;
	cout << "--help" << endl;
	cout << "--dir1 <path>" << endl;
	cout << "--dir2 <path>" << endl;
	cout << "--listStrategies" << endl;
	cout << "--fileStrategy <id>" << endl;
	cout << "--dirStrategy <id>" << endl;
	cout << "--ignoreFile <name>" << endl;
	cout << "--ignoreDir <name>" << endl;
	cout << "--logfile <path>" << endl;

	cout << endl << "For further information please consult the "
		"documentation." << endl;
}

static int dirCompare(int argc, char** argv)
{
	Commandline cl(argc - 1, argv + 1);
	auto sp = make_shared<SystemParameters>();

	try
	{
		cl.parse(sp);
	}
	catch (gp_exception e)
	{
		cerr << e.what() << endl;
		cerr << endl << "Maybe give --help a try?" << endl;
		return 1;
	}

	if (!sp->isValid(cerr))
	{
		cerr << endl << "invalid parameters, exiting. "
			"Maybe give --help a try?" << endl;

		return 1;
	}

	if (sp->getHelp())
	{
		printHelp();
		return 0;
	}
	else if (sp->getListStrategies())
	{
		auto csf = createComparisonStrategyFactory(sp);

		auto fileStrategies = csf->createFileStrategies();
		auto dirStrategies = csf->createDirStrategies();

		cout << "File comparison strategies: (id) description" << endl;
		cout << "-----------------------------------------------------------" << endl;

		for (auto s : fileStrategies)
		{
			cout << "(" << s->getID() << ") " << s->getDescription() << endl;
		}

		cout << endl << "Directory comparison strategies: (id) description" << endl;
		cout << "-----------------------------------------------------------" << endl;

		for (auto s : dirStrategies)
		{
			cout << "(" << s->getID() << ") " << s->getDescription() << endl;
		}

		return 0;
	}
	else
	{
		*(sp->getLog()) << endl << "ignored files:" << endl;
		for (auto f : sp->getIgnoreFiles())
		{
			*(sp->getLog()) << f << endl;
		}

		*(sp->getLog()) << endl << "ignored directories:" << endl;
		for (auto d : sp->getIgnoreDirectories())
		{
			*(sp->getLog()) << d << endl;
		}

		*(sp->getLog()) << endl;

		auto csf = createComparisonStrategyFactory(sp);

		auto fileStrategy = csf->createFileStrategy(
			sp->getFileComparisonStrategy());

		auto dirStrategy = csf->createDirStrategy(
			sp->getDirectoryComparisonStrategy());

		ComparisonContext cc(sp);
		cc.setFileComparisonStrategy(fileStrategy);
		cc.setDirectoryComparisonStrategy(dirStrategy);

		auto f = createItemFactory(sp);

		/* test directory */
		auto d1 = f->createDirectory(sp->getDirectory1());
		auto d2 = f->createDirectory(sp->getDirectory2());

		bool equal = cc.compare(d1, d2);
		*(sp->getLog()) << (equal ? "match" : "differ") << endl;

		return equal ? 0 : 1;
	}
}

int main(int argc, char** argv)
{
	int result = 1;

	try
	{
		result = dirCompare(argc, argv);
	}
	catch (exception& e)
	{
		cerr << "Caught an exception. That means the program was in an erroneous"
		" state so that further processing might have lead to failure. Therefore"
		" the program is terminated." << endl << "Error message: " << e.what()
		<< endl;

		result = 1;
	}
	catch (...)
	{
		cerr << "Caught an unknown exception. That means the program was in"
		" an unknown erroneous state so that further processing might have"
		" lead to failure. Therefore the program is terminated." << endl;

		result = 1;
	}

	return result;
}
