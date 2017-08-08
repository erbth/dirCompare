#include <memory>
#include <string>
#include "SystemParameters.h"
#include "gp_exception.h"
#include "Item.h"
#include "File.h"
#include "Directory.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "ComparisonContext.h"

ComparisonContext::ComparisonContext(shared_ptr<SystemParameters> sp)
{
	fileStrategy = nullptr;
	dirStrategy = nullptr;
	this->sp = sp;
}

shared_ptr<FileComparisonStrategy>
	ComparisonContext::getFileComparisonStrategy() const
{
	return fileStrategy;
}

void ComparisonContext::setFileComparisonStrategy(
	shared_ptr<FileComparisonStrategy> s)
{
	fileStrategy = s;
}

shared_ptr<DirectoryComparisonStrategy>
	ComparisonContext::getDirectoryComparisonStrategy() const
{
	return dirStrategy;
}

void ComparisonContext::setDirectoryComparisonStrategy(
	shared_ptr<DirectoryComparisonStrategy> s)
{
	if (dirStrategy != s)
	{
		dirStrategy = s;
		s->setComparisonContext(this);
	}
}

bool ComparisonContext::compare(
	shared_ptr<const Item> i1,
	shared_ptr<const Item> i2) const
{
	if (i1->getLevel() == i2->getLevel())
	{
		auto f1 = dynamic_pointer_cast<const File>(i1);
		auto f2 = dynamic_pointer_cast<const File>(i2);

		if (f1 != nullptr && f2 != nullptr)
		{
			if (!fileStrategy)
			{
				throw gp_exception("file comparison strategy not set!");
			}

			bool equal = fileStrategy->compare(f1, f2);

			if (!equal)
			{
				for (int i = 0; i < f1->getLevel(); i++)
				{
					*(sp->getLog()) << "  ";
				}

				*(sp->getLog()) << "files " << f1->getPath() << " and "
					<< f2->getPath() << " differ" << endl;
			}

			return equal;
		}

		auto d1 = dynamic_pointer_cast<const Directory>(i1);
		auto d2 = dynamic_pointer_cast<const Directory>(i2);

		if (d1 != nullptr && d2 != nullptr)
		{
			if (!dirStrategy)
			{
				throw gp_exception("directory comparison strategy not set!");
			}

			bool equal = dirStrategy->compare(d1, d2);

			if (!equal)
			{
				for (int i = 0; i < d1->getLevel(); i++)
				{
					*(sp->getLog()) << "  ";
				}

				*(sp->getLog()) << "directories " << d1->getPath() << " and "
					<< d2->getPath() << " differ" << endl;
			}

			return equal;
		}
	}

	throw gp_exception("items " + i1->getPath() + " and " +
		i2->getPath() + " are not comparable");

	return false;
}
