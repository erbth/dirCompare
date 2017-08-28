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

#ifndef _LINUX_COMPARISON_FACTORY_H
#define _LINUX_COMPARISON_FACTORY_H

#include <vector>
#include <memory>
#include "SystemParameters.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "ComparisonStrategyFactory.h"

using namespace std;

class LinuxComparisonFactory : public ComparisonStrategyFactory
{
public:
	LinuxComparisonFactory(shared_ptr<SystemParameters> sp);

	virtual vector<shared_ptr<FileComparisonStrategy>>
		createFileStrategies() override;

	virtual vector<shared_ptr<DirectoryComparisonStrategy>>
		createDirStrategies() override;
};

#endif /* _LINUX_COMPARISON_FACTORY_H */
