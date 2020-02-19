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
	HANDLE handle = INVALID_HANDLE_VALUE;
	std::wstring wname;
	std::wstring wpath;

	mutable DWORD fileType = FILE_TYPE_UNKNOWN;
	mutable uint64_t size;
	mutable bool sizeValid = false;

	mutable FILE_BASIC_INFO fbi;
	mutable bool fbiValid = false;
	void getFileBasicInfo() const;

	mutable PSECURITY_DESCRIPTOR pSecurityDescriptor;
	mutable PSID pSidOwner;
	mutable PSID pSidGroup;
	mutable PACL pDacl;
	mutable bool securityInfoValid = false;
	void getSecurityInfo() const;

public:
	Win32File(const std::wstring& path, std::shared_ptr<SystemParameters> sp);
	Win32File(
		const std::wstring& name,
		std::shared_ptr<SystemParameters> sp,
		std::shared_ptr<const Directory> dir);

	virtual ~Win32File();

	HANDLE getHandle() const;

	DWORD getType() const;
	uint64_t getSize() const;
	LARGE_INTEGER getCreationTime() const;
	LARGE_INTEGER getLastAccessTime() const;
	LARGE_INTEGER getLastWriteTime() const;
	LARGE_INTEGER getChangeTime() const;
	DWORD getAttributes() const;
	const PSID getOwner() const;
	const PSID getGroup() const;
	const PACL getDacl() const;
	bool isDaclProtected() const;

	bool compare_all_attributes_to(const Win32File* other, string& reason) const;

	virtual std::ostream& dump(std::ostream& o) const
	{
		return o << "Win32File: " << path;
	}
};

#endif /* _WIN32_FILE_H */
