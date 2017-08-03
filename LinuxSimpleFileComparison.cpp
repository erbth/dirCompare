#include <string>
#include <memory>
#include "File.h"
#include "LinuxFile.h"
#include "LinuxSimpleFileComparison.h"

using namespace std;

bool LinuxSimpleFileComparison::compare(
	shared_ptr<const File> f1,
	shared_ptr<const File> f2) const
{
	auto lf1 = dynamic_pointer_cast<const LinuxFile>(f1);
	auto lf2 = dynamic_pointer_cast<const LinuxFile>(f2);

	if (lf1 != nullptr && lf2 != nullptr)
	{
		LinuxFileInfo info2 = lf2->getFileInfo();

		if (lf1->getFileInfo().equivalent_to(info2))
		{
			return true;
		}
	}

	return false;
}

const string LinuxSimpleFileComparison::getID() const
{
	string v("simple file comparison on Linux, compares only file parameters");
	return v;
}
