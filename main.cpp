#include <iostream>
#include <vector>
#include <memory>
#include "platform.h"
#include "ItemFactory.h"
#include "ComparisonStrategyFactory.h"
#include "ComparisonContext.h"
#include "Commandline.h"
#include "SystemParameters.h"

int main(int argc, char** argv)
{
	Commandline cl(argc - 1, argv + 1);
	SystemParameters sp;

	cl.parse(sp);

	if (!sp.isValid(cerr))
	{
		cerr << endl << "invalid parameters, exiting." << endl;
		return EXIT_SUCCESS;
	}

	if (sp.getListStrategies())
	{
		auto csf = createComparisonStrategyFactory();

		auto fileStrategies = csf->createFileStrategies();
		auto dirStrategies = csf->createDirStrategies();

		cout << "File comparison strategies: (id) description" << endl;
		cout << "-----------------------------------------------------------" << endl;

		for (auto s : fileStrategies)
		{
			cout << "(" << s->getID() << ") " << s->getDescription() << endl;
		}

		cout << endl << "Directory comparison strategies: (id) description" << endl;
		cout << "-----------------------------------------------------------" << endl;

		for (auto s : dirStrategies)
		{
			cout << "(" << s->getID() << ") " << s->getDescription() << endl;
		}
	}
	else
	{
	}	

	ComparisonContext cc;
	cc.setFileComparisonStrategy(sp.getFileComparisonStrategy());
	cc.setDirectoryComparisonStrategy(sp.getDirectoryComparisonStrategy());

	auto f = createItemFactory();

	/* test directory */
	auto d1 = f->createDirectory(sp.getDirectory1());
	auto d2 = f->createDirectory(sp.getDirectory2());

	cout << cc.compare(d1, d2) << endl;
}
