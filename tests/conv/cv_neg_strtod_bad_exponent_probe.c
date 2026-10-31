/* TC
 * id:    cv_neg_strtod_bad_exponent_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.3 (subject sequence is the longest initial prefix of the expected form)
 * note:  Probes that an 'e'/'E' with no exponent digits is NOT consumed ("1e" -> 1.0, endptr after '1'). Standard-required, but
 *        pre-UCRT scanners (msvcrt.dll, msvcr80-100) are not known to back up correctly, so isolated here.
 */
#include "tc.h"

TC_DECL_PROC(double, strtod, (const char *, char **));
static const char *volatile vs;
static void prefix(const char *s, double want, int used)
{
    char *e = (char *)1; vs = s;
    CHECK_BITS_D(tc_p_strtod((const char *)vs, &e), want);
    CHECK(e == s + used);
}
TC_CASE(e_no_digits) { prefix("1e", 1.0, 1); prefix("2.5E", 2.5, 3); }
TC_CASE(e_sign_no_digits) { prefix("1e+", 1.0, 1); prefix("1e-x", 1.0, 1); }
int main(void)
{
    tc_begin(); TC_LOAD_PROC(strtod);
    if (!tc_p_strtod) { tc_write("TC FAIL setup\n"); return 1; }
    TC_RUN(e_no_digits); TC_RUN(e_sign_no_digits);
    return tc_end();
}
