#include <string>
#include "File.h"
#include "LinuxFile.h"
#include "LinuxSimpleFileComparison.h"

using namespace std;

bool LinuxSimpleFileComparison::compare(const File *f1, const File *f2)
{
	const LinuxFile *lf1 = dynamic_cast<const LinuxFile*>(f1);
	const LinuxFile *lf2 = dynamic_cast<const LinuxFile*>(f2);

	if (lf1 && lf2)
	{
		LinuxFileInfo info2 = lf2->getFileInfo();

		if (lf1->getFileInfo().equivalent_to(info2))
		{
			return true;
		}
	}

	return false;
}

const string LinuxSimpleFileComparison::getID()
{
	string v("simple file comparison on Linux, compares only file parameters");
	return v;
}
