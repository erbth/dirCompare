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

	ItemFactory *f = new LinuxItemFactory();


	/* test file */
	Item *f1 = f->createFile("test1");
	cout << endl << f1 << endl;

	LinuxFile *fl1 = dynamic_cast<LinuxFile*>(f1);
	fl1->getFileInfo().print_summary(cout);
	cout << endl;

	Item *f2 = f->createFile("test2");
	cout << endl << f2 << endl;

	LinuxFile *fl2 = dynamic_cast<LinuxFile*>(f2);
	fl2->getFileInfo().print_summary(cout);
	cout << endl;

	cout << cc.compare(shared_ptr<Item>(f1), shared_ptr<Item>(f2)) << endl << endl;


	/* test directory */
	cout << endl;
	Item *d1 = f->createDirectory("testDir");
	cout << endl << d1 << endl;

	LinuxDirectory *ld1 = dynamic_cast<LinuxDirectory*>(d1);
	ld1->getFileInfo().print_summary(cout);
	cout << endl;

	delete d1;
	delete f2;
	delete f1;
	delete f;
	delete csf;
}
