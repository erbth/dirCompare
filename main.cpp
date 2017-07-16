#include <iostream>
#include "Item.h"
#include "LinuxFactory.h"

int main()
{
	AbstractFactory *f = new LinuxFactory();

	string str("file #1");

	Item *i = f->createFile(str);
	Item *i2 = f->createDirectory("directory #1");

	cout << i << endl;
	cout << i2 << endl;

	delete i;
	delete i2;

	delete f;
}
