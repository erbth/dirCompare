#include <memory>
#include <string>
#include "gp_exception.h"
#include "Item.h"
#include "File.h"
#include "Directory.h"
#include "FileComparisonStrategy.h"
#include "ComparisonContext.h"

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

const string ComparisonContext::compare(Item *i1, Item *i2) const
{
	File *f1 = dynamic_cast<File*>(i1);
	File *f2 = dynamic_cast<File*>(i2);

	if (f1 && f2)
	{
		if (!fileStrategy)
		{
			throw gp_exception("file comparison strategy not set!");
		}

		if (fileStrategy->compare(f1, f2))
		{
			return string("match");
		}
		else
		{
			return string("differ");
		}
	}

	return string("not comparable");
}
