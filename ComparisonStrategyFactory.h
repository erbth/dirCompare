#ifndef _COMPARISON_STRATEGY_FACTORY_H
#define _COMPARISON_STRATEGY_FACTORY_H

#include <vector>
#include <memory>
#include <string>
#include "SystemParameters.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"

using namespace std;

class ComparisonStrategyFactory
{
protected:
	shared_ptr<SystemParameters> sp;

public:
	ComparisonStrategyFactory(shared_ptr<SystemParameters> sp);

	virtual vector<shared_ptr<FileComparisonStrategy>>
		createFileStrategies() = 0;

	virtual vector<shared_ptr<DirectoryComparisonStrategy>>
		createDirStrategies() = 0;

	shared_ptr<FileComparisonStrategy> createFileStrategy(const string id);
	shared_ptr<DirectoryComparisonStrategy> createDirStrategy(const string id);
};

#endif /* _COMPARISON_STRATEGY_FACTORY_H */
