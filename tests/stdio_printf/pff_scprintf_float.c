/* TC
 * id:    pff_scprintf_float
 * area:  stdio_printf
 * funcs: _scprintf _vscprintf sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _scprintf, _vscprintf
 * note:  MS docs _scprintf: returns the number of characters that would be written, excluding the terminating NUL
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static double dfb(unsigned long long u) { union { unsigned long long u; double d; } x; x.u = u; return x.d; }
static volatile double v_zero = 0.0, v_one = 1.0;
static char b[512];

static int vsc(const char *f, ...) { va_list ap; int n; va_start(ap, f); n = _vscprintf(f, ap); va_end(ap); return n; }
TC_CASE(prec2) { volatile double d = 3.14159; CHECK_EQ_I(_scprintf("%.2f", d), 4); }
TC_CASE(matches_sprintf) { volatile double d = 1.5; int n = sprintf(b, "%f", d); CHECK_EQ_I(_scprintf("%f", d), 8); CHECK_EQ_I(n, 8); }
TC_CASE(prec100) { volatile double d = 0.5; CHECK_EQ_I(_scprintf("%.100f", d), 102); }
TC_CASE(g_fmt) { volatile double d = 100000.0; CHECK_EQ_I(_scprintf("%g", d), 6); }
TC_CASE(vscprintf) { volatile double d = 1.5; CHECK_EQ_I(vsc("%.2f", d), 4); CHECK_EQ_I(vsc("%e", d), TC_AT_LEAST(140) ? 12 : 13); CHECK_EQ_I(vsc("%.100f", d), 102); }
int main(void) { tc_begin(); TC_RUN(prec2); TC_RUN(matches_sprintf); TC_RUN(prec100); TC_RUN(g_fmt); TC_RUN(vscprintf); return tc_end(); }
