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

#include <memory>
#include <string>
#include "SystemParameters.h"
#include "gp_exception.h"
#include "Item.h"
#include "File.h"
#include "Directory.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "log.h"
#include "ComparisonContext.h"

ComparisonContext::ComparisonContext(shared_ptr<SystemParameters> sp)
{
	fileStrategy = nullptr;
	dirStrategy = nullptr;
	this->sp = sp;
}

shared_ptr<FileComparisonStrategy>
	ComparisonContext::getFileComparisonStrategy() const
{
	return fileStrategy;
}

void ComparisonContext::setFileComparisonStrategy(
	shared_ptr<FileComparisonStrategy> s)
{
	fileStrategy = s;
}

shared_ptr<DirectoryComparisonStrategy>
	ComparisonContext::getDirectoryComparisonStrategy() const
{
	return dirStrategy;
}

void ComparisonContext::setDirectoryComparisonStrategy(
	shared_ptr<DirectoryComparisonStrategy> s)
{
	if (dirStrategy != s)
	{
		dirStrategy = s;
		s->setComparisonContext(this);
	}
}

bool ComparisonContext::compare(
	shared_ptr<const Item> i1,
	shared_ptr<const Item> i2) const
{
	if (i1->getLevel() == i2->getLevel())
	{
		auto f1 = dynamic_pointer_cast<const File>(i1);
		auto f2 = dynamic_pointer_cast<const File>(i2);

		if (f1 != nullptr && f2 != nullptr)
		{
			if (!fileStrategy)
			{
				throw gp_exception("file comparison strategy not set!");
			}

			string reason;
			bool equal = fileStrategy->compare(f1, f2, &reason);

			if (!equal)
			{
				logIndentation(sp->getLog(), f1);

				*(sp->getLog()) << "files " << f1->getPath() << " and "
					<< f2->getPath() << " differ" << (reason.empty() ? "" :
					" (reason: " + reason + ")") << endl;
			}

			return equal;
		}

		auto d1 = dynamic_pointer_cast<const Directory>(i1);
		auto d2 = dynamic_pointer_cast<const Directory>(i2);

		if (d1 != nullptr && d2 != nullptr)
		{
			if (!dirStrategy)
			{
				throw gp_exception("directory comparison strategy not set!");
			}

			string reason;
			bool equal = dirStrategy->compare(d1, d2, &reason);

			if (!equal)
			{
				 logIndentation(sp->getLog(), d1);

				*(sp->getLog()) << "directories " << d1->getPath() << " and "
					<< d2->getPath() << " differ" << (reason.empty() ? "" :
					" (reason: " + reason + ")") << endl;
			}

			return equal;
		}
	}

	throw gp_exception("items " + i1->getPath() + " and " +
		i2->getPath() + " are not comparable");

	return false;
}
