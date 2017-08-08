#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "platform.h"
#include "gp_exception.h"
#include "Token.h"
#include "Keyword.h"
#include "Value.h"
#include "SystemParameters.h"
#include "ComparisonStrategyFactory.h"
#include "FileComparisonStrategy.h"
#include "DirectoryComparisonStrategy.h"
#include "Commandline.h"

using namespace std;

Commandline::Commandline(int argc, char** argv)
{
	for (int i = 0; i < argc; i++)
	{
			string arg(argv[i]);
			
		if (arg.length() > 0)
		{
			while (arg[0] == ' ' || arg[0] == '\t')
			{
				arg.erase(arg.begin());
			}

			while (arg.back() == ' ' || arg.back() == '\t')
			{
				arg.pop_back();
			}

			addToken(arg);
		}
	}
}

void Commandline::addToken(const string& str)
{
	if (str.length() >= 2 && str.substr(0, 2) == "--")
	{
		tokens.push_back(Keyword::create(str.substr(2, str.length() - 2)));
	}
	else
	{
		tokens.push_back(Value::create(str));
	}
}

void Commandline::parse(shared_ptr<SystemParameters> param) const
{
	auto i = tokens.cbegin();

	auto csf = createComparisonStrategyFactory(param);

	string logpath;

	while (i != tokens.cend())
	{
		auto t = *i++;
		auto k = dynamic_pointer_cast<Keyword>(t);

		if (k != nullptr)
		{
			auto v = i != tokens.cend() ? dynamic_pointer_cast<Value>(*i) : nullptr;

			if (k->getStr() == "dir1")
			{
				if (!param->getDirectory1().empty())
				{
					throw gp_exception("dir1 duplicated");
				}

				if (v == nullptr)
				{
					throw gp_exception("dir1: need to specify path");
				}

				i++;

				param->setDirectory1(v->getStr());
			}
			else if (k->getStr() == "dir2")
			{
				if (!param->getDirectory2().empty())
				{
					throw gp_exception("dir2 duplicated");
				}

				if (v == nullptr)
				{
					throw gp_exception("dir2: need to specify path");
				}
				
				i++;

				param->setDirectory2(v->getStr());
			}
			else if (k->getStr() == "listStrategies")
			{
				param->setListStrategies(true);
			}
			else if (k->getStr() == "fileStrategy")
			{
				if (!param->getFileComparisonStrategy().empty())
				{
					throw gp_exception("fileStrategy duplicated");
				}

				if (v == nullptr)
				{
					throw gp_exception("fileStrategy: need to specify name");
				}

				i++;

				auto strategies = csf->createFileStrategies();
				bool found = false;
				
				for (auto i : strategies)
				{
					if (i->getID() == v->getStr())
					{
						param->setFileComparisonStrategy(i->getID());
						found = true;
						break;
					}
				}

				if (!found)
				{
					throw gp_exception("unknown file comparison strategy: " +
						v->getStr());
				}
			}
			else if (k->getStr() == "dirStrategy")
			{
				if (!param->getDirectoryComparisonStrategy().empty())
				{
					throw gp_exception("dirStrategy duplicated");
				}

				if (v == nullptr)
				{
					throw gp_exception("dirStrategy: need to specify name");
				}

				i++;

				auto strategies = csf->createDirStrategies();
				bool found = false;
				
				for (auto i : strategies)
				{
					if (i->getID() == v->getStr())
					{
						param->setDirectoryComparisonStrategy(i->getID());
						found = true;
						break;
					}
				}

				if (!found)
				{
					throw gp_exception("unknown directory comparison strategy: " +
						v->getStr());
				}
			}
			else if (k->getStr() == "logfile")
			{
				if (param->isExternalLogSet() || !logpath.empty())
				{
					throw gp_exception("logfile duplicated");
				}

				if (v == nullptr)
				{
					throw gp_exception("logfile: need to specify path");
				}

				i++;

				logpath = v->getStr();
			}
			else
			{
				throw gp_exception("unknown parameter: " + k->getStr());
			}
		}
		else
		{
			throw gp_exception("invalid token: " + t->getStr());
		}
	}

	if (!logpath.empty() && !param->getListStrategies())
	{
		auto log = make_shared<ofstream>(
			logpath,
			ios_base::out | ios_base::trunc);

		if (!log->good())
		{
			throw gp_exception("logfile stream ist not in good state. "
				"logfile: " + logpath);
		}

		param->setLog(log);
	}
}
