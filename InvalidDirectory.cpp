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

#include <memory>
#include <string>
#include <vector>
#include "InvalidDirectory.h"
#include "SystemParameters.h"
#include "InvalidItem.h"
#include "Directory.h"
#include "gp_exception.h"

using namespace std;

InvalidDirectory::InvalidDirectory(
	const string& path,
	shared_ptr<SystemParameters> sp)
	: Directory(sp)
{
	int pos = path.rfind('\\');
	if (pos == string::npos)
	{
		name = path;
		this->path = path;
	}
	else
	{
		if (pos == path.size() - 1)
			name = '\\';
		else
			name = path.substr(pos + 1LL, path.size() - pos - 1);

		this->path = path;
	}
}

InvalidDirectory::InvalidDirectory(
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: Directory(name, dir->getPath() + "\\" + name, sp, dir)
{
}

const string InvalidDirectory::getErrorDescription() const
{
	return "Unable to open directory \"" + path + "\": " +
		getErrorMessage();
}

vector<shared_ptr<Item>> InvalidDirectory::getItems() const
{
	throw gp_exception("Calling getItems() on InvalidDirectory is invalid.");
}
