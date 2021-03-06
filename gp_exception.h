/** This file is part of dirCompare
 *
 * Copyright 2017-2020 Thomas Erbesdobler <t.erbesdobler@team103.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _GP_EXCEPTION_H
#define _GP_EXCEPTION_H

#include <exception>

#include <string>

class gp_exception : public std::exception
{
private:
	const std::string msg;

public:
	gp_exception(const std::string& msg) : msg(msg)
	{
	}

	virtual const char* what() const throw() override
	{
		return msg.c_str();
	}
};

#endif /* _GP_EXCEPTION_H */
