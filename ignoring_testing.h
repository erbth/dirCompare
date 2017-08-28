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

#ifndef _IGNORING_TESTING_H
#define _IGNORING_TESTING_H

#include "dirCompareConfig.h"

#ifdef WITH_TESTING

#include <string>
#include "ignoring.h"

bool ignoring_match_test(const string& cpptext, const string& cpppattern);

#endif /* WITH_TESTING */

#endif /* _IGNORING_TESTING_H */
