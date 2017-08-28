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

#ifndef _LINUX_DIRECTORY_H
#define _LINUX_DIRECTORY_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <memory>
#include "SystemParameters.h"
#include "Directory.h"
#include "Item.h"
#include "LinuxFileInfo.h"

extern "C"
{
	#include <dirent.h>
}

class LinuxDirectory : public Directory
{
private:
	void init();

protected:
	DIR *dir;

public:
	LinuxDirectory(const string& path, shared_ptr<SystemParameters> sp);
	LinuxDirectory(
		const string& path,
		shared_ptr<SystemParameters> sp,
		shared_ptr<const Directory> dir);

	virtual ~LinuxDirectory() override;

	virtual vector<shared_ptr<Item>> getItems() const override;

	LinuxFileInfo getFileInfo() const;

	int getFd() const;

	virtual ostream& dump(ostream& o) const
	{
		o << "LinuxDirectory: " << path << "\n" <<
			"-----------------------------------------------" << endl;

		for (auto i : getItems())
		{
			o << i->getPath() << endl;
		}

		return o;
	}
};

#endif /* _LINUX_DIRECTORY_H */
