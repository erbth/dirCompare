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

#include <string>
#include <vector>
#include <memory>
#include "SystemParameters.h"
#include "gp_exception.h"
#include "log.h"
#include "Item.h"
#include "Directory.h"
#include "Win32Directory.h"
#include "InvalidDirectory.h"
#include "Win32FullDirectoryComparison.h"
#include "ComparisonContext.h"
#include "ignoring.h"

Win32FullDirectoryComparison::Win32FullDirectoryComparison(
	shared_ptr<SystemParameters> sp)
	: DirectoryComparisonStrategy(sp)
{
}

bool Win32FullDirectoryComparison::compare(
	shared_ptr<const Directory> d1,
	shared_ptr<const Directory> d2,
	string* reason) const
{
	if (!comparisonContext)
	{
		throw gp_exception("comparison context not set");
	}

	/* If one of the directories is invalid, we are done. */
	{
		shared_ptr<const InvalidDirectory> id1;
		
		if ((id1 = dynamic_pointer_cast<const InvalidDirectory>(d1)) != nullptr)
		{
			logIndentation(sp->getLog(), id1);

			*(sp->getLog()) << "Unable to open directory \"" << id1->getPath()
				<< "\" in directory 1: " << id1->getErrorMessage() << endl;
		}

		shared_ptr<const InvalidDirectory> id2;
		
		if ((id2 = dynamic_pointer_cast<const InvalidDirectory>(d2)) != nullptr)
		{
			logIndentation(sp->getLog(), id2);

			*(sp->getLog()) << "Unable to open directory \"" << id2->getPath()
				<< "\" in directory 2: " << id2->getErrorMessage() << endl;
		}

		if (id1 || id2)
		{
			return false;
		}
	}

	/* Compare the directories' contents */
	vector<shared_ptr<Item>> itemsD1;
	vector<shared_ptr<Item>> itemsD2;
	
	try
	{
		itemsD1 = d1->getItems();
		itemsD2 = d2->getItems();
	}
	catch (exception& e)
	{
		logIndentation(sp->getLog(), d1);

		*(sp->getLog()) << "Error while retrieving items: " << e.what() << endl;
		return false;
	}
	catch (...)
	{
		logIndentation(sp->getLog(), d1);

		*(sp->getLog()) << "Unknown error while retrieving items." << endl;
		return false;
	}

	auto it1 = itemsD1.cbegin();
	auto it2 = itemsD2.cbegin();

	bool equal = true;

	for (;;)
	{
		if (it1 == itemsD1.end() && it2 == itemsD2.end())
			break;
		
		string p1 = it1 != itemsD1.end() ? (*it1)->getName() : string();
		string p2 = it2 != itemsD2.end() ? (*it2)->getName() : string();

		if (!p1.empty() && ignoreItem(*it1, sp))
		{
			it1++;
			continue;
		}

		if (!p2.empty() && ignoreItem(*it2, sp))
		{
			it2++;
			continue;
		}

		if (p1 == p2)
		{
			if (!comparisonContext->compare(*it1, *it2))
			{
				equal = false;
			}

			it1++;
			it2++;
		}
		else
		{
			equal = false;

			if (p2.empty() || !p1.empty() && p1.compare(p2) < 0)
			{
				logIndentation(sp->getLog(), *it1);

				string type;

				if (dynamic_pointer_cast<File>(*it1) == *it1)
				{
					type = "file ";
				}
				else if (dynamic_pointer_cast<Directory>(*it1) == *it1)
				{
					type = "directory ";
				}

				*(sp->getLog()) << type << "\"" << p1 << "\" not in directory 2" << endl;

				it1++;
			}
			else if (p1.empty() || !p2.empty() && p1.compare(p2) > 0)
			{
				logIndentation(sp->getLog(), *it2);

				string type;

				if (dynamic_pointer_cast<File>(*it2) == *it2)
				{
					type = "file ";
				}
				else if (dynamic_pointer_cast<Directory>(*it2) == *it2)
				{
					type = "directory ";
				}

				*(sp->getLog()) << type << "\"" << p2 << "\" not in directory 1" << endl;

				it2++;
			}
		}
	}

	if (!equal)
		*reason = "different content";

	/* Compare the directories' attributes */
	auto wd1 = dynamic_pointer_cast<const Win32Directory>(d1);
	auto wd2 = dynamic_pointer_cast<const Win32Directory>(d2);

	try
	{
		string attr_reason;

		if (!wd1->compare_all_attributes_to(wd2.get(), attr_reason))
		{
			*reason = equal ? attr_reason : (*reason + "; " + attr_reason);
			equal = false;
		}
	}
	catch (exception& e)
	{
		logIndentation(sp->getLog(), wd1);

		*(sp->getLog()) << "Error while comparing attributes of directories \"" <<
			wd1->getPath() << "\", \"" << wd2->getPath() <<
			"\": " << e.what() << endl;
	}
	catch (...)
	{
		logIndentation(sp->getLog(), wd1);

		*(sp->getLog()) << "Unknown error while comparing attributes of directories \"" <<
			wd1->getPath() << "\", \"" << wd2->getPath() <<
			"\"" << endl;
	}

	return equal;
}

const string Win32FullDirectoryComparison::getID() const
{
	return "full";
}

const string Win32FullDirectoryComparison::getDescription() const
{
	return "Recursive directory comparison strategy for win32, "
		"compares the directories content (recursively) and the their attributes in post-order traversal. "
		"That is contents are always compared first and then the directories' attributes.";
}
