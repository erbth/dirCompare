#include <memory>
#include "gp_exception.h"
#include "ItemFactory.h"
#include "ComparisonStrategyFactory.h"
#include "platform.h"

#if TARGET_PLATFORM == LINUX
#include "LinuxItemFactory.h"
#include "LinuxComparisonFactory.h"
#endif

shared_ptr<ItemFactory> createItemFactory()
{
#if TARGET_PLATFORM == LINUX
	return make_shared<LinuxItemFactory>();
#endif
}

shared_ptr<ComparisonStrategyFactory> createComparisonStrategyFactory()
{
#if TARGET_PLATFORM == LINUX
	return make_shared<LinuxComparisonFactory>();
#endif
}
