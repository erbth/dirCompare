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

#ifndef _DIRECTORY_COMPARISON_STRATEGY_H
#define _DIRECTORY_COMPARISON_STRATEGY_H

#include <string>
#include <memory>
#include "SystemParameters.h"
#include "Directory.h"
#include "ItemFactory.h"

class ComparisonContext;

class DirectoryComparisonStrategy :
	public enable_shared_from_this<DirectoryComparisonStrategy>
{
protected:
	shared_ptr<SystemParameters> sp;
	const ComparisonContext* comparisonContext = nullptr;

public:
	DirectoryComparisonStrategy(shared_ptr<SystemParameters> sp);

	const ComparisonContext *getComparisonContext() const;
	void setComparisonContext(ComparisonContext *c);

	virtual bool compare(
		shared_ptr<const Directory> d1,
		shared_ptr<const Directory> d2,
		string* reason = nullptr) const = 0;

	virtual const string getID() const = 0;
	virtual const string getDescription() const = 0;
};

#endif /* _DIRECTORY_COMPARISON_STRATEGY_H */
