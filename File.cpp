#include <memory>
#include <string>
#include "File.h"
#include "Directory.h"
#include "Item.h"

using namespace std;

File::File(const string& path) : Item(path)
{
}

File::File(const string& path, shared_ptr<const Directory> dir) : Item(path, dir)
{
}

File::~File()
{
}
