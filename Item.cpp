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
	directory = nullptr;
}

Item::Item(
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
{
	this->path = path;
	this->sp = sp;
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
