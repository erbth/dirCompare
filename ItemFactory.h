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

#ifndef _ITEM_FACTORY_H
#define _ITEM_FACTORY_H

#include <memory>
#include <string>
#include "SystemParameters.h"
#include "File.h"
#include "Directory.h"

class ItemFactory
{
protected:
	shared_ptr<SystemParameters> sp;

public:
	ItemFactory(shared_ptr<SystemParameters> sp);

	virtual shared_ptr<File> createFile(
		const string& path,
		shared_ptr<const Directory> parent = nullptr) noexcept = 0;

	virtual shared_ptr<Directory> createDirectory(
		const string& path,
		shared_ptr<const Directory> parent = nullptr) noexcept = 0;
};

#endif /* _ITEM_FACTORY_H */
