#include <memory>
#include "SystemParameters.h"
#include "gp_exception.h"
#include "ItemFactory.h"
#include "ComparisonStrategyFactory.h"
#include "platform.h"

#if TARGET_PLATFORM == LINUX
#include "LinuxItemFactory.h"
#include "LinuxComparisonFactory.h"
#endif

shared_ptr<ItemFactory> createItemFactory(shared_ptr<SystemParameters> sp)
{
#if TARGET_PLATFORM == LINUX
	return make_shared<LinuxItemFactory>(sp);
#endif
}

shared_ptr<ComparisonStrategyFactory> createComparisonStrategyFactory(
	shared_ptr<SystemParameters> sp)
{
#if TARGET_PLATFORM == LINUX
	return make_shared<LinuxComparisonFactory>(sp);
#endif
}
