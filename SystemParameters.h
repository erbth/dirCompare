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

#ifndef _SYSTEM_PARAMETERS_H
#define _SYSTEM_PARAMETERS_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using namespace std;

class FileComparisonStrategy;
class DirectoryComparisonStrategy;

class SystemParameters
{
protected:
	string directory1;
	string directory2;
	bool listStrategies;
	string fileComparisonStrategy;
	string directoryComparisonStrategy;
	vector<string> ignoreFiles;
	vector<string> ignoreDirectories;
	shared_ptr<ostream> log;
	bool externalLog;
	bool help;

public:
	SystemParameters();

	string getDirectory1() const;
	string getDirectory2() const;
	bool getListStrategies() const;
	const string getFileComparisonStrategy() const;
	const string getDirectoryComparisonStrategy() const;
	const vector<string> getIgnoreFiles() const;
	const vector<string> getIgnoreDirectories() const;
	shared_ptr<ostream> getLog() const;
	bool getHelp() const;

	void setDirectory1(string dir);
	void setDirectory2(string dir);
	void setListStrategies(bool l);
	void setFileComparisonStrategy(const string s);
	void setDirectoryComparisonStrategy(const string s);
	void setLog(shared_ptr<ostream> l);
	void setHelp(bool help);

	bool isValid(ostream& errors) const;
	bool isExternalLogSet() const;
	void ignoreFile(const string name);
	void ignoreDirectory(const string name);
};

#endif /* _SYSTEM_PARAMETERS_H */
