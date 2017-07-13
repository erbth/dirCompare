#include <string>
#include "Item.h"

using namespace std;

Item::Item(string& name)
{
	this->name.assign(name);
}

Item::Item(const char* name)
{
	this->name.assign(name);
}

const string& Item::getName()
{
	return name;
}

void Item::setName(const string &name)
{
	this->name.assign(name);
}

ostream& operator<<(ostream& o, const Item& i) { return i.dump(o); }
