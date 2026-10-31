/* TC
 * id:    str_neg_strstr_notfound
 * area:  string
 * funcs: strstr
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.24.5.7; MS docs strstr
 */
#include "tc.h"
#include <string.h>

static const char *volatile v_h, *volatile v_n;
static char *find(const char *h, const char *n) { v_h = h; v_n = n; return strstr((const char *)v_h, (const char *)v_n); }

TC_CASE(needle_longer) { CHECK(find("abc", "abcd") == NULL); }
TC_CASE(absent) { CHECK(find("hello world", "xyz") == NULL); }
TC_CASE(case_differs) { CHECK(find("Hello World", "world") == NULL); CHECK(find("abc", "ABC") == NULL); }
TC_CASE(empty_haystack) { CHECK(find("", "a") == NULL); }
TC_CASE(partial_prefix_at_end) { CHECK(find("abcabcab", "abcd") == NULL); CHECK(find("xxab", "abc") == NULL); }

int main(void) { tc_begin(); TC_RUN(needle_longer); TC_RUN(absent); TC_RUN(case_differs); TC_RUN(empty_haystack); TC_RUN(partial_prefix_at_end); return tc_end(); }
