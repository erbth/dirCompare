#include <ostream>
#include <memory>
#include <string>
#include "File.h"
#include "Directory.h"
#include "SystemParameters.h"
#include "StubFile.h"

StubFile::StubFile(
	const string& path, shared_ptr<SystemParameters> sp)
	: File(path, sp)
{
}

StubFile::StubFile(
	const string& path,
	shared_ptr<SystemParameters> sp,
	shared_ptr<const Directory> dir)
	: File(path, sp, dir)
{
}

StubFile::~StubFile()
{
}