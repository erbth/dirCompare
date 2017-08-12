#include <memory>
#include <vector>
#include <string>
#include "gp_exception.h"
#include "Item.h"
#include "File.h"
#include "Directory.h"
#include "SystemParameters.h"
#include "ignoring.h"

/* prototypes */
static void computeBorders(int* border, int m, char* s);
static int KMP(char* t, int n, char* s, int m);

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

	int starPositions[cStars];
	int j = 0;

	for (int i = 0; i < m && j < cStars; i++)
	{
		if (pattern[i] == '*')
		{
			starPositions[j++] = i;
		}
	}

	/* alpha */
	if (starPositions[0] > 0)
	{
		for (j = 0; j < starPositions[0]; j++)
		{
			if (pattern[j] != text[j])
			{
				return false;
			}
		}

		pattern += j;
		text += j;
		m -= j;
		n -= j;
	}

	/* omega */
	if (starPositions[cStars - 1] < m - 1)
	{
		for (j = 1; j <= m - (starPositions[cStars - 1] + 1); j++)
		{
			if (pattern[m - j] != text[n - j])
			{
				return false;
			}
		}

		m -= j - 1;
		n -= j - 1;
	}

	/* sigmata */

	return true;
}

#ifdef WITH_TESTING
bool ignoring_match_test(const string& cpptext, const string& cpppattern)
{
	match(cpppattern, cpptext);
}
#endif

/* the following implementation of the Knuth-Morris-Pratt-algorithm
 * was taken from the lecture "Grundlagen Algorthmen und Datenstrukturen"
 * at Techische Universität München 2017 */
static int KMP(char* t, int n, char* s, int m)
{
	int border[m + 1];
	computeBorders(border, m, s);
	int i = 0;
	int j = 0;

	while (i <= n - m)
	{
		while (t[i + j] == s[j])
		{
			j++;
			if (j == m)
			{
				return i;
			}
		}
		
		i += j - border[j];
		j = border[j] > 0 ? border[j] : 0;
	}

	return -1;
}

static void computeBorders(int* border, int m, char* s)
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
