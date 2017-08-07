#ifndef _PLATFORM_H
#define _PLATFORM_H

#include <memory>
#include "ItemFactory.h"
#include "ComparisonStrategyFactory.h"

#ifndef TARGET_PLATFORM
#error TARGET_PLATFORM undefined
#else
#if TARGET_PLATFORM == 0
#error TARGET_PLATFORM invalid
#endif
#endif

/* symbolic constants for target platforms */
#define LINUX 1

/* methods for creating platform dependent factories */
shared_ptr<ItemFactory> createItemFactory();
shared_ptr<ComparisonStrategyFactory> createComparisonStrategyFactory();

#endif /* _PLATFORM_H */
