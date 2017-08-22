#ifndef _LOG_H
#define _LOG_H

#include <ostream>
#include <memory>
#include "Item.h"

/* Helper functions for log output */
void logIndentation(shared_ptr<ostream> o, shared_ptr<const Item> item);

#endif /* _LOG_H */
