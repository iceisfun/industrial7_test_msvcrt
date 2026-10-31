/* TC
 * id:    cv_strtod_rounding_halfway
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p0
 * conf:  standard
 * note:  Only inputs of <=17 significant digits; longer halfway inputs are in cv_strtod_halfway_long_ucrt.
 * ref:   C11 7.22.1.3 / 5.2.4.2.2: correctly rounded, ties to even in the default rounding mode; IEEE 754
 */
#include "tc.h"

TC_DECL_PROC(double, strtod, (const char *, char **));
static volatile const char *vs;
static double cv(const char *s) { vs = s; return tc_p_strtod((const char *)vs, NULL); }

TC_CASE(integers_2_53)
{
    CHECK_BITS_D(cv("9007199254740993"), 9007199254740992.0);
    CHECK_BITS_D(cv("9007199254740995"), 9007199254740996.0);
    CHECK_BITS_D(cv("-9007199254740993"), -9007199254740992.0);
}
TC_CASE(fraction_2_52)
{
    CHECK_BITS_D(cv("4503599627370497.5"), 4503599627370498.0);
    CHECK_BITS_D(cv("4503599627370496.5"), 4503599627370496.0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(strtod);
    if (!tc_p_strtod) { tc_write("TC FAIL setup\n"); return 1; }
    TC_RUN(integers_2_53); TC_RUN(fraction_2_52);
    return tc_end();
}
