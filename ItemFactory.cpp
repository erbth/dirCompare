#include <memory>
#include "SystemParameters.h"
#include "ItemFactory.h"

ItemFactory::ItemFactory(shared_ptr<SystemParameters> sp)
{
	this->sp = sp;
}
