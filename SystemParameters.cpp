#include <memory>
#include <string>
#include <ostream>
#include "gp_exception.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "SystemParameters.h"

using namespace std;

SystemParameters::SystemParameters() :
	listStrategies(false),
	log(&cout, [](ostream*){}),
	externalLog(false)
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

const string SystemParameters::getFileComparisonStrategy() const
{
	return fileComparisonStrategy;
}

const string SystemParameters::getDirectoryComparisonStrategy() const
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

void SystemParameters::setFileComparisonStrategy(const string s)
{
	fileComparisonStrategy = s;
}

void SystemParameters::setDirectoryComparisonStrategy(const string s)
{
	directoryComparisonStrategy = s;
}

void SystemParameters::setLog(shared_ptr<ostream> l)
{
	log = l;
	externalLog = true;
}

bool SystemParameters::isValid(ostream& errors) const
{
	if (listStrategies)
	{
		return true;
	}
	else
	{
		if (directory1.empty())
		{
			errors << "mandatory: directory 1's path not given" << endl;
			return false;
		}
			
		if (directory2.empty())
		{
			errors << "mandatory: directory 2's path not given" << endl;
			return false;
		}

		if (fileComparisonStrategy.empty())
		{
			errors << "mandatory: no file comparison strategy given" << endl;
			return false;
		}
		
		if (directoryComparisonStrategy.empty())
		{
			errors << "mandatory: no directory comparison strategy given" << endl;
			return false;
		}

		if (log == nullptr)
		{
			throw gp_exception("SystemParameters.log is nullptr");
		}

		return true;
	}
}

bool SystemParameters::isExternalLogSet() const
{
	return externalLog;
}

