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
#include "SystemParameters.h"
#include "gp_exception.h"
#include "ItemFactory.h"
#include "ComparisonStrategyFactory.h"
#include "platform.h"

#if TARGET_PLATFORM == LINUX
#include "LinuxItemFactory.h"
#include "LinuxComparisonFactory.h"
#elif TARGET_PLATFORM == WIN32
#pragma message (": warning: not implemented yet")
#endif

shared_ptr<ItemFactory> createItemFactory(shared_ptr<SystemParameters> sp)
{
#if TARGET_PLATFORM == LINUX
	return make_shared<LinuxItemFactory>(sp);
#elif TARGET_PLATFORM == WIN32
	return nullptr;
#endif
}

shared_ptr<ComparisonStrategyFactory> createComparisonStrategyFactory(
	shared_ptr<SystemParameters> sp)
{
#if TARGET_PLATFORM == LINUX
	return make_shared<LinuxComparisonFactory>(sp);
#elif TARGET_PLATFORM == WIN32
	return nullptr;
#endif
}
