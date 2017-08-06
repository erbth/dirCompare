#include <memory>
#include <string>
#include <ostream>
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "SystemParameters.h"

using namespace std;

SystemParameters::SystemParameters() :
	listStrategies(false),
	fileComparisonStrategy(nullptr),
	directoryComparisonStrategy(nullptr),
	log(&cout, [](ostream*){})
{
}

string SystemParameters::getDirectory1() const
{
	return directory1;
}

string SystemParameters::getDirectory2() const
{
	return directory2;
}

bool SystemParameters::getListStrategies() const
{
	return listStrategies;
}

shared_ptr<FileComparisonStrategy>
	SystemParameters::getFileComparisonStrategy() const
{
	return fileComparisonStrategy;
}

shared_ptr<DirectoryComparisonStrategy>
	SystemParameters::getDirectoryComparisonStrategy() const
{
	return directoryComparisonStrategy;
}

shared_ptr<ostream> SystemParameters::getLog() const
{
	return log;
}

void SystemParameters::setDirectory1(string dir)
{
	directory1 = dir;
}

void SystemParameters::setDirectory2(string dir)
{
	directory2 = dir;
}

void SystemParameters::setListStrategies(bool l)
{
	listStrategies = l;
}

void SystemParameters::setFileComparisonStrategy(
	shared_ptr<FileComparisonStrategy> s)
{
	fileComparisonStrategy = s;
}

void SystemParameters::setDirectoryComparisonStrategy(
	shared_ptr<DirectoryComparisonStrategy> s)
{
	directoryComparisonStrategy = s;
}

void SystemParameters::setLog(shared_ptr<ostream> l)
{
	log = l;
}

bool SystemParameters::isValid(ostream& errors) const
{
	if (listStrategies)
	{
		return true;
	}
	else
	{
		if (directory1.empty() || directory2.empty())
		{
			return false;
		}

		if (fileComparisonStrategy == nullptr ||
			directoryComparisonStrategy == nullptr)
		{
			return false;
		}

		if (log == nullptr)
		{
			return false;
		}

		return true;
	}
}

