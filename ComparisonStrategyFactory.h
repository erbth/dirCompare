#ifndef _COMPARISON_STRATEGY_FACTORY_H
#define _COMPARISON_STRATEGY_FACTORY_H

#include <vector>
#include <memory>
#include "FileComparisonStrategy.h"

using namespace std;

class ComparisonStrategyFactory
{
public:
	virtual vector<shared_ptr<FileComparisonStrategy>>
		createFileStrategies() = 0;
};

#endif /* _COMPARISON_STRATEGY_FACTORY_H */
