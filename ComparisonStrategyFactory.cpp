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
#include <vector>
#include "gp_exception.h"
#include "SystemParameters.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "ComparisonStrategyFactory.h"

ComparisonStrategyFactory::ComparisonStrategyFactory(
	shared_ptr<SystemParameters> sp)
{
	this->sp = sp;
}

shared_ptr<FileComparisonStrategy>
	ComparisonStrategyFactory::createFileStrategy(const string id)
{
	for (auto s : createFileStrategies())
	{
		if (s->getID() == id)
		{
			return s;
		}
	}

	throw gp_exception("invalid file comparison strategy: " + id);
}

shared_ptr<DirectoryComparisonStrategy>
	ComparisonStrategyFactory::createDirStrategy(const string id)
{
	for (auto s : createDirStrategies())
	{
		if (s->getID() == id)
		{
			return s;
		}
	}

	throw gp_exception("invalid directory comparison strategy: " + id);
}
