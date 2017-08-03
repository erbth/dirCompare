#ifndef _DIRECTORY_COMPARISON_STRATEGY_H
#define _DIRECTORY_COMPARISON_STRATEGY_H

#include <string>
#include <memory>
#include "Directory.h"

class ComparisonContext;

class DirectoryComparisonStrategy :
	enable_shared_from_this<DirectoryComparisonStrategy>
{
protected:
	const ComparisonContext* comparisonContext = nullptr;

public:
	const ComparisonContext *getComparisonContext() const;
	void setComparisonContext(ComparisonContext *c);

	virtual bool compare(
		shared_ptr<const Directory> d1,
		shared_ptr<const Directory> d2) const = 0;

	virtual const string getID() const = 0;
};

#endif /* _DIRECTORY_COMPARISON_STRATEGY_H */
