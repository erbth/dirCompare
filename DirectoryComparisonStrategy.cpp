#include <memory>
#include "SystemParameters.h"
#include "ComparisonContext.h"
#include "DirectoryComparisonStrategy.h"

DirectoryComparisonStrategy::DirectoryComparisonStrategy(
	shared_ptr<SystemParameters> sp)
{
	this->sp = sp;
}

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
