#include <string>
#include <vector>
#include <memory>
#include "gp_exception.h"
#include "Item.h"
#include "Directory.h"
#include "LinuxSimpleDirectoryComparison.h"
#include "ComparisonContext.h"

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

	for (;;)
	{
		if (it1 == itemsD1.end() && it2 == itemsD2.end())
		{
			return true;
		}
		
		if (it1 == itemsD1.end() || it2 == itemsD2.end())
		{
			return false;
		}

		if (!comparisonContext->compare(*it1, *it2))
		{
			return false;
		}

		it1++;
		it2++;
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
