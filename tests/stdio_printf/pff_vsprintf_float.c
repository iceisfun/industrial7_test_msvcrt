/* TC
 * id:    pff_vsprintf_float
 * area:  stdio_printf
 * funcs: vsprintf _vscprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.8; MS docs vsprintf, _vscprintf
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int wrap(char *b, const char *f, ...) { int r; va_list ap; va_start(ap, f); r = vsprintf(b, f, ap); va_end(ap); return r; }
static int wrap_len(const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }

TC_CASE(basic) { char b[64]; volatile double a = 1.5, c = 2.345, d = 0.25; int n = wrap(b, "%f %.2f %g", a, c, d); CHECK_STR(b, "1.500000 2.35 0.25"); CHECK_EQ_I(n, 18); }
TC_CASE(float_promoted) { char b[64]; volatile float f = 0.5f; wrap(b, "%f|%g", f, f); CHECK_STR(b, "0.500000|0.5"); }
TC_CASE(mixed) { char b[64]; volatile double a = 2.5; volatile int i = 7; wrap(b, "%d:%.1f:%s:%5.2f", i, a, "x", a); CHECK_STR(b, "7:2.5:x: 2.50"); }
TC_CASE(vscprintf) { volatile double a = 1.5; CHECK_EQ_I(wrap_len("%f", a), 8); CHECK_EQ_I(wrap_len("%.3f|%g", a, a), 9); }
int main(void) { tc_begin(); TC_RUN(basic); TC_RUN(float_promoted); TC_RUN(mixed); TC_RUN(vscprintf); return tc_end(); }
