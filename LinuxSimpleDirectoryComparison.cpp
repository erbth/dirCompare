#include <string>
#include <vector>
#include <memory>
#include "SystemParameters.h"
#include "gp_exception.h"
#include "Item.h"
#include "Directory.h"
#include "LinuxSimpleDirectoryComparison.h"
#include "ComparisonContext.h"

LinuxSimpleDirectoryComparison::LinuxSimpleDirectoryComparison(
	shared_ptr<SystemParameters> sp)
	: LinuxDirectoryComparisonStrategy(sp)
{
}

bool LinuxSimpleDirectoryComparison::compare(
	shared_ptr<const Directory> d1,
	shared_ptr<const Directory> d2) const
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

				*(sp->getLog()) << p1 << " not in " << d2->getPath() << endl;

				it1++;
			}
			else if (p1.empty() || p1.compare(p2) > 0)
			{
				for (int i = 0; i < (*it2)->getLevel(); i++)
				{
					*(sp->getLog()) << "  ";
				}

				*(sp->getLog()) << p2 << " not in " << d1->getPath() << endl;

				it2++;
			}
		}
	}
}

const string LinuxSimpleDirectoryComparison::getID() const
{
	return "simple";
}

const string LinuxSimpleDirectoryComparison::getDescription() const
{
	return "simple recursive directory comparison on Linux, "
		"compares only directory content (recursively)";
}
