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
	string fileComparisonStrategy;
	string directoryComparisonStrategy;
	shared_ptr<ostream> log;
	bool externalLog;

public:
	SystemParameters();

	string getDirectory1() const;
	string getDirectory2() const;
	bool getListStrategies() const;
	const string getFileComparisonStrategy() const;
	const string getDirectoryComparisonStrategy() const;
	shared_ptr<ostream> getLog() const;

	void setDirectory1(string dir);
	void setDirectory2(string dir);
	void setListStrategies(bool l);
	void setFileComparisonStrategy(const string s);
	void setDirectoryComparisonStrategy(const string s);
	void setLog(shared_ptr<ostream> l);

	bool isValid(ostream& errors) const;
	bool isExternalLogSet() const;
};

#endif /* _SYSTEM_PARAMETERS_H */
