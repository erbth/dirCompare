#ifndef _FILE_COMPARISON_STRATEGY_H
#define _FILE_COMPARISON_STRATEGY_H

#include <string>
#include <memory>
#include "File.h"

class FileComparisonStrategy
{
public:
	virtual bool compare(
		shared_ptr<const File> f1,
		shared_ptr<const File> f2) const = 0;

	virtual const string getID() const = 0;
};

#endif /* _FILE_COMPARISON_STRATEGY_H */
