#include <iostream>
#include <memory>
#include <string>
#include "SystemParameters.h"
#include "Item.h"
#include "Directory.h"

using namespace std;

Item::Item(const string& path, shared_ptr<SystemParameters> sp)
{
	this->path.assign(path);
	this->sp = sp;
	level = 0;
	directory = nullptr;
}

Item::Item(
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
{
	this->path = path;
	this->sp = sp;
	level = dir->getLevel() + 1;
	directory = dir;
}

Item::~Item()
{
}

const string& Item::getPath() const
{
	return path;
}

const int Item::getLevel() const
{
	return level;
}

ostream& operator<<(ostream& o, const Item *i) { return i->dump(o); }
