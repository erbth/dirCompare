#include <memory>
#include <string>
#include <vector>
#include "SystemParameters.h"
#include "Item.h"
#include "Directory.h"
#include "StubFile.h"
#include "StubDirectory.h"

StubDirectory::StubDirectory(
	const string& path,
	shared_ptr<SystemParameters> sp)
	: Directory(path, sp)
{
}

StubDirectory::StubDirectory(
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: Directory(path, sp, dir)
{
}

StubDirectory::~StubDirectory()
{
}

vector<shared_ptr<Item>> StubDirectory::getItems() const
{
	vector<shared_ptr<Item>> v;

	v.push_back(make_shared<StubFile>("file1", sp, shared_from_this()));
	v.push_back(make_shared<StubFile>("file2", sp, shared_from_this()));

	return v;
}