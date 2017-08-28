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

#ifndef _INVALID_DIRECTORY_H
#define _INVALID_DIRECTORY_H

#include <ostream>
#include <memory>
#include <string>
#include <vector>
#include "SystemParameters.h"
#include "Directory.h"
#include "InvalidItem.h"

class InvalidDirectory : public Directory, public InvalidItem
{
public:
	InvalidDirectory(const std::string& path, shared_ptr<SystemParameters> sp);
	InvalidDirectory(
		const std::string& path,
		shared_ptr<SystemParameters> sp,
		shared_ptr<const Directory> dir);
	
	virtual const std::string getErrorDescription() const override;
	virtual vector<shared_ptr<Item>> getItems() const override;
	
	virtual ostream& dump(ostream& o) const override
	{
		return o << "InvalidDirectory: \"" << path << "\", Error: " <<
			getErrorMessage();
	}
};

#endif /* _INVALID_DIRECTORY_H */
