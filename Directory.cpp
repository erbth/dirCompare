#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Directory.h"
#include "Item.h"

using namespace std;

Directory::Directory(const string& path) : Item(path)
{
}

Directory::Directory(const string& path, shared_ptr<const Directory> dir)
 : Item(path, dir)
{
}

Directory::~Directory()
{
}
