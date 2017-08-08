#include <vector>
#include <memory>
#include "SystemParameters.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "LinuxComparisonFactory.h"
#include "LinuxSimpleFileComparison.h"
#include "LinuxSimpleDirectoryComparison.h"

using namespace std;

LinuxComparisonFactory::LinuxComparisonFactory(shared_ptr<SystemParameters> sp)
	: ComparisonStrategyFactory(sp)
{
}

vector<shared_ptr<FileComparisonStrategy>>
	LinuxComparisonFactory::createFileStrategies()
{
	vector<shared_ptr<FileComparisonStrategy>> v;

	v.push_back(make_shared<LinuxSimpleFileComparison>(sp));

	return v;
}

vector<shared_ptr<DirectoryComparisonStrategy>>
	LinuxComparisonFactory::createDirStrategies()
{
	vector<shared_ptr<DirectoryComparisonStrategy>> v;

	v.push_back(make_shared<LinuxSimpleDirectoryComparison>(sp));

	return v;
}
