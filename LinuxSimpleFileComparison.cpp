#include <string>
#include <memory>
#include "SystemParameters.h"
#include "File.h"
#include "LinuxFile.h"
#include "InvalidFile.h"
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
	string* reason) const noexcept
{
	if (dynamic_pointer_cast<const InvalidFile>(f1) == nullptr &&
		dynamic_pointer_cast<const InvalidFile>(f2) == nullptr)
	{
		auto lf1 = dynamic_pointer_cast<const LinuxFile>(f1);
		auto lf2 = dynamic_pointer_cast<const LinuxFile>(f2);

		if (lf1 != nullptr && lf2 != nullptr)
		{
			try
			{
				LinuxFileInfo info2 = lf2->getFileInfo();

				if (lf1->getFileInfo().equivalent_to(info2, reason))
				{
					return true;
				}
			}
			catch (exception& e)
			{
				*(sp->getLog()) << "Error while comparing file \"" <<
					lf1->getPath() << "\": " << e.what() << endl;
			}
			catch (...)
			{
				*(sp->getLog()) << "Unknown error while comparing file \"" <<
					lf1->getPath() << endl;
			}
		}
	}
	else
	{
		shared_ptr<const InvalidFile> if1;

		if ((if1 = dynamic_pointer_cast<const InvalidFile>(f1)) != nullptr)
		{
			*(sp->getLog()) << "Unable to open file \"" << if1->getPath()
				<< "\" in directory 1: " << if1->getErrorMessage() << endl;
		}

		shared_ptr<const InvalidFile> if2;

		if ((if2 = dynamic_pointer_cast<const InvalidFile>(f2)) != nullptr)
		{
			*(sp->getLog()) << "Unable to open file \"" << if2->getPath()
				<< "\" in directory 2: " << if2->getErrorMessage() << endl;
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
