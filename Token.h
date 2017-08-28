/** This file is part of dirCompare
 *
 * Copyright 2017 Thomas Erbesdobler <t.erbesdobler@team103.com>
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

#ifndef _TOKEN_H
#define _TOKEN_H

#include <memory>
#include <string>

using namespace std;

class Token : public enable_shared_from_this<Token>
{
protected:
	string str;
	Token(string s);

public:
	/* trick to make this class polymorphic */
	virtual string getStr() const;
	void setStr(string s);
};

#endif /* _TOKEN_H */
