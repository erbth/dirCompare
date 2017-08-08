#include <memory>
#include "SystemParameters.h"
#include "LinuxFileComparisonStrategy.h"

LinuxFileComparisonStrategy::LinuxFileComparisonStrategy(
	shared_ptr<SystemParameters> sp)
	: FileComparisonStrategy(sp)
{
}
