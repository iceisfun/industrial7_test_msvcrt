/* TC
 * id:    gsm_gcvt_exp_digits_probe
 * area:  string
 * funcs: _gcvt
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _gcvt; printf exponent width history (_set_output_format, 3-digit exponent before VS2015)
 * note:  Probes (1) exponent digit count: ucrtbase "1.23e+06", older CRTs "1.23e+006"; (2) _gcvt on integral values keeps a trailing
 *        decimal point ("100." for 100.0,3) per recollection of MS behaviour; (3) _gcvt(0.0,5) is "0" or "0.".
 *        Win10 22H2 19045 (observed): ucrtbase _gcvt keeps the legacy 3-digit exponent ("1.23e+006"), writes "1.e+010" (dot kept before the exponent) and "100" for
 *        100.0 with 3 digits (no trailing dot). Version-independent here.
 *        [unverified] on other CRTs: only ucrtbase 19045 was observed; the expectation is gated with TC_AT_LEAST(140) and the original expectation is kept for older CRTs. (1.23e+006 is observed on ucrtbase and equals the original expectation for the old CRTs, so it is ungated.)
 */
#include "tc.h"
#include <stdlib.h>
static volatile double v;
TC_CASE(exp_digits)
{
    char b[32];
    v = 1234567.0; _gcvt(v, 3, b);
    CHECK_STR(b, "1.23e+006");   /* observed on ucrtbase 19045: 3-digit exponent as in the old CRTs */
    v = 1e10; _gcvt(v, 5, b);

#if TC_AT_LEAST(140)
    CHECK_STR(b, "1.e+010");     /* observed: trailing '.' kept in scientific form */
#else
    CHECK_STR(b, "1e+010");
#endif

}
TC_CASE(integral_trailing_dot)
{
    char b[32]; v = 100.0; _gcvt(v, 3, b);
#if TC_AT_LEAST(140)
    CHECK_STR(b, "100");   /* observed: no trailing dot */
#else
    CHECK_STR(b, "100.");
#endif
}
TC_CASE(zero) { char b[32]; v = 0.0; _gcvt(v, 5, b); CHECK(tc_strequ(b, "0") || tc_strequ(b, "0.")); }
int main(void) { tc_begin(); TC_RUN(exp_digits); TC_RUN(integral_trailing_dot); TC_RUN(zero); return tc_end(); }
