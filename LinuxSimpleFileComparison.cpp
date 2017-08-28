/** This file is part of dirCompare
 *
 * Copyright 2017 Thomas Erbesdobler <t.erbesdobler@team103.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>
#include <memory>
#include "SystemParameters.h"
#include "log.h"
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
				logIndentation(sp->getLog(), lf1);

				*(sp->getLog()) << "Error while comparing files \"" <<
					lf1->getPath() << "\", \"" << lf2->getPath() <<
					"\": " << e.what() << endl;
			}
			catch (...)
			{
				logIndentation(sp->getLog(), lf1);

				*(sp->getLog()) << "Unknown error while comparing files \"" <<
					lf1->getPath() << "\", \"" << lf2->getPath() <<
					"\"" << endl;
			}
		}
	}
	else
	{
		shared_ptr<const InvalidFile> if1;

		if ((if1 = dynamic_pointer_cast<const InvalidFile>(f1)) != nullptr)
		{
			logIndentation(sp->getLog(), if1);

			*(sp->getLog()) << "Unable to open file \"" << if1->getPath()
				<< "\" in directory 1: " << if1->getErrorMessage() << endl;
		}

		shared_ptr<const InvalidFile> if2;

		if ((if2 = dynamic_pointer_cast<const InvalidFile>(f2)) != nullptr)
		{
			logIndentation(sp->getLog(), if2);

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
