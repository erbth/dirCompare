#ifndef _STUB_FILE_COMPARISON_STRATEGY_H
#define _STUB_FILE_COMPARISON_STRATEGY_H

#include <memory>
#include <string>
#include "FileComparisonStrategy.h"
#include "File.h"
#include "SystemParameters.h"
#include "FileComparisonStrategy.h"

class StubFileComparisonStrategy : public FileComparisonStrategy
{
public:
	StubFileComparisonStrategy(shared_ptr<SystemParameters> sp);

	virtual bool compare(
		shared_ptr<const File> f1,
		shared_ptr<const File> f2,
		string* reason = nullptr) const noexcept override;

	virtual const string getID() const override;
	virtual const string getDescription() const override;
};

#endif /* _STUB_FILE_COMPARISON_STRATEGY_H */
