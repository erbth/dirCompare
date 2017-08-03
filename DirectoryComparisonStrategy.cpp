#include <memory>
#include "ComparisonContext.h"
#include "DirectoryComparisonStrategy.h"

const ComparisonContext* DirectoryComparisonStrategy::
	getComparisonContext() const
{
	return comparisonContext;
}

void DirectoryComparisonStrategy::setComparisonContext(ComparisonContext *c)
{
	if (comparisonContext != c)
	{
		comparisonContext = c;
		c->setDirectoryComparisonStrategy(shared_from_this());
	}
}
