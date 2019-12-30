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

#ifndef _WIN32_SECURITY_TOOLS_H
#define _WIN32_SECURITY_TOOLS_H

extern "C" {
#include <Windows.h>
#include <AclAPI.h>
#include <AccCtrl.h>
}

bool win32_explicit_aces_equal(const EXPLICIT_ACCESS &e1, const EXPLICIT_ACCESS &e2);
bool win32_trustees_equal(const TRUSTEE& t1, const TRUSTEE& t2);

#endif /* _WIN32_SECURITY_TOOLS_H */