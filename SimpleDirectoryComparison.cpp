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

				*(sp->getLog()) << type << p1 << " not in directory 2" << endl;

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

				*(sp->getLog()) << type << p2 << " not in directory 1" << endl;

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
