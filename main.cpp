#include <iostream>
#include <vector>
#include <memory>
#include <exception>
#include "platform.h"
#include "gp_exception.h"
#include "ItemFactory.h"
#include "ComparisonStrategyFactory.h"
#include "ComparisonContext.h"
#include "Commandline.h"
#include "SystemParameters.h"

int dirCompare(int argc, char** argv)
{
	Commandline cl(argc - 1, argv + 1);
	auto sp = make_shared<SystemParameters>();

	try
	{
		cl.parse(sp);
	}
	catch (gp_exception e)
	{
		cerr << e.what() << endl;
		return 1;
	}

	if (!sp->isValid(cerr))
	{
		cerr << endl << "invalid parameters, exiting." << endl;
		return 1;
	}

	cout << endl << "ignored files:" << endl;
	for (auto f : sp->getIgnoreFiles())
	{
		cout << f << endl;
	}

	cout << endl << "ignored directories:" << endl;
	for (auto d : sp->getIgnoreDirectories())
	{
		cout << d << endl;
	}

	cout << endl;

	if (sp->getListStrategies())
	{
		auto csf = createComparisonStrategyFactory(sp);

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

		return 0;
	}
	else
	{
		auto csf = createComparisonStrategyFactory(sp);

		auto fileStrategy = csf->createFileStrategy(
			sp->getFileComparisonStrategy());

		auto dirStrategy = csf->createDirStrategy(
			sp->getDirectoryComparisonStrategy());

		ComparisonContext cc(sp);
		cc.setFileComparisonStrategy(fileStrategy);
		cc.setDirectoryComparisonStrategy(dirStrategy);

		auto f = createItemFactory(sp);

		/* test directory */
		shared_ptr<Directory> d1;

		try
		{
			d1 = f->createDirectory(sp->getDirectory1());
		}
		catch (exception& e)
		{
			cerr << "unable to open directory 1:" << endl << e.what() << endl;
			return 1;
		}

		shared_ptr<Directory> d2;
		
		try
		{
			d2 = f->createDirectory(sp->getDirectory2());
		}
		catch (exception& e)
		{
			cerr << "unable to open directory 2:" << endl << e.what() << endl;
			return 1;
		}

		bool equal = cc.compare(d1, d2);
		*(sp->getLog()) << (equal ? "match" : "differ") << endl;

		return equal ? 0 : 1;
	}
}

int main(int argc, char** argv)
{
	int result = 1;

	try
	{
		result = dirCompare(argc, argv);
	}
	catch (exception& e)
	{
		cerr << "Caught an exception. That means the program was in an erroneous"
		" state so that further processing might have lead to failure. Therefore"
		" the program is terminated." << endl << "Error message: " << e.what()
		<< endl;

		result = 1;
	}
	catch (...)
	{
		cerr << "Caught an unknown exception. That means the program was in"
		" an unknown erroneous state so that further processing might have"
		" lead to failure. Therefore the program is terminated." << endl;

		result = 1;
	}

	return result;
}
