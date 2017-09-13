#include <iostream>
#include <memory>
#include <string>
#include <exception>
#include "SystemParameters.h"
#include "Win32File.h"

using namespace std;

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
	auto sp = make_shared<SystemParameters>();

	shared_ptr<Win32File> wf;

	try
	{
		wf = make_shared<Win32File>(L"test", sp);
	}
	catch (exception& e)
	{
		cout << "Failed to open file: " << e.what() << endl;
		return 1;
	}
	catch (...)
	{
		cout << "Failed to open file, unknown reason." << endl;
		return 1;
	}

	cout << wf << endl;

	return 0;
}
