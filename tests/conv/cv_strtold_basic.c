/* TC
 * id:    cv_strtold_basic
 * area:  conv
 * funcs:
 * procs: strtold strtod
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strtold ("long double is the same as double"); C11 7.22.1.3
 * note:  On the MSVC ABI long double is 64-bit; on x64 it is returned in xmm0 exactly like double, so the proc is typed as returning double.
 */
#include "tc.h"
#include <errno.h>

typedef double (__cdecl *strtold_t)(const char *, char **);
static strtold_t p_strtold, p_strtod;

TC_CASE(simple) { const char *volatile s = "1.5"; if (!p_strtold) TC_SKIP("no strtold"); CHECK_BITS_D(p_strtold(s, NULL), 1.5); }
TC_CASE(equals_strtod)
{
    const char *volatile s = "0.1";
    if (!p_strtold || !p_strtod) TC_SKIP("missing proc");
    CHECK_BITS_D(p_strtold(s, NULL), p_strtod(s, NULL));
    CHECK_EQ_U(tc_dbits(p_strtold(s, NULL)), 0x3FB999999999999AULL);
}
TC_CASE(overflow)
{
    const char *volatile s = "1e309"; double d;
    if (!p_strtold) TC_SKIP("no strtold");
    errno = 0; d = p_strtold(s, NULL);
    CHECK_EQ_U(tc_dbits(d), 0x7FF0000000000000ULL); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(endptr)
{
    const char *volatile s = "2.5e1rest"; char *e = NULL;
    if (!p_strtold) TC_SKIP("no strtold");
    CHECK_BITS_D(p_strtold(s, &e), 25.0); CHECK_EQ_P(e, s + 5);
}
#ifdef TC_UCRT
TC_CASE(hex)
{
    const char *volatile s = "0x1p0"; char *e = NULL;
    if (!p_strtold) TC_SKIP("no strtold");
    CHECK_BITS_D(p_strtold(s, &e), 1.0); CHECK_EQ_P(e, s + 5);
}
#endif

int main(void)
{
    tc_begin();
    p_strtold = (strtold_t)tc_crt_proc("strtold"); p_strtod = (strtold_t)tc_crt_proc("strtod");
    TC_RUN(simple); TC_RUN(equals_strtod); TC_RUN(overflow); TC_RUN(endptr);
#ifdef TC_UCRT
    TC_RUN(hex);
#endif
    return tc_end();
}
