#include <string>
#include "File.h"
#include "Item.h"

using namespace std;

File::File(const string& path) : Item(path)
{
}

File::~File()
{
}
