/* TC
 * id:    pfo_probe_float_special_ucrt
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   observed UCRT behaviour; none documented
 * note:  PROBE: (1) indefinite NaN 0xFFF8000000000000 prints "-nan(ind)"; (2) signaling NaN 0x7FF0000000000001 prints "nan(snan)";
 *        (3) %a of 0.0 / -0.0 with default precision yields "0x0.0000000000000p+0" / "-0x0.0000000000000p+0";
 *        (5) quiet NaN with sign set and non-zero payload 0xFFF8000000000001 prints "-nan" (only the zero-payload indefinite gets "(ind)");
 *        (6) "%+f"/"% f" of +inf give "+inf"/" inf";
 *        (4) the '0' flag does not zero-pad inf: "%06.1f" -> "   inf".
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int calls(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(0, b, n, f, NULL, ap); va_end(ap); return r; }
static double mk(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }

TC_CASE(indefinite) { char b[64]; volatile double v = mk(0xFFF8000000000000ULL); calls(b, 64, "%f", v); CHECK_STR(b, "-nan(ind)"); }
TC_CASE(neg_nan_payload) { char b[64]; volatile double v = mk(0xFFF8000000000001ULL); calls(b, 64, "%f", v); CHECK_STR(b, "-nan"); }
TC_CASE(snan)       { char b[64]; volatile double v = mk(0x7FF0000000000001ULL); calls(b, 64, "%f", v); CHECK_STR(b, "nan(snan)"); }
TC_CASE(hexfloat_zero)
{
    char b[64]; volatile double z = 0.0, nz = mk(0x8000000000000000ULL);
    calls(b, 64, "%a", z);  CHECK_STR(b, "0x0.0000000000000p+0");
    calls(b, 64, "%a", nz); CHECK_STR(b, "-0x0.0000000000000p+0");
}
TC_CASE(inf_sign_flags) { char b[64]; volatile double p = mk(0x7FF0000000000000ULL); calls(b, 64, "[%+f][% f]", p, p); CHECK_STR(b, "[+inf][ inf]"); }
TC_CASE(inf_zero_flag) { char b[64]; volatile double p = mk(0x7FF0000000000000ULL); calls(b, 64, "%06.1f", p); CHECK_STR(b, "   inf"); }
int main(void)
{
    tc_begin();
    TC_RUN(indefinite); TC_RUN(neg_nan_payload); TC_RUN(snan); TC_RUN(hexfloat_zero); TC_RUN(inf_sign_flags); TC_RUN(inf_zero_flag);
    return tc_end();
}
