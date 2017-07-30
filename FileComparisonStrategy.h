#ifndef _FILE_COMPARISON_STRATEGY_H
#define _FILE_COMPARISON_STRATEGY_H

#include <string>
#include <vector>
#include "File.h"

class FileComparisonStrategy
{
public:
	virtual bool compare(const File *f1, const File *f2) = 0;
	virtual const string getID() = 0;
};

#endif /* _FILE_COMPARISON_STRATEGY_H */
