/* TC
 * id:    pfu_sprintf_s_special_floats
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 (inf/nan, %E uppercase); MS docs printf format specification (UCRT prints inf/nan in the C99 style)
 * note:  Indefinite / negative NaN spellings are probed in pfu_sprintf_s_nan_sign_probe. Legacy "1.#INF" forms are in pfu_sprintf_s_special_floats_old. The '0' flag with inf is in pfu_sprintf_s_inf_zero_flag_probe.
 */
#include "tc.h"
#include <stdio.h>

static volatile unsigned long long pinf = 0x7FF0000000000000ULL, ninf = 0xFFF0000000000000ULL, qnan = 0x7FF8000000000000ULL;
static double D(volatile unsigned long long *p) { union { unsigned long long u; double d; } x; x.u = *p; return x.d; }

TC_CASE(inf_f)  { char b[32]; sprintf_s(b, 32, "%f", D(&pinf)); CHECK_STR(b, "inf"); sprintf_s(b, 32, "%f", D(&ninf)); CHECK_STR(b, "-inf"); }
TC_CASE(inf_e_g) { char b[32]; sprintf_s(b, 32, "%e|%g", D(&pinf), D(&pinf)); CHECK_STR(b, "inf|inf"); }
TC_CASE(inf_upper) { char b[32]; sprintf_s(b, 32, "%E|%G|%F", D(&pinf), D(&ninf), D(&pinf)); CHECK_STR(b, "INF|-INF|INF"); }
TC_CASE(qnan_plain) { char b[32]; sprintf_s(b, 32, "%f|%E", D(&qnan), D(&qnan)); CHECK_STR(b, "nan|NAN"); }
TC_CASE(inf_width_plus) { char b[32]; sprintf_s(b, 32, "[%6f][%-6f][%+f]", D(&pinf), D(&pinf), D(&pinf)); CHECK_STR(b, "[   inf][inf   ][+inf]"); }

int main(void)
{
    tc_begin();
    TC_RUN(inf_f); TC_RUN(inf_e_g); TC_RUN(inf_upper); TC_RUN(qnan_plain); TC_RUN(inf_width_plus);
    return tc_end();
}
