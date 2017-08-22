#include <memory>
#include <string>
#include "InvalidFile.h"
#include "SystemParameters.h"
#include "InvalidItem.h"

using namespace std;

InvalidFile::InvalidFile(
	const string& path,
	shared_ptr<SystemParameters> sp)
	: File(path, sp)
{
}

InvalidFile::InvalidFile(
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: File(path, sp)
{
}

const string InvalidFile::getErrorDescription() const
{
	return "Unable to open file \"" + path + "\": " +
		getErrorMessage();
}
