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

#include "Win32Item.h"
#include "win32_security_tools.h"
#include "win32_error_exception.h"

extern "C"
{
#include <Windows.h>
#include <AclAPI.h>
#include <AccCtrl.h>
}

using namespace std;

bool Win32Item::compare_all_attributes_to(const Win32Item* other, string &reason) const
{
	if (getType() != other->getType())
	{
		reason = "different type";
		return false;
	}

	if (getSize() != other->getSize())
	{
		reason = "different size";
		return false;
	}

	if (getCreationTime().QuadPart != other->getCreationTime().QuadPart)
	{
		reason = "different creation time";
		return false;
	}
	
	if (getLastAccessTime().QuadPart != other->getLastAccessTime().QuadPart)
	{
		reason = "different last access time";
		return false;
	}

	if (getLastWriteTime().QuadPart != other->getLastWriteTime().QuadPart)
	{
		reason = "different last write time";
		return false;
	}

	if (getChangeTime().QuadPart != other->getChangeTime().QuadPart)
	{
		reason = "different change time";
		return false;
	}

	if (getAttributes() != other->getAttributes())
	{
		reason = "different attributes";
		return false;
	}

	if (!EqualSid(getOwner(), other->getOwner()))
	{
		reason = "different owners";
		return false;
	}

	if (!EqualSid(getGroup(), other->getGroup()))
	{
		reason = "different primary group";
		return false;
	}

	if (isDaclProtected() != other->isDaclProtected())
	{
		reason = "different DACL protection state (ACL inheritance)";
		return false;
	}

	/* Compare DACLs */
	if (getDacl() == nullptr && other->getDacl() != nullptr || getDacl() != nullptr && other->getDacl() != nullptr)
	{
		reason = "only one DACL is a NULL ACL";
		return false;
	}

	PEXPLICIT_ACCESS peACEs1, peACEs2;
	ULONG ceACEs1, ceACEs2;

	auto ret = GetExplicitEntriesFromAcl(getDacl(), &ceACEs1, &peACEs1);
	if (ret != ERROR_SUCCESS)
		throw win32_error_exception(ret, L"GetExplicitEntriesFromAcl failed: ");

	ret = GetExplicitEntriesFromAcl(other->getDacl(), &ceACEs2, &peACEs2);
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

			for (ULONG j = 0; j < ceACEs1; i++)
			{
				if (win32_explicit_aces_equal(peACEs1[i], peACEs2[i]))
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

			for (ULONG j = 0; j < ceACEs1; i++)
			{
				if (win32_explicit_aces_equal(peACEs2[i], peACEs1[i]))
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

	/* All attributes seem to be equal. */
	return true;
}