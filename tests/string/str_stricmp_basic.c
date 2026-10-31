/* TC
 * id:    str_stricmp_basic
 * area:  string
 * funcs: _stricmp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _stricmp, _strnicmp (convert to lowercase before comparing)
 */
#include "tc.h"
#include <string.h>

#define SGN(x) ((x) < 0 ? -1 : (x) > 0 ? 1 : 0)
static volatile const char *va, *vb;
static int icmp(const char *a, const char *b) { va = a; vb = b; return SGN(_stricmp((const char *)va, (const char *)vb)); }

TC_CASE(case_equal) { CHECK_EQ_I(icmp("Hello", "hELLO"), 0); CHECK_EQ_I(icmp("ABC", "abc"), 0); }
TC_CASE(a_lt_B) { CHECK_EQ_I(icmp("a", "B"), -1); CHECK_EQ_I(icmp("B", "a"), 1); CHECK_EQ_I(icmp("A", "b"), -1); }
TC_CASE(underscore_lt_letters) { CHECK_EQ_I(icmp("_", "a"), -1); CHECK_EQ_I(icmp("_", "A"), -1); CHECK_EQ_I(icmp("a", "_"), 1); CHECK_EQ_I(icmp("A", "_"), 1); }
TC_CASE(empty) { CHECK_EQ_I(icmp("", ""), 0); CHECK_EQ_I(icmp("", "a"), -1); }
TC_CASE(prefix_less) { CHECK_EQ_I(icmp("abc", "ABCD"), -1); CHECK_EQ_I(icmp("ABCD", "abc"), 1); }

int main(void) { tc_begin(); TC_RUN(case_equal); TC_RUN(a_lt_B); TC_RUN(underscore_lt_letters); TC_RUN(empty); TC_RUN(prefix_less); return tc_end(); }
