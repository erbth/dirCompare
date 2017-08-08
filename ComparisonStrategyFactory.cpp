#include <memory>
#include <string>
#include <vector>
#include "gp_exception.h"
#include "SystemParameters.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "ComparisonStrategyFactory.h"

ComparisonStrategyFactory::ComparisonStrategyFactory(
	shared_ptr<SystemParameters> sp)
{
	this->sp = sp;
}

shared_ptr<FileComparisonStrategy>
	ComparisonStrategyFactory::createFileStrategy(const string id)
{
	for (auto s : createFileStrategies())
	{
		if (s->getID() == id)
		{
			return s;
		}
	}

	throw gp_exception("invalid file comparison strategy: " + id);
}

shared_ptr<DirectoryComparisonStrategy>
	ComparisonStrategyFactory::createDirStrategy(const string id)
{
	for (auto s : createDirStrategies())
	{
		if (s->getID() == id)
		{
			return s;
		}
	}

	throw gp_exception("invalid directory comparison strategy: " + id);
}
