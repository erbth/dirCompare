#ifndef _SYSTEM_PARAMETERS_H
#define _SYSTEM_PARAMETERS_H

#include <iostream>
#include <string>
#include <memory>

using namespace std;

class FileComparisonStrategy;
class DirectoryComparisonStrategy;

class SystemParameters
{
protected:
	string directory1;
	string directory2;
	bool listStrategies;
	shared_ptr<FileComparisonStrategy> fileComparisonStrategy;
	shared_ptr<DirectoryComparisonStrategy> directoryComparisonStrategy;
	shared_ptr<ostream> log;
	bool externalLog;

public:
	SystemParameters();

	string getDirectory1() const;
	string getDirectory2() const;
	bool getListStrategies() const;
	shared_ptr<FileComparisonStrategy> getFileComparisonStrategy() const;
	shared_ptr<DirectoryComparisonStrategy> getDirectoryComparisonStrategy() const;
	shared_ptr<ostream> getLog() const;

	void setDirectory1(string dir);
	void setDirectory2(string dir);
	void setListStrategies(bool l);
	void setFileComparisonStrategy(shared_ptr<FileComparisonStrategy> s);
	void setDirectoryComparisonStrategy(shared_ptr<DirectoryComparisonStrategy> s);
	void setLog(shared_ptr<ostream> l);

	bool isValid(ostream& errors) const;
	bool isExternalLogSet() const;
};

#endif /* _SYSTEM_PARAMETERS_H */
