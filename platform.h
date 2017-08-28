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

#ifndef _PLATFORM_H
#define _PLATFORM_H

#include <memory>
#include "SystemParameters.h"
#include "ItemFactory.h"
#include "ComparisonStrategyFactory.h"
#include "dirCompareConfig.h"

#ifndef TARGET_PLATFORM
#error TARGET_PLATFORM undefined
#else
#if TARGET_PLATFORM == 0
#error TARGET_PLATFORM invalid
#endif
#endif

/* methods for creating platform dependent factories */
shared_ptr<ItemFactory> createItemFactory(shared_ptr<SystemParameters> sp);
shared_ptr<ComparisonStrategyFactory> createComparisonStrategyFactory(
	shared_ptr<SystemParameters> sp);

#endif /* _PLATFORM_H */
