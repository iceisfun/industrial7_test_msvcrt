/* TC
 * id:    pfo_vsprintf_p_win7
 * area:  stdio_printf
 * funcs: _vsprintf_p
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _printf_p, _vsprintf_p (positional parameters)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int c_vsprintf_p(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vsprintf_p(b, n, f, ap); va_end(ap); return r; }

TC_CASE(reorder) { char b[32]; int r = c_vsprintf_p(b, 32, "%2$s %1$s", "b", "a"); CHECK_STR(b, "a b"); CHECK_EQ_I(r, 3); }
TC_CASE(repeat) { char b[32]; int r = c_vsprintf_p(b, 32, "%1$d/%1$d/%2$d", 4, 5); CHECK_STR(b, "4/4/5"); CHECK_EQ_I(r, 5); }
int main(void) { tc_begin(); TC_RUN(reorder); TC_RUN(repeat); return tc_end(); }
