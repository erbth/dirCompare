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

#include <vector>
#include <memory>
#include "SystemParameters.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "Win32ComparisonFactory.h"
#include "Win32SimpleFileComparison.h"
#include "SimpleDirectoryComparison.h"

using namespace std;

Win32ComparisonFactory::Win32ComparisonFactory(shared_ptr<SystemParameters> sp)
	: ComparisonStrategyFactory(sp)
{
}

vector<shared_ptr<FileComparisonStrategy>>
	Win32ComparisonFactory::createFileStrategies()
{
	vector<shared_ptr<FileComparisonStrategy>> v;

	v.push_back(make_shared<Win32SimpleFileComparison>(sp));

	return v;
}

vector<shared_ptr<DirectoryComparisonStrategy>>
	Win32ComparisonFactory::createDirStrategies()
{
	vector<shared_ptr<DirectoryComparisonStrategy>> v;

	v.push_back(make_shared<SimpleDirectoryComparison>(sp));

	return v;
}
