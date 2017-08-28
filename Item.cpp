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

#include <iostream>
#include <memory>
#include <string>
#include "SystemParameters.h"
#include "Item.h"
#include "Directory.h"

using namespace std;

Item::Item(const string& path, shared_ptr<SystemParameters> sp)
{
	this->path.assign(path);
	this->sp = sp;
	level = 0;
	directory = nullptr;
}

Item::Item(
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
{
	this->path = path;
	this->sp = sp;
	level = dir->getLevel() + 1;
	directory = dir;
}

Item::~Item()
{
}

const string& Item::getPath() const
{
	return path;
}

const int Item::getLevel() const
{
	return level;
}

ostream& operator<<(ostream& o, const Item *i) { return i->dump(o); }
