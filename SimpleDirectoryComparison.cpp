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

#include <string>
#include <vector>
#include <memory>
#include "SystemParameters.h"
#include "gp_exception.h"
#include "log.h"
#include "Item.h"
#include "Directory.h"
#include "InvalidDirectory.h"
#include "SimpleDirectoryComparison.h"
#include "ComparisonContext.h"
#include "ignoring.h"

SimpleDirectoryComparison::SimpleDirectoryComparison(
	shared_ptr<SystemParameters> sp)
	: DirectoryComparisonStrategy(sp)
{
}

bool SimpleDirectoryComparison::compare(
	shared_ptr<const Directory> d1,
	shared_ptr<const Directory> d2,
	string* reason) const
{
	if (!comparisonContext)
	{
		throw gp_exception("comparison context not set");
	}

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
		bool have_p1 = it1 != itemsD1.cend();
		bool have_p2 = it2 != itemsD2.cend();

		if (!have_p1 && !have_p2)
			return equal;
		
		string name_p1 = have_p1 ? (*it1)->getName() : string();
		string name_p2 = have_p2 ? (*it2)->getName() : string();

		if (have_p1 && ignoreItem(*it1, sp))
		{
			it1++;
			continue;
		}

		if (have_p2 && ignoreItem(*it2, sp))
		{
			it2++;
			continue;
		}

		if (have_p1 && have_p2 && name_p1 == name_p2)
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

			if (!have_p2 || have_p1 && name_p1.compare(name_p2) < 0)
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

				*(sp->getLog()) << type << "\"" << name_p1 << "\" not in directory 2" << endl;

				it1++;
			}
			else if (!have_p1 || have_p2 && name_p1.compare(name_p2) > 0)
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

				*(sp->getLog()) << type << "\"" << name_p2 << "\" not in directory 1" << endl;

				it2++;
			}
		}
	}
}

const string SimpleDirectoryComparison::getID() const
{
	return "simple";
}

const string SimpleDirectoryComparison::getDescription() const
{
	return "simple platfrom independent recursive directory comparison, "
		"compares only directory content (recursively)";
}
