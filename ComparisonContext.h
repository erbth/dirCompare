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

#ifndef _COMPARISON_CONTEXT_H
#define _COMPARISON_CONTEXT_H

#include <memory>
#include "SystemParameters.h"
#include "Item.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"

using namespace std;

class ComparisonContext
{
private:
	shared_ptr<SystemParameters> sp;
	shared_ptr<FileComparisonStrategy> fileStrategy;
	shared_ptr<DirectoryComparisonStrategy> dirStrategy;

public:
	ComparisonContext(shared_ptr<SystemParameters> sp);

	shared_ptr<FileComparisonStrategy> getFileComparisonStrategy() const;
	void setFileComparisonStrategy(shared_ptr<FileComparisonStrategy> s);

	shared_ptr<DirectoryComparisonStrategy> getDirectoryComparisonStrategy() const;
	void setDirectoryComparisonStrategy(shared_ptr<DirectoryComparisonStrategy> s);

	bool compare(
		shared_ptr<const Item> i1,
		shared_ptr<const Item> i2) const;
};

#endif /* _COMPARISON_CONTEXT_H */
