#include <memory>
#include <string>
#include "SystemParameters.h"
#include "File.h"
#include "FileComparisonStrategy.h"
#include "StubFileComparisonStrategy.h"

StubFileComparisonStrategy::StubFileComparisonStrategy(
	shared_ptr<SystemParameters> sp)
	: FileComparisonStrategy(sp)
{
}

bool StubFileComparisonStrategy::compare(
	shared_ptr<const File> f1,
	shared_ptr<const File> f2,
	string* reason) const noexcept
{
	return true;
}

const string StubFileComparisonStrategy::getID() const
{
	return "StubFileComparisonStrategy";
}

const string StubFileComparisonStrategy::getDescription() const
{
	return "Stub File Comparison Strategy";
}