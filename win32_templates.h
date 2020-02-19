/** This file is part of dirCompare
 *
 * Copyright 2017-2020 Thomas Erbesdobler <t.erbesdobler@team103.com>
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

#ifndef _WIN32_TEMPLATES_H
#define _WIN32_TEMPLATES_H

#include "win32_security_tools.h"
#include <string>

extern "C"
{
	#include <Windows.h>
	#include <AclAPI.h>
	#include <AccCtrl.h>
}

template<class T>
bool compare_all_win32_item_attributes(const T* a, const T* b, std::string& reason)
{
	if (a->getType() != b->getType())
	{
		reason = "different type";
		return false;
	}

	if (a->getSize() != b->getSize())
	{
		reason = "different size";
		return false;
	}

	if (a->isDaclProtected() != b->isDaclProtected())
	{
		reason = "different DACL protection state (ACL inheritance)";
		return false;
	}

	/* Compare DACLs */
	if (a->getDacl() == nullptr && b->getDacl() != nullptr || a->getDacl() != nullptr && b->getDacl() == nullptr)
	{
		reason = "only one DACL is a NULL ACL";
		return false;
	}

	PEXPLICIT_ACCESS peACEs1, peACEs2;
	ULONG ceACEs1, ceACEs2;

	auto ret = GetExplicitEntriesFromAcl(a->getDacl(), &ceACEs1, &peACEs1);
	if (ret != ERROR_SUCCESS)
		throw win32_error_exception(ret, L"GetExplicitEntriesFromAcl failed: ");

	ret = GetExplicitEntriesFromAcl(b->getDacl(), &ceACEs2, &peACEs2);
	if (ret != ERROR_SUCCESS)
	{
		LocalFree(peACEs1);
		throw win32_error_exception(ret, L"GetExplicitEntriesFromAcl failed: ");
	}

	auto different = false;

	if (ceACEs1 != ceACEs2)
	{
		reason = "different count of DACL entries";
		different = true;
	}

	if (!different)
	{
		for (ULONG i = 0; i < ceACEs1; i++)
		{
			bool found = false;

			for (ULONG j = 0; j < ceACEs1; j++)
			{
				if (win32_explicit_aces_equal(peACEs1[i], peACEs2[j]))
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				different = true;
				reason = "different DACL entries";
				break;
			}
		}
	}

	if (!different)
	{
		for (ULONG i = 0; i < ceACEs1; i++)
		{
			bool found = false;

			for (ULONG j = 0; j < ceACEs1; j++)
			{
				if (win32_explicit_aces_equal(peACEs2[i], peACEs1[j]))
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				different = true;
				reason = "different DACL entries";
				break;
			}
		}
	}

	LocalFree(peACEs2);
	LocalFree(peACEs1);

	if (different)
		return false;

	/* Owner, group */
	if (!EqualSid(a->getOwner(), b->getOwner()))
	{
		reason = "different owners";
		return false;
	}

	if (!EqualSid(a->getGroup(), b->getGroup()))
	{
		reason = "different primary group";
		return false;
	}

	/* Timestamps */
	if (a->getCreationTime().QuadPart != b->getCreationTime().QuadPart)
	{
		std::cout << a->getCreationTime().QuadPart << " != " << b->getCreationTime().QuadPart << endl;
		reason = "different creation time";
		return false;
	}

	if (a->getLastAccessTime().QuadPart != b->getLastAccessTime().QuadPart)
	{
		reason = "different last access time";
		return false;
	}

	if (a->getLastWriteTime().QuadPart != b->getLastWriteTime().QuadPart)
	{
		reason = "different last write time";
		return false;
	}

	if (a->getChangeTime().QuadPart != b->getChangeTime().QuadPart)
	{
		reason = "different change time";
		return false;
	}

	if (a->getAttributes() != b->getAttributes())
	{
		reason = "different attributes";
		return false;
	}

	/* All attributes seem to be equal. */
	return true;
}

#endif /* _WIN32_TEMPLATES_H */
