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

#ifndef _COMPARISON_STRATEGY_FACTORY_H
#define _COMPARISON_STRATEGY_FACTORY_H

#include <vector>
#include <memory>
#include <string>
#include "SystemParameters.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"

using namespace std;

class ComparisonStrategyFactory
{
protected:
	shared_ptr<SystemParameters> sp;

public:
	ComparisonStrategyFactory(shared_ptr<SystemParameters> sp);

	virtual vector<shared_ptr<FileComparisonStrategy>>
		createFileStrategies() = 0;

	virtual vector<shared_ptr<DirectoryComparisonStrategy>>
		createDirStrategies() = 0;

	shared_ptr<FileComparisonStrategy> createFileStrategy(const string id);
	shared_ptr<DirectoryComparisonStrategy> createDirStrategy(const string id);
};

#endif /* _COMPARISON_STRATEGY_FACTORY_H */
