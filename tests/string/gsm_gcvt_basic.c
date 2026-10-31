/* TC
 * id:    gsm_gcvt_basic
 * area:  string
 * funcs: _gcvt
 * procs: _gcvt_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _gcvt, _gcvt_s (digits significant digits, trailing zeros suppressed, leading '-' for negatives)
 * note:  Integral values (trailing '.') and exponent digit count are probed in gsm_gcvt_exp_digits_probe.
 */
#include "tc.h"
#include <stdlib.h>
TC_DECL_PROC(int, _gcvt_s, (char *, size_t, double, int));
static volatile double v;

TC_CASE(trailing_zeros) { char b[32]; v = 1.5; _gcvt(v, 5, b); CHECK_STR(b, "1.5"); }
TC_CASE(rounding) { char b[32]; v = 123.456; _gcvt(v, 5, b); CHECK_STR(b, "123.46"); }
TC_CASE(negative) { char b[32]; v = -2.5; _gcvt(v, 3, b); CHECK_STR(b, "-2.5"); v = -123.456; _gcvt(v, 4, b); CHECK_STR(b, "-123.5"); }
TC_CASE(gcvt_s_pi)
{
    char b[32];
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    v = 3.14159; CHECK_EQ_I(tc_p__gcvt_s(b, sizeof b, v, 4), 0); CHECK_STR(b, "3.142");
}
TC_CASE(gcvt_s_size_ok)
{
    char b[32];
    if (!tc_p__gcvt_s) TC_SKIP("not exported");
    v = -2.5; CHECK_EQ_I(tc_p__gcvt_s(b, sizeof b, v, 3), 0); CHECK_STR(b, "-2.5");
    v = 0.0001; CHECK_EQ_I(tc_p__gcvt_s(b, sizeof b, v, 3), 0); CHECK_STR(b, "0.0001");
}
int main(void) { tc_begin(); TC_LOAD_PROC(_gcvt_s); TC_RUN(trailing_zeros); TC_RUN(rounding); TC_RUN(negative); TC_RUN(gcvt_s_pi); TC_RUN(gcvt_s_size_ok); return tc_end(); }
