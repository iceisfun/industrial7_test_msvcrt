/* TC
 * id:    pfo_probe_vsprintf_p_star_width
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf_p
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf_p positional parameters
 * note:  PROBE: a positional width argument "%1$*2$d" (value 1, width taken from argument 2) is accepted by the _p
 *        family and right-justifies to width 4.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int callp(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_p(o, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(star_width) { char b[64]; int r = callp(0, b, 64, "[%1$*2$d]", 7, 4); CHECK_STR(b, "[   7]"); CHECK_EQ_I(r, 6); }
int main(void) { tc_begin(); TC_RUN(star_width); return tc_end(); }
