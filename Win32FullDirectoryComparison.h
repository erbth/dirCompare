/** This file is part of dirCompare
 *
 * Copyright 2020 Thomas Erbesdobler <t.erbesdobler@team103.com>
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

#ifndef _WIN32_FULL_DIRECTORY_COMPARISON_H
#define _WIN32_FULL_DIRECTORY_COMPARISON_H

#include <string>
#include <memory>
#include "SystemParameters.h"
#include "Directory.h"
#include "DirectoryComparisonStrategy.h"

class Win32FullDirectoryComparison : public DirectoryComparisonStrategy
{
public:
	Win32FullDirectoryComparison(std::shared_ptr<SystemParameters> sp);

	virtual bool compare(
		std::shared_ptr<const Directory> d1,
		std::shared_ptr<const Directory> d2,
		std::string* reason = nullptr) const override;

	virtual const std::string getID() const override;
	virtual const std::string getDescription() const override;
};

#endif /* _WIN32_FULL_DIRECTORY_COMPARISON_H */
