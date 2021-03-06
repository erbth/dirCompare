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

#include <ostream>
#include <memory>
#include <vector>
#include <string>
#include "dirCompareConfig.h"
#include "gp_exception.h"
#include "Item.h"
#include "File.h"
#include "Directory.h"
#include "SystemParameters.h"
#include "ignoring.h"

/* prototypes */
static int KMP(const char* t, int n, const char* s, int m);
static void computeBorders(int* border, int m, const char* s);

static bool match(const string& cpptext, const string& cpppattern)
{
	int m = cpppattern.length();
	int n = cpptext.length();
	const char* pattern = cpppattern.c_str();
	const char* text = cpptext.c_str();

	int cStars = 0;
	for (int i = 0; i < m; i++)

	{
		if (pattern[i] == '*')
		{
			cStars++;
		}
	}

	/* trivial case */
	if (m - cStars > n)
	{
		return false;
	}

	/* no star case */
	if (cStars == 0)
	{
		if (n == m)
		{
			for (int i = 0; i < m; i++)
			{
				if (pattern[i] != text[i])
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	int* starPositions = new int[cStars];
	int j = 0;

	for (int i = 0; i < m && j < cStars; i++)
	{
		if (pattern[i] == '*')
		{
			starPositions[j++] = i;
		}
	}

	/* offset in pattern */
	j = 0;

	/* alpha */
	if (starPositions[0] > 0)
	{
		int i;

		for (i = 0; i < starPositions[0]; i++)
		{
			if (pattern[i] != text[i])
			{
				delete[] starPositions;
				return false;
			}
		}

		j = i;
		text += i;
		m -= i;
		n -= i;
	}

	/* omega */
	if (starPositions[cStars - 1] - j < m - 1)
	{
		int i;

		for (i = 1; i <= m - (starPositions[cStars - 1] + 1 - j); i++)
		{
			if (pattern[m - i + j] != text[n - i])
			{
				delete[] starPositions;
				return false;
			}
		}

		m -= i - 1;
		n -= i - 1;
	}

	/* sigmata */
	for (int k = 0; k < cStars - 1; k++)
	{
		int subPatternLength = starPositions[k + 1] - starPositions[k] - 1;

		int i = KMP(
			text,
			n,
			pattern + starPositions[k] + 1,
			subPatternLength);

		if (i < 0)
		{
			delete[] starPositions;
			return false;
		}

		text += i + subPatternLength;
		n -= i + subPatternLength;
	}

	delete[] starPositions;
	return true;
}

#ifdef WITH_TESTING
bool ignoring_match_test(const string& cpptext, const string& cpppattern)
{
	return match(cpptext, cpppattern);
}
#endif

/* The following implementation of the Knuth-Morris-Pratt-algorithm
 * was taken from the lecture "Grundlagen Algorthmen und Datenstrukturen"
 * at Techische Universität München 2017, with some modifications to have
 * the epsilon pattern matching. */
static int KMP(const char* t, int n, const char* s, int m)
{
	int* border = new int[m + 1];
	computeBorders(border, m, s);
	int i = 0;
	int j = 0;

	if (m == 0)
	{
		delete[] border;
		return 0;
	}

	while (i <= n - m)
	{
		while (t[i + j] == s[j])
		{
			j++;
			if (j == m)
			{
				delete[] border;
				return i;
			}
		}
		
		i += j - border[j];
		j = border[j] > 0 ? border[j] : 0;
	}

	delete[] border;
	return -1;
}

static void computeBorders(int* border, int m, const char* s)
{
	border[0] = -1;
	border[1] = 0;
	int i = 0;

	for (int j = 2; j <= m; j++)
	{
		while (i >= 0 && s[i] != s[j - 1])
		{
			i = border[i];
		}

		i++;
		border[j] = i;
	}
}

bool ignoreItem(shared_ptr<Item> i, shared_ptr<SystemParameters> sp)
{
	auto f = dynamic_pointer_cast<File>(i);

	if (f != nullptr)
	{
		for (auto n : sp->getIgnoreFiles())
		{
			if (match(f->getPath(), n))
			{
				return true;
			}
		}
		
		return false;
	}

	auto d = dynamic_pointer_cast<Directory>(i);

	if (d != nullptr)
	{
		for (auto n : sp->getIgnoreDirectories())
		{
			if (match(d->getPath(), n))
			{
				return true;
			}
		}

		return false;
	}

	throw gp_exception("ignoreItem: invalid item");
}
