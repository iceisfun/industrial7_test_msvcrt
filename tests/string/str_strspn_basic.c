/* TC
 * id:    str_strspn_basic
 * area:  string
 * funcs: strspn
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.5.6; MS docs strspn
 */
#include "tc.h"
#include <string.h>

static const char *volatile v_s, *volatile v_set;
static size_t sp(const char *s, const char *set) { v_s = s; v_set = set; return strspn((const char *)v_s, (const char *)v_set); }

TC_CASE(prefix) { CHECK_EQ_U(sp("cabbage", "abc"), 5); CHECK_EQ_U(sp("12345abc", "0123456789"), 5); }
TC_CASE(none) { CHECK_EQ_U(sp("hello", "xyz"), 0); }
TC_CASE(whole) { CHECK_EQ_U(sp("abcabc", "cba"), 6); }
TC_CASE(empty_string) { CHECK_EQ_U(sp("", "abc"), 0); }
TC_CASE(empty_set) { CHECK_EQ_U(sp("abc", ""), 0); }

int main(void) { tc_begin(); TC_RUN(prefix); TC_RUN(none); TC_RUN(whole); TC_RUN(empty_string); TC_RUN(empty_set); return tc_end(); }
