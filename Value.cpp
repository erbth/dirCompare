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

#include <memory>
#include <string>
#include "Token.h"
#include "Value.h"
#include "Keyword.h"

Value::Value(string s) : Token(s)
{
}

shared_ptr<Value> Value::create(string s)
{
	return shared_ptr<Value>(new Value(s));
}

shared_ptr<Keyword> Value::getKeyword() const
{
	return keyword;
}

void Value::setKeyword(shared_ptr<Keyword> k)
{
	if (keyword != k)
	{
		keyword = k;
		k->setValue(static_pointer_cast<Value>(shared_from_this()));
	}
}
