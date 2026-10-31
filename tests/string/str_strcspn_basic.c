/* TC
 * id:    str_strcspn_basic
 * area:  string
 * funcs: strcspn
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.5.3; MS docs strcspn
 */
#include "tc.h"
#include <string.h>

static const char *volatile v_s, *volatile v_set;
static size_t cs(const char *s, const char *set) { v_s = s; v_set = set; return strcspn((const char *)v_s, (const char *)v_set); }

TC_CASE(stops_at_first) { CHECK_EQ_U(cs("hello, world", " ,"), 5); }
TC_CASE(none_in_set) { CHECK_EQ_U(cs("hello", "xyz"), 5); }
TC_CASE(starts_in_set) { CHECK_EQ_U(cs("hello", "h"), 0); }
TC_CASE(empty_set) { CHECK_EQ_U(cs("hello", ""), 5); }
TC_CASE(empty_string) { CHECK_EQ_U(cs("", "abc"), 0); CHECK_EQ_U(cs("", ""), 0); }
TC_CASE(high_bit) { CHECK_EQ_U(cs("ab\xe9\xff" "cd", "\xff"), 3); CHECK_EQ_U(cs("ab\xe9\xff", "\xe9"), 2); CHECK_EQ_U(cs("ab\x80z", "\x81"), 4); }

int main(void) { tc_begin(); TC_RUN(stops_at_first); TC_RUN(none_in_set); TC_RUN(starts_in_set); TC_RUN(empty_set); TC_RUN(empty_string); TC_RUN(high_bit); return tc_end(); }
