/** This file is part of dirCompare
 *
 * Copyright 2019 Thomas Erbesdobler <t.erbesdobler@team103.com>
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

#ifndef _WIN32_ITEM_H
#define _WIN32_ITEM_H

#include <string>

extern "C" {
#include <Windows.h>
}

class Win32Item
{
public:
	virtual DWORD getType() const = 0;
	virtual uint64_t getSize() const = 0;
	virtual LARGE_INTEGER getCreationTime() const = 0;
	virtual LARGE_INTEGER getLastAccessTime() const = 0;
	virtual LARGE_INTEGER getLastWriteTime() const = 0;
	virtual LARGE_INTEGER getChangeTime() const = 0;
	virtual DWORD getAttributes() const = 0;
	virtual const PSID getOwner() const = 0;
	virtual const PSID getGroup() const = 0;
	virtual const PACL getDacl() const = 0;
	virtual bool isDaclProtected() const = 0;

	/* 'All attributes' means: DACL, owner, group, timestamps, size, attributes and type. */
	bool compare_all_attributes_to(const Win32Item* other, std::string &reason) const;
};

#endif /* _WIN32_ITEM_H */