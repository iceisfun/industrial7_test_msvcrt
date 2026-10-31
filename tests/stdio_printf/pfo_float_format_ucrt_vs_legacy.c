/* TC
 * id:    pfo_float_format_ucrt_vs_legacy
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf format specification; UCRT breaking changes (inf/nan printed as "inf"/"nan", not 1.#INF/1.#QNAN)
 * note:  Default options (0). Negative NaN "-nan" (payload-dependent), the indefinite NaN "-nan(ind)", %a, and zero-flag width on inf live in pfo_probe_float_special_ucrt.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int calls(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(0, b, n, f, NULL, ap); va_end(ap); return r; }
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(inf)
{
    char b[64]; volatile double p = mk(0x7FF0000000000000ULL), m = mk(0xFFF0000000000000ULL);
    calls(b, 64, "%f|%f", p, m); CHECK_STR(b, "inf|-inf");
    calls(b, 64, "%e|%g", p, m); CHECK_STR(b, "inf|-inf");
    calls(b, 64, "%E|%G", p, m); CHECK_STR(b, "INF|-INF");
}
TC_CASE(nan)
{
    char b[64]; volatile double p = mk(0x7FF8000000000001ULL);
    calls(b, 64, "%f", p); CHECK_STR(b, "nan");
    calls(b, 64, "%G", p); CHECK_STR(b, "NAN");
}
TC_CASE(zeros)
{
    char b[64]; volatile double z = 0.0, nz = mk(0x8000000000000000ULL);
    calls(b, 64, "%e|%e", z, nz); CHECK_STR(b, "0.000000e+00|-0.000000e+00");
    calls(b, 64, "%g|%g", z, nz); CHECK_STR(b, "0|-0");
    calls(b, 64, "%f|%.1f", z, nz); CHECK_STR(b, "0.000000|-0.0");
}
TC_CASE(inf_width)
{
    char b[64]; volatile double p = mk(0x7FF0000000000000ULL), m = mk(0xFFF0000000000000ULL);
    calls(b, 64, "[%5.1f][%-5.1f][%6.1f]", p, p, m); CHECK_STR(b, "[  inf][inf  ][  -inf]");
}
int main(void)
{
    tc_begin();
    TC_RUN(inf); TC_RUN(nan); TC_RUN(zeros); TC_RUN(inf_width);
    return tc_end();
}
