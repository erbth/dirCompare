#include <string>
#include "Item.h"

using namespace std;

Item::Item(const string& path)
{
	this->path.assign(path);
}

Item::Item(const char* path)
{
	this->path.assign(path);
}

Item::~Item()
{
}

const string& Item::getPath()
{
	return path;
}

void Item::setPath(const string &path)
{
	this->path.assign(path);
}

ostream& operator<<(ostream& o, const Item *i) { return i->dump(o); }
