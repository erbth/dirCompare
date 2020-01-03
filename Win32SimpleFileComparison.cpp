/** This file is part of dirCompare
 *
 * Copyright 2020 Thomas Erbesdobler <t.erbesdobler@team103.com>
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
#include "InvalidFile.h"
#include "Win32File.h"
#include "Win32SimpleFileComparison.h"

using namespace std;

Win32SimpleFileComparison::Win32SimpleFileComparison(
	shared_ptr<SystemParameters> sp)
	: FileComparisonStrategy(sp)
{
}

bool Win32SimpleFileComparison::compare(
	shared_ptr<const File> f1,
	shared_ptr<const File> f2,
	string* reason) const noexcept
{
	if (dynamic_pointer_cast<const InvalidFile>(f1) == nullptr &&
		dynamic_pointer_cast<const InvalidFile>(f2) == nullptr)
	{
		auto wf1 = dynamic_pointer_cast<const Win32File>(f1);
		auto wf2 = dynamic_pointer_cast<const Win32File>(f2);

		try
		{
			if (wf1->compare_all_attributes_to(wf2.get(), *reason))
				return true;
		}
		catch (exception& e)
		{
			logIndentation(sp->getLog(), wf1);

			*(sp->getLog()) << "Error while comparing files \"" <<
				wf1->getPath() << "\", \"" << wf2->getPath() <<
				"\": " << e.what() << endl;
		}
		catch (...)
		{
			logIndentation(sp->getLog(), wf1);

			*(sp->getLog()) << "Unknown error while comparing files \"" <<
				wf1->getPath() << "\", \"" << wf2->getPath() <<
				"\"" << endl;
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

const string Win32SimpleFileComparison::getID() const
{
	return "simple";
}

const string Win32SimpleFileComparison::getDescription() const
{
	return "simple file comparison on the Win32 platform (all Windows except UWP), compares only file parameters";
}
