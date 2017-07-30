#include <vector>
#include <memory>
#include "FileComparisonStrategy.h"
#include "LinuxComparisonFactory.h"
#include "LinuxSimpleFileComparison.h"

using namespace std;

vector<shared_ptr<FileComparisonStrategy>>
	LinuxComparisonFactory::createFileStrategies()
{
	vector<shared_ptr<FileComparisonStrategy>> v;

	v.push_back(make_shared<LinuxSimpleFileComparison>());

	return v;
}
