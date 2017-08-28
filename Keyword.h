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

#ifndef _KEYWORD_H
#define _KEYWORD_H

#include <memory>
#include "Token.h"

using namespace std;

class Value;

class Keyword :	public Token
{
private:
	shared_ptr<Value> value;
	Keyword(string s);

public:
	static shared_ptr<Keyword> create(string s);

	shared_ptr<Value> getValue() const;
	void setValue(shared_ptr<Value> v);
};

#endif /* _KEYWORD_H */
