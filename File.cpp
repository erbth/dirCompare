#include <memory>
#include <string>
#include "SystemParameters.h"
#include "File.h"
#include "Directory.h"
#include "Item.h"

using namespace std;

File::File(const string& path, shared_ptr<SystemParameters> sp)
	: Item(path, sp)
{
}

File::File(
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: Item(path, sp, dir)
{
}

File::~File()
{
}
