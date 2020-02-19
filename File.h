/** This file is part of dirCompare
 *
 * Copyright 2017-2020 Thomas Erbesdobler <t.erbesdobler@team103.com>
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

#ifndef _FILE_H
#define _FILE_H

#include <iostream>
#include <memory>
#include <string>
#include "SystemParameters.h"
#include "Item.h"
#include "Directory.h"


class File : public Item
{
protected:
	File(std::shared_ptr<SystemParameters> sp);
	File(const std::string& name, const std::string& path, std::shared_ptr<SystemParameters> sp);
	File(std::shared_ptr<SystemParameters> sp, std::shared_ptr<const Directory> dir);
	File(
		const std::string& name,
		const std::string& path,
		std::shared_ptr<SystemParameters> sp,
		std::shared_ptr<const Directory> dir);

public:
	virtual ~File() = 0;

	virtual std::ostream& dump(std::ostream& o) const override
	{
		return o << "File: " << path;
	}
};

#endif /* _FILE_H */
