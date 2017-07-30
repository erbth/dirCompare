#ifndef _COMPARISON_CONTEXT_H
#define _COMPARISON_CONTEXT_H

#include <string>
#include <memory>
#include "Item.h"
#include "FileComparisonStrategy.h"

using namespace std;

class ComparisonContext
{
private:
	shared_ptr<FileComparisonStrategy> fileStrategy;

public:
	ComparisonContext() : fileStrategy(nullptr) {};

	shared_ptr<FileComparisonStrategy> getFileComparisonStrategy() const;
	void setFileComparisonStrategy(shared_ptr<FileComparisonStrategy> s);

	const string compare(Item *i1, Item *i2) const;
};

#endif /* _COMPARISON_CONTEXT_H */
