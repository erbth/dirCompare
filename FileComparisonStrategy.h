#ifndef _FILE_COMPARISON_STRATEGY_H
#define _FILE_COMPARISON_STRATEGY_H

#include <string>
#include <memory>
#include "SystemParameters.h"
#include "File.h"

class FileComparisonStrategy
{
protected:
	shared_ptr<SystemParameters> sp;

public:
	FileComparisonStrategy(shared_ptr<SystemParameters> sp);

	virtual bool compare(
		shared_ptr<const File> f1,
		shared_ptr<const File> f2,
		string* reason = nullptr) const noexcept = 0;

	virtual const string getID() const = 0;
	virtual const string getDescription() const = 0;
};

#endif /* _FILE_COMPARISON_STRATEGY_H */
