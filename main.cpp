#include <iostream>
#include <vector>
#include <memory>
#include "Item.h"
#include "LinuxItemFactory.h"
#include "LinuxFile.h"
#include "LinuxDirectory.h"
#include "ComparisonContext.h"
#include "ComparisonStrategyFactory.h"
#include "LinuxComparisonFactory.h"

int main()
{
	ComparisonContext cc;
	ComparisonStrategyFactory *csf = new LinuxComparisonFactory();

	auto fileStrategies = csf->createFileStrategies();

	cout << "offered file comparison strategies:" << endl;

	for (auto s : fileStrategies)
	{
		cout << s->getID() << endl;
	}

	cc.setFileComparisonStrategy(fileStrategies[0]);

	cout << endl;

	auto dirStrategies = csf->createDirStrategies();

	cout << "offered directory comparison strategies:" << endl;

	for (auto s : dirStrategies)
	{
		cout << s->getID() << endl;
	}

	cc.setDirectoryComparisonStrategy(dirStrategies[0]);

	cout << endl;

	ItemFactory *f = new LinuxItemFactory();


	/* test file */
	/* auto f1 = f->createFile("test1");
	cout << endl << f1 << endl;

	auto fl1 = dynamic_pointer_cast<LinuxFile>(f1);
	fl1->getFileInfo().print_summary(cout);
	cout << endl;

	auto f2 = f->createFile("test2");
	cout << endl << f2 << endl;

	auto fl2 = dynamic_pointer_cast<LinuxFile>(f2);
	fl2->getFileInfo().print_summary(cout);
	cout << endl;

	cout << cc.compare(f1, f2) << endl << endl; */


	/* test directory */
	cout << endl;
	auto d1 = f->createDirectory("testDir1");
	/* cout << endl << d1 << endl; */

	/* auto ld1 = dynamic_pointer_cast<LinuxDirectory>(d1);
	ld1->getFileInfo().print_summary(cout);
	cout << endl; */

	auto d2 = f->createDirectory("testDir2");

	cout << cc.compare(d1, d2) << endl;

	delete f;
	delete csf;
}
