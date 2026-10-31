/* TC
 * id:    str_strpbrk_basic
 * area:  string
 * funcs: strpbrk
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.5.4; MS docs strpbrk
 */
#include "tc.h"
#include <string.h>

static const char *volatile v_s, *volatile v_set;
static char *pb(const char *s, const char *set) { v_s = s; v_set = set; return strpbrk((const char *)v_s, (const char *)v_set); }

TC_CASE(first_match) { static char s[] = "hello, world"; char *r = pb(s, "ow"); CHECK_EQ_P(r, s + 4); }
TC_CASE(match_at_start) { static char s[] = "abc"; CHECK_EQ_P(pb(s, "cba"), s); }
TC_CASE(set_order) { static char s[] = "xxyz"; CHECK_EQ_P(pb(s, "zy"), s + 2); CHECK_EQ_P(pb(s, "yz"), s + 2); }
TC_CASE(empty_set) { static char s[] = "abc"; CHECK(pb(s, "") == NULL); }
TC_CASE(high_bit) { static char s[] = "ab\xe9\xff"; CHECK_EQ_P(pb(s, "\xff\xe9"), s + 2); CHECK_EQ_P(pb(s, "\xff"), s + 3); }

int main(void) { tc_begin(); TC_RUN(first_match); TC_RUN(match_at_start); TC_RUN(set_order); TC_RUN(empty_set); TC_RUN(high_bit); return tc_end(); }
