/* TC
 * id:    pfv_neg_underscore_snprintf_truncate
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf: if len > count, count bytes stored and a negative value returned
 * note:  Does not assert a NUL byte after truncation (documented as not terminated).
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

TC_CASE(ret_minus1) { char b[16]; volatile int n; memset(b, 'x', sizeof b); n = _snprintf(b, 4, "%s", "abcdefgh"); CHECK(n < 0); }
TC_CASE(count_chars_written) { char b[16]; volatile int n; memset(b, 'x', sizeof b); n = _snprintf(b, 4, "%s", "abcdefgh"); CHECK(n < 0); CHECK_MEM(b, "abcd", 4); }
TC_CASE(no_write_past_count) { char b[16]; int i; volatile int n; memset(b, 'x', sizeof b); n = _snprintf(b, 4, "%s", "abcdefgh"); CHECK(n < 0);
    for (i = 4; i < 16; i++) CHECK_EQ_I(b[i], 'x'); }
TC_CASE(count_zero_buf_untouched) { char b[8]; int i; volatile int n; memset(b, 'x', sizeof b); n = _snprintf(b, 0, "%s", "abc"); CHECK(n < 0);
    for (i = 0; i < 8; i++) CHECK_EQ_I(b[i], 'x'); }

int main(void) { tc_begin(); TC_RUN(ret_minus1); TC_RUN(count_chars_written); TC_RUN(no_write_past_count); TC_RUN(count_zero_buf_untouched); return tc_end(); }
