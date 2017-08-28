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

#ifndef _ITEM_H
#define _ITEM_H

#include <iostream>
#include <memory>
#include <string>
#include "SystemParameters.h"

using namespace std;

class Directory;

class Item
{
protected:
	shared_ptr<SystemParameters> sp;
	string path;
	int level;
	shared_ptr<const Directory> directory;

public:
	Item(const string& path, shared_ptr<SystemParameters> sp);
	Item(
		const string& path,
		shared_ptr<SystemParameters> sp,
		shared_ptr<const Directory> dir);

	virtual ~Item();

	const string& getPath() const;
	const int getLevel() const;

	virtual ostream& dump(ostream& o) const
	{
		return o << "Item: " << path;
	}
};

ostream& operator<<(ostream& o, const Item *i);

#endif /* _ITEM_H */
