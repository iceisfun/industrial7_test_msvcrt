/* TC
 * id:    pfr_vsnprintf_c99
 * area:  stdio_printf
 * funcs: vsnprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.12; MS docs vsnprintf
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int w(char *b, size_t c, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = vsnprintf(b, c, f, ap); va_end(ap); return r; }

TC_CASE(fit) { char b[32]; volatile size_t c = 32; int n = w(b, c, "%s-%d", "ab", 7); CHECK_EQ_I(n, 4); CHECK_STR(b, "ab-7"); }
TC_CASE(trunc) { char b[16]; volatile size_t c = 4; int n; memset(b, 'Z', sizeof b); n = w(b, c, "%s", "abcdefgh"); CHECK_EQ_I(n, 8); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z'); }
TC_CASE(null_buf) { char *volatile nb = NULL; int n = w(nb, 0, "%s%d", "abc", 12); CHECK_EQ_I(n, 5); }
TC_CASE(zero_count) { char b[8]; int n; memset(b, 'Z', sizeof b); n = w(b, 0, "%s", "abcdef"); CHECK_EQ_I(n, 6); CHECK_EQ_I(b[0], 'Z'); CHECK_EQ_I(b[7], 'Z'); }

int main(void) { tc_begin(); TC_RUN(fit); TC_RUN(trunc); TC_RUN(null_buf); TC_RUN(zero_count); return tc_end(); }
