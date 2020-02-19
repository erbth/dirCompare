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

#include <memory>
#include <string>
#include "SystemParameters.h"
#include "File.h"
#include "Directory.h"
#include "Item.h"

using namespace std;

File::File(shared_ptr<SystemParameters> sp)
	: Item(string(), string(), sp)
{
}

File::File(const string& name, const string& path, shared_ptr<SystemParameters> sp)
	: Item(name, path, sp)
{
}

File::File(shared_ptr<SystemParameters> sp, shared_ptr<const Directory> dir)
	: Item(string(), string(), sp, dir)
{
}

File::File(
	const string& name,
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: Item(name, path, sp, dir)
{
}

File::~File()
{
}
