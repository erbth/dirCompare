#include <vector>
#include <memory>
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "LinuxComparisonFactory.h"
#include "LinuxSimpleFileComparison.h"
#include "LinuxSimpleDirectoryComparison.h"

using namespace std;

vector<shared_ptr<FileComparisonStrategy>>
	LinuxComparisonFactory::createFileStrategies()
{
	vector<shared_ptr<FileComparisonStrategy>> v;

	v.push_back(make_shared<LinuxSimpleFileComparison>());

	return v;
}

vector<shared_ptr<DirectoryComparisonStrategy>>
	LinuxComparisonFactory::createDirStrategies()
{
	vector<shared_ptr<DirectoryComparisonStrategy>> v;

	v.push_back(make_shared<LinuxSimpleDirectoryComparison>());

	return v;
}
