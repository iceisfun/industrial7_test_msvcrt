/* TC
 * id:    cv_neg_strtod_inf_nan_malformed_ucrt
 * area:  conv
 * funcs: strtod wcstod strtof
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.3 (subject sequence: INF or INFINITY, NAN or NAN(n-char-seq)); MS docs strtod
 * note:  Sign of a zero result for no-conversion ("-i") is not asserted (see cv_neg_strtod_nodigits_errno_probe); "infi" forms are in
 *        cv_neg_strtod_inf_nan_partial_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <math.h>

static const char *volatile vs;
static const wchar_t *volatile vw;

TC_CASE(not_inf_nan)
{
    static const char *const t[] = { "in", "nn", "na", "i", "n", "-i", "+na" };
    unsigned k;
    for (k = 0; k < sizeof t / sizeof t[0]; k++) {
        char *e = NULL; double d; vs = t[k]; d = strtod(vs, &e);
        CHECK(d == 0.0); CHECK_EQ_P(e, vs);
    }
}
TC_CASE(float_not_inf) { char *e; float f; vs = "na"; f = strtof(vs, &e); CHECK_BITS_F(f, 0.0f); CHECK_EQ_P(e, vs); }
TC_CASE(wide_not_inf)
{
    wchar_t *e; double d;
    vw = L"in"; d = wcstod(vw, &e); CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, vw);
    vw = L"nn"; d = wcstod(vw, &e); CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, vw);
}
TC_CASE(nan_ok) { char *e; double d; vs = "nan"; d = strtod(vs, &e); CHECK_IS_NAN(d); CHECK_EQ_P(e, vs + 3); vs = "-nan"; d = strtod(vs, &e); CHECK_IS_NAN(d); CHECK_EQ_P(e, vs + 4); }
int main(void) { tc_begin(); TC_RUN(not_inf_nan); TC_RUN(float_not_inf); TC_RUN(wide_not_inf); TC_RUN(nan_ok); return tc_end(); }
