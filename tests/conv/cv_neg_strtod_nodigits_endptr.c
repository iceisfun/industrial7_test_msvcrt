/* TC
 * id:    cv_neg_strtod_nodigits_endptr
 * area:  conv
 * funcs: atof
 * procs: strtod strtof
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.3 (no conversion: nptr stored in *endptr, zero returned); MS docs strtod, atof
 * note:  No-digit inputs. Incomplete exponents ("1e") are in cv_neg_strtod_incomplete_exp_probe. errno and the -0.0 question are in cv_neg_strtod_nodigits_errno_probe.
 */
#include "tc.h"
#include <stdlib.h>

TC_DECL_PROC(double, strtod, (const char *, char **));
TC_DECL_PROC(float, strtof, (const char *, char **));

static const char *volatile vs;


static void nodig(const char *s)
{
    char *e = (char *)1; double d; float f; char *e2 = (char *)1;
    vs = s; d = tc_p_strtod(vs, &e);
    CHECK(d == 0.0); CHECK_EQ_P(e, s);
    if (tc_p_strtof) { f = tc_p_strtof(vs, &e2); CHECK(f == 0.0f); CHECK_EQ_P(e2, s); }
}
TC_CASE(exported)
{
    CHECK(tc_p_strtod != NULL);
#if TC_CRT_VER >= 120
    CHECK(tc_p_strtof != NULL);
#endif
}
TC_CASE(empty_blank) { nodig(""); nodig("  "); nodig(" \t\n"); }
TC_CASE(sign_only) { nodig("+"); nodig("-"); nodig("+ 1"); }
TC_CASE(dot_only) { nodig("."); nodig("+."); nodig("-."); }
TC_CASE(exp_only) { nodig("e10"); nodig(".e1"); nodig("E5"); }
TC_CASE(letters) { nodig("abc"); nodig("x1"); }
TC_CASE(atof_none)
{
    vs = "abc"; CHECK_BITS_D(atof(vs), 0.0);
    vs = "";    CHECK_BITS_D(atof(vs), 0.0);
    vs = "  ";  CHECK_BITS_D(atof(vs), 0.0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(strtod); TC_LOAD_PROC(strtof);
    TC_RUN(exported); TC_RUN(empty_blank); TC_RUN(sign_only); TC_RUN(dot_only); TC_RUN(exp_only); TC_RUN(letters);
    TC_RUN(atof_none);
    return tc_end();
}
