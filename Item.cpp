#include <iostream>
#include <memory>
#include <string>
#include "Item.h"
#include "Directory.h"

using namespace std;

Item::Item(const string& path)
{
	this->path.assign(path);
	directory = nullptr;
}

Item::Item(const string& path, shared_ptr<const Directory> dir)
{
	this->path = path;
	directory = dir;
}

Item::~Item()
{
}

const string& Item::getPath()
{
	return path;
}

ostream& operator<<(ostream& o, const Item *i) { return i->dump(o); }
