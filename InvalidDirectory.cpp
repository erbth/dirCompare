#include <memory>
#include <string>
#include <vector>
#include "InvalidDirectory.h"
#include "SystemParameters.h"
#include "InvalidItem.h"
#include "Directory.h"
#include "gp_exception.h"

using namespace std;

InvalidDirectory::InvalidDirectory(
	const string& path,
	shared_ptr<SystemParameters> sp)
	: Directory(path, sp)
{
}

InvalidDirectory::InvalidDirectory(
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: Directory(path, sp, dir)
{
}

const string InvalidDirectory::getErrorDescription() const
{
	return "Unable to open directory \"" + path + "\": " +
		getErrorMessage();
}

vector<shared_ptr<Item>> InvalidDirectory::getItems() const
{
	throw gp_exception("Calling getItems() on InvalidDirectory is invalid.");
}
