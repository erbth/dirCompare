#include <ostream>
#include <memory>
#include "Item.h"
#include "log.h"

/* Helper functions for log output */
void logIndentation(shared_ptr<ostream> o, shared_ptr<const Item> item)
{
	for (int i = 0; i < item->getLevel(); i++)
	{
		*o << "  ";
	}
}
