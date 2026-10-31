/* TC
 * id:    pfr_snprintf_c99_trunc
 * area:  stdio_printf
 * funcs: snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.5; MS docs snprintf
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(count_eq_len) { char b[16]; volatile size_t c = 5; int n; memset(b, 'Z', sizeof b); n = snprintf(b, c, "%s", "hello"); CHECK_EQ_I(n, 5); CHECK_STR(b, "hell"); CHECK_EQ_I(b[4], 0); CHECK_EQ_I(b[5], 'Z'); }
TC_CASE(count4) { char b[16]; volatile size_t c = 4; int n; memset(b, 'Z', sizeof b); n = snprintf(b, c, "%s", "abcdefgh"); CHECK_EQ_I(n, 8); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z'); }
TC_CASE(count1) { char b[16]; volatile size_t c = 1; int n; memset(b, 'Z', sizeof b); n = snprintf(b, c, "%s", "abcdefgh"); CHECK_EQ_I(n, 8); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[1], 'Z'); }
TC_CASE(sentinels) { char b[16]; volatile size_t c = 3; int n, i; memset(b, 'Z', sizeof b); n = snprintf(b, c, "%d", 123456); CHECK_EQ_I(n, 6); CHECK_STR(b, "12"); for (i = 3; i < 16; i++) CHECK_EQ_I(b[i], 'Z'); }

int main(void) { tc_begin(); TC_RUN(count_eq_len); TC_RUN(count4); TC_RUN(count1); TC_RUN(sentinels); return tc_end(); }
