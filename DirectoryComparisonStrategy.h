#ifndef _DIRECTORY_COMPARISON_STRATEGY_H
#define _DIRECTORY_COMPARISON_STRATEGY_H

#include <string>
#include <memory>
#include "SystemParameters.h"
#include "Directory.h"

class ComparisonContext;

class DirectoryComparisonStrategy :
	public enable_shared_from_this<DirectoryComparisonStrategy>
{
protected:
	shared_ptr<SystemParameters> sp;
	const ComparisonContext* comparisonContext = nullptr;

public:
	DirectoryComparisonStrategy(shared_ptr<SystemParameters> sp);

	const ComparisonContext *getComparisonContext() const;
	void setComparisonContext(ComparisonContext *c);

	virtual bool compare(
		shared_ptr<const Directory> d1,
		shared_ptr<const Directory> d2) const = 0;

	virtual const string getID() const = 0;
	virtual const string getDescription() const = 0;
};

#endif /* _DIRECTORY_COMPARISON_STRATEGY_H */
