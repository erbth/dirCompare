#ifndef _SIMPLE_DIRECTORY_COMPARISON_H
#define _SIMPLE_DIRECTORY_COMPARISON_H

#include <string>
#include <memory>
#include "SystemParameters.h"
#include "Directory.h"
#include "DirectoryComparisonStrategy.h"

class SimpleDirectoryComparison : public DirectoryComparisonStrategy
{
public:
	SimpleDirectoryComparison(shared_ptr<SystemParameters> sp);

	virtual bool compare(
		shared_ptr<const Directory> d1,
		shared_ptr<const Directory> d2,
		string* reason = nullptr) const override;

	virtual const string getID() const override;
	virtual const string getDescription() const override;
};

#endif /* _SIMPLE_DIRECTORY_COMPARISON_H */
