#include <iostream>
#include <memory>
#include "Item.h"
#include "LinuxItemFactory.h"
#include "LinuxFile.h"

int main()
{
	ItemFactory *f = new LinuxItemFactory();

	Item *f1 = f->createFile("Readme");
	cout << f1 << endl;

	LinuxFile *fl = dynamic_cast<LinuxFile*>(f1);
	fl->getFileInfo().print_summary(cout);

	delete f1;
	delete f;
}
