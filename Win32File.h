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

#ifndef _WIN32_FILE_H
#define _WIN32_FILE_H

#include <iostream>
#include <memory>
#include <string>
#include "SystemParameters.h"
#include "Item.h"
#include "File.h"
#include "Directory.h"

extern "C"
{
	#include <Windows.h>
}

class Directory;

class Win32File : public File
{
private:
	void init();
	
protected:
	HANDLE handle;
	std::wstring wpath;

public:
	Win32File(const std::wstring& path, std::shared_ptr<SystemParameters> sp);
	Win32File(
		const std::wstring& path,
		std::shared_ptr<SystemParameters> sp,
		std::shared_ptr<const Directory> dir);

	virtual ~Win32File();

	HANDLE getHandle() const;

	virtual std::ostream& dump(std::ostream& o) const
	{
		return o << "WindowsFile: " << path;
	}
};

#endif /* _WIN32_FILE_H */
