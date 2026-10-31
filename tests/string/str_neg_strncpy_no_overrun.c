/* TC
 * id:    str_neg_strncpy_no_overrun
 * area:  string
 * funcs: strncpy
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.24.2.4 (at most n chars written; no NUL added when len>=n)
 */
#include "tc.h"
#include <string.h>

static char buf[64];
static void fill(void) { memset(buf, 'G', sizeof buf); }

TC_CASE(guard_intact) { volatile const char *s = "0123456789abcdef"; size_t i; fill(); strncpy(buf + 8, (const char *)s, 4); for (i = 0; i < 8; i++) CHECK_EQ_I(buf[i], 'G'); for (i = 12; i < 64; i++) CHECK_EQ_I(buf[i], 'G'); CHECK_MEM(buf + 8, "0123", 4); }
TC_CASE(not_terminated_len_ge_n) { volatile const char *s = "abcd"; fill(); strncpy(buf, (const char *)s, 4); CHECK_EQ_I(buf[4], 'G'); fill(); strncpy(buf, (const char *)s, 2); CHECK_EQ_I(buf[2], 'G'); }
TC_CASE(n_zero_untouched) { volatile const char *s = "abcd"; size_t i; fill(); strncpy(buf, (const char *)s, 0); for (i = 0; i < 64; i++) CHECK_EQ_I(buf[i], 'G'); }
TC_CASE(padding_stops_at_n) { volatile const char *s = "ab"; size_t i; fill(); strncpy(buf + 4, (const char *)s, 20); CHECK_MEM(buf + 4, "ab", 2); for (i = 6; i < 24; i++) CHECK_EQ_I(buf[4 + i - 4], 0); CHECK_EQ_I(buf[24], 'G'); CHECK_EQ_I(buf[3], 'G'); }

int main(void) { tc_begin(); TC_RUN(guard_intact); TC_RUN(not_terminated_len_ge_n); TC_RUN(n_zero_untouched); TC_RUN(padding_stops_at_n); return tc_end(); }
