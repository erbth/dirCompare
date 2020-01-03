/** This file is part of dirCompare
 *
 * Copyright 2020 Thomas Erbesdobler <t.erbesdobler@team103.com>
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

#ifndef _WIN32_ITEM_FACTORY_H
#define _WIN32_ITEM_FACTORY_H

#include <memory>
#include <string>
#include "SystemParameters.h"
#include "File.h"
#include "Directory.h"
#include "ItemFactory.h"

class Win32ItemFactory : public ItemFactory
{
private:
	template<class T> std::shared_ptr<T> createInvalidItem(
		const std::string& path,
		std::shared_ptr<const Directory> parent,
		const std::string message) const;

public:
	Win32ItemFactory(std::shared_ptr<SystemParameters> sp);

	virtual std::shared_ptr<File> createFile(
		const std::string& path,
		std::shared_ptr<const Directory> parent = nullptr) noexcept override;

	virtual std::shared_ptr<Directory> createDirectory(
		const std::string& path,
		std::shared_ptr<const Directory> parent = nullptr) noexcept override;
};

#endif /* _WIN32_ITEM_FACTORY_H */
