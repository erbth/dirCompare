#include <memory>
#include <vector>
#include <string>
#include "gp_exception.h"
#include "Item.h"
#include "File.h"
#include "Directory.h"
#include "SystemParameters.h"
#include "ignoring.h"

static bool match(const string& s1, const string& s2)
{
	return s1 == s2;
}

bool ignoreItem(shared_ptr<Item> i, shared_ptr<SystemParameters> sp)
{
	auto f = dynamic_pointer_cast<File>(i);

	if (f != nullptr)
	{
		for (auto n : sp->getIgnoreFiles())
		{
			if (match(n, f->getPath()))
			{
				return true;
			}
		}
		
		return false;
	}

	auto d = dynamic_pointer_cast<Directory>(i);

	if (d != nullptr)
	{
		for (auto n : sp->getIgnoreDirectories())
		{
			if (match(n, d->getPath()))
			{
				return true;
			}
		}

		return false;
	}

	throw gp_exception("ignoreItem: invalid item");
}
