#ifndef _IGNORING_H
#define _IGNORING_H

#include <string>
#include <memory>
#include "dirCompareConfig.h"
#include "Item.h"
#include "SystemParameters.h"

bool ignoreItem(shared_ptr<Item> i, shared_ptr<SystemParameters> sp);

#endif /* _IGNORING_H */
