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

#include "win32_security_tools.h"
#include "gp_exception.h"

using namespace std;

bool win32_trustees_equal(const TRUSTEE& t1, const TRUSTEE& t2)
{
	if (t1.TrusteeForm != TRUSTEE_IS_SID || t2.TrusteeForm != TRUSTEE_IS_SID)
		throw gp_exception("At least one trustee is not of the form SID.");

	return EqualSid(t1.ptstrName, t2.ptstrName);
}

bool win32_explicit_aces_equal(const EXPLICIT_ACCESS& e1, const EXPLICIT_ACCESS& e2)
{
	return e1.grfAccessPermissions == e2.grfAccessPermissions &&
		e1.grfAccessMode == e2.grfAccessMode &&
		e1.grfInheritance == e2.grfInheritance &&
		win32_trustees_equal(e1.Trustee, e2.Trustee);
}