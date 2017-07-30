#include <iostream>
#include <string>
#include <vector>
#include "Directory.h"
#include "Item.h"

using namespace std;

Directory::Directory(const string& path) : Item(path)
{
}

Directory::~Directory()
{
	for (auto i : items)
	{
		delete i;
	}

	cout << "Directory " << path << " deleted." << endl;
}

vector<Item *> *Directory::getItems()
{
	return new vector<Item *>(items);
}
