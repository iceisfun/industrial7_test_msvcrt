/* TC
 * id:    str_strstr_basic
 * area:  string
 * funcs: strstr
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.5.7; MS docs strstr
 */
#include "tc.h"
#include <string.h>

static const char *h, *n;

TC_CASE(middle) { static const char s[] = "hello world"; h = s; n = "o w"; CHECK_EQ_P(strstr(h, n), s + 4); }
TC_CASE(start) { static const char s[] = "hello"; h = s; n = "hel"; CHECK_EQ_P(strstr(h, n), s); }
TC_CASE(end) { static const char s[] = "hello"; h = s; n = "llo"; CHECK_EQ_P(strstr(h, n), s + 2); }
TC_CASE(empty_needle) { static const char s[] = "hello"; h = s; n = ""; CHECK_EQ_P(strstr(h, n), s); }
TC_CASE(overlap) { static const char s[] = "aaaab"; h = s; n = "aaab"; CHECK_EQ_P(strstr(h, n), s + 1); }
TC_CASE(first_of_repeated) { static const char s[] = "abcabcabc"; h = s; n = "bc"; CHECK_EQ_P(strstr(h, n), s + 1); }
TC_CASE(not_found) { static const char s[] = "hello"; h = s; n = "helloo"; CHECK(strstr(h, n) == NULL); n = "x"; CHECK(strstr(h, n) == NULL); }
int main(void) { tc_begin(); TC_RUN(middle); TC_RUN(start); TC_RUN(end); TC_RUN(empty_needle); TC_RUN(overlap); TC_RUN(first_of_repeated); TC_RUN(not_found); return tc_end(); }
