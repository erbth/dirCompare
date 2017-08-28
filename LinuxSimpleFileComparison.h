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

#ifndef _LINUX_SIMPLE_FILE_COMPARISON_H
#define _LINUX_SIMPLE_FILE_COMPARISON_H

#include <string>
#include <memory>
#include "SystemParameters.h"
#include "File.h"
#include "LinuxFileComparisonStrategy.h"

using namespace std;

class LinuxSimpleFileComparison : public LinuxFileComparisonStrategy
{
public:
	LinuxSimpleFileComparison(shared_ptr<SystemParameters> sp);

	virtual bool compare(
		shared_ptr<const File> f1,
		shared_ptr<const File> f2,
		string* reason = nullptr) const noexcept override;

	virtual const string getID() const override;
	virtual const string getDescription() const override;
};

#endif /* _LINUX_SIMPLE_FILE_COMPARISON_H */
