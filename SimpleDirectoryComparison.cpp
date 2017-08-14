#include <string>
#include <vector>
#include <memory>
#include "SystemParameters.h"
#include "gp_exception.h"
#include "Item.h"
#include "Directory.h"
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

	auto itemsD1 = d1->getItems();
	auto itemsD2 = d2->getItems();

	auto it1 = itemsD1.cbegin();
	auto it2 = itemsD2.cbegin();

	bool equal = true;

	for (;;)
	{
		if (it1 == itemsD1.end() && it2 == itemsD2.end())
		{
			return equal;
		}
		
		string p1 = it1 != itemsD1.end() ? (*it1)->getPath() : string();
		string p2 = it2 != itemsD2.end() ? (*it2)->getPath() : string();

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

			if (p2.empty() || p1.compare(p2) < 0)
			{
				for (int i = 0; i < (*it1)->getLevel(); i++)
				{
					*(sp->getLog()) << "  ";
				}

				string type;

				if (dynamic_pointer_cast<File>(*it1) == *it1)
				{
					type = "file ";
				}
				else if (dynamic_pointer_cast<Directory>(*it1) == *it1)
				{
					type = "directory ";
				}

				*(sp->getLog()) << type << p1 << " not in " << d2->getPath() << endl;

				it1++;
			}
			else if (p1.empty() || p1.compare(p2) > 0)
			{
				for (int i = 0; i < (*it2)->getLevel(); i++)
				{
					*(sp->getLog()) << "  ";
				}

				string type;

				if (dynamic_pointer_cast<File>(*it1) == *it1)
				{
					type = "file ";
				}
				else if (dynamic_pointer_cast<Directory>(*it1) == *it1)
				{
					type = "directory ";
				}

				*(sp->getLog()) << type << p2 << " not in " << d1->getPath() << endl;

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
