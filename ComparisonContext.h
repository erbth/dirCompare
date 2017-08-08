#ifndef _COMPARISON_CONTEXT_H
#define _COMPARISON_CONTEXT_H

#include <memory>
#include "SystemParameters.h"
#include "Item.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"

using namespace std;

class ComparisonContext
{
private:
	shared_ptr<SystemParameters> sp;
	shared_ptr<FileComparisonStrategy> fileStrategy;
	shared_ptr<DirectoryComparisonStrategy> dirStrategy;

public:
	ComparisonContext(shared_ptr<SystemParameters> sp);

	shared_ptr<FileComparisonStrategy> getFileComparisonStrategy() const;
	void setFileComparisonStrategy(shared_ptr<FileComparisonStrategy> s);

	shared_ptr<DirectoryComparisonStrategy> getDirectoryComparisonStrategy() const;
	void setDirectoryComparisonStrategy(shared_ptr<DirectoryComparisonStrategy> s);

	bool compare(
		shared_ptr<const Item> i1,
		shared_ptr<const Item> i2) const;
};

#endif /* _COMPARISON_CONTEXT_H */
