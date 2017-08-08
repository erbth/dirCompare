#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "SystemParameters.h"
#include "Directory.h"
#include "Item.h"

using namespace std;

Directory::Directory(const string& path, shared_ptr<SystemParameters> sp)
	: Item(path, sp)
{
}

Directory::Directory(
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: Item(path, sp, dir)
{
}

Directory::~Directory()
{
}
