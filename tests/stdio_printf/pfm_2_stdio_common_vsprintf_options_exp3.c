/* TC
 * id:    pfm_2_stdio_common_vsprintf_options_exp3
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.1 (e: at least two exponent digits); MS docs printf (UCRT: two-digit minimum exponent)
 * note:  options 0 only; the 0x10 three-digit option is probed in pfm_2_stdio_common_vsprintf_options_exp3_probe.
 *        1e100 needs three digits either way.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int sv(unsigned __int64 opt, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(opt, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile double v15 = 1.5, big = 1e100;

TC_CASE(opt0_big)     { char b[64]; int r = sv(0, b, 64, "%e", big); CHECK_STR(b, "1.000000e+100"); CHECK_EQ_I(r, 13); }
TC_CASE(opt0_small)   { char b[64]; int r = sv(0, b, 64, "%e", v15); CHECK_STR(b, "1.500000e+00"); CHECK_EQ_I(r, 12); }

int main(void)
{
    tc_begin();
     TC_RUN(opt0_big);  TC_RUN(opt0_small);  
    return tc_end();
}
