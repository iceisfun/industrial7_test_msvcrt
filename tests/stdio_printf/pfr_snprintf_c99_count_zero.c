/* TC
 * id:    pfr_snprintf_c99_count_zero
 * area:  stdio_printf
 * funcs: snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.5 (n==0: nothing written, s may be null pointer)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

TC_CASE(null_buf) { volatile int v = 42; char *volatile nb = NULL; int n = snprintf(nb, 0, "hello %d", v); CHECK_EQ_I(n, 8); }
TC_CASE(untouched) { char b[8]; volatile size_t c = 0; int n; memset(b, 'Z', sizeof b); n = snprintf(b, c, "%s", "abcdef"); CHECK_EQ_I(n, 6); CHECK_EQ_I(b[0], 'Z'); CHECK_EQ_I(b[1], 'Z'); CHECK_EQ_I(b[7], 'Z'); }
TC_CASE(null_empty) { char *volatile nb = NULL; volatile const char *f = ""; int n = snprintf(nb, 0, (const char *)f); CHECK_EQ_I(n, 0); }

int main(void) { tc_begin(); TC_RUN(null_buf); TC_RUN(untouched); TC_RUN(null_empty); return tc_end(); }
