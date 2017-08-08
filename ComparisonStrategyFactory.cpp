#include <memory>
#include "SystemParameters.h"
#include "ComparisonStrategyFactory.h"

ComparisonStrategyFactory::ComparisonStrategyFactory(
	shared_ptr<SystemParameters> sp)
{
	this->sp = sp;
}
