#include <string>
#include <memory>
#include "SystemParameters.h"
#include "File.h"
#include "LinuxFile.h"
#include "LinuxSimpleFileComparison.h"

using namespace std;

LinuxSimpleFileComparison::LinuxSimpleFileComparison(
	shared_ptr<SystemParameters> sp)
	: LinuxFileComparisonStrategy(sp)
{
}

bool LinuxSimpleFileComparison::compare(
	shared_ptr<const File> f1,
	shared_ptr<const File> f2,
	string* reason) const
{
	auto lf1 = dynamic_pointer_cast<const LinuxFile>(f1);
	auto lf2 = dynamic_pointer_cast<const LinuxFile>(f2);

	if (lf1 != nullptr && lf2 != nullptr)
	{
		LinuxFileInfo info2 = lf2->getFileInfo();

		if (lf1->getFileInfo().equivalent_to(info2, reason))
		{
			return true;
		}
	}

	return false;
}

const string LinuxSimpleFileComparison::getID() const
{
	return "simple";
}

const string LinuxSimpleFileComparison::getDescription() const
{
	return "simple file comparison on Linux, compares only file parameters";
}
