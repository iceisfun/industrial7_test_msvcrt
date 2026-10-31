/* TC
 * id:    str_neg_strpbrk_nomatch
 * area:  string
 * funcs: strpbrk
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.24.5.4 (returns null pointer if no character from s2 occurs in s1)
 */
#include "tc.h"
#include <string.h>

static const char *volatile v_s, *volatile v_set;
static char *pb(const char *s, const char *set) { v_s = s; v_set = set; return strpbrk((const char *)v_s, (const char *)v_set); }

TC_CASE(disjoint) { CHECK(pb("abcdef", "xyz") == NULL); CHECK(pb("abc", "ABC") == NULL); }
TC_CASE(empty_haystack) { CHECK(pb("", "abc") == NULL); CHECK(pb("", "") == NULL); }
TC_CASE(terminator_not_matched) { CHECK(pb("abc", "") == NULL); CHECK(pb("", "") == NULL); }

int main(void) { tc_begin(); TC_RUN(disjoint); TC_RUN(empty_haystack); TC_RUN(terminator_not_matched); return tc_end(); }
