/* TC
 * id:    cv_neg_strtod_wrong_forms
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.3: no valid subject sequence -> 0 returned, *endptr = nptr; parsing stops at the longest valid prefix
 */
#include "tc.h"

TC_DECL_PROC(double, strtod, (const char *, char **));
static const char *volatile vs;

static void noconv(const char *s)
{
    char *e = (char *)1; vs = s;
    CHECK_BITS_D(tc_p_strtod((const char *)vs, &e), 0.0);
    CHECK(e == s);
}
static void prefix(const char *s, double want, int used)
{
    char *e = (char *)1; vs = s;
    CHECK_BITS_D(tc_p_strtod((const char *)vs, &e), want);
    CHECK(e == s + used);
}
TC_CASE(no_digits)    { noconv("e5"); noconv("E5"); noconv("."); noconv("-"); noconv("+"); noconv("-."); noconv(""); noconv("   "); }
TC_CASE(double_sign)  { noconv("+-1"); noconv("--1"); noconv("++1"); noconv("-+1"); }
TC_CASE(stops_early)  { prefix("1__0", 1.0, 1); prefix("1.5.5", 1.5, 3); prefix("12abc", 12.0, 2); prefix("1 2", 1.0, 1); }
TC_CASE(hex_prefix_no_digits) { prefix("0x", 0.0, 1); prefix("0xg", 0.0, 1); }
TC_CASE(dot_forms_ok_but_partial) { prefix(".5.", 0.5, 2); prefix("5.", 5.0, 2); prefix("-.5x", -0.5, 3); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(strtod);
    if (!tc_p_strtod) { tc_write("TC FAIL setup\n"); return 1; }
    TC_RUN(no_digits); TC_RUN(double_sign); TC_RUN(stops_early); TC_RUN(hex_prefix_no_digits); TC_RUN(dot_forms_ok_but_partial);
    return tc_end();
}
