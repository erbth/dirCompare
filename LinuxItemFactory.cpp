#include <memory>
#include <string>
#include <exception>
#include "SystemParameters.h"
#include "File.h"
#include "Directory.h"
#include "InvalidFile.h"
#include "InvalidDirectory.h"
#include "LinuxFile.h"
#include "LinuxDirectory.h"
#include "LinuxItemFactory.h"

template<class T>
shared_ptr<T> LinuxItemFactory::createInvalidItem(
	const string& path,
	shared_ptr<const Directory> parent,
	const string message) const
{
	shared_ptr<T> invalid;

	if (parent != nullptr)
	{
		invalid = make_shared<T>(path, sp, parent);
	}
	else
	{
		invalid = make_shared<T>(path, sp);
	}

	invalid->setErrorMessage(message);
	return invalid;
}

LinuxItemFactory::LinuxItemFactory(shared_ptr<SystemParameters> sp)
	: ItemFactory(sp)
{
}

shared_ptr<File> LinuxItemFactory::createFile(
	const string& path,
	shared_ptr<const Directory> parent) noexcept
{
	shared_ptr<File> f;
	
	try
	{
		if (parent != nullptr)
		{
			f = make_shared<LinuxFile>(path, sp, parent);
		}
		else
		{
			f = make_shared<LinuxFile>(path, sp);
		}
	}
	catch (exception& e)
	{
		f = createInvalidItem<InvalidFile>(path, parent, e.what());
	}
	catch (...)
	{
		f = createInvalidItem<InvalidFile>(path, parent, "unknown error");
	}

	return f;
}

shared_ptr<Directory> LinuxItemFactory::createDirectory(
	const string& path,
	shared_ptr<const Directory> parent) noexcept
{
	shared_ptr<Directory> d;
	
	try
	{
		if (parent != nullptr)
		{
			d = make_shared<LinuxDirectory>(path, sp, parent);
		}
		else
		{
			d = make_shared<LinuxDirectory>(path, sp);
		}
	}
	catch (exception& e)
	{
		d = createInvalidItem<InvalidDirectory>(path, parent, e.what());
	}
	catch (...)
	{
		d = createInvalidItem<InvalidDirectory>(path, parent, "unknown error");
	}

	return d;
}
