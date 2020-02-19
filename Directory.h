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

#ifndef _DIRECTORY_H
#define _DIRECTORY_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <memory>
#include "SystemParameters.h"
#include "Item.h"

class Directory : public Item, public std::enable_shared_from_this<Directory>
{
protected:
	Directory(std::shared_ptr<SystemParameters> sp);
	Directory(const std::string& name, const std::string& path, std::shared_ptr<SystemParameters> sp);
	Directory(std::shared_ptr<SystemParameters> sp, std::shared_ptr<const Directory> dir);
	Directory(
		const std::string& name,
		const std::string& path,
		std::shared_ptr<SystemParameters> sp,
		std::shared_ptr<const Directory> dir);

public:
	virtual ~Directory() = 0;

	virtual std::vector<std::shared_ptr<Item>> getItems() const = 0;

	virtual std::ostream& dump(std::ostream& o) const
	{
		o << "Directory: " << path << "\n" <<
			"-----------------------------------------------" << endl;

		for (auto i : getItems())
		{
			o << i->getPath() << endl;
		}

		return o;
	}
};

#endif /* _DIRECTORY_H */
