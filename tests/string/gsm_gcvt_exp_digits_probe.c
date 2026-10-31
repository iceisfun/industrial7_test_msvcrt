/* TC
 * id:    gsm_gcvt_exp_digits_probe
 * area:  string
 * funcs: _gcvt
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _gcvt; printf exponent width history (_set_output_format, 3-digit exponent before VS2015)
 * note:  Probes (1) exponent digit count: ucrtbase "1.23e+06", older CRTs "1.23e+006"; (2) _gcvt on integral values keeps a trailing
 *        decimal point ("100." for 100.0,3) per recollection of MS behaviour; (3) _gcvt(0.0,5) is "0" or "0.".
 */
#include "tc.h"
#include <stdlib.h>
static volatile double v;
TC_CASE(exp_digits)
{
    char b[32];
    v = 1234567.0; _gcvt(v, 3, b);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "1.23e+06");
#else
    CHECK_STR(b, "1.23e+006");
#endif
    v = 1e10; _gcvt(v, 5, b);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "1e+10");
#else
    CHECK_STR(b, "1e+010");
#endif
}
TC_CASE(integral_trailing_dot) { char b[32]; v = 100.0; _gcvt(v, 3, b); CHECK_STR(b, "100."); }
TC_CASE(zero) { char b[32]; v = 0.0; _gcvt(v, 5, b); CHECK(tc_strequ(b, "0") || tc_strequ(b, "0.")); }
int main(void) { tc_begin(); TC_RUN(exp_digits); TC_RUN(integral_trailing_dot); TC_RUN(zero); return tc_end(); }
