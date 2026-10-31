/* TC
 * id:    pfo_vsprintf_p_basic
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf_p
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters (_printf_p, _sprintf_p)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int callp(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_p(o, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(reorder) { char b[64]; int r = callp(0, b, 64, "%2$s %1$s", "world", "hello"); CHECK_STR(b, "hello world"); CHECK_EQ_I(r, 11); }
TC_CASE(repeat) { char b[64]; int r = callp(0, b, 64, "%1$d,%1$d", 42); CHECK_STR(b, "42,42"); CHECK_EQ_I(r, 5); }
TC_CASE(percent) { char b[64]; int r = callp(0, b, 64, "%1$d%%|%2$s", 50, "ok"); CHECK_STR(b, "50%|ok"); CHECK_EQ_I(r, 6); }
TC_CASE(mixed_types) { char b[64]; callp(0, b, 64, "%3$s=%2$x/%1$c", 'q', 255, "k"); CHECK_STR(b, "k=ff/q"); }
int main(void) { tc_begin(); TC_RUN(reorder); TC_RUN(repeat); TC_RUN(percent); TC_RUN(mixed_types); return tc_end(); }
