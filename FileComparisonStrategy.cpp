#include <memory>
#include "SystemParameters.h"
#include "FileComparisonStrategy.h"

FileComparisonStrategy::FileComparisonStrategy(shared_ptr<SystemParameters> sp)
{
	this->sp = sp;
}
