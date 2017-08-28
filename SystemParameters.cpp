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

#include <memory>
#include <string>
#include <ostream>
#include <vector>
#include <algorithm>
#include "gp_exception.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "SystemParameters.h"

using namespace std;

SystemParameters::SystemParameters() :
	listStrategies(false),
	log(&cout, [](ostream*){}),
	externalLog(false),
	help(false)
{
}

string SystemParameters::getDirectory1() const
{
	return directory1;
}

string SystemParameters::getDirectory2() const
{
	return directory2;
}

bool SystemParameters::getListStrategies() const
{
	return listStrategies;
}

const string SystemParameters::getFileComparisonStrategy() const
{
	return fileComparisonStrategy;
}

const string SystemParameters::getDirectoryComparisonStrategy() const
{
	return directoryComparisonStrategy;
}

const vector<string> SystemParameters::getIgnoreFiles() const
{
	return ignoreFiles;
}

const vector<string> SystemParameters::getIgnoreDirectories() const
{
	return ignoreDirectories;
}

shared_ptr<ostream> SystemParameters::getLog() const
{
	return log;
}

bool SystemParameters::getHelp() const
{
	return help;
}

void SystemParameters::setDirectory1(string dir)
{
	directory1 = dir;
}

void SystemParameters::setDirectory2(string dir)
{
	directory2 = dir;
}

void SystemParameters::setListStrategies(bool l)
{
	listStrategies = l;
}

void SystemParameters::setFileComparisonStrategy(const string s)
{
	fileComparisonStrategy = s;
}

void SystemParameters::setDirectoryComparisonStrategy(const string s)
{
	directoryComparisonStrategy = s;
}

void SystemParameters::setLog(shared_ptr<ostream> l)
{
	log = l;
	externalLog = true;
}

void SystemParameters::setHelp(bool help)
{
	this->help = help;
}

bool SystemParameters::isValid(ostream& errors) const
{
	if (listStrategies || help)
	{
		return true;
	}
	else
	{
		if (directory1.empty())
		{
			errors << "mandatory: directory 1's path not given" << endl;
			return false;
		}
			
		if (directory2.empty())
		{
			errors << "mandatory: directory 2's path not given" << endl;
			return false;
		}

		if (fileComparisonStrategy.empty())
		{
			errors << "mandatory: no file comparison strategy given" << endl;
			return false;
		}
		
		if (directoryComparisonStrategy.empty())
		{
			errors << "mandatory: no directory comparison strategy given" << endl;
			return false;
		}

		if (log == nullptr)
		{
			throw gp_exception("SystemParameters.log is nullptr");
		}

		return true;
	}
}

bool SystemParameters::isExternalLogSet() const
{
	return externalLog;
}

void SystemParameters::ignoreFile(const string name)
{
	if (!name.empty())
	{
		if (find(
			ignoreFiles.cbegin(),
			ignoreFiles.cend(),
			name) == ignoreFiles.cend())
		{
			ignoreFiles.push_back(name);
		}
	}
}

void SystemParameters:: ignoreDirectory(const string name)
{
	if (!name.empty())
	{
		if (find(
			ignoreDirectories.cbegin(),
			ignoreDirectories.cend(),
			name) == ignoreDirectories.cend())
		{
			ignoreDirectories.push_back(name);
		}
	}
}
