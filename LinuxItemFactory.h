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

#ifndef _LINUX_ITEM_FACTORY_H
#define _LINUX_ITEM_FACTORY_H

#include <memory>
#include <string>
#include "SystemParameters.h"
#include "File.h"
#include "Directory.h"
#include "ItemFactory.h"

class LinuxItemFactory : public ItemFactory
{
private:
	template<class T> shared_ptr<T> createInvalidItem(
		const string& path,
		shared_ptr<const Directory> parent,
		const string message) const;

public:
	LinuxItemFactory(shared_ptr<SystemParameters> sp);

	virtual shared_ptr<File> createFile(
		const string& path,
		shared_ptr<const Directory> parent = nullptr) noexcept override;

	virtual shared_ptr<Directory> createDirectory(
		const string& path,
		shared_ptr<const Directory> parent = nullptr) noexcept override;
};

#endif /* _LINUX_ITEM_FACTORY_H */
