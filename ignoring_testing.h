#ifndef _IGNORING_TESTING_H
#define _IGNORING_TESTING_H

#ifdef WITH_TESTING

#include <string>
#include "ignoring.h"

bool ignoring_match_test(const string& cpptext, const string& cpppattern);

#endif /* WITH_TESTING */

#endif /* _IGNORING_TESTING_H */
