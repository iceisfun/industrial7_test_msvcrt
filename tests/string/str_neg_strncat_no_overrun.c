/* TC
 * id:    str_neg_strncat_no_overrun
 * area:  string
 * funcs: strncat
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.24.3.2 (at most n chars plus a NUL appended; src NUL stops copy, no padding)
 */
#include "tc.h"
#include <string.h>

static char buf[64];
static void fill(void) { memset(buf, 'G', sizeof buf); strcpy(buf, "ab"); }

TC_CASE(guard_beyond_n_plus_1) { volatile const char *s = "0123456789"; size_t i; fill(); strncat(buf, (const char *)s, 4); CHECK_STR(buf, "ab0123"); CHECK_EQ_I(buf[7], 'G'); for (i = 7; i < 64; i++) CHECK_EQ_I(buf[i], 'G'); }
TC_CASE(huge_n_stops_at_src_nul) { volatile const char *s = "cd"; size_t i; fill(); strncat(buf, (const char *)s, (size_t)-1); CHECK_STR(buf, "abcd"); CHECK_EQ_I(buf[5], 'G'); for (i = 5; i < 64; i++) CHECK_EQ_I(buf[i], 'G'); }
TC_CASE(no_zero_padding) { volatile const char *s = "c"; size_t i; fill(); strncat(buf, (const char *)s, 20); CHECK_STR(buf, "abc"); for (i = 4; i < 24; i++) CHECK_EQ_I(buf[i], 'G'); }

int main(void) { tc_begin(); TC_RUN(guard_beyond_n_plus_1); TC_RUN(huge_n_stops_at_src_nul); TC_RUN(no_zero_padding); return tc_end(); }
