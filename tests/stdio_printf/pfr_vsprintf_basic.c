/* TC
 * id:    pfr_vsprintf_basic
 * area:  stdio_printf
 * funcs: vsprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.13
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int w(char *b, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = vsprintf(b, f, ap); va_end(ap); return r; }

TC_CASE(length) { char b[64]; volatile const char *f = "%s=%d"; int n = w(b, (const char *)f, "key", 42); CHECK_EQ_I(n, 6); CHECK_STR(b, "key=42"); }
TC_CASE(order) { char b[64]; volatile const char *f = "%d,%d,%d,%s"; int n = w(b, (const char *)f, 1, 2, 3, "end"); CHECK_EQ_I(n, 9); CHECK_STR(b, "1,2,3,end"); }
TC_CASE(empty) { char b[8]; volatile const char *f = ""; int n; b[0] = 'Q'; n = w(b, (const char *)f); CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0); }

int main(void) { tc_begin(); TC_RUN(length); TC_RUN(order); TC_RUN(empty); return tc_end(); }
